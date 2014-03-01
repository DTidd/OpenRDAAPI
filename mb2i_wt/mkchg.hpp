/******************************************************************************
			MAKE CHANGE FILE FUNCTIONS HEADER
					mkchg.h
******************************************************************************/
#ifndef MKCHG_H
#define MKCHG_H

#include<dfchg.hpp>
#include<mix.hpp>

#define addChangeFileSupportingRDAconfld(a,b,c,d)	xaddChangeFileSupportingRDAconfld(a,b,c,d,__LINE__,__FILE__)
short xaddChangeFileSupportingRDAconfld(RDAsearch *,char *,char *,char,int,char *);

#define addChangeFileSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	xaddChangeFileSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,__LINE__,__FILE__)
int xaddChangeFileSupporting(CHGfile *,char *,char *,int,short,RDAconfld *,char *,char *,char *,short,char *,char,char *,char *,char *,char *,int,char *);

#define addChangeField(a,b,c)	xaddChangeField(a,b,c,__LINE__,__FILE__)
int xaddChangeField(CHGfile *,char,char *,int,char *);

#define addChangeFile(a,b,c,d,e,f,g,h,i)	xaddChangeFile(a,b,c,d,e,f,g,h,i,__LINE__,__FILE__)
int xaddChangeFile(CHGstruct *,char *,char *,char *,char *,int,CHGfield *,int,RDAsearch *,int,char *);

#define RDACHGstructNEW(a,b,c,d,e,f,g,h,i,j,k)	xRDACHGstructNEW(a,b,c,d,e,f,g,h,i,j,k,__LINE__,__FILE__)
CHGstruct *xRDACHGstructNEW(char *,char *,char *,char *,short,unsigned,char,char *,char *,int,CHGfile *,int,char *);


#endif /* MKCHG_H*/
