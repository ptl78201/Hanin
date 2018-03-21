/****************************************************************************
 * File Name   : hdicmain.c
 * Description : main function of Hanin dicitonary maker tool
 * Revision History     :
 *  2007.12.28  PTL Wu Jia-Kwan     for HDicMaker tool
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "hdic_def.h"

/**************************************
  Definition
**************************************/
#define SIZE_FNAME      128

/**************************************
  Global Variables
**************************************/
int iCharSrcCount ;
int iWordSrcCount ;
CHAR_CMP_BUF stCharSource[MAX_CHAR_LINE_CNT] ;
WORD_CMP_BUF stWordSource[MAX_WORD_LINE_CNT] ;

/**************************************
  external functions
**************************************/
extern int MakeHaninDictionary ( char *, char *, char *, char * ) ;
extern int MakeAimeiDictionary ( char *, char * ) ;

/********************************************************************
 * Function Name: main
 * Description: main function
 * Parameters:
 *     [input] int argc
 *             char *argv
 *    [return]
 * Syntax:
 *      hdicmaker char.cmp hv4.cmp hv4.dic 系統
 * Revision Histor:
 *  2007.12.28  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
main ( int argc, char *argv[] )
{
    int iResult ;
    char wdfname[SIZE_FNAME] ;
    char chfname[SIZE_FNAME] ;
    char dicfname[SIZE_FNAME] ;
    char diclabel[SIZE_FNAME] ;

	if ( argc != 3 && argc != 5) {
        printf( "USAGE 1> hdicmaker <character file> <word file> " ) ;
   	    printf( "<target file> <dictionary label> \n" ) ;
       	printf( "   EX: hdicmaker char.cmp word.big hv4.dic 系統\n" ) ;
       	printf( "       ==> build Hanin dicitonary hv4.dic using char.cmp ") ;
       	printf( "and word.big \n" ) ;

		printf( "USAGE 2> hdicmaker <aimei text file> <aimei binary file> \n" ) ;
		printf( "   EX: hdicmaker aimei.big aimei.dic \n" ) ;
		return 0 ;
    }
    strcpy ( chfname, *(argv+1) ) ;
    strcpy ( wdfname, *(argv+2) ) ;
	if ( argc == 5 ) {
	    strcpy ( dicfname, *(argv+3) ) ;
    	strcpy ( diclabel, *(argv+4) ) ;
	}

	if ( argc == 3 ) {
	    iResult = MakeAimeiDictionary ( chfname, wdfname ) ;
	}
	else if ( argc == 5 ) {
	    iResult = MakeHaninDictionary ( wdfname, chfname, dicfname, diclabel ) ;
	}

    if ( iResult < 0 ) {
        if ( iResult == -1 ) {
            printf ( "fatal> %s can't be opend !\n", chfname ) ;
        }
        else if ( iResult == -2 ) {
            printf ( "fatal> %s can't be opend !\n", wdfname ) ;
        }
        else if ( iResult == -3 ) {
            printf ( "fatal> %s can't be opend !\n", dicfname ) ;
        }
        return 0 ;
    }
    else {
        return 1 ;
    }
}

/****************************************************************************
 * Copyright (C) 1999~2008 Panasonic Taiwan Laboratories Co., Ltd.
 ****************************************************************************/

