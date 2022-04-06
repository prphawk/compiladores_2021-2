Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)

Em etapa3, para compilar as files:

> make

> ./etapa3 < teste.txt

Checar leaks de memória com teste.txt **(instalar o valgrind)**:

> make checkleak

Debugar **(instalar o gdb)**:

> make debug
> run < teste.txt

(outros comandos: next, print (x), **where**)

Executar casos de testes (converter `\eof` para UNIX):

> sh teste.sh

Limpar:

> make clean

Entregar:

> tar cvzf etapa3.tgz .
> OU
> make submit

Unzip:

> tar -xvzf etapa3.tgz
