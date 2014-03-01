/* procexc2.c - Run Report Process Exceptions Maintain Master Browse */
/*lint -library */
#include<mix.hpp>
#include<rptgen.hpp>
#include<powadd.hpp>
#include<guip.hpp>
#define REPLICATE	100

char TableEdit=FALSE;

void process_exceptions2(HoldReport *);
static void REPORTExecuteListFunc(RDArsrc *,int,void *,MTNPassableStruct *,int,char *);
static void MakeREPORTMasterButtons(RDArsrc *,MTNPassableStruct *,void (*)(...),int,char *);
void REPORTExecuteBrowseButtonFunc(RDArsrc *,MaintainButton *,void *,MTNPassableStruct *);
void xMakeREPORTMasterBrowseButtons(RDArsrc *,MTNPassableStruct *,void (*)(...),int,char *);
static void okREPORTBrowseButtonFunc(RDArsrc *,MaintainButton *);
void ClearProcExcpAccums(MakeBrowseList *);
void UpdateProcExcpAccums(MakeBrowseList *);
static void REPORTButtonFunction(RDArsrc *,MaintainButton *);
static void doexit_forced(MTNPassableStruct *);
static void mtnutlmstr(short,void *targetkey,MTNPassableStruct *,int,void (*)(...),RDArsrc *,void *);
static void save_check(RDArsrc *,MTNPassableStruct *,short);
static void save_record(RDArsrc *,MTNPassableStruct *,short);
static void save_record_quit(RDArsrc *,MTNPassableStruct *);
static void save_record_quit3(RDArsrc *,MTNPassableStruct *);
static void save_record_up(RDArsrc *,MTNPassableStruct *);
static void save_record_nup(RDArsrc *,MTNPassableStruct *);
static void save_record_now(RDArsrc *,MTNPassableStruct *);
static void save_record_now1(RDArsrc *,MTNPassableStruct *);
static void quit_record(RDArsrc *,MTNPassableStruct *);
static void quit_record4(RDArsrc *,MTNPassableStruct *);
static void quit_record_test(RDArsrc *,MTNPassableStruct *);
static void quit_record_test3(RDArsrc *,MTNPassableStruct *);
static void quit_record_test4(RDArsrc *,MTNPassableStruct *);
static void readscreen(RDArsrc *,MTNPassableStruct *);
static void resetdefs(RDArsrc *,MTNPassableStruct *);
static void getpreviouscb(RDArmem *,MTNPassableStruct *);
static void getnextcb(RDArmem *,MTNPassableStruct *);
static void SETID(RDArmem *,MTNPassableStruct *);
static void SETFILES(RDArmem *,MTNPassableStruct *); 
static void SETID1(RDArmem *,MTNPassableStruct *);
static void SETFILES1(RDArmem *,MTNPassableStruct *); 
static void run_prequit_buttonsmtn(RDArsrc *,MTNPassableStruct *,short);
static void run_presave_buttons(RDArsrc *,MTNPassableStruct *,short);
static void run_postsave_buttons(RDArsrc *,MTNPassableStruct *,short);
static void updateallMaintainII(RDArsrc *,MTNPassableStruct *,void *);
static short procexcpselectfunction2(MakeBrowseList *);
static void mtnexcpts2(short ,void *,MTNPassableStruct *,int,void (*)(...),RDArsrc *,void *);
static void procexcpquitrecord2(RDArsrc *,MTNPassableStruct *);
static void procexcpokfunc2(MakeBrowseList *,void *);
static void procexcpnewrecord2(RDArsrc *,MakeBrowseList *,void *);
static void abort_procexcp2(MakeBrowseList *);
static void abort_procexcp3(RDArsrc *,MTNPassableStruct *);
static void doabort_procexcptest2(MakeBrowseList *);
static void doabort_procexcptest3(RDArsrc *,MTNPassableStruct *);
static void quitreport_procexcp2(RDArsrc *,HoldReport *);
static void procexcpdoexit2(RDArsrc *,MTNPassableStruct *);
static void procexcpdoexit3(RDArsrc *,MTNPassableStruct *);
static void updateallcb2(RDArsrc *,MTNPassableStruct *);
static void REPORTMasterBrowseInitFunction(MakeBrowseList *);
static short REPORTMasterBrowseBreakFunction(MakeBrowseList *);
static short REPORTMasterBrowseSelectFunction(MakeBrowseList *);
static void REPORTMasterBrowseInitFunctionLPM(MakeBrowseList *);
static short REPORTMasterBrowseBreakFunctionLPM(MakeBrowseList *);
static void REPORTBrowseQuitFunction(MakeBrowseList *);
static void readscreenTE(RDArsrc *,MTNPassableStruct *,short);
static short RPTMTN_GUIEVAL(char *,RDArsrc *,MTNPassableStruct *);
static char *RPTMTN_GUIEVALSTR(char *,RDArsrc *,void *,MTNPassableStruct *);
void process_exceptions_browseB(RDArsrc *,MTNPassableStruct *);
void process_exceptions_browseA(RDArsrc *,MTNPassableStruct *,int,void (*)(...),RDArsrc *,void *);
void process_exceptions_TableEdit(MTNPassableStruct *,int,void (*)(...),RDArsrc *,void *);
static void save_record_nupTE(RDArsrc *,MTNPassableStruct *,short);
void ClearProcExcpAccumsTE(RDArsrc *,MTNPassableStruct *);
void UpdateProcExcpAccumsTE(RDArsrc *,MTNPassableStruct *,short);
static void save_checkTE(RDArsrc *,MTNPassableStruct *);
#define TE_ChangeRow(a,b,c,d)	xTE_ChangeRow(a,b,c,d,__LINE__,__FILE__)

