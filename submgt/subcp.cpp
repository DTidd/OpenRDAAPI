/* subcsub.c - Substitution Managment's Contact Substitutes */
#ifndef WIN32
#define __NAM__ "subcp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subcp.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
#include <rptgen.hpp>
/*CPP_OPART subcp1 */
/*ADDLIB rpt */
/*ADDLIB add */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="SUBMGT";
MakeBrowseList *mbl_empabtr;
APPlib *makefieldvallist(void);
RDArsrc *mainrsrc=NULL;
short empabtr=(-1),empadef=(-1),empabs=(-1);
short prsnnl=(-1),jobmst=(-1);
short posmstr=(-1),posper=(-1),posgrs=(-1),poscls=(-1),posdedc=(-1);
short possmst=(-1),posngms=(-1),poslmst=(-1);
short subctact=(-1),subwrk=(-1),submst=(-1),subemp=(-1),subpos=(-1);
short subloc=(-1),subcls=(-1),subprsnnl=(-1),subprsnnl2=(-1);
short subjobmst=(-1),subposmstr=(-1),subposlmst=(-1);
short subposper=(-1),subposgrs=(-1),subposdedc=(-1);
short subpossmst=(-1),subposngms=(-1);
void getsupportingrecords(RDArsrc *,short,short);
void getsupportingrecords2(RDArsrc *);
RDAfinmgt *fmgt=NULL;
int SUBCTACT_TRANSACTION_COUNT=0;
MaintainMaster *MTNMASTER=NULL;
void quit_record(RDArsrc *);
static void makesubvalues(void);
static void getempabtr(void);
static void freesubvalues(void);
static void loadvalue(RDArsrc *,char *);
char *select_expression=NULL;
RDAvirtual *csortsequence=NULL;
static char *reportname=NULL,**run_type=NULL;
static void empabtrm(short,void *);
static void doexit(MakeBrowseList *);
static void okfunc(MakeBrowseList *,void *);
static short selectfunction(MakeBrowseList *);
static void select_subs2(RDArsrc *,void *);
static void select_subs_test(RDArsrc *);
extern void select_subs(RDArsrc *);
static void browse_positions(RDArsrc *),okfunc_position(MakeBrowseList *,void *);
static void doexit_position(MakeBrowseList *),getpositionid(RDArsrc *);
static short select_position(void);

