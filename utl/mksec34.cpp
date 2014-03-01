/* SECURITY's - Make (COPYMSMTPRC) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_COPYMSMTPRC()
{
	int x=0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	short engine=0;
/*
	RDApval *pval=NULL,*pvals=NULL;
	int y,z;
	short longest=132;
	int numpvars=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	char *defdir=NULL;
*/
	int imageoff=0;
	RDAimage *image=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;

	char *temp1=NULL,*temp2=NULL,*temp3=NULL;



	memset(stemp,0,101);
	rptname=Rmalloc(11+1);
	sprintf(rptname,"COPYMSMTPRC");
	t=Rmalloc(46+1);
	sprintf(t,"Create MSMTPRC File From User Security Records");
	desc=Rmalloc(43+1);
	sprintf(desc,"Copy MSMTPRC File To User's Home Directory.");
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",FALSE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,0,FALSE,0,NULL,TRUE,0,1,0,pvars,0,0,0,FALSE,"");
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[MY OUTPUT DEVICE]");
	tmp->display->set_lpp=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* SECURITY USERS SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","USERS",0,NULL,NULL,"USERS KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[USER IDENTIFICATION]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,40,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* MSMTPRC VIRTUAL FIELD */
	temp1=Rmalloc(60+1);
	sprintf(temp1,"%s",
		"(\"./\"+[SECURITY][USERS][USER IDENTIFICATION]+\"/msmtprc\")");
	addvirtuallineADV(tmp,"MSMTPRC",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* LOGFILE VIRTUAL FIELD */
	temp1=Rmalloc(62+1);
	sprintf(temp1,"%s",
		"(\"./\"+[SECURITY][USERS][USER IDENTIFICATION]+\"/msmtp.log\")");
	addvirtuallineADV(tmp,"LOGFILE",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* START TLS VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF [SECURITY][USERS][TLS START]=TRUE  THEN RETURN_VALUE=\"on\" ELSE RETURN_VALUE=\"off\" FI");
	addvirtuallineADV(tmp,"START TLS",1,3,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* MY OUTPUT DEVICE VIRTUAL FIELD */
	temp1=Rmalloc(141+1);
	sprintf(temp1,"%s%s",
		"IF [LOGIN IDENTIFICATION]=[USER IDENTIFICATION] THEN  RETURN_VALUE=\"./MoveMSMTPRC.lnx user\" ELSE R",
		"ETURN_VALUE=\"./MoveMSMTPRC.lnx root\" FI");
	addvirtuallineADV(tmp,"MY OUTPUT DEVICE",0,0,temp1,1,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"LAST",3,"[SECURITY][USERS][USER IDENTIFICATION]",0);
	addaccumline(tmp,"LAST",3,"[LOGIN IDENTIFICATION]",0);
	addaccumline(tmp,"LAST",3,"[SECURITY][USERS][TLS]",0);
	addaccumline(tmp,"LAST",3,"[SECURITY][USERS][TLS CERT CHECK]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"DETAIL LINES","@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","USER IDENTIFICATION=@LLLLLLLLLLLLLL   LOGIN IDENTIFICATION=@LLLLLLLLLLLLLLL @@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][USER IDENTIFICATION]",0,NULL);
		addimagepval(image,0,"[LOGIN IDENTIFICATION]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [COPYMSMTPRC], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
void MAKE_RPT_COPYMSMTPRC()
{
	RPT_COPYMSMTPRC();
}
