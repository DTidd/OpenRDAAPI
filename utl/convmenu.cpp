/* convmenu.c - Converts Extensions in Menu Definitions */
#ifndef WIN32
#define __NAM__ "convmenu.lnx"
#endif
#ifdef WIN32
#define __NAM__ "convmenu.exe"
#endif
#include <app.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <olh.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
static char *module="GUI";
static APPlib *hlp_bins=NULL,*deflst=NULL,*holddefs=NULL;
static APPlib *dir_libs=NULL;
static void getdir(RDArsrc *),convmenu(RDArsrc *);
static void getdirectorymenu(RDArsrc *),setdirlist(RDArsrc *);
static void setextension(RDArsrc *),chkextension(RDArsrc *);
static void convquit(RDArsrc *);

static void convmenu(RDArsrc *parent)
{
	int selected=0,x,typex=0,y,length=0;
	char *libx=NULL,*tmp=NULL,*tmp2=NULL,*dirx=NULL,*e=NULL;
	APPlib *holdmenu=NULL;
	RDAmenu *menu=NULL;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"MENU LIBRARIES",&selected)) return;
	if(FINDRSCGETINT(parent,"TYPE LIST",&typex)) return;
	if(FINDRSCGETSTRING(parent,"DIRECTORY",&dirx)) return;
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[selected])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.MNU",dirx,hlp_bins->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.MNU",dirx,hlp_bins->libs[selected]);
#endif
	holdmenu=getunsortedlibnames(libx,FALSE); 
	if(holdmenu!=NULL)
	{
		e=Rmalloc(RDAstrlen(hlp_bins->libs[selected])+20);
		sprintf(e,"Converting Library %s",hlp_bins->libs[selected]);
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","GUI",e,NULL,holdmenu->numlibs);
		if(e!=NULL) Rfree(e); 
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
		}
		for(y=0;y<holdmenu->numlibs;++y)
		{
			menu=RDAmenuNEW(holdmenu->libs[y],NULL);
			getmenubin(libx,menu);
			if(menu->command!=NULL)
			{
				for(x=0;x<(RDAstrlen(menu->command)-3);++x)
				{
					if(!strncmp(&menu->command[x],".sco",4) || 
						!strncmp(&menu->command[x],".lnx",4) ||
						!strncmp(&menu->command[x],".rsc",4)
						|| !strncmp(&menu->command[x],".unw",4)
						|| !strncmp(&menu->command[x],".exe",4) 
						|| !strncmp(&menu->command[x],".EXE",4)
						|| !strncmp(&menu->command[x],".EXT",4))
					{
						length=RDAstrlen(menu->command)-3+
							RDAstrlen(holddefs->libs[typex]);
						if(tmp!=NULL) Rfree(tmp);
						tmp=Rmalloc(length);
						if(x<1)
						{
							sprintf(tmp,"%s%s",holddefs->libs[typex]
								,menu->command);
						} else if(x<(RDAstrlen(menu->command)-4))
						{
							sprintf(tmp,"%.*s%s%s",x,&menu->command[0],
								holddefs->libs[typex],&menu->command[x+4]);
						} else {
							if(tmp2!=NULL) Rfree(tmp2);
							tmp2=stralloc(menu->command);
							tmp2[RDAstrlen(tmp2)-4]=0;
							sprintf(tmp,"%s%s",tmp2,
								holddefs->libs[typex]);
						}
						if(menu->command!=NULL) Rfree(menu->command); 
						if(tmp!=NULL)
						{
							menu->command=stralloc(tmp);
							Rfree(tmp);
						}
					}
				}
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
			writemenubin(libx,menu);
			if(menu!=NULL) { free_menu(menu); menu=NULL; }
		}
		if(holdmenu!=NULL) freeapplib(holdmenu); 
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	if(libx!=NULL) Rfree(libx);
}
static void setextension(RDArsrc *parent)
{
	int selected=0;

	if(FINDRSCGETINT(parent,"TYPE LIST",&selected)) return;
	FINDRSCSETSTRING(parent,"EXTENSION",holddefs->libs[selected]);
	updatersrc(parent,"EXTENSION");
}
static void chkextension(RDArsrc *parent)
{
	char *tmp=NULL;
	int x;

	FINDRSCGETSTRING(parent,"EXTENSION",&tmp);
	if(tmp!=NULL)
	{
		for(x=0;x<holddefs->numlibs;++x)
		{
			if(!RDAstrcmp(tmp,holddefs->libs[x])) break;
		}
		if(x>=holddefs->numlibs)
		{
			addAPPlib(holddefs,tmp);
			addAPPlib(deflst,tmp);
		}
		FINDRSCSETINT(parent,"TYPE LIST",x);
		updatersrc(parent,"TYPE LIST");
	} else {
		FINDRSCGETINT(parent,"TYPE LIST",&x);
		FINDRSCSETSTRING(parent,"EXTENSION",holddefs->libs[x]);
		updatersrc(parent,"EXTENSION");
	}
}
static void convquit(RDArsrc *parent)
{
	if(parent!=NULL) free_rsrc(parent);
	if(hlp_bins!=NULL) freeapplib(hlp_bins);
	if(deflst!=NULL) freeapplib(deflst);
	if(holddefs!=NULL) freeapplib(holddefs);
}
static void getdirectorymenu(RDArsrc *parent)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL,*directory=NULL;

	if(FINDRSCGETSTRING(parent,"DIRECTORY",&directory)) return;
	FINDRSCGETINT(parent,"MENU LIBRARIES",&selectedm);
	if(hlp_bins!=NULL) freeapplib(hlp_bins); 
	hlp_bins=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(directory)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",directory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",directory);
