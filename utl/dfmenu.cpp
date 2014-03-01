/* dfmenu.c - Define Menu */
#ifndef WIN32
#define __NAM__ "dfmenu.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfmenu.exe"
#endif
#include <app.hpp>



#include <misc.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
/* Notes: RDArsrc * RDA Resource Structures contain a dynamically created 
	structure for each resource (input, button, list, etc) available on a 
	screen */
static void getdir(RDArsrc *),setdirlist(RDArsrc *);
static void savemenu(RDArsrc *);
static void printbin(RDArsrc *);
static RDAdefault *mtndefaults=NULL;
static RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL;
static char *module="GUI";
/* Notes: APPlib structures contains an int and char **. Really used to Manage 
	a list of character strings */
static APPlib *hlp_bins=NULL,*hlp_defs=NULL;
static APPlib *dir_libs;
/* Notes: RDAmenu structure contains a two char *'s, for a name and command */
static RDAmenu *menu=NULL;

void deletedfmenu(RDArsrc *),quitdfmenu(RDArsrc *),okdfmenu(RDArsrc *);
void makemtnscreen(char *,char *,char *),newdfmenu(RDArsrc *);
void getmenuentries(RDArsrc *),getdirectorymenu(void),quitmenu(RDArsrc *);
void quitmenutest(RDArsrc *);
static void printmoduledefs(RDArsrc *,RDArsrc *);
static void printmodulecb(RDArsrc *);
static void quit_print_moduledefs(RDArsrc *);
static void domenu(RDArsrc *);

