/* DATABASE's - Make (MTN DATTRN) Maintain Master */
#include <cstdio>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>

extern char *module;
extern APPlib *errorlist;

static void MTN_MTN_DATTRN()
{
	char *defdir=NULL;
	int x=0;
	RDAsearch *S=NULL;
	short r=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(132+1);
	sprintf(temp1,"%s%s",
		"This maintain master is used to record the changes to each field within a database holding the previ",
		"ous and new value of each field.");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN DATTRN",temp1,1,"DATTRN","DATABASE MASTER LOG TRANSACTION BROWSE","DATABASE MASTER LOG TRANSACTION DEFINE LIST","DATABASE MASTER LOG TRANSACTION SEARCH BROWSE","DATABASE MASTER LOG TRANSACTION MAINTAIN SCREEN",0,1,0,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* DATABASE DATLOG */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"DATABASE","DATLOG",0,0,NULL,"DATLOG KEY 1",temp1,TRUE,"LOAD DATLOG","SELECT DATABASE MASTER LOG BROWSE","SELECT DATABASE MASTER LOG DEFINE LIST","SELECT DATABASE MASTER LOG SEARCH BROWSE",0,"DATABASE","DATTRN");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"MODULE","[DATTRN][MODULE]",FALSE);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[DATTRN][USER IDENTIFICATION]",FALSE);
			addSupportingRDAconfld(S,"LOG NUMBER","[DATTRN][LOG NUMBER]",FALSE);
		}


/* SECURITY USERS */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,TRUE,"LOAD USERS","SELECT USER IDENTIFICATION BROWSE","SELECT USER IDENTIFICATION DEFINE LIST","SELECT USER IDENTIFICATION SEARCH BROWSE",0,"DATABASE","DATTRN");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[DATTRN][USER IDENTIFICATION]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);


		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MTN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,module);
