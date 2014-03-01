/* payvwcheckdsa.c - Archive Vendor Withholding Checks - Double Stub */
#ifndef WIN32
#define __NAM__ "payvwcheckdsa.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payvwcheckdsa.exe"
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

int BODY_COUNT = 10;

char *module="PAYROLL",*output_device=NULL;
char check_formfeed=FALSE,top_feeder=FALSE;
/*
short spelledamtpos=0;
char *module="PAYROLL",*output_device=NULL;
*/
char *bank_id=NULL;
short vennum=(-1),payddsv=(-1),ddsnum=(-1);
short banknum=(-1),venadr=(-1);
short check_address_offset=(-1),check_offset=(-1),signature_type=(-1);
static RDAvirtual *VendorAddr1=NULL,*VendorAddr2=NULL,*VendorAddr3=NULL;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

void quitfunc(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) 
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	mainrsrc=NULL;
	if(bank_id!=NULL) Rfree(bank_id);
	if(payddsv!=(-1)) CLSNRD(payddsv);
	if(ddsnum!=(-1)) CLSNRD(ddsnum);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(venadr!=(-1)) CLSNRD(venadr);
	if(banknum!=(-1)) CLSNRD(banknum);
	if(output_device!=NULL) Rfree(output_device);
	ShutdownSubsystems();
	exit(0);
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
	key=KEYNUM(payddsv,1);
	addDBfield(dbsrt,"CHECK NUMBER",LONGV,10);
	addDBkeypart(dbsrt,keyno,"CHECK NUMBER");
	addDBfield(dbsrt,"VIRTUAL CHECK NUMBER",LONGV,10);
	addDBkeypart(dbsrt,keyno,"VIRTUAL CHECK NUMBER");
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(payddsv,part->name);
			if(field!=NULL) 
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
				addDBkeypart(dbsrt,keyno,field->name);
			}
		}
	}
/*
	addDBfield(dbsrt,"DESCRIPTION",1,40);
*/
	addDBfield(dbsrt,"CHECK DATE",DATES,10);
	addDBfield(dbsrt,"AMOUNT",2,20);
	addDBfield(dbsrt,"VENDOR NAME",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 1",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 2",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 3",1,40);
	addDBfield(dbsrt,"DEFAULT CHECK DETAIL",10,1);
}
void addsortrecord(RDArsrc *mainrsrc,DBsort *dbsrt)
{
	char deleteflag=FALSE;
	NRDkey *key=NULL;
	NRDpart *part;
	int x;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	key=KEYNUM(payddsv,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(payddsv,dbsrt->fileno,part->name);
		}
	}
	COPYFIELD(payddsv,dbsrt->fileno,"CHECK NUMBER");
	COPYFIELD(payddsv,dbsrt->fileno,"VIRTUAL CHECK NUMBER");
/*
	COPYFIELD(ddsnum,dbsrt->fileno,"DESCRIPTION");
*/
	COPYFIELD(payddsv,dbsrt->fileno,"CHECK DATE");
	COPYFIELD(payddsv,dbsrt->fileno,"AMOUNT");
	COPYFIELD(vennum,dbsrt->fileno,"VENDOR NAME");
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
	WRTNRD(dbsrt->fileno,0,NULL);
}
static void RepeatStub(short sfile,int checkno,int vcheckno,FILE *fp)
{
	short ef=0,count=0;
	int checkno1=0,vcheckno1=0,fyear=0,trnnum=0;
	char *ddsid=NULL;
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
		FINDFLDGETSTRING(sfile,"DEDUCTION DESCRIPTION IDENTIFICATION",&ddsid);
		FINDFLDGETINT(sfile,"FISCAL YEAR",&fyear);
		FINDFLDGETINT(sfile,"TRANSACTION NUMBER",&trnnum);
		if(checkno!=checkno1 || vcheckno!=vcheckno1) break;
		FINDFLDGETDOUBLE(sfile,"AMOUNT",&amt);
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&dcd);
/*
		FINDFLDGETSTRING(sfile,"DESCRIPTION",&desc);
		if(RDAstrlen(desc)>40) desc[40]=0;
*/
		if(count==0)
		{
			fprintf(fp,"\r\n\r\n");	
			if(dcd==FALSE)
			{
				fprintf(fp,"\r\n");
				fprintf(fp,"     SEE ATTACHED LIST FOR CHECK DETAIL\r\n");
				count+=2;
			}
		}
		if(dcd==TRUE)
		{
			fprintf(fp,"%5d %-49s %5d $%12.2f\r\n",
                            	fyear, ddsid, trnnum, amt/100);
			if(ddsid!=NULL) Rfree(ddsid);
			++count;
		}
		ef=NXTNRD(sfile,1);
	}
	/*while(count<BODY_COUNT+8)*/
	while(count<BODY_COUNT+7)
	{
		fprintf(fp,"\r\n");	
		++count;
	}
}
static void OutputChecks(short sfile)
{
	short ef=0,count=0;
	int checkno=0,vcheckno=0,last_checkno=0,last_vcheckno=0,fyear=0,trnnum=0;
	FILE *fp=NULL;
	double total_amt=0.0,amt=0.0;
	char *emp_name=NULL,*emp_add1=NULL,*emp_add2=NULL,*emp_add3=NULL;
	char *spelled_amt=NULL,*checkdate=NULL;
	char dcd=FALSE;
/*
	char *desc=NULL;
*/
	char *ddsid=NULL;
	char *tempstr=NULL,*temp=NULL,*temp1=NULL,*temp5=NULL,*temp6=NULL;

	int ret_int=0;


	ZERNRD(sfile);
	ef=FRSNRD(sfile,1);
	FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno);
	FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno);
	last_checkno=checkno;
	last_vcheckno=vcheckno;
	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"BNKREC/%s",(bank_id!=NULL ? bank_id:""));
