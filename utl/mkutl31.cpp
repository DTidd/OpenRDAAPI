/* UTILITIES's - Make (PURGE-MENUITEM) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_PURGE_MENUITEM()
{
	int x=0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	short engine=0;
/*
	RDApval *pval=NULL,*pvals=NULL;
	int y,z;
	short longest=132;
	int numpvars=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	char *defdir=NULL;
*/
	int imageoff=0;
	RDAimage *image=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;

	char *temp1=NULL,*temp2=NULL,*temp3=NULL;



	memset(stemp,0,101);
	rptname=Rmalloc(14+1);
	sprintf(rptname,"PURGE-MENUITEM");
	t=Rmalloc(33+1);
	sprintf(t,"Purge Utilities Menuitem Database");
	desc=Rmalloc(3+1);
	sprintf(desc,"");
#if defined(USE_MYISAM) 
	engine=NRDMyIsamEngine;
#else /* USE_MYISAM */
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
#endif /* USE_MYISAM */
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* UTILITIES MENUITEM SEARCH FILE */

	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"UTILITIES","MENUITEM",0,NULL,NULL,"MENUITEM KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"ITEM NUMBER","");
	}
	if(t!=NULL) Rfree(t);











	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[UTILITIES][MENUITEM][DROP DOWN LIST]",0,"[UTILITIES][MENUITEM][DROP DOWN LIST]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][ITEM NUMBER]",0,"[UTILITIES][MENUITEM][ITEM NUMBER]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][MODULE NAME]",0,"[UTILITIES][MENUITEM][MODULE NAME]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][PARENT]",0,"[UTILITIES][MENUITEM][PARENT]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][RANK]",0,"[UTILITIES][MENUITEM][RANK]");
		addimagepval(image,1,"TRUE",0,"[UTILITIES][MENUITEM][DELETEFLAG]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+9+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [UTILITIES] Report [PURGE-MENUITEM], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void SCN_PURGE_MENUITEM_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","PURGE-MENUITEM RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","PURGE UTILITIES MENUITEM DATABASE","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z ITEM NUMBER]","Range on Z Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z ITEM NUMBER]","From Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z ITEM NUMBER]","To Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z PARENT]","Range on Z Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z PARENT]","From Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z PARENT]","To Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(27+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [PURGE-MENUITEM RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void RPT_PURGE_MODULE()
{
	int x=0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	short engine=0;
/*
	RDApval *pval=NULL,*pvals=NULL;
	int y,z;
	short longest=132;
	int numpvars=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	char *defdir=NULL;
*/
	int imageoff=0;
	RDAimage *image=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;

	char *temp1=NULL,*temp2=NULL,*temp3=NULL;



	memset(stemp,0,101);
	rptname=Rmalloc(12+1);
	sprintf(rptname,"PURGE-MODULE");
	t=Rmalloc(31+1);
	sprintf(t,"Purge Utilities Module Database");
	desc=Rmalloc(3+1);
	sprintf(desc,"");
#if defined(USE_MYISAM) 
	engine=NRDMyIsamEngine;
#else /* USE_MYISAM */
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
#endif /* USE_MYISAM */
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* UTILITIES MODULE SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MODULE][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"UTILITIES","MODULE",0,NULL,NULL,"MODULE KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"GROUP","");
		addsearchconfld(s,"RANK","");
		addsearchconfld(s,"MODULE NAME","");
	}
	if(t!=NULL) Rfree(t);











	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [UTILITIES][MODULE]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [UTILITIES][MODULE]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[UTILITIES][MODULE][MODULE NAME]",0,"[UTILITIES][MODULE][MODULE NAME]");
		addimagepval(image,0,"[UTILITIES][MODULE][GROUP]",0,"[UTILITIES][MODULE][GROUP]");
		addimagepval(image,0,"[UTILITIES][MODULE][RANK]",0,"[UTILITIES][MODULE][RANK]");
		addimagepval(image,1,"TRUE",0,"[UTILITIES][MODULE][DELETEFLAG]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [UTILITIES][MODULE]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+9+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [UTILITIES] Report [PURGE-MODULE], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void SCN_PURGE_MODULE_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","PURGE-MODULE RANGE SCREEN");
	if(scrn!=NULL)
	{
		/*ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);*/
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","PURGE UTILITIES MODULE DATABASE","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][GROUP]","Range on Group","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][GROUP]","From Group","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][GROUP]","To Group","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][DESCRIPTION]","Range on Description","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][DESCRIPTION]","From Description","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][DESCRIPTION]","To Description","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		/*ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);*/
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(25+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [PURGE-MODULE RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}

static void RPT_SETZPARENTITEM()
{
	int x=0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	short engine=0;
/*
	RDApval *pval=NULL,*pvals=NULL;
	int y,z;
	short longest=132;
	int numpvars=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	char *defdir=NULL;
*/
	int imageoff=0;
	RDAimage *image=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;

	char *temp1=NULL,*temp2=NULL,*temp3=NULL;



	memset(stemp,0,101);
	rptname=Rmalloc(14+1);
	sprintf(rptname,"SETZPARENTITEM");
	t=Rmalloc(3+1);
	sprintf(t,"");
	desc=Rmalloc(3+1);
	sprintf(desc,"");
#if defined(USE_MYISAM) 
	engine=NRDMyIsamEngine;
#else /* USE_MYISAM */
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
#endif /* USE_MYISAM */
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* UTILITIES MENUITEM SEARCH FILE */

	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"UTILITIES","MENUITEM",0,NULL,NULL,"MENUITEM KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"ITEM NUMBER","");
	}
	if(t!=NULL) Rfree(t);






/* VIRTUAL FIELD DEFINITIONS */

/* FACTOR VIRTUAL FIELD */
	temp1=Rmalloc(65+1);
	sprintf(temp1,"%s",
		"IF([FACTOR]=0) THEN RETURN_VALUE=10 ELSE RETURN_VALUE=[FACTOR] FI");
	addvirtuallineADV(tmp,"FACTOR",8,4,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* Z PARENT VIRTUAL FIELD */
	temp1=Rmalloc(43+1);
	sprintf(temp1,"%s",
		"[UTILITIES][MENUITEM][PARENT]*[FACTOR]");
	addvirtuallineADV(tmp,"Z PARENT",8,8,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* Z ITEM NUMBER VIRTUAL FIELD */
	temp1=Rmalloc(48+1);
	sprintf(temp1,"%s",
		"[UTILITIES][MENUITEM][ITEM NUMBER]*[FACTOR]");
	addvirtuallineADV(tmp,"Z ITEM NUMBER",8,8,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[UTILITIES][MENUITEM][ITEM NUMBER]",0,"[UTILITIES][MENUITEM][ITEM NUMBER]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][MODULE NAME]",0,"[UTILITIES][MENUITEM][MODULE NAME]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][DROP DOWN LIST]",0,"[UTILITIES][MENUITEM][DROP DOWN LIST]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][RANK]",0,"[UTILITIES][MENUITEM][RANK]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][PARENT]",0,"[UTILITIES][MENUITEM][PARENT]");
		addimagepval(image,0,"[Z ITEM NUMBER]",0,"[UTILITIES][MENUITEM][Z ITEM NUMBER]");
		addimagepval(image,0,"[Z PARENT]",0,"[UTILITIES][MENUITEM][Z PARENT]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+9+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [UTILITIES] Report [SETZPARENTITEM], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void SCN_SETZPARENTITEM_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","SETZPARENTITEM RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Factor:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FACTOR","Factor:","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z ITEM NUMBER]","Range on Z Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z ITEM NUMBER]","From Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z ITEM NUMBER]","To Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z PARENT]","Range on Z Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z PARENT]","From Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z PARENT]","To Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(27+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [SETZPARENTITEM RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}

static void RPT_SHOWZPARENTITEM()
{
	int x=0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	short engine=0;
/*
	RDApval *pval=NULL,*pvals=NULL;
	int y,z;
	short longest=132;
	int numpvars=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	char *defdir=NULL;
*/
	int imageoff=0;
	RDAimage *image=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;

	char *temp1=NULL,*temp2=NULL,*temp3=NULL;



	memset(stemp,0,101);
	rptname=Rmalloc(15+1);
	sprintf(rptname,"SHOWZPARENTITEM");
	t=Rmalloc(3+1);
	sprintf(t,"");
	desc=Rmalloc(3+1);
	sprintf(desc,"");
#if defined(USE_MYISAM) 
	engine=NRDMyIsamEngine;
#else /* USE_MYISAM */
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
#endif /* USE_MYISAM */
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,TRUE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,0,FALSE,0,NULL,FALSE,0,1,0,pvars,1,1,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* UTILITIES MENUITEM SEARCH FILE */

	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"UTILITIES","MENUITEM",0,NULL,NULL,"MENUITEM KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"ITEM NUMBER","");
	}
	if(t!=NULL) Rfree(t);






/* VIRTUAL FIELD DEFINITIONS */

/* EXECUTED BY VIRTUAL FIELD */
	temp1=Rmalloc(42+1);
	sprintf(temp1,"%s",
		"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",1,60,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* REPORT TITLE VIRTUAL FIELD */
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"\"REPORT NEW PARENT - ITEM COMBINATIONS\"");
	addvirtuallineADV(tmp,"REPORT TITLE",1,60,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);





/* SORT DEFINITIONS */

	addsortline(tmp,0,"[UTILITIES][MENUITEM][MODULE NAME]");
	addsortline(tmp,0,"[UTILITIES][MENUITEM][DROP DOWN LIST]");
	addsortline(tmp,0,"[UTILITIES][MENUITEM][Z PARENT]");
	addsortline(tmp,0,"[UTILITIES][MENUITEM][Z ITEM NUMBER]");


	SetReportDefaultEngine(tmp);


/* CONTROL BREAKS */

	addcontrolline(tmp,"[UTILITIES][MENUITEM][MODULE NAME]",0);
	addcontrolline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST]",0);
	addcontrolline(tmp,"[UTILITIES][MENUITEM][Z PARENT]",0);
	addcontrolline(tmp,"[UTILITIES][MENUITEM][Z ITEM NUMBER]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(5+1);
	sprintf(temp1,"%s",
		"FALSE");
	imageoff=addimageline(tmp,"PAGE HEADER","123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*12@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   PAGE: @RRRRRRR@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[ORGANIZATION NAME]",0,NULL);
		addimagepval(image,0,"[PAGE NO]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   DATE: @LLLLLLL@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[REPORT TITLE]",0,NULL);
		addimagepval(image,0,"[SYSTEM DATE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   TIME: @LLLLLLL@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[EXECUTED BY]",0,NULL);
		addimagepval(image,0,"[SYSTEM TIME]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][MODULE NAME] HEADER","@V@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[UTILITIES][MENUITEM][MODULE NAME]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DROP DOWN LIST]=0)");
	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST] HEADER","     QUERY DROP DOWN@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DROP DOWN LIST]=1)");
	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST] HEADER","     REPORTS DROP DOWN@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DROP DOWN LIST]=2)");
	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST] HEADER","     MAINTENANCE DROP DOWN@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DROP DOWN LIST]=3)");
	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST] HEADER","     SETUP DROP DOWN@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DROP DOWN LIST]=4)");
	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST] HEADER","     OTHER DROP DOWN@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DROP DOWN LIST]=5)");
	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST] HEADER","     PROCESS AREA@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","@RRRRRR @RRRRRR @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @RRRRRR @RR @RRRRRR@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[UTILITIES][MENUITEM][Z PARENT]",0,NULL);
		addimagepval(image,0,"[UTILITIES][MENUITEM][Z ITEM NUMBER]",0,NULL);
		addimagepval(image,0,"[UTILITIES][MENUITEM][NAME]",0,NULL);
		addimagepval(image,0,"[UTILITIES][MENUITEM][PARENT]",0,NULL);
		addimagepval(image,0,"[UTILITIES][MENUITEM][RANK]",0,NULL);
		addimagepval(image,0,"[UTILITIES][MENUITEM][ITEM NUMBER]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][DROP DOWN LIST] FOOTER","@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"PAGE FOOTER","@@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
		addimagepval(image,1,"FORMFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(42+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][MODULE NAME]!=\"\")");
	imageoff=addimageline(tmp,"[UTILITIES][MENUITEM][MODULE NAME] FOOTER","@@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
		addimagepval(image,1,"FORMFEED",0,NULL);
	}


	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+9+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [UTILITIES] Report [SHOWZPARENTITEM], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void SCN_SHOWZPARENTITEM_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","SHOWZPARENTITEM RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z ITEM NUMBER]","Range on Z Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z ITEM NUMBER]","From Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z ITEM NUMBER]","To Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z PARENT]","Range on Z Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z PARENT]","From Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z PARENT]","To Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(28+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [SHOWZPARENTITEM RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}

static void RPT_ZEROZPARENTITEM()
{
	int x=0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	short engine=0;
/*
	RDApval *pval=NULL,*pvals=NULL;
	int y,z;
	short longest=132;
	int numpvars=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	char *defdir=NULL;
*/
	int imageoff=0;
	RDAimage *image=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;

	char *temp1=NULL,*temp2=NULL,*temp3=NULL;



	memset(stemp,0,101);
	rptname=Rmalloc(15+1);
	sprintf(rptname,"ZEROZPARENTITEM");
	t=Rmalloc(3+1);
	sprintf(t,"");
	desc=Rmalloc(3+1);
	sprintf(desc,"");
#if defined(USE_MYISAM) 
	engine=NRDMyIsamEngine;
#else /* USE_MYISAM */
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
#endif /* USE_MYISAM */
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* UTILITIES MENUITEM SEARCH FILE */

	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([UTILITIES][MENUITEM][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"UTILITIES","MENUITEM",0,NULL,NULL,"MENUITEM KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"ITEM NUMBER","");
	}
	if(t!=NULL) Rfree(t);











	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[UTILITIES][MENUITEM][DROP DOWN LIST]",0,"[UTILITIES][MENUITEM][DROP DOWN LIST]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][ITEM NUMBER]",0,"[UTILITIES][MENUITEM][ITEM NUMBER]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][MODULE NAME]",0,"[UTILITIES][MENUITEM][MODULE NAME]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][PARENT]",0,"[UTILITIES][MENUITEM][PARENT]");
		addimagepval(image,0,"[UTILITIES][MENUITEM][RANK]",0,"[UTILITIES][MENUITEM][RANK]");
		addimagepval(image,1,"0",0,"[UTILITIES][MENUITEM][Z ITEM NUMBER]");
		addimagepval(image,1,"0",0,"[UTILITIES][MENUITEM][Z PARENT]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [UTILITIES][MENUITEM]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+9+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [UTILITIES] Report [ZEROZPARENTITEM], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void SCN_ZEROZPARENTITEM_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","ZEROZPARENTITEM RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z ITEM NUMBER]","Range on Z Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z ITEM NUMBER]","From Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z ITEM NUMBER]","To Z Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][Z PARENT]","Range on Z Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][Z PARENT]","From Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][Z PARENT]","To Z Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(28+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [ZEROZPARENTITEM RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
void MENUITEMRPTS()
{
	RPT_PURGE_MENUITEM();
	SCN_PURGE_MENUITEM_RANGE_SCREEN();
	RPT_PURGE_MODULE();
	SCN_PURGE_MODULE_RANGE_SCREEN();
	RPT_SETZPARENTITEM();
	SCN_SETZPARENTITEM_RANGE_SCREEN();
	RPT_SHOWZPARENTITEM();
	SCN_SHOWZPARENTITEM_RANGE_SCREEN();
	RPT_ZEROZPARENTITEM();
	SCN_ZEROZPARENTITEM_RANGE_SCREEN();
}
