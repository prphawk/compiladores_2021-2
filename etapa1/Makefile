# Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)
etapa1.o: lex.yy.c
	gcc lex.yy.c -o etapa1

lex.yy.c: scanner.l
	flex scanner.l

clean: 
	rm lex.yy.c etapa1