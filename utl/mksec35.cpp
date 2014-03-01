/* SECURITY's - Make (CREATEMSMTPRC) Report */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void RPT_CREATEMSMTPRC()
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
	rptname=Rmalloc(13+1);
	sprintf(rptname,"CREATEMSMTPRC");
	t=Rmalloc(46+1);
	sprintf(t,"Create MSMTPRC File From User Security Records");
	desc=Rmalloc(46+1);
	sprintf(desc,"Create MSMTPRC File From User Security Records");
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
#ifdef WIN32
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
#else
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,TRUE,"SECURITY","COPYMSMTPRC",FALSE,0,FALSE,"",FALSE,FALSE,FALSE,FALSE);
#endif
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,1,FALSE,0,NULL,TRUE,0,1,0,pvars,0,0,0,FALSE,"");
	temp1=Rmalloc(9+1);
	sprintf(temp1,"%s",
		"[MSMTPRC]");
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

/* APPDATA VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"APPDATA",1,200,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,40,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* MSMTPRC VIRTUAL FIELD */
	temp1=Rmalloc(163+1);
	sprintf(temp1,"%s%s",
		"IF ((STRING_LENGTH([APPDATA]))=0) THEN RETURN_VALUE=(\"./\"+[SECURITY][USERS][USER IDENTIFICATION]+",
		"\"/msmtprc\") ELSE RETURN_VALUE=([APPDATA]+\"\\msmtprc.txt\") FI");
	addvirtuallineADV(tmp,"MSMTPRC",1,400,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* LOGFILE VIRTUAL FIELD */
	temp1=Rmalloc(197+1);
	sprintf(temp1,"%s%s",
		"IF ((STRING_LENGTH([APPDATA]))=0) THEN RETURN_VALUE=(\"./\"+[SECURITY][USERS][USER IDENTIFICATION]+",
		"\"/msmtp.log\") ELSE RETURN_VALUE=(\"\"+[SECURITY][USERS][USER IDENTIFICATION]+\"\\msmtp.log\") FI");
	addvirtuallineADV(tmp,"LOGFILE",1,400,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* START TLS VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF [SECURITY][USERS][TLS START]=TRUE  THEN RETURN_VALUE=\"on\" ELSE RETURN_VALUE=\"off\" FI");
	addvirtuallineADV(tmp,"START TLS",1,3,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"",0,"[SECURITY][USERS][USER IDENTIFICATION]",0);
	addaccumline(tmp,"LAST",3,"[SECURITY][USERS][TLS]",0);
	addaccumline(tmp,"LAST",3,"[SECURITY][USERS][TLS CERT CHECK]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"DETAIL LINES","# Use the SMTP server of your provider@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","account default@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","host @V@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][SMTP SERVER]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","from @V@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][EMAIL]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][TLS]=TRUE)");
	imageoff=addimageline(tmp,"DETAIL LINES","tls on @@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][TLS]=TRUE)");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_starttls @LLL@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[START TLS]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(111+1);
	sprintf(temp1,"%s%s",
		"([SECURITY][USERS][TLS]=TRUE)AND\n((STRING_TO_LOWER([SECURITY][USERS][SMTP SERVER]))=\"smtp.sendit.n",
		"odak.edu\")");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_min_dh_prime_bits 512@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(122+1);
	sprintf(temp1,"%s%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS TRUST FILE]=\"\") AND ([SECURITY][USERS][T",
		"LS CERT CHECK]=FALSE))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_certcheck off @@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(159+1);
	sprintf(temp1,"%s%s",
		"((([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS TRUST FILE]!=\"\")) OR (([SECURITY][USERS",
		"][TLS]=TRUE) AND ([SECURITY][USERS][TLS CERT CHECK]=TRUE)))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_certcheck on @@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(77+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS TRUST FILE]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_trust_file @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][TLS TRUST FILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(75+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS KEY FILE]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_key_file @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][TLS KEY FILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(75+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS CRT FILE]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_cert_file @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][TLS CRT FILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(33+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][SMTP PORT]!=0)");
	imageoff=addimageline(tmp,"DETAIL LINES","port @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][SMTP PORT]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(86+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][SMTP AUTH]!=\"default\") AND ([SECURITY][USERS][SMTP AUTH]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","auth @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][SMTP AUTH]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(78+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][SMTP AUTH]!=\"\") AND ([SECURITY][USERS][USERNAME]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","user @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][USERNAME]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(78+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][SMTP AUTH]!=\"\") AND ([SECURITY][USERS][PASSWORD]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","password @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][PASSWORD]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(37+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][NTLMDOMAIN]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","domain @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][NTLMDOMAIN]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(77+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][PROTOCOL]!=\"smtp\")AND([SECURITY][USERS][PROTOCOL]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","protocol @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][PROTOCOL]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][SMTP AUTH]=\"ntlm\")");
	imageoff=addimageline(tmp,"DETAIL LINES","ntlmdomain @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][NTLMDOMAIN]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][CONNECT TIMEOUT]!=0)");
	imageoff=addimageline(tmp,"DETAIL LINES","timeout @LLL@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][CONNECT TIMEOUT]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","logfile @V@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[LOGFILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+8+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [SECURITY] Report [CREATEMSMTPRC], Can Not Save Report!");
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
void MAKE_RPT_CREATEMSMTPRC()
{
        RPT_CREATEMSMTPRC();
}

