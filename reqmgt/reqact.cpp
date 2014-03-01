/* reqact.c - Requisition's Activity Maintain */
#ifndef WIN32
#define __NAM__ "reqact.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqact.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static void browse_reqact(RDArsrc *,RDATData *);
static MakeBrowseList *mbl_reqact=NULL;
static MaintainMaster *MTNMSTR=NULL;
static char editable=TRUE;
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
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *
);
static void seteditable(RDArsrc *,RDATData *,short);
static void seteditablecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
void reqactm(short,void *targetkey);

void reqactm(short dowhich,void *targetkey)
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
					set_appdefaults(mtnrsrc,FALSE);
				} else {
					GetRDATData(MTNMSTR->passkey->fileno,previous);
					filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,previous);
				filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MTNMSTR->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMSTR->passkey);
			if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMSTR->passkey);
				set_appdefaults(mtnrsrc,FALSE);
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,previous);
				filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
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
					set_appdefaults(mtnrsrc,FALSE);
				} else {
					GetRDATData(MTNMSTR->passkey->fileno,previous);
					filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,previous);
				filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MTNMSTR);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_reqact!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_reqact,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MTNMSTR);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMSTR);
	if(mbl_reqact!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_reqact==NULL?TRUE:FALSE));
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
                member->parent,keyno,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
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
        	member->parent,keyno,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MTNMSTR,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,set_appdefaultscb,previous,
		NULL,NULL,seteditablecb,previous);
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

	if(ADVRECORDsec(MTNMSTR->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMSTR->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		updatebrowse(update,mbl_reqact,MTNMSTR->passkey->fileno,MTNMSTR->module,mtnrsrc);
		if(mbl_reqact!=NULL)
		{
			quit_record(mtnrsrc,previous);
		} else {
			seteditable(mtnrsrc,previous,TRUE);
			GetRDATData(MTNMSTR->passkey->fileno,previous);
		}
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
static void set_appdefaults(RDArsrc *mtnrsrc,short update)
{
	char *date=NULL,*timex=NULL;

	FINDRSCSETSTRING(mtnrsrc,"[REQACT][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[REQACT][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[REQACT][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update)
	{
		updatersrc(mtnrsrc,"[REQACT][SOURCE USER]");
		updatersrc(mtnrsrc,"[REQACT][ENTRY DATE]");
		updatersrc(mtnrsrc,"[REQACT][ENTRY TIME]");
	}
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	set_appdefaults(mtnrsrc,TRUE);
}
static void seteditable(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[REQITM][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"[REQACT][DELETEFLAG]",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,FALSE,FALSE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,TRUE,TRUE);
	}
}
static void seteditablecb(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR,
        void (*SubFunc)(...),void *args,RDATData *previous)
{
	filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
        seteditable(mtnrsrc,previous,TRUE);
}
void browse_reqact(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->search_browse);
	definelist=RDArsrcNEW(MTNMSTR->module,MTNMSTR->define_list);
	ZERNRD(MTNMSTR->passkey->fileno);
	ApplyPassKey(MTNMSTR->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMSTR,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MTNMSTR);
	mbl_reqact=ADVPbrowse(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MTNMSTR->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="REQMGT",*mtnname="MTN REQACT";

	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMSTR=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MTNMSTR))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MTNMSTR->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MTNMSTR->module);
		ERRORDIALOG("MAINTAIN MTNMSTR NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMSTR->module,MTNMSTR->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MTNMSTR))
		{
			doexit(NULL);
			return;
		}
		MTNMSTR->passkey=READPassKey(filenum);
		MTNMSTR->inheritrsrcs=TRUE;
		switch(MTNMSTR->start_with)
		{
			default:
			case 0:
				reqactm(1,NULL);
				break;
			case 1:
				browse_reqact(NULL,NULL);
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
		CLOSE_MASTER_FILES(MTNMSTR);
		if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		ShutdownSubsystems();
	}
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) 
		{
			reqactm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMSTR);
		}
	} else {
		reqactm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	reqactm(2,NULL);
} 
