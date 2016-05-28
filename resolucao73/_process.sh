#!/bin/sh


# Import the helper functions.
. ./installer_files/__helper_functions.sh

# The EPOSMotes2 installer
EPOS_MOTES2_INSTALLER="red-bsl.py"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1
secondCommandLineArgument=$2

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
     || [ $# -eq 0 ]
then
    printf "\nPROCESS ERROR!\nCould not find '$PWD_COMPILE_EPOS_LAMP/$programFileToCompile'\n"
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
    printf "\nPROCESS ERROR!\nCould not to copy '$programFileToCompile' to '$EPOS/app'\n"
    printHelp
    exit 1
fi


# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP

# Print help to the output stream, when the second parameter is not an integer.
tryPrintHelp $secondCommandLineArgument

# Exits the program using a successful exit status code.
exit 0


