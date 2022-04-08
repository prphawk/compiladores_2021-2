# Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)
etapa5: lex.yy.o parser.tab.o main.o ast.o valor_lexico.o tabela_simbolos.o errors.o mem.o iloc.o iloc_codigo.o
	gcc -g -o etapa5 lex.yy.o parser.tab.o main.o ast.o valor_lexico.o tabela_simbolos.o errors.o mem.o iloc.o iloc_codigo.o -lfl

lex.yy.o parser.tab.o main.o ast.o tabela_simbolos.o errors.o mem.o iloc.o: lex.yy.c parser.tab.c ast.c main.c valor_lexico.c tabela_simbolos.c errors.c mem.c iloc.c iloc_codigo.c
	gcc -g -c *.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y --report-file=FILE

clean:
	rm lex.yy.* parser.tab.* main.o ast.o valor_lexico.o tabela_simbolos.o errors.o mem.o iloc.o iloc_codigo.o etapa5

submit:
	tar cvzf etapa5.tgz .

debug:
	gdb ./etapa5

checkleak: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./etapa5 < teste.txt