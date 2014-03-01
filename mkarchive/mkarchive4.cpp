/* ARCHIVE's - Make (MTN ARCHIVES) Maintain Master */
#include <cstdio>
#include <mkadd.hpp>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>
#include <mkez.hpp>

extern char *module;
extern APPlib *errorlist;
extern char *swb_utl5_rgd;

static void MTN_MTN_ARCHIVES()
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

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
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

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN ARCHIVES",temp1,1,"ARCHIVES","ARCHIVES BROWSE","ARCHIVES DEFINE LIST","ARCHIVES SEARCH BROWSE","ARCHIVES MAINTAIN SCREEN",0,0,1,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* ARCHIVE ARCHWORK */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHWORK",0,0,NULL,"ARCHWORK KEY 1",temp1,TRUE,"LOAD WORK GROUP","SELECT WORK GROUP BROWSE","SELECT WORK GROUP DEFINE LIST","SELECT WORK GROUP SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"WORK GROUP","[ARCHIVES][WORK GROUP]",FALSE);
		}


/* SECURITY USERS */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,TRUE,"LOAD USERS","SELECT USERS BROWSE","SELECT USERS DEFINE LIST","SELECT USERS SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[ARCHIVES][SOURCE USER]",FALSE);
		}


/* ARCHIVE ARCHTYPE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHTYPE",0,0,NULL,"ARCHTYPE KEY",temp1,TRUE,"LOAD ARCHTYPE","SELECT ARCHIVE TYPE BROWSE","SELECT ARCHIVE TYPE DEFINE LIST","SELECT ARCHIVE TYPE SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"TYPE","[ARCHIVES][TYPE]",FALSE);
		}


/* ARCHIVE ARCHCLS */

		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHCLS][DELETEFLAG]=FALSE)");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHCLS",0,0,NULL,"ARCHCLS KEY",temp1,TRUE,"LOAD ARCHCLS","SELECT CLASS BROWSE","SELECT CLASS DEFINE LIST","SELECT CLASS SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"CLASS","[ARCHIVES][CLASS]",FALSE);
		}


/* ARCHIVE ARCHSUB */

		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHSUB][DELETEFLAG]=FALSE)");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHSUB",1,0,NULL,"ARCHSUB KEY",temp1,TRUE,"LOAD ARCHSUB","SELECT SUBJECT BROWSE","SELECT SUBJECT DEFINE LIST","SELECT SUBJECT SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"SUBJECT","[ARCHIVES][SUBJECT]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);
/* BUTTONS Beginning */


/* VIEW ARCHIVE DISPLAYFILE BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY]+\"/\"+[ARCHIVE][ARCHIVES][FILENAME]");
		x=addMaintainMasterButton(MTNMSTR,"DISPLAYFILE","View Archive","VIEW ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 2",temp1,0,1,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);


/* EMAIL ARCHIVE doreport BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"EMAILARCHIVE2");
		x=addMaintainMasterButton(MTNMSTR,"doreport","Email Archive","EMAIL ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"WORK GROUP","WORK_GROUP");
			x=addMaintainMasterButtonField(button,"ARCHIVE NAME","ARCHIVE_NAME");
		}



/* DOWNLOAD ARCHIVE RUNPOWERADD BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"DOWNLOAD ARCHIVE");
		x=addMaintainMasterButton(MTNMSTR,"RUNPOWERADD","DOWNLOAD ARCHIVE","DOWNLOAD ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"WORK GROUP","WORK_GROUP");
			x=addMaintainMasterButtonField(button,"ARCHIVE NAME","ARCHIVE_NAME");
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
			temp1=Rmalloc(12+7+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [ARCHIVE] Maintain Master [MTN ARCHIVES], Can Not Save Maintain Master!");
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
static void MTN_MTN_ARCHIVES_BROWSE()
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

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
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

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN ARCHIVES.BROWSE",temp1,1,"ARCHIVES","ARCHIVES BROWSE","ARCHIVES DEFINE LIST","ARCHIVES SEARCH BROWSE","ARCHIVES MAINTAIN SCREEN",0,1,1,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* ARCHIVE ARCHWORK */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHWORK",0,0,NULL,"ARCHWORK KEY 1",temp1,TRUE,"LOAD WORK GROUP","SELECT WORK GROUP BROWSE","SELECT WORK GROUP DEFINE LIST","SELECT WORK GROUP SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"WORK GROUP","[ARCHIVES][WORK GROUP]",FALSE);
		}


/* SECURITY USERS */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,TRUE,"LOAD USERS","SELECT USERS BROWSE","SELECT USERS DEFINE LIST","SELECT USERS SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[ARCHIVES][SOURCE USER]",FALSE);
		}


/* ARCHIVE ARCHTYPE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHTYPE",0,0,NULL,"ARCHTYPE KEY",temp1,TRUE,"LOAD ARCHTYPE","SELECT ARCHIVE TYPE BROWSE","SELECT ARCHIVE TYPE DEFINE LIST","SELECT ARCHIVE TYPE SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"TYPE","[ARCHIVES][TYPE]",FALSE);
		}


/* ARCHIVE ARCHCLS */

		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHCLS][DELETEFLAG]=FALSE)");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHCLS",0,0,NULL,"ARCHCLS KEY",temp1,TRUE,"LOAD ARCHCLS","SELECT CLASS BROWSE","SELECT CLASS DEFINE LIST","SELECT CLASS SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"CLASS","[ARCHIVES][CLASS]",FALSE);
		}


/* ARCHIVE ARCHSUB */

		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHSUB][DELETEFLAG]=FALSE)");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHSUB",1,0,NULL,"ARCHSUB KEY",temp1,TRUE,"LOAD ARCHSUB","SELECT SUBJECT BROWSE","SELECT SUBJECT DEFINE LIST","SELECT SUBJECT SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"SUBJECT","[ARCHIVES][SUBJECT]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);
/* BUTTONS Beginning */


/* VIEW ARCHIVE DISPLAYFILE BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY]+\"/\"+[ARCHIVE][ARCHIVES][FILENAME]");
		x=addMaintainMasterButton(MTNMSTR,"DISPLAYFILE","View Archive","VIEW ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 2",temp1,0,1,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);


/* EMAIL ARCHIVE doreport BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"EMAILARCHIVE2");
		x=addMaintainMasterButton(MTNMSTR,"doreport","Email Archive","EMAIL ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"WORK GROUP","WORK_GROUP");
			x=addMaintainMasterButtonField(button,"ARCHIVE NAME","ARCHIVE_NAME");
		}




/* DOWNLOAD ARCHIVE RUNPOWERADD BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"DOWNLOAD ARCHIVE");
		x=addMaintainMasterButton(MTNMSTR,"RUNPOWERADD","DOWNLOAD ARCHIVE","DOWNLOAD ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"WORK GROUP","WORK_GROUP");
			x=addMaintainMasterButtonField(button,"ARCHIVE NAME","ARCHIVE_NAME");
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
			temp1=Rmalloc(19+7+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [ARCHIVE] Maintain Master [MTN ARCHIVES.BROWSE], Can Not Save Maintain Master!");
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
static void MTN_MTN_ARCHIVES_SEARCH()
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

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
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

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN ARCHIVES.SEARCH",temp1,1,"ARCHIVES","ARCHIVES BROWSE","ARCHIVES DEFINE LIST","ARCHIVES SEARCH BROWSE","ARCHIVES MAINTAIN SCREEN",0,2,1,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* ARCHIVE ARCHWORK */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHWORK",0,0,NULL,"ARCHWORK KEY 1",temp1,TRUE,"LOAD WORK GROUP","SELECT WORK GROUP BROWSE","SELECT WORK GROUP DEFINE LIST","SELECT WORK GROUP SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"WORK GROUP","[ARCHIVES][WORK GROUP]",FALSE);
		}


