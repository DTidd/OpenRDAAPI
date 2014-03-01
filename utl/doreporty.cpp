/* doreport.c - Do Report */
#ifndef WIN32
#define __NAM__ "doreporty.lnx"
#endif
#ifdef WIN32
#define __NAM__ "doreporty.exe"
#endif
#include <app.hpp>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void setvalues(RDArsrc *);
static APPlib *rptavl=NULL,*modlst=NULL;
char *module="RPTGEN";

void getmodulelist(RDArsrc *mainrsrc)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
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
void getrptentries(RDArsrc *mainrsrc)
{
	int x,selected=0;
	char *tmp=NULL,*dirx=NULL,*temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+3);
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modlst->libs[selected]);
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
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"REPORTS AVAILABLE",0,rptavl))
			updatersrc(mainrsrc,"REPORTS AVAILABLE");
	}
	setvalues(mainrsrc);
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
void setvalues(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(rptavl->libs[selected],"No Report Definitions Available"))
		return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	temp=getreporttitle(modlst->libs[selectedm],rptavl->libs[selected]);
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE",temp))
		updatersrc(mainrsrc,"TITLE");
	if(temp!=NULL) Rfree(temp);
	temp=getreportdesc(modlst->libs[selectedm],rptavl->libs[selected]);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
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
int main(int argc,char **argv)
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],argv[2])) 
		{
			RDAAPPMAINLOOP();
			return;
		}
		RUNREPORT(argv[1],argv[2],NULL,NULL,FALSE,TRUE,NULL);
		ShutdownSubsystems();
		std::exit;
	} else {
		if(InitializeSubsystems(argc,argv,module,"DO REPORT")) 
		{
			RDAAPPMAINLOOP();
			return;
		}
		getmodulelist(mainrsrc);
		mainrsrc=RDArsrcNEW(module,"DO REPORTS");
		x=0;
		if(argc>1)
		{
			for(x=0;x<modlst->numlibs;++x)
			{
				if(!RDAstrcmp(argv[1],modlst->libs[x])) break;
			}
			if(x>=modlst->numlibs) x=0;
		}
		addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,getrptentries,
			modlst->numlibs,&modlst->libs,NULL);
		x=0;
		if(rptavl!=NULL) freeapplib(rptavl);
		rptavl=APPlibNEW();
		addlstrsrc(mainrsrc,"REPORTS AVAILABLE",&x,TRUE,setvalues,
			rptavl->numlibs,&rptavl->libs,NULL);
		addstdrsrc(mainrsrc,"TITLE",VARIABLETEXT,0,NULL,FALSE);
		addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
		getrptentries(mainrsrc);
		setvalues(mainrsrc);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,doreport,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdorpt,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitdorpt,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
