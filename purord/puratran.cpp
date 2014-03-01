/* puratran.c - Add Purchase Order Transaction */
#define CLEARWDGT
#include <cstdio>

#include <mix.hpp>
#include <fin.hpp>
#include <venpmt.hpp>
#include <purord.hpp>
#define DIAGNOSTIC_SCREENS

extern short msrnum,linenum,accnum,trannum;
extern short expnum,revnum,balnum,vennum;
extern short byrnum,eyrnum,ryrnum,finyrnum;
extern short banknum;
extern RDAfinmgt *finsetup;
extern RDAvenpmt *venpmt;
extern RDApurord *purord;
extern int PO_COUNT;
extern char *POID,*module;
extern double po_total;
extern int yearid;
static double total_lineout=0.0,total_lines=0.0;
static char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL;
static char *defdebcode1=NULL,*defpaycode1=NULL,*defcredcode1=NULL;
static char *defdueto=NULL,*defclearfrom=NULL,*defclearcash=NULL;
static RDArsrc *definelist=NULL;
static APPlib *ACCTtypes=NULL;
static int lineno=0;
static APPlib *linesavl=NULL,*prevtran=NULL;
static double total_exp=0,total_rev=0,total_deb=0,total_cred=0;
static short acctcodenum=0,debitcodenum=1,creditcodenum=2;
static short defdebcodenum=3,defpaycodenum=4,defcredcodenum=5;
static short defduetonum=6,defclearfromnum=7,defclearcashnum=8;
static void ACCTtypecb(RDArsrc *),DEFtypecb(RDArsrc *);
static void changeACCTtype(RDArsrc *);
static void changeDEFtype(RDArsrc *);
static void changeclearDEFtype(RDArsrc *);
static void changeLINES(RDArsrc *);
static double getout_amt(RDArsrc *);
static double getpo_total(RDArsrc *);
static void getlines(RDArsrc *);
static void getacctdesc(RDArsrc *);
static void changeCODElist(RDArsrc *);
static void changeclearCODElist(RDArsrc *);
static void set_defaults(RDArsrc *);
static void set_defaultscb(RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *),savelist(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *),quit_record1(RDArsrc *);
static void save_check(RDArsrc *),quit_check(RDArsrc *);
static void acctsearch(RDArsrc *,short *);
static void getsearchstr(RDArsrc *,short *);
static void clearwidget(RDArsrc *,char *);
static short check_acctcode(RDArsrc *,short);
static void getsupportingrecords(RDArsrc *);
static void readscreen(RDArsrc *);
static void list2file(RDArsrc *);
static void SETFILES(RDArsrc *);
APPlib *acctavl=NULL,*debitavl=NULL,*creditavl=NULL;
APPlib *defdebavl=NULL,*defpayavl=NULL,*defcredavl=NULL;
APPlib *DEFtypes=NULL,*clearDEFtypes=NULL;
APPlib *defduetoavl=NULL,*defclearfromavl=NULL,*defclearcashavl=NULL;
int year_identification=(-1);
short clear_definition_type=(-1),definition_type=(-1),account_type=(-1);
void addtransactions(void);

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
	char *acct=NULL,*poid1=NULL;
	int yearid1=0,lineno1=0,acctno1=0;
	RDArsrc *tmprsrc=NULL;

	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(prevtran!=NULL) freeapplib(prevtran);
	prevtran=APPlibNEW();
#ifdef DIAGNOSTIC_SCREENS
	tmprsrc=diagscrn(trannum,"DIAGNOSTIC SCREEN",module,
		"Reading Undistributed PO Transactions",NULL);
#else 
	tmprsrc=NULL;
