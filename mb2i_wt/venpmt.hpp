/******************************************************************************
				VENPMT FUNCTIONS HEADER
					venpmt.h
******************************************************************************/

#ifndef VENPMT_H
#define VENPMT_H
#include <mix.hpp>

struct RDAvenpmts
{
	char use_venpmt_fund;
	char *ein;
	char use_fiscal_summary;

	char Use_Taxes;

	char Use_Tax1;
	char Sep_Tax1_Vendor;
	char *Default_Tax1_Vendor;
	char *Default_Tax1_Address;
	double Tax1_Rate;

	char Use_Tax2;
	char Sep_Tax2_Vendor;
	char *Default_Tax2_Vendor;
	char *Default_Tax2_Address;
	double Tax2_Rate;

	char Use_Tax3;
	char Sep_Tax3_Vendor;
	char *Default_Tax3_Vendor;
	char *Default_Tax3_Address;
	double Tax3_Rate;

	char Use_Tax4;
	char Sep_Tax4_Vendor;
	char *Default_Tax4_Vendor;
	char *Default_Tax4_Address;
	double Tax4_Rate;

	char Use_Tax5;
	char Sep_Tax5_Vendor;
	char *Default_Tax5_Vendor;
	char *Default_Tax5_Address;
	double Tax5_Rate;

	char *admin_fee;

	char use_fiscal_months;
};
typedef struct RDAvenpmts RDAvenpmt;

#define getvenpmtbin(x)	xgetvenpmtbin(x,__LINE__,__FILE__)
short xgetvenpmtbin(RDAvenpmt *,int,char *);
#define writevenpmtbin(x)	xwritevenpmtbin(x,__LINE__,__FILE__)
short xwritevenpmtbin(RDAvenpmt *,int,char *);
#define RDAvenpmtNEW()	xRDAvenpmtNEW(__LINE__,__FILE__)
RDAvenpmt *xRDAvenpmtNEW(int,char *);
#define free_venpmt(x)	{xfree_venpmt(x,__LINE__,__FILE__);x=NULL;}
void xfree_venpmt(RDAvenpmt *,int,char *);
#define GetVenpmtSetup(a,b,c)	xGetVenpmtSetup(a,b,c,__LINE__,__FILE__)
short xGetVenpmtSetup(RDAvenpmt *,void (*)(),void *,int,char *);
#endif /* VENPMT_H */
