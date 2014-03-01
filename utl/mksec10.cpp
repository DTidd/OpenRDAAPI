/* SECURITY's - Make (COPYMODSEC RANGE SCREEN) Screen */
#include <cstdio>
#include <rptgen.hpp>
#include <mkez.hpp>
#include <mkmsc.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;
extern void free_pvals(RDApval *,int);
extern void free_pvars(RDApvar *,int);


static void RPT_COPYSECURITY()
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
	rptname=Rmalloc(12+1);
	sprintf(rptname,"COPYSECURITY");
	t=Rmalloc(20+1);
	sprintf(t,"Copy Module Security");
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
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)8.500000,(double)11.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,1,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* SECURITY MODSEC SEARCH FILE */

	temp1=Rmalloc(68+1);
	sprintf(temp1,"%s",
		"([SECURITY][MODSEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","MODSEC",0,NULL,NULL,"MODSEC KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",2,"SECURITY","MODSEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[MODSEC][USER IDENTIFICATION]");
	}


/* SECURITY PROCSEC SEARCH FILE */

	temp1=Rmalloc(69+1);
	sprintf(temp1,"%s",
		"([SECURITY][PROCSEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","PROCSEC",0,NULL,NULL,"PROCSEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"PROCESS NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",2,"SECURITY","PROCSEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[PROCSEC][USER IDENTIFICATION]");
	}


/* SECURITY FILESEC SEARCH FILE */

	temp1=Rmalloc(69+1);
	sprintf(temp1,"%s",
		"([SECURITY][FILESEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","FILESEC",0,NULL,NULL,"FILESEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"FILE NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",2,"SECURITY","FILESEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FILESEC][USER IDENTIFICATION]");
	}