static void doexit_forced(MTNPassableStruct *PSTRUCT)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	int level=0,rrptlevel=0;
	
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rrpt=h->rrpt;
				level=MTNMASTER->level;
				rrptlevel=rrpt->level;
				CLOSE_MASTER_FILES(MTNMASTER);
				if(MTNMASTER->Use_Audit_Trail==TRUE)
				{
					args=APPlibNEW();
					addAPPlib(args,MTNMASTER->module);
					addAPPlib(args,MTNMASTER->Audit_Trail);
					ExecuteProgram("doreport",args);
					if(args!=NULL) freeapplib(args);
				}
				if(MTNMASTER->mbl_utlmstr!=NULL) MTNMASTER->mbl_utlmstr=NULL;
				if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
				Rfree(PSTRUCT);
				ShutdownSubsystems();
			}
		}
	}
}
static void REPORTExecuteListFunc(RDArsrc *mainrsrc,int listnum,
	void *targetkey,MTNPassableStruct *PSTRUCT,int line,char *file)
{
	APPlib *envpx=NULL,*newargs=NULL;
	MaintainButton *button=NULL;
	int x;
	char *value=NULL;
	MaintainMaster *MTNMASTER=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	MakeBrowseList *blist=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG REPORTExecuteListFunc at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(listnum<MTNMASTER->numbuttons && listnum>=0)
			{
				button=MTNMASTER->buttons+listnum;
				if(button!=NULL)
				{
					h=(HoldReport *)MTNMASTER->passparent;
					if(h!=NULL)
					{
					rrpt=h->rrpt;
					if(rrpt!=NULL)
					{
					if(mainrsrc!=NULL)
					{
					blist=MTNMASTER->mbl_utlmstr;
					if(blist!=NULL)
					{
						if(blist->definelist!=NULL)
						{
							if(EQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey)) 
							{
								ZERNRD(MTNMASTER->passkey->fileno);
#ifdef USE_RDA_DIAGNOSTICS
								if(diagmix || diagmix_field)
								{
									prterr("Error in REPORTExecuteListFunc targetkey not found at line [%d] program [%s].",line,file);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							}
							if(!MTNMASTER->file_type)
							{
								rsrc2filerecord(MTNMASTER->passkey->fileno,blist->definelist);
							} else {
								rsrc2singlefilerecord(MTNMASTER->passkey->fileno,blist->definelist);
							}
							CLEAR_SCREEN_VIRTUAL(blist->definelist);
							SetRDArsrcFiles(blist->fileno,blist->definelist);
							envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
							newargs=APPlibNEW();
							for(x=0;x<button->args->numlibs;++x)
							{
								addAPPlib(newargs,button->args->libs[x]);
							}
							if(button->evalargs==TRUE)
							{
								PSTRUCT->rsrc=blist->definelist;
								for(x=0;x<newargs->numlibs;++x)
								{
									if(!isEMPTY(newargs->libs[x]))
									{
										value=EVALUATEstr(newargs->libs[x],RPTMTNSubData,PSTRUCT);
										if(value==NULL) value=stralloc("");
										if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
										newargs->libs[x]=stralloc(value);
										if(value!=NULL) Rfree(value);
									} else {
										if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
										newargs->libs[x]=stralloc("");
									}
								}
								PSTRUCT->rsrc=rrpt->mainrsrc;
							}
							if(!RDAstrcmp(button->progname,"RUNREPORT"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNREPORTADV3(newargs->libs[0],newargs->libs[1],NULL,NULL,rrpt->APPmainLoop,2,NULL,button->func,mainrsrc,(void *)PSTRUCT,(MTNMASTER->level+1),rrpt->ProcDataLine);
			
							} else if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
							} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
							} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
							} else {
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								if(button->type==0)
								{
									ADVExecuteProgram(button->progname,newargs,envpx);
								} else {
									ADVExecute2Program(button->progname,newargs,envpx);
								}
							}
							if(newargs!=NULL) freeapplib(newargs);
						}
					}
					}
					}
					}
				}
			} else {
				prterr("Error in ExecuteListFunc Button Number [%d] not defined in Maintain Master [%s][%s] at line [%d] program [%s].",
					listnum,MTNMASTER->module,MTNMASTER->name,line,file);
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",line,file);
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",line,file);
	}
}
void REPORTButtonFunction(RDArsrc *mainrsrc,MaintainButton *button)
{
	APPlib *envpx=NULL,*newargs=NULL;
	int x;
	char *value=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;

	if(button!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)button->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				h=(HoldReport *)MTNMASTER->passparent;
				rrpt=h->rrpt;
				if(mainrsrc!=NULL)
				{
					if(!MTNMASTER->file_type)
					{
						rsrc2filerecord(MTNMASTER->passkey->fileno,mainrsrc);
					} else {
						rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mainrsrc);
					}
					ApplyActions(mainrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE);
					envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
					newargs=APPlibNEW();
					for(x=0;x<button->args->numlibs;++x)
					{
						addAPPlib(newargs,button->args->libs[x]);
					}
					if(button->evalargs==TRUE)
					{
						for(x=0;x<newargs->numlibs;++x)
						{
							if(!isEMPTY(newargs->libs[x]))
							{
								value=EVALUATEstr(newargs->libs[x],RPTMTNSubData,PSTRUCT);
								if(value==NULL) value=stralloc("");
								if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
								newargs->libs[x]=stralloc(value);
								if(value!=NULL) Rfree(value);
							} else {
								if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
								newargs->libs[x]=stralloc("");
							}
						}
					}
					if(!RDAstrcmp(button->progname,"RUNREPORT"))
					{
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNREPORTADV3(newargs->libs[0],newargs->libs[1],NULL,NULL,rrpt->APPmainLoop,2,NULL,button->func,mainrsrc,(void *)PSTRUCT,(MTNMASTER->level+1),rrpt->ProcDataLine);
					} else if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
					{
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
					} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD"))
					{
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
					} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
					{
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
					} else {
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						if(button->type==0)
						{
							ADVExecuteProgram(button->progname,newargs,envpx);
						} else {
							ADVExecute2Program(button->progname,newargs,envpx);
						}
/* remember can't free this because it is passed to the shell's environment by address and that memory used by the shell, SCO, DOS, and UNIXWARE will memoryfault sometimes- we might need to do stralloc and pass that value so we can delete tthe applib since it has more items other then the strings? */
/*
						if(envpx!=NULL) freeapplib(envpx);
*/
					}
					if(newargs!=NULL) freeapplib(newargs);
				} else {
				}
			} else { 
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	}
}
static void updateallMaintainII(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,void *v)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(rsrc!=NULL)
			{
				readallwidgets(rsrc);
				if(!ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,v,RPTMTNSubData,PSTRUCT))
				{
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					if(!MTNMASTER->file_type)
					{
						filerecord2rsrc(MTNMASTER->passkey->fileno,rsrc);
					} else {
						singlefilerecord2rsrc(MTNMASTER->passkey->fileno,rsrc);
					}
				}
				GET_SUPPORTING(rsrc,MTNMASTER,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE);
				xcomputeandupdateallSCRNvirtuals(rsrc,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
				updateallrsrc(rsrc);
				SetAllEditableAndSensitive(rsrc);
			}
		}
	}
}	
static void mtnutlmstr(short dowhich,void *targetkey,MTNPassableStruct *PSTRUCTbl,
	int level,void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromargs)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;

	if(PSTRUCTbl!=NULL)
	{
		h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
		rrpt=h->rrpt;
		rpt=h->rpt;
		MTNMASTER=PSTRUCTbl->MTNMASTER;

		PSTRUCT=Rmalloc(sizeof(MTNPassableStruct));
       		PSTRUCT->MTNMASTER=MTNMASTER;
		PSTRUCT->quitfunc=quitfunc;
		PSTRUCT->fromrsrc=fromrsrc;
		PSTRUCT->fromargs=fromargs;
		PSTRUCT->args=(void *)PSTRUCTbl;
	
		mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
		ZERNRD(MTNMASTER->passkey->fileno);
		PSTRUCT->previous=RDATDataNEW(MTNMASTER->passkey->fileno);
		MTNMASTER->passparent=(void *)h;
       		PSTRUCT->MTNMASTER=MTNMASTER;
       		PSTRUCT->rsrc=mtnrsrc;
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
				edit_rsrc=(short)MTNMASTER->editable;
				ADVnonlstmakefld(mtnrsrc,MTNMASTER->passkey->fileno,fldx,MTNMASTER->file_type,edit_rsrc);
				if(!MTNMASTER->file_type)
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
				} else {
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+1);
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+1);
					}
					sprintf(name,"%s",fldx->name);
				}
				ADVFINDRSCSETFUNC(mtnrsrc,name,SETFILES,PSTRUCT,TRUE);
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
						if(!MTNMASTER->file_type)
						{
							if(name!=NULL) 
							{ 
								name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
							} else {
								name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
							}
							sprintf(name,"[%s][%s]",MTNMASTER->mainfile,part->name);
						} else {
							if(name!=NULL) 
							{ 
								name=Rrealloc(name,RDAstrlen(part->name)+1);
							} else {
								name=Rmalloc(RDAstrlen(part->name)+1);
							}
							sprintf(name,"%s",part->name);
						}
						ADVFINDRSCSETFUNC(mtnrsrc,name,SETID,PSTRUCT,TRUE);
						FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
					}
				}
			}
		}
		if(name!=NULL) Rfree(name);
		SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
		ADVSCRNvirtual2rsrc(mtnrsrc,RPTMTNSubData,PSTRUCT);
		DefaultScreens(mtnrsrc);
       		PSTRUCT->MTNMASTER=MTNMASTER;
       		PSTRUCT->rsrc=mtnrsrc;
		ApplyActions(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE);
		switch(dowhich)
		{
			case 1:
				ApplyPassKey(MTNMASTER->passkey);
				if(EQLNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno))
				{
					KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
					ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
					if(EQLNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno))
					{
						ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
					} else {
						GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
						if(!MTNMASTER->file_type)
						{
							filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
						} else {
							singlefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
						}
					}
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					if(!MTNMASTER->file_type)
					{
						filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					} else {
						singlefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					}
				}
				break;
			case 2:
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(EQLNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					if(!MTNMASTER->file_type)
					{
						filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					} else {
						singlefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					}
				}
				break;
			default:
			case 0:
				if(EQLNRDKEY(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey))
				{
					ZERNRD(MTNMASTER->passkey->fileno);
					ApplyPassKey(MTNMASTER->passkey);
					ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
					if(EQLNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno))
					{
						ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
					} else {
						GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
						if(!MTNMASTER->file_type)
						{
							filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
						} else {
							singlefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
						}
					}
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					if(!MTNMASTER->file_type)
					{
						filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					} else {
						singlefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					}
				}
				break;
		}
		GET_SUPPORTING(mtnrsrc,MTNMASTER,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE);
		xSetVirFieldFuncs(mtnrsrc,updateallcb2,(void *)PSTRUCT,1,MTNMASTER,NULL,__LINE__,__FILE__);
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,PSTRUCT);
		if(MTNMASTER->mbl_utlmstr!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,PSTRUCT);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,PSTRUCT);
		}
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,process_exceptions_browseB,PSTRUCT);
		addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,PSTRUCT);
		if(MTNMASTER->mbl_utlmstr!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,PSTRUCT);
		} else {
			addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test3,PSTRUCT);
		}
		if(rpt->display->device<3 || rpt->display->device==5)
		{
			addbtnrsrc(mtnrsrc,"CONTINUE",TRUE,procexcpdoexit3,PSTRUCT);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE UPDATE DATA",TRUE,procexcpdoexit3,PSTRUCT);
		}
		addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
       		PSTRUCT->rsrc=mtnrsrc;
       		PSTRUCT->MTNMASTER=MTNMASTER;
		MakePrevNextButtons(mtnrsrc,keys,nokeys,PSTRUCT);
		MakeREPORTMasterButtons(mtnrsrc,PSTRUCT,NULL,__LINE__,__FILE__);
       		PSTRUCT->rsrc=mtnrsrc;
       		PSTRUCT->MTNMASTER=MTNMASTER;
		MSTADVupdateSCRNvirtuals(mtnrsrc,RPTMTNSubData,(void *)PSTRUCT,MTNMASTER);
		if(MTNMASTER->mbl_utlmstr!=NULL)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
			if(rpt->display->device<3 || rpt->display->device==5)
			{
				FINDRSCSETSENSITIVE(mtnrsrc,"CONTINUE",FALSE);
			} else {
				FINDRSCSETSENSITIVE(mtnrsrc,"SAVE UPDATE DATA",FALSE);
			}
		}
		if(!MTNMASTER->editable)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
		}
       		PSTRUCT->MTNMASTER=MTNMASTER;
		mtnrsrc->SubFunc=(void (*)(...))RPTMTNSubData;
		mtnrsrc->SubFuncArgs=PSTRUCT;
       		PSTRUCT->rsrc=mtnrsrc;
		MTNMASTER->mtnrsrc=mtnrsrc;
		h->mtnrsrc=mtnrsrc;
		if(!xmakescrn(mtnrsrc,(MTNMASTER->level==0?TRUE:TRUE),RPTMTN_GUIEVAL,(void *)PSTRUCT,addoswidgets,RPTMTN_GUIEVALSTR,(void *)PSTRUCT,RPTMTNSubData,PSTRUCT,NULL,__LINE__,__FILE__))
		{
			rrpt->screen_count+=1;
			MTNMASTER->WindowCount+=1;
		} else {
			ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom Process Exceptions MAINTAIN SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
			procexcpquitrecord2(h->mtnrsrc,PSTRUCT);
			return;
		}
		ApplyActions(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE);
		if(rrpt->APPmainLoop==FALSE)
		{
			rrpt->APPmainLoop=TRUE;	
			return;
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void resetdefs(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;
	RDArmem *mem=NULL;
	int x=0;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			ZERNRD(MTNMASTER->passkey->fileno);
			DefaultResetScreen(mtnrsrc,TRUE);
			updateallrsrc(mtnrsrc);
			x=FINDRSC(mtnrsrc,"RESET DEFAULTS");
			if(x!=(-1))
			{
				mem=mtnrsrc->rscs+x;
			} else {
				mem=mtnrsrc->rscs;
			}
			SETID(mem,PSTRUCT);
		}
	}
}
static void getpreviouscb(RDArmem *member,MTNPassableStruct *PSTRUCT)
{
	short keyno=(-1);
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
			if(keyno==(-1))
			{
				keyno=MTNMASTER->passkey->keyno;
			}
			GET_PREVIOUS(member->parent,MTNMASTER,PSTRUCT->previous,RPTMTNSubData,PSTRUCT,keyno);
		}
	}
}
static void getnextcb(RDArmem *member,MTNPassableStruct *PSTRUCT)
{
	short keyno=(-1);
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
			if(keyno==(-1))
			{
				keyno=MTNMASTER->passkey->keyno;
			}
			GET_NEXT(member->parent,MTNMASTER,PSTRUCT->previous,RPTMTNSubData,PSTRUCT,keyno);
		}
	}
}
static void SETID(RDArmem *member,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			RMEMSET_KEYFUNC(member,MTNMASTER,PSTRUCT->previous,RPTMTNSubData,(void *)PSTRUCT,MTNMASTER->inheritrsrcs);
		}
	}
}
static void SETFILES(RDArmem *member,MTNPassableStruct *PSTRUCT) 
{
	MaintainMaster *MTNMASTER=NULL;
	RDArsrc *mtnrsrc=NULL;

	if(PSTRUCT!=NULL)
	{		
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			mtnrsrc=member->parent;
			if(mtnrsrc!=NULL)
			{
				readallwidgets(mtnrsrc);
				IncVirRsrcstoFile(mtnrsrc);
				if(!MTNMASTER->file_type)
				{
					rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
					Actionrsrc2filerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
				} else {
					rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
					Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
				}
				GET_SUPPORTING(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE,FALSE);
				MSTADVupdateSCRNvirtuals(mtnrsrc,RPTMTNSubData,PSTRUCT,MTNMASTER);
				updateallrsrc(mtnrsrc);
				SetAllEditableAndSensitive(mtnrsrc);
			} else {
			}
		} else {
		}
	} else {	
	}
}
static void readscreen(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			ZERNRD(MTNMASTER->passkey->fileno);
			readallwidgets(mtnrsrc);
			if(!MTNMASTER->file_type)
			{
				rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			} else {
				rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			}
		}
	}
}
static void run_prequit_buttonsmtn(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,short update)
{
        int x;
        MaintainButton *button=NULL;
        char do_btn=FALSE;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
			{
				if(button->type==3)
				{
					do_btn=FALSE;
					if(!isEMPTY(button->expression))
					{
						do_btn=EVALUATEbol(button->expression,RPTMTNSubData,(void *)PSTRUCT);
					} else do_btn=TRUE;
					if(do_btn==TRUE)
					{
						button->func=NULL;
						button->parent=PSTRUCT;
						REPORTButtonFunction(rsrc,button);
					}
				}
			}
		}
	}
}
static void run_presave_buttons(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,short update)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
			{
				if(button->type==1)
				{
					do_btn=FALSE;
					if(!isEMPTY(button->expression))
					{
						do_btn=EVALUATEbol(button->expression,RPTMTNSubData,(void *)PSTRUCT);
					} else do_btn=TRUE;
					if(do_btn==TRUE)
					{
						button->func=NULL;
						button->parent=PSTRUCT;
						REPORTButtonFunction(rsrc,button);
					}
				}
			}
		}
	}
}
static void run_postsave_buttons(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,short update)
{
        int x;
        MaintainButton *button=NULL;
        char do_btn=FALSE;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
			{
				if(button->type==2)
				{
					do_btn=FALSE;
					if(!isEMPTY(button->expression))
					{
						do_btn=EVALUATEbol(button->expression,RPTMTNSubData,(void *)PSTRUCT);
					} else do_btn=TRUE;
					if(do_btn==TRUE)
					{
						button->func=NULL;
						button->parent=PSTRUCT;
						REPORTButtonFunction(rsrc,button);
					}
				}
			}
		}
	}
}
static void save_record_now(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	if(PSTRUCT!=NULL)
	{
		readscreen(mtnrsrc,PSTRUCT);
		run_presave_buttons(mtnrsrc,PSTRUCT,TRUE);
		save_record(mtnrsrc,PSTRUCT,TRUE);
		run_postsave_buttons(mtnrsrc,PSTRUCT,TRUE);
	}
}
static void save_record_now1(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	if(PSTRUCT!=NULL)
	{
		readscreen(mtnrsrc,PSTRUCT);
		run_presave_buttons(mtnrsrc,PSTRUCT,FALSE);
		save_record(mtnrsrc,PSTRUCT,FALSE);
		run_postsave_buttons(mtnrsrc,PSTRUCT,FALSE);
	}
}
static void save_check(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT,short update)
{
	char *desc=NULL;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(!SAVE_CHECK(mtnrsrc,MTNMASTER,RPTMTNSubData,(void *)PSTRUCT))
			{
				return;
			}
			if(!isEMPTY(MTNMASTER->save_expression))
			{
				if(EVALUATEbol(MTNMASTER->save_expression,RPTMTNSubData,PSTRUCT))
				{
					desc=EVALUATEstr(MTNMASTER->save_error_desc,RPTMTNSubData,PSTRUCT);
					ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
					if(desc!=NULL) Rfree(desc);
					return;
				}
			}
			if(!isEMPTY(MTNMASTER->save_warning))
			{
				if(EVALUATEbol(MTNMASTER->save_warning,RPTMTNSubData,PSTRUCT))
				{
					desc=EVALUATEstr(MTNMASTER->save_warning_desc,RPTMTNSubData,PSTRUCT);
					ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
						desc,(update?save_record_now:save_record_now1),
						NULL,FALSE,2,mtnrsrc,PSTRUCT,NULL);
					if(desc!=NULL) Rfree(desc);
					return;
				}
			}
			run_presave_buttons(mtnrsrc,PSTRUCT,update);
			save_record(mtnrsrc,PSTRUCT,update);
			run_postsave_buttons(mtnrsrc,PSTRUCT,update);
		}
	}
}
static void save_record_up(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	if(PSTRUCT!=NULL)
	{
		readscreen(mtnrsrc,PSTRUCT);
		save_check(mtnrsrc,PSTRUCT,TRUE);
	}
}
static void save_record_nup(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	if(PSTRUCT!=NULL)
	{
		readscreen(mtnrsrc,PSTRUCT);
		save_check(mtnrsrc,PSTRUCT,FALSE);
	}
}
static void save_record(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT,short update_list)
{
	RDATData *NewWriteData=NULL;
	MaintainMaster *MTNMASTER=NULL;
	char update=FALSE;
	void *v=NULL;
	short length=0; 

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->UsePowerWrite==TRUE)
			{
				NewWriteData=RDATDataNEW(MTNMASTER->passkey->fileno);
				RDATData2File(MTNMASTER->passkey->fileno,PSTRUCT->previous);
				if(!EQLNRD(MTNMASTER->passkey->fileno,1))
				{
					MaintainMasterPowerWrite(MTNMASTER,MTNMASTER->numPrevWrites,MTNMASTER->PrevWrites);
					/* Back out previous from accums if updatelist==FALSE */
				}
				RDATData2File(MTNMASTER->passkey->fileno,NewWriteData);
					MaintainMasterPowerWrite(MTNMASTER,MTNMASTER->numNewWrites,MTNMASTER->NewWrites);
				if(NewWriteData!=NULL) FreeRDATData(NewWriteData);
			} else {
				WRTNRD(MTNMASTER->passkey->fileno,0,NULL);
			}
			length=0;
			v=GETNRDKEY(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,&length);
			update=DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT);
			if(MTNMASTER->mbl_utlmstr!=NULL) 
			{
				if(update_list==TRUE) ClearProcExcpAccums(MTNMASTER->mbl_utlmstr);
				updatebrowse(update_list,MTNMASTER->mbl_utlmstr,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
				if(update_list==TRUE) UpdateProcExcpAccums(MTNMASTER->mbl_utlmstr);
				quit_record(mtnrsrc,PSTRUCT);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
				if(update==TRUE)
				{
					updateallMaintainII(mtnrsrc,PSTRUCT,v);
				}
			}
			if(v!=NULL) Rfree(v);
		}
	}
}
static void quit_record4(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	int level=0,rrptlevel=0;

	if(PSTRUCT!=NULL)
	{
		if(PSTRUCT->previous!=NULL) FreeRDATData(PSTRUCT->previous);
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rrpt=h->rrpt;
				level=MTNMASTER->level;
				rrptlevel=rrpt->level;
				if(MTNMASTER->WindowCount<2)
				{
					if(level!=0)
					{
/*lint -e746 */
						if(PSTRUCT->quitfunc!=NULL) PSTRUCT->quitfunc(PSTRUCT->fromrsrc,PSTRUCT->fromargs);
/*lint +e746 */
					}
/*
					CLOSE_MASTER_FILES(MTNMASTER);
*/
					if(MTNMASTER->Use_Audit_Trail==TRUE)
					{
						args=APPlibNEW();
						addAPPlib(args,MTNMASTER->module);
						addAPPlib(args,MTNMASTER->Audit_Trail);
						ExecuteProgram("doreport",args);
						if(args!=NULL) freeapplib(args);
					}
					if(mtnrsrc!=NULL)
					{
						killwindow(mtnrsrc);
						free_rsrc(mtnrsrc); 
					}
					if(PSTRUCT!=NULL) Rfree(PSTRUCT);
				}
				runprtscreen(h);
				return;
			}
		}
		if(mtnrsrc!=NULL)
		{
			killwindow(mtnrsrc);
			free_rsrc(mtnrsrc); 
		}
		if(MTNMASTER!=NULL) MTNMASTER->WindowCount-=1;
		if(PSTRUCT!=NULL) Rfree(PSTRUCT);
	}
}
static void quit_record(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	int level=0,rrptlevel=0;

	if(PSTRUCT!=NULL)
	{
		if(PSTRUCT->previous!=NULL) FreeRDATData(PSTRUCT->previous);
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rrpt=h->rrpt;
				level=MTNMASTER->level;
				rrptlevel=rrpt->level;
				if(MTNMASTER->WindowCount<2)
				{
					run_prequit_buttonsmtn(mtnrsrc,PSTRUCT,FALSE);
					if(level!=0)
					{
/*lint -e746 */
						if(PSTRUCT->quitfunc!=NULL) PSTRUCT->quitfunc(PSTRUCT->fromrsrc,PSTRUCT->fromargs);
/*lint +e746 */
					}
					CLOSE_MASTER_FILES(MTNMASTER);
					if(MTNMASTER->Use_Audit_Trail==TRUE)
					{
						args=APPlibNEW();
						addAPPlib(args,MTNMASTER->module);
						addAPPlib(args,MTNMASTER->Audit_Trail);
						ExecuteProgram("doreport",args);
						if(args!=NULL) freeapplib(args);
					}
					if(mtnrsrc!=NULL)
					{
						killwindow(mtnrsrc);
						free_rsrc(mtnrsrc); 
					}
					if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
					if(PSTRUCT!=NULL) Rfree(PSTRUCT);
					ShutdownSubsystems();
				}
			}
		}
		if(mtnrsrc!=NULL)
		{
			killwindow(mtnrsrc);
			free_rsrc(mtnrsrc); 
		}
		if(MTNMASTER!=NULL) MTNMASTER->WindowCount-=1;
