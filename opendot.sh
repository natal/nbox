#! /bin/bash

FILE=$(echo $1 | sed 's/\(.*\)\..*/\1/')
echo "Creating "$FILE".png"
FILE=$FILE".png"
touch $FILE
dot -Tpng -o$FILE $1
echo "Opening "$FILE
gnome-open $FILE &
