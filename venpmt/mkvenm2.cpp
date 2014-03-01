/* mkvenm2.c - Make VENPMT's (MTN_VENYR) Maintain Master */
#include <stdio.h>
#include <mix.h>
#include <mkmtn.h>
#include <mkmsc.h>
#include <mkdef.h>

extern char *module;
extern APPlib *errorlist;
void MTN_VENYR(void);
static void VENDOR_YEAR_BROWSE(void);
static void VENDOR_YEAR_DEFINE_LIST(void);
static void VENDOR_YEAR_SEARCH_BROWSE(void);
static void VENDOR_YEAR_SEARCH_BROWSE_EXPRESSION(void);
static void VENDOR_YEAR_MAINTAIN_SCREEN(void);
static void DEF_VENDOR_YEAR_DEFINE_LIST(void);

void MTN_VENYR()
{
	char *defdir=NULL;
	RDAsearch *S=NULL;
	RDAsubord *SUBORD=NULL;
	PowerWrite *WRTE=NULL;
	short r=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	int x=0;
	MaintainButton *button=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(146+1);
	sprintf(temp1,"%s%s",
		"This maintain master definition is used to Add/View/Edit/Delete records in the Vendor Payment's Vend",
		"or Calendar Year Summary Master database file.");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");
	temp5=Rmalloc(88+1);
	sprintf(temp5,"%s",
		"IF([VENPMT][VENYR][DELETEFLAG]=TRUE) THEN RETURN_VALUE=TRUE \nELSE RETURN_VALUE=FALSE FI");
	temp6=Rmalloc(429+1);
	sprintf(temp6,"%s%s%s%s%s",
		"IF([VENPMT][VENYR][DELETEFLAG]=TRUE) THEN RETURN_VALUE=\"The DELETEFLAG has been marked as TRUE.  Ar",
		"e you sure that you want to DELETE this Vendor Calendar Year Summary record.  Deleting this record w",
		"ill also DELETE any Invoice records and Voucher records for this Vendor and Calendar Year.  Also, an",
		"y Vouchers that haven't been DISTRIBUTED and/or CLEARED will be backed out of FINANCIAL MANAGEMENT.",
		"\" \nELSE RETURN_VALUE=\"\" FI");
	MTNMSTR=RDAMaintainMasterNEW(module,"MTN VENYR",temp1,1,"VENYR","VENDOR YEAR BROWSE","VENDOR YEAR DEFINE LIST","VENDOR YEAR SEARCH BROWSE","VENDOR YEAR MAINTAIN SCREEN",0,0,0,0,temp2,temp3,temp4,temp5,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(MTNMSTR!=NULL)
	{
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupporting(MTNMSTR,"VENPMT","VENMSR",0,0,NULL,temp1,"VENDOR ID KEY",FALSE,"","","","","VENPMT","VENYR");
		if(temp1!=NULL) Rfree(temp1);
		S=MTNMSTR->s+(r-1);
		addSupportingRDAconfld(S,"VENDOR IDENTIFICATION","[VENYR][VENDOR IDENTIFICATION]",FALSE);



		SetRDAsearchOccurrence(MTNMSTR);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSubordinate(MTNMSTR,"VENPMT","VENINV",22,"","","VENDOR INVOICE KEY",temp1,TRUE);
		if(temp1!=NULL) Rfree(temp1);
		SUBORD=MTNMSTR->subord+(r-1);
		addSubordinateRDAconfld(SUBORD,"VENDOR IDENTIFICATION","[VENYR][VENDOR IDENTIFICATION]",FALSE);
		addSubordinateRDAconfld(SUBORD,"CALENDAR YEAR","[VENYR][CALENDAR YEAR]",FALSE);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addSubordinateWrite(SUBORD,"VENPMT","VENINV",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(39+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENINV][VENDOR IDENTIFICATION]");
		temp2=Rmalloc(39+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENINV][VENDOR IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENINV][CALENDAR YEAR]");
		temp2=Rmalloc(31+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENINV][CALENDAR YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(40+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENINV][INVOICE IDENTIFICATION]");
		temp2=Rmalloc(40+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENINV][INVOICE IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(28+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENINV][DELETEFLAG]");
		temp2=Rmalloc(4+1);
		sprintf(temp2,"%s",
			"TRUE");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addSubordinateWrite(SUBORD,"VENPMT","VENYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][VENDOR IDENTIFICATION]");
		temp2=Rmalloc(39+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENINV][VENDOR IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][CALENDAR YEAR]");
		temp2=Rmalloc(31+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENINV][CALENDAR YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(42+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][NUMBER OF INVOICES ISSUED]");
		temp2=Rmalloc(46+1);
		sprintf(temp2,"%s",
			"([VENPMT][VENYR][NUMBER OF INVOICES ISSUED]-1)");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"([USE VENPMT FISCAL SUMMARY]=TRUE)");
		x=addSubordinateWrite(SUBORD,"VENPMT","VENFYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(39+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENFYR][VENDOR IDENTIFICATION]");
		temp2=Rmalloc(39+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENINV][VENDOR IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENFYR][FISCAL YEAR]");
		temp2=Rmalloc(31+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENINV][CALENDAR YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENFYR][NUMBER OF INVOICES ISSUED]");
		temp2=Rmalloc(47+1);
		sprintf(temp2,"%s",
			"([VENPMT][VENFYR][NUMBER OF INVOICES ISSUED]-1)");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSubordinate(MTNMSTR,"VENPMT","VENVCH",22,"","","VENDOR VOUCHER KEY",temp1,TRUE);
		if(temp1!=NULL) Rfree(temp1);
		SUBORD=MTNMSTR->subord+(r-1);
		addSubordinateRDAconfld(SUBORD,"VENDOR IDENTIFICATION","[VENYR][VENDOR IDENTIFICATION]",FALSE);
		addSubordinateRDAconfld(SUBORD,"CALENDAR YEAR","[VENYR][CALENDAR YEAR]",FALSE);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addSubordinateWrite(SUBORD,"VENPMT","VENVCH",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(39+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
		temp2=Rmalloc(39+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENVCH][CALENDAR YEAR]");
		temp2=Rmalloc(31+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][CALENDAR YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(40+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
		temp2=Rmalloc(40+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(36+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENVCH][TRANSACTION NUMBER]");
		temp2=Rmalloc(36+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][TRANSACTION NUMBER]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(28+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENVCH][DELETEFLAG]");
		temp2=Rmalloc(4+1);
		sprintf(temp2,"%s",
			"TRUE");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addSubordinateWrite(SUBORD,"VENPMT","VENYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][VENDOR IDENTIFICATION]");
		temp2=Rmalloc(39+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][CALENDAR YEAR]");
		temp2=Rmalloc(31+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][CALENDAR YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JANUARY GROSS AMOUNT]");
		temp2=Rmalloc(183+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=0)\nTHEN RETURN_VALUE=[VENPMT][VENYR][JANUARY GROSS AMOUNT]-[V",
			"ENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][JANUARY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][FEBRUARY GROSS AMOUNT]");
		temp2=Rmalloc(185+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=1)\nTHEN RETURN_VALUE=[VENPMT][VENYR][FEBRUARY GROSS AMOUNT]-[",
			"VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][FEBRUARY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(35+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MARCH GROSS AMOUNT]");
		temp2=Rmalloc(179+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=2)\nTHEN RETURN_VALUE=[VENPMT][VENYR][MARCH GROSS AMOUNT]-[VEN",
			"PMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][MARCH GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(35+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][APRIL GROSS AMOUNT]");
		temp2=Rmalloc(179+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=3)\nTHEN RETURN_VALUE=[VENPMT][VENYR][APRIL GROSS AMOUNT]-[VEN",
			"PMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][APRIL GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MAY GROSS AMOUNT]");
		temp2=Rmalloc(175+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=4)\nTHEN RETURN_VALUE=[VENPMT][VENYR][MAY GROSS AMOUNT]-[VENPM",
			"T][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][MAY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JUNE GROSS AMOUNT]");
		temp2=Rmalloc(177+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=5)\nTHEN RETURN_VALUE=[VENPMT][VENYR][JUNE GROSS AMOUNT]-[VENP",
			"MT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][JUNE GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JULY GROSS AMOUNT]");
		temp2=Rmalloc(177+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=6)\nTHEN RETURN_VALUE=[VENPMT][VENYR][JULY GROSS AMOUNT]-[VENP",
			"MT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][JULY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(36+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][AUGUST GROSS AMOUNT]");
		temp2=Rmalloc(181+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=7)\nTHEN RETURN_VALUE=[VENPMT][VENYR][AUGUST GROSS AMOUNT]-[VE",
			"NPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][AUGUST GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(39+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT]");
		temp2=Rmalloc(187+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=8)\nTHEN RETURN_VALUE=[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT]-",
			"[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][OCTOBER GROSS AMOUNT]");
		temp2=Rmalloc(183+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=9)\nTHEN RETURN_VALUE=[VENPMT][VENYR][OCTOBER GROSS AMOUNT]-[V",
			"ENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][OCTOBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][NOVEMBER GROSS AMOUNT]");
		temp2=Rmalloc(186+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=10)\nTHEN RETURN_VALUE=[VENPMT][VENYR][NOVEMBER GROSS AMOUNT]-",
			"[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][NOVEMBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][DECEMBER GROSS AMOUNT]");
		temp2=Rmalloc(186+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=11)\nTHEN RETURN_VALUE=[VENPMT][VENYR][DECEMBER GROSS AMOUNT]-",
			"[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][DECEMBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(36+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JANUARY 1099 AMOUNT]");
		temp2=Rmalloc(226+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=0) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][JANUARY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENY",
			"R][JANUARY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][FEBRUARY 1099 AMOUNT]");
		temp2=Rmalloc(228+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=1) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][FEBRUARY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VEN",
			"YR][FEBRUARY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MARCH 1099 AMOUNT]");
		temp2=Rmalloc(222+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=2) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][MARCH 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR]",
			"[MARCH 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][APRIL 1099 AMOUNT]");
		temp2=Rmalloc(222+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=3) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][APRIL 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR]",
			"[APRIL 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(32+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MAY 1099 AMOUNT]");
		temp2=Rmalloc(218+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=4) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][MAY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][M",
			"AY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JUNE 1099 AMOUNT]");
		temp2=Rmalloc(220+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=5) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][JUNE 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][",
			"JUNE 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JULY 1099 AMOUNT]");
		temp2=Rmalloc(220+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=6) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][JULY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][",
			"JULY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(35+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][AUGUST 1099 AMOUNT]");
		temp2=Rmalloc(224+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=7) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][AUGUST 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR",
			"][AUGUST 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]");
		temp2=Rmalloc(230+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=8) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VE",
			"NYR][SEPTEMBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(36+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][OCTOBER 1099 AMOUNT]");
		temp2=Rmalloc(226+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=9) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][OCTOBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENY",
			"R][OCTOBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][NOVEMBER 1099 AMOUNT]");
		temp2=Rmalloc(229+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=10) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_",
			"VALUE=[VENPMT][VENYR][NOVEMBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VE",
			"NYR][NOVEMBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][DECEMBER 1099 AMOUNT]");
		temp2=Rmalloc(229+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=11) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_",
			"VALUE=[VENPMT][VENYR][DECEMBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VE",
			"NYR][DECEMBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"([USE VENPMT FISCAL SUMMARY]=TRUE)");
		x=addSubordinateWrite(SUBORD,"VENPMT","VENFYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(39+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENFYR][VENDOR IDENTIFICATION]");
		temp2=Rmalloc(39+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENFYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(35+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENFYR][APRIL 1099 AMOUNT]");
		temp2=Rmalloc(192+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1)\nTHEN RETURN_VALUE=[VENPMT][VENFYR][JANUARY GROSS AM",
			"OUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENFYR][JANUARY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][FEBRUARY GROSS AMOUNT]");
		temp2=Rmalloc(185+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=1)\nTHEN RETURN_VALUE=[VENPMT][VENYR][FEBRUARY GROSS AMOUNT]-[",
			"VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][FEBRUARY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(35+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MARCH GROSS AMOUNT]");
		temp2=Rmalloc(179+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=2)\nTHEN RETURN_VALUE=[VENPMT][VENYR][MARCH GROSS AMOUNT]-[VEN",
			"PMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][MARCH GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(35+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][APRIL GROSS AMOUNT]");
		temp2=Rmalloc(179+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=3)\nTHEN RETURN_VALUE=[VENPMT][VENYR][APRIL GROSS AMOUNT]-[VEN",
			"PMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][APRIL GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MAY GROSS AMOUNT]");
		temp2=Rmalloc(175+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=4)\nTHEN RETURN_VALUE=[VENPMT][VENYR][MAY GROSS AMOUNT]-[VENPM",
			"T][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][MAY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JUNE GROSS AMOUNT]");
		temp2=Rmalloc(177+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=5)\nTHEN RETURN_VALUE=[VENPMT][VENYR][JUNE GROSS AMOUNT]-[VENP",
			"MT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][JUNE GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JULY GROSS AMOUNT]");
		temp2=Rmalloc(177+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=6)\nTHEN RETURN_VALUE=[VENPMT][VENYR][JULY GROSS AMOUNT]-[VENP",
			"MT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][JULY GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(36+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][AUGUST GROSS AMOUNT]");
		temp2=Rmalloc(181+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=7)\nTHEN RETURN_VALUE=[VENPMT][VENYR][AUGUST GROSS AMOUNT]-[VE",
			"NPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][AUGUST GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(39+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT]");
		temp2=Rmalloc(187+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=8)\nTHEN RETURN_VALUE=[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT]-",
			"[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][OCTOBER GROSS AMOUNT]");
		temp2=Rmalloc(183+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=9)\nTHEN RETURN_VALUE=[VENPMT][VENYR][OCTOBER GROSS AMOUNT]-[V",
			"ENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][OCTOBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][NOVEMBER GROSS AMOUNT]");
		temp2=Rmalloc(186+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=10)\nTHEN RETURN_VALUE=[VENPMT][VENYR][NOVEMBER GROSS AMOUNT]-",
			"[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][NOVEMBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][DECEMBER GROSS AMOUNT]");
		temp2=Rmalloc(186+1);
		sprintf(temp2,"%s%s",
			"IF([VENPMT][VENVCH][PROCESSING MONTH]=11)\nTHEN RETURN_VALUE=[VENPMT][VENYR][DECEMBER GROSS AMOUNT]-",
			"[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][DECEMBER GROSS AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(36+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JANUARY 1099 AMOUNT]");
		temp2=Rmalloc(226+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=0) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][JANUARY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENY",
			"R][JANUARY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][FEBRUARY 1099 AMOUNT]");
		temp2=Rmalloc(228+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=1) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][FEBRUARY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VEN",
			"YR][FEBRUARY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MARCH 1099 AMOUNT]");
		temp2=Rmalloc(222+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=2) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][MARCH 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR]",
			"[MARCH 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(34+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][APRIL 1099 AMOUNT]");
		temp2=Rmalloc(222+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=3) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][APRIL 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR]",
			"[APRIL 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(32+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][MAY 1099 AMOUNT]");
		temp2=Rmalloc(218+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=4) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][MAY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][M",
			"AY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JUNE 1099 AMOUNT]");
		temp2=Rmalloc(220+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=5) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][JUNE 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][",
			"JUNE 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][JULY 1099 AMOUNT]");
		temp2=Rmalloc(220+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=6) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][JULY 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR][",
			"JULY 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(35+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][AUGUST 1099 AMOUNT]");
		temp2=Rmalloc(224+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=7) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][AUGUST 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENYR",
			"][AUGUST 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]");
		temp2=Rmalloc(230+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=8) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VE",
			"NYR][SEPTEMBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(36+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][OCTOBER 1099 AMOUNT]");
		temp2=Rmalloc(226+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=9) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_V",
			"ALUE=[VENPMT][VENYR][OCTOBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VENY",
			"R][OCTOBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][NOVEMBER 1099 AMOUNT]");
		temp2=Rmalloc(229+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=10) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_",
			"VALUE=[VENPMT][VENYR][NOVEMBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VE",
			"NYR][NOVEMBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(37+1);
		sprintf(temp1,"%s",
			"[VENPMT][VENYR][DECEMBER 1099 AMOUNT]");
		temp2=Rmalloc(229+1);
		sprintf(temp2,"%s%s%s",
			"IF(([VENPMT][VENVCH][PROCESSING MONTH]=11) AND ([VENPMT][VENVCH][1099 PAYMENT]=TRUE)) \nTHEN RETURN_",
			"VALUE=[VENPMT][VENYR][DECEMBER 1099 AMOUNT]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[VENPMT][VE",
			"NYR][DECEMBER 1099 AMOUNT] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(164+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]=0) AND \n([VENPMT][VENVCH][ACCOUNT CODE]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINEYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][ACCOUNT CODE]");
		temp2=Rmalloc(30+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][ACCOUNT CODE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(53+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(223+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][PRE-YEAR UNDIST",
			"RIBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][PRE-YEAR UNDISTR",
			"IBUTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(52+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(221+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][JANUARY UNDISTR",
			"IBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][JANUARY UNDISTRIB",
			"UTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(53+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(223+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY UNDIST",
			"RIBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY UNDISTR",
			"IBUTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(50+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(217+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][MARCH UNDISTRIB",
			"UTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][MARCH UNDISTRIBUTED",
			" EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(50+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(217+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][APRIL UNDISTRIB",
			"UTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][APRIL UNDISTRIBUTED",
			" EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][MAY UNDISTRIBUT",
			"ED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][MAY UNDISTRIBUTED EXP",
			"ENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(215+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][JUNE UNDISTRIBU",
			"TED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][JUNE UNDISTRIBUTED E",
			"XPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(215+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][JULY UNDISTRIBU",
			"TED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][JULY UNDISTRIBUTED E",
			"XPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(51+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(219+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][AUGUST UNDISTRI",
			"BUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][AUGUST UNDISTRIBUT",
			"ED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(54+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(225+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER UNDIS",
			"TRIBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER UNDIS",
			"TRIBUTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(52+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(222+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][OCTOBER UNDIST",
			"RIBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][OCTOBER UNDISTRI",
			"BUTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(53+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(224+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER UNDIS",
			"TRIBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER UNDIST",
			"RIBUTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(53+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(224+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][DECEMBER UNDIS",
			"TRIBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][DECEMBER UNDIST",
			"RIBUTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(54+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED EXPENDITURES]");
		temp2=Rmalloc(226+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINEYR][POST YEAR UNDI",
			"STRIBUTED EXPENDITURES]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINEYR][POST YEAR UNDI",
			"STRIBUTED EXPENDITURES] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(164+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]=1) AND \n([VENPMT][VENVCH][ACCOUNT CODE]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINRYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][ACCOUNT CODE]");
		temp2=Rmalloc(30+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][ACCOUNT CODE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][PRE-YEAR UNDIST",
			"RIBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTE",
			"D REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][JANUARY UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][JANUARY UNDISTR",
			"IBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][JANUARY UNDISTRIBUTED ",
			"REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][FEBRUARY UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][FEBRUARY UNDIST",
			"RIBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][FEBRUARY UNDISTRIBUTE",
			"D REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][MARCH UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][MARCH UNDISTRIB",
			"UTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][MARCH UNDISTRIBUTED REVE",
			"NUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][APRIL UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][APRIL UNDISTRIB",
			"UTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][APRIL UNDISTRIBUTED REVE",
			"NUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][MAY UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][MAY UNDISTRIBUT",
			"ED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][MAY UNDISTRIBUTED REVENUE]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][JUNE UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][JUNE UNDISTRIBU",
			"TED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][JUNE UNDISTRIBUTED REVENU",
			"E] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][JULY UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][JULY UNDISTRIBU",
			"TED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][JULY UNDISTRIBUTED REVENU",
			"E] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][AUGUST UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(209+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][AUGUST UNDISTRI",
			"BUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][AUGUST UNDISTRIBUTED RE",
			"VENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][SEPTEMBER UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(215+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][SEPTEMBER UNDIS",
			"TRIBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][SEPTEMBER UNDISTRIBU",
			"TED REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][OCTOBER UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][OCTOBER UNDIST",
			"RIBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][OCTOBER UNDISTRIBUTED",
			" REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][NOVEMBER UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][NOVEMBER UNDIS",
			"TRIBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][NOVEMBER UNDISTRIBUT",
			"ED REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][DECEMBER UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][DECEMBER UNDIS",
			"TRIBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][DECEMBER UNDISTRIBUT",
			"ED REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINRYR][POST YEAR UNDISTRIBUTED REVENUE]");
		temp2=Rmalloc(216+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINRYR][POST YEAR UNDI",
			"STRIBUTED REVENUE]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINRYR][POST YEAR UNDISTRIB",
			"UTED REVENUE] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(162+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]<3) AND \n([VENPMT][VENVCH][DEBIT CODE]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINBYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][ACCOUNT CODE]");
		temp2=Rmalloc(28+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEBIT CODE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED",
			" DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(209+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTR",
			"IBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED D",
			"EBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED",
			" DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIB",
			"UTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIB",
			"UTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(42+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(201+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUT",
			"ED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS] F",
			"I");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBU",
			"TED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBU",
			"TED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRI",
			"BUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEB",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUT",
			"ED DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(210+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED ",
			"DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTE",
			"D DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTE",
			"D DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDI",
			"STRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIBU",
			"TED DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(163+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]<3) AND \n([VENPMT][VENVCH][CREDIT CODE]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINBYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][ACCOUNT CODE]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][CREDIT CODE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTE",
			"D CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTR",
			"IBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED ",
			"CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTE",
			"D CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIB",
			"UTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CRED",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIB",
			"UTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CRED",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUT",
			"ED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBU",
			"TED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBU",
			"TED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(209+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRI",
			"BUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CR",
			"EDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(215+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBU",
			"TED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED",
			" CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUT",
			"ED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUT",
			"ED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(216+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDI",
			"STRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIB",
			"UTED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(164+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]<3) AND \n([VENPMT][VENVCH][PAYABLE CODE]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINBYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][ACCOUNT CODE]");
		temp2=Rmalloc(30+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][PAYABLE CODE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED",
			" DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(209+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTR",
			"IBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED D",
			"EBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED",
			" DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIB",
			"UTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIB",
			"UTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(42+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(201+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUT",
			"ED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS] F",
			"I");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBU",
			"TED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBU",
			"TED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRI",
			"BUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEB",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUT",
			"ED DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(210+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED ",
			"DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTE",
			"D DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTE",
			"D DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDI",
			"STRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIBU",
			"TED DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(164+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]<3) AND \n([VENPMT][VENVCH][PAYABLE CODE]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINBYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][ACCOUNT CODE]");
		temp2=Rmalloc(30+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][PAYABLE CODE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTE",
			"D CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTR",
			"IBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED ",
			"CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTE",
			"D CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIB",
			"UTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CRED",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIB",
			"UTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CRED",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUT",
			"ED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBU",
			"TED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBU",
			"TED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(209+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRI",
			"BUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CR",
			"EDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(215+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBU",
			"TED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED",
			" CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUT",
			"ED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUT",
			"ED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(216+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDI",
			"STRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIB",
			"UTED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(160+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]<2) AND \n([VENPMT][VENVCH][DUE FROM]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINBYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][ACCOUNT CODE]");
		temp2=Rmalloc(26+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DUE FROM]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED",
			" DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(209+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTR",
			"IBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED D",
			"EBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED",
			" DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIB",
			"UTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIB",
			"UTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(42+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(201+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUT",
			"ED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS] F",
			"I");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBU",
			"TED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBU",
			"TED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRI",
			"BUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEB",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUT",
			"ED DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(210+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDIST",
			"RIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED ",
			"DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTE",
			"D DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDIS",
			"TRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTE",
			"D DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDI",
			"STRIBUTED DEBITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIBU",
			"TED DEBITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(165+1);
		sprintf(temp1,"%s%s",
			"([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][CLEARED]=FALSE) AND  \n([VENPMT][VENVC",
			"H][ACCOUNT TYPE]<2) AND \n([VENPMT][VENVCH][CLEARING CASH]!=\"\")");
		x=addSubordinateWrite(SUBORD,"FINMGT","FINBYR",temp1);
		if(temp1!=NULL) Rfree(temp1);
		WRTE=SUBORD->Writes+(x-1);
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][ACCOUNT CODE]");
		temp2=Rmalloc(31+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][CLEARING CASH]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(33+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DEFINITION TYPE]");
		temp2=Rmalloc(33+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][DEFINITION TYPE]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FISCAL YEAR]");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"[VENPMT][VENVCH][FISCAL YEAR]");
		addMaintainMasterWriteField(WRTE,temp1,0,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTE",
			"D CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(211+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTR",
			"IBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED ",
			"CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(213+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTE",
			"D CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIB",
			"UTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CRED",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(45+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(207+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIB",
			"UTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CRED",
			"ITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(43+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(203+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUT",
			"ED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]",
			" FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBU",
			"TED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(44+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(205+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBU",
			"TED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDIT",
			"S] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(46+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(209+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRI",
			"BUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CR",
			"EDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(215+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBU",
			"TED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(212+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDIST",
			"RIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED",
			" CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUT",
			"ED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(48+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(214+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDIS",
			"TRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUT",
			"ED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]");
		temp2=Rmalloc(216+1);
		sprintf(temp2,"%s%s%s",
			"IF([VENPMT][VENVCH][FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDI",
			"STRIBUTED CREDITS]-[VENPMT][VENVCH][AMOUNT] \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIB",
			"UTED CREDITS] FI");
		addMaintainMasterWriteField(WRTE,temp1,1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);



		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"VENPMT");
		addAPPlib(args,"MTN VENINV");
		x=addMaintainMasterButton(MTNMSTR,"mtnmst","Maintain Vendor Invoices","VENDOR INVOICES","VENPMT","VENINV","VENDOR INVOICE KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"VENDOR IDENTIFICATION","VENDOR_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"CALENDAR YEAR","CALENDAR_YEAR");
		}
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"VENPMT");
		addAPPlib(args,"MTN VENVCH");
		x=addMaintainMasterButton(MTNMSTR,"mtnmst","Maintain Vendor Vouchers","VENDOR VOUCHERS","VENPMT","VENVCH","VENDOR VOUCHER KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"VENDOR IDENTIFICATION","VENDOR_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"CALENDAR YEAR","CALENDAR_YEAR");
		}


		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MTN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,module);
