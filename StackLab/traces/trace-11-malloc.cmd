# Test of malloc failure on push
option fail 50
option malloc 0
new
push 4 20
option malloc 25
push 3 20



