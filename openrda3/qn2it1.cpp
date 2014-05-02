#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

extern Wt::WContainerWidget *UnPostedExpenditureActivity(void);
extern Wt::WContainerWidget *UnPostedRevenueActivity(void);
extern Wt::WContainerWidget *UnProcessedJournalEntry(void);
extern Wt::WContainerWidget *UnProcessedReceipt(void);
extern Wt::WContainerWidget *UnPostedGeneralLedgerActivity(void);
extern Wt::WContainerWidget *UnProcessedVoucherStatus(void);
extern Wt::WContainerWidget *OpenPObyVendorDisplayDashBoard(void);
extern int ExpendActValuetoVisual,RevenueActValuetoVisual;
extern short ExpendActShowTable,RevenueActShowTable;
extern int JournalEntryValuetoVisual,ReceiptValuetoVisual;
extern short JournalEntryShowTable,ReceiptShowTable;
extern int GenLdgrActValuetoVisual;
extern short GenLdgrActShowTable,OPENRDA_DATLOG;
extern int VoucherStatusValuetoVisual;
extern short VoucherStatusShowTable,OPENRDA_STYLE;
extern int OpenPObyVendorValuetoVisual;
extern short OpenPObyVendorShowTable;
extern Wt::WContainerWidget *CreateFeed(char *);

extern FILE *myMENUITEMS;
extern Wt::WPushButton *Reports_P,*Query_P;
extern Wt::WPushButton *Maintenance_P,*Setup_P,*Other_P;

extern void CheckRDAUpdates(void);
extern void ReportsMenuAboutToShow(Wt::WPopupMenu *);
extern void QueryMenuAboutToShow(Wt::WPopupMenu *);
extern void FAST_Build(Wt::WWidget *),FAST_Scans(Wt::WWidget *),FAST_Cabinet(Wt::WWidget *);
extern APPlib *ENV_Start;
extern Wt::WContainerWidget *MainWindow,*MainWindowCenter,*MainWindowRight;
extern Wt::WContainerWidget *MainWindowDock,*MainWindowStatus;
extern Wt::WGridLayout *MWCL;
extern Wt::WPopupMenu *FPOP,*Edit,*QueryPop,*Reports;
extern Wt::WPopupMenu *Maintenance,*Setup,*Other,*Modules;
extern Wt::WPopupMenu *Window;
extern char *CURRENT_MODULE,*LAST_MODULE;
extern int LAST_EXE,ARCHIVE_STYLE;
extern short MODULE_FILENO,MENUITEM_FILENO;
extern char IGNORE_RESTRICTED;
extern void CreateStatusBar(void),CreateAccordionBar(void);

extern RDArsrc *DockRSRC;
Wt::WGridLayout *CenterLayout=NULL;
Wt::WContainerWidget *RDA_Canvas=NULL;
Wt::WString *LandingPage=NULL,*LandingPageSource=NULL;

char *Plainfamt(double v,short l)
{
	char *tmp=NULL;

	tmp=Rmalloc(l+1);
	sprintf(tmp,"%*.02f",l-3,v/100);
	return(tmp);	
}

