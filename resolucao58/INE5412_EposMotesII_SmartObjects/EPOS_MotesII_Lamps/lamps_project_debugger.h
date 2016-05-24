/*********************** Licensing *******************************************************
*
*  Copyright 2016 @ Evandro  Coan
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 3 of the License, or ( at
*  your option ) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************************
*/
#include <utility/ostream.h>
#include <semaphore.h>

#include <cstdarg>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <cstring>


/**
 * This is to view internal program data while execution. Default value: 0
 * 
 *  0   = Disables this feature.
 *  1  >= Basic debugging.
 */
#define DEBUG_LEVEL 1


#define DEBUG_LEVEL_DISABLED_DEBUG 0
#define DEBUG_LEVEL_BASIC_DEBUG    1


#if DEBUG_LEVEL > DEBUG_LEVEL_DISABLED_DEBUG

/**
 * A value like a127 (111111) for 'g_debugLevel' enables all 'a' mask debugging levels. To enable all
 * debugging levels at once, use "a127 b127 c127" etc, supposing the level 64 is the highest to each
 * mask 'a', 'b', 'c', etc.
 * 
 * MemoryManager debugging:
 * a0   - Disabled all debug.
 * a1   - Basic debug messages.
 * a2   - Functions entrances.
 * 
 * Algorithm Strategy debugging:
 * b0   - Disabled all debug.
 * b1   - MyClass object test after the main program to run.
 */
const char* const g_debugLevel = "a1 a2 b1";
#endif


/**
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;

/**
 * Stdout for debuggging.
 */
OStream cout;


/**
 * MemoryManager debugging.
 */
#if DEBUG_LEVEL > DEBUG_LEVEL_DISABLED_DEBUG
#define DEBUG


/**
 * The semaphore used to sincronize the debugging output.
 */
Semaphore g_debuger_semaphore;

/**
 * Print like function for logging putting a new line at the end of string. See the variables
 * 'g_debugLevel', 'g_debugMask', for the avalibles levels.
 * 
 * @param level     the debugging desired level to be printed.
 * @param ...       variable number os formating arguments parameters.
 */
