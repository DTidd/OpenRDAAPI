#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;
static void MBGUI_UPDATE_MENU_MENU(void);
static void MBGUI_UPDATE_MENU(void);
static void MBGUI_UPDATE_FINMGT_MENU(void);
static void MBGUI_UPDATE_VENPMT_MENU(void);
static void MBGUI_UPDATE_PURORD_MENU(void);
static void MBGUI_UPDATE_PRSNNL_MENU(void);
static void MBGUI_UPDATE_PAYROLL_MENU(void);
static void MBGUI_UPDATE_LVEMGT_MENU(void);
static void MBGUI_UPDATE_RLVEMGT_MENU(void);
static void MBGUI_UPDATE_BNKREC_MENU(void);
static void MBGUI_UPDATE_BUDPREP_MENU(void);
static void MBGUI_UPDATE_PROPERTY_MENU(void);
static void MBGUI_UPDATE_UTLBLL_MENU(void);
static void MBGUI_UPDATE_MISCBILL_MENU(void);
static void MBGUI_UPDATE_OCCTAX_MENU(void);
static void MBGUI_UPDATE_DMVREG_MENU(void);
static void MBGUI_UPDATE_BLDPRMT_MENU(void);
static void UPDATE_ARCHIVE_MENU(void);
static void MBGUI_UPDATE_CITWEB_MENU(void);
static void MBGUI_UPDATE_RLSTMGT_MENU(void);
static void MBGUI_UPDATE_VIOLATION_MENU(void);
static void MBGUI_UPDATE_CSHDWR_MENU(void);

static void MBGUI_UPDATE_DATABASE_MENU(void);
static void MBGUI_UPDATE_SECURITY_MENU(void);
static void MBGUI_UPDATE_RPTGEN_MENU(void);
static void MBGUI_UPDATE_GUI_MENU(void);
static void MBGUI_UPDATE_UTILITIES_MENU(void);
static void MBGUI_UPDATE_IMPORTEXPORT(void);
static void MBGUI_UPDATE_OLHELP_MENU(void);
static void MBGUI_UPDATE_FIXASS_MENU(void);
void Make_Update_Menu(void);
static void XPERT_UPDATE_FINMGT_MENU(void);
static void XPERT_UPDATE_VENPMT_MENU(void);
static void XPERT_UPDATE_PURORD_MENU(void);
static void XPERT_UPDATE_REQMGT_MENU(void);
static void XPERT_UPDATE_OPENSS_MENU(void);
static void XPERT_UPDATE_PRSNNL_MENU(void);
static void XPERT_UPDATE_PAYROLL_MENU(void);
static void XPERT_UPDATE_POSTRK_MENU(void);
static void XPERT_UPDATE_LVEMGT_MENU(void);
static void XPERT_UPDATE_EMPABS_MENU(void);
static void XPERT_UPDATE_SUBMGT_MENU(void);
static void XPERT_UPDATE_LVSIMP_MENU(void);
static void XPERT_UPDATE_BNKREC_MENU(void);
static void XPERT_UPDATE_BFTMGT_MENU(void);
static void XPERT_UPDATE_BUDPREP_MENU(void);
static void XPERT_UPDATE_PROPERTY_MENU(void);
static void XPERT_UPDATE_UTLBLL_MENU(void);
static void XPERT_UPDATE_MISCBILL_MENU(void);
static void XPERT_UPDATE_OCCTAX_MENU(void);
static void XPERT_UPDATE_DMVREG_MENU(void);
static void XPERT_UPDATE_BLDPRMT_MENU(void);
static void XPERT_UPDATE_CITWEB_MENU(void);
static void XPERT_UPDATE_RLSTMGT_MENU(void);
static void XPERT_UPDATE_VIOLATION_MENU(void);
static void XPERT_UPDATE_CSHDWR_MENU(void);
static void XPERT_UPDATE_FIXASS_MENU(void);
static void XPERT_UPDATE_INVCTL_MENU(void);
static void XPERT_UPDATE_APPMGT_MENU(void);
static void XPERT_UPDATE_CATALOGUE_MENU(void);
static void MNU_PAYROLL_MKW2(void);
static void MNU_VENPMT_MK1099(void);
static void MNU_DOWNLOAD_INSTRUCTIONS(void);

