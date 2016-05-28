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
 * A value like 127 (111111) for 'g_debugLevel' enables all debug levels, supposing the level 64 is
 * the highest to each available.
 * 
 * MemoryManager debugging:
 * 0   - Disabled all debug.
 * 1   - Basic debug messages.
 * 2   - Functions entrances.
 * 4   - Step by step functions.
 */
const int g_debugLevel = 7;
#endif



/**
 * This is used to print the same message 'C_OUT_REPEATED_OUTPUT_NUMBER' times. This is because the
 * EPOS 1.1 motherfucker just ignores some messages outputs some times it feels it needs to ignores
 * you.
 */
#define C_OUT_REPEATED_OUTPUT_NUMBER 3

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
 * Print like function for logging putting a new line at the end of string. See the variable
 * 'g_debugLevel' for the available levels.
 * 
 * @param level     the debugging desired level to be printed.
 * @param ...       variable number os formating arguments parameters.
 */
#define DEBUGGERLN( level, ... ) \
do \
{ \
    if( level & g_debugLevel ) \
    { \
        g_debuger_semaphore.p(); \
        for( int i = 0; i < C_OUT_REPEATED_OUTPUT_NUMBER; i++ ) \
            cout << __VA_ARGS__ << endl; \
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
    if( level & g_debugLevel ) \
    { \
        g_debuger_semaphore.p(); \
        for( int i = 0; i < C_OUT_REPEATED_OUTPUT_NUMBER; i++ ) \
            cout << __VA_ARGS__; \
        g_debuger_semaphore.v(); \
    } \
} \
while( 0 )

/**
 * The same as DEBUGGER(...), but it is for standard program output.
 */
#define PRINT( level, ... ) \
do \
{ \
    if( level & g_debugLevel ) \
    { \
        g_debuger_semaphore.p(); \
        for( int i = 0; i < C_OUT_REPEATED_OUTPUT_NUMBER; i++ ) \
            cout << __VA_ARGS__; \
        g_debuger_semaphore.v(); \
    } \
} \
while( 0 )

/**
 * The same as DEBUGGERLN(...), but it is for standard program output.
 */
#define PRINTLN( level, ... ) \
do \
{ \
    if( level & g_debugLevel ) \
    { \
        g_debuger_semaphore.p(); \
        for( int i = 0; i < C_OUT_REPEATED_OUTPUT_NUMBER; i++ ) \
            cout << __VA_ARGS__ << endl; \
        g_debuger_semaphore.v(); \
    } \
} \
while( 0 )


#else
    #define DEBUGGER( stream, ... )
    #define DEBUGGERLN( stream, ... )

/**
 * The same as DEBUGGER(...), but it is for standard program output when the debugging is disabled.
 */
#define PRINT( level, ... ) \
do \
{ \
    cout << __VA_ARGS__; \
} \
while( 0 )


/**
 * The same as DEBUGGERLN(...), but it is for standard program output when the debugging is disabled.
 */
#define PRINTLN( level, ... ) \
do \
{ \
    cout << __VA_ARGS__ << endl; \
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



