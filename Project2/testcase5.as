	lw 0 1 one		// r1 = 1
	lw 0 2 two		// r2 = 2
	add 1 2 1		// r1 = r1 + r2, data hazard
	add 1 2 2		// r2 = r1 + r2
	add 1 2 3		// r3 = r1 + r2
	halt
one	.fill 1
two	.fill 2
