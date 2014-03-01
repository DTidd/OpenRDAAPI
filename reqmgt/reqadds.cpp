/* reqadds.c - Add Requisition Items Single */
#define CLEARWDGT
#include <cstdio>
#include <cstdlib>

#include <mix.hpp>
#include <fin.hpp>
#define DIAGNOSTIC_SCREENS

extern short reqven,reqitm,expnum,balnum,reqmst,reqloc,reqctlg;
extern short byrnum,eyrnum,finyrnum,reqcat,vennum,reqacc,banknum;
extern short reqpven,venadrnum;
extern RDAfinmgt *finsetup;
extern int REQ_COUNT;
extern char *module,*locid,*reqid;
static char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL;
static char *defdebcode1=NULL,*defcredcode1=NULL,*defpaycode1=NULL;
RDArsrc *definelist=NULL;
APPlib *prevvch=NULL;
APPlib *DEFtypes=NULL,*addresslist=NULL;
APPlib *acctavl=NULL,*debitavl=NULL,*creditavl=NULL,*categories=NULL;
APPlib *defdebitavl=NULL,*defcreditavl=NULL,*defpayableavl=NULL;
APPlib *pmonth=NULL;
APPlib *yearavl=NULL;
void getBANKids(RDArsrc *);
void setBANKidscb(RDArsrc *);
void BANKidscb(RDArsrc *);
APPlib *BANKids=NULL;
short acctcodenum=0,debitcodenum=1,creditcodenum=2;
short defdebitcodenum=3,defcreditcodenum=4,defpayablecodenum=5;
void DEFtypecb(RDArsrc *);
void changeDEFtype(RDArsrc *,short);
static int getyears(RDArsrc *,RDAfinmgt *);
static void setyear(RDArsrc *);
static void changeyear(RDArsrc *,short);
static void changeyearcb(RDArsrc *);
void getacctdesc(RDArsrc *);
void changeCODElist(RDArsrc *);
static void set_defaults(RDArsrc *);
void savelist(RDArsrc *,RDArsrc *,short),savelistcb(RDArsrc *,RDArsrc *);
void definelistfunc(RDArsrc *,RDArsrc *),makedflist(void);
static void save_record(RDArsrc *),quit_record(RDArsrc *);
static void save_check(RDArsrc *),quit_check(RDArsrc *);
void acctsearch(RDArsrc *,short *,short);
void acctsearchcb(RDArsrc *,short *);
void getsearchstr(RDArsrc *,short *);
void clearwidget(RDArsrc *,char *);
static short check_acctcode(RDArsrc *,short);
void getsupportingfiles(RDArsrc *);
void browse_items(RDArsrc *);
extern void browse_venmsr(RDArsrc *);
void browse_vendors(RDArsrc *);
void browse_catalog(RDArsrc *);
void okitemfunc(MakeBrowseList *,void *);
void quititem(MakeBrowseList *);
void inititem(MakeBrowseList *);
short selectitem(MakeBrowseList *);
void changecategory(RDArsrc *);
void caltotalqty(RDArsrc *);
void caltotalur(RDArsrc *);
void checkvendorcb(RDArsrc *);
short checkvendor(RDArsrc *,short);
void okreqvenfunc(MakeBrowseList *,void *);
short selectreqven(MakeBrowseList *);
void initreqven(MakeBrowseList *);
int year_identification=(-1);
short definition_type=(-1);
double total_req=0.0;
void setaddresses(RDArsrc *);
void changeADDRlist(RDArsrc *);
void getaddr(RDArsrc *);
void getaddrcb(RDArsrc *);

