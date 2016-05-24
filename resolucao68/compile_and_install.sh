#!/bin/sh


# Saves the current opened path, to restore it when this scripts finish.
installManual=$(cat "__installManual.txt")
PWD_COMPILE_EPOS_LAMP=$(pwd)


# Print help to the output stream.
printHelp()
{
    echo "The start directory is $PWD_COMPILE_EPOS_LAMP"
    echo "The current directory is $EPOS"
    echo "The second parameter is the USB port number to install."
    echo "The third parameter is the optional value clean for a make veryclean all."
    echo "Examples:"
    echo "./compile_and_install.sh MY_COOL_PROGRAM_NAME_WITHOUT_HYPHEN.cc 0 clean"
    echo "./compile_and_install.sh MY_COOL_PROGRAM_NAME_WITHOUT_HYPHEN.cc 1"
    echo "$installManual"
}



# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ]
then
    echo "\nERROR! Could not to find your program $programFileToCompile!"
    echo "Please try again providing an correct program. Example:"
    echo "./compile.sh MY_COOL_PROGRAM_NAME_WITHOUT_HYPHEN.cc\n"
    printHelp
    exit 1
elif ! [ -f structuredLEDControl.cc ]
then
    echo "\nERROR! Could not to find your program 'structuredLEDControl.cc'!"
    echo "Please try again providing an correct program. Example:"
    echo "./compile.sh MY_COOL_PROGRAM_NAME_WITHOUT_HYPHEN.cc\n"
    printHelp
    exit 1
fi


# Notify an invalid file passed as parameter.
if [ $# -eq 0 ]
then
    cp $EPOS/app/INE5412_EposMotesII_SmartObjects/EPOS_MotesII_Lamps/structuredLEDControl.cc $EPOS/app
    cd $EPOS
    make veryclean all
    cd $PWD_COMPILE_EPOS_LAMP
    sh _copy.sh structuredLEDControl.cc 0
    cd $EPOS
    make APPLICATION=structuredLEDControl
    arm-objcopy -I elf32-little -O binary img/structuredLEDControl.img img/structuredLEDControl.bin
    python red-bsl.py -t /dev/ttyUSB0 -f img/structuredLEDControl.bin -S
    cd $PWD_COMPILE_EPOS_LAMP
else
    if ! sh _copy.sh $programFileToCompile 0
    then
        echo "\nERROR! Could not to copy the initial files!"
        printHelp
        exit 1
    fi
    
    if ! sh _make.sh $programFileToCompile $3 0
    then
        echo "\nERROR! Could not to compile the program!"
        printHelp
        exit 1
    fi
    
    if ! sh _process.sh $programFileToCompile 0
    then
        echo "\nERROR! Could not to process the objcopy!"
        printHelp
        exit 1
    fi
    
    if ! sh _install.sh $programFileToCompile $2 0
    then
        echo "\nERROR! Could not to install the program into the EPOSMotes2!"
        printHelp
        exit 1
    fi
fi


# Exits the program using a successful exit status code.
exit 0





