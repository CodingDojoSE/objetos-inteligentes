#!/bin/sh


# Import the helper functions.
. ./files/__helper_functions.sh

# The new Trait.h file to changes the compiler scheduler
NEW_SCHEDULER_FILE="$PWD_COMPILE_EPOS_LAMP/files/traits.h"
NEW_EPOS_COMPILER_FILE="$PWD_COMPILE_EPOS_LAMP/files/eposcc"
NEW_EPOS_MAKEDEFS_FILE="$PWD_COMPILE_EPOS_LAMP/files/makedefs"
NEW_EPOS_PLASMA_MAKEFILE_FILE="$PWD_COMPILE_EPOS_LAMP/files/makefile_plasma"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1
secondCommandLineArgument=$2

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)

# Notify when $EPOS environment variable is not found.
if ! [ -f $EPOS/tools/makefile ]
then
    printf "\nINSTALL ERROR!\nCould not find the 'EPOS' environment variable or it is invalid.\n"
    printf "The start directory is '$PWD_COMPILE_EPOS_LAMP'\n"
    printf "The current directory is '$EPOS'\n"
    printHelp
    exit 1
fi


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
     || [ $# -eq 0 ]
then
    printf "\nCOPY ERROR!\nCould not find '$programFileToCompile'\n"
    printf "The start directory is '$PWD_COMPILE_EPOS_LAMP'\n"
    printf "The current directory is '$EPOS'\n"
    printHelp
    exit 1
fi


# To install the new scheduler.
if cp $NEW_SCHEDULER_FILE $EPOS/include/
then
    printf "The copy of 'NEW_SCHEDULER_FILE' to '$EPOS/include' was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy '$NEW_SCHEDULER_FILE' to '$EPOS/include'\n"
    printHelp
    exit 1
fi


# To install the new compiler.
if cp $NEW_EPOS_COMPILER_FILE $EPOS/tools/eposcc/
then
    printf "The copy of 'NEW_EPOS_COMPILER_FILE' to '$EPOS/tools/eposcc/' was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy '$NEW_EPOS_COMPILER_FILE' to '$EPOS/include'\n"
    printHelp
    exit 1
fi


# To install the new makedefs file.
if cp $NEW_EPOS_MAKEDEFS_FILE $EPOS/
then
    printf "The copy of 'NEW_EPOS_MAKEDEFS_FILE' to '$EPOS/' was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy '$NEW_EPOS_MAKEDEFS_FILE' to '$EPOS/'\n"
    printHelp
    exit 1
fi


# To install the new compiler.
if cp $NEW_EPOS_PLASMA_MAKEFILE_FILE $EPOS/tools/plasma/makefile
then
    printf "The copy of 'NEW_EPOS_PLASMA_MAKEFILE_FILE' to '$EPOS/tools/plasma/makefile' was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy '$NEW_EPOS_PLASMA_MAKEFILE_FILE' to '$EPOS/plasma/makefile'\n"
    printHelp
    exit 1
fi


# To copy the program to the main/compilation directory.
if cp $programFileToCompile $EPOS/app/
then
    # -updm | For overwrite destination content.
    # --quiet | Do not print the number of blocks copied
    find . -name '*.cc' | cpio -updm --quiet $EPOS/app/
    find . -name '*.h' | cpio -updm --quiet $EPOS/include/
    printf "The copy of '$programFileToCompile' and it dependents files to '$EPOS/app' was successful\n"
else
    printf "\nCOPY ERROR!\nCould not to copy '$programFileToCompile' to '$EPOS/app'\n"
    printHelp
    exit 1
fi


# Print help to the output stream, when the second parameter is not an integer.
tryPrintHelp $secondCommandLineArgument

# Exits the program using a successful exit status code.
exit 0


