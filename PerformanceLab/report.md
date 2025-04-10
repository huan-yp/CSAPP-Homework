## PerfLab

极端性能优化

### pinwheel

本机平均优化比约 6.0。

![image-20250410084820231](https://raw.githubusercontent.com/huan-yp/image_space/master/img/202504100848268.png)

#### 访存优化

本来是访问内存是分散的，可以 32x32 分块，块先转，块内再转，全部卡进 L1，减少 cache miss。

二级访存优化我选的是 128x128 分块，这部分能卡进 L2，但是效果不是很明显。

单写一个访存已经到 3.0 了，超过 1.5 的标准很多了。

#### ILP

做 4x4 的循环展开，利用好 cacheline 的存取特性。

现代 CPU 的 OOO 执行能力很强，设计好依赖链短的操作，基本上能吃满单核里面的执行单元。

理论上 2x8 应该更好（不浪费 cacheline），但我没写了。

加上 ILP 之后能到 4.0 的优化比。

#### 强制对齐

pixel 的内存是没对齐的，6 位数据需要两次寄存器操作来处理。

使用 long long 指针强制读取 pixel 内存，忽略末两位的危险存取操作，再强制写回 dst。

对于 corner 部分只能使用 pixel 指针，防止写到外面出事。

通过合理设计写的顺序，能够抵消掉强制对齐带来的副作用。

这块直接是时间砍掉了 1/3，平均可到 5.5 左右。

#### 多线程

没有禁止多线程，对于 512 和 1024 的可以做一次 2x2 的 blocking，然后开多线程跑。其余更小的线程开销显著大于收益，所以不开。

### motion

本机平均优化比为 8.8。

这个应该允许误差的，不允许误差优化空间太小了，毕竟除法太麻烦了。

如果允许 1% 的误差，应该能做到 20.0 以上的优化比。

#### 除法优化

把除以不同数的操作分开写，写成除以一个常数，编译器会自动用上一些除法优化。

#### ILP

循环展开，这里没有用 blocking，所以循环展开是 1x4 的。

#### 中间量优化

本来要算 9 次加法，但可以先算竖着的 3 次记到中间变量里。

对于中间变量再算横着的 3 次，最后再算一次加法，只需要做 7 次。

### 允许误差的 motion

由于实际上不允许，这里只做理论讨论。

#### 除法优化

数据直接拿的 Intel CPU 手册的数据，ARM 架构不适用。

$\frac{1}{9} \approx \frac{1}{16} + \frac{1}{32} + \frac{1}{64} + \frac{1}{512}$。

所以 `x / 9 = (x >> 4) + (x >> 5) + (x >> 6) + (x >> 9)`。

这个运算的依赖链是 3, 每个指令 latency 是 0.5，所以总 latency 是 1.5。

除法的 latency 是 20，定除数的优化除法 latency 大概也有 4-5，差距非常明显。

这个指令吞吐量瓶颈在于四次位运算，其倒数大约是 $0.66$ (我的 CPU 位运算吞吐量倒数是 $0.167$)。

#### 仿 SIMD

由于不准内嵌汇编，直接 SIMD 操作是不行的，但是可以用以下方式来模拟 SIMD。

通过位掩码和位运算，又可以用 long long 指针来强制存取 pixel 数据，这个时候对 long long 的操作全部是各个段上的位运算或者加减操作，

于是可以用一个 long long 当作一个 pixel 的三个值来做批量操作，构造位掩码来屏蔽高位 bit 对低位的干扰。

下面是除法的代码：

```cpp
void multi_thread_motion(int dim, pixel *src, pixel *dst)
{
    pixel *mid = (pixel *)malloc(sizeof(pixel) * dim * dim);
    const long long mask1 = 0x0fff0fff0fffll;
    const long long mask2 = 0x07ff07ff07ffll;
    const long long mask3 = 0x03ff03ff03ffll;
    const long long mask4 = 0x007f007f007fll;
    const long long mask = 0xffffffffffff;
    for(int i=0;i<dim;i++)
    for(int j=0;j<dim;j++){
        long long val = *((long long*)(src + RIDX(i, j, dim))); 
        val = ((val >> 4) & mask1) + ((val >> 5) & mask2) + ((val >> 6) & mask3) + ((val >> 9) & mask4); 
        *(mid + RIDX(i, j, dim)) = *((pixel*)&val);
    }
}
```

多了一次位掩码计算和内存操作，latency 变成 2.5，吞吐量倒数不变还是 0.66。

#### ILP

根据上面的 throughput 和 latency 计算，做 1x4 的循环展开能得到比较好的效果。

#### 理论优化比计算

内存由于访存连续可以忽略延迟，其吞吐量显然也不会成为瓶颈。

还需要过一次中间量计算和一次中间量累加，两次操作严格依赖，不能做循环融合。吞吐量瓶颈是加法，其倒数均为 $0.75$。

总吞吐量倒数为 $0.66 + 0.75 + 0.75 = 2.16$，也就是 CPE 是 $2.16$，计算理论优化比为 $19.9$。





