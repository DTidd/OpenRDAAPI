/* ARCHIVE's - Make (PURGEARCHIVE) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
#include <mkdef.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_PURGEARCHIVE()
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
	sprintf(rptname,"PURGEARCHIVE");
	t=Rmalloc(33+1);
	sprintf(t,"Purge Archive Records & Documents");
	desc=Rmalloc(76+1);
	sprintf(desc,"This output definition is used to purge Archive Records and their Documents.");
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
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHWORK",1,"ARCHIVE","ARCHIVES","ARCHWORK KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[ARCHIVES][WORK GROUP]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* PURGE FILENAME VIRTUAL FIELD */
	temp1=Rmalloc(182+1);
	sprintf(temp1,"%s",
		"\"[CURRENT DIRECTORY]+\"/\"+ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY]+\"/\"+[ARCHIVE][ARCHIVES][FILENAME]");
	addvirtuallineADV(tmp,"PURGE FILENAME",1,200,temp1,0,0);
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







	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [ARCHIVE][ARCHIVES]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [ARCHIVE][ARCHIVES]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][WORK GROUP]",0,"[ARCHIVE][ARCHIVES][WORK GROUP]");
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]",0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]");
		addimagepval(image,1,"TRUE",0,"[ARCHIVE][ARCHIVES][DELETEFLAG]");
	}


	imageoff=addimageline(tmp,"DETAIL LINES","UNLINK",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[PURGE FILENAME]",0,"FILENAME");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [ARCHIVE][ARCHIVES]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+7+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [ARCHIVE] Report [PURGEARCHIVE], Can Not Save Report!");
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
static void SCN_PURGEARCHIVE_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","PURGEARCHIVE RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Report Instructions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","This report writes directly to the database.  Please range carefully.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Range and Select","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][WORK GROUP]","Range on Work Group/Folder","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][WORK GROUP]","From Work Group","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][WORK GROUP]","To Work Group","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHWORK][NAME]","Range on Work Group Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHWORK][NAME]","From Work Group Name","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHWORK][NAME]","To Work Group Name","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][CLASS]","Range on Class","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][CLASS]","From Class","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][CLASS]","To Class","","",0,4,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,6,"RUN REPORT","Write to Database","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(25+14+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [PURGEARCHIVE RANGE SCREEN], Can Not Save Maintain Master!");
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
static void DEF_PURGEARCHIVE_RANGE_SCREEN()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("ARCHIVE","PURGEARCHIVE RANGE SCREEN");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,1);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,1);
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
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,1);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][MODULE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][MODULE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][MODULE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHWORK][DELETEFLAG] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [ARCHWORK][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHWORK][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHWORK][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHWORK][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHWORK][USER IDENTIFICATION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHWORK][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHWORK][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHWORK][NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHWORK][NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHWORK][NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHWORK][DIRECTORY]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHWORK][DIRECTORY]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHWORK][DIRECTORY]",TRUE,1,"");
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
			temp1=Rmalloc(25+14+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [PURGEARCHIVE RANGE SCREEN], Can Not Save Screen Defaults!");
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
static void MNU_ARCHIVE_PURGEARCHIVE()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE PURGEARCHIVE","doreport.EXT ARCHIVE PURGEARCHIVE");
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

void PURGEARCHIVE()
{
	RPT_PURGEARCHIVE();
	SCN_PURGEARCHIVE_RANGE_SCREEN();
	DEF_PURGEARCHIVE_RANGE_SCREEN();
	MNU_ARCHIVE_PURGEARCHIVE();
}
