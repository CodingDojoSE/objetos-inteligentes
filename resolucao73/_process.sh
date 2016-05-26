#!/bin/sh


# Saves the current opened path, to restore it when this scripts finish.
PWD_COMPILE_EPOS_LAMP=$(dirname $(readlink -f $0))

#import the helper functions.
. ./__helper_functions.sh


# The EPOSMotes2 installer
EPOS_MOTES2_INSTALLER="red-bsl.py"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
     || [ $# -eq 0 ]
then
    printf "\nPROCESS ERROR!\nCould not find $PWD_COMPILE_EPOS_LAMP/$programFileToCompile\n"
    printHelp
    exit 1
fi


# Switch to the main/compilation directory.
cd $EPOS

# To prepare the binary file to be exported the the EPOSMotes2 board.
if arm-objcopy -I elf32-little -O binary img/$programNameToCompile.img img/$programNameToCompile.bin
then
    printf "The arm-objcop $programFileToCompile was successful\n"
else
    printf "\nPROCESS ERROR!\nCould not to copy $programFileToCompile to $EPOS/app\n"
    printHelp
    exit 1
fi


# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP

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