/* SECURITY USERS */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,TRUE,"LOAD USERS","SELECT USERS BROWSE","SELECT USERS DEFINE LIST","SELECT USERS SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"USER IDENTIFICATION","[ARCHIVES][SOURCE USER]",FALSE);
		}


/* ARCHIVE ARCHTYPE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHTYPE",0,0,NULL,"ARCHTYPE KEY",temp1,TRUE,"LOAD ARCHTYPE","SELECT ARCHIVE TYPE BROWSE","SELECT ARCHIVE TYPE DEFINE LIST","SELECT ARCHIVE TYPE SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"TYPE","[ARCHIVES][TYPE]",FALSE);
		}


/* ARCHIVE ARCHCLS */

		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHCLS][DELETEFLAG]=FALSE)");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHCLS",0,0,NULL,"ARCHCLS KEY",temp1,TRUE,"LOAD ARCHCLS","SELECT CLASS BROWSE","SELECT CLASS DEFINE LIST","SELECT CLASS SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"CLASS","[ARCHIVES][CLASS]",FALSE);
		}


/* ARCHIVE ARCHSUB */

		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHSUB][DELETEFLAG]=FALSE)");
		r=addMaintainMasterSupportingADV(MTNMSTR,"ARCHIVE","ARCHSUB",1,0,NULL,"ARCHSUB KEY",temp1,TRUE,"LOAD ARCHSUB","SELECT SUBJECT BROWSE","SELECT SUBJECT DEFINE LIST","SELECT SUBJECT SEARCH BROWSE",1,"ARCHIVE","ARCHIVES");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"SUBJECT","[ARCHIVES][SUBJECT]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);
/* BUTTONS Beginning */


