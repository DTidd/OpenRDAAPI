/* lvemst.c - Xpert Leave Master */
/*lint -library */
#ifndef WIN32
#define __NAM__ "lvemst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "lvemst.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short dscnum=(-1),lvetyp=(-1);
char editable=TRUE;
MakeBrowseList *mbl_mst=NULL;
MaintainMaster *MASTER=NULL;
void browse_mst(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,short,RDATData *);
static void save_check(RDArsrc *,short,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static short getsupportingrecords(RDArsrc *,short,short);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(),void *,RDATData *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(),void *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);
void lvemstm(short,void *);

static short getsupportingrecords(RDArsrc *mtnrsrc,short update,short display_diag)
{
	short return_value=TRUE;
	char delflag=FALSE;

	if(dscnum!=(-1))
	{
		COPYFIELD(MASTER->passkey->fileno,dscnum,"LEAVE DESCRIPTION IDENTIFICATION");
		if(ADVEQLNRDsec(dscnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(dscnum,1);
			if(display_diag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE DESCRIPTION IDENTIFICATION!","The Leave Description Identification is an invalid value.  It does not exist in the [LVEMGT][LVEDSC] data file.  Please select a valid leave description identification to save this record.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
		} else {
			if(display_diag)
			{
				FINDFLDGETCHAR(dscnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE DESCRIPTION IDENTIFICATION!","The Leave Description Identification is in the state of deletion in the [LVEMGT][LVEDSC] data file.  Please select a non-deleted leave description identification to save this record.",NULL,NULL,TRUE,NULL);
					return_value=FALSE;
				}
				if(lvetyp!=(-1))
				{
					COPYFIELD(dscnum,lvetyp,"LEAVE TYPE IDENTIFICATION");
					if(ADVEQLNRDsec(lvetyp,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(lvetyp,1);
					if(update) updatefilerecord2rsrc(lvetyp,mtnrsrc);
					else filerecord2rsrc(lvetyp,mtnrsrc);
				}
			}
		}
		if(update) updatefilerecord2rsrc(dscnum,mtnrsrc);
		else filerecord2rsrc(dscnum,mtnrsrc);
	}
	return(return_value);
}
void lvemstm(short dowhich,void *targetkey)
{
	char *name=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	short nofields=0,nokeys=0,x=0,y=0;
	short edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	
	ZERNRD(MASTER->passkey->fileno);
	previous=RDATDataNEW(MASTER->passkey->fileno);
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
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
			if(fldx->type!=SDECIMALV) edit_rsrc=editable;
			else edit_rsrc=(short)(editable &&
					RDAstrcmp(fldx->name,"PROCESSED EARNINGS") && 
					RDAstrcmp(fldx->name,"UNPROCESSED EARNINGS") && 
					RDAstrcmp(fldx->name,"PROCESSED USED") && 
					RDAstrcmp(fldx->name,"UNPROCESSED USED"));
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
	if(dscnum!=(-1)) file2rsrc(dscnum,mtnrsrc,FALSE);
	if(lvetyp!=(-1)) file2rsrc(lvetyp,mtnrsrc,FALSE);
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
	getsupportingrecords(mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MASTER);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_mst!=NULL)
	{
		addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_mst,previous);
	if(mbl_mst!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	MakeMasterButtons(mtnrsrc,MASTER);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_mst==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,TRUE,previous);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,FALSE,previous);
}
static void save_check(RDArsrc *mtnrsrc,short update_list,RDATData *previous)
{
	char *desc=NULL;

	readscreen(mtnrsrc,previous);
	if(!getsupportingrecords(mtnrsrc,TRUE,TRUE))
	{
		return;
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
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,update_list,previous);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,RDATData *previous)
{
	char *rcddesc=NULL;

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
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_mst,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_mst!=NULL) quit_record(mtnrsrc,previous);
			else GetRDATData(MASTER->passkey->fileno,previous);
	}
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
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),
	void *args,RDATData *previous)
{
	getsupportingrecords(mtnrsrc,TRUE,FALSE);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),
	void *args,RDATData *previous)
{
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	getsupportingrecords(mtnrsrc,TRUE,TRUE);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_PREVIOUS(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,previous,NULL,NULL,
		AUEQLfunction,previous);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);
	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_NEXT(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,previous,NULL,NULL,
		AUEQLfunction,previous);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,NEfunction,previous,NULL,NULL,AUEQLfunction,previous);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
void browse_mst(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	if(dscnum!=(-1))
	{
		COPYFIELD(MASTER->passkey->fileno,dscnum,"LEAVE DESCRIPTION IDENTIFICATION");
		if(ADVEQLNRDsec(dscnum,1,SCRNvirtualSubData,mainrsrc)) 
		{
			ZERNRD(dscnum);
		} else {
			file2rsrc(dscnum,mainrsrc,FALSE);
			filerecord2rsrc(dscnum,mainrsrc);
			if(lvetyp!=(-1))
			{
				COPYFIELD(dscnum,lvetyp,"LEAVE TYPE IDENTIFICATION");
				if(ADVEQLNRDsec(lvetyp,1,SCRNvirtualSubData,mainrsrc)) 
				{
					ZERNRD(lvetyp);
				} else {
					file2rsrc(lvetyp,mainrsrc,FALSE);
					filerecord2rsrc(lvetyp,mainrsrc);
				}
			}
		}
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_mst=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,searchrsrc,
		definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
		MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *module=NULL,*mtnname=NULL;

	module="LVEMGT";
	mtnname="MTN LVEMSTR";
	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process can not be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		dscnum=APPReturnSupportingFileno(MASTER,MASTER->module,"LVEDSC",1);
		lvetyp=APPReturnSupportingFileno(MASTER,MASTER->module,"LVETPMS",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=FALSE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				lvemstm(1,NULL);
				break;
			case 1:
				browse_mst(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL)
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
			blist->definelist=NULL;
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
			blist->searchrsrc=NULL;
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
			blist->mainrsrc=NULL;
		}
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
		exit(0);
	}
	mbl_mst=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) 
		{
			lvemstm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else lvemstm(0,targetkey);
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	lvemstm(2,NULL);
}
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,TRUE,previous);
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
	save_record(mtnrsrc,TRUE,previous);
}
static void save_record_now1(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,FALSE,previous);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
