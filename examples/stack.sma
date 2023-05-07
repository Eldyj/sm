sadd:
pop a
pop b
add a b
psh a
gb
ssub:
pop a
pop b
sub a b
psh a
gb
smul:
pop a
pop b
mul a b
psh a
gb
_start:
psh 10
psh 30
jb sadd
pop a
prn a
psh 10
psh 30
jb ssub
pop a
prn a
psh 10
psh 30
jb smul
pop a
prn a
hlt 0
