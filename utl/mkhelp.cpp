/* cpscnhlp.c - Copy Screen Help */
#ifndef WIN32
#define __NAM__ "mkhelp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkhelp.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="OLHELP";
APPlib *helptypes=NULL;

static void select_pattern(APPlib *modsavl)
{
	int y,z;
	char *libx=NULL,*e=NULL,*dira=NULL;
	RDArsrc *tmprsrc=NULL;
	APPlib *scrlist=NULL;
	short changedit=FALSE;
	RDAhelp *tmpdata=NULL;

	for(z=0;z<modsavl->numlibs;++z)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
			RDAstrlen(modsavl->libs[z])+10);
		sprintf(libx,"%s/rda/%s.SCN",CURRENTDIRECTORY,
			modsavl->libs[z]);
		dira=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl->libs[z])+10);
		sprintf(dira,"%s/rda/%s.HLP",CURRENTDIRECTORY,modsavl->libs[z]);
		scrlist=getunsortedlibnames(libx,FALSE);
		if(scrlist!=NULL)
		{
			e=Rmalloc(RDAstrlen(modsavl->libs[z])+40);
			sprintf(e,"Converting Module %s, %d of %d",
				modsavl->libs[z],z,modsavl->numlibs);
			tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,e,NULL,
				scrlist->numlibs);
			if(e!=NULL) Rfree(e);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE))
					ForceWindowUpdate(tmprsrc);
			}
			for(y=0;y<scrlist->numlibs;++y)
			{
				changedit=FALSE;
				tmpdata=RDAhelpNEW();
				if(gethelpbin(dira,scrlist->libs[y],tmpdata,FALSE))
				{
					if(tmpdata==NULL) tmpdata=RDAhelpNEW();
					if(tmpdata->name==NULL) tmpdata->name=stralloc(scrlist->libs[y]);
					if(tmpdata->desc!=NULL) Rfree(tmpdata->desc);
					writehelpbin(dira,tmpdata);
					changedit=TRUE;
				}
				if(tmpdata!=NULL) free_help(tmpdata);
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,
							(changedit ? TRUE:FALSE));
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			freeapplib(scrlist);
		}
		if(libx!=NULL) Rfree(libx);
		if(dira!=NULL) Rfree(dira);
	}
}
static void selectcp()
{
	int x=0;
	char *dirx=NULL,*tmp=NULL;
	APPlib *modsavl=NULL;

	modsavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modsavl,tmp);
	}
	if(modsavl->numlibs<1)
	{
		addAPPlib(modsavl,"Directory Contains No Libraries");
	} else if(modsavl->numlibs>2)
	{
		SORTAPPlib(modsavl);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	select_pattern(modsavl);
	if(modsavl!=NULL) freeapplib(modsavl);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"COPY SCREEN HELP")) 
	{
		return;
	}
	selectcp();
	ShutdownSubsystems();
	std::exit;
}
