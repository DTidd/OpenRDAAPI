/* finjnlm.c - Financial Journal Maintain Screen */
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
	APPlib *DEFtypes;
	APPlib *ACCTtypes;
	APPlib *JEtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short baccnum,eaccnum,raccnum;
extern MakeBrowseList *mbl_finjnl;
extern MaintainMaster *MASTER;
extern void browse_finjnl(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *,short,short);
static void changeACCTtype(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void setACCTtype(RDArsrc *,mtnstruct *,short);
static void setLISTS(RDArsrc *,mtnstruct *,short);
static void setLISTScb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);
static void list2filecb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void getsupportingrecords(RDArsrc *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void SETID(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
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
static void check_allcodes(RDArsrc *,mtnstruct *,short);
static short check_code(RDArsrc *,mtnstruct *,short);

void finjnlm(short,void *);
char editable=TRUE;

void finjnlm(short dowhich,void *targetkey)
{
	int which_type=0;
	char *name=NULL;
	short nofields=0,nokeys=0,x,y,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->ACCTtypes=NULL;
	mtn->JEtypes=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->DEFtypes=APPlibNEW();
	addAPPlib(mtn->DEFtypes,"No Definitions Available");
	mtn->ACCTtypes=APPlibNEW();
	mtn->JEtypes=APPlibNEW();
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
			if(!RDAstrcmp(fldx->name,"DEFAULT ACCOUNT TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"ACCOUNT TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						ACCTtypecb,mtn->ACCTtypes->numlibs,
						&mtn->ACCTtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT DEFINITION TYPE"))	
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFAULT JOURNAL ENTRY TYPE"))	
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"JOURNAL ENTRY TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						NULL,mtn->JEtypes->numlibs,
						&mtn->JEtypes->libs,NULL);
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
	FINDRSCSETFUNC(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",credcodecb,mtn);
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
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setLISTS(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setLISTS(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setLISTS(mtnrsrc,mtn,FALSE);
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
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setLISTS(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setLISTS(mtnrsrc,mtn,FALSE);
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
	if(mbl_finjnl!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_finjnl,mtn);
	if(mbl_finjnl!=NULL)
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
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_finjnl==NULL?TRUE:FALSE));
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn,TRUE);
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
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	int selected=0;
	short type=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selected)) return;
	if(selected>=mtn->ACCTtypes->numlibs) selected=0;
	if(RDAstrcmp(mtn->ACCTtypes->libs[selected],"No Account Types Available") &&
		RDAstrcmp(mtn->ACCTtypes->libs[selected],"Invalid-Account Type"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT ACCOUNT TYPE",type);
	}
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected)) return;
	if(selected>=mtn->DEFtypes->numlibs) selected=0;
	if(RDAstrcmp(mtn->DEFtypes->libs[selected],"No Definitions Available") &&
		RDAstrcmp(mtn->DEFtypes->libs[selected],"Invalid-Definition Type"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",type);
	}
	if(FINDRSCGETINT(mtnrsrc,"JOURNAL ENTRY TYPES",&selected)) return;
	if(selected>=mtn->JEtypes->numlibs) selected=0;
	if(RDAstrcmp(mtn->JEtypes->libs[selected],"No Journal Entry Types Available") &&
		RDAstrcmp(mtn->JEtypes->libs[selected],"Invalid-Journal Entry Type"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT JOURNAL ENTRY TYPE",type);
	}
	setACCTtype(mtnrsrc,mtn,update_list);
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
	char deleteflag=0,*warnmessage=NULL,*desc=NULL;
	int selACCTtype=0,selDEFtype=0,selJEtype=0;

	readscreen(mtnrsrc,mtn);
	FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selACCTtype);
		if(!RDAstrcmp(mtn->ACCTtypes->libs[selACCTtype],"No Account Types Available") ||
			!RDAstrcmp(mtn->ACCTtypes->libs[selACCTtype],"Invalid-Account Type"))
		{
			warnmessage=Rmalloc(201);
			sprintf(warnmessage,"The ACCOUNT TYPE selected is invalid.  This must be corrected before this record can be saved.");
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ACCOUNT TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selDEFtype);
			if(!RDAstrcmp(mtn->DEFtypes->libs[selDEFtype],"No Definitions Available") ||
				!RDAstrcmp(mtn->DEFtypes->libs[selDEFtype],"Invalid-Definition Type"))
			{
				warnmessage=Rmalloc(201);
				sprintf(warnmessage,"The DEFINITION TYPE selected is invalid.  This must be corrected before this record can be saved.");
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDRSCGETINT(mtnrsrc,"JOURNAL ENTRY TYPES",&selJEtype);
				if(!RDAstrcmp(mtn->JEtypes->libs[selJEtype],"No Journal Entry Types Available") ||
					!RDAstrcmp(mtn->JEtypes->libs[selJEtype],"Invalid-Journal Entry Type"))
				{
					warnmessage=Rmalloc(201);
					sprintf(warnmessage,"The Journal Entry Type Selected is not valid.  Cannot save this record.");
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOURNAL ENTRY TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(check_acctcode(mtnrsrc,mtn)==TRUE)
		{
			if(check_debcode(mtnrsrc,mtn)==TRUE)
			{
				if(check_credcode(mtnrsrc,mtn)==FALSE) return;
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
	char *rcddesc=NULL,*temp=NULL;
	int selaccttype=0;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	if(selaccttype==BAL_ACCT)
	{
		FINDRSCGETSTRING(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",&temp);
		if(temp!=NULL)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"DEFAULT DEBIT CODE",NULL);
			updatersrc(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]");
			Rfree(temp);
		}
		FINDRSCGETSTRING(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",&temp);
		if(temp!=NULL)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"DEFAULT CREDIT CODE",NULL);
			updatersrc(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]");
			Rfree(temp);
		}
	}
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
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_finjnl,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_finjnl!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MASTER->passkey->fileno,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->ACCTtypes!=NULL)  freeapplib(mtn->ACCTtypes);
		if(mtn->JEtypes!=NULL)  freeapplib(mtn->JEtypes);
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
static void acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]");
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
	readwidget(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]");
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
	readwidget(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]");
	check_credcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	SETFILES(mtnrsrc,MASTER);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,2));
}
static void check_allcodes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	if(update)
	{
		readwidget(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]");
		readwidget(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]");
		readwidget(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]");
	}
	check_code(mtnrsrc,mtn,0);
	check_code(mtnrsrc,mtn,1);
	check_code(mtnrsrc,mtn,2);
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget)
{
	short accttype1=0,deftype1=0;
	int selaccttype=0,seldeftype=0,seljtype=0;
	char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL,*tmp=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	accttype1=(short)selaccttype;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype1=(short)seldeftype;
	FINDRSCGETINT(mtnrsrc,"JOURNAL ENTRY TYPES",&seljtype);
	FINDRSCGETSTRING(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]",&acctcode1);
	FINDRSCGETSTRING(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",&debcode1);
	FINDRSCGETSTRING(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",&credcode1);
	switch(acctwidget)
	{
		case 0: /* Account Code Test */
			if(isEMPTY(acctcode1))
			{
				FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
				if(acctcode1!=NULL) Rfree(acctcode1); 
				if(credcode1!=NULL) Rfree(credcode1); 
				if(debcode1!=NULL) Rfree(debcode1); 
				return(TRUE);
			}
			switch(accttype1)
			{
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT((-1),eaccnum,acctcode1,accttype1,deftype1,0,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EXPENDITURE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						if(tmp!=NULL) Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(eaccnum,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				case REV_ACCT:
					if((tmp=ADVCHECKACCT((-1),raccnum,acctcode1,accttype1,deftype1,0,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						if(tmp!=NULL) Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(raccnum,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				case BAL_ACCT:
					if((tmp=ADVCHECKACCT((-1),baccnum,acctcode1,accttype1,deftype1,0,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						if(tmp!=NULL) Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1); 
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			}
			break;
		case 1: /* Debit Code Test */
			if(isEMPTY(debcode1))
			{
				FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
				if(acctcode1!=NULL) Rfree(acctcode1); 
				if(credcode1!=NULL) Rfree(credcode1); 
				if(debcode1!=NULL) Rfree(debcode1); 
				return(TRUE);
			}
			switch(accttype1)
			{
				case REV_ACCT :
					if((tmp=ADVCHECKACCT((-1),baccnum,debcode1,accttype1,deftype1,0,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						if(tmp!=NULL) Rfree(tmp);
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
								if(RDAstrcmp(tmp,"UNRESERVED FUND BALANCE"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE." ,NULL,NULL,TRUE,NULL);
									if(tmp!=NULL) Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1); 
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1); 
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
									if(tmp!=NULL) Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE." ,NULL,NULL,TRUE,NULL);
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
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT((-1),baccnum,debcode1,accttype1,deftype1,0,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						if(tmp!=NULL) Rfree(tmp);
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
									if(tmp!=NULL) Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
									if(tmp!=NULL) Rfree(tmp);
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
								if(RDAstrcmp(tmp,"APPROPRIATION"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of APPROPRIATIONS for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of APPROPRIATIONS" ,NULL,NULL,TRUE,NULL);
									if(tmp!=NULL) Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1); 
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
									if(tmp!=NULL) Rfree(tmp);
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
									if(tmp!=NULL) Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1); 
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
									if(tmp!=NULL) Rfree(tmp);
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
			if(isEMPTY(credcode1))
			{
				FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
				if(acctcode1!=NULL) Rfree(acctcode1); 
				if(credcode1!=NULL) Rfree(credcode1); 
				if(debcode1!=NULL) Rfree(debcode1); 
				return(TRUE);
			}
			switch(accttype1)
			{
				case REV_ACCT:
					if((tmp=ADVCHECKACCT((-1),baccnum,credcode1,accttype1,deftype1,0,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						if(tmp!=NULL) Rfree(tmp);
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
								if(RDAstrcmp(tmp,"ESTIMATED REVENUE"))
								{
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ESTIMATED REVENUE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE" ,NULL,NULL,TRUE,NULL);
									if(tmp!=NULL) Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
									if(tmp!=NULL) Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of ESTIMATED REVENUE for ACCOUNT TYPE of REVENUE, JOURNAL ENTRY TYPE of ESTIMATED REVENUE" ,NULL,NULL,TRUE,NULL);
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
									if(tmp!=NULL) Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1);
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									if(tmp!=NULL) Rfree(tmp);
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
									if(tmp!=NULL) Rfree(tmp);
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
					if((tmp=ADVCHECKACCT((-1),baccnum,credcode1,accttype1,deftype1,0,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						if(tmp!=NULL) Rfree(tmp);
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
									WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of APPROPRIATION." ,NULL,NULL,TRUE,NULL);
									if(tmp!=NULL) Rfree(tmp);
									if(acctcode1!=NULL) Rfree(acctcode1); 
									if(credcode1!=NULL) Rfree(credcode1);
									if(debcode1!=NULL) Rfree(debcode1);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
									return(FALSE);
								} else {
									FINDFLDGETSTRING(baccnum,"DESCRIPTION",&tmp);
									FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
									if(tmp!=NULL) Rfree(tmp);
								}
							} else {
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of UNRESERVED FUND BALANCE for ACCOUNT TYPE of EXPENDITURE, JOURNAL ENTRY TYPE of APPROPRIATION." ,NULL,NULL,TRUE,NULL);
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
static void list2filecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
        list2file(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,TRUE);
}
static void setLISTScb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
        setLISTS(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,TRUE);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	changeDEFtype(mtnrsrc,mtn,TRUE,FALSE);
	SETFILES(mtnrsrc,MASTER);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list,
	short restore_accounts)
{
	int selecteda=0,selectedd=0;
	short deftype1=0,accttype1=0;
	short deftype2=0,accttype2=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETACCTTYPE(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]",&accttype1);
	FINDRSCGETDEFTYPE(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]",&deftype1);
	accttype2=(short)selecteda;
	deftype2=(short)selectedd;
	if(accttype1!=accttype2 || deftype1!=deftype2)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",TRUE);
		if(!restore_accounts)
		{
			FINDRSCSETSTRING(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]",NULL);
			FINDRSCSETSTRING(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",NULL);
			FINDRSCSETSTRING(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",NULL);
		}
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETACCTTYPE(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]",accttype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",deftype2);
		if(selecteda==BAL_ACCT)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]",FALSE);
		}
	}
	if(update_list)
	{
		updatersrc(mtnrsrc,"[FINJRNL][DEFAULT ACCOUNT CODE]");
		updatersrc(mtnrsrc,"[FINJRNL][DEFAULT DEBIT CODE]");
		updatersrc(mtnrsrc,"[FINJRNL][DEFAULT CREDIT CODE]");
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	}
}
static void setLISTS(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	setACCTtype(mtnrsrc,mtn,update_list);
	check_allcodes(mtnrsrc,mtn,update_list);	
	if(update_list)
	{
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	}
}
static void setACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	int selecteda=0,x=0;
	short accttype=0,deftype1=0,jetype1=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFAULT ACCOUNT TYPE",&accttype);
	selecteda=(int)accttype;
	if(!FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",selecteda))
	{
		if(update_list) updatersrc(mtnrsrc,"ACCOUNT TYPES");
	} else return;
	changeACCTtype(mtnrsrc,mtn);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",&deftype1);
	x=(int)deftype1;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",x,mtn->DEFtypes))
	{
		if(update_list) updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFAULT JOURNAL ENTRY TYPE",&jetype1);
	x=(int)jetype1;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"JOURNAL ENTRY TYPES",x,mtn->JEtypes))
	{
		if(update_list) updatersrc(mtnrsrc,"JOURNAL ENTRY TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,update_list,TRUE);
}
static void ACCTtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0;
	short accttype=0,deftype=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	accttype=(short)selecteda;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT ACCOUNT TYPE",accttype);
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	deftype=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",deftype);
	changeACCTtype(mtnrsrc,mtn);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",deftype,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,TRUE,FALSE);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"JOURNAL ENTRY TYPES",0,mtn->JEtypes))
	{
		updatersrc(mtnrsrc,"JOURNAL ENTRY TYPES");
	}
	SETFILES(mtnrsrc,MASTER);
}
static void changeACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=0,selecteda=0;
	short accttype=0;
	RDAacct *acct=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda);
	accttype=(short)selecteda;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT ACCOUNT TYPE",accttype);
	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
	if(mtn->JEtypes!=NULL) freeapplib(mtn->JEtypes);
	mtn->JEtypes=APPlibNEW();
	switch(accttype)
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
			addAPPlib(mtn->JEtypes,"Normal");
			addAPPlib(mtn->JEtypes,"Appropriation");
			addAPPlib(mtn->JEtypes,"Encumbrance");
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
			addAPPlib(mtn->JEtypes,"Normal");
			addAPPlib(mtn->JEtypes,"Estimated Revenue");
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
			addAPPlib(mtn->JEtypes,"Debit");
			addAPPlib(mtn->JEtypes,"Credit");
			break;
		default:
			prterr("Invalid Account Type [%d]",accttype);
			break;
	}
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
}
static void getsupportingrecords(RDArsrc *mtnrsrc,short update)
{
	short accttype=0,x,tmpnum[3]={(-1),(-1),(-1)};

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFAULT ACCOUNT TYPE",&accttype);
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
		FIELDCOPY(MASTER->passkey->fileno,"DEFAULT ACCOUNT CODE",*tmpnum,"ACCOUNT CODE");
		FIELDCOPY(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",*tmpnum,"DEFINITION TYPE");
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
		(void *)mtnrsrc,list2filecb,mtn,
		NULL,NULL,setLISTScb,mtn);
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
		member->parent,keyno,list2filecb,mtn,
		NULL,NULL,setLISTScb,mtn);
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
		member->parent,keyno,list2filecb,mtn,
		NULL,NULL,setLISTScb,mtn);
}
