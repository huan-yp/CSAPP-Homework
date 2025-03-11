# Test of push, reverse, and pop
option fail 0
option malloc 0
new
push 2
push 1
push 3
reverse
push 5
push 1
push 3
reverse
push 7
reverse
push 7
push 8
reverse
pop 7
pop 3
pop 1
pop 2
pop 5
pop 1
pop 3
pop 7
pop 8