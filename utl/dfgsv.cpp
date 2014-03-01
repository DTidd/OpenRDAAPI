/* dfgsv.c - Define Generic Setup Variables */
#ifndef WIN32
#define __NAM__ "dfgsv.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfgsv.exe"
#endif
#include <app.hpp>





#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTILITIES";
RDArsrc *mainrsrc=NULL,*genstprsrc=NULL;
APPlib *modsavl=NULL,*genstpavl=NULL;
APPlib *ftypes=NULL;
RDAGenericSetup *GSV=NULL;
static int getgenstpmodules(RDArsrc *,char *);
static void getgenstpmodulescb(RDArsrc *);
static void savegenericsetup(RDArsrc *,RDArsrc *);
static void quitgenericsetuptest(RDArsrc *,RDArsrc *);
static void setvaluesx(RDArsrc *,char *,char *),setvaluesxcb(RDArsrc *);
static void quitgenericsetup(RDArsrc *,RDArsrc *);
static RDAdefault *genstpdefaults=NULL;
static void print_genericsetupscb(RDArsrc *);
static void print_genericsetups(RDArsrc *,RDArsrc *);
static char changedvalues=FALSE;
static void modlistcb(RDArsrc *);

static void quitgenericsetup(RDArsrc *genstprsrc,RDArsrc *mainrsrc)
{
	if(ftypes!=NULL) freeapplib(ftypes);
	changedvalues=FALSE;
	if(genstprsrc!=NULL)
	{
		killwindow(genstprsrc);
		free_rsrc(genstprsrc);
	}
	if(genstpdefaults!=NULL) FreeRDAdefault(genstpdefaults);
}
static void quitgenericsetuptest(RDArsrc *genstprsrc,RDArsrc *mainrsrc)
{
	readallwidgets(genstprsrc);
	if(COMPARE_RSRCS(genstprsrc,genstpdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the GENERIC SETUP VARIABLE DEFINITION.\nDo you want to Save these changes?",savegenericsetup,quitgenericsetup,FALSE,2,genstprsrc,mainrsrc,NULL);
	} else {
		quitgenericsetup(genstprsrc,mainrsrc);
	}
}
static void savegenericsetup(RDArsrc *genstprsrc,RDArsrc *mainrsrc)
{
	char *libname=NULL;
	int ftypex=0;
	
	readallwidgets(genstprsrc);
	if(GSV->module!=NULL) Rfree(GSV->module);
	FINDRSCGETSTRING(genstprsrc,"MODULE",&GSV->module);
	if(GSV->name!=NULL) Rfree(GSV->name);
	FINDRSCGETSTRING(genstprsrc,"NAME",&GSV->name);
	if(GSV->module!=NULL && GSV->name!=NULL)
	{
		FINDRSCGETSTRING(genstprsrc,"DESCRIPTION",&GSV->desc);
		FINDRSCGETSTRING(genstprsrc,"LABEL",&GSV->label);
		FINDRSCGETSHORT(genstprsrc,"LENGTH",&GSV->length);
		ftypex=0;
		FINDRSCGETINT(genstprsrc,"FIELD TYPES",&ftypex);
		GSV->type=(short)ftypex;
		switch(ftypex)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case BOOLNS:
			case CHARACTERS:
				GSV->value.string_value=NULL;
				break;
			case SHORTV:
			case SSHORTV:
				if(GSV->value.short_value==NULL) 
				{
					GSV->value.short_value=Rmalloc(sizeof(short));
					*GSV->value.short_value=0;
				}
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(GSV->value.integer_value==NULL) 
				{
					GSV->value.integer_value=Rmalloc(sizeof(int));
					*GSV->value.integer_value=0;
				}
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				if(GSV->value.float_value==NULL) 
				{
					GSV->value.float_value=Rmalloc(sizeof(double));
					*GSV->value.float_value=0;
				}
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error Unrecognized Field type [%d].",ftypex); 
				break;
		}
	
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(GSV->module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.GSV",CURRENTDIRECTORY,GSV->module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,GSV->module);
#endif
		if(writeRDAGenericSetupbin(libname,GSV))
		{
			ERRORDIALOG("Cannot Write Generic Setup Variable","Error attempting to write the genericsetup field.  Check the permissions on the Generic Setup Variable Binary Library, and retry.  Call your installer.",NULL,FALSE);
		}
		if(libname!=NULL) Rfree(libname);
		FreeRDAGenericSetup(GSV);
	}
	getgenstpmodulescb(mainrsrc);
	modlistcb(mainrsrc);
	quitgenericsetup(genstprsrc,mainrsrc);
}
static void changeftype(RDArsrc *r)
{
	int type=0;

	FINDRSCGETINT(r,"FIELD TYPES",&type);
	switch(type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case PLAINTEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PHONE:
		case CUSTOMTYPE:
		case ZIPCODE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case BOOLNS:
		case CHARACTERS:
			GSV->value.string_value=NULL;
			break;
		case SHORTV:
		case SSHORTV:
			GSV->value.short_value=NULL;
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			GSV->value.integer_value=NULL;
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
			GSV->value.float_value=NULL;
			break;
		case BUTTONS:
			break;
		default:
			prterr("Error Unrecognized Field type [%d].",type); 
			break;
	}
}
static void genericsetupscreen(RDArsrc *mainrsrc,char *modulx,char *name,
	short type,short length,char *desc,char *label)
{
	int typex,x=0;
	RDArsrc *genstprsrc=NULL;

	genstprsrc=RDArsrcNEW(module,"MAINTAIN GENERIC SETUP VARIABLE");
	addstdrsrc(genstprsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),
		TRUE);
	addstdrsrc(genstprsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),
		TRUE);
	addsctrsrc(genstprsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	addstdrsrc(genstprsrc,"LABEL",VARIABLETEXT,0,(label!=NULL ? label:""),
		TRUE);
	addstdrsrc(genstprsrc,"LENGTH",SHORTV,3,&length,TRUE);
	typex=type;
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x) addAPPlib(ftypes,standardfieldtypes[x]);
	addlstrsrc(genstprsrc,"FIELD TYPES",&typex,TRUE,changeftype,
		ftypes->numlibs,&ftypes->libs,NULL);
	addrfcbrsrc(genstprsrc,"SAVE",TRUE,savegenericsetup,mainrsrc);
	addrfcbrsrc(genstprsrc,"QUIT",TRUE,quitgenericsetuptest,mainrsrc);
	addbtnrsrc(genstprsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(genstprsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	genstpdefaults=GetDefaults(genstprsrc);
	APPmakescrn(genstprsrc,TRUE,quitgenericsetup,mainrsrc,FALSE);
}
static void okgenericsetup(RDArsrc *mainrsrc)
{
	int selected=0,selectedv=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"GENERIC SETUP VARIABLES AVAILABLE",&selectedv)) return;
	if(GSV!=NULL) { FreeRDAGenericSetup(GSV); GSV=NULL; }
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,modsavl->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,modsavl->libs[selected]);
#endif
	if(GSV==NULL)
	{
		if(!RDAstrcmp(genstpavl->libs[selectedv],"No Definitions")) 
		{
			GSV=RDAGenericSetupNew(modsavl->libs[selected],NULL);
		} else {			
			GSV=RDAGenericSetupNew(modsavl->libs[selected],genstpavl->libs[selectedv]);
			if(getRDAGenericSetupbin(libx,GSV))
			{
				ERRORDIALOG("Cannot Read Generic Setup Variable","Error in permissions, the user cannot read the genericsetup field binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
				return;
			}
		}
	}
	if(libx!=NULL) Rfree(libx);
	if(GSV->type==(-1)) GSV->type=0;
	genericsetupscreen(mainrsrc,GSV->module,GSV->name,
		GSV->type,GSV->length,GSV->desc,GSV->label);
}
static void newgenericsetup(RDArsrc *mainrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(GSV!=NULL) { FreeRDAGenericSetup(GSV); GSV=NULL; }
	GSV=RDAGenericSetupNew(modsavl->libs[selected],NULL);
	if(GSV->type==(-1)) GSV->type=0;
	genericsetupscreen(mainrsrc,GSV->module,GSV->name,GSV->type,
		GSV->length,GSV->desc,GSV->label);
}
static void quitdfgsv(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(modsavl!=NULL) freeapplib(modsavl);
	if(genstpavl!=NULL) freeapplib(genstpavl);
	if(genstpdefaults!=NULL) FreeRDAdefault(genstpdefaults);
	ShutdownSubsystems();
}
static int getgenstpmodules(RDArsrc *mainrsrc,char *modname)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	
	if(modsavl!=NULL) freeapplib(modsavl); 
	modsavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.GSV",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modsavl,tmp);
	}
	if(modsavl->numlibs<1)
	{
		addAPPlib(modsavl,"No Libraries");
		selectedm=0;
	} else {
		SORTAPPlib(modsavl);
		if(selectedm>=modsavl->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(modsavl,modname))==(-1))
		{
			selectedm=0;
		} else {
			selectedm=x;
		}
	} else {
		selectedm=0;
	}
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selectedm,modsavl))
		{
			updatersrc(mainrsrc,"MODULE LIST");
		}
	}
	return(selectedm);
}
static void getgenstpmodulescb(RDArsrc *mainrsrc)
{
	int selectedm=0;
	char *temp=NULL;
	
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	temp=stralloc(modsavl->libs[selectedm]);
	getgenstpmodules(mainrsrc,temp);
	if(temp!=NULL) Rfree(temp);
}
static void deletegsv(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulename=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"GENERIC SETUP VARIABLES AVAILABLE",&s)) return;
	modulename=adddashes(modsavl->libs[m]);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,modulename);
