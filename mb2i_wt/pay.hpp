/******************************************************************************
				PAYROLL FUNCTIONS HEADER
					       pay.h
******************************************************************************/

#ifndef PAY_H
#define PAY_H
#include <cstdio>
#include <mix.hpp>

extern char Pay_Balance_of_Contract,Dump_Deferred_Pay_Balance;

struct RDApayrolls
{
	char manage_earnings;
	short use_payroll_fund;
	char paysel_audit;
	char paynet_audit;
	char paymts_audit;
	char subsel_audit;
	int direct_deposit_type;
	int perid;
	int perid_resource;
	int paycheck_form_type;
	int dirdep_form_type;
	char *ein;
	char payjams_rollover;
};
typedef struct RDApayrolls RDApayroll;

#define getpayrollbin(x)	xgetpayrollbin(x,__LINE__,__FILE__)
short xgetpayrollbin(RDApayroll *,int,char *);
#define writepayrollbin(x)	xwritepayrollbin(x,__LINE__,__FILE__)
short xwritepayrollbin(RDApayroll *,int,char *);
#define RDApayrollNEW()	xRDApayrollNEW(__LINE__,__FILE__)
RDApayroll *xRDApayrollNEW(int,char *);
#define free_payroll(x)	{xfree_payroll(x,__LINE__,__FILE__);x=NULL;}
void xfree_payroll(RDApayroll *,int,char *);
#define GetPayrollSetup(a,b,c)	xGetPayrollSetup(a,b,c,__LINE__,__FILE__)
short xGetPayrollSetup(RDApayroll *,void (*)(...),void *,int,char *);
#endif /* PAY_H */
