/* mknrd.c - Make Datafiles */
#ifndef MKNRD_H
#define MKNRD_H
#include<mix.hpp>

#define CompareDATABASE(a)	xCompareDATABASE(a,__LINE__,__FILE__)
char xCompareDATABASE(DBsort *,int,char *);
#define MergeDATABASE(a)	xMergeDATABASE(a,__LINE__,__FILE__)
void xMergeDATABASE(DBsort *,int,char *);
#define CHANGE_DATABASE_FILE(a,b,c,d,e,f,g,h)	xCHANGE_DATABASE_FILE(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
short xCHANGE_DATABASE_FILE(char *,short,char *,short,NRDfield *,short,
	NRDkey *,char *,int,char *);
#define SetupFileDefinition(a)	xSetupFileDefinition(a,__LINE__,__FILE__)
void xSetupFileDefinition(DBsort *,int,char *);
#endif
