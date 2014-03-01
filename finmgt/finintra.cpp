/* finintra.c - Financial Management Intra-Bank Transfers */
#ifndef WIN32
#define __NAM__ "finintra.lnx"
#endif
#ifdef WIN32
#define __NAM__ "finintra.exe"
#endif
#include <app.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <rdadiag.h>
#include <alloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <olh.h>
#include <mix.h>
#include <rptgen.h>
#include <prterr.h>
#include <miscfunc.h>
#include <fin.h>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static short filenum=0,yearnum=0,balnum=0,jrnlnum=0;
static short byrnum=0;
static RDArsrc *mainrsrc=NULL,*definelist=NULL;
static APPlib *prevtrans=NULL,*DEFtypes=NULL;
static APPlib *yearavl=NULL,*cashfromavl=NULL,*cashtoavl=NULL;
static APPlib *journalavl=NULL,*pmonth=NULL;
static char *module="FINMGT";

void mk_intra_define_list(short fileno)
{
	short x,l;
	RDArmem *member;
	char *s=NULL,*fieldname=NULL;
	int sellist=0;
	short listvalue=0;

	for(x=0,member=mainrsrc->rscs+x;x<mainrsrc->numrscs;++x)
	{
		s=Rmalloc(RDAstrlen(member->rscrname)+1);
		sprintf(s,"%s",member->rscrname);
		if(member->field_type!=SCROLLEDTEXT 
			&& member->field_type!=BUTTONS
			&& RDAstrcmp(member->rscrname,"DELETEFLAG"))
		{
			fieldname=stripfilename(s);
			if(s!=NULL) 
			{
				Rfree(s); 
				s=NULL; 
			}
			if(fieldname!=NULL)
			{
				l=RDAstrlen(fieldname)+10;
				s=Rmalloc(l);
				sprintf(s,"%s POSITION",fieldname);
				prterr("s=%s:[%d],l=[%d]",s,RDAstrlen(s),l);TRACE;
				l=x+1;
				addstdrsrc(definelist,s,SHORTV,3,&l,TRUE);	
				if(s!=NULL) { Rfree(s); s=NULL; }
				l=RDAstrlen(fieldname)+8;
				s=Rmalloc(l);
				sprintf(s,"%s LENGTH",fieldname);
				prterr("s=%s:[%d],l=[%d]",s,RDAstrlen(s),l);TRACE;
				l=RDAstrlen(s);
				if(l>40) l=40;
				addstdrsrc(definelist,s,SHORTV,3,&l,TRUE);	
			}
		}
		if(s!=NULL) Rfree(s);
		if(fieldname!=NULL) Rfree(fieldname);
	}
	addbtnrsrc(definelist,"DEFAULTS",TRUE,SaveDefaults,definelist);
	addrfkwrsrc(definelist,"SELECT",TRUE,donothing,NULL);
	addrfkwrsrc(definelist,"QUIT",TRUE,donothing,NULL);
	addrfcbrsrc(definelist,"HELP",TRUE,screenhelp,definelist);
	addrfcbrsrc(definelist,"PRINT RESOURCES",TRUE,printrsrcs,definelist);
	DefaultScreens(definelist);
}
void savelist()
{
	readallwidgets(definelist);
}
void definelistfunc()
{
	if(makescrn(definelist,FALSE)) 
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen INTRA BANK TRANS DEFINE LIST.  Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		return;
	}
}
static void save_record()
{
	int selcashfrom=0,selcashto=0;
	int seljrnl=0,selyear=0,selmonth=0,seldeftype=0;
	char *rcddesc=NULL,deleteflag=FALSE,*nullstring=NULL;
	short f=0,x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member,*member;
	char *s,*hold=NULL,*temp=NULL,*date=NULL,*timex=NULL,*period=NULL;
	NRDfield *field;
	double amount=0,initamt=0;

	readallwidgets(mainrsrc);
	if((!strncmp(cashfromavl->libs[0],"No Accounts Available",21))
		||(!strncmp(cashtoavl->libs[0],"No Accounts Available",21)))
	{
		ERRORDIALOG("NO ACCOUNTS AVAILABLE","This Bank Transfer can not be saved without an ACCOUNT CODE selected.  Check to see that there is atleast one ACCOUNT CODE defined for the BALANCE SHEET ACCOUNT TYPE.  If there is and it is not displayed as a selection on both of the CASHTO & CASHFROM CODES LISTS call your installer.",NULL,FALSE);
		prterr("NO ACCOUNTS AVAILABLE for the corresponds ACCOUNT TYPE selected from the list in ADD INTRA BANK TRANS.");
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"CASHFROM CODES",&selcashfrom)) return;
	if(FINDRSCGETINT(mainrsrc,"CASHTO CODES",&selcashto)) return;
	if(FINDRSCGETDOUBLE(mainrsrc,"[FINREC][AMOUNT]",&amount)) return;
	if(amount==0) return;	
	if(FINDRSCGETINT(mainrsrc,"JOURNALS AVAILABLE",&seljrnl)) return;
	if(FINDRSCGETINT(mainrsrc,"YEARS AVAILABLE",&selyear)) return;
	if(FINDRSCGETINT(mainrsrc,"PROCESSING MONTHS",&selmonth)) return;
	selyear=Ratoi(yearavl->libs[selyear]);
	period=stralloc(pmonth->libs[selmonth]);
	FINDFLDSETINT(byrnum,"FISCAL YEAR",selyear);
	x=(short)seldeftype;
	FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",x);
	FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",cashfromavl->libs[selcashfrom]);
	if(amount<0)
	{
		sprintf(stemp,"%s DISTRIBUTED DEBITS",period);
	} else {
		sprintf(stemp,"%s DISTRIBUTED CREDITS",period);
	}
	KEYNRD(byrnum,1);
	ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,mainrsrc);
	LOCNRD(byrnum);
	FINDFLDGETDOUBLE(byrnum,stemp,&initamt);
	initamt+=amount;
	FINDFLDSETDOUBLE(byrnum,stemp,initamt);
	WRTNRDtrans(byrnum,0,NULL);
	UNLNRD(byrnum);
	FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",cashtoavl->libs[selcashto]);
	if(amount<0)
	{
		sprintf(stemp,"%s DISTRIBUTED CREDITS",period);
	} else {
		sprintf(stemp,"%s DISTRIBUTED DEBITS",period);
	}
	KEYNRD(byrnum,1);
	ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,mainrsrc);
	LOCNRD(byrnum);
	FINDFLDGETDOUBLE(byrnum,stemp,&initamt);
	initamt+=amount;
	FINDFLDSETDOUBLE(byrnum,stemp,initamt);
	WRTNRDtrans(byrnum,0,NULL);
	UNLNRD(byrnum);
	if(period!=NULL) Rfree(period);
	FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(filenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(filenum,"SOURCE USER VERIFIED",deleteflag);
	x=2;
	FINDFLDSETSHORT(filenum,"SOURCE PROCESS TYPE",x);
	FINDFLDGETSTRING(filenum,"DATE",&date);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	if(date==NULL || RDAstrlen(date)<1)
	{
		FINDFLDSETSTRING(filenum,"DATE",date);
	}
	if(date!=NULL) Rfree(date);
	FINDFLDGETSTRING(filenum,"TIME",&timex);
	if(timex==NULL || RDAstrlen(timex)<1)
	{
		FINDFLDSETSTRING(filenum,"TIME",timex);	
	}
	if(timex!=NULL) Rfree(timex);
	if(ADVRECORDsec(filenum,SCRNvirtualSubData,mainrsrc))
	{
		rcddesc=ADVRECORDsecDesc(filenum,SCRNvirtualSubData,mainrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		if(!WRTNRDtrans(filenum,1,"TRANSACTION NUMBER"))
		{
			for(x=1,length=0;x<(definelist->numrscs-4);x+=2)
			{
				pos_member=definelist->rscs+x;
				len_member=definelist->rscs+(x+1);
				if(*pos_member->value.short_value>0) 
				{
					length+=(*len_member->value.short_value+2);
				}
			}
			if(prevtrans->numlibs==1 && !RDAstrcmp(prevtrans->libs[0],
				"No Bank Transfers added this session"))
			{
				freeapplib(prevtrans);
				prevtrans=APPlibNEW();
			}
			hold=Rmalloc(length+1);
			lcount=0;
			cur=0;
			while(cur<(definelist->numrscs+1))
			{
				for(x=1;x<(definelist->numrscs-4);x+=2)
				{
					pos_member=definelist->rscs+x;
					len_member=definelist->rscs+(x+1);
					if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
					{
						if(*pos_member->value.short_value==cur)
						{
							s=stralloc(pos_member->rscrname);
							s[RDAstrlen(s)-9]=0;
							field=FLDNRD(filenum,s);
							if(s!=NULL) Rfree(s);
							if(field!=NULL)
							{
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case ZIPCODE:
									case SOCSECNUM:
									case DATES:
									case TIMES:
									case EXPENDITURE:
									case BALANCESHEET:
									case REVENUE:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
										if(*len_member->value.short_value<RDAstrlen(field->data.string_value))
										{
											sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
										} else {
											sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
										}
										break;
									case DECIMALV:
									case SDECIMALV:
										sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,*field->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.short_value);
										break;
									case LONGV:
									case SLONGV:
										sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.integer_value);
										break;
									case DOLLARS:
										temp=famt(*field->data.float_value,*len_member->value.short_value);
										sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case DOUBLEV:
									case SDOUBLEV:
										sprintf(&hold[lcount],"%*.0f ",*len_member->value.short_value,*field->data.float_value);
										break;
									case BOOLNS:
										sprintf(&hold[lcount],"%c ",(*field->data.string_value ? 'Y':'N'));
										break;
									case CHARACTERS:
										sprintf(&hold[lcount],"%c ",*field->data.string_value);
										break;
									case BUTTONS:
									case SCROLLEDLIST:
										prterr("Huh? Do a %s?",standardfieldtypes[field->type]);
										break;
								}
								lcount+=*len_member->value.short_value+1;
							} else {
								prterr("Error field [%s] not found.");
							}
						}
					}
				}
				++cur;
			}
			addAPPlib(prevtrans,hold);
			if(hold!=NULL) Rfree(hold);
			hold=NULL;
		} else {
			ERRORDIALOG("Write Error Occurred.","The write function failed, the record you attempted to save was not created. Please try again.",NULL,FALSE);
		}
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"PREVIOUS BANK TRANSFERS"
		,(prevtrans->numlibs-1),prevtrans))
	{
		updatersrc(mainrsrc,"PREVIOUS BANK TRANSFERS");
	}
}
static void quit_record()
{
	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist);
		definelist=NULL;
	}
	if(prevtrans->numlibs>1 || RDAstrcmp(prevtrans->libs[0],
		"No Bank Transfers added this session"))
	{
		RUNREPORT("FINMGT","INTRAUD",NULL,NULL,TRUE,FALSE);
	}
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	if(balnum!=(-1)) CLSNRD(balnum);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(yearnum!=(-1)) CLSNRD(yearnum);
	if(jrnlnum!=(-1)) CLSNRD(jrnlnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(yearavl!=NULL) freeapplib(yearavl);
	if(cashtoavl!=NULL) freeapplib(cashtoavl);
	if(cashfromavl!=NULL) freeapplib(cashfromavl);
	if(journalavl!=NULL) freeapplib(journalavl);
	if(pmonth!=NULL) freeapplib(pmonth);
	if(prevtrans!=NULL) freeapplib(prevtrans);
	ShutdownSubsystems();
	exit(0);
}
static void getcashfromdesc()
{
	int selacct=0,deftype=0;
	short filedeftype=0;
	char *descript=NULL;

	FINDRSCGETINT(mainrsrc,"CASHFROM CODES",&selacct);
	descript=stralloc(cashfromavl->libs[selacct]);
	FINDRSCGETINT(mainrsrc,"DEFINITION TYPES",&deftype);
	filedeftype=(short)deftype;
	ZERNRD(balnum);
	FINDFLDSETSHORT(balnum,"DEFINITION TYPE",filedeftype);
	FINDFLDSETSTRING(balnum,"ACCOUNT CODE",descript);
	KEYNRD(balnum,1);
	ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,mainrsrc);
	if(descript!=NULL) Rfree(descript);
	FINDFLDGETSTRING(balnum,"DESCRIPTION",&descript);
	FINDRSCSETSTRING(mainrsrc,"CASHFROM DESCRIPTION",descript);
	updatersrc(mainrsrc,"CASHFROM DESCRIPTION");
	if(descript!=NULL) Rfree(descript);
}
static void getcashtodesc()
{
	int selacct=0,deftype=0;
	short filedeftype=0;
	char *descript=NULL;

	FINDRSCGETINT(mainrsrc,"CASHTO CODES",&selacct);
	descript=stralloc(cashtoavl->libs[selacct]);
	FINDRSCGETINT(mainrsrc,"DEFINITION TYPES",&deftype);
	filedeftype=(short)deftype;
	ZERNRD(balnum);
	FINDFLDSETSHORT(balnum,"DEFINITION TYPE",filedeftype);
	FINDFLDSETSTRING(balnum,"ACCOUNT CODE",descript);
	KEYNRD(balnum,1);
	ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,mainrsrc);
	if(descript!=NULL) Rfree(descript);
	FINDFLDGETSTRING(balnum,"DESCRIPTION",&descript);
	FINDRSCSETSTRING(mainrsrc,"CASHTO DESCRIPTION",descript);
	updatersrc(mainrsrc,"CASHTO DESCRIPTION");
	if(descript!=NULL) Rfree(descript);
}
void changeyear()
{
	int yearid=0,yearno=0,x=0;
	RDArmem *member;

	if(FINDRSCGETINT(mainrsrc,"YEARS AVAILABLE",&yearno)) return;
	yearid=Ratoi(yearavl->libs[yearno]);
	FINDFLDSETINT(yearnum,"FISCAL YEAR",yearid);
	FINDFLDSETINT(filenum,"FISCAL YEAR",yearid);
	KEYNRD(yearnum,1);
	ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mainrsrc);
	filerecord2rsrc(yearnum,mainrsrc);
	for(x=0,member=mainrsrc->rscs+x;x<mainrsrc->numrscs;++x,++member)
	{
		if(member->field_type!=SCROLLEDLIST && member->type<1 &&
			IsWidgetDisplayed(member))
		{
			updatersrc(mainrsrc,member->rscrname);
		}
	}	
}
void changejournal()
{
	int jrnalno=0,x=0;
	RDArmem *member;
	short deftype=0,defaccttype=0;

	if(FINDRSCGETINT(mainrsrc,"JOURNALS AVAILABLE",&jrnalno)) return;
	FINDFLDSETSTRING(jrnlnum,"JOURNAL IDENTIFICATION",journalavl->libs[jrnalno]);
	FINDFLDSETSTRING(filenum,"JOURNAL IDENTIFICATION",journalavl->libs[jrnalno]);
	KEYNRD(jrnlnum,1);
	ADVEQLNRDsec(jrnlnum,1,SCRNvirtualSubData,mainrsrc);
	filerecord2rsrc(jrnlnum,mainrsrc);
	for(x=0,member=mainrsrc->rscs+x;x<mainrsrc->numrscs;++x,++member)
	{
		if(member->type<1 && member->field_type!=SCROLLEDLIST && 
			IsWidgetDisplayed(member))
		{
			updatersrc(mainrsrc,member->rscrname);
		}
	}	
}
void setDEFtype()
{
	int x=0;
	RDAacct *acct;

	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	DEFtypes=APPlibNEW();
	if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
	{
		for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
		x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
		{
			addAPPlib(DEFtypes,acct->name);
		}
	}
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITION TYPES",0,DEFtypes))
		updatersrc(mainrsrc,"DEFINITION TYPES");
}
void changeDEFtype()
{
	int selecteda=0,selectedd=0,jtype=0;
	short ef=0,deftype=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL;

	if(FINDRSCGETINT(mainrsrc,"DEFINITION TYPES",&selectedd)) return;
	if(cashtoavl!=NULL) freeapplib(cashtoavl);
	cashtoavl=APPlibNEW();
	if(cashfromavl!=NULL) freeapplib(cashfromavl);
	cashfromavl=APPlibNEW();
	deftype=selectedd;
	ZERNRD(balnum);
	FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype);
	ef=ADVGTENRDsec(balnum,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSHORT(balnum,"DEFINITION TYPE",&deftype);
		if(deftype!=selectedd) break;
		FINDFLDGETSTRING(balnum,"ACCOUNT CODE",&account_code);
		FINDFLDGETCHAR(balnum,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(balnum,"ACTIVE",&active);
		if(account_code!=NULL && !deleteflag && active) 
		{	
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(!RDAstrcmp(baltype,"CASH"))
			{
				addAPPlib(cashfromavl,account_code);
				addAPPlib(cashtoavl,account_code);
			}
			Rfree(account_code);
			if(baltype!=NULL) Rfree(baltype);
			baltype=NULL;
			account_code=NULL;
		}
		ef=ADVNXTNRDsec(balnum,1,SCRNvirtualSubData,mainrsrc);
	}
	if(cashfromavl->numlibs<1)
	{
		addAPPlib(cashfromavl,"No Accounts Available");
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"CASHFROM CODES",0,cashfromavl))
		updatersrc(mainrsrc,"CASHFROM CODES");
	if(cashtoavl->numlibs<1)
	{
		addAPPlib(cashtoavl,"No Accounts Available");
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"CASHTO CODES",0,cashtoavl))
		updatersrc(mainrsrc,"CASHTO CODES");
	getcashfromdesc();
	getcashtodesc();
}
void getjournals()
{
	short ef;
	char *journal=NULL;

	if(journalavl!=NULL) freeapplib(journalavl);
	journalavl=APPlibNEW();
	ZERNRD(jrnlnum);
	ef=ADVFRSNRDsec(jrnlnum,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(jrnlnum,"JOURNAL IDENTIFICATION",&journal);
		if(journal!=NULL)
		{
			addAPPlib(journalavl,journal);
			Rfree(journal);
			journal=NULL;
		}
		ef=ADVNXTNRDsec(jrnlnum,1,SCRNvirtualSubData,mainrsrc);
	}
}
void getyears()
{
	short ef;
	int year=0;

	if(yearavl!=NULL) freeapplib(yearavl);
	yearavl=APPlibNEW();
	ZERNRD(yearnum);
	ef=ADVFRSNRDsec(yearnum,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETINT(yearnum,"FISCAL YEAR",&year);
		sprintf(stemp,"%-4d",year);
		addAPPlib(yearavl,stemp);
		ef=ADVNXTNRDsec(yearnum,1,SCRNvirtualSubData,mainrsrc);
	}
}
void main(int argc,char **argv)
{
	short engine=0,nofields=0,nokeys=0,x,y,edit_rsrc=TRUE,ef;
	short f=0,cur=1,length=0,lcount=0,accttype=0;
	int start=0;
	NRDfield *fields=NULL,*fldx,*field;
	double amount=0;
	RDArmem *len_member,*pos_member,*member;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	char *name=NULL,*username=NULL,deleteflag=FALSE,verified=FALSE;
	char *s,*hold=NULL,*temp=NULL,*acct=NULL;

	if(InitializeSubsystems(argc,argv,module,"ADD INTRA BANK TRANS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=OPNNRDsec(module,"FINREC",TRUE,TRUE,FALSE))==(-1))
	{
		ERRORDIALOG("Security Access Denied","Security Access denied to the [FINREC] file or the file cannot otherwise be opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		quit_record();
		return;
	}
	if((byrnum=OPNNRDsec(module,"FINBYR",TRUE,TRUE,FALSE))==(-1)) 
	{
		ERRORDIALOG("Security Access Denied","Security Access denied to the [FINBYR] file or the file cannot otherwise be opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		quit_record();
		return;
	}
	if(GETBIN(MODULENAME(filenum),FILENAME(filenum),&engine,&nofields,&fields,&nokeys,&keys))
	{
		ERRORDIALOG("Database Binary Error","Cannot retrieve the database field and key information from the binary.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		quit_record();
		return;
	}
	mainrsrc=RDArsrcNEW(module,"ADD INTRA BANK TRANS");
	definelist=RDArsrcNEW(module,"ADD INTRA BANK TRANS DEFINE LIST");
	yearnum=OPNNRDsec(module,"FINYEAR",TRUE,FALSE,FALSE);
	jrnlnum=OPNNRDsec(module,"FINRJRNL",TRUE,FALSE,FALSE);
	balnum=OPNNRDsec(module,"FINBACC",TRUE,FALSE,FALSE);
	FINDRSCSETFUNC(definelist,"SELECT",savelist,NULL);
	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->name!=NULL) Rfree(keyx->name);
			if(keyx->part!=NULL)
			{
				for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
				{
					if(part->name!=NULL) Rfree(part->name);
				}
				Rfree(keyx->part);
			}
		}
		Rfree(keys);
		keys=NULL;
	}
	for(x=0,fldx=fields;x<nofields;++x,++fldx)
	{
		if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
		{
		} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
		{
		} else if(!RDAstrcmp(fldx->name,"ACCOUNT CODE"))
		{
		} else if(!RDAstrcmp(fldx->name,"DEBIT CODE"))
		{
		} else if(!RDAstrcmp(fldx->name,"CREDIT CODE"))
		{
		} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
		{
		} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
		{
		} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
		{
		} else if(!RDAstrcmp(fldx->name,"PROCESSING MONTH"))
		{
		} else {
			edit_rsrc=TRUE;
			if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
			{
				if(name!=NULL) 
				{ 
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+
						RDAstrlen(FILENAME(filenum)));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+
						RDAstrlen(FILENAME(filenum)));
				}
				sprintf(name,"[%s][%s]",FILENAME(filenum),fldx->name);
				makefieldrsrc(mainrsrc,name,fldx->type,
					fldx->len,edit_rsrc);
			}
		}
	}
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(fldx->name!=NULL) Rfree(fldx->name);
		}
		Rfree(fields);
		fields=NULL;
	}
	nofields=0;
	nokeys=0;
	file2rsrc(yearnum,mainrsrc,FALSE);
	file2rsrc(jrnlnum,mainrsrc,FALSE);
	getjournals();
	if(journalavl->numlibs<1)
	{
		ERRORDIALOG("FINRJRNL Empty File","The FINRJRNL file is an empty file.  There must exist at least 1 journal on file to process journal entries appropriately.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		quit_record();
		return;
	}
	getyears();
	if(yearavl->numlibs<1)
	{
		ERRORDIALOG("FINYEAR Empty File","The FINYEAR file is an empty file.  There must exist at least 1 year on file to process journal entries.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		quit_record();
		return;
	}
	addlstrsrc(mainrsrc,"YEARS AVAILABLE",&start,TRUE,changeyear,
		yearavl->numlibs,&yearavl->libs,NULL);
	addlstrsrc(mainrsrc,"JOURNALS AVAILABLE",&start,TRUE,changejournal,
		journalavl->numlibs,&journalavl->libs,NULL);
	DEFtypes=APPlibNEW();
	addlstrsrc(mainrsrc,"DEFINITION TYPES",&start,TRUE,changeDEFtype,
		DEFtypes->numlibs,&DEFtypes->libs,NULL);
	cashfromavl=APPlibNEW();
	addlstrsrc(mainrsrc,"CASHFROM CODES",&start,TRUE,getcashfromdesc
		,cashfromavl->numlibs,&cashfromavl->libs,NULL);
	cashtoavl=APPlibNEW();
	addlstrsrc(mainrsrc,"CASHTO CODES",&start,TRUE,getcashtodesc
		,cashtoavl->numlibs,&cashtoavl->libs,NULL);
	pmonth=APPlibNEW();
	addAPPlib(pmonth,"PRE-YEAR");
	addAPPlib(pmonth,"JANUARY");
	addAPPlib(pmonth,"FEBRUARY");
	addAPPlib(pmonth,"MARCH");
	addAPPlib(pmonth,"APRIL");
	addAPPlib(pmonth,"MAY");
	addAPPlib(pmonth,"JUNE");
	addAPPlib(pmonth,"JULY");
	addAPPlib(pmonth,"AUGUST");
	addAPPlib(pmonth,"SEPTEMBER");
	addAPPlib(pmonth,"OCTOBER");
	addAPPlib(pmonth,"NOVEMBER");
	addAPPlib(pmonth,"DECEMBER");
	addAPPlib(pmonth,"POST YEAR");
	start=Ratoi(CURRENT_DATE);
	addlstrsrc(mainrsrc,"PROCESSING MONTHS",&start,TRUE,NULL,pmonth->numlibs,&pmonth->libs,NULL);
	addstdrsrc(mainrsrc,"CASHFROM DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(mainrsrc,"CASHTO DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	changejournal();
	changeyear();
	setDEFtype();
	changeDEFtype();
	addbtnrsrc(mainrsrc,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addrfcbrsrc(mainrsrc,"SAVE",TRUE,save_record,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quit_record,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,mainrsrc);
	if(prevtrans!=NULL) freeapplib(prevtrans);
	prevtrans=APPlibNEW();
	ef=ADVFRSNRDsec(filenum,1,SCRNvirualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETCHAR(filenum,"SOURCE USER VERIFIED",&verified);
			if(!verified)
			{
				FINDFLDGETSTRING(filenum,"SOURCE USER",&username);
				if(!RDAstrcmp(username,USERLOGIN))
				{
					for(x=1,length=0;x<(definelist->numrscs-4);x+=2)
					{
						pos_member=definelist->rscs+x;
						len_member=definelist->rscs+(x+1);
						if(*pos_member->value.short_value>0) 
						{
							length+=(*len_member->value.short_value+2);
						}
					}
					hold=Rmalloc(length+1);
					lcount=0;
					cur=0;
					while(cur<(definelist->numrscs+1))
					{
						for(x=1;x<(definelist->numrscs-4);x+=2)
						{
							pos_member=definelist->rscs+x;
							len_member=definelist->rscs+(x+1);
							if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
							{
								if(*pos_member->value.short_value==cur)
								{
									s=stralloc(pos_member->rscrname);
									s[RDAstrlen(s)-9]=0;
									field=FLDNRD(filenum,s);
									if(s!=NULL) Rfree(s);
									if(field!=NULL)
									{
										switch(field->type)
										{
											case SCROLLEDTEXT:
											case VARIABLETEXT:
											case PLAINTEXT:
											case PHONE:
											case CUSTOMTYPE:
											case ZIPCODE:
											case SOCSECNUM:
											case DATES:
											case TIMES:
											case EXPENDITURE:
											case BALANCESHEET:
											case REVENUE:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
												if(*len_member->value.short_value<RDAstrlen(field->data.string_value))
												{
													sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,field->data.string_value);
												} else {
													sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,field->data.string_value);
												}
												break;
											case DECIMALV:
											case SDECIMALV:
												sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,*field->data.float_value);
												break;
											case SHORTV:
											case SSHORTV:
												sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.short_value);
												break;
											case LONGV:
											case SLONGV:
												sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.integer_value);
												break;
											case DOLLARS:
												temp=famt(*field->data.float_value,*len_member->value.short_value);
												sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
												if(temp!=NULL) Rfree(temp);
												break;
											case DOUBLEV:
											case SDOUBLEV:
												sprintf(&hold[lcount],"%*.0f ",*len_member->value.short_value,*field->data.float_value);
												break;
											case BOOLNS:
												sprintf(&hold[lcount],"%c ",(*field->data.string_value ? 'Y':'N'));
												break;
											case CHARACTERS:
												sprintf(&hold[lcount],"%c ",*field->data.string_value);
												break;
											case BUTTONS:
											case SCROLLEDLIST:
												prterr("Huh? Do a %s?",standardfieldtypes[field->type]);
												break;
										}
										lcount+=*len_member->value.short_value+1;
									} else {
											prterr("Error field [%s] not found.");
									}
								}
							}
						}
						++cur;
					}
					addAPPlib(prevtrans,hold);
					if(hold!=NULL) Rfree(hold);
					FINDFLDGETDOUBLE(filenum,"AMOUNT",&amount);
				}
			}
		}
		ef=ADVNXTNRDsec(filenum,1,SCRNvirtualSubData,mainrsrc);
	}
	if(prevtrans->numlibs<1)
	{
		addAPPlib(prevtrans,"No Bank Transfers added this session");
	}
	if(name!=NULL) Rfree(name);
	addlstrsrc(mainrsrc,"PREVIOUS BANK TRANSFERS",&x,TRUE,
		NULL,prevtrans->numlibs,&prevtrans->libs,NULL);
	mk_intra_define_list(filenum);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,mainrsrc);
	DefaultScreens(mainrsrc);
	if(makescrn(mainrsrc,FALSE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen ADD INTRA BANK TRANS.  Check to see the screen is available. If it is, call your installer.",quit_record,TRUE);
		quit_record();
	}
	RDAAPPMAINLOOP();
}
