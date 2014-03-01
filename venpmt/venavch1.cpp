/* venavch1.c - Add Voucher Screen */
#define CLEARWDGT
#include <stdio.h>
#include <rdaglob.h>
#include <mix.h>
#include <addlists.h>
#include <fin.h>
#include <venpmt.h>
#define DIAGNOSTIC_SCREENS

extern short vchnum,expnum,revnum,balnum,invnum,filenum,yearnum;
extern short byrnum,eyrnum,ryrnum,finyrnum,banknum;
extern RDAfinmgt *finsetup;
extern RDAvenpmt *venpmt;
extern int VOUCHER_COUNT,venyrid;
extern double invoice_total;
extern char *module,*vendorid,*invid;
static char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL,*paycode1=NULL;
static char *dueto1=NULL,*clearfrom=NULL,*clearcash=NULL;
static RDArsrc *definelist=NULL;
static APPlib *prevvch=NULL;
static APPlib *ACCTtypes=NULL;
APPlib *DEFtypes=NULL;
int year_identification=(-1);
short definition_type=(-1),account_type=(-1);
short clear_definition_type=(-1);
APPlib *acctavl=NULL,*debitavl=NULL,*creditavl=NULL,*payableavl=NULL;
APPlib *clearDEFtypes=NULL,*clearfromavl=NULL,*clearcashavl=NULL,*duetoavl=NULL;
static APPlib *yearavl=NULL;
static double total_exp=0,total_rev=0,total_deb=0,total_cred=0;
static short acctcodenum=0,debitcodenum=1,creditcodenum=2,payablecodenum=3;
static short duetonum=4,clearfromnum=5,clearcashnum=6;
static void ACCTtypecb(RDArsrc *),DEFtypecb(RDArsrc *);
static void clearDEFtypecb(RDArsrc *);
static void changeACCTtype(RDArsrc *);
static void changeDEFtype(RDArsrc *,short);
static void changeclearDEFtype(RDArsrc *,short);
static int getyears(RDAfinmgt *,RDArsrc *);
static void setyear(RDArsrc *);
static void changeyear(RDArsrc *,short);
static void changeyearcb(RDArsrc *);
static void getacctdesc(RDArsrc *);
static void changeCODElist(RDArsrc *);
static void changeclearCODElist(RDArsrc *);
static void set_defaults(RDArsrc *);
static void set_defaultscb(RDArsrc *);
static void savelist(RDArsrc *,RDArsrc *),savelistcb(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *),quit_record(RDArsrc *);
static void save_check(RDArsrc *),quit_check(RDArsrc *);
static void acctsearch(RDArsrc *,short *);
static void getsearchstr(RDArsrc *,short *);
static void clearwidget(RDArsrc *,char *);
static short check_acctcode(RDArsrc *,short);
static void getsupportingfiles(RDArsrc *);
static void readscreen(RDArsrc *);
static void readscreencb(RDArsrc *);
static void SETFILES(RDArsrc *);
static void SETFILEScb(RDArsrc *);

