#!/bin/bash

total = 0
count = 0

while [ $count -le 100 ]
do 
       let total=total+count
       let count++

done

echo "result: $count"