void Make_Update_Menu()
{
	MBGUI_UPDATE_MENU_MENU();
	MBGUI_UPDATE_MENU();
	MBGUI_UPDATE_DATABASE_MENU();
	MBGUI_UPDATE_SECURITY_MENU();
	MBGUI_UPDATE_RPTGEN_MENU();
	MBGUI_UPDATE_GUI_MENU();
	MBGUI_UPDATE_UTILITIES_MENU();
	MBGUI_UPDATE_OLHELP_MENU();
	MNU_DOWNLOAD_INSTRUCTIONS();
	if(XPERT_SETUP->software_type==2)
	{
		MBGUI_UPDATE_IMPORTEXPORT();
		if(XPERT_SETUP->FINMGT==TRUE)
		{
			MBGUI_UPDATE_FINMGT_MENU();
		}
		if(XPERT_SETUP->VENPMT==TRUE)
		{
			MBGUI_UPDATE_VENPMT_MENU();
			MNU_VENPMT_MK1099();
		}
		if(XPERT_SETUP->PURORD==TRUE)
		{
			MBGUI_UPDATE_PURORD_MENU();
			XPERT_UPDATE_CATALOGUE_MENU();
		}
		if(XPERT_SETUP->PRSNNL==TRUE)
		{
			MBGUI_UPDATE_PRSNNL_MENU();
		}
		if(XPERT_SETUP->PAYROLL==TRUE)
		{
			MBGUI_UPDATE_PAYROLL_MENU();
			MNU_PAYROLL_MKW2();
		}
		if(XPERT_SETUP->LVEMGT==TRUE)
		{
			MBGUI_UPDATE_LVEMGT_MENU();
		}
		if(XPERT_SETUP->RLVEMGT==TRUE)
		{
			MBGUI_UPDATE_RLVEMGT_MENU();
		}
		if(XPERT_SETUP->BNKREC==TRUE)
		{
			MBGUI_UPDATE_BNKREC_MENU();
		}
		if(XPERT_SETUP->BUDPREP==TRUE)
		{
			MBGUI_UPDATE_BUDPREP_MENU();
		}
		if(XPERT_SETUP->DMVREG==TRUE)
		{
			MBGUI_UPDATE_DMVREG_MENU();
		} 
		if(XPERT_SETUP->OCCTAX==TRUE)
		{
			MBGUI_UPDATE_OCCTAX_MENU();
		}
		if(XPERT_SETUP->UTLBLL==TRUE)
		{
			MBGUI_UPDATE_UTLBLL_MENU();
		} 
		if(XPERT_SETUP->MISCBILL==TRUE)
		{
			MBGUI_UPDATE_MISCBILL_MENU();
		} 
		if(XPERT_SETUP->PROPERTY==TRUE)
		{	
			MBGUI_UPDATE_PROPERTY_MENU();
		}
		if(XPERT_SETUP->RLSTMGT==TRUE)
		{
			MBGUI_UPDATE_RLSTMGT_MENU();
		}
		if(XPERT_SETUP->BLDPRMT==TRUE)
		{
			MBGUI_UPDATE_BLDPRMT_MENU();
		}
		if(XPERT_SETUP->ARCHIVE==TRUE)
		{
			UPDATE_ARCHIVE_MENU();
		}
		if(XPERT_SETUP->CITWEB==TRUE)
		{
			MBGUI_UPDATE_CITWEB_MENU();
		}
		if(XPERT_SETUP->VIOLATION==TRUE)
		{	
			MBGUI_UPDATE_VIOLATION_MENU();
		}
		if(XPERT_SETUP->CSHDWR==TRUE)
		{	
			MBGUI_UPDATE_CSHDWR_MENU();
		}
		if(XPERT_SETUP->FIXASS==TRUE)
		{
			MBGUI_UPDATE_FIXASS_MENU();
		}
	} else {
		if(XPERT_SETUP->FINMGT==TRUE)
		{
			XPERT_UPDATE_FINMGT_MENU();
		}
		if(XPERT_SETUP->VENPMT==TRUE)
		{
			XPERT_UPDATE_VENPMT_MENU();
		}
		if(XPERT_SETUP->PURORD==TRUE)
		{
			XPERT_UPDATE_PURORD_MENU();
		}
		if(XPERT_SETUP->REQMGT==TRUE)
		{
			XPERT_UPDATE_REQMGT_MENU();
		}
		if(XPERT_SETUP->HRM==TRUE)
		{
			XPERT_UPDATE_OPENSS_MENU();
		}
		if(XPERT_SETUP->PRSNNL==TRUE)
		{
			XPERT_UPDATE_PRSNNL_MENU();
		}
		if(XPERT_SETUP->PAYROLL==TRUE)
		{
			XPERT_UPDATE_PAYROLL_MENU();
		}
		if(XPERT_SETUP->POSTRK==TRUE)
		{
			XPERT_UPDATE_POSTRK_MENU();
		}
		if(XPERT_SETUP->LVEMGT==TRUE)
		{
			XPERT_UPDATE_LVEMGT_MENU();
		}
		if(XPERT_SETUP->EMPABS==TRUE)
		{
			XPERT_UPDATE_EMPABS_MENU();
		}
		if(XPERT_SETUP->SUBMGT==TRUE)
		{
			XPERT_UPDATE_SUBMGT_MENU();
		}
		if(XPERT_SETUP->LVSIMP==TRUE)
		{
			XPERT_UPDATE_LVSIMP_MENU();
		}
		if(XPERT_SETUP->BNKREC==TRUE)
		{
			XPERT_UPDATE_BNKREC_MENU();
		}
		if(XPERT_SETUP->BFTMGT==TRUE)
		{
			XPERT_UPDATE_BFTMGT_MENU();
		}
		if(XPERT_SETUP->BUDPREP==TRUE)
		{
			XPERT_UPDATE_BUDPREP_MENU();
		}
		if(XPERT_SETUP->DMVREG==TRUE)
		{
			XPERT_UPDATE_DMVREG_MENU();
		} 
		if(XPERT_SETUP->OCCTAX==TRUE)
		{
			XPERT_UPDATE_OCCTAX_MENU();
		}
		if(XPERT_SETUP->UTLBLL==TRUE)
		{
			XPERT_UPDATE_UTLBLL_MENU();
		} 
		if(XPERT_SETUP->MISCBILL==TRUE)
		{
			XPERT_UPDATE_MISCBILL_MENU();
		} 
		if(XPERT_SETUP->PROPERTY==TRUE)
		{	
			XPERT_UPDATE_PROPERTY_MENU();
		}
		if(XPERT_SETUP->RLSTMGT==TRUE)
		{
			XPERT_UPDATE_RLSTMGT_MENU();
		}
		if(XPERT_SETUP->BLDPRMT==TRUE)
		{
			XPERT_UPDATE_BLDPRMT_MENU();
		}
		if(XPERT_SETUP->ARCHIVE==TRUE)
		{
			UPDATE_ARCHIVE_MENU();
		}
		if(XPERT_SETUP->CITWEB==TRUE)
		{
			XPERT_UPDATE_CITWEB_MENU();
		}
		if(XPERT_SETUP->VIOLATION==TRUE)
		{	
			XPERT_UPDATE_VIOLATION_MENU();
		}
		if(XPERT_SETUP->CSHDWR==TRUE)
		{	
			XPERT_UPDATE_CSHDWR_MENU();
		}
		if(XPERT_SETUP->FIXASS==TRUE)
		{
			XPERT_UPDATE_FIXASS_MENU();
		}
		if(XPERT_SETUP->INVCTL==TRUE)
		{
			XPERT_UPDATE_INVCTL_MENU();
		}
		if(XPERT_SETUP->APPMGT==TRUE)
		{
			XPERT_UPDATE_APPMGT_MENU();
		}
	}
}

