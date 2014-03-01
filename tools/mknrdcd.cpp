/* mknrdcd.c - Generate Database C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mknrdcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mknrdcd.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <nrdp1.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


APPlib *db_libs=NULL,*db_defs=NULL;
static void getdbfilelist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void makedbfilecode(NRDbuffer *NRDtmp,FILE *fp,char *libname,char *filename)
{
	int x=0,y=0;
	NRDfield *f=NULL;
	NRDkey *k=NULL;
	NRDpart *p=NULL;	
	char *temp=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"/* %s's - Make (%s) Database File */\n",NRDtmp->modname,NRDtmp->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"#include <mknrd.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		fprintf(fp,"\n");
		temp=adddashes(NRDtmp->name);
		fprintf(fp,"void DB_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tDBsort *sort=NULL;\n");
		fprintf(fp,"\tshort x=(-1);\n");
		fprintf(fp,"\tchar same=FALSE;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
/*
		fprintf(fp,"\tchar *tempstr=NULL;\n");
*/
		fprintf(fp,"\n");

		fprintf(fp,"\tsort=DBsortNEW(\"%s\",\"%s\",0);\n",(libname!=NULL ? libname:""),(filename!=NULL?filename:""));
		fprintf(fp,"\tif(sort!=NULL)\n");
		fprintf(fp,"\t{\n");
		if(NRDtmp->field!=NULL)
		{
			for(x=0,f=NRDtmp->field;x<NRDtmp->nofields;++x,++f)
			{
				fprintf(fp,"\t\taddDBfield(sort,\"%s\",%d,%d);\n",(f->name!=NULL ? f->name:""),f->type,f->len);
			}
		}
		if(NRDtmp->key!=NULL)
		{
			for(x=0,k=NRDtmp->key;x<NRDtmp->nokeys;++x,++k)
			{
				fprintf(fp,"\t\taddDBkey(sort,\"%s\");\n",(k->name!=NULL ? k->name:""));
				if(k->part!=NULL)
				{
					for(y=0,p=k->part;y<k->numparts;++y,++p)
					{
						fprintf(fp,"\t\taddDBkeypart(sort,%d,\"%s\");\n",(x+1),(p->name!=NULL ? p->name:""));
					}
				}
			}
		}
		fprintf(fp,"\n");
		fprintf(fp,"\t\tsame=CompareDATABASE(sort);\n");
		fprintf(fp,"\t\tif(same==FALSE)\n");
		fprintf(fp,"\t\t{\n");
		fprintf(fp,"\t\t\tMergeDATABASE(sort);\n");
		fprintf(fp,"\t\t\tSetupFileDefinition(sort);\n");
		fprintf(fp,"\t\t\tx=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);\n");
		fprintf(fp,"\t\t\tif(x==(-1))\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\t\ttemp1=Rmalloc(%d+%d+77+1);\n",RDAstrlen(NRDtmp->name),RDAstrlen(NRDtmp->modname));
		fprintf(fp,"\t\t\t\tsprintf(temp1,\"DATABASE FILE WRITE ERROR: Module [%s] Screen [%s], Can Not Save Database File!\");\n",(NRDtmp->modname!=NULL?NRDtmp->modname:""),(NRDtmp->name!=NULL?NRDtmp->name:""));
		fprintf(fp,"\t\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t\t{\n");
		fprintf(fp,"\t\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t\t}\n");
		fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

/*
		fprintf(fp,"\t\t\t\ttempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));\n");
		fprintf(fp,"\t\t\t\tsprintf(tempstr,\"The Database File Changes may not be saved to the specified library: [%%s][%%s].  Check your permissions and re-try.  Call your installer for further instructions.\",sort->module,sort->filename);\n");
		fprintf(fp,"\t\t\t\tprterr(\"ERROR: Cannot Save Database File, [%%s]\",tempstr);\n");
		fprintf(fp,"\t\t\t\tERRORDIALOG(\"Cannot Save Database File\",tempstr,NULL,FALSE);\n");
		fprintf(fp,"\t\t\t\tif(tempstr!=NULL) Rfree(tempstr);\n");
*/

		fprintf(fp,"\t\t\t} else {\n");
		fprintf(fp,"\t\t\t\tCLSNRD(x);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(sort!=NULL) ADVFreeDBsort(sort,FALSE);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

	}

}
static void quit_printNRDbuffercode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printNRDbuffercode Quitting Print RDA Screen Code");
        }
        free_rsrc(prsrc);
}
static void printNRDbuffercode(RDArsrc *prsrc,NRDbuffer *NRDtmp)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*filename=NULL;
	int l=0,s=0;

	if(NRDtmp!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
			if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"DATABASE LIST",&s))
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
				libname=stralloc(db_libs->libs[l]);
				filename=stralloc(db_defs->libs[s]);
				makedbfilecode(NRDtmp,fp,libname,filename);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(NRDtmp!=NULL) Rfree(NRDtmp);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintNRDbuffer popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
				if(dirname!=NULL) dirname=NULL;
				if(outdevice!=NULL) Rfree(outdevice);
			}
		} else {
			ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
	} else {
		ERRORDIALOG("DATABASE FILE ERROR!","The attempt to retrieve the Database File definition failed.",NULL,FALSE);
		prterr("SCREEN RETRIVAL ERROR: The attempt to retrieve the Database File definition failed.",NULL,FALSE);
	}
	if(libname!=NULL) libname=NULL;
	if(filename!=NULL) filename=NULL;
}
static void GetDevicePrintNRDbuffercode(NRDbuffer *NRDtmp)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printNRDbuffercode,NRDtmp);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printNRDbuffercode,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PACKAGE BINARY. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
static NRDbuffer *okdbfile(char *libname,char *filename)
{
	NRDbuffer *NRDtmp=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(NRDtmp!=NULL) freenrd(NRDtmp); 
		NRDtmp=NRDtmpGETBIN(libname,filename);
		if(NRDtmp==NULL)
		{
			prterr("ERROR: Cannot Read Database File Definition.  Error in permissions or Database File Definition [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,filename);
			if(NRDtmp!=NULL) freenrd(NRDtmp);
			return(NULL);
		} else {
			return(NRDtmp);
		}
	} else { 
		return(NULL);
	}
}
void okdbfilecb(RDArsrc *mainrsrc)
{
	NRDbuffer *NRDtmp=NULL;
	int l=0,s=0;
	char *libname=NULL,*filename=NULL;

	if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
	{
		return;
	} else {
		libname=stralloc(db_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"DATABASE LIST",&s))
	{
		return;
	} else {
		filename=stralloc(db_defs->libs[s]);
	}
	NRDtmp=okdbfile(libname,filename);
	if(NRDtmp!=NULL)
	{
		GetDevicePrintNRDbuffercode(NRDtmp);
	}
	if(libname!=NULL) Rfree(libname);
	if(filename!=NULL) Rfree(filename);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"DATABASE LIST",&s);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	freeapplib(db_defs);
	if(!RDAstrcmp(db_libs->libs[l],"Directory Contains No Libraries"))
	{
		db_defs=APPlibNEW();
		addAPPlib(db_defs,"Contains No Screen Definitions");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(db_libs->libs[l])+6);
		sprintf(libx,"%s/%s.FIL",dirx,db_libs->libs[l]);
		db_defs=getlibnames(libx,FALSE);
		if(db_defs==NULL)
		{
			db_defs=APPlibNEW();
			addAPPlib(db_defs,"Contains No Screen Definitions");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=db_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"DATABASE LIST",s,db_defs))
	{
		updatersrc(mainrsrc,"DATABASE LIST");
	}
}
static void getdbfilelist(RDArsrc *mainrsrc)
{
	int l=0,x=0;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l))
	{
		l=0;
	}
	if(db_libs!=NULL) freeapplib(db_libs);
	db_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(db_libs,tmp);
	}
	if(db_libs->numlibs<1)
	{
		addAPPlib(db_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(db_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(l>=db_libs->numlibs) l=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LIBRARY LIST",l,db_libs))
	{
		updatersrc(mainrsrc,"LIBRARY LIST");
	}
	doliblist(mainrsrc);
}
void quitdbfilecode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(db_libs!=NULL) freeapplib(db_libs);
	if(db_defs!=NULL) freeapplib(db_defs);
	ShutdownSubsystems();
	exit(0);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int l=0,s=0;
	char *libname=NULL,*filename=NULL;
	NRDbuffer *NRDtmp=NULL;
	FILE *fp=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(0,NULL,"TOOLS","MAKE DATABASE CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		libname=stralloc(argv[1]);
		filename=stralloc(argv[2]);
		NRDtmp=okdbfile(libname,filename);
		if(NRDtmp!=NULL)
		{
			makedbfilecode(NRDtmp,stdout,libname,filename);
		}
		if(NRDtmp!=NULL) Rfree(NRDtmp);
		if(libname!=NULL) Rfree(libname);
		if(filename!=NULL) Rfree(filename);
		ShutdownSubsystems();
		exit(0);
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE DATABASE CODE"))
		{
			ShutdownSubsystems();
			exit(1);
/*
			RDAAPPMAINLOOP();
			return;
*/
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE DATABASE CODE");
		db_libs=APPlibNEW();
		db_defs=APPlibNEW();
		addlstrsrc(mrsrc,"LIBRARY LIST",&l,TRUE,doliblist,db_libs->numlibs,&db_libs->libs,NULL);
		addlstrsrc(mrsrc,"DATABASE LIST",&s,TRUE,NULL,db_defs->numlibs,&db_defs->libs,NULL);
		getdbfilelist(mrsrc);
		if(argc>1)
		{
			for(l=0;l<db_libs->numlibs;++l)
			{
				if(!RDAstrcmp(argv[1],db_libs->libs[l])) break;
			}
			if(l>=db_libs->numlibs)
			{
				l=0;
			}
		}
		FINDRSCLISTAPPlib(mrsrc,"LIBRARY LIST",l,db_libs);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"DATABASE LIST",s,db_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okdbfilecb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitdbfilecode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"LIBRARY LIST");
		APPmakescrn(mrsrc,TRUE,quitdbfilecode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
