factorial:
mv a 1
loop:
mul a b
cmp b 1
dec b
jg loop
gb
_start:
mv b 19
jb factorial
prn a
hlt 0