static void MBGUI_UPDATE_MENU_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE MENU","domenu.EXT UTILITIES \"MBGUI UPDATE MENU\"");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE MENU","domenu.EXT UTILITIES \"XPERT UPDATE MENU\"");
	}
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
		/* for Xpert's Update Make programs, different names */
	}
}

static void MBGUI_UPDATE_DATABASE_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE DATABASE","mkdat.EXT");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE DATABASE","mkdat.EXT");
	}
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
static void MBGUI_UPDATE_SECURITY_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE SECURITY","mksec.EXT");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE SECURITY","mksec.EXT");
	}
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
static void MBGUI_UPDATE_RPTGEN_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE RPTGEN","mkrpt.EXT");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE RPTGEN","mkrpt.EXT");
	}
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
static void MBGUI_UPDATE_GUI_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE GUI","mkgui.EXT");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE GUI","mkgui.EXT");
	}
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
static void MBGUI_UPDATE_IMPORTEXPORT()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE IMPORT/EXPORT","mkie.EXT");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE IMPORT/EXPORT","mkimpexp.EXT");
	}
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
static void MBGUI_UPDATE_UTILITIES_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE UTILITIES","mkutl.EXT");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE UTILITIES","mkutl.EXT");
	}
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
static void MBGUI_UPDATE_OLHELP_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		menu=RDAmenuNEW("MBGUI UPDATE OLHELP","mkolh.EXT");
	} else {
		menu=RDAmenuNEW("XPERT UPDATE OLHELP","mkolh.EXT");
	}
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
static void MBGUI_UPDATE_FINMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE FINMGT","mkfin.EXT");
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