#endif
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED.","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that this screen is available in this module.  If not, call your installer.\n",NULL,NULL,TRUE,NULL);
			prterr("Error Cannot Create Screen [%s] [%s].",tmprsrc->module,tmprsrc->screen);
		} else ForceWindowUpdate(tmprsrc);
	}
	ZERNRD(trannum);
	FINDFLDSETINT(trannum,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(trannum,"PO IDENTIFICATION",POID);
	ef=ADVGTENRDsec(trannum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		if(poid1!=NULL) Rfree(poid1);
		FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid1);
		FINDFLDGETSTRING(trannum,"PO IDENTIFICATION",&poid1);
		if(RDAstrcmp(poid1,POID) || yearid1!=yearid) break;
		FINDFLDGETCHAR(trannum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDSETSTRING(accnum,"PO IDENTIFICATION",POID);
			FINDFLDSETINT(accnum,"FISCAL YEAR",yearid);
			FINDFLDGETINT(trannum,"LINE NUMBER",&lineno1);
			FINDFLDSETINT(accnum,"LINE NUMBER",lineno1);
			FINDFLDGETINT(trannum,"ACCOUNT NUMBER",&acctno1);
			FINDFLDSETINT(accnum,"ACCOUNT NUMBER",acctno1);
			if(ADVEQLNRDsec(accnum,1,SCRNvirtualSubData,parent))
			{
				KEYNRD(accnum,1);
			}
			FINDFLDGETSHORT(accnum,"ACCOUNT TYPE",&accttype);
			FINDFLDGETDOUBLE(trannum,"AMOUNT",&amount);
			FINDFLDGETINT(trannum,"LINE NUMBER",&lineno1);
			switch(accttype)
			{
				case EXP_ACCT:
					local_exp+=amount;
					FINDFLDGETSTRING(accnum,"DEBIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						local_deb+=amount;
						Rfree(acct);
					}
					FINDFLDGETSTRING(accnum,"CREDIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						local_cred+=amount;
						Rfree(acct);
					}
					break;
				case REV_ACCT:
					local_rev+=amount;
					FINDFLDGETSTRING(accnum,"DEBIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						local_deb+=amount;
						Rfree(acct);
					}
					FINDFLDGETSTRING(accnum,"CREDIT CODE",&acct);
					if(acct!=NULL && RDAstrlen(acct)>0)
					{
						local_cred+=amount;
						Rfree(acct);
					}
					break;
				case BAL_ACCT:
					if(amount>0.0) local_deb+=amount;
					else local_cred-=amount;
					FINDFLDGETSTRING(accnum,"CREDIT CODE",&acct);
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
			FINDFLDGETCHAR(trannum,"SOURCE USER VERIFIED",&verified);
			if(!verified)
			{
				FINDFLDGETSTRING(trannum,"SOURCE USER",&username);
				if(!RDAstrcmp(username,USERLOGIN))
				{
					makedflist();
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				} else {
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
				}
			}
		}
		ef=ADVNXTNRDsec(trannum,1,SCRNvirtualSubData,parent);
	}
	if(poid1!=NULL) Rfree(poid1);
	total_exp=local_exp;
	total_rev=local_rev;
	total_deb=local_deb;
	total_cred=local_cred;
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(prevtran->numlibs<1)
	{
		addAPPlib(prevtran,"No PO Transactions added this session.");
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS TRANSACTIONS",(prevtran->numlibs-1),
		prevtran))
	{
		updatersrc(parent,"PREVIOUS TRANSACTIONS");
	}
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(prevtran!=NULL)
	{
		if(prevtran->numlibs==1 && !RDAstrcmp(prevtran->libs[0],
			"No PO Transactions added this session."))
		{
			freeapplib(prevtran);
			prevtran=APPlibNEW();
		}
	} else {
		prevtran=APPlibNEW();
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
					field=FLDNRD(trannum,s);
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
	addAPPlib(prevtran,hold);
	++PO_COUNT;
	if(hold!=NULL) { Rfree(hold); hold=NULL; }
}
static void definelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static double getpo_total(RDArsrc *parent)
{
	short ef=0;
	double amount=0.0,line_totals=0.0;
	char deleteflag=FALSE;
	char *poid1=NULL;
	int yearid1=0;

	ZERNRD(linenum);
	FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
	ef=ADVGTENRDsec(linenum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		if(poid1!=NULL) Rfree(poid1);
		FINDFLDGETINT(linenum,"FISCAL YEAR",&yearid1);
		FINDFLDGETSTRING(linenum,"PO IDENTIFICATION",&poid1);
		if(RDAstrcmp(poid1,POID) || yearid1!=yearid) break;
		FINDFLDGETCHAR(linenum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETDOUBLE(linenum,"TOTAL",&amount);
			line_totals+=amount;
		}
		ef=ADVNXTNRDsec(linenum,1,SCRNvirtualSubData,parent);
	}
	if(poid1!=NULL) Rfree(poid1);
	return(line_totals);
}
static double getout_amt(RDArsrc *parent)
{
	short ef=0;
	double amount=0.0,trans_total=0.0;
	char deleteflag=FALSE;
	char *poid1=NULL;
	int yearid1=0,lineno1=0;

	ZERNRD(trannum);
	FINDFLDSETINT(trannum,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(trannum,"PO IDENTIFICATION",POID);
	FINDFLDSETINT(trannum,"LINE NUMBER",lineno);
	FINDFLDGETDOUBLE(linenum,"TOTAL",&total_lines);
	ef=ADVGTENRDsec(trannum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		if(poid1!=NULL) Rfree(poid1);
		FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid1);
		FINDFLDGETSTRING(trannum,"PO IDENTIFICATION",&poid1);
		FINDFLDGETINT(trannum,"LINE NUMBER",&lineno1);
		if(RDAstrcmp(poid1,POID) || yearid1!=yearid ||
			lineno1!=lineno) break;
		FINDFLDGETCHAR(trannum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETDOUBLE(trannum,"AMOUNT",&amount);
			trans_total+=amount;
		}
		ef=ADVNXTNRDsec(trannum,1,SCRNvirtualSubData,parent);
	}
	if(poid1!=NULL) Rfree(poid1);
	total_lineout=(total_lines-trans_total);
	FINDRSCSETDOUBLE(parent,"OUTSTANDING LINE AMOUNT",total_lineout);
	return(total_lineout);
}
static void set_defaults(RDArsrc *parent)
{
	int selprev=0,sellineno=0;
        int start=0;
        short x=0;

        start=(val(CURRENT_DATE,2)-1);
        x=(short)start;
        FINDRSCSETSHORT(parent,"[PURTRN][PROCESSING MONTH]",x);
        FINDFLDSETSHORT(trannum,"PROCESSING MONTH",x);
	FINDRSCGETINT(parent,"PREVIOUS TRANSACTIONS",&selprev);
	DefaultResetScreen(parent,TRUE);
	FINDRSCSETINT(parent,"[PURTRN][FISCAL YEAR]",yearid);
	FINDRSCSETINT(parent,"[PURACC][FISCAL YEAR]",yearid);
	FINDFLDSETINT(trannum,"FISCAL YEAR",yearid);
	FINDFLDSETINT(accnum,"FISCAL YEAR",yearid);
	FINDRSCSETSTRING(parent,"[PURTRN][PO IDENTIFICATION]",POID);
	FINDRSCSETSTRING(parent,"[PURACC][PO IDENTIFICATION]",POID);
	FINDFLDSETSTRING(trannum,"PO IDENTIFICATION",POID);
	FINDFLDSETSTRING(accnum,"PO IDENTIFICATION",POID);
	FINDRSCGETINT(parent,"LINE NUMBERS",&sellineno);
	lineno=Ratoi(linesavl->libs[sellineno]);
	FINDRSCSETDOUBLE(parent,"TOTAL EXPENDITURE",total_exp);
	FINDRSCSETDOUBLE(parent,"TOTAL REVENUE",total_rev);
	FINDRSCSETDOUBLE(parent,"TOTAL DEBITS",total_deb);
	FINDRSCSETDOUBLE(parent,"TOTAL CREDITS",total_cred);
	if(po_total>=0)
	{
		po_total=getpo_total(parent);
	}
	FINDRSCSETDOUBLE(parent,"PO TOTAL",po_total);
	FINDRSCSETINT(parent,"PREVIOUS TRANSACTIONS",selprev);
	getsupportingrecords(parent);
	changeACCTtype(parent);
	changeDEFtype(parent);
	getacctdesc(parent);
	debitcodenum=1;
	getsearchstr(parent,&debitcodenum);
	creditcodenum=2;
	getsearchstr(parent,&creditcodenum); 
	defdebcodenum=3;
	getsearchstr(parent,&defdebcodenum); 
	defpaycodenum=4;
	getsearchstr(parent,&defpaycodenum); 
	defcredcodenum=5;
	getsearchstr(parent,&defcredcodenum); 
	if(venpmt->use_venpmt_fund)
	{
		changeclearDEFtype(parent);
		defduetonum=6;
		getsearchstr(parent,&defduetonum);
		defclearfromnum=7;
		getsearchstr(parent,&defclearfromnum);
		defclearcashnum=8;
		getsearchstr(parent,&defclearcashnum);
	}
	total_lineout=getout_amt(parent);
	FINDRSCSETDOUBLE(parent,"[PURTRN][AMOUNT]",total_lineout);
}
static void set_defaultscb(RDArsrc *parent)
{
	DefaultResetScreen(parent,TRUE);
	set_defaults(parent);
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
	char active=FALSE,delflag=FALSE;
	int selectedd=0,selecteda=0;

	FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	deftype1=(short)selectedd;
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCGETINT(parent,"DEFAULT CLEARING DEFINITION TYPES",&selectedd);
		cdeftype1=(short)selectedd;
		FINDFLDSETSHORT(accnum,"DEFAULT CLEARING DEFINITION TYPE",cdeftype1);
	}
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
		case 0:	/* Account Code Widget */
			if(acctcode1!=NULL) Rfree(acctcode1);
			FINDRSCSETSTRING(parent,"ACCOUNT CODE DESCRIPTION",tmp);
			updatersrc(parent,"ACCOUNT CODE DESCRIPTION");
			readwidget(parent,"ACCOUNT CODE SEARCH");
			FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
			ZERNRD(tempaccfile);
			FINDFLDSETSHORT(tempaccfile,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(tempaccfile,"ACCOUNT CODE",acctcode1);
			if(acctcode1==NULL || RDAstrlen(acctcode1)<1)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","NO ACCOUNT CODE!","No ACCOUNT CODE was entered.",NULL,NULL,TRUE,NULL);
				prterr("No ACCOUNT CODE was entered.");
				if(acctcode1!=NULL) { Rfree(acctcode1); acctcode1=NULL; }
				return(FALSE);
			}
			if(ADVEQLNRDsec(tempaccfile,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+256);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED ACCOUNT CODE.  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",acctcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT CODE DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempaccfile,"DELETEFLAG",&delflag);
			if(delflag)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+128);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",acctcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT CODE DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} 
			FINDFLDGETCHAR(tempaccfile,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+80);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE ACCOUNT CODE.",acctcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT CODE DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			ZERNRD(tempyrfile);
			FINDFLDSETSHORT(tempyrfile,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(tempyrfile,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(tempyrfile,"ACCOUNT CODE",acctcode1);
			if(ADVEQLNRDsec(tempyrfile,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+256);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",
					acctcode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT CODE DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+150);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if this account should not be deleted.",acctcode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT CODE DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+100);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE YEAR ACCOUNT CODE for the YEAR [%d].",acctcode1,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE SEARCH");
				clearwidget(parent,"ACCOUNT CODE DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			getacctdesc(parent);
			break;
		case 1:	/* Debit Code Widget */
			if(debcode1!=NULL) Rfree(debcode1); 
			readwidget(parent,"DEBIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
			if(selecteda==BAL_ACCT)
			{
				FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",NULL);
				updatersrc(parent,"DEBIT CODE SEARCH ");
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				return(FALSE);
			}
			if(debcode1==NULL && RDAstrlen(debcode1)>0)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","NO DEBIT CODE!","No DEBIT CODE was entered.",NULL,NULL,TRUE,NULL);
				prterr("No DEBIT CODE was entered.");
				if(debcode1!=NULL) Rfree(debcode1); 
				return(FALSE);
			}
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",debcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(debcode1)+180);
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
				tmp=Rmalloc(RDAstrlen(debcode1)+150);
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
				tmp=Rmalloc(RDAstrlen(debcode1)+100);
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
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",debcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+325);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",debcode1,yearid); 
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
				tmp=Rmalloc(RDAstrlen(debcode1)+180);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",debcode1,yearid);
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
				tmp=Rmalloc(RDAstrlen(debcode1)+120);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",debcode1,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"ENCUMBRANCE"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ENCUMBRANCE].",NULL,NULL,TRUE,NULL);
				prterr("The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ENCUMBRANCE].");
				clearwidget(parent,"DEBIT CODE SEARCH");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			} 
			break;
		case 2:	/* Credit Code Widget */
			if(credcode1!=NULL) Rfree(credcode1);
			readwidget(parent,"CREDIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
			if(credcode1==NULL || RDAstrlen(credcode1)<1)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","NO CREDIT CODE!","No CREDIT CODE was entered.",NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			} 
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",credcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(credcode1)+190);
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
				tmp=Rmalloc(RDAstrlen(credcode1)+128);
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
				tmp=Rmalloc(RDAstrlen(credcode1)+100);
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
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",credcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+280);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",credcode1,yearid);
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
				tmp=Rmalloc(RDAstrlen(credcode1)+180);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",credcode1,yearid);
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
				tmp=Rmalloc(RDAstrlen(credcode1)+125);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",credcode1,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"RESERVE FOR ENCUMBRANCE"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [RESERVE FOR ENCUMBRANCE].",NULL,NULL,TRUE,NULL);
				prterr("The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [RESERVE FOR ENCUMBRANCE].");
				clearwidget(parent,"CREDIT CODE SEARCH");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			} 
			break;
		case 3: /* Default Debit Code Widget */	
			if(defdebcode1!=NULL) Rfree(defdebcode1);
			readwidget(parent,"DEFAULT DEBIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"DEFAULT DEBIT CODE SEARCH",&defdebcode1);
			if(selecteda==BAL_ACCT || (selecteda==REV_ACCT && (defdebcode1==NULL || RDAstrlen(defdebcode1)<1)))
			{
				clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				return(FALSE);
			}
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defdebcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(defdebcode1)+190);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defdebcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+160);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defdebcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+110);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",defdebcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defdebcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+300);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",defdebcode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+190);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defdebcode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+140);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defdebcode1,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(selecteda==REV_ACCT)
			{
				if(defdebcode1==NULL || RDAstrlen(defdebcode1)<1)
				{
					clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
					clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
					if(defdebcode1!=NULL) Rfree(defdebcode1);
					return(FALSE);
				}
			} else if(selecteda==EXP_ACCT)
			{
				if(RDAstrcmp(baltype,"EXPENDITURE"))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEFAULT DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
					prterr("The DEFAULT DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE for the ACCOUNT TYPE EXPENDITURE.");
					clearwidget(parent,"DEFAULT DEBIT CODE SEARCH");
					clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
					if(defdebcode1!=NULL) Rfree(defdebcode1);
					return(FALSE);
				} 
			}	
			break;
		case 4: /* Default Payable Code Widget */	
			if(defpaycode1!=NULL) Rfree(defpaycode1);
			readwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
			FINDRSCGETSTRING(parent,"DEFAULT PAYABLE CODE SEARCH",&defpaycode1);
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defpaycode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+200);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defpaycode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+160);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defpaycode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+110);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",defpaycode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defpaycode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+300);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",defpaycode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+180);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defpaycode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+130);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defpaycode1,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(selecteda==REV_ACCT)
			{
				if(defpaycode1==NULL || RDAstrlen(defpaycode1)<1)
				{
					clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
					clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
					if(defpaycode1!=NULL) Rfree(defpaycode1);
					return(FALSE);
				}
			} else if(selecteda==EXP_ACCT)
			{
				if(RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEFAULT PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ACCOUNTS PAYABLE] for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
					prterr("The DEFAULT PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ACCOUNTS PAYABLE] for the ACCOUNT TYPE EXPENDITURE.");
					clearwidget(parent,"DEFAULT PAYABLE CODE SEARCH");
					clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
					if(defpaycode1!=NULL) Rfree(defpaycode1);
					return(FALSE);
				} 
			}	
			break;
		case 5: /* Default Credit Code Widget */	
			if(defcredcode1!=NULL) { Rfree(defcredcode1); defcredcode1=NULL; }
			readwidget(parent,"DEFAULT CREDIT CODE SEARCH");
			FINDRSCGETSTRING(parent,"DEFAULT CREDIT CODE SEARCH",&defcredcode1);
			if(selecteda==EXP_ACCT && (defcredcode1==NULL || RDAstrlen(defcredcode1)<1))
			{
				clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defcredcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+190);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defcredcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+150);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defcredcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+110);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",defcredcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defcredcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+290);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",defcredcode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+180);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defcredcode1,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+130);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defcredcode1,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(selecteda==EXP_ACCT)
			{
				if(defcredcode1==NULL || RDAstrlen(defcredcode1)<1)
				{
					clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
					clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
					if(defcredcode1!=NULL) Rfree(defcredcode1);
					return(FALSE);
				}
			} else if(selecteda==REV_ACCT)
			{
				if(RDAstrcmp(baltype,"REVENUE"))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEFAULT CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [REVENUE] for the ACCOUNT TYPE REVENUE.",NULL,NULL,TRUE,NULL);
					prterr("The DEFAULT CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [REVENUE] for the ACCOUNT TYPE REVENUE.");
					clearwidget(parent,"DEFAULT CREDIT CODE SEARCH");
					clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
					if(defcredcode1!=NULL) Rfree(defcredcode1);
					return(FALSE);
				} 
			}	
			break;
		case 6: /* Due from */
			if(defdueto!=NULL) { Rfree(defdueto); defdueto=NULL; }
			readwidget(parent,"DEFAULT DUE TO SEARCH");
			FINDRSCGETSTRING(parent,"DEFAULT DUE TO SEARCH",&defdueto);
			if(selecteda==EXP_ACCT && (defdueto==NULL || RDAstrlen(defdueto)<1))
			{
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defdueto);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdueto)+190);
				sprintf(tmp,"The DEFAULT DUE TO [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defdueto);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				prterr(tmp);
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdueto)+150);
				sprintf(tmp,"The DEFAULT DUE TO [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defdueto);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				prterr(tmp);
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdueto)+110);
				sprintf(tmp,"The DEFAULT DUE TO [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",defdueto); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				prterr(tmp);
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defdueto);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdueto)+290);
				sprintf(tmp,"The DEFAULT DUE TO [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",defdueto,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				prterr(tmp);
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdueto)+180);
				sprintf(tmp,"The DEFAULT DUE TO [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defdueto,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				prterr(tmp);
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdueto)+130);
				sprintf(tmp,"The DEFAULT DUE TO [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defdueto,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				prterr(tmp);
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"DUE TO"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEFAULT DUE TO entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE TO] for the ACCOUNT TYPE DUE TO.",NULL,NULL,TRUE,NULL);
				prterr("The DEFAULT DUE TO entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE TO] for the ACCOUNT TYPE DUE TO.");
				clearwidget(parent,"DEFAULT DUE TO SEARCH");
				clearwidget(parent,"DEFAULT DUE TO DESCRIPTION");
				if(defdueto!=NULL) Rfree(defdueto);
				return(FALSE);
			}	
			break;
		case 7: /* Clear Due To */
			if(defclearfrom!=NULL) { Rfree(defclearfrom); defclearfrom=NULL; }
			readwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
			FINDRSCGETSTRING(parent,"DEFAULT CLEARING DUE FROM SEARCH",&defclearfrom);
			if(selecteda==EXP_ACCT && (defclearfrom==NULL || RDAstrlen(defclearfrom)<1))
			{
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defclearfrom);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearfrom)+190);
				sprintf(tmp,"The DEFAULT CLEARING DUE FROM [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defclearfrom);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearfrom)+150);
				sprintf(tmp,"The DEFAULT CLEARING DUE FROM [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defclearfrom);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearfrom)+110);
				sprintf(tmp,"The DEFAULT CLEARING DUE FROM [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",defclearfrom); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defclearfrom);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearfrom)+290);
				sprintf(tmp,"The DEFAULT CLEARING DUE FROM [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",defclearfrom,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearfrom)+180);
				sprintf(tmp,"The DEFAULT CLEARING DUE FROM [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defclearfrom,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearfrom)+130);
				sprintf(tmp,"The DEFAULT CLEARING DUE FROM [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defclearfrom,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				prterr(tmp);
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"DUE FROM"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEFAULT CLEARING DUE FROM entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE FROM].",NULL,NULL,TRUE,NULL);
				prterr("The DEFAULT CLEARING DUE FROM entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE FROM] for the CLEARING DUE FROM.");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM SEARCH");
				clearwidget(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				if(defclearfrom!=NULL) Rfree(defclearfrom);
				return(FALSE);
			}	
			break;
		case 8: /* Clear Cash */
			if(defclearcash!=NULL) { Rfree(defclearcash); defclearcash=NULL; }
			readwidget(parent,"DEFAULT CLEARING CASH SEARCH");
			FINDRSCGETSTRING(parent,"DEFAULT CLEARING CASH SEARCH",&defclearcash);
			if(selecteda==EXP_ACCT && (defclearcash==NULL || RDAstrlen(defclearcash)<1))
			{
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
			}
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defclearcash);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearcash)+190);
				sprintf(tmp,"The DEFAULT CLEARING CASH [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defclearcash);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearcash)+150);
				sprintf(tmp,"The DEFAULT CLEARING CASH [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defclearcash);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearcash)+110);
				sprintf(tmp,"The DEFAULT CLEARING CASH [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",defclearcash); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",cdeftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defclearcash);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearcash)+290);
				sprintf(tmp,"The DEFAULT CLEARING CASH [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",defclearcash,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearcash)+180);
				sprintf(tmp,"The DEFAULT CLEARING CASH [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defclearcash,yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defclearcash)+130);
				sprintf(tmp,"The DEFAULT CLEARING CASH [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defclearcash,yearid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				prterr(tmp);
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"CASH"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEFAULT CLEARING CASH entered does not have the correct BALANCE SHEET REFERENCE TYPE of [CASH] for the ACCOUNT TYPE CASH.",NULL,NULL,TRUE,NULL);
				prterr("The DEFAULT CLEARING CASH entered does not have the correct BALANCE SHEET REFERENCE TYPE of [CASH] for the ACCOUNT TYPE CASH.");
				clearwidget(parent,"DEFAULT CLEARING CASH SEARCH");
				clearwidget(parent,"DEFAULT CLEARING CASH DESCRIPTION");
				if(defclearcash!=NULL) Rfree(defclearcash);
				return(FALSE);
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

	readscreen(parent);
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
		case 3:	/* default debit code */
			searchwidget=stralloc("DEFAULT DEBIT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEFAULT DEBIT CODES");
				holdapplib=defdebavl;
			}
			break;
		case 4:	/* default payable code */
			searchwidget=stralloc("DEFAULT PAYABLE CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEFAULT PAYABLE CODES");
				holdapplib=defpayavl;
			}
			break;
		case 5:	/* default credit code */
			searchwidget=stralloc("DEFAULT CREDIT CODE SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEFAULT CREDIT CODES");
				holdapplib=defcredavl;
			}
			break;
		case 6: /* default due from */
			searchwidget=stralloc("DEFAULT DUE TO SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEFAULT DUE TO CODES");
				holdapplib=defduetoavl;
			}
			break;
		case 7: /* default clear due to */
			searchwidget=stralloc("DEFAULT CLEARING DUE FROM SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEFAULT CLEARING DUE FROM CODES");
				holdapplib=defclearfromavl;
			}
			break;
		case 8: /* default clearing cash */
			searchwidget=stralloc("DEFAULT CLEARING CASH SEARCH");
			if(AddList)
			{
				listwidget=stralloc("DEFAULT CLEARING CASH CODES");
				holdapplib=defclearcashavl;
			}
			break;
		default: 
			prterr("Invalid Acount List [%d]",*listtype);
			break;
	}
	if(searchwidget==NULL) return;
