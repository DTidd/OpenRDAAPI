/* venavcim.c - Xpert's Add Vendor Voucher Invoice Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <mix.h>

struct mtnstructs
{
	APPlib *addresslist;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void addvouchers(void);
extern short yearnum,filenum,vchnum,adrnum;
extern MakeBrowseList *mbl_veninv;
extern char *vendorid,*invid;
extern int venyrid;
extern void browse_veninv(RDArsrc *,mtnstruct *);
extern MaintainMaster *VENINV_MASTER;

static short save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void setaddresses(RDArsrc *,mtnstruct *,short);
static void setlists(RDArsrc *,mtnstruct *,short);
static void setlistscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void set_appdefaults(RDArsrc *,mtnstruct *,short);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static short save_venyr(RDArsrc *);
static void getsupportingrecords(RDArsrc *,mtnstruct *,short);
static void getsupportingrecordscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
void change_invoice_total(RDArsrc *);
static void add_vouchers(RDArsrc *);
static void test_add_vouchers(RDArsrc *,mtnstruct *);
static void saveinv_addvchs(RDArsrc *,mtnstruct *);
char editinv=TRUE;
double invoice_total=0;
void changeADDRlist(RDArsrc *,mtnstruct *);
int setaddrlist(RDArsrc *,mtnstruct *);
void veninvm(short,void *);
int Calendar_Year=0;

void veninvm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	int which_type=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	double dlrs=0;

	mtnrsrc=RDArsrcNEW(VENINV_MASTER->module,VENINV_MASTER->maintain_screen);
	ZERNRD(VENINV_MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(VENINV_MASTER->passkey->fileno);
	mtn->addresslist=NULL;
	nokeys=NUMKEYS(VENINV_MASTER->passkey->fileno);
	keys=KEYPOINTER(VENINV_MASTER->passkey->fileno);
	nofields=NUMFLDS(VENINV_MASTER->passkey->fileno);
	fields=FLDPOINTER(VENINV_MASTER->passkey->fileno);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,filenum);
	addDFincvir(mtnrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(mtnrsrc,"VENPMT","VENINV",NULL,VENINV_MASTER->passkey->fileno);
	addDFincvir(mtnrsrc,"VENPMT","VENADR",NULL,adrnum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editinv;
			if(!RDAstrcmp(fldx->name,"ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(VENINV_MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
					mtn->addresslist=APPlibNEW();
					addAPPlib(mtn->addresslist,"No Addresses Defined");
					addlstrsrc(mtnrsrc,
						"VENINV ADDRESS IDENTIFICATIONS",
						&which_type,
						(edit_rsrc?TRUE:FALSE),
						changeADDRlist,
						mtn->addresslist->numlibs,
						&mtn->addresslist->libs,mtn);
				}
			} else {
				nonlstmakefld(mtnrsrc,VENINV_MASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(VENINV_MASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(VENINV_MASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",VENINV_MASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,VENINV_MASTER);
			}
		}
	}
	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->part!=NULL)
			{
				for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
				{
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(VENINV_MASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(VENINV_MASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",VENINV_MASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",NULL,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][CALENDAR YEAR]",NULL,NULL);
	if(filenum!=(-1)) file2rsrc(filenum,mtnrsrc,FALSE);
	if(yearnum!=(-1)) file2rsrc(yearnum,mtnrsrc,FALSE);
	if(adrnum!=(-1)) file2rsrc(adrnum,mtnrsrc,FALSE);
	addstdrsrc(mtnrsrc,"INVOICE TOTAL",DOLLARS,20,&dlrs,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"INVOICE TOTAL",change_invoice_total,NULL);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			if(filenum!=(-1))
			{
				ReadRDAScrolledLists(mtnrsrc);
				if(ADVEQLNRDKEYsec(filenum,1,targetkey,SCRNvirtualSubData,mtnrsrc)) 
				{
					KEYNRD(filenum,1);
					list2file(mtnrsrc,mtn);
				} else {
					filerecord2rsrc(filenum,mtnrsrc);
				}
			}
			venyrid=Calendar_Year;
			FINDFLDGETSTRING(filenum,"VENDOR IDENTIFICATION",&vendorid);
			if(VENINV_MASTER->passkey->fileno!=(-1))
			{
				ZERNRD(VENINV_MASTER->passkey->fileno);
				FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid);
				FINDFLDSETINT(VENINV_MASTER->passkey->fileno,"CALENDAR YEAR",venyrid);
				FINDRSCSETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",vendorid);
				FINDRSCSETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",venyrid);
				if(ADVEQLNRDsec(VENINV_MASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
				{
					KEYNRD(VENINV_MASTER->passkey->fileno,1);
					ReadRDAScrolledListsNoPassKeys(mtnrsrc,VENINV_MASTER->passkey);
					if(ADVEQLNRDsec(VENINV_MASTER->passkey->fileno,VENINV_MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
					{
						set_appdefaults(mtnrsrc,mtn,FALSE);
						list2file(mtnrsrc,mtn);
					} else {
						GetRDATData(VENINV_MASTER->passkey->fileno,mtn->previous);
						filerecord2rsrc(VENINV_MASTER->passkey->fileno,mtnrsrc);
						setlists(mtnrsrc,mtn,FALSE);
					}
				} else {
					GetRDATData(VENINV_MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(VENINV_MASTER->passkey->fileno,mtnrsrc);
					setlists(mtnrsrc,mtn,FALSE);
				}
			}
			break;
		case 2:
			ZERNRD(VENINV_MASTER->passkey->fileno);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,VENINV_MASTER->passkey);
			if(ADVEQLNRDsec(VENINV_MASTER->passkey->fileno,VENINV_MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(VENINV_MASTER->passkey->fileno,1);
				set_appdefaults(mtnrsrc,mtn,FALSE);
				list2file(mtnrsrc,mtn);
			} else {
				GetRDATData(VENINV_MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(VENINV_MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(VENINV_MASTER->passkey->fileno,1,targetkey,SCRNvirtualSubData,mtnrsrc)) 
			{
				KEYNRD(VENINV_MASTER->passkey->fileno,1);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,VENINV_MASTER->passkey);
				if(ADVEQLNRDsec(VENINV_MASTER->passkey->fileno,VENINV_MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					set_appdefaults(mtnrsrc,mtn,FALSE);
					list2file(mtnrsrc,mtn);
				} else {
					GetRDATData(VENINV_MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(VENINV_MASTER->passkey->fileno,mtnrsrc);
					setlists(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(VENINV_MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(VENINV_MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,mtn,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD VOUCHERS",TRUE,test_add_vouchers,mtn);
        addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
        if(mbl_veninv!=NULL)
        {
                addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
        } else {
                addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
        }
        addbtnrsrc(mtnrsrc,"BROWSE",TRUE,browse_veninv,mtn);
        addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
        addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
        addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
        addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
        addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
        MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
        computeallSCRNvirtuals(mtnrsrc);
        if(mbl_veninv!=NULL)
        {
                FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
        }
        if(!editinv)
        {
                FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
                FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
        }
	FINDRSCSETEDITABLE(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"[VENINV][CALENDAR YEAR]",FALSE);
	APPmakescrn(mtnrsrc,TRUE,quit_record,mtn,
		(mbl_veninv==NULL?TRUE:FALSE)); 
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *venid=NULL;
	int calyr=0;

	readwidget(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]");
	readwidget(mtnrsrc,"[VENINV][CALENDAR YEAR]");
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&venid);
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&calyr);
	ZERNRD(VENINV_MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",venid);
	FINDRSCSETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",calyr);
	if(venid!=NULL) Rfree(venid);
	updateallrsrc(mtnrsrc);
#ifdef ASK_SJS
	SEERSRCS(mtnrsrc);TRACE;
	SETID(mtnrsrc,mtn);
	SEERSRCS(mtnrsrc);TRACE;
#else
	readscreen(mtnrsrc,mtn);
	set_appdefaults(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
#endif
	updateSCRNvirtuals(mtnrsrc);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);
	char *venid=NULL,*invid=NULL;
	int calyr=0;

	readwidget(member->parent,"[VENINV][VENDOR IDENTIFICATION]");
	readwidget(member->parent,"[VENINV][CALENDAR YEAR]");
	readwidget(member->parent,"[VENINV][INVOICE IDENTIFICATION]");
	FINDRSCGETSTRING(member->parent,"[VENINV][VENDOR IDENTIFICATION]",&venid);
	FINDRSCGETINT(member->parent,"[VENINV][CALENDAR YEAR]",&calyr);
	FINDRSCGETSTRING(member->parent,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",venid);
	FINDFLDSETINT(VENINV_MASTER->passkey->fileno,"CALENDAR YEAR",calyr);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"INVOICE IDENTIFICATION",invid);
	keyno=KEYNUMBER(VENINV_MASTER->passkey->fileno,"VENDOR INVOICE KEY");
	if(keyno==(-1))
	{
		keyno=VENINV_MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,VENINV_MASTER,mtn->previous,
		SCRNvirtualSubData,member->parent,keyno,set_appdefaultscb,mtn,
		setlistscb,mtn,getsupportingrecordscb,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);
	char *venid=NULL,*invid=NULL;
	int calyr=0;

	readwidget(member->parent,"[VENINV][VENDOR IDENTIFICATION]");
	readwidget(member->parent,"[VENINV][CALENDAR YEAR]");
	readwidget(member->parent,"[VENINV][INVOICE IDENTIFICATION]");
	FINDRSCGETSTRING(member->parent,"[VENINV][VENDOR IDENTIFICATION]",&venid);
	FINDRSCGETINT(member->parent,"[VENINV][CALENDAR YEAR]",&calyr);
	FINDRSCGETSTRING(member->parent,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",venid);
	FINDFLDSETINT(VENINV_MASTER->passkey->fileno,"CALENDAR YEAR",calyr);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"INVOICE IDENTIFICATION",invid);
	keyno=KEYNUMBER(VENINV_MASTER->passkey->fileno,"VENDOR INVOICE KEY");
	if(keyno==(-1))
	{
		keyno=VENINV_MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,VENINV_MASTER,mtn->previous,
		SCRNvirtualSubData,member->parent,keyno,set_appdefaultscb,mtn,
		setlistscb,mtn,getsupportingrecordscb,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,VENINV_MASTER,mtn->previous,
		SCRNvirtualSubData,(void *)mtnrsrc,set_appdefaultscb,mtn,
		setlistscb,mtn,getsupportingrecordscb,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *VENINV_MASTER)
{
	SET_MASTER(mtnrsrc,VENINV_MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	
	FINDRSCGETINT(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->addresslist->libs[selected],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[selected]);
	} else {
		FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",NULL);
	}
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(VENINV_MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(VENINV_MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static short save_venyr(RDArsrc *mtnrsrc)
{
	char deleteflag=FALSE;

	ZERNRD(yearnum);
	FINDFLDSETSTRING(yearnum,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETINT(yearnum,"CALENDAR YEAR",venyrid);
	if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		if(ADVWRTTRANSsec(yearnum,0,NULL,NULL,SCRNvirtualSubData,mtnrsrc))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","SECURITY FAILED ON WRITE!","Security failed on attempt to New Vendor Year record.  Invoice entered exist but the Vendor Year record does not.  Cannot add Vendor Voucher records until this is resolved.",NULL,NULL,FALSE,NULL);
			return(FALSE);
		}
	} else {
		FINDFLDGETCHAR(yearnum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR YEAR!","Vendor Year record exist for this Invoice but it is in a state of deletion.  Cannot add Vendor Voucher records until this is resolved.",NULL,NULL,FALSE,NULL);
			return(FALSE);
		}
	}
	return(TRUE);
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
static short save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	if(save_venyr(mtnrsrc))
	{
		save_record(mtnrsrc,mtn,update);
		return(TRUE);
	} else return(FALSE);
}
void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *rcddesc=NULL,*vendorid1=NULL,*vendorid2=NULL;
	char *invid1=NULL,*invid2=NULL,*date=NULL,*timex=NULL;
	char deleteflag=0,mdeleteflag=0;
	short x=(-1),ef_vch=0;
	int yearid1=0,yearid2=0;
	int lf=0;
	RDArmem *member=NULL;
	RDATData *prev=NULL;

	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(VENINV_MASTER->passkey->fileno,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(VENINV_MASTER->passkey->fileno,"SOURCE USER VERIFIED",deleteflag);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	if((x=FINDRSC(mtnrsrc,"[VENINV][ENTRY DATE]"))!=(-1))
	{
		member=mtnrsrc->rscs+x;
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
			FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ENTRY DATE",date);
		}
	} else {
		FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date); 
	if((x=FINDRSC(mtnrsrc,"[VENINV][ENTRY TIME]"))!=(-1))
	{
		member=mtnrsrc->rscs+x;
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
			FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ENTRY TIME",timex);
		}
	} else {
		FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ENTRY DATE",timex);
	}
	if(timex!=NULL) Rfree(timex);
	if(ADVRECORDsec(VENINV_MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(VENINV_MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(VENINV_MASTER->passkey->fileno,0,NULL,mtn->previous);
		FINDFLDGETCHAR(VENINV_MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
			FINDFLDGETINT(VENINV_MASTER->passkey->fileno,"CALENDAR YEAR",&yearid1);
			FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"INVOICE IDENTIFICATION",&invid1);
			ZERNRD(vchnum);
			FINDFLDSETSTRING(vchnum,"VENDOR IDENTIFICATION",vendorid1);
			FINDFLDSETINT(vchnum,"CALENDAR YEAR",yearid1);
			FINDFLDSETSTRING(vchnum,"INVOICE IDENTIFICATION",invid1);
			ef_vch=ADVGTENRDsec(vchnum,1,SCRNvirtualSubData,mtnrsrc);
			while(!ef_vch)
			{
				FINDFLDGETCHAR(vchnum,"DELETEFLAG",&mdeleteflag);
				if(!mdeleteflag)
				{
					FINDFLDGETSTRING(vchnum,"VENDOR IDENTIFICATION",&vendorid2);
					FINDFLDGETINT(vchnum,"CALENDAR YEAR",&yearid2);
					FINDFLDGETSTRING(vchnum,"INVOICE IDENTIFICATION",&invid2);
					if(!RDAstrcmp(vendorid2,vendorid1) && yearid2==yearid1 && 
						(!RDAstrcmp(invid2,invid1)))
					{
						prev=RDATDataNEW(vchnum);
						FINDFLDSETCHAR(vchnum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(vchnum,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
						if(prev!=NULL) FreeRDATData(prev);
					}
				}
				ef_vch=ADVNXTNRDsec(vchnum,1,SCRNvirtualSubData,mtnrsrc);
				if(vendorid2!=NULL) Rfree(vendorid2);
				vendorid2=NULL;
				if(invid2!=NULL) Rfree(invid2);
				invid2=NULL;
			}
		}
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		updatebrowse(update,mbl_veninv,filenum,"VENPMT",mtnrsrc);
		if(mbl_veninv!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(VENINV_MASTER->passkey->fileno,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
	}
	if(WindowCount<2)
	{
		CLSNRD(VENINV_MASTER->passkey->fileno);
		CLSNRD(yearnum);
		CLSNRD(filenum);
		CLSNRD(vchnum);
		CLSNRD(adrnum);
		if(VENINV_MASTER!=NULL) FreeMaintainMaster(VENINV_MASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,VENINV_MASTER->passkey->fileno,mtn->previous,
		readscreen,save_record_quit,quit_record,mtn);
}
void change_invoice_total(RDArsrc *mtnrsrc)
{
	readwidget(mtnrsrc,"INVOICE TOTAL");
	FINDRSCGETDOUBLE(mtnrsrc,"INVOICE TOTAL",&invoice_total);
}
void test_add_vouchers(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	ZERNRD(VENINV_MASTER->passkey->fileno);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETINT(VENINV_MASTER->passkey->fileno,"CALENDAR YEAR",venyrid);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"INVOICE IDENTIFICATION",invid);
	if(ADVEQLNRDsec(VENINV_MASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","INVOICE DOES NOT EXIST!","Vouchers can not be added until the Invoice has been saved.  Invoice entered does not presently exist in the VENINV file.",saveinv_addvchs,NULL,FALSE,2,mtnrsrc,mtn,NULL);
	} else {
		if(save_venyr(mtnrsrc)) add_vouchers(mtnrsrc);
	}
}
void saveinv_addvchs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	if(mbl_veninv!=NULL)
	{
		if(save_check(mtnrsrc,mtn,TRUE)) add_vouchers(mtnrsrc);
	} else {
		if(save_check(mtnrsrc,mtn,FALSE)) add_vouchers(mtnrsrc);
	}
}
void add_vouchers(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	FINDRSCGETDOUBLE(mtnrsrc,"INVOICE TOTAL",&invoice_total);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	addvouchers();
}
static void setaddresses(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL;
	char delflag=FALSE;
	int x=0;

	if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
	mtn->addresslist=APPlibNEW();
	FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
	FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",&addrid1);
	ZERNRD(adrnum);
	FINDFLDSETSTRING(adrnum,"VENDOR IDENTIFICATION",vendorid1);
	ef=GTENRDsec(adrnum,1);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(adrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(adrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(adrnum,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(mtn->addresslist,addrid2);
		}
		ef=NXTNRDsec(adrnum,1);
	}
	if(mtn->addresslist->numlibs<1)
	{
		addAPPlib(mtn->addresslist,"No Addresses Defined");
	} else {
		for(x=0;x<mtn->addresslist->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addresslist->libs[x]))
			break;
		}
		if(x>=mtn->addresslist->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(adrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(adrnum,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[x]);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[x]);
	if(ADVEQLNRDsec(adrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(adrnum,1);
	filerecord2rsrc(adrnum,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS",x,mtn->addresslist))
	{
		updatersrc(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS");
	}
	if(addrid1!=NULL) Rfree(addrid1);
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
void changeADDRlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seladdr;
	char *vendorid1=NULL;

	if(FINDRSCGETINT(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS",&seladdr)) return;
	FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
	ZERNRD(adrnum);
	if(!RDAstrcmp(mtn->addresslist->libs[seladdr],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(adrnum,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(adrnum,"ADDRESS IDENTIFICATION",NULL);
		FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",NULL);
	} else {
		FINDFLDSETSTRING(adrnum,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(adrnum,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
		FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
	}
	if(ADVEQLNRDsec(adrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(adrnum,1);
	updatefilerecord2rsrc(adrnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
	if(vendorid1!=NULL) Rfree(vendorid1); 
}
int setaddrlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL;
	char delflag=FALSE,billing=FALSE;
	int billaddrno=0,x=0;

	if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
	mtn->addresslist=APPlibNEW();
	FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
	FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",&addrid1);
	ZERNRD(adrnum);
	FINDFLDSETSTRING(adrnum,"VENDOR IDENTIFICATION",vendorid1);
	ef=ADVGTENRDsec(adrnum,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(adrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(adrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(adrnum,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(mtn->addresslist,addrid2);
			if(!billing)
			{
				FINDFLDGETCHAR(adrnum,"DEFAULT BILLING",&billing);
				if(billing)
				{
					billaddrno=(mtn->addresslist->numlibs-1);
				}
			}
		}
		ef=ADVNXTNRDsec(adrnum,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(mtn->addresslist->numlibs<1)
	{
		addAPPlib(mtn->addresslist,"No Addresses Defined");
	} else {
		for(x=0;x<mtn->addresslist->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addresslist->libs[x]))
				break;
		}
		if(x>=mtn->addresslist->numlibs)
		{
			x=billaddrno;
		}
	}
	FINDFLDSETSTRING(adrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(adrnum,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[x]);
	if(ADVEQLNRDsec(adrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(adrnum,1);
	updatefilerecord2rsrc(adrnum,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS",x,mtn->addresslist))
	{
		updatersrc(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS");
	}
	if(addrid1!=NULL) Rfree(addrid1); 
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
	return(x);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *addrid=NULL;

	if(adrnum!=(-1))
	{
		FINDFLDGETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",&addrid);
		FINDFLDSETSTRING(adrnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETSTRING(adrnum,"ADDRESS IDENTIFICATION",addrid);
		if(ADVEQLNRDsec(adrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(adrnum,1);
		if(update) updatefilerecord2rsrc(adrnum,mtnrsrc);
		else filerecord2rsrc(adrnum,mtnrsrc);
		if(addrid!=NULL) Rfree(addrid);
	}
	if(yearnum!=(-1))
	{
		FINDFLDSETSTRING(yearnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(yearnum,"CALENDAR YEAR",venyrid);
		if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(yearnum,1);
		if(update) updatefilerecord2rsrc(yearnum,mtnrsrc);
		else filerecord2rsrc(yearnum,mtnrsrc);
	}
	if(filenum!=(-1))
	{
		FINDFLDSETSTRING(filenum,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(filenum,1);
		if(update) updatefilerecord2rsrc(filenum,mtnrsrc);
		else filerecord2rsrc(filenum,mtnrsrc);
	}
}
static void getsupportingrecordscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(VENINV_MASTER->passkey->fileno,mtnrsrc);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setaddresses(mtnrsrc,mtn,update);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *VENINV_MASTER,void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	setlists(mtnrsrc,mtn,TRUE);
}
static void set_appdefaults(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *date=NULL,*timex=NULL;
	char *vendorid1=NULL,*addrid1=NULL;
/*
	char *duedate=NULL,*invdate=NULL;
*/
	int yrid=0;

	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid1);
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&yrid);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][ADDRESS IDENTIFICATION]",&addrid1);
/*
	DefaultResetScreen(mtnrsrc,TRUE);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][DUE DATE]",&duedate);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE DATE]",&invdate);
*/
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",vendorid1);
	FINDRSCSETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",yrid);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][ADDRESS IDENTIFICATION]",addrid1);
	date=GETCURRENTDATE();
