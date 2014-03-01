/******************************************************************************
				MAKE POWER ADD FUNCTIONS HEADER
					mkadd.h
******************************************************************************/
#ifndef MKPOWADD_H
#define MKPOWADD_H

#include<mix.hpp>
#include<powadd.hpp>

#define addPowerAddButtonField(a,b,c) xaddPowerAddButtonField(a,b,c,__LINE__,__FILE__)
int xaddPowerAddButtonField(MaintainButton *,char *,char *,int,char *);

#define addPowerAddButton(a,b,c,d,e,f,g,h,i,j,k,l,m) xaddPowerAddButton(a,b,c,d,e,f,g,h,i,j,k,l,m,__LINE__,__FILE__)
int xaddPowerAddButton(PowerAdd *,char *,char *,char *,char *,char *,char *,char *,short,char,APPlib *,int,ButtonField *,int,char *);

#define addPowerAddEditWdgtEditFld(a,b,c) xaddPowerAddEditWdgtEditFld(a,b,c,__LINE__,__FILE__)
int xaddPowerAddEditWdgtEditFld(AddEditWdgt *,char *,char *,int,char *);

#define addPowerAddEditWdgts(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t) xaddPowerAddEditWdgts(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,__LINE__,__FILE__)
int xaddPowerAddEditWdgts(PowerAdd *,char *,short,unsigned,int,char *,char,char *,char *,char *,int,RDAeditfld *,char *,char *,char *,char *,char,APPlib *,char,APPlib *,int,char *);

#define addPowerAddSupportingConfld(a,b,c,d) xaddPowerAddSupportingConfld(a,b,c,d,__LINE__,__FILE__)
int xaddPowerAddSupportingConfld(RDAaddsub *,char *,char *,char,int,char *);

#define addPowerAddSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m) xaddPowerAddSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m,FALSE,__LINE__,__FILE__)
#define addPowerAddSupportingADV(a,b,c,d,e,f,g,h,i,j,k,l,m,n) xaddPowerAddSupporting(a,b,c,d,e,f,g,h,i,j,k,l,m,n,__LINE__,__FILE__)
int xaddPowerAddSupporting(PowerAdd *,char *,char *,int,short,RDAconfld *,char *,char *,char,char *,char *,char *,char *,char,int,char *);

#define addPowerAddButtonField(a,b,c) xaddPowerAddButtonField(a,b,c,__LINE__,__FILE__)
int xaddPowerAddButtonField(MaintainButton *,char *,char *,int,char *);

#define addPowerAddWriteField(a,b,c,d) xaddPowerAddWriteField(a,b,c,d,__LINE__,__FILE__)
int xaddPowerAddWriteField(AddWrite *,char *,int,char *,int,char *);

#define addPowerAddWrite(a,b,c,d,e,f) xaddPowerAddWrite(a,b,c,d,e,f,__LINE__,__FILE__)
int xaddPowerAddWrite(PowerAdd *,char *,char *,char *,int,AddWriteField *,int,char *);

#define RDAPowerAddNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w) xRDAPowerAddNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,__LINE__,__FILE__)
PowerAdd *xRDAPowerAddNEW(char *,char *,char *,char *,char *,char *,int,char *,char *,char *,char *,char *,char *,char *,char *,int,MaintainButton *,int,RDAaddsub *,int,AddWrite *,int,AddEditWdgt *,int,char *);

#endif /* MKPOWADD_H */
