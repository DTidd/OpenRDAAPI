/* UTILITIES's - Make (MENUITEM-EXPORT) Import/Export */
#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>
#include <mkimp.hpp>

#ifdef USING_QT
extern char *module;
extern APPlib *errorlist;

static void IMPEXP_MENUITEM_EXPORT()
{
	IEfile *IMPEXP=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");

	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");

#ifdef WIN32
	IMPEXP=IMPEXPdefNEW("MENUITEM-EXPORT","UTILITIES","MENUITEM","MENUITEM.csv",temp1,",","\r\n",1,1,1,1,1,0,1,0,temp2,0,NULL);
#endif /* ifdef WIN32 */
#ifndef WIN32
	IMPEXP=IMPEXPdefNEW("MENUITEM-EXPORT","UTILITIES","MENUITEM","MENUITEM.csv",temp1,",","\r\n",1,1,1,1,1,0,1,0,temp2,0,NULL);
#endif /* ifndef WIN32 */
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(IMPEXP!=NULL)
	{
		addIMPEXPfieldADV(IMPEXP,"MODULE NAME",1,1,15,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ITEM NUMBER",8,16,8,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 1",1,24,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 2",1,144,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 3",1,264,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 4",1,384,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 5",1,504,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"DELETEFLAG",10,624,1,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"DROP DOWN LIST",7,625,2,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 1",1,627,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 2",1,747,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 3",1,867,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 4",1,987,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 5",1,1107,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"NAME",1,1227,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"PARENT",8,1347,8,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"PROGRAM",1,1355,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"RANK",7,1475,4,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"STYLE",7,1479,2,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 1",14,1481,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 2",14,11480,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 3",14,21479,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 4",14,31478,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 5",14,-24059,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"VISIBLE EXPRESSION",14,-14060,9999,NULL,1,NULL);
		if(writeIMPORTbin(module,IMPEXP))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+9+110+1);
			sprintf(temp1,"IMPORT/EXPORT WRITE ERROR: Module [UTILITIES] Import/Export definition [MENUITEM-EXPORT], Can Not Save Import/Export Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(IMPEXP!=NULL) free_import(IMPEXP);
	}
}
static void IMPEXP_MENUITEM_IMPORT()
{
	IEfile *IMPEXP=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");

	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");

#ifdef WIN32
	IMPEXP=IMPEXPdefNEW("MENUITEM-IMPORT","UTILITIES","MENUITEM","MENUITEM.csv",temp1,",","\r\n",0,1,1,1,1,0,1,0,temp2,0,NULL);
#endif /* ifdef WIN32 */
#ifndef WIN32
	IMPEXP=IMPEXPdefNEW("MENUITEM-IMPORT","UTILITIES","MENUITEM","MENUITEM.csv",temp1,",","\r\n",0,1,1,1,1,0,1,0,temp2,0,NULL);
#endif /* ifndef WIN32 */
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(IMPEXP!=NULL)
	{
		addIMPEXPfieldADV(IMPEXP,"MODULE NAME",1,1,15,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ITEM NUMBER",8,16,8,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 1",1,24,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 2",1,144,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 3",1,264,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 4",1,384,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ARGUMENT 5",1,504,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"DELETEFLAG",10,624,1,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"DROP DOWN LIST",7,625,2,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 1",1,627,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 2",1,747,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 3",1,867,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 4",1,987,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT NAME 5",1,1107,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"NAME",1,1227,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"PARENT",8,1347,8,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"PROGRAM",1,1355,120,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"RANK",7,1475,4,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"STYLE",7,1479,2,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 1",14,1481,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 2",14,11480,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 3",14,21479,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 4",14,31478,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"ENVIRONMENT VALUE 5",14,-24059,9999,NULL,1,NULL);
		addIMPEXPfieldADV(IMPEXP,"VISIBLE EXPRESSION",14,-14060,9999,NULL,1,NULL);
		if(writeIMPORTbin(module,IMPEXP))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+9+110+1);
			sprintf(temp1,"IMPORT/EXPORT WRITE ERROR: Module [UTILITIES] Import/Export definition [MENUITEM-IMPORT], Can Not Save Import/Export Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(IMPEXP!=NULL) free_import(IMPEXP);
	}
}
static void SCN_MENUITEM_EXPORT_IMPORT_EXPORT_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MENUITEM-EXPORT IMPORT/EXPORT RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FILENAME","Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(42+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MENUITEM-EXPORT IMPORT/EXPORT RANGE SCREEN], Can Not Save Maintain Master!");
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
static void SCN_MENUITEM_IMPORT_IMPORT_EXPORT_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MENUITEM-IMPORT IMPORT/EXPORT RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FILENAME","Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(42+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MENUITEM-IMPORT IMPORT/EXPORT RANGE SCREEN], Can Not Save Maintain Master!");
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
#endif /* USING_QT */
void IMPEXP_MENUITEM()
{
#ifdef USING_QT
	IMPEXP_MENUITEM_EXPORT();
	IMPEXP_MENUITEM_IMPORT();
	SCN_MENUITEM_EXPORT_IMPORT_EXPORT_RANGE_SCREEN();
	SCN_MENUITEM_IMPORT_IMPORT_EXPORT_RANGE_SCREEN();
#endif /* USING_QT */
}
