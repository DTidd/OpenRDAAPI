/* UTILITIES's - Make (TEST SCRIPT) */
#include <mkmsc.hpp>


#include <mix.hpp>


extern char *module; 
char *drvspc=NULL; 

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(S_IRWXU|S_IRWXG)
#endif /* Not LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(00770)
#define MODE_RW_RW_R__	(0664)
#endif /* LINUX || LINUX2_2 || UBUNTU_OS*/

static void TEST_ADM_SCRIPT()
{
	FILE *fp;
	char *name=NULL;
	int fd=(-1);

	name=Rmalloc(12);
#ifdef WIN32
	sprintf(name,"REPLACE_SCR_NAME.bat");
#endif
#ifdef RS6000
	sprintf(name,"REPLACE_SCR_NAME.rsc");
#endif
#ifdef SCO50
	sprintf(name,"REPLACE_SCR_NAME.sco");
#endif
#ifdef SCO50P
	sprintf(name,"REPLACE_SCR_NAME.sco");
#endif
#ifdef UNIXWARE21
	sprintf(name,"REPLACE_SCR_NAME.unw");
#endif
#ifdef UNIXWARE7
	sprintf(name,"REPLACE_SCR_NAME.unw");
#endif
#ifdef LINUX
	sprintf(name,"./scripts/REPLACE_SCR_NAME");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(name,"./scripts/REPLACE_SCR_NAME");
#endif
	fp=fopen(name,"w+");
	if(fp!=NULL)
	{
#ifdef UNIXWARE7
#endif
#ifdef LINUX
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
#endif
#ifdef WIN32
#endif
#ifdef RS6000
#endif
#ifdef SCO50
#endif
#ifdef SCO50P
#endif
#ifdef UNIXWARE21
#endif
		fclose(fp);
#ifndef WIN32
		fd=chmod(name,(mode_t)MODE_RWXRWXR_X);
		if(fd==(-1))
		{
			prterr("Error:  Couldn't set Mode for [%s].",(name!=NULL ? name:""));
		}
#endif
	}
	if(name!=NULL) Rfree(name);
}
static void TEST_ADM_DATA_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("TEST SCRIPT","TEST.EXT");
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

/* 
	END OF MKADM4.C
*/


/* EditVNCSERVERS.c - Program to Backup Xpert Package */
/*lint -library */

/*  char *module="DATABASE",*drvspc=NULL; */

static void quitfunc(RDArsrc r*)
{
	if(r!=NULL) free_rsrc(r);
	if(drvspc!=NULL) Rfree(drvspc);
	ShutdownSubsystems();
	std::exit;
}
static void EditVNCSERVERS(RDArsrc r*) 
{
#ifndef WIN32	
	char *command=NULL;
	char *dsp2use=NULL;

	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	dsp2use=RDA_GetEnv("DISPLAY");
	command=Rmalloc(RDAstrlen(drvspc)+200);
#ifdef LINUX
	sprintf(command,"/usr/X11R6/bin/xterm -T \"BACKUP DATA\" -fn 10x24 -display %s -e backup.lnx %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(command,"/usr/X11R6/bin/xterm -T \"BACKUP DATA\" -fn 10x24 -display %s -e backup.lnx %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef RS6000
#endif
#ifdef SCO50
#endif
#ifdef SCO50P
#endif
#ifdef UNIXWARE21
#endif
#ifdef UNIXWARE7
#endif
#ifdef SUNX86
#endif
	system_command(command);
	if(command!=NULL) Rfree(command);
#endif
#ifdef WIN32
	APPlib *args=NULL;
	
	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	sprintf(stemp,"%s.zip",(drvspc!=NULL ? drvspc:""));
	unlink(stemp);
	args=APPlibNEW();
	addAPPlib(args,"-r");
	addAPPlib(args,drvspc);
	addAPPlib(args,".");
	addAPPlib(args,"-x");
	addAPPlib(args,"\\*.exe");
	addAPPlib(args,"\\*.dll");
	ADVExecute2Program("zip",args,NULL);
	if(args!=NULL) freeapplib(args);
	killwindow(r);
	quitfunc(r);
#endif
}
void EditVNCFile()
{
	RDArsrc *mainrsrc=NULL;

#ifdef RS6000
	drvspc=stralloc("/dev/rmt0");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	/* drvspc=stralloc("/dev/st0"); */
	drvspc=stralloc("cdrom");
#endif
#ifdef WIN32
	drvspc=adddashes(XPERT_SETUP->company_name);
#endif
	mainrsrc=RDArsrcNEW(module,"BACKUP DATA");
	addstdrsrc(mainrsrc,"DRIVE SPEC",VARIABLETEXT,0,drvspc,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,EditVNCSERVERS,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
void TEST_SCRIPT()
{
	TEST_ADM_SCRIPT();
	TEST_ADM_DATA_MENU();
	EditVNCFile();
}

