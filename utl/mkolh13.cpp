/* OLHELP's - Make (ADD FEEDBACK) Power Add */
#include <cstdio>
#include <mkadd.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void ADD_ADD_FEEDBACK()
{
	int x=0;
	PowerAdd *add=NULL;
	char *defdir=NULL;
	APPlib *args=NULL,*args2=NULL;
	RDAaddsub *subord=NULL;
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
		"([ADD_SUBJECT]=\"\")");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(136+1);
	sprintf(temp3,"%s%s",
		"IF([ADD_SUBJECT]=\"\") THEN RETURN_VALUE=\"Error:  A suggestion / comment must at minimum include a ",
		"subject.\" ELSE RETURN_VALUE=\"\" FI");


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
	add=RDAPowerAddNEW(module,"ADD FEEDBACK",temp1,"ADD FEEDBACK","ADD FEEDBACK DEFINE LIST","",3,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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

/* FEEDBACKC wwwdoadd BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"OLHELP");
		addAPPlib(args,"FEEDBACKC");
		x=addPowerAddButton(add,"wwwdoadd","","FEEDBACKC","OLHELP","RSCNOT","RSCNOT KEY",temp1,1,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_COMMENTS]","ADD_COMMENTS");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[HTTP PROXY PORT]","HTTP_PROXY_PORT");
			x=addPowerAddButtonField(button,"[HTTP PROXY TO USE]","HTTP_PROXY_TO_USE");
			x=addPowerAddButtonField(button,"[HTTP PROXY]","HTTP_PROXY");
			x=addPowerAddButtonField(button,"[HTTP USE PROXY]","HTTP_USE_PROXY");
			x=addPowerAddButtonField(button,"[ADD EMAIL]","ADD_EMAIL");
			x=addPowerAddButtonField(button,"[ADD PHONE NUMBER]","ADD_PHONE_NUMBER");
		}


/* POWERADD SUPPORTING FILES  */


/* SECURITY USERS SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,0,"","","","",0);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(21+1);
			sprintf(temp1,"%s",
				"[USER IDENTIFICATION]");
			x=addPowerAddSupportingConfld(subord,"USER IDENTIFICATION",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}


/* POWERADD EDIT WIDGETS  */


/* HTTP USE PROXY EDIT WIDGET */
		temp1=Rmalloc(57+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][HTTP USE PROXY]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP USE PROXY",10,1,3,temp1,0,"","","",0,NULL,temp2,"","HTTP USE PROXY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HTTP PROXY EDIT WIDGET */
		temp1=Rmalloc(53+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][HTTP PROXY]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP PROXY",1,60,3,temp1,0,"","","",0,NULL,temp2,"","HTTP PROXY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HTTP PROXY PORT EDIT WIDGET */
		temp1=Rmalloc(58+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][HTTP PROXY PORT]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP PROXY PORT",8,4,3,temp1,0,"","","",0,NULL,temp2,"","HTTP PROXY PORT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HTTP PROXY TO USE EDIT WIDGET */
		temp1=Rmalloc(129+1);
		sprintf(temp1,"%s%s",
			"IF([HTTP USE PROXY]=TRUE) THEN RETURN_VALUE=[HTTP PROXY]+\":\"+(INTEGER_TO_STRING([HTTP PROXY PORT])",
			") \nELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP PROXY TO USE",0,0,2,temp1,0,"","","",0,NULL,temp2,"","HTTP PROXY TO USE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SOFTWARE NAME EDIT WIDGET */
		temp1=Rmalloc(380+1);
		sprintf(temp1,"%s",
			"IF([OPENRDA SETUP]=TRUE) THEN RETURN_VALUE=\"OpenRDA\" \nELSE IF([SOFTWARE TYPE]<2) THEN RETURN_VALUE=\"xpert\" \nELSE RETURN_VALUE=\"mbgui\" FI FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SOFTWARE NAME",0,0,2,temp1,0,"","","",0,NULL,temp2,"","SOFTWARE NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* USER IDENTIFICATION EDIT WIDGET */
		temp1=Rmalloc(22+1);
		sprintf(temp1,"%s",
			"[LOGIN IDENTIFICATION]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"USER IDENTIFICATION",1,15,2,temp1,0,"","","",0,NULL,temp2,"","USER IDENTIFICATION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_WEB_ID EDIT WIDGET */
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][WEB ID]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_WEB_ID",1,15,3,temp1,0,"","","",0,NULL,temp2,"","ADD_WEB_ID",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_WEB_LOGIN EDIT WIDGET */
		temp1=Rmalloc(17+1);
		sprintf(temp1,"%s",
			"[USERS WEB LOGIN]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_WEB_LOGIN",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD_WEB_LOGIN",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_WEB_PASSWORD EDIT WIDGET */
		temp1=Rmalloc(20+1);
		sprintf(temp1,"%s",
			"[USERS WEB PASSWORD]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_WEB_PASSWORD",0,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD_WEB_PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_SUBJECT EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_SUBJECT",1,120,0,temp1,0,"","","",0,NULL,temp2,"","ADD_SUBJECT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_COMMENTS EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_COMMENTS",14,0,0,temp1,0,"","","",0,NULL,temp2,"","ADD_COMMENTS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD EMAIL EDIT WIDGET */
		temp1=Rmalloc(89+1);
		sprintf(temp1,"%s",
			"IF [ADD EMAIL]!=\"\" THEN RETURN_VALUE=[ADD EMAIL]\nELSE RETURN_VALUE=[USERS EMAIL]\nFI\n");
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

/* ADD PHONE NUMBER EDIT WIDGET */
		temp1=Rmalloc(103+1);
		sprintf(temp1,"%s%s",
			"IF [ADD PHONE NUMBER]!=\"\" THEN RETURN_VALUE=[ADD PHONE NUMBER]\nELSE RETURN_VALUE=[USERS PHONE]\nF",
			"I\n");
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
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [OLHELP] Power Add [ADD FEEDBACK], Can Not Save Power Add!");
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
static void ADD_FEEDBACKC()
{
	int x=0;
	PowerAdd *add=NULL;
	char *defdir=NULL;
	APPlib *args=NULL,*args2=NULL;
	RDAaddsub *subord=NULL;
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
		"([ADD_SUBJECT]=\"\")");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(174+1);
	sprintf(temp3,"%s%s",
		"IF([ADD_SUBJECT]=\"\") THEN RETURN_VALUE=\"Error:  We cannot process a request without detailed info",
		"rmation describing the subject of the request.\" ELSE RETURN_VALUE=\"\" FI");


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
	add=RDAPowerAddNEW(module,"FEEDBACKC",temp1,"ADD FEEDBACK REQUEST","ADD FEEDBACK REQUEST DEFINE LIST","",1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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

/* POST FEEDBACK HTTP POST BUTTON  */
		temp1=Rmalloc(54+1);
		sprintf(temp1,"%s",
			"([HTTP USE PROXY]=FALSE) OR ([HTTP PROXY TO USE]=\"\")");
		
		args=APPlibNEW();
		temp2=Rmalloc(106+1);
		sprintf(temp2,"%s%s",
			"\"http://\"+(GENERIC_SETUP_VARIABLE(\"[UTILITIES][RDA SUPPORT IP ADDRESS]\"))+\":80/cgi-bin/FEEDBACK",
			".cgi\"");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"HTTP POST","","POST FEEDBACK","OLHELP","URSCNOT","URSCNOT KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
			x=addPowerAddButtonField(button,"[ADD_COMMENTS]","ADD_COMMENTS");
			x=addPowerAddButtonField(button,"[ADD EMAIL]","ADD_EMAIL");
			x=addPowerAddButtonField(button,"[ADD PHONE NUMBER]","ADD_PHONE_NUMBER");
		}
/* POST FEEDBACK HTTP POST BUTTON  */
		temp1=Rmalloc(55+1);
		sprintf(temp1,"%s",
			"([HTTP USE PROXY]=TRUE) AND ([HTTP PROXY TO USE]!=\"\")");
		
		args=APPlibNEW();
		temp2=Rmalloc(106+1);
		sprintf(temp2,"%s%s",
			"\"http://\"+(GENERIC_SETUP_VARIABLE(\"[UTILITIES][RDA SUPPORT IP ADDRESS]\"))+\":80/cgi-bin/FEEDBACK",
			".cgi\"");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(19+1);
		sprintf(temp2,"%s",
			"[HTTP PROXY TO USE]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"HTTP POST","","POST FEEDBACK","OLHELP","URSCNOT","URSCNOT KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
			x=addPowerAddButtonField(button,"[ADD_COMMENTS]","ADD_COMMENTS");
			x=addPowerAddButtonField(button,"[ADD EMAIL]","ADD_EMAIL");
			x=addPowerAddButtonField(button,"[ADD PHONE NUMBER]","ADD_PHONE_NUMBER");
		}


/* POWERADD SUPPORTING FILES  */


/* SECURITY USERS SUPPORTING FILE */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupportingADV(add,"SECURITY","USERS",0,0,NULL,"USERS KEY",temp1,0,"","","","",0);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			subord=add->subord+(x-1);
			temp1=Rmalloc(21+1);
			sprintf(temp1,"%s",
				"[USER IDENTIFICATION]");
			x=addPowerAddSupportingConfld(subord,"USER IDENTIFICATION",temp1,0);
			if(temp1!=NULL) Rfree(temp1);
		}


/* POWERADD EDIT WIDGETS  */


/* HTTP USE PROXY EDIT WIDGET */
		temp1=Rmalloc(57+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][HTTP USE PROXY]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP USE PROXY",10,1,3,temp1,0,"","","",0,NULL,temp2,"","HTTP USE PROXY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HTTP PROXY EDIT WIDGET */
		temp1=Rmalloc(53+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][HTTP PROXY]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP PROXY",1,60,3,temp1,0,"","","",0,NULL,temp2,"","HTTP PROXY",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HTTP PROXY PORT EDIT WIDGET */
		temp1=Rmalloc(58+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][HTTP PROXY PORT]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP PROXY PORT",8,4,3,temp1,0,"","","",0,NULL,temp2,"","HTTP PROXY PORT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HTTP PROXY TO USE EDIT WIDGET */
		temp1=Rmalloc(129+1);
		sprintf(temp1,"%s%s",
			"IF([HTTP USE PROXY]=TRUE) THEN RETURN_VALUE=[HTTP PROXY]+\":\"+(INTEGER_TO_STRING([HTTP PROXY PORT])",
			") \nELSE RETURN_VALUE=\"\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HTTP PROXY TO USE",0,0,2,temp1,0,"","","",0,NULL,temp2,"","HTTP PROXY TO USE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* SOFTWARE NAME EDIT WIDGET */
		temp1=Rmalloc(380+1);
		sprintf(temp1,"%s",
			"IF([OPENRDA SETUP]=TRUE) THEN RETURN_VALUE=\"OpenRDA\" \nELSE IF([SOFTWARE TYPE]<2) THEN RETURN_VALUE=\"xpert\" \nELSE RETURN_VALUE=\"mbgui\" FI FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"SOFTWARE NAME",0,0,2,temp1,0,"","","",0,NULL,temp2,"","SOFTWARE NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* USER IDENTIFICATION EDIT WIDGET */
		temp1=Rmalloc(22+1);
		sprintf(temp1,"%s",
			"[LOGIN IDENTIFICATION]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"USER IDENTIFICATION",1,15,3,temp1,0,"","","",0,NULL,temp2,"","USER IDENTIFICATION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_WEB_ID EDIT WIDGET */
		temp1=Rmalloc(49+1);
		sprintf(temp1,"%s",
			"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][WEB ID]\"))");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_WEB_ID",1,15,3,temp1,0,"","","",0,NULL,temp2,"","ADD_WEB_ID",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_WEB_LOGIN EDIT WIDGET */
		temp1=Rmalloc(17+1);
		sprintf(temp1,"%s",
			"[USERS WEB LOGIN]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_WEB_LOGIN",1,15,0,temp1,0,"","","",0,NULL,temp2,"","ADD_WEB_LOGIN",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_WEB_PASSWORD EDIT WIDGET */
		temp1=Rmalloc(20+1);
		sprintf(temp1,"%s",
			"[USERS WEB PASSWORD]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_WEB_PASSWORD",0,15,0,temp1,0,"","","",0,NULL,temp2,"","ADD_WEB_PASSWORD",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_SUBJECT EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_SUBJECT",1,120,0,temp1,0,"","","",0,NULL,temp2,"","ADD_SUBJECT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_COMMENTS EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_COMMENTS",14,9999,0,temp1,0,"","","",0,NULL,temp2,"","ADD_COMMENTS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD EMAIL EDIT WIDGET */
		temp1=Rmalloc(89+1);
		sprintf(temp1,"%s",
			"IF [ADD EMAIL]!=\"\" THEN RETURN_VALUE=[ADD EMAIL]\nELSE RETURN_VALUE=[USERS EMAIL]\nFI\n");
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

/* ADD PHONE NUMBER EDIT WIDGET */
		temp1=Rmalloc(103+1);
		sprintf(temp1,"%s%s",
			"IF [ADD PHONE NUMBER]!=\"\" THEN RETURN_VALUE=[ADD PHONE NUMBER]\nELSE RETURN_VALUE=[USERS PHONE]\nF",
			"I\n");
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
			temp1=Rmalloc(9+6+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [OLHELP] Power Add [FEEDBACKC], Can Not Save Power Add!");
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
static void SCN_ADD_FEEDBACK()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","ADD FEEDBACK");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"HTTP USE PROXY","Use Proxy","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Proxy:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(23+1);
		sprintf(temp2,"%s",
			"([HTTP USE PROXY]=TRUE)");
		temp3=Rmalloc(23+1);
		sprintf(temp3,"%s",
			"([HTTP USE PROXY]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"HTTP PROXY","Http Proxy:","","",0,40,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","Port:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(23+1);
		sprintf(temp2,"%s",
			"([HTTP USE PROXY]=TRUE)");
		temp3=Rmalloc(23+1);
		sprintf(temp3,"%s",
			"([HTTP USE PROXY]=TRUE)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"HTTP PROXY PORT","Http Proxy Port:","","",0,4,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_SUBJECT","Add Subject","","",0,120,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Email:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD EMAIL","Add Email","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PHONE NUMBER","Add Phone Number","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Comments:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		temp4=Rmalloc(8+1);
		sprintf(temp4,"%s",
			"\"SAVE\"");
		ADVaddwdgt(scrn,8,"ADD_COMMENTS","Add Comments","","",10,65,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(79+1);
		sprintf(temp2,"%s",
			"([ADD_WEB_ID]!=\"\")");
		temp3=Rmalloc(79+1);
		sprintf(temp3,"%s",
			"([ADD_WEB_ID]!=\"\")");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"SAVE","Submit","","",0,0,20,temp1,temp2,temp3,temp4);
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
			temp1=Rmalloc(12+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [ADD FEEDBACK], Can Not Save Maintain Master!");
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
static void SCN_ADD_FEEDBACK_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","ADD FEEDBACK DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,200,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_comments:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_web_id:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_web_login:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_web_password:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Http Proxy:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Http Proxy Port:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Http Proxy To Use:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Http Use Proxy:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Software Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_COMMENTS POSITION","Add_comments Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_COMMENTS LENGTH","Add_comments Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_SUBJECT POSITION","Add_subject Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_SUBJECT LENGTH","Add_subject Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_ID POSITION","Add_web_id Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_ID LENGTH","Add_web_id Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_LOGIN POSITION","Add_web_login Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_LOGIN LENGTH","Add_web_login Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_PASSWORD POSITION","Add_web_password Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_PASSWORD LENGTH","Add_web_password Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY POSITION","Http Proxy Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY LENGTH","Http Proxy Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY PORT POSITION","Http Proxy Port Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY PORT LENGTH","Http Proxy Port Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY TO USE POSITION","Http Proxy To Use Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP PROXY TO USE LENGTH","Http Proxy To Use Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP USE PROXY POSITION","Http Use Proxy Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"HTTP USE PROXY LENGTH","Http Use Proxy Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOFTWARE NAME POSITION","Software Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOFTWARE NAME LENGTH","Software Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(24+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [ADD FEEDBACK DEFINE LIST], Can Not Save Maintain Master!");
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
static void MNU_OLHELP_ADD_FEEDBACK()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("OLHELP ADD FEEDBACK","doadd.EXT OLHELP \"ADD FEEDBACK\"");
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
void ADDFEEDBACK()
{
	MNU_OLHELP_ADD_FEEDBACK();
	ADD_ADD_FEEDBACK();
	ADD_FEEDBACKC();
	SCN_ADD_FEEDBACK();
	SCN_ADD_FEEDBACK_DEFINE_LIST();
}
