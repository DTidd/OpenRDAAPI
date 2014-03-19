/* dogsv.c - Do Generic Setup Variables */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dogsv.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dogsv.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*CPP_OPART gsvcm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define GetAllSetupVariables(a)	xGetAllSetupVariables(a,__LINE__,__FILE__)
extern void xGetAllSetupVariables(char *,int,char *);
#define RDAGenericSetup2rsrc(a)	xRDAGenericSetup2rsrc(a,__LINE__,__FILE__)
extern void xRDAGenericSetup2rsrc(RDArsrc *,int,char *);
#define rsrc2RDAGenericSetup(a)	xrsrc2RDAGenericSetup(a,__LINE__,__FILE__)
extern void xrsrc2RDAGenericSetup(RDArsrc *,int,char *);
#define WriteAllSetupVariables(a)	xWriteAllSetupVariables(a,__LINE__,__FILE__)
extern void xWriteAllSetupVariables(char *,int,char *);


static void quitdogsv(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	ShutdownSubsystems();
}
static void savegsv(RDArsrc *mainrsrc)
{
	rsrc2RDAGenericSetup(mainrsrc);
	WriteAllSetupVariables(mainrsrc->module);
	quitdogsv(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,argv[1],"GENERIC SETUP VARIABLES"))
	{
		ShutdownSubsystems();
		return;
	}
	if(argc<2)
	{
		ShutdownSubsystems();
		return;
	}
	if(argc>2)
	{
		mainrsrc=RDArsrcNEW(argv[1],argv[2]);
	} else {
		mainrsrc=RDArsrcNEW(argv[1],"GENERIC SETUP VARIABLES");
	}
	GetAllSetupVariables(argv[1]);
	RDAGenericSetup2rsrc(mainrsrc);
	addrfexrsrc(mainrsrc,"SAVE",TRUE,savegsv,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdogsv,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADV2GET_SCREEN_VIRTUALS(mainrsrc,0,FALSE,TRUE,FALSE);	
	ADVSCRNvirtual2rsrc(mainrsrc,SCRNvirtualSubData,mainrsrc);
	SetVirFieldFuncs(mainrsrc);

	ADVcomputeandupdateallSCRNvirtuals(mainrsrc,SCRNvirtualSubData,mainrsrc,TRUE);
	APPmakescrn(mainrsrc,TRUE,quitdogsv,NULL,TRUE);
}