#endif
#ifdef WIN32
	sprintf(stemp,"BNKREC\\%s",(bank_id!=NULL ? bank_id:""));
#endif
	temp6=adddashes(stemp);
	RDAmkdir(temp6,00770,&ret_int);
#ifndef WIN32
	sprintf(stemp,"BNKREC/%s/CHECKS",(bank_id!=NULL ? bank_id:""));
#endif

#ifdef WIN32
	sprintf(stemp,"BNKREC\\%s\\CHECKS",(bank_id!=NULL ? bank_id:""));
#endif
	temp5=adddashes(stemp);
	RDAmkdir(temp5,00770,&ret_int);

	temp=Rmalloc(529);
	memset(temp,0,528);
	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"BNKREC/%s/CHECKS/%d.pdf",bank_id,checkno);
#endif 
#ifdef WIN32
	sprintf(stemp,"BNKREC\\%s/CHECKS\\%d.pdf",bank_id,checkno);
#endif 

	temp1=adddashes(stemp);
#ifndef WIN32
	sprintf(temp,"viewpdf.lnx --dont-view --dont-delete -pt1 -ps1 -o \"%s/%s\"",CURRENTDIRECTORY,temp1);
#endif 
#ifdef WIN32
	sprintf(temp,"viewpdf.exe --dont-view --dont-delete -pt1 -ps1 -o \"%s/%s\"",CURRENTDIRECTORY,temp1);
