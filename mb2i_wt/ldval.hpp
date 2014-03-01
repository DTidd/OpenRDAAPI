#ifndef LDVAL_H
#define LDVAL_H

#include<mix.hpp>


struct LoadValues
{
	RDArsrc *r;
	RDArsrc *val;
	char *fieldname;
};
typedef struct LoadValues LoadValue;

extern char *OpenRDA_operand[16];
extern char *OpenRDA_grouper[7];
extern char *OpenRDA_controls[50];
#define FreeLoadValue(a)	xFreeLoadValue(a,__LINE__,__FILE__)
void xFreeLoadValue(LoadValue *,int,char *);
#define LoadValueNew(a,b,c)	xLoadValueNew(a,b,c,__LINE__,__FILE__)
LoadValue *xLoadValueNew(RDArsrc *,RDArsrc *,char *,int,char *);
void quitldval(RDArsrc *,LoadValue *);
void okldval(RDArsrc *,LoadValue *);
void loadoperand(RDArsrc *,char *);
void loadgrouper(RDArsrc *,char *);
void loadcontrol(RDArsrc *,char *);
void loadglobals(APPlib *);
extern RDArsrc *ldvalrsrc;
extern APPlib *loadlist;
#define AddRDAGenericSetup2APPlib(a)	xAddRDAGenericSetup2APPlib(a,__LINE__,__FILE__)
void xAddRDAGenericSetup2APPlib(APPlib *,int,char *);
#endif
