/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
 =---------------------------------------="
 |  COMPILERS - ALGONQUIN COLLEGE (W24)  |"
 =---------------------------------------="
     By Hayden Martens and Allen Wong
 =---------------------------------------="
       ---_ ......._-_--.
      (|\ /      / /| \  \
      /  /     .'  -=-'   `.
     /  /    .'             )
   _/  /   .'        _.)   /
  / o   o        _.-' /  .'
  \          _.-'    / .'*|
   \______.-'//    .'.' \*|
    \|  \ | //   .'.' _ |*|
    `   \|//  .'.'_ _ _|*|
      .  .// .'.' | _ _ \*|
      \`-|\_/ /    \ _ _ \*\
      `/'\__/      \ _ _ \*\
      /^|            \ _ _ \*
     '  `             \ _ _ \
                       \_
 __      _______ _____  ______ _____
 \ \    / /_   _|  __ \|  ____|  __ \
  \ \  / /  | | | |__) | |__  | |__) |
   \ \/ /   | | |  ___/|  __| |  _  /
    \  /   _| |_| |    | |____| | \ \
     \/   |_____|_|    |______|_|  \_\
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* Language name */
#define STR_LANGNAME	"Viper"

/* Logical constants - adapt for your language */
#define VIPER_TRUE  1
#define VIPER_FALSE 0

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/



typedef char			viper_char;
typedef char* 			viper_str;
typedef int				viper_int;
typedef float			viper_float;
typedef int				viper_bool;
typedef void 			viper_void;
typedef char            viper_byte;
typedef long            viper_long;     // Added typedef for A22

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER	= 'r',
	PGM_SCANNER = 's',
	PGM_PARSER	= 'p'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
viper_int mainReader(viper_int argc, viper_str* argv);

viper_int mainScanner(viper_int agrc, viper_str* argv);

/* 
TO_DO: Include later mainScaner (A22) and mainParser (A32)
*/
viper_void printLogo();

#endif
