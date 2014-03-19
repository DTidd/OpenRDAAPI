/* doreport.c - Do Report */
#ifndef WIN32
#define __NAM__ "doreport.lnx"
#endif
#ifdef WIN32
#define __NAM__ "doreport.exe"
#endif
#include <app.hpp>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB add */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static APPlib *rptavl=NULL,*modlst=NULL;
static void setvaluescb(RDArsrc *);
static void setvalues(RDArsrc *,char *,char *);
static void getrptentriescb(RDArsrc *);
static void getrptentries(RDArsrc *,char *);
char *module="RPTGEN";
static char allow_all=FALSE;

void getmodulelist(RDArsrc *mainrsrc)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlst);
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",0,modlst))
		{
			updatersrc(mainrsrc,"MODULE LIST");
		}
	}
}
void getrptentries(RDArsrc *mainrsrc,char *modname)
{
	int x=0;
	char *tmp=NULL,*dirx=NULL,*temp=NULL;

	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,modname);
#endif
	for(x=0;findfile(dirx,"*.RPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		temp=stripdashes(tmp);
		addAPPlib(rptavl,temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	}
	if(rptavl->numlibs<1)
	{
		addAPPlib(rptavl,"No Report Definitions Available");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(rptavl);
}
void getrptentriescb(RDArsrc *mainrsrc)
{
	int selected=0,selr=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selr)) return;
	getrptentries(mainrsrc,modlst->libs[selected]);
	if(selr>=rptavl->numlibs) selr=0;
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"REPORTS AVAILABLE",selr,rptavl))
			updatersrc(mainrsrc,"REPORTS AVAILABLE");
		setvaluescb(mainrsrc);
	}
}
void doreport(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(rptavl->numlibs==1 && !RDAstrcmp(rptavl->libs[0],"No Report Definitions Available")) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlst->libs[selectedm]);
	addAPPlib(args,rptavl->libs[selected]);
	ExecuteProgram("doreport",args);
	if(args!=NULL) freeapplib(args);
}
void setvalues(RDArsrc *mainrsrc,char *modname,char *rptname)
{
	RDAreport *report=NULL;
	RDAdisplay *display=NULL;
	char *temp=NULL;
	
	if(!RDAstrcmp(rptname,"No Report Definitions Available"))
	{
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
		if(!FINDRSCSETSTRING(mainrsrc,"TITLE",temp))
			updatersrc(mainrsrc,"TITLE");
		if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
			updatersrc(mainrsrc,"DESCRIPTION");
		return;
	}
	report=GETRDAreport(modname,rptname);
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE",report->title))
		updatersrc(mainrsrc,"TITLE");
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",report->desc))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(allow_all==FALSE)
	{
		if(report!=NULL)
		{
			if(report->display!=NULL)
			{
				display=report->display;
				if(display->device>2)
				{
					FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
					FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
				} else {
					FINDRSCSETSENSITIVE(mainrsrc,"SELECT",TRUE);
					FINDRSCSETEDITABLE(mainrsrc,"SELECT",TRUE);
				}
			} else {
				FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
				FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
			}
		} else {
			FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
			FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
		}
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT",TRUE);
	}
}
void setvaluescb(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	setvalues(mainrsrc,modlst->libs[selectedm],rptavl->libs[selected]);
}
void quitdorpt(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	r=NULL;
	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=NULL;
	if(modlst!=NULL) freeapplib(modlst);
	modlst=NULL;
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	int y=0,arg2use=1;
	RDArsrc *mainrsrc=NULL;

	if(argc>2)
	{
		if(argc==4 && !strcmp(argv[1],"-a"))
		{
			if(InitializeSubsystems(argc,argv,argv[2],argv[3])) 
			{
       		      	        return(1);
			}
	diagmyisam=1;
			RUNREPORT(argv[2],argv[3],NULL,NULL,FALSE,TRUE,NULL);
			return(0);
		} else if(argc==3 && strcmp(argv[1],"-a"))
		{
			if(InitializeSubsystems(argc,argv,argv[1],argv[2])) 
			{
       			        return(1);
			}
	diagmyisam=1;
			RUNREPORT(argv[1],argv[2],NULL,NULL,FALSE,TRUE,NULL);
			return(0);
		}
	}
	if(InitializeSubsystems(argc,argv,module,"DO REPORT")) 
	{
        	return(1);
	}
	getmodulelist(mainrsrc);
	mainrsrc=RDArsrcNEW(module,"DO REPORTS");
	x=0;
	if(!RDAstrcmp(argv[1],"-a"))
	{
		allow_all=TRUE;
		arg2use=2;
	} else {
		arg2use=1;
	}
	for(y=0;y<modlst->numlibs;++y)
	{
		if(!RDAstrcmp(argv[arg2use],modlst->libs[y])) break;
	}
	if(y>=modlst->numlibs)
	{
		y=0;
	}
	addlstrsrc(mainrsrc,"MODULE LIST",&y,TRUE,getrptentriescb,
		modlst->numlibs,&modlst->libs,NULL);
	x=0;
	addstdrsrc(mainrsrc,"TITLE",VARIABLETEXT,0,NULL,FALSE);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	getrptentries(mainrsrc,modlst->libs[y]);
	if((arg2use+1)<=argc)
	{
		for(x=0;x<rptavl->numlibs;++x)
		{
			if(!RDAstrcmp(argv[arg2use+1],rptavl->libs[x])) break;
		}
		if(x>=rptavl->numlibs)
		{
			x=0;
		}
	}
	addlstrsrc(mainrsrc,"REPORTS AVAILABLE",&x,TRUE,setvaluescb,
		rptavl->numlibs,&rptavl->libs,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,doreport,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdorpt,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitdorpt,NULL,TRUE);
	setvalues(mainrsrc,modlst->libs[y],rptavl->libs[x]);
	return(0);
}
