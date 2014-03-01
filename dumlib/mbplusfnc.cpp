#include<mbplus.hpp>
#include<misc.hpp>

struct vacf varac;
char offbs,partbs;
struct finadss finadsd;

short xfindpart(char *s,short type,int line,char *file)
{
	short x;

	if(type<0 || type>4)
	{
		prterr("Error findpart has invalid account type [%d] at line [%d] program [%s].",type,line,file);
		return(-1);
	}
	for(x=0;x<10;++x) if(!strcmp(varac.header[x][type],s)) break;
	if(x>9)
	{
		prterr("Error findpart failed to find part [%d] at line [%d] program [%s].",x,line,file);
	}
	return(x<10?x:(-1));
}

short xoffpart(short p,short type,int line,char *file)
{
	short x,y;

	for(x=0,y=1;x<p;++x) if(varac.bl[x][type]>0) y+=varac.bl[x][type]+1;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG offpart returning [%d] at line [%d] program [%s].",y,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(y);
}
static char *ACCtype[]={"Expenditure","Revenue","Balance Sheet","Beginning Balance"};
short xacread(int line,char *file)
{
	int buf0;
	int x=0,y=0,count=0;

#ifndef WIN32
	buf0 = Ropen("./data/variable.dta");
#endif
#ifdef WIN32
	buf0 = Ropen(".\\data\\VARIABLE.DTA");
#endif
	if (buf0<0) 
	{
		prterr("Error Couldn't Read VARIABLE.DTA file at line [%d] program [%s].",line,file);
		return(-1);
	}
	Rseek(buf0,(long)512,0);
	Rread (buf0,&varac,sizeof(varac));
	Rclose(buf0);
	partbs=findpart("B/S",BAL_ACCT);
	if(partbs>=0) offbs=offpart(partbs,BAL_ACCT); else offbs=1;
	for(x=0;x<4;++x)
	{
		prterr("Starting %s ",ACCtype[x]);TRACE;
		count=0;
		for(y=0;y<10;++y)
		{
			if((varac.bl[y][x]!=0) && !isEMPTY(varac.header[y][x])) ++count;
		}
		for(y=0;y<count;++y)
		{
			prterr("\t%-5s %d %d ",varac.header[y][x],varac.bl[y][x],varac.ll[y][x]);TRACE;
		}
	}
	return(0);
}
short xadread(int line,char *file)
{
	int buf0;

#ifndef WIN32
	buf0 = Ropen("./data/variable.dta");
#endif
#ifdef WIN32
	buf0 = Ropen(".\\data\\VARIABLE.DTA");
#endif
	if (buf0<0) 
	{
		prterr("Error Couldn't Read VARIABLE.DTA file at line [%d] program [%s].",line,file);
		return(-1);
	}
	Rseek(buf0,(long)2048,0);
	Rread (buf0,&finadsd,sizeof(struct finadss));
	Rclose(buf0);
	return(0);
}
void MakeXpertAccountStructure()
{
	int x,count=0;
	RDAacct *a=NULL;
	RDAaccpart *p=NULL;

	if(acread()) 
	{
		prterr("Error MakeXpertAccountStructure from MBGUI's VARIABLE.DTA Failed.");
		return;
	}
	if(EXPENDITURE_ACCOUNT!=NULL) FreeRDAacctmstr(EXPENDITURE_ACCOUNT);
	if(REVENUE_ACCOUNT!=NULL) FreeRDAacctmstr(REVENUE_ACCOUNT);
	if(BALANCE_SHEET_ACCOUNT!=NULL) 
		FreeRDAacctmstr(BALANCE_SHEET_ACCOUNT);
	if(BEGINNING_BALANCE_ACCOUNT!=NULL) 
		FreeRDAacctmstr(BEGINNING_BALANCE_ACCOUNT);
	EXPENDITURE_ACCOUNT=RDAacctNEW(EXP_ACCT);
	REVENUE_ACCOUNT=RDAacctNEW(REV_ACCT);
	BALANCE_SHEET_ACCOUNT=RDAacctNEW(BAL_ACCT);
	BEGINNING_BALANCE_ACCOUNT=RDAacctNEW(BEG_ACCT);
	count=0;
	for(x=0;x<10;++x)
	{
		if((varac.bl[x][EXP_ACCT]!=0) && !isEMPTY(varac.header[x][EXP_ACCT])) ++count;
	}
	if(count>0)
	{
		EXPENDITURE_ACCOUNT->codetype=Rmalloc(sizeof(RDAacct));
		EXPENDITURE_ACCOUNT->num=1;
		a=EXPENDITURE_ACCOUNT->codetype;
		a->name=stralloc("MicroBUDGET+");
		a->acct=Rmalloc(count*sizeof(RDAaccpart));
		a->num=count;
		for(x=0;x<count;++x)
		{
			p=a->acct+x;
			p->name=stralloc(varac.header[x][EXP_ACCT]);
			p->length=varac.bl[x][EXP_ACCT];
			p->definition=Rmalloc(p->length+1);
			memset(p->definition,'N',p->length);	
			p->default_zero=FALSE;
			p->force_zero=FALSE;
			p->last_dimension=FALSE;
		}
		if(writeacctbin(EXPENDITURE_ACCOUNT))
		{
			prterr("Error Attempting to Write EXPENDITURE_ACCOUNT definitions.");
		}
	}
	count=0;
	for(x=0;x<10;++x)
	{
		if((varac.bl[x][REV_ACCT]!=0) && !isEMPTY(varac.header[x][REV_ACCT])) ++count;
	}
	if(count>0)
	{
		REVENUE_ACCOUNT->codetype=Rmalloc(sizeof(RDAacct));
		a=REVENUE_ACCOUNT->codetype;
		REVENUE_ACCOUNT->num=1;
		a->name=stralloc("MicroBUDGET+");
		a->acct=Rmalloc(count*sizeof(RDAaccpart));
		a->num=count;
		for(x=0;x<count;++x)
		{
			p=a->acct+x;
			p->name=stralloc(varac.header[x][REV_ACCT]);
			p->length=varac.bl[x][REV_ACCT];
			p->definition=Rmalloc(p->length+1);
			memset(p->definition,'N',p->length);	
			p->default_zero=FALSE;
			p->force_zero=FALSE;
			p->last_dimension=FALSE;
		}
		if(writeacctbin(REVENUE_ACCOUNT))
		{
			prterr("Error Attempting to Write REVENUE_ACCOUNT definitions.");
		}
	}
	count=0;
	for(x=0;x<10;++x)
	{
		if((varac.bl[x][BAL_ACCT]!=0) && !isEMPTY(varac.header[x][BAL_ACCT])) ++count;
	}
	if(count>0)
	{
		BALANCE_SHEET_ACCOUNT->codetype=Rmalloc(sizeof(RDAacct));
		a=BALANCE_SHEET_ACCOUNT->codetype;
		a->name=stralloc("MicroBUDGET+");
		BALANCE_SHEET_ACCOUNT->num=1;
		a->acct=Rmalloc(count*sizeof(RDAaccpart));
		a->num=count;
		for(x=0;x<count;++x)
		{
			p=a->acct+x;
			p->name=stralloc(varac.header[x][BAL_ACCT]);
			p->length=varac.bl[x][BAL_ACCT];
			p->definition=Rmalloc(p->length+1);
			memset(p->definition,'N',p->length);	
			p->default_zero=FALSE;
			p->force_zero=FALSE;
			p->last_dimension=FALSE;
		}
		if(writeacctbin(BALANCE_SHEET_ACCOUNT))
		{
			prterr("Error Attempting to Write BALANCE_SHEET_ACCOUNT definitions.");
		}
	}
	count=0;
	for(x=0;x<10;++x)
	{
		if((varac.bl[x][BEG_ACCT]!=0) && !isEMPTY(varac.header[x][BEG_ACCT])) ++count;
	}
	if(count>0)
	{
		BEGINNING_BALANCE_ACCOUNT->codetype=Rmalloc(sizeof(RDAacct));
		a=BEGINNING_BALANCE_ACCOUNT->codetype;
		BEGINNING_BALANCE_ACCOUNT->num=1;
		a->name=stralloc("MicroBUDGET+");
		a->acct=Rmalloc(count*sizeof(RDAaccpart));
		a->num=count;
		for(x=0;x<count;++x)
		{
			p=a->acct+x;
			p->name=stralloc(varac.header[x][BEG_ACCT]);
			p->length=varac.bl[x][BEG_ACCT];
			p->definition=Rmalloc(p->length+1);
			memset(p->definition,'N',p->length);	
			p->default_zero=FALSE;
			p->force_zero=FALSE;
			p->last_dimension=FALSE;
		}
		if(writeacctbin(BEGINNING_BALANCE_ACCOUNT))
		{
			prterr("Error Attempting to Write BEGINNING_BALANCE_ACCOUNT definitions.");
		}
	}
}
static short AddFINSBCRecord(short fileno,char *balsht,char *name,char *type,char std)
{
	ZERNRD(fileno);
	FINDFLDSETSTRING(fileno,"BALANCE SHEET",balsht);
	FINDFLDSETSTRING(fileno,"REFERENCE TYPE",type);
	FINDFLDSETSTRING(fileno,"NAME",name);
	FINDFLDSETCHAR(fileno,"STANDARD",std);
	return(WRTNRD(fileno,0,NULL));
}
void MBPlusFINSBC()
{
	short fileno=(-1);

	if(acread()) 
	{
		prterr("Error MakeXpertAccountStructure from MBGUI's VARIABLE.DTA Failed.");
		return;
	}
	fileno=OPNNRD("FINMGT","FINSBC");
	if(fileno!=(-1))
	{
		if(!isEMPTY(varac.bs_code[0]))
			AddFINSBCRecord(fileno,varac.bs_code[0],"Cash",
				"CASH",TRUE);
		if(!isEMPTY(varac.bs_code[1]))
			AddFINSBCRecord(fileno,varac.bs_code[1],"Revenue",
				"REVENUE",TRUE);
		if(!isEMPTY(varac.bs_code[2]))
			AddFINSBCRecord(fileno,varac.bs_code[2],"Expenditure",
				"EXPENDITURE",TRUE);
		if(!isEMPTY(varac.bs_code[3]))
			AddFINSBCRecord(fileno,varac.bs_code[3],
				"Accounts Payable","ACCOUNTS PAYABLE",TRUE);
		if(!isEMPTY(varac.bs_code[4]))
			AddFINSBCRecord(fileno,varac.bs_code[4],"Encumbrance",
				"ENCUMBRANCE",TRUE);
		if(!isEMPTY(varac.bs_code[5]))
			AddFINSBCRecord(fileno,varac.bs_code[5],
				"Reserve for Encumbrance",
				"RESERVE FOR ENCUMBRANCE",TRUE);
		if(!isEMPTY(varac.bs_code[6]))
			AddFINSBCRecord(fileno,varac.bs_code[6],
				"Salaries / Benefits Payable",
				"SALARIES / BENEFITS PAYABLE",TRUE);
		if(!isEMPTY(varac.bs_code[7]))
			AddFINSBCRecord(fileno,varac.bs_code[7],"Fund Balance",
				"FUND BALANCE",TRUE);
		if(!isEMPTY(varac.bs_code[8]))
			AddFINSBCRecord(fileno,varac.bs_code[8],
				"Estimated Revenue","ESTIMATED REVENUE",TRUE);
		if(!isEMPTY(varac.bs_code[9]))
			AddFINSBCRecord(fileno,varac.bs_code[9],
				"Appropriation","APPROPRIATION",TRUE);
		if(!isEMPTY(varac.bs_code[10]))
			AddFINSBCRecord(fileno,varac.bs_code[10],
				"Accounts Receivable","ACCOUNTS RECEIVABLE",
				TRUE);
		if(!isEMPTY(varac.bs_code[11]))
			AddFINSBCRecord(fileno,varac.bs_code[11],
				"Discounts Allowed",
				"DISCOUNTS ALLOWED",TRUE);
		if(!isEMPTY(varac.bs_code[12]))
			AddFINSBCRecord(fileno,varac.bs_code[12],
				"Prior Year Encumbrance",
				"PRIOR YEAR ENCUMBRANCE",TRUE);
		if(!isEMPTY(varac.bs_code[13]))
			AddFINSBCRecord(fileno,varac.bs_code[13],
				"Deposits Payable","DEPOSITS PAYABLE",TRUE);
		CLSNRD(fileno);
	}
}
