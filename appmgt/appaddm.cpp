/* appaddm.c - Add Applicants Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

extern void browse_dmg(RDArsrc *),add_education(RDArsrc *);
extern void add_positions(RDArsrc *),add_criteria(RDArsrc *);
extern void add_documents(RDArsrc *),add_endorsements(RDArsrc *);
extern void add_certifications(RDArsrc *),add_references(RDArsrc *);
extern void add_previous_employment(RDArsrc *);
extern MaintainMaster *MTNMASTER;
extern char editable;
static void save_record(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record_now(RDArsrc *,RDATData *);
static void save_check(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void CopyAddress2Mailing(RDArsrc *);
static void CopyAddress2Emergency(RDArsrc *);
static void readscreen(RDArsrc *,RDATData *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void resetdefs(RDArsrc *,RDATData *);

void mtndmg(void *targetkey)
{
	char *name=NULL;
	short nofields=0,nokeys=0,x,y,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
	ZERNRD(MTNMASTER->passkey->fileno);
	previous=RDATDataNEW(MTNMASTER->passkey->fileno);
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
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,
		SCRNvirtualSubData,mtnrsrc))
	{
		ZERNRD(MTNMASTER->passkey->fileno);
		ApplyPassKey(MTNMASTER->passkey);
		ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
		if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
		{
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
		} else {
			GetRDATData(MTNMASTER->passkey->fileno,previous);
			filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
		}
	} else {
		GetRDATData(MTNMASTER->passkey->fileno,previous);
		filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
	}
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MTNMASTER);
	addbtnrsrc(mtnrsrc,"EDUCATION",TRUE,add_education,NULL);
	addbtnrsrc(mtnrsrc,"POSITIONS",TRUE,add_positions,NULL);
	addbtnrsrc(mtnrsrc,"CRITERIA",TRUE,add_criteria,NULL);
	addbtnrsrc(mtnrsrc,"CERTIFICATIONS",TRUE,add_certifications,NULL);
	addbtnrsrc(mtnrsrc,"ENDORSEMENTS",TRUE,add_endorsements,NULL);
	addbtnrsrc(mtnrsrc,"DOCUMENTS",TRUE,add_documents,NULL);
	addbtnrsrc(mtnrsrc,"REFERENCES",TRUE,add_references,NULL);
	addbtnrsrc(mtnrsrc,"PREVIOUS EMPLOYMENT",TRUE,add_previous_employment,NULL);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	addbtnrsrc(mtnrsrc,"BROWSE",TRUE,browse_dmg,NULL);
	addbtnrsrc(mtnrsrc,"COPY TO MAILING ADDRESS",TRUE,
		CopyAddress2Mailing,NULL);
	addbtnrsrc(mtnrsrc,"COPY TO EMERGENCY ADDRESS",TRUE,
		CopyAddress2Emergency,NULL);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,TRUE);
}
static void CopyAddress2Mailing(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][ADDRESS #1]","[APPCNT][MAILING ADDRESS #1]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][ADDRESS #2]","[APPCNT][MAILING ADDRESS #2]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][CITY]","[APPCNT][MAILING CITY]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][STATE]","[APPCNT][MAILING STATE]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][ZIP CODE]","[APPCNT][MAILING ZIP CODE]");
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
	updateallrsrc(mtnrsrc);
}
static void CopyAddress2Emergency(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][ADDRESS #1]","[APPCNT][EMERGENCY ADDRESS #1]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][ADDRESS #2]","[APPCNT][EMERGENCY ADDRESS #2]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][CITY]","[APPCNT][EMERGENCY CITY]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][STATE]","[APPCNT][EMERGENCY STATE]");
	COPYRSRC2RSRC(mtnrsrc,"[APPCNT][ZIP CODE]","[APPCNT][EMERGENCY ZIP CODE]");
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
	updateallrsrc(mtnrsrc);
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MTNMASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,
		NULL,NULL,NULL,NULL,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER)
{
	SET_MTNMASTER(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous)
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
		WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,previous);
		GetRDATData(MTNMASTER->passkey->fileno,previous);
  	}
}
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MTNMASTER);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}

static void save_record_now(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readscreen(mtnrsrc,previous);
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
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,save_record_now,
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous);
}
