/* SECURITY's - Make (SETFONTSIZE) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_SETFONTSIZE()
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
	sprintf(rptname,"SETFONTSIZE");
	t=Rmalloc(42+1);
	sprintf(t,"Sets the Font Size for Users if left at 0.");
	desc=Rmalloc(3+1);
	sprintf(desc,"");
#if defined(USE_MYISAM) 
	engine=NRDMyIsamEngine;
#else /* USE_MYISAM */
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
#endif /* USE_MYISAM */
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",FALSE,1,TRUE,"",TRUE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

/* SELECT RECORDS EXPRESSION */

	temp1=Rmalloc(81+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][FONT SIZE]!=[FONT SIZE]) OR ([SECURITY][USERS][FONT WEIGHT]=0)");
	tmp->select=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);




/* SEARCH FILES DEFINITIONS */

/* SECURITY USERS SEARCH FILE */

	temp1=Rmalloc(37+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][DELETEFLAG]=FALSE)");
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
		addsearchconfld(s,"USER IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);






/* VIRTUAL FIELD DEFINITIONS */

/* FONT SIZE VIRTUAL FIELD */
	temp1=Rmalloc(180+1);
	sprintf(temp1,"%s%s",
		"IF(([SECURITY][USERS][FONT SIZE]=8) OR ([SECURITY][USERS][FONT SIZE]=10) OR ([SECURITY][USERS][FONT ",
		"SIZE]=12)) THEN RETURN_VALUE=[SECURITY][USERS][FONT SIZE] ELSE RETURN_VALUE=8 FI");
	addvirtuallineADV(tmp,"FONT SIZE",7,2,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FONT WEIGHT VIRTUAL FIELD */
	temp1=Rmalloc(110+1);
	sprintf(temp1,"%s%s",
		"IF([SECURITY][USERS][FONT WEIGHT]!=0) THEN RETURN_VALUE=[SECURITY][USERS][FONT WEIGHT] ELSE RETURN_V",
		"ALUE=60 FI");
	addvirtuallineADV(tmp,"FONT WEIGHT",7,2,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);






/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][USERS]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][USERS]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][USER IDENTIFICATION]",0,"[SECURITY][USERS][USER IDENTIFICATION]");
		addimagepval(image,0,"[FONT SIZE]",0,"[SECURITY][USERS][FONT SIZE]");
		addimagepval(image,0,"[FONT WEIGHT]",0,"[SECURITY][USERS][FONT WEIGHT]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][USERS]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [SETFONTSIZE], Can Not Save Report!");
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
void SETFONTSIZE()
{
	APPlib *args=NULL;

	RPT_SETFONTSIZE();
	args=APPlibNEW();
	addAPPlib(args,"SECURITY");
	addAPPlib(args,"SETFONTSIZE");
	ExecuteProgram("doreport",args);
	if(args!=NULL) freeapplib(args);
}
