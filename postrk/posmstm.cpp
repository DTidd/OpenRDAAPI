/* posmstm.c - Position Master Maintain Screen */
#include <stdio.h>
#include <mix.h>
#include <postrk.h>

extern void browse_posmst(RDArsrc *,RDATData *);
extern char editable;
MakeBrowseList *mbl_posmst=NULL;
extern MaintainMaster *MASTER;
static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void SETID(RDArsrc *,RDATData *),SETFILES(RDArsrc *,MaintainMaster *);
void posmstm(short,void *);
#define numfiles2del    13
static char *files2delete[numfiles2del*2]={"PAYJMST","JOBMSTR KEY","PAYJAMS",
	"PAYJAMS KEY","PAYJCSM","PAYJCSM KEY","PAYJRCS","PAYJRCS KEY","PAYJDCM",
	"PAYJDCM KEY","PAYJFSM","PAYJFSM KEY","PAYJRFS","PAYJRFS KEY","PAYJDFM",
	"PAYJDFM KEY","PAYJPMS","PAYJPMS KEY","PAYJDPM","RDA PAYJDPM KEY","PAYJPRM",
	"PAYJPRM KEY","PAYJTRN","PAYJTRN KEY","PAYDTRN","PAYDTRN KEY"};
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);

static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	GET_PREVIOUS(member->parent,MASTER,previous,SCRNvirtualSubData,member->parent,keyno);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	GET_NEXT(member->parent,MASTER,previous,SCRNvirtualSubData,member->parent,keyno);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	 SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,FALSE);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
void posmstm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short nofields=0,nokeys,x=0,y=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	previous=RDATDataNEW(MASTER->passkey->fileno);
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
			if(!RDAstrcmp(fldx->name,"FILLED FTE AMOUNT"))
			{
				edit_rsrc=FALSE;
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
			} else if(!RDAstrcmp(fldx->name,"TOTAL FTE AMOUNT"))
			{
				edit_rsrc=FALSE;
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
			} else {
				edit_rsrc=editable;
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
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
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
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
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
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
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
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_posmst!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_posmst,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL); 
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_posmst!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_posmst==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
}
static void save_record_now(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_record(mtnrsrc,previous,FALSE);
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
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *desc=NULL;

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
				desc,(update ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	short ef=FALSE,ef2=FALSE,keyno=(-1),poskeyno=(-1);
	short sub_file=(-1),sub_file1=(-1);
	char *rcddesc=NULL,**files,*errdesc=NULL;
	char deleteflag=FALSE,deleteflag1=FALSE;
	char *posid=NULL;
	char *perid=NULL,*perid1=NULL,*temp=NULL;
	int jobno=0,jobno1=0,x=0;
	double fteamount=0.0;
	RDATData *previous1=NULL,*previous2=NULL;
	RDArsrc *tmprsrc=NULL,*tmprsrc1=NULL;

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
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
		if(deleteflag)
		{
			sub_file=OPNNRDsec(MASTER->module,"POSPER",TRUE,TRUE,FALSE);
 			poskeyno=KEYNUMBER(sub_file,"POSPER KEY");
			if(poskeyno==(-1))
			{
				CLSNRD(sub_file);
				if(tmprsrc1!=NULL) free_rsrc(tmprsrc1);
				errdesc=Rmalloc(550+RDAstrlen("POSTRK")+RDAstrlen("POSPER")+RDAstrlen("POSPER KEY"));
				sprintf(errdesc,"The keyname [%s] is missing for the [%s][%s] file.  Therefore, the associated records cannot be deleted for this file.  You will need to add the key if it is supposed to exist and bring up the main record in deleted form and resave as deleted, or select the correct key for the subordinate file to connect correctly and then bring up this main record in the deleted form and resave as deleted, or manually set the records to delete.","POSPER KEY","POSTRK","POSPER");
				ERRORDIALOG("MISSING KEY NAME!",errdesc,NULL,FALSE);
				prterr(errdesc);
				if(errdesc!=NULL) Rfree(errdesc);
			} else {
				FINDFLDGETSTRING(MASTER->passkey->fileno,"POSITION IDENTIFICATION",&posid);
				tmprsrc=RDArsrcNEW(MASTER->module,NULL);
				addDFincvir(tmprsrc,MASTER->module,"POSMSTR",NULL,MASTER->passkey->fileno);
				addDFincvir(tmprsrc,MASTER->module,"POSPER",NULL,sub_file);
				GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
				if(sub_file!=(-1))
				{
				ZERNRD(sub_file);
				FINDFLDSETSTRING(sub_file,"POSITION IDENTIFICATION",posid);
				ef=ADVGTENRDsec(sub_file,1,SCRNvirtualSubData,tmprsrc);
				while(!ef)
				{
					FINDFLDGETSTRING(sub_file,"POSITION IDENTIFICATION",&temp);
					if(RDAstrcmp(temp,posid)) break;
					FINDFLDGETCHAR(sub_file,"DELETEFLAG",&deleteflag1);
					if(!deleteflag1)
					{
						LOCNRD(sub_file);
						previous1=RDATDataNEW(sub_file);
						FINDFLDGETSTRING(sub_file,"PERSONNEL IDENTIFICATION",&perid);
						FINDFLDGETINT(sub_file,"JOB NUMBER",&jobno);
						for(x=0,files=files2delete;x<numfiles2del;++x,files+=2)
						{
							tmprsrc1=RDArsrcNEW(MASTER->module,NULL);
							sub_file1=OPNNRDsec("PAYROLL",files[0],TRUE,TRUE,FALSE);
							addDFincvir(tmprsrc1,"PAYROLL",files[0],NULL,sub_file1);
							addDFincvir(tmprsrc1,MASTER->module,"POSMSTR",NULL,MASTER->passkey->fileno);
							addDFincvir(tmprsrc1,MASTER->module,"POSPER",NULL,sub_file);
							GET_ALL_SCREEN_VIRTUALS(tmprsrc1,0);
							if(sub_file1!=(-1))
							{
								ZERNRD(sub_file1);
								keyno=KEYNUMBER(sub_file1,files[1]);
								if(keyno==(-1))
								{
									CLSNRD(sub_file1);
									if(tmprsrc1!=NULL) free_rsrc(tmprsrc1);
									errdesc=Rmalloc(550+RDAstrlen(files[1])+RDAstrlen(files[0])+RDAstrlen("PAYROLL"));
									sprintf(errdesc,"The keyname [%s] is missing for the [%s][%s] file.  Therefore, the associated records cannot be deleted for this file.  You will need to add the key if it is supposed to exist and bring up the main record in deleted form and resave as deleted, or select the correct key for the subordinate file to connect correctly and then bring up this main record in the deleted form and resave as deleted, or manually set the records to delete.",files[1],"PAYROLL",files[0]);
									ERRORDIALOG("MISSING KEY NAME!",errdesc,NULL,FALSE);
									prterr(errdesc);
									if(errdesc!=NULL) Rfree(errdesc);
								} else {
									FINDFLDSETSTRING(sub_file1,"PERSONNEL IDENTIFICATION",perid);
									FINDFLDSETINT(sub_file1,"JOB NUMBER",jobno);
									ef2=ADVGTENRDsec(sub_file1,keyno,SCRNvirtualSubData,tmprsrc1);
									while(!ef2)
									{
										FINDFLDGETSTRING(sub_file1,"PERSONNEL IDENTIFICATION",&perid1);
										FINDFLDGETINT(sub_file1,"JOB NUMBER",&jobno1);
										if(RDAstrcmp(perid1,perid) || jobno1!=jobno) break;
										FINDFLDGETCHAR(sub_file1,"DELETFLAG",&deleteflag1);
										if(!deleteflag1)
										{
										LOCNRD(sub_file1);
										previous2=RDATDataNEW(sub_file1);
										if(!x)
										{
											FINDFLDGETDOUBLE(sub_file1,"FTE AMOUNT",&fteamount);
											ADVupdateftes(MASTER->passkey->fileno,posid,-fteamount,SCRNvirtualSubData,tmprsrc);
										}	
										FINDFLDSETCHAR(sub_file1,"DELETEFLAG",deleteflag);
										ADVWRTTRANSsec(sub_file1,0,NULL,previous2,SCRNvirtualSubData,tmprsrc1);
																					UNLNRD(sub_file1);
										if(previous2!=NULL) FreeRDATData(previous2);
										}
										ef2=ADVNXTNRDsec(sub_file1,keyno,SCRNvirtualSubData,tmprsrc1);
									}
									if(perid1!=NULL) Rfree(perid1);
									CLSNRD(sub_file1);
								}
								if(tmprsrc1!=NULL) free_rsrc(tmprsrc1);
							}
						}
						FINDFLDSETCHAR(sub_file,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(sub_file,0,NULL,previous1,SCRNvirtualSubData,tmprsrc);
						UNLNRD(sub_file);
						if(previous1!=NULL) FreeRDATData(previous1);
						if(temp!=NULL) Rfree(temp);
					}
					ef=ADVNXTNRDsec(sub_file,1,SCRNvirtualSubData,tmprsrc);
				}
				CLSNRD(sub_file);
				if(temp!=NULL) Rfree(temp);
				if(perid!=NULL) Rfree(perid);
				if(perid1!=NULL) Rfree(perid1);
				}
				if(posid!=NULL) Rfree(posid);
			}
			DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		} 
		updatebrowse(update_list,mbl_posmst,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_posmst!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(MASTER->passkey->fileno,previous);
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
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
