/* procsecm.c - Process Security Maintain Screen */
#include <cstdio>


#include <mix.hpp>

struct procsecms
{
	APPlib *scr_libs;
	RDATData *previous;
};
typedef struct procsecms procsecm;

extern short filenum,modnum,usernum;
extern char editable;
extern char *module;
extern MakeBrowseList *mbl_procsec;
extern void browse_procsec(RDArsrc *,procsecm *);
static void save_record(RDArsrc *,procsecm *,short);
static void save_check(RDArsrc *,procsecm *,short);
static void quit_record(RDArsrc *,procsecm *);
static void quit_record_test(RDArsrc *,procsecm *);
static void save_record_up(RDArsrc *,procsecm *);
static void save_record_nup(RDArsrc *,procsecm *);
static void getrecord(RDArsrc *,procsecm *);
static void getsupportingrecords(RDArsrc *,short);
static void getmod(RDArsrc *,procsecm *);
static void domodlist(RDArsrc *,procsecm *);
static void copymodname(RDArsrc *,procsecm *);
static void setmodname(RDArsrc *,procsecm *);
void mtnprocsec(short,void *);
extern MaintainMaster *MTNMASTER;

static void quit_record_test(RDArsrc *mtnrsrc,procsecm *prsc)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,prsc->previous))
	{
		desc=QUITvfydesc(filenum,prsc->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_up,quit_record,
			FALSE,2,mtnrsrc,prsc,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,prsc);
	}
}
void mtnprocsec(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	procsecm *prsc=NULL;
	APPlib *scr_libs;

	prsc=Rmalloc(sizeof(procsecm));
	mtnrsrc=NULL;
	prsc->scr_libs=NULL;
	ZERNRD(filenum);
	prsc->previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[PROCSEC][USER IDENTIFICATION]",getrecord,prsc);
	FINDRSCSETFUNC(mtnrsrc,"[PROCSEC][MODULE NAME]",copymodname,prsc);
	FINDRSCSETFUNC(mtnrsrc,"[PROCSEC][PROCESS NAME]",getrecord,prsc);
	if(modnum!=(-1)) file2rsrc(modnum,mtnrsrc,FALSE);
	if(usernum!=(-1)) file2rsrc(usernum,mtnrsrc,FALSE);
	if(prsc->scr_libs!=NULL) freeapplib(prsc->scr_libs);
	prsc->scr_libs=APPlibNEW();
	scr_libs=prsc->scr_libs;
	addlstrsrc(mtnrsrc,"MODULE LIST",0,TRUE,getmod,scr_libs->numlibs,&scr_libs->libs,prsc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				GetRDATData(filenum,prsc->previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(filenum,MTNMASTER->passkey->keyno,targetkey))
			{
				ZERNRD(filenum);
				ApplyPassKey(MTNMASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				GetRDATData(filenum,prsc->previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,FALSE);
	domodlist(mtnrsrc,prsc);
	setmodname(mtnrsrc,prsc);
	addDFincvir(mtnrsrc,module,"USERS",NULL,usernum);
	addDFincvir(mtnrsrc,module,"MODSEC",NULL,modnum);
	addDFincvir(mtnrsrc,module,"PROCSEC",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,prsc);
		if(mbl_procsec!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,prsc);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,prsc);
		}
	}
	if(mbl_procsec==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_procsec,prsc);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,prsc);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,prsc,(mbl_procsec==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,procsecm *prsc)
{
	save_check(mtnrsrc,prsc,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,procsecm *prsc)
{
	save_check(mtnrsrc,prsc,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,procsecm *prsc,short update_list)
{
	char deleteflag=0,*warnmessage=NULL,*userid=NULL;
	char *modname=NULL,*filename=NULL,*modulename=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[PROCSEC][DELETEFLAG]",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETSTRING(mtnrsrc,"[PROCSEC][USER IDENTIFICATION]",&userid);
		FINDFLDSETSTRING(usernum,"USER IDENTIFICATION",userid);
		if(EQLNRDsec(usernum,1))
		{
			filename=FILENAME(usernum);
			modulename=MODULENAME(usernum);
			warnmessage=Rmalloc(RDAstrlen(userid)+RDAstrlen(filename)+RDAstrlen(modulename)+150);
			sprintf(warnmessage,"The User Identification of [%s] does not exist in the file [%s][%s].  Before this record can be saved it must have a valid User Identifcation.",(userid==NULL?"":userid),modulename,filename);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID USER IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDFLDGETCHAR(usernum,"DELETEFLAG",&deleteflag);
			if(deleteflag)
			{
				filename=FILENAME(usernum);
				modulename=MODULENAME(usernum);
				warnmessage=Rmalloc(RDAstrlen(userid)+RDAstrlen(filename)+RDAstrlen(modulename)+150);
				sprintf(warnmessage,"The User Identification of [%s] is flagged to be deleted in the file [%s][%s].  Therefore, this record cannot be saved.",(userid==NULL?"":userid),modulename,filename);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED USER IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
			}
		}
		FINDRSCGETSTRING(mtnrsrc,"[PROCSEC][MODULE NAME]",&modname);
		FINDFLDSETSTRING(modnum,"USER IDENTIFICATION",userid);
		FINDFLDSETSTRING(modnum,"MODULE NAME",modname);
		if(EQLNRDsec(modnum,1))
		{
			filename=FILENAME(modnum);
			modulename=MODULENAME(modnum);
			warnmessage=Rmalloc(RDAstrlen(modname)+RDAstrlen(filename)+RDAstrlen(modulename)+150);
			sprintf(warnmessage,"The Module Name of [%s] does not exist in the file [%s][%s].  Before this record can be saved it must have a valid Module Name.",(modname==NULL?"":modname),modulename,filename);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID MODULE NAME!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDFLDGETCHAR(modnum,"DELETEFLAG",&deleteflag);
			if(deleteflag)
			{
				filename=FILENAME(modnum);
				modulename=MODULENAME(modnum);
				warnmessage=Rmalloc(RDAstrlen(modname)+RDAstrlen(filename)+RDAstrlen(modulename)+150);
				sprintf(warnmessage,"The Module Name of [%s] is flagged to be deleted in the file [%s][%s].  Therefore, this record cannot be saved.",(modname==NULL?"":modname),modulename,filename);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED MODULE NAME!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
			}
		}
		if(userid!=NULL) Rfree(userid);
		if(modname!=NULL) Rfree(modname);
	}
	save_record(mtnrsrc,prsc,update_list);
}
static void save_record(RDArsrc *mtnrsrc,procsecm *prsc,short update_list)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(RECORDsec(filenum))
	{
		desc=RECORDsecDesc(filenum);
		if(desc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",desc,NULL,FALSE);
			Rfree(desc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,prsc->previous);
		updatebrowse(update_list,mbl_procsec,filenum,module,mtnrsrc);
		if(mbl_procsec!=NULL) quit_record(mtnrsrc,prsc);
		else GetRDATData(filenum,prsc->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,procsecm *prsc)
{
	if(prsc!=NULL)
	{
		if(prsc->scr_libs!=NULL) freeapplib(prsc->scr_libs);
		if(prsc->previous!=NULL) FreeRDATData(prsc->previous);
		Rfree(prsc);
	}
	if(WindowCount==1)
	{
		CLSNRD(filenum);
		CLSNRD(modnum);
		CLSNRD(usernum);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
		std::exit;
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecord(RDArsrc *mtnrsrc,procsecm *prsc)
{
	char *luserid=NULL,*lmodname=NULL,*lprocname=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[PROCSEC][USER IDENTIFICATION]",&luserid)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[PROCSEC][MODULE NAME]",&lmodname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[PROCSEC][PROCESS NAME]",&lprocname)) return;
	FINDFLDSETSTRING(filenum,"MODULE NAME",lmodname);
	FINDFLDSETSTRING(filenum,"USER IDENTIFICATION",luserid);
	FINDFLDSETSTRING(filenum,"PROCESS NAME",lprocname);
	if(luserid!=NULL) Rfree(luserid);
	if(lmodname!=NULL) Rfree(lmodname);
	if(lprocname!=NULL) Rfree(lprocname);
	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		ClearRDATData(prsc->previous);
		updatefilersrc(filenum,mtnrsrc);
	} else {
		GetRDATData(filenum,prsc->previous);
		updatefilerecord2rsrc(filenum,mtnrsrc);	
	}
	getsupportingrecords(mtnrsrc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,short update)
{
	if(usernum!=(-1))
	{
		ZERNRD(usernum);
		COPYFIELD(filenum,usernum,"USER IDENTIFICATION");
		if(EQLNRDsec(usernum,1)) KEYNRD(usernum,1);
		if(update) updatefilerecord2rsrc(usernum,mtnrsrc);
		else filerecord2rsrc(usernum,mtnrsrc);
	}
	if(modnum!=(-1))
	{
		ZERNRD(modnum);
		COPYFIELD(filenum,modnum,"USER IDENTIFICATION");
		COPYFIELD(filenum,modnum,"MODULE NAME");
		if(EQLNRDsec(modnum,1)) KEYNRD(modnum,1);
		if(update) updatefilerecord2rsrc(modnum,mtnrsrc);
		else filerecord2rsrc(modnum,mtnrsrc);
	}
}
static void domodlist(RDArsrc *mtnrsrc,procsecm *prsc)
{
	int x=0;
	char *tmp=NULL,*directory=NULL;
	APPlib *scr_libs;

	directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(directory,"%s/rda/",CURRENTDIRECTORY);
	if(prsc->scr_libs!=NULL) freeapplib(prsc->scr_libs);
	prsc->scr_libs=APPlibNEW();
	for(x=0;findfile(directory,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(prsc->scr_libs,tmp);
	}
	scr_libs=prsc->scr_libs;
	if(scr_libs->numlibs<1)
	{
		addAPPlib(prsc->scr_libs,"Directory Contains No Modules");
	}
	SORTAPPlib(prsc->scr_libs);
	x=0;
	FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,prsc->scr_libs);
	if(tmp!=NULL) Rfree(tmp);
	if(directory!=NULL) Rfree(directory);
}
static void getmod(RDArsrc *mtnrsrc,procsecm *prsc)
{
	int s=0;
	APPlib *scr_libs;

	if(FINDRSCGETINT(mtnrsrc,"MODULE LIST",&s)) return;
	scr_libs=prsc->scr_libs;
	if(FINDRSCSETSTRING(mtnrsrc,"[PROCSEC][MODULE NAME]",scr_libs->libs[s])) return;
	if(FINDFLDSETSTRING(filenum,"MODULE NAME",scr_libs->libs[s])) return;
	updatersrc(mtnrsrc,"[PROCSEC][MODULE NAME]");
	getrecord(mtnrsrc,prsc);
}
static void copymodname(RDArsrc *mtnrsrc,procsecm *prsc)
{
	int x;
	APPlib *scr_libs;
	char *lmodname=NULL;

	getrecord(mtnrsrc,prsc);
	FINDFLDGETSTRING(filenum,"MODULE NAME",&lmodname);
	if(lmodname!=NULL && RDAstrlen(lmodname)>0)
	{
		scr_libs=prsc->scr_libs;
		for(x=0;x<scr_libs->numlibs;++x)
		{
			if(!RDAstrcmp(lmodname,scr_libs->libs[x])) break;
		}
		if(x>=scr_libs->numlibs) 
		{
			addAPPlib(prsc->scr_libs,lmodname);
			SORTAPPlib(prsc->scr_libs);
			scr_libs=prsc->scr_libs;
			for(x=0;x<scr_libs->numlibs;++x)
			{
				if(!RDAstrcmp(lmodname,scr_libs->libs[x])) break;
			}
			FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,prsc->scr_libs);
		} else {
			FINDRSCSETINT(mtnrsrc,"MODULE LIST",x);
		}
	} else {
		getmod(mtnrsrc,prsc);
	}
	if(lmodname!=NULL) Rfree(lmodname);
	updatersrc(mtnrsrc,"MODULE LIST");
	updateSCRNvirtuals(mtnrsrc);
}
static void setmodname(RDArsrc *mtnrsrc,procsecm *prsc)
{
	int x;
	APPlib *scr_libs;
	char *lmodname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[MODSEC][MODULE NAME]",&lmodname);
	if(lmodname!=NULL && RDAstrlen(lmodname)>0)
	{
		scr_libs=prsc->scr_libs;
		for(x=0;x<scr_libs->numlibs;++x)
		{
			if(!RDAstrcmp(lmodname,scr_libs->libs[x])) break;
		}
		if(x>=scr_libs->numlibs) 
		{
			addAPPlib(prsc->scr_libs,lmodname);
			SORTAPPlib(prsc->scr_libs);
			scr_libs=prsc->scr_libs;
			for(x=0;x<scr_libs->numlibs;++x)
			{
				if(!RDAstrcmp(lmodname,scr_libs->libs[x])) break;
			}
			FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,prsc->scr_libs);
		} else {
			FINDRSCSETINT(mtnrsrc,"MODULE LIST",x);
		}
	} else {
		x=0;
		FINDRSCSETSTRING(mtnrsrc,"[PROCSEC][MODULE NAME]",prsc->scr_libs->libs[x]);
	}
	if(lmodname!=NULL) Rfree(lmodname);
}
