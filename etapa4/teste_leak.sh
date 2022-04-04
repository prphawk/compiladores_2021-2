#!/bin/bash

SUCCESS=0
executable="./etapa4"
evaluation=""
echo "\nSTARTING MEM LEAK TEST CASES\n"

#checar memleaks
for file in ../etapa3/testes_E3_format/*
    do
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "$executable" < "$file"
    result=$?
    print="FROM:${file}"
    echo "$print\n"
done

for file in ../testes_E4/right/*
    do
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "$executable" < "$file"
    result=$?
    print="FROM:${file}"
    echo "$print\n"
done