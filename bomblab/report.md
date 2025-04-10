## 总结

做了 5 个, flags 如下:

```
Border relations with Canada have never been better.
1 2 4 8 16 32
0 207
0 0
9?>567
```

压缩包里的 phase 文件夹包含了对应的汇编代码, 汇编代码中包含解析汇编时打的注释, 下面是流程汇报和附录.

## Bombs:

### 准备

先 objdump 可执行文件即可获取汇编代码.

### Bomb1:

从 main 开始简单阅读一下, 找到 phase_1, 定位 strings_not_equal, 猜测 `esi` 的初值就是字符串起始, `x/s` 输出即可得到 flag

`Border relations with Canada have never been better.`

### Bomb2:

分析汇编的符号表, 有一个 read_six_numbers 的调用, 猜测功能是读六个整数.

read_six_numbers 中只有读到的数不足才会爆, 直接转到 read_six_numbers 调用之后.

有点看不懂后面的逻辑, 大概是乘二之类的, 打印出 rbx 的值看了一下直接是按我输入的顺序排布的数, 猜测是按顺序乘二.

还有一个 `cmpl $%rsp 0x1`，开头得是 1.

输入 `1 2 4 8 16 32` 即可得到 flag.

### Bomb3:

前面是读入，从 `cmp $0x1,%eax` 看出来是读两个整数。

第一个读到了 `0x8(%rsp)`, 第二个读到了 `0xc(%rsp)`。

发现要跳到 `*0x402470(,%rax,8)`, `%rax` 就是 `0x8(%rsp)`, 手看一下具体 `*0x402470(,%rax,8)` 的值, 跳到了下面的代码.

第二个值和对应部分的十六进制数匹配上就行.

可行的两个解:

```
0 207
2 707
```

### Bomb4:

老套路，换了个 `jxx` 写法, 还是读两个数, 读到 `0x8(%rsp)` 和 `0xc(%rsp)`. `401033` 的 `jbe` 必须要走，不然就炸，所以 `0x8(%rsp)` 必须小于小于等于 14.

0xc(%rsp) 读到的数没有传进 `func4` 里, 看一下 `phase_4` 后面的代码发现它必须是 0, 第二个数确定下来.

后面是 `func4` 的调用判断, `eax` 必须是 0, 也就是 `func4` 返回值是 0.

倒退出去的路有两条, 一条是 `ff0` 的 `jmp 007`, 这里需要满足 `fe9` 递归调用的 eax 是 0. 或者走 `ff9` 的 `jge 007`, 需要满足 `ecx>=n1`, 这里 ecx 按规范一般是被调用者保存的, 所以不会变, 就是原来的那个中间变量 `eax + esi`.

eax 初值 `14`, esi 初值 `0`, 分析一下前面几步在 `fe9` 的递归调用, eax 变化是 `14->6->2->0`, 恰好能在 `n1` 完全不干预的时候变成 0, 后面令 `n1=0` 也可以正常走 `ff9` 的 `jge`, 所以 `n1=0`. 似乎就拆完了.

一组可行解是 `0 0`.

### Bomb5:

没有递归, 看上去挺简单的.

`fs:28` 有一个神秘的大数字, 好像是随机的还是有什么规律, 不知道.

call 了一个 `string_length`, 字符串长度必须是 6. 后面有个循环, 功能是从每个输入字符的低 4 位拿下标, 然后从 `0x4024b0 + 下标` 的位置拿数值(还是取低 8 位).

后面有个 `strings_not_equal` 的调用, 是从 `0x40245e` 拿字符串, 看一下, flag 是 `flyers`. 查一下 `0x4024b0` 发现是 `maduiersnfotvbyl`, 下标组合是 `9 15 14 5 6 7`, 从 0 的 acsii 48 开始点就行.

一组可行解是 `9?>567`.

## Bomb6:

先读 6 个整数.

耗时太长了, 放弃了.

## 常用 GDB 命令

我只用了下面的命令.

- `gdb bomb`: 调试可执行文件 bomb.
- `b <func_name>`: 函数入口处打断点.
- `b *<address>`: 汇编指令地址打断点.
- `r`: 运行程序, 暂停在第一个断点处.
- `si`: 单指令, 会进入函数体.
- `s`: 单行执行, 有点不可控.
- `n`: 单行跳过, 不进函数, 不可控程度更高.
- `p $eax`: 打印寄存器 eax 的值.
- `info r`: 显示所有寄存器的值.
- `x/s <address>`: 打印内存地址为 `<address>` 的内容, 以字符串形式输出.
- `x/d <address>`: 打印内存地址为 `<address>` 的内容, 以十进制输出.

## 汇编指令

看上去是 AT&T 语法.

### 定义

`src`: 源操作数, 执行过程中不被修改
`dst`: 目标操作数, 执行过程中可能被修改

### 取内存

- `[<base>]([<offset>][,<index>][,<scale>])`: 表示一个内存地址, `<base>`, `<offset>`, `<index>` 缺省时默认为 0, `<scale>` 缺省时默认 1. 内存计算公式为 `<base> + <offset> + <index> * <scale>`. 这个式子称为内存表达式

- `*<expr>`: 找到 `<expr>` 所代表的地址, 读取这个地址的值作为寻址结果.

### ADD && SUB

- `ADD src, dst`
- `SUB src, dst`

支持的操作数类型: `reg`, `mem`, `imm`:

- `reg` + `mem`: `add <expr>, %eax  # %eax = %eax + *<expr>`
- `reg` + `reg`: `add %ebx, %eax  # %eax = %eax + %ebx`
- `mem` + `reg`: `add <expr>, %eax  # %eax = %eax + *<expr>`
- `imm` + `mem`: `add $0x1, <expr>  # *<expr> = *<expr> + 1`
- `imm` + `reg`: `add $0x1, %eax  # %eax = %eax + 1`


*`<expr>` 表示一个内存地址, 会取到这个地址的值, 然后和后面的操作符相加.*

*`$0x1` 等表示立即数, `0x1` 表示一个内存地址.*

### LEA

`LEA src, dst`: 将作为内存表达式的 `src` 计算出来, 放入 `dst` 中. 

### MOV

`mov src, dst`: 自动选择大小移动.
`movw src, dst`: 16bit 移动
`movb src, dst`: 8bit 移动
`movzbl src, dst`: 8bit 移动到 32bit 寄存器, 0 拓展.
`movsbl src, dst`: 8bit 移动到 8bit 寄存器, 符号拓展.

### 寄存器符号

`rax`: 64bit 寄存器
`eax`: 32bit 寄存器
`ax`: 16bit 寄存器
`al`: 8bit 寄存器 

