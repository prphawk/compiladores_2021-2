#!/bin/bash

SUCCESS=0
executable="./etapa5"

echo "------------- STARTING TEST CASES -------------\n"
for file in ../testes_E5/*
    do
    "$executable" < "$file" > "saidas/testes_E5/$file.txt"
    result=$?
    
    print="FROM:${file}"
    echo "$print\n"
done

for file in ../testes_laura_E5/*
    do
    "$executable" < "$file" > "saidas/testes_laura_E5/$file.txt"
    result=$?
    
    print="FROM:${file}"
    echo "$print\n"
done
  