#endif
	if(deletelibbin(libx,genstpavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Virtual","Error attempting to delete genericsetup field.  Check the permissions on the Generic Setup Variable Binary Library, and retry.  Call your installer.",NULL,FALSE);
	} else {
		getgenstpmodulescb(mainrsrc);
		modlistcb(mainrsrc);
	}
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
}
static void deletegenstpest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(genstpavl->libs[s],"No Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE GENERIC SETUP VARIABLE!","Are you sure you want to delete this genericsetup field?",deletegsv,NULL,FALSE,mainrsrc);
}
static void setvaluesx(RDArsrc *mainrsrc,char *modname,char *fldname)
{
	char *temp=NULL,*temp2=NULL,*temp3=NULL;
	char *libx=NULL;

	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,modname);
#endif
		if(fldname!=NULL)
		{
			if(!RDAstrcmp(fldname,"No Definitions"))
			{
				temp=NULL;
			} else {
				getRDAGenericSetupdesc(libx,fldname,&temp);
			}
		} else {
			temp3=stralloc(genstpavl->libs[0]);
			if(!RDAstrcmp(temp3,"No Definitions"))
			{
				temp=NULL;
			} else {
				getRDAGenericSetupdesc(libx,temp3,&temp);
			}
		}
	} else {
		temp2=stralloc(modsavl->libs[0]);
		temp3=stralloc(genstpavl->libs[0]);
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp2)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,temp2);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,temp2);
#endif
		if(!RDAstrcmp(temp3,"No Definitions"))
		{
			temp=NULL;
		} else {
			getRDAGenericSetupdesc(libx,temp3,&temp);
		}
	}
	if(libx!=NULL) Rfree(libx);
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		{
			updatersrc(mainrsrc,"DESCRIPTION");
		}
	}
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
}
static void setvaluesxcb(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"GENERIC SETUP VARIABLES AVAILABLE",&selected)) return;
	setvaluesx(mainrsrc,modsavl->libs[selectedm],genstpavl->libs[selected]);
}
static int dogenstpfldlist(RDArsrc *mainrsrc,char *modname,char *fldname)
{
	int x=0,selectedf=0;
	char *libx=NULL;

	if(genstpavl!=NULL) freeapplib(genstpavl); 
	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,modname);
