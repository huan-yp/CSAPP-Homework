# Test of push, pop, reverse, and size
option fail 0
option malloc 0
new
push 2
push 1
push 3
reverse
size
push 5
push 1
push 3
size
pop 3
reverse
size
pop 3
pop 1
pop 2
pop 5
pop 1
size
free
