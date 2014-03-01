/* occcgl.c - Xpert Create Group License Numbers Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "occcgl.lnx"
#endif
#ifdef WIN32
#define __NAM__ "occcgl.exe"
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

char *module="OCCTAX";
short occglic=(-1);
short occyear=(-1);
short occgrp=(-1);
APPlib *yearsavl=NULL,*grpsavl=NULL;
int year_len=0,group_len=0;

void selectocccgl(RDArsrc *mainrsrc)
{
	char Stemp[101],*pfix=NULL,*sfix=NULL,*e=NULL;
	char zfill=TRUE,*idx=NULL,deleteflag=FALSE,assigned=FALSE;
	int fromnum=0,tonum=0,x,len=0;
	RDArsrc *tmprsrc=NULL;
	int selyear=0,year=0,selgroup_no=0,group_no=0;
	short keyno=0;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"CHARGE YEARS",&selyear);
	if(!RDAstrcmp(yearsavl->libs[selyear],"No Years")) return;
	FINDRSCGETINT(mainrsrc,"GROUP NUMBERS",&selgroup_no);
	if(!RDAstrcmp(grpsavl->libs[selgroup_no],"No Year Groups")) return;
	year=Ratoi(yearsavl->libs[selyear]);
	group_no=Ratoi(grpsavl->libs[selgroup_no]);
	FINDRSCGETINT(mainrsrc,"FROM NUMBER",&fromnum);
	FINDRSCGETINT(mainrsrc,"TO NUMBER",&tonum);
	if((tonum-fromnum)<1) return;
	FINDRSCGETCHAR(mainrsrc,"ZERO FILL",&zfill);
	FINDRSCGETSTRING(mainrsrc,"PREFIX",&pfix);
	FINDRSCGETSTRING(mainrsrc,"SUFFIX",&sfix);
	e=Rmalloc((RDAstrlen(pfix)*2)+(RDAstrlen(sfix)*2)+51);
	sprintf(e,"Create GROUP LIC NO's %s%d%s to %s%d%s",
		(pfix==NULL?"":pfix),fromnum,(sfix==NULL?"":sfix),
		(pfix==NULL?"":pfix),tonum,(sfix==NULL?"":sfix));
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,e,stop_diagnostic,(tonum-fromnum+1));
	if(e!=NULL) Rfree(e);
	addDFincvir(tmprsrc,module,"OCCGLIC",NULL,occglic);
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
		keyno=KEYNUMBER(occglic,"OCCGLIC KEY");
		if(keyno<1) keyno=1;
		ZERNRD(occglic);
		FINDFLDSETINT(occglic,"CHARGE YEAR",year);
		FINDFLDSETINT(occglic,"GROUP NUMBER",group_no);
		FINDFLDSETSTRING(occglic,"LICENSE NUMBER",idx);
/*
		LOCNRDFILE(occglic);	
*/
		if(ADVEQLNRDsec(occglic,1,SCRNvirtualSubData,tmprsrc)) 
		{
			FINDFLDSETINT(occglic,"CHARGE YEAR",year);
			FINDFLDSETINT(occglic,"GROUP NUMBER",group_no);
			FINDFLDSETCHAR(occglic,"DELETEFLAG",deleteflag);
			FINDFLDSETCHAR(occglic,"ASSIGNED",assigned);
			WRTTRANS(occglic,0,NULL,NULL);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		} else {
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
		}
		if(idx!=NULL) Rfree(idx);
