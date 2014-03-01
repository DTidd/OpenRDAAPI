/* ARCHIVE's - Make (ADD WORKSTATION ARCHIVE) Power Add */
#include <cstdio>
#include <mkadd.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void ADD_ADD_WORKSTATION_ARCHIVE()
{
	int x=0;
	PowerAdd *add=NULL;
	char *defdir=NULL;
	APPlib *args=NULL,*args2=NULL;
	RDAaddsub *subord=NULL;
	AddWrite *Write=NULL;
	MaintainButton *button=NULL;
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
	temp2=Rmalloc(20+1);
	sprintf(temp2,"%s",
		"([SAVE ERROR]!=\"\")");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(12+1);
	sprintf(temp3,"%s",
		"[SAVE ERROR]");


/* SAVE WARNING */
	temp4=Rmalloc(22+1);
	sprintf(temp4,"%s",
		"([SAVE WARNING]!=\"\")");


/* SAVE WARNING DESCRIPTION */
	temp5=Rmalloc(14+1);
	sprintf(temp5,"%s",
		"[SAVE WARNING]");


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
	add=RDAPowerAddNEW(module,"ADD WORKSTATION ARCHIVE",temp1,"ADD WORKSTATION ARCHIVE","ADD WORKSTATION ARCHIVE DEFINE LIST","",1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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

/* ADD WORKGROUP RUNPOWERADD BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"ADD WORK GROUP");
		x=addPowerAddButton(add,"RUNPOWERADD","ADD WORKGROUP","ADD WORKGROUP","ARCHIVE","ARCHWORK","ARCHWORK KEY 1",temp1,0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD WORK GROUP]","ADD_WORK_GROUP");
		}
/* SCAN DOCUMENT scandoc BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		x=addPowerAddButton(add,"scandoc","Scan Documents","SCAN DOCUMENT","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD DIRECTORY NAME]","ADD_SCAN_DIRECTORY");
		}
/* CreateDirectory CreateDir BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		temp2=Rmalloc(44+1);
		sprintf(temp2,"%s",
			"\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"CreateDir","Create Directory","CreateDirectory","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
/* SFTPFILE sftpfile BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		temp2=Rmalloc(44+1);
		sprintf(temp2,"%s",
			"\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(14+1);
		sprintf(temp2,"%s",
			"[ADD FILENAME]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(24+1);
		sprintf(temp2,"%s",
			"[WORKSTATION IP ADDRESS]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(23+1);
		sprintf(temp2,"%s",
			"[WORKSTATION DOCUMENTS]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(19+1);
		sprintf(temp2,"%s",
			"[WORKSTATION LOGIN]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(22+1);
		sprintf(temp2,"%s",
			"[WORKSTATION PASSWORD]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(1+1);
		sprintf(temp2,"%s",
			"1");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(14+1);
		sprintf(temp2,"%s",
			"[ADD FILENAME]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"sftpfile","SFTPFILE","SFTPFILE","ARCHIVE","ARCHIVES","ARCHIVE KEY 1",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);


/* POWERADD SUPPORTING FILES  */


/* ARCHIVE ARCHWORK SUPPORTING FILE */

		temp1=Rmalloc(39+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHWORK][DELETEFLAG]=FALSE)");
		x=addPowerAddSupportingADV(add,"ARCHIVE","ARCHWORK",0,0,NULL,"ARCHWORK KEY 1",temp1,1,"LOAD WORK GROUP","SELECT WORK GROUP BROWSE","SELECT WORK GROUP DEFINE LIST","SELECT WORK GROUP SEARCH BROWSE",0);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WORK GROUP]");
			x=addPowerAddSupportingConfld(subord,"WORK GROUP",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

/* ARCHIVE ARCHTYPE SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"ARCHIVE","ARCHTYPE",0,0,NULL,"ARCHTYPE KEY",temp1,1,"LOAD ARCHTYPE","SELECT ARCHIVE TYPE BROWSE","SELECT ARCHIVE TYPE DEFINE LIST","SELECT ARCHIVE TYPE SEARCH BROWSE",1);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(10+1);
			sprintf(temp1,"%s",
				"[ADD TYPE]");
			x=addPowerAddSupportingConfld(subord,"TYPE",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

/* ARCHIVE ARCHIVES SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"ARCHIVE","ARCHIVES",1,0,NULL,"ARCHIVE KEY 1",temp1,1,"LOAD ARCHIVES","SELECT ARCHIVE BROWSE","SELECT ARCHIVE DEFINE LIST","SELECT ARCHIVE SEARCH BROWSE",3);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WORK GROUP]");
			x=addPowerAddSupportingConfld(subord,"WORK GROUP",temp1,1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD ARCHIVE NAME]");
			x=addPowerAddSupportingConfld(subord,"ARCHIVE NAME",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

/* ARCHIVE ARCHCLS SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"ARCHIVE","ARCHCLS",0,0,NULL,"ARCHCLS KEY",temp1,1,"LOAD ARCHCLS","SELECT CLASS BROWSE","SELECT CLASS DEFINE LIST","SELECT CLASS SEARCH BROWSE",0);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"[ADD CLASS]");
			x=addPowerAddSupportingConfld(subord,"CLASS",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

/* ARCHIVE ARCHSUB SUPPORTING FILE */

		temp1=Rmalloc(38+1);
		sprintf(temp1,"%s",
			"([ARCHIVE][ARCHSUB][DELETEFLAG]=FALSE)");
		x=addPowerAddSupportingADV(add,"ARCHIVE","ARCHSUB",1,0,NULL,"ARCHSUB KEY",temp1,1,"LOAD ARCHSUB","SELECT SUBJECT BROWSE","SELECT SUBJECT DEFINE LIST","SELECT SUBJECT SEARCH BROWSE",0);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(13+1);
			sprintf(temp1,"%s",
				"[ADD SUBJECT]");
			x=addPowerAddSupportingConfld(subord,"SUBJECT",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}


/* POWERADD WRITES  */


/* ARCHIVE ARCHIVES POWER WRITE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddWrite(add,"ARCHIVE","ARCHIVES",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WORK GROUP]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][WORK GROUP]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD ARCHIVE NAME]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][ARCHIVE NAME]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+1);
			sprintf(temp1,"%s",
				"[ADD FILENAME]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][FILENAME]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[SYSTEM DATE-YYYY]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][DATE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+1);
			sprintf(temp1,"%s",
				"[SYSTEM TIME]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][TIME]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+1);
			sprintf(temp1,"%s",
				"[LOGIN IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][SOURCE USER]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(1+1);
			sprintf(temp1,"%s",
				"0");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][SOURCE]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+1);
			sprintf(temp1,"%s",
				"[ADD DESCRIPTION]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][DESCRIPTION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+1);
			sprintf(temp1,"%s",
				"[ADD TYPE]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][TYPE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+1);
			sprintf(temp1,"%s",
				"[ADD SUBJECT]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][SUBJECT]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"[ADD CLASS]");
			x=addPowerAddWriteField(Write,"[ARCHIVE][ARCHIVES][CLASS]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}


/* POWERADD EDIT WIDGETS  */


/* ADD NAME STYLE EDIT WIDGET */
		temp1=Rmalloc(73+1);
		sprintf(temp1,"%s",
			"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ADD ARCHIVE DEFAULT NAME STYLE]\")))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD NAME STYLE",7,2,3,temp1,0,"","","",0,NULL,temp2,"","ADD NAME STYLE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD UPDATE EXISTING EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD UPDATE EXISTING",10,1,3,temp1,0,"","","",0,NULL,temp2,"","ADD UPDATE EXISTING",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD WORK GROUP EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD WORK GROUP",1,15,3,temp1,0,"","","",0,NULL,temp2,"","ADD WORK GROUP",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD MODULE EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD MODULE",1,15,3,temp1,0,"","","",0,NULL,temp2,"","ADD MODULE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD ARCHIVE NAME EDIT WIDGET */
		temp1=Rmalloc(2301+1);
		sprintf(temp1,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			"IF(([ADD UPDATE EXISTING]=TRUE) OR (([ADD NAME STYLE]=[LAST NAME STYLE]) AND ([ADD MODULE]=[LAST MOD",
			"ULE]) AND ([ADD FILENAME]=[LAST FILENAME]) AND ([ADD NAME STYLE]!=12)) \nOR ([ADD NAME STYLE]=0)) \n",
			"	THEN RETURN_VALUE=[ADD ARCHIVE NAME] \nELSE IF(([ADD NAME STYLE]=1)) THEN \n	RETURN_VALUE=[SYSTEM D",
			"ATE-YYYY][7,4]+\"-\"+[SYSTEM DATE-YYYY][1,2]+\"-\"+\n		[SYSTEM DATE-YYYY][4,2]+\".\"\nELSE IF(([ADD ",
			"NAME STYLE]=2)) THEN \n	RETURN_VALUE=[SYSTEM DATE-YYYY][7,4]+\"-\"+[SYSTEM DATE-YYYY][1,2]+\"-\"+\n	",
			"	[SYSTEM DATE-YYYY][4,2]+\".\"+[ADD MODULE]+\".\"\nELSE IF(([ADD NAME STYLE]=3)) THEN \n	RETURN_VALU",
			"E=[SYSTEM DATE-YYYY][7,4]+\"-\"+[SYSTEM DATE-YYYY][1,2]+\"-\"+\n		[SYSTEM DATE-YYYY][4,2]+\".\"+[ADD",
			" MODULE]+\".\"+[LOGIN IDENTIFICATION]+\".\"\nELSE IF(([ADD NAME STYLE]=4)) THEN \n	RETURN_VALUE=[SYS",
			"TEM DATE-YYYY][7,4]+\"-\"+[SYSTEM DATE-YYYY][1,2]+\"-\"+\n		[SYSTEM DATE-YYYY][4,2]+\".\"+[LOGIN IDE",
			"NTIFICATION]+\".\"\nELSE IF(([ADD NAME STYLE]=5)) THEN \n	RETURN_VALUE=[SYSTEM DATE-YYYY][7,4]+\"-\"",
			"+[SYSTEM DATE-YYYY][1,2]+\"-\"+\n		[SYSTEM DATE-YYYY][4,2]+\".\"+[LOGIN IDENTIFICATION]+\".\"+[ADD M",
			"ODULE]+\".\"\nELSE IF(([ADD NAME STYLE]=6)) THEN \n	RETURN_VALUE=[SYSTEM DATE-YYYY][7,4]+\"-\"+[SYST",
			"EM DATE-YYYY][1,2]+\".\"\nELSE IF(([ADD NAME STYLE]=7)) THEN \n	RETURN_VALUE=[SYSTEM DATE-YYYY][7,4]",
			"+\"-\"+[SYSTEM DATE-YYYY][1,2]+\".\"+[ADD MODULE]+\".\"\nELSE IF(([ADD NAME STYLE]=8)) THEN \n	RETUR",
			"N_VALUE=[SYSTEM DATE-YYYY][7,4]+\"-\"+[SYSTEM DATE-YYYY][1,2]+\".\"+\n		[ADD MODULE]+\".\"+[LOGIN ID",
			"ENTIFICATION]+\".\"\nELSE IF(([ADD NAME STYLE]=9)) THEN \n	RETURN_VALUE=[SYSTEM DATE-YYYY][7,4]+\"-",
			"\"+[SYSTEM DATE-YYYY][1,2]+\".\"+\n		[LOGIN IDENTIFICATION]+\".\"\nELSE IF(([ADD NAME STYLE]=10)) TH",
			"EN \n	RETURN_VALUE=[SYSTEM DATE-YYYY][7,4]+\"-\"+[SYSTEM DATE-YYYY][1,2]+\".\"+\n		[LOGIN IDENTIFICA",
			"TION]+\".\"+[ADD MODULE]+\".\"\nELSE IF([ADD NAME STYLE]=11) THEN \n RETURN_VALUE=[SYSTEM DATE-YYYY]",
			"[7,4]+\"-\"+[SYSTEM DATE-YYYY][1,2]+\".\"+[SYSTEM TIME][1,2]+\"-\"+[SYSTEM TIME][4,2]+\".\"+[LOGIN I",
			"DENTIFICATION]+\".\" \nELSE IF([ADD NAME STYLE]=12) THEN \nRETURN_VALUE=[SYSTEM DATE-YYYY][7,4]+\"-",
			"\"+[SYSTEM DATE-YYYY][1,2]+\".\"+[SYSTEM TIME][1,2]+\"-\"+[SYSTEM TIME][4,2]+\".\"+[LOGIN IDENTIFICA",
			"TION]+\".\"+[ADD FILENAME]\nELSE RETURN_VALUE=[ADD ARCHIVE NAME] FI FI FI FI FI FI FI FI FI FI FI FI",
			" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD ARCHIVE NAME",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD ARCHIVE NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HOME EDIT WIDGET */
		temp1=Rmalloc(67+1);
		sprintf(temp1,"%s",
			"IF([HOME]=\"\") THEN RETURN_VALUE=\".\" ELSE RETURN_VALUE=[HOME] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HOME",1,80,2,temp1,0,"","","",0,NULL,temp2,"","HOME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* DEFAULT DIRECTORY EDIT WIDGET */
		temp1=Rmalloc(23+1);
		sprintf(temp1,"%s",
			"[WORKSTATION DOCUMENTS]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"DEFAULT DIRECTORY",1,80,2,temp1,0,"","","",0,NULL,temp2,"","DEFAULT DIRECTORY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD DIRECTORY NAME EDIT WIDGET */
		temp1=Rmalloc(540+1);
		sprintf(temp1,"%s%s%s%s%s%s",
			"IF([ADD UPDATE EXISTING]=TRUE) THEN RETURN_VALUE=\"ARCHIVE/.\"+[ARCHIVE][ARCHWORK][DIRECTORY NAME]",
			"\nELSE \nIF(([DEFAULT DIRECTORY]!=\"\") AND ([ADD DIRECTORY NAME]=\"\")) \nTHEN RETURN_VALUE=[DEFAUL",
			"T DIRECTORY] \nELSE IF(([LAST DIRECTORY NAME]!=\"\") AND ([ADD DIRECTORY NAME]!=[LAST DIRECTORY NAME",
			"]) \nAND ([ADD DIRECTORY NAME] NC \"..\")) THEN \n	RETURN_VALUE=[LAST DIRECTORY NAME]+\"/\"+[ADD DIR",
			"ECTORY NAME]\nELSE IF([ADD DIRECTORY NAME]!=\"\") THEN RETURN_VALUE=[ADD DIRECTORY NAME] \nELSE RETU",
			"RN_VALUE=[DEFAULT DIRECTORY] FI FI FI FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD DIRECTORY NAME",1,80,3,temp1,0,"","","",0,NULL,temp2,"","ADD DIRECTORY NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD FROM FILENAME EDIT WIDGET */
		temp1=Rmalloc(119+1);
		sprintf(temp1,"%s%s",
			"IF([ADD UPDATE EXISTING]=TRUE) THEN RETURN_VALUE=[ARCHIVE][ARCHIVES][FILENAME] ELSE RETURN_VALUE=[AD",
			"D FROM FILENAME] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD FROM FILENAME",1,80,2,temp1,0,"","","",0,NULL,temp2,"","ADD FROM FILENAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD FILENAME EDIT WIDGET */
		temp1=Rmalloc(208+1);
		sprintf(temp1,"%s%s%s",
			"IF((([ADD WORK GROUP]!=[LAST WORK GROUP]) OR ([ADD ARCHIVE NAME]!=[LAST ARCHIVE NAME])) AND \n([ADD ",
			"UPDATE EXISTING]=TRUE))\nTHEN RETURN_VALUE=[ARCHIVE][ARCHIVES][FILENAME] ELSE RETURN_VALUE=[ADD FILE",
			"NAME] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD FILENAME",1,80,2,temp1,0,"","","",0,NULL,temp2,"","ADD FILENAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD CLASS EDIT WIDGET */
		temp1=Rmalloc(201+1);
		sprintf(temp1,"%s%s%s",
			"IF((([ADD WORK GROUP]!=[LAST WORK GROUP]) OR ([ADD ARCHIVE NAME]!=[LAST ARCHIVE NAME])) \nAND ([ADD ",
			"UPDATE EXISTING]=TRUE)) THEN RETURN_VALUE=[ARCHIVE][ARCHIVES][CLASS] ELSE RETURN_VALUE=[ADD CLASS] F",
			"I");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD CLASS",8,4,2,temp1,0,"","","",0,NULL,temp2,"","ADD CLASS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD SUBJECT EDIT WIDGET */
		temp1=Rmalloc(205+1);
		sprintf(temp1,"%s%s%s",
			"IF((([ADD WORK GROUP]!=[LAST WORK GROUP]) OR ([ADD ARCHIVE NAME]!=[LAST ARCHIVE NAME])) \nAND ([ADD ",
			"UPDATE EXISTING]=TRUE)) THEN RETURN_VALUE=[ARCHIVE][ARCHIVES][SUBJECT] ELSE RETURN_VALUE=[ADD SUBJEC",
			"T] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD SUBJECT",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD SUBJECT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD TYPE EDIT WIDGET */
		temp1=Rmalloc(199+1);
		sprintf(temp1,"%s%s",
			"IF((([ADD WORK GROUP]!=[LAST WORK GROUP]) OR ([ADD ARCHIVE NAME]!=[LAST ARCHIVE NAME])) \nAND ([ADD ",
			"UPDATE EXISTING]=TRUE)) THEN RETURN_VALUE=[ARCHIVE][ARCHIVES][TYPE] ELSE RETURN_VALUE=[ADD TYPE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TYPE",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD TYPE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD DESCRIPTION EDIT WIDGET */
		temp1=Rmalloc(213+1);
		sprintf(temp1,"%s%s%s",
			"IF((([ADD WORK GROUP]!=[LAST WORK GROUP]) OR ([ADD ARCHIVE NAME]!=[LAST ARCHIVE NAME])) \nAND ([ADD ",
			"UPDATE EXISTING]=TRUE)) THEN RETURN_VALUE=[ARCHIVE][ARCHIVES][DESCRIPTION] ELSE RETURN_VALUE=[ADD DE",
			"SCRIPTION] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD DESCRIPTION",14,800,2,temp1,0,"","","",0,NULL,temp2,"","ADD DESCRIPTION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD TRANSFER TYPE EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TRANSFER TYPE",7,2,3,temp1,0,"","","",0,NULL,temp2,"","ADD TRANSFER TYPE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LAST WORK GROUP EDIT WIDGET */
		temp1=Rmalloc(16+1);
		sprintf(temp1,"%s",
			"[ADD WORK GROUP]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST WORK GROUP",1,15,2,temp1,0,"","","",0,NULL,temp2,"","LAST WORK GROUP",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LAST ARCHIVE NAME EDIT WIDGET */
		temp1=Rmalloc(18+1);
		sprintf(temp1,"%s",
			"[ADD ARCHIVE NAME]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST ARCHIVE NAME",1,40,2,temp1,0,"","","",0,NULL,temp2,"","LAST ARCHIVE NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LAST DIRECTORY NAME EDIT WIDGET */
		temp1=Rmalloc(20+1);
		sprintf(temp1,"%s",
			"[ADD DIRECTORY NAME]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST DIRECTORY NAME",0,0,2,temp1,0,"","","",0,NULL,temp2,"","LAST DIRECTORY NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LAST NAME STYLE EDIT WIDGET */
		temp1=Rmalloc(16+1);
		sprintf(temp1,"%s",
			"[ADD NAME STYLE]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST NAME STYLE",7,2,2,temp1,0,"","","",0,NULL,temp2,"","LAST NAME STYLE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LAST TYPE EDIT WIDGET */
		temp1=Rmalloc(10+1);
		sprintf(temp1,"%s",
			"[ADD TYPE]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST TYPE",1,15,2,temp1,0,"","","",0,NULL,temp2,"","LAST TYPE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LAST MODULE EDIT WIDGET */
		temp1=Rmalloc(12+1);
		sprintf(temp1,"%s",
			"[ADD MODULE]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST MODULE",1,15,2,temp1,0,"","","",0,NULL,temp2,"","LAST MODULE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LAST FILENAME EDIT WIDGET */
		temp1=Rmalloc(14+1);
		sprintf(temp1,"%s",
			"[ADD FILENAME]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST FILENAME",1,80,2,temp1,0,"","","",0,NULL,temp2,"","LAST FILENAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE ERROR 01 EDIT WIDGET */
		temp1=Rmalloc(138+1);
		sprintf(temp1,"%s%s",
			"IF([ADD WORK GROUP]=\"\") THEN RETURN_VALUE=\"Error 01:  You must associate your archive with a work",
			" group.  \"\nELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE ERROR 01",0,999,2,temp1,0,"","","",0,NULL,temp2,"","SAVE ERROR 01",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE ERROR 02 EDIT WIDGET */
		temp1=Rmalloc(131+1);
		sprintf(temp1,"%s%s",
			"IF([ADD ARCHIVE NAME]=\"\") THEN RETURN_VALUE=\"Error 02:  You must name your archive for the index.",
			"  \"\nELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE ERROR 02",0,999,2,temp1,0,"","","",0,NULL,temp2,"","SAVE ERROR 02",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE ERROR 03 EDIT WIDGET */
		temp1=Rmalloc(125+1);
		sprintf(temp1,"%s%s",
			"IF([ADD FILENAME]=\"\") THEN RETURN_VALUE=\"Error 03:  You must provide a filename to archive.  \"\n",
			"ELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE ERROR 03",0,999,2,temp1,0,"","","",0,NULL,temp2,"","SAVE ERROR 03",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE ERROR EDIT WIDGET */
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"([SAVE ERROR 01]+[SAVE ERROR 02]+[SAVE ERROR 03])");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE ERROR",0,0,2,temp1,0,"","","",0,NULL,temp2,"","SAVE ERROR",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE WARNING 01 EDIT WIDGET */
		temp1=Rmalloc(157+1);
		sprintf(temp1,"%s%s",
			"IF([ADD SUBJECT]=\"\") THEN RETURN_VALUE=\"Warning 01:  It is recommended to associate your archive ",
			"with a particular subject.  \"\nELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE WARNING 01",0,0,2,temp1,0,"","","",0,NULL,temp2,"","SAVE WARNING 01",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE WARNING 02 EDIT WIDGET */
		temp1=Rmalloc(150+1);
		sprintf(temp1,"%s%s",
			"IF([ADD CLASS]=0) THEN RETURN_VALUE=\"Warning 02:  It is recommended to associate your archive with ",
			"a particular class.  \"\nELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE WARNING 02",0,0,2,temp1,0,"","","",0,NULL,temp2,"","SAVE WARNING 02",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE WARNING 03 EDIT WIDGET */
		temp1=Rmalloc(185+1);
		sprintf(temp1,"%s%s",
			"IF([ADD DESCRIPTION]=\"\") THEN RETURN_VALUE=\"Warning 03:  Giving your archives a description helps",
			" searching, querying, and utilization of your archive.  \"\nELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE WARNING 03",0,0,2,temp1,0,"","","",0,NULL,temp2,"","SAVE WARNING 03",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SAVE WARNING EDIT WIDGET */
		temp1=Rmalloc(55+1);
		sprintf(temp1,"%s",
			"([SAVE WARNING 01]+[SAVE WARNING 02]+[SAVE WARNING 03])");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SAVE WARNING",0,0,2,temp1,0,"","","",0,NULL,temp2,"","SAVE WARNING",temp3,0,NULL,0,NULL);
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
			temp1=Rmalloc(23+7+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [ARCHIVE] Power Add [ADD WORKSTATION ARCHIVE], Can Not Save Power Add!");
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
static void LST_ADD_WORKSTATION_FILENAME_LIST()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","ADD WORKSTATION FILENAME LIST");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=2;
	temp1=Rmalloc(20+1);
	sprintf(temp1,"%s",
		"[ADD DIRECTORY NAME]");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("WORKSTATION DIRECTORY CONTENTS (SFTP)");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=1;
		list->conmod=stralloc("ADD FILENAME");
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
			temp1=Rmalloc(24+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [ADD WORKSTATION FILENAME LIST], Can Not Save Scrolled List!");
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
static void SCN_ADD_WORKSTATION_ARCHIVE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ADD WORKSTATION ARCHIVE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(19+1);
		sprintf(temp4,"%s",
			"\"LOAD WORK GROUP\"");
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,15,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,7,"PREVIOUS ADD'S","Previous Add's","","",3,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Naming Conventions:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(29+1);
		sprintf(temp3,"%s",
			"([ADD UPDATE EXISTING]=FALSE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,7,"ADD NAME STYLES","Name Style","","",1,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","           Modules:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(31+1);
		sprintf(temp3,"%s",
			"(([ADD UPDATE EXISTING]=FALSE))");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,7,"ADD MODULE LIST","Modules","","",1,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","     Remote Files:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD WORKSTATION FILENAME LIST","Filename List","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","       Transfer Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD TRANSFER TYPES","Transfer Types","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Work Group/Folder:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD WORKGROUP","Add Work Group","","",0,0,31,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(16+1);
		sprintf(temp4,"%s",
			"\"LOAD ARCHCLS\"");
		ADVaddwdgt(scrn,6,"LOAD WORK GROUP","Load","","",0,0,1,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD WORK GROUP","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(13+1);
		sprintf(temp4,"%s",
			"\"ADD CLASS\"");
		ADVaddwdgt(scrn,0,"ADD WORK GROUP","Work Group","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"NEXT LOAD WORK GROUP","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHWORK][NAME]","Name of Work Group","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","             Class:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(16+1);
		sprintf(temp4,"%s",
			"\"LOAD ARCHSUB\"");
		ADVaddwdgt(scrn,6,"LOAD ARCHCLS","Load","","",0,0,1,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHCLS","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(15+1);
		sprintf(temp4,"%s",
			"\"ADD SUBJECT\"");
		ADVaddwdgt(scrn,0,"ADD CLASS","Class","","",0,4,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHCLS","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHCLS][NAME]","Class Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","           Subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(17+1);
		sprintf(temp4,"%s",
			"\"LOAD ARCHTYPE\"");
		ADVaddwdgt(scrn,6,"LOAD ARCHSUB","Load","","",0,0,1,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHSUB","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(12+1);
		sprintf(temp4,"%s",
			"\"ADD TYPE\"");
		ADVaddwdgt(scrn,0,"ADD SUBJECT","Subject","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHSUB","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[ARCHSUB][TITLE]","Subject Title","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","      Archive Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(29+1);
		sprintf(temp3,"%s",
			"([ADD UPDATE EXISTING]=FALSE)");
		temp4=Rmalloc(20+1);
		sprintf(temp4,"%s",
			"\"ADD ARCHIVE NAME\"");
		ADVaddwdgt(scrn,6,"LOAD ARCHTYPE","Load","","",0,0,1,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(29+1);
		sprintf(temp3,"%s",
			"([ADD UPDATE EXISTING]=FALSE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"PREVIOUS LOAD ARCHTYPE","Previous","","",0,0,2,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(29+1);
		sprintf(temp2,"%s",
			"([ADD UPDATE EXISTING]=FALSE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(20+1);
		sprintf(temp4,"%s",
			"\"ADD ARCHIVE NAME\"");
		ADVaddwdgt(scrn,0,"ADD TYPE","Add Type","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(29+1);
		sprintf(temp3,"%s",
			"([ADD UPDATE EXISTING]=FALSE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"NEXT LOAD ARCHTYPE","Next","","",0,0,1,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,0,"[ARCHTYPE][DESCRIPTION]","Archive Type Description","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD UPDATE EXISTING","Update Existing","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   Name of Archive:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(28+1);
		sprintf(temp3,"%s",
			"([ADD UPDATE EXISTING]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"LOAD ARCHIVES","Load","","",0,0,1,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(8+1);
		sprintf(temp4,"%s",
			"\"SAVE\"");
		ADVaddwdgt(scrn,0,"ADD ARCHIVE NAME","Archive Name:","","",0,40,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,""," Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD FILENAME","Filename:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"ADD DESCRIPTION","Description:","","",4,65,0,NULL,NULL,NULL,NULL);
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
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(18+1);
		sprintf(temp4,"%s",
			"\"RESET DEFAULTS\"");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"RESET DEFAULTS","Reset","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(18+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ADD WORKSTATION ARCHIVE], Can Not Save Maintain Master!");
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
static void SCN_ADD_WORKSTATION_ARCHIVE_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ADD WORKSTATION ARCHIVE DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Archive Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Directory Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"ADD ARCHIVE NAME POSITION","Add Archive Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD ARCHIVE NAME LENGTH","Add Archive Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DESCRIPTION POSITION","Add Description Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DESCRIPTION LENGTH","Add Description Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DIRECTORY NAME POSITION","Add Directory Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DIRECTORY NAME LENGTH","Add Directory Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD FILENAME POSITION","Add Filename Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD FILENAME LENGTH","Add Filename Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TYPE POSITION","Add Type Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TYPE LENGTH","Add Type Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WORK GROUP POSITION","Add Work Group Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WORK GROUP LENGTH","Add Work Group Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(30+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ADD WORKSTATION ARCHIVE DEFINE LIST], Can Not Save Maintain Master!");
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
void ADDWORKSTATIONARCHIVE()
{
	ADD_ADD_WORKSTATION_ARCHIVE();
	LST_ADD_WORKSTATION_FILENAME_LIST();
	SCN_ADD_WORKSTATION_ARCHIVE();
	SCN_ADD_WORKSTATION_ARCHIVE_DEFINE_LIST();
}
