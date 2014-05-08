/* OLHELP's - Make (USER PROFILE) Power Add */
#include <cstdio>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <mkadd.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;

static void ADD_USER_PROFILE()
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
	temp2=Rmalloc(380+1);
	sprintf(temp2,"%s",
		"IF([ADD USER IDENTIFICATION]=\"\") THEN RETURN_VALUE=(-1) \nELSE IF([NEW PASSWORD]!=[CONFIRM PASSWORD]) THEN RETURN_VALUE=(-1) \nELSE RETURN_VALUE=0 FI FI");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(841+1);
	sprintf(temp3,"%s%s",
		"IF([ADD USER IDENTIFICATION]=\"\") THEN RETURN_VALUE=\"Error: You MUST enter a User Identification b",
		"efore saving.\" \nELSE IF([NEW PASSWORD]!=[CONFIRM PASSWORD]) THEN RETURN_VALUE=\"Error: Confirmation of New Password is incorrect.  Please ensure the New Password and Confirmation match.  \" \nELSE RETURN_VALUE=\"\" FI FI");


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
	add=RDAPowerAddNEW(module,"USER PROFILE",temp1,"USER PROFILE","USER PROFILE DEFINE LIST","",3,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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

/* MAKE USER DIRECTORY AUTO MKDIR BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		temp2=Rmalloc(25+1);
		sprintf(temp2,"%s",
			"[ADD USER IDENTIFICATION]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"MKDIR","Make User Directory Auto","MAKE USER DIRECTORY AUTO","SECURITY","USERS","USERS KEY",temp1,2,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
/* CREATEMSMTPRC doreport BUTTON  */
		temp1=Rmalloc(77+1);
		sprintf(temp1,"%s",
			"(([SECURITY][USERS][EMAIL]!=\"\") AND ([SECURITY][USERS][SMTP SERVER]!=\"\"))");
		
		args=APPlibNEW();
		addAPPlib(args,"OLHELP");
		addAPPlib(args,"CREATEMSMTPRC");
		x=addPowerAddButton(add,"RUNREPORT","CREATEMSMTPRC","CREATEMSMTPRC","SECURITY","USERS","USERS KEY",temp1,2,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD USER IDENTIFICATION]","USER_IDENTIFICATION");
		}

/* CHECK EMAIL checkemail BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		x=addPowerAddButton(add,"checkemail","CHECK EMAIL","CHECK EMAIL","OLHELP","RSCNOT","RSCNOT KEY",temp1,0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);

/* VALIDATE WORKSTATION setupsftp BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		x=addPowerAddButton(add,"setupsftp","VALIDATE WORKSTATION","VALIDATE WORKSTATION","SECURITY","USERS","USERS KEY",temp1,0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);




/* POWERADD SUPPORTING FILES  */


/* SECURITY USERS SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"SECURITY","USERS",1,0,NULL,"USERS KEY",temp1,1,"LOAD USERS","SELECT USER BROWSE","SELECT USER DEFINE LIST","SELECT USER SEARCH BROWSE",0);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddSupportingConfld(subord,"USER IDENTIFICATION",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

/* UTILITIES MSG-GATEWAY SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"UTILITIES","MSG-GATEWAY",1,0,NULL,"MSG-GATEWAY KEY 1",temp1,1,"LOAD CARRIER","SELECT CARRIER BROWSE","SELECT CARRIER DEFINE LIST","SELECT CARRIER SEARCH BROWSE",1);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(13+1);
			sprintf(temp1,"%s",
				"[ADD CARRIER]");
			x=addPowerAddSupportingConfld(subord,"CARRIER",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD CARRIER TYPE]");
			x=addPowerAddSupportingConfld(subord,"TYPE",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

/* POWERADD WRITES  */


