/* appstp.c - Application Management Setup Variables */
#ifndef WIN32
#define __NAM__ "appstp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "appstp.exe"
#endif
#include <app.hpp>




#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <appmgt.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


static void quitsetup(RDArsrc *parent,RDAappmgt *appmgt)
{
	if(parent!=NULL) free_rsrc(parent); 
	if(appmgt!=NULL) free_appmgt(appmgt);
	ShutdownSubsystems();
}
static void savesetup(RDArsrc *parent,RDAappmgt *appmgt)
{
	readallwidgets(parent);
	FINDRSCGETCHAR(parent,"USE PERSONNEL INTERVIEW",
		&appmgt->use_personnel_interview);
	FINDRSCGETCHAR(parent,"USE ADMINISTRATIVE INTERVIEW",
		&appmgt->use_administrative_interview);
	writeappmgtbin(appmgt);
	quitsetup(parent,appmgt);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDAappmgt *appmgt=NULL;
	RDArsrc *parent=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"APPMGT","APPMGT SETUP")) 
	{
		RDAAPPMAINLOOP();
	}
	appmgt=RDAappmgtNEW();
	getappmgtbin(appmgt);
	parent=RDArsrcNEW("APPMGT","APPMGT SETUP");
	addstdrsrc(parent,"USE PERSONNEL INTERVIEW",BOOLNS,1,
		&appmgt->use_personnel_interview,TRUE);
	addstdrsrc(parent,"USE ADMINISTRATIVE INTERVIEW",BOOLNS,1,
		&appmgt->use_administrative_interview,TRUE);
	addrfexrsrc(parent,"SAVE",TRUE,savesetup,appmgt);
	addrfexrsrc(parent,"QUIT",TRUE,quitsetup,appmgt);
	addbtnrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(parent,TRUE,quitsetup,appmgt,TRUE);
	RDAAPPMAINLOOP();
}
