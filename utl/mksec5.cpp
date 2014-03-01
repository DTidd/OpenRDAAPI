/* SECURITY's - Make (MTN FILESEC) Maintain Master */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
#include <mkdef.hpp>

extern char *module;
extern APPlib *errorlist;

static void MTN_FILESEC()
{
	char *defdir=NULL;
	int x=0;
	RDAsearch *S=NULL;
	short r=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	MaintainButton *button=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(75+1);
	sprintf(temp1,"%s",
		"This Maintain Master definition is used to define the File Security Master.");
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

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN FILESEC",temp1,0,"FILESEC","FILE SECURITY BROWSE","FILE SECURITY DEFINE LIST","FILE SECURITY SEARCH BROWSE","FILE SECURITY MAINTAIN SCREEN",0,1,0,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* SECURITY USERS */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,FALSE,"","","","",FALSE,"SECURITY","FILESEC");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[FILESEC][USER IDENTIFICATION]",FALSE);
		}


/* SECURITY MODSEC */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","MODSEC",0,0,NULL,"MODSEC KEY",temp1,FALSE,"","","","",FALSE,"SECURITY","FILESEC");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[FILESEC][USER IDENTIFICATION]",TRUE);
			addSupportingRDAconfld(S,"MODULE NAME","[FILESEC][MODULE NAME]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);
/* BUTTONS Beginning */


/* FIELD SECURITY mtnmst BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"SECURITY");
		addAPPlib(args,"MTN FLDSEC");
		x=addMaintainMasterButton(MTNMSTR,"mtnmst","Field Security","FIELD SECURITY","SECURITY","FLDSEC","FLDSEC KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"USER IDENTIFICATION","USER_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"MODULE NAME","MODULE_NAME");
			x=addMaintainMasterButtonField(button,"FILE NAME","FILE_NAME");
		}

/* CRTFLDSEC mtnmst BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=NULL;
		x=addMaintainMasterButton(MTNMSTR,"crtfldsec","Create Field Security","CRTFLDSEC","SECURITY","FLDSEC","FLDSEC KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"USER IDENTIFICATION","USER_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"MODULE NAME","MODULE");
			x=addMaintainMasterButtonField(button,"FILE NAME","FILE");
		}


/* ADD FLDSEC doadd BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"SECURITY");
		addAPPlib(args,"ADD FIELD SECURITY");
		x=addMaintainMasterButton(MTNMSTR,"doadd","Add Field Security","ADD FLDSEC","SECURITY","FILESEC","FILESEC KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"USER IDENTIFICATION","ADD_USER_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"MODULE NAME","ADD_MODULE_NAME");
			x=addMaintainMasterButtonField(button,"FILE NAME","ADD_FILE_NAME");
		}


/* BUTTONS Ending */




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
			temp1=Rmalloc(11+8+89+1);
			sprintf(temp1,"MAINTAIN MASTER WRITE ERROR: Module [SECURITY] Maintain Master [MTN FILESEC], Can Not Save Maintain Master!");
			prterr(temp1);
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
static void FILE_SECURITY_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("SECURITY","FILE SECURITY BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Run Function:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"RUN FUNCTION","Run Function","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"(NOT([SECURITY][MODSEC][USER IDENTIFICATION]=\"\"))","","","");
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[MODSEC][USER IDENTIFICATION]","User Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,0,"[USERS][USER NAME]","User Name","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"(NOT([SECURITY][MODSEC][MODULE NAME]=\"\"))","","","");
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[MODSEC][MODULE NAME]","Module Name","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PRINT BROWSE","Print","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help Browse","","",0,0,0,"","","","");
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
static void FILE_SECURITY_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("SECURITY","FILE SECURITY DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Keys Available:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"KEY LIST","Key List","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Enter the position and length for each field:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"USER IDENTIFICATION POSITION","User ID Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"USER IDENTIFICATION LENGTH","User ID Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"MODULE NAME POSITION","Module Name Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"MODULE NAME LENGTH","Module Name Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","File name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FILE NAME POSITION","File Name Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"FILE NAME LENGTH","File Name Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Field Security:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FIELD SECURITY POSITION","Field Security Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"FIELD SECURITY LENGTH","Field Security Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Read file:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"READ FILE POSITION","Read File Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"READ FILE LENGTH","Read File Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Write File:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"WRITE FILE POSITION","Write File Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"WRITE FILE LENGTH","Write File Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Print file:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"PRINT FILE POSITION","Print File Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"PRINT FILE LENGTH","Print File Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Expression:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"EXPRESSION POSITION","Expression Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"EXPRESSION LENGTH","Expression Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Delete flag:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
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
static void FILE_SECURITY_MAINTAIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("SECURITY","FILE SECURITY MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[FILESEC][DELETEFLAG]","Delete this record","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PREVIOUS FILESEC KEY","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,0,"[FILESEC][USER IDENTIFICATION]","User Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,6,"NEXT FILESEC KEY","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,5,"","User Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[USERS][USER NAME]","User Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Modules:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"FILESEC MODULES","Module List","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Module name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PREVIOUS RDA FILESEC KEY","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,0,"[FILESEC][MODULE NAME]","Module Name","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,6,"NEXT RDA FILESEC KEY","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Files:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"FILESEC FILES","File List","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","File name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PREVIOUS RDA FILESEC KEY 2","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,0,"[FILESEC][FILE NAME]","File Name","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,6,"NEXT RDA FILESEC KEY 2","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[FILESEC][FIELD SECURITY]","Field Security","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[FILESEC][READ FILE]","Read File","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[FILESEC][WRITE FILE]","Write File","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[FILESEC][PRINT FILE]","Print File","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[FILESEC][DELETE FILE]","Delete File","","",0,0,0,"","","([SECURITY][MODSEC][DELETE DATA]=TRUE)","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Expression Evaluated for Record Level Security:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"[FILESEC][EXPRESSION]","Expression","","",8,70,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"FILESEC EXPRESSION LOAD VALUE","Load Value","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"FILESEC EXPRESSION LOAD GROUPER","Load Grouper","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"FILESEC EXPRESSION LOAD OPERATOR","Load Operator","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"FILESEC EXPRESSION LOAD CONTROL","Load Control","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"BROWSE","Browse","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"FIELD SECURITY","Field Security","","",0,0,0,"","","([SECURITY][FILESEC][FIELD SECURITY]=TRUE)","");
		ADVaddwdgt(scrn,6,"ADD FLDSEC","Add Field Security","","",0,0,0,"","","([SECURITY][FILESEC][FIELD SECURITY]=TRUE)","");
		ADVaddwdgt(scrn,6,"CRTFLDSEC","Create Field Security","","",0,0,0,"","","([SECURITY][FILESEC][FIELD SECURITY]=TRUE)","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,0,"","","([SECURITY][FILESEC][FILE NAME]!=\"No FILESEC FILE\")","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,"","","([SECURITY][FILESEC][FILE NAME]!=\"No FILESEC FILE\")","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
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
static void FILE_SECURITY_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","FILE SECURITY SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][DELETE FILE] FALSE","Select when Delete File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][DELETE FILE] TRUE","Select when Delete File True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][FIELD SECURITY] FALSE","Select when Field Security False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][FIELD SECURITY] TRUE","Select when Field Security True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][PRINT FILE] FALSE","Select when Print File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][PRINT FILE] TRUE","Select when Print File True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][READ FILE] FALSE","Select when Read File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][READ FILE] TRUE","Select when Read File True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][WRITE FILE] FALSE","Select when Write File False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [FILESEC][WRITE FILE] TRUE","Select when Write File True","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [FILESEC][FILE NAME]","Range on File Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [FILESEC][FILE NAME]","From File Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [FILESEC][FILE NAME]","To File Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [FILESEC][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [FILESEC][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [FILESEC][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [FILESEC][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [FILESEC][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [FILESEC][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(27+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [FILE SECURITY SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void LST_FILESEC_FILES()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAScrolledListNew("SECURITY","FILESEC FILES");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=2;
		ScrolledListDFincvir(scrn,"SECURITY","FILESEC","",0);
	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"[SECURITY][FILESEC][MODULE NAME]");
		scrn->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->special_function=stralloc("FILES AVAILABLE");
		scrn->num=0;
		scrn->initfld=NULL;
		scrn->ffield=stralloc("");
		scrn->ffinfo=Rmalloc(sizeof(DFincvir));
		scrn->ffinfo->module=stralloc("");
		scrn->ffinfo->file=stralloc("");
		scrn->ffinfo->keyname=stralloc("");
		scrn->contype=0;
		scrn->conmod=stralloc("SECURITY");
		scrn->confil=stralloc("FILESEC");
		scrn->confld=stralloc("FILE NAME");
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
		scrn->timing=1;

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
			temp1=Rmalloc(13+8+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [SECURITY] RDAScrolledList [FILESEC FILES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDAScrolledList(scrn);
	}
}
static void LST_FILESEC_MODULES()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAScrolledListNew("SECURITY","FILESEC MODULES");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=2;
		ScrolledListDFincvir(scrn,"SECURITY","FILESEC","",0);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		scrn->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->special_function=stralloc("MODULES AVAILABLE");
		scrn->num=0;
		scrn->initfld=NULL;
		scrn->ffield=stralloc("");
		scrn->ffinfo=Rmalloc(sizeof(DFincvir));
		scrn->ffinfo->module=stralloc("");
		scrn->ffinfo->file=stralloc("");
		scrn->ffinfo->keyname=stralloc("");
		scrn->contype=0;
		scrn->conmod=stralloc("SECURITY");
		scrn->confil=stralloc("FILESEC");
		scrn->confld=stralloc("MODULE NAME");
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
		scrn->timing=1;

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
			temp1=Rmalloc(15+8+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [SECURITY] RDAScrolledList [FILESEC MODULES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDAScrolledList(scrn);
	}
}
static void RLV_FILESEC_EXPRESSION()
{
	RDALoadValue *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDALoadValueNEW("SECURITY","FILESEC EXPRESSION");
	if(scrn!=NULL)
	{
		scrn->conname=stralloc("[FILESEC][EXPRESSION]");
		scrn->desc=stralloc("");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LDV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,module);
#endif
		if(writeRDALoadValuebin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+8+100+1);
			sprintf(temp1,"LOAD VALUE WRITE ERROR: Module [SECURITY] RDALoadValue [FILESEC EXPRESSION], Can Not Save Load Value!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDALoadValue(scrn);
	}
}
static void DEF_FILE_SECURITY_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("SECURITY","FILE SECURITY DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,1);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"USER IDENTIFICATION POSITION",TRUE,7,6);
		addDefaultFieldSHORT(def,"USER IDENTIFICATION LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"FILE NAME POSITION",TRUE,7,1);
		addDefaultFieldSHORT(def,"FILE NAME LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"FIELD SECURITY POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"FIELD SECURITY LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"READ FILE POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"READ FILE LENGTH",TRUE,7,5);
		addDefaultFieldSHORT(def,"WRITE FILE POSITION",TRUE,7,4);
		addDefaultFieldSHORT(def,"WRITE FILE LENGTH",TRUE,7,5);
		addDefaultFieldSHORT(def,"PRINT FILE POSITION",TRUE,7,5);
		addDefaultFieldSHORT(def,"PRINT FILE LENGTH",TRUE,7,5);
		addDefaultFieldSHORT(def,"DELETE FILE POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DELETE FILE LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MODULE NAME POSITION",TRUE,7,7);
		addDefaultFieldSHORT(def,"MODULE NAME LENGTH",TRUE,7,12);
		addDefaultFieldSHORT(def,"EXPRESSION POSITION",TRUE,7,15);
		addDefaultFieldSHORT(def,"EXPRESSION LENGTH",TRUE,7,40);

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
			temp1=Rmalloc(25+8+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [SECURITY] Screen [FILE SECURITY DEFINE LIST], Can Not Save Screen Defaults!");
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
static void MTN_MTN_FILESEC_SEARCH()
{
	char *defdir=NULL;
	int x=0;
	RDAsearch *S=NULL;
	short r=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	MaintainButton *button=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(75+1);
	sprintf(temp1,"%s",
		"This Maintain Master definition is used to define the File Security Master.");
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

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN FILESEC.SEARCH",temp1,0,"FILESEC","FILE SECURITY BROWSE","FILE SECURITY DEFINE LIST","FILE SECURITY SEARCH BROWSE","FILE SECURITY MAINTAIN SCREEN",0,2,0,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* SECURITY USERS */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,FALSE,"","","","",0,"SECURITY","FILESEC");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[FILESEC][USER IDENTIFICATION]",FALSE);
		}


/* SECURITY MODSEC */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","MODSEC",0,0,NULL,"MODSEC KEY",temp1,FALSE,"","","","",0,"SECURITY","FILESEC");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[FILESEC][USER IDENTIFICATION]",TRUE);
			addSupportingRDAconfld(S,"MODULE NAME","[FILESEC][MODULE NAME]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);
/* BUTTONS Beginning */


/* FIELD SECURITY mtnmst BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"SECURITY");
		addAPPlib(args,"MTN FLDSEC");
		x=addMaintainMasterButton(MTNMSTR,"mtnmst","Field Security","FIELD SECURITY","SECURITY","FLDSEC","FLDSEC KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"USER IDENTIFICATION","USER_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"MODULE NAME","MODULE_NAME");
			x=addMaintainMasterButtonField(button,"FILE NAME","FILE_NAME");
		}


/* CRTFLDSEC crtfldsec BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		x=addMaintainMasterButton(MTNMSTR,"crtfldsec","Create Field Security","CRTFLDSEC","SECURITY","FLDSEC","FLDSEC KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"USER IDENTIFICATION","USER_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"MODULE NAME","MODULE");
			x=addMaintainMasterButtonField(button,"FILE NAME","FILE");
		}


/* ADD FLDSEC doadd BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"SECURITY");
		addAPPlib(args,"ADD FIELD SECURITY");
		x=addMaintainMasterButton(MTNMSTR,"doadd","Add Field Security","ADD FLDSEC","SECURITY","FILESEC","FILESEC KEY",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"USER IDENTIFICATION","ADD_USER_IDENTIFICATION");
			x=addMaintainMasterButtonField(button,"MODULE NAME","ADD_MODULE_NAME");
			x=addMaintainMasterButtonField(button,"FILE NAME","ADD_FILE_NAME");
		}


/* BUTTONS Ending */




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
			temp1=Rmalloc(18+8+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [SECURITY] Maintain Master [MTN FILESEC.SEARCH], Can Not Save Maintain Master!");
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

void FILESEC_MASTER()
{
	MTN_FILESEC();
	FILE_SECURITY_BROWSE();
	FILE_SECURITY_DEFINE_LIST();
	FILE_SECURITY_MAINTAIN_SCREEN();
	FILE_SECURITY_SEARCH_BROWSE();
	LST_FILESEC_FILES();
	LST_FILESEC_MODULES();
	RLV_FILESEC_EXPRESSION();
	DEF_FILE_SECURITY_DEFINE_LIST();
	MTN_MTN_FILESEC_SEARCH();
}
