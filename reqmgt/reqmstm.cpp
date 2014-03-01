/* reqmstm.c - Requisition Management's Requisition Master Maintain Program */
#include <cstdio>


#include <mix.hpp>
#include <reqmgt.hpp>

static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void set_appdefaults(RDArsrc *,short);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void seteditable(RDArsrc *,RDATData *,short);
static void seteditablecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
void reqmstm(short,void *targetkey);
extern short reqloc,fineyr,finbyr;
extern MakeBrowseList *mbl_reqmst;
extern char editable;
extern void browse_reqmst(RDArsrc *,RDATData *);
extern MaintainMaster *MTNMASTER;

void reqmstm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
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
					set_appdefaults(mtnrsrc,FALSE);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				set_appdefaults(mtnrsrc,FALSE);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
					set_appdefaults(mtnrsrc,FALSE);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MTNMASTER);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_reqmst!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_reqmst,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
	if(mbl_reqmst!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_reqmst==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MTNMASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
        short keyno=(-1);

        keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
        if(keyno==(-1))
        {
                keyno=MTNMASTER->passkey->keyno;
        }
	ADV2GET_NEXT(member->parent,MTNMASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MTNMASTER,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER)
{
	SET_MTNMASTER(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
}
static void save_record_now(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *desc=NULL;

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
				(update?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update);
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
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *rcddesc=NULL;
	char deleteflag1=0,deleteflag2=0,deleteflag3=0;
	char *locid1=NULL,*locid2=NULL,*locid3=NULL;
	char *reqid1=NULL,*reqid2=NULL,*reqid3=NULL;
	int itemno1=0,itemno2=0;
	short ef_subfile1=0,ef_subfile2=0,ef_subfile3=0;
	short sub_file1=(-1),sub_file2=(-1),sub_file3=(-1);
	int fiscalyr=0;
	short pmonth=0;
	double amount=0.0;
	REQACC *prev_REQACC=NULL;
	RDATData *prev_subfile1=NULL,*prev_subfile2=NULL,*prev_subfile3=NULL;
	RDArsrc *tmprsrc=NULL;

	if(ADVRECORDsec(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,previous);
		FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&deleteflag1);
		if(deleteflag1)
		{
			FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"LOCATION IDENTIFICATION",&locid1);
			FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"REQUISITION IDENTIFICATION",&reqid1);
			sub_file1=OPNNRDsec("REQMGT","REQITM",TRUE,TRUE,FALSE);
			sub_file2=OPNNRDsec("REQMGT","REQACT",TRUE,TRUE,FALSE);
			sub_file3=OPNNRDsec("REQMGT","REQACC",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW("REQMGT",NULL);
			addDFincvir(tmprsrc,"REQMGT","REQMST",NULL,MTNMASTER->passkey->fileno);
			addDFincvir(tmprsrc,"REQMGT","REQLOC",NULL,reqloc);
			addDFincvir(tmprsrc,"REQMGT","REQITM",NULL,sub_file1);
			addDFincvir(tmprsrc,"REQMGT","REQACT",NULL,sub_file2);
			addDFincvir(tmprsrc,"REQMGT","REQACC",NULL,sub_file3);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			if(sub_file1!=(-1))
			{
				ZERNRD(sub_file1);
				FINDFLDSETSTRING(sub_file1,"LOCATION IDENTIFICATION",locid1);
				FINDFLDSETSTRING(sub_file1,"REQUISITION IDENTIFICATION",reqid1);
				ef_subfile1=ADVGTENRDsec(sub_file1,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_subfile1)
				{
					FINDFLDGETSTRING(sub_file1,"LOCATION IDENTIFICATION",&locid2);
					FINDFLDGETSTRING(sub_file1,"REQUISITION IDENTIFICATION",&reqid2);
					if(RDAstrcmp(locid2,locid1) || RDAstrcmp(reqid2,reqid1)) break;
					FINDFLDGETCHAR(sub_file1,"DELETEFLAG",&deleteflag2);
					if(!deleteflag2)
					{
						FINDFLDGETINT(sub_file1,"ITEM NUMBER",&itemno1);
 						FINDFLDGETINT(sub_file1,"FISCAL YEAR",&fiscalyr);
						FINDFLDGETSHORT(sub_file1,"PROCESSING MONTH",&pmonth);
						FINDFLDGETDOUBLE(sub_file1,"AMOUNT",&amount);
						prev_REQACC=ADVGetREQACC(sub_file3,locid1,reqid1,itemno1,SCRNvirtualSubData,mtnrsrc);
						ComputeREQACC(prev_REQACC,amount);
						ADVREQACC_BACKOUT_UNDIST(prev_REQACC,fiscalyr,pmonth,fineyr,finbyr,SCRNvirtualSubData,mtnrsrc);
						FreeREQACC(prev_REQACC);
						prev_subfile1=RDATDataNEW(sub_file1);
						FINDFLDSETCHAR(sub_file1,"DELETEFLAG",TRUE);
						ADVWRTTRANSsec(sub_file1,0,NULL,prev_subfile1,SCRNvirtualSubData,tmprsrc);
						if(prev_subfile1!=NULL) FreeRDATData(prev_subfile1);
						if(sub_file2!=(-1))
						{
							ZERNRD(sub_file2);
							FINDFLDSETSTRING(sub_file2,"LOCATION IDENTIFICATION",locid1);
							FINDFLDSETSTRING(sub_file2,"REQUISITION IDENTIFICATION",reqid1);
							FINDFLDSETINT(sub_file2,"ITEM NUMBER",itemno1);
							ef_subfile2=ADVGTENRDsec(sub_file2,1,SCRNvirtualSubData,tmprsrc);
							while(!ef_subfile2)
							{
								FINDFLDGETSTRING(sub_file2,"LOCATION IDENTIFICATION",&locid3);
								FINDFLDGETSTRING(sub_file2,"REQUISITION IDENTIFICATION",&reqid3);
								FINDFLDGETINT(sub_file3,"ITEM NUMBER",&itemno2);
								if(RDAstrcmp(locid3,locid1) || RDAstrcmp(reqid3,reqid1) || itemno2!=itemno1) break;
								FINDFLDGETCHAR(sub_file2,"DELETEFLAG",&deleteflag3);
								if(!deleteflag3)
								{
									prev_subfile2=RDATDataNEW(sub_file2);
									FINDFLDSETCHAR(sub_file2,"DELETEFLAG",TRUE);
									ADVWRTTRANSsec(sub_file2,0,NULL,prev_subfile2,SCRNvirtualSubData,tmprsrc);
									if(prev_subfile2!=NULL) FreeRDATData(prev_subfile2);
								}
								ef_subfile2=ADVNXTNRDsec(sub_file2,1,SCRNvirtualSubData,tmprsrc);
							}
							if(locid3!=NULL) Rfree(locid3);
							if(reqid3!=NULL) Rfree(reqid3);
						}
						if(sub_file3!=(-1))
						{
							ZERNRD(sub_file3);
							FINDFLDSETSTRING(sub_file3,"LOCATION IDENTIFICATION",locid1);
							FINDFLDSETSTRING(sub_file3,"REQUISITION IDENTIFICATION",reqid1);
							FINDFLDSETINT(sub_file3,"ITEM NUMBER",itemno1);
							ef_subfile3=ADVGTENRDsec(sub_file3,1,SCRNvirtualSubData,tmprsrc);
							while(!ef_subfile3)
							{
								FINDFLDGETSTRING(sub_file3,"LOCATION IDENTIFICATION",&locid3);
								FINDFLDGETSTRING(sub_file3,"REQUISITION IDENTIFICATION",&reqid3);
								FINDFLDGETINT(sub_file3,"ITEM NUMBER",&itemno2);
								if(RDAstrcmp(locid3,locid1) || RDAstrcmp(reqid3,reqid1) || itemno2!=itemno1) break;
								FINDFLDGETCHAR(sub_file3,"DELETEFLAG",&deleteflag3);
								if(!deleteflag3)
								{
									prev_subfile3=RDATDataNEW(sub_file3);
									FINDFLDSETCHAR(sub_file3,"DELETEFLAG",TRUE);
									ADVWRTTRANSsec(sub_file3,0,NULL,prev_subfile3,SCRNvirtualSubData,tmprsrc);
									if(prev_subfile3!=NULL) FreeRDATData(prev_subfile3);
								}
								ef_subfile3=ADVNXTNRDsec(sub_file3,1,SCRNvirtualSubData,tmprsrc);
							}
							if(locid3!=NULL) Rfree(locid3);
							if(reqid3!=NULL) Rfree(reqid3);
						}
					}
					ef_subfile1=ADVNXTNRDsec(sub_file1,1,SCRNvirtualSubData,tmprsrc);
				}
				if(locid2!=NULL) Rfree(locid2);
				if(reqid2!=NULL) Rfree(reqid2);
				if(sub_file1!=(-1)) CLSNRD(sub_file1);
				if(sub_file2!=(-1)) CLSNRD(sub_file2);
				if(sub_file3!=(-1)) CLSNRD(sub_file3);
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
		}
		if(locid1!=NULL) Rfree(locid1);
		if(locid2!=NULL) Rfree(locid2);
		if(locid3!=NULL) Rfree(locid3);
		if(reqid1!=NULL) Rfree(reqid1);
		if(reqid2!=NULL) Rfree(reqid2);
		DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_reqmst,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
		if(mbl_reqmst!=NULL)
		{
			quit_record(mtnrsrc,previous);
		} else {
			seteditable(mtnrsrc,previous,update);
			GetRDATData(MTNMASTER->passkey->fileno,previous);
		}
	}
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
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
static void set_appdefaults(RDArsrc *mtnrsrc,short update)
{
	char *date=NULL,*timex=NULL;
	
	FINDRSCSETSTRING(mtnrsrc,"[REQMST][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update)
	{
		 updatersrc(mtnrsrc,"[REQMST][SOURCE USER]");
		 updatersrc(mtnrsrc,"[REQMST][ENTRY DATE]");
		 updatersrc(mtnrsrc,"[REQMST][ENTRY TIME]");
	}
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	set_appdefaults(mtnrsrc,TRUE);
}
static void seteditable(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[REQMST][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"[REQMST][DELETEFLAG]",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,FALSE,FALSE);
        } else {
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,TRUE,TRUE);
        }
}
static void seteditablecb(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
	seteditable(mtnrsrc,previous,TRUE);
}
