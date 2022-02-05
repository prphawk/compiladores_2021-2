etapa1: main.c
	gcc lex.yy.c -o etapa1

lex.yy.c: scanner.l
	flex scanner.l

clean: 
	rm *.o lex.yy.c etapa1