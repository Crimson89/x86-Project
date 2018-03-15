#!/bin/bash

echo Enter file folder

read var1

echo Enter file name with no file extensions

read var2

./macro11 $var1/$var2.mac -o $var2.obj -l $var2.lst
./obj2ascii $var2.obj $var2.ascii

mv $var2.* $var1/

touch $var1/$var2.txt
