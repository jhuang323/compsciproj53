#!/bin/bash

NETID="huangjk2"
HWNUM=4
DIRTOTAR="hw4"

#run make clean first
cd $DIRTOTAR

make clean

cd ..

#tar the file
tar -cvf "hw${HWNUM}_${NETID}.tar" $DIRTOTAR