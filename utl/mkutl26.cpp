/* UTILITIES's - Make (HTTP PROXY) List */
#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void GSV_HTTP_PROXY()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","HTTP PROXY");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=40;
			gsv->desc=stralloc("");
			gsv->label=stralloc("HTTP Proxy");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(10+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [HTTP PROXY], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_HTTP_PROXY_PORT()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","HTTP PROXY PORT");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=8;
			gsv->length=4;
			gsv->desc=stralloc("");
			gsv->label=stralloc("Proxy Port");
			gsv->value.integer_value=Rmalloc(sizeof(int));
			*gsv->value.integer_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(15+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [HTTP PROXY PORT], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_HTTP_USE_PROXY()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","HTTP USE PROXY");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("");
			gsv->label=stralloc("Use HTTP Proxy");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(14+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [HTTP USE PROXY], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void SCN_GENERIC_SETUP_VARIABLES()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","GENERIC SETUP VARIABLES");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Installation Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(5+1);
		sprintf(temp2,"%s",
			"FALSE");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"INSTALLATION DATE","Installation Date","","",0,10,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","Compile Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(5+1);
		sprintf(temp2,"%s",
			"FALSE");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"COMPILE DATE","Compile Date","","",0,10,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Support","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   CyberSupport Web Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WEB ID","CyberSupport Web Identification:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Customer Care: support-srv.rdasys.com","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","IP Address of RDA's Support Server:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RDA SUPPORT IP ADDRESS","RDA Support IP Address:","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"AUTOMATICALLY SUBMIT ERRORS AS HELPDESK TICKETS","Submit Errors as Help Desk Tickets via CyberSupport","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CHECK FOR UPDATES","Check for Updates","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"BALANCE SHEET INFO NOT EDITBALE","Balance Sheet Information Not Editable","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Attributes","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Organization Logo:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ORGANIZATION LOGO","Organization Logo","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","[Filename of XPixmap (xpm)]","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CENTER WINDOWS","Center Windows on Display","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  MSMTP Arguments:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MSMTP CONFIGURATION ARGUMENTS","MSMTP Arguments:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"USE SPEECH SYNTHESIZER","Use Speech Synthesizer (Festival)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Used to denote when to print the Range Screen selection critiera for Simple Printer style reports.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"PRINT RANGE CRITERIA","Print Range Criteria","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","(Shown at end of report on separate page.)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DEFAULT DUPLEX PRINTING","Default Duplex Printing","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DOMENU STYLE BLOCKS TAB BAR ON DOCK WINDOW","Use [DOMENU STYLE] to block the TAB BAR on Dock Windows","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Browsing","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Browser Types:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSER TYPES","Browser Types","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","URL when No URL:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"URL NO URL","URL when No URL:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","CD Writing","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Temporary Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TEMPORARY DIRECTORY","Temporary Directory:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FOR USE WITH CDRW:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","                Bus:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CDRW BUS","Bus:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Speed:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CDRW SPEED","Speed:","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CDRW DEFAULT BLANK","Blank Used CDRW by Default","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   CRON Tape Device:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CRON TAPE DEVICE","CRON Tape Device:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","HTTP Proxy","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FOR USE WITH WWW:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"HTTP USE PROXY","Use HTTP Proxy","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","HTTP Proxy:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY","HTTP PROXY","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Port:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY PORT","HTTP PROXY PORT","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"FTP USE PROXY","Use FTP Proxy","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," FTP Proxy:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FTP PROXY","FTP PROXY","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Port:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FTP PROXY PORT","FTP PROXY PORT","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Menu Config","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Used to denote whether or not to keep the main menu alive after launch of another menu.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"KEEP ALIVE MAIN MENU","Keep Alive Main Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","(This is applicable only to Simple Menu.)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DISPLAY CONVERSION MENU","Display Conversion Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"USE CUSTOM MENU","Display \"Custom Menu\" off the Main Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		if(XPERT_SETUP->software_type<2)
		{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"USE SIMPLIFIED PROCESSES","Use Simplified / Combination Procedures","","",0,0,0,NULL,"([DEV_LICENSE]=TRUE) AND ([ARCHIVE SETUP]=TRUE)",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"USE SIMPLIFIED MENUS","Use Simplified Menus","","",0,0,0,NULL,"([DEV_LICENSE]=TRUE)",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		}
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DISPLAY RPTGEN ON MAIN MENU","Display \"Report Generator\" off the Main Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SKIP DOMENU TRANSVERSALS","Skip Menu Transversals","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		if(XPERT_SETUP->software_type<2)
		{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"FIRST DEMONSTRATION","First Demonstration","","",0,0,0,NULL,"([DEV_LICENSE]=TRUE)",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#ifdef _NOT_WANTED_
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DISPLAY TOOLBAR","Display ToolBar (OpenRDA 2.0 Only)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#endif
		}
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","OpenRDA Groups","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Optional Module Group #1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"OPTIONAL MODULE GROUP 1","Optional Module Group 1","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Optional Module Group #2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"OPTIONAL MODULE GROUP 2","Optional Module Group 2","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Optional Module Group #3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"OPTIONAL MODULE GROUP 3","Optional Module Group 3","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Optional Module Group #4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"OPTIONAL MODULE GROUP 4","Optional Module Group 4","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [GENERIC SETUP VARIABLES], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void MNU_UTILITIES_DOGSV()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("UTILITIES DOGSV","dogsv.EXT UTILITIES");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
static void GSV_CDRW_BUS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CDRW BUS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=15;
			gsv->desc=stralloc("");
			gsv->label=stralloc("CDRW BUS:");
			gsv->value.string_value=stralloc("0,0,0");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(8+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [CDRW BUS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_CDRW_DEFAULT_BLANK()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CDRW DEFAULT BLANK");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("");
			gsv->label=stralloc("Default Blank:");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(18+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [CDRW DEFAULT BLANK], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_CDRW_SPEED()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CDRW SPEED");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=7;
			gsv->length=2;
			gsv->desc=stralloc("");
			gsv->label=stralloc("CDRW BUS:");
			gsv->value.short_value=Rmalloc(sizeof(short));
			*gsv->value.short_value=16;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(10+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [CDRW SPEED], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_WEB_ID()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","WEB ID");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=15;
			gsv->desc=stralloc("");
			gsv->label=stralloc("CyberSupport Web Identification:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(6+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [WEB ID], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_FTP_PROXY()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","FTP PROXY");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=40;
			gsv->desc=stralloc("");
			gsv->label=stralloc("FTP Proxy");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(9+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [FTP PROXY], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_FTP_PROXY_PORT()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","FTP PROXY PORT");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=8;
			gsv->length=4;
			gsv->desc=stralloc("");
			gsv->label=stralloc("Proxy Port");
			gsv->value.integer_value=Rmalloc(sizeof(int));
			*gsv->value.integer_value=80;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(14+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [FTP PROXY PORT], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_FTP_USE_PROXY()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","FTP USE PROXY");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("");
			gsv->label=stralloc("Use FTP Proxy");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(13+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [FTP USE PROXY], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_CRON_TAPE_DEVICE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CRON TAPE DEVICE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=40;
			gsv->desc=stralloc("");
			gsv->label=stralloc("Cron Tape Device:");
#if !defined(LINUX2_2) || !defined(UBUNTU_OS)
			gsv->value.string_value=stralloc("/dev/st0");
#endif
#ifdef LINUX
			gsv->value.string_value=stralloc("/dev/st0");
#endif
#if !defined(LINUX2_2) || !defined(LINUX) || !defined(UBUNTU_OS)
			gsv->value.string_value=stralloc("");
#endif
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(16+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [CRON TAPE DEVICE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_TEMPORARY_DIRECTORY()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","TEMPORARY DIRECTORY");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=60;
			gsv->desc=stralloc("");
			gsv->label=stralloc("Temporary Directory:");
#ifdef WIN32
			gsv->value.string_value=stralloc("C:\\TEMP\\rdacddir");
#endif
#ifndef WIN32
			gsv->value.string_value=stralloc("/tmp/rdacddir");
#endif
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(19+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [TEMPORARY DIRECTORY], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_BROWSE_TYPE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","BROWSER TYPE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=7;
			gsv->length=2;
			gsv->desc=stralloc("Used to denote which browser type you wich to install.  Different options are available with other OS Types.");
			gsv->label=stralloc("Browser Type:");
			gsv->value.short_value=Rmalloc(sizeof(short));
			*gsv->value.short_value=1;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(11+0+111+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [BROWSER TYPE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void LST_BROWSE_TYPES()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAScrolledListNew("UTILITIES","BROWSER TYPES");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=1;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		scrn->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->special_function=stralloc("");
		scrn->num=0;
		scrn->initfld=NULL;
		scrn->ffield=stralloc("");
		scrn->ffinfo=Rmalloc(sizeof(DFincvir));
		scrn->ffinfo->module=stralloc("");
		scrn->ffinfo->file=stralloc("");
		scrn->ffinfo->keyname=stralloc("");
		scrn->contype=1;
		scrn->conmod=stralloc("BROWSER TYPE");
		scrn->confil=stralloc("");
		scrn->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		scrn->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		scrn->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		scrn->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->desc=stralloc("");
		scrn->timing=0;
		scrn->list=APPlibNEW();
#ifndef WIN32
		addAPPlib(scrn->list,"Opera");
		addAPPlib(scrn->list,"Mozilla");
		addAPPlib(scrn->list,"FireFox");
#endif
#ifdef WIN32
		addAPPlib(scrn->list,"Netscape");
		addAPPlib(scrn->list,"Mozilla");
		addAPPlib(scrn->list,"Internet Explorer");
		addAPPlib(scrn->list,"FireFox");
#endif

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+9+101+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [UTILITIES] RDAScrolledList [BROWSER TYPES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDAScrolledList(scrn);
	}
}
static void GSV_DISPLAY_CONVERSION_MENU()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","DISPLAY CONVERSION MENU");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Used to denote when to display the CONVERSION MENU on the SYSTEM ADMINISTRATION MENU.");
			gsv->label=stralloc("Display Conversion Menu");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(23+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [DISPLAY CONVERSION MENU], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_MSMTP_CONFIGURATION_ARGUMENTS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","MSMTP CONFIGURATION ARGUMENTS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=0;
			gsv->length=750;
			gsv->desc=stralloc("This will store the msmtp arguments which are specific to your site's email configuration.");
			gsv->label=stralloc("MSMTP Configuration Arguments:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(29+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [MSMTP CONFIGURATION ARGUMENTS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_USE_SPEECH_SYNTHESIZER()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","USE SPEECH SYNTHESIZER");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Denotes the use of the Speech Synthesizer and (Festival) as an audio aid to software implementation.");
			gsv->label=stralloc("Use Speech Synthesizer");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(22+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [USE SPEECH SYNTHESIZER], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_KEEP_ALIVE_MAIN_MENU()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","KEEP ALIVE MAIN MENU");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Used to denote whether or not to keep the main menu alive after launch of another menu.  This is applicable only to Simple Menu.");
			gsv->label=stralloc("KEEP ALIVE MAIN MENU");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=TRUE;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(20+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [KEEP ALIVE MAIN MENU], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_PRINT_RANGE_CRITERIA()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","PRINT RANGE CRITERIA");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Print Range Criteria on a separate page in the report footer for simple printer report styles.");
			gsv->label=stralloc("Print Range Criteria");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(20+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [PRINT RANGE CRITERIA], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_RDA_SUPPORT_IP_ADDRESS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","RDA SUPPORT IP ADDRESS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=20;
			gsv->desc=stralloc("The IP Address of RDA's Support Server.");
			gsv->label=stralloc("RDA's Support IP Address:");
			gsv->value.string_value=stralloc("support-srv.rdasys.com");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(22+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [RDA SUPPORT IP ADDRESS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_URL_NO_URL()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","URL NO URL");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=0;
			gsv->length=0;
			gsv->desc=stralloc("URL to use when no URL is available.");
			gsv->label=stralloc("URL to USE when not Specified:");
			gsv->value.string_value=stralloc("www.google.com/search");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(10+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [URL NO URL], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_AUTOMATICALLY_SUBMIT_ERRORS_AS_HELPDESK_TICKETS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","AUTOMATICALLY SUBMIT ERRORS AS HELPDESK TICKETS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("This boolean is used to denote when you wish to automatically submit errors as Help Desk Tickets via CyberSupport.  This will enable RDA Staff to take proactive measures on your behalf.");
			gsv->label=stralloc("Automatically Submit Errors as Help Desk Tickets via CyberSupport");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=TRUE;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(47+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [AUTOMATICALLY SUBMIT ERRORS AS HELPDESK TICKETS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_BALANCE_SHEET_INFO_NOT_EDITBALE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","BALANCE SHEET INFO NOT EDITBALE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("This boolean is used to denote when you balance sheet info is editable or not.");
			gsv->label=stralloc("Balance Sheet Info Not Editable ");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=FALSE;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(47+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [AUTOMATICALLY SUBMIT ERRORS AS HELPDESK TICKETS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_CHECK_FOR_UPDATES()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CHECK FOR UPDATES");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("This boolean is used to denote when you wish to automatically check for updates and display status on a modules main menu.");
			gsv->label=stralloc("Check for Updates");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=1;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(17+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [CHECK FOR UPDATES], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_USE_CUSTOM_MENU()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","USE CUSTOM MENU");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Denotes whether or not to use the use Custom Menu displayed on the Main Menu.");
			gsv->label=stralloc("Use Custom Menu");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(15+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [USE CUSTOM MENU], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_USE_SIMPLIFIED_PROCESSES()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","USE SIMPLIFIED PROCESSES");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Denotes when the Organization is going to use our Simplified Processes.");
			gsv->label=stralloc("Use Simplified Processes");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(24+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [USE SIMPLIFIED PROCESSES], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_USE_SIMPLE_MENU()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","USE SIMPLIFIED MENUS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Denotes when customers want to implement our new simple menu's.");
			gsv->label=stralloc("Use Simple Menus");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(15+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [USE SIMPLE MENU], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_DISPLAY_RPTGEN_ON_MAIN_MENU()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","DISPLAY RPTGEN ON MAIN MENU");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Used to denote when the Report Generator menu is presented off the Main Menu.");
			gsv->label=stralloc("Display Report Generator Menu");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=TRUE;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(27+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [DISPLAY RPTGEN ON MAIN MENU], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_SKIP_DOMENU_TRANSVERSALS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","SKIP DOMENU TRANSVERSALS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Used to denote when a customer wishes to skip transversals on menus.");
			gsv->label=stralloc("Skip Domenu Transversals");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(24+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [SKIP DOMENU TRANSVERSALS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_ORGANIZATION_LOGO()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","ORGANIZATION LOGO");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=0;
			gsv->length=0;
			gsv->desc=stralloc("Denotes the name of the organization's logo for displaying on menus.");
			gsv->label=stralloc("Organization Logo");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(17+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [ORGANIZATION LOGO], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_CENTER_WINDOWS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CENTER WINDOWS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Center Windows on the Display as they are created.");
			gsv->label=stralloc("Center Windows");
			gsv->value.string_value=Rmalloc(1);
#ifndef __GTK_H__
			*gsv->value.string_value=TRUE;
#else
			*gsv->value.string_value=0;
#endif /* __GTK_H__ */
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(14+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [CENTER WINDOWS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_SWB_S1_FAST()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","SWB_S1_FAST");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("FAST on Menu Bar System Wide Bundle");
			gsv->label=stralloc("S1 FAST");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=(XPERT_SETUP->software_type==0 ? TRUE:FALSE);
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(11+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [SWB_S1_FAST], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_SWB_S2_INTERNET_ENABLED()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","SWB_S2_INTERNET_ENABLED");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Internet Enabled System Wide Bundle");
			gsv->label=stralloc("S2 Internet Enabled");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=(XPERT_SETUP->software_type==0 ? TRUE:FALSE);
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(23+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [SWB_S2_INTERNET_ENABLED], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_SWB_S3_CLEARING_FUND()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","SWB_S3_CLEARING_FUND");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Clearing Fund System Wide Bundle");
			gsv->label=stralloc("S3 Clearing fund");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=(XPERT_SETUP->software_type==0 ? TRUE:FALSE);
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(20+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [SWB_S3_CLEARING_FUND], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_SWB_S4_MULTIPLE_DEFINITION_TYPES()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","SWB_S4_MULTIPLE_DEFINITION_TYPES");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Multiple Definition Types System Wide Bundle");
			gsv->label=stralloc("S4 Multiple Definition Types");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=(XPERT_SETUP->software_type==0 ? TRUE:FALSE);
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(32+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [SWB_S4_MULTIPLE_DEFINITION_TYPES], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_SWB_S5_GROUP_DEFAULTS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","SWB_S5_GROUP_DEFAULTS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Group Defaults System Wide Bundle");
			gsv->label=stralloc("S5 Group Defaults");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=(XPERT_SETUP->software_type==0 ? TRUE:FALSE);
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(21+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [SWB_S5_GROUP_DEFAULTS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void SCN_SYSTEM_WIDE_BUNDLES()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","SYSTEM WIDE BUNDLES");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(20+1);
		sprintf(temp2,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SWB_S1_FAST","S1 FAST","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","(Implements the FAST Functions on Primary ToolBar)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(20+1);
		sprintf(temp2,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SWB_S2_INTERNET_ENABLED","S2 Internet Enabled","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","(Enables Internet Searching, Maps, Geo Codes, etc)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(20+1);
		sprintf(temp2,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SWB_S3_CLEARING_FUND","S3 Clearing Fund","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","(Enables the use of clearing funds where applicable)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(20+1);
		sprintf(temp2,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SWB_S4_MULTIPLE_DEFINITION_TYPES","S4 Multiple Definition Types","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","(Manages multiple account code structures simultaneously)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(20+1);
		sprintf(temp2,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SWB_S5_GROUP_DEFAULTS","S5 Group Defaults","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","(Enables Group Defaults on Range Screens, etc)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(20+1);
		sprintf(temp2,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"SWB_S6_CHART_DATABASE","S6 Chart Database","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","(Enables implements the chartdat program for pie, vertical bar and horizontal bar charts)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(20+1);
		sprintf(temp1,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [SYSTEM WIDE BUNDLES], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void GSV_SWB_S6_CHART_DATABASE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","SWB_S6_CHART_DATABASE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Chart Database System Wide Bundle");
			gsv->label=stralloc("S6 Chart Database");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(21+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [SWB_S6_CHART_DATABASE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_BALANCE_SHEET_INFO_EDIT_FORMULA()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","BALANCE SHEET INFO EDIT FORMULA");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=14;
			gsv->length=0;
			gsv->desc=stralloc("This generic setup variable to make balance sheet info editbale");
			gsv->label=stralloc("Balance Sheet Info Editable");
			gsv->value.string_value=stralloc("TRUE");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(31+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [BALANCE SHEET INFO EDIT FORMULA], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_OPTIONAL_MODULE_GROUP_1()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 1");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=25;
			gsv->desc=stralloc("Denotes the first optional module group used to contain modules within OpenRDA.");
			gsv->label=stralloc("Optional Module Group #1:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(23+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [OPTIONAL MODULE GROUP 1], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_OPTIONAL_MODULE_GROUP_2()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 2");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=25;
			gsv->desc=stralloc("Denotes the second optional module group used to contain modules within OpenRDA.");
			gsv->label=stralloc("Optional Module Group #2:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(23+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [OPTIONAL MODULE GROUP 2], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_OPTIONAL_MODULE_GROUP_3()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 3");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=25;
			gsv->desc=stralloc("Denotes the third optional module group used to contain modules within OpenRDA.");
			gsv->label=stralloc("Optional Module Group #3:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(23+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [OPTIONAL MODULE GROUP 3], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_OPTIONAL_MODULE_GROUP_4()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 4");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=25;
			gsv->desc=stralloc("Denotes the fourth optional module group used to contain modules within OpenRDA.");
			gsv->label=stralloc("Optional Module Group #4:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(23+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [OPTIONAL MODULE GROUP 4], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_FIRST_DEMONSTRATION()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","FIRST DEMONSTRATION");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("This boolean is used to denote when performing a demonstration for the first time.  This will remove convenience items from the menus, ensuring they do not appear complicated.");
			gsv->label=stralloc("First Demonstration");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(19+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [FIRST DEMONSTRATION], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_DEFAULT_DUPLEX_PRINTING()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","DEFAULT DUPLEX PRINTING");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("This generic setup variable is used to determine when users want to their reports to default to duplex printing.");
			gsv->label=stralloc("Default Duplex Printing");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(23+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [DEFAULT DUPLEX PRINTING], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_DOMENU_STYLE_BLOCKS_TAB_BAR_ON_DOCK_WINDOW()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("UTILITIES","DOMENU STYLE BLOCKS TAB BAR ON DOCK WINDOW");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("This generic setup variable is used to denote when [DOMENU STYLE]!=0 is blocked from viewing the Tab Bar's on Dock Windows.");
			gsv->label=stralloc("DOMENU STYLE BLOCKS TAB BAR ON DOCK WINDOW");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(42+9+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [UTILITIES] GSV [DOMENU STYLE BLOCKS TAB BAR ON DOCK WINDOW], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
void UTLGSV()
{
	GSV_HTTP_PROXY();
	GSV_HTTP_PROXY_PORT();
	GSV_HTTP_USE_PROXY();
	SCN_GENERIC_SETUP_VARIABLES();
	MNU_UTILITIES_DOGSV();
	GSV_CDRW_BUS();
	GSV_CDRW_DEFAULT_BLANK();
	GSV_CDRW_SPEED();
	GSV_WEB_ID();
	GSV_FTP_PROXY();
	GSV_FTP_PROXY_PORT();
	GSV_FTP_USE_PROXY();
	GSV_CRON_TAPE_DEVICE();
	GSV_TEMPORARY_DIRECTORY();
	GSV_BROWSE_TYPE();
	LST_BROWSE_TYPES();
	GSV_DISPLAY_CONVERSION_MENU();
	GSV_MSMTP_CONFIGURATION_ARGUMENTS();
	GSV_USE_SPEECH_SYNTHESIZER();
	GSV_KEEP_ALIVE_MAIN_MENU();
	GSV_PRINT_RANGE_CRITERIA();
	GSV_RDA_SUPPORT_IP_ADDRESS();
	GSV_URL_NO_URL();
	GSV_AUTOMATICALLY_SUBMIT_ERRORS_AS_HELPDESK_TICKETS();
	GSV_CHECK_FOR_UPDATES();
	GSV_USE_CUSTOM_MENU();
	GSV_USE_SIMPLIFIED_PROCESSES();
	GSV_USE_SIMPLE_MENU();
	GSV_DISPLAY_RPTGEN_ON_MAIN_MENU();
	GSV_SKIP_DOMENU_TRANSVERSALS();
	GSV_ORGANIZATION_LOGO();
	GSV_CENTER_WINDOWS();
	GSV_DEFAULT_DUPLEX_PRINTING();
	if(XPERT_SETUP->software_type<2)
	{
		GSV_DOMENU_STYLE_BLOCKS_TAB_BAR_ON_DOCK_WINDOW();
		GSV_SWB_S1_FAST();
		GSV_SWB_S2_INTERNET_ENABLED();
		GSV_SWB_S3_CLEARING_FUND();
		GSV_SWB_S4_MULTIPLE_DEFINITION_TYPES();
		GSV_SWB_S5_GROUP_DEFAULTS();
		GSV_SWB_S6_CHART_DATABASE();
		SCN_SYSTEM_WIDE_BUNDLES();
		GSV_BALANCE_SHEET_INFO_EDIT_FORMULA();
		GSV_BALANCE_SHEET_INFO_NOT_EDITBALE();
		GSV_OPTIONAL_MODULE_GROUP_1();
		GSV_OPTIONAL_MODULE_GROUP_2();
		GSV_OPTIONAL_MODULE_GROUP_3();
		GSV_OPTIONAL_MODULE_GROUP_4();
		GSV_FIRST_DEMONSTRATION();
	}
}
