/* doie.c - Do Import/Export */
#ifndef WIN32
#define __NAM__ "doie.lnx"
#endif
#ifdef WIN32
#define __NAM__ "doie.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <impexp.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void setvalues(RDArsrc *);
static APPlib *defavl=NULL,*modlst=NULL;
char *module="DATABASE";

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
	for(x=0;findfile(dirx,"*.IMP",&tmp,(int)x+1);++x) 
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
	int selected=0,seldef=0;
	char *dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&seldef)) return;
	if(defavl!=NULL) freeapplib(defavl);
	defavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+11);
#ifndef WIN32
	sprintf(dirx,"%s/rda/%s.IMP",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\%s.IMP",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
	defavl=getlibnames(dirx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(defavl==NULL) defavl=APPlibNEW();
	if(defavl->numlibs<1)
	{
		addAPPlib(defavl,"No Definitions Available");
	} else SORTAPPlib(defavl);
	if(mainrsrc!=NULL)
	{
		if(seldef>=defavl->numlibs) seldef=0;
		if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",
			seldef,defavl))
		{
			updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
		}
	}
	setvalues(mainrsrc);
}
void doreport(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(defavl->numlibs==1 && !RDAstrcmp(defavl->libs[0],"No Definitions Available")) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlst->libs[selectedm]);
	addAPPlib(args,defavl->libs[selected]);
	ExecuteProgram("doie",args);
	if(args!=NULL) freeapplib(args);
}
void setvalues(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	if(RDAstrcmp(defavl->libs[selected],"No Virtual Field Definitions"))
	{
		temp=getIMPORTdesc(modlst->libs[selectedm],defavl->libs[selected]);
	} else temp=NULL;
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
}
void quitdorpt(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	r=NULL;
	if(defavl!=NULL) freeapplib(defavl);
	defavl=NULL;
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
	RDArsrc *mainrsrc=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],argv[2])) 
		{
			return;
		}
		EXECUTEimport(argv[1],argv[2]);
		ShutdownSubsystems();
		std::exit;
	} else {
		if(InitializeSubsystems(argc,argv,module,"DO IMPORT/EXPORT")) 
		{
			return;
		}
		getmodulelist(mainrsrc);
		mainrsrc=RDArsrcNEW(module,"DO IMPORT/EXPORT");
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
		if(defavl!=NULL) freeapplib(defavl);
		defavl=APPlibNEW();
		addlstrsrc(mainrsrc,"DEFINITIONS AVAILABLE",&x,TRUE,setvalues,
			defavl->numlibs,&defavl->libs,NULL);
		addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
		getrptentries(mainrsrc);
		setvalues(mainrsrc);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,doreport,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdorpt,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitdorpt,NULL,TRUE);
	}
}
