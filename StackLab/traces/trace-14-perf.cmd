# Test performance of push, size, and reverse
option fail 0
option malloc 0
new
push 2 1000000
push 3 1000000
size 1000
reverse
reverse
size 1000