#endif
	for(x=0;findfile(dirx,"*.MNU",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(hlp_bins,tmp);
	}
	if(hlp_bins->numlibs<1)
	{
		addAPPlib(hlp_bins,"Directory Contains No Menu Entries");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(hlp_bins);
	if(selectedm>=hlp_bins->numlibs) selectedm=0;
	if(!FINDRSCLISTAPPlib(parent,"MENU LIBRARIES",selectedm,hlp_bins))
		updatersrc(parent,"MENU LIBRARIES");
}	
static void setdirlist(RDArsrc *parent)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(parent,"DIRECTORY");
	if(FINDRSCGETSTRING(parent,"DIRECTORY",&directory)) return;
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
	FINDRSCLISTAPPlib(parent,"DIRECTORY LIST",x,dir_libs);
		updatersrc(parent,"DIRECTORY LIST");
}
static void getdir(RDArsrc *parent)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(parent,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(parent,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s]))
	{
		if(FINDRSCSETSTRING(parent,"DIRECTORY",dir_libs->libs[s])) return;
		updatersrc(parent,"DIRECTORY");
	}
	getdirectorymenu(parent);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	char *defdir=NULL;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"CONVERT MENU")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"CONVERT MENU");
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
	addlstrsrc(mainrsrc,"DIRECTORY LIST",0,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[0],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",0,dir_libs);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist,NULL);
	hlp_bins=APPlibNEW();
	addlstrsrc(mainrsrc,"MENU LIBRARIES",&x,TRUE,NULL,
		hlp_bins->numlibs,&hlp_bins->libs,NULL);
	getdirectorymenu(mainrsrc);
	deflst=APPlibNEW();
	addAPPlib(deflst,"EXT (Generic) Extensions");
	addAPPlib(deflst,"AIX Extensions");
	addAPPlib(deflst,"SCO Extensions");
	addAPPlib(deflst,"UNW Extensions");
	addAPPlib(deflst,"LNX Extensions");
	addAPPlib(deflst,"EXE Extensions");
	holddefs=APPlibNEW();
	addAPPlib(holddefs,".EXT");
	addAPPlib(holddefs,".rsc");
	addAPPlib(holddefs,".sco");
	addAPPlib(holddefs,".unw");
	addAPPlib(holddefs,".lnx");
	addAPPlib(holddefs,".EXE");
	addlstrsrc(mainrsrc,"TYPE LIST",&x,TRUE,setextension,deflst->numlibs,
		&deflst->libs,NULL);
	addstdrsrc(mainrsrc,"EXTENSION",VARIABLETEXT,0,holddefs->libs[0],TRUE);
	FINDRSCSETFUNC(mainrsrc,"EXTENSION",chkextension,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,convmenu,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,convquit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,convquit,NULL,TRUE);
}
