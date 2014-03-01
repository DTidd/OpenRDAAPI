#ifndef REQMGT_H
#define REQMGT_H
#include <cstdio>
#include <cstdlib>
#include <mix.hpp>
#include <fin.hpp>

#define UpdateCatalogue(a,b,c,d,e,f,g,h) xUpdateCatalogue(a,b,c,d,e,f,g,h,NULL,NULL,__LINE__,__FILE__)
#define ADVUpdateCatalogue(a,b,c,d,e,f,g,h,i,j) xUpdateCatalogue(a,b,c,d,e,f,g,h,i,j,__LINE__,__FILE__)
short xUpdateCatalogue(short,short,char *,char *,char *,char *,char *,char *,void (*)(...),void *,int,char *);

/* Note any changes to these structures requires a change the the Add Requisitions
   program, especially in the Psydo-Define list functions that are coded with the
   exact current definition of these structures!!!  */

struct ReqAcctStructs
{
	int accno;
	short deftype;
	char *account;
	char *debit;
	char *credit;
	char prorate;
	double percentage;
	double amount;
	double enc;
	double pre_enc;
	char *defdebit;
	char *defcredit;
	char *defpayable;
	char *defbankid;
	short cdeftype;
	char *defccash;
	char *defcdfrom;

};
typedef struct ReqAcctStructs ReqAcctStruct;
struct REQACCS
{
	int num;
	ReqAcctStruct *a;
};
typedef struct REQACCS REQACC;

#define SeeReqAcctStruct(a)	xSeeReqAcctStruct(a,__LINE__,__FILE__)
void xSeeReqAcctStruct(ReqAcctStruct *,int,char *);
#define SeeREQACC(a)	xSeeREQACC(a,__LINE__,__FILE__)
void xSeeREQACC(REQACC *,int,char *);
#define REQACCNEW()	xREQACCNEW(__LINE__,__FILE__)
REQACC *xREQACCNEW(int,char *);
#define AddReqAcctStruct(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	xAddReqAcctStruct(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,__LINE__,__FILE__)
void xAddReqAcctStruct(REQACC *,int,short,char *,
	char *,char *,char,double,double,char *,char *,char *,char *,short,char *,char *,int,char *);
#define FreeREQACC(a)	{xFreeREQACC(a,__LINE__,__FILE__);a=NULL;}
void xFreeREQACC(REQACC *,int,char *);
#define GetREQACC(a,b,c,d)	xGetREQACC(a,b,c,d,NULL,NULL,__LINE__,__FILE__)
#define ADVGetREQACC(a,b,c,d,e,f)	xGetREQACC(a,b,c,d,e,f,__LINE__,__FILE__)
REQACC *xGetREQACC(short,char *,char *,int,void (*)(...),void *,int,char *);
#define ADVCHGCHECKREQACC(a,b,c,d,e,f,g)	xCHECKREQACC(a,b,c,d,e,TRUE,f,g,__LINE__,__FILE__)
#define CHGCHECKREQACC(a,b,c,d,e)	xCHECKREQACC(a,b,c,d,e,TRUE,NULL,NULL,__LINE__,__FILE__)
#define ADVCHECKREQACC(a,b,c,d,e,f,g)	xCHECKREQACC(a,b,c,d,e,FALSE,f,g,__LINE__,__FILE__)
#define CHECKREQACC(a,b,c,d,e)	xCHECKREQACC(a,b,c,d,e,FALSE,NULL,NULL,__LINE__,__FILE__)
short xCHECKREQACC(REQACC *,short,int,short,RDAfinmgt *,short,void (*)(...),void *,int,char *);
#define ADVREQACCDIFF_UPDATE_UNDIST(a,b,c,d,e,f,g) xREQACC_UPDATE_UNDIST(a,b,c,d,e,0,TRUE,f,g,__LINE__,__FILE__)
#define REQACCDIFF_UPDATE_UNDIST(a,b,c,d,e) xREQACC_UPDATE_UNDIST(a,b,c,d,e,0,TRUE,NULL,NULL,__LINE__,__FILE__)
#define ADVREQACCDIFF_BACKOUT_UNDIST(a,b,c,d,e,f,g) xREQACC_UPDATE_UNDIST(a,b,c,d,e,(-1),TRUE,f,g,__LINE__,__FILE__)
#define REQACCDIFF_BACKOUT_UNDIST(a,b,c,d,e) xREQACC_UPDATE_UNDIST(a,b,c,d,e,(-1),TRUE,NULL,NULL,__LINE__,__FILE__)
#define ADVREQACCDIFF_CHANGE_UNDIST(a,b,c,d,e,f,g,h) xREQACC_UPDATE_UNDIST(a,b,c,d,e,f,TRUE,g,h,__LINE__,__FILE__)
#define REQACCDIFF_CHANGE_UNDIST(a,b,c,d,e,f) xREQACC_UPDATE_UNDIST(a,b,c,d,e,f,TRUE,NULL,NULL,__LINE__,__FILE__)
#define ADVREQACC_UPDATE_UNDIST(a,b,c,d,e,f,g) xREQACC_UPDATE_UNDIST(a,b,c,d,e,0,FALSE,f,g,__LINE__,__FILE__)
#define REQACC_UPDATE_UNDIST(a,b,c,d,e) xREQACC_UPDATE_UNDIST(a,b,c,d,e,0,FALSE,NULL,NULL,__LINE__,__FILE__)
#define ADVREQACC_BACKOUT_UNDIST(a,b,c,d,e,f,g) xREQACC_UPDATE_UNDIST(a,b,c,d,e,(-1),FALSE,f,g,__LINE__,__FILE__)
#define REQACC_BACKOUT_UNDIST(a,b,c,d,e) xREQACC_UPDATE_UNDIST(a,b,c,d,e,(-1),FALSE,NULL,NULL,__LINE__,__FILE__)
#define ADVREQACC_CHANGE_UNDIST(a,b,c,d,e,f,g,h) xREQACC_UPDATE_UNDIST(a,b,c,d,e,f,FALSE,g,h,__LINE__,__FILE__)
#define REQACC_CHANGE_UNDIST(a,b,c,d,e,f) xREQACC_UPDATE_UNDIST(a,b,c,d,e,f,FALSE,NULL,NULL,__LINE__,__FILE__)
void xREQACC_UPDATE_UNDIST(REQACC *,int,short,short,short,short,short,void (*)(...),void *,int,char *);
#define ComputeREQACC(a,b)	xComputeREQACC(a,b,__LINE__,__FILE__);
void xComputeREQACC(REQACC *,double,int,char *);
#define writeAddReqDefAcct(a)	xwriteAddReqDefAcct(a,__LINE__,__FILE__)
short xwriteAddReqDefAcct(REQACC *,int,char *);
#define getAddReqDefAcct(a)	xgetAddReqDefAcct(a,__LINE__,__FILE__)
short xgetAddReqDefAcct(REQACC *,int,char *);
#define MoveREQACC(a)	xMoveREQACC(a,__LINE__,__FILE__)
void xMoveREQACC(REQACC *,int,char *);
#endif /* REQMGT_H */
