/* runmtn.c - Run Maintain Master Browse */
/*lint -library */
#include<powadd.hpp>
#include<rptgen.hpp>
#include<mix.hpp>

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

short PowerMasterButtonFunction(RDArsrc *,MaintainButton *,short);
void PowerExecuteBrowseButtonFunc(RDArsrc *,MaintainButton *,void *,MTNPassableStruct *);
void xMakePowerMasterBrowseButtons(RDArsrc *,MTNPassableStruct *,void (*)(...),int,char *);
static void okBrowseButtonFunc(RDArsrc *,MaintainButton *);
static void doexit_forced(MTNPassableStruct *);
static void doexit(MTNPassableStruct *,int);
static void doexitbl(MakeBrowseList *);
static void okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
void mtnutlmstr(short,void *targetkey,MTNPassableStruct *,int,void (*)(...),RDArsrc *,void *);
void browse_utlA(RDArsrc *,MTNPassableStruct *,int,void (*)(...),RDArsrc *,void *);
void search_browse_utlA(RDArsrc *,MTNPassableStruct *,int,void (*)(...),RDArsrc *,void *);
void browse_utlB(RDArsrc *,MTNPassableStruct *);
static void save_check(RDArsrc *,MTNPassableStruct *,short);
static void save_record(RDArsrc *,MTNPassableStruct *,short);
static void save_record_quit(RDArsrc *,MTNPassableStruct *);
static void save_record_up(RDArsrc *,MTNPassableStruct *);
static void save_record_nup(RDArsrc *,MTNPassableStruct *);
static void save_record_now(RDArsrc *,MTNPassableStruct *);
static void save_record_now1(RDArsrc *,MTNPassableStruct *);
static void quit_record(RDArsrc *,MTNPassableStruct *);
static void quit_record_2(RDArsrc *,MTNPassableStruct *);
static void quit_record_test(RDArsrc *,MTNPassableStruct *);
static void readscreen(RDArsrc *,MTNPassableStruct *);
static void resetdefs(RDArsrc *,MTNPassableStruct *);
static void getpreviouscb(RDArmem *,MTNPassableStruct *);
static void getnextcb(RDArmem *,MTNPassableStruct *);
static void SETID(RDArmem *,MTNPassableStruct *);
static void SETFILES(RDArmem *,MTNPassableStruct *); 
short MTNrun_prequit_buttonsmtn(RDArsrc *,MTNPassableStruct *,short);
short MTNrun_prequit_buttonsbl(RDArsrc *,MTNPassableStruct *,short);
void MTNrun_presave_buttons(RDArsrc *,MTNPassableStruct *,short);
void MTNrun_postsave_buttons(RDArsrc *,MTNPassableStruct *,short);
void updateallMaintainII(RDArsrc *,MTNPassableStruct *,void *);
void updateallMaintain(RDArsrc *,MTNPassableStruct *);
void updateallMaintainquitbl(RDArsrc *,MTNPassableStruct *);
void updateallMaintainquitmtn(RDArsrc *,MTNPassableStruct *);

