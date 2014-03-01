/* filesecm.c - File Security Maintain Screen */
#include <cstdio>


#include <gui.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <ldval.hpp>

struct filesecms
{
	APPlib *modlst;
	APPlib *file_defs;
	RDATData *previous;
};
typedef struct filesecms filesecm;

extern void browse_filesec(RDArsrc *,filesecm *);
extern MakeBrowseList *mbl_filesec;
extern char editable;
extern char *module;
extern short filenum,modnum,usernum;
static void save_record(RDArsrc *,filesecm *,short);
static void quit_record(RDArsrc *,filesecm *);
static void quit_record_test(RDArsrc *,filesecm *);
static void save_record_up(RDArsrc *,filesecm *);
static void save_record_nup(RDArsrc *,filesecm *);
static void save_check(RDArsrc *,filesecm *,short);
static void setmodname(RDArsrc *,filesecm *),setfilename(RDArsrc *,filesecm *);
static void setmodname1(RDArsrc *,filesecm *),setfilename1(RDArsrc *,filesecm *);
static void setmodnotempty(RDArsrc *,filesecm *,char *);
static void setfilenotempty(RDArsrc *,filesecm *,char *);
static void getrecord(RDArsrc *,filesecm *),getrecordcb(RDArsrc *,filesecm *);
static void modinputcb(RDArsrc *,filesecm *),modinput(RDArsrc *,filesecm *);
static void fileinputcb(RDArsrc *,filesecm *),fileinput(RDArsrc *,filesecm *);
static void domodlist(RDArsrc *,filesecm *);
static void dofilelist(RDArsrc *,filesecm *);
static void getmod(RDArsrc *,filesecm *),getmodcb(RDArsrc *,filesecm *);
static void getfilecb(RDArsrc *,filesecm *);
static void getfile(RDArsrc *,filesecm *);
static void getsupportingrecords(RDArsrc *,short);
void mtnfilesec(short,void *);
extern MaintainMaster *MTNMASTER;

static void getpreviouscb(RDArmem *member,filesecm *flsc)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MTNMASTER,flsc->previous,
		SCRNvirtualSubData,member->parent,keyno,NULL,NULL,NULL,NULL,
		NULL,flsc);
}
static void getnextcb(RDArmem *member,filesecm *flsc)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MTNMASTER,flsc->previous,
		SCRNvirtualSubData,member->parent,keyno,NULL,NULL,NULL,NULL,
		NULL,flsc);
}
static void quit_record_test(RDArsrc *mtnrsrc,filesecm *flsc)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,flsc->previous))
	{
		desc=QUITvfydesc(filenum,flsc->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_up,quit_record,
			FALSE,2,mtnrsrc,flsc,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,flsc);
	}
}
static APPlib *makefieldvallist(RDArsrc *r)
{
	APPlib *tmp=NULL;
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL,*selectedm=NULL,*selectedf=NULL;

	tmp=APPlibNEW();
	if(FINDRSCGETSTRING(r,"[FILESEC][MODULE NAME]",&selectedm)) return(tmp);
	if(FINDRSCGETSTRING(r,"[FILESEC][FILE NAME]",&selectedf)) 
	{
		if(selectedm!=NULL) Rfree(selectedm);
		return(tmp);
	}
	if((selectedm!=NULL && RDAstrlen(selectedm)>0) &&
		(selectedf!=NULL && RDAstrlen(selectedf)>0))
	{
		if(!GETBIN(selectedm,selectedf,&engine,&nofields,&fields,
			&nokeys,&keys))
		{
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part
							;z<keyx->numparts;++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
					}
					Rfree(keyx->part);
				}
			}
			Rfree(keys);
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(f->name!=NULL)
					{
						temp=Rmalloc(RDAstrlen(selectedm)+RDAstrlen(selectedf)+RDAstrlen(f->name)+7);
						sprintf(temp,"[%s][%s][%s]",selectedm,selectedf,f->name);
						addAPPlib(tmp,temp);
						if(temp!=NULL) Rfree(temp);
						Rfree(f->name);
					}
				}
				Rfree(fields);
			}
		}
	}
	if(selectedm!=NULL) Rfree(selectedm);
	if(selectedf!=NULL) Rfree(selectedf);
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist(load->val);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
void mtnfilesec(short dowhich,void *targetkey)
{
	filesecm *flsc=NULL;
	APPlib *modlst,*file_defs;
	RDArsrc *mtnrsrc=NULL;
	NRDkey *keys=NULL;
	short nokeys=0;

	flsc=Rmalloc(sizeof(filesecm));
	flsc->modlst=NULL;
	flsc->file_defs=NULL;
	ZERNRD(filenum);
	flsc->previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[FILESEC][USER IDENTIFICATION]",getrecordcb,flsc);
	FINDRSCSETFUNC(mtnrsrc,"[FILESEC][MODULE NAME]",modinputcb,flsc);
	FINDRSCSETFUNC(mtnrsrc,"[FILESEC][FILE NAME]",fileinputcb,flsc);
	if(modnum!=(-1)) file2rsrc(modnum,mtnrsrc,FALSE);
	if(usernum!=(-1)) file2rsrc(usernum,mtnrsrc,FALSE);
	if(flsc->modlst!=NULL) freeapplib(flsc->modlst);
	if(flsc->file_defs!=NULL) freeapplib(flsc->file_defs);
	flsc->modlst=APPlibNEW();
	flsc->file_defs=APPlibNEW();
	modlst=flsc->modlst;
	file_defs=flsc->file_defs;
	addlstrsrc(mtnrsrc,"MODULE LIST",0,TRUE,getmodcb,modlst->numlibs,&modlst->libs,flsc);
	addlstrsrc(mtnrsrc,"FILE LIST",0,TRUE,getfilecb,file_defs->numlibs,&file_defs->libs,flsc);
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
				GetRDATData(filenum,flsc->previous);
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
				GetRDATData(filenum,flsc->previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,FALSE);
	domodlist(mtnrsrc,flsc);
	setmodname1(mtnrsrc,flsc);
	dofilelist(mtnrsrc,flsc);
	setfilename1(mtnrsrc,flsc);
	addDFincvir(mtnrsrc,module,"USERS",NULL,usernum);
	addDFincvir(mtnrsrc,module,"MODSEC",NULL,modnum);
	addDFincvir(mtnrsrc,module,"FILESEC",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	addbtnrsrc(mtnrsrc,"LOAD VALUE",TRUE,loadvalue,
		"[FILESEC][EXPRESSION]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR",TRUE,loadoperand,
		"[FILESEC][EXPRESSION]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER",TRUE,loadgrouper,
		"[FILESEC][EXPRESSION]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL",TRUE,loadcontrol,
		"[FILESEC][EXPRESSION]");
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,flsc);
		if(mbl_filesec!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,flsc);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,flsc);
		}
	}
	if(mbl_filesec==NULL) addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_filesec,flsc);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,flsc);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	nokeys=NUMKEYS(MTNMASTER->passkey->fileno);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,flsc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,flsc,(mbl_filesec==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,filesecm *flsc)
{
	save_check(mtnrsrc,flsc,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,filesecm *flsc)
{
	save_check(mtnrsrc,flsc,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,filesecm *flsc,short update_list)
{
	char deleteflag=0,*warnmessage=NULL,*userid=NULL;
	char *modname=NULL,*filename=NULL,*modulename=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[FILESEC][DELETEFLAG]",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETSTRING(mtnrsrc,"[FILESEC][USER IDENTIFICATION]",&userid);
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
		FINDRSCGETSTRING(mtnrsrc,"[FILESEC][MODULE NAME]",&modname);
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
	save_record(mtnrsrc,flsc,update_list);
}
static void save_record(RDArsrc *mtnrsrc,filesecm *flsc,short update_list)
{
	char *rcddesc=NULL;
	char deleteflag=0,flddeleteflag=0;
	char *uid=NULL,*mod=NULL,*file=NULL;
	char *flduid=NULL,*fldmod=NULL,*fldfile=NULL;
	short fldsec=0;
	short ef_fld;

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
		WRTTRANS(filenum,0,NULL,flsc->previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			fldsec=OPNNRDsec(module,"FLDSEC",TRUE,TRUE,FALSE);
			FINDFLDGETSTRING(filenum,"USER IDENTIFICATION",&uid);
			FINDFLDGETSTRING(filenum,"MODULE NAME",&mod);
			FINDFLDGETSTRING(filenum,"FILE NAME",&file);
			if(fldsec!=(-1))
			{
				ZERNRD(fldsec);
 				FINDFLDSETSTRING(fldsec,"USER IDENTIFICATION",uid);
 				FINDFLDSETSTRING(fldsec,"MODULE NAME",mod);
 				FINDFLDSETSTRING(fldsec,"FILE NAME",file);
 				ef_fld=GTENRDsec(fldsec,1);
 				while(!ef_fld)
 				{
 					FINDFLDGETCHAR(fldsec,"DELETEFLAG",&flddeleteflag);
 					if(!flddeleteflag)
 					{
 						FINDFLDGETSTRING(fldsec,"USER IDENTIFICATION",&flduid);
 						FINDFLDGETSTRING(fldsec,"MODULE NAME",&fldmod);
 						FINDFLDGETSTRING(fldsec,"FILE NAME",&fldfile);
 						if(RDAstrcmp(flduid,uid) || RDAstrcmp(fldmod,mod) || 
							RDAstrcmp(fldfile,file)) break;
						FINDFLDSETCHAR(fldsec,"DELETEFLAG",deleteflag);
						WRTNRDtranssec(fldsec,0,NULL);
					} 
					ef_fld=NXTNRDsec(fldsec,1);
   				}
   			}
			if(fldsec!=(-1)) CLSNRD(fldsec);
		}
		if(uid!=NULL) Rfree(uid);
		if(mod!=NULL) Rfree(mod);
		if(file!=NULL) Rfree(file);
		if(flduid!=NULL) Rfree(flduid);
		if(fldmod!=NULL) Rfree(fldmod);
		if(fldfile!=NULL) Rfree(fldfile);
		updatebrowse(update_list,mbl_filesec,filenum,module,mtnrsrc);
		if(mbl_filesec!=NULL) quit_record(mtnrsrc,flsc);
		else GetRDATData(filenum,flsc->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,filesecm *flsc)
{
	if(flsc!=NULL)
	{
		if(flsc->modlst!=NULL) freeapplib(flsc->modlst);
		if(flsc->file_defs!=NULL) freeapplib(flsc->file_defs);
		if(flsc->previous!=NULL) FreeRDATData(flsc->previous);
		Rfree(flsc);
	}
	if(WindowCount==1)
	{
		CLSNRD(filenum);
		CLSNRD(usernum);
		CLSNRD(modnum);
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
static void setmodname1(RDArsrc *mtnrsrc,filesecm *flsc)
{
	char *lmodname=NULL;
	int x=0;
	
	FINDRSCGETSTRING(mtnrsrc,"[FILESEC][MODULE NAME]",&lmodname);
	if(!isEMPTY(lmodname))
	{
		setmodnotempty(mtnrsrc,flsc,lmodname);
	} else if(RDAstrcmp(flsc->modlst->libs[x],"Directory Contains No Modules"))
	{
		FINDRSCSETSTRING(mtnrsrc,"[FILESEC][MODULE NAME]",flsc->modlst->libs[x]);
	}
	if(lmodname!=NULL) Rfree(lmodname);
}
static void setmodname(RDArsrc *mtnrsrc,filesecm *flsc)
{
	char *lmodname=NULL;
	
	FINDRSCGETSTRING(mtnrsrc,"[FILESEC][MODULE NAME]",&lmodname);
	if(!isEMPTY(lmodname))
	{
		setmodnotempty(mtnrsrc,flsc,lmodname);
	} else {
		getmodcb(mtnrsrc,flsc);
	}
	if(lmodname!=NULL) Rfree(lmodname);
}
static void setmodnotempty(RDArsrc *mtnrsrc,filesecm *flsc,char *lmodname)
{
	int x=0;
	APPlib *modlst;

	modlst=flsc->modlst;
	for(x=0;x<modlst->numlibs;++x)
	{
		if(!RDAstrcmp(lmodname,modlst->libs[x])) break;
	}
	if(x>=modlst->numlibs) 
	{
		addAPPlib(flsc->modlst,lmodname);
		SORTAPPlib(flsc->modlst);
		modlst=flsc->modlst;
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(lmodname,modlst->libs[x])) break;
		}
		FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,flsc->modlst);
	} else {
		FINDRSCSETINT(mtnrsrc,"MODULE LIST",x);
	}
}
static void setfilename1(RDArsrc *mtnrsrc,filesecm *flsc)
{
	char *lfilename=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FILESEC][FILE NAME]",&lfilename);
	if(!isEMPTY(lfilename)) setfilenotempty(mtnrsrc,flsc,lfilename);
	else getfile(mtnrsrc,flsc);
	if(lfilename!=NULL) Rfree(lfilename);
}
static void setfilename(RDArsrc *mtnrsrc,filesecm *flsc)
{
	char *lfilename=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FILESEC][FILE NAME]",&lfilename);
	if(!isEMPTY(lfilename)) setfilenotempty(mtnrsrc,flsc,lfilename);
	else getfilecb(mtnrsrc,flsc);
	if(lfilename!=NULL) Rfree(lfilename);
}
static void setfilenotempty(RDArsrc *mtnrsrc,filesecm *flsc,char *lfilename)
{
	int x=0;
	APPlib *file_defs=NULL;

	file_defs=flsc->file_defs;
	for(x=0;x<file_defs->numlibs;++x)
	{
		if(!RDAstrcmp(lfilename,file_defs->libs[x])) break;
	}
	if(x>=file_defs->numlibs) 
	{
		if(!RDAstrcmp(flsc->file_defs->libs[0],"No Files"))
		{
			if(flsc->file_defs!=NULL) freeapplib(flsc->file_defs);
			flsc->file_defs=APPlibNEW();
		}
		addAPPlib(flsc->file_defs,lfilename);
		SORTAPPlib(flsc->file_defs);
		file_defs=flsc->file_defs;
		for(x=0;x<file_defs->numlibs;++x)
		{
			if(!RDAstrcmp(lfilename,file_defs->libs[x])) break;
		}
		FINDRSCLISTAPPlib(mtnrsrc,"FILE LIST",x,flsc->file_defs);
	} else {
		FINDRSCSETINT(mtnrsrc,"FILE LIST",x);
	}
}
static void getrecordcb(RDArsrc *mtnrsrc,filesecm *flsc)
{
	readallwidgets(mtnrsrc);
	getrecord(mtnrsrc,flsc);
	setmodname(mtnrsrc,flsc);
	dofilelist(mtnrsrc,flsc);
	setfilename(mtnrsrc,flsc);
	updatersrc(mtnrsrc,"FILE LIST");
	updatersrc(mtnrsrc,"MODULE LIST");
	updateSCRNvirtuals(mtnrsrc);
}
static void modinputcb(RDArsrc *mtnrsrc,filesecm *flsc)
{
	readallwidgets(mtnrsrc);
	modinput(mtnrsrc,flsc);
	updatersrc(mtnrsrc,"FILE LIST");
	updatersrc(mtnrsrc,"MODULE LIST");
	updateSCRNvirtuals(mtnrsrc);
}
static void modinput(RDArsrc *mtnrsrc,filesecm *flsc)
{
	setmodname(mtnrsrc,flsc);
	dofilelist(mtnrsrc,flsc);	
	fileinput(mtnrsrc,flsc);
}
static void fileinputcb(RDArsrc *mtnrsrc,filesecm *flsc)
{
	readallwidgets(mtnrsrc);
	fileinput(mtnrsrc,flsc);
	updatersrc(mtnrsrc,"FILE LIST");
	updatersrc(mtnrsrc,"MODULE LIST");
	updateSCRNvirtuals(mtnrsrc);
}
static void fileinput(RDArsrc *mtnrsrc,filesecm *flsc)
{
	setfilename(mtnrsrc,flsc);
	getrecord(mtnrsrc,flsc);
}
static void getrecord(RDArsrc *mtnrsrc,filesecm *flsc)
{
	char *luserid=NULL,*lmodname=NULL,*lfilename=NULL;

	if(FINDRSCGETSTRING(mtnrsrc,"[FILESEC][USER IDENTIFICATION]",&luserid)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[FILESEC][MODULE NAME]",&lmodname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[FILESEC][FILE NAME]",&lfilename)) return;
	FINDFLDSETSTRING(filenum,"MODULE NAME",lmodname);
	FINDFLDSETSTRING(filenum,"USER IDENTIFICATION",luserid);
	FINDFLDSETSTRING(filenum,"FILE NAME",lfilename);
	if(luserid!=NULL) Rfree(luserid);
	if(lmodname!=NULL) Rfree(lmodname);
	if(lfilename!=NULL) Rfree(lfilename);
	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		ClearRDATData(flsc->previous);
		updatefilersrc(filenum,mtnrsrc);
	} else {
		GetRDATData(filenum,flsc->previous);
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
}
static void domodlist(RDArsrc *mtnrsrc,filesecm *flsc)
{
	int x=0;
	char *tmp=NULL,*directory=NULL;
	APPlib *modlst=NULL;

	directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(directory,"%s/rda/",CURRENTDIRECTORY);
	if(flsc->modlst!=NULL) freeapplib(flsc->modlst);
	flsc->modlst=APPlibNEW();
	for(x=0;findfile(directory,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(flsc->modlst,tmp);
	}
	modlst=flsc->modlst;
	if(modlst->numlibs<1)
	{
		addAPPlib(flsc->modlst,"Directory Contains No Modules");
	} else {
		SORTAPPlib(flsc->modlst);
	}
	x=0;
	FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,flsc->modlst);
	if(directory!=NULL) Rfree(directory);
	if(tmp!=NULL) Rfree(tmp);
}
static void getmod(RDArsrc *mtnrsrc,filesecm *flsc)
{
	FINDRSCSETSTRING(mtnrsrc,"[FILESEC][FILE NAME]",NULL);
	dofilelist(mtnrsrc,flsc);	
	getfilecb(mtnrsrc,flsc);
}
static void getmodcb(RDArsrc *mtnrsrc,filesecm *flsc)
{
	int s=0;
	APPlib *modlst;

	readwidget(mtnrsrc,"MODULE LIST");
	if(FINDRSCGETINT(mtnrsrc,"MODULE LIST",&s)) return;
	modlst=flsc->modlst;
	if(FINDRSCSETSTRING(mtnrsrc,"[FILESEC][MODULE NAME]",modlst->libs[s])) return;
	getmod(mtnrsrc,flsc);
}
static void getfilecb(RDArsrc *mtnrsrc,filesecm *flsc)
{
	readwidget(mtnrsrc,"FILE LIST");
	getfile(mtnrsrc,flsc);
	getrecord(mtnrsrc,flsc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getfile(RDArsrc *mtnrsrc,filesecm *flsc)
{
	int s=0;
	APPlib *file_defs;

	if(FINDRSCGETINT(mtnrsrc,"FILE LIST",&s)) return;
	file_defs=flsc->file_defs;
	if(file_defs->numlibs>0)
	{
		if(s>=file_defs->numlibs)
		{
			s=0;
			if(FINDRSCSETINT(mtnrsrc,"FILE LIST",s)) return;
		}
		FINDRSCSETSTRING(mtnrsrc,"[FILESEC][FILE NAME]",file_defs->libs[s]);
	}
}
static void dofilelist(RDArsrc *mtnrsrc,filesecm *flsc)
{
	char *libx=NULL,*lmodname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[FILESEC][MODULE NAME]",&lmodname);
	if(flsc->file_defs!=NULL) { freeapplib(flsc->file_defs); flsc->file_defs=NULL; }
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(lmodname)+10);
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,lmodname);
	if(lmodname!=NULL) Rfree(lmodname);
	flsc->file_defs=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(flsc->file_defs==NULL)
	{
		flsc->file_defs=APPlibNEW();
		addAPPlib(flsc->file_defs,"No Files");
	}
	FINDRSCLISTAPPlib(mtnrsrc,"FILE LIST",0,flsc->file_defs);
}
