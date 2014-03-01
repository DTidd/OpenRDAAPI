/* UTILITIES's - Make (KILLWHO SCRIPT) */
#include <mkmsc.hpp>

extern char *module;
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(S_IRWXU|S_IRWXG)
#endif /* Not LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(00770)
#endif /* LINUX || LINUX2_2 || UBUNTU_OS*/

static void KILLWHO_ADM_SCRIPT()
{
	FILE *fp;
	char *name=NULL;
	int fd=(-1);

	name=Rmalloc(12);
#ifdef WIN32
	sprintf(name,"killwho.bat");
#endif
#ifdef RS6000
	sprintf(name,"killwho.rsc");
#endif
#ifdef SCO50
	sprintf(name,"killwho.sco");
#endif
#ifdef SCO50P
	sprintf(name,"killwho.sco");
#endif
#ifdef UNIXWARE21
	sprintf(name,"killwho.unw");
#endif
#ifdef UNIXWARE7
	sprintf(name,"killwho.unw");
#endif
#ifdef LINUX
	sprintf(name,"./scripts/killwho");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(name,"./scripts/killwho");
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
		fprintf(fp,"kill -9 `ps aux | grep $* | cut -c10-14`\n"); 
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
static void KILLWHO_ADM_DATA_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("KILLWHO SCRIPT","KILLWHO.EXT");
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
void KILLWHO_SCRIPT()
{
	KILLWHO_ADM_SCRIPT();
	KILLWHO_ADM_DATA_MENU();
}
