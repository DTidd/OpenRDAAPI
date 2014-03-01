/* empabtr.c - Xpert Employee Job Absentee Transaction Master */
/*lint -library */
#ifndef WIN32
#define __NAM__ "empabtr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "empabtr.exe"
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

static MaintainMaster *MASTER=NULL;
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static short check_subid(RDArsrc *,char *,char *,char *);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void getsupportingrecords(RDArsrc *,short);
static void empabtrm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short jmstnum=(-1),dmg2num=(-1),posmstr=(-1);
static char editable=TRUE;
static MakeBrowseList *mbl_empabtr=NULL;
static void browse_empabtr(RDArsrc *,RDATData *);
static void Shutdown(void);
static void seteditable(RDArsrc *,short);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void SETID(RDArsrc *,RDATData *);
static void set_appdefaults(RDArsrc *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void readscreen(RDArsrc *);

static void Shutdown(void)
{
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
		exit(0);
	}
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,FALSE);
}
static void browse_empabtr(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(previous!=NULL) FreeRDATData(previous);
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_empabtr=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,searchrsrc,
		definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
		MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module=NULL,*mtnname=NULL;

	module="EMPABS";
	mtnname="MTN EMPABTR";
	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MASTER->module);
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
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
		jmstnum=APPReturnSupportingFileno(MASTER,"PAYROLL","PAYJMST",1);
		posmstr=APPReturnSupportingFileno(MASTER,"POSTRK","POSMSTR",1);
		dmg2num=APPReturnSupportingFileno(MASTER,"PRSNNL","PERDMG",2);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				empabtrm(1,NULL);
				break;
			case 1:
				browse_empabtr(NULL,NULL);
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
	mbl_empabtr=NULL;
	Shutdown();
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	empabtrm(0,targetkey);
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	empabtrm(2,NULL);
} 
static void empabtrm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short nofields=0,nokeys=0,x=0,y=0,edit_rsrc=FALSE;
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
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	if(posmstr!=(-1)) file2rsrc(posmstr,mtnrsrc,FALSE);
	if(dmg2num!=(-1)) singlefile2rsrc(dmg2num,mtnrsrc,FALSE);
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
					set_appdefaults(mtnrsrc);
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
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				set_appdefaults(mtnrsrc);
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					set_appdefaults(mtnrsrc);
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
        getsupportingrecords(mtnrsrc,FALSE);
	seteditable(mtnrsrc,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MASTER);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_empabtr!=NULL)
	{
		addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_empabtr,previous);
	if(mbl_empabtr!=NULL) 
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_empabtr==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
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
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,FALSE);
}
static short check_subid(RDArsrc *mtnrsrc,char *warnmessage,char *header,char *perid2)
{
	char delflag=FALSE;

	if(!isEMPTY(perid2))
	{
		FINDFLDSETSTRING(dmg2num,"PERSONNEL IDENTIFICATION",perid2);
		if(ADVEQLNRDsec(dmg2num,1,SCRNvirtualSubData,mtnrsrc))
		{
			sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is an invalid value.  It does not exist in the [PRSNNL][PERDMG] file.  Please select a valid Substitute Personnel Identification to save this record.",(perid2==NULL?"":perid2));
			sprintf(header,"INVALID SUBSTITUTE PERSONNEL IDENTIFICATION!");
			return(TRUE);
		} else {
			FINDFLDGETCHAR(dmg2num,"DELETEFLAG",&delflag);
			if(delflag)
			{
				sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is a deleted value.  It exist in the [PRSNNL][PERDMG] file but is in a deleted state.  Please select a valid Personnel Identification from this file to save this record.",(perid2==NULL?"":perid2));
				sprintf(header,"DELETED PERSONNEL IDENTIFICATION!");
				return(TRUE);
			}
		}
		return(FALSE);
	} else {
		return(FALSE);
	}
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char delflag=FALSE,*warnmessage=NULL,*header=NULL,*desc=NULL;
	char *perid1=NULL,*perid2=NULL;
	int jobno1=(-1);
	short warning=FALSE;

	readscreen(mtnrsrc);
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
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"SAVE WARNING",desc,
				(update_list?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	FINDRSCGETCHAR(mtnrsrc,"[EMPABTR][DELETEFLAG]",&delflag);
	FINDRSCGETSTRING(mtnrsrc,"[EMPABTR][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(mtnrsrc,"[EMPABTR][SUBSTITUTE PERSONNEL IDENTIFICATION]",&perid2);
	FINDRSCGETINT(mtnrsrc,"[EMPABTR][JOB NUMBER]",&jobno1);
	if(!delflag)
	{
		warnmessage=Rmalloc(500);
		header=Rmalloc(60);
		if(check_subid(mtnrsrc,warnmessage,header,perid2))
		{
			warning=TRUE;
		}
		if(warning) WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
	}
	if(header!=NULL) Rfree(header);
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(warning==FALSE) save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
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
		updatebrowse(update_list,mbl_empabtr,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_empabtr!=NULL) 
		{
			quit_record(mtnrsrc,previous);
		} else {
			GetRDATData(MASTER->passkey->fileno,previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) Rfree(previous);
	Shutdown();
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);
	RDArsrc *mtnrsrc;

	mtnrsrc=member->parent;
	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
	keyno=MASTER->passkey->keyno;
	}
	if(KEYNUM(MASTER->passkey->fileno,keyno)==NULL) keyno=1;
	readallwidgets(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	if(ADVEQLNRDsec(MASTER->passkey->fileno,keyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(MASTER->passkey->fileno,keyno);
		if(ADVLTENRDsec(MASTER->passkey->fileno,keyno,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(MASTER->passkey->fileno,keyno);
			ClearRDATData(previous);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(MASTER->inheritrsrcs)
			{
				updatefilersrc(MASTER->passkey->fileno,mtnrsrc);
				rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
			} else {
				updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			set_appdefaults(mtnrsrc);
			GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
		} else {
			GetRDATData(MASTER->passkey->fileno,previous);
        		updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,TRUE);
		}
	} else {
		ADVPRVNRDsec(MASTER->passkey->fileno,keyno,SCRNvirtualSubData,mtnrsrc);
		GetRDATData(MASTER->passkey->fileno,previous);
        	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
		GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,TRUE);
	}
	getsupportingrecords(mtnrsrc,TRUE);
	seteditable(mtnrsrc,TRUE);
	MSTupdateSCRNvirtuals(mtnrsrc,MASTER);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);
	RDArsrc *mtnrsrc;

	mtnrsrc=member->parent;
	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
	keyno=MASTER->passkey->keyno;
	}
	if(KEYNUM(MASTER->passkey->fileno,keyno)==NULL) keyno=1;
	readallwidgets(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	if(ADVEQLNRDsec(MASTER->passkey->fileno,keyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(MASTER->passkey->fileno,keyno);
		if(ADVGTENRDsec(MASTER->passkey->fileno,keyno,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(MASTER->passkey->fileno,keyno);
			ClearRDATData(previous);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(MASTER->inheritrsrcs)
			{
				updatefilersrc(MASTER->passkey->fileno,mtnrsrc);
				rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
			} else {
				updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			set_appdefaults(mtnrsrc);
			GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
		} else {
			GetRDATData(MASTER->passkey->fileno,previous);
        		updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,TRUE);
		}
	} else {
		ADVNXTNRDsec(MASTER->passkey->fileno,keyno,SCRNvirtualSubData,mtnrsrc);
		GetRDATData(MASTER->passkey->fileno,previous);
        	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
		GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,TRUE);
	}
	getsupportingrecords(mtnrsrc,TRUE);
	seteditable(mtnrsrc,TRUE);
	MSTupdateSCRNvirtuals(mtnrsrc,MASTER);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
        readallwidgets(mtnrsrc);
        ReadRDAScrolledLists(mtnrsrc);
        rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
        if(ADVEQLNRDsec(MASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
        {
                KEYNRD(MASTER->passkey->fileno,1);
                ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
		if(MASTER->inheritrsrcs)
                {
                        updatefilersrc(MASTER->passkey->fileno,mtnrsrc);
                        rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
                } else {
                        updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
                }
                ClearRDATData(previous);
		set_appdefaults(mtnrsrc);
                GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
        } else {
                GetRDATData(MASTER->passkey->fileno,previous);
        	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
                GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,TRUE);
        }
	getsupportingrecords(mtnrsrc,TRUE);
	seteditable(mtnrsrc,TRUE);
        MSTADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc,MASTER);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,mtnrsrc,TRUE,FALSE);
	getsupportingrecords(mtnrsrc,TRUE);
	seteditable(mtnrsrc,TRUE);
	MSTADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc,MASTER);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,short update)
{
	if(dmg2num!=(-1))
	{
		FIELDCOPY(MASTER->passkey->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",dmg2num,"PERSONNEL IDENTIFICATION");
		if(ADVEQLNRDsec(dmg2num,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(dmg2num,1);
		if(update) updatesinglerecord2rsrc(dmg2num,mtnrsrc);
		else singlefilerecord2rsrc(dmg2num,mtnrsrc);
	}
	if(posmstr!=(-1))
	{
		COPYFIELD(jmstnum,posmstr,"POSITION IDENTIFICATION");
		if(ADVEQLNRDsec(posmstr,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(posmstr,1);
		if(update) updatefilerecord2rsrc(posmstr,mtnrsrc);
		else filerecord2rsrc(posmstr,mtnrsrc);
	}
}
static void set_appdefaults(RDArsrc *mtnrsrc)
{
	char *date=NULL,*timex=NULL;

	FINDRSCSETSTRING(mtnrsrc,"[EMPABTR][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[EMPABTR][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[EMPABTR][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
	SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void seteditable(RDArsrc *mtnrsrc,short update)
{
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[EMPABS][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}	
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
		}
	}
}
