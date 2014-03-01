/* appai.c - Administrative Interview */
/*lint -library */
#ifndef WIN32
#define __NAM__ "appai.lnx"
#endif
#ifdef WIN32
#define __NAM__ "appai.exe"
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

short appcnt=(-1),appcat=(-1),apppos=(-1),posmst=(-1),posvac=(-1);
char *module="APPMGT",*reportname=NULL;
APPlib *VACANCIES=NULL;
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
static void addsortrecord(DBsort *);
static void makeDBsortfile(DBsort *);
static void interview_backend(DBsort *);
static void interviewm(void *,DBsort *);
static void dovacancylist(RDArsrc *,DBsort *);

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
	char rec2hire=FALSE,*vac=NULL,deleteflag=FALSE;
	int appno=0,selected=0;

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
		FINDRSCGETCHAR(mtnrsrc,"RECOMMEND TO HIRE",&rec2hire);
		FINDRSCGETCHAR(mtnrsrc,"APPROVED TO HIRE",&deleteflag);
		if(posvac!=(-1) && (rec2hire || deleteflag))
		{
			FINDRSCGETINT(mtnrsrc,"VACANCY IDENTIFICATIONS",&selected);
			FINDFLDSETSTRING(dbsort->fileno,"VACANCY IDENTIFICATION",
				VACANCIES->libs[selected]);
			vac=VACANCIES->libs[selected];
			FINDFLDSETSTRING(posvac,"POSITION IDENTIFICATION",posid);
			FINDFLDSETSTRING(posvac,"VACANCY IDENTIFICATION",vac);
			if(ADVEQLNRDsec(posvac,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(posvac,1);
				warnmessage=Rmalloc(RDAstrlen(vac)+137);
				sprintf(warnmessage,"The VACANCY IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Vacancy Master.",(vac==NULL?"":vac));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VACANCY IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(posid!=NULL) Rfree(posid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(posvac,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(vac)+140);
					sprintf(warnmessage,"The VACANCY IDENTIFICATION of [%s] entered is deleted.  It exists as a vacancy, but is presently set up in a deleted state.",(vac==NULL ? "":vac));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VACANCY IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
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

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,"Browse Diagnostic",NULL);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE))
					ForceWindowUpdate(tmprsrc);
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
	if(VACANCIES!=NULL) freeapplib(VACANCIES);
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(WindowCount<2) interview_backend(dbsort);
}
static void interview_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE;
	char deleteflag=FALSE,*curdate=NULL,*curtime=NULL;
	char withdraw=FALSE,sup2int=FALSE,*interview=NULL;
	char pers_int=FALSE;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Scheduling Personnel Appointments",NULL);
		addDFincvir(tmprsrc,module,"APPCNT",NULL,appcnt);
		addDFincvir(tmprsrc,module,"APPCAT",NULL,appcat);
		addDFincvir(tmprsrc,module,"APPPOS",NULL,apppos);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(dbsort->fileno,"WITHDRAW APPLICANT",&withdraw);
			FINDFLDGETSTRING(dbsort->fileno,"ADMINISTRATOR INTERVIEW",&interview);
			FINDFLDGETCHAR(dbsort->fileno,"APPROVED TO HIRE",&sup2int);
			FINDFLDGETCHAR(dbsort->fileno,"ADMINISTRATOR INTERVIEWED",&pers_int);
			if(!deleteflag && (pers_int || withdraw ||
				sup2int))
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
					curdate=GETCURRENTDATE10();
					curtime=GETCURRENTTIME();
					if(withdraw)
					{
						FINDFLDSETCHAR(appcat,"WITHDRAW APPLICANT",withdraw);
						FINDFLDSETSTRING(appcat,"WITHDRAWN BY",USERLOGIN);
						FINDFLDSETSTRING(appcat,"WITHDRAWN DATE",curdate);
						FINDFLDSETSTRING(appcat,"WITHDRAWN TIME",curtime);
					}
					FINDFLDSETSTRING(appcat,"ADMINISTRATOR INTERVIEW",interview);
					FINDFLDSETCHAR(appcat,"ADMINISTRATOR INTERVIEWED",pers_int);
					if(pers_int)
					{
						FINDFLDSETSTRING(appcat,"ADMINISTRATOR INTERVIEWED BY",USERLOGIN);
						FINDFLDSETSTRING(appcat,"ADMINISTRATOR INTERVIEWED DATE",curdate);
						FINDFLDSETSTRING(appcat,"ADMINISTRATOR INTERVIEWED TIME",curtime);
					}
					if(curdate!=NULL) Rfree(curdate);
					if(curtime!=NULL) Rfree(curtime);
					ADVWRTTRANSsec(appcat,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
					if(prev!=NULL) FreeRDATData(prev);
					if(sup2int)
					{
						COPYFIELD(dbsort->fileno,apppos,
							"APPLICANT IDENTIFICATION");
						COPYFIELD(dbsort->fileno,apppos,
							"APPLICATION NUMBER");
						COPYFIELD(dbsort->fileno,apppos,
							"POSITION IDENTIFICATION");
						LOCNRDFILE(apppos);
						if(!ADVEQLNRDsec(apppos,1,SCRNvirtualSubData,
							tmprsrc))
						{
							prev=RDATDataNEW(apppos);
							FINDFLDSETCHAR(apppos,"APPROVED TO HIRE",
								sup2int);
							curdate=GETCURRENTDATE10();
							curtime=GETCURRENTTIME();
							FINDFLDSETSTRING(apppos,"APPROVED TO HIRE BY",USERLOGIN);
							FINDFLDSETSTRING(apppos,"APPROVED TO HIRE DATE",curdate);
							FINDFLDSETSTRING(apppos,"APPROVED TO HIRE TIME",curtime);
							if(curdate!=NULL) Rfree(curdate);
							if(curtime!=NULL) Rfree(curtime);
							ADVWRTTRANSsec(apppos,0,NULL,prev,
								SCRNvirtualSubData,tmprsrc);
							if(prev!=NULL) FreeRDATData(prev);
						}
						UNLNRDFILE(apppos);
					}
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
		if(interview!=NULL) Rfree(interview);
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
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *appid=NULL,*posid=NULL,*vac=NULL;
	int appno=0,start=0;

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
	if(EQLNRD(dbsort->fileno,1)) 
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
	}
	getsupportingfiles(mtnrsrc,dbsort);
	dovacancylist(mtnrsrc,dbsort);
	FINDFLDGETSTRING(dbsort->fileno,"VACANCY IDENTIFICATION",&vac);
	for(start=0;start<VACANCIES->numlibs;++start)
	{
		if(!RDAstrcmp(vac,VACANCIES->libs[start])) break;
	}
	if(start>=VACANCIES->numlibs) start=0;
	FINDRSCLISTAPPlib(mtnrsrc,"VACANCY IDENTIFICATIONS",start,VACANCIES);
	if(vac!=NULL) Rfree(vac);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void dovacancylist(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	short ef=FALSE;
	char *posid=NULL,*lposid=NULL,deleteflag=FALSE,*vac=NULL;
	char filled=FALSE;

	if(VACANCIES!=NULL) freeapplib(VACANCIES);
	VACANCIES=APPlibNEW();
	ZERNRD(posvac);
	FINDFLDGETSTRING(dbsort->fileno,"POSITION IDENTIFICATION",&posid);
/* 	FINDFLDSETSTRING(dbsort->fileno,"POSITION IDENTIFICATION",posid); */
	FINDFLDSETSTRING(posvac,"POSITION IDENTIFICATION",posid);
	ef=ADVGTENRDsec(posvac,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(posvac,"POSITION IDENTIFICATION",&lposid);
		if(RDAstrcmp(lposid,posid)) break;
		FINDFLDGETCHAR(posvac,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(posvac,"FILLED",&filled);
		if(!deleteflag && !filled)
		{
			FINDFLDGETSTRING(posvac,"VACANCY IDENTIFICATION",&vac);
			addAPPlib(VACANCIES,vac);
			if(vac!=NULL) Rfree(vac);
		}
		ef=ADVNXTNRDsec(posvac,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(posid!=NULL) Rfree(posid);
	if(lposid!=NULL) Rfree(lposid);
	if(VACANCIES->numlibs<1)
	{
		addAPPlib(VACANCIES,"No Vacancies Available");
	}
}
static void changevacancy(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	int selected=0;
	char *posid=NULL;

	ZERNRD(posvac);
	readwidget(mtnrsrc,"POSITION IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"POSITION IDENTIFICATION",&posid);
	FINDFLDSETSTRING(posvac,"POSITION IDENTIFICATION",posid);
	if(posid!=NULL) Rfree(posid);
	FINDRSCGETINT(mtnrsrc,"VACANCY IDENTIFICATIONS",&selected);
	if(RDAstrcmp(VACANCIES->libs[selected],"No Vacancies Available"))
	{
		FINDFLDSETSTRING(posvac,"VACANCY IDENTIFICATION",
			VACANCIES->libs[selected]);
		FINDFLDSETSTRING(dbsort->fileno,"VACANCY IDENTIFICATION",
			VACANCIES->libs[selected]);
	}
	if(ADVEQLNRDsec(posvac,1,SCRNvirtualSubData,mtnrsrc))
		KEYNRD(posvac,1);
	filerecord2rsrc(posvac,mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void appcertfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPCERT");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPCERT KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void appendfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPEND");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPEND KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void appcrtfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPCRT.BROWSE");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPCRT KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void appeducfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPEDM");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPEDM KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void appdocfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPDOC");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPDOC KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void appreferfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPREF");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPREF KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void appempfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPEMP");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPEMP KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void apptestfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"APPMGT");
	addAPPlib(args,"MTN APPTEST");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"APPTEST KEY",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void posrespfunc(RDArsrc *parent,DBsort *dbsort)
{
	APPlib *args=NULL;

	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	args=APPlibNEW();
	addAPPlib(args,"POSTRK");
	addAPPlib(args,"MTN POSPRMS");
	ADVMTNSCRN_EXECUTEPROGRAM("mtnmst",parent,"POSPRMS",
		dbsort->fileno,1,1,args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void interviewm(void *targetkey,DBsort *t)
{
	int x=0,start=0;
	RDArsrc *mtnrsrc=NULL;
	short nofields=0;
	NRDfield *fldx=NULL,*fields=NULL;
	char *vac=NULL;

	mtnrsrc=RDArsrcNEW(module,
		"ADMINISTRATOR INTERVIEW MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"APPCAT",NULL,appcat);
	addDFincvir(mtnrsrc,module,"APPCNT",NULL,appcnt);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(mtnrsrc,module,"APPPOS",NULL,apppos);
	addDFincvir(mtnrsrc,"POSTRK","POSVAC",NULL,posvac);
	if(VACANCIES!=NULL) freeapplib(VACANCIES);
	VACANCIES=NULL;
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"VACANCY IDENTIFICATION"))
			{
				VACANCIES=APPlibNEW();
				addlstrsrc(mtnrsrc,"VACANCY IDENTIFICATIONS",
					&start,TRUE,changevacancy,VACANCIES->numlibs,
					&VACANCIES->libs,t);
			} else {
				ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,TRUE,TRUE);
			}
		}
	}
	if(appcat!=(-1)) file2rsrc(appcat,mtnrsrc,FALSE);
	if(appcnt!=(-1)) file2rsrc(appcnt,mtnrsrc,FALSE);
	if(posmst!=(-1)) file2rsrc(posmst,mtnrsrc,FALSE);
	if(apppos!=(-1)) file2rsrc(apppos,mtnrsrc,FALSE);
	if(posvac!=(-1)) file2rsrc(posvac,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"APPLICANT IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"APPLICATION NUMBER",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"POSITION IDENTIFICATION",getfile,t);
	DefaultScreens(mtnrsrc);
	if(EQLNRDKEY(t->fileno,1,targetkey)) 
	{
		KEYNRD(t->fileno,1);
	} else { 
		singlefilerecord2rsrc(t->fileno,mtnrsrc);
	}
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	dovacancylist(mtnrsrc,t);
	FINDFLDGETSTRING(t->fileno,"VACANCY IDENTIFICATION",&vac);
	for(x=0;x<VACANCIES->numlibs;++x)
	{
		if(!RDAstrcmp(VACANCIES->libs[x],vac)) break;
	}
	if(x<VACANCIES->numlibs) start=x;
		else start=0;
	FINDRSCLISTAPPlib(mtnrsrc,"VACANCY IDENTIFICATIONS",start,VACANCIES);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	getsupportingfiles(mtnrsrc,t);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(mtnrsrc,"RESPONSIBILITIES",TRUE,posrespfunc,t);
	addbtnrsrc(mtnrsrc,"CRITERIA",TRUE,appcrtfunc,t);
	addbtnrsrc(mtnrsrc,"CERTIFICATIONS",TRUE,appcertfunc,t);
	addbtnrsrc(mtnrsrc,"ENDORSEMENTS",TRUE,appendfunc,t);
	addbtnrsrc(mtnrsrc,"EDUCATION",TRUE,appeducfunc,t);
	addbtnrsrc(mtnrsrc,"DOCUMENTS",TRUE,appdocfunc,t);
	addbtnrsrc(mtnrsrc,"REFERENCES",TRUE,appreferfunc,t);
	addbtnrsrc(mtnrsrc,"PREVIOUS EMPLOYMENT",TRUE,appempfunc,t);
	addbtnrsrc(mtnrsrc,"TESTS",TRUE,apptestfunc,t);
	APPmakescrn(mtnrsrc,FALSE,quit_record,t,FALSE);
	changevacancy(mtnrsrc,t);
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
	d=getDFvirtual("APPMGT","ADMINISTRATOR INTERVIEW SORT SEQUENCE");
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
	addDBfield(dbsrt,"APPOINTMENT DATE",DATES,10);
	addDBkeypart(dbsrt,keyno,"APPOINTMENT DATE");
	addDBfield(dbsrt,"APPOINTMENT TIME",TIMES,8);
	addDBkeypart(dbsrt,keyno,"APPOINTMENT TIME");
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
	addDBfield(dbsrt,"ADMINISTRATOR INTERVIEW",SCROLLEDTEXT,400);
	addDBfield(dbsrt,"NAME",PLAINTEXT,40);
	addDBfield(dbsrt,"ADMINISTRATOR INTERVIEWED",BOOLNS,1);
	addDBfield(dbsrt,"APPROVED TO HIRE",BOOLNS,1);
	addDBfield(dbsrt,"WITHDRAW APPLICANT",BOOLNS,1);
	addDBfield(dbsrt,"VACANCY IDENTIFICATION",VARIABLETEXT,0);
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
	FIELDCOPY(appcat,"ADMINISTRATOR APPOINTMENT DATE",
		dbsrt->fileno,"APPOINTMENT DATE");
	FIELDCOPY(appcat,"ADMINISTRATOR APPOINTMENT TIME",
		dbsrt->fileno,"APPOINTMENT TIME");
	COPYFIELD(appcat,dbsrt->fileno,"ADMINISTRATOR INTERVIEWED");
	COPYFIELD(appcat,dbsrt->fileno,"ADMINISTRATOR INTERVIEW");
	COPYFIELD(appcnt,dbsrt->fileno,"NAME");
	COPYFIELD(appcnt,dbsrt->fileno,"WORK PHONE");
	COPYFIELD(appcnt,dbsrt->fileno,"HOME PHONE");
	COPYFIELD(appcat,dbsrt->fileno,"WITHDRAW APPLICANT");
	COPYFIELD(apppos,dbsrt->fileno,"VACANCY IDENTIFICATION");
	WRTNRD(dbsrt->fileno,0,NULL);
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	interviewm(targetkey,blist->funcargs);
}
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without processing the Interviews?",NULL,doexit,FALSE,blist);
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
	if(WindowCount<=2) interview_backend(x);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static void selectinterview(RDArsrc *mainrsrc)
{
	DBsort *dbsort=NULL;
	char *sortname=NULL,deleteflag=FALSE,sup2int=FALSE,hired=FALSE;
	char *exprssn=NULL,withdraw=FALSE,pappt=FALSE,rec2hire=FALSE;
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
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE)) 
			ForceWindowUpdate(tmprsrc);
	dbsort->fileno=OPNDBsort(dbsort);
	ef=ADVBEGNRDsec(apppos,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		if(abort_diagnostic) break;
		update_diag=FALSE;
		FINDFLDGETCHAR(apppos,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(apppos,"SUPERVISOR INTERVIEW VERIFIED",&sup2int);
		FINDFLDGETCHAR(apppos,"RECOMMEND TO HIRE",&rec2hire);
		FINDFLDGETCHAR(apppos,"HIRED",&hired);
		/*
		if(!deleteflag && sup2int && rec2hire)
		 */
		if(!deleteflag && !hired)
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
								"ADMINISTRATOR APPOINTMENT VERIFIED",&pappt);
							FINDFLDGETCHAR(appcat,
								"ADMINISTRATOR INTERVIEWED",&sup2int);
							FINDFLDGETCHAR(appcat,"WITHDRAW APPLICANT",
								&withdraw);
							/*
							if(!deleteflag && pappt && !withdraw && !sup2int)
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
			"ADMINISTRATOR INTERVIEW BROWSE");
		srsrc=RDArsrcNEW(module,
			"ADMINISTRATOR INTERVIEW SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,
			"ADMINISTRATOR INTERVIEW DEFINE LIST");
		addDFincvir(drsrc,module,"APPCAT",NULL,appcat);
		addDFincvir(drsrc,module,"APPCNT",NULL,appcnt);
		addDFincvir(drsrc,module,"APPPOS",NULL,apppos);
		addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmst);
		addDFincvir(drsrc,"POSTRK","POSVAC",NULL,posvac);
		addDFincvir(srsrc,module,"APPCAT",NULL,appcat);
		addDFincvir(srsrc,module,"APPCNT",NULL,appcnt);
		addDFincvir(srsrc,module,"APPPOS",NULL,apppos);
		addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmst);
		addDFincvir(srsrc,"POSTRK","POSVAC",NULL,posvac);
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
static void quitinterview(RDArsrc *mainrsrc)
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
	CLSNRD(appcnt);
	CLSNRD(posvac);
	CLSNRD(appcat);
	CLSNRD(apppos);
	CLSNRD(posmst);
	if(VACANCIES!=NULL) freeapplib(VACANCIES);
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
		"ADMINISTRATOR INTERVIEWS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("ADMINT");
	if((appcat=APPOPNNRD(module,"APPCAT",TRUE,TRUE))==(-1)) return;
	if((appcnt=APPOPNNRD(module,"APPCNT",TRUE,FALSE))==(-1)) return;
	if((apppos=APPOPNNRD(module,"APPPOS",TRUE,TRUE))==(-1)) return;
	if((posmst=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((posvac=APPOPNNRD("POSTRK","POSVAC",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"ADMINISTRATOR INTERVIEWS");
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
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectinterview,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitinterview,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DID_STUFF=0;
	SetRangersrcsensitive(mainrsrc);
	APPmakescrn(mainrsrc,FALSE,quitinterview,NULL,TRUE);
	RDAAPPMAINLOOP();
}
