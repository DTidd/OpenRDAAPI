/*MakeNRDlistAllSTRING - create a list from a NRD file, with field name and all
records*/
#include <cstdio>
#include <mix.hpp>

APPlib *xMakeNRDlistAllSTRING(short filenum,char *module,char *desc,short keyno,char *fieldname,void (*SubFunc)(...),void *args,int line,char *file)
{
	APPlib *list=NULL;
	short ef=0;
	char *temp=NULL;
	char deleteflag=FALSE;
#ifdef DIAGNOSTICS
	RDArsrc *tmprsrc=NULL;
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG MakeNRDlistAllSTRING at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	list=APPlibNEW();
	if(list==NULL) return(list);
#ifdef DIAGNOSTICS
	tmprsrc=diagscrn(filenum,"DIAGNOSTIC SCREEN",module,desc,NULL);
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,FALSE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
			prterr("Error Cannot Create Diagnostic Screen.");
		} else ForceWindowUpdate(tmprsrc);
	}
#endif
	if(SubFunc!=NULL) ef=ADVFRSNRDsec(filenum,keyno,SubFunc,args);
		else ef=FRSNRDsec(filenum,keyno);
	while(!ef)
	{
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(filenum,fieldname,&temp);
			addAPPlib(list,temp);
			if(temp!=NULL) { Rfree(temp); temp=NULL; }
		}
#ifdef DIAGNOSTICS
		update_diagnostic(tmprsrc,TRUE);
#endif
		if(SubFunc!=NULL) ef=ADVNXTNRDsec(filenum,keyno,SubFunc,args);
			else ef=NXTNRDsec(filenum,keyno);
	}
#ifdef DIAGNOSTICS
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	ForceWindowUpdate(mtnrsrc);
#endif
	return(list);
}

