/* fldsecm.c - Field Security Maintain Screen */
#include <cstdio>


#include <gui.hpp>
#include <nrd.hpp>
#include <mix.hpp>

struct fldsecms 
{
	APPlib *modlst;
	APPlib *file_defs;
	APPlib *field_defs;
	RDATData *previous;
};
typedef struct fldsecms fldsecm;

extern short filenum,modnum,usernum,fnum;
extern char editable;
extern char *module;
extern MakeBrowseList *mbl_fldsec;
extern void browse_fldsec(RDArsrc *,fldsecm *);
static void save_record(RDArsrc *,fldsecm *,short);
static void save_check(RDArsrc *,fldsecm *,short);
static void quit_record(RDArsrc *,fldsecm *);
static void quit_record_test(RDArsrc *,fldsecm *);
static void save_record_up(RDArsrc *,fldsecm *);
static void save_record_nup(RDArsrc *,fldsecm *);
static void setmodname(RDArsrc *,fldsecm *,short);
static void setmodname1(RDArsrc *,fldsecm *,short);
static void setmodnotempty(RDArsrc *,fldsecm *,char *,short);
static void setfilenotempty(RDArsrc *,fldsecm *,char *,short);
static void setfieldnotempty(RDArsrc *,fldsecm *,char *,short);
static void setfilename(RDArsrc *,fldsecm *,short);
static void setfilename1(RDArsrc *,fldsecm *,short);
static void setfieldname(RDArsrc *,fldsecm *,short);
static void setfieldname1(RDArsrc *,fldsecm *,short);
static void getrecordcb(RDArsrc *,fldsecm *);
static void modinputcb(RDArsrc *,fldsecm *);
static void modinput(RDArsrc *,fldsecm *,short);
static void fileinputcb(RDArsrc *,fldsecm *);
static void fileinput(RDArsrc *,fldsecm *,short);
static void fieldinputcb(RDArsrc *,fldsecm *);
static void fieldinput(RDArsrc *,fldsecm *,short);
static void getrecord(RDArsrc *,fldsecm *);
static void domodlist(RDArsrc *,fldsecm *);
static void dofilelist(RDArsrc *,fldsecm *,short);
static void dofieldlist(RDArsrc *,fldsecm *,short);
static void getmodcb(RDArsrc *,fldsecm *);
static void getfilecb(RDArsrc *,fldsecm *);
static void getfieldcb(RDArsrc *,fldsecm *);
static void getfield(RDArsrc *,fldsecm *,short);
static void getsupportingrecords(RDArsrc *,short);
void mtnfldsec(short,void *);
extern MaintainMaster *MASTER;

