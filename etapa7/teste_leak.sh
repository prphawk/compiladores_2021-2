#!/bin/bash

SUCCESS=0
executable="./etapa7"
evaluation=""
echo "\nSTARTING MEM LEAK TEST CASES\n"

for file in ../testes_E7/*
    do
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "$executable" < "$file"    
    print="FROM:${file}"
    echo "$print\n"
done