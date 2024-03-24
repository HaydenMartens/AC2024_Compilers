/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=
|              ....                     |
|          ........::.::::::.           |
|        .:........::.:^^^~~~:          |
|        :^^::::::^^^::^!7??7~^.        |
|       .:^~~^!77777~~7?YY?7??7^.       |
|       :.^~!??!^::::^^~!?5PY??!~.      |
|       ~!!7J~.:::^^^^~!!~~?G5J?~       |
|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |
|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |
|       .~77..    . .~^:^^^~7?:         |
|       .^!^~:::.:^!7?~^~!77J:          |
|        ^^!Y~^^^^~?YJ77??7JJ^          |
|       .^7J?~^~~^~7??7??7JY?~:         |
|        ::^^~^7?!^~~!7???J?J7~:.       |
|         ^~~!.^7YPPPP5Y?7J7777~.       |
|        ..:~..:^!JPP5YJ?!777!^.        |
| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |
|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |
|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |
|    :~75 \__ \/ _ \  _| / _` | 5?.     |
|     7~7 |___/\___/_| |_\__,_| Y5?.    |
|    .^~!~.....................P5YY7.   |
|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |
|                                       |
=---------------------------------------=
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */
#define FL_LEN	10

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 15

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	SOS_T,		/*  9: Start of scope (colon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T,		/* 12: Comment token */
	FL_T,		// 13: Float Token
	AS_T		// 14: Assignment Token
};

/* TO_DO: Define the list of keywords */
static viper_str tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"SOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"FL_T",
	"AS_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	viper_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	viper_int intValue;				/* integer literal attribute (value) */
	viper_int keywordIndex;			/* keyword index in the keyword table */
	viper_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	viper_float floatValue;				/* floating-point literal attribute (value) */
	viper_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	viper_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	viper_byte flags;			/* Flags information */
	union {
		viper_int intValue;				/* Integer value */
		viper_float floatValue;			/* Float value */
		viper_str stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	viper_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	viper_int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '\n'
#define CHRCOL3 '\'' 
#define CHRCOL3A '"'
#define CHRCOL4 '_'
#define CHRCOL6 '#'
#define CHRCOL7 '.'

/* These constants will be used on VID / MID function */
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	12		/* Error state with no retract */
#define ESWR	13		/* Error state with retract */	
#define FS		14		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		14
#define CHAR_CLASSES	9

/* TO_DO: Transition table - type of states defined in separate table */
static viper_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],   \n,    \',   _', SEOF,    #, other
	   L(0), D(1), P(2), Q(3), Q(4), E(5), C(6), .(7)  O(8) */
	{     1,    6, ESNR,   10, ESNR, ESNR,   4,  ESNR,ESNR},	// S0: NOAS
	{     1,    1,	 2,    3,	  1, ESNR,   1,  ESNR,   2},	// S1: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS,  FS},	// S2: FUNC DEF
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS,  FS},	// S3: ASWR (KEY)
	{     4,    4,    5,    4,    4,    5,	  4,	4,   4},	// S4: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS,  FS},	// S5: ASNR (COMMENT)
	{  ESNR,    6,    8, ESNR, ESNR, ESNR, ESNR,	7,   8},	// S6: NOAS
	{  ESNR,    7,	  9, ESNR, ESNR, ESNR, ESNR, ESNR,	 9},	// S7: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS,  FS},	// S8: ASNR (INT)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS,  FS}, // S9: ASWR (FLOAT)
	{    10,   10,   10,   11,   10,  ESNR,  10, ESNR,  10}, // S10: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS}, // S11: STR LIT
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS}, // S12 ASNR
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS}  // S13 ASWR
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static viper_int stateType[NUM_STATES] = {
	NOFS,  /* 00 */
	NOFS,  /* 01 */
	FSWR,  /* 02 (MID) - Methods */
	FSNR,  /* 03 (KEY) */
	NOFS,  /* 04 */
	FSNR,  /* 05 (COM) */
	NOFS,  /* 06 */
	NOFS,  /* 07 (COM) */
	FSNR,  /* 08 (Err1 - no retract) */
	FSNR,  /* 09 (Err2 - retract) */
	NOFS,  // 10
	FSNR,  // 11
	FSNR,
	FSWR
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
viper_int			startScanner(BufferPointer psc_buf);
static viper_int	nextClass(viper_char c);					/* character class function */
static viper_int	nextState(viper_int, viper_char);		/* state machine function */
viper_void			printScannerData(ScannerData scData);
Token				tokenizer(viper_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(viper_str lexeme);

/* Declare accepting states functions */
Token funcSL	(viper_str lexeme);
Token funcIL	(viper_str lexeme);
Token funcFL	(viper_str lexeme);
Token funcID	(viper_str lexeme);
Token funcCMT   (viper_str lexeme);
Token funcKEY	(viper_str lexeme);
Token funcErr	(viper_str lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcCMT,	/* CMT  [05] */
	NULL,		/* -    [06] */
	NULL,		//		[07]
	funcIL,	    /* COM  [08] */
	funcFL,		/* Fl   [09] */
	NULL,	    /* ERR2 [10] */
	funcSL,		//		[11]
	funcErr,	//		[12]
	funcErr		//		[13]
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 13

/* TO_DO: Define the list of keywords */
static viper_str keywordTable[KWT_SIZE] = {
	"def",		/* KW00 */
	"if",		/* KW01 */
	"int",		/* KW02 */
	"else",		/* KW03 */
	"then",	    /* KW04 */
	"while",	/* KW05 */
	"return",	/* KW06 */
	"try",		/* KW07 */
	"except",	/* KW08 */
	"do",		/* KW09 */
	"elif",		/* KW10 */
	"True",		/* KW11 */
	"False"		/* KW12 */
};
//def, if, else, then, while, return, try, except, do, while}
/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	viper_char indentationCharType;
	viper_int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
viper_int numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif