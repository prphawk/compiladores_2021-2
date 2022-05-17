###Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)

-   Em etapa5, para compilar as files:

```
make
./etapa5 < teste.txt
```

-   Usar o simulador ILOC:

```
./etapa5 < teste > saida
python3 ilocsim.py -s saida
```

-   Checar mem leaks com teste.txt **(instalar o valgrind)**:

`make checkleak`

-   Debugar **(instalar o gdb)**:

```
make debug
run < teste.txt
```

(outros comandos: next, print (x), **where**)

-   Executar casos de testes (converter \eof para UNIX):

`sh teste.sh`

-   Executar casos de testes avaliando leaks de memória (converter \eof para UNIX):

`sh teste_leak.sh`

-   Limpar:

`make clear`

-   Entregar:

`tar cvzf etapa5.tgz`

OU

`make submit`

-   Unzip:

`tar -xvzf -f etapa5.tgz`
