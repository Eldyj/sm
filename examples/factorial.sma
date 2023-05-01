factorial:
mv a 1
loop:
mul a b
cmp b 1
sub b 1
jg loop
gb
_start:
mv b 19
jb factorial
prn a
