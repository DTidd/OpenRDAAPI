/* paydds.c - Deduction Setup */
#ifndef WIN32
#define __NAM__ "paydds.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paydds.exe"
#endif
#include <app.hpp>

#include <mkmsc.hpp>
#include <mix.hpp>
#include <ldval.hpp>
/*ADDLIB mklib */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *DeductionID=NULL,*dedtype=NULL;
static APPlib *dedlist=NULL;
RDArsrc *mainrsrc=NULL;
short payddms=(-1);
static void TBL_ANNUAL_AL_STATE_WITHHOLDING(void);
static void TBL_VIRGINIA_STATE_TAX_TABLE(void);
static void TBL_ANNUAL_FEDERAL_WITHHOLDING_MARRIED(void);
static void TBL_ANNUAL_FEDERAL_WITHHOLDING_SINGLE(void);
static void TBL_UNIFORM_LIFE_INSURANCE_TABLE(void);
static void TBL_ANNUAL_VRS_OLIP_COST(void);
static void TBL_ANNUAL_WVSTATE_SINGLE(void),TBL_ANNUAL_WVSTATE_MARRIED(void);
static void TBL_ANNUAL_NDSTATE_SINGLE(void),TBL_ANNUAL_NDSTATE_MARRIED(void);
/*
static void TBL_ANNUAL_NC_STATE_HEAD_OF_HOUSEHOLD(void);
static void TBL_ANNUAL_NC_STATE_MARRIED_WIDOWED(void);
static void TBL_ANNUAL_NC_STATE_SINGLE(void);
*/
static void TBL_ANNUAL_GEORGIA_STATE_MARRIED_ONE_INCOME_HEAD_OF_HOUSEHOLD(void);
static void TBL_ANNUAL_GEORGIA_STATE_MARRIED_SEPARATE_TWO_INCOMES(void);
static void TBL_ANNUAL_GEORGIA_STATE_SINGLE(void);
static void TBL_ANNUAL_NEW_YORK_STATE_SINGLE(void);
static void TBL_ANNUAL_NEW_YORK_STATE_MARRIED(void);

