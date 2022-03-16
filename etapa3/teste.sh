#!/bin/bash

SUCCESS=0
executable="./etapa3"

echo "STARTING RIGHT TESTS"

for file in teste_right/*
do
    "$executable" < "$file"
    result=$?
    if [ $result != $SUCCESS ]
        then echo "TEST FAILED!! FILE $file\n"
    fi
  
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