#endif
	} else {
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl->libs[0])+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,modsavl->libs[0]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,modsavl->libs[0]);
#endif
	}
	genstpavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(genstpavl==NULL)
	{
		genstpavl=APPlibNEW();
		addAPPlib(genstpavl,"No Definitions");
	}
	if(fldname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(genstpavl,fldname))==(-1))
		{
			selectedf=0;
		} else {
			selectedf=x;
		}
	} else {
		selectedf=0;
	}
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"GENERIC SETUP VARIABLES AVAILABLE",selectedf,genstpavl))
		{
			updatersrc(mainrsrc,"GENERIC SETUP VARIABLES AVAILABLE");
		}
	}
	return(selectedf);
}
static void modlistcb(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *temp=NULL,*temp2=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"GENERIC SETUP VARIABLES AVAILABLE",&selectedf)) return; 
	temp=stralloc(modsavl->libs[selected]);
	temp2=stralloc(genstpavl->libs[selectedf]);
	dogenstpfldlist(mainrsrc,temp,temp2);
	setvaluesx(mainrsrc,temp,temp2);
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
}
static void print_genericsetups(RDArsrc *prsrc,RDArsrc *mainrsrc)
{
	RDA_PFILE *fp=NULL;
	int x=0,selected=0;
	char *tmpx=NULL,*libx=NULL,*odevice=NULL,print_style=TRUE;
	RDAGenericSetup *d=NULL;
	APPlib *genstpnames=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&odevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(odevice==NULL)
	{
		ERRORDIALOG("Output Device Failure","There was no device specified.",NULL,FALSE);
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl->libs[selected])+10);
	tmpx=adddashes(modsavl->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,tmpx);
