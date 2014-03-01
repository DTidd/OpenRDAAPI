/* finjenm.c - Financial Management Journal Entry Maintain */
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

struct mtnstructs
{
	APPlib *ACCTtypes;
	APPlib *DEFtypes;
	APPlib *jtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short byrnum,eyrnum,ryrnum;
extern short baccnum,eaccnum,raccnum;
extern RDAfinmgt *finsetup;
extern MaintainMaster *MASTER;
extern MakeBrowseList *mbl_finjen;
extern void browse_finjen(RDArsrc *,mtnstruct *);
static void getrecordsetedit(RDArsrc *,mtnstruct *,short);
static void getrecordseteditcb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void SETID(RDArsrc *,mtnstruct *);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void set_appdefaults(RDArsrc *,mtnstruct *,short);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *,short);
static void getsupportingrecords(RDArsrc *,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc*,mtnstruct *,int,int,short,short);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void changeACCTtype(RDArsrc *,mtnstruct *,int);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void setACCTtype(RDArsrc *,mtnstruct *,int,int,int,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static short check_acctcode(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static void acctcodecb(RDArsrc *,mtnstruct *);
static void debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void check_allcodes(RDArsrc *,mtnstruct *);
static short check_code(RDArsrc *,mtnstruct *,short);
static short testavailbalance(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);
char editable=TRUE;
void finjenm(short,void *);

void finjenm(short dowhich,void *targetkey)
{
	int which_type=0;
	char *name=NULL;
	short nofields=0,nokeys=0,x,y,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->ACCTtypes=NULL;
	mtn->DEFtypes=NULL;
	mtn->jtypes=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->DEFtypes=APPlibNEW();
	mtn->ACCTtypes=APPlibNEW();
	mtn->jtypes=APPlibNEW();
	addAPPlib(mtn->ACCTtypes,accttypes[0]);
	addAPPlib(mtn->ACCTtypes,accttypes[1]);
	addAPPlib(mtn->ACCTtypes,accttypes[2]);
	which_type=0;
	nokeys=NUMKEYS(MASTER->passkey->fileno);
	keys=KEYPOINTER(MASTER->passkey->fileno);
	nofields=NUMFLDS(MASTER->passkey->fileno);
	fields=FLDPOINTER(MASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"ACCOUNT TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						ACCTtypecb,mtn->ACCTtypes->numlibs,
						&mtn->ACCTtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"JOURNAL ENTRY TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"JOURNAL ENTRY TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						NULL,mtn->jtypes->numlibs,
						&mtn->jtypes->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))	
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else {
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MASTER);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[FINJEN][ACCOUNT CODE]",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[FINJEN][CREDIT CODE]",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[FINJEN][DEBIT CODE]",debcodecb,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	if(baccnum!=(-1)) file2rsrc(baccnum,mtnrsrc,FALSE);
	if(eaccnum!=(-1)) file2rsrc(eaccnum,mtnrsrc,FALSE);
	if(raccnum!=(-1)) file2rsrc(raccnum,mtnrsrc,FALSE);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					set_appdefaults(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				set_appdefaults(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					set_appdefaults(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,FALSE);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MASTER);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_finjen!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_finjen,mtn);
	if(mbl_finjen!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_finjen==NULL?TRUE:FALSE));
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	set_appdefaults(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,TRUE);
}
static void set_appdefaults(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *date=NULL,*timex=NULL;
	short accttype=0;

	FINDRSCSETSTRING(mtnrsrc,"[FINJEN][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[FINJEN][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[FINJEN][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update)
	{
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE);
	} else {
		seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
	}
	SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"JOURNAL ENTRY TYPES",TRUE);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	if(accttype==BAL_ACCT)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"[FINJEN][DEBIT CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[FINJEN][CREDIT CODE]",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][DEBIT CODE]",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][CREDIT CODE]",FALSE);
	}
	list2file(mtnrsrc,mtn,update);
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->jtypes!=NULL) freeapplib(mtn->jtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL) 
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecordseteditcb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
        void (*SubFunc)(),void *args,mtnstruct *mtn)
{
        getrecordsetedit(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,TRUE);
}
static void acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[FINJEN][FISCAL YEAR]");
	readwidget(mtnrsrc,"[FINJEN][ACCOUNT CODE]");
	check_acctcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	SETFILES(mtnrsrc,MASTER);
}
static short check_acctcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,0));
}
static void debcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[FINJEN][FISCAL YEAR]");
	readwidget(mtnrsrc,"[FINJEN][DEBIT CODE]");
	check_debcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
	SETFILES(mtnrsrc,MASTER);
}
static short check_debcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,1));
}
static void credcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[FINJEN][FISCAL YEAR]");
	readwidget(mtnrsrc,"[FINJEN][CREDIT CODE]");
	check_credcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	SETFILES(mtnrsrc,MASTER);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,2));
}
static void check_allcodes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[FINJEN][FISCAL YEAR]");
	readwidget(mtnrsrc,"[FINJEN][ACCOUNT CODE]");
	readwidget(mtnrsrc,"[FINJEN][DEBIT CODE]");
	readwidget(mtnrsrc,"[FINJEN][CREDIT CODE]");
	check_code(mtnrsrc,mtn,0);
	check_code(mtnrsrc,mtn,1);
	check_code(mtnrsrc,mtn,2);
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget)
{
	short accttype1=0,deftype1=0;
	int selaccttype=0,seldeftype=0,seljtype=0,yearid=0;
	char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL,*tmp=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	accttype1=(short)selaccttype;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype1=(short)seldeftype;
	FINDRSCGETINT(mtnrsrc,"[FINJEN][FISCAL YEAR]",&yearid);
	FINDRSCGETINT(mtnrsrc,"JOURNAL ENTRY TYPES",&seljtype);
	FINDRSCGETSTRING(mtnrsrc,"[FINJEN][ACCOUNT CODE]",&acctcode1);
	FINDRSCGETSTRING(mtnrsrc,"[FINJEN][DEBIT CODE]",&debcode1);
	FINDRSCGETSTRING(mtnrsrc,"[FINJEN][CREDIT CODE]",&credcode1);
	switch(acctwidget)
	{
		case 0: /* Account Code Test */
			switch(accttype1)
			{
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(eyrnum,eaccnum,acctcode1,accttype1,deftype1,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EXPENDITURE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(eaccnum,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						Rfree(tmp);
					}
					break;
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(ryrnum,raccnum,acctcode1,accttype1,deftype1,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(raccnum,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						Rfree(tmp);
					}
					break;
				case BAL_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,baccnum,acctcode1,accttype1,deftype1,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1); 
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						Rfree(tmp);
					}
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			}
			break;
		case 1: /* Debit Code Test */
			switch(accttype1)
			{
				case REV_ACCT :
					if(debcode1!=NULL && RDAstrlen(debcode1)>0)
					{
						if((tmp=ADVCHECKACCT(byrnum,baccnum,debcode1,accttype1,deftype1,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							Rfree(tmp);
							FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
							if(acctcode1!=NULL) Rfree(acctcode1);
							if(credcode1!=NULL) Rfree(credcode1);
							if(debcode1!=NULL) Rfree(debcode1);
							return(FALSE);
						} else {
							if(seljtype==1)
							{
								if((tmp=ADVGETBALREF(baccnum,debcode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
								{
									if(RDAstrcmp(tmp,"ESTIMATED REVENUE"))
									{
										WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ESTIMATED REVNEUE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE." ,NULL,NULL,TRUE,NULL);
										Rfree(tmp);
										if(acctcode1!=NULL) Rfree(acctcode1); 
										if(credcode1!=NULL) Rfree(credcode1);
										if(debcode1!=NULL) Rfree(debcode1); 
										FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
										return(FALSE);
									} else {
										Rfree(tmp);
										FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
										FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
										Rfree(tmp);
									}
								} else {
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ESTIMATED REVENUE BALANCE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE." ,NULL,NULL,TRUE,NULL);
									if(acctcode1!=NULL) Rfree(acctcode1); 
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1); 
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								}
							} else {
								FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
								FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
								if(tmp!=NULL) Rfree(tmp);
							}
						} /* end else */
					} else {
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
					}
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,baccnum,debcode1,accttype1,deftype1,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						if(seljtype==2)
						{
							if((tmp=ADVGETBALREF(baccnum,debcode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
							{
								if(RDAstrcmp(tmp,"ENCUMBRANCE"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ENCUMBRANCES for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of ENCUMBRANCES" ,NULL,NULL,TRUE,NULL);
									Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
									Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ENCUMBRANCES for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of ENCUMBRANCES" ,NULL,NULL,TRUE,NULL);
								if(acctcode1!=NULL) Rfree(acctcode1);
								if(credcode1!=NULL) Rfree(credcode1);
								if(debcode1!=NULL) Rfree(debcode1);
								FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
								return(FALSE);
							}
						} else if(seljtype==1)
						{
							if((tmp=ADVGETBALREF(baccnum,debcode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
							{
								if(RDAstrcmp(tmp,"UNRESERVED FUND BALANCE"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of APPROPRIATIONS" ,NULL,NULL,TRUE,NULL);
									Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1); 
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
									Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ENCUMBRANCES for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of ENCUMBRANCES" ,NULL,NULL,TRUE,NULL);
								if(acctcode1!=NULL) Rfree(acctcode1);
								if(credcode1!=NULL) Rfree(credcode1);
								if(debcode1!=NULL) Rfree(debcode1);
								FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
								return(FALSE);
							}
						} else {
							if((tmp=ADVGETBALREF(baccnum,debcode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
							{
								if(RDAstrcmp(tmp,"EXPENDITURE"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of NORMAL(EXPENDITURE)" ,NULL,NULL,TRUE,NULL);
									Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1); 
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
									Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of NORMAL(EXPENDITURE)" ,NULL,NULL,TRUE,NULL);
								if(acctcode1!=NULL) Rfree(acctcode1); 
								if(credcode1!=NULL) Rfree(credcode1);
								if(debcode1!=NULL) Rfree(debcode1);
								FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
								return(FALSE);
							}
						}
					}
					break;
				case BAL_ACCT:
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		case 2: /* Credit Code Test */
			switch(accttype1)
			{
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,baccnum,credcode1,accttype1,deftype1,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						if(seljtype==1)
						{
							if((tmp=ADVGETBALREF(baccnum,credcode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
							{
								if(RDAstrcmp(tmp,"UNRESERVED FUND BALANCE"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE" ,NULL,NULL,TRUE,NULL);
									Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
									Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE" ,NULL,NULL,TRUE,NULL);
								if(acctcode1!=NULL) Rfree(acctcode1);
								if(credcode1!=NULL) Rfree(credcode1);
								if(debcode1!=NULL) Rfree(debcode1);
								FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
								return(FALSE);
							}
						} else {
							if((tmp=ADVGETBALREF(baccnum,credcode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
							{
								if(RDAstrcmp(tmp,"REVENUE"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of REVENUE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of NORMAL(REVENUE)" ,NULL,NULL,TRUE,NULL);
									Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
									Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of REVENUE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of NORMAL(REVENUE)" ,NULL,NULL,TRUE,NULL);
								if(acctcode1!=NULL) Rfree(acctcode1);
								if(credcode1!=NULL) Rfree(credcode1);
								if(debcode1!=NULL) Rfree(debcode1);
								FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
								return(FALSE);
							}
						}
					}
					break;
				case EXP_ACCT:
					if(credcode1!=NULL && RDAstrlen(credcode1)>0)
					{
						if((tmp=ADVCHECKACCT(byrnum,baccnum,credcode1,accttype1,deftype1,yearid,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							Rfree(tmp);
							if(acctcode1!=NULL) Rfree(acctcode1);
							if(credcode1!=NULL) Rfree(credcode1);
							if(debcode1!=NULL) Rfree(debcode1);
							FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
							return(FALSE);
						} else {
							if(seljtype==1)
							{
								if((tmp=ADVGETBALREF(baccnum,credcode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
								{
									if(RDAstrcmp(tmp,"APPROPRIATION"))
									{
										WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of APPROPRIATION for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of APPROPRIATION." ,NULL,NULL,TRUE,NULL);
										Rfree(tmp);
										if(acctcode1!=NULL) Rfree(acctcode1); 
										if(credcode1!=NULL) Rfree(credcode1);
										if(debcode1!=NULL) Rfree(debcode1);
										FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
										return(FALSE);
									} else {
										FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
										FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
										Rfree(tmp);
									}
								} else {
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of APPROPRIATION for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of APPROPRIATION." ,NULL,NULL,TRUE,NULL);
									if(acctcode1!=NULL) Rfree(acctcode1); 
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
									return(FALSE);
								}
							} else {
								FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
								FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
								if(tmp!=NULL) Rfree(tmp);
							}
						}
					} else {
						FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
					}
					break;
				case BAL_ACCT:
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		default:
			prterr("Invalid \"acctwidget\" type [%d]",acctwidget);
			break;
	}
	if(tmp!=NULL) Rfree(tmp); 
	if(acctcode1!=NULL) Rfree(acctcode1); 
	if(credcode1!=NULL) Rfree(credcode1); 
	if(debcode1!=NULL) Rfree(debcode1); 
	return(TRUE);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0;

	readallwidgets(mtnrsrc);
        if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	changeDEFtype(mtnrsrc,mtn,selecteda,selectedd,TRUE,FALSE);
	SETFILES(mtnrsrc,MASTER);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,int selecteda,
	int selectedd,short update_list,short restore_accounts)
{
	short deftype1=0,accttype1=0;
	short deftype2=0,accttype2=0;

        FINDRSCGETACCTTYPE(mtnrsrc,"[FINJEN][ACCOUNT CODE]",&accttype1);
        FINDRSCGETDEFTYPE(mtnrsrc,"[FINJEN][ACCOUNT CODE]",&deftype1);
        accttype2=(short)selecteda;
        deftype2=(short)selectedd;
        if(accttype1!=accttype2 || deftype1!=deftype2)
        {
		FINDRSCSETSENSITIVE(mtnrsrc,"[FINJEN][DEBIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[FINJEN][CREDIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][DEBIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][CREDIT CODE]",TRUE);
		if(!restore_accounts)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINJEN][ACCOUNT CODE]",NULL);
			FINDRSCSETSTRING(mtnrsrc,"[FINJEN][DEBIT CODE]",NULL);
			FINDRSCSETSTRING(mtnrsrc,"[FINJEN][CREDIT CODE]",NULL);
		}
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETACCTTYPE(mtnrsrc,"[FINJEN][ACCOUNT CODE]",accttype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINJEN][ACCOUNT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINJEN][DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINJEN][CREDIT CODE]",deftype2);
		if(selecteda==BAL_ACCT)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"[FINJEN][DEBIT CODE]",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"[FINJEN][CREDIT CODE]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][DEBIT CODE]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][CREDIT CODE]",FALSE);
		}
	}
	if(update_list)
	{
		updatersrc(mtnrsrc,"[FINJEN][ACCOUNT CODE]");
		updatersrc(mtnrsrc,"[FINJEN][DEBIT CODE]");
		updatersrc(mtnrsrc,"[FINJEN][CREDIT CODE]");
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	}
}
static void ACCTtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0,seljtype=0;
	short accttype=0,deftype=0;

	readallwidgets(mtnrsrc);
        if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
        accttype=(short)selecteda;
        FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	deftype=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
        if(FINDRSCGETINT(mtnrsrc,"JOURNAL ENTRY TYPES",&seljtype)) return;
	changeACCTtype(mtnrsrc,mtn,selecteda);
	if(selectedd>=mtn->DEFtypes->numlibs) selectedd=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,selecteda,selectedd,TRUE,FALSE);
	if(seljtype>=mtn->jtypes->numlibs) seljtype=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"JOURNAL ENTRY TYPES",seljtype,mtn->jtypes))
	{
		updatersrc(mtnrsrc,"JOURNAL ENTRY TYPES");
	}
	SETFILES(mtnrsrc,MASTER);
}
static void setACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,int selaccttype,
	int seldeftype,int seljtype,short update_list)
{

	if(update_list) updatersrc(mtnrsrc,"ACCOUNT TYPES");
	changeACCTtype(mtnrsrc,mtn,selaccttype);
	if(seldeftype>=mtn->DEFtypes->numlibs) seldeftype=0;
        if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",seldeftype,mtn->DEFtypes))
        {
                if(update_list) updatersrc(mtnrsrc,"DEFINITION TYPES");
        }
	if(seljtype>=mtn->jtypes->numlibs) seljtype=0;
        if(!FINDRSCLISTAPPlib(mtnrsrc,"JOURNAL ENTRY TYPES",seljtype,mtn->jtypes))
        {
                if(update_list) updatersrc(mtnrsrc,"JOURNAL ENTRY TYPES");
        }
	changeDEFtype(mtnrsrc,mtn,selaccttype,seldeftype,update_list,TRUE);
}
static void changeACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,int selaccttype)
{
	int x;
	RDAacct *acct=NULL;

	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
	if(mtn->jtypes!=NULL) freeapplib(mtn->jtypes);
	mtn->jtypes=APPlibNEW();
	switch(selaccttype)
	{
		case EXP_ACCT:		
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=EXPENDITURE_ACCOUNT->codetype;
					x<EXPENDITURE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			addAPPlib(mtn->jtypes,"Normal");
			addAPPlib(mtn->jtypes,"Appropriation");
			addAPPlib(mtn->jtypes,"Encumbrance");
			break;
		case REV_ACCT:	
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=REVENUE_ACCOUNT->codetype;
				x<REVENUE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			addAPPlib(mtn->jtypes,"Normal");
			addAPPlib(mtn->jtypes,"Estimated Revenue");
			break;
		case BAL_ACCT:
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
					x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			addAPPlib(mtn->jtypes,"Debit");
			addAPPlib(mtn->jtypes,"Credit");
			break;
		default:
			prterr("Invalid Account Type [%d]",selaccttype);
			break;
	}
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
}
static short testavailbalance(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0,seljtype=0,yearid=0;
	short deftype=0,testavl=TRUE,p_month=0;
	char *accountcode=NULL,*header=NULL,*message=NULL;
	double amount=0.0;

	if(FINDRSCGETSTRING(mtnrsrc,"[FINJEN][ACCOUNT CODE]",&accountcode)) return(FALSE);
	if(accountcode==NULL) return(FALSE);
	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return(FALSE);
	if(selecteda==EXP_ACCT)
	{
		if(FINDRSCGETINT(mtnrsrc,"JOURNAL ENTRY TYPES",&seljtype)) return(FALSE);
		if(seljtype!=1)
		{
			if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return(FALSE);
			FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
			if(FINDRSCGETDOUBLE(mtnrsrc,"[FINJEN][AMOUNT]",&amount)) return(FALSE);
			if(FINDRSCGETINT(mtnrsrc,"[FINJEN][FISCAL YEAR]",&yearid)) return(FALSE);
			ZERNRD(eyrnum);
			FINDFLDSETINT(eyrnum,"FISCAL YEAR",yearid);
			deftype=(short)selectedd;
			FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",deftype);
			FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",accountcode);
			if(!ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				if(finsetup->budget==ANNUALBUDGET)
				{
					testavl=testapprotot(eyrnum,amount,NULL);
				} else {
					testavl=testapprotot(eyrnum,amount,FISCALPERIOD[p_month]);
				}
				if(testavl==FALSE)
				{
					header=stralloc("This Journal Entry will cause the POSTED APPROPRIATED amount to be less than all of the POSTED, DISTRIBUTED, and UNDISTRIBUTED EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT [%s] in the YEAR [%d].");
					message=Rrealloc(message,RDAstrlen(header)+RDAstrlen(accountcode)+6);
					sprintf(message,header,accountcode,yearid);
					message[RDAstrlen(header)+RDAstrlen(accountcode)+6-1]=0;
					ADVWARNINGDIALOG("WARNING DIALOG SCREEN","AVAILABLE BALANCE NOT ENOUGH!",message,save_record_up,NULL,FALSE,2,mtnrsrc,mtn,NULL);
					if(header!=NULL) Rfree(header);
					if(message!=NULL) Rfree(message);
					if(accountcode!=NULL) Rfree(accountcode);
					return(FALSE);
				} else {
					return(TRUE);
				}
			} else {
				/* no account code for that year */
				return(FALSE);
			}
		} else {
			return(TRUE);
		}
	}
	if(accountcode!=NULL) Rfree(accountcode);
	return(TRUE);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn,TRUE);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selecteda=0,selectedd=0,selectedj=0;
	short deftype=0,accttype=0,jtype=0;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda);
	accttype=(short)selecteda;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd);
	deftype=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
	FINDRSCGETINT(mtnrsrc,"JOURNAL ENTRY TYPES",&selectedj);
	jtype=(short)selectedj;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"JOURNAL ENTRY TYPE",jtype);
	setACCTtype(mtnrsrc,mtn,selecteda,selectedd,selectedj,update);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *desc=NULL,delflag=FALSE;

	readscreen(mtnrsrc,mtn);
	FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(check_acctcode(mtnrsrc,mtn)==TRUE) 
		{
			if(check_debcode(mtnrsrc,mtn)==TRUE)
			{
				if(check_credcode(mtnrsrc,mtn)==FALSE) return;
			} else return;
		} else return;
		if(testavailbalance(mtnrsrc,mtn)==FALSE) return;
	}
	if(!SAVE_CHECK(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MASTER->save_expression))
	{
		if(EVALUATEbol(MASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
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
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"SAVE WARNING",desc,
				(update_list?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	short numflds=0,y=0,jtype=0,p_month=0;
	char *rcddesc=NULL;
	char *credcode1=NULL,*debcode1=NULL,*type=NULL;
	short deftype=0,accttype=0;
	char delflag=FALSE,dist=FALSE;
	int year=0;
	double amount=0.0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	if(accttype==BAL_ACCT)
	{
		if(debcode1!=NULL)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINJEN][DEBIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"DEBIT CODE",NULL);
			updatersrc(mtnrsrc,"[FINJEN][DEBIT CODE]");
			Rfree(debcode1);
		}
		if(credcode1!=NULL)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINJEN][CREDIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"CREDIT CODE",NULL);
			updatersrc(mtnrsrc,"[FINJEN][CREDIT CODE]");
			Rfree(credcode1);
		}
	}
	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE);
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.",rcddesc);
		}
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
			if(!ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
				if(dist==FALSE && delflag==FALSE)
				{
					FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
					FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
					FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&year);
					FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
					FINDFLDGETSHORT(MASTER->passkey->fileno,"JOURNAL ENTRY TYPE",&jtype);
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
					FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&debcode1);
					FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&credcode1);
					switch(accttype)
					{
						case EXP_ACCT:
							switch(jtype)
							{
								case(0) :
									type=stralloc("EXPENDITURES");
									break;
								case(1) :
									type=stralloc("APPROPRIATIONS");
									break;
								case(2) :
									type=stralloc("ENCUMBRANCES");
									break;
								default :
									prterr("Invalid Journal Entry Type [%d]",jtype);
									break;
							}
							if(type!=NULL)
							{
								if(credcode1!=NULL && RDAstrlen(credcode1)>0)
								{
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
								} else {
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE",NULL,NULL,eyrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
								}
							}
							break;
						case REV_ACCT:
							switch(jtype)
							{
								case(0) :
									type=stralloc("REVENUE");
									break;
								case(1) :
									type=stralloc("ESTIMATED REVENUE");
									break;
								default :
									prterr("Invalid Journal Entry Type [%d]",jtype);
									break;
							}
							if(type!=NULL)
							{
								if(debcode1!=NULL && RDAstrlen(debcode1)>0)
								{
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
								} else {
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,"CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
								}
							}
							break;
						case BAL_ACCT:
							switch(jtype)
							{
								case(0) :
									if(amount<0.0)
									{
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
									} else {
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"DEBITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
									}
									break;
								case(1) :
									if(amount<0.0)
									{
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
									} else {
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"CREDITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
									}
									break;
								default :
									prterr("Invalid Journal Entry Type [%d]",jtype);
									break;
							}
							break;
						default:
							prterr("Invalid Account Type [%d]",accttype);
							break;
					} /* end switch/case */
					if(type!=NULL) Rfree(type);
				}
			} /* end if(!ADVEQLNRDsec) */
			UNLNRDFILE(MASTER->passkey->fileno);
			fields2file(MASTER->passkey->fileno,tmpfld);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
			WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
			if(dist==FALSE && delflag==FALSE)
			{
				FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"JOURNAL ENTRY TYPE",&jtype);
				FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&year);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
				FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
				FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&debcode1);
				FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&credcode1);
				switch(accttype)
				{
					case EXP_ACCT:
						switch(jtype)
						{
							case 0:
								type=stralloc("EXPENDITURES");
								break;
							case 1:
								type=stralloc("APPROPRIATIONS");
								break;
							case 2:
								type=stralloc("ENCUMBRANCES");
								break;
							default:
								prterr("Invalid Journal Entry Type [%d]",jtype);
								break;
						}
						if(type!=NULL)
						{
							if(credcode1!=NULL && RDAstrlen(credcode1)>0)	
							{
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE",NULL,NULL,eyrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							}
						}
						break;
					case REV_ACCT:
						switch(jtype)
						{
							case 0:
								type=stralloc("REVENUE");
								break;
							case 1:
								type=stralloc("ESTIMATED REVENUE");
								break;
							default:
								prterr("Invalid Journal Entry Type [%d]",jtype);
								break;
						}
						if(type!=NULL)
						{
							if(debcode1!=NULL && RDAstrlen(debcode1)>0)
							{
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,"CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,type,FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							}
						}
						break;
					case BAL_ACCT:
						switch(jtype)
						{
							case(0) :
								if(amount<0.0) 
								{
									amount=(-1)*amount;
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
									FINDRSCSETDOUBLE(mtnrsrc,"[FINJEN][AMOUNT]",amount);
									updatersrc(mtnrsrc,"[FINJEN][AMOUNT]");
									jtype+=1;
									FINDRSCSETINT(mtnrsrc,"JOURNAL ENTRY TYPES",jtype);
									updatersrc(mtnrsrc,"JOURNAL ENTRY TYPE");
								} else {
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								}
								break;
							case 1:
								if(amount<0.0) 
								{
									amount=(-1)*amount;
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
									FINDRSCSETDOUBLE(mtnrsrc,"[FINJEN][AMOUNT]",amount);
									updatersrc(mtnrsrc,"[FINJEN][AMOUNT]");
									jtype-=1;
									FINDRSCSETINT(mtnrsrc,"JOURNAL ENTRY TYPES",jtype);
									updatersrc(mtnrsrc,"JOURNAL ENTRY TYPE");
								} else {
									ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								}
								break;
							default:
								prterr("Invalid Journal Entry Type [%d]",jtype);
								break;
						} /* end switch/case */
						break;
					default:
						prterr("Invalid Account Type [%d]",accttype); 
				} /* end switch */
				if(type!=NULL) Rfree(type);
			}
			DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
			updatebrowse(update_list,mbl_finjen,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		}
		if(debcode1!=NULL) Rfree(debcode1);
		if(credcode1!=NULL) Rfree(credcode1);
		if(mbl_finjen!=NULL)
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn,TRUE);
			GetRDATData(MASTER->passkey->fileno,mtn->previous);
		}
	} 
}
static void getrecordsetedit(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int setacct=0,setdef=0,setj=0;
	short x=0,accttype=0;

	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	setacct=(int)accttype;
	FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",setacct);
	updatersrc(mtnrsrc,"ACCOUNT TYPES");
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&x);
	setdef=(int)x;
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",setdef);
	updatersrc(mtnrsrc,"DEFINITION TYPES");
	FINDFLDGETSHORT(MASTER->passkey->fileno,"JOURNAL ENTRY TYPE",&x);
	setj=(int)x;
	FINDRSCSETINT(mtnrsrc,"JOURNAL ENTRY TYPES",setj);
	setACCTtype(mtnrsrc,mtn,setacct,setdef,setj,update);
	seteditable(mtnrsrc,mtn,update);
	check_allcodes(mtnrsrc,mtn);
	if(update)
	{
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	}
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char source_user_verified=FALSE;
	short accttype=0;

	FINDRSCGETCHAR(mtnrsrc,"[FINJEN][SOURCE USER VERIFIED]",&source_user_verified);
        if(source_user_verified)
        {
		if(update)
		{
                	updateeditfilersrcsnokeys(MASTER->passkey->fileno,
				MASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,
				mtnrsrc,FALSE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,FALSE,FALSE);
                FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",FALSE);
                FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
                FINDRSCSETEDITABLE(mtnrsrc,"JOURNAL ENTRY TYPES",FALSE);
        	FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][DELETEFLAG]",TRUE);
        } else {
		FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
		if(update)
		{
                	updateeditfilersrcsnokeys(MASTER->passkey->fileno,
				MASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,
				mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
                if(accttype==BAL_ACCT)
                {
                        FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][DEBIT CODE]",FALSE);
                        FINDRSCSETEDITABLE(mtnrsrc,"[FINJEN][CREDIT CODE]",FALSE);
                }
                FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",TRUE);
                FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
                FINDRSCSETEDITABLE(mtnrsrc,"JOURNAL ENTRY TYPES",TRUE);
        }
}
static void getsupportingrecords(RDArsrc *mtnrsrc,short update)
{
	short accttype=0,x,tmpnum[3]={(-1),(-1),(-1)};

	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	switch(accttype)
	{
		case EXP_ACCT:
			tmpnum[0]=eaccnum;
			tmpnum[1]=raccnum;
			tmpnum[2]=baccnum;
			break;
		case REV_ACCT:
			tmpnum[0]=raccnum;
			tmpnum[1]=eaccnum;
			tmpnum[2]=baccnum;
			break;
		case BAL_ACCT:
			tmpnum[0]=baccnum;
			tmpnum[1]=eaccnum;
			tmpnum[2]=raccnum;
			break;
		default:
			prterr("Invalid Account Type [%d]",accttype);
			break;
	}
	if(*tmpnum!=(-1))
	{
		COPYFIELD(MASTER->passkey->fileno,*tmpnum,"ACCOUNT CODE");
		COPYFIELD(MASTER->passkey->fileno,*tmpnum,"DEFINITION TYPE");
		if(ADVEQLNRDsec(*tmpnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(*tmpnum,1);
	}
	for(x=0;x<3;++x)
	{
		if(tmpnum[x]!=(-1))
		{
			if(x>0) ZERNRD(tmpnum[x]);
			if(update) updatefilerecord2rsrc(tmpnum[x],mtnrsrc);
			else filerecord2rsrc(tmpnum[x],mtnrsrc);
		}
	}
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	readallwidgets(mtnrsrc);
	IncVirRsrcstoFile(mtnrsrc);
	getsupportingrecords(mtnrsrc,TRUE);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
	MSTADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc,MASTER);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,set_appdefaultscb,mtn,
		NULL,NULL,getrecordseteditcb,mtn);
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
		NULL,NULL,getrecordseteditcb,mtn);
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
		NULL,NULL,getrecordseteditcb,mtn);
}