static void Set_Skip_Widgets(MaintainMaster *p)
{
	RDAsearch *addsub=NULL,*addsub1=NULL;
	int x,y;
	short count=0;

	if(p->s!=NULL)
	{
		for(x=0,addsub=p->s;x<p->num;++x,++addsub)
		{
			count=0;
			if(!RDAstrcmp(addsub->module,p->module) && 
				!RDAstrcmp(addsub->filename,p->mainfile))
			{
				++count;
				if(count>0) addsub->skip_widget=TRUE;
			}
			for(y=0,addsub1=p->s;y<p->num;++y,++addsub1)
			{
				if(!RDAstrcmp(addsub->module,addsub1->module) && 
					!RDAstrcmp(addsub->filename,addsub1->filename))
				{
					 ++count;
					if(count>1) addsub1->skip_widget=TRUE;
				}
			}
		}
	}
}
void browse_utlA(RDArsrc *parent,MTNPassableStruct *PSTRUCT,
		int level,void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromargs)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(parent!=NULL) free_rsrc(parent);
			mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
			searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
			definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
			ZERNRD(MTNMASTER->passkey->fileno);
			ApplyPassKey(MTNMASTER->passkey);
			GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
			GET_SCREEN_VIRTUALS(mainrsrc,0);
			GET_SCREEN_VIRTUALS(definelist,1);
			GET_SCREEN_VIRTUALS(searchrsrc,2);
			mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
			findscrnbin(mainrsrc->scn);
			MakeBrowseListTotals(mainrsrc,MTNMASTER->passkey->fileno,definelist);
			MakeRunFunction(mainrsrc,MTNMASTER);
			xMakePowerMasterBrowseButtons(mainrsrc,PSTRUCT,NULL,__LINE__,__FILE__);
			MTNMASTER->mbl_utlmstr=START_ADV2Pbrowse(MTNMASTER->passkey->fileno,
				MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,definelist,
				doexitbl,okfunc,(MTNMASTER->editable==TRUE ? new_record:NULL),
				PSTRUCT,(MTNMASTER->level==0?FALSE:TRUE),NULL,0,0,2,0,
				MTNMASTER->passkey,MTNMASTER->start_where,1);
       			PSTRUCT->rsrc=MTNMASTER->mbl_utlmstr->mainrsrc;
			if(MTNMASTER->mbl_utlmstr!=NULL)
			{
				MTNMASTER->WindowCount+=1;
			}
		} else {
			prterr("Error Master structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
void search_browse_utlA(RDArsrc *parent,MTNPassableStruct *PSTRUCT,
		int level,void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromargs)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(parent!=NULL) free_rsrc(parent);
			mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
			searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
			definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
			ZERNRD(MTNMASTER->passkey->fileno);
			ApplyPassKey(MTNMASTER->passkey);
			GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
			GET_SCREEN_VIRTUALS(mainrsrc,0);
			GET_SCREEN_VIRTUALS(definelist,1);
			GET_SCREEN_VIRTUALS(searchrsrc,2);
			mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
			findscrnbin(mainrsrc->scn);
			MakeBrowseListTotals(mainrsrc,MTNMASTER->passkey->fileno,definelist);
			MakeRunFunction(mainrsrc,MTNMASTER);
			xMakePowerMasterBrowseButtons(mainrsrc,PSTRUCT,NULL,__LINE__,__FILE__);
			MTNMASTER->mbl_utlmstr=START_ADV2Pbrowse(MTNMASTER->passkey->fileno,
				MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,definelist,
				doexitbl,okfunc,(MTNMASTER->editable==TRUE ? new_record:NULL),
				PSTRUCT,(MTNMASTER->level==0?FALSE:TRUE),NULL,0,0,2,0,
				MTNMASTER->passkey,MTNMASTER->start_where,2);
       			PSTRUCT->rsrc=MTNMASTER->mbl_utlmstr->mainrsrc;
			if(MTNMASTER->mbl_utlmstr!=NULL)
			{
				MTNMASTER->WindowCount+=1;
			}
		} else {
			prterr("Error Master structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
void browse_utlB(RDArsrc *parent,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;
	void (*quitfunc)(...)=NULL;
	RDArsrc *fromrsrc=NULL;
	void *fromargs=NULL;
	MTNPassableStruct *PSTRUCTbl=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(PSTRUCT->previous!=NULL) FreeRDATData(PSTRUCT->previous);
		if(MTNMASTER!=NULL)
		{
			quitfunc=PSTRUCT->quitfunc;
			fromrsrc=PSTRUCT->fromrsrc;
			fromargs=PSTRUCT->fromargs;
			if(PSTRUCT->args!=NULL) PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
			Rfree(PSTRUCT);
			if(parent!=NULL)
			{
				MTNMASTER->WindowCount-=1;
				killwindow(parent);
				free_rsrc(parent); 
			}
			browse_utlA(parent,PSTRUCTbl,MTNMASTER->level,quitfunc,fromrsrc,fromargs);
		}
	}
}
void xRUNMAINTAIN(char *module,char *master,int level,
		void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromargs,
		short APPmainLoop,int line,char *file)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	MTNMASTER=MaintainMasterNew(module,master);
	MTNMASTER->mbl_utlmstr=NULL;
	MTNMASTER->WindowCount=0;
	MTNMASTER->editable=TRUE;
	MTNMASTER->level=level;
	MTNMASTER->SubFunc=SCRNvirtualSubData;
	MTNMASTER->SubFuncArgs=NULL;
	MTNMASTER->APPmainLoop=APPmainLoop;
	PSTRUCT=Rmalloc(sizeof(MTNPassableStruct));
	PSTRUCT->MTNMASTER=MTNMASTER;
	PSTRUCT->quitfunc=quitfunc;
	PSTRUCT->fromrsrc=fromrsrc;
	PSTRUCT->fromargs=fromargs;
	PSTRUCT->args=NULL;

	if(findMaintainMaster(MTNMASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(module)+RDAstrlen(master));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",master,module);
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.",warnmessage,NULL,TRUE); 
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		PSTRUCT->MTNMASTER=MTNMASTER;
		doexit_forced(PSTRUCT);
	} else {
		PSTRUCT->MTNMASTER=MTNMASTER;
		if((filenum=APPOPNNRDEDITABLE(MTNMASTER->module,MTNMASTER->mainfile,
			&MTNMASTER->editable))==(-1))
		{
			doexit_forced(PSTRUCT);
			return;
		}
		if(!OPEN_MASTER_FILES(MTNMASTER))
		{
			doexit_forced(PSTRUCT);
			return;
		}
		Set_Skip_Widgets(MTNMASTER);
		MTNMASTER->passkey=READPassKey(filenum);
		PSTRUCT->MTNMASTER=MTNMASTER;
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				mtnutlmstr(1,NULL,PSTRUCT,level,quitfunc,fromrsrc,fromargs);
				break;
			case 1:
				browse_utlA(NULL,PSTRUCT,level,quitfunc,fromrsrc,fromargs);
				break;
			case 2:
				search_browse_utlA(NULL,PSTRUCT,level,quitfunc,fromrsrc,fromargs);
				break;
		}
	}
	if(!APPmainLoop)
	{
		APPmainLoop=TRUE;
		MTNMASTER->APPmainLoop=TRUE;
		RDAAPPMAINLOOP();
	}
}
static void doexit_forced(MTNPassableStruct *PSTRUCT)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	char tempx[1024];
	int level=0;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
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
			level=MTNMASTER->level;
			if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
			Rfree(PSTRUCT);
			if(level==0)
			{
				memset(tempx,0,1024);
				sprintf(tempx,"%s/%d.csv",CURRENTDIRECTORY,RGETPID());
				unlink(tempx);
				ShutdownSubsystems();
			} else {
				return;
			}
		}
	}
}
static void doexit(MTNPassableStruct *PSTRUCT,int ab)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	char tempx[1024];
	int level=0;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->WindowCount<1)
			{
				level=MTNMASTER->level;
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
				if(level==0)
				{
					ShutdownSubsystems();
				}
			} else {
				if(MTNMASTER->mbl_utlmstr!=NULL) MTNMASTER->mbl_utlmstr=NULL;
			}
		}
	}
	memset(tempx,0,1024);
	sprintf(tempx,"%s/%d.csv",CURRENTDIRECTORY,RGETPID());
	unlink(tempx);
}
static void doexit_2(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;
	int level=0;
	int pn=0;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->WindowCount<2)
			{
				level=MTNMASTER->level;
				if(level!=0)
				{
/*lint -e746 */
					if(PSTRUCT->quitfunc!=NULL) PSTRUCT->quitfunc(PSTRUCT->fromrsrc,PSTRUCT->fromargs);
/*lint +e746 */
				}
			}
		}
		if(rsrc!=NULL)
		{
			MTNMASTER->WindowCount-=1;
			if(IsScreenDisplayed(rsrc)) killwindow(rsrc);
			free_rsrc(rsrc);
		}
		doexit(PSTRUCT,pn);
	}
}
static void doexitbl(MakeBrowseList *blist)
{
	short did_ya=FALSE;
	MTNPassableStruct *PSTRUCT=NULL, *PSTRUCTbl=NULL;
	MaintainMaster *MTNMASTER=NULL;
	int level=0;
	int pn=0;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
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
				if(PSTRUCT->args!=NULL)
				{
					PSTRUCTbl=(MTNPassableStruct *)PSTRUCT->args;
					if(PSTRUCTbl!=NULL) Rfree(PSTRUCTbl);
				}
				if(MTNMASTER->WindowCount<2)
				{
					level=MTNMASTER->level;
					did_ya=MTNrun_prequit_buttonsbl(PSTRUCT->rsrc,PSTRUCT,TRUE);
					if(level!=0)
					{
/*lint -e746 */
						if(PSTRUCT->quitfunc!=NULL) PSTRUCT->quitfunc(PSTRUCT->fromrsrc,PSTRUCT->fromargs);
/*lint +e746 */
					}
				}
				if(blist->mainrsrc!=NULL)
				{
					MTNMASTER->WindowCount-=1;
					if(IsScreenDisplayed(blist->mainrsrc)) killwindow(blist->mainrsrc);
					free_rsrc(blist->mainrsrc);
				}
				if(!did_ya) doexit(PSTRUCT,pn);
			}
		}
	}
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
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
					mtnutlmstr(0,targetkey,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
				} else {	
					PowerExecuteListFunc(blist->mainrsrc,run_func-1,targetkey,PSTRUCT);
				}
			} else {
				mtnutlmstr(0,targetkey,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
			}
		}
	}
}
static void okBrowseButtonFunc(RDArsrc *brsrc,MaintainButton *button)
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
						PowerExecuteBrowseButtonFunc(brsrc,button,list->keyvalue,PSTRUCT);
					}
				}
			}
		}
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;

	if(blist!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)blist->funcargs;
		if(PSTRUCT!=NULL)
		{	
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				mtnutlmstr(2,NULL,PSTRUCT,MTNMASTER->level,PSTRUCT->quitfunc,PSTRUCT->fromrsrc,PSTRUCT->fromargs);
			}
			
		}
	}
} 
void xPowerExecuteListFunc(RDArsrc *mainrsrc,int listnum,void *targetkey,
		 MTNPassableStruct *PSTRUCT,int line,char *file)
{
	APPlib *envpx=NULL,*newargs=NULL,*envpx1=NULL;
	MaintainButton *button=NULL;
	int x;
	char *value=NULL,blank_envpx1=FALSE;
	MaintainMaster *MTNMASTER=NULL;
	MakeBrowseList *blist=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG ExecuteListFunc at line [%d] program [%s].",line,file);
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
				if(mainrsrc!=NULL)
				{
					blist=MTNMASTER->mbl_utlmstr;
					if(blist!=NULL)
					{
						if(blist->definelist!=NULL)
						{
							if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mainrsrc)) 
							{
								ZERNRD(MTNMASTER->passkey->fileno);
#ifdef USE_RDA_DIAGNOSTICS
								if(diagmix || diagmix_field)
								{
									prterr("Error in ExecuteListFunc targetkey not found at line [%d] program [%s].",line,file);
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
							envpx1=MaintainButton2ClearPassKey(button,MTNMASTER->passkey->fileno);
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
										value=EVALUATEstr(newargs->libs[x],SCRNvirtualSubData,blist->definelist);
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
							if(!RDAstrcmp(button->progname,"RUNREPORT") || !RDAstrcmp(button->progname,"runreport"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNREPORTADV2(newargs->libs[0],newargs->libs[1],NULL,NULL,TRUE,2,NULL,button->func,mainrsrc,(void *)PSTRUCT,(MTNMASTER->level+1));
							} else if(!RDAstrcmp(button->progname,"RUNPOWERADD") || !RDAstrcmp(button->progname,"runpoweradd"))
							{
								blank_envpx1=TRUE;
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
							} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD") || !RDAstrcmp(button->progname,"multipoweradd"))
							{
								blank_envpx1=TRUE;
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
							} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"runmaintain"))
							{
								blank_envpx1=TRUE;
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
							}
							if(envpx1!=NULL)
							{
								if(blank_envpx1==TRUE)
								{
									for(x=0;x<envpx1->numlibs;++x)
									{
										RDA_PutEnv(envpx1->libs[x]);
									}
								}
								freeapplib(envpx1);
							}
							if(newargs!=NULL) freeapplib(newargs);
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
void PowerExecuteBrowseButtonFunc(RDArsrc *brsrc,MaintainButton *button,void *targetkey,MTNPassableStruct *PSTRUCT)
{
	APPlib *envpx=NULL,*newargs=NULL,*envpx1=NULL;
	int x,ret_int=0;
	char *value=NULL,*ret_string=NULL,blank_envpx1=FALSE;
	MaintainMaster *MTNMASTER=NULL;
	MakeBrowseList *blist=NULL;

	if(button!=NULL)
	{
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				if(brsrc!=NULL)
				{
					blist=MTNMASTER->mbl_utlmstr;
					if(blist!=NULL)
					{
						if(blist->definelist!=NULL)
						{
							if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,blist->definelist)) 
							{
								ZERNRD(MTNMASTER->passkey->fileno);
#ifdef USE_RDA_DIAGNOSTICS
								if(diagmix || diagmix_field)
								{
									prterr("Error in ExecuteListFunc targetkey not found at line [%d] program [%s].",__LINE__,__FILE__);
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
							envpx1=MaintainButton2ClearPassKey(button,MTNMASTER->passkey->fileno);
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
										value=EVALUATEstr(newargs->libs[x],SCRNvirtualSubData,blist->definelist);
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
							if(!RDAstrcmp(button->progname,"RUNREPORT") || !RDAstrcmp(button->progname,"runreport"))
							{
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNREPORTADV2(newargs->libs[0],newargs->libs[1],NULL,NULL,TRUE,2,NULL,button->func,brsrc,(void *)PSTRUCT,(MTNMASTER->level+1));
							} else if(!RDAstrcmp(button->progname,"MKDIR") || !RDAstrcmp(button->progname,"mkdir"))
							{
								blank_envpx1=TRUE;
								if(newargs!=NULL)
								{
									for(x=0;x<newargs->numlibs;++x)
									{
										if(!isEMPTY(newargs->libs[x])) 
										{
											ret_string=RDAmkdir(newargs->libs[x],MODE_RWXRWX___,&ret_int);
											if(!isEMPTY(ret_string))
											{
												if(ret_int!=EEXIST)
												{
													prterr("Error:  Cannot create directory [%s] because of [%s].",newargs->libs[x],ret_string);
												} 
											}
											if(ret_string!=NULL) Rfree(ret_string);
											ret_string=RDAchmod(newargs->libs[x],MODE_RWXRWX___,&ret_int);
											if(ret_string!=NULL) Rfree(ret_string);
										}
									}
								}
							} else if(!RDAstrcmp(button->progname,"RMDIR") || !RDAstrcmp(button->progname,"rmdir"))
							{
								blank_envpx1=TRUE;
								if(newargs!=NULL)
								{
								for(x=0;x<newargs->numlibs;++x)
								{
									if(!isEMPTY(newargs->libs[x])) rmdir(newargs->libs[x]);
								}
								}
							} else if(!RDAstrcmp(button->progname,"UNLINK") || !RDAstrcmp(button->progname,"unlink"))
							{
								blank_envpx1=TRUE;
								if(newargs!=NULL)
								{
								for(x=0;x<newargs->numlibs;++x)
								{
									if(!isEMPTY(newargs->libs[x])) unlink(newargs->libs[x]);
								}
								}
							} else if(!RDAstrcmp(button->progname,"COPYFILE") || !RDAstrcmp(button->progname,"copyfile"))
							{
								RDACopyFile(newargs->libs[0],newargs->libs[1]);
							} else if(!RDAstrcmp(button->progname,"MOVEFILE") || !RDAstrcmp(button->progname,"movefile"))
							{
								RDAMoveFile(newargs->libs[0],newargs->libs[1]);
							} else if(!RDAstrcmp(button->progname,"DISPLAYRELATIVEFILE") || !RDAstrcmp(button->progname,"displayrelativefile"))
							{
								DisplayRelativeFile(newargs->libs[0]);
							} else if(!RDAstrcmp(button->progname,"DISPLAYFILE") || !RDAstrcmp(button->progname,"displayfile") || !RDAstrcmp(button->progname,"doarchive"))
							{
								DisplayFile(newargs->libs[0]);
							} else if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
							{
								blank_envpx1=TRUE;
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)PSTRUCT,TRUE);
							} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD"))
							{
								blank_envpx1=TRUE;
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)PSTRUCT,TRUE);
							} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"runmaintain"))
							{
								blank_envpx1=TRUE;
								if(envpx!=NULL)
								{
									for(x=0;x<envpx->numlibs;++x)
									{
										RDA_PutEnv(envpx->libs[x]);
									}
								}
								RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)PSTRUCT,TRUE);
							} else {
								if(button->type==0)
								{
									ADVExecuteProgram(button->progname,newargs,envpx);
								} else {
									ADVExecute2Program(button->progname,newargs,envpx);
								}
							}
							if(envpx1!=NULL)
							{
								if(blank_envpx1==TRUE)
								{
									for(x=0;x<envpx1->numlibs;++x)
									{
										RDA_PutEnv(envpx1->libs[x]);
									}
								}
								freeapplib(envpx1);
							}
							if(newargs!=NULL) freeapplib(newargs);
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
void PostSaveButtonFunction(RDArsrc *mainrsrc,MaintainButton *button)
{
	APPlib *envpx=NULL,*newargs=NULL,*envpx1=NULL;
	int x,ret_int=0;
	char *value=NULL,*ret_string=NULL,blank_envpx1=FALSE;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	if(button!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)button->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				if(mainrsrc!=NULL)
				{
					envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
					envpx1=MaintainButton2ClearPassKey(button,MTNMASTER->passkey->fileno);
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
								value=EVALUATEstr(newargs->libs[x],SCRNvirtualSubData,mainrsrc);
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
					if(!RDAstrcmp(button->progname,"RUNREPORT") || !RDAstrcmp(button->progname,"runreport"))
					{
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNREPORTADV2(newargs->libs[0],newargs->libs[1],NULL,NULL,TRUE,2,NULL,button->func,mainrsrc,(void *)PSTRUCT,(MTNMASTER->level+1));
					} else if(!RDAstrcmp(button->progname,"RUNPOWERADD") || !RDAstrcmp(button->progname,"runpoweradd"))
					{
						blank_envpx1=TRUE;
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
					} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD") || !RDAstrcmp(button->progname,"multipoweradd"))
					{
						blank_envpx1=TRUE;
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
					} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"runmaintain"))
					{
						blank_envpx1=TRUE;
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
					} else if(!RDAstrcmp(button->progname,"MKDIR") || !RDAstrcmp(button->progname,"mkdir"))
					{
						blank_envpx1=TRUE;
						if(newargs!=NULL)
						{
							for(x=0;x<newargs->numlibs;++x)
							{
								if(!isEMPTY(newargs->libs[x])) 
								{
									ret_string=RDAmkdir(newargs->libs[x],MODE_RWXRWX___,&ret_int);
									if(!isEMPTY(ret_string))
									{
										if(ret_int!=EEXIST)
										{
											prterr("Error:  Cannot create directory [%s] because of [%s].",newargs->libs[x],ret_string);
										} 
									}
									if(ret_string!=NULL) Rfree(ret_string);
									ret_string=RDAchmod(newargs->libs[x],MODE_RWXRWX___,&ret_int);
									if(ret_string!=NULL) Rfree(ret_string);
										}
							}
						}
					} else if(!RDAstrcmp(button->progname,"UNLINK") || !RDAstrcmp(button->progname,"unlink"))
					{
						blank_envpx1=TRUE;
						if(newargs!=NULL)
						{
							for(x=0;x<newargs->numlibs;++x)
							{
								if(!isEMPTY(newargs->libs[x]))
								{
									unlink(newargs->libs[x]);
								}
							}
						}
					} else if(!RDAstrcmp(button->progname,"RMDIR") || !RDAstrcmp(button->progname,"rmdir"))
					{
						blank_envpx1=TRUE;
						if(newargs!=NULL)
						{
							for(x=0;x<newargs->numlibs;++x)
							{
								if(!isEMPTY(newargs->libs[x]))
								{
									rmdir(newargs->libs[x]);
								}
							}
						}
					} else if(!RDAstrcmp(button->progname,"COPYFILE") || !RDAstrcmp(button->progname,"copyfile"))
					{
						RDACopyFile(newargs->libs[0],newargs->libs[1]);
					} else if(!RDAstrcmp(button->progname,"MOVEFILE") || !RDAstrcmp(button->progname,"movefile"))
					{
						RDAMoveFile(newargs->libs[0],newargs->libs[1]);
					} else if(!RDAstrcmp(button->progname,"DISPLAYRELATIVEFILE") || !RDAstrcmp(button->progname,"displayrelativefile"))
					{
						DisplayRelativeFile(newargs->libs[0]);
					} else if(!RDAstrcmp(button->progname,"DISPLAYFILE") || !RDAstrcmp(button->progname,"displayfile") || !RDAstrcmp(button->progname,"doarchive"))
					{
						DisplayFile(newargs->libs[0]);
					} else {
						if(button->type==0)
						{
							ADVExecuteProgram(button->progname,newargs,envpx);
						} else {
							ADVExecute2Program(button->progname,newargs,envpx);
						}
					}
					if(envpx1!=NULL)
					{
						if(blank_envpx1==TRUE)
						{
							for(x=0;x<envpx1->numlibs;++x)
							{
								RDA_PutEnv(envpx1->libs[x]);
							}
						}
						freeapplib(envpx1);
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
short PowerMasterButtonFunction(RDArsrc *mainrsrc,MaintainButton *button,short quit)
{
	short did_ya=FALSE;
	APPlib *envpx=NULL,*newargs=NULL,*envpx1=NULL;
	int x,ret_int=0;
	char *value=NULL,*ret_string=NULL,blank_envpx1=FALSE;
	char *temp=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	if(button!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)button->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				if(mainrsrc!=NULL)
				{
					if(!MTNMASTER->file_type)
					{
						rsrc2filerecord(MTNMASTER->passkey->fileno,mainrsrc);
					} else {
						rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mainrsrc);
					}
					ApplyActions(mainrsrc,MTNMASTER,SCRNvirtualSubData,mainrsrc,TRUE);
					envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
					envpx1=MaintainButton2ClearPassKey(button,MTNMASTER->passkey->fileno);
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
								value=EVALUATEstr(newargs->libs[x],SCRNvirtualSubData,mainrsrc);
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
					if(!RDAstrcmp(button->progname,"RUNREPORT") || !RDAstrcmp(button->progname,"runreport"))
					{
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						if(quit==FALSE)
						{
							RUNREPORTADV2(newargs->libs[0],newargs->libs[1],NULL,NULL,TRUE,2,NULL,button->func,mainrsrc,(void *)PSTRUCT,(MTNMASTER->level+1));
						} else {
							RUNREPORTADV2(newargs->libs[0],newargs->libs[1],NULL,NULL,TRUE,1,NULL,button->func,mainrsrc,(void *)PSTRUCT,0);
						}
						did_ya=TRUE;
					} else if(!RDAstrcmp(button->progname,"RUNPOWERADD") || !RDAstrcmp(button->progname,"runpoweradd"))
					{
						blank_envpx1=TRUE;
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
						did_ya=TRUE;
					} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD") || !RDAstrcmp(button->progname,"multipoweradd"))
					{
						blank_envpx1=TRUE;
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
						did_ya=TRUE;
					} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"runmaintain"))
					{
						blank_envpx1=TRUE;
						if(envpx!=NULL)
						{
							for(x=0;x<envpx->numlibs;++x)
							{
								RDA_PutEnv(envpx->libs[x]);
							}
						}
						RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,mainrsrc,(void *)PSTRUCT,TRUE);
						did_ya=TRUE;
					} else if(!RDAstrcmp(button->progname,"RMDIR") || !RDAstrcmp(button->progname,"rmdir"))
					{
						blank_envpx1=TRUE;
						if(newargs!=NULL)
						{
							for(x=0;x<newargs->numlibs;++x)
							{
								if(!isEMPTY(newargs->libs[x])) rmdir(newargs->libs[x]);
							}
						}
					} else if(!RDAstrcmp(button->progname,"UNLINK") || !RDAstrcmp(button->progname,"unlink"))
					{
						blank_envpx1=TRUE;
						if(newargs!=NULL)
						{
							for(x=0;x<newargs->numlibs;++x)
							{
								if(!isEMPTY(newargs->libs[x])) unlink(newargs->libs[x]);
							}
						}
					} else if(!RDAstrcmp(button->progname,"MKDIR") || !RDAstrcmp(button->progname,"mkdir"))
					{
						blank_envpx1=TRUE;
						if(newargs!=NULL)
						{
							for(x=0;x<newargs->numlibs;++x)
							{
								if(!isEMPTY(newargs->libs[x])) 
								{
									ret_string=RDAmkdir(newargs->libs[x],MODE_RWXRWX___,&ret_int);
									if(!isEMPTY(ret_string))
									{
										if(ret_int!=EEXIST)
										{
											prterr("Error:  Cannot create directory [%s] because of [%s].",newargs->libs[x],ret_string);
										} 
									}
									if(ret_string!=NULL) Rfree(ret_string);
									ret_string=RDAchmod(newargs->libs[x],MODE_RWXRWX___,&ret_int);
									if(ret_string!=NULL) Rfree(ret_string);
								}
							}
						}
					} else if(!RDAstrcmp(button->progname,"COPYFILE") || !RDAstrcmp(button->progname,"copyfile"))
					{
						RDACopyFile(newargs->libs[0],newargs->libs[1]);
					} else if(!RDAstrcmp(button->progname,"MOVEFILE") || !RDAstrcmp(button->progname,"movefile"))
					{
						RDAMoveFile(newargs->libs[0],newargs->libs[1]);
					} else if(!RDAstrcmp(button->progname,"DISPLAYRELATIVEFILE") || !RDAstrcmp(button->progname,"displayrelativefile"))
					{
						DisplayRelativeFile(newargs->libs[0]);
					} else if(!RDAstrcmp(button->progname,"DISPLAYFILE") || !RDAstrcmp(button->progname,"displayfile") || !RDAstrcmp(button->progname,"doarchive"))
					{
						DisplayFile(newargs->libs[0]);
					} else { 
						if(button->type==0)
						{
							ADVExecuteProgram(button->progname,newargs,envpx);
						} else {
							ADVExecute2Program(button->progname,newargs,envpx);
						}
					}
					if(envpx1!=NULL)
					{
						if(blank_envpx1==TRUE)
						{
							for(x=0;x<envpx1->numlibs;++x)
							{
								temp=RDAstrstr(envpx1->libs[x],"=");
								if(temp!=NULL)
								{
									*temp=0;
								}
								RDA_UnSetEnv(envpx1->libs[x]);
							}
						}
						freeapplib(envpx1);
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
	return(did_ya);
}
void updateallMaintainII(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,void *v)
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
				if(!ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,v,SCRNvirtualSubData,rsrc))
				{
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,rsrc);
				}
				GET_SUPPORTING(rsrc,MTNMASTER,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE);
				xcomputeandupdateallSCRNvirtuals(rsrc,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
				updateallrsrc(rsrc);
				SetAllEditableAndSensitive(rsrc);
			}
		}
	}
}
void PostSaveupdateallMaintain(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->mbl_utlmstr==NULL)
			{
			if(rsrc!=NULL)
			{
				readallwidgets(rsrc);
				if(!ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,rsrc))
				{
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,rsrc);
				}
				GET_SUPPORTING(rsrc,MTNMASTER,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE);
				xcomputeandupdateallSCRNvirtuals(rsrc,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
				updateallrsrc(rsrc);
				SetAllEditableAndSensitive(rsrc);
			}
			}
		}
	}
}
void updateallMaintain(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT)
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
				if(!ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,rsrc))
				{
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,rsrc);
				}
				GET_SUPPORTING(rsrc,MTNMASTER,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE);
				xcomputeandupdateallSCRNvirtuals(rsrc,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
				updateallrsrc(rsrc);
				SetAllEditableAndSensitive(rsrc);
			}
		}
	}
}
void updateallMaintainquitbl(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(rsrc!=NULL)
			{
				doexit_2(rsrc,PSTRUCT);
			} 
		}
	}
}
void updateallMaintainquitmtn(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(rsrc!=NULL)
			{
/*
				readallwidgets(rsrc);
				GET_SUPPORTING(rsrc,MTNMASTER,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE);
				xcomputeandupdateallSCRNvirtuals(rsrc,SCRNvirtualSubData,(void *)rsrc,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
				updateallrsrc(rsrc);
				SetAllEditableAndSensitive(rsrc);
*/
			}
			quit_record_2(rsrc,PSTRUCT);
		}
	}
}
void mtnutlmstr(short dowhich,void *targetkey,MTNPassableStruct *PSTRUCTbl,
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

	if(PSTRUCTbl!=NULL)
	{
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
				nonlstmakefld(mtnrsrc,MTNMASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL) 
				{ 
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
				ADVFINDRSCSETFUNC(mtnrsrc,name,SETFILES,PSTRUCT,TRUE);
				if(RDAstrcmp(fldx->name,"IMAGE"))
				{
					switch(fldx->type)
					{
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
							FINDRSCSETCALLBACKTYPE(mtnrsrc,name,TRUE);
							break;
						default:
							break;
					}
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
						ADVFINDRSCSETFUNC(mtnrsrc,name,SETID,PSTRUCT,TRUE);
						FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
					}
				}
			}
		}
		if(name!=NULL) Rfree(name);
		SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
		SCRNvirtual2rsrc(mtnrsrc);
		DefaultScreens(mtnrsrc);
       		PSTRUCT->MTNMASTER=MTNMASTER;
       		PSTRUCT->rsrc=mtnrsrc;
		ApplyActions(mtnrsrc,MTNMASTER,SCRNvirtualSubData,mtnrsrc,TRUE);
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
						GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
						filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					}
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
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
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
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
					} else {
						GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
						filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					}
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				}
				break;
		}
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
		MSTSetVirFieldFuncs(mtnrsrc,MTNMASTER);
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,PSTRUCT);
		if(MTNMASTER->mbl_utlmstr!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,PSTRUCT);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,PSTRUCT);
		}
		addrfcbrsrc(mtnrsrc,"BROWSE",TRUE,browse_utlB,PSTRUCT);
