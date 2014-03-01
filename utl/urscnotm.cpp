/* urscnotm.c - User Resource Notes Maintain Screen */
#include <cstdio>


#include <mix.hpp>

extern short filenum,usernum,rscnot,scnnot;
extern char editable;
extern char *module;
extern MakeBrowseList *mbl_urscnot;
extern void browse_urscnot(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *),getrecord(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void getsupportingrecords(RDArsrc *,short);
void mtnurscnot(short,void *);
extern MaintainMaster *MASTER;

static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,previous))
	{
		desc=QUITvfydesc(filenum,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_up,quit_record,FALSE,2,mtnrsrc,previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}

void mtnurscnot(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[URSCNOT][MODULE NAME]",getrecord,previous);
	FINDRSCSETFUNC(mtnrsrc,"[URSCNOT][SCREEN NAME]",getrecord,previous);
	FINDRSCSETFUNC(mtnrsrc,"[URSCNOT][RESOURCE NAME]",getrecord,previous);
	FINDRSCSETFUNC(mtnrsrc,"[URSCNOT][USER IDENTIFICATION]",getrecord,previous);
	if(scnnot!=(-1)) file2rsrc(scnnot,mtnrsrc,FALSE);
	if(rscnot!=(-1)) file2rsrc(rscnot,mtnrsrc,FALSE);
	if(usernum!=(-1)) file2rsrc(usernum,mtnrsrc,FALSE);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(EQLNRDsec(filenum,MASTER->passkey->keyno))
			{
				KEYNRD(filenum,MASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(filenum,MASTER->passkey->keyno,targetkey))
			{
				ZERNRD(filenum);
				ApplyPassKey(MASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,FALSE);
	addDFincvir(mtnrsrc,"SECURITY","USERS",NULL,usernum);
	addDFincvir(mtnrsrc,module,"SCNNOT",NULL,scnnot);
	addDFincvir(mtnrsrc,module,"URSCNOT",NULL,rscnot);
	addDFincvir(mtnrsrc,module,"URSCNOT",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_urscnot!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
		}
	}
	if(mbl_urscnot==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_urscnot,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_urscnot==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char deleteflag=0,*warnmessage=NULL,*scrname=NULL,*modname=NULL,*rscname=NULL;
	char *filename=NULL,*modulename=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[URSCNOT][DELETEFLAG]",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETSTRING(mtnrsrc,"[URSCNOT][MODULE NAME]",&modname);
		FINDRSCGETSTRING(mtnrsrc,"[URSCNOT][SCREEN NAME]",&scrname);
		FINDFLDSETSTRING(scnnot,"MODULE NAME",modname);
		FINDFLDSETSTRING(scnnot,"SCREEN NAME",scrname);
		if(EQLNRDsec(scnnot,1))
		{
			filename=FILENAME(scnnot);
			modulename=MODULENAME(scnnot);
			warnmessage=Rmalloc(RDAstrlen(modname)+RDAstrlen(filename)+RDAstrlen(modulename)+160);
			sprintf(warnmessage,"The Module Name of [%s] and the Screen Name of [%s] do not co-exist in the file [%s][%s].  Before this record can be saved these values must be valid.",(modname==NULL?"":modname),(scrname==NULL?"":scrname),modulename,filename);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID MODULE NAME AND SCREEN NAME!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDFLDGETCHAR(scnnot,"DELETEFLAG",&deleteflag);
			if(deleteflag)
			{
				filename=FILENAME(scnnot);
				modulename=MODULENAME(scnnot);
				warnmessage=Rmalloc(RDAstrlen(modname)+RDAstrlen(filename)+RDAstrlen(modulename)+160);
				sprintf(warnmessage,"The record containing Module Name of [%s] and the Screen Name of [%s] is flagged to be deleted in the file [%s][%s].  Therefore, this record cannot be saved.",(modname==NULL?"":modname),(scrname==NULL?"":scrname),modulename,filename);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED MODULE NAME AND SCREEN NAME!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
			}
		}
		FINDRSCGETSTRING(mtnrsrc,"[URSCNOT][RESOURCE NAME]",&rscname);
		FINDFLDSETSTRING(rscnot,"MODULE NAME",modname);
		FINDFLDSETSTRING(rscnot,"SCREEN NAME",scrname);
		FINDFLDSETSTRING(rscnot,"RESOURCE NAME",rscname);
		if(EQLNRDsec(rscnot,1))
		{
			filename=FILENAME(rscnot);
			modulename=MODULENAME(rscnot);
			warnmessage=Rmalloc(RDAstrlen(modname)+RDAstrlen(filename)+RDAstrlen(modulename)+160);
			sprintf(warnmessage,"The Resource Name of [%s] does not exist in the file [%s][%s].  Before this record can be saved it must have a valid resource name.",(rscname==NULL?"":rscname),modulename,filename);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID RESOURCE NAME!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDFLDGETCHAR(rscnot,"DELETEFLAG",&deleteflag);
			if(deleteflag)
			{
				filename=FILENAME(rscnot);
				modulename=MODULENAME(rscnot);
				warnmessage=Rmalloc(RDAstrlen(modname)+RDAstrlen(filename)+RDAstrlen(modulename)+160);
				sprintf(warnmessage,"The Resource Name of [%s] is flagged to be deleted in the file [%s][%s].  Therefore, this record cannot be saved.",(rscname==NULL?"":rscname),modulename,filename);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED RESOURCE NAME!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
			}
		}
		if(modname!=NULL) Rfree(modname);
		if(scrname!=NULL) Rfree(scrname);
		if(rscname!=NULL) Rfree(rscname);
	}
	save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,previous);
		updatebrowse(update_list,mbl_urscnot,filenum,module,mtnrsrc);
		if(mbl_urscnot!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(filenum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(usernum!=(-1)) CLSNRD(usernum);
		if(scnnot!=(-1)) CLSNRD(scnnot);
		if(rscnot!=(-1)) CLSNRD(rscnot);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
		std::exit;
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecord(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *modname=NULL,*scrname=NULL,*rscname=NULL,*userid=NULL; 

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[URSCNOT][MODULE NAME]",&modname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[URSCNOT][SCREEN NAME]",&scrname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[URSCNOT][RESOURCE NAME]",&rscname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[URSCNOT][USER IDENTIFICATION]",&userid)) return;
	FINDFLDSETSTRING(filenum,"SCREEN NAME",scrname);
	FINDFLDSETSTRING(filenum,"MODULE NAME",modname);
	FINDFLDSETSTRING(filenum,"RESOURCE NAME",rscname);
	FINDFLDSETSTRING(filenum,"USER IDENTIFICATION",userid);
	if(EQLNRDsec(filenum,MASTER->passkey->keyno))
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(previous);
		updatefilersrc(filenum,mtnrsrc);
	} else {
		GetRDATData(filenum,previous);
		updatefilerecord2rsrc(filenum,mtnrsrc);	
	}
	if(modname!=NULL) Rfree(modname);
	if(scrname!=NULL) Rfree(scrname);
	if(rscname!=NULL) Rfree(rscname);
	if(userid!=NULL) Rfree(userid);
	getsupportingrecords(mtnrsrc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,short update)
{
	if(usernum!=(-1))
	{
		COPYFIELD(filenum,usernum,"USER IDENTIFICATION");
		if(EQLNRDsec(usernum,1)) KEYNRD(usernum,1);
		if(update) updatefilerecord2rsrc(usernum,mtnrsrc);
		else filerecord2rsrc(usernum,mtnrsrc);
	}
	if(scnnot!=(-1))
	{
		COPYFIELD(filenum,scnnot,"MODULE NAME");
		COPYFIELD(filenum,scnnot,"SCREEN NAME");
		if(EQLNRDsec(scnnot,1)) KEYNRD(scnnot,1);
		if(update) updatefilerecord2rsrc(scnnot,mtnrsrc);
		else filerecord2rsrc(scnnot,mtnrsrc);
	}	
	if(rscnot!=(-1))
	{
		COPYFIELD(filenum,rscnot,"MODULE NAME");
		COPYFIELD(filenum,rscnot,"SCREEN NAME");
		COPYFIELD(filenum,rscnot,"RESOURCE NAME");
		if(EQLNRDsec(rscnot,1)) KEYNRD(rscnot,1);
		if(update) updatefilerecord2rsrc(rscnot,mtnrsrc);
		else filerecord2rsrc(rscnot,mtnrsrc);
	}	
}
