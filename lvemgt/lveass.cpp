/* lveass.c - Leave Managment's Assign Leave from Employee Absentees*/
#ifndef WIN32
#define __NAM__ "lveass.lnx"
#endif
#ifdef WIN32
#define __NAM__ "lveass.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
#include <rptgen.hpp>
/*CPP_OPART lveass1 */
/*ADDLIB rpt */
/*ADDLIB add */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="LVEMGT";
MakeBrowseList *mbl_empabtr;
RDArsrc *mainrsrc=NULL;
short empabtr=(-1),empadef=(-1),empabs=(-1);
short prsnnl=(-1),jobmst=(-1),lvejob=(-1);
short posmstr=(-1),posper=(-1),posgrs=(-1),poscls=(-1),posdedc=(-1);
short possmst=(-1),posngms=(-1),poslmst=(-1);
short lvetpms=(-1),lvedsc=(-1),lvemstr=(-1),lvetran=(-1);
void getsupportingrecords(RDArsrc *,short,short);
int LEAVE_TRANSACTION_COUNT=0;
MaintainMaster *MTNMASTER=NULL;
void quit_record(RDArsrc *);
static void makelvevalues(void);
static void getempabtr(void);
static void freelvevalues(void);
static char *reportname=NULL,**run_type=NULL;
static void empabtrm(short,void *);
static void doexit(MakeBrowseList *);
static void okfunc(MakeBrowseList *,void *);
static short selectfunction(MakeBrowseList *);
static void select_lves2(RDArsrc *,void *);
static void select_lves_test(RDArsrc *);
extern void select_lves(RDArsrc *);

