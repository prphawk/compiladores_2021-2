#!/bin/bash

SUCCESS=0
executable="./etapa2"
evaluation=""

for file in ../testes_E2/*
    do
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
  
done

# run < ../testes_E2/asl151