#define DEBUGGERLN( level, ... ) \
do \
{ \
    if( __computeDeggingLevel( #level ) ) \
    { \
        g_debuger_semaphore.p(); \
        cout << __VA_ARGS__ << "\n"; \
        g_debuger_semaphore.v(); \
    } \
} \
while( 0 )

/**
 * The same as DEBUGGERLN(...) just below, but do not put automatically a new line.
 */
#define DEBUGGER( level, ... ) \
do \
{ \
    if( __computeDeggingLevel( #level ) ) \
    { \
        g_debuger_semaphore.p(); \
        cout << __VA_ARGS__; \
        g_debuger_semaphore.v(); \
    } \
} \
while( 0 )

/**
 * The same as DEBUGGER(...), but it is for standard program output.
 */
#define FPRINT( level, ... ) \
do \
{ \
    if( __computeDeggingLevel( #level ) ) \
    { \
        g_debuger_semaphore.p(); \
        cout << __VA_ARGS__; \
        g_debuger_semaphore.v(); \
    } \
} \
while( 0 )

/**
 * The same as DEBUGGERLN(...), but it is for standard program output.
 */
#define FPRINTLN( level, ... ) \
do \
{ \
    if( __computeDeggingLevel( #level ) ) \
    { \
        g_debuger_semaphore.p(); \
        cout << __VA_ARGS__ << "\n"; \
        g_debuger_semaphore.v(); \
    } \
} \
while( 0 )


/**
 * Determines whether the input char is a digit. We are not using the std
 */
int check_digit( char c )
{
    if( ( c >= '0' ) && ( c <= '9' ) )
    {
        return 1;
    }
    return 0;
}

/**
 * Downloaded from: https://github.com/KohnAir/mystrtok/blob/master/strtok.c
 */
int* create_delim_dict( char *delim )
{
#define DICT_LEN 256
    int* d = (int *) malloc( sizeof( int ) * DICT_LEN );
    memset( (void *) d, 0, sizeof( int ) * DICT_LEN );
    
    int i;
    
    for( i = 0; i < strlen( delim ); i++ )
    {
        d[ delim[ i ] ] = 1;
    }
    return d;
}

/**
 * Downloaded from: https://github.com/KohnAir/mystrtok/blob/master/strtok.c
 */
char* my_strtok( char *str, char *delim )
{
    static char* last;
    static char* to_free;
    
    int *deli_dict = create_delim_dict( delim );
    
    if( !deli_dict )
    {
        return NULL;
    }
    
    if( str )
    {
        last = (char *) malloc( strlen( str ) + 1 );
        
        if( !last )
        {
            free( deli_dict );
        }
        to_free = last;
        strcpy( last, str );
    }
    
    while( deli_dict[ *last ] && *last != '\0' )
    {
        last++;
    }
    str = last;
    
    if( *last == '\0' )
    {
        free( deli_dict );
        free( to_free );
        return NULL;
    }
    
    while( *last != '\0' && !deli_dict[ *last ] )
    {
        last++;
    }
    
    *last = '\0';
    last++;
    
    free( deli_dict );
    return str;
}

/**
 * Downloaded from: http://www.geeksforgeeks.org/write-your-own-atoi/
 */
int myAtoi( const char *str )
{
    int res = 0; // Initialize result
    
    // Iterate through all characters of input string and
    // update result
    for( int i = 0; str[ i ] != '\0'; ++i )
    {
        res = res * 10 + str[ i ] - '0';
    }
    
    // return result.
    return res;
}

/**
 * Determines whether the given debug level is enabled.
 * 
 * @param debugLevel       the given char* string level to the debugger.
 * @return true when the current debug output is enabled, false otherwise.
 */
inline bool __computeDeggingLevel( const char* debugLevel )
{
#define COMPUTE_DEBUGGING_LEVEL_DEBUG      0
#define COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE 32
    
    int inputLevel;
    int builtInLevel;
    
    int inputLevelSize;
    int builtInLevelSize;
    
    int inputLevelTokenSize;
    int builtInLevelTokenSize;
    
    const char* inputLevelToken;
    const char* builtInLevelToken;
    
    char builtInLevelChar[ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ];
    char inputLevelChar  [ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ];
    char inputLevelChars [ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ][ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ];
    
    int  inputLevels  = 0;
    char separator[2] = " ";
    
    inputLevelSize   = strlen( debugLevel );
    builtInLevelSize = strlen( g_debugLevel );
    
    if( ( 2 > inputLevelSize
        && inputLevelSize > COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE )
        || ( 2 > builtInLevelSize
           && builtInLevelSize > COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ) )
    {
        cout << "ERROR while processing the DEBUG LEVEL: " << debugLevel << "\n";
        cout << "! The masks sizes are " << inputLevelSize << " and " << builtInLevelSize;
        cout << ", but they must to be between 1 and 32." << "\n";
        
        //exit( EXIT_FAILURE ); This could not to be included due the <stdlib.h> conflict within EPOS.
        return false;
    }
    
    strcpy( inputLevelChar, debugLevel );
    strcpy( builtInLevelChar, g_debugLevel );
    
    // So, how do we debug the debugger?
#if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
    int currentExternLoop = 0;
    int currentInternLoop = 0;

    cout << "\ng_debugLevel: " << g_debugLevel << ", builtInLevelSize: " << builtInLevelSize ;
    cout << ", debugLevel: " << debugLevel << ", inputLevelSize: " << inputLevelSize  << "\n";
#endif
    
    inputLevelToken = my_strtok( inputLevelChar, separator );
    
    do
    {
        strcpy( inputLevelChars[ inputLevels++ ], inputLevelToken );
    } while( ( inputLevelToken = my_strtok( NULL, separator ) ) != NULL );
    
    while( inputLevels-- > 0 )
    {
    #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
        currentInternLoop = 0;
        cout << "CURRENT_ExternLoop: " << currentExternLoop++ << "\n";
    #endif
        
        builtInLevelToken   = my_strtok( builtInLevelChar, separator );
        inputLevelTokenSize = strlen( inputLevelChars[ inputLevels ] );
        
        do
        {
            builtInLevelTokenSize = strlen( builtInLevelToken );
            
        #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
            cout << "space" << "\n";
            cout << "CURRENT_InternLoop: " << currentInternLoop++ << "\n";
            
            cout << "builtInLevelToken: " << builtInLevelToken << "\n";
            cout << "builtInLevelTokenSize: " << builtInLevelTokenSize << "\n";
            cout << "inputLevelChars[" << inputLevels << "]: " << inputLevelChars[ inputLevels ] << "\n";
            cout << "inputLevelTokenSize: " << inputLevelTokenSize << "\n";
        #endif
            
            if( inputLevelTokenSize > 0
                && builtInLevelTokenSize > 0 )
            {
                if( check_digit( inputLevelChars[ inputLevels ][ 1 ] )
                    && check_digit( builtInLevelToken[ 1 ] ) )
                {
                    if( builtInLevelToken[ 0 ] == inputLevelChars[ inputLevels ][ 0 ] )
                    {
                        inputLevel   = myAtoi( &inputLevelChars[ inputLevels ][ 1 ] );
                        builtInLevel = myAtoi( &builtInLevelToken[ 1 ] );
                        
                    #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
                        cout << "builtInLevel: " << builtInLevel << "\n";
                        cout << "inputLevel: " << inputLevel << "\n";
                        cout << "Is activeated? " << ( ( inputLevel & builtInLevel ) > 0 ) << "\n";
                    #endif
                        
                        if( ( inputLevel & builtInLevel ) > 0 )
                        {
                            return true;
                        }
                    }
                }
            }
            
        } while( ( builtInLevelToken = my_strtok( NULL, separator ) ) != NULL );
        
    #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
        cout << "space" << "\n";
    #endif
    }
    
    return false;
}


#else
    #define DEBUGGER( stream, ... )
    #define DEBUGGERLN( stream, ... )

/**
 * The same as DEBUGGER(...), but it is for standard program output when the debugging is disabled.
 */
#define FPRINT( level, ... ) \
do \
{ \
    cout << __VA_ARGS__; \
} \
while( 0 )


/**
 * The same as DEBUGGERLN(...), but it is for standard program output when the debugging is disabled.
 */
#define FPRINTLN( level, ... ) \
do \
{ \
    cout << __VA_ARGS__ << "\n"; \
} \
while( 0 )


#endif // #if DEBUG_LEVEL > 0



/**
 * Preprocessor directive designed to cause the current source file to be included only once in a
 * single compilation. Thus, serves the same purpose as #include guards, but with several
 * advantages, including: less code, avoidance of name clashes, and sometimes improvement in
 * compilation speed. In main file this is enabled by default.
 */
#pragma once

/**
 *  Calculates a static array size.
 */
#if !defined STATIC_ARRAY_SIZE
    #define STATIC_ARRAY_SIZE( array ) ( sizeof( ( array ) ) / sizeof( ( array )[0] ) )

#endif



/**
 * Missing string printf. This is safe and convenient but not exactly efficient.
 * 
 * @param fmt     a char array
 * @param ...     a variable length number of formating characters.
 * 
 * @see http://stackoverflow.com/a/10150393/4934640
 * @see http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf/10150393#10150393
 */
inline char* debugger_string_formatter(const char* fmt, ...)
{
    int   size   = 512;
    char* buffer = 0;
    
    buffer = new char[size];
    
    va_list vl;
    va_start(vl, fmt);
    
    int nsize = sprintf(buffer, fmt, vl);
    
    //fail delete buffer and try again
    if(nsize < 0)
    { 
        cout << "\nERROR ON: inline string format(const char* fmt, ...)\n";
    }
    
    va_end(vl);

    return buffer;
}
