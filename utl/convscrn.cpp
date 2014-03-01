/* convscrn.c - Converts Screen Items */
#ifndef WIN32
#define __NAM__ "convscrn.lnx"
#endif
#ifdef WIN32
#define __NAM__ "convscrn.exe"
#endif
#include <app.hpp>
#include <cnvscn.hpp>
/*ADDLIB cscn */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
static char **wdgtlist=NULL;
static APPlib *exactlist=NULL,*rsrclistfrom=NULL,*rsrclistto=NULL;
static APPlib *scr_libs=NULL,*dir_libs=NULL,*set_whichs=NULL;
static void changewidgetlist(RDArsrc *,short *);
static void changersctype(RDArsrc *,short *);

static void convscrn(RDArsrc *parent)
{
	RDAcnvscn *csn=NULL;
	int selectedlib=0,x=0;

	csn=RDAcnvscnNEW();
	readallwidgets(parent);
	FINDRSCGETCHAR(parent,"RANGE ON SCREEN",&csn->rng_scn);
	FINDRSCGETCHAR(parent,"PATTERN MATCH SCREEN",&csn->pat_scn);
	FINDRSCGETSTRING(parent,"SCREEN PATTERN",&csn->scn_pattern);
	FINDRSCGETINT(parent,"SET WHICH",&csn->set_which);
	FINDRSCGETSTRING(parent,"FROM SCREEN",&csn->from_scn);
	FINDRSCGETSTRING(parent,"TO SCREEN",&csn->to_scn);
	FINDRSCGETCHAR(parent,"ALL LIBRARIES",&csn->all_libs);
	FINDRSCGETCHAR(parent,"ALL DIRECTORIES",&csn->all_dirs);
	if(FINDRSCGETINT(parent,"MATCH TYPE",&csn->match_type)) return;
	if(FINDRSCGETINT(parent,"LIBRARY LIST",&selectedlib)) return;
	FINDRSCGETSTRING(parent,"DIRECTORY",&csn->directory);

	FINDRSCGETSTRING(parent,"FROM WIDGET NAME",&csn->from.resource_name);
	FINDRSCGETSTRING(parent,"FROM WIDGET LABEL",&csn->from.label);
	FINDRSCGETSTRING(parent,"FROM WIDGET PIXMAP",&csn->from.pixmap);
	FINDRSCGETSTRING(parent,"FROM YWINDOW LABEL",&csn->from.XHTML_Label);
	FINDRSCGETSTRING(parent,"FROM EXPRESSION",&csn->from.expression);
	FINDRSCGETSTRING(parent,"FROM EDITABLE EXPRESSION",&csn->from.editable_expression);
	FINDRSCGETSTRING(parent,"FROM SENSITIVE EXPRESSION",&csn->from.sensitive_expression);
	FINDRSCGETSTRING(parent,"FROM TRANSVERSAL EXPRESSION",&csn->from.transversal_expression);
	x=0;
	FINDRSCGETINT(parent,"FROM WIDGET TYPES",&x);
	csn->from.type=x;
	FINDRSCGETINT(parent,"FROM RESOURCE TYPES",&csn->from.rtype);
	FINDRSCGETSHORT(parent,"FROM NUMBER ROWS",&csn->from.rows);
	FINDRSCGETSHORT(parent,"FROM NUMBER COLUMNS",&csn->from.cols);
	FINDRSCGETSTRING(parent,"TO WIDGET NAME",&csn->to.resource_name);
	FINDRSCGETSTRING(parent,"TO WIDGET LABEL",&csn->to.label);
	FINDRSCGETSTRING(parent,"TO WIDGET PIXMAP",&csn->to.pixmap);
	FINDRSCGETSTRING(parent,"TO YWINDOW LABEL",&csn->to.XHTML_Label);
	x=0;
	FINDRSCGETINT(parent,"TO WIDGET TYPES",&x);
	csn->to.type=x;
	FINDRSCGETINT(parent,"TO RESOURCE TYPES",&csn->to.rtype);
	FINDRSCGETSHORT(parent,"TO NUMBER ROWS",&csn->to.rows);
	FINDRSCGETSHORT(parent,"TO NUMBER COLUMNS",&csn->to.cols);
	FINDRSCGETSTRING(parent,"TO EXPRESSION",&csn->to.expression);
	FINDRSCGETSTRING(parent,"TO EDITABLE EXPRESSION",&csn->to.editable_expression);
	FINDRSCGETSTRING(parent,"TO SENSITIVE EXPRESSION",&csn->to.sensitive_expression);
	FINDRSCGETSTRING(parent,"TO TRANSVERSAL EXPRESSION",&csn->to.transversal_expression);
	csn->library=stralloc(scr_libs->libs[selectedlib]);
	Execute_RDAcnvscn(csn);
	FreeRDAcnvscn(csn);
}
static void maketestlist()
{
	short x;

	wdgtlist=Rmalloc(NE(wdgttypes)*sizeof(char *));
	for(x=0;x<NE(wdgttypes);++x) wdgtlist[x]=stralloc(wdgttypes[x]);
}
static void freetestlist()
{
	short x;

	if(wdgtlist!=NULL)
	{
		for(x=0;x<NE(wdgttypes);++x) Rfree(wdgtlist[x]);
		Rfree(wdgtlist);
		wdgtlist=NULL;
	}
}
static void changewidgetlist(RDArsrc *parent,short *which)
{
	int x=0,selected=0,selectedr=0;
	RDAacct *acc=NULL;

	if(*which)
	{
		if(FINDRSCGETINT(parent,"FROM WIDGET TYPES",&selected)) return;
		if(FINDRSCGETINT(parent,"FROM RESOURCE TYPES",&selectedr)) return;
		if(rsrclistfrom!=NULL) freeapplib(rsrclistfrom);
		rsrclistfrom=APPlibNEW();
	} else {
		if(FINDRSCGETINT(parent,"TO WIDGET TYPES",&selected)) return;
		if(FINDRSCGETINT(parent,"TO RESOURCE TYPES",&selectedr)) return;
		if(rsrclistto!=NULL) freeapplib(rsrclistto);
		rsrclistto=APPlibNEW();
	}
	switch(selected)
	{
		case 15: /* custom input fields */
			if(CUSTOM_INPUTS_ACCOUNT!=NULL)
			{
				if(CUSTOM_INPUTS_ACCOUNT->codetype!=NULL)
				{
					for(x=0,acc=CUSTOM_INPUTS_ACCOUNT->codetype;
						x<CUSTOM_INPUTS_ACCOUNT->num;++x,++acc)
					{
						if(*which)
						{
							addAPPlib(rsrclistfrom,acc->name);
						} else {
							addAPPlib(rsrclistto,acc->name);
						}
					}
				}
			}		 
			if(*which)
			{
				if(rsrclistfrom->numlibs<1) 
				{
					addAPPlib(rsrclistfrom,"Not Applicable");
					FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",TRUE);
				}
			} else {
				if(rsrclistto->numlibs<1) 
				{
					addAPPlib(rsrclistto,"Not Applicable");
					FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",TRUE);
				}
			}
			if(*which)
			{
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",TRUE);
			} else {
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",TRUE);
			}
			break;
		case 0: /* standard resource */
			if(*which)
			{
				addAPPlib(rsrclistfrom,"Not Applicable");
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",FALSE);
			} else {
				addAPPlib(rsrclistto,"Not Applicable");
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",FALSE);
			}
			break;
		case 1: /* new line */
		case 2: /* end line */
		case 3: /* new box */
		case 4: /* end box */
		case 12: /* start scrolled window */
		case 13: /* end scrolled window */
			if(*which)
			{
				addAPPlib(rsrclistfrom,"Not Applicable");
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",FALSE);
			} else {
				addAPPlib(rsrclistto,"Not Applicable");
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",FALSE);
			}
			break;
		case 5: /* label */
			if(*which)
			{
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",TRUE);
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",TRUE);
				}
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",TRUE);
				addAPPlib(rsrclistfrom,"Text Label");
				addAPPlib(rsrclistfrom,"Pixmap Label");
			} else {
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",TRUE);
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",TRUE);
				}
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",TRUE);
				addAPPlib(rsrclistto,"Text Label");
				addAPPlib(rsrclistto,"Pixmap Label");
			}
			break;
		case 14: /* OPTIONAL SCREEN */
		case 6: /* button */
			if(*which)
			{
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",TRUE);
				if(selectedr==5)
				{
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",TRUE);
				} else {
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				}
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",TRUE);
				addAPPlib(rsrclistfrom,"Push Button");
				addAPPlib(rsrclistfrom,"Arrow Button Right");
				addAPPlib(rsrclistfrom,"Arrow Button Left");
				addAPPlib(rsrclistfrom,"Arrow Button Up");
				addAPPlib(rsrclistfrom,"Arrow Button Down");
				addAPPlib(rsrclistfrom,"Pixmap");
				addAPPlib(rsrclistfrom,"Find");
				addAPPlib(rsrclistfrom,"Printer");
				addAPPlib(rsrclistfrom,"Redo");
				addAPPlib(rsrclistfrom,"Undo");
				addAPPlib(rsrclistfrom,"FileFind");
				addAPPlib(rsrclistfrom,"Help");
				addAPPlib(rsrclistfrom,"Search");
				addAPPlib(rsrclistfrom,"Edit");
				addAPPlib(rsrclistfrom,"Exit");
				addAPPlib(rsrclistfrom,"Define List");
				addAPPlib(rsrclistfrom,"Add / Create");
				addAPPlib(rsrclistfrom,"Save Defaults");
				addAPPlib(rsrclistfrom,"Reset Defaults");
				addAPPlib(rsrclistfrom,"Cabinet");
				addAPPlib(rsrclistfrom,"Mail Send");
				addAPPlib(rsrclistfrom,"Scanner");
				addAPPlib(rsrclistfrom,"Wizard");
				addAPPlib(rsrclistfrom,"Save");
				addAPPlib(rsrclistfrom,"Fast Save");
				addAPPlib(rsrclistfrom,"Exec");
				addAPPlib(rsrclistfrom,"Html");
				addAPPlib(rsrclistfrom,"Pdf");
				addAPPlib(rsrclistfrom,"Zip");
				addAPPlib(rsrclistfrom,"Display");
				addAPPlib(rsrclistfrom,"Archive");
				addAPPlib(rsrclistfrom,"Folder");
				addAPPlib(rsrclistfrom,"WWW");
				addAPPlib(rsrclistfrom,"Delete/Trash");
				addAPPlib(rsrclistfrom,"Configure");
				addAPPlib(rsrclistfrom,"Look-n-Feel");
				addAPPlib(rsrclistfrom,"View");
				addAPPlib(rsrclistfrom,"View-Detailed");
				addAPPlib(rsrclistfrom,"Main Menu");
				addAPPlib(rsrclistfrom,"Open Source");
				addAPPlib(rsrclistfrom,"GoogleMaps");
				addAPPlib(rsrclistfrom,"Top");
				addAPPlib(rsrclistfrom,"Bottom");

			} else {
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",TRUE);
				if(selectedr==5)
				{
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",TRUE);
				} else {
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				}
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",TRUE);
				addAPPlib(rsrclistto,"Push Button");
				addAPPlib(rsrclistto,"Arrow Button Right");
				addAPPlib(rsrclistto,"Arrow Button Left");
				addAPPlib(rsrclistto,"Arrow Button Up");
				addAPPlib(rsrclistto,"Arrow Button Down");
				addAPPlib(rsrclistto,"Pixmap");
				addAPPlib(rsrclistto,"Find");
				addAPPlib(rsrclistto,"Printer");
				addAPPlib(rsrclistto,"Redo");
				addAPPlib(rsrclistto,"Undo");
				addAPPlib(rsrclistto,"FileFind");
				addAPPlib(rsrclistto,"Help");
				addAPPlib(rsrclistto,"Search");
				addAPPlib(rsrclistto,"Edit");
				addAPPlib(rsrclistto,"Exit");
				addAPPlib(rsrclistto,"Define List");
				addAPPlib(rsrclistto,"Add / Create");
				addAPPlib(rsrclistto,"Save Defaults");
				addAPPlib(rsrclistto,"Reset Defaults");
				addAPPlib(rsrclistto,"Cabinet");
				addAPPlib(rsrclistto,"Mail Send");
				addAPPlib(rsrclistto,"Scanner");
				addAPPlib(rsrclistto,"Wizard");
				addAPPlib(rsrclistto,"Save");
				addAPPlib(rsrclistto,"Fast Save");
				addAPPlib(rsrclistto,"Exec");
				addAPPlib(rsrclistto,"Html");
				addAPPlib(rsrclistto,"Pdf");
				addAPPlib(rsrclistto,"Zip");
				addAPPlib(rsrclistto,"Display");
				addAPPlib(rsrclistto,"Archive");
				addAPPlib(rsrclistto,"Folder");
				addAPPlib(rsrclistto,"WWW");
				addAPPlib(rsrclistto,"Delete/Trash");
				addAPPlib(rsrclistto,"Configure");
				addAPPlib(rsrclistto,"Look-n-Feel");
				addAPPlib(rsrclistto,"View");
				addAPPlib(rsrclistto,"View-Detailed");
				addAPPlib(rsrclistto,"Main Menu");
				addAPPlib(rsrclistto,"Open Source");
				addAPPlib(rsrclistto,"GoogleMaps");
				addAPPlib(rsrclistto,"Top");
				addAPPlib(rsrclistto,"Bottom");
			}
			break;
		case 7: /* scrolled list */
			if(*which)
			{
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",FALSE);
				addAPPlib(rsrclistfrom,"Not Applicable");
			} else {
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",FALSE);
				addAPPlib(rsrclistto,"Not Applicable");
			}
			break;
		case 9: /* toggle button */
			if(*which)
			{
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",TRUE);
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",TRUE);
				}
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",TRUE);
				addAPPlib(rsrclistfrom,"Text Label");
				addAPPlib(rsrclistfrom,"Pixmap Label");
			} else {
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",TRUE);
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",TRUE);
				}
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",TRUE);
				addAPPlib(rsrclistto,"Text Label");
				addAPPlib(rsrclistto,"Pixmap Label");
			}
			break;
		case 8: /* scrolled text */
			if(*which)
			{
				addAPPlib(rsrclistfrom,"WordWrap & No Horizontal SB");
				addAPPlib(rsrclistfrom,"Horizontal SB & No WordWrap");
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",TRUE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",TRUE);
			} else {
				addAPPlib(rsrclistto,"WordWrap & No Horizontal SB");
				addAPPlib(rsrclistto,"Horizontal SB & No WordWrap");
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",TRUE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",TRUE);
			}
			break;
		case 10: /* frame */
			if(*which)
			{
				addAPPlib(rsrclistfrom,"Shadow In");
				addAPPlib(rsrclistfrom,"Shadow Out");
				addAPPlib(rsrclistfrom,"Shadow Etched In");
				addAPPlib(rsrclistfrom,"Shadow Etched Out");
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",TRUE);
			} else {
				addAPPlib(rsrclistto,"Shadow In");
				addAPPlib(rsrclistto,"Shadow Out");
				addAPPlib(rsrclistto,"Shadow Etched In");
				addAPPlib(rsrclistto,"Shadow Etched Out");
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",TRUE);
			}
			break;
		case 11: /* seperator */
			if(*which)
			{
				addAPPlib(rsrclistfrom,"Horizontal Single Line");
				addAPPlib(rsrclistfrom,"Horizontal Double Line");
				addAPPlib(rsrclistfrom,"Horizontal Single Dashed");
				addAPPlib(rsrclistfrom,"Horizontal Double Dashed");
				addAPPlib(rsrclistfrom,"Vertical Single Line");
				addAPPlib(rsrclistfrom,"Vertical Double Line");
				addAPPlib(rsrclistfrom,"Vertical Single Dashed");
				addAPPlib(rsrclistfrom,"Vertical Double Dashed");
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"FROM RESOURCE TYPES",TRUE);
			} else {
				addAPPlib(rsrclistto,"Horizontal Single Line");
				addAPPlib(rsrclistto,"Horizontal Double Line");
				addAPPlib(rsrclistto,"Horizontal Single Dashed");
				addAPPlib(rsrclistto,"Horizontal Double Dashed");
				addAPPlib(rsrclistto,"Vertical Single Line");
				addAPPlib(rsrclistto,"Vertical Double Line");
				addAPPlib(rsrclistto,"Vertical Single Dashed");
				addAPPlib(rsrclistto,"Vertical Double Dashed");
				FINDRSCSETSENSITIVE(parent,"TO WIDGET NAME",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO YWINDOW LABEL",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER ROWS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO NUMBER COLUMNS",FALSE);
				FINDRSCSETSENSITIVE(parent,"TO RESOURCE TYPES",TRUE);
			}
			break;
		default:
			prterr("Error Incorrect Widget Type.");
			break;
	}
	if(*which)
	{
		if(selectedr>=rsrclistfrom->numlibs) selectedr=0;
		if(!FINDRSCLISTAPPlib(parent,"FROM RESOURCE TYPES",selectedr,
			rsrclistfrom))
		{
			updatersrc(parent,"FROM RESOURCE TYPES");
		}
	} else {
		if(selectedr>=rsrclistto->numlibs) selectedr=0;
		if(!FINDRSCLISTAPPlib(parent,"TO RESOURCE TYPES",selectedr,
			rsrclistto))
		{
			updatersrc(parent,"TO RESOURCE TYPES");
		}
	}
}
static void changersctype(RDArsrc *parent,short *which)
{
	int selected=0,selectedr=0;

	if(*which)
	{
		if(FINDRSCGETINT(parent,"FROM WIDGET TYPES",&selected)) return;
		if(FINDRSCGETINT(parent,"FROM RESOURCE TYPES",&selectedr)) return;
	} else {
		if(FINDRSCGETINT(parent,"TO WIDGET TYPES",&selected)) return;
		if(FINDRSCGETINT(parent,"TO RESOURCE TYPES",&selectedr)) return;
	}
	switch(selected)
	{
		case 15: /* custom input fields */
		case 0: /* standard resource */
		case 1: /* new line */
		case 2: /* end line */
		case 3: /* new box */
		case 4: /* end box */
		case 12: /* start scrolled window */
		case 13: /* end scrolled window */
		case 7: /* scrolled list */
		case 8: /* scrolled text */
		case 10: /* frame */
		case 11: /* seperator */
			break;
		case 5: /* label */
			if(*which)
			{
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",TRUE);
				}
			} else {
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",TRUE);
				}
			}
			break;
		case 14: /* OPTIONAL SCREEN */
		case 6: /* button */
			if(*which)
			{
				if(selectedr==5)
				{
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",TRUE);
				} else {
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				}
			} else {
				if(selectedr==5)
				{
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",TRUE);
				} else {
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				}
			}
			break;
		case 9: /* toggle button */
			if(*which)
			{
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"FROM WIDGET PIXMAP",TRUE);
				}
			} else {
				if(selectedr==0)
				{
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",FALSE);
				} else {
					FINDRSCSETSENSITIVE(parent,"TO WIDGET PIXMAP",TRUE);
				}
			}
			break;
		default:
			prterr("Error Incorrect Widget Type.");
			break;
	}
}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int x,selectedx=0;
	char *tmp=NULL,*directory=NULL,*dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&selectedx)) selectedx=0;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(scr_libs!=NULL) freeapplib(scr_libs);
	scr_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(directory)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",directory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",directory);
