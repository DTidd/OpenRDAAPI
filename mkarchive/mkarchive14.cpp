/* ARCHIVE's - Make (COPYARCHIVE) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void LST_COPYARCHIVE_WORKGROUPS()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","COPYARCHIVE WORKGROUPS");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=0;
		ScrolledListDFincvir(list,"ARCHIVE","ARCHWORK","",0);
	temp1=Rmalloc(46+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHWORK][DELETEFLAG][1,1,1]=FALSE)");
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
		list->conmod=stralloc("NEW WORK GROUP");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[ARCHIVE][ARCHWORK][WORK GROUP][1,1,15]");
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
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [COPYARCHIVE WORKGROUPS], Can Not Save Scrolled List!");
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
static void DEF_COPYARCHIVE_RANGE_SCREEN()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("ARCHIVE","COPYARCHIVE RANGE SCREEN");
	if(def!=NULL)
	{
		def->input_focus=stralloc("COPYARCHIVE WORKGROUPS");
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
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
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
		addDefaultFieldSTRING(def,"NEW WORK GROUP",TRUE,1,"");
		addDefaultFieldSHORT(def,"STYLE",TRUE,7,0);
		addDefaultFieldINT(def,"COPYARCHIVE WORKGROUPS",TRUE,13,0);
		addDefaultFieldINT(def,"COPYARCHIVE STYLES",TRUE,13,0);

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
			temp1=Rmalloc(24+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [COPYARCHIVE RANGE SCREEN], Can Not Save Screen Defaults!");
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
static void MNU_ARCHIVE_COPYARCHIVE()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE COPYARCHIVE","doreport.EXT ARCHIVE \"COPYARCHIVE\"");
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
static void SCN_COPYARCHIVE_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","COPYARCHIVE RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","New Work Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"COPYARCHIVE WORKGROUPS","Work Groups","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"COPYARCHIVE STYLES","Styles","","",1,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,23,NULL,NULL,"([NEW WORK GROUP]!=\"\")",NULL);
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
			temp1=Rmalloc(24+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [COPYARCHIVE RANGE SCREEN], Can Not Save Maintain Master!");
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
static void LST_COPYARCHIVE_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","COPYARCHIVE STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=3;
		list->conmod=stralloc("STYLE");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
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
		list->list=APPlibNEW();
		addAPPlib(list->list,"Copy File");
		addAPPlib(list->list,"Move File");

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
			temp1=Rmalloc(18+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [COPYARCHIVE STYLES], Can Not Save Scrolled List!");
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
static void RPT_COPYARCHIVE()
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
	rptname=Rmalloc(11+1);
	sprintf(rptname,"COPYARCHIVE");
	t=Rmalloc(57+1);
	sprintf(t,"Copy/Move Archive Records & Documents between Work Groups");
	desc=Rmalloc(80+1);
	sprintf(desc,"This output definition is used to copy/move Archive records between Work Groups.");
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

/* ARCHIVE ARCHIVES SEARCH FILE */

	temp1=Rmalloc(95+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHIVES][DELETEFLAG]=FALSE) AND ([ARCHIVE][ARCHIVES][WORK GROUP]!=[NEW WORK GROUP])");
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

	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHWORK",2,"ARCHIVE","ARCHIVES","ARCHWORK KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[NEW WORK GROUP]");
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
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHWORK",2,"ARCHIVE","ARCHIVES","ARCHWORK KEY 1",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[ARCHIVES][WORK GROUP]");
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







/* VIRTUAL FIELD DEFINITIONS */

/* NEW WORK GROUP VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"NEW WORK GROUP",1,15,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* STYLE VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"STYLE",7,1,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* TO FILENAME VIRTUAL FIELD */
	temp1=Rmalloc(82+1);
	sprintf(temp1,"%s",
		"\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY]+\"/\"+[ARCHIVE][ARCHIVES][FILENAME]");
	addvirtuallineADV(tmp,"TO FILENAME",1,200,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FROM FILENAME VIRTUAL FIELD */
	temp1=Rmalloc(100+1);
	sprintf(temp1,"%s",
		"\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY][2,1,999]+\"/\"+[ARCHIVE][ARCHIVES][FILENAME]");
	addvirtuallineADV(tmp,"FROM FILENAME",1,200,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"LAST STYLE",3,"[STYLE]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [ARCHIVE][ARCHIVES]",FALSE,NULL,0,NULL);

	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"([STYLE]=0)");
	imageoff=addimageline(tmp,"DETAIL LINES","COPYFILE",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[FROM FILENAME]",0,"FROM FILENAME");
		addimagepval(image,0,"[TO FILENAME]",0,"TO FILENAME");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"([STYLE]=1)");
	imageoff=addimageline(tmp,"DETAIL LINES","MOVEFILE",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[FROM FILENAME]",0,"FROM FILENAME");
		addimagepval(image,0,"[TO FILENAME]",0,"TO FILENAME");
	}


	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [ARCHIVE][ARCHIVES]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]",0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]");
		addimagepval(image,1,"FALSE",0,"[ARCHIVE][ARCHIVES][DELETEFLAG]");
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][DESCRIPTION]",0,"[ARCHIVE][ARCHIVES][DESCRIPTION]");
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][FILENAME]",0,"[ARCHIVE][ARCHIVES][FILENAME]");
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][SUBJECT]",0,"[ARCHIVE][ARCHIVES][SUBJECT]");
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][CLASS]",0,"[ARCHIVE][ARCHIVES][CLASS]");
		addimagepval(image,0,"[LOGIN IDENTIFICATION]",0,"[ARCHIVE][ARCHIVES][SOURCE USER]");
		addimagepval(image,1,"3",0,"[ARCHIVE][ARCHIVES][SOURCE]");
		addimagepval(image,0,"[SYSTEM DATE-YYYY]",0,"[ARCHIVE][ARCHIVES][DATE]");
		addimagepval(image,0,"[SYSTEM TIME]",0,"[ARCHIVE][ARCHIVES][TIME]");
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][TYPE]",0,"[ARCHIVE][ARCHIVES][TYPE]");
		addimagepval(image,0,"[NEW WORK GROUP]",0,"[ARCHIVE][ARCHIVES][WORK GROUP]");
	}


	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [ARCHIVE][ARCHIVES]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][WORK GROUP]",0,"[ARCHIVE][ARCHIVES][WORK GROUP]");
		addimagepval(image,0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]",0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]");
		addimagepval(image,0,"[STYLE]",0,"[ARCHIVE][ARCHIVES][DELETEFLAG]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [ARCHIVE][ARCHIVES]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+7+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [ARCHIVE] Report [COPYARCHIVE], Can Not Save Report!");
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
static void LST_TO_ARCHIVE_WORKGROUP()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","TO ARCHIVE WORKGROUP");
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
		list->conmod=stralloc("TO [ARCHWORK][WORK GROUP]");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[ARCHIVE][ARCHWORK][WORK GROUP][1,1,15]");
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
			temp1=Rmalloc(20+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [TO ARCHIVE WORKGROUP], Can Not Save Scrolled List!");
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
static void LST_FROM_ARCHIVE_WORKGROUP()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","FROM ARCHIVE WORKGROUP");
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
		list->conmod=stralloc("FROM [ARCHWORK][WORK GROUP]");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"[ARCHIVE][ARCHWORK][WORK GROUP][1,1,15]");
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
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [FROM ARCHIVE WORKGROUP], Can Not Save Scrolled List!");
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
void COPYARCHIVE()
{
	RPT_COPYARCHIVE();
	MNU_ARCHIVE_COPYARCHIVE();
	SCN_COPYARCHIVE_RANGE_SCREEN();
	LST_COPYARCHIVE_STYLES();
	LST_COPYARCHIVE_WORKGROUPS();
	DEF_COPYARCHIVE_RANGE_SCREEN();
	LST_FROM_ARCHIVE_WORKGROUP();
	LST_TO_ARCHIVE_WORKGROUP();
}
