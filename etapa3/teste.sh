#!/bin/bash

SUCCESS=0
executable="./etapa3"
evaluation=""
echo "STARTING OFFICIAL TEST CASES"

#checa pra ver se não quebrou a gramatica
for file in ../testes_E2/*
    do
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
  
done