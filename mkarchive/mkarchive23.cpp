/* ARCHIVE's - Make (AUTOWRKGRPDEPT) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_AUTOWRKGRPDEPT()
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
	sprintf(rptname,"AUTOWRKGRPDEPT");
	t=Rmalloc(47+1);
	sprintf(t,"Create Work Groups for Automatic Report Capture");
	desc=Rmalloc(88+1);
	sprintf(desc,"This output device creates the basic Work Group specifications based on Module Security.");
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
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

/* SECURITY USERS SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","USERS",0,NULL,NULL,"USERS KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","");
	}


/* SECURITY DEPARTMENT SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([SECURITY][DEPARTMENT][SECURITY]=TRUE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","DEPARTMENT",2,"SECURITY","USERS","DEPARTMENT KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEPARTMENT","[USERS][DEPARTMENT]");
	}
	if(t!=NULL) Rfree(t);


/* SECURITY MODSEC SEARCH FILE */

	temp1=Rmalloc(114+1);
	sprintf(temp1,"%s%s",
		"([SECURITY][MODSEC][DELETEFLAG]=FALSE) AND ([SECURITY][MODSEC][USER IDENTIFICATION]=[SECURITY][USERS",
		"][DEPARTMENT])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","MODSEC",16,"SECURITY","USERS","MODSEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[USERS][DEPARTMENT]");
		addsearchconfld(s,"MODULE NAME","");
	}
	if(t!=NULL) Rfree(t);


/* ARCHIVE ARCHWORK SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHWORK",1,"SECURITY","MODSEC","ARCHWORK KEY 1",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[MODSEC][MODULE NAME]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* NAME VIRTUAL FIELD */
	temp1=Rmalloc(61+1);
	sprintf(temp1,"%s",
		"[SECURITY][MODSEC][MODULE NAME]+\" AUTOMATIC REPORT CAPTURE\"");
	addvirtuallineADV(tmp,"NAME",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* CREATE WORKGROUP VIRTUAL FIELD */
	temp1=Rmalloc(185+1);
	sprintf(temp1,"%s%s",
		"IF(([ARCHIVE][ARCHWORK][USER IDENTIFICATION]=\"\") OR ([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE) OR ([A",
		"RCHIVE][ARCHWORK][DIRECTORY]=\"\")) THEN RETURN_VALUE=TRUE ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"CREATE WORKGROUP",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);





/* SORT DEFINITIONS */

	addsortline(tmp,0,"[SECURITY][MODSEC][MODULE NAME]");
	addsortline(tmp,0,"[SECURITY][USERS][USER IDENTIFICATION]");


	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"LAST USER NAME",3,"[SECURITY][USERS][USER NAME]",0);
	addaccumline(tmp,"LAST NAME",3,"[NAME]",0);
	addaccumline(tmp,"LAST CREATE WORKGROUP",3,"[CREATE WORKGROUP]",0);
/* CONTROL BREAKS */

	addcontrolline(tmp,"[SECURITY][MODSEC][MODULE NAME]",0);
	addcontrolline(tmp,"[SECURITY][USERS][USER IDENTIFICATION]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [ARCHIVE][ARCHWORK]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);

	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(25+1);
	sprintf(temp1,"%s",
		"([CREATE WORKGROUP]=TRUE)");
	imageoff=addimageline(tmp,"[SECURITY][MODSEC][MODULE NAME] HEADER","WRITE [ARCHIVE][ARCHWORK]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][MODSEC][MODULE NAME]",0,"[ARCHIVE][ARCHWORK][WORK GROUP]");
		addimagepval(image,0,"[LOGIN IDENTIFICATION]",0,"[ARCHIVE][ARCHWORK][USER IDENTIFICATION]");
		addimagepval(image,0,"[SECURITY][MODSEC][MODULE NAME]",0,"[ARCHIVE][ARCHWORK][DIRECTORY]");
		addimagepval(image,1,"FALSE",0,"[ARCHIVE][ARCHWORK][DELETEFLAG]");
		addimagepval(image,0,"[NAME]",0,"[ARCHIVE][ARCHWORK][NAME]");
	}


	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"FALSE",0,"[ARCHIVE][ARCHUSER][DELETEFLAG]");
		addimagepval(image,1,"TRUE",0,"[ARCHIVE][ARCHUSER][READ]");
		addimagepval(image,1,"TRUE",0,"[ARCHIVE][ARCHUSER][WRITE]");
		addimagepval(image,0,"[SECURITY][USERS][USER IDENTIFICATION]",0,"[ARCHIVE][ARCHUSER][USER IDENTIFICATION]");
		addimagepval(image,0,"[SECURITY][MODSEC][MODULE NAME]",0,"[ARCHIVE][ARCHUSER][WORK GROUP]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [ARCHIVE][ARCHWORK]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+7+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [ARCHIVE] Report [AUTOWRKGRPDEPT], Can Not Save Report!");
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
static void SCN_AUTOWRKGRPDEPT_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE.110706","AUTOWRKGRPDEPT RANGE SCREEN");
	if(scrn!=NULL)
	{
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
		ADVaddwdgt(scrn,9,"RANGE ON [DEPARTMENT][DEPARTMENT]","Range on Department","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DEPARTMENT][DEPARTMENT]","From Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DEPARTMENT][DEPARTMENT]","To Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DEPARTMENT][SUPERVISOR]","Range on Supervisor","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DEPARTMENT][SUPERVISOR]","From Supervisor","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DEPARTMENT][SUPERVISOR]","To Supervisor","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DEPARTMENT][SECURITY] FALSE","Select when Security False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DEPARTMENT][SECURITY] TRUE","Select when Security True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(27+14+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE.110706] Screen [AUTOWRKGRPDEPT RANGE SCREEN], Can Not Save Maintain Master!");
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
static void MNU_ARCHIVE_AUTOWRKGRPDEPT()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE AUTOWRKGRPDEPT","doreport.EXT ARCHIVE \"AUTOWRKGRPDEPT\"");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}

