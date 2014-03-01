/* restyrm.c - Real Estate Year Master Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>

extern short filenum,rstmst,fsclyr;
extern char editable;
extern char *module;
extern void browse_msr(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record_quit(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void getrecord(RDArsrc *,RDATData *);
MakeBrowseList *mbl_restmst=NULL;
extern MaintainMaster *MASTER;
void restmstm(short,void *targetkey);
static void getsupportingfiles(RDArsrc *);

static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readscreen(mtnrsrc,previous);
	if(QUITvfy(filenum,previous))
	{
		desc=QUITvfydesc(filenum,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_quit,quit_record,FALSE,2,mtnrsrc,previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}
void restmstm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	int nofields=0,x;
	NRDfield *fields=NULL,*fldx;
	short edit_rsrc=FALSE;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	addDFincvir(mtnrsrc,module,"RESTYEAR",NULL,filenum);
	addDFincvir(mtnrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(mtnrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if((fldx->type==DOLLARS) &&
				!strncmp(fldx->name,"JANUARY",7) &&
				!strncmp(fldx->name,"FEBRUARY",8) &&
				!strncmp(fldx->name,"MARCH",5) &&
				!strncmp(fldx->name,"APRIL",5) &&
				!strncmp(fldx->name,"MAY",3) &&
				!strncmp(fldx->name,"JUNE",4) &&
				!strncmp(fldx->name,"JULY",4) &&
				!strncmp(fldx->name,"AUGUST",6) &&
				!strncmp(fldx->name,"SEPTEMBER",9) &&
				!strncmp(fldx->name,"OCTOBER",7) &&
				!strncmp(fldx->name,"NOVEMBER",8) &&
				!strncmp(fldx->name,"DECEMBER",8))
			{
				nonlstmakefld(mtnrsrc,filenum,fldx,FALSE);
			} else {
				nonlstmakefld(mtnrsrc,filenum,fldx,edit_rsrc);
			}
		}
	}
	nofields=0;
	file2rsrc(rstmst,mtnrsrc,FALSE);
	file2rsrc(fsclyr,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"[RESTYEAR][REAL ESTATE IDENTIFICATION]",
		getrecord,previous);
	FINDRSCSETFUNC(mtnrsrc,"[RESTYEAR][FISCAL YEAR]",getrecord,previous);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(filenum,MASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(filenum,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(filenum);
				ApplyPassKey(MASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	getsupportingfiles(mtnrsrc);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_restmst!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		}
	}
	if(mbl_restmst==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_msr,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_restmst==NULL ? TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,FALSE);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,
	short update_list)
{
	char *temp=NULL,*warnmessage=NULL,delflag=FALSE;
	int yearid=0;

	FINDFLDGETCHAR(filenum,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(rstmst!=(-1))
		{
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&temp);
			FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",temp);
			if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(rstmst,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+137);
				sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Real Estate Master.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The REAL ESTATE IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTMST] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(rstmst,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exists as a Real Estate, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exist as a Real Estate, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(fsclyr!=(-1))
		{
			FINDFLDGETINT(filenum,"FISCAL YEAR",&yearid);
			FINDFLDSETINT(fsclyr,"FISCAL YEAR",yearid);
			if(ADVEQLNRDsec(fsclyr,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(fsclyr,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+137);
				sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is not valid.  It does not exist as a Fiscal Year.",yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The FISCAL YEAR of [%d] entered does not exist in the [FINMGT][FINYEAR] data file.",yearid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(fsclyr,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is deleted.  It exists as a Fiscal Year, but is presently set up in a deleted state.",yearid);
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The FISCAL YEAR of [%d] entered is deleted.  It exist as a Fiscal Year, but is presently set up in a deleted state.",yearid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				} else {
					FINDFLDGETCHAR(fsclyr,"ACTIVE",&delflag);
					if(!delflag)
					{
						warnmessage=Rmalloc(RDAstrlen(temp)+140);
						sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is not active.  It exists as a Fiscal Year, but is presently set up in an inactive state.",yearid);
						WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
						prterr("The FISCAL YEAR of [%d] entered is not active.  It exists as a Fiscal Year, but is presently set up in an inactive state.",yearid);
						if(warnmessage!=NULL) Rfree(warnmessage);
						return;
					}
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,
	short update_list)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(filenum,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(filenum,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,previous);
		updatebrowse(update_list,mbl_restmst,filenum,module,mtnrsrc);
		if(mbl_restmst!=NULL) quit_record(mtnrsrc,previous);
			else GetRDATData(filenum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(rstmst!=(-1)) CLSNRD(rstmst);
		if(fsclyr!=(-1)) CLSNRD(fsclyr);
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
static void getrecord(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *realestateid1=NULL;
	int fiscalyear=0;

	readwidget(mtnrsrc,"[RESTYEAR][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTYEAR][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTYEAR][FISCAL YEAR]");
	FINDRSCGETINT(mtnrsrc,"[RESTYEAR][FISCAL YEAR]",&fiscalyear);
	FINDFLDSETINT(filenum,"FISCAL YEAR",fiscalyear);
	if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(previous);
	} else {
		GetRDATData(filenum,previous);
		filerecord2rsrc(filenum,mtnrsrc);	
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	getsupportingfiles(mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getsupportingfiles(RDArsrc *mtnrsrc)
{
	if(rstmst!=(-1))
	{
		COPYFIELD(filenum,rstmst,"REAL ESTATE IDENTIFICATION");
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstmst,1);
		}
		filerecord2rsrc(rstmst,mtnrsrc);
	}
	if(fsclyr!=(-1))
	{
		COPYFIELD(filenum,fsclyr,"FISCAL YEAR");
		if(ADVEQLNRDsec(fsclyr,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(fsclyr,1);
		}
		filerecord2rsrc(fsclyr,mtnrsrc);
	}
}
