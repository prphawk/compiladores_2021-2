SUCCESS=0

for file in ../teste_right/*
do
    echo "TEST FILE $file"
    cat "$file" | make exec --silent
    result=$?
    if [ $result -eq $SUCCESS ]
    then
        echo "SUCCESS!"
    else
        echo "TEST FAILED!"
    fi
  
done