/* DATABASE's - Make (DEFINE IMPORT EXPORT) Screen */
#include<mix.hpp>

extern char *module;

static void DEFINE_IMPORT_EXPORT()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("DATABASE","DEFINE IMPORT EXPORT");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Modules:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Definitions:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"MODULE LIST","Module List","","",15,0,0,"","","","");
		ADVaddwdgt(scrn,7,"DEFINITIONS AVAILABLE","Definitions Available","","",15,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"DESCRIPTION","Description","","",3,80,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,16,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,"","","","");
		ADVaddwdgt(scrn,6,"EXECUTE","Execute","","",0,0,25,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,"","","","");
		ADVaddwdgt(scrn,6,"DELETE","Delete","","",0,0,33,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void MAINTAIN_IMPORT_EXPORT_DEFINITION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","MAINTAIN IMPORT EXPORT DEFINITION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Name / Description","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Definition Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME","","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Direction:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"DIRECTION","","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description of Import /Export Definition:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"DESCRIPTION","","","",2,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Import Definition","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Ascii File:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ASCII FILE","","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE SCREEN","Range Screen","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RANGE SCREEN NAME","","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DEFAULT RANGE SCREEN","Make Range Screen","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DEFAULT MENU","Make Menu Definition","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SKIP DUPLICATES","Skip Duplicates","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Types:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"TYPES","","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"FORCE QUOTES","Force Quotes","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CARRIAGE RETURN","CR","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"LINE FEED","LF","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Field Delimiter:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FIELD DELIMITER","","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Record Delimiter:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RECORD DELIMITER","","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Modules:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Files:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"MODULE LIST","","","",3,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"FILE LIST","","","",3,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Field Definitions:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"FIELD DEFINITIONS","","","",5,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD ABOVE","Add Above","","",0,0,57,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD BELOW","Add Below","","",0,0,58,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DELETE","Delete","","",0,0,33,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Start Offset:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"START OFFSET","Start Offset","","",0,5,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Additional Offset:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADDITIONAL OFFSET AMOUNT","Additional Offset Amount","","",0,5,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"AUTO BUILD FIELDS","Auto Build Fields","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RE-CALCULATE OFFSETS","Re-Calculate","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Select Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"EXPRESSION","","","",1,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD VALUE","Load Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD OPERATOR","Load Operator","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD GROUPER","Load Grouper","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD CONTROL","Load Control","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DFSCN","Define Screen","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DFMENU","Define Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PRINT DEFINITION","Print Definition","","",0,0,7,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(33+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [MAINTAIN IMPORT EXPORT DEFINITION], Can Not Save Maintain Master!");
			prterr(temp1);
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
static void MAINTAIN_IMPORT_EXPORT_FIELD()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("DATABASE","MAINTAIN IMPORT EXPORT FIELD");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Fields Available:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"FIELD LIST","","","",5,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Start:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"START","","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,5,"","Length:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"LENGTH","","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","Field Type:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"FIELD TYPES","","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Format Type:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"USE QUOTES","Use Quotes","","",6,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Expression:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"EXPRESSION","","","",3,80,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD VALUE","Load Value","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD OPERATOR","Load Operator","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD GROUPER","Load Grouper","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD CONTROL","Load Control","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Value Expression:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"VALUE EXPRESSION","","","",3,80,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD VALUE VALUE EXPRESSION","Load Value","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD OPERATOR VALUE EXPRESSION","Load Operator","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD GROUPER VALUE EXPRESSION","Load Grouper","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD CONTROL VALUE EXPRESSION","Load Control","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,24,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void DEFINE_I_E_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("DEFINE I/E","dfie.EXT");
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
void mk_dfie()
{
	DEFINE_IMPORT_EXPORT();
	MAINTAIN_IMPORT_EXPORT_DEFINITION();
	MAINTAIN_IMPORT_EXPORT_FIELD();
	DEFINE_I_E_MENU();
}