#endif 

	if((fp=popen(temp,"w"))!=NULL)
	{
	} else {
		if(temp!=NULL) Rfree(temp);
		return;
	}
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);

	while(!ef)
	{
		FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno);
		FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno);
		if(checkno!=last_checkno || vcheckno!=last_vcheckno)
		{
			while(count<BODY_COUNT+7)
			{
				fprintf(fp,"\r\n");	
				++count;
			}
			if(RDAstrlen(emp_name)>30) emp_name[30]=0;
			if(checkno==last_checkno)
			{
				fprintf(fp,"%10s %8d %-30s Sub-Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
			}
			else
			{
				fprintf(fp,"%10s %8d %-30s     Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
			}
			++count;
			fprintf(fp,"\r\n");	
			RepeatStub(sfile,last_checkno,last_vcheckno,fp);
			if(checkno==last_checkno)
			{
				fprintf(fp,"%10s %8d %-30s Sub-Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
			}
			else
			{
				fprintf(fp,"%10s %8d %-30s     Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
			}
			++count;
			fprintf(fp,"\r\n");	
	/* negotiable check */
			/*fprintf(fp,"\r\n");*/
			fprintf(fp,"\r\n");
			fprintf(fp,"\r\n");
			if(top_feeder==FALSE)
			{
				fprintf(fp,"\r\n");
				fprintf(fp,"\r\n");
			}
			fprintf(fp,"\r\n");
			fprintf(fp,"\r\n");
			fprintf(fp,"\r\n");
			fprintf(fp,"\r\n");
			fprintf(fp,"\r\n");
			fprintf(fp,"\r\n");
			if(checkno!=last_checkno)
			{
				spelled_amt=spell(total_amt);
				fprintf(fp,"  %s\r\n",spelled_amt);
				if(spelled_amt!=NULL) Rfree(spelled_amt);
				fprintf(fp,"\r\n");
				fprintf(fp,"\r\n");
				tempstr=famtpad(total_amt,14,'*');
				fprintf(fp,"%44s %-10s%10d %-14s\r\n",
					"",(checkdate!=NULL?checkdate:""),last_vcheckno,
					(tempstr!=NULL?tempstr:""));
				if(tempstr!=NULL) Rfree(tempstr);
				fprintf(fp,"\r\n");
			} else {
				fprintf(fp,"  *** V O I D **** V O I D **** V O I D **** \r\n");
				fprintf(fp,"\r\n");
				fprintf(fp,"\r\n");
				tempstr=famtpad(0,14,'*');
				fprintf(fp,"%44s %-10s%10d %-14s\r\n",
					"",(checkdate!=NULL?checkdate:""),last_vcheckno,
					(tempstr!=NULL?tempstr:""));
				if(tempstr!=NULL) Rfree(tempstr);
/*
				fprintf(fp,"\r\n");
*/
			}
			if(check_address_offset<0)
			{
				fprintf(fp,"%*s%-40s\r\n",8,"",(emp_name!=NULL ? emp_name:""));
				fprintf(fp,"%*s%-40s\r\n",8,"",(emp_add1!=NULL ? emp_add1:""));
				fprintf(fp,"%*s%-40s\r\n",8,"",(emp_add2!=NULL ? emp_add2:""));
				fprintf(fp,"%*s%-40s\r\n",8,"",(emp_add3!=NULL ? emp_add3:""));
			} else {
				fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_name!=NULL ? emp_name:""));
				fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_add1!=NULL ? emp_add1:""));
				fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_add2!=NULL ? emp_add2:""));
				fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_add3!=NULL ? emp_add3:""));
			}
			fprintf(fp,"\f");
			count=0;	
			if(checkno!=last_checkno)
			{
				total_amt=0;
			}
			last_checkno=checkno;
			last_vcheckno=vcheckno;
			pclose(fp);
			memset(stemp,0,101);
#ifndef WIN32
			sprintf(stemp,"BNKREC/%s",(bank_id!=NULL ? bank_id:""));
#endif
#ifdef WIN32
			sprintf(stemp,"BNKREC\\%s",(bank_id!=NULL ? bank_id:""));
#endif
			temp6=adddashes(stemp);
			RDAmkdir(temp6,00770,&ret_int);
#ifndef WIN32
			sprintf(stemp,"BNKREC/%s/CHECKS",(bank_id!=NULL ? bank_id:""));
#endif

#ifdef WIN32
			sprintf(stemp,"BNKREC\\%s\\CHECKS",(bank_id!=NULL ? bank_id:""));
#endif
			temp5=adddashes(stemp);
			RDAmkdir(temp5,00770,&ret_int);

			temp=Rmalloc(529);
			memset(temp,0,528);
			memset(stemp,0,101);
#ifndef WIN32
			sprintf(stemp,"BNKREC/%s/CHECKS/%d.pdf",bank_id,checkno);
#endif 
#ifdef WIN32
			sprintf(stemp,"BNKREC\\%s/CHECKS\\%d.pdf",bank_id,checkno);
#endif 

			temp1=adddashes(stemp);
#ifndef WIN32
			sprintf(temp,"viewpdf.lnx --dont-view --dont-delete -pt1 -ps1 -o \"%s/%s\"",CURRENTDIRECTORY,temp1);
#endif 
#ifdef WIN32
			sprintf(temp,"viewpdf.exe --dont-view --dont-delete -pt1 -ps1 -o \"%s/%s\"",CURRENTDIRECTORY,temp1);
#endif 
			if((fp=popen(temp,"w"))!=NULL)
			{
			} else {
				if(temp!=NULL) Rfree(temp);
				return;
			}
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			if(temp5!=NULL) Rfree(temp5);
			if(temp6!=NULL) Rfree(temp6);
		}
		FINDFLDGETINT(sfile,"FISCAL YEAR",&fyear);
		FINDFLDGETSTRING(sfile,"DEDUCTION DESCRIPTION IDENTIFICATION",&ddsid);
		FINDFLDGETINT(sfile,"TRANSACTION NUMBER",&trnnum);
		FINDFLDGETSTRING(sfile,"CHECK DATE",&checkdate);
		FINDFLDGETSTRING(sfile,"NAME",&emp_name);
		FINDFLDGETDOUBLE(sfile,"AMOUNT",&amt);
		FINDFLDGETSTRING(sfile,"VENDOR NAME",&emp_name);
		if(RDAstrlen(emp_name)>30) emp_name[30]=0;
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 1",&emp_add1);
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 2",&emp_add2);
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 3",&emp_add3);
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&dcd);
	/* print 1st detail stub */
		if(count==0)
		{
			fprintf(fp,"\r\n\r\n");	
			if(dcd==FALSE)
			{
				fprintf(fp,"\r\n");
				fprintf(fp,"     SEE ATTACHED LIST FOR CHECK DETAIL\r\n");
				count+=2;
			}
		}
		if(dcd==TRUE)
		{
			fprintf(fp,"%5d %-49s %5d $%12.2f\r\n",
                            	fyear, ddsid, trnnum, amt/100);
			if(ddsid!=NULL) Rfree(ddsid);
			++count;
		}
		total_amt+=amt;
		ef=NXTNRD(sfile,1);
	}
	/*while(count<BODY_COUNT+8)*/
	while(count<BODY_COUNT+7)
	{
		fprintf(fp,"\r\n");	
		++count;
	}
	fprintf(fp,"%10s %8d %-30s     Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
	++count;
	fprintf(fp,"\r\n");	
	/* repeat stub and print negotiable */
	RepeatStub(sfile,last_checkno,last_vcheckno,fp);
	fprintf(fp,"%10s %8d %-30s     Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
	++count;
	fprintf(fp,"\r\n");	
	fprintf(fp,"\r\n");
	fprintf(fp,"\r\n");
	if(top_feeder==FALSE)
	{
		fprintf(fp,"\r\n");
		fprintf(fp,"\r\n");
	}
	fprintf(fp,"\r\n");
	fprintf(fp,"\r\n");
	fprintf(fp,"\r\n");
	fprintf(fp,"\r\n");
	fprintf(fp,"\r\n");
	fprintf(fp,"\r\n");
	spelled_amt=spell(total_amt);
	fprintf(fp,"  %s\r\n",spelled_amt);
	if(spelled_amt!=NULL) Rfree(spelled_amt);
	fprintf(fp,"\r\n");
	fprintf(fp,"\r\n");
	tempstr=famtpad(total_amt,14,'*');
	fprintf(fp,"%44s %-10s%10d %-14s\r\n",
		"",(checkdate!=NULL?checkdate:""),last_vcheckno,
		(tempstr!=NULL?tempstr:""));
	if(tempstr!=NULL) Rfree(tempstr);
	if(check_address_offset<0)
	{
		fprintf(fp,"%*s%-40s\r\n",8,"",(emp_name!=NULL ? emp_name:""));
		fprintf(fp,"%*s%-40s\r\n",8,"",(emp_add1!=NULL ? emp_add1:""));
		fprintf(fp,"%*s%-40s\r\n",8,"",(emp_add2!=NULL ? emp_add2:""));
		fprintf(fp,"%*s%-40s\r\n",8,"",(emp_add3!=NULL ? emp_add3:""));
	} else {
		fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_name!=NULL ? emp_name:""));
		fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_add1!=NULL ? emp_add1:""));
		fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_add2!=NULL ? emp_add2:""));
		fprintf(fp,"%*s%-40s\r\n",(check_address_offset+8),"",(emp_add3!=NULL ? emp_add3:""));
	}
	fprintf(fp,"\f");
	count=0;	
	if(checkno!=last_checkno)
	{
		total_amt=0;
	}
	last_checkno=checkno;
	last_vcheckno=vcheckno;
	pclose(fp);
	if(checkdate!=NULL) Rfree(checkdate);
}
void setcheck(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE;
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*bankid1=NULL;
	int checknum=0;
	RDArsrc *tmprsrc=NULL;
	int x=0;
	RDAvirtual *v;
	char manual_voucher=FALSE,deleteflag=FALSE,cleared=FALSE;
	char distributed=FALSE,verified=FALSE,electronic=FALSE;
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
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&output_device);
	FINDRSCGETINT(mainrsrc,"BODY COUNT",&BODY_COUNT);
	sortname=unique_name();
	sortfile=DBsortNEW("PAYROLL",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(payddsv,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(payddsv,"DIAGNOSTIC SCREEN",module,
		"Selecting Vendor Vouchers",NULL);
	if(tmprsrc!=NULL)
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	ef=ADVBEGNRDsec(payddsv,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(payddsv,"SOURCE USER VERIFIED",&verified);
		FINDFLDGETCHAR(payddsv,"MANUAL VOUCHER",&manual_voucher);
		FINDFLDGETCHAR(payddsv,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(payddsv,"BNKREC UPDATED",&cleared);
		FINDFLDGETCHAR(payddsv,"ELECTRONIC TRANSFER",&electronic);
		FINDFLDGETCHAR(payddsv,"DISTRIBUTED",&distributed);
		if(verified && !cleared && !manual_voucher && !electronic && !deleteflag && !distributed)
		{
		if(banknum!=(-1))
		{
			if(bank_id!=NULL)
			{
				FINDFLDGETSTRING(payddsv,"BANK IDENTIFICATION",&bankid1);
			}
		}
		if(!RDAstrcmp(bank_id,bankid1))
		{
			FINDFLDGETINT(payddsv,"CHECK NUMBER",&checknum);
			if(checknum>0)
			{
				COPYFIELD(payddsv,vennum,"VENDOR IDENTIFICATION");
				if(!ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc))
				{
					FINDFLDGETCHAR(vennum,"DELETEFLAG",
						&deleteflag);
					if(!deleteflag)
					{
						COPYFIELD(payddsv,venadr,"VENDOR IDENTIFICATION");
						COPYFIELD(payddsv,venadr,"ADDRESS IDENTIFICATION");
						if(!ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mainrsrc))
						{
							FINDFLDGETCHAR(venadr,"DELETEFLAG",
								&deleteflag);
							if(!deleteflag)
							{
								COPYFIELD(payddsv,ddsnum,"DEDUCTION DESCRIPTION IDENTIFICATION");
								if(!ADVEQLNRDsec(ddsnum,1,SCRNvirtualSubData,mainrsrc))
								{
									FINDFLDGETCHAR(ddsnum,"DELETEFLAG",
										&deleteflag);
									if(!deleteflag)
									{
										if(!testfilerangersrc(payddsv,rs)/* && 
											!testfilerangersrc(vennum,rs)*/)
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
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		ef=ADVSEQNRDsec(payddsv,SCRNvirtualSubData,mainrsrc);
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
	if(bankid1!=NULL) Rfree(bankid1);
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
/*
	quitfunc(mainrsrc);
*/
}
int main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_APP;
	if(InitializeSubsystems(argc,argv,module,"ARCHIVE VENDOR WITHHOLDING CHECKS DOUBLE STUB")) 
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
	gsv=RDAGenericSetupNew(module,"VWCHECK FORMFEED");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_formfeed=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			check_formfeed=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"TOP FED LASER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			top_feeder=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			top_feeder=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VWCHECK ADDRESS OFFSET");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_address_offset=*gsv->value.short_value;
		} else {
			check_address_offset=0;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VWCHECK CHECK OFFSET");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_offset=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			check_offset=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VWCHECK SIGNATURE TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			signature_type=*gsv->value.short_value;
		} else {
			signature_type=0;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if((payddsv=APPOPNNRD("PAYROLL","PAYDDSV",TRUE,FALSE))==(-1)) return;
	if((ddsnum=APPOPNNRD("PAYROLL","PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((venadr=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((banknum=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"ARCHIVE VENDOR WITHHOLDING CHECKS DOUBLE STUB");
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,banknum);
	if(banknum!=(-1)) file2rsrc(banknum,mainrsrc,0);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	if(payddsv!=(-1)) file2rangersrc(payddsv,mainrsrc);
	if(ddsnum!=(-1)) file2rangersrc(ddsnum,mainrsrc);
	if(vennum!=(-1)) file2rangersrc(vennum,mainrsrc);
	addDFincvir(mainrsrc,module,"PAYDDSV",NULL,payddsv);
	addDFincvir(mainrsrc,module,"PAYDDMS",NULL,ddsnum);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,banknum);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	if(output_device!=NULL) Rfree(output_device);
	output_device=DefaultPrinter();
	addbtnrsrc(mainrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncOutputDevice);
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,output_device,TRUE);
	addstdrsrc(mainrsrc,"BODY COUNT",LONGV,0,&BODY_COUNT,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	rsrc2filerecord(banknum,mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	filerecord2rsrc(banknum,mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,setcheck,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
