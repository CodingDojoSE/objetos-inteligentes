Controle de objetos inteligentes numa Internet das Coisas.

#Installation

1. After to install EPOS as explained at the first section
[INE5412_EposMotesII_SmartObjects](https://github.com/evandrocoan/INE5412_EposMotesII_SmartObjects/tree/develop#installation),
just use our project installer to compile and install your application:
```
./compile_and_install.sh PROGRAM_NAME.cc
```

The './compile_and_install.sh' parameters are:
```
The FIRST parameter is the file name to install.
The SECOND parameter is the USB port number to install.
The THIRD parameter is the optional value clean for a 'make veryclean' all.
Examples:
./compile_and_install.sh PROGRAM_NAME.cc 0 clean
./compile_and_install.sh PROGRAM_NAME.cc 1
```
The 'compile_and_install.sh' is a wrapper which calls the following programs to perform
the program required steps. These steps must to be performed run in following order:
```
./_copy.sh    PROGRAM_NAME.cc
./_make.sh    PROGRAM_NAME.cc OPTION_NAME           Use 'clean' as 2nd parameter to perform a make veryclean all.
./_process.sh PROGRAM_NAME.cc ANYTHIN...            If passed anything, to install the program to the RAM memory, instead of the flash memory.
./_install.sh PROGRAM_NAME.cc USB_PORT_NUMBER       If not provided, default is 0.