#endif
		if(writeMaintainMaster(defdir,MTNMSTR))
		{
/*
			ERRORDIALOG("CANNOT SAVE MAINTAIN MASTER","This Maintain Master definition may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+6+89+1);
			sprintf(temp1,"MAINTAIN MASTER WRITE ERROR: Module [VENPMT] Maintain Master [MTN VENYR], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		} else {
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
			VENDOR_YEAR_BROWSE();
			VENDOR_YEAR_DEFINE_LIST();
			VENDOR_YEAR_SEARCH_BROWSE();
			VENDOR_YEAR_SEARCH_BROWSE_EXPRESSION();
			VENDOR_YEAR_MAINTAIN_SCREEN();
			DEF_VENDOR_YEAR_DEFINE_LIST();
		}
	}
}
static void VENDOR_YEAR_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("VENPMT","VENDOR YEAR BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Run Function:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"RUN FUNCTION","Run Function","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"([VENPMT][VENMSR][VENDOR IDENTIFICATION]!=\"\")","","","");
		ADVaddwdgt(scrn,5,"","Vendor Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENMSR][VENDOR IDENTIFICATION]","Vendor Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENMSR][VENDOR NAME]","Vendor Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PRINT BROWSE","Print Browse","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search Field:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [VENPMT] Screen [%s], Can Not Save Screen!",(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void VENDOR_YEAR_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("VENPMT","VENDOR YEAR DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Enter the position and length for the following fields:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Vendor Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"VENDOR IDENTIFICATION POSITION","Vendor Identification Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"VENDOR IDENTIFICATION LENGTH","Vendor Identification Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Calendar Year:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"CALENDAR YEAR POSITION","Calendar Year Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"CALENDAR YEAR LENGTH","Calendar Year Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Annual Number of Inv.:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"NUMBER OF INVOICES ISSUED POSITION","Number of Invoices Issued Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"NUMBER OF INVOICES ISSUED LENGTH","Number of Invoices Issued Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Annual Gross Amount:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"ANNUAL CALENDAR GROSS AMOUNT POSITION","Annual Calendar Gross Amount Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"ANNUAL CALENDAR GROSS AMOUNT LENGTH","Annual Calendar Gross Amount Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Annual 1099 Amount:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"ANNUAL CALENDAR 1099 AMOUNT POSITION","Annual Calendar 1099 Amount Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"ANNUAL CALENDAR 1099 AMOUNT LENGTH","Annual Calendar 1099 Amount Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [VENPMT] Screen [%s], Can Not Save Screen!",(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void VENDOR_YEAR_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("VENPMT","VENDOR YEAR SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [VENYR][CALENDAR YEAR]","Range on Calendar Year","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [VENYR][CALENDAR YEAR]","From Calendar Year","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [VENYR][CALENDAR YEAR]","To Calendar Year","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SELECT [VENYR][DELETEFLAG] FALSE","Select non-deleted records","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SELECT [VENYR][DELETEFLAG] TRUE","Select deleted records","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,14,"VENDOR YEAR SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [VENPMT] Screen [%s], Can Not Save Screen!",(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}

static void VENDOR_YEAR_MAINTAIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	DFvirtual *d=NULL;

	d=getDFvirtual(module,"ANNUAL CALENDAR 1099 AMOUNT");
	if(d==NULL)
	{
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(120+1);
		sprintf(temp1,"VIRTUAL FIELD ERROR: Screen [VENDOR YEAR MAINTAIN SCREEN], Virtual Field Not Found [ANNUAL CALENDAR 1099 AMOUNT]");
		prterr(temp1);
		if(errorlist!=NULL)
		{
			addERRlist(errorlist,temp1);
		}
		if(temp1!=NULL) Rfree(temp1);
	} else {
		FreeDFvirtual(d);
	}
	d=getDFvirtual(module,"ANNUAL CALENDAR GROSS AMOUNT");
	if(d==NULL)
	{
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(120+1);
		sprintf(temp1,"VIRTUAL FIELD ERROR: Screen [VENDOR YEAR MAINTAIN SCREEN], Virtual Field Not Found [ANNUAL CALENDAR GROSS AMOUNT]");
		prterr(temp1);
		if(errorlist!=NULL)
		{
			addERRlist(errorlist,temp1);
		}
		if(temp1!=NULL) Rfree(temp1);
	} else {
		FreeDFvirtual(d);
	}
	scrn=RDAscrnNEW("VENPMT","VENDOR YEAR MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[VENYR][DELETEFLAG]","Delete this record","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Vendor Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PREVIOUS YEAR VENDOR KEY","Previous Year Vendor Key","","<",0,0,2,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][VENDOR IDENTIFICATION]","Vendor Identification","","",0,30,0,"","","","");
		ADVaddwdgt(scrn,6,"NEXT YEAR VENDOR KEY","Previous Year Vendor Key","",">",0,0,1,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENMSR][VENDOR NAME]","Vendor Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Calendar Year:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PREVIOUS VENDOR YEAR KEY","Previous Vendor Year Key","","<",0,0,2,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][CALENDAR YEAR]","Calendar Year","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,6,"NEXT VENDOR YEAR KEY","Next Vendor Year Key","",">",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Month","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Gross Amount","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","1099 Amount","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Month","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Gross Amount","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","1099 Amount","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","January:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][JANUARY GROSS AMOUNT]","January Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][JANUARY 1099 AMOUNT]","January 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","February:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][FEBRUARY GROSS AMOUNT]","February Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][FEBRUARY 1099 AMOUNT]","February 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","March:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][MARCH GROSS AMOUNT]","March Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][MARCH 1099 AMOUNT]","March 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","April:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][APRIL GROSS AMOUNT]","April Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][APRIL 1099 AMOUNT]","April 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","May:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][MAY GROSS AMOUNT]","May Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][MAY 1099 AMOUNT]","May 1099  Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","June:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][JUNE GROSS AMOUNT]","June Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][JUNE 1099 AMOUNT]","June 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","July:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][JULY GROSS AMOUNT]","July Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][JULY 1099 AMOUNT]","July 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","August:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][AUGUST GROSS AMOUNT]","August Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][AUGUST 1099 AMOUNT]","August 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","September:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][SEPTEMBER GROSS AMOUNT]","September Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][SEPTEMBER 1099 AMOUNT]","September 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","October:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][OCTOBER GROSS AMOUNT]","October Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][OCTOBER 1099 AMOUNT]","October 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","November:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][NOVEMBER GROSS AMOUNT]","November Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][NOVEMBER 1099 AMOUNT]","November 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","December:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][DECEMBER GROSS AMOUNT]","December Gross Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][DECEMBER 1099 AMOUNT]","December 1099 Amount","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","(Year Totals)","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","# Invoices:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[VENYR][NUMBER OF INVOICES ISSUED]","Number of Invoices Issued:","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,5,"","Gross:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"ANNUAL CALENDAR GROSS AMOUNT","Annual Calendar Gross Amount","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","1099's:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"ANNUAL CALENDAR 1099 AMOUNT","Annual Calendar 1099 Amount","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"BROWSE","Browse","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"VENDOR INVOICES","Invoices","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"VENDOR VOUCHERS","Vouchers","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RESET DEFAULTS","Reset Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [VENPMT] Screen [%s], Can Not Save Screen!",(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void VENDOR_YEAR_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("VENPMT","VENDOR YEAR SEARCH BROWSE EXPRESSION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Expression:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"EXPRESSION","Expression","","",5,80,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD VALUE","Load Value","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD GROUPER","Load Grouper","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD OPERATOR","Load Operator","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD CONTROL","Load Control","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [VENPMT] Screen [%s], Can Not Save Screen!",(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}

static void DEF_VENDOR_YEAR_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("VENPMT","VENDOR YEAR DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus="DELETEFLAG POSITION";
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,1);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"CALENDAR YEAR POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"CALENDAR YEAR LENGTH",TRUE,7,4);
		addDefaultFieldSHORT(def,"JANUARY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JANUARY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JANUARY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JANUARY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"NUMBER OF INVOICES ISSUED POSITION",TRUE,7,4);
		addDefaultFieldSHORT(def,"NUMBER OF INVOICES ISSUED LENGTH",TRUE,7,6);
		addDefaultFieldSHORT(def,"VENDOR IDENTIFICATION POSITION",TRUE,7,2);
		addDefaultFieldSHORT(def,"VENDOR IDENTIFICATION LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR GROSS AMOUNT POSITION",TRUE,7,5);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR GROSS AMOUNT LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR 1099 AMOUNT POSITION",TRUE,7,6);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR 1099 AMOUNT LENGTH",TRUE,7,20);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
		if(writedefaultbin(defdir,def))
		{
/*
			ERRORDIALOG("Cannot Save Screen Defaults","This screen default may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+6+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [VENPMT] Screen [VENDOR YEAR DEFINE LIST], Can Not Save Screen Defaults!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(def!=NULL) FreeRDAdefault(def);
	}
}
