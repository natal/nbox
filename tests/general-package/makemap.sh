#! /bin/bash

if test $# -lt 2; then
    echo "Usage: ./domap [number of inputs] [number neurons in layer 1]
          .. [number of neurons in layer n] [number of outputs]" >& 2
    echo "There should be at least inputs and outputs, n >= 0" >& 2
    exit 1
fi

ITERATOR=$#
NEXT=0
NAME="INPUT"
NEXT="$(( $1 ))"

while test $ITERATOR -gt 1; do
    LINKS=""
    NEURON_START=$NEXT
    NEURON_END=$(( $NEXT + $1 ))
    NEXT="$(( $NEXT + 1))"
    TO="$(( $NEXT + $2))"
    while test $NEXT -lt $TO; do
        LINKS="$LINKS"" $NEXT"
        NEXT=$(( $NEXT + 1 ))
    done

    ITERATOR=$(( $ITERATOR - 1))

    while test $NEURON_START -lt $NEURON_END; do
        echo "${NAME}${LINKS}"
        NEURON_START=$(( $NEURON_START + 1))
    done

    NAME="NEURON"

    echo "${NAME}${LINKS}"

    shift
done

#OUTPUT neurons

NEURON_START=$NEXT
NEURON_END=$(( $NEXT + $1 ))

while test $NEURON_START -lt $NEURON_END; do
    echo "NEURON"
    NEURON_START=$(($NEURON_START + 1))
done

