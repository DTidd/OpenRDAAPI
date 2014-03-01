/****************************************************
	rscnotm.c - Resource Notes Maintain Screen
*****************************************************/
#include <cstdio>

#include <mix.hpp>

extern short filenum,scnnot;
extern char editable;
extern char *module;
extern MakeBrowseList *mbl_rscnot;
extern void browse_rscnot(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void getrecord(RDArsrc *,RDATData *);
static void getsupportingrecords(RDArsrc *,short);
void mtnrscnot(short,void *);
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
void mtnrscnot(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[RSCNOT][MODULE NAME]",getrecord,previous);
	FINDRSCSETFUNC(mtnrsrc,"[RSCNOT][SCREEN NAME]",getrecord,previous);
	FINDRSCSETFUNC(mtnrsrc,"[RSCNOT][RESOURCE NAME]",getrecord,previous);
	if(scnnot!=(-1)) file2rsrc(scnnot,mtnrsrc,FALSE);
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
	addDFincvir(mtnrsrc,module,"SCNNOT",NULL,scnnot);
	addDFincvir(mtnrsrc,module,"RSCNOT",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_rscnot!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
		}
	}
	if(mbl_rscnot==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_rscnot,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_rscnot==NULL?TRUE:FALSE));
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
	char deleteflag=0,*warnmessage=NULL,*scrname=NULL,*modname=NULL;
	char *filename=NULL,*modulename=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[RSCNOT][DELETEFLAG]",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETSTRING(mtnrsrc,"[RSCNOT][MODULE NAME]",&modname);
		FINDRSCGETSTRING(mtnrsrc,"[RSCNOT][SCREEN NAME]",&scrname);
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
			if(modname!=NULL) Rfree(modname);
			if(scrname!=NULL) Rfree(scrname);
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
				if(modname!=NULL) Rfree(modname);
				if(scrname!=NULL) Rfree(scrname);
			}
		}
	}
	save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL,*mod=NULL,*scn=NULL,*rsc=NULL;
	char *urmod=NULL,*urscn=NULL,*urrsc=NULL;
	char deleteflag=0,urdeleteflag=0;
	short urscnot=0;
	short ef;
	
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Acces Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			urscnot=OPNNRDsec(module,"URSCNOT",TRUE,TRUE,FALSE);
			FINDFLDGETSTRING(filenum,"MODULE NAME",&mod);
			FINDFLDGETSTRING(filenum,"SCREEN NAME",&scn);
			FINDFLDGETSTRING(filenum,"RESOURCE NAME",&rsc);
			if(urscnot!=(-1))
			{
				ZERNRD(urscnot);
				FINDFLDSETSTRING(urscnot,"RESOURCE NAME",rsc);
				FINDFLDSETSTRING(urscnot,"MODULE NAME",mod);
				FINDFLDSETSTRING(urscnot,"SCREEN NAME",scn);
				ef=GTENRDsec(urscnot,1);
				while(!ef)
				{
					FINDFLDGETCHAR(urscnot,"DELETEFLAG",&urdeleteflag);
					if(!urdeleteflag)
					{
						FINDFLDGETSTRING(urscnot,"MODULE NAME",&urmod);		
						FINDFLDGETSTRING(urscnot,"SCREEN NAME",&urscn);		
						FINDFLDGETSTRING(urscnot,"RESOURCE NAME",&urrsc);		
						if(RDAstrcmp(mod,urmod) || RDAstrcmp(scn,urscn) || RDAstrcmp(rsc,urrsc)) break;
						FINDFLDSETCHAR(urscnot,"DELETEFLAG",deleteflag);
						WRTNRDtranssec(urscnot,0,NULL);
					}						
					ef=NXTNRDsec(urscnot,1);
				}
			}
			if(urscnot!=(-1)) CLSNRD(urscnot);
		}
		if(mod!=NULL) Rfree(mod);
		if(scn!=NULL) Rfree(scn);
		if(rsc!=NULL) Rfree(rsc);
		if(urmod!=NULL) Rfree(urmod);
		if(urscn!=NULL) Rfree(urscn);
		if(urrsc!=NULL) Rfree(urrsc);
		updatebrowse(update_list,mbl_rscnot,filenum,module,mtnrsrc);
		if(mbl_rscnot!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(filenum,previous);
    	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(scnnot!=(-1)) CLSNRD(scnnot);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSecurity();
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
	char *modname=NULL,*scrname=NULL,*rscname=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[RSCNOT][MODULE NAME]",&modname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[RSCNOT][SCREEN NAME]",&scrname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[RSCNOT][RESOURCE NAME]",&rscname)) return;
	FINDFLDSETSTRING(filenum,"MODULE NAME",modname);
	FINDFLDSETSTRING(filenum,"SCREEN NAME",scrname);
	FINDFLDSETSTRING(filenum,"RESOURCE NAME",rscname);
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
	getsupportingrecords(mtnrsrc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,short update)
{
	if(scnnot!=(-1))
	{
		COPYFIELD(filenum,scnnot,"MODULE NAME");
		COPYFIELD(filenum,scnnot,"SCREEN NAME");
		if(EQLNRDsec(scnnot,1)) KEYNRD(scnnot,1);
		if(update) updatefilerecord2rsrc(scnnot,mtnrsrc);
		else filerecord2rsrc(scnnot,mtnrsrc);
	}	
}