static void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,RDArsrc *parent)
{
	short ef=0,accttype=0;
	double amount=0.0,local_exp=0.0,local_rev=0.0,local_deb=0.0,local_cred=0.0;
	char *username=NULL,deleteflag=FALSE,verified=FALSE;
	char *acct=NULL,*venid=NULL,*iid=NULL;
	int yid=0;
	RDArsrc *tmprsrc=NULL;

	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(prevvch!=NULL) freeapplib(prevvch);
	prevvch=APPlibNEW();
#ifdef DIAGNOSTIC_SCREENS
	tmprsrc=diagscrn(vchnum,"DIAGNOSTIC SCREEN",module,
		"Reading Unprocessed Vouchers",NULL);
#else 
	tmprsrc=NULL;
#endif
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			ForceWindowUpdate(tmprsrc);
		}
	}
	ZERNRD(vchnum);
	FINDFLDSETSTRING(vchnum,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETINT(vchnum,"CALENDAR YEAR",venyrid);
	FINDFLDSETSTRING(vchnum,"INVOICE IDENTIFICATION",invid);
	ef=ADVGTENRDsec(vchnum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		if(venid!=NULL) Rfree(venid);
		FINDFLDGETSTRING(vchnum,"VENDOR IDENTIFICATION",&venid);
		FINDFLDGETINT(vchnum,"CALENDAR YEAR",&yid);
		if(iid!=NULL) Rfree(iid); 
		FINDFLDGETSTRING(vchnum,"INVOICE IDENTIFICATION",&iid);
		if(RDAstrcmp(venid,vendorid) || yid!=venyrid ||
			RDAstrcmp(iid,invid)) break;
		FINDFLDGETCHAR(vchnum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETCHAR(vchnum,"SOURCE USER VERIFIED",&verified);
			if(!verified)
			{
				FINDFLDGETSTRING(vchnum,"SOURCE USER",&username);
				if(!RDAstrcmp(username,USERLOGIN))
				{
					makedflist();
					FINDFLDGETSHORT(vchnum,"ACCOUNT TYPE",&accttype);
					FINDFLDGETDOUBLE(vchnum,"AMOUNT",&amount);
					switch(accttype)
					{
						case EXP_ACCT:
							local_exp+=amount;
							FINDFLDGETSTRING(vchnum,"DEBIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0) 
							{
								local_deb+=amount;
								Rfree(acct);
							}
							FINDFLDGETSTRING(vchnum,"CREDIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								local_cred+=amount;
								Rfree(acct);
							}
							break;
						case REV_ACCT:
							local_rev+=amount;
							FINDFLDGETSTRING(vchnum,"DEBIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								local_deb+=amount;
								Rfree(acct);
							}
							FINDFLDGETSTRING(vchnum,"CREDIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								local_cred+=amount;
								Rfree(acct);
							}
							break;
						case BAL_ACCT:
							if(amount>0.0) local_deb+=amount;
							else local_cred-=amount;
							FINDFLDGETSTRING(vchnum,"CREDIT CODE",&acct);
							if(acct!=NULL && RDAstrlen(acct)>0)
							{
								if(amount>0.0) local_cred+=amount;
								else local_deb-=amount;
								Rfree(acct);
							}
							break;
						default:
							prterr("Invalid Account Type [%d]",accttype);
							break;
					}
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				} else {
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
				}
			}
		}
		ef=ADVNXTNRDsec(vchnum,1,SCRNvirtualSubData,parent);
	}
	if(venid!=NULL) Rfree(venid);
	if(iid!=NULL) Rfree(iid);
	total_exp=local_exp;
	total_rev=local_rev;
	total_deb=local_deb;
	total_cred=local_cred;
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(prevvch->numlibs<1)
	{
		addAPPlib(prevvch,"No Vouchers added this session");
	}
	if(definelist!=NULL)
	{
		if(!FINDRSCLISTAPPlib(parent,"PREVIOUS VOUCHER'S",(prevvch->numlibs-1),
			prevvch))
		{
			updatersrc(parent,"PREVIOUS VOUCHER'S");
		}
	}
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(prevvch!=NULL)
	{
		if(prevvch->numlibs==1 && !RDAstrcmp(prevvch->libs[0],
			"No Vouchers added this session"))
		{
			freeapplib(prevvch);
			prevvch=APPlibNEW();
		}
	} else {
		prevvch=APPlibNEW();
	}
	for(x=1,length=0;x<(definelist->numrscs-5);x+=2)
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
	while(cur<(definelist->numrscs-5))
	{
		for(x=1;x<(definelist->numrscs-5);x+=2)
		{
			pos_member=definelist->rscs+x;
			len_member=definelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && 
				*len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(vchnum,s);
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
	addAPPlib(prevvch,hold);
	VOUCHER_COUNT+=1;
	if(hold!=NULL) Rfree(hold);
}
static void definelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void set_defaults(RDArsrc *parent)
{
	DefaultResetScreen(parent,TRUE);
	FINDRSCSETSTRING(parent,"[VENVCH][VENDOR IDENTIFICATION]",vendorid);
	FINDRSCSETSTRING(parent,"[VENVCH][INVOICE IDENTIFICATION]",invid);
	FINDRSCSETINT(parent,"[VENVCH][CALENDAR YEAR]",venyrid);
	FINDRSCSETDOUBLE(parent,"TOTAL EXPENDITURE",total_exp);
	FINDRSCSETDOUBLE(parent,"TOTAL REVENUE",total_rev);
	FINDRSCSETDOUBLE(parent,"TOTAL DEBITS",total_deb);
	FINDRSCSETDOUBLE(parent,"TOTAL CREDITS",total_cred);
	FINDRSCSETDOUBLE(parent,"INVOICE TOTAL",invoice_total);
        ReadRDAScrolledLists(parent);
	changeACCTtype(parent);
	changeyear(parent,TRUE);
	getacctdesc(parent);
	debitcodenum=1;
	getsearchstr(parent,&debitcodenum);
	creditcodenum=2;
	getsearchstr(parent,&creditcodenum);
	payablecodenum=3;
	getsearchstr(parent,&payablecodenum);
	if(venpmt->use_venpmt_fund)
	{
	duetonum=4;
	getsearchstr(parent,&duetonum);
	changeclearDEFtype(parent,TRUE);
	clearfromnum=5;
	getsearchstr(parent,&clearfromnum);
	clearcashnum=6;
	getsearchstr(parent,&clearcashnum);
	}
	readscreen(parent);
}
static void set_defaultscb(RDArsrc *parent)
{
	int selprev=0;

	FINDRSCGETINT(parent,"PREVIOUS VOUCHERS",&selprev);
	set_defaults(parent);
	SETFILES(parent);
	FINDRSCSETINT(parent,"PREVIOUS VOUCHERS",selprev);
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
	short cdeftype1=0;
	int selcdeftype=0;
	char active=FALSE,delflag=FALSE;
	int selectedd=0,selecteda=0,selyear=0,procyrid=0;

	FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	deftype1=(short)selectedd;
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCGETINT(parent,"CLEARING DEFINITION TYPES",&selcdeftype);
	cdeftype1=(short)selcdeftype;
	}
	FINDRSCGETINT(parent,"FISCAL YEARS",&selyear);
	procyrid=Ratoi(yearavl->libs[selyear]);
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
		case 0:	/* check account code value */
			if(acctcode1!=NULL) Rfree(acctcode1);
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
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			if(ADVEQLNRDsec(tempaccfile,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+300);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED ACCOUNT CODE.  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",acctcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempaccfile,"DELETEFLAG",&delflag);
			if(delflag)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+250);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",acctcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} 
			FINDFLDGETCHAR(tempaccfile,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+200);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE ACCOUNT CODE.",acctcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			ZERNRD(tempyrfile);
			FINDFLDSETSHORT(tempyrfile,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(tempyrfile,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(tempyrfile,"ACCOUNT CODE",acctcode1);
			if(ADVEQLNRDsec(tempyrfile,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(acctcode1)+400);
				sprintf(tmp,
					"The ACCOUNT CODE [%s] that was entered is not a DEFINED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",
					acctcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(acctcode1)+300);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if this account should not be deleted.",acctcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+200);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE YEAR ACCOUNT CODE for the YEAR [%d].",acctcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			getacctdesc(parent);
			break;
		case 1:	/* check debit code value */
			if(debcode1!=NULL) Rfree(debcode1); 
			readwidget(parent,"DEBIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
			if(selecteda==BAL_ACCT || (selecteda==REV_ACCT && (debcode1==NULL || RDAstrlen(debcode1)<1)))
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
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+200);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",debcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+200);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",debcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(debcode1)+200);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",debcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",debcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+280);
				sprintf(tmp,
					"The DEBIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",
					debcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+250);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",debcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+300);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",debcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(selecteda==REV_ACCT)
			{
				if(debcode1==NULL || RDAstrlen(debcode1)<1)
				{
					clearwidget(parent,"DEBIT CODE SEARCH");
					clearwidget(parent,"DEBIT CODE DESCRIPTION");
					if(debcode1!=NULL) Rfree(debcode1);
					return(FALSE);
				}
			} else if(selecteda==EXP_ACCT)
			{
				if(RDAstrcmp(baltype,"EXPENDITURE"))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
					prterr("The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE for the ACCOUNT TYPE EXPENDITURE.");
					clearwidget(parent,"DEBIT CODE SEARCH");
					clearwidget(parent,"DEBIT CODE DESCRIPTION");
					if(debcode1!=NULL) Rfree(debcode1);
					return(FALSE);
				} 
			}	
			break;
		case 2:	/* check credit code value */
			if(credcode1!=NULL) Rfree(credcode1);
			readwidget(parent,"CREDIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
			if(selecteda==EXP_ACCT && (credcode1==NULL || RDAstrlen(credcode1)<1))
			{
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				if(credcode1!=NULL)  Rfree(credcode1);
				return(FALSE);
			} 
			ZERNRD(balnum);
			FINDFLDSETCHAR(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",credcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(credcode1)+200);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",credcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1); 
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+200);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",credcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				prterr(tmp);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+150);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",credcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",credcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+255);
				sprintf(tmp,
					"The CREDIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up."
,credcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+200);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",credcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+150);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",credcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(selecteda==EXP_ACCT)
			{
				if(credcode1==NULL || RDAstrlen(credcode1)<1)
				{
					clearwidget(parent,"CREDIT CODE SEARCH");
					clearwidget(parent,"CREDIT CODE DESCRIPTION");
					if(credcode1!=NULL) Rfree(credcode1);
					return(FALSE);
				}
			} else if(selecteda==REV_ACCT)
			{
				if(RDAstrcmp(baltype,"CASH"))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of CASH for the ACCOUNT TYPE REVENUE.",NULL,NULL,TRUE,NULL);
					prterr("The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of CASH for the ACCOUNT TYPE REVENUE.");
					clearwidget(parent,"CREDIT CODE SEARCH");
					clearwidget(parent,"CREDIT CODE DESCRIPTION");
					if(credcode1!=NULL) Rfree(credcode1);
					return(FALSE);
				} 
			}	
			break;
		case 3: /* check payable code value */
			if(paycode1!=NULL) Rfree(paycode1); 
			readwidget(parent,"DEBIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"PAYABLE CODE SEARCH",&paycode1);
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",paycode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(paycode1)+200);
				sprintf(tmp,"The PAYABLE CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",paycode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"PAYABLE CODE SEARCH");
				clearwidget(parent,"PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(paycode1!=NULL) Rfree(paycode1);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(paycode1)+200);
				sprintf(tmp,"The PAYABLE CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",paycode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"PAYABLE CODE SEARCH");
				clearwidget(parent,"PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(paycode1!=NULL) Rfree(paycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(paycode1)+150);
				sprintf(tmp,"The PAYABLE CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",paycode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"PAYABLE CODE SEARCH");
				clearwidget(parent,"PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(paycode1!=NULL) Rfree(paycode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",paycode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(paycode1)+282);
				sprintf(tmp,"The PAYABLE CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",paycode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"PAYABLE CODE SEARCH");
				clearwidget(parent,"PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(paycode1!=NULL) Rfree(paycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(paycode1)+200);
				sprintf(tmp,"The PAYABLE CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",paycode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"PAYABLE CODE SEARCH");
				clearwidget(parent,"PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(paycode1!=NULL) Rfree(paycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(paycode1)+150);
				sprintf(tmp,"The PAYABLE CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",debcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"PAYABLE CODE SEARCH");
				clearwidget(parent,"PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(paycode1!=NULL) Rfree(paycode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ACCOUNTS PAYABLE for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
				prterr("The PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ACCOUNTS PAYABLE for the ACCOUNT TYPE EXPENDITURE.");
				clearwidget(parent,"PAYABLE CODE SEARCH");
				clearwidget(parent,"PAYABLE CODE DESCRIPTION");
				if(paycode1!=NULL) Rfree(paycode1);
				return(FALSE);
			}	
			break;
		case 4: /* Due From */
			if(venpmt->use_venpmt_fund)
			{
			if(dueto1!=NULL) Rfree(dueto1); 
			readwidget(parent,"DUE TO SEARCH");
			FINDRSCGETSTRING(parent,"DUE TO SEARCH",&dueto1);
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",dueto1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(dueto1)+200);
				sprintf(tmp,"The DUE TO [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",dueto1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DUE TO SEARCH");
				clearwidget(parent,"DUE TO DESCRIPTION");
				prterr(tmp);
				if(dueto1!=NULL) Rfree(dueto1);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(dueto1)+200);
				sprintf(tmp,"The DUE TO [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",dueto1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DUE TO SEARCH");
				clearwidget(parent,"DUE TO DESCRIPTION");
				prterr(tmp);
				if(dueto1!=NULL) Rfree(dueto1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(dueto1)+150);
				sprintf(tmp,"The DUE TO [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",dueto1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DUE TO SEARCH");
				clearwidget(parent,"DUE TO DESCRIPTION");
				prterr(tmp);
				if(dueto1!=NULL) Rfree(dueto1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",dueto1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(dueto1)+282);
				sprintf(tmp,"The DUE TO [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",dueto1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DUE TO SEARCH");
				clearwidget(parent,"DUE TO DESCRIPTION");
				prterr(tmp);
				if(dueto1!=NULL) Rfree(dueto1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(dueto1)+200);
				sprintf(tmp,"The DUE TO [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",dueto1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DUE TO SEARCH");
				clearwidget(parent,"DUE TO DESCRIPTION");
				prterr(tmp);
				if(dueto1!=NULL) Rfree(dueto1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(dueto1)+150);
				sprintf(tmp,"The DUE TO [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",debcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DUE TO SEARCH");
				clearwidget(parent,"DUE TO DESCRIPTION");
				prterr(tmp);
				if(dueto1!=NULL) Rfree(dueto1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"DUE TO"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DUE TO entered does not have the correct BALANCE SHEET REFERENCE TYPE of DUE TO for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
				prterr("The DUE TO entered does not have the correct BALANCE SHEET REFERENCE TYPE of DUE TO for the ACCOUNT TYPE EXPENDITURE.");
				clearwidget(parent,"DUE TO SEARCH");
				clearwidget(parent,"DUE TO DESCRIPTION");
				if(dueto1!=NULL) Rfree(dueto1);
				return(FALSE);
			}	
			}
			break;
		case 5: /* Clear Due To */
			if(venpmt->use_venpmt_fund)
			{
			if(clearfrom!=NULL) Rfree(clearfrom); 
			readwidget(parent,"CLEARING DUE FROM SEARCH");
			FINDRSCGETSTRING(parent,"CLEARING DUE FROM SEARCH",&clearfrom);
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",clearfrom);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearfrom)+200);
				sprintf(tmp,"The CLEARING DUE FROM [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",clearfrom);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING DUE FROM SEARCH");
				clearwidget(parent,"CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(clearfrom!=NULL) Rfree(clearfrom);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(clearfrom)+200);
				sprintf(tmp,"The CLEARING DUE FROM [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",clearfrom);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING DUE FROM SEARCH");
				clearwidget(parent,"CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(clearfrom!=NULL) Rfree(clearfrom);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearfrom)+150);
				sprintf(tmp,"The CLEARING DUE FROM [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",clearfrom); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING DUE FROM SEARCH");
				clearwidget(parent,"CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(clearfrom!=NULL) Rfree(clearfrom);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",clearfrom);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearfrom)+320);
				sprintf(tmp,"The CLEARING DUE FROM [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected CLEARING DEFINITION TYPE.  If not and it is needed it needs to be set up.",clearfrom,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING DUE FROM SEARCH");
				clearwidget(parent,"CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(clearfrom!=NULL) Rfree(clearfrom);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearfrom)+200);
				sprintf(tmp,"The CLEARING DUE FROM [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",clearfrom,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING DUE FROM SEARCH");
				clearwidget(parent,"CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(clearfrom!=NULL) Rfree(clearfrom);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearfrom)+150);
				sprintf(tmp,"The CLEARING DUE FROM [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",debcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING DUE FROM SEARCH");
				clearwidget(parent,"CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(clearfrom!=NULL) Rfree(clearfrom);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"DUE FROM"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CLEARING DUE FROM entered does not have the correct BALANCE SHEET REFERENCE TYPE of DUE FROM for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
				prterr("The CLEARING DUE FROM entered does not have the correct BALANCE SHEET REFERENCE TYPE of DUE FROM for the ACCOUNT TYPE EXPENDITURE.");
				clearwidget(parent,"CLEARING DUE FROM SEARCH");
				clearwidget(parent,"CLEARING DUE FROM DESCRIPTION");
				if(clearfrom!=NULL) Rfree(clearfrom);
				return(FALSE);
			}	
			}
			break;
		case 6: /* Clear Cash */
			if(venpmt->use_venpmt_fund)
			{
			if(clearcash!=NULL) Rfree(clearcash); 
			readwidget(parent,"CLEARING CASH SEARCH");
			FINDRSCGETSTRING(parent,"CLEARING CASH SEARCH",&clearcash);
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",clearcash);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearcash)+200);
				sprintf(tmp,"The CLEARING CASH [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",clearcash);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING CASH SEARCH");
				clearwidget(parent,"CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(clearcash!=NULL) Rfree(clearcash);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(clearcash)+200);
				sprintf(tmp,"The CLEARING CASH [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",clearcash);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING CASH SEARCH");
				clearwidget(parent,"CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(clearcash!=NULL) Rfree(clearcash);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearcash)+150);
				sprintf(tmp,"The CLEARING CASH [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",clearcash); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING CASH SEARCH");
				clearwidget(parent,"CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(clearcash!=NULL) Rfree(clearcash);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",clearcash);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearcash)+282);
				sprintf(tmp,"The CLEARING CASH [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",clearcash,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING CASH SEARCH");
				clearwidget(parent,"CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(clearcash!=NULL) Rfree(clearcash);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearcash)+200);
				sprintf(tmp,"The CLEARING CASH [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",clearcash,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING CASH SEARCH");
				clearwidget(parent,"CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(clearcash!=NULL) Rfree(clearcash);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(clearcash)+150);
				sprintf(tmp,"The CLEARING CASH [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",debcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CLEARING CASH SEARCH");
				clearwidget(parent,"CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(clearcash!=NULL) Rfree(clearcash);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"CASH"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CLEARING CASH CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of CASH for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
				prterr("The CLEARING CASH entered does not have the correct BALANCE SHEET REFERENCE TYPE of CASH for the ACCOUNT TYPE EXPENDITURE.");
				clearwidget(parent,"CLEARING CASH SEARCH");
				clearwidget(parent,"CLEARING CASH DESCRIPTION");
				if(clearcash!=NULL) Rfree(clearcash);
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
	int x=0,len=0;

	switch(*listtype)
	{
		case 0:	 /* account code */
			searchwidget=stralloc("ACCOUNT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("ACCOUNT CODES");
				holdapplib=acctavl;
			}
			break;
		case 1:	/* debit code */
			searchwidget=stralloc("DEBIT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEBIT CODES");
				holdapplib=debitavl;
			}
			break;
		case 2:	/* credit code */
			searchwidget=stralloc("CREDIT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("CREDIT CODES");
				holdapplib=creditavl;
			}
			break;
		case 3: /* payable code */
			searchwidget=stralloc("PAYABLE CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("PAYABLE CODES");
				holdapplib=payableavl;
			}
			break;
		case 4: /* Due From */
			searchwidget=stralloc("DUE TO SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DUE TO CODES");
				holdapplib=duetoavl;
			}
			break;
		case 5: /* Clearing Due To */
			searchwidget=stralloc("CLEARING DUE FROM SEARCH");
			if(AddList)
			{
				listwidget=stralloc("CLEARING DUE FROM CODES");
				holdapplib=clearfromavl;
			}
			break;
		case 6: /* Clearing Cash */
			searchwidget=stralloc("CLEARING CASH SEARCH");
			if(AddList)
			{
				listwidget=stralloc("CLEARING CASH CODES");
				holdapplib=clearcashavl;
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
			len=RDAstrlen(selectedc);
			if(len!=0)
			{
				for(x=0;x<holdapplib->numlibs;++x)
				{
					if(strncmp(selectedc,holdapplib->libs[x],len)<=0) break;
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
	} else if(check_acctcode(parent,*listtype)==FALSE)
	{
		if(selectedc!=NULL) Rfree(selectedc);
		if(searchwidget!=NULL) Rfree(searchwidget);
		if(listwidget!=NULL) Rfree(listwidget);
		return;
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
	int selcdeftype=0;
	short deftype=0;
	char *listwidget=NULL,*searchwidget=NULL;
	char *descript=NULL,*descriptwidget=NULL;
	char *accountcode=NULL;
	APPlib *holdapplib=NULL;

	switch(*whichlist)
	{
		case 1:	 /* debit availble */
			if(AddList)
			{
				holdapplib=debitavl;
				listwidget=stralloc("DEBIT CODES");
			}
			searchwidget=stralloc("DEBIT CODE SEARCH");
			descriptwidget=stralloc("DEBIT CODE DESCRIPTION");
			break;
		case 2:	 /* credit available */
			if(AddList)
			{
				holdapplib=creditavl;
				listwidget=stralloc("CREDIT CODES");
			}
			searchwidget=stralloc("CREDIT CODE SEARCH");
			descriptwidget=stralloc("CREDIT CODE DESCRIPTION");
			break;
		case 3: /* payable available */
			if(AddList)
			{
				holdapplib=payableavl;
				listwidget=stralloc("PAYABLE CODES");
			}
			searchwidget=stralloc("PAYABLE CODE SEARCH");
			descriptwidget=stralloc("PAYABLE CODE DESCRIPTION");
			break;
		case 4: /* Due From */
			if(AddList)
			{
				holdapplib=duetoavl;
				listwidget=stralloc("DUE TO CODES");
			}
			searchwidget=stralloc("DUE TO SEARCH");
			descriptwidget=stralloc("DUE TO DESCRIPTION");
			break;
		case 5: /* Clearing Due To */
			if(AddList)
			{
				holdapplib=clearfromavl;
				listwidget=stralloc("CLEARING DUE FROM CODES");
			}
			searchwidget=stralloc("CLEARING DUE FROM SEARCH");
			descriptwidget=stralloc("CLEARING DUE FROM DESCRIPTION");
			break;
		case 6: /* Clearing Cash */
			if(AddList)
			{
				holdapplib=clearcashavl;
				listwidget=stralloc("CLEARING CASH CODES");
			}
			searchwidget=stralloc("CLEARING CASH SEARCH");
			descriptwidget=stralloc("CLEARING CASH DESCRIPTION");
			break;
		default:
			prterr("Invalid Account List [%d]",*whichlist);
			break;
	}
        FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCGETINT(parent,"CLEARING DEFINITION TYPES",&selcdeftype);
	}
	if(AddList==TRUE)
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
				switch(*whichlist)
				{
					default:
					case 1:	 /* debit availble */
					case 2:
					case 3:
					case 4:
						deftype=(short)selectedd;
						break;
					case 5:
					case 6:
						deftype=(short)selcdeftype;
						break;
				}
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
		switch(*whichlist)
		{
			default:
			case 1:	 /* debit availble */
			case 2:
			case 3:
			case 4:
				deftype=(short)selectedd;
				break;
			case 5:
			case 6:
				deftype=(short)selcdeftype;
				break;
		}
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
	
	if(AddList)
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
		} else {
			FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
		}
		updatersrc(parent,"ACCOUNT CODE SEARCH");
	} else {
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&descript);
	}
	FINDRSCGETINT(parent,"ACCOUNT TYPES",&accttype);
	fileaccttype=(short)accttype;
	FINDFLDSETSHORT(vchnum,"ACCOUNT TYPE",fileaccttype);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&deftype);
	filedeftype=(short)deftype;
	switch(accttype)
	{
		case EXP_ACCT:
			ZERNRD(expnum);
			FINDFLDSETSHORT(expnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(expnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(expnum,1,SCRNvirtualSubData,parent)) KEYNRD(expnum,1);
			if(descript!=NULL) Rfree(descript); 
			FINDFLDGETSTRING(expnum,"DESCRIPTION",&descript);
			break;
		case REV_ACCT:
			ZERNRD(revnum);
			FINDFLDSETSHORT(revnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(revnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(revnum,1,SCRNvirtualSubData,parent)) KEYNRD(revnum,1);
			if(descript!=NULL) Rfree(descript);
			FINDFLDGETSTRING(revnum,"DESCRIPTION",&descript);
			break;
		case BAL_ACCT:
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent)) KEYNRD(balnum,1);
			if(descript!=NULL) Rfree(descript);
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
static void readscreen(RDArsrc *parent)
{
        ZERNRD(vchnum);
        ReadRDAScrolledLists(parent);
	rsrc2filerecord(vchnum,parent);
}
static void readscreencb(RDArsrc *parent)
{
        readallwidgets(parent);
	readscreen(parent);
}
static void save_check(RDArsrc *parent)
{
	int selacct=0,seldeb=0,selcred=0,seldeftype=0,selaccttype=0;
	int seldueto=0,selduefrom=0,selclearcash=0,selpay=0,selyear=0,procyrid=0;
	short x=0,testapp=TRUE;
	short selmonth=0;
	char *acctstr=NULL,*header=NULL,*message=NULL;
	double amount=0.0;

	readscreencb(parent);
	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selaccttype)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	procyrid=Ratoi(yearavl->libs[selyear]);
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(paycode1!=NULL) Rfree(paycode1);
	if(dueto1!=NULL) Rfree(dueto1);
	if(clearfrom!=NULL) Rfree(clearfrom);
	if(clearcash!=NULL) Rfree(clearcash);
	if(AddList)
	{
		FINDRSCGETINT(parent,"ACCOUNT CODES",&selacct);
		if(RDAstrcmp(acctavl->libs[selacct],"No Accounts Available") &&
			RDAstrcmp(acctavl->libs[selacct],"Not Applicable"))
                {
			acctcode1=stralloc(acctavl->libs[selacct]);
		}
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
		FINDRSCGETINT(parent,"DEBIT CODES",&seldeb);
		if(RDAstrcmp(debitavl->libs[seldeb],"No Accounts Available") &&
			RDAstrcmp(debitavl->libs[seldeb],"Not Applicable"))
                {
			debcode1=stralloc(debitavl->libs[seldeb]);
		}
		FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
		FINDRSCGETINT(parent,"CREDIT CODES",&selcred);
		if(RDAstrcmp(creditavl->libs[selcred],"No Accounts Available") &&
			RDAstrcmp(creditavl->libs[selcred],"Not Applicable"))
                {
			credcode1=stralloc(creditavl->libs[selcred]);
		}
		FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
		FINDRSCGETINT(parent,"PAYABLE CODES",&selpay);
		if(RDAstrcmp(payableavl->libs[selpay],"No Accounts Available") &&
			RDAstrcmp(payableavl->libs[selpay],"Not Applicable"))
                {
			paycode1=stralloc(payableavl->libs[selpay]);
		}
		FINDRSCGETSTRING(parent,"PAYABLE CODE SEARCH",&paycode1);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCGETINT(parent,"DUE TO CODES",&seldueto);
		if(RDAstrcmp(duetoavl->libs[seldueto],"No Accounts Available") &&
			RDAstrcmp(duetoavl->libs[seldueto],"Not Applicable"))
                {
			dueto1=stralloc(duetoavl->libs[seldueto]);
		}
		FINDRSCGETSTRING(parent,"DUE TO SEARCH",&dueto1);
		FINDRSCGETINT(parent,"CLEARING DUE FROM CODES",&selduefrom);
		if(RDAstrcmp(clearfromavl->libs[selduefrom],"No Accounts Available") &&
			RDAstrcmp(clearfromavl->libs[selduefrom],"Not Applicable"))
                {
			clearfrom=stralloc(clearfromavl->libs[selduefrom]);
		}
		FINDRSCGETSTRING(parent,"CLEARING DUE FROM SEARCH",&clearfrom);
		FINDRSCGETINT(parent,"CLEARING CASH CODES",&selclearcash);
		if(RDAstrcmp(clearcashavl->libs[selclearcash],"No Accounts Available") &&
			RDAstrcmp(clearcashavl->libs[selclearcash],"Not Applicable"))
                {
			clearcash=stralloc(clearcashavl->libs[selclearcash]);
		}
		FINDRSCGETSTRING(parent,"CLEARING CASH SEARCH",&clearcash);
		}
	} else {
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
		FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
		FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
		FINDRSCGETSTRING(parent,"PAYABLE CODE SEARCH",&paycode1);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCGETSTRING(parent,"DUE TO SEARCH",&dueto1);
		FINDRSCGETSTRING(parent,"CLEARING DUE FROM SEARCH",&clearfrom);
		FINDRSCGETSTRING(parent,"CLEARING CASH SEARCH",&clearcash);
		}
		if(check_acctcode(parent,0)==FALSE) return;
		if(selaccttype!=BAL_ACCT)
		{
			if(check_acctcode(parent,1)==FALSE) return;
		}
		if(check_acctcode(parent,2)==FALSE) return;
		if(check_acctcode(parent,3)==FALSE) return;
		if(venpmt->use_venpmt_fund)
		{
		if(check_acctcode(parent,4)==FALSE) return;
		if(check_acctcode(parent,5)==FALSE) return;
		if(check_acctcode(parent,6)==FALSE) return;
		}
	}
	if(FINDRSCGETDOUBLE(parent,"[VENVCH][AMOUNT]",&amount)) return;
	if(amount==0.0) 
	{
		ERRORDIALOG("INVALID AMOUNT","This Voucher can not be saved with an AMOUNT of [$  0.00].  Please enter the correct amount for this voucher.",NULL,FALSE);
		prterr("Attemped to save a voucher with a zero dollar amount");
		return;
	}
	switch(selaccttype)
	{
		case EXP_ACCT:
			ZERNRD(eyrnum);
			FINDFLDSETINT(eyrnum,"FISCAL YEAR",procyrid);
			x=(short)seldeftype;
			FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",x);
 			FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acctcode1);
			if(!ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,parent))
			{
				if(finsetup->budget==ANNUALBUDGET) 
				{ 
					testapp=testapprotot(eyrnum,amount,NULL);
				} else {
/*
					FINDFLDGETSHORT(vchnum,"PROCESSING MONTH",&selmonth);
*/
					FINDFLDGETSHORT(vchnum,"FISCAL PROCESSING MONTH",&selmonth);
					if(venpmt->use_fiscal_months==TRUE)
					{
						testapp=testapprotot(eyrnum,amount,FISCALPERIOD[selmonth]);
					} else {
						testapp=testapprotot(eyrnum,amount,CALENDAR_MONTHS[selmonth]);
					}
				}
				if(testapp==FALSE)
				{
         	 		FINDFLDGETSHORT(eyrnum,"DEFINITION TYPE",&x);
           	 		FINDFLDGETSTRING(eyrnum,"ACCOUNT CODE",&acctstr);
           	  		FINDFLDGETINT(eyrnum,"FISCAL YEAR",&procyrid);
           	 		header=stralloc("This Voucher will cause the POSTED APPROPRIATED amount to be less than all of the EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT [%s] in the YEAR [%d].");
              			message=Rrealloc(message,RDAstrlen(header)+RDAstrlen(acctstr)+6);
/* I don't know about this sprintf */
           			sprintf(message,header,acctstr,procyrid);
           	   		message[RDAstrlen(header)+RDAstrlen(acctstr)+6-1]=0;
					WARNINGDIALOG("WARNING DIALOG SCREEN","APPROPRIATIONS NOT ENOUGH FOR TRANSACTION",message,save_record,NULL,FALSE,parent);
           	  		if(header!=NULL) Rfree(header);
           			if(message!=NULL) Rfree(message);
           			if(acctstr!=NULL) Rfree(acctstr);
				} else {
					save_record(parent);
				}
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
	int seldueto=0,selduefrom=0,selclearcash=0,selpay=0,selyear=0,procyrid=0;
	int selcdeftype=0;
	short selmonth=0;
	char *rcddesc=NULL,*errordesc=NULL,deleteflag=FALSE,*acct=NULL;
	short x=0,cdeftype1=0,deftype1=0,accttype=0;
	int lf=0;
	char *date=NULL,*timex=NULL;
	double amount=0.0;
	RDArmem *member=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selaccttype)) return;
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(paycode1!=NULL) Rfree(paycode1);
	if(dueto1!=NULL) Rfree(dueto1);
	if(clearfrom!=NULL) Rfree(clearfrom);
	if(clearcash!=NULL) Rfree(clearcash);
	if(AddList)
	{
		FINDRSCGETINT(parent,"ACCOUNT CODES",&selacct);
		acctcode1=stralloc(acctavl->libs[selacct]);
		FINDRSCGETINT(parent,"DEBIT CODES",&seldebit);
		debcode1=stralloc(debitavl->libs[seldebit]);
		FINDRSCGETINT(parent,"CREDIT CODES",&selcredit);
		credcode1=stralloc(creditavl->libs[selcredit]);
		FINDRSCGETINT(parent,"PAYABLE CODES",&selpay);
		paycode1=stralloc(payableavl->libs[selpay]);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCGETINT(parent,"DUE TO CODES",&seldueto);
		dueto1=stralloc(duetoavl->libs[seldueto]);
		FINDRSCGETINT(parent,"CLEARING DUE FROM CODES",&selduefrom);
		clearfrom=stralloc(clearfromavl->libs[selduefrom]);
		FINDRSCGETINT(parent,"CLEARING CASH CODES",&selclearcash);
		clearcash=stralloc(clearcashavl->libs[selclearcash]);
		}
	} else {
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
		FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
		FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
		FINDRSCGETSTRING(parent,"PAYABLE CODE SEARCH",&paycode1);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCGETSTRING(parent,"DUE TO SEARCH",&dueto1);
		FINDRSCGETSTRING(parent,"CLEARING DUE FROM SEARCH",&clearfrom);
		FINDRSCGETSTRING(parent,"CLEARING CASH SEARCH",&clearcash);
		}
	}
	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selaccttype)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(venpmt->use_venpmt_fund)
	{
	if(FINDRSCGETINT(parent,"CLEARING DEFINITION TYPES",&selcdeftype)) return;
	cdeftype1=(short)selcdeftype;
	}
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	if(FINDRSCGETDOUBLE(parent,"[VENVCH][AMOUNT]",&amount)) return;
	procyrid=Ratoi(yearavl->libs[selyear]);
	FINDFLDSETINT(vchnum,"CALENDAR YEAR",venyrid);
	FINDFLDSETSTRING(vchnum,"ACCOUNT CODE",acctcode1);
	x=(short)selaccttype;
	FINDFLDSETSHORT(vchnum,"ACCOUNT TYPE",x);
	deftype1=(short)seldeftype;
	FINDFLDSETSHORT(vchnum,"DEFINITION TYPE",deftype1);
	if(venpmt->use_venpmt_fund)
	{
	FINDFLDSETSHORT(vchnum,"CLEARING DEFINITION TYPE",cdeftype1);
	}
	FINDFLDSETINT(vchnum,"FISCAL YEAR",procyrid);
	x=0;
	FINDFLDSETSHORT(vchnum,"CLEAR PROCESSING MONTH",x);
	if(!RDAstrcmp(debcode1,"Not Applicable") || 
		!RDAstrcmp(debcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(vchnum,"DEBIT CODE",NULL);
	} else {
		FINDFLDSETSTRING(vchnum,"DEBIT CODE",debcode1);
	}
	if(!RDAstrcmp(credcode1,"Not Applicable") || 
		!RDAstrcmp(credcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(vchnum,"CREDIT CODE",NULL);
	} else {
		FINDFLDSETSTRING(vchnum,"CREDIT CODE",credcode1);
	}
	if(!RDAstrcmp(paycode1,"Not Applicable") ||
		!RDAstrcmp(paycode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(vchnum,"PAYABLE CODE",NULL);
	} else {
		FINDFLDSETSTRING(vchnum,"PAYABLE CODE",paycode1);
	}
	if(venpmt->use_venpmt_fund)
	{
	if(!RDAstrcmp(dueto1,"Not Applicable") ||
		!RDAstrcmp(dueto1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(vchnum,"DUE TO",NULL);
	} else {
		FINDFLDSETSTRING(vchnum,"DUE TO",dueto1);
	}
	if(!RDAstrcmp(clearfrom,"Not Applicable") ||
		!RDAstrcmp(clearfrom,"No Accounts Available"))
	{
		FINDFLDSETSTRING(vchnum,"CLEARING DUE FROM",NULL);
	} else {
		FINDFLDSETSTRING(vchnum,"CLEARING DUE FROM",clearfrom);
	}
	if(!RDAstrcmp(clearcash,"Not Applicable") ||
		!RDAstrcmp(clearcash,"No Accounts Available"))
	{
		FINDFLDSETSTRING(vchnum,"CLEARING CASH",NULL);
	} else {
		FINDFLDSETSTRING(vchnum,"CLEARING CASH",clearcash);
	}
	}
	FINDFLDSETSTRING(vchnum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(vchnum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(vchnum,"SOURCE USER VERIFIED",deleteflag);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	if((x=FINDRSC(parent,"[VENVCH][ENTRY DATE]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
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
			FINDFLDSETSTRING(vchnum,"ENTRY DATE",date);
		}
	} else {
		FINDFLDSETSTRING(vchnum,"ENTRY DATE",date);
	}
        if(date!=NULL) Rfree(date);
	if((x=FINDRSC(parent,"[VENVCH][ENTRY TIME]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
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
			FINDFLDSETSTRING(vchnum,"ENTRY TIME",timex);
		}
	} else {
		FINDFLDSETSTRING(vchnum,"ENTRY TIME",timex);
	}
	if(timex!=NULL) Rfree(timex);
	if(ADVRECORDsec(vchnum,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(vchnum,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  ADVRECORDsec() failed without a description returned!");
		}
	} else {
		if(!WRTTRANS(vchnum,1,"TRANSACTION NUMBER",NULL))
		{
			FINDFLDGETSHORT(vchnum,"ACCOUNT TYPE",&accttype);
			switch(accttype)
			{
				case EXP_ACCT:
					total_exp+=amount;
					FINDFLDGETSTRING(vchnum,"DEBIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_deb+=amount;
						Rfree(acct);
					}
					FINDFLDGETSTRING(vchnum,"CREDIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_cred+=amount;
						Rfree(acct);
					}
					break;
				case REV_ACCT:
					total_rev+=amount;
					FINDFLDGETSTRING(vchnum,"CREDIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_cred+=amount;
						Rfree(acct);
					}
					FINDFLDGETSTRING(vchnum,"DEBIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						total_deb+=amount;
						Rfree(acct);
					}
					break;
				case BAL_ACCT:
					if(amount>0.0) total_deb+=amount;
					else total_cred-=amount;
					FINDFLDGETSTRING(vchnum,"CREDIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						if(amount>0.0) total_cred+=amount;
						else total_deb-=amount;
						Rfree(acct);
					}
					break;
				default:
					prterr("Invalid Account Type [%d]",accttype);
					break;
			}
			FINDRSCSETDOUBLE(parent,"TOTAL EXPENDITURE",total_exp);
			FINDRSCSETDOUBLE(parent,"TOTAL REVENUE",total_rev);
			FINDRSCSETDOUBLE(parent,"TOTAL DEBITS",total_deb);
			FINDRSCSETDOUBLE(parent,"TOTAL CREDITS",total_cred);
			makedflist();
/*
			FINDFLDGETSHORT(vchnum,"PROCESSING MONTHS",&selmonth);
*/
			FINDFLDGETSHORT(vchnum,"FISCAL PROCESSING MONTHS",&selmonth);
			switch(selaccttype)
			{
				case EXP_ACCT:
					if(credcode1!=NULL && RDAstrlen(credcode1)>0)
					{
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(vchnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",eyrnum,byrnum,procyrid,deftype1,"EXPENDITURES",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(vchnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",eyrnum,byrnum,procyrid,deftype1,"EXPENDITURES",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
					} else {
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(vchnum,"ACCOUNT CODE","DEBIT CODE",NULL,"PAYABLE CODE",eyrnum,byrnum,procyrid,deftype1,"EXPENDITURES",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(vchnum,"ACCOUNT CODE","DEBIT CODE",NULL,"PAYABLE CODE",eyrnum,byrnum,procyrid,deftype1,"EXPENDITURES",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
					}
					break;
				case REV_ACCT:
					if(debcode1!=NULL && RDAstrlen(debcode1)>0)
					{
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(vchnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",ryrnum,byrnum,procyrid,deftype1,"REVENUE",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(vchnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",ryrnum,byrnum,procyrid,deftype1,"REVENUE",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
					} else {
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(vchnum,"ACCOUNT CODE",NULL,"CREDIT CODE","PAYABLE CODE",ryrnum,byrnum,procyrid,deftype1,"REVENUE",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(vchnum,"ACCOUNT CODE",NULL,"CREDIT CODE","PAYABLE CODE",ryrnum,byrnum,procyrid,deftype1,"REVENUE",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
					}
					break;
				case BAL_ACCT:
					if(amount>0.0)
					{
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(vchnum,NULL,"ACCOUNT CODE","CREDIT CODE","PAYABLE CODE",(-1),byrnum,procyrid,deftype1,"",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(vchnum,NULL,"ACCOUNT CODE","CREDIT CODE","PAYABLE CODE",(-1),byrnum,procyrid,deftype1,"",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
					} else {
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(vchnum,NULL,"CREDIT CODE","ACCOUNT CODE","PAYABLE CODE",(-1),byrnum,procyrid,deftype1,"",FISCALPERIOD[selmonth],-amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(vchnum,NULL,"CREDIT CODE","ACCOUNT CODE","PAYABLE CODE",(-1),byrnum,procyrid,deftype1,"",CALENDAR_MONTHS[selmonth],-amount,SCRNvirtualSubData,parent);
						}
					}
					break;
				default:
					prterr("Error Invalid Account Type [%d]",selaccttype);
					break;
			}
			if(venpmt->use_venpmt_fund)
			{
				if(venpmt->use_fiscal_months==TRUE)
				{
					ADVupdateundist(vchnum,NULL,NULL,NULL,"DUE TO",(-1),byrnum,procyrid,deftype1,"",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
					ADVupdateundist(vchnum,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),byrnum,procyrid,cdeftype1,"",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
					ADVupdateundist(vchnum,NULL,"CLEARING CASH","CLEARING DUE FROM",NULL,(-1),byrnum,procyrid,cdeftype1,"",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
				} else {
					ADVupdateundist(vchnum,NULL,NULL,NULL,"DUE TO",(-1),byrnum,procyrid,deftype1,"",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
					ADVupdateundist(vchnum,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),byrnum,procyrid,cdeftype1,"",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
					ADVupdateundist(vchnum,NULL,"CLEARING CASH","CLEARING DUE FROM",NULL,(-1),byrnum,procyrid,cdeftype1,"",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
				}
			}
		} else {
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=Rmalloc(RDAstrlen(MODULENAME(vchnum))+RDAstrlen(FILENAME(vchnum))+139);
			sprintf(errordesc,"The write function failed to the file [%s][%s], the record and all subfiles that you attempted to save were not created.  Please try again.",MODULENAME(vchnum),FILENAME(vchnum));
			ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
			prterr("Error Couldn't Write Record: [%s]",errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		}
	}
	updatersrc(parent,"TOTAL EXPENDITURE");
	updatersrc(parent,"TOTAL REVENUE");
	updatersrc(parent,"TOTAL DEBITS");
	updatersrc(parent,"TOTAL CREDITS");
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS VOUCHER'S",(prevvch->numlibs-1),
		prevvch))
	{
		updatersrc(parent,"PREVIOUS VOUCHER'S");
	}
}
static void quit_check(RDArsrc *parent)
{
	if(total_deb!=total_cred)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","OUT OF BALANCE WARNING",
			"Exiting ADD VENDOR VOUCHERS with the entries added this session will make the Balance Sheet Out of Balance.  The TOTAL DEBITS entered this session are not equivalent to the TOTAL CREDITS.",
			quit_record,NULL,FALSE,parent);

	} else if(total_deb!=invoice_total)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVOICE BALANCE WARNING",
			"Exiting ADD VENDOR VOUCHERS with the total of vouchers out of balance with the total invoice.  Please correct this before continuing.",
			quit_record,NULL,FALSE,parent);
	} else {
		quit_record(parent);
	}
}
static void quit_record(RDArsrc *parent)
{
	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist); 
	}
	if(parent!=NULL)
	{
		killwindow(parent);
		free_rsrc(parent);
	}
	if(ACCTtypes!=NULL) freeapplib(ACCTtypes);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(clearDEFtypes!=NULL) freeapplib(clearDEFtypes);
	if(prevvch!=NULL) freeapplib(prevvch);
	if(finsetup!=NULL) free_finmgt(finsetup);
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(paycode1!=NULL) Rfree(paycode1);
}
static void setyear(RDArsrc *parent)
{
	int procyearid=0,yearno=0;

	ZERNRD(finyrnum);
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&yearno)) return;
	procyearid=Ratoi(yearavl->libs[yearno]);
	FINDFLDSETINT(finyrnum,"FISCAL YEAR",procyearid);
	FINDFLDSETINT(vchnum,"FISCAL YEAR",procyearid);
	if(ADVEQLNRDsec(finyrnum,1,SCRNvirtualSubData,parent)) KEYNRD(finyrnum,1);
	updatefilerecord2rsrc(finyrnum,parent);
	updateSCRNvirtuals(parent);
}
static void changeyearcb(RDArsrc *parent)
{
	changeyear(parent,FALSE);
}
static void changeyear(RDArsrc *parent,short force)
{
	int selecteda=0,selectedyear=0,procyrid=0;

	FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(parent,"FISCAL YEARS",&selectedyear);
	if(selectedyear>=yearavl->numlibs) 
	{
		selectedyear=0;
		FINDRSCSETINT(parent,"FISCAL YEARS",selectedyear);
	}
	procyrid=Ratoi(yearavl->libs[selectedyear]);
	if(procyrid!=year_identification || selecteda!=account_type || force)
	{
		year_identification=(-1);
		account_type=(-1);
		definition_type=(-1);
		clear_definition_type=(-1);
		setyear(parent);
		changeDEFtype(parent,force);
		if(venpmt->use_venpmt_fund)
		{
			changeclearDEFtype(parent,force);
		}
		updateSCRNvirtuals(parent);
	}
}
static void changeclearCODElist(RDArsrc *parent)
{
	int selectedd=0,selyear=0,procyrid=0,yearid=0;
	short ef=0,deftype=0,deftype1=0;
	short keyn=0,keynumber=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL,*e=NULL,*actyp=NULL,*speedkeyname=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"CLEARING DEFINITION TYPES",&selectedd)) return;
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	procyrid=Ratoi(yearavl->libs[selyear]);
	deftype=(short)selectedd;
	FINDRSCSETDEFTYPE(parent,"CLEARING DUE FROM SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"CLEARING CASH SEARCH",deftype);
	if(AddList==TRUE)
	{
		if((selectedd!=clear_definition_type ||
			year_identification!=procyrid) && AddList)
		{
			if(clearfromavl!=NULL) freeapplib(clearfromavl);
			clearfromavl=APPlibNEW();
			if(clearcashavl!=NULL) freeapplib(clearcashavl);
			clearcashavl=APPlibNEW();
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			if((keyn=KEYNUMBER(byrnum,"RDA FINBYR KEY"))!=(-1))
			{
				keynumber=keyn;
			} else {
				keynumber=1;
			}
			ef=ADVGTENRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
			e=stralloc("Updating Debit and Credit Accounts Lists");
#ifdef DIAGNOSTIC_SCREENS
			tmprsrc=diagscrn(byrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
#else 
			tmprsrc=NULL;
#endif
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
					if(yearid!=procyrid) break;
				} 
				if(yearid==procyrid) 
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
							if(account_code!=NULL && !deleteflag && active) 
							{	
								FINDFLDGETSTRING(balnum,"REFERENCE TYPE",
									&baltype);
								if(!RDAstrcmp(baltype,"CASH"))
								{
									addAPPlib(clearcashavl,account_code);
								} else if(!RDAstrcmp(baltype,"DUE TO"))
								{
									addAPPlib(clearfromavl,account_code);
								}
							}
						}
					}
				}
				if(account_code!=NULL) Rfree(account_code);
				if(baltype!=NULL) Rfree(baltype);
				ef=ADVNXTNRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,listupdated);
		
			}
        		if(tmprsrc!=NULL)
        		{
                		killwindow(tmprsrc);
                		free_rsrc(tmprsrc);
        		}
			clear_definition_type=selectedd;
			year_identification=procyrid;
		}
	}
	if(account_code!=NULL) Rfree(account_code);
	if(baltype!=NULL) Rfree(baltype); 
	if(actyp!=NULL) Rfree(actyp);
	if(speedkeyname!=NULL) Rfree(speedkeyname);
}
static void changeCODElist(RDArsrc *parent)
{
	int selecteda=0,selectedd=0,selyear=0,procyrid=0,yearid=0;
	short ef=0,accttype=0,deftype=0,deftype1=0,tempyrfile=0,tempaccfile=0;
	short keyn=0,keynumber=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL,*e=NULL,*actyp=NULL,*speedkeyname=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	procyrid=Ratoi(yearavl->libs[selyear]);
	accttype=(short)selecteda;
	deftype=(short)selectedd;
	FINDRSCSETACCTTYPE(parent,"ACCOUNT CODE SEARCH",accttype);
	FINDRSCSETDEFTYPE(parent,"ACCOUNT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEBIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"CREDIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"PAYABLE CODE SEARCH",deftype);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCSETDEFTYPE(parent,"DUE TO SEARCH",deftype);
	}
	if(AddList==TRUE)
	{
		if((selecteda!=account_type || selectedd!=definition_type ||
			year_identification!=procyrid) && AddList)
		{
			if(debitavl!=NULL) freeapplib(debitavl);
			debitavl=APPlibNEW();
			if(creditavl!=NULL) freeapplib(creditavl);
			creditavl=APPlibNEW();
			if(payableavl!=NULL) freeapplib(payableavl);
			payableavl=APPlibNEW();
			if(duetoavl!=NULL) freeapplib(duetoavl);
			duetoavl=APPlibNEW();
			if(acctavl!=NULL) freeapplib(acctavl);
			acctavl=APPlibNEW();
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
			ZERNRD(tempyrfile);
			FINDFLDSETSHORT(tempyrfile,"DEFINITION TYPE",deftype);
			FINDFLDSETINT(tempyrfile,"FISCAL YEAR",procyrid);
			if((keyn=KEYNUMBER(tempyrfile,speedkeyname))!=(-1))
			{
				keynumber=keyn;
			} else {
				keynumber=1;
			}
			ef=ADVGTENRDsec(tempyrfile,keynumber,SCRNvirtualSubData,parent);
			e=Rmalloc(RDAstrlen(actyp)+30);
			sprintf(e,"Updating %s Accounts Available",actyp);
#ifdef DIAGNOSTIC_SCREENS
			tmprsrc=diagscrn(tempyrfile,"DIAGNOSTIC SCREEN",module,e,NULL);
#else 
			tmprsrc=NULL;
#endif
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
				FINDFLDGETSHORT(tempyrfile,"DEFINITION TYPE",&deftype1); 
				if(deftype!=deftype1) break;
				FINDFLDGETINT(tempyrfile,"FISCAL YEAR",&yearid);
				FINDFLDGETSTRING(tempyrfile,"ACCOUNT CODE",&account_code);
				if(keyn!=(-1)) if(yearid!=procyrid) break;
/* I don't know what's going on here? Think ther might be something 
   wrong with the yearid / procyrid stuff here, please verify */
				if(yearid==procyrid)
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
							if(account_code!=NULL && !deleteflag && active)
							{	
								addAPPlib(acctavl,account_code);
								listupdated=TRUE;
							}
						}
					}
				} /* end else */
				if(account_code!=NULL) Rfree(account_code);
				account_code=NULL;
				ef=ADVNXTNRDsec(tempyrfile,keynumber,SCRNvirtualSubData,parent);
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,listupdated);
			}
     	   		if(tmprsrc!=NULL)
      	  		{
           			killwindow(tmprsrc);
         			free_rsrc(tmprsrc);
        		}
			switch(selecteda)
			{
				case EXP_ACCT: /* Expenditures */
					ZERNRD(byrnum);
					FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
					FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
					if((keyn=KEYNUMBER(byrnum,"RDA FINBYR KEY"))!=(-1))
					{
						keynumber=keyn;
					} else {
						keynumber=1;
					}
					ef=ADVGTENRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
					e=stralloc("Updating Debit and Credit Accounts Lists");
#ifdef DIAGNOSTIC_SCREENS
					tmprsrc=diagscrn(byrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
#else 
					tmprsrc=NULL;
#endif
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
							if(yearid!=procyrid) break;
						} 
						if(yearid==procyrid) 
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
									if(account_code!=NULL && !deleteflag && active) 
									{	
										FINDFLDGETSTRING(balnum,"REFERENCE TYPE",
											&baltype);
										if(!RDAstrcmp(baltype,"EXPENDITURE"))
										{
											addAPPlib(debitavl,account_code);
										} else if(!RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
										{
											addAPPlib(payableavl,account_code);
										} else if(!RDAstrcmp(baltype,"DUE TO"))
										{
											if(venpmt->use_venpmt_fund) addAPPlib(duetoavl,account_code);
										} else addAPPlib(creditavl,account_code);
									}
								}
							}
						}
						if(account_code!=NULL) Rfree(account_code);
						if(baltype!=NULL) Rfree(baltype);
						ef=ADVNXTNRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,listupdated);
		
					}
        				if(tmprsrc!=NULL)
        				{
                				killwindow(tmprsrc);
                				free_rsrc(tmprsrc);
        				}
					addAPPlib(creditavl,"Not Applicable");
					break;
				case REV_ACCT: /* Revenue */
					ZERNRD(byrnum);
					FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
					FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
					if((keyn=KEYNUMBER(byrnum,"RDA FINBYR KEY"))!=(-1))
					{
						keynumber=keyn;
					} else {
						keynumber=1;
					}
					ef=ADVGTENRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
					e=stralloc("Updating Debit and Credit Accounts Lists");
#ifdef DIAGNOSTIC_SCREENS
					tmprsrc=diagscrn(byrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
#else 
					tmprsrc=NULL;
#endif
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
						FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",
							&deftype);
						if(deftype!=selectedd) break;
						FINDFLDGETINT(byrnum,"FISCAL YEAR",
							&yearid);
						if(keyn!=(-1))
						{
							if(yearid!=procyrid) break;
						} 
						if(yearid==procyrid) 
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
									FINDFLDGETCHAR(byrnum,"DELETEFLAG",
										&deleteflag);
									FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
									if(account_code!=NULL&&!deleteflag&&active) 
									{		
										FINDFLDGETSTRING(balnum,"REFERENCE TYPE",
											&baltype);
										if(!RDAstrcmp(baltype,"CASH"))
										{
											addAPPlib(creditavl,account_code);
										} else if(!RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
										{
											addAPPlib(payableavl,account_code);
										} else if(!RDAstrcmp(baltype,"DUE TO"))
										{
											if(venpmt->use_venpmt_fund) addAPPlib(duetoavl,account_code);
										} else addAPPlib(debitavl,account_code);
										listupdated=TRUE;
									}
								}
							}
						}
						if(account_code!=NULL) Rfree(account_code);
						if(baltype!=NULL) Rfree(baltype);
						ef=ADVNXTNRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,listupdated);
					}
     	   				if(tmprsrc!=NULL)
        				{
              			  		killwindow(tmprsrc);
              			  		free_rsrc(tmprsrc);
        				}
					addAPPlib(debitavl,"Not Applicable");
					break;
				case BAL_ACCT: /* Balance Sheet */
					ZERNRD(byrnum);
					FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
					FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
					if((keyn=KEYNUMBER(byrnum,"RDA FINBYR KEY"))!=(-1))
					{
						keynumber=keyn;
					} else {
						keynumber=1;
					}
					ef=ADVGTENRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
					e=stralloc("Updating Debit and Credit Accounts Lists");
#ifdef DIAGNOSTIC_SCREENS
					tmprsrc=diagscrn(byrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
#else 
					tmprsrc=NULL;
#endif
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
						FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",&deftype);
						if(deftype!=selectedd) break;
						FINDFLDGETINT(byrnum,"FISCAL YEAR",&yearid);
						if(keyn!=(-1))
						{
							if(yearid!=procyrid) break;
						} 
						if(yearid==procyrid) 
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
									FINDFLDGETCHAR(byrnum,"DELETEFLAG",
										&deleteflag);
									FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
									if(account_code!=NULL&&!deleteflag&&active) 
									{		
										FINDFLDGETSTRING(balnum,"REFERENCE TYPE",
											&baltype);
										if(!RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
										{
											addAPPlib(payableavl,account_code);
										} else if(!RDAstrcmp(baltype,"DUE TO"))
										{
											if(venpmt->use_venpmt_fund) addAPPlib(duetoavl,account_code);
										} else addAPPlib(creditavl,account_code);
										listupdated=TRUE;
									}
								}
							}
						}
						if(account_code!=NULL) { Rfree(account_code); account_code=NULL; }
						if(baltype!=NULL) { Rfree(baltype); baltype=NULL; }
						ef=ADVNXTNRDsec(byrnum,keynumber,SCRNvirtualSubData,parent);
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,listupdated);
					}
    					if(tmprsrc!=NULL)
    		    			{
		                		killwindow(tmprsrc);
                				free_rsrc(tmprsrc);
        				}
					addAPPlib(debitavl,"Not Applicable");
					break;
				default:
					prterr("Invalid Account Type [%d]",selecteda);
					break;
			}
			account_type=selecteda;
			definition_type=selectedd;
			year_identification=procyrid;
		}
	}
	if(account_code!=NULL) Rfree(account_code);
	if(baltype!=NULL) Rfree(baltype); 
	if(actyp!=NULL) Rfree(actyp);
	if(speedkeyname!=NULL) Rfree(speedkeyname);
}
static void changeclearDEFtype(RDArsrc *parent,short force)
{
	int seld=0,selc=0;
	int selecteddef=0;

	FINDRSCGETINT(parent,"CLEARING DEFINITION TYPES",&selecteddef);
	if(selecteddef!=clear_definition_type || force)
	{
		year_identification=(-1);
		if(AddList)
		{
			FINDRSCGETINT(parent,"CLEARING DUE FROM CODES",&seld);
			FINDRSCGETINT(parent,"CLEARING CASH CODES",&selc);
		}
		changeclearCODElist(parent);
		if(AddList)
		{
			if(clearfromavl->numlibs<1)
			{
				addAPPlib(clearfromavl,"No Accounts Available"); 
			}
			if(seld>=clearfromavl->numlibs) seld=0;
			if(!FINDRSCLISTAPPlib(parent,"CLEARING DUE FROM CODES",seld,clearfromavl))
			{
				updatersrc(parent,"CLEARING DUE FROM CODES");
			}
			if(clearcashavl->numlibs<1)
			{
				addAPPlib(clearcashavl,"No Accounts Available");
			}
			if(selc>=clearcashavl->numlibs) selc=0;
			if(!FINDRSCLISTAPPlib(parent,"CLEARING CASH CODES",selc,clearcashavl))
			{
				updatersrc(parent,"CLEARING CASH CODES");
			}
		}
		clearfromnum=5;
		getsearchstr(parent,&clearfromnum); 
		clearcashnum=6;
		getsearchstr(parent,&clearcashnum);
		getacctdesc(parent);
	}
}
static void changeDEFtype(RDArsrc *parent,short force)
{
	int sela=0,seld=0,selc=0,selp=0,self=0;
	int selecteddef=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&selecteddef);
	if(selecteddef!=definition_type || force)
	{
		year_identification=(-1);
		if(AddList)
		{
			FINDRSCGETINT(parent,"ACCOUNT CODES",&sela);
			FINDRSCGETINT(parent,"DEBIT CODES",&seld);
			FINDRSCGETINT(parent,"CREDIT CODES",&selc);
			FINDRSCGETINT(parent,"PAYABLE CODES",&selp);
			if(venpmt->use_venpmt_fund)
			{
				FINDRSCGETINT(parent,"DUE TO CODES",&self);
			}
		}
		changeCODElist(parent);
		if(AddList)
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
			if(payableavl->numlibs<1)
			{
				addAPPlib(payableavl,"No Accounts Available");
			}
			if(selp>=payableavl->numlibs) selp=0;
			if(!FINDRSCLISTAPPlib(parent,"PAYABLE CODES",selp,payableavl))
			{
				updatersrc(parent,"PAYABLE CODES");
			}
			if(venpmt->use_venpmt_fund)
			{
			if(duetoavl->numlibs<1)
			{
				addAPPlib(duetoavl,"No Accounts Available");
			}
			if(self>=duetoavl->numlibs) self=0;
			if(!FINDRSCLISTAPPlib(parent,"DUE TO CODES",self,duetoavl))
			{
				updatersrc(parent,"DUE TO CODES");
			}
			}
		}
		debitcodenum=1;
		getsearchstr(parent,&debitcodenum);
		creditcodenum=2;
		getsearchstr(parent,&creditcodenum); 
		payablecodenum=3;
		getsearchstr(parent,&payablecodenum);
		if(venpmt->use_venpmt_fund)
		{
			duetonum=4;
			getsearchstr(parent,&duetonum);
			getacctdesc(parent);
		}
	}
}
static void clearDEFtypecb(RDArsrc *parent)
{
	FINDRSCSETSTRING(parent,"CLEARING DUE FROM SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CLEARING CASH SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CLEARING DUE FROM DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CLEARING CASH DESCRIPTION",NULL);
	changeclearDEFtype(parent,FALSE);
}
static void DEFtypecb(RDArsrc *parent)
{
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"PAYABLE CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"PAYABLE CODE DESCRIPTION",NULL);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCSETSTRING(parent,"DUE TO SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DUE TO DESCRIPTION",NULL);
	}
	changeDEFtype(parent,FALSE);
}
static void ACCTtypecb(RDArsrc *parent)
{
	int sdef=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&sdef);
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"PAYABLE CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"PAYABLE CODE DESCRIPTION",NULL);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCSETSTRING(parent,"DUE TO SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DUE TO DESCRIPTION",NULL);
	}
	changeACCTtype(parent);
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	if(sdef>=DEFtypes->numlibs) sdef=0;
	if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",sdef,DEFtypes))
	{
		updatersrc(parent,"DEFINITION TYPES");
	}
	changeDEFtype(parent,FALSE);
}
static void changeACCTtype(RDArsrc *parent)
{
	int selectedd=0,acctype=0;
	int x;
	RDAacct *acct;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&acctype)) return;
	if(acctype!=account_type)
	{
		definition_type=(-1);
		year_identification=(-1);
		if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
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
				break;
			default:
				prterr("Invalid Account Type [%d]",acctype);
				break;
		}
		if(DEFtypes->numlibs<1)
		{
			addAPPlib(DEFtypes,"Not Available");
		}
		if(selectedd>=DEFtypes->numlibs) selectedd=0;
		if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",selectedd,DEFtypes))
		{
			updatersrc(parent,"DEFINITION TYPES");
		}
	}
	if(acctype==BAL_ACCT)
	{
		FINDRSCSETEDITABLE(parent,"DEBIT CODE SEARCH",FALSE);
		FINDRSCSETSENSITIVE(parent,"DEBIT CODE SEARCH",FALSE);
		if(AddList)
		{
			FINDRSCSETEDITABLE(parent,"DEBIT CODES",FALSE);
			FINDRSCSETSENSITIVE(parent,"DEBIT CODES",FALSE);
		}
	} else {
		FINDRSCSETSENSITIVE(parent,"DEBIT CODE SEARCH",TRUE);
		FINDRSCSETEDITABLE(parent,"DEBIT CODE SEARCH",TRUE);
		if(AddList)
		{
			FINDRSCSETSENSITIVE(parent,"DEBIT CODES",TRUE);
			FINDRSCSETEDITABLE(parent,"DEBIT CODES",TRUE);
		}
	}
}
static int getyears(RDAfinmgt *finsetup,RDArsrc *parent)
{
        short ef;
        int selyear=0,curryear=0,year=0;
        char delflag=FALSE/*,*yearstr=NULL*/;

	curryear=GETCURRENTFISCALYEAR(finsetup->month);
        if(yearavl!=NULL) freeapplib(yearavl);
        yearavl=APPlibNEW();
        ZERNRD(finyrnum);
        ef=ADVFRSNRDsec(finyrnum,1,SCRNvirtualSubData,parent);
        while(!ef)
        {
                FINDFLDGETCHAR(finyrnum,"DELETEFLAG",&delflag);
                if(!delflag)
                {
                FINDFLDGETCHAR(finyrnum,"ACTIVE",&delflag);
                if(delflag)
                {
                        FINDFLDGETINT(finyrnum,"FISCAL YEAR",&year);
                        sprintf(stemp,"%-4d",year);
                        addAPPlib(yearavl,stemp);
			if(year==curryear) selyear=((yearavl->numlibs)-1);
                }
		}
                ef=ADVNXTNRDsec(finyrnum,1,SCRNvirtualSubData,parent);
        }
        return(selyear);
}
static void SETFILES(RDArsrc *parent)
{
        getsupportingfiles(parent);
}
static void SETFILEScb(RDArsrc *parent)
{
        readallwidgets(parent);
        IncVirRsrcstoFile(parent);
	SETFILES(parent);
        updateSCRNvirtuals(parent);
}
static void getsupportingfiles(RDArsrc *parent)
{
	char *bankid=NULL;

	if(invnum!=(-1))
	{
		ZERNRD(invnum);
		FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
		FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",invid);
		if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,parent)) KEYNRD(invnum,1);
		filerecord2rsrc(invnum,parent);
	}
	if(yearnum!=(-1))
	{
		FINDFLDSETSTRING(yearnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(yearnum,"CALENDAR YEAR",venyrid);
		if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,parent)) KEYNRD(yearnum,1);
		filerecord2rsrc(yearnum,parent);
	}
	if(filenum!=(-1))
	{
		FINDFLDSETSTRING(filenum,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,parent)) KEYNRD(filenum,1);
		filerecord2rsrc(filenum,parent);
	}
	if(banknum!=(-1))
	{
		FINDFLDGETSTRING(vchnum,"BANK IDENTIFICATION",&bankid);
		FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",bankid);
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,parent)) KEYNRD(banknum,1);
		filerecord2rsrc(banknum,parent);
		if(bankid!=NULL) Rfree(bankid);
	}
}
void addvouchers(void)
{
	short nofields=0,nokeys=0,x,edit_rsrc=TRUE;
	int selecteda=0,selectedd=0,start=0,presentyear=0;
	short deftype=0,accttype=0;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL;
	char *name=NULL/*,editable=FALSE*/;
	int w=0;
	RDAacct *acct=NULL;

	nokeys=NUMKEYS(vchnum);
	keys=KEYPOINTER(vchnum);
	nofields=NUMFLDS(vchnum);
	fields=FLDPOINTER(vchnum);
	if(AddList)
	{
		parent=RDArsrcNEW(module,"ADD VENDOR VOUCHERS");
	} else {
		parent=RDArsrcNEW(module,"ADD VENDOR VOUCHERS W/O LISTS");
	}
	definelist=RDArsrcNEW(module,"ADD VOUCHER DEFINE LIST");
	addDFincvir(parent,"VENPMT","VENMSR",NULL,filenum);
	addDFincvir(parent,"VENPMT","VENVCH",NULL,vchnum);
	addDFincvir(parent,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(parent,"VENPMT","VENYR",NULL,yearnum);
	if(banknum!=(-1))
	{
		addDFincvir(parent,"BNKREC","BNKACC",NULL,banknum);
	}
	addDFincvir(parent,"FINMGT","FINBACC",NULL,balnum);
	addDFincvir(parent,"FINMGT","FINRACC",NULL,revnum);
	addDFincvir(parent,"FINMGT","FINEACC",NULL,expnum);
	addDFincvir(parent,"FINMGT","FINRYR",NULL,ryrnum);
	addDFincvir(parent,"FINMGT","FINBYR",NULL,byrnum);
	addDFincvir(parent,"FINMGT","FINEYR",NULL,eyrnum);
	addDFincvir(parent,"FINMGT","FINYEAR",NULL,finyrnum);
	addDFincvir(definelist,"VENPMT","VENVCH",NULL,vchnum);
	addDFincvir(definelist,"BNKREC","BNKACC",NULL,banknum);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	if(invnum!=(-1))
	{
		file2rsrc(invnum,parent,FALSE);
	}
	if(yearnum!=(-1))
	{
		file2rsrc(yearnum,parent,FALSE);
	}
	if(filenum!=(-1))
	{
		file2rsrc(filenum,parent,FALSE);
	}
	if(banknum!=(-1))
	{
		file2rsrc(banknum,parent,FALSE);
	}
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"VENDOR IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(vchnum,fldx->name,&edit_rsrc))
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(vchnum)));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(vchnum)));
					}
					sprintf(name,"[%s][%s]",FILENAME(vchnum),fldx->name);
					makefieldrsrc(parent,name,fldx->type,
						fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"INVOICE IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(vchnum,fldx->name,&edit_rsrc))
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(vchnum)));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(vchnum)));
					}
					sprintf(name,"[%s][%s]",FILENAME(vchnum),fldx->name);
					makefieldrsrc(parent,name,fldx->type,
						fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"CALENDAR YEAR"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(vchnum,fldx->name,&edit_rsrc))
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(vchnum)));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(vchnum)));
					}
					sprintf(name,"[%s][%s]",FILENAME(vchnum),fldx->name);
					makefieldrsrc(parent,name,fldx->type,
						fldx->len,FALSE);
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
					addlstrsrc(parent,"ACCOUNT TYPES",
						&start,edit_rsrc,ACCTtypecb,
						ACCTtypes->numlibs,
						&ACCTtypes->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(DEFtypes==NULL) DEFtypes=APPlibNEW();
					addlstrsrc(parent,"DEFINITION TYPES",
						&start,edit_rsrc,DEFtypecb,
						DEFtypes->numlibs,
						&DEFtypes->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"ACCOUNT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(acctavl==NULL) acctavl=APPlibNEW();
						start=0;
						addlstrsrc(parent,"ACCOUNT CODES",
							&start,edit_rsrc,
							getacctdesc,
							acctavl->numlibs,
							&acctavl->libs,NULL);
					}
					addstdrsrc(parent,"ACCOUNT DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"ACCOUNT CODE SEARCH",EXPENDITURE,40,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"ACCOUNT CODE SEARCH",acctsearch,&acctcodenum);
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
						if(debitavl==NULL) debitavl=APPlibNEW();
						addlstrsrc(parent,"DEBIT CODES",
							&start,edit_rsrc,getsearchstr,
							debitavl->numlibs,
							&debitavl->libs,
							&debitcodenum);
					}
					addstdrsrc(parent,"DEBIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEBIT CODE SEARCH",BALANCESHEET,40,NULL,edit_rsrc);
					debitcodenum=1;
					FINDRSCSETFUNC(parent,"DEBIT CODE SEARCH",acctsearch,&debitcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"CREDIT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(creditavl==NULL) creditavl=APPlibNEW();
						start=0;
						creditcodenum=2;
						addlstrsrc(parent,"CREDIT CODES",
							&start,edit_rsrc,getsearchstr,
							creditavl->numlibs,
							&creditavl->libs,
							&creditcodenum);
					}
					addstdrsrc(parent,"CREDIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"CREDIT CODE SEARCH",BALANCESHEET,40,NULL,edit_rsrc);
					creditcodenum=2;
					FINDRSCSETFUNC(parent,"CREDIT CODE SEARCH",acctsearch,&creditcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"PAYABLE CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(payableavl==NULL) payableavl=APPlibNEW();
						start=0;
						payablecodenum=2;
						addlstrsrc(parent,"PAYABLE CODES",
							&start,edit_rsrc,getsearchstr,
							payableavl->numlibs,
							&payableavl->libs,
							&payablecodenum);
					}
					addstdrsrc(parent,"PAYABLE CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"PAYABLE CODE SEARCH",BALANCESHEET,40,NULL,edit_rsrc);
					payablecodenum=2;
					FINDRSCSETFUNC(parent,"PAYABLE CODE SEARCH",acctsearch,&payablecodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DUE TO"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(duetoavl==NULL) duetoavl=APPlibNEW();
						start=0;
						duetonum=4;
						addlstrsrc(parent,"DUE TO CODES",
							&start,edit_rsrc,getsearchstr,
							duetoavl->numlibs,
							&duetoavl->libs,
							&duetonum);
					}
					addstdrsrc(parent,"DUE TO DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DUE TO SEARCH",BALANCESHEET,40,NULL,edit_rsrc);
					duetonum=4;
					FINDRSCSETFUNC(parent,"DUE TO SEARCH",acctsearch,&duetonum);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"CLEARING DEFINITION TYPE"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(clearDEFtypes==NULL) clearDEFtypes=APPlibNEW();
					if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
					{
						for(w=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
							w<BALANCE_SHEET_ACCOUNT->num;++w,++acct)
						{
							addAPPlib(clearDEFtypes,acct->name);
						}
					}
					addlstrsrc(parent,"CLEARING DEFINITION TYPES",
						&start,edit_rsrc,clearDEFtypecb,
						clearDEFtypes->numlibs,
						&clearDEFtypes->libs,NULL);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"CLEARING DUE FROM"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(clearfromavl==NULL) clearfromavl=APPlibNEW();
						start=0;
						clearfromnum=5;
						addlstrsrc(parent,"CLEARING DUE FROM CODES",
							&start,edit_rsrc,getsearchstr,
							clearfromavl->numlibs,
							&clearfromavl->libs,
							&clearfromnum);
					}
					addstdrsrc(parent,"CLEARING DUE FROM DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"CLEARING DUE FROM SEARCH",BALANCESHEET,40,NULL,edit_rsrc);
					clearfromnum=5;
					FINDRSCSETFUNC(parent,"CLEARING DUE FROM SEARCH",acctsearch,&clearfromnum);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"CLEARING CASH"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(clearcashavl==NULL) clearcashavl=APPlibNEW();
						start=0;
						clearcashnum=6;
						addlstrsrc(parent,"CLEARING CASH CODES",
							&start,edit_rsrc,getsearchstr,
							clearcashavl->numlibs,
							&clearcashavl->libs,
							&clearcashnum);
					}
					addstdrsrc(parent,"CLEARING CASH DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"CLEARING CASH SEARCH",BALANCESHEET,40,NULL,edit_rsrc);
					clearcashnum=6;
					FINDRSCSETFUNC(parent,"CLEARING CASH SEARCH",acctsearch,&clearcashnum);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
			{
				presentyear=getyears(finsetup,parent);
				if(yearavl->numlibs<1)
				{
					ERRORDIALOG("FINYEAR Empty File","The FINYEAR file is an empty file.  There must exist at least 1 year on file to process vouchers.\n",NULL,TRUE);
					ShutdownSecurity();
					RDAAPPMAINLOOP();
					quit_record(parent);
					return;
				}
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(finyrnum,parent,FALSE);
					addlstrsrc(parent,"FISCAL YEARS",
						&presentyear,edit_rsrc,changeyearcb,
						yearavl->numlibs,
						&yearavl->libs,NULL);
				}
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,vchnum,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(vchnum)));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(vchnum)));
				}
				sprintf(name,"[%s][%s]",FILENAME(vchnum),fldx->name);
				FINDRSCSETFUNC(parent,name,SETFILEScb,NULL);

			}
		}
	}
	nofields=0;
	nokeys=0;
	if(name!=NULL) Rfree(name);
	getsupportingfiles(parent);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
        ReadRDAScrolledLists(parent);
	if(prevvch!=NULL) freeapplib(prevvch);
	prevvch=APPlibNEW();
	addAPPlib(prevvch,"No Vouchers added this session");
	addlstrsrc(parent,"PREVIOUS VOUCHER'S",&x,TRUE,NULL,prevvch->numlibs,
		&prevvch->libs,NULL);
	FINDRSCSETSTRING(parent,"[VENVCH][ENTRY DATE]",CURRENT_DATE);
	FINDRSCSETSTRING(parent,"[VENVCH][ENTRY TIME]",CURRENT_TIME);
	savelist(definelist,parent);
	addstdrsrc(parent,"INVOICE TOTAL",DOLLARS,20,&invoice_total,FALSE);
	addstdrsrc(parent,"TOTAL EXPENDITURE",DOLLARS,20,&total_exp,FALSE);
	addstdrsrc(parent,"TOTAL REVENUE",DOLLARS,20,&total_rev,FALSE);
	addstdrsrc(parent,"TOTAL DEBITS",DOLLARS,20,&total_deb,FALSE);
	addstdrsrc(parent,"TOTAL CREDITS",DOLLARS,20,&total_cred,FALSE);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_defaultscb,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addbtnrsrc(parent,"QUIT",TRUE,quit_check,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaultscb(parent);
	APPmakescrn(parent,TRUE,quit_record,NULL,FALSE);
	FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	accttype=(short)selecteda;
	deftype=(short)selectedd;
	FINDRSCSETACCTTYPE(parent,"ACCOUNT CODE SEARCH",accttype);
	FINDRSCSETDEFTYPE(parent,"ACCOUNT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEBIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"CREDIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"PAYABLE CODE SEARCH",deftype);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCGETINT(parent,"CLEARING DEFINITION TYPES",&selectedd);
	deftype=(short)selectedd;
	FINDRSCSETDEFTYPE(parent,"CLEARING DUE FROM SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"CLEARING CASH SEARCH",deftype);
	}
}
