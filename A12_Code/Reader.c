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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to initial (default) values.
*************************************************************
*/

BufferPointer readerCreate(viper_int size, viper_int increment, viper_char mode) {
	BufferPointer readerPointer;
	if(size <= 0){ // Error checking params
		size = READER_DEFAULT_SIZE;
	}
	if(increment == 0){ // checking increment for no param
		increment = READER_DEFAULT_INCREMENT;
	}
	
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return NULL;
	readerPointer->content = (viper_str)malloc(size);

	for(int i = 0; i < NCHAR; i++){
		readerPointer->histogram[i] = 0; // intialize the histogram to each slot
	}
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;

	readerPointer->flags = READER_DEFAULT_FLAG;	// Initalize the flag to EMP
	readerPointer->flags = readerPointer->flags | READER_EMP_FLAG;
	/* NEW: Cleaning the content */
	if (readerPointer->content)
		readerPointer->content[0] = READER_TERMINATOR;	
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, viper_char ch) {
	viper_str tempReader = NULL;
	viper_int newSize = 0;

	if(!readerPointer){
		return NULL;
	}

	if((int)(ch) < 0){
		readerPointer->numReaderErrors++;
	//	return NULL;
	}

	readerPointer->flags = readerPointer->flags & !(READER_REL_FLAG); // reset REL flag

	if (readerPointer->position.wrte * (viper_int)sizeof(viper_char) < readerPointer->size) {
		if(readerPointer->position.wrte * (viper_int)sizeof(viper_char) == readerPointer->size){ // test if I need to set the FUL bit
			readerPointer->flags = readerPointer->flags | READER_FUL_FLAG; // set the FUL bit
		}
	} else {
		readerPointer->flags = readerPointer->flags & !(READER_FUL_FLAG); // apply & to !FULL flag to reset it
		switch (readerPointer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			if(newSize <= readerPointer->size || newSize > READER_MAX_SIZE){
				return NULL;
			}
			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			if(newSize <= readerPointer->size || newSize > READER_MAX_SIZE){
				return NULL;
			}
			break;
		default:
			return NULL;
		}

		tempReader = realloc(readerPointer->content,newSize);
		if(tempReader == readerPointer->content){
			readerPointer->content = tempReader;
		} else if (tempReader == NULL){
			return NULL;
		} else {
			readerPointer->flags = readerPointer->flags | READER_REL_FLAG; // set REL flag due to pointer location change
			readerPointer->content = tempReader;
			}
	}

	readerPointer->content[readerPointer->position.wrte++] = ch;

	readerPointer->histogram[ch]++; // Increment Histogram at location ch
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
viper_bool readerClear(BufferPointer const readerPointer) {

	if(!readerPointer){
		return VIPER_FALSE;
	}
	readerPointer->flags = readerPointer->flags | READER_EMP_FLAG; // set EMP flag
	readerPointer->flags = readerPointer->flags & !(READER_FUL_FLAG); // reset FUL flag

	readerPointer->position.wrte = readerPointer->position.mark = readerPointer->position.read = 0;
	return VIPER_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
viper_bool readerFree(BufferPointer const readerPointer) {
	
	if(!readerPointer){
		return VIPER_FALSE;
	}

	free(readerPointer->content); // free content pointer
	free(readerPointer); // free structure pointer

	return VIPER_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_bool readerIsFull(BufferPointer const readerPointer) {
	
	if(!readerPointer){
		return VIPER_FALSE;
	}

	if((readerPointer->flags & READER_FUL_FLAG) != 0){ // check if empty
		return VIPER_FALSE; // return if FUL bit is empty
	} else {
		return VIPER_TRUE; // otherwise it is FUL so return true
	}
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
viper_bool readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return VIPER_TRUE;
	/* TO_DO: Check flag if buffer is EMP */
	if(!readerPointer){
		return VIPER_FALSE;
	}

	if((readerPointer->flags & READER_EMP_FLAG) != 0){ // check if empty
		return VIPER_FALSE; // return if EMP bit is empty
	} else {
		return VIPER_TRUE; // otherwise it is EMP so return true
	}
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
*************************************************************
*/
viper_bool readerSetMark(BufferPointer const readerPointer, viper_int mark) {
	if(!readerPointer){
		return VIPER_FALSE;
	}
	if(mark > 0 && mark <= readerPointer->position.wrte){
		return VIPER_FALSE;
	}
	readerPointer->position.mark = mark;
	return VIPER_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
*************************************************************
*/
viper_int readerPrint(BufferPointer const readerPointer) {
	viper_int cont = 0;
	viper_char c;

	if(!readerPointer){
		return VIPER_FALSE;
	}

	c = readerGetChar(readerPointer);

	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
		if((readerPointer->flags & READER_EMP_FLAG) != 0){
			break; // break loop on END bit flag
		}
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
*************************************************************
*/
viper_int readerLoad(BufferPointer const readerPointer, FILE* const fileDescriptor) {
	viper_int size = 0;
	viper_char c;
	if(!readerPointer){
		return READER_ERROR;
	}
	c = (viper_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_bool readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return VIPER_FALSE;
	/* TO_DO: Recover positions */
	readerPointer->position.read = 0;
	readerPointer->position.read = 0;
	return VIPER_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
viper_bool readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer)
		return VIPER_FALSE;

	if(readerPointer->position.read == 0){
		return VIPER_FALSE;
	} else {
		readerPointer->position.read--;
	}
	return VIPER_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_bool readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return VIPER_FALSE;
	/* TO_DO: Restore positions (read/mark) */
	readerPointer->position.read = readerPointer->position.mark;
	return VIPER_TRUE;
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL){
		return CHARSEOF;
	}

	if (readerPointer->position.read >= readerPointer->position.wrte){
		readerPointer->flags = readerPointer->flags | (READER_END_FLAG);
		return READER_TERMINATOR;
	}

	readerPointer->flags = readerPointer->flags & !(READER_END_FLAG);

	if (readerPointer->position.wrte>0){
		return readerPointer->content[readerPointer->position.read++];
	}
	return READER_TERMINATOR;
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_str readerGetContent(BufferPointer const readerPointer, viper_int pos) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return NULL;
	if (pos < 0)
		return NULL;
	/* TO_DO: Return content (string) */
	return readerPointer->content + pos;;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerGetPosRead(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerGetPosRead");
		return -1;
	}
	/* TO_DO: Return read */
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerGetPosWrte(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerGetPosWrte");
		return -1;
	}
	/* TO_DO: Return wrte */
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerGetPosMark(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerGetPosMark");
		return -1;
	}
	/* TO_DO: Return mark */
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerGetSize");
		return -1;
	}
	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerGetInc(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerGetInc");
		return -1;
	}
	/* TO_DO: Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerGetMode(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerGetMode");
		return -1;
	}
	/* TO_DO: Return mode */
	return readerPointer->mode;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerGetFlags(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerGetFlags");
		return -1;
	}
	/* TO_DO: Return flags */
	return readerPointer->flags;
}



/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_void readerPrintStat(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerPrintStat");
	}
	else {
		/* TO_DO: Print the histogram */
	for (int i = 0; i <= NCHAR; i++)
			if(readerPointer->histogram[i] != 0){
				printf("Histogram[%c]=%d\n",(char)(i), readerPointer->histogram[i]);
			}
	}
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
viper_int readerNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("Error in readerNumErrors");
		return -1;
	}
	/* TO_DO: Returns the number of errors */
	return readerPointer->numReaderErrors;
}

