/* posperm.c - Xpert Position Personnel Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <postrk.h>

struct mtnstructs
{
	RDATData *previous;
	RDATData *previousjmst;
};
typedef struct mtnstructs mtnstruct;

extern short msrnum,grsnum,jmstnum;
extern char editable;
extern MakeBrowseList *mbl_posper;
extern MaintainMaster *MASTER;
extern void browse_posper(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void get_payjmst(RDArsrc *,mtnstruct *,short);
static void get_payjmst_up(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void get_payjmst_supporting(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
void posperm(short,void *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);

static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,get_payjmst_up,mtn,NULL,NULL);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,get_payjmst_up,mtn,NULL,NULL);
}
static void get_payjmst_up(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),
	void *args,mtnstruct *mtn)
{
	get_payjmst(mtnrsrc,mtn,TRUE);
}
static void get_payjmst(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
        if(jmstnum!=(-1))
        {
		COPYFIELD(MASTER->passkey->fileno,jmstnum,"PERSONNEL IDENTIFICATION");
		COPYFIELD(MASTER->passkey->fileno,jmstnum,"JOB NUMBER");
                if(ADVEQLNRDsec(jmstnum,1,SCRNvirtualSubData,mtnrsrc))
                {
                        KEYNRD(jmstnum,1);
                	if(update) updatefilersrc(jmstnum,mtnrsrc);
                	rsrc2filerecord(jmstnum,mtnrsrc);
                        ClearRDATData(mtn->previousjmst);
                } else {
                	if(update) updatefilerecord2rsrc(jmstnum,mtnrsrc);
                	else filerecord2rsrc(jmstnum,mtnrsrc);
                        GetRDATData(jmstnum,mtn->previousjmst);
                }
        }
	get_payjmst_supporting(mtnrsrc,mtn,update);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NULL,NULL,get_payjmst_up,mtn,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	rsrc2filerecord(jmstnum,mtnrsrc);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
	get_payjmst_supporting(mtnrsrc,mtn,TRUE);
	ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc);
}
void posperm(short dowhich,void *targetkey)
{
	char *name=NULL;
	mtnstruct *mtn=NULL;
	short nofields=0,nokeys=0,x,y,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	ZERNRD(jmstnum);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->previousjmst=NULL;
	mtn->previousjmst=RDATDataNEW(jmstnum);
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
			nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
			}
			sprintf(name,"[%s][%s]",MASTER->mainfile,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,SETFILES,mtn);
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
	nofields=NUMFLDS(jmstnum);
	fields=FLDPOINTER(jmstnum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=editable;
			if(!RDAstrcmp(fldx->name,"PERSONNEL IDENTIFICATION")
					|| !RDAstrcmp(fldx->name,"POSITION IDENTIFICATION")
					|| !RDAstrcmp(fldx->name,"JOB NUMBER")
					|| !RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
				edit_rsrc=FALSE;
				
			}
			nonlstmakefld(mtnrsrc,jmstnum,fldx,edit_rsrc);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen("PAYJMST"));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen("PAYJMST"));
			}
			sprintf(name,"[%s][%s]","PAYJMST",fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,SETFILES,mtn);
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	file2rsrc(grsnum,mtnrsrc,FALSE);
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
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
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
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	get_payjmst(mtnrsrc,mtn,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_posper!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_posper,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_posper!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_posper==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	ZERNRD(jmstnum);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	ZERNRD(jmstnum);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	rsrc2filerecord(jmstnum,mtnrsrc);
	COPYFIELD(MASTER->passkey->fileno,jmstnum,"POSITION IDENTIFICATION");
	COPYFIELD(MASTER->passkey->fileno,jmstnum,"PERSONNEL IDENTIFICATION");
	COPYFIELD(MASTER->passkey->fileno,jmstnum,"JOB NUMBER");
	COPYFIELD(MASTER->passkey->fileno,jmstnum,"DELETEFLAG");
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_record(mtnrsrc,mtn,FALSE);
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
	char delflag=FALSE,*grossid=NULL,*desc=NULL;

	FINDRSCGETCHAR(mtnrsrc,"[POSPER][DELETEFLAG]",&delflag);
	if(!delflag)
	{
		FINDFLDGETSTRING(jmstnum,"GROSS IDENTIFICATION",&grossid);
		FINDFLDSETSTRING(grsnum,"GROSS IDENTIFICATION",grossid);
		if(ADVEQLNRDsec(grsnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			desc=Rmalloc(RDAstrlen(grossid)+125);
			sprintf(desc,"The Gross Identification of [%s] is an invalid value.  Please select a valid Gross Identification to save this record.",(grossid==NULL?"":grossid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID GROSS IDENTIFICATION!",desc,NULL,NULL,TRUE,NULL);
			if(desc!=NULL) Rfree(desc);
			if(grossid!=NULL) Rfree(grossid);
			GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
			get_payjmst_supporting(mtnrsrc,mtn,TRUE);
			return;
		} else {
			FINDFLDGETCHAR(grsnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				desc=Rmalloc(RDAstrlen(grossid)+125);
				sprintf(desc,"The Gross Identification of [%s] is a deleted value.  Please select a valid Gross Identification to save this record.",(grossid==NULL?"":grossid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED GROSS IDENTIFICATION!",desc,NULL,NULL,TRUE,NULL);
				if(desc!=NULL) Rfree(desc);
				if(grossid!=NULL) Rfree(grossid);
				GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
				get_payjmst_supporting(mtnrsrc,mtn,TRUE);
				return;
			}
		}
		if(grossid!=NULL) Rfree(grossid);
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
				desc,(update_list ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL,*posid1=NULL,*posid2=NULL;
	char delflag=FALSE,active=FALSE;
	double /* totalfte=0.0,filledfte=0.0, */ fteamount=0.0;
	short numflds=0,y=0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	RDATData *previous=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else if(ADVRECORDsec(jmstnum,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(jmstnum,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		numflds=NUMFLDS(jmstnum);
		if(numflds>0)
		{
			FINDFLDGETSTRING(jmstnum,"POSITION IDENTIFICATION",&posid2);
			tmpfld=Rmalloc(numflds*sizeof(NRDfield));
			for(y=0,fldx=tmpfld;y<numflds;++y,++fldx)
			{
				tmp=FLDNUM(jmstnum,y);
				cpfldinfo(fldx,tmp);
			}
			LOCNRDFILE(MASTER->passkey->fileno);
			if(!ADVEQLNRDsec(jmstnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				FINDFLDGETSTRING(jmstnum,"POSITION IDENTIFICATION",&posid1);
				if(RDAstrcmp(posid1,posid2))
				{
					FINDFLDSETSTRING(MASTER->passkey->fileno,"POSITION IDENTIFICATION",posid1);
					if(!ADVEQLNRDsec(MASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
					{
						previous=RDATDataNEW(MASTER->passkey->fileno);
						delflag=TRUE;
						FINDFLDSETCHAR(MASTER->passkey->fileno,"DELETEFLAG",delflag);
						WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
						if(previous!=NULL) FreeRDATData(previous);
					}
					rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
				}
				FINDFLDGETCHAR(jmstnum,"ACTIVE",&active);
				FINDFLDGETCHAR(jmstnum,"DELETEFLAG",&delflag);
				if(active==TRUE  && delflag==FALSE)
				{
					FINDFLDGETSTRING(jmstnum,"POSITION IDENTIFICATION",&posid1);
					FINDFLDGETDOUBLE(jmstnum,"FTE AMOUNT",&fteamount);
					ADVupdateftes(msrnum,posid1,-fteamount,SCRNvirtualSubData,mtnrsrc);
				} /* end if(active==TRUE) */
			} /* end if(!ADVEQLNRDsec(jmstnum,1,SCRNvirtualSubData,mtnrsrc)) */
			UNLNRDFILE(MASTER->passkey->fileno);
			fields2file(jmstnum,tmpfld);
			FINDFLDGETCHAR(jmstnum,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(jmstnum,"ACTIVE",&active);
			if(active==TRUE && delflag==FALSE)
			{
				FINDFLDGETSTRING(jmstnum,"POSITION IDENTIFICATION",&posid1);
				FINDFLDGETDOUBLE(jmstnum,"FTE AMOUNT",&fteamount);
				ADVupdateftes(msrnum,posid1,fteamount,SCRNvirtualSubData,mtnrsrc);
			} /* end if(active==FALSE) */
			WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
			WRTTRANS(jmstnum,0,NULL,mtn->previousjmst);
			if(posid1!=NULL) Rfree(posid1); 
			if(posid2!=NULL) Rfree(posid2); 
			DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
			updatebrowse(update_list,mbl_posper,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		}
		if(mbl_posper!=NULL)
		{
			quit_record(mtnrsrc,mtn);
		} else {
			GetRDATData(MASTER->passkey->fileno,mtn->previous);
			GetRDATData(jmstnum,mtn->previousjmst);
		}
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_testjmst(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	if(QUITvfy(jmstnum,mtn->previousjmst))
	{
		desc=QUITvfydesc(jmstnum,mtn->previousjmst);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_quit,
			quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=(-1);
	RDArmem *member;
	char test=FALSE;
	char *desc=NULL;

	if((x=FINDRSC(mtnrsrc,"SAVE"))!=(-1))
	{
		member=mtnrsrc->rscs+x;
		test=(char)IsWidgetDisplayed(member);
	}
	if(!test)
	{
		if((x=FINDRSC(mtnrsrc,"SAVE NO UPDATE"))!=(-1))
		{
			member=mtnrsrc->rscs+x;
			test=(char)IsWidgetDisplayed(member);
		}
	}
	if(test)
	{
		if(readscreen!=NULL)
		{
			readscreen(mtnrsrc,mtn);
		}
		if(QUITvfy(MASTER->passkey->fileno,mtn->previous))
		{
			desc=QUITvfydesc(MASTER->passkey->fileno,mtn->previous);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_quit,
			quit_record_testjmst,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
		} else {
			quit_record_testjmst(mtnrsrc,mtn);
		}
	} else  {
		quit_record(mtnrsrc,mtn);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->previousjmst!=NULL) FreeRDATData(mtn->previousjmst);
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
static void get_payjmst_supporting(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *grossid=NULL;

	if(grsnum!=(-1))
	{
		COPYFIELD(jmstnum,grsnum,"GROSS IDENTIFICATION");
		if(ADVEQLNRDsec(grsnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(grsnum,1);
		if(update) updatefilerecord2rsrc(grsnum,mtnrsrc);
		else filerecord2rsrc(grsnum,mtnrsrc);
	}
	if(grossid!=NULL) Rfree(grossid);
}
