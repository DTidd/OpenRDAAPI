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
static void RPT_LOGSTAT()
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
	rptname=Rmalloc(7+1);
	sprintf(rptname,"LOGSTAT");
	t=Rmalloc(39+1);
	sprintf(t,"Process Statistics from Transaction Log");
	desc=Rmalloc(123+1);
	sprintf(desc,"This output definition is used to print statistics on processes executed in OpenRDA 4.0 from the database transaction logs.");
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
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,TRUE,FALSE,FALSE,FALSE,"","",TRUE,1,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,0,FALSE,0,NULL,FALSE,0,1,0,pvars,1,1,0,FALSE,"");

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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",1,"DATABASE","DATLOG","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[DATLOG][USER IDENTIFICATION]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* MODULE NAME VIRTUAL FIELD */
	temp1=Rmalloc(3170+1);
	sprintf(temp1,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([DATABASE][DATLOG][MODULE]=\"DATABASE\") THEN RETURN_VALUE=\"Database Management\" ELSE \nIF([DAT",
		"ABASE][DATLOG][MODULE]=\"GUI\") THEN RETURN_VALUE=\"Graphical User Interface\" ELSE \nIF([DATABASE][",
		"DATLOG][MODULE]=\"OLHELP\") THEN RETURN_VALUE=\"On-Line Help\" ELSE \nIF([DATABASE][DATLOG][MODULE]=",
		"\"RPTGEN\") THEN RETURN_VALUE=\"Report/Output Generator\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"SEC",
		"URITY\") THEN RETURN_VALUE=\"Security\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"TRANS\") THEN RETURN_",
		"VALUE=\"Transaction Processing\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"TOOLS\") THEN RETURN_VALUE=",
		"\"Tool Set / Packaging\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"UTILITIES\") THEN RETURN_VALUE=\"Uti",
		"lities / Maintenance\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"BUDPREP\") THEN RETURN_VALUE=\"Budget ",
		"Preparation\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"FINMGT\") THEN RETURN_VALUE=\"Financial Managem",
		"ent\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"BNKREC\") THEN RETURN_VALUE=\"Bank Reconciliation\" ELS",
		"E \nIF([DATABASE][DATLOG][MODULE]=\"FIXASS\") THEN RETURN_VALUE=\"Fixed Assets\" ELSE \nIF([DATABASE",
		"][DATLOG][MODULE]=\"INVCTL\") THEN RETURN_VALUE=\"Inventory Control\" ELSE \nIF([DATABASE][DATLOG][M",
		"ODULE]=\"VENPMT\") THEN RETURN_VALUE=\"Vendor Payments\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"APPM",
		"GT\") THEN RETURN_VALUE=\"Applicant Tracking\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"CATALOGUE\") T",
		"HEN RETURN_VALUE=\"Catalogue\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"PURORD\") THEN RETURN_VALUE=\"",
		"Purchase Orders\" ELSE\nIF([DATABASE][DATLOG][MODULE]=\"REQMGT\") THEN RETURN_VALUE=\"Requisitions\"",
		" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"PRSNNL\") THEN RETURN_VALUE=\"Personnel Demographics\" ELSE ",
		"\nIF([DATABASE][DATLOG][MODULE]=\"PAYROLL\") THEN RETURN_VALUE=\"Payroll\" ELSE \nIF([DATABASE][DATL",
		"OG][MODULE]=\"POSTRK\") THEN RETURN_VALUE=\"Position Tracking\" ELSE \nIF([DATABASE][DATLOG][MODULE]",
		"=\"LVEMGT\") THEN RETURN_VALUE=\"Leave Management\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"EMPABS\")",
		" THEN RETURN_VALUE=\"Employee Absentee\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"SUBMGT\") THEN RETUR",
		"N_VALUE=\"Substitute Management\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"OPENSS\") THEN RETURN_VALUE",
		"=\"Open Self Service\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"RLSTMGT\") THEN RETURN_VALUE=\"Real Es",
		"tate Taxes\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"PROPERTY\") THEN RETURN_VALUE=\"Property Taxes\"",
		" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"DMVREG\") THEN RETURN_VALUE=\"Motor Vehicle Registration\" E",
		"LSE \nIF([DATABASE][DATLOG][MODULE]=\"BLDPRMT\") THEN RETURN_VALUE=\"Building Permits\" ELSE \nIF([D",
		"ATABASE][DATLOG][MODULE]=\"OCCTAX\") THEN RETURN_VALUE=\"Occupational Tax/Business Lic\" ELSE \nIF([",
		"DATABASE][DATLOG][MODULE]=\"UTLBLL\") THEN RETURN_VALUE=\"Utility Billing\" ELSE \nIF([DATABASE][DAT",
		"LOG][MODULE]=\"MISCBILL\") THEN RETURN_VALUE=\"Misc Billing\" ELSE \nIF([DATABASE][DATLOG][MODULE]=",
		"\"CSHDWR\") THEN RETURN_VALUE=\"Collections\" ELSE \nIF([DATABASE][DATLOG][MODULE]=\"ARCHIVE\") THEN",
		" RETURN_VALUE=\"E-Documents\" \nELSE RETURN_VALUE=[DATABASE][DATLOG][MODULE] FI FI FI FI FI FI FI FI",
		" FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"MODULE NAME",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* EXECUTED BY VIRTUAL FIELD */
	temp1=Rmalloc(42+1);
	sprintf(temp1,"%s",
		"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",1,60,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* REPORT TITLE VIRTUAL FIELD */
	temp1=Rmalloc(44+1);
	sprintf(temp1,"%s",
		"\"PROCESS STATISTICS FROM TRANSACTION LOGS\"");
	addvirtuallineADV(tmp,"REPORT TITLE",1,60,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DURATION VIRTUAL FIELD */
	temp1=Rmalloc(304+1);
	sprintf(temp1,"%s%s%s%s",
		"IF(([DATABASE][DATLOG][END DATE]=\"\") OR ([DATABASE][DATLOG][END TIME]=\"\")) THEN\nRETURN_VALUE=0 ",
		"ELSE \nRETURN_VALUE= \n((DATE_TO_VALUE(([DATABASE][DATLOG][END DATE]+\" \"+[DATABASE][DATLOG][END TI",
		"ME])))\n-\n(DATE_TO_VALUE(([DATABASE][DATLOG][START DATE]+\" \"+[DATABASE][DATLOG][START TIME]))))\n",
		"\nFI");
	addvirtuallineADV(tmp,"DURATION",20,10,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DURATION TIME VIRTUAL FIELD */
	temp1=Rmalloc(27+1);
	sprintf(temp1,"%s",
		"(VALUE_TO_TIME([DURATION]))");
	addvirtuallineADV(tmp,"DURATION TIME",17,8,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* COUNT VIRTUAL FIELD */
	temp1=Rmalloc(1+1);
	sprintf(temp1,"%s",
		"1");
	addvirtuallineADV(tmp,"COUNT",8,4,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* AVERAGE DURATION TIME VIRTUAL FIELD */
	temp1=Rmalloc(49+1);
	sprintf(temp1,"%s",
		"(VALUE_TO_TIME(([TOTAL DURATION]/[TOTAL COUNT])))");
	addvirtuallineADV(tmp,"AVERAGE DURATION TIME",17,8,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* AVERAGE DURATION VIRTUAL FIELD */
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"(([TOTAL DURATION]/[TOTAL COUNT]))");
	addvirtuallineADV(tmp,"AVERAGE DURATION",6,10,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);





/* SORT DEFINITIONS */

	addsortline(tmp,0,"[DATABASE][DATLOG][MODULE]");
	addsortline(tmp,0,"[DATABASE][DATLOG][PROCESS NAME]");
	addsortline(tmp,0,"[DATABASE][DATLOG][USER IDENTIFICATION]");
	addsortline(tmp,0,"[DATABASE][DATLOG][LOG NUMBER]");


	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"LAST MODULE",3,"[DATABASE][DATLOG][MODULE]",0);
	addaccumline(tmp,"LAST PROCESS NAME",3,"[DATABASE][DATLOG][PROCESS NAME]",0);
	addaccumline(tmp,"LAST USER IDENTIFICATION",3,"[DATABASE][DATLOG][USER IDENTIFICATION]",0);
	addaccumline(tmp,"LAST SECURITY USERS NAME",3,"[SECURITY][USERS][USER NAME]",0);
	addaccumline(tmp,"FREQUENCY",1,"[DATABASE][DATLOG][LOG NUMBER]",0);
	addaccumline(tmp,"TOTAL COUNT",7,"[COUNT]",0);
	addaccumline(tmp,"TOTAL DURATION",7,"[DURATION]",0);
/* CONTROL BREAKS */

	addcontrolline(tmp,"[DATABASE][DATLOG][MODULE]",0);
	addcontrolline(tmp,"[DATABASE][DATLOG][PROCESS NAME]",0);
	addcontrolline(tmp,"[DATABASE][DATLOG][USER IDENTIFICATION]",0);




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


	imageoff=addimageline(tmp,"[DATABASE][DATLOG][PROCESS NAME] HEADER","@LLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[DATABASE][DATLOG][MODULE]",0,NULL);
		addimagepval(image,0,"[MODULE NAME]",0,NULL);
		addimagepval(image,0,"[DATABASE][DATLOG][PROCESS NAME]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"[DATABASE][DATLOG][PROCESS NAME] HEADER","@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"[DATABASE][DATLOG][USER IDENTIFICATION] FOOTER","               @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLL      @RRRR    @RRRRRRRR.RR      @RR.RR    @LLLLLLL@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[LAST SECURITY USERS NAME]",0,NULL);
		addimagepval(image,0,"[LAST USER IDENTIFICATION]",0,NULL);
		addimagepval(image,0,"[TOTAL COUNT]",0,NULL);
		addimagepval(image,0,"[TOTAL DURATION]",0,NULL);
		addimagepval(image,0,"[AVERAGE DURATION]",0,NULL);
		addimagepval(image,0,"[AVERAGE DURATION TIME]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"([FREQUENCY]>1) AND ([TOTAL DURATION]!=0)");
	imageoff=addimageline(tmp,"[DATABASE][DATLOG][PROCESS NAME] FOOTER","               AVERAGE: @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL      @RRRR    @RRRRRRRR.RR      @RR.RR    @LLLLLLL@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[LAST PROCESS NAME]",0,NULL);
		addimagepval(image,0,"[TOTAL COUNT]",0,NULL);
		addimagepval(image,0,"[TOTAL DURATION]",0,NULL);
		addimagepval(image,0,"[AVERAGE DURATION]",0,NULL);
		addimagepval(image,0,"[AVERAGE DURATION TIME]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"[DATABASE][DATLOG][PROCESS NAME] FOOTER","@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"[DATABASE][DATLOG][PROCESS NAME] FOOTER","@@",FALSE,NULL,0,NULL);
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


	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(7+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [DATABASE] Report [LOGSTAT], Can Not Save Report!");
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
static void SCN_LOGSTAT_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","LOGSTAT RANGE SCREEN");
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
		ADVaddwdgt(scrn,5,"","PROCESS STATISTICS FROM TRANSACTION LOG","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","This output definition is used to print statistics on processes executed in OpenRDA 4.0 from the database transaction logs.","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][APPROVAL LIMIT]","Range on Approval Limit","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][APPROVAL LIMIT]","From Approval Limit","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][APPROVAL LIMIT]","To Approval Limit","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][BROWSER TYPE]","Range on Browser Type","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][BROWSER TYPE]","From Browser Type","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][BROWSER TYPE]","To Browser Type","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][CARRIER]","Range on Carrier","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][CARRIER]","From Carrier","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][CARRIER]","To Carrier","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][CARRIER TYPE]","Range on Carrier Type","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][CARRIER TYPE]","From Carrier Type","","",0,1,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][CARRIER TYPE]","To Carrier Type","","",0,1,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][CELL PHONE]","Range on Cell Phone","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][CELL PHONE]","From Cell Phone","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][CELL PHONE]","To Cell Phone","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][COLOR PROFILE]","Range on Color Profile","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][COLOR PROFILE]","From Color Profile","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][COLOR PROFILE]","To Color Profile","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][CONNECT TIMEOUT]","Range on Connect Timeout","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][CONNECT TIMEOUT]","From Connect Timeout","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][CONNECT TIMEOUT]","To Connect Timeout","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEFAULT FILE OUTPUT DEVICE]","Range on Default File Output Device","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEFAULT FILE OUTPUT DEVICE]","From Default File Output Device","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEFAULT FILE OUTPUT DEVICE]","To Default File Output Device","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEFAULT PRINTER]","Range on Default Printer","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEFAULT PRINTER]","From Default Printer","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEFAULT PRINTER]","To Default Printer","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEPARTMENT]","Range on Department","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEPARTMENT]","From Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEPARTMENT]","To Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DOMENU STYLE]","Range on Domenu Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DOMENU STYLE]","From Domenu Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DOMENU STYLE]","To Domenu Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][EMAIL]","Range on Email","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][EMAIL]","From Email","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][EMAIL]","To Email","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][FONT]","Range on Font","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][FONT]","From Font","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][FONT]","To Font","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][FONT SIZE]","Range on Font Size","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][FONT SIZE]","From Font Size","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][FONT SIZE]","To Font Size","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][FONT WEIGHT]","Range on Font Weight","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][FONT WEIGHT]","From Font Weight","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][FONT WEIGHT]","To Font Weight","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][GTKRC]","Range on Gtkrc","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][GTKRC]","From Gtkrc","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][GTKRC]","To Gtkrc","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][HOST]","Range on Host","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][HOST]","From Host","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][HOST]","To Host","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][LOCATION IDENTIFICATION]","Range on Location Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][LOCATION IDENTIFICATION]","From Location Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][LOCATION IDENTIFICATION]","To Location Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][NTLMDOMAIN]","Range on Ntlmdomain","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][NTLMDOMAIN]","From Ntlmdomain","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][NTLMDOMAIN]","To Ntlmdomain","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PASSWORD]","Range on Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PASSWORD]","From Password","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PASSWORD]","To Password","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PAYROLL SIGNATURE FILENAME]","Range on Payroll Signature Filename","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PAYROLL SIGNATURE FILENAME]","From Payroll Signature Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PAYROLL SIGNATURE FILENAME]","To Payroll Signature Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PAYROLL SIGNATURE PASSWORD]","Range on Payroll Signature Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PAYROLL SIGNATURE PASSWORD]","From Payroll Signature Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PAYROLL SIGNATURE PASSWORD]","To Payroll Signature Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PHONE EXTENSION]","Range on Phone Extension","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PHONE EXTENSION]","From Phone Extension","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PHONE EXTENSION]","To Phone Extension","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PHONE NUMBER]","Range on Phone Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PHONE NUMBER]","From Phone Number","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PHONE NUMBER]","To Phone Number","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PROTOCOL]","Range on Protocol","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PROTOCOL]","From Protocol","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PROTOCOL]","To Protocol","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PURORD SIGNATURE FILENAME]","Range on Purord Signature Filename","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PURORD SIGNATURE FILENAME]","From Purord Signature Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PURORD SIGNATURE FILENAME]","To Purord Signature Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PURORD SIGNATURE PASSWORD]","Range on Purord Signature Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PURORD SIGNATURE PASSWORD]","From Purord Signature Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PURORD SIGNATURE PASSWORD]","To Purord Signature Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PUSHBUTTON STYLE]","Range on Pushbutton Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PUSHBUTTON STYLE]","From Pushbutton Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PUSHBUTTON STYLE]","To Pushbutton Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][SCAN DIRECTORY]","Range on Scan Directory","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][SCAN DIRECTORY]","From Scan Directory","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][SCAN DIRECTORY]","To Scan Directory","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][SITE IDENTIFICATION]","Range on Site Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][SITE IDENTIFICATION]","From Site Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][SITE IDENTIFICATION]","To Site Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][SMTP AUTH]","Range on Smtp Auth","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][SMTP AUTH]","From Smtp Auth","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][SMTP AUTH]","To Smtp Auth","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][SMTP PORT]","Range on Smtp Port","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][SMTP PORT]","From Smtp Port","","",0,5,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][SMTP PORT]","To Smtp Port","","",0,5,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][SMTP SERVER]","Range on Smtp Server","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][SMTP SERVER]","From Smtp Server","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][SMTP SERVER]","To Smtp Server","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][TLS CRT FILE]","Range on Tls Crt File","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][TLS CRT FILE]","From Tls Crt File","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][TLS CRT FILE]","To Tls Crt File","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][TLS KEY FILE]","Range on Tls Key File","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][TLS KEY FILE]","From Tls Key File","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][TLS KEY FILE]","To Tls Key File","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][TLS TRUST FILE]","Range on Tls Trust File","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][TLS TRUST FILE]","From Tls Trust File","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][TLS TRUST FILE]","To Tls Trust File","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER NAME]","Range on User Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER NAME]","From User Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER NAME]","To User Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USERNAME]","Range on Username","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USERNAME]","From Username","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USERNAME]","To Username","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][VENPMT SIGNATURE FILENAME]","Range on Venpmt Signature Filename","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][VENPMT SIGNATURE FILENAME]","From Venpmt Signature Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][VENPMT SIGNATURE FILENAME]","To Venpmt Signature Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][VENPMT SIGNATURE PASSWORD]","Range on Venpmt Signature Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][VENPMT SIGNATURE PASSWORD]","From Venpmt Signature Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][VENPMT SIGNATURE PASSWORD]","To Venpmt Signature Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WEB LOGIN]","Range on Web Login","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WEB LOGIN]","From Web Login","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WEB LOGIN]","To Web Login","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WEB PASSWORD]","Range on Web Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WEB PASSWORD]","From Web Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WEB PASSWORD]","To Web Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WORKSTATION DOCUMENTS]","Range on Workstation Documents","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WORKSTATION DOCUMENTS]","From Workstation Documents","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WORKSTATION DOCUMENTS]","To Workstation Documents","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WORKSTATION LOGIN]","Range on Workstation Login","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WORKSTATION LOGIN]","From Workstation Login","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WORKSTATION LOGIN]","To Workstation Login","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WORKSTATION PASSWORD]","Range on Workstation Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WORKSTATION PASSWORD]","From Workstation Password","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WORKSTATION PASSWORD]","To Workstation Password","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WT FAILED LOGIN ATTEMPTS]","Range on Wt Failed Login Attempts","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WT FAILED LOGIN ATTEMPTS]","From Wt Failed Login Attempts","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WT FAILED LOGIN ATTEMPTS]","To Wt Failed Login Attempts","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WT LAST LOGIN ATTEMPT DATE]","Range on Wt Last Login Attempt Date","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WT LAST LOGIN ATTEMPT DATE]","From Wt Last Login Attempt Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WT LAST LOGIN ATTEMPT DATE]","To Wt Last Login Attempt Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WT LAST LOGIN ATTEMPT TIME]","Range on Wt Last Login Attempt Time","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WT LAST LOGIN ATTEMPT TIME]","From Wt Last Login Attempt Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WT LAST LOGIN ATTEMPT TIME]","To Wt Last Login Attempt Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WT PASSWORD]","Range on Wt Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WT PASSWORD]","From Wt Password","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WT PASSWORD]","To Wt Password","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WT PASSWORD DATE]","Range on Wt Password Date","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WT PASSWORD DATE]","From Wt Password Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WT PASSWORD DATE]","To Wt Password Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WT PASSWORD METHOD]","Range on Wt Password Method","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WT PASSWORD METHOD]","From Wt Password Method","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WT PASSWORD METHOD]","To Wt Password Method","","",0,34,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ABLE TO APPROVE] FALSE","Select when Able To Approve False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ABLE TO APPROVE] TRUE","Select when Able To Approve True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ADMIN EDITS] FALSE","Select when Admin Edits False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ADMIN EDITS] TRUE","Select when Admin Edits True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ALLOW BEGINNING BALANCES] FALSE","Select when Allow Beginning Balances False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ALLOW BEGINNING BALANCES] TRUE","Select when Allow Beginning Balances True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ALLOW MULTIPLE ENTRIES] FALSE","Select when Allow Multiple Entries False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ALLOW MULTIPLE ENTRIES] TRUE","Select when Allow Multiple Entries True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ALLOW POST ACTIVITY] FALSE","Select when Allow Post Activity False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ALLOW POST ACTIVITY] TRUE","Select when Allow Post Activity True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][APPROVE REQUISITIONS] FALSE","Select when Approve Requisitions False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][APPROVE REQUISITIONS] TRUE","Select when Approve Requisitions True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][AUTO COMPLETE] FALSE","Select when Auto Complete False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][AUTO COMPLETE] TRUE","Select when Auto Complete True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][BUDGET OVERRIDE] FALSE","Select when Budget Override False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][BUDGET OVERRIDE] TRUE","Select when Budget Override True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][DISENGAGE WORK FLOW NAVIGATION] FALSE","Select when Disengage Work Flow Navigation False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][DISENGAGE WORK FLOW NAVIGATION] TRUE","Select when Disengage Work Flow Navigation True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][DISPLAY PURGE FUNCTIONS] FALSE","Select when Display Purge Functions False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][DISPLAY PURGE FUNCTIONS] TRUE","Select when Display Purge Functions True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][EDIT TEXT BROWSER] FALSE","Select when Edit Text Browser False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][EDIT TEXT BROWSER] TRUE","Select when Edit Text Browser True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][HELP INCLUSION TYPE] FALSE","Select when Help Inclusion Type False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][HELP INCLUSION TYPE] TRUE","Select when Help Inclusion Type True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][IGNORE RESTRICTED PROCESS CONSTRAINTS] FALSE","Select when Ignore Restricted Process Constraints False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][IGNORE RESTRICTED PROCESS CONSTRAINTS] TRUE","Select when Ignore Restricted Process Constraints True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][JOURNAL ENTRY APPROVAL] FALSE","Select when Journal Entry Approval False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][JOURNAL ENTRY APPROVAL] TRUE","Select when Journal Entry Approval True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][PREFER TEXT MESSAGE] FALSE","Select when Prefer Text Message False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][PREFER TEXT MESSAGE] TRUE","Select when Prefer Text Message True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][SUBMIT REQUISITIONS] FALSE","Select when Submit Requisitions False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][SUBMIT REQUISITIONS] TRUE","Select when Submit Requisitions True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][SUPERVISOR] FALSE","Select when Supervisor False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][SUPERVISOR] TRUE","Select when Supervisor True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TEXT MESSAGE] FALSE","Select when Text Message False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TEXT MESSAGE] TRUE","Select when Text Message True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TLS] FALSE","Select when Tls False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TLS] TRUE","Select when Tls True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TLS CERT CHECK] FALSE","Select when Tls Cert Check False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TLS CERT CHECK] TRUE","Select when Tls Cert Check True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TLS START] FALSE","Select when Tls Start False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][TLS START] TRUE","Select when Tls Start True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][USE BROWSE LIST LABEL] FALSE","Select when Use Browse List Label False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][USE BROWSE LIST LABEL] TRUE","Select when Use Browse List Label True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][VOUCHER APPROVAL] FALSE","Select when Voucher Approval False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][VOUCHER APPROVAL] TRUE","Select when Voucher Approval True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][WORKSTATION VALIDATED] FALSE","Select when Workstation Validated False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][WORKSTATION VALIDATED] TRUE","Select when Workstation Validated True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][WT FORCE PASSWORD CHANGE] FALSE","Select when Wt Force Password Change False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][WT FORCE PASSWORD CHANGE] TRUE","Select when Wt Force Password Change True","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(20+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [LOGSTAT RANGE SCREEN], Can Not Save Maintain Master!");
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
void LOGSTAT()
{
	RPT_LOGSTAT();
	SCN_LOGSTAT_RANGE_SCREEN();
}
