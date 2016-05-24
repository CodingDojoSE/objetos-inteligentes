#!/bin/sh


# Saves the current opened path, to restore it when this scripts finish.
installManual=$(cat "__installManual.txt")
PWD_COMPILE_EPOS_LAMP=$(pwd)


# Print help to the output stream.
printHelp()
{
    echo "$installManual"
}

# Determine whether the first parameter is an integer or not.
#
# Returns 1 if the specified string is an integer, otherwise returns 0.
isInteger()
{
    if [ "$1" -eq "$1" ] 2>/dev/null
    then
        return 1
    else
        return 0
    fi
}


# The new Trait.h file to changes the compiler scheduler
NEW_SCHEDULER_FILE="traits.h"
NEW_EPOS_COMPILER_FILE="eposcc"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
    || [ $# -eq 0 ]
then
    echo "\nERROR! Could not find '$PWD_COMPILE_EPOS_LAMP/$programFileToCompile'"
    echo "The start directory is '$PWD_COMPILE_EPOS_LAMP'"
    echo "The current directory is $EPOS"
    printHelp
    exit 1
fi


# To install the new scheduler.
if cp /home/user/EPOS/OpenEPOS-1.1RC__emoteII/app/objetos_inteligentes/resolucao68/$NEW_SCHEDULER_FILE $EPOS/include/
then
    echo "The copy of '$NEW_SCHEDULER_FILE' to '$EPOS/include' was successful"
else
    echo "ERROR! Could not to copy $NEW_SCHEDULER_FILE to $EPOS/include"
    printHelp
    exit 1
fi

# To install the new compiler.
if cp /home/user/EPOS/OpenEPOS-1.1RC__emoteII/app/objetos_inteligentes/resolucao68/$NEW_EPOS_COMPILER_FILE $EPOS/tools/eposcc/
then
    echo "The copy of '$NEW_EPOS_COMPILER_FILE' to $EPOS//tools/eposcc/ was successful"
else
    echo "ERROR! Could not to copy $NEW_SCHEDULER_FILE to $EPOS/include"
    printHelp
    exit 1
fi


# To copy the program to the main/compilation directory.
if cp /home/user/EPOS/OpenEPOS-1.1RC__emoteII/app/objetos_inteligentes/resolucao68/$programFileToCompile $EPOS/app/
then
    # -updm | For overwrite destination content.
    # --quiet | Do not print the number of blocks copied
    find . -name '*.cc' | cpio -updm --quiet $EPOS/app/
    find . -name '*.h' | cpio -updm --quiet $EPOS/include/
    echo "The copy of '$programFileToCompile' to '$EPOS/app' was successful"
else
    echo "ERROR! Could not to copy '$programFileToCompile' to '$EPOS/app'"
    printHelp
    exit 1
fi


# Print help when it is not passed a second command line argument integer
if isInteger $2
then
    printHelp
fi



