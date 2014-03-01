/* SECURITY's - Make (CHGRECORDSEC) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_CHGRECORDSEC()
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
	sprintf(rptname,"CHGRECORDSEC");
	t=Rmalloc(22+1);
	sprintf(t,"Change Record Security");
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
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

/* SELECT RECORDS EXPRESSION */

	temp1=Rmalloc(55+1);
	sprintf(temp1,"%s",
		"([SECURITY][FILESEC][EXPRESSION]=[EXISTING EXPRESSION])");
	tmp->select=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);




/* SEARCH FILES DEFINITIONS */

/* SECURITY FILESEC SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([SECURITY][FILESEC][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","FILESEC",0,NULL,NULL,"FILESEC KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","");
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"FILE NAME","");
	}
	if(t!=NULL) Rfree(t);






/* VIRTUAL FIELD DEFINITIONS */

/* EXISTING EXPRESSION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"EXISTING EXPRESSION",14,0,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* NEW EXPRESSION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"NEW EXPRESSION",14,0,temp1,1,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][FILESEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][FILESEC]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][FILESEC][FILE NAME]",0,"[SECURITY][FILESEC][FILE NAME]");
		addimagepval(image,0,"[SECURITY][FILESEC][MODULE NAME]",0,"[SECURITY][FILESEC][MODULE NAME]");
		addimagepval(image,0,"[SECURITY][FILESEC][USER IDENTIFICATION]",0,"[SECURITY][FILESEC][USER IDENTIFICATION]");
		addimagepval(image,0,"[NEW EXPRESSION]",0,"[SECURITY][FILESEC][EXPRESSION]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][FILESEC]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [CHGRECORDSEC], Can Not Save Report!");
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
static void SCN_CHGRECORDSEC_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","CHGRECORDSEC RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Report Description","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(91+1);
		sprintf(temp1,"%s",
			"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","CHANGE RECORD SECURITY","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","From Expression:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"EXISTING EXPRESSION","Existing Expression:","","",10,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","New Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"NEW EXPRESSION","New Expression:","","",10,80,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [FILESEC][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [FILESEC][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [FILESEC][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [FILESEC][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [FILESEC][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [FILESEC][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [FILESEC][FILE NAME]","Range on File Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [FILESEC][FILE NAME]","From File Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [FILESEC][FILE NAME]","To File Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][DELETE FILE] FALSE","Select when Delete File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][DELETE FILE] TRUE","Select when Delete File True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][FIELD SECURITY] FALSE","Select when Field Security False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][FIELD SECURITY] TRUE","Select when Field Security True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][PRINT FILE] FALSE","Select when Print File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][PRINT FILE] TRUE","Select when Print File True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][READ FILE] FALSE","Select when Read File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][READ FILE] TRUE","Select when Read File True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][WRITE FILE] FALSE","Select when Write File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][WRITE FILE] TRUE","Select when Write File True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(25+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [CHGRECORDSEC RANGE SCREEN], Can Not Save Maintain Master!");
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
static void DEF_CHGRECORDSEC_RANGE_SCREEN()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("SECURITY","CHGRECORDSEC RANGE SCREEN");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldCHAR(def,"RANGE ON [FILESEC][USER IDENTIFICATION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [FILESEC][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [FILESEC][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [FILESEC][MODULE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [FILESEC][MODULE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [FILESEC][MODULE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [FILESEC][FILE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [FILESEC][FILE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [FILESEC][FILE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"SELECT [FILESEC][DELETE FILE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][DELETE FILE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][DELETEFLAG] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][FIELD SECURITY] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][FIELD SECURITY] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][PRINT FILE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][PRINT FILE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][READ FILE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][READ FILE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][WRITE FILE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [FILESEC][WRITE FILE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [FILESEC][EXPRESSION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [FILESEC][EXPRESSION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [FILESEC][EXPRESSION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"EXISTING EXPRESSION",TRUE,14,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
		addDefaultFieldSTRING(def,"NEW EXPRESSION",TRUE,14,"");
		addDefaultFieldCHAR(def,"PRINT RANGE CRITERIA",TRUE,10,0);

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
			temp1=Rmalloc(25+8+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [SECURITY] Screen [CHGRECORDSEC RANGE SCREEN], Can Not Save Screen Defaults!");
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
void CHGRECORDSEC()
{
	RPT_CHGRECORDSEC();
	SCN_CHGRECORDSEC_RANGE_SCREEN();
	DEF_CHGRECORDSEC_RANGE_SCREEN();
}
