/* xpert setup functions */
#include<xpert.hpp>

XPERTstp *XPERT_SETUP=NULL;

short xgetXPERTbin(XPERTstp *x,char *liboverride,int line,char *file)
{
	short y,test=0;
	int test2=0;
	BIN *bin;
	char *libname=NULL;
	short version=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG getXPERTbin Reading Xpert Setup Binary at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(isEMPTY(liboverride))
	{
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
		sprintf(libname,"%s/rda/XPERT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\XPERT.STP",CURRENTDIRECTORY);	
#endif
	} else {
		libname=Rmalloc(RDAstrlen(liboverride)+12);
#ifndef WIN32
		sprintf(libname,"%s/XPERT.STP",liboverride);	
#endif
#ifdef WIN32
		sprintf(libname,"%s\\XPERT.STP",liboverride);	
#endif
	}
	bin=getlibbin(libname,"XPERT SETUP",TRUE);
	if(bin==NULL) return(-1);
	y=BINgetshort(bin);
	if(y!=(-1683) && y!=(-1684) && y!=(-1685) && y!=(-1686) && y!=(-1687) && y!=(-1688) && y!=(-1689) && y!=(-1690) && 
 	 	y!=(-1691) && y!=(-1692) && y!=(-1693) && y!=(-1694) && 
		y!=(-1695) && y!=(-1696) && y!=(-1697) && y!=(-1698) && 
		y!=(-1699) && y!=(-1701) && y!=(-1700))
	{
		prterr("Error Invalid version or bad binary for [XPERT SETUP] in [%s] at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(y==(-1683)) version=19;
	else if(y==(-1684)) version=18;
	else if(y==(-1685)) version=17;
	else if(y==(-1686)) version=16;
	else if(y==(-1687)) version=15;
	else if(y==(-1688)) version=14;
	else if(y==(-1689)) version=13;
	else if(y==(-1690)) version=12;
	else if(y==(-1691)) version=11;
	else if(y==(-1692)) version=10;
	else if(y==(-1693)) version=9;
	else if(y==(-1694)) version=8;
	else if(y==(-1695)) version=7;
	else if(y==(-1696)) version=6;
	else if(y==(-1697)) version=5;
	else if(y==(-1698)) version=4;
	else if(y==(-1699)) version=3;
	else if(y==(-1700)) version=2;
	else if(y==(-1701)) version=1;

	if(libname!=NULL) Rfree(libname);
	test=BINgetshort(bin);
	if(test) x->company_name=BINgetstring(bin);
		else x->company_name=NULL;
	test=BINgetshort(bin);
	if(test) x->addr1=BINgetstring(bin);
		else x->addr1=NULL;
	test=BINgetshort(bin);
	if(test) x->addr2=BINgetstring(bin);
		else x->addr2=NULL;
	test=BINgetshort(bin);
	if(test) x->city=BINgetstring(bin);
		else x->city=NULL;
	test=BINgetshort(bin);
	if(test) x->state=BINgetstring(bin);
		else x->state=NULL;
	test=BINgetshort(bin);
	if(test) x->zip=BINgetstring(bin);
		else x->zip=NULL;
	test=BINgetshort(bin);
	if(test) x->phone=BINgetstring(bin);
		else x->phone=NULL;
	if(version>2)
	{
		test=BINgetshort(bin);
		if(test) x->fax=BINgetstring(bin);
			else x->fax=NULL;
		test=BINgetshort(bin);
		if(test) x->use_century_threshold=TRUE;
			else x->use_century_threshold=FALSE;
		x->century_threshold=BINgetshort(bin);
	} else {
		x->fax=NULL;
		x->century_threshold=0;
		x->use_century_threshold=FALSE;
	}
	if(version>1)
	{
		test=BINgetshort(bin);
		x->UTILITIES=(char)test;
		test=BINgetshort(bin);
		x->FINMGT=(char)test;
		test=BINgetshort(bin);
		x->BNKREC=(char)test;
		test=BINgetshort(bin);
		x->PRSNNL=(char)test;
		test=BINgetshort(bin);
		x->IIDMGT=(char)test;
		test=BINgetshort(bin);
		x->APPMGT=(char)test;
		test=BINgetshort(bin);
		x->POSTRK=(char)test;
		test=BINgetshort(bin);
		x->PAYROLL=(char)test;
		test=BINgetshort(bin);
		x->SUBMGT=(char)test;
		test=BINgetshort(bin);
		x->LVEMGT=(char)test;
		test=BINgetshort(bin);
		x->EMPABS=(char)test;
		test=BINgetshort(bin);
		x->BFTMGT=(char)test;
		test=BINgetshort(bin);
		x->PAYENC=(char)test;
		test=BINgetshort(bin);
		x->VENPMT=(char)test;
		test=BINgetshort(bin);
		x->PURORD=(char)test;
		test=BINgetshort(bin);
		x->CATALOGUE=(char)test;
		test=BINgetshort(bin);
		x->REQMGT=(char)test;
		test=BINgetshort(bin);
		x->FIXASS=(char)test;
		test=BINgetshort(bin);
		x->INVCTL=(char)test;
		test=BINgetshort(bin);
		x->OCCTAX=(char)test;
		test=BINgetshort(bin);
		x->BUDPREP=(char)test;
		test=BINgetshort(bin);
		x->MISCBILL=(char)test;
		test=BINgetshort(bin);
		x->TAXCLT=(char)test;
		test=BINgetshort(bin);
		x->RLSTMGT=(char)test;
		test=BINgetshort(bin);
		x->PPTMGT=(char)test;
		test=BINgetshort(bin);
		x->STUDFIN=(char)test;
		test=BINgetshort(bin);
		x->VEHMTN=(char)test;
		test=BINgetshort(bin);
		x->WRKORD=(char)test;
	}
	if(version>3)
	{
		test=BINgetshort(bin);
		x->TOOLS=(char)test;
		test=BINgetshort(bin);
		x->TRANSACTIONS=(char)test;
		test=BINgetshort(bin);
		x->EXTENSIONS=(char)test;
		test2=BINgetint(bin);
		x->software_type=(int)test2;
	} else {
		x->TOOLS=TRUE;
		x->TRANSACTIONS=TRUE;
		x->EXTENSIONS=TRUE;
		x->software_type=0;
	}
	if(version>4)
	{
		test=BINgetshort(bin);
		x->SECURITYEXTENSIONS=(char)test;
	} else {
		x->SECURITYEXTENSIONS=TRUE;
	}
	if(version>5)
	{
		test=BINgetshort(bin);
		x->RLVEMGT=(char)test;
	} else {
		x->RLVEMGT=FALSE;
	}
	if(version>6)
	{
		test=BINgetshort(bin);
		x->PROPERTY=(char)test;
		test=BINgetshort(bin);
		x->VIOLATION=(char)test;
	} else {
		x->PROPERTY=FALSE;
		x->VIOLATION=FALSE;
	}
	if(version>7)
	{
		test=BINgetshort(bin);
		x->UTLBLL=(char)test;
	} else {
		x->UTLBLL=FALSE;
	}
	if(version>8)
	{
		test=BINgetshort(bin);
		x->DMVREG=(char)test;
	} else {
		if(x->PPTMGT) x->DMVREG=TRUE;
			else x->DMVREG=FALSE;
	}
	if(version>9)
	{
		test=BINgetshort(bin);
		x->LVSIMP=(char)test;
	} else {
		if(x->LVEMGT) x->LVSIMP=TRUE;
			else x->LVSIMP=FALSE;
	}
	if(version>10)
	{
		test=BINgetshort(bin);
		x->CSHDWR=(char)test;
		test=BINgetshort(bin);
		x->RREQMGT=(char)test;
		test=BINgetshort(bin);
		x->BLDPRMT=(char)test;
	} else {
		x->CSHDWR=FALSE;
		x->RREQMGT=FALSE;
		x->BLDPRMT=FALSE;
	}
	if(version>14)
	{
		test=BINgetshort(bin);
		x->CITWEB=(char)test;
	} else {
		x->CITWEB=FALSE;
	}
	if(version>15)
	{
		test=BINgetshort(bin);
		x->OPENSOURCE=(char)test;
	} else {
		x->OPENSOURCE=FALSE;
	}
	if(version>11)
	{
		test=BINgetshort(bin);
		x->SimpleMenu=(char)test;
		x->OrgType=BINgetshort(bin);
	} else {
		if(x->software_type==2) x->SimpleMenu=TRUE;
			else x->SimpleMenu=FALSE;
		x->OrgType=0;
	}
	if(version>12)
	{
		test=BINgetshort(bin);
		x->ShowDefineListNew=(char)test;
		x->NoUsers=BINgetint(bin);
		test=BINgetshort(bin);
		x->UseEndDate=(char)test;
		test=BINgetshort(bin);
		if(test) x->EndDate=BINgetstring(bin);
			else x->EndDate=NULL;
		test=BINgetshort(bin);
		if(test) x->ProgDir=BINgetstring(bin);
			else x->ProgDir=NULL;
		test=BINgetshort(bin);
		if(test) x->DataDir=BINgetstring(bin);
			else x->DataDir=NULL;
	} else {
		x->ShowDefineListNew=FALSE;
		x->NoUsers=0;
		x->UseEndDate=FALSE;
		x->EndDate=NULL;
		x->ProgDir=NULL;
		x->DataDir=NULL;
	}
	if(version>13)
	{
		test=BINgetshort(bin);
		if(test) x->SoftwareKey=BINgetstring(bin);
			else x->SoftwareKey=NULL;
	} else {
		x->SoftwareKey=NULL;
	}
	if(version>16)
	{
		test=BINgetshort(bin);
		if(test) x->ARCHIVE=TRUE;
			else x->ARCHIVE=FALSE;
	} else {
		x->ARCHIVE=FALSE;
	}
	if(version>17)
	{
		test=BINgetshort(bin);
		if(test) x->CIS=TRUE;
			else x->CIS=FALSE;
		test=BINgetshort(bin);
		if(test) x->ASP=TRUE;
			else x->ASP=FALSE;
		test=BINgetshort(bin);
		if(test) x->BARCODING=TRUE;
			else x->BARCODING=FALSE;
		test=BINgetshort(bin);
		if(test) x->CREDITCARDS=TRUE;
			else x->CREDITCARDS=FALSE;
		test=BINgetshort(bin);
		if(test) x->FOODSVC=TRUE;
			else x->FOODSVC=FALSE;
		test=BINgetshort(bin);
		if(test) x->ROLLACCT=TRUE;
			else x->ROLLACCT=FALSE;
		x->AccountingMethod=BINgetshort(bin);
	} else {
		x->CIS=FALSE;
		x->ASP=FALSE;
		x->BARCODING=FALSE;
		x->CREDITCARDS=FALSE;
		x->FOODSVC=FALSE;
		x->ROLLACCT=FALSE;
		x->AccountingMethod=0;
	}
	if(version>18)
	{
		test=BINgetshort(bin);
		if(test) x->HRM=TRUE;
			else x->HRM=FALSE;
	} else x->HRM=FALSE;
	BINfree(bin);
	return(version);
}
short xwriteXPERTbin(XPERTstp *x,char *liboverride,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;
	short test=0;
	int test2=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG writeXPERTbin Writing [XPERT SETUP] Binary to Binary Library [XPERT.STP] at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(isEMPTY(liboverride))
	{
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
		sprintf(libname,"%s/rda/XPERT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\XPERT.STP",CURRENTDIRECTORY);	
#endif
	} else {
		libname=Rmalloc(RDAstrlen(liboverride)+12);
#ifndef WIN32
		sprintf(libname,"%s/XPERT.STP",liboverride);	
#endif
#ifdef WIN32
		sprintf(libname,"%s\\XPERT.STP",liboverride);	
#endif
	}
	bin=BINnew();
	BINaddshort(bin,(-1683));	

	if(x->company_name!=NULL && RDAstrlen(x->company_name)>0)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->company_name);
	} else BINaddshort(bin,FALSE);
	if(x->addr1!=NULL && RDAstrlen(x->addr1)>0)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->addr1);
	} else BINaddshort(bin,FALSE);
	if(x->addr2!=NULL && RDAstrlen(x->addr2)>0)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->addr2);
	} else BINaddshort(bin,FALSE);
	if(x->city!=NULL && RDAstrlen(x->city)>0)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->city);
	} else BINaddshort(bin,FALSE);
	if(x->state!=NULL && RDAstrlen(x->state)>0)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->state);
	} else BINaddshort(bin,FALSE);
	if(x->zip!=NULL && RDAstrlen(x->zip)>0)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->zip);
	} else BINaddshort(bin,FALSE); 
	if(x->phone!=NULL && RDAstrlen(x->phone)>0)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->phone);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(x->fax))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->fax);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,(x->use_century_threshold ? TRUE:FALSE));
	BINaddshort(bin,x->century_threshold);

	test=(short)x->UTILITIES;
	BINaddshort(bin,test);
	test=(short)x->FINMGT;
	BINaddshort(bin,test);
	test=(short)x->BNKREC;
	BINaddshort(bin,test);
	test=(short)x->PRSNNL;
	BINaddshort(bin,test);
	test=(short)x->IIDMGT;
	BINaddshort(bin,test);
	test=(short)x->APPMGT;
	BINaddshort(bin,test);
	test=(short)x->POSTRK;
	BINaddshort(bin,test);
	test=(short)x->PAYROLL;
	BINaddshort(bin,test);
	test=(short)x->SUBMGT;
	BINaddshort(bin,test);
	test=(short)x->LVEMGT;
	BINaddshort(bin,test);
	test=(short)x->EMPABS;
	BINaddshort(bin,test);
	test=(short)x->BFTMGT;
	BINaddshort(bin,test);
	test=(short)x->PAYENC;
	BINaddshort(bin,test);
	test=(short)x->VENPMT;
	BINaddshort(bin,test);
	test=(short)x->PURORD;
	BINaddshort(bin,test);
	test=(short)x->CATALOGUE;
	BINaddshort(bin,test);
	test=(short)x->REQMGT;
	BINaddshort(bin,test);
	test=(short)x->FIXASS;
	BINaddshort(bin,test);
	test=(short)x->INVCTL;
	BINaddshort(bin,test);
	test=(short)x->OCCTAX;
	BINaddshort(bin,test);
	test=(short)x->BUDPREP;
	BINaddshort(bin,test);
	test=(short)x->MISCBILL;
	BINaddshort(bin,test);
	test=(short)x->TAXCLT;
	BINaddshort(bin,test);
	test=(short)x->RLSTMGT;
	BINaddshort(bin,test);
	test=(short)x->PPTMGT;
	BINaddshort(bin,test);
	test=(short)x->STUDFIN;
	BINaddshort(bin,test);
	test=(short)x->VEHMTN;
	BINaddshort(bin,test);
	test=(short)x->WRKORD;
	BINaddshort(bin,test);
	test=(short)x->TOOLS;
	BINaddshort(bin,test);
	test=(short)x->TRANSACTIONS;
	BINaddshort(bin,test);
	test=(short)x->EXTENSIONS;
	BINaddshort(bin,test);
	test2=(int)x->software_type;
	BINaddint(bin,test2);
	test=(short)x->SECURITYEXTENSIONS;
	BINaddshort(bin,test);
	test=(short)x->RLVEMGT;
	BINaddshort(bin,test);
	test=(short)x->PROPERTY;
	BINaddshort(bin,test);
	test=(short)x->VIOLATION;
	BINaddshort(bin,test);
	test=(short)x->UTLBLL;
	BINaddshort(bin,test);
	test=(short)x->DMVREG;
	BINaddshort(bin,test);
	test=(short)x->LVSIMP;
	BINaddshort(bin,test);
	test=(short)x->CSHDWR;
	BINaddshort(bin,test);
	test=(short)x->RREQMGT;
	BINaddshort(bin,test);
	test=(short)x->BLDPRMT;
	BINaddshort(bin,test);
	test=(short)x->CITWEB;
	BINaddshort(bin,test);
	test=(short)x->OPENSOURCE;
	BINaddshort(bin,test);
	test=(short)x->SimpleMenu;
	BINaddshort(bin,test);
	BINaddshort(bin,x->OrgType);
	test=(short)x->ShowDefineListNew;
	BINaddshort(bin,test);
	BINaddint(bin,x->NoUsers);
	test=(short)x->UseEndDate;
	BINaddshort(bin,test);
	if(!isEMPTY(x->EndDate))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->EndDate);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(x->ProgDir))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->ProgDir);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(x->DataDir))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->DataDir);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(x->SoftwareKey))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,x->SoftwareKey);
	} else BINaddshort(bin,FALSE);
	test=(short)x->ARCHIVE;
	BINaddshort(bin,test);

	test=(short)x->CIS;
	BINaddshort(bin,test);
	test=(short)x->ASP;
	BINaddshort(bin,test);
	test=(short)x->BARCODING;
	BINaddshort(bin,test);
	test=(short)x->CREDITCARDS;
	BINaddshort(bin,test);
	test=(short)x->FOODSVC;
	BINaddshort(bin,test);
	test=(short)x->ROLLACCT;
	BINaddshort(bin,test);
	BINaddshort(bin,x->AccountingMethod);
	test=(short)x->HRM;
	BINaddshort(bin,test);
	if(writelibbin(libname,bin,"XPERT SETUP"))
	{
		prterr("Error Can't write Xpert Setup binary [%s] into library [%s] at line [%d] program [%s].","XPERT SETUP",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
XPERTstp *xXPERTstpNEW(int line,char *file)
{
	XPERTstp *x=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG XPERTstpNEW Creating Structure for Xpert Setup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=Rmalloc(sizeof(XPERTstp));
	x->company_name=NULL;
	x->addr1=NULL;
	x->addr2=NULL;
	x->city=NULL;
	x->state=NULL;
	x->zip=NULL;
	x->phone=NULL;
	x->fax=NULL;
	x->use_century_threshold=FALSE;
	x->century_threshold=0;
	x->TOOLS=TRUE;
	x->TRANSACTIONS=TRUE;
	x->EXTENSIONS=TRUE;
	x->software_type=0;
	x->SimpleMenu=FALSE;
	x->OrgType=0;
	x->ShowDefineListNew=FALSE;
	x->NoUsers=0;
	x->AccountingMethod=0;
	x->UseEndDate=FALSE;
	x->EndDate=NULL;
	x->ProgDir=NULL;
	x->DataDir=NULL;
	x->SoftwareKey=NULL;
	return(x);
}
void xfree_xpert(XPERTstp *x,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG free_xpert Freeing Xpert Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(x!=NULL)
	{
		if(x->company_name!=NULL) Rfree(x->company_name);
		if(x->addr1!=NULL) Rfree(x->addr1);
		if(x->addr2!=NULL) Rfree(x->addr2);
		if(x->city!=NULL) Rfree(x->city);
		if(x->state!=NULL) Rfree(x->state);
		if(x->zip!=NULL) Rfree(x->zip);
		if(x->phone!=NULL) Rfree(x->phone);
		if(x->fax!=NULL) Rfree(x->fax);
		if(x->EndDate!=NULL) Rfree(x->EndDate);
		if(x->ProgDir!=NULL) Rfree(x->ProgDir);
		if(x->DataDir!=NULL) Rfree(x->DataDir);
		if(x->SoftwareKey!=NULL) Rfree(x->SoftwareKey);
		Rfree(x);
	}
}
