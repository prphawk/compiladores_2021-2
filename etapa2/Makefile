# Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)
etapa2: lex.yy.c parser.tab.o
	gcc -o etapa2 lex.yy.o parser.tab.o -lfl

lex.yy.o parser.tab.o: lex.yy.c parser.tab.c
	gcc -c lex.yy.c parser.tab.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y --report-file=FILE

clean:
	rm lex.yy.* parser.tab.* etapa2