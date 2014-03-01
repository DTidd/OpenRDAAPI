/* dorestore.c - Program to Restore Xpert Package */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dorestore.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dorestore.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="DATABASE",*drvspc=NULL;

static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(drvspc!=NULL) Rfree(drvspc);
	ShutdownSubsystems();
	std::exit;
}
static void dorestore(RDArsrc *r)
{
#ifndef WIN32
	char *command=NULL;
	char *dsp2use=NULL;

	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	dsp2use=RDA_GetEnv("DISPLAY");
	command=Rmalloc(RDAstrlen(drvspc)+200);
#ifdef LINUX
	sprintf(command,"/usr/X11R6/bin/xterm -T \"RESTORE DATA\" -display %s -e restore.lnx %s %s",(dsp2use!=NULL ? dsp2use:""),(adddashes(XPERT_SETUP->company_name)),(drvspc!=NULL ? drvspc:"")); 
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(command,"/usr/X11R6/bin/xterm -T \"RESTORE DATA\" -display %s -e restore.lnx %s %s",(dsp2use!=NULL ? dsp2use:""),(adddashes(XPERT_SETUP->company_name)),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef RS6000
	sprintf(command,"/usr/bin/X11/aixterm -ls -fn pc8x16 -T \"RESTORE DATA\" -d %s -e restore.rsc %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef SCO50
	sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"RESTORE DATA\" -d %s -e restore.sco %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef SCO50P
	sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"RESTORE DATA\" -d %s -e restore.sco %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef UNIXWARE21
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"RESTORE DATA\" -d %s -e restore.unw %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef UNIXWARE7
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"RESTORE DATA\" -d %s -e restore.unw %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef SUNX86
	sprintf(command,"/usr/X/bin/xterm -T \"RESTORE DATA\" -display %s -e restore.sun %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
	system_command(command);
	if(command!=NULL) Rfree(command);
#endif
#ifdef WIN32
	APPlib *args=NULL;

	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	args=APPlibNEW();
	addAPPlib(args,"x");
	sprintf(stemp,"%s.7z",(drvspc!=NULL ? drvspc:""));
	addAPPlib(args,stemp);
	ADVExecute2Program("7zg",args,NULL);
	if(args!=NULL) freeapplib(args);
	killwindow(r);
	quitfunc(r);
#endif
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"RESTORE DATA")) 
	{
		return;
	}
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	/* drvspc=stralloc("/dev/st0"); */
	drvspc=stralloc("cdrom");
#endif
#ifdef WIN32
	drvspc=adddashes(XPERT_SETUP->company_name);
#endif
	mainrsrc=RDArsrcNEW(module,"RESTORE DATA");
	addstdrsrc(mainrsrc,"DRIVE SPEC",VARIABLETEXT,0,drvspc,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,dorestore,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
