/* load value functions */
#include<ldval.hpp>

char *OpenRDA_operand[16]={"+","-","*","/","%","(",")","=","<",">",">=","<=","<>","C","NC","MOD"};
char *OpenRDA_grouper[7]={" IF "," OR "," AND "," NOT "," THEN "," ELSE "," FI "};
char *OpenRDA_controls[50]={"ADD_DASHES","APPEND","BASE_SIXTYFOUR_DECODE", 
	"BASE_SIXTYFOUR_DECODE_TO_HEX","BASE_SIXTYFOUR_ENCODE",
	"BASE_SIXTYFOUR_WRITE_FILE",
	"BOOLEAN_TO_STRING","COMPUTE_TABLE",
	"COMPUTE_TABLE_BASE","COMPUTE_TABLE_RATE","COMPUTE_MATRIX_LINEARLY",
	"COMPUTE_MATRIX_ROWCOLUMN","DATE_TO_VALUE","DATE_TO_YEARDAY",
	"DECIMAL_TO_STRING","DOLLARS_TO_STRING","DOLLARSNS_TO_STRING",
	"DOLLARS_TO_TEXT",
	"EVALUATE_BOOLEAN","EVALUATE_SHORT","EVALUATE_INTEGER",
	"EVALUATE_DOUBLE","EVALUATE_STRING","FALSE","FLOAT_TO_STRING",
	"GENERIC_SETUP_VARIABLE","GENERIC_COUNTER","HEX_TO_BASE_SIXTYFOUR_ENCODE",
	"HEX_TO_STRING","INTEGER_TO_ENCRYPTEDA",
	"INTEGER_TO_STRING","MD_FIVE_SUM","MERGE_ACCOUNT","READ_FILE",
	"READ_FILE_BASE_SIXTYFOUR","READ_FILE_HEX",
	"RETURN_VALUE","ROUND","ROUNDUP","SHORT_TO_STRING",
	"SPELL","STRING_LENGTH","STRING_TO_HEX","STRING_TO_LOWER",
	"STRING_TO_UPPER",
	"STRING_TO_VALUE","TRUE","VALUE_TO_STRING","VALUE_TO_TIME","WT_BCRYPT_VALUE"};

RDArsrc *ldvalrsrc=NULL;
APPlib *loadlist=NULL;

