#!/bin/bash

NETID="huangjk2"
HWNUM=3
DIRTOTAR="hw3"

#run make clean first
cd $DIRTOTAR

make clean

cd ..

#tar the file
tar -cvf "hw${HWNUM}_${NETID}.tar" $DIRTOTAR