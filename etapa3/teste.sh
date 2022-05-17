#!/bin/bash

SUCCESS=0
executable="./etapa3"
echo "STARTING OFFICIAL TEST CASES"

for file in ../testes_E3/entradas/*
    do
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "$executable" < "$file"
    result=$?
    
    print="FROM:${file}"
    echo "$print\n"
  
done