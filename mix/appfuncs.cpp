/* appfuncs.c - AppMgt Setup Variables */
#include<appmgt.hpp>

short xgetappmgtbin(RDAappmgt *appmgt,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/APPMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\APPMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG getappmgtbin Reading AppMgt Setup Variables from [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,"APPMGT SETUP",TRUE);
	if(bin==NULL) 
	{
		prterr("Error AppMgt Setup Variables were not found in Library [%s] or the Library doesn't exist at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701))
	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read AppMgt Setup Variables at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	test=BINgetshort(bin);
	if(test) appmgt->use_personnel_interview=TRUE;
		else appmgt->use_personnel_interview=FALSE;
	test=BINgetshort(bin);
	if(test) appmgt->use_administrative_interview=TRUE;
		else appmgt->use_administrative_interview=FALSE;
	BINfree(bin);
	if(libname!=NULL) Rfree(libname);
	return(0);
}
short xwriteappmgtbin(RDAappmgt *appmgt,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/APPMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\APPMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG writeappmgtbin Writing AppMgt Setup Variables to Library [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1701);	/*type identifier*/
	BINaddshort(bin,(appmgt->use_personnel_interview ? TRUE:FALSE));
	BINaddshort(bin,(appmgt->use_administrative_interview ? TRUE:FALSE));
	if(writelibbin(libname,bin,"APPMGT SETUP"))
	{
		prterr("Error Can't write AppMgt Setup Variables in binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
RDAappmgt *xRDAappmgtNEW(int line,char *file)
{
	RDAappmgt *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDAappmgtNEW Creating a structure for the Fincancial Management Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAappmgt));
	tmp->use_personnel_interview=FALSE;
	tmp->use_administrative_interview=FALSE;
	return(tmp);
}
void xfree_appmgt(RDAappmgt *appmgt,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG free_appmgt freeing memory used for the AppMgt Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(appmgt!=NULL) 
	{
		Rfree(appmgt);
	}
}
short xGetAppMgtSetup(RDAappmgt *appsetup,int line,
	char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GetAppMgtSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(getappmgtbin(appsetup)==(-1))
	{
		ERRORDIALOG("AppMgt Setup Not Available","The AppMgt Setup File cannot opened.\n",NULL,TRUE);
		return(-1);
	}
	return(0);
}
