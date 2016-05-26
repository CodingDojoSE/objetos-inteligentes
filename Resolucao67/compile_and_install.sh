#!/bin/sh

# Saves the current opened path, to restore it when this scripts finish.
PWD_COMPILE_EPOS_LAMP=$(dirname $(readlink -f $0))

#import the helper functions.
. ./__helper_functions.sh


# Print help to the output stream.
printHelp()
{
    printf "\nCOMPILE AND INSTALL HELP:\n"
    printf "The './compile_and_install.sh' parameters are:\n"
    printf "The FIRST parameter is the file name to install.\n"
    printf "The SECOND parameter is the USB port number to install.\n"
    printf "The THIRD parameter is the optional value clean for a 'make veryclean' all.\n"
    printf "Examples:\n"
    printf "./compile_and_install.sh PROGRAM_NAME.cc 0 clean\n"
    printf "./compile_and_install.sh PROGRAM_NAME.cc 1\n"
}


# The default file name to compile when no arguments are provided
defaultFileToCompile="structuredLEDControl.cc"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1
usbPortNumberToInstall=$2
isVeryCleanCompilation=$3


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ]
then
    printf "\nERROR! Could not to find your program $programFileToCompile!\n"
    printf "Please try again providing an correct program. Example:\n"
    printf "./compile.sh PROGRAM_NAME.cc\n"
    printHelp
    exit 1
fi


# Notify an invalid file passed as parameter.
if [ $# -eq 0 ]
then
    printHelp
    exit 1
else
    if sh _copy.sh $programFileToCompile 0
    then
        if sh _make.sh $programFileToCompile $isVeryCleanCompilation a
        then
            if sh _process.sh $programFileToCompile 0
            then
                if sh _install.sh $programFileToCompile $usbPortNumberToInstall
                then
                    printf "\nThe COMPILE_AND_INSTALL.SH was executed successfully!\n"
                else
                    printf "\n_INSTALL.SH ERROR! Could not to install the program into the EPOSMotes2!\n"
                    printHelp
                    exit 1
                fi
            else
                printf "\n_PROCESS.SH ERROR! Could not to process the objcopy!\n"
                printHelp
                exit 1
            fi
        else
            printf "\n_MAKE.SH ERROR! Could not to compile the program!\n"
            printHelp
            exit 1
        fi
    else
        printf "\n_COPY.SH ERROR! Could not to copy the initial files!\n"
        printHelp
        exit 1
    fi
fi


# Exits the program using a successful exit status code.
exit 0





