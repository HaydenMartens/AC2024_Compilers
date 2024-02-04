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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for Viper */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
/* TO_DO: BIT 3: END = End of buffer flag */
#define READER_END_FLAG 0x08 // (0000.1000)
/* TO_DO: BIT 2: REL = Rellocation memory flag */
#define READER_REL_FLAG 0x04 // (0000.0100)
/* TO_DO: BIT 1: EMP = Buffer empty flag */
#define READER_EMP_FLAG 0x02 // (0000.0010)
/* TO_DO: BIT 0: FUL = Buffer full flag */
#define READER_FUL_FLAG 0x01 // (0000.00001)_2 = (0.1)_16

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	viper_int mark;			/* the offset to the mark position (in chars) */
	viper_int read;			/* the offset to the get a char position (in chars) */
	viper_int wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	viper_str	content;			/* pointer to the beginning of character array (character buffer) */
	viper_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	viper_int		increment;			/* character array increment factor */
	viper_int		mode;				/* operational mode indicator */
	viper_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;				/* Offset / position field */
	viper_int		histogram[NCHAR];	/* Statistics of chars */
	viper_int		numReaderErrors;	/* Number of errors from Reader */
} Buffer, *BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(viper_int, viper_int, viper_char);
BufferPointer	readerAddChar		(BufferPointer const, viper_char);
viper_bool		readerClear		    (BufferPointer const);
viper_bool		readerFree		    (BufferPointer const);
viper_bool		readerIsFull		(BufferPointer const);
viper_bool		readerIsEmpty		(BufferPointer const);
viper_bool		readerSetMark		(BufferPointer const, viper_int);
viper_int		readerPrint		    (BufferPointer const);
viper_int		readerLoad			(BufferPointer const, FILE* const);
viper_bool		readerRecover		(BufferPointer const);
viper_bool		readerRetract		(BufferPointer const);
viper_bool		readerRestore		(BufferPointer const);
/* Getters */
viper_char		readerGetChar		(BufferPointer const);
viper_str	readerGetContent	(BufferPointer const, viper_int);
viper_int		readerGetPosRead	(BufferPointer const);
viper_int		readerGetPosWrte	(BufferPointer const);
viper_int		readerGetPosMark	(BufferPointer const);
viper_int		readerGetSize		(BufferPointer const);
viper_int		readerGetInc		(BufferPointer const);
viper_int		readerGetMode		(BufferPointer const);
viper_int		readerGetFlags		(BufferPointer const);
viper_void		readerPrintStat		(BufferPointer const);
viper_int		readerNumErrors		(BufferPointer const);

#endif
