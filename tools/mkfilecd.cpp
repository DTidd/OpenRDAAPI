/* makefilecode.c - Generate File C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkfilecd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkfilecd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <packlib.hpp>
/*ADDLIB tool */
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


APPlib *scr_libs=NULL,*dir_libs=NULL;
static void getscreenlist(RDArsrc *);
char *module=NULL,use_cr=FALSE,use_chmod=FALSE;
RDArsrc *mrsrc=NULL;

static void makefilecode(FILE *file,FILE *fp,char *dirname,char *scrnname)
{
	char *temp=NULL,*temp1=NULL,*temp2=NULL;
	char *dirx1=NULL,*dirx2=NULL;
	int len=0,count=0,x=0;

	if(fp!=NULL)
	{	
		dirx1=Rmalloc(RDAstrlen(dirname)+15);
		dirx2=Rmalloc(RDAstrlen(dirname)+15);
		x=RDAstrlen(CURRENTDIRECTORY);
		temp=dirname+(x+1);
		if(!isEMPTY(temp))
		{
			sprintf(dirx1,"\\\\%s\\\\",temp);
			sprintf(dirx2,"/%s/",temp);
		} else {
			sprintf(dirx1,"\\\\");
			sprintf(dirx2,"/");
		}
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"\n");

		fprintf(fp,"#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)\n");
		fprintf(fp,"#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)\n");
		fprintf(fp,"#endif /* ifndef LINUX */\n");
		fprintf(fp,"#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)\n");
		fprintf(fp,"#define MODE_RWXRWX___ (00770)\n");
		fprintf(fp,"#endif /* ifdef LINUX */\n");

		fprintf(fp,"\n");
		temp=fixname(scrnname);
		fprintf(fp,"static void SCRIPT_%s(void);\n",temp);
		fprintf(fp,"\n");
		fprintf(fp,"static void SCRIPT_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tFILE *fp=NULL;\n");
		fprintf(fp,"\tchar *temp=NULL;\n");
		fprintf(fp,"\n");
		fprintf(fp,"\ttemp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(\"%s\")+15);\n",scrnname);
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\tsprintf(temp,\"%s%s%s\",CURRENTDIRECTORY);\n","%s",dirx1,scrnname);
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\tsprintf(temp,\"%s%s%s\",CURRENTDIRECTORY);\n","%s",dirx2,scrnname);
		fprintf(fp,"#endif\n");
		
		fprintf(fp,"\tfp=fopen(temp,\"w+b\");\n");
		fprintf(fp,"\n");
		if(dirx1!=NULL) Rfree(dirx1);
		if(dirx2!=NULL) Rfree(dirx2);
		temp=Rmalloc(1024+1);
		temp1=Rmalloc(1024+50);
		while((len=readline(file,temp,1024))!=(-1)) 
		{
			if(!isEMPTY(temp))
			{
			memset(temp1,0,1074);
			count=0;
			temp2=temp;
			for(;*temp2;++temp2) 
			{
				if(*temp2!='\"')
				{
					temp1[count]=*temp2;	
					++count;
				} else {
					temp1[count]='\\';	
					temp1[count+1]='\"';	
					count+=2;
				}
			}
			if(use_cr)
			{
				fprintf(fp,"\tfprintf(fp,\"%s\\r\\n\");\r\n",temp1);
			} else {
				fprintf(fp,"\tfprintf(fp,\"%s\\n\");\n",temp1);
			}
			}
		}
		if(temp!=NULL) Rfree(temp);
		if(temp1!=NULL) Rfree(temp1);
		fseek(file,0L,SEEK_SET);
		fprintf(fp,"\tfclose(fp);\n");
		if(use_chmod)
		{
			fprintf(fp,"\tchmod(temp,(mode_t)MODE_RWXRWX___);\n");
		}
		fprintf(fp,"\tif(temp!=NULL) Rfree(temp);\n");
		fprintf(fp,"}\n");
	}
}
static void quit_printRDAscrncode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDAscrncode Quitting Print RDA File Code");
        }
        free_rsrc(prsrc);
}
static void printRDAscrncode(RDArsrc *prsrc,FILE *scrn)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*scrnname=NULL;
	int l=0;

	if(scrn!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
			if(FINDRSCGETSTRING(mrsrc,"DIRECTORY",&dirname))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"FILE LIST",&l))
			{
				return;
			}
#ifndef WIN32
			fp=popen(outdevice,"w");
#endif
#ifdef WIN32
			fp=fopen(outdevice,"a+");