static void quit_print_moduledefs(RDArsrc *prsrc)
{
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void printmoduledefs(RDArsrc *prsrc,RDArsrc *mainrsrc)
{
	RDAmenu *tmpmenu=NULL;
	RDA_PFILE *fp=NULL;
	char *dirx=NULL,*dira=NULL,*message=NULL;
	int x=0,s=0,pages=0;
	char *outdevice=NULL,print_style=TRUE;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
			RDA_fprintf(fp,"%*sXPERT MENU DEFINITIONS                      Page: %*d\r\n",28,"",4,++pages);
			RDA_fprintf(fp,"%*sModule [%s]\r\n",
				((80-(10+RDAstrlen(module)))/2),"",hlp_bins->libs[s]);
			RDA_fprintf(fp,"%*s Executed By: %s\r\n\r\n",
				((80-(14+RDAstrlen(USERLOGIN)))/2),"",USERLOGIN);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+68);
			sprintf(message,"The output attempt to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
			return;
		}
		readwidget(mainrsrc,"DIRECTORY");
		if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
		readwidget(mainrsrc,"MENU LIBRARIES");
		if(FINDRSCGETINT(mainrsrc,"MENU LIBRARIES",&s)) return;
		if(hlp_defs->numlibs>0)
		{
			for(x=0;x<hlp_defs->numlibs;++x)
			{
				if(tmpmenu!=NULL) free_menu(tmpmenu);
				tmpmenu=RDAmenuNEW(hlp_defs->libs[x],NULL);
				dira=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[s])+6);
				sprintf(dira,"%s/%s.MNU",dirx,hlp_bins->libs[s]);
				getmenubin(dira,tmpmenu);
				RDA_fprintf(fp,"Menu Name: %s\r\n",tmpmenu->name);
				if(RDAstrlen(tmpmenu->command)<67)
				{
					RDA_fprintf(fp,"     Command: %s\r\n\r\n",tmpmenu->command);
				} else {
					RDA_fprintf(fp,"     Command: \r\n");
					RDA_fprintf(fp,"              %s\r\n\r\n",tmpmenu->command);
					
				}
			}
		}
		RDA_pclose(fp);
		if(outdevice!=NULL) Rfree(outdevice); 
		if(dirx!=NULL) Rfree(dirx); 
		if(dira!=NULL) Rfree(dira); 
		if(tmpmenu!=NULL) free_menu(tmpmenu); 
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}
static void printmodulecb(RDArsrc *mainrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(!RDAstrcmp(hlp_bins->libs[1],"Directory Contains No Libraries")) return;
	if(!RDAstrcmp(hlp_defs->libs[1],"Contains No Menu Definitions")) return;
	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printmoduledefs,mainrsrc);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_moduledefs,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_moduledefs(prsrc);
	}
}
void printbin(RDArsrc *mtnrsrc)
{
	RDA_PFILE *fp1=NULL;
	char *libname=NULL,*sname=NULL,*desc=NULL;
	char *header=NULL,*menulib=NULL,*menuname=NULL,*menucomm=NULL;
	char *outdevice=NULL;

	outdevice=DefaultPrinter();
	readwidget(mtnrsrc,"MENU LIBRARY");
	readwidget(mtnrsrc,"MENU NAME");
	readwidget(mtnrsrc,"MENU COMMAND");
	FINDRSCGETSTRING(mtnrsrc,"MENU LIBRARY",&libname);
	FINDRSCGETSTRING(mtnrsrc,"MENU NAME",&sname);
	FINDRSCGETSTRING(mtnrsrc,"MENU COMMAND",&desc);
	header=Rmalloc(80);
	sprintf(header,"                              XPERT MENU DEFINITION                            ");
	menulib=Rmalloc(RDAstrlen(libname)+18);
	sprintf(menulib ,"MENU LIBRARY:  [%s]",libname);
	menuname=Rmalloc(RDAstrlen(sname)+18);
	sprintf(menuname,"MENU NAME:     [%s]",sname);
	menucomm=Rmalloc(RDAstrlen(desc)+18);
	sprintf(menucomm,"MENU COMMAND:  [%s]",desc);
	if((fp1=RDA_popen(outdevice,"w"))!=NULL)
	{
		RDA_fprintf(fp1,"%s\r\n%s\r\n%s\r\n%s\r\n\r\n",header,menulib,menuname,menucomm);
		RDA_pclose(fp1);
	}
	if(libname!=NULL) Rfree(libname);
	if(sname!=NULL) Rfree(sname);
	if(desc!=NULL) Rfree(desc);
	if(header!=NULL) Rfree(header);
	if(menulib!=NULL) Rfree(menulib);
	if(menuname!=NULL) Rfree(menuname);
	if(menucomm!=NULL) Rfree(menucomm);
}
void deletedfmenu(RDArsrc *mainrsrc)
/* This function is used to delete a menu item from a menu binary library */
{
	int l=0,s=0;
	char *libx=NULL,*directory=NULL;

	readallwidgets(mainrsrc); /* reads all widgets on mainrsrc screen */
	if(FINDRSCGETINT(mainrsrc,"MENU LIBRARIES",&l)) return;
		/* FIND RESOURCE GET INTEGER */
	if(FINDRSCGETINT(mainrsrc,"MENU LIST",&s)) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(!RDAstrcmp(hlp_defs->libs[s],"Contains No Menu Definitions")) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(hlp_bins->libs[l])+6);
		/* RDA's malloc of memory */
	sprintf(libx,"%s/%s.MNU",directory,hlp_bins->libs[l]);
	if(deletelibbin(libx,hlp_defs->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Help","The user cannot delete the help binary from the help binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(directory!=NULL) Rfree(directory);
		return;
	}
/* deletelibbin deletes a binary out of a binary library, this function is from binlib.h */
	if(hlp_defs!=NULL) { freeapplib(hlp_defs); hlp_defs=NULL; }
/* free's an APPlib structure allocated in memory */
	hlp_defs=getlibnames(libx,FALSE);
/* get's the names of the binary's in a binary library */
	if(hlp_defs==NULL)
	{
		hlp_defs=APPlibNEW(); 
/* Create's a New APPlib structure in memory */
		addAPPlib(hlp_defs,"Contains No Menu Definitions");
/* addAPPlib add's a string to an APPlib structure */
	}
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	/* FINDRSCLISTAPPlib FIND SCROLLED LIST RESOURCE and set list items and number of items with the APPlib structure hlp_defs and set the initial value in the list to the 1st item position 0. */
	if(s>=hlp_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MENU LIST",s,hlp_defs))
		updatersrc(mainrsrc,"MENU LIST");
	/* updatersrc Updates the window with the resource's new value */
}
void deletemenutest(RDArsrc *mainrsrc)
{
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE MENU DEFINITION!","Are you sure that you want to delete this MENU DEFINITION?",deletedfmenu,NULL,FALSE,mainrsrc);
}
void quitdfmenu(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc); /* free's a RDArsrc * structure in memory */
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(hlp_bins!=NULL) { freeapplib(hlp_bins); hlp_bins=NULL; } /* free's APPlib structure */
	if(hlp_defs!=NULL) { freeapplib(hlp_defs); hlp_defs=NULL; }
	ShutdownSubsystems();
}
void quitmenu(RDArsrc *mtnrsrc)
{
	if(menu!=NULL) free_menu(menu); /* free_menu free's an RDAmenu structre */
	if(mtnrsrc!=NULL) killwindow(mtnrsrc);
	if(mtndefaults!=NULL) FreeRDAdefault(mtndefaults);
	NULL_RSCS(mtnrsrc);
}
void quitmenutest(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,2))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this Menu Definition.\nExiting now without saving will loose these changes.",savemenu,quitmenu,FALSE,mtnrsrc);
	} else {
		quitmenu(mtnrsrc);
	}
}
void makemtnscreen(char *libname,char *sname,char *desc)
{
	char *tmp=NULL,*directory=NULL;

	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&tmp);
	directory=Rmalloc(RDAstrlen(tmp)+RDAstrlen(libname)+2);
