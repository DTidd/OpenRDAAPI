/* appspa.c - Schedule Personnel Appointments */
/*lint -library */
#ifndef WIN32
#define __NAM__ "appspa.lnx"
#endif
#ifdef WIN32
#define __NAM__ "appspa.exe"
#endif
#include <app.hpp>



#include <cstdlib>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <ldval.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

short appcnt=(-1),appcat=(-1),apppos=(-1),posmst=(-1);
char *module="APPMGT",*reportname=NULL;
int DID_STUFF=0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static void save_check(RDArsrc *,short,DBsort *);
static void save_record(RDArsrc *,short,DBsort *);
static void quit_record(RDArsrc *,DBsort *);
static void doexit_test(MakeBrowseList *);
static void doexit(MakeBrowseList *);
static void okfunc(MakeBrowseList *,void *);
static void saveexit(RDArsrc *,MakeBrowseList *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static void getfile(RDArsrc *,DBsort *);
static void getpreviouscb(RDArmem *,DBsort *);
static void getnextcb(RDArmem *,DBsort *);
static void setkeys(RDArsrc *,DBsort *);
static void resetdefs(RDArsrc *,DBsort *);
static void setfiles(RDArsrc *,DBsort *);
static void addsortrecord(DBsort *);
static void makeDBsortfile(DBsort *);
static void schedule_backend(DBsort *);
static void schedulem(void *,DBsort *);

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,apppos);
	AddFileFields(list,appcat);
	AddFileFields(list,appcnt);
	AddFileFields(list,posmst);
	addAPPlibNoDuplicates(list,"[SYSTEM DATE]");
	addAPPlibNoDuplicates(list,"[SYSTEM TIME]");
	addAPPlibNoDuplicates(list,"[LOGIN IDENTIFICATION]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION NAME]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ADDRESS #1]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ADDRESS #2]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION CITY]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION STATE]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ZIP]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION PHONE]");
	addAPPlibNoDuplicates(list,"[STARTING FISCAL MONTH]");
	addAPPlibNoDuplicates(list,"[USE PERSONNEL INTERVIEW]");
	addAPPlibNoDuplicates(list,"[USE ADMINISTRATIVE INTERVIEW]");
	SORTAPPlib(list);
	return(list);
}
void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist();
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
static void save_record_up(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_check(mtnrsrc,TRUE,dbsort);
}
static void save_record_nup(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_check(mtnrsrc,FALSE,dbsort);
}
static void save_check(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	char *appid=NULL,*warnmessage=NULL,delflag=FALSE,*posid=NULL;
	int appno=0;

	ZERNRD(dbsort->fileno);
	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(appcnt!=(-1))
		{
			FINDFLDGETSTRING(dbsort->fileno,"APPLICANT IDENTIFICATION",&appid);
			FINDFLDSETSTRING(appcnt,"APPLICANT IDENTIFICATION",appid);
			if(ADVEQLNRDsec(appcnt,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(appcnt,1);
				warnmessage=Rmalloc(RDAstrlen(appid)+137);
				sprintf(warnmessage,"The APPLICANT IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Applicant Master.",(appid==NULL?"":appid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID APPLICANT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(appcnt,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(appid)+140);
					sprintf(warnmessage,"The APPLICANT IDENTIFICATION of [%s] entered is deleted.  It exists as an applicant, but is presently set up in a deleted state.",(appid==NULL ? "":appid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED APPLICANT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(appcat!=(-1))
		{
			FINDFLDGETSTRING(dbsort->fileno,"APPLICANT IDENTIFICATION",&appid);
			FINDFLDGETINT(dbsort->fileno,"APPLICATION NUMBER",&appno);
			FINDFLDSETSTRING(appcat,"APPLICANT IDENTIFICATION",appid);
			FINDFLDSETINT(appcat,"APPLICATION NUMBER",appno);
			if(ADVEQLNRDsec(appcat,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(appcat,1);
				warnmessage=Rmalloc(RDAstrlen(appid)+137);
				sprintf(warnmessage,"The APPLICATION NUMBER of [%d] entered is not valid.  It does not exist as an Application Master.",appno);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID APPLICATION NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(appcat,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(appid)+140);
					sprintf(warnmessage,"The APPLICATION NUMBER of [%d] entered is deleted.  It exists as an application, but is presently set up in a deleted state.",appno);
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED APPLICATION NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(posmst!=(-1))
		{
			FINDFLDGETSTRING(dbsort->fileno,"POSITION IDENTIFICATION",&posid);
			FINDFLDSETSTRING(posmst,"POSITION IDENTIFICATION",posid);
			if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(posmst,1);
				warnmessage=Rmalloc(RDAstrlen(posid)+137);
				sprintf(warnmessage,"The POSITION IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Position Master.",(posid==NULL?"":posid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID POSITION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(posid!=NULL) Rfree(posid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(posmst,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(posid)+140);
					sprintf(warnmessage,"The POSITION IDENTIFICATION of [%s] entered is deleted.  It exists as an position, but is presently set up in a deleted state.",(posid==NULL ? "":posid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED POSITION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(posid!=NULL) Rfree(posid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(apppos!=(-1))
		{
			FINDFLDSETSTRING(apppos,"APPLICANT IDENTIFICATION",appid);
			FINDFLDSETINT(apppos,"APPLICATION NUMBER",appno);
			FINDFLDSETSTRING(apppos,"POSITION IDENTIFICATION",posid);
			if(ADVEQLNRDsec(apppos,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(apppos,1);
				warnmessage=Rmalloc(RDAstrlen(appid)+137);
				sprintf(warnmessage,"The APPLICANT POSITION of [%s] entered is not valid.  It does not exist for this Applicant.",(posid==NULL?"":posid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID APPLICANT POSITION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(posid!=NULL) Rfree(posid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(apppos,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(posid)+200);
					sprintf(warnmessage,"The APPLICANT POSITION of [%s] entered is deleted.  It exists as an applicant position, but is presently set up in a deleted state.",(posid==NULL ? "":posid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED APPLICANT POSITION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(posid!=NULL) Rfree(posid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
	}
	if(posid!=NULL) Rfree(posid);
	if(appid!=NULL) Rfree(appid);
	save_record(mtnrsrc,update_list,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,"Browse Diagnostic",NULL);
			if(tmprsrc!=NULL)
			{
				if(!ADVmakescrn(tmprsrc,FALSE))
				{
					ForceWindowUpdate(tmprsrc);
				} else {
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				}
			}
			xmakebrowselist(mbl_trans,tmprsrc);
			if(mbl_trans->list->num<=valuex) valuex=0;
			mbl_trans->list->keyvalue=mbl_trans->list->key[valuex];
			if(!FINDRSCSETLIST(mbl_trans->mainrsrc,"BROWSE LIST",valuex,
				mbl_trans->list->num,&mbl_trans->list->string))
			{
				updatersrc(mbl_trans->mainrsrc,"BROWSE LIST");
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			ForceWindowUpdate(mtnrsrc);
		}
	}
	if(mtnrsrc!=NULL) killwindow(mtnrsrc);
	quit_record(mtnrsrc,dbsort);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	COPYFIELD(dbsort->fileno,appcnt,"APPLICANT IDENTIFICATION");
	if(ADVEQLNRDsec(appcnt,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(appcnt,1);
	filerecord2rsrc(appcnt,mtnrsrc);
	COPYFIELD(dbsort->fileno,appcat,"APPLICANT IDENTIFICATION");
	COPYFIELD(dbsort->fileno,appcat,"APPLICATION NUMBER");
	if(ADVEQLNRDsec(appcat,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(appcat,1);
	filerecord2rsrc(appcat,mtnrsrc);
	COPYFIELD(dbsort->fileno,apppos,"APPLICANT IDENTIFICATION");
	COPYFIELD(dbsort->fileno,apppos,"APPLICATION NUMBER");
	COPYFIELD(dbsort->fileno,apppos,"POSITION IDENTIFICATION");
	if(ADVEQLNRDsec(apppos,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(apppos,1);
	filerecord2rsrc(apppos,mtnrsrc);
	COPYFIELD(dbsort->fileno,posmst,"POSITION IDENTIFICATION");
	if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(posmst,1);
	filerecord2rsrc(posmst,mtnrsrc);
}
static void quit_record(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(WindowCount<2) schedule_backend(dbsort);
}
static void schedule_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE;
	char deleteflag=FALSE,*curdate=NULL,*curtime=NULL;
	char *apptdate=NULL,*appttime=NULL,withdraw=FALSE,*e=NULL;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Scheduling Personnel Appointments",NULL);
		addDFincvir(tmprsrc,module,"APPCNT",NULL,appcnt);
		addDFincvir(tmprsrc,module,"APPCAT",NULL,appcat);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		if(tmprsrc!=NULL)
		{
			if(!ADVmakescrn(tmprsrc,TRUE))
			{
				ForceWindowUpdate(tmprsrc);
			} else {
				e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
				sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
				prterr("Error %s.",e);
				if(e!=NULL) Rfree(e);
			}
		}
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETSTRING(dbsort->fileno,"APPOINTMENT DATE",&apptdate);
			FINDFLDGETSTRING(dbsort->fileno,"APPOINTMENT TIME",&appttime);
			FINDFLDGETCHAR(dbsort->fileno,"WITHDRAW APPLICANT",&withdraw);
			if(!deleteflag && ((!isEMPTY(apptdate) && !isEMPTY(appttime))
				|| withdraw))
			{
				COPYFIELD(dbsort->fileno,appcnt,"APPLICANT IDENTIFICATION");
				if(ADVEQLNRDsec(appcnt,1,SCRNvirtualSubData,tmprsrc)) KEYNRD(appcnt,1);
				COPYFIELD(dbsort->fileno,appcat,
					"APPLICANT IDENTIFICATION");
				COPYFIELD(dbsort->fileno,appcat,
					"APPLICATION NUMBER");
				LOCNRDFILE(appcat);
				if(!ADVEQLNRDsec(appcat,1,SCRNvirtualSubData,
					tmprsrc))
				{
					prev=RDATDataNEW(appcat);
					COPYFIELD(dbsort->fileno,appcat,"PERSONNEL INTERVIEW");
					FINDFLDSETSTRING(appcat,"PERSONNEL APPOINTMENT DATE",
						apptdate);
					FINDFLDSETSTRING(appcat,"PERSONNEL APPOINTMENT TIME",
						appttime);
					curdate=GETCURRENTDATE10();
					curtime=GETCURRENTTIME();
					if(withdraw)
					{
						FINDFLDSETCHAR(appcat,"WITHDRAW APPLICANT",withdraw);
						FINDFLDSETSTRING(appcat,"WITHDRAWN BY",USERLOGIN);
						FINDFLDSETSTRING(appcat,"WITHDRAWN DATE",curdate);
						FINDFLDSETSTRING(appcat,"WITHDRAWN TIME",curtime);
					}
					if(!isEMPTY(apptdate) && !isEMPTY(appttime))
					{
						FINDFLDSETSTRING(appcat,"PERSONNEL APPOINTMENT VERIFIED BY",USERLOGIN);
						FINDFLDSETSTRING(appcat,"PERSONNEL APPOINTMENT VERIFIED DATE",curdate);
						FINDFLDSETSTRING(appcat,"PERSONNEL APPOINTMENT VERIFIED TIME",curtime);
					}
					if(!isEMPTY(apptdate) && !isEMPTY(appttime))
					{
					    	/*
						FINDFLDSETCHAR(appcat,"PERSONNEL INTERVIEWED",TRUE);
						FINDFLDSETSTRING(appcat,"PERSONNEL INTERVIEWED BY",USERLOGIN);
						FINDFLDSETSTRING(appcat,"PERSONNEL INTERVIEWED DATE",curdate);
						FINDFLDSETSTRING(appcat,"PERSONNEL INTERVIEWED TIME",curtime);
						*/
					}
					if(curdate!=NULL) Rfree(curdate);
					if(curtime!=NULL) Rfree(curtime);
					ADVWRTTRANSsec(appcat,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
					if(prev!=NULL) FreeRDATData(prev);
					update_diag=TRUE;
					DID_STUFF=TRUE;
				}
				UNLNRDFILE(appcat);
			}
			if(tmprsrc!=NULL)
			{
				update_diagnostic(tmprsrc,(update_diag ? 
					TRUE:FALSE));
			}
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(apptdate!=NULL) Rfree(apptdate);
		if(appttime!=NULL) Rfree(appttime);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);	
			free_rsrc(tmprsrc);
		}
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		dbsort=NULL;
	}
}
static void setfiles(RDArsrc *parent,DBsort *dbsort)
{
        readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	getsupportingfiles(parent,dbsort);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void setkeys(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *appid=NULL,*posid=NULL;
	int appno=0;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"APPLICANT IDENTIFICATION",&appid);
	FINDRSCGETINT(mtnrsrc,"APPLICATION NUMBER",&appno);
	FINDRSCGETSTRING(mtnrsrc,"POSITION IDENTIFICATION",&posid);
	FINDFLDSETSTRING(dbsort->fileno,"APPLICANT IDENTIFICATION",appid);
	FINDFLDSETINT(dbsort->fileno,"APPLICATION NUMBER",appno);
	FINDFLDSETSTRING(dbsort->fileno,"POSITION IDENTIFICATION",posid);
	if(appid!=NULL) Rfree(appid);
	if(posid!=NULL) Rfree(posid);
}
static void getpreviouscb(RDArmem *member,DBsort *dbsort)
{
	RDArsrc *mtnrsrc;

	mtnrsrc=member->parent;
	setkeys(mtnrsrc,dbsort);
	if(EQLNRD(dbsort->fileno,1)) 
	{
		KEYNRD(dbsort->fileno,1);
		if(LTENRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
		}
	} else {
		PRVNRD(dbsort->fileno,1);
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
	}
	getsupportingfiles(mtnrsrc,dbsort);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getnextcb(RDArmem *member,DBsort *dbsort)
{
	RDArsrc *mtnrsrc;

	mtnrsrc=member->parent;
	setkeys(mtnrsrc,dbsort);
	if(EQLNRD(dbsort->fileno,1)) 
	{
		KEYNRD(dbsort->fileno,1);
		if(GTENRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
		}
	} else {
		NXTNRD(dbsort->fileno,1);
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
	}
	getsupportingfiles(mtnrsrc,dbsort);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	setkeys(mtnrsrc,dbsort);
	if(EQLNRD(dbsort->fileno,1)) 
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
	}
	getsupportingfiles(mtnrsrc,dbsort);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void resetdefs(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	DefaultResetScreen(mtnrsrc,TRUE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
	getfile(mtnrsrc,dbsort);
}
static void schedulem(void *targetkey,DBsort *t)
{
	int x=0,y=0;
	RDArsrc *mtnrsrc=NULL;
	short nofields=0,nokeys=0;
	NRDfield *fldx=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;

	mtnrsrc=RDArsrcNEW(module,
		"SCHEDULE PERSONNEL APPOINTMENT MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"APPCAT",NULL,appcat);
	addDFincvir(mtnrsrc,module,"APPCNT",NULL,appcnt);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(mtnrsrc,module,"APPPOS",NULL,apppos);
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	nokeys=NUMKEYS(t->fileno);
	keys=KEYPOINTER(t->fileno);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,TRUE,TRUE);
			FINDRSCSETFUNC(mtnrsrc,fldx->name,setfiles,t);
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
					FINDRSCSETFUNC(mtnrsrc,part->name,getfile,t);
				}
			}
		}
	}
	if(appcat!=(-1)) file2rsrc(appcat,mtnrsrc,FALSE);
	if(appcnt!=(-1)) file2rsrc(appcnt,mtnrsrc,FALSE);
	if(posmst!=(-1)) file2rsrc(posmst,mtnrsrc,FALSE);
	if(apppos!=(-1)) file2rsrc(apppos,mtnrsrc,FALSE);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	if(EQLNRDKEY(t->fileno,1,targetkey)) 
	{
		KEYNRD(t->fileno,1);
	} else { 
		singlefilerecord2rsrc(t->fileno,mtnrsrc);
	}
	getsupportingfiles(mtnrsrc,t);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,t);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,t);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,t,FALSE);
}
static void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("APPMGT","PERSONNEL APPOINTMENT SCHEDULE SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=stralloc(d->expression);
		} else csortsequence->expression=NULL;
		csortsequence->computed=FALSE;
		csortsequence->range=FALSE;
		csortsequence->dlen=0;
		switch(csortsequence->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				csortsequence->value.string_value=NULL;
				csortsequence->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				csortsequence->value.string_value=Rmalloc(1);
				*csortsequence->value.string_value=0;
				csortsequence->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				csortsequence->value.float_value=Rmalloc(sizeof(double));
				*csortsequence->value.float_value=0;
				csortsequence->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				csortsequence->value.short_value=Rmalloc(sizeof(short));
				*csortsequence->value.short_value=0;
				csortsequence->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				csortsequence->value.integer_value=Rmalloc(sizeof(int));
				*csortsequence->value.integer_value=0;
				csortsequence->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
		addDBfield(dbsrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsrt,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	key=KEYNUM(apppos,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(apppos,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
	addDBfield(dbsrt,"APPOINTMENT DATE",DATES,10);
	addDBfield(dbsrt,"APPOINTMENT TIME",TIMES,8);
	addDBfilefield(dbsrt,appcat,"PERSONNEL INTERVIEW");
	addDBfilefield(dbsrt,appcnt,"NAME");
	addDBfilefield(dbsrt,appcnt,"WORK PHONE");
	addDBfilefield(dbsrt,appcnt,"HOME PHONE");
	addDBfilefield(dbsrt,appcat,"WITHDRAW APPLICANT");
}
static void addsortrecord(DBsort *dbsrt)
{
	char deleteflag=FALSE;
	NRDkey *key=NULL;
	NRDpart *part;
	int x;

	ZERNRD(dbsrt->fileno);
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		switch(csortsequence->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				FINDFLDSETSTRING(dbsrt->fileno,csortsequence->name,
					csortsequence->value.string_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				FINDFLDSETCHAR(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				FINDFLDSETDOUBLE(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				FINDFLDSETSHORT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				FINDFLDSETINT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.integer_value);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
	}
	key=KEYNUM(apppos,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(apppos,dbsrt->fileno,part->name);
		}
	}
	COPYFIELD(appcnt,dbsrt->fileno,"NAME");
	COPYFIELD(appcnt,dbsrt->fileno,"WORK PHONE");
	COPYFIELD(appcnt,dbsrt->fileno,"HOME PHONE");
	COPYFIELD(appcat,dbsrt->fileno,"WITHDRAW APPLICANT");
	WRTNRD(dbsrt->fileno,0,NULL);
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	schedulem(targetkey,blist->funcargs);
}
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without processing the Appointments?",NULL,doexit,FALSE,blist);
	} else {
		doexit(blist);
	}
}
static void doexit(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

	dbsort=blist->funcargs;
	drsrc=blist->definelist;
	srsrc=blist->searchrsrc;
	brsrc=blist->mainrsrc;
	FREE_BROWSE_EXIT(blist);
	if(drsrc!=NULL) 
	{
		killwindow(drsrc);
		free_rsrc(drsrc);
	}
	if(srsrc!=NULL) 
	{
		killwindow(srsrc);
		free_rsrc(srsrc);
	}
	if(brsrc!=NULL)
	{
		killwindow(brsrc);
		free_rsrc(brsrc);
	}
	if(dbsort!=NULL)
	{
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static void saveexit(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	DBsort *x;
	RDArsrc *brsrc,*srsrc,*drsrc;

	x=blist->funcargs;
	brsrc=blist->mainrsrc;
	drsrc=blist->definelist;
	srsrc=blist->searchrsrc;
	FREE_BROWSE_EXIT(blist);
	if(drsrc!=NULL) 
	{
		killwindow(drsrc);
		free_rsrc(drsrc);
	}
	if(srsrc!=NULL) 
	{
		killwindow(srsrc);
		free_rsrc(srsrc);
	}
	if(brsrc!=NULL)
	{
		free_rsrc(brsrc);
	}
	if(WindowCount<=2) schedule_backend(x);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static void selectschedule(RDArsrc *mainrsrc)
{
	DBsort *dbsort=NULL;
	char *sortname=NULL,deleteflag=FALSE,sup2int=FALSE,pappt=FALSE,hired=FALSE;
	char *exprssn=NULL,withdraw=FALSE,*e=NULL;
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	short ef=0,update_diag=FALSE;
	RangeScreen *rs=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&exprssn);
	sortname=unique_name();
	dbsort=DBsortNEW("APPMGT",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(appcnt,rs);
	ReadRangeScreen(appcat,rs);
	ReadRangeScreen(apppos,rs);
	ReadRangeScreen(posmst,rs);
	tmprsrc=diagscrn(apppos,"DIAGNOSTIC SCREEN",module,
		"Selecting Applicant Positions",stop_diagnostic);
	addDFincvir(tmprsrc,module,"APPCNT",NULL,appcnt);
	addDFincvir(tmprsrc,module,"APPCAT",NULL,appcat);
	addDFincvir(tmprsrc,module,"APPPOS",NULL,apppos);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,posmst);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(tmprsrc!=NULL)
	{
		if(!ADVmakescrn(tmprsrc,TRUE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	dbsort->fileno=OPNDBsort(dbsort);
	ef=ADVBEGNRDsec(apppos,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		update_diag=FALSE;
		if(abort_diagnostic) break;
		FINDFLDGETCHAR(apppos,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(apppos,"SUPERVISOR TO INTERVIEW",&sup2int);
		FINDFLDGETCHAR(apppos,"HIRED",&hired);
		if(!deleteflag && !hired)
		/*
		if(!deleteflag && !sup2int)
		    */
		{
			if(!testfilerangersrc(apppos,rs))
			{
				COPYFIELD(apppos,posmst,"POSITION IDENTIFICATION");
				if(!ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,tmprsrc))
				{
					FINDFLDGETCHAR(posmst,"DELETEFLAG",&deleteflag);
					if(!deleteflag && !testfilerangersrc(posmst,rs))
					{
						COPYFIELD(apppos,appcat,"APPLICANT IDENTIFICATION");
						COPYFIELD(apppos,appcat,"APPLICATION NUMBER");
						if(!ADVEQLNRDsec(appcat,1,SCRNvirtualSubData,tmprsrc))
						{
							FINDFLDGETCHAR(appcat,"DELETEFLAG",&deleteflag);
							FINDFLDGETCHAR(appcat,
								"PERSONNEL APPOINTMENT VERIFIED",&pappt);
							FINDFLDGETCHAR(appcat,"PERSONNEL INTERVIEWED",
								&sup2int);
							FINDFLDGETCHAR(appcat,"WITHDRAW APPLICANT",
								&withdraw);
							/*
							if(!deleteflag && !pappt && !withdraw &&
								!sup2int)
							    */
							if(!deleteflag)
							{
								if(!testfilerangersrc(appcat,rs))
								{
									COPYFIELD(apppos,appcnt,
										"APPLICANT IDENTIFICATION");
									if(!ADVEQLNRDsec(appcnt,1,
										SCRNvirtualSubData,tmprsrc))
									{
										FINDFLDGETCHAR(appcnt,"DELETEFLAG",
											&deleteflag);
										if(!deleteflag && 
											!testfilerangersrc(appcnt,rs))
										{
											if(!isEMPTY(exprssn))
											{
												if(EVALUATEbol(exprssn,
													SCRNvirtualSubData,
													tmprsrc))
												{
													addsortrecord(dbsort);
													update_diag=TRUE;
												}
											} else {
												addsortrecord(dbsort);
												update_diag=TRUE;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if(tmprsrc!=NULL)
		{
			update_diagnostic(tmprsrc,(update_diag ? 
				TRUE:FALSE));
		}
		ef=ADVSEQNRDsec(apppos,SCRNvirtualSubData,tmprsrc);
	}
	if(exprssn!=NULL) Rfree(exprssn);
	if(rs!=NULL) FreeRangeScreen(rs);
	if(tmprsrc!=NULL)
	{
		if(!abort_diagnostic) killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(SIZNRD(dbsort->fileno))
	{
		brsrc=RDArsrcNEW(module,
			"SCHEDULE PERSONNEL APPOINTMENT BROWSE");
		srsrc=RDArsrcNEW(module,
			"SCHEDULE PERSONNEL APPOINTMENT SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,
			"SCHEDULE PERSONNEL APPOINTMENT DEFINE LIST");
		addDFincvir(drsrc,module,"APPCAT",NULL,appcat);
		addDFincvir(drsrc,module,"APPCNT",NULL,appcnt);
		addDFincvir(drsrc,module,"APPPOS",NULL,apppos);
		addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmst);
		addDFincvir(srsrc,module,"APPCAT",NULL,appcat);
		addDFincvir(srsrc,module,"APPCNT",NULL,appcnt);
		addDFincvir(srsrc,module,"APPPOS",NULL,apppos);
		addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmst);
		GET_SCREEN_VIRTUALS(drsrc,1);
		GET_SCREEN_VIRTUALS(srsrc,2);
		ZERNRD(dbsort->fileno);
		addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
		mbl_trans=ADVbrowse(dbsort->fileno,1,brsrc,srsrc,drsrc,NULL,
			selectfunction,doexit_test,okfunc,NULL,dbsort,FALSE,NULL,
			0,0,0,TRUE);
		FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
	} else {
		prterr("Error No Applicant were Selected.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO APPLICANTS SELECTED!","There were no applicants selected using the specified range criteria.  Please try again and/or call your installer.",NULL,NULL,TRUE,NULL);
		if(dbsort!=NULL) 
		{
			CLSDBsort(dbsort);
			FreeDBsort(dbsort);
		}
	}
}
static void quitschedule(RDArsrc *mainrsrc)
{
	APPlib *args=NULL;

	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(DID_STUFF)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(reportname!=NULL) Rfree(reportname);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,
		"SCHEDULE PERSONNEL APPOINTMENTS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("PERSCH");
	if((appcat=APPOPNNRD(module,"APPCAT",TRUE,TRUE))==(-1)) return;
	if((appcnt=APPOPNNRD(module,"APPCNT",TRUE,FALSE))==(-1)) return;
	if((apppos=APPOPNNRD(module,"APPPOS",TRUE,FALSE))==(-1)) return;
	if((posmst=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"SCHEDULE PERSONNEL APPOINTMENTS");
	addDFincvir(mainrsrc,module,"APPCAT",NULL,appcat);
	addDFincvir(mainrsrc,module,"APPCNT",NULL,appcnt);
	addDFincvir(mainrsrc,module,"APPPOS",NULL,apppos);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmst);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	file2rangersrc(appcat,mainrsrc);
	file2rangersrc(appcnt,mainrsrc);
	file2rangersrc(apppos,mainrsrc);
	file2rangersrc(posmst,mainrsrc);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectschedule,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitschedule,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DID_STUFF=0;
	SetRangersrcsensitive(mainrsrc);
	APPmakescrn(mainrsrc,FALSE,quitschedule,NULL,TRUE);
	RDAAPPMAINLOOP();
}
