triangular:
mv a 0
loop:
add a b
cmp b 1
dec b
jg loop
gb
_start:
mv b 100
jb triangular
prn a
hlt 0
