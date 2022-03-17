# Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)
etapa3: lex.yy.o parser.tab.o main.o ast.o
	gcc -g -o etapa3 lex.yy.o parser.tab.o main.o ast.o -lfl

lex.yy.o parser.tab.o main.o ast.o: lex.yy.c parser.tab.c ast.c main.c 
	gcc -g -c lex.yy.c main.c ast.c parser.tab.c 

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y --report-file=FILE

clean:
	rm lex.yy.* parser.tab.* main.o etapa3

submit:
	make clean
	tar cvzf etapa3.tgz .

debug:
	gdb ./etapa3

checkleak: valgrind --leak-check=full --show-leak-kinds=all ./etapa3 < teste.txt