/*
 *  @file getch.h
 *	@brief Linux implementation of getch
 *  @author Guillaume Roux
 *  @version 17 november 2019
 */

#ifndef SRC_MODULES_GETCH_H_
#define SRC_MODULES_GETCH_H_

void initTermios(int echo) ;

void resetTermios(void) ;

char getch_(int echo) ;

char getch(void);

char getche(void);


#endif /* SRC_MODULES_GETCH_H_ */