/*
	if(duedate==NULL) duedate=stralloc(date);
	if(invdate==NULL) invdate=stralloc(date);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][DUE DATE]",duedate);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][INVOICE DATE]",invdate);
*/
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETINT(VENINV_MASTER->passkey->fileno,"CALENDAR YEAR",yrid);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",addrid1);
/*
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"DUE DATE",duedate);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"INVOICE DATE",date);
*/
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"SOURCE USER",USERLOGIN);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][SOURCE USER]",USERLOGIN);
	timex=GETCURRENTTIME();
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ENTRY DATE",date);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY DATE]",date);
	FINDFLDSETSTRING(VENINV_MASTER->passkey->fileno,"ENTRY TIME",timex);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
/*
	if(duedate!=NULL) Rfree(duedate);
	if(invdate!=NULL) Rfree(invdate);
*/
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(addrid1!=NULL) Rfree(addrid1);
	setaddresses(mtnrsrc,mtn,update);
	if(update)
	{
/*
		updatersrc(mtnrsrc,"[VENINV][DUE DATE]");
		updatersrc(mtnrsrc,"[VENINV][INVOICE DATE]");
*/
		updatersrc(mtnrsrc,"[VENINV][SOURCE USER]");
		updatersrc(mtnrsrc,"[VENINV][ENTRY DATE]");
		updatersrc(mtnrsrc,"[VENINV][ENTRY TIME]");
	}
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *VENINV_MASTER,
        void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	set_appdefaults(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
}
