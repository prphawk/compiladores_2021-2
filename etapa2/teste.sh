#!/bin/bash

SUCCESS=0
executable="./etapa2"
evaluation=""

for file in E2/*
    do
    "$executable" < "$file"
    result=$?
    evaluation=$(head -1 "$file")
    
    print="FROM:${file} RESULT:${evaluation}"
    echo "$print\n"
  
done

# echo "STARTING WRONG TESTS"

# for file in teste_wrong/*
# do
#     "$executable" < "$file" --silent
#     result=$?
#     if [ $result -eq $SUCCESS ]
#         then echo "TEST FAILED!! FILE $file\n"
#     fi
  
# done