#endif
                	if(fp!=NULL)
                	{
				if(outdevice!=NULL) Rfree(outdevice);
				scrnname=stralloc(scr_libs->libs[l]);
				makefilecode(scrn,fp,dirname,scrnname);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDAscrn popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
				if(dirname!=NULL) dirname=NULL;
				if(outdevice!=NULL) Rfree(outdevice);
			}
		} else {
			ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
	} else {
		ERRORDIALOG("FILE ERROR!","The attempt to retrieve the screen definition failed.",NULL,FALSE);
		prterr("FILE RETRIVAL ERROR: The attempt to retrieve the screen definition failed.",NULL,FALSE);
	}
	if(dirname!=NULL) dirname=NULL;
	if(scrnname!=NULL) scrnname=NULL;
}
static void GetDevicePrintRDAscrncode(FILE *scrn)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAscrncode,scrn);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAscrncode,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make File function failed for the custom screen PACKAGE BINARY. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
static FILE *okfile(char *dirname,char *scrnname)
{
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(dirname)+RDAstrlen(scrnname)+6);
	sprintf(libx,"%s/%s",dirname,scrnname);
	return(fopen(libx,"r"));
}
void okfilecb(RDArsrc *mainrsrc)
{
	int l=0;
	char *dirname=NULL;
	FILE *fp=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&l))
	{
		l=0;
	}
	readwidget(mainrsrc,"USE CARRIAGE RETURN");
	FINDRSCGETCHAR(mainrsrc,"USE CARRIAGE RETURN",&use_cr);
	readwidget(mainrsrc,"USE CHMOD");
	FINDRSCGETCHAR(mainrsrc,"USE CHMOD",&use_chmod);
	fp=okfile(dirname,scr_libs->libs[l]);
	if(fp!=NULL)
	{
		GetDevicePrintRDAscrncode(fp);
	}
	if(dirname!=NULL) Rfree(dirname);
}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int l=0,x=0;
	char *dirname=NULL;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&l))
	{
		l=0;
	}
	if(scr_libs!=NULL) freeapplib(scr_libs);
	scr_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(dirname)+2);
	sprintf(dirx,"%s/",dirname);
	for(x=0;findfile(dirx,"*",&tmp,(int)x+1);++x)
	{
		addAPPlib(scr_libs,tmp);
	}
	if(scr_libs->numlibs<1)
	{
		addAPPlib(scr_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(scr_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(l>=scr_libs->numlibs) l=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"FILE LIST",l,scr_libs))
	{
		updatersrc(mainrsrc,"FILE LIST");
	}
	if(dirname!=NULL) Rfree(dirname);
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory))
		{
			inlist=TRUE;
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
	updatersrc(mainrsrc,"DIRECTORY LIST");
	getscreenlist(mainrsrc);
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s]))
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s])) return;
		updatersrc(mainrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getscreenlist(mainrsrc);
}
void quitscrncode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(scr_libs!=NULL) freeapplib(scr_libs);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	ShutdownSubsystems();
	exit(0);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int d=0,l=0;
	char *dirname=NULL,*libname=NULL;
	char *defdir=NULL;
	FILE *file;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,"TOOLS","MAKE FILE CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		dirname=stralloc(argv[1]);
		libname=stralloc(argv[2]);
		file=okfile(dirname,libname);
		if(file!=NULL)
		{
			makefilecode(file,stdout,dirname,libname);
			fclose(file);
		}
		if(dirname!=NULL) Rfree(dirname);
		if(libname!=NULL) Rfree(libname);
		ShutdownSubsystems();
		exit(0);
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE FILE CODE"))
		{
			ShutdownSubsystems();
			exit(1);
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE FILE CODE");
		addstdrsrc(mrsrc,"USE CARRIAGE RETURN",BOOLNS,1,&use_cr,TRUE);
		addstdrsrc(mrsrc,"USE CHMOD",BOOLNS,1,&use_chmod,TRUE);
		scr_libs=APPlibNEW();
		dir_libs=APPlibNEW();
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
		sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
		sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
		sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+9);
#ifndef WIN32
		sprintf(defdir,"%s/html",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\html",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+9);
#ifndef WIN32
		sprintf(defdir,"%s/cgi-bin",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\cgi-bin",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+9);
#ifndef WIN32
		sprintf(defdir,"%s/pixmaps",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\pixmaps",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+9);
#ifndef WIN32
		sprintf(defdir,"%s/scripts",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\scripts",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+9);
#ifndef WIN32
		sprintf(defdir,"%s",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		if(argc>1)
		{
			for(d=0;d<dir_libs->numlibs;++d)
			{
				if(!RDAstrcmp(argv[1],dir_libs->libs[d])) break;
			}
			if(d>=dir_libs->numlibs)
			{
				d=0;
			}
		}
		addlstrsrc(mrsrc,"DIRECTORY LIST",&d,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
		addstdrsrc(mrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[0],TRUE);
		FINDRSCLISTAPPlib(mrsrc,"DIRECTORY LIST",d,dir_libs);
		FINDRSCSETFUNC(mrsrc,"DIRECTORY",setdirlist,NULL);
		addlstrsrc(mrsrc,"FILE LIST",&l,TRUE,NULL,scr_libs->numlibs,&scr_libs->libs,NULL);
		getdir(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"FILE LIST",l,scr_libs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okfilecb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitscrncode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"DIRECTORY LIST");
		APPmakescrn(mrsrc,TRUE,quitscrncode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