/* SECURITY USERS POWER WRITE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddWrite(add,"SECURITY","USERS",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD PHONE NUMBER]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][PHONE NUMBER]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(21+1);
			sprintf(temp1,"%s",
				"[ADD PHONE EXTENSION]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][PHONE EXTENSION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(21+1);
			sprintf(temp1,"%s",
				"[ADD DEFAULT PRINTER]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][DEFAULT PRINTER]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(32+1);
			sprintf(temp1,"%s",
				"[ADD DEFAULT FILE OUTPUT DEVICE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][DEFAULT FILE OUTPUT DEVICE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"[ADD EMAIL]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][EMAIL]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+1);
			sprintf(temp1,"%s",
				"[ADD HOST]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][HOST]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD WEB LOGIN]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WEB LOGIN]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD WEB PASSWORD]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WEB PASSWORD]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD BROWSER TYPE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][BROWSER TYPE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+1);
			sprintf(temp1,"%s",
				"[ADD PUSHBUTTON STYLE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][PUSHBUTTON STYLE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD CERT CHECK]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][TLS CERT CHECK]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(21+1);
			sprintf(temp1,"%s",
				"[ADD CONNECT TIMEOUT]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][CONNECT TIMEOUT]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD NTLMDOMAIN]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][NTLMDOMAIN]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(45+1);
			sprintf(temp1,"%s",
				"[ADD AUTO COMPLETE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][AUTO COMPLETE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+1);
			sprintf(temp1,"%s",
				"[ADD PASSWORD]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][PASSWORD]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+1);
			sprintf(temp1,"%s",
				"[ADD PROTOCOL]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][PROTOCOL]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD SMTP AUTH]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][SMTP AUTH]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD SMTP PORT]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][SMTP PORT]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+1);
			sprintf(temp1,"%s",
				"[ADD SMTP SERVER]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][SMTP SERVER]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+1);
			sprintf(temp1,"%s",
				"[ADD TLS CERT FILE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][TLS CRT FILE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD TLS KEY FILE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][TLS KEY FILE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"[ADD TLS TRUST FILE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][TLS TRUST FILE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+1);
			sprintf(temp1,"%s",
				"[ADD TLS]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][TLS]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD TLS START]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][TLS START]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+1);
			sprintf(temp1,"%s",
				"[ADD USERNAME]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][USERNAME]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+1);
			sprintf(temp1,"%s",
				"[ADD CARRIER]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][CARRIER]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD CARRIER TYPE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][CARRIER TYPE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+1);
			sprintf(temp1,"%s",
				"[ADD COLOR PROFILE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][COLOR PROFILE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);

			temp1=Rmalloc(45+1);
			sprintf(temp1,"%s",
				"[ADD FONT]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][FONT]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);


			temp1=Rmalloc(45+1);
			sprintf(temp1,"%s",
				"[ADD FONT SIZE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][FONT SIZE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);


			temp1=Rmalloc(45+1);
			sprintf(temp1,"%s",
				"[ADD FONT WEIGHT]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][FONT WEIGHT]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD CELL PHONE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][CELL PHONE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+1);
			sprintf(temp1,"%s",
				"[ADD MODULE NAME]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][MODULE NAME]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD PREFER TEXT MESSAGE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][PREFER TEXT MESSAGE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+1);
			sprintf(temp1,"%s",
				"[ADD TEXT MESSAGE]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][TEXT MESSAGE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(5+1);
			sprintf(temp1,"%s",
				"FALSE");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][DELETEFLAG]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(45+1);
			sprintf(temp1,"%s",
				"[ADD SCAN DIRECTORY]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][SCAN DIRECTORY]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(81+1);
			sprintf(temp1,"%s",
				"[ADD WORKSTATION LOGIN]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WORKSTATION LOGIN]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(81+1);
			sprintf(temp1,"%s",
				"[ADD WORKSTATION PASSWORD]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WORKSTATION PASSWORD]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(81+1);
			sprintf(temp1,"%s",
				"[ADD WORKSTATION DOCUMENTS]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WORKSTATION DOCUMENTS]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}

		temp1=Rmalloc(203+1);
		sprintf(temp1,"%s",
			"([NEW PASSWORD]!=\"\")");
		x=addPowerAddWrite(add,"SECURITY","USERS",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(105+1);
			sprintf(temp1,"%s",
				"(WT_BCRYPT_VALUE([NEW PASSWORD]))");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WT PASSWORD]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(45+1);
			sprintf(temp1,"%s",
				"\"bcrypt\"");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WT PASSWORD METHOD]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(45+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WT FORCE PASSWORD CHANGE]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[SYSTEM DATE-YYYY]");
			x=addPowerAddWriteField(Write,"[SECURITY][USERS][WT PASSWORD DATE]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}

/* POWERADD EDIT WIDGETS  */


