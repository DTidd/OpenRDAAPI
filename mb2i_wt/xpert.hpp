#ifndef XPERT_H
#define XPERT_H
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <misc.hpp>

struct XPERTstps
{
	char *company_name;
	char *addr1;
	char *addr2;
	char *city;
	char *state;
	char *zip;
	char *phone;
	char *fax;
	char use_century_threshold;
	short century_threshold;
	char UTILITIES;
	char FINMGT;
	char BNKREC;
	char PRSNNL;
	char IIDMGT;
	char APPMGT;
	char POSTRK;
	char PAYROLL;
	char SUBMGT;
	char LVEMGT;
	char EMPABS;
	char BFTMGT;
	char PAYENC;
	char VENPMT;
	char PURORD;
	char CATALOGUE;
	char REQMGT;
	char FIXASS;
	char INVCTL;
	char OCCTAX;
	char BUDPREP;
	char MISCBILL;
	char TAXCLT;
	char RLSTMGT;
	char PPTMGT;
	char STUDFIN;
	char VEHMTN;
	char WRKORD;
	char TOOLS;
	char TRANSACTIONS;
	char EXTENSIONS;
	int software_type;
	char SECURITYEXTENSIONS;
	char RLVEMGT;
	char VIOLATION;
	char PROPERTY;
	char UTLBLL;
	char DMVREG;
	char LVSIMP;
	char CSHDWR;
	char RREQMGT;
	char BLDPRMT;
	char CITWEB;
	char OPENSOURCE;
	char ARCHIVE;
	char CIS;
	char ASP;
	char BARCODING;
	char CREDITCARDS;
	char FOODSVC;
	char ROLLACCT;
	char HRM;
	char SimpleMenu;
	short OrgType;
	short AccountingMethod;
	short ShowDefineListNew;
	int NoUsers;
	short UseEndDate;
	char *EndDate;
	char *ProgDir;
	char *DataDir;
	char *SoftwareKey;
};
typedef struct XPERTstps XPERTstp;

extern XPERTstp *XPERT_SETUP;
#define getXPERTbin(x)	xgetXPERTbin(x,NULL,__LINE__,__FILE__)
#define getXPERTbinADV(x,y)	xgetXPERTbin(x,y,__LINE__,__FILE__)
short xgetXPERTbin(XPERTstp *,char *,int,char *);
#define writeXPERTbin(x)	xwriteXPERTbin(x,NULL,__LINE__,__FILE__)
#define writeXPERTbinADV(x,y)	xwriteXPERTbin(x,y,__LINE__,__FILE__)
short xwriteXPERTbin(XPERTstp *,char *,int,char *);
#define XPERTstpNEW()	xXPERTstpNEW(__LINE__,__FILE__)
XPERTstp *xXPERTstpNEW(int,char *);
#define free_xpert(x)	{xfree_xpert(x,__LINE__,__FILE__);x=NULL;}
void xfree_xpert(XPERTstp *,int,char *);
#endif

