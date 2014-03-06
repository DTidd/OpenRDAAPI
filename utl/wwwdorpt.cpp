/* doreport.c - Do Report */
#ifndef WIN32
#define __NAM__ "wwwdorpt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "wwwdorpt.exe"
#endif
#include <app.hpp>

#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
/*CPP_OPART wwwdo1 */
/*ADDLIB curl */
/*ADDLIB rpt */
/*ADDLIB add */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void WWWprocess_dataline(RDArunrpt *,RDAreport *,RDAimage *,short);

short WWWInitializeSubsystems(int argc,char **argv,char *module,char *process)
{
	char *desc=NULL;
	short return_value=0;
	long longtemp=0;

	initrdadiag();
	SETUSERLOGIN();
	desc=RDA_GetEnv("RDA_CURRENCY_SYMBOL");
	if(!isEMPTY(desc))
	{
		longtemp=strtol(desc,NULL,0);
		RDA_CURRENCY_SYMBOL=(char)longtemp;
	} else {
		RDA_CURRENCY_SYMBOL='$';
	}
#ifdef WIN32
	Pay_Balance_of_Contract=FALSE;
	Dump_Deferred_Pay_Balance=FALSE;
#endif
	RDA_NOGUI=TRUE;
	SECURITY_USERLOGIN=USERLOGIN;
	INITGUI(argc,argv,CURRENTDIRECTORY);
	SETCWD();
	PP_translate_GUIFUNC=PP_translate_GUI;
	PP_translate_GUIFUNCALL=PP_translate_GUIALL;
	XPERT_SETUP=XPERTstpNEW();
	if((getXPERTbin(XPERT_SETUP))==(-1))
	{
		prterr("ERROR: Error Invalid Version or Bad Binary for [XPERT SETUP] from Module: [%s] Process: [%s].",module,process);
		return_value=(-1);
	}
	if(INITNRD()) 
	{
		prterr("ERROR: Cannot Initialize the Database Subsystem.");
		return_value=(-1);
	} else if(DoInitializeSecurity()) 
	{
		prterr("Error Cannot Initialize the Security Subsystem.");
		return_value=(-1);
	} else if(!DoPROCsecurity(module,process)) 
	{
		prterr("ERROR: Security Access to Module [%s] Process [%s] DENIED!",module,process);
		desc=Rmalloc(RDAstrlen(module)+RDAstrlen(process)+111);
		sprintf(desc,"The Security SubSystem denied you access to Module [%s] Process [%s].\nPlease call your installer for support.",module,process);
		Rfree(desc);
		return_value=(-1);
	} else {
		INITTRANS();
		OPNLOG(module,process);
	}
	return(return_value);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(argc==3)
	{
		RDA_NOGUI=TRUE;
#ifdef LINUX 
		umask(002);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS) 
		umask(002);
#endif
		fprintf(stdout,"\n\n");
		if(WWWInitializeSubsystems(argc,argv,argv[1],argv[2])) 
		{
			ShutdownSubsystems();
                        exit(1);
		}
		RDA_NOGUI=TRUE;
		RUNREPORTADV3(argv[1],argv[2],NULL,NULL,FALSE,TRUE,NULL,NULL,NULL,NULL,0,(void (*)(...))WWWprocess_dataline);
		ShutdownSubsystems();
		std::exit;
	} else {
		exit(1);
	}
}