/* ADD USER IDENTIFICATION EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD USER IDENTIFICATION",1,15,3,temp1,0,"","","",0,NULL,temp2,"","ADD USER IDENTIFICATION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"NEW PASSWORD",1,25,0,temp1,0,"","","",0,NULL,temp2,"","NEW PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"CONFIRM PASSWORD",1,25,0,temp1,0,"","","",0,NULL,temp2,"","CONFIRM PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD PREFER TEXT MESSAGE EDIT WIDGET */
		temp1=Rmalloc(163+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][PREF",
			"ER TEXT MESSAGE] ELSE RETURN_VALUE=[ADD PREFER TEXT MESSAGE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD PREFER TEXT MESSAGE",10,1,2,temp1,0,"","","",0,NULL,temp2,"","ADD PREFER TEXT MESSAGE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD TEXT MESSAGE EDIT WIDGET */
		temp1=Rmalloc(149+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][TEXT",
			" MESSAGE] ELSE RETURN_VALUE=[ADD TEXT MESSAGE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TEXT MESSAGE",10,1,2,temp1,0,"","","",0,NULL,temp2,"","ADD TEXT MESSAGE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD CARRIER EDIT WIDGET */
		temp1=Rmalloc(139+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][CARR",
			"IER] ELSE RETURN_VALUE=[ADD CARRIER] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD CARRIER",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD CARRIER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD CARRIER TYPE EDIT WIDGET */
		temp1=Rmalloc(149+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][CARR",
			"IER TYPE] ELSE RETURN_VALUE=[ADD CARRIER TYPE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD CARRIER TYPE",7,1,2,temp1,0,"","","",0,NULL,temp2,"","ADD CARRIER TYPE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD COLOR PROFILE EDIT WIDGET */
		temp1=Rmalloc(151+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][COLO",
			"R PROFILE] ELSE RETURN_VALUE=[ADD COLOR PROFILE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD COLOR PROFILE",7,2,2,temp1,0,"","","",0,NULL,temp2,"","ADD COLOR PROFILE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD FONT EDIT WIDGET */
		temp1=Rmalloc(175+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][",
			"FONT] ELSE RETURN_VALUE=[ADD FONT] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD FONT",7,2,2,temp1,0,"","","",0,NULL,temp2,"","ADD FONT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD FONT SIZE EDIT WIDGET */
		temp1=Rmalloc(199+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][FO",
			"NT SIZE] ELSE \nIF([ADD FONT SIZE]=0) THEN RETURN_VALUE=8 ELSE \nRETURN_VALUE=[ADD FONT SIZE] FI FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD FONT SIZE",7,2,2,temp1,0,"","","",0,NULL,temp2,"","ADD FONT SIZE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD FONT WEIGHT EDIT WIDGET */
		temp1=Rmalloc(206+1);
		sprintf(temp1,"%s%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][FO",
			"NT WEIGHT] \nELSE IF([ADD FONT WEIGHT]=0) THEN RETURN_VALUE=60 \nELSE RETURN_VALUE=[ADD FONT WEIGHT]",
			" FI FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD FONT WEIGHT",7,2,2,temp1,0,"","","",0,NULL,temp2,"","ADD FONT WEIGHT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD CELL PHONE EDIT WIDGET */
		temp1=Rmalloc(145+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][CELL",
			" PHONE] ELSE RETURN_VALUE=[ADD CELL PHONE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD CELL PHONE",3,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD CELL PHONE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD SCAN DIRECTORY EDIT WIDGET */
		temp1=Rmalloc(155+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][",
			"SCAN DIRECTORY] ELSE RETURN_VALUE=[ADD SCAN DIRECTORY] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD SCAN DIRECTORY",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD SCAN DIRECTORY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);



/* ADD MODULE NAME EDIT WIDGET */
		temp1=Rmalloc(147+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][MODU",
			"LE NAME] ELSE RETURN_VALUE=[ADD MODULE NAME] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD MODULE NAME",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD MODULE NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD AUTO COMPLETE EDIT WIDGET */
		temp1=Rmalloc(185+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][",
			"AUTO COMPLETE] ELSE RETURN_VALUE=[ADD AUTO COMPLETE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD AUTO COMPLETE",10,1,2,temp1,0,"","","",0,NULL,temp2,"","ADD AUTO COMPLETE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);




/* ADD PHONE NUMBER EDIT WIDGET */
		temp1=Rmalloc(149+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][PHON",
			"E NUMBER] ELSE RETURN_VALUE=[ADD PHONE NUMBER] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD PHONE NUMBER",3,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD PHONE NUMBER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD PHONE EXTENSION EDIT WIDGET */
		temp1=Rmalloc(155+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][PHON",
			"E EXTENSION] ELSE RETURN_VALUE=[ADD PHONE EXTENSION] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD PHONE EXTENSION",1,10,2,temp1,0,"","","",0,NULL,temp2,"","ADD PHONE EXTENSION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD USER NAME EDIT WIDGET */
		temp1=Rmalloc(143+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][USER",
			" NAME] ELSE RETURN_VALUE=[ADD USER NAME] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD USER NAME",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD USER NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD DEFAULT PRINTER EDIT WIDGET */
		temp1=Rmalloc(155+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][DEFA",
			"ULT PRINTER] ELSE RETURN_VALUE=[ADD DEFAULT PRINTER] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD DEFAULT PRINTER",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD DEFAULT PRINTER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD DEFAULT FILE OUTPUT DEVICE EDIT WIDGET */
		temp1=Rmalloc(177+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][DEFA",
			"ULT FILE OUTPUT DEVICE] ELSE RETURN_VALUE=[ADD DEFAULT FILE OUTPUT DEVICE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD DEFAULT FILE OUTPUT DEVICE",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD DEFAULT FILE OUTPUT DEVICE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD HOST EDIT WIDGET */
		temp1=Rmalloc(133+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][HOST",
			"] ELSE RETURN_VALUE=[ADD HOST] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD HOST",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD HOST",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD WEB LOGIN EDIT WIDGET */
		temp1=Rmalloc(143+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][WEB ",
			"LOGIN] ELSE RETURN_VALUE=[ADD WEB LOGIN] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD WEB LOGIN",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD WEB LOGIN",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD WEB PASSWORD EDIT WIDGET */
		temp1=Rmalloc(149+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][WEB ",
			"PASSWORD] ELSE RETURN_VALUE=[ADD WEB PASSWORD] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD WEB PASSWORD",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD WEB PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD EMAIL EDIT WIDGET */
		temp1=Rmalloc(135+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN RETURN_VALUE=[SECURITY][USERS][EMAI",
			"L] ELSE RETURN_VALUE=[ADD EMAIL] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD EMAIL",1,60,2,temp1,0,"","","",0,NULL,temp2,"","ADD EMAIL",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD BROWSER TYPE EDIT WIDGET */
		temp1=Rmalloc(151+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][BR",
			"OWSER TYPE] ELSE RETURN_VALUE=[ADD BROWSER TYPE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD BROWSER TYPE",7,2,2,temp1,0,"","","",0,NULL,temp2,"","ADD BROWSER TYPE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD PUSHBUTTON STYLE EDIT WIDGET */
		temp1=Rmalloc(159+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][PU",
			"SHBUTTON STYLE] ELSE RETURN_VALUE=[ADD PUSHBUTTON STYLE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD PUSHBUTTON STYLE",7,2,2,temp1,0,"","","",0,NULL,temp2,"","ADD PUSHBUTTON STYLE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD SMTP SERVER EDIT WIDGET */
		temp1=Rmalloc(149+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][SM",
			"TP SERVER] ELSE RETURN_VALUE=[ADD SMTP SERVER] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD SMTP SERVER",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD SMTP SERVER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD SMTP PORT EDIT WIDGET */
		temp1=Rmalloc(145+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][SM",
			"TP PORT] ELSE RETURN_VALUE=[ADD SMTP PORT] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD SMTP PORT",8,5,2,temp1,0,"","","",0,NULL,temp2,"","ADD SMTP PORT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD SMTP AUTH EDIT WIDGET */
		temp1=Rmalloc(145+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][SM",
			"TP AUTH] ELSE RETURN_VALUE=[ADD SMTP AUTH] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD SMTP AUTH",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD SMTP AUTH",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD CONNECT TIMEOUT EDIT WIDGET */
		temp1=Rmalloc(157+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][CO",
			"NNECT TIMEOUT] ELSE RETURN_VALUE=[ADD CONNECT TIMEOUT] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD CONNECT TIMEOUT",7,3,2,temp1,0,"","","",0,NULL,temp2,"","ADD CONNECT TIMEOUT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD PROTOCOL EDIT WIDGET */
		temp1=Rmalloc(143+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][PR",
			"OTOCOL] ELSE RETURN_VALUE=[ADD PROTOCOL] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD PROTOCOL",1,60,2,temp1,0,"","","",0,NULL,temp2,"","ADD PROTOCOL",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD USERNAME EDIT WIDGET */
		temp1=Rmalloc(143+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][US",
			"ERNAME] ELSE RETURN_VALUE=[ADD USERNAME] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD USERNAME",1,50,2,temp1,0,"","","",0,NULL,temp2,"","ADD USERNAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD PASSWORD EDIT WIDGET */
		temp1=Rmalloc(143+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][PA",
			"SSWORD] ELSE RETURN_VALUE=[ADD PASSWORD] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD PASSWORD",1,20,2,temp1,0,"","","",0,NULL,temp2,"","ADD PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD NTLMDOMAIN EDIT WIDGET */
		temp1=Rmalloc(147+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][NT",
			"LMDOMAIN] ELSE RETURN_VALUE=[ADD NTLMDOMAIN] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD NTLMDOMAIN",1,60,2,temp1,0,"","","",0,NULL,temp2,"","ADD NTLMDOMAIN",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD TLS EDIT WIDGET */
		temp1=Rmalloc(133+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][TL",
			"S] ELSE RETURN_VALUE=[ADD TLS] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TLS",10,1,2,temp1,0,"","","",0,NULL,temp2,"","ADD TLS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD TLS START EDIT WIDGET */
		temp1=Rmalloc(145+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][TL",
			"S START] ELSE RETURN_VALUE=[ADD TLS START] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TLS START",10,1,2,temp1,0,"","","",0,NULL,temp2,"","ADD TLS START",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD CERT CHECK EDIT WIDGET */
		temp1=Rmalloc(151+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][TL",
			"S CERT CHECK] ELSE RETURN_VALUE=[ADD CERT CHECK] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD CERT CHECK",10,1,2,temp1,0,"","","",0,NULL,temp2,"","ADD CERT CHECK",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD TLS TRUST FILE EDIT WIDGET */
		temp1=Rmalloc(155+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][TL",
			"S TRUST FILE] ELSE RETURN_VALUE=[ADD TLS TRUST FILE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TLS TRUST FILE",1,60,2,temp1,0,"","","",0,NULL,temp2,"","ADD TLS TRUST FILE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD TLS KEY FILE EDIT WIDGET */
		temp1=Rmalloc(151+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][TL",
			"S KEY FILE] ELSE RETURN_VALUE=[ADD TLS KEY FILE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TLS KEY FILE",1,60,2,temp1,0,"","","",0,NULL,temp2,"","ADD TLS KEY FILE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD TLS CERT FILE EDIT WIDGET */
		temp1=Rmalloc(152+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][TL",
			"S CRT FILE] ELSE RETURN_VALUE=[ADD TLS CERT FILE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD TLS CERT FILE",1,60,2,temp1,0,"","","",0,NULL,temp2,"","ADD TLS CERT FILE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD WORKSTATION LOGIN EDIT WIDGET */
		temp1=Rmalloc(352+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][",
			"WORKSTATION LOGIN] ELSE RETURN_VALUE=[ADD WORKSTATION LOGIN] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD WORKSTATION LOGIN",1,40,2,temp1,0,"","","",0,NULL,temp2,"","ADD WORKSTATION LOGIN",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD WORKSTATION PASSWORD EDIT WIDGET */
		temp1=Rmalloc(352+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][",
			"WORKSTATION PASSWORD] ELSE RETURN_VALUE=[ADD WORKSTATION PASSWORD] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD WORKSTATION PASSWORD",1,25,2,temp1,0,"","","",0,NULL,temp2,"","ADD WORKSTATION PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD WORKSTATION DOCUMENTS EDIT WIDGET */
		temp1=Rmalloc(352+1);
		sprintf(temp1,"%s%s",
			"IF(([ADD USER IDENTIFICATION]!=[LAST USER IDENTIFICATION])) THEN \nRETURN_VALUE=[SECURITY][USERS][",
			"WORKSTATION DOCUMENTS] ELSE RETURN_VALUE=[ADD WORKSTATION DOCUMENTS] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD WORKSTATION DOCUMENTS",1,120,2,temp1,0,"","","",0,NULL,temp2,"","ADD WORKSTATION DOCUMENTS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* LAST USER IDENTIFICATION EDIT WIDGET */
		temp1=Rmalloc(25+1);
		sprintf(temp1,"%s",
			"[ADD USER IDENTIFICATION]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LAST USER IDENTIFICATION",1,15,2,temp1,0,"","","",0,NULL,temp2,"","LAST USER IDENTIFICATION",temp3,0,NULL,0,NULL);
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
			temp1=Rmalloc(12+6+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [OLHELP] Power Add [USER PROFILE], Can Not Save Power Add!");
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
static void SCN_USER_PROFILE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","USER PROFILE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"ADD USER IDENTIFICATION","User Identification:","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","Name:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"[USERS][USER NAME]","User Name","","",0,40,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Password","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     New Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NEW PASSWORD","New Password:","","",0,25,3,NULL,NULL,NULL,"\"CONFIRM PASSWORD\"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Confirm Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CONFIRM PASSWORD","Confirm Password:","","",0,25,3,NULL,"([NEW PASSWORD]!=\"\")","([NEW PASSWORD]!=\"\")",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Contact","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          Phone Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PHONE NUMBER","Phone Number:","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Phone Extension:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PHONE EXTENSION","","","",0,7,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","            Cell Phone:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CELL PHONE","Cell Phone:","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD CARRIER","Load Carrier","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Carrier:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CARRIER","Carrier:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","      TEXT PREFERENCES:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD TEXT MESSAGE","Text Message","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD PREFER TEXT MESSAGE","Prefer Text Message","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","         Email Address:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD EMAIL","Email Address","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Preferences","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","    Default Printer:","","",0,0,0,"FALSE",NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD DEFAULT PRINTERS","Default Printer:","","",1,0,0,"FALSE",NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","            Browser:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD USERS BROWSER TYPES","Browser Type:","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Push Button Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD USERS PUSHBUTTON STYLES","PushButton Style:","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   Color Profile:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"USERS COLOR PROFILES","Color Profile","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#ifndef WIN32
		ADVaddwdgt(scrn,5,"","               Font:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD USERS FONTS","Font:","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Font Size:","","",0,0,0,NULL,NULL,NULL,NULL);
#else
		ADVaddwdgt(scrn,5,"","         Font Size:","","",0,0,0,NULL,NULL,NULL,NULL);
#endif
		ADVaddwdgt(scrn,7,"ADD USERS FONTSIZES","Font Size:","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Font Weight:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD FONT WEIGHT","Font Weight:","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","        Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD MODULE NAME","Module Name:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD AUTO COMPLETE","Auto Complete","","",0,0,0,"([DEV_LICENSE]=TRUE) AND ([SOFTWARE TYPE]<2)",NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," File Output Device:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DEFAULT FILE OUTPUT DEVICE","Default File Output Device:","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     Scan Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SCAN DIRECTORY","Scan Directory:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","SMTP Setup (Email)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     SMTP Server:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP SERVER","SMTP Server","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SMTP Port:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP PORT","SMTP Port","","",0,5,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","   Authorization:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD USERS SMTP AUTHORITY","SMTP Authority","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Protocol:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD USERS PROTOCOLS","Protocols","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","        Username:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(30+1);
		sprintf(temp3,"%s",
			"([ADD SMTP AUTH]!=\"default\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"ADD USERNAME","SMTP User Name","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(30+1);
		sprintf(temp3,"%s",
			"([ADD SMTP AUTH]!=\"default\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"ADD PASSWORD","SMTP Password","","",0,20,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","Connection Timeout:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CONNECT TIMEOUT","Connection Timeout","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     NTLM Domain:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(26+1);
		sprintf(temp3,"%s",
			"([ADD SMTP AUTH]=\"ntlm\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"ADD NTLMDOMAIN","NTLM Domain","","",0,60,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD TLS","TLS/SSL Connection to Mail Server","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(16+1);
		sprintf(temp3,"%s",
			"([ADD TLS]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"ADD TLS START","Start TLS","","",0,0,0,temp1,temp2,temp3,temp4);
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
		temp3=Rmalloc(16+1);
		sprintf(temp3,"%s",
			"([ADD TLS]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,9,"ADD CERT CHECK","TLS Certificate Verification","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  TLS Trust File:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(16+1);
		sprintf(temp3,"%s",
			"([ADD TLS]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"ADD TLS TRUST FILE","TLS Trust Filename","","",0,60,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","    TLS Key File:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(16+1);
		sprintf(temp3,"%s",
			"([ADD TLS]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"ADD TLS KEY FILE","TLS Key Filename","","",0,60,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TLS CRT Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(44+1);
		sprintf(temp3,"%s",
			"([ADD TLS]=TRUE) AND ([ADD CERT CHECK]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"ADD TLS CERT FILE","TLS CRT Filename","","",0,60,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#ifndef WIN32
		ADVaddwdgt(scrn,20,"","Workstation","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","    Workstation Login:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WORKSTATION LOGIN","Workstation Login","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Workstation Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WORKSTATION PASSWORD","Workstation Password","","",0,25,3,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Workstation Documents:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WORKSTATION DOCUMENTS","Workstation Documents","","",0,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#endif /* WIN32 */
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CHECK EMAIL","Check Email","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"VALIDATE WORKSTATION","Validate Workstation","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(12+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [USER PROFILE], Can Not Save Maintain Master!");
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
static void SCN_USER_PROFILE_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","USER PROFILE DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,500,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Browser Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Carrier:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Carrier Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Cell Phone:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Cert Check:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Color Profile:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Connect Timeout:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Default File Output Device:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"FALSE",NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Default Printer:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Email:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Host:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Ntlmdomain:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Phone Extension:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Phone Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Prefer Text Message:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Protocol:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Pushbutton Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Smtp Auth:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Smtp Port:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Smtp Server:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Text Message:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Tls:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Tls Cert File:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Tls Key File:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Tls Start:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Tls Trust File:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add User Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Username:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Web Login:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add Web Password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD BROWSER TYPE POSITION","Add Browser Type Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD BROWSER TYPE LENGTH","Add Browser Type Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CARRIER POSITION","Add Carrier Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CARRIER LENGTH","Add Carrier Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CARRIER TYPE POSITION","Add Carrier Type Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CARRIER TYPE LENGTH","Add Carrier Type Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CELL PHONE POSITION","Add Cell Phone Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CELL PHONE LENGTH","Add Cell Phone Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CERT CHECK POSITION","Add Cert Check Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CERT CHECK LENGTH","Add Cert Check Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD COLOR PROFILE POSITION","Add Color Profile Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD COLOR PROFILE LENGTH","Add Color Profile Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CONNECT TIMEOUT POSITION","Add Connect Timeout Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD CONNECT TIMEOUT LENGTH","Add Connect Timeout Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DEFAULT FILE OUTPUT DEVICE POSITION","Add Default File Output Device Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DEFAULT FILE OUTPUT DEVICE LENGTH","Add Default File Output Device Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"FALSE",NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DEFAULT PRINTER POSITION","Add Default Printer Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DEFAULT PRINTER LENGTH","Add Default Printer Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD EMAIL POSITION","Add Email Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD EMAIL LENGTH","Add Email Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD HOST POSITION","Add Host Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD HOST LENGTH","Add Host Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD MODULE NAME POSITION","Add Module Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD MODULE NAME LENGTH","Add Module Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD NTLMDOMAIN POSITION","Add Ntlmdomain Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD NTLMDOMAIN LENGTH","Add Ntlmdomain Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PASSWORD POSITION","Add Password Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PASSWORD LENGTH","Add Password Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PHONE EXTENSION POSITION","Add Phone Extension Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PHONE EXTENSION LENGTH","Add Phone Extension Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PHONE NUMBER POSITION","Add Phone Number Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PHONE NUMBER LENGTH","Add Phone Number Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PREFER TEXT MESSAGE POSITION","Add Prefer Text Message Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PREFER TEXT MESSAGE LENGTH","Add Prefer Text Message Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PROTOCOL POSITION","Add Protocol Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PROTOCOL LENGTH","Add Protocol Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PUSHBUTTON STYLE POSITION","Add Pushbutton Style Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PUSHBUTTON STYLE LENGTH","Add Pushbutton Style Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP AUTH POSITION","Add Smtp Auth Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP AUTH LENGTH","Add Smtp Auth Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP PORT POSITION","Add Smtp Port Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP PORT LENGTH","Add Smtp Port Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP SERVER POSITION","Add Smtp Server Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD SMTP SERVER LENGTH","Add Smtp Server Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TEXT MESSAGE POSITION","Add Text Message Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TEXT MESSAGE LENGTH","Add Text Message Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS POSITION","Add Tls Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS LENGTH","Add Tls Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS CERT FILE POSITION","Add Tls Cert File Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS CERT FILE LENGTH","Add Tls Cert File Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS KEY FILE POSITION","Add Tls Key File Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS KEY FILE LENGTH","Add Tls Key File Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS START POSITION","Add Tls Start Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS START LENGTH","Add Tls Start Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS TRUST FILE POSITION","Add Tls Trust File Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD TLS TRUST FILE LENGTH","Add Tls Trust File Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USER IDENTIFICATION POSITION","Add User Identification Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USER IDENTIFICATION LENGTH","Add User Identification Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USER NAME POSITION","Add User Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USER NAME LENGTH","Add User Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USERNAME POSITION","Add Username Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USERNAME LENGTH","Add Username Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WEB LOGIN POSITION","Add Web Login Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WEB LOGIN LENGTH","Add Web Login Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WEB PASSWORD POSITION","Add Web Password Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WEB PASSWORD LENGTH","Add Web Password Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(24+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [USER PROFILE DEFINE LIST], Can Not Save Maintain Master!");
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

static void RPT_CREATEMSMTPRC()
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
	sprintf(rptname,"CREATEMSMTPRC");
	t=Rmalloc(46+1);
	sprintf(t,"Create MSMTPRC File From User Security Records");
	desc=Rmalloc(46+1);
	sprintf(desc,"Create MSMTPRC File From User Security Records");
#if defined(USE_MYISAM) 
	engine=NRDMyIsamEngine;
#else /* USE_MYISAM */
#if !defined(WIN32) && !defined(LINUX) 
	engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) 
	engine=NRDLocalCtreeEngine;
#endif
#endif /* USE_MYISAM */
	tmp=RDAreportNEWADV3(rptname,t,desc,engine,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",FALSE,0,TRUE,"",FALSE,FALSE,FALSE,FALSE);
	tmp->report_completion=0;
	tmp->remove_file=0;
	tmp->remove_name=stralloc("");
	tmp->manual_form=0;
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);



/* REPORT DISPLAY DEFINITION */

	pvars=NULL;
	adddisplayADV5(tmp,(double)7.500000,(double)10.000000,1,FALSE,0,NULL,TRUE,0,1,0,pvars,0,0,0,FALSE,"");
	temp1=Rmalloc(9+1);
	sprintf(temp1,"%s",
		"[MSMTPRC]");
	tmp->display->set_lpp=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);

/*----------------------------------------------------------------------*/

	tmp->select=NULL;




/* SEARCH FILES DEFINITIONS */

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
	x=addsearchfileADV2(tmp,"SECURITY","USERS",0,NULL,NULL,"USERS KEY",FALSE,temp1,temp2,1,0,temp3);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"USER IDENTIFICATION","[USER IDENTIFICATION]");
	}






/* VIRTUAL FIELD DEFINITIONS */

/* APPDATA VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"APPDATA",1,200,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* USER IDENTIFICATION VIRTUAL FIELD */
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	addvirtuallineADV(tmp,"USER IDENTIFICATION",1,40,temp1,0,3);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* MSMTPRC VIRTUAL FIELD */
	temp1=Rmalloc(189+1);
	sprintf(temp1,"%s%s",
		"IF ((STRING_LENGTH([APPDATA]))=0) THEN RETURN_VALUE=([CURRENT DIRECTORY]+\"/\"+[SECURITY][USERS][USER IDENTIFICATION]+",
		"\"/msmtprc\") ELSE RETURN_VALUE=([APPDATA]+\"\\msmtprc.txt\") FI");
	addvirtuallineADV(tmp,"MSMTPRC",1,400,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* LOGFILE VIRTUAL FIELD */
	temp1=Rmalloc(223+1);
	sprintf(temp1,"%s%s",
		"IF ((STRING_LENGTH([APPDATA]))=0) THEN RETURN_VALUE=([CURRENT DIRECTORY]+\"/\"+[SECURITY][USERS][USER IDENTIFICATION]+",
		"\"/msmtp.log\") ELSE RETURN_VALUE=(\"\"+[SECURITY][USERS][USER IDENTIFICATION]+\"\\msmtp.log\") FI");
	addvirtuallineADV(tmp,"LOGFILE",1,400,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);


/* START TLS VIRTUAL FIELD */
	temp1=Rmalloc(91+1);
	sprintf(temp1,"%s",
		"IF [SECURITY][USERS][TLS START]=TRUE  THEN RETURN_VALUE=\"on\" ELSE RETURN_VALUE=\"off\" FI");
	addvirtuallineADV(tmp,"START TLS",1,3,temp1,0,0);
	if(temp1!=NULL) Rfree(temp1);
	if(t!=NULL) Rfree(t);







	SetReportDefaultEngine(tmp);


/* ACCUMULATORS */

	addaccumline(tmp,"",0,"[SECURITY][USERS][USER IDENTIFICATION]",0);
	addaccumline(tmp,"LAST",3,"[SECURITY][USERS][TLS]",0);
	addaccumline(tmp,"LAST",3,"[SECURITY][USERS][TLS CERT CHECK]",0);




/* IMAGE LINE DEFINITIONS */

	imageoff=addimageline(tmp,"DETAIL LINES","# Use the SMTP server of your provider@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","account default@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","host @V@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][SMTP SERVER]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","from @V@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][EMAIL]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][TLS]=TRUE)");
	imageoff=addimageline(tmp,"DETAIL LINES","tls on @@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(29+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][TLS]=TRUE)");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_starttls @LLL@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[START TLS]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(111+1);
	sprintf(temp1,"%s%s",
		"([SECURITY][USERS][TLS]=TRUE)AND\n((STRING_TO_LOWER([SECURITY][USERS][SMTP SERVER]))=\"smtp.sendit.n",
		"odak.edu\")");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_min_dh_prime_bits 512@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(122+1);
	sprintf(temp1,"%s%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS TRUST FILE]=\"\") AND ([SECURITY][USERS][T",
		"LS CERT CHECK]=FALSE))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_certcheck off @@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(159+1);
	sprintf(temp1,"%s%s",
		"((([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS TRUST FILE]!=\"\")) OR (([SECURITY][USERS",
		"][TLS]=TRUE) AND ([SECURITY][USERS][TLS CERT CHECK]=TRUE)))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_certcheck on @@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(77+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS TRUST FILE]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_trust_file @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][TLS TRUST FILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(75+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS KEY FILE]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_key_file @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][TLS KEY FILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(75+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][TLS]=TRUE) AND ([SECURITY][USERS][TLS CRT FILE]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","tls_cert_file @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][TLS CRT FILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(33+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][SMTP PORT]!=0)");
	imageoff=addimageline(tmp,"DETAIL LINES","port @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][SMTP PORT]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(86+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][SMTP AUTH]!=\"default\") AND ([SECURITY][USERS][SMTP AUTH]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","auth @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][SMTP AUTH]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(78+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][SMTP AUTH]!=\"\") AND ([SECURITY][USERS][USERNAME]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","user @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][USERNAME]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(78+1);
	sprintf(temp1,"%s",
		"(([SECURITY][USERS][SMTP AUTH]!=\"\") AND ([SECURITY][USERS][PASSWORD]!=\"\"))");
	imageoff=addimageline(tmp,"DETAIL LINES","password @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][PASSWORD]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(37+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][NTLMDOMAIN]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","domain @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][NTLMDOMAIN]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(77+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][PROTOCOL]!=\"smtp\")AND([SECURITY][USERS][PROTOCOL]!=\"\")");
	imageoff=addimageline(tmp,"DETAIL LINES","protocol @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][PROTOCOL]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][SMTP AUTH]=\"ntlm\")");
	imageoff=addimageline(tmp,"DETAIL LINES","ntlmdomain @V@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][NTLMDOMAIN]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(39+1);
	sprintf(temp1,"%s",
		"([SECURITY][USERS][CONNECT TIMEOUT]!=0)");
	imageoff=addimageline(tmp,"DETAIL LINES","timeout @LLL@@",TRUE,temp1,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[SECURITY][USERS][CONNECT TIMEOUT]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	imageoff=addimageline(tmp,"DETAIL LINES","logfile @V@@",FALSE,NULL,0,NULL);
	if(imageoff>0)
	{
		image=tmp->image+(imageoff-1);
		addimagepval(image,0,"[LOGFILE]",0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
	}


	tmp->MTNMSTR=NULL;

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+6+100+1);
			sprintf(temp1,"REPORT WRITE ERROR: Module [OLHELP] Report [CREATEMSMTPRC], Can Not Save Report!");
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
static void LST_ADD_DEFAULT_PRINTERS()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","ADD DEFAULT PRINTERS");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=2;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("OUTPUT DEVICES");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=1;
		list->conmod=stralloc("ADD DEFAULT PRINTER");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(21+1);
	sprintf(temp1,"%s",
		"[ADD DEFAULT PRINTER]");
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
			temp1=Rmalloc(20+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [ADD DEFAULT PRINTERS], Can Not Save Scrolled List!");
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
static void LST_ADD_USERS_BROWSER_TYPES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","ADD USERS BROWSER TYPES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
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
		list->contype=1;
		list->conmod=stralloc("ADD BROWSER TYPE");
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
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Opera");
		addAPPlib(list->list,"Mozilla");
		addAPPlib(list->list,"Firefox");

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
			temp1=Rmalloc(23+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [ADD USERS BROWSER TYPES], Can Not Save Scrolled List!");
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
static void LST_ADD_USERS_PROTOCOLS()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","ADD USERS PROTOCOLS");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
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
		list->contype=1;
		list->conmod=stralloc("ADD PROTOCOL");
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
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"smtp");
		addAPPlib(list->list,"lmtp");

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
			temp1=Rmalloc(19+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [ADD USERS PROTOCOLS], Can Not Save Scrolled List!");
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
static void LST_ADD_USERS_PUSHBUTTON_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","ADD USERS PUSHBUTTON STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
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
		list->contype=1;
		list->conmod=stralloc("ADD PUSHBUTTON STYLE");
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
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Graphics");
		addAPPlib(list->list,"Labels Only");
/*
		addAPPlib(list->list,"Mixed Graphics");
*/

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
			temp1=Rmalloc(27+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [ADD USERS PUSHBUTTON STYLES], Can Not Save Scrolled List!");
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
static void LST_ADD_USERS_SMTP_AUTHORITY()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","ADD USERS SMTP AUTHORITY");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
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
		list->contype=1;
		list->conmod=stralloc("ADD SMTP AUTH");
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
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"default");
		addAPPlib(list->list,"plain");
		addAPPlib(list->list,"cram-md5");
		addAPPlib(list->list,"digest-md5");
		addAPPlib(list->list,"gssapi");
		addAPPlib(list->list,"external");
		addAPPlib(list->list,"login");
		addAPPlib(list->list,"ntlm");

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
			temp1=Rmalloc(24+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [ADD USERS SMTP AUTHORITY], Can Not Save Scrolled List!");
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
static void LST_USERS_COLOR_PROFILES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","USERS COLOR PROFILES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
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
		list->contype=1;
		list->conmod=stralloc("ADD COLOR PROFILE");
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
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Purple");
		addAPPlib(list->list,"Blue (Windows)");
		addAPPlib(list->list,"Black");
		addAPPlib(list->list,"Orange");
		addAPPlib(list->list,"Teal");
		addAPPlib(list->list,"Blue");
		addAPPlib(list->list,"Gray");

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
			temp1=Rmalloc(20+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [USERS COLOR PROFILES], Can Not Save Scrolled List!");
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
static void LST_ADD_USERS_FONTS()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","ADD USERS FONTS");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
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
		list->contype=1;
		list->conmod=stralloc("ADD FONT");
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
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"DejaVu Sans Mono");
		addAPPlib(list->list,"Decorative");
		addAPPlib(list->list,"Courier");
		addAPPlib(list->list,"Liberation Mono");

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
			temp1=Rmalloc(15+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [ADD USERS FONTS], Can Not Save Scrolled List!");
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
static void LST_ADD_USERS_FONTSIZES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","ADD USERS FONTSIZES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
		ScrolledListDFincvir(list,"SECURITY","USERS","",0);
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
		list->contype=1;
		list->conmod=stralloc("ADD FONT SIZE");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(36+1);
	sprintf(temp1,"%s",
		"(INTEGER_TO_STRING([ADD FONT SIZE]))");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(40+1);
	sprintf(temp1,"%s",
		"(STRING_TO_VALUE([ADD USERS FONTSIZES]))");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"8");
		addAPPlib(list->list,"10");
		addAPPlib(list->list,"12");

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
			temp1=Rmalloc(19+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [ADD USERS FONTSIZES], Can Not Save Scrolled List!");
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
void USERPROFILE()
{
	ADD_USER_PROFILE();
	SCN_USER_PROFILE();
	SCN_USER_PROFILE_DEFINE_LIST();
	RPT_CREATEMSMTPRC();
	LST_ADD_DEFAULT_PRINTERS();
	LST_ADD_USERS_BROWSER_TYPES();
	LST_ADD_USERS_PROTOCOLS();
	LST_ADD_USERS_PUSHBUTTON_STYLES();
	LST_ADD_USERS_SMTP_AUTHORITY();
	LST_USERS_COLOR_PROFILES();
	LST_ADD_USERS_FONTS();
	LST_ADD_USERS_FONTSIZES();
}
