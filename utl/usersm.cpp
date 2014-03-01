/* usersm.c - XPERT Users Maintain Screen */
#include <cstdio>


#include <gui.hpp>
#include <mix.hpp>

extern void browse_users(RDArsrc *,RDATData *);
extern short filenum;
extern char editable;
extern char *module;
extern MakeBrowseList *mbl_users;
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *),save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void getrecordcb(RDArsrc *,RDATData *);
void mtnusers(short,void *);
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
void mtnusers(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[USERS][USER IDENTIFICATION]",getrecordcb,previous);
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
	addDFincvir(mtnrsrc,module,"USERS",NULL,filenum); 
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_users!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
		}
	}
	if(mbl_users==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_users,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_users==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL;
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0;
	char fildeleteflag=0,flddeleteflag=0;
	char *uid=NULL,*mod=NULL,*file=NULL;
	char *puid=NULL,*pmod=NULL;
	char *flduid=NULL,*fldmod=NULL,*fldfile=NULL;
	char *filuid=NULL,*filmod=NULL;
	char *muid=NULL;	
	short modsec=0,procsec=0,filesec=0,fldsec=0;
	short ef_mod,ef_proc,ef_file,ef_fld;

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
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			modsec=OPNNRDsec(module,"MODSEC",TRUE,TRUE,FALSE);	
			procsec=OPNNRDsec(module,"PROCSEC",TRUE,TRUE,FALSE);
			filesec=OPNNRDsec(module,"FILESEC",TRUE,TRUE,FALSE);
			fldsec=OPNNRDsec(module,"FLDSEC",TRUE,TRUE,FALSE); 	
			if(modsec!=(-1))
			{
				ZERNRD(modsec);
				FINDFLDGETSTRING(filenum,"USER IDENTIFICATION",&uid);
				FINDFLDSETSTRING(modsec,"USER IDENTIFICATION",uid);
				ef_mod=GTENRDsec(modsec,1);
				while(!ef_mod)
				{
					FINDFLDGETCHAR(modsec,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(modsec,"USER IDENTIFICATION",&muid);
						if(RDAstrcmp(muid,uid)) break;
						FINDFLDGETSTRING(modsec,"MODULE NAME",&mod);
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
						FINDFLDSETCHAR(modsec,"DELETEFLAG",deleteflag);
						WRTNRDtranssec(modsec,0,NULL);
					}
					ef_mod=NXTNRDsec(modsec,1); 	
				}
				CLSNRD(modsec);		
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
		if(muid!=NULL) Rfree(muid);
		updatebrowse(update_list,mbl_users,filenum,module,mtnrsrc);
		if(mbl_users!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(filenum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		CLSNRD(filenum);
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
static void getrecordcb(RDArsrc *mtnrsrc,RDATData *previous)
{

	char *userid=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[USERS][USER IDENTIFICATION]",&userid)) return;
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
	if(userid!=NULL) Rfree(userid);
	updateSCRNvirtuals(mtnrsrc);
}
