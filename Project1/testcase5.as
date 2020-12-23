	lw 0 1 input
	lw 0 2 pos
	lw 2 5 pos
	nor 1 0 3
	nor 1 1 4
	add 2 3 3
	add 2 4 4
	sw 5 3 0
	sw 5 4 1
done halt
input .fill 20
pos .fill 1
