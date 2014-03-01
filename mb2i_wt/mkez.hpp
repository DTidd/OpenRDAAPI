/******************************************************************************
				MAKE EZRPT FUNCTIONS HEADER
					mkez.h
******************************************************************************/
#ifndef MKEZ_H
#define MKEZ_H
#include<rptgen.hpp>

#define ADVaddEZdetail(a,b,c,d,e,f,g,h,i,j)	xaddEZdetail(a,b,c,d,e,f,g,h,i,j,__LINE__,__FILE__)
#define addEZdetail(a,b,c,d,e,f,g,h,i)	xaddEZdetail(a,b,c,d,e,f,FALSE,g,h,i,__LINE__,__FILE__)
short xaddEZdetail(RDAezrpt *,char *,char *,short,short,short,char,short,char *,char *,int,char *);
#define addEZsort(a,b,c,d,e,f,g)	xaddEZsort(a,b,c,d,e,f,g,__LINE__,__FILE__)
short xaddEZsort(RDAezrpt *,char *,short,short,short,short,char,int,char *);
void addEZsortFROMTOSTRING(RDAezsrt *,char *,char *);
void addEZsortFROMTOBOOLNS(RDAezsrt *,char,char);
void addEZsortFROMTOSHORT(RDAezsrt *,short,short);
void addEZsortFROMTOINT(RDAezsrt *,int,int);
void addEZsortFROMTODOUBLE(RDAezsrt *,double,double);
#define addEZincvir(a,b,c,d)	xaddEZincvir(a,b,c,d,__LINE__,__FILE__)
short xaddEZincvir(RDAezrpt *,char *,char *,char *,int,char *);

#define SetReportDefaultEngine(a)	xSetReportDefaultEngine(a,__LINE__,__FILE__)
void xSetReportDefaultEngine(RDAreport *,int,char *);

#endif /* MKEZ_H */
