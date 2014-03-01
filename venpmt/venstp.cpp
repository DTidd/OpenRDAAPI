/* venstp.c - Venpmt Setup Variables */
#ifndef WIN32
#define __NAM__ "venstp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "venstp.exe"
#endif
#include <app.hpp>




#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <venpmt.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void quitsetup(RDArsrc *parent,RDAvenpmt *venpmt)
{
	if(parent!=NULL) free_rsrc(parent); 
	if(venpmt!=NULL) free_venpmt(venpmt); 
	ShutdownSubsystems();
}
static void use_taxes(RDArsrc *r)
{
	char utax=FALSE,sv=FALSE;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"USE TAXES",&utax);
	if(utax==TRUE)
	{
		FINDRSCSETEDITABLE(r,"USE TAX1",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX1",TRUE);
		FINDRSCSETEDITABLE(r,"TAX1 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX1 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX1 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX1 VENDOR",TRUE);
		FINDRSCGETCHAR(r,"SEPERATE TAX1 VENDOR",&sv);
		if(sv==TRUE)
		{
			FINDRSCSETEDITABLE(r,"DEFAULT TAX1 VENDOR",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 VENDOR",TRUE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX1 ADDRESS",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 ADDRESS",TRUE);
		} else {
			FINDRSCSETEDITABLE(r,"DEFAULT TAX1 VENDOR",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 VENDOR",FALSE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX1 ADDRESS",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 ADDRESS",FALSE);
		}

		FINDRSCSETEDITABLE(r,"USE TAX2",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX2",TRUE);
		FINDRSCSETEDITABLE(r,"TAX2 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX2 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX2 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX2 VENDOR",TRUE);
		FINDRSCGETCHAR(r,"SEPERATE TAX2 VENDOR",&sv);
		if(sv==TRUE)
		{
			FINDRSCSETEDITABLE(r,"DEFAULT TAX2 VENDOR",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 VENDOR",TRUE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX2 ADDRESS",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 ADDRESS",TRUE);
		} else {
			FINDRSCSETEDITABLE(r,"DEFAULT TAX2 VENDOR",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 VENDOR",FALSE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX2 ADDRESS",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 ADDRESS",FALSE);
		}

		FINDRSCSETEDITABLE(r,"USE TAX3",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX3",TRUE);
		FINDRSCSETEDITABLE(r,"TAX3 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX3 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX3 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX3 VENDOR",TRUE);
		FINDRSCGETCHAR(r,"SEPERATE TAX3 VENDOR",&sv);
		if(sv==TRUE)
		{
			FINDRSCSETEDITABLE(r,"DEFAULT TAX3 VENDOR",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 VENDOR",TRUE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX3 ADDRESS",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 ADDRESS",TRUE);
		} else {
			FINDRSCSETEDITABLE(r,"DEFAULT TAX3 VENDOR",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 VENDOR",FALSE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX3 ADDRESS",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 ADDRESS",FALSE);
		}

		FINDRSCSETEDITABLE(r,"USE TAX4",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX4",TRUE);
		FINDRSCSETEDITABLE(r,"TAX4 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX4 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX4 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX4 VENDOR",TRUE);
		FINDRSCGETCHAR(r,"SEPERATE TAX4 VENDOR",&sv);
		if(sv==TRUE)
		{
			FINDRSCSETEDITABLE(r,"DEFAULT TAX4 VENDOR",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 VENDOR",TRUE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX4 ADDRESS",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 ADDRESS",TRUE);
		} else {
			FINDRSCSETEDITABLE(r,"DEFAULT TAX4 VENDOR",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 VENDOR",FALSE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX4 ADDRESS",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 ADDRESS",FALSE);
		}

		FINDRSCSETEDITABLE(r,"USE TAX5",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX5",TRUE);
		FINDRSCSETEDITABLE(r,"TAX5 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX5 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX5 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX5 VENDOR",TRUE);
		FINDRSCGETCHAR(r,"SEPERATE TAX5 VENDOR",&sv);
		if(sv==TRUE)
		{
			FINDRSCSETEDITABLE(r,"DEFAULT TAX5 VENDOR",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 VENDOR",TRUE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX5 ADDRESS",TRUE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 ADDRESS",TRUE);
		} else {
			FINDRSCSETEDITABLE(r,"DEFAULT TAX5 VENDOR",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 VENDOR",FALSE);
			FINDRSCSETEDITABLE(r,"DEFAULT TAX5 ADDRESS",FALSE);
			FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 ADDRESS",FALSE);
		}

		FINDRSCSETEDITABLE(r,"ADMIN FEE",TRUE);
		FINDRSCSETSENSITIVE(r,"ADMIN FEE",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"USE TAX1",FALSE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX1 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX1 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX1 ADDRESS",FALSE);
		FINDRSCSETEDITABLE(r,"TAX1 RATE",FALSE);
		FINDRSCSETEDITABLE(r,"USE TAX2",FALSE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX2 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX2 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX2 ADDRESS",FALSE);
		FINDRSCSETEDITABLE(r,"TAX2 RATE",FALSE);
		FINDRSCSETEDITABLE(r,"USE TAX3",FALSE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX3 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX3 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX3 ADDRESS",FALSE);
		FINDRSCSETEDITABLE(r,"TAX3 RATE",FALSE);
		FINDRSCSETEDITABLE(r,"USE TAX4",FALSE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX4 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX4 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX4 ADDRESS",FALSE);
		FINDRSCSETEDITABLE(r,"TAX4 RATE",FALSE);
		FINDRSCSETEDITABLE(r,"USE TAX5",FALSE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX5 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX5 VENDOR",FALSE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX5 ADDRESS",FALSE);
		FINDRSCSETEDITABLE(r,"TAX5 RATE",FALSE);
		FINDRSCSETEDITABLE(r,"ADMIN FEE",FALSE);
		FINDRSCSETSENSITIVE(r,"USE TAX1",FALSE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX1 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 ADDRESS",FALSE);
		FINDRSCSETSENSITIVE(r,"TAX1 RATE",FALSE);
		FINDRSCSETSENSITIVE(r,"USE TAX2",FALSE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX2 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 ADDRESS",FALSE);
		FINDRSCSETSENSITIVE(r,"TAX2 RATE",FALSE);
		FINDRSCSETSENSITIVE(r,"USE TAX3",FALSE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX3 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 ADDRESS",FALSE);
		FINDRSCSETSENSITIVE(r,"TAX3 RATE",FALSE);
		FINDRSCSETSENSITIVE(r,"USE TAX4",FALSE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX4 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 ADDRESS",FALSE);
		FINDRSCSETSENSITIVE(r,"TAX4 RATE",FALSE);
		FINDRSCSETSENSITIVE(r,"USE TAX5",FALSE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX5 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 VENDOR",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 ADDRESS",FALSE);
		FINDRSCSETSENSITIVE(r,"TAX5 RATE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADMIN FEE",FALSE);
	}
}
static void savesetup(RDArsrc *parent,RDAvenpmt *venpmt)
{
	readallwidgets(parent);
	FINDRSCGETCHAR(parent,"USE VENPMT FUND",&venpmt->use_venpmt_fund);
	FINDRSCGETSTRING(parent,"EIN NUMBER",&venpmt->ein);
	FINDRSCGETCHAR(parent,"USE FISCAL SUMMARY",&venpmt->use_fiscal_summary);
	FINDRSCGETCHAR(parent,"USE TAXES",&venpmt->Use_Taxes);
	FINDRSCGETSTRING(parent,"ADMIN FEE",&venpmt->admin_fee);
	if(venpmt->Use_Taxes)
	{
		FINDRSCGETCHAR(parent,"USE TAX1",&venpmt->Use_Tax1);
		if(venpmt->Use_Tax1)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX1 VENDOR",&venpmt->Sep_Tax1_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX1 VENDOR",&venpmt->Default_Tax1_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX1 ADDRESS",&venpmt->Default_Tax1_Address);
			FINDRSCGETDOUBLE(parent,"TAX1 RATE",&venpmt->Tax1_Rate);
		} else {
			venpmt->Sep_Tax1_Vendor=FALSE;
			if(venpmt->Default_Tax1_Vendor) Rfree(venpmt->Default_Tax1_Vendor);
			if(venpmt->Default_Tax1_Address) Rfree(venpmt->Default_Tax1_Address);
			venpmt->Tax1_Rate=0.0;
		}

		FINDRSCGETCHAR(parent,"USE TAX2",&venpmt->Use_Tax2);
		if(venpmt->Use_Tax2)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX2 VENDOR",&venpmt->Sep_Tax2_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX2 VENDOR",&venpmt->Default_Tax2_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX2 ADDRESS",&venpmt->Default_Tax2_Address);
			FINDRSCGETDOUBLE(parent,"TAX2 RATE",&venpmt->Tax2_Rate);
		} else {
			venpmt->Sep_Tax2_Vendor=FALSE;
			if(venpmt->Default_Tax2_Vendor) Rfree(venpmt->Default_Tax2_Vendor);
			if(venpmt->Default_Tax2_Address) Rfree(venpmt->Default_Tax2_Address);
			venpmt->Tax2_Rate=0.0;
		}

		FINDRSCGETCHAR(parent,"USE TAX3",&venpmt->Use_Tax3);
		if(venpmt->Use_Tax3)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX3 VENDOR",&venpmt->Sep_Tax3_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX3 VENDOR",&venpmt->Default_Tax3_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX3 ADDRESS",&venpmt->Default_Tax3_Address);
			FINDRSCGETDOUBLE(parent,"TAX3 RATE",&venpmt->Tax3_Rate);
		} else {
			venpmt->Sep_Tax3_Vendor=FALSE;
			if(venpmt->Default_Tax3_Vendor) Rfree(venpmt->Default_Tax3_Vendor);
			if(venpmt->Default_Tax3_Address) Rfree(venpmt->Default_Tax3_Address);
			venpmt->Tax3_Rate=0.0;
		}

		FINDRSCGETCHAR(parent,"USE TAX4",&venpmt->Use_Tax4);
		if(venpmt->Use_Tax4)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX4 VENDOR",&venpmt->Sep_Tax4_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX4 VENDOR",&venpmt->Default_Tax4_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX4 ADDRESS",&venpmt->Default_Tax4_Address);
			FINDRSCGETDOUBLE(parent,"TAX4 RATE",&venpmt->Tax4_Rate);
		} else {
			venpmt->Sep_Tax4_Vendor=FALSE;
			if(venpmt->Default_Tax4_Vendor) Rfree(venpmt->Default_Tax4_Vendor);
			if(venpmt->Default_Tax4_Address) Rfree(venpmt->Default_Tax4_Address);
			venpmt->Tax4_Rate=0.0;
		}

		FINDRSCGETCHAR(parent,"USE TAX5",&venpmt->Use_Tax5);
		if(venpmt->Use_Tax5)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX5 VENDOR",&venpmt->Sep_Tax5_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX5 VENDOR",&venpmt->Default_Tax5_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX5 ADDRESS",&venpmt->Default_Tax5_Address);
			FINDRSCGETDOUBLE(parent,"TAX5 RATE",&venpmt->Tax5_Rate);
		} else {
			venpmt->Sep_Tax5_Vendor=FALSE;
			if(venpmt->Default_Tax5_Vendor) Rfree(venpmt->Default_Tax5_Vendor);
			if(venpmt->Default_Tax5_Address) Rfree(venpmt->Default_Tax5_Address);
			venpmt->Tax5_Rate=0.0;
		}
	} else {
		venpmt->Use_Tax1=FALSE;
		venpmt->Sep_Tax1_Vendor=FALSE;
		if(venpmt->Default_Tax1_Vendor) Rfree(venpmt->Default_Tax1_Vendor);
		if(venpmt->Default_Tax1_Address) Rfree(venpmt->Default_Tax1_Address);
		venpmt->Tax1_Rate=0.0;
		venpmt->Use_Tax2=FALSE;
		venpmt->Sep_Tax2_Vendor=FALSE;
		if(venpmt->Default_Tax2_Vendor) Rfree(venpmt->Default_Tax2_Vendor);
		if(venpmt->Default_Tax2_Address) Rfree(venpmt->Default_Tax2_Address);
		venpmt->Tax2_Rate=0.0;
		venpmt->Use_Tax3=FALSE;
		venpmt->Sep_Tax3_Vendor=FALSE;
		if(venpmt->Default_Tax3_Vendor) Rfree(venpmt->Default_Tax3_Vendor);
		if(venpmt->Default_Tax3_Address) Rfree(venpmt->Default_Tax3_Address);
		venpmt->Tax3_Rate=0.0;
		venpmt->Use_Tax4=FALSE;
		venpmt->Sep_Tax4_Vendor=FALSE;
		if(venpmt->Default_Tax4_Vendor) Rfree(venpmt->Default_Tax4_Vendor);
		if(venpmt->Default_Tax4_Address) Rfree(venpmt->Default_Tax4_Address);
		venpmt->Tax4_Rate=0.0;
		venpmt->Use_Tax5=FALSE;
		venpmt->Sep_Tax5_Vendor=FALSE;
		if(venpmt->Default_Tax5_Vendor) Rfree(venpmt->Default_Tax5_Vendor);
		if(venpmt->Default_Tax5_Address) Rfree(venpmt->Default_Tax5_Address);
		venpmt->Tax5_Rate=0.0;
	}
	FINDRSCGETCHAR(parent,"USE FISCAL MONTHS",&venpmt->use_fiscal_months);
	writevenpmtbin(venpmt);
	quitsetup(parent,venpmt);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDAvenpmt *venpmt=NULL;
	RDArsrc *parent=NULL;

	if(InitializeSubsystems(argc,argv,"VENPMT","VENPMT SETUP")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	venpmt=RDAvenpmtNEW();
	getvenpmtbin(venpmt);
	parent=RDArsrcNEW("VENPMT","VENPMT SETUP");
	addstdrsrc(parent,"USE VENPMT FUND",BOOLNS,1,&venpmt->use_venpmt_fund,TRUE);
	addstdrsrc(parent,"EIN NUMBER",VARIABLETEXT,0,venpmt->ein,TRUE);
	addstdrsrc(parent,"USE FISCAL SUMMARY",BOOLNS,1,&venpmt->use_fiscal_summary,TRUE);

	addstdrsrc(parent,"USE TAXES",BOOLNS,1,&venpmt->Use_Taxes,TRUE);

	addstdrsrc(parent,"USE TAX1",BOOLNS,1,&venpmt->Use_Tax1,TRUE);
	addstdrsrc(parent,"SEPERATE TAX1 VENDOR",BOOLNS,1,&venpmt->Sep_Tax1_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX1 VENDOR",VARIABLETEXT,0,venpmt->Default_Tax1_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX1 ADDRESS",VARIABLETEXT,0,venpmt->Default_Tax1_Address,TRUE);
	addstdrsrc(parent,"TAX1 RATE",SDECIMALV,10,&venpmt->Tax1_Rate,TRUE);
	addstdrsrc(parent,"USE TAX2",BOOLNS,1,&venpmt->Use_Tax2,TRUE);
	addstdrsrc(parent,"SEPERATE TAX2 VENDOR",BOOLNS,1,&venpmt->Sep_Tax2_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX2 VENDOR",VARIABLETEXT,0,venpmt->Default_Tax2_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX2 ADDRESS",VARIABLETEXT,0,venpmt->Default_Tax2_Address,TRUE);
	addstdrsrc(parent,"TAX2 RATE",SDECIMALV,10,&venpmt->Tax2_Rate,TRUE);

	addstdrsrc(parent,"USE TAX3",BOOLNS,1,&venpmt->Use_Tax3,TRUE);
	addstdrsrc(parent,"SEPERATE TAX3 VENDOR",BOOLNS,1,&venpmt->Sep_Tax3_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX3 VENDOR",VARIABLETEXT,0,venpmt->Default_Tax3_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX3 ADDRESS",VARIABLETEXT,0,venpmt->Default_Tax3_Address,TRUE);
	addstdrsrc(parent,"TAX3 RATE",SDECIMALV,10,&venpmt->Tax3_Rate,TRUE);

	addstdrsrc(parent,"USE TAX4",BOOLNS,1,&venpmt->Use_Tax4,TRUE);
	addstdrsrc(parent,"SEPERATE TAX4 VENDOR",BOOLNS,1,&venpmt->Sep_Tax4_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX4 VENDOR",VARIABLETEXT,0,venpmt->Default_Tax4_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX4 ADDRESS",VARIABLETEXT,0,venpmt->Default_Tax4_Address,TRUE);
	addstdrsrc(parent,"TAX4 RATE",SDECIMALV,10,&venpmt->Tax5_Rate,TRUE);

	addstdrsrc(parent,"USE TAX5",BOOLNS,1,&venpmt->Use_Tax5,TRUE);
	addstdrsrc(parent,"SEPERATE TAX5 VENDOR",BOOLNS,1,&venpmt->Sep_Tax5_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX5 VENDOR",VARIABLETEXT,0,venpmt->Default_Tax5_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX5 ADDRESS",VARIABLETEXT,0,venpmt->Default_Tax5_Address,TRUE);
	addstdrsrc(parent,"TAX5 RATE",SDECIMALV,10,&venpmt->Tax5_Rate,TRUE);

	addsctrsrc(parent,"ADMIN FEE",0,venpmt->admin_fee,TRUE);
	addstdrsrc(parent,"USE FISCAL MONTHS",BOOLNS,1,&venpmt->use_fiscal_months,TRUE);

	FINDRSCSETFUNC(parent,"USE TAXES",use_taxes,NULL);
	FINDRSCSETFUNC(parent,"SEPERATE TAX1 VENDOR",use_taxes,NULL);
	FINDRSCSETFUNC(parent,"SEPERATE TAX2 VENDOR",use_taxes,NULL);
	FINDRSCSETFUNC(parent,"SEPERATE TAX3 VENDOR",use_taxes,NULL);
	FINDRSCSETFUNC(parent,"SEPERATE TAX4 VENDOR",use_taxes,NULL);
	FINDRSCSETFUNC(parent,"SEPERATE TAX5 VENDOR",use_taxes,NULL);
	use_taxes(parent);
	addrfexrsrc(parent,"SAVE",TRUE,savesetup,venpmt);
	addrfexrsrc(parent,"QUIT",TRUE,quitsetup,venpmt);
	addbtnrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(parent,TRUE,quitsetup,venpmt,TRUE);
	RDAAPPMAINLOOP();
}