#endif
		if(writeMaintainMaster(defdir,MTNMSTR))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+8+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [DATABASE] Maintain Master [MTN DATTRN], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		} else {
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		}
	}
}
static void SCN_DATABASE_MASTER_LOG_TRANSACTION_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","DATABASE MASTER LOG TRANSACTION BROWSE");
	if(scrn!=NULL)
	{
		temp1=Rmalloc(23+1);
		sprintf(temp1,"%s",
			"[EXTENSIONS SETUP]=TRUE");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PRINT BROWSE","Print Browse","","",0,0,7,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search Field:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",20,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Position","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST POSITION","Position","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Count","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG TOTAL","Count","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,16,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(38+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [DATABASE MASTER LOG TRANSACTION BROWSE], Can Not Save Maintain Master!");
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
static void SCN_DATABASE_MASTER_LOG_TRANSACTION_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","DATABASE MASTER LOG TRANSACTION DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,500,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Changed Field:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Changed Field Length:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Changed Field Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Changed File:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Changed Module:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 1 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 1 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 2 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 2 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 3 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 3 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 4 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 4 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 5 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 5 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 6 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 6 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 7 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 7 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 8 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 8 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 9 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 9 Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Log Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","New Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","No Previous Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Previous Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Transaction Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FIELD POSITION","Changed Field Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FIELD LENGTH","Changed Field Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FIELD LENGTH POSITION","Changed Field Length Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FIELD LENGTH LENGTH","Changed Field Length Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FIELD TYPE POSITION","Changed Field Type Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FIELD TYPE LENGTH","Changed Field Type Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FILE POSITION","Changed File Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED FILE LENGTH","Changed File Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED MODULE POSITION","Changed Module Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CHANGED MODULE LENGTH","Changed Module Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 1 NAME POSITION","Key Field 1 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 1 NAME LENGTH","Key Field 1 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 1 VALUE POSITION","Key Field 1 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 1 VALUE LENGTH","Key Field 1 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 2 NAME POSITION","Key Field 2 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 2 NAME LENGTH","Key Field 2 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 2 VALUE POSITION","Key Field 2 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 2 VALUE LENGTH","Key Field 2 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 3 NAME POSITION","Key Field 3 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 3 NAME LENGTH","Key Field 3 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 3 VALUE POSITION","Key Field 3 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 3 VALUE LENGTH","Key Field 3 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 4 NAME POSITION","Key Field 4 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 4 NAME LENGTH","Key Field 4 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 4 VALUE POSITION","Key Field 4 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 4 VALUE LENGTH","Key Field 4 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 5 NAME POSITION","Key Field 5 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 5 NAME LENGTH","Key Field 5 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 5 VALUE POSITION","Key Field 5 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 5 VALUE LENGTH","Key Field 5 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 6 NAME POSITION","Key Field 6 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 6 NAME LENGTH","Key Field 6 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 6 VALUE POSITION","Key Field 6 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 6 VALUE LENGTH","Key Field 6 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 7 NAME POSITION","Key Field 7 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 7 NAME LENGTH","Key Field 7 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 7 VALUE POSITION","Key Field 7 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 7 VALUE LENGTH","Key Field 7 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 8 NAME POSITION","Key Field 8 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 8 NAME LENGTH","Key Field 8 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 8 VALUE POSITION","Key Field 8 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 8 VALUE LENGTH","Key Field 8 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 9 NAME POSITION","Key Field 9 Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 9 NAME LENGTH","Key Field 9 Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 9 VALUE POSITION","Key Field 9 Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY FIELD 9 VALUE LENGTH","Key Field 9 Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"LOG NUMBER POSITION","Log Number Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"LOG NUMBER LENGTH","Log Number Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE POSITION","Module Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE LENGTH","Module Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NEW VALUE POSITION","New Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NEW VALUE LENGTH","New Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NO PREVIOUS VALUE POSITION","No Previous Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NO PREVIOUS VALUE LENGTH","No Previous Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PREVIOUS VALUE POSITION","Previous Value Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PREVIOUS VALUE LENGTH","Previous Value Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TRANSACTION NUMBER POSITION","Transaction Number Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TRANSACTION NUMBER LENGTH","Transaction Number Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"USER IDENTIFICATION POSITION","User Identification Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"USER IDENTIFICATION LENGTH","User Identification Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(91+1);
		sprintf(temp1,"%s",
			"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
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
			temp1=Rmalloc(43+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [DATABASE MASTER LOG TRANSACTION DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_DATABASE_MASTER_LOG_TRANSACTION_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","DATABASE MASTER LOG TRANSACTION SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATTRN][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATTRN][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATTRN][NO PREVIOUS VALUE] FALSE","Select when No Previous Value False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [DATTRN][NO PREVIOUS VALUE] TRUE","Select when No Previous Value True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Range","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][CHANGED FIELD]","Range on Changed Field","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][CHANGED FIELD]","From Changed Field","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][CHANGED FIELD]","To Changed Field","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][CHANGED FIELD LENGTH]","Range on Changed Field Length","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][CHANGED FIELD LENGTH]","From Changed Field Length","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][CHANGED FIELD LENGTH]","To Changed Field Length","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][CHANGED FIELD TYPE]","Range on Changed Field Type","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][CHANGED FIELD TYPE]","From Changed Field Type","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][CHANGED FIELD TYPE]","To Changed Field Type","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][CHANGED FILE]","Range on Changed File","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][CHANGED FILE]","From Changed File","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][CHANGED FILE]","To Changed File","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][CHANGED MODULE]","Range on Changed Module","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][CHANGED MODULE]","From Changed Module","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][CHANGED MODULE]","To Changed Module","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 1 NAME]","Range on Key Field 1 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 1 NAME]","From Key Field 1 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 1 NAME]","To Key Field 1 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 1 VALUE]","Range on Key Field 1 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 1 VALUE]","From Key Field 1 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 1 VALUE]","To Key Field 1 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 2 NAME]","Range on Key Field 2 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 2 NAME]","From Key Field 2 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 2 NAME]","To Key Field 2 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 2 VALUE]","Range on Key Field 2 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 2 VALUE]","From Key Field 2 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 2 VALUE]","To Key Field 2 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 3 NAME]","Range on Key Field 3 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 3 NAME]","From Key Field 3 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 3 NAME]","To Key Field 3 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 3 VALUE]","Range on Key Field 3 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 3 VALUE]","From Key Field 3 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 3 VALUE]","To Key Field 3 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 4 NAME]","Range on Key Field 4 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 4 NAME]","From Key Field 4 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 4 NAME]","To Key Field 4 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 4 VALUE]","Range on Key Field 4 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 4 VALUE]","From Key Field 4 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 4 VALUE]","To Key Field 4 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 5 NAME]","Range on Key Field 5 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 5 NAME]","From Key Field 5 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 5 NAME]","To Key Field 5 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 5 VALUE]","Range on Key Field 5 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 5 VALUE]","From Key Field 5 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 5 VALUE]","To Key Field 5 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 6 NAME]","Range on Key Field 6 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 6 NAME]","From Key Field 6 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 6 NAME]","To Key Field 6 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 6 VALUE]","Range on Key Field 6 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 6 VALUE]","From Key Field 6 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 6 VALUE]","To Key Field 6 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 7 NAME]","Range on Key Field 7 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 7 NAME]","From Key Field 7 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 7 NAME]","To Key Field 7 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 7 VALUE]","Range on Key Field 7 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 7 VALUE]","From Key Field 7 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 7 VALUE]","To Key Field 7 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 8 NAME]","Range on Key Field 8 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 8 NAME]","From Key Field 8 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 8 NAME]","To Key Field 8 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 8 VALUE]","Range on Key Field 8 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 8 VALUE]","From Key Field 8 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 8 VALUE]","To Key Field 8 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 9 NAME]","Range on Key Field 9 Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 9 NAME]","From Key Field 9 Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 9 NAME]","To Key Field 9 Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][KEY FIELD 9 VALUE]","Range on Key Field 9 Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][KEY FIELD 9 VALUE]","From Key Field 9 Value","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][KEY FIELD 9 VALUE]","To Key Field 9 Value","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][LOG NUMBER]","Range on Log Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][LOG NUMBER]","From Log Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][LOG NUMBER]","To Log Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][MODULE]","Range on Module","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][MODULE]","From Module","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][MODULE]","To Module","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][TRANSACTION NUMBER]","Range on Transaction Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][TRANSACTION NUMBER]","From Transaction Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][TRANSACTION NUMBER]","To Transaction Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [DATTRN][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [DATTRN][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [DATTRN][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Query:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,14,"DATABASE MASTER LOG TRANSACTION SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(91+1);
		sprintf(temp1,"%s",
			"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
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
			temp1=Rmalloc(45+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [DATABASE MASTER LOG TRANSACTION SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_DATABASE_MASTER_LOG_TRANSACTION_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","DATABASE MASTER LOG TRANSACTION SEARCH BROWSE EXPRESSION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"EXPRESSION","Expression","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD VALUE","Load Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD GROUPER","Load Grouper","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD OPERATOR","Load Operator","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD CONTROL","Load Control","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(56+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [DATABASE MASTER LOG TRANSACTION SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void MTN_MTN_DATTRN_SEARCH()
{
	char *defdir=NULL;
	int x=0;
	RDAsearch *S=NULL;
	short r=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(132+1);
	sprintf(temp1,"%s%s",
		"This maintain master is used to record the changes to each field within a database holding the previ",
		"ous and new value of each field.");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN DATTRN.SEARCH",temp1,1,"DATTRN","DATABASE MASTER LOG TRANSACTION BROWSE","DATABASE MASTER LOG TRANSACTION DEFINE LIST","DATABASE MASTER LOG TRANSACTION SEARCH BROWSE","DATABASE MASTER LOG TRANSACTION MAINTAIN SCREEN",0,2,0,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* DATABASE DATLOG */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"DATABASE","DATLOG",0,0,NULL,"DATLOG KEY 1",temp1,TRUE,"LOAD DATLOG","SELECT DATABASE MASTER LOG BROWSE","SELECT DATABASE MASTER LOG DEFINE LIST","SELECT DATABASE MASTER LOG SEARCH BROWSE",0,"DATABASE","DATTRN");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"MODULE","[DATTRN][MODULE]",FALSE);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[DATTRN][USER IDENTIFICATION]",FALSE);
			addSupportingRDAconfld(S,"LOG NUMBER","[DATTRN][LOG NUMBER]",FALSE);
		}


/* SECURITY USERS */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,TRUE,"LOAD USERS","SELECT USER IDENTIFICATION BROWSE","SELECT USER IDENTIFICATION DEFINE LIST","SELECT USER IDENTIFICATION SEARCH BROWSE",0,"DATABASE","DATTRN");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[DATTRN][USER IDENTIFICATION]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);


		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MTN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,module);
#endif
		if(writeMaintainMaster(defdir,MTNMSTR))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+8+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [DATABASE] Maintain Master [MTN DATTRN.SEARCH], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		} else {
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		}
	}
}
static void DEF_DATABASE_MASTER_LOG_TRANSACTION_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("DATABASE","DATABASE MASTER LOG TRANSACTION DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"MODULE POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"MODULE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"USER IDENTIFICATION POSITION",TRUE,7,4);
		addDefaultFieldSHORT(def,"USER IDENTIFICATION LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"LOG NUMBER POSITION",TRUE,7,5);
		addDefaultFieldSHORT(def,"LOG NUMBER LENGTH",TRUE,7,8);
		addDefaultFieldSHORT(def,"TRANSACTION NUMBER POSITION",TRUE,7,6);
		addDefaultFieldSHORT(def,"TRANSACTION NUMBER LENGTH",TRUE,7,8);
		addDefaultFieldSHORT(def,"CHANGED FIELD POSITION",TRUE,7,12);
		addDefaultFieldSHORT(def,"CHANGED FIELD LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"CHANGED FIELD LENGTH POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"CHANGED FIELD LENGTH LENGTH",TRUE,7,4);
		addDefaultFieldSHORT(def,"CHANGED FIELD TYPE POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"CHANGED FIELD TYPE LENGTH",TRUE,7,2);
		addDefaultFieldSHORT(def,"CHANGED FILE POSITION",TRUE,7,11);
		addDefaultFieldSHORT(def,"CHANGED FILE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"CHANGED MODULE POSITION",TRUE,7,10);
		addDefaultFieldSHORT(def,"CHANGED MODULE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 1 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 1 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 1 VALUE POSITION",TRUE,7,15);
		addDefaultFieldSHORT(def,"KEY FIELD 1 VALUE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"KEY FIELD 2 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 2 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 2 VALUE POSITION",TRUE,7,16);
		addDefaultFieldSHORT(def,"KEY FIELD 2 VALUE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"KEY FIELD 3 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 3 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 3 VALUE POSITION",TRUE,7,17);
		addDefaultFieldSHORT(def,"KEY FIELD 3 VALUE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"KEY FIELD 4 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 4 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 4 VALUE POSITION",TRUE,7,18);
		addDefaultFieldSHORT(def,"KEY FIELD 4 VALUE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"KEY FIELD 5 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 5 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 5 VALUE POSITION",TRUE,7,19);
		addDefaultFieldSHORT(def,"KEY FIELD 5 VALUE LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"KEY FIELD 6 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 6 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 6 VALUE POSITION",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 6 VALUE LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 7 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 7 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 7 VALUE POSITION",TRUE,7,21);
		addDefaultFieldSHORT(def,"KEY FIELD 7 VALUE LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 8 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 8 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 8 VALUE POSITION",TRUE,7,22);
		addDefaultFieldSHORT(def,"KEY FIELD 8 VALUE LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 9 NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY FIELD 9 NAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"KEY FIELD 9 VALUE POSITION",TRUE,7,23);
		addDefaultFieldSHORT(def,"KEY FIELD 9 VALUE LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"NO PREVIOUS VALUE POSITION",TRUE,7,8);
		addDefaultFieldSHORT(def,"NO PREVIOUS VALUE LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"NEW VALUE POSITION",TRUE,7,30);
		addDefaultFieldSHORT(def,"NEW VALUE LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"PREVIOUS VALUE POSITION",TRUE,7,31);
		addDefaultFieldSHORT(def,"PREVIOUS VALUE LENGTH",TRUE,7,20);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(43+8+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [DATABASE] Screen [DATABASE MASTER LOG TRANSACTION DEFINE LIST], Can Not Save Screen Defaults!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(def!=NULL) FreeRDAdefault(def);
	}
}
static void SCN_DATABASE_MASTER_LOG_TRANSACTION_MAINTAIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","DATABASE MASTER LOG TRANSACTION MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"[DATTRN][DELETEFLAG]","Delete this record","","",0,0,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","    Module:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][MODULE]","Module:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD USERS","Load Users","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD USERS","Previous Load Users","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][USER IDENTIFICATION]","User Identification:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD USERS","Next Load Users","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[USERS][USER NAME]","User Name","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Log Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD DATLOG","Load Datlog","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD DATLOG","Previous Load Datlog","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][LOG NUMBER]","Log Number:","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD DATLOG","Next Load Datlog","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATLOG][PROCESS NAME]","Process Name","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Transaction Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS DATTRN KEY 1","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][TRANSACTION NUMBER]","Transaction Number:","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT DATTRN KEY 1","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Key Information","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 1 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 1 NAME]","Key Field 1 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 1 VALUE]","Key Field 1 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 2 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 2 NAME]","Key Field 2 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 2 VALUE]","Key Field 2 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 3 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 3 NAME]","Key Field 3 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 3 VALUE]","Key Field 3 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 4 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 4 NAME]","Key Field 4 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 4 VALUE]","Key Field 4 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 5 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 5 NAME]","Key Field 5 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 5 VALUE]","Key Field 5 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 6 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 6 NAME]","Key Field 6 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 6 VALUE]","Key Field 6 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 7 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 7 NAME]","Key Field 7 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 7 VALUE]","Key Field 7 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 8 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 8 NAME]","Key Field 8 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 8 VALUE]","Key Field 8 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Field 9 Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 9 NAME]","Key Field 9 Name:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][KEY FIELD 9 VALUE]","Key Field 9 Value:","","",0,40,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Field Information","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Changed Module:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][CHANGED MODULE]","Changed Module:","","",0,15,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","File:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][CHANGED FILE]","Changed File:","","",0,15,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Field:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][CHANGED FIELD]","Changed Field:","","",0,30,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","        Length:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][CHANGED FIELD LENGTH]","Changed Field Length:","","",0,4,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[DATTRN][CHANGED FIELD TYPE]","Changed Field Type:","","",0,2,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,9,"[DATTRN][NO PREVIOUS VALUE]","No Previous Value","","",0,0,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Previous Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[DATTRN][PREVIOUS VALUE]","","","",3,80,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","New Value:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[DATTRN][NEW VALUE]","","","",3,80,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BROWSE","Browse","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET DEFAULTS","Reset","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,24,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(47+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [DATABASE MASTER LOG TRANSACTION MAINTAIN SCREEN], Can Not Save Maintain Master!");
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
void MTNDATTRN()
{
	MTN_MTN_DATTRN();
	SCN_DATABASE_MASTER_LOG_TRANSACTION_BROWSE();
	SCN_DATABASE_MASTER_LOG_TRANSACTION_DEFINE_LIST();
	SCN_DATABASE_MASTER_LOG_TRANSACTION_SEARCH_BROWSE();
	SCN_DATABASE_MASTER_LOG_TRANSACTION_SEARCH_BROWSE_EXPRESSION();
	SCN_DATABASE_MASTER_LOG_TRANSACTION_MAINTAIN_SCREEN();
	DEF_DATABASE_MASTER_LOG_TRANSACTION_DEFINE_LIST();
	MTN_MTN_DATTRN_SEARCH();
}
