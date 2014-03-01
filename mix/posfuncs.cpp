#include <postrk.hpp>
#include <mix.hpp>

short xupdateftes(short fileno,char *posid,double amount,short type,
	void (*SubFunc)(...),void *args,int line,char *file)
{
	char /* *tmp=NULL, */ delflag=FALSE;
	char *modulename=NULL,*filename=NULL;
	double tmpamt=0.0;
	RDATData *previous=NULL;
	short ef=FALSE;
	
	modulename=MODULENAME(fileno);
	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG updateftes Updating FTE Amounts for the Position Identication of [%s] in Module [%s] Fileno [%d] [%s] for Amount [%.02f] at line [%d] program [%s].",posid,modulename,fileno,filename,amount,line,file); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(fileno);
	FINDFLDSETSTRING(fileno,"POSITION IDENTIFICATION",posid);
	LOCNRDFILE(fileno);
	if(SubFunc!=NULL) ef=ADVEQLNRDsec(fileno,1,SubFunc,args);
		else ef=EQLNRDsec(fileno,1);
	if(ef)
	{
		UNLNRDFILE(fileno);
/*
		if(tmp!=NULL) Rfree(tmp);
		tmp=Rmalloc(RDAstrlen(posid)+RDAstrlen(modulename)+RDAstrlen(filename)+211);
		sprintf(tmp,"The POSITION IDENTIFICATION [%s] that was entered is not a DEFINED POSITION IDENTIFICATION in the [%s][%s] data file.  Check to see if it is defined.  If not and it is needed it needs to be set up.",(posid!=NULL?posid:""),modulename,filename);
*/
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error updateftes has identifified an invalid Position Identification [%s] in the data file [%s][%s] at line [%d] program [%s].",posid,modulename,filename,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
	if(delflag==TRUE)
	{
		UNLNRDFILE(fileno);
/*
		if(tmp!=NULL) Rfree(tmp); 
		tmp=Rmalloc(RDAstrlen(posid)+100);
		sprintf(tmp,"The Position Identification [%s] that was entered has been marked for deletion.",(posid==NULL?"":posid););
*/
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error updateftes has identified an invalid Position Identification [%s] in the data file [%s][%s] at line [%d] program [%s].  It is marked for deletion at line [%d] program [%s].",posid,modulename,filename,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	} 
	previous=RDATDataNEW(fileno);
	FINDFLDGETDOUBLE(fileno,"TOTAL FTE AMOUNT",&tmpamt);
	tmpamt+=amount;
	if(tmpamt<0.0) tmpamt=0.0;
	FINDFLDSETDOUBLE(fileno,"TOTAL FTE AMOUNT",tmpamt);
	if(type==1)
	{
		FINDFLDGETDOUBLE(fileno,"FILLED FTE AMOUNT",&tmpamt);
		tmpamt+=amount;
		if(tmpamt<0.0) tmpamt=0.0;
		FINDFLDSETDOUBLE(fileno,"FILLED FTE AMOUNT",tmpamt);
	}
	if(SubFunc!=NULL) ef=ADVWRTTRANSsec(fileno,0,NULL,previous,SubFunc,args);
		else ef=WRTTRANSsec(fileno,0,NULL,previous);
	if(ef)
	{
		if(previous!=NULL) FreeRDATData(previous);
		UNLNRDFILE(fileno);
		/*todo - warning dialog*/
		prterr("Error writing Position Identification Master record, posid=[%s]",posid);
		return(-1);
	}
	if(previous!=NULL) FreeRDATData(previous);
	UNLNRDFILE(fileno);
	return(0);
}
