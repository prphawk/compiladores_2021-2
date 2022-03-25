#!/bin/bash

SUCCESS=0
executable="etapa2/etapa2"

for file in teste_right/*
do
    echo "TEST FILE $file"
    "$executable" < "$file"
    result=$?
    if [ $result -eq $SUCCESS ]
    then
        echo "SUCCESS!"
    else
        echo "TEST FAILED!"
    fi
  
done

#echo "STARTING WRONG TESTS"

# for file in teste_wrong/*
# do
#     echo "TEST FILE $file"
#     cat "$file" | make exec --silent
#     result=$?
#     if [ $result -eq $SUCCESS ]
#     then
#         echo "SUCCESS!"
#     else
#         echo "TEST FAILED!"
#     fi
  
# done

