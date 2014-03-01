/******************************************************************************
				FIN FUNCTIONS HEADER
					fin.h
******************************************************************************/

#ifndef FIN_H
#define FIN_H
#include <cstdio>
#include <mix.hpp>

#define ANNUALBUDGET 0
#define MONTHLYBUDGET 1
extern char *FISCALPERIOD[];
extern char *FISCALPERIODLabel[];
extern char *FISCALPERIODAbbr[];

struct RDAfinmgts
{
	char budget;
	char month;
};
typedef struct RDAfinmgts RDAfinmgt;

#define getfinmgtbin(x)	xgetfinmgtbin(x,__LINE__,__FILE__)
short xgetfinmgtbin(RDAfinmgt *,int,char *);
#define writefinmgtbin(x)	xwritefinmgtbin(x,__LINE__,__FILE__)
short xwritefinmgtbin(RDAfinmgt *,int,char *);
#define RDAfinmgtNEW()	xRDAfinmgtNEW(__LINE__,__FILE__)
RDAfinmgt *xRDAfinmgtNEW(int,char *);
#define free_finmgt(x)	{xfree_finmgt(x,__LINE__,__FILE__);x=NULL;}
void xfree_finmgt(RDAfinmgt *,int,char *);
#define testapprotot(f,a,m) xtestapprotot(f,a,m,__LINE__,__FILE__)
short xtestapprotot(short,double,char *,int,char *);
/* xtestapprotot function test the appropriations total for an account 
	against an Expenditure or Encumbrance value according to the
	year or month total.
	args:	#1(f)->file number with keys set already.
		#2(a)->amount to test against appropriations total.
		#3(m)->month. NULL if(finsetup==ANNUALBUDGET)
			else MONTH STRING if(finsetup==MONTHLYBUDGET)
*/
#define GETBALREF(x,y,z) xGETBALREF(x,y,z,NULL,NULL,__LINE__,__FILE__)
#define ADVGETBALREF(x,y,z,a,b) xGETBALREF(x,y,z,a,b,__LINE__,__FILE__)
char *xGETBALREF(short,char *,short,void (*)(...),void *,int,char *);
#define CHECKACCT(a,b,c,d,e,f) xCHECKACCT(a,b,c,d,e,f,0,NULL,NULL,__LINE__,__FILE__)
#define CHECKACCT2(a,b,c,d,e,f,g) xCHECKACCT(a,b,c,d,e,f,0,NULL,NULL,__LINE__,__FILE__)
#define ADVCHECKACCT(a,b,c,d,e,f,g,h) xCHECKACCT(a,b,c,d,e,f,0,g,h,__LINE__,__FILE__)
#define ADVCHECKACCT2(a,b,c,d,e,f,g,h,i) xCHECKACCT(a,b,c,d,e,f,g,h,i,__LINE__,__FILE__)
char *xCHECKACCT(short,short,char *,short,short,int,short,void (*)(...),void *,int,char *);
#define GetFinmgtSetup(a,b,c)	xGetFinmgtSetup(a,b,c,__LINE__,__FILE__)
short xGetFinmgtSetup(RDAfinmgt *,void (*)(void *),void *,int,char *);
#define updateundist(a,b,c,d,e,f,g,h,i,j,k,l)	xupdatefinyear(a,b,c,d,e,f,g,h,i,j,k,0,l,NULL,NULL,__LINE__,__FILE__)
#define ADVupdateundist(a,b,c,d,e,f,g,h,i,j,k,l,m,n)	xupdatefinyear(a,b,c,d,e,f,g,h,i,j,k,0,l,m,n,__LINE__,__FILE__)
#define updatedist(a,b,c,d,e,f,g,h,i,j,k,l)	xupdatefinyear(a,b,c,d,e,f,g,h,i,j,k,1,l,NULL,NULL,__LINE__,__FILE__)
#define ADVupdatedist(a,b,c,d,e,f,g,h,i,j,k,l,m,n)	xupdatefinyear(a,b,c,d,e,f,g,h,i,j,k,1,l,m,n,__LINE__,__FILE__)
void xupdatefinyear(short,char *,char *,char *,char *,short,short,int,short,
	char *,char *,short,double,void (*)(...),void *,int,char *);
/*Updates undistributed amounts or distributed amounts in f/m record. 
	arguements: source file number,account field name (exp, rev, or NULL), 
	debit field name(b/s or NULL),credit field name(b/s or NULL), 
	payable field name(b/s or NULL),account year file number(exp or rev), 
	b/s year file number, year id(int),deftype(short), 
	amount type (EXPENDITURES, REVENUE, etc. for exp & rev
	only), Period(string), Amount.*/
#define UpdateFinMgtYear(a,b,c,d,e,f,g,h,i,j,k,l) xUpdateFinMgtYear(a,b,c,d,e,f,g,h,i,j,k,l,NULL,NULL,__LINE__,__FILE__)
#define ADVUpdateFinMgtYear(a,b,c,d,e,f,g,h,i,j,k,l,m,n) xUpdateFinMgtYear(a,b,c,d,e,f,g,h,i,j,k,l,m,n,__LINE__,__FILE__)
#define UpdateUnDistFinMgtYear(b,c,d,e,f,g,h,i,j,k,l) xUpdateFinMgtYear(b,c,d,e,f,g,h,i,j,k,FALSE,l,NULL,NULL,__LINE__,__FILE__)
#define ADVUpdateUnDistFinMgtYear(b,c,d,e,f,g,h,i,j,k,l,m,n) xUpdateFinMgtYear(b,c,d,e,f,g,h,i,j,k,FALSE,l,m,n,__LINE__,__FILE__)
#define UpdateDistFinMgtYear(b,c,d,e,f,g,h,i,j,k,l) xUpdateFinMgtYear(b,c,d,e,f,g,h,i,j,k,TRUE,l,NULL,NULL,__LINE__,__FILE__)
#define ADVUpdateDistFinMgtYear(b,c,d,e,f,g,h,i,j,k,l,m,n) xUpdateFinMgtYear(b,c,d,e,f,g,h,i,j,k,TRUE,l,m,n,__LINE__,__FILE__)
void xUpdateFinMgtYear(short,char *,char *,char *,char *,short,short,
	int,char *,char *,short,double,void (*)(...),void *,int,char *);
#define CHECKACCTW(a,b,c,d,e,f,g,h,i)	xCHECKACCTW(a,b,c,d,e,f,g,h,i,NULL,NULL,__LINE__,__FILE__)
#define ADVCHECKACCTW(a,b,c,d,e,f,g,h,i,j,k)	xCHECKACCTW(a,b,c,d,e,f,g,h,i,j,k,__LINE__,__FILE__)
short xCHECKACCTW(RDArsrc *,char *,char *,short,short,short,short,char *,short,
	void (*)(...),void *,int,char *);
/*Call's CHECKACCT and displayes warning dialog. arguements: screen resources,
	account field name, year field name, definition type(short),year file
	number, account file number, account type(short), error description (ie
	"INVALID DEBIUT CODE!"), short TRUE/FALSE skip NULL strings.*/
#endif /* FIN_H */

