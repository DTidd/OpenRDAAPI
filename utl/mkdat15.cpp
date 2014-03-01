/* DATABASE's - Make (PURGE-DATLOG) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_PURGE_DATLOG()
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
	sprintf(rptname,"PURGE-DATLOG");
	t=Rmalloc(35+1);
	sprintf(t,"Purge Database Log and Transactions");
	desc=Rmalloc(66+1);
	sprintf(desc,"This output device is used to purge Database Logs and Transactions");
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
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,TRUE);
	tmp->report_completion=1;
	tmp->remove_file=0;
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* DATABASE DATLOG SEARCH FILE */

	temp1=Rmalloc(38+1);
	sprintf(temp1,"%s",
		"([DATABASE][DATLOG][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"DATABASE","DATLOG",0,NULL,NULL,"DATLOG KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"MODULE","");
		addsearchconfld(s,"USER IDENTIFICATION","");
		addsearchconfld(s,"LOG NUMBER","");
	}
	if(t!=NULL) Rfree(t);


/* DATABASE DATTRN SEARCH FILE */

	temp1=Rmalloc(252+1);
	sprintf(temp1,"%s%s%s",
		"([DATABASE][DATTRN][DELETEFLAG]=FALSE) AND ([DATABASE][DATTRN][MODULE]=[DATABASE][DATLOG][MODULE]) A",
		"ND ([DATABASE][DATTRN][USER IDENTIFICATION]=[DATABASE][DATLOG][USER IDENTIFICATION]) AND ([DATABASE]",
		"[DATTRN][LOG NUMBER]=[DATABASE][DATLOG][LOG NUMBER])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"DATABASE","DATTRN",16,"DATABASE","DATLOG","DATTRN KEY 1",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"MODULE","[DATLOG][MODULE]");
		addsearchconfld(s,"USER IDENTIFICATION","[DATLOG][USER IDENTIFICATION]");
		addsearchconfld(s,"LOG NUMBER","[DATLOG][LOG NUMBER]");
		addsearchconfld(s,"TRANSACTION NUMBER","");
	}
	if(t!=NULL) Rfree(t);






/* VIRTUAL FIELD DEFINITIONS */

/* MODULE VIRTUAL FIELD */
	temp1=Rmalloc(26+1);
	sprintf(temp1,"%s",
		"[DATABASE][DATLOG][MODULE]");
	addvirtuallineADV(tmp,"MODULE",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[DATABASE][DATLOG][USER IDENTIFICATION]");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* LOG NUMBER VIRTUAL FIELD */
	temp1=Rmalloc(30+1);
	sprintf(temp1,"%s",
		"[DATABASE][DATLOG][LOG NUMBER]");
	addvirtuallineADV(tmp,"LOG NUMBER",8,8,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* TRANSACTION NUMBER VIRTUAL FIELD */
	temp1=Rmalloc(102+1);
	sprintf(temp1,"%s%s",
		"IF([DETAIL COUNT]=1) THEN RETURN_VALUE=0 ELSE \nRETURN_VALUE=[DATABASE][DATTRN][TRANSACTION NUMBER] ",
		"FI");
	addvirtuallineADV(tmp,"TRANSACTION NUMBER",8,8,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);





/* SORT DEFINITIONS */

	addsortline(tmp,0,"[MODULE]");
	addsortline(tmp,0,"[USER IDENTIFICATION]");
	addsortline(tmp,0,"[LOG NUMBER]");
	addsortline(tmp,0,"[TRANSACTION NUMBER]");


	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"MODULE",3,"[MODULE]",0);
	addaccumline(tmp,"USER IDENTIFICATION",3,"[USER IDENTIFICATION]",0);
	addaccumline(tmp,"LOG NUMBER",3,"[LOG NUMBER]",0);
/* CONTROL BREAKS */

	addcontrolline(tmp,"[MODULE]",0);
	addcontrolline(tmp,"[USER IDENTIFICATION]",0);
	addcontrolline(tmp,"[LOG NUMBER]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [DATABASE][DATLOG]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [DATABASE][DATTRN]",FALSE,NULL,0,NULL);

	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(24+1);
	sprintf(temp1,"%s",
		"([TRANSACTION NUMBER]=0)");
	imageoff=addimageline(tmp,"[LOG NUMBER] HEADER","WRITE [DATABASE][DATLOG]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[MODULE]",0,"[DATABASE][DATLOG][MODULE]");
		addimagepval(image,0,"[USER IDENTIFICATION]",0,"[DATABASE][DATLOG][USER IDENTIFICATION]");
		addimagepval(image,0,"[LOG NUMBER]",0,"[DATABASE][DATLOG][LOG NUMBER]");
		addimagepval(image,1,"TRUE",0,"[DATABASE][DATLOG][DELETEFLAG]");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(25+1);
	sprintf(temp1,"%s",
		"([TRANSACTION NUMBER]!=0)");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [DATABASE][DATTRN]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[MODULE]",0,"[DATABASE][DATTRN][MODULE]");
		addimagepval(image,0,"[USER IDENTIFICATION]",0,"[DATABASE][DATTRN][USER IDENTIFICATION]");
		addimagepval(image,0,"[LOG NUMBER]",0,"[DATABASE][DATTRN][LOG NUMBER]");
		addimagepval(image,0,"[TRANSACTION NUMBER]",0,"[DATABASE][DATTRN][TRANSACTION NUMBER]");
		addimagepval(image,1,"TRUE",0,"[DATABASE][DATTRN][DELETEFLAG]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [DATABASE][DATTRN]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [DATABASE][DATLOG]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [DATABASE] Report [PURGE-DATLOG], Can Not Save Report!");
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
static void SCN_PURGE_DATLOG_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","PURGE-DATLOG RANGE SCREEN");
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
		ADVaddwdgt(scrn,5,"","PURGE DATABASE LOG AND TRANSACTIONS","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","This output device is used to purge Database Logs and Transactions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Range","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,791,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][MODULE]","Range on Module","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][MODULE]","From Module","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][MODULE]","To Module","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][LOG NUMBER]","Range on Log Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][LOG NUMBER]","From Log Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][LOG NUMBER]","To Log Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][END DATE]","Range on End Date","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][END DATE]","From End Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][END DATE]","To End Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][END TIME]","Range on End Time","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][END TIME]","From End Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][END TIME]","To End Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][PROCESS NAME]","Range on Process Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][PROCESS NAME]","From Process Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][PROCESS NAME]","To Process Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][START DATE]","Range on Start Date","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][START DATE]","From Start Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][START DATE]","To Start Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][START TIME]","Range on Start Time","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][START TIME]","From Start Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][START TIME]","To Start Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][UNDONE BY]","Range on Undone By","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][UNDONE BY]","From Undone By","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][UNDONE BY]","To Undone By","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][UNDONE DATE]","Range on Undone Date","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][UNDONE DATE]","From Undone Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][UNDONE DATE]","To Undone Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATLOG][UNDONE TIME]","Range on Undone Time","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATLOG][UNDONE TIME]","From Undone Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATLOG][UNDONE TIME]","To Undone Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATLOG][HAS TRANSACTIONS] FALSE","Select when Has Transactions False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATLOG][HAS TRANSACTIONS] TRUE","Select when Has Transactions True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATLOG][UNDONE] FALSE","Select when Undone False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATLOG][UNDONE] TRUE","Select when Undone True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [PURGE-DATLOG RANGE SCREEN], Can Not Save Maintain Master!");
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
static void DEF_PURGE_DATLOG_RANGE_SCREEN()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("DATABASE","PURGE-DATLOG RANGE SCREEN");
	if(def!=NULL)
	{
		def->input_focus=stralloc("RANGE ON [DATLOG][MODULE]");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][MODULE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][MODULE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][MODULE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][USER IDENTIFICATION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][LOG NUMBER]",TRUE,10,0);
		addDefaultFieldINT(def,"FROM [DATLOG][LOG NUMBER]",TRUE,8,0);
		addDefaultFieldINT(def,"TO [DATLOG][LOG NUMBER]",TRUE,8,0);
		addDefaultFieldCHAR(def,"SELECT [DATLOG][DELETEFLAG] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [DATLOG][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][END DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][END DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][END DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][END TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][END TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][END TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"SELECT [DATLOG][HAS TRANSACTIONS] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [DATLOG][HAS TRANSACTIONS] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][PROCESS NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][PROCESS NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][PROCESS NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][START DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][START DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][START DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][START TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][START TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][START TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"SELECT [DATLOG][UNDONE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [DATLOG][UNDONE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][UNDONE BY]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][UNDONE BY]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][UNDONE BY]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][UNDONE DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][UNDONE DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][UNDONE DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"RANGE ON [DATLOG][UNDONE TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [DATLOG][UNDONE TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [DATLOG][UNDONE TIME]",TRUE,17,"");
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
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [DATABASE] Screen [PURGE-DATLOG RANGE SCREEN], Can Not Save Screen Defaults!");
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
void PURGEDATLOGTRN()
{
	RPT_PURGE_DATLOG();
	SCN_PURGE_DATLOG_RANGE_SCREEN();
	DEF_PURGE_DATLOG_RANGE_SCREEN();
}
