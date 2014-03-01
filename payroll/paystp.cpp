/* paystp.c - Payroll Setup Variables */
#ifndef WIN32
#define __NAM__ "paystp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paystp.exe"
#endif
#include <app.hpp>




#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <pay.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

APPlib *cmpmtd=NULL,*fund_types=NULL;
APPlib *perids=NULL,*peridtypes=NULL,*paychecks=NULL,*dirdeps=NULL;

static void quitsetup(RDArsrc *parent,RDApayroll *payroll)
{
	if(parent!=NULL) free_rsrc(parent); 
	if(payroll!=NULL) free_payroll(payroll); 
	if(cmpmtd!=NULL) freeapplib(cmpmtd);
	if(fund_types!=NULL) freeapplib(fund_types);
	if(perids!=NULL) freeapplib(perids);
	if(peridtypes!=NULL) freeapplib(peridtypes);
	if(paychecks!=NULL) freeapplib(paychecks);
	if(dirdeps!=NULL) freeapplib(dirdeps);
	ShutdownSubsystems();
}
static void savesetup(RDArsrc *parent,RDApayroll *payroll)
{
	int x=0; 

	readallwidgets(parent);
	FINDRSCGETCHAR(parent,"PAYSEL AUDIT",&payroll->paysel_audit);
	FINDRSCGETCHAR(parent,"PAYNET AUDIT",&payroll->paynet_audit);
	FINDRSCGETCHAR(parent,"PAYMTS AUDIT",&payroll->paymts_audit);
	FINDRSCGETCHAR(parent,"SUBSEL AUDIT",&payroll->subsel_audit);
	FINDRSCGETCHAR(parent,"MANAGE EARNINGS",&payroll->manage_earnings);
	FINDRSCGETINT(parent,"USE PAYROLL FUNDS",&x);
	payroll->use_payroll_fund=(short)x;
	FINDRSCGETINT(parent,"PERSONNEL IDENTIFICATION TYPES",&payroll->perid);
	FINDRSCGETINT(parent,"PERSONNEL IDENTIFICATION RESOURCES",&payroll->perid_resource);
	FINDRSCGETINT(parent,"PAYCHECK FORM TYPES",&payroll->paycheck_form_type);
	FINDRSCGETINT(parent,"DIRECT DEPOSIT FORM TYPES",&payroll->dirdep_form_type);
	FINDRSCGETCHAR(parent,"PAYJAMS ROLLOVER",&payroll->payjams_rollover);
	FINDRSCGETINT(parent,"DIRECT DEPOSIT TYPES",&payroll->direct_deposit_type);
	FINDRSCGETSTRING(parent,"EIN NUMBER",&payroll->ein);
	writepayrollbin(payroll);
	quitsetup(parent,payroll);
}
static void changeperids(RDArsrc *r)
{
	int s=0;

	FINDRSCGETINT(r,"PERSONNEL IDENTIFICATION TYPES",&s);
	if(s==2)
	{
		FINDRSCSETEDITABLE(r,"PERSONNEL IDENTIFICATION RESOURCES",TRUE);
		FINDRSCSETSENSITIVE(r,"PERSONNEL IDENTIFICATION RESOURCES",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"PERSONNEL IDENTIFICATION RESOURCES",FALSE);
		FINDRSCSETSENSITIVE(r,"PERSONNEL IDENTIFICATION RESOURCES",FALSE);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0,start=0;
	RDApayroll *payroll=NULL;
	RDArsrc *parent=NULL;
	RDAacct *acc=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"PAYROLL","PAYROLL SETUP")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	cmpmtd=APPlibNEW();
	addAPPlib(cmpmtd,"ACH Standard Detail Direct Deposits");
	addAPPlib(cmpmtd,"ACH non-Detail Direct Deposits");
	addAPPlib(cmpmtd,"Other Direct Deposits");
	fund_types=APPlibNEW();
	addAPPlib(fund_types,"Non-Recurring/Non-Clearing");
	addAPPlib(fund_types,"Payroll Recurring");
	addAPPlib(fund_types,"Payroll Clearing");
	payroll=RDApayrollNEW();
	getpayrollbin(payroll);
	start=payroll->direct_deposit_type;
	parent=RDArsrcNEW("PAYROLL","PAYROLL SETUP");
	addlstrsrc(parent,"DIRECT DEPOSIT TYPES",&start,TRUE,NULL,
		cmpmtd->numlibs,&cmpmtd->libs,NULL);
	start=(int)payroll->use_payroll_fund;
	addlstrsrc(parent,"USE PAYROLL FUNDS",&start,TRUE,NULL,
		fund_types->numlibs,&fund_types->libs,NULL);
	addstdrsrc(parent,"MANAGE EARNINGS",BOOLNS,1,&payroll->manage_earnings,TRUE);
	addstdrsrc(parent,"PAYSEL AUDIT",BOOLNS,1,&payroll->paysel_audit,TRUE);
	addstdrsrc(parent,"PAYNET AUDIT",BOOLNS,1,&payroll->paynet_audit,TRUE);
	addstdrsrc(parent,"PAYMTS AUDIT",BOOLNS,1,&payroll->paymts_audit,TRUE);
	addstdrsrc(parent,"SUBSEL AUDIT",BOOLNS,1,&payroll->subsel_audit,TRUE);
	addstdrsrc(parent,"EIN NUMBER",VARIABLETEXT,0,payroll->ein,TRUE);
	addstdrsrc(parent,"PAYJAMS ROLLOVER",BOOLNS,1,&payroll->payjams_rollover,TRUE);
	perids=APPlibNEW();
	addAPPlib(perids,"Plaintext-15");
	addAPPlib(perids,"Social Security Number-11");
	addAPPlib(perids,"Custom Input-15");
	addlstrsrc(parent,"PERSONNEL IDENTIFICATION TYPES",&payroll->perid,TRUE,changeperids,perids->numlibs,&perids->libs,NULL);
	peridtypes=APPlibNEW();

	if(CUSTOM_INPUTS_ACCOUNT!=NULL)
	{
		for(x=0,acc=CUSTOM_INPUTS_ACCOUNT->codetype;x<CUSTOM_INPUTS_ACCOUNT->num;++x,++acc)
		{
			addAPPlib(peridtypes,acc->name);
		}
	}
	if(peridtypes->numlibs<1) addAPPlib(peridtypes,"Not Available");

	addlstrsrc(parent,"PERSONNEL IDENTIFICATION RESOURCES",&payroll->perid_resource,TRUE,NULL,peridtypes->numlibs,&peridtypes->libs,NULL);
	changeperids(parent);
	paychecks=APPlibNEW();
	addAPPlib(paychecks,"Xpert Standard Paycheck");	
	addAPPlib(paychecks,"MicroBUDGET Standard Paycheck");	
	addAPPlib(paychecks,"Glendale's Paycheck");
	addAPPlib(paychecks,"Charlotte County's Paycheck");
	addAPPlib(paychecks,"Williamsburg's Paycheck");
	addlstrsrc(parent,"PAYCHECK FORM TYPES",&payroll->paycheck_form_type,TRUE,NULL,paychecks->numlibs,&paychecks->libs,NULL);
	dirdeps=APPlibNEW();
	addAPPlib(dirdeps,"Xpert's Standard-11");
	addAPPlib(dirdeps,"Xpert's Standard-5.5");
	addAPPlib(dirdeps,"Glendale's Direct Deposit");
	addAPPlib(dirdeps,"Smyth County's Direct Deposit");
	addlstrsrc(parent,"DIRECT DEPOSIT FORM TYPES",&payroll->dirdep_form_type,TRUE,NULL,dirdeps->numlibs,&dirdeps->libs,NULL);

	addrfexrsrc(parent,"SAVE",TRUE,savesetup,payroll);
	addrfexrsrc(parent,"QUIT",TRUE,quitsetup,payroll);
	addbtnrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(parent,TRUE,quitsetup,payroll,TRUE);
	RDAAPPMAINLOOP();
}
