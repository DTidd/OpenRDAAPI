#ifndef INVCTL_H 
#define INVCTL_H

#include <cstdio>
#include <cstdlib>
#include <mix.hpp>
#include <fin.hpp>

struct InvAcctStructs
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
};
typedef struct InvAcctStructs InvAcctStruct;
struct INVACCS
{
	int num;
	InvAcctStruct *a;
};
typedef struct INVACCS INVACC;

/*  The *_ISSUE_* functions have the same arguments as the first 14 arguments of
	UpdateInvIssue listed below.  */
#define UPDATE_ISSUE_REQUESTED(a,b,c,d,e,f,g,h,i,j,k,l,m,n,s,r) xUpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,0,(-1),s,r,__LINE__,__FILE__)
#define BACKOUT_ISSUE_REQUESTED(a,b,c,d,e,f,g,h,i,j,k,l,m,n,s,r) xUpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,0,0,s,r,__LINE__,__FILE__)
#define UPDATE_ISSUE_RESERVED(a,b,c,d,e,f,g,h,i,j,k,l,m,n,s,r) xUpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,1,(-1),s,r,__LINE__,__FILE__)
#define BACKOUT_ISSUE_RESERVED(a,b,c,d,e,f,g,h,i,j,k,l,m,n,s,r) xUpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,1,0,s,r,__LINE__,__FILE__)
#define UPDATE_ISSUE_PROCESSED(a,b,c,d,e,f,g,h,i,j,k,l,m,n,s,r) xUpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,2,(-1),s,r,__LINE__,__FILE__)
#define BACKOUT_ISSUE_PROCESSED(a,b,c,d,e,f,g,h,i,j,k,l,m,n,s,r) xUpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,2,0,s,r,__LINE__,__FILE__)
#define UpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,s,r) xUpdateInvIssue(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,s,r,__LINE__,__FILE__)
/* UpdateInvIssue has 16 arguments as follows:
	a. INVITM file pointer (short)
	b. INVILOC file pointer (short) 
	c. INVISLC file pointer (short) 
	d. INVITMY file pointer (short)
	e. INVILOCY file pointer (short) 
	f. INVISLCY file pointer (short) 
	g. CATEGORY IDENTIFICATION (char *)
	h. ITEM IDENTIFICATION (char *)
	i. LOCATION IDENTIFICATION (char *)
	j. SUB-LOCATION IDENTIFICATION (char *)
	k. FISCAL YEAR (int)
	l. PROCESSING MONTH (char *)
	m. QUANTITY (double)  	
	n. VALUE (double)
	o. TYPE (short)
		0 - Requested
		1 - Reserved
		2 - Processed
	p. OP (short)
		  0  -  Addition
		(-1) -  Subtraction 
	s. Substitute Data Function
	r. Substitute Data Function argment resource (RDArsrc *)
*/
void xUpdateInvIssue(short,short,short,short,short,short,char *,char *,char *,char *,
                int,char *,double,double,short,short,void (*)(),void *,int,char *);

/*  The *_RECEIVE_* functions have the same arguments as the first 9 arguments of 
	UpdateInvNonYear listed below.  */