#endif
	genstpnames=getlibnames(libx,FALSE);
	if(genstpnames!=NULL)
	{
		fp=RDA_popen(odevice,"w");
		if(odevice!=NULL) Rfree(odevice);
		if(fp==NULL)
		{
			prterr("Error Couldn't open device.");
			if(genstpnames!=NULL) freeapplib(genstpnames);
			if(tmpx!=NULL) Rfree(tmpx);
			if(libx!=NULL) Rfree(libx);
			return;
		} 
		SelectPrintType(odevice,print_style,fp);
		RDA_fprintf(fp,"GENERIC SETUP VARIABLES AVAILABLE\r\n");
		for(x=0;x<genstpnames->numlibs;++x)
		{
			RDAGenericSetupNew(modsavl->libs[selected],genstpnames->libs[x]);
			if(!getRDAGenericSetupbin(libx,d))
			{
				RDA_fprintf(fp,"Name: %s\r\n",d->name);
				RDA_fprintf(fp,"\tType [%d] [%s] Length [%d]\r\n",d->type,standardfieldtypes[d->type],d->length);
				RDA_fprintf(fp,"\tDescription [%s]\r\n\r\n",(d->desc!=NULL ? d->desc:""));
				FreeRDAGenericSetup(d);
				d=NULL;
			}
		}
		RDA_pclose(fp);
		if(genstpnames!=NULL) freeapplib(genstpnames);
	} else {
		prterr("Error No Generic Setup Variables Available.");
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
}
static void quit_print_genericsetups(RDArsrc *prsrc)
{
	free_rsrc(prsrc);
}
static void print_genericsetupscb(RDArsrc *mainrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,print_genericsetups,mainrsrc);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_genericsetups,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_genericsetups(prsrc);
	}
}
static void gsvscn(RDArsrc *r)
{
	APPlib *args=NULL;
	int x=0;
	char udesc=FALSE;

	readwidget(r,"USE DESCRIPTION");
	FINDRSCGETCHAR(r,"USE DESCRIPTION",&udesc);
	FINDRSCGETINT(r,"MODULE LIST",&x);
	args=APPlibNEW();
	addAPPlib(args,modsavl->libs[x]);
	if(udesc) addAPPlib(args,"TRUE");
		else addAPPlib(args,"FALSE");
	ExecuteProgram("gsvscn",args);
	if(args!=NULL) freeapplib(args);
}
static void dogsv(RDArsrc *r)
{
	APPlib *args=NULL;
	int x=0;

	FINDRSCGETINT(r,"MODULE LIST",&x);
	args=APPlibNEW();
	addAPPlib(args,modsavl->libs[x]);
	ExecuteProgram("dogsv",args);
	if(args!=NULL) freeapplib(args);
}
static void dfscn(RDArsrc *r)
{
	ExecuteProgram("dfscn",NULL);
}
static void dfmenu(RDArsrc *r)
{
	ExecuteProgram("dfmenu",NULL);
}
static void dflist(RDArsrc *r)
{
	APPlib *args=NULL;
	int x=0;

	FINDRSCGETINT(r,"MODULE LIST",&x);
	args=APPlibNEW();
	addAPPlib(args,modsavl->libs[x]);
	ExecuteProgram("dflist",args);
	if(args!=NULL) freeapplib(args);
}
static void dfload(RDArsrc *r)
{
	APPlib *args=NULL;
	int x=0;

	FINDRSCGETINT(r,"MODULE LIST",&x);
	args=APPlibNEW();
	addAPPlib(args,modsavl->libs[x]);
	ExecuteProgram("dfload",args);
	if(args!=NULL) freeapplib(args);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0,y=0;
	RDArsrc *mainrsrc=NULL;
	char *modname=NULL,*fldname=NULL;
	char udesc=FALSE;

	if(InitializeSubsystems(argc,argv,module,"DEFINE GENERIC SETUP VARIABLES")) 
	{
		return;
	}
	if(argc>2)
	{ 
		modname=stralloc(argv[1]);
		fldname=stralloc(argv[2]);
	} else if(argc>1) 
	{
		modname=stralloc(argv[1]);
	}
	x=getgenstpmodules(mainrsrc,modname);
	y=dogenstpfldlist(mainrsrc,modname,fldname);
	mainrsrc=RDArsrcNEW(module,"DEFINE GENERIC SETUP VARIABLES");
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,modlistcb,modsavl->numlibs,&modsavl->libs,NULL);
	addlstrsrc(mainrsrc,"GENERIC SETUP VARIABLES AVAILABLE",&y,TRUE,setvaluesxcb,genstpavl->numlibs,&genstpavl->libs,NULL);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	addstdrsrc(mainrsrc,"USE DESCRIPTION",BOOLNS,1,&udesc,TRUE);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newgenericsetup,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okgenericsetup,NULL);
	addbtnrsrc(mainrsrc,"PRINT",TRUE,print_genericsetupscb,NULL);
	addbtnrsrc(mainrsrc,"DOGSV",TRUE,dogsv,NULL);
	addbtnrsrc(mainrsrc,"SCREEN",TRUE,gsvscn,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfgsv,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletegenstpest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(mainrsrc,"DFSCN",TRUE,dfscn,NULL);
	addbtnrsrc(mainrsrc,"DFMENU",TRUE,dfmenu,NULL);
	addbtnrsrc(mainrsrc,"DFLIST",TRUE,dflist,NULL);
	addbtnrsrc(mainrsrc,"DFLOAD",TRUE,dfload,NULL);
	setvaluesx(mainrsrc,modname,fldname);
	APPmakescrn(mainrsrc,TRUE,quitdfgsv,NULL,TRUE);
}
