/* VENPMT's - Make (VENDOR YEAR DEFINE LIST) Screen Default */
#include <stdio.h>
#include <mkmsc.h>
#include <mkdef.h>

char *module="VENPMT";
extern APPlib *errorlist;
static void DEF_VENDOR_YEAR_DEFINE_LIST(void);

static void DEF_VENDOR_YEAR_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("VENPMT","VENDOR YEAR DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus="DELETEFLAG POSITION";
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,1);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"CALENDAR YEAR POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"CALENDAR YEAR LENGTH",TRUE,7,4);
		addDefaultFieldSHORT(def,"JANUARY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JANUARY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JANUARY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JANUARY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"FEBRUARY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MARCH 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"APRIL 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"MAY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JUNE 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"JULY 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"AUGUST 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"SEPTEMBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"OCTOBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOVEMBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER GROSS AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER GROSS AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER 1099 AMOUNT POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DECEMBER 1099 AMOUNT LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"NUMBER OF INVOICES ISSUED POSITION",TRUE,7,4);
		addDefaultFieldSHORT(def,"NUMBER OF INVOICES ISSUED LENGTH",TRUE,7,6);
		addDefaultFieldSHORT(def,"VENDOR IDENTIFICATION POSITION",TRUE,7,2);
		addDefaultFieldSHORT(def,"VENDOR IDENTIFICATION LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR GROSS AMOUNT POSITION",TRUE,7,5);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR GROSS AMOUNT LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR 1099 AMOUNT POSITION",TRUE,7,6);
		addDefaultFieldSHORT(def,"ANNUAL CALENDAR 1099 AMOUNT LENGTH",TRUE,7,20);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
		if(writedefaultbin(defdir,def))
		{
/*
			ERRORDIALOG("Cannot Save Screen Defaults","This screen default may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+6+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [VENPMT] Screen [VENDOR YEAR DEFINE LIST], Can Not Save Screen Defaults!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(def!=NULL) FreeRDAdefault(def);
	}
}