LoadValue *xLoadValueNew(RDArsrc *r,RDArsrc *val,char *fieldname,
	int line,char *file)
{
	LoadValue *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG LoadValueNew for Building Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(LoadValue));
	tmp->r=r;
	tmp->val=val;
	tmp->fieldname=stralloc(fieldname);
	return(tmp);
}
void xFreeLoadValue(LoadValue *l,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG FreeLoadValue at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(l!=NULL)
	{
		if(l->r!=NULL) free_rsrc(l->r);
		if(l->fieldname!=NULL) Rfree(l->fieldname);
		Rfree(l);
	}
}
void quitldval(RDArsrc *r,LoadValue *load)
{
	FreeLoadValue(load);
	if(loadlist!=NULL) { freeapplib(loadlist); loadlist=NULL; }
}
void okldval(RDArsrc *r,LoadValue *load)
{
	int pos=0,selected=0;
	char *tmp1=NULL,*tmp2=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG okldval Saving the LoadValue item into field [%s] on Screen [%s] [%s].",load->fieldname,load->val->module,load->val->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(FINDRSCGETINT(load->r,"LOAD LIST",&selected)) return;
	readwidget(load->val,load->fieldname);
	pos=FINDRSCGETCURSORPOSITION(load->val,load->fieldname);
	FINDRSCGETSTRING(load->val,load->fieldname,&tmp2);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG okldval Loading Value [%s] at position [%d] in Field [%s] on Screen [%s] [%s].",loadlist->libs[selected],pos,load->fieldname,load->val->module,load->val->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(pos!=(-1))
	{
		tmp1=Rmalloc(RDAstrlen(loadlist->libs[selected])+(tmp2!=NULL ? RDAstrlen(tmp2):0)+1);
		if(pos>0 && pos<RDAstrlen(tmp2) && tmp2!=NULL)
		{
			sprintf(tmp1,"%.*s%s%s",pos,(tmp2!=NULL ? &tmp2[0]:""),loadlist->libs[selected],(tmp2!=NULL ? &tmp2[pos]:""));
			if(tmp2!=NULL) Rfree(tmp2);
		} else if(pos>0 && pos>=RDAstrlen(tmp2) && tmp2!=NULL)
		{
			sprintf(tmp1,"%s%s",(tmp2!=NULL ? tmp2:""),loadlist->libs[selected]);
			if(tmp2!=NULL) Rfree(tmp2);
		} else if(pos==0)
		{
			sprintf(tmp1,"%s%s",loadlist->libs[selected],
				(tmp2!=NULL ? tmp2:""));
			if(tmp2!=NULL) Rfree(tmp2);
		}
		if(!FINDRSCSETSTRING(load->val,load->fieldname,tmp1)) 
			updatersrc(load->val,load->fieldname);
		FINDRSCSETCURSOR(load->val,load->fieldname,
			(pos+RDAstrlen(loadlist->libs[selected])));
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("DIAG okldval Setting New Value to [%s] cursor to position [%d] in field [%s] on Screen [%s] [%s].",tmp1,(pos+RDAstrlen(loadlist->libs[selected])),load->fieldname,load->val->module,load->val->screen);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(tmp1!=NULL) Rfree(tmp1);
	}
	quitldval(r,load);
}
void loadoperand(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;
	char *e=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG loadoperand Load Operand into Field [%s] on Screen [%s] [%s].",fieldname,r->module,r->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD OPERATOR SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=APPlibNEW();
	for(x=0;x<NE(OpenRDA_operand);++x) addAPPlib(loadlist,OpenRDA_operand[x]);
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
/*
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
*/
	if(ADVmakescrn(ldvalrsrc,TRUE))
	{
		e=Rmalloc(130+RDAstrlen(ldvalrsrc->module)+RDAstrlen(ldvalrsrc->screen));
		sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",ldvalrsrc->module,ldvalrsrc->screen);
		WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
		prterr("Error %s.",e);
		if(e!=NULL) Rfree(e);
	}

}
void loadgrouper(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;
	char *e=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG loadgrouper Load Grouper into Field [%s] on Screen [%s] [%s].",fieldname,r->module,r->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD GROUPER SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=APPlibNEW();
	for(x=0;x<NE(OpenRDA_grouper);++x) addAPPlib(loadlist,OpenRDA_grouper[x]);
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,screenhelp,NULL);
/*
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
*/
	if(ADVmakescrn(ldvalrsrc,TRUE))
	{
		e=Rmalloc(130+RDAstrlen(ldvalrsrc->module)+RDAstrlen(ldvalrsrc->screen));
		sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",ldvalrsrc->module,ldvalrsrc->screen);
		WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
		prterr("Error %s.",e);
		if(e!=NULL) Rfree(e);
	}
}
void loadcontrol(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;
	char *e=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG loadcontrol Load Control Function into Field [%s] on Screen [%s] [%s].",fieldname,r->module,r->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD CONTROL SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=APPlibNEW();
	for(x=0;x<NE(OpenRDA_controls);++x) addAPPlib(loadlist,OpenRDA_controls[x]);
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
/*
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
*/
	if(ADVmakescrn(ldvalrsrc,TRUE))
	{
		e=Rmalloc(130+RDAstrlen(ldvalrsrc->module)+RDAstrlen(ldvalrsrc->screen));
		sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",ldvalrsrc->module,ldvalrsrc->screen);
		WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
		prterr("Error %s.",e);
		if(e!=NULL) Rfree(e);
	}
}
void loadglobals(APPlib *a)
{
	addAPPlibNoDuplicates(a,"[ALLOW POST ACTIVITY]");
	addAPPlibNoDuplicates(a,"[ALLOW BEGINNING BALANCES]");
	addAPPlibNoDuplicates(a,"[ALLOW MULTIPLE ENTRIES]");
	addAPPlibNoDuplicates(a,"[ALLOW MULTIPLE ENTRIES]");
	addAPPlibNoDuplicates(a,"[SUBMIT REQUISITIONS]");
	addAPPlibNoDuplicates(a,"[APPROVE REQUISITIONS]");
	addAPPlibNoDuplicates(a,"[ABLE TO APPROVE]");
	addAPPlibNoDuplicates(a,"[PUSHBUTTON STYLE]");
	addAPPlibNoDuplicates(a,"[DOMENU STYLE]");
	addAPPlibNoDuplicates(a,"[APPROVAL LIMIT]");
	addAPPlibNoDuplicates(a,"[SIMPLE SCREENS]");
	addAPPlibNoDuplicates(a,"[DEV_LICENSE]");
	addAPPlibNoDuplicates(a,"[SYSTEM DATE]");
	addAPPlibNoDuplicates(a,"[SYSTEM DATE-YYYY]");
	addAPPlibNoDuplicates(a,"[SYSTEM TIME]");
	addAPPlibNoDuplicates(a,"[SYSTEM TYPE]");
	addAPPlibNoDuplicates(a,"[CURRENT DIRECTORY]");
	addAPPlibNoDuplicates(a,"[CURRENT PROCESS ID]");
	addAPPlibNoDuplicates(a,"[DEFAULT PRINTER]");
	addAPPlibNoDuplicates(a,"[LOGIN IDENTIFICATION]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION NAME]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION ADDRESS #1]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION ADDRESS #2]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION CITY]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION STATE]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION ZIP]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION PHONE]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION FAX]");
	addAPPlibNoDuplicates(a,"[OPENRDA SETUP]");
	addAPPlibNoDuplicates(a,"[VERSION DATE]");
	addAPPlibNoDuplicates(a,"[PROGRAM NAME]");
	addAPPlibNoDuplicates(a,"[SOFTWARE TYPE]");
	addAPPlibNoDuplicates(a,"[UTILITIES SETUP]");
	addAPPlibNoDuplicates(a,"[ARCHIVE SETUP]");
	addAPPlibNoDuplicates(a,"[FINMGT SETUP]");
	addAPPlibNoDuplicates(a,"[BNKREC SETUP]");
	addAPPlibNoDuplicates(a,"[PRSNNL SETUP]");
	addAPPlibNoDuplicates(a,"[IIDMGT SETUP]");
	addAPPlibNoDuplicates(a,"[APPMGT SETUP]");
	addAPPlibNoDuplicates(a,"[POSTRK SETUP]");
	addAPPlibNoDuplicates(a,"[PAYROLL SETUP]");
	addAPPlibNoDuplicates(a,"[SUBMGT SETUP]");
	addAPPlibNoDuplicates(a,"[LVEMGT SETUP]");
	addAPPlibNoDuplicates(a,"[INVCTL SETUP]");
	addAPPlibNoDuplicates(a,"[LVSIMP SETUP]");
	addAPPlibNoDuplicates(a,"[RLVEMGT SETUP]");
	addAPPlibNoDuplicates(a,"[CSHDWR SETUP]");
	addAPPlibNoDuplicates(a,"[RREQMGT SETUP]");
	addAPPlibNoDuplicates(a,"[BLDPRMT SETUP]");
	addAPPlibNoDuplicates(a,"[CITWEB SETUP]");
	addAPPlibNoDuplicates(a,"[VIOLATION SETUP]");
	addAPPlibNoDuplicates(a,"[PROPERTY SETUP]");
	addAPPlibNoDuplicates(a,"[UTLBLL SETUP]");
	addAPPlibNoDuplicates(a,"[EMPABS SETUP]");
	addAPPlibNoDuplicates(a,"[BFTMGT SETUP]");
	addAPPlibNoDuplicates(a,"[PAYENC SETUP]");
	addAPPlibNoDuplicates(a,"[PURORD SETUP]");
	addAPPlibNoDuplicates(a,"[VENPMT SETUP]");
	addAPPlibNoDuplicates(a,"[CATALOGUE SETUP]");
	addAPPlibNoDuplicates(a,"[REQMGT SETUP]");
	addAPPlibNoDuplicates(a,"[FIXASS SETUP]");
	addAPPlibNoDuplicates(a,"[INVCTL SETUP]");
	addAPPlibNoDuplicates(a,"[OCCTAX SETUP]");
	addAPPlibNoDuplicates(a,"[BUDPREP SETUP]");
	addAPPlibNoDuplicates(a,"[MISCBILL SETUP]");
	addAPPlibNoDuplicates(a,"[TAXCLT SETUP]");
	addAPPlibNoDuplicates(a,"[RLSTMGT SETUP]");
	addAPPlibNoDuplicates(a,"[PPTMGT SETUP]");
	addAPPlibNoDuplicates(a,"[DMVREG SETUP]");
	addAPPlibNoDuplicates(a,"[STUDFIN SETUP]");
	addAPPlibNoDuplicates(a,"[VEHMTN SETUP]");
	addAPPlibNoDuplicates(a,"[WRKORD SETUP]");
	addAPPlibNoDuplicates(a,"[TOOLS SETUP]");
	addAPPlibNoDuplicates(a,"[TRANSACTIONS SETUP]");
	addAPPlibNoDuplicates(a,"[EXTENSIONS SETUP]");
	addAPPlibNoDuplicates(a,"[SIMPLE MENU]");
	addAPPlibNoDuplicates(a,"[ORGANIZATION TYPE]");
	addAPPlibNoDuplicates(a,"[SHOW NEW IN DEFINE LIST]");
	addAPPlibNoDuplicates(a,"[NO USERS]");
	addAPPlibNoDuplicates(a,"[USE END DATE]");
	addAPPlibNoDuplicates(a,"[END DATE]");
	addAPPlibNoDuplicates(a,"[PROGRAM DIRECTORY]");
	addAPPlibNoDuplicates(a,"[USERS DEPARTMENT]");
	addAPPlibNoDuplicates(a,"[USERS NAME]");
	addAPPlibNoDuplicates(a,"[USERS SITE IDENTIFICATION]");
	addAPPlibNoDuplicates(a,"[USERS LOCATION IDENTIFICATION]");
	addAPPlibNoDuplicates(a,"[USERS SUPERVISOR]");
	addAPPlibNoDuplicates(a,"[DATA DIRECTORY]");
	addAPPlibNoDuplicates(a,"[USER INTERFACE TYPE]");
	addAPPlibNoDuplicates(a,"[USE BROWSE LIST LABEL]");
	addAPPlibNoDuplicates(a,"[USERS SCAN DIRECTORY]");
	addAPPlibNoDuplicates(a,"[USERS PHONE]");
	addAPPlibNoDuplicates(a,"[USERS PHONE EXTENSION]");
	addAPPlibNoDuplicates(a,"[USERS EMAIL]");
	addAPPlibNoDuplicates(a,"[USERS WEB LOGIN]");
	addAPPlibNoDuplicates(a,"[USERS WEB PASSWORD]");
	addAPPlibNoDuplicates(a,"[ADMINISTRATIVE EDITS]");
	addAPPlibNoDuplicates(a,"[JOURNAL ENTRY APPROVAL]");
	addAPPlibNoDuplicates(a,"[VOUCHER APPROVAL]");
	addAPPlibNoDuplicates(a,"[WORKSTATION IP ADDRESS]");
	addAPPlibNoDuplicates(a,"[WORKSTATION VALIDATED]");
	addAPPlibNoDuplicates(a,"[WORKSTATION LOGIN]");
	addAPPlibNoDuplicates(a,"[WORKSTATION PASSWORD]");
	addAPPlibNoDuplicates(a,"[WORKSTATION DOCUMENTS]");
#ifndef WIN32
	addAPPlibNoDuplicates(a,"[UUID GENERATE]");
#endif
	if(XPERT_SETUP->FINMGT==TRUE)
	{
		addAPPlibNoDuplicates(a,"[STARTING FISCAL MONTH]");
		addAPPlibNoDuplicates(a,"[FINMGT BUDGET TYPE]");
		if(XPERT_SETUP->software_type<2)
		{
			addAPPlibNoDuplicates(a,"[ACCOUNT MANAGER]");
			addAPPlibNoDuplicates(a,"[ACCOUNT GRANT IDENTIFICATION]");
			addAPPlibNoDuplicates(a,"[ACCOUNT REFERENCE TYPE]");
			addAPPlibNoDuplicates(a,"[CURRENT SUBSIDIARY ACCOUNT CODE]");
		}
	}
	if(XPERT_SETUP->PAYROLL==TRUE)
	{
		addAPPlibNoDuplicates(a,"[PAYROLL MANAGE EARNINGS]");
		addAPPlibNoDuplicates(a,"[PAY BALANCE OF CONTRACT]");
		addAPPlibNoDuplicates(a,"[USE PAYROLL FUND]");
		addAPPlibNoDuplicates(a,"[PAYSEL AUDIT]");
		addAPPlibNoDuplicates(a,"[PAYNET AUDIT]");
		addAPPlibNoDuplicates(a,"[PAYMTS AUDIT]");
		addAPPlibNoDuplicates(a,"[SUBSEL AUDIT]");
		addAPPlibNoDuplicates(a,"[DIRECT DEPOSIT TYPE]");
		addAPPlibNoDuplicates(a,"[EIN NUMBER]");
		addAPPlibNoDuplicates(a,"[PAYJAMS ROLLOVER]");
/* These should have been like this for consistancy and making generic setup easier */
/* It would also group them together by module */
/*
		addAPPlibNoDuplicates(a,"[PAYROLL PAY BALANCE OF CONTRACT]");
		addAPPlibNoDuplicates(a,"[PAYROLL USE FUND]");
		addAPPlibNoDuplicates(a,"[PAYROLL PAYSEL AUDIT]");
		addAPPlibNoDuplicates(a,"[PAYROLL PAYNET AUDIT]");
		addAPPlibNoDuplicates(a,"[PAYROLL PAYMTS AUDIT]");
		addAPPlibNoDuplicates(a,"[PAYROLL SUBSEL AUDIT]");
		addAPPlibNoDuplicates(a,"[PAYROLL DIRECT DEPOSIT TYPE]");
		addAPPlibNoDuplicates(a,"[PAYROLL EIN NUMBER]");
		addAPPlibNoDuplicates(a,"[PAYROLL JOB ACCOUNT ROLLOVER]");
*/
	}
	if(XPERT_SETUP->VENPMT==TRUE)
	{
		addAPPlibNoDuplicates(a,"[VENPMT EIN NUMBER]");
		addAPPlibNoDuplicates(a,"[USE VENPMT CLEARING FUND]");
		addAPPlibNoDuplicates(a,"[USE VENPMT FISCAL SUMMARY]");
		addAPPlibNoDuplicates(a,"[USE VENPMT FISCAL MONTHS]");
/* These should have been like this for consistancy and making generic setup easier */
/* It would also group them together by module */
/*
		addAPPlibNoDuplicates(a,"[VENPMT USE CLEARING FUND]");
		addAPPlibNoDuplicates(a,"[VENPMT USE FISCAL SUMMARY]");
		addAPPlibNoDuplicates(a,"[VENPMT USE FISCAL MONTHS]");
*/
		addAPPlibNoDuplicates(a,"[VENPMT SETUP USE TAXES]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP ADMIN FEE]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP USE TAX1]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP SEPERATE TAX1 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX1 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX1 ADDRESS]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP TAX1 RATE]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP USE TAX2]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP SEPERATE TAX2 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX2 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX2 ADDRESS]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP TAX2 RATE]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP USE TAX3]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP SEPERATE TAX3 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX3 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX3 ADDRESS]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP TAX3 RATE]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP USE TAX4]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP SEPERATE TAX4 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX4 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX4 ADDRESS]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP TAX4 RATE]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP USE TAX5]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP SEPERATE TAX5 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX5 VENDOR]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP DEFAULT TAX5 ADDRESS]");
		addAPPlibNoDuplicates(a,"[VENPMT SETUP TAX5 RATE]");
	}
	if(XPERT_SETUP->PURORD==TRUE)
	{
		addAPPlibNoDuplicates(a,"[USE PURORD FISCAL MONTHS]");
/* These should have been like this for consistancy and making generic setup easier */
/* It would also group them together by module */
/*
		addAPPlibNoDuplicates(a,"[PURORD USE FISCAL MONTHS]");
*/
	}
	if(XPERT_SETUP->APPMGT==TRUE)
	{
		addAPPlibNoDuplicates(a,"[USE PERSONNEL INTERVIEW]");
		addAPPlibNoDuplicates(a,"[USE ADMINISTRATIVE INTERVIEW]");
/* These should have been like this for consistancy and making generic setup easier */
/* It would also group them together by module */
/*
		addAPPlibNoDuplicates(a,"[APPMGT USE PERSONNEL INTERVIEW]");
		addAPPlibNoDuplicates(a,"[APPMGT USE ADMINISTRATIVE INTERVIEW]");
*/
	}
	if(XPERT_SETUP->RLSTMGT==TRUE)
	{
		addAPPlibNoDuplicates(a,"[RLSTMGT COMMISSIONER]");
		addAPPlibNoDuplicates(a,"[RLSTMGT COMMISSIONER PHONE]");
		addAPPlibNoDuplicates(a,"[RLSTMGT COMMISSIONER ADDRESS 1]");
		addAPPlibNoDuplicates(a,"[RLSTMGT COMMISSIONER ADDRESS 2]");
		addAPPlibNoDuplicates(a,"[RLSTMGT COMMISSIONER CITY]");
		addAPPlibNoDuplicates(a,"[RLSTMGT COMMISSIONER STATE]");
		addAPPlibNoDuplicates(a,"[RLSTMGT COMMISSIONER ZIP]");
		addAPPlibNoDuplicates(a,"[RLSTMGT TREASURER]");
		addAPPlibNoDuplicates(a,"[RLSTMGT TREASURER PHONE]");
		addAPPlibNoDuplicates(a,"[RLSTMGT TREASURER ADDRESS 1]");
		addAPPlibNoDuplicates(a,"[RLSTMGT TREASURER ADDRESS 2]");
		addAPPlibNoDuplicates(a,"[RLSTMGT TREASURER CITY]");
		addAPPlibNoDuplicates(a,"[RLSTMGT TREASURER STATE]");
		addAPPlibNoDuplicates(a,"[RLSTMGT TREASURER ZIP]");
	}
	if(XPERT_SETUP->CSHDWR==TRUE)
	{
		addAPPlibNoDuplicates(a,"[CSHUSER BLDPRMT]");
		addAPPlibNoDuplicates(a,"[CSHUSER BLDPRMT DEPOSIT]");
		addAPPlibNoDuplicates(a,"[CSHUSER BLDPRMT DISTRIBUTION]");
		addAPPlibNoDuplicates(a,"[CSHUSER FINMGT]");
		addAPPlibNoDuplicates(a,"[CSHUSER FINMGT DEPOSIT]");
		addAPPlibNoDuplicates(a,"[CSHUSER FINMGT DISTRIBUTION]");
		addAPPlibNoDuplicates(a,"[CSHUSER MISCBILL]");
		addAPPlibNoDuplicates(a,"[CSHUSER MISCBILL DEPOSIT]");
		addAPPlibNoDuplicates(a,"[CSHUSER MISCBILL DISTRIBUTION]");
		addAPPlibNoDuplicates(a,"[CSHUSER OCCTAX]");
		addAPPlibNoDuplicates(a,"[CSHUSER OCCTAX DEPOSIT]");
		addAPPlibNoDuplicates(a,"[CSHUSER OCCTAX DISTRIBUTION]");
		addAPPlibNoDuplicates(a,"[CSHUSER PROPERTY]");
		addAPPlibNoDuplicates(a,"[CSHUSER PROPERTY DEPOSIT]");
		addAPPlibNoDuplicates(a,"[CSHUSER PROPERTY DISTRIBUTION]");
		addAPPlibNoDuplicates(a,"[CSHUSER RLSTMGT]");
		addAPPlibNoDuplicates(a,"[CSHUSER RLSTMGT DEPOSIT]");
		addAPPlibNoDuplicates(a,"[CSHUSER RLSTMGT DISTRIBUTION]");
		addAPPlibNoDuplicates(a,"[CSHUSER UTLBLL]");
		addAPPlibNoDuplicates(a,"[CSHUSER UTLBLL DEPOSIT]");
		addAPPlibNoDuplicates(a,"[CSHUSER UTLBLL DISTRIBUTION]");
		addAPPlibNoDuplicates(a,"[CSHUSER POST ACTIVITY]");
	}
	AddRDAGenericSetup2APPlib(a);
}
void xAddRDAGenericSetup2APPlib(APPlib *a,int line,char *file)
{
	int x,y;
	char *tmp=NULL,*dirx=NULL,*libx=NULL,*tmpx=NULL;
	APPlib *modsavl=NULL,*stpvar=NULL;
	
	modsavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.GSV",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modsavl,tmp);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(modsavl->numlibs>0)
	{
		for(x=0;x<modsavl->numlibs;++x)
		{
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl->libs[x])+10);
			tmpx=adddashes(modsavl->libs[x]);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,tmpx);