static void MBGUI_UPDATE_VENPMT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE VENPMT","mkven.EXT");
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
static void MBGUI_UPDATE_PURORD_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE PURORD","mkpur.EXT");
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

static void MBGUI_UPDATE_PRSNNL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE PRSNNL","mkper.EXT");
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

static void MBGUI_UPDATE_PAYROLL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE PAYROLL","mkpay.EXT");
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

static void UPDATE_ARCHIVE_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("UPDATE ARCHIVE","mkarchive.EXT");
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
static void MBGUI_UPDATE_BLDPRMT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE BLDPRMT","mkbld.EXT");
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
static void MBGUI_UPDATE_CITWEB_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE CITWEB","mkctw.EXT");
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


static void MBGUI_UPDATE_RLSTMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE RLSTMGT","mkrlst.EXT");
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

static void MBGUI_UPDATE_RLVEMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE RLVEMGT","mkrlv.EXT");
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
static void MBGUI_UPDATE_LVEMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE LVEMGT","mklve.EXT");
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

static void MBGUI_UPDATE_BNKREC_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE BNKREC","mkbnk.EXT");
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
static void MBGUI_UPDATE_BUDPREP_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE BUDPREP","mkbud.EXT");
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
static void MBGUI_UPDATE_FIXASS_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE FIXASS","mkfix.EXT");
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