/* SECURITY FLDSEC SEARCH FILE */

	temp1=Rmalloc(68+1);
	sprintf(temp1,"%s",
		"([SECURITY][FLDSEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","FLDSEC",0,NULL,NULL,"FLDSEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"FILE NAME","");
		addsearchconfld(s,"FIELD NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",1,"SECURITY","FLDSEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FLDSEC][USER IDENTIFICATION]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* TO USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"TO USER IDENTIFICATION",1,15,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FROM USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"FROM USER IDENTIFICATION",1,15,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* WHICH VIRTUAL FIELD */
	temp1=Rmalloc(21+1);
	sprintf(temp1,"%s",
		"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"WHICH",7,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETEFLAG VIRTUAL FIELD */
	temp1=Rmalloc(312+1);
	sprintf(temp1,"%s%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][DELETEFLAG] ELSE \nIF([WHICH]=2) THEN RETURN_VALU",
		"E=[SECURITY][PROCSEC][DELETEFLAG] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][DELETEF",
		"LAG] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][DELETEFLAG] ELSE \nRETURN_VALUE=FALSE",
		" FI FI FI FI");
	addvirtuallineADV(tmp,"DELETEFLAG",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETEFLAG FLDSEC VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][DELETEFLAG] ELSE \nRETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"DELETEFLAG FLDSEC",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETEFLAG FILESEC VIRTUAL FIELD */
	temp1=Rmalloc(92+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][DELETEFLAG] ELSE \nRETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"DELETEFLAG FILESEC",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETEFLAG PROCSEC VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=2) THEN RETURN_VALUE=[SECURITY][PROCSEC][DELETEFLAG] ELSE\nRETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"DELETEFLAG PROCSEC",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETEFLAG MODSEC VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][DELETEFLAG] ELSE \nRETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"DELETEFLAG MODSEC",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* WRITE DELETEFLAG VIRTUAL FIELD */
	temp1=Rmalloc(206+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=TRUE ELSE \nIF([WHICH]=2) THEN RETURN_VALUE=TRUE ELSE \nIF([WHICH]=3",
		") THEN RETURN_VALUE=TRUE ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=TRUE ELSE \nRETURN_VALUE=FALSE FI FI",
		" FI FI");
	addvirtuallineADV(tmp,"WRITE DELETEFLAG",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(347+1);
	sprintf(temp1,"%s%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][USER IDENTIFICATION] ELSE \nIF([WHICH]=2) THEN RE",
		"TURN_VALUE=[SECURITY][PROCSEC][USER IDENTIFICATION] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE=[SECURITY",
		"][FILESEC][USER IDENTIFICATION] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][USER IDENT",
		"IFICATION] ELSE \nRETURN_VALUE=\"\" FI FI FI FI");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* MODULE NAME VIRTUAL FIELD */
	temp1=Rmalloc(317+1);
	sprintf(temp1,"%s%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][MODULE NAME] ELSE \nIF([WHICH]=2) THEN RETURN_VAL",
		"UE=[SECURITY][PROCSEC][MODULE NAME] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][MODUL",
		"E NAME] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][MODULE NAME] ELSE \nRETURN_VALUE=",
		"\"\" FI FI FI FI\n");
	addvirtuallineADV(tmp,"MODULE NAME",1,15,temp1,0,1);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FILE NAME VIRTUAL FIELD */
	temp1=Rmalloc(162+1);
	sprintf(temp1,"%s%s",
		"IF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][FILE NAME] ELSE \nIF([WHICH]=4) THEN RETURN_VALU",
		"E=[SECURITY][FLDSEC][FILE NAME] ELSE \nRETURN_VALUE=\"\" FI FI");
	addvirtuallineADV(tmp,"FILE NAME",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FIELD NAME VIRTUAL FIELD */
	temp1=Rmalloc(90+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][FIELD NAME] ELSE \nRETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"FIELD NAME",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* PROCESS NAME VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=2) THEN RETURN_VALUE=[SECURITY][PROCSEC][PROCESS NAME] ELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"PROCESS NAME",1,80,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ALL FILES VIRTUAL FIELD */
	temp1=Rmalloc(88+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][ALL FILES] ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"ALL FILES",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ALL PROCESSES VIRTUAL FIELD */
	temp1=Rmalloc(92+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][ALL PROCESSES] ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"ALL PROCESSES",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* EXECUTE FUNCTIONS VIRTUAL FIELD */
	temp1=Rmalloc(177+1);
	sprintf(temp1,"%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][EXECUTE FUNCTIONS] ELSE \nIF([WHICH]=2) THEN RETU",
		"RN_VALUE=[SECURITY][PROCSEC][EXECUTE PROCESS] ELSE \nRETURN_VALUE=FALSE FI FI");
	addvirtuallineADV(tmp,"EXECUTE FUNCTIONS",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* READ DATA VIRTUAL FIELD */
	temp1=Rmalloc(235+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][READ DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE",
		"=[SECURITY][FILESEC][READ FILE] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][READ FIELD",
		"] ELSE\nRETURN_VALUE=FALSE FI FI FI");
	addvirtuallineADV(tmp,"READ DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* WRITE DATA VIRTUAL FIELD */
	temp1=Rmalloc(239+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][WRITE DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VALU",
		"E=[SECURITY][FILESEC][WRITE FILE] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][WRITE FI",
		"ELD] ELSE \nRETURN_VALUE=FALSE FI FI FI");
	addvirtuallineADV(tmp,"WRITE DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* PRINT DATA VIRTUAL FIELD */
	temp1=Rmalloc(239+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][PRINT DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VALU",
		"E=[SECURITY][FILESEC][PRINT FILE] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][PRINT FI",
		"ELD] ELSE \nRETURN_VALUE=FALSE FI FI FI");
	addvirtuallineADV(tmp,"PRINT DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETE DATA VIRTUAL FIELD */
	temp1=Rmalloc(167+1);
	sprintf(temp1,"%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][DELETE DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VAL",
		"UE=[SECURITY][FILESEC][DELETE FILE] ELSE \nRETURN_VALUE=FALSE FI FI");
	addvirtuallineADV(tmp,"DELETE DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FIELD SECURITY VIRTUAL FIELD */
	temp1=Rmalloc(94+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][FIELD SECURITY] ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"FIELD SECURITY",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* EXPRESSION VIRTUAL FIELD */
	temp1=Rmalloc(89+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][EXPRESSION] ELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"EXPRESSION",14,999,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"LAST TO USER IDENTIFICATION",3,"[TO USER IDENTIFICATION]",0);
	addaccumline(tmp,"LAST WHICH",3,"[WHICH]",0);
	addaccumline(tmp,"LAST WRITE DELETEFLAG",3,"[WRITE DELETEFLAG]",0);
	addaccumline(tmp,"LAST DELETEFLAG",3,"[DELETEFLAG]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][MODSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][PROCSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][FILESEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][FLDSEC]",FALSE,NULL,0,NULL);

	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"([WHICH]=1)");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][MODSEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[ALL FILES]",0,"[SECURITY][MODSEC][ALL FILES]");
		addimagepval(image,0,"[ALL PROCESSES]",0,"[SECURITY][MODSEC][ALL PROCESSES]");
		addimagepval(image,0,"[DELETEFLAG MODSEC]",0,"[SECURITY][MODSEC][DELETEFLAG]");
		addimagepval(image,0,"[EXECUTE FUNCTIONS]",0,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][MODSEC][MODULE NAME]");
		addimagepval(image,0,"[PRINT DATA]",0,"[SECURITY][MODSEC][PRINT DATA]");
		addimagepval(image,0,"[READ DATA]",0,"[SECURITY][MODSEC][READ DATA]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][MODSEC][USER IDENTIFICATION]");
		addimagepval(image,0,"[WRITE DATA]",0,"[SECURITY][MODSEC][WRITE DATA]");
		addimagepval(image,0,"[DELETE DATA]",0,"[SECURITY][MODSEC][DELETE DATA]");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"([WHICH]=2)");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][PROCSEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[DELETEFLAG PROCSEC]",0,"[SECURITY][PROCSEC][DELETEFLAG]");
		addimagepval(image,0,"[EXECUTE FUNCTIONS]",0,"[SECURITY][PROCSEC][EXECUTE PROCESS]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][PROCSEC][MODULE NAME]");
		addimagepval(image,0,"[PROCESS NAME]",0,"[SECURITY][PROCSEC][PROCESS NAME]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][PROCSEC][USER IDENTIFICATION]");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"([WHICH]=3)");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][FILESEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[DELETEFLAG FILESEC]",0,"[SECURITY][FILESEC][DELETEFLAG]");
		addimagepval(image,0,"[EXPRESSION]",0,"[SECURITY][FILESEC][EXPRESSION]");
		addimagepval(image,0,"[FIELD SECURITY]",0,"[SECURITY][FILESEC][FIELD SECURITY]");
		addimagepval(image,0,"[FILE NAME]",0,"[SECURITY][FILESEC][FILE NAME]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][FILESEC][MODULE NAME]");
		addimagepval(image,0,"[PRINT DATA]",0,"[SECURITY][FILESEC][PRINT FILE]");
		addimagepval(image,0,"[READ DATA]",0,"[SECURITY][FILESEC][READ FILE]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][FILESEC][USER IDENTIFICATION]");
		addimagepval(image,0,"[WRITE DATA]",0,"[SECURITY][FILESEC][WRITE FILE]");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(11+1);
	sprintf(temp1,"%s",
		"([WHICH]=4)");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][FLDSEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[DELETEFLAG FLDSEC]",0,"[SECURITY][FLDSEC][DELETEFLAG]");
		addimagepval(image,0,"[FIELD NAME]",0,"[SECURITY][FLDSEC][FIELD NAME]");
		addimagepval(image,0,"[FILE NAME]",0,"[SECURITY][FLDSEC][FILE NAME]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][FLDSEC][MODULE NAME]");
		addimagepval(image,0,"[PRINT DATA]",0,"[SECURITY][FLDSEC][PRINT FIELD]");
		addimagepval(image,0,"[READ DATA]",0,"[SECURITY][FLDSEC][READ FIELD]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][FLDSEC][USER IDENTIFICATION]");
		addimagepval(image,0,"[WRITE DATA]",0,"[SECURITY][FLDSEC][WRITE FIELD]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][MODSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][PROCSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][FILESEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][FLDSEC]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [COPYSECURITY], Can Not Save Report!");
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
static void SCN_COPYSECURITY_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","COPYSECURITY RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Copy From User ID:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"COPYMODSEC FROM USER IDENTIFICATIONS","From User Identifications","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","To User ID:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"COPYMODSEC TO USER IDENTIFICATIONS","To User Identifications","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE NAME]","Range on Modules","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Copy Security","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(25+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [COPYSECURITY RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void COPYSECURITY_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("COPYSECURITY","doreport.EXT SECURITY COPYSECURITY");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
static void COPYMODSEC_FROM_USER_IDENTIFICATIONS()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAScrolledListNew("SECURITY","COPYMODSEC FROM USER IDENTIFICATIONS");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=0;
		ScrolledListDFincvir(scrn,"SECURITY","USERS","",0);
		scrn->select_formula=stralloc("([SECURITY][USERS][DELETEFLAG]=FALSE)");
		scrn->special_function=stralloc("");
		scrn->num=0;
		scrn->initfld=NULL;
		scrn->ffield=stralloc("");
		scrn->ffinfo=Rmalloc(sizeof(DFincvir));
		scrn->ffinfo->module=stralloc("SECURITY");
		scrn->ffinfo->file=stralloc("USERS");
		scrn->ffinfo->keyname=stralloc("USER KEY");
		scrn->contype=3;
		scrn->conmod=stralloc("FROM USER IDENTIFICATION");
		scrn->confil=stralloc("");
		scrn->confld=stralloc("");
		scrn->format_formula=stralloc("[SECURITY][USERS][USER IDENTIFICATION]");
		scrn->unformat_formula=stralloc("");
		scrn->desc=stralloc("");
		scrn->timing=0;

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This RDAScrolledList may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDAScrolledList(scrn);
	}
}
static void COPYMODSEC_TO_USER_IDENTIFICATIONS()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAScrolledListNew("SECURITY","COPYMODSEC TO USER IDENTIFICATIONS");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=0;
		ScrolledListDFincvir(scrn,"SECURITY","USERS","",0);
		scrn->select_formula=stralloc("([SECURITY][USERS][DELETEFLAG]=FALSE)");
		scrn->special_function=stralloc("");
		scrn->num=0;
		scrn->initfld=NULL;
		scrn->ffield=stralloc("");
		scrn->ffinfo=Rmalloc(sizeof(DFincvir));
		scrn->ffinfo->module=stralloc("SECURITY");
		scrn->ffinfo->file=stralloc("USERS");
		scrn->ffinfo->keyname=stralloc("USER KEY");
		scrn->contype=3;
		scrn->conmod=stralloc("TO USER IDENTIFICATION");
		scrn->confil=stralloc("");
		scrn->confld=stralloc("");
		scrn->format_formula=stralloc("[SECURITY][USERS][USER IDENTIFICATION]");
		scrn->unformat_formula=stralloc("");
		scrn->desc=stralloc("");
		scrn->timing=0;

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This RDAScrolledList may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDAScrolledList(scrn);
	}
}
static void RPT_COPYSECURITYADD()
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
	rptname=Rmalloc(15+1);
	sprintf(rptname,"COPYSECURITYADD");
	t=Rmalloc(20+1);
	sprintf(t,"Copy Module Security");
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
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)8.500000,(double)11.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,1,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* SECURITY MODSEC SEARCH FILE */

	temp1=Rmalloc(68+1);
	sprintf(temp1,"%s",
		"([SECURITY][MODSEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","MODSEC",0,NULL,NULL,"MODSEC KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",2,"SECURITY","MODSEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[MODSEC][USER IDENTIFICATION]");
	}


