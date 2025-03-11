# Test of push, and size
option fail 0
option malloc 0
new
push 3
push 1
push 2
size
push 5
push 1
push 3
size
pop 
pop
pop
pop
size

