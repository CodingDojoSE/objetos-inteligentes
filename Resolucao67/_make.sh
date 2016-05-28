#!/bin/sh


# Import the helper functions.
. ./files/__helper_functions.sh

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1
secondCommandLineArgument=$2

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
     || [ $# -eq 0 ]
then
    printf "\nMAKE ERROR:\n Could not find '$PWD_COMPILE_EPOS_LAMP/$programFileToCompile'\n"
    printHelp
    exit 1
fi

# Switch to the main/compilation directory.
cd $EPOS


# Calculates whether the seconds program parameter contains the clean word
contains $secondCommandLineArgument "clean"

# Captures the return value of the previous function call command
containsReturnValue=$?

# To clear any last compilation data.
if ! [ $containsReturnValue -eq 1 ]
then
    make veryclean all
    cd $PWD_COMPILE_EPOS_LAMP
    sh _copy.sh structuredLEDControl.cc 0
    cd $EPOS
else
    make
fi


# To compile the application passed as parameter '$programNameToCompile'
if ! make APPLICATION=$programNameToCompile
then
    printf "\nMAKE ERROR:\nThe start directory is '$PWD_COMPILE_EPOS_LAMP'\n"
    printf "The current directory is '$EPOS'\n"
    printHelp
    exit 1
fi


# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP

# Exits the program using a successful exit status code.
exit 0


