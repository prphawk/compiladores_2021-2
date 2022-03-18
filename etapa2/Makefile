# Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)
etapa2: lex.yy.c parser.tab.o main.o
	gcc -o etapa2 lex.yy.o parser.tab.o main.o -lfl

lex.yy.o parser.tab.o main.o: lex.yy.c parser.tab.c main.c
	gcc -c lex.yy.c main.c parser.tab.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d -t parser.y --report-file=FILE

clean:
	rm lex.yy.* parser.tab.* main.o etapa2