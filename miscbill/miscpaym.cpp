/* miscpaym.c - Xpert Misc Billing Payment Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <miscbill.h>
#include <fin.h>

extern short byrnum,ryrnum,rstrn,taxdwr;
extern short pptmstr;
static void getrecordsetedit(RDArsrc *);
static void seteditable(RDArsrc *);
static void save_record(RDArsrc *,RDATData *,short);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *mtnrsrc);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void getrecordcb(RDArsrc *,RDATData *);
static void getsupportingrecords(RDArsrc *,char);
static void save_check(RDArsrc *,RDATData *,short);
extern char *module;
extern char editable;
extern MakeBrowseList *mbl_miscpay;
extern void browse_vch(RDArsrc *,RDATData *);
void miscpaym(short,void *);
extern MaintainMaster *MASTER;
static void changedrawer(RDArsrc *);
static void fieldfunc(RDArsrc *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(),void *,void *);
static void set_appdefaults(RDArsrc *,short);

static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
static void changedrawer(RDArsrc *mtnrsrc)
{
	char *cashdrwr=NULL;

	readwidget(mtnrsrc,"[MISCPAY][CASH DRAWER]");
	FINDRSCGETSTRING(mtnrsrc,"[MISCPAY][CASH DRAWER]",&cashdrwr);
	ZERNRD(taxdwr);
	FINDFLDSETSTRING(taxdwr,"CASH DRAWER",cashdrwr);
	if(ADVEQLNRDsec(taxdwr,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(taxdwr,1);
	}
	updatefilerecord2rsrc(taxdwr,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
void miscpaym(short dowhich,void *targetkey)
{
	char *date=NULL,*timex=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	
	ZERNRD(MASTER->passkey->fileno);
	previous=RDATDataNEW(MASTER->passkey->fileno);
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(MASTER->passkey->fileno!=(-1)) ADVfile2rsrc(MASTER->passkey->fileno,mtnrsrc,(editable ? TRUE:FALSE),fieldfunc,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[MISCPAY][MISC IDENTIFICATION]",
		getrecordcb,previous);
	FINDRSCSETFUNC(mtnrsrc,"[MISCPAY][CLASS IDENTIFICATION]",getrecordcb,previous);
	FINDRSCSETFUNC(mtnrsrc,"[MISCPAY][OWNER IDENTIFICATION]",getrecordcb,
		previous);
	FINDRSCSETFUNC(mtnrsrc,"[MISCPAY][TAX YEAR]",getrecordcb,previous);
	FINDRSCSETFUNC(mtnrsrc,"[MISCPAY][TRANSACTION NUMBER]",getrecordcb,
		previous);
	FINDRSCSETFUNC(mtnrsrc,"[MISCPAY][PAYMENT NUMBER]",getrecordcb,previous);
	FINDRSCSETFUNC(mtnrsrc,"[MISCPAY][CASH DRAWER]",changedrawer,NULL);
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
					FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][SOURCE USER]",USERLOGIN);
					FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY DATE]",date);
					FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY TIME]",timex);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					getrecordsetedit(mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				getrecordsetedit(mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
				SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				date=GETCURRENTDATE();
				timex=GETCURRENTTIME();
				FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY TIME]",timex);
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				getrecordsetedit(mtnrsrc);
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
					FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][SOURCE USER]",USERLOGIN);
					FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY DATE]",date);
					FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY TIME]",timex);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					getrecordsetedit(mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				getrecordsetedit(mtnrsrc);
			}
			break;
	}
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	getsupportingrecords(mtnrsrc,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_miscpay!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_vch,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_miscpay!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	ADVFMakePrevNextButtons(mtnrsrc,MASTER->passkey->fileno,getpreviouscb,
		previous,getnextcb,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_miscpay==NULL?TRUE:FALSE));
}
static void getrecordsetedit(RDArsrc *mtnrsrc)
{
	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	seteditable(mtnrsrc);
}
static void seteditable(RDArsrc *mtnrsrc)
{
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[MISCPAY][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[MISCPAY][DELETEFLAG]",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,
			MASTER->passkey->keyno,FALSE,FALSE);
	} else {
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,
			MASTER->passkey->keyno,TRUE,TRUE);
	}
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc);
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc);
	save_check(mtnrsrc,previous,FALSE);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *desc=NULL;

	if(!SAVE_CHECK(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MASTER->save_expression))
	{
		if(EVALUATEbol(MASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MASTER->save_error_desc,SCRNvirtualSubData,
				mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MASTER->save_warning))
	{
		if(EVALUATEbol(MASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,(update_list ? save_record_up:save_record_nup),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,
	short update_list)
{
	char deleteflag=0,*rcddesc=NULL,dist=FALSE;
	short numflds=0,y,deftype=0,p_month=0,type=0;
	double amount=0.0;
	int yearid=0,fyear=0;
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
		tmprsrc=RDArsrcNEW(module,NULL);
		copyDFincvir(mtnrsrc,tmprsrc);
		addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
		addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
		if(numflds>0)
		{
			tmpfld=Rmalloc(numflds*sizeof(NRDfield));
			for(y=0,fldx=tmpfld;y<numflds;++y,++fldx)
			{
				tmp=FLDNUM(MASTER->passkey->fileno,y);
				cpfldinfo(fldx,tmp);
			}
			LOCNRDFILE(MASTER->passkey->fileno);
			if(!ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTION",&dist);
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
					if(!dist)
					{
						COPYFIELD(MASTER->passkey->fileno,rstrn,"MISC IDENTIFICATION");
						COPYFIELD(MASTER->passkey->fileno,rstrn,"CLASS IDENTIFICATION");
						COPYFIELD(MASTER->passkey->fileno,rstrn,"OWNER IDENTIFICATION");
						FINDFLDGETINT(MASTER->passkey->fileno,"TAX YEAR",&yearid);
						FINDFLDSETINT(rstrn,"TAX YEAR",yearid);
						COPYFIELD(MASTER->passkey->fileno,rstrn,"TRANSACTION NUMBER");
						if(ADVEQLNRDsec(rstrn,1,SCRNvirtualSubData,tmprsrc)) KEYNRD(rstrn,1);
						FINDFLDGETSHORT(rstrn,"TRANSACTION TYPE",&type);
						FINDFLDGETSHORT(rstrn,"DEFINITION TYPE",&deftype);
						FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
						FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&fyear);
						if(!type)
						{
							ADVupdateundist(rstrn,NULL,"CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,fyear,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
						} else {
							ADVupdateundist(rstrn,"ACCOUNT CODE","CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,fyear,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
						}
						ADVUpdateMiscBillingPayments(taxdwr,pptmstr,rstrn,MASTER->passkey->fileno,-amount,SCRNvirtualSubData,tmprsrc);
					}
				}
			}
			UNLNRDFILE(MASTER->passkey->fileno);
			fields2file(MASTER->passkey->fileno,tmpfld);
		}
		WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTION",&dist);
			FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
			if(dist==FALSE)
			{
				COPYFIELD(MASTER->passkey->fileno,rstrn,"MISC IDENTIFICATION");
				COPYFIELD(MASTER->passkey->fileno,rstrn,"CLASS IDENTIFICATION");
				COPYFIELD(MASTER->passkey->fileno,rstrn,"OWNER IDENTIFICATION");
				FINDFLDGETINT(MASTER->passkey->fileno,"TAX YEAR",&yearid);
				FINDFLDSETINT(rstrn,"TAX YEAR",yearid);
				COPYFIELD(MASTER->passkey->fileno,rstrn,"TRANSACTION NUMBER");
				if(ADVEQLNRDsec(rstrn,1,SCRNvirtualSubData,tmprsrc)) KEYNRD(rstrn,1);
				FINDFLDGETSHORT(rstrn,"TRANSACTION TYPE",&type);
				FINDFLDGETSHORT(rstrn,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
				FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&fyear);
				if(!type)
				{
					ADVupdateundist(rstrn,NULL,"CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,fyear,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,tmprsrc);
				} else {
					ADVupdateundist(rstrn,"ACCOUNT CODE","CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,fyear,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,tmprsrc);
				}
				ADVUpdateMiscBillingPayments(taxdwr,pptmstr,rstrn,MASTER->passkey->fileno,amount,SCRNvirtualSubData,tmprsrc);
			}
		}
		if(tmprsrc!=NULL) free_rsrc(tmprsrc);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_miscpay,MASTER->passkey->fileno,module,mtnrsrc);
		if(mbl_miscpay!=NULL) 
		{
			quit_record(mtnrsrc,previous);
		} else {
			seteditable(mtnrsrc);
			GetRDATData(MASTER->passkey->fileno,previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
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
static void seteditablecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
        void (*SubFunc)(),void *args,RDATData *previous)
{
		getrecordsetedit(mtnrsrc);
}
static void getrecordcb(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV2SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,set_appdefaultscb,seteditablecb,previous);
}
static void fieldfunc(RDArsrc *mtnrsrc)
{
	readscreen(mtnrsrc);
	getsupportingrecords(mtnrsrc,TRUE);
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
	FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][SOURCE USER]",USERLOGIN);
	FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[MISCPAY][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update) 
	{
		updatersrc(mtnrsrc,"[MISCPAY][SOURCE USER]");
		updatersrc(mtnrsrc,"[MISCPAY][ENTRY DATE]");
		updatersrc(mtnrsrc,"[MISCPAY][ENTRY TIME]");
	}
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,void *funcargs)
{
	set_appdefaults(mtnrsrc,TRUE);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
}
