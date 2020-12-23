	lw 0 1 one			// r1 = 1
	lw 0 2 two			// r2 = 2
	noop
	noop
	noop
	add 1 2 3
loop beq 0 0 res		// check the branch when you arrive at the mem
	add 1 1 1			// flush
	add 2 2 2			// flush
	add 1 2 3			// flush
res add 1 2 4			// I expect "r1 = 1, r2 = 2, r3 = r4 = 3"
	halt
one .fill 1
two .fill 2
