/* miscids.c - Xpert Create Misc Billing Identification Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "miscids.lnx"
#endif
#ifdef WIN32
#define __NAM__ "miscids.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="MISCBILL";
short miscids=(-1),miscclas=(-1);

void selectpurcpo(RDArsrc *mainrsrc)
{
	char Stemp[101],*pfix=NULL,*sfix=NULL,*e=NULL;
	char *classid=NULL,zfill=TRUE,*idx=NULL,deleteflag=FALSE,assigned=FALSE;
	int fromnum=0,tonum=0,x,len=0;
	RDArsrc *tmprsrc=NULL;

	readallwidgets(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	rsrc2filerecord(miscclas,mainrsrc);
	FINDFLDGETSTRING(miscclas,"CLASS IDENTIFICATION",&classid);
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
	addDFincvir(tmprsrc,module,"MISCIDS",NULL,miscids);
	addDFincvir(tmprsrc,module,"MISCCLAS",NULL,miscclas);
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
		ZERNRD(miscids);
		FINDFLDSETSTRING(miscids,"CLASS IDENTIFICATION",classid);
		FINDFLDSETSTRING(miscids,"MISC IDENTIFICATION",idx);
		LOCNRDFILE(miscids);
		if(ADVEQLNRDsec(miscids,1,SCRNvirtualSubData,tmprsrc)) 
		{
			FINDFLDSETCHAR(miscids,"DELETEFLAG",deleteflag);
			FINDFLDSETCHAR(miscids,"ASSIGNED",assigned);
			ADVWRTTRANSsec(miscids,0,NULL,NULL,SCRNvirtualSubData,tmprsrc);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		} else {
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
		}
		if(idx!=NULL) Rfree(idx);
		UNLNRDFILE(miscids);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(pfix!=NULL) Rfree(pfix);
	if(classid!=NULL) Rfree(classid);
	if(sfix!=NULL) Rfree(sfix);
}
void quitpurcpo(RDArsrc *mainrsrc)
{
	if(miscids!=(-1)) CLSNRD(miscids);
	if(miscclas!=(-1)) CLSNRD(miscclas);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
}
static void setclass(RDArsrc *mainrsrc)
{
	char *classid=NULL;

	readwidget(mainrsrc,"[MISCCLAS][CLASS IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[MISCCLAS][CLASS IDENTIFICATION]",&classid);
	FINDFLDSETSTRING(miscclas,"CLASS IDENTIFICATION",classid);
	if(ADVEQLNRDsec(miscclas,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(miscclas,1);
	filerecord2rsrc(miscclas,mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	updateallrsrc(mainrsrc);
}
void main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	int fromnum=1,tonum=99999;
	char zfill=TRUE;

	if(InitializeSubsystems(argc,argv,module,
		"CREATE MISC BILLING IDENTIFICATIONS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((miscids=APPOPNNRD(module,"MISCIDS",TRUE,TRUE))==(-1)) return;
	if((miscclas=APPOPNNRD(module,"MISCCLAS",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"CREATE MISC BILLING IDENTIFICATIONS");
	addDFincvir(mainrsrc,module,"MISCCLAS",NULL,miscclas);
	file2rsrc(miscclas,mainrsrc,TRUE);
	FINDRSCSETFUNC(mainrsrc,"[MISCCLAS][CLASS IDENTIFICATION]",setclass,NULL);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
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
	SCRNvirtual2rsrc(mainrsrc);
	DefaultScreens(mainrsrc);
	setclass(mainrsrc);
	computeandupdateallSCRNvirtuals(mainrsrc,FALSE);
	APPmakescrn(mainrsrc,FALSE,quitpurcpo,NULL,TRUE);
	RDAAPPMAINLOOP();
}