static void TBL_UNIFORM_LIFE_INSURANCE_TABLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","UNIFORM LIFE INSURANCE TABLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("Federal Uniform Life Insurance");
		scrn->title2=stralloc("Annualized Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the Annual Federal Uniform Life Insurance Rates for Current Calendar Year for calculating Imputed Income Amounts.");
		scrn->desc=stralloc("This RDA Table is used to reflect the Annual Federal Uniform Life Insurance Rates for Current Calendar Year for calculating Imputed Income Amounts.");
		scrn->inclusive=0;
		addRDATableValue(scrn,25.000000,0.000000,0.600000);
		addRDATableValue(scrn,30.000000,0.000000,0.720000);
		addRDATableValue(scrn,35.000000,0.000000,0.960000);
		addRDATableValue(scrn,40.000000,0.000000,1.080000);
		addRDATableValue(scrn,45.000000,0.000000,1.200000);
		addRDATableValue(scrn,50.000000,0.000000,1.800000);
		addRDATableValue(scrn,55.000000,0.000000,2.760000);
		addRDATableValue(scrn,60.000000,0.000000,5.160000);
		addRDATableValue(scrn,65.000000,0.000000,7.920000);
		addRDATableValue(scrn,70.000000,0.000000,15.240000);
		addRDATableValue(scrn,99999999.999900,0.000000,24.720000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(28+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [UNIFORM LIFE INSURANCE TABLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_VIRGINIA_STATE_TAX_TABLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","VIRGINIA STATE TAX TABLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("");
		scrn->title2=stralloc("");
		scrn->desc=stralloc("");
		scrn->desc=stralloc("");
		scrn->inclusive=0;
		addRDATableValue(scrn,300000.000000,0.000000,0.020000);
		addRDATableValue(scrn,500000.000000,6000.000000,0.030000);
		addRDATableValue(scrn,1700000.000000,12000.000000,0.050000);
		addRDATableValue(scrn,9999999999.000000,72000.000000,0.057500);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(24+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [VIRGINIA STATE TAX TABLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_AL_STATE_WITHHOLDING()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL AL STATE WITHHOLDING-MARRIED");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("");
		scrn->title2=stralloc("");
		scrn->desc=stralloc("");
		scrn->desc=stralloc("");
		scrn->inclusive=0;
		addRDATableValue(scrn,100000.000000,0.000000,0.020000);
		addRDATableValue(scrn,600000.000000,2000.000000,0.040000);
		addRDATableValue(scrn,99999999999999.000000,22000.000000,0.050000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(27+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL AL STATE WITHHOLDING], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
	scrn=RDATABLENEW("PAYROLL","ANNUAL AL STATE WITHHOLDING-SINGLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("");
		scrn->title2=stralloc("");
		scrn->desc=stralloc("");
		scrn->desc=stralloc("");
		scrn->inclusive=0;
		addRDATableValue(scrn,50000.000000,0.000000,0.020000);
		addRDATableValue(scrn,250000.000000,1000.000000,0.040000);
		addRDATableValue(scrn,99999999999999.000000,11000.000000,0.050000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(27+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL AL STATE WITHHOLDING], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
	scrn=RDATABLENEW("PAYROLL","ANNUAL AL STATE WITHHOLDING-OTHER");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("");
		scrn->title2=stralloc("");
		scrn->desc=stralloc("");
		scrn->desc=stralloc("");
		scrn->inclusive=0;
		addRDATableValue(scrn,50000.000000,0.000000,0.020000);
		addRDATableValue(scrn,300000.000000,1000.000000,0.040000);
		addRDATableValue(scrn,99999999999999.000000,11000.000000,0.050000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(27+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL AL STATE WITHHOLDING], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_WVSTATE_SINGLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL WVSTATE SINGLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("WV State Single Income Tax Table");
		scrn->title2=stralloc("Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the WV State Income Tax Single/Head of Household/Widow(er) Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,1000000.000000,0.000000,0.03);
		addRDATableValue(scrn,2500000.000000,30000.000000,0.040000);
		addRDATableValue(scrn,4000000.000000,90000.000000,0.04500);
		addRDATableValue(scrn,6000000.000000,157500.000000,0.06000);
		addRDATableValue(scrn,99999999.999990,277500.00,0.065000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(40+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL WVSTATE SINGLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_WVSTATE_MARRIED()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL WVSTATE MARRIED");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("WV State Married Income Tax Table");
		scrn->title2=stralloc("Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the WV State Income Tax Single/Head of Household/Widow(er) Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,500000.000000,0.000000,0.0300);
		addRDATableValue(scrn,1250000.000000,15000.000000,0.040000);
		addRDATableValue(scrn,2000000.000000,45000.000000,0.04500);
		addRDATableValue(scrn,3000000.000000,78750.000000,0.06000);
		addRDATableValue(scrn,99999999.999990,138750.00,0.065000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(40+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL WVSTATE MARRIED], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_FEDERAL_WITHHOLDING_MARRIED()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL FEDERAL WITHHOLDING MARRIED");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("Federal Married Tax Table");
		scrn->title2=stralloc("2014 Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the Federal Married Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,845000.000000,0.000000,0.000000);
		addRDATableValue(scrn,2660000.000000,0.000000,0.100000);
		addRDATableValue(scrn,8225000.000000,181500.000000,0.150000);
		addRDATableValue(scrn,15730000.000000,1016250.000000,0.250000);
		addRDATableValue(scrn,23530000.000000,2892500.000000,0.280000);
		addRDATableValue(scrn,41355000.000000,5076500.000000,0.330000);
		addRDATableValue(scrn,46605000.000000,10958750.000000,0.350000);
		addRDATableValue(scrn,99999999.999990,12796250.000000,0.396000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(34+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL FEDERAL WITHHOLDING MARRIED], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_FEDERAL_WITHHOLDING_SINGLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL FEDERAL WITHHOLDING SINGLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("Federal Single Tax Table");
		scrn->title2=stralloc("2014 Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the Federal Single Tax Rates for Current Calendar Year.");
		scrn->inclusive=0;
		addRDATableValue(scrn,225000.000000,0.000000,0.000000);
		addRDATableValue(scrn,1132500.000000,0.000000,0.100000);
		addRDATableValue(scrn,3915000.000000,90750.000000,0.150000);
		addRDATableValue(scrn,9160000.000000,508125.000000,0.250000);
		addRDATableValue(scrn,18860000.000000,1819375.000000,0.280000);
		addRDATableValue(scrn,40735000.000000,4535375.000000,0.330000);
		addRDATableValue(scrn,40900000.000000,11754125.000000,0.350000);
		addRDATableValue(scrn,99999999.999990,11811875.000000,0.396000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(33+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL FEDERAL WITHHOLDING SINGLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_VRS_OLIP_COST()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL VRS OLIP COST");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("VA VRS Optional Life Insurance Cost");
		scrn->title2=stralloc("07/01/2011 Annualized Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the Annual Virginia Retirement System Optional Life Insurance Program Cost Rates,");
		scrn->inclusive=0;
		addRDATableValue(scrn,30.000000,0.000000,0.600000);
		addRDATableValue(scrn,35.000000,0.000000,0.720000);
		addRDATableValue(scrn,40.000000,0.000000,0.960000);
		addRDATableValue(scrn,45.000000,0.000000,1.080000);
		addRDATableValue(scrn,50.000000,0.000000,1.680000);
		addRDATableValue(scrn,55.000000,0.000000,2.520000);
		addRDATableValue(scrn,60.000000,0.000000,4.800000);
		addRDATableValue(scrn,65.000000,0.000000,7.920000);
		addRDATableValue(scrn,70.000000,0.000000,15.240000);
		addRDATableValue(scrn,75.000000,0.000000,24.720000);
		addRDATableValue(scrn,999.000000,0.000000,24.720000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL VRS OLIP COST], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_NDSTATE_MARRIED()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL NDSTATE MARRIED");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("ND State Married Income Tax Table");
		scrn->title2=stralloc("2014 Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the ND State Income Tax Married Withholding Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,1000000.000000,0.000000,0.0000);
		addRDATableValue(scrn,7000000.000000,0.000000,0.012200);
		addRDATableValue(scrn,13200000.000000,73200.000000,0.02270);
		addRDATableValue(scrn,23500000.00000,213940.00000,0.02520);
		addRDATableValue(scrn,41200000.00000,473500.00000,0.02930);
		addRDATableValue(scrn,99999999.999990,992110.00,0.032200);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(40+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL NDSTATE MARRIED], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_NDSTATE_SINGLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL NDSTATE SINGLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("ND State Single Income Tax Table");
		scrn->title2=stralloc("2014 Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the ND State Income Tax Single Withholding Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,420000.000000,0.000000,0.0000);
		addRDATableValue(scrn,4000000.000000,0.000000,0.012200);
		addRDATableValue(scrn,8200000.000000,43676.000000,0.02270);
		addRDATableValue(scrn,18800000.00000,139016.00000,0.02520);
		addRDATableValue(scrn,40500000.00000,406136.00000,0.02930);
		addRDATableValue(scrn,99999999.999990,1041646.00,0.032200);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(40+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL NDSTATE SINGLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
/*
static void TBL_ANNUAL_NC_STATE_HEAD_OF_HOUSEHOLD()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL NC STATE HEAD OF HOUSEHOLD");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("NC State Head of Household Tax Table");
		scrn->title2=stralloc("2012 Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the North Carolina State Head of Household Tax Rates.");
		scrn->desc=stralloc("This RDA Table is used to reflect the North Carolina State Head of Household Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,1700000.000000,0.000000,0.060000);
		addRDATableValue(scrn,8000000.000000,102000.000000,0.070000);
		addRDATableValue(scrn,99999999.000000,543000.000000,0.077500);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(33+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL NC STATE HEAD OF HOUSEHOLD], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_NC_STATE_MARRIED_WIDOWED()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL NC STATE MARRIED-WIDOWED");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("NC State Married-Widowed Tax Table");
		scrn->title2=stralloc("2012 Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the North Carolina State Married-Widowed Tax Rates.");
		scrn->desc=stralloc("This RDA Table is used to reflect the North Carolina State Married-Widowed Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,1062500.000000,0.000000,0.060000);
		addRDATableValue(scrn,5000000.000000,63750.000000,0.070000);
		addRDATableValue(scrn,99999999.000000,339375.000000,0.077500);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(39+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL NC STATE MARRIED-WIDOWED], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_NC_STATE_SINGLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL NC STATE SINGLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("NC State Single Tax Table");
		scrn->title2=stralloc("2012 Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the North Carolina State Single Tax Rates.");
		scrn->desc=stralloc("This RDA Table is used to reflect the North Carolina State Single Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,1275000.000000,0.000000,0.060000);
		addRDATableValue(scrn,6000000.000000,76500.000000,0.070000);
		addRDATableValue(scrn,99999999.000000,407250.000000,0.077500);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL NC STATE SINGLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
*/
static void TBL_ANNUAL_GEORGIA_STATE_MARRIED_ONE_INCOME_HEAD_OF_HOUSEHOLD()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL GEORGIA STATE MARRIED ONE INCOME/HEAD OF HOUSEHOLD");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("Georgia Married One Income or Head of Household Tax Table");
		scrn->title2=stralloc("Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the Georgia State Married One Income or Head of Household Tax Rates.");
		scrn->desc=stralloc("This RDA Table is used to reflect the Georgia State Married One Income or Head of Household Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,100000.000000,0.000000,0.010000);
		addRDATableValue(scrn,300000.000000,1000.000000,0.020000);
		addRDATableValue(scrn,500000.000000,5000.000000,0.030000);
		addRDATableValue(scrn,700000.000000,11000.000000,0.040000);
		addRDATableValue(scrn,1000000.000000,19000.000000,0.050000);
		addRDATableValue(scrn,9999999999.000000,34000.000000,0.060000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(57+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL GEORGIA STATE MARRIED ONE INCOME/HEAD OF HOUSEHOLD], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_GEORGIA_STATE_MARRIED_SEPARATE_TWO_INCOMES()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL GEORGIA STATE MARRIED/SEPARATE TWO INCOMES");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("Georgia Married/Separate Two Incomes Tax Table");
		scrn->title2=stralloc("Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the Georgia State Married, Filing Jointly or Separate (Two Incomes) Tax Rates.");
		scrn->desc=stralloc("This RDA Table is used to reflect the Georgia State Married, Filing Jointly or Separate (Two Incomes) Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,50000.000000,0.000000,0.010000);
		addRDATableValue(scrn,150000.000000,500.000000,0.020000);
		addRDATableValue(scrn,250000.000000,2500.000000,0.030000);
		addRDATableValue(scrn,350000.000000,5500.000000,0.040000);
		addRDATableValue(scrn,500000.000000,9500.000000,0.050000);
		addRDATableValue(scrn,9999999999.000000,17000.000000,0.060000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(49+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL GEORGIA STATE MARRIED/SEPARATE TWO INCOMES], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_GEORGIA_STATE_SINGLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL GEORGIA STATE SINGLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("Georgia Single Income Tax Table");
		scrn->title2=stralloc("Annual Rates");
		scrn->desc=stralloc("This RDA Table is used to reflect the Georgia State Single Tax Rates.");
		scrn->desc=stralloc("This RDA Table is used to reflect the Georgia State Single Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,75000.000000,0.000000,0.010000);
		addRDATableValue(scrn,225000.000000,750.000000,0.020000);
		addRDATableValue(scrn,375000.000000,3750.000000,0.030000);
		addRDATableValue(scrn,525000.000000,8250.000000,0.040000);
		addRDATableValue(scrn,700000.000000,14250.000000,0.050000);
		addRDATableValue(scrn,9999999999.000000,23000.000000,0.060000);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(27+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [] Table [ANNUAL GEORGIA STATE SINGLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_NEW_YORK_STATE_SINGLE()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL NEW YORK STATE SINGLE");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("New York Single Income Tax Table");
		scrn->title2=stralloc("Annual Rate 2014");
		scrn->desc=stralloc("This RDA Table is used to reflect the New York State Single Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,830000.000000,0.000000,0.040000);
		addRDATableValue(scrn,1145000.000000,33200.0000,0.045000);
		addRDATableValue(scrn,1355000.000000,47400.000000,0.052500);
		addRDATableValue(scrn,2085000.000000,58400.000000,0.059000);
		addRDATableValue(scrn,7840000.000000,101500.000000,0.064500);
		addRDATableValue(scrn,9410000.000000,472700.000000,0.066500);
		addRDATableValue(scrn,10460000.000000,577100.000000,0.075800);
		addRDATableValue(scrn,15690000.000000,656700.000000,0.080800);
		addRDATableValue(scrn,20925000.000000,1079200.000000,0.071500);
		addRDATableValue(scrn,26155000.000000,1453500.000000,0.081500);
		addRDATableValue(scrn,104635000.000000,1879800.000000,0.073500);
		addRDATableValue(scrn,109870000.000000,7648100.000000,0.490200);
		addRDATableValue(scrn,9999999999.000000,10214300.000000,0.096200);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(47+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [PAYROLL] Table [ANNUAL NEW YORK STATE SINGLE], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}
static void TBL_ANNUAL_NEW_YORK_STATE_MARRIED()
{
	RDATABLE *scrn=NULL;
	char *temp1=NULL;

	scrn=RDATABLENEW("PAYROLL","ANNUAL NEW YORK STATE MARRIED");
	if(scrn!=NULL)
	{
		scrn->title1=stralloc("New York Married Income Tax Table");
		scrn->title2=stralloc("Annual Rate 2014");
		scrn->desc=stralloc("This RDA Table is used to reflect the New York State Married Tax Rates.");
		scrn->inclusive=0;
		addRDATableValue(scrn,830000.000000,0.000000,0.040000);
		addRDATableValue(scrn,1145000.000000,33200.0000,0.045000);
		addRDATableValue(scrn,1355000.000000,47400.000000,0.052500);
		addRDATableValue(scrn,2085000.000000,58400.000000,0.059000);
		addRDATableValue(scrn,7840000.000000,101500.000000,0.064500);
		addRDATableValue(scrn,9410000.000000,472700.000000,0.066500);
		addRDATableValue(scrn,10460000.000000,577100.000000,0.072800);
		addRDATableValue(scrn,15690000.000000,653500.000000,0.077800);
		addRDATableValue(scrn,20925000.000000,1060400.000000,0.080800);
		addRDATableValue(scrn,31385000.000000,1483400.000000,0.071500);
		addRDATableValue(scrn,36620000.000000,2231300.000000,0.081500);
		addRDATableValue(scrn,104635000.000000,2657900.000000,0.073500);
		addRDATableValue(scrn,209280000.000000,7657000.000000,0.076500);
		addRDATableValue(scrn,214515000.000000,15662400.000000,0.884200);
		addRDATableValue(scrn,9999999999.000000,20291200.000000,0.096200);

		if(ADVwriteRDATablebin("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(87+0+110+1);
			sprintf(temp1,"TABLE WRITE ERROR: Module [PAYROLL] Table [ANNUAL NEW YORK STATE MARRIED], Can Not Save Table!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeRDATABLE(scrn);
	}
}

static void quitsetup(RDArsrc *r)
{
	if(DeductionID!=NULL) Rfree(DeductionID);
	if(dedtype!=NULL) Rfree(dedtype);
	if(payddms!=(-1)) CLSNRD(payddms);
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	ShutdownSubsystems();
	exit(0);
}

static void VIR_STATE_ALLOWANCES()
{
	DFvirtual *scrn=NULL;
	char *temp1=NULL;

	scrn=DFvirtualNEW("PAYROLL","STATE ALLOWANCES");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=2;
		DFvirtualDFincvir(scrn,"PRSNNL","PERDMG","",0);
		DFvirtualDFincvir(scrn,"PAYROLL","PAYDEDM","",0);
		scrn->avail2rsrc=1;
		scrn->type=2;
		scrn->length=20;
		scrn->when=0;
		if(!RDAstrcmp(XPERT_SETUP->state,"GA"))
		{
		temp1=Rmalloc(1703+1);
		sprintf(temp1,"%s%s%s%s%s%s%s%s",
			"IF([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\") THEN\n	RETURN_VALUE=230000+(270000*[",
			"PRSNNL][PERDMG][STATE EXEMPTION])+300000*[PRSNNL][PERDMG][STATE DEPENDENTS] ELSE\nIF([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED-1\") THEN",
			"\n	RETURN_VALUE=300000+(270000*[PRSNNL][PERDMG][STATE EXEMPTION])+300000*[PRSNNL][PERDMG][STATE DEPENDENTS] ELSE\nIF([PRSNNL][PERDMG][STATE FI",
			"LING STATUS]=\"MARRIED-2\") THEN\n	RETURN_VALUE=150000+(270000*[PRSNNL][PERDMG][STATE EXEMPTION])+300000*[PRSNNL][PERDMG][STATE DEPENDENTS] EL",
			"SE\nIF([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED-SEPARATE\") THEN\n	RETURN_VALUE=150000+(27000",
			"0*[PRSNNL][PERDMG][STATE EXEMPTION])+300000*[PRSNNL][PERDMG][STATE DEPENDENTS] ELSE\nIF([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") THEN",
			"\n	RETURN_VALUE=230000+(270000*[PRSNNL][PERDMG][STATE EXEMPTION])+300000*[PRSNNL][PERDMG][STATE DEPENDENTS]\nELSE RETURN_VALUE=0 \nFI FI FI FI",
			" FI");
		} else if(!RDAstrcmp(XPERT_SETUP->state,"AL"))
		{
			temp1=Rmalloc(9703+1);
			sprintf(temp1,"%s%s%s%s%s%s",
				"IF((([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED\") OR \n([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\")) AND ([CURRENT DEDUCTION ANNUAL AFFECT BASE]>=2000000)) THEN \n	RETURN_VALUE=400000+(ROUND(150000*[PRSNNL][PERDMG][STATE EXEMPTION]+[FEDERAL][AMOUNT]/[FEDERAL][RATIO]+[PRSNNL][PERDMG][STATE DEPENDENTS]*[PAYROLL][PAYDDMS][EXEMPTION AMOUNT])) \nELSE ",
				"IF((([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED\") OR \n([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\")) AND ([CURRENT DEDUCTION ANNUAL AFFECT BASE]<2000000)) THEN \n	RETURN_VALUE=(ROUND(150000*[PRSNNL][PERDMG][STATE EXEMPTION]+[FEDERAL][AMOUNT]/[FEDERAL][RATIO]+[CURRENT DEDUCTION ANNUAL AFFECT BASE]*0.20+[PRSNNL][PERDMG][STATE DEPENDENTS]*[PAYROLL][PAYDDMS][EXEMPTION AMOUNT])) \nELSE ",
				"IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") AND ([CURRENT DEDUCTION ANNUAL AFFECT BASE]<1000000)) THEN \n	RETURN_VALUE=(ROUND(150000*[PRSNNL][PERDMG][STATE EXEMPTION]+[FEDERAL][AMOUNT]/[FEDERAL][RATIO]+[CURRENT DEDUCTION ANNUAL AFFECT BASE]*0.20+[PRSNNL][PERDMG][STATE DEPENDENTS]*[PAYROLL][PAYDDMS][EXEMPTION AMOUNT])) \nELSE ",
				"IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") AND ([CURRENT DEDUCTION ANNUAL AFFECT BASE]>=1000000)) THEN \n	RETURN_VALUE=200000+(ROUND(150000*[PRSNNL][PERDMG][STATE EXEMPTION]+[FEDERAL][AMOUNT]/[FEDERAL][RATIO]+[PRSNNL][PERDMG][STATE DEPENDENTS]*[PAYROLL][PAYDDMS][EXEMPTION AMOUNT])) \nELSE ",
				"IF(([CURRENT DEDUCTION ANNUAL AFFECT BASE]>=1000000)) THEN \n	RETURN_VALUE=200000+(ROUND([FEDERAL][AMOUNT]/[FEDERAL][RATIO]+[PRSNNL][PERDMG][STATE DEPENDENTS]*[PAYROLL][PAYDDMS][EXEMPTION AMOUNT])) \nELSE ",
				"RETURN_VALUE=(ROUND([FEDERAL][AMOUNT]/[FEDERAL][RATIO]+[CURRENT DEDUCTION ANNUAL AFFECT BASE]*0.20+[PRSNNL][PERDMG][STATE DEPENDENTS]*[PAYROLL][PAYDDMS][EXEMPTION AMOUNT])) FI FI FI FI FI");
		/*
		} else if(!RDAstrcmp(XPERT_SETUP->state,"NC"))
		{
			temp1=Rmalloc(526+1);
			sprintf(temp1,"%s%s%s%s%s%s",
				"IF((([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") AND ([CURRENT DEDUCTION ANNUAL AFFECT BASE]>6",
				"000000)) OR (([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\") AND ([CURRENT DEDUCTION A",
				"NNUAL AFFECT BASE]>8000000)) OR ((([PRSNNL][PERDMG][STATE FILING STATUS] C \"MARRIED\") OR ([PRSNNL]",
				"[PERDMG][STATE FILING STATUS]=\"WIDOWED\")) AND ([CURRENT DEDUCTION ANNUAL AFFECT BASE]>5000000))) T",
				"HEN RETURN_VALUE=(200000*[PRSNNL][PERDMG][STATE EXEMPTION]) ELSE \nRETURN_VALUE=(250000*[PRSNNL][PER",
				"DMG][STATE EXEMPTION])) FI");
			temp1=Rmalloc(117+1);
			sprintf(temp1,"%s%s",
				"IF([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\") THEN RETURN_VALUE=1200000 ELSE RETUR",
				"N_VALUE=750000 FI");
		} else if(!RDAstrcmp(XPERT_SETUP->state,"NY"))
		{
			temp1=Rmalloc(105+1);
			sprintf(temp1,"%s%s",
				"IF([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") THEN RETURN_VALUE=725000 ELSE RETURN_VALUE=775000",
				" FI");
		*/
		}  
		scrn->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->desc=stralloc("This RDA Virtual Field is used to denote the State Taxable Earnings Computation.");

		if(writeDFvirtual("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+0+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [] Virtual Field [STATE ALLOWANCES], Can Not Save Virtual Field!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeDFvirtual(scrn);
	}
}
static void VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT()
{
	DFvirtual *scrn=NULL;
	char *temp1=NULL;

	scrn=DFvirtualNEW("PAYROLL","STANDARD COMPUTATION EXEMPTION AMOUNT");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=2;
		DFvirtualDFincvir(scrn,"PAYROLL","PAYDDMS","",0);
		DFvirtualDFincvir(scrn,"PAYROLL","PAYDEDM","",0);
		DFvirtualDFincvir(scrn,"PRSNNL","PERDMG","",0);
		scrn->avail2rsrc=1;
		scrn->type=2;
		scrn->length=20;
		scrn->when=0;
		if(!RDAstrcmp(XPERT_SETUP->state,"GA"))
		{
			temp1=Rmalloc(264+1);
			sprintf(temp1,"%s%s%s",
				"IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE\") THEN\n	RETURN_VALUE=[STATE ALLOWANCES] \nELSE IF([P",
				"AYROLL][PAYDDMS][USE EXEMPTIONS]=TRUE) THEN\n	RETURN_VALUE=[STANDARD COMPUTATION NUMBER EXEMPTIONS]*",
				"[PAYROLL][PAYDDMS][EXEMPTION AMOUNT] \nELSE RETURN_VALUE=0 FI FI");
		} else if(!RDAstrcmp(XPERT_SETUP->state,"AL"))
		{
			temp1=Rmalloc(264+1);
			sprintf(temp1,"%s%s%s",
				"IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE\") THEN\n	RETURN_VALUE=[STATE ALLOWANCES] \nELSE IF([P",
				"AYROLL][PAYDDMS][USE EXEMPTIONS]=TRUE) THEN\n	RETURN_VALUE=[STANDARD COMPUTATION NUMBER EXEMPTIONS]*",
				"[PAYROLL][PAYDDMS][EXEMPTION AMOUNT] \nELSE RETURN_VALUE=0 FI FI");
		} else {
			temp1=Rmalloc(264+1);
			sprintf(temp1,"%s%s%s",
				"IF([P",
				"AYROLL][PAYDDMS][USE EXEMPTIONS]=TRUE) THEN\n	RETURN_VALUE=[STANDARD COMPUTATION NUMBER EXEMPTIONS]*",
				"[PAYROLL][PAYDDMS][EXEMPTION AMOUNT] \nELSE RETURN_VALUE=0 FI");
		}
		scrn->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->desc=stralloc("This virtual field is used to compute the Exemption Amount used in the STANDARD COMPUTATION virtual field to avoid duplication.");

		if(writeDFvirtual("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(37+0+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [] Virtual Field [STANDARD COMPUTATION EXEMPTION AMOUNT], Can Not Save Virtual Field!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeDFvirtual(scrn);
	}
}
static void VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION()
{
	DFvirtual *scrn=NULL;
	char *temp1=NULL;

	scrn=DFvirtualNEW("PAYROLL","STANDARD COMPUTATION STANDARD DEDUCTION");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=2;
		DFvirtualDFincvir(scrn,"PAYROLL","PAYDDMS","",0);
		DFvirtualDFincvir(scrn,"PAYROLL","PAYDEDM","",0);
		DFvirtualDFincvir(scrn,"PRSNNL","PERDMG","",0);
		scrn->avail2rsrc=1;
		scrn->type=2;
		scrn->length=20;
		scrn->when=0;
		if(!RDAstrcmp(XPERT_SETUP->state,"NC"))
		{
			temp1=Rmalloc(437+1);
			sprintf(temp1,"%s%s%s%s%s",
				"IF([PAYROLL][PAYDDMS][USE STANDARD DEDUCTION]=TRUE) THEN \n	RETURN_VALUE=[PAYROLL][PAYDDMS][STANDARD",
				" DEDUCTION AMOUNT] \nELSE IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\") AND ([PAY",
				"ROLL][PAYDDMS][DEDUCTION DESCRIPTION IDENTIFICATION]=\"NC STATE\")) THEN \n	RETURN_VALUE=1200000 \nE",
				"LSE IF(([PAYROLL][PAYDDMS][DEDUCTION DESCRIPTION IDENTIFICATION]=\"NC STATE\")) THEN\n	RETURN_VALUE=",
				"750000 \nELSE RETURN_VALUE=0 FI FI FI");
		} else if(!RDAstrcmp(XPERT_SETUP->state,"NY"))
		{
			temp1=Rmalloc(355+1);
			sprintf(temp1,"%s%s%s%s",
				"IF (([ORGANIZATION STATE]=\"NY\") AND ([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE\")) \n   THEN IF (",
				"[PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED\") THEN RETURN_VALUE=775000 ELSE RETURN_VALUE=725000",
				" FI \nELSE IF (([PAYROLL][PAYDDMS][USE STANDARD DEDUCTION]=TRUE) THEN RETURN_VALUE=[PAYROLL][PAYDDMS",
				"][STANDARD DEDUCTION AMOUNT]\nELSE RETURN_VALUE=0 FI FI");
		} else {
			temp1=Rmalloc(142+1);
			sprintf(temp1,"%s%s",
				"IF([PAYROLL][PAYDDMS][USE STANDARD DEDUCTION]=TRUE) THEN\n	RETURN_VALUE=[PAYROLL][PAYDDMS][STANDARD ",
				"DEDUCTION AMOUNT] \nELSE RETURN_VALUE=0 FI");
		}
		scrn->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->desc=stralloc("This virtual field is used to compute the Standard Deduction Amount used in the STANDARD COMPUTATION virtual field to avoid duplication.");

		if(writeDFvirtual("PAYROLL",scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(39+0+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [] Virtual Field [STANDARD COMPUTATION STANDARD DEDUCTION], Can Not Save Virtual Field!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(scrn!=NULL) FreeDFvirtual(scrn);
	}
}

static void VRSINS50K(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0,factorx=2;

	TBL_UNIFORM_LIFE_INSURANCE_TABLE();
	comptype=17;
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
	FINDFLDSETSHORT(payddms,"FACTOR",factorx);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);

	FINDFLDSETSTRING(payddms,"TABLE NAME","\"UNIFORM LIFE INSURANCE TABLE\"");
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void FEDERAL(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	TBL_ANNUAL_FEDERAL_WITHHOLDING_MARRIED();
	TBL_ANNUAL_FEDERAL_WITHHOLDING_SINGLE();
	comptype=9; /* Table Lookup Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
	FINDFLDSETSTRING(payddms,"TABLE NAME","IF([PRSNNL][PERDMG][FEDERAL FILING STATUS]=0) THEN RETURN_VALUE=\"ANNUAL FEDERAL WITHHOLDING MARRIED\" \nELSE RETURN_VALUE=\"ANNUAL FEDERAL WITHHOLDING SINGLE\" FI");
	FINDFLDSETCHAR(payddms,"CUSTOM CONFIGURATION",FALSE);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=395000;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void FICA(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=3; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETCHAR(payddms,"USE GROSS AMOUNT LIMITS",TRUE);
	gamt=11700000.0;
	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=6.2;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
	FINDFLDSETCHAR(payddms,"USE WITHHOLDING LIMITS",TRUE);
	famt=725400.0;
	FINDFLDSETDOUBLE(payddms,"WITHHOLDING LIMIT",famt);


}
static void FICAM(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=3; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETCHAR(payddms,"USE GROSS AMOUNT LIMITS",TRUE);
	gamt=11700000.0;
	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=6.2;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
	FINDFLDSETCHAR(payddms,"USE WITHHOLDING LIMITS",TRUE);
	famt=725400.0;
	FINDFLDSETDOUBLE(payddms,"WITHHOLDING LIMIT",famt);


}
static void MEDICARE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=3; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
	FINDFLDSETSTRING(payddms,"TABLE NAME","");
	FINDFLDSETSTRING(payddms,"COMPUTATION","IF(([ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS]+[CURRENT DEDUCTION CHECK GROSS])<20000000) THEN \nRETURN_VALUE=[STANDARD COMPUTATION PERCENTAGE ADJUSTED GROSS] ELSE \nIF(([ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS]>20000000)) THEN \nRETURN_VALUE=[STANDARD COMPUTATION PERCENTAGE ADJUSTED GROSS]+(ROUND(([CURRENT DEDUCTION CHECK GROSS]*0.009))) ELSE \nRETURN_VALUE=[STANDARD COMPUTATION PERCENTAGE ADJUSTED GROSS]+(ROUND(((([CURRENT DEDUCTION CHECK GROSS]+[ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS]-20000000)*0.009)))) FI FI");
	FINDFLDSETCHAR(payddms,"CUSTOM CONFIGURATION",TRUE);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);

	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=1.45;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);


}
static void MEDICAREM(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=3; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=1.45;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);


}
static void VRSOLIPDEP1(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETSTRING(payddms,"GROSS AMOUNT","[STANDARD GROSS AMOUNT]");


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=80.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPDEP2(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=80.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPDEP3(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=160.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPDEP4(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETSTRING(payddms,"GROSS AMOUNT","[STANDARD GROSS AMOUNT]");


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=240.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPEMP1(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	TBL_ANNUAL_VRS_OLIP_COST();
	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPEMP2(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	TBL_ANNUAL_VRS_OLIP_COST();
	FINDFLDSETCHAR(payddms,"ELECTRONIC TRANSFER",FALSE);
	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETSTRING(payddms,"GROSS AMOUNT","[CURRENT DEDUCTION LIFE INSURANCE SALARY]");


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPEMP3(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	TBL_ANNUAL_VRS_OLIP_COST();
	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPEMP4(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	TBL_ANNUAL_VRS_OLIP_COST();
	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPSPC1(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPSPC2(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPSPC3(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VRSOLIPSPC4(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; 
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);

}
static void VASTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	TBL_VIRGINIA_STATE_TAX_TABLE();
	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	comptype=9; /* Table Lookup Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);


	FINDFLDSETSTRING(payddms,"TABLE NAME","\"VIRGINIA STATE TAX TABLE\"");
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=93000;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
	eamt=300000;
	FINDFLDSETDOUBLE(payddms,"STANDARD DEDUCTION AMOUNT",eamt);
}
static void ALSTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	TBL_ANNUAL_AL_STATE_WITHHOLDING();
	comptype=9; /* Table Lookup Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);


	FINDFLDSETSTRING(payddms,"TABLE NAME","IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED\") OR ([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\")) THEN RETURN_VALUE=\"ANNUAL AL STATE WITHHOLDING-MARRIED\" \nELSE IF([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") THEN RETURN_VALUE=\"ANNUAL AL STATE WITHHOLDING-SINGLE\" \nELSE RETURN_VALUE=\"ANNUAL AL STATE WITHHOLDING-OTHER\" FI FI");
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=30000;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);


}
static void WVSTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	TBL_ANNUAL_WVSTATE_SINGLE();
	TBL_ANNUAL_WVSTATE_MARRIED();
	FINDFLDSETCHAR(payddms,"USE STANDARD DEDUCTION",TRUE);
	comptype=9; /* Table Lookup Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
	FINDFLDSETSTRING(payddms,"TABLE NAME","IF([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") THEN RETURN_VALUE=\"ANNUAL WVSTATE SINGLE\" \nELSE RETURN_VALUE=\"ANNUAL WVSTATE MARRIED\" FI");
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=90000;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
	eamt=300000;
	FINDFLDSETDOUBLE(payddms,"STANDARD DEDUCTION AMOUNT",eamt);
}
static void PSERS(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=4.00;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void PSERSM(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=5.26;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDSTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	TBL_ANNUAL_NDSTATE_SINGLE();
	TBL_ANNUAL_NDSTATE_MARRIED();
	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	comptype=9; /* Table Lookup Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
	FINDFLDSETSTRING(payddms,"TABLE NAME","IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED\") OR ([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\")) THEN RETURN_VALUE=\"ANNUAL NDSTATE MARRIED\" \nELSE RETURN_VALUE=\"ANNUAL NDSTATE SINGLE\" FI");
 
	FINDFLDSETCHAR(payddms,"CUSTOM CONFIGURATION",TRUE);
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=395000.00;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDSTATEPERC(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=3; /* Percentage of Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
	FINDFLDSETSTRING(payddms,"TABLE NAME","");
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=370000.00;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=21.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPCONMOD0(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=10.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFRMEMASSMOD0(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=9.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPCONMOD1(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=10.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFRMEMASSMOD1(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=9.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPCONMOD2(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=10.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPASSMOD2(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=9.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFRMEMASSMOD4(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=3.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPCONMOD4(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=10.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPASSMOD4(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=4.00;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFRMEMASSMOD3(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=0; /* Flat Amount */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.00;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPCONMOD3(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=1; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=10.75;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NDTFFREMPASSMOD3(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	comptype=0; /* Flat Amount */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.00;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void ILSTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	comptype=3; /* Percentage of Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);
	FINDFLDSETSTRING(payddms,"TABLE NAME","");
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=210000;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=5.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NCSTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

/*
	TBL_ANNUAL_NC_STATE_HEAD_OF_HOUSEHOLD();
	TBL_ANNUAL_NC_STATE_MARRIED_WIDOWED();
	TBL_ANNUAL_NC_STATE_SINGLE();
*/
	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	comptype=3; /* Percentage of Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);


/*
	FINDFLDSETSTRING(fileno,"TABLE NAME","IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\")) THEN RETURN_VALUE=\"ANNUAL NC STATE HEAD OF HOUSEHOLD\" \nELSE IF([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") THEN RETURN_VALUE=\"ANNUAL NC STATE SINGLE\" \nELSE IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"WIDOWED\") OR ([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED\")) THEN RETURN_VALUE=\"ANNUAL NC STATE MARRIED-WIDOWED\" \nELSE RETURN_VALUE=\"ANNUAL NC STATE SINGLE\" FI FI FI");
*/
	FINDFLDSETSTRING(fileno,"TABLE NAME","");
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=250000;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=5.8;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
	FINDFLDSETCHAR(payddms,"USE STANDARD DEDUCTION",FALSE);
	eamt=0;
	FINDFLDSETDOUBLE(payddms,"STANDARD DEDUCTION AMOUNT",eamt);
}
static void GASTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	TBL_ANNUAL_GEORGIA_STATE_MARRIED_ONE_INCOME_HEAD_OF_HOUSEHOLD();
	TBL_ANNUAL_GEORGIA_STATE_MARRIED_SEPARATE_TWO_INCOMES();
	TBL_ANNUAL_GEORGIA_STATE_SINGLE();
	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	comptype=9; /* Table Lookup Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);


	FINDFLDSETSTRING(payddms,"TABLE NAME","IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"HEAD OF HOUSEHOLD\") OR ([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED-1\")) THEN RETURN_VALUE=\"ANNUAL GEORGIA STATE MARRIED ONE INCOME/HEAD OF HOUSEHOLD\" \nELSE IF([PRSNNL][PERDMG][STATE FILING STATUS]=\"SINGLE\") THEN RETURN_VALUE=\"ANNUAL GEORGIA STATE SINGLE\" \nELSE IF(([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED-2\") OR ([PRSNNL][PERDMG][STATE FILING STATUS]=\"MARRIED-SEPARATE\")) THEN RETURN_VALUE=\"ANNUAL GEORGIA STATE MARRIED/SEPARATE TWO INCOMES\" \nELSE RETURN_VALUE=\"ANNUAL GEORIGA STATE SINGLE\" FI FI FI");
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void NYSTATE(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0;
	short comptype=0;

	TBL_ANNUAL_NEW_YORK_STATE_SINGLE();
	TBL_ANNUAL_NEW_YORK_STATE_MARRIED();
	VIR_STATE_ALLOWANCES();
	VIR_STANDARD_COMPUTATION_EXEMPTION_AMOUNT();
	VIR_STANDARD_COMPUTATION_STANDARD_DEDUCTION();
	FINDFLDSETDOUBLE(payddms,"STANDARD DEDUCTION AMOUNT",eamt);
	FINDFLDSETCHAR(payddms,"EXPENSED",TRUE);
	FINDFLDSETCHAR(payddms,"LIABILITY",TRUE);
	comptype=9; /* Table Lookup Adjusted Gross */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);


	FINDFLDSETSTRING(payddms,"TABLE NAME","IF([PRSNNL][PERDMG][STATE FILING STATUS]!=\"SINGLE\") THEN RETURN_VALUE=\"ANNUAL NEW YORK STATE MARRIED\" \nELSE RETURN_VALUE=\"ANNUAL NEW YORK STATE SINGLE\" FI");
 
	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
	eamt=100000.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);
}
static void FICA3RD(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; /* flat amount*/
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	FINDFLDSETCHAR(payddms,"USE GROSS AMOUNT LIMITS",FALSE);
	gamt=0.0;
	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.0;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);


}
static void MEDICARE3RD(short payddms)
{
	double eamt=0.0,pamt=0.0,famt=0.0,gamt=0.0;
	short comptype=0;

	comptype=0; /* Flat Amount */
	FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",comptype);

	gamt=0.0;
	FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",gamt);
	FINDFLDSETCHAR(payddms,"USE GROSS AMOUNT LIMITS",FALSE);


	FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",FALSE);
	eamt=0.0;
	FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
	pamt=0.00;
	FINDFLDSETDOUBLE(payddms,"PERCENTAGE",pamt);
	famt=0.0;
	FINDFLDSETDOUBLE(payddms,"FLAT AMOUNT",famt);


}
static void setupded(RDArsrc *r)
{
	int x=0;
	RDATData *prev=NULL;

	FINDRSCGETINT(r,"SETUPS",&x);
	ZERNRD(payddms);
	FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",DeductionID);
	if(EQLNRD(payddms,1)) KEYNRD(payddms,1);
		else prev=RDATDataNEW(payddms);
	if(!RDAstrcmp(dedlist->libs[x],"INS>50K")) VRSINS50K(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"FEDERAL")) FEDERAL(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"FICA")) FICA(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"FICA3RD")) FICA3RD(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"FICA-M")) FICAM(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"MEDICARE")) MEDICARE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"MEDICARE3RD")) MEDICARE3RD(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"MEDICARE-M")) MEDICAREM(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"AL STATE")) ALSTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"GA STATE")) GASTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"NY STATE")) NYSTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"NC STATE")) NCSTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"IL STATE")) ILSTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"WV STATE")) WVSTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"PSERS")) PSERS(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"PSERSM")) PSERSM(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VA STATE")) VASTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP DEP1")) VRSOLIPDEP1(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP DEP2")) VRSOLIPDEP2(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP DEP3")) VRSOLIPDEP3(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP DEP4")) VRSOLIPDEP4(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP EMP1")) VRSOLIPEMP1(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP EMP2")) VRSOLIPEMP2(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP EMP3")) VRSOLIPEMP3(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP EMP4")) VRSOLIPEMP4(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP SPC1")) VRSOLIPSPC1(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP SPC2")) VRSOLIPSPC2(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP SPC3")) VRSOLIPSPC3(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"VRS OLIP SPC4")) VRSOLIPSPC4(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND STATE")) NDSTATE(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND STATE PERCENTAGE")) NDSTATEPERC(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPASS MOD4")) NDTFFREMPASSMOD4(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPCON MOD4")) NDTFFREMPCONMOD4(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR MEMASS MOD4")) NDTFFRMEMASSMOD4(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPASS MOD3")) NDTFFREMPASSMOD3(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPCON MOD3")) NDTFFREMPCONMOD3(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR MEMASS MOD3")) NDTFFRMEMASSMOD3(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPASS MOD2")) NDTFFREMPASSMOD2(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPCON MOD2")) NDTFFREMPCONMOD2(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR MEMASS MOD1")) NDTFFRMEMASSMOD1(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPCON MOD1")) NDTFFREMPCONMOD1(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR MEMASS MOD0")) NDTFFRMEMASSMOD0(payddms);
	else if(!RDAstrcmp(dedlist->libs[x],"ND TFFR EMPCON MOD0")) NDTFFREMPCONMOD0(payddms);

	WRTTRANS(payddms,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);
	quitsetup(r);
}

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	char *temp=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"PAYROLL","COPY DEDUCTION SETUP")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	temp=RDA_GetEnv("DEDUCTION_ID");
	if(temp!=NULL) DeductionID=stralloc(temp);
	temp=RDA_GetEnv("D_TYPE");
	if(temp!=NULL) dedtype=stralloc(temp);
	payddms=OPNNRD("PAYROLL","PAYDDMS");
	if(payddms<0) 
	{
		prterr("Error:  Unable to Open Deduction Description Master (PAYDDMS).");
		ERRORDIALOG("UNABLE TO OPEN PAYDDMS","Unable to open the Deduction Description Master, file doesn't exist, lock, or security prohibits opening.",NULL,TRUE);
		RDAAPPMAINLOOP();
		return;
	}
	dedlist=APPlibNEW();
	addAPPlib(dedlist,"INS>50K");
	addAPPlib(dedlist,"FEDERAL");
	addAPPlib(dedlist,"FICA");
	addAPPlib(dedlist,"FICA-M");
	addAPPlib(dedlist,"MEDICARE");
	addAPPlib(dedlist,"MEDICARE-M");
