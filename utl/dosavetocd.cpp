/* savetocd.c - Program to save files to CD */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dosavetocd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dosavetocd.exe"
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

char *module="DATABASE",*filesved=NULL;

static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(filesved!=NULL) Rfree(filesved);
	ShutdownSubsystems();
}
static void savetocd(RDArsrc *r)
{
	char *command=NULL;
#ifndef WIN32	
	char *dsp2use=NULL;
#endif

	readallwidgets(r);
	FINDRSCGETSTRING(r,"FILE NAME",&filesved);
#ifndef WIN32	
	dsp2use=RDA_GetEnv("DISPLAY");
#endif
	command=Rmalloc(RDAstrlen(filesved)+200);
#ifdef WIN32
	sprintf(command,"savetocd.bat %s",(filesved!=NULL ? filesved:""));
#endif
#ifdef LINUX
	sprintf(command,"/usr/bin/X11/xterm -T \"SAVE DATA TO CD\"  -display %s -e savetocd.lnx %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(command,"/usr/bin/X11/xterm -T \"SAVE DATA TO CD\"  -display %s -e savetocd.lnx %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
#ifdef RS6000
	sprintf(command,"/usr/bin/X11/aixterm -ls -T \"SAVE DATA TO CD\" -d %s -e savetocd.rsc %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
#ifdef SCO50
	sprintf(command,"/usr/bin/X11/scoterm -ls -T \"SAVE DATA TO CD\" -d %s -e savetocd.sco %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
#ifdef SCO50P
	sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"SAVE DATA TO CD\" -d %s -e savetocd.sco %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
#ifdef UNIXWARE21
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"SAVE DATA TO CD\" -d %s -e savetocd.unw %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
#ifdef UNIXWARE7
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"SAVE DATA TO CD\" -d %s -e savetocd.unw %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
#ifdef SUNX86
	sprintf(command,"/usr/X/bin/xterm -T \"SAVE DATA TO CD\" -display %s -e savetocd.sun %s",(dsp2use!=NULL ? dsp2use:""),(filesved!=NULL ? filesved:"")); 
#endif
	system_command(command);
	if(command!=NULL) Rfree(command);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"SAVE DATA TO CD")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"SAVE DATA TO CD");
	addstdrsrc(mainrsrc,"FILE NAME",VARIABLETEXT,0,NULL,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,savetocd,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