#endif
			if(tmpx!=NULL) Rfree(tmpx);
			stpvar=getlibnames(libx,FALSE);
			if(stpvar!=NULL)
			{
				for(y=0;y<stpvar->numlibs;++y)
				{
					tmp=Rmalloc(RDAstrlen(modsavl->libs[x])+RDAstrlen(stpvar->libs[y])+32);
					sprintf(tmp,"GENERIC_SETUP_VARIABLE(\"[%s][%s]\")",
						modsavl->libs[x],stpvar->libs[y]);
					addAPPlib(a,tmp);
					if(tmp!=NULL) Rfree(tmp);
				}
			}
			if(stpvar!=NULL) freeapplib(stpvar);
			if(libx!=NULL) Rfree(libx);
		}
	}
	if(dirx!=NULL) Rfree(dirx);
}
RDALoadValue *xRDALoadValueNEW(char *module,char *name,int line,char *file)
{
	RDALoadValue *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDALoadValueNEW for Module [%s] LoadValue [%s] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDALoadValue));
	if(!isEMPTY(module)) tmp->module=stralloc(module);
		else tmp->module=NULL;
	if(!isEMPTY(name)) tmp->name=stralloc(name);
		else tmp->name=NULL;
	tmp->type=0;
	tmp->numincvirs=0;
	tmp->incvir=NULL;

	tmp->conname=NULL;
	tmp->dlist=NULL;
	tmp->desc=NULL;
	tmp->mlist=NULL;
	tmp->SubFunc=NULL;
	tmp->args=NULL;
	return(tmp);
}
short xgetRDALoadValuebin(char *libname,RDALoadValue *RLV,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	int z=0,y=0;
	DFincvir *i=NULL;
	short version=(-1);
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getRDALoadValuebin Attempting to Read LoadValue Binary [%s] from LoadValue Library [%s] at line [%d] program [%s].",RLV->name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,RLV->name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("Error Attempting to Read LoadValue Binary [%s] from LoadValue Library [%s] Failed at line [%d] program [%s].",RLV->name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1700) && x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for LoadValue [%s] in LoadValue Binary Library [%s] at line [%d] program [%s].",RLV->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1700)) version=2;
	else if(x==(-1701)) version=1;
	else version=0;
	test=BINgetshort(bin);
	if(test) RLV->module=BINgetstring(bin);
		else RLV->module=NULL;
	test=BINgetshort(bin);
	if(test) RLV->name=BINgetstring(bin);
		else RLV->name=NULL;
	test=BINgetshort(bin);
	if(test) RLV->desc=BINgetstring(bin);
		else RLV->desc=NULL;
	RLV->type=BINgetint(bin);
	RLV->numincvirs=BINgetint(bin);
	if(RLV->numincvirs>0)
	{
		RLV->incvir=Rmalloc(RLV->numincvirs*sizeof(DFincvir));
		for(z=0,i=RLV->incvir;z<RLV->numincvirs;++z,++i)
		{
			test=BINgetshort(bin);
			if(test) i->module=BINgetstring(bin);
				else i->module=NULL;
			test=BINgetshort(bin);
			if(test) i->file=BINgetstring(bin);
				else i->file=NULL;
			test=BINgetshort(bin);
			if(test) i->keyname=BINgetstring(bin);
				else i->keyname=NULL;
		}
	} else {
		BINaddint(bin,0);
	}
	RLV->dlist=NULL;
	test=BINgetshort(bin);
	if(test) RLV->conname=BINgetstring(bin);
		else RLV->conname=NULL;
	if(version>1)
	{
		y=BINgetint(bin);
		if(y>0)
		{
			RLV->mlist=APPlibNEW();
			for(z=0;z<y;++z)
			{
				test=BINgetshort(bin);
				if(test) temp=BINgetstring(bin);
					else temp=NULL;
				addAPPlib(RLV->mlist,temp);
				if(temp!=NULL) Rfree(temp);
			}
		} else {
			RLV->mlist=NULL;
		}
	} else {
		RLV->mlist=NULL;
	}
	BINfree(bin);
	return(0);
}
short xwriteRDALoadValuebin(char *libname,RDALoadValue *RLV,int line,
	char *file)
{
	BIN *bin;
	int x;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG writeRDALoadValuebin Writing LoadValue [%s] into LoadValue Binary Library [%s] at line [%d] program [%s].",(RLV->name!=NULL ? RLV->name:""),libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1700);	/*type identifier*/
	if(RLV->module!=NULL) 
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,RLV->module);
	} else BINaddshort(bin,FALSE);
	if(RLV->name!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,RLV->name);
	} else BINaddshort(bin,FALSE);
	if(RLV->desc!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,RLV->desc);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,RLV->type);
	if(RLV->incvir!=NULL && RLV->numincvirs>0)
	{
		BINaddint(bin,RLV->numincvirs);
		for(x=0,i=RLV->incvir;x<RLV->numincvirs;++x,++i)
		{
			if(!isEMPTY(i->module))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->module);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(i->file))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->file);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(i->keyname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->keyname);
			} else BINaddshort(bin,FALSE);
		}
	} else {
		BINaddint(bin,0);
	}
	if(!isEMPTY(RLV->conname))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,RLV->conname);
	} else BINaddshort(bin,FALSE);
	if(RLV->mlist!=NULL)
	{
		BINaddint(bin,RLV->mlist->numlibs);
		if(RLV->mlist->numlibs>0)
		{
			for(x=0;x<RLV->mlist->numlibs;++x)
			{
				if(!isEMPTY(RLV->mlist->libs[x]))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,RLV->mlist->libs[x]);
				} else BINaddshort(bin,FALSE);
			}
		}
	} else BINaddint(bin,0);
	if(writelibbin(libname,bin,RLV->name))
	{
		prterr("Error Can't write LoadValue binary [%s] into LoadValue library [%s] at line [%d] program [%s].",(RLV->name!=NULL ? RLV->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
char *xgetRDALoadValuedesc(char *libname,char *name,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getRDALoadValuedesc Attempting to Read LoadValue description for [%s] from LoadValue Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("Error Attempting to Read LoadValue Binary [%s] from LoadValue Library [%s] Failed at line [%d] program [%s].",name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(NULL);
	}
	x=BINgetshort(bin);
	if(x!=(-1700) && x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for LoadValue [%s] in LoadValue Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
		BINfree(bin);
		return(NULL);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
	} else temp=NULL;
	BINfree(bin);
	return(temp);
}
static void SEERDALoadValue_header(RDA_PFILE *fp,RDALoadValue *RLV,char print_head,int *pages,int *numpagelines)
{
	char *date=NULL,*timex=NULL;

	if(print_head)
	{
		if(*pages==1 && *numpagelines==1)
		{
			date=GETCURRENTDATE10();
			timex=GETCURRENTTIME();
			RDA_fprintf(fp,"%*sXPERT LOAD VALUE LIST DEFINITION       Page: %*d\r\n",
				24,"",10,*pages);
			RDA_fprintf(fp,"%*sModule [%s]  Name [%s]%*sDate: %s\r\n",
				((80-(19+RDAstrlen(RLV->module)+RDAstrlen(RLV->name)))/2),
				"",
				RLV->module,RLV->name,(((80-(18+RDAstrlen(RLV->module)+RDAstrlen(RLV->name)))/2)-16),
				"",date);
			RDA_fprintf(fp,"%*s Executed By: %s%*sTime:   %s\r\n",
				((80-(16+RDAstrlen(USERLOGIN)))/2),"",
				USERLOGIN,
				(((80-(16+RDAstrlen(USERLOGIN)))/2)-18),"",
				timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
			*numpagelines=*numpagelines+3;
		}
	}
	RDA_fprintf(fp,"\r\n"); *numpagelines=*numpagelines+1;
	if(print_head)
	{
		if(*numpagelines>=60)
		{
			RDA_fprintf(fp,"\f"); *numpagelines=1; *pages=*pages+1;
			RDA_fprintf(fp,"%*sXPERT LOAD VALUE LIST DEFINITION       Page: %*d\r\n",
				24,"",10,*pages);
			RDA_fprintf(fp,"%*sModule [%s]  Name [%s]\r\n",
				((79-(18+RDAstrlen(RLV->module)+
				RDAstrlen(RLV->name)))/2),"",
				RLV->module,RLV->name);
			RDA_fprintf(fp,"%*s Executed By: %s\r\n\r\n",
				((80-(14+RDAstrlen(USERLOGIN)))/2),"",USERLOGIN);
				*numpagelines=*numpagelines+5;
		}
	}
}
static void fixRLVout(RDA_PFILE *fp,char *data,RDALoadValue *RLV,short print_head,int *pages,int *numpagelines,unsigned line_length,char *initstring)
{
	unsigned y=0,x=0,length=0,initoffset=0;
	char *temp=NULL,*temp1=NULL,*temp2=NULL;

	if(!isEMPTY(data))
	{
		initoffset=RDAstrlen(initstring);
		temp=strchr(data,'\n');
		temp1=strchr(data,'\r');
		if(temp==NULL && temp1==NULL)
		{
			length=RDAstrlen(data);
		} else if(temp==NULL)
		{
			length=temp1-data;
		} else if(temp1==NULL)
		{
			length=temp-data;
		} else if(temp<temp1)
		{
			length=temp-data;
		} else {
			length=temp1-data;
		}
		for(x=0,y=0;x<length;++x,++y)
		{
			if(data[x]=='\t')
			{
				if(y+9>(line_length-initoffset))
				{
					break;
				}  
				y+=7;
			}
			if(y+2>(line_length-initoffset))
			{
				break;
			}
		}
		y=x;
		temp2=(data+x+1);
		if(temp2[0]!=0)
		{
			if(temp2[0]!=' ')
			{
				for(y=x;y>0;--y)
				{
					if(data[y]==' ') break;
				}
			}
			if(y<1) y=x;
		}
		temp2=Rmalloc(initoffset+1);
		sprintf(temp2,"%*s",initoffset,"");
		if(y<length)
		{
			RDA_fprintf(fp,"%s%.*s\r\n",initstring,y+1,data);
			fixRLVout(fp,data+y+1,RLV,print_head,pages,numpagelines,line_length,temp2);
		} else {
			RDA_fprintf(fp,"%s%.*s",initstring,length,data);
			if(temp!=NULL || temp1!=NULL)
			{
				fixRLVout(fp,data+length+1,RLV,print_head,pages,numpagelines,line_length,temp2);
			}
		}
		if(temp2!=NULL) Rfree(temp2);
	}
}
void xSEERDALoadValue_detail(RDALoadValue *RLV,RDA_PFILE *fp,char detail,int *pages,int *numpagelines,int line,char *file)
{
	DFincvir *i=NULL;
	unsigned line_length=79;
	char *temp_head=NULL,*temp=NULL;
	int y;

	if(fp!=NULL)
	{
	if(RLV!=NULL)
	{
		SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
		temp_head=stralloc("Description: [");
		temp=Rmalloc(3+(RLV->desc==NULL?3:RDAstrlen(RLV->desc)));
		sprintf(temp,"%s]",(RLV->desc==NULL?"":RLV->desc));
		if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
		{
			fixRLVout(fp,temp,RLV,detail,pages,numpagelines,line_length,temp_head);
			SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
		} else {
			RDA_fprintf(fp,"%s%s",temp_head,temp);
			SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
		}
		if(temp_head!=NULL) Rfree(temp_head);
		if(temp!=NULL) Rfree(temp);
		if(RLV->type==0)
		{
			temp=stralloc("Not Used");
		} else if(RLV->type==1)
		{
			temp=stralloc("Not Used");
		} else {
			temp=stralloc("*** BAD VALUE! ***");
		}
		RDA_fprintf(fp,"Load Value Type: [%d]=[%s]",RLV->type,temp);
		if(temp!=NULL) Rfree(temp);
		SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
		RDA_fprintf(fp,"   Connect Name: [%s]",RLV->conname);
		SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
		if(RLV->incvir!=NULL)
		{
			for(y=0,i=RLV->incvir;y<RLV->numincvirs;++y,++i)
			{
				RDA_fprintf(fp,"     Module [%s] File [%s] Keyname [%s] Fileno [%d]",
					(i->module!=NULL?i->module:""),
					(i->file!=NULL?i->file:""),
					(i->keyname!=NULL?i->keyname:""),
					i->fileno);
				SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
			}
		}
		if(temp!=NULL) Rfree(temp);
		if(RLV->mlist!=NULL)
		{
			RDA_fprintf(fp,"   Manual Values:");
			SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
			for(y=0;y<RLV->mlist->numlibs;++y)
			{
				RDA_fprintf(fp,"     Element [%s]",
					(RLV->mlist->libs[y]!=NULL?RLV->mlist->libs[y]:""));
				SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
			}
		}
		SEERDALoadValue_header(fp,RLV,detail,pages,numpagelines);
	} else {
		prterr("ERROR: Null Load Value.");
	}
	} else {
		prterr("ERROR: No file pointer.");
	}
}
static void printRDALoadValue(RDArsrc *prsrc,RDALoadValue *RLV)
{
	int pages=1,numpagelines=1;
	char *outdevice=NULL;
	RDA_PFILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			if(outdevice!=NULL) Rfree(outdevice);
			SEERDALoadValue_detail(RLV,fp,TRUE,&pages,&numpagelines);
			RDA_pclose(fp);
		} else {
			ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
                        prterr("Error PrintRDALoadValue popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
                        if(outdevice!=NULL) Rfree(outdevice);
                }
        } else {
		ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
        }
}
static void quit_printRDALoadValue(RDArsrc *prsrc)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG quit_printRDALoadValue Qruitting Print RDA Load Value Definitions");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	free_rsrc(prsrc);
}
static void GetDevicePrintRDALoadValue(RDALoadValue *RLV)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDALoadValue,RLV);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDALoadValue,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
void xSEERDALoadValue(RDALoadValue *RLV,RDA_PFILE *fp,char detail,int line,char *file)
{
	int pages=1,numpagelines=1;

	if(RLV==NULL)
	{
		prterr("Error SEERDALoadValue has no Load Value Definition, i.e. the structure is NULL prior to line [%d] program [%s].",line,file);
		return;
	}
	if(detail)
	{
		GetDevicePrintRDALoadValue(RLV);
	} else {
		prterr("DIAG SEERDALoadValue Show Structure for RDALoadValue [%s] at line [%d] program [%s].",RLV->name,line,file);
		SEERDALoadValue_detail(RLV,fp,FALSE,&pages,&numpagelines);
	}
}
void xRDALoadValueDFincvir(RDALoadValue *l,char *mod,char *fil,char *key,
	short fileno,int line,char *file)
{
	DFincvir *i=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG LoadValueDFincvir at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(l!=NULL)
	{
		if(l->incvir!=NULL)
		{
			l->incvir=Rrealloc(l->incvir,(l->numincvirs+1)*sizeof(DFincvir));
		} else {
			l->incvir=Rmalloc(sizeof(DFincvir));
			l->numincvirs=0;
		}
		i=l->incvir+l->numincvirs;
		i->module=stralloc(mod);
		i->file=stralloc(fil);
		i->keyname=stralloc(key);
		i->fileno=fileno;
		i->v=NULL;
		++l->numincvirs;
	} else {
		prterr("Error RDALoadValue is NULL at line [%d] program [%s].",line,file);
	}
}
