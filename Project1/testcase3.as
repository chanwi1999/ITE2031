	lw 0 1 one
	lw 0 2 two
	add 0 1 3 tmp=one
	add 0 2 1 one=two
	add 0 3 2 two=tmp
done halt
one .fill 1
two .fill 2
