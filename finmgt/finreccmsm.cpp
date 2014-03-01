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
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short banknum;
extern short byrnum,eyrnum,ryrnum;
extern short baccnum,eaccnum,raccnum;
extern RDAfinmgt *finsetup;
extern MakeBrowseList *mbl_finrec;
extern void browse_finrec(RDArsrc *,mtnstruct *);
extern MaintainMaster *MASTER;
static void getrecordsetedit(RDArsrc *,mtnstruct *,short);
static void getrecordseteditcb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void SETID(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void set_appdefaults(RDArsrc *,mtnstruct *,short);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *,char);
static void getsupportingrecords(RDArsrc *,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *,int,int,short,short);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void changeACCTtype(RDArsrc *,mtnstruct *,int);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void setACCTtype(RDArsrc *,mtnstruct *,int,int,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static short check_acctcode(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static void acctcodecb(RDArsrc *,mtnstruct *);
static void debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void check_allcodes(RDArsrc *,mtnstruct *);
static short check_code(RDArsrc *,mtnstruct *,short);
static short testavailbalance(RDArsrc *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *,char);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);
void finrecm(short,void *);
char editable=TRUE;

void finrecm(short dowhich,void *targetkey)
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
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->DEFtypes=APPlibNEW();
	mtn->ACCTtypes=APPlibNEW();
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
	FINDRSCSETFUNC(mtnrsrc,"[FINREC][ACCOUNT CODE]",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[FINREC][CREDIT CODE]",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[FINREC][DEBIT CODE]",debcodecb,mtn);
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
	if(mbl_finrec!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_finrec,mtn);
	if(mbl_finrec!=NULL) 
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
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_finrec==NULL?TRUE:FALSE));
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

	FINDRSCSETSTRING(mtnrsrc,"[FINREC][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[FINREC][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[FINREC][ENTRY TIME]",timex);
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
	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	if(accttype==BAL_ACCT)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][DEBIT CODE]",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][CREDIT CODE]",FALSE);
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
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		if(banknum!=(-1)) CLSNRD(banknum);
		if(finsetup!=NULL) free_finmgt(finsetup);
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
	readwidget(mtnrsrc,"[FINREC][ACCOUNT CODE]");
	readwidget(mtnrsrc,"[FINREC][FISCAL YEAR]");
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
	readwidget(mtnrsrc,"[FINREC][DEBIT CODE]");
	readwidget(mtnrsrc,"[FINREC][FISCAL YEAR]");
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
	readwidget(mtnrsrc,"[FINREC][CREDIT CODE]");
	readwidget(mtnrsrc,"[FINREC][FISCAL YEAR]");
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
	readwidget(mtnrsrc,"[FINREC][FISCAL YEAR]");
	readwidget(mtnrsrc,"[FINREC][ACCOUNT CODE]");
	readwidget(mtnrsrc,"[FINREC][DEBIT CODE]");
	readwidget(mtnrsrc,"[FINREC][CREDIT CODE]");
	check_code(mtnrsrc,mtn,0);
	check_code(mtnrsrc,mtn,1);
	check_code(mtnrsrc,mtn,2);
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget)
{
	short accttype1=0,deftype1=0;
	int selaccttype=0,seldeftype=0,yearid1=0;
	char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL,*tmp=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	accttype1=(short)selaccttype;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype1=(short)seldeftype;
	FINDRSCGETINT(mtnrsrc,"[FINREC][FISCAL YEAR]",&yearid1);
	FINDRSCGETSTRING(mtnrsrc,"[FINREC][ACCOUNT CODE]",&acctcode1);
	FINDRSCGETSTRING(mtnrsrc,"[FINREC][DEBIT CODE]",&debcode1);
	FINDRSCGETSTRING(mtnrsrc,"[FINREC][CREDIT CODE]",&credcode1);
	switch(acctwidget)
	{
		case 0:
			switch(accttype1)
			{
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(ryrnum,raccnum,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(eyrnum,eaccnum,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			}
			break;
		case 1:
			switch(accttype1)
			{
				case REV_ACCT :
					if(debcode1!=NULL && RDAstrlen(debcode1)>0)
					{
						if((tmp=ADVCHECKACCT(byrnum,baccnum,debcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							Rfree(tmp);
							if(acctcode1!=NULL) Rfree(acctcode1);
							if(credcode1!=NULL) Rfree(credcode1);
							if(debcode1!=NULL) Rfree(debcode1);
							FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
							return(FALSE);
						} else {
							FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
							FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
							Rfree(tmp);
						}
					} else {
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
					}
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,baccnum,debcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
						return(FALSE);
					}
					break;
				case BAL_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,baccnum,debcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
						Rfree(tmp);
					}
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		case 2:
			switch(accttype1)
			{
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,baccnum,credcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
						return(FALSE);
					}
					break;
				case EXP_ACCT:
					if(credcode1!=NULL && RDAstrlen(credcode1)>0)
					{
						if((tmp=ADVCHECKACCT(byrnum,baccnum,credcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
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
						FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
					}
					break;
				case BAL_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,baccnum,credcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
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
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		default:
			prterr("\"acctwidget\" value of [%d] out of range.",acctwidget);
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

	FINDRSCGETACCTTYPE(mtnrsrc,"[FINREC][ACCOUNT CODE]",&accttype1);
	FINDRSCGETDEFTYPE(mtnrsrc,"[FINREC][ACCOUNT CODE]",&deftype1);
	accttype2=(short)selecteda;
	deftype2=(short)selectedd;
	if(accttype1!=accttype2 || deftype1!=deftype2)
	{
        	FINDRSCSETSENSITIVE(mtnrsrc,"[FINREC][DEBIT CODE]",TRUE);
        	FINDRSCSETSENSITIVE(mtnrsrc,"[FINREC][CREDIT CODE]",TRUE);
        	FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][DEBIT CODE]",TRUE);
        	FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][CREDIT CODE]",TRUE);
		if(!restore_accounts)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINREC][ACCOUNT CODE]",NULL);
			FINDRSCSETSTRING(mtnrsrc,"[FINREC][DEBIT CODE]",NULL);
			FINDRSCSETSTRING(mtnrsrc,"[FINREC][CREDIT CODE]",NULL);
		}
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETACCTTYPE(mtnrsrc,"[FINREC][ACCOUNT CODE]",accttype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINREC][ACCOUNT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINREC][DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINREC][CREDIT CODE]",deftype2);
		if(selecteda==BAL_ACCT)
		{
				FINDRSCSETSENSITIVE(mtnrsrc,"[FINREC][ACCOUNT CODE]",FALSE);
        			FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][ACCOUNT CODE]",FALSE);
		}
	}
	if(update_list)
	{
		updatersrc(mtnrsrc,"[FINREC][ACCOUNT CODE]");
		updatersrc(mtnrsrc,"[FINREC][DEBIT CODE]");
		updatersrc(mtnrsrc,"[FINREC][CREDIT CODE]");
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	}
}
static void ACCTtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0;
	short accttype=0,deftype=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	accttype=(short)selecteda;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	deftype=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
	changeACCTtype(mtnrsrc,mtn,selecteda);
	if(selectedd>=mtn->DEFtypes->numlibs) selectedd=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,selecteda,selectedd,TRUE,FALSE);
	SETFILES(mtnrsrc,MASTER);
}
static void setACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,int selecteda,
	int selectedd,short update_list)
{

	if(update_list) updatersrc(mtnrsrc,"ACCOUNT TYPES");
	changeACCTtype(mtnrsrc,mtn,selecteda);
	if(selectedd>=mtn->DEFtypes->numlibs) selectedd=0;	
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		if(update_list) updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,selecteda,selectedd,update_list,TRUE);
}
static void changeACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,int selaccttype)
{
	int x;
	RDAacct *acct=NULL;

	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
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
	int selecteda=0,selectedd=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda);
	type=(short)selecteda;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",type);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd);
	type=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",type);
	setACCTtype(mtnrsrc,mtn,selecteda,selectedd,update);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}  
