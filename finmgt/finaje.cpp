/* finaje.c - Financial Management Add Jounral Entries */
#define CLEARWDGT
#ifndef WIN32
#define __NAM__ "finaje.lnx"
#endif
#ifdef WIN32
#define __NAM__ "finaje.exe"
#endif
#include <app.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <fin.h>
#include <addlists.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL;
static short filenum=0,yearnum=0,expnum=0,revnum=0,balnum=0,jrnlnum=0;
static short byrnum=0,eyrnum=0,ryrnum=0;
static RDAfinmgt *finsetup=NULL;
static RDArsrc *definelist=NULL;
static APPlib *prevjes=NULL,*ACCTtypes=NULL,*DEFtypes=NULL;
static APPlib *yearavl=NULL,*acctavl=NULL,*debitavl=NULL,*creditavl=NULL;
static APPlib *journalavl=NULL,*jtypes=NULL,*pmonth=NULL;
static char *module="FINMGT";
static double total_exp=0,total_rev=0,total_deb=0,total_cred=0;
static int counter=0;
static short acctcodenum=0,debitcodenum=1,creditcodenum=2;
static int year_identification=(-1),journal_entry_type=(-1);
static int account_type=(-1),definition_type=(-1);
static void ACCTtypecb(RDArsrc *),DEFtypecb(RDArsrc *);
static void changeACCTtype(RDArsrc *);
static void changeDEFtype(RDArsrc *);
static void getacctdesc(RDArsrc *);
static void changeyear(RDArsrc *),setyear(RDArsrc *);
static int getyears(RDArsrc *,RDAfinmgt *);
static void changejournal(RDArsrc *),getjournals(RDArsrc *);
static void changeCODElist(RDArsrc *,short);
static void setjournal(RDArsrc *);
static void set_defaults(RDArsrc *);
static void changejtype(RDArsrc *);
static void savelist(RDArsrc *,RDArsrc *),savelistcb(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *),quit_record(RDArsrc *);
static void save_check(RDArsrc *),quit_check(RDArsrc *);
static void acctsearch(RDArsrc *,short *);
static void getsearchstr(RDArsrc *,short *);
static void clearwidget(RDArsrc *,char *);
static short check_acctcode(RDArsrc *,short);
static void setfiles(RDArsrc *);
static void getsupportingrecords(RDArsrc *,short);
char *reportname=NULL;

#ifdef SETJOURNALDEFS
static void setjournaldefs(RDArsrc *);
static void setACCTtype(RDArsrc *);
#endif

