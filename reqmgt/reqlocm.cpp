/* reqlocm.c - Requisition Management's Location Master Maintain Program */
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
void reqlocm(short,void *targetkey);
extern short fineyr,finbyr;
extern MakeBrowseList *mbl_reqloc;
extern char editable;
extern void browse_reqloc(RDArsrc *,RDATData *);
extern MaintainMaster *MTNMSTR;

void reqlocm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	mtnrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->maintain_screen);
	ZERNRD(MTNMSTR->passkey->fileno);
	previous=RDATDataNEW(MTNMSTR->passkey->fileno);
	nokeys=NUMKEYS(MTNMSTR->passkey->fileno);
	keys=KEYPOINTER(MTNMSTR->passkey->fileno);
	nofields=NUMFLDS(MTNMSTR->passkey->fileno);
	fields=FLDPOINTER(MTNMSTR->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMSTR);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			nonlstmakefld(mtnrsrc,MTNMSTR->passkey->fileno,fldx,edit_rsrc);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMSTR->mainfile));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMSTR->mainfile));
			}
			sprintf(name,"[%s][%s]",MTNMSTR->mainfile,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MTNMSTR);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMSTR->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMSTR->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMSTR->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMSTR);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMSTR->passkey);
			if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMSTR->passkey);
				if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMSTR->passkey);
				} else {
					GetRDATData(MTNMSTR->passkey->fileno,previous);
					filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,previous);
				filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MTNMSTR->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMSTR->passkey);
			if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMSTR->passkey);
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,previous);
				filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MTNMSTR->passkey->fileno);
				ApplyPassKey(MTNMSTR->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMSTR->passkey);
				if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMSTR->passkey);
				} else {
					GetRDATData(MTNMSTR->passkey->fileno,previous);
					filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,previous);
				filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MTNMSTR);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_reqloc!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_reqloc,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MTNMSTR);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMSTR);
	if(mbl_reqloc!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_reqloc==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMSTR->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMSTR->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MTNMSTR->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MTNMSTR,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
        short keyno=(-1);

        keyno=KEYNUMBER(MTNMSTR->passkey->fileno,(member->rscrname+5));
        if(keyno==(-1))
        {
                keyno=MTNMSTR->passkey->keyno;
        }
	ADV2GET_NEXT(member->parent,MTNMSTR,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MTNMSTR,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR)
{
	SET_MTNMASTER(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMSTR->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMSTR->passkey->fileno,mtnrsrc);
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

	if(!SAVE_CHECK(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MTNMSTR->save_expression))
	{
		if(EVALUATEbol(MTNMSTR->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMSTR->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MTNMSTR->save_warning))
	{
		if(EVALUATEbol(MTNMSTR->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMSTR->save_warning_desc,
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
	short sub_file1=(-1),sub_file2=(-1),sub_file3=(-1),sub_file4=(-1);
	short ef_subfile1=0,ef_subfile2=0,ef_subfile3=0,ef_subfile4=0;
	char deleteflag1=FALSE,deleteflag2=FALSE;
	char deleteflag3=FALSE,deleteflag4=FALSE;
	char *locid1=NULL,*locid2=NULL,*locid3=NULL,*locid4=NULL;
	char *reqid1=NULL,*reqid2=NULL,*reqid3=NULL;
	int itemno1=0,itemno2=0;
	RDATData *prev_subfile1=NULL,*prev_subfile2=NULL;
	RDATData *prev_subfile3=NULL,*prev_subfile4=NULL;
	RDArsrc *tmprsrc=NULL;
	int fiscalyr=0;
	short pmonth=0;
	double amount=0.0;
	REQACC *prev_REQACC=NULL;

	if(ADVRECORDsec(MTNMSTR->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMSTR->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MTNMSTR->passkey->fileno,0,NULL,previous);
		FINDFLDGETCHAR(MTNMSTR->passkey->fileno,"DELETEFLAG",&deleteflag1);
		if(deleteflag1)
		{
			FINDFLDGETSTRING(MTNMSTR->passkey->fileno,"LOCATION IDENTIFICATION",&locid1);
			sub_file1=OPNNRDsec("REQMGT","REQMST",TRUE,TRUE,FALSE);
			sub_file2=OPNNRDsec("REQMGT","REQITM",TRUE,TRUE,FALSE);
			sub_file3=OPNNRDsec("REQMGT","REQACT",TRUE,TRUE,FALSE);
			sub_file4=OPNNRDsec("REQMGT","REQACC",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW("REQMGT",NULL);
			addDFincvir(tmprsrc,"REQMGT","REQLOC",NULL,MTNMSTR->passkey->fileno);
			addDFincvir(tmprsrc,"REQMGT","REQMST",NULL,sub_file1);
			addDFincvir(tmprsrc,"REQMGT","REQITM",NULL,sub_file2);
			addDFincvir(tmprsrc,"REQMGT","REQACT",NULL,sub_file3);
			addDFincvir(tmprsrc,"REQMGT","REQACC",NULL,sub_file4);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			if(sub_file1!=(-1))
			{
				ZERNRD(sub_file1);
				FINDFLDSETSTRING(sub_file1,"LOCATION IDENTIFICATION",locid1);
				ef_subfile1=ADVGTENRDsec(sub_file1,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_subfile1)
				{
					FINDFLDGETSTRING(sub_file1,"LOCATION IDENTIFICATION",&locid2);
					if(RDAstrcmp(locid1,locid2)) break;
					FINDFLDGETCHAR(sub_file1,"DELETEFLAG",&deleteflag2);
					if(!deleteflag2)
					{
						FINDFLDGETSTRING(sub_file1,"REQUISITION IDENTIFICATION",&reqid1);
						prev_subfile1=RDATDataNEW(sub_file1);
						FINDFLDSETCHAR(sub_file1,"DELETEFLAG",TRUE);
						ADVWRTTRANSsec(sub_file1,0,NULL,prev_subfile1,SCRNvirtualSubData,tmprsrc);
						if(prev_subfile1!=NULL) FreeRDATData(prev_subfile1);
						if(sub_file2!=(-1))
						{
							ZERNRD(sub_file2);
							FINDFLDSETSTRING(sub_file2,"LOCATION IDENTIFICATION",locid2);
							FINDFLDSETSTRING(sub_file2,"REQUISITION IDENTIFICATION",reqid1);
							ef_subfile2=ADVGTENRDsec(sub_file2,1,SCRNvirtualSubData,tmprsrc);
							while(!ef_subfile2)
							{
								FINDFLDGETSTRING(sub_file2,"LOCATION IDENTIFICATION",&locid3);
								FINDFLDGETSTRING(sub_file2,"REQUISITION IDENTIFICATION",&reqid2);
								if(RDAstrcmp(locid3,locid1) || RDAstrcmp(reqid2,reqid1)) break;
								FINDFLDGETCHAR(sub_file2,"DELETEFLAG",&deleteflag3);
								if(!deleteflag3)
								{
									FINDFLDGETINT(sub_file2,"ITEM NUMBER",&itemno1);
 									FINDFLDGETINT(sub_file1,"FISCAL YEAR",&fiscalyr);
									FINDFLDGETSHORT(sub_file1,"PROCESSING MONTH",&pmonth);
									FINDFLDGETDOUBLE(sub_file1,"AMOUNT",&amount);
									prev_REQACC=ADVGetREQACC(sub_file4,locid1,reqid1,itemno1,SCRNvirtualSubData,mtnrsrc);
									ComputeREQACC(prev_REQACC,amount);
									ADVREQACC_BACKOUT_UNDIST(prev_REQACC,fiscalyr,pmonth,fineyr,finbyr,SCRNvirtualSubData,mtnrsrc);
									FreeREQACC(prev_REQACC);
									prev_subfile2=RDATDataNEW(sub_file2);
									FINDFLDSETCHAR(sub_file2,"DELETEFLAG",TRUE);
									ADVWRTTRANSsec(sub_file2,0,NULL,prev_subfile2,SCRNvirtualSubData,tmprsrc);
									if(prev_subfile2!=NULL) FreeRDATData(prev_subfile2);
									if(sub_file3!=(-1))
									{
										ZERNRD(sub_file3);
										FINDFLDSETSTRING(sub_file3,"LOCATION IDENTIFICATION",locid1);
										FINDFLDSETSTRING(sub_file3,"REQUISITION IDENTIFICATION",reqid1);
										FINDFLDSETINT(sub_file3,"ITEM NUMBER",itemno1);
										ef_subfile3=ADVGTENRDsec(sub_file3,1,SCRNvirtualSubData,tmprsrc);
										while(!ef_subfile3)
										{
											FINDFLDGETSTRING(sub_file3,"LOCATION IDENTIFICATION",&locid4);
											FINDFLDGETSTRING(sub_file3,"REQUISITION IDENTIFICATION",&reqid3);
											FINDFLDGETINT(sub_file3,"ITEM NUMBER",&itemno2);
											if(RDAstrcmp(locid4,locid1) || RDAstrcmp(reqid3,reqid1) || itemno2!=itemno1) break;
											FINDFLDGETCHAR(sub_file3,"DELETEFLAG",&deleteflag4);
											if(!deleteflag4)
											{
												prev_subfile3=RDATDataNEW(sub_file3);
												FINDFLDSETCHAR(sub_file3,"DELETEFLAG",TRUE);
												ADVWRTTRANSsec(sub_file3,0,NULL,prev_subfile3,SCRNvirtualSubData,tmprsrc);
												if(prev_subfile3!=NULL) FreeRDATData(prev_subfile3);
											}
											ef_subfile3=ADVNXTNRDsec(sub_file3,1,SCRNvirtualSubData,tmprsrc);
										}
										if(locid4!=NULL) Rfree(locid4);
										if(reqid3!=NULL) Rfree(reqid3);
									}
									if(sub_file4!=(-1))
									{
										ZERNRD(sub_file4);
										FINDFLDSETSTRING(sub_file4,"LOCATION IDENTIFICATION",locid1);
										FINDFLDSETSTRING(sub_file4,"REQUISITION IDENTIFICATION",reqid1);
										FINDFLDSETINT(sub_file4,"ITEM NUMBER",itemno1);
										ef_subfile4=ADVGTENRDsec(sub_file4,1,SCRNvirtualSubData,tmprsrc);
										while(!ef_subfile4)
										{
											FINDFLDGETSTRING(sub_file4,"LOCATION IDENTIFICATION",&locid4);
											FINDFLDGETSTRING(sub_file4,"REQUISITION IDENTIFICATION",&reqid3);
											FINDFLDGETINT(sub_file4,"ITEM NUMBER",&itemno2);
											if(RDAstrcmp(locid4,locid1) || RDAstrcmp(reqid3,reqid1) || itemno2!=itemno1) break;
											FINDFLDGETCHAR(sub_file4,"DELETEFLAG",&deleteflag4);
											if(!deleteflag4)
											{
												prev_subfile4=RDATDataNEW(sub_file4);
												FINDFLDSETCHAR(sub_file4,"DELETEFLAG",TRUE);
												ADVWRTTRANSsec(sub_file4,0,NULL,prev_subfile4,SCRNvirtualSubData,tmprsrc);
												if(prev_subfile4!=NULL) FreeRDATData(prev_subfile4);
											}
											ef_subfile4=ADVNXTNRDsec(sub_file4,1,SCRNvirtualSubData,tmprsrc);
										}
										if(locid4!=NULL) Rfree(locid4);
										if(reqid3!=NULL) Rfree(reqid3);
									}
								}
								ef_subfile2=ADVNXTNRDsec(sub_file2,1,SCRNvirtualSubData,tmprsrc);
							}
							if(locid3!=NULL) Rfree(locid3);
							if(reqid2!=NULL) Rfree(reqid2);
						}
					}
					ef_subfile1=ADVNXTNRDsec(sub_file1,1,SCRNvirtualSubData,tmprsrc);
				}
				if(locid2!=NULL) Rfree(locid2);
				if(reqid1!=NULL) Rfree(reqid1);
				if(sub_file1!=(-1)) CLSNRD(sub_file1);
				if(sub_file2!=(-1)) CLSNRD(sub_file2);
				if(sub_file3!=(-1)) CLSNRD(sub_file3);
				if(sub_file4!=(-1)) CLSNRD(sub_file4);
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
		}
		if(locid1!=NULL) Rfree(locid1);
		if(locid2!=NULL) Rfree(locid2);
		if(locid3!=NULL) Rfree(locid3);
		if(locid4!=NULL) Rfree(locid4);
		if(reqid1!=NULL) Rfree(reqid1);
		if(reqid2!=NULL) Rfree(reqid2);
		if(reqid3!=NULL) Rfree(reqid3);
		DELETE_SUBORDINATES(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_reqloc,MTNMSTR->passkey->fileno,MTNMSTR->module,mtnrsrc);
		if(mbl_reqloc!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(MTNMSTR->passkey->fileno,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MTNMSTR);
		if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
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
	QUIT_RECORD_TEST(mtnrsrc,MTNMSTR->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
