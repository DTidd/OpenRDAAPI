/* empaabw1.c - Add Employee Absentee Substitute Worked*/
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <rptgen.hpp>

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

extern short filenum,empabtr,empadef;
extern short dmgnum,payjmst,posmstr,poslmst;
extern short posgrs,poscls,posper,posdedc;
extern short possmst,posngms;
extern short subdmgnum,submstr,subjmstnum;
extern DBsort *dbsort;
extern RDArsrc *mainrsrc;
extern void getsupportingrecords(RDArsrc *,short,PassableStruct *);
extern char *module;
extern void EmpAbsSubData(char **,PassableStruct *);
static void doexit2(MakeBrowseList *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static short selectfunction(MakeBrowseList *);
static void DCTsetlist(RDArsrc *,PassableStruct *);
static APPlib *subjobnolist=NULL;
/*
static void check_peridcb(RDArsrc *,PassableStruct *p);
static short check_perid(RDArsrc *,PassableStruct *p,short);
*/
static void check_unitscb(RDArsrc *,PassableStruct *p);
static short check_units(RDArsrc *,PassableStruct *p,short);
static short check_subid(RDArsrc *,PassableStruct *,short);
static short check_subjobno(RDArsrc *,PassableStruct *,short);
static void changesub(RDArsrc *,PassableStruct *,short);
static void changesubjobno1(RDArsrc *,PassableStruct *);
static void changesubjobnocb1(RDArsrc *,PassableStruct *);
static void save_record_up(RDArsrc *,PassableStruct *);
static void save_record_nup(RDArsrc *,PassableStruct *);
static void save_check(RDArsrc *,PassableStruct *,short);
static void save_record(RDArsrc *,PassableStruct *,short);
static void quit_record(RDArsrc *,PassableStruct *);
static void getsupportingfiles(RDArsrc *,PassableStruct *,short);
static void check_subjobnocb(RDArsrc *,PassableStruct *);
static char *subpersonnel_identification;
static short sub_selectfunc(MakeBrowseList *);
static void browse_submstr(RDArsrc *,PassableStruct *);
static void resetdefs(RDArsrc *,PassableStruct *);
static void setfiles(RDArsrc *,PassableStruct *);
static void getfile(RDArsrc *,PassableStruct *);
static void getpreviouscb(RDArmem *,PassableStruct *);
static void getnextcb(RDArmem *,PassableStruct *);
static void setkeys(RDArsrc *);
void browse_empabtr(RDArsrc *,PassableStruct *);
MakeBrowseList *mbl_empabtr=NULL;

static void DCTsetlist(RDArsrc *parent,PassableStruct *p)
{
	int x,jobno=0;
	char *xstemp=NULL;

	FINDFLDGETINT(dbsort->fileno,"SUBSTITUTE JOB NUMBER",&jobno);
	xstemp=Rmalloc(10);
	sprintf(xstemp,"%8d",jobno);	
	if(subjobnolist!=NULL)
	{
		for(x=0;x<subjobnolist->numlibs;++x)
		{
			if(!strcmp(xstemp,subjobnolist->libs[x])) break;
		}
		if(x>=subjobnolist->numlibs) x=0;
		FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBERS",x);
		updatersrc(parent,"SUBSTITUTE JOB NUMBERS");
		FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBER",jobno);
		updatersrc(parent,"SUBSTITUTE JOB NUMBER");
	}
	if(xstemp!=NULL) Rfree(xstemp);
}
/*
static void check_peridcb(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(dbsort->fileno,parent);
	check_perid(parent,p,TRUE);
	updateSCRNvirtuals(parent);
}
static short check_perid(RDArsrc *parent,PassableStruct *p,short updatesupporting)
{
	char *perid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"PERSONNEL IDENTIFICATION",&perid1);
	if(dmgnum!=(-1))
	{
		ZERNRD(dmgnum);
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(ADVEQLNRDsec(dmgnum,1,EmpAbsSubData,p))
		{
			KEYNRD(dmgnum,1);
			warnmessage=Rmalloc(RDAstrlen(perid1)+145);
			sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid1);
			getsupportingfiles(parent,p,updatesupporting);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+150);
				sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid1);
				getsupportingfiles(parent,p,updatesupporting);
				if(perid1!=NULL) Rfree(perid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(dmgnum,"CURRENT EMPLOYEE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(perid1)+250);
					sprintf(warnmessage,"The Personnel Identification of [%s] is not a CURRENT EMPLOYEE in the [PRSNNL][PERDMG] data file.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","NOT CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingfiles(parent,p,updatesupporting);
					if(perid1!=NULL) Rfree(perid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	}
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid1);
	getsupportingfiles(parent,p,updatesupporting);
	if(perid1!=NULL) Rfree(perid1);
	return(TRUE);
}
*/
static void check_unitscb(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(dbsort->fileno,parent);
	check_units(parent,p,TRUE);
	updateSCRNvirtuals(parent);
}
static short check_units(RDArsrc *parent,PassableStruct *p,short updatesupporting)
{
	char *perid1=NULL,*warnmessage=NULL,*perid2=NULL;
	double sub_unass_units1=0.0,sub_units1=0.0;
	double total_ass_units=0.0,ass_units=0.0;
	int transno1=0,jobno1=0,subtransno1=0;
	int transno2=0,jobno2=0,subtransno2=0;
	short ef=0;

	FINDRSCGETSTRING(parent,"PERSONNEL IDENTIFICATION",&perid1);
	FINDRSCGETINT(parent,"TRANSACTION NUMBER",&transno1);
	FINDRSCGETINT(parent,"JOB NUMBER",&jobno1);
	FINDRSCGETINT(parent,"SUBSTITUTE TRANSACTION NUMBER",&subtransno1);
	ZERNRD(dbsort->fileno);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",transno1);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno1);
	ef=GTENRD(dbsort->fileno,1);
	while(!ef)
	{
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&perid2);
		FINDFLDGETINT(dbsort->fileno,"TRANSACTION NUMBER",&transno2);
		FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&jobno2);
		if(!RDAstrcmp(perid2,perid1) && transno2==transno1 && jobno2==jobno1)
		{
			FINDFLDGETINT(dbsort->fileno,"SUBSTITUTE TRANSACTION NUMBER",&subtransno2);
			if(subtransno2!=subtransno1)
			{
				FINDFLDGETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS",&ass_units);
				total_ass_units+=ass_units;
			}
		}
		if(perid2!=NULL) Rfree(perid2);
		ef=NXTNRD(dbsort->fileno,1);
	}
	FINDRSCGETDOUBLE(parent,"SUBSTITUTE UNITS UNASSIGNED",&sub_unass_units1);
	FINDRSCGETDOUBLE(parent,"SUBSTITUTE UNITS",&sub_units1);
	if((total_ass_units+sub_units1)>sub_unass_units1)
	{
		warnmessage=Rmalloc(300);
		sprintf(warnmessage,"The Substitute Units of [%8f] when added to the Substitute Units already assigned of [%8f] are greater than the total of the UNASSIGNED SUBSTITUTE UNITS of [%8f] that is needed for a substitution.  Please select a valid Substitute Units value to save this record.",sub_units1,total_ass_units,sub_unass_units1);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(perid1!=NULL) Rfree(perid1);
		if(perid2!=NULL) Rfree(perid2);
		if(warnmessage!=NULL) Rfree(warnmessage);
		getsupportingfiles(parent,p,updatesupporting);
		return(FALSE);
	} else if(sub_units1==0.0)
	{
		warnmessage=Rmalloc(170);
		sprintf(warnmessage,"Can not save a Zero [0.0] value for Substitute Units.  Please select a valid Substitute Units value to save this record.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(perid1!=NULL) Rfree(perid1);
		if(perid2!=NULL) Rfree(perid2);
		if(warnmessage!=NULL) Rfree(warnmessage);
		getsupportingfiles(parent,p,updatesupporting);
		return(FALSE);
	}
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	return(TRUE);
}
static short check_subid(RDArsrc *parent,PassableStruct *p,short updatesupporting)
{
	char *subperid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(subdmgnum!=(-1))
	{
		ZERNRD(subdmgnum);
		FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",subperid1);
		if(ADVEQLNRDsec(subdmgnum,1,EmpAbsSubData,p))
		{
			KEYNRD(subdmgnum,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is an invalid value.  It does not exist in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record.",
				(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingfiles(parent,p,updatesupporting);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(subdmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is a deleted value in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record."
					,(subperid1==NULL?"" :subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingfiles(parent,p,updatesupporting);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(subdmgnum,"CURRENT EMPLOYEE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
					sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is not a CURRENT EMPLOYEE in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record."
						,(subperid1==NULL?"":subperid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","NOT CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingfiles(parent,p,updatesupporting);
					if(subperid1!=NULL) Rfree(subperid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	if(submstr!=(-1))
	{
		ZERNRD(submstr);
		FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",subperid1);
		if(ADVEQLNRDsec(submstr,1,SCRNvirtualSubData,p))
		{
			KEYNRD(submstr,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is an invalid value.  It does not exist in the [SUBMGT][SUBMASTR] data file.  Please select a valid Substitute Personnel Identification to save this record."
				,(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingfiles(parent,p,updatesupporting);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(submstr,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is a deleted value in the [SUBMGT][SUBMASTR] data file.   Please select a valid Substitute Personnel Identification to save this record."
					,(subperid1==NULL?"":subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingfiles(parent,p,updatesupporting);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(submstr,"ACTIVE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
					sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is not ACTIVE in the [SUBMGT][SUBMASTR] data file.   Please select a valid Substitute Personnel Identification to save this record."
						,(subperid1==NULL?"":subperid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingfiles(parent,p,updatesupporting);
					if(subperid1!=NULL) Rfree(subperid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	getsupportingfiles(parent,p,updatesupporting);
	if(subperid1!=NULL) Rfree(subperid1);
	return(TRUE);
}
static void changesub(RDArsrc *parent,PassableStruct *p,short update)
{
	short ef=0;
	short keyn=0,keynumber=0;
	char active=TRUE,deleteflag=FALSE;
	char *e=NULL;
	char *subperid1=NULL,*subperid2=NULL,*subjobnostr=NULL;
	int selsubjobno=0,subjobno1=(-1);
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(RDAstrcmp(subperid1,subpersonnel_identification) || subpersonnel_identification==NULL)
	{
			if(subjobnolist!=NULL) freeapplib(subjobnolist);
			subjobnolist=APPlibNEW();
			FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBERS",&selsubjobno);
			if((keyn=KEYNUMBER(subjmstnum,"JOBMSTR KEY"))!=(-1))
			{
				keynumber=keyn;
			} else {
				keynumber=1;
			}
			ZERNRD(subjmstnum);
			FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",subperid1);
			ef=ADVGTENRDsec(subjmstnum,keynumber,EmpAbsSubData,p);
			e=stralloc("Updating Substitute Job Numbers List");
			tmprsrc=diagscrn(subjmstnum,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
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
			while(!ef)
			{
				listupdated=FALSE;
				FINDFLDGETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",&subperid2);
				if(RDAstrcmp(subperid2,subperid1)) break;
				FINDFLDGETINT(subjmstnum,"JOB NUMBER",&subjobno1);
				FINDFLDGETCHAR(subjmstnum,"ACTIVE",&active);
				FINDFLDGETCHAR(subjmstnum,"DELETEFLAG",&deleteflag);
				if(active && !deleteflag)
				{
					subjobnostr=Rrealloc(subjobnostr,9);
					sprintf(subjobnostr,"%8d",subjobno1);
					addAPPlib(subjobnolist,subjobnostr);
					listupdated=TRUE;
				}
				if(subjobnostr!=NULL) Rfree(subjobnostr);
				ef=ADVNXTNRDsec(subjmstnum,keynumber,EmpAbsSubData,p);
				if(tmprsrc!=NULL)
					update_diagnostic(tmprsrc,listupdated);
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			if(subjobnolist->numlibs<1) addAPPlib(subjobnolist,"No Jobs Available");
			if(selsubjobno>=subjobnolist->numlibs) selsubjobno=0;
			if(!FINDRSCLISTAPPlib(parent,"SUBSTITUTE JOB NUMBERS",selsubjobno,subjobnolist))
			{
				updatersrc(parent,"SUBSTITUTE JOB NUMBERS");
			}
			changesubjobno1(parent,p);
		if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
		subpersonnel_identification=stralloc(subperid1);
	}
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	if(subperid1!=NULL) Rfree(subperid1);
	if(subperid2!=NULL) Rfree(subperid2);
}
static void check_subidcb(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	check_subid(parent,p,TRUE);
	changesub(parent,p,TRUE);
	updateSCRNvirtuals(parent);
}
static short check_subjobno(RDArsrc *parent,PassableStruct *p,short updatesupporting)
{
	char *subperid1=NULL,*warnmessage=NULL;
	int subjobno1=(-1);
	char delflag=FALSE;

	FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBER",&subjobno1);
	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(subjmstnum!=(-1))
	{
		ZERNRD(subjmstnum);
		FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",subperid1);
		FINDFLDSETINT(subjmstnum,"JOB NUMBER",subjobno1);
		if(ADVEQLNRDsec(subjmstnum,1,EmpAbsSubData,p))
		{
			KEYNRD(subjmstnum,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Job Number of [%8d] is an invalid value for the Substitute Personnel Identification of [%s].  Please select a valid Job Number to save this record.",subjobno1,(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE JOBNUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingfiles(parent,p,updatesupporting);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(subjmstnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Job Number of [%8d] is a deleted value for the Substitute Personnel Identification of [%s].  Please select a valid Job Number to save this record.",subjobno1,(subperid1==NULL?"":subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingfiles(parent,p,updatesupporting);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	getsupportingfiles(parent,p,updatesupporting);
	if(subperid1!=NULL) Rfree(subperid1);
	return(TRUE);
}
static void check_subjobnocb(RDArsrc *parent,PassableStruct *p)
{
	char *subperid1=NULL,*subjobnostr=NULL;
	int x=0,subjobno1=(-1);

	readwidget(parent,"SUBSTITUTE JOB NUMBER");
	readwidget(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION");
	FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBER",&subjobno1);
	subjobnostr=Rmalloc(7);
	sprintf(subjobnostr,"%6d",subjobno1);
	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
		if(subjobnostr!=NULL)
		{
			for(x=0;x<subjobnolist->numlibs;++x)
			{
				if(strncmp(subjobnostr,subjobnolist->libs[x],RDAstrlen(subjobnostr))<=0)
				{
					break;
				}
			}
			if(x>=subjobnolist->numlibs) x=(subjobnolist->numlibs-1);
		}
		FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBERS",x);
		subjobno1=Ratoi(subjobnostr);
		FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBER",subjobno1);
		updatersrc(parent,"SUBSTITUTE JOB NUMBERS");
		updatersrc(parent,"SUBSTITUTE JOB NUMBER");
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	if(subperid1!=NULL) Rfree(subperid1);
	getsupportingfiles(parent,p,TRUE);
	updateSCRNvirtuals(parent);
}
static void changesubjobno1(RDArsrc *parent,PassableStruct *p)
{
	char *subperid1=NULL,*subjobnostr=NULL;
	int subjobno1=(-1),selsubjobno=0;

	if(FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1)) return;
	FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBERS",&selsubjobno);
	if((RDAstrcmp(subjobnolist->libs[selsubjobno],"No Jobs Available")) &&
		(RDAstrcmp(subjobnolist->libs[selsubjobno],"Not Applicable")))
	{
		subjobnostr=stralloc(subjobnolist->libs[selsubjobno]);
	}
	subjobno1=Ratoi(subjobnostr);
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBER",subjobno1);
	updatersrc(parent,"SUBSTITUTE JOB NUMBER");
	if(subperid1!=NULL) Rfree(subperid1);
}
static void changesubjobnocb1(RDArsrc *parent,PassableStruct *p)
{
	changesubjobno1(parent,p);
	getsupportingfiles(parent,p,TRUE);
	updateSCRNvirtuals(parent);
}
static void getsupportingfiles(RDArsrc *parent,PassableStruct *p,short updatesupporting)
{
	char *temp=NULL;
	int num=0;
/*
	if(dmgnum!=(-1))
	{
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&temp);
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",temp);
		if(ADVEQLNRDsec(dmgnum,1,EmpAbsSubData,p)) KEYNRD(dmgnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(dmgnum,parent);
		} else {
			filerecord2rsrc(dmgnum,parent);
		}
	}
	if(payjmst!=(-1))
	{
		FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&num);
		FINDFLDSETINT(payjmst,"JOB NUMBER",num);
		if(ADVEQLNRDsec(payjmst,1,EmpAbsSubData,p)) KEYNRD(payjmst,1);
		if(updatesupporting)
		{
			updatesinglerecord2rsrc(payjmst,parent);
		} else {
			singlefilerecord2rsrc(payjmst,parent);
		}
        }
*/
	if(subdmgnum!=(-1))
	{
		FINDFLDGETSTRING(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
		FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",temp);
		if(ADVEQLNRDsec(subdmgnum,1,EmpAbsSubData,p)) KEYNRD(subdmgnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(subdmgnum,parent);
		} else {
			filerecord2rsrc(subdmgnum,parent);
		}
	}
	if(submstr!=(-1))
	{
		FINDFLDGETSTRING(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
		FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",temp);
		if(ADVEQLNRDsec(submstr,1,EmpAbsSubData,p)) KEYNRD(submstr,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(submstr,parent);
		} else {
			filerecord2rsrc(submstr,parent);
		}
	}
	if(subjmstnum!=(-1))
	{
		FINDFLDGETSTRING(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
		FINDFLDGETINT(dbsort->fileno,"SUBSTITUTE JOB NUMBER",&num);
		FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",temp);
		FINDFLDSETINT(subjmstnum,"JOB NUMBER",num);
		if(ADVEQLNRDsec(subjmstnum,1,EmpAbsSubData,p)) KEYNRD(subjmstnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(subjmstnum,parent);
		} else {
			filerecord2rsrc(subjmstnum,parent);
		}
        }
	if(temp!=NULL) Rfree(temp);
}
static short selectfunction(MakeBrowseList *blist)
{
	return(TRUE);
}
static void doexit(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	PassableStruct *p=NULL;

	if(blist!=NULL)
	{
		p=(PassableStruct *)blist->funcargs;
		brsrc=blist->mainrsrc;
		drsrc=blist->definelist;
		srsrc=blist->searchrsrc;
		if(blist!=NULL)
		{
			FREE_BROWSE_EXIT(blist);
			blist=NULL;
		}
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
	}
	getsupportingrecords(mainrsrc,FALSE,p);
}
static void save_record_up(RDArsrc *mtnrsrc,PassableStruct *p)
{
	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	save_check(mtnrsrc,p,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,PassableStruct *p)
{
	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	save_check(mtnrsrc,p,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,PassableStruct *p,short update_list)
{
	if(check_subid(mtnrsrc,p,TRUE)==FALSE)
	{
		return;
	}
	if(check_subjobno(mtnrsrc,p,TRUE)==FALSE)
	{
		return;
	}
	if(check_units(mtnrsrc,p,TRUE)==FALSE)
	{
		return;
	}
	save_record(mtnrsrc,p,update_list);
}
static void save_record(RDArsrc *mtnrsrc,PassableStruct *p,short update_list)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_empabtr->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_empabtr->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,"Browse Diagnostic",NULL);
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
			xmakebrowselist(mbl_empabtr,tmprsrc);
			if(mbl_empabtr->list->num<=valuex) valuex=0;
			mbl_empabtr->list->keyvalue=mbl_empabtr->list->key[valuex];
			if(!FINDRSCSETLIST(mbl_empabtr->mainrsrc,"BROWSE LIST",valuex,
			mbl_empabtr->list->num,&mbl_empabtr->list->string))
			{
				updatersrc(mbl_empabtr->mainrsrc,"BROWSE LIST");
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			ForceWindowUpdate(mtnrsrc);
		}
	}
	quit_record(mtnrsrc,p);
}
static void quit_record(RDArsrc *mtnrsrc,PassableStruct *p)
{
	if(p!=NULL) Rfree(p);
        if(mtnrsrc!=NULL)
        {
                killwindow(mtnrsrc);
                free_rsrc(mtnrsrc);
        }
}
static void setkeys(RDArsrc *mtnrsrc)
{
	char *perid1=NULL;
	int jobno=0,transno=0,subtransno=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid1);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&transno);
	FINDRSCGETINT(mtnrsrc,"SUBSTITUTE TRANSACTION NUMBER",&subtransno);
	ZERNRD(dbsort->fileno);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",transno);
	FINDFLDSETINT(dbsort->fileno,"SUBSTITUTE TRANSACTION NUMBER",subtransno);
	if(perid1!=NULL) Rfree(perid1);
}
static void getpreviouscb(RDArmem *member,PassableStruct *p)
{
	RDArsrc *mtnrsrc;

	mtnrsrc=member->parent;
	setkeys(mtnrsrc);
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
	getsupportingfiles(mtnrsrc,p,TRUE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getnextcb(RDArmem *member,PassableStruct *p)
{
	RDArsrc *mtnrsrc;

	mtnrsrc=member->parent;
	setkeys(mtnrsrc);
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
	getsupportingfiles(mtnrsrc,p,TRUE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getfile(RDArsrc *mtnrsrc,PassableStruct *p)
{
	setkeys(mtnrsrc);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
	}
	getsupportingfiles(mtnrsrc,p,TRUE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void doexit2(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

	if(blist!=NULL)
	{
		brsrc=blist->mainrsrc;
		drsrc=blist->definelist;
		srsrc=blist->searchrsrc;
		if(blist!=NULL)
		{
			FREE_BROWSE_EXIT(blist);
			blist=NULL;
		}
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
	}
}
static void okfunc2(MakeBrowseList *b,void *targetkey)
{
	char *perid1=NULL;
	PassableStruct *p=NULL;

	p=b->funcargs;
	ZERNRD(submstr);
	if(ADVEQLNRDKEYsec(submstr,1,targetkey,EmpAbsSubData,p))
	{
		KEYNRD(submstr,1);
	} else {
		FINDFLDGETSTRING(submstr,"PERSONNEL IDENTIFICATION",&perid1);
	}
	FINDRSCSETSTRING(p->r,"SUBSTITUTE PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",perid1);
	changesub(p->r,p,TRUE);
	getsupportingfiles(p->r,p,FALSE);
	computeallSCRNvirtuals(p->r);
	updateallrsrc(p->r);
	doexit2(b);
	if(perid1!=NULL) Rfree(perid1);
}
static short sub_selectfunc(MakeBrowseList *blist)
{
	char *perid=NULL,*subperid=NULL,delflag=FALSE;
	PassableStruct *p=NULL;

	FINDFLDGETCHAR(submstr,"DELETEFLAG",&delflag);
	if(delflag) return(FALSE);
	p=blist->funcargs;
	FINDRSCGETSTRING(p->r,"PERSONNEL IDENTIFICATION",&perid);
	FINDFLDGETSTRING(submstr,"PERSONNEL IDENTIFICATION",&subperid);
	if(!RDAstrcmp(perid,subperid))
	{
		if(perid!=NULL) Rfree(perid);
		if(subperid!=NULL) Rfree(subperid);
		return(FALSE);
	}
	if(perid!=NULL) Rfree(perid);
	if(subperid!=NULL) Rfree(subperid);
	return(TRUE);
}
static void browse_submstr(RDArsrc *rsrc,PassableStruct *p)
{
	RDArsrc *browsersrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	browsersrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUBSTITUTE BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUBSTITUTE SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUBSTITUTE DEFINE LIST");
	addDFincvir(searchrsrc,"SUBMGT","SUBMASTR",NULL,submstr);
	addDFincvir(definelist,"SUBMGT","SUBMASTR",NULL,submstr);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ADVSbrowse(submstr,1,browsersrc,searchrsrc,definelist,
		NULL,NULL,sub_selectfunc,doexit2,okfunc2,NULL,
		p,TRUE,NULL,0,0,0,FALSE);
}
static void selectm(short dowhich,void *targetkey,
	int argc,char **argv,MakeBrowseList *blist)
{
	RDArsrc *mtnrsrc=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	short nofields=0,nokeys=0,x=0,edit_rsrc=FALSE;
	char editable=TRUE,*name=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	PassableStruct *p=NULL;
	int start=0;

	mtnrsrc=RDArsrcNEW(module,"ADD EMPLOYEE JOB ABSENTEE SUB-WORKED MAINTAIN SCREEN");
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,subdmgnum);
	addDFincvir(mtnrsrc,"PAYROLL","PAYJMST",NULL,subjmstnum);
	addDFincvir(mtnrsrc,"SUBMGT","SUBMASTR",NULL,submstr);
	addDFincvir(mtnrsrc,"EMPABS","EMPABS",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(dbsort->fileno);
	fields=FLDPOINTER(dbsort->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"SUBSTITUTE PERSONNEL IDENTIFICATION") ||
				!RDAstrcmp(fldx->name,"SUBSTITUTE TRANSACTION NUMBER") ||
				!RDAstrcmp(fldx->name,"SUBSTITUTE UNITS") ||
				!RDAstrcmp(fldx->name,"STARTING DATE") ||
				!RDAstrcmp(fldx->name,"ENDING DATE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					editable=(char)edit_rsrc;
					if(name==NULL)
					{
						name=Rmalloc(RDAstrlen(fldx->name)+1);
					} else {
						name=Rrealloc(name,RDAstrlen(fldx->name)+1);
					}
					sprintf(name,"%s",fldx->name);
					makefieldrsrc(mtnrsrc,name,fldx->type,fldx->len,editable);
					FINDRSCSETFUNC(mtnrsrc,name,setfiles,p);
				}
			} else if(!RDAstrcmp(fldx->name,"SUBSTITUTE JOB NUMBER"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					editable=(char)edit_rsrc;
					if(name==NULL)
					{
						name=Rmalloc(RDAstrlen(fldx->name)+1);
					} else {
						name=Rrealloc(name,RDAstrlen(fldx->name)+1);
					}
					sprintf(name,"%s",fldx->name);
					makefieldrsrc(mtnrsrc,name,fldx->type,fldx->len,editable);
					FINDRSCSETFUNC(mtnrsrc,name,setfiles,p);
					addlstrsrc(mtnrsrc,"SUBSTITUTE JOB NUMBERS",&start,TRUE,
						changesubjobnocb1,subjobnolist->numlibs,
						&subjobnolist->libs,p);
				}
			} else {
				edit_rsrc=FALSE;
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					editable=(char)edit_rsrc;
					if(name==NULL)
					{
						name=Rmalloc(RDAstrlen(fldx->name)+1);
					} else {
						name=Rrealloc(name,RDAstrlen(fldx->name)+1);
					}
					sprintf(name,"%s",fldx->name);
					makefieldrsrc(mtnrsrc,name,fldx->type,fldx->len,editable);
					FINDRSCSETFUNC(mtnrsrc,name,setfiles,p);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	key=KEYNUM(dbsort->fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			FINDRSCSETFUNC(mtnrsrc,part->name,getfile,p);
		}
	}
	FINDRSCSETFUNC(mtnrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",check_subidcb,p);
	FINDRSCSETFUNC(mtnrsrc,"SUBSTITUTE JOB NUMBER",check_subjobnocb,p);
	FINDRSCSETFUNC(mtnrsrc,"SUBSTITUTE UNITS",check_unitscb,p);
	if(subdmgnum!=(-1)) file2rsrc(subdmgnum,mtnrsrc,FALSE);
	if(subjmstnum!=(-1)) file2rsrc(subjmstnum,mtnrsrc,FALSE);
	if(submstr!=(-1)) file2rsrc(submstr,mtnrsrc,FALSE);
	if(filenum!=(-1)) file2rsrc(filenum,mtnrsrc,FALSE);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* this option isn't used */
			break;
		case 2:
			rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(dbsort->fileno,1,targetkey,EmpAbsSubData,p))
			{
				KEYNRD(dbsort->fileno,1);
				singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
			} else {
				singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
			}
			break;
	}
	changesub(mtnrsrc,p,FALSE);
	DCTsetlist(mtnrsrc,p);
	getsupportingfiles(mtnrsrc,p,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	nokeys=NUMKEYS(dbsort->fileno);
	MakePrevNextButtons(mtnrsrc,key,nokeys,p);
	addbtnrsrc(mtnrsrc,"BROWSE SUBSTITUTES",TRUE,browse_submstr,p);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,p);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,p);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,p);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record,p);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mtnrsrc,TRUE,quit_record,p,FALSE);
}
static void resetdefs(RDArsrc *mtnrsrc,PassableStruct *p)
{
	ZERNRD(dbsort->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
	getfile(mtnrsrc,p);
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	selectm(0,targetkey,0,NULL,blist);
}
static void setfiles(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2singlefilerecord(dbsort->fileno,parent);
	getsupportingfiles(parent,p,TRUE);
	updateSCRNvirtuals(parent);
}
void browse_empabtr(RDArsrc *parent,PassableStruct *p)
{
        RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
	getsupportingrecords(mainrsrc,TRUE,p);
	brsrc=RDArsrcNEW(module,"ADD EMPLOYEE JOB ABSENTEE SUB-WORKED BROWSE");
	drsrc=RDArsrcNEW(module,"ADD EMPLOYEE JOB ABSENTEE SUB-WORKED DEFINE LIST");
	srsrc=RDArsrcNEW(module,"ADD EMPLOYEE JOB ABSENTEE SUB-WORKED SEARCH BROWSE");
	addDFincvir(brsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(brsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(drsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(drsrc,module,"EMPABTR",NULL,empabtr);
	addDFincvir(drsrc,module,"EMPADEF",NULL,empadef);
	addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(drsrc,"PAYROLL","PAYJMST",NULL,payjmst);
	addDFincvir(drsrc,"POSTRK","POSPER",NULL,posper);
	addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(drsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(drsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(drsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(drsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(drsrc,"POSTRK","POSSMST",NULL,possmst);
	addDFincvir(drsrc,"POSTRK","POSNGMS",NULL,posngms);
	addDFincvir(srsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(srsrc,module,"EMPABTR",NULL,empabtr);
	addDFincvir(srsrc,module,"EMPADEF",NULL,empadef);
	addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(srsrc,"PAYROLL","PAYJMST",NULL,payjmst);
	addDFincvir(srsrc,"POSTRK","POSPER",NULL,posper);
	addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(srsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(srsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(srsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(srsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(srsrc,"POSTRK","POSSMST",NULL,possmst);
	file2rsrc(filenum,brsrc,FALSE);
	filerecord2rsrc(filenum,brsrc);
	file2rsrc(dmgnum,brsrc,FALSE);
	filerecord2rsrc(dmgnum,brsrc);
	GET_SCREEN_VIRTUALS(brsrc,0);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	mbl_empabtr=ADVbrowse(dbsort->fileno,1,brsrc,srsrc,drsrc,NULL,
		selectfunction,doexit,okfunc,NULL,p,TRUE,NULL,
		0,0,0,TRUE);
	FINDRSCSETEDITABLE(brsrc,"SEARCH BUTTON",FALSE);
}
