/* subwrkms.c - Xpert Substitute Work Master */
/*lint -library */
#ifndef WIN32
#define __NAM__ "subwrkms.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subwrkms.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
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
};
typedef struct mtnstructs mtnstruct;

static void quit_record(RDArsrc *,mtnstruct *);
static short check_peridmsr(char *,char *,char *);
static short check_periddmg(char *,char *,char *);
static short check_empid(char *,char *,char *);
static short check_empabs(char *,char *,char *,int);
static short check_empabtr(char *,char *,char *,int,int);
static short check_jobno(char *,char *,char *,int);
static short check_posid(char *,char *,char *);
static short check_empjobno(char *,char *,char *,int);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *);
static void empidcb(RDArsrc *);
static void jobnocb(RDArsrc *);
static void empjobnocb(RDArsrc *);
static void empabscb(RDArsrc *);
static void posidcb(RDArsrc *);
static void getsupportingrecords(RDArsrc *);
static void subwrkmsm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short filenum=(-1),msrnum=(-1),posnum=(-1),dmgnum=(-1),jmstnum=(-1);
static short dmg2num=(-1),jmst2num=(-1),empabs=(-1),empabtr=(-1);
static char editable=TRUE;
static char *module="SUBMGT";
static MakeBrowseList *mbl_subwrkms=NULL;
static MaintainMaster *MTNMASTER=NULL;
static void browse_subwrkms(RDArsrc *,mtnstruct *);