/* SECURITY PROCSEC SEARCH FILE */

	temp1=Rmalloc(69+1);
	sprintf(temp1,"%s",
		"([SECURITY][PROCSEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","PROCSEC",0,NULL,NULL,"PROCSEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"PROCESS NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",2,"SECURITY","PROCSEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[PROCSEC][USER IDENTIFICATION]");
	}


/* SECURITY FILESEC SEARCH FILE */

	temp1=Rmalloc(69+1);
	sprintf(temp1,"%s",
		"([SECURITY][FILESEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","FILESEC",0,NULL,NULL,"FILESEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"FILE NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",2,"SECURITY","FILESEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FILESEC][USER IDENTIFICATION]");
	}


/* SECURITY FLDSEC SEARCH FILE */

	temp1=Rmalloc(68+1);
	sprintf(temp1,"%s",
		"([SECURITY][FLDSEC][USER IDENTIFICATION]=[FROM USER IDENTIFICATION])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","FLDSEC",0,NULL,NULL,"FLDSEC KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FROM USER IDENTIFICATION]");
		addsearchconfld(s,"MODULE NAME","");
		addsearchconfld(s,"FILE NAME","");
		addsearchconfld(s,"FIELD NAME","");
	}
	if(t!=NULL) Rfree(t);


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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",1,"SECURITY","FLDSEC","USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[FLDSEC][USER IDENTIFICATION]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* TO USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"TO USER IDENTIFICATION",1,15,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FROM USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"FROM USER IDENTIFICATION",1,15,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* WHICH VIRTUAL FIELD */
	temp1=Rmalloc(21+1);
	sprintf(temp1,"%s",
		"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"WHICH",7,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETEFLAG VIRTUAL FIELD */
	temp1=Rmalloc(312+1);
	sprintf(temp1,"%s%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][DELETEFLAG] ELSE \nIF([WHICH]=2) THEN RETURN_VALU",
		"E=[SECURITY][PROCSEC][DELETEFLAG] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][DELETEF",
		"LAG] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][DELETEFLAG] ELSE \nRETURN_VALUE=FALSE",
		" FI FI FI FI");
	addvirtuallineADV(tmp,"DELETEFLAG",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(347+1);
	sprintf(temp1,"%s%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][USER IDENTIFICATION] ELSE \nIF([WHICH]=2) THEN RE",
		"TURN_VALUE=[SECURITY][PROCSEC][USER IDENTIFICATION] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE=[SECURITY",
		"][FILESEC][USER IDENTIFICATION] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][USER IDENT",
		"IFICATION] ELSE \nRETURN_VALUE=\"\" FI FI FI FI");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* MODULE NAME VIRTUAL FIELD */
	temp1=Rmalloc(317+1);
	sprintf(temp1,"%s%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][MODULE NAME] ELSE \nIF([WHICH]=2) THEN RETURN_VAL",
		"UE=[SECURITY][PROCSEC][MODULE NAME] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][MODUL",
		"E NAME] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][MODULE NAME] ELSE \nRETURN_VALUE=",
		"\"\" FI FI FI FI\n");
	addvirtuallineADV(tmp,"MODULE NAME",1,15,temp1,0,1);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FILE NAME VIRTUAL FIELD */
	temp1=Rmalloc(162+1);
	sprintf(temp1,"%s%s",
		"IF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][FILE NAME] ELSE \nIF([WHICH]=4) THEN RETURN_VALU",
		"E=[SECURITY][FLDSEC][FILE NAME] ELSE \nRETURN_VALUE=\"\" FI FI");
	addvirtuallineADV(tmp,"FILE NAME",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FIELD NAME VIRTUAL FIELD */
	temp1=Rmalloc(90+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][FIELD NAME] ELSE \nRETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"FIELD NAME",1,40,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* PROCESS NAME VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=2) THEN RETURN_VALUE=[SECURITY][PROCSEC][PROCESS NAME] ELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"PROCESS NAME",1,80,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ALL FILES VIRTUAL FIELD */
	temp1=Rmalloc(88+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][ALL FILES] ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"ALL FILES",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ALL PROCESSES VIRTUAL FIELD */
	temp1=Rmalloc(92+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][ALL PROCESSES] ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"ALL PROCESSES",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* EXECUTE FUNCTIONS VIRTUAL FIELD */
	temp1=Rmalloc(177+1);
	sprintf(temp1,"%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][EXECUTE FUNCTIONS] ELSE \nIF([WHICH]=2) THEN RETU",
		"RN_VALUE=[SECURITY][PROCSEC][EXECUTE PROCESS] ELSE \nRETURN_VALUE=FALSE FI FI");
	addvirtuallineADV(tmp,"EXECUTE FUNCTIONS",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* READ DATA VIRTUAL FIELD */
	temp1=Rmalloc(235+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][READ DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VALUE",
		"=[SECURITY][FILESEC][READ FILE] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][READ FIELD",
		"] ELSE\nRETURN_VALUE=FALSE FI FI FI");
	addvirtuallineADV(tmp,"READ DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* WRITE DATA VIRTUAL FIELD */
	temp1=Rmalloc(239+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][WRITE DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VALU",
		"E=[SECURITY][FILESEC][WRITE FILE] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][WRITE FI",
		"ELD] ELSE \nRETURN_VALUE=FALSE FI FI FI");
	addvirtuallineADV(tmp,"WRITE DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* PRINT DATA VIRTUAL FIELD */
	temp1=Rmalloc(239+1);
	sprintf(temp1,"%s%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][PRINT DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VALU",
		"E=[SECURITY][FILESEC][PRINT FILE] ELSE \nIF([WHICH]=4) THEN RETURN_VALUE=[SECURITY][FLDSEC][PRINT FI",
		"ELD] ELSE \nRETURN_VALUE=FALSE FI FI FI");
	addvirtuallineADV(tmp,"PRINT DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* DELETE DATA VIRTUAL FIELD */
	temp1=Rmalloc(167+1);
	sprintf(temp1,"%s%s",
		"IF([WHICH]=1) THEN RETURN_VALUE=[SECURITY][MODSEC][DELETE DATA] ELSE \nIF([WHICH]=3) THEN RETURN_VAL",
		"UE=[SECURITY][FILESEC][DELETE FILE] ELSE \nRETURN_VALUE=FALSE FI FI");
	addvirtuallineADV(tmp,"DELETE DATA",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* FIELD SECURITY VIRTUAL FIELD */
	temp1=Rmalloc(94+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][FIELD SECURITY] ELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"FIELD SECURITY",10,1,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* EXPRESSION VIRTUAL FIELD */
	temp1=Rmalloc(89+1);
	sprintf(temp1,"%s",
		"IF([WHICH]=3) THEN RETURN_VALUE=[SECURITY][FILESEC][EXPRESSION] ELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"EXPRESSION",14,999,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"LAST TO USER IDENTIFICATION",3,"[TO USER IDENTIFICATION]",0);
	addaccumline(tmp,"LAST WHICH",3,"[WHICH]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][MODSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][PROCSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][FILESEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][FLDSEC]",FALSE,NULL,0,NULL);

	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(48+1);
	sprintf(temp1,"%s",
		"([WHICH]=1) AND ([TO USER IDENTIFICATION]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][MODSEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[ALL FILES]",0,"[SECURITY][MODSEC][ALL FILES]");
		addimagepval(image,0,"[ALL PROCESSES]",0,"[SECURITY][MODSEC][ALL PROCESSES]");
		addimagepval(image,0,"[DELETEFLAG]",0,"[SECURITY][MODSEC][DELETEFLAG]");
		addimagepval(image,0,"[EXECUTE FUNCTIONS]",0,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][MODSEC][MODULE NAME]");
		addimagepval(image,0,"[PRINT DATA]",0,"[SECURITY][MODSEC][PRINT DATA]");
		addimagepval(image,0,"[READ DATA]",0,"[SECURITY][MODSEC][READ DATA]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][MODSEC][USER IDENTIFICATION]");
		addimagepval(image,0,"[WRITE DATA]",0,"[SECURITY][MODSEC][WRITE DATA]");
		addimagepval(image,0,"[DELETE DATA]",0,"[SECURITY][MODSEC][DELETE DATA]");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(48+1);
	sprintf(temp1,"%s",
		"([WHICH]=2) AND ([TO USER IDENTIFICATION]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][PROCSEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[DELETEFLAG]",0,"[SECURITY][PROCSEC][DELETEFLAG]");
		addimagepval(image,0,"[EXECUTE FUNCTIONS]",0,"[SECURITY][PROCSEC][EXECUTE PROCESS]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][PROCSEC][MODULE NAME]");
		addimagepval(image,0,"[PROCESS NAME]",0,"[SECURITY][PROCSEC][PROCESS NAME]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][PROCSEC][USER IDENTIFICATION]");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(48+1);
	sprintf(temp1,"%s",
		"([WHICH]=3) AND ([TO USER IDENTIFICATION]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][FILESEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[DELETEFLAG]",0,"[SECURITY][FILESEC][DELETEFLAG]");
		addimagepval(image,0,"[EXPRESSION]",0,"[SECURITY][FILESEC][EXPRESSION]");
		addimagepval(image,0,"[FIELD SECURITY]",0,"[SECURITY][FILESEC][FIELD SECURITY]");
		addimagepval(image,0,"[FILE NAME]",0,"[SECURITY][FILESEC][FILE NAME]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][FILESEC][MODULE NAME]");
		addimagepval(image,0,"[PRINT DATA]",0,"[SECURITY][FILESEC][PRINT FILE]");
		addimagepval(image,0,"[READ DATA]",0,"[SECURITY][FILESEC][READ FILE]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][FILESEC][USER IDENTIFICATION]");
		addimagepval(image,0,"[WRITE DATA]",0,"[SECURITY][FILESEC][WRITE FILE]");
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(48+1);
	sprintf(temp1,"%s",
		"([WHICH]=4) AND ([TO USER IDENTIFICATION]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][FLDSEC]",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[DELETEFLAG]",0,"[SECURITY][FLDSEC][DELETEFLAG]");
		addimagepval(image,0,"[FIELD NAME]",0,"[SECURITY][FLDSEC][FIELD NAME]");
		addimagepval(image,0,"[FILE NAME]",0,"[SECURITY][FLDSEC][FILE NAME]");
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][FLDSEC][MODULE NAME]");
		addimagepval(image,0,"[PRINT DATA]",0,"[SECURITY][FLDSEC][PRINT FIELD]");
		addimagepval(image,0,"[READ DATA]",0,"[SECURITY][FLDSEC][READ FIELD]");
		addimagepval(image,0,"[TO USER IDENTIFICATION]",0,"[SECURITY][FLDSEC][USER IDENTIFICATION]");
		addimagepval(image,0,"[WRITE DATA]",0,"[SECURITY][FLDSEC][WRITE FIELD]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][MODSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][PROCSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][FILESEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][FLDSEC]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [COPYSECURITYADD], Can Not Save Report!");
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
static void SCN_COPYSECURITYADD_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","COPYSECURITYADD RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Copy From User ID:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"COPYMODSEC FROM USER IDENTIFICATIONS","From User Identifications","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE NAME]","Range on Modules","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Copy Security","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(28+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [COPYSECURITYADD RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
void copymodsec()
{
	RPT_COPYSECURITY();
	SCN_COPYSECURITY_RANGE_SCREEN();
	RPT_COPYSECURITYADD();
	SCN_COPYSECURITYADD_RANGE_SCREEN();
	COPYSECURITY_MENU();
	COPYMODSEC_FROM_USER_IDENTIFICATIONS();
	COPYMODSEC_TO_USER_IDENTIFICATIONS();
}

static void RPT_ADDMODSEC()
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
	rptname=Rmalloc(9+1);
	sprintf(rptname,"ADDMODSEC");
	t=Rmalloc(19+1);
	sprintf(t,"Add Module Security");
	desc=Rmalloc(30+1);
	sprintf(desc,"Add Security in Mass by Module");
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)8.500000,(double)11.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",0,NULL,NULL,"USERS KEY",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* MODULE NAME VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"MODULE NAME",1,15,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);





/* SORT DEFINITIONS */

	addsortline(tmp,0,"[SECURITY][USERS][USER IDENTIFICATION]");


	SetReportDefaultEngine(tmp);


/* CONTROL BREAKS */

	addcontrolline(tmp,"[SECURITY][USERS][USER IDENTIFICATION]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"REPORT HEADER","OPEN [SECURITY][MODSEC]",FALSE,NULL,0,NULL);

	imageoff=addimageline(tmp,"DETAIL LINES","WRITE [SECURITY][MODSEC]",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[MODULE NAME]",0,"[SECURITY][MODSEC][MODULE NAME]");
		addimagepval(image,0,"[SECURITY][USERS][USER IDENTIFICATION]",0,"[SECURITY][MODSEC][USER IDENTIFICATION]");
		addimagepval(image,1,"FALSE",0,"[SECURITY][MODSEC][DELETEFLAG]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][MODSEC][ALL FILES]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][MODSEC][ALL PROCESSES]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][MODSEC][DELETE DATA]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][MODSEC][PRINT DATA]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][MODSEC][READ DATA]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][MODSEC][WRITE DATA]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][MODSEC]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [ADDMODSEC], Can Not Save Report!");
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
static void SCN_ADDMODSEC_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","ADDMODSEC RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"FROM MODSEC MODULE LIST","","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE NAME","Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+15+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [ADDMODSEC RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void MNU_SECURITY_ADDMODSEC()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("SECURITY ADDMODSEC","doreport.EXT SECURITY \"ADDMODSEC\"");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}

void RPTADDMODSEC()
{
	RPT_ADDMODSEC();
	SCN_ADDMODSEC_RANGE_SCREEN();
	MNU_SECURITY_ADDMODSEC();
}
static void RPT_SETWTPASSWD()
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
	sprintf(rptname,"SETWTPASSWD");
	t=Rmalloc(41+1);
	sprintf(t,"Set's User WT PASSWORD to a Default Value");
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
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",TRUE,FALSE,FALSE,FALSE);
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

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* SECURITY USERS SEARCH FILE */

	temp1=Rmalloc(237+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][DELETEFLAG]=FALSE)");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","USERS",0,NULL,NULL,"USERS KEY",TRUE,temp1,temp2,1,0,temp3);
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

/* WT PASSWORD VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"WT PASSWORD",1,60,temp1,0,2);
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
		addimagepval(image,1,"(WT_BCRYPT_VALUE([WT PASSWORD]))",0,"[SECURITY][USERS][WT PASSWORD]");
		addimagepval(image,1,"TRUE",0,"[SECURITY][USERS][WT FORCE PASSWORD CHANGE]");
	}


	imageoff=addimageline(tmp,"REPORT FOOTER","CLOSE [SECURITY][USERS]",FALSE,NULL,0,NULL);

	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [SETWTPASSWD], Can Not Save Report!");
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
static void SCN_SETWTPASSWD_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","SETWTPASSWD RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Report Description","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(91+1);
		sprintf(temp1,"%s",
			"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SET'S USER WT PASSWORD TO A DEFAULT VALUE","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Range","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","New Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WT PASSWORD","New Password:","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][CELL PHONE]","Range on Cell Phone","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][CELL PHONE]","From Cell Phone","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][CELL PHONE]","To Cell Phone","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEPARTMENT]","Range on Department","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEPARTMENT]","From Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEPARTMENT]","To Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][EMAIL]","Range on Email","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][EMAIL]","From Email","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][EMAIL]","To Email","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][LOCATION IDENTIFICATION]","Range on Location Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][LOCATION IDENTIFICATION]","From Location Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][LOCATION IDENTIFICATION]","To Location Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][SITE IDENTIFICATION]","Range on Site Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][SITE IDENTIFICATION]","From Site Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][SITE IDENTIFICATION]","To Site Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER NAME]","Range on User Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER NAME]","From User Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER NAME]","To User Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USERNAME]","Range on Username","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USERNAME]","From Username","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USERNAME]","To Username","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,25,NULL,NULL,"([WT PASSWORD]!=\"\")",NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(24+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [SETWTPASSWD RANGE SCREEN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
void SETWTPASSWD()
{
	APPlib *args=NULL;

	RPT_SETWTPASSWD();
	SCN_SETWTPASSWD_RANGE_SCREEN();
	args=APPlibNEW();
	addAPPlib(args,"SECURITY");
	addAPPlib(args,"SETWTPASSWD");
	Execute2Program("doreport",args);
	if(args!=NULL) freeapplib(args);
}
