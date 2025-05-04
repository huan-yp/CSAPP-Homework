
## Shell Lab

### 阻塞信号

- 如果全局变量在信号处理函数中被访问或修改，同时主线程或其他线程也在访问或修改它，就可能会出现竞态条件。

### 细节

- 内置命令没有后台模式
- 调试的时候要主动刷新缓冲区才有输出。
- 只能调用一次 waitpid 获取状态
- SIGSTOP 不能向进程组发送
- 要创建了进程组才能正常发进程组的 kill，
- 好像必须要创建进程组才能正常运行


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

