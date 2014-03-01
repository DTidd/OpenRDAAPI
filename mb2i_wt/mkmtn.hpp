/******************************************************************************
				MAKE MAINTAINMASTER FUNCTIONS HEADER
					mkmtn.h
******************************************************************************/
#ifndef MKMTN_H
#define MKMTN_H
#include<mix.hpp>

#define addMaintainMasterButtonField(a,b,c)	xaddMaintainMasterButtonField(a,b,c,__LINE__,__FILE__)
int xaddMaintainMasterButtonField(MaintainButton *,char *,char *,int,char *);
#define addMaintainMasterButton(a,b,c,d,e,f,g,h,i,j,k,l,m)	xaddMaintainMasterButton(a,b,c,d,e,f,g,h,i,j,k,l,m,__LINE__,__FILE__)
int xaddMaintainMasterButton(MaintainMaster *,char *,char *,char *,char *,
	char *,char *,char *,short,char,APPlib *,int,ButtonField *,int,char *);
#define addActionStruct(a,b,c,d)	xaddActionStruct(a,b,c,d,__LINE__,__FILE__)
int xaddActionStruct(MTNAction *,char *,char *,int,int,char *);
#define addMaintainMasterAction(a,b)	xaddMaintainMasterAction(a,b,__LINE__,__FILE__)
int xaddMaintainMasterAction(MaintainMaster *,char *,int,char *);
#define addSubordinateWrite(a,b,c,d)	xaddSubordinateWrite(a,b,c,d,__LINE__,__FILE__)
int xaddSubordinateWrite(RDAsubord *,char *,char *,char *,int,char *);
#define addSubordinateRDAconfld(a,b,c,d)	xaddSubordinateRDAconfld(a,b,c,d,__LINE__,__FILE__)
short xaddSubordinateRDAconfld(RDAsubord *,char *,char *,char,int,char *);
#define addMaintainMasterSubordinate(a,b,c,d,e,f,g,h,i)	xaddMaintainMasterSubordinate(a,b,c,d,e,f,g,h,i,__LINE__,__FILE__)
int xaddMaintainMasterSubordinate(MaintainMaster *,char *,char *,int,char *,char *,char *,char *,char,int,char *);
#define addSupportingRDAconfld(a,b,c,d)	xaddSupportingRDAconfld(a,b,c,d,__LINE__,__FILE__)
short xaddSupportingRDAconfld(RDAsearch *,char *,char *,char,int,char *);
#define addMaintainMasterSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)	xaddMaintainMasterSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m,FALSE,n,o,__LINE__,__FILE__)
#define addMaintainMasterSupportingADV(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	xaddMaintainMasterSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,__LINE__,__FILE__)
int xaddMaintainMasterSupporting(MaintainMaster *,char *,char *,int,short,RDAconfld *,char *,char *,char,char *,char *,char *,char *,char,char *,char *,int,char *);
#define addMaintainMasterWriteField(a,b,c,d)	xaddMaintainMasterWriteField(a,b,c,d,__LINE__,__FILE__)
int xaddMaintainMasterWriteField(PowerWrite *,char *,int,char *,int,char *);
#define addMaintainMasterWrite(a,b,c,d,e,f,g)	xaddMaintainMasterWrite(a,b,c,d,e,f,g,__LINE__,__FILE__)
int xaddMaintainMasterWrite(short,MaintainMaster *,char *,char *,char *,int,PowerWriteField *,int,char *);
#define RDAMaintainMasterNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v)	xRDAMaintainMasterNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,__LINE__,__FILE__)
MaintainMaster *xRDAMaintainMasterNEW(char *,char *,char *,int,char *,char *,
	char *,char *,char *,int,int,int,int,char *,char *,
	char *,char *,char *,short,char,char,char *,int,char *);
char *fixmtnname(char *);
void makemtncode(MaintainMaster *,FILE *,char *);

#endif /* MKMTN_H */
