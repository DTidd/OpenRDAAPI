/* dftable.c - Define Tables */
#ifndef WIN32
#define __NAM__ "paytbl.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paytbl.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

RDATABLE *DFV=NULL;

static void MakeTable(short fileno)
{
	RDATABLE *DFV=NULL;
	TABLEVALUE *v=NULL;
	char *name=NULL,*libname=NULL;
	int x=0;
	double u=0.0,b=0.0,p=0.0;

	FINDFLDGETSTRING(fileno,"DESCRIPTION",&name);
	DFV=RDATABLENEW("PAYROLL",name);
	if(DFV!=NULL)
	{
		DFV->num=0;
		for(x=1;x<19;++x)
		{
			sprintf(stemp,"UPPER AMOUNT %d",x);
			FINDFLDGETDOUBLE(fileno,stemp,&u);
			sprintf(stemp,"BASE AMOUNT %d",x);
			FINDFLDGETDOUBLE(fileno,stemp,&b);
			sprintf(stemp,"PERCENTAGE %d",x);	
			FINDFLDGETDOUBLE(fileno,stemp,&p);
			if(u==0.0) break;
			++DFV->num;
			if(DFV->values!=NULL)
			{
				DFV->values=Rrealloc(DFV->values,(DFV->num*sizeof(TABLEVALUE)));
			} else {
				DFV->values=Rmalloc((DFV->num*sizeof(TABLEVALUE)));
			}
			v=DFV->values+(DFV->num-1);
			v->amount=u;
			v->base=b;
			v->rate=p/100;
		}
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(DFV->module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,DFV->module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,DFV->module);
#endif			
		if(writeRDATablebin(libname,DFV))
		{
			prterr("Error:  Cannot write RDA Table Definition.  Check the permissions on the RDA Table Binary Library, and retry.");
		}
		if(libname!=NULL) Rfree(libname);
		FreeRDATABLE(DFV);
		DFV=NULL;
	}
}
int main(int argc,char **argv)
{
	short fileno=(-1),ef=0;
	char delflag=FALSE;

	if(InitializeSubsystems(argc,argv,"CONVERSION","PAYTBL")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	fileno=OPNNRD("CONVERSION","PAYTBL");
	if(fileno!=(-1))
	{
		ef=BEGNRD(fileno);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				MakeTable(fileno);
			}
			ef=SEQNRD(fileno);
		}
		CLSNRD(fileno);
	}	
	ShutdownSubsystems();
	exit(0);
}