/* Possible memory leak by commenting this out, needed to fix memoryfault, other stack pointers to this must be tring to use the address after it is free'ed. 
		if(PSTRUCT!=NULL) Rfree(PSTRUCT);
*/
	}
}
static void save_record_quit3(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			readscreen(mtnrsrc,PSTRUCT);
			save_check(mtnrsrc,PSTRUCT,FALSE);
			doabort_procexcptest3(mtnrsrc,PSTRUCT);
		}
	}
}
static void save_record_quit4(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL) 
			{	
				readscreen(mtnrsrc,PSTRUCT);
				save_check(mtnrsrc,PSTRUCT,FALSE);
				quit_record4(mtnrsrc,PSTRUCT);
			}
		}
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	if(PSTRUCT!=NULL)
	{
		readscreen(mtnrsrc,PSTRUCT);
		save_check(mtnrsrc,PSTRUCT,FALSE);
	}
}
static void continue_report(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL) 
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				quit_record4(mtnrsrc,PSTRUCT);
				runprtscreen(h);
			}
		}
	}
}
static void quit_record_test4(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,PSTRUCT->previous,readscreen,save_record_quit4,continue_report,PSTRUCT);
		}
	}
}
static void quit_record_test3(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,PSTRUCT->previous,readscreen,save_record_quit3,doabort_procexcptest3,PSTRUCT);
		}
	}
}
static void quit_record_test(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,PSTRUCT->previous,readscreen,save_record_quit,quit_record,PSTRUCT);
		}
	}
}
static void REPORTMasterBrowseInitFunction(MakeBrowseList *BL)
{
	RDAsearch *sup=NULL;

	if(BL!=NULL)
	{
		sup=(RDAsearch *)BL->funcargs;
		if(sup!=NULL)
		{
			PowerMasterBrowseSupInitFunction(sup);
		}
	}
}
static short REPORTMasterBrowseSelectFunction(MakeBrowseList *BL)
{
	RDAsearch *sup=NULL;
	RDAconfld *confld=NULL;
	int y,ivalue=0;
	short return_value=TRUE,shvalue=0;
	char *temp=NULL,cvalue=FALSE,*svalue=NULL;
	double fvalue=0.0;
	NRDfield *field=NULL;
	RDArsrc *rsrc=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	sup=(RDAsearch *)BL->funcargs;
	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				if(sup->parent_rsrc!=NULL)
				{
					rsrc=sup->parent_rsrc;
				} else {
					rsrc=MTNMASTER->mtnrsrc;
				}
				for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
				{
					if(return_value==FALSE) break;
					if(confld->inherit==TRUE)
					{
						field=FLDNRD(sup->fileno,confld->fldname);
						if(field!=NULL)
						{
							if(!isEMPTY(confld->conname))
							{
								temp=Rmalloc(RDAstrlen(sup->filename)+RDAstrlen(confld->fldname)+5);
								sprintf(temp,"[%s][%s]",sup->filename,confld->fldname);
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										FINDRSCGETSTRING(rsrc,temp,&svalue);
										if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
										if(svalue!=NULL) Rfree(svalue);
										break;
									case BOOLNS:
									case CHARACTERS:
										FINDRSCGETCHAR(rsrc,temp,&cvalue);
										if(*field->data.string_value!=cvalue) return_value=FALSE;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										FINDRSCGETDOUBLE(rsrc,temp,&fvalue);
										if(*field->data.float_value!=fvalue) return_value=FALSE;
										break;
									case SHORTV:
									case SSHORTV:
										FINDRSCGETSHORT(rsrc,temp,&shvalue);
										if(*field->data.short_value!=shvalue) return_value=FALSE;
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										FINDRSCGETINT(rsrc,temp,&ivalue);
										if(*field->data.integer_value!=ivalue) return_value=FALSE;
										break;
									default:
									case BUTTONS:
										break;
								}
								if(temp!=NULL) Rfree(temp);
							} else {
								temp=Rmalloc(RDAstrlen(sup->filename)+RDAstrlen(confld->fldname)+5);
								sprintf(temp,"[%s][%s]",sup->filename,confld->fldname);
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										FINDRSCGETSTRING(rsrc,temp,&svalue);
										if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
										if(svalue!=NULL) Rfree(svalue);
										break;
									case BOOLNS:
									case CHARACTERS:
										FINDRSCGETCHAR(rsrc,temp,&cvalue);
										if(*field->data.string_value!=cvalue) return_value=FALSE;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										FINDRSCGETDOUBLE(rsrc,temp,&fvalue);
										if(*field->data.float_value!=fvalue) return_value=FALSE;
										break;
									case SHORTV:
									case SSHORTV:
										FINDRSCGETSHORT(rsrc,temp,&shvalue);
										if(*field->data.short_value!=shvalue) return_value=FALSE;
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										FINDRSCGETINT(rsrc,temp,&ivalue);
										if(*field->data.integer_value!=ivalue) return_value=FALSE;
										break;
									default:
									case BUTTONS:
										break;
								}
								if(temp!=NULL) Rfree(temp);
							}
						} else {
							prterr("Error field not found for confld->fldname [%s] ",(confld->fldname!=NULL ? confld->fldname:""));
						}
					}
				}
			} else {
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error RDAsearch (Supporting Files) structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
	return(return_value);
}
static void REPORTMasterBrowseInitFunctionLPM(MakeBrowseList *BL)
{
	RDAsearch *sup=NULL;

	if(BL!=NULL)
	{
		sup=(RDAsearch *)BL->funcargs;
		if(sup!=NULL)
		{
			PowerMasterBrowseSupInitFunctionLPM(sup);
		}
	}
}
static short REPORTMasterBrowseBreakFunction(MakeBrowseList *BL)
{
	RDAsearch *sup=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArsrc *rsrc=NULL;
	char return_value=TRUE;

	sup=(RDAsearch *)BL->funcargs;
	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				if(sup->parent_rsrc!=NULL)
				{
					rsrc=sup->parent_rsrc;
				} else {
					rsrc=MTNMASTER->mtnrsrc;
				}
				if(TableEdit==TRUE) readscreenTE(rsrc,PSTRUCT,TRUE);
				else readscreen(rsrc,PSTRUCT);
				return_value=PowerMasterBrowseSupBreakFunction(sup,rsrc);
				return(return_value);
			} else {
				return(TRUE);
			}
		} else {
			return(TRUE);
		}
	} else {
		return(TRUE);
	}
}
static short REPORTMasterBrowseBreakFunctionLPM(MakeBrowseList *BL)
{
	RDAsearch *sup=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArsrc *rsrc=NULL;
	char return_value=TRUE;

	sup=(RDAsearch *)BL->funcargs;
	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				if(sup->parent_rsrc!=NULL)
				{
					rsrc=sup->parent_rsrc;
				} else {
					rsrc=MTNMASTER->mtnrsrc;
				}
				if(TableEdit==TRUE) readscreenTE(rsrc,PSTRUCT,TRUE);
				else readscreen(rsrc,PSTRUCT);
				return_value=PowerMasterBrowseSupBreakFunctionLPM(sup,rsrc);
				return(return_value);
			} else {
				return(TRUE);
			}
		} else {
			return(TRUE);
		}
	} else {
		return(TRUE);
	}
}
static void REPORTBrowseQuitFunction(MakeBrowseList *BL)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDAsearch *sup=NULL;
	RDAconfld *c=NULL;
	Browse_Exit *BE=NULL;
	char kill_browse=FALSE;
	int x;

	if(BL!=NULL)
	{
		drsrc=BL->definelist;
		srsrc=BL->searchrsrc;
		brsrc=BL->mainrsrc;
		sup=(RDAsearch *)BL->funcargs;
		if(sup!=NULL)
		{
			if(sup->confld!=NULL)
			{
				for(x=0,c=sup->confld;x<sup->num;++x,++c)
				{
					if(c->inherit==TRUE)
					{
						kill_browse=TRUE;
					}
				}
			} else {
				kill_browse=TRUE;
			}
			if(kill_browse==TRUE)
			{
				sup->BL=NULL;
			}
			if(sup->parent_rsrc!=NULL) sup->parent_rsrc=NULL;
		} else {
			kill_browse=TRUE;
		}
		if(drsrc!=NULL)
		{
			killwindow(drsrc);
			if(kill_browse==TRUE)
			{
				free_rsrc(drsrc);
				drsrc=NULL;
			}
		}
		if(srsrc!=NULL)
		{
			killwindow(srsrc);
			if(kill_browse==TRUE)
			{
				free_rsrc(srsrc);
				srsrc=NULL;
			}
		}
		if(kill_browse==TRUE)
		{
			BE=RETURN_BROWSE_EXIT(BL);
			if(BE!=NULL)
			{
				FreeBrowseExit(BE);
			}
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			if(kill_browse==TRUE)
			{
				free_rsrc(brsrc);
				brsrc=NULL;
			}
		}
	}
}
static void SetREPORTMasterNotInherited(RDArsrc *rsrc,RDAsearch *sup,RDAconfld *confld,MTNPassableStruct *PSTRUCT,NRDfield *field)
{
	char *confilename=NULL,*confieldname=NULL;
	char *mainfilename=NULL;
	RDArmem *member=NULL;
	int y=0;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->file_type==0)
			{ 	
				confilename=stripmodulename(confld->conname);
			} else {
				if(!RDAstrcmp(sup->cfilename,"SORTFILE"))
				{ 
					confilename=stralloc("SORTFILE");
						
				} else {
					confilename=stripmodulename(confld->conname);
				}
			}
			if(confilename!=NULL)
			{	
				if(MTNMASTER->file_type==0)
				{
					mainfilename=FILENAME(MTNMASTER->passkey->fileno);
				} else {
					mainfilename=stralloc(MTNMASTER->mainfile);
				}
				if(!RDAstrcmp(confilename,mainfilename))
				{
					if(MTNMASTER->file_type && !RDAstrcmp(sup->cfilename,"SORTFILE"))
					{ 
						confieldname=stralloc(confld->conname);
					} else {
						confieldname=stripfilename(confld->conname);
					}
					if(confieldname!=NULL)
					{
						if((y=FINDRSC(rsrc,confld->conname))!=(-1))
						{
							member=rsrc->rscs+y;
							switch(member->field_type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case ZIPCODE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
									if(member->value.string_value!=NULL) Rfree(member->value.string_value);
									member->dlen=0;
									if(!isEMPTY(field->data.string_value))
									{
										member->value.string_value=stralloc(field->data.string_value);
										member->dlen=RDAstrlen(member->value.string_value)+1;
									} else {
										member->value.string_value=NULL;
										member->dlen=0;
									}
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									*member->value.float_value=*field->data.float_value;
									break;
								case SHORTV:
								case SSHORTV:
									*member->value.short_value=*field->data.short_value;
									break;
								case LONGV:
								case SLONGV:
									*member->value.integer_value=*field->data.integer_value;
									break;
								case CHARACTERS:
								case BOOLNS:
									*member->value.string_value=*field->data.string_value;;
									break;
								case BUTTONS:
									member->dlen=0;
									break;
								default:
									
									prterr("Error field type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",
										member->field_type,member->rscrname,rsrc->module,rsrc->screen,__LINE__,__FILE__);
									break;
							}
							updatemember(member);
							if(member->function!=NULL)
							{
								ExecuteRDArmemFunction(member);
							}

						}
					} else {
					}
				} else {
/* Should have to worry about this case because Supporting file fields are not editable .... */
				}
			}
		}
		if(confilename!=NULL) Rfree(confilename);
		if(confieldname!=NULL) Rfree(confieldname);
	}
}
static void REPORTMasterBrowseOKFunction(MakeBrowseList *BL,void *target)
{
	Browse_Exit *BE;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDAsearch *sup=NULL;
	RDAconfld *c=NULL;
	int x;
	int y;
	NRDfield *field=NULL;
	char *temp=NULL;
	RDAconfld *confld=NULL;
	short keyno=1;
	char kill_browse=FALSE;
	RDArsrc *rsrc=NULL;
	void (*SubFunc)(...);
	void *SubFuncArgs;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG REPORTMasterBrowseOKFunction().");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(BL!=NULL)
	{
		sup=(RDAsearch *)BL->funcargs;
		if(sup!=NULL)
		{
			PSTRUCT=(MTNPassableStruct *)sup->parent;
			if(PSTRUCT!=NULL)
			{
				MTNMASTER=PSTRUCT->MTNMASTER;
				if(MTNMASTER!=NULL)
				{
					if(sup->parent_rsrc!=NULL)
					{
						rsrc=sup->parent_rsrc;
					} else {
						rsrc=MTNMASTER->mtnrsrc;
					}
					SubFunc=RPTMTNSubData;
					SubFuncArgs=PSTRUCT;
					if(sup->confld!=NULL)
					{
						for(x=0,c=sup->confld;x<sup->num;++x,++c)
						{
							if(c->inherit==TRUE) kill_browse=TRUE;
						}
					} else kill_browse=TRUE;
					if(sup->keyno!=(-1) && sup->keyno>0)
					{
						keyno=sup->keyno;
					} else {
						keyno=1;
					}
					if(!ADVEQLNRDKEYsec(sup->fileno,keyno,target,SubFunc,SubFuncArgs))
					{
						for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
						{
							if(confld->inherit==FALSE)
							{
								field=FLDNRD(sup->fileno,confld->fldname);
								if(field!=NULL)
								{
									SetREPORTMasterNotInherited(rsrc,sup,confld,PSTRUCT,field);
								}
							}
						}
					}
					IncVirRsrcstoFile(rsrc);
					GET_SUPPORTING(rsrc,MTNMASTER,SubFunc,SubFuncArgs,TRUE,FALSE);
					xcomputeandupdateallSCRNvirtuals(rsrc,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
					if(!isEMPTY(rsrc->input_focus))
					{
						temp=stralloc(rsrc->input_focus);
					}
					updateallrsrc(rsrc);
					if(!isEMPTY(temp))
					{
						FINDRSCSETINPUTFOCUS(rsrc,temp);
						if(temp!=NULL) Rfree(temp);
					}
					SetAllEditableAndSensitive(rsrc);
					if(kill_browse==TRUE)
					{
						sup->BL=NULL;
					}
				} else {
					prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
				}
			} else {
				prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else { 
			kill_browse=TRUE;
		}
		drsrc=BL->definelist;
		srsrc=BL->searchrsrc;
		brsrc=BL->mainrsrc;
		if(drsrc!=NULL)
		{
			killwindow(drsrc);
			if(kill_browse==TRUE)
			{
				free_rsrc(drsrc);
				drsrc=NULL;
			}
		}
		if(srsrc!=NULL)
		{
			killwindow(srsrc);
			if(kill_browse==TRUE)
			{
				free_rsrc(srsrc);
				srsrc=NULL;
			}
		}
		if(kill_browse==TRUE)
		{
			BE=RETURN_BROWSE_EXIT(BL);
			if(BE!=NULL)
			{
				FreeBrowseExit(BE);
			}
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			if(kill_browse==TRUE)
			{
				free_rsrc(brsrc);
				brsrc=NULL;
			}
		}
	}
}
static void REPORTMasterBrowseFunction(RDArmem *member,RDAsearch *sup)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	short keyno=1;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	char kill_browse=FALSE;

	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{	
				sup->parent_rsrc=(RDArsrc *)member->parent;
				if(sup->BL!=NULL)
				{
					if(sup->smart_browse==TRUE)
					{
						kill_browse=PowerMasterBrowseTestInherited(sup->BL,member);
						if(kill_browse==FALSE)
						{
							if(APPmakescrn(sup->BL->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
							{
								REPORTBrowseQuitFunction(sup->BL);
							}
							return;
						} else {
							PowerMasterBrowseFreeFunction(sup->BL);
							sup->BL=NULL;
						}
					}
				} else {
					PowerMasterBrowseFreeFunction(sup->BL);
					sup->BL=NULL;
				}
				if(!isEMPTY(sup->browse_screen) && !isEMPTY(sup->define_list))
				{
					brsrc=RDArsrcNEW(MTNMASTER->module,sup->browse_screen);
					GET_SCREEN_VIRTUALS(brsrc,0);
/*
					SCRNvirtual2rsrc(brsrc);
*/
					ADVSCRNvirtual2rsrc(brsrc,RPTMTNSubData,PSTRUCT);
					drsrc=RDArsrcNEW(MTNMASTER->module,sup->define_list);
					addDFincvir(drsrc,sup->module,sup->filename,NULL,sup->fileno);
					GET_SCREEN_VIRTUALS(drsrc,1);
					srsrc=RDArsrcNEW(MTNMASTER->module,sup->search_browse);
					addDFincvir(srsrc,sup->module,sup->filename,NULL,sup->fileno);
					GET_SCREEN_VIRTUALS(srsrc,2);
/*
					SCRNvirtual2rangersrc(brsrc);
*/
					ADVSCRNvirtual2rsrc(brsrc,RPTMTNSubData,PSTRUCT);
					if(sup->keyno!=(-1) && sup->keyno>0)
					{
						keyno=sup->keyno;
					} else {
						keyno=1;
					}
					switch(sup->smart_browse)
					{
						case 0:
						case 1:
							sup->BL=xbrowse(sup->fileno,keyno,brsrc,srsrc,drsrc,REPORTMasterBrowseInitFunction,REPORTMasterBrowseBreakFunction,REPORTMasterBrowseSelectFunction,REPORTBrowseQuitFunction,REPORTMasterBrowseOKFunction,NULL,sup,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
							break;
						case 2:
							sup->BL=xbrowse(sup->fileno,keyno,brsrc,srsrc,drsrc,REPORTMasterBrowseInitFunction,REPORTMasterBrowseBreakFunction,REPORTMasterBrowseSelectFunction,REPORTBrowseQuitFunction,REPORTMasterBrowseOKFunction,NULL,sup,TRUE,NULL,0,0,0,0,NULL,0,2,__LINE__,__FILE__);
							break;
						case 3:
							sup->BL=xbrowse(sup->fileno,keyno,brsrc,srsrc,drsrc,REPORTMasterBrowseInitFunctionLPM,REPORTMasterBrowseBreakFunctionLPM,REPORTMasterBrowseSelectFunction,REPORTBrowseQuitFunction,REPORTMasterBrowseOKFunction,NULL,sup,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
							break;
					}
				}
			} else {
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	}
}
static void MakeREPORTMasterButtons(RDArsrc *mainrsrc,
	MTNPassableStruct *PSTRUCT,void (*quitfunc)(...),int line,char *file)
{
	int x,y;
	MaintainButton *button=NULL;
	MaintainMaster *MTNMASTER=NULL;
	char *bname=NULL;
	RDAsearch *sup=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MakeMasterButtons at line [%d] program [%s].",
			line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->buttons!=NULL)
			{
				for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
				{
					button->parent=PSTRUCT;
					if(!RDAstrcmp(button->progname,"RUNREPORT"))
					{
						button->func=quitfunc;
					} else  if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
					{
						button->func=quitfunc;
					} else  if(!RDAstrcmp(button->progname,"MULTIPOWERADD"))
					{
						button->func=quitfunc;
					} else  if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
					{
						button->func=quitfunc;
					}
					addbtnrsrc(mainrsrc,button->buttonname,TRUE,
						REPORTButtonFunction,button);
				}
			}
			if(MTNMASTER->s!=NULL)
			{
				for(y=0,sup=MTNMASTER->s;y<MTNMASTER->num;++y,++sup)
				{
					if((sup->browse==TRUE) && !isEMPTY(sup->browse_screen) && !isEMPTY(sup->define_list))
					{
						if(!isEMPTY(sup->button_name))
						{
							bname=stralloc(sup->button_name);
						} else {
							bname=Rmalloc(strlen(sup->module)+strlen(sup->filename)+25);
							sprintf(bname,"BROWSE %s %s %d",(sup->module!=NULL?sup->module:""),(sup->filename!=NULL?sup->filename:""),(y+1));
						}
						sup->parent=PSTRUCT;
						sup->parent_rsrc=mainrsrc;
						RMEMaddbtnrsrc(mainrsrc,bname,TRUE,REPORTMasterBrowseFunction,sup);
						if(bname!=NULL) Rfree(bname);
					}
				}
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}

void process_exceptions_browseA(RDArsrc *parent,MTNPassableStruct *PSTRUCT,int level,void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromargs)
{
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDAaccum *a=NULL;
	NRDfield *field=NULL;
	int x=0;
	RDAvirtual *v=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG process_exceptions Process Exceptions on Report [%s]'s sort file [%s].",rpt->name,FILENAME(rrpt->sortno));  
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCT!=NULL)
	{
	MTNMASTER=PSTRUCT->MTNMASTER;
	if(MTNMASTER!=NULL)
	{
	h=(HoldReport *)MTNMASTER->passparent;
	if(h!=NULL)
	{
		rrpt=h->rrpt;
		rpt=h->rpt;
		if(rpt!=NULL)
		{
			if(parent!=NULL) free_rsrc(parent);
			rrpt->mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
			rrpt->searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
			rrpt->definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);

			MTNMASTER->mbl_utlmstr=NULL;
			MTNMASTER->WindowCount=0;
			MTNMASTER->editable=TRUE;
			MTNMASTER->level=0;
			MTNMASTER->SubFunc=RPTMTNSubData;
			MTNMASTER->SubFuncArgs=PSTRUCT;
			MTNMASTER->passparent=(void *)h;
			PSTRUCT->MTNMASTER=MTNMASTER;
			PSTRUCT->quitfunc=quitfunc;
			PSTRUCT->fromrsrc=fromrsrc;
			PSTRUCT->fromargs=fromargs;
			PSTRUCT->args=NULL;
			PSTRUCT->MTNMASTER=MTNMASTER;
			PSTRUCT->rsrc=rrpt->mainrsrc;
			PSTRUCT->MTNMASTER=MTNMASTER;
			ZERNRD(MTNMASTER->passkey->fileno);
			GET_BROWSE_SUPPORTING(rrpt->mainrsrc,rrpt->searchrsrc,rrpt->definelist,MTNMASTER,RPTMTNSubData,(void *)PSTRUCT);
			GET_SCREEN_VIRTUALS(rrpt->mainrsrc,0);
			GET_SCREEN_VIRTUALS(rrpt->definelist,1);
			GET_SCREEN_VIRTUALS(rrpt->searchrsrc,2);
			MakeRunFunction(rrpt->mainrsrc,MTNMASTER);
			if(rrpt->mainrsrc->scn!=NULL) free_scrn(rrpt->mainrsrc->scn);
			rrpt->mainrsrc->scn=RDAscrnNEW(rrpt->mainrsrc->module,rrpt->mainrsrc->screen);
			if(findscrnbin(rrpt->mainrsrc->scn))
			{
				prterr("Error couldn't read Screen Binary [%s] [%s] from Binary Library [%s.SCN]",rrpt->mainrsrc->module,(rrpt->mainrsrc->screen!=NULL ? rrpt->mainrsrc->screen:""),rrpt->mainrsrc->module);
				free_scrn(rrpt->mainrsrc->scn);
				rrpt->mainrsrc->scn=NULL;
			} else {
				if(rpt->virflds!=NULL)
				{
					for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
					{
						if(v->range==2 || v->range==3)
						{
							makefieldrsrc(rrpt->mainrsrc,v->name,v->type,v->len,FALSE);
							switch(v->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case ZIPCODE:
									FINDRSCSETSTRING(rrpt->mainrsrc,v->name,v->value.string_value);
									break;
								case BOOLNS:
								case CHARACTERS:
									FINDRSCSETCHAR(rrpt->mainrsrc,v->name,*v->value.string_value);
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									FINDRSCSETDOUBLE(rrpt->mainrsrc,v->name,*v->value.float_value);
									break;
								case SHORTV:
								case SSHORTV:
									FINDRSCSETSHORT(rrpt->mainrsrc,v->name,*v->value.short_value);
									break;
								case LONGV:
								case SLONGV:
								case SCROLLEDLIST:
									FINDRSCSETINT(rrpt->mainrsrc,v->name,*v->value.integer_value);
									break;
								default:
								case BUTTONS:
									break;
							}
						}
					}
				}
				rrpt->no_pe_accums=TRUE;
				if(rpt->accum!=NULL)
				{
					for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
					{
						a->displayed=FALSE;
						if((FINDWDGT(rrpt->mainrsrc->scn,a->name))!=(-1)) 
						{
							if(!alreadyrsrc(a->name,rrpt->mainrsrc))
							{
								a->displayed=TRUE;
								rrpt->no_pe_accums=FALSE;
								
								if(a->atype==1)
								{
									makefieldrsrc(rrpt->mainrsrc,a->name,LONGV,8,FALSE);
								} else if(a->atype==7)
								{
									field=FLDNRD(rrpt->sortno,a->accname);
									if(field!=NULL)
									{
										switch(field->type)
										{
											case SCROLLEDTEXT:
											case VARIABLETEXT:
											case DATES:
											case TIMES:
											case SOCSECNUM:
											case PLAINTEXT:
											case PHONE:
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
											case ZIPCODE:
												break;
											case BOOLNS:
											case CHARACTERS:
												makefieldrsrc(rrpt->mainrsrc,a->name,LONGV,8,FALSE);
												break;
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												makefieldrsrc(rrpt->mainrsrc,a->name,field->type,field->len,FALSE);
												break;
											case SHORTV:
											case SSHORTV:
												makefieldrsrc(rrpt->mainrsrc,a->name,field->type,field->len,FALSE);
												break;
											case LONGV:
											case SLONGV:
											case SCROLLEDLIST:
												makefieldrsrc(rrpt->mainrsrc,a->name,field->type,field->len,FALSE);
												break;
											default:
											case BUTTONS:
												break;
										}
									}
								}
							}
						}
					}
				}
			}
			MTNMASTER->SubFunc=RPTMTNSubData;
			MTNMASTER->SubFuncArgs=PSTRUCT;
			if(rpt->display->device<3 || rpt->display->device==5)
			{
				addbtnrsrc(rrpt->mainrsrc,"CONTINUE",TRUE,procexcpdoexit2,PSTRUCT);
			} else {
				addbtnrsrc(rrpt->mainrsrc,"SAVE",TRUE,procexcpdoexit2,PSTRUCT);
			}
			xMakeREPORTMasterBrowseButtons(rrpt->mainrsrc,PSTRUCT,NULL,__LINE__,__FILE__);
			rrpt->definelist->scn=RDAscrnNEW(rrpt->definelist->module,rrpt->definelist->screen);
			findscrnbin(rrpt->definelist->scn);
			rrpt->mbl=START_ADVbrowse(MTNMASTER->passkey->fileno,
				MTNMASTER->passkey->keyno,
				rrpt->mainrsrc,
				rrpt->searchrsrc,
				rrpt->definelist,
				NULL,
				procexcpselectfunction2,
				doabort_procexcptest2,
				procexcpokfunc2,
				procexcpnewrecord2,
				PSTRUCT,
				(MTNMASTER->level==0?TRUE:TRUE),
				NULL,
				0,0,0,MTNMASTER->file_type,
				MTNMASTER->start_where);
			MTNMASTER->mbl_utlmstr=rrpt->mbl;
			if(MTNMASTER->mbl_utlmstr!=NULL)
			{
				MTNMASTER->WindowCount+=1;
				rrpt->screen_count+=1;
			}
			if(rrpt->APPmainLoop==FALSE)
			{
				rrpt->APPmainLoop=TRUE;	
				return;
			}
		} else {
			prterr("Error RDAreport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
	} else {
		prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
	} else {
	}
}
void process_exceptions_browseB(RDArsrc *parent,MTNPassableStruct *PSTRUCT)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCTbl=NULL;
	void (*quitfunc)(...)=NULL;
	RDArsrc *fromrsrc=NULL;
	void *fromargs=NULL;
	RDAreport *rpt=NULL;
	int level=0;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(PSTRUCT->previous!=NULL) FreeRDATData(PSTRUCT->previous);
		if(MTNMASTER!=NULL) 
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rpt=h->rpt;
				if(rpt!=NULL)
				{
					quitfunc=PSTRUCT->quitfunc;
					fromrsrc=PSTRUCT->fromrsrc;
					fromargs=PSTRUCT->fromargs;
					level=MTNMASTER->level;
					if(PSTRUCT->args!=NULL) PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
					Rfree(PSTRUCT);
					process_exceptions_browseA(parent,PSTRUCTbl,level,quitfunc,fromrsrc,fromargs);
				}
			}
		}
	}
}

void process_exceptions2(HoldReport *h)
{
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;
	void *targetkey=NULL;
	short r=FALSE,ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG process_exceptions Process Exceptions on Report [%s]'s sort file [%s].",rpt->name,FILENAME(rrpt->sortno));  
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(h!=NULL)
	{
		rrpt=h->rrpt;
		rpt=h->rpt;
		if(rpt!=NULL)
		{
			MTNMASTER=rpt->MTNMSTR;
			if(MTNMASTER!=NULL)
			{
			MTNMASTER->passkey=READPassKey(rrpt->sortno);
			MTNMASTER->passkey->fileno=rrpt->sortno;
			MTNMASTER->passkey->keyno=1;
			MTNMASTER->passparent=(void *)h;

			MTNMASTER->mbl_utlmstr=NULL;
			MTNMASTER->WindowCount=0;
			MTNMASTER->editable=TRUE;
			MTNMASTER->level=0;
			MTNMASTER->SubFunc=RPTMTNSubData;
			MTNMASTER->SubFuncArgs=PSTRUCT;
			MTNMASTER->passparent=(void *)h;
			PSTRUCT=Rmalloc(sizeof(MTNPassableStruct));
			PSTRUCT->MTNMASTER=MTNMASTER;
			PSTRUCT->quitfunc=NULL;
			PSTRUCT->fromrsrc=NULL;
			PSTRUCT->fromargs=NULL;
			PSTRUCT->args=NULL;
			PSTRUCT->MTNMASTER=MTNMASTER;
			PSTRUCT->rsrc=rrpt->mainrsrc;

			if(!OPEN_MASTER_FILES(MTNMASTER))
			{
				doexit_forced(PSTRUCT);
				return;
			}
			PSTRUCT->MTNMASTER=MTNMASTER;
			MTNMASTER->SubFunc=RPTMTNSubData;
			MTNMASTER->SubFuncArgs=PSTRUCT;
			TableEdit=FALSE;
			switch(MTNMASTER->start_with)
			{
				default:
				case 1:
					process_exceptions_browseA(NULL,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
					break;
				case 0:
					ZERNRD(MTNMASTER->passkey->fileno);
					ef=FRSNRD(MTNMASTER->passkey->fileno,1);
					if(!ef)
					{
						targetkey=GETNRDKEY((short)MTNMASTER->passkey->fileno,(short)MTNMASTER->passkey->keyno,&r);
						mtnexcpts2(0,targetkey,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
						if(targetkey!=NULL) Rfree(targetkey);
					} else {
						targetkey=NULL;
						mtnexcpts2(1,NULL,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
					}
					break;
				case 2: /* Table Edit */
					process_exceptions_TableEdit(PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
					break;
			}
			} else {
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error RDAreport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static short RPTMTN_GUIEVAL(char *string,RDArsrc *r,MTNPassableStruct *PSTRUCT)
{
	short tmp=FALSE;

	if(PSTRUCT!=NULL)
	{
		if(!isEMPTY(string))
		{
			tmp=EVALUATEbol(string,RPTMTNSubData,(void *)PSTRUCT);
		} else {
			tmp=TRUE;
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
	return(tmp);
}
static char *RPTMTN_GUIEVALSTR(char *string,RDArsrc *r,void *SubFuncArgs,MTNPassableStruct *PSTRUCT)
{
	char *tmp=NULL;

	if(PSTRUCT!=NULL)
	{
		if(!isEMPTY(string))
		{
			tmp=PP_EVALUATEstr(string,RPTMTNSubData,(void *)PSTRUCT);
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
	return(tmp);
}
static void quitreport_procexcp2(RDArsrc *parent,HoldReport *h)
{
	RDAreport *rpt=NULL;
	RDArunrpt *rrpt=NULL;

	if(h!=NULL)
	{
		rrpt=h->rrpt;
		rpt=h->rpt;
		if(rrpt!=NULL && rpt!=NULL)
		{
			exit_cleanly(rrpt,rpt,FALSE,NULL,NULL,TRUE,1);
			Rfree(h);
		} else {
		}
	} else {
	}
}
static void doabort_procexcptest3(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	HoldReport *h=NULL;
	char *warnmessage=NULL;
 
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rrpt=h->rrpt;
				rpt=h->rpt;
				if(rpt!=NULL)
				{
					if(rrpt->sortno!=(-1))
					{
						warnmessage=Rmalloc(RDAstrlen(rrpt->module)+RDAstrlen(rpt->name)+157+1);
						sprintf(warnmessage,"Are you sure that you want to quit/abort this report without processing this data?\n  !!!ALL DATA PROCESSED WILL BE LOST IF ABORTED!!!\nModule: [%s] Report: [%s]",
							(rrpt->module==NULL?"":rrpt->module),
							(rpt->name=NULL?"":rpt->name));
						PSTRUCT->MTNMASTER=MTNMASTER;
						ADVWARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!",
							warnmessage,abort_procexcp3,
							NULL,FALSE,2,mtnrsrc,PSTRUCT,NULL);
						if(warnmessage!=NULL) Rfree(warnmessage);
					} else {
						abort_procexcp3(mtnrsrc,PSTRUCT);
					}
				}
			} else {
				prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void doabort_procexcptest2(MakeBrowseList *blist)
{
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	HoldReport *h=NULL;
	char *warnmessage=NULL;
 
	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				h=(HoldReport *)MTNMASTER->passparent;
				if(h!=NULL)
				{
					rrpt=h->rrpt;
					rpt=h->rpt;
					if(rpt!=NULL)
					{
						if(rrpt->sortno!=(-1))
						{
							warnmessage=Rmalloc(RDAstrlen(rrpt->module)+RDAstrlen(rpt->name)+160+1);
							sprintf(warnmessage,"Are you sure that you want to quit/abort this report without processing this data?\n  !!!ALL DATA PROCESSED WILL BE LOST IF ABORTED!!!\nModule: [%s] Report: [%s]",
								(rrpt->module==NULL?"":rrpt->module),
								(rpt->name=NULL?"":rpt->name));
							WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!",
								warnmessage,abort_procexcp2,NULL,FALSE,blist);
							if(warnmessage!=NULL) Rfree(warnmessage);
						} else {
							abort_procexcp2(blist);
						}
					}
				} else {
					prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
				}
			} else {
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error BrowseList structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void abort_procexcp3(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;
	HoldReport *h=NULL;
	RDArsrc *rsrc=NULL;
	RDArunrpt *rrpt=NULL;
	int level=0,rrptlevel=0;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rrpt=h->rrpt;
				rrptlevel=rrpt->level;
				level=MTNMASTER->level;
				rsrc=MTNMASTER->mtnrsrc;
				if(rsrc!=NULL) 
				{
					killwindow(rsrc);
					free_rsrc(rsrc);
					rsrc=NULL;
					MTNMASTER->mtnrsrc=NULL;
				}
				rrpt->screen_count-=1; 
				if(rrptlevel==0) 
				{
					quitreport(rsrc,h);
					if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
					if(PSTRUCT!=NULL) Rfree(PSTRUCT);
					ShutdownSubsystems();
				}
			} else {
				prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void abort_procexcp2(MakeBrowseList *blist)
{
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	int level=0,rrptlevel=0;
	Browse_Exit *BE=NULL;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				h=(HoldReport *)MTNMASTER->passparent;
				if(h!=NULL)
				{
					rrpt=h->rrpt;
					rrptlevel=rrpt->level;
					level=MTNMASTER->level;
					drsrc=blist->definelist;
					srsrc=blist->searchrsrc;
					brsrc=blist->mainrsrc;
					BE=RETURN_BROWSE_EXIT(blist);
					if(drsrc!=NULL) 
					{
						killwindow(drsrc);
						free_rsrc(drsrc);
						drsrc=NULL;
						rrpt->definelist=NULL;
					}
					if(srsrc!=NULL) 
					{
						killwindow(srsrc);
						free_rsrc(srsrc);
						srsrc=NULL;
						rrpt->searchrsrc=NULL;
					}
					if(brsrc!=NULL)
					{
						killwindow(brsrc);
						free_rsrc(brsrc);
						brsrc=NULL;
						rrpt->mainrsrc=NULL;
						rrpt->screen_count-=1; 
					}
					if(rrptlevel==0) 
					{
						quitreport(brsrc,h);
						FreeBrowseExit(BE);
						if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
						if(PSTRUCT!=NULL) Rfree(PSTRUCT);
						ShutdownSubsystems();
					}
				} else {
					prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
				}
			} else {
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error BrowseList structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
void UpdateProcExcpAccums(MakeBrowseList *blist)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	int x=0;
	RDAaccum *a=NULL;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rpt=h->rpt;
				rrpt=h->rrpt;
				if(rrpt->no_pe_accums==TRUE) return;
				if(rpt->accum!=NULL)
				{
					for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
					{
						if(a->displayed==TRUE)
						{
							if(a->atype==1)
							{
								updatersrc(rrpt->mainrsrc,a->name);
							} else if(a->atype==7)
							{
								updatersrc(rrpt->mainrsrc,a->name);
							}
						}
					}
				}
			}
		}
	}
}
void ClearProcExcpAccums(MakeBrowseList *blist)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	NRDfield *field=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	int x=0,wr=0;
	RDAaccum *a=NULL;
	RDArmem *member=NULL;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			if(PSTRUCT->MTNMASTER!=NULL)
			{
			MTNMASTER=PSTRUCT->MTNMASTER;
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rpt=h->rpt;
				rrpt=h->rrpt;
				if(rrpt->no_pe_accums==TRUE) return;
				if(rpt->accum!=NULL)
				{
					for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
					{
						if(a->displayed==TRUE && rrpt->mainrsrc!=NULL)
						{
							wr=FINDRSC(rrpt->mainrsrc,a->name);
							if(wr!=(-1))
							{
							member=rrpt->mainrsrc->rscs+wr;
							if(a->atype==1)
							{
								*member->value.integer_value=0;
							} else if(a->atype==7)
							{
								field=FLDNRD(rrpt->sortno,a->accname);
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										break;
									case BOOLNS:
									case CHARACTERS:
										member->value.integer_value=0;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										*member->value.float_value=0;
										break;
									case SHORTV:
									case SSHORTV:
										*member->value.short_value=0;
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										*member->value.integer_value=0;
										break;
									default:
									case BUTTONS:
										break;
								}
							}
							}
						}
					}
				}
			}
			}
		}
	}
}
static short procexcpselectfunction2(MakeBrowseList *blist)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	NRDfield *field=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	int x=0,wr=0;
	RDAaccum *a=NULL;
	RDArmem *member=NULL;
	char delflag=FALSE,dskip=FALSE;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rpt=h->rpt;
				rrpt=h->rrpt;
				FINDFLDGETCHAR(rrpt->sortno,"DELETEFLAG",&delflag);
				FINDFLDGETCHAR(rrpt->sortno,"[DEFAULT SKIP]",&dskip);
				if(rpt->accum!=NULL && !delflag && !dskip)
				{
					for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
					{
						if(a->displayed==TRUE)
						{
							wr=FINDRSC(rrpt->mainrsrc,a->name);
							if(wr!=(-1))
							{
							member=rrpt->mainrsrc->rscs+wr;
							if(a->atype==1)
							{
								*member->value.integer_value+=1;
							} else if(a->atype==7)
							{
								field=FLDNRD(rrpt->sortno,a->accname);
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										break;
									case BOOLNS:
									case CHARACTERS:
										member->value.integer_value+=1;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										*member->value.float_value+=*field->data.float_value;
										break;
									case SHORTV:
									case SSHORTV:
										*member->value.short_value+=*field->data.short_value;
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										*member->value.integer_value+=*field->data.integer_value;
										break;
									default:
									case BUTTONS:
										break;
								}
							}
							}
						}
					}
				}
			}
		}
	}
	return(TRUE);
}
static void procexcpdoexit3(RDArsrc *parent,MTNPassableStruct *PSTRUCT)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;


#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpdoexit3 Exiting Process Exceptions.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				quit_record_test4(parent,PSTRUCT);
			} else {
				prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void procexcpdoexit2(RDArsrc *parent,MTNPassableStruct *PSTRUCTbl)
{
	RDArunrpt *rrpt=NULL;
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;


#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpdoexit2 Exiting Process Exceptions.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCTbl!=NULL)
	{
		MTNMASTER=PSTRUCTbl->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rrpt=h->rrpt;
				if(rrpt->definelist!=NULL) 
				{
					killwindow(rrpt->definelist);
					free_rsrc(rrpt->definelist);
					rrpt->definelist=NULL;
				}
				if(rrpt->searchrsrc!=NULL) 
				{
					killwindow(rrpt->searchrsrc);
					free_rsrc(rrpt->searchrsrc);
					rrpt->searchrsrc=NULL;
				}
				if(rrpt->mainrsrc!=NULL) 
				{
					killwindow(rrpt->mainrsrc);
					free_rsrc(rrpt->mainrsrc);
				}
				if(MTNMASTER!=NULL) MTNMASTER->WindowCount-=1;
				if(PSTRUCTbl!=NULL) Rfree(PSTRUCTbl);
				rrpt->mainrsrc=NULL;
				runprtscreen(h);
			} else {
				prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void procexcpokfunc2(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
			{
				if(!run_func)
				{ 
					mtnexcpts2(0,targetkey,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
				} else {
					REPORTExecuteListFunc(blist->mainrsrc,run_func-1,targetkey,PSTRUCT,__LINE__,__FILE__);
				}
			} else {
				mtnexcpts2(0,targetkey,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
/*
				mtnexcpts2(targetkey,(MTNPassableStruct *)blist->funcargs);
*/
			}
		}
	}
}
static void okREPORTBrowseButtonFunc(RDArsrc *brsrc,MaintainButton *button)
{
	MakeBrowseList *blist=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDAbrowse *list=NULL;
	char **key=NULL;
	int sellist=0;

	if(brsrc!=NULL)
	{
		if(button!=NULL)
		{
			PSTRUCT=button->parent;
			if(PSTRUCT!=NULL)
			{
				MTNMASTER=PSTRUCT->MTNMASTER;
				blist=MTNMASTER->mbl_utlmstr;
				if(blist!=NULL) 
				{
					list=blist->list;
					key=(char **)list->key;
					FINDRSCGETINT(blist->mainrsrc,"BROWSE LIST",&sellist);
					if(sellist>=list->num)
					{
						prterr("Error: Selected Browse Element [%d] isn't in actual Browse List.  Resetting list to first element in Browse List, element number ZERO (0).",sellist);
					} else { 
						list->keyvalue=key[sellist];
						REPORTExecuteBrowseButtonFunc(brsrc,button,list->keyvalue,PSTRUCT);
					}
				}
			}
		}
	}
}
void REPORTExecuteBrowseButtonFunc(RDArsrc *brsrc,MaintainButton *button,void *targetkey,MTNPassableStruct *PSTRUCT)
{
	APPlib *envpx=NULL,*newargs=NULL;
	int x;
	char *value=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MakeBrowseList *blist=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;

	if(button!=NULL)
	{
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				h=(HoldReport *)MTNMASTER->passparent;
				if(h!=NULL)
				{
				rrpt=h->rrpt;
				if(rrpt!=NULL)
				{
				if(brsrc!=NULL)
				{
					blist=MTNMASTER->mbl_utlmstr;
					if(blist!=NULL)
					{
						if(blist->definelist!=NULL)
						{
							if(EQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey)) 
							{
								ZERNRD(MTNMASTER->passkey->fileno);
#ifdef USE_RDA_DIAGNOSTICS
								if(diagmix || diagmix_field)
								{
									prterr("Error in REPORTExecuteBrowseButtonFunc targetkey not found at line [%d] program [%s].",__LINE__,__FILE__);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							}
							if(!MTNMASTER->file_type)
							{
								rsrc2filerecord(MTNMASTER->passkey->fileno,blist->definelist);
							} else {
								rsrc2singlefilerecord(MTNMASTER->passkey->fileno,blist->definelist);
							}
							CLEAR_SCREEN_VIRTUAL(blist->definelist);
							SetRDArsrcFiles(blist->fileno,blist->definelist);
							envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
							newargs=APPlibNEW();
							for(x=0;x<button->args->numlibs;++x)
							{
								addAPPlib(newargs,button->args->libs[x]);
							}
							if(button->evalargs==TRUE)
							{
								PSTRUCT->rsrc=blist->definelist;
								for(x=0;x<newargs->numlibs;++x)
								{
									if(!isEMPTY(newargs->libs[x]))
									{
										value=EVALUATEstr(newargs->libs[x],RPTMTNSubData,(void *)PSTRUCT);
										if(value==NULL) value=stralloc("");
										if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
										newargs->libs[x]=stralloc(value);
										if(value!=NULL) Rfree(value);
									} else {
										if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
										newargs->libs[x]=stralloc("");
									}
								}
								PSTRUCT->rsrc=rrpt->mainrsrc;
							}
							if(!RDAstrcmp(button->progname,"RUNREPORT"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNREPORTADV3(newargs->libs[0],newargs->libs[1],NULL,NULL,rrpt->APPmainLoop,2,NULL,button->func,brsrc,(void *)PSTRUCT,(MTNMASTER->level+1),rrpt->ProcDataLine);
							} else if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
							} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
							} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)PSTRUCT,rrpt->APPmainLoop);
							} else {
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								if(button->type==0)
								{
									ADVExecuteProgram(button->progname,newargs,envpx);
								} else {
									ADVExecute2Program(button->progname,newargs,envpx);
								}
/* remember can't free this because it is passed to the shell's environment by address and that memory used by the shell, SCO, DOS, and UNIXWARE will memoryfault sometimes- we might need to do stralloc and pass that value so we can delete tthe applib since it has more items other then the strings? */
/*
								if(envpx!=NULL) freeapplib(envpx);
*/
							}
							if(newargs!=NULL) freeapplib(newargs);
						}
					}
				}
				}
				} else {
				}
			} else { 
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	}
}
void xMakeREPORTMasterBrowseButtons(RDArsrc *brsrc,MTNPassableStruct *PSTRUCT,
	void (*quitfunc)(...),int line,char *file)
{
	int x;
	MaintainButton *button=NULL;
	MaintainMaster *MTNMASTER=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MakeREPORTMasterBrowseButtons at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->buttons!=NULL)
			{
				for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
				{
					button->parent=PSTRUCT;
					if(!RDAstrcmp(button->progname,"RUNREPORT"))
					{
						button->func=quitfunc;
					} else  if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
					{
						button->func=quitfunc;
					} else  if(!RDAstrcmp(button->progname,"MULTIPOWERADD"))
					{
						button->func=quitfunc;
					} else  if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
					{
						button->func=quitfunc;
					}
					addbtnrsrc(brsrc,button->buttonname,TRUE,
						okREPORTBrowseButtonFunc,button);
				}
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void procexcpnewrecord2(RDArsrc *parent,MakeBrowseList *blist,void *targetkey)
{
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				mtnexcpts2(2,targetkey,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
			} else {
			}
		} else {
		}
	} else {
	}
} 
static void updateallcb2(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArsrc *rsrc=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rsrc=PSTRUCT->rsrc;
				if(rsrc!=NULL)
				{
					rrpt=h->rrpt;
					readallwidgets(rsrc);
					rsrc2singlefilerecord(rrpt->sortno,rsrc);
					xcomputeandupdateallSCRNvirtuals(rsrc,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
					updateallrsrc(rsrc);
					SetAllEditableAndSensitive(rsrc);
				} else {
					prterr("Error RDArsrc structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
				}
			} else {
				prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void mtnexcpts2(short dowhich,void *targetkey,MTNPassableStruct *PSTRUCTbl,int level,void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromargs)
{
	MaintainMaster *MTNMASTER=NULL;
	HoldReport *h=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG mtnexcpts2 Display Maintain Screen for Process Exceptions."); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCTbl!=NULL)
	{
		MTNMASTER=PSTRUCTbl->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				mtnutlmstr(dowhich,targetkey,PSTRUCTbl,level,quitfunc,fromrsrc,fromargs);
			} else {
				prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
/*
static void save_no_update2(RDArsrc *parent,MaintainMaster *MTNMASTER)
{
	procexcpsaverecord2(parent,MTNMASTER,FALSE);
}
static void save_updatew2(RDArsrc *parent,MaintainMaster *MTNMASTER)
{
	procexcpsaverecord2(parent,MTNMASTER,TRUE);
}
void procexcpsaverecord2(RDArsrc *parent,MaintainMaster *MTNMASTER,short update)
{
	char *rcddesc=NULL;
	RDArunrpt *rrpt=NULL;
	HoldReport *h=NULL;

	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpsaverecord2 Process Exceptions Save Record Function from Screen [%s] [%s].",parent->module,parent->screen);
	}
	if(MTNMASTER!=NULL)
	{
		h=(HoldReport *)MTNMASTER->passparent;
		if(h!=NULL)
		{
			rrpt=h->rrpt;
			readallwidgets(h->mtnrsrc);
			if(!MTNMASTER->file_type)
			{
				rsrc2filerecord(rrpt->sortno,h->mtnrsrc);
			} else {
				rsrc2singlefilerecord(rrpt->sortno,h->mtnrsrc);
			}
			if(RECORDsec(rrpt->sortno))
			{
				rcddesc=RECORDsecDesc(rrpt->sortno);
				if(rcddesc!=NULL)
				{
					ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
					Rfree(rcddesc);
				}
				prterr("Error didn't write record.");
			} else {
				WRTNRD(rrpt->sortno,0,NULL);
				if(update==TRUE) ClearProcExcpAccums(MTNMASTER->mbl_utlmstr);
				updatebrowse(update,rrpt->mbl,rrpt->sortno,rrpt->mainrsrc->module,rrpt->mbl->mainrsrc);
				if(update==TRUE) UpdateProcExcpAccums(MTNMASTER->mbl_utlmstr);
			}
			procexcpquitrecord2(parent,PSTRUCT);
		} else {
			prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
  	} else {
		prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
*/
static void procexcpquitrecord2(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	int level=0,rrptlevel=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpquitrecord2 Quit Process Exceptions Maintain Screen [%s] [%s] function.",mtnrsrc->module,mtnrsrc->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCT!=NULL)
	{
		if(PSTRUCT->previous!=NULL) FreeRDATData(PSTRUCT->previous);
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
		h=(HoldReport *)MTNMASTER->passparent;
		if(h!=NULL)
		{
			rrpt=h->rrpt;
			h->rrpt->screen_count-=1;
			rrptlevel=rrpt->level;
			level=MTNMASTER->level;
			if(MTNMASTER->WindowCount<2)
			{
				run_prequit_buttonsmtn(mtnrsrc,PSTRUCT,FALSE);
				if(level!=0)
				{
/*lint -e746 */
					if(PSTRUCT->quitfunc!=NULL) PSTRUCT->quitfunc(PSTRUCT->fromrsrc,PSTRUCT->fromargs);
/*lint +e746 */
				}
				CLOSE_MASTER_FILES(MTNMASTER);
				if(MTNMASTER->Use_Audit_Trail==TRUE)
				{
					args=APPlibNEW();
					addAPPlib(args,MTNMASTER->module);
					addAPPlib(args,MTNMASTER->Audit_Trail);
					ExecuteProgram("doreport",args);
					if(args!=NULL) freeapplib(args);
				}
				if(mtnrsrc!=NULL)
				{
					killwindow(mtnrsrc);
					free_rsrc(mtnrsrc); 
				}
				if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
				if(PSTRUCT!=NULL) Rfree(PSTRUCT);
				ShutdownSubsystems();
			}
			h->mtnrsrc=NULL;
		} else {
			prterr("Error HoldReport structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
		if(mtnrsrc!=NULL)
		{
			killwindow(mtnrsrc);
			free_rsrc(mtnrsrc); 
		}
		if(MTNMASTER!=NULL) MTNMASTER->WindowCount-=1;
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
		if(PSTRUCT!=NULL) Rfree(PSTRUCT);
	} else {
		prterr("Error MTNPassableStruct structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
/* TABLE EDIT */
void TE_CleanTable(RDArsrc *r)
{
	int x=0;
	RDArmem *member=NULL;

	if(r!=NULL)
	{
		for(x=0,member=r->rscs;x<r->numrscs;++x,++member)
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(member->value.string_value))
					{
						memset(member->value.string_value,0,member->dlen);
					}
					break;
				case BOOLNS:
				case CHARACTERS:
					*member->value.string_value=0;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*member->value.float_value=0.0;
					break;
				case SHORTV:
				case SSHORTV:
					*member->value.short_value=0;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*member->value.integer_value=0;
					break;
				case BUTTONS:
				default:
					break;
			}
			updatersrc(r,member->rscrname);
		}
	}
}
static short TE_SelectFunc(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	MTNPassableStruct *PSTRUCTbl=NULL;
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	NRDfield *field=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	int x=0,wr=0;
	RDAaccum *a=NULL;
	RDArmem *member=NULL;
	char delflag=FALSE,dskip=FALSE;

	PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
	h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
	rrpt=h->rrpt;
	rpt=h->rpt;
	MTNMASTER=PSTRUCT->MTNMASTER;
	if(h!=NULL)
	{
		FINDFLDGETCHAR(rrpt->sortno,"DELETEFLAG",&delflag);
		FINDFLDGETCHAR(rrpt->sortno,"[DEFAULT SKIP]",&dskip);
		if(rpt->accum!=NULL && !delflag && !dskip)
		{
			for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
			{			
				if(a->displayed==TRUE)
				{
					wr=FINDRSC(mtnrsrc,a->name);
					if(wr!=(-1))
					{
					member=mtnrsrc->rscs+wr;
					if(a->atype==1)
					{
						*member->value.integer_value+=1;
					} else if(a->atype==7)
					{
						field=FLDNRD(rrpt->sortno,a->accname);
						switch(field->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								break;
							case BOOLNS:
							case CHARACTERS:
								member->value.integer_value+=1;
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								*member->value.float_value+=*field->data.float_value;
								break;
							case SHORTV:
							case SSHORTV:
								*member->value.short_value+=*field->data.short_value;
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								*member->value.integer_value+=*field->data.integer_value;
								break;
							default:
							case BUTTONS:
								break;
						}
					}
					}
				}
			}
		}
	}
	return(TRUE);
}
void TableEditfilerecord2rsrc(short fileno,RDArsrc *rsrc,int row,char current)
{
	short x=0,nofields=0,y=0,atype=0,dtype=0;
	NRDfield *field=NULL,*fields=NULL;
	RDArmem *res;
	char tempx[512];

	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		if(RDAstrstr(field->name,"ACCOUNT TYPE")) atype=*field->data.short_value;
		else if(RDAstrstr(field->name,"DEFINITION TYPE")) dtype=*field->data.short_value;
	}
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		memset(tempx,0,512);
		sprintf(tempx,"%04d:%s",row+1,field->name);
		y=FINDRSC(rsrc,tempx);
		if(y>(-1))
		{
			res=rsrc->rscs+y;
			MEMBERSETEDITABLE(res,TRUE);
			MEMBERSETSENSITIVE(res,TRUE);
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(RDAstrstr(field->name,"ACCOUNT CODE"))
					{
						MEMBERSETACCTTYPE(res,atype);
						MEMBERSETDEFTYPE(res,dtype);
					} else if(field->type==BALANCESHEET)
					{
						MEMBERSETDEFTYPE(res,dtype);
					}
					if(!isEMPTY(field->data.string_value))
					{
						QUICKALLOC(res->value.string_value,res->dlen,field->dlen);
						memcpy(res->value.string_value,field->data.string_value,field->dlen);
					} else if(res->dlen>0)
					{
						memset(res->value.string_value,0,res->dlen);
					} else {
						res->value.string_value=NULL;
						res->dlen=0;
					}
					break;
				case SHORTV:
				case SSHORTV:
					*res->value.short_value=*field->data.short_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*res->value.integer_value=*field->data.integer_value;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					*res->value.float_value=*field->data.float_value;
					break;
				case BOOLNS:
				case CHARACTERS:
					*res->value.string_value=
						*field->data.string_value;
					break;
				default:
					break;
			}
		}
		if(current)
		{
			y=FINDRSC(rsrc,field->name);
			if(y>(-1))
			{
				res=rsrc->rscs+y;
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(!isEMPTY(field->data.string_value))
						{
							QUICKALLOC(res->value.string_value,res->dlen,field->dlen);
							memcpy(res->value.string_value,field->data.string_value,field->dlen);
						} else if(res->dlen>0)
						{
							memset(res->value.string_value,0,res->dlen);
						} else {
							res->value.string_value=NULL;
							res->dlen=0;
						}
						break;
					case SHORTV:
					case SSHORTV:
						*res->value.short_value=*field->data.short_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						*res->value.integer_value=*field->data.integer_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*res->value.float_value=*field->data.float_value;
						break;
					case BOOLNS:
					case CHARACTERS:
						*res->value.string_value=
							*field->data.string_value;
						break;
					default:
						break;
				}
			}
		}
	}
}
static void xTE_ChangeRow(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT,short Readit,short which,int line,char *file)
{
	MTNPassableStruct *PSTRUCTbl=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	short x=0,nofields=0,y=0;
	NRDfield *field=NULL,*fields=NULL;
	RDArmem *res;
	char tempx[512];

	if(diagrptgen)
	{
		prterr("DIAG TE_ChangeRow Readit [%d] Which [%d] at line [%d] program [%s].",Readit,which,line,file);
	}
	PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
	h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
	rrpt=h->rrpt;
	rpt=h->rpt;
	nofields=NUMFLDS(rpt->MTNMSTR->passkey->fileno);
	fields=FLDPOINTER(rpt->MTNMSTR->passkey->fileno);
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		memset(tempx,0,512);
		sprintf(tempx,"%04d:%s",rrpt->table_row+1,field->name);
		if(which==FALSE)
		{
			if(Readit) readwidget(mtnrsrc,tempx);
			y=FINDRSC(mtnrsrc,tempx);
			if(y>(-1))
			{
				res=mtnrsrc->rscs+y;
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						FINDRSCSETSTRING(mtnrsrc,field->name,res->value.string_value);
						break;
					case SHORTV:
					case SSHORTV:
						FINDRSCSETSHORT(mtnrsrc,field->name,*res->value.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						FINDRSCSETINT(mtnrsrc,field->name,*res->value.integer_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						FINDRSCSETDOUBLE(mtnrsrc,field->name,*res->value.float_value);
						break;
					case BOOLNS:
					case CHARACTERS:
						FINDRSCSETCHAR(mtnrsrc,field->name,*res->value.string_value);
						break;
					default:
						break;
				}
				updatersrc(mtnrsrc,field->name);
			}
		} else {
			if(Readit)
			{
				readwidget(mtnrsrc,field->name);
			}
			y=FINDRSC(mtnrsrc,field->name);
			if(y>(-1))
			{
				res=mtnrsrc->rscs+y;
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						y=FINDRSCSETSTRING(mtnrsrc,tempx,res->value.string_value);
						break;
					case SHORTV:
					case SSHORTV:
						y=FINDRSCSETSHORT(mtnrsrc,tempx,*res->value.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						y=FINDRSCSETINT(mtnrsrc,tempx,*res->value.integer_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						y=FINDRSCSETDOUBLE(mtnrsrc,tempx,*res->value.float_value);
						break;
					case BOOLNS:
					case CHARACTERS:
						y=FINDRSCSETCHAR(mtnrsrc,tempx,*res->value.string_value);
						break;
					default:
						break;
				}
				if(y>(-1)) updatersrc(mtnrsrc,tempx);
			}
		}
	}
}
static void SETID1(RDArmem *member,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCTbl=NULL;
	RDArsrc *mtnrsrc=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;

	mtnrsrc=(RDArsrc *)member->parent;
	PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
	h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
	rrpt=h->rrpt;
	rpt=h->rpt;

	SETFILES1(member,PSTRUCT);
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			RMEMSET_KEYFUNC(member,MTNMASTER,PSTRUCT->previous,RPTMTNSubData,(void *)PSTRUCT,MTNMASTER->inheritrsrcs);
		}
	}
	rrpt->lastwidget=member;
}
static void SETFILES1(RDArmem *member,MTNPassableStruct *PSTRUCT) 
{
	MaintainMaster *MTNMASTER=NULL;
	RDArsrc *mtnrsrc=NULL;
	int row=0,row1=0;
	MTNPassableStruct *PSTRUCTbl=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
/*
	short y=0;
*/

	PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
	h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
	rrpt=h->rrpt;
	rpt=h->rpt;
	mtnrsrc=(RDArsrc *)member->parent;
	memset(stemp,0,101);
	sprintf(stemp,"%.4s",member->rscrname);
	row1=atoi(stemp);
	if(member==rrpt->lastwidget) return;
	
	if(member->field_type!=BUTTONS)
	{
		memset(stemp,0,101);
		sprintf(stemp,"%.4s",rrpt->lastwidget->rscrname);
		row=atoi(stemp);
		if(row>0 && row<=REPLICATE && row1>0)
		{
			rrpt->table_row=row-1;
			readwidget(mtnrsrc,rrpt->lastwidget->rscrname);
			memset(stemp,0,101);
			sprintf(stemp,"%s",&rrpt->lastwidget->rscrname[5]);
			switch(rrpt->lastwidget->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					FINDRSCSETSTRING(mtnrsrc,stemp,rrpt->lastwidget->value.string_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDRSCSETCHAR(mtnrsrc,stemp,*rrpt->lastwidget->value.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					FINDRSCSETDOUBLE(mtnrsrc,stemp,*rrpt->lastwidget->value.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDRSCSETSHORT(mtnrsrc,stemp,*rrpt->lastwidget->value.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					FINDRSCSETINT(mtnrsrc,stemp,*rrpt->lastwidget->value.integer_value);
					break;
				case BUTTONS:
				default:
					break;
			}
			updatersrc(mtnrsrc,stemp);
			save_record_nupTE(mtnrsrc,PSTRUCT,FALSE);
			if(PSTRUCT!=NULL)
			{		
				MTNMASTER=PSTRUCT->MTNMASTER;
				if(MTNMASTER!=NULL)
				{
					mtnrsrc=(RDArsrc *)member->parent;
					if(mtnrsrc!=NULL)
					{
						readallwidgets(mtnrsrc);
						IncVirRsrcstoFile(mtnrsrc);
						if(!MTNMASTER->file_type)
						{
							rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
							Actionrsrc2filerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
						} else {
							rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
							Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
						}
						GET_SUPPORTING(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE,FALSE);
						MSTADVupdateSCRNvirtuals(mtnrsrc,RPTMTNSubData,PSTRUCT,MTNMASTER);
						updateallrsrc(mtnrsrc);
						SetAllEditableAndSensitive(mtnrsrc);
					} else {
					}
				} else {
				}
			} else {	
			}
			memset(stemp,0,101);
			sprintf(stemp,"%.4s",member->rscrname);
			row=atoi(stemp);
			if(row!=(rrpt->table_row+1) && row>0 && row<=REPLICATE)
			{
				rrpt->table_row=row-1;
				TE_ChangeRow(mtnrsrc,PSTRUCT,TRUE,FALSE);
				if(!MTNMASTER->file_type)
				{
					rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
					Actionrsrc2filerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
				} else {
					rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
					Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
				}
				GET_SUPPORTING(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE,FALSE);
				MSTADVupdateSCRNvirtuals(mtnrsrc,RPTMTNSubData,PSTRUCT,MTNMASTER);
				updateallrsrc(mtnrsrc);
				SetAllEditableAndSensitive(mtnrsrc);
			}
		} else {
			save_record_nupTE(mtnrsrc,PSTRUCT,TRUE);
			updateallrsrc(mtnrsrc);
		}
		rrpt->lastwidget=member;
		memset(stemp,0,101);
		sprintf(stemp,"%.4s",member->rscrname);
		row=atoi(stemp);
		if(row>0 && row<=REPLICATE)
		{
			rrpt->table_row=row-1;
			FINDRSCSETINT(mtnrsrc,"CURRENT ROW",row);
			updatersrc(mtnrsrc,"CURRENT ROW");
			rrpt->table_row=row-1;
			TE_ChangeRow(mtnrsrc,PSTRUCT,TRUE,FALSE);
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(!MTNMASTER->file_type)
			{
				rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
				Actionrsrc2filerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
			} else {
				rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
				Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
			}
			GET_SUPPORTING(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE,FALSE);
			MSTADVupdateSCRNvirtuals(mtnrsrc,RPTMTNSubData,PSTRUCT,MTNMASTER);
			updateallrsrc(mtnrsrc);
			SetAllEditableAndSensitive(mtnrsrc);
		} else {
			MTNMASTER=PSTRUCT->MTNMASTER;
			row=rrpt->table_row+1;
			FINDRSCSETINT(mtnrsrc,"CURRENT ROW",row);
			updatersrc(mtnrsrc,"CURRENT ROW");
			TE_ChangeRow(mtnrsrc,PSTRUCT,FALSE,FALSE);
			readallwidgets(mtnrsrc);
			IncVirRsrcstoFile(mtnrsrc);
			if(!MTNMASTER->file_type)
			{
				rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
				Actionrsrc2filerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
			} else {
				rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);	
				Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,RPTMTNSubData,PSTRUCT);
			}
			GET_SUPPORTING(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE,FALSE);
			MSTADVupdateSCRNvirtuals(mtnrsrc,RPTMTNSubData,PSTRUCT,MTNMASTER);
			updateallrsrc(mtnrsrc);
			SetAllEditableAndSensitive(mtnrsrc);
		}
	}
}
/*
static void ChangePage(RDArmem *mem,MTNPassableStruct *PSTRUCT)
{
	MTNPassableStruct *PSTRUCTbl=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	RDArsrc *r=NULL;

	r=(RDArsrc *)mem->parent;
	PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
	h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
	rrpt=h->rrpt;
	rpt=h->rpt;
	SETFILES(mem,PSTRUCT);
	readwidget(r,"PAGE");
	FINDRSCGETINT(r,"PAGE",&rrpt->page);
	TE_CleanTable(r);
}
*/
static void ChangePreviousPage(RDArsrc *r,MTNPassableStruct *PSTRUCT)
{
	MTNPassableStruct *PSTRUCTbl=NULL;
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	RDArmem *res=NULL;
	short ef=0;
	NRDkey *keys=NULL,*key=NULL;
	NRDpart *part;
	char tempx[512];
	short x=0,y=0;
	int w=0;

	MTNMASTER=PSTRUCT->MTNMASTER;
	PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
	h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
	rrpt=h->rrpt;
	rpt=h->rpt;
	save_record_nupTE(r,PSTRUCT,FALSE);
	readwidget(r,"PAGE");
	FINDRSCGETINT(r,"PAGE",&rrpt->page);
	--rrpt->page;
	if(rrpt->page<rrpt->total_pages)
	{
		FINDRSCSETSENSITIVE(r,"NEXT PAGE",TRUE);
	} else if(rrpt->page==rrpt->total_pages)
	{
		FINDRSCSETSENSITIVE(r,"NEXT PAGE",FALSE);
	}
	if(rrpt->page>1)
	{
		FINDRSCSETSENSITIVE(r,"PREVIOUS PAGE",TRUE);
	} else if(rrpt->page==1)
	{
		FINDRSCSETSENSITIVE(r,"PREVIOUS PAGE",FALSE);
	}
	ClearProcExcpAccumsTE(r,PSTRUCT);
	ZERNRD(MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	key=keys;
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		memset(tempx,0,512);
		sprintf(tempx,"%04d:%s",1,part->name);
		readwidget(r,tempx);
		y=FINDRSC(r,tempx);
		if(y>(-1))
		{
			res=r->rscs+y;
			switch(res->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					FINDFLDSETSTRING(MTNMASTER->passkey->fileno,part->name,res->value.string_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDFLDSETSHORT(MTNMASTER->passkey->fileno,part->name,*res->value.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					FINDFLDSETINT(MTNMASTER->passkey->fileno,part->name,*res->value.integer_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					FINDFLDSETDOUBLE(MTNMASTER->passkey->fileno,part->name,*res->value.float_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDFLDSETCHAR(MTNMASTER->passkey->fileno,part->name,*res->value.string_value);
					break;
				default:
					break;
			}
		}
	}
	TE_CleanTable(r);
	ef=EQLNRD(MTNMASTER->passkey->fileno,1);
	ef=PRVNRD(MTNMASTER->passkey->fileno,1);
	rrpt->table_row=0;
	w=1;
	FINDRSCSETINT(r,"TOTAL PAGES",rrpt->total_pages);
	FINDRSCSETINT(r,"CURRENT ROW",w);
	updatersrc(r,"CURRENT ROW");
	w=REPLICATE;
	while(!ef)
	{
		if(TE_SelectFunc(r,PSTRUCT))
		{
			--w;
			GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
			TableEditfilerecord2rsrc(MTNMASTER->passkey->fileno,r,w,(w==0 ? TRUE:FALSE));
			if(w==0) break;
		}
		ef=PRVNRD(MTNMASTER->passkey->fileno,1);
	}

	UpdateProcExcpAccumsTE(r,PSTRUCT,TRUE);

	FINDRSCSETINT(r,"PAGE",rrpt->page);
	xcomputeandupdateallSCRNvirtuals(r,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
	updateallrsrc(r);
}
static void ChangeNextPage(RDArsrc *r,MTNPassableStruct *PSTRUCT)
{
	MTNPassableStruct *PSTRUCTbl=NULL;
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	RDArmem *res=NULL;
	short ef=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*key=NULL;
	NRDpart *part;
	char tempx[512];
	short x=0,y=0,nofields=0;
	int w=0;

	MTNMASTER=PSTRUCT->MTNMASTER;
	PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
	h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
	rrpt=h->rrpt;
	rpt=h->rpt;
	save_record_nupTE(r,PSTRUCT,FALSE);
	readwidget(r,"PAGE");
	FINDRSCGETINT(r,"PAGE",&rrpt->page);
	++rrpt->page;
	if(rrpt->page<rrpt->total_pages)
	{
		FINDRSCSETSENSITIVE(r,"NEXT PAGE",TRUE);
	} else if(rrpt->page==rrpt->total_pages)
	{
		FINDRSCSETSENSITIVE(r,"NEXT PAGE",FALSE);
	}
	if(rrpt->page>1)
	{
		FINDRSCSETSENSITIVE(r,"PREVIOUS PAGE",TRUE);
	} else if(rrpt->page==1)
	{
		FINDRSCSETSENSITIVE(r,"PREVIOUS PAGE",FALSE);
	}
	ClearProcExcpAccumsTE(r,PSTRUCT);
	ZERNRD(MTNMASTER->passkey->fileno);
	nofields=NUMFLDS(MTNMASTER->passkey->fileno);
	fields=FLDPOINTER(MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	key=keys;
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		memset(tempx,0,512);
		sprintf(tempx,"%04d:%s",REPLICATE,part->name);
		readwidget(r,tempx);
		y=FINDRSC(r,tempx);
		if(y>(-1))
		{
			res=r->rscs+y;
			switch(res->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					FINDFLDSETSTRING(MTNMASTER->passkey->fileno,part->name,res->value.string_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDFLDSETSHORT(MTNMASTER->passkey->fileno,part->name,*res->value.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					FINDFLDSETINT(MTNMASTER->passkey->fileno,part->name,*res->value.integer_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					FINDFLDSETDOUBLE(MTNMASTER->passkey->fileno,part->name,*res->value.float_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDFLDSETCHAR(MTNMASTER->passkey->fileno,part->name,*res->value.string_value);
					break;
				default:
					break;
			}
		}
	}
	TE_CleanTable(r);
	ef=EQLNRD(MTNMASTER->passkey->fileno,1);
	ef=NXTNRD(MTNMASTER->passkey->fileno,1);
	rrpt->table_row=0;
	w=1;
	FINDRSCSETINT(r,"TOTAL PAGES",rrpt->total_pages);
	FINDRSCSETINT(r,"CURRENT ROW",w);
	updatersrc(r,"CURRENT ROW");
	w=0;
	while(!ef)
	{
		if(TE_SelectFunc(r,PSTRUCT))
		{
			GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
			TableEditfilerecord2rsrc(MTNMASTER->passkey->fileno,r,w,(w==0 ? TRUE:FALSE));
			++w;
			if(w>REPLICATE) break;
		}
		ef=NXTNRD(MTNMASTER->passkey->fileno,1);
	}
	++w;
	if(w<=REPLICATE)
	{
		for(;w<=REPLICATE;++w)
		{
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				memset(tempx,0,512);
				sprintf(tempx,"%04d:%s",w,fldx->name);
				FINDRSCSETEDITABLE(r,tempx,FALSE);
				FINDRSCSETSENSITIVE(r,tempx,FALSE);
			}
		}
	}
	UpdateProcExcpAccumsTE(r,PSTRUCT,TRUE);
	FINDRSCSETINT(r,"PAGE",rrpt->page);
	xcomputeandupdateallSCRNvirtuals(r,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
	updateallrsrc(r);
}
void UpdateProcExcpAccumsTE(RDArsrc *r,MTNPassableStruct *PSTRUCT,short gather)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	int x=0;
	RDAaccum *a=NULL;
	short ef=0;

	MTNMASTER=PSTRUCT->MTNMASTER;
	h=(HoldReport *)MTNMASTER->passparent;
	if(h!=NULL)
	{
		rpt=h->rpt;
		rrpt=h->rrpt;
		if(rrpt->no_pe_accums==TRUE) return;
		if(rpt->accum!=NULL)
		{
			if(gather)
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ef=FRSNRD(MTNMASTER->passkey->fileno,1);
				while(!ef)
				{
					TE_SelectFunc(r,PSTRUCT);
					ef=NXTNRD(MTNMASTER->passkey->fileno,1);
				}
			}
			for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
			{
				if(a->displayed==TRUE)
				{
					if(a->atype==1)
					{
						updatersrc(r,a->name);
					} else if(a->atype==7)
					{
						updatersrc(r,a->name);
					}
				}
			}
		}
	}
}
void ClearProcExcpAccumsTE(RDArsrc *r,MTNPassableStruct *PSTRUCT)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	NRDfield *field=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	int x=0,wr=0;
	RDAaccum *a=NULL;
	RDArmem *member=NULL;

	if(PSTRUCT->MTNMASTER!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		h=(HoldReport *)MTNMASTER->passparent;
		if(h!=NULL)
		{
			rpt=h->rpt;
			rrpt=h->rrpt;
			if(rrpt->no_pe_accums==TRUE) return;
			if(rpt->accum!=NULL)
			{
				for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
				{
					if(a->displayed==TRUE && r!=NULL)
					{
						wr=FINDRSC(r,a->name);
						if(wr!=(-1))
						{
							member=r->rscs+wr;
							if(a->atype==1)
							{
								*member->value.integer_value=0;
							} else if(a->atype==7)
							{
								field=FLDNRD(rrpt->sortno,a->accname);
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										break;
									case BOOLNS:
									case CHARACTERS:
										member->value.integer_value=0;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										*member->value.float_value=0;
										break;
									case SHORTV:
									case SSHORTV:
										*member->value.short_value=0;
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										*member->value.integer_value=0;
										break;
									default:
									case BUTTONS:
										break;
								}
							}
							updatersrc(r,a->name);
						}
					}
				}
			}
		}
	}
}
static void save_recordTE(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	RDATData *NewWriteData=NULL;
	MaintainMaster *MTNMASTER=NULL;
	char update=FALSE;
	void *v=NULL;
	short length=0; 

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->UsePowerWrite==TRUE)
			{
				NewWriteData=RDATDataNEW(MTNMASTER->passkey->fileno);
				RDATData2File(MTNMASTER->passkey->fileno,PSTRUCT->previous);
				if(!EQLNRD(MTNMASTER->passkey->fileno,1))
				{
					MaintainMasterPowerWrite(MTNMASTER,MTNMASTER->numPrevWrites,MTNMASTER->PrevWrites);
				}
				RDATData2File(MTNMASTER->passkey->fileno,NewWriteData);
					MaintainMasterPowerWrite(MTNMASTER,MTNMASTER->numNewWrites,MTNMASTER->NewWrites);
				if(NewWriteData!=NULL) FreeRDATData(NewWriteData);
			} else {
				WRTNRD(MTNMASTER->passkey->fileno,0,NULL);
			}
			length=0;
			v=GETNRDKEY(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,&length);
			update=DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT);
			ClearProcExcpAccumsTE(mtnrsrc,PSTRUCT);
			UpdateProcExcpAccumsTE(mtnrsrc,PSTRUCT,TRUE);
			GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
			if(update==TRUE)
			{
				updateallMaintainII(mtnrsrc,PSTRUCT,v);
			}
			if(v!=NULL) Rfree(v);
		}
	}
}
static void save_checkTE(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	char *desc=NULL;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(!SAVE_CHECK(mtnrsrc,MTNMASTER,RPTMTNSubData,(void *)PSTRUCT))
			{
				return;
			}
			if(!isEMPTY(MTNMASTER->save_expression))
			{
				if(EVALUATEbol(MTNMASTER->save_expression,RPTMTNSubData,PSTRUCT))
				{
					desc=EVALUATEstr(MTNMASTER->save_error_desc,RPTMTNSubData,PSTRUCT);
					ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
					if(desc!=NULL) Rfree(desc);
					return;
				}
			}
			if(!isEMPTY(MTNMASTER->save_warning))
			{
				if(EVALUATEbol(MTNMASTER->save_warning,RPTMTNSubData,PSTRUCT))
				{
					desc=EVALUATEstr(MTNMASTER->save_warning_desc,RPTMTNSubData,PSTRUCT);
					ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
						desc,save_record_now1,NULL,FALSE,2,mtnrsrc,PSTRUCT,NULL);
					if(desc!=NULL) Rfree(desc);
					return;
				}
			}
			run_presave_buttons(mtnrsrc,PSTRUCT,FALSE);
			save_recordTE(mtnrsrc,PSTRUCT);
			run_postsave_buttons(mtnrsrc,PSTRUCT,FALSE);
		}
	}
}
static void readscreenTE(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT,short which)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		h=(HoldReport *)MTNMASTER->passparent;
		rpt=h->rpt;
		rrpt=h->rrpt;
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			ZERNRD(MTNMASTER->passkey->fileno);
			readallwidgets(mtnrsrc);
			TE_ChangeRow(mtnrsrc,PSTRUCT,FALSE,(which==0 ? FALSE:TRUE));
			rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			ApplyActions(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,FALSE);
			Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,rrpt->lastwidget,MTNMASTER,RPTMTNSubData,PSTRUCT);
			if(which==0)
			{
				TE_ChangeRow(mtnrsrc,PSTRUCT,FALSE,TRUE);
			} else {
				TE_ChangeRow(mtnrsrc,PSTRUCT,FALSE,TRUE);
				updateallrsrc(mtnrsrc);
			}
		}
	}
}
static void save_record_nupTE(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT,short which)
{
	if(PSTRUCT!=NULL)
	{
		readscreenTE(mtnrsrc,PSTRUCT,which);
		save_checkTE(mtnrsrc,PSTRUCT);
	}
}
static void TE_Continue(RDArsrc *r,MTNPassableStruct *PSTRUCT)
{
	HoldReport *h=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	int row=0;

	MTNMASTER=PSTRUCT->MTNMASTER;
	h=(HoldReport *)MTNMASTER->passparent;
	rpt=h->rpt;
	rrpt=h->rrpt;
	memset(stemp,0,101);
	sprintf(stemp,"%.4s",rrpt->lastwidget->rscrname);
	row=atoi(stemp);
	if(row>0 && row<=REPLICATE)
	{
		save_record_nupTE(r,PSTRUCT,FALSE);
	} else {
		save_record_nupTE(r,PSTRUCT,TRUE);
	}
	procexcpdoexit3(r,PSTRUCT);
}
void process_exceptions_TableEdit(MTNPassableStruct *PSTRUCTbl,int level,void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromargs)
{
	char tempx[512],foundit=FALSE;
	short nofields=0,nokeys=0,x=0,y=0,ef=0;
	int w=0;
	double recs=0.0;
	NRDfield *fields=NULL,*fldx=NULL,*field=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	RDAaccum *a=NULL;
	RDAwdgt *wdgt=NULL;
	RDArmem *mem=NULL;
	HoldReport *h=NULL;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;

	if(PSTRUCTbl!=NULL)
	{
		h=(HoldReport *)PSTRUCTbl->MTNMASTER->passparent;
		rrpt=h->rrpt;
		rpt=h->rpt;
		MTNMASTER=PSTRUCTbl->MTNMASTER;

		mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
		if(mtnrsrc->scn!=NULL) free_scrn(mtnrsrc->scn);
		mtnrsrc->scn=RDAscrnNEW(mtnrsrc->module,mtnrsrc->screen);
		if(findscrnbin(mtnrsrc->scn))
		{
			prterr("Error couldn't read Screen Binary [%s] [%s] from Binary Library [%s.SCN]",mtnrsrc->module,(mtnrsrc->screen!=NULL ? mtnrsrc->screen:""),mtnrsrc->module);
			free_scrn(mtnrsrc->scn);
			mtnrsrc->scn=NULL;
			return;
		}

		TableEdit=TRUE;
		PSTRUCT=Rmalloc(sizeof(MTNPassableStruct));
       		PSTRUCT->MTNMASTER=MTNMASTER;
		PSTRUCT->quitfunc=quitfunc;
		PSTRUCT->fromrsrc=fromrsrc;
		PSTRUCT->fromargs=fromargs;
		PSTRUCT->args=(void *)PSTRUCTbl;
	
		ZERNRD(MTNMASTER->passkey->fileno);
		PSTRUCT->previous=RDATDataNEW(MTNMASTER->passkey->fileno);
		MTNMASTER->passparent=(void *)h;
       		PSTRUCT->MTNMASTER=MTNMASTER;
       		PSTRUCT->rsrc=mtnrsrc;
		nokeys=NUMKEYS(MTNMASTER->passkey->fileno);
		keys=KEYPOINTER(MTNMASTER->passkey->fileno);
		nofields=NUMFLDS(MTNMASTER->passkey->fileno);
		fields=FLDPOINTER(MTNMASTER->passkey->fileno);
		GET_MAINTAIN_INCVARS(mtnrsrc,MTNMASTER);
		GET_SCREEN_VIRTUALS(mtnrsrc,0);
		rrpt->table_row=0;
		if(fields!=NULL)
		{
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				makefieldrsrc(mtnrsrc,fldx->name,fldx->type,fldx->len,MTNMASTER->editable);
				ADVFINDRSCSETFUNC(mtnrsrc,fldx->name,SETFILES1,PSTRUCT,TRUE);
				for(y=1;y<(REPLICATE+1);++y)
				{
					memset(tempx,0,512);
					sprintf(tempx,"%04d:%s",y,fldx->name);
					makefieldrsrc(mtnrsrc,tempx,fldx->type,fldx->len,MTNMASTER->editable);
					ADVFINDRSCSETFUNC(mtnrsrc,tempx,SETFILES1,PSTRUCT,TRUE);
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
						ADVFINDRSCSETFUNC(mtnrsrc,part->name,SETID1,PSTRUCT,TRUE);
						FINDRSCSETEDITABLE(mtnrsrc,part->name,TRUE);
						for(y=1;y<(REPLICATE+1);++y)
						{
							memset(tempx,0,512);
							sprintf(tempx,"%04d:%s",y,part->name);
							ADVFINDRSCSETFUNC(mtnrsrc,tempx,SETID1,PSTRUCT,TRUE);
						}
					}
				}
			}
		}
		if(rpt->accum!=NULL)
		{
			for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
			{
				a->displayed=FALSE;
				if((FINDWDGT(mtnrsrc->scn,a->name))!=(-1)) 
				{
					if(!alreadyrsrc(a->name,mtnrsrc))
					{
						a->displayed=TRUE;
						rrpt->no_pe_accums=FALSE;
						if(a->atype==1)
						{
							makefieldrsrc(mtnrsrc,a->name,LONGV,8,FALSE);
						} else if(a->atype==7)
						{
							field=FLDNRD(rrpt->sortno,a->accname);
							if(field!=NULL)
							{
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										break;
									case BOOLNS:
									case CHARACTERS:
										makefieldrsrc(mtnrsrc,a->name,LONGV,8,FALSE);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										makefieldrsrc(mtnrsrc,a->name,field->type,field->len,FALSE);
										break;
									case SHORTV:
									case SSHORTV:
										makefieldrsrc(mtnrsrc,a->name,field->type,field->len,FALSE);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										makefieldrsrc(mtnrsrc,a->name,field->type,field->len,FALSE);
										break;
									default:
									case BUTTONS:
										break;
								}
							}
						}
					}
				}
			}
		}
		SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
		ADVSCRNvirtual2rsrc(mtnrsrc,RPTMTNSubData,PSTRUCT);
		DefaultScreens(mtnrsrc);
       		PSTRUCT->MTNMASTER=MTNMASTER;
       		PSTRUCT->rsrc=mtnrsrc;
		ApplyActions(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE);
		ZERNRD(MTNMASTER->passkey->fileno);
		w=0;
		ef=FRSNRD(MTNMASTER->passkey->fileno,1);
		while(!ef)
		{
			if(TE_SelectFunc(mtnrsrc,PSTRUCT))
			{
				GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
				TableEditfilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc,w,(w==0 ? TRUE:FALSE));
				++w;
				if(w==REPLICATE) break;
			}
			ef=NXTNRD(MTNMASTER->passkey->fileno,1);
		}
		++w;
		if(w<REPLICATE)
		{
			for(;w<REPLICATE;++w)
			{
				for(x=0,fldx=fields;x<nofields;++x,++fldx)
				{
					memset(tempx,0,512);
					sprintf(tempx,"%04d:%s",w,fldx->name);
					FINDRSCSETEDITABLE(mtnrsrc,tempx,FALSE);
					FINDRSCSETSENSITIVE(mtnrsrc,tempx,FALSE);
				}
			}
		}
		while(!ef)
		{
			TE_SelectFunc(mtnrsrc,PSTRUCT);
			ef=NXTNRD(MTNMASTER->passkey->fileno,1);
		}
		GET_SUPPORTING(mtnrsrc,MTNMASTER,RPTMTNSubData,(void *)PSTRUCT,FALSE,FALSE);
		xSetVirFieldFuncs(mtnrsrc,updateallcb2,(void *)PSTRUCT,1,MTNMASTER,NULL,__LINE__,__FILE__);
		addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,PSTRUCT);
		addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test3,PSTRUCT);
		addbtnrsrc(mtnrsrc,"CONTINUE",TRUE,TE_Continue,PSTRUCT);
		addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
       		PSTRUCT->rsrc=mtnrsrc;
       		PSTRUCT->MTNMASTER=MTNMASTER;
		MakePrevNextButtons(mtnrsrc,keys,nokeys,PSTRUCT);
		MakeREPORTMasterButtons(mtnrsrc,PSTRUCT,NULL,__LINE__,__FILE__);
       		PSTRUCT->rsrc=mtnrsrc;
       		PSTRUCT->MTNMASTER=MTNMASTER;
		MSTADVupdateSCRNvirtuals(mtnrsrc,RPTMTNSubData,(void *)PSTRUCT,MTNMASTER);
       		PSTRUCT->MTNMASTER=MTNMASTER;
		mtnrsrc->SubFunc=(void (*)(...))RPTMTNSubData;
		mtnrsrc->SubFuncArgs=PSTRUCT;
		rrpt->page=1;
		w=rrpt->table_row+1;
		addstdrsrc(mtnrsrc,"CURRENT ROW",LONGV,8,&w,TRUE);
		addstdrsrc(mtnrsrc,"PAGE",LONGV,8,&rrpt->page,FALSE);
		addbtnrsrc(mtnrsrc,"PREVIOUS PAGE",TRUE,ChangePreviousPage,PSTRUCT);
