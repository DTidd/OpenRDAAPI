/* ARCHIVE's - Make (EMAILARCHIVE) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;
extern char *swb_utl5_rgd;

static void RPT_EMAILARCHIVE()
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
	sprintf(rptname,"EMAILARCHIVE");
	t=Rmalloc(16+1);
	sprintf(t,"Email Archive(s)");
	desc=Rmalloc(67+1);
	sprintf(desc,"This output definition is used to Email Archive(s) to Recipient(s).");
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	temp1=Rmalloc(157+1);
	sprintf(temp1,"%s%s",
		"(([NAME CONTAINS]=\"\") OR ([ARCHIVE][ARCHIVES][ARCHIVE NAME] C [NAME CONTAINS])) AND (([SELECT EXPR",
		"ESSION]=\"\") OR (EVALUATE_BOOLEAN([SELECT EXPRESSION])))");
	tmp->select=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);





/* SEARCH FILES DEFINITIONS */

/* ARCHIVE ARCHIVES SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHIVES][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHIVES",0,NULL,NULL,"ARCHIVE KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","");
		addsearchconfld(s,"ARCHIVE NAME","");
	}
	if(t!=NULL) Rfree(t);

/* ARCHIVE ARCHCLS SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHCLS",1,"ARCHIVE","ARCHIVES","ARCHCLS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"CLASS","[ARCHIVES][CLASS]");
	}



/* ARCHIVE ARCHTYPE SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHTYPE",2,"ARCHIVE","ARCHIVES","ARCHTYPE KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"TYPE","[ARCHIVES][TYPE]");
	}


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",3,"ARCHIVE","ARCHIVES","USERS KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[USER IDENTIFICATION]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LOGIN IDENTIFICATION]");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* RECIPIENT NAME VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"RECIPIENT NAME",1,40,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* RECIPIENT EMAIL ADDRESS VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"RECIPIENT EMAIL ADDRESS",1,60,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* SUBJECT VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"SUBJECT",1,60,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* BODY VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"BODY",14,999,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ABSOLUTE FILENAME VIRTUAL FIELD */
	temp1=Rmalloc(499+1);
	sprintf(temp1,"%s",
		"([CURRENT DIRECTORY]+\"/ARCHIVE/.\"+(ADD_DASHES([ARCHIVE][ARCHIVES][WORK GROUP]))+\"/\"+[ARCHIVE][ARCHIVES][FILENAME])");
	addvirtuallineADV(tmp,"ABSOLUTE FILENAME",1,400,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ATTACHMENT NAME VIRTUAL FIELD */
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[ARCHIVE][ARCHIVES][FILENAME]");
	addvirtuallineADV(tmp,"ATTACHMENT NAME",1,80,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* NAME CONTAINS VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"NAME CONTAINS",0,0,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* SELECT EXPRESSION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"SELECT EXPRESSION",14,0,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);



/* SORT DEFINITIONS */

	addsortline(tmp,0,"[ARCHIVE][ARCHIVES][WORK GROUP]");
	addsortline(tmp,0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]");


	SetReportDefaultEngine(tmp);


/* CONTROL BREAKS */

	addcontrolline(tmp,"[ARCHIVE][ARCHIVES][WORK GROUP]",0);
	addcontrolline(tmp,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"DETAIL LINES","SENDATTACH-1",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[BODY]",0,"BODY");
		addimagepval(image,0,"[ABSOLUTE FILENAME]",0,"FILENAME TO BE ATTACHED");
		addimagepval(image,0,"[ATTACHMENT NAME]",0,"EMAIL ATTACHMENT TO BE NAMED");
		addimagepval(image,0,"[SECURITY][USERS][USER NAME]",0,"NAME OF SENDER");
		addimagepval(image,0,"[RECIPIENT EMAIL ADDRESS]",0,"RECIPIENT EMAIL ADDRESS");
		addimagepval(image,0,"[SECURITY][USERS][EMAIL]",0,"SENDER EMAIL ADDRESS");
		addimagepval(image,0,"[SUBJECT]",0,"SUBJECT");
	}


	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+7+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [ARCHIVE] Report [EMAILARCHIVE], Can Not Save Report!");
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
static void MNU_ARCHIVE_EMAILARCHIVE()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE EMAILARCHIVE","doreport.EXT ARCHIVE EMAILARCHIVE");
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
static void SCN_EMAILARCHIVE_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","EMAILARCHIVE RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Recipient","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,swb_utl5_rgd,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","    Recipient Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RECIPIENT NAME","Recipient Name:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Recipient Email Address:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RECIPIENT EMAIL ADDRESS","Recipient Email Address:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Message","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUBJECT","Subject:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   Body:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"BODY","Body:","","",10,65,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Range","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][WORK GROUP]","Range on Work Group","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][WORK GROUP]","From Work Group","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][WORK GROUP]","To Work Group","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][CLASS]","Range on Class","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"FROM CLASS LIST","From Class","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"TO CLASS LIST","To Class","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][SUBJECT]","Range on Subject","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][SUBJECT]","From Subject","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][SUBJECT]","To Subject","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][ARCHIVE NAME]","Range on Archive Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][ARCHIVE NAME]","From Archive Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][ARCHIVE NAME]","To Archive Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][TYPE]","Range on Type","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][TYPE]","From Type","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][TYPE]","To Type","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][FILENAME]","Range on Filename","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][FILENAME]","From Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][FILENAME]","To Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][DATE]","Range on Date","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][DATE]","From Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][DATE]","To Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][TIME]","Range on Time","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][TIME]","From Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][TIME]","To Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][SOURCE USER]","Range on Source User","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][SOURCE USER]","From Source User","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][SOURCE USER]","To Source User","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][SOURCE]","Range on Source","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ARCHIVES SOURCES FROM","From Source","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ARCHIVES SOURCES TO","To Source","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Contains/Select Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Select when Archive Name Contains:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME CONTAINS","Select when Archive Name Contains","","",0,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Select Expression/Formula:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"SELECT EXPRESSION","Select Expression/Formula:","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(84+1);
		sprintf(temp3,"%s",
			"([RECIPIENT NAME]!=\"\") AND ([RECIPIENT EMAIL ADDRESS]!=\"\") AND ([SUBJECT]!=\"\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,20,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
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
			temp1=Rmalloc(25+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [EMAILARCHIVE RANGE SCREEN], Can Not Save Maintain Master!");
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
static void DEF_EMAILARCHIVE_RANGE_SCREEN()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("ARCHIVE","EMAILARCHIVE RANGE SCREEN");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,1);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,1);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,1);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,1);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"RECIPIENT NAME",TRUE,1,"");
		addDefaultFieldSTRING(def,"RECIPIENT EMAIL ADDRESS",TRUE,1,"");
		addDefaultFieldSTRING(def,"SUBJECT",TRUE,1,"");
		addDefaultFieldSTRING(def,"BODY",TRUE,14,"");
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(25+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [EMAILARCHIVE RANGE SCREEN], Can Not Save Screen Defaults!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(def!=NULL) FreeRDAdefault(def);
	}
}
void EMAILARCHIVE()
{
	RPT_EMAILARCHIVE();
	MNU_ARCHIVE_EMAILARCHIVE();
	SCN_EMAILARCHIVE_RANGE_SCREEN();
	DEF_EMAILARCHIVE_RANGE_SCREEN();
}