static void RPT_AUTOWRKGRPDEPTONE()
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
	rptname=Rmalloc(17+1);
	sprintf(rptname,"AUTOWRKGRPDEPTONE");
	t=Rmalloc(47+1);
	sprintf(t,"Create Work Groups for Automatic Report Capture");
	desc=Rmalloc(88+1);
	sprintf(desc,"This output device creates the basic Work Group specifications based on Module Security.");
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
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",FALSE,2,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

/* SELECT RECORDS EXPRESSION */

	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"([USER IDENTIFICATION]!=\"\")");
	tmp->select=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);




/* SEARCH FILES DEFINITIONS */

/* SECURITY USERS SEARCH FILE */

	temp1=Rmalloc(104+1);
	sprintf(temp1,"%s%s",
		"([SECURITY][USERS][USER IDENTIFICATION]=[USER IDENTIFICATION]) AND ([SECURITY][USERS][DELETEFLAG]=FA",
		"LSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","USERS",0,NULL,NULL,"USERS KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[USER IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);


/* SECURITY DEPARTMENT SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([SECURITY][DEPARTMENT][SECURITY]=TRUE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","DEPARTMENT",2,"SECURITY","USERS","DEPARTMENT KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEPARTMENT","[USERS][DEPARTMENT]");
	}
	if(t!=NULL) Rfree(t);


/* SECURITY MODSEC SEARCH FILE */

	temp1=Rmalloc(114+1);
	sprintf(temp1,"%s%s",
		"([SECURITY][MODSEC][DELETEFLAG]=FALSE) AND ([SECURITY][MODSEC][USER IDENTIFICATION]=[SECURITY][USERS",
		"][DEPARTMENT])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","MODSEC",16,"SECURITY","USERS","MODSEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[USERS][DEPARTMENT]");
		addsearchconfld(s,"MODULE NAME","");
	}
	if(t!=NULL) Rfree(t);


