Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)

Em etapa3, para compilar as files:

> make

> ./etapa4 < teste.txt

Checar mem leaks com teste.txt:

(instalar o valgrind)

> make checkleak

Debugar:

(instalar o gdb)

> make debug
> run < teste.txt

(outros comandos: next, print (x), where)

Limpar:

> make clear

Entregar:

> tar cvzf etapa4.tgz .
> OU
> make submit

Unzip:

> tar -xvzf -f etapa4.tgz