static void MBGUI_UPDATE_MENU()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	if(XPERT_SETUP->software_type==2)
	{
		scrn=RDAscrnNEW("UTILITIES","MBGUI UPDATE MENU");
	} else {
		scrn=RDAscrnNEW("UTILITIES","XPERT UPDATE MENU");
	}
	if(scrn!=NULL)
	{
		if(XPERT_SETUP->software_type==2)
		{
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,6,"DOWNLOAD INSTRUCTIONS","Download Instructions","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,6,"FTPGET FTP.RDASYS.COM","DOWNLOAD PROGRAMS","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"MBGUI UPDATE SECURITY","Update MBGUI SECURITY","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"MBGUI UPDATE UTILITIES","Update MBGUI UTILITIES","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"MBGUI UPDATE DATABASE","Update MBGUI DATABASE","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"MBGUI UPDATE OLHELP","Update MBGUI OLHELP","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"MBGUI UPDATE RPTGEN","Update MBGUI RPTGEN","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"MBGUI UPDATE GUI","Update MBGUI GUI","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"MBGUI UPDATE IMPORT/EXPORT","Update Import/Export","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			if(XPERT_SETUP->ARCHIVE==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"UPDATE ARCHIVE","Update F.A.S.T.","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->FINMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE FINMGT","Update MBGUI FINMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->VENPMT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE VENPMT","Update MBGUI VENPMT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"VENPMT MK1099","Update/Install 1099s","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->PURORD==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE PURORD","Update MBGUI PURORD","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->PRSNNL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE PRSNNL","Update MBGUI PRSNNL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->PAYROLL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE PAYROLL","Update MBGUI PAYROLL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"PAYROLL MKW2","Update/Install W2s","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->LVEMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE LVEMGT","Update MBGUI LVEMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->BNKREC==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE BNKREC","Update MBGUI BNKREC","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->RLVEMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE RLVEMGT","Update MBGUI RLVEMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->BUDPREP==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE BUDPREP","Update MBGUI BUDPREP","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->DMVREG==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE DMVREG","Update MBGUI DMVREG","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			} 
			if(XPERT_SETUP->OCCTAX==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE OCCTAX","Update MBGUI OCCTAX","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->UTLBLL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE UTLBLL","Update MBGUI UTLBLL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->MISCBILL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE MISCBILL","Update MBGUI MISCBILL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->PROPERTY==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE PROPERTY","Update MBGUI PROPERTY","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->RLSTMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE RLSTMGT","Update MBGUI RLSTMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->BLDPRMT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE BLDPRMT","Update MBGUI BLDPRMT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->VIOLATION==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE VIOLATION","Update MBGUI VIOLATION","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->CSHDWR==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE CSHDWR","Update MBGUI CSHDWR","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->FIXASS==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"MBGUI UPDATE FIXASS","Update MBGUI FIXASS","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		} else {
			ADVaddwdgt(scrn,12,"","","","",400,220,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,6,"DOWNLOAD INSTRUCTIONS","Download Instructions","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,6,"FTPGET FTP.RDASYS.COM","DOWNLOAD PROGRAMS","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"XPERT UPDATE SECURITY","Update XPERT SECURITY","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"XPERT UPDATE UTILITIES","Update XPERT UTILITIES","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"XPERT UPDATE DATABASE","Update XPERT DATABASE","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"XPERT UPDATE OLHELP","Update XPERT OLHELP","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"XPERT UPDATE RPTGEN","Update XPERT RPTGEN","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"XPERT UPDATE GUI","Update XPERT GUI","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			if(XPERT_SETUP->ARCHIVE==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"UPDATE ARCHIVE","Update F.A.S.T.","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->FINMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE FINMGT","Update XPERT FINMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->BNKREC==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE BNKREC","Update XPERT BNKREC","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->VENPMT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE VENPMT","Update XPERT VENPMT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"VENPMT MK1099","Update/Install 1099s","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			/*
			if(XPERT_SETUP->REQMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE REQMGT","Update XPERT REQMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			*/
			if(XPERT_SETUP->PURORD==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE PURORD","Update XPERT PURORD","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->REQMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE REQMGT","Update XPERT REQMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->HRM==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE OPENSS","Update XPERT OPENSS","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->PRSNNL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE PRSNNL","Update XPERT PRSNNL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->POSTRK==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE POSTRK","Update XPERT POSTRK","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->PAYROLL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE PAYROLL","Update XPERT PAYROLL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"PAYROLL MKW2","Update/Install W2s","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->BFTMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE BFTMGT","Update XPERT BFTMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->LVEMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE LVEMGT","Update XPERT LVEMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->EMPABS==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE EMPABS","Update XPERT EMPABS","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->SUBMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE SUBMGT","Update XPERT SUBMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->LVSIMP==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE LVSIMP","Update XPERT LVSIMP","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->BUDPREP==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE BUDPREP","Update XPERT BUDPREP","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->CSHDWR==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE CSHDWR","Update XPERT CSHDWR","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->DMVREG==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE DMVREG","Update XPERT DMVREG","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			} 
			if(XPERT_SETUP->OCCTAX==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE OCCTAX","Update XPERT OCCTAX","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->UTLBLL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE UTLBLL","Update XPERT UTLBLL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->MISCBILL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE MISCBILL","Update XPERT MISCBILL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->PROPERTY==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE PROPERTY","Update XPERT PROPERTY","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->RLSTMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE RLSTMGT","Update XPERT RLSTMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->BLDPRMT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE BLDPRMT","Update XPERT BLDPRMT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->VIOLATION==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE VIOLATION","Update XPERT VIOLATION","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->FIXASS==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE FIXASS","Update XPERT FIXASS","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->INVCTL==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE INVCTL","Update XPERT INVCTL","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->APPMGT==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE APPMGT","Update XPERT APPMGT","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			if(XPERT_SETUP->CATALOGUE==TRUE)
			{
				ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,6,"XPERT UPDATE CATALOGUE","Update XPERT CATALOGUE","","",0,0,0,NULL,NULL,NULL,NULL);
				ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			}
			ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		}
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SYSTEM ADMINISTRATION MENU","System Administration Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MAIN MENU","Main Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(17+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [XPERT UPDATE MENU], Can Not Save Maintain Master!");
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
static void MBGUI_UPDATE_DMVREG_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE DMVREG","mkdmv.EXT");
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
static void MBGUI_UPDATE_OCCTAX_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE OCCTAX","mkocc.EXT");
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
static void MBGUI_UPDATE_UTLBLL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE UTLBLL","mkubll.EXT");
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
static void MBGUI_UPDATE_MISCBILL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE MISCBILL","mkmbll.EXT");
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
static void MBGUI_UPDATE_PROPERTY_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE PROPERTY","mkprp.EXT");
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
static void MBGUI_UPDATE_VIOLATION_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE VIOLATION","mkvltn.EXT");
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
static void MBGUI_UPDATE_CSHDWR_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("MBGUI UPDATE CSHDWR","mkcsh.EXT");
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
static void XPERT_UPDATE_DMVREG_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE DMVREG","mkdmv.EXT");
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
static void XPERT_UPDATE_OCCTAX_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE OCCTAX","mkocctax.EXT");
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
static void XPERT_UPDATE_UTLBLL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE UTLBLL","mkutlbll.EXT");
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
static void XPERT_UPDATE_MISCBILL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE MISCBILL","mkmiscbill.EXT");
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
static void XPERT_UPDATE_PROPERTY_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE PROPERTY","mkproperty.EXT");
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
static void XPERT_UPDATE_VIOLATION_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE VIOLATION","mkvltn.EXT");
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
static void XPERT_UPDATE_CSHDWR_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE CSHDWR","mkcshdwr.EXT");
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
static void XPERT_UPDATE_FINMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE FINMGT","mkfinmgt.EXT");
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

static void XPERT_UPDATE_VENPMT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE VENPMT","mkvenpmt.EXT");
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
static void XPERT_UPDATE_PURORD_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE PURORD","mkpurord.EXT");
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
static void XPERT_UPDATE_REQMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE REQMGT","mkreqmgt.EXT");
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
static void XPERT_UPDATE_OPENSS_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE OPENSS","mkopenss.EXT");
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
static void XPERT_UPDATE_PRSNNL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE PRSNNL","mkprsnnl.EXT");
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

static void XPERT_UPDATE_POSTRK_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE POSTRK","mkpostrk.EXT");
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

static void XPERT_UPDATE_PAYROLL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE PAYROLL","mkpayroll.EXT");
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

static void XPERT_UPDATE_BLDPRMT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE BLDPRMT","mkbldprmt.EXT");
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
static void XPERT_UPDATE_CITWEB_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE CITWEB","mkctw.EXT");
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


static void XPERT_UPDATE_RLSTMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE RLSTMGT","mkrlstmgt.EXT");
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

static void XPERT_UPDATE_LVSIMP_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE LVSIMP","mklvsimp.EXT");
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

static void XPERT_UPDATE_LVEMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE LVEMGT","mklvemgt.EXT");
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

static void XPERT_UPDATE_BNKREC_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE BNKREC","mkbnkrec.EXT");
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
static void XPERT_UPDATE_BUDPREP_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE BUDPREP","mkbudprep.EXT");
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
static void XPERT_UPDATE_FIXASS_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE FIXASS","mkfixass.EXT");
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
static void XPERT_UPDATE_INVCTL_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE INVCTL","mkinvctl.EXT");
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
static void XPERT_UPDATE_APPMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE APPMGT","mkappmgt.EXT");
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
static void XPERT_UPDATE_CATALOGUE_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE CATALOGUE","mkcatalogue.EXT");
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
static void XPERT_UPDATE_EMPABS_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE EMPABS","mkempabs.EXT");
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
static void XPERT_UPDATE_SUBMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE SUBMGT","mksubmgt.EXT");
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
static void XPERT_UPDATE_BFTMGT_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("XPERT UPDATE BFTMGT","mkbftmgt.EXT");
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
static void MNU_PAYROLL_MKW2()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("PAYROLL MKW2","mkw2.EXT");
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
static void MNU_VENPMT_MK1099()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("VENPMT MK1099","mk1099.EXT");
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
static void MNU_DOWNLOAD_INSTRUCTIONS()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("DOWNLOAD INSTRUCTIONS","dohttp.EXT \"http://wiki.openrda.com/wiki/index.php/Category:PROGRAM_UPDATES\"");
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
