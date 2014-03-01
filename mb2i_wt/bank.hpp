/******************************************************************************
				BANK FUNCTIONS HEADER
					bank.h
******************************************************************************/

#ifndef BANK_H
#define BANK_H
#include <stdio.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <rdadiag.h>
#include <gcwd.h>
#include <glogin.h>
#include <gui.h>
#include <olh.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <alloc.h>
#include <prterr.h>
#include <miscfunc.h>
#include <mix.h>


#define ADVupdate_bal(a,b,c,d,e)	xupdate_bal(a,b,c,d,e,__LINE__,__FILE__)
#define update_bal(a,b,c)	xupdate_bal(a,b,c,NULL,NULL,__LINE__,__FILE__)
short xupdate_bal(short,double,double,void (*)(),void *,int,char *);
/* xupdate_bal has five arguments and is used to update the BALANCE and 
  	UNPROCESSED BALANCE fields in the file specified by the file number.  The
	first argument is the file number.  The second argument is the number to be 
	added to the BALANCE field.  Third argument is the number to be add to the
	UNPROCESSED BALANCE field.  Fourth argument is a SubFunc() function used
	with SECURITY.  Fifth argument is an argument passed to the SubFunc().	
*/
#endif /* BANK_H */