/*
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_utlB,PSTRUCT);
*/
		addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,PSTRUCT);
		addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,PSTRUCT);
		addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
       		PSTRUCT->rsrc=mtnrsrc;
       		PSTRUCT->MTNMASTER=MTNMASTER;
		MakePrevNextButtons(mtnrsrc,keys,nokeys,PSTRUCT);
		MakePowerMasterButtons(mtnrsrc,PSTRUCT,updateallMaintain);
       		PSTRUCT->rsrc=mtnrsrc;
       		PSTRUCT->MTNMASTER=MTNMASTER;
/* It's important to use the do_sensitive argument =FALSE to avoid calling the
   SetAllEditableAndSensitive() twice prior to screen creation.  There are a 
   number of different kinds of computeall*SCRNvirtual() #define's so be specific
   use the new one.... thanks.... */
		MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMASTER);
		if(MTNMASTER->mbl_utlmstr!=NULL)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
		}
		if(!MTNMASTER->editable)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
		}
       		PSTRUCT->MTNMASTER=MTNMASTER;
       		PSTRUCT->rsrc=mtnrsrc;
		MTNMASTER->mtnrsrc=mtnrsrc;
		if(!APPmakescrn(mtnrsrc,(MTNMASTER->level==0?FALSE:TRUE),quit_record,PSTRUCT,(MTNMASTER->mbl_utlmstr==NULL?TRUE:FALSE)))
