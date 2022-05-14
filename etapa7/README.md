Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)

Em etapa7, para compilar as files:

> make

> ./etapa7 < teste.txt

Checar mem leaks com teste.txt **(instalar o valgrind)**:

> make checkleak

Debugar **(instalar o gdb)**:

> make debug
> run < teste.txt

(outros comandos: next, print (x), **where**)

Limpar:

> make clear

Entregar:

> tar cvzf etapa7.tgz .

OU

> make submit

Unzip:

> tar -xvzf etapa7.tgz