static void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,RDArsrc *parent)
{
	short ef=0,accttype=0;
	short seljtype=0;
	double amount=0.0,local_exp=0.0,local_rev=0.0;
	double local_deb=0.0,local_cred=0.0;
	char *username=NULL,deleteflag=FALSE,verified=FALSE;
	char *acct=NULL;

	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(prevjes!=NULL) freeapplib(prevjes);
	prevjes=APPlibNEW();
	ef=ADVFRSNRDsec(filenum,1,SCRNvirtualSubData,parent);
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
					makedflist();
					FINDFLDGETSHORT(filenum,"ACCOUNT TYPE",&accttype);
					FINDFLDGETDOUBLE(filenum,"AMOUNT",&amount);
					switch(accttype)
					{
						case EXP_ACCT:
							local_exp+=amount;
							FINDFLDGETSTRING(filenum,"DEBIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								local_deb+=amount;
								Rfree(acct);
							}
							FINDFLDGETSTRING(filenum,"CREDIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								local_cred+=amount;
								Rfree(acct);
							}
							break;
						case REV_ACCT:
							local_rev+=amount;
							FINDFLDGETSTRING(filenum,"DEBIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								local_deb+=amount;
								Rfree(acct);
							}
							FINDFLDGETSTRING(filenum,"CREDIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								local_cred+=amount;
								Rfree(acct);
							}
							break;
						case BAL_ACCT:
							FINDFLDGETSHORT(filenum,"JOURNAL ENTRY TYPE",&seljtype);
							if(seljtype==0)
							{
								if(amount>0)
									local_deb+=amount;
								else local_cred-=amount;
							} else {
								if(amount>0)
									local_cred+=amount;
								else local_deb-=amount;
							}
							break;
						default:
							prterr("Invalid Account Type [%d]",accttype);
							break;
					}
				}
			}
		}
		ef=ADVNXTNRDsec(filenum,1,SCRNvirtualSubData,parent);
	}
	total_exp=local_exp;
	total_rev=local_rev;
	total_deb=local_deb;
	total_cred=local_cred;
	if(prevjes->numlibs<1)
	{
		addAPPlib(prevjes,"No Journal Entries added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS J/E'S",(prevjes->numlibs-1),prevjes);
	updatersrc(parent,"PREVIOUS J/E'S");

}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;
	char strtemp[1024];

	if(prevjes!=NULL)
	{
		if(prevjes->numlibs==1 && !RDAstrcmp(prevjes->libs[0],
			"No Journal Entries added this session"))
		{
			freeapplib(prevjes);
			prevjes=APPlibNEW();
		}
	} else {
		prevjes=APPlibNEW();
	}
	for(x=1,length=0;x<(definelist->numrscs-5);x+=2)
	{
		pos_member=definelist->rscs+x;
		len_member=definelist->rscs+(x+1);
		if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
		{
			length+=(*len_member->value.short_value+1);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(definelist->numrscs-5))
	{
		for(x=1;x<(definelist->numrscs-5);x+=2)
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
								memset(strtemp,0,1024);
								sprintf(strtemp,"%-*f",*len_member->value.short_value,*field->data.float_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,*field->data.float_value);
*/

								break;
							case SDECIMALV:
								memset(strtemp,0,1024);
								sprintf(strtemp,"%- *f",*len_member->value.short_value,*field->data.float_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,*field->data.float_value);
*/

								break;
							case SHORTV:
								memset(strtemp,0,1024);
								sprintf(strtemp,"%-*d",*len_member->value.short_value,*field->data.short_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.short_value);
*/
								break;
							case SSHORTV:
								memset(strtemp,0,1024);
								sprintf(strtemp,"%- *d",*len_member->value.short_value,*field->data.short_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.short_value);
*/
								break;
							case LONGV:
								memset(strtemp,0,1024);
								sprintf(strtemp,"%-*d",*len_member->value.short_value,*field->data.integer_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.integer_value);
*/
								break;
							case SLONGV:
								memset(strtemp,0,1024);
								sprintf(strtemp,"%- *d",*len_member->value.short_value,*field->data.integer_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.integer_value);
*/
								break;
							case DOLLARS:
								temp=famt(*field->data.float_value,*len_member->value.short_value);
								sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
								if(temp!=NULL) Rfree(temp);
								break;
							case DOUBLEV:
								memset(strtemp,0,1024);
								sprintf(strtemp,"%-*.0f",*len_member->value.short_value,*field->data.float_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*.0f ",*len_member->value.short_value,*field->data.float_value);
*/
								break;
							case SDOUBLEV:
								memset(strtemp,0,1024);
								sprintf(strtemp,"%- *.0f",*len_member->value.short_value,*field->data.float_value);
								strtemp[*len_member->value.short_value]=0;
								sprintf(&hold[lcount],"%s ",strtemp);
/*
								sprintf(&hold[lcount],"%*.0f ",*len_member->value.short_value,*field->data.float_value);
*/
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
							default:
								prterr("Invalid Field Type [%d]",field->type);
								break;
						}
						lcount+=*len_member->value.short_value+1;
					} else {
						prterr("Error field [%s] not found.",s);
					}
					if(s!=NULL) Rfree(s);
				}
			}
		}
		++cur;
	}
	addAPPlib(prevjes,hold);
	++counter;
	if(hold!=NULL) Rfree(hold);
}
static void definelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void set_defaults(RDArsrc *parent)
{
	int selprev;

	FINDRSCGETINT(parent,"PREVIOUS J/E'S",&selprev);
	DefaultResetScreen(parent,TRUE);
	FINDRSCSETDOUBLE(parent,"TOTAL EXPENDITURE",total_exp);
	FINDRSCSETDOUBLE(parent,"TOTAL REVENUE",total_rev);
	FINDRSCSETDOUBLE(parent,"TOTAL DEBITS",total_deb);
	FINDRSCSETDOUBLE(parent,"TOTAL CREDITS",total_cred);
	changejournal(parent);
	changeACCTtype(parent);
	changeyear(parent);
	changeDEFtype(parent);
	changejtype(parent);
	getacctdesc(parent);
	debitcodenum=1;
	getsearchstr(parent,&debitcodenum);
	creditcodenum=2;
	getsearchstr(parent,&creditcodenum); 
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void clearwidget(RDArsrc *parent,char *widgetname)
{
#ifdef CLEARWDGT

	FINDRSCSETSTRING(parent,widgetname,NULL);
	updatersrc(parent,widgetname);
#endif
}
static short check_acctcode(RDArsrc *parent,short listtype)
{
	char *tmp=NULL,*baltype=NULL;
	short tempyrfile=0,tempaccfile=0,deftype1=0;
	char active=FALSE,delflag=FALSE;
	int selectedd=0,selecteda=0,selyear=0,yearid=0,seljtype=0;

	FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	deftype1=(short)selectedd;
	FINDRSCGETINT(parent,"YEARS AVAILABLE",&selyear);
	yearid=Ratoi(yearavl->libs[selyear]);
	switch(selecteda)
	{
		case EXP_ACCT:
			tempyrfile=eyrnum;
			tempaccfile=expnum;
			break;
		case REV_ACCT:
			tempyrfile=ryrnum;
			tempaccfile=revnum;
			break;
		case BAL_ACCT:
			tempyrfile=byrnum;
			tempaccfile=balnum;
			break;
		default:
			prterr("Invalid Account Type [%d]",selecteda);
			break;
	}
	switch(listtype)
	{
		case 0:	/* Account Credit Code */
			if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
			FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",tmp);
			updatersrc(parent,"ACCOUNT DESCRIPTION");
			readwidget(parent,"ACCOUNT CODE SEARCH");
			FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
			ZERNRD(tempaccfile);
			FINDFLDSETSHORT(tempaccfile,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(tempaccfile,"ACCOUNT CODE",acctcode1);
			if(acctcode1==NULL || RDAstrlen(acctcode1)<1)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","NO ACCOUNT CODE!","No ACCOUNT CODE was entered.",NULL,NULL,TRUE,NULL);
				prterr("No Account Code was entered.");
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				return(FALSE);
			}
			if(ADVEQLNRDsec(tempaccfile,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(acctcode1)+220);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED ACCOUNT CODE.  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",(acctcode1==NULL?"":acctcode1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(tmp!=NULL) Rfree(tmp);
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				return(FALSE);
			}
			FINDFLDGETCHAR(tempaccfile,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(acctcode1)+115);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",(acctcode1==NULL?"":acctcode1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			} 
			FINDFLDGETCHAR(tempaccfile,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(acctcode1)+70);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE ACCOUNT CODE.",(acctcode1==NULL?"":acctcode1)); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			ZERNRD(tempyrfile);
			FINDFLDSETSHORT(tempyrfile,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(tempyrfile,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(tempyrfile,"ACCOUNT CODE",acctcode1);
			if(ADVEQLNRDsec(tempyrfile,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(acctcode1)+234+4+1);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",(acctcode1==NULL?"":acctcode1),yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(acctcode1)+139+4+1);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if this account should not be deleted.",(acctcode1==NULL?"":acctcode1),yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(acctcode1)+86+4+1);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE YEAR ACCOUNT CODE for the YEAR [%d].",(acctcode1==NULL?"":acctcode1),yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			getacctdesc(parent);
			break;
		case 1:	/* Check Debit Code */
			if(debcode1!=NULL) Rfree(debcode1);
			readwidget(parent,"DEBIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
			if(selecteda==REV_ACCT && isEMPTY(debcode1))
			{
				if(debcode1!=NULL) Rfree(debcode1);
				break;
			}
			FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&seljtype);
			readwidget(parent,"JOURNAL ENTRY TYPES");
			if(selecteda==BAL_ACCT || (selecteda==EXP_ACCT && isEMPTY(debcode1)))
			{
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				return(FALSE);
			}
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",debcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(debcode1)+168+4+1);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",(debcode1==NULL?"":debcode1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(debcode1)+122+1);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",(debcode1==NULL?"":debcode1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(debcode1)+88+1);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",(debcode1==NULL?"":debcode1)); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",debcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(debcode1)+280);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",(debcode1==NULL?"":debcode1),yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(debcode1)+153);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",(debcode1==NULL?"":debcode1),yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(debcode1)+140+4+1);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",(debcode1==NULL?"":debcode1),yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(selecteda==REV_ACCT)
			{
				if(debcode1==NULL || RDAstrlen(debcode1)<1)
				{
					clearwidget(parent,"DEBIT CODE SEARCH");
					clearwidget(parent,"DEBIT CODE DESCRIPTION");
					if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
					return(FALSE);
				} else if(seljtype==1)
				{
					if(RDAstrcmp(baltype,"ESTIMATED REVENUE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ESTIMATED REVENUE] for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE.",NULL,NULL,TRUE,NULL);
						prterr("The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ESTIMATED REVENUE] for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE.");
						clearwidget(parent,"DEBIT CODE SEARCH");
						clearwidget(parent,"DEBIT CODE DESCRIPTION");
						if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
						return(FALSE);
					}
				}
			} else if(selecteda==EXP_ACCT)
			{
				if(seljtype==0)
				{
					if(RDAstrcmp(baltype,"EXPENDITURE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [EXPENDITURE] for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
						prterr("The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE for the ACCOUNT TYPE EXPENDITURE.");
						clearwidget(parent,"DEBIT CODE SEARCH");
						clearwidget(parent,"DEBIT CODE DESCRIPTION");
						if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
						return(FALSE);
					} 
				} else if(seljtype==1)
				{
					if(RDAstrcmp(baltype,"UNRESERVED FUND BALANCE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [UNRESERVED FUND BALANCE] for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
						prterr("The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for the ACCOUNT TYPE EXPENDITURE.");
						clearwidget(parent,"DEBIT CODE SEARCH");
						clearwidget(parent,"DEBIT CODE DESCRIPTION");
						if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
						return(FALSE);
					} 
				} else if(seljtype==2)
				{
					if(RDAstrcmp(baltype,"ENCUMBRANCE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ENCUMBRANCE] for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
						prterr("The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ENCUMBRANCE for the ACCOUNT TYPE EXPENDITURE.");
						clearwidget(parent,"DEBIT CODE SEARCH");
						clearwidget(parent,"DEBIT CODE DESCRIPTION");
						if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
						return(FALSE);
					} 
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOURNAL ENTRY TYPE!","The JOURNAL ENTRY TYPE is not a valid JOURNAL ENTRY TYPE for the ACCOUNT TYPE of EXPENDITURE.",NULL,NULL,TRUE,NULL);
					prterr("The JOURNAL ENTRY TYPE is not a valid JOURNAL ENTRY TYPE for the ACCOUNT TYPE of REVENUE.");
					clearwidget(parent,"DEBIT CODE SEARCH");
					clearwidget(parent,"DEBIT CODE DESCRIPTION");
					if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
					return(FALSE);
				}
			}	
			break;
		case 2:	/* Check Credit Code */
			if(credcode1!=NULL) Rfree(credcode1);
			readwidget(parent,"CREDIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
			if(selecteda==EXP_ACCT && isEMPTY(credcode1))
			{
				if(credcode1!=NULL) Rfree(credcode1);
				break;
			}
			readwidget(parent,"JOURNAL ENTRY TYPES");
			FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&seljtype);
			if(selecteda==BAL_ACCT || (selecteda==REV_ACCT && isEMPTY(credcode1)))
			{
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
				return(FALSE);
			} 
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",credcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(credcode1)+163+1);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",(credcode1==NULL?"":credcode1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(credcode1)+114+1);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",(credcode1==NULL?"":credcode1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				prterr(tmp);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(credcode1)+83+1);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",(credcode1==NULL?"":credcode1)); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",credcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(credcode1)+255+4+1);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",(credcode1==NULL?"":credcode1),yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(credcode1)+155+4+1);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",(credcode1==NULL?"":credcode1),yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				tmp=Rmalloc(RDAstrlen(credcode1)+105+4+1);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",(credcode1==NULL?"":credcode1),yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
				if(tmp!=NULL) Rfree(tmp);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(selecteda==EXP_ACCT)
			{
				if(credcode1==NULL || RDAstrlen(credcode1)<1)
				{
					clearwidget(parent,"CREDIT CODE SEARCH");
					clearwidget(parent,"CREDIT CODE DESCRIPTION");
					if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
					return(FALSE);
				} else if(seljtype==1)
				{
					if(RDAstrcmp(baltype,"APPROPRIATION"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [APPROPRIATION] for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of APPROPRIATION.",NULL,NULL,TRUE,NULL);
						prterr("The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [UNRESERVED FUND BALANCE] for ACCOUNT TYPE of APPROPRIATION, JOURNAL ENTRY TYPE of APPROPRIATION.");
						clearwidget(parent,"CREDIT CODE SEARCH");
						clearwidget(parent,"CREDIT CODE DESCRIPTION");
						if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
						return(FALSE);
					}
				}
			} else if(selecteda==REV_ACCT)
			{
				if(seljtype==0)
				{
					if(RDAstrcmp(baltype,"REVENUE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [REVENUE] for the ACCOUNT TYPE REVENUE.",NULL,NULL,TRUE,NULL);
						prterr("The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of REVENUE for the ACCOUNT TYPE REVENUE.");
						clearwidget(parent,"CREDIT CODE SEARCH");
						clearwidget(parent,"CREDIT CODE DESCRIPTION");
						if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
						return(FALSE);
					} 
				} else if(seljtype==1)
				{
					if(RDAstrcmp(baltype,"UNRESERVED FUND BALANCE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [UNRESERVED FUND BALANCE] for the ACCOUNT TYPE REVENUE.",NULL,NULL,TRUE,NULL);
						prterr("The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for the ACCOUNT TYPE REVENUE.");
						clearwidget(parent,"CREDIT CODE SEARCH");
						clearwidget(parent,"CREDIT CODE DESCRIPTION");
						if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
						return(FALSE);
					} 
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOURNAL ENTRY TYPE!","The JOURNAL ENTRY TYPE is not a valid JOURNAL ENTRY TTYPE for the ACCOUNT TYPE of REVENUE.",NULL,NULL,TRUE,NULL);
					prterr("The JOURNAL ENTRY TYPE is not a valid JOURNAL ENTRY TTYPE for the ACCOUNT TYPE of REVENUE.");
					clearwidget(parent,"CREDIT CODE SEARCH");
					clearwidget(parent,"CREDIT CODE DESCRIPTION");
					if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
					return(FALSE);
				}
			}	
			break;
		default: 
			prterr("Invalid Account List [%d]",listtype);
			return(FALSE);
	}
	return(TRUE);
}
static void acctsearch(RDArsrc *parent,short *listtype)
{
	char *searchwidget=NULL,*listwidget=NULL,*selectedc=NULL;
	APPlib *holdapplib=NULL;
	int x=0;

	switch(*listtype)
	{
		case 0:	
			searchwidget=stralloc("ACCOUNT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("ACCOUNT CODES");
				holdapplib=acctavl;
			}
			break;
		case 1:	
			searchwidget=stralloc("DEBIT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEBIT CODES");
				holdapplib=debitavl;
			}
			break;
		case 2:	
			searchwidget=stralloc("CREDIT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("CREDIT CODES");
				holdapplib=creditavl;
			}
			break;
		default: 
			prterr("Invalid Acount List [%d]",*listtype);
			break;
	}
	if(searchwidget==NULL) return;
	readwidget(parent,searchwidget);
	if(selectedc!=NULL) { Rfree(selectedc); selectedc=NULL; }
	FINDRSCGETSTRING(parent,searchwidget,&selectedc);
	if(AddList==TRUE)
	{
		if(selectedc!=NULL)
		{
			if(RDAstrlen(selectedc)!=0)
			{
				for(x=0;x<holdapplib->numlibs;++x)
				{
					if(strncmp(selectedc,holdapplib->libs[x],RDAstrlen(selectedc))<=0)
					{
						break;
					} 
				}
				if(x>=holdapplib->numlibs) x=(holdapplib->numlibs-1);
			}
		}
		FINDRSCSETINT(parent,listwidget,x);
		updatersrc(parent,listwidget);
		if(*listtype>0)
		{
			FINDRSCSETSTRING(parent,searchwidget,NULL);
			updatersrc(parent,searchwidget);
			FINDRSCSETSTRING(parent,searchwidget,holdapplib->libs[x]);
			updatersrc(parent,searchwidget);
		}
/*
	} else if(check_acctcode(parent,*listtype)==FALSE) 
	{
		if(selectedc!=NULL) Rfree(selectedc);
		if(searchwidget!=NULL) Rfree(searchwidget);
		if(listwidget!=NULL) Rfree(listwidget);
		return;
*/
	}
	if(*listtype==0)
	{
		getacctdesc(parent);
	} else {
		getsearchstr(parent,listtype);
	}
	if(selectedc!=NULL) Rfree(selectedc);
	if(searchwidget!=NULL) Rfree(searchwidget);
	if(listwidget!=NULL) Rfree(listwidget);
}
static void getsearchstr(RDArsrc *parent,short *whichlist)
{
	int selacct=0,selectedd=0;
	short deftype=0;
	char *listwidget=NULL,*searchwidget=NULL;
	char *descript=NULL,*descriptwidget=NULL;
	char *accountcode=NULL;
	APPlib *holdapplib=NULL;

	switch(*whichlist)
	{
		case 1:	/* Debit Code List */
			if(AddList)
			{
				holdapplib=debitavl;
				listwidget=stralloc("DEBIT CODES");
			}
			searchwidget=stralloc("DEBIT CODE SEARCH");
			descriptwidget=stralloc("DEBIT CODE DESCRIPTION");
			break;
		case 2:	/*Credit Code List */
			if(AddList)
			{
				holdapplib=creditavl;
				listwidget=stralloc("CREDIT CODES");
			}
			searchwidget=stralloc("CREDIT CODE SEARCH");
			descriptwidget=stralloc("CREDIT CODE DESCRIPTION");
			break;
		default:
			prterr("Invalid Account List [%d]",*whichlist);
			return;
	}
	FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	if(AddList)
	{
		if(holdapplib==NULL)
		{
			if(listwidget!=NULL) Rfree(listwidget);
			if(searchwidget!=NULL) Rfree(searchwidget);
			if(descript!=NULL) Rfree(descript);
			if(descriptwidget!=NULL) Rfree(descriptwidget);
			updateSCRNvirtuals(parent);
			return;
		}
		FINDRSCGETINT(parent,listwidget,&selacct);
		if(selacct<holdapplib->numlibs)
		{
			if(RDAstrcmp(holdapplib->libs[selacct],"Not Applicable") &&
				RDAstrcmp(holdapplib->libs[selacct],"No Accounts Available"))
			{
				FINDRSCSETSTRING(parent,searchwidget,holdapplib->libs[selacct]);
				accountcode=stralloc(holdapplib->libs[selacct]);
				ZERNRD(balnum);
				deftype=(short)selectedd;
				FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype);
				FINDFLDSETSTRING(balnum,"ACCOUNT CODE",accountcode);
				if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent)) KEYNRD(balnum,1);
				FINDFLDGETSTRING(balnum,"DESCRIPTION",&descript);
				FINDRSCSETSTRING(parent,descriptwidget,descript);
			} else {
				FINDRSCSETSTRING(parent,searchwidget,NULL);
				FINDRSCSETSTRING(parent,descriptwidget,NULL);
			}
		} else {
			FINDRSCSETSTRING(parent,searchwidget,NULL);
			FINDRSCSETSTRING(parent,descriptwidget,NULL);
		}
	} else {
                FINDRSCGETSTRING(parent,searchwidget,&accountcode);
		ZERNRD(balnum);
		deftype=(short)selectedd;
		FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype);
		FINDFLDSETSTRING(balnum,"ACCOUNT CODE",accountcode);
		if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent)) KEYNRD(balnum,1);
		FINDFLDGETSTRING(balnum,"DESCRIPTION",&descript);
		FINDRSCSETSTRING(parent,descriptwidget,descript);
	}
	updatersrc(parent,descriptwidget);
	updatersrc(parent,searchwidget);
	if(listwidget!=NULL) Rfree(listwidget);
	if(searchwidget!=NULL) Rfree(searchwidget);
	if(descript!=NULL) Rfree(descript);
	if(descriptwidget!=NULL) Rfree(descriptwidget);
	if(accountcode!=NULL) Rfree(accountcode);
	updateSCRNvirtuals(parent);
}
static void getacctdesc(RDArsrc *parent)
{
	int selacct=0,accttype=0,deftype=0;
	short fileaccttype=0,filedeftype=0;
	char *descript=NULL;

	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"ACCOUNT CODES",&selacct);
		if(selacct<acctavl->numlibs)
		{
			if(RDAstrcmp(acctavl->libs[selacct],"No Accounts Available"))
			{
				descript=stralloc(acctavl->libs[selacct]);
				FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",acctavl->libs[selacct]);
			} else {
				FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
			}
		}
		updatersrc(parent,"ACCOUNT CODE SEARCH");
	} else {
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&descript);
	}
	FINDRSCGETINT(parent,"ACCOUNT TYPES",&accttype);
	fileaccttype=(short)accttype;
	FINDFLDSETSHORT(filenum,"ACCOUNT TYPE",fileaccttype);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&deftype);
	filedeftype=(short)deftype;
	switch(accttype)
	{
		case EXP_ACCT:
			ZERNRD(expnum);
			FINDFLDSETSHORT(expnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(expnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(expnum,1,SCRNvirtualSubData,parent)) KEYNRD(expnum,1);
			if(descript!=NULL) { Rfree(descript); descript=NULL; }
			FINDFLDGETSTRING(expnum,"DESCRIPTION",&descript);
			break;
		case REV_ACCT:
			ZERNRD(revnum);
			FINDFLDSETSHORT(revnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(revnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(revnum,1,SCRNvirtualSubData,parent)) KEYNRD(revnum,1);
			if(descript!=NULL) { Rfree(descript); descript=NULL; }
			FINDFLDGETSTRING(revnum,"DESCRIPTION",&descript);
			break;
		case BAL_ACCT:
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent)) KEYNRD(balnum,1);
			if(descript!=NULL) { Rfree(descript); descript=NULL; }
			FINDFLDGETSTRING(balnum,"DESCRIPTION",&descript);
			break;
		default:
			prterr("Invalid Account Type [%d]",accttype);
			break;
	}
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",descript);
	updatersrc(parent,"ACCOUNT DESCRIPTION");
	if(descript!=NULL) Rfree(descript);
	updateSCRNvirtuals(parent);
}
static void save_check(RDArsrc *parent)
{
	int selacct=0,seldeb=0,selcred=0,seldeftype=0,selaccttype=0;
	int selyear=0,seljtype=0,selmonth=0;
	short x=0,testapp=TRUE;
	char *acctstr=NULL,*header=NULL,*message=NULL;
	double amount=0.0;

	readallwidgets(parent);
	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selaccttype)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETINT(parent,"YEARS AVAILABLE",&selyear)) return;
	if(FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&seljtype)) return;
	if(FINDRSCGETINT(parent,"PROCESSING MONTHS",&selmonth)) return;
	if(acctcode1!=NULL) Rfree(acctcode1); 
	if(debcode1!=NULL) Rfree(debcode1); 
	if(credcode1!=NULL) Rfree(credcode1); 
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"ACCOUNT CODES",&selacct);
		if(RDAstrcmp(acctavl->libs[selacct],"No Accounts Available") &&
			RDAstrcmp(acctavl->libs[selacct],"Not Applicable"))
		{
			acctcode1=stralloc(acctavl->libs[selacct]);
		}
		FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",acctcode1);
		FINDRSCGETINT(parent,"DEBIT CODES",&seldeb);
		if(RDAstrcmp(debitavl->libs[seldeb],"No Accounts Available") &&
			RDAstrcmp(debitavl->libs[seldeb],"Not Applicable"))
		{
			debcode1=stralloc(debitavl->libs[seldeb]);
		}
		FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",debcode1);
		FINDRSCGETINT(parent,"CREDIT CODES",&selcred);
		if(RDAstrcmp(creditavl->libs[selcred],"No Accounts Available") &&
			RDAstrcmp(creditavl->libs[selcred],"Not Applicable"))
		{
			credcode1=stralloc(creditavl->libs[selcred]);
		}
		FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",credcode1);
	} else {
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
		FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
		FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
		if(check_acctcode(parent,0)==FALSE) return;
		if(selaccttype!=BAL_ACCT)
		{
			if(check_acctcode(parent,1)==FALSE) return;
			if(check_acctcode(parent,2)==FALSE) return;
		}
	}
	if(FINDRSCGETDOUBLE(parent,"[FINJEN][AMOUNT]",&amount)) return;
	if(amount==0.0) 
	{
		ERRORDIALOG("INVALID AMOUNT","This Journal Entry can not be saved with an AMOUNT of [$  0.00].  Please enter the correct amount for this journal entry.",NULL,FALSE);
		prterr("Attemped to save a journal entry with a zero dollar amount");
		return;
	}
	selyear=Ratoi(yearavl->libs[selyear]);
	switch(selaccttype)
	{
	  case EXP_ACCT:
		if(seljtype!=1)
		{
			ZERNRD(eyrnum);
			FINDFLDSETINT(eyrnum,"FISCAL YEAR",selyear);
			x=(short)seldeftype;
			FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",x);
 			FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acctcode1);
			if(!ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,parent))
			{
				if(finsetup->budget==ANNUALBUDGET) 
				{ 
					testapp=testapprotot(eyrnum,amount,NULL);
				} else {
					testapp=testapprotot(eyrnum,amount,pmonth->libs[selmonth]);
				}
				if(testapp==FALSE)
				{
               				FINDFLDGETSHORT(eyrnum,"DEFINITION TYPE",&x);
               				FINDFLDGETSTRING(eyrnum,"ACCOUNT CODE",&acctstr);
               				FINDFLDGETINT(eyrnum,"FISCAL YEAR",&selyear);
               				header=stralloc("This Journal Entry will cause the POSTED APPROPRIATED amount to be less than all of the EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT [%s] in the YEAR [%d].");
               				message=Rrealloc(message,RDAstrlen(header)+RDAstrlen(acctstr)+6);
               				sprintf(message,header,acctstr,selyear);
               				message[RDAstrlen(header)+RDAstrlen(acctstr)+6-1]=0;
					WARNINGDIALOG("WARNING DIALOG SCREEN","APPROPRIATIONS NOT ENOUGH FOR TRANSACTION",message,save_record,NULL,FALSE,parent);
               				if(header!=NULL) Rfree(header);
               				if(message!=NULL) Rfree(message);
               				if(acctstr!=NULL) Rfree(acctstr);
				} else {
					save_record(parent);
				}
			}
		} else {
			save_record(parent);
		}
		break;
	    case REV_ACCT:
	    case BAL_ACCT:
		save_record(parent);
		break;
	    default:
		prterr("Invalid Account Type [%d]",selaccttype);
		break;
	}
}
static void save_record(RDArsrc *parent)
{
	int selacct=0,seldebit=0,selcredit=0,seldeftype=0,selaccttype=0;
	int seljrnl=0,selyear=0,tempseljtype=0,seljtype=0,selmonth=0;
	char *rcddesc=NULL,*errordesc=NULL,deleteflag=FALSE,*acct=NULL;
	char *type=NULL;
	short x=0,deftype1=0,accttype=0;
	int lf=0;
	char *date=NULL,*timex=NULL;
	double amount=0.0;
	RDArmem *member=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selaccttype)) return;
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"ACCOUNT CODES",&selacct);
		acctcode1=stralloc(acctavl->libs[selacct]);
		FINDRSCGETINT(parent,"DEBIT CODES",&seldebit);
		debcode1=stralloc(debitavl->libs[seldebit]);
		FINDRSCGETINT(parent,"CREDIT CODES",&selcredit);
		credcode1=stralloc(creditavl->libs[selcredit]);
	} else {
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
		FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
		FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
	}
	if(FINDRSCGETINT(parent,"JOURNALS AVAILABLE",&seljrnl)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETINT(parent,"YEARS AVAILABLE",&selyear)) return;
	if(FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&seljtype)) return;
	if(FINDRSCGETINT(parent,"PROCESSING MONTHS",&selmonth)) return;
	if(FINDRSCGETDOUBLE(parent,"[FINJEN][AMOUNT]",&amount)) return;
	rsrc2filerecord(filenum,parent);
	selyear=Ratoi(yearavl->libs[selyear]);
	FINDFLDSETINT(filenum,"FISCAL YEAR",selyear);
	FINDFLDSETSTRING(filenum,"JOURNAL IDENTIFICATION",journalavl->libs[seljrnl]);
	FINDFLDSETSTRING(filenum,"ACCOUNT CODE",acctcode1);
	x=(short)seljtype;
	FINDFLDSETSHORT(filenum,"JOURNAL ENTRY TYPE",x);
	x=(short)selaccttype;
	FINDFLDSETSHORT(filenum,"ACCOUNT TYPE",x);
	deftype1=(short)seldeftype;
	FINDFLDSETSHORT(filenum,"DEFINITION TYPE",deftype1);
	x=(short)selmonth;
	FINDFLDSETSHORT(filenum,"PROCESSING MONTH",x);
	if(!RDAstrcmp(debcode1,"Not Applicable") || !RDAstrcmp(debcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(filenum,"DEBIT CODE",NULL);
		Rfree(debcode1); debcode1=NULL;
	} else {
		FINDFLDSETSTRING(filenum,"DEBIT CODE",debcode1);
	}
	if(!RDAstrcmp(credcode1,"Not Applicable") || !RDAstrcmp(credcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(filenum,"CREDIT CODE",NULL);
		Rfree(credcode1); credcode1=NULL;
	} else {
		FINDFLDSETSTRING(filenum,"CREDIT CODE",credcode1);
	}
	FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
	deleteflag=FALSE;
	FINDFLDSETCHAR(filenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(filenum,"SOURCE USER VERIFIED",deleteflag);
	if((x=FINDRSC(parent,"[FINJEN][ENTRY DATE]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			date=GETCURRENTDATE();
			lf=RDAstrlen(date);
			if(date!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,date,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
		}
	} else {
		date=GETCURRENTDATE();
		FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date);
	if((x=FINDRSC(parent,"[FINJEN][ENTRY TIME]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			timex=GETCURRENTTIME();
			lf=RDAstrlen(timex);
			if(timex!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,timex,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
		}
	} else {
		timex=GETCURRENTTIME();
		FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
	}
	if(timex!=NULL) Rfree(timex);
	if(ADVRECORDsec(filenum,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(filenum,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  RECORDsec() failed without a description returned!");
		}
	} else {
		if(!WRTTRANS(filenum,1,"TRANSACTION NUMBER",NULL))
		{
			switch(selaccttype)
			{
				case EXP_ACCT:
					total_exp+=amount;
					FINDFLDGETSTRING(filenum,"DEBIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_deb+=amount;
						Rfree(acct);
					}
					FINDFLDGETSTRING(filenum,"CREDIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_cred+=amount;
						Rfree(acct);
					}
					break;
				case REV_ACCT:
					total_rev+=amount;
					FINDFLDGETSTRING(filenum,"CREDIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_cred+=amount;
						Rfree(acct);
					}
					FINDFLDGETSTRING(filenum,"DEBIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_deb+=amount;
						Rfree(acct);
					}
					break;
				case BAL_ACCT:
					if(seljtype==0)
					{
						if(amount>0)
							total_deb+=amount;
						else total_cred-=amount;
					} else {
						if(amount>0)
							total_cred+=amount;
						else total_deb-=amount;
					}
					break;
				default:
					prterr("Error Invalid Account Type [%d]",accttype);
					break;
			}
			FINDRSCSETDOUBLE(parent,"TOTAL EXPENDITURE",total_exp);
			FINDRSCSETDOUBLE(parent,"TOTAL REVENUE",total_rev);
			FINDRSCSETDOUBLE(parent,"TOTAL DEBITS",total_deb);
			FINDRSCSETDOUBLE(parent,"TOTAL CREDITS",total_cred);
			makedflist();
			switch(selaccttype)
			{
				case EXP_ACCT:
					switch(seljtype)
					{
						case(1) :
							type=stralloc("APPROPRIATIONS");
							break;
						case(2) :
							type=stralloc("ENCUMBRANCES");
							break;
						case(0) : 
						default :
							type=stralloc("EXPENDITURES");
							break;
					}
					if(credcode1!=NULL && RDAstrlen(credcode1)>0)
					{
					if(amount>0.0)
					{
						ADVupdateundist(filenum,"ACCOUNT CODE","DEBIT CODE",
							"CREDIT CODE",NULL,eyrnum,byrnum,selyear,deftype1,
							type,pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
					} else {
						ADVupdateundist(filenum,"ACCOUNT CODE",NULL,
							NULL,NULL,eyrnum,byrnum,selyear,deftype1,
							type,pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
						ADVupdateundist(filenum,NULL,"CREDIT CODE",
							"DEBIT CODE",NULL,eyrnum,byrnum,selyear,deftype1,
							type,pmonth->libs[selmonth],-amount,
							SCRNvirtualSubData,parent);
					}
					} else {
					if(amount>0.0)
					{
						ADVupdateundist(filenum,"ACCOUNT CODE","DEBIT CODE",
							NULL,NULL,eyrnum,byrnum,selyear,deftype1,type,
							pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
					} else {
						ADVupdateundist(filenum,"ACCOUNT CODE",NULL,
							NULL,NULL,eyrnum,byrnum,selyear,deftype1,type,
							pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
						ADVupdateundist(filenum,NULL,NULL,
							"DEBIT CODE",NULL,eyrnum,byrnum,selyear,deftype1,type,
							pmonth->libs[selmonth],-amount,
							SCRNvirtualSubData,parent);
					}
					}
					break;
				case REV_ACCT:
					switch(seljtype)
					{
						case(1) :
							type=stralloc("ESTIMATED REVENUE");
							break;
						case(0) : 
						default :
							type=stralloc("REVENUE");
							break;
					}
					if(debcode1!=NULL && RDAstrlen(debcode1)>0)
					{
					if(amount>0.0)
					{
						ADVupdateundist(filenum,"ACCOUNT CODE","DEBIT CODE",
							"CREDIT CODE",NULL,ryrnum,byrnum,selyear,deftype1,
							type,pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
					} else {
						ADVupdateundist(filenum,"ACCOUNT CODE",NULL,
							NULL,NULL,ryrnum,byrnum,selyear,deftype1,
							type,pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
						ADVupdateundist(filenum,NULL,"CREDIT CODE",
							"DEBIT CODE",NULL,ryrnum,byrnum,selyear,deftype1,
							type,pmonth->libs[selmonth],-amount,
							SCRNvirtualSubData,parent);
					}
					} else {
					if(amount>0.0)
					{
						ADVupdateundist(filenum,"ACCOUNT CODE",NULL,
							"CREDIT CODE",NULL,ryrnum,byrnum,selyear,
							deftype1,type,pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
					} else {
						ADVupdateundist(filenum,"ACCOUNT CODE",NULL,
							NULL,NULL,ryrnum,byrnum,selyear,
							deftype1,type,pmonth->libs[selmonth],amount,
							SCRNvirtualSubData,parent);
						ADVupdateundist(filenum,NULL,"CREDIT CODE",
							NULL,NULL,ryrnum,byrnum,selyear,
							deftype1,type,pmonth->libs[selmonth],-amount,
							SCRNvirtualSubData,parent);
					}
					}
					break;
				case BAL_ACCT:
					if(amount>0.0)
					{
						if(seljtype==1)
						{
							ADVupdateundist(filenum,"ACCOUNT CODE",NULL,NULL,
								NULL,byrnum,(-1),selyear,deftype1,"CREDITS",
								pmonth->libs[selmonth],amount,
								SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(filenum,"ACCOUNT CODE",NULL,NULL,
								NULL,byrnum,(-1),selyear,deftype1,"DEBITS",
								pmonth->libs[selmonth],amount,
								SCRNvirtualSubData,parent);
						}
					} else {
						if(seljtype==1)
						{
							ADVupdateundist(filenum,"ACCOUNT CODE",NULL,NULL,
								NULL,byrnum,(-1),selyear,deftype1,"DEBITS",
								pmonth->libs[selmonth],-amount,
								SCRNvirtualSubData,parent);
							tempseljtype=seljtype+1;
							x=(short)tempseljtype;
							FINDFLDSETSHORT(filenum,"JOURNAL ENTRY TYPE",x);
						} else {
							ADVupdateundist(filenum,"ACCOUNT CODE",NULL,NULL,
								NULL,byrnum,(-1),selyear,deftype1,"CREDITS",
								pmonth->libs[selmonth],-amount,
								SCRNvirtualSubData,parent);
							tempseljtype=seljtype-1;
							x=(short)tempseljtype;
							FINDFLDSETSHORT(filenum,"JOURNAL ENTRY TYPE",x);
						}
					}
					break;
				default:
					prterr("Error Invalid Account Type [%d]",selaccttype);
					break;
			}
			if(type!=NULL) Rfree(type);
		} else {
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=Rmalloc(RDAstrlen(MODULENAME(filenum))+RDAstrlen(FILENAME(filenum))+139);
			sprintf(errordesc,"The write function failed to the file [%s][%s], the record and all subfiles that you attempted to save were not created.  Please try again.",MODULENAME(filenum),FILENAME(filenum));
			ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
			prterr("Error Couldn't Write Record: [%s]",errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		}
	}
	updatersrc(parent,"TOTAL EXPENDITURE");
	updatersrc(parent,"TOTAL REVENUE");
	updatersrc(parent,"TOTAL DEBITS");
	updatersrc(parent,"TOTAL CREDITS");
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS J/E'S",(prevjes->numlibs-1),
		prevjes))
	{
		updatersrc(parent,"PREVIOUS J/E'S");
	}
}
static void quit_check(RDArsrc *parent)
{
	if(total_deb!=total_cred)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","OUT OF BALANCE WARNING","Exiting ADD JOURNAL ENTRIES with the entries added this session will make the Balance Sheet Out of Balance.  The TOTAL DEBITS entered this session are not equivalent to the TOTAL CREDITS.",quit_record,NULL,FALSE,parent);
	} else {
		quit_record(parent);
	}
}
static void quit_record(RDArsrc *parent)
{
	APPlib *args=NULL;

	if(parent!=NULL) free_rsrc(parent); 
	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist); 
	}
	if(counter>0)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(expnum!=(-1)) CLSNRD(expnum);
	if(revnum!=(-1)) CLSNRD(revnum);
	if(balnum!=(-1)) CLSNRD(balnum);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(yearnum!=(-1)) CLSNRD(yearnum);
	if(jrnlnum!=(-1)) CLSNRD(jrnlnum);
	if(eyrnum!=(-1)) CLSNRD(eyrnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(ACCTtypes!=NULL) freeapplib(ACCTtypes);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(yearavl!=NULL) freeapplib(yearavl);
	if(acctavl!=NULL) freeapplib(acctavl); 
	if(debitavl!=NULL) freeapplib(debitavl);
	if(creditavl!=NULL) freeapplib(creditavl);
	if(jtypes!=NULL) freeapplib(jtypes); 
	if(journalavl!=NULL) freeapplib(journalavl);
	if(pmonth!=NULL) freeapplib(pmonth);
	if(prevjes!=NULL) freeapplib(prevjes);
	if(finsetup!=NULL) free_finmgt(finsetup);
	if(reportname!=NULL) Rfree(reportname);
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	ShutdownSubsystems();
	exit(0);
}
static void setyear(RDArsrc *parent)
{
	int yearid=0,yearno=0;
	char *descript=NULL;

	if(FINDRSCGETINT(parent,"YEARS AVAILABLE",&yearno)) return;
	yearid=Ratoi(yearavl->libs[yearno]);
	ZERNRD(yearnum);
	FINDFLDSETINT(yearnum,"FISCAL YEAR",yearid);
	FINDFLDSETINT(filenum,"FISCAL YEAR",yearid);
	if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,parent)) KEYNRD(yearnum,1);
	updatefilerecord2rsrc(yearnum,parent);
	if(descript!=NULL) Rfree(descript);
}
static void changeyear(RDArsrc *parent)
{
	int selecteda=0,selectedyear=0,yearid=0;

	FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(parent,"YEARS AVAILABLE",&selectedyear);
	if(selectedyear>=yearavl->numlibs) selectedyear=0;
	yearid=Ratoi(yearavl->libs[selectedyear]);
	if(yearid!=year_identification || selecteda!=account_type)
	{
		year_identification=(-1);
		account_type=(-1);
		definition_type=(-1);
		setyear(parent);
		changeDEFtype(parent);
		updateSCRNvirtuals(parent);
	}
}
#ifdef SETJOURNALDEFS
void setjournaldefs(RDArsrc *parent)
{
	int x=0;
	short deftype=0,defaccttype=0,defjtype=0;

	FINDFLDGETSHORT(jrnlnum,"DEFAULT ACCOUNT TYPE",&defaccttype);
	FINDFLDGETSHORT(jrnlnum,"DEFAULT DEFINITION TYPE",&deftype);
	FINDFLDGETSHORT(jrnlnum,"DEFAULT JOURNAL ENTRY TYPE",&defjtype);
	x=(int)defaccttype;
	FINDRSCSETINT(parent,"ACCOUNT TYPES",x);
	updatersrc(parent,"ACCOUNT TYPES");
	x=(int)deftype;
	FINDRSCSETINT(parent,"DEFINITION TYPES",x);
	updatersrc(parent,"DEFINITION TYPES");
	x=(int)defjtype;
	FINDRSCSETINT(parent,"JOURNAL ENTRY TYPES",x);
	updatersrc(parent,"JOURNAL ENTRY TYPES");
	setACCTtype(parent);
	if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
	FINDFLDGETSTRING(jrnlnum,"DEFAULT ACCOUNT CODE",&acctcode1);
	if(debcode1!=NULL) { Rfree(debcode1); debcode1=NULL; }
	FINDFLDGETSTRING(jrnlnum,"DEFAULT DEBIT CODE",&debcode1);
	if(credcode1!=NULL) { Rfree(credcode1); credcode1=NULL; }
	FINDFLDGETSTRING(jrnlnum,"DEFAULT CREDIT CODE",&credcode1);
	if(AddList==TRUE)
	{
		for(x=0;x<acctavl->numlibs;++x)
		{
			if(!RDAstrcmp(acctavl->libs[x],acctcode1)) break;
		}
		if(x>=acctavl->numlibs) x=0;
		if(!FINDRSCSETINT(parent,"ACCOUNT CODES",&))
		{
			updatersrc(parent,"ACCOUNT CODES");
		}
		for(x=0;x<debitavl->numlibs;++x) 
		{
			if(!RDAstrcmp(debitavl->libs[x],debcode1)) break;
		}
		if(x>=debitavl->numlibs) x=0;
		if(!FINDRSCSETINT(parent,"DEBIT CODES",x))
		{
			updatersrc(parent,"DEBIT CODES");
		}
		for(x=0;x<creditavl->numlibs;++x)
		{
			if(!RDAstrcmp(creditavl->libs[x],credcode1)) break;
		}
		if(x>=creditavl->numlibs) x=0;
		if(!FINDRSCSETINT(parent,"CREDIT CODES",x))
		{
			updatersrc(parent,"CREDIT CODES");
		}
	} else {
		FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",acctcode1);
		updatersrc(parent,"ACCOUNT CODE SEARCH");
		FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",debcode1);
		updatersrc(parent,"DEBIT CODE SEARCH");
		FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",credcode1);
		updatersrc(parent,"CREDIT CODE SEARCH");
	}
	debitcodenum=1;
	getsearchstr(parent,&debitcodenum);
	creditcodenum=2;
	getsearchstr(parent,&creditcodenum); 
	getacctdesc(parent);
}
#endif
void setjournal(RDArsrc *parent)
{
	int jrnalno=0;

	if(FINDRSCGETINT(parent,"JOURNALS AVAILABLE",&jrnalno)) return;
	ZERNRD(jrnlnum);
	FINDFLDSETSTRING(jrnlnum,"JOURNAL IDENTIFICATION",journalavl->libs[jrnalno]);
	FINDFLDSETSTRING(filenum,"JOURNAL IDENTIFICATION",journalavl->libs[jrnalno]);
	if(ADVEQLNRDsec(jrnlnum,1,SCRNvirtualSubData,parent)) KEYNRD(jrnlnum,1);
	updatefilerecord2rsrc(jrnlnum,parent);
	updateSCRNvirtuals(parent);
}
void changejournal(RDArsrc *parent)
{
	setjournal(parent);
#ifdef SETJOURNALDEFS
	setjournaldefs(parent);
#endif
	updateSCRNvirtuals(parent);
}
static void changeCODElist(RDArsrc *parent,short functype)
{
	int selecteda=0,selectedd=0,jtype=(-1),selyear=0,yearid=0;
	short ef=0,accttype=0,deftype=0,deftype1=0,tempyrfile=0,tempaccfile=0;
	short keyn=0,keynumber=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL,*e=NULL,*actyp=NULL,*speedkeyname=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
	if(FINDRSCGETINT(parent,"YEARS AVAILABLE",&selyear)) return;
	if(FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&jtype)) return;
	yearid=Ratoi(yearavl->libs[selyear]);
	selyear=yearid;
	accttype=(short)selecteda;
	deftype=(short)selectedd;		
	FINDRSCSETACCTTYPE(parent,"ACCOUNT CODE SEARCH",accttype);
	FINDRSCSETDEFTYPE(parent,"ACCOUNT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEBIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"CREDIT CODE SEARCH",deftype);
	if(AddList==TRUE)
	{
		if(yearid!=year_identification || selecteda!=account_type
			|| selectedd!=definition_type 
			|| jtype!=journal_entry_type)
		{
		if(functype==0)
		{
			if(acctavl!=NULL) freeapplib(acctavl);
			acctavl=APPlibNEW();
		}
		if(debitavl!=NULL) freeapplib(debitavl);
		debitavl=APPlibNEW();
		if(creditavl!=NULL) freeapplib(creditavl);
		creditavl=APPlibNEW();
		switch(selecteda)
		{
			case EXP_ACCT:
				tempyrfile=eyrnum;
				speedkeyname=stralloc("RDA FINEYR KEY");
				tempaccfile=expnum;
				actyp=stralloc("Expenditure");
				break;
			case REV_ACCT:
				tempyrfile=ryrnum;
				speedkeyname=stralloc("RDA FINRYR KEY");
				tempaccfile=revnum;
				actyp=stralloc("Revenue");
				break;
			case BAL_ACCT:
				tempyrfile=byrnum;
				speedkeyname=stralloc("RDA FINBYR KEY");
				tempaccfile=balnum;
				actyp=stralloc("Balance Sheet");
				break;
			default:
				prterr("Invalid Account Type [%d]",selecteda);
				break;
		}
/*  Skipping this for changing Journal Entry Type */
		if(!functype)
		{
			ZERNRD(tempyrfile);
			FINDFLDSETSHORT(tempyrfile,"DEFINITION TYPE",deftype);
			FINDFLDSETINT(tempyrfile,"FISCAL YEAR",selyear);
			if((keyn=KEYNUMBER(tempyrfile,speedkeyname))!=(-1))
			{
				keynumber=keyn;
			} else {
				keynumber=1;
			}
			ef=ADVGTENRDsec(tempyrfile,keynumber,SCRNvirtualSubData,parent);
			e=Rmalloc(RDAstrlen(actyp)+34);
			sprintf(e,"Updating %s Accounts Available List",actyp);
			tmprsrc=diagscrn(tempyrfile,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
					prterr("Error Cannot Create Diagnostic Screen."); 
		 		} else ForceWindowUpdate(tmprsrc);
			}
			while(!ef)
			{
				listupdated=FALSE;
				FINDFLDGETSHORT(tempyrfile,"DEFINITION TYPE",
					&deftype1);
				if(deftype!=deftype1) break;
				FINDFLDGETINT(tempyrfile,"FISCAL YEAR",&yearid);
				FINDFLDGETSTRING(tempyrfile,"ACCOUNT CODE",&account_code);
				if(keyn!=(-1))
				{
					if(yearid!=selyear) break;
				}
				if(yearid==selyear)
				{
					ZERNRD(tempaccfile);
					FINDFLDSETSHORT(tempaccfile,"DEFINITION TYPE",deftype);
					FINDFLDSETSTRING(tempaccfile,"ACCOUNT CODE",account_code);
					if(!ADVEQLNRDsec(tempaccfile,1,SCRNvirtualSubData,parent)) 
					{
						FINDFLDGETCHAR(tempaccfile,"DELETEFLAG",&deleteflag);
						FINDFLDGETCHAR(tempaccfile,"ACTIVE",&active);
						if(!deleteflag && active)
						{
							FINDFLDGETCHAR(tempyrfile,"DELETEFLAG",&deleteflag);
							FINDFLDGETCHAR(tempyrfile,"ACTIVE",&active);
							if(!isEMPTY(account_code) && !deleteflag && active)
							{	
								addAPPlib(acctavl,account_code);
								listupdated=TRUE;
							}
						}
					}
				} /* end else */
				if(account_code!=NULL) Rfree(account_code);
				account_code=NULL;
				ef=ADVNXTNRDsec(tempyrfile,keynumber,
					SCRNvirtualSubData,parent);
				if(tmprsrc!=NULL)
					 update_diagnostic(tmprsrc,listupdated);
			}
        	if(tmprsrc!=NULL)
        	{
               		killwindow(tmprsrc);
               		free_rsrc(tmprsrc);
               		tmprsrc=NULL;
        	}
		}
/* does this section in all cases */
		switch(selecteda)
		{
			case EXP_ACCT: /* Expenditures */
				ZERNRD(byrnum);
				FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
				FINDFLDSETINT(byrnum,"FISCAL YEAR",selyear);
				if((keyn=KEYNUMBER(byrnum,"RDA FINBYR KEY"))!=(-1))
				{
					keynumber=keyn;
				} else {
					keynumber=1;
				}
				ef=ADVGTENRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
				e=stralloc("Updating Debit and Credit Accounts Lists");
				tmprsrc=diagscrn(byrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
				if(e!=NULL) Rfree(e);
				if(tmprsrc!=NULL) 
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
						prterr("Error Cannot Create Diagnostic Screen."); 
					} else ForceWindowUpdate(tmprsrc);
				}
				while(!ef)
				{
					listupdated=FALSE;
					FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",&deftype1);
					if(deftype!=deftype1) break;
					FINDFLDGETINT(byrnum,"FISCAL YEAR",&yearid);
					if(keyn!=(-1))
					{
						if(yearid!=selyear) break;
					} 
					if(yearid==selyear) 
					{
						FINDFLDGETSTRING(byrnum,"ACCOUNT CODE",&account_code);
						ZERNRD(balnum);
						FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype);
						FINDFLDSETSTRING(balnum,"ACCOUNT CODE",account_code);
						if(!ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
						{
							FINDFLDGETCHAR(balnum,"DELETEFLAG",&deleteflag);
							FINDFLDGETCHAR(balnum,"ACTIVE",&active);
							if(!deleteflag && active)
							{
								FINDFLDGETCHAR(byrnum,"DELETEFLAG",&deleteflag);
								FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
								if(account_code!=NULL&&!deleteflag&&active) 
								{	
									FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
									switch(jtype)
									{
										case 1:
											if(!RDAstrcmp(baltype,"UNRESERVED FUND BALANCE"))
											{
												addAPPlib(debitavl,account_code);
												listupdated=TRUE;
											} else if(!RDAstrcmp(baltype,"APPROPRIATION"))
											{
												addAPPlib(creditavl,account_code);
													listupdated=TRUE;
											}
											break;
										case 2:
											if(!RDAstrcmp(baltype,"ENCUMBRANCE"))
												addAPPlib(debitavl,account_code);
											addAPPlib(creditavl,account_code);
											listupdated=TRUE;
											break;
										case 0:
											if(!RDAstrcmp(baltype,"EXPENDITURE"))
												addAPPlib(debitavl,account_code);
											addAPPlib(creditavl,account_code);
											listupdated=TRUE;
											break;
										default:
										  prterr("Invalid Journal Entry Type [%d]",jtype);
										  break;
									}
								
								}
							}
						}
					}
					if(account_code!=NULL) Rfree(account_code);
					if(baltype!=NULL) Rfree(baltype);
					baltype=NULL;
					account_code=NULL;
					ef=ADVNXTNRDsec(byrnum,keynumber,
						SCRNvirtualSubData,parent);
					if(tmprsrc!=NULL)
						update_diagnostic(tmprsrc,listupdated);
	
				}
        		if(tmprsrc!=NULL)
        		{
               			killwindow(tmprsrc);
               			free_rsrc(tmprsrc);
               			tmprsrc=NULL;
        		}
				addAPPlib(creditavl,"Not Applicable");
				break;
			case REV_ACCT: /* Revenue */
				ZERNRD(byrnum);
				FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
				FINDFLDSETINT(byrnum,"FISCAL YEAR",selyear);
				if((keyn=KEYNUMBER(byrnum,"RDA FINBYR KEY"))!=(-1))
				{
					keynumber=keyn;
				} else {
					keynumber=1;
				}
				ef=ADVGTENRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
				e=stralloc("Updating Debit and Credit Accounts Lists");
				tmprsrc=diagscrn(byrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
				if(e!=NULL) Rfree(e);
				if(tmprsrc!=NULL) 
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
						prterr("Error Cannot Create Diagnostic Screen."); 
					} else ForceWindowUpdate(tmprsrc);
				}
				while(!ef)
				{
					listupdated=FALSE;
					FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",&deftype1);
					if(deftype1!=deftype) break;
					FINDFLDGETINT(byrnum,"FISCAL YEAR",&yearid);
					if(keyn!=(-1))
					{
						if(yearid!=selyear) break;
					} 
					if(yearid==selyear) 
					{
						FINDFLDGETSTRING(byrnum,"ACCOUNT CODE",&account_code);
						ZERNRD(balnum);
						FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype);
						FINDFLDSETSTRING(balnum,"ACCOUNT CODE",account_code);
						if(!ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
						{
							FINDFLDGETCHAR(balnum,"DELETEFLAG",&deleteflag);
							FINDFLDGETCHAR(balnum,"ACTIVE",&active);
							if(!deleteflag && active)
							{
								FINDFLDGETCHAR(byrnum,"DELETEFLAG",&deleteflag);
								FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
								if(account_code!=NULL&&!deleteflag&&active) 
								{		
									FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
									switch(jtype)
									{
										case 1:
											if(!RDAstrcmp(baltype,"ESTIMATED REVENUE"))
											{
												addAPPlib(debitavl,account_code);
												listupdated=TRUE;
											} else if(!RDAstrcmp(baltype,"UNRESERVED FUND BALANCE"))
											{
												addAPPlib(creditavl,account_code);
												listupdated=TRUE;
											}
											break;
										case 0:
											if(!RDAstrcmp(baltype,"REVENUE"))
												addAPPlib(creditavl,account_code);
											addAPPlib(debitavl,account_code);
											listupdated=TRUE;
											break;
										default:
											  prterr("Invalid Journal Entry Type [%d]",jtype);
											  break;
									}
								}
							}
						}
					}
					if(account_code!=NULL) Rfree(account_code);
					if(baltype!=NULL) Rfree(baltype);
					ef=ADVNXTNRDsec(byrnum,keynumber,
						SCRNvirtualSubData,parent);
					if(tmprsrc!=NULL) 
						update_diagnostic(tmprsrc,listupdated);
				}
        		if(tmprsrc!=NULL)
        		{
               			killwindow(tmprsrc);
               			free_rsrc(tmprsrc);
        		}
				addAPPlib(debitavl,"Not Applicable");
				break;
			case BAL_ACCT: /* Balance Sheet */
				addAPPlib(debitavl,"Not Applicable");
				addAPPlib(creditavl,"Not Applicable");
				break;
			default:
				prterr("Invalid Account Type [%d]",selecteda);
				break;
		}
		}
		year_identification=selyear;
		account_type=selecteda;
		definition_type=selectedd;
		journal_entry_type=jtype;
	}
	if(account_code!=NULL) Rfree(account_code);
	if(baltype!=NULL) Rfree(baltype);
	if(actyp!=NULL) Rfree(actyp);
	if(speedkeyname!=NULL) Rfree(speedkeyname);
}
static void changeDEFtype(RDArsrc *parent)
{
	int sela=0,seld=0,selc=0;
	int selecteddef=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&selecteddef);
	if(selecteddef!=definition_type)
	{
	year_identification=(-1);
	journal_entry_type=(-1);
	FINDRSCGETINT(parent,"ACCOUNT CODES",&sela);
	FINDRSCGETINT(parent,"DEBIT CODES",&seld);
	FINDRSCGETINT(parent,"CREDIT CODES",&selc);
	changeCODElist(parent,0);
	if(AddList==TRUE)
	{
		if(acctavl->numlibs<1)
		{
			addAPPlib(acctavl,"No Accounts Available");
		}
		if(sela>=acctavl->numlibs) sela=0;
		if(!FINDRSCLISTAPPlib(parent,"ACCOUNT CODES",sela,acctavl))
		{
			updatersrc(parent,"ACCOUNT CODES"); 
		}
		if(debitavl->numlibs<1)
		{
			addAPPlib(debitavl,"No Accounts Available"); 
		}
		if(seld>=debitavl->numlibs) seld=0;
		if(!FINDRSCLISTAPPlib(parent,"DEBIT CODES",seld,debitavl))
		{
			updatersrc(parent,"DEBIT CODES");
		}
		if(creditavl->numlibs<1)
		{
			addAPPlib(creditavl,"No Accounts Available");
		}
		if(selc>=creditavl->numlibs) selc=0;
		if(!FINDRSCLISTAPPlib(parent,"CREDIT CODES",selc,creditavl))
		{
			updatersrc(parent,"CREDIT CODES");
		}
	}
	debitcodenum=1;
	getsearchstr(parent,&debitcodenum);
	creditcodenum=2;
	getsearchstr(parent,&creditcodenum); 
	getacctdesc(parent);
	}
}
static void DEFtypecb(RDArsrc *parent)
{
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE DESCRIPTION",NULL);
	changeDEFtype(parent);
}
static void changejtype(RDArsrc *parent)
{
	int selj=0,seld=0,selc=0;

	FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&selj);
	if(selj!=journal_entry_type)
	{
	journal_entry_type=(-1);
	FINDRSCGETINT(parent,"DEBIT CODES",&seld);
	FINDRSCGETINT(parent,"CREDIT CODES",&selc);
	changeCODElist(parent,1);
	if(AddList==TRUE)
	{
		if(debitavl->numlibs<1)
		{
			addAPPlib(debitavl,"No Accounts Available");
		}
		if(seld>=debitavl->numlibs) seld=0;
		if(!FINDRSCLISTAPPlib(parent,"DEBIT CODES",seld,debitavl))
		{
			updatersrc(parent,"DEBIT CODES");
		}
		if(creditavl->numlibs<1)
		{
			addAPPlib(creditavl,"No Accounts Available");
		}
		if(selc>=creditavl->numlibs) selc=0;
		if(!FINDRSCLISTAPPlib(parent,"CREDIT CODES",selc,creditavl))
		{
			updatersrc(parent,"CREDIT CODES");
		}
	}
	debitcodenum=1;
	getsearchstr(parent,&debitcodenum);
	creditcodenum=2;
	getsearchstr(parent,&creditcodenum); 
	}
}
static void ACCTtypecb(RDArsrc *parent)
{
	int sdef=0,stype=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&sdef);
	FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&stype);
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE DESCRIPTION",NULL);
	changeACCTtype(parent);
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",sdef,DEFtypes))
	{
		updatersrc(parent,"DEFINITION TYPES");
	}
	if(stype>=jtypes->numlibs) stype=0;
	if(!FINDRSCLISTAPPlib(parent,"JOURNAL ENTRY TYPES",stype,jtypes))
	{
		updatersrc(parent,"JOURNAL ENTRY TYPES");
	}
	changeDEFtype(parent);
}
#ifdef SETJOURNALDEFS
static void setACCTtype(RDArsrc *parent)
{
	int seldeftype=0,seljtype=0;

	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&seljtype)) return;
	changeACCTtype(parent);
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",seldeftype,DEFtypes))
	{
		updatersrc(parent,"DEFINITION TYPES");
	}
	if(!FINDRSCLISTAPPlib(parent,"JOURNAL ENTRY TYPES",seljtype,jtypes))
	{
		updatersrc(parent,"JOURNAL ENTRY TYPES");
	}
	changeDEFtype(parent);
}
#endif
static void changeACCTtype(RDArsrc *parent)
{
	int selecteda=0,acctype=0;
	int x,selectedj=0;
	RDAacct *acct;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&acctype)) return;
	if(acctype!=account_type)
	{
	definition_type=(-1);
	year_identification=(-1);
	journal_entry_type=(-1);
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selecteda)) return;
	if(FINDRSCGETINT(parent,"JOURNAL ENTRY TYPES",&selectedj)) return;
	if(jtypes!=NULL) freeapplib(jtypes);
	jtypes=APPlibNEW();
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	DEFtypes=APPlibNEW();
	switch(acctype)
	{
		case EXP_ACCT: /* Expenditures */
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=EXPENDITURE_ACCOUNT->codetype;
					x<EXPENDITURE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(DEFtypes,acct->name);
				}
			}
			addAPPlib(jtypes,"Normal");
			addAPPlib(jtypes,"Appropriation");
			addAPPlib(jtypes,"Encumbrance");
			break;
		case REV_ACCT: /* Revenues */
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=REVENUE_ACCOUNT->codetype;
					x<REVENUE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(DEFtypes,acct->name);
				}
			}
			addAPPlib(jtypes,"Normal");
			addAPPlib(jtypes,"Estimated Revenue");
			break;
		case BAL_ACCT: /* Balance Sheets */
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
					x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(DEFtypes,acct->name);
				}
			}
			addAPPlib(jtypes,"Debit");
			addAPPlib(jtypes,"Credit");
			break;
		default:
			prterr("Invalid Account Type [%d]",selecteda);
			break;
	}
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"Not Available");
	}
	if(selecteda>=DEFtypes->numlibs) selecteda=0;
	if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",selecteda,DEFtypes))
	{
		updatersrc(parent,"DEFINITION TYPES");
	}
	if(selectedj>=jtypes->numlibs) selectedj=0;
	if(!FINDRSCLISTAPPlib(parent,"JOURNAL ENTRY TYPES",selectedj,jtypes))
	{
		updatersrc(parent,"JOURNAL ENTRY TYPES");
	}
	}
}
static void getjournals(RDArsrc *parent)
{

	if(journalavl!=NULL) freeapplib(journalavl);
	journalavl=APPlibNEW();
	ZERNRD(jrnlnum);
	journalavl=ADVMakeNRDlistAllSTRING(jrnlnum,module,
				"Creating Journals Available List",1,
				"JOURNAL IDENTIFICATION",SCRNvirtualSubData,
				parent);
}
static int getyears(RDArsrc *parent,RDAfinmgt *finsetup)
{
	short ef;
	int selyear=0,curryear=0,year=0;
	char delflag=FALSE/*,*yearstr=NULL*/;

	curryear=GETCURRENTFISCALYEAR(finsetup->month);
	if(yearavl!=NULL) freeapplib(yearavl);
	yearavl=APPlibNEW();
	ZERNRD(yearnum);
	ef=ADVFRSNRDsec(yearnum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETCHAR(yearnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(yearnum,"FISCAL YEAR",&year);
			sprintf(stemp,"%-4d",year);
			addAPPlib(yearavl,stemp);
			if(year==curryear) selyear=((yearavl->numlibs)-1);
		}
		ef=ADVNXTNRDsec(yearnum,1,SCRNvirtualSubData,parent);
	}
	return(selyear);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
	if(yearnum!=(-1))
	{
		COPYFIELD(filenum,yearnum,"FISCAL YEAR");
		if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,parent)) KEYNRD(yearnum,1);
		if(update) updatefilerecord2rsrc(yearnum,parent);
		else filerecord2rsrc(yearnum,parent);
	}
	if(yearnum!=(-1))
	{
		COPYFIELD(filenum,jrnlnum,"JOURNAL IDENTIFICATION");
		if(ADVEQLNRDsec(jrnlnum,1,SCRNvirtualSubData,parent)) KEYNRD(jrnlnum,1);
		if(update) updatefilerecord2rsrc(jrnlnum,parent);
		else filerecord2rsrc(jrnlnum,parent);
	}
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
}
void main(int argc,char **argv)
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	short shval=0;
	int start=0,presentyear=0;
	int sela=0,seld=0;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL;
	char *name=NULL,*filename=NULL;

	if(InitializeSubsystems(argc,argv,module,"ADD JOURNAL ENT")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	GetAddList();
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("JEAUDIT");
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,quit_record,parent)==(-1)) return;
	if((filenum=APPOPNNRD(module,"FINJEN",TRUE,TRUE))==(-1)) return;
	if((byrnum=APPOPNNRD(module,"FINBYR",TRUE,TRUE))==(-1)) return;
	if((eyrnum=APPOPNNRD(module,"FINEYR",TRUE,TRUE))==(-1)) return;
	if((ryrnum=APPOPNNRD(module,"FINRYR",TRUE,TRUE))==(-1)) return;
	if((yearnum=APPOPNNRD(module,"FINYEAR",TRUE,FALSE))==(-1)) return;
	if((jrnlnum=APPOPNNRD(module,"FINJRNL",TRUE,FALSE))==(-1)) return;
	if((expnum=APPOPNNRD(module,"FINEACC",TRUE,FALSE))==(-1)) return;
	if((revnum=APPOPNNRD(module,"FINRACC",TRUE,FALSE))==(-1)) return;
	if((balnum=APPOPNNRD(module,"FINBACC",TRUE,FALSE))==(-1)) return;
	filename=FILENAME(filenum);
	nokeys=NUMKEYS(filenum);
	keys=KEYPOINTER(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(AddList)
	{
		parent=RDArsrcNEW(module,"ADD JOURNAL ENTRIES");
	} else {
		parent=RDArsrcNEW(module,"ADD JOURNAL ENTRIES W/O LISTS");
	}
	definelist=RDArsrcNEW(module,"ADD JOURNAL ENTRY DEFINE LIST");
	addDFincvir(parent,module,"FINBACC",NULL,balnum);
	addDFincvir(parent,module,"FINRACC",NULL,revnum);
	addDFincvir(parent,module,"FINEACC",NULL,expnum);
	addDFincvir(parent,module,"FINRYR",NULL,ryrnum);
	addDFincvir(parent,module,"FINBYR",NULL,byrnum);
	addDFincvir(parent,module,"FINEYR",NULL,eyrnum);
	addDFincvir(parent,module,"FINJRNL",NULL,jrnlnum);
	addDFincvir(parent,module,"FINYEAR",NULL,yearnum);
	addDFincvir(parent,module,"FINJEN",NULL,filenum);
	addDFincvir(definelist,module,"FINJEN",NULL,filenum);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"JOURNAL IDENTIFICATION"))
			{
				getjournals(parent);
				if(journalavl->numlibs<1)
				{
					ERRORDIALOG("FINJRNL Empty File","The FINJRNL file is an empty file.  There must exist at least 1 journal on file to process journal entries appropriately.\n",NULL,TRUE);
					ShutdownSecurity();
					RDAAPPMAINLOOP();
					quit_record(parent);
					return;
				}
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(jrnlnum,parent,FALSE);
					addlstrsrc(parent,"JOURNALS AVAILABLE",&start,TRUE,
						changejournal,journalavl->numlibs,
						&journalavl->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,
							"JOURNALS AVAILABLE",
							(edit_rsrc ? TRUE:FALSE));
					FINDFLDSETSTRING(filenum,"JOURNAL IDENTIFICATION",journalavl->libs[start]);
				}
			} else if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
			{
				presentyear=getyears(parent,finsetup);
				if(yearavl->numlibs<1)
				{
					ERRORDIALOG("FINYEAR Empty File","The FINYEAR file is an empty file.  There must exist at least 1 year on file to process journal entries.\n",NULL,TRUE);
					ShutdownSecurity();
					RDAAPPMAINLOOP();
					quit_record(parent);
					return;
				}
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(yearnum,parent,FALSE);
					addlstrsrc(parent,"YEARS AVAILABLE",&presentyear,TRUE,changeyear,yearavl->numlibs,&yearavl->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,
							"YEARS AVAILABLE",
							(edit_rsrc ? TRUE:FALSE));
					FINDFLDSETINT(filenum,"FISCAL YEAR",presentyear);
				}
			} else if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				ACCTtypes=APPlibNEW();
				addAPPlib(ACCTtypes,accttypes[0]);
				addAPPlib(ACCTtypes,accttypes[1]);
				addAPPlib(ACCTtypes,accttypes[2]);
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addlstrsrc(parent,"ACCOUNT TYPES",&start,TRUE,ACCTtypecb,
						ACCTtypes->numlibs,&ACCTtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,
							"ACCOUNT TYPES",
							(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					DEFtypes=APPlibNEW();
					addlstrsrc(parent,"DEFINITION TYPES",&start,TRUE,DEFtypecb,DEFtypes->numlibs,&DEFtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,"DEFINITION TYPES",(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"ACCOUNT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						acctavl=APPlibNEW();
						start=0;
						addlstrsrc(parent,"ACCOUNT CODES",&start,TRUE,
							getacctdesc,acctavl->numlibs,&acctavl->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"ACCOUNT CODES",FALSE);
					}
					addstdrsrc(parent,"ACCOUNT DESCRIPTION",VARIABLETEXT,40,
						NULL,FALSE);
					addstdrsrc(parent,"ACCOUNT CODE SEARCH",EXPENDITURE,40,
						NULL,(edit_rsrc ? TRUE:FALSE));
					acctcodenum=0;
					FINDRSCSETFUNC(parent,"ACCOUNT CODE SEARCH",acctsearch,
						&acctcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DEBIT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						debitcodenum=1;
						start=0;
						debitavl=APPlibNEW();
						addlstrsrc(parent,"DEBIT CODES",&start,TRUE,
							getsearchstr,debitavl->numlibs,&debitavl->libs,
							&debitcodenum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"DEBIT CODES",FALSE);
					}
					addstdrsrc(parent,"DEBIT CODE DESCRIPTION",VARIABLETEXT,
						40,NULL,FALSE);
					addstdrsrc(parent,"DEBIT CODE SEARCH",BALANCESHEET,40,
						NULL,TRUE);
					debitcodenum=1;
					FINDRSCSETFUNC(parent,"DEBIT CODE SEARCH",acctsearch,
						&debitcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"CREDIT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						creditavl=APPlibNEW();
						start=0;
						creditcodenum=2;
						addlstrsrc(parent,"CREDIT CODES",&start,TRUE,
							getsearchstr,creditavl->numlibs,&creditavl->libs,
							&creditcodenum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"CREDIT CODES",FALSE);
					} 
					addstdrsrc(parent,"CREDIT CODE DESCRIPTION",VARIABLETEXT,
						40,NULL,FALSE);
					addstdrsrc(parent,"CREDIT CODE SEARCH",BALANCESHEET,40,
						NULL,TRUE);
					creditcodenum=2;
					FINDRSCSETFUNC(parent,"CREDIT CODE SEARCH",acctsearch,
						&creditcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(fldx->name,"JOURNAL ENTRY TYPE"))
			{
				jtypes=APPlibNEW();
				start=0;
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addlstrsrc(parent,"JOURNAL ENTRY TYPES",&start,TRUE,changejtype,jtypes->numlibs,&jtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,"JOURNAL ENTRY TYPES",FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"PROCESSING MONTH"))
			{
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
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					start=Ratoi(CURRENT_DATE);
					addlstrsrc(parent,"PROCESSING MONTHS",&start,TRUE,NULL,pmonth->numlibs,&pmonth->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,"PROCESSING MONTHS",FALSE);
				}
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,filenum,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				}
				sprintf(name,"[%s][%s]",filename,fldx->name);
				FINDRSCSETFUNC(parent,name,setfiles,NULL);
			}
		}
	}
	if(name!=NULL) Rfree(name);
	getsupportingrecords(parent,FALSE);
	if(prevjes!=NULL) freeapplib(prevjes);
	prevjes=APPlibNEW();
	addAPPlib(prevjes,"No Journal Entries added this session");
	addlstrsrc(parent,"PREVIOUS J/E'S",&x,TRUE,NULL,prevjes->numlibs,
		&prevjes->libs,NULL);
	FINDRSCSETSTRING(parent,"[FINJEN][ENTRY DATE]",CURRENT_DATE);
	FINDRSCSETSTRING(parent,"[FINJEN][ENTRY TIME]",CURRENT_TIME);
