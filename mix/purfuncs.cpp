#ifndef WIN32
#include <mix.hpp>
#endif
#include <venpmt.hpp>
#include <purord.hpp>

short xupdateoutstanding(short fileno,short purlin,int yearid,char *poid,
	int linenum,double cost,double salestax,double adminfee,
	double prorateamt,double amount,short testclosed,short type,
	void (*SubFunc)(...),void *args,RDAvenpmt*venpmt,int line,char *file)
{
	char /* *tmp=NULL, */delflag=FALSE,closed=FALSE;
	double tmpamt=0.0;
	double tmptax=0.0,tmpcost=0.0;
	double tmpadmin=0.0,tmpprorate=0.0;
	double linetax=0.0,linecost=0.0,lineadmin=0.0,lineprorate=0.0;
	double lineamt=0.0;
	RDATData *previous=NULL;
	short ef=FALSE;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG updateoutstanding Updating Outstanding Encumbrances for Year Identification of [%d] Purchase Order [%s] in Module [%s] Fileno [%d] [%s] for Amount [%.02f] at line [%d] program [%s].",
			yearid,poid,MODULENAME(fileno),fileno,FILENAME(fileno),
			amount,line,file); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(fileno);
	FINDFLDSETINT(fileno,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(fileno,"PO IDENTIFICATION",poid);
	LOCNRDFILE(fileno);
	if(SubFunc!=NULL)
	{
		ef=ADVEQLNRDsec(fileno,1,SubFunc,args);
	} else {
		ef=EQLNRDsec(fileno,1);
	}
	if(ef)
	{
		UNLNRDFILE(fileno);
/*
		if(tmp!=NULL) Rfree(tmp); 
		tmp=Rmalloc(RDAstrlen(poid)+212);
		sprintf(tmp,"The PO IDENTIFICATION [%s] that was entered is not a DEFINED PO IDENTIFICATION for the FISCAL YEAR of [%4d].  Check to see if it is defined.  If not and it is needed it needs to be set up.",poid,yearid);
*/
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error updateoutstanding has identifified an invalid PO Identification [%s] for the Year of [%4d] at line [%d] program [%s].",poid,yearid,line,file);
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
		tmp=Rmalloc(RDAstrlen(poid)+100);
		sprintf(tmp,"The Purchase Order [%s] that was entered has been marked for deletion.",poid);
*/
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix)
		{
			prterr("Error updateoutstanding has identified an invalid Purchase Order [%s] for the Year of [%4d] at line [%d] program [%s].  It is marked for deletion at line [%d] program [%s].",poid,yearid,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	} 
	if(testclosed==TRUE)
	{
		FINDFLDGETCHAR(fileno,"CLOSED",&closed);
		if(closed==TRUE)
		{
			UNLNRDFILE(fileno);
/*
			if(tmp!=NULL) Rfree(tmp);
			tmp=Rmalloc(RDAstrlen(poid)+75);
			sprintf(tmp,"The Purchase Order [%s] that was entered is marked as CLOSED.",poid); 
*/
#ifdef USE_RDA_DIAGNOSTICS
			if(diagmix)
			{
				prterr("Error updateoutstanding has identified a Closed Purchase Order [%s] for the Year of [%4d] at line [%d] program [%s].",poid,yearid,line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	}
	previous=RDATDataNEW(fileno);
	FINDFLDGETDOUBLE(fileno,"OUTSTANDING AMOUNT",&tmpamt);
	if(type==0)
	{
		FINDFLDGETDOUBLE(fileno,"TOTAL LINE AMOUNT",&lineamt);
	}
	if(venpmt!=NULL)
	{
		if(venpmt->Use_Taxes==TRUE)
		{
			FINDFLDGETDOUBLE(fileno,"OUTSTANDING COST",&tmpcost);
			FINDFLDGETDOUBLE(fileno,"OUTSTANDING TAX AMOUNT",&tmptax);
			FINDFLDGETDOUBLE(fileno,"OUTSTANDING ADMIN FEE",&tmpadmin);
			FINDFLDGETDOUBLE(fileno,"OUTSTANDING PRORATE AMOUNT",&tmpprorate);
			if(type==0) /* normal */
			{
				FINDFLDGETDOUBLE(fileno,"TOTAL LINE COST",&linecost);
				FINDFLDGETDOUBLE(fileno,"TOTAL LINE TAX AMOUNT",&linetax);
				FINDFLDGETDOUBLE(fileno,"TOTAL LINE ADMIN FEE",&lineadmin);
				FINDFLDGETDOUBLE(fileno,"TOTAL LINE PRORATE AMOUNT",&lineprorate);
			}
		}
	}
	tmpamt+=amount;
	lineamt+=amount;
	FINDFLDSETDOUBLE(fileno,"OUTSTANDING AMOUNT",tmpamt);
	if(type==0)
	{
		FINDFLDSETDOUBLE(fileno,"TOTAL LINE AMOUNT",lineamt);
	}
	if(venpmt!=NULL)
	{
		if(venpmt->Use_Taxes==TRUE)
		{
			tmpcost+=cost;
			tmptax+=salestax;
			tmpadmin+=adminfee;
			tmpprorate+=prorateamt;
			linecost+=cost;
			linetax+=salestax;
			lineadmin+=adminfee;
			lineprorate=prorateamt;
			FINDFLDSETDOUBLE(fileno,"OUTSTANDING COST",tmpcost);
			FINDFLDSETDOUBLE(fileno,"OUTSTANDING TAX AMOUNT",tmptax);
			FINDFLDSETDOUBLE(fileno,"OUTSTANDING ADMIN FEE",tmpadmin);
			FINDFLDSETDOUBLE(fileno,"OUTSTANDING PRORATE AMOUNT",tmpprorate);
			if(type==0) /* normal */
			{
				FINDFLDSETDOUBLE(fileno,"TOTAL LINE COST",linecost);
				FINDFLDSETDOUBLE(fileno,"TOTAL LINE TAX AMOUNT",linetax);
				FINDFLDSETDOUBLE(fileno,"TOTAL LINE ADMIN FEE",lineadmin);
				FINDFLDSETDOUBLE(fileno,"TOTAL LINE PRORATE AMOUNT",lineprorate);
			}
		}
	}
	if(SubFunc!=NULL)
	{
		ef=ADVWRTTRANSsec(fileno,0,NULL,previous,SubFunc,args);
	} else {
		ef=WRTTRANSsec(fileno,0,NULL,previous);
	}
	if(ef)
	{
		if(previous!=NULL) FreeRDATData(previous);
		UNLNRDFILE(fileno);
		/*todo - warning dialog*/
		prterr("Error writing Purchase Order Master record, yearid=[%4d], poid=[%s]",yearid,poid);
		return(-1);
	}
	if(previous!=NULL) FreeRDATData(previous);
	UNLNRDFILE(fileno);
	if(purlin!=(-1))
	{
		ZERNRD(purlin);
		FINDFLDSETINT(purlin,"FISCAL YEAR",yearid);
		FINDFLDSETSTRING(purlin,"PO IDENTIFICATION",poid);
		FINDFLDSETINT(purlin,"LINE NUMBER",linenum);
		LOCNRDFILE(purlin);
		if(SubFunc!=NULL)
		{
			ef=ADVEQLNRDsec(purlin,1,SubFunc,args);
		} else {
			ef=EQLNRDsec(purlin,1);
		}
		if(ef)
		{
			UNLNRDFILE(purlin);
			return(-1);
		}
		previous=RDATDataNEW(purlin);	
		FINDFLDGETDOUBLE(purlin,"OUTSTANDING LINE AMOUNT",&tmpamt);
		tmpamt+=amount;
		if(venpmt!=NULL)
		{
			if(venpmt->Use_Taxes==TRUE)
			{
				FINDFLDGETDOUBLE(purlin,"OUTSTANDING LINE COST",&tmpcost);
				FINDFLDGETDOUBLE(purlin,"OUTSTANDING LINE TAX AMOUNT",&tmptax);
				FINDFLDGETDOUBLE(purlin,"OUTSTANDING LINE ADMIN FEE",&tmpadmin);
				FINDFLDGETDOUBLE(purlin,"OUTSTANDING LINE PRORATE AMOUNT",&tmpprorate);
				tmpcost+=cost;
				tmptax+=salestax;
				tmpadmin+=adminfee;
				tmpprorate+=prorateamt;
				FINDFLDSETDOUBLE(purlin,"OUTSTANDING LINE COST",tmpcost);
				FINDFLDSETDOUBLE(purlin,"OUTSTANDING LINE TAX AMOUNT",tmptax);
				FINDFLDSETDOUBLE(purlin,"OUTSTANDING LINE ADMIN FEE",tmpadmin);
				FINDFLDSETDOUBLE(purlin,"OUTSTANDING LINE PRORATE AMOUNT",tmpprorate);
			}
		}
		FINDFLDSETDOUBLE(purlin,"OUTSTANDING LINE AMOUNT",tmpamt);
		if(SubFunc!=NULL)
		{
			ef=ADVWRTTRANSsec(purlin,0,NULL,previous,SubFunc,args);
		} else {
			ef=WRTTRANSsec(purlin,0,NULL,previous);
		}
		if(ef)
		{
			if(previous!=NULL) FreeRDATData(previous);
			UNLNRDFILE(purlin);
			/*todo - warning dialog*/
			prterr("Error writing Purchase Order Line record, yearid=[%4d], poid=[%s] Line=[%d]",yearid,poid,linenum);
			return(-1);
		}
		UNLNRDFILE(purlin);
	}
	return(0);
}
char *xGetAssignNextPOID(short purids,char *poid,void (*SubFunc)(...),void *args,int line,char *file)
{
	char deleteflag=FALSE,assigned=FALSE;
	char *tmp=NULL;
	RDATData *prev=NULL;
	short ef=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagapps)
	{
		prterr("DIAG GetAssignNextPOID at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(purids);
	LOCNRDFILE(purids);
	FINDFLDSETSTRING(purids,"PO IDENTIFICATION",poid);
	if(SubFunc!=NULL) ef=ADVGTENRDsec(purids,1,SubFunc,args);
		else ef=GTENRDsec(purids,1);
	while(!ef)
	{
		FINDFLDGETCHAR(purids,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(purids,"ASSIGNED",&assigned);
		FINDFLDGETSTRING(purids,"PO IDENTIFICATION",&tmp);
		if(!isEMPTY(poid)) if(strncmp(tmp,poid,RDAstrlen(poid))) break;
		if(!deleteflag && !assigned)
		{
			prev=RDATDataNEW(purids);
			assigned=TRUE;
			FINDFLDSETCHAR(purids,"ASSIGNED",assigned);
			if(SubFunc!=NULL) ADVWRTTRANSsec(purids,0,NULL,prev,SubFunc,args);
				else WRTTRANSsec(purids,0,NULL,prev);
			if(prev!=NULL) FreeRDATData(prev);
			UNLNRDFILE(purids);
			return(tmp);
		}
		if(SubFunc!=NULL) ef=ADVNXTNRDsec(purids,1,SubFunc,args);
			else ef=NXTNRDsec(purids,1);
	}
	if(tmp!=NULL) Rfree(tmp);
	UNLNRDFILE(purids);
	return(NULL);
}
short xCheckPOID(short purids,char *id,void (*SubFunc)(...),void *args,int line,
	char *file)
{
	char deleteflag=TRUE,assigned=TRUE;
	short ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagapps)
	{
		prterr("DIAG CheckPOID at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(purids);
	FINDFLDSETSTRING(purids,"PO IDENTIFICATION",id);
	if(SubFunc!=NULL) ef=ADVEQLNRDsec(purids,1,SubFunc,args);
		else ef=EQLNRDsec(purids,1);
	if(!ef)
	{
		FINDFLDGETCHAR(purids,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(purids,"ASSIGNED",&assigned);
	}
	if(deleteflag || assigned) return(FALSE);
		else return(TRUE);
}
void xAssignPOID(short purids,char *id,void (*SubFunc)(...),void *args,int line,char *file)
{
	RDATData *prev=NULL;
	short ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagapps)
	{
		prterr("DIAG AssignPOID at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(purids);
	FINDFLDSETSTRING(purids,"PO IDENTIFICATION",id);
	LOCNRDFILE(purids);
	if(SubFunc!=NULL) ef=ADVEQLNRDsec(purids,1,SubFunc,args);
		else ef=EQLNRDsec(purids,1);
	if(!ef)
	{
		prev=RDATDataNEW(purids);
		FINDFLDSETCHAR(purids,"ASSIGNED",TRUE);
		FINDFLDSETCHAR(purids,"DELETEFLAG",TRUE);
		if(SubFunc!=NULL) ADVWRTTRANSsec(purids,0,NULL,prev,SubFunc,args);
			else WRTTRANSsec(purids,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
	}
	UNLNRDFILE(purids);
}
void xUndoIDifnoPO(short purids,short purmst,char *id,void (*SubFunc)(...),void *args,
	int line,char *file)
{
	char *test=NULL,assigned=FALSE,force=FALSE;
	RDATData *prev=NULL;
	short keyno=(-1),ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagapps)
	{
		prterr("DIAG UndoIDifnoPO at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(purmst);
	FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",id);
	keyno=KEYNUMBER(purmst,"RDA PURMST KEY");
	if(SubFunc!=NULL) ef=ADVGTENRDsec(purmst,keyno,SubFunc,args);
		else ef=GTENRDsec(purmst,keyno);
	if(ef) 
	{
		ZERNRD(purmst);
		force=TRUE;
	}
	FINDFLDGETSTRING(purmst,"PO IDENTIFICATION",&test);
	if(RDAstrcmp(test,id) || force)
	{
		ZERNRD(purids);
		FINDFLDSETSTRING(purids,"PO IDENTIFICATION",id);
		LOCNRDFILE(purids);
		if(SubFunc!=NULL) ef=ADVEQLNRDsec(purids,1,SubFunc,args);
			else ef=EQLNRDsec(purids,1);
		if(!ef)
		{
			prev=RDATDataNEW(purids);
			FINDFLDSETCHAR(purids,"ASSIGNED",assigned);
			if(SubFunc!=NULL) ADVWRTTRANSsec(purids,0,NULL,prev,SubFunc,args);
				else WRTTRANSsec(purids,0,NULL,prev);
			if(prev!=NULL) FreeRDATData(prev);
		}
		UNLNRDFILE(purids);
	}
	if(test!=NULL) Rfree(test);
}
void xRemovePOID(short purids,char *id,void (*SubFunc)(...),void *args,
	int line,char *file)
{
	RDATData *prev=NULL;
	char deleteflag=TRUE;
	short ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagapps)
	{
		prterr("DIAG RemovePOID at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(purids);
	FINDFLDSETSTRING(purids,"PO IDENTIFICATION",id);
	LOCNRDFILE(purids);
	if(SubFunc!=NULL) ef=ADVEQLNRDsec(purids,1,SubFunc,args);
		else ef=EQLNRDsec(purids,1);
	if(!ef)
	{
		prev=RDATDataNEW(purids);
		FINDFLDSETCHAR(purids,"DELETEFLAG",deleteflag);
		if(SubFunc!=NULL) ADVWRTTRANSsec(purids,0,NULL,prev,SubFunc,args);
			else WRTTRANSsec(purids,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
	}
	UNLNRDFILE(purids);
}
