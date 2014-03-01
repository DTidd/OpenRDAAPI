
/*****************************************************************************
			APPLICATION HEADER
				 curver.h 
******************************************************************************/

#ifndef CURVER_H
#define CURVER_H
#include <misc.hpp>

int CurrentBinaryVersion=0;
int BinaryVersionInstalled=0;
#define GetBinaryVersion(a)	xGetBinaryVersion(a,__LINE__,__FILE__)
int xGetBinaryVersion(char *,int,char *);
#define WriteBinaryVersion(a,b)	xWriteBinaryVersion(a,b,__LINE__,__FILE__)
short xWriteBinaryVersion(char *,int,int,char *);

#endif /* CURVER */
