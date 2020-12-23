	lw 0 2 mul
	lw 0 3 times
	lw 0 4 neg
loop beq 0 3 done
	add 1 2 1
	add 3 4 3
	beq 0 0 loop
done halt
mul .fill 1234
times .fill 10
neg .fill -1