static void select_lves_test(RDArsrc *rsrc)
{
	char deleteflag=TRUE,*warnmessage=NULL;

	if(EQLNRDsec(empabtr,1))
	{
		KEYNRD(empabtr,1);
		warnmessage=Rmalloc(230);
		sprintf(warnmessage,"This is not a valid record selected from the [EMPABS][EMPABTR] data file.  Please select a valid Employee Job Absentee record before Assigning Leave.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EMPLOYEE JOB ABSENTEE RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return;
	} else {
		FINDFLDGETCHAR(empabtr,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			warnmessage=Rmalloc(255);
			sprintf(warnmessage,"This is not a valid record selected from the [EMPABS][EMPABTR] data file.  It exist but is in a Deleted state.  Please select a valid Employee Job Absentee record before Assigning Leave.");
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED EMPLOYEE JOB ABSENTEE RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDFLDGETCHAR(empabtr,"LEAVE FINALED",&deleteflag);
			if(deleteflag)
			{
				warnmessage=Rmalloc(255);
				sprintf(warnmessage,"This is not a valid record selected from the [EMPABS][EMPABTR] data file.  It exist but is Leave Finaled.  Please select a valid Employee Job Absentee record before Assigning Leave.");
				WARNINGDIALOG("WARNING DIALOG SCREEN","LEAVE FINALED RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			}
		}
	}
	select_lves(rsrc);
}
static void select_lves2(RDArsrc *rsrc,void *targetkey)
{
	if(!EQLNRDKEYsec(empabtr,1,targetkey))
	{
		getsupportingrecords(rsrc,FALSE,FALSE);
		select_lves(rsrc);
	}
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		switch(run_func)
		{
			default:
			case 0:
				empabtrm(0,targetkey);
				break;
			case 1:
				select_lves2(blist->mainrsrc,targetkey);
				break;
		}
	}
}
static void doexit(MakeBrowseList *blist)
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
		if(mbl_empabtr!=NULL) mbl_empabtr=NULL;
	}
	if(MTNMASTER!=NULL)
	{
		FreeMaintainMaster(MTNMASTER);
	}
	freelvevalues();
	if(empabtr!=(-1)) { CLSNRD(empabtr); empabtr=(-1); }
	if(lvejob!=(-1)) { CLSNRD(lvejob); lvejob=(-1); }
	if(empabs!=(-1)) { CLSNRD(empabs); empabs=(-1); }
	if(empadef!=(-1)) { CLSNRD(empadef); empadef=(-1); }
	if(lvetpms!=(-1)) { CLSNRD(lvetpms); lvetpms=(-1); }
	if(lvedsc!=(-1)) { CLSNRD(lvedsc); lvedsc=(-1); }
	if(lvemstr!=(-1)) { CLSNRD(lvemstr); lvemstr=(-1); }
	if(lvetran!=(-1)) { CLSNRD(lvetran); lvetran=(-1); }
	if(prsnnl!=(-1)) { CLSNRD(prsnnl); prsnnl=(-1); }
	if(jobmst!=(-1)) { CLSNRD(jobmst); jobmst=(-1); }
	if(posmstr!=(-1)) { CLSNRD(posmstr); posmstr=(-1); }
	if(posper!=(-1)) { CLSNRD(posper); posper=(-1); }
	if(poslmst!=(-1)) { CLSNRD(poslmst); poslmst=(-1); }
	if(posgrs!=(-1)) { CLSNRD(posgrs); posgrs=(-1); }
	if(poscls!=(-1)) { CLSNRD(poscls); poscls=(-1); }
	if(posdedc!=(-1)) { CLSNRD(posdedc); posdedc=(-1); }
	if(possmst!=(-1)) { CLSNRD(possmst); possmst=(-1); }
	if(posngms!=(-1)) { CLSNRD(posngms); posngms=(-1); }
	if(LEAVE_TRANSACTION_COUNT>0)
	{
		RUNREPORT(module,reportname,NULL,NULL,FALSE,TRUE,NULL);
	}
	if(reportname!=NULL) Rfree(reportname);
	ShutdownSubsystems();
	exit(0);
}
static void freelvevalues(void)
{
	short x;

	if(run_type!=NULL)
	{
		for(x=0;x<2;++x) Rfree(run_type[x]);
		Rfree(run_type);
	}
}
static short selectfunction(MakeBrowseList *b)
{
	char *perid=NULL;
	char lvefinaled=TRUE,deleteflag=TRUE,suv=FALSE;
	int transno=0;
	double lve_units_unass=0.0;

	FINDFLDGETCHAR(empabtr,"LEAVE FINALED",&lvefinaled);
	FINDFLDGETCHAR(empabtr,"DELETEFLAG",&deleteflag);
	FINDFLDGETDOUBLE(empabtr,"LEAVE UNITS UNASSIGNED",&lve_units_unass);
	if(!lvefinaled && !deleteflag && lve_units_unass>0.0)
	{
		FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&perid);
		FINDFLDSETSTRING(empabs,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDGETINT(empabtr,"TRANSACTION NUMBER",&transno);
		FINDFLDSETINT(empabs,"TRANSACTION NUMBER",transno);
		if(!EQLNRDsec(empabs,1))
		{
			FINDFLDGETCHAR(empabs,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(empabs,"SOURCE USER VERIFIED",&suv);
			if(!deleteflag && suv)
			{
				return(TRUE);
			} else {
				return(FALSE);
			}
		} else {
			return(FALSE);
		}

	} else {
		return(FALSE);
	}
}
static void browse_empabtr(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*definelist=NULL,*searchrsrc=NULL;
	int wx=1;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	addDFincvir(mainrsrc,"EMPABS","EMPABS",NULL,empabs);
	addDFincvir(mainrsrc,"EMPABS","EMPADEF",NULL,empadef);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	addDFincvir(searchrsrc,"EMPABS","EMPABTR",NULL,empabtr);
	addDFincvir(searchrsrc,"EMPABS","EMPABS",NULL,empabs);
	addDFincvir(searchrsrc,"EMPABS","EMPADEF",NULL,empadef);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(searchrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(searchrsrc,"POSTRK","POSPER",NULL,posper);
	addDFincvir(searchrsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(searchrsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(searchrsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(searchrsrc,"POSTRK","POSSMST",NULL,possmst);
	addDFincvir(searchrsrc,"POSTRK","POSNGMS",NULL,posngms);
	addDFincvir(definelist,"EMPABS","EMPABTR",NULL,empabtr);
	addDFincvir(definelist,"EMPABS","EMPABS",NULL,empabs);
	addDFincvir(definelist,"EMPABS","EMPADEF",NULL,empadef);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(definelist,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(definelist,"POSTRK","POSPER",NULL,posper);
	addDFincvir(definelist,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(definelist,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(definelist,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(definelist,"POSTRK","POSSMST",NULL,possmst);
	addDFincvir(definelist,"POSTRK","POSNGMS",NULL,posngms);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ZERNRD(empabtr);
	addlstrsrc(mainrsrc,"RUN FUNCTION",0,TRUE,NULL,2,&run_type,NULL);
	wx=1;
	FINDRSCSETINT(mainrsrc,"RUN FUNCTION",wx);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	mbl_empabtr=ADVSbrowse(empabtr,1,mainrsrc,searchrsrc,definelist,
		NULL,NULL,selectfunction,doexit,okfunc,
		NULL,NULL,TRUE,NULL,0,0,0,TRUE);
}
static void makelvevalues(void)
{
        run_type=Rmalloc(2*sizeof(char *));
        run_type[0]=stralloc("View Employee Job Absentee");
        run_type[1]=stralloc("Apply Leaves");
}
static void getempabtr(void)
{
	MTNMASTER=MaintainMasterNew(module,"EMPLOYEE ABSENTEE ASSIGN LEAVE");
	makelvevalues();
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","EMPLOYEE ABSENTEE ASSIGN LEAVE",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(empabtr);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				empabtrm(1,NULL);
				break;
			case 1:
				browse_empabtr(NULL);
				break;
		}
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"ASSIGN LEAVE")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else {
		reportname=stralloc("LUAUDIT");
	}
	if((empabtr=APPOPNNRD("EMPABS","EMPABTR",TRUE,TRUE))==(-1)) return;
	if((empabs=APPOPNNRD("EMPABS","EMPABS",TRUE,TRUE))==(-1)) return;
	if((empadef=APPOPNNRD("EMPABS","EMPADEF",TRUE,FALSE))==(-1)) return;
	if((lvetpms=APPOPNNRD(module,"LVETPMS",TRUE,FALSE))==(-1)) return;
	if((lvedsc=APPOPNNRD(module,"LVEDSC",TRUE,FALSE))==(-1)) return;
	if((lvemstr=APPOPNNRD(module,"LVEMSTR",TRUE,TRUE))==(-1)) return;
	if((lvetran=APPOPNNRD(module,"LVETRAN",TRUE,TRUE))==(-1)) return;
	if((prsnnl=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((jobmst=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((lvejob=APPOPNNRD(module,"LVEJOB",TRUE,TRUE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((poslmst=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	if((posper=APPOPNNRD("POSTRK","POSPER",TRUE,FALSE))==(-1)) return;
	if((posgrs=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((poscls=APPOPNNRD("POSTRK","POSCLS",TRUE,FALSE))==(-1)) return;
	if((posdedc=APPOPNNRD("POSTRK","POSDEDC",TRUE,FALSE))==(-1)) return;
	if((possmst=APPOPNNRD("POSTRK","POSSMST",TRUE,FALSE))==(-1)) return;
	if((posngms=APPOPNNRD("POSTRK","POSNGMS",TRUE,FALSE))==(-1)) return;
	getempabtr();
	RDAAPPMAINLOOP();
}
static void getrecordcb(RDArsrc *mtnrsrc)
{
	char *perid1=NULL;
	int jobno=0,transno=0;

	ZERNRD(empabtr);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[EMPABTR][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETINT(mtnrsrc,"[EMPABTR][JOB NUMBER]",&jobno);
	FINDRSCGETINT(mtnrsrc,"[EMPABTR][TRANSACTION NUMBER]",&transno);
	FINDFLDSETSTRING(empabtr,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETINT(empabtr,"JOB NUMBER",jobno);
	FINDFLDSETINT(empabtr,"TRANSACTION NUMBER",transno);
	if(EQLNRD(empabtr,1))
	{
		KEYNRD(empabtr,1);
		updatefilerecord2rsrc(empabtr,mtnrsrc);	
	} else {
		updatefilerecord2rsrc(empabtr,mtnrsrc);	
	}
	if(perid1!=NULL) Rfree(perid1);
	getsupportingrecords(mtnrsrc,TRUE,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void empabtrm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;

	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	file2rsrc(empabtr,mtnrsrc,FALSE);
	addDFincvir(mtnrsrc,"EMPABS","EMPABTR",NULL,empabtr);
	addDFincvir(mtnrsrc,"EMPABS","EMPABS",NULL,empabs);
	addDFincvir(mtnrsrc,"EMPABS","EMPADEF",NULL,empadef);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mtnrsrc,"PAYROLL","PAYJMST",NULL,jobmst);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(mtnrsrc,"POSTRK","POSPER",NULL,posper);
	addDFincvir(mtnrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(mtnrsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(mtnrsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(mtnrsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(mtnrsrc,"POSTRK","POSSMST",NULL,possmst);
	addDFincvir(mtnrsrc,"POSTRK","POSNGMS",NULL,posngms);
	ZERNRD(empabtr);
	if(empabs!=(-1)) file2rsrc(empabs,mtnrsrc,FALSE);
	if(empadef!=(-1)) file2rsrc(empadef,mtnrsrc,FALSE);
	if(prsnnl!=(-1)) file2rsrc(prsnnl,mtnrsrc,FALSE);
	if(jobmst!=(-1)) file2rsrc(jobmst,mtnrsrc,FALSE);
	if(posmstr!=(-1)) file2rsrc(posmstr,mtnrsrc,FALSE);
	if(posper!=(-1)) file2rsrc(posper,mtnrsrc,FALSE);
	if(poslmst!=(-1)) file2rsrc(poslmst,mtnrsrc,FALSE);
	if(posgrs!=(-1)) file2rsrc(posgrs,mtnrsrc,FALSE);
	if(poscls!=(-1)) file2rsrc(poscls,mtnrsrc,FALSE);
	if(posdedc!=(-1)) file2rsrc(posdedc,mtnrsrc,FALSE);
	if(possmst!=(-1)) file2rsrc(possmst,mtnrsrc,FALSE);
	if(posngms!=(-1)) file2rsrc(posngms,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"[EMPABTR][PERSONNEL IDENTIFICATION]",getrecordcb,
		NULL);
	FINDRSCSETFUNC(mtnrsrc,"[EMPABTR][TRANSACTION NUMBER]",getrecordcb,
		NULL);
	FINDRSCSETFUNC(mtnrsrc,"[EMPABTR][JOB NUMBER]",getrecordcb,
		NULL);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* this option isn't used */
			FINDRSCSETEDITABLE(mtnrsrc,"[EMPABTR][PERSONNEL IDENTIFICATION]",TRUE);
			FINDRSCSETEDITABLE(mtnrsrc,"[EMPABTR][TRANSACTION NUMBER]",TRUE);
			FINDRSCSETEDITABLE(mtnrsrc,"[EMPABTR][JOB NUMBER]",TRUE);
			break;
		case 2: /* defaults made, no inheritance */
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(empabtr,1,targetkey)) 
			{
				ZERNRD(empabtr);
			} else {
				filerecord2rsrc(empabtr,mtnrsrc);
			}
			break;
	}
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	getsupportingrecords(mtnrsrc,TRUE,FALSE);
	if(mbl_empabtr==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_empabtr,NULL);
#ifndef XXX
		addbtnrsrc(mtnrsrc,"ASSIGN LEAVE",TRUE,select_lves_test,NULL);
#endif
	}
#ifdef XXX
	addbtnrsrc(mtnrsrc,"ASSIGN LEAVE",TRUE,select_lves_test,NULL);
#endif
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record,NULL);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quit_record,NULL,FALSE);
}
void quit_record(RDArsrc *mtnrsrc)
{
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(mbl_empabtr==NULL)
	{
		if(MTNMASTER!=NULL)
		{
			FreeMaintainMaster(MTNMASTER);
		}
		freelvevalues();
		if(empabtr!=(-1)) { CLSNRD(empabtr); empabtr=(-1); }
		if(lvejob!=(-1)) { CLSNRD(lvejob); lvejob=(-1); }
		if(empabs!=(-1)) { CLSNRD(empabs); empabs=(-1); }
		if(empadef!=(-1)) { CLSNRD(empadef); empadef=(-1); }
		if(lvetpms!=(-1)) { CLSNRD(lvetpms); lvetpms=(-1); }
		if(lvedsc!=(-1)) { CLSNRD(lvedsc); lvedsc=(-1); }
		if(lvemstr!=(-1)) { CLSNRD(lvemstr); lvemstr=(-1); }
		if(lvetran!=(-1)) { CLSNRD(lvetran); lvetran=(-1); }
		if(prsnnl!=(-1)) { CLSNRD(prsnnl); prsnnl=(-1); }
		if(jobmst!=(-1)) { CLSNRD(jobmst); jobmst=(-1); }
		if(posmstr!=(-1)) { CLSNRD(posmstr); posmstr=(-1); }
		if(posper!=(-1)) { CLSNRD(posper); posper=(-1); }
		if(poslmst!=(-1)) { CLSNRD(poslmst); poslmst=(-1); }
		if(posgrs!=(-1)) { CLSNRD(posgrs); posgrs=(-1); }
		if(poscls!=(-1)) { CLSNRD(poscls); poscls=(-1); }
		if(posdedc!=(-1)) { CLSNRD(posdedc); posdedc=(-1); }
		if(possmst!=(-1)) { CLSNRD(possmst); possmst=(-1); }
		if(posngms!=(-1)) { CLSNRD(posngms); posngms=(-1); }
		if(LEAVE_TRANSACTION_COUNT>0)
		{
			RUNREPORT(module,reportname,NULL,NULL,TRUE,TRUE,NULL);
		}
		if(reportname!=NULL) Rfree(reportname);
		ShutdownSubsystems();
		exit(0);
	}
}
void getsupportingrecords(RDArsrc *mtnrsrc,short doloops,short update)
{
	COPYFIELD(empabtr,empabs,"PERSONNEL IDENTIFICATION");
	COPYFIELD(empabtr,empabs,"TRANSACTION NUMBER");
	if(EQLNRDsec(empabs,1)) KEYNRD(empabs,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(empabs,mtnrsrc);
		else filerecord2rsrc(empabs,mtnrsrc);
	}

	COPYFIELD(empabtr,prsnnl,"PERSONNEL IDENTIFICATION");
	if(EQLNRDsec(prsnnl,1)) KEYNRD(prsnnl,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(prsnnl,mtnrsrc);
		else filerecord2rsrc(prsnnl,mtnrsrc);
	}

	COPYFIELD(empabtr,jobmst,"PERSONNEL IDENTIFICATION");
	COPYFIELD(empabtr,jobmst,"JOB NUMBER");
	if(EQLNRDsec(jobmst,1)) KEYNRD(jobmst,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(jobmst,mtnrsrc);
		else filerecord2rsrc(jobmst,mtnrsrc);
	}

	COPYFIELD(jobmst,posmstr,"POSITION IDENTIFICATION");
	if(EQLNRDsec(posmstr,1)) KEYNRD(posmstr,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(posmstr,mtnrsrc);
		else filerecord2rsrc(posmstr,mtnrsrc);
	}

	COPYFIELD(posmstr,posper,"POSITION IDENTIFICATION");
	COPYFIELD(jobmst,posper,"PERSONNEL IDENTIFICATION");
	COPYFIELD(jobmst,posper,"JOB NUMBER");
	if(EQLNRDsec(posper,1)) KEYNRD(posper,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(posper,mtnrsrc);
		else filerecord2rsrc(posper,mtnrsrc);
	}

	COPYFIELD(posmstr,poslmst,"LOCATION IDENTIFICATION");
	if(EQLNRDsec(poslmst,1)) KEYNRD(poslmst,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(poslmst,mtnrsrc);
		else filerecord2rsrc(poslmst,mtnrsrc);
	}

	COPYFIELD(posmstr,empadef,"GROSS IDENTIFICATION");
	if(EQLNRDsec(empadef,1)) KEYNRD(empadef,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(empadef,mtnrsrc);
		else filerecord2rsrc(empadef,mtnrsrc);
	}

	COPYFIELD(posmstr,posgrs,"GROSS IDENTIFICATION");
	if(EQLNRDsec(posgrs,1)) KEYNRD(posgrs,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(posgrs,mtnrsrc);
		else filerecord2rsrc(posgrs,mtnrsrc);
	}

	COPYFIELD(posmstr,poscls,"CLASS IDENTIFICATION");
	if(EQLNRDsec(poscls,1)) KEYNRD(poscls,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(poscls,mtnrsrc);
		else filerecord2rsrc(poscls,mtnrsrc);
	}

	COPYFIELD(posmstr,posdedc,"DEDUCTION CLASS IDENTIFICATION");
	if(EQLNRDsec(posdedc,1)) KEYNRD(posdedc,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(posdedc,mtnrsrc);
		else filerecord2rsrc(posdedc,mtnrsrc);
	}

	COPYFIELD(posmstr,possmst,"SERVICE AREA IDENTIFICATION");
	if(EQLNRDsec(possmst,1)) KEYNRD(possmst,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(possmst,mtnrsrc);
		else filerecord2rsrc(possmst,mtnrsrc);
	}

	COPYFIELD(posmstr,posngms,"NEGOTIATING GROUP IDENTIFICATION");
	if(EQLNRDsec(posngms,1)) KEYNRD(posngms,1);
	if(doloops)
	{
		if(update) updatefilerecord2rsrc(posngms,mtnrsrc);
		else filerecord2rsrc(posngms,mtnrsrc);
	}
}
