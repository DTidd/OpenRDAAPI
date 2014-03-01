/******************************************************************************
		   POSITION TRACKING FUNCTIONS HEADER
				postrk.h
******************************************************************************/

#ifndef POSTRK_H
#define POSTRK_H
#include <cstdio>
#include <cstdlib>
#include <mix.hpp>
#include <olh.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <gui.hpp>
#include <misc.hpp>

#define updateftes(f,p,a) xupdateftes(f,p,a,1,NULL,NULL,__LINE__,__FILE__)
#define updatetotfte(f,p,a) xupdateftes(f,p,a,0,NULL,NULL,__LINE__,__FILE__)
#define ADVupdateftes(f,p,a,b,c) xupdateftes(f,p,a,1,b,c,__LINE__,__FILE__)
#define ADVupdatetotfte(f,p,a,b,c) xupdateftes(f,p,a,0,b,c,__LINE__,__FILE__)
short xupdateftes(short,char *,double,short,void (*)(...),void *,int,char *);
/* xupdateftes function update the Total and Filled FTE amounts for
	the Position Identification Master.
	args:	#1(f)->file number for [POSTRK][POSMSTR].
		#2(p)->Position Identification Key Value.
		#3(a)->amount of the adjustment.
		#4(a)->1=TOTAL and FILLED FTE AMOUNTs, 
		       0=only TOTAL FTE AMOUNT.
*/
#endif /* POSTRK_H */
