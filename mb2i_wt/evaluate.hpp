
/******************************************************************************
				EVALUATE FUNCTIONS HEADER
					evaluate.h
******************************************************************************/
#ifndef EVALUATE_H
#define EVALUATE_H

#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <misc.hpp>

#include <cstdio>
#include <cstdlib>
#include <limits.h>


#ifndef WIN32
#include <values.h>
#endif

#define NEW_EVAL
extern char ComputeVirtualType;

struct RDAevals
{
	char *token;
	char *prog;
	int type;
};
typedef struct RDAevals RDAeval;

struct RDAresults
{
	short which;
	RDAeval *eval;
	union rda_field_types value;
#ifdef ADV_EVAL
	short fieldtype;
	short subtype;
#endif
#ifdef NEW_EVAL
	unsigned dlen;
#endif
};
typedef struct RDAresults RDAresult;

extern char *func;

#ifdef ADV_EVAL
#define ADVEVALUATEstr(u,v,x,y,z)	xEVALUATEstr(u,v,x,y,z,TRUE,__LINE__,__FILE__)
#define ADVEVALUATEbol(u,v,x,y,z)	xEVALUATEbol(u,v,x,y,z,TRUE,__LINE__,__FILE__)
#define ADVEVALUATEdbl(u,v,x,y,z)	xEVALUATEdbl(u,v,x,y,z,TRUE,__LINE__,__FILE__)
#define ADVEVALUATEsht(u,v,x,y,z)	xEVALUATEsht(u,v,x,y,z,TRUE,__LINE__,__FILE__)
#define ADVEVALUATEint(u,v,x,y,z)	xEVALUATEint(u,v,x,y,z,TRUE,__LINE__,__FILE__)
#define EVALUATEstr(x,y,z)	xEVALUATEstr(x,y,z,VARIABLETEXT,0,TRUE,__LINE__,__FILE__)
#define EVALUATEbol(x,y,z)	xEVALUATEbol(x,y,z,BOOLNS,0,TRUE,__LINE__,__FILE__)
#define EVALUATEdbl(x,y,z)	xEVALUATEdbl(x,y,z,SDECIMALV,0,TRUE,__LINE__,__FILE__)
#define EVALUATEsht(x,y,z)	xEVALUATEsht(x,y,z,SSHORTV,0,TRUE,__LINE__,__FILE__)
#define EVALUATEint(x,y,z)	xEVALUATEint(x,y,z,SLONGV,0,TRUE,__LINE__,__FILE__)
#else
#define EVALUATEstr(x,y,z)	xEVALUATEstr(x,y,z,TRUE,__LINE__,__FILE__)
#define EVALUATEbol(x,y,z)	xEVALUATEbol(x,y,z,TRUE,__LINE__,__FILE__)
#define EVALUATEdbl(x,y,z)	xEVALUATEdbl(x,y,z,TRUE,__LINE__,__FILE__)
#define EVALUATEsht(x,y,z)	xEVALUATEsht(x,y,z,TRUE,__LINE__,__FILE__)
#define EVALUATEint(x,y,z)	xEVALUATEint(x,y,z,TRUE,__LINE__,__FILE__)
#define PP_EVALUATEstr(x,y,z)	xEVALUATEstr(x,y,z,FALSE,__LINE__,__FILE__)
#define PP_EVALUATEbol(x,y,z)	xEVALUATEbol(x,y,z,FALSE,__LINE__,__FILE__)
#define PP_EVALUATEdbl(x,y,z)	xEVALUATEdbl(x,y,z,FALSE,__LINE__,__FILE__)
#define PP_EVALUATEsht(x,y,z)	xEVALUATEsht(x,y,z,FALSE,__LINE__,__FILE__)
#define PP_EVALUATEint(x,y,z)	xEVALUATEint(x,y,z,FALSE,__LINE__,__FILE__)
#endif

