#!/bin/bash

SUCCESS=0
executable="./etapa5"

echo "------------- STARTING TEST CASES -------------\n"
for file in ../testes_E5/*
    do
    "$executable" < "$file" > "saida"
    python3 ilocsim.py -s saida

    result=$?
    
    print="FROM:${file}"
    echo "$print\n"
done
