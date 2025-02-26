# CSAPP-Homework

仅供参考, 请勿抄袭.

## Env Prepare

### WSL

我用的 WSL.

```
wsl --install
```

直接安装即可, 安装后找到 "Windows" 功能, 打开 "适用于 Linux 的 Windows 子系统", "虚拟机平台" 即可.

重启后 `wsl` 进入 Linux 环境.

### 物理机

计算机楼 237 CPC 俱乐部有一台 Linux 物理机, 密码 `123456`, 可以用.

### Docker

安装 docker, 拉一个 ubuntu 镜像, 但是只有基础环境, 需要自己安装绝大部分等工具.

### VMware

VMware 可以用 jht 发的安装包安装, 如果联网没出问题最好, 出问题了自求多福.

## DataLab

### BitXor

不难, 离散数学讲了, 逐位分析构造即可.

7ops

### isTMax

就是判断是不是 2147483647, 就是最高位是 0, 其它位全是 1.

有点难想, 可以分正负数讨论.

9ops

### allOddBits

构造掩码即可.

判等可以用亦或和逻辑非.

7ops

### negate

经典补码问题, 补码取负数即可.

2ops

### isAsciiDigit

构造掩码判断, 线性构造.

11ops

### conditional

构造条件掩码, 迭代式构造.

15ops

### isLessOrEqual

数学技巧, 判断正负.

5ops

### logicalNeg

所有位合并到第一位, 迭代合并.

12ops

### howManyBits

分块, 用右移技巧控制最高位, 类似数位 DP 卡高位统计

61ops

### floatScale2

ez

10ops

### floatFloat2Int

ez

14ops

### floatPower2

ez

9ops