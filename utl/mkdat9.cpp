#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

static void SCRIPT_datazip_lnx(void);

static void SCRIPT_datazip_lnx()
{
	FILE *fp=NULL;
	char *name=NULL;
        char *temp=NULL;
        int fd=(-1);

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("datazip.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\datazip.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/datazip.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"# script to move the pack directory\n");
	fprintf(fp,"USAGE=\"Error Usage: `basename $0` [No Argument needed] \"\n");
	fprintf(fp,"if [ $# -ne 0 ] ; then \n");
	fprintf(fp,"  echo \" \"\n");
	fprintf(fp,"  echo \"$USAGE\"\n");
	fprintf(fp,"  echo \" \"\n");
	fprintf(fp,"  exit 1\n");
	fprintf(fp,"fi\n");
	fprintf(fp,"printf \">>>>>>> Please, hit Enter to start the zipping the ./data/ directory.\"\n");
	fprintf(fp,"   read USERANS\n");
	fprintf(fp,"echo \n");
	fprintf(fp,"if  [ -d data -a -w data ] ; then\n");
	fprintf(fp," cd data\n");
	fprintf(fp," printf \"\nZipping the `pwd` directory.\n\"\n");
	fprintf(fp," sleep 1\n");
	fprintf(fp," zip -9Trm DATA .  2> /dev/null \n");
	fprintf(fp," cd ..\n");
	fprintf(fp,"else \n");
	fprintf(fp," echo \"data directory does not exist...\" ;\n");
	fprintf(fp,"fi\n");
	fprintf(fp,"echo\n");
	fprintf(fp,"printf \">>>>>>> Please, hit Enter to Exit.\"\n");
	fprintf(fp,"   read USERANS\n");
	fprintf(fp,"#free variables used in this script\n");
	fprintf(fp,"unset USAGE USERANS\n");
	fclose(fp);

	name=Rmalloc(12);
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
        sprintf(name,"datazip.lnx");
#endif

#ifndef WIN32
                fd=chmod(name,(mode_t)MODE_RWXRWX___);
                if(fd==(-1))
                {
                        prterr("Error:  Couldn't set Mode for [%s].",(name!=NULL ? name:""));
                }
#endif

}
static void MNU_ZIP_DATA_DIRECTORY()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ZIP DATA DIRECTORY","xterm -e datazip.lnx");
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

int SCRIPTdatazip()
{
	SCRIPT_datazip_lnx();
	MNU_ZIP_DATA_DIRECTORY();
}
