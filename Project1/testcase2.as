	lw 0 1 input
	lw 0 2 idx
	add 0 0 3
	lw 0 4 neg
	lw 0 5 adr
	jalr 5 6
	sw 0 3 100
	halt
loop add 3 1 3
	add 2 4 2
	beq 0 2 done
	beq 0 0 loop
done jalr 6 7
adr .fill loop
idx .fill 10
input .fill 3
neg .fill -1
