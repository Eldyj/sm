fibanacci:
mv c 1
mv a 0
fibanacci_loop:
add c a
swp a c
dec b
cmp b 0
jg fibanacci_loop
gb
_start:
mv b 10
jb fibanacci
prn a
hlt 0
