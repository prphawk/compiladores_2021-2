#!/bin/bash

SUCCESS=0
executable="./etapa6"
evaluation=""

# gcc -S small.c

# gcc -c small.s
# gcc small.o -o small

# echo "\n------------- STARTING TEST CASES -------------\n"
# for file in ../testes_E6/*
#     do
#     "$executable" < "$file" > "saidas_E6/saida_$file.s"
#     gcc -c "saidas_E6/saida_$file.s" -o "saidas_E6/programa_$file"

#     result=$?
#     evaluation=$(head -1 "$file")
    
#     print="FROM:${file} EXPECTED RESULT:${evaluation} RESULT:${result}"
#     echo "$print\n"
# done

echo "\n------------- STARTING ACTUAL GCC COMPILING -------------\n"
for file in ../testes_E6/* 
    do
    gcc -S "$file" > "$file.S"

    
    echo "$print\n"
done
