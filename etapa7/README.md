Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)

Simulador:

> ./etapa5 < ../testes_E5_incorretos/ijk25 > saida
> python3 ilocsim.py -s saida

Em etapa5, para compilar as files:

> make

> ./etapa5 < teste.txt

Checar mem leaks com teste.txt **(instalar o valgrind)**:

> make checkleak

Debugar **(instalar o gdb)**:

> make debug
> run < teste.txt

(outros comandos: next, print (x), **where**)

Limpar:

> make clear

Entregar:

> tar cvzf etapa5.tgz .

OU

> make submit

Unzip:

> tar -xvzf etapa5.tgz
