/* purcpo.c - Xpert Create Purchase Order Identification Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "purcpo.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purcpo.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="PURORD";
short purids=(-1);

void selectpurcpo(RDArsrc *mainrsrc)
{
	char Stemp[101],*pfix=NULL,*sfix=NULL,*e=NULL;
	char zfill=TRUE,*idx=NULL,deleteflag=FALSE,assigned=FALSE;
	int fromnum=0,tonum=0,x,len=0;
	RDArsrc *tmprsrc=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"FROM NUMBER",&fromnum);
	FINDRSCGETINT(mainrsrc,"TO NUMBER",&tonum);
	if((tonum-fromnum)<1) return;
	FINDRSCGETCHAR(mainrsrc,"ZERO FILL",&zfill);
	FINDRSCGETSTRING(mainrsrc,"PREFIX",&pfix);
	FINDRSCGETSTRING(mainrsrc,"SUFFIX",&sfix);
	e=Rmalloc((RDAstrlen(pfix)*2)+(RDAstrlen(sfix)*2)+51);
	sprintf(e,"Create PO ID's %s%d%s to %s%d%s",pfix,fromnum,sfix,
		pfix,tonum,sfix);
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,e,stop_diagnostic,(tonum-fromnum+1));
	if(e!=NULL) Rfree(e);
	addDFincvir(tmprsrc,module,"PURIDS",NULL,purids);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			prterr("Error Cannot Create Diagnostic Screen.");
		} else ForceWindowUpdate(tmprsrc);
	}
	memset(Stemp,0,101);
	sprintf(Stemp,"%d",tonum);
	len=RDAstrlen(Stemp);
	for(x=fromnum;x<(tonum+1);++x)
	{
		if(abort_diagnostic) break;
		memset(Stemp,0,101);
		if(zfill) sprintf(Stemp,"%0*d",len,x);
			else sprintf(Stemp,"%d",x);
		idx=Rmalloc(RDAstrlen(pfix)+RDAstrlen(sfix)+RDAstrlen(Stemp)+1);
		sprintf(idx,"%s%s%s",(pfix!=NULL ? pfix:""),Stemp,
			(sfix!=NULL ? sfix:""));
		ZERNRD(purids);
		FINDFLDSETSTRING(purids,"PO IDENTIFICATION",idx);
		LOCNRDFILE(purids);
		if(ADVEQLNRDsec(purids,1,SCRNvirtualSubData,tmprsrc)) 
		{
			FINDFLDSETCHAR(purids,"DELETEFLAG",deleteflag);
			FINDFLDSETCHAR(purids,"ASSIGNED",assigned);
			WRTTRANS(purids,0,NULL,NULL);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		} else {
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
		}
		if(idx!=NULL) Rfree(idx);
		UNLNRDFILE(purids);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(pfix!=NULL) Rfree(pfix);
	if(sfix!=NULL) Rfree(sfix);
}
void quitpurcpo(RDArsrc *mainrsrc)
{
	if(purids!=(-1)) CLSNRD(purids);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int fromnum=1,tonum=99999;
	char zfill=TRUE;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,
		"CREATE PURCHASE ORDER IDENTIFICATIONS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((purids=APPOPNNRD(module,"PURIDS",TRUE,TRUE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDER IDENTIFICATIONS");
	addstdrsrc(mainrsrc,"PREFIX",VARIABLETEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"SUFFIX",VARIABLETEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"FROM NUMBER",LONGV,15,&fromnum,TRUE);
	addstdrsrc(mainrsrc,"TO NUMBER",LONGV,15,&tonum,TRUE);
	addstdrsrc(mainrsrc,"ZERO FILL",BOOLNS,1,&zfill,TRUE);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectpurcpo,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitpurcpo,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(mainrsrc);
	APPmakescrn(mainrsrc,FALSE,quitpurcpo,NULL,TRUE);
	RDAAPPMAINLOOP();
}
