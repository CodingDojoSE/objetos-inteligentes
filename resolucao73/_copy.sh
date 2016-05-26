#!/bin/sh


# Saves the current opened path, to restore it when this scripts finish.
PWD_COMPILE_EPOS_LAMP=$(dirname $(readlink -f $0))

#import the helper functions.
. ./__helper_functions.sh


# The new Trait.h file to changes the compiler scheduler
NEW_SCHEDULER_FILE="traits.h"
NEW_EPOS_COMPILER_FILE="eposcc"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)

# Notify when $EPOS environment variable is not found.
if ! [ -f $EPOS/tools/makefile ]
then
    printf "\nINSTALL ERROR!\nCould not find the \$EPOS\n"
    printf "The start directory is '$PWD_COMPILE_EPOS_LAMP'\n"
    printf "The current directory is $EPOS\n"
    printHelp
    exit 1
fi


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
     || [ $# -eq 0 ]
then
    printf "\nCOPY ERROR!\nCould not find '$PWD_COMPILE_EPOS_LAMP/$programFileToCompile'\n"
    printf "The start directory is '$PWD_COMPILE_EPOS_LAMP'\n"
    printf "The current directory is $EPOS\n"
    printHelp
    exit 1
fi


# To install the new scheduler.
if cp $PWD_COMPILE_EPOS_LAMP/$NEW_SCHEDULER_FILE $EPOS/include/
then
    printf "The copy of '$NEW_SCHEDULER_FILE' to '$EPOS/include' was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy $NEW_SCHEDULER_FILE to $EPOS/include\n"
    printHelp
    exit 1
fi


# To install the new compiler.
if cp $PWD_COMPILE_EPOS_LAMP/$NEW_EPOS_COMPILER_FILE $EPOS/tools/eposcc/
then
    printf "The copy of '$NEW_EPOS_COMPILER_FILE' to $EPOS/tools/eposcc/ was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy $NEW_EPOS_COMPILER_FILE to $EPOS/include\n"
    printHelp
    exit 1
fi


# To copy the program to the main/compilation directory.
if cp $PWD_COMPILE_EPOS_LAMP/$programFileToCompile $EPOS/app/
then
    # -updm | For overwrite destination content.
    # --quiet | Do not print the number of blocks copied
    find . -name '*.cc' | cpio -updm --quiet $EPOS/app/
    find . -name '*.h' | cpio -updm --quiet $EPOS/include/
    printf "The copy of '$programFileToCompile' to '$EPOS/app' was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy '$programFileToCompile' to '$EPOS/app'\n"
    printHelp
    exit 1
fi


# Calculates whether the seconds program parameter is an integer number
isInteger $2

# Captures the return value of the previous function call command
isIntegerReturnValue=$?

# Print help when it is not passed a second command line argument integer
if ! [ $isIntegerReturnValue -eq 1 ]
then
    printHelp
fi


# Exits the program using a successful exit status code.
exit 0