/*
	readwidget(parent,searchwidget);
*/
	if(selectedc!=NULL) Rfree(selectedc);
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
	} else if(check_acctcode(parent,*listtype)==FALSE) return;
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
		case 3:	 /* default debit availble */
			if(AddList)
			{
				holdapplib=defdebavl;
				listwidget=stralloc("DEFAULT DEBIT CODES");
			}
			searchwidget=stralloc("DEFAULT DEBIT CODE SEARCH");
			descriptwidget=stralloc("DEFAULT DEBIT CODE DESCRIPTION");
			break;
		case 4:	 /* default payable available */
			if(AddList)
			{
				holdapplib=defpayavl;
				listwidget=stralloc("DEFAULT PAYABLE CODES");
			}
			searchwidget=stralloc("DEFAULT PAYABLE CODE SEARCH");
			descriptwidget=stralloc("DEFAULT PAYABLE CODE DESCRIPTION");
			break;
		case 5:	 /* default credit available */
			if(AddList)
			{
				holdapplib=defcredavl;
				listwidget=stralloc("DEFAULT CREDIT CODES");
			}
			searchwidget=stralloc("DEFAULT CREDIT CODE SEARCH");
			descriptwidget=stralloc("DEFAULT CREDIT CODE DESCRIPTION");
			break;
		case 6: /* Default Due froms */
			if(AddList)
			{
				holdapplib=defduetoavl;
				listwidget=stralloc("DEFAULT DUE TO CODES");
			}
			searchwidget=stralloc("DEFAULT DUE TO SEARCH");
			descriptwidget=stralloc("DEFAULT DUE TO DESCRIPTION");
			break;
		case 7: /* default clear due to's */
			if(AddList)
			{
				holdapplib=defclearfromavl;
				listwidget=stralloc("DEFAULT CLEARING DUE FROM CODES");
			}
			searchwidget=stralloc("DEFAULT CLEARING DUE FROM SEARCH");
			descriptwidget=stralloc("DEFAULT CLEARING DUE FROM DESCRIPTION");
			break;
		case 8: /* default clearing cash */
			if(AddList)
			{
				holdapplib=defclearcashavl;
				listwidget=stralloc("DEFAULT CLEARING CASH CODES");
			}
			searchwidget=stralloc("DEFAULT CLEARING CASH SEARCH");
			descriptwidget=stralloc("DEFAULT CLEARING CASH DESCRIPTION");
			break;
		default:
			prterr("Invalid Account List [%d]",*whichlist);
			break;
	}
        FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	if(AddList)
	{
		if(holdapplib==NULL)
		{
			if(listwidget!=NULL) Rfree(listwidget);
			if(searchwidget!=NULL) Rfree(searchwidget);
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
	FINDFLDSETSHORT(accnum,"ACCOUNT TYPE",fileaccttype);
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
	FINDRSCSETSTRING(parent,"ACCOUNT CODE DESCRIPTION",descript);
	updatersrc(parent,"ACCOUNT CODE DESCRIPTION");
	if(descript!=NULL) Rfree(descript);
	updateSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *parent)
{
	ZERNRD(trannum);
	ZERNRD(accnum);
	readallwidgets(parent);
	ReadRDAScrolledLists(parent);
	rsrc2filerecord(trannum,parent);
	rsrc2filerecord(accnum,parent);
	list2file(parent);
}
static void list2file(RDArsrc *parent)
{
	int select=0;
	short x=0;
	
	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&select)) return;
	x=(short)select;
	FINDFLDSETSHORT(accnum,"ACCOUNT TYPE",x);
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&select)) return;
	x=(short)select;
	FINDFLDSETSHORT(accnum,"DEFINITION TYPE",x);
	if(venpmt->use_venpmt_fund)
	{
		if(FINDRSCGETINT(parent,"DEFAULT CLEARING DEFINITION TYPES",&select)) return;
		x=(short)select;
		FINDFLDSETSHORT(accnum,"DEFAULT CLEARING DEFINITION TYPE",x);
	}
	if(FINDRSCGETINT(parent,"LINE NUMBERS",&select)) return;
	x=Ratoi(linesavl->libs[select]);
	FINDFLDSETINT(accnum,"LINE NUMBER",x);
	FINDFLDSETINT(trannum,"LINE NUMBER",x);
	FINDFLDSETSTRING(trannum,"PO IDENTIFICATION",POID);
	FINDFLDSETSTRING(accnum,"PO IDENTIFICATION",POID);
	FINDFLDSETINT(trannum,"FISCAL YEAR",yearid);
	FINDFLDSETINT(accnum,"FISCAL YEAR",yearid);
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(defdebcode1!=NULL) Rfree(defdebcode1);
	if(defcredcode1!=NULL) Rfree(defcredcode1);
	if(defpaycode1!=NULL) Rfree(defpaycode1);
	if(defdueto!=NULL) Rfree(defdueto);
	if(defclearfrom!=NULL) Rfree(defclearfrom);
	if(defclearcash!=NULL) Rfree(defclearcash);
	if(AddList)
	{
		FINDRSCGETINT(parent,"ACCOUNT CODES",&select);
		if(RDAstrcmp(acctavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(acctavl->libs[select],"Not Applicable"))
		{
			acctcode1=stralloc(acctavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"ACCOUNT CODE",acctcode1);
		FINDRSCGETINT(parent,"DEBIT CODES",&select);
		if(RDAstrcmp(debitavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(debitavl->libs[select],"Not Applicable"))
		{
			debcode1=stralloc(debitavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"DEBIT CODE",debcode1);
		FINDRSCGETINT(parent,"CREDIT CODES",&select);
		if(RDAstrcmp(creditavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(creditavl->libs[select],"Not Applicable"))
		{
			credcode1=stralloc(creditavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"CREDIT CODE",credcode1);
		FINDRSCGETINT(parent,"DEFAULT DEBIT CODES",&select);
		if(RDAstrcmp(defdebavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(defdebavl->libs[select],"Not Applicable"))
		{
			defdebcode1=stralloc(defdebavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"DEFAULT DEBIT CODE",defdebcode1);
		FINDRSCGETINT(parent,"DEFAULT CREDIT CODES",&select);
		if(RDAstrcmp(defcredavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(defcredavl->libs[select],"Not Applicable"))
		{
			defcredcode1=stralloc(defcredavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"DEFAULT CREDIT CODE",defcredcode1);
		FINDRSCGETINT(parent,"DEFAULT PAYABLE CODES",&select);
		if(RDAstrcmp(defpayavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(defpayavl->libs[select],"Not Applicable"))
		{
			defpaycode1=stralloc(defpayavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"DEFAULT PAYABLE CODE",defpaycode1);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCGETINT(parent,"DEFAULT DUE TO CODES",&select);
		if(RDAstrcmp(defduetoavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(defduetoavl->libs[select],"Not Applicable"))
		{
			defdueto=stralloc(defduetoavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"DEFAULT DUE TO",defdueto);
		FINDRSCGETINT(parent,"DEFAULT CLEARING DUE FROM CODES",&select);
		if(RDAstrcmp(defclearfromavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(defclearfromavl->libs[select],"Not Applicable"))
		{
			defclearfrom=stralloc(defclearfromavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"DEFAULT CLEARING DUE FROM",defclearfrom);
		FINDRSCGETINT(parent,"DEFAULT CLEARING CASH CODES",&select);
		if(RDAstrcmp(defclearcashavl->libs[select],"No Accounts Available") &&
			RDAstrcmp(defclearcashavl->libs[select],"Not Applicable"))
		{
			defclearcash=stralloc(defclearcashavl->libs[select]);
		}
		FINDFLDSETSTRING(accnum,"DEFAULT CLEARING CASH",defclearcash);
		}
	} else {
		FINDRSCGETSTRING(parent,"ACCOUNT CODE SEARCH",&acctcode1);
		FINDFLDSETSTRING(accnum,"ACCOUNT CODE",acctcode1);
		FINDRSCGETSTRING(parent,"DEBIT CODE SEARCH",&debcode1);
		FINDFLDSETSTRING(accnum,"DEBIT CODE",debcode1);
		FINDRSCGETSTRING(parent,"CREDIT CODE SEARCH",&credcode1);
		FINDFLDSETSTRING(accnum,"CREDIT CODE",credcode1);
		FINDRSCGETSTRING(parent,"DEFAULT DEBIT CODE SEARCH",&defdebcode1);
		FINDFLDSETSTRING(accnum,"DEFAULT DEBIT CODE",defdebcode1);
		FINDRSCGETSTRING(parent,"DEFAULT CREDIT CODE SEARCH",&defcredcode1);
		FINDFLDSETSTRING(accnum,"DEFAULT CREDIT CODE",defcredcode1);
		FINDRSCGETSTRING(parent,"DEFAULT PAYABLE CODE SEARCH",&defpaycode1);
		FINDFLDSETSTRING(accnum,"DEFAULT PAYABLE CODE",defpaycode1);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCGETSTRING(parent,"DEFAULT DUE TO SEARCH",&defdueto);
		FINDFLDSETSTRING(accnum,"DEFAULT DUE TO",defdueto);
		FINDRSCGETSTRING(parent,"DEFAULT CLEARING DUE FROM SEARCH",&defclearfrom);
		FINDFLDSETSTRING(accnum,"DEFAULT CLEARING DUE FROM",defclearfrom);
		FINDRSCGETSTRING(parent,"DEFAULT CLEARING CASH SEARCH",&defclearcash);
		FINDFLDSETSTRING(accnum,"DEFAULT CLEARING CASH",defclearcash);
		}
	}
}
static void save_check(RDArsrc *parent)
{
	int selaccttype=0,seldeftype=0;
	short selmonth=0;
	short x=0,testapp=TRUE;
	char *acctstr=NULL,*header=NULL,*message=NULL;
	double amount=0.0;

	readscreen(parent);
	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selaccttype)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETDOUBLE(parent,"[PURTRN][AMOUNT]",&amount)) return;
	if(amount==0.0) 
	{
		ERRORDIALOG("INVALID AMOUNT","This PO Transaction can not be saved with an AMOUNT of [$  0.00].  Please enter the correct amount for this voucher.",NULL,FALSE);
		prterr("Attemped to save a voucher with a zero dollar amount");
		return;
	}
	switch(selaccttype)
	{
		case EXP_ACCT:
			ZERNRD(eyrnum);
			FINDFLDSETINT(eyrnum,"FISCAL YEAR",yearid);
			x=(short)seldeftype;
			FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",x);
 			FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acctcode1);
			if(!ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,parent))
			{
				if(finsetup->budget==ANNUALBUDGET) 
				{ 
					testapp=testapprotot(eyrnum,amount,NULL);
				} else {
					FINDFLDGETSHORT(trannum,"PROCESSING MONTH",&selmonth);
					if(purord->use_fiscal_months==TRUE)
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
           	 		header=stralloc("This PO Transactions will cause the POSTED APPROPRIATED amount to be less than all of the EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT [%s] in the YEAR [%d].");
              			message=Rrealloc(message,RDAstrlen(header)+RDAstrlen(acctstr)+6);
/* I don't know about this sprintf */
           			sprintf(message,header,acctstr,yearid);
           	   		message[RDAstrlen(header)+RDAstrlen(acctstr)+6-1]=0;
					WARNINGDIALOG("WARNING DIALOG SCREEN","APPROPRIATIONS NOT ENOUGH FOR TRANSACTION",message,save_record,NULL,FALSE,parent);
           	  		if(header!=NULL) Rfree(header);
           			if(message!=NULL) Rfree(message);
           			if(acctstr!=NULL) Rfree(acctstr);
				} else {
					save_record(parent);
					FINDRSCSETDOUBLE(parent,"OUTSTANDING LINE AMOUNT",(total_lineout-amount));
					updatersrc(parent,"OUTSTANDING LINE AMOUNT");
					FINDRSCSETDOUBLE(parent,"[PURTRN][AMOUNT]",(total_lineout-amount));
					updatersrc(parent,"[PURTRN][AMOUNT]");
				}
			}
			break;
	    case REV_ACCT:
	    case BAL_ACCT:
			save_record(parent);
			FINDRSCSETDOUBLE(parent,"OUTSTANDING LINE AMOUNT",(total_lineout-amount));
			updatersrc(parent,"OUTSTANDING LINE AMOUNT");
			FINDRSCSETDOUBLE(parent,"[PURTRN][AMOUNT]",(total_lineout-amount));
			updatersrc(parent,"[PURTRN][AMOUNT]");
			break;
	    default:
			prterr("Invalid Account Type [%d]",selaccttype);
			break;
	}
}
static void save_record(RDArsrc *parent)
{
	int selcdeftype=0,seldeftype=0,selaccttype=0;
	int selmonth=0,sellineno=0,acctno1=0;
	char *rcddesc=NULL,*errordesc=NULL,deleteflag=FALSE,*acct=NULL;
	short x=0,deftype1=0,accttype=0;
	int lf=0;
	char *date=NULL,*timex=NULL,delflag=FALSE;
	double amount=0.0;
	RDArmem *member=NULL;
	RDATData *prev=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selaccttype)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETINT(parent,"LINE NUMBERS",&sellineno)) return;
	if(FINDRSCGETDOUBLE(parent,"[PURTRN][AMOUNT]",&amount)) return;
	FINDFLDSETINT(trannum,"FISCAL YEAR",yearid);
	FINDRSCGETSHORT(parent,"[PURTRN][PROCESSING MONTH]",&x);
	FINDFLDSETSHORT(trannum,"PROCESSING MONTH",x);
	FINDFLDSETSHORT(trannum,"CLEAR PROCESSING MONTH",x);
	selmonth=(int)x;
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCGETINT(parent,"DEFAULT CLEARING DEFINITION TYPES",&selcdeftype);
		x=(short)selcdeftype;
		FINDFLDSETSHORT(accnum,"DEFAULT CLEARING DEFINITION TYPE",x);
	}
	if(ADVRECORDsec(accnum,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(accnum,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write records in ADD PO TRANSACTIONS.");
	} else if(ADVRECORDsec(trannum,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(trannum,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write records in ADD PO TRANSACTIONS.");
		}
	} else {
		if(lineno==0)
		{
			ZERNRD(linenum);
			FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
			FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
			FINDFLDSETINT(linenum,"LINE NUMBER",lineno);
			if(!ADVEQLNRDsec(linenum,1,SCRNvirtualSubData,parent))
			{
				FINDFLDGETCHAR(linenum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					prev=RDATDataNEW(linenum);
					delflag=FALSE;
					FINDFLDSETCHAR(linenum,"DELETEFLAG",delflag);
					if(rcddesc!=NULL) Rfree(rcddesc);
					rcddesc=stralloc("Not Associated");
					FINDFLDSETSTRING(linenum,"DESCRIPTION",rcddesc);
					ADVWRTTRANSsec(linenum,0,NULL,prev,SCRNvirtualSubData,parent);
					if(prev!=NULL) FreeRDATData(prev);
					if(rcddesc!=NULL) Rfree(rcddesc);
				}
			} else {
				if(rcddesc!=NULL) Rfree(rcddesc);
				rcddesc=stralloc("Not Associated");
				FINDFLDSETSTRING(linenum,"DESCRIPTION",rcddesc);
				ADVWRTTRANSsec(linenum,0,NULL,NULL,SCRNvirtualSubData,parent); /* Adding Situation */
				if(rcddesc!=NULL) Rfree(rcddesc);
			}
		}		
		FINDFLDSETSTRING(trannum,"PO IDENTIFICATION",POID);
		FINDFLDSETSTRING(accnum,"PO IDENTIFICATION",POID);
		FINDFLDSETSTRING(trannum,"SOURCE USER",USERLOGIN);
		deleteflag=FALSE;
		FINDFLDSETCHAR(trannum,"DELETEFLAG",deleteflag);
		FINDFLDSETCHAR(trannum,"SOURCE USER VERIFIED",deleteflag);
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		if((x=FINDRSC(parent,"[PURTRN][ENTRY DATE]"))!=(-1))
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
				FINDFLDSETSTRING(trannum,"ENTRY DATE",date);
			}
		} else {
			FINDFLDSETSTRING(trannum,"ENTRY DATE",date);
		}
		if(date!=NULL) Rfree(date);
		if((x=FINDRSC(parent,"[PURTRN][ENTRY TIME]"))!=(-1))
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
				FINDFLDSETSTRING(trannum,"ENTRY TIME",timex);
			}
		} else {
			FINDFLDSETSTRING(trannum,"ENTRY TIME",timex);
		}
		if(timex!=NULL) Rfree(timex);
		if(!WRTTRANS(accnum,1,"ACCOUNT NUMBER",NULL))
		{ 
			FINDFLDGETINT(accnum,"ACCOUNT NUMBER",&acctno1);
			FINDFLDSETINT(trannum,"ACCOUNT NUMBER",acctno1);
			FINDFLDSETDOUBLE(trannum,"AMOUNT",amount);
			if(!WRTTRANS(trannum,1,"TRANSACTION NUMBER",NULL))
			{
				FINDFLDGETSHORT(accnum,"ACCOUNT TYPE",&accttype);
				switch(accttype)
				{
					case EXP_ACCT:
						total_exp+=amount;
						FINDFLDGETSTRING(accnum,"DEBIT CODE",&acct);
						if(acct!=NULL && RDAstrlen(acct)>0)
						{
							total_deb+=amount;
							Rfree(acct);
						}
						FINDFLDGETSTRING(accnum,"CREDIT CODE",&acct);
						if(acct!=NULL && RDAstrlen(acct)>0)
						{
							total_cred+=amount;
							Rfree(acct);
						}
						break;
					case REV_ACCT:
						total_rev+=amount;
						FINDFLDGETSTRING(accnum,"CREDIT CODE",&acct);
						if(acct!=NULL && RDAstrlen(acct)>0)
						{
							total_cred+=amount;
							Rfree(acct);
						}
						FINDFLDGETSTRING(accnum,"DEBIT CODE",&acct);
						if(acct!=NULL && RDAstrlen(acct)>0)
						{
							total_deb+=amount;
							Rfree(acct);
						}
						break;
					case BAL_ACCT: /* Should also do credit code */
						if(amount>0.0) total_deb+=amount;
						else total_cred-=amount;
						FINDFLDGETSTRING(accnum,"CREDIT CODE",&acct);
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
				if(selaccttype==EXP_ACCT)
				{
					ADVupdateoutstanding(msrnum,linenum,yearid,POID,lineno,amount,SCRNvirtualSubData,parent);
				}
				deftype1=(short)seldeftype;
				switch(selaccttype)
				{
					case EXP_ACCT:
						if(purord->use_fiscal_months==TRUE)
						{
						ADVupdateundist(accnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,yearid,deftype1,"ENCUMBRANCES",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
						ADVupdateundist(accnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,yearid,deftype1,"ENCUMBRANCES",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
						break;
					case REV_ACCT:
						if(purord->use_fiscal_months==TRUE)
						{
						ADVupdateundist(accnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,yearid,deftype1,"ESTIMATED REVENUE",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
						ADVupdateundist(accnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,yearid,deftype1,"ESTIMATED REVENUE",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
						break;
					case BAL_ACCT:
						if(amount>0.0)
						{
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(accnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid,deftype1,"",FISCALPERIOD[selmonth],amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(accnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid,deftype1,"",CALENDAR_MONTHS[selmonth],amount,SCRNvirtualSubData,parent);
						}
						} else {
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(accnum,NULL,"CREDIT CODE","ACCOUNT CODE",NULL,(-1),byrnum,yearid,deftype1,"",FISCALPERIOD[selmonth],-amount,SCRNvirtualSubData,parent);
						} else {
							ADVupdateundist(accnum,NULL,"CREDIT CODE","ACCOUNT CODE",NULL,(-1),byrnum,yearid,deftype1,"",CALENDAR_MONTHS[selmonth],-amount,SCRNvirtualSubData,parent);
						}
						}
						break;
					default:
						prterr("Invalid Account Type [%d]",selaccttype);
						break;
				}
			} else {
				if(errordesc!=NULL) Rfree(errordesc); 
				errordesc=Rmalloc(RDAstrlen(MODULENAME(trannum))+RDAstrlen(FILENAME(trannum))+139);
				sprintf(errordesc,"The write function failed to the file [%s][%s], the record and all subfiles that you attempted to save were not created.  Please try again.",MODULENAME(trannum),FILENAME(trannum));
				ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
				prterr("Error Couldn't Write Record: [%s]",errordesc);
				if(errordesc!=NULL) Rfree(errordesc);
			}
		} else {
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=Rmalloc(RDAstrlen(MODULENAME(accnum))+RDAstrlen(FILENAME(accnum))+139);
			sprintf(errordesc,"The write function failed to the file [%s][%s], the record and all subfiles that you attempted to save were not created.  Please try again.",MODULENAME(accnum),FILENAME(accnum));
			ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
			prterr("Error Couldn't Write Record: [%s]",errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		}
	}
	updatersrc(parent,"TOTAL EXPENDITURE");
	updatersrc(parent,"TOTAL REVENUE");
	updatersrc(parent,"TOTAL DEBITS");
	updatersrc(parent,"TOTAL CREDITS");
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS TRANSACTIONS",(prevtran->numlibs-1),
		prevtran))
	{
		updatersrc(parent,"PREVIOUS TRANSACTIONS");
	}
}
static void quit_check(RDArsrc *parent)
{
	if(total_deb!=total_cred)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","OUT OF BALANCE WARNING!",
			"Exiting ADD PO TRANSACTIONS with the entries added this session will make the Balance Sheet Out of Balance.  The TOTAL DEBITS entered this session are not equivalent to the TOTAL CREDITS.",
			quit_record1,NULL,FALSE,parent);
	} else if(total_deb!=po_total && po_total!=0.0)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVOICE BALANCE WARNING",
			"Exiting ADD PO TRANSACTIONS with the total of transactions out of balance with the total amount of the lines.  Please correct this before continuing.",quit_record1,NULL,FALSE,parent);
	} else {
		quit_record1(parent);
	}
}
static void quit_record1(RDArsrc *parent)
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
	if(prevtran->numlibs>1 || RDAstrcmp(prevtran->libs[0],
		"No PO Transactions added this session."))
	{
		PO_COUNT+=prevtran->numlibs;
	}
	if(ACCTtypes!=NULL) freeapplib(ACCTtypes);
	if(prevtran!=NULL) freeapplib(prevtran);
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(defdebcode1!=NULL) Rfree(defdebcode1);
	if(defcredcode1!=NULL) Rfree(defcredcode1);
	if(defpaycode1!=NULL) Rfree(defpaycode1);
	if(defdueto!=NULL) Rfree(defdueto);
	if(defclearfrom!=NULL) Rfree(defclearfrom);
	if(defclearcash!=NULL) Rfree(defclearcash);
}
static void getsupportingrecords(RDArsrc *parent)
{
	int lineno1=0;

	if(msrnum!=(-1))
	{
		FINDFLDSETINT(msrnum,"FISCAL YEAR",yearid);
		FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",POID);
		if(ADVEQLNRDsec(msrnum,1,SCRNvirtualSubData,parent)) KEYNRD(msrnum,1);
		filerecord2rsrc(msrnum,parent);
	}
	if(linenum!=(-1))
	{
		FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
		FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
		FINDFLDSETINT(linenum,"LINE NUMBER",lineno);
		if(ADVEQLNRDsec(linenum,1,SCRNvirtualSubData,parent))
		{
			KEYNRD(linenum,1);
			FINDFLDGETINT(linenum,"LINE NUMBER",&lineno1);
			if(lineno1==0)
			{
				FINDFLDSETSTRING(linenum,"DESCRIPTION","Not Associated");
			}
		}
		filerecord2rsrc(linenum,parent);
	}
	if(vennum!=(-1))
	{
		COPYFIELD(msrnum,vennum,"VENDOR IDENTIFICATION");
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,parent)) KEYNRD(vennum,1);
		filerecord2rsrc(vennum,parent);
	}
	if(banknum!=(-1))
	{
		FIELDCOPY(accnum,"DEFAULT BANK IDENTIFICATION",banknum,"BANK IDENTIFICATION");
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,parent)) KEYNRD(banknum,1);
		filerecord2rsrc(banknum,parent);
	}
}
static void getlines(RDArsrc *parent)
{
	short ef;
	char *poid1=NULL;
	int lineno1=0,yearid1=0;
	char delflag=FALSE;

	if(linesavl!=NULL) freeapplib(linesavl);
	linesavl=APPlibNEW();
	ZERNRD(linenum);
	FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
	ef=ADVGTENRDsec(linenum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETINT(linenum,"FISCAL YEAR",&yearid1);
		if(poid1!=NULL) Rfree(poid1);
		FINDFLDGETSTRING(linenum,"PO IDENTIFICATION",&poid1);
		if(yearid1==yearid && !RDAstrcmp(poid1,POID))
		{
			FINDFLDGETCHAR(linenum,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETINT(linenum,"LINE NUMBER",&lineno1);
				sprintf(stemp,"%4d",lineno1);
				addAPPlib(linesavl,stemp);
			}
		} else {
			break;
		}
		ef=ADVNXTNRDsec(linenum,1,SCRNvirtualSubData,parent);
	}
	lineno1=0;
	sprintf(stemp,"%4d",lineno1);
	if(FINDAPPLIBELEMENT(linesavl,stemp))
	{
		addAPPlib(linesavl,stemp);
	}
	SORTAPPlib(linesavl);
	if(poid1!=NULL) Rfree(poid1);
}
static void changeCODElist(RDArsrc *parent)
{
	int selecteda=0,selectedd=0,yearid1=0;
	short ef=0,accttype=0,deftype=0,deftype1=0,tempyrfile=(-1),tempaccfile=(-1);
	short keyn=0,keynumber=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL,*e=NULL,*actyp=NULL,*speedkeyname=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
	accttype=(short)selecteda;
	deftype=(short)selectedd;
	FINDRSCSETACCTTYPE(parent,"ACCOUNT CODE SEARCH",accttype);
	FINDRSCSETDEFTYPE(parent,"ACCOUNT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEBIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"CREDIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEFAULT DEBIT CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEFAULT PAYABLE CODE SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEFAULT CREDIT CODE SEARCH",deftype);
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCSETDEFTYPE(parent,"DEFAULT DUE TO SEARCH",deftype);
	}
	if(AddList==TRUE)
	{
		if((selecteda!=account_type || selectedd!=definition_type ||
			year_identification!=yearid) && AddList)
		{
			if(debitavl!=NULL) freeapplib(debitavl); 
			debitavl=APPlibNEW();
			if(creditavl!=NULL) freeapplib(creditavl);
			creditavl=APPlibNEW();
			if(acctavl!=NULL) freeapplib(acctavl);
			acctavl=APPlibNEW();
			if(defdebavl!=NULL) freeapplib(defdebavl);
			defdebavl=APPlibNEW();
			if(defcredavl!=NULL) freeapplib(defcredavl);
			defcredavl=APPlibNEW();
			if(defpayavl!=NULL) freeapplib(defpayavl);
			defpayavl=APPlibNEW();
			if(venpmt->use_venpmt_fund)
			{
				if(defduetoavl!=NULL) freeapplib(defduetoavl);
				defduetoavl=APPlibNEW();
			}
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
			FINDFLDSETINT(tempyrfile,"FISCAL YEAR",yearid);
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
			if(tmprsrc!=NULL)
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
					ForceWindowUpdate(tmprsrc);
			while(!ef)
			{
				listupdated=FALSE;
				FINDFLDGETSHORT(tempyrfile,"DEFINITION TYPE",&deftype1); 
					if(deftype!=deftype1) break;
				FINDFLDGETINT(tempyrfile,"FISCAL YEAR",&yearid1);
				FINDFLDGETSTRING(tempyrfile,"ACCOUNT CODE",&account_code);
				if(keyn!=(-1)) if(yearid1!=yearid) break;
				if(yearid1==yearid)
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
								if(selecteda==BAL_ACCT)
								{
									FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
									if(!RDAstrcmp(baltype,"ENCUMBRANCE"))
									{
										addAPPlib(acctavl,account_code);
										listupdated=TRUE;
									}
								} else {
									addAPPlib(acctavl,account_code);
									listupdated=TRUE;
								}
							}
						}
					}
				} /* end else */
				if(account_code!=NULL) Rfree(account_code);
				if(baltype!=NULL) Rfree(baltype);
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
					FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
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
						if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
							ForceWindowUpdate(tmprsrc);
					while(!ef)
					{
						listupdated=FALSE;
						FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",&deftype1);
						if(deftype!=deftype1) break;
						FINDFLDGETINT(byrnum,"FISCAL YEAR",&yearid1);
						if(keyn!=(-1))
						{
							if(yearid1!=yearid) break;
						} 
						if(yearid1==yearid) 
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
										FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
										if(!RDAstrcmp(baltype,"ENCUMBRANCE"))
										{
											addAPPlib(debitavl,account_code);
										} else if(!RDAstrcmp(baltype,"RESERVE FOR ENCUMBRANCE"))
										{
											addAPPlib(creditavl,account_code);
										} 
										if(!RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
										{
											addAPPlib(defpayavl,account_code);
										}
										if(venpmt->use_venpmt_fund)
										{
										if(!RDAstrcmp(baltype,"DUE TO"))
										{
											addAPPlib(defduetoavl,account_code);
										}
										}
										addAPPlib(defcredavl,account_code);
										addAPPlib(defdebavl,account_code);
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
					break;
				case REV_ACCT: /* Revenue */
					ZERNRD(byrnum);
					FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
					FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
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
						if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
							ForceWindowUpdate(tmprsrc);
					while(!ef)
					{
						listupdated=FALSE;
						FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",
							&deftype);
						if(deftype!=selectedd) break;
						FINDFLDGETINT(byrnum,"FISCAL YEAR",
							&yearid1);
						if(keyn!=(-1))
						{
							if(yearid1!=yearid) break;
						} 
						if(yearid1==yearid) 
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
										if(!RDAstrcmp(baltype,"ENCUMBRANCE"))
										{
											addAPPlib(debitavl,account_code);
										} else if(!RDAstrcmp(baltype,"RESERVE FOR ENCUMBRANCE"))
										{
											addAPPlib(creditavl,account_code);
										}
										if(!RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
										{
											addAPPlib(defpayavl,account_code);
										}
										if(venpmt->use_venpmt_fund)
										{
										if(!RDAstrcmp(baltype,"DUE TO"))
										{
											addAPPlib(defduetoavl,account_code);
										}
										}
										addAPPlib(defcredavl,account_code);
										addAPPlib(defdebavl,account_code);
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
					break;
				case BAL_ACCT: /* Balance Sheet */
					ZERNRD(byrnum);
					FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
					FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
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
					e=NULL;
					if(tmprsrc!=NULL)
						if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
							ForceWindowUpdate(tmprsrc);
					while(!ef)
					{
						listupdated=FALSE;
						FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",&deftype);
						if(deftype!=selectedd) break;
						FINDFLDGETINT(byrnum,"FISCAL YEAR",&yearid1);
						if(keyn!=(-1))
						{
							if(yearid1!=yearid) break;
						} 
						if(yearid1==yearid) 
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
										if(!RDAstrcmp(baltype,"RESERVE FOR ENCUMBRANCE"))
										{
											addAPPlib(creditavl,account_code);
										}
										if(!RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
										{
											addAPPlib(defpayavl,account_code);
										}
										if(venpmt->use_venpmt_fund)
										{
										if(!RDAstrcmp(baltype,"DUE TO"))
										{
											addAPPlib(defduetoavl,account_code);
										}
										}
										addAPPlib(defcredavl,account_code);
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
					addAPPlib(defdebavl,"Not Applicable");
					break;
				default:
					prterr("Invalid Account Type [%d]",selecteda);
					break;
			}
			account_type=selecteda;
			definition_type=selectedd;
			year_identification=yearid;
		}
	}
	if(actyp!=NULL) Rfree(actyp);
}
static void changeclearCODElist(RDArsrc *parent)
{
	int selectedd=0,yearid1=0;
	short ef=0,deftype=0,deftype1=0;
	short keyn=0,keynumber=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL,*e=NULL,*actyp=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"DEFAULT CLEARING DEFINITION TYPES",&selectedd)) return;
	deftype=(short)selectedd;
	FINDRSCSETDEFTYPE(parent,"DEFAULT CLEARING DUE FROM SEARCH",deftype);
	FINDRSCSETDEFTYPE(parent,"DEFAULT CLEARING CASH SEARCH",deftype);
	if(AddList==TRUE)
	{
		if((selectedd!=clear_definition_type ||
			year_identification!=yearid) && AddList)
		{
			if(defclearfromavl!=NULL) freeapplib(defclearfromavl);
			defclearfromavl=APPlibNEW();
			if(defclearcashavl!=NULL) freeapplib(defclearcashavl);
			defclearcashavl=APPlibNEW();
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",yearid);
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
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
			while(!ef)
			{
				listupdated=FALSE;
				FINDFLDGETSHORT(byrnum,"DEFINITION TYPE",&deftype1);
				if(deftype!=deftype1) break;
				FINDFLDGETINT(byrnum,"FISCAL YEAR",&yearid1);
				if(keyn!=(-1))
				{
					if(yearid1!=yearid) break;
				} 
				if(yearid1==yearid) 
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
								FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
								if(!RDAstrcmp(baltype,"DUE TO"))
								{
									addAPPlib(defclearfromavl,account_code);
								} else if(!RDAstrcmp(baltype,"CASH"))
								{
									addAPPlib(defclearcashavl,account_code);
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
			}
			clear_definition_type=selectedd;
			year_identification=yearid;
		}
	}
	if(actyp!=NULL) Rfree(actyp);
}
static void changeclearDEFtype(RDArsrc *parent)
{
	int seldc=0,seldp=0;
	int selecteddef=0;

	FINDRSCGETINT(parent,"DEFAULT CLEARING DEFINITION TYPES",&selecteddef);
	if(selecteddef!=clear_definition_type)
	{
		year_identification=(-1);
		FINDRSCGETINT(parent,"DEFAULT CLEARING DUE FROM CODES",&seldp);
		FINDRSCGETINT(parent,"DEFAULT CLEARING CASH CODES",&seldc);
		changeclearCODElist(parent);
		if(AddList)
		{
			if(defclearfromavl->numlibs<1)
			{
				addAPPlib(defclearfromavl,"No Accounts Available");
			}
			if(seldp>=defclearfromavl->numlibs) seldp=0;
			if(!FINDRSCLISTAPPlib(parent,"DEFAULT CLEARING DUE FROM CODES",seldp,defclearfromavl))
			{
				updatersrc(parent,"DEFAULT CLEARING DUE FROM CODES");
			}
			if(defclearcashavl->numlibs<1)
			{
				addAPPlib(defclearcashavl,"No Accounts Available");
			}
			if(seldc>=defclearcashavl->numlibs) seldc=0;
			if(!FINDRSCLISTAPPlib(parent,"DEFAULT CLEARING CASH CODES",seldc,defclearcashavl))
			{
				updatersrc(parent,"DEFAULT CLEARING CASH CODES");
			}
		}
		defclearfromnum=7;
		getsearchstr(parent,&defclearfromnum); 
		defclearcashnum=8;
		getsearchstr(parent,&defclearcashnum); 
	}
}
static void changeDEFtype(RDArsrc *parent)
{
	int sela=0,self=0,seld=0,selc=0,seldd=0,seldc=0,seldp=0;
	int selecteddef=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&selecteddef);
	if(selecteddef!=definition_type)
	{
		year_identification=(-1);
		FINDRSCGETINT(parent,"ACCOUNT CODES",&sela);
		FINDRSCGETINT(parent,"DEBIT CODES",&seld);
		FINDRSCGETINT(parent,"CREDIT CODES",&selc);
		FINDRSCGETINT(parent,"DEFAULT DEBIT CODES",&seldd);
		FINDRSCGETINT(parent,"DEFAULT PAYABLE CODES",&seldp);
		FINDRSCGETINT(parent,"DEFAULT CREDIT CODES",&seldc);
		if(venpmt->use_venpmt_fund)
		{
			FINDRSCGETINT(parent,"DEFAULT DUE TO CODES",&self);
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
				updatersrc(parent,"ACCOUNT CODES"); 
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
			if(defdebavl->numlibs<1)
			{
				addAPPlib(defdebavl,"No Accounts Available");
			}
			if(seldd>=defdebavl->numlibs) seldd=0;
			if(!FINDRSCLISTAPPlib(parent,"DEFAULT DEBIT CODES",seldd,defdebavl))
			{
				updatersrc(parent,"DEFAULT DEBIT CODES");
			}
			if(defpayavl->numlibs<1)
			{
				addAPPlib(defpayavl,"No Accounts Available");
			}
			if(seldp>=defpayavl->numlibs) seldp=0;
			if(!FINDRSCLISTAPPlib(parent,"DEFAULT PAYABLE CODES",seldp,defpayavl))
			{
				updatersrc(parent,"DEFAULT PAYABLE CODES");
			}
			if(defcredavl->numlibs<1)
			{
				addAPPlib(defcredavl,"No Accounts Available");
			}
			if(seldc>=defcredavl->numlibs) seldc=0;
			if(!FINDRSCLISTAPPlib(parent,"DEFAULT CREDIT CODES",seldc,defcredavl))
			{
				updatersrc(parent,"DEFAULT CREDIT CODES");
			}
			if(venpmt->use_venpmt_fund)
			{
			if(defduetoavl->numlibs<1)
			{
				addAPPlib(defduetoavl,"No Accounts Available");
			}
			if(seldc>=defduetoavl->numlibs) seldc=0;
			if(!FINDRSCLISTAPPlib(parent,"DEFAULT DUE TO CODES",seldc,defduetoavl))
			{
				updatersrc(parent,"DEFAULT DUE TO CODES");
			}
			}
		}
		debitcodenum=1;
		getsearchstr(parent,&debitcodenum);
		creditcodenum=2;
		getsearchstr(parent,&creditcodenum); 
		defdebcodenum=3;
		getsearchstr(parent,&defdebcodenum); 
		defpaycodenum=4;
		getsearchstr(parent,&defpaycodenum); 
		defcredcodenum=5;
		getsearchstr(parent,&defcredcodenum); 
		if(venpmt->use_venpmt_fund)
		{
			defduetonum=6;
			getsearchstr(parent,&defduetonum);
		}
		getacctdesc(parent);
	}
}
static void clearDEFtypecb(RDArsrc *parent)
{
	FINDRSCSETSTRING(parent,"DEFAULT CLEARING DUE FROM SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT CLEARING CASH SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT CLEARING CASH DESCRIPTION",NULL);
	changeclearDEFtype(parent);
}
static void DEFtypecb(RDArsrc *parent)
{
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT PAYABLE CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT PAYABLE CODE DESCRIPTION",NULL);
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCSETSTRING(parent,"DEFAULT DUE TO SEARCH",NULL);
		FINDRSCSETSTRING(parent,"DEFAULT DUE TO DESCRIPTION",NULL);
	}
	changeDEFtype(parent);
}
static void ACCTtypecb(RDArsrc *parent)
{
	int dtype=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&dtype);
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT DEBIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT CREDIT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT PAYABLE CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"DEFAULT PAYABLE CODE DESCRIPTION",NULL);
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCSETSTRING(parent,"DEFAULT DUE TO SEARCH",NULL);
		FINDRSCSETSTRING(parent,"DEFAULT DUE TO DESCRIPTION",NULL);
	}
	changeACCTtype(parent);
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	if(dtype>=DEFtypes->numlibs) dtype=0;
	if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",dtype,DEFtypes))
	{
		updatersrc(parent,"DEFINITION TYPES");
	}
	changeDEFtype(parent);
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
}
static void changeLINES(RDArsrc *parent)
{
	int sellineno=0;

	FINDRSCGETINT(parent,"LINE NUMBERS",&sellineno);
	lineno=Ratoi(linesavl->libs[sellineno]);
	FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
	FINDFLDSETINT(linenum,"LINE NUMBER",lineno);
	if(ADVEQLNRDsec(linenum,1,SCRNvirtualSubData,parent))
	{
		KEYNRD(linenum,1);
		FINDFLDGETINT(linenum,"LINE NUMBER",&lineno);
		if(lineno==0)
		{
			FINDFLDSETSTRING(linenum,"DESCRIPTION","Not Associated");
		}
	}
	updatefilerecord2rsrc(linenum,parent);
	total_lineout=getout_amt(parent);
	FINDRSCSETDOUBLE(parent,"[PURTRN][AMOUNT]",total_lineout);
	updatersrc(parent,"[PURTRN][AMOUNT]");
	updatersrc(parent,"OUTSTANDING LINE AMOUNT");
	updateSCRNvirtuals(parent);
}
static void SETFILES(RDArsrc *parent)
{
	readallwidgets(parent);
	IncVirRsrcstoFile(parent);
	getsupportingrecords(parent);
	updateSCRNvirtuals(parent);
}
void addtransactions(void)
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	int w,start=0;
	RDAacct *acct=NULL;
	double zero=0.0;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL;
	char *name=NULL;

	nokeys=NUMKEYS(trannum);
	keys=KEYPOINTER(trannum);
	nofields=NUMFLDS(trannum);
	fields=FLDPOINTER(trannum);
	if(AddList)
	{
		parent=RDArsrcNEW(module,"ADD PO TRANSACTIONS");
	} else {
		parent=RDArsrcNEW(module,"ADD PO TRANSACTIONS W/O LISTS");
	}
	definelist=RDArsrcNEW(module,"ADD PO TRANSACTIONS DEFINE LIST");
 	addDFincvir(parent,module,"PURMST",NULL,msrnum);
 	addDFincvir(parent,module,"PURLIN",NULL,linenum);
 	addDFincvir(parent,module,"PURACC",NULL,accnum);
 	addDFincvir(parent,module,"PURTRN",NULL,trannum);
 	addDFincvir(parent,"FINMGT","FINBACC",NULL,balnum);
 	addDFincvir(parent,"FINMGT","FINRACC",NULL,revnum);
 	addDFincvir(parent,"FINMGT","FINEACC",NULL,expnum);
 	addDFincvir(parent,"FINMGT","FINRYR",NULL,ryrnum);
 	addDFincvir(parent,"FINMGT","FINBYR",NULL,byrnum);
 	addDFincvir(parent,"FINMGT","FINEYR",NULL,eyrnum);
 	addDFincvir(parent,"FINMGT","FINYEAR",NULL,finyrnum);
	if(banknum!=(-1))
	{
 		addDFincvir(parent,"BNKREC","BNKACC",NULL,banknum);
	}
 	addDFincvir(definelist,module,"PURTRN",NULL,trannum);
 	addDFincvir(definelist,module,"PURACC",NULL,accnum);
 	addDFincvir(definelist,module,"PURLIN",NULL,linenum);
 	addDFincvir(definelist,module,"PURMST",NULL,msrnum);
 	GET_SCREEN_VIRTUALS(parent,0);
 	GET_SCREEN_VIRTUALS(definelist,1);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	if(linenum!=(-1)) file2rsrc(linenum,parent,FALSE);
	if(vennum!=(-1)) file2rsrc(vennum,parent,FALSE);
	if(msrnum!=(-1)) file2rsrc(msrnum,parent,FALSE);
	if(finyrnum!=(-1)) file2rsrc(finyrnum,parent,FALSE);
	if(banknum!=(-1)) file2rsrc(banknum,parent,FALSE);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
			{
				edit_rsrc=FALSE;
				nonlstmakefld(parent,trannum,fldx,edit_rsrc);
				addstdrsrc(parent,"[FINYEAR][DESCRIPTION]",PLAINTEXT,40,NULL,FALSE);
			} else if(!RDAstrcmp(fldx->name,"PO IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				nonlstmakefld(parent,trannum,fldx,edit_rsrc);
			} else if(!RDAstrcmp(fldx->name,"LINE NUMBER"))
			{
				edit_rsrc=TRUE;
                                if(FIELDscrn(trannum,fldx->name,&edit_rsrc))
                                {
					getlines(parent);
					sprintf(stemp,"%4d",lineno);
					for(start=0;start<linesavl->numlibs;++start)
					{
						if(!RDAstrcmp(stemp,linesavl->libs[start])) break;
					}
					if(start>=linesavl->numlibs) start=0;
					addlstrsrc(parent,"LINE NUMBERS",
						&start,TRUE,changeLINES,
						linesavl->numlibs,
						&linesavl->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,
							"LINE NUMBERS",
							(edit_rsrc ? TRUE:FALSE));
                                }
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,trannum,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(trannum)));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(trannum)));
				}
				sprintf(name,"[%s][%s]",FILENAME(trannum),fldx->name);
				FINDRSCSETFUNC(parent,name,SETFILES,NULL);
			}
		}
	}
	nokeys=0;
	nofields=0;
	fields=NULL;
	keys=NULL;
	nokeys=NUMKEYS(accnum);
	keys=KEYPOINTER(accnum);
	nofields=NUMFLDS(accnum);
	fields=FLDPOINTER(accnum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"LINE NUMBER"))
			{
			} else if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
			{
			} else if(!RDAstrcmp(fldx->name,"PO IDENTIFICATION"))
			{
			} else if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				ACCTtypes=APPlibNEW();
				addAPPlib(ACCTtypes,accttypes[0]);
				addAPPlib(ACCTtypes,accttypes[1]);
				addAPPlib(ACCTtypes,accttypes[2]);
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if((-1)<account_type && account_type<ACCTtypes->numlibs)
					{
						start=(int)account_type;
					} else start=0;
					addlstrsrc(parent,"ACCOUNT TYPES",
						&start,TRUE,ACCTtypecb,
						ACCTtypes->numlibs,
						&ACCTtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,
							"ACCOUNT TYPES",
							(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(DEFtypes==NULL)
					{
						DEFtypes=APPlibNEW();
						addAPPlib(DEFtypes,"No Definitions Available");
					}
					if((-1)<definition_type && definition_type<DEFtypes->numlibs)
					{	
						start=(int)definition_type;
					} else start=0;
					addlstrsrc(parent,"DEFINITION TYPES",
						&start,TRUE,DEFtypecb,
						DEFtypes->numlibs,
						&DEFtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,
							"DEFINITION TYPES",
							(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"ACCOUNT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(acctavl==NULL)
							acctavl=APPlibNEW();
						start=0;
						addlstrsrc(parent,"ACCOUNT CODES",
							&start,TRUE,
							getacctdesc,
							acctavl->numlibs,
							&acctavl->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"ACCOUNT CODES",FALSE);
					}
					addstdrsrc(parent,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"ACCOUNT CODE SEARCH",EXPENDITURE,40,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"ACCOUNT CODE SEARCH",acctsearch,&acctcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DEBIT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(debitavl==NULL)
							debitavl=APPlibNEW();
						debitcodenum=1;
						start=0;
						addlstrsrc(parent,"DEBIT CODES",
							&start,TRUE,getsearchstr,
							debitavl->numlibs,
							&debitavl->libs,
							&debitcodenum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,
								"DEBIT CODES",FALSE);
					}
					addstdrsrc(parent,"DEBIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEBIT CODE SEARCH",BALANCESHEET,40,NULL,TRUE);
					debitcodenum=1;
					FINDRSCSETFUNC(parent,"DEBIT CODE SEARCH",acctsearch,&debitcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"CREDIT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(creditavl==NULL)
							creditavl=APPlibNEW();
						start=0;
						creditcodenum=2;
						addlstrsrc(parent,"CREDIT CODES",
							&start,TRUE,getsearchstr,
							creditavl->numlibs,
							&creditavl->libs,
							&creditcodenum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"CREDIT CODES",FALSE);
					}
					addstdrsrc(parent,"CREDIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"CREDIT CODE SEARCH",BALANCESHEET,40,NULL,TRUE);
					creditcodenum=2;
					FINDRSCSETFUNC(parent,"CREDIT CODE SEARCH",acctsearch,&creditcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT DEBIT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(defdebavl==NULL)
							defdebavl=APPlibNEW();
						start=0;
						defdebcodenum=3;
						addlstrsrc(parent,
							"DEFAULT DEBIT CODES",
							&start,TRUE,getsearchstr,
							defdebavl->numlibs,
							&defdebavl->libs,
							&defdebcodenum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"DEFAULT DEBIT CODES",FALSE);
					}
					addstdrsrc(parent,"DEFAULT DEBIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEFAULT DEBIT CODE SEARCH",BALANCESHEET,40,NULL,TRUE);
					defdebcodenum=3;
					FINDRSCSETFUNC(parent,"DEFAULT DEBIT CODE SEARCH",acctsearch,&defdebcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT PAYABLE CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(defpayavl==NULL) 
							defpayavl=APPlibNEW();
						start=0;
						defpaycodenum=4;
						addlstrsrc(parent,
							"DEFAULT PAYABLE CODES",
							&start,TRUE,getsearchstr,
							defpayavl->numlibs,
							&defpayavl->libs,
							&defpaycodenum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"DEFAULT PAYABLE CODES",FALSE);
					}
					addstdrsrc(parent,"DEFAULT PAYABLE CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEFAULT PAYABLE CODE SEARCH",BALANCESHEET,40,NULL,TRUE);
					defpaycodenum=4;
					FINDRSCSETFUNC(parent,"DEFAULT PAYABLE CODE SEARCH",acctsearch,&defpaycodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT CLEARING DEFINITION TYPE"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(clearDEFtypes==NULL)
					{
						clearDEFtypes=APPlibNEW();
						if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
						{
							for(w=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
								w<BALANCE_SHEET_ACCOUNT->num;++w,++acct)
							{
								addAPPlib(clearDEFtypes,acct->name);
							}
						}
						if(clearDEFtypes->numlibs<1)
						{
							addAPPlib(clearDEFtypes,"No Definitions Available");
						}
					}
					if((-1)<clear_definition_type && clear_definition_type<clearDEFtypes->numlibs)
					{	
						start=(int)clear_definition_type;
					} else start=0;
					addlstrsrc(parent,"DEFAULT CLEARING DEFINITION TYPES",
						&start,TRUE,clearDEFtypecb,
						clearDEFtypes->numlibs,
						&clearDEFtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(parent,
							"DEFAULT CLEARING DEFINITION TYPES",
							(edit_rsrc ? TRUE:FALSE));
				}
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT DUE TO"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(defduetoavl==NULL)
							defduetoavl=APPlibNEW();
						start=0;
						defduetonum=6;
						addlstrsrc(parent,
							"DEFAULT DUE TO CODES",
							&start,TRUE,getsearchstr,
							defduetoavl->numlibs,
							&defduetoavl->libs,
							&defduetonum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"DEFAULT CREDIT CODES",FALSE);
					}
					addstdrsrc(parent,"DEFAULT DUE TO DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEFAULT DUE TO SEARCH",BALANCESHEET,40,NULL,TRUE);
					defduetonum=6;
					FINDRSCSETFUNC(parent,"DEFAULT DUE TO SEARCH",acctsearch,&defduetonum);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT CLEARING DUE FROM"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(defclearfromavl==NULL)
							defclearfromavl=APPlibNEW();
						start=0;
						defclearfromnum=7;
						addlstrsrc(parent,
							"DEFAULT CLEARING DUE FROM CODES",
							&start,TRUE,getsearchstr,
							defclearfromavl->numlibs,
							&defclearfromavl->libs,
							&defclearfromnum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"DEFAULT CLEARING DUE FROM CODES",FALSE);
					}
					addstdrsrc(parent,"DEFAULT CLEARING DUE FROM DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEFAULT CLEARING DUE FROM SEARCH",BALANCESHEET,40,NULL,TRUE);
					defclearfromnum=7;
					FINDRSCSETFUNC(parent,"DEFAULT CLEARING DUE FROM SEARCH",acctsearch,&defclearfromnum);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT CLEARING CASH"))
			{
				if(venpmt->use_venpmt_fund)
				{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(defclearcashavl==NULL)
							defclearcashavl=APPlibNEW();
						start=0;
						defclearcashnum=8;
						addlstrsrc(parent,
							"DEFAULT CLEARING CASH CODES",
							&start,TRUE,getsearchstr,
							defclearcashavl->numlibs,
							&defclearcashavl->libs,
							&defclearcashnum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"DEFAULT CLEARING CASH CODES",FALSE);
					}
					addstdrsrc(parent,"DEFAULT CLEARING CASH DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEFAULT CLEARING CASH SEARCH",BALANCESHEET,40,NULL,TRUE);
					defclearcashnum=8;
					FINDRSCSETFUNC(parent,"DEFAULT CLEARING CASH SEARCH",acctsearch,&defclearcashnum);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT CREDIT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(accnum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						if(defcredavl==NULL)
							defcredavl=APPlibNEW();
						start=0;
						defcredcodenum=5;
						addlstrsrc(parent,
							"DEFAULT CREDIT CODES",
							&start,TRUE,getsearchstr,
							defcredavl->numlibs,
							&defcredavl->libs,
							&defcredcodenum);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"DEFAULT CREDIT CODES",FALSE);
					}
					addstdrsrc(parent,"DEFAULT CREDIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(parent,"DEFAULT CREDIT CODE SEARCH",BALANCESHEET,40,NULL,TRUE);
					defcredcodenum=5;
					FINDRSCSETFUNC(parent,"DEFAULT CREDIT CODE SEARCH",acctsearch,&defcredcodenum);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,accnum,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(accnum)));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(accnum)));
				}
				sprintf(name,"[%s][%s]",FILENAME(accnum),fldx->name);
				FINDRSCSETFUNC(parent,name,SETFILES,NULL);
			}
		}
	}
	nofields=0;
	nokeys=0;
	if(name!=NULL) Rfree(name);
	if(prevtran!=NULL) freeapplib(prevtran);
	prevtran=APPlibNEW();
	addAPPlib(prevtran,"No PO Transactions added this session.");
	addlstrsrc(parent,"PREVIOUS TRANSACTIONS",&x,TRUE,NULL,prevtran->numlibs,
		&prevtran->libs,NULL);
	FINDRSCSETSTRING(parent,"[PURTRN][ENTRY DATE]",CURRENT_DATE);
	FINDRSCSETSTRING(parent,"[PURTRN][ENTRY TIME]",CURRENT_TIME);
	savelist(definelist,parent);
	FINDRSCSETINT(parent,"[PURTRN][FISCAL YEAR]",yearid);
	FINDRSCSETSTRING(parent,"[PURTRN][PO IDENTIFICATION]",POID);
	FINDRSCSETEDITABLE(parent,"[PURTRN][FISCAL YEAR]",FALSE);
	FINDRSCSETEDITABLE(parent,"[PURTRN][PO IDENTIFICATION]",FALSE);
	addstdrsrc(parent,"PO TOTAL",DOLLARS,20,&po_total,FALSE);
	addstdrsrc(parent,"OUTSTANDING LINE AMOUNT",DOLLARS,20,&zero,FALSE);
	addstdrsrc(parent,"TOTAL EXPENDITURE",DOLLARS,20,&total_exp,FALSE);
	addstdrsrc(parent,"TOTAL REVENUE",DOLLARS,20,&total_rev,FALSE);
	addstdrsrc(parent,"TOTAL DEBITS",DOLLARS,20,&total_deb,FALSE);
	addstdrsrc(parent,"TOTAL CREDITS",DOLLARS,20,&total_cred,FALSE);
	getsupportingrecords(parent);
 	SetVirFieldFuncs(parent);
 	SCRNvirtual2rsrc(parent);
	ReadRDAScrolledLists(parent);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_defaultscb,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addbtnrsrc(parent,"QUIT",TRUE,quit_check,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(parent);
	APPmakescrn(parent,TRUE,quit_record1,NULL,FALSE);
}