void checkvendorcb(RDArsrc *mtnrsrc)
{
	char *venid=NULL;
	double percdisc=0.0;

	readwidget(mtnrsrc,"[REQITM][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[REQITM][VENDOR IDENTIFICATION]",&venid);
	FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",venid);
	if(venid!=NULL) Rfree(venid);
	checkvendor(mtnrsrc,TRUE);
	FINDFLDGETDOUBLE(vennum,"DEFAULT PERCENTAGE DISCOUNT",&percdisc);
	FINDRSCSETDOUBLE(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]",percdisc);
	updatersrc(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]");
	setaddresses(mtnrsrc);
	updatefilerecord2rsrc(vennum,mtnrsrc);	
	updateSCRNvirtuals(mtnrsrc);
}
short checkvendor(RDArsrc *mtnrsrc,short dg)
{
	char delflag=FALSE,*venid=NULL,*warnmessage=NULL;

	if(vennum!=(-1))
	{
		FINDFLDGETSTRING(reqitm,"VENDOR IDENTIFICATION",&venid);
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",venid);
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(vennum,1);
			if(dg)
			{
				warnmessage=Rmalloc(RDAstrlen(venid)+170);
				sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Vendor in the [VENPMT][VENMSR] data file.",(venid==NULL?"":venid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The VENDOR IDENTIFICATION of [%s] entered does not exist in the [VENPMT][VENMSR] data file.",(venid==NULL?"":venid));
				if(warnmessage!=NULL) Rfree(warnmessage);
			}
			if(venid!=NULL) Rfree(venid);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(vennum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				if(dg)
				{
					warnmessage=Rmalloc(RDAstrlen(venid)+200);
					sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is deleted.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a deleted state.",(venid==NULL?"":venid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The VENDOR IDENTIFICATION of [%s] entered is deleted.  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a deleted state.",(venid==NULL?"":venid));
					if(warnmessage!=NULL) Rfree(warnmessage);
				}
				if(venid!=NULL) Rfree(venid);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(vennum,"ACTIVE",&delflag);
				if(!delflag)
				{
					if(dg)
					{
						warnmessage=Rmalloc(RDAstrlen(venid)+200);
						sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not Active.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a non-Active state.",(venid==NULL?"":venid));
						WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
						prterr("The VENDOR IDENTIFICATION of [%s] entered is not Active.  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a non-Active state.",(venid==NULL?"":venid));
						if(warnmessage!=NULL) Rfree(warnmessage);
					}
					if(venid!=NULL) Rfree(venid);
					return(FALSE);
				} else {
					FINDFLDGETCHAR(vennum,"CATALOGUE VENDOR",&delflag);
					if(!delflag)
					{
						if(dg)
						{
							warnmessage=Rmalloc(RDAstrlen(venid)+250);
							sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not a Catalogue Vendor.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up as a non-Catalogue Vendor.",(venid==NULL?"":venid));
							WARNINGDIALOG("WARNING DIALOG SCREEN","NON_CATALOGUE VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
							prterr("The VENDOR IDENTIFICATION of [%s] entered is not a Catalogue Vendor .  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up as a non-Catalogue Vendor.",(venid==NULL?"":venid));
							if(warnmessage!=NULL) Rfree(warnmessage);
						}
						if(venid!=NULL) Rfree(venid);
						return(FALSE);
					}
				}
			}
		}
		if(venid!=NULL) Rfree(venid);
		if(warnmessage!=NULL) Rfree(warnmessage);
	}
	return(TRUE);
}
void caltotalqty(RDArsrc *mtnrsrc)
{
	int quantity=0,selcat=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;
	char *catid1=NULL,*venid1=NULL,*itemid1=NULL; 
	char *catid2=NULL,*venid2=NULL,*itemid2=NULL; 

	FINDRSCGETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	if(RDAstrcmp(categories->libs[selcat],"No Categories"))
	{
		catid1=stralloc(categories->libs[selcat]);
	}
	readwidget(mtnrsrc,"[REQITM][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[REQITM][VENDOR IDENTIFICATION]",&venid1);
	readwidget(mtnrsrc,"[REQITM][ITEM IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[REQITM][ITEM IDENTIFICATION]",&itemid1);
	readwidget(mtnrsrc,"[REQITM][QUANTITY]");
	if(FINDRSCGETINT(mtnrsrc,"[REQITM][QUANTITY]",&quantity)) return;
	readwidget(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]");
	if(FINDRSCGETDOUBLE(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc)) 
		return;
	ZERNRD(reqpven);
	FINDFLDSETSTRING(reqpven,"CATEGORY IDENTIFICATION",catid1);
	FINDFLDSETSTRING(reqpven,"ITEM IDENTIFICATION",itemid1);
	FINDFLDSETSTRING(reqpven,"VENDOR IDENTIFICATION",venid1);
	FINDFLDSETINT(reqpven,"QUANTITY",quantity);
	if(!ADVLTENRDsec(reqpven,1,SCRNvirtualSubData,mtnrsrc))
	{
		FINDFLDGETSTRING(reqpven,"VENDOR IDENTIFICATION",&venid2);		
		FINDFLDGETSTRING(reqpven,"ITEM IDENTIFICATION",&itemid2);
		FINDFLDGETSTRING(reqpven,"CATEGORY IDENTIFICATION",&catid2);
		if(RDAstrcmp(venid1,venid2) || RDAstrcmp(itemid1,itemid2) || RDAstrcmp(catid1,catid2))
		{
			readwidget(mtnrsrc,"[REQITM][UNIT RATE]");
			FINDRSCGETDOUBLE(mtnrsrc,"[REQITM][UNIT RATE]",&unitrate);
			readwidget(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]");
			FINDRSCGETDOUBLE(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
		} else {
			FINDFLDGETDOUBLE(reqpven,"UNIT RATE",&unitrate);
			FINDFLDGETDOUBLE(reqpven,"PERCENTAGE DISCOUNT",&percdisc);
		}
	} else {
		readwidget(mtnrsrc,"[REQITM][UNIT RATE]");
		FINDRSCGETDOUBLE(mtnrsrc,"[REQITM][UNIT RATE]",&unitrate);
		readwidget(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]");
		FINDRSCGETDOUBLE(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
	}
	FINDRSCSETDOUBLE(mtnrsrc,"[REQITM][UNIT RATE]",unitrate);
	updatersrc(mtnrsrc,"[REQITM][UNIT RATE]");
	FINDRSCSETDOUBLE(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]",percdisc);
	updatersrc(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]");
	if(percdisc!=0.0)
	{
		total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
	} else {
		total=round(((unitrate*quantity)*100.0));
	}
	FINDRSCSETDOUBLE(mtnrsrc,"[REQITM][AMOUNT]",total);
	updatersrc(mtnrsrc,"[REQITM][AMOUNT]");
	if(catid1!=NULL) Rfree(catid1);
	if(catid2!=NULL) Rfree(catid2);
	if(venid1!=NULL) Rfree(venid1);
	if(venid2!=NULL) Rfree(venid2);
	if(itemid1!=NULL) Rfree(itemid1);
	if(itemid2!=NULL) Rfree(itemid2);
	updateSCRNvirtuals(mtnrsrc);
}
void caltotalur(RDArsrc *mtnrsrc)
{
	int quantity=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;

	readwidget(mtnrsrc,"[REQITM][QUANTITY]");
	if(FINDRSCGETINT(mtnrsrc,"[REQITM][QUANTITY]",&quantity)) return;
	readwidget(mtnrsrc,"[REQITM][UNIT RATE]");
	if(FINDRSCGETDOUBLE(mtnrsrc,"[REQITM][UNIT RATE]",&unitrate)) return;
	readwidget(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]");
	if(FINDRSCGETDOUBLE(mtnrsrc,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc)) return;
	if(percdisc!=0.0)
	{
		total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
	} else {
		total=round(((unitrate*quantity)*100.0));
	}
	FINDRSCSETDOUBLE(mtnrsrc,"[REQITM][AMOUNT]",total);
	updatersrc(mtnrsrc,"[REQITM][AMOUNT]");
	updateSCRNvirtuals(mtnrsrc);
}
void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent,FALSE);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
void savelist(RDArsrc *definelist,RDArsrc *parent,short maketotal)
{
	short ef=0;
	char *username=NULL,deleteflag=FALSE,verified=FALSE;
	char *venid=NULL,*iid=NULL;
	RDArsrc *tmprsrc=NULL;
	double total=0.0;

	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(prevvch!=NULL) freeapplib(prevvch);
	prevvch=APPlibNEW();
#ifdef DIAGNOSTIC_SCREENS
	tmprsrc=diagscrn(reqitm,"DIAGNOSTIC SCREEN",module,
		"Reading Requisition Items",NULL);
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
	ZERNRD(reqitm);
	FINDFLDSETSTRING(reqitm,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(reqitm,"REQUISITION IDENTIFICATION",reqid);
	ef=ADVGTENRDsec(reqitm,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		if(venid!=NULL) Rfree(venid);
		FINDFLDGETSTRING(reqitm,"LOCATION IDENTIFICATION",&venid);
		if(iid!=NULL) Rfree(iid); 
		FINDFLDGETSTRING(reqitm,"REQUISITION IDENTIFICATION",&iid);
		if(RDAstrcmp(venid,locid) || RDAstrcmp(iid,reqid)) break;
		FINDFLDGETCHAR(reqitm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETCHAR(reqitm,"SOURCE USER VERIFIED",&verified);
			if(!verified)
			{
				FINDFLDGETSTRING(reqitm,"SOURCE USER",&username);
				if(!RDAstrcmp(username,USERLOGIN))
				{
					if(maketotal)
					{
						FINDFLDGETDOUBLE(reqitm,"AMOUNT",&total);
						total_req+=total;
					}
					makedflist();
					if(tmprsrc!=NULL) 
						update_diagnostic(tmprsrc,TRUE);
				} else {
					if(tmprsrc!=NULL) 
						update_diagnostic(tmprsrc,FALSE);
				}
			}
		}
		ef=ADVNXTNRDsec(reqitm,1,SCRNvirtualSubData,parent);
	}
	if(venid!=NULL) Rfree(venid);
	if(iid!=NULL) Rfree(iid);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(prevvch->numlibs<1)
	{
		addAPPlib(prevvch,"No Requisition Items added this session");
	}
	if(definelist!=NULL)
	{
		if(!FINDRSCLISTAPPlib(parent,"PREVIOUS ITEM'S",(prevvch->numlibs-1),
			prevvch))
		{
			updatersrc(parent,"PREVIOUS ITEM'S");
		}
	}
	FINDRSCSETDOUBLE(parent,"TOTAL REQUISITION",total_req);
	updatersrc(parent,"TOTAL REQUISITION");
}
void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(prevvch!=NULL)
	{
		if(prevvch->numlibs==1 && !RDAstrcmp(prevvch->libs[0],
			"No Requisition Items added this session"))
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
					field=FLDNRD(reqitm,s);
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
						if(s!=NULL) Rfree(s);
					}
				}
			}
		}
		++cur;
	}
	addAPPlib(prevvch,hold);
	REQ_COUNT+=1;
	if(hold!=NULL) Rfree(hold);
}
void definelistfunc(RDArsrc *parent,RDArsrc *definelist)
{
	computeallSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void set_defaults(RDArsrc *parent)
{
	int selprev=0,selBANKid=0;
	char *venid=NULL;
	double total=0;

	FINDRSCGETINT(parent,"PREVIOUS ITEM'S",&selprev);
	FINDRSCGETDOUBLE(parent,"TOTAL REQUISITION",&total);
	DefaultResetScreen(parent,TRUE);
	FINDRSCSETSTRING(parent,"[REQITM][LOCATION IDENTIFICATION]",locid);
	FINDRSCSETSTRING(parent,"[REQITM][REQUISITION IDENTIFICATION]",reqid);
	if(banknum!=(-1))
	{
		FINDRSCGETINT(parent,"BANK IDENTIFICATIONS",&selBANKid);
		if(selBANKid>=BANKids->numlibs) selBANKid=0;
		if(RDAstrcmp(BANKids->libs[selBANKid],"No Banks Defined."))
		{
			FINDRSCSETSTRING(parent,"DEFAULT BANK IDENTIFICATION",BANKids->libs[selBANKid]);
		} else {
			FINDRSCSETSTRING(parent,"DEFAULT BANK IDENTIFICATION",NULL);
		}
	}
	FINDRSCSETINT(parent,"PREVIOUS ITEM'S",selprev);
	FINDRSCSETDOUBLE(parent,"TOTAL REQUISITION",total);
	getsupportingfiles(parent);
	changecategory(parent);
	changeyear(parent,TRUE);
/*
	changeDEFtype(parent,TRUE);
*/
	readwidget(parent,"[REQITM][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[REQITM][VENDOR IDENTIFICATION]",&venid);
	FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",venid);
	if(venid!=NULL) Rfree(venid);
	checkvendor(parent,FALSE);
	getacctdesc(parent);
	debitcodenum=1;
	getsearchstr(parent,&debitcodenum);
	creditcodenum=2;
	getsearchstr(parent,&creditcodenum);
	defdebitcodenum=3;
	getsearchstr(parent,&defdebitcodenum);
	defcreditcodenum=4;
	getsearchstr(parent,&defcreditcodenum);
	defpayablecodenum=5;
	getsearchstr(parent,&defpayablecodenum);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
void clearwidget(RDArsrc *parent,char *widgetname)
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
	int selectedd=0,selyear=0,procyrid=0;

	if(parent==NULL) return(FALSE);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	deftype1=(short)selectedd;
	FINDRSCGETINT(parent,"FISCAL YEARS",&selyear);
	procyrid=atoi(yearavl->libs[selyear]);
	tempyrfile=eyrnum;
	tempaccfile=expnum;
	switch(listtype)
	{
		case 0:	/* check account code value */
			if(acctcode1!=NULL) Rfree(acctcode1);
			FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",tmp);
			updatersrc(parent,"ACCOUNT DESCRIPTION");
			readwidget(parent,"ACCOUNT CODE");
			FINDRSCGETSTRING(parent,"ACCOUNT CODE",&acctcode1);
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
				tmp=Rmalloc(RDAstrlen(acctcode1)+212);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is not a DEFINED ACCOUNT CODE.  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",acctcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempaccfile,"DELETEFLAG",&delflag);
			if(delflag)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(acctcode1)+130);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",acctcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
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
				clearwidget(parent,"ACCOUNT CODE");
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
				tmp=Rmalloc(RDAstrlen(acctcode1)+261);
				sprintf(tmp,
					"The ACCOUNT CODE [%s] that was entered is not a DEFINED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",
					acctcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(acctcode1)+180);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is a DELETED YEAR ACCOUNT CODE for the YEAR [%d].  Check to see if this account should not be deleted.",acctcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(tempyrfile,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(acctcode1)+130);
				sprintf(tmp,"The ACCOUNT CODE [%s] that was entered is an INACTIVE YEAR ACCOUNT CODE for the YEAR [%d].",acctcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"ACCOUNT CODE");
				clearwidget(parent,"ACCOUNT DESCRIPTION");
				prterr(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			}
			getacctdesc(parent);
			break;
		case 1:	/* check debit code value */
			if(debcode1!=NULL) Rfree(debcode1); 
			readwidget(parent,"DEBIT CODE");
			FINDRSCGETSTRING(parent,"DEBIT CODE",&debcode1);
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",debcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+200);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",debcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(debcode1)+140);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",debcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE");
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
				clearwidget(parent,"DEBIT CODE");
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
				tmp=Rmalloc(RDAstrlen(debcode1)+380);
				sprintf(tmp,
					"The DEBIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",
					debcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+200);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",debcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(debcode1)+150);
				sprintf(tmp,"The DEBIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",debcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEBIT CODE");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(debcode1!=NULL) Rfree(debcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"ENCUMBRANCE"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ENCUMBRANCE for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
				prterr("The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ENCUMBRANCE for the ACCOUNT TYPE EXPENDITURE.");
				clearwidget(parent,"DEBIT CODE");
				clearwidget(parent,"DEBIT CODE DESCRIPTION");
				if(debcode1!=NULL) Rfree(debcode1);
				if(baltype!=NULL) Rfree(baltype);
				return(FALSE);
			} 
			break;
		case 2:	/* check credit code value */
			if(credcode1!=NULL) Rfree(credcode1);
			readwidget(parent,"CREDIT CODE");
			FINDRSCGETSTRING(parent,"CREDIT CODE",&credcode1);
			ZERNRD(balnum);
			FINDFLDSETCHAR(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",credcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(credcode1)+200);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",credcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE");
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
				clearwidget(parent,"CREDIT CODE");
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
				clearwidget(parent,"CREDIT CODE");
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
				tmp=Rmalloc(RDAstrlen(credcode1)+350);
				sprintf(tmp,
					"The CREDIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",credcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+155);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",credcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(credcode1)+250);
				sprintf(tmp,"The CREDIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",credcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"CREDIT CODE");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(credcode1!=NULL) Rfree(credcode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			/*if(RDAstrcmp(baltype,"RESERVE FOR ENCUMBRANCE"))*/
			if(RDAstrcmp(baltype,"BUDGETARY FD BAL-RESERVED ENCUMBRANCES"))
			{
				/*WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of RESERVE FOR ENCUMBRANCE for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);*/
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of BUDGETARY FD BAL-RESERVED ENCUMBRANCES for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
				/*prterr("The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of RESERVE FOR ENCUMBRANCE for the ACCOUNT TYPE EXPENDITURE.");*/
				prterr("The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of BUDGETARY FD BAL-RESERVED ENCUMBRANCES for the ACCOUNT TYPE EXPENDITURE.");
				clearwidget(parent,"CREDIT CODE");
				clearwidget(parent,"CREDIT CODE DESCRIPTION");
				if(credcode1!=NULL) Rfree(credcode1);
				if(baltype!=NULL) Rfree(baltype);
				return(FALSE);
			} 
			break;
		case 3:	/* check default debit code value */
			if(defdebcode1!=NULL) Rfree(defdebcode1);
			readwidget(parent,"DEFAULT DEBIT CODE");
			FINDRSCGETSTRING(parent,"DEFAULT DEBIT CODE",&defdebcode1);
			ZERNRD(balnum);
			FINDFLDSETCHAR(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defdebcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(defdebcode1)+250);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defdebcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1); 
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+180);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defdebcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				prterr(tmp);
				clearwidget(parent,"DEFAULT DEBIT CODE");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+150);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",credcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defdebcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+350);
				sprintf(tmp,
					"The DEFAULT DEBIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up."
,defdebcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+220);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defdebcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defdebcode1)+155);
				sprintf(tmp,"The DEFAULT DEBIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defdebcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT DEBIT CODE");
				clearwidget(parent,"DEFAULT DEBIT CODE DESCRIPTION");
				prterr(tmp);
				if(defdebcode1!=NULL) Rfree(defdebcode1);
				return(FALSE);
			}
			break;
		case 4:	/* check default credit code value */
			if(defcredcode1!=NULL) Rfree(defcredcode1);
			readwidget(parent,"DEFAULT CREDIT CODE");
			FINDRSCGETSTRING(parent,"DEFAULT CREDIT CODE",&defcredcode1);
			ZERNRD(balnum);
			FINDFLDSETCHAR(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defcredcode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(defcredcode1)+300);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defcredcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1); 
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+180);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defcredcode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				prterr(tmp);
				clearwidget(parent,"DEFAULT CREDIT CODE");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+150);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",credcode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defcredcode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+350);
				sprintf(tmp,
					"The DEFAULT CREDIT CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up."
,defcredcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+250);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defcredcode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defcredcode1)+200);
				sprintf(tmp,"The DEFAULT CREDIT CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defcredcode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT CREDIT CODE");
				clearwidget(parent,"DEFAULT CREDIT CODE DESCRIPTION");
				prterr(tmp);
				if(defcredcode1!=NULL) Rfree(defcredcode1);
				return(FALSE);
			}
			break;
		case 5:	/* check default payable code value */
			if(defpaycode1!=NULL) Rfree(defpaycode1);
			readwidget(parent,"DEFAULT PAYABLE CODE");
			FINDRSCGETSTRING(parent,"DEFAULT PAYABLE CODE",&defpaycode1);
			ZERNRD(balnum);
			FINDFLDSETCHAR(balnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",defpaycode1);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp); 
				tmp=Rmalloc(RDAstrlen(defpaycode1)+230);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is not a DEFINED BALANCE SHEET ACCOUNT CODE.  Check to see if it is defined.  If not and it is needed it needs to be set up.",defpaycode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1); 
				return(FALSE);
			}	
			FINDFLDGETCHAR(balnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+180);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is a DELETED ACCOUNT.  Check to see if this account should not be deleted.",defpaycode1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				prterr(tmp);
				clearwidget(parent,"DEFAULT PAYABLE CODE");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(balnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+150);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is an INACTIVE BALANCE SHEET ACCOUNT CODE.",defpaycode1); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			ZERNRD(byrnum);
			FINDFLDSETSHORT(byrnum,"DEFINITION TYPE",deftype1);
			FINDFLDSETINT(byrnum,"FISCAL YEAR",procyrid);
			FINDFLDSETSTRING(byrnum,"ACCOUNT CODE",defpaycode1);
			if(ADVEQLNRDsec(byrnum,1,SCRNvirtualSubData,parent))
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+350);
				sprintf(tmp,
					"The DEFAULT PAYABLE CODE [%s] that was entered is not a DEFINED YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].  Check to see if it is defined for the selected ACCOUNT TYPE and the selected DEFINITION TYPE.  If not and it is needed it needs to be set up.",defpaycode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","UNDEFINED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+255);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is a DELETED YEAR BALANCE SHEET ACCOUNT for the YEAR [%d].  Check to see if this account should not be deleted.",defpaycode1,procyrid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETCHAR(byrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				if(tmp!=NULL) Rfree(tmp);
				tmp=Rmalloc(RDAstrlen(defpaycode1)+200);
				sprintf(tmp,"The DEFAULT PAYABLE CODE [%s] that was entered is an INACTIVE YEAR BALANCE SHEET ACCOUNT CODE for the YEAR [%d].",defpaycode1,procyrid); 
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE YEAR BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(parent,"DEFAULT PAYABLE CODE");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				prterr(tmp);
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				return(FALSE);
			}
			FINDFLDGETSTRING(balnum,"REFERENCE TYPE",&baltype);
			if(RDAstrcmp(baltype,"ACCOUNTS PAYABLE"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INCORRECT BALANCE SHEET REFERENCE TYPE!","The DEFAULT PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ACCOUNTS PAYABLE for the ACCOUNT TYPE EXPENDITURE.",NULL,NULL,TRUE,NULL);
				prterr("The DEFAULT PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ACCOUNTS PAYABLE for the ACCOUNT TYPE EXPENDITURE.");
				clearwidget(parent,"DEFAULT PAYABLE CODE");
				clearwidget(parent,"DEFAULT PAYABLE CODE DESCRIPTION");
				if(defpaycode1!=NULL) Rfree(defpaycode1);
				if(baltype!=NULL) Rfree(baltype);
				return(FALSE);
			} 
			break;
		default: 
			prterr("Invalid Account List [%d]",listtype);
			return(FALSE);
	}
	if(baltype!=NULL) Rfree(baltype);
	return(TRUE);
}
void acctsearchcb(RDArsrc *parent,short *listtype)
{
	acctsearch(parent,listtype,TRUE);
}
void acctsearch(RDArsrc *parent,short *listtype,short docheck)
{
	char *searchwidget=NULL,*listwidget=NULL,*selectedc=NULL;
	APPlib *holdapplib=NULL;
	int x=0,len=0;

	switch(*listtype)
	{
		case 0:	 /* account code */
			searchwidget=stralloc("ACCOUNT CODE");
			break;
		case 1:	/* debit code */
			searchwidget=stralloc("DEBIT CODE");
			break;
		case 2:	/* credit code */
			searchwidget=stralloc("CREDIT CODE");
			break;
		case 3:	/* default debit code */
			searchwidget=stralloc("DEFAULT DEBIT CODE");
			break;
		case 4:	/* default credit code */
			searchwidget=stralloc("DEFAULT CREDIT CODE");
			break;
		case 5:	/* default payable code */
			searchwidget=stralloc("DEFAULT PAYABLE CODE");
			break;
		default: 
			prterr("Invalid Acount List [%d]",*listtype);
			break;
	}
	if(searchwidget==NULL) return;
	readwidget(parent,searchwidget);
	if(selectedc!=NULL) Rfree(selectedc);
	FINDRSCGETSTRING(parent,searchwidget,&selectedc);
	if(docheck==TRUE)
	{
		if(check_acctcode(parent,*listtype)==FALSE)
		{
			if(selectedc!=NULL) Rfree(selectedc);
			if(searchwidget!=NULL) Rfree(searchwidget);
			if(listwidget!=NULL) Rfree(listwidget);
			return;
		}
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
void getsearchstr(RDArsrc *parent,short *whichlist)
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
			searchwidget=stralloc("DEBIT CODE");
			descriptwidget=stralloc("DEBIT CODE DESCRIPTION");
			break;
		case 2:	 /* credit available */
			searchwidget=stralloc("CREDIT CODE");
			descriptwidget=stralloc("CREDIT CODE DESCRIPTION");
			break;
		case 3:	 /* default debit available */
			searchwidget=stralloc("DEFAULT DEBIT CODE");
			descriptwidget=stralloc("DEFAULT DEBIT CODE DESCRIPTION");
			break;
		case 4:	 /* default credit available */
			searchwidget=stralloc("DEFAULT CREDIT CODE");
			descriptwidget=stralloc("DEFAULT CREDIT CODE DESCRIPTION");
			break;
		case 5:	 /* default payable available */
			searchwidget=stralloc("DEFAULT PAYABLE CODE");
			descriptwidget=stralloc("DEFAULT PAYABLE CODE DESCRIPTION");
			break;
		default:
			prterr("Invalid Account List [%d]",*whichlist);
			break;
	}
	FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd);
	FINDRSCGETSTRING(parent,searchwidget,&accountcode);
	ZERNRD(balnum);
	deftype=(short)selectedd;
	FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype);
	FINDFLDSETSTRING(balnum,"ACCOUNT CODE",accountcode);
	if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent)) KEYNRD(balnum,1);
	FINDFLDGETSTRING(balnum,"DESCRIPTION",&descript);
	FINDRSCSETSTRING(parent,descriptwidget,descript);
	updatersrc(parent,descriptwidget);
	updatersrc(parent,searchwidget);
	if(listwidget!=NULL) Rfree(listwidget);
	if(searchwidget!=NULL) Rfree(searchwidget);
	if(descript!=NULL) Rfree(descript);
	if(descriptwidget!=NULL) Rfree(descriptwidget);
	if(accountcode!=NULL) Rfree(accountcode);
	updateSCRNvirtuals(parent);
}
void getacctdesc(RDArsrc *parent)
{
	int selacct=0,deftype=0;
	short filedeftype=0;
	char *descript=NULL;
	
	FINDRSCGETSTRING(parent,"ACCOUNT CODE",&descript);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&deftype);
	filedeftype=(short)deftype;
	ZERNRD(expnum);
	FINDFLDSETSHORT(expnum,"DEFINITION TYPE",filedeftype);
	FINDFLDSETSTRING(expnum,"ACCOUNT CODE",descript);
	if(ADVEQLNRDsec(expnum,1,SCRNvirtualSubData,parent)) KEYNRD(expnum,1);
	if(descript!=NULL) Rfree(descript); 
	FINDFLDGETSTRING(expnum,"DESCRIPTION",&descript);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",descript);
	updatersrc(parent,"ACCOUNT DESCRIPTION");
	if(descript!=NULL) Rfree(descript);
	updateSCRNvirtuals(parent);
}
static void save_check(RDArsrc *parent)
{
	int selacct=0,seldeb=0,selcred=0,seldeftype=0;
	int seldefdeb=0,seldefcred=0,seldefpay=0;
	int selmonth=0,selyear=0,procyrid=0,selcat=0,seladdr=0;
	short x=0,testapp=TRUE;
	char *acctstr=NULL,*header=NULL,*message=NULL;
	double amount=0.0;

	readallwidgets(parent);
	if(FINDRSCGETINT(parent,"ADDRESS IDENTIFICATIONS",&seladdr)) return;
	if(seladdr>=addresslist->numlibs)
	{
		header=stralloc("This Requisition Item doesn't contain a valid Vendor Address Identification, please select a valid Address Identification or set them up for this Vendor.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR ADDRESS IDENTIFICATION!",header,NULL,NULL,TRUE,NULL);
		if(header!=NULL) Rfree(header);
		return;
	}
	if(!RDAstrcmp(addresslist->libs[seladdr],"No Addresses Defined."))
	{
		header=stralloc("This Requisition Item doesn't contain a valid Vendor Address Identification, please select a valid Address Identification or set them up for this Vendor.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR ADDRESS IDENTIFICATION!",header,NULL,NULL,TRUE,NULL);
		if(header!=NULL) Rfree(header);
		return;
	}
	FINDFLDSETSTRING(reqitm,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	if(FINDRSCGETINT(parent,"PROCESSING MONTHS",&selmonth)) return;
	if(FINDRSCGETINT(parent,"CATEGORY IDENTIFICATIONS",&selcat)) return;
	procyrid=atoi(yearavl->libs[selyear]);
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(defdebcode1!=NULL) Rfree(defdebcode1);
	if(defcredcode1!=NULL) Rfree(defcredcode1);
	if(defpaycode1!=NULL) Rfree(defpaycode1);
	FINDRSCGETSTRING(parent,"ACCOUNT CODE",&acctcode1);
	FINDRSCGETSTRING(parent,"DEBIT CODE",&debcode1);
	FINDRSCGETSTRING(parent,"CREDIT CODE",&credcode1);
	FINDRSCGETSTRING(parent,"DEFAULT DEBIT CODE",&defdebcode1);
	FINDRSCGETSTRING(parent,"DEFAULT CREDIT CODE",&defcredcode1);
	FINDRSCGETSTRING(parent,"DEFAULT PAYABLE CODE",&defpaycode1);
	if(check_acctcode(parent,0)==FALSE) return;
	if(check_acctcode(parent,1)==FALSE) return;
	if(check_acctcode(parent,2)==FALSE) return;
	if(check_acctcode(parent,3)==FALSE) return;
	if(check_acctcode(parent,4)==FALSE) return;
	if(check_acctcode(parent,5)==FALSE) return;
	setBANKidscb(parent);
	if(FINDRSCGETDOUBLE(parent,"[REQITM][AMOUNT]",&amount)) return;
	if(amount==0.0) 
	{
		ERRORDIALOG("INVALID AMOUNT","This Requisition Items can not be saved with an AMOUNT of [$  0.00].  Please enter the correct amount for this requisition item.",NULL,FALSE);
		prterr("Attemped to save a requisition item with a zero dollar amount");
		return;
	}
	if(checkvendor(parent,TRUE)==FALSE) return;
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
			testapp=testapprotot(eyrnum,amount,pmonth->libs[selmonth]);
		}
		if(testapp==FALSE)
		{
			FINDFLDGETSHORT(eyrnum,"DEFINITION TYPE",&x);
			FINDFLDGETSTRING(eyrnum,"ACCOUNT CODE",&acctstr);
			FINDFLDGETINT(eyrnum,"FISCAL YEAR",&procyrid);
			header=stralloc("This Requisition Item will cause the POSTED APPROPRIATED amount to be less than all of the EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT [%s] in the YEAR [%d].");
			message=Rrealloc(message,RDAstrlen(header)+RDAstrlen(acctstr)+6);
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
}
static void save_record(RDArsrc *parent)
{
	int seldebit=0,selcredit=0,seldeftype=0,selcat=0;
	int seldefdebit=0,seldefcredit=0,seldefpayable=0;
	int selmonth=0,selyear=0,procyrid=0,selacct=0,seladdr=0;
	char *rcddesc=NULL,*errordesc=NULL,deleteflag=FALSE,prorate=TRUE;
	char *bankid=NULL;
	short x=0,deftype1=0;
	int lf=0,itemnumber=0;
	char *date=NULL,*timex=NULL;
	double amount=0.0,perc=100.0;
	RDArmem *member=NULL;

	if(FINDRSCGETINT(parent,"ADDRESS IDENTIFICATIONS",&seladdr)) return;
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(defdebcode1!=NULL) Rfree(defdebcode1);
	if(defcredcode1!=NULL) Rfree(defcredcode1);
	if(defpaycode1!=NULL) Rfree(defpaycode1);
	FINDRSCGETSTRING(parent,"ACCOUNT CODE",&acctcode1);
	FINDRSCGETSTRING(parent,"DEBIT CODE",&debcode1);
	FINDRSCGETSTRING(parent,"CREDIT CODE",&credcode1);
	FINDRSCGETSTRING(parent,"DEFAULT DEBIT CODE",&defdebcode1);
	FINDRSCGETSTRING(parent,"DEFAULT CREDIT CODE",&defcredcode1);
	FINDRSCGETSTRING(parent,"DEFAULT PAYABLE CODE",&defpaycode1);
	if(banknum!=(-1))
	{
		FINDRSCGETSTRING(parent,"DEFAULT BANK IDENTIFICATION",&bankid);
	}
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&seldeftype)) return;
	if(FINDRSCGETINT(parent,"PROCESSING MONTHS",&selmonth)) return;
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	if(FINDRSCGETDOUBLE(parent,"[REQITM][AMOUNT]",&amount)) return;
	if(FINDRSCGETINT(parent,"CATEGORY IDENTIFICATIONS",&selcat)) return;
	rsrc2filerecord(reqitm,parent);
	FINDFLDSETSTRING(reqitm,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
	ZERNRD(reqacc);
	procyrid=atoi(yearavl->libs[selyear]);
	FINDFLDSETCHAR(reqacc,"PRORATE",prorate);
	FINDFLDSETDOUBLE(reqacc,"PERCENTAGE",perc);
	deftype1=(short)seldeftype;
	FINDFLDSETSHORT(reqacc,"DEFINITION TYPE",deftype1);
	x=(short)selmonth;
	FINDFLDSETDOUBLE(reqitm,"AMOUNT",amount);
	FINDFLDSETSHORT(reqitm,"PROCESSING MONTH",x);
	FINDFLDSETINT(reqitm,"FISCAL YEAR",procyrid);
	if(RDAstrcmp(categories->libs[selcat],"No Categories"))
	{
		FINDFLDSETSTRING(reqitm,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
	} else FINDFLDSETSTRING(reqitm,"CATEGORY IDENTIFICATION",NULL);
	if(!RDAstrcmp(acctcode1,"Not Applicable") || 
		!RDAstrcmp(acctcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(reqacc,"ACCOUNT CODE",NULL);
	} else {
		FINDFLDSETSTRING(reqacc,"ACCOUNT CODE",acctcode1);
	}
	if(!RDAstrcmp(debcode1,"Not Applicable") || 
		!RDAstrcmp(debcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(reqacc,"DEBIT CODE",NULL);
	} else {
		FINDFLDSETSTRING(reqacc,"DEBIT CODE",debcode1);
	}
	if(!RDAstrcmp(credcode1,"Not Applicable") || 
		!RDAstrcmp(credcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(reqacc,"CREDIT CODE",NULL);
	} else {
		FINDFLDSETSTRING(reqacc,"CREDIT CODE",credcode1);
	}
	if(!RDAstrcmp(defdebcode1,"Not Applicable") || 
		!RDAstrcmp(defdebcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(reqacc,"DEFAULT DEBIT CODE",NULL);
	} else {
		FINDFLDSETSTRING(reqacc,"DEFAULT DEBIT CODE",defdebcode1);
	}
	if(!RDAstrcmp(defcredcode1,"Not Applicable") || 
		!RDAstrcmp(defcredcode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(reqacc,"DEFAULT CREDIT CODE",NULL);
	} else {
		FINDFLDSETSTRING(reqacc,"DEFAULT CREDIT CODE",defcredcode1);
	}
	if(!RDAstrcmp(defpaycode1,"Not Applicable") || 
		!RDAstrcmp(defpaycode1,"No Accounts Available"))
	{
		FINDFLDSETSTRING(reqacc,"DEFAULT PAYABLE CODE",NULL);
	} else {
		FINDFLDSETSTRING(reqacc,"DEFAULT PAYABLE CODE",defpaycode1);
	}
	FINDFLDSETSTRING(reqacc,"DEFAULT BANK IDENTIFICATION",bankid);
	if(bankid!=NULL) Rfree(bankid);
	FINDFLDSETSTRING(reqitm,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(reqitm,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(reqitm,"SOURCE USER VERIFIED",deleteflag);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	if((x=FINDRSC(parent,"[REQITM][ENTRY DATE]"))!=(-1))
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
			FINDFLDSETSTRING(reqitm,"ENTRY DATE",date);
		}
	} else {
		FINDFLDSETSTRING(reqitm,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date);
	if((x=FINDRSC(parent,"[REQITM][ENTRY TIME]"))!=(-1))
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
			FINDFLDSETSTRING(reqitm,"ENTRY TIME",timex);
		}
	} else {
		FINDFLDSETSTRING(reqitm,"ENTRY TIME",timex);
	}
	if(timex!=NULL) Rfree(timex);
	COPYFIELD(reqitm,reqacc,"LOCATION IDENTIFICATION");
	COPYFIELD(reqitm,reqacc,"REQUISITION IDENTIFICATION");
	COPYFIELD(reqitm,reqacc,"ITEM IDENTIFICATION");
	FIELDCOPY(reqitm,"VENDOR IDENTIFICATION",reqitm,"REQUESTED VENDOR IDENTIFICATION");
	FIELDCOPY(reqitm,"ITEM IDENTIFICATION",reqitm,"REQUESTED ITEM IDENTIFICATION");
	FIELDCOPY(reqitm,"VENDOR ITEM NUMBER",reqitm,"REQUESTED VENDOR ITEM NUMBER");
	if(ADVRECORDsec(reqitm,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(reqitm,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  ADVRECORDsec() failed without a description returned!");
		}
	} else if(ADVRECORDsec(reqacc,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(reqacc,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  ADVRECORDsec() failed without a description returned!");
		}
	} else {
		if(!WRTTRANS(reqitm,TRUE,"ITEM NUMBER",NULL))
		{
			FINDFLDGETINT(reqitm,"ITEM NUMBER",&itemnumber);
			FINDFLDSETINT(reqacc,"ITEM NUMBER",itemnumber);
			WRTTRANS(reqacc,TRUE,"ACCOUNT NUMBER",NULL);
			total_req+=amount;
			FINDRSCSETDOUBLE(parent,"TOTAL REQUISITION",total_req);
			updatersrc(parent,"TOTAL REQUISITION");
			if(credcode1!=NULL && RDAstrlen(credcode1)>0)
			{
				ADVupdateundist(reqacc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",
					NULL,eyrnum,byrnum,procyrid,deftype1,"ENCUMBRANCES",
					pmonth->libs[selmonth],amount,
					SCRNvirtualSubData,parent);
			} else {
				ADVupdateundist(reqacc,"ACCOUNT CODE","DEBIT CODE",NULL,NULL,
					eyrnum,byrnum,procyrid,deftype1,"ENCUMBRANCES",
					pmonth->libs[selmonth],amount,
					SCRNvirtualSubData,parent);
			}
			++REQ_COUNT;
			makedflist();
		} else {
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=Rmalloc(RDAstrlen(MODULENAME(reqitm))+RDAstrlen(FILENAME(reqitm))+139);
			sprintf(errordesc,"The write function failed to the file [%s][%s], the record and all subfiles that you attempted to save were not created.  Please try again.",MODULENAME(reqitm),FILENAME(reqitm));
			ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
			prterr("Error Couldn't Write Record: [%s]",errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		}
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS ITEM'S",(prevvch->numlibs-1),
		prevvch))
	{
		updatersrc(parent,"PREVIOUS ITEM'S");
	}
}
static void quit_check(RDArsrc *parent)
{
	quit_record(parent);
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
	if(pmonth!=NULL) freeapplib(pmonth);
	if(addresslist!=NULL) freeapplib(addresslist);
	if(prevvch!=NULL) freeapplib(prevvch);
	if(categories!=NULL) freeapplib(categories);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(defdebcode1!=NULL) Rfree(defdebcode1);
	if(defcredcode1!=NULL) Rfree(defcredcode1);
	if(defpaycode1!=NULL) Rfree(defpaycode1);
}
static void setyear(RDArsrc *parent)
{
	int procyearid=0,yearno=0;

	ZERNRD(finyrnum);
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&yearno)) return;
	procyearid=atoi(yearavl->libs[yearno]);
	procyearid=atoi(yearavl->libs[yearno]);
	FINDFLDSETINT(finyrnum,"FISCAL YEAR",procyearid);
	FINDFLDSETINT(reqitm,"FISCAL YEAR",procyearid);
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
	int selectedyear=0,procyrid=0;

	FINDRSCGETINT(parent,"FISCAL YEARS",&selectedyear);
	if(selectedyear>=yearavl->numlibs)
	{
		selectedyear=0;
		FINDRSCSETINT(parent,"FISCAL YEARS",selectedyear);
	}
	procyrid=atoi(yearavl->libs[selectedyear]);
	if((procyrid!=year_identification) || force)
	{
		year_identification=(-1);
		definition_type=(-1);
		setyear(parent);
		changeDEFtype(parent,force);
		updateSCRNvirtuals(parent);
	}
}
void changeCODElist(RDArsrc *parent)
{
	int selectedd=0,selyear=0,procyrid=0,yearid=0;
	short ef=0,deftype=0,deftype1=0,tempyrfile=0,tempaccfile=0;
	short keyn=0,keynumber=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL,*e=NULL,*actyp=NULL,*speedkeyname=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	procyrid=atoi(yearavl->libs[selyear]);
	deftype=(short)selectedd;
	FINDRSCSETDEFTYPE(parent,"ACCOUNT CODE",deftype);
	FINDRSCSETDEFTYPE(parent,"DEBIT CODE",deftype);
	FINDRSCSETDEFTYPE(parent,"CREDIT CODE",deftype);
	FINDRSCSETDEFTYPE(parent,"DEFAULT DEBIT CODE",deftype);
	FINDRSCSETDEFTYPE(parent,"DEFAULT CREDIT CODE",deftype);
	FINDRSCSETDEFTYPE(parent,"DEFAULT PAYABLE CODE",deftype);
	if(account_code!=NULL) Rfree(account_code);
	if(baltype!=NULL) Rfree(baltype); 
	if(actyp!=NULL) Rfree(actyp);
	if(speedkeyname!=NULL) Rfree(speedkeyname);
}
void changeDEFtype(RDArsrc *parent,short force)
{
	int sela=0,seld=0,selc=0;
	int seldefd=0,seldefc=0,seldefp=0;
	int selecteddef=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&selecteddef);
	if((selecteddef!=definition_type) || force)
	{
		year_identification=(-1);
		FINDRSCGETINT(parent,"ACCOUNT CODES",&sela);
		FINDRSCGETINT(parent,"DEBIT CODES",&seld);
		FINDRSCGETINT(parent,"CREDIT CODES",&selc);
		FINDRSCGETINT(parent,"DEFAULT DEBIT CODES",&seldefd);
		FINDRSCGETINT(parent,"DEFAULT CREDIT CODES",&seldefc);
		FINDRSCGETINT(parent,"DEFAULT PAYABLE CODES",&seldefp);
		changeCODElist(parent);
		debitcodenum=1;
		getsearchstr(parent,&debitcodenum);
		creditcodenum=2;
		getsearchstr(parent,&creditcodenum); 
		defdebitcodenum=3;
		getsearchstr(parent,&defdebitcodenum); 
		defcreditcodenum=4;
		getsearchstr(parent,&defcreditcodenum); 
		defpayablecodenum=5;
		getsearchstr(parent,&defpayablecodenum); 
		getacctdesc(parent);
	}
}
void DEFtypecb(RDArsrc *parent)
{
	FINDRSCSETSTRING(parent,"ACCOUNT CODE",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(parent,"CREDIT CODE DESCRIPTION",NULL);
	changeDEFtype(parent,FALSE);
}
static int getyears(RDArsrc *parent,RDAfinmgt *finsetup)
{
	short ef;
	int selyear=0,curryear=0,year=0;
	char delflag=FALSE;

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
			if(year==curryear)
			{
				selyear=((yearavl->numlibs)-1);
			}
		}
		}
		ef=ADVNXTNRDsec(finyrnum,1,SCRNvirtualSubData,parent);
	}
	return(selyear);
}
void getsupportingfiles(RDArsrc *parent)
{
	if(reqmst!=(-1))
	{
		ZERNRD(reqmst);
		FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid);
		FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid);
		if(ADVEQLNRDsec(reqmst,1,SCRNvirtualSubData,parent)) KEYNRD(reqmst,1);
		filerecord2rsrc(reqmst,parent);
	}
	if(reqloc!=(-1))
	{
		FINDFLDSETSTRING(reqloc,"LOCATION IDENTIFICATION",locid);
		if(ADVEQLNRDsec(reqloc,1,SCRNvirtualSubData,parent)) KEYNRD(reqloc,1);
		filerecord2rsrc(reqloc,parent);
	}
}
void changecategory(RDArsrc *mtnrsrc)
{
	int selcat=0;

	FINDRSCGETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	FINDFLDSETSTRING(reqcat,"CATEGORY IDENTIFICATION",categories->libs[selcat]);
	if(ADVEQLNRDsec(reqcat,1,SCRNvirtualSubData,mtnrsrc))
	{
		ZERNRD(reqcat);
	}
	updatefilerecord2rsrc(reqcat,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
void getBANKids(RDArsrc *parent)
{
	if(banknum!=(-1))
	{
		if(BANKids==NULL)
		{
			BANKids=APPlibNEW();
			ZERNRD(banknum);
			BANKids=ADVMakeNRDlistAllSTRING(banknum,"BNKREC",
				"Creating Bank Identification List",1,
				"BANK IDENTIFICATION",SCRNvirtualSubData,parent);
			if(BANKids->numlibs<1)
				addAPPlib(BANKids,"No Banks Defined.");
			FINDRSCSETSTRING(parent,"DEFAULT BANK IDENTIFICATION",
				BANKids->libs[0]);
			updatersrc(parent,"DEFAULT BANK IDENTIFICATION");
			FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",BANKids->libs[0]);
			if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,parent)) KEYNRD(banknum,1);
			filerecord2rsrc(banknum,parent);
		}
	}
}
void setBANKidscb(RDArsrc *parent)
{
	char *bankid=NULL,*tmp=NULL;
	int x=0;

	if(banknum!=(-1))
	{
		readwidget(parent,"DEFAULT BANK IDENTIFICATION");
		FINDRSCGETSTRING(parent,"DEFAULT BANK IDENTIFICATION",&bankid);
		for(x=0;x<BANKids->numlibs;++x)
		{
			if(!RDAstrcmp(BANKids->libs[x],bankid)) break;
		}
		if(x>=BANKids->numlibs)
		{
			tmp=Rmalloc(RDAstrlen(bankid)+211);
			sprintf(tmp,"The DEFAULT BANK IDENTIFICATION of [%s] that was entered does not existing the [BNKREC][BNKACC] Bank Account File.  If it is a valid bank make sure that it is in the Bank Account File and that it is not deleted.",bankid);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT BANK IDENTIFICATION!",tmp,NULL,NULL,TRUE,NULL);
			FINDRSCGETINT(parent,"DEFAULT BANK IDENTIFICATIONS",&x);
			FINDRSCSETSTRING(parent,"DEFAULT BANK IDENTIFICATION",BANKids->libs[x]);
			updatersrc(parent,"DEFAULT BANK IDENTIFICATION");
			if(tmp!=NULL) Rfree(tmp);
		} else {
			FINDRSCSETINT(parent,"DEFAULT BANK IDENTIFICATIONS",x);
			updatersrc(parent,"DEFAULT BANK IDENTIFICATIONS");
		}
		FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",BANKids->libs[x]);
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,parent)) KEYNRD(banknum,1);
		updatefilerecord2rsrc(banknum,parent);
		updateSCRNvirtuals(parent);
		if(bankid!=NULL) Rfree(bankid);
	}
}
void BANKidscb(RDArsrc *parent)
{
	char *bankid=NULL;
	int x=0;

	if(banknum!=(-1))
	{
		FINDRSCGETINT(parent,"DEFAULT BANK IDENTIFICATIONS",&x);
		if(!RDAstrcmp(BANKids->libs[x],"No Banks Defined.")) return;
		FINDRSCSETSTRING(parent,"DEFAULT BANK IDENTIFICATION",BANKids->libs[x]);
		updatersrc(parent,"DEFAULT BANK IDENTIFICATION");
		FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",BANKids->libs[x]);
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,parent)) KEYNRD(banknum,1);
		updatefilerecord2rsrc(banknum,parent);
		updateSCRNvirtuals(parent);
		if(bankid!=NULL) Rfree(bankid);
	}
}
void additemssingle(void)
{
	short nofields=0,nokeys=0,x,edit_rsrc=TRUE;
	int y=0,start=0,presentyear=0,which_type=0;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL;
	char *name=NULL,*date=NULL,*timex=NULL,catalogvendor=TRUE;
	RDAacct *acc=NULL;

	nokeys=NUMKEYS(reqitm);
	keys=KEYPOINTER(reqitm);
	nofields=NUMFLDS(reqitm);
	fields=FLDPOINTER(reqitm);
	parent=RDArsrcNEW(module,"ADD REQUISITION ITEMS SINGLE W/O LISTS");
	definelist=RDArsrcNEW(module,"ADD REQUISITION ITEMS DEFINE LIST");
	addDFincvir(parent,"REQMGT","REQLOC",NULL,reqloc);
	addDFincvir(parent,"REQMGT","REQITM",NULL,reqitm);
	addDFincvir(parent,"REQMGT","REQMST",NULL,reqmst);
	addDFincvir(parent,"REQMGT","REQACC",NULL,reqacc);
	addDFincvir(parent,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(parent,"CATALOGUE","CTLGVP",NULL,reqpven); 
	addDFincvir(parent,"FINMGT","FINBACC",NULL,balnum);
	addDFincvir(parent,"FINMGT","FINEACC",NULL,expnum);
	addDFincvir(parent,"FINMGT","FINBYR",NULL,byrnum);
	addDFincvir(parent,"FINMGT","FINEYR",NULL,eyrnum);
	addDFincvir(parent,"FINMGT","FINYEAR",NULL,finyrnum);
	addDFincvir(parent,"BNKREC","BNKACC",NULL,banknum);
	addDFincvir(parent,"VENPMT","VENADR",NULL,venadrnum);
	addDFincvir(parent,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(definelist,"REQMGT","REQITM",NULL,reqitm);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	if(reqmst!=(-1)) file2rsrc(reqmst,parent,FALSE);
	if(reqloc!=(-1)) file2rsrc(reqloc,parent,FALSE);
	if(reqcat!=(-1)) file2rsrc(reqcat,parent,FALSE);
	if(vennum!=(-1)) file2rsrc(vennum,parent,FALSE);
	if(venadrnum!=(-1)) file2rsrc(venadrnum,parent,FALSE);
	getsupportingfiles(parent);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"LOCATION IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(reqitm,fldx->name,&edit_rsrc))
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					}
					sprintf(name,"[%s][%s]",FILENAME(reqitm),fldx->name);
					makefieldrsrc(parent,name,fldx->type,
						fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"REQUISITION IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(reqitm,fldx->name,&edit_rsrc))
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					}
					sprintf(name,"[%s][%s]",FILENAME(reqitm),fldx->name);
					makefieldrsrc(parent,name,fldx->type,
						fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(fldx->name,"CATEGORY IDENTIFICATION"))
			{
				which_type=0;
				if(categories!=NULL) freeapplib(categories);
				ZERNRD(reqcat);
				categories=ADVMakeNRDlistAllSTRING(reqcat,module,
					"Creating Category List",1,"CATEGORY IDENTIFICATION",
					SCRNvirtualSubData,parent);
				if(categories->numlibs<1)
				{
					addAPPlib(categories,"No Categories");
				}
				addlstrsrc(parent,"CATEGORY IDENTIFICATIONS",
					&which_type,(edit_rsrc==TRUE?TRUE:FALSE),
					changecategory,categories->numlibs,
					&categories->libs,NULL);
			} else if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
			{
				presentyear=getyears(parent,finsetup);
				if(yearavl->numlibs<1)
				{
					ERRORDIALOG("FINYEAR Empty File","The FINYEAR file is an empty file.  There must exist at least 1 year on file to process requisition items.\n",NULL,TRUE);
					ShutdownSecurity();
					RDAAPPMAINLOOP();
					quit_record(parent);
					return;
				}
				edit_rsrc=TRUE;
				if(FIELDscrn(reqloc,fldx->name,&edit_rsrc))
				{
					file2rsrc(finyrnum,parent,FALSE);
					addlstrsrc(parent,"FISCAL YEARS",
						&presentyear,edit_rsrc,changeyearcb,
						yearavl->numlibs,
						&yearavl->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"PROCESSING MONTH"))
			{
				pmonth=APPlibNEW();
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
				edit_rsrc=TRUE;
				if(FIELDscrn(reqloc,fldx->name,&edit_rsrc))
				{
					start=(val(CURRENT_DATE,2)-1);
					addlstrsrc(parent,"PROCESSING MONTHS",
						&start,edit_rsrc,NULL,
						pmonth->numlibs,
						&pmonth->libs,NULL);
				}
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,reqitm,fldx,edit_rsrc);
			}
		}
	}
	edit_rsrc=TRUE;
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	DEFtypes=APPlibNEW();
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
				y<EXPENDITURE_ACCOUNT->num;++y,++acc)
			{
				addAPPlib(DEFtypes,acc->name);
			}
		}
	} 
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	start=0;
	addlstrsrc(parent,"DEFINITION TYPES",&start,edit_rsrc,DEFtypecb,
		DEFtypes->numlibs,&DEFtypes->libs,NULL);
	addstdrsrc(parent,"ACCOUNT DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(parent,"ACCOUNT CODE",EXPENDITURE,40,NULL,(edit_rsrc ? TRUE:FALSE));
	FINDRSCSETFUNC(parent,"ACCOUNT CODE",acctsearchcb,&acctcodenum);
	FINDRSCSETFUNC(parent,"[REQITM][QUANTITY]",caltotalqty,NULL);
	FINDRSCSETFUNC(parent,"[REQITM][UNIT RATE]",caltotalur,NULL);
	FINDRSCSETFUNC(parent,"[REQITM][PERCENTAGE DISCOUNT]",caltotalur,NULL);
	addstdrsrc(parent,"DEBIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(parent,"DEBIT CODE",BALANCESHEET,40,NULL,edit_rsrc);
	debitcodenum=1;
	FINDRSCSETFUNC(parent,"DEBIT CODE",acctsearchcb,&debitcodenum);
	addstdrsrc(parent,"CREDIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(parent,"CREDIT CODE",BALANCESHEET,40,NULL,edit_rsrc);
	creditcodenum=2;
	FINDRSCSETFUNC(parent,"CREDIT CODE",acctsearchcb,&creditcodenum);
	addstdrsrc(parent,"DEFAULT DEBIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(parent,"DEFAULT DEBIT CODE",BALANCESHEET,40,NULL,edit_rsrc);
	defdebitcodenum=3;
	FINDRSCSETFUNC(parent,"DEFAULT DEBIT CODE",acctsearchcb,&defdebitcodenum);
	addstdrsrc(parent,"DEFAULT CREDIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(parent,"DEFAULT CREDIT CODE",BALANCESHEET,40,NULL,edit_rsrc);
	defcreditcodenum=3;
	FINDRSCSETFUNC(parent,"DEFAULT CREDIT CODE",acctsearchcb,&defcreditcodenum);
	addstdrsrc(parent,"DEFAULT PAYABLE CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(parent,"DEFAULT PAYABLE CODE",BALANCESHEET,40,NULL,edit_rsrc);
	defpayablecodenum=3;
	FINDRSCSETFUNC(parent,"DEFAULT PAYABLE CODE",acctsearchcb,&defpayablecodenum);
	addstdrsrc(parent,"DEFAULT BANK IDENTIFICATION",VARIABLETEXT,0,NULL,edit_rsrc);
	if(banknum!=(-1))
	{
		file2rsrc(banknum,parent,FALSE);
		getBANKids(parent);
		start=0;
		addlstrsrc(parent,
			"DEFAULT BANK IDENTIFICATIONS",
			&start,(edit_rsrc==TRUE?TRUE:FALSE),
			BANKidscb,BANKids->numlibs,
			&BANKids->libs,NULL);
		FINDRSCSETFUNC(parent,"DEFAULT BANK IDENTIFICATION",setBANKidscb,NULL);
	}
	FINDRSCSETFUNC(parent,"[REQITM][VENDOR IDENTIFICATION]",checkvendorcb,
		NULL);
	nofields=0;
	nokeys=0;
	if(name!=NULL) Rfree(name);
	if(prevvch!=NULL) freeapplib(prevvch);
	prevvch=APPlibNEW();
	addAPPlib(prevvch,"No Requisition Items added this session");
	addlstrsrc(parent,"PREVIOUS ITEM'S",&x,TRUE,NULL,prevvch->numlibs,
		&prevvch->libs,NULL);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(parent,"[REQITM][ENTRY DATE]",date);
	FINDRSCSETSTRING(parent,"[REQITM][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	total_req=0.0;
	savelist(definelist,parent,TRUE);
	if(addresslist!=NULL) freeapplib(addresslist);
	addresslist=APPlibNEW();
	addlstrsrc(parent,"ADDRESS IDENTIFICATIONS",&x,TRUE,changeADDRlist,
		addresslist->numlibs,&addresslist->libs,NULL);
/* FINDRSCSETFUNC(parent,"[REQITM][ADDRESS IDENTIFICATION]",getaddrcb,NULL); */
	setaddresses(parent);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	addstdrsrc(parent,"TOTAL REQUISITION",DOLLARS,20,&total_req,TRUE);
	addstdrsrc(parent,"CATALOGUE VENDOR",BOOLNS,1,&catalogvendor,TRUE);
	addbtnrsrc(parent,"BROWSE VENDORS",TRUE,browse_vendors,NULL);
	addbtnrsrc(parent,"BROWSE ITEMS",TRUE,browse_items,NULL);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,definelist);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addbtnrsrc(parent,"QUIT",TRUE,quit_check,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(parent);
	APPmakescrn(parent,TRUE,quit_record,NULL,FALSE);
}
void browse_catalog(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	char *item=NULL,price=TRUE;
	int selcat=0;

	mainrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE VENDOR BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE VENDOR SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE VENDOR DEFINE LIST");
	ZERNRD(reqven);
	addDFincvir(mainrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(mainrsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVP",NULL,reqpven); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(searchrsrc,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(searchrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(searchrsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(definelist,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(definelist,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(definelist,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum);
	FINDRSCGETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	readwidget(mtnrsrc,"[REQITM][ITEM IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[REQITM][ITEM IDENTIFICATION]",&item);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	if(reqcat!=(-1))
	{
		FINDFLDSETSTRING(reqcat,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
		if(ADVEQLNRDsec(reqcat,1,SCRNvirtualSubData,mainrsrc)) 
		{
			ZERNRD(reqcat);
		} else {
			file2rsrc(reqcat,mainrsrc,FALSE);
			filerecord2rsrc(reqcat,mainrsrc);
		}
	}
	if(reqctlg!=(-1))
	{
		FINDFLDSETSTRING(reqctlg,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
		FINDFLDSETSTRING(reqctlg,"ITEM IDENTIFICATION",item);
		if(ADVEQLNRDsec(reqctlg,1,SCRNvirtualSubData,mainrsrc)) 
		{
			ZERNRD(reqctlg);
		} else {
			file2rsrc(reqctlg,mainrsrc,FALSE);
			filerecord2rsrc(reqctlg,mainrsrc);
		}
	}
	if(item!=NULL) Rfree(item);
	addstdrsrc(mainrsrc,"LOAD PRICE",BOOLNS,1,&price,TRUE);
	ADVbrowse(reqven,1,mainrsrc,searchrsrc,definelist,
		initreqven,selectreqven,quititem,okreqvenfunc,NULL,
		mtnrsrc,TRUE,NULL,2,2,2,FALSE);
}
void initreqven(MakeBrowseList *blist)
{
	int selcat=0;
	char *item=NULL;

	readallwidgets(blist->funcargs);
	FINDRSCGETINT(blist->funcargs,"CATEGORY IDENTIFICATIONS",&selcat);
	readwidget(blist->funcargs,"[REQITM][ITEM IDENTIFICATION]");
	FINDRSCGETSTRING(blist->funcargs,"[REQITM][ITEM IDENTIFICATION]",&item);
	FINDFLDSETSTRING(reqven,"CATEGORY IDENTIFICATION",
		categories->libs[selcat]);
	if(!isEMPTY(item))
	{
		FINDFLDSETSTRING(reqven,"ITEM IDENTIFICATION",item);
	}
	if(item!=NULL) Rfree(item);
}
short selectreqven(MakeBrowseList *blist)
{
	char deleteflag=FALSE,*catid=NULL,*item1=NULL,*item2=NULL;
	int selcat=0;

	FINDFLDGETCHAR(reqven,"DELETEFLAG",&deleteflag);
	if(deleteflag) return(FALSE);
	FINDRSCGETINT(blist->funcargs,"CATEGORY IDENTIFICATIONS",&selcat);
	FINDFLDGETSTRING(reqven,"CATEGORY IDENTIFICATION",&catid);
	if(RDAstrcmp(catid,categories->libs[selcat]))
	{
		if(catid!=NULL) Rfree(catid);
		return(FALSE);
	}
	if(catid!=NULL) Rfree(catid);
	FINDRSCGETSTRING(blist->funcargs,"[REQITM][ITEM IDENTIFICATION]",&item1);
	if(!isEMPTY(item1))
	{
		FINDFLDGETSTRING(reqven,"ITEM IDENTIFICATION",&item2);
		if(RDAstrcmp(item1,item2))
		{
			if(item1!=NULL) Rfree(item1);
			if(item2!=NULL) Rfree(item2);
			return(FALSE);
		}
	}
	if(item1!=NULL) Rfree(item1);
	if(item2!=NULL) Rfree(item2);
	return(TRUE);
}
void okreqvenfunc(MakeBrowseList *b,void *targetkey)
{
	char *vendorid=NULL,*venitem=NULL,bestprice=FALSE;
	char *venid1=NULL,*item1=NULL,*item2=NULL,*cat1=NULL,*cat2=NULL;
	char *venadrid=NULL,*desc=NULL;
	int selcat=0,quantity=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;

	ZERNRD(reqven);
	if(ADVEQLNRDKEYsec(reqven,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(reqven,1);
	} else {
		FINDFLDGETSTRING(reqven,"VENDOR IDENTIFICATION",&vendorid);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR IDENTIFICATION]",
			vendorid);
		FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDGETSTRING(reqven,"ADDRESS IDENTIFICATION",&venadrid);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][ADDRESS IDENTIFICATION]",
			venadrid);
		FINDFLDSETSTRING(reqitm,"ADDRESS IDENTIFICATION",venadrid);
		FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",venadrid);
		FINDFLDGETSTRING(reqven,"VENDOR ITEM NUMBER",&venitem);
		FINDFLDSETSTRING(reqitm,"VENDOR ITEM NUMBER",venitem);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR ITEM NUMBER]",
			venitem);
		checkvendor(b->funcargs,TRUE);
		filerecord2rsrc(vennum,b->funcargs);
		FINDFLDGETSTRING(reqven,"ITEM IDENTIFICATION",&item1);
		FINDFLDSETSTRING(reqitm,"ITEM IDENTIFICATION",item1);
		FINDFLDSETSTRING(reqctlg,"ITEM IDENTIFICATION",item1);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][ITEM IDENTIFICATION]",item1);
		FINDRSCGETINT(b->funcargs,"CATEGORY IDENTIFICATIONS",&selcat);
		if(RDAstrcmp(categories->libs[selcat],"No Categories"))
		{
			cat1=stralloc(categories->libs[selcat]);
		}
		FINDFLDSETSTRING(reqctlg,"CATEGORY IDENTIFICATION",cat1);
		if(ADVEQLNRDsec(reqctlg,1,SCRNvirtualSubData,b->mainrsrc)) 
		{
			KEYNRD(reqctlg,1);
		} else {
			filerecord2rsrc(reqctlg,b->funcargs);
			FINDFLDGETSTRING(reqctlg,"DESCRIPTION",&desc);
			FINDRSCSETSTRING(b->funcargs,"[REQITM][DESCRIPTION]",desc);
			if(desc!=NULL) Rfree(desc);
		}
		if(cat1!=NULL) Rfree(cat1);
		readwidget(b->mainrsrc,"LOAD PRICE");
		FINDRSCGETCHAR(b->mainrsrc,"LOAD PRICE",&bestprice);
		if(bestprice)
		{
			ZERNRD(reqpven);
			FINDFLDGETSTRING(reqven,"CATEGORY IDENTIFICATION",&cat1);
			FINDFLDGETSTRING(reqven,"ITEM IDENTIFICATION",&item1);
			FINDFLDSETSTRING(reqpven,"CATEGORY IDENTIFICATION",cat1);
			FINDFLDSETSTRING(reqpven,"ITEM IDENTIFICATION",item1);
			FINDFLDSETSTRING(reqpven,"VENDOR IDENTIFICATION",vendorid);
			readwidget(b->funcargs,"[REQITM][QUANTITY]");
			FINDRSCGETINT(b->funcargs,"[REQITM][QUANTITY]",&quantity);
			readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
			FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
			FINDFLDSETINT(reqpven,"QUANTITY",quantity);
			if(!ADVLTENRDsec(reqpven,1,SCRNvirtualSubData,b->mainrsrc))
			{
				FINDFLDGETSTRING(reqpven,"VENDOR IDENTIFICATION",&venid1);		
				FINDFLDGETSTRING(reqpven,"ITEM IDENTIFICATION",&item2);
				FINDFLDGETSTRING(reqpven,"CATEGORY IDENTIFICATION",&cat2);
				if(RDAstrcmp(vendorid,venid1) || RDAstrcmp(item1,item2) || RDAstrcmp(cat1,cat2))
				{
					readwidget(b->funcargs,"[REQITM][UNIT RATE]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
					readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
				} else {
					FINDFLDGETDOUBLE(reqpven,"UNIT RATE",&unitrate);
					FINDFLDGETDOUBLE(reqpven,"PERCENTAGE DISCOUNT",&percdisc);
				}
			} else {
				readwidget(b->funcargs,"[REQITM][UNIT RATE]");
				FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
				readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
				FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
			}
			if(percdisc!=0.0)
			{
				total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
			} else {
				total=round(((unitrate*quantity)*100.0));
			}
			FINDRSCSETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",unitrate);
			FINDRSCSETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",percdisc);
			FINDRSCSETDOUBLE(b->funcargs,"[REQITM][AMOUNT]",total);
		}
		if(venid1!=NULL) Rfree(venid1);
		if(cat1!=NULL) Rfree(cat1);
		if(cat2!=NULL) Rfree(cat2);
		if(item1!=NULL) Rfree(item1);
		if(item2!=NULL) Rfree(item2);
		if(vendorid!=NULL) Rfree(vendorid);
		if(venitem!=NULL) Rfree(venitem);
		computeallSCRNvirtuals(b->funcargs);
		updateallrsrc(b->funcargs);
	}
	setaddresses(b->funcargs);
}
void browse_vendors(RDArsrc *mtnrsrc)
{
	char catalogvendor=TRUE;

	readwidget(mtnrsrc,"CATALOGUE VENDOR");
	FINDRSCGETCHAR(mtnrsrc,"CATALOGUE VENDOR",&catalogvendor);
	if(catalogvendor)
	{
		if(diagapps)
		{
			prterr("Selecting Browse on CATALOGUE VENDOR=TRUE Vendor Identifications.");
		}
		browse_catalog(mtnrsrc);
	} else {
		if(diagapps)
		{
			prterr("Selecting Browse on CATALOGUE VENDOR=FALSE Vendor Identifications.");
		}
		browse_venmsr(mtnrsrc);
	}
}
void browse_items(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	char primven=TRUE;
	int selcat=0;

	FINDRSCGETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	mainrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE DEFINE LIST");
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ZERNRD(reqctlg);
	addDFincvir(mainrsrc,"CATALOGUE","CATMST",NULL,reqcat); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVP",NULL,reqpven); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg); 
	addDFincvir(searchrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg); 
	addDFincvir(searchrsrc,"CATALOGUE","CATMST",NULL,reqcat); 
	addDFincvir(definelist,"CATALOGUE","CTLGMST",NULL,reqctlg); 
	addDFincvir(definelist,"CATALOGUE","CATMST",NULL,reqcat); 
	if(reqcat!=(-1))
	{
		FINDFLDSETSTRING(reqcat,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
		if(ADVEQLNRDsec(reqcat,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(reqcat);
		} else {
			file2rsrc(reqcat,mainrsrc,FALSE);
			filerecord2rsrc(reqcat,mainrsrc);
		}
	}
	addstdrsrc(mainrsrc,"PRIMARY VENDOR",BOOLNS,1,&primven,TRUE);
	addstdrsrc(mainrsrc,"LOAD PRICE",BOOLNS,1,&primven,TRUE);
	ADVbrowse(reqctlg,1,mainrsrc,searchrsrc,definelist,inititem,selectitem,
		quititem,okitemfunc,NULL,mtnrsrc,TRUE,NULL,2,2,2,FALSE);
}
void inititem(MakeBrowseList *blist)
{
	int selcat=0;

	FINDRSCGETINT(blist->funcargs,"CATEGORY IDENTIFICATIONS",&selcat);
	FINDFLDSETSTRING(reqctlg,"CATEGORY IDENTIFICATION",categories->libs[selcat]);
}
short selectitem(MakeBrowseList *blist)
{
	char deleteflag=FALSE,*catid=NULL;
	int selcat=0;

	FINDFLDGETCHAR(reqctlg,"DELETEFLAG",&deleteflag);
	if(deleteflag) return(FALSE);
	FINDRSCGETINT(blist->funcargs,"CATEGORY IDENTIFICATIONS",&selcat);
	FINDFLDGETSTRING(reqctlg,"CATEGORY IDENTIFICATION",&catid);
	if(RDAstrcmp(catid,categories->libs[selcat]))
	{
		if(catid!=NULL) Rfree(catid);
		return(FALSE);
	}
	if(catid!=NULL) Rfree(catid);
	return(TRUE);
}
void quititem(MakeBrowseList *blist)
{
	if(blist->definelist!=NULL) 
	{
		killwindow(blist->definelist);
		free_rsrc(blist->definelist);
		blist->definelist=NULL;
	}
	if(blist->searchrsrc!=NULL) 
	{
		killwindow(blist->searchrsrc);
		free_rsrc(blist->searchrsrc);
		blist->searchrsrc=NULL;
	}
	if(blist->mainrsrc!=NULL)
	{
		free_rsrc(blist->mainrsrc);
		blist->mainrsrc=NULL;
	}
}
void okitemfunc(MakeBrowseList *b,void *targetkey)
{
	char *itemid=NULL,primven=FALSE,*vendorid=NULL,*desc=NULL;
	char *venitem=NULL,bestprice=FALSE,*itemname=NULL;
	char *cat1=NULL,*cat2=NULL,*itemid1=NULL,*venid1=NULL;
	int quantity=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;

	ZERNRD(reqctlg);
	if(ADVEQLNRDKEYsec(reqctlg,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(reqctlg,1);
	} else {
		FINDFLDGETSTRING(reqctlg,"ITEM IDENTIFICATION",&itemid);
		FINDFLDGETSTRING(reqctlg,"DESCRIPTION",&desc);
		FINDFLDGETSTRING(reqctlg,"ITEM NAME",&itemname);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][DESCRIPTION]",desc);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][ITEM NAME]",itemname);
		FINDFLDSETSTRING(reqitm,"[REQITM][ITEM NAME]",itemname);
		if(itemname!=NULL) Rfree(itemname);
		if(desc!=NULL) Rfree(desc);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][ITEM IDENTIFICATION]",itemid);
		readwidget(b->mainrsrc,"PRIMARY VENDOR");
		FINDRSCGETCHAR(b->mainrsrc,"PRIMARY VENDOR",&primven);
		if(primven)
		{
			readwidget(b->mainrsrc,"LOAD PRICE");
			FINDRSCGETCHAR(b->mainrsrc,"LOAD PRICE",&bestprice);
			FINDFLDGETSTRING(reqctlg,"PRIMARY VENDOR IDENTIFICATION",&vendorid);
			FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR IDENTIFICATION]",vendorid);
			FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",vendorid);
			FINDFLDGETSTRING(reqctlg,"CATEGORY IDENTIFICATION",&cat1);
			FINDFLDSETSTRING(reqven,"CATEGORY IDENTIFICATION",cat1);
			COPYFIELD(reqctlg,reqven,"ITEM IDENTIFICATION");
			FINDFLDSETSTRING(reqven,"VENDOR IDENTIFICATION",vendorid);
			if(!ADVEQLNRDsec(reqven,1,SCRNvirtualSubData,b->mainrsrc))
			{
				FINDFLDGETSTRING(reqven,"VENDOR ITEM NUMBER",&venitem);
			}
			FINDFLDSETSTRING(reqven,"VENDOR ITEM NUMBER",venitem);
			FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR ITEM NUMBER]",
				venitem);
			checkvendor(b->funcargs,TRUE);
			filerecord2rsrc(vennum,b->funcargs);
			if(bestprice)
			{
				ZERNRD(reqpven);
				COPYFIELD(reqctlg,reqpven,"CATEGORY IDENTIFICATION");
				FINDFLDSETSTRING(reqpven,"ITEM IDENTIFICATION",itemid);
				FINDFLDSETSTRING(reqpven,"VENDOR IDENTIFICATION",vendorid);
				readwidget(b->funcargs,"[REQITM][QUANTITY]");
				FINDRSCGETINT(b->funcargs,"[REQITM][QUANTITY]",&quantity);
				readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
				FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
				FINDFLDSETINT(reqpven,"QUANTITY",quantity);
				if(!ADVLTENRDsec(reqpven,1,SCRNvirtualSubData,b->mainrsrc))
				{
					FINDFLDGETSTRING(reqpven,"CATEGORY IDENTIFICATION",&cat2);
					FINDFLDGETSTRING(reqpven,"ITEM IDENTIFICATION",&itemid1);
					FINDFLDGETSTRING(reqpven,"VENDOR IDENTIFICATION",&venid1);
					if(RDAstrcmp(cat1,cat2) || RDAstrcmp(itemid,itemid1) ||
						RDAstrcmp(vendorid,venid1))
					{
						readwidget(b->funcargs,"[REQITM][UNIT RATE]");
						FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
						readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
						FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
					} else {
						FINDFLDGETDOUBLE(reqpven,"UNIT RATE",&unitrate);
						FINDFLDGETDOUBLE(reqpven,"PERCENTAGE DISCOUNT",&percdisc);
					}
				} else {
					readwidget(b->funcargs,"[REQITM][UNIT RATE]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
					readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
				}
				if(percdisc!=0.0)
				{
					total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
				} else {
					total=round(((unitrate*quantity)*100.0));
				}
				FINDRSCSETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",unitrate);
				FINDRSCSETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",
					percdisc);
				FINDRSCSETDOUBLE(b->funcargs,"[REQITM][AMOUNT]",total);
			}
			setaddresses(b->funcargs);
		}
		if(cat1!=NULL) Rfree(cat1);
		if(cat2!=NULL) Rfree(cat2);
		if(itemid1!=NULL) Rfree(itemid1);
		if(venid1!=NULL) Rfree(venid1);
		if(itemid!=NULL) Rfree(itemid);
		if(vendorid!=NULL) Rfree(vendorid);
		if(venitem!=NULL) Rfree(venitem);
		computeallSCRNvirtuals(b->funcargs);
		updateallrsrc(b->funcargs);
	}
	setaddresses(b->funcargs);
}
void setaddresses(RDArsrc *mtnrsrc)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL;
	char delflag=FALSE;
	int x=0;

	if(addresslist!=NULL) freeapplib(addresslist);
		addresslist=APPlibNEW();
	FINDRSCGETSTRING(mtnrsrc,"[REQITM][VENDOR IDENTIFICATION]",&vendorid1);
/*
	FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDGETSTRING(reqitm,"VENDOR IDENTIFICATION",&vendorid1);
*/
	FINDFLDGETSTRING(reqitm,"ADDRESS IDENTIFICATION",&addrid1);
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	ef=ADVGTENRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(addresslist,addrid2);
		}
		ef=ADVNXTNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(addresslist->numlibs<1)
	{
		addAPPlib(addresslist,"No Addresses Defined.");
	} else {
		for(x=0;x<addresslist->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,addresslist->libs[x]))
				break;
		}
		if(x>=addresslist->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addresslist->libs[x]);
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
	filerecord2rsrc(venadrnum,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"ADDRESS IDENTIFICATIONS",x,addresslist))
	{
		updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
	}
	if(addrid1!=NULL) Rfree(addrid1);
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
void getaddrcb(RDArsrc *mtnrsrc)
{
	char *addrid1=NULL;

	readwidget(mtnrsrc,"[REQITM][ADDRESS IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[REQITM][ADDRESS IDENTIFICATION]",&addrid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addrid1);
	getaddr(mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
	if(addrid1!=NULL) Rfree(addrid1);
}
void getaddr(RDArsrc *mtnrsrc)
{
	char *addrid1=NULL;
	int x;

	FINDFLDGETSTRING(reqitm,"ADDRESS IDENTIFICATION",&addrid1);
	if(RDAstrcmp(addresslist->libs[0],"No Addresses Defined."))
	{
		for(x=0;x<addresslist->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,addresslist->libs[x]))
				break;
		}
		if(x>=addresslist->numlibs)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ADDRESS IDENTIFICATION!","The Vendor Address Identification entered does not exist for the selected Vendor.",NULL,NULL,TRUE,NULL);
		} else {
			FINDRSCSETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",x);
			updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
		}
	}
	if(addrid1!=NULL) Rfree(addrid1);
}
void changeADDRlist(RDArsrc *mtnrsrc)
{
	int seladdr;
	char *vendorid1=NULL;

	if(FINDRSCGETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",&seladdr)) return;
	readwidget(mtnrsrc,"[REQITM][ADDRESS IDENTIFICATION]");
	FINDRSCSETSTRING(mtnrsrc,"[REQITM][ADDRESS IDENTIFICATION]",
	addresslist->libs[seladdr]);
	ZERNRD(venadrnum);
	if(!RDAstrcmp(addresslist->libs[seladdr],"No Addresses Defined."))
	{
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",NULL);
	} else {
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
		FINDRSCSETSTRING(mtnrsrc,"[REQITM][ADDRESS IDENTIFICATION]",addresslist->libs[seladdr]);
		FINDFLDSETSTRING(reqitm,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
	}
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
	filerecord2rsrc(venadrnum,mtnrsrc);
	updatersrc(mtnrsrc,"[REQITM][ADDRESS IDENTIFICATION]");
	updateSCRNvirtuals(mtnrsrc);
	if(vendorid1!=NULL) Rfree(vendorid1);
}
