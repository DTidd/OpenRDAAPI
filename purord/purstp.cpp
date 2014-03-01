/* purstp.c - Purchase Orders Setup Variables */
#ifndef WIN32
#define __NAM__ "purstp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purstp.exe"
#endif
#include <app.hpp>




#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <purord.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


static void quitsetup(RDArsrc *parent,RDApurord *purord)
{
	if(parent!=NULL) free_rsrc(parent); 
	if(purord!=NULL) free_purord(purord);
}
static void savesetup(RDArsrc *parent,RDApurord *purord)
{
	readallwidgets(parent);
/*
	FINDRSCGETCHAR(parent,"USE TAXES",&purord->Use_Taxes);
	FINDRSCGETSTRING(parent,"ADMIN FEE",&purord->admin_fee);
	if(purord->Use_Taxes)
	{
		FINDRSCGETCHAR(parent,"USE TAX1",&purord->Use_Tax1);
		if(purord->Use_Tax1)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX1 VENDOR",&purord->Sep_Tax1_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX1 VENDOR",&purord->Default_Tax1_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX1 ADDRESS",&purord->Default_Tax1_Address);
			FINDRSCGETDOUBLE(parent,"TAX1 RATE",&purord->Tax1_Rate);
		} else {
			purord->Sep_Tax1_Vendor=FALSE;
			if(purord->Default_Tax1_Vendor) Rfree(purord->Default_Tax1_Vendor);
			if(purord->Default_Tax1_Address) Rfree(purord->Default_Tax1_Address);
			purord->Tax1_Rate=0.0;
		}
		FINDRSCGETCHAR(parent,"USE TAX2",&purord->Use_Tax2);
		if(purord->Use_Tax2)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX2 VENDOR",&purord->Sep_Tax2_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX2 VENDOR",&purord->Default_Tax2_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX2 ADDRESS",&purord->Default_Tax2_Address);
			FINDRSCGETDOUBLE(parent,"TAX2 RATE",&purord->Tax2_Rate);
		} else {
			purord->Sep_Tax2_Vendor=FALSE;
			if(purord->Default_Tax2_Vendor) Rfree(purord->Default_Tax2_Vendor);
			if(purord->Default_Tax2_Address) Rfree(purord->Default_Tax2_Address);
			purord->Tax2_Rate=0.0;
		}
		FINDRSCGETCHAR(parent,"USE TAX3",&purord->Use_Tax3);
		if(purord->Use_Tax3)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX3 VENDOR",&purord->Sep_Tax3_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX3 VENDOR",&purord->Default_Tax3_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX3 ADDRESS",&purord->Default_Tax3_Address);
			FINDRSCGETDOUBLE(parent,"TAX3 RATE",&purord->Tax3_Rate);
		} else {
			purord->Sep_Tax3_Vendor=FALSE;
			if(purord->Default_Tax3_Vendor) Rfree(purord->Default_Tax3_Vendor);
			if(purord->Default_Tax3_Address) Rfree(purord->Default_Tax3_Address);
			purord->Tax3_Rate=0.0;
		}
		FINDRSCGETCHAR(parent,"USE TAX4",&purord->Use_Tax4);
		if(purord->Use_Tax4)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX4 VENDOR",&purord->Sep_Tax4_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX4 VENDOR",&purord->Default_Tax4_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX4 ADDRESS",&purord->Default_Tax4_Address);
			FINDRSCGETDOUBLE(parent,"TAX4 RATE",&purord->Tax4_Rate);
		} else {
			purord->Sep_Tax4_Vendor=FALSE;
			if(purord->Default_Tax4_Vendor) Rfree(purord->Default_Tax4_Vendor);
			if(purord->Default_Tax4_Address) Rfree(purord->Default_Tax4_Address);
			purord->Tax4_Rate=0.0;
		}
		FINDRSCGETCHAR(parent,"USE TAX5",&purord->Use_Tax5);
		if(purord->Use_Tax5)
		{
			FINDRSCGETCHAR(parent,"SEPERATE TAX5 VENDOR",&purord->Sep_Tax5_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX5 VENDOR",&purord->Default_Tax5_Vendor);
			FINDRSCGETSTRING(parent,"DEFAULT TAX5 ADDRESS",&purord->Default_Tax5_Address);
			FINDRSCGETDOUBLE(parent,"TAX5 RATE",&purord->Tax5_Rate);
		} else {
			purord->Sep_Tax5_Vendor=FALSE;
			if(purord->Default_Tax5_Vendor) Rfree(purord->Default_Tax5_Vendor);
			if(purord->Default_Tax5_Address) Rfree(purord->Default_Tax5_Address);
			purord->Tax5_Rate=0.0;
		}
	} else {
		purord->Use_Tax1=FALSE;
		purord->Sep_Tax1_Vendor=FALSE;
		if(purord->Default_Tax1_Vendor) Rfree(purord->Default_Tax1_Vendor);
		if(purord->Default_Tax1_Address) Rfree(purord->Default_Tax1_Address);
		purord->Tax1_Rate=0.0;
		purord->Use_Tax2=FALSE;
		purord->Sep_Tax2_Vendor=FALSE;
		if(purord->Default_Tax2_Vendor) Rfree(purord->Default_Tax2_Vendor);
		if(purord->Default_Tax2_Address) Rfree(purord->Default_Tax2_Address);
		purord->Tax2_Rate=0.0;
		purord->Use_Tax3=FALSE;
		purord->Sep_Tax3_Vendor=FALSE;
		if(purord->Default_Tax3_Vendor) Rfree(purord->Default_Tax3_Vendor);
		if(purord->Default_Tax3_Address) Rfree(purord->Default_Tax3_Address);
		purord->Tax3_Rate=0.0;
		purord->Use_Tax4=FALSE;
		purord->Sep_Tax4_Vendor=FALSE;
		if(purord->Default_Tax4_Vendor) Rfree(purord->Default_Tax4_Vendor);
		if(purord->Default_Tax4_Address) Rfree(purord->Default_Tax4_Address);
		purord->Tax4_Rate=0.0;
		purord->Use_Tax5=FALSE;
		purord->Sep_Tax5_Vendor=FALSE;
		if(purord->Default_Tax5_Vendor) Rfree(purord->Default_Tax5_Vendor);
		if(purord->Default_Tax5_Address) Rfree(purord->Default_Tax5_Address);
		purord->Tax5_Rate=0.0;
	}
*/
	FINDRSCGETCHAR(parent,"USE FISCAL MONTHS",&purord->use_fiscal_months);
	writepurordbin(purord);
	quitsetup(parent,purord);
}
/*
static void use_taxes(RDArsrc *r)
{
	char utax=FALSE;

	readwidget(r,"USE TAXES");
	FINDRSCGETCHAR(r,"USE TAXES",&utax);
	if(utax==TRUE)
	{
		FINDRSCSETEDITABLE(r,"USE TAX1",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX1 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX1 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX1 ADDRESS",TRUE);
		FINDRSCSETEDITABLE(r,"TAX1 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"USE TAX2",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX2 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX2 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX2 ADDRESS",TRUE);
		FINDRSCSETEDITABLE(r,"TAX2 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"USE TAX3",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX3 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX3 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX3 ADDRESS",TRUE);
		FINDRSCSETEDITABLE(r,"TAX3 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"USE TAX4",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX4 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX4 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX4 ADDRESS",TRUE);
		FINDRSCSETEDITABLE(r,"TAX4 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"USE TAX5",TRUE);
		FINDRSCSETEDITABLE(r,"SEPERATE TAX5 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX5 VENDOR",TRUE);
		FINDRSCSETEDITABLE(r,"DEFAULT TAX5 ADDRESS",TRUE);
		FINDRSCSETEDITABLE(r,"TAX5 RATE",TRUE);
		FINDRSCSETEDITABLE(r,"ADMIN FEE",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX1",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX1 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX1 ADDRESS",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX1 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX2",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX2 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX2 ADDRESS",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX2 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX3",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX3 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX3 ADDRESS",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX3 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX4",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX4 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX4 ADDRESS",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX4 RATE",TRUE);
		FINDRSCSETSENSITIVE(r,"USE TAX5",TRUE);
		FINDRSCSETSENSITIVE(r,"SEPERATE TAX5 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 VENDOR",TRUE);
		FINDRSCSETSENSITIVE(r,"DEFAULT TAX5 ADDRESS",TRUE);
		FINDRSCSETSENSITIVE(r,"TAX5 RATE",TRUE);
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
*/
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDApurord *purord=NULL;
	RDArsrc *parent=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"PURORD","PURORD SETUP")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	purord=RDApurordNEW();
	getpurordbin(purord);
	parent=RDArsrcNEW("PURORD","PURORD SETUP");
