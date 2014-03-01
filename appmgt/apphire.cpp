/* apphire.c - Hire Applicants Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "apphire.lnx"
#endif
#ifdef WIN32
#define __NAM__ "apphire.exe"
#endif
#include <app.hpp>



#include <cstdlib>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <postrk.hpp>
#include <ldval.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB add */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	RDATData *previous;
	MakeBrowseList *mbl_paydedm;
};
typedef struct mtnstructs mtnstruct;

char *module="APPMGT",*hiredate=NULL,*peridform=NULL,*reportname=NULL;
int step=1;
MakeBrowseList *mbl_locmstr=NULL;
char GetDocuments=TRUE,GetEducation=TRUE,GetCertifications=TRUE;
char GetEndorsements=TRUE,GetCriteria=TRUE;
static APPlib *VACANCIES=NULL;
static APPlib *STATUSES=NULL;
static APPlib *PAYGROUPS=NULL;
static short DID_STUFF=0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static void browse_locations(RDArsrc *);
static void save_check(RDArsrc *,short,DBsort *);
static void save_record(RDArsrc *,short,DBsort *);
static void quit_record(RDArsrc *,DBsort *);
static void doexit_test(MakeBrowseList *);
static void doexit(MakeBrowseList *);
static void okfunc(MakeBrowseList *,void *);
static void saveexit(RDArsrc *,MakeBrowseList *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static void addsortrecord(RDArsrc *,DBsort *);
static void makeDBsortfile(DBsort *);
static void hire_applicant(RDArsrc *,DBsort *);
static void hire_backend(DBsort *);
static void hirem(void *,DBsort *);
static void dovacancylist(RDArsrc *,DBsort *);
static void dostatuslist(RDArsrc *,DBsort *);
static void dopaygrouplist(RDArsrc *,DBsort *);
short appcnt=(-1),appcat=(-1),apppos=(-1),posmst=(-1),poslnum=(-1),posvac=(-1),poscls=(-1);
short posper=(-1),perdmg=(-1),percrt=(-1),percert=(-1),perend=(-1);
short perdoc=(-1),peredm=(-1),perstat=(-1),payjmst=(-1),paydedm=(-1),paygrp=(-1);
short posdedc=(-1),posdded=(-1),appcrt=(-1),appcert=(-1),append=(-1);
short appdoc=(-1),appedm=(-1),personnel_type=(-1);

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
static void closefiles()
{
	if(appcnt!=(-1)) CLSNRD(appcnt);
	if(appcat!=(-1)) CLSNRD(appcat);
	if(apppos!=(-1)) CLSNRD(apppos);
	if(posmst!=(-1)) CLSNRD(posmst);
	if(poslnum!=(-1)) CLSNRD(poslnum);
	if(posvac!=(-1)) CLSNRD(posvac);
	if(poscls!=(-1)) CLSNRD(poscls);
	if(posper!=(-1)) CLSNRD(posper);
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(percrt!=(-1)) CLSNRD(percrt);
	if(percert!=(-1)) CLSNRD(percert);
	if(perend!=(-1)) CLSNRD(perend);
	if(perdoc!=(-1)) CLSNRD(perdoc);
	if(peredm!=(-1)) CLSNRD(peredm);
	if(perstat!=(-1)) CLSNRD(perstat);
	if(paygrp!=(-1)) CLSNRD(paygrp);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(paydedm!=(-1)) CLSNRD(paydedm);
	if(posdded!=(-1)) CLSNRD(posdded);
	if(posdedc!=(-1)) CLSNRD(posdedc);
	if(appcrt!=(-1)) CLSNRD(appcrt);
	if(appcert!=(-1)) CLSNRD(appcert);
	if(append!=(-1)) CLSNRD(append);
	if(appdoc!=(-1)) CLSNRD(appdoc);
	if(appedm!=(-1)) CLSNRD(appedm);
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
	char *appid=NULL,*warnmessage=NULL,delflag=FALSE,*posid=NULL,*locid=NULL;
	char rec2hire=FALSE,*vac=NULL,*status=NULL,*paygroup=NULL;
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
		FINDFLDGETCHAR(dbsort->fileno,"RECOMMEND TO HIRE",&rec2hire);
		if(posvac!=(-1) && rec2hire)
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
					sprintf(warnmessage,"The VACANCY IDENTIFICATION of [%s] entered is deleted.  It exists as a vacancy, but is presently set up in a deleted state.",(vac==NULL?"":vac));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VACANCY IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(posid!=NULL) Rfree(posid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(perstat!=(-1) && rec2hire)
		{
			FINDRSCGETINT(mtnrsrc,"STATUS TITLES",&selected);
			FINDFLDSETSTRING(dbsort->fileno,"STATUS TITLE",
				STATUSES->libs[selected]);
			status=STATUSES->libs[selected];
			FINDFLDSETSTRING(perstat,"STATUS TITLE",status);
			if(ADVEQLNRDsec(perstat,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(perstat,1);
				warnmessage=Rmalloc(RDAstrlen(status)+137);
				sprintf(warnmessage,"The STATUS TITLE of [%s] entered is not valid.  It does not exist in Personnel.",(status==NULL?"":status));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID STATUS TITLE!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(posid!=NULL) Rfree(posid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(perstat,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(status)+140);
					sprintf(warnmessage,"The STATUS TITLE of [%s] entered is deleted.  It exists, but it is presently set up in a deleted state.",(status==NULL?"":status));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED STATUS TITLE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(posid!=NULL) Rfree(posid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(paygrp!=(-1) && rec2hire)
		{
			FINDRSCGETINT(mtnrsrc,"PAY GROUPS",&selected);
			FINDFLDSETSTRING(dbsort->fileno,"PAY GROUP",
				PAYGROUPS->libs[selected]);
			paygroup=PAYGROUPS->libs[selected];
			FINDFLDSETSTRING(paygrp,"PAY GROUP",paygroup);
			if(ADVEQLNRDsec(paygrp,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(paygrp,1);
				warnmessage=Rmalloc(RDAstrlen(paygroup)+137);
				sprintf(warnmessage,"The PAY GROUP of [%s] entered is not valid.  It does not exist in Payroll.",(paygroup==NULL?"":paygroup));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAY GROUP!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(posid!=NULL) Rfree(posid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(paygrp,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(paygroup)+140);
					sprintf(warnmessage,"The PAY GROUP of [%s] entered is deleted.  It exists, but it is presently set up in a deleted state.",(paygroup==NULL?"":paygroup));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PAY GROUP!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(posid!=NULL) Rfree(posid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(poslnum!=(-1) && rec2hire)
		{
			FINDFLDGETSTRING(dbsort->fileno,"LOCATION IDENTIFICATION",&locid);
			FINDFLDSETSTRING(poslnum,"LOCATION IDENTIFICATION",locid);
			if(ADVEQLNRDsec(poslnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(poslnum,1);
				warnmessage=Rmalloc(RDAstrlen(locid)+141);
				sprintf(warnmessage,"The LOCATION IDENTIFICATION of [%s] entered is not valid.  It does not exist in Position Tracking.",(locid==NULL?"":locid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LOCATION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(appid!=NULL) Rfree(appid); 
				if(locid!=NULL) Rfree(locid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(poslnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(locid)+154);
					sprintf(warnmessage,"The LOCATION IDENTIFICATION of [%s] entered is deleted.  It exists, but it is presently set up in a deleted state.",(locid==NULL?"":locid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LOCATION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(appid!=NULL) Rfree(appid); 
					if(locid!=NULL) Rfree(locid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
	}
	if(posid!=NULL) Rfree(posid);
	if(locid!=NULL) Rfree(locid);
	if(appid!=NULL) Rfree(appid);
	save_record(mtnrsrc,update_list,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;
	char *vac=NULL,rec2hire=TRUE,*e=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDFLDGETCHAR(dbsort->fileno,"RECOMMEND TO HIRE",&rec2hire);
	if(!rec2hire)
	{
		FINDFLDSETSTRING(dbsort->fileno,"VACANCY IDENTIFICATION",vac);
	}
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
	COPYFIELD(dbsort->fileno,poslnum,"LOCATION IDENTIFICATION");
	if(ADVEQLNRDsec(poslnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(poslnum,1);
	filerecord2rsrc(poslnum,mtnrsrc);
	COPYFIELD(dbsort->fileno,posvac,"POSITION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,posvac,"VACANCY IDENTIFICATION");
	if(ADVEQLNRDsec(posvac,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(posvac,1);
	filerecord2rsrc(posvac,mtnrsrc);
	COPYFIELD(dbsort->fileno,perstat,"STATUS TITLE");
	if(ADVEQLNRDsec(perstat,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(perstat,1);
	filerecord2rsrc(perstat,mtnrsrc);
}
static void updatelocation(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *locid=NULL;
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"LOCATION IDENTIFICATION",&locid);
	FINDFLDSETSTRING(poslnum,"LOCATION IDENTIFICATION",locid);
	if(ADVEQLNRDsec(poslnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(poslnum,1);
	filerecord2rsrc(poslnum,mtnrsrc);
	if(locid!=NULL) Rfree(locid);
	/*computeallSCRNvirtuals(mtnrsrc);*/
	updateallrsrc(mtnrsrc);
}
static void quit_record(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	if(VACANCIES!=NULL) freeapplib(VACANCIES);
	if(STATUSES!=NULL) freeapplib(STATUSES);
	if(PAYGROUPS!=NULL) freeapplib(PAYGROUPS);
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(WindowCount<2) hire_backend(dbsort);
}
static void hire_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE;
	char deleteflag=FALSE,sup2int=FALSE,hired=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(dbsort!=NULL)
	{
		/*RUNREPORT("APPMGT","APPHIREAUDIT",dbsort->servername,dbsort->filename,FALSE,TRUE,NULL);*/
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Hiring Applicants",NULL);
		addDFincvir(tmprsrc,module,"APPPOS",NULL,apppos);
		addDFincvir(tmprsrc,module,"APPCNT",NULL,appcnt);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			CLEAR_SCREEN_VIRTUAL(tmprsrc);
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(dbsort->fileno,"HIRED",&hired);
			FINDFLDGETCHAR(apppos,"APPROVED TO HIRE",&sup2int);
			if(!deleteflag && sup2int && !hired)
			{
				COPYFIELD(dbsort->fileno,appcnt,"APPLICANT IDENTIFICATION");
				if(!ADVEQLNRDsec(appcnt,1, SCRNvirtualSubData,tmprsrc))
				{
					COPYFIELD(dbsort->fileno,apppos,"APPLICANT IDENTIFICATION");
					COPYFIELD(dbsort->fileno,apppos,"APPLICATION NUMBER");
					COPYFIELD(dbsort->fileno,apppos,"POSITION IDENTIFICATION");
					COPYFIELD(dbsort->fileno,posmst,"POSITION IDENTIFICATION");
					if(!ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,tmprsrc))
					{
						COPYFIELD(dbsort->fileno,posmst,"LOCATION IDENTIFICATION");
					}
					if(!ADVEQLNRDsec(apppos,1,SCRNvirtualSubData,tmprsrc))
					{
						hire_applicant(tmprsrc, dbsort);
					}
				}
			}
			if(tmprsrc!=NULL)
			{
				update_diagnostic(tmprsrc,(update_diag ? TRUE : FALSE));
			}
			ef=NXTNRD(dbsort->fileno,1);
		}
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
	char *appid=NULL,*posid=NULL,*vac=NULL,*status=NULL,*paygroup=NULL;
	int appno=0,start=0,startstatus=0,startpaygroup=0;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"VACANCY IDENTIFICATIONS",&start);
	FINDRSCGETINT(mtnrsrc,"STATUS TITLES",&startstatus);
	FINDRSCGETINT(mtnrsrc,"PAYGROUPS",&startpaygroup);
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
	dostatuslist(mtnrsrc,dbsort);
	dopaygrouplist(mtnrsrc,dbsort);
	FINDFLDGETSTRING(dbsort->fileno,"VACANCY IDENTIFICATION",&vac);
	for(start=0;start<VACANCIES->numlibs;++start)
	{
		if(!RDAstrcmp(vac,VACANCIES->libs[start])) break;
	}
	if(start>=VACANCIES->numlibs) start=0;
	FINDRSCLISTAPPlib(mtnrsrc,"VACANCY IDENTIFICATIONS",start,VACANCIES);
	if(vac!=NULL) Rfree(vac);
	FINDFLDGETSTRING(dbsort->fileno,"STATUS TITLE",&status);
	for(start=0;start<STATUSES->numlibs;++start)
	{
		if(!RDAstrcmp(status,STATUSES->libs[start])) break;
	}
	if(start>=STATUSES->numlibs) start=0;
	FINDRSCLISTAPPlib(mtnrsrc,"STATUS TITLES",start,STATUSES);
	if(status!=NULL) Rfree(status);
	FINDFLDGETSTRING(dbsort->fileno,"PAY GROUP",&paygroup);
	for(start=0;start<PAYGROUPS->numlibs;++start)
	{
		if(!RDAstrcmp(paygroup,PAYGROUPS->libs[start])) break;
	}
	if(start>=PAYGROUPS->numlibs) start=0;
	FINDRSCLISTAPPlib(mtnrsrc,"PAY GROUPS",start,PAYGROUPS);
	if(paygroup!=NULL) Rfree(paygroup);
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
static void dostatuslist(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	short ef=FALSE;
	char *status=NULL,deleteflag=FALSE;

	if(STATUSES!=NULL) freeapplib(STATUSES);
	STATUSES=APPlibNEW();
	ZERNRD(perstat);
	ef=ADVGTENRDsec(perstat,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		FINDFLDGETCHAR(perstat,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(perstat,"STATUS TITLE",&status);
			addAPPlib(STATUSES,status);
			if(status!=NULL) Rfree(status);
		}
		ef=ADVNXTNRDsec(perstat,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(status!=NULL) Rfree(status);
	if(STATUSES->numlibs<1)
	{
		addAPPlib(STATUSES,"No Statuses Available");
	}
}
static void changestatus(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	int selected=0;

	ZERNRD(perstat);
	FINDRSCGETINT(mtnrsrc,"STATUS TITLES",&selected);
	if(RDAstrcmp(STATUSES->libs[selected],"No Statuses Available"))
	{
		FINDFLDSETSTRING(perstat,"STATUS TITLE",
			STATUSES->libs[selected]);
		FINDFLDSETSTRING(dbsort->fileno,"STATUS TITLE",
			STATUSES->libs[selected]);
	}
	if(ADVEQLNRDsec(perstat,1,SCRNvirtualSubData,mtnrsrc))
		KEYNRD(perstat,1);
	filerecord2rsrc(perstat,mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void dopaygrouplist(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	short ef=FALSE;
	char *paygroup=NULL,deleteflag=FALSE,active=FALSE;

	if(PAYGROUPS!=NULL) freeapplib(PAYGROUPS);
	PAYGROUPS=APPlibNEW();
	ZERNRD(paygrp);
	ef=ADVGTENRDsec(paygrp,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		FINDFLDGETCHAR(paygrp,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(paygrp,"ACTIVE",&active);
		if(!deleteflag && active)
		{
			FINDFLDGETSTRING(paygrp,"PAY GROUP",&paygroup);
			addAPPlib(PAYGROUPS,paygroup);
			if(paygroup!=NULL) Rfree(paygroup);
		}
		ef=ADVNXTNRDsec(paygrp,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(paygroup!=NULL) Rfree(paygroup);
	if(PAYGROUPS->numlibs<1)
	{
		addAPPlib(PAYGROUPS,"No Pay Group Available");
	}
}
static void changepaygroup(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	int selected=0;

	ZERNRD(paygrp);
	FINDRSCGETINT(mtnrsrc,"PAY GROUPS",&selected);
	if(RDAstrcmp(PAYGROUPS->libs[selected],"No Pay Groups Available"))
	{
		FINDFLDSETSTRING(paygrp,"PAY GROUP",
			PAYGROUPS->libs[selected]);
		FINDFLDSETSTRING(dbsort->fileno,"PAY GROUP",
			PAYGROUPS->libs[selected]);
	}
	if(ADVEQLNRDsec(paygrp,1,SCRNvirtualSubData,mtnrsrc))
		KEYNRD(paygrp,1);
	filerecord2rsrc(paygrp,mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void SETID(RDArmem *member,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;

	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			RMEMSET_KEYFUNC(member,MTNMASTER,PSTRUCT->previous,SCRNvirtualSubData,(void *)PSTRUCT,MTNMASTER->inheritrsrcs);
		}
	}
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
static void hirem(void *targetkey,DBsort *t)
{
	mtnstruct *mtn=NULL;
	int x=0,y=0,start=0,startstatus=0,startpaygroup=0;
	RDArsrc *mtnrsrc=NULL;
	short nofields=0,nokeys=0;
	NRDkey *keyx=NULL,*keys=NULL;
	NRDpart *part=NULL;
	NRDfield *fldx=NULL,*fields=NULL;
	/*MTNPassableStruct *PSTRUCT=NULL;*/
	/*MaintainMaster *MTNMASTER=NULL;*/
	char *vac=NULL,*status=NULL,*paygroup=NULL,*name=NULL;

	mtnrsrc=RDArsrcNEW(module,
		"HIRE APPLICANTS MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"APPCAT",NULL,appcat);
	addDFincvir(mtnrsrc,module,"APPCNT",NULL,appcnt);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(mtnrsrc,"POSTRK","POSVAC",NULL,posvac);
	addDFincvir(mtnrsrc,"PRSNNL","PERSTAT",NULL,perstat);
	addDFincvir(mtnrsrc,"PAYROLL","PAYGRP",NULL,paygrp);
	addDFincvir(mtnrsrc,module,"APPPOS",NULL,apppos);
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	nokeys=NUMKEYS(t->fileno);
	keys=KEYPOINTER(t->fileno);

	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(perdmg);
	/*PSTRUCT=Rmalloc(sizeof(MTNPassableStruct));*/
	/*PSTRUCT->MTNMASTER=MTNMASTER;*/
	/*PSTRUCT->quitfunc=((MTNPassableStruct *) t)->quitfunc;
	PSTRUCT->fromrsrc=((MTNPassableStruct *) t)->fromrsrc;
	PSTRUCT->fromargs=((MTNPassableStruct *) t)->fromargs;
	PSTRUCT->args=(void *) t;*/

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
			} else if(!RDAstrcmp(fldx->name,"STATUS TITLE"))
			{
				STATUSES=APPlibNEW();
				addlstrsrc(mtnrsrc,"STATUS TITLES",
					&startstatus,TRUE,changestatus,STATUSES->numlibs,
					&STATUSES->libs,t);
			} else if(!RDAstrcmp(fldx->name,"PAY GROUP"))
			{
				PAYGROUPS=APPlibNEW();
				addlstrsrc(mtnrsrc,"PAY GROUPS",
					&startpaygroup,TRUE,changepaygroup,PAYGROUPS->numlibs,
					&PAYGROUPS->libs,t);
			} else {
				ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,TRUE,TRUE);
			}
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
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(part->name)+1);
					} else {
						name=Rmalloc(RDAstrlen(part->name)+1);
					}
					sprintf(name,"%s",part->name);
					ADVFINDRSCSETFUNC(mtnrsrc,name,SETID,t,TRUE);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	if(appcat!=(-1)) file2rsrc(appcat,mtnrsrc,FALSE);
	if(appcnt!=(-1)) file2rsrc(appcnt,mtnrsrc,FALSE);
	if(posmst!=(-1)) file2rsrc(posmst,mtnrsrc,FALSE);
	if(poslnum!=(-1)) file2rsrc(poslnum,mtnrsrc,FALSE);
	if(apppos!=(-1)) file2rsrc(apppos,mtnrsrc,FALSE);
	if(posvac!=(-1)) file2rsrc(posvac,mtnrsrc,FALSE);
	if(perstat!=(-1)) file2rsrc(perstat,mtnrsrc,FALSE);
	if(paygrp!=(-1)) file2rsrc(paygrp,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"APPLICANT IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"APPLICATION NUMBER",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"POSITION IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"LOCATION IDENTIFICATION",updatelocation,t);
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
	dostatuslist(mtnrsrc,t);
	FINDFLDGETSTRING(t->fileno,"STATUS TITLE",&status);
	for(x=0;x<STATUSES->numlibs;++x)
	{
		if(!RDAstrcmp(STATUSES->libs[x],status)) break;
	}
	if(x<STATUSES->numlibs) startstatus=x;
		else startstatus=0;
	dopaygrouplist(mtnrsrc,t);
	FINDFLDGETSTRING(t->fileno,"PAY GROUP",&paygroup);
	for(x=0;x<PAYGROUPS->numlibs;++x)
	{
		if(!RDAstrcmp(PAYGROUPS->libs[x],paygroup)) break;
	}
	if(x<PAYGROUPS->numlibs) startpaygroup=x;
		else startpaygroup=0;
	getsupportingfiles(mtnrsrc,t);
	FINDRSCLISTAPPlib(mtnrsrc,"VACANCY IDENTIFICATIONS",start,VACANCIES);
	FINDRSCLISTAPPlib(mtnrsrc,"STATUS TITLES",startstatus,STATUSES);
	FINDRSCLISTAPPlib(mtnrsrc,"PAY GROUPS",startpaygroup,PAYGROUPS);
	addbtnrsrc(mtnrsrc,"BROWSE LOCATIONS",TRUE,browse_locations,NULL);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,t);
	APPmakescrn(mtnrsrc,TRUE,quit_record,t,FALSE);
	changevacancy(mtnrsrc,t);
	changestatus(mtnrsrc,t);
	changepaygroup(mtnrsrc,t);
}
static void copyperdmg(RDArsrc *tmprsrc,char *perid,DBsort *dbsort)
{
	NRDfield *fields=NULL,*fldx=NULL,*field=NULL;
	char *posid=NULL,*xclass=NULL,*curdate=NULL,*statustitle=NULL,*state=NULL;
	short nofields=0;
	RDATData *prev=NULL;
	int x=0;

	nofields=NUMFLDS(appcnt);
	fields=FLDPOINTER(appcnt);
	if(fields!=NULL)
	{
		FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",perid);
		if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,tmprsrc))
		{
			KEYNRD(perdmg,1);
		} else prev=RDATDataNEW(perdmg);
		FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",perid);
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			field=FLDNRD(perdmg,fldx->name);
			if(field!=NULL)
			{
				NRD2NRD(fldx,field);
			}
		}
		FINDFLDGETSTRING(apppos,"POSITION IDENTIFICATION",&posid);
		FINDFLDSETSTRING(posmst,"POSITION IDENTIFICATION",posid);
		FINDFLDGETSTRING(dbsort->fileno,"STATUS TITLE",&statustitle);
		FINDFLDSETSTRING(perdmg,"STATUS TITLE",statustitle);
		FINDFLDGETSTRING(appcnt,"STATE",&state);
		if(!RDAstrcmp(state,"VA")) 
			FINDFLDSETSTRING(perdmg,"STATE FILING STATUS","UNIFIED");
		else
			FINDFLDSETSTRING(perdmg,"STATE FILING STATUS","");
		if(!EQLNRDsec(posmst,1))
		{
			FINDFLDGETSTRING(posmst,"CLASS IDENTIFICATION",&xclass);
			FINDFLDSETSTRING(perdmg,"PERSONNEL CLASS",xclass);
			COPYFIELD(dbsort->fileno,perdmg,"LOCATION IDENTIFICATION");
		}
		else
		{
			prterr("Warning:  The position identification %s does not exist in the POSMSTR file.", posid);
		}
		curdate=GETCURRENTDATE10();
		FINDFLDSETCHAR(perdmg,"CURRENT EMPLOYEE",TRUE);
		COPYFIELD(dbsort->fileno,perdmg,"HIRE DATE");
		COPYFIELD(appcnt,perdmg,"MILITARY YEARS");
		COPYFIELD(appcnt,perdmg,"YEARS IN DIST");
		COPYFIELD(appcnt,perdmg,"YEARS IN STATE");
		COPYFIELD(appcnt,perdmg,"SUPPORT YEARS");
		FIELDCOPY(dbsort->fileno,"HIRE DATE",perdmg,"ADJUSTED DOE");
		ADVWRTTRANSsec(perdmg,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
		if(prev!=NULL) FreeRDATData(prev);
	}
	if(curdate!=NULL) Rfree(curdate);
}
static void copypayjmst(RDArsrc *tmprsrc,char *perid,char *curdate,char *curtime,DBsort *dbsort)
{
	char active=TRUE,*posid=NULL;
	RDATData *prev=NULL;
	double fteamt=0;

	ZERNRD(payjmst);
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
	/*
	FINDFLDSETSTRING(payjmst,"START DATE",hiredate);
	*/
	FINDFLDGETDOUBLE(posvac,"FTE AMOUNT",&fteamt);
	FINDFLDSETDOUBLE(payjmst,"FTE AMOUNT",fteamt);
	FINDFLDGETSTRING(apppos,"POSITION IDENTIFICATION",&posid);
	FINDFLDSETSTRING(payjmst,"POSITION IDENTIFICATION",posid);
	COPYFIELD(posmst,payjmst,"GROSS IDENTIFICATION");
	COPYFIELD(posmst,payjmst,"SUBSTITUTE WHEN ABSENT");
	COPYFIELD(posmst,payjmst,"LEAVE ALLOWED");
	FINDFLDSETCHAR(payjmst,"ACTIVE",active);
	FINDFLDSETINT(payjmst,"STEP",step);
	COPYFIELD(dbsort->fileno,payjmst,"LOCATION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjmst,"PAY GROUP");
	COPYFIELD(dbsort->fileno,payjmst,"START DATE");
	COPYFIELD(dbsort->fileno,payjmst,"STEP");
	COPYFIELD(dbsort->fileno,payjmst,"STEP ALPHA");
	COPYFIELD(dbsort->fileno,payjmst,"GRADE");
	COPYFIELD(posmst,payjmst,"DESCRIPTION");
	ADVWRTTRANSsec(payjmst,1,"JOB NUMBER",NULL,SCRNvirtualSubData,tmprsrc);
	ZERNRD(posper);
	COPYFIELD(payjmst,posper,"PERSONNEL IDENTIFICATION");
	COPYFIELD(payjmst,posper,"JOB NUMBER");
	FINDFLDSETSTRING(posper,"POSITION IDENTIFICATION",posid);
	if(!ADVEQLNRDsec(posper,1,SCRNvirtualSubData,tmprsrc))
	{
		prev=RDATDataNEW(posper);
		active=FALSE;
		FINDFLDSETCHAR(posper,"DELETEFLAG",active);
	}
	ADVWRTTRANSsec(posper,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
	if(prev!=NULL) FreeRDATData(prev);
	prev=RDATDataNEW(posvac);
	active=TRUE;
	FINDFLDSETCHAR(posvac,"FILLED",active);
	FINDFLDSETSTRING(posvac,"FILLED BY",USERLOGIN);
	FINDFLDSETSTRING(posvac,"FILLED DATE",curdate);
	FINDFLDSETSTRING(posvac,"FILLED TIME",curtime);
	ADVWRTTRANSsec(posvac,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
	if(prev!=NULL) FreeRDATData(prev);
	ADVupdatetotfte(posmst,posid,-fteamt,SCRNvirtualSubData,tmprsrc);
	ADVupdateftes(posmst,posid,fteamt,SCRNvirtualSubData,tmprsrc);
	if(posid!=NULL) Rfree(posid);
}
static void copypaydedm(RDArsrc *tmprsrc,char *perid)
{
	RDATData *prev=NULL;
	short ef=FALSE;
	char deleteflag=FALSE,*dedclas=NULL,*dedclas1=NULL,*dedid=NULL;
	char active=TRUE;

	ZERNRD(posdedc);
	ZERNRD(posdded);
	FINDFLDGETSTRING(posmst,"DEDUCTION CLASS IDENTIFICAITON",&dedclas);
	FINDFLDSETSTRING(posdedc,"DEDUCTION CLASS IDENTIFICATION",dedclas);
	FINDFLDSETSTRING(posdded,"DEDUCTION CLASS IDENTIFICATION",dedclas);
	if(ADVEQLNRDsec(posdedc,1,SCRNvirtualSubData,tmprsrc)) 
		KEYNRD(posdedc,1);
	ef=ADVGTENRDsec(posdded,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(posdded,"DEDUCTION CLASS IDENTIFICATION",&dedclas1);
		if(RDAstrcmp(dedclas,dedclas1)) break;
		FINDFLDGETCHAR(posdded,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDGETSTRING(posdded,"DEDUCTION DESCRIPTION IDENTIFICATION",
				&dedid);
			FINDFLDSETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",
				dedid);
			LOCNRDFILE(paydedm);
			if(!ADVEQLNRDsec(paydedm,1,SCRNvirtualSubData,tmprsrc))
			{
				prev=RDATDataNEW(paydedm);
			}
			FINDFLDSETCHAR(paydedm,"ACTIVE",active);
			ADVWRTTRANSsec(paydedm,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
			UNLNRDFILE(paydedm);
		}
		ef=ADVNXTNRDsec(posdded,1,SCRNvirtualSubData,tmprsrc);
	}
	if(dedclas!=NULL) Rfree(dedclas);
	if(dedclas1!=NULL) Rfree(dedclas1);
	if(dedid!=NULL) Rfree(dedid);
}
static void copyperdoc(RDArsrc *tmprsrc,char *perid)
{
	RDATData *prev=NULL;
	short ef=FALSE;
	char deleteflag=FALSE,*lperid=NULL,*appid=NULL,*lappid=NULL;
	NRDfield *fields=NULL,*fldx=NULL,*field=NULL;
	short nofields=0;
	int x=0;

	ZERNRD(perdoc);
	FINDFLDSETSTRING(perdoc,"PERSONNEL IDENTIFICATION",perid);
	LOCNRDFILE(perdoc);
	ef=ADVGTENRDsec(perdoc,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(perdoc,"PERSONNEL IDENTIFICATION",&lperid);
		if(RDAstrcmp(perid,lperid)) break;
		FINDFLDGETCHAR(perdoc,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			prev=RDATDataNEW(perdoc);
			deleteflag=TRUE;
			FINDFLDSETCHAR(perdoc,"DELETEFLAG",deleteflag);
			ADVWRTTRANSsec(perdoc,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(perdoc,1,SCRNvirtualSubData,tmprsrc);
	}
	if(lperid!=NULL) Rfree(lperid);
	UNLNRDFILE(perdoc);
	nofields=NUMFLDS(appdoc);
	fields=FLDPOINTER(appdoc);
	FINDFLDGETSTRING(apppos,"APPLICANT IDENTIFICATION",&appid);
	FINDFLDSETSTRING(appdoc,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appdoc,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(appdoc,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appdoc,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDSETSTRING(perdoc,"PERSONNEL IDENTIFICATION",perid);
			COPYFIELD(appdoc,perdoc,"DOCUMENT NUMBER");
			if(ADVEQLNRDsec(perdoc,1,SCRNvirtualSubData,tmprsrc))
				KEYNRD(perdoc,1);
			if(fields!=NULL)
			{
				for(x=0,fldx=fields;x<nofields;++x,++fldx)
				{
					field=FLDNRD(perdoc,fldx->name);
					if(field!=NULL)
					{
						NRD2NRD(fldx,field);
					}
				}
			}
			ADVWRTTRANSsec(perdoc,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(appdoc,1,SCRNvirtualSubData,tmprsrc);
	}
	if(appid!=NULL) Rfree(appid);
	if(lappid!=NULL) Rfree(lappid);
}
static void copypercrt(RDArsrc *tmprsrc,char *perid)
{
	RDATData *prev=NULL;
	short ef=FALSE;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	FINDFLDGETSTRING(apppos,"APPLICANT IDENTIFICATION",&appid);
	ZERNRD(appcrt);
	FINDFLDSETSTRING(appcrt,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appcrt,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(appcrt,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appcrt,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDSETSTRING(percrt,"PERSONNEL IDENTIFICATION",perid);
			COPYFIELD(appcrt,percrt,"CRITERIA IDENTIFICATION");
			if(ADVEQLNRDsec(percrt,1,SCRNvirtualSubData,tmprsrc)) 
			{
				KEYNRD(percrt,1);
				prev=NULL;
			} else {
				prev=RDATDataNEW(percrt);
			}
			FINDFLDSETCHAR(percrt,"DELETEFLAG",deleteflag);
			ADVWRTTRANSsec(percrt,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(appcrt,1,SCRNvirtualSubData,tmprsrc);
	}
	if(appid!=NULL) Rfree(appid);
	if(lappid!=NULL) Rfree(lappid);
}
static void copypercert(RDArsrc *tmprsrc,char *perid)
{
	RDATData *prev=NULL;
	short ef=FALSE;
	char deleteflag=FALSE,*lperid=NULL,*appid=NULL,*lappid=NULL;
	NRDfield *fields=NULL,*fldx=NULL,*field=NULL;
	short nofields=0;
	int x=0;

	ZERNRD(percert);
	FINDFLDSETSTRING(percert,"PERSONNEL IDENTIFICATION",perid);
	LOCNRDFILE(percert);
	ef=ADVGTENRDsec(percert,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(percert,"PERSONNEL IDENTIFICATION",&lperid);
		if(RDAstrcmp(perid,lperid)) break;
		FINDFLDGETCHAR(percert,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			prev=RDATDataNEW(percert);
			deleteflag=TRUE;
			FINDFLDSETCHAR(percert,"DELETEFLAG",deleteflag);
			ADVWRTTRANSsec(percert,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(percert,1,SCRNvirtualSubData,tmprsrc);
	}
	if(lperid!=NULL) Rfree(lperid);
	UNLNRDFILE(percert);
	nofields=NUMFLDS(appcert);
	fields=FLDPOINTER(appcert);
	FINDFLDGETSTRING(apppos,"APPLICANT IDENTIFICATION",&appid);
	FINDFLDSETSTRING(appcert,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appcert,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(appcert,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appcert,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDSETSTRING(percert,"PERSONNEL IDENTIFICATION",perid);
			COPYFIELD(appdoc,percert,"CERTIFICATE NUMBER");
			if(ADVEQLNRDsec(percert,1,SCRNvirtualSubData,tmprsrc))
				KEYNRD(percert,1);
			if(fields!=NULL)
			{
				for(x=0,fldx=fields;x<nofields;++x,++fldx)
				{
					field=FLDNRD(percert,fldx->name);
					if(field!=NULL)
					{
						NRD2NRD(fldx,field);
					}
				}
			}
			ADVWRTTRANSsec(percert,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(appcert,1,SCRNvirtualSubData,tmprsrc);
	}
	if(appid!=NULL) Rfree(appid);
	if(lappid!=NULL) Rfree(lappid);
}
static void copyperend(RDArsrc *tmprsrc,char *perid)
{
	RDATData *prev=NULL;
	short ef=FALSE;
	char deleteflag=FALSE,*lperid=NULL,*appid=NULL,*lappid=NULL;
	NRDfield *fields=NULL,*fldx=NULL,*field=NULL;
	short nofields=0;
	int x=0;

	ZERNRD(perend);
	FINDFLDSETSTRING(perend,"PERSONNEL IDENTIFICATION",perid);
	LOCNRDFILE(perend);
	ef=ADVGTENRDsec(perend,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(perend,"PERSONNEL IDENTIFICATION",&lperid);
		if(RDAstrcmp(perid,lperid)) break;
		FINDFLDGETCHAR(perend,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			prev=RDATDataNEW(perend);
			deleteflag=TRUE;
			FINDFLDSETCHAR(perend,"DELETEFLAG",deleteflag);
			ADVWRTTRANSsec(perend,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(perend,1,SCRNvirtualSubData,tmprsrc);
	}
	if(lperid!=NULL) Rfree(lperid);
	UNLNRDFILE(perend);
	nofields=NUMFLDS(append);
	fields=FLDPOINTER(append);
	FINDFLDGETSTRING(apppos,"APPLICANT IDENTIFICATION",&appid);
	FINDFLDSETSTRING(append,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(append,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(append,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(append,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDSETSTRING(perend,"PERSONNEL IDENTIFICATION",perid);
			COPYFIELD(append,perend,"CERTIFICATE NUMBER");
			COPYFIELD(append,perend,"ENDORSEMENT NUMBER");
			if(ADVEQLNRDsec(perend,1,SCRNvirtualSubData,tmprsrc))
				KEYNRD(perend,1);
			if(fields!=NULL)
			{
				for(x=0,fldx=fields;x<nofields;++x,++fldx)
				{
					field=FLDNRD(perend,fldx->name);
					if(field!=NULL)
					{
						NRD2NRD(fldx,field);
					}
				}
			}
			ADVWRTTRANSsec(perend,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(append,1,SCRNvirtualSubData,tmprsrc);
	}
	if(appid!=NULL) Rfree(appid);
	if(lappid!=NULL) Rfree(lappid);
}
static void copyperedm(RDArsrc *tmprsrc,char *perid)
{
	RDATData *prev=NULL;
	short ef=FALSE;
	char deleteflag=FALSE,*lperid=NULL,*appid=NULL,*lappid=NULL;
	NRDfield *fields=NULL,*fldx=NULL,*field=NULL;
	short nofields=0;
	int x=0;

	ZERNRD(peredm);
	FINDFLDSETSTRING(peredm,"PERSONNEL IDENTIFICATION",perid);
	LOCNRDFILE(peredm);
	ef=ADVGTENRDsec(peredm,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(peredm,"PERSONNEL IDENTIFICATION",&lperid);
		if(RDAstrcmp(perid,lperid)) break;
		FINDFLDGETCHAR(peredm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			prev=RDATDataNEW(peredm);
			deleteflag=TRUE;
			FINDFLDSETCHAR(peredm,"DELETEFLAG",deleteflag);
			ADVWRTTRANSsec(peredm,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(peredm,1,SCRNvirtualSubData,tmprsrc);
	}
	if(lperid!=NULL) Rfree(lperid);
	UNLNRDFILE(peredm);
	nofields=NUMFLDS(appedm);
	fields=FLDPOINTER(appedm);
	FINDFLDGETSTRING(apppos,"APPLICANT IDENTIFICATION",&appid);
	FINDFLDSETSTRING(appedm,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appedm,1,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(appedm,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appedm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDSETSTRING(peredm,"PERSONNEL IDENTIFICATION",perid);
			COPYFIELD(appdoc,peredm,"EDUCATION NUMBER");
			if(ADVEQLNRDsec(peredm,1,SCRNvirtualSubData,tmprsrc))
				KEYNRD(peredm,1);
			if(fields!=NULL)
			{
				for(x=0,fldx=fields;x<nofields;++x,++fldx)
				{
					field=FLDNRD(peredm,fldx->name);
					if(field!=NULL)
					{
						NRD2NRD(fldx,field);
					}
				}
			}
			ADVWRTTRANSsec(peredm,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
			if(prev!=NULL) FreeRDATData(prev);
		}
		ef=ADVNXTNRDsec(appedm,1,SCRNvirtualSubData,tmprsrc);
	}
	if(appid!=NULL) Rfree(appid);
	if(lappid!=NULL) Rfree(lappid);
}
static void hire_applicant(RDArsrc *tmprsrc,DBsort *dbsort)
{
	char *perid=NULL,*curdate=NULL,*curtime=NULL;
	RDATData *prev=NULL;
	DFvirtual *d=NULL;
	RDAvirtual *EmployeeName=NULL;

	if(!isEMPTY(peridform))
	{
		perid=EVALUATEstr(peridform,SCRNvirtualSubData,tmprsrc);
	}
	if(isEMPTY(perid))
	{
		switch(personnel_type)
		{
			case 0: /* Applicant ID */
			FINDFLDGETSTRING(appcnt,"APPLICANT IDENTIFICATION",&perid);
			break;

			case 1: /* SSN */
			FINDFLDGETSTRING(appcnt,"SOCIAL SECURITY NUMBER",&perid);
			break;

			case 2: /* Applicant Name (LAST LINEAGE, FIRST MIDDLE)*/
			d=getDFvirtual(module,"NAME LAST LINEAGE FIRST MIDDLE");
			if(d!=NULL)
			{
				EmployeeName=Rmalloc(sizeof(RDAvirtual));
				EmployeeName->name=stralloc(d->name);
				EmployeeName->when=0;
				EmployeeName->len=d->length;
				EmployeeName->type=d->type;
				if(d->expression!=NULL) 
				{
					EmployeeName->expression=PP_translate(d->expression);
				} else {
					EmployeeName->expression=NULL;
				}
				EmployeeName->computed=FALSE;
				EmployeeName->range=FALSE;
				EmployeeName->dlen=0;
				switch(EmployeeName->type)
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
						EmployeeName->value.string_value=NULL;
						EmployeeName->dlen=0;
						break;
					case BOOLNS:
					case CHARACTERS:
						EmployeeName->value.string_value=Rmalloc(1);
						*EmployeeName->value.string_value=0;
						EmployeeName->dlen=1;
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case SDECIMALV:
						EmployeeName->value.float_value=Rmalloc(sizeof(double));
						*EmployeeName->value.float_value=0;
						EmployeeName->dlen=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						EmployeeName->value.short_value=Rmalloc(sizeof(short));
						*EmployeeName->value.short_value=0;
						EmployeeName->dlen=sizeof(short);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						EmployeeName->value.integer_value=Rmalloc(sizeof(int));
						*EmployeeName->value.integer_value=0;
						EmployeeName->dlen=sizeof(int);
						break;
					default:
						prterr("Error virtual field type [%d] not found for field [%s].",EmployeeName->type,EmployeeName->name);
						break;
				}
				FreeDFvirtual(d);
			}
			if(EmployeeName!=NULL)
			{
				EmployeeName->computed=FALSE;
				computevirtual(EmployeeName,SCRNvirtualSubData,tmprsrc);
				perid=Rmalloc(15+1);
				sprintf(perid,"%.15s",EmployeeName->value.string_value);
			}
			break;

			case 3: /* Applicant Name (LAST LINEAGE FIRST MIDDLE)*/
			d=getDFvirtual(module,"NAME LAST LINEAGE FIRST MIDDLE W/O COMMAS");
			if(d!=NULL)
			{
				EmployeeName=Rmalloc(sizeof(RDAvirtual));
				EmployeeName->name=stralloc(d->name);
				EmployeeName->when=0;
				EmployeeName->len=d->length;
				EmployeeName->type=d->type;
				if(d->expression!=NULL) 
				{
					EmployeeName->expression=PP_translate(d->expression);
				} else {
					EmployeeName->expression=NULL;
				}
				EmployeeName->computed=FALSE;
				EmployeeName->range=FALSE;
				EmployeeName->dlen=0;
				switch(EmployeeName->type)
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
						EmployeeName->value.string_value=NULL;
						EmployeeName->dlen=0;
						break;
					case BOOLNS:
					case CHARACTERS:
						EmployeeName->value.string_value=Rmalloc(1);
						*EmployeeName->value.string_value=0;
						EmployeeName->dlen=1;
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case SDECIMALV:
						EmployeeName->value.float_value=Rmalloc(sizeof(double));
						*EmployeeName->value.float_value=0;
						EmployeeName->dlen=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						EmployeeName->value.short_value=Rmalloc(sizeof(short));
						*EmployeeName->value.short_value=0;
						EmployeeName->dlen=sizeof(short);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						EmployeeName->value.integer_value=Rmalloc(sizeof(int));
						*EmployeeName->value.integer_value=0;
						EmployeeName->dlen=sizeof(int);
						break;
					default:
						prterr("Error virtual field type [%d] not found for field [%s].",EmployeeName->type,EmployeeName->name);
						break;
				}
				FreeDFvirtual(d);
			}
			if(EmployeeName!=NULL)
			{
				EmployeeName->computed=FALSE;
				computevirtual(EmployeeName,SCRNvirtualSubData,tmprsrc);
				perid=Rmalloc(15+1);
				sprintf(perid,"%.15s",EmployeeName->value.string_value);
			}
			break;

			default:
			prterr("Error:  Unrecognized value of %d for Personnel ID Type.",personnel_type);
		}
	}
	LOCNRDFILE(apppos);
	if(!ADVEQLNRDsec(apppos,1,SCRNvirtualSubData,tmprsrc))
	{
		prev=RDATDataNEW(apppos);
	}
	curdate=GETCURRENTDATE10();
	curtime=GETCURRENTTIME();
	FINDFLDSETSTRING(apppos,"HIRED BY",USERLOGIN);
	FINDFLDSETSTRING(apppos,"HIRED DATE",curdate);
	FINDFLDSETSTRING(apppos,"HIRED TIME",curtime);
	FINDFLDSETCHAR(apppos,"HIRED",TRUE);
	ADVWRTTRANSsec(apppos,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
	if(prev!=NULL) FreeRDATData(prev);
	UNLNRDFILE(apppos);
	DID_STUFF=FALSE;
	copyperdmg(tmprsrc,perid,dbsort);
	copypayjmst(tmprsrc,perid,curdate,curtime,dbsort);
	copypaydedm(tmprsrc,perid);
	if(GetDocuments) copyperdoc(tmprsrc,perid);
	if(GetCriteria) copypercrt(tmprsrc,perid);
	if(GetCertifications) copypercert(tmprsrc,perid);
	if(GetEndorsements) copyperend(tmprsrc,perid);
	if(GetEducation) copyperedm(tmprsrc,perid);
	if(curdate!=NULL) Rfree(curdate);
	if(curtime!=NULL) Rfree(curtime);
	if(perid!=NULL) Rfree(perid);
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
	d=getDFvirtual("APPMGT","SUPERVISOR INTERVIEW SORT SEQUENCE");
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
	addDBfield(dbsrt,"STATUS TITLE",PLAINTEXT,15);
	addDBfield(dbsrt,"PAY GROUP",PLAINTEXT,15);
	addDBfield(dbsrt,"LOCATION IDENTIFICATION",PLAINTEXT,15);
	if(peridform!=NULL)
	{
		addDBfield(dbsrt,"PERSONNEL IDENTIFICATION EXPRESSION",SCROLLEDTEXT,RDAstrlen(peridform));
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
	addDBfilefield(dbsrt,appcnt,"NAME");
	addDBfilefield(dbsrt,apppos,"SUPERVISOR INTERVIEWED");
	addDBfilefield(dbsrt,apppos,"RECOMMEND TO HIRE");
	addDBfilefield(dbsrt,apppos,"APPROVED TO HIRE");
	addDBfilefield(dbsrt,appcat,"WITHDRAW APPLICANT");
	addDBfilefield(dbsrt,posvac,"VACANCY IDENTIFICATION");
	addDBfilefield(dbsrt,perdmg,"HIRE DATE");
	addDBfilefield(dbsrt,payjmst,"START DATE");
	addDBfilefield(dbsrt,payjmst,"STEP");
	addDBfilefield(dbsrt,payjmst,"STEP ALPHA");
	addDBfilefield(dbsrt,payjmst,"GRADE");
}
static void addsortrecord(RDArsrc *mainrsrc, DBsort *dbsrt)
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
	FIELDCOPY(apppos,"SUPERVISOR APPOINTMENT DATE",
		dbsrt->fileno,"APPOINTMENT DATE");
	FIELDCOPY(apppos,"SUPERVISOR APPOINTMENT TIME",
		dbsrt->fileno,"APPOINTMENT TIME");
	COPYFIELD(apppos,dbsrt->fileno,"SUPERVISOR INTERVIEWED");
	COPYFIELD(apppos,dbsrt->fileno,"RECOMMEND TO HIRE");
	COPYFIELD(apppos,dbsrt->fileno,"APPROVED TO HIRE");
	COPYFIELD(appcnt,dbsrt->fileno,"NAME");
	COPYFIELD(appcnt,dbsrt->fileno,"WORK PHONE");
	COPYFIELD(appcnt,dbsrt->fileno,"HOME PHONE");
	COPYFIELD(appcat,dbsrt->fileno,"WITHDRAW APPLICANT");
	COPYFIELD(apppos,dbsrt->fileno,"VACANCY IDENTIFICATION");
	COPYFIELD(perstat,dbsrt->fileno,"STATUS TITLE");
	COPYFIELD(posmst,dbsrt->fileno,"LOCATION IDENTIFICATION");
	FINDFLDSETSTRING(dbsrt->fileno,"PERSONNEL IDENTIFICATION EXPRESSION",peridform);
	/* Default to the first PAY GROUP if one is available. */
	dopaygrouplist(mainrsrc, dbsrt);
	if(PAYGROUPS!=NULL && PAYGROUPS->libs!=NULL)
	{
		FINDFLDSETSTRING(dbsrt->fileno,"PAY GROUP",PAYGROUPS->libs[0]);
	}
	/*
	FINDFLDSETSTRING(dbsrt->fileno,"HIRE DATE",CURRENT_DATE10);
	FINDFLDSETSTRING(dbsrt->fileno,"START DATE",hiredate);
	FINDFLDSETINT(dbsrt->fileno,"STEP",step);
	*/
	WRTNRD(dbsrt->fileno,0,NULL);
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	hirem(targetkey,blist->funcargs);
}
/* BROWSE LOCATION CODE STARTING */
static void doexit_browse_location(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_location(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_location(MakeBrowseList *BL,void *targetkey)
{
	char *locid=NULL; 
	/*int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;*/

	ZERNRD(poslnum);
	if(ADVEQLNRDKEYsec(poslnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(poslnum,1);
	} else {
		if(locid!=NULL) { Rfree(locid); locid=NULL; }
		FINDFLDGETSTRING(poslnum,"LOCATION IDENTIFICATION",&locid);
	}
	filerecord2rsrc(poslnum,BL->funcargs);
	/*FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][LOCATION IDENTIFICATION]",locid);*/
	/*FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"LOCATION IDENTIFICATION",locid);*/
	FINDRSCSETSTRING(BL->funcargs,"LOCATION IDENTIFICATION",locid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	/*y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);*/
	quit_browse_location(BL);
	if(locid!=NULL) Rfree(locid);
}
static short select_browse_location(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_location(MakeBrowseList *BL)
{
	ZERNRD(poslnum);
}
static short break_browse_location(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_locations(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_locmstr!=NULL)
	{
		mbl_locmstr->funcargs=(void *)mtnrsrc;
		mbl_locmstr->okfunction=okfunc_browse_location;
		if(APPmakescrn(mbl_locmstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_location(mbl_locmstr);
		}
	} else {
		mainrsrc=RDArsrcNEW("APPMGT","SELECT LOCATION BROWSE");
		searchrsrc=RDArsrcNEW("APPMGT","SELECT LOCATION SEARCH BROWSE");
		definelist=RDArsrcNEW("APPMGT","SELECT LOCATION DEFINE LIST");
		ZERNRD(poslnum);
		addDFincvir(searchrsrc,"POSTRK","POSLMST",NULL,poslnum);
		addDFincvir(definelist,"POSTRK","POSLMST",NULL,poslnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_locmstr=xbrowse(poslnum,1,mainrsrc,searchrsrc,definelist,init_browse_location,break_browse_location,select_browse_location,quit_browse_location,okfunc_browse_location,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE LOCATION CODE ENDING */
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without processing the Applicants?",NULL,doexit,FALSE,blist);
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
	if(WindowCount<=2) hire_backend(x);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static void selecthire(RDArsrc *mainrsrc)
{
	DBsort *dbsort=NULL;
	char *sortname=NULL,deleteflag=FALSE,sup2int=FALSE,hired=FALSE;
	char *exprssn=NULL,withdraw=FALSE,*e=NULL;
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	short ef=0,update_diag=FALSE;
	RangeScreen *rs=NULL;
	RDAvirtual *v=NULL;
	int x=0;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&exprssn);
	FINDRSCGETSTRING(mainrsrc,"PERSONNEL IDENTIFICATION EXPRESSION",
		&peridform);
	FINDRSCGETSTRING(mainrsrc,"HIRE DATE",&hiredate);
	FINDRSCGETCHAR(mainrsrc,"DOCUMENTS",&GetDocuments);
	FINDRSCGETCHAR(mainrsrc,"EDUCATION",&GetEducation);
	FINDRSCGETCHAR(mainrsrc,"CERTIFICATIONS",&GetCertifications);
	FINDRSCGETCHAR(mainrsrc,"ENDORSEMENTS",&GetEndorsements);
	FINDRSCGETCHAR(mainrsrc,"CRITERIA",&GetCriteria);
	sortname=unique_name();
	dbsort=DBsortNEW("RPTGEN",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(appcnt,rs);
	ReadRangeScreen(appcat,rs);
	ReadRangeScreen(apppos,rs);
	ReadRangeScreen(posmst,rs);
	ReadRangeScreen(posvac,rs);
	ReadRangeScreen(perstat,rs);
	ReadRangeScreen(paygrp,rs);
	ReadRangeScreen(poscls,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(apppos,"DIAGNOSTIC SCREEN",module,
		"Hiring Applicants",stop_diagnostic);
	addDFincvir(tmprsrc,module,"APPCNT",NULL,appcnt);
	addDFincvir(tmprsrc,module,"APPCAT",NULL,appcat);
	addDFincvir(tmprsrc,module,"APPPOS",NULL,apppos);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(tmprsrc,"POSTRK","POSLMST",NULL,poslnum);
	addDFincvir(tmprsrc,"POSTRK","POSPER",NULL,posper);
	addDFincvir(tmprsrc,"POSTRK","POSVAC",NULL,posvac);
	addDFincvir(tmprsrc,"PRSNNL","PERSTAT",NULL,perstat);
	addDFincvir(tmprsrc,"PAYROLL","PAYGRP",NULL,paygrp);
	addDFincvir(tmprsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(tmprsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(tmprsrc,"POSTRK","POSDDED",NULL,posdded);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(tmprsrc,"PRSNNL","PERCRT",NULL,percrt);
	addDFincvir(tmprsrc,"PRSNNL","PERCERT",NULL,percert);
	addDFincvir(tmprsrc,"PRSNNL","PEREND",NULL,perend);
	addDFincvir(tmprsrc,"PRSNNL","PEREDM",NULL,peredm);
	addDFincvir(tmprsrc,"PRSNNL","PERDOC",NULL,perdoc);
	addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,payjmst);
	addDFincvir(tmprsrc,"PAYROLL","PAYDEDM",NULL,paydedm);
	addDFincvir(tmprsrc,module,"APPCRT",NULL,appcrt);
	addDFincvir(tmprsrc,module,"APPCERT",NULL,appcert);
	addDFincvir(tmprsrc,module,"APPEND",NULL,append);
	addDFincvir(tmprsrc,module,"APPDOC",NULL,appdoc);
	addDFincvir(tmprsrc,module,"APPEDM",NULL,appedm);
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
		if(abort_diagnostic) break;
		CLEAR_SCREEN_VIRTUAL(tmprsrc);
		update_diag=FALSE;
		FINDFLDGETCHAR(apppos,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(apppos,"HIRED",&hired);
		FINDFLDGETCHAR(apppos,"APPROVED TO HIRE",&sup2int);
		if(!deleteflag && sup2int && !hired)
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
							FINDFLDGETCHAR(appcat,"WITHDRAW APPLICANT",
								&withdraw);
							if(!deleteflag && !withdraw)
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
											COPYFIELD(apppos,posvac,
												"POSITION IDENTIFICATION");
											COPYFIELD(apppos,posvac,
												"VACANCY IDENTIFICATION");
											if(!ADVEQLNRDsec(posvac,1,
												SCRNvirtualSubData,
												tmprsrc))
											{
												FINDFLDGETCHAR(posvac,
													"DELETEFLAG",
													&deleteflag);
												FINDFLDGETCHAR(posvac,
													"FILLED",&withdraw);
												if(!deleteflag && !withdraw &&
													!testfilerangersrc(posvac,rs))
												{
													for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
													{
														if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mainrsrc))
														{
															break;
														}
													}
													if(x>=mainrsrc->num)
													{
														if(!isEMPTY(exprssn))
														{
															if(EVALUATEbol(exprssn,
																SCRNvirtualSubData,
																tmprsrc))
															{
																/*hire_applicant(tmprsrc);*/
																addsortrecord(mainrsrc, dbsort);
																update_diag=TRUE;
															}
														} else {
															/*hire_applicant(tmprsrc);*/
															addsortrecord(mainrsrc, dbsort);
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
			"HIRE APPLICANTS BROWSE");
		srsrc=RDArsrcNEW(module,
			"HIRE APPLICANTS SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,
			"HIRE APPLICANTS DEFINE LIST");
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
			selectfunction,doexit_test,okfunc,NULL,dbsort,TRUE,NULL,
			0,0,0,TRUE);
		FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
	}
	else
	{
		prterr("Warning:  No records were selected using the given criteria.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO RECORDS SELECTED!","There were no records selected using the specified range criteria.  Please try again and/or call your installer.",NULL,NULL,TRUE,NULL);
	}
}
static void quithire(RDArsrc *mainrsrc)
{
	APPlib *args=NULL;

	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(DID_STUFF)
	{
		args=APPlibNEW();
		addAPPlib(args,"APPMGT");
		addAPPlib(args,"APPHIREAUDIT");
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(reportname!=NULL) Rfree(reportname);
	if(hiredate!=NULL) Rfree(hiredate);
	if(VACANCIES!=NULL) freeapplib(VACANCIES);
	if(STATUSES!=NULL) freeapplib(STATUSES);
	if(PAYGROUPS!=NULL) freeapplib(PAYGROUPS);
	closefiles();
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	ShutdownSubsystems();
}
static void updateselect(RDArsrc *mainrsrc)
{
	char delflag=FALSE;
	char *defstatustitle=NULL;

	/*readwidget(mainrsrc,"DEFAULT STATUS TITLE");
	FINDRSCGETSTRING(mainrsrc,"DEFAULT STATUS TITLE",&defstatustitle);*/
	/* If the default status title is blank, don't let the user continue. */
	/*if(defstatustitle == NULL)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
	}
	else
	{
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",TRUE);
	}*/

	readwidget(mainrsrc,"[PERSTAT][STATUS TITLE]");
	FINDRSCGETSTRING(mainrsrc,"[PERSTAT][STATUS TITLE]",&defstatustitle);
	FINDFLDSETSTRING(perstat,"STATUS TITLE",defstatustitle);
	if(ADVEQLNRDsec(perstat,1,SCRNvirtualSubData,mainrsrc))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(perstat,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(perstat);
		}
	} else {
		FINDFLDGETCHAR(perstat,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(perstat,1,SCRNvirtualSubData,mainrsrc))
			{
				ZERNRD(perstat);
			}
		}
	}
	updatefilerecord2rsrc(perstat,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	RDAGenericSetup *gsv=NULL;
	char *libx=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"HIRE APPLICANTS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((appcnt=APPOPNNRD(module,"APPCNT",TRUE,FALSE))==(-1)) return;
	if((appcat=APPOPNNRD(module,"APPCAT",TRUE,FALSE))==(-1)) return;
	if((apppos=APPOPNNRD(module,"APPPOS",TRUE,TRUE))==(-1)) return;
	if((posmst=APPOPNNRD("POSTRK","POSMSTR",TRUE,TRUE))==(-1)) return;
	if((poslnum=APPOPNNRD("POSTRK","POSLMST",TRUE,TRUE))==(-1)) return;
	if((posvac=APPOPNNRD("POSTRK","POSVAC",TRUE,TRUE))==(-1)) return;
	if((poscls=APPOPNNRD("POSTRK","POSCLS",TRUE,TRUE))==(-1)) return;
	if((posper=APPOPNNRD("POSTRK","POSPER",TRUE,TRUE))==(-1)) return;
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,TRUE))==(-1)) return;
	if((percrt=APPOPNNRD("PRSNNL","PERCRT",TRUE,TRUE))==(-1)) return;
	if((percert=APPOPNNRD("PRSNNL","PERCERT",TRUE,TRUE))==(-1)) return;
	if((perstat=APPOPNNRD("PRSNNL","PERSTAT",TRUE,TRUE))==(-1)) return;
	if((paygrp=APPOPNNRD("PAYROLL","PAYGRP",TRUE,TRUE))==(-1)) return;
	if((perend=APPOPNNRD("PRSNNL","PEREND",TRUE,TRUE))==(-1)) return;
	if((perdoc=APPOPNNRD("PRSNNL","PERDOC",TRUE,TRUE))==(-1)) return;
	if((peredm=APPOPNNRD("PRSNNL","PEREDM",TRUE,TRUE))==(-1)) return;
	if((payjmst=APPOPNNRD("PAYROLL","PAYJMST",TRUE,TRUE))==(-1)) return;
	if((paydedm=APPOPNNRD("PAYROLL","PAYDEDM",TRUE,TRUE))==(-1)) return;
	if((posdedc=APPOPNNRD("POSTRK","POSDEDC",TRUE,FALSE))==(-1)) return;
	if((posdded=APPOPNNRD("POSTRK","POSDDED",TRUE,FALSE))==(-1)) return;
	if((appcrt=APPOPNNRD(module,"APPCRT",TRUE,FALSE))==(-1)) return;
	if((appcert=APPOPNNRD(module,"APPCERT",TRUE,FALSE))==(-1)) return;
	if((append=APPOPNNRD(module,"APPEND",TRUE,FALSE))==(-1)) return;
	if((appdoc=APPOPNNRD(module,"APPDOC",TRUE,FALSE))==(-1)) return;
	if((appedm=APPOPNNRD(module,"APPEDM",TRUE,FALSE))==(-1)) return;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew(module,"PERSONNEL IDENTIFICATION TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			personnel_type=*gsv->value.short_value;
		} else {
			personnel_type=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);

	mainrsrc=RDArsrcNEW(module,"HIRE APPLICANTS");
	addDFincvir(mainrsrc,module,"APPCAT",NULL,appcat);
	addDFincvir(mainrsrc,module,"APPCNT",NULL,appcnt);
	addDFincvir(mainrsrc,module,"APPPOS",NULL,apppos);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(mainrsrc,"POSTRK","POSVAC",NULL,posvac);
	addDFincvir(mainrsrc,"PRSNNL","PERSTAT",NULL,perstat);
	addDFincvir(mainrsrc,"PAYROLL","PAYGRP",NULL,paygrp);
	addDFincvir(mainrsrc,"POSTRK","POSCLS",NULL,poscls);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	file2rangersrc(appcat,mainrsrc);
	file2rangersrc(appcnt,mainrsrc);
	file2rangersrc(apppos,mainrsrc);
	file2rangersrc(posmst,mainrsrc);
	file2rangersrc(posvac,mainrsrc);
	file2rangersrc(perstat,mainrsrc);
	file2rangersrc(paygrp,mainrsrc);
	file2rangersrc(poscls,mainrsrc);
	file2rsrc(perstat,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[PERSTAT][STATUS TITLE]",TRUE);
	filerecord2rsrc(perstat,mainrsrc);
	FINDRSCSETFUNC(mainrsrc,"[PERSTAT][STATUS TITLE]",updateselect,NULL);
	addstdrsrc(mainrsrc,"HIRE DATE",DATES,10,CURRENT_DATE10,TRUE);
	addsctrsrc(mainrsrc,"PERSONNEL IDENTIFICATION EXPRESSION",
		0,NULL,TRUE);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,
		"PERSONNEL IDENTIFICATION EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,
		"PERSONNEL IDENTIFICATION EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,
		"PERSONNEL IDENTIFICATION EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,
		"PERSONNEL IDENTIFICATION EXPRESSION");
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	addstdrsrc(mainrsrc,"DOCUMENTS",BOOLNS,1,&GetDocuments,TRUE);
	addstdrsrc(mainrsrc,"EDUCATION",BOOLNS,1,&GetEducation,TRUE);
	addstdrsrc(mainrsrc,"CERTIFICATIONS",BOOLNS,1,&GetCertifications,TRUE);
	addstdrsrc(mainrsrc,"ENDORSEMENTS",BOOLNS,1,&GetEndorsements,TRUE);
	addstdrsrc(mainrsrc,"CRITERIA",BOOLNS,1,&GetCriteria,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selecthire,NULL);
	updateselect(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quithire,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	APPmakescrn(mainrsrc,FALSE,quithire,NULL,TRUE);
	RDAAPPMAINLOOP();
}