#ifndef WIN32
	sprintf(directory,"%s/%s",tmp,libname);
#endif
#ifdef WIN32
	sprintf(directory,"%s\\%s",tmp,libname);
#endif
	FINDRSCSETSTRING(mtnrsrc,"MENU LIBRARY",(directory!=NULL ? directory:""));
		/* FIND RESOURCE SET STRING */
	FINDRSCSETSTRING(mtnrsrc,"MENU NAME",(sname!=NULL ? sname:""));
	FINDRSCSETSTRING(mtnrsrc,"MENU COMMAND",(desc!=NULL ? desc:""));
	mtndefaults=GetDefaults(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitmenu,NULL,FALSE);
}
void okdfmenu(RDArsrc *mainrsrc)
{
	int s=0,h=0;
	char *deff=NULL;
	char *dira=NULL,*dirx=NULL;

	readallwidgets(mainrsrc); /* read all widgets on a window for the mainrsrc RDArsrc structure */
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"MENU LIBRARIES",&s)) return;
		/* FIND RESOURCE GET INTEGER */
	if(FINDRSCGETINT(mainrsrc,"MENU LIST",&h)) return;
	dira=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[s])+6);
	sprintf(dira,"%s/%s.MNU",dirx,hlp_bins->libs[s]);
	if(RDAstrcmp(hlp_defs->libs[h],"Contains No Menu Definitions"))
	{
		deff=stralloc(hlp_defs->libs[h]);
		menu=RDAmenuNEW(deff,NULL); /* Allocate an RDAmenu * structure in memory */
		if(getmenubin(dira,menu))
		{
			ERRORDIALOG("Cannot Read Menu","Error the user cannot read the menu binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			if(deff!=NULL) Rfree(deff);
			if(menu!=NULL) free_menu(menu);
			if(dira!=NULL) Rfree(dira);
			if(dirx!=NULL) Rfree(dirx);
			return;
		}
	}  else menu=RDAmenuNEW(NULL,NULL);
	makemtnscreen(hlp_bins->libs[s],deff,menu->command);
		/* see above, makes a maintain window */
	if(deff!=NULL) Rfree(deff);
		/* RDA's free memory function */
	if(dira!=NULL) Rfree(dira);
	if(dirx!=NULL) Rfree(dirx);
}
void newdfmenu(RDArsrc *mainrsrc)
{
	int s=0;
	char *deflib=NULL;

	readallwidgets(mainrsrc); /* read all widgets on mainrsrc RDArsrc structure (window) */
	FINDRSCGETINT(mainrsrc,"MENU LIBRARIES",&s); 
		/* FIND RESOURCE GET INTEGER */
	if(RDAstrcmp(hlp_bins->libs[s],"Directory Contains No Libraries"))
	{
		deflib=stralloc(hlp_bins->libs[s]);
	} else deflib=stralloc(" ");
	menu=RDAmenuNEW(NULL,NULL); /* create a new RDAmenu structure in memory */
	makemtnscreen(deflib,NULL,NULL); /* see above makes a maintain window */
	if(deflib!=NULL) Rfree(deflib); /* RDA's free memory function */
}
void savemenu(RDArsrc *mtnrsrc)
{
	char *libname=NULL;
	char *deflib=NULL;

	readallwidgets(mtnrsrc); /* reads all resources on a RDArsrc structure */
	FINDRSCGETSTRING(mtnrsrc,"MENU COMMAND",&menu->command);
		/* FIND RESOURCE GET STRING */
	if(FINDRSCGETSTRING(mtnrsrc,"MENU NAME",&menu->name)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"MENU LIBRARY",&libname))
	{
		prterr("Error Couldn't write screen because the library name resource was not found.");
		return;
	}
	deflib=Rmalloc(RDAstrlen(libname)+6);
	sprintf(deflib,"%s.MNU",libname);
	if(writemenubin(deflib,menu))
	{
 /* writemenubin write's an RDAmenu structure to a menu binary library */
		ERRORDIALOG("Cannot Write Menu","Error user cannot write the menu definition into the library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
	}
	if(libname!=NULL) Rfree(libname);
		/* RDA's free memory */
	if(deflib!=NULL) Rfree(deflib);
	getdirectorymenu(); /* finds menu binary libraries in a directory */
	getmenuentries(mainrsrc); /* reads the binary library and display's contents */
	quitmenu(mtnrsrc); /* quit's the mtnrsrc window function */
} 
void getdirectorymenu()
/* reads the "rda" subdirectory below the current working directory for the menu binary libraries, they all have *.MNU conventions */
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL,*directory=NULL;

	FINDRSCGETINT(mainrsrc,"MENU LIBRARIES",&selectedm);
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(hlp_bins!=NULL) { freeapplib(hlp_bins); hlp_bins=NULL; }
	hlp_bins=APPlibNEW(); /* create's a new APPlib structure in memory */
	dirx=Rmalloc(RDAstrlen(directory)+2);
	sprintf(dirx,"%s/",directory);
	/* findfile finds file's in directory dirx, with patten *.MNU,write's the name in tmp the element number x+1 */
	/* addAPPlib adds the filename tmp to the hlp_bins APPlib structure */
	for(x=0;findfile(dirx,"*.MNU",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(!isEMPTY(tmp)) addAPPlib(hlp_bins,tmp);
	}
	if(hlp_bins->numlibs<1)
	{
		addAPPlib(hlp_bins,"Directory Contains No Libraries");
		/* adds a string to the APPlib structure */
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
		/* FIND LIST RESOURCE "MENU LIBRARIES" and set the list elements with an APPlib structure and the default position to 0 */
	SORTAPPlib(hlp_bins);
	if(selectedm>=hlp_bins->numlibs) selectedm=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MENU LIBRARIES",selectedm,hlp_bins))
		updatersrc(mainrsrc,"MENU LIBRARIES");
	if(directory!=NULL) Rfree(directory);
/* updates the resource "MENU LIBRARIES" on the mainrsrc window (RDArsrc structure) */
	getmenuentries(mainrsrc);
}
void getmenuentries(RDArsrc *mainrsrc)
/* this function read's the menu binary from "MENU LIBRARIES" resource and 
	displays the names of the menu's items within on the "MENU LIST" 
	resource */
{
	int selected,selectedl=0;
	char *libx=NULL,*dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	/* FIND RESOURCE GET INTEGER */
	if(FINDRSCGETINT(mainrsrc,"MENU LIBRARIES",&selected)) return;
	FINDRSCGETINT(mainrsrc,"MENU LIST",&selectedl);
	if(hlp_defs!=NULL) { freeapplib(hlp_defs);  hlp_defs=NULL; }/* free's an APPlib structure in memory */
	if(!RDAstrcmp(hlp_bins->libs[selected],"Directory Contains No Libraries"))
	{
		hlp_defs=APPlibNEW();
		addAPPlib(hlp_defs,"Contains No Menu Definitions");
		selectedl=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[selected])+6);
		sprintf(libx,"%s/%s.MNU",dirx,hlp_bins->libs[selected]);
		hlp_defs=getlibnames(libx,FALSE); /* get's the names of binaries contain in binary library libx */
		if(libx!=NULL) Rfree(libx); /* RDA's free memory */
		if(hlp_defs==NULL)
		{
			hlp_defs=APPlibNEW(); /* create's a new APPlib structure */
			addAPPlib(hlp_defs,"Contains No Menu Definitions"); /* add's an element to an APPlib structure */
			selectedl=0;
		}
	}
	/* FIND LIST RESOURCE and set list elements and number of elements with an APPlib structure and set the default position to 0 */
	if(selectedl>=hlp_defs->numlibs) selectedl=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MENU LIST",selectedl,hlp_defs))
		updatersrc(mainrsrc,"MENU LIST");
	/* update the MENU LIST resource on the mainrsrc window */
	if(dirx!=NULL) Rfree(dirx);
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory)) 
		{
			inlist=TRUE;		
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
		updatersrc(mainrsrc,"DIRECTORY LIST");
	getdirectorymenu();
/*
	getmenuentries(mainrsrc);
*/
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s])) 
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s])) return;
			updatersrc(mainrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getdirectorymenu();
}
void runcommand(RDArsrc *mainrsrc,char *command)
{
	short x;
	char *errdesc=NULL,*cmd=NULL,*tmp=NULL;
	int offset=0;
	char *srchstr=".EXT";
#if defined(RS6000)
	char *repstr=".rsc";
#elif defined(LINUX)
	char *repstr=".lnx";
#elif defined(LINUX2_2) || defined(UBUNTU_OS)
	char *repstr=".lnx";
#elif defined(UNIXWARE7)
	char *repstr=".unw";
#elif defined(UNIXWARE21)
	char *repstr=".unw";
#elif defined(SCO386)
	char *repstr=".sco";
#elif defined(SCO50)
	char *repstr=".sco";
#elif defined(SCO50P)
	char *repstr=".sco";
#elif defined(INT386)
	char *repstr=".unx";
#elif defined(SUNX86)
	char *repstr=".sun";
#elif defined(WIN32)
	char *repstr=".exe";
	char *repstr1=".bat";
#else 
	char *repstr=".EXT";
#endif

#ifndef WIN32
	cmd=Rmalloc(RDAstrlen(command)+3);
	sprintf(cmd,"%s &",command);
#endif
#ifdef WIN32
	cmd=stralloc(command);
#endif
	tmp=RDAstrstr(cmd,srchstr);
	if(tmp!=NULL)
	{
		offset=((int)(tmp-cmd));
		for(x=0;x<(RDAstrlen(srchstr));++x)
		{
			cmd[x+offset]=repstr[x];
		}
		
	}
	x=system_command(cmd);
#ifdef WIN32
	if(x)
	{
		if(cmd!=NULL) Rfree(cmd);
		cmd=stralloc(command);
		tmp=RDAstrstr(cmd,srchstr);
		if(tmp!=NULL)
		{
			offset=((int)(tmp-cmd));
			for(x=0;x<(RDAstrlen(srchstr));++x)
			{
				cmd[x+offset]=repstr1[x];
			}
			
		}
		x=system_command(cmd);
		if(x)
		{
			errdesc=Rmalloc(RDAstrlen(cmd)+87);
			sprintf(errdesc,"The command [%s] won't execute. Check your syntax and/or try executing it at the command prompt.",cmd);
			ERRORDIALOG("COMMAND FAILED",errdesc,NULL,FALSE);
			if(errdesc!=NULL) Rfree(errdesc);
		}
	}
#endif
#ifndef WIN32
	if(x)
	{
		errdesc=Rmalloc(RDAstrlen(cmd)+87);
		sprintf(errdesc,"The command [%s] won't execute. Check your syntax and/or try executing it at the command prompt.",cmd);
		ERRORDIALOG("COMMAND FAILED",errdesc,NULL,FALSE);
		if(errdesc!=NULL) Rfree(errdesc);
	}
#endif
	if(cmd!=NULL) Rfree(cmd);
}
static void domenu(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	RDAmenu *tmpmenu=NULL;
	char *dirx=NULL,*dira=NULL;
/*
	char *errdesc=NULL;
	short x;
*/

	if(hlp_defs->numlibs==1 && !RDAstrcmp(hlp_defs->libs[0],"Contains No Menu Definitions")) return;
	if(FINDRSCGETINT(mainrsrc,"MENU LIBRARIES",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"MENU LIST",&selected)) return;
	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	tmpmenu=RDAmenuNEW(hlp_defs->libs[selected],NULL);
	dira=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[selectedm])+6);
	sprintf(dira,"%s/%s.MNU",dirx,hlp_bins->libs[selectedm]);
	getmenubin(dira,tmpmenu);
	if(!isEMPTY(tmpmenu->command))
	{
		runcommand(mainrsrc,tmpmenu->command);
	}