static short testavailbalance(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0,yearid=0;
	short deftype=0,testavl=TRUE,p_month=0;
	char *acctcode=NULL,*header=NULL,*message=NULL;
	double amount=0.0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return(FALSE);
	if(selecteda==EXP_ACCT)
	{
		if(FINDRSCGETSTRING(mtnrsrc,"[FINREC][ACCOUNT CODE]",&acctcode)) return(FALSE);
		if(acctcode==NULL) return(FALSE);
		if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return(FALSE);
		FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
		if(FINDRSCGETDOUBLE(mtnrsrc,"[FINREC][AMOUNT]",&amount)) return(FALSE);
		if(FINDRSCGETINT(mtnrsrc,"[FINREC][FISCAL YEAR]",&yearid)) return(FALSE);
		ZERNRD(eyrnum);
		FINDFLDSETINT(eyrnum,"FISCAL YEAR",yearid);
		deftype=(short)selectedd;
		FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",deftype);
		FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acctcode);
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
				message=Rrealloc(message,RDAstrlen(header)+RDAstrlen(acctcode)+6);
				sprintf(message,header,acctcode,yearid);
				message[RDAstrlen(header)+RDAstrlen(acctcode)+6-1]=0;
				ADVWARNINGDIALOG("WARNING DIALOG SCREEN","AVAILABLE BALANCE NOT ENOUGH!",message,save_record_up,NULL,FALSE,2,mtnrsrc,mtn,NULL);
				if(header!=NULL) Rfree(header); 
				if(message!=NULL) Rfree(message); 
				if(acctcode!=NULL) Rfree(acctcode);
				return(FALSE);
			} else {
				return(TRUE);
			}
		} else {
			/* no account code for that year */
			return(FALSE);
		}
	}
	return(TRUE);
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
	char *bankid=NULL;
	short numflds=0,y=0,p_month=0;
	char *rcddesc=NULL,*debcode1=NULL,*credcode1=NULL;
	char *acctcode1=NULL;
	short deftype=0,accttype=0;
	char *date=NULL,*timex=NULL;
	char delflag=FALSE,dist=FALSE;
	int year=0;
	double amount=0.0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	if(accttype==BAL_ACCT)
	{
/*
		FINDRSCGETSTRING(mtnrsrc,"[FINREC][DEBIT CODE]",&debcode1);
		if(debcode1!=NULL)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINREC][DEBIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"DEBIT CODE",NULL);
			updatersrc(mtnrsrc,"[FINREC][DEBIT CODE]");
			Rfree(debcode1);
		}
		FINDRSCGETSTRING(mtnrsrc,"[FINREC][CREDIT CODE]",&credcode1);
		if(credcode1!=NULL)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINREC][CREDIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"CREDIT CODE",NULL);
			updatersrc(mtnrsrc,"[FINREC][CREDIT CODE]");
			Rfree(credcode1);
		}
*/
		FINDRSCGETSTRING(mtnrsrc,"[FINREC][ACCOUNT CODE]",&acctcode1);
		if(acctcode1!=NULL)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINREC][ACCOUNT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",NULL);
			updatersrc(mtnrsrc,"[FINREC][ACCOUNT CODE]");
			Rfree(acctcode1);
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
			prterr("Didn't write record.");
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
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
					FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&debcode1);
					FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&credcode1);
					switch(accttype)
					{
						case EXP_ACCT:
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,eyrnum,byrnum,year,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							if(credcode1!=NULL && RDAstrlen(credcode1)>0)
							{
								ADVupdateundist(MASTER->passkey->fileno,NULL,"CREDIT CODE","DEBIT CODE",NULL,eyrnum,byrnum,year,deftype,"EXPENDITURES",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,"DEBIT CODE",NULL,eyrnum,byrnum,year,deftype,"EXPENDITURES",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							}
							break;
						case REV_ACCT:
							if(debcode1!=NULL && RDAstrlen(debcode1)>0)
							{
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,"CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							}
							break;
						case BAL_ACCT:
							if(amount>0.0)
							{
								ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),byrnum,year,deftype,"CREDITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,NULL,"CREDIT CODE","DEBIT CODE",NULL,(-1),byrnum,year,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							}
							break;
						default:
							prterr("Invalid Account Type [%d]",accttype);
							break;
					} /* end switch/case */
					if(banknum!=(-1))
					{
						if(bankid!=NULL) {Rfree(bankid); bankid=NULL; }
						FINDFLDGETSTRING(MASTER->passkey->fileno,"BANK IDENTIFICATION",&bankid);
					}
				}
			} /* end if(!ADVEQLNRDsec) */
			UNLNRDFILE(MASTER->passkey->fileno);
			fields2file(MASTER->passkey->fileno,tmpfld);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
			WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
			if(dist==FALSE && delflag==FALSE)
			{
				FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
				FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&year);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
				FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
				FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&debcode1);
				FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&credcode1);
				switch(accttype)
				{
					case EXP_ACCT:
						ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,eyrnum,byrnum,year,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
						if(credcode1!=NULL && RDAstrlen(credcode1)>0)
						{
							ADVupdateundist(MASTER->passkey->fileno,NULL,"CREDIT CODE","DEBIT CODE",NULL,eyrnum,byrnum,year,deftype,"EXPENDITURES",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,"DEBIT CODE",NULL,eyrnum,byrnum,year,deftype,"EXPENDITURES",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						}
						break;
					case REV_ACCT:
						if(debcode1!=NULL && RDAstrlen(debcode1)>0)
						{
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,"CREDIT CODE",NULL,ryrnum,byrnum,year,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						}
						break;
					case BAL_ACCT:
						if(amount>0.0)
						{
							ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),byrnum,year,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,NULL,"CREDIT CODE","DEBIT CODE",NULL,(-1),byrnum,year,deftype,"CREDITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
						}
/*
						if(amount>0.0)
						{
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,byrnum,(-1),year,deftype,"CREDITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
						}
*/
						break;
					default:
						prterr("Invalid Account Type [%d]",accttype);
						break;
				} /* end switch */
				if(banknum!=(-1))
				{
					if(bankid!=NULL) Rfree(bankid);
					FINDFLDGETSTRING(MASTER->passkey->fileno,"BANK IDENTIFICATION",&bankid);
				}
			}
		} /* end if(numflds>0) */
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		if(bankid!=NULL) Rfree(bankid);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_finrec,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_finrec!=NULL)
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
	int setacct=0,setdef=0;
	short x=0;

	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&x);
	setacct=(int)x;
	FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",setacct);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&x);
	setdef=(int)x;
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",setdef);
	setACCTtype(mtnrsrc,mtn,setacct,setdef,update);
	seteditable(mtnrsrc,mtn,update);
	check_allcodes(mtnrsrc,mtn);
	if(update)
	{
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	}
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn,char update)
{
	char dep=FALSE,dist=FALSE;
	short accttype=0;

	FINDRSCGETCHAR(mtnrsrc,"[FINREC][SOURCE USER VERIFIED]",&dist);
	FINDRSCGETCHAR(mtnrsrc,"[FINREC][BANK RECONCILIATION UPDATED]",&dep);
	if(dist || dep)
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
		FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][DELETEFLAG]",TRUE);
	} else {
		if(update) 
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,
				MASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,
				mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
		FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
		if(accttype==BAL_ACCT)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][DEBIT CODE]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[FINREC][CREDIT CODE]",FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
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
