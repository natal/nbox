#! /bin/bash

if test $# -lt 3; then
    echo 'Usage: ./domap [number of inputs] [number of hidden neurons] [number of outputs]'
    exit 1
fi

NBHD=$2
NBIN=$1
NBOUT=$3

LINK=""

I=$(($NBIN + 1))
echo $I
TO=$(($I + $NBHD))
echo $TO

LINK=$I
I=$(($I + 1))

while test $I -lt $TO; do
    LINK="$LINK"" ""$I"
    I=$(($I + 1))
done

rm -rf letter_map.nm
touch letter_map.nm

I=$NBIN
while test $I -gt 0; do
   echo 'INPUT '"$LINK" >> letter_map.nm
   I=$(($I - 1))
done

echo "NEURON $LINK" >> letter_map.nm


I=$(($NBHD + $NBIN + 2))
TO=$(($I + $NBOUT))

LINK=$I
I=$(($I + 1))

while test $I -lt $TO; do
    LINK="$LINK"" ""$I"
    I=$(($I + 1))
done

I=$NBHD
while test $I -gt 0; do
   echo 'NEURON '"$LINK" >> letter_map.nm
   I=$(($I - 1))
done

echo "NEURON $LINK" >> letter_map.nm

while test $NBOUT -gt 0; do
   echo 'NEURON' >> letter_map.nm
   NBOUT=$(($NBOUT - 1))
done
