/* DATABASE's - Make (MYSQL SERVER DATABASE) List */
#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void GSV_MYSQL_USERNAME()
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

	gsv=RDAGenericSetupNew("DATABASE","MYSQL USERNAME");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=80;
			gsv->desc=stralloc("Used to Represent the MySQL Server Username where the RDA Database resides.");
			gsv->label=stralloc("MySQL Username:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(14+8+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [DATABASE] GSV [MYSQL USERNAME], Can Not Save Generic Setup Variable!");
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
static void GSV_MYSQL_PASSWORD()
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

	gsv=RDAGenericSetupNew("DATABASE","MYSQL PASSWORD");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=80;
			gsv->desc=stralloc("Used to Represent the MySQL Server Password where the RDA Database resides.");
			gsv->label=stralloc("MySQL Password:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(14+8+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [DATABASE] GSV [MYSQL PASSWORD], Can Not Save Generic Setup Variable!");
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
static void MNU_DOGSV_DATABASE()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("DOGSV DATABASE","dogsv.EXT DATABASE");
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
static void SCN_GENERIC_SETUP_VARIABLES()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","GENERIC SETUP VARIABLES");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","MySQL Server Hostname, Username and Password where the RDA Database resides.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","(These only work with the MySQL Distribution.)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","MySQL Database:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","MySQL Username:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","MySQL Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MYSQL DATABASE","MYSQL DATABASE","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MYSQL USERNAME","MYSQL USERNAME","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MYSQL PASSWORD","MYSQL PASSWORD","","",0,25,3,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"MYSQL CASE SENSITIVE COLLATION","MySQL Case Sensitive Collation","","",0,0,0,NULL,"([DEV_LICENSE]=TRUE)",NULL,NULL);
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
			temp1=Rmalloc(23+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [GENERIC SETUP VARIABLES], Can Not Save Maintain Master!");
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
static void GSV_MYSQL_DATABASE()
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

	gsv=RDAGenericSetupNew("DATABASE","MYSQL DATABASE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=80;
			gsv->desc=stralloc("Used to Represent the MySQL Database where the RDA Tables reside.");
			gsv->label=stralloc("MySQL Database Name:");
			gsv->value.string_value=stralloc("");
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(14+8+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [DATABASE] GSV [MYSQL DATABASE], Can Not Save Generic Setup Variable!");
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
static void GSV_MYSQL_CASE_SENSITIVE_COLLATION()
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

	gsv=RDAGenericSetupNew("DATABASE","MYSQL CASE SENSITIVE COLLATION");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Used to denote when Case Sensitive Collation should be implemented while building indexes.");
			gsv->label=stralloc("MySQL Case Sensitive Collation");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=1;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(30+8+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [DATABASE] GSV [MYSQL CASE SENSITIVE COLLATION], Can Not Save Generic Setup Variable!");
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
void DFGSV_DATABASE()
{
	GSV_MYSQL_USERNAME();
	GSV_MYSQL_PASSWORD();
	MNU_DOGSV_DATABASE();
	SCN_GENERIC_SETUP_VARIABLES();
	GSV_MYSQL_DATABASE();
	GSV_MYSQL_CASE_SENSITIVE_COLLATION();
}
