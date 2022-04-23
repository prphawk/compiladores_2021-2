#!/bin/bash

SUCCESS=0
executable="./etapa6"
evaluation=""
echo "\nSTARTING GRAMMAR TEST CASES\n"

#checar gramatica
for file in ../testes_E2/*
    do
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
  
done