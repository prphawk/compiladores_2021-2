#!/bin/bash

SUCCESS=0
executable="./etapa3"
evaluation=""
echo "STARTING OFFICIAL TEST CASES"

for file in ../etapa2/E2/*
    do
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
  
done