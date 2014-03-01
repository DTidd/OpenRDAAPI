/* modsecm.c - Module Security Maintain Screen */
#include <cstdio>


#include <mix.hpp>

struct modsecms
{
	APPlib *scr_libs;
	RDATData *previous;
};
typedef struct modsecms modsecm;

extern short filenum,usernum;
extern char editable;
extern char *module;
extern MakeBrowseList *mbl_modsec;
extern void browse_modsec(RDArsrc *,modsecm *);
static void save_record(RDArsrc *,modsecm *,short);
static void quit_record(RDArsrc *,modsecm *);
static void quit_record_test(RDArsrc *,modsecm *);
static void getmodcb(RDArsrc *,modsecm *),getmod(RDArsrc *,modsecm *);
static void setmodname(RDArsrc *,modsecm *);
static void save_record_up(RDArsrc *,modsecm *);
static void save_record_nup(RDArsrc *,modsecm *);
static void save_check(RDArsrc *,modsecm *,short);
static void domodlist(RDArsrc *,modsecm *);
static void getrecord(RDArsrc *,modsecm *),getrecordcb(RDArsrc *,modsecm *);
static void getsupportingrecords(RDArsrc *,short);
void mtnmodsec(short,void *);
extern MaintainMaster *MASTER;

static void quit_record_test(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,mdsc->previous))
	{
		desc=QUITvfydesc(filenum,mdsc->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_up,quit_record,
			FALSE,2,mtnrsrc,mdsc,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mdsc);
	}
}
void mtnmodsec(short dowhich,void *targetkey)
{
	modsecm *mdsc=NULL;
	APPlib *scr_libs;
	RDArsrc *mtnrsrc=NULL;

	mdsc=Rmalloc(sizeof(modsecm));
	mdsc->scr_libs=NULL;
	ZERNRD(filenum);
	mdsc->previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[MODSEC][USER IDENTIFICATION]",getrecordcb,mdsc);
	FINDRSCSETFUNC(mtnrsrc,"[MODSEC][MODULE NAME]",getrecordcb,mdsc);
	if(usernum!=(-1)) file2rsrc(usernum,mtnrsrc,FALSE);
	if(mdsc->scr_libs!=NULL) freeapplib(mdsc->scr_libs);
	mdsc->scr_libs=APPlibNEW();
	scr_libs=mdsc->scr_libs;
	addlstrsrc(mtnrsrc,"MODULE LIST",0,TRUE,getmodcb,scr_libs->numlibs,&scr_libs->libs,mdsc);
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
				GetRDATData(filenum,mdsc->previous);
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
				GetRDATData(filenum,mdsc->previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,FALSE);
	domodlist(mtnrsrc,mdsc);
	setmodname(mtnrsrc,mdsc);
	addDFincvir(mtnrsrc,module,"USERS",NULL,usernum);
	addDFincvir(mtnrsrc,module,"MODSEC",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mdsc);
		if(mbl_modsec!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mdsc);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mdsc);
		}
	}
	if(mbl_modsec==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_modsec,mdsc);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mdsc);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mdsc,(mbl_modsec==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	save_check(mtnrsrc,mdsc,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	save_check(mtnrsrc,mdsc,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,modsecm *mdsc,short update_list)
{
	char deleteflag=0,*warnmessage=NULL,*userid=NULL,*filename=NULL,*modulename=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[MODSEC][DELETEFLAG]",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETSTRING(mtnrsrc,"[MODSEC][USER IDENTIFICATION]",&userid);
		FINDFLDSETSTRING(usernum,"USER IDENTIFICATION",userid);
		if(EQLNRDsec(usernum,1))
		{
			filename=FILENAME(usernum);
			modulename=MODULENAME(usernum);
			warnmessage=Rmalloc(RDAstrlen(userid)+RDAstrlen(filename)+RDAstrlen(modulename)+150);
			sprintf(warnmessage,"The User Identification of [%s] does not exist in the file [%s][%s].  Before this record can be saved it must have a valid User Identifcation.",(userid==NULL?"":userid),modulename,filename);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID USER IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(userid!=NULL) Rfree(userid);
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
				if(userid!=NULL) Rfree(userid);
			}
		}
	}
	save_record(mtnrsrc,mdsc,update_list);
}
static void save_record(RDArsrc *mtnrsrc,modsecm *mdsc,short update_list)
{
  char *rcddesc=NULL;
  char deleteflag=0,pdeleteflag=0,fildeleteflag=0,flddeleteflag=0;
  char *uid=NULL,*mod=NULL,*file=NULL;
  char *puid=NULL,*pmod=NULL;
  char *flduid=NULL,*fldmod=NULL,*fldfile=NULL;
  char *filuid=NULL,*filmod=NULL;
  short procsec=0,filesec=0,fldsec=0;
  short ef_proc,ef_file,ef_fld;

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
           WRTTRANS(filenum,0,NULL,mdsc->previous);
           FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
           if(deleteflag)
           {
             procsec=OPNNRDsec(module,"PROCSEC",TRUE,TRUE,FALSE);
             filesec=OPNNRDsec(module,"FILESEC",TRUE,TRUE,FALSE);
             fldsec=OPNNRDsec(module,"FLDSEC",TRUE,TRUE,FALSE); 	
             FINDFLDGETSTRING(filenum,"USER IDENTIFICATION",&uid);
             FINDFLDGETSTRING(filenum,"MODULE NAME",&mod);
             if(procsec!=(-1))
             {
               ZERNRD(procsec);
               FINDFLDSETSTRING(procsec,"USER IDENTIFICATION",uid);
               FINDFLDSETSTRING(procsec,"MODULE NAME",mod);
               ef_proc=GTENRDsec(procsec,1);
               while(!ef_proc)
               {
                 FINDFLDGETCHAR(procsec,"DELETEFLAG",&pdeleteflag);
                 if(!pdeleteflag)
                 {
                   FINDFLDGETSTRING(procsec,"USER IDENTIFICATION",&puid);
                   FINDFLDGETSTRING(procsec,"MODULE NAME",&pmod);
                   if(RDAstrcmp(puid,uid) || RDAstrcmp(pmod,mod)) break;
                   FINDFLDSETCHAR(procsec,"DELETEFLAG",deleteflag);
                   WRTNRDtranssec(procsec,0,NULL);		
                  }
                  ef_proc=NXTNRDsec(procsec,1);
               }
             }
             if(filesec!=(-1))	 		
             {
               ZERNRD(filesec);
               FINDFLDSETSTRING(filesec,"USER IDENTIFICATION",uid);
               FINDFLDSETSTRING(filesec,"MODULE NAME",mod);
               ef_file=GTENRDsec(filesec,1);
               while(!ef_file)
               {
                 FINDFLDGETCHAR(filesec,"DELETEFLAG",&fildeleteflag);
                 if(!fildeleteflag)
                 {
                   FINDFLDGETSTRING(filesec,"USER IDENTIFICATION",&filuid);
                   FINDFLDGETSTRING(filesec,"MODULE NAME",&filmod);
                   if(RDAstrcmp(filuid,uid) || RDAstrcmp(filmod,mod)) break;
                   if(fldsec!=(-1))
                   {
                     ZERNRD(fldsec);
                     FINDFLDGETSTRING(filesec,"FILE NAME",&file);
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
                         if(RDAstrcmp(flduid,uid) || RDAstrcmp(fldmod,mod) || RDAstrcmp(fldfile,file)) break;
                         FINDFLDSETCHAR(fldsec,"DELETEFLAG",deleteflag);
                         WRTNRDtranssec(fldsec,0,NULL);		
                       } 
                       ef_fld=NXTNRDsec(fldsec,1);
                     }
                   }
                   FINDFLDSETCHAR(filesec,"DELETEFLAG",deleteflag);
                   WRTNRDtranssec(filesec,0,NULL);		
                 } 
                 ef_file=NXTNRDsec(filesec,1);
               }   
             }
             if(procsec!=(-1)) CLSNRD(procsec);
             if(fldsec!=(-1)) CLSNRD(fldsec);
             if(filesec!=(-1)) CLSNRD(filesec);
           }
           if(uid!=NULL) Rfree(uid);
           if(mod!=NULL) Rfree(mod);
           if(file!=NULL) Rfree(file);
           if(puid!=NULL) Rfree(puid);
           if(pmod!=NULL) Rfree(pmod);
           if(flduid!=NULL) Rfree(flduid);
           if(fldmod!=NULL) Rfree(fldmod);
           if(fldfile!=NULL) Rfree(fldfile);
           if(filuid!=NULL) Rfree(filuid);
           if(filmod!=NULL) Rfree(filmod);
           updatebrowse(update_list,mbl_modsec,filenum,module,mtnrsrc);
           if(mbl_modsec!=NULL) quit_record(mtnrsrc,mdsc);
           else GetRDATData(filenum,mdsc->previous);
         }
}
static void quit_record(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	if(mdsc!=NULL)
	{
		if(mdsc->scr_libs!=NULL) freeapplib(mdsc->scr_libs);
		if(mdsc->previous!=NULL) FreeRDATData(mdsc->previous);
		Rfree(mdsc);
	}
	if(WindowCount==1)
	{
		CLSNRD(filenum);
		CLSNRD(usernum);
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
static void getrecordcb(RDArsrc *mtnrsrc,modsecm *mdsc)
{

	readallwidgets(mtnrsrc);
	getrecord(mtnrsrc,mdsc);
	setmodname(mtnrsrc,mdsc);
	updatersrc(mtnrsrc,"MODULE LIST");
	updateSCRNvirtuals(mtnrsrc);
}
static void getrecord(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	char *luserid=NULL,*lmodname=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[MODSEC][USER IDENTIFICATION]",&luserid);
	FINDRSCGETSTRING(mtnrsrc,"[MODSEC][MODULE NAME]",&lmodname);
	FINDFLDSETSTRING(filenum,"USER IDENTIFICATION",luserid);
	FINDFLDSETSTRING(filenum,"MODULE NAME",lmodname);
	if(luserid!=NULL) Rfree(luserid);
	if(lmodname!=NULL) Rfree(lmodname);
	if(EQLNRDsec(filenum,MASTER->passkey->keyno))
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(mdsc->previous);
		updatefilersrc(filenum,mtnrsrc);
	} else {
		GetRDATData(filenum,mdsc->previous);
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
}
static void setmodname(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	char *lmodname=NULL; 
	APPlib *scr_libs;
	int x=0;

	FINDRSCGETSTRING(mtnrsrc,"[MODSEC][MODULE NAME]",&lmodname);
	if(!isEMPTY(lmodname))
	{
		scr_libs=mdsc->scr_libs;
		for(x=0;x<scr_libs->numlibs;++x)
		{
			if(!RDAstrcmp(lmodname,scr_libs->libs[x])) break;
		}
		if(x>=scr_libs->numlibs) 
		{
			addAPPlib(mdsc->scr_libs,lmodname);
			SORTAPPlib(mdsc->scr_libs);
			scr_libs=mdsc->scr_libs;
			for(x=0;x<scr_libs->numlibs;++x)
			{
				if(!RDAstrcmp(lmodname,scr_libs->libs[x])) break;
			}
			FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,scr_libs);
		} else {
			FINDRSCSETINT(mtnrsrc,"MODULE LIST",x);
		}
	} else {
		getmodcb(mtnrsrc,mdsc);
	}
	if(lmodname!=NULL) Rfree(lmodname);
}
static void domodlist(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	int x=0;
	char *tmp=NULL,*directory=NULL;
	APPlib *scr_libs;

	directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(directory,"%s/rda/",CURRENTDIRECTORY);
	if(mdsc->scr_libs!=NULL) freeapplib(mdsc->scr_libs);
	mdsc->scr_libs=APPlibNEW();
	for(x=0;findfile(directory,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(mdsc->scr_libs,tmp);
	}
	scr_libs=mdsc->scr_libs;
	if(scr_libs->numlibs<1)
	{
		addAPPlib(mdsc->scr_libs,"Directory Contains No Modules");
	}
	SORTAPPlib(mdsc->scr_libs);
	if(tmp!=NULL) Rfree(tmp);
	if(directory!=NULL) Rfree(directory);
	scr_libs=mdsc->scr_libs;
	FINDRSCGETINT(mtnrsrc,"MODULE LIST",&x);
	if(x>=scr_libs->numlibs) x=0;
	FINDRSCLISTAPPlib(mtnrsrc,"MODULE LIST",x,scr_libs);
}
static void getmodcb(RDArsrc *mtnrsrc, modsecm *mdsc)
{
	int s=0;
	APPlib *scr_libs;

	readwidget(mtnrsrc,"MODULE LIST");
	if(FINDRSCGETINT(mtnrsrc,"MODULE LIST",&s)) return;
	scr_libs=mdsc->scr_libs;
	if(FINDRSCSETSTRING(mtnrsrc,"[MODSEC][MODULE NAME]",scr_libs->libs[s])) return;
	getmod(mtnrsrc,mdsc);
}
static void getmod(RDArsrc *mtnrsrc,modsecm *mdsc)
{
	getrecord(mtnrsrc,mdsc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
