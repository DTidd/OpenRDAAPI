/* payddsv.c - Xpert Deduction Description Voucher Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "payddsv.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payddsv.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *DEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short baccnum=(-1),ddanum=(-1);
static short byrnum=(-1);
static RDAfinmgt *finsetup=NULL;
static char editable=TRUE;
static MakeBrowseList *mbl_payddsv=NULL;
static MaintainMaster *MASTER=NULL;
static void browse_payddsv(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecordsetedit(RDArsrc *,mtnstruct *,short);
static void seteditable(RDArsrc *,mtnstruct *);
static void set_appdefaults(RDArsrc *,short);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void SETID(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void setlist(RDArsrc *,mtnstruct *,short);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static void debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);

static void setlist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int x;
	short d;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&d);
	x=d;
	if(mtn->DEFtypes->numlibs<=x)
	{
		x=0;
		d=0;
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
	changeDEFtype(mtnrsrc,mtn,update);
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
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",type);
	changeDEFtype(mtnrsrc,mtn,update);
}
static void getrecordsetedit(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	seteditable(mtnrsrc,mtn);
	setlist(mtnrsrc,mtn,update);
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char dist=FALSE,bnkupd=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[PAYDDSV][DISTRIBUTED]",&dist);
	FINDRSCGETCHAR(mtnrsrc,"[PAYDDSV][BNKREC UPDATED]",&bnkupd);
	if(dist)
	{
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,
			mtnrsrc,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][DELETEFLAG]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][DISTRIBUTED]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][SOURCE USER VERIFIED]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][BNKREC UPDATED]",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,FALSE,FALSE);
		if(!bnkupd)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][BNKREC UPDATED DATE]",TRUE);
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][BNKREC UPDATED TIME]",TRUE);
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][BNKREC UPDATED BY]",TRUE);
		}	
	} else {
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,
			mtnrsrc,TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
		if(!bnkupd)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][BNKREC UPDATED DATE]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][BNKREC UPDATED TIME]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDSV][BNKREC UPDATED BY]",FALSE);
		}	
	}
}
void payddsvm(short dowhich,void *targetkey)
{
	char *name=NULL;
	int which_type=0;
	short nofields=0,x=0,y=0,nokeys=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;	
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	RDAacct *acct=NULL;
	
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);;
	mtn->DEFtypes=APPlibNEW();
	if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
	{
		for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
			x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
		{
			addAPPlib(mtn->DEFtypes,acct->name);
		}
	}
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
			edit_rsrc=editable;
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
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
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[PAYDDSV][DEBIT CODE]",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYDDSV][CREDIT CODE]",credcodecb,mtn);
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
					set_appdefaults(mtnrsrc,FALSE);
					list2file(mtnrsrc,mtn,FALSE);
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
				set_appdefaults(mtnrsrc,FALSE);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					set_appdefaults(mtnrsrc,FALSE);
					list2file(mtnrsrc,mtn,FALSE);
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
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_payddsv!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_payddsv,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	if(mbl_payddsv!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_payddsv==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void debcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_debcode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short check_debcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seldeftype=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	if(!ADVCHECKACCTW(mtnrsrc,"[PAYDDSV][DEBIT CODE]",
		"[PAYDDSV][FISCAL YEAR]",(short)seldeftype,byrnum,
		baccnum,(short)BAL_ACCT,
		"INVALID DEBIT CODE!",FALSE,SCRNvirtualSubData,mtnrsrc)) return(FALSE);
	return(TRUE);
}
static void credcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_credcode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seldeftype=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	if(!ADVCHECKACCTW(mtnrsrc,"[PAYDDSV][CREDIT CODE]",
		"[PAYDDSV][FISCAL YEAR]",(short)seldeftype,byrnum,
		baccnum,(short)BAL_ACCT,
		"INVALID CREDIT CODE!",FALSE,SCRNvirtualSubData,mtnrsrc)) return(FALSE);
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
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *desc=NULL,delflag=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[PAYDDSV][DELETEFLAG]",&delflag);
	if(!delflag)
	{
		if(check_debcode(mtnrsrc,mtn)==TRUE)
		{
			if(check_credcode(mtnrsrc,mtn)==FALSE) return; 
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
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,(update_list ? save_record_now:save_record_now1)
				,NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	short numflds=0,y=0,p_month=0;
	char *rcddesc=NULL;
	short deftype=0;
	char delflag=FALSE;
	char dist=FALSE;
	int procyrid=0;
	double amount=0.0,initamt=0.0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	RDATData *prev_dda=NULL;

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
			if(!ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
				if(delflag==FALSE)
				{
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
					COPYFIELD(MASTER->passkey->fileno,ddanum,"DEDUCTION DESCRIPTION IDENTIFICATION");
					COPYFIELD(MASTER->passkey->fileno,ddanum,"DEFINITION TYPE");
					if(ADVEQLNRDsec(ddanum,1,SCRNvirtualSubData,mtnrsrc))
					{
						prterr("Error - Preveous Deduction Description Account NOT found, may be out of balance!");
					} else {
						LOCNRDFILE(ddanum);
						prev_dda=RDATDataNEW(ddanum);
						FINDFLDGETDOUBLE(ddanum,"AMOUNT",&initamt);
						/*these records reduce undistributed amount!*/
						initamt+=amount;
						FINDFLDSETDOUBLE(ddanum,"AMOUNT",initamt);
						if(ADVWRTTRANSsec(ddanum,0,NULL,prev_dda,SCRNvirtualSubData,mtnrsrc))
						{
							prterr("Error - Error writing Preveous Deduction Description Account, may be out of balance!");
						}
						UNLNRDFILE(ddanum);
						if(prev_dda!=NULL) FreeRDATData(prev_dda);
					}
					if(dist==FALSE)
					{
						FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
						FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&procyrid);
						FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
						/*type not used!*/
						ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),byrnum,procyrid,deftype,""/*Not Used!*/,CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
					}
				}
			} /* end if(!ADVEQLNRDsec) */
			fields2file(MASTER->passkey->fileno,tmpfld);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
			WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
			if(delflag==FALSE)
			{
				FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
				COPYFIELD(MASTER->passkey->fileno,ddanum,"DEDUCTION DESCRIPTION IDENTIFICATION");
				COPYFIELD(MASTER->passkey->fileno,ddanum,"DEFINITION TYPE");
				if(ADVEQLNRDsec(ddanum,1,SCRNvirtualSubData,mtnrsrc))
				{
					prterr("Error - Preveous Deduction Description Account NOT found, may be out of balance!");
				} else {
					LOCNRDFILE(ddanum);
					prev_dda=RDATDataNEW(ddanum);
					FINDFLDGETDOUBLE(ddanum,"AMOUNT",&initamt);
					/*these records reduce undistributed amount!*/
					initamt-=amount;
					FINDFLDSETDOUBLE(ddanum,"AMOUNT",initamt);
					if(ADVWRTTRANSsec(ddanum,0,NULL,prev_dda,SCRNvirtualSubData,mtnrsrc))
					{
						prterr("Error - Error writing Preveous Deduction Description Account, may be out of balance!");
					}
					UNLNRDFILE(ddanum);
					if(prev_dda!=NULL) FreeRDATData(prev_dda);
				}
				if(dist==FALSE)
				{
					FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
					FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&procyrid);
					FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
					/*type not used!*/
					ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),byrnum,procyrid,deftype,""/*Not Used!*/,CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
				}
			}
			DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
			updatebrowse(update_list,mbl_payddsv,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		}
		if(mbl_payddsv!=NULL) 
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn);
			GetRDATData(MASTER->passkey->fileno,mtn->previous);
		}
	} /* end if(numflds<0) */
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
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
static void set_appdefaults(RDArsrc *mtnrsrc,short update)
{
	char *date=NULL,*timex=NULL;
	NRDfield *field=NULL;

	FINDRSCSETSTRING(mtnrsrc,"[PAYDDSV][SOURCE USER]",USERLOGIN);
	field=FLDNRD(MASTER->passkey->fileno,"ENTRY DATE");
	if(field!=NULL)
	{
	if(field->len==10)
	{	
		date=GETCURRENTDATE10();
	} else date=GETCURRENTDATE();
	}
	timex=GETCURRENTTIME();
	FINDFLDSETSTRING(MASTER->passkey->fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ENTRY DATE",date);
	FINDRSCSETSTRING(mtnrsrc,"[PAYDDSV][ENTRY DATE]",date);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ENTRY TIME",timex);
	FINDRSCSETSTRING(mtnrsrc,"[PAYDDSV][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update)
	{
		updatersrc(mtnrsrc,"[PAYDDSV][SOURCE USER]");
		updatersrc(mtnrsrc,"[PAYDDSV][ENTRY DATE]");
		updatersrc(mtnrsrc,"[PAYDDSV][ENTRY TIME]");
	}
}
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	set_appdefaults(mtnrsrc,TRUE);
	list2file(mtnrsrc,mtn,TRUE);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	getrecordsetedit(mtnrsrc,mtn,TRUE);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,member->parent,TRUE,TRUE);
	FINDRSCSETEDITABLE(member->parent,"DEFINITION TYPES",TRUE);
	SetRDAScrolledListEditable(member->parent,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,member->parent,TRUE,TRUE);
	FINDRSCSETEDITABLE(member->parent,"DEFINITION TYPES",TRUE);
	SetRDAScrolledListEditable(member->parent,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
	SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selectedd=0;
	short deftype1=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"[PAYDDSV][DEBIT CODE]",&deftype1);
	deftype2=(short)selectedd;
	if(deftype1!=deftype2)
	{
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDSV][DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDSV][CREDIT CODE]",deftype2);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDSV][DEBIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDSV][CREDIT CODE]",NULL);
	}
	updatersrc(mtnrsrc,"[PAYDDSV][DEBIT CODE]");
	updatersrc(mtnrsrc,"[PAYDDSV][CREDIT CODE]");
	updateSCRNvirtuals(mtnrsrc);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selectedd=0;
	short deftype1=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"[PAYDDSV][DEBIT CODE]",&deftype1);
	deftype2=(short)selectedd;
	if(deftype1!=deftype2)
	{
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDSV][DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDSV][CREDIT CODE]",deftype2);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"[PAYDDSV][DEBIT CODE]");
		updatersrc(mtnrsrc,"[PAYDDSV][CREDIT CODE]");
        }
}
void browse_payddsv(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_payddsv=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,"PAYROLL","MTN PAYDDSV")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
	MASTER=MaintainMasterNew("PAYROLL","MTN PAYDDSV");
	if(findMaintainMaster(MASTER))
        {
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PAYDDSV",MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"PAYDDSV",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MASTER,MASTER->module,"PAYDDAC",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		byrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		ddanum=APPReturnSupportingFileno(MASTER,MASTER->module,"PAYDDAC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				payddsvm(1,NULL);
				break;
			case 1:
				browse_payddsv(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
static void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL) 
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		if(finsetup!=NULL) free_finmgt(finsetup);
		ShutdownSubsystems();
	}
	if(mbl_payddsv!=NULL) mbl_payddsv=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			payddsvm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		payddsvm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	payddsvm(2,NULL);
} 