/*
		UNLNRDFILE(occglic);
*/
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(pfix!=NULL) Rfree(pfix);
	if(sfix!=NULL) Rfree(sfix);
}
void getsupportingrecords(RDArsrc *mainrsrc)
{
	int selyear=0,selgroup_no=0,year=0,group_no=0;

	FINDRSCGETINT(mainrsrc,"CHARGE YEARS",&selyear);
	if(!RDAstrcmp(yearsavl->libs[selyear],"No Years")) return;
	FINDRSCGETINT(mainrsrc,"GROUP NUMBERS",&selgroup_no);
	if(!RDAstrcmp(grpsavl->libs[selgroup_no],"No Year Groups")) return;
	year=Ratoi(yearsavl->libs[selyear]);
	group_no=Ratoi(grpsavl->libs[selgroup_no]);
	ZERNRD(occgrp);
	FINDFLDSETINT(occgrp,"GROUP NUMBER",group_no);
	FINDFLDSETINT(occgrp,"CHARGE YEAR",year);
	if(ADVEQLNRDsec(occgrp,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(occgrp,1);
	}
	ZERNRD(occyear);
	FINDFLDSETINT(occyear,"CHARGE YEAR",year);
	if(ADVEQLNRDsec(occyear,1,SCRNvirtualSubData,mainrsrc)) 
	{
		KEYNRD(occyear,1);
	}
}
void quitocccgl(RDArsrc *mainrsrc)
{
	if(yearsavl!=NULL) freeapplib(yearsavl);
	if(grpsavl!=NULL) freeapplib(grpsavl);
	UNLNRDFILE(occglic);
	UNLNRDFILE(occgrp);
	if(occglic!=(-1)) CLSNRD(occglic);
	if(occyear!=(-1)) CLSNRD(occyear);
	if(occgrp!=(-1)) CLSNRD(occgrp);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
}
/*
int GetFieldLength(char *modname,char *filename,char *fieldname)
{
	short engine=0,nofields=0,nokeys=0;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	NRDfield *fields=NULL,*fldx=NULL;
	int y=0,z=0;
	int return_value=0;


	if(!GETBIN(modname,filename,&engine,&nofields,&fields,&nokeys,&keys))
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
		}
		Rfree(keys);
		if(fields!=NULL)
		{
			for(y=0,fldx=fields;y<nofields;++y,++fldx)
			{
				if(fldx->name!=NULL)
				{
					if(!RDAstrcmp(fldx->name,fieldname))
					{
						if(fldx->len>0)
						{
							return_value=fldx->len;
						} else {
							return_value=0;
						}
					}
					Rfree(fldx->name);
				}
			}
			Rfree(fields);
		}
	}
	return(return_value);
}
*/
APPlib *makeyears(APPlib *yearsavl,int len)
{
	short ef=FALSE;
	char deleteflag=FALSE;
	char *yearstr=NULL;
	int year=0;
	
	if(yearsavl!=NULL)
	{
		freeapplib(yearsavl);
	}
	yearsavl=APPlibNEW();
	ef=GTENRD(occyear,1);
	while(!ef)
	{
		FINDFLDGETCHAR(occyear,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETINT(occyear,"CHARGE YEAR",&year);
			yearstr=Rmalloc(len+1);
			sprintf(yearstr,"%*d",len,year);
			addAPPlib(yearsavl,yearstr);
			if(yearstr!=NULL) Rfree(yearstr);
		}
		ef=NXTNRD(occyear,1);
	}
	if(yearsavl->numlibs<1) 
	{
		addAPPlib(yearsavl,"No Years"); 
	}
	return(yearsavl);
}
APPlib *makegrps(RDArsrc *mainrsrc,APPlib *grpsavl,int len)
{
	short ef=FALSE;
	char deleteflag=FALSE;
	char *grpstr=NULL;
	int group_num=0;
	int selyear=0,year1=0,year2=0;
	short keyno=0;
	
	if(grpsavl!=NULL)
	{
		freeapplib(grpsavl);
	}
	grpsavl=APPlibNEW();
	if(FINDRSCGETINT(mainrsrc,"CHARGE YEARS",&selyear))
	{
		addAPPlib(grpsavl,"No Year Groups");
		return(grpsavl);
	}
	year1=Ratoi(yearsavl->libs[selyear]);
	keyno=KEYNUMBER(occgrp,"OCCGRP KEY");
	if(keyno<1) keyno=1;
	ZERNRD(occgrp);
	FINDFLDSETINT(occgrp,"CHARGE YEAR",year1);
	ef=GTENRD(occgrp,keyno);
	while(!ef)
	{
		FINDFLDGETINT(occgrp,"CHARGE YEAR",&year2);
		if(year2!=year1) break;
		FINDFLDGETCHAR(occgrp,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETINT(occgrp,"GROUP NUMBER",&group_num);
			grpstr=Rmalloc(len+1);
			sprintf(grpstr,"%*d",len,group_num);
			addAPPlib(grpsavl,grpstr);
			if(grpstr!=NULL) Rfree(grpstr);
		}
		ef=NXTNRD(occgrp,1);
	}
	if(grpsavl->numlibs<1) 
	{
		addAPPlib(grpsavl,"No Year Groups"); 
	}
	return(grpsavl);
}
void changegrps(RDArsrc *mainrsrc)
{
	char *lic_format=NULL;
	int fromnum=1,tonum=3000,no_lic=0;

	readallwidgets(mainrsrc);
	getsupportingrecords(mainrsrc);
	FINDFLDGETINT(occgrp,"NEXT LICENSE NUMBER",&fromnum);
	FINDFLDGETINT(occgrp,"NUMBER OF LICENSES",&no_lic);
	FINDFLDGETSTRING(occgrp,"LICENSE NUMBER FORMAT",&lic_format);
	tonum=fromnum+no_lic;
	FINDRSCSETSTRING(mainrsrc,"PREFIX",lic_format);
	FINDRSCSETINT(mainrsrc,"FROM NUMBER",fromnum);
	FINDRSCSETINT(mainrsrc,"TO NUMBER",tonum);
	if(lic_format!=NULL) Rfree(lic_format);
	updateallrsrc(mainrsrc);
}
void changeyears(RDArsrc *mainrsrc)
{
	char *lic_format=NULL;
	int fromnum=1,tonum=3000,no_lic=0;

	readallwidgets(mainrsrc);
	grpsavl=makegrps(mainrsrc,grpsavl,group_len);
	if(!FINDRSCLISTAPPlib(mainrsrc,"GROUP NUMBERS",0,grpsavl))
	{
		updatersrc(mainrsrc,"GROUP NUMBERS");
	}
	getsupportingrecords(mainrsrc);
	FINDFLDGETINT(occgrp,"NEXT LICENSE NUMBER",&fromnum);
	FINDFLDGETINT(occgrp,"NUMBER OF LICENSES",&no_lic);
	FINDFLDGETSTRING(occgrp,"LICENSE NUMBER FORMAT",&lic_format);
	tonum=fromnum+no_lic;
	FINDRSCSETSTRING(mainrsrc,"PREFIX",lic_format);
	FINDRSCSETINT(mainrsrc,"FROM NUMBER",fromnum);
	FINDRSCSETINT(mainrsrc,"TO NUMBER",tonum);
	if(lic_format!=NULL) Rfree(lic_format);
	updateallrsrc(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	time_t ltime;
	RDArsrc *mainrsrc=NULL;
	int fromnum=1,tonum=3000;
	char zfill=TRUE;
        int x,nofields=0;
        char *name=NULL;
        NRDfield *fldx=NULL,*fields=NULL;
        short edit_rsrc=TRUE;
	int z=0,curyear=0;
	char *curyearstr=NULL;
/*
	char *lic_format=NULL;
	int no_lic=0;
*/

	if(InitializeSubsystems(argc,argv,module,
		"CREATE OCCTAX GROUP LICENSE NUMBERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((occglic=APPOPNNRD(module,"OCCGLIC",TRUE,TRUE))==(-1)) return;
	if((occyear=APPOPNNRD(module,"OCCYEAR",TRUE,FALSE))==(-1)) return;
	if((occgrp=APPOPNNRD(module,"OCCGRP",TRUE,FALSE))==(-1)) return;
	LOCNRDFILE(occglic);	
	LOCNRDFILE(occgrp);	
	mainrsrc=RDArsrcNEW(module,"CREATE OCCTAX GROUP LICENSE NUMBERS");
	nofields=NUMFLDS(occglic);
	fields=FLDPOINTER(occglic);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"CHARGE YEAR"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(occglic,fldx->name,&edit_rsrc))
				{
					year_len=fldx->len;
					yearsavl=makeyears(yearsavl,fldx->len);
					curyearstr=Rmalloc(5);
					time(&ltime);
#ifndef WIN32
#ifdef _POSIX_SOURCE
					strftime(curyearstr,5,"%Y",localtime(&ltime));
#else
					ascftime(curyearstr,"%Y",localtime(&ltime));
#endif
#endif
#ifdef WIN32
					strftime(curyearstr,5,"%Y",localtime(&ltime));
#endif
					curyear=FINDAPPLIBELEMENT(yearsavl,curyearstr);
					if(curyear==(-1))
					{
						curyear=0;
					}
					addlstrsrc(mainrsrc,"CHARGE YEARS",
						&curyear,
						TRUE,changeyears,yearsavl->numlibs,
						&yearsavl->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"GROUP NUMBER"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(occglic,fldx->name,&edit_rsrc))
				{
					if(grpsavl!=NULL)
					{
						freeapplib(grpsavl);
					}
					group_len=fldx->len;
					grpsavl=APPlibNEW();
					addAPPlib(grpsavl,"No Year Groups");
					addlstrsrc(mainrsrc,"GROUP NUMBERS",
						&z,TRUE,changegrps,grpsavl->numlibs,
						&grpsavl->libs,NULL);
				}
			} else {
/*
				nonlstmakefld(mainrsrc,paymstr,fldx,TRUE);
*/
			}
		}
	}
	if(name!=NULL) Rfree(name);
	grpsavl=makegrps(mainrsrc,grpsavl,group_len);
	FINDRSCLISTAPPlib(mainrsrc,"GROUP NUMBERS",0,grpsavl);
	if(curyearstr!=NULL) Rfree(curyearstr);

	addstdrsrc(mainrsrc,"PREFIX",VARIABLETEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"SUFFIX",VARIABLETEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"FROM NUMBER",LONGV,15,&fromnum,TRUE);
	addstdrsrc(mainrsrc,"TO NUMBER",LONGV,15,&tonum,TRUE);
	addstdrsrc(mainrsrc,"ZERO FILL",BOOLNS,1,&zfill,TRUE);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectocccgl,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitocccgl,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(mainrsrc);

	APPmakescrn(mainrsrc,FALSE,quitocccgl,NULL,TRUE);

	changeyears(mainrsrc);
/*
	FINDFLDGETINT(occgrp,"NEXT LICENSE NUMBER",&fromnum);
	FINDFLDGETINT(occgrp,"NUMBER OF LICENSES",&no_lic);
	FINDFLDGETSTRING(occgrp,"LICENSE NUMBER FORMAT",&lic_format);
	tonum=fromnum+no_lic;
	FINDRSCSETSTRING(mainrsrc,"PREFIX",lic_format);
	FINDRSCSETINT(mainrsrc,"FROM NUMBER",fromnum);
	FINDRSCSETINT(mainrsrc,"TO NUMBER",tonum);
	if(lic_format!=NULL) Rfree(lic_format);
	updateallrsrc(mainrsrc);
*/

	RDAAPPMAINLOOP();
}