/*
	addstdrsrc(parent,"USE TAXES",BOOLNS,1,&purord->Use_Taxes,TRUE);
	FINDRSCSETFUNC(parent,"USE TAXES",use_taxes,NULL);
	addstdrsrc(parent,"USE TAX1",BOOLNS,1,&purord->Use_Tax1,TRUE);
	addstdrsrc(parent,"SEPERATE TAX1 VENDOR",BOOLNS,1,&purord->Sep_Tax1_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX1 VENDOR",VARIABLETEXT,0,purord->Default_Tax1_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX1 ADDRESS",VARIABLETEXT,0,purord->Default_Tax1_Address,TRUE);
	addstdrsrc(parent,"TAX1 RATE",SDECIMALV,10,&purord->Tax1_Rate,TRUE);
	addstdrsrc(parent,"USE TAX2",BOOLNS,1,&purord->Use_Tax2,TRUE);
	addstdrsrc(parent,"SEPERATE TAX2 VENDOR",BOOLNS,1,&purord->Sep_Tax2_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX2 VENDOR",VARIABLETEXT,0,purord->Default_Tax2_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX2 ADDRESS",VARIABLETEXT,0,purord->Default_Tax2_Address,TRUE);
	addstdrsrc(parent,"TAX2 RATE",SDECIMALV,10,&purord->Tax2_Rate,TRUE);
	addstdrsrc(parent,"USE TAX3",BOOLNS,1,&purord->Use_Tax3,TRUE);
	addstdrsrc(parent,"SEPERATE TAX3 VENDOR",BOOLNS,1,&purord->Sep_Tax3_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX3 VENDOR",VARIABLETEXT,0,purord->Default_Tax3_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX3 ADDRESS",VARIABLETEXT,0,purord->Default_Tax3_Address,TRUE);
	addstdrsrc(parent,"TAX3 RATE",SDECIMALV,10,&purord->Tax3_Rate,TRUE);
	addstdrsrc(parent,"USE TAX4",BOOLNS,1,&purord->Use_Tax4,TRUE);
	addstdrsrc(parent,"SEPERATE TAX4 VENDOR",BOOLNS,1,&purord->Sep_Tax4_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX4 VENDOR",VARIABLETEXT,0,purord->Default_Tax4_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX4 ADDRESS",VARIABLETEXT,0,purord->Default_Tax4_Address,TRUE);
	addstdrsrc(parent,"TAX4 RATE",SDECIMALV,10,&purord->Tax5_Rate,TRUE);
	addstdrsrc(parent,"USE TAX5",BOOLNS,1,&purord->Use_Tax5,TRUE);
	addstdrsrc(parent,"SEPERATE TAX5 VENDOR",BOOLNS,1,&purord->Sep_Tax5_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX5 VENDOR",VARIABLETEXT,0,purord->Default_Tax5_Vendor,TRUE);
	addstdrsrc(parent,"DEFAULT TAX5 ADDRESS",VARIABLETEXT,0,purord->Default_Tax5_Address,TRUE);
	addstdrsrc(parent,"TAX5 RATE",SDECIMALV,10,&purord->Tax5_Rate,TRUE);
	addsctrsrc(parent,"ADMIN FEE",0,purord->admin_fee,TRUE);

	use_taxes(parent);
*/
	addstdrsrc(parent,"USE FISCAL MONTHS",BOOLNS,1,&purord->use_fiscal_months,TRUE);
	addrfexrsrc(parent,"SAVE",TRUE,savesetup,purord);
	addrfexrsrc(parent,"QUIT",TRUE,quitsetup,purord);
	addbtnrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(parent,TRUE,quitsetup,purord,TRUE);
	RDAAPPMAINLOOP();
}
