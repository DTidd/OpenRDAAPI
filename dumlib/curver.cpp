#include <mix.hpp>

int xGetBinaryVersion(char *mod,int line,char *file)
{
	RDAGenericSetup *g=NULL;
	char *libname=NULL;
	int VN=(-1);

	g=RDAGenericSetupNew(mod,"BINARY VERSION NUMBER");
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(mod)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.GSV",CURRENTDIRECTORY,(mod!=NULL ? mod:""));
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,(mod!=NULL ? mod:""));
#endif
	if(!getRDAGenericSetupbin(libname,g))
	{
		VN=*g->value.integer_value;
	}
	if(g!=NULL) FreeRDAGenericSetup(g);
	if(libname!=NULL) Rfree(libname);
	return(VN);
}
short xWriteBinaryVersion(char *mod,int VN,int line,char *file)
{
	RDAGenericSetup *g=NULL;
	char *libname=NULL;

	g=RDAGenericSetupNew(mod,"BINARY VERSION NUMBER");
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(mod)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.GSV",CURRENTDIRECTORY,(mod!=NULL ? mod:""));
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,(mod!=NULL ? mod:""));
#endif
	if(getRDAGenericSetupbin(libname,g))
	{
		g->type=LONGV;
		g->value.integer_value=Rmalloc(sizeof(int));
		*g->value.integer_value=0;
		g->desc=stralloc("Binary Version Number of the last Make Applied completely with Select All.");
		g->label=stralloc("Binary Version Number:");
	}
	*g->value.integer_value=VN;
	if(writeRDAGenericSetupbin(libname,g))	
	{
		prterr("Error Couldn't create BINARY VERSION NUMBER.");
	}
	if(g!=NULL) FreeRDAGenericSetup(g);
	if(libname!=NULL) Rfree(libname);
	return(VN);
}