#ifdef SETJOURNALDEFS
	setjournaldefs(parent);
#endif
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	savelist(definelist,parent);
	addstdrsrc(parent,"TOTAL EXPENDITURE",DOLLARS,20,&total_exp,FALSE);
	addstdrsrc(parent,"TOTAL REVENUE",DOLLARS,20,&total_rev,FALSE);
	addstdrsrc(parent,"TOTAL DEBITS",DOLLARS,20,&total_deb,FALSE);
	addstdrsrc(parent,"TOTAL CREDITS",DOLLARS,20,&total_cred,FALSE);
	nofields=0;
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addbtnrsrc(parent,"QUIT",TRUE,quit_check,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	account_type=(-1);definition_type=(-1);
	set_defaults(parent);
	APPmakescrn(parent,TRUE,quit_record,NULL,TRUE);
	FINDRSCGETINT(parent,"ACCOUNT TYPES",&sela);
	shval=sela;
	FINDRSCSETACCTTYPE(parent,"ACCOUNT CODE SEARCH",shval);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&seld);
	shval=seld;
	FINDRSCSETDEFTYPE(parent,"ACCOUNT CODE SEARCH",shval);
	FINDRSCSETDEFTYPE(parent,"DEBIT CODE SEARCH",shval);
	FINDRSCSETDEFTYPE(parent,"CREDIT CODE SEARCH",shval);
	FINDRSCGETDEFTYPE(parent,"ACCOUNT CODE SEARCH",&shval);
	RDAAPPMAINLOOP();
}
