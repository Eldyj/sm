triangular:
mv a 0
cmp b 0
jle end
loop:
add a b
cmp b 1
sub b 1
jg loop
ja end
_start:
mv b 10000000
ja triangular
end:
prn a
