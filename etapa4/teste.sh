#!/bin/bash

SUCCESS=0
executable="./etapa4"
evaluation=""


# o que NAO deve dar erro

# echo "------------- STARTING CORRECT TEST CASES -------------\n"
# for file in ../testes_E4/right/*
#     do
#     "$executable" < "$file"
#     result=$?
    
#     print="FROM:${file}"
#     echo "$print\n"
# done
  

# o que deve dar erro

echo "\n------------- STARTING ERROR TEST CASES -------------\n"
for file in ../testes_E4/*
    do
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
done
