/******************************************************************************
			MAKE SCREEN DEFAULTS FUNCTIONS HEADER
					mkdef.h
******************************************************************************/
#ifndef MKDEF_H
#define MKDEF_H

#include<mix.hpp>

#define addDefaultFieldSTRING(a,b,c,d,e)	xaddDefaultFieldSTRING(a,b,c,d,e,__LINE__,__FILE__)
int xaddDefaultFieldSTRING(RDAdefault *,char *,char ,short,char *,int,char *);

#define addDefaultFieldDOUBLE(a,b,c,d,e)	xaddDefaultFieldDOUBLE(a,b,c,d,e,__LINE__,__FILE__)
int xaddDefaultFieldDOUBLE(RDAdefault *,char *,char ,short,double,int,char *);

#define addDefaultFieldINT(a,b,c,d,e)	xaddDefaultFieldINT(a,b,c,d,e,__LINE__,__FILE__)
int xaddDefaultFieldINT(RDAdefault *,char *,char ,short,int,int,char *);

#define addDefaultFieldSHORT(a,b,c,d,e)	xaddDefaultFieldSHORT(a,b,c,d,e,__LINE__,__FILE__)
int xaddDefaultFieldSHORT(RDAdefault *,char *,char ,short,short,int,char *);

#define addDefaultFieldCHAR(a,b,c,d,e)	xaddDefaultFieldCHAR(a,b,c,d,e,__LINE__,__FILE__)
int xaddDefaultFieldCHAR(RDAdefault *,char *,char,short,char,int,char *);
#endif /* MKDEF_H */