APPlib *makefieldvallist(void)
{
	APPlib *list=NULL,*virflist=NULL;
	RDArsrc *tmprsrc=NULL;
	short dumb=0,addvf=FALSE;
	char *libx=NULL;
	int y,x,z;
	DFincvir *i,*a;
	DFvirtual *d;

	list=APPlibNEW();
	AddFileFields(list,subprsnnl);
	AddFileFields(list,submst);
	AddFileFields(list,subemp);
	AddFileFields(list,subpos);
	AddFileFields(list,subloc);
	AddFileFields(list,subposmstr);
	AddFileFields(list,subposlmst);
	AddFileFields(list,subposgrs);
	AddFileFields(list,subcls);
	AddFileFields(list,subposdedc);
	AddFileFields(list,subposper);
	AddFileFields(list,subpossmst);
	AddFileFields(list,subposngms);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dumb);
	addDFincvir(tmprsrc,module,"SUBMSTR",NULL,dumb);
	addDFincvir(tmprsrc,module,"SUBPOS",NULL,dumb);
	addDFincvir(tmprsrc,module,"SUBEMP",NULL,dumb);
	addDFincvir(tmprsrc,module,"SUBLOC",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSPER",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSLMST",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRS",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSDEDC",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSCLS",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSSMST",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSNGMS",NULL,dumb);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/SUBMGT.VIR",CURRENTDIRECTORY);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(list,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	loadglobals(list);
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
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
static void select_subs_test(RDArsrc *rsrc)
{
	char deleteflag=TRUE,*warnmessage=NULL;

	if(EQLNRDsec(empabtr,1))
	{
		KEYNRD(empabtr,1);
		warnmessage=Rmalloc(230);
		sprintf(warnmessage,"This is not a valid record selected from the [EMPABS][EMPABTR] data file.  Please select a valid Employee Job Absentee record before selecting Substitutes.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EMPLOYEE JOB ABSENTEE RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return;
	} else {
		FINDFLDGETCHAR(empabtr,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			warnmessage=Rmalloc(255);
			sprintf(warnmessage,"This is not a valid record selected from the [EMPABS][EMPABTR] data file.  It exist in the file but is in a Deleted state presently.  Please select a valid Employee Job Absentee record before selecting Substitutes.");
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED EMPLOYEE JOB ABSENTEE RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		} else {
			FINDFLDGETCHAR(empabtr,"SUBSTITUTE FINALED",&deleteflag);
			if(deleteflag)
			{
				warnmessage=Rmalloc(255);
				sprintf(warnmessage,"This is not a valid record selected from the [EMPABS][EMPABTR] data file.  It exist in the file but is presently Substitue Finaled.  Please select a valid Employee Job Absentee record before selecting Substitutes.");
				WARNINGDIALOG("WARNING DIALOG SCREEN","SUBSTITUE FINALED RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			}
		}
	}
	select_subs(rsrc);
}
static void select_subs2(RDArsrc *rsrc,void *targetkey)
{
	if(!EQLNRDKEYsec(empabtr,1,targetkey))
	{
		getsupportingrecords2(rsrc);
		select_subs(rsrc);
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
				select_subs2(blist->mainrsrc,targetkey);
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
	if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
	freesubvalues();
	if(empabtr!=(-1)) { CLSNRD(empabtr); empabtr=(-1); }
	if(empabs!=(-1)) { CLSNRD(empabs); empabs=(-1); }
	if(empadef!=(-1)) { CLSNRD(empadef); empadef=(-1); }
	if(subctact!=(-1)) { CLSNRD(subctact); subctact=(-1); }
	if(subwrk!=(-1)) { CLSNRD(subwrk); subwrk=(-1); }
	if(submst!=(-1)) { CLSNRD(submst); submst=(-1); }
	if(subemp!=(-1)) { CLSNRD(subemp); subemp=(-1); }
	if(subpos!=(-1)) { CLSNRD(subpos); subpos=(-1); }
	if(subloc!=(-1)) { CLSNRD(subloc); subloc=(-1); }
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
	if(subprsnnl!=(-1)) { CLSNRD(subprsnnl); subprsnnl=(-1); }
	if(subprsnnl2!=(-1)) { CLSNRD(subprsnnl2); subprsnnl2=(-1); }
	if(subjobmst!=(-1)) { CLSNRD(subjobmst); subjobmst=(-1); }
	if(subcls!=(-1)) { CLSNRD(subcls); subcls=(-1); }
	if(subposmstr!=(-1)) { CLSNRD(subposmstr); subposmstr=(-1); }
	if(subposlmst!=(-1)) { CLSNRD(subposlmst); subposlmst=(-1); }
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(SUBCTACT_TRANSACTION_COUNT>0)
	{
		RUNREPORT(module,reportname,NULL,NULL,FALSE,TRUE,NULL);
	}
	if(reportname!=NULL) Rfree(reportname);
	ShutdownSubsystems();
	exit(0);
}
static void freesubvalues(void)
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
	char *perid=NULL,replace=FALSE;
	char subfinaled=TRUE,deleteflag=TRUE,suv=FALSE;
	int transno=0;
	double sub_units_unass=0.0;

	FINDFLDGETCHAR(empabtr,"SUBSTITUTE FINALED",&subfinaled);
	FINDFLDGETCHAR(empabtr,"DELETEFLAG",&deleteflag);
	FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units_unass);
	if(!subfinaled && !deleteflag && sub_units_unass>0.0)
	{
		FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&perid);
		FINDFLDSETSTRING(empabs,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDGETINT(empabtr,"TRANSACTION NUMBER",&transno);
		FINDFLDSETINT(empabs,"TRANSACTION NUMBER",transno);
		if(!EQLNRDsec(empabs,1))
		{
			FINDFLDGETCHAR(empabs,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(empabs,"REPLACEMENT WORKER",&replace);
			FINDFLDGETCHAR(empabs,"SOURCE USER VERIFIED",&suv);
			if(!deleteflag && replace && suv)
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

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	addDFincvir(mainrsrc,module,"SUBMSTR",NULL,submst);
	addDFincvir(mainrsrc,module,"SUBEMP",NULL,subemp);
	addDFincvir(mainrsrc,module,"SUBLOC",NULL,subloc);
	addDFincvir(mainrsrc,module,"SUBPOS",NULL,subpos);
	addDFincvir(mainrsrc,"PAYROLL","PAYJMST",NULL,subjobmst);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,subposmstr);
	addDFincvir(mainrsrc,"POSTRK","POSLMST",NULL,subposlmst);
	addDFincvir(mainrsrc,"POSTRK","POSPER",NULL,subposper);
	addDFincvir(mainrsrc,"POSTRK","POSGRS",NULL,subposgrs);
	addDFincvir(mainrsrc,"POSTRK","POSDEDC",NULL,subposdedc);
	addDFincvir(mainrsrc,"POSTRK","POSCLS",NULL,subcls);
	addDFincvir(mainrsrc,"POSTRK","POSSMST",NULL,subpossmst);
	addDFincvir(mainrsrc,"POSTRK","POSNGMS",NULL,subposngms);
	file2rangersrc(submst,mainrsrc);
	file2rangersrc(subemp,mainrsrc);
	file2rangersrc(subloc,mainrsrc);
	file2rangersrc(subpos,mainrsrc);
	file2rangersrc(subjobmst,mainrsrc);
	file2rangersrc(subposlmst,mainrsrc);
	file2rangersrc(subposper,mainrsrc);
	file2rangersrc(subposgrs,mainrsrc);
	file2rangersrc(subcls,mainrsrc);
	file2rangersrc(subposdedc,mainrsrc);
	file2rangersrc(subpossmst,mainrsrc);
	file2rangersrc(subposngms,mainrsrc);
	file2rsrc(subposmstr,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[POSMSTR][POSITION IDENTIFICATION]",TRUE);
	FINDRSCSETFUNC(mainrsrc,"[POSMSTR][POSITION IDENTIFICATION]",getpositionid,NULL);
	addbtnrsrc(mainrsrc,"BROWSE POSITIONS",TRUE,browse_positions,NULL);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	SCRNvirtual2rangersrc(mainrsrc);
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
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ZERNRD(empabtr);
	addlstrsrc(mainrsrc,"RUN FUNCTION",0,TRUE,NULL,2,&run_type,NULL);
	addsctrsrc(mainrsrc,"SUBSTITUTE EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(mainrsrc,"SUBSTITUTE LOAD VALUE",TRUE,loadvalue,"SUBSTITUTE EXPRESSION");
	addbtnrsrc(mainrsrc,"SUBSTITUTE LOAD OPERATOR",TRUE,loadoperand,"SUBSTITUTE EXPRESSION");
	addbtnrsrc(mainrsrc,"SUBSTITUTE LOAD GROUPER",TRUE,loadgrouper,"SUBSTITUTE EXPRESSION");
	addbtnrsrc(mainrsrc,"SUBSTITUTE LOAD CONTROL",TRUE,loadcontrol,"SUBSTITUTE EXPRESSION");
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	mbl_empabtr=ADVSbrowse(empabtr,1,mainrsrc,searchrsrc,definelist,
		NULL,NULL,selectfunction,doexit,okfunc,
		NULL,NULL,TRUE,NULL,0,0,0,TRUE);
}
static void makesubvalues(void)
{
	run_type=Rmalloc(2*sizeof(char *));
	run_type[0]=stralloc("View Employee Job Absentee");
	run_type[1]=stralloc("Contact Substitute");
}
static void getempabtr(void)
{
	MTNMASTER=MaintainMasterNew(module,"EMPLOYEE ABSENTEE CONTACT SUBS");
	makesubvalues();
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","EMPLOYEE ABSENTEE CONTACT SUBSTITUTES",module);
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
	if(InitializeSubsystems(argc,argv,module,"CONTACT SUBS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else {
		reportname=stralloc("ECAUDIT");
	}
	if((empabtr=APPOPNNRD("EMPABS","EMPABTR",TRUE,TRUE))==(-1)) return;
	if((empabs=APPOPNNRD("EMPABS","EMPABS",TRUE,TRUE))==(-1)) return;
	if((empadef=APPOPNNRD("EMPABS","EMPADEF",TRUE,FALSE))==(-1)) return;
	if((subctact=APPOPNNRD(module,"SUBCTACT",TRUE,TRUE))==(-1)) return;
	if((subwrk=APPOPNNRD(module,"SUBWKMAS",TRUE,TRUE))==(-1)) return;
	if((submst=APPOPNNRD(module,"SUBMASTR",TRUE,TRUE))==(-1)) return;
	if((subemp=APPOPNNRD(module,"SUBEMP",TRUE,TRUE))==(-1)) return;
	if((subpos=APPOPNNRD(module,"SUBPOS",TRUE,TRUE))==(-1)) return;
	if((subloc=APPOPNNRD(module,"SUBLOC",TRUE,TRUE))==(-1)) return;
	if((prsnnl=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((subprsnnl=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((subprsnnl2=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((jobmst=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((poslmst=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	if((posper=APPOPNNRD("POSTRK","POSPER",TRUE,FALSE))==(-1)) return;
	if((posgrs=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((poscls=APPOPNNRD("POSTRK","POSCLS",TRUE,FALSE))==(-1)) return;
	if((posdedc=APPOPNNRD("POSTRK","POSDEDC",TRUE,FALSE))==(-1)) return;
	if((possmst=APPOPNNRD("POSTRK","POSSMST",TRUE,FALSE))==(-1)) return;
	if((posngms=APPOPNNRD("POSTRK","POSNGMS",TRUE,FALSE))==(-1)) return;
	if((subjobmst=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((subposmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((subposlmst=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	if((subposper=APPOPNNRD("POSTRK","POSPER",TRUE,FALSE))==(-1)) return;
	if((subposgrs=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((subposdedc=APPOPNNRD("POSTRK","POSDEDC",TRUE,FALSE))==(-1)) return;
	if((subpossmst=APPOPNNRD("POSTRK","POSSMST",TRUE,FALSE))==(-1)) return;
	if((subposngms=APPOPNNRD("POSTRK","POSNGMS",TRUE,FALSE))==(-1)) return;
	if((subcls=APPOPNNRD("POSTRK","POSCLS",TRUE,FALSE))==(-1)) return;
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
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getpositionid(RDArsrc *mtnrsrc)
{
	char *posid=NULL;

	readwidget(mtnrsrc,"[POSMSTR][POSITION IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[POSMSTR][POSITION IDENTIFICATION]",&posid);
	FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",posid);
	if(EQLNRDsec(posmstr,1)) KEYNRD(posmstr,1);
	updatefilerecord2rsrc(posmstr,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void doexit_position(MakeBrowseList *blist)
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
}
static void okfunc_position(MakeBrowseList *b,void *targetkey)
{
	char *posid=NULL; 

	ZERNRD(subposmstr);
	if(EQLNRDKEYsec(subposmstr,1,targetkey))
	{
		KEYNRD(subposmstr,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(subposmstr,"POSITION IDENTIFICATION",&posid);
	}
	filerecord2rsrc(subposmstr,b->funcargs);
	FINDRSCSETSTRING(b->funcargs,"[POSMSTR][POSITION IDENTIFICATION]",posid);
	computeallSCRNvirtuals(b->funcargs);
	updateallrsrc(b->funcargs);
	if(posid!=NULL) Rfree(posid);
}
static short select_position(void)
{
	return TRUE;
}
static void browse_positions(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	mainrsrc=RDArsrcNEW(module,"SELECT POSITION BROWSE");
	searchrsrc=RDArsrcNEW(module,"SELECT POSITION SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"SELECT POSITION DEFINE LIST");
	ZERNRD(subposmstr);
	addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,subposmstr);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,subposmstr);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ADVbrowse(subposmstr,1,mainrsrc,searchrsrc,definelist,
		NULL,select_position,doexit_position,okfunc_position,NULL,
		mtnrsrc,TRUE,NULL,2,2,2,0);
}
static void empabtrm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;

	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	if(mbl_empabtr==NULL)
	{
		addDFincvir(mtnrsrc,module,"SUBMSTR",NULL,submst);
		addDFincvir(mtnrsrc,module,"SUBEMP",NULL,subemp);
		addDFincvir(mtnrsrc,module,"SUBLOC",NULL,subloc);
		addDFincvir(mtnrsrc,module,"SUBPOS",NULL,subpos);
		addDFincvir(mtnrsrc,"PAYROLL","PAYJMST",NULL,subjobmst);
		addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,subposmstr);
		addDFincvir(mtnrsrc,"POSTRK","POSLMST",NULL,subposlmst);
		addDFincvir(mtnrsrc,"POSTRK","POSPER",NULL,subposper);
		addDFincvir(mtnrsrc,"POSTRK","POSGRS",NULL,subposgrs);
		addDFincvir(mtnrsrc,"POSTRK","POSDEDC",NULL,subposdedc);
		addDFincvir(mtnrsrc,"POSTRK","POSCLS",NULL,subcls);
		addDFincvir(mtnrsrc,"POSTRK","POSSMST",NULL,subpossmst);
		addDFincvir(mtnrsrc,"POSTRK","POSNGMS",NULL,subposngms);
		addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posmstr);
		file2rangersrc(submst,mtnrsrc);
		file2rangersrc(subemp,mtnrsrc);
		file2rangersrc(subloc,mtnrsrc);
		file2rangersrc(subpos,mtnrsrc);
		file2rangersrc(subjobmst,mtnrsrc);
		file2rangersrc(subposlmst,mtnrsrc);
		file2rangersrc(subposper,mtnrsrc);
		file2rangersrc(subposgrs,mtnrsrc);
		file2rangersrc(subcls,mtnrsrc);
		file2rangersrc(subposdedc,mtnrsrc);
		file2rangersrc(subpossmst,mtnrsrc);
		file2rangersrc(subposngms,mtnrsrc);
		file2rsrc(subposmstr,mtnrsrc,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[POSMSTR][POSITION IDENTIFICATION]",TRUE);
		FINDRSCSETFUNC(mtnrsrc,"[POSMSTR][POSITION IDENTIFICATION]",getpositionid,NULL);
		addbtnrsrc(mtnrsrc,"BROWSE POSITIONS",TRUE,browse_positions,NULL);
		GET_ALL_SCREEN_VIRTUALS(mtnrsrc,2);
		SCRNvirtual2rangersrc(mtnrsrc);
		addsctrsrc(mtnrsrc,"SUBSTITUTE EXPRESSION",0,NULL,TRUE);
		addbtnrsrc(mtnrsrc,"SUBSTITUTE LOAD VALUE",TRUE,loadvalue,"SUBSTITUTE EXPRESSION");
		addbtnrsrc(mtnrsrc,"SUBSTITUTE LOAD OPERATOR",TRUE,loadoperand,"SUBSTITUTE EXPRESSION");
		addbtnrsrc(mtnrsrc,"SUBSTITUTE LOAD GROUPER",TRUE,loadgrouper,"SUBSTITUTE EXPRESSION");
		addbtnrsrc(mtnrsrc,"SUBSTITUTE LOAD CONTROL",TRUE,loadcontrol,"SUBSTITUTE EXPRESSION");
	}
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
	if(submst!=(-1)) file2rsrc(submst,mtnrsrc,FALSE);
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
		addbtnrsrc(mtnrsrc,"CONTACT SUBSTITUTES",TRUE,select_subs_test,NULL);
	}
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
		freesubvalues();
		if(empabtr!=(-1)) { CLSNRD(empabtr); empabtr=(-1); }
		if(empabs!=(-1)) { CLSNRD(empabs); empabs=(-1); }
		if(empadef!=(-1)) { CLSNRD(empadef); empadef=(-1); }
		if(subctact!=(-1)) { CLSNRD(subctact); subctact=(-1); }
		if(subwrk!=(-1)) { CLSNRD(subwrk); subwrk=(-1); }
		if(submst!=(-1)) { CLSNRD(submst); submst=(-1); }
		if(subemp!=(-1)) { CLSNRD(subemp); subemp=(-1); }
		if(subpos!=(-1)) { CLSNRD(subpos); subpos=(-1); }
		if(subloc!=(-1)) { CLSNRD(subloc); subloc=(-1); }
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
		if(subprsnnl!=(-1)) { CLSNRD(subprsnnl); subprsnnl=(-1); }
		if(subprsnnl2!=(-1)) { CLSNRD(subprsnnl2); subprsnnl2=(-1); }
		if(subjobmst!=(-1)) { CLSNRD(subjobmst); subjobmst=(-1); }
		if(subcls!=(-1)) { CLSNRD(subcls); subcls=(-1); }
		if(subposmstr!=(-1)) { CLSNRD(subposmstr); subposmstr=(-1); }
		if(subposlmst!=(-1)) { CLSNRD(subposlmst); subposlmst=(-1); }
		if(subposper!=(-1)) { CLSNRD(subposper); subposper=(-1); }
		if(subposgrs!=(-1)) { CLSNRD(subposgrs); subposgrs=(-1); }
		if(subposdedc!=(-1)) { CLSNRD(subposdedc); subposdedc=(-1); }
		if(subpossmst!=(-1)) { CLSNRD(subpossmst); subpossmst=(-1); }
		if(subposngms!=(-1)) { CLSNRD(subposngms); subposngms=(-1); }
		if(fmgt!=NULL) free_finmgt(fmgt);
		if(SUBCTACT_TRANSACTION_COUNT>0)
		{
			RUNREPORT(module,reportname,NULL,NULL,FALSE,TRUE,NULL);
		}
		if(reportname!=NULL) Rfree(reportname);
		ShutdownSubsystems();
		exit(0);
	}
}
void getsupportingrecords2(RDArsrc *mtnrsrc)
{
	COPYFIELD(empabtr,empabs,"PERSONNEL IDENTIFICATION");
	COPYFIELD(empabtr,empabs,"TRANSACTION NUMBER");
	if(EQLNRDsec(empabs,1)) KEYNRD(empabs,1);
	COPYFIELD(empabtr,prsnnl,"PERSONNEL IDENTIFICATION");
	if(EQLNRDsec(prsnnl,1)) KEYNRD(prsnnl,1);
	COPYFIELD(empabtr,jobmst,"PERSONNEL IDENTIFICATION");
	COPYFIELD(empabtr,jobmst,"JOB NUMBER");
	if(EQLNRDsec(jobmst,1)) KEYNRD(jobmst,1);
	COPYFIELD(jobmst,posmstr,"POSITION IDENTIFICATION");
	if(EQLNRDsec(posmstr,1)) KEYNRD(posmstr,1);
	COPYFIELD(posmstr,posper,"POSITION IDENTIFICATION");
	COPYFIELD(jobmst,posper,"PERSONNEL IDENTIFICATION");
	COPYFIELD(jobmst,posper,"JOB NUMBER");
	if(EQLNRDsec(posper,1)) KEYNRD(posper,1);
	COPYFIELD(posmstr,poslmst,"LOCATION IDENTIFICATION");
	if(EQLNRDsec(poslmst,1)) KEYNRD(poslmst,1);
	COPYFIELD(posmstr,empadef,"GROSS IDENTIFICATION");
	if(EQLNRDsec(empadef,1)) KEYNRD(empadef,1);
	COPYFIELD(posmstr,posgrs,"GROSS IDENTIFICATION");
	if(EQLNRDsec(posgrs,1)) KEYNRD(posgrs,1);
	COPYFIELD(posmstr,poscls,"CLASS IDENTIFICATION");
	if(EQLNRDsec(poscls,1)) KEYNRD(poscls,1);
	COPYFIELD(posmstr,posdedc,"DEDUCTION CLASS IDENTIFICATION");
	if(EQLNRDsec(posdedc,1)) KEYNRD(posdedc,1);
	COPYFIELD(posmstr,possmst,"SERVICE AREA IDENTIFICATION");
	if(EQLNRDsec(possmst,1)) KEYNRD(possmst,1);
	COPYFIELD(posmstr,posngms,"NEGOTIATING GROUP IDENTIFICATION");
	if(EQLNRDsec(posngms,1)) KEYNRD(posngms,1);
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
 		filerecord2rsrc(prsnnl,mtnrsrc);
	}
	COPYFIELD(empabtr,jobmst,"PERSONNEL IDENTIFICATION");
	COPYFIELD(empabtr,jobmst,"JOB NUMBER");
	if(EQLNRDsec(jobmst,1)) KEYNRD(jobmst,1);
	if(update) updatefilerecord2rsrc(jobmst,mtnrsrc);
	else filerecord2rsrc(jobmst,mtnrsrc);
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
