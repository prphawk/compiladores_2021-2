#!/bin/bash

SUCCESS=0
executable="./etapa4"
evaluation=""
echo "STARTING OFFICIAL TEST CASES"

for file in ../testes_E3_format/*
    do
    #valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
  
done