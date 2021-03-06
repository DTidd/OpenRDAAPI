/* append.c  Application Management Endorsement Master Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "append.lnx"
#endif
#ifdef WIN32
#define __NAM__ "append.exe"
#endif
#include <app.hpp>



#include <cstdlib>
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
static short appcert=(-1),appctyp=(-1);
static char editable=TRUE;
static MakeBrowseList *mbl_append=NULL;
static void browse_append(RDArsrc *,RDATData *);
static MaintainMaster *MASTER=NULL;
static void appendm(short,void *);
static void setlists(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_check(RDArsrc *,RDATData *,short);
static void getrecord(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void dotypelist(RDArsrc *,RDATData *);
static void settypelist(RDArsrc *,RDATData *);
static void gettype(RDArsrc *,RDATData *);
static void gettype_cb(RDArsrc *,RDATData *);
static short getsupportingrecords(RDArsrc *,short,short);
static void readscreen(RDArsrc *,RDATData *);

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
static void browse_append(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *searchrsrc=NULL,*definelist=NULL;
	RDArsrc *mainrsrc=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,
		SCRNvirtualSubData,(void *)mainrsrc);	
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	if(appcert!=(-1))
	{
		COPYFIELD(MASTER->passkey->fileno,appcert,"APPLICANT IDENTIFICATION");
		COPYFIELD(MASTER->passkey->fileno,appcert,"CERTIFICATE NUMBER");
		if(ADVEQLNRDsec(appcert,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(appcert);
		} else if(appctyp!=(-1))
		{
			COPYFIELD(appcert,appctyp,"TYPE");
			if(ADVEQLNRDsec(appctyp,1,SCRNvirtualSubData,mainrsrc))
			{
				ZERNRD(appctyp);
			} else {
				file2rsrc(appcert,mainrsrc,FALSE);
				filerecord2rsrc(appcert,mainrsrc);
				file2rsrc(appctyp,mainrsrc,FALSE);
				filerecord2rsrc(appctyp,mainrsrc);
			}
		}
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_append=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"APPMGT","MNT APPEND")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew("APPMGT","MTN APPEND");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process can not be executed without it's definition [%s] in the [%s.MTN] library.","MTN APPEND",MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"APPEND",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		appcert=APPReturnSupportingFileno(MASTER,MASTER->module,"APPCERT",1);
		appctyp=APPReturnSupportingFileno(MASTER,MASTER->module,"APPCTYP",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				appendm(1,NULL);
				break;
			case 1:
				browse_append(NULL,NULL);
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
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_append!=NULL) mbl_append=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			appendm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		appendm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *b)
{
	appendm(2,NULL);
} 
static void appendm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short nofields=0,nokeys=0,x,y,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDATData *previous=NULL;
	RDArsrc *mtnrsrc=NULL;

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
			edit_rsrc=(short)editable;
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
	if(appcert!=(-1)) file2rsrc(appcert,mtnrsrc,FALSE);
	if(appctyp!=(-1)) file2rsrc(appctyp,mtnrsrc,FALSE);
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
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,
				SCRNvirtualSubData,mtnrsrc))
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
	if(mbl_append!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_append,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	if(mbl_append!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_append==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
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
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,RDATData *previous)
{
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
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *desc=NULL;

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
	save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL;

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
		WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_append,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
  		if(mbl_append!=NULL) quit_record(mtnrsrc,previous);
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
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,RDATData *previous)
{
	getsupportingrecords(mtnrsrc,TRUE,FALSE);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,RDATData *previous)
{
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	getsupportingrecords(mtnrsrc,TRUE,TRUE);
}
static short getsupportingrecords(RDArsrc *mtnrsrc,short update,short display_diag)
{
	short return_value=TRUE;
	char delflag=FALSE;

	if(appcert!=(-1))
	{
		COPYFIELD(MASTER->passkey->fileno,appcert,"APPLICANT IDENTIFICATION");
		COPYFIELD(MASTER->passkey->fileno,appcert,"CERTIFICATE NUMBER");
		if(ADVEQLNRDsec(appcert,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(appcert,1);
			if(display_diag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID APPLICANT IDENTIFICATION!","The Certificate Number is an invalid value. It does not exist in the [APPMGT][APPCERT] data file.  Please select a valid certificate number to save this record.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
		} else {
			if(display_diag)
			{
			FINDFLDGETCHAR(appcert,"DELETEFLAG",&delflag);
			if(delflag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED APPLICANT IDENTIFICATION!","The Certificate Number is in the state of deletion in the [APPMGT][APPCERT] data file.  Please select a non-deleted certificate number to save this record.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
			if(appctyp!=(-1))
			{
				COPYFIELD(appcert,appctyp,"TYPE");
				if(ADVEQLNRDsec(appctyp,1,SCRNvirtualSubData,mtnrsrc)) 
					KEYNRD(appctyp,1);
				if(update) updatefilerecord2rsrc(appctyp,mtnrsrc);
				else filerecord2rsrc(appctyp,mtnrsrc);
			}
			}
		}
		if(update) updatefilerecord2rsrc(appcert,mtnrsrc);
		else filerecord2rsrc(appcert,mtnrsrc);
	}
	return(return_value);
}
