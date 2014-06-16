/* fincoam.c - Chart of Accounts Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

struct mtnstructs
{
	APPlib *ACCTtypes;
	APPlib *DEFtypes;
	APPlib *DIMension;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern MakeBrowseList *mbl_fincoa;
extern void browse_fincoa(RDArsrc *,mtnstruct *);
extern MaintainMaster *MTNMASTER;
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *,short);
static void changeACCTtype(RDArsrc *,mtnstruct *);
static void setLISTS(RDArsrc *,mtnstruct *,short);
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void list2filecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void setDEFtype(RDArsrc *,mtnstruct *,short);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void DIMcb(RDArsrc *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
void fincoam(short,void *);
char editable=TRUE;

void fincoam(short dowhich,void *targetkey)
{
	int which_type=0;
	char *name=NULL;
	short nofields=0,nokeys=0,x=0,y=0,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
	ZERNRD(MTNMASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DIMension=NULL;
	mtn->DEFtypes=NULL;
	mtn->ACCTtypes=NULL;
        mtn->previous=NULL;
        mtn->previous=RDATDataNEW(MTNMASTER->passkey->fileno);
	mtn->DIMension=APPlibNEW();
	mtn->DEFtypes=APPlibNEW();
	mtn->ACCTtypes=APPlibNEW();
	addAPPlib(mtn->ACCTtypes,accttypes[0]);
	addAPPlib(mtn->ACCTtypes,accttypes[1]);
	addAPPlib(mtn->ACCTtypes,accttypes[2]);
	nokeys=NUMKEYS(MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	nofields=NUMFLDS(MTNMASTER->passkey->fileno);
	fields=FLDPOINTER(MTNMASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"ACCOUNT TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						ACCTtypecb,mtn->ACCTtypes->numlibs,
						&mtn->ACCTtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))	
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DIMENSION"))	
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"DIMENSIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DIMcb,mtn->DIMension->numlibs,
						&mtn->DIMension->libs,mtn);
				}
			} else {
				nonlstmakefld(mtnrsrc,MTNMASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MTNMASTER);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey))
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	setLISTS(mtnrsrc,mtn,FALSE);
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MTNMASTER);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_fincoa!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addbtnrsrc(mtnrsrc,"BROWSE",TRUE,browse_fincoa,mtn);
	if(mbl_fincoa!=NULL)
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
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_fincoa==NULL?TRUE:FALSE));
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
	int selACCTtype=0,selDEFtype=0,selDIM=0;

	readscreen(mtnrsrc,mtn);
	FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
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
				FINDRSCGETINT(mtnrsrc,"DIMENSIONS",&selDIM);
				if(!RDAstrcmp(mtn->DIMension->libs[selDIM],"No Dimensions Available") ||
					!RDAstrcmp(mtn->DIMension->libs[selDIM],"Invalid-Dimension"))
				{
					warnmessage=Rmalloc(201);
					sprintf(warnmessage,"The Dimension Selected is not valid for the defined Account Code Structure.  Cannot save this record.");
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DIMENSION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
	}
	if(!SAVE_CHECK(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MTNMASTER->save_expression))
	{
		if(EVALUATEbol(MTNMASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MTNMASTER->save_warning))
	{
		if(EVALUATEbol(MTNMASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_warning_desc,
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
	char *rcddesc=NULL;

	if(ADVRECORDsec(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else { 
		WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_fincoa,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
		if(mbl_fincoa!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
	}
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,mtn->previous,readscreen,
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
		if(mtn->DIMension!=NULL) freeapplib(mtn->DIMension);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc); 
	}
	if(WindowCount<1)
	{
		CLOSE_MASTER_FILES(MTNMASTER);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void DIMcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn);
	SETID(mtnrsrc,mtn);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selected)) return;
	type=(short)selected;
	FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"ACCOUNT TYPE",type);
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected)) return;
	type=(short)selected;
	FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",type);
	if(FINDRSCGETINT(mtnrsrc,"DIMENSIONS",&selected)) return;
	type=(short)selected;
	FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"DIMENSION",type);
	changeDEFtype(mtnrsrc,mtn,TRUE);
	SETID(mtnrsrc,mtn);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short accttype=0,deftype=0,dim=0;
	int x=0;
	RDAacct *accts=NULL;
	RDAaccpart *acct_part=NULL;

	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"DIMENSION",&dim);
	if(mtn->DIMension!=NULL) freeapplib(mtn->DIMension);
	mtn->DIMension=APPlibNEW();
	switch(accttype)
	{
		case EXP_ACCT:	
			accts=EXPENDITURE_ACCOUNT->codetype+deftype;
			if(accts!=NULL)
			{
				for(x=0,acct_part=accts->acct;x<accts->num;++x,++acct_part)
				{
					addAPPlib(mtn->DIMension,acct_part->name);
				}
			}
			break;
		case REV_ACCT:
			accts=REVENUE_ACCOUNT->codetype+deftype;
			if(accts!=NULL)
			{
				for(x=0,acct_part=accts->acct;x<accts->num;++x,++acct_part)
				{
					addAPPlib(mtn->DIMension,acct_part->name);
				}
			}
			break;
		case BAL_ACCT:
			accts=BALANCE_SHEET_ACCOUNT->codetype+deftype;
			if(accts!=NULL)
			{
				for(x=0,acct_part=accts->acct;x<accts->num;++x,++acct_part)
				{
					addAPPlib(mtn->DIMension,acct_part->name);
				}
			}
			break;
		default:
			prterr("Invalid Account Type [%d]",accttype);
	}
	if(mtn->DIMension->numlibs<1)
	{
		addAPPlib(mtn->DIMension,"No Dimensions Available");
	}
	if(dim>=mtn->DIMension->numlibs) x=0;
	else x=dim;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DIMENSIONS",x,mtn->DIMension))
	{
		if(update) updatersrc(mtnrsrc,"DIMENSIONS");
	}
}
void ACCTtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0;
	short accttype=0;	

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	accttype=(short)selecteda;
	FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd))
		selectedd=0;
	changeACCTtype(mtnrsrc,mtn);
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	DEFtypecb(mtnrsrc,mtn);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selected);
	if(RDAstrcmp(mtn->ACCTtypes->libs[selected],"No Account Types Available") &&
		RDAstrcmp(mtn->ACCTtypes->libs[selected],"Invalid-Account Type"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"ACCOUNT TYPE",type);
	}
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	if(RDAstrcmp(mtn->DEFtypes->libs[selected],"No Definitions Available") && 
		RDAstrcmp(mtn->DEFtypes->libs[selected],"Invalid-Definition Type"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",type);
	}
	FINDRSCGETINT(mtnrsrc,"DIMENSIONS",&selected);
	if(RDAstrcmp(mtn->DIMension->libs[selected],"No Dimensions Available") &&
		RDAstrcmp(mtn->DIMension->libs[selected],"Invalid-Dimension"))
	{
		{
			type=(short)selected;
			FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"DIMENSION",type);
		}
	}
}
static void setLISTS(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	setDEFtype(mtnrsrc,mtn,update_list);
	changeDEFtype(mtnrsrc,mtn,update_list);
}
static void setDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	int selecteda=0,x=0,listlen=0;
	short accttype=0,deftype1=0;	

	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	selecteda=(int)accttype;
	if(selecteda>=(mtn->ACCTtypes->numlibs))
	{
		listlen=selecteda-mtn->ACCTtypes->numlibs+1;
		for(x=0;x<listlen;x++)
		{
			addAPPlib(mtn->ACCTtypes,"Invalid-Account Type");
		}
	}
	if(FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",selecteda)) return;
	if(update_list) updatersrc(mtnrsrc,"ACCOUNT TYPES");
	changeACCTtype(mtnrsrc,mtn);
	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",&deftype1);
	if(deftype1>=mtn->DEFtypes->numlibs)
	{
		listlen=deftype1-mtn->DEFtypes->numlibs+1;
		for(x=0;x<listlen;x++)
		{
			addAPPlib(mtn->DEFtypes,"Invalid-Definition Type");
		}
	}
	x=(int)deftype1;
	if(x>=mtn->DEFtypes->numlibs) x=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",x,mtn->DEFtypes))
	{
		if(update_list) updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
}
void changeACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=0;
	short accttype=0;
	RDAacct *acct=NULL;

	if(FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"ACCOUNT TYPE",&accttype)) return;
	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
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
			prterr("Invalid Account Type [%d]",accttype);
			break;
	}
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
	updateallrsrc(mtnrsrc);
	list2file(mtnrsrc,mtn);
	SETID(mtnrsrc,mtn);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	list2file(member->parent,mtn);
	ADV2GET_PREVIOUS(member->parent,MTNMASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,list2filecb,
		mtn,setlistscb,mtn,NULL,NULL);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	list2file(member->parent,mtn);
	ADV2GET_NEXT(member->parent,MTNMASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,list2filecb,
		mtn,setlistscb,mtn,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MTNMASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,list2filecb,mtn,
		setlistscb,mtn,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER)
{
	SET_MTNMASTER(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void list2filecb(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,
	void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
        setLISTS(mtnrsrc,mtn,TRUE);
}
