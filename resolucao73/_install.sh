#!/bin/sh


# Import the helper functions.
. ./installer_files/__helper_functions.sh

# The EPOSMotes2 installer
EPOS_MOTES2_INSTALLER="red-bsl.py"
EPOS_MOTES2_INSTALLER_BINARY1="flasher.bin"
EPOS_MOTES2_INSTALLER_BINARY2="ssl.bin"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1
computerUSBNumber=$2


# Read the EPOSMotes2 installer flags, if omitted, use as default one '-S' to write the program
# to the flash memory.
if [ $# -gt 2 ]
then
    installerFlags=""
    printf "Sending the program to the RAM Memory!\n"
else
    installerFlags="-S"
    printf "Sending the program to the FLASH Memory!\n"
fi


# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)

# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
     || [ $# -eq 0 ]
then
    printf "\nINSTALL ERROR!\nCould not find '$PWD_COMPILE_EPOS_LAMP/$programFileToCompile'\n"
    printHelp
    exit 1
fi


# Calculates whether the $computerUSBNumber is a number
isInteger $computerUSBNumber

# Captures the return value of the previous function call command
isIntegerReturnValue=$?

# Notify an invalid USB port number passed as parameter.
if ! [ $isIntegerReturnValue -eq 1 ]
then
    # Calculates whether the $computerUSBNumber is empty
    isEmpty $computerUSBNumber
    
    # Captures the return value of the previous function call command
    isEmptyReturnValue=$?
    
    # Notify an invalid USB port number passed as parameter, or uses 0 as default if not provided.
    if [ $isEmptyReturnValue -eq 1 ]
    then
        computerUSBNumber=0
    else
        printf "\nINSTALL ERROR! Bad USB port number: '$computerUSBNumber'!\n"
        printf "Use: ./install.sh PROGRAM_NAME.cc 0\n"
        printHelp
        exit 1
    fi
fi


# Switch to the main/compilation directory.
cd $EPOS

# To copy the the compiler file 'red-bsl.py' to be used.
if ! [ -f $EPOS_MOTES2_INSTALLER ] \
     || ! [ -f $EPOS_MOTES2_INSTALLER_BINARY1 ] \
     || ! [ -f $EPOS_MOTES2_INSTALLER_BINARY2 ]
then
    cp $EPOS/tools/emote/$EPOS_MOTES2_INSTALLER $EPOS/
    cp $EPOS/tools/emote/$EPOS_MOTES2_INSTALLER_BINARY1 $EPOS/
    cp $EPOS/tools/emote/$EPOS_MOTES2_INSTALLER_BINARY2 $EPOS/
fi


# To send the compiled application to the EPOSMotes2 board.
# python red-bsl.py -t /dev/ttyUSB0 -f img/structuredLEDControl.bin -S
if sudo python $EPOS_MOTES2_INSTALLER -t /dev/ttyUSB$computerUSBNumber -f img/$programNameToCompile.bin $installerFlags
then
    printf "\nATTENTION:\nInstall/use cutecom to read the EPOSMotes2 cout stream output.\n"
    printf "To install it, use:\n"
    printf "sudo apt-get install cutecom\n"
    printf "To run it, use:\n"
    printf "sudo cutecom /dev/ttyUSB0 &\n"
    printf "After open cutecom, click on the open button then press the EPOSMotes2 reset\n"
    printf "button, otherwise it will not work, to send commands to the EPOSMotes2 by\n"
    printf "USB device. As: echo :R100 > /dev/ttyUSB0\n"
else
    printf "\nINSTALL ERROR!\nCould not send the application to the EPOSMotes2 board!\n"
    printHelp
    exit 1
fi


# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP

# Exits the program using a successful exit status code.
exit 0


