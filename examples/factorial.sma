factorial:
mv a 1
cmp b 0
jle end
loop:
mul a b
cmp b 1
sub b 1
jg loop
ja end
_start:
mv b 19
ja factorial
end:
prn a