static void getpreviouscb(RDArmem *member,fldsecm *fdsc)
{
	short keyno=(-1);
	
	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,fdsc->previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,NULL,NULL,
		NULL,fdsc);
}
static void getnextcb(RDArmem *member,fldsecm *fdsc)
{
	short keyno=(-1);
	
	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MASTER,fdsc->previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,NULL,NULL,
		NULL,fdsc);
}
static void quit_record_test(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,fdsc->previous))
	{
		desc=QUITvfydesc(filenum,fdsc->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_up,quit_record,
			FALSE,2,mtnrsrc,fdsc,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,fdsc);
	}
}
void mtnfldsec(short dowhich,void *targetkey)
{
	fldsecm *fdsc=NULL;
	APPlib *modlst=NULL,*file_defs=NULL,*field_defs=NULL;
	RDArsrc *mtnrsrc=NULL;
	NRDkey *keys=NULL;
	short nokeys=0;

	fdsc=Rmalloc(sizeof(fldsecm));
	fdsc->modlst=NULL;
	fdsc->file_defs=NULL;
	fdsc->field_defs=NULL;
	ZERNRD(filenum);
	fdsc->previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[FLDSEC][USER IDENTIFICATION]",getrecordcb,fdsc);
	FINDRSCSETFUNC(mtnrsrc,"[FLDSEC][MODULE NAME]",modinputcb,fdsc);
	FINDRSCSETFUNC(mtnrsrc,"[FLDSEC][FILE NAME]",fileinputcb,fdsc);
	FINDRSCSETFUNC(mtnrsrc,"[FLDSEC][FIELD NAME]",fieldinputcb,fdsc);
	if(modnum!=(-1)) file2rsrc(modnum,mtnrsrc,FALSE);
	if(usernum!=(-1)) file2rsrc(usernum,mtnrsrc,FALSE);
	if(fnum!=(-1)) file2rsrc(fnum,mtnrsrc,FALSE);
	if(fdsc->modlst!=NULL) freeapplib(fdsc->modlst); 
	if(fdsc->file_defs!=NULL) freeapplib(fdsc->file_defs);
	if(fdsc->field_defs!=NULL) freeapplib(fdsc->field_defs); 
	fdsc->modlst=APPlibNEW();
	fdsc->file_defs=APPlibNEW();
	fdsc->field_defs=APPlibNEW();
	addlstrsrc(mtnrsrc,"MODULE LIST",0,TRUE,getmodcb,modlst->numlibs,&modlst->libs,fdsc);
	addlstrsrc(mtnrsrc,"FILE LIST",0,TRUE,getfilecb,file_defs->numlibs,&file_defs->libs,fdsc);
	addlstrsrc(mtnrsrc,"FIELD LIST",0,TRUE,getfieldcb,field_defs->numlibs,&field_defs->libs,fdsc);
	modlst=fdsc->modlst;
	file_defs=fdsc->file_defs;
	field_defs=fdsc->field_defs;
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
				GetRDATData(filenum,fdsc->previous);
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
				GetRDATData(filenum,fdsc->previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,FALSE);
	domodlist(mtnrsrc,fdsc);
	setmodname1(mtnrsrc,fdsc,FALSE);
	dofilelist(mtnrsrc,fdsc,FALSE);
	setfilename1(mtnrsrc,fdsc,FALSE);
	dofieldlist(mtnrsrc,fdsc,FALSE);
	setfieldname1(mtnrsrc,fdsc,FALSE);
	addDFincvir(mtnrsrc,module,"USERS",NULL,usernum);
	addDFincvir(mtnrsrc,module,"MODSEC",NULL,modnum);
	addDFincvir(mtnrsrc,module,"FILESEC",NULL,fnum);
	addDFincvir(mtnrsrc,module,"FLDSEC",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,fdsc);
		if(mbl_fldsec!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,fdsc);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,fdsc);
		}
	}
	if(mbl_fldsec==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_fldsec,fdsc);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);	
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,fdsc);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	keys=KEYPOINTER(MASTER->passkey->fileno);
	nokeys=NUMKEYS(MASTER->passkey->fileno);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,fdsc);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,fdsc,(mbl_fldsec==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	save_check(mtnrsrc,fdsc,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	save_check(mtnrsrc,fdsc,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,fldsecm *fdsc,short update_list)
{
	char deleteflag=0,*warnmessage=NULL,*userid=NULL;
	char *modname=NULL,*filename=NULL,*modulename=NULL,*fname=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[FLDSEC][DELETEFLAG]",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][USER IDENTIFICATION]",&userid);
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
		FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",&modname);
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
		FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FILE NAME]",&fname);
		FINDFLDSETSTRING(fnum,"USER IDENTIFICATION",userid);
		FINDFLDSETSTRING(fnum,"MODULE NAME",modname);
		FINDFLDSETSTRING(fnum,"FILE NAME",fname);
		if(EQLNRDsec(fnum,1))
		{
			filename=FILENAME(fnum);
			modulename=MODULENAME(fnum);
			warnmessage=Rmalloc(RDAstrlen(fname)+RDAstrlen(filename)+RDAstrlen(modulename)+150);
			sprintf(warnmessage,"The File Name of [%s] does not exist in the file [%s][%s].  Before this record can be saved it must have a valid File Name.",(fname==NULL?"":fname),modulename,filename);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID MODULE NAME!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDFLDGETCHAR(fnum,"DELETEFLAG",&deleteflag);
			if(deleteflag)
			{
				filename=FILENAME(fnum);
				modulename=MODULENAME(fnum);
				warnmessage=Rmalloc(RDAstrlen(fname)+RDAstrlen(filename)+RDAstrlen(modulename)+150);
				sprintf(warnmessage,"The File Name of [%s] is flagged to be deleted in the file [%s][%s].  Therefore, this record cannot be saved.",(fname==NULL?"":fname),modulename,filename);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED MODULE NAME!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
			}
		}
		if(userid!=NULL) Rfree(userid);
		if(modname!=NULL) Rfree(modname);
		if(userid!=NULL) Rfree(userid);
		if(modname!=NULL) Rfree(modname);
		if(fname!=NULL) Rfree(fname);
	}
	save_record(mtnrsrc,fdsc,update_list);
}
static void save_record(RDArsrc *mtnrsrc,fldsecm *fdsc,short update_list)
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
		WRTTRANS(filenum,0,NULL,fdsc->previous);
		updatebrowse(update_list,mbl_fldsec,filenum,module,mtnrsrc);
		if(mbl_fldsec!=NULL) quit_record(mtnrsrc,fdsc);
		else GetRDATData(filenum,fdsc->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	if(fdsc!=NULL)
	{
		if(fdsc->modlst!=NULL) freeapplib(fdsc->modlst);
		if(fdsc->file_defs!=NULL) freeapplib(fdsc->file_defs);
		if(fdsc->field_defs!=NULL) freeapplib(fdsc->field_defs);
		if(fdsc->previous!=NULL) FreeRDATData(fdsc->previous);
		Rfree(fdsc);
	}
	if(WindowCount==1)
	{
		CLSNRD(filenum);
		CLSNRD(usernum);
		CLSNRD(modnum);
		CLSNRD(fnum);
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
static void setmodname1(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	char *lmodname=NULL;
	int x=0;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",&lmodname);
	if(!isEMPTY(lmodname))
	{
		setmodnotempty(mtnrsrc,fdsc,lmodname,update);
	} else if(RDAstrcmp(fdsc->modlst->libs[x],"Directory Contains No Modules"))
	{
		FINDRSCSETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",fdsc->modlst->libs[x]);
	}
	if(lmodname!=NULL) Rfree(lmodname);
}
static void setmodname(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	char *lmodname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",&lmodname);
	if(!isEMPTY(lmodname))
	{
		setmodnotempty(mtnrsrc,fdsc,lmodname,update);
	} else {
		getmodcb(mtnrsrc,fdsc);
	}
	if(lmodname!=NULL) Rfree(lmodname);
}
static void setmodnotempty(RDArsrc *mtnrsrc,fldsecm *fdsc,char *lmodname,short update)
{
	int x;
	APPlib *modlst;

	modlst=fdsc->modlst;
	for(x=0;x<modlst->numlibs;++x)
	{
		if(!RDAstrcmp(lmodname,modlst->libs[x])) break;
	}
	if(x>=modlst->numlibs) 
	{
		addAPPlib(fdsc->modlst,lmodname);
		SORTAPPlib(fdsc->modlst);
		modlst=fdsc->modlst;
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(lmodname,modlst->libs[x])) break;
		}
		FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,fdsc->modlst);
	} else {
		FINDRSCSETINT(mtnrsrc,"MODULE LIST",x);
	}
	if(update) updatersrc(mtnrsrc,"MODULE LIST");
}
static void setfilename1(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)		
{
	char *lfilename=NULL;
	int x=0;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FILE NAME]",&lfilename);
	if(!isEMPTY(lfilename))
	{
		setfilenotempty(mtnrsrc,fdsc,lfilename,update);
	} else if(RDAstrcmp(fdsc->file_defs->libs[0],"No Files"))
	{
		FINDRSCSETSTRING(mtnrsrc,"[FLDSEC][FILE NAME]",fdsc->file_defs->libs[x]);
	}
	if(lfilename!=NULL) Rfree(lfilename);
}
static void setfilename(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)		
{
	char *lfilename=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FILE NAME]",&lfilename);
	if(!isEMPTY(lfilename))
	{
		setfilenotempty(mtnrsrc,fdsc,lfilename,update);
	} else {
		getfilecb(mtnrsrc,fdsc);
	}
	if(lfilename!=NULL) Rfree(lfilename);
}
static void setfilenotempty(RDArsrc *mtnrsrc,fldsecm *fdsc,char *lfilename,short update)		
{
	int x;
	APPlib *file_defs;

	file_defs=fdsc->file_defs;
	for(x=0;x<file_defs->numlibs;++x)
	{
		if(!RDAstrcmp(lfilename,file_defs->libs[x])) break;
	}
	if(x>=file_defs->numlibs) 
	{
		if(!RDAstrcmp(fdsc->file_defs->libs[0],"No Files"))
		{
			if(fdsc->file_defs!=NULL) freeapplib(fdsc->file_defs);
			fdsc->file_defs=APPlibNEW();
		}
		addAPPlib(fdsc->file_defs,lfilename);
		SORTAPPlib(fdsc->file_defs);
		file_defs=fdsc->file_defs;
		for(x=0;x<file_defs->numlibs;++x)
		{
			if(!RDAstrcmp(lfilename,file_defs->libs[x])) break;
		}
		FINDRSCLISTAPPlib(mtnrsrc,"FILE LIST",x,fdsc->file_defs);
	} else {
		FINDRSCSETINT(mtnrsrc,"FILE LIST",x);
	}
	if(update) updatersrc(mtnrsrc,"FILE LIST");
}
static void setfieldname1(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	char *lfieldname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FIELD NAME]",&lfieldname);
	if(!isEMPTY(lfieldname)) setfieldnotempty(mtnrsrc,fdsc,lfieldname,update);
	else getfield(mtnrsrc,fdsc,update);
	if(lfieldname!=NULL) Rfree(lfieldname);
}
static void setfieldname(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	char *lfieldname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FIELD NAME]",&lfieldname);
	if(!isEMPTY(lfieldname)) setfieldnotempty(mtnrsrc,fdsc,lfieldname,update);
	else getfieldcb(mtnrsrc,fdsc);
	if(lfieldname!=NULL) Rfree(lfieldname);
}
static void setfieldnotempty(RDArsrc *mtnrsrc,fldsecm *fdsc,char *lfieldname,short update)
{
	int x=0;
	APPlib *field_defs=NULL;

	field_defs=fdsc->field_defs; 
	for(x=0;x<field_defs->numlibs;++x)
	{
		if(!RDAstrcmp(lfieldname,field_defs->libs[x])) break;
	}
	if(x>=field_defs->numlibs) 
	{
		if(!RDAstrcmp(fdsc->field_defs->libs[0],"No Fields"))
		{
				if(fdsc->field_defs!=NULL) freeapplib(fdsc->field_defs);
			fdsc->field_defs=APPlibNEW();
		}
		addAPPlib(fdsc->field_defs,lfieldname);
		SORTAPPlib(fdsc->field_defs);
		field_defs=fdsc->field_defs;
		for(x=0;x<field_defs->numlibs;++x)
		{
			if(!RDAstrcmp(lfieldname,field_defs->libs[x])) break;
		}
		FINDRSCLISTAPPlib(mtnrsrc,"FIELD LIST",x,fdsc->field_defs);
	} else {
		FINDRSCSETINT(mtnrsrc,"FIELD LIST",x);
	}
	if(update) updatersrc(mtnrsrc,"FIELD LIST");
}
static void domodlist(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	int x=0;
	char *tmp=NULL,*directory=NULL;
	APPlib *modlst;

	directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(directory,"%s/rda/",CURRENTDIRECTORY);
	if(fdsc->modlst!=NULL) freeapplib(fdsc->modlst);
	fdsc->modlst=APPlibNEW();
	for(x=0;findfile(directory,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(fdsc->modlst,tmp);
	}
	modlst=fdsc->modlst;
	if(modlst->numlibs<1)
	{
		addAPPlib(fdsc->modlst,"Directory Contains No Modules");
	} else {
		SORTAPPlib(fdsc->modlst);
	}
	x=0;
	modlst=fdsc->modlst;
	FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,fdsc->modlst);
	if(tmp!=NULL) Rfree(tmp);
	if(directory!=NULL) Rfree(directory);
}
static void getmodcb(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	int s=0;
	APPlib *modlst;

	if(FINDRSCGETINT(mtnrsrc,"MODULE LIST",&s)) return;
	modlst=fdsc->modlst;
	if(modlst->numlibs>0)
	{
		if(FINDRSCSETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",modlst->libs[s])) return;
		updatersrc(mtnrsrc,"[FLDSEC][MODULE NAME]");
	}
	dofilelist(mtnrsrc,fdsc,TRUE);
	getfilecb(mtnrsrc,fdsc);
}
static void getfilecb(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	int s=0;
	APPlib *file_defs;

	readwidget(mtnrsrc,"FILE LIST");
	if(FINDRSCGETINT(mtnrsrc,"FILE LIST",&s)) return;
	file_defs=fdsc->file_defs;
	if(file_defs->numlibs>0)
	{
		if(s>=file_defs->numlibs)
		{
			s=0;
			if(FINDRSCSETINT(mtnrsrc,"FILE LIST",s)) return;
		}
		if(FINDRSCSETSTRING(mtnrsrc,"[FLDSEC][FILE NAME]",file_defs->libs[s])) return;
		updatersrc(mtnrsrc,"[FLDSEC][FILE NAME]");
	}
	dofieldlist(mtnrsrc,fdsc,TRUE);
	getfieldcb(mtnrsrc,fdsc);
}
static void getfieldcb(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	readallwidgets(mtnrsrc);	
	getfield(mtnrsrc,fdsc,TRUE);
	getrecord(mtnrsrc,fdsc);
	updateSCRNvirtuals(mtnrsrc);
}
static void getfield(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	int s=0;
	APPlib *field_defs;

	if(FINDRSCGETINT(mtnrsrc,"FIELD LIST",&s)) return;
	field_defs=fdsc->field_defs;
	if(field_defs->numlibs>0)
	{
		if(s>=field_defs->numlibs)
		{
			s=0;
			if(FINDRSCSETINT(mtnrsrc,"FIELD LIST",s)) return;
		}
		if(FINDRSCSETSTRING(mtnrsrc,"[FLDSEC][FIELD NAME]",field_defs->libs[s])) return;
	}
	if(update) updatersrc(mtnrsrc,"[FLDSEC][FIELD NAME]");
}
void dofilelist(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	char *libx=NULL,*lmodname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",&lmodname);
	if(fdsc->file_defs!=NULL) freeapplib(fdsc->file_defs);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(lmodname)+10);
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,lmodname);
	fdsc->file_defs=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(fdsc->file_defs==NULL)
	{
		fdsc->file_defs=APPlibNEW();
		addAPPlib(fdsc->file_defs,"No Files");
	}
	FINDRSCLISTAPPlib(mtnrsrc,"FILE LIST",0,fdsc->file_defs);
	if(update) updatersrc(mtnrsrc,"FILE LIST");
	if(lmodname!=NULL) Rfree(lmodname); 
}
static void dofieldlist(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	int x=0,y=0;
	short engine=0,nofields=0,nokeys=0;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part=NULL;
	char *lfilename=NULL,*lmodname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",&lmodname);
	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FILE NAME]",&lfilename);
	if(GETBIN(lmodname,lfilename,&engine,&nofields,&fields,&nokeys,&keys))
		nofields=0;
	if(lmodname!=NULL) Rfree(lmodname); 
	if(lfilename!=NULL) Rfree(lfilename);
	if(fdsc->field_defs!=NULL) freeapplib(fdsc->field_defs);
	fdsc->field_defs=APPlibNEW();
	if(nofields==0)
	{
			addAPPlib(fdsc->field_defs,"No Fields");
	} else {
		for(x=0;x<nofields;++x)
			addAPPlib(fdsc->field_defs,(fields+x)->name);
		SORTAPPlib(fdsc->field_defs);
	}
	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->name!=NULL) Rfree(keyx->name);
			if(keyx->part!=NULL)
			{
				for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
				{
					if(part->name!=NULL) Rfree(part->name);
				}
				Rfree(keyx->part);
			}
		}
		Rfree(keys);
		keys=NULL;
	}
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(fldx->name!=NULL) Rfree(fldx->name);
		}
		Rfree(fields);
		fields=NULL;
	}
	FINDRSCLISTAPPlib(mtnrsrc,"FIELD LIST",0,fdsc->field_defs);
	if(update) updatersrc(mtnrsrc,"FIELD LIST");
}
static void modinputcb(RDArsrc *mtnrsrc,fldsecm *fdsc)
{

	readallwidgets(mtnrsrc);
	modinput(mtnrsrc,fdsc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void modinput(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	setmodname(mtnrsrc,fdsc,update);
	dofilelist(mtnrsrc,fdsc,update);
	fileinput(mtnrsrc,fdsc,update);
}
static void fileinputcb(RDArsrc *mtnrsrc,fldsecm *fdsc)
{

	readallwidgets(mtnrsrc);
	fileinput(mtnrsrc,fdsc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void fileinput(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	setfilename(mtnrsrc,fdsc,update);
	dofieldlist(mtnrsrc,fdsc,update);
	fieldinput(mtnrsrc,fdsc,update);
}
static void fieldinputcb(RDArsrc *mtnrsrc,fldsecm *fdsc)
{

	readallwidgets(mtnrsrc);
	fieldinput(mtnrsrc,fdsc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void fieldinput(RDArsrc *mtnrsrc,fldsecm *fdsc,short update)
{
	setfieldname(mtnrsrc,fdsc,update);
	getrecord(mtnrsrc,fdsc);
}
static void getrecordcb(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	readallwidgets(mtnrsrc);
	getrecord(mtnrsrc,fdsc);
	setmodname(mtnrsrc,fdsc,TRUE);
	dofilelist(mtnrsrc,fdsc,TRUE);
	setfilename(mtnrsrc,fdsc,TRUE);
	dofieldlist(mtnrsrc,fdsc,TRUE);
	setfieldname(mtnrsrc,fdsc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void getrecord(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	char *luserid=NULL,*lmodname=NULL,*lfilename=NULL,*lfieldname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][USER IDENTIFICATION]",&luserid);
	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][MODULE NAME]",&lmodname);
	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FILE NAME]",&lfilename);
	FINDRSCGETSTRING(mtnrsrc,"[FLDSEC][FIELD NAME]",&lfieldname);
	ZERNRD(filenum);
	FINDFLDSETSTRING(filenum,"MODULE NAME",lmodname);
	FINDFLDSETSTRING(filenum,"USER IDENTIFICATION",luserid);
	FINDFLDSETSTRING(filenum,"FILE NAME",lfilename);
	FINDFLDSETSTRING(filenum,"FIELD NAME",lfieldname);
	if(luserid!=NULL) Rfree(luserid);
	if(lmodname!=NULL) Rfree(lmodname);
	if(lfilename!=NULL) Rfree(lfilename); 
	if(lfieldname!=NULL) Rfree(lfieldname); 
	if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(fdsc->previous);
		updatefilersrc(filenum,mtnrsrc);
	} else {
		GetRDATData(filenum,fdsc->previous);
		updatefilerecord2rsrc(filenum,mtnrsrc);	
	}
	getsupportingrecords(mtnrsrc,TRUE);
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
	if(modnum!=(-1))
	{
		COPYFIELD(filenum,modnum,"USER IDENTIFICATION");
		COPYFIELD(filenum,modnum,"MODULE NAME");
		if(EQLNRDsec(modnum,1)) KEYNRD(modnum,1);
		if(update) updatefilerecord2rsrc(modnum,mtnrsrc);
		else filerecord2rsrc(modnum,mtnrsrc);
	}	
	if(fnum!=(-1))
	{
		COPYFIELD(filenum,fnum,"USER IDENTIFICATION");
		COPYFIELD(filenum,fnum,"MODULE NAME");
		COPYFIELD(filenum,fnum,"FILE NAME");
		if(EQLNRDsec(fnum,1)) KEYNRD(fnum,1);
		if(update) updatefilerecord2rsrc(fnum,mtnrsrc);
		else filerecord2rsrc(fnum,mtnrsrc);
	}
}
