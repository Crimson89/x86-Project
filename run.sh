#!/bin/bash

echo Enter file folder in testing

read var1

echo Enter file name without extension

read var2

./RunMe.exe -f testing/$var1/$var2.ascii -m verbose -t testing/$var1/$var2.txt
