/* OLHELP's - Make (CYBERSUPPORT) Power Add */
#include <cstdio>
#include <mkadd.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>

extern char *module;
extern APPlib *errorlist;

static void ADD_CYBERSUPPORT()
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
	temp2=Rmalloc(150+1);
	sprintf(temp2,"%s",
		"([ADD_SUBJECT]=\"\") OR (([ADD_PRODUCT]=\"\") OR ([ADD_PROCESS]=\"\") OR ([ADD_INSTALL_DATE]=\"\") OR ([ADD_COMPILE_DATE]=\"\"))");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(890+1);
	sprintf(temp3,"%s%s%s%s",
		"IF([ADD_SUBJECT]=\"\") THEN RETURN_VALUE=\"Error:  We cannot process a request without detailed info",
		"rmation describing the subject of the request.\" ELSE \nIF(([ADD_PRODUCT]=\"\") OR ([ADD_PROCESS]=\"",
		"\")) THEN RETURN_VALUE=\"Error:  We cannot process a request without having a product/module and ass",
		"ociated process to appropriate technical respresentative.\" ELSE \nIF([ADD_INSTALL_DATE]=\"\") THEN RETURN_VALUE=\"Error:  Please enter your installation date off the \"+[ADD_PRODUCT]+\"'s Menu.\" ELSE \nIF([ADD_COMPILE_DATE]=\"\") THEN RETURN_VALUE=\"Error:  Please enter you compile date off the \"+[ADD_PRODUCT]+\"'s Menu.\" RETURN_VALUE=\"\" FI FI");


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
	add=RDAPowerAddNEW(module,"CYBERSUPPORT",temp1,"ADD CYBERSUPPORT REQUEST","ADD CYBERSUPPORT REQUEST DEFINE LIST","",3,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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

/* CYBERSUPPORTC wwwdoadd BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"OLHELP");
		addAPPlib(args,"CYBERSUPPORTC");
		x=addPowerAddButton(add,"wwwdoadd","","CYBERSUPPORTC","OLHELP","RSCNOT","RSCNOT KEY",temp1,1,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_COMPILE_DATE]","ADD_COMPILE_DATE");
			x=addPowerAddButtonField(button,"[ADD_EMAIL_ADDRESS]","ADD_EMAIL_ADDRESS");
			x=addPowerAddButtonField(button,"[ADD_INSTALL_DATE]","ADD_INSTALL_DATE");
			x=addPowerAddButtonField(button,"[ADD_PHONE_EXTENSION]","ADD_PHONE_EXTENSION");
			x=addPowerAddButtonField(button,"[ADD_PHONE_NUMBER]","ADD_PHONE_NUMBER");
			x=addPowerAddButtonField(button,"[ADD_PROCESS]","ADD_PROCESS");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT]","ADD_PRODUCT");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT_LINE]","ADD_PRODUCT_LINE");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[HTTP PROXY PORT]","HTTP_PROXY_PORT");
			x=addPowerAddButtonField(button,"[HTTP PROXY TO USE]","HTTP_PROXY_TO_USE");
			x=addPowerAddButtonField(button,"[HTTP PROXY]","HTTP_PROXY");
			x=addPowerAddButtonField(button,"[HTTP USE PROXY]","HTTP_USE_PROXY");
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
		temp1=Rmalloc(110+1);
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
		temp1=Rmalloc(119+1);
		sprintf(temp1,"%s","[USERS WEB PASSWORD]");
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

/* ADD_EMAIL_ADDRESS EDIT WIDGET */
		temp1=Rmalloc(114+1);
		sprintf(temp1,"%s","[USERS EMAIL]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_EMAIL_ADDRESS",1,60,2,temp1,0,"","","",0,NULL,temp2,"","ADD_EMAIL_ADDRESS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD_PHONE_NUMBER EDIT WIDGET */
		temp1=Rmalloc(99+1);
		sprintf(temp1,"%s",
			"IF([USERS PHONE]!=\"\") THEN RETURN_VALUE=[USERS PHONE]\n	ELSE RETURN_VALUE=[ORGANIZATION PHONE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PHONE_NUMBER",3,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PHONE_NUMBER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PHONE_EXTENSION EDIT WIDGET */
		temp1=Rmalloc(138+1);
		sprintf(temp1,"%s%s",
			"IF([USERS PHONE EXTENSION]!=\"\") THEN RETURN_VALUE=[USERS PHONE EXTENSION]\nELSE RETURN_VALUE=[ADD_",
			"PHONE_EXTENSION] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PHONE_EXTENSION",1,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PHONE_EXTENSION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD_PRODUCT_LINE EDIT WIDGET */
		temp1=Rmalloc(282+1);
		sprintf(temp1,"%s",
			"IF([SOFTWARE TYPE]=0) THEN RETURN_VALUE=\"XPERT\" \nELSE IF([SOFTWARE TYPE]=1) THEN RETURN_VALUE=\"OpenRDA\" \nELSE RETURN_VALUE=\"MBGUI\" FI FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PRODUCT_LINE",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PRODUCT_LINE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PRODUCT EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s","");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PRODUCT",1,15,0,temp1,0,"","","",0,NULL,temp2,"","ADD_PRODUCT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PROCESS EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s","");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PROCESS",1,40,0,temp1,0,"","","",0,NULL,temp2,"","ADD_PROCESS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD_INSTALL_DATE EDIT WIDGET */
		temp1=Rmalloc(222+1);
		sprintf(temp1,"%s%s%s",
			"IF((GENERIC_SETUP_VARIABLE(((\"[\"+[ADD_PRODUCT]+\"][INSTALLATION DATE]\"))))!=\"\") THEN \n	RETURN_",
			"VALUE=(GENERIC_SETUP_VARIABLE(((\"[\"+[ADD_PRODUCT]+\"][INSTALLATION DATE]\")))) \nELSE RETURN_VALUE",
			"=[ADD_INSTALL_DATE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_INSTALL_DATE",16,10,2,temp1,0,"","","",0,NULL,temp2,"","ADD_INSTALL_DATE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_COMPILE_DATE EDIT WIDGET */
		temp1=Rmalloc(211+1);
		sprintf(temp1,"%s%s%s",
			"IF((GENERIC_SETUP_VARIABLE(((\"[\"+[ADD_PRODUCT]+\"][COMPILE DATE]\"))))!=\"\") THEN \n	RETURN_VALUE",
			"=(GENERIC_SETUP_VARIABLE(((\"[\"+[ADD_PRODUCT]+\"][COMPILE DATE]\"))))\nELSE RETURN_VALUE=[ADD_COMPI",
			"LE_DATE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_COMPILE_DATE",16,10,2,temp1,0,"","","",0,NULL,temp2,"","ADD_COMPILE_DATE",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD_SUBJECT",14,0,0,temp1,0,"","","",0,NULL,temp2,"","ADD_SUBJECT",temp3,0,NULL,0,NULL);
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
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [OLHELP] Power Add [CYBERSUPPORT], Can Not Save Power Add!");
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
static void ADD_CYBERSUPPORTC()
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
	temp2=Rmalloc(70+1);
	sprintf(temp2,"%s",
		"([ADD_SUBJECT]=\"\") OR (([ADD_PRODUCT]=\"\") OR ([ADD_PROCESS]=\"\"))");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(390+1);
	sprintf(temp3,"%s%s%s%s",
		"IF([ADD_SUBJECT]=\"\") THEN RETURN_VALUE=\"Error:  We cannot process a request without detailed info",
		"rmation describing the subject of the request.\" ELSE \nIF(([ADD_PRODUCT]=\"\") OR ([ADD_PROCESS]=\"",
		"\")) THEN RETURN_VALUE=\"Error:  We cannot process a request without having a product/module and ass",
		"ociated process to appropriate technical respresentative.\" ELSE \nRETURN_VALUE=\"\" FI FI");


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
	add=RDAPowerAddNEW(module,"CYBERSUPPORTC",temp1,"ADD CYBERSUPPORT REQUEST","ADD CYBERSUPPORT REQUEST DEFINE LIST","",1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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

/* POST CYBERSUPPORT HTTP POST-GET BUTTON  */
		temp1=Rmalloc(54+1);
		sprintf(temp1,"%s",
			"([HTTP USE PROXY]=FALSE) OR ([HTTP PROXY TO USE]=\"\")");
		
		args=APPlibNEW();
		temp2=Rmalloc(141+1);
		sprintf(temp2,"%s",
			"\"http://\"+(GENERIC_SETUP_VARIABLE(\"[UTILITIES][RDA SUPPORT IP ADDRESS]\"))+\":80/cgi-bin/ADDREQUEST.cgi\"");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"HTTP POST-GET","","POST CYBERSUPPORT","OLHELP","URSCNOT","URSCNOT KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[ADD_EMAIL_ADDRESS]","ADD_EMAIL_ADDRESS");
			x=addPowerAddButtonField(button,"[ADD_PHONE_NUMBER]","ADD_PHONE_NUMBER");
			x=addPowerAddButtonField(button,"[ADD_PHONE_EXTENSION]","ADD_PHONE_EXTENSION");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT_LINE]","ADD_PRODUCT_LINE");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT]","ADD_PRODUCT");
			x=addPowerAddButtonField(button,"[ADD_PROCESS]","ADD_PROCESS");
			x=addPowerAddButtonField(button,"[ADD_INSTALL_DATE]","ADD_INSTALL_DATE");
			x=addPowerAddButtonField(button,"[ADD_COMPILE_DATE]","ADD_COMPILE_DATE");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
		}
/* POST CYBERSUPPORT HTTP POST-GET BUTTON  */
		temp1=Rmalloc(55+1);
		sprintf(temp1,"%s",
			"([HTTP USE PROXY]=TRUE) AND ([HTTP PROXY TO USE]!=\"\")");
		
		args=APPlibNEW();
		temp2=Rmalloc(141+1);
		sprintf(temp2,"%s",
			"\"http://\"+(GENERIC_SETUP_VARIABLE(\"[UTILITIES][RDA SUPPORT IP ADDRESS]\"))+\":80/cgi-bin/ADDREQUEST.cgi\"");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(19+1);
		sprintf(temp2,"%s",
			"[HTTP PROXY TO USE]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"HTTP POST-GET","","POST CYBERSUPPORT","OLHELP","URSCNOT","URSCNOT KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[ADD_EMAIL_ADDRESS]","ADD_EMAIL_ADDRESS");
			x=addPowerAddButtonField(button,"[ADD_PHONE_NUMBER]","ADD_PHONE_NUMBER");
			x=addPowerAddButtonField(button,"[ADD_PHONE_EXTENSION]","ADD_PHONE_EXTENSION");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT_LINE]","ADD_PRODUCT_LINE");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT]","ADD_PRODUCT");
			x=addPowerAddButtonField(button,"[ADD_PROCESS]","ADD_PROCESS");
			x=addPowerAddButtonField(button,"[ADD_INSTALL_DATE]","ADD_INSTALL_DATE");
			x=addPowerAddButtonField(button,"[ADD_COMPILE_DATE]","ADD_COMPILE_DATE");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
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
		temp1=Rmalloc(110+1);
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
		temp1=Rmalloc(119+1);
		sprintf(temp1,"%s","[USERS WEB PASSWORD]");
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

/* ADD_EMAIL_ADDRESS EDIT WIDGET */
		temp1=Rmalloc(24+1);
		sprintf(temp1,"%s",
			"[SECURITY][USERS][EMAIL]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_EMAIL_ADDRESS",1,60,3,temp1,0,"","","",0,NULL,temp2,"","ADD_EMAIL_ADDRESS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PHONE_NUMBER EDIT WIDGET */
		temp1=Rmalloc(99+1);
		sprintf(temp1,"%s",
			"IF([USERS PHONE]!=\"\") THEN RETURN_VALUE=[USERS PHONE]\n	ELSE RETURN_VALUE=[ORGANIZATION PHONE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PHONE_NUMBER",3,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PHONE_NUMBER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PHONE_EXTENSION EDIT WIDGET */
		temp1=Rmalloc(138+1);
		sprintf(temp1,"%s%s",
			"IF([USERS PHONE EXTENSION]!=\"\") THEN RETURN_VALUE=[USERS PHONE EXTENSION]\nELSE RETURN_VALUE=[ADD_",
			"PHONE_EXTENSION] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PHONE_EXTENSION",1,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PHONE_EXTENSION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);


/* ADD_PRODUCT_LINE EDIT WIDGET */
		temp1=Rmalloc(282+1);
		sprintf(temp1,"%s",
			"IF([SOFTWARE TYPE]=0) THEN RETURN_VALUE=\"XPERT\" \nELSE IF([SOFTWARE TYPE]=1) THEN RETURN_VALUE=\"OpenRDA\" \nELSE RETURN_VALUE=\"MBGUI\" FI FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PRODUCT_LINE",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PRODUCT_LINE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PRODUCT EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PRODUCT",1,15,0,temp1,0,"","","",0,NULL,temp2,"","ADD_PRODUCT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PROCESS EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PROCESS",1,40,0,temp1,0,"","","",0,NULL,temp2,"","ADD_PROCESS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_INSTALL_DATE EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_INSTALL_DATE",1,10,0,temp1,0,"","","",0,NULL,temp2,"","ADD_INSTALL_DATE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_COMPILE_DATE EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_COMPILE_DATE",1,10,0,temp1,0,"","","",0,NULL,temp2,"","ADD_COMPILE_DATE",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD_SUBJECT",14,0,0,temp1,0,"","","",0,NULL,temp2,"","ADD_SUBJECT",temp3,0,NULL,0,NULL);
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
			temp1=Rmalloc(15+6+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [OLHELP] Power Add [CYBERSUPPORTC], Can Not Save Power Add!");
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
static void DEF_ADD_CYBERSUPPORT_REQUEST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("OLHELP","ADD CYBERSUPPORT REQUEST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("ADD_SUBJECT");

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
			temp1=Rmalloc(24+6+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [OLHELP] Screen [ADD CYBERSUPPORT REQUEST], Can Not Save Screen Defaults!");
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
static void MNU_OLHELP_CYBERSUPPORT()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("OLHELP CYBERSUPPORT","doadd.EXT OLHELP CYBERSUPPORT");
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
static void LST_CYBERSUPPORT_MODULE_LIST()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAScrolledListNew("OLHELP","CYBERSUPPORT MODULE LIST");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=2;
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
		scrn->contype=1;
		scrn->conmod=stralloc("ADD_PRODUCT");
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
			temp1=Rmalloc(24+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [CYBERSUPPORT MODULE LIST], Can Not Save Scrolled List!");
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
static void SCN_ADD_CYBERSUPPORT_REQUEST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","ADD CYBERSUPPORT REQUEST");
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
		ADVaddwdgt(scrn,5,"","Web ID:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_ID","Add_web_id:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Web Login:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_WEB_LOGIN","Add_web_login:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Password:","","",0,0,0,NULL,NULL,NULL,NULL);
#ifdef __GTK_H__
		ADVaddwdgt(scrn,0,"ADD_WEB_PASSWORD","Password:","","",0,15,3,NULL,NULL,NULL,NULL);
#endif
#ifndef __GTK_H__
		ADVaddwdgt(scrn,0,"ADD_WEB_PASSWORD","Password:","","",0,15,0,NULL,NULL,NULL,NULL);
#endif
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Email:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_EMAIL_ADDRESS","Add_email_address:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Phone:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PHONE_NUMBER","Add_phone_number:","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Ext:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PHONE_EXTENSION","Add_phone_extension:","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Product/Module:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"CYBERSUPPORT MODULE LIST","Module List:","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Process:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PROCESS","Add_process:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Install Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_INSTALL_DATE","Add_install_date:","","",0,10,0,NULL,NULL,NULL,"\"ADD_SUBJECT\"");
		ADVaddwdgt(scrn,5,"","Compile Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_COMPILE_DATE","Add_compile_date:","","",0,10,0,NULL,NULL,NULL,"\"ADD_SUBJECT\"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Subject of Request:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"ADD_SUBJECT","Add Subject:","","",10,65,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(165+1);
		sprintf(temp2,"%s%s",
			"([ADD_WEB_ID]!=\"\") AND ([ADD_WEB_LOGIN]!=\"\") AND ([ADD_WEB_PASSWORD]!=\"\") AND ([ADD_EMAIL_ADDR",
			"ESS]!=\"\") AND \n([ADD_PRODUCT]!=\"\")");
		temp3=Rmalloc(165+1);
		sprintf(temp3,"%s%s",
			"([ADD_WEB_ID]!=\"\") AND ([ADD_WEB_LOGIN]!=\"\") AND ([ADD_WEB_PASSWORD]!=\"\") AND ([ADD_EMAIL_ADDR",
			"ESS]!=\"\") AND \n([ADD_PRODUCT]!=\"\")");
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
			temp1=Rmalloc(24+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [ADD CYBERSUPPORT REQUEST], Can Not Save Maintain Master!");
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
static void SCN_ADD_CYBERSUPPORT_REQUEST_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","ADD CYBERSUPPORT REQUEST DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",400,200,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Http Use Proxy:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,5,"","Software Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,5,"","Add_email_address:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_phone_number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_phone_extension:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_product_line:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_product:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_process:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_install_date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_compile_date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Add_subject:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"ADD_EMAIL_ADDRESS POSITION","Add_email_address Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_EMAIL_ADDRESS LENGTH","Add_email_address Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PHONE_NUMBER POSITION","Add_phone_number Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PHONE_NUMBER LENGTH","Add_phone_number Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PHONE_EXTENSION POSITION","Add_phone_extension Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PHONE_EXTENSION LENGTH","Add_phone_extension Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PRODUCT_LINE POSITION","Add_product_line Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PRODUCT_LINE LENGTH","Add_product_line Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PRODUCT POSITION","Add_product Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PRODUCT LENGTH","Add_product Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PROCESS POSITION","Add_process Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_PROCESS LENGTH","Add_process Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_INSTALL_DATE POSITION","Add_install_date Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_INSTALL_DATE LENGTH","Add_install_date Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_COMPILE_DATE POSITION","Add_compile_date Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD_COMPILE_DATE LENGTH","Add_compile_date Length","","",0,3,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(36+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [ADD CYBERSUPPORT REQUEST DEFINE LIST], Can Not Save Maintain Master!");
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
static void ADD_AUTOSUPPORT()
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
	temp2=Rmalloc(70+1);
	sprintf(temp2,"%s",
		"([ADD_SUBJECT]=\"\") OR (([ADD_PRODUCT]=\"\") OR ([ADD_PROCESS]=\"\"))");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(390+1);
	sprintf(temp3,"%s%s%s%s",
		"IF([ADD_SUBJECT]=\"\") THEN RETURN_VALUE=\"Error:  We cannot process a request without detailed info",
		"rmation describing the subject of the request.\" ELSE \nIF(([ADD_PRODUCT]=\"\") OR ([ADD_PROCESS]=\"",
		"\")) THEN RETURN_VALUE=\"Error:  We cannot process a request without having a product/module and ass",
		"ociated process to appropriate technical respresentative.\" ELSE \nRETURN_VALUE=\"\" FI FI");


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
	add=RDAPowerAddNEW(module,"AUTOSUPPORT",temp1,"ADD CYBERSUPPORT REQUEST","ADD CYBERSUPPORT REQUEST DEFINE LIST","",1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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

/* POST CYBERSUPPORT HTTP POST BUTTON  */
		temp1=Rmalloc(54+1);
		sprintf(temp1,"%s",
			"([HTTP USE PROXY]=FALSE) OR ([HTTP PROXY TO USE]=\"\")");
		
		args=APPlibNEW();
		temp2=Rmalloc(141+1);
		sprintf(temp2,"%s",
			"\"http://\"+(GENERIC_SETUP_VARIABLE(\"[UTILITIES][RDA SUPPORT IP ADDRESS]\"))+\":80/cgi-bin/AUTOREQUEST.cgi\"");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"HTTP POST","","POST CYBERSUPPORT","OLHELP","URSCNOT","URSCNOT KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[ADD_EMAIL_ADDRESS]","ADD_EMAIL_ADDRESS");
			x=addPowerAddButtonField(button,"[ADD_PHONE_NUMBER]","ADD_PHONE_NUMBER");
			x=addPowerAddButtonField(button,"[ADD_PHONE_EXTENSION]","ADD_PHONE_EXTENSION");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT_LINE]","ADD_PRODUCT_LINE");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT]","ADD_PRODUCT");
			x=addPowerAddButtonField(button,"[ADD_PROCESS]","ADD_PROCESS");
			x=addPowerAddButtonField(button,"[ADD_INSTALL_DATE]","ADD_INSTALL_DATE");
			x=addPowerAddButtonField(button,"[ADD_COMPILE_DATE]","ADD_COMPILE_DATE");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
		}
/* POST CYBERSUPPORT HTTP POST BUTTON  */
		temp1=Rmalloc(55+1);
		sprintf(temp1,"%s",
			"([HTTP USE PROXY]=TRUE) AND ([HTTP PROXY TO USE]!=\"\")");
		
		args=APPlibNEW();
		temp2=Rmalloc(141+1);
		sprintf(temp2,"%s",
			"\"http://\"+(GENERIC_SETUP_VARIABLE(\"[UTILITIES][RDA SUPPORT IP ADDRESS]\"))+\":80/cgi-bin/AUTOREQUEST.cgi\"");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(19+1);
		sprintf(temp2,"%s",
			"[HTTP PROXY TO USE]");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"HTTP POST","","POST CYBERSUPPORT","OLHELP","URSCNOT","URSCNOT KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD_WEB_ID]","ADD_WEB_ID");
			x=addPowerAddButtonField(button,"[ADD_WEB_LOGIN]","ADD_WEB_LOGIN");
			x=addPowerAddButtonField(button,"[ADD_WEB_PASSWORD]","ADD_WEB_PASSWORD");
			x=addPowerAddButtonField(button,"[ADD_EMAIL_ADDRESS]","ADD_EMAIL_ADDRESS");
			x=addPowerAddButtonField(button,"[ADD_PHONE_NUMBER]","ADD_PHONE_NUMBER");
			x=addPowerAddButtonField(button,"[ADD_PHONE_EXTENSION]","ADD_PHONE_EXTENSION");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT_LINE]","ADD_PRODUCT_LINE");
			x=addPowerAddButtonField(button,"[ADD_PRODUCT]","ADD_PRODUCT");
			x=addPowerAddButtonField(button,"[ADD_PROCESS]","ADD_PROCESS");
			x=addPowerAddButtonField(button,"[ADD_INSTALL_DATE]","ADD_INSTALL_DATE");
			x=addPowerAddButtonField(button,"[ADD_COMPILE_DATE]","ADD_COMPILE_DATE");
			x=addPowerAddButtonField(button,"[ADD_SUBJECT]","ADD_SUBJECT");
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
		temp1=Rmalloc(47+1);
		sprintf(temp1,"%s",
			"GENERIC_SETUP_VARIABLE(\"[UTILITIES][WEB ID]\")");
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
		temp1=Rmalloc(110+1);
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
		temp1=Rmalloc(119+1);
		sprintf(temp1,"%s","[USERS WEB PASSWORD]");
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

/* ADD_EMAIL_ADDRESS EDIT WIDGET */
		temp1=Rmalloc(24+1);
		sprintf(temp1,"%s",
			"[SECURITY][USERS][EMAIL]");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_EMAIL_ADDRESS",1,60,3,temp1,0,"","","",0,NULL,temp2,"","ADD_EMAIL_ADDRESS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PHONE_NUMBER EDIT WIDGET */
		temp1=Rmalloc(99+1);
		sprintf(temp1,"%s",
			"IF([USERS PHONE]!=\"\") THEN RETURN_VALUE=[USERS PHONE]\n	ELSE RETURN_VALUE=[ORGANIZATION PHONE] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PHONE_NUMBER",3,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PHONE_NUMBER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PHONE_EXTENSION EDIT WIDGET */
		temp1=Rmalloc(119+1);
		sprintf(temp1,"%s%s",
			"IF([USERS PHONE EXTENSION]!=\"\") THEN RETURN_VALUE=[USERS PHONE EXTENSION]\nELSE RETURN_VALUE=[ADD_",
			"PHONE_EXTENSION] FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PHONE_EXTENSION",1,12,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PHONE_EXTENSION",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PRODUCT_LINE EDIT WIDGET */
		temp1=Rmalloc(82+1);
		sprintf(temp1,"%s",
			"IF([SOFTWARE TYPE]<2) THEN RETURN_VALUE=\"XPERT\" \nELSE RETURN_VALUE=\"MBGUI\" FI");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PRODUCT_LINE",1,15,2,temp1,0,"","","",0,NULL,temp2,"","ADD_PRODUCT_LINE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PRODUCT EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PRODUCT",1,15,0,temp1,0,"","","",0,NULL,temp2,"","ADD_PRODUCT",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_PROCESS EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_PROCESS",1,40,0,temp1,0,"","","",0,NULL,temp2,"","ADD_PROCESS",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_INSTALL_DATE EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_INSTALL_DATE",1,10,0,temp1,0,"","","",0,NULL,temp2,"","ADD_INSTALL_DATE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* ADD_COMPILE_DATE EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD_COMPILE_DATE",1,10,0,temp1,0,"","","",0,NULL,temp2,"","ADD_COMPILE_DATE",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD_SUBJECT",14,0,0,temp1,0,"","","",0,NULL,temp2,"","ADD_SUBJECT",temp3,0,NULL,0,NULL);
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
			temp1=Rmalloc(11+6+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [OLHELP] Power Add [AUTOSUPPORT], Can Not Save Power Add!");
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
void CYBERSUPPORT()
{
	ADD_CYBERSUPPORT();
	ADD_CYBERSUPPORTC();
	MNU_OLHELP_CYBERSUPPORT();
	LST_CYBERSUPPORT_MODULE_LIST();
	SCN_ADD_CYBERSUPPORT_REQUEST();
	SCN_ADD_CYBERSUPPORT_REQUEST_DEFINE_LIST();
	DEF_ADD_CYBERSUPPORT_REQUEST();
	ADD_AUTOSUPPORT();
}