/* VIEW ARCHIVE DISPLAYFILE BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY]+\"/\"+[ARCHIVE][ARCHIVES][FILENAME]");
		x=addMaintainMasterButton(MTNMSTR,"DISPLAYFILE","View Archive","VIEW ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 2",temp1,0,1,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);

/* EMAIL ARCHIVE doreport BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"EMAILARCHIVE2");
		x=addMaintainMasterButton(MTNMSTR,"doreport","Email Archive","EMAIL ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"WORK GROUP","WORK_GROUP");
			x=addMaintainMasterButtonField(button,"ARCHIVE NAME","ARCHIVE_NAME");
		}



/* DOWNLOAD ARCHIVE RUNPOWERADD BUTTON */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"DOWNLOAD ARCHIVE");
		x=addMaintainMasterButton(MTNMSTR,"RUNPOWERADD","DOWNLOAD ARCHIVE","DOWNLOAD ARCHIVE","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,0,0,args,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"WORK GROUP","WORK_GROUP");
			x=addMaintainMasterButtonField(button,"ARCHIVE NAME","ARCHIVE_NAME");
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
			temp1=Rmalloc(19+7+89+1);
			sprintf(temp1,"MAINTAIN MASTER WRITE ERROR: Module [ARCHIVE] Maintain Master [MTN ARCHIVES.SEARCH], Can Not Save Maintain Master!");
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
static void SCN_ARCHIVES_MAINTAIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVES MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"[ARCHIVES][DELETEFLAG]","Delete this record","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Work Group/Folder:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD WORK GROUP","Load","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD WORK GROUP","Previous","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][WORK GROUP]","Work Group/Folder","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD WORK GROUP","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHWORK][NAME]","Work Group Name","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     Archive Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS ARCHIVE KEY 1","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][ARCHIVE NAME]","Archive Name:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT ARCHIVE KEY 1","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","    Source:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ARCHIVES SOURCES","Source","","",1,0,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     Archive Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD ARCHTYPE","Load","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHTYPE","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][TYPE]","Archive Type","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHTYPE","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHTYPE][DESCRIPTION]","Archive Type Description","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          Subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD ARCHSUB","Load","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHSUB","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][SUBJECT]","Subject","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHSUB","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHSUB][TITLE]","Subject Title","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","            Class:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD ARCHCLS","Load","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHCLS","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][CLASS]","Class","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHCLS","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHCLS][NAME]","Name of Class","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","         Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][FILENAME]","Filename:","","",0,45,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][DATE]","Date:","","",0,10,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,5,"","Time:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][TIME]","Time:","","",0,8,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","      Source User:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD USERS","Load Users","","",0,0,1,NULL,"([DEV_LICENSE]=TRUE)","([DEV_LICENSE]=TRUE)",NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD USERS","Previous Load Users","","",0,0,2,NULL,"([DEV_LICENSE]=TRUE)","([DEV_LICENSE]=TRUE)",NULL);
		ADVaddwdgt(scrn,0,"[ARCHIVES][SOURCE USER]","Source User:","","",0,15,0,NULL,"([DEV_LICENSE]=TRUE)",NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD USERS","Next Load Users","","",0,0,1,NULL,"([DEV_LICENSE]=TRUE)","([DEV_LICENSE]=TRUE)",NULL);
		ADVaddwdgt(scrn,0,"[USERS][USER NAME]","User Name","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[ARCHIVES][DESCRIPTION]","","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(55+1);
		sprintf(temp1,"%s","");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(152+1);
		sprintf(temp3,"%s","((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))>0) AND ([ARCHIVE][ARCHIVES][FILENAME]!=\"\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"VIEW ARCHIVE","View Archive","","",0,0,29,NULL,NULL,temp3,NULL);
		ADVaddwdgt(scrn,6,"EMAIL ARCHIVE","Email Archive","","",0,0,20,temp1,temp2,temp3,temp4);
#ifndef WIN32
		if(temp3!=NULL) Rfree(temp3);
		temp3=Rmalloc(352+1);
		sprintf(temp3,"%s","([ARCHIVE][ARCHIVES][FILENAME]!=\"\") AND ([WORKSTATION VALIDATED]=TRUE)");
		ADVaddwdgt(scrn,6,"DOWNLOAD ARCHIVE","Download Archive","","",0,0,45,NULL,NULL,temp3,NULL);
#endif
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,54,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,23,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(24+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES MAINTAIN SCREEN], Can Not Save Maintain Master!");
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
static void MNU_ARCHIVE_MTN_ARCHIVES()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE MTN ARCHIVES","mtnmst.EXT ARCHIVE \"MTN ARCHIVES.BROWSE\"");
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
static void LST_ARCHIVES_SOURCES()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAScrolledListNew("ARCHIVE","ARCHIVES SOURCES");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=1;
		ScrolledListDFincvir(scrn,"ARCHIVE","ARCHIVES","",0);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		scrn->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->special_function=NULL;
		scrn->num=0;
		scrn->initfld=NULL;
		scrn->ffield=stralloc("");
		scrn->ffinfo=Rmalloc(sizeof(DFincvir));
		scrn->ffinfo->module=stralloc("");
		scrn->ffinfo->file=stralloc("");
		scrn->ffinfo->keyname=stralloc("");
		scrn->contype=0;
		scrn->conmod=stralloc("ARCHIVE");
		scrn->confil=stralloc("ARCHIVES");
		scrn->confld=stralloc("SOURCE");
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
		scrn->list=APPlibNEW();
		addAPPlib(scrn->list,"Manual");
		addAPPlib(scrn->list,"RDA Text Browser");
		addAPPlib(scrn->list,"Scanner");
		addAPPlib(scrn->list,"Copy/Move Archive");
		addAPPlib(scrn->list,"Report Capture");
		addAPPlib(scrn->list,"DoFTPzip/CronFTPzip");
		addAPPlib(scrn->list,"Auto Archive");

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
			temp1=Rmalloc(16+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [ARCHIVES SOURCES], Can Not Save Scrolled List!");
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
static void SCN_ARCHIVES_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVES SEARCH BROWSE EXPRESSION");
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
			temp1=Rmalloc(33+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void SCN_SELECT_ARCHIVE_TYPE_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT ARCHIVE TYPE BROWSE");
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
		ADVaddwdgt(scrn,5,"","Search:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(26+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT ARCHIVE TYPE BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_ARCHIVE_TYPE_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT ARCHIVE TYPE DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"DESCRIPTION POSITION","Description Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESCRIPTION LENGTH","Description Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TYPE POSITION","Type Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TYPE LENGTH","Type Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(31+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT ARCHIVE TYPE DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_SELECT_ARCHIVE_TYPE_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT ARCHIVE TYPE SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHTYPE][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHTYPE][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHTYPE][DESCRIPTION]","Range on Description","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHTYPE][DESCRIPTION]","From Description","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHTYPE][DESCRIPTION]","To Description","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHTYPE][TYPE]","Range on Type","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHTYPE][TYPE]","From Type","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHTYPE][TYPE]","To Type","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,14,"SELECT ARCHIVE TYPE SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(33+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT ARCHIVE TYPE SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_ARCHIVE_TYPE_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT ARCHIVE TYPE SEARCH BROWSE EXPRESSION");
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
			temp1=Rmalloc(44+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT ARCHIVE TYPE SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void SCN_SELECT_USERS_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT USERS BROWSE");
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
		ADVaddwdgt(scrn,5,"","Search:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(19+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT USERS BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_USERS_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT USERS DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,200,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Able To Approve:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Approval Limit:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Budget Override:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Default File Output Device:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Default Printer:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Department:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Email:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Gtkrc:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Help Inclusion Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Job Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Phone Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Site Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Supervisor:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","User Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Web Login:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Web Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ABLE TO APPROVE POSITION","Able To Approve Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ABLE TO APPROVE LENGTH","Able To Approve Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"APPROVAL LIMIT POSITION","Approval Limit Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"APPROVAL LIMIT LENGTH","Approval Limit Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BUDGET OVERRIDE POSITION","Budget Override Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BUDGET OVERRIDE LENGTH","Budget Override Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEFAULT FILE OUTPUT DEVICE POSITION","Default File Output Device Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEFAULT FILE OUTPUT DEVICE LENGTH","Default File Output Device Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEFAULT PRINTER POSITION","Default Printer Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEFAULT PRINTER LENGTH","Default Printer Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"DEPARTMENT POSITION","Department Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEPARTMENT LENGTH","Department Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"EMAIL POSITION","Email Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"EMAIL LENGTH","Email Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"GTKRC POSITION","Gtkrc Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"GTKRC LENGTH","Gtkrc Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HELP INCLUSION TYPE POSITION","Help Inclusion Type Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HELP INCLUSION TYPE LENGTH","Help Inclusion Type Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"JOB DESCRIPTION POSITION","Job Description Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"JOB DESCRIPTION LENGTH","Job Description Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PHONE NUMBER POSITION","Phone Number Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PHONE NUMBER LENGTH","Phone Number Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SITE IDENTIFICATION POSITION","Site Identification Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SITE IDENTIFICATION LENGTH","Site Identification Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUPERVISOR POSITION","Supervisor Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUPERVISOR LENGTH","Supervisor Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"USER NAME POSITION","User Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"USER NAME LENGTH","User Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WEB LOGIN POSITION","Web Login Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WEB LOGIN LENGTH","Web Login Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WEB PASSWORD POSITION","Web Password Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WEB PASSWORD LENGTH","Web Password Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(24+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT USERS DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_SELECT_USERS_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT USERS SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ABLE TO APPROVE] FALSE","Select when Able To Approve False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][ABLE TO APPROVE] TRUE","Select when Able To Approve True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][BUDGET OVERRIDE] FALSE","Select when Budget Override False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][BUDGET OVERRIDE] TRUE","Select when Budget Override True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][HELP INCLUSION TYPE] FALSE","Select when Help Inclusion Type False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][HELP INCLUSION TYPE] TRUE","Select when Help Inclusion Type True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][SUPERVISOR] FALSE","Select when Supervisor False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [USERS][SUPERVISOR] TRUE","Select when Supervisor True","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][APPROVAL LIMIT]","Range on Approval Limit","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][APPROVAL LIMIT]","From Approval Limit","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][APPROVAL LIMIT]","To Approval Limit","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEFAULT FILE OUTPUT DEVICE]","Range on Default File Output Device","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEFAULT FILE OUTPUT DEVICE]","From Default File Output Device","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEFAULT FILE OUTPUT DEVICE]","To Default File Output Device","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEFAULT PRINTER]","Range on Default Printer","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEFAULT PRINTER]","From Default Printer","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEFAULT PRINTER]","To Default Printer","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][DEPARTMENT]","Range on Department","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][DEPARTMENT]","From Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][DEPARTMENT]","To Department","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][EMAIL]","Range on Email","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][EMAIL]","From Email","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][EMAIL]","To Email","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][GTKRC]","Range on Gtkrc","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][GTKRC]","From Gtkrc","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][GTKRC]","To Gtkrc","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][PHONE NUMBER]","Range on Phone Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][PHONE NUMBER]","From Phone Number","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][PHONE NUMBER]","To Phone Number","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][SITE IDENTIFICATION]","Range on Site Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][SITE IDENTIFICATION]","From Site Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][SITE IDENTIFICATION]","To Site Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER IDENTIFICATION]","Range on User Identification","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER IDENTIFICATION]","From User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER IDENTIFICATION]","To User Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][USER NAME]","Range on User Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][USER NAME]","From User Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][USER NAME]","To User Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WEB LOGIN]","Range on Web Login","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WEB LOGIN]","From Web Login","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WEB LOGIN]","To Web Login","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [USERS][WEB PASSWORD]","Range on Web Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [USERS][WEB PASSWORD]","From Web Password","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [USERS][WEB PASSWORD]","To Web Password","","",0,15,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,14,"SELECT USERS SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(26+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT USERS SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void MNU_ARCHIVE_MTN_ARCHIVES_SEARCH()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE MTN ARCHIVES.SEARCH","mtnmst.EXT ARCHIVE \"MTN ARCHIVES.SEARCH\"");
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
static void SCN_SELECT_CLASS_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT CLASS BROWSE");
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
		ADVaddwdgt(scrn,5,"","Search:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(19+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT CLASS BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_CLASS_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT CLASS DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Class:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CLASS POSITION","Class Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CLASS LENGTH","Class Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"DESCRIPTION POSITION","Description Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESCRIPTION LENGTH","Description Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME POSITION","Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME LENGTH","Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(24+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT CLASS DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_SELECT_CLASS_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT CLASS SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHCLS][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHCLS][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHCLS][CLASS]","Range on Class","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHCLS][CLASS]","From Class","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHCLS][CLASS]","To Class","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHCLS][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHCLS][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHCLS][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,14,"SELECT CLASS SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(26+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT CLASS SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_CLASS_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT CLASS SEARCH BROWSE EXPRESSION");
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
			temp1=Rmalloc(37+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT CLASS SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void SCN_SELECT_SUBJECT_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT SUBJECT BROWSE");
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
		ADVaddwdgt(scrn,5,"","Search:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(21+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT SUBJECT BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_SUBJECT_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT SUBJECT DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Title:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"DESCRIPTION POSITION","Description Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESCRIPTION LENGTH","Description Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUBJECT POSITION","Subject Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUBJECT LENGTH","Subject Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TITLE POSITION","Title Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TITLE LENGTH","Title Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(26+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT SUBJECT DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_SELECT_SUBJECT_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT SUBJECT SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHSUB][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHSUB][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHSUB][SUBJECT]","Range on Subject","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHSUB][SUBJECT]","From Subject","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHSUB][SUBJECT]","To Subject","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHSUB][TITLE]","Range on Title","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHSUB][TITLE]","From Title","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHSUB][TITLE]","To Title","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,14,"SELECT SUBJECT SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(28+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT SUBJECT SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_SUBJECT_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","SELECT SUBJECT SEARCH BROWSE EXPRESSION");
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
			temp1=Rmalloc(39+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [SELECT SUBJECT SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("QUERY");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");

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
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void LST_ARCHIVES_SOURCES_FROM()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","ARCHIVES SOURCES FROM");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
		ScrolledListDFincvir(list,"ARCHIVE","ARCHIVES","",0);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=3;
		list->conmod=stralloc("FROM [ARCHIVES][SOURCE]");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=1;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Manual");
		addAPPlib(list->list,"RDA Text Browser");
		addAPPlib(list->list,"Scanner");
		addAPPlib(list->list,"Copy/Move Archive");
		addAPPlib(list->list,"Report Capture");
		addAPPlib(list->list,"DoFTPzip/CronFTPzip");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(21+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [ARCHIVES SOURCES FROM], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void LST_ARCHIVES_SOURCES_TO()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","ARCHIVES SOURCES TO");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
		ScrolledListDFincvir(list,"ARCHIVE","ARCHIVES","",0);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=3;
		list->conmod=stralloc("TO [ARCHIVES][SOURCE]");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=1;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Manual");
		addAPPlib(list->list,"RDA Text Browser");
		addAPPlib(list->list,"Scanner");
		addAPPlib(list->list,"Copy/Move Archive");
		addAPPlib(list->list,"Report Capture");
		addAPPlib(list->list,"DoFTPzip/CronFTPzip");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [ARCHIVES SOURCES TO], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void RPT_EMAILARCHIVE2()
{
	int x=0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	short engine=0;
/*
	RDApval *pval=NULL,*pvals=NULL;
	int y,z;
	short longest=132;
	int numpvars=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	char *defdir=NULL;
*/
	int imageoff=0;
	RDAimage *image=NULL;
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;

	char *temp1=NULL,*temp2=NULL,*temp3=NULL;



	memset(stemp,0,101);
	rptname=Rmalloc(13+1);
	sprintf(rptname,"EMAILARCHIVE2");
	t=Rmalloc(16+1);
	sprintf(t,"Email an Archive");
	desc=Rmalloc(75+1);
	sprintf(desc,"This output definition is used to Email an Archiven from a Maintain Master.");
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,4,FALSE,0,NULL,FALSE,0,1,0,pvars,0,0,0,FALSE,"");

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

/* ARCHIVE ARCHIVES SEARCH FILE */

	temp1=Rmalloc(145+1);
	sprintf(temp1,"%s%s",
		"([ARCHIVE][ARCHIVES][DELETEFLAG]=FALSE) AND ([ARCHIVE][ARCHIVES][WORK GROUP]=[WORK GROUP]) AND ([ARC",
		"HIVE][ARCHIVES][ARCHIVE NAME]=[ARCHIVE NAME])");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHIVES",0,NULL,NULL,"ARCHIVE KEY 1",TRUE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"WORK GROUP","[WORK GROUP]");
		addsearchconfld(s,"ARCHIVE NAME","[ARCHIVE NAME]");
	}
	if(t!=NULL) Rfree(t);


/* ARCHIVE ARCHTYPE SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"ARCHIVE","ARCHTYPE",2,"ARCHIVE","ARCHIVES","ARCHTYPE KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"TYPE","[ARCHIVES][TYPE]");
	}


/* SECURITY USERS SEARCH FILE */

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	x=addsearchfileADV2(tmp,"SECURITY","USERS",3,"ARCHIVE","ARCHIVES","USERS KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[USER IDENTIFICATION]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* ARCHIVE NAME VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"ARCHIVE NAME",1,60,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* WORK GROUP VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"WORK GROUP",1,15,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(22+1);
	sprintf(temp1,"%s",
		"[LOGIN IDENTIFICATION]");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,15,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* RECIPIENT NAME VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"RECIPIENT NAME",1,40,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* RECIPIENT EMAIL ADDRESS VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"RECIPIENT EMAIL ADDRESS",1,60,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* SUBJECT VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"SUBJECT",1,60,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* BODY VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"BODY",14,999,temp1,0,2);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ABSOLUTE FILENAME VIRTUAL FIELD */
	temp1=Rmalloc(149+1);
	sprintf(temp1,"%s",
		"(\"[CURRENT DIRECTORY]+\"/\"+ARCHIVE/.\"+(ADD_DASHES([ARCHIVE][ARCHIVES][WORK GROUP]))+\"/\"+[ARCHIVE][ARCHIVES][FILENAME])");
	addvirtuallineADV(tmp,"ABSOLUTE FILENAME",1,400,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* ATTACHMENT NAME VIRTUAL FIELD */
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"[ARCHIVE][ARCHIVES][FILENAME]");
	addvirtuallineADV(tmp,"ATTACHMENT NAME",1,80,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);





/* SORT DEFINITIONS */

	addsortline(tmp,0,"[ARCHIVE][ARCHIVES][WORK GROUP]");
	addsortline(tmp,0,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]");


	SetReportDefaultEngine(tmp);


/* CONTROL BREAKS */

	addcontrolline(tmp,"[ARCHIVE][ARCHIVES][WORK GROUP]",0);
	addcontrolline(tmp,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"DETAIL LINES","SENDATTACH-1",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[BODY]",0,"BODY");
		addimagepval(image,0,"[ABSOLUTE FILENAME]",0,"FILENAME TO BE ATTACHED");
		addimagepval(image,0,"[ATTACHMENT NAME]",0,"EMAIL ATTACHMENT TO BE NAMED");
		addimagepval(image,0,"[SECURITY][USERS][USER NAME]",0,"NAME OF SENDER");
		addimagepval(image,0,"[RECIPIENT EMAIL ADDRESS]",0,"RECIPIENT EMAIL ADDRESS");
		addimagepval(image,0,"[SECURITY][USERS][EMAIL]",0,"SENDER EMAIL ADDRESS");
		addimagepval(image,0,"[SUBJECT]",0,"SUBJECT");
	}


	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+7+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [ARCHIVE] Report [EMAILARCHIVE2], Can Not Save Report!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void LST_FROM_CLASS_LIST()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","FROM CLASS LIST");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=0;
		ScrolledListDFincvir(list,"ARCHIVE","ARCHIVES","",0);
	temp1=Rmalloc(38+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHCLS][DELETEFLAG]=FALSE)");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("ARCHIVE");
		list->ffinfo->file=stralloc("ARCHCLS");
		list->ffinfo->keyname=stralloc("ARCHCLS KEY");
		list->contype=3;
		list->conmod=stralloc("FROM [ARCHIVES][CLASS]");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(447+1);
	sprintf(temp1,"%s",
		"IF([ARCHIVE][ARCHCLS][CLASS]<10) THEN RETURN_VALUE=\"0\"+((INTEGER_TO_STRING([ARCHIVE][ARCHCLS][CLASS]))+\" \"+[ARCHIVE][ARCHCLS][NAME]) ELSE RETURN_VALUE=((INTEGER_TO_STRING([ARCHIVE][ARCHCLS][CLASS]))+\" \"+[ARCHIVE][ARCHCLS][NAME]) FI");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(46+1);
	sprintf(temp1,"%s",
		"(STRING_TO_VALUE([FROM CLASS LIST][1,2]))");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [FROM CLASS LIST], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void LST_TO_CLASS_LIST()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","TO CLASS LIST");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=0;
		ScrolledListDFincvir(list,"ARCHIVE","ARCHIVES","",0);
		ScrolledListDFincvir(list,"ARCHIVE","ARCHCLS","",0);
	temp1=Rmalloc(38+1);
	sprintf(temp1,"%s",
		"([ARCHIVE][ARCHCLS][DELETEFLAG]=FALSE)");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("ARCHIVE");
		list->ffinfo->file=stralloc("ARCHCLS");
		list->ffinfo->keyname=stralloc("ARCHCLS KEY");
		list->contype=3;
		list->conmod=stralloc("TO [ARCHIVES][CLASS]");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(447+1);
	sprintf(temp1,"%s",
		"IF([ARCHIVE][ARCHCLS][CLASS]<10) THEN RETURN_VALUE=(\"0\"+(INTEGER_TO_STRING([ARCHIVE][ARCHCLS][CLASS]))+\" \"+[ARCHIVE][ARCHCLS][NAME]) ELSE RETURN_VALUE=((INTEGER_TO_STRING([ARCHIVE][ARCHCLS][CLASS]))+\" \"+[ARCHIVE][ARCHCLS][NAME]) FI");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(44+1);
	sprintf(temp1,"%s",
		"(STRING_TO_VALUE([TO CLASS LIST][1,2]))");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [TO CLASS LIST], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void SCN_EMAILARCHIVE2_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","EMAILARCHIVE2 RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Instructions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","This process is used to Email an Archive.  The Email button cannot be","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","engaged without a Recipient Name, Recipient Address, and Subject.  Additionally,","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","the Body of the email should not be blank as that can cause problems with delivery.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Delivery of emails can be ascertained from the Email Log (from Help on the Resource Bar).","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Recipient/Message","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,swb_utl5_rgd,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","         Recipient Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RECIPIENT NAME","Recipient Name:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Recipient Email Address:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RECIPIENT EMAIL ADDRESS","Recipient Email Address:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUBJECT","Subject:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   Body:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"BODY","Body:","","",10,65,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(114+1);
		sprintf(temp3,"%s",
			/*"([RECIPIENT NAME]!=\"\") AND ([RECIPIENT EMAIL ADDRESS]!=\"\") AND ([SUBJECT]!=\"\") AND ([BODY]!=\"\")");*/
			"([RECIPIENT NAME]!=\"\") AND ([RECIPIENT EMAIL ADDRESS]!=\"\") AND ([SUBJECT]!=\"\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,20,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(26+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [EMAILARCHIVE2 RANGE SCREEN], Can Not Save Maintain Master!");
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
static void DEF_ARCHIVES_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"WORK GROUP POSITION",TRUE,7,2);
		addDefaultFieldSHORT(def,"WORK GROUP LENGTH",TRUE,7,17);
		addDefaultFieldSHORT(def,"SUBJECT POSITION",TRUE,7,4);
		addDefaultFieldSHORT(def,"SUBJECT LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"CLASS POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"CLASS LENGTH",TRUE,7,2);
		addDefaultFieldSHORT(def,"ARCHIVE NAME POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"ARCHIVE NAME LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"TYPE POSITION",TRUE,7,9);
		addDefaultFieldSHORT(def,"TYPE LENGTH",TRUE,7,5);
		addDefaultFieldSHORT(def,"FILENAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"FILENAME LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"OCR NAME POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCR NAME LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"DATE POSITION",TRUE,7,6);
		addDefaultFieldSHORT(def,"DATE LENGTH",TRUE,7,10);
		addDefaultFieldSHORT(def,"TIME POSITION",TRUE,7,7);
		addDefaultFieldSHORT(def,"TIME LENGTH",TRUE,7,8);
		addDefaultFieldSHORT(def,"SOURCE USER POSITION",TRUE,7,8);
		addDefaultFieldSHORT(def,"SOURCE USER LENGTH",TRUE,7,11);
		addDefaultFieldSHORT(def,"SOURCE POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"SOURCE LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"DESCRIPTION POSITION",TRUE,7,10);
		addDefaultFieldSHORT(def,"DESCRIPTION LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"KEY WORDS POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"KEY WORDS LENGTH",TRUE,7,0);

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
			temp1=Rmalloc(20+14+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES DEFINE LIST], Can Not Save Screen Defaults!");
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
static void SCN_ARCHIVES_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVES BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,5,"","Search:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Browse List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Statistics","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Position","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST POSITION","Position","","",0,6,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Count","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG TOTAL","Count","","",0,6,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"VIEW ARCHIVE","View Archive","","",0,0,29,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(56+1);
		sprintf(temp3,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))>0");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"EMAIL ARCHIVE","Email Archive","","",0,0,20,"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))>0)",temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
