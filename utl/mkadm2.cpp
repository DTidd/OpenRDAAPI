/* UTILITIES's - Make (LPS SCRIPT) */
#include <mkmsc.hpp>

extern char *module;
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(S_IRWXU|S_IRWXG)
#endif /* Not LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(00770)
#endif /* LINUX || LINUX2_2 || UBUNTU_OS*/

static void LPS_ADM_SCRIPT()
{
	FILE *fp;
	char *name=NULL;
	int fd=(-1);

	name=Rmalloc(12);
#ifdef WIN32
	sprintf(name,"lps.bat");
#endif
#ifdef RS6000
	sprintf(name,"lps.rsc");
#endif
#ifdef SCO50
	sprintf(name,"lps.sco");
#endif
#ifdef SCO50P
	sprintf(name,"lps.sco");
#endif
#ifdef UNIXWARE21
	sprintf(name,"lps.unw");
#endif
#ifdef UNIXWARE7
	sprintf(name,"lps.unw");
#endif
#ifdef LINUX
	sprintf(name,"./scripts/lps");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(name,"./scripts/lps");
#endif
	fp=fopen(name,"w+");
	if(fp!=NULL)
	{
#ifdef UNIXWARE7
#endif
#ifdef LINUX
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
/* Script C code goes here like example below
		fprintf(fp,"done\n"); */
		fprintf(fp,"ps aux | grep $*\n"); 
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
static void LPS_ADM_DATA_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("LPS SCRIPT","LPS.EXT");
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
void LPS_SCRIPT()
{
	LPS_ADM_SCRIPT();
	LPS_ADM_DATA_MENU();
}
