/*MakeNRDlist - create a list from a NRD file*/
#include <cstdio>
#include <mix.hpp>

APPlib *xMakeNRDlist(short filenum,char *module,char *desc,void (*func)(short,APPlib *),short keyno,void (*SubFunc)(...),void *args,int line,char *file)
{
	APPlib *list=NULL;
	short ef=0;
#ifdef DIAGNOSTICS
	RDArsrc *tmprsrc=NULL;
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG MakeNRDlist at line [%d] program [%s].",line,file);
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
		func(filenum,list);
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
int FINDFLDLISTELEMENT(short filenum,char *fieldname,APPlib *list,char *fieldvalue)
{
	int x=(-1);
	char *warndesc=NULL,*filename=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG FINDFLDLISTELEMENT checking file field value.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(list==NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("DIAG Error FINDFLDLISTELEMENT passed a NULL list.");
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	} else if(list->numlibs<1)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("DIAG Error FINDFLDLISTELEMENT passed an empty list.");
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	} else {
	
		if((x=FINDAPPLIBELEMENT(list,fieldvalue))==(-1))
		{
			filename=FILENAME(filenum);
			warndesc=Rmalloc(RDAstrlen(fieldvalue)+RDAstrlen(fieldname)+RDAstrlen(filename)+RDAstrlen(filename)+315);
			sprintf(warndesc,"The value of [%s] is an Invalid Field Value for the field [%s].  It doesn't exist in a non-deleted form in the supporting file [%s].  If this should be a valid value then it must be added to the supporting file [%s].  The List for this field is being set to the value [%s].",(fieldvalue==NULL?"":fieldvalue),fieldname,filename,filename,list->libs[0]);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FIELD VALUE IN RECORD!",warndesc,NULL,NULL,FALSE,NULL);
			Rfree(warndesc);
		}
	}
	return(x);
}
