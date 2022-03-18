#!/bin/bash

SUCCESS=0
executable="./etapa3"

echo "STARTING PROF'S TEST CASES"

for file in ../etapa2/E2/*
do
    "$executable" < "$file"
    result=$?
    if [ $result != $SUCCESS ]
        then echo "TEST FAILED!! FILE $file\n"
    fi
  
done