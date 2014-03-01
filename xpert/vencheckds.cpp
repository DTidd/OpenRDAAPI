/* vencheckds.c - Vendor Check Double Stub */
#ifndef WIN32
#define __NAM__ "vencheckds.lnx"
#endif
#ifdef WIN32
#define __NAM__ "vencheckds.exe"
#endif
#include <app.hpp>

#include <nrd.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define BODY_COUNT	10

char *module="VENPMT",*bank_id=NULL,*output_device=NULL;
char extra_formfeed=FALSE,use_check_offset=FALSE;
static char PRINT_COLUMN_HEADERS=FALSE,select_reissued=FALSE;
short venyr=(-1),vennum=(-1),invnum=(-1),vchnum=(-1),venadr=(-1);
short eaccnum=(-1),raccnum=(-1),baccnum=(-1),banknum=(-1);
/*
APPlib *BANKids=NULL;
static void getBANKids(RDArsrc *),changeBANKids(RDArsrc *);
*/
static RDAvirtual *VendorAddr1=NULL,*VendorAddr2=NULL,*VendorAddr3=NULL;
static short ADDRESS_OFFSET=0,ACCDESC_TYPE=0,CHECK_OFFSET=0;
static short SIGNATURE_TYPE=0,SHOW_INVOICE_DATE=0;

void quitfunc(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) 
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	mainrsrc=NULL;
	if(eaccnum!=(-1)) CLSNRD(eaccnum);
	if(raccnum!=(-1)) CLSNRD(raccnum);
	if(baccnum!=(-1)) CLSNRD(baccnum);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(venyr!=(-1)) CLSNRD(venyr);
	if(invnum!=(-1)) CLSNRD(invnum);
	if(vchnum!=(-1)) CLSNRD(vchnum);
	if(banknum!=(-1)) CLSNRD(banknum);
	if(venadr!=(-1)) CLSNRD(venadr);
	if(output_device!=NULL) Rfree(output_device);
	ShutdownSubsystems();
	exit(0);
}
static void SetPrintedFlag(short sfile)
{
	short ef=0;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	int x=0;
	RDATData *prev=NULL;

	key=KEYNUM(vchnum,1);
	ZERNRD(sfile);
	ef=FRSNRD(sfile,1);
	while(!ef)
	{
		ZERNRD(vchnum);
		if(key!=NULL)
		{
			for(x=0,part=key->part;x<key->numparts;++x,++part)
			{
				COPYFIELD(sfile,vchnum,part->name);
			}
		}
		if(!EQLNRD(vchnum,1)) KEYNRD(vchnum,1);
		prev=RDATDataNEW(vchnum);
		FINDFLDSETCHAR(vchnum,"PRINTED",TRUE);	
		WRTTRANS(vchnum,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		ef=NXTNRD(sfile,1);
	}
}
void makeDBsortfile(DBsort *dbsrt)
{
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	key=KEYNUM(vchnum,1);
	addDBfield(dbsrt,"CHECK NUMBER",LONGV,8);
	addDBkeypart(dbsrt,keyno,"CHECK NUMBER");
	addDBfield(dbsrt,"VIRTUAL CHECK NUMBER",LONGV,8);
	addDBkeypart(dbsrt,keyno,"VIRTUAL CHECK NUMBER");
	addDBfield(dbsrt,"COUNTER",LONGV,6);
	addDBkeypart(dbsrt,keyno,"COUNTER");
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(vchnum,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
		}
	}
	addDBfield(dbsrt,"DESCRIPTION",1,40);
	addDBfield(dbsrt,"CHECK DATE",DATES,10);
	addDBfield(dbsrt,"AMOUNT",2,20);
	addDBfield(dbsrt,"PO IDENTIFICATION",1,15);
	addDBfield(dbsrt,"ACCOUNT CODE",1,40);
	addDBfield(dbsrt,"INVOICE DATE",DATES,10);
	addDBfield(dbsrt,"VENDOR NAME",1,40);
	addDBfield(dbsrt,"VENDOR IDENTIFICATION",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 1",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 2",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 3",1,40);
	addDBfield(dbsrt,"CUSTOMER NUMBER",1,40);
	addDBfield(dbsrt,"DEFAULT CHECK DETAIL",10,1);
}
void addsortrecord(RDArsrc *mainrsrc,DBsort *dbsrt)
{
	char deleteflag=FALSE;
	NRDkey *key=NULL;
	NRDpart *part;
	int x;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	key=KEYNUM(vchnum,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(vchnum,dbsrt->fileno,part->name);
		}
	}
	COPYFIELD(vchnum,dbsrt->fileno,"CHECK NUMBER");
	COPYFIELD(vchnum,dbsrt->fileno,"VIRTUAL CHECK NUMBER");
	COPYFIELD(vchnum,dbsrt->fileno,"DESCRIPTION");
	COPYFIELD(vchnum,dbsrt->fileno,"AMOUNT");
	COPYFIELD(vchnum,dbsrt->fileno,"CHECK DATE");
	COPYFIELD(vchnum,dbsrt->fileno,"PO IDENTIFICATION");
	COPYFIELD(vchnum,dbsrt->fileno,"ACCOUNT CODE");
	COPYFIELD(invnum,dbsrt->fileno,"INVOICE DATE");
	COPYFIELD(vennum,dbsrt->fileno,"VENDOR NAME");
	COPYFIELD(vennum,dbsrt->fileno,"VENDOR IDENTIFICATION");
	COPYFIELD(venadr,dbsrt->fileno,"CUSTOMER NUMBER");
	COPYFIELD(vennum,dbsrt->fileno,"DEFAULT CHECK DETAIL");
	if(VendorAddr1!=NULL)
	{
		VendorAddr1->computed=FALSE;
		computevirtual(VendorAddr1,SCRNvirtualSubData,mainrsrc);
		FINDFLDSETSTRING(dbsrt->fileno,"VENDOR ADDRESS 1",VendorAddr1->value.string_value);
	}
	if(VendorAddr2!=NULL)
	{
		VendorAddr2->computed=FALSE;
		computevirtual(VendorAddr2,SCRNvirtualSubData,mainrsrc);
		FINDFLDSETSTRING(dbsrt->fileno,"VENDOR ADDRESS 2",VendorAddr2->value.string_value);
	}
	if(VendorAddr3!=NULL)
	{
		VendorAddr3->computed=FALSE;
		computevirtual(VendorAddr3,SCRNvirtualSubData,mainrsrc);
		FINDFLDSETSTRING(dbsrt->fileno,"VENDOR ADDRESS 3",VendorAddr3->value.string_value);
	}
	WRTNRD(dbsrt->fileno,1,"COUNTER");
}
static void RepeatStub(short sfile,int checkno,int vcheckno,double total_amt,RDA_PFILE *fp)
{
	short ef=0,count=0;
	int checkno1=0,vcheckno1=0;
	char *poid=NULL,*invid=NULL,*acc=NULL,*desc=NULL;
	char *emp_name=NULL,*checkdate=NULL,*invoicedate=NULL;
	char *emp_id=NULL,*custno=NULL;
	char dcd=FALSE;
	double amt=0.0;

	ZERNRD(sfile);
	FINDFLDSETINT(sfile,"CHECK NUMBER",checkno);
	FINDFLDSETINT(sfile,"VIRTUAL CHECK NUMBER",vcheckno);
	ef=GTENRD(sfile,1);
	while(!ef)
	{
		FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno1);
		FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno1);
		if(checkno!=checkno1 || vcheckno!=vcheckno1) break;
		FINDFLDGETDOUBLE(sfile,"AMOUNT",&amt);
		FINDFLDGETSTRING(sfile,"PO IDENTIFICATION",&poid);
		if(RDAstrlen(poid)>15) poid[15]=0;
		FINDFLDGETSTRING(sfile,"INVOICE IDENTIFICATION",&invid);
		if(RDAstrlen(invid)>15) invid[15]=0;
		FINDFLDGETSTRING(sfile,"ACCOUNT CODE",&acc);
		FINDFLDGETSTRING(sfile,"DESCRIPTION",&desc);
		FINDFLDGETSTRING(sfile,"CHECK DATE",&checkdate);
		FINDFLDGETSTRING(sfile,"INVOICE DATE",&invoicedate);
		FINDFLDGETSTRING(sfile,"CUSTOMER NUMBER",&custno);
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&dcd);
		if(RDAstrlen(checkdate)>10) checkdate[10]=0;
		if(RDAstrlen(invoicedate)>10) invoicedate[10]=0;
		if(count==0)
		{
			FINDFLDGETSTRING(sfile,"VENDOR NAME",&emp_name);
			if(RDAstrlen(emp_name)>40) emp_name[40]=0;
			RDA_fprintf(fp,"Check: %8d Date: %-10s Vendor: %s\r\n",checkno,(checkdate!=NULL ? checkdate:""),(emp_name!=NULL ? emp_name:""));
			if(emp_name!=NULL) Rfree(emp_name);
			FINDFLDGETSTRING(sfile,"VENDOR IDENTIFICATION",&emp_id);
			if(RDAstrlen(emp_id)>40) emp_id[40]=0;
			RDA_fprintf(fp,"                              Vendor ID: %s\r\n",(emp_id!=NULL ? emp_id:""));
			if(emp_id!=NULL) Rfree(emp_id);
			if(dcd==FALSE)
			{
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"     SEE ATTACHED LIST FOR CHECK DETAIL\r\n");
				count+=2;
			}
			if(PRINT_COLUMN_HEADERS && dcd==TRUE)
			{
			switch(ACCDESC_TYPE)
			{
				case 0:
					RDA_fprintf(fp,"%-15s %-15s %-36s %10s\r\n",
						(SHOW_INVOICE_DATE==0 ? "PO ID":"INVOICE DATE"),
						"INVOICE","ACCOUNT CODE","AMOUNT");
					break;	
				case 1:
					RDA_fprintf(fp,"%-15s %-15s %-36s %10s\r\n",
						(SHOW_INVOICE_DATE==0 ? "PO ID":"INVOICE DATE"),
						"INVOICE","DESCRIPTION","AMOUNT");
					break;
				default:
				case 2:	
					if(RDAstrlen(acc)>20) acc[20]=0;
					if(RDAstrlen(desc)>15) acc[15]=0;
					RDA_fprintf(fp,"%-15s %-15s %-20s %-15s %10s\r\n",
						(SHOW_INVOICE_DATE==0 ? "PO ID":"INVOICE DATE"),
						"INVOICE","ACCOUNT CODE","DESCRIPTION","AMOUNT");
					break;
			}
			}
		}
		if(dcd==TRUE)
		{
		switch(ACCDESC_TYPE)
		{
			case 0:
				if(RDAstrlen(acc)>36) acc[36]=0;
				if(SHOW_INVOICE_DATE==0)
				{
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(poid!=NULL ? poid:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),amt/100);
				} else {
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(invoicedate!=NULL ? invoicedate:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),amt/100);
				}
				break;	
			case 1:
				if(RDAstrlen(desc)>36) desc[36]=0;
				if(SHOW_INVOICE_DATE==0)
				{
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(poid!=NULL ? poid:""),(invid!=NULL ? invid:""),
						(desc!=NULL ? desc:""),amt/100);
				} else {
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(invoicedate!=NULL ? invoicedate:""),(invid!=NULL ? invid:""),
						(desc!=NULL ? desc:""),amt/100);
				}
				break;
			default:
			case 2:	
				if(RDAstrlen(acc)>20) acc[20]=0;
				if(RDAstrlen(desc)>15) desc[15]=0;
				if(SHOW_INVOICE_DATE==0)
				{
					RDA_fprintf(fp,"%-15s %-15s %-20s %-15s %10.02f\r\n",
						(poid!=NULL ? poid:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),(desc!=NULL ? desc:""),amt/100);
				} else {
					RDA_fprintf(fp,"%-15s %-15s %-20s %-15s %10.02f\r\n",
						(invoicedate!=NULL ? invoicedate:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),(desc!=NULL ? desc:""),amt/100);
				}
				break;
		}
		}
		if(poid!=NULL) Rfree(poid);
		if(invid!=NULL) Rfree(invid);
		if(acc!=NULL) Rfree(acc);
		if(checkdate!=NULL) Rfree(checkdate);
		if(invoicedate!=NULL) Rfree(invoicedate);
		if(desc!=NULL) Rfree(desc);
		if(dcd==TRUE) ++count;
		ef=NXTNRD(sfile,1);
	}
	while(count<BODY_COUNT+5)
	{
		RDA_fprintf(fp,"\r\n");	
		++count;
	}
	if(checkno==vcheckno)
	{
		RDA_fprintf(fp,"Customer No.: %-40s  Check Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
	} else {
		RDA_fprintf(fp,"Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
	}
	if(custno!=NULL) Rfree(custno);
	while(count<BODY_COUNT+7)
	{
		RDA_fprintf(fp,"\r\n");	
		++count;
	}
}
static void OutputChecks(short sfile)
{
	short ef=0,count=0;
	int checkno=0,vcheckno=0,last_checkno=0,last_vcheckno=0,x=0;
	RDA_PFILE *fp=NULL;
	double total_amt=0.0,amt=0.0;
	char *emp_name=NULL,*emp_add1=NULL,*emp_add2=NULL,*emp_add3=NULL;
	char *emp_id=NULL,*custno=NULL;
	char *spelled_amt=NULL,*checkdate=NULL,*poid=NULL,*invid=NULL;
	char *acc=NULL,*desc=NULL,*invoicedate=NULL;
	char *tempstr=NULL,dcd=FALSE;
	short prtsig=1;

	if(isEMPTY(output_device))
	{
		prterr("Error:  An appropriate output device must be selected to print checks.");
		return;
	}
	fp=RDA_popen(output_device,"w");
	SelectPrintTypeByName(output_device,"PCL JOB HEADER",fp);
	if(SIGNATURE_TYPE==4)
	{
		SelectPrintTypeByName(output_device,"LOAD VENPMT SIGNATURE",fp);
	}
	SelectPrintTypeByName(output_device,"SIMPLEX",fp);
	SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
	if(fp==NULL)
	{
		prterr("Error:  Couldn't open output device [%s].",output_device);
	}
	if(extra_formfeed)
	{
		RDA_fprintf(fp,"\f");
	}
	ZERNRD(sfile);
	ef=FRSNRD(sfile,1);
	FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno);
	FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno);
	last_checkno=checkno;
	last_vcheckno=vcheckno;
	while(!ef)
	{
		FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno);
		FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno);
		if(checkno!=last_checkno || vcheckno!=last_vcheckno)
		{
			if(use_check_offset)
			{
				for(x=0;x<CHECK_OFFSET;x++)
				{
					RDA_fprintf(fp,"\r\n");	
				}
			}
			while(count<BODY_COUNT+5)
			{
				RDA_fprintf(fp,"\r\n");	
				++count;
			}
			if(last_checkno==last_vcheckno)
			{
				RDA_fprintf(fp,"Customer No.: %-40s  Check Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
			} else {
				RDA_fprintf(fp,"Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
			}
			if(custno!=NULL) Rfree(custno);
			while(count<BODY_COUNT+7)
			{
				RDA_fprintf(fp,"\r\n");	
				++count;
			}
/* repeat stub and print negotiable */
			RepeatStub(sfile,last_checkno,last_vcheckno,total_amt,fp);
/* negotiable check */
			if(!PRINT_COLUMN_HEADERS)
			{
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			}
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			if(checkno!=last_checkno)
			{
				spelled_amt=spell(total_amt);
				RDA_fprintf(fp,"  %s\r\n",spelled_amt);
				if(spelled_amt!=NULL) Rfree(spelled_amt);
				RDA_fprintf(fp,"\r\n");
				tempstr=famtpad(total_amt,15,'*');
				RDA_fprintf(fp,"%43s %-10s  %8d %-16s\r\n",
					"",(checkdate!=NULL ? checkdate:""),last_vcheckno,(tempstr!=NULL?tempstr:""));
				if(tempstr!=NULL) Rfree(tempstr);
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"\r\n");
				prtsig=1;
			} else {
				RDA_fprintf(fp,"  *** V O I D **** V O I D **** V O I D **** \r\n");
				RDA_fprintf(fp,"\r\n");
				tempstr=famtpad(0,16,'*');
				RDA_fprintf(fp,"%43s %-10s  %8d %-16s\r\n",
					"",(checkdate!=NULL ? checkdate:""),last_vcheckno,(tempstr!=NULL?tempstr:""));
				if(tempstr!=NULL) Rfree(tempstr);
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"\r\n");
				prtsig=0;
			}
			RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_name!=NULL ? emp_name:""));
			RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add1!=NULL ? emp_add1:""));
			RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add2!=NULL ? emp_add2:""));
			RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add3!=NULL ? emp_add3:""));
			if(SIGNATURE_TYPE==4)
			{
				SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
				if(prtsig) { SelectPrintTypeByName(output_device,"VENPMT SIGNATURE",fp); }
				SelectPrintTypeByName(output_device,"POP CURSOR",fp);
			}
			RDA_fprintf(fp,"\f");
			count=0;	
			if(checkno!=last_checkno)
			{
				total_amt=0;
			}
			last_checkno=checkno;
			last_vcheckno=vcheckno;
		}
/* print 1st detail stub */
		FINDFLDGETDOUBLE(sfile,"AMOUNT",&amt);
		FINDFLDGETSTRING(sfile,"PO IDENTIFICATION",&poid);
		if(RDAstrlen(poid)>15) poid[15]=0;
		FINDFLDGETSTRING(sfile,"INVOICE IDENTIFICATION",&invid);
		if(RDAstrlen(invid)>15) invid[15]=0;
		FINDFLDGETSTRING(sfile,"ACCOUNT CODE",&acc);
		FINDFLDGETSTRING(sfile,"DESCRIPTION",&desc);

		FINDFLDGETSTRING(sfile,"CHECK DATE",&checkdate);
		if(RDAstrlen(checkdate)>10) checkdate[10]=0;
		FINDFLDGETSTRING(sfile,"INVOICE DATE",&invoicedate);
		if(RDAstrlen(invoicedate)>10) invoicedate[10]=0;
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&dcd);
		FINDFLDGETSTRING(sfile,"CUSTOMER NUMBER",&custno);
		FINDFLDGETSTRING(sfile,"VENDOR NAME",&emp_name);
		if(RDAstrlen(emp_name)>40) emp_name[40]=0;
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 1",&emp_add1);
		if(RDAstrlen(emp_add1)>40) emp_add1[40]=0;
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 2",&emp_add2);
		if(RDAstrlen(emp_add2)>40) emp_add2[40]=0;
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 3",&emp_add3);
		if(RDAstrlen(emp_add3)>40) emp_add3[40]=0;
		if(count==0)
		{
			RDA_fprintf(fp,"Check: %8d Date: %-10s Vendor: %s\r\n",checkno,(checkdate!=NULL ? checkdate:""),(emp_name!=NULL ? emp_name:""));
			FINDFLDGETSTRING(sfile,"VENDOR IDENTIFICATION",&emp_id);
			if(RDAstrlen(emp_id)>40) emp_id[40]=0;
			RDA_fprintf(fp,"                              Vendor ID: %s\r\n",(emp_id!=NULL ? emp_id:""));
			if(dcd==FALSE)
			{
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"     SEE ATTACHED LIST FOR CHECK DETAIL\r\n");
				count+=2;
			}
			if(PRINT_COLUMN_HEADERS && dcd==TRUE)
			{
			switch(ACCDESC_TYPE)
			{
				case 0:
					RDA_fprintf(fp,"%-15s %-15s %-36s %10s\r\n",
						(SHOW_INVOICE_DATE==0 ? "PO ID":"INVOICE DATE"),
						"INVOICE","ACCOUNT CODE","AMOUNT");
					break;	
				case 1:
					RDA_fprintf(fp,"%-15s %-15s %-36s %10s\r\n",
						(SHOW_INVOICE_DATE==0 ? "PO ID":"INVOICE DATE"),
						"INVOICE","DESCRIPTION","AMOUNT");
					break;
				default:
				case 2:	
					RDA_fprintf(fp,"%-15s %-15s %-20s %-15s %10s\r\n",
						(SHOW_INVOICE_DATE==0 ? "PO ID":"INVOICE DATE"),
						"INVOICE","ACCOUNT CODE","DESCRIPTION","AMOUNT");
					break;
			}
			}
		}
		if(dcd==TRUE)
		{
		switch(ACCDESC_TYPE)
		{
			case 0:
				if(RDAstrlen(acc)>36) acc[36]=0;
				if(SHOW_INVOICE_DATE==0)
				{
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(poid!=NULL ? poid:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),amt/100);
				} else {
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(invoicedate!=NULL ? invoicedate:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),amt/100);
				}
				break;	
			case 1:
				if(RDAstrlen(desc)>36) desc[36]=0;
				if(SHOW_INVOICE_DATE==0)
				{
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(poid!=NULL ? poid:""),(invid!=NULL ? invid:""),
						(desc!=NULL ? desc:""),amt/100);
				} else {
					RDA_fprintf(fp,"%-15s %-15s %-36s %10.02f\r\n",
						(invoicedate!=NULL ? invoicedate:""),(invid!=NULL ? invid:""),
						(desc!=NULL ? desc:""),amt/100);
				}
				break;
			default:
			case 2:	
				if(RDAstrlen(acc)>20) acc[20]=0;
				if(RDAstrlen(desc)>15) desc[15]=0;
				if(SHOW_INVOICE_DATE==0)
				{
					RDA_fprintf(fp,"%-15s %-15s %-20s %-15s %10.02f\r\n",
						(poid!=NULL ? poid:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),(desc!=NULL ? desc:""),amt/100);
				} else {
					RDA_fprintf(fp,"%-15s %-15s %-20s %-15s %10.02f\r\n",
						(invoicedate!=NULL ? invoicedate:""),(invid!=NULL ? invid:""),
						(acc!=NULL ? acc:""),(desc!=NULL ? desc:""),amt/100);
				}
				break;
		}
		}
		if(poid!=NULL) Rfree(poid);
		if(invid!=NULL) Rfree(invid);
		if(acc!=NULL) Rfree(acc);
		if(desc!=NULL) Rfree(desc);
		if(dcd==TRUE) ++count;
		total_amt+=amt;
		ef=NXTNRD(sfile,1);
	}
	if(use_check_offset)
	{
		for(x=0;x<CHECK_OFFSET;x++)
		{
			RDA_fprintf(fp,"\r\n");	
		}
	}
	while(count<BODY_COUNT+5)
	{
		RDA_fprintf(fp,"\r\n");	
		++count;
	}
	if(last_checkno==last_vcheckno)
	{
		RDA_fprintf(fp,"Customer No.: %-40s  Check Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
	} else {
		RDA_fprintf(fp,"Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
	}
	if(custno!=NULL) Rfree(custno);
	while(count<BODY_COUNT+7)
	{
		RDA_fprintf(fp,"\r\n");	
		++count;
	}
/* repeat stub and print negotiable */
	RepeatStub(sfile,last_checkno,last_vcheckno,total_amt,fp);
/* negotiable check */
	if(!PRINT_COLUMN_HEADERS)
	{
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	}
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	spelled_amt=spell(total_amt);
	RDA_fprintf(fp,"  %s\r\n",spelled_amt);
	if(spelled_amt!=NULL) Rfree(spelled_amt);
	RDA_fprintf(fp,"\r\n");
	tempstr=famtpad(total_amt,15,'*');
	prtsig=1;
	RDA_fprintf(fp,"%43s %-10s  %8d %-16s\r\n",
		"",(checkdate!=NULL ? checkdate:""),last_vcheckno,(tempstr!=NULL?tempstr:""));
	if(tempstr!=NULL) Rfree(tempstr);
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_name!=NULL ? emp_name:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add1!=NULL ? emp_add1:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add2!=NULL ? emp_add2:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add3!=NULL ? emp_add3:""));
	if(SIGNATURE_TYPE==4)
	{
		SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
		if(prtsig) { SelectPrintTypeByName(output_device,"VENPMT SIGNATURE",fp); }
		SelectPrintTypeByName(output_device,"POP CURSOR",fp);
	}
	RDA_fprintf(fp,"\f");
	count=0;	
	if(checkno!=last_checkno)
	{
		total_amt=0;
	}
	last_checkno=checkno;
	last_vcheckno=vcheckno;
	if(SIGNATURE_TYPE==4)
	{
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_200",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_201",fp);
	}
	SelectPrintTypeByName(output_device,"PCL JOB FOOTER",fp);
	if(checkdate!=NULL) Rfree(checkdate);
	if(invoicedate!=NULL) Rfree(invoicedate);
	RDA_pclose(fp);
}
void setcheck(RDArsrc *mainrsrc)
{
	short accttype=0,ef=0,increment_diagnostic=FALSE;
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*vendorid=NULL,*last_vendorid=NULL;
	char *bankid1=NULL;
	RDArsrc *tmprsrc=NULL;
	int x=0,cnum=0;
	RDAvirtual *v;
	char manual_voucher=FALSE,deleteflag=FALSE,cleared=FALSE;
	char verified=FALSE,electronic=FALSE,re=FALSE;
	DFvirtual *d=NULL;

	d=getDFvirtual("VENPMT","VENDOR ADDRESS 1");
	if(d!=NULL)
	{
		VendorAddr1=Rmalloc(sizeof(RDAvirtual));
		VendorAddr1->name=stralloc(d->name);
		VendorAddr1->when=0;
		VendorAddr1->len=d->length;
		VendorAddr1->type=d->type;
		if(d->expression!=NULL) 
		{
			VendorAddr1->expression=PP_translate(d->expression);
		} else {
			VendorAddr1->expression=NULL;
		}
		VendorAddr1->computed=FALSE;
		VendorAddr1->range=FALSE;
		VendorAddr1->dlen=0;
		switch(VendorAddr1->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				VendorAddr1->value.string_value=NULL;
				VendorAddr1->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				VendorAddr1->value.string_value=Rmalloc(1);
				*VendorAddr1->value.string_value=0;
				VendorAddr1->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				VendorAddr1->value.float_value=Rmalloc(sizeof(double));
				*VendorAddr1->value.float_value=0;
				VendorAddr1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				VendorAddr1->value.short_value=Rmalloc(sizeof(short));
				*VendorAddr1->value.short_value=0;
				VendorAddr1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				VendorAddr1->value.integer_value=Rmalloc(sizeof(int));
				*VendorAddr1->value.integer_value=0;
				VendorAddr1->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",VendorAddr1->type,VendorAddr1->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("VENPMT","VENDOR ADDRESS 2");
	if(d!=NULL)
	{
		VendorAddr2=Rmalloc(sizeof(RDAvirtual));
		VendorAddr2->name=stralloc(d->name);
		VendorAddr2->when=0;
		VendorAddr2->len=d->length;
		VendorAddr2->type=d->type;
		if(d->expression!=NULL) 
		{
			VendorAddr2->expression=PP_translate(d->expression);
		} else {
			VendorAddr2->expression=NULL;
		}
		VendorAddr2->computed=FALSE;
		VendorAddr2->range=FALSE;
		VendorAddr2->dlen=0;
		switch(VendorAddr2->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				VendorAddr2->value.string_value=NULL;
				VendorAddr2->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				VendorAddr2->value.string_value=Rmalloc(1);
				*VendorAddr2->value.string_value=0;
				VendorAddr2->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				VendorAddr2->value.float_value=Rmalloc(sizeof(double));
				*VendorAddr2->value.float_value=0;
				VendorAddr2->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				VendorAddr2->value.short_value=Rmalloc(sizeof(short));
				*VendorAddr2->value.short_value=0;
				VendorAddr2->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				VendorAddr2->value.integer_value=Rmalloc(sizeof(int));
				*VendorAddr2->value.integer_value=0;
				VendorAddr2->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",VendorAddr2->type,VendorAddr2->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("VENPMT","VENDOR ADDRESS 3");
	if(d!=NULL)
	{
		VendorAddr3=Rmalloc(sizeof(RDAvirtual));
		VendorAddr3->name=stralloc(d->name);
		VendorAddr3->when=0;
		VendorAddr3->len=d->length;
		VendorAddr3->type=d->type;
		if(d->expression!=NULL) 
		{
			VendorAddr3->expression=PP_translate(d->expression);
		} else {
			VendorAddr3->expression=NULL;
		}
		VendorAddr3->computed=FALSE;
		VendorAddr3->range=FALSE;
		VendorAddr3->dlen=0;
		switch(VendorAddr3->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				VendorAddr3->value.string_value=NULL;
				VendorAddr3->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				VendorAddr3->value.string_value=Rmalloc(1);
				*VendorAddr3->value.string_value=0;
				VendorAddr3->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				VendorAddr3->value.float_value=Rmalloc(sizeof(double));
				*VendorAddr3->value.float_value=0;
				VendorAddr3->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				VendorAddr3->value.short_value=Rmalloc(sizeof(short));
				*VendorAddr3->value.short_value=0;
				VendorAddr3->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				VendorAddr3->value.integer_value=Rmalloc(sizeof(int));
				*VendorAddr3->value.integer_value=0;
				VendorAddr3->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",VendorAddr3->type,VendorAddr3->name);
				break;
		}
		FreeDFvirtual(d);
	}
	readallwidgets(mainrsrc);
	/*
	if(banknum!=(-1))
	{
		if(BANKids->numlibs>0)
		{
			FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&selbankid);
			if(RDAstrcmp(BANKids->libs[selbankid],"No Banks Defined."))
			{
				bank_id=stralloc(BANKids->libs[selbankid]);
			}
		} 
	}
	*/
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&output_device);
	FINDRSCGETCHAR(mainrsrc,"FORMFEED",&extra_formfeed);
	FINDRSCGETCHAR(mainrsrc,"USE CHECK OFFSET",&use_check_offset);
	FINDRSCGETCHAR(mainrsrc,"SELECT REISSUED",&select_reissued);
	sortname=unique_name();
	sortfile=DBsortNEW("VENPMT",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(vchnum,rs);
	ReadRangeScreen(invnum,rs);
	ReadRangeScreen(vennum,rs);
	ReadRangeScreen(venyr,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(vchnum,"DIAGNOSTIC SCREEN",module,
		"Selecting Vendor Vouchers",NULL);
	if(tmprsrc!=NULL)
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	ef=ADVBEGNRDsec(vchnum,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(vchnum,"SOURCE USER VERIFIED",&verified);
		FINDFLDGETCHAR(vchnum,"CLEARED",&cleared);
		FINDFLDGETCHAR(vchnum,"MANUAL VOUCHER",&manual_voucher);
		FINDFLDGETCHAR(vchnum,"ELECTRONIC TRANSFER",&electronic);
		FINDFLDGETCHAR(vchnum,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(vchnum,"REISSUED CHECK",&re);
		FINDFLDGETINT(vchnum,"CHECK NUMBER",&cnum);
		/*
		if(verified && !cleared && !manual_voucher && !electronic && !deleteflag && cnum!=0 && ((re==FALSE || select_reissued==TRUE)))
		*/
		if(verified && !manual_voucher && !electronic && !deleteflag && cnum!=0 && (!cleared || (re==TRUE && select_reissued==TRUE)))
		{
		if(banknum!=(-1))
		{
			if(bank_id!=NULL && RDAstrlen(bank_id)>0)
			{
				FINDFLDGETSTRING(vchnum,"BANK IDENTIFICATION",&bankid1);
			}
		}
		if(!RDAstrcmp(bank_id,bankid1))
		{
			FINDFLDGETSHORT(vchnum,"ACCOUNT TYPE",&accttype);
			switch(accttype)
			{
				case 0: /* Expenditures */
					COPYFIELD(vchnum,eaccnum,"DEFINITION TYPE");
					COPYFIELD(vchnum,eaccnum,"ACCOUNT CODE");
					if(ADVEQLNRDsec(eaccnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(eaccnum,1);
					ZERNRD(raccnum);
					ZERNRD(baccnum);
					break;
				case 1: /* Revenue */
					COPYFIELD(vchnum,raccnum,"DEFINITION TYPE");
					COPYFIELD(vchnum,raccnum,"ACCOUNT CODE");
					if(ADVEQLNRDsec(raccnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(raccnum,1);
					ZERNRD(eaccnum);
					ZERNRD(baccnum);
					break;
				case 2: /* Balance Sheets */
					COPYFIELD(vchnum,baccnum,"DEFINITION TYPE");
					COPYFIELD(vchnum,baccnum,"ACCOUNT CODE");
					if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(baccnum,1);
					ZERNRD(eaccnum);
					ZERNRD(raccnum);
					break;
				default:
					ZERNRD(eaccnum);
					ZERNRD(baccnum);
					ZERNRD(raccnum);
					break;
			}
			COPYFIELD(vchnum,vennum,"VENDOR IDENTIFICATION");
			if(!ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(vennum,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					COPYFIELD(vchnum,venyr,"VENDOR IDENTIFICATION");
					COPYFIELD(vchnum,venyr,"CALENDAR YEAR");
					if(!ADVEQLNRDsec(venyr,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDGETCHAR(venyr,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							COPYFIELD(vchnum,invnum,"VENDOR IDENTIFICATION");
							COPYFIELD(vchnum,invnum,"CALENDAR YEAR");
							COPYFIELD(vchnum,invnum,
								"INVOICE IDENTIFICATION");
							if(!ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mainrsrc))
							{
								FINDFLDGETCHAR(invnum,"DELETEFLAG",
									&deleteflag);
								if(!deleteflag)
								{
									COPYFIELD(invnum,venadr,"VENDOR IDENTIFICATION");
									COPYFIELD(invnum,venadr,"ADDRESS IDENTIFICATION");
									if(ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(venadr,1);
									if(!testfilerangersrc(vchnum,rs) && 
										!testfilerangersrc(invnum,rs) &&
										!testfilerangersrc(vennum,rs) && 
										!testfilerangersrc(venyr,rs)) 
									{
										for(x=0,v=mainrsrc->virflds;
											x<mainrsrc->num;++x,++v)
										{
											if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mainrsrc)) 
											{
												break;
											}
										}
										if(x>=mainrsrc->num)
										{
											addsortrecord(mainrsrc,sortfile);
											increment_diagnostic=TRUE;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		if(bankid1!=NULL) { Rfree(bankid1); bankid1=NULL; }
		ef=ADVSEQNRDsec(vchnum,SCRNvirtualSubData,mainrsrc);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(SIZNRD(sortfile->fileno))
	{
		OutputChecks(sortfile->fileno);
	} else {
		prterr("Error no Vendor Vouchers Selected for Check Printing.");
	}
	if(last_vendorid!=NULL) Rfree(last_vendorid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(bankid1!=NULL) Rfree(bankid1);
	if(sortfile!=NULL) 
	{
		SetPrintedFlag(sortfile->fileno);
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
	quitfunc(mainrsrc);
}
#ifdef XXX
static void getBANKids(RDArsrc *mainrsrc)
{

	if(banknum!=(-1))
	{
		if(BANKids!=NULL) { freeapplib(BANKids); BANKids=NULL; }
		BANKids=APPlibNEW();
		ZERNRD(banknum);
		BANKids=ADVMakeNRDlistAllSTRING(banknum,"BNKREC",
					"Creating Bank Identification List",1,
					"BANK IDENTIFICATION",
					SCRNvirtualSubData,mainrsrc);
		if(BANKids->numlibs<1) addAPPlib(BANKids,"No Banks Defined.");
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(banknum,1);
		filerecord2rsrc(banknum,mainrsrc);
	}
}
void changeBANKids(RDArsrc *mainrsrc)
{
}
#endif
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"VENDOR CHECKS DOUBLE STUB")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif
	gsv=RDAGenericSetupNew(module,"VENCHECK ADDRESS OFFSET");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			ADDRESS_OFFSET=*gsv->value.short_value;
		} else {
			ADDRESS_OFFSET=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRINT COLUMN HEADERS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			PRINT_COLUMN_HEADERS=*gsv->value.short_value;
		} else {
			PRINT_COLUMN_HEADERS=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VENCHK SHOW ACCOUNT CODE DESCRIPTION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			ACCDESC_TYPE=*gsv->value.short_value;
		} else {
			ACCDESC_TYPE=FALSE;
		}
	}
	if(!RDAstrcmp(XPERT_SETUP->zip,"30114")) ACCDESC_TYPE=2;
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VENDOR CHECK OFFSET");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			CHECK_OFFSET=*gsv->value.short_value;
		} else {
			CHECK_OFFSET=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VENCHECK SIGNATURE TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SIGNATURE_TYPE=*gsv->value.short_value;
		} else {
			SIGNATURE_TYPE=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VENCHECK SHOW INVOICE DATE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SHOW_INVOICE_DATE=*gsv->value.short_value;
		} else {
			SHOW_INVOICE_DATE=0;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("VENPMT","CLEARING BANK IDENTIFICATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			bank_id=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if((eaccnum=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((raccnum=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
	if((baccnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((invnum=APPOPNNRD(module,"VENINV",TRUE,FALSE))==(-1)) return;
	if((venyr=APPOPNNRD(module,"VENYR",TRUE,FALSE))==(-1)) return;
	if((vchnum=APPOPNNRD(module,"VENVCH",TRUE,TRUE))==(-1)) return;
	if((vennum=APPOPNNRD(module,"VENMSR",TRUE,FALSE))==(-1)) return;
	if((venadr=APPOPNNRD(module,"VENADR",TRUE,FALSE))==(-1)) return;
	if((banknum=OPNNRDsec("BNKREC","BNKACC",TRUE,FALSE,FALSE))==(-1))
	{
		if((banknum=OPNNRD("BNKREC","BNKACC"))>(-1))
		{
			ERRORDIALOG("Security Access Denied","Security Access denied to the [BNKACC] file or the file cannot otherwise be opened.\n",NULL,TRUE);
			ShutdownSecurity();
			RDAAPPMAINLOOP();
			return;
		} else {
			CLSNRD(banknum);
			banknum=(-1);
		}
	}
	mainrsrc=RDArsrcNEW(module,"VENDOR CHECKS DOUBLE STUB");
	if(vennum!=(-1)) file2rangersrc(vennum,mainrsrc);
	if(venyr!=(-1)) file2rangersrc(venyr,mainrsrc);
	if(invnum!=(-1)) file2rangersrc(invnum,mainrsrc);
	if(vchnum!=(-1)) file2rangersrc(vchnum,mainrsrc);
	if(eaccnum!=(-1)) file2rangersrc(eaccnum,mainrsrc);
	if(raccnum!=(-1)) file2rangersrc(raccnum,mainrsrc);
	if(baccnum!=(-1)) file2rangersrc(baccnum,mainrsrc);
	if(banknum!=(-1)) file2rsrc(banknum,mainrsrc,0);
	addDFincvir(mainrsrc,module,"VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,module,"VENADR",NULL,venadr);
	addDFincvir(mainrsrc,module,"VENYR",NULL,venyr);
	addDFincvir(mainrsrc,module,"VENINV",NULL,invnum);
	addDFincvir(mainrsrc,module,"VENVCH",NULL,vchnum);
	addDFincvir(mainrsrc,"FINMGT","FINEACC",NULL,eaccnum);
	addDFincvir(mainrsrc,"FINMGT","FINRACC",NULL,raccnum);
	addDFincvir(mainrsrc,"FINMGT","FINBACC",NULL,baccnum);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,banknum);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
#ifdef XXX
	if(BANKids!=NULL) { freeapplib(BANKids); BANKids=NULL; }
	BANKids=APPlibNEW();
	if(banknum==(-1))
	{
		addAPPlib(BANKids,"No BNKREC Module.");
	} else {
		addAPPlib(BANKids,"No Banks Defined.");
	}
	getBANKids(mainrsrc);
	addlstrsrc(mainrsrc,"BANK IDENTIFICATIONS",&which_type,TRUE,changeBANKids,BANKids->numlibs,&BANKids->libs,NULL);
	if(bank_id!=NULL)
	{
		for(x=0;x<BANKids->numlibs;++x)
		{
			if(!RDAstrcmp(bank_id,BANKids->libs[x])) break;
		}
	/*
		FINDRSCSETINT(mainrsrc,"BANK IDENTIFICATIONS",x);
		FINDRSCSETLIST(mainrsrc,"BANK IDENTIFICATIONS",x,BANKids->numlibs,&BANKids->libs);
	*/
		FINDRSCLISTAPPlib(mainrsrc,"BANK IDENTIFICATIONS",x,BANKids);
		updatersrc(mainrsrc,"BANK IDENTIFICATIONS");
	}
#endif
	if(output_device!=NULL) Rfree(output_device);
	output_device=DefaultPrinter();
	addbtnrsrc(mainrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncOutputDevice);
	addstdrsrc(mainrsrc,"USE CHECK OFFSET",BOOLNS,1,&use_check_offset,TRUE);
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,output_device,TRUE);
	addstdrsrc(mainrsrc,"FORMFEED",BOOLNS,1,&extra_formfeed,TRUE);
	addstdrsrc(mainrsrc,"SELECT REISSUED",BOOLNS,1,&select_reissued,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	rsrc2filerecord(banknum,mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	if(bank_id!=NULL)
	{
		FINDRSCSETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",bank_id);
		updatersrc(mainrsrc,"[BNKACC][BANK IDENTIFICATION]");
	}
	if(banknum!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
		FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",bank_id);
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(banknum,1);
		filerecord2rsrc(banknum,mainrsrc);
	}
	addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,setcheck,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
