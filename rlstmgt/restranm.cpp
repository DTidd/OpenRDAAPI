/* restranm.c - Xpert Vendor Voucher Master Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <fin.h>
#include <rlstmgt.h>

struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *trantypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;
extern short baccnum,raccnum,byrnum,ryrnum;
extern short rstmst,rstcm,rstcp,rstci;
static void getrecordsetedit(RDArsrc *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *mtn);
static void getsupportingrecords(RDArsrc *,char);
static void save_check(RDArsrc *,mtnstruct *,short);
extern char *module;
extern char editable;
extern MakeBrowseList *mbl_restran;
extern void browse_vch(RDArsrc *,mtnstruct *);
void restranm(short,void *);
extern MaintainMaster *MASTER;
static void changeDEFtype(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static short chkaccountcode(RDArsrc *,mtnstruct *);
static void chkaccountcodecb(RDArsrc *,mtnstruct *);
static short chkrevenuecode(RDArsrc *,mtnstruct *);
static void chkrevenuecodecb(RDArsrc *,mtnstruct *);
static short chkcashcode(RDArsrc *,mtnstruct *);
static void chkcashcodecb(RDArsrc *,mtnstruct *);
static void clearwidget(RDArsrc *,char *);
static short checktrantype(RDArsrc *,mtnstruct *);
static void changetrantype(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(),void *,void *);
static void set_appdefaults(RDArsrc *,short);

static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,save_record_quit,quit_record,mtn);
}
static void clearwidget(RDArsrc *mtnrsrc,char *widgetname)
{
#ifdef CLEARWDGT
	FINDRSCSETSTRING(mtnrsrc,widgetname,NULL);
	updatersrc(mtnrsrc,widgetname);
#endif
}
static void chkaccountcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0,yearid=0;

	readwidget(mtnrsrc,"[RESTRAN][TAX YEAR]");
	FINDRSCGETINT(mtnrsrc,"[RESTRAN][TAX YEAR]",&yearid);
	FINDFLDSETINT(MASTER->passkey->fileno,"TAX YEAR",yearid);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[RESTRAN][ACCOUNT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTRAN][ACCOUNT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkaccountcode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkaccountcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL;
	short deftype=0;
	int yearid=0;

	FINDFLDGETINT(MASTER->passkey->fileno,"TAX YEAR",&yearid);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",&acctcode);
	if((tmp=ADVCHECKACCT(ryrnum,raccnum,acctcode,REV_ACCT,deftype,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ACCOUNT CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"[RESTRAN][ACCOUNT CODE]",NULL);
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		Rfree(tmp);
		clearwidget(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		return(FALSE);
	}
	FINDFLDGETSTRING(raccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	return(TRUE);
}
static void chkrevenuecodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0,yearid=0;

	readwidget(mtnrsrc,"[RESTRAN][TAX YEAR]");
	FINDRSCGETINT(mtnrsrc,"[RESTRAN][TAX YEAR]",&yearid);
	FINDFLDSETINT(MASTER->passkey->fileno,"TAX YEAR",yearid);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[RESTRAN][REVENUE CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTRAN][REVENUE CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"REVENUE CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkrevenuecode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkrevenuecode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL,*tmp2=NULL;
	short deftype=0;
	int yearid=0;

	FINDFLDGETINT(MASTER->passkey->fileno,"TAX YEAR",&yearid);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"REVENUE CODE",&acctcode);
	if((tmp=ADVCHECKACCT(byrnum,baccnum,acctcode,BAL_ACCT,deftype,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"[RESTRAN][REVENUE CODE]",NULL);
		clearwidget(mtnrsrc,"REVENUE CODE DESCRIPTION");
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		Rfree(tmp);
		return(FALSE);
	}
	if(RDAstrcmp(tmp=ADVGETBALREF(baccnum,acctcode,deftype,SCRNvirtualSubData,mtnrsrc),"REVENUE"))
	{
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode)+200);
		sprintf(tmp2,"The Balance Sheet Account [%s] exists and is active for the REVENUE CODE, but it's REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [REVENUE] is required.",acctcode,tmp);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE CODE!",tmp2,NULL,NULL,TRUE,NULL);
		clearwidget(mtnrsrc,"[RESTRAN][REVENUE CODE]");
		clearwidget(mtnrsrc,"REVENUE CODE DESCRIPTION");
		if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		return(FALSE);
	}
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"REVENUE CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"REVENUE CODE DESCRIPTION");
	return(TRUE);
}
static void chkcashcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0,yearid=0;

	readwidget(mtnrsrc,"[RESTRAN][TAX YEAR]");
	FINDRSCGETINT(mtnrsrc,"[RESTRAN][TAX YEAR]",&yearid);
	FINDFLDSETINT(MASTER->passkey->fileno,"TAX YEAR",yearid);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[RESTRAN][CASH CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTRAN][CASH CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"CASH CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkcashcode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkcashcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL,*tmp2=NULL;
	short deftype=0;
	int yearid=0;

	FINDFLDGETINT(MASTER->passkey->fileno,"TAX YEAR",&yearid);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CASH CODE",&acctcode);
	if((tmp=ADVCHECKACCT(byrnum,baccnum,acctcode,BAL_ACCT,deftype,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"[RESTRAN][CASH CODE]",NULL);
		clearwidget(mtnrsrc,"CASH CODE DESCRIPTION");
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		Rfree(tmp);
		return(FALSE);
	}
	if(RDAstrcmp(tmp=ADVGETBALREF(baccnum,acctcode,deftype,SCRNvirtualSubData,mtnrsrc),"CASH"))
	{
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode)+200);
		sprintf(tmp2,"The Balance Sheet Account [%s] exists and is active for the CASH CODE, but it's REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [CASH] is required.",acctcode,tmp);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE!",tmp2,NULL,NULL,TRUE,NULL);
		clearwidget(mtnrsrc,"[RESTRAN][CASH CODE]");
		clearwidget(mtnrsrc,"CASH CODE DESCRIPTION");
		if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		return(FALSE);
	}
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	return(TRUE);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[RESTRAN][ACCOUNT CODE]");
	changeDEFtype(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[RESTRAN][ACCOUNT CODE]");
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	updatersrc(mtnrsrc,"[RESTRAN][REVENUE CODE]");
	updatersrc(mtnrsrc,"REVENUE CODE DESCRIPTION");
	updatersrc(mtnrsrc,"[RESTRAN][CASH CODE]");
	updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short deftype1=0,deftype2=0;
	int seldeftype2=0;

	FINDRSCGETDEFTYPE(mtnrsrc,"[RESTRAN][ACCOUNT CODE]",&deftype1);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype2);
	deftype2=(short)seldeftype2;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ACCOUNT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[RESTRAN][ACCOUNT CODE]",deftype2);
		FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][REVENUE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"REVENUE CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[RESTRAN][REVENUE CODE]",deftype2);
		FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][CASH CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[RESTRAN][CASH CODE]",deftype2);
	}
}
static void changetrantype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[RESTRAN][REFERENCE IDENTIFICATION]");
	readwidget(mtnrsrc,"[RESTMST][CLASS IDENTIFICATION]");
	if(!checktrantype(mtnrsrc,mtn))
		clearwidget(mtnrsrc,"[RESTRAN][REFERENCE IDENTIFICATION]");
	updateSCRNvirtuals(mtnrsrc);
}
static short checktrantype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	char *refid=NULL,*classid=NULL,*tmp2=NULL;

	FINDRSCGETINT(mtnrsrc,"TRANSACTION TYPES",&selected);
	FINDRSCGETSTRING(mtnrsrc,"[RESTRAN][REFERENCE IDENTIFICATION]",&refid);
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][CLASS IDENTIFICATION]",&classid);
	switch(selected)
	{
		default:
		case 0:
			ZERNRD(rstcp);
			ZERNRD(rstci);
			FINDFLDSETSTRING(rstcm,"CLASS IDENTIFICATION",classid);
			FINDFLDSETSTRING(rstcm,"MILLAGE IDENTIFICATION",refid);
			if(ADVEQLNRDsec(rstcm,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(rstcm,1);
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				tmp2=Rmalloc(RDAstrlen(refid)+RDAstrlen(classid)+200);
				sprintf(tmp2,"The Reference Identification [%s] is not found for Class [%s] in the Real Estate Class Millage Master.",refid,classid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REFERENCE IDENTIFICATION!",tmp2,NULL,NULL,TRUE,NULL);
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				if(classid!=NULL) Rfree(classid);
				if(refid!=NULL) Rfree(refid);
				return(FALSE);
			}
			break;
		case 1:
			ZERNRD(rstcm);
			ZERNRD(rstci);
			FINDFLDSETSTRING(rstcp,"CLASS IDENTIFICATION",classid);
			FINDFLDSETSTRING(rstcp,"PENALTY IDENTIFICATION",refid);
			if(ADVEQLNRDsec(rstcp,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(rstcp,1);
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				tmp2=Rmalloc(RDAstrlen(refid)+RDAstrlen(classid)+200);
				sprintf(tmp2,"The Reference Identification [%s] is not found for Class [%s] in the Real Estate Class Penalty Master.",refid,classid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REFERENCE IDENTIFICATION!",tmp2,NULL,NULL,TRUE,NULL);
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				if(classid!=NULL) Rfree(classid);
				if(refid!=NULL) Rfree(refid);
				return(FALSE);
			}
			break;
		case 2:
			ZERNRD(rstcm);
			ZERNRD(rstcp);
			FINDFLDSETSTRING(rstci,"CLASS IDENTIFICATION",classid);
			FINDFLDSETSTRING(rstci,"INTEREST IDENTIFICATION",refid);
			if(ADVEQLNRDsec(rstci,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(rstci,1);
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				tmp2=Rmalloc(RDAstrlen(refid)+RDAstrlen(classid)+200);
				sprintf(tmp2,"The Reference Identification [%s] is not found for Class [%s] in the Real Estate Class Interest Master.",refid,classid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REFERENCE IDENTIFICATION!",tmp2,NULL,NULL,TRUE,NULL);
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				if(classid!=NULL) Rfree(classid);
				if(refid!=NULL) Rfree(refid);
				return(FALSE);
			}
			break;
	}
	if(classid!=NULL) Rfree(classid);
	if(refid!=NULL) Rfree(refid);
	return(TRUE);
}
static void setfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	getsupportingrecords(mtnrsrc,TRUE);
}
void restranm(short dowhich,void *targetkey)
{
	char *date=NULL,*timex=NULL,*name=NULL;
	int which_type=0,x;
	short nofields=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn;
	RDAacct *acc=NULL;
	
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=APPlibNEW();
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acc=REVENUE_ACCOUNT->codetype;x<REVENUE_ACCOUNT->num;++x,++acc)
			{
				addAPPlib(mtn->DEFtypes,acc->name);
			}
		}
	} 
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	mtn->trantypes=APPlibNEW();
	addAPPlib(mtn->trantypes,"Tax Millage");
	addAPPlib(mtn->trantypes,"Penalty");
	addAPPlib(mtn->trantypes,"Interest");
	ZERNRD(MASTER->passkey->fileno);
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(MASTER->passkey->fileno);
	fields=FLDPOINTER(MASTER->passkey->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"TRANSACTION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"TRANSACTION TYPES",
						&which_type,(edit_rsrc==TRUE?TRUE:FALSE),
						changetrantype,mtn->trantypes->numlibs,
						&mtn->trantypes->libs,mtn);
				}
			} else {
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,
						RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5
						+RDAstrlen(MASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,setfiles,MASTER);
			}
		}
		if(name!=NULL) Rfree(name);
	}
	nofields=0;
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][REAL ESTATE IDENTIFICATION]",
		getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][OWNER IDENTIFICATION]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][TAX YEAR]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][TRANSACTION NUMBER]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][ACCOUNT CODE]",chkaccountcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][REVENUE CODE]",chkrevenuecodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][CASH CODE]",chkcashcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTRAN][REFERENCE IDENTIFICATION]",
		changetrantype,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"REVENUE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CASH CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
				SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
					SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					date=GETCURRENTDATE();
					timex=GETCURRENTTIME();
					FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][SOURCE USER]",
						USERLOGIN);
					FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY DATE]",date);
					FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY TIME]",timex);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				date=GETCURRENTDATE();
				timex=GETCURRENTTIME();
				FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY TIME]",timex);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,
				SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					date=GETCURRENTDATE();
					timex=GETCURRENTTIME();
					FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][SOURCE USER]",
						USERLOGIN);
					FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY DATE]",date);
					FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY TIME]",timex);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn);
			}
			break;
	}
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	getsupportingrecords(mtnrsrc,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_restran!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_vch,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADVFMakePrevNextButtons(mtnrsrc,MASTER->passkey->fileno,getpreviouscb,
		mtn,getnextcb,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	if(mbl_restran!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_restran==NULL?TRUE:FALSE));
}
static void getrecordsetedit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int which_type=0;
	short deftype1=0,x=0;

	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype1);
	which_type=(int)deftype1;
	if(which_type>=mtn->DEFtypes->numlibs)
	{
		addAPPlib(mtn->DEFtypes,"Invalid Definition Type");
		which_type=mtn->DEFtypes->numlibs-1;
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",which_type);
	updatersrc(mtnrsrc,"DEFINITION TYPES");
	FINDRSCSETDEFTYPE(mtnrsrc,"[RESTRAN][ACCOUNT CODE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[RESTRAN][REVENUE CODE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[RESTRAN][CASH CODE]",deftype1);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"TRANSACTION TYPE",&x);
	which_type=(int)x;
	FINDRSCSETINT(mtnrsrc,"TRANSACTION TYPES",which_type);
	updatersrc(mtnrsrc,"TRANSACTION TYPES");
	updatefilersrc(MASTER->passkey->fileno,mtnrsrc);
	chkaccountcode(mtnrsrc,mtn);
	chkrevenuecode(mtnrsrc,mtn);
	chkcashcode(mtnrsrc,mtn);
	seteditable(mtnrsrc,mtn);
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
#ifdef RADDOG
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[RESTRAN][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"TRANSACTION TYPES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[RESTRAN][DELETEFLAG]",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,
			MASTER->passkey->keyno,FALSE,FALSE);
	} else {
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"TRANSACTION TYPES",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,
			MASTER->passkey->keyno,TRUE,TRUE);
	}
#endif
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,mtn,FALSE);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",type);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION TYPES",&selected);
	FINDFLDSETINT(MASTER->passkey->fileno,"TRANSACTION TYPE",selected);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,
	short update_list)
{
	char *temp=NULL,delflag=FALSE;

	FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(chkaccountcode(mtnrsrc,mtn)) 
		{
			if(chkrevenuecode(mtnrsrc,mtn))
			{
				if(!chkcashcode(mtnrsrc,mtn)) return;
/*
				if(chkcashcode(mtnrsrc,mtn))
				{
					if(!checktrantype(mtnrsrc,mtn)) return;
				} else return;
*/
			} else return;
		} else return;
	}
	if(!SAVE_CHECK(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MASTER->save_expression))
	{
		if(EVALUATEbol(MASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			temp=EVALUATEstr(MASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",temp,NULL,FALSE);
			if(temp!=NULL) Rfree(temp);
			return;
		}
	}
	if(!isEMPTY(MASTER->save_warning))
	{
		if(EVALUATEbol(MASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			temp=EVALUATEstr(MASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				temp,(update_list ? save_record_up:save_record_nup),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(temp!=NULL) Rfree(temp);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,
	short update_list)
{
	char *rcddesc=NULL,*realestateid1=NULL,*realestateid2=NULL;
	char *ownid1=NULL,*ownid2=NULL;
	char deleteflag=0,vdeleteflag=0;
	char dist=FALSE;
	short paidnum=0,taxdwr=0,numflds=0,y;
	short ef_paid=0,deftype=0,p_month=0,type=0;
	double amount=0.0;
	int tran1=0,tran2=0,yearid1=0,yearid2=0,FiscalYear=0;
	RDATData *prev=NULL;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	RDArsrc *tmprsrc=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		numflds=NUMFLDS(MASTER->passkey->fileno);
		if(numflds>0)
		{
			tmpfld=Rmalloc(numflds*sizeof(NRDfield));
			for(y=0,fldx=tmpfld;y<numflds;++y,++fldx)
			{
				tmp=FLDNUM(MASTER->passkey->fileno,y);
				cpfldinfo(fldx,tmp);
			}
			LOCNRDFILE(MASTER->passkey->fileno);
			if(!ADVEQLNRDsec(MASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
			{
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
					ADVUpdateRealEstateTransactions(rstmst,(-1),MASTER->passkey->fileno,-amount,SCRNvirtualSubData,mtnrsrc);
				}
			}
			UNLNRDFILE(MASTER->passkey->fileno);
			fields2file(MASTER->passkey->fileno,tmpfld);
		}
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			ADVUpdateRealEstateTransactions(rstmst,(-1),MASTER->passkey->fileno,amount,SCRNvirtualSubData,mtnrsrc);
		} else {
			paidnum=OPNNRDsec(module,"RESTPAID",TRUE,TRUE,FALSE);
			taxdwr=OPNNRDsec("TAXCLT","TAXDRWR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(module,NULL);
			copyDFincvir(mtnrsrc,tmprsrc);
			addDFincvir(tmprsrc,module,"RESTPAID",NULL,paidnum);
			addDFincvir(tmprsrc,"TAXCLT","TAXDRWR",NULL,taxdwr);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			FINDFLDGETSTRING(MASTER->passkey->fileno,"REAL ESTATE IDENTIFICATION",
				&realestateid1);
			FINDFLDGETSTRING(MASTER->passkey->fileno,"OWNER IDENTIFICATION",&ownid1);
			FINDFLDGETINT(MASTER->passkey->fileno,"TAX YEAR",&yearid1);
			FINDFLDGETINT(MASTER->passkey->fileno,"TRANSACTION NUMBER",&tran1);
			if(paidnum!=(-1) && ryrnum!=(-1) && byrnum!=(-1))
			{
				ZERNRD(paidnum);
				FINDFLDSETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",realestateid1);
				FINDFLDSETSTRING(paidnum,"OWNER IDENTIFICATION",ownid1);
				FINDFLDSETINT(paidnum,"TAX YEAR",yearid1);
				FINDFLDSETINT(paidnum,"TRANSACTION NUMBER",tran1);
				ef_paid=ADVGTENRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_paid)
				{
					FINDFLDGETCHAR(paidnum,"DELETEFLAG",&vdeleteflag);
					if(!vdeleteflag)
					{
						FINDFLDGETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",&realestateid2);
						FINDFLDGETSTRING(paidnum,"OWNER IDENTIFICATION",&ownid2);
						FINDFLDGETINT(paidnum,"TAX YEAR",&yearid2);
						FINDFLDGETINT(paidnum,"TRANSACTION NUMBER",&tran2);
						if(RDAstrcmp(realestateid2,realestateid1) || RDAstrcmp(ownid1,ownid2) || yearid1!=yearid2 || tran1!=tran2) break;
						prev=RDATDataNEW(paidnum);
						FINDFLDGETCHAR(paidnum,"DISTRIBUTED",&dist);
						if(dist==FALSE)
						{
							FINDFLDGETSHORT(paidnum,"TRANSACTION TYPE",&type);
							FINDFLDGETSHORT(paidnum,"DEFINITION TYPE",&deftype);
							FINDFLDGETSHORT(paidnum,"PROCESSING MONTH",&p_month);
				 			FINDFLDGETDOUBLE(paidnum,"AMOUNT",&amount);
							FINDFLDGETINT(paidnum,"FISCAL YEAR",&FiscalYear);
							if(!type)
							{
								ADVupdateundist(MASTER->passkey->fileno,NULL,"CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,FiscalYear,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,FiscalYear,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
							}
						}
						ADVUpdateRealEstatePayments(taxdwr,rstmst,MASTER->passkey->fileno,paidnum,-amount,SCRNvirtualSubData,tmprsrc);
						FINDFLDSETCHAR(paidnum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(paidnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
						if(prev!=NULL) FreeRDATData(prev);
					}
					ef_paid=ADVNXTNRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
				}
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			if(paidnum!=(-1)) CLSNRD(paidnum);
			if(taxdwr!=(-1)) CLSNRD(taxdwr);
		}
		if(realestateid1!=NULL) Rfree(realestateid1);
		if(realestateid2!=NULL) Rfree(realestateid2);
		if(ownid1!=NULL) Rfree(ownid1);
		if(ownid2!=NULL) Rfree(ownid2);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_restran,MASTER->passkey->fileno,module,mtnrsrc);
		if(mbl_restran!=NULL) 
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn);
			GetRDATData(MASTER->passkey->fileno,mtn->previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->trantypes!=NULL) freeapplib(mtn->trantypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(MASTER!=NULL) 
		{
			CLOSE_MASTER_FILES(MASTER);
			FreeMaintainMaster(MASTER);
		}
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid=NULL,*ownid=NULL;
	int yearid=0,transno=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[RESTRAN][REAL ESTATE IDENTIFICATION]",&realestateid);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"REAL ESTATE IDENTIFICATION",realestateid);
	FINDRSCGETSTRING(mtnrsrc,"[RESTRAN][OWNER IDENTIFICATION]",&ownid);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"OWNER IDENTIFICATION",ownid);
	FINDRSCGETINT(mtnrsrc,"[RESTRAN][TAX YEAR]",&yearid);
	FINDFLDSETINT(MASTER->passkey->fileno,"TAX YEAR",yearid);
	FINDRSCGETINT(mtnrsrc,"[RESTRAN][TRANSACTION NUMBER]",&transno);
	FINDFLDSETINT(MASTER->passkey->fileno,"TRANSACTION NUMBER",transno);
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"TRANSACTION TYPES",TRUE);
	if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
		set_appdefaults(mtnrsrc,FALSE);
		updatefilersrc(MASTER->passkey->fileno,mtnrsrc);
		ClearRDATData(mtn->previous);
	} else {
		GetRDATData(MASTER->passkey->fileno,mtn->previous);
		getrecordsetedit(mtnrsrc,mtn);
	}
	getsupportingrecords(mtnrsrc,TRUE);
	if(ownid!=NULL) Rfree(ownid);
	if(realestateid!=NULL) Rfree(realestateid);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,char update)
{
	SetSupportingFiles(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,
		update,FALSE);
	if(update) updateSCRNvirtuals(mtnrsrc);
	else computeallSCRNvirtuals(mtnrsrc);
}
static void set_appdefaults(RDArsrc *mtnrsrc,short update)
{
	char *date=NULL,*timex=NULL;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][SOURCE USER]",USERLOGIN);
	FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[RESTRAN][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update) 
	{
		updatersrc(mtnrsrc,"[RESTRAN][SOURCE USER]");
		updatersrc(mtnrsrc,"[RESTRAN][ENTRY DATE]");
		updatersrc(mtnrsrc,"[RESTRAN][ENTRY TIME]");
	}
}
static void seteditablecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
        void (*SubFunc)(),void *args,mtnstruct *mtn)
{
		getrecordsetedit(mtnrsrc,mtn);
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,void *funcargs)
{
	set_appdefaults(mtnrsrc,TRUE);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,mtn,
		NULL,NULL,seteditablecb,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,mtn,
		NULL,NULL,seteditablecb,mtn);
}
