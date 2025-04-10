## 总结

最开始做错了一遍题, 这个 ver 是你车的作业.

## 各个函数实现

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

分块, 用右移技巧控制最高位, 类似数位 DP 卡高位统计.

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