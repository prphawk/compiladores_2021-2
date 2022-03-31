#!/bin/bash

# o programa retorna 1 em caso de erro
SUCCESS=0
executable="./etapa1"

echo "STARTING RIGHT TESTS"

for file in teste_right/*
do
    "$executable" < "$file"
    result=$?
    if [ $result != $SUCCESS ]
        then echo "TEST FAILED!! FILE $file\n"
    fi
  
done

# a correção é formada pelos casos que vc deveria aceitar e por aqueles que NÃO deveria. ao executar os casos de testes errados,
# lembre-se que É PRA TER um aviso de erro sintático printado na tela pra cada um deles, e a razão. abaixo ele ta comentado pra
# evitar poluição na hora de testar os corretos.

# echo "STARTING WRONG TESTS"
 
# for file in teste_wrong/*
# do
#     "$executable" < "$file" --silent
#     result=$?
#     if [ $result -eq $SUCCESS ]
#         then echo "TEST FAILED!! FILE $file\n"
#     fi
  
# done