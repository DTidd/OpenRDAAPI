/* SECURITY's - Make (SETBROWSER) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
#include <mkdef.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_SETBROWSER()
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
	rptname=Rmalloc(10+1);
	sprintf(rptname,"SETBROWSER");
	t=Rmalloc(29+1);
	sprintf(t,"Set Browser Type in User File");
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
	adddisplayADV5(tmp,(double)8.500000,(double)11.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,1,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* SECURITY USERS SEARCH FILE */

	temp1=Rmalloc(37+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","USERS",0,NULL,NULL,"USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);






/* VIRTUAL FIELD DEFINITIONS */

/* SET BROWSER TYPE VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"SET BROWSER TYPE",7,2,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][USERS]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][USERS]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][USER IDENTIFICATION]",0,"[SECURITY][USERS][USER IDENTIFICATION]");
		addimagepval(image,0,"[SET BROWSER TYPE]",0,"[SECURITY][USERS][BROWSER TYPE]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][USERS]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [SETBROWSER], Can Not Save Report!");
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
static void SCN_SETBROWSER_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","SETBROWSER RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Set Browser Type to:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"USERS BROWSER TYPES LIST","","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER IDENTIFICATION]","Range on User ID","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER NAME]","Range on User Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER NAME]","From User Name","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER NAME]","To User Name","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEPARTMENT]","Range on Department","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEPARTMENT]","From Department","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEPARTMENT]","To Department","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(23+15+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [SETBROWSER RANGE SCREEN], Can Not Save Maintain Master!");
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
static void MNU_SECURITY_SETBROWSER()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("SECURITY SETBROWSER","doreport.EXT SECURITY \"SETBROWSER\"");
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
static void LST_USERS_BROWSER_TYPES_LIST()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("SECURITY","USERS BROWSER TYPES LIST");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
		ScrolledListDFincvir(list,"SECURITY","USERS","",0);
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
		list->conmod=stralloc("SET BROWSER TYPE");
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
#ifndef WIN32
		addAPPlib(list->list,"Opera");
		addAPPlib(list->list,"Mozilla");
		addAPPlib(list->list,"Firefox");
#endif
#ifdef WIN32
		addAPPlib(list->list,"Netscape");
		addAPPlib(list->list,"Mozilla");
		addAPPlib(list->list,"Explorer");
		addAPPlib(list->list,"Firefox");
#endif

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
			temp1=Rmalloc(24+8+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [SECURITY] RDAScrolledList [USERS BROWSER TYPES LIST], Can Not Save Scrolled List!");
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
static void DEF_SETBROWSER_RANGE_SCREEN()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("SECURITY","SETBROWSER RANGE SCREEN");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldCHAR(def,"SELECT [USERS][DELETEFLAG] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][USER IDENTIFICATION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][USER IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][PHONE NUMBER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][PHONE NUMBER]",TRUE,3,"");
		addDefaultFieldSTRING(def,"TO [USERS][PHONE NUMBER]",TRUE,3,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][PHONE EXTENSION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][PHONE EXTENSION]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][PHONE EXTENSION]",TRUE,1,"");
		addDefaultFieldCHAR(def,"SELECT [USERS][HELP INCLUSION TYPE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][HELP INCLUSION TYPE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][SITE IDENTIFICATION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][SITE IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][SITE IDENTIFICATION]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][USER NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][USER NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][USER NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][DEFAULT PRINTER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][DEFAULT PRINTER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][DEFAULT PRINTER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][DEFAULT FILE OUTPUT DEVICE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][DEFAULT FILE OUTPUT DEVICE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][DEFAULT FILE OUTPUT DEVICE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][DEPARTMENT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][DEPARTMENT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][DEPARTMENT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][HOST]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][HOST]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][HOST]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][EMAIL]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][EMAIL]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][EMAIL]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][GTKRC]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][GTKRC]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][GTKRC]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][WEB LOGIN]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][WEB LOGIN]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][WEB LOGIN]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][WEB PASSWORD]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][WEB PASSWORD]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][WEB PASSWORD]",TRUE,1,"");
		addDefaultFieldCHAR(def,"SELECT [USERS][BUDGET OVERRIDE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][BUDGET OVERRIDE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][SUPERVISOR] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][SUPERVISOR] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][EDIT TEXT BROWSER] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][EDIT TEXT BROWSER] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][ABLE TO APPROVE] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][ABLE TO APPROVE] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][APPROVAL LIMIT]",TRUE,10,0);
		addDefaultFieldDOUBLE(def,"FROM [USERS][APPROVAL LIMIT]",TRUE,2,0.000000);
		addDefaultFieldDOUBLE(def,"TO [USERS][APPROVAL LIMIT]",TRUE,2,0.000000);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][DOMENU STYLE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [USERS][DOMENU STYLE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [USERS][DOMENU STYLE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][BROWSER TYPE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [USERS][BROWSER TYPE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [USERS][BROWSER TYPE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][SMTP SERVER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][SMTP SERVER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][SMTP SERVER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][SMTP PORT]",TRUE,10,0);
		addDefaultFieldINT(def,"FROM [USERS][SMTP PORT]",TRUE,8,0);
		addDefaultFieldINT(def,"TO [USERS][SMTP PORT]",TRUE,8,0);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][SMTP AUTH]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][SMTP AUTH]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][SMTP AUTH]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][CONNECT TIMEOUT]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [USERS][CONNECT TIMEOUT]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [USERS][CONNECT TIMEOUT]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][PROTOCOL]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][PROTOCOL]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][PROTOCOL]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][USERNAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][USERNAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][USERNAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][PASSWORD]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][PASSWORD]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][PASSWORD]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][NTLMDOMAIN]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][NTLMDOMAIN]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][NTLMDOMAIN]",TRUE,1,"");
		addDefaultFieldCHAR(def,"SELECT [USERS][TLS] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][TLS] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][TLS START] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][TLS START] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][TLS TRUST FILE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][TLS TRUST FILE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][TLS TRUST FILE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][TLS KEY FILE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][TLS KEY FILE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][TLS KEY FILE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [USERS][TLS CRT FILE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][TLS CRT FILE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [USERS][TLS CRT FILE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"SELECT [USERS][TLS CERT CHECK] TRUE",TRUE,10,1);
		addDefaultFieldCHAR(def,"SELECT [USERS][TLS CERT CHECK] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [USERS][JOB DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [USERS][JOB DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [USERS][JOB DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSHORT(def,"SET BROWSER TYPE",TRUE,7,1);
#ifndef WIN32
		addDefaultFieldINT(def,"USERS BROWSER TYPES LIST",TRUE,13,1);
#endif
#ifndef WIN32
		addDefaultFieldINT(def,"USERS BROWSER TYPES LIST",TRUE,13,2);
#endif

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
			temp1=Rmalloc(23+8+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [SECURITY] Screen [SETBROWSER RANGE SCREEN], Can Not Save Screen Defaults!");
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

void RPTSETBROWSER()
{
	RPT_SETBROWSER();
	SCN_SETBROWSER_RANGE_SCREEN();
	MNU_SECURITY_SETBROWSER();
	LST_USERS_BROWSER_TYPES_LIST();
	DEF_SETBROWSER_RANGE_SCREEN();
}