/*
		ADVFINDRSCSETFUNC(mtnrsrc,"PAGE",ChangePage,PSTRUCT,TRUE);
*/
		addbtnrsrc(mtnrsrc,"NEXT PAGE",TRUE,ChangeNextPage,PSTRUCT);
		recs=SIZNRD(MTNMASTER->passkey->fileno);
		rrpt->total_pages=RDAroundup((recs/REPLICATE));
		addstdrsrc(mtnrsrc,"TOTAL PAGES",LONGV,8,&rrpt->total_pages,FALSE);
       		PSTRUCT->rsrc=mtnrsrc;
		MTNMASTER->mtnrsrc=mtnrsrc;
		h->mtnrsrc=mtnrsrc;
		foundit=FALSE;
		for(x=0,wdgt=mtnrsrc->scn->wdgts;x<mtnrsrc->scn->numwdgts;++x,++wdgt)
		{
			if(wdgt->type==31) 
			{
				foundit=TRUE;
			} else if(!isEMPTY(wdgt->resource_name) && foundit==TRUE && isEMPTY(wdgt->editable_expression) && isEMPTY(wdgt->expression) && isEMPTY(wdgt->sensitive_expression))
			{
				y=FINDRSC(mtnrsrc,wdgt->resource_name);
				if(y>(-1))
				{
					mem=mtnrsrc->rscs+y;
					break;
				}
			}
		}
		if(!xmakescrn(mtnrsrc,(MTNMASTER->level==0?TRUE:TRUE),RPTMTN_GUIEVAL,(void *)PSTRUCT,addoswidgets,RPTMTN_GUIEVALSTR,(void *)PSTRUCT,RPTMTNSubData,PSTRUCT,NULL,__LINE__,__FILE__))
		{
			rrpt->screen_count+=1;
			MTNMASTER->WindowCount+=1;
		} else {
			ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom Process Exceptions MAINTAIN SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
			procexcpquitrecord2(h->mtnrsrc,PSTRUCT);
			return;
		}
		if(rrpt->total_pages==1)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"NEXT PAGE",FALSE);
		}
		FINDRSCSETSENSITIVE(mtnrsrc,"PREVIOUS PAGE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"PAGE",FALSE);
		if(mem!=NULL)
		{
			FINDRSCSETINPUTFOCUS(mtnrsrc,mem->rscrname);
			rrpt->lastwidget=mem;
		}
		ApplyActions(mtnrsrc,MTNMASTER,RPTMTNSubData,PSTRUCT,TRUE);
		if(rrpt->APPmainLoop==FALSE)
		{
			rrpt->APPmainLoop=TRUE;	
			return;
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
