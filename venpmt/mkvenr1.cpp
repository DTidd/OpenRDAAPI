/* VENPMT's - Make (VCHDIST) Report - mkvenr1.c */
#include <stdio.h>
#include <mkmsc.h>
#include <mkvir.h>

extern char *module;
extern APPlib *errorlist;
static void RPT_VCHDISTa(void);
static void RPT_VCHDIST1a(void);
static void VCHDIST_RANGE_SCREEN(void);

void RPT_VCHDIST()
{
	RPT_VCHDISTa();
	RPT_VCHDIST1a();
	VCHDIST_RANGE_SCREEN();
}
void RPT_VCHDISTa()
{
	int x;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	RDApval *pval=NULL,*pvals=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;
	char *temp1=NULL,*temp2=NULL;

	DFvirtual *d=NULL;

	memset(stemp,0,101);
	rptname=Rmalloc(7+1);
	sprintf(rptname,"VCHDIST");
	t=Rmalloc(27+1);
	sprintf(t,"VENDOR VOUCHER DISTRIBUTION");
	desc=Rmalloc(132+1);
	sprintf(desc,"This output definition is used to produce the vendor distribution activity report to verify the vendor voucher(s) to be distributed.");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,TRUE,TRUE,"VENPMT","VCHDIST1",TRUE,1,FALSE,"");
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);

	pvars=NULL;
	adddisplayADV(tmp,(double)8.500000,(double)10.000000,0,FALSE,1,"",0,1,20,pvars);

	temp1=Rmalloc(1834+1);
	sprintf(temp1,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF(([WHICH]=1) AND ([ACCOUNT TYPE]=0))\n	THEN RETURN_VALUE=\n	(([VENPMT][VENVCH][ACCOUNT CODE]=[FINM",
		"GT][FINEACC][ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINEYR][ACCOUNT CODE])\n	",
		"AND ([FINMGT][FINEACC][ACTIVE]=TRUE) \n	AND ([FINMGT][FINEACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FI",
		"NEYR][ACTIVE]=TRUE) \n	AND ([FINMGT][FINEYR][DELETEFLAG]=FALSE)) \nELSE IF(([WHICH]=1) AND ([ACCOUNT",
		" TYPE]=1))\n	THEN RETURN_VALUE=\n	(([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINRACC][ACCOUNT CODE])",
		"\n	AND ([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINRYR][ACCOUNT CODE])\n	AND ([FINMGT][FINRACC][ACTI",
		"VE]=TRUE) \n	AND ([FINMGT][FINRACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FINRYR][ACTIVE]=TRUE) \n	AND ",
		"([FINMGT][FINRYR][DELETEFLAG]=FALSE))\nELSE IF(([WHICH]=1) AND ([ACCOUNT TYPE]=2))\n	THEN RETURN_VAL",
		"UE=\n	(([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINBACC][ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][ACCO",
		"UNT CODE]=[FINMGT][FINBYR][ACCOUNT CODE])\n	AND ([FINMGT][FINBACC][ACTIVE]=TRUE)\n	AND ([FINMGT][FIN",
		"BACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FINBYR][ACTIVE]=TRUE) \n	AND ([FINMGT][FINBYR][DELETEFLAG]=",
		"FALSE))\nELSE IF([WHICH]=2)\n	THEN RETURN_VALUE=\n	(([VENPMT][VENVCH][DEBIT CODE]=[FINMGT][FINBACC][",
		"ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][DEBIT CODE]=[FINMGT][FINBYR][ACCOUNT CODE])\n	AND ([FINMGT][F",
		"INBACC][ACTIVE]=TRUE) \n	AND ([FINMGT][FINBACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FINBYR][ACTIVE]=T",
		"RUE)\n	AND ([FINMGT][FINBYR][DELETEFLAG]=FALSE)) \nELSE IF([WHICH]=3)\n	THEN RETURN_VALUE=\n	(([VENP",
		"MT][VENVCH][PAYABLE CODE]=[FINMGT][FINBACC][ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][PAYABLE CODE]=[FI",
		"NMGT][FINBYR][ACCOUNT CODE])\n	AND ([FINMGT][FINBACC][ACTIVE]=TRUE) \n	AND ([FINMGT][FINBACC][DELETE",
		"FLAG]=FALSE)\n	AND ([FINMGT][FINBYR][ACTIVE]=TRUE)\n	AND ([FINMGT][FINBYR][DELETEFLAG]=FALSE)) \nELS",
		"E RETURN_VALUE=FALSE FI FI FI FI FI");
	tmp->select=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);


	temp1=Rmalloc(195+1);
	sprintf(temp1,"%s%s",
		"(([VENPMT][VENVCH][DELETEFLAG]=FALSE) AND \n([VENPMT][VENVCH][FISCAL YEAR]=[SELECTED FISCAL YEAR]) A",
		"ND \n([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VENVCH][SOURCE USER VERIFIED]=TRUE))");
	temp2=Rmalloc(29+1);
	sprintf(temp2,"%s",
		"([SELECTED FISCAL YEAR]<1990)");
	x=addsearchfileADV(tmp,"VENPMT","VENVCH",0,NULL,NULL,"VENDOR VOUCHER KEY",TRUE,temp1,temp2,3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","");
		addsearchconfld(s,"CALENDAR YEAR","");
		addsearchconfld(s,"INVOICE IDENTIFICATION","");
		addsearchconfld(s,"TRANSACTION NUMBER","");
	}
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"VENPMT","VENINV",2,"VENPMT","VENVCH","VENDOR INVOICE KEY",TRUE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[VENVCH][VENDOR IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[VENVCH][CALENDAR YEAR]");
		addsearchconfld(s,"INVOICE IDENTIFICATION","[VENVCH][INVOICE IDENTIFICATION]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"VENPMT","VENMSR",2,"VENPMT","VENVCH","VENDOR ID KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[VENVCH][VENDOR IDENTIFICATION]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINBYR",1,"VENPMT","VENVCH","FINBYR KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINEYR",1,"VENPMT","VENVCH","FINEYR MAIN KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINRYR",1,"VENPMT","VENVCH","FINRYR MAIN KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINYEAR",1,"VENPMT","VENVCH","FINYEAR KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINBACC",1,"VENPMT","VENVCH","ACCOUNT CODE KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINEACC",1,"VENPMT","VENVCH","ACCOUNT CODE KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINRACC",1,"VENPMT","VENVCH","ACCOUNT CODE KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINCOA",1,"VENPMT","VENVCH","FINCOA KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"ACCOUNT TYPE","[VENVCH][ACCOUNT TYPE]");
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"DIMENSION","[DIMENSION #0]");
		addsearchconfld(s,"NAME","[DIMENSION #0 CODE]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"SELECTED FISCAL YEAR",8,4,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(133+1);
	sprintf(temp1,"%s%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]+(INTEGER_TO_STRING([VENPMT][VENVCH][CALENDAR YEAR]))+[VENPMT",
		"][VENVCH][INVOICE IDENTIFICATION]");
	addvirtuallineADV(tmp,"VENDOR/CY/INVOICE IDENTIFICATION",1,45,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[WRITE COUNT]");
	addvirtuallineADV(tmp,"WHICH",7,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(184+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENINV][INVOICE DATE][1,1,2]!=\"\") \nTHEN RETURN_VALUE=(STRING_TO_VALUE([VENPMT][VENINV",
		"][INVOICE DATE][1,1,2])) \nELSE RETURN_VALUE=[VENPMT][VENVCH][PROCESSING MONTH] \nFI");
	addvirtuallineADV(tmp,"INVOICE MONTH",7,2,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][FISCAL PROCESSING MONTH]");
	addvirtuallineADV(tmp,"FISCAL PROCESSING MONTH",7,2,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(1030+1);
	sprintf(temp1,"%s%s%s%s%s%s%s%s%s%s%s",
		"IF([FISCAL PROCESSING MONTH]=0) THEN RETURN_VALUE=\"PRE-YEAR\" \nELSE IF([FISCAL PROCESSING MONTH]=1",
		") THEN RETURN_VALUE=\"JANUARY\" \nELSE IF([FISCAL PROCESSING MONTH]=2) THEN RETURN_VALUE=\"FEBRUARY",
		"\" \nELSE IF([FISCAL PROCESSING MONTH]=3) THEN RETURN_VALUE=\"MARCH\" \nELSE IF([FISCAL PROCESSING M",
		"ONTH]=4) THEN RETURN_VALUE=\"APRIL\" \nELSE IF([FISCAL PROCESSING MONTH]=5) THEN RETURN_VALUE=\"MAY",
		"\" \nELSE IF([FISCAL PROCESSING MONTH]=6) THEN RETURN_VALUE=\"JUNE\" \nELSE IF([FISCAL PROCESSING MO",
		"NTH]=7) THEN RETURN_VALUE=\"JULY\" \nELSE IF([FISCAL PROCESSING MONTH]=8) THEN RETURN_VALUE=\"AUGUST",
		"\" \nELSE IF([FISCAL PROCESSING MONTH]=9) THEN RETURN_VALUE=\"SEPTEMBER\" \nELSE IF([FISCAL PROCESSI",
		"NG MONTH]=10) THEN RETURN_VALUE=\"OCTOBER\" \nELSE IF([FISCAL PROCESSING MONTH]=11) THEN RETURN_VALU",
		"E=\"NOVEMBER\" \nELSE IF([FISCAL PROCESSING MONTH]=12) THEN RETURN_VALUE=\"DECEMBER\" \nELSE IF([FIS",
		"CAL PROCESSING MONTH]=13) THEN RETURN_VALUE=\"POST YEAR\" \nELSE RETURN_VALUE=\"Unknown!\" FI FI FI ",
		"FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"FISCAL PROCESSING MONTH STRING",1,12,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PROCESSING MONTH]");
	addvirtuallineADV(tmp,"CALENDAR PROCESSING MONTH",7,2,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(1+1);
	sprintf(temp1,"%s",
		"0");
	addvirtuallineADV(tmp,"DIMENSION #0",7,2,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	memset(stemp,0,101);
	sprintf(stemp,"DIMENSION #0 CODE");
	d=getDFvirtual("VENPMT",stemp);
	if(d!=NULL)
	{
		addvirtuallineADV(tmp,stemp,d->type,
			d->length,d->expression,
			d->when,1);
		FreeDFvirtual(d);
	} else {
		if(t!=NULL) Rfree(t);
		t=Rmalloc(RDAstrlen(rptname)+RDAstrlen(stemp)+60+1);
		sprintf(t,"VIRTUAL FIELD ERROR: Report [%s], Virtual Field Not Found [%s]",rptname,stemp);
		prterr(t);
		if(errorlist!=NULL)
		{
			addERRlist(errorlist,t);
		}
		if(t!=NULL) Rfree(t);
	}

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) \nTHEN RETURN_VALUE=([VENPMT][VENVCH][ACCOUNT TYPE]) \nELSE RETURN_VALUE=2 FI");
	addvirtuallineADV(tmp,"ACCOUNT TYPE",7,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(298+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) \nTHEN RETURN_VALUE=([VENPMT][VENVCH][ACCOUNT CODE]) \nELSE IF([WHICH]=2) \n     THEN ",
		"RETURN_VALUE=([VENPMT][VENVCH][DEBIT CODE]) \n     ELSE IF([WHICH]=3) \n          THEN RETURN_VALUE=",
		"([VENPMT][VENVCH][PAYABLE CODE]) \n          ELSE RETURN_VALUE=(\"\") \n          FI\n     FI \nFI");
	addvirtuallineADV(tmp,"ACCOUNT CODE",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(322+1);
	sprintf(temp1,"%s%s%s%s",
		"IF([ACCOUNT TYPE]=0) \nTHEN RETURN_VALUE=([FINMGT][FINEACC][DESCRIPTION]) \nELSE IF([ACCOUNT TYPE]=1",
		") \n     THEN RETURN_VALUE=([FINMGT][FINRACC][DESCRIPTION]) \n     ELSE IF([ACCOUNT TYPE]=2) \n     ",
		"     THEN RETURN_VALUE=([FINMGT][FINBACC][DESCRIPTION]) \n          ELSE RETURN_VALUE=(\"\") \n     ",
		"     FI \n     FI \nFI");
	addvirtuallineADV(tmp,"DESCRIPTION",0,0,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(128+1);
	sprintf(temp1,"%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=0)) \nTHEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]",
		") \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"EXPENDITURE AMOUNT",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(129+1);
	sprintf(temp1,"%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=1)) \nTHEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]",
		")  \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"REVENUE AMOUNT",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(432+1);
	sprintf(temp1,"%s%s%s%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=2) \n  AND ([VENPMT][VENVCH][AMOUNT]>0)) \nTHEN R",
		"ETURN_VALUE=([VENPMT][VENVCH][AMOUNT])  \nELSE IF(([WHICH]=2) AND ([VENPMT][VENVCH][AMOUNT]>0)) \n  ",
		"   THEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]) \n     ELSE IF(([WHICH]=3) AND ([VENPMT][VENVCH][AMO",
		"UNT]<0)) \n          THEN RETURN_VALUE=((-1)*([VENPMT][VENVCH][AMOUNT])) \n          ELSE RETURN_VAL",
		"UE=0 \n          FI\n     FI\nFI");
	addvirtuallineADV(tmp,"TOTAL DEBITS",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(439+1);
	sprintf(temp1,"%s%s%s%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=2) \n  AND ([VENPMT][VENVCH][AMOUNT]<0)) \nTHEN R",
		"ETURN_VALUE=((-1)*([VENPMT][VENVCH][AMOUNT])) \nELSE IF(([WHICH]=2) AND ([VENPMT][VENVCH][AMOUNT]<0)",
		") \n     THEN RETURN_VALUE=((-1)*([VENPMT][VENVCH][AMOUNT])) \n     ELSE IF(([WHICH]=3) AND ([VENPMT",
		"][VENVCH][AMOUNT]>0)) \n          THEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]) \n          ELSE RETU",
		"RN_VALUE=0 \n          FI \n     FI\nFI");
	addvirtuallineADV(tmp,"TOTAL CREDITS",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(249+1);
	sprintf(temp1,"%s%s%s",
		"IF(([WHICH]=2) AND ([VENPMT][VENVCH][AMOUNT]>0)) \nTHEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]) \nEL",
		"SE IF(([WHICH]=3) AND ([VENPMT][VENVCH][AMOUNT]<0)) \n     THEN RETURN_VALUE=((-1)*[VENPMT][VENVCH][",
		"AMOUNT]) \n     ELSE RETURN_VALUE=0\n     FI \nFI");
	addvirtuallineADV(tmp,"TOTAL DEBITS SUMMARY",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(250+1);
	sprintf(temp1,"%s%s%s",
		"IF(([WHICH]=2) AND ([VENPMT][VENVCH][AMOUNT]<0)) \nTHEN RETURN_VALUE=((-1)*[VENPMT][VENVCH][AMOUNT])",
		" \nELSE IF(([WHICH]=3) AND ([VENPMT][VENVCH][AMOUNT]>0)) \n     THEN RETURN_VALUE=([VENPMT][VENVCH][",
		"AMOUNT]) \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"TOTAL CREDITS SUMMARY",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(268+1);
	sprintf(temp1,"%s%s%s",
		"IF(([WHICH]=1) AND ([ACCOUNT TYPE]=0)) \nTHEN RETURN_VALUE=([FINMGT][FINEACC][DESCRIPTION]) \nELSE I",
		"F(([WHICH]=1) AND ([ACCOUNT TYPE]=1)) \n     THEN RETURN_VALUE=([FINMGT][FINRACC][DESCRIPTION]) \n  ",
		"   ELSE RETURN_VALUE=([FINMGT][FINBACC][DESCRIPTION]) \n     FI \nFI");
	addvirtuallineADV(tmp,"ACCOUNT DESCRIPTION",0,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(227+1);
	sprintf(temp1,"%s%s%s",
		"IF(([DEBIT ACCUMULATOR]>[CREDIT ACCUMULATOR]) \n  OR ([DEBIT ACCUMULATOR]=[CREDIT ACCUMULATOR])) \nT",
		"HEN RETURN_VALUE=([DEBIT ACCUMULATOR]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=([CREDIT ACCUMULATOR",
		"]-[DEBIT ACCUMULATOR]) \nFI");
	addvirtuallineADV(tmp,"TOTAL BALANCE SHEET",2,20,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(226+1);
	sprintf(temp1,"%s%s%s",
		"F(([DEBIT ACCUMULATOR]>[CREDIT ACCUMULATOR]) \n  OR ([DEBIT ACCUMULATOR]=[CREDIT ACCUMULATOR])) \nTH",
		"EN RETURN_VALUE=([DEBIT ACCUMULATOR]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=([CREDIT ACCUMULATOR]",
		"-[DEBIT ACCUMULATOR]) \nFI");
	addvirtuallineADV(tmp,"TOTAL BALANCE SHEET SUMMARY",2,20,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(63+1);
	sprintf(temp1,"%s",
		"IF ([WHICH]=1) \nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=1 \nFI");
	addvirtuallineADV(tmp,"TOTAL B/S LINES",10,1,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(128+1);
	sprintf(temp1,"%s%s",
		"IF(([VENPMT][VENVCH][1099 PAYMENT]=TRUE) AND ([WHICH]=1)) \nTHEN RETURN_VALUE=[VENPMT][VENVCH][AMOUN",
		"T]\nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"1099 AMOUNT",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(84+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) \nTHEN RETURN_VALUE=[VENPMT][VENVCH][AMOUNT]\nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"GROSS AMOUNT",2,20,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(42+1);
	sprintf(temp1,"%s",
		"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(61+1);
	sprintf(temp1,"%s",
		"\"Fiscal Year: \"+(INTEGER_TO_STRING([SELECTED FISCAL YEAR]))");
	addvirtuallineADV(tmp,"REPORT SUB-TITLE",1,80,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);

	addsortline(tmp,0,"[ACCOUNT TYPE]");
	addsortline(tmp,0,"[VENPMT][VENVCH][DEFINITION TYPE]");
	addsortline(tmp,0,"[DIMENSION #0 CODE]");
	addsortline(tmp,0,"[ACCOUNT CODE]");
	addsortline(tmp,0,"[WHICH]");
	addsortline(tmp,0,"[FISCAL PROCESSING MONTH]");
	addsortline(tmp,0,"[VENDOR/CY/INVOICE IDENTIFICATION]");
	addaccumline(tmp,"TOTAL EXPENDITURES",7,"[EXPENDITURE AMOUNT]",0);
	addaccumline(tmp,"TOTAL REVENUE",7,"[REVENUE AMOUNT]",0);
	addaccumline(tmp,"DEBIT ACCUMULATOR",7,"[TOTAL DEBITS]",0);
	addaccumline(tmp,"CREDIT ACCUMULATOR",7,"[TOTAL CREDITS]",0);
	addaccumline(tmp,"LAST ACCOUNT TYPE",3,"[ACCOUNT TYPE]",0);
	addaccumline(tmp,"LAST DEFINITION TYPE",3,"[VENPMT][VENVCH][DEFINITION TYPE]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE",3,"[ACCOUNT CODE]",0);
	addaccumline(tmp,"LAST FISCAL YEAR",3,"[VENPMT][VENVCH][FISCAL YEAR]",0);
	addaccumline(tmp,"LAST DIMENSION #0 CODE",3,"[DIMENSION #0 CODE]",0);
	addaccumline(tmp,"LAST CALENDAR PROCESSING MONTH",3,"[CALENDAR PROCESSING MONTH]",0);
	addaccumline(tmp,"LAST FISCAL PROCESSING MONTH",3,"[FISCAL PROCESSING MONTH]",0);
	addaccumline(tmp,"LAST FISCAL PROCESSING MONTH STRING",3,"[FISCAL PROCESSING MONTH STRING]",0);
	addaccumline(tmp,"LAST FINYEAR DESCRIPTION",3,"[FINMGT][FINYEAR][DESCRIPTION]",0);
	addaccumline(tmp,"FREQ TRANSACTION",1,"[VENPMT][VENVCH][TRANSACTION NUMBER]",0);
	addaccumline(tmp,"FREQ CALENDAR YEAR",1,"[VENPMT][VENVCH][CALENDAR YEAR]",0);
	addaccumline(tmp,"TOTAL DEBITS ACCUMULATOR",7,"[TOTAL DEBITS SUMMARY]",0);
	addaccumline(tmp,"TOTAL CREDITS ACCUMULATOR",7,"[TOTAL CREDITS SUMMARY]",0);
	addaccumline(tmp,"TOTAL B/S LINES SUMMARY",7,"[TOTAL B/S LINES]",0);
	addaccumline(tmp,"LAST INVOICE DATE",3,"[VENPMT][VENINV][INVOICE DATE]",0);
	addaccumline(tmp,"LAST INVOICE MONTH",3,"[INVOICE MONTH]",0);
	addaccumline(tmp,"LAST WHICH",3,"[WHICH]",0);
	addaccumline(tmp,"LAST VENVCH AMOUNT",3,"[VENPMT][VENVCH][AMOUNT]",0);
	addaccumline(tmp,"TOTAL VENVCH AMOUNT",7,"[VENPMT][VENVCH][AMOUNT]",0);
	addaccumline(tmp,"TOTAL GROSS AMOUNT",7,"[GROSS AMOUNT]",0);
	addaccumline(tmp,"TOTAL 1099 AMOUNT",7,"[1099 AMOUNT]",0);
	addcontrolline(tmp,"[ACCOUNT TYPE]",0);
	addcontrolline(tmp,"[VENPMT][VENVCH][DEFINITION TYPE]",0);
	addcontrolline(tmp,"[DIMENSION #0 CODE]",0);
	addcontrolline(tmp,"[ACCOUNT CODE]",0);
	addcontrolline(tmp,"[WHICH]",0);
	addcontrolline(tmp,"[FISCAL PROCESSING MONTH]",0);
	addcontrolline(tmp,"[VENDOR/CY/INVOICE IDENTIFICATION]",0);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[ORGANIZATION NAME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(9+1);
	sprintf(temp1,"%s",
		"[PAGE NO]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","                           @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                   Page: @RRRRRRR@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","                                              VENDOR DISTRIBUTION ACTIVITY REPORT                                       Date: @LLLLLLL@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[REPORT SUB-TITLE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","                           @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                 @@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[EXECUTED BY]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM TIME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","                           @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                   Time: @LLLLLLL@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[DIMENSION #0 CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[FINMGT][FINCOA][DESCRIPTION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"[DIMENSION #0 CODE] HEADER","Fund: @VV - @VV@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(14+1);
	sprintf(temp1,"%s",
		"[ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[DESCRIPTION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"([ACCOUNT TYPE]=0)");
	addimageline(tmp,"[ACCOUNT CODE] HEADER","  Account: @VVVV  @VV@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(14+1);
	sprintf(temp1,"%s",
		"[ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[DESCRIPTION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"([ACCOUNT TYPE]=1)");
	addimageline(tmp,"[ACCOUNT CODE] HEADER","  Account: @VVVV  @VV@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(14+1);
	sprintf(temp1,"%s",
		"[ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[DESCRIPTION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"([ACCOUNT TYPE]=2)");
	addimageline(tmp,"[ACCOUNT CODE] HEADER","  Account: @VVVV  @VV@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","    Vendor ID            Vendor Name               Invoice ID      Trn# Proc.Mo    Chk# Chk Date   PO ID       PO Line@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","    -------------------- ------------------------- --------------- ---- ------- ------- ---------- ----------- ----@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[ACCOUNT TYPE]=0");
	addimageline(tmp,"[ACCOUNT TYPE] HEADER","EXPENDITURE ACTIVITY ----------------------------------------------------------------------------------------------            Amount @@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[ACCOUNT TYPE]=1");
	addimageline(tmp,"[ACCOUNT TYPE] HEADER","REVENUE ACTIVITY --------------------------------------------------------------------------------------------------            Amount @@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[ACCOUNT TYPE]=2");
	addimageline(tmp,"[ACCOUNT TYPE] HEADER","BALANCE SHEET ACTIVITY ------------------------------------------------------------------------            Debits             Credits @@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENMSR][VENDOR NAME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(40+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(36+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][TRANSACTION NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[FISCAL PROCESSING MONTH STRING]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(28+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(35+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO LINE NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[EXPENDITURE AMOUNT]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([ACCOUNT TYPE]=0)");
	addimageline(tmp,"DETAIL LINES","    @LLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLL @RRR @LLLLLL @RRRRRR @LLLLLLLLL @LLLLLLLLLL @RRR @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENMSR][VENDOR NAME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(40+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(36+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][TRANSACTION NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[FISCAL PROCESSING MONTH STRING]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(28+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(35+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO LINE NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[REVENUE AMOUNT]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([ACCOUNT TYPE]=1)");
	addimageline(tmp,"DETAIL LINES","    @LLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLL @RRR @LLLLLL @RRRRRR @LLLLLLLLL @LLLLLLLLLL @RRR @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENMSR][VENDOR NAME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(40+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(36+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][TRANSACTION NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[FISCAL PROCESSING MONTH STRING]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(28+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(35+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO LINE NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[REVENUE AMOUNT]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([ACCOUNT TYPE]=1)");
	addimageline(tmp,"DETAIL LINES","    @LLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLL @RRR @LLLLLL @RRRRRR @LLLLLLLLL @LLLLLLLLLL @RRR @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENMSR][VENDOR NAME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(40+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(36+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][TRANSACTION NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[FISCAL PROCESSING MONTH STRING]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(28+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(35+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO LINE NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(55+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([ACCOUNT TYPE]=2) AND ([TOTAL DEBITS])");
	addimageline(tmp,"DETAIL LINES","    @LLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLL @RRR @LLLLLL @RRRRRR @LLLLLLLLL @LLLLLLLLLL @RRR@@",TRUE,temp1,11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(14+1);
	sprintf(temp1,"%s",
		"[TOTAL DEBITS]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(55+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([ACCOUNT TYPE]=2) AND ([TOTAL DEBITS])");
	addimageline(tmp,"DETAIL LINES","                                                                                                @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENMSR][VENDOR NAME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(40+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(36+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][TRANSACTION NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[FISCAL PROCESSING MONTH STRING]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(28+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CHECK DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(35+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PO LINE NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(56+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([ACCOUNT TYPE]=2) AND ([TOTAL CREDITS])");
	addimageline(tmp,"DETAIL LINES","    @LLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLL @RRR @LLLLLL @RRRRRR @LLLLLLLLL @LLLLLLLLLL @RRR@@",TRUE,temp1,11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[TOTAL CREDITS]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(56+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([ACCOUNT TYPE]=2) AND ([TOTAL CREDITS])");
	addimageline(tmp,"DETAIL LINES","                                                                                                                    @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(26+1);
	sprintf(temp1,"%s",
		"[TOTAL DEBITS ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(61+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2) AND (!([TOTAL DEBITS ACCUMULATOR]=0))");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","    Summary Entry1                                                                              @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(27+1);
	sprintf(temp1,"%s",
		"[TOTAL CREDITS ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(62+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2) AND (!([TOTAL CREDITS ACCUMULATOR]=0))");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","    Summary Entry2                                                                                                  @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(111+1);
	sprintf(temp1,"%s%s",
		"(([LAST ACCOUNT TYPE]=0) OR ([LAST ACCOUNT TYPE]=1)) AND \n(([FREQ CALENDAR YEAR]>1) OR ([FREQ TRANS",
		"ACTION]>1))");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","                                                                                                                    ------------------@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(211+1);
	sprintf(temp1,"%s%s%s",
		"(([LAST ACCOUNT TYPE]>0) AND ([FREQ TRANSACTION]>1)) AND \n(((([FREQ TRANSACTION]-[TOTAL B/S LINES S",
		"UMMARY])>0) AND \n[TOTAL B/S LINES SUMMARY]) OR \n(([FREQ TRANSACTION]>1) AND (!([TOTAL B/S LINES SU",
		"MMARY])))))");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","                                                                                                ------------------  ------------------@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(21+1);
	sprintf(temp1,"%s",
		"[TOTAL BALANCE SHEET]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(272+1);
	sprintf(temp1,"%s%s%s",
		"(([LAST ACCOUNT TYPE]=2) AND (([DEBIT ACCUMULATOR]>[CREDIT ACCUMULATOR]) OR ([DEBIT ACCUMULATOR]=[CR",
		"EDIT ACCUMULATOR])) AND (((([FREQ TRANSACTION]-[TOTAL B/S LINES SUMMARY])>0) AND [TOTAL B/S LINES SU",
		"MMARY]) OR (([FREQ TRANSACTION]>1) AND (!([TOTAL B/S LINES SUMMARY])))))");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","  Total Account @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL                                            @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(21+1);
	sprintf(temp1,"%s",
		"[TOTAL BALANCE SHEET]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(225+1);
	sprintf(temp1,"%s%s%s",
		"(([LAST ACCOUNT TYPE]=2) AND ([CREDIT ACCUMULATOR]>[DEBIT ACCUMULATOR])) AND (((([FREQ TRANSACTION]-",
		"[TOTAL B/S LINES SUMMARY])>0) AND [TOTAL B/S LINES SUMMARY]) OR (([FREQ TRANSACTION]>1) AND (!([TOTA",
		"L B/S LINES SUMMARY])))))");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","  Total Account @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL                                                                @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[TOTAL EXPENDITURES]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=0) AND ([FREQ TRANSACTION]>1)");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","  Total Account @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL                                                                @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[TOTAL REVENUE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=1) AND ([FREQ TRANSACTION]>1)");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","  Total Account @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL                                                                @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=0) OR ([LAST ACCOUNT TYPE]=1)");
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","                                                                                                                    ------------------@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=0) OR ([LAST ACCOUNT TYPE]=1)");
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","                                                                                                                    ------------------@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2)");
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","                                                                                                ------------------  ------------------@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2)");
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","                                                                                                ------------------  ------------------@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(24+1);
	sprintf(temp1,"%s",
		"[LAST DIMENSION #0 CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[TOTAL EXPENDITURES]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=0)");
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","Total Fund @LLLLLL                                                                                                  @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(24+1);
	sprintf(temp1,"%s",
		"[LAST DIMENSION #0 CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[TOTAL REVENUE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=1)");
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","Total Fund @LLLLLL                                                                                                  @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(24+1);
	sprintf(temp1,"%s",
		"[LAST DIMENSION #0 CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[DEBIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[CREDIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2)");
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","Total Fund @LLLLLL                                                                              @,.RRRRRRRRRRRRRRR  @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2)");
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[TOTAL EXPENDITURES]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=0)");
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","Total Expenditure Type:                                                                                             @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[TOTAL REVENUE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2)");
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","Total Revenue Type:                                                                                                 @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[DEBIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[CREDIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2)");
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","Total Balance Sheet Type:                                                                       @,.RRRRRRRRRRRRRRR  @,.RRRRRRRRRRRRRRR@@",TRUE,temp1,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	temp1=Rmalloc(118+1);
	sprintf(temp1,"%s%s",
		"([LAST ACCOUNT TYPE]=2) AND (([DEBIT ACCUMULATOR]<[CREDIT ACCUMULATOR]) OR ([DEBIT ACCUMULATOR]>[CRE",
		"DIT ACCUMULATOR]))");
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","***ERRORS FOUND!  FUND OUT OF BALANCE!!  FUND OUT OF BALANCE!!  FUND OUT OF BALANCE!!  FUND OUT OF BALANCE!!  FUND OUT OF BALANCE!!***@@",TRUE,temp1,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"FORMFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE FOOTER","@@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[TOTAL EXPENDITURES]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","TOTAL EXPENDITURES:      @,.RRRRRRRRRRRRRRRRRRRR@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[TOTAL REVENUE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","TOTAL REVENUE:           @,.RRRRRRRRRRRRRRRRRRRR@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[DEBIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","TOTAL DEBITS:            @,.RRRRRRRRRRRRRRRRRRRR@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[CREDIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","TOTAL CREDITS:           @,.RRRRRRRRRRRRRRRRRRRR@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"CARRIAGE RETURN");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"LINEFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(8+1);
	sprintf(temp1,"%s",
		"FORMFEED");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","@@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
/*
			ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Cannont Write Report definition for Report [%s].  Error in permissions, user cannot write the report definition.",rptname);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(7+6+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [VENPMT] Report [VCHDIST], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
void RPT_VCHDIST1a()
{
	int x;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	RDApval *pval=NULL,*pvals=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;
	char *temp1=NULL,*temp2=NULL;

	memset(stemp,0,101);
	rptname=Rmalloc(8+1);
	sprintf(rptname,"VCHDIST1");
	t=Rmalloc(27+1);
	sprintf(t,"VENDOR VOUCHER DISTRIBUTION");
	desc=Rmalloc(145+1);
	sprintf(desc,"This output definition is used to update the activity file and appropriate summary records necessary to complete the vendor voucher distribution.");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"");
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);

	pvars=NULL;
	adddisplayADV(tmp,(double)8.500000,(double)11.000000,4,FALSE,1,"",0,1,0,pvars);
	free_pvars(pvars,0);


	temp1=Rmalloc(1834+1);
	sprintf(temp1,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF(([WHICH]=1) AND ([ACCOUNT TYPE]=0))\n	THEN RETURN_VALUE=\n	(([VENPMT][VENVCH][ACCOUNT CODE]=[FINM",
		"GT][FINEACC][ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINEYR][ACCOUNT CODE])\n	",
		"AND ([FINMGT][FINEACC][ACTIVE]=TRUE) \n	AND ([FINMGT][FINEACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FI",
		"NEYR][ACTIVE]=TRUE) \n	AND ([FINMGT][FINEYR][DELETEFLAG]=FALSE)) \nELSE IF(([WHICH]=1) AND ([ACCOUNT",
		" TYPE]=1))\n	THEN RETURN_VALUE=\n	(([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINRACC][ACCOUNT CODE])",
		"\n	AND ([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINRYR][ACCOUNT CODE])\n	AND ([FINMGT][FINRACC][ACTI",
		"VE]=TRUE) \n	AND ([FINMGT][FINRACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FINRYR][ACTIVE]=TRUE) \n	AND ",
		"([FINMGT][FINRYR][DELETEFLAG]=FALSE))\nELSE IF(([WHICH]=1) AND ([ACCOUNT TYPE]=2))\n	THEN RETURN_VAL",
		"UE=\n	(([VENPMT][VENVCH][ACCOUNT CODE]=[FINMGT][FINBACC][ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][ACCO",
		"UNT CODE]=[FINMGT][FINBYR][ACCOUNT CODE])\n	AND ([FINMGT][FINBACC][ACTIVE]=TRUE)\n	AND ([FINMGT][FIN",
		"BACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FINBYR][ACTIVE]=TRUE) \n	AND ([FINMGT][FINBYR][DELETEFLAG]=",
		"FALSE))\nELSE IF([WHICH]=2)\n	THEN RETURN_VALUE=\n	(([VENPMT][VENVCH][DEBIT CODE]=[FINMGT][FINBACC][",
		"ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][DEBIT CODE]=[FINMGT][FINBYR][ACCOUNT CODE])\n	AND ([FINMGT][F",
		"INBACC][ACTIVE]=TRUE) \n	AND ([FINMGT][FINBACC][DELETEFLAG]=FALSE)\n	AND ([FINMGT][FINBYR][ACTIVE]=T",
		"RUE)\n	AND ([FINMGT][FINBYR][DELETEFLAG]=FALSE)) \nELSE IF([WHICH]=3)\n	THEN RETURN_VALUE=\n	(([VENP",
		"MT][VENVCH][PAYABLE CODE]=[FINMGT][FINBACC][ACCOUNT CODE])\n	AND ([VENPMT][VENVCH][PAYABLE CODE]=[FI",
		"NMGT][FINBYR][ACCOUNT CODE])\n	AND ([FINMGT][FINBACC][ACTIVE]=TRUE) \n	AND ([FINMGT][FINBACC][DELETE",
		"FLAG]=FALSE)\n	AND ([FINMGT][FINBYR][ACTIVE]=TRUE)\n	AND ([FINMGT][FINBYR][DELETEFLAG]=FALSE)) \nELS",
		"E RETURN_VALUE=FALSE FI FI FI FI FI");
	tmp->select=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);


	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"([VENPMT][VENVCH][DELETEFLAG]=FALSE) AND \n([VENPMT][VENVCH][DISTRIBUTED]=FALSE) AND \n([VENPMT][VEN",
		"VCH][SOURCE USER VERIFIED]=TRUE) AND \n([VENPMT][VENVCH][FISCAL YEAR]=[SELECTED FISCAL YEAR]) \n");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"VENPMT","VENVCH",0,NULL,NULL,"VENDOR VOUCHER KEY",TRUE,temp1,temp2,3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","");
		addsearchconfld(s,"CALENDAR YEAR","");
		addsearchconfld(s,"INVOICE IDENTIFICATION","");
		addsearchconfld(s,"TRANSACTION NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"VENPMT","VENINV",1,"VENPMT","VENVCH","VENDOR INVOICE KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[VENVCH][VENDOR IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[VENVCH][CALENDAR YEAR]");
		addsearchconfld(s,"INVOICE IDENTIFICATION","[VENVCH][INVOICE IDENTIFICATION]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"VENPMT","VENMSR",1,"VENPMT","VENVCH","VENDOR ID KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[VENVCH][VENDOR IDENTIFICATION]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINBYR",1,"VENPMT","VENVCH","FINBYR KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINEYR",1,"VENPMT","VENVCH","FINEYR MAIN KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINRYR",1,"VENPMT","VENVCH","FINRYR MAIN KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINYEAR",1,"VENPMT","VENVCH","FINYEAR KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"FISCAL YEAR","[VENVCH][FISCAL YEAR]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINBACC",1,"VENPMT","VENVCH","ACCOUNT CODE KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINEACC",1,"VENPMT","VENVCH","ACCOUNT CODE KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINRACC",1,"VENPMT","VENVCH","ACCOUNT CODE KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	x=addsearchfileADV(tmp,"FINMGT","FINCOA",1,"VENPMT","VENVCH","FINCOA KEY",FALSE,temp1,temp2,1);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"ACCOUNT TYPE","[VENVCH][ACCOUNT TYPE]");
		addsearchconfld(s,"DEFINITION TYPE","[VENVCH][DEFINITION TYPE]");
		addsearchconfld(s,"DIMENSION","[DIMENSION #0]");
		addsearchconfld(s,"NAME","[DIMENSION #0 CODE]");
	}


	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"SELECTED FISCAL YEAR",8,4,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(133+1);
	sprintf(temp1,"%s%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]+(INTEGER_TO_STRING([VENPMT][VENVCH][CALENDAR YEAR]))+[VENPMT",
		"][VENVCH][INVOICE IDENTIFICATION]");
	addvirtuallineADV(tmp,"VENDOR/CY/INVOICE IDENTIFICATION",1,40,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(10+1);
	sprintf(temp1,"%s",
		"\"VENPMT\"");
	addvirtuallineADV(tmp,"SOURCE MODULE",0,0,temp1,1,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"\"VCHDIST\"");
	addvirtuallineADV(tmp,"SOURCE PROCESS",0,0,temp1,1,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[WRITE COUNT]");
	addvirtuallineADV(tmp,"WHICH",7,1,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(184+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENINV][INVOICE DATE][1,1,2]!=\"\") \nTHEN RETURN_VALUE=(STRING_TO_VALUE([VENPMT][VENINV",
		"][INVOICE DATE][1,1,2])) \nELSE RETURN_VALUE=[VENPMT][VENVCH][PROCESSING MONTH] \nFI");
	addvirtuallineADV(tmp,"INVOICE MONTH",7,2,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][FISCAL PROCESSING MONTH]");
	addvirtuallineADV(tmp,"FISCAL PROCESSING MONTH",7,2,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(1030+1);
	sprintf(temp1,"%s%s%s%s%s%s%s%s%s%s%s",
		"IF([FISCAL PROCESSING MONTH]=0) THEN RETURN_VALUE=\"PRE-YEAR\" \nELSE IF([FISCAL PROCESSING MONTH]=1",
		") THEN RETURN_VALUE=\"JANUARY\" \nELSE IF([FISCAL PROCESSING MONTH]=2) THEN RETURN_VALUE=\"FEBRUARY",
		"\" \nELSE IF([FISCAL PROCESSING MONTH]=3) THEN RETURN_VALUE=\"MARCH\" \nELSE IF([FISCAL PROCESSING M",
		"ONTH]=4) THEN RETURN_VALUE=\"APRIL\" \nELSE IF([FISCAL PROCESSING MONTH]=5) THEN RETURN_VALUE=\"MAY",
		"\" \nELSE IF([FISCAL PROCESSING MONTH]=6) THEN RETURN_VALUE=\"JUNE\" \nELSE IF([FISCAL PROCESSING MO",
		"NTH]=7) THEN RETURN_VALUE=\"JULY\" \nELSE IF([FISCAL PROCESSING MONTH]=8) THEN RETURN_VALUE=\"AUGUST",
		"\" \nELSE IF([FISCAL PROCESSING MONTH]=9) THEN RETURN_VALUE=\"SEPTEMBER\" \nELSE IF([FISCAL PROCESSI",
		"NG MONTH]=10) THEN RETURN_VALUE=\"OCTOBER\" \nELSE IF([FISCAL PROCESSING MONTH]=11) THEN RETURN_VALU",
		"E=\"NOVEMBER\" \nELSE IF([FISCAL PROCESSING MONTH]=12) THEN RETURN_VALUE=\"DECEMBER\" \nELSE IF([FIS",
		"CAL PROCESSING MONTH]=13) THEN RETURN_VALUE=\"POST YEAR\" \nELSE RETURN_VALUE=\"Unknown!\" FI FI FI ",
		"FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"FISCAL PROCESSING MONTH STRING",1,12,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][PROCESSING MONTH]");
	addvirtuallineADV(tmp,"CALENDAR PROCESSING MONTH",7,2,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(1+1);
	sprintf(temp1,"%s",
		"0");
	addvirtuallineADV(tmp,"DIMENSION #0",7,2,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(517+1);
	sprintf(temp1,"%s%s%s%s%s%s",
		"IF([ACCOUNT TYPE]=0) THEN\n	IF([FINMGT][FINEACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][F",
		"INEACC][ACCOUNT CODE][1,1,3]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=1) THEN\n	IF([FINMG",
		"T][FINRACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,1,3]\n	ELSE R",
		"ETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=2) THEN\n	IF([FINMGT][FINBACC][DEFINITION TYPE]=0) THEN",
		"\n		RETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,1,3]\n	ELSE RETURN_VALUE=\"\" FI\nELSE RETURN_VAL",
		"UE=\"\" FI FI FI\n");
	addvirtuallineADV(tmp,"DIMENSION #0 CODE",1,3,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(87+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=([VENPMT][VENVCH][ACCOUNT TYPE]) ELSE RETURN_VALUE=2 FI");
	addvirtuallineADV(tmp,"ACCOUNT TYPE",7,1,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(236+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=([VENPMT][VENVCH][ACCOUNT CODE]) ELSE IF([WHICH]=2) THEN RETURN_VALU",
		"E=([VENPMT][VENVCH][DEBIT CODE]) ELSE IF([WHICH]=3) THEN RETURN_VALUE=([VENPMT][VENVCH][PAYABLE CODE",
		"]) ELSE RETURN_VALUE=(\"\") FI FI FI");
	addvirtuallineADV(tmp,"ACCOUNT CODE",1,40,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(128+1);
	sprintf(temp1,"%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=0)) \nTHEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]",
		") \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"EXPENDITURE AMOUNT",2,20,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(128+1);
	sprintf(temp1,"%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=1)) \nTHEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]",
		") \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"REVENUE AMOUNT",2,20,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(434+1);
	sprintf(temp1,"%s%s%s%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=2) \n  AND ([VENPMT][VENVCH][AMOUNT]>0)) \nTHEN R",
		"ETURN_VALUE=([VENPMT][VENVCH][AMOUNT]) \nELSE IF(([WHICH]=2) AND ([VENPMT][VENVCH][AMOUNT]>0)) \n   ",
		"  THEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]) \n     ELSE IF(([WHICH]=3) AND ([VENPMT][VENVCH][AMOU",
		"NT]<0)) \n          THEN RETURN_VALUE=((-1)*([VENPMT][VENVCH][AMOUNT])) \n          ELSE RETURN_VALU",
		"E=0 \n          FI \n     FI  \nFI");
	addvirtuallineADV(tmp,"TOTAL DEBITS",2,20,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(438+1);
	sprintf(temp1,"%s%s%s%s%s",
		"IF(([WHICH]=1) AND ([VENPMT][VENVCH][ACCOUNT TYPE]=2) \n  AND ([VENPMT][VENVCH][AMOUNT]<0)) \nTHEN R",
		"ETURN_VALUE=((-1)*([VENPMT][VENVCH][AMOUNT])) \nELSE IF(([WHICH]=2) AND ([VENPMT][VENVCH][AMOUNT]<0)",
		") \n     THEN RETURN_VALUE=((-1)*([VENPMT][VENVCH][AMOUNT])) \n     ELSE IF(([WHICH]=3) AND ([VENPMT",
		"][VENVCH][AMOUNT]>0)) \n          THEN RETURN_VALUE=([VENPMT][VENVCH][AMOUNT]) \n          ELSE RETU",
		"RN_VALUE=0 \n          FI\n     FI\nFI");
	addvirtuallineADV(tmp,"TOTAL CREDITS",2,20,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(269+1);
	sprintf(temp1,"%s%s%s",
		"IF(([WHICH]=1) AND ([ACCOUNT CODE]=0)) \nTHEN RETURN_VALUE=([FINMGT][FINEACC][DESCRIPTION]) \nELSE I",
		"F(([WHICH]=1) AND ([ACCOUNT CODE]=1)) \n     THEN RETURN_VALUE=([FINMGT][FINRACC][DESCRIPTION]) \n  ",
		"   ELSE RETURN_VALUE=([FINMGT][FINBACC][DESCRIPTION]) \n     FI  \nFI");
	addvirtuallineADV(tmp,"DESCRIPTION",0,0,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(4+1);
	sprintf(temp1,"%s",
		"TRUE");
	addvirtuallineADV(tmp,"SETFLAG",10,1,temp1,1,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(128+1);
	sprintf(temp1,"%s%s",
		"IF(([VENPMT][VENVCH][1099 PAYMENT]=TRUE) AND ([WHICH]=1)) \nTHEN RETURN_VALUE=[VENPMT][VENVCH][AMOUN",
		"T]\nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"1099 AMOUNT",2,20,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);
	temp1=Rmalloc(83+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1)\nTHEN RETURN_VALUE=[VENPMT][VENVCH][AMOUNT]\nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"GROSS AMOUNT",2,20,temp1,0,0);
if(temp1!=NULL) Rfree(temp1);

	if(t!=NULL) Rfree(t);

	addsortline(tmp,0,"[ACCOUNT TYPE]");
	addsortline(tmp,0,"[VENPMT][VENVCH][DEFINITION TYPE]");
	addsortline(tmp,0,"[DIMENSION #0 CODE]");
	addsortline(tmp,0,"[ACCOUNT CODE]");
	addsortline(tmp,0,"[WHICH]");
	addsortline(tmp,0,"[FISCAL PROCESSING MONTH]");
	addsortline(tmp,0,"[VENDOR/CY/INVOICE IDENTIFICATION]");
	addaccumline(tmp,"TOTAL EXPENDITURES",7,"[EXPENDITURE AMOUNT]",0);
	addaccumline(tmp,"TOTAL REVENUE",7,"[REVENUE AMOUNT]",0);
	addaccumline(tmp,"DEBIT ACCUMULATOR",7,"[TOTAL DEBITS]",0);
	addaccumline(tmp,"CREDIT ACCUMULATOR",7,"[TOTAL CREDITS]",0);
	addaccumline(tmp,"LAST ACCOUNT TYPE",3,"[ACCOUNT TYPE]",0);
	addaccumline(tmp,"LAST DEFINITION TYPE",3,"[VENPMT][VENVCH][DEFINITION TYPE]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE",3,"[ACCOUNT CODE]",0);
	addaccumline(tmp,"LAST FISCAL YEAR",3,"[VENPMT][VENVCH][FISCAL YEAR]",0);
	addaccumline(tmp,"LAST DIMENSION #0 CODE",3,"[DIMENSION #0 CODE]",0);
	addaccumline(tmp,"LAST CALENDAR PROCESSING MONTH",3,"[CALENDAR PROCESSING MONTH]",0);
	addaccumline(tmp,"LAST FISCAL PROCESSING MONTH",3,"[FISCAL PROCESSING MONTH]",0);
	addaccumline(tmp,"LAST VENDOR IDENTIFICATION",3,"[VENPMT][VENVCH][VENDOR IDENTIFICATION]",0);
	addaccumline(tmp,"LAST CALENDAR YEAR",3,"[VENPMT][VENVCH][CALENDAR YEAR]",0);
	addaccumline(tmp,"LAST INVOICE MONTH",3,"[INVOICE MONTH]",0);
	addaccumline(tmp,"LAST WHICH",3,"[WHICH]",0);
	addaccumline(tmp,"TOTAL VENVCH AMOUNT",7,"[VENPMT][VENVCH][AMOUNT]",0);
	addaccumline(tmp,"TOTAL GROSS AMOUNT",7,"[GROSS AMOUNT]",0);
	addaccumline(tmp,"TOTAL 1099 AMOUNT",7,"[1099 AMOUNT]",0);
	addcontrolline(tmp,"[ACCOUNT TYPE]",0);
	addcontrolline(tmp,"[VENPMT][VENVCH][DEFINITION TYPE]",0);
	addcontrolline(tmp,"[DIMENSION #0 CODE]",0);
	addcontrolline(tmp,"[ACCOUNT CODE]",0);
	addcontrolline(tmp,"[WHICH]",0);
	addcontrolline(tmp,"[FISCAL PROCESSING MONTH]",0);
	addcontrolline(tmp,"[VENDOR/CY/INVOICE IDENTIFICATION]",0);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINBYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINRYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINEYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINBACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINRACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINEACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [VENPMT][VENYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [VENPMT][VENFYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [VENPMT][VENVCH]",FALSE,NULL,0,pval);


	pval=Rmalloc(8*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(31+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CALENDAR YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][CALENDAR YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(40+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][INVOICE IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(36+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][TRANSACTION NUMBER]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(9+1);
	sprintf(temp1,"%s",
		"[SETFLAG]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][DISTRIBUTED]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LOGIN IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][DISTRIBUTED BY]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][DISTRIBUTED DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM TIME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENVCH][DISTRIBUTED TIME]");
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"([WHICH]=1)");
	addimageline(tmp,"DETAIL LINES","WRITE [VENPMT][VENVCH]",TRUE,temp1,8,pval);
	free_pvals(pval,8);


	pval=Rmalloc(26*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][VENDOR IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(31+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][CALENDAR YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][CALENDAR YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(173+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([VENPMT][VENYR][JANUARY 1099 AMOUNT]+[",
		"1099 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][JANUARY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][JANUARY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(176+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([VENPMT][VENYR][JANUARY GROSS AMOUNT]+",
		"[GROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][JANUARY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][JANUARY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(175+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([VENPMT][VENYR][FEBRUARY 1099 AMOUNT]+",
		"[1099 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][FEBRUARY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][FEBRUARY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(178+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([VENPMT][VENYR][FEBRUARY GROSS AMOUNT]",
		"+[GROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][FEBRUARY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][FEBRUARY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(169+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([VENPMT][VENYR][MARCH 1099 AMOUNT]+[10",
		"99 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][MARCH 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][MARCH 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(172+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([VENPMT][VENYR][MARCH GROSS AMOUNT]+[G",
		"ROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][MARCH GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][MARCH GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(169+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([VENPMT][VENYR][APRIL 1099 AMOUNT]+[10",
		"99 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][APRIL 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][APRIL 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(172+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([VENPMT][VENYR][APRIL GROSS AMOUNT]+[G",
		"ROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][APRIL GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][APRIL GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(165+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([VENPMT][VENYR][MAY 1099 AMOUNT]+[1099",
		" AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][MAY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][MAY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(168+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([VENPMT][VENYR][MAY GROSS AMOUNT]+[GRO",
		"SS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][MAY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][MAY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(167+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([VENPMT][VENYR][JUNE 1099 AMOUNT]+[109",
		"9 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][JUNE 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][JUNE 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(170+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([VENPMT][VENYR][JUNE GROSS AMOUNT]+[GR",
		"OSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][JUNE GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][JUNE GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(167+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([VENPMT][VENYR][JULY 1099 AMOUNT]+[109",
		"9 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][JULY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][JULY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(170+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([VENPMT][VENYR][JULY GROSS AMOUNT]+[GR",
		"OSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][JULY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][JULY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(171+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([VENPMT][VENYR][AUGUST 1099 AMOUNT]+[1",
		"099 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][AUGUST 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][AUGUST 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(174+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([VENPMT][VENYR][AUGUST GROSS AMOUNT]+[",
		"GROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][AUGUST GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][AUGUST GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(177+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]",
		"+[1099 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(180+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([VENPMT][VENYR][SEPTEMBER GROSS AMOUNT",
		"]+[GROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(173+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([VENPMT][VENYR][OCTOBER 1099 AMOUNT]+[",
		"1099 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][OCTOBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][OCTOBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(176+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([VENPMT][VENYR][OCTOBER GROSS AMOUNT]+",
		"[GROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][OCTOBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][OCTOBER GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(176+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([VENPMT][VENYR][NOVEMBER 1099 AMOUNT]",
		"+[1099 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][NOVEMBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][NOVEMBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(179+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([VENPMT][VENYR][NOVEMBER GROSS AMOUNT",
		"]+[GROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][NOVEMBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][NOVEMBER GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(176+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([VENPMT][VENYR][DECEMBER 1099 AMOUNT]",
		"+[1099 AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][DECEMBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][DECEMBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(179+1);
	sprintf(temp1,"%s%s",
		"IF([VENPMT][VENVCH][PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([VENPMT][VENYR][DECEMBER GROSS AMOUNT",
		"]+[GROSS AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENYR][DECEMBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][DECEMBER GROSS AMOUNT]");
	addimageline(tmp,"DETAIL LINES","WRITE [VENPMT][VENYR]",FALSE,NULL,26,pval);
	free_pvals(pval,26);


	pval=Rmalloc(30*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][VENDOR IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENYR][VENDOR IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[VENPMT][VENVCH][FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][APRIL 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(168+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][PRE-YEAR 1099 AMOUNT]+[1099 AM",
		"OUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][PRE-YEAR 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][PRE-YEAR 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(171+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][PRE-YEAR GROSS AMOUNT]+[GROSS ",
		"AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][PRE-YEAR GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][PRE-YEAR GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(166+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][JANUARY 1099 AMOUNT]+[1099 AMO",
		"UNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][JANUARY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][JANUARY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(169+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][JANUARY GROSS AMOUNT]+[GROSS A",
		"MOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][JANUARY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][JANUARY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(168+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][FEBRUARY 1099 AMOUNT]+[1099 AM",
		"OUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][FEBRUARY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][FEBRUARY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(171+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][FEBRUARY GROSS AMOUNT]+[GROSS ",
		"AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][FEBRUARY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][FEBRUARY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(162+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][MARCH 1099 AMOUNT]+[1099 AMOUN",
		"T]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][MARCH 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][MARCH 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(165+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][MARCH GROSS AMOUNT]+[GROSS AMO",
		"UNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][MARCH GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][MARCH GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(162+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][APRIL 1099 AMOUNT]+[1099 AMOUN",
		"T]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][APRIL 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][APRIL 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(165+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][APRIL GROSS AMOUNT]+[GROSS AMO",
		"UNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][APRIL GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][APRIL GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(158+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][MAY 1099 AMOUNT]+[1099 AMOUNT]",
		") \nELSE RETURN_VALUE=[VENPMT][VENFYR][MAY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][MAY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(161+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][MAY GROSS AMOUNT]+[GROSS AMOUN",
		"T]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][MAY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][MAY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(160+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][JUNE 1099 AMOUNT]+[1099 AMOUNT",
		"]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][JUNE 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][JUNE 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(163+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][JUNE GROSS AMOUNT]+[GROSS AMOU",
		"NT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][JUNE GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][JUNE GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(160+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][JULY 1099 AMOUNT]+[1099 AMOUNT",
		"]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][JULY 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][JULY 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(163+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][JULY GROSS AMOUNT]+[GROSS AMOU",
		"NT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][JULY GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][JULY GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(164+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][AUGUST 1099 AMOUNT]+[1099 AMOU",
		"NT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][AUGUST 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][AUGUST 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(167+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][AUGUST GROSS AMOUNT]+[GROSS AM",
		"OUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][AUGUST GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][AUGUST GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(170+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][SEPTEMBER 1099 AMOUNT]+[1099 A",
		"MOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][SEPTEMBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][SEPTEMBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(173+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][SEPTEMBER GROSS AMOUNT]+[GROSS",
		" AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][SEPTEMBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][SEPTEMBER GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(167+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][OCTOBER 1099 AMOUNT]+[1099 AM",
		"OUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][OCTOBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][OCTOBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(170+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][OCTOBER GROSS AMOUNT]+[GROSS ",
		"AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][OCTOBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][OCTOBER GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(169+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][NOVEMBER 1099 AMOUNT]+[1099 A",
		"MOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][NOVEMBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][NOVEMBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(172+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][NOVEMBER GROSS AMOUNT]+[GROSS",
		" AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][NOVEMBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][NOVEMBER GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(169+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][DECEMBER 1099 AMOUNT]+[1099 A",
		"MOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][DECEMBER 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][DECEMBER 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(172+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][DECEMBER GROSS AMOUNT]+[GROSS",
		" AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][DECEMBER GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][DECEMBER GROSS AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(171+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][POST YEAR 1099 AMOUNT]+[1099 ",
		"AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][POST YEAR 1099 AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][POST YEAR 1099 AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(174+1);
	sprintf(temp1,"%s%s",
		"IF([FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([VENPMT][VENFYR][POST YEAR GROSS AMOUNT]+[GROS",
		"S AMOUNT]) \nELSE RETURN_VALUE=[VENPMT][VENFYR][POST YEAR GROSS AMOUNT] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[VENPMT][VENFYR][POST YEAR GROSS AMOUNT]");
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"([USE VENPMT FISCAL SUMMARY]=TRUE)");
	addimageline(tmp,"DETAIL LINES","WRITE [VENPMT][VENFYR]",TRUE,temp1,30,pval);
	free_pvals(pval,30);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"NEXT TRANSACTION NO");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM TIME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL PROCESSING MONTH]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[SOURCE MODULE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[SOURCE PROCESS]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LOGIN IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(1+1);
	sprintf(temp1,"%s",
		"0");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[DEBIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][AMOUNT]");
	temp1=Rmalloc(49+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2) AND ([DEBIT ACCUMULATOR])");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBACT]",TRUE,temp1,12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"NEXT TRANSACTION NO");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM TIME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL PROCESSING MONTH]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(1+1);
	sprintf(temp1,"%s",
		"1");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[SOURCE MODULE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[SOURCE PROCESS]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LOGIN IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[CREDIT ACCUMULATOR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBACT][AMOUNT]");
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2) AND ([CREDIT ACCUMULATOR])");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBACT]",TRUE,temp1,12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"NEXT TRANSACTION NO");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM TIME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL PROCESSING MONTH]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(1+1);
	sprintf(temp1,"%s",
		"0");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[SOURCE MODULE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[SOURCE PROCESS]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LOGIN IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[TOTAL EXPENDITURES]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEACT][AMOUNT]");
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=0) AND ([TOTAL EXPENDITURES])");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINEACT]",TRUE,temp1,12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"NEXT TRANSACTION NO");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM DATE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(13+1);
	sprintf(temp1,"%s",
		"[SYSTEM TIME]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL PROCESSING MONTH]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(1+1);
	sprintf(temp1,"%s",
		"0");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[SOURCE MODULE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(16+1);
	sprintf(temp1,"%s",
		"[SOURCE PROCESS]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LOGIN IDENTIFICATION]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(15+1);
	sprintf(temp1,"%s",
		"[TOTAL REVENUE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRACT][AMOUNT]");
	temp1=Rmalloc(45+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=1) AND ([TOTAL REVENUE])");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINRACT]",TRUE,temp1,12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(31*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(210+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EX",
		"PENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPEND",
		"ITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(206+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPE",
		"NDITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITUR",
		"ES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(208+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXP",
		"ENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDIT",
		"URES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(204+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][JANUARY DISTRIBUTED EXPEN",
		"DITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES",
		"] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(210+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EX",
		"PENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPEND",
		"ITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(206+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPE",
		"NDITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITUR",
		"ES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(204+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPEN",
		"DITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES",
		"] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(200+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDI",
		"TURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(204+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPEN",
		"DITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES",
		"] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(200+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDI",
		"TURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(200+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDI",
		"TURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITU",
		"RES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(202+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPEND",
		"ITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES] ",
		"FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(198+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDIT",
		"URES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(202+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][JULY UNDISTRIBUTED EXPEND",
		"ITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES] ",
		"FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(198+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][JULY DISTRIBUTED EXPENDIT",
		"URES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(206+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPE",
		"NDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITUR",
		"ES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(202+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][AUGUST DISTRIBUTED EXPEND",
		"ITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES] ",
		"FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(212+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED E",
		"XPENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPE",
		"NDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(208+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXP",
		"ENDITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDIT",
		"URES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(209+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EX",
		"PENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDI",
		"TURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(205+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPE",
		"NDITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURE",
		"S] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(211+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED E",
		"XPENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPEN",
		"DITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(207+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXP",
		"ENDITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITU",
		"RES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(211+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][DECEMBER UNDISTRIBUTED E",
		"XPENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPEN",
		"DITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(207+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][DECEMBER DISTRIBUTED EXP",
		"ENDITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITU",
		"RES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(213+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][POST YEAR UNDISTRIBUTED ",
		"EXPENDITURES]-[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED EXP",
		"ENDITURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(209+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINEYR][POST YEAR DISTRIBUTED EX",
		"PENDITURES]+[TOTAL EXPENDITURES]) \nELSE RETURN_VALUE=[FINMGT][FINEYR][POST YEAR DISTRIBUTED EXPENDI",
		"TURES] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINEYR][POST YEAR DISTRIBUTED EXPENDITURES]");
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=0)");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINEYR]",TRUE,temp1,31,pval);
	free_pvals(pval,31);


	pval=Rmalloc(31*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(195+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED RE",
		"VENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(191+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][PRE-YEAR DISTRIBUTED REVE",
		"NUE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][PRE-YEAR DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][PRE-YEAR DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(193+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][JANUARY UNDISTRIBUTED REV",
		"ENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][JANUARY UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][JANUARY UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(189+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][JANUARY DISTRIBUTED REVEN",
		"UE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][JANUARY DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][JANUARY DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(195+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][FEBRUARY UNDISTRIBUTED RE",
		"VENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][FEBRUARY UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][FEBRUARY UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(191+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][FEBRUARY DISTRIBUTED REVE",
		"NUE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][FEBRUARY DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][FEBRUARY DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(189+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][MARCH UNDISTRIBUTED REVEN",
		"UE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][MARCH UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][MARCH UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(185+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][MARCH DISTRIBUTED REVENUE",
		"]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][MARCH DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][MARCH DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(189+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][APRIL UNDISTRIBUTED REVEN",
		"UE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][APRIL UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][APRIL UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(185+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][APRIL DISTRIBUTED REVENUE",
		"]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][APRIL DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][APRIL DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(185+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][MAY UNDISTRIBUTED REVENUE",
		"]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][MAY UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][MAY UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(181+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][MAY DISTRIBUTED REVENUE]+",
		"[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][MAY DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][MAY DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(187+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][JUNE UNDISTRIBUTED REVENU",
		"E]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][JUNE UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][JUNE UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(183+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][JUNE DISTRIBUTED REVENUE]",
		"+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][JUNE DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][JUNE DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(187+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][JULY UNDISTRIBUTED REVENU",
		"E]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][JULY UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][JULY UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(183+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][JULY DISTRIBUTED REVENUE]",
		"+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][JULY DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][JULY DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(191+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][AUGUST UNDISTRIBUTED REVE",
		"NUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][AUGUST UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][AUGUST UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(187+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][AUGUST DISTRIBUTED REVENU",
		"E]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][AUGUST DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][AUGUST DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(197+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][SEPTEMBER UNDISTRIBUTED R",
		"EVENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][SEPTEMBER UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][SEPTEMBER UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(193+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][SEPTEMBER DISTRIBUTED REV",
		"ENUE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][SEPTEMBER DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][SEPTEMBER DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(194+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][OCTOBER UNDISTRIBUTED RE",
		"VENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][OCTOBER UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][OCTOBER UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(190+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][OCTOBER DISTRIBUTED REVE",
		"NUE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][OCTOBER DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][OCTOBER DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][NOVEMBER UNDISTRIBUTED R",
		"EVENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][NOVEMBER UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][NOVEMBER UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(192+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][NOVEMBER DISTRIBUTED REV",
		"ENUE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][NOVEMBER DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][NOVEMBER DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][DECEMBER UNDISTRIBUTED R",
		"EVENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][DECEMBER UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][DECEMBER UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(192+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][DECEMBER DISTRIBUTED REV",
		"ENUE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][DECEMBER DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][DECEMBER DISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(198+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][POST YEAR UNDISTRIBUTED ",
		"REVENUE]-[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][POST YEAR UNDISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][POST YEAR UNDISTRIBUTED REVENUE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(194+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINRYR][POST YEAR DISTRIBUTED RE",
		"VENUE]+[TOTAL REVENUE]) \nELSE RETURN_VALUE=[FINMGT][FINRYR][POST YEAR DISTRIBUTED REVENUE] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINRYR][POST YEAR DISTRIBUTED REVENUE]");
	temp1=Rmalloc(23+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=1)");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINRYR]",TRUE,temp1,31,pval);
	free_pvals(pval,31);


	pval=Rmalloc(31*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(197+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DE",
		"BITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(193+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBI",
		"TS]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(195+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEB",
		"ITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(191+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JANUARY DISTRIBUTED DEBIT",
		"S]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(197+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DE",
		"BITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(193+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBI",
		"TS]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(191+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBIT",
		"S]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(187+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]",
		"+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(191+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBIT",
		"S]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(187+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]",
		"+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(187+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]",
		"-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(183+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]+[",
		"DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(189+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS",
		"]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(185+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]+",
		"[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(189+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS",
		"]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(185+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]+",
		"[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(193+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBI",
		"TS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(189+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS",
		"]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(199+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED D",
		"EBITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(195+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEB",
		"ITS]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DE",
		"BITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(192+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBI",
		"TS]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(198+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED D",
		"EBITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(194+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEB",
		"ITS]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(198+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][DECEMBER UNDISTRIBUTED D",
		"EBITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(194+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][DECEMBER DISTRIBUTED DEB",
		"ITS]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(200+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][POST YEAR UNDISTRIBUTED ",
		"DEBITS]-[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][POST YEAR DISTRIBUTED DE",
		"BITS]+[DEBIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR DISTRIBUTED DEBITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][POST YEAR DISTRIBUTED DEBITS]");
	temp1=Rmalloc(49+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2) AND ([DEBIT ACCUMULATOR])");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBYR]",TRUE,temp1,31,pval);
	free_pvals(pval,31);


	pval=Rmalloc(31*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LAST DEFINITION TYPE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(19+1);
	sprintf(temp1,"%s",
		"[LAST ACCOUNT CODE]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[LAST FISCAL YEAR]");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(200+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CR",
		"EDITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=0) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CRED",
		"ITS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(198+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JANUARY UNDISTRIBUTED CRE",
		"DITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(194+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=1) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JANUARY DISTRIBUTED CREDI",
		"TS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(200+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CR",
		"EDITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=2) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][FEBRUARY DISTRIBUTED CRED",
		"ITS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(193+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDI",
		"TS]-[CREDIT ACCUMULATOR])\nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(190+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=3) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS",
		"]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(194+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDI",
		"TS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(190+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=4) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS",
		"]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(190+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS",
		"]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(186+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=5) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]+",
		"[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(192+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDIT",
		"S]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(188+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=6) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]",
		"+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(192+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JULY UNDISTRIBUTED CREDIT",
		"S]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(188+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=7) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]",
		"+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(196+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][AUGUST UNDISTRIBUTED CRED",
		"ITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(192+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=8) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][AUGUST DISTRIBUTED CREDIT",
		"S]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(202+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED C",
		"REDITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS] ",
		"FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(198+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=9) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CRE",
		"DITS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(199+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CR",
		"EDITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(195+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=10) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][OCTOBER DISTRIBUTED CRED",
		"ITS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(201+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED C",
		"REDITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS] F",
		"I");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(197+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=11) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][NOVEMBER DISTRIBUTED CRE",
		"DITS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(201+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][DECEMBER UNDISTRIBUTED C",
		"REDITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS] F",
		"I");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(197+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=12) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][DECEMBER DISTRIBUTED CRE",
		"DITS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(203+1);
	sprintf(temp1,"%s%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][POST YEAR UNDISTRIBUTED ",
		"CREDITS]-[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]",
		" FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	temp1=Rmalloc(199+1);
	sprintf(temp1,"%s%s",
		"IF([LAST FISCAL PROCESSING MONTH]=13) \nTHEN RETURN_VALUE=([FINMGT][FINBYR][POST YEAR DISTRIBUTED CR",
		"EDITS]+[CREDIT ACCUMULATOR]) \nELSE RETURN_VALUE=[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS] FI");
	pvals->name=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	pvals->fieldname=stralloc("[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS]");
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"([LAST ACCOUNT TYPE]=2) AND ([CREDIT ACCUMULATOR])");
	addimageline(tmp,"[FISCAL PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBYR]",TRUE,temp1,31,pval);
	free_pvals(pval,31);

	addimageline(tmp,"REPORT FOOTER","CLOSE [VENPMT][VENVCH]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [VENPMT][VENYR]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [VENPMT][VENFYR]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINBYR]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINRYR]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINEYR]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINBACT]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINRACT]",FALSE,NULL,0,pval);
	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINEACT]",FALSE,NULL,0,pval);

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(8+6+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [VENPMT] Report [VCHDIST1], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
void VCHDIST_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("VENPMT","VCHDIST RANGE SCREEN");
	if(scrn!=NULL)
	{
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","Selected Fiscal Year:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"SELECTED FISCAL YEAR","Selected Fiscal Year","","",0,4,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENVCH][BANK IDENTIFICATION]","Range on Bank Identification","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENVCH][BANK IDENTIFICATION]","From Bank ID","","",0,20,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENVCH][BANK IDENTIFICATION]","To Bank ID","","",0,20,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENVCH][VENDOR IDENTIFICATION]","Range on Vendor Identification","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENVCH][VENDOR IDENTIFICATION]","From Vendor Identification","","",0,20,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENVCH][VENDOR IDENTIFICATION]","To Vendor Identification","","",0,20,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENVCH][CALENDAR YEAR]","Range on Calendar Year","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENVCH][CALENDAR YEAR]","From Calendar Year","","",0,4,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENVCH][CALENDAR YEAR]","To Calendar Year","","",0,4,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENVCH][INVOICE IDENTIFICATION]","Range on Invoice Identification","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENVCH][INVOICE IDENTIFICATION]","From Invoice Identification","","",0,20,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENVCH][INVOICE IDENTIFICATION]","To Invoice Identification","","",0,20,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENINV][DUE DATE]","Range on Due Date","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENINV][DUE DATE]","From Due Date","","",0,8,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENINV][DUE DATE]","To Due Date","","",0,8,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENVCH][ENTRY DATE]","Range on Entry Date","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENVCH][ENTRY DATE]","From Entry Date","","",0,8,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENVCH][ENTRY DATE]","To Entry Date","","",0,8,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENVCH][SOURCE USER]","Range on Souce User","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENVCH][SOURCE USER]","From Source User","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENVCH][SOURCE USER]","To Source User","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"RANGE ON [VENVCH][CHECK NUMBER]","Range on Check Number","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"FROM [VENVCH][CHECK NUMBER]","From Check Number","","",0,12,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"TO [VENVCH][CHECK NUMBER]","To Check Number","","",0,12,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SELECT [VENVCH][MANUAL VOUCHER] FALSE","Select non-manual voucher records","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SELECT [VENVCH][MANUAL VOUCHER] TRUE","Select manual voucher records","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SELECT [VENVCH][CLEARED] FALSE","Select non-cleared records","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SELECT [VENVCH][CLEARED] TRUE","Select cleared records","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RUN REPORT","Run Report","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);

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
			temp1=Rmalloc(20+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [VENPMT] Screen [VCHDIST RANGE SCREEN], Can Not Save Maintain Master!");
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
