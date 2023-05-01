swp_ab:
swp a b
gb
_start:
mv a 20
mv b 10
shl a 2
shr b 2
jb swp_ab
prn a
prn b
hlt 0