short isdelim(char);
short is_in(char,char *);
short iswhite(char);
void subfielddata(char **,short *);
void subkeyword(char *);
void presortsubkeyword(char *);
short get_info(char **,int *,char **,void (*)(...),void *,short);
#define stripmodulename(a)	xstripmodulename(a,__LINE__,__FILE__)
char *xstripmodulename(char *,int,char *);
#define stripfilename(x)	xstripfilename(x,__LINE__,__FILE__)
char *xstripfilename(char *,int,char *);
#define stripfieldname(x)	xstripfieldname(x,__LINE__,__FILE__)
char *xstripfieldname(char *,int,char *);
#define OpenRDA_translate(x,y,z,a)	xtranslate(x,y,z,a,__LINE__,__FILE__)
char *xtranslate(char *,void (*)(...),void *,short,int,char *);
#define RDAevalNEW(x)	xRDAevalNEW(x,__LINE__,__FILE__)
RDAeval *xRDAevalNEW(char *,int,char *);
#define FreeRDAeval(x)	{xFreeRDAeval(x,__LINE__,__FILE__);x=NULL;}
void xFreeRDAeval(RDAeval *,int,char *);
#define SEERDAresult(x)	xSEERDAresult(x,__LINE__,__FILE__)
void xSEERDAresult(RDAresult *,int,char *);
#define Performeval(x,y,a,b)	xPerformeval(x,y,a,b,__LINE__,__FILE__)
RDAresult *xPerformeval(short,char *,void (*)(...),void *,int,char *);
#define RDAresultNEW(x,y)	xRDAresultNEW(x,y,__LINE__,__FILE__)
RDAresult *xRDAresultNEW(short,RDAeval *,int,char *);
#define FreeRDAresult(x)	{xFreeRDAresult(x,__LINE__,__FILE__);x=NULL;}
void xFreeRDAresult(RDAresult *,int,char *);
#define get_exp(a,b,c)	xget_exp(a,b,c,__LINE__,__FILE__)
void xget_exp(RDAresult *,void (*)(...),void *,int,char *);
void level2(RDAresult *,void (*)(...),void *),level3(RDAresult *,void (*)(...),void *),level4(RDAresult *,void (*)(...),void *);
void level5(RDAresult *,void (*)(...),void *),level6(RDAresult *,void (*)(...),void *),level7(RDAresult *,void (*)(...),void *);
#define get_token(a)	xget_token(a,__LINE__,__FILE__)
void xget_token(RDAresult *,int,char *);
#define StepThroughIF(x,y,a,b)	xStepThroughIF(x,y,a,b,__LINE__,__FILE__)
RDAresult *xStepThroughIF(short,char *,void (*)(...),void *,int,char *);
#ifdef ADV_EVAL
double xEVALUATEdbl(char *,void (*)(...),void *,short,short,short,int,char *);
short xEVALUATEsht(char *,void (*)(...),void *,short,short,short,int,char *);
int xEVALUATEint(char *,void (*)(...),void *,short,short,short,int,char *);
char *xEVALUATEstr(char *,void (*)(...),void *,short,short,short,int,char *);
short xEVALUATEbol(char *,void (*)(...),void *,short,short,short,int,char *);
#else
#endif
double xEVALUATEdbl(char *,void (*)(...),void *,short,int,char *);
short xEVALUATEsht(char *,void (*)(...),void *,short,int,char *);
int xEVALUATEint(char *,void (*)(...),void *,short,int,char *);
char *xEVALUATEstr(char *,void (*)(...),void *,short,int,char *);
short xEVALUATEbol(char *,void (*)(...),void *,short,int,char *);
#define PP_translate(a)	xPP_translate(a,__LINE__,__FILE__)
char *xPP_translate(char *,int,char *);
void sub_quotes(char **);
#define COMPUTEGenericCounter(a)	xCOMPUTEGenericCounter(a,__LINE__,__FILE__)
int xCOMPUTEGenericCounter(char *,int,char *);
char HexToAscii(char,char);
#endif

