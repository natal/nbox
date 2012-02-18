#! /bin/bash
OLDNAME='Copyright (C) 1993, 1994, 1995, 1997, 1999, 2000, 2001'
NEWNAME='Copyright (C) 2011'


for f in $@; do
   sed -e "s/$OLDNAME/$NEWNAME/g" $f > tmp
   cat tmp > $f
   rm tmp
done