#ifdef _MORE_DEDS_
	addAPPlib(dedlist,"FICA3RD");
	addAPPlib(dedlist,"MEDICARE3RD");
#endif /* _MORE_DEDS_ */
	if(!RDAstrcmp(XPERT_SETUP->state,"AL"))
	{
		addAPPlib(dedlist,"AL STATE");
	}
	if(!RDAstrcmp(XPERT_SETUP->state,"GA"))
	{
		addAPPlib(dedlist,"GA STATE");
	}
	if(!RDAstrcmp(XPERT_SETUP->state,"NY"))
	{
		addAPPlib(dedlist,"NY STATE");
	}
	if(!RDAstrcmp(XPERT_SETUP->state,"NC"))
	{
		addAPPlib(dedlist,"NC STATE");
	}
	if(!RDAstrcmp(XPERT_SETUP->state,"IL"))
	{
		addAPPlib(dedlist,"IL STATE");
	}
	if(!RDAstrcmp(XPERT_SETUP->state,"VA"))
	{
		addAPPlib(dedlist,"WV STATE");
		addAPPlib(dedlist,"VA STATE");
#ifdef _MORE_DEDS_
		addAPPlib(dedlist,"PSERS");
		addAPPlib(dedlist,"PSERSM");
		addAPPlib(dedlist,"VRS OLIP DEP1");
		addAPPlib(dedlist,"VRS OLIP DEP2");
		addAPPlib(dedlist,"VRS OLIP DEP3");
		addAPPlib(dedlist,"VRS OLIP DEP4");
		addAPPlib(dedlist,"VRS OLIP EMP1");
		addAPPlib(dedlist,"VRS OLIP EMP2");
		addAPPlib(dedlist,"VRS OLIP EMP3");
		addAPPlib(dedlist,"VRS OLIP EMP4");
		addAPPlib(dedlist,"VRS OLIP SPC1");
		addAPPlib(dedlist,"VRS OLIP SPC2");
		addAPPlib(dedlist,"VRS OLIP SPC3");
		addAPPlib(dedlist,"VRS OLIP SPC4");
#endif /* _MORE_DEDS_ */
	}
	if(!RDAstrcmp(XPERT_SETUP->state,"ND"))
	{
		addAPPlib(dedlist,"ND STATE");
#ifdef _MORE_DEDS_
		addAPPlib(dedlist,"ND STATE PERCENTAGE");
		addAPPlib(dedlist,"ND TFFR EMPASS MOD4");
		addAPPlib(dedlist,"ND TFFR EMPCON MOD4");
		addAPPlib(dedlist,"ND TFFR MEMASS MOD4");
		addAPPlib(dedlist,"ND TFFR EMPASS MOD3");
		addAPPlib(dedlist,"ND TFFR EMPCON MOD3");
		addAPPlib(dedlist,"ND TFFR MEMASS MOD3");
		addAPPlib(dedlist,"ND TFFR EMPASS MOD2");
		addAPPlib(dedlist,"ND TFFR EMPCON MOD2");
		addAPPlib(dedlist,"ND TFFR EMPASS MOD1");
		addAPPlib(dedlist,"ND TFFR EMPCON MOD1");
		addAPPlib(dedlist,"ND TFFR EMPASS MOD0");
		addAPPlib(dedlist,"ND TFFR EMPCON MOD0");
#endif /* _MORE_DEDS_ */
	}

	SORTAPPlib(dedlist);
	for(x=0;x<dedlist->numlibs;++x)
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s STATE",(XPERT_SETUP->state!=NULL ? XPERT_SETUP->state:""));
		if(!RDAstrcmp(dedlist->libs[x],stemp) && !RDAstrcmp(dedtype,"STATE")) break;
		else if(!RDAstrcmp(dedlist->libs[x],dedtype)) break;
	}
	mainrsrc=RDArsrcNEW("PAYROLL","COPY DEDUCTION SETUP");
	addstdrsrc(mainrsrc,"DEDUCTION ID",PLAINTEXT,30,DeductionID,FALSE);
	addlstrsrc(mainrsrc,"SETUPS",&x,TRUE,NULL,dedlist->numlibs,&dedlist->libs,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,setupded,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitsetup,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	mainrsrc->scn=RDAscrnNEW("PAYROLL","COPY DEDUCTION SETUP");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","Deduction Description ID:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"DEDUCTION ID","Deduction Description ID","","",0,30,0,"","FALSE","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","    Deduction Setup Type:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,7,"SETUPS","Deduction Setup Type","","",1,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,6,"SELECT","Select","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,6,"QUIT","Cancel","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,6,"HELP","Help","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	APPmakescrn(mainrsrc,TRUE,quitsetup,NULL,TRUE);
	RDAAPPMAINLOOP();

	
}
