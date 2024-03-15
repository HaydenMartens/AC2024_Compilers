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
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/


#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

viper_int main(int argc, char** argv) {
	viper_int i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_READER, "] - Reader\n* [",
			PGM_SCANNER, "] - Scanner\n* [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	viper_char option = argv[1][0];
	switch (option) {
	case PGM_READER:
		printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
		mainReader(argc, argv);
		break;
	case PGM_SCANNER :
		printf("%s%c%s", "\n[Option '", PGM_SCANNER, "': Starting SCANNER .....]\n\n");
		mainScanner(argc, argv);
		break;
	
	/* TO_DO: Include PARSER (A32) */
	default:
		printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
			PGM_READER, "] - Reader\n- [",
			PGM_SCANNER, "] - Scanner\n- [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver S23
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

/* TO_DO: Update your logo with your own language message */

viper_void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------= \n",
 		"\t|  COMPILERS - ALGONQUIN COLLEGE (W24)  |\n",
 		"\t=---------------------------------------=\n",
     	"\t   By Hayden Martens and Allen Wong      \n",
        "\t=---------------------------------------=\n",
		"\t__      _______ _____  ______ _____    \n",
 		"\t\\ \\    / /_   _|  __ \\|  ____|  __ \\   \n", 
        "\t \\ \\  / /  | | | |__) | |__  | |__) |  \n",
        "\t  \\ \\/ /   | | |  ___/|  __| |  _  /   \n",
    	"\t   \\  /   _| |_| |    | |____| |\\ \\   \n",
        "\t    \\/   |_____|_|    |______|_| \\_\\  \n"	
	);
}
