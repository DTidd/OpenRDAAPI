/* dobackup.c - Program to Backup Xpert Package */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dobackup.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dobackup.exe"
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

char *module="DATABASE",*drvspc=NULL,*cdrw_bus=NULL;
short cdrw_speed=0;
short blank_cdrw;

static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(drvspc!=NULL) Rfree(drvspc);
	if(cdrw_bus!=NULL) Rfree(cdrw_bus);
	ShutdownSubsystems();
	std::exit;
}
static void dobackup(RDArsrc *r)
{
#ifndef WIN32	
	char *command=NULL;
	char *dsp2use=NULL;
	char *libx=NULL;

	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
	cdrw_bus=Rmalloc(20);
	blank_cdrw=FALSE;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	FINDRSCGETSHORT(r,"BLANK CDRW",&blank_cdrw);
	dsp2use=RDA_GetEnv("DISPLAY");
	command=Rmalloc(RDAstrlen(drvspc)+200);

	if(!(strcmp(drvspc,"cdrom")) && (blank_cdrw==TRUE))
	{
		cdrw_bus=NULL;
		gsv=RDAGenericSetupNew("UTILITIES","CDRW BUS");
		if(!getRDAGenericSetupbin(libx,gsv)) 
		{
			cdrw_bus=stralloc(gsv->value.string_value);
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","CDRW SPEED");
		if(!getRDAGenericSetupbin(libx,gsv)) 
		{
			cdrw_speed=*gsv->value.short_value;
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		
	}
	if(libx!=NULL) Rfree(libx);

#ifdef LINUX
	if(!(strcmp(drvspc,"cdrom")) && (blank_cdrw==TRUE)){
		sprintf(command,"/usr/X11R6/bin/xterm -T \'BACKUP DATA\' -fn 10x24 -display %s -e backup.lnx \"%s\" %s %s %d %s",(dsp2use!=NULL ? dsp2use:""),(adddashes(XPERT_SETUP->company_name)),(drvspc!=NULL ? drvspc:""),(cdrw_bus),(cdrw_speed),("TRUE")); 
	} else {
		sprintf(command,"/usr/X11R6/bin/xterm -T \"BACKUP DATA\" -fn 10x24 -display %s -e backup.lnx %s %s",(dsp2use!=NULL ? dsp2use:""),(adddashes(XPERT_SETUP->company_name)),(drvspc!=NULL ? drvspc:"")); 
	}
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	if(!(strcmp(drvspc,"cdrom")) && (blank_cdrw==TRUE)){
		sprintf(command,"/usr/X11R6/bin/xterm -T \"BACKUP DATA\" -fn 10x24 -display %s -e backup.lnx %s %s %s %d %s",(dsp2use!=NULL ? dsp2use:""),(adddashes(XPERT_SETUP->company_name)),(drvspc!=NULL ? drvspc:""),(cdrw_bus),(cdrw_speed),("TRUE")); 
	} else {
		sprintf(command,"/usr/X11R6/bin/xterm -T \"BACKUP DATA\" -fn 10x24 -display %s -e backup.lnx %s %s",(dsp2use!=NULL ? dsp2use:""),(adddashes(XPERT_SETUP->company_name)),(drvspc!=NULL ? drvspc:"")); 
	}
#endif
#ifdef RS6000
	sprintf(command,"/usr/bin/X11/aixterm -ls -fn pc8x16 -T \"BACKUP DATA\" -d %s -e backup.rsc %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef SCO50
	sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"BACKUP DATA\" -d %s -e backup.sco %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef SCO50P
	sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"BACKUP DATA\" -d %s -e backup.sco %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef UNIXWARE21
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"BACKUP DATA\" -d %s -e backup.unw %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef UNIXWARE7
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"BACKUP DATA\" -d %s -e backup.unw %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
#ifdef SUNX86
	sprintf(command,"/usr/X/bin/xterm -T \"BACKUP DATA\" -display %s -e backup.sun %s",(dsp2use!=NULL ? dsp2use:""),(drvspc!=NULL ? drvspc:"")); 
#endif
	system_command(command);
	if(command!=NULL) Rfree(command);
#endif
#ifdef WIN32
	APPlib *args=NULL;
	
	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	sprintf(stemp,"%s.7z",(drvspc!=NULL ? drvspc:""));
	unlink(stemp);
	args=APPlibNEW();
	addAPPlib(args,"a");
	addAPPlib(args,"-t7z");
	addAPPlib(args,"-mmt=on");
	addAPPlib(args,"-ssw");
	addAPPlib(args,drvspc);
	addAPPlib(args,"-xr-!*.exe");
	addAPPlib(args,"-xr-!*.dll");
	addAPPlib(args,".");
#ifndef WIN32
	ADVExecute2Program("7za",args,NULL);
#endif
#ifdef WIN32
	ADVExecute2Program("7zg",args,NULL);
#endif
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

	if(InitializeSubsystems(argc,argv,module,"BACKUP DATA")) 
	{
		return;
	}
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
	addstdrsrc(mainrsrc,"BLANK CDRW",BOOLNS,1,&blank_cdrw,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,dobackup,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
