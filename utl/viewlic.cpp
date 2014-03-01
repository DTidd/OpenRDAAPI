/* RDA XPERT/MBGUI license functions */
#ifndef WIN32
#define __NAM__ "viewlic.lnx"
#endif
#ifdef WIN32
#define __NAM__ "viewlic.exe"
#endif
#include <app.hpp>
#include <rdalic.hpp>
#include <olh.hpp>
#include <mix.hpp>
#ifdef WIN32
#include <pay.hpp>
#endif /* WIN32 */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
static char *module="SECURITY";
/*
static void printfp(RDArsrc *,void (*)(...),RDArsrc *);
static void printheader(FILE *,int *,int *,char *,unsigned);
static void checkheader(FILE *,int *,int *,unsigned);
static RDAdefault *maindefaults=NULL;
static int changedvalues=FALSE;
static void quit_print_definition(RDArsrc *);
*/
void quitviewlic(RDArsrc *);
static APPlib *licenselist=NULL;

APPlib *RDA_load_licenses(void)
{
	RDAlicense *rdal=NULL;
	long x=0;
	char *temp=NULL;
	VF vf=NULL;
	BIN *bin=NULL;
	APPlib *licenselist=NULL;

	licenselist=APPlibNEW();
	vf=getRDAlicvfile();
	if(vf!=NULL)
	{
/*
		for(x=0;x<(XPERT_SETUP->NoUsers);++x)
		{
*/
		x=0;
		while(TRUE)
		{
			bin=VFread(vf,(long)(x+1));
			if(bin==NULL)
			{
				/* what to do if VFread returns unfound record number */
				break;
			}
			rdal=getRDAlicbin(bin);
			if(rdal!=NULL)
			{
				temp=Rmalloc(RDAstrlen(rdal->display)+20+51);
				sprintf(temp,"License Slot: [%5d]  Processes: [%5d]  License Key: [%s]",(x+1),rdal->process_count,(rdal->display!=NULL?rdal->display:""));
				addAPPlib(licenselist,temp);
			} else {
/*
				prterr("DIAG INITRDALIC ERROR: can't allocated binary memory at line [%d] program [%s].",line,file);
*/
			}
			++x;
		}
	}
	if(licenselist->numlibs<1)
	{
		addAPPlib(licenselist,"No Licenses Used");
	}
	return(licenselist);
}
/*
static void quit_print_definition(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print_definition Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}	
*/
void quitviewtest(RDArsrc *mainrsrc)
{
	readallwidgets(mainrsrc);
/*
	if(changedvalues)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this ACCOUNT DEFINITION.\nExiting now without saving will loose these changes.",saveviewlic,quitviewlic,FALSE,mainrsrc);
	} else {
		quitviewlic(mainrsrc);
	}
*/
	quitviewlic(mainrsrc);
}
void quitviewlic(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL)
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	if(licenselist!=NULL) freeapplib(licenselist);
	ShutdownSubsystems();
	std::exit;
}
void deletelic(RDArsrc *mainrsrc)
{
	int y=0;
	RDAlicense *rdal=NULL;
	long x=0;
	VF vf=NULL;
	BIN *bin=NULL;


	readwidget(mainrsrc,"LICENSES LIST");
	FINDRSCGETINT(mainrsrc,"LICENSES LIST",&y);
	x=y;
	vf=getRDAlicvfile();
	if(vf!=NULL)
	{
		bin=VFread(vf,(long)(x+1));
		if(bin==NULL)
		{
			if(bin!=NULL) BINfree(bin);
			return;
		}
		rdal=getRDAlicbin(bin);
		if(rdal!=NULL)
		{
			if(rdal->used==TRUE)
			{
				rdal->used=FALSE;
				if(rdal->display!=NULL) Rfree(rdal->display);
				rdal->display=stralloc("UNUSED");
/*
				VFdelrec(vf,(x+1));
*/
				bin=writeRDAlicbin(rdal);
				VFwrite(vf,(x+1),bin);
				Free_RDAlic(rdal);
				if(bin!=NULL) BINfree(bin);
			}
			if(rdal!=NULL) Free_RDAlic(rdal);
		}
		if(bin!=NULL) BINfree(bin);
	}
	if(bin!=NULL) BINfree(bin);
	if(licenselist!=NULL) freeapplib(licenselist);
	licenselist=RDA_load_licenses();
	if(!FINDRSCLISTAPPlib(mainrsrc,"LICENSES LIST",0,licenselist))
		updatersrc(mainrsrc,"LICENSES LIST");
}
#define ViewLicInitializeSubsystems(a,b,c,d)	xViewLicInitializeSubsystems(a,b,c,d,__LINE__,__FILE__)
short xViewLicInitializeSubsystems(int argc,char **argv,char *module,char *process,int line,char *file)
{
	char *desc=NULL;
	short x=0;
	short return_value=0;
	long longtemp=0;

	initrdadiag();
	if(diagmix || diagmix_field)
	{
		prterr("DIAG InitializeSubsystems Initializing Subsystems for Module [%s] Process [%s] at line [%d] program [%s].",module,process,line,file);
	}
	SETUSERLOGIN();
	SETCWD();
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
	INITGUI(argc,argv,CURRENTDIRECTORY);
	PP_translate_GUIFUNC=PP_translate_GUI;
	PP_translate_GUIFUNCALL=PP_translate_GUIALL;
	ADVINITHELP(GUIEVAL,addoswidgets);
	XPERT_SETUP=XPERTstpNEW();
	if((getXPERTbin(XPERT_SETUP))==(-1))
	{
		prterr("ERROR: Error Invalid Version or Bad Binary for [XPERT SETUP] from Module: [%s] Process: [%s] at line [%d] program [%s].",module,process,line,file);
		ERRORDIALOG("INVALID BINARY VERSION!","The Xpert/MBGUI Setup binary is invalid or missing.",ShutdownOnError,TRUE);
		return_value=(-1);
	} else {
		if(diagmix || diagapps)
		{
			prterr("DIAG XPERT.STP version=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",x,module,process,line,file);
		}
	}
	if(INITNRD()) 
	{
		prterr("ERROR: Cannot Initialize the Database Subsystem at line [%d] program [%s].",line,file);
		ERRORDIALOG("CANNOT INITIALIZE DATABASE SUBSYSTEM!","The Database SubSystem cannot be initialized.\nPlease call your installer for support.",ShutdownOnError,TRUE);
		return_value=(-1);
	} else if(InitializeSecurity()) 
	{
		prterr("Error Cannot Initialize the Security Subsystem at line [%d] program [%s].",line,file);
		ERRORDIALOG("CANNOT INITIALIZE SECURITY SUBSYSTEMS!","The Security SubSystem cannot be initialized.\nPlease call your installer for support.",ShutdownOnError,TRUE);
		return_value=(-1);
	} else if(!PROCsecurity(module,process)) 
	{
		prterr("ERROR: Security Access to Module [%s] Process [%s] DENIED! at line [%d] program [%s].",module,process,line,file);
		desc=Rmalloc(RDAstrlen(module)+RDAstrlen(process)+111);
		sprintf(desc,"The Security SubSystem denied you access to Module [%s] Process [%s].\nPlease call your installer for support.",module,process);
		ERRORDIALOG("SECURITY ACCESS DENIED!",desc,ShutdownOnError,TRUE);
		Rfree(desc);
		return_value=(-1);
	} else {
		if(RDA_SOFTWARE_TYPE!=(-1))
		{
			if(RDA_SOFTWARE_TYPE!=XPERT_SETUP->software_type)
			{ 
				prterr("ERROR: Invalid RDA Software Type of [%d] for application from Module: [%s] Process: [%s] at line [%d] program [%s].",RDA_SOFTWARE_TYPE,module,process,line,file);
				desc=Rmalloc(10+RDAstrlen(module)+RDAstrlen(process)+141);
				sprintf(desc,"Invalid RDA Software Type of [%2d] for application from Module: [%s]  Process: [%s], Access Denied.\nPlease call your installer for support.",RDA_SOFTWARE_TYPE,module,process);
				ERRORDIALOG("INVALID SOFTWARE TYPE!",desc,ShutdownOnError,TRUE);
				Rfree(desc);
				return_value=(-1);
			} else {
				if(diagmix || diagapps)
				{
					prterr("DIAG RDA_SOFTWARE_TYPE=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",RDA_SOFTWARE_TYPE,module,process,line,file);
				}
			}
		} else {
			if(diagmix || diagapps)
			{
				prterr("DIAG RDA_SOFTWARE_TYPE=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",RDA_SOFTWARE_TYPE,module,process,line,file);
			}
		}
		INITTRANS();
		OPNLOG(module,process);
	}
	return(return_value);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(ViewLicInitializeSubsystems(argc,argv,module,"VIEW LICENSES"))
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"VIEW LICENSES");
	licenselist=RDA_load_licenses();
	addlstrsrc(mainrsrc,"LICENSES LIST",&x,TRUE,NULL,licenselist->numlibs,&licenselist->libs,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletelic,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitviewtest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,screenhelp,NULL);
	APPmakescrn(mainrsrc,TRUE,quitviewlic,NULL,TRUE);
}
