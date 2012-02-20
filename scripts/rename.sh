#! /bin/bash
OLDNAME='Copyright (C) 2011'
NEWNAME='Copyright (C) 2011-2012'


for f in $@; do
   sed -e "s/$OLDNAME/$NEWNAME/g" $f > tmp
   sed -e "s/basis_fun/activ_fun/g" $f > tmp
   cat tmp > $f
   rm tmp
done