/* ARCHIVE ARCHWORK SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHWORK",1,"SECURITY","MODSEC","ARCHWORK KEY 1",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[MODSEC][MODULE NAME]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,15,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* NAME VIRTUAL FIELD */
	temp1=Rmalloc(61+1);
	sprintf(temp1,"%s",
		"[SECURITY][MODSEC][MODULE NAME]+\" AUTOMATIC REPORT CAPTURE\"");
	addvirtuallineADV(tmp,"NAME",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* CREATE WORKGROUP VIRTUAL FIELD */
	temp1=Rmalloc(185+1);
	sprintf(temp1,"%s%s",
		"IF(([ARCHIVE][ARCHWORK][USER IDENTIFICATION]=\"\") OR ([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE) OR ([A",
		"RCHIVE][ARCHWORK][DIRECTORY]=\"\")) THEN RETURN_VALUE=TRUE ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"CREATE WORKGROUP",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);





/* SORT DEFINITIONS */

	addsortline(tmp,0,"[SECURITY][MODSEC][MODULE NAME]");
	addsortline(tmp,0,"[SECURITY][USERS][USER IDENTIFICATION]");


	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"LAST USER NAME",3,"[SECURITY][USERS][USER NAME]",0);
	addaccumline(tmp,"LAST NAME",3,"[NAME]",0);
	addaccumline(tmp,"LAST CREATE WORKGROUP",3,"[CREATE WORKGROUP]",0);
/* CONTROL BREAKS */

	addcontrolline(tmp,"[SECURITY][MODSEC][MODULE NAME]",0);
	addcontrolline(tmp,"[SECURITY][USERS][USER IDENTIFICATION]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [ARCHIVE][ARCHWORK]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);

	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(25+1);
	sprintf(temp1,"%s",
		"([CREATE WORKGROUP]=TRUE)");
	imageoff=addimageline(tmp,"[SECURITY][MODSEC][MODULE NAME] HEADER","WRITE [ARCHIVE][ARCHWORK]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][MODSEC][MODULE NAME]",0,"[ARCHIVE][ARCHWORK][WORK GROUP]");
		addimagepval(image,0,"[LOGIN IDENTIFICATION]",0,"[ARCHIVE][ARCHWORK][USER IDENTIFICATION]");
		addimagepval(image,0,"[SECURITY][MODSEC][MODULE NAME]",0,"[ARCHIVE][ARCHWORK][DIRECTORY]");
		addimagepval(image,1,"FALSE",0,"[ARCHIVE][ARCHWORK][DELETEFLAG]");
		addimagepval(image,0,"[NAME]",0,"[ARCHIVE][ARCHWORK][NAME]");
	}


	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"FALSE",0,"[ARCHIVE][ARCHUSER][DELETEFLAG]");
		addimagepval(image,1,"TRUE",0,"[ARCHIVE][ARCHUSER][READ]");
		addimagepval(image,1,"TRUE",0,"[ARCHIVE][ARCHUSER][WRITE]");
		addimagepval(image,0,"[SECURITY][USERS][USER IDENTIFICATION]",0,"[ARCHIVE][ARCHUSER][USER IDENTIFICATION]");
		addimagepval(image,0,"[SECURITY][MODSEC][MODULE NAME]",0,"[ARCHIVE][ARCHUSER][WORK GROUP]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [ARCHIVE][ARCHWORK]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+7+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [ARCHIVE] Report [AUTOWRKGRPDEPTONE], Can Not Save Report!");
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
void RPTAUTOWRKGRPDEPT()
{
	RPT_AUTOWRKGRPDEPT();
	SCN_AUTOWRKGRPDEPT_RANGE_SCREEN();
	MNU_ARCHIVE_AUTOWRKGRPDEPT();
	RPT_AUTOWRKGRPDEPTONE();

}
