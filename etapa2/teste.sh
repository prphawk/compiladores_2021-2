#!/bin/bash

SUCCESS=0
executable="./etapa2"
evaluation=""

# para checagem de erros de sintaxe. os casos de testes tem comentários na primeira linha dizendo o resultado correto de sua avaliação.
# nesse script um caso de teste executado deve printar "//CORRECT" e nada mais, ou então "//INCORRECT" e o erro de sintaxe que seu programa identificou.
for file in ../testes_E2/*
    do
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
  
done