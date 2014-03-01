/* ARCHIVE's - Make (COPYGROUPUSER) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_COPYGROUPUSER()
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
	rptname=Rmalloc(13+1);
	sprintf(rptname,"COPYGROUPUSER");
	t=Rmalloc(31+1);
	sprintf(t,"Copies Archive Work Group Users");
	desc=Rmalloc(3+1);
	sprintf(desc,"");
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
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,1,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* ARCHIVE ARCHUSER SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHUSER][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHUSER",0,NULL,NULL,"ARCHUSER KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[FROM WORK GROUP]");
		addsearchconfld(s,"USER IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);


/* ARCHIVE ARCHWORK SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHWORK",2,"ARCHIVE","ARCHUSER","ARCHWORK KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[ARCHUSER][WORK GROUP]");
	}
	if(t!=NULL) Rfree(t);


/* ARCHIVE ARCHWORK SEARCH FILE */

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHWORK",2,"ARCHIVE","ARCHUSER","ARCHWORK KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[TO WORK GROUP]");
	}
	if(t!=NULL) Rfree(t);






/* VIRTUAL FIELD DEFINITIONS */

/* FROM WORK GROUP VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"FROM WORK GROUP",1,15,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* TO WORK GROUP VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"TO WORK GROUP",1,15,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"FALSE",0,"[ARCHIVE][ARCHUSER][DELETEFLAG]");
		addimagepval(image,0,"[ARCHIVE][ARCHUSER][READ]",0,"[ARCHIVE][ARCHUSER][READ]");
		addimagepval(image,0,"[ARCHIVE][ARCHUSER][USER IDENTIFICATION]",0,"[ARCHIVE][ARCHUSER][USER IDENTIFICATION]");
		addimagepval(image,0,"[ARCHIVE][ARCHUSER][WRITE]",0,"[ARCHIVE][ARCHUSER][WRITE]");
		addimagepval(image,0,"[TO WORK GROUP]",0,"[ARCHIVE][ARCHUSER][WORK GROUP]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [ARCHIVE][ARCHUSER]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+7+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [ARCHIVE] Report [COPYGROUPUSER], Can Not Save Report!");
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
static void MNU_ARCHIVE_COPYGROUPUSER()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE COPYGROUPUSER","doreport.EXT ARCHIVE \"COPYGROUPUSER\"");
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
static void LST_COPYGROUPUSER_FROM_WORK_GROUP()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","COPYGROUPUSER FROM WORK GROUP");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=0;
		ScrolledListDFincvir(list,"ARCHIVE","ARCHWORK","",0);
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE)");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("ARCHIVE");
		list->ffinfo->file=stralloc("ARCHWORK");
		list->ffinfo->keyname=stralloc("ARCHWORK KEY 1");
		list->contype=3;
		list->conmod=stralloc("FROM WORK GROUP");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(31+1);
	sprintf(temp1,"%s",
		"[ARCHIVE][ARCHWORK][WORK GROUP]");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(29+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [COPYGROUPUSER FROM WORK GROUP], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void LST_COPYGROUPUSER_TO_WORK_GROUP()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","COPYGROUPUSER TO WORK GROUP");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=0;
		ScrolledListDFincvir(list,"ARCHIVE","ARCHWORK","",0);
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE)");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("ARCHIVE");
		list->ffinfo->file=stralloc("ARCHWORK");
		list->ffinfo->keyname=stralloc("ARCHWORK KEY 1");
		list->contype=3;
		list->conmod=stralloc("TO WORK GROUP");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(31+1);
	sprintf(temp1,"%s",
		"[ARCHIVE][ARCHWORK][WORK GROUP]");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(27+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [COPYGROUPUSER TO WORK GROUP], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void SCN_COPYGROUPUSER_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","COPYGROUPUSER RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","From Work Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"COPYGROUPUSER FROM WORK GROUP","From Work Group","","",5,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","To Work Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"COPYGROUPUSER TO WORK GROUP","To Work Group","","",5,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHUSER][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHUSER][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHUSER][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHUSER][READ] FALSE","Select when Read False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHUSER][READ] TRUE","Select when Read True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHUSER][WRITE] FALSE","Select when Write False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHUSER][WRITE] TRUE","Select when Write True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		temp3=Rmalloc(53+1);
		sprintf(temp3,"%s",
			"([FROM WORK GROUP]!=\"\") AND ([TO WORK GROUP]!=\"\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,23,temp1,temp2,temp3,temp4);
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
			temp1=Rmalloc(26+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [COPYGROUPUSER RANGE SCREEN], Can Not Save Maintain Master!");
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
void COPYGROUPUSER()
{
	RPT_COPYGROUPUSER();
	MNU_ARCHIVE_COPYGROUPUSER();
	LST_COPYGROUPUSER_FROM_WORK_GROUP();
	LST_COPYGROUPUSER_TO_WORK_GROUP();
	SCN_COPYGROUPUSER_RANGE_SCREEN();
}
