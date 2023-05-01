triangular:
mv a 0
loop:
add a b
cmp b 1
sub b 1
jg loop
gb
_start:
mv b 1000000
jb triangular
prn a
