Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)

Em etapa6, para compilar as files:

> make

> ./etapa6 < teste.txt

Checar mem leaks com teste.txt **(instalar o valgrind)**:

> make checkleak

Debugar **(instalar o gdb)**:

> make debug
> run < teste.txt

(outros comandos: next, print (x), **where**)

Limpar:

> make clear

Entregar:

> tar cvzf etapa6.tgz .

OU

> make submit

Unzip:

> tar -xvzf etapa6.tgz
