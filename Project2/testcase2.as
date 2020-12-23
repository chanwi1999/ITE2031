	lw 0 2 mcand		// r2 = 32766
	lw 0 3 mplier		// r3 = 10383
    lw 0 5 tmp			// r5 = -2
    lw 0 6 mbit			// r6 = 15
loop nor 3 5 4			// r4 = r3 nor r5
     beq 0 4 res		// 0 != r4
     beq 0 0 next		// check the MEM for branching without doing anything
	 noop
	 noop
	 noop
res add 1 2 1
next lw 0 4 neg
	noop				// to prevent data hazard for r4
	add 6 4 6
    beq 0 6 done		// 0 != r6
    add 2 2 2
    add 5 5 5
	lw 0 4 one
	noop				// to prevent data hazard for r4
    add 5 4 5
    beq 0 0 loop		// check the MEM for bracnching without doing anything
	noop
	noop
	noop
done halt
mcand .fill 32766
mplier .fill 10383
tmp .fill -2
mbit .fill 15
neg .fill -1 
one .fill 1
