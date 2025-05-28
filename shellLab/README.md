
## Shell Lab

### 阻塞信号

- 如果全局变量在信号处理函数中被访问或修改，同时主线程或其他线程也在访问或修改它，就可能会出现竞态条件。

### 细节

- 内置命令不需要支持后台模式
- 调试的时候要主动刷新缓冲区才有输出。
- 只能调用一次 waitpid 获取状态
- Ctrl+Z 默认行为是向前台进程组里所有进程发送 SIGTSTP 信号，前台任务一般得创建新进程组。


################
CS:APP Shell Lab
################

Files:

Makefile	# Compiles your shell program and runs the tests
README		# This file
tsh.c		# The shell program that you will write and hand in
tshref		# The reference shell binary.

# The remaining files are used to test your shell
sdriver.pl	# The trace-driven shell driver
trace*.txt	# The 15 trace files that control the shell driver
tshref.out 	# Example output of the reference shell on all 15 traces

# Little C programs that are called by the trace files
myspin.c	# Takes argument <n> and spins for <n> seconds
mysplit.c	# Forks a child that spins for <n> seconds
mystop.c        # Spins for <n> seconds and sends SIGTSTP to itself
myint.c         # Spins for <n> seconds and sends SIGINT to itself

