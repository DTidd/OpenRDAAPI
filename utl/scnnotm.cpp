/* scnnotm.c - Screen Notes Maintain Screen */
#include <cstdio>


#include <mix.hpp>

extern char editable;
extern char *module;
extern short filenum;
extern MakeBrowseList *mbl_scnnot;
extern void browse_scn(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *),save_record_nup(RDArsrc *,RDATData *);
static void getrecord(RDArsrc *,RDATData *);
void mtnscnnot(short,void *);
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
void mtnscnnot(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[SCNNOT][MODULE NAME]",getrecord,previous);	 
	FINDRSCSETFUNC(mtnrsrc,"[SCNNOT][SCREEN NAME]",getrecord,previous);	 
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
	addDFincvir(mtnrsrc,module,"MODSEC",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_scnnot!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
		}
	}
	if(mbl_scnnot==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_scn,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_scnnot==NULL?TRUE:FALSE));
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
	char *rcddesc=NULL,*mod=NULL,*scn=NULL,*rsc=NULL;
	char *usmod=NULL,*rmod=NULL,*urmod=NULL;
	char *usscn=NULL,*rscn=NULL,*urscn=NULL,*urrsc=NULL;
	char deleteflag=0,usdeleteflag=0,rdeleteflag=0,urdeleteflag=0;
	short uscnnot=0,rscnot=0,urscnot=0;
	short ef_us,ef_r,ef_ur;

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
			uscnnot=OPNNRDsec(module,"USCNNOT",TRUE,TRUE,FALSE);
			rscnot=OPNNRDsec(module,"RSCNOT",TRUE,TRUE,FALSE);
			urscnot=OPNNRDsec(module,"URSCNOT",TRUE,TRUE,FALSE);
			FINDFLDGETSTRING(filenum,"MODULE NAME",&mod);
			FINDFLDGETSTRING(filenum,"SCREEN NAME",&scn);
			if(uscnnot!=(-1))
			{
				ZERNRD(uscnnot);
				FINDFLDSETSTRING(uscnnot,"MODULE NAME",mod);
				FINDFLDSETSTRING(uscnnot,"SCREEN NAME",scn);
				ef_us=GTENRDsec(uscnnot,1);
				while(!ef_us)
				{
					FINDFLDGETCHAR(uscnnot,"DELETEFLAG",&usdeleteflag);
					if(!usdeleteflag)
					{
						FINDFLDGETSTRING(uscnnot,"MODULE NAME",&usmod);		
						FINDFLDGETSTRING(uscnnot,"SCREEN NAME",&usscn);		
						if(RDAstrcmp(mod,usmod) || RDAstrcmp(scn,usscn)) break;
						FINDFLDSETCHAR(uscnnot,"DELETEFLAG",deleteflag);
						WRTNRDtranssec(uscnnot,0,NULL);
					
					}						
					ef_us=NXTNRDsec(uscnnot,1);
				}
				
			}
			if(rscnot!=(-1))
			{
				ZERNRD(rscnot);
				FINDFLDSETSTRING(rscnot,"MODULE NAME",mod);
				FINDFLDSETSTRING(rscnot,"SCREEN NAME",scn);
				ef_r=GTENRDsec(rscnot,1);
				while(!ef_r)
				{
					FINDFLDGETCHAR(rscnot,"DELETEFLAG",&rdeleteflag);
					if(!rdeleteflag)
					{
						FINDFLDGETSTRING(rscnot,"MODULE NAME",&rmod);		
						FINDFLDGETSTRING(rscnot,"SCREEN NAME",&rscn);		
						if(RDAstrcmp(mod,rmod) || RDAstrcmp(scn,rscn)) break;
						if(urscnot!=(-1))
						{
							ZERNRD(urscnot);
							FINDFLDGETSTRING(rscnot,"RESOURCE NAME",&rsc);
							FINDFLDSETSTRING(urscnot,"RESOURCE NAME",rsc);
							FINDFLDSETSTRING(urscnot,"MODULE NAME",mod);
							FINDFLDSETSTRING(urscnot,"SCREEN NAME",scn);
							ef_ur=GTENRDsec(urscnot,1);
							while(!ef_ur)
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
								ef_ur=NXTNRDsec(urscnot,1);
							}
						}
						FINDFLDSETCHAR(rscnot,"DELETEFLAG",deleteflag);
						WRTNRDtranssec(rscnot,0,NULL);
					}
					ef_r=NXTNRDsec(rscnot,1);
				}
			}
			if(urscnot!=(-1)) CLSNRD(urscnot);
			if(rscnot!=(-1)) CLSNRD(rscnot);
			if(uscnnot!=(-1)) CLSNRD(uscnnot);
		}
		if(mod!=NULL) Rfree(mod);
		if(scn!=NULL) Rfree(scn);
		if(rsc!=NULL) Rfree(rsc);
		if(usmod!=NULL) Rfree(usmod);
		if(rmod!=NULL) Rfree(rmod);
		if(urmod!=NULL) Rfree(urmod);
		if(usscn!=NULL) Rfree(usscn);
		if(rscn!=NULL) Rfree(rscn);
		if(urscn!=NULL) Rfree(urscn);
		if(urrsc!=NULL) Rfree(urrsc);
		updatebrowse(update_list,mbl_scnnot,filenum,module,mtnrsrc);
		if(mbl_scnnot!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(filenum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		CLSNRD(filenum);
		if(MASTER!=NULL) killwindow(mtnrsrc);
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
	char *modname=NULL,*scnname=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[SCNNOT][MODULE NAME]",&modname)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"[SCNNOT][SCREEN NAME]",&scnname)) return;
	FINDFLDSETSTRING(filenum,"MODULE NAME",modname);
	FINDFLDSETSTRING(filenum,"SCREEN NAME",scnname);
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
	if(scnname!=NULL) Rfree(scnname);
	updateSCRNvirtuals(mtnrsrc);
}