/*
		x=system_command(tmpmenu->command);
		if(x)
		{
			errdesc=Rmalloc(RDAstrlen(tmpmenu->command)+97);
			sprintf(errdesc,"The command [%s] won't execute. Check your syntax and/or try executing it at the command prompt.",tmpmenu->command);
			ERRORDIALOG("COMMAND FAILED!",errdesc,NULL,FALSE);
			if(errdesc!=NULL) Rfree(errdesc);
		}
	} else {
		errdesc=Rmalloc(87);
		sprintf(errdesc,"The command [] won't execute. Check your syntax and/or try executing it at the command prompt.");
		ERRORDIALOG("COMMAND FAILED!",errdesc,NULL,FALSE);
		if(errdesc!=NULL) Rfree(errdesc);
	}
*/
	if(tmpmenu!=NULL) free_menu(tmpmenu);
}
static void dfscn(RDArsrc *r)
{
	ExecuteProgram("dfscn",NULL);
}
static void addmenu(RDArsrc *r)
{
	APPlib *args=NULL;
	int h=0;
	
	if(FINDRSCGETINT(mainrsrc,"MENU LIST",&h)) return;
	args=APPlibNEW();
	addAPPlib(args,hlp_defs->libs[h]);	
	ExecuteProgram("addmenu",args);
	if(args!=NULL) freeapplib(args);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0,w=2,m=0,s=0;
	char *defdir=NULL;

	/* InitializeSubsystems Initailizes the Security, Help, Database, and test's the module and process level security */
	if(InitializeSubsystems(argc,argv,module,"DEFINE MENU")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"DEFINE MENU"); /* create a new RDArsrc structure in memory */
	mtnrsrc=RDArsrcNEW(module,"MAINTAIN MENU");
	hlp_bins=APPlibNEW(); /* create a new APPlib structure in memory */
	hlp_defs=APPlibNEW(); /* create a new APPlib structure in memory */
	dir_libs=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	addlstrsrc(mainrsrc,"DIRECTORY LIST",&w,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[w],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",0,dir_libs);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist,NULL);
	setdirlist(mainrsrc);
	/* addlstrsrc create a list resource on an RDArsrc structure */
	addlstrsrc(mainrsrc,"MENU LIBRARIES",&x,TRUE,getmenuentries,
		hlp_bins->numlibs,&hlp_bins->libs,NULL);
	getdirectorymenu();
	addlstrsrc(mainrsrc,"MENU LIST",&x,TRUE,NULL,hlp_defs->numlibs,
		&hlp_defs->libs,NULL);
	getmenuentries(mainrsrc);
	if(argc>1 && hlp_bins!=NULL)
	{
		for(m=0;m<hlp_bins->numlibs;++m)
		{
			if(!RDAstrcmp(hlp_bins->libs[m],argv[1])) break;
		}
		FINDRSCSETINT(mainrsrc,"MENU LIBRARIES",m);
		getmenuentries(mainrsrc);
	}
	if(argc>2 && hlp_defs!=NULL)
	{
		for(s=0;s<hlp_defs->numlibs;++s)
		{
			if(!RDAstrcmp(hlp_defs->libs[s],argv[2])) break;
		}
		FINDRSCSETINT(mainrsrc,"MENU LIST",s);
	}
	/* addbtnrsrc Add a Button resource on an RDArsrc structure */
	addbtnrsrc(mainrsrc,"ADD",TRUE,newdfmenu,NULL);
	addbtnrsrc(mainrsrc,"RUN MENU",TRUE,domenu,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okdfmenu,NULL);
	addbtnrsrc(mainrsrc,"DELETE MENU",TRUE,deletemenutest,NULL);
	/* addrfexrsrc add a button resource that will kill the window and exit the program upon completion of it's function quitdfmenu */
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfmenu,NULL);
	/* addrfcbrsrc is the same as addbtnrsrc */
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfcbrsrc(mainrsrc,"PRINT DEFINITIONS",TRUE,printmodulecb,NULL);
	addstdrsrc(mtnrsrc,"MENU LIBRARY",VARIABLETEXT,80,NULL,TRUE);
	addstdrsrc(mtnrsrc,"MENU NAME",VARIABLETEXT,80,NULL,TRUE);
	/* addsctrsrc Add Scrolled Text Resource to the RDArsrc structure */
	addsctrsrc(mtnrsrc,"MENU COMMAND",0,NULL,TRUE);
	/* addrfkwrsrc add a button resource that will kill the window upon completion of it's function */
	addbtnrsrc(mainrsrc,"DFSCN",TRUE,dfscn,NULL);
	addbtnrsrc(mainrsrc,"ADDMENU",TRUE,addmenu,NULL);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,savemenu,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quitmenutest,NULL);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(mtnrsrc,"PRINT DEFINITION",TRUE,printbin,NULL);
	FINDRSCSETINPUTFOCUS(mainrsrc,"MENU LIBRARIES");
	APPmakescrn(mainrsrc,TRUE,quitdfmenu,NULL,TRUE);
}
