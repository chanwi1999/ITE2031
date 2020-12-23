	lw 0 1 one			// r1 = 1
	lw 0 2 two			// r2 = 2
	beq 1 2 done
	add 1 1 1
	add 2 2 2
done halt				// I expect "r1 = 2, r2 = 4"
one .fill 1
two .fill 2 
