/* mkven1.c - Make VENPMT's Database Files */
#include <stdio.h>
#include <mix.h>
#include <mknrd.h>
#include <mkmsc.h>

extern APPlib *errorlist;

void DB_VENADR(void);
void DB_VENFYR(void);
void DB_VENINV(void);
void DB_VENMSR(void);
void DB_VENRMSR(void);
void DB_VENRVCH(void);
void DB_VENVCH(void);
void DB_VENYR(void);

void DB_VENADR()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENADR",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"DEFAULT BILLING",10,1);
		addDBfield(sort,"DEFAULT ORDER",10,1);
		addDBfield(sort,"DEFAULT RFP",10,1);
		addDBfield(sort,"ZIP",5,10);
		addDBfield(sort,"PHONE",3,12);
		addDBfield(sort,"FAX",3,12);
		addDBfield(sort,"STATE",1,2);
		addDBfield(sort,"VENDOR IDENTIFICATION",1,15);
		addDBfield(sort,"ADDRESS IDENTIFICATION",1,15);
		addDBfield(sort,"ADDRESS 1",1,40);
		addDBfield(sort,"ADDRESS 2",1,40);
		addDBfield(sort,"CITY",1,40);
		addDBfield(sort,"REPRESENTATIVE",1,40);
		addDBfield(sort,"COMMENTS",14,400);
		addDBkey(sort,"VENDOR ADDRESS KEY");
		addDBkeypart(sort,1,"VENDOR IDENTIFICATION");
		addDBkeypart(sort,1,"ADDRESS IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+6+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENADR], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_VENFYR()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENFYR",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"FISCAL YEAR",8,4);
		addDBfield(sort,"PRE-YEAR GROSS AMOUNT",2,15);
		addDBfield(sort,"PRE-YEAR 1099 AMOUNT",2,15);
		addDBfield(sort,"JANUARY GROSS AMOUNT",2,15);
		addDBfield(sort,"JANUARY 1099 AMOUNT",2,15);
		addDBfield(sort,"FEBRUARY GROSS AMOUNT",2,15);
		addDBfield(sort,"FEBRUARY 1099 AMOUNT",2,15);
		addDBfield(sort,"MARCH GROSS AMOUNT",2,15);
		addDBfield(sort,"MARCH 1099 AMOUNT",2,15);
		addDBfield(sort,"APRIL GROSS AMOUNT",2,15);
		addDBfield(sort,"APRIL 1099 AMOUNT",2,15);
		addDBfield(sort,"MAY GROSS AMOUNT",2,15);
		addDBfield(sort,"MAY 1099 AMOUNT",2,15);
		addDBfield(sort,"JUNE GROSS AMOUNT",2,15);
		addDBfield(sort,"JUNE 1099 AMOUNT",2,15);
		addDBfield(sort,"JULY GROSS AMOUNT",2,15);
		addDBfield(sort,"JULY 1099 AMOUNT",2,15);
		addDBfield(sort,"AUGUST GROSS AMOUNT",2,15);
		addDBfield(sort,"AUGUST 1099 AMOUNT",2,15);
		addDBfield(sort,"SEPTEMBER GROSS AMOUNT",2,15);
		addDBfield(sort,"SEPTEMBER 1099 AMOUNT",2,15);
		addDBfield(sort,"OCTOBER GROSS AMOUNT",2,15);
		addDBfield(sort,"OCTOBER 1099 AMOUNT",2,15);
		addDBfield(sort,"NOVEMBER GROSS AMOUNT",2,15);
		addDBfield(sort,"NOVEMBER 1099 AMOUNT",2,15);
		addDBfield(sort,"DECEMBER GROSS AMOUNT",2,15);
		addDBfield(sort,"DECEMBER 1099 AMOUNT",2,15);
		addDBfield(sort,"POST YEAR GROSS AMOUNT",2,15);
		addDBfield(sort,"POST YEAR 1099 AMOUNT",2,15);
		addDBfield(sort,"NUMBER OF INVOICES ISSUED",8,6);
		addDBfield(sort,"VENDOR IDENTIFICATION",1,15);
		addDBkey(sort,"VENFYR KEY");
		addDBkeypart(sort,1,"VENDOR IDENTIFICATION");
		addDBkeypart(sort,1,"FISCAL YEAR");
		addDBkey(sort,"VENFYR YEAR KEY");
		addDBkeypart(sort,2,"FISCAL YEAR");
		addDBkeypart(sort,2,"VENDOR IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+6+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENFYR], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_VENINV()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENINV",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"ENTRY DATE",16,8);
		addDBfield(sort,"ENTRY TIME",17,8);
		addDBfield(sort,"INVOICE DATE",16,8);
		addDBfield(sort,"DUE DATE",16,8);
		addDBfield(sort,"CALENDAR YEAR",8,4);
		addDBfield(sort,"FROM RECURRING VOUCHER",10,1);
		addDBfield(sort,"VENDOR IDENTIFICATION",1,15);
		addDBfield(sort,"INVOICE IDENTIFICATION",1,15);
		addDBfield(sort,"SOURCE USER",1,15);
		addDBfield(sort,"ADDRESS IDENTIFICATION",1,15);
		addDBfield(sort,"RECURRING VOUCHER IDENTIFICATION",1,15);
		addDBkey(sort,"VENDOR INVOICE KEY");
		addDBkeypart(sort,1,"VENDOR IDENTIFICATION");
		addDBkeypart(sort,1,"CALENDAR YEAR");
		addDBkeypart(sort,1,"INVOICE IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+6+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENINV], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_VENMSR()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENMSR",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"ACTIVE",10,1);
		addDBfield(sort,"DEFAULT CHECK DETAIL",10,1);
		addDBfield(sort,"DEFAULT 1099 PAYMENT",10,1);
		addDBfield(sort,"DEFAULT TAX TYPE",7,1);
		addDBfield(sort,"CATALOGUE VENDOR",10,1);
		addDBfield(sort,"RFP VENDOR",10,1);
		addDBfield(sort,"PREVIOUS VENDOR NUMBER",1,6);
		addDBfield(sort,"DEFAULT PERCENTAGE DISCOUNT",6,5);
		addDBfield(sort,"TAX IDENTIFICATION",1,15);
		addDBfield(sort,"VENDOR IDENTIFICATION",1,15);
		addDBfield(sort,"VENDOR NAME",1,40);
		addDBfield(sort,"1099 NAME",1,40);
		addDBfield(sort,"REMARKS",1,80);
		addDBkey(sort,"VENDOR ID KEY");
		addDBkeypart(sort,1,"VENDOR IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+6+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENMSR], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_VENRMSR()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENRMSR",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"RECURRING IDENTIFICATION",1,15);
		addDBfield(sort,"DESCRIPTION",1,60);
		addDBkey(sort,"VENDOR RECURRING VOUCHER KEY");
		addDBkeypart(sort,1,"RECURRING IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+6+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENRMSR], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_VENRVCH()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENRVCH",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"TRANSACTION NUMBER",8,6);
		addDBfield(sort,"PRORATION",10,1);
		addDBfield(sort,"PRORATION PERCENTAGE",6,5);
		addDBfield(sort,"FLAT AMOUNT",2,15);
		addDBfield(sort,"ACCOUNT TYPE",7,1);
		addDBfield(sort,"DEFINITION TYPE",7,1);
		addDBfield(sort,"ACCOUNT CODE",22,40);
		addDBfield(sort,"DEBIT CODE",24,40);
		addDBfield(sort,"PAYABLE CODE",24,40);
		addDBfield(sort,"CREDIT CODE",24,40);
		addDBfield(sort,"PREPAID",10,1);
		addDBfield(sort,"1099 PAYMENT",10,1);
		addDBfield(sort,"CHECK DETAIL",10,1);
		addDBfield(sort,"RECURRING IDENTIFICATION",1,15);
		addDBfield(sort,"VENDOR IDENTIFICATION",1,15);
		addDBfield(sort,"ADDRESS IDENTIFICATION",1,15);
		addDBfield(sort,"PRORATION IDENTIFICATION",1,15);
		addDBfield(sort,"DESCRIPTION",1,60);
		addDBkey(sort,"RECURRING VOUCHERS KEY");
		addDBkeypart(sort,1,"RECURRING IDENTIFICATION");
		addDBkeypart(sort,1,"TRANSACTION NUMBER");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+7+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENRVCH], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_VENVCH()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENVCH",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"TRANSACTION NUMBER",8,6);
		addDBfield(sort,"SOURCE USER VERIFIED",10,1);
		addDBfield(sort,"ENTRY DATE",16,8);
		addDBfield(sort,"ENTRY TIME",17,8);
		addDBfield(sort,"PO VOUCHER",10,1);
		addDBfield(sort,"PO LINE NUMBER",8,4);
		addDBfield(sort,"PO ACCOUNT NUMBER",8,4);
		addDBfield(sort,"MANUAL VOUCHER",10,1);
		addDBfield(sort,"VIRTUAL CHECK NUMBER",8,12);
		addDBfield(sort,"CHECK NUMBER",8,12);
		addDBfield(sort,"CHECK DATE",16,8);
		addDBfield(sort,"AMOUNT",2,15);
		addDBfield(sort,"ACCOUNT TYPE",7,1);
		addDBfield(sort,"DEFINITION TYPE",7,1);
		addDBfield(sort,"ACCOUNT CODE",22,40);
		addDBfield(sort,"DEBIT CODE",24,40);
		addDBfield(sort,"PAYABLE CODE",24,40);
		addDBfield(sort,"CREDIT CODE",24,40);
		addDBfield(sort,"CLEARED",10,1);
		addDBfield(sort,"CLEARED DATE",16,8);
		addDBfield(sort,"CLEARED TIME",17,8);
		addDBfield(sort,"CLEAR FISCAL YEAR",8,4);
		addDBfield(sort,"CLEAR PROCESSING MONTH",7,2);
		addDBfield(sort,"DISTRIBUTED",10,1);
		addDBfield(sort,"DISTRIBUTED DATE",16,8);
		addDBfield(sort,"DISTRIBUTED TIME",17,8);
		addDBfield(sort,"PROCESSING MONTH",7,2);
		addDBfield(sort,"BNKREC UPDATED",10,1);
		addDBfield(sort,"BNKREC UPDATED DATE",16,8);
		addDBfield(sort,"BNKREC UPDATED TIME",17,8);
		addDBfield(sort,"PREPAID",10,1);
		addDBfield(sort,"1099 PAYMENT",10,1);
		addDBfield(sort,"CHECK DETAIL",10,1);
		addDBfield(sort,"CALENDAR YEAR",8,4);
		addDBfield(sort,"FISCAL YEAR",8,4);
		addDBfield(sort,"FISCAL PROCESSING MONTH",7,2);
		addDBfield(sort,"CLEARED BY",1,15);
		addDBfield(sort,"DISTRIBUTED BY",1,15);
		addDBfield(sort,"BNKREC UPDATED BY",1,15);
		addDBfield(sort,"SOURCE USER",1,15);
		addDBfield(sort,"VENDOR IDENTIFICATION",1,15);
		addDBfield(sort,"INVOICE IDENTIFICATION",1,15);
		addDBfield(sort,"PO IDENTIFICATION",1,15);
		addDBfield(sort,"BANK IDENTIFICATION",1,25);
		addDBfield(sort,"DESCRIPTION",1,60);
		addDBfield(sort,"CLEARING DEFINITION TYPE",7,2);
		addDBfield(sort,"CLEARING DUE FROM",24,40);
		addDBfield(sort,"CLEARING CASH",24,40);
		addDBfield(sort,"TAX TYPE",7,1);
		addDBfield(sort,"COST",2,20);
		addDBfield(sort,"TAX AMOUNT",2,20);
		addDBfield(sort,"ADMIN FEE",2,20);
		addDBfield(sort,"PRORATE AMOUNT",2,20);
		addDBfield(sort,"PRORATE TYPE",1,25);
		addDBkey(sort,"VENDOR VOUCHER KEY");
		addDBkeypart(sort,1,"VENDOR IDENTIFICATION");
		addDBkeypart(sort,1,"CALENDAR YEAR");
		addDBkeypart(sort,1,"INVOICE IDENTIFICATION");
		addDBkeypart(sort,1,"TRANSACTION NUMBER");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+6+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENVCH], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_VENYR()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("VENPMT","VENYR",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"CALENDAR YEAR",8,4);
		addDBfield(sort,"JANUARY GROSS AMOUNT",2,15);
		addDBfield(sort,"JANUARY 1099 AMOUNT",2,15);
		addDBfield(sort,"FEBRUARY GROSS AMOUNT",2,15);
		addDBfield(sort,"FEBRUARY 1099 AMOUNT",2,15);
		addDBfield(sort,"MARCH GROSS AMOUNT",2,15);
		addDBfield(sort,"MARCH 1099 AMOUNT",2,15);
		addDBfield(sort,"APRIL GROSS AMOUNT",2,15);
		addDBfield(sort,"APRIL 1099 AMOUNT",2,15);
		addDBfield(sort,"MAY GROSS AMOUNT",2,15);
		addDBfield(sort,"MAY 1099 AMOUNT",2,15);
		addDBfield(sort,"JUNE GROSS AMOUNT",2,15);
		addDBfield(sort,"JUNE 1099 AMOUNT",2,15);
		addDBfield(sort,"JULY GROSS AMOUNT",2,15);
		addDBfield(sort,"JULY 1099 AMOUNT",2,15);
		addDBfield(sort,"AUGUST GROSS AMOUNT",2,15);
		addDBfield(sort,"AUGUST 1099 AMOUNT",2,15);
		addDBfield(sort,"SEPTEMBER GROSS AMOUNT",2,15);
		addDBfield(sort,"SEPTEMBER 1099 AMOUNT",2,15);
		addDBfield(sort,"OCTOBER GROSS AMOUNT",2,15);
		addDBfield(sort,"OCTOBER 1099 AMOUNT",2,15);
		addDBfield(sort,"NOVEMBER GROSS AMOUNT",2,15);
		addDBfield(sort,"NOVEMBER 1099 AMOUNT",2,15);
		addDBfield(sort,"DECEMBER GROSS AMOUNT",2,15);
		addDBfield(sort,"DECEMBER 1099 AMOUNT",2,15);
		addDBfield(sort,"NUMBER OF INVOICES ISSUED",8,6);
		addDBfield(sort,"VENDOR IDENTIFICATION",1,15);
		addDBkey(sort,"VENDOR YEAR KEY");
		addDBkeypart(sort,1,"VENDOR IDENTIFICATION");
		addDBkeypart(sort,1,"CALENDAR YEAR");
		addDBkey(sort,"YEAR VENDOR KEY");
		addDBkeypart(sort,2,"CALENDAR YEAR");
		addDBkeypart(sort,2,"VENDOR IDENTIFICATION");


		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+6+79+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [VENPMT] Screen [VENYR], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
