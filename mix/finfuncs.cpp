#include <fin.hpp>

char *FISCALPERIOD[14]={"PRE-YEAR","JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER","POST YEAR"};
char *FISCALPERIODLabel[14]={"Pre-Year","January","February","March","April","May","June","July","August","September","October","November","December","Post Year"};
char *FISCALPERIODAbbr[14]={"Pre","Jan","Feb","March","April","May","June","July","Aug","Sept","Oct","Nov","Dec","Post"};

short xgetfinmgtbin(RDAfinmgt *finmgt,int line,char *file)
{
	short x,version=FALSE;
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libname,"%s/rda/FINMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\FINMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG getfinmgtbin Reading Financial Management Setup Variables from [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,"FINMGT SETUP",TRUE);
	if(bin==NULL) 
	{
		prterr("Error Financial Management Setup Variables were not found in Library [%s] or the Library doesn't exist at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read Financial Management Setup Variables at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(x==(-1700)) version=TRUE;
		else version=FALSE;
	finmgt->budget=BINgetint(bin);
	if(version)
	{
		finmgt->month=BINgetshort(bin);
	} else finmgt->month=6;
	BINfree(bin);
	if(libname!=NULL) Rfree(libname);
	return(0);
}
short xwritefinmgtbin(RDAfinmgt *finmgt,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libname,"%s/rda/FINMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\FINMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG writefinmgtbin Writing Financial Management Setup Variables to Library [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1700);	/*type identifier*/
	BINaddint(bin,finmgt->budget);
	BINaddshort(bin,finmgt->month);
	if(writelibbin(libname,bin,"FINMGT SETUP"))
	{
		prterr("Error Can't write Financial Management Setup Variables in binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
RDAfinmgt *xRDAfinmgtNEW(int line,char *file)
{
	RDAfinmgt *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDAfinmgtNEW Creating a structure for the Fincancial Management Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAfinmgt));
	tmp->budget=0;
	tmp->month=0;
	return(tmp);
}
void xfree_finmgt(RDAfinmgt *finmgt,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG free_finmgt freeing memory used for the Financial Management Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(finmgt!=NULL) Rfree(finmgt);
}
short xtestapprotot(short fileno,double amount,char *pmonth,int line,char *file)
{
	short numflds=0,x=0;
	NRDfield *field=NULL;
	double apptot=0,exptot=0,enctot=0,avlbal=0;
	char budgettype;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG testapprotot Testing Available Balance for Module [%s] Fileno [%d] [%s] for Amount [%.02f] Processing Month [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),(amount/100),(pmonth!=NULL ? pmonth:""),line,file); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(pmonth==NULL)
	{
		budgettype=ANNUALBUDGET;
	} else {
		budgettype=MONTHLYBUDGET;
	}
	switch(budgettype)
	{
		case ANNUALBUDGET:
			numflds=NUMFLDS(fileno);
			for(x=0;x<numflds;++x)
			{
				field=FLDNUM(fileno,x);
				if(field!=NULL)
				{
					if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS)
					{
						if((RDAstrstr(field->name,"AMEND")==NULL) && (RDAstrstr(field->name,"UNDISTRIBUTED APPROPRIATION")==NULL)
							&&(RDAstrstr(field->name,"DISTRIBUTED APPROPRIATION")==NULL)
							&&(RDAstrstr(field->name,"APPROPRIATION")!=NULL))
						{
							apptot+=*field->data.float_value;
						} else if((RDAstrstr(field->name,"EXPENDITURE")!=NULL))
						{
							exptot+=*field->data.float_value;
						} else if((RDAstrstr(field->name,"ENCUMBRANCE")!=NULL))
						{
							enctot+=*field->data.float_value;
						}
					}
				}
			}
			break;
		case MONTHLYBUDGET:
			numflds=NUMFLDS(fileno);
			for(x=0;x<numflds;++x)
			{
				field=FLDNUM(fileno,x);
				if(field!=NULL)
				{
					if(((field->type==DOLLARS || field->type==DOLLARS_NOCENTS)&&(RDAstrstr(pmonth,field->name))!=NULL))
					{
						if((RDAstrstr(field->name,"AMEND")==NULL) && (RDAstrstr(field->name,"UNDISTRIBUTED APPROPRIATION")==NULL)
							&&(RDAstrstr(field->name,"DISTRIBUTED APPROPRIATION")==NULL)
							&&(RDAstrstr(field->name,"APPROPRIATION")!=NULL))
						{
							apptot+=*field->data.float_value;
						} else if((RDAstrstr(field->name,"EXPENDITURE")!=NULL))
						{
							exptot+=*field->data.float_value;
						} else if((RDAstrstr(field->name,"ENCUMBRANCE")!=NULL))
						{
							enctot+=*field->data.float_value;
						}
					}
				}
			}
			break;
		default:
			prterr("Error Invalid Budget Type used in Testing Available Balance at line [%d] program [%s].",line,file);
			break;
	}
	avlbal=apptot-exptot-enctot;
	if(apptot==0.0) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("DIAG testapprotot Testing Available Balance is returning available, for No Appropriations($0.00) are on file currently at line [%d] program [%s].",line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(TRUE);
	}
	if((avlbal-amount)>=0) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("DIAG testapprotot Testing Available Balance is returning available balance of [%.02f] - amount of [%.02f] which equals [%.02f] at line [%d] program [%s].",avlbal/100,amount/100,(avlbal-amount)/100,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(TRUE);
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("DIAG testapprotot Testing Available Balance is returning insufficient funds for available balance of [%.02f] - amount of [%.02f] which equals [%.02f] line [%d] program [%s].",avlbal/100,amount/100,(avlbal-amount)/100,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(FALSE);
	}
}
char *xGETBALREF(short balfile,char *acctcode,short deftype,void (*SubFunc)(...),
	void *args,int line,char *file)
{
	char *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GETBALREF Get Balance sheet Reference for Module [%s] File [%d] [%s] Definition [%d] Code [%s] at line [%d] program [%s].",MODULENAME(balfile),balfile,FILENAME(balfile),deftype,acctcode,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	FINDFLDSETSHORT(balfile,"DEFINITION TYPE",deftype);
	FINDFLDSETSTRING(balfile,"ACCOUNT CODE",acctcode);
	if(SubFunc!=NULL)
	{ 
		if(!ADVEQLNRDsec(balfile,1,SubFunc,args))
		{
			FINDFLDGETSTRING(balfile,"REFERENCE TYPE",&tmp);
		}
	} else {
		if(!EQLNRDsec(balfile,1))
		{
			FINDFLDGETSTRING(balfile,"REFERENCE TYPE",&tmp);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GETBALREF returning [%s] at line [%d] program [%s].",tmp,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(tmp);
}
char *xCHECKACCT(short yrfile,short accfile,char *acctcode,short accttype,short deftype,int year,short type,void (*SubFunc)(...),void *args,int line,char *file)
{
	char *tmp=NULL;
	char active=FALSE,delflag=FALSE;
	short ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG CHECKACCT Check Account Year File [%d] [%s] Acct File [%d] [%s] Account Type [%d] Definition Type [%d] Account Code [%s] Year [%d] at line [%d] program [%s].",yrfile,FILENAME(yrfile),accfile,FILENAME(accfile),accttype,deftype,acctcode,year,
line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(acctcode==NULL || RDAstrlen(acctcode)<1)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error No ACCOUNT CODE was entered into CHECKACCT at line [%d] program [%s].",line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(type==0)
		{
			tmp=stralloc("No ACCOUNT CODE was entered.");
		} else {
			tmp=stralloc("NO ACCOUNT CODE");
		}
		return(tmp);
	}
	ZERNRD(accfile);
	FINDFLDSETSHORT(accfile,"DEFINITION TYPE",deftype);
	FINDFLDSETSTRING(accfile,"ACCOUNT CODE",acctcode);
	if(SubFunc!=NULL)
	{
		ef=ADVEQLNRDsec(accfile,1,SubFunc,args);
	} else ef=EQLNRDsec(accfile,1);
	if(ef)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error CHECKACCT has identified an invalid Account Code [%s] at line [%d] program [%s].",acctcode,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(tmp!=NULL) Rfree(tmp);
		if(type==0)
		{
			tmp=Rmalloc(RDAstrlen(acctcode)+212);
			sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED ACCOUNT CODE.  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",acctcode);
		} else {
			tmp=Rmalloc(RDAstrlen(acctcode)+50);
			sprintf(tmp,"UNDEFINED ACCOUNT CODE, Account Code [%s]",acctcode);
		}
		return(tmp);
	
	}
	FINDFLDGETCHAR(accfile,"DELETEFLAG",&delflag);
	if(delflag==TRUE)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error CHECKACCT has identifified an invalid Account Code [%s] marked for deletion at line [%d] program [%s].",acctcode,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(tmp!=NULL) Rfree(tmp); 
		if(type==0)
		{
			tmp=Rmalloc(RDAstrlen(acctcode)+114);
			sprintf(tmp,"The ACCOUNT CODE [%s] that was entered has been marked for deletion.",acctcode);
		} else {
			tmp=Rmalloc(RDAstrlen(acctcode)+40);
			sprintf(tmp,"DELETED ACCOUNT CODE, Account Code [%s]",acctcode);
		}
		return(tmp);
	} 
	FINDFLDGETCHAR(accfile,"ACTIVE",&active);
	if(active==FALSE)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error CHECKACCT has identifified an inactive Account Code [%s] at line [%d] program [%s].",acctcode,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(tmp!=NULL) Rfree(tmp);
		if(type==0)
		{
			tmp=Rmalloc(RDAstrlen(acctcode)+65);
			sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE ACCOUNT CODE.",acctcode); 
		} else {
			tmp=Rmalloc(RDAstrlen(acctcode)+55);
			sprintf(tmp,"INACTIVE ACCOUNT CODE, Account Code [%s]",acctcode); 
		}
		return(tmp);
	}
	if(yrfile!=(-1))
	{
		ZERNRD(yrfile);
		FINDFLDSETSHORT(yrfile,"DEFINITION TYPE",deftype);
		FINDFLDSETINT(yrfile,"FISCAL YEAR",year);
		FINDFLDSETSTRING(yrfile,"ACCOUNT CODE",acctcode);
		if(SubFunc!=NULL)
		{
			ef=ADVEQLNRDsec(yrfile,1,SubFunc,args);
		} else ef=EQLNRDsec(yrfile,1);
		if(ef)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagmix)
			{
				prterr("Error CHECKACCT has identified an Account Code [%s] used with no corresponding Year [%d] at line [%d] program [%s].",acctcode,year,line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(tmp!=NULL) Rfree(tmp);
			if(type==0)
			{
				tmp=Rmalloc(RDAstrlen(acctcode)+239);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",
					acctcode,year);
			} else {
				tmp=Rmalloc(RDAstrlen(acctcode)+80);
				sprintf(tmp,"UNDEFINED YEAR ACCOUNT CODE, Account Code [%s], Year [%d]",acctcode,year);
			}
			return(tmp);
		}
		FINDFLDGETCHAR(yrfile,"DELETEFLAG",&delflag);
		if(delflag==TRUE)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagmix)
			{
				prterr("Error CHECKACCT has identifified an Account Code [%s] used with no corresponding Year [%d] (marked for deletion) at line [%d] program [%s].",acctcode,year,line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(tmp!=NULL) Rfree(tmp);
			if(type==0)
			{
				tmp=Rmalloc(RDAstrlen(acctcode)+144);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if this account should not be deleted.",acctcode,year);
			} else {
				tmp=Rmalloc(RDAstrlen(acctcode)+68);
				sprintf(tmp,"DELETED YEAR ACCOUNT CODE, Account Code [%s], Year [%d]",acctcode,year);
			}
			return(tmp);
		}
		FINDFLDGETCHAR(yrfile,"ACTIVE",&active);
		if(active==FALSE)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagmix)
			{
				prterr("Error CHECKACCT has identifified an Account Code [%s] used with an inactive Year [%d] at line [%d] program [%s].",acctcode,year,line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(tmp!=NULL) Rfree(tmp);
			if(type==0)
			{
				tmp=Rmalloc(RDAstrlen(acctcode)+98);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE YEAR ACCOUNT CODE for the YEAR [%d].",acctcode,year); 
			} else {
				tmp=Rmalloc(RDAstrlen(acctcode)+68);
				sprintf(tmp,"INACTIVE YEAR ACCOUNT CODE, Account Code [%s], Year [%d].",acctcode,year); 
			}
			return(tmp);
		}
	}
	return(tmp);
}
short xGetFinmgtSetup(RDAfinmgt *finsetup,void (*qfunc)(void *),void *arg,int line,
	char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GetFinmgtSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(getfinmgtbin(finsetup)==(-1))
	{
		ERRORDIALOG("Financial Management Setup Not Available","The Financial Management Setup File cannot opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		if(qfunc!=NULL) qfunc(arg);
		return(-1);
	}
	return(0);
}

static void xupdatewrt(short yrnum,int year,short deftype,char *accountcode,
char *fieldname,double amount,void (*SubFunc)(...),void *args,int line,char *file)
{
	double initamt=0.0;
	RDATData *previous=NULL;
	short ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG update undistributed write amounts of [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(yrnum);
	FINDFLDSETINT(yrnum,"FISCAL YEAR",year);
	FINDFLDSETSHORT(yrnum,"DEFINITION TYPE",deftype);
	FINDFLDSETSTRING(yrnum,"ACCOUNT CODE",accountcode);
	LOCNRDFILE(yrnum);
	if(SubFunc!=NULL)
	{
		ef=ADVEQLNRDsec(yrnum,1,SubFunc,args);
	} else ef=EQLNRDsec(yrnum,1);
	if(ef)
	{
		/*todo - warning dialog*/
		if(RDAstrcmp(USERLOGIN,"dct"))
		{
		prterr("Error can't find Account Year Record, year=[%d], deftype=[%d], acc=[%s]",year,deftype,accountcode);
		} else {
			prterr("\"%d\",\"%d\",\"%s\"\r\n",year,deftype,(accountcode!=NULL ? accountcode:""));
		}
	} else {
		previous=RDATDataNEW(yrnum);
		FINDFLDGETDOUBLE(yrnum,fieldname,&initamt);
		initamt+=amount;
		FINDFLDSETDOUBLE(yrnum,fieldname,initamt);
		if(SubFunc!=NULL)
		{
			ef=ADVWRTTRANSsec(yrnum,0,NULL,previous,SubFunc,args);
		} else ef=WRTTRANSsec(yrnum,0,NULL,previous);
		if(ef)
		{
			/*todo - warning dialog*/
			prterr("Error writing Account Year Record, year=[%d], deftype=[%d], acc=[%s]",year,deftype,accountcode);
		}
		if(previous!=NULL) FreeRDATData(previous);
	}
	UNLNRDFILE(yrnum);
}
void xupdatefinyear(short filenum,char *accountfld,char *debitfld,
char *creditfld,char *payablefld,short yrnum,short byrnum,int year,
short deftype,char *accounttype,char *month,short type,double amount,
void (*SubFunc)(...),void *args,int line,char *file)
{
	char *accountcode=NULL;
	char *debitcode=NULL;
	char *creditcode=NULL;
	char *payablecode=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG update undistributed amounts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(accountfld!=NULL) FINDFLDGETSTRING(filenum,accountfld,&accountcode);
	if(debitfld!=NULL) FINDFLDGETSTRING(filenum,debitfld,&debitcode);
	if(creditfld!=NULL) FINDFLDGETSTRING(filenum,creditfld,&creditcode);
	if(payablefld!=NULL) FINDFLDGETSTRING(filenum,payablefld,&payablecode);
	ADVUpdateFinMgtYear(deftype,accountcode,debitcode,creditcode,
		payablecode,yrnum,byrnum,year,accounttype,month,type,amount,
		NULL,NULL);
}

short xCHECKACCTW(RDArsrc *mtnrsrc,char *accountfld,char *yearfld,
short deftype,short yrnum,short accnum,short accttype,char *errorname,
short skipnull,void (*SubFunc)(...),void *args,int line,char *file)
{
	int procyrid=0;
	char *accountcode=NULL,*tmp=NULL;
	short rtn=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG checkacctw(arning) at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(yrnum!=(-1))
	{
		readwidget(mtnrsrc,yearfld);
		FINDRSCGETINT(mtnrsrc,yearfld,&procyrid);
	}
	readwidget(mtnrsrc,accountfld);
	FINDRSCGETSTRING(mtnrsrc,accountfld,&accountcode);
	if(skipnull)
	{
		if(accountcode==NULL) return(TRUE);
		if(RDAstrlen(accountcode)<1)
		{
			Rfree(accountcode);
			return(TRUE);
		}
	}
	if((tmp=ADVCHECKACCT(yrnum,accnum,accountcode,accttype,deftype,procyrid,SubFunc,args))
		!=NULL)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",errorname,tmp,NULL,NULL,
			TRUE,NULL);
		rtn=FALSE;
		Rfree(tmp);
	}
	if(accountcode!=NULL) Rfree(accountcode);
	return(rtn);
}
void xUpdateFinMgtYear(short deftype,char *accountcode,
	char *debitcode,char *creditcode,char *payablecode,
	short yrnum,short byrnum,int year,char *accounttype,char *month,
	short type,double amount,void (*SubFunc)(...),void *args,
	int line,char *file)
{
	char *fieldname=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG Update Undistributed Amounts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(accountcode))
	{
		if(type==0)
		{
			fieldname=Rmalloc(RDAstrlen(month)+RDAstrlen(accounttype)+16);
			sprintf(fieldname,"%s UNDISTRIBUTED %s",month,accounttype);
		} else if(type==1)
		{
			fieldname=Rmalloc(RDAstrlen(month)+RDAstrlen(accounttype)+14);
			sprintf(fieldname,"%s DISTRIBUTED %s",month,accounttype);
		}
		xupdatewrt(yrnum,year,deftype,accountcode,fieldname,amount,SubFunc,args,line,file);
		if(fieldname!=NULL) Rfree(fieldname);
	}
	if(!isEMPTY(creditcode))
	{
		if(type==0)
		{
			fieldname=Rmalloc(RDAstrlen(month)+23);
			sprintf(fieldname,"%s UNDISTRIBUTED CREDITS",month);
		} else if(type==1)
		{
			fieldname=Rmalloc(RDAstrlen(month)+21);
			sprintf(fieldname,"%s DISTRIBUTED CREDITS",month);
		}
		xupdatewrt(byrnum,year,deftype,creditcode,fieldname,amount,SubFunc,args,line,file);
		if(fieldname!=NULL) Rfree(fieldname);
	}
	if(!isEMPTY(debitcode))
	{
		if(type==0)
		{
			fieldname=Rmalloc(RDAstrlen(month)+22);
			sprintf(fieldname,"%s UNDISTRIBUTED DEBITS",month);
		} else if(type==1)
		{
			fieldname=Rmalloc(RDAstrlen(month)+20);
			sprintf(fieldname,"%s DISTRIBUTED DEBITS",month);
		}
		xupdatewrt(byrnum,year,deftype,debitcode,fieldname,amount,SubFunc,args,line,file);
		if(fieldname!=NULL) Rfree(fieldname);
	}
	if(!isEMPTY(payablecode) && !isEMPTY(debitcode))
	{
		if(type==0)
		{
			fieldname=Rmalloc(RDAstrlen(month)+23);
			sprintf(fieldname,"%s UNDISTRIBUTED CREDITS",month);
		} else if(type==1)
		{
			fieldname=Rmalloc(RDAstrlen(month)+21);
			sprintf(fieldname,"%s DISTRIBUTED CREDITS",month);
		}
		xupdatewrt(byrnum,year,deftype,payablecode,fieldname,amount,SubFunc,args,line,file);
		if(fieldname!=NULL) Rfree(fieldname);
	}
	if(!isEMPTY(payablecode) && !isEMPTY(creditcode))
	{
		if(type==0)
		{
			fieldname=Rmalloc(RDAstrlen(month)+22);
			sprintf(fieldname,"%s UNDISTRIBUTED DEBITS",month);
		} else if(type==1)
		{
			fieldname=Rmalloc(RDAstrlen(month)+20);
			sprintf(fieldname,"%s DISTRIBUTED DEBITS",month);
		}
		xupdatewrt(byrnum,year,deftype,payablecode,fieldname,amount,SubFunc,args,line,file);
		if(fieldname!=NULL) Rfree(fieldname);
	}
}