char HasSecurity(int w)
{
	char s=FALSE;
	char *prog=NULL,*arg1=NULL,*arg2=NULL,*arg3=NULL,*arg4=NULL,*arg5=NULL;
	char *arg6=NULL,*arg7=NULL,*arg8=NULL;
	
	ZERNRD(MENUITEM_FILENO);
	FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
	FINDFLDSETINT(MENUITEM_FILENO,"ITEM NUMBER",w);
	if(!EQLNRD(MENUITEM_FILENO,1))
	{
		FINDFLDGETSTRING(MENUITEM_FILENO,"PROGRAM",&prog);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 1",&arg1);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 2",&arg2);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 3",&arg3);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 4",&arg4);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 5",&arg5);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 6",&arg6);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 7",&arg7);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 8",&arg8);
		if(!RDAstrcmp(prog,"RUNPOWERADD"))
		{
			if(!RDAstrcmp(arg1,"-s"))
			{
				s=PROCsecurity(arg2,arg3);
			} else {
				s=PROCsecurity(arg1,arg2);
			}
		} else if(!RDAstrcmp(prog,"MULTIPOWERADD"))
		{
			if(!RDAstrcmp(arg1,"-s"))
			{
				s=PROCsecurity(arg2,arg3);
			} else {
				s=PROCsecurity(arg1,arg2);
			}
		} else if(!RDAstrcmp(prog,"DOADD") || !RDAstrcmp(prog,"doadd"))
		{
			if(!RDAstrcmp(arg1,"-s"))
			{
				s=PROCsecurity(arg2,arg3);
			} else {
				s=PROCsecurity(arg1,arg2);
			}
		} else if(!RDAstrcmp(prog,"DOADDS") || !RDAstrcmp(prog,"doadds"))
		{
			if(!RDAstrcmp(arg1,"-s"))
			{
				s=PROCsecurity(arg2,arg3);
			} else {
				s=PROCsecurity(arg1,arg2);
			}
		} else if(!RDAstrcmp(prog,"RUNMAINTAIN"))
		{
			s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"MTNMST"))
		{
			s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"RUNREPORT"))
		{
			s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"RUNSCRIPT"))
		{
			s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"DOMENU"))
		{
			s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"DOREPORT"))
		{
			s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"DOGSV"))
		{
			if(isEMPTY(arg2)) 
			{
				s=PROCsecurity(arg1,"GENERIC SETUP VARIABLES");
			} else s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"PROGUPD"))
		{
			s=PROCsecurity("UTILITIES","DISPLAY PROGRAM UPDATE STATUS");
		} else if(!RDAstrcmp(prog,"APPSTP"))
		{
			s=PROCsecurity("APPMGT","APPMGT SETUP");
		} else if(!RDAstrcmp(prog,"APPAI"))
		{
			s=PROCsecurity("APPMGT","ADMINISTRATOR INTERVIEWS");
		} else if(!RDAstrcmp(prog,"APPHIRE"))
		{
			s=PROCsecurity("APPMGT","HIRE APPLICANTS");
		} else if(!RDAstrcmp(prog,"APPPI"))
		{
			s=PROCsecurity("APPMGT","PERSONNEL INTERVIEWS");
		} else if(!RDAstrcmp(prog,"APPSAA"))
		{
			s=PROCsecurity("APPMGT","SCHEDULE ADMINISTRATOR APPOINTMENTS");
		} else if(!RDAstrcmp(prog,"APPSI"))
		{
			s=PROCsecurity("APPMGT","SUPERVISOR INTERVIEWS");
		} else if(!RDAstrcmp(prog,"APPSPA"))
		{
			s=PROCsecurity("APPMGT","SCHEDULE PERSONNEL APPOINTMENTS");
		} else if(!RDAstrcmp(prog,"APPSSA"))
		{
			s=PROCsecurity("APPMGT","SCHEDULE SUPERVISOR APPOINTMENTS");
		} else if(!RDAstrcmp(prog,"BUDSEL"))
		{
			s=PROCsecurity("BUDPREP","SELECT PAYROLL");
		} else if(!RDAstrcmp(prog,"BUDNET"))
		{
			s=PROCsecurity("BUDPREP","COMPUTE PAYROLL");
		} else if(!RDAstrcmp(prog,"BUDMTS"))
		{
			s=PROCsecurity("BUDPREP","BUDMTS");
		} else if(!RDAstrcmp(prog,"BUDUNMTS"))
		{
			s=PROCsecurity("BUDPREP","BUDUNMTS");
		} else if(!RDAstrcmp(prog,"PAYAVW"))
		{
			s=PROCsecurity("PAYROLL","ADD VENDOR WITHHOLDING VOUCHERS");
		} else if(!RDAstrcmp(prog,"PAYCHECK"))
		{
			s=PROCsecurity("PAYROLL","PRINT PAYROLL CHECKS 8.5x11");
		} else if(!RDAstrcmp(prog,"PAYCHECKMICR"))
		{
			s=PROCsecurity("PAYROLL","PRINT PAYROLL CHECKS MICR 8.5x11");
		} else if(!RDAstrcmp(prog,"PAYCHECKMICRA"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE PAYROLL CHECKS MICR 8.5x11");
		} else if(!RDAstrcmp(prog,"PAYCHECKA"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE PAYROLL CHECKS 8.5x11");
		} else if(!RDAstrcmp(prog,"PAYCHECK1"))
		{
			s=PROCsecurity("PAYROLL","PRINT PAYROLL CHECKS");
		} else if(!RDAstrcmp(prog,"PAYCHECK1A"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE PAYROLL CHECKS");
		} else if(!RDAstrcmp(prog,"PAYCHECK2"))
		{
			s=PROCsecurity("PAYROLL","PRINT PAYROLL CHECKS TRIFOLD");
		} else if(!RDAstrcmp(prog,"PAYCHECK2A"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE PAYROLL CHECKS TRIFOLD");
		} else if(!RDAstrcmp(prog,"PAYCHECKTXT"))
		{
			s=PROCsecurity("PAYROLL","PAYROLL CHECKS 8.5x11 to TEXT");
		} else if(!RDAstrcmp(prog,"DIRDEP"))
		{
			s=PROCsecurity("PAYROLL","PRINT DIRECT DEPOSIT 8.5x11");
		} else if(!RDAstrcmp(prog,"DIRDEPA"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE DIRECT DEPOSIT 8.5x11");
		} else if(!RDAstrcmp(prog,"DIRDEP1"))
		{
			s=PROCsecurity("PAYROLL","PRINT PAYROLL TRANSMITTALS");
		} else if(!RDAstrcmp(prog,"DIRDEP1A"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE PAYROLL TRANSMITTALS");
		} else if(!RDAstrcmp(prog,"DIRDEP2"))
		{
			s=PROCsecurity("PAYROLL","PRINT DIRECT DEPOSIT TRIFOLD");
		} else if(!RDAstrcmp(prog,"DIRDEP2A"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE DIRECT DEPOSIT TRIFOLD");
		} else if(!RDAstrcmp(prog,"DIRDEPFORM"))
		{
			s=PROCsecurity("PAYROLL","PRINT PAYROLL DIRECT DEPOSIT FORM 8.5x11");
		} else if(!RDAstrcmp(prog,"DIRDEPFORMA"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE PAYROLL DIRECT DEPOSIT FORM 8.5x11");
		} else if(!RDAstrcmp(prog,"DIRDEPTXT"))
		{
			s=PROCsecurity("PAYROLL","DIRECT DEPOSIT TEXT");
		} else if(!RDAstrcmp(prog,"PAYSEL"))
		{
			s=PROCsecurity("PAYROLL","SELECT PAYROLL");
		} else if(!RDAstrcmp(prog,"PAYNET"))
		{
			s=PROCsecurity("PAYROLL","COMPUTE PAYROLL");
		} else if(!RDAstrcmp(prog,"PAYSCHK"))
		{
			s=PROCsecurity("PAYROLL","SET CHECK NUMBERS");
		} else if(!RDAstrcmp(prog,"PAYSDDN"))
		{
			s=PROCsecurity("PAYROLL","SET DIRECT DEPOSIT NUMBERS");
		} else if(!RDAstrcmp(prog,"PAYMTS"))
		{
			s=PROCsecurity("PAYROLL","PAYMTS");
		} else if(!RDAstrcmp(prog,"PAYJMST"))
		{
			s=PROCsecurity("PAYROLL","MTN PAYJMST");
		} else if(!RDAstrcmp(prog,"DQUERY"))
		{
			s=PROCsecurity("FINMGT","DETAIL QUERY");
		} else if(!RDAstrcmp(prog,"PAYUNMTS"))
		{
			s=PROCsecurity("PAYROLL","PAYUNMTS");
		} else if(!RDAstrcmp(prog,"PAYVSCHK"))
		{
			s=PROCsecurity("PAYROLL","SET VENDOR WITHHOLDING CHECK NUMBERS");
		} else if(!RDAstrcmp(prog,"PAYVSETR"))
		{
			s=PROCsecurity("PAYROLL","SET VENDOR WITHHOLDING ELECTRONIC TRANSFER NUMBERS");
		} else if(!RDAstrcmp(prog,"PAYVWCHECKDS"))
		{
			s=PROCsecurity("PAYROLL","VENDOR WITHHOLDING CHECKS DOUBLE STUB");
		} else if(!RDAstrcmp(prog,"PAYVWCHECKMICR"))
		{
			s=PROCsecurity("PAYROLL","VENDOR WITHHOLDING CHECKS MICR");
		} else if(!RDAstrcmp(prog,"PAYVWCHECKMICRA"))
		{
			s=PROCsecurity("PAYROLL","ARCHIVE VENDOR WITHHOLDING CHECKS MICR");
		} else if(!RDAstrcmp(prog,"SUBCSUB"))
		{
			s=PROCsecurity("SUBMGT","CONTACT SUBSTITUTES");
		} else if(!RDAstrcmp(prog,"SUBSEL"))
		{
			s=PROCsecurity("PAYROLL","SELECT SUBSTITUTES PAYROLL");
		} else if(!RDAstrcmp(prog,"LVEASS"))
		{
			s=PROCsecurity("LVEMGT","ASSIGN LEAVE");
		} else if(!RDAstrcmp(prog,"DOIE"))
		{
			if(!isEMPTY(arg1) && !isEMPTY(arg2))
			{
				s=PROCsecurity(arg1,arg2);
			} else s=PROCsecurity("DATABASE","DO IMPORT/EXPORT");
		} else if(!RDAstrcmp(prog,"CHARTDAT"))
		{
			if(!isEMPTY(arg2) && !isEMPTY(arg3))
			{
				s=FILEsecurity(arg2,arg3,TRUE,FALSE,TRUE);
			} else s=FALSE;
		} else if(!RDAstrcmp(prog,"REMDEL"))
		{
			s=PROCsecurity("DATABASE","REMOVE DELETED RECORDS");
		} else if(!RDAstrcmp(prog,"REMDEL"))
		{
			s=PROCsecurity("DATABASE","VIEW TRANSACTION");
		} else if(!RDAstrcmp(prog,"DOCHG"))
		{
			s=PROCsecurity(arg1,arg2);
		} else if(!RDAstrcmp(prog,"DOFTPGET"))
		{
			s=PROCsecurity("UTILITIES","FTP PROGRAMS");
		} else if(!RDAstrcmp(prog,"DOFTPZIP"))
		{
			s=PROCsecurity("DATABASE","DOFTPZIP");
		} else if(!RDAstrcmp(prog,"DOARCHIVE"))
		{
			s=PROCsecurity("ARCHIVE","DOARCHIVE");
		} else if(!RDAstrcmp(prog,"DOBACKUP"))
		{
			s=PROCsecurity("DATABASE","BACKUP DATA");
		} else if(!RDAstrcmp(prog,"DORESTORE"))
		{
			s=PROCsecurity("DATABASE","RESTORE DATA");
		} else if(!RDAstrcmp(prog,"CHECKEMAIL"))
		{
			s=PROCsecurity("UTILITIES","TEST EMAIL");
		} else if(!RDAstrcmp(prog,"CSHAIDC"))
		{
			s=PROCsecurity("CSHDWR","CREATE RECEIPT IDENTIFICATIONS");
		} else if(!RDAstrcmp(prog,"DOREPORT"))
		{
			s=PROCsecurity("DATABASE","RESTORE DATA");
			if(!isEMPTY(arg1) && !isEMPTY(arg2))
			{
				s=PROCsecurity(arg1,arg2);
			} else s=PROCsecurity("RPTGEN","DO REPORT");
		} else if(!RDAstrcmp(prog,"DOHTTP"))
		{
			s=TRUE;
		} else if(!RDAstrcmp(prog,"DOWIKI"))
		{
			s=TRUE;
		} else if(!RDAstrcmp(prog,"FINSTP"))
		{
			s=PROCsecurity("FINMGT","FIN MANAGEMENT SETUP");
		} else if(!RDAstrcmp(prog,"FINCOA"))
		{
			s=PROCsecurity("FINMGT","MTN FINCOA");
		} else if(!RDAstrcmp(prog,"VENSTP"))
		{
			s=PROCsecurity("VENPMT","VENPMT SETUP");
		} else if(!RDAstrcmp(prog,"VENSCHK"))
		{
			s=PROCsecurity("VENPMT","SET CHECK NUMBERS");
		} else if(!RDAstrcmp(prog,"VENSETR"))
		{
			s=PROCsecurity("VENPMT","SET ELECTRONIC TRANSFER NUMBERS");
		} else if(!RDAstrcmp(prog,"VENCHECKDS"))
		{
			s=PROCsecurity("VENPMT","VENDOR CHECKS DOUBLE STUB");
		} else if(!RDAstrcmp(prog,"VENCHECKMICR"))
		{
			s=PROCsecurity("VENPMT","VENDOR CHECKS MICR");
		} else if(!RDAstrcmp(prog,"VENCHECKMICRA"))
		{
			s=PROCsecurity("VENPMT","ARCHIVE VENDOR CHECKS MICR");
		} else if(!RDAstrcmp(prog,"PAYSTP"))
		{
			s=PROCsecurity("PAYROLL","PAYROLL SETUP");
		} else if(!RDAstrcmp(prog,"DFACCT"))
		{
			s=PROCsecurity("GUI","DEFINE ACCOUNTS");
		} else if(!RDAstrcmp(prog,"DFADD"))
		{
			s=PROCsecurity("UTILITIES","DEFINE POWER ADD");
		} else if(!RDAstrcmp(prog,"DFCHG"))
		{
			s=PROCsecurity("UTILITIES","DEFINE CHANGE");
		} else if(!RDAstrcmp(prog,"DFGSV"))
		{
			s=PROCsecurity("UTILITIES","DEFINE GENERIC SETUP VARIABLES");
		} else if(!RDAstrcmp(prog,"DFHELP"))
		{
			s=PROCsecurity("OLHELP","DEFINE HELP");
		} else if(!RDAstrcmp(prog,"DFIE"))
		{
			s=PROCsecurity("DATABASE","DEFINE IMP EXP");
		} else if(!RDAstrcmp(prog,"DFLIST"))
		{
			s=PROCsecurity("GUI","DEFINE SCROLLED LISTS");
		} else if(!RDAstrcmp(prog,"DFLOAD"))
		{
			s=PROCsecurity("GUI","DEFINE LOAD VALUES");
		} else if(!RDAstrcmp(prog,"DFMATRIX"))
		{
			s=PROCsecurity("UTILITIES","DEFINE MATRIX");
		} else if(!RDAstrcmp(prog,"DFMENU"))
		{
			s=PROCsecurity("GUI","DEFINE MENU");
		} else if(!RDAstrcmp(prog,"DFMTN"))
		{
			s=PROCsecurity("UTILITIES","DEFINE MAINTAIN MASTERS");
		} else if(!RDAstrcmp(prog,"DFNRD"))
		{
			s=PROCsecurity("DATABASE","DEFINE FILES");
		} else if(!RDAstrcmp(prog,"RBDFIL"))
		{
			s=PROCsecurity("DATABASE","REBUILD DATAFILE");
		} else if(!RDAstrcmp(prog,"RESTRUCT"))
		{
			s=PROCsecurity("DATABASE","RESTRUCTURE DATAFILE");
		} else if(!RDAstrcmp(prog,"RNUMTRAN"))
		{
			s=PROCsecurity("DATABASE","RENUMBER TRANSACTIONS");
		} else if(!RDAstrcmp(prog,"REMTRANS"))
		{
			s=PROCsecurity("DATABASE","REMOVE TRANS FILES");
		} else if(!RDAstrcmp(prog,"VIEWTRAN"))
		{
			s=PROCsecurity("DATABASE","VIEW TRANSACTIONS");
		} else if(!RDAstrcmp(prog,"DISPLAYTAPE"))
		{
			s=PROCsecurity("DATABASE","DISPLAYTAPE");
		} else if(!RDAstrcmp(prog,"DFPRT"))
		{
			s=PROCsecurity("RPTGEN","SETUP OUTPUT DEVICES");
		} else if(!RDAstrcmp(prog,"SETPRT"))
		{
			s=PROCsecurity("RPTGEN","DEFINE OUTPUT DEVICES");
		} else if(!RDAstrcmp(prog,"DFRPT"))
		{
			s=PROCsecurity("RPTGEN","DEFINE REPORTS");
		} else if(!RDAstrcmp(prog,"DFSCN"))
		{
			s=PROCsecurity("GUI","DEFINE SCREENS");
		} else if(!RDAstrcmp(prog,"DFSCNDEF"))
		{
			s=PROCsecurity("GUI","DEFINE SCREEN DEFAULTS");
		} else if(!RDAstrcmp(prog,"DFTABLE"))
		{
			s=PROCsecurity("UTILITIES","DEFINE TABLES");
		} else if(!RDAstrcmp(prog,"DFVIR"))
		{
			s=PROCsecurity("RPTGEN","DEFINE EZ VFS");
		} else if(!RDAstrcmp(prog,"DFWWW"))
		{
			s=PROCsecurity("UTILITIES","DFWWW");
		} else if(!RDAstrcmp(prog,"EZRPT"))
		{
			s=PROCsecurity("RPTGEN","EZ DEFINE REPORTS");
		} else if(!RDAstrcmp(prog,"LASER"))
		{
			s=PROCsecurity("UTILITIES","LASER FORM TEST");
		} else if(!RDAstrcmp(prog,"MK1099"))
		{
			s=PROCsecurity("VENPMT","MAKE VEN1099");
		} else if(!RDAstrcmp(prog,"MISCSSTM"))
		{
			s=PROCsecurity("MISCBILL","SET STATEMENT NUMBERS");
		} else if(!RDAstrcmp(prog,"MKAPPMGT"))
		{
			s=PROCsecurity("APPMGT","MAKE APPMGT");
		} else if(!RDAstrcmp(prog,"MKARCHIVE"))
		{
			s=PROCsecurity("ARCHIVE","MAKE ARCHIVE");
		} else if(!RDAstrcmp(prog,"MKBFTMGT"))
		{
			s=PROCsecurity("BFTMGT","MAKE BFTMGT");
		} else if(!RDAstrcmp(prog,"MKBLDPRMT"))
		{
			s=PROCsecurity("BLDPRMT","MAKE BLDPRMT");
		} else if(!RDAstrcmp(prog,"MKBNKREC"))
		{
			s=PROCsecurity("BNKREC","MAKE BNKREC");
		} else if(!RDAstrcmp(prog,"MKBUDPREP"))
		{
			s=PROCsecurity("BUDPREP","MAKE BUDPREP");
		} else if(!RDAstrcmp(prog,"MKCATALOGUE"))
		{
			s=PROCsecurity("CATALOGUE","MAKE CATALOGUE");
		} else if(!RDAstrcmp(prog,"MKINVCTL"))
		{
			s=PROCsecurity("INVCTL","MAKE INVCTL");
		} else if(!RDAstrcmp(prog,"MKDAT"))
		{
			s=PROCsecurity("DATABASE","MAKE DATABASE");
		} else if(!RDAstrcmp(prog,"MKDMV"))
		{
			s=PROCsecurity("DMVREG","MAKE DMVREG");
		} else if(!RDAstrcmp(prog,"MKEMPABS"))
		{
			s=PROCsecurity("EMPABS","MAKE EMPABS");
		} else if(!RDAstrcmp(prog,"MKFINMGT"))
		{
			s=PROCsecurity("FINMGT","MAKE FINMGT");
		} else if(!RDAstrcmp(prog,"MKFIXASS"))
		{
			s=PROCsecurity("FIXASS","MAKE FIXASS");
		} else if(!RDAstrcmp(prog,"MKGUI"))
		{
			s=PROCsecurity("GUI","MAKE GUI SCREENS");
		} else if(!RDAstrcmp(prog,"MKLVEMGT"))
		{
			s=PROCsecurity("LVEMGT","MAKE LVEMGT");
		} else if(!RDAstrcmp(prog,"MKMISCBILL"))
		{
			s=PROCsecurity("MISCBILL","MAKE MISCBILL");
		} else if(!RDAstrcmp(prog,"MKOCCTAX"))
		{
			s=PROCsecurity("OCCTAX","MAKE OCCTAX");
		} else if(!RDAstrcmp(prog,"MKOLH"))
		{
			s=PROCsecurity("OLHELP","MAKE OLHELP");
		} else if(!RDAstrcmp(prog,"MKPAYROLL"))
		{
			s=PROCsecurity("PAYROLL","MAKE PAYROLL");
		} else if(!RDAstrcmp(prog,"MKPOSTRK"))
		{
			s=PROCsecurity("POSTRK","MAKE POSTRK");
		} else if(!RDAstrcmp(prog,"MKPROPERTY"))
		{
			s=PROCsecurity("PROPERTY","MAKE PROPERTY");
		} else if(!RDAstrcmp(prog,"MKOPENSS"))
		{
			s=PROCsecurity("OPENSS","MAKE OPENSS");
		} else if(!RDAstrcmp(prog,"MKPRSNNL"))
		{
			s=PROCsecurity("PRSNNL","MAKE PRSNNL");
		} else if(!RDAstrcmp(prog,"MKREQMGT"))
		{
			s=PROCsecurity("REQMGT","MAKE REQMGT");
		} else if(!RDAstrcmp(prog,"MKPURORD"))
		{
			s=PROCsecurity("PURORD","MAKE PURORD");
		} else if(!RDAstrcmp(prog,"MKRLSTMGT"))
		{
			s=PROCsecurity("RLSTMGT","MAKE RLSTMGT");
		} else if(!RDAstrcmp(prog,"COMPRLST"))
		{
			s=PROCsecurity("RLSTMGT","COMPUTE REAL ESTATE TAXES");
		} else if(!RDAstrcmp(prog,"RESTSSTM"))
		{
			s=PROCsecurity("RLSTMGT","SET STATEMENT NUMBERS");
		} else if(!RDAstrcmp(prog,"MKRPT"))
		{
			s=PROCsecurity("RPTGEN","MAKE RPTGEN");
		} else if(!RDAstrcmp(prog,"MKSEC"))
		{
			s=PROCsecurity("SECURITY","MAKE SECURITY");
		} else if(!RDAstrcmp(prog,"MKSUBMGT"))
		{
			s=PROCsecurity("SUBMGT","MAKE SUBMGT");
		} else if(!RDAstrcmp(prog,"MKUTLBLL"))
		{
			s=PROCsecurity("UTLBLL","MAKE UTLBLL");
		} else if(!RDAstrcmp(prog,"MKCSHDWR"))
		{
			s=PROCsecurity("CSHDWR","MAKE CSHDWR");
		} else if(!RDAstrcmp(prog,"MKUTL"))
		{
			s=PROCsecurity("UTILITIES","MAKE UTILITIES");
		} else if(!RDAstrcmp(prog,"MKVENPMT"))
		{
			s=PROCsecurity("VENPMT","MAKE VENPMT");
		} else if(!RDAstrcmp(prog,"MKVLTN"))
		{
			s=PROCsecurity("VIOLATION","MAKE VIOLATION");
		} else if(!RDAstrcmp(prog,"MKW2"))
		{
			s=PROCsecurity("PAYROLL","MAKE PAYW2");
		} else if(!RDAstrcmp(prog,"PLAINPAPER"))
		{
			s=PROCsecurity("UTILITIES","PLAINPAPER TEST");
		} else if(!RDAstrcmp(prog,"GETFORMS"))
		{
			s=PROCsecurity("RPTGEN","GET FORMS");
		} else if(!RDAstrcmp(prog,"OCCCGL"))
		{
			s=PROCsecurity("OCCTAX","CREATE OCCTAX GROUP LICENSE NUMBERS");
		} else if(!RDAstrcmp(prog,"REQADD"))
		{
			s=PROCsecurity("REQMGT","APPROVE REQUISITION");
		} else if(!RDAstrcmp(prog,"REQAPP"))
		{
			s=PROCsecurity("REQMGT","CHANGE REQUISITION");
		} else if(!RDAstrcmp(prog,"REQCHG"))
		{
			s=PROCsecurity("REQMGT","CREATE PURCHASE ORDERS");
		} else if(!RDAstrcmp(prog,"REQCPO"))
		{
			s=PROCsecurity("REQMGT","ADD REQUISITION");
		} else if(!RDAstrcmp(prog,"PRPSTT"))
		{
			s=PROCsecurity("PROPERTY","SET STATEMENT NUMBERS");
		} else if(!RDAstrcmp(prog,"PURAPV"))
		{
			s=PROCsecurity("PURORD","ADD PURCHASE ORDER VOUCHERS");
		} else if(!RDAstrcmp(prog,"PURAPVGUSD"))
		{
			s=PROCsecurity("PURORD","ADD PURCHASE ORDER VOUCHERS");
		} else if(!RDAstrcmp(prog,"PURAREC"))
		{
			s=PROCsecurity("PURORD","ADD PO ITEMS RECEIVED");
		} else if(!RDAstrcmp(prog,"PAYBC"))
		{
			s=PROCsecurity("PAYROLL","BENEFITS COORDINATOR");
		} else if(!RDAstrcmp(prog,"PURCPO"))
		{
			s=PROCsecurity("PURORD","CREATE PURCHASE ORDER IDENTIFICATIONS");
		} else if(!RDAstrcmp(prog,"PRTAPO"))
		{
			s=PROCsecurity("PURORD","PRINT PURCHASE ORDERS");
		} else if(!RDAstrcmp(prog,"PRTAPOA"))
		{
			s=PROCsecurity("PURORD","ARCHIVE PURCHASE ORDERS");
		} else if(!RDAstrcmp(prog,"RDAOUT"))
		{
			s=PROCsecurity("UTILITIES","TEXT BROWSER");
		} else if(!RDAstrcmp(prog,"RDADIAG"))
		{
			s=PROCsecurity("UTILITIES","XPERT DIAGNOSTICS");
		} else if(!RDAstrcmp(prog,"XPERTSTP"))
		{
			s=PROCsecurity("UTILITIES","XPERT SETUP");
		} else if(!RDAstrcmp(prog,"MASSDEL"))
		{
			s=PROCsecurity("DATABASE","MASS DELETE FILES");
		} else if(!RDAstrcmp(prog,"ADDMENU"))
		{
			s=PROCsecurity("GUI","ADD MENU OPTION");
		} else if(!RDAstrcmp(prog,"MASSREMDEL"))
		{
			s=PROCsecurity("DATABASE","REMOVE DELETED RECORDS IN MASS");
		} else if(!RDAstrcmp(prog,"CHECKFILE"))
		{
			s=PROCsecurity("DATABASE","CHECK FILE");
		} else if(!RDAstrcmp(prog,"CRTIMPEXP"))
		{
			s=PROCsecurity("DATABASE","CREATE IMPORT/EXPORT");
		} else if(!RDAstrcmp(prog,"MASSIE"))
		{
			s=PROCsecurity("DATABASE","MASS IMPORT/EXPORT");
		} else if(!RDAstrcmp(prog,"CHGENGINE"))
		{
			s=PROCsecurity("DATABASE","CHANGE ENGINE");
		} else if(!RDAstrcmp(prog,"MASSDIFF"))
		{
			s=PROCsecurity("DATABASE","MASSDIFF");
		} else {
		}
		if(prog!=NULL) Rfree(prog);
		if(arg1!=NULL) Rfree(arg1);
		if(arg2!=NULL) Rfree(arg2);
		if(arg3!=NULL) Rfree(arg3);
		if(arg4!=NULL) Rfree(arg4);
		if(arg5!=NULL) Rfree(arg5);
		if(arg6!=NULL) Rfree(arg6);
		if(arg7!=NULL) Rfree(arg7);
		if(arg8!=NULL) Rfree(arg8);
	} else {
		KEYNRD(MENUITEM_FILENO,1);
		prterr("Error:  Menuitem not found for Module [%s] Item Number [%d].",(CURRENT_MODULE!=NULL ? CURRENT_MODULE:""),w);
	}
	return(s);
}
#ifndef WIN32
short ExamineRunningProcesses(char *name,APPlib *args)
{
	short ef=FALSE,r=FALSE;
	int x=0,count=0;
	char *u=NULL,*m=NULL,*p=NULL,delflag=FALSE;
	char *mM=NULL,*pP=NULL,*ed=NULL;

	if(OPENRDA_DATLOG!=(-1))
	{
		ZERNRD(OPENRDA_DATLOG);
		FINDFLDSETSTRING(OPENRDA_DATLOG,"USER IDENTIFICATION",USERLOGIN);
		FINDFLDSETINT(OPENRDA_DATLOG,"LOG NUMBER",INT_MAX-1);
		for(x=0;x<args->numlibs;++x)
		{
			if(!RDAstrcmp(args->libs[x],"-s")) 
			{
			} else if(count==0)
			{
				FINDFLDSETSTRING(OPENRDA_DATLOG,"MODULE",args->libs[x]);
				mM=args->libs[x];
				++count;
			} else if(count>0)
			{
				pP=args->libs[x];
				break;
			}
		}
		ef=LTENRD(OPENRDA_DATLOG,1);
		while(!ef)
		{
			FINDFLDGETSTRING(OPENRDA_DATLOG,"MODULE",&m);
			FINDFLDGETSTRING(OPENRDA_DATLOG,"USER IDENTIFICATION",&u);
			if(RDAstrcmp(m,mM) || RDAstrcmp(u,USERLOGIN)) break;
			FINDFLDGETCHAR(OPENRDA_DATLOG,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(OPENRDA_DATLOG,"PROCESS NAME",&p);
				if(!RDAstrcmp(p,pP))
				{
					FINDFLDGETSTRING(OPENRDA_DATLOG,"END DATE",&ed);
					if(isEMPTY(ed))
					{
						r=TRUE; 
					} else r=FALSE;
					break;
				}
			}
			ef=PRVNRD(OPENRDA_DATLOG,1);
		}
		if(ed!=NULL) Rfree(ed);
		if(m!=NULL) Rfree(m);
		if(p!=NULL) Rfree(p);
		if(u!=NULL) Rfree(u);
	}
	return(r);
}
#endif
void ExecuteOption(int w)
{
	char *prog=NULL,*arg1=NULL,*arg2=NULL,*arg3=NULL,*arg4=NULL,*arg5=NULL;
	char *arg6=NULL,*arg7=NULL,*arg8=NULL;
	char *env_n1=NULL,*env_v1=NULL;
	char *env_n2=NULL,*env_v2=NULL;
	char *env_n3=NULL,*env_v3=NULL;
	char *env_n4=NULL,*env_v4=NULL;
	char *env_n5=NULL,*env_v5=NULL,*name=NULL;
	char temp[512],RestrictedProcess=FALSE;
	APPlib *args=NULL;
	
	if(myMENUITEMS!=NULL)
	{
		fprintf(myMENUITEMS,"\"%s\",%08d\r\n",CURRENT_MODULE,w);
	}
	SIMPLE_SCREENS=FALSE;
	ZERNRD(MENUITEM_FILENO);
	FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
	FINDFLDSETINT(MENUITEM_FILENO,"ITEM NUMBER",w);
	if(LAST_MODULE!=NULL) Rfree(LAST_MODULE);
	LAST_MODULE=stralloc(CURRENT_MODULE);
	LAST_EXE=w;
	if(!EQLNRD(MENUITEM_FILENO,1))
	{
		FINDFLDGETCHAR(MENUITEM_FILENO,"RESTRICTED PROCESS",&RestrictedProcess);
		FINDFLDGETSTRING(MENUITEM_FILENO,"PROGRAM",&prog);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 1",&arg1);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 2",&arg2);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 3",&arg3);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 4",&arg4);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 5",&arg5);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 6",&arg6);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 7",&arg7);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ARGUMENT 8",&arg8);
		FINDFLDGETSTRING(MENUITEM_FILENO,"NAME",&name);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT NAME 1",&env_n1);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT NAME 2",&env_n2);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT NAME 3",&env_n3);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT NAME 4",&env_n4);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT NAME 5",&env_n5);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT VALUE 1",&env_v1);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT VALUE 2",&env_v2);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT VALUE 3",&env_v3);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT VALUE 4",&env_v4);
		FINDFLDGETSTRING(MENUITEM_FILENO,"ENVIRONMENT VALUE 5",&env_v5);
		if(!isEMPTY(env_n1))
		{
			RDA_SetEnv(env_n1,env_v1);
		}
		if(!isEMPTY(env_n2))
		{
			RDA_SetEnv(env_n2,env_v2);
		}
		if(!isEMPTY(env_n3))
		{
			RDA_SetEnv(env_n3,env_v3);
		}
		if(!isEMPTY(env_n4))
		{
			RDA_SetEnv(env_n4,env_v4);
		}
		if(!isEMPTY(env_n5))
		{
			RDA_SetEnv(env_n5,env_v5);
		}
		if(!RDAstrcmp(prog,"RUNPOWERADD") || !RDAstrcmp(prog,"doadd"))
		{
			args=APPlibNEW();
			if(!RDAstrcmp(arg1,"-s"))
			{
				addAPPlib(args,"-s");
			} else {
				if(!isEMPTY(arg1)) addAPPlib(args,arg1);
			}
			if(!isEMPTY(arg2)) addAPPlib(args,arg2);
			if(!isEMPTY(arg3)) addAPPlib(args,arg3);
			if(!isEMPTY(arg4)) addAPPlib(args,arg4);
			if(!isEMPTY(arg5)) addAPPlib(args,arg5);
			if(RestrictedProcess && !IGNORE_RESTRICTED)
			{
#ifndef WIN32
				if(!ExamineRunningProcesses("doadd",args))
				{
					ExecuteProgram("doadd",args);
				} else {
					memset(stemp,0,101);
					sprintf(stemp,"RESTRICTED PROCESS \"%s\"",(name!=NULL ? name:""));
					stoupper(stemp);
					ERRORDIALOG(stemp,"You already have an instance of this restricted process running.  You must wait until it is completed in order to launch another.",NULL,FALSE);
				}
#else
				Execute2Program("doadd",args);
#endif
			} else {
				ExecuteProgram("doadd",args);
			}
			if(args!=NULL) freeapplib(args);
		} else if(!RDAstrcmp(prog,"MULTIPOWERADD") || !RDAstrcmp(prog,"doadds"))
		{
			args=APPlibNEW();
			if(!RDAstrcmp(arg1,"-s"))
			{
				addAPPlib(args,"-s");
			} else {
				if(!isEMPTY(arg1)) addAPPlib(args,arg1);
			}
			if(!isEMPTY(arg2)) addAPPlib(args,arg2);
			if(!isEMPTY(arg3)) addAPPlib(args,arg3);
			if(!isEMPTY(arg4)) addAPPlib(args,arg4);
			if(!isEMPTY(arg5)) addAPPlib(args,arg5);
			if(RestrictedProcess && !IGNORE_RESTRICTED)
			{
#ifndef WIN32
				if(!ExamineRunningProcesses("doadds",args))
				{
					ExecuteProgram("doadds",args);
				} else {
					memset(stemp,0,101);
					sprintf(stemp,"RESTRICTED PROCESS \"%s\"",(name!=NULL ? name:""));
					stoupper(stemp);
					ERRORDIALOG(stemp,"You already have an instance of this restricted process running.  You must wait until it is completed in order to launch another.",NULL,FALSE);
				}
#else
				Execute2Program("doadds",args);
#endif
			} else {
				ExecuteProgram("doadds",args);
			}
			if(args!=NULL) freeapplib(args);
		} else if(!RDAstrcmp(prog,"RUNMAINTAIN"))
		{
			if(!RDAstrcmp(arg1,"ARCHIVE") && !RDAstrcmp(arg2,"MTN ARCHIVES") && !isEMPTY(arg3)) 
			{
				sprintf(temp,"WORK_GROUP=%s",(arg3!=NULL ? arg3:""));
				RDA_PutEnv(temp);	
			}
			args=APPlibNEW();
			if(!isEMPTY(arg1)) addAPPlib(args,arg1);
			if(!isEMPTY(arg2)) addAPPlib(args,arg2);
			if(!isEMPTY(arg3)) addAPPlib(args,arg3);
			if(!isEMPTY(arg4)) addAPPlib(args,arg4);
			if(!isEMPTY(arg5)) addAPPlib(args,arg5);
			if(RestrictedProcess && !IGNORE_RESTRICTED)
			{
#ifndef WIN32
				if(!ExamineRunningProcesses("mtnmst",args))
				{
					ExecuteProgram("mtnmst",args);
				} else {
					memset(stemp,0,101);
					sprintf(stemp,"RESTRICTED PROCESS \"%s\"",(name!=NULL ? name:""));
					stoupper(stemp);
					ERRORDIALOG(stemp,"You already have an instance of this restricted process running.  You must wait until it is completed in order to launch another.",NULL,FALSE);
				}
#else
				Execute2Program("mtnmst",args);
#endif
			} else {
				ExecuteProgram("mtnmst",args);
			}
			if(args!=NULL) freeapplib(args);
		} else if(!RDAstrcmp(prog,"RUNREPORT"))
		{
			args=APPlibNEW();
			if(!isEMPTY(arg1)) addAPPlib(args,arg1);
			if(!isEMPTY(arg2)) addAPPlib(args,arg2);
			if(!isEMPTY(arg3)) addAPPlib(args,arg3);
			if(!isEMPTY(arg4)) addAPPlib(args,arg4);
			if(!isEMPTY(arg5)) addAPPlib(args,arg5);
			if(RestrictedProcess && !IGNORE_RESTRICTED)
			{
#ifndef WIN32
				if(!ExamineRunningProcesses("doreport",args))
				{
					ExecuteProgram("doreport",args);
				} else {
					memset(stemp,0,101);
					sprintf(stemp,"RESTRICTED PROCESS \"%s\"",(name!=NULL ? name:""));
					stoupper(stemp);
					ERRORDIALOG(stemp,"You already have an instance of this restricted process running.  You must wait until it is completed in order to launch another.",NULL,FALSE);
				}
#else
				Execute2Program("doreport",args);
#endif
			} else {
				ExecuteProgram("doreport",args);
			}
			if(args!=NULL) freeapplib(args);
		} else if(!RDAstrcmp(prog,"DOWIKI"))
		{
			OpenRDAWiki(arg1);
		} else if(!RDAstrcmp(prog,"DOHTTP"))
		{
			GUI_OpenURL(arg1);
		} else {
			args=APPlibNEW();
			if(!isEMPTY(arg1)) addAPPlib(args,arg1);
			if(!isEMPTY(arg2)) addAPPlib(args,arg2);
			if(!isEMPTY(arg3)) addAPPlib(args,arg3);
			if(!isEMPTY(arg4)) addAPPlib(args,arg4);
			if(!isEMPTY(arg5)) addAPPlib(args,arg5);
			if(!isEMPTY(arg6)) addAPPlib(args,arg6);
			if(!isEMPTY(arg7)) addAPPlib(args,arg7);
			if(!isEMPTY(arg8)) addAPPlib(args,arg8);
			if(RestrictedProcess && !IGNORE_RESTRICTED)
			{
#ifndef WIN32
				memset(stemp,0,101);
				sprintf(stemp,"%s",prog);
				stolower(stemp);
				if(!ExamineRunningProcesses(stemp,args))
				{
					ExecuteProgram(stemp,args);
				} else {
					memset(stemp,0,101);
					sprintf(stemp,"RESTRICTED PROCESS \"%s\"",(name!=NULL ? name:""));
					stoupper(stemp);
					ERRORDIALOG(stemp,"You already have an instance of this restricted process running.  You must wait until it is completed in order to launch another.",NULL,FALSE);
				}
#else
				memset(stemp,0,101);
				sprintf(stemp,"%s",prog);
				stolower(stemp);
				Execute2Program(stemp,args);
#endif
			} else {
				memset(stemp,0,101);
				sprintf(stemp,"%s",prog);
				stolower(stemp);
				ExecuteProgram(stemp,args);
			}
			if(args!=NULL) freeapplib(args);
		}
		if(name!=NULL) Rfree(name);
		if(prog!=NULL) Rfree(prog);
		if(arg1!=NULL) Rfree(arg1);
		if(arg2!=NULL) Rfree(arg2);
		if(arg3!=NULL) Rfree(arg3);
		if(arg4!=NULL) Rfree(arg4);
		if(arg5!=NULL) Rfree(arg5);
		if(arg6!=NULL) Rfree(arg6);
		if(arg7!=NULL) Rfree(arg7);
		if(arg8!=NULL) Rfree(arg8);
		if(env_n1!=NULL) 
		{
			RDA_UnSetEnv(env_n1);
			Rfree(env_n1);
		}
		if(env_v1!=NULL) Rfree(env_v1);
		if(env_n2!=NULL) 
		{
			RDA_UnSetEnv(env_n2);
			Rfree(env_n2);
		}
		if(env_v2!=NULL) Rfree(env_v2);
		if(env_n3!=NULL) 
		{
			RDA_UnSetEnv(env_n3);
			Rfree(env_n3);
		}
		if(env_v3!=NULL) Rfree(env_v3);
		if(env_n4!=NULL) 
		{
			RDA_UnSetEnv(env_n4);
			Rfree(env_n4);
		}
		if(env_v4!=NULL) Rfree(env_v4);
		if(env_n5!=NULL) 
		{
			RDA_UnSetEnv(env_n5);
			Rfree(env_n5);
		}
		if(env_v5!=NULL) Rfree(env_v5);
	} else {
		KEYNRD(MENUITEM_FILENO,1);
		prterr("Error:  Menuitem not found for Module [%s] Item Number [%d].",(CURRENT_MODULE!=NULL ? CURRENT_MODULE:""),w);
	}
}
void ExecuteOptionCycle(int w)
{
	ExecuteOption(w);
}
void ExecuteDockRSRC(RDArmem *member)
{
	ExecuteOption(member->items);
}
Wt::WPopupMenu *MySubMenu(Wt::WWidget *Parent_Widget,short ddl,int p,char *namex)
{
	Wt::WPopupMenu *pop=NULL,*pop1=NULL;
	char *mod=NULL,DID_STUFF=FALSE,*name=NULL,delflag=FALSE;
	char *vis_exp=NULL;
	short ddl1=0,style=0;
	int itemNumber=0,p1=0,a=0;
	Wt::WString *c=NULL;
	Wt::WWidget *label=NULL;
	Wt::WMenuItem *WMi=NULL;
	short ef=0,count=0;


	pop=new Wt::WPopupMenu();
	pop1=(Wt::WWidget *)Parent_Widget;
	pop1->addMenu(namex,(Wt::WMenu *)pop);
	ZERNRD(MENUITEM_FILENO);
	FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
	FINDFLDSETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",ddl);
	FINDFLDSETINT(MENUITEM_FILENO,"PARENT",p);
	ef=GTENRD(MENUITEM_FILENO,2);
	while(!ef)
	{
		FINDFLDGETSTRING(MENUITEM_FILENO,"MODULE NAME",&mod);
		FINDFLDGETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",&ddl1);
		FINDFLDGETINT(MENUITEM_FILENO,"PARENT",&p1);
		if(RDAstrcmp(mod,CURRENT_MODULE) || ddl!=ddl1 || p!=p1) break;
		FINDFLDGETCHAR(MENUITEM_FILENO,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(MENUITEM_FILENO,"ITEM NUMBER",&itemNumber);
			FINDFLDGETSTRING(MENUITEM_FILENO,"NAME",&name);
			FINDFLDGETSHORT(MENUITEM_FILENO,"STYLE",&style);
			if(HasSecurity(itemNumber) || style!=0)
			{
			FINDFLDGETSTRING(MENUITEM_FILENO,"VISIBLE EXPRESSION",&vis_exp);
			if(EVALUATEbol(vis_exp,SCRNvirtualSubData,DockRSRC) || isEMPTY(vis_exp))
			{
			switch(style)
			{
				default:
				case 0: /* Action */
					DID_STUFF=TRUE;
							
					pop->addItem(name)->triggered().connect(boost::bind(&ExecuteOption,itemNumber));
					WMi=pop->items().at(count);
					if(itemNumber==LAST_EXE)
					{
						WMi->setChecked(TRUE);
					} else {
						WMi->setChecked(FALSE);
					}
					break;
				case 1: /* Anchor */
					pop1=MySubMenu((Wt::WWidget *)pop,ddl,itemNumber,name);
					pop1->setDisabled(FALSE);
					break;
				case 2: /* Heading - Label */
					c=new Wt::WString(name);
					pop->addSectionHeader(*c);
					c->~WString();	
					pop->addSeparator();
					break;
			}
			++count;
			ZERNRD(MENUITEM_FILENO);
			FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
			FINDFLDSETINT(MENUITEM_FILENO,"ITEM NUMBER",itemNumber);
			EQLNRD(MENUITEM_FILENO,1);
			}
			}
		}
		ef=NXTNRD(MENUITEM_FILENO,2);
	}
	if(mod!=NULL) Rfree(mod);
	if(name!=NULL) Rfree(name);
	pop->setDisabled((DID_STUFF ? TRUE:FALSE));
	return(pop);
}
Wt::WPopupMenu *ModuleDDL(short ddl)
{
	Wt::WPopupMenu *WHICH=NULL;
	Wt::WPopupMenu *pop1=NULL;
	char *mod=NULL,DID_STUFF=FALSE,*name=NULL,delflag=FALSE;
	char *vis_exp=NULL;
	short ddl1=0,style=0,count=0;
	int itemNumber=0,p=0,p1=0,a=0;
	Wt::WString *c=NULL;
	Wt::WWidget *label=NULL,*pop=NULL;
	Wt::WMenuItem *WMi=NULL;
	short ef=0;

	WHICH=new Wt::WPopupMenu();
	ZERNRD(MENUITEM_FILENO);
	FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
	FINDFLDSETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",ddl);
	FINDFLDSETINT(MENUITEM_FILENO,"PARENT",p);
	ef=GTENRD(MENUITEM_FILENO,2);
	while(!ef)
	{
		FINDFLDGETSTRING(MENUITEM_FILENO,"MODULE NAME",&mod);
		FINDFLDGETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",&ddl1);
		FINDFLDGETINT(MENUITEM_FILENO,"PARENT",&p1);
		if(RDAstrcmp(mod,CURRENT_MODULE) || ddl!=ddl1 || p!=p1) break;
		FINDFLDGETCHAR(MENUITEM_FILENO,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(MENUITEM_FILENO,"ITEM NUMBER",&itemNumber);
			FINDFLDGETSTRING(MENUITEM_FILENO,"NAME",&name);
			FINDFLDGETSHORT(MENUITEM_FILENO,"STYLE",&style);
			if(HasSecurity(itemNumber) || style!=0)
			{
			FINDFLDGETSTRING(MENUITEM_FILENO,"VISIBLE EXPRESSION",&vis_exp);
			if(EVALUATEbol(vis_exp,SCRNvirtualSubData,DockRSRC) || isEMPTY(vis_exp))
			{
			switch(style)
			{
				default:
				case 0: /* Action */
					WHICH->addItem(name)->triggered().connect(boost::bind(&ExecuteOption,itemNumber));
					DID_STUFF=TRUE;
					WMi=WHICH->items().at(count);
					if(itemNumber==LAST_EXE)
					{
						WMi->setChecked(TRUE);
					} else {
						WMi->setChecked(FALSE);
					}
					break;
				case 1: /* Anchor */
					pop1=MySubMenu((Wt::WWidget *)WHICH,ddl,itemNumber,name);
					DID_STUFF=TRUE;
					pop1->setDisabled(FALSE);
					break;
				case 2: /* Heading - Label */
					c=new Wt::WString(name);
					WHICH->addSectionHeader(*c);
					c->~WString();	
					WHICH->addSeparator();
					break;
			}
			++count;
			ZERNRD(MENUITEM_FILENO);
			FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
			FINDFLDSETINT(MENUITEM_FILENO,"ITEM NUMBER",itemNumber);
			EQLNRD(MENUITEM_FILENO,1);
			}
			}
		}
		ef=NXTNRD(MENUITEM_FILENO,2);
	}
	if(mod!=NULL) Rfree(mod);
	if(name!=NULL) Rfree(name);
	if(!DID_STUFF)
	{
		if(WHICH!=NULL) WHICH->~WMenu();
		WHICH=NULL;
	}
	return(WHICH);
}
void CreateDockRSRC(char *modname)
{
	char *mod=NULL,*name=NULL,delflag=FALSE,*temp=NULL;
	short ddl=5,ddl1=0,style=0,count=0;
	int itemNumber=0,p1=0,p=0;
	Wt::WString *c=NULL;
	int psize=0;
	short ef=0;
	RDArmem *mem=NULL;
	Wt::WLink *WK=NULL;
	Wt::WContainerWidget *h=NULL,*v=NULL,*SC=NULL;
	Wt::WVBoxLayout *vbox=NULL,*vb=NULL;
	Wt::WHBoxLayout *hbox=NULL;
	Wt::WText *text=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WLength spc;

	if(DockRSRC!=NULL)
	{
		free_rsrc(DockRSRC);
		DockRSRC=NULL;
	} 
	if(MainWindowDock!=NULL)
	{
		MainWindowDock->clear();
		Reports_P=NULL;
		Query_P=NULL;
		QueryPop=NULL;
		Reports=NULL;
	}
	if(!RDAstrcmp(modname,"HOME")) return;
	memset(stemp,0,101);
	sprintf(stemp,"%s DOCK WINDOW",(!isEMPTY(modname) ? modname:"FINMGT"));
	count=0;
	DockRSRC=RDArsrcNEW(CURRENT_MODULE,stemp);

/* Module Description */
	spc=Wt::WLength(2,Wt::WLength::Pixel);
	MainWindowDock->setPadding(spc,All);

	ZERNRD(MODULE_FILENO);
	FINDFLDSETSTRING(MODULE_FILENO,"MODULE NAME",modname);
	GTENRD(MODULE_FILENO,2);
	FINDFLDGETSTRING(MODULE_FILENO,"DESCRIPTION",&temp);

	h=new Wt::WContainerWidget();
	MainWindowDock->addWidget(h);
	hbox=new Wt::WHBoxLayout();	
	daL=(Wt::WLayout *)hbox;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	h->setPadding(spc,All);
	h->setLayout(hbox);
	text=new Wt::WText();
	text->setText(temp);
	h->addWidget(text);

/* Query and Reports Container*/	
	h=new Wt::WContainerWidget();
	hbox=new Wt::WHBoxLayout();	
	h->setLayout(hbox);
	daL=(Wt::WLayout *)hbox;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	h->setPadding(spc,All);
	MainWindowDock->addWidget(h);
	h->setPadding(spc,All);

/* Query */
	Query_P=new Wt::WPushButton((Wt::WContainerWidget *)h);
	hbox->addWidget(Query_P);
	Query_P->setDefault(FALSE);
	c=new Wt::WString("Query");
	Query_P->setText(*c);
	c->~WString();
	QueryPop=ModuleDDL(0);
	if(QueryPop==NULL)
	{
		Query_P->setEnabled(FALSE);
	} else {
		QueryPop->addStyleClass("OpenRDA ResourceBar Query WPopupMenu");
		Query_P->setMenu(QueryPop);
		Query_P->setEnabled(TRUE);
	}

/* Reports */
	Reports_P=new Wt::WPushButton((Wt::WContainerWidget *)h);
	hbox->addWidget(Reports_P);
	Reports_P->setDefault(FALSE);
	c=new Wt::WString("Reports");
	Reports_P->setText(*c);
	c->~WString();
	Reports=ModuleDDL(1);
	if(Reports==NULL)
	{
		Reports_P->setEnabled(FALSE);
	} else {
		Reports->addStyleClass("OpenRDA ResourceBar Reports WPopupMenu");
		Reports_P->setMenu(Reports);
		Reports_P->setEnabled(TRUE);
	}

/* MainDockWindow Elements */
	SC=new Wt::WContainerWidget();
	SC->setPadding(spc,All);
	MainWindowDock->addWidget(SC);


	ZERNRD(MENUITEM_FILENO);
	FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
	FINDFLDSETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",ddl);
	FINDFLDSETINT(MENUITEM_FILENO,"PARENT",p);
	ef=GTENRD(MENUITEM_FILENO,2);
	while(!ef)
	{
		FINDFLDGETSTRING(MENUITEM_FILENO,"MODULE NAME",&mod);
		FINDFLDGETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",&ddl1);
		FINDFLDGETINT(MENUITEM_FILENO,"PARENT",&p1);
		if(RDAstrcmp(mod,CURRENT_MODULE) || ddl!=ddl1 || p!=p1) break;
		FINDFLDGETCHAR(MENUITEM_FILENO,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(MENUITEM_FILENO,"ITEM NUMBER",&itemNumber);
			FINDFLDGETSTRING(MENUITEM_FILENO,"NAME",&name);
			FINDFLDGETSHORT(MENUITEM_FILENO,"STYLE",&style);
			if(style==0)
			{
				if(HasSecurity(itemNumber))
				{
					RMEMaddbtnrsrc(DockRSRC,name,TRUE,ExecuteDockRSRC,NULL);
					++count;
					mem=DockRSRC->rscs+(DockRSRC->numrscs-1);
					mem->items=itemNumber;
				}
			} else {
				prterr("Error:  Style must be Action for DockWindow [%d][%s] ",itemNumber,name);
			}
		}
		ef=NXTNRD(MENUITEM_FILENO,2);
	}
	if(mod!=NULL) Rfree(mod);
	if(name!=NULL) Rfree(name);
	QN2IT_PARENT_TYPE=1;
	QN2IT_PARENT=(Wt::WContainerWidget *)SC;
	
	if(ADVmakescrn(DockRSRC,FALSE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the Dock Window.  Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
	}
	SC->resize(300,Wt::WLength::Auto);
	QN2IT_PARENT_TYPE=0;
}
void CreateCenterPanel()
{
	Wt::WTabWidget *tabW=NULL;
	Wt::WMenuItem *tab=NULL;
	Wt::WContainerWidget *w=NULL;
	Wt::WHBoxLayout *hbox=NULL;

	ExpendActShowTable=FALSE;
	ExpendActValuetoVisual=0;
	RevenueActShowTable=FALSE;
	RevenueActValuetoVisual=0;
	JournalEntryShowTable=FALSE;
	JournalEntryValuetoVisual=0;
	ReceiptShowTable=FALSE;
	ReceiptValuetoVisual=0;
	GenLdgrActShowTable=FALSE;
	GenLdgrActValuetoVisual=0;
	VoucherStatusValuetoVisual=0;
	VoucherStatusShowTable=FALSE;
	OpenPObyVendorValuetoVisual=1;
	OpenPObyVendorShowTable=FALSE;
	if(MainWindowCenter->count()>0) MainWindowCenter->clear();
	if(!RDAstrcmp(CURRENT_MODULE,"FINMGT"))
	{
		tabW=new Wt::WTabWidget(MainWindowCenter);
		w=UnPostedExpenditureActivity();
		tabW->addTab(w,"Exp Activity",Wt::WTabWidget::PreLoading);
		w=UnPostedRevenueActivity();
		tabW->addTab(w,"Rev Activity",Wt::WTabWidget::PreLoading);
		w=UnPostedGeneralLedgerActivity();
		tabW->addTab(w,"B/S Activity",Wt::WTabWidget::PreLoading);
		w=UnProcessedJournalEntry();
		tabW->addTab(w,"J/E",Wt::WTabWidget::PreLoading);
		w=UnProcessedReceipt();
		tabW->addTab(w,"Receipts",Wt::WTabWidget::PreLoading);
	} else if(!RDAstrcmp(CURRENT_MODULE,"VENPMT"))
	{
		tabW=new Wt::WTabWidget(MainWindowCenter);
		w=UnProcessedVoucherStatus();
		tabW->addTab(w,"Vouchers",Wt::WTabWidget::PreLoading);
	} else if(!RDAstrcmp(CURRENT_MODULE,"PURORD"))
	{
		tabW=new Wt::WTabWidget(MainWindowCenter);
		w=OpenPObyVendorDisplayDashBoard();
		tabW->addTab(w,"Open PO's by Vendor",Wt::WTabWidget::PreLoading);
	} else if(!RDAstrcmp(CURRENT_MODULE,"HOME"))
	{
/*
		tabW=new Wt::WTabWidget(MainWindowCenter);
		w=CreateFeed("http://www.openrda.com/feed/");
		tabW->addTab(w,"Blogs",Wt::WTabWidget::PreLoading);
		tabW->addTab(w,"News",Wt::WTabWidget::PreLoading);
*/
		hbox=new Wt::WHBoxLayout();
		MainWindowCenter->setLayout(hbox);
		w=CreateFeed("http://www.openrda.net/OpenRDALandingPage.php");
		hbox->addWidget(w,500);
	}
}
void ChooseModule(char *modname)
{
	char *temp=NULL;
	short x=0;

	LAST_EXE=(-1);
	if(!isEMPTY(modname))
	{
		if(CURRENT_MODULE!=NULL) Rfree(CURRENT_MODULE);
		if(!RDAstrcmp(modname,"VW")) CURRENT_MODULE=stralloc("PAYROLL");
		else if(!RDAstrcmp(modname,"BUDPAY")) CURRENT_MODULE=stralloc("BUDPREP");
		else if(!RDAstrcmp(modname,"W2")) CURRENT_MODULE=stralloc("PAYROLL");
		else if(!RDAstrcmp(modname,"1099")) CURRENT_MODULE=stralloc("VENPMT");
		else if(!RDAstrcmp(modname,"TOOLS")) CURRENT_MODULE=stralloc("UTILITIES");
		else if(!RDAstrcmp(modname,"VERSION")) CURRENT_MODULE=stralloc("UTILITIES");
		else if(RDAstrstr(modname,"RLST")) CURRENT_MODULE=stralloc("RLSTMGT"); 
		else if(RDAstrstr(modname,"PROP")) CURRENT_MODULE=stralloc("PROPERTY");
		else if(RDAstrstr(modname,"IRMS")) CURRENT_MODULE=stralloc("VAIRMS");
		else if(RDAstrstr(modname,"DMV")) CURRENT_MODULE=stralloc("DMVREG");
		else if(RDAstrstr(modname,"MISCB")) CURRENT_MODULE=stralloc("MISCBILL");
		else if(RDAstrstr(modname,"UTLBLL")) CURRENT_MODULE=stralloc("UTLBLL");
		else CURRENT_MODULE=stralloc(modname);
		ZERNRD(MODULE_FILENO);
		FINDFLDSETSTRING(MODULE_FILENO,"MODULE NAME",modname);
		GTENRD(MODULE_FILENO,2);
		FINDFLDGETSTRING(MODULE_FILENO,"DESCRIPTION",&temp);
		CreateDockRSRC(modname);
		if(!OPENRDA_STYLE) CreateStatusBar();
			else CreateAccordionBar();
		CreateCenterPanel();
	}
}
