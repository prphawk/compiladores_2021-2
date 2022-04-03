#!/bin/bash

SUCCESS=0
executable="./etapa4"
evaluation=""

echo "STARTING ERROR TEST CASES"

# o que nao deve dar erro
for file in ../testes_E4/right/*
    do
    "$executable" < "$file"
    result=$?
    
    print="FROM:${file}"
    echo "$print\n"
  
done

# echo "\nSTARTING CORRECT TEST CASES\n"

# # o que deve dar erro
# for file in ../testes_E4/*
#     do
#     "$executable" < "$file"
#     result=$?
#     evaluation=$(head -1 "$file")
    
#     print="FROM:${file} RESULT:${evaluation}"
#     echo "$print\n"
  
#done
