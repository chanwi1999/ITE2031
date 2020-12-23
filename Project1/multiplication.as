	lw 0 2 mcand
	lw 0 3 mplier
    lw 0 5 tmp
    lw 0 6 mbit
loop nor 3 5 4
     beq 0 4 res
     beq 0 0 next
res add 1 2 1
next lw 0 4 neg
	add 6 4 6
    beq 0 6 done
    add 2 2 2
    add 5 5 5
	lw 0 4 one
    add 5 4 5
    beq 0 0 loop
done halt
mcand .fill 32766
mplier .fill 10383
tmp .fill -2
mbit .fill 15
neg .fill -1 
one .fill 1
