CC = gcc
CFLAGS = -O0 -g -Wall -Werror

all: stest
	-tar -cf handin.tar stack.c stack.h

stack.o: stack.c stack.h harness.h
	$(CC) $(CFLAGS) -c stack.c 

stest: stest.c report.c console.c harness.c stack.o
	$(CC) $(CFLAGS) -o stest stest.c report.c console.c harness.c stack.o
	tar cf handin.tar stack.c stack.h

test: stest driver.py
	chmod +x driver.py
	python3 driver.py

clean:
	rm -f *.o *~ stest 
	rm -rf *.dSYM
	(cd traces; rm -f *~)