#define UPDATE_RECEIVE_REQUESTED(a,b,c,d,e,f,g,h,i,s,r) xUpdateInvNonYear(a,b,c,d,e,f,g,h,i,0,0,s,r,__LINE__,__FILE__)
#define BACKOUT_RECEIVE_REQUESTED(a,b,c,d,e,f,g,h,i,s,r) xUpdateInvNonYear(a,b,c,d,e,f,g,h,i,0,(-1),s,r,__LINE__,__FILE__)
#define UPDATE_RECEIVE_RESERVED(a,b,c,d,e,f,g,h,i,s,r) xUpdateInvNonYear(a,b,c,d,e,f,g,h,i,1,0,s,r,__LINE__,__FILE__)
#define BACKOUT_RECEIVE_RESERVED(a,b,c,d,e,f,g,h,i,s,r) xUpdateInvNonYear(a,b,c,d,e,f,g,h,i,1,(-1),s,r,__LINE__,__FILE__)
#define UPDATE_RECEIVE_PROCESSED(a,b,c,d,e,f,g,h,i,s,r) xUpdateInvNonYear(a,b,c,d,e,f,g,h,i,2,0,s,r,__LINE__,__FILE__)
#define BACKOUT_RECEIVE_PROCESSED(a,b,c,d,e,f,g,h,i,s,r) xUpdateInvNonYear(a,b,c,d,e,f,g,h,i,2,(-1),s,r,__LINE__,__FILE__)
#define UpdateInvNonYear(a,b,c,d,e,f,g,h,i,j,k,s,r) xUpdateInvNonYear(a,b,c,d,e,f,g,h,i,j,k,s,r,__LINE__,__FILE__)
/* UpdateInvNonYear has 11 arguments as follows:
	a. INVITM file pointer (short)
	b. INVILOC file pointer (short) 
	c. INVISLC file pointer (short) 
	d. CATEGORY IDENTIFICATION (char *)
	e. ITEM IDENTIFICATION (char *)
	f. LOCATION IDENTIFICATION (char *)
	g. SUB-LOCATION IDENTIFICATION (char *)
	h. QUANTITY (double)  	
	i. VALUE (double)
	j. TYPE (short)
		0 - Requested
		1 - Reserved
		2 - Processed
	k. OP (short)
		  0  -  Addition
		(-1) -  Subtraction 
	s. Substitute Data Function
	r. Substitute Data Function argument resource (RDArsrc *)
*/
void xUpdateInvNonYear(short,short,short,char *,char *,char *,char *,double,double,short,short,void (*)(),void *,int,char *);
#define CheckInvItmIssue(a,b,c,d,e,f,g,s,r)	xCheckInvItmIssue(a,b,c,d,e,f,g,s,r,__LINE__,__FILE__)
char *xCheckInvItmIssue(short,char *,char *,char *,char *,double,double,void (*)(),void *,int,char *);
#define SeeInvAcctStruct(a)	xSeeInvAcctStruct(a,__LINE__,__FILE__)
void xSeeInvAcctStruct(InvAcctStruct *,int,char *);
#define SeeINVACC(a)	xSeeINVACC(a,__LINE__,__FILE__)
void xSeeINVACC(INVACC *,int,char *);
#define INVACCNEW()	xINVACCNEW(__LINE__,__FILE__)
INVACC *xINVACCNEW(int,char *);
#define AddInvAcctStruct(a,b,c,d,e,f,g,h,i)	xAddInvAcctStruct(a,b,c,d,e,f,g,h,i,__LINE__,__FILE__)
void xAddInvAcctStruct(INVACC *,int,short,char *,
	char *,char *,char,double,double,int,char *);
#define FreeINVACC(a)	xFreeINVACC(a,__LINE__,__FILE__)
void xFreeINVACC(INVACC *,int,char *);
#define GetINVACC(a,b,c,s,r)	xGetINVACC(a,b,c,s,r,__LINE__,__FILE__)
INVACC *xGetINVACC(short,char *,int,void (*)(),void *,int,char *);
#define CHECKINVACC(a,b,c,d,e,s,r)	xCHECKINVACC(a,b,c,d,e,s,r,__LINE__,__FILE__)
short xCHECKINVACC(INVACC *,short,int,short,RDAfinmgt *,void (*)(),void *,int,char *);
#define INVACC_UPDATE_UNDIST(a,b,c,d,e,s,r) xINVACC_UPDATE_UNDIST(a,b,c,d,e,0,s,r,__LINE__,__FILE__)
#define INVACC_BACKOUT_UNDIST(a,b,c,d,e,s,r) xINVACC_UPDATE_UNDIST(a,b,c,d,e,(-1),s,r,__LINE__,__FILE__)
#define INVACC_CHANGE_UNDIST(a,b,c,d,e,f,s,r) xINVACC_UPDATE_UNDIST(a,b,c,d,e,f,s,r,__LINE__,__FILE__)
void xINVACC_UPDATE_UNDIST(INVACC *,int,short,short,short,short,void (*)(),void *,int,char *);
#define ComputeINVACC(a,b)	xComputeINVACC(a,b,__LINE__,__FILE__);
void xComputeINVACC(INVACC *,double,int,char *);
#define writeAddInvDefAcct(a)   xwriteAddInvDefAcct(a,__LINE__,__FILE__)
short xwriteAddInvDefAcct(INVACC *,int,char *);
#define getAddInvDefAcct(a)     xgetAddInvDefAcct(a,__LINE__,__FILE__)
short xgetAddInvDefAcct(INVACC *,int,char *);
#define UpdateTotalItems(a,b,c,d,e,f,g,h,i,j,k,l,m,s,r)	xUpdateTotalItems(a,b,c,d,e,f,g,h,i,j,k,l,m,s,r,__LINE__,__FILE__)
void xUpdateTotalItems(short,short,char *,char *,char *,double,double,double,double,double,double,double,double,void (*)(),void *,int,char *);

#endif /* INVCTL_H */
