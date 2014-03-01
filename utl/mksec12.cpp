/* SECURITY's - Make (ADD MODULE SECURITY) Power Add */
/* CONTAINS HAND CODING in POWER WRITES */
#include <cstdio>
#include <mkdef.hpp>
#include <mkadd.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void ADD_MODULE_SECURITY()
{
	int x=0;
	PowerAdd *add=NULL;
	char *defdir=NULL;
	APPlib *args=NULL,*args2=NULL;
	MaintainButton *button=NULL;
	RDAaddsub *subord=NULL;
	AddWrite *Write=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;
	char *temp7=NULL,*temp8=NULL;
	char *temp9=NULL;

	temp1=Rmalloc(32+1);
	sprintf(temp1,"%s",
		"ADDS MODULES FOR EXISTING USERS.");
	temp2=Rmalloc(32+1);
	sprintf(temp2,"%s",
		"([ADD USER IDENTIFICATION]=\"\")");
	temp3=Rmalloc(132+1);
	sprintf(temp3,"%s%s",
		"IF ([ADD USER IDENTIFICATION]=\"\")\nTHEN RETURN_VALUE=\"Error:     You must enter a valid user iden",
		"tification before saving!!!\" FI");
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");
	temp7=Rmalloc(3+1);
	sprintf(temp7,"%s",
		"");
	temp8=Rmalloc(3+1);
	sprintf(temp8,"%s",
		"");
	temp9=Rmalloc(3+1);
	sprintf(temp9,"%s",
		"");
	add=RDAPowerAddNEW(module,"ADD MODULE SECURITY",temp1,"ADD MODULE SECURITY SCREEN","ADD MODULE SECURITY DEFINE LIST","",1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
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
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"SECURITY");
		addAPPlib(args,"ADD FILE SECURITY");
		x=addPowerAddButton(add,"doadd","","FILE SECURITY","SECURITY","FLDSEC","FLDSEC KEY",temp1,0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD USER IDENTIFICATION]","ADD_USER_IDENTIFICATION");
			x=addPowerAddButtonField(button,"[ADD MODULE NAME]","ADD_MODULE_NAME");
		}
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"SECURITY");
		addAPPlib(args,"ADD PROCESS SECURITY");
		x=addPowerAddButton(add,"doadd","","PROCESS SECURITY","SECURITY","PROCSEC","PROCSEC KEY",temp1,0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD USER IDENTIFICATION]","[ADD_USER_IDENTIFICATION]");
			x=addPowerAddButtonField(button,"[ADD MODULE NAME]","ADD_MODULE_NAME");
		}
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		addAPPlib(args,"SECURITY");
		addAPPlib(args,"ADD PROCESS SECURITY-NL");
		x=addPowerAddButton(add,"doadd","","PROCESS SECURITY-NL","SECURITY","PROCSEC","PROCSEC KEY",temp1,0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			button=add->buttons+(x-1);
			x=addPowerAddButtonField(button,"[ADD USER IDENTIFICATION]","[ADD_USER_IDENTIFICATION]");
			x=addPowerAddButtonField(button,"[ADD MODULE NAME]","ADD_MODULE_NAME");
		}

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddSupporting(add,"SECURITY","USERS",1,0,NULL,"USERS KEY",temp1,0,"","","","");
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


		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+1);
			sprintf(temp1,"%s",
				"[ADD MODULE NAME]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+1);
			sprintf(temp1,"%s",
				"[ADD ALL PROCESSES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL PROCESSES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+1);
			sprintf(temp1,"%s",
				"[ADD EXECUTE FUNCTIONS]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(26+1);
			sprintf(temp1,"%s",
				"[ADD DELETE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][DELETE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(XPERT_SETUP->software_type==2)
		{
		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"\"PAYROLL\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(4+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(26+1);
			sprintf(temp1,"%s",
				"[ADD DELETE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][DELETE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
/* SECURITY PROCSEC POWER WRITE */

		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","PROCSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"\"PAYROLL\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(4+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][EXECUTE PROCESS]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(5+1);
			sprintf(temp1,"%s",
				"FALSE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][DELETEFLAG]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+1);
			sprintf(temp1,"%s",
				"\"MTN PAYLOC\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][PROCESS NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
/* SECURITY PROCSEC POWER WRITE */

		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","PROCSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"\"PAYROLL\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(4+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][EXECUTE PROCESS]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(5+1);
			sprintf(temp1,"%s",
				"FALSE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][DELETEFLAG]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(24+1);
			sprintf(temp1,"%s",
				"\"MTN PRSNNL\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][PROCESS NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}


		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"RLVEMGT\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+1);
			sprintf(temp1,"%s",
				"\"LVEMGT\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+1);
			sprintf(temp1,"%s",
				"[ADD EXECUTE FUNCTIONS]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}

/* SECURITY PROCSEC POWER WRITE */

		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"RLVEMGT\")");
		x=addPowerAddWrite(add,"SECURITY","PROCSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+1);
			sprintf(temp1,"%s",
				"\"LVEMGT\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(4+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][EXECUTE PROCESS]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(5+1);
			sprintf(temp1,"%s",
				"FALSE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][DELETEFLAG]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(26+1);
			sprintf(temp1,"%s",
				"\"MTN LVETRN.REMOTEQUERY\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][PROCESS NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}


/* SECURITY MODSEC POWER WRITE */

		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"RLVEMGT\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"\"PAYROLL\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		temp1=Rmalloc(98+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"RLVEMGT\") OR ([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(21+1);
			sprintf(temp1,"%s",
				"\"UTILITIES\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(4+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"FALSE");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL PROCESSES]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		temp1=Rmalloc(97+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"RLVEMGT\") OR ([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","PROCSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"\"UTILITIES\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(4+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][EXECUTE PROCESS]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(5+1);
			sprintf(temp1,"%s",
				"FALSE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][DELETEFLAG]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(44+1);
			sprintf(temp1,"%s",
				"\"MAIN MENU\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][PROCESS NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		temp1=Rmalloc(97+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"RLVEMGT\") OR ([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","PROCSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"\"UTILITIES\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(4+1);
			sprintf(temp1,"%s",
				"TRUE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][EXECUTE PROCESS]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(5+1);
			sprintf(temp1,"%s",
				"FALSE");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][DELETEFLAG]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(54+1);
			sprintf(temp1,"%s",
				"\"TEXT BROWSER\"");
			x=addPowerAddWriteField(Write,"[SECURITY][PROCSEC][PROCESS NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		if(XPERT_SETUP->CSHDWR==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"SECURITY\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"UTILITIES\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}

		if(XPERT_SETUP->BLDPRMT==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"BLDPRMT\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		if(XPERT_SETUP->OCCTAX==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"OCCTAX\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"FINMGT\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(XPERT_SETUP->RLSTMGT==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"RLSTMGT\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		if(XPERT_SETUP->MISCBILL==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"MISCBILL\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		if(XPERT_SETUP->PROPERTY==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"PROPERTY\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		if(XPERT_SETUP->UTLBLL==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"UTLBLL\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		if(XPERT_SETUP->BNKREC==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"CSHDWR\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"BNKREC\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		}
		if(XPERT_SETUP->FINMGT==TRUE)
		{
			temp1=Rmalloc(30+1);
			sprintf(temp1,"%s",
				"([ADD MODULE NAME]=\"FINMGT\")");
			x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
			if(temp1!=NULL) Rfree(temp1);
			if(x!=(-1))
			{
				Write=add->Writes+(x-1);
				temp1=Rmalloc(25+1);
				sprintf(temp1,"%s",
					"[ADD USER IDENTIFICATION]");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(12+1);
				sprintf(temp1,"%s",
					"\"SECURITY\"");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(XPERT_SETUP->software_type<2)
		{
		if(XPERT_SETUP->EMPABS==TRUE)
		{
			temp1=Rmalloc(30+1);
			sprintf(temp1,"%s",
				"([ADD MODULE NAME]=\"EMPABS\")");
			x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
			if(temp1!=NULL) Rfree(temp1);
			if(x!=(-1))
			{
				Write=add->Writes+(x-1);
				temp1=Rmalloc(25+1);
				sprintf(temp1,"%s",
					"[ADD USER IDENTIFICATION]");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(10+1);
				sprintf(temp1,"%s",
					"\"FINMGT\"");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
			}
			temp1=Rmalloc(30+1);
			sprintf(temp1,"%s",
				"([ADD MODULE NAME]=\"EMPABS\")");
			x=addPowerAddWrite(add,"SECURITY","FILESEC",temp1,0,NULL);
			if(temp1!=NULL) Rfree(temp1);
			if(x!=(-1))
			{
				Write=add->Writes+(x-1);
				temp1=Rmalloc(25+1);
				sprintf(temp1,"%s",
					"[ADD USER IDENTIFICATION]");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][USER IDENTIFICATION]",0,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(10+1);
				sprintf(temp1,"%s",
					"\"FINMGT\"");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][MODULE NAME]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(11+1);
				sprintf(temp1,"%s",
					"\"FINYEAR\"");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][FILE NAME]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][PRINT FILE]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][READ FILE]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][WRITE FILE]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(XPERT_SETUP->LVEMGT==TRUE)
		{
			temp1=Rmalloc(30+1);
			sprintf(temp1,"%s",
				"([ADD MODULE NAME]=\"LVEMGT\")");
			x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
			if(temp1!=NULL) Rfree(temp1);
			if(x!=(-1))
			{
				Write=add->Writes+(x-1);
				temp1=Rmalloc(25+1);
				sprintf(temp1,"%s",
					"[ADD USER IDENTIFICATION]");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(10+1);
				sprintf(temp1,"%s",
					"\"FINMGT\"");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
			}
			temp1=Rmalloc(30+1);
			sprintf(temp1,"%s",
				"([ADD MODULE NAME]=\"LVEMGT\")");
			x=addPowerAddWrite(add,"SECURITY","FILESEC",temp1,0,NULL);
			if(temp1!=NULL) Rfree(temp1);
			if(x!=(-1))
			{
				Write=add->Writes+(x-1);
				temp1=Rmalloc(25+1);
				sprintf(temp1,"%s",
					"[ADD USER IDENTIFICATION]");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][USER IDENTIFICATION]",0,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(10+1);
				sprintf(temp1,"%s",
					"\"FINMGT\"");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][MODULE NAME]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(11+1);
				sprintf(temp1,"%s",
					"\"FINYEAR\"");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][FILE NAME]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][PRINT FILE]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][READ FILE]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(4+1);
				sprintf(temp1,"%s",
					"TRUE");
				x=addPowerAddWriteField(Write,"[SECURITY][FILESEC][WRITE FILE]",1,temp1);
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(XPERT_SETUP->PRSNNL==TRUE)
		{
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"IIDMGT\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+1);
			sprintf(temp1,"%s",
				"[ADD ALL PROCESSES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL PROCESSES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+1);
			sprintf(temp1,"%s",
				"[ADD EXECUTE FUNCTIONS]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(26+1);
			sprintf(temp1,"%s",
				"[ADD DELETE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][DELETE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		temp1=Rmalloc(31+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"JOBPERF\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+1);
			sprintf(temp1,"%s",
				"[ADD ALL PROCESSES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL PROCESSES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+1);
			sprintf(temp1,"%s",
				"[ADD EXECUTE FUNCTIONS]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][EXECUTE FUNCTIONS]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(26+1);
			sprintf(temp1,"%s",
				"[ADD DELETE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][DELETE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		if(XPERT_SETUP->POSTRK==TRUE)
		{
/* SECURITY MODSEC POWER WRITE */

		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+1);
			sprintf(temp1,"%s",
				"\"POSTRK\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+1);
			sprintf(temp1,"%s",
				"[ADD DELETE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][DELETE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}

		}
		if(XPERT_SETUP->ARCHIVE==TRUE)
		{
/* SECURITY MODSEC POWER WRITE */

		temp1=Rmalloc(30+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PRSNNL\")");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+1);
			sprintf(temp1,"%s",
				"\"ARCHIVE\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+1);
			sprintf(temp1,"%s",
				"[ADD DELETE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][DELETE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}

		if(XPERT_SETUP->PURORD==TRUE)
		{
		temp1=Rmalloc(331+1);
		sprintf(temp1,"%s",
			"([ADD MODULE NAME]=\"PURORD\") AND ([ADD READ DATA]=TRUE) AND ([ADD WRITE DATA]=TRUE) AND ([ADD EXECUTE FUNCTIONS]=TRUE)");
		x=addPowerAddWrite(add,"SECURITY","MODSEC",temp1,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(x!=(-1))
		{
			Write=add->Writes+(x-1);
			temp1=Rmalloc(25+1);
			sprintf(temp1,"%s",
				"[ADD USER IDENTIFICATION]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][USER IDENTIFICATION]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+1);
			sprintf(temp1,"%s",
				"\"SECURITY\"");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][MODULE NAME]",1,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD ALL FILES]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][ALL FILES]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD PRINT DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][PRINT DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+1);
			sprintf(temp1,"%s",
				"[ADD READ DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][READ DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+1);
			sprintf(temp1,"%s",
				"[ADD WRITE DATA]");
			x=addPowerAddWriteField(Write,"[SECURITY][MODSEC][WRITE DATA]",0,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		}
		}










		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"ADD USER IDENTIFICATION",0,0,1,temp1,0,"","","",0,NULL,temp2,"","ADD USER IDENTIFICATION",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD MODULE NAME",0,0,0,temp1,0,"","","",0,NULL,temp2,"","ADD MODULE NAME",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD ALL FILES",10,1,0,temp1,0,"","","",0,NULL,temp2,"","ADD ALL FILES",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD ALL PROCESSES",10,1,0,temp1,0,"","","",0,NULL,temp2,"","ADD ALL PROCESSES",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD EXECUTE FUNCTIONS",10,1,0,temp1,0,"","","",0,NULL,temp2,"","ADD EXECUTE FUNCTIONS",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD READ DATA",10,1,0,temp1,0,"","","",0,NULL,temp2,"","ADD READ DATA",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD WRITE DATA",10,1,0,temp1,0,"","","",0,NULL,temp2,"","ADD WRITE DATA",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD DELETE DATA",10,1,0,temp1,0,"","","",0,NULL,temp2,"","ADD DELETE DATA",temp3,0,NULL,0,NULL);
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
		x=addPowerAddEditWdgts(add,"ADD PRINT DATA",10,1,0,temp1,0,"","","",0,NULL,temp2,"","ADD PRINT DATA",temp3,0,NULL,0,NULL);
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
			temp1=Rmalloc(19+8+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [SECURITY] Power Add [ADD MODULE SECURITY], Can Not Save Power Add!");
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
static void ADD_MODULE_SECURITY_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","ADD MODULE SECURITY SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"PREVIOUS ADD'S","Previous Adds","","",5,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		temp4=Rmalloc(26+1);
		sprintf(temp4,"%s",
			"\"ADD MODSEC MODULE LIST\"");
		ADVaddwdgt(scrn,0,"ADD USER IDENTIFICATION","User Identification","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","User Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[USERS][USER NAME]","User Name","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Modules:","","",0,0,0,NULL,NULL,NULL,NULL);
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
			"\"ADD MODULE NAME\"");
		ADVaddwdgt(scrn,7,"ADD MODSEC MODULE LIST","Module List","","",1,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,0,"ADD MODULE NAME","Module Name","","",0,15,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD ALL PROCESSES","All Processes","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD ALL FILES","All Files","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD EXECUTE FUNCTIONS","Execute Functions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD READ DATA","Read Data","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD WRITE DATA","Write Data","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD PRINT DATA","Print Data","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ADD DELETE DATA","Delete Data","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"[EXTENSIONS SETUP]=TRUE",NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"FILE SECURITY","Add File Security","","",0,0,0,NULL,NULL,"([ADD ALL FILES]=FALSE)",NULL);
		ADVaddwdgt(scrn,6,"PROCESS SECURITY","Add Process Security","","",0,0,0,NULL,NULL,"([ADD ALL PROCESSES]=FALSE) AND ([ADD EXECUTE FUNCTIONS]=TRUE)",NULL);
		ADVaddwdgt(scrn,6,"PROCESS SECURITY-NL","Add Process Security (NL)","","",0,0,0,NULL,NULL,"([ADD ALL PROCESSES]=FALSE) AND ([ADD EXECUTE FUNCTIONS]=TRUE)",NULL);
		ADVaddwdgt(scrn,6,"RESET DEFAULTS","Reset Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(26+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [ADD MODULE SECURITY SCREEN], Can Not Save Maintain Master!");
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
static void ADD_MODULE_SECURITY_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("SECURITY","ADD MODULE SECURITY DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter the position and length for each field.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD MODULE NAME POSITION","Module Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD MODULE NAME LENGTH","Module Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","User Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USER IDENTIFICATION POSITION","User Identification Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD USER IDENTIFICATION LENGTH","User Identification Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","All Processes:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD ALL PROCESSES POSITION","All Processes Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD ALL PROCESSES LENGTH","All Processes Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","All Files:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD ALL FILES POSITION","All Files Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD ALL FILES LENGTH","All Files Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Execute Functions:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD EXECUTE FUNCTIONS POSITION","Execute Functions Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD EXECUTE FUNCTIONS LENGTH","Execute Functions Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Read Data:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD READ DATA POSITION","Read Data Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD READ DATA LENGTH","Read Data Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Write data:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WRITE DATA POSITION","Write Data Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD WRITE DATA LENGTH","Write Data Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Print Data:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PRINT DATA POSITION","Print Data Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD PRINT DATA LENGTH","Print Data Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Delete data:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DELETE DATA POSITION","Delete Data Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADD DELETE DATA LENGTH","Delete Data Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help Define List","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(31+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [SECURITY] Screen [ADD MODULE SECURITY DEFINE LIST], Can Not Save Maintain Master!");
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
static void DEF_ADD_MODULE_SECURITY_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("SECURITY","ADD MODULE SECURITY DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("SELECT");
		addDefaultFieldSHORT(def,"ADD USER IDENTIFICATION POSITION",TRUE,7,1);
		addDefaultFieldSHORT(def,"ADD USER IDENTIFICATION LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"ADD MODULE NAME POSITION",TRUE,7,2);
		addDefaultFieldSHORT(def,"ADD MODULE NAME LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"ADD ALL FILES POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"ADD ALL FILES LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"ADD ALL PROCESSES POSITION",TRUE,7,4);
		addDefaultFieldSHORT(def,"ADD ALL PROCESSES LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"ADD EXECUTE FUNCTIONS POSITION",TRUE,7,5);
		addDefaultFieldSHORT(def,"ADD EXECUTE FUNCTIONS LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"ADD READ DATA POSITION",TRUE,7,6);
		addDefaultFieldSHORT(def,"ADD READ DATA LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"ADD WRITE DATA POSITION",TRUE,7,7);
		addDefaultFieldSHORT(def,"ADD WRITE DATA LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"ADD PRINT DATA POSITION",TRUE,7,8);
		addDefaultFieldSHORT(def,"ADD PRINT DATA LENGTH",TRUE,7,1);
		addDefaultFieldSHORT(def,"ADD DELETE DATA POSITION",TRUE,7,9);
		addDefaultFieldSHORT(def,"ADD DELETE DATA LENGTH",TRUE,7,1);

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
			temp1=Rmalloc(31+8+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [SECURITY] Screen [ADD MODULE SECURITY DEFINE LIST], Can Not Save Screen Defaults!");
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
static void LST_ADD_MODSEC_MODULE_LIST()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAScrolledListNew("SECURITY","ADD MODSEC MODULE LIST");
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
		scrn->conmod=stralloc("ADD MODULE NAME");
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
			temp1=Rmalloc(22+8+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [SECURITY] RDAScrolledList [ADD MODSEC MODULE LIST], Can Not Save Scrolled List!");
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
void ADDMODSEC()
{
	ADD_MODULE_SECURITY();
	ADD_MODULE_SECURITY_DEFINE_LIST();
	ADD_MODULE_SECURITY_SCREEN();
	LST_ADD_MODSEC_MODULE_LIST();
	DEF_ADD_MODULE_SECURITY_DEFINE_LIST();
}
