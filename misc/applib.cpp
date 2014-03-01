/* applib.c - Applib Functions */
#include <misc.hpp>

void xfreeapplib(APPlib *applib,int line,char *file)
{
	int x;

	if(diagblb)
	{
		prterr("DIAG freeapplib Freeing APPlib structure at line [%d] file [%s].",line,file);
	}
	if(applib!=NULL)
	{
		if(applib->libs!=NULL)
		{
			for(x=0;x<applib->numlibs;++x) if(applib->libs[x]!=NULL) Rfree(applib->libs[x]);
			Rfree(applib->libs);
		}
		Rfree(applib);
		applib=NULL;
	}
}
APPlib *xAPPlibNEW(int line,char *file)
{
	APPlib *tmp=NULL;
	
	if(diagblb)
	{
		prterr("DIAG APPlibNEW creating new applib structure at line [%d] file [%s].",line,file);
	}
	if(diagmisc && diagallocx) tmp=(APPlib *)xRmalloc(sizeof(APPlib),line,file);
		else tmp=(APPlib *)Rmalloc(sizeof(APPlib));
	tmp->numlibs=0;
	tmp->libs=NULL;
	return(tmp);
}
void xSORTAPPlib(APPlib *a,int line,char *file)
{
	int x,y;
	char *hold;

	if(diagblb)
	{
		prterr("DIAG SORTAPPlib Sorting Contents in an APPlib Structure at line [%d] program [%s].",line,file);
	}
	for(x=0;x<a->numlibs-1;++x) for(y=x+1;y<a->numlibs;++y)
	{
		if(RDAstrcmp(a->libs[y],a->libs[x])<0)
		{
			hold=a->libs[y];
			a->libs[y]=a->libs[x];
			a->libs[x]=hold;
		}
	} 
}
int xaddAPPlib(APPlib *lib,char *name,int line,char *file)
{
	if(diagblb)
	{
		prterr("DIAG addAPPlib Adding Element [%s] to APPlib Structure at line [%d] program [%s].",name,line,file);
	} 
	if(diagallocx && diagmisc)
	{
		if(lib->libs==NULL)
		{
			lib->libs=(char **)xRmalloc(sizeof(char *),line,file);
		} else {
			lib->libs=(char **)xRrealloc(lib->libs,(lib->numlibs+1)*sizeof(char *),line,file);
		}
	} else {
		if(lib->libs==NULL)
		{
			lib->libs=(char **)Rmalloc(sizeof(char *));
			lib->numlibs=0;
		} else {
			lib->libs=(char **)Rrealloc(lib->libs,(lib->numlibs+1)*sizeof(char *));
		}
	}
	if(diagallocx && diagmisc)
	{
		lib->libs[lib->numlibs]=xstralloc(name,line,file);
	} else lib->libs[lib->numlibs]=stralloc(name);
	++lib->numlibs;
	return(lib->numlibs);
}
int xaddAPPlibNoDuplicates(APPlib *lib,char *name,int line,char *file)
{
	int x,return_value=(-1);

	if(diagblb)
	{
		prterr("DIAG addAPPlibNoDuplicates Adding [%s] to APPlib Structure ensuring No Duplicates at line [%d] program [%s].",name,line,file);
	} 
	for(x=0;x<lib->numlibs;++x)
	{
		if(!RDAstrcmp(lib->libs[x],name)) break;
	}
	if(x>=lib->numlibs)
	{
		if(diagallocx && diagmisc)
		{
			return(xaddAPPlib(lib,name,line,file));
		} else return(addAPPlib(lib,name));
	}	
	return(return_value);
}
void xSEEAPPlib(APPlib *a,int line,char *file)
{
	int x;

	if(a!=NULL)
	{
		prterr("DIAG SEEAPPlib Showing Contents of APPlib Structure, has [%d] elements at line [%d] program [%s].",a->numlibs,line,file);
		for(x=0;x<a->numlibs;++x)
		{
			prterr("     APPlib #%d [%s]\n",x,a->libs[x]);
		}
	} else {
		prterr("DIAG Error APPlib is NULL at line [%d] program [%s].",line,file);
	}
}
int xFINDAPPLIBELEMENT(APPlib *a,char *string,int line,char *file)
{
	int x=(-1);

	if(diagblb)
	{
		prterr("DIAG FINDAPPLIBELEMENT at line [%d] program [%s].",line,file);
	}
	if(a!=NULL)
	{
		for(x=0;x<a->numlibs;++x) if(!RDAstrcmp(a->libs[x],string)) return(x);
	} 
	return(-1);
}
APPlib *xCopyAPPlib(APPlib *a,int line,char *file)
{
	APPlib *b=NULL;
	int x;

	if(diagblb)
	{
		prterr("DIAG CopyAPPlib at line [%d] program [%s].",line,file);
	}
	if(a!=NULL)
	{
		if(a->numlibs>0)
		{
			b=APPlibNEW();
			for(x=0;x<a->numlibs;++x) addAPPlib(b,a->libs[x]);
		}
	}
	return(b);
}