/*
		if(!xmakescrn(mtnrsrc,(MTNMASTER->level==0?FALSE:TRUE),EvalFunc,EvalFuncArgs,addoswidgets,EvalStr,EvalStrArgs,SubFunc,SubFuncArgs,quit_record,PSTRUCT,(MTNMASTER->mbl_utlmstr==NULL?TRUE:FALSE)))
*/
		{
			MTNMASTER->WindowCount+=1;
		} else {
			quit_record(mtnrsrc,PSTRUCT);
		}
		ApplyActions(mtnrsrc,MTNMASTER,SCRNvirtualSubData,mtnrsrc,TRUE);
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
			} else mem=mtnrsrc->rscs;
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
			GET_PREVIOUS(member->parent,MTNMASTER,PSTRUCT->previous,SCRNvirtualSubData,member->parent,keyno);
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
			GET_NEXT(member->parent,MTNMASTER,PSTRUCT->previous,SCRNvirtualSubData,member->parent,keyno);
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
			RMEMSET_KEYFUNC(member,MTNMASTER,PSTRUCT->previous,SCRNvirtualSubData,(void *)member->parent,MTNMASTER->inheritrsrcs);
		}
	}
}
static void SETFILES(RDArmem *member,MTNPassableStruct *PSTRUCT) 
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{		
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			RMEMSET_MASTER(member,MTNMASTER,SCRNvirtualSubData,(void *)member->parent);
		}
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
/* DCT: Believe this is unnecessary */
			rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
		}
	}
}
short MTNrun_prequit_buttonsmtn(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,short update)
{
	short did_ya=FALSE;
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
						do_btn=EVALUATEbol(button->expression,SCRNvirtualSubData,(void *)rsrc);
					} else do_btn=TRUE;
					if(do_btn==TRUE)
					{
						button->func=updateallMaintainquitmtn;
						button->parent=PSTRUCT;
						if(PowerMasterButtonFunction(rsrc,button,TRUE)) did_ya=TRUE;
					}
				}
			}
		}
	}
	return(did_ya);
}
short MTNrun_prequit_buttonsbl(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,short update)
{
	short did_ya=FALSE;
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
						do_btn=EVALUATEbol(button->expression,SCRNvirtualSubData,(void *)rsrc);
					} else do_btn=TRUE;
					if(do_btn==TRUE)
					{
/*
						button->func=updateallMaintainquitbl;
*/
						button->func=NULL;
						button->parent=PSTRUCT;
						if(PowerMasterButtonFunction(rsrc,button,TRUE)) did_ya=TRUE;
					}
				}
			}
		}
	}
	return(did_ya);
}
void MTNrun_presave_buttons(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,short update)
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
						do_btn=EVALUATEbol(button->expression,SCRNvirtualSubData,(void *)rsrc);
					} else do_btn=TRUE;
					if(do_btn==TRUE)
					{
						button->func=NULL;
						button->parent=PSTRUCT;
						PowerMasterButtonFunction(rsrc,button,FALSE);
					}
				}
			}
		}
	}
}
void MTNrun_postsave_buttons(RDArsrc *rsrc,MTNPassableStruct *PSTRUCT,short update)
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
						do_btn=EVALUATEbol(button->expression,SCRNvirtualSubData,(void *)rsrc);
					} else do_btn=TRUE;
					if(do_btn==TRUE)
					{
						button->func=PostSaveupdateallMaintain;
						button->parent=PSTRUCT;
						PostSaveButtonFunction(rsrc,button);
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
		MTNrun_presave_buttons(mtnrsrc,PSTRUCT,TRUE);
		save_record(mtnrsrc,PSTRUCT,TRUE);
	}
}
static void save_record_now1(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	if(PSTRUCT!=NULL)
	{
		readscreen(mtnrsrc,PSTRUCT);
		MTNrun_presave_buttons(mtnrsrc,PSTRUCT,FALSE);
		save_record(mtnrsrc,PSTRUCT,FALSE);
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
						desc,(update?save_record_now:save_record_now1),
						NULL,FALSE,2,mtnrsrc,PSTRUCT,NULL);
					if(desc!=NULL) Rfree(desc);
					return;
				}
			}
			MTNrun_presave_buttons(mtnrsrc,PSTRUCT,update);
			save_record(mtnrsrc,PSTRUCT,update);
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
	char *rcddesc=NULL;
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
			if(ADV2RECORDsec(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc,1))
			{
				rcddesc=ADV2RECORDsecDesc(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc,1);
				if(rcddesc!=NULL)
				{
					ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
					Rfree(rcddesc);
				}
				prterr("Error didn't write record.");
			} else {
				if(MTNMASTER->UsePowerWrite==TRUE)
				{
					NewWriteData=RDATDataNEW(MTNMASTER->passkey->fileno);
					RDATData2File(MTNMASTER->passkey->fileno,PSTRUCT->previous);
					if(!ADVEQLNRDsec(MTNMASTER->passkey->fileno,1,
						SCRNvirtualSubData,mtnrsrc))
					{
						MaintainMasterPowerWrite(MTNMASTER,MTNMASTER->numPrevWrites,
							MTNMASTER->PrevWrites);
					}
					RDATData2File(MTNMASTER->passkey->fileno,NewWriteData);
					MaintainMasterPowerWrite(MTNMASTER,MTNMASTER->numNewWrites,
						MTNMASTER->NewWrites);
					if(NewWriteData!=NULL) FreeRDATData(NewWriteData);
				} else {
					WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,PSTRUCT->previous);
				}
				length=0;
				v=GETNRDKEY(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,&length);
				update=DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,
					(void *)mtnrsrc);
				MTNrun_postsave_buttons(mtnrsrc,PSTRUCT,FALSE);
				if(MTNMASTER->mbl_utlmstr!=NULL) 
				{
					updatebrowse(update_list,MTNMASTER->mbl_utlmstr,
						MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
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
}
static void quit_record_2(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	int level=0;

	if(PSTRUCT!=NULL)
	{
		if(PSTRUCT->previous!=NULL) FreeRDATData(PSTRUCT->previous);
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->WindowCount<2)
			{
				level=MTNMASTER->level;
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
				if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
				if(PSTRUCT!=NULL) Rfree(PSTRUCT);
				if(mtnrsrc!=NULL)
				{
					MTNMASTER->WindowCount-=1;
					killwindow(mtnrsrc);
					free_rsrc(mtnrsrc); 
				}
				if(level==0)
				{
					ShutdownSubsystems();
				} else {
					return;
				}
			}
		}
		if(mtnrsrc!=NULL)
		{
			if(MTNMASTER!=NULL) MTNMASTER->WindowCount-=1;
			killwindow(mtnrsrc);
			free_rsrc(mtnrsrc); 
		}
		if(PSTRUCT!=NULL) Rfree(PSTRUCT);
	}
}
static void quit_record(RDArsrc *mtnrsrc,MTNPassableStruct *PSTRUCT)
{
	APPlib *args=NULL;
	MaintainMaster *MTNMASTER=NULL;
	int level=0;

	if(PSTRUCT!=NULL)
	{
		if(PSTRUCT->previous!=NULL) FreeRDATData(PSTRUCT->previous);
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			if(MTNMASTER->WindowCount<2)
			{
				level=MTNMASTER->level;
				MTNrun_prequit_buttonsmtn(mtnrsrc,PSTRUCT,FALSE);
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
				if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
				if(PSTRUCT!=NULL) Rfree(PSTRUCT);
				if(mtnrsrc!=NULL)
				{
					killwindow(mtnrsrc);
					free_rsrc(mtnrsrc); 
				}
				if(level==0)
				{
					ShutdownSubsystems();
				} else {
					return;
				}
			}
		}
		if(mtnrsrc!=NULL)
		{
			if(MTNMASTER!=NULL) MTNMASTER->WindowCount-=1;
			killwindow(mtnrsrc);
			free_rsrc(mtnrsrc); 
		}
/*  Freaking it out here because the pointer is used in buttons.  It will get free'd when it is the last screen or when the process exits.
		if(PSTRUCT!=NULL) Rfree(PSTRUCT);
*/
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
void PowerMasterBrowseSupInitFunctionLPM(RDAsearch *sup)
{
	RDAconfld *confld=NULL;
	int y,last_inherit=(-1);
	char *temp=NULL;
	NRDfield *field=NULL;
	RDArsrc *rsrc=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;

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
				readallwidgets(rsrc);
				for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
				{
					if(confld->inherit==TRUE)
					{
						last_inherit=y;
					} else break;
				}
				for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
				{
					if((confld->inherit==TRUE) || (y==(last_inherit+1)))
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
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->dlen=0;
										FINDRSCGETSTRING(rsrc,temp,&field->data.string_value);
										if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
										if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
										break;
									case BOOLNS:
									case CHARACTERS:
										if(confld->inherit==TRUE) FINDRSCGETCHAR(rsrc,temp,field->data.string_value);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(confld->inherit==TRUE) FINDRSCGETDOUBLE(rsrc,temp,field->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										if(confld->inherit==TRUE) FINDRSCGETSHORT(rsrc,temp,field->data.short_value);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										if(confld->inherit==TRUE) FINDRSCGETINT(rsrc,temp,field->data.integer_value);
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
										if(confld->inherit==TRUE) FINDRSCGETSTRING(rsrc,temp,&field->data.string_value);
										break;
									case BOOLNS:
									case CHARACTERS:
										if(confld->inherit==TRUE) FINDRSCGETCHAR(rsrc,temp,field->data.string_value);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(confld->inherit==TRUE) FINDRSCGETDOUBLE(rsrc,temp,field->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										if(confld->inherit==TRUE) FINDRSCGETSHORT(rsrc,temp,field->data.short_value);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										if(confld->inherit==TRUE) FINDRSCGETINT(rsrc,temp,field->data.integer_value);
										break;
									default:
									case BUTTONS:
										break;
								}
								if(temp!=NULL) Rfree(temp);
							}
						} else {
							prterr("Error field not found for confld->fldname [%s] ",(confld->fldname!=NULL?confld->fldname:""));
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
}
void PowerMasterBrowseInitFunctionLPM(MakeBrowseList *BL)
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
void PowerMasterBrowseSupInitFunction(RDAsearch *sup)
{
	RDAconfld *confld=NULL;
	int y;
	char *temp=NULL;
	NRDfield *field=NULL;
	RDArsrc *rsrc=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	MaintainMaster *MTNMASTER=NULL;

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
				readallwidgets(rsrc);
				for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
				{
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
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->dlen=0;
										FINDRSCGETSTRING(rsrc,temp,&field->data.string_value);
										if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
										if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
										break;
									case BOOLNS:
									case CHARACTERS:
										FINDRSCGETCHAR(rsrc,temp,field->data.string_value);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										FINDRSCGETDOUBLE(rsrc,temp,field->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										FINDRSCGETSHORT(rsrc,temp,field->data.short_value);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										FINDRSCGETINT(rsrc,temp,field->data.integer_value);
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
										FINDRSCGETSTRING(rsrc,temp,&field->data.string_value);
										break;
									case BOOLNS:
									case CHARACTERS:
										FINDRSCGETCHAR(rsrc,temp,field->data.string_value);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										FINDRSCGETDOUBLE(rsrc,temp,field->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										FINDRSCGETSHORT(rsrc,temp,field->data.short_value);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										FINDRSCGETINT(rsrc,temp,field->data.integer_value);
										break;
									default:
									case BUTTONS:
										break;
								}
								if(temp!=NULL) Rfree(temp);
							}
						} else {
							prterr("Error field not found for confld->fldname [%s] ",(confld->fldname!=NULL?confld->fldname:""));
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
}
void PowerMasterBrowseInitFunction(MakeBrowseList *BL)
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
short PowerMasterBrowseSelectFunction(MakeBrowseList *BL)
{
	RDAsearch *sup=NULL;
	short return_value=TRUE;
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
				if(!isEMPTY(sup->expression))
				{
					if(MTNMASTER->SubFuncArgs!=NULL)
					{
						return_value=PP_EVALUATEbol(sup->expression,MTNMASTER->SubFunc,MTNMASTER->SubFuncArgs);
					} else {
						return_value=PP_EVALUATEbol(sup->expression,MTNMASTER->SubFunc,BL->definelist);
					}
				}	
			} else {
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	}
	return(return_value);
}
short PowerMasterBrowseSupBreakFunctionLPM(RDAsearch *sup,RDArsrc *rsrc)
{
	RDAconfld *confld=NULL;
	int y,ivalue=0,last_inherit=0,x=0;
	short return_value=TRUE,shvalue=0;
	char *temp=NULL,cvalue=FALSE,*svalue=NULL;
	char *temp3=NULL;
#ifdef USE_MYISAM
	char *temp2=NULL;
#endif
	double fvalue=0.0;
	NRDfield *field=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
				last_inherit=(-1);
				for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
				{
					if(confld->inherit==TRUE)
					{
						last_inherit=y;
					}
					if(confld->inherit==FALSE) break;
				}
				for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
				{
					if(return_value==FALSE) break;
					if(confld->inherit==TRUE || (y==(last_inherit+1)))
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
										FINDRSCGETSTRING(rsrc,temp,&svalue);
										if(!isEMPTY(svalue))
										{
											x=RDAstrlen(svalue);
											for(;x>0;--x)
											{
												temp3=svalue+(x-1);
												if(*temp3==' ') *temp3=0;
											}
											if(isEMPTY(svalue)) 
											{
												if(svalue!=NULL) Rfree(svalue);
											}
										}
										if(confld->inherit==TRUE) 
										{
											if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
										} else {
#ifndef USE_MYISAM
											if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
											if(RDA_CollateType==FALSE)
											{
												stoupper(svalue);
												temp2=stralloc(field->data.string_value);
												stoupper(temp2);
												if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
												if(temp2!=NULL) Rfree(temp2);
											} else {
												if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
											}
#endif
										}
										if(svalue!=NULL) Rfree(svalue);
										break;
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										FINDRSCGETSTRING(rsrc,temp,&svalue);
										if(confld->inherit==TRUE) 
										{
											if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
										} else {
#ifndef USE_MYISAM
											if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
											if(RDA_CollateType==FALSE)
											{
												stoupper(svalue);
												temp2=stralloc(field->data.string_value);
												stoupper(temp2);
												if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
												if(temp2!=NULL) Rfree(temp2);
											} else {
												if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
											}
#endif
										}
										if(svalue!=NULL) Rfree(svalue);
										break;
									case BOOLNS:
									case CHARACTERS:
										if(confld->inherit==TRUE)
										{
										FINDRSCGETCHAR(rsrc,temp,&cvalue);
										if(*field->data.string_value!=cvalue) return_value=FALSE;
										}
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(confld->inherit==TRUE)
										{
										FINDRSCGETDOUBLE(rsrc,temp,&fvalue);
/*
										fvalue=EVALUATEdbl(confld->conname,SubFunc,SubFuncArgs);
*/
										if(*field->data.float_value!=fvalue) return_value=FALSE;
										}
										break;
									case SHORTV:
									case SSHORTV:
										if(confld->inherit==TRUE)
										{

										FINDRSCGETSHORT(rsrc,temp,&shvalue);
/*
										shvalue=EVALUATEsht(confld->conname,SubFunc,SubFuncArgs);
*/
										if(*field->data.short_value!=shvalue) return_value=FALSE;
										}
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										if(confld->inherit==TRUE)
										{
										FINDRSCGETINT(rsrc,temp,&ivalue);
/*
										ivalue=EVALUATEint(confld->conname,SubFunc,SubFuncArgs);
*/
										if(*field->data.integer_value!=ivalue) return_value=FALSE;
										}
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
										FINDRSCGETSTRING(rsrc,temp,&svalue);
										if(!isEMPTY(svalue))
										{
											x=RDAstrlen(svalue);
											for(;x>0;--x)
											{
												temp3=svalue+(x-1);
												if(*temp3==' ') *temp3=0;
											}
											if(isEMPTY(svalue)) 
											{
												if(svalue!=NULL) Rfree(svalue);
											}
										}
										if(confld->inherit==TRUE) 
										{
											if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
										} else {
#ifndef USE_MYISAM
											if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
											if(RDA_CollateType==FALSE)
											{
												stoupper(svalue);
												temp2=stralloc(field->data.string_value);
												stoupper(temp2);
												if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
												if(temp2!=NULL) Rfree(temp2);
											} else {
												if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
											}
#endif
										}
										if(svalue!=NULL) Rfree(svalue);
										break;
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										FINDRSCGETSTRING(rsrc,temp,&svalue);
										if(confld->inherit==TRUE) 
										{
											if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
										} else {
#ifndef USE_MYISAM
											if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
											if(RDA_CollateType==FALSE)
											{
												stoupper(svalue);
												temp2=stralloc(field->data.string_value);
												stoupper(temp2);
												if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
												if(temp2!=NULL) Rfree(temp2);
											} else {
												if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
											}
#endif
										}
										if(svalue!=NULL) Rfree(svalue);
										break;
									case BOOLNS:
									case CHARACTERS:
										if(confld->inherit==TRUE) 
										{
										FINDRSCGETCHAR(rsrc,temp,&cvalue);
										if(*field->data.string_value!=cvalue) return_value=FALSE;
										}
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(confld->inherit==TRUE) 
										{
										FINDRSCGETDOUBLE(rsrc,temp,&fvalue);
										if(*field->data.float_value!=fvalue) return_value=FALSE;
										}
										break;
									case SHORTV:
									case SSHORTV:
										if(confld->inherit==TRUE) 
										{
										FINDRSCGETSHORT(rsrc,temp,&shvalue);
										if(*field->data.short_value!=shvalue) return_value=FALSE;
										}
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										if(confld->inherit==TRUE) 
										{
										FINDRSCGETINT(rsrc,temp,&ivalue);
										if(*field->data.integer_value!=ivalue) return_value=FALSE;
										}
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
	}
	return(return_value);
}
short PowerMasterBrowseSupBreakFunction(RDAsearch *sup,RDArsrc *rsrc)
{
	RDAconfld *confld=NULL;
	int y,ivalue=0;
	short return_value=TRUE,shvalue=0;
	char *temp=NULL,cvalue=FALSE,*svalue=NULL;
	double fvalue=0.0;
	NRDfield *field=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
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
/*
										svalue=EVALUATEstr(confld->conname,SubFunc,SubFuncArgs);
*/
										if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
										if(svalue!=NULL) Rfree(svalue);
										break;
									case BOOLNS:
									case CHARACTERS:
										FINDRSCGETCHAR(rsrc,temp,&cvalue);
/*
										cvalue=EVALUATEbol(confld->conname,SubFunc,SubFuncArgs);
*/
										if(*field->data.string_value!=cvalue) return_value=FALSE;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										FINDRSCGETDOUBLE(rsrc,temp,&fvalue);
/*
										fvalue=EVALUATEdbl(confld->conname,SubFunc,SubFuncArgs);
*/
										if(*field->data.float_value!=fvalue) return_value=FALSE;
										break;
									case SHORTV:
									case SSHORTV:

										FINDRSCGETSHORT(rsrc,temp,&shvalue);
/*
										shvalue=EVALUATEsht(confld->conname,SubFunc,SubFuncArgs);
*/
										if(*field->data.short_value!=shvalue) return_value=FALSE;
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										FINDRSCGETINT(rsrc,temp,&ivalue);
/*
										ivalue=EVALUATEint(confld->conname,SubFunc,SubFuncArgs);
*/
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
	}
	return(return_value);
}
short PowerMasterBrowseBreakFunctionLPM(MakeBrowseList *BL)
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
				readscreen(rsrc,PSTRUCT);
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
short PowerMasterBrowseBreakFunction(MakeBrowseList *BL)
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
				readscreen(rsrc,PSTRUCT);
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
short PowerMasterBrowseSupBreakFunction2(MakeBrowseList *BL)
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
				rsrc=MTNMASTER->mtnrsrc;
				readscreen(rsrc,PSTRUCT);
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
void PowerMasterBrowseQuitFunction2(MakeBrowseList *BL)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDAsearch *sup=NULL;
	Browse_Exit *BE=NULL;

	if(BL!=NULL)
	{
		drsrc=BL->definelist;
		srsrc=BL->searchrsrc;
		brsrc=BL->mainrsrc;
		sup=(RDAsearch *)BL->funcargs;
		if(sup!=NULL)
		{
			sup->BL=NULL;
			if(sup->parent_rsrc!=NULL) sup->parent_rsrc=NULL;
		}
		if(drsrc!=NULL)
		{
			free_rsrc(drsrc);
			drsrc=NULL;
		}
		if(srsrc!=NULL)
		{
			free_rsrc(srsrc);
			srsrc=NULL;
		}
		BE=RETURN_BROWSE_EXIT(BL);
		if(BE!=NULL)
		{
			FreeBrowseExit(BE);
		}
		if(brsrc!=NULL)
		{
			free_rsrc(brsrc);
			brsrc=NULL;
		}
	}
}
void PowerMasterBrowseQuitFunction(MakeBrowseList *BL)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDAsearch *sup=NULL;

	if(BL!=NULL)
	{
		drsrc=BL->definelist;
		srsrc=BL->searchrsrc;
		brsrc=BL->mainrsrc;
		sup=(RDAsearch *)BL->funcargs;
		if(sup!=NULL)
		{
			if(sup->parent_rsrc!=NULL) sup->parent_rsrc=NULL;
		}
		if(drsrc!=NULL)
		{
			killwindow(drsrc);
		}
		if(srsrc!=NULL)
		{
			killwindow(srsrc);
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
		}
	}
}
static void SetPowerMasterNotInherited(RDArsrc *rsrc,RDAconfld *confld,MTNPassableStruct *PSTRUCT,NRDfield *field)
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
			confilename=stripmodulename(confld->conname);
			if(confilename!=NULL)
			{
				mainfilename=FILENAME(MTNMASTER->passkey->fileno);
				if(!RDAstrcmp(confilename,mainfilename))
				{
					confieldname=stripfilename(confld->conname);
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
short PowerMasterBrowseTestInherited(MakeBrowseList *BL,RDArmem *memeber)
{
	RDAsearch *sup=NULL;
	RDAconfld *c=NULL;
	short return_value=FALSE;
	int x=0;
	short keyno=0;
	short fileno=0;
	short len=0;
	void *comptarget=NULL;
	void *origtarget=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd==TRUE)
	{
		prterr("DIAG PowerMasterBrowseTestInherited().");
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
					if(sup->confld!=NULL)
					{
						for(x=0,c=sup->confld;x<sup->num;++x,++c)
						{
							if(c->inherit==TRUE)
							{
								return_value=TRUE;
							}
						 }
					} else {
						return_value=FALSE;
					}
					if(return_value==FALSE)
					{
						return(FALSE);
					}
					if(sup->keyno!=(-1) && sup->keyno>0)
					{
						keyno=sup->keyno;
					} else {
						keyno=1;
					}
					fileno=sup->fileno;
					origtarget=GETNRDKEY(fileno,keyno,&len);
					if(BL->list!=NULL)
					{
						if(BL->list->key!=NULL && BL->list->num>0)
						{
							comptarget=BL->list->key[0];
							if(ADVEQLNRDKEYsec(fileno,keyno,comptarget,MTNMASTER->SubFunc,MTNMASTER->SubFuncArgs))
							{
								KEYNRD(fileno,keyno);
								return_value=FALSE;
							} else {
								return_value=PowerMasterBrowseSupBreakFunction2(BL);
							}
						} else {
							return_value=FALSE;
						}
					} else {
						return_value=FALSE;
					}
					EQLNRDKEY(fileno,keyno,origtarget);
					if(origtarget!=NULL) Rfree(origtarget);
/* Commented Out line 2457
					if(comptarget!=NULL) Rfree(comptarget);
*/
					return(!return_value);
				}
				return(FALSE);
			}
			return(FALSE);
		}
		return(FALSE);
	}
	return(FALSE);
}
char PowerMasterSupOKFunction(RDAsearch *sup,void *target,RDArsrc *rsrc)
{
	RDAconfld *c=NULL;
	RDAconfld *confld=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	int x;
	int y;
	NRDfield *field=NULL;
	char *temp=NULL;
#ifdef XXXXX
	char apply_actions=FALSE;
#endif
	short keyno=1;
	char kill_browse=FALSE;
/*
	RDArsrc *rsrc=NULL;
*/
	void (*SubFunc)(...);
	void *SubFuncArgs;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG PowerMasterSupOKFunction().");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
/*
				if(sup->parent_rsrc!=NULL)
				{
					rsrc=sup->parent_rsrc;
				} else {
					rsrc=MTNMASTER->mtnrsrc;
				}
*/
				if(MTNMASTER->SubFunc!=NULL)
				{
					SubFunc=MTNMASTER->SubFunc;
				} else {
					SubFunc=SCRNvirtualSubData;;
				}
				if(MTNMASTER->SubFuncArgs!=NULL)
				{
					SubFuncArgs=MTNMASTER->SubFuncArgs;
				} else {
					SubFuncArgs=(void *)rsrc;
				}
				if(sup->confld!=NULL)
				{
					for(x=0,c=sup->confld;x<sup->num;++x,++c)
					{
						if(c->inherit==TRUE) kill_browse=TRUE;
					}
				} else { 
					kill_browse=TRUE;
				}
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
								SetPowerMasterNotInherited(rsrc,confld,PSTRUCT,field);
							}
						}
					}