static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,mtn->previous))
	{
		desc=QUITvfydesc(filenum,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_quit,
			quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
static void browse_subwrkms(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	addDFincvir(mainrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(searchrsrc,module,"SUBWKMAS",NULL,filenum);
	addDFincvir(searchrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(searchrsrc,"PAYROLL","PAYJMST",NULL,jmstnum);
	addDFincvir(definelist,module,"SUBWKMAS",NULL,filenum);
	addDFincvir(definelist,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(definelist,"PAYROLL","PAYJMST",NULL,jmstnum);
	ApplyPassKey(MTNMASTER->passkey);
	if(dmgnum!=(-1))
	{
		COPYFIELD(filenum,dmgnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmgnum,1)) 
		{
			ZERNRD(dmgnum);
		} else {
			file2rsrc(dmgnum,mainrsrc,FALSE);
			filerecord2rsrc(dmgnum,mainrsrc);
		}
	}
	if(msrnum!=(-1))
	{
		COPYFIELD(filenum,msrnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(msrnum,1)) 
		{
			ZERNRD(msrnum);
		} else {
			file2rsrc(msrnum,mainrsrc,FALSE);
			filerecord2rsrc(msrnum,mainrsrc);
		}
	}
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	SCRNvirtual2rsrc(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_subwrkms=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,
		new_record,NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN SUBWKMAS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SUBWKMAS",&editable))==(-1)) return;
	if((msrnum=APPOPNNRD(module,"SUBMASTR",TRUE,FALSE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((jmstnum=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((dmg2num=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((posnum=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((jmst2num=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((empabs=APPOPNNRD("EMPABS","EMPABS",TRUE,FALSE))==(-1)) return;
	if((empabtr=APPOPNNRD("EMPABS","EMPABTR",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN SUBWKMAS");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SUBWKMAS",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				subwrkmsm(1,NULL);
				break;
			case 1:
				browse_subwrkms(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
void doexit(MakeBrowseList *blist)
{
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
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(msrnum!=(-1)) CLSNRD(msrnum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(jmstnum!=(-1)) CLSNRD(jmstnum);
		if(dmg2num!=(-1)) CLSNRD(dmg2num);
		if(posnum!=(-1)) CLSNRD(posnum);
		if(jmst2num!=(-1)) CLSNRD(jmst2num);
		if(empabs!=(-1)) CLSNRD(empabs);
		if(empabtr!=(-1)) CLSNRD(empabtr);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	mbl_subwrkms=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			subwrkmsm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		subwrkmsm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	subwrkmsm(2,NULL);
} 
static void subwrkmsm(short dowhich,void *targetkey)
{
	char *date=NULL,*timex=NULL;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	ZERNRD(filenum);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=RDATDataNEW(filenum);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[SUBWKMAS][PERSONNEL IDENTIFICATION]",getrecordcb,
		mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBWKMAS][TRANSACTION NUMBER]",getrecordcb,
		mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBWKMAS][JOB NUMBER]",jobnocb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]",empidcb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[SUBWKMAS][EMPLOYEE JOB NUMBER]",empjobnocb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[SUBWKMAS][EMPLOYEE TRANSACTION NUMBER]",empabscb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[SUBWKMAS][POSITION IDENTIFICATION]",posidcb,NULL);
	FINDRSCSETEDITABLE(mtnrsrc,"[SUBWKMAS][PAID UNITS]",FALSE);
	if(msrnum!=(-1)) file2rsrc(msrnum,mtnrsrc,FALSE);
	if(posnum!=(-1)) file2rsrc(posnum,mtnrsrc,FALSE);
	if(dmgnum!=(-1)) file2rsrc(dmgnum,mtnrsrc,FALSE);
	if(jmstnum!=(-1)) file2rsrc(jmstnum,mtnrsrc,FALSE);
	if(dmg2num!=(-1)) singlefile2rsrc(dmg2num,mtnrsrc,FALSE);
	if(jmst2num!=(-1)) singlefile2rsrc(jmst2num,mtnrsrc,FALSE);
	if(empabs!=(-1)) file2rsrc(empabs,mtnrsrc,FALSE);
	if(empabtr!=(-1)) file2rsrc(empabtr,mtnrsrc,FALSE);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][ENTRY TIME]",timex);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,mtn->previous);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][SOURCE USER]",USERLOGIN);
			FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][ENTRY DATE]",date);
			FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][ENTRY TIME]",timex);
			break;
		case 0:
		default:
			if(EQLNRDKEYsec(filenum,MTNMASTER->passkey->keyno,targetkey))
			{
				ZERNRD(filenum);
				ApplyPassKey(MTNMASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[SUBWKMAS][ENTRY TIME]",timex);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,mtn->previous);
			}
			break;
	}
	getsupportingrecords(mtnrsrc);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(mtnrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mtnrsrc,"PAYROLL","PAYJMST",NULL,jmstnum);
	addDFincvir(mtnrsrc,module,"SUBWKMAS",NULL,filenum);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posnum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_subwrkms!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
		}
	}
	if(mbl_subwrkms==NULL) 
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_subwrkms,mtn);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_subwrkms==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static short check_peridmsr(char *warnmessage,char *header,char *perid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(msrnum,"PERSONNEL IDENTIFICATION",perid1);
	if(EQLNRDsec(msrnum,1))
	{
		sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  It does not exist in the [SUBMGT][SUBMASTR] file.  Please select a valid Personnel Identification.",(perid1==NULL?"":perid1));
		sprintf(header,"INVALID PERSONNEL IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(msrnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Personnel Identification of [%s] is deleted value.  It exists in the [SUBMGT][SUBMASTR] but is in a delted state.  Please select a valid Personnel Identification.",(perid1==NULL?"":perid1));
			sprintf(header,"DELETED PERSONNEL IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_periddmg(char *warnmessage,char *header,char *perid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
	if(EQLNRDsec(dmgnum,1))
	{
		sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  It does not exist in the [PRSNNL][PERDMG] file.  Please select a valid Personnel Identification.",(perid1==NULL?"":perid1));
		sprintf(header,"INVALID PERSONNEL IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value. It exists in the [PRSNNL][PERDMG] file but is in a deleted state.  Please select a valid Personnel Identification from this file.",(perid1==NULL?"":perid1));
			sprintf(header,"DELETED PERSONNEL IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_empid(char *warnmessage,char *header,char *perid2)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(dmg2num,"PERSONNEL IDENTIFICATION",perid2);
	if(EQLNRDsec(dmg2num,1))
	{
		sprintf(warnmessage,"The Employee Identification of [%s] is an invalid value.  It does not exist as a Personnel Identification in the [PRSNNL][PERDMG] file.  Please select a valid Personnel Identification.",(perid2==NULL?"":perid2));
		sprintf(header,"INVALID EMPLOYEE IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(dmg2num,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Employee Identification of [%s] is a deleted value.  It exists as a Personnel Identification in the [PRSNNL][PERDMG] file but is in a deleted state.  Please select a valid Personnel Identification from this file.",(perid2==NULL ?"":perid2));
			sprintf(header,"DELETED EMPLOYEE JOB NUMBER!");
			return(TRUE);
		 }
	}
	return(FALSE);
}
static short check_jobno(char *warnmessage,char *header,char *perid1,int jobno1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETINT(jmstnum,"JOB NUMBER",jobno1);
	if(EQLNRDsec(jmstnum,1))
	{
		sprintf(warnmessage,"The Job Number of [%d] is an invalid value for the Personnel Identification of [%s].  It does not exist as a Job Number for this Personnel in the [PAYROLL][PAYJMST] file.  Please select a valid Job Numberg.",jobno1,(perid1==NULL?"":perid1));
		sprintf(header,"INVALID JOB NUMBER!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(jmstnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Job Number of [%d] is a deleted value for the Personnel Identification [%s].  It exists as a Job Number for this Personnel in the [PAYROLL][PAYJMST] file but is in a deleted state.  Please select a valid Job Number from this file.",jobno1,(perid1==NULL?"":perid1));
			sprintf(header,"DELETED JOB NUMBER!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_posid(char *warnmessage,char *header,char *posid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(posnum,"POSITION IDENTIFICATION",posid1);
	if(EQLNRDsec(posnum,1))
	{
		sprintf(warnmessage,"The Position Identification of [%s] is an invalid value.  It does not exist in the [POSTRK][POSMSTR] file.  Please select a valid Position Identification.",(posid1==NULL?"":posid1));
		sprintf(header,"INVALID POSITION IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(posnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Position Identification of [%s] is a deleted value. It exists for the [POSTRK][POSMSTR] file but is in a deleted state.  Please select a valid Position Identification from this file.",(posid1==NULL?"":posid1));
			sprintf(header,"DELETED POSITION IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_empjobno(char *warnmessage,char *header,char *perid2,int jobno2)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(jmst2num,"PERSONNEL IDENTIFICATION",perid2);
	FINDFLDSETINT(jmst2num,"JOB NUMBER",jobno2);
	if(EQLNRDsec(jmst2num,1))
	{
		sprintf(warnmessage,"The Employee Job Number of [%d] is an invalid value for the Employee Identification [%s].  It does not exist as a Job Number for this employee in the [PAYROLL][PAYJMST] file.  Please select a valid Job Number.",jobno2,(perid2==NULL?"":perid2));
		sprintf(header,"INVALID EMPLOYEE JOB NUMBER!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(jmst2num,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Employee Job Number of [%d] is a deleted value for the Employee Identification [%s].  It exists as a Job Number for this employee in the [PAYROLL][PAYJMST] file but is in a deleted state.  Please select a valid Job Number from this file.",jobno2,(perid2==NULL?"":perid2));
			sprintf(header,"DELETED EMPLOYEE JOB NUMBER!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_empabs(char *warnmessage,char *header,char *perid2,int transno2)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(empabs,"PERSONNEL IDENTIFICATION",perid2);
	FINDFLDSETINT(empabs,"TRANSACTION NUMBER",transno2);
	if(EQLNRDsec(empabs,1))
	{
		sprintf(warnmessage,"The Employee Transaction Number of [%d] is an invalid value for the Employee Identification [%s].  It does not exist as a Transaction Number for this employee in the [EMPABS][EMPABS] file.  Please select a valid Transaction Number.",transno2,(perid2==NULL?"":perid2));
		sprintf(header,"INVALID EMPLOYEE TRANSACTION NUMBER!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(empabs,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Employee Transaction Number of [%d] is a deleted value for the Employee Identification [%s].  It exists as a Transaction Number for this employee in the [EMPABS][EMPABS] file but is in a deleted state.  Please select a valid Transaction Number from this file.",transno2,(perid2==NULL?"":perid2));
			sprintf(header,"DELETED EMPLOYEE TRANSCTION NUMBER!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_empabtr(char *warnmessage,char *header,
	char *perid2,int transno2,int jobno2)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(empabtr,"PERSONNEL IDENTIFICATION",perid2);
	FINDFLDSETINT(empabtr,"TRANSACTION NUMBER",transno2);
	FINDFLDSETINT(empabtr,"JOB NUMBER",jobno2);
	if(EQLNRDsec(empabtr,1))
	{
		sprintf(warnmessage,"The Employee Job Number of [%d] is an invalid value for the Employee Identification [%s] and Employee Transaction Number of [%d].  It does not exist as a Job Number for this employee and transaction number in the [EMPABS][EMPABTR] file.  Please select a valid Job Number.",jobno2,(perid2==NULL?"":perid2),transno2);
		sprintf(header,"INVALID EMPLOYEE JOB NUMBER!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(empabtr,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Employee Job Number of [%d] is a deleted value for the Employee Identification [%s] and Employee Transaction Number of [%d].  It exists as a Job Number for this employee in the [EMPABS][EMPABTR] file but is in a deleted state.  Please select a valid Job Number from this file.",jobno2,(perid2==NULL?"":perid2),transno2);
			sprintf(header,"DELETED EMPLOYEE JOB NUMBER!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char delflag=FALSE,*warnmessage=NULL,*header=NULL;
	char *perid1=NULL,*perid2=NULL,*posid1=NULL;
	int transno1=0,transno2=0,jobno1=0,jobno2=0;
	short return_value=FALSE;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[SUBWKMAS][DELETEFLAG]",&delflag);
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]",&perid2);
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][POSITION IDENTIFICATION]",&posid1);
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][TRANSACTION NUMBER]",&transno1);
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][JOB NUMBER]",&jobno1);
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][EMPLOYEE JOB NUMBER]",&jobno2);
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][EMPLOYEE TRANSACTION NUMBER]",&transno2);
	if(!delflag)
	{
		warnmessage=Rmalloc(500);
		header=Rmalloc(60);
		if(check_peridmsr(warnmessage,header,perid1))
		{
			return_value=TRUE;
		} else if(check_periddmg(warnmessage,header,perid1))
		{
			return_value=TRUE;
		} else if(check_jobno(warnmessage,header,perid1,jobno1))
		{
			return_value=TRUE;
		} else if(check_empid(warnmessage,header,perid2))
		{
			return_value=TRUE;
		} else if(check_empjobno(warnmessage,header,perid2,jobno2))
		{
			return_value=TRUE;
		} else if(check_posid(warnmessage,header,posid1))
		{
			return_value=TRUE;
		} else if(check_empabs(warnmessage,header,perid2,transno2))
		{
			return_value=TRUE;
		} else if(check_empabtr(warnmessage,header,perid2,transno2,jobno2))
		{
			return_value=TRUE;
		}
	}
	if(return_value) WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(posid1!=NULL) Rfree(posid1);
	if(return_value==FALSE) save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL,deleteflag=FALSE,subdeleteflag=FALSE;
	char *perid=NULL,*subid=NULL;
	short subfilenum=0,ef=0;
	int trans1=0,trans2=0;
	RDATData *prev=NULL;

	rsrc2filerecord(filenum,mtnrsrc);
	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		WRTTRANS(filenum,0,NULL,mtn->previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			subfilenum=OPNNRDsec(module,"SUBPAID",TRUE,TRUE,FALSE);	
			if(subfilenum!=(-1))
			{
				ZERNRD(subfilenum);
				FINDFLDGETSTRING(filenum,"PERSONNEL IDENTIFICATION",&perid);
				FINDFLDGETINT(filenum,"TRANSACTION NUMBER",&trans1);
				FINDFLDSETSTRING(subfilenum,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETINT(subfilenum,"TRANSACTION NUMBER",trans1);
				ef=GTENRDsec(subfilenum,1);
				while(!ef)
				{
					FINDFLDGETCHAR(subfilenum,"DELETEFLAG",&subdeleteflag);
					if(!subdeleteflag)
					{
						FINDFLDGETSTRING(subfilenum,"PERSONNEL IDENTIFICATION",&subid);
						FINDFLDGETINT(subfilenum,"TRANSACTION NUMBER",&trans2);
						if(RDAstrcmp(subid,perid) || trans1!=trans2) break;
						prev=RDATDataNEW(subfilenum);
						FINDFLDSETCHAR(subfilenum,"DELETEFLAG",deleteflag);
						WRTTRANSsec(subfilenum,0,NULL,prev);
						if(prev!=NULL) FreeRDATData(prev);
					}
					ef=NXTNRDsec(subfilenum,1); 	
				}
				CLSNRD(subfilenum);		
			}
		} 
		if(perid!=NULL) Rfree(perid);
		if(subid!=NULL) Rfree(subid);
		updatebrowse(update_list,mbl_subwrkms,filenum,module,mtnrsrc);
		if(mbl_subwrkms!=NULL) 
		{
			quit_record(mtnrsrc,mtn);
		} else {
			GetRDATData(filenum,mtn->previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(msrnum!=(-1)) CLSNRD(msrnum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(jmstnum!=(-1)) CLSNRD(jmstnum);
		if(dmg2num!=(-1)) CLSNRD(dmg2num);
		if(posnum!=(-1)) CLSNRD(posnum);
		if(jmst2num!=(-1)) CLSNRD(jmst2num);
		if(empabs!=(-1)) CLSNRD(empabs);
		if(empabtr!=(-1)) CLSNRD(empabtr);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void empidcb(RDArsrc *mtnrsrc)
{
	char *empid=NULL,*warnmessage=NULL,*header=NULL;

	readwidget(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]",&empid);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_empid(warnmessage,header,empid))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(empid!=NULL) Rfree(empid);
	if(EQLNRDsec(dmg2num,1)) KEYNRD(dmg2num,1);
	updatesinglerecord2rsrc(dmg2num,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void jobnocb(RDArsrc *mtnrsrc)
{
	char *perid=NULL,*warnmessage=NULL,*header=NULL;
	int jobno=0;

	readwidget(mtnrsrc,"[SUBWKMAS][PERSONNEL IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][PERSONNEL IDENTIFICATION]",&perid);
	readwidget(mtnrsrc,"[SUBWKMAS][JOB NUMBER]");
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][JOB NUMBER]",&jobno);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_jobno(warnmessage,header,perid,jobno))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(perid!=NULL) Rfree(perid);
	if(EQLNRDsec(jmstnum,1)) KEYNRD(jmstnum,1);
	updatefilerecord2rsrc(jmstnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void posidcb(RDArsrc *mtnrsrc)
{
	char *posid=NULL,*warnmessage=NULL,*header=NULL;

	readwidget(mtnrsrc,"[SUBWKMAS][POSITION IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][POSITION IDENTIFICATION]",&posid);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_posid(warnmessage,header,posid))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(posid!=NULL) Rfree(posid);
	if(EQLNRDsec(posnum,1)) KEYNRD(posnum,1);
	updatefilerecord2rsrc(posnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void empjobnocb(RDArsrc *mtnrsrc)
{
	char *empid=NULL,*warnmessage=NULL,*header=NULL;
	int transno=0,jobno=0;

	readwidget(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]",&empid);
	readwidget(mtnrsrc,"[SUBWKMAS][EMPLOYEE JOB NUMBER]");
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][EMPLOYEE JOB NUMBER]",&jobno);
	readwidget(mtnrsrc,"[SUBWKMAS][EMPLOYEE TRANSACTION NUMBER]");
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][EMPLOYEE TRANSACTION NUMBER]",&transno);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_empjobno(warnmessage,header,empid,jobno))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	} else if(check_empabtr(warnmessage,header,empid,transno,jobno))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(empid!=NULL) Rfree(empid);
	if(EQLNRDsec(jmst2num,1)) KEYNRD(jmst2num,1);
	updatesinglerecord2rsrc(jmst2num,mtnrsrc);
	updatefilerecord2rsrc(empabtr,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void empabscb(RDArsrc *mtnrsrc)
{
	char *empid=NULL,*warnmessage=NULL,*header=NULL;
	int transno=0;

	readwidget(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][EMPLOYEE IDENTIFICATION]",&empid);
	readwidget(mtnrsrc,"[SUBWKMAS][EMPLOYEE TRANSACTION NUMBER]");
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][EMPLOYEE TRANSACTION NUMBER]",&transno);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_empabs(warnmessage,header,empid,transno))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(empid!=NULL) Rfree(empid);
	if(EQLNRDsec(empabs,1)) KEYNRD(empabs,1);
	updatefilerecord2rsrc(empabs,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL;
	int transno=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[SUBWKMAS][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETINT(mtnrsrc,"[SUBWKMAS][TRANSACTION NUMBER]",&transno);
	ZERNRD(filenum);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(filenum,"TRANSACTION NUMBER",transno);
	getrecord(mtnrsrc,mtn);
	if(perid!=NULL) Rfree(perid);
	computeallSCRNvirtuals(mtnrsrc);
   	updateallrsrc(mtnrsrc);
}
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *date=NULL,*timex=NULL;

	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
		FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		ClearRDATData(mtn->previous);
	} else {
		filerecord2rsrc(filenum,mtnrsrc);	
		GetRDATData(filenum,mtn->previous);
	}
	getsupportingrecords(mtnrsrc);
}
static void getsupportingrecords(RDArsrc *mtnrsrc)
{
	if(msrnum!=(-1))
	{
		COPYFIELD(filenum,msrnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(msrnum,1)) KEYNRD(msrnum,1);
		filerecord2rsrc(msrnum,mtnrsrc);
	}
	if(dmgnum!=(-1))
	{
		COPYFIELD(filenum,dmgnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmgnum,1)) KEYNRD(dmgnum,1);
		filerecord2rsrc(dmgnum,mtnrsrc);
	}
	if(dmg2num!=(-1))
	{
		FIELDCOPY(filenum,"EMPLOYEE IDENTIFICATION",dmg2num,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmg2num,1)) KEYNRD(dmg2num,1);
		singlefilerecord2rsrc(dmg2num,mtnrsrc);
	}
	if(jmstnum!=(-1))
	{
		COPYFIELD(filenum,jmstnum,"PERSONNEL IDENTIFICATION");
		COPYFIELD(filenum,jmstnum,"JOB NUMBER");
		if(EQLNRDsec(jmstnum,1)) KEYNRD(jmstnum,1);
		filerecord2rsrc(jmstnum,mtnrsrc);
	}
	if(posnum!=(-1))
	{
		COPYFIELD(filenum,posnum,"POSITION IDENTIFICATION");
		if(EQLNRDsec(posnum,1)) KEYNRD(posnum,1);
		filerecord2rsrc(posnum,mtnrsrc);
	}
	if(jmst2num!=(-1))
	{
		FIELDCOPY(filenum,"EMPLOYEE IDENTIFICATION",jmst2num,"PERSONNEL IDENTIFICATION");
		FIELDCOPY(filenum,"EMPLOYEE JOB NUMBER",jmst2num,"JOB NUMBER");
		if(EQLNRDsec(jmst2num,1)) KEYNRD(jmst2num,1);
		singlefilerecord2rsrc(jmst2num,mtnrsrc);
	}
	if(empabs!=(-1))
	{
		FIELDCOPY(filenum,"EMPLOYEE IDENTIFICATION",empabs,"PERSONNEL IDENTIFICATION");
		FIELDCOPY(filenum,"EMPLOYEE TRANSACTION NUMBER",empabs,"TRANSACTION NUMBER");
		if(EQLNRDsec(empabs,1)) KEYNRD(empabs,1);
		filerecord2rsrc(empabs,mtnrsrc);
	}
	if(empabtr!=(-1))
	{
		FIELDCOPY(filenum,"EMPLOYEE IDENTIFICATION",empabtr,"PERSONNEL IDENTIFICATION");
		FIELDCOPY(filenum,"EMPLOYEE TRANSACTION NUMBER",empabtr,"TRANSACTION NUMBER");
		FIELDCOPY(filenum,"EMPLOYEE JOB NUMBER",empabtr,"JOB NUMBER");
		if(EQLNRDsec(empabtr,1)) KEYNRD(empabtr,1);
		filerecord2rsrc(empabtr,mtnrsrc);
	}
}