#ifndef WIN32
		ADVaddwdgt(scrn,6,"DOWNLOAD ARCHIVE","Download Archive","","",0,0,45,NULL,NULL,"([WORKSTATION VALIDATED]=TRUE)",NULL);
#endif
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
			temp1=Rmalloc(15+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES BROWSE], Can Not Save Maintain Master!");
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
static void SCN_ARCHIVES_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHIVES][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [ARCHIVES][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][ARCHIVE NAME]","Range on Archive Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][ARCHIVE NAME]","From Archive Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][ARCHIVE NAME]","To Archive Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][CLASS]","Range on Class","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"FROM CLASS LIST","From Class","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"TO CLASS LIST","To Class","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][DATE]","Range on Date","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][DATE]","From Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][DATE]","To Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][FILENAME]","Range on Filename","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][FILENAME]","From Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][FILENAME]","To Filename","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(5+1);
		sprintf(temp1,"%s",
			"FALSE");
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
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][OCR NAME]","Range on Ocr Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][OCR NAME]","From Ocr Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][OCR NAME]","To Ocr Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][SOURCE]","Range on Source","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ARCHIVES SOURCES FROM","From Source","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ARCHIVES SOURCES TO","To Source","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][SOURCE USER]","Range on Source User","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][SOURCE USER]","From Source User","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][SOURCE USER]","To Source User","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][SUBJECT]","Range on Subject","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][SUBJECT]","From Subject","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][SUBJECT]","To Subject","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][TIME]","Range on Time","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][TIME]","From Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][TIME]","To Time","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][TYPE]","Range on Type","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][TYPE]","From Type","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][TYPE]","To Type","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [ARCHIVES][WORK GROUP]","Range on Work Group/Folder","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [ARCHIVES][WORK GROUP]","From Work Group","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [ARCHIVES][WORK GROUP]","To Work Group","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,14,"ARCHIVES SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,swb_utl5_rgd,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(22+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_ARCHIVES_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVES DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Archived Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Archive Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Class:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Key Words:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(5+1);
		sprintf(temp1,"%s",
			"FALSE");
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
		ADVaddwdgt(scrn,5,"","Ocr Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Source:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Source User:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Time:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Work Group/Folder:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DATE ARCHIVED POSITION","Archivd Date Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DATE ARCHIVED LENGTH","Archived Date Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARCHIVE NAME POSITION","Archive Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARCHIVE NAME LENGTH","Archive Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CLASS POSITION","Class Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CLASS LENGTH","Class Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DATE POSITION","Date Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DATE LENGTH","Date Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"DESCRIPTION POSITION","Description Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESCRIPTION LENGTH","Description Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FILENAME POSITION","Filename Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FILENAME LENGTH","Filename Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY WORDS POSITION","Key Words Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"KEY WORDS LENGTH","Key Words Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(5+1);
		sprintf(temp1,"%s",
			"FALSE");
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
		ADVaddwdgt(scrn,0,"OCR NAME POSITION","Ocr Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"OCR NAME LENGTH","Ocr Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOURCE POSITION","Source Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOURCE LENGTH","Source Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOURCE USER POSITION","Source User Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOURCE USER LENGTH","Source User Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUBJECT POSITION","Subject Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SUBJECT LENGTH","Subject Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TIME POSITION","Time Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TIME LENGTH","Time Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TYPE POSITION","Type Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TYPE LENGTH","Type Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WORK GROUP POSITION","Work Group/Folder Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WORK GROUP LENGTH","Work Group/Folder Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,swb_utl5_rgd,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(20+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES DEFINE LIST], Can Not Save Maintain Master!");
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
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

static void dctMKDIR(char *d)
{
	char *ret_string;
	int ret_int=0;

	ret_string=RDAmkdir(d,MODE_RWXRWX___,&ret_int);
	if(ret_string!=NULL)
	{
		if(ret_int!=EEXIST)
		{
			prterr("ERROR: Make Financial Management can not make %s Directory/Folder and it doesn't already exist!.  System Reason: (%s), exiting abnormally",d,ret_string);
			Rfree(ret_string);
			exit(1);
		} else {
			prterr("ERROR: Make Financial Management directory [%s] already exist!.",d,ret_string);
			Rfree(ret_string);
		}
	}
	ret_string=RDAchmod(d,MODE_RWXRWX___,&ret_int);
	if(ret_string!=NULL)
	{
		Rfree(ret_string);
	}
}
static void DEF_ARCHIVES_SEARCH_BROWSE_001()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-ADJUSTMENTS(INVCTL)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"ADJ\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_002()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-APPROVAL";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"APPROVE\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_003()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-AUDIT";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"(([ARCHIVE][ARCHIVES][FILENAME] C \"AUDIT\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"ADT\"))");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_004()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-BANKBALANCE";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"BNKUPDATE\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_005()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-CHANGE";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"(([ARCHIVE][ARCHIVES][FILENAME] C \"CHANGE\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"CHG\"))");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_006()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-CHARGES(BANKREC)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"CHG\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_007()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-CLEARCHECK";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"CLCK\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_008()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-DEDUCTIONSMATCH";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"([ARCHIVE][ARCHIVES][FILENAME] C \"DEDUCT\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_009()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-DETAILREPORTS(FINMGT)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"([ARCHIVE][ARCHIVES][FILENAME] C \"DET\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_010()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-DIRDEPOSITDSK";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"DIRDEP\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_011()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-DISTRIBUTION";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"(([ARCHIVE][ARCHIVES][FILENAME] C \"DIST\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"DST\"))");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_012()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-GLMIXEDREPORTS(FINMGT)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"([ARCHIVE][ARCHIVES][FILENAME] C \"MIXED\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_013()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-ISSUES(INVCTL)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"ISS\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_014()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-MAKETRANSACTIONS(PAYROLL)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"([ARCHIVE][ARCHIVES][FILENAME] C \"MTSA\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_015()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-PACKINGLIST";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"([ARCHIVE][ARCHIVES][FILENAME] C \"PACK\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_016()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-PAYDDAC(PAYROLL)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"([ARCHIVE][ARCHIVES][FILENAME] C \"PAYDDAC\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_017()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-PICKLIST";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"PICK\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_018()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-POST";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"POST\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_019()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-PURGE";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"PURGE\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_020()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-REGISTER";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"(([ARCHIVE][ARCHIVES][FILENAME] C \"REG\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"VWETR\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"VWCR\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"VWR\"))");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_021()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-REVERSAL";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"REVERSE\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_022()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-SUMMARYREPORTS(FINMGT)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"([ARCHIVE][ARCHIVES][FILENAME] C \"SUMMARY\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_023()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-TREASURERCSV(VENPMT)";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"TREASURE\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_024()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-UNDOS";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"UN\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_025()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-VOID";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"[ARCHIVE][ARCHIVES][FILENAME] C \"VOID\")");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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
static void DEF_ARCHIVES_SEARCH_BROWSE_026()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;
	char *ddname="RDA-UPDATEBANKREC";

	def=RDAdefaultNEW("ARCHIVE","ARCHIVES SEARCH BROWSE");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldSTRING(def,"QUERY",TRUE,0,"");
		addDefaultFieldSTRING(def,"EXPRESSION",TRUE,14,"(([ARCHIVE][ARCHIVES][FILENAME] C \"RECDEPOSIT\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"RECON\") OR ([ARCHIVE][ARCHIVES][FILENAME] C \"UPDBR\"))");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][WORK GROUP]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][WORK GROUP]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][ARCHIVE NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][ARCHIVE NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][CLASS]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][CLASS]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DATE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DATE]",TRUE,16,"");
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] TRUE",TRUE,10,0);
		addDefaultFieldCHAR(def,"SELECT [ARCHIVES][DELETEFLAG] FALSE",TRUE,10,1);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][FILENAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][FILENAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][OCR NAME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][OCR NAME]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE]",TRUE,10,0);
		addDefaultFieldSHORT(def,"FROM [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldSHORT(def,"TO [ARCHIVES][SOURCE]",TRUE,7,0);
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SOURCE USER]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SOURCE USER]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][SUBJECT]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][SUBJECT]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TIME]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TIME]",TRUE,17,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][TYPE]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][TYPE]",TRUE,1,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][DESCRIPTION]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][DESCRIPTION]",TRUE,14,"");
		addDefaultFieldCHAR(def,"RANGE ON [ARCHIVES][KEY WORDS]",TRUE,10,0);
		addDefaultFieldSTRING(def,"FROM [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldSTRING(def,"TO [ARCHIVES][KEY WORDS]",TRUE,14,"");
		addDefaultFieldINT(def,"FROM CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"TO CLASS LIST",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES FROM",TRUE,13,0);
		addDefaultFieldINT(def,"ARCHIVES SOURCES TO",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+92);
		dctMKDIR(ddname);
#ifndef WIN32
		sprintf(defdir,"%s/%s/%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,ddname,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+7+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES SEARCH BROWSE], Can Not Save Screen Defaults!");
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

static void ADD_DOWNLOAD_ARCHIVE()
{
	int x=0;
	PowerAdd *add=NULL;
	char *defdir=NULL;
	APPlib *args=NULL,*args2=NULL;
	RDAaddsub *subord=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;
	char *temp7=NULL,*temp8=NULL;
	char *temp9=NULL;

/* ADD DESCRIPTION */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");


/* SAVE EXPRESSION */
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");


/* SAVE WARNING */
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");


/* SAVE WARNING DESCRIPTION */
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");


/* QUIT ERROR */
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");


/* QUIT ERROR DESCRIPTION */
	temp7=Rmalloc(3+1);
	sprintf(temp7,"%s",
		"");


/* QUIT WARNING */
	temp8=Rmalloc(3+1);
	sprintf(temp8,"%s",
		"");


/* QUIT WARNING DESCRIPTION */
	temp9=Rmalloc(3+1);
	sprintf(temp9,"%s",
		"");
	add=RDAPowerAddNEW(module,"DOWNLOAD ARCHIVE",temp1,"DOWNLOAD ARCHIVE","DOWNLOAD ARCHIVE DEFINE LIST","",3,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(temp7!=NULL) Rfree(temp7);
	if(temp8!=NULL) Rfree(temp8);
	if(temp9!=NULL) Rfree(temp9);
	if(add!=NULL)
	{

/* POWERADD BUTTONS  */

/* DOWNLOAD ARCHIVE sftpfile BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		temp2=Rmalloc(48+1);
		sprintf(temp2,"%s",
			"(\"ARCHIVE/.\"+[SOURCE DIRECTORY])");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(10+1);
		sprintf(temp2,"%s",
			"[FILENAME]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(16+1);
		sprintf(temp2,"%s",
			"[DESTINATION IP]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(23+1);
		sprintf(temp2,"%s",
			"[DESTINATION DIRECTORY]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(10+1);
		sprintf(temp2,"%s",
			"[USERNAME]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(10+1);
		sprintf(temp2,"%s",
			"[PASSWORD]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"sftpfile","DOWNLOAD ARCHIVE","DOWNLOAD ARCHIVE","ARCHIVE","ARCHCLS","ARCHCLS KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);


/* POWERADD SUPPORTING FILES  */


/* ARCHIVE ARCHWORK SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"ARCHIVE","ARCHWORK",0,0,NULL,"ARCHWORK KEY 1",temp1,1,"LOAD ARCHWORK","SELECT WORK GROUP BROWSE","SELECT WORK GROUP DEFINE LIST","SELECT WORK GROUP SEARCH BROWSE",0);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(12+1);
			sprintf(temp1,"%s",
				"[WORK GROUP]");
			x=addPowerAddSupportingConfld(subord,"WORK GROUP",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

/* ARCHIVE ARCHIVES SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"ARCHIVE","ARCHIVES",0,0,NULL,"ARCHIVE KEY 1",temp1,1,"LOAD ARCHIVE","SELECT ARCHIVE BROWSE","SELECT ARCHIVE DEFINE LIST","SELECT ARCHIVE SEARCH BROWSE",3);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(12+1);
			sprintf(temp1,"%s",
				"[WORK GROUP]");
			x=addPowerAddSupportingConfld(subord,"WORK GROUP",temp1,1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+1);
			sprintf(temp1,"%s",
				"[ARCHIVE NAME]");
			x=addPowerAddSupportingConfld(subord,"ARCHIVE NAME",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}


/* POWERADD EDIT WIDGETS  */


/* WORK GROUP EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"WORK GROUP",1,15,0,temp1,0,"","","",0,NULL,temp2,"","WORK GROUP",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ARCHIVE NAME EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ARCHIVE NAME",1,60,0,temp1,0,"","","",0,NULL,temp2,"","ARCHIVE NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* USERNAME EDIT WIDGET */
		temp1=Rmalloc(19+1);
		sprintf(temp1,"%s",
			"[WORKSTATION LOGIN]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"USERNAME",1,40,0,temp1,0,"","","",0,NULL,temp2,"","USERNAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* PASSWORD EDIT WIDGET */
		temp1=Rmalloc(22+1);
		sprintf(temp1,"%s",
			"[WORKSTATION PASSWORD]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"PASSWORD",1,25,2,temp1,0,"","","",0,NULL,temp2,"","PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* FILENAME EDIT WIDGET */
		temp1=Rmalloc(29+1);
		sprintf(temp1,"%s",
			"[ARCHIVE][ARCHIVES][FILENAME]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"FILENAME",0,0,2,temp1,0,"","","",0,NULL,temp2,"","FILENAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* DESTINATION FILENAME EDIT WIDGET */
		temp1=Rmalloc(10+1);
		sprintf(temp1,"%s",
			"[FILENAME]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"DESTINATION FILENAME",0,0,2,temp1,0,"","","",0,NULL,temp2,"","DESTINATION FILENAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SOURCE DIRECTORY EDIT WIDGET */
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"[ARCHIVE][ARCHWORK][DIRECTORY]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SOURCE DIRECTORY",0,0,0,temp1,0,"","","",0,NULL,temp2,"","SOURCE DIRECTORY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* DESTINATION DIRECTORY EDIT WIDGET */
		temp1=Rmalloc(23+1);
		sprintf(temp1,"%s",
			"[WORKSTATION DOCUMENTS]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"DESTINATION DIRECTORY",0,0,2,temp1,0,"","","",0,NULL,temp2,"","DESTINATION DIRECTORY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* DESTINATION IP EDIT WIDGET */
		temp1=Rmalloc(24+1);
		sprintf(temp1,"%s",
			"[WORKSTATION IP ADDRESS]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"DESTINATION IP",1,40,2,temp1,0,"","","",0,NULL,temp2,"","DESTINATION IP",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.ADD",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.ADD",CURRENTDIRECTORY,module);
#endif
		if(writePowerAdd(defdir,add))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+7+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [ARCHIVE] Power Add [DOWNLOAD ARCHIVE], Can Not Save Power Add!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(args!=NULL) freeapplib(args);
		if(args2!=NULL) freeapplib(args2);
		if(add!=NULL) FreePowerAdd(add);
	}
}
static void SCN_DOWNLOAD_ARCHIVE_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","DOWNLOAD ARCHIVE DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,200,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Archive Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Destination Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Destination Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Destination Ip:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Source Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Username:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Work Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARCHIVE NAME POSITION","Archive Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARCHIVE NAME LENGTH","Archive Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESTINATION DIRECTORY POSITION","Destination Directory Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESTINATION DIRECTORY LENGTH","Destination Directory Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESTINATION FILENAME POSITION","Destination Filename Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESTINATION FILENAME LENGTH","Destination Filename Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESTINATION IP POSITION","Destination Ip Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESTINATION IP LENGTH","Destination Ip Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FILENAME POSITION","Filename Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FILENAME LENGTH","Filename Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PASSWORD POSITION","Password Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PASSWORD LENGTH","Password Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOURCE DIRECTORY POSITION","Source Directory Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOURCE DIRECTORY LENGTH","Source Directory Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"USERNAME POSITION","Username Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"USERNAME LENGTH","Username Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WORK GROUP POSITION","Work Group Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WORK GROUP LENGTH","Work Group Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(28+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [DOWNLOAD ARCHIVE DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_DOWNLOAD_ARCHIVE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","DOWNLOAD ARCHIVE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","            Work Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD ARCHWORK","Load Archwork","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHWORK","Previous Load Archwork","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"WORK GROUP","Work Group:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHWORK","Next Load Archwork","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHWORK][NAME]","Work Group Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          Archive Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD ARCHIVE","Load Archive","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHIVE","Previous Load Archive","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARCHIVE NAME","Archive Name:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHIVE","Next Load Archive","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","              Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"FILENAME","FILENAME","","",0,60,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Workstation IP Address:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"DESTINATION IP","Workstation IP Adddress","","",0,40,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","              Username:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"USERNAME","Username:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","              Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PASSWORD","Password:","","",0,25,3,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   Documents Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESTINATION DIRECTORY","Documents Directory:","","",0,60,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(16+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [DOWNLOAD ARCHIVE], Can Not Save Maintain Master!");
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
static void VIR_DATE_ARCHIVED()
{
	DFvirtual *virfld=NULL;
	char *temp1=NULL;

	virfld=DFvirtualNEW("ARCHIVE","DATE ARCHIVED");
	if(virfld!=NULL)
	{
		virfld->vtype=0;
		virfld->type=16;
		DFvirtualDFincvir(virfld,"ARCHIVE","ARCHIVES","",0);
		virfld->avail2rsrc=1;
		virfld->type=16;
		virfld->length=10;
		virfld->when=0;
	temp1=Rmalloc(110+1);
	sprintf(temp1,"%s%s",
		"[ARCHIVE][ARCHIVES][DATE][1,7,4]+\"-\"+[ARCHIVE][ARCHIVES][DATE][1,1,2]+\"-\"+[ARCHIVE][ARCHIVES][DA",
		"TE][1,4,2]");
		virfld->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		virfld->desc=stralloc("");

		if(writeDFvirtual(module,virfld))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+7+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [ARCHIVE] Virtual Field [DATE ARCHIVED], Can Not Save Virtual Field!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(virfld!=NULL) FreeDFvirtual(virfld);
	}
}
void MTNARCHIVES()
{
	MTN_MTN_ARCHIVES();
	MTN_MTN_ARCHIVES_BROWSE();
	MNU_ARCHIVE_MTN_ARCHIVES();
	LST_ARCHIVES_SOURCES();
	SCN_ARCHIVES_BROWSE();
	SCN_ARCHIVES_DEFINE_LIST();
	SCN_ARCHIVES_MAINTAIN_SCREEN();
	SCN_ARCHIVES_SEARCH_BROWSE();
	SCN_ARCHIVES_SEARCH_BROWSE_EXPRESSION();
	SCN_SELECT_ARCHIVE_TYPE_BROWSE();
	SCN_SELECT_ARCHIVE_TYPE_DEFINE_LIST();
	SCN_SELECT_ARCHIVE_TYPE_SEARCH_BROWSE();
	SCN_SELECT_ARCHIVE_TYPE_SEARCH_BROWSE_EXPRESSION();
	SCN_SELECT_USERS_BROWSE();
	SCN_SELECT_USERS_DEFINE_LIST();
	SCN_SELECT_USERS_SEARCH_BROWSE();
	DEF_ARCHIVES_DEFINE_LIST();
	MTN_MTN_ARCHIVES_SEARCH();
	MNU_ARCHIVE_MTN_ARCHIVES_SEARCH();
	SCN_SELECT_CLASS_BROWSE();
	SCN_SELECT_CLASS_DEFINE_LIST();
	SCN_SELECT_CLASS_SEARCH_BROWSE();
	SCN_SELECT_CLASS_SEARCH_BROWSE_EXPRESSION();
	SCN_SELECT_SUBJECT_BROWSE();
	SCN_SELECT_SUBJECT_DEFINE_LIST();
	SCN_SELECT_SUBJECT_SEARCH_BROWSE();
	SCN_SELECT_SUBJECT_SEARCH_BROWSE_EXPRESSION();
	DEF_ARCHIVES_SEARCH_BROWSE();
	LST_ARCHIVES_SOURCES_FROM();
	LST_ARCHIVES_SOURCES_TO();
	RPT_EMAILARCHIVE2();
	LST_FROM_CLASS_LIST();
	LST_TO_CLASS_LIST();
	SCN_EMAILARCHIVE2_RANGE_SCREEN();
	DEF_ARCHIVES_SEARCH_BROWSE_001();
	DEF_ARCHIVES_SEARCH_BROWSE_002();
	DEF_ARCHIVES_SEARCH_BROWSE_003();
	DEF_ARCHIVES_SEARCH_BROWSE_004();
	DEF_ARCHIVES_SEARCH_BROWSE_005();
	DEF_ARCHIVES_SEARCH_BROWSE_006();
	DEF_ARCHIVES_SEARCH_BROWSE_007();
	DEF_ARCHIVES_SEARCH_BROWSE_008();
	DEF_ARCHIVES_SEARCH_BROWSE_009();
	DEF_ARCHIVES_SEARCH_BROWSE_010();
	DEF_ARCHIVES_SEARCH_BROWSE_011();
	DEF_ARCHIVES_SEARCH_BROWSE_012();
	DEF_ARCHIVES_SEARCH_BROWSE_013();
	DEF_ARCHIVES_SEARCH_BROWSE_014();
	DEF_ARCHIVES_SEARCH_BROWSE_015();
	DEF_ARCHIVES_SEARCH_BROWSE_016();
	DEF_ARCHIVES_SEARCH_BROWSE_017();
	DEF_ARCHIVES_SEARCH_BROWSE_018();
	DEF_ARCHIVES_SEARCH_BROWSE_019();
	DEF_ARCHIVES_SEARCH_BROWSE_020();
	DEF_ARCHIVES_SEARCH_BROWSE_021();
	DEF_ARCHIVES_SEARCH_BROWSE_022();
	DEF_ARCHIVES_SEARCH_BROWSE_023();
	DEF_ARCHIVES_SEARCH_BROWSE_024();
	DEF_ARCHIVES_SEARCH_BROWSE_025();
	DEF_ARCHIVES_SEARCH_BROWSE_026();
	ADD_DOWNLOAD_ARCHIVE();
	SCN_DOWNLOAD_ARCHIVE_DEFINE_LIST();
	SCN_DOWNLOAD_ARCHIVE();
	VIR_DATE_ARCHIVED();
}