#endif
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(scr_libs,tmp);
	}
	if(scr_libs->numlibs<1)
	{
		addAPPlib(scr_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(scr_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(directory!=NULL) Rfree(directory);
	if(dirx!=NULL) Rfree(dirx);
	if(selectedx>=scr_libs->numlibs) selectedx=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LIBRARY LIST",selectedx,scr_libs))
		updatersrc(mainrsrc,"LIBRARY LIST");
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory)) 
		{
			inlist=TRUE;		
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
		updatersrc(mainrsrc,"DIRECTORY LIST");
	getscreenlist(mainrsrc);
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s])) 
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s])) return;
			updatersrc(mainrsrc,"DIRECTORY");
	}
	getscreenlist(mainrsrc);
}
static void doexit(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	freetestlist();
	freeapplib(scr_libs);
	if(set_whichs!=NULL) freeapplib(set_whichs);
	if(rsrclistfrom!=NULL) freeapplib(rsrclistfrom);
	if(rsrclistto!=NULL) freeapplib(rsrclistto);
	if(exactlist!=NULL) freeapplib(exactlist);
	ShutdownSecurity();
}
static void setalllibs(RDArsrc *r)
{
	char s=FALSE;

	readwidget(r,"ALL LIBRARIES");
	FINDRSCGETCHAR(r,"ALL LIBRARIES",&s);
	if(s)
	{
		FINDRSCSETEDITABLE(r,"LIBRARY LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"LIBRARY LIST",FALSE);
	} else {
		FINDRSCSETEDITABLE(r,"LIBRARY LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"LIBRARY LIST",TRUE);
	}
}
static void setros(RDArsrc *r)
{
	char s=FALSE;

	readwidget(r,"RANGE ON SCREEN");
	FINDRSCGETCHAR(r,"RANGE ON SCREEN",&s);
	if(!s)
	{
		FINDRSCSETEDITABLE(r,"FROM SCREEN",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM SCREEN",FALSE);
		FINDRSCSETEDITABLE(r,"TO SCREEN",FALSE);
		FINDRSCSETSENSITIVE(r,"TO SCREEN",FALSE);
	} else {
		FINDRSCSETEDITABLE(r,"FROM SCREEN",TRUE);
		FINDRSCSETSENSITIVE(r,"FROM SCREEN",TRUE);
		FINDRSCSETEDITABLE(r,"TO SCREEN",TRUE);
		FINDRSCSETSENSITIVE(r,"TO SCREEN",TRUE);
	}
}
static void setallds(RDArsrc *r)
{
	char s=FALSE;

	readwidget(r,"ALL DIRECTORIES");
	FINDRSCGETCHAR(r,"ALL DIRECTORIES",&s);
	if(s)
	{
		FINDRSCSETEDITABLE(r,"DIRECTORY LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"DIRECTORY LIST",FALSE);
	} else {
		FINDRSCSETEDITABLE(r,"DIRECTORY LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"DIRECTORY LIST",TRUE);
	}
}
static void setpatscn(RDArsrc *r)
{
	char s=FALSE;

	readwidget(r,"PATTERN MATCH SCREEN");
	FINDRSCGETCHAR(r,"PATTERN MATCH SCREEN",&s);
	if(s==FALSE)
	{
		FINDRSCSETEDITABLE(r,"SCREEN PATTERN",FALSE);
		FINDRSCSETSENSITIVE(r,"SCREEN PATTERN",FALSE);
	} else {
		FINDRSCSETEDITABLE(r,"SCREEN PATTERN",TRUE);
		FINDRSCSETSENSITIVE(r,"SCREEN PATTERN",TRUE);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char t=FALSE,*defdir=NULL,allds=FALSE,alllibs=FALSE;
	short to_value=FALSE,from_value=TRUE;
	RDArsrc *mainrsrc=NULL;
	int zr=0;

	if(InitializeSubsystems(argc,argv,"GUI","CONVERT SCREENS")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW("GUI","CONVERT SCREENS");
	set_whichs=APPlibNEW();
	addAPPlib(set_whichs,"Matching Widget");
	addAPPlib(set_whichs,"Previous Widget");
	addAPPlib(set_whichs,"Next Widget");
	addAPPlib(set_whichs,"Two Widgets Previous");
	addAPPlib(set_whichs,"Two Widgets Next");
	addlstrsrc(mainrsrc,"SET WHICH",&zr,TRUE,NULL,set_whichs->numlibs,&set_whichs->libs,NULL);
	scr_libs=APPlibNEW();
	maketestlist();
	if(exactlist!=NULL) freeapplib(exactlist);
	exactlist=APPlibNEW();
	addAPPlib(exactlist,"Match Name Only");
	addAPPlib(exactlist,"Match All Resources");
	addAPPlib(exactlist,"Match Name Substring Only");
	addAPPlib(exactlist,"Pattern Match Name");
	addlstrsrc(mainrsrc,"MATCH TYPE",&zr,TRUE,NULL,exactlist->numlibs,&exactlist->libs,NULL);
	if(rsrclistfrom!=NULL) freeapplib(rsrclistfrom);
	rsrclistfrom=APPlibNEW();
	addAPPlib(rsrclistfrom,"WordWrap & No Horizontal SB");
	addAPPlib(rsrclistfrom,"Horizontal SB & No WordWrap");
	if(rsrclistto!=NULL) freeapplib(rsrclistto);
	rsrclistto=APPlibNEW();
	addAPPlib(rsrclistto,"WordWrap & No Horizontal SB");
	addAPPlib(rsrclistto,"Horizontal SB & No WordWrap");
	dir_libs=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	addstdrsrc(mainrsrc,"ALL DIRECTORIES",BOOLNS,1,&allds,TRUE);
	FINDRSCSETFUNC(mainrsrc,"ALL DIRECTORIES",setallds,NULL);
	addstdrsrc(mainrsrc,"ALL LIBRARIES",BOOLNS,1,&alllibs,TRUE);
	FINDRSCSETFUNC(mainrsrc,"ALL LIBRARIES",setalllibs,NULL);
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	addlstrsrc(mainrsrc,"DIRECTORY LIST",&zr,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[0],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",0,dir_libs);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist,NULL);
	addlstrsrc(mainrsrc,"LIBRARY LIST",&zr,TRUE,NULL,scr_libs->numlibs,&scr_libs->libs,NULL);
	getscreenlist(mainrsrc);
	addstdrsrc(mainrsrc,"FROM WIDGET NAME",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"FROM WIDGET LABEL",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"FROM WIDGET PIXMAP",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"FROM YWINDOW LABEL",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(mainrsrc,"FROM WIDGET TYPES",0,TRUE,changewidgetlist,NE(wdgttypes),&wdgtlist,&from_value);
	addstdrsrc(mainrsrc,"FROM NUMBER ROWS",SHORTV,3,0,TRUE);
	addstdrsrc(mainrsrc,"FROM NUMBER COLUMNS",SHORTV,3,0,TRUE);
	addlstrsrc(mainrsrc,"FROM RESOURCE TYPES",&zr,TRUE,changersctype,rsrclistfrom->numlibs,&rsrclistfrom->libs,&from_value);
	addsctrsrc(mainrsrc,"FROM EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"FROM EDITABLE EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"FROM SENSITIVE EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"FROM TRANSVERSAL EXPRESSION",0,NULL,TRUE);	
	addstdrsrc(mainrsrc,"TO WIDGET NAME",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO WIDGET LABEL",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO WIDGET PIXMAP",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO YWINDOW LABEL",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(mainrsrc,"TO WIDGET TYPES",&zr,TRUE,changewidgetlist,NE(wdgttypes),&wdgtlist,&to_value);
	addstdrsrc(mainrsrc,"TO NUMBER ROWS",SHORTV,3,0,TRUE);
	addstdrsrc(mainrsrc,"TO NUMBER COLUMNS",SHORTV,3,0,TRUE);
	addlstrsrc(mainrsrc,"TO RESOURCE TYPES",&zr,TRUE,changersctype,rsrclistto->numlibs,&rsrclistto->libs,&to_value);
	addsctrsrc(mainrsrc,"TO EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"TO EDITABLE EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"TO SENSITIVE EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"TO TRANSVERSAL EXPRESSION",0,NULL,TRUE);	
	addstdrsrc(mainrsrc,"PATTERN MATCH SCREEN",BOOLNS,1,&t,TRUE);
	FINDRSCSETFUNC(mainrsrc,"PATTERN MATCH SCREEN",setpatscn,NULL);
	addstdrsrc(mainrsrc,"SCREEN PATTERN",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"RANGE ON SCREEN",BOOLNS,1,&t,TRUE);
	FINDRSCSETFUNC(mainrsrc,"RANGE ON SCREEN",setros,NULL);
	addstdrsrc(mainrsrc,"FROM SCREEN",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO SCREEN",VARIABLETEXT,0,NULL,TRUE);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,convscrn,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setros(mainrsrc);
	setpatscn(mainrsrc);
	setallds(mainrsrc);
	setalllibs(mainrsrc);
	changewidgetlist(mainrsrc,&from_value);
	changewidgetlist(mainrsrc,&to_value);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