#ifdef XXXXX
					if(apply_actions) ApplyActions(rsrc,MTNMASTER,SubFunc,SubFuncArgs,FALSE);
#endif /* XXXXX */
					IncVirRsrcstoFile(rsrc);
					GET_SUPPORTING(rsrc,MTNMASTER,SubFunc,SubFuncArgs,TRUE,FALSE);
					xcomputeandupdateallSCRNvirtuals(rsrc,SubFunc,SubFuncArgs,FALSE,FALSE,MTNMASTER,NULL,__LINE__,__FILE__);
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
/*
					if(kill_browse==TRUE) sup->BL=NULL;
*/
				}
/*
				} else {
					prterr("Error Parent Resource structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
				}
*/
			} else {
				prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
			}
		} else {
			prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else { 
		kill_browse=TRUE;
	}
	return(kill_browse);
}
void PowerMasterBrowseFreeFunction(MakeBrowseList *BL)
{
	RDAsearch *sup=NULL;
	Browse_Exit *BE=NULL;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

	if(BL!=NULL)
	{
		sup=(RDAsearch *)BL->funcargs;
		if(sup!=NULL)
		{
			drsrc=BL->definelist;
			srsrc=BL->searchrsrc;
			brsrc=BL->mainrsrc;
			if(drsrc!=NULL)
			{
				free_rsrc(drsrc);
				drsrc=NULL;
			}
			if(srsrc!=NULL)
			{
				free_rsrc(srsrc);
				srsrc=NULL;
			}
			BE=RETURN_BROWSE_EXIT(BL);
			if(BE!=NULL)
			{
				FreeBrowseExit(BE);
			}
			if(brsrc!=NULL)
			{
				free_rsrc(brsrc);
				brsrc=NULL;
			}
		}
	}
}
void PowerMasterBrowseOKFunction(MakeBrowseList *BL,void *target)
{
	RDAsearch *sup=NULL;
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDArsrc *rsrc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG PowerMasterBrowseOKFunction().");
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
					PowerMasterSupOKFunction(sup,target,rsrc);
					drsrc=BL->definelist;
					srsrc=BL->searchrsrc;
					brsrc=BL->mainrsrc;
					if(drsrc!=NULL)
					{
						killwindow(drsrc);
					}
					if(srsrc!=NULL)
					{
						killwindow(srsrc);
					}
					if(brsrc!=NULL)
					{
						killwindow(brsrc);
					}
				}
			}
		}
	}
}
void PowerMasterSupNextFunction(RDArmem *member,RDAsearch *sup)
{
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	short keyno=1;
	short fileno=(-1);
	void *target=NULL;
	void *origtarget=NULL;
	short len=0;
	RDArsrc *rsrc=NULL;
	void (*SubFunc)(...);
	void *SubFuncArgs;
	char return_value=TRUE;

	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
/*
				rsrc=MTNMASTER->mtnrsrc;
*/
				rsrc=(RDArsrc *)member->parent;
				if(MTNMASTER->SubFunc!=NULL)
				{
					SubFunc=MTNMASTER->SubFunc;
				} else {
					SubFunc=SCRNvirtualSubData;;
				}
				if(MTNMASTER->SubFuncArgs!=NULL)
				{
					SubFuncArgs=MTNMASTER->SubFuncArgs;
				} else {
					SubFuncArgs=(void *)rsrc;
				}
/*
				sup->parent_rsrc=(RDArsrc *)member->parent;
*/
				if(sup->keyno!=(-1) && sup->keyno>0)
				{
					keyno=sup->keyno;
				} else {
					keyno=1;
				}
				fileno=sup->fileno;
				origtarget=GETNRDKEY(fileno,keyno,&len);
				PowerMasterBrowseSupInitFunction(sup);
				if(ADVNXTNRDsec(fileno,keyno,SubFunc,SubFuncArgs))
				{
					ADVEQLNRDKEYsec(fileno,keyno,origtarget,MTNMASTER->SubFunc,MTNMASTER->SubFuncArgs);
					PowerMasterSupOKFunction(sup,origtarget,rsrc);
				} else {
					return_value=PowerMasterBrowseSupBreakFunction(sup,rsrc);
					if(return_value==FALSE)
					{
						ADVEQLNRDKEYsec(fileno,keyno,origtarget,MTNMASTER->SubFunc,MTNMASTER->SubFuncArgs);
						PowerMasterSupOKFunction(sup,origtarget,rsrc);
					} else {
						target=GETNRDKEY(fileno,keyno,&len);
						PowerMasterSupOKFunction(sup,target,rsrc);
					}
				}
				if(origtarget!=NULL) Rfree(origtarget);
				if(target!=NULL) Rfree(target);
			}
		}
	}
}
void PowerMasterSupPrevFunction(RDArmem *member,RDAsearch *sup)
{
	MaintainMaster *MTNMASTER=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	short keyno=1;
	short fileno=(-1);
	void *target=NULL;
	void *origtarget=NULL;
	short len=0;
	RDArsrc *rsrc=NULL;
	void (*SubFunc)(...);
	void *SubFuncArgs;
	char return_value=TRUE;

	if(sup!=NULL)
	{
		PSTRUCT=(MTNPassableStruct *)sup->parent;
		if(PSTRUCT!=NULL)
		{
			MTNMASTER=PSTRUCT->MTNMASTER;
			if(MTNMASTER!=NULL)
			{
/*
				rsrc=MTNMASTER->mtnrsrc;
*/
				rsrc=member->parent;
				if(MTNMASTER->SubFunc!=NULL)
				{
					SubFunc=MTNMASTER->SubFunc;
				} else {
					SubFunc=SCRNvirtualSubData;;
				}
				if(MTNMASTER->SubFuncArgs!=NULL)
				{
					SubFuncArgs=MTNMASTER->SubFuncArgs;
				} else {
					SubFuncArgs=(void *)rsrc;
				}
/*
				sup->parent_rsrc=(RDArsrc *)member->parent;
*/
				if(sup->keyno!=(-1) && sup->keyno>0)
				{
					keyno=sup->keyno;
				} else {
					keyno=1;
				}
				fileno=sup->fileno;
				origtarget=GETNRDKEY(fileno,keyno,&len);
				PowerMasterBrowseSupInitFunction(sup);
				if(ADVPRVNRDsec(fileno,keyno,SubFunc,SubFuncArgs))
				{
					ADVEQLNRDKEYsec(fileno,keyno,origtarget,MTNMASTER->SubFunc,MTNMASTER->SubFuncArgs);
					PowerMasterSupOKFunction(sup,origtarget,rsrc);
				} else {
					return_value=PowerMasterBrowseSupBreakFunction(sup,rsrc);
					if(return_value==FALSE)
					{
						ADVEQLNRDKEYsec(fileno,keyno,origtarget,MTNMASTER->SubFunc,MTNMASTER->SubFuncArgs);
						PowerMasterSupOKFunction(sup,origtarget,rsrc);
					} else {
						target=GETNRDKEY(fileno,keyno,&len);
						PowerMasterSupOKFunction(sup,target,rsrc);
					}
				}
				if(origtarget!=NULL) Rfree(origtarget);
				if(target!=NULL) Rfree(target);
			}
		}
	}
}
void PowerMasterBrowseFunction(RDArmem *member,RDAsearch *sup)
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
								PowerMasterBrowseQuitFunction2(sup->BL);
							}
							return;
						} else {
							PowerMasterBrowseFreeFunction(sup->BL);
							sup->BL=NULL;
						}
					} else {
						PowerMasterBrowseFreeFunction(sup->BL);
						sup->BL=NULL;
					}
				}	
				if(!isEMPTY(sup->browse_screen) && !isEMPTY(sup->define_list))
				{
					brsrc=RDArsrcNEW(MTNMASTER->module,sup->browse_screen);
					GET_SCREEN_VIRTUALS(brsrc,0);
					SCRNvirtual2rsrc(brsrc);
					drsrc=RDArsrcNEW(MTNMASTER->module,sup->define_list);
					addDFincvir(drsrc,sup->module,sup->filename,NULL,sup->fileno);
					GET_SCREEN_VIRTUALS(drsrc,1);
					srsrc=RDArsrcNEW(MTNMASTER->module,sup->search_browse);
					addDFincvir(srsrc,sup->module,sup->filename,NULL,sup->fileno);
					GET_SCREEN_VIRTUALS(srsrc,2);
					brsrc->scn=RDAscrnNEW(brsrc->module,brsrc->screen);
					if(findscrnbin(brsrc->scn)) 
					{
						prterr("Error:  Failed to read screen.");
					}
					MakeBrowseListTotals(brsrc,sup->fileno,drsrc);
					if(sup->keyno!=(-1) && sup->keyno>0)
					{
						keyno=sup->keyno;
					} else {
							keyno=1;
					}
					switch(sup->smart_browse)
					{
						default:
						case 0:
						case 1:
							sup->BL=xbrowse(sup->fileno,keyno,brsrc,srsrc,drsrc,PowerMasterBrowseInitFunction,PowerMasterBrowseBreakFunction,PowerMasterBrowseSelectFunction,PowerMasterBrowseQuitFunction,PowerMasterBrowseOKFunction,NULL,sup,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
							break;
						case 2:
							sup->BL=xbrowse(sup->fileno,keyno,brsrc,srsrc,drsrc,PowerMasterBrowseInitFunction,PowerMasterBrowseBreakFunction,PowerMasterBrowseSelectFunction,PowerMasterBrowseQuitFunction,PowerMasterBrowseOKFunction,NULL,sup,TRUE,NULL,0,0,0,0,NULL,0,2,__LINE__,__FILE__);
							break;
						case 3:
							sup->BL=xbrowse(sup->fileno,keyno,brsrc,srsrc,drsrc,PowerMasterBrowseInitFunctionLPM,PowerMasterBrowseBreakFunctionLPM,PowerMasterBrowseSelectFunction,PowerMasterBrowseQuitFunction,PowerMasterBrowseOKFunction,NULL,sup,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
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
void xMakePowerMasterButtons(RDArsrc *mainrsrc,MTNPassableStruct *PSTRUCT,
	void (*quitfunc)(...),int line,char *file)
{
	int x,y;
	MaintainButton *button=NULL;
	MaintainMaster *MTNMASTER=NULL;
	char *bname=NULL,ps=FALSE;
	RDAsearch *sup=NULL;
	SECmodule *m=NULL;
	char *mstring=NULL,*pstring=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MakePowerMasterButtons at line [%d] program [%s].",
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
					ps=TRUE;
					if(RDAstrcmp(button->progname,"dohttp") && RDAstrcmp(button->progname,"doarchive") && RDAstrcmp(button->progname,"rdaout") && RDAstrcmp(button->progname,"dquery"))
					{
					if(button->args!=NULL)
					{
						if(button->args->numlibs>=2)
						{
							if(button->evalargs==TRUE)
							{
								if(!isEMPTY(button->args->libs[0]))
								{
										mstring=EVALUATEstr(button->args->libs[0],SCRNvirtualSubData,mainrsrc);
								} 
								if(!isEMPTY(button->args->libs[1]))
								{
										pstring=EVALUATEstr(button->args->libs[1],SCRNvirtualSubData,mainrsrc);
								} 
							} else {
								mstring=stralloc(button->args->libs[0]);
								pstring=stralloc(button->args->libs[1]);
							}
							m=FINDMODSEC(mstring);
							if(m==NULL)
							{	
								ps=FALSE;
							} else if(m->execfunc==TRUE && m->allprocs==TRUE) 
							{
								ps=TRUE;
							} else if(m->execfunc==TRUE)
							{

								ps=PROCsecurity(mstring,pstring);
							} else ps=FALSE;
							if(mstring!=NULL) Rfree(mstring);
							if(pstring!=NULL) Rfree(pstring);
						}
					}
					}
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
						PowerMasterButtonFunction,button);
					if(ps==FALSE)
					{
						FINDRSCSETSENSITIVE(mainrsrc,button->buttonname,FALSE);
					}
				}
			}
			if(MTNMASTER->s!=NULL)
			{
				for(y=0,sup=MTNMASTER->s;y<MTNMASTER->num;++y,++sup)
				{
					if(sup->browse==TRUE) 
					{
						if(!isEMPTY(sup->browse_screen) && !isEMPTY(sup->define_list))
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
							MTNMASTER->mtnrsrc=mainrsrc;
							RMEMaddbtnrsrc(mainrsrc,bname,TRUE,PowerMasterBrowseFunction,sup);
							if(bname!=NULL) Rfree(bname);
						}
						if(!isEMPTY(sup->button_name))
						{
							bname=Rmalloc(strlen(sup->button_name)+6);
							sprintf(bname,"NEXT %s",sup->button_name);
						} else {
							bname=Rmalloc(strlen(sup->module)+strlen(sup->filename)+25);
							sprintf(bname,"NEXT %s %s %d",(sup->module!=NULL?sup->module:""),(sup->filename!=NULL?sup->filename:""),(y+1));
						}
						sup->parent=PSTRUCT;
						sup->parent_rsrc=mainrsrc;
						MTNMASTER->mtnrsrc=mainrsrc;
						RMEMaddbtnrsrc(mainrsrc,bname,TRUE,PowerMasterSupNextFunction,sup);
						if(bname!=NULL) Rfree(bname);
						if(!isEMPTY(sup->button_name))
						{
							bname=Rmalloc(strlen(sup->button_name)+11);
							sprintf(bname,"PREVIOUS %s",sup->button_name);
						} else {
							bname=Rmalloc(strlen(sup->module)+strlen(sup->filename)+30);
							sprintf(bname,"PREVIOUS %s %s %d",(sup->module!=NULL?sup->module:""),(sup->filename!=NULL?sup->filename:""),(y+1));
						}
						sup->parent=PSTRUCT;
						sup->parent_rsrc=mainrsrc;
						MTNMASTER->mtnrsrc=mainrsrc;
						RMEMaddbtnrsrc(mainrsrc,bname,TRUE,PowerMasterSupPrevFunction,sup);
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
void xMakePowerMasterBrowseButtons(RDArsrc *brsrc,MTNPassableStruct *PSTRUCT,
	void (*quitfunc)(...),int line,char *file)
{
	int x;
	MaintainButton *button=NULL;
	MaintainMaster *MTNMASTER=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MakePowerMasterBrowseButtons at line [%d] program [%s].",
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
					addbtnrsrc(brsrc,button->buttonname,TRUE,
						okBrowseButtonFunc,button);
				}
			}
		} else {
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error MTN Passable structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
