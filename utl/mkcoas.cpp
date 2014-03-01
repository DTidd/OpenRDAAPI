/* mkcoas.c - Make Chart of Accounts EZ Virtual Fields */
#ifndef WIN32
#define __NAM__ "mkcoas.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkcoas.exe"
#endif
#include <app.hpp>
#include <mkvir.hpp>

/*ADDLIB mkvir */
/*ADDLIB mklib */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

APPlib *modlst=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
static void make_ezvirt(int,char **);

static void PurgeCOAs(char *mod)
{
	APPlib *list=NULL;
	char *libx=NULL;
	int x=0;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(mod)+12);
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,(mod!=NULL ? mod:""));
#endif /* WIN32 */
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,(mod!=NULL ? mod:""));
#endif /* Not WIN32 */
	
	list=getlibnames(libx,FALSE);
	if(list!=NULL)
	{
		if(list->libs!=NULL)
		{
		for(x=0;x<list->numlibs;++x)
		{
			if(!strncmp(list->libs[x],"BALANCE SHEET",13))
			{
				if(diagapps) {prterr("PurgeCOAs(): Purging [%s]:%d from library [%s].",list->libs[x],x,libx);TRACE;}
				deletelibbin(libx,list->libs[x]);
			} else if(!strncmp(list->libs[x],"DIMENSION",9))
			{
				if(diagapps) {prterr("PurgeCOAs(): Purging [%s]:%d from library [%s].",list->libs[x],x,libx);TRACE;}
				deletelibbin(libx,list->libs[x]);
			} else if(!strncmp(list->libs[x],"EXPENDITURE",11))
			{
				if(diagapps) {prterr("PurgeCOAs(): Purging [%s]:%d from library [%s].",list->libs[x],x,libx);TRACE;}
				deletelibbin(libx,list->libs[x]);
			} else if(!strncmp(list->libs[x],"REVENUE",7))
			{ 
				if(diagapps) {prterr("PurgeCOAs(): Purging [%s]:%d from library [%s].",list->libs[x],x,libx);TRACE;}
				deletelibbin(libx,list->libs[x]);
			} else if(!strncmp(list->libs[x],"BEGBAL",6))
			{
				if(diagapps) {prterr("PurgeCOAs(): Purging [%s]:%d from library [%s].",list->libs[x],x,libx);TRACE;}
				deletelibbin(libx,list->libs[x]);
			} else {
			}
		}	
		}
		freeapplib(list);
	}
	if(libx!=NULL) Rfree(libx);
}
static void PurgeCOAscb(RDArsrc *mainrsrc)
{
	int selected=0;

	if(modlst!=NULL)
	{
		FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected);
		PurgeCOAs(modlst->libs[selected]);
	}
}
static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
	ShutdownSubsystems();
}

