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

static void chktape()
{
	FILE *fp=NULL;
	char *temp=NULL;
#ifndef WIN32
	int fd=0;
#endif

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("chktape.lnx")+15);
#ifdef LINUX
	sprintf(temp,"%s/chktape.lnx",CURRENTDIRECTORY);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(temp,"%s/chktape.lnx",CURRENTDIRECTORY);
#endif
#ifdef RS6000
	sprintf(temp,"%s/chktape.rsc",CURRENTDIRECTORY);
#endif
#ifdef SCO50
	sprintf(temp,"%s/chktape.sco",CURRENTDIRECTORY);
#endif
#ifdef SCO50P
	sprintf(temp,"%s/chktape.sco",CURRENTDIRECTORY);
#endif
#ifdef UNIXWARE21
	sprintf(temp,"%s/chktape.unw",CURRENTDIRECTORY);
#endif
#ifdef UNIXWARE7
	sprintf(temp,"%s/chktape.unw",CURRENTDIRECTORY);
#endif
#ifdef SUNX86
	sprintf(temp,"%s/chktape.sun",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(temp,"%s\\chktape.exe",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"echo \"You must specify a tape device (/dev/st0):\" \n");
	fprintf(fp,"read TAPEDEVICE\n");
	fprintf(fp,"echo \"\"\n");
	fprintf(fp,"echo \"You must specify the archive type to list the tape contents!!!\" \n");
	fprintf(fp,"echo \"Valid archive types are afio, tar, tgz, cpio, and compressed\" \n");
	fprintf(fp,"read TAPETYPE\n");
	fprintf(fp,"echo \"\"\n");
	fprintf(fp,"export TAPETYPE TAPEDEVICE\n");
	fprintf(fp,"case $TAPETYPE in\n");
	fprintf(fp,"	afio)	afio -tv $TAPEDEVICE ;;\n");
	fprintf(fp,"	tar)	tar tvf $TAPEDEVICE ;;\n");
	fprintf(fp,"	tbz)	tar tvf $TAPEDEVICE ;;\n");
	fprintf(fp,"	tgz)	tar tvzf $TAPEDEVICE ;;\n");
	fprintf(fp,"	cpio)	afio -tv $TAPEDEVICE ;;\n");
	fprintf(fp,"	gzip)	gzip -d <$TAPEDEVICE | afio -tv - ;;\n");
	fprintf(fp,"	bzip)	bzip2 -d <$TAPEDEVICE | afio -tv - ;;\n");
	fprintf(fp,"	compressed)	uncompress <$TAPEDEVICE | afio -tv - ;;\n");
	fprintf(fp,"	\"\")	export ERROR1=\"true\" ;;\n");
	fprintf(fp,"#	*)	export ERROR2=\"true\" ;;\n");
	fprintf(fp,"esac\n");
	fclose(fp);
#ifndef WIN32
		fd=chmod(temp,(mode_t)MODE_RWXRWX___);
		if(fd==(-1))
		{
			prterr("Error:  Couldn't set Mode for [%s].",(temp!=NULL ? temp:""));
		}
#endif
	if(temp!=NULL) Rfree(temp);
}
static void displaytape()
{
	FILE *fp=NULL;
	char *temp=NULL;
#ifndef WIN32
	int fd=0;
#endif

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("displaytape.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\displaytape.exe",CURRENTDIRECTORY);
#endif
#ifdef LINUX
	sprintf(temp,"%s/displaytape.lnx",CURRENTDIRECTORY);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(temp,"%s/displaytape.lnx",CURRENTDIRECTORY);
#endif
#ifdef RS6000
	sprintf(temp,"%s/displaytape.rsc",CURRENTDIRECTORY);
#endif
#ifdef SCO50
	sprintf(temp,"%s/displaytape.sco",CURRENTDIRECTORY);
#endif
#ifdef SCO50P
	sprintf(temp,"%s/displaytape.sco",CURRENTDIRECTORY);
#endif
#ifdef UNIXWARE21
	sprintf(temp,"%s/displaytape.unw",CURRENTDIRECTORY);
#endif
#ifdef UNIXWARE7
	sprintf(temp,"%s/displaytape.unw",CURRENTDIRECTORY);
#endif
#ifdef SUNX86
	sprintf(temp,"%s/displaytape.sun",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	if(fp!=NULL)
	{
#ifdef LINUX
		fprintf(fp,"/usr/X11R6/bin/xterm -T \"DOFTPZIP\" -fn 10x24 -display $DISPLAY -e chktape.lnx -o %s",(stemp!=NULL ? stemp:"")); 
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		fprintf(fp,"/usr/X11R6/bin/xterm -T \"DOFTPZIP\" -fn 10x24 -display $DISPLAY -e chktape.lnx -o %s",(stemp!=NULL ? stemp:"")); 
#endif
#ifdef RS6000
		fprintf(fp,"/usr/bin/X11/aixterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d $DISPLAY -e chktape.rsc -o %s",(stemp!=NULL ? stemp:"")); 
#endif
#ifdef SCO50
		fprintf(fp,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d $DISPLAY -e chktape.sco -o %s",(stemp!=NULL ? stemp:"")); 
#endif
#ifdef SCO50P
		fprintf(fp,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d $DISPLAY -e chktape.sco -o %s",(stemp!=NULL ? stemp:"")); 
#endif
#ifdef UNIXWARE21
		fprintf(fp,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d $DISPLAY -e chktape.unw -o %s",(stemp!=NULL ? stemp:"")); 
#endif
#ifdef UNIXWARE7
		fprintf(fp,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d $DISPLAY -e chktape.unw -o %s",(stemp!=NULL ? stemp:"")); 
#endif
#ifdef SUNX86
		sprintf(fp,"/usr/X/bin/xterm -T \"DOFTPZIP\" -display $DISPLAY -e chktape.sun -o %s",(stemp!=NULL ? stemp:"")); 
#endif
		fclose(fp);
#ifndef WIN32
		fd=chmod(temp,(mode_t)MODE_RWXRWX___);
		if(fd==(-1))
		{
			prterr("Error:  Couldn't set Mode for [%s].",(temp!=NULL ? temp:""));
		}
#endif
	}
	if(temp!=NULL) Rfree(temp);
}
static void MNU_DISPLAY_TAPE()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("DISPLAY TAPE","displaytape.EXT");
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
void CHKTAPE()
{
	MNU_DISPLAY_TAPE();
	chktape();
	displaytape();
}
