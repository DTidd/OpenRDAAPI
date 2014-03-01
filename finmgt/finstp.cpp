/* finstp.c - Financial Management Setup Variables */
#ifndef WIN32
#define __NAM__ "finstp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "finstp.exe"
#endif
#include <app.hpp>




#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
static void quitsetup(RDArsrc *,RDAfinmgt *);
static void savesetup(RDArsrc *,RDAfinmgt *);

static void quitsetup(RDArsrc *parent,RDAfinmgt *finmgt)
{
	if(parent!=NULL) free_rsrc(parent); 
	if(finmgt!=NULL) free_finmgt(finmgt); 
	ShutdownSubsystems();
}
static void savesetup(RDArsrc *parent,RDAfinmgt *finmgt)
{
	char compmthod=0,fiscalmo=6;

	ReadRDAScrolledLists(parent);
	FINDRSCGETCHAR(parent,"COMPUTE METHOD",&compmthod);
	finmgt->budget=compmthod;
	FINDRSCGETCHAR(parent,"FISCAL STARTING MONTH",&fiscalmo);
	finmgt->month=fiscalmo;
	writefinmgtbin(finmgt);
	quitsetup(parent,finmgt);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int start=0;
	RDAfinmgt *finmgt=NULL;
	RDArsrc *parent=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"FINMGT","FIN MANAGEMENT SETUP")) 
	{
		return;
	}
	finmgt=RDAfinmgtNEW();
	getfinmgtbin(finmgt);
	start=finmgt->budget;
	parent=RDArsrcNEW("FINMGT","FIN MANAGEMENT SETUP");
	addstdrsrc(parent,"COMPUTE METHOD",LONGV,2,&start,TRUE);
	start=finmgt->month;
	addstdrsrc(parent,"FISCAL STARTING MONTH",LONGV,2,&start,TRUE);
	GET_SCREEN_VIRTUALS(parent,0);
	SCRNvirtual2rsrc(parent);
	addrfexrsrc(parent,"SAVE",TRUE,savesetup,finmgt);
	addrfexrsrc(parent,"QUIT",TRUE,quitsetup,finmgt);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(parent);
	APPmakescrn(parent,TRUE,quitsetup,finmgt,TRUE);
}
