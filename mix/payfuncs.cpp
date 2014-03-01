/* payfuncs.c - Payroll Setup Variable Functions */
#include<pay.hpp>
#include<mix.hpp>

short xgetpayrollbin(RDApayroll *payroll,int line,char *file)
{
	short x,test=FALSE,version=0;
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/PAYROLL.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PAYROLL.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG getpayrollbin Reading Payroll Setup Variables from [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,"PAYROLL SETUP",TRUE);
	if(bin==NULL) 
	{
		prterr("Error Payroll Setup Variables were not found in Library [%s] or the Library doesn't exist at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read Payroll Setup Variables at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(x==(-1696)) version=5;
	else if(x==(-1697)) version=4;
	else if(x==(-1698)) version=3;
	else if(x==(-1699)) version=2;
	else if(x==(-1700)) version=1;
	else version=0;
	if(version>4)
	{
		payroll->manage_earnings=BINgetshort(bin);
	} else {
		payroll->manage_earnings=FALSE;
	}
	if(version>2)	
	{
		payroll->use_payroll_fund=BINgetshort(bin);
	} else {
		test=BINgetshort(bin);
		if(test) payroll->use_payroll_fund=TRUE;
			else payroll->use_payroll_fund=FALSE;
	}
	test=BINgetshort(bin);
	if(test) payroll->paysel_audit=TRUE;
		else payroll->paysel_audit=FALSE;
	test=BINgetshort(bin);
	if(test) payroll->paynet_audit=TRUE;
		else payroll->paynet_audit=FALSE;
	test=BINgetshort(bin);
	if(test) payroll->paymts_audit=TRUE;
		else payroll->paymts_audit=FALSE;
	if(version)
	{
		test=BINgetshort(bin);
		if(test) payroll->subsel_audit=TRUE;
			else payroll->subsel_audit=FALSE;
	} else payroll->subsel_audit=TRUE;
	payroll->direct_deposit_type=BINgetint(bin);
	test=BINgetshort(bin);
	if(test) payroll->ein=BINgetstring(bin);
		else payroll->ein=NULL;
	if(version>1)
	{
		test=BINgetshort(bin);
		if(test) payroll->payjams_rollover=TRUE;
			else payroll->payjams_rollover=FALSE;
	} else payroll->payjams_rollover=FALSE;
	if(version>3)
	{
		payroll->perid=BINgetint(bin);
		payroll->perid_resource=BINgetint(bin);
		payroll->paycheck_form_type=BINgetint(bin);
		payroll->dirdep_form_type=BINgetint(bin);
	} else {
		payroll->perid=0;
		payroll->perid_resource=0;
		payroll->paycheck_form_type=0;
		payroll->dirdep_form_type=0;
	}
	BINfree(bin);
	if(libname!=NULL) Rfree(libname);
	return(0);
}
short xwritepayrollbin(RDApayroll *payroll,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/PAYROLL.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PAYROLL.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG writepayrollbin Writing Payroll Setup Variables to Library [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1696);	/*type identifier*/
	BINaddshort(bin,(payroll->manage_earnings ? TRUE:FALSE));
	BINaddshort(bin,payroll->use_payroll_fund);
	BINaddshort(bin,(payroll->paysel_audit ? TRUE:FALSE));
	BINaddshort(bin,(payroll->paynet_audit ? TRUE:FALSE));
	BINaddshort(bin,(payroll->paymts_audit ? TRUE:FALSE));
	BINaddshort(bin,(payroll->subsel_audit ? TRUE:FALSE));
	BINaddint(bin,payroll->direct_deposit_type);
	if(!isEMPTY(payroll->ein))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,payroll->ein);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,(payroll->payjams_rollover ? TRUE:FALSE));
	BINaddint(bin,payroll->perid);
	BINaddint(bin,payroll->perid_resource);
	BINaddint(bin,payroll->paycheck_form_type);
	BINaddint(bin,payroll->dirdep_form_type);
	if(writelibbin(libname,bin,"PAYROLL SETUP"))
	{
		prterr("Error Can't write Payroll Setup Variables in binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
RDApayroll *xRDApayrollNEW(int line,char *file)
{
	RDApayroll *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDApayrollNEW Creating a structure for the Fincancial Management Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDApayroll));
	tmp->manage_earnings=FALSE;
	tmp->use_payroll_fund=TRUE;
	tmp->paysel_audit=FALSE;
	tmp->paynet_audit=FALSE;
	tmp->paymts_audit=FALSE;
	tmp->subsel_audit=FALSE;
	tmp->direct_deposit_type=0;
	tmp->ein=NULL;
	tmp->payjams_rollover=FALSE;
	tmp->perid=0;
	tmp->perid_resource=0;
	tmp->paycheck_form_type=0;
	tmp->dirdep_form_type=0;
	return(tmp);
}
void xfree_payroll(RDApayroll *payroll,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG free_payroll freeing memory used for the Payroll Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(payroll!=NULL) 
	{
		if(payroll->ein!=NULL) Rfree(payroll->ein);
		Rfree(payroll);
	}
}
short xGetPayrollSetup(RDApayroll *paysetup,void (*qfunc)(void *),void *arg,int line,
	char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GetPayrollSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(getpayrollbin(paysetup)==(-1))
	{
		ERRORDIALOG("Payroll Setup Not Available","The Payroll Setup File cannot opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		if(qfunc!=NULL) qfunc(arg);
		return(-1);
	}
	return(0);
}