static void printerrorlist(RDArsrc *parent,RDA_PFILE *fp)
{
	int x;
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		RDA_fprintf(fp,"                         XPERT MAKE REPORTS ERROR LIST          Date: %*s\n",8,date);
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\n\n",
			 ((80-(14+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(14+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		for(x=0;x<errorlist->numlibs;++x)
		{
			RDA_fprintf(fp,"(%5d) %s\n",(x+1),errorlist->libs[x]);
		}
		RDA_fprintf(fp,"\f");
	}
}

void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
			printerrorlist(parent,fp);
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}

void print_list(RDArsrc *parent,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(diaggui)
	{
		prterr("DIAG printerrorlist Displaying the Print Screen in preparation to print Error List.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,parent);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print(prsrc);
	}
}

static APPlib *getfilemodules()
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	APPlib *tmplst=NULL;

	if(tmplst!=NULL) freeapplib(tmplst);
	tmplst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(tmplst,tmp);
	}
	if(tmplst->numlibs<1)
	{
		addAPPlib(tmplst,"No Database Files");
		selectedm=0;
	} else {
		SORTAPPlib(tmplst);
		if(selectedm>=tmplst->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	return(tmplst);
}
static void quitfinrpt(RDArsrc *mainrsrc)
{
	if(modlst!=NULL) freeapplib(modlst);
	if(errorlist!=NULL) freeapplib(errorlist);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	ShutdownSubsystems();
}
static void select_mod(RDArsrc *mainrsrc)
{
	int selected=0;
	char *temp=NULL;
	RDArsrc *tmprsrc=NULL;

	if(modlst!=NULL)
	{
		FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected);
		temp=Rmalloc(RDAstrlen(modlst->libs[selected])+26);
		sprintf(temp,"Creating %s COA EZ Virtuals",(modlst->libs[selected]!=NULL?modlst->libs[selected]:""));
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","UTILITIES",temp,NULL,modlst->numlibs);
		if(temp!=NULL) Rfree(temp);
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				prterr("Error Cannot Create Diagnostic Screen.");
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
		PurgeCOAs(modlst->libs[selected]);
		if(!RDAstrcmp(modlst->libs[selected],"FINMGT"))
		{
			CreateBEGBALs(modlst->libs[selected]);
		}
		CreateCOAs(modlst->libs[selected]);
		FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
		updatersrc(mainrsrc,"ERROR LIST");
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,TRUE);
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
}
static void selectall(RDArsrc *mainrsrc)
{
	int x;
	RDArsrc *tmprsrc=NULL;

	if(modlst!=NULL)
	{
		if(mainrsrc!=NULL)
		{
			tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","UTILITIES","Creating COA EZ Virtuals",NULL,modlst->numlibs);
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					prterr("Error Cannot Create Diagnostic Screen.");
				} else {
					ForceWindowUpdate(tmprsrc);
				}
			}
		}
		for(x=0;x<modlst->numlibs;++x)
		{
			PurgeCOAs(modlst->libs[x]);
			if(!RDAstrcmp(modlst->libs[x],"FINMGT"))
			{
				CreateBEGBALs(modlst->libs[x]);
			}
		 	CreateCOAs(modlst->libs[x]);
		}
		if(mainrsrc!=NULL)
		{
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
	}
}
static void make_ezvirt(int argc,char **argv)
{
	int x=0,s=0;

	if(argc==2 && !RDAstrcmp(argv[1],"SELECT ALL"))
	{
		selectall(NULL);
	} else {
		for(x=1;x<argc;++x)
		{
			for(s=0; s<modlst->numlibs; ++s)
			{
				if(!RDAstrcmp(modlst->libs[s],argv[x])) break;
			}
			if(s<modlst->numlibs)
			{
				PurgeCOAs(argv[x]);
				if(!RDAstrcmp(argv[x],"FINMGT"))
				{
					CreateBEGBALs(argv[x]);
				}
				CreateCOAs(argv[x]);
			}
			else
			{
				prterr("Error, module %s not found.", argv[x]);
			}
		}
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x=0;

	if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE COA EZ VIRTUAL FIELDS")) 
	{
		return;
	}
	if(argc>1)
	{
		modlst=getfilemodules();
		make_ezvirt(argc,argv);
		if(modlst!=NULL) freeapplib(modlst);
		if(errorlist!=NULL) freeapplib(errorlist);
		ShutdownSubsystems();
	} else {
		mainrsrc=RDArsrcNEW("UTILITIES","MAKE COA EZ VIRTUAL FIELDS");
		modlst=getfilemodules();
		addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,NULL,modlst->numlibs,&modlst->libs,NULL);
		errorlist=APPlibNEW();
		addAPPlib(errorlist,"No Errors Detected");
		addlstrsrc(mainrsrc,"ERROR LIST",&x,TRUE,NULL,errorlist->numlibs,&errorlist->libs,NULL);
/*lint -e611 */
		addbtnrsrc(mainrsrc,"PRINT ERROR LIST",TRUE,print_list,(void *)printerrorlistcb);
/*lint +e611 */
		addbtnrsrc(mainrsrc,"SELECT ALL",TRUE,selectall,NULL);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,select_mod,NULL);
		addbtnrsrc(mainrsrc,"PURGE COAS",TRUE,PurgeCOAscb,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfinrpt,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitfinrpt,NULL,TRUE);
	}
}
