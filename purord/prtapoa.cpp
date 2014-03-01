/* prtapoa.c - Archive Actual Purchase Orders */

#ifndef WIN32
#define __NAM__ "prtapoa.lnx"
#endif
#ifdef WIN32
#define __NAM__ "prtapoa.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <fin.hpp>
#include <misc.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define DEFAULT_NUMBER_LINES	31
#define PAGE_WIDTH	80
static char *module="PURORD";
static short purtrn=(-1),puracc=(-1),purlin=(-1);
static short purmst=(-1),pursit=(-1),venmsr=(-1);
static short venadr=(-1),fineacc=(-1),finracc=(-1);
static short finbacc=(-1),DID_STUFF=0;
static int selected_fyear=0,num_pocrt_lines=0;
static RDArsrc *diagrsrc=NULL;
static char po_signature=FALSE,long_acct_code=FALSE;
static DBsort *sortfile=NULL;
static char *OrgName=NULL,*OrgAddr1=NULL,*OrgAddr2=NULL,*OrgAddr3=NULL;
static char *OrgPhone=NULL,*OrgFax=NULL,*pocrt=NULL;
static RDAvirtual *org_addr1=NULL,*org_addr2=NULL,*org_addr3=NULL;
static RDAvirtual *VendorAddr1=NULL,*VendorAddr2=NULL,*VendorAddr3=NULL;
static char *outdevice=NULL,print_page_footers=FALSE;
static char print_po_accounts=FALSE,disable_account_desc=FALSE;
static char use_po_lines=FALSE,print_po_comments=FALSE;
static char *selected_poid=NULL,print_sales_rep=FALSE;
static char print_purlin_comments=FALSE;
static char *page_footer1=NULL,*page_footer2=NULL;
static char *page_footer3=NULL,*page_footer4=NULL;
static short address_position=0;
static char *vendor_addr1=NULL,*vendor_addr2=NULL,*vendor_addr3=NULL;
static char *shipping_addr1=NULL,*shipping_addr2=NULL,*shipping_addr3=NULL,*shipping_addr4=NULL;
static char *purmst_salesman=NULL,*pursit_phone=NULL,*purlin_entrydt=NULL;
static char *purmst_dlvrydt=NULL,*purmst_shipvia=NULL,*purmst_fob=NULL;
static char *purmst_terms=NULL,*vendor_phone=NULL,*vendor_fax=NULL,*purmst_ordered_by=NULL;
static char *poid1=NULL,*vendor_id=NULL,*vendor_name=NULL,*purmst_attention_to=NULL;
static char *purlin_unit=NULL;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

static void SetArchiveName(short sfile)
{
	short ef=0;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	int x=0;
	RDATData *prev=NULL;
	char *arch_type=NULL,*poid=NULL;
	char *filename=NULL,*temp=NULL;

	arch_type=stralloc("pdf");
	key=KEYNUM(purmst,1);
	ZERNRD(sfile);
	ef=FRSNRD(sfile,1);
	while(!ef)
	{
		ZERNRD(purmst);
		if(key!=NULL)
		{
			for(x=0,part=key->part;x<key->numparts;++x,++part)
			{
				COPYFIELD(sfile,purmst,part->name);
			}
		}
		if(!EQLNRD(purmst,1)) KEYNRD(purmst,1);
		prev=RDATDataNEW(purmst);
		FINDFLDGETSTRING(purmst,"PO IDENTIFICATION",&poid);	
		temp=adddashes(poid);
		filename=Rmalloc(RDAstrlen(temp)+4);
		sprintf(filename,"%s%s",temp,".pdf");
		FINDFLDSETSTRING(purmst,"FILENAME",filename);	
		FINDFLDSETSTRING(purmst,"TYPE",arch_type);	
		WRTTRANS(purmst,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		ef=NXTNRD(sfile,1);
		if(temp!=NULL) Rfree(temp);
		if(filename!=NULL) Rfree(filename);
	}
	if(arch_type!=NULL) Rfree(arch_type);
}
static void quitfunc()
{
	if(purtrn!=(-1)) CLSNRD(purtrn);
	if(puracc!=(-1)) CLSNRD(puracc);
	if(purlin!=(-1)) CLSNRD(purlin);
	if(purmst!=(-1)) CLSNRD(purmst);
	if(pursit!=(-1)) CLSNRD(pursit);
	if(venmsr!=(-1)) CLSNRD(venmsr);
	if(venadr!=(-1)) CLSNRD(venadr);
	if(fineacc!=(-1)) CLSNRD(fineacc);
	if(finracc!=(-1)) CLSNRD(finracc);
	if(finbacc!=(-1)) CLSNRD(finbacc);
	ShutdownSubsystems();
	exit(0);
}
static void quitprint()
{
	if(sortfile!=NULL)
	{
		SetArchiveName(sortfile->fileno);
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
		sortfile=NULL;
	}
	quitfunc();
}
static void printheaders(RDA_PFILE *fp,int page_no)
{
/* PAGE HEADERS */
	switch(address_position)
	{
		default:
		case 0:
			RDA_fprintf(fp,"                                                                Page: %6d\r\n",page_no);
			RDA_fprintf(fp,"%*sP.O.: %-10.10s\r\n",(PAGE_WIDTH-6-10),"",(poid1!=NULL ? poid1:""));
			RDA_fprintf(fp,"%*s%s%*sDATE: %10s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgName)))/2," ",
				(OrgName!=NULL ? OrgName:""),((PAGE_WIDTH-16-(PAGE_WIDTH-(RDAstrlen(OrgName)))/2)-RDAstrlen(OrgName)),"",
				(purlin_entrydt!=NULL ? purlin_entrydt:""));
			RDA_fprintf(fp,"%*s%s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgAddr1)))/2," ",
			(OrgAddr1!=NULL ? OrgAddr1:""));
			RDA_fprintf(fp,"%*s%s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgAddr2)))/2," ",
			(OrgAddr2!=NULL ? OrgAddr2:""));
			RDA_fprintf(fp,"%*s%s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgAddr3)))/2," ",
			(OrgAddr3!=NULL ? OrgAddr3:""));
			RDA_fprintf(fp,"                              PHONE: %-12s\r\n",
			(OrgPhone!=NULL ? OrgPhone:""));
			RDA_fprintf(fp,"                                FAX: %-12s\r\n",
			(OrgFax!=NULL ? OrgFax:""));
			RDA_fprintf(fp,"\r\n");
			break;
		case 1:
			RDA_fprintf(fp,"                                                                Page: %d\r\n",page_no);
			RDA_fprintf(fp,"%*sP.O.: %-10.10s\r\n",(PAGE_WIDTH-6-10),"",(poid1!=NULL ? poid1:""));
			RDA_fprintf(fp,"            REMIT BILL TO: %-36.36s%*sDATE: %10s\r\n",
				(OrgName!=NULL ? OrgName:""),(PAGE_WIDTH-16-63),"",(purlin_entrydt!=NULL ? purlin_entrydt:""));
			RDA_fprintf(fp,"                           %-50s\r\n",(OrgAddr1!=NULL ? OrgAddr1:""));
			RDA_fprintf(fp,"                           %-50s\r\n",(OrgAddr2!=NULL ? OrgAddr2:""));
			RDA_fprintf(fp,"                           %-50s\r\n",(OrgAddr3!=NULL ? OrgAddr3:""));
			RDA_fprintf(fp,"                           PHONE: %-12s\r\n",(OrgPhone!=NULL ? OrgPhone:""));
			RDA_fprintf(fp,"                           FAX:   %-12s\r\n",(OrgFax!=NULL ? OrgFax:""));
			RDA_fprintf(fp,"\r\n");
			break;
		case 2:
			RDA_fprintf(fp,"                                                                Page: %d\r\n",page_no);
			RDA_fprintf(fp,"%*sP.O.: %-10.10s\r\n",(PAGE_WIDTH-6-10),"",(poid1!=NULL ? poid1:""));
			RDA_fprintf(fp,"REMIT BILL TO:%-49.49s DATE: %10s\r\n",
				(OrgName!=NULL ? OrgName:""),(purlin_entrydt!=NULL ? purlin_entrydt:""));
			RDA_fprintf(fp,"              %-50s\r\n",(OrgAddr1!=NULL ? OrgAddr1:""));
			RDA_fprintf(fp,"              %-50s\r\n",(OrgAddr2!=NULL ? OrgAddr2:""));
			RDA_fprintf(fp,"              %-50s\r\n",(OrgAddr3!=NULL ? OrgAddr3:""));
			RDA_fprintf(fp,"              PHONE: %-12s\r\n",(OrgPhone!=NULL ? OrgPhone:""));
			RDA_fprintf(fp,"              FAX: %-12s\r\n",(OrgFax!=NULL ? OrgFax:""));
			RDA_fprintf(fp,"\r\n");
			break;
		case 3:
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"                                                                Page: %d\r\n",page_no);
			RDA_fprintf(fp,"%*sP.O.: %-10.10s\r\n",(PAGE_WIDTH-6-10),"",(poid1!=NULL ? poid1:""));
			RDA_fprintf(fp,"%*sDATE: %s\r\n",(PAGE_WIDTH-7-RDAstrlen(poid1)),"",(purlin_entrydt!=NULL ? purlin_entrydt:""));
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			break;
		case 4:
			RDA_fprintf(fp,"                                                                Page: %d\r\n",page_no);
			RDA_fprintf(fp,"%*sP.O.: %-10.10s\r\n",(PAGE_WIDTH-6-10),"",(poid1!=NULL ? poid1:""));
			RDA_fprintf(fp,"%*s%s%*sDATE: %10s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgName)))/2," ",
				(OrgName!=NULL ? OrgName:""),((PAGE_WIDTH-16-(PAGE_WIDTH-(RDAstrlen(OrgName)))/2)-RDAstrlen(OrgName)),"",
				(purlin_entrydt!=NULL ? purlin_entrydt:""));
			RDA_fprintf(fp,"%*s%s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgAddr1)))/2," ",
				(OrgAddr1!=NULL ? OrgAddr1:""));
			RDA_fprintf(fp,"%*s%s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgAddr2)))/2," ",
				(OrgAddr2!=NULL ? OrgAddr2:""));
			RDA_fprintf(fp,"%*s%s\r\n",(PAGE_WIDTH-(RDAstrlen(OrgAddr3)))/2," ",
				(OrgAddr3!=NULL ? OrgAddr3:""));
			RDA_fprintf(fp,"                              PHONE: %-12s\r\n",
				(OrgPhone!=NULL ? OrgPhone:""));
			RDA_fprintf(fp,"                                FAX: %-12s\r\n",
				(OrgFax!=NULL ? OrgFax:""));
			RDA_fprintf(fp,"\r\n");
			break;
	}
	RDA_fprintf(fp," TO:                                      SHIP TO:                              \r\n");
	memset(stemp,0,101);
	if(!isEMPTY(purmst_attention_to))
	{
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_name!=NULL ? vendor_name:""),
		(purmst_attention_to!=NULL ? purmst_attention_to:""));
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_addr1!=NULL ? vendor_addr1:""),
		(shipping_addr1!=NULL ? shipping_addr1:""));
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_addr2!=NULL ? vendor_addr2:""),
		(shipping_addr2!=NULL ? shipping_addr2:""));
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_addr3!=NULL ? vendor_addr3:""),
		(shipping_addr3!=NULL ? shipping_addr3:""));
	RDA_fprintf(fp,"   Phone: %-12s  Fax: %-12s   %-35s\r\n",(vendor_phone!=NULL ? vendor_phone:""),
		(vendor_fax!=NULL ? vendor_fax:""),(shipping_addr4!=NULL ? shipping_addr4:""));
	} else {
	sprintf(stemp,"Phone Number: %s",(pursit_phone!=NULL ? pursit_phone:""));
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_name!=NULL ? vendor_name:""),(shipping_addr1!=NULL ? shipping_addr1:""));
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_addr1!=NULL ? vendor_addr1:""),
		(shipping_addr2!=NULL ? shipping_addr2:""));
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_addr2!=NULL ? vendor_addr2:""),
		(shipping_addr3!=NULL ? shipping_addr3:""));
	RDA_fprintf(fp,"   %-35s      %-35s\r\n",(vendor_addr3!=NULL ? vendor_addr3:""),
		(shipping_addr4!=NULL ? shipping_addr4:""));
	RDA_fprintf(fp,"   Phone: %-12s  Fax: %-12s   %-35s\r\n",(vendor_phone!=NULL ? vendor_phone:""),
		(vendor_fax!=NULL ? vendor_fax:""),stemp);
	}
	if(print_sales_rep && isEMPTY(stemp))
	{
		RDA_fprintf(fp,"   Sales Rep: %-15s               Phone Number: %-12s\r\n",
			(purmst_salesman!=NULL ? purmst_salesman:""),(pursit_phone!=NULL ? pursit_phone:""));
	} else if(print_sales_rep && !isEMPTY(stemp))
	{
		RDA_fprintf(fp,"   Sales Rep: %-15s\r\n",
			(purmst_salesman!=NULL ? purmst_salesman:""));
	} else if(!isEMPTY(purmst_attention_to))
	{
		RDA_fprintf(fp,"                                            Phone Number: %-12s\r\n",
			(pursit_phone!=NULL ? pursit_phone:""));
	} else {
		RDA_fprintf(fp,"\r\n");
	}
	SelectPrintTypeByName(outdevice,"HALF LINE FEED",fp);
	RDA_fprintf(fp," VENDOR ID: %-29.29s SHIP VIA: %-15s\r\n",(vendor_id!=NULL ? vendor_id:""),
		(purmst_shipvia!=NULL ? purmst_shipvia:""));
	RDA_fprintf(fp," DELIVERY: %-10.10s %19.19s F.O.B.: %-20s\r\n",
		(purmst_dlvrydt!=NULL ? purmst_dlvrydt:"")," ",(purmst_fob!=NULL ? purmst_fob:""));
	RDA_fprintf(fp," ORDERED BY: %-25s    TERMS: %-30s\r\n",(purmst_ordered_by!=NULL ? purmst_ordered_by:""),
		(purmst_terms!=NULL ? purmst_terms:""));
	SelectPrintTypeByName(outdevice,"HALF LINE FEED",fp);
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
/* END PAGE HEADERS */
}
static void printfooters(RDA_PFILE *fp)
{
	if(print_page_footers)
	{
/* PAGE FOOTERS */
		RDA_fprintf(fp,"\r\n");
		RDA_fprintf(fp,"%-80s\r\n",(page_footer1!=NULL ? page_footer1:""));
		RDA_fprintf(fp,"%-80s\r\n",(page_footer2!=NULL ? page_footer2:""));
		RDA_fprintf(fp,"%-80s\r\n",(page_footer3!=NULL ? page_footer3:""));
		RDA_fprintf(fp,"%-80s\r\n\f",(page_footer4!=NULL ? page_footer4:""));
	} else {
		RDA_fprintf(fp,"\r\n\f");
	}
}
static void printactualpos(APPlib *imagelines,int fyear1)
{
	int x=0,y=0,pageno=1,pagelength=0;
	char *fyear_string=NULL;
	char *temp=NULL,*temp1=NULL,*temp5=NULL,*temp6=NULL;
	RDA_PFILE *fp=NULL;
	FILE *fp2=NULL;
	struct stat fst;
	char *PO_SIG_XY=NULL,*filename=NULL,*password=NULL;
	int usersigsec=0;
	int ret_int=0;
	
	password=RDA_GetEnv("PASSWD");
	fyear_string=Rmalloc(4+1);
	sprintf(fyear_string,"%i",fyear1);
#ifndef WIN32
	sprintf(stemp,"PURORD/%s%s",".",(fyear_string!=NULL ? fyear_string:""));
#endif
#ifdef WIN32
	sprintf(stemp,"PURORD\\%s%s",".",(fyear_string!=NULL ? fyear_string:""));
#endif
	temp6=adddashes(stemp);
	RDAmkdir(temp6,00770,&ret_int);

	temp=Rmalloc(529);
	memset(temp,0,528);
	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"PURORD/%s%s/%s.pdf",".",fyear_string,poid1);
	temp1=adddashes(stemp);
	sprintf(temp,"viewpdf.lnx --dont-view --dont-delete -pt1 -ps1 -o \"%s/%s\"",CURRENTDIRECTORY,temp1);
#endif 
#ifdef WIN32
	sprintf(stemp,"PURORD\\%s%s\\%s.pdf",".",fyear_string,poid1);
	temp1=adddashes(stemp);
	sprintf(temp,"viewpdf.exe --dont-view --dont-delete -pt1 -ps1 -o \"%s/%s\"",CURRENTDIRECTORY,temp1);
#endif 

	if((fp=RDA_popen(temp,"w"))!=NULL)
	{
	} else {
		if(temp!=NULL) Rfree(temp);
		return;
	}
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(fyear_string!=NULL) Rfree(fyear_string);
		
	/* Add PCL JOB HEADER here? */
	if(po_signature)
	{
		/* SelectPrintTypeByName(outdevice,"LOAD PO SIGNATURE",fp); */
		if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
		if(SEC_USERS_FILENO!=(-1))
		{
			FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
			if(!EQLNRD(SEC_USERS_FILENO,1))
			{
				FINDFLDGETSTRING(SEC_USERS_FILENO,"PURORD SIGNATURE FILENAME",&filename);
				FINDFLDGETSTRING(SEC_USERS_FILENO,"PURORD SIGNATURE PASSWORD",&password);
			}
		}

		if((!isEMPTY(filename))&&(!isEMPTY(password)))
		{
			usersigsec=1;
		}

		if(usersigsec)
		{
			y=0;
			memset(stemp,0,101);
			strcpy(stemp,filename);
			
			if(stemp[0]=='/')
			{
				/* PO SIG FILENAME is absolute */
				for(x=0;x<80;x++)
				{
					if(stemp[x]=='/')
					{
						y=x;
					}
				}
				if(y>0)
				{
					stemp[(y+1)]=NULL;
				}
			}else{
				/* PO SIG FILENAME is assumed relative */
				memset(stemp,0,101);
			}


			if(FileExists("rda-pur1a.dat"))
			{
				RDA_fprintf(fp,"%c260101:%srda-pur1a.dat:%s%c",27,stemp,(password!=NULL ? password:""),27);
			}
			if(FileExists("rda-pur2a.dat"))
			{
				RDA_fprintf(fp,"%c260102:%srda-pur2a.dat:%s%c",27,stemp,(password!=NULL ? password:""),27);
			}
			if(FileExists("rda-pur3a.dat"))
			{
				RDA_fprintf(fp,"%c260103:%srda-pur3a.dat:%s%c",27,stemp,(password!=NULL ? password:""),27);
			}
			if(FileExists("po-siga.dat"))
			{
				stat("po-siga.dat",&fst);
				PO_SIG_XY=malloc(sizeof(char *)*(fst.st_size+1));
				fp2=fopen("po-siga.dat","rb");
				fread(PO_SIG_XY,1,fst.st_size,fp2);
				/* prterr("\nPO_SIG_XY [%s]\n\n",PO_SIG_XY); */
				if(fp2!=NULL) fclose(fp2);
			}
		}
		if(filename!=NULL) { Rfree(filename); }
		if(password!=NULL) { Rfree(password); }
	}
	SelectPrintTypeByName(outdevice,"LOAD_PCLMACRO_5000",fp);

	if(print_page_footers) pagelength=DEFAULT_NUMBER_LINES;
	else pagelength=DEFAULT_NUMBER_LINES+5;
	if(imagelines->numlibs>0)
	{
		printheaders(fp,pageno);
		for(x=0,y=1;x<imagelines->numlibs;++x,++y)
		{
			if(diagapps)
			{
				RDA_fprintf(fp,"Line %d: %s",y,imagelines->libs[x]);
			} else {
				RDA_fprintf(fp,"%s",imagelines->libs[x]);
			}
			if(y==pagelength && x<(imagelines->numlibs-1))
			{
				++pageno;
				SelectPrintTypeByName(outdevice,"PUSH CURSOR",fp);
				SelectPrintTypeByName(outdevice,"USE_PCLMACRO_5000",fp);
				SelectPrintTypeByName(outdevice,"POP CURSOR",fp);
				printfooters(fp);
				printheaders(fp,pageno);
				y=0;
			}
			if(num_pocrt_lines>0 && x==(imagelines->numlibs-1) && pocrt!=NULL)
			{
				if((pagelength-num_pocrt_lines)<y)
				{
					if(print_page_footers)
					{
						while(y<pagelength)
						{
							++y;
							if(diagapps)
							{
								RDA_fprintf(fp,"Line %d: \r\n",y);
							} else {
								RDA_fprintf(fp,"\r\n");
							}
						}
					}
					++pageno;
					SelectPrintTypeByName(outdevice,"PUSH CURSOR",fp);
					SelectPrintTypeByName(outdevice,"USE_PCLMACRO_5000",fp);
					SelectPrintTypeByName(outdevice,"POP CURSOR",fp);
					printfooters(fp);
					printheaders(fp,pageno);
					y=0;
				}
				while((pagelength-num_pocrt_lines)>y)
				{
					++y;
					if(diagapps)
					{
						RDA_fprintf(fp,"Line %d: \r\n",y);
					} else {
						RDA_fprintf(fp,"\r\n");
					}
				}
				y+=num_pocrt_lines;
				SelectPrintTypeByName(outdevice,"COMPRESSED TEXT",fp);
				RDA_fprintf(fp,pocrt);
				SelectPrintTypeByName(outdevice,"NORMAL TEXT",fp);
			}
			if(print_page_footers && x==(imagelines->numlibs-1) && y<pagelength)
			{
				while(y<pagelength)
				{
					++y;
					if(diagapps)
					{
						RDA_fprintf(fp,"Line %d: \r\n",y);
					} else {
						RDA_fprintf(fp,"\r\n");
					}
				}
			}
		}
		if((po_signature)&&(usersigsec))
		{
			SelectPrintTypeByName(outdevice,"PUSH CURSOR",fp);
			/* SelectPrintTypeByName(outdevice,"PO SIGNATURE",fp);*/
			if(PO_SIG_XY!=NULL) 
			{
				RDA_fprintf(fp,"%s",PO_SIG_XY);
			}

			SelectPrintTypeByName(outdevice,"POP CURSOR",fp);
		}
		SelectPrintTypeByName(outdevice,"PUSH CURSOR",fp);
		SelectPrintTypeByName(outdevice,"USE_PCLMACRO_5000",fp);
		SelectPrintTypeByName(outdevice,"POP CURSOR",fp);
		printfooters(fp);
	}
	SelectPrintTypeByName(outdevice,"DELETE_PCLMACRO_5000",fp);
	fp->wrote_something=FALSE;
	RDA_pclose(fp);
	if(PO_SIG_XY!=NULL) Rfree(PO_SIG_XY);
}
static void createoutput()
{
	APPlib *imagelines=NULL;
	short ef=FALSE;
	double tot_purtrn_amt=0.0,tot_purlin_amt=0.0,acc_purtrn_amt=0.0;
	char *purlin_itemid=NULL,*poid=NULL,*line_desc=NULL;
	int line_no=0,line_no1=0,acc_no=0,acc_no1=0,fyear=0,fyear1=0;
	char *purmst_remarks1=NULL,*purmst_remarks2=NULL;
	char *purmst_remarks3=NULL,*purmst_remarks4=NULL;
	char *purmst_remarks5=NULL,*purmst_remarks6=NULL;
	char *purmst_remarks7=NULL,*purmst_remarks8=NULL;
	char *purlin_comments1=NULL,*purlin_comments2=NULL;
	char *purlin_comments3=NULL,*purlin_comments4=NULL;
	char *purlin_comments5=NULL,*purlin_comments6=NULL;
	char *purlin_comments7=NULL,*purlin_comments8=NULL;
	char *enc_code=NULL,*acc_desc=NULL,*tempstr=NULL;
	double purlin_qty=0.0,unit_amt=0.0,purlin_total=0.0;
	double ship_amt=0.0,purtrn_amt=0.0;

	imagelines=APPlibNEW();
	ZERNRD(sortfile->fileno);
	ef=FRSNRD(sortfile->fileno,1);
	FINDFLDGETINT(sortfile->fileno,"FISCAL YEAR",&fyear);
	FINDFLDGETSTRING(sortfile->fileno,"PO IDENTIFICATION",&poid);
	FINDFLDGETINT(sortfile->fileno,"LINE NUMBER",&line_no);
	FINDFLDGETINT(sortfile->fileno,"ACCOUNT NUMBER",&acc_no);
	fyear1=fyear;
	poid1=stralloc(poid);
	line_no1=line_no;
	acc_no1=acc_no;
	
	while(!ef)
	{
		FINDFLDGETINT(sortfile->fileno,"FISCAL YEAR",&fyear);
		FINDFLDGETSTRING(sortfile->fileno,"PO IDENTIFICATION",&poid);
		FINDFLDGETINT(sortfile->fileno,"LINE NUMBER",&line_no);
		FINDFLDGETINT(sortfile->fileno,"ACCOUNT NUMBER",&acc_no);
/* IMAGE LINES  */

/* PO ACCOUNT NUMBER FOOTERS */
		if(diagapps)
		{
			prterr("poid: %s poid1: %s fyear: %i fyear1: %i lineno: %i lineno1: %i accno: %i accno1: %i\n",poid,poid1,fyear,fyear1,line_no,line_no1,acc_no,acc_no1);
		}
		if((fyear!=fyear1) || (RDAstrcmp(poid,poid1)) || (line_no!=line_no1) || (acc_no!=acc_no1))
		{
			if(diagapps)
			{
				prterr("Enc. Code: %s Print Accounts: %s Tot. Amt.: %f\n",(enc_code!=NULL ? enc_code:""),(print_po_accounts ? "Y":"N"),tot_purtrn_amt);
			}
			if((!isEMPTY(enc_code)) && print_po_accounts && acc_purtrn_amt!=0)
			{
				if(diagapps)
				{
					prterr("Disable Acc. Desc.: %s\n",
						(disable_account_desc ? "Y":"N") );
				}
				if(long_acct_code==FALSE)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%23s%-36.36s  %9.2f\r\n","",enc_code,acc_purtrn_amt/100);
					addAPPlib(imagelines,stemp);
				} else {
					memset(stemp,0,101);
					sprintf(stemp,"%19s%40s  %9.2f\r\n","",enc_code,acc_purtrn_amt/100);
					addAPPlib(imagelines,stemp);
				}
				if(disable_account_desc==FALSE)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%23s%-36.36s\r\n","",(acc_desc!=NULL ? acc_desc:""));
					addAPPlib(imagelines,stemp);
				}
			}
			acc_purtrn_amt=0.0;
		}
/* END PO ACCOUNT NUMBER FOOTERS */

/* PO LINE NUMBER FOOTERS */
		if((fyear!=fyear1) || (RDAstrcmp(poid,poid1)) || (line_no!=line_no1))
		{
			tot_purlin_amt+=purlin_total;
			if(print_purlin_comments)
			{
				if(!isEMPTY(purlin_comments1))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments1);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purlin_comments2))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments2);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purlin_comments3))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments3);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purlin_comments4))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments4);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purlin_comments5))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments5);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purlin_comments6))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments6);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purlin_comments7))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments7);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purlin_comments8))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purlin_comments8);
					addAPPlib(imagelines,stemp);
				}
			}
			if(print_po_accounts && acc_no1>0) /* to be printed only when last record is a transaction */
			{
				memset(stemp,0,101);
				sprintf(stemp,"\r\n");
				addAPPlib(imagelines,stemp);
			}
		}
/* END PO LINE NUMBER FOOTERS */

/* PO ID FOOTERS */
		if((fyear!=fyear1) || (RDAstrcmp(poid,poid1)))
		{
			if(diagapps)
			{
				prterr("Line 367 poid: %s poid1: %s fyear: %i fyear1: %i \n",poid,poid1,fyear,fyear1);
			}
			memset(stemp,0,101);
			sprintf(stemp,"                                                                 ===============\r\n");
			addAPPlib(imagelines,stemp);
			if(use_po_lines)
			{
				tempstr=famt(tot_purlin_amt,15);
				memset(stemp,0,101);
				sprintf(stemp,"                                                           TOTAL %-15s\r\n",tempstr);
				addAPPlib(imagelines,stemp);
				if(tempstr!=NULL) Rfree(tempstr);
			} else {
				tempstr=famt(tot_purtrn_amt,15);
				memset(stemp,0,101);
				sprintf(stemp,"                                                           TOTAL %-15s\r\n",tempstr);
				addAPPlib(imagelines,stemp);
				if(tempstr!=NULL) Rfree(tempstr);
			}
			memset(stemp,0,101);
			sprintf(stemp,"\r\n");
			addAPPlib(imagelines,stemp);
			if(print_po_comments)
			{
				if(!isEMPTY(purmst_remarks1))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks1);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purmst_remarks2))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks2);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purmst_remarks3))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks3);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purmst_remarks4))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks4);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purmst_remarks5))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks5);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purmst_remarks6))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks6);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purmst_remarks7))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks7);
					addAPPlib(imagelines,stemp);
				}
				if(!isEMPTY(purmst_remarks8))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%-80s\r\n",purmst_remarks8);
					addAPPlib(imagelines,stemp);
				}
			}
			memset(stemp,0,101);
			sprintf(stemp,"\r\n");
			addAPPlib(imagelines,stemp);
			if(address_position!=3)
			{
				memset(stemp,0,101);
				sprintf(stemp,"\r\n");
				addAPPlib(imagelines,stemp);
			}
			printactualpos(imagelines,fyear1);
			if(imagelines!=NULL) freeapplib(imagelines);
			imagelines=APPlibNEW();
			tot_purlin_amt=0.0;
			tot_purtrn_amt=0.0;
		}
/* END PO ID FOOTERS */

		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 1",&purmst_remarks1);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 2",&purmst_remarks2);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 3",&purmst_remarks3);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 4",&purmst_remarks4);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 5",&purmst_remarks5);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 6",&purmst_remarks6);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 7",&purmst_remarks7);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST REMARKS 8",&purmst_remarks8);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 1",&purlin_comments1);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 2",&purlin_comments2);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 3",&purlin_comments3);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 4",&purlin_comments4);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 5",&purlin_comments5);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 6",&purlin_comments6);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 7",&purlin_comments7);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN COMMENTS 8",&purlin_comments8);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN DESCRIPTION",&line_desc);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN ITEM IDENTIFICATION",&purlin_itemid);
		FINDFLDGETDOUBLE(sortfile->fileno,"PURLIN UNIT AMOUNT",&unit_amt);
		FINDFLDGETDOUBLE(sortfile->fileno,"PURLIN AMOUNT",&purlin_total);
		FINDFLDGETDOUBLE(sortfile->fileno,"PURLIN SHIPPING AMOUNT",&ship_amt);
		FINDFLDGETSTRING(sortfile->fileno,"VENDOR NAME",&vendor_name);
		FINDFLDGETSTRING(sortfile->fileno,"VENDOR ADDRESS 1",&vendor_addr1);
		FINDFLDGETSTRING(sortfile->fileno,"VENDOR ADDRESS 2",&vendor_addr2);
		FINDFLDGETSTRING(sortfile->fileno,"VENDOR ADDRESS 3",&vendor_addr3);
		FINDFLDGETSTRING(sortfile->fileno,"VENADR PHONE",&vendor_phone);
		FINDFLDGETSTRING(sortfile->fileno,"VENADR FAX",&vendor_fax);
		FINDFLDGETSTRING(sortfile->fileno,"SHIPPING ADDRESS 1",&shipping_addr1);
		FINDFLDGETSTRING(sortfile->fileno,"SHIPPING ADDRESS 2",&shipping_addr2);
		FINDFLDGETSTRING(sortfile->fileno,"SHIPPING ADDRESS 3",&shipping_addr3);
		FINDFLDGETSTRING(sortfile->fileno,"SHIPPING ADDRESS 4",&shipping_addr4);
		FINDFLDGETSTRING(sortfile->fileno,"SHIPPING ADDRESS 4",&shipping_addr4);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST ATTENTION TO",&purmst_attention_to);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST SALESMAN",&purmst_salesman);
		FINDFLDGETSTRING(sortfile->fileno,"PURSIT PHONE",&pursit_phone);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST DELIVERY DATE",&purmst_dlvrydt);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST SHIP VIA",&purmst_shipvia);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST FOB",&purmst_fob);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST TERMS",&purmst_terms);
		FINDFLDGETSTRING(sortfile->fileno,"PURMST ORDERED BY",&purmst_ordered_by);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN ENTRY DATE",&purlin_entrydt);
		FINDFLDGETDOUBLE(sortfile->fileno,"PURTRN AMOUNT",&purtrn_amt);
		FINDFLDGETSTRING(sortfile->fileno,"VENDOR IDENTIFICATION",&vendor_id);
		FINDFLDGETSTRING(sortfile->fileno,"PO IDENTIFICATION",&poid);
		FINDFLDGETSTRING(sortfile->fileno,"ACCOUNT CODE DESCRIPTION",&acc_desc);
		FINDFLDGETSTRING(sortfile->fileno,"PURACC ENCUMBRANCE CODE",&enc_code);
		FINDFLDGETDOUBLE(sortfile->fileno,"PURLIN QUANTITY",&purlin_qty);
		FINDFLDGETSTRING(sortfile->fileno,"PURLIN UNIT",&purlin_unit);
		tot_purtrn_amt+=purtrn_amt;
		acc_purtrn_amt+=purtrn_amt;



/* DETAIL LINES */
		memset(stemp,0,101);
		if(RDAstrcmp(line_desc,"NOT ASSOCIATED"))
		{
			if(isEMPTY(enc_code))
			{
				if(ship_amt==0.0)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%4.0f %-4s %-12.12s %-36.36s   %8.2f %9.2f\r\n",purlin_qty,
						(purlin_unit!=NULL ? purlin_unit:""),(purlin_itemid!=NULL ? purlin_itemid:""),
						(line_desc!=NULL ? line_desc:""),unit_amt,purlin_total/100);
					addAPPlib(imagelines,stemp);
				} else {
					if(diagapps)
					{
						prterr("Shipping: %f \n",ship_amt);
					}
					memset(stemp,0,101);
					sprintf(stemp,"%4.0f %-4s %-12.12s %-36.36s   %8.2f \r\n",purlin_qty,
						(purlin_unit!=NULL ? purlin_unit:""),
						(purlin_itemid!=NULL ? purlin_itemid:""),
						(line_desc!=NULL ? line_desc:""),unit_amt);
					addAPPlib(imagelines,stemp);
					memset(stemp,0,101);
					sprintf(stemp,
						"                                                         SHIPPING %5.2f  %6.2f\r\n",
						ship_amt/100,purlin_total/100);
					addAPPlib(imagelines,stemp);
				}
			}
		} else {
			addAPPlib(imagelines,"\r\n");
		}

/* END DETAIL LINES */

		fyear1=fyear;
		poid1=stralloc(poid);
		line_no1=line_no;
		acc_no1=acc_no;
		ef=NXTNRD(sortfile->fileno,1);
	}
/* PO ACCOUNT NUMBER FOOTERS */
	if(diagapps)
	{
		prterr("LAST RECORD: Enc. Code: %s Print Accounts: %s Tot. Amt.: %f\n",(enc_code!=NULL ? enc_code:""),(print_po_accounts ? "Y":"N"),tot_purtrn_amt);
	}
	if((!isEMPTY(enc_code)) && print_po_accounts && acc_purtrn_amt!=0)
	{
		if(diagapps)
		{
			prterr("Disable Acc. Desc.: %s\n",
				(disable_account_desc ? "Y":"N") );
			prterr("Account Code Description: %s\n",
				(acc_desc!=NULL ? acc_desc:""));
		}
		if(long_acct_code==FALSE)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%23s%-36.36s  %9.2f\r\n","",enc_code,acc_purtrn_amt/100);
			addAPPlib(imagelines,stemp);
		} else {
			memset(stemp,0,101);
			sprintf(stemp,"%19s%40s  %9.2f\r\n","",enc_code,acc_purtrn_amt/100);
			addAPPlib(imagelines,stemp);
		}
		if(disable_account_desc==FALSE)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%23s%-36.36s\r\n","",(acc_desc!=NULL ? acc_desc:""));
			addAPPlib(imagelines,stemp);
		}
	}
/* END PO ACCOUNT NUMBER FOOTERS */

/* PO LINE NUMBER FOOTERS */
	tot_purlin_amt+=purlin_total;
	if(print_purlin_comments)
	{
		if(!isEMPTY(purlin_comments1))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments1);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purlin_comments2))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments2);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purlin_comments3))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments3);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purlin_comments4))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments4);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purlin_comments5))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments5);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purlin_comments6))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments6);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purlin_comments7))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments7);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purlin_comments8))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purlin_comments8);
			addAPPlib(imagelines,stemp);
		}
	}
	if(print_po_accounts && acc_no1>0) /* to be printed only when last record is a transaction */
	{
		memset(stemp,0,101);
		sprintf(stemp,"\r\n");
		addAPPlib(imagelines,stemp);
	}
/* END PO LINE NUMBER FOOTERS */

/* PO ID FOOTERS */
	if(diagapps)
	{
		prterr("Line 367 poid: %s poid1: %s fyear: %i fyear1: %i \n",poid,poid1,fyear,fyear1);
	}
	memset(stemp,0,101);
	sprintf(stemp,"                                                                 ===============\r\n");
	addAPPlib(imagelines,stemp);
	if(use_po_lines)
	{
		tempstr=famt(tot_purlin_amt,15);
		memset(stemp,0,101);
		sprintf(stemp,"                                                           TOTAL %-15s\r\n",tempstr);
		addAPPlib(imagelines,stemp);
		if(tempstr!=NULL) Rfree(tempstr);
	} else {
		tempstr=famt(tot_purtrn_amt,15);
		memset(stemp,0,101);
		sprintf(stemp,"                                                           TOTAL %-15s\r\n",tempstr);
		addAPPlib(imagelines,stemp);
		if(tempstr!=NULL) Rfree(tempstr);
	}
	memset(stemp,0,101);
	sprintf(stemp,"\r\n");
	addAPPlib(imagelines,stemp);
	if(print_po_comments)
	{
		if(!isEMPTY(purmst_remarks1))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks1);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purmst_remarks2))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks2);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purmst_remarks3))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks3);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purmst_remarks4))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks4);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purmst_remarks5))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks5);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purmst_remarks6))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks6);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purmst_remarks7))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks7);
			addAPPlib(imagelines,stemp);
		}
		if(!isEMPTY(purmst_remarks8))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%-80s\r\n",purmst_remarks8);
			addAPPlib(imagelines,stemp);
		}
	}
	memset(stemp,0,101);
	sprintf(stemp,"\r\n");
	addAPPlib(imagelines,stemp);
	if(address_position!=3)
	{
		memset(stemp,0,101);
		sprintf(stemp,"\r\n");
		addAPPlib(imagelines,stemp);
	}
	printactualpos(imagelines,fyear1);
/* END PO ID FOOTERS */
	if(imagelines!=NULL) freeapplib(imagelines);
}
static void printpos(void)
{
	RDApdev *pdev=NULL;
	RDApvar *pvar;
	FILE *pocert=NULL;
	int x=0,y=0,count=0;

	outdevice=stralloc("viewpdf.lnx");
	diagrsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
		"Archiving Purchase Orders",NULL);
	if(diagrsrc!=NULL)
	{
		if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(diagrsrc);
	}
	/* Getting POCERT and calculating number of lines */
	/* Add PCL JOB HEADER here? */
	pdev=RDApdevNEW(outdevice);
	getpdevbin(pdev);
	if(pdev!=NULL)
	{
	if(pdev->pvar!=NULL)
	{
		for(x=0,pvar=pdev->pvar;x<pdev->num;++x,++pvar)
		{
			if(!RDAstrcmp(pvar->name,"POCERT")) break;
		}
		if(x<pdev->num)
		{
			if(pvar->type==1) /* file substitution */
			{
				if(!isEMPTY(pvar->command))
				{
					pocert=fopen(pvar->command,"r");
					if(pocert!=NULL)
					{
						memset(stemp,0,133);
						while((y=readline(pocert,stemp,132))!=(-1))
						{
							if(diagapps)
							{
								prterr("Return value for read length is [%d].\n",y);
							}
							if(pocrt!=NULL)
							{
								pocrt=Rrealloc(pocrt,count+y+3);
							} else {
								pocrt=Rmalloc(y+3);
								count=0;
							}
							sprintf(&pocrt[count],"%s\r\n",stemp);
							count+=(y+2);
							++num_pocrt_lines;
							memset(stemp,0,133);
						}
						fclose(pocert);
						if(num_pocrt_lines>0)
						{
							num_pocrt_lines=ceil(((double)num_pocrt_lines)*6/8);
							if(diagapps)
							{
								prterr("Return value for number of lines is [%d].\n",num_pocrt_lines);
							}
						}
					}
				}
			} else {
				if(diagapps)
				{
					prterr("Error: pvar [%s] not a valid certification file for output device [%s].\n",pvar->command,outdevice);
				}
			}
		} else {
			if(diagapps)
			{
				prterr("Error: pvar [%s] not a valid certification file for output device [%s].\n",pvar->command,outdevice);
			}
		}
	}
	}
	if(pdev!=NULL) free_pdev(pdev);
	createoutput();
	if(outdevice!=NULL) Rfree(outdevice);
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
		diagrsrc=NULL;
	}
	if(pocrt!=NULL) Rfree(pocrt);
	quitprint();
}
static void addsortrecord(RangeScreen *rs,RDArsrc *mainrsrc,short sfile,char added_purtrn)
{

	short account_type=0;
	char *shipping_city=NULL,*shipping_state=NULL,*shipping_zip=NULL;
	char *acc_desc=NULL;
	char *addr1=NULL,*addr2=NULL,*addr3=NULL;

	COPYFIELD(purmst,venmsr,"VENDOR IDENTIFICATION");
	if(ADVEQLNRDsec(venmsr,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(venmsr,1);
	
	COPYFIELD(purmst,venadr,"VENDOR IDENTIFICATION");
	FIELDCOPY(purmst,"PO VENDOR ADDRESS IDENTIFICATION",venadr,"ADDRESS IDENTIFICATION");
	if(ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(venadr,1);

	COPYFIELD(purmst,pursit,"SITE IDENTIFICATION");
	if(ADVEQLNRDsec(pursit,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(pursit,1);

	COPYFIELD(purlin,sfile,"FISCAL YEAR");
	COPYFIELD(purlin,sfile,"PO IDENTIFICATION");
	COPYFIELD(purlin,sfile,"LINE NUMBER");

	FINDFLDSETINT(sfile,"ACCOUNT NUMBER",0);
	FINDFLDSETINT(sfile,"TRANSACTION NUMBER",0);
	FINDFLDSETDOUBLE(sfile,"PURTRN AMOUNT",0.0);
	FINDFLDSETSTRING(sfile,"PURACC ENCUMBRANCE CODE","");
	FINDFLDSETSTRING(sfile,"ACCOUNT CODE DESCRIPTION","");

	COPYFIELD(purmst,sfile,"VENDOR IDENTIFICATION");
	COPYFIELD(venmsr,sfile,"VENDOR NAME");
	FIELDCOPY(purlin,"DESCRIPTION",sfile,"PURLIN DESCRIPTION");
	FIELDCOPY(purlin,"PRORATE AMOUNT",sfile,"PURLIN SHIPPING AMOUNT");
	FIELDCOPY(purlin,"ENTRY DATE",sfile,"PURLIN ENTRY DATE");
	FIELDCOPY(purlin,"TOTAL",sfile,"PURLIN AMOUNT");
	FIELDCOPY(purlin,"ITEM IDENTIFICATION",sfile,"PURLIN ITEM IDENTIFICATION");
	FIELDCOPY(purlin,"UNIT RATE",sfile,"PURLIN UNIT AMOUNT");
	FIELDCOPY(purlin,"QUANTITY",sfile,"PURLIN QUANTITY");
	FIELDCOPY(purlin,"UNIT",sfile,"PURLIN UNIT");
	FIELDCOPY(purmst,"DELIVERY DATE",sfile,"PURMST DELIVERY DATE");
	FIELDCOPY(purmst,"SHIP VIA",sfile,"PURMST SHIP VIA");
	FIELDCOPY(purmst,"FOB",sfile,"PURMST FOB");
	FIELDCOPY(purmst,"TERMS",sfile,"PURMST TERMS");
	FIELDCOPY(pursit,"PHONE",sfile,"PURSIT PHONE");
	FIELDCOPY(purmst,"SALESMAN",sfile,"PURMST SALESMAN");
	FIELDCOPY(purmst,"ATTENTION TO",sfile,"PURMST ATTENTION TO");
	FIELDCOPY(purmst,"ORDERED BY",sfile,"PURMST ORDERED BY");
	FIELDCOPY(purmst,"REMARKS 1",sfile,"PURMST REMARKS 1");
	FIELDCOPY(purmst,"REMARKS 2",sfile,"PURMST REMARKS 2");
	FIELDCOPY(purmst,"REMARKS 3",sfile,"PURMST REMARKS 3");
	FIELDCOPY(purmst,"REMARKS 4",sfile,"PURMST REMARKS 4");
	FIELDCOPY(purmst,"REMARKS 5",sfile,"PURMST REMARKS 5");
	FIELDCOPY(purmst,"REMARKS 6",sfile,"PURMST REMARKS 6");
	FIELDCOPY(purmst,"REMARKS 7",sfile,"PURMST REMARKS 7");
	FIELDCOPY(purmst,"REMARKS 8",sfile,"PURMST REMARKS 8");
	FIELDCOPY(purlin,"COMMENTS 1",sfile,"PURLIN COMMENTS 1");
	FIELDCOPY(purlin,"COMMENTS 2",sfile,"PURLIN COMMENTS 2");
	FIELDCOPY(purlin,"COMMENTS 3",sfile,"PURLIN COMMENTS 3");
	FIELDCOPY(purlin,"COMMENTS 4",sfile,"PURLIN COMMENTS 4");
	FIELDCOPY(purlin,"COMMENTS 5",sfile,"PURLIN COMMENTS 5");
	FIELDCOPY(purlin,"COMMENTS 6",sfile,"PURLIN COMMENTS 6");
	FIELDCOPY(purlin,"COMMENTS 7",sfile,"PURLIN COMMENTS 7");
	FIELDCOPY(purlin,"COMMENTS 8",sfile,"PURLIN COMMENTS 8");
	FIELDCOPY(venadr,"PHONE",sfile,"VENADR PHONE");
	FIELDCOPY(venadr,"FAX",sfile,"VENADR FAX");
	FINDFLDGETSTRING(pursit,"ADDRESS 1",&addr1);
	FINDFLDGETSTRING(pursit,"ADDRESS 2",&addr2);
	FINDFLDGETSTRING(pursit,"ADDRESS 3",&addr3);

	if(!isEMPTY(addr1))
	{
		FIELDCOPY(pursit,"ADDRESS 1",sfile,"SHIPPING ADDRESS 1");
	}
	if(!isEMPTY(addr2))
	{
		FIELDCOPY(pursit,"ADDRESS 2",sfile,"SHIPPING ADDRESS 2");
	}
	if(!isEMPTY(addr3))
	{
		FIELDCOPY(pursit,"ADDRESS 3",sfile,"SHIPPING ADDRESS 3");
	}
	FINDFLDGETSTRING(pursit,"CITY",&shipping_city);
	FINDFLDGETSTRING(pursit,"STATE",&shipping_state);
	FINDFLDGETSTRING(pursit,"ZIP",&shipping_zip);
	memset(stemp,0,101);
	sprintf(stemp,"%s, %s %s",
		(shipping_city!=NULL ? shipping_city:""),
		(shipping_state!=NULL ? shipping_state:""),
		(shipping_zip!=NULL ? shipping_zip:""));
	if(RDAstrlen(stemp)>40) stemp[40]=0;
	if(isEMPTY(addr1)) 
	{
	FINDFLDSETSTRING(sfile,"SHIPPING ADDRESS 1",stemp);
	} else if(isEMPTY(addr2))
	{
	FINDFLDSETSTRING(sfile,"SHIPPING ADDRESS 2",stemp);
	} else if(isEMPTY(addr3))
	{
	FINDFLDSETSTRING(sfile,"SHIPPING ADDRESS 3",stemp);
	} else FINDFLDSETSTRING(sfile,"SHIPPING ADDRESS 4",stemp);

	if(VendorAddr1!=NULL)
	{
		VendorAddr1->computed=FALSE;
		computevirtual(VendorAddr1,SCRNvirtualSubData,mainrsrc);
		FINDFLDSETSTRING(sfile,"VENDOR ADDRESS 1",VendorAddr1->value.string_value);
	}
	if(VendorAddr2!=NULL)
	{
		VendorAddr2->computed=FALSE;
		computevirtual(VendorAddr2,SCRNvirtualSubData,mainrsrc);
		FINDFLDSETSTRING(sfile,"VENDOR ADDRESS 2",VendorAddr2->value.string_value);
	}
	if(VendorAddr3!=NULL)
	{
		VendorAddr3->computed=FALSE;
		computevirtual(VendorAddr3,SCRNvirtualSubData,mainrsrc);
		FINDFLDSETSTRING(sfile,"VENDOR ADDRESS 3",VendorAddr3->value.string_value);
	}

	if(added_purtrn)
	{
		COPYFIELD(purtrn,sfile,"ACCOUNT NUMBER");
		COPYFIELD(purtrn,sfile,"TRANSACTION NUMBER");
		FIELDCOPY(purtrn,"AMOUNT",sfile,"PURTRN AMOUNT");
		FINDFLDGETSHORT(puracc,"ACCOUNT TYPE",&account_type);
		switch(account_type)
		{
			case 0:
			FIELDCOPY(puracc,"ACCOUNT CODE",sfile,"PURACC ENCUMBRANCE CODE");
			FIELDCOPY(puracc,"ACCOUNT CODE",fineacc,"ACCOUNT CODE");
			FIELDCOPY(puracc,"DEFINITION TYPE",fineacc,"DEFINITION TYPE");
			if(ADVEQLNRDsec(fineacc,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(fineacc,1);
			FIELDCOPY(fineacc,"DESCRIPTION",sfile,"ACCOUNT CODE DESCRIPTION");
			if(diagapps)
			{
				FINDFLDGETSTRING(fineacc,"DESCRIPTION",&acc_desc);
				prterr("Acc. Desc.: %s\n",acc_desc);
				if(acc_desc!=NULL) Rfree(acc_desc);
			}
			break;
			case 1:
			FIELDCOPY(puracc,"ACCOUNT CODE",sfile,"PURACC ENCUMBRANCE CODE");
			FIELDCOPY(puracc,"ACCOUNT CODE",finracc,"ACCOUNT CODE");
			FIELDCOPY(puracc,"DEFINITION TYPE",finracc,"DEFINITION TYPE");
			if(ADVEQLNRDsec(finracc,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(finracc,1);
			FIELDCOPY(finracc,"DESCRIPTION",sfile,"ACCOUNT CODE DESCRIPTION");
			break;
			case 2:
			FIELDCOPY(puracc,"DEFAULT DEBIT CODE",sfile,"PURACC ENCUMBRANCE CODE");
			FIELDCOPY(puracc,"DEFAULT DEBIT CODE",finbacc,"ACCOUNT CODE");
			FIELDCOPY(puracc,"DEFINITION TYPE",finbacc,"DEFINITION TYPE");
			if(ADVEQLNRDsec(finbacc,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(finbacc,1);
			FIELDCOPY(finbacc,"DESCRIPTION",sfile,"ACCOUNT CODE DESCRIPTION");
		}
	}
	WRTNRD(sfile,1,NULL);
	if(addr1!=NULL) Rfree(addr1);
	if(addr2!=NULL) Rfree(addr2);
	if(addr3!=NULL) Rfree(addr3);
}
static void CreateSortFile(RangeScreen *rs,RDArsrc *mainrsrc,short sfile)
{
	short ef=0,ef1=0,ef2=0,ef3=0,diag_update=FALSE;
	char deleteflag=FALSE,verified=FALSE,disenc=FALSE;
	char *poid=NULL,added_purtrn=FALSE,*poid1=NULL;
	int lineno=0,lineno1=0;
	short approve=0,approve_type=0;
	int fyear=0,fyear1=0,accno=0,accno1=0;


	diagrsrc=diagscrn(purmst,"DIAGNOSTIC SCREEN",module,
		"Selecting Purchase Orders",NULL);
	if(diagrsrc!=NULL)
	{
		if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(diagrsrc);
	}
	ZERNRD(purmst);
	if(selected_poid!=NULL)
	{
		FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",selected_poid);
		FINDFLDSETINT(purmst,"FISCAL YEAR",selected_fyear);
		ef=ADVGTENRDsec(purmst,1,SCRNvirtualSubData,diagrsrc);
	} else {
		ef=ADVFRSNRDsec(purmst,1,SCRNvirtualSubData,diagrsrc);
	}
	while(!ef)
	{
		diag_update=FALSE;
		FINDFLDGETSTRING(purmst,"PO IDENTIFICATION",&poid);
		FINDFLDGETINT(purmst,"FISCAL YEAR",&fyear);
		if(selected_poid!=NULL)
		{
			if((RDAstrcmp(selected_poid,poid)) || (fyear!=selected_fyear)) break;
		}
		FINDFLDGETCHAR(purmst,"DELETEFLAG",&deleteflag);
		FINDFLDGETSHORT(purmst,"APPROVE TYPE",&approve_type);
		FINDFLDGETSHORT(purmst,"APPROVE",&approve);
		if(!deleteflag && !testfilerangersrc(purmst,rs) && (approve_type==0 || approve==1))
		{
			COPYFIELD(purmst,purlin,"PO IDENTIFICATION");
			COPYFIELD(purmst,purlin,"FISCAL YEAR");
			ef1=ADVGTENRDsec(purlin,1,SCRNvirtualSubData,diagrsrc);
			while(!ef1)
			{
				FINDFLDGETSTRING(purlin,"PO IDENTIFICATION",&poid1);
				FINDFLDGETINT(purlin,"FISCAL YEAR",&fyear1);
				if((RDAstrcmp(poid,poid1)) || (fyear!=fyear1)) break;
				FINDFLDGETCHAR(purlin,"DELETEFLAG",&deleteflag);
				if(!deleteflag && !testfilerangersrc(purlin,rs))
				{
					added_purtrn=FALSE;
					COPYFIELD(purlin,puracc,"PO IDENTIFICATION");
					COPYFIELD(purlin,puracc,"FISCAL YEAR");
					COPYFIELD(purlin,puracc,"LINE NUMBER");
					FINDFLDGETINT(purlin,"LINE NUMBER",&lineno);
					if(added_purtrn==FALSE)
					{
						if(diagapps)
						{
							prterr("added_purtrn=FALSE poid: %s poid1: %s fyear: %i fyear1: %i lineno: %i lineno1: %i accno: %i accno1: %i\n",poid,poid1,fyear,fyear1,lineno,lineno1,accno,accno1);
						}
						addsortrecord(rs,mainrsrc,sfile,added_purtrn);
						DID_STUFF=TRUE;
						diag_update=TRUE;
					}
					ef2=ADVGTENRDsec(puracc,1,SCRNvirtualSubData,diagrsrc);
					while(!ef2)
					{
						FINDFLDGETSTRING(puracc,"PO IDENTIFICATION",&poid1);
						FINDFLDGETINT(puracc,"FISCAL YEAR",&fyear1);
						FINDFLDGETINT(puracc,"LINE NUMBER",&lineno1);
						if((RDAstrcmp(poid,poid1)) || (fyear!=fyear1) || (lineno!=lineno1)) break;
						FINDFLDGETCHAR(puracc,"DELETEFLAG",&deleteflag);
						if(!deleteflag && !testfilerangersrc(puracc,rs))
						{
							COPYFIELD(puracc,purtrn,"PO IDENTIFICATION");
							COPYFIELD(puracc,purtrn,"FISCAL YEAR");
							COPYFIELD(puracc,purtrn,"LINE NUMBER");
							COPYFIELD(puracc,purtrn,"ACCOUNT NUMBER");
							FINDFLDGETINT(puracc,"ACCOUNT NUMBER",&accno);
							ef3=ADVGTENRDsec(purtrn,1,SCRNvirtualSubData,diagrsrc);
							while(!ef3)
							{
								FINDFLDGETSTRING(purtrn,"PO IDENTIFICATION",&poid1);
								FINDFLDGETINT(purtrn,"FISCAL YEAR",&fyear1);
								FINDFLDGETINT(purtrn,"LINE NUMBER",&lineno1);
								FINDFLDGETINT(purtrn,"ACCOUNT NUMBER",&accno1);
								if(diagapps)
								{
									prterr("poid: %s poid1: %s fyear: %i fyear1: %i lineno: %i lineno1: %i accno: %i accno1: %i\n",poid,poid1,fyear,fyear1,lineno,lineno1,accno,accno1);
								}
								if((RDAstrcmp(poid,poid1)) || (fyear!=fyear1) || 
									(lineno!=lineno1) || (accno!=accno1)) break;
								FINDFLDGETCHAR(purtrn,"DELETEFLAG",&deleteflag);
								FINDFLDGETCHAR(purtrn,"SOURCE USER VERIFIED",&verified);
								FINDFLDGETCHAR(purtrn,"VOUCHER DISENCUMBRANCE",&disenc);
								if(!deleteflag && verified && !disenc &&
									!testfilerangersrc(purtrn,rs))
								{
									added_purtrn=TRUE;
									addsortrecord(rs,mainrsrc,sfile,added_purtrn);
									DID_STUFF=TRUE;
									diag_update=TRUE;
								}
								ef3=ADVNXTNRDsec(purtrn,1,SCRNvirtualSubData,diagrsrc);
							}
						}
						ef2=ADVNXTNRDsec(puracc,1,SCRNvirtualSubData,diagrsrc);
					}
				}
				ef1=ADVNXTNRDsec(purlin,1,SCRNvirtualSubData,diagrsrc);
			}
		}

		/* uncompute virtual fields */
		/*
		if(!deleteflag && computed && !testfilerangersrc(perdmg,rs) 
			&& !testfilerangersrc(payppms,rs) && !RDAstrcmp(payid,payid1))
		{
			COPYFIELD(payppms,paymst,"PAY IDENTIFICATION");
			if(ADVEQLNRDsec(paymst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(paymst,1);
			for(x=0,v=mainrsrc->virflds; x<mainrsrc->num;++x,++v)
			{
				if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mainrsrc)) 
					break;
			}
			if(x>=mainrsrc->num)
			{
				doit=TRUE;
			} else {
				doit=FALSE;
			}

			if(doit==TRUE)
			{
				FINDFLDGETSTRING(payppms,"PERSONNEL IDENTIFICATION",&empid);
				if(AtLeastOneJob(empid,rs,mainrsrc))
				{
					diag_update=TRUE;
				}
			}
		}
		if(diagrsrc!=NULL) 
			update_diagnostic(diagrsrc,
				(diag_update==TRUE ? TRUE:FALSE));
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		*/
		if(diagrsrc!=NULL) update_diagnostic(diagrsrc,diag_update);
		ef=ADVNXTNRDsec(purmst,1,SCRNvirtualSubData,diagrsrc);
	}
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
		diagrsrc=NULL;
	}
}
static void selectpos(RDArsrc *r)
{
	char *sortname=NULL,*temp1=NULL;
	RangeScreen *rs=NULL;
	DFvirtual *d=NULL;
	int x=0;

	readallwidgets(r);
	rs=RangeScreenNew(r);
	ReadRangeScreen(purmst,rs);
	ReadRangeScreen(purlin,rs);
	ReadRangeScreen(puracc,rs);
	/*
	if(r->virflds!=NULL)
	{
		for(x=0,v=r->virflds;x<r->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	*/

	d=getDFvirtual("PURORD","ORGANIZATIONAL ADDRESS 1");
	if(d!=NULL)
	{
		org_addr1=Rmalloc(sizeof(RDAvirtual));
		org_addr1->name=stralloc(d->name);
		org_addr1->when=0;
		org_addr1->len=d->length;
		org_addr1->type=d->type;
		if(d->expression!=NULL) 
		{
			org_addr1->expression=PP_translate(d->expression);
		} else {
			org_addr1->expression=NULL;
		}
		org_addr1->computed=FALSE;
		org_addr1->range=FALSE;
		org_addr1->dlen=0;
		switch(org_addr1->type)
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
				org_addr1->value.string_value=NULL;
				org_addr1->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				org_addr1->value.string_value=Rmalloc(1);
				*org_addr1->value.string_value=0;
				org_addr1->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				org_addr1->value.float_value=Rmalloc(sizeof(double));
				*org_addr1->value.float_value=0;
				org_addr1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				org_addr1->value.short_value=Rmalloc(sizeof(short));
				*org_addr1->value.short_value=0;
				org_addr1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				org_addr1->value.integer_value=Rmalloc(sizeof(int));
				*org_addr1->value.integer_value=0;
				org_addr1->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",org_addr1->type,org_addr1->name);
				break;
		}
		FreeDFvirtual(d);
	}

	d=getDFvirtual("PURORD","ORGANIZATIONAL ADDRESS 2");
	if(d!=NULL)
	{
		org_addr2=Rmalloc(sizeof(RDAvirtual));
		org_addr2->name=stralloc(d->name);
		org_addr2->when=0;
		org_addr2->len=d->length;
		org_addr2->type=d->type;
		if(d->expression!=NULL) 
		{
			org_addr2->expression=PP_translate(d->expression);
		} else {
			org_addr2->expression=NULL;
		}
		org_addr2->computed=FALSE;
		org_addr2->range=FALSE;
		org_addr2->dlen=0;
		switch(org_addr2->type)
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
				org_addr2->value.string_value=NULL;
				org_addr2->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				org_addr2->value.string_value=Rmalloc(1);
				*org_addr2->value.string_value=0;
				org_addr2->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				org_addr2->value.float_value=Rmalloc(sizeof(double));
				*org_addr2->value.float_value=0;
				org_addr2->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				org_addr2->value.short_value=Rmalloc(sizeof(short));
				*org_addr2->value.short_value=0;
				org_addr2->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				org_addr2->value.integer_value=Rmalloc(sizeof(int));
				*org_addr2->value.integer_value=0;
				org_addr2->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",org_addr2->type,org_addr2->name);
				break;
		}
		FreeDFvirtual(d);
	}

	d=getDFvirtual("PURORD","ORGANIZATIONAL ADDRESS 3");
	if(d!=NULL)
	{
		org_addr3=Rmalloc(sizeof(RDAvirtual));
		org_addr3->name=stralloc(d->name);
		org_addr3->when=0;
		org_addr3->len=d->length;
		org_addr3->type=d->type;
		if(d->expression!=NULL) 
		{
			org_addr3->expression=PP_translate(d->expression);
		} else {
			org_addr3->expression=NULL;
		}
		org_addr3->computed=FALSE;
		org_addr3->range=FALSE;
		org_addr3->dlen=0;
		switch(org_addr3->type)
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
				org_addr3->value.string_value=NULL;
				org_addr3->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				org_addr3->value.string_value=Rmalloc(1);
				*org_addr3->value.string_value=0;
				org_addr3->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				org_addr3->value.float_value=Rmalloc(sizeof(double));
				*org_addr3->value.float_value=0;
				org_addr3->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				org_addr3->value.short_value=Rmalloc(sizeof(short));
				*org_addr3->value.short_value=0;
				org_addr3->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				org_addr3->value.integer_value=Rmalloc(sizeof(int));
				*org_addr3->value.integer_value=0;
				org_addr3->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",org_addr3->type,org_addr3->name);
				break;
		}
		FreeDFvirtual(d);
	}
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

	sortname=unique_name();
	sortfile=DBsortNEW("PURORD",sortname,NRDMyIsamEngine);
	if(sortname!=NULL) Rfree(sortname);
	addDBfield(sortfile,"DELETEFLAG",BOOLNS,1);
	addDBfield(sortfile,"FISCAL YEAR",LONGV,4);
	addDBfield(sortfile,"PO IDENTIFICATION",PLAINTEXT,15);
	addDBfield(sortfile,"ACCOUNT NUMBER",LONGV,4);
	addDBfield(sortfile,"LINE NUMBER",LONGV,4);
	addDBfield(sortfile,"TRANSACTION NUMBER",LONGV,6);
	addDBfield(sortfile,"VENDOR IDENTIFICATION",PLAINTEXT,30);
	addDBfield(sortfile,"VENDOR NAME",PLAINTEXT,40);
	addDBfield(sortfile,"VENDOR ADDRESS 1",PLAINTEXT,40);
	addDBfield(sortfile,"VENDOR ADDRESS 2",PLAINTEXT,40);
	addDBfield(sortfile,"VENDOR ADDRESS 3",PLAINTEXT,40);
	addDBfield(sortfile,"PURLIN ITEM IDENTIFICATION",1,15);
	addDBfield(sortfile,"PURLIN DESCRIPTION",PLAINTEXT,40);
	addDBfield(sortfile,"PURMST ATTENTION TO",PLAINTEXT,25);
	addDBfield(sortfile,"PURMST DELIVERY DATE",DATES,10);
	addDBfield(sortfile,"PURMST SHIP VIA",PLAINTEXT,15);
	addDBfield(sortfile,"PURMST FOB",PLAINTEXT,20);
	addDBfield(sortfile,"PURMST ORDERED BY",PLAINTEXT,25);
	addDBfield(sortfile,"PURMST SALESMAN",PLAINTEXT,15);
	addDBfield(sortfile,"PURLIN ENTRY DATE",DATES,10);
	addDBfield(sortfile,"PURLIN AMOUNT",2,20);
	addDBfield(sortfile,"PURLIN SHIPPING AMOUNT",2,20);
	addDBfield(sortfile,"PURLIN UNIT AMOUNT",6,10);
	addDBfield(sortfile,"PURLIN QUANTITY",6,10);
	addDBfield(sortfile,"PURLIN UNIT",1,4);
	addDBfield(sortfile,"PURSIT PHONE",3,12);
	addDBfield(sortfile,"PURMST TERMS",1,40);
	addDBfield(sortfile,"PURTRN AMOUNT",2,20);
	addDBfield(sortfile,"VENADR PHONE",3,12);
	addDBfield(sortfile,"VENADR FAX",3,12);
	addDBfield(sortfile,"PURACC ENCUMBRANCE CODE",1,40);
	addDBfield(sortfile,"ACCOUNT CODE DESCRIPTION",1,40);
	addDBfield(sortfile,"SHIPPING ADDRESS 1",PLAINTEXT,40);
	addDBfield(sortfile,"SHIPPING ADDRESS 2",PLAINTEXT,40);
	addDBfield(sortfile,"SHIPPING ADDRESS 3",PLAINTEXT,40);
	addDBfield(sortfile,"SHIPPING ADDRESS 4",PLAINTEXT,40);
	memset(stemp,0,101);
	for(x=1;x<9;++x)
	{
		sprintf(stemp,"PURMST REMARKS %d",x);
		addDBfield(sortfile,stemp,PLAINTEXT,80);
	}
	memset(stemp,0,101);
	for(x=1;x<9;++x)
	{
		sprintf(stemp,"PURLIN COMMENTS %d",x);
		addDBfield(sortfile,stemp,PLAINTEXT,80);
	}

/* Key Info */
	addDBkey(sortfile,"PRINT PO KEY");
	addDBkeypart(sortfile,1,"FISCAL YEAR");
	addDBkeypart(sortfile,1,"PO IDENTIFICATION");
	addDBkeypart(sortfile,1,"LINE NUMBER");
	addDBkeypart(sortfile,1,"ACCOUNT NUMBER");
	addDBkeypart(sortfile,1,"TRANSACTION NUMBER");
	sortfile->fileno=OPNDBsort(sortfile);
	if(sortfile->fileno==(-1))
	{
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(6+7+77+1+RDAstrlen(sortfile->filename));
		sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [PAYROLL] Filename [%s], Can Not Save Database File!",sortfile->filename);
		prterr(temp1);
		return;
	}
	/*
	FINDRSCGETSTRING(r,"MESSAGE 1",&message1);
	FINDRSCGETSTRING(r,"MESSAGE 9",&message9);
	FINDRSCGETINT(r,"PROCESS EXCEPTIONS TYPES",&process_except);
	*/

	CreateSortFile(rs,r,sortfile->fileno);
	if(DID_STUFF)
	{	
	/*
		diagrsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
			"Collecting Details",NULL);
		if(diagrsrc!=NULL)
		{
			if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(diagrsrc);
		}
	*/
		if(XPERT_SETUP->company_name!=NULL)
		{
			OrgName=stralloc(XPERT_SETUP->company_name);
		}
		if(XPERT_SETUP->phone!=NULL)
		{
			OrgPhone=stralloc(XPERT_SETUP->phone);
		}
		if(XPERT_SETUP->fax!=NULL)
		{
			OrgFax=stralloc(XPERT_SETUP->fax);
		}
		if(org_addr1!=NULL)
		{
			org_addr1->computed=FALSE;
			computevirtual(org_addr1,SCRNvirtualSubData,r);
			OrgAddr1=stralloc(org_addr1->value.string_value);
		}
		if(org_addr2!=NULL)
		{
			org_addr2->computed=FALSE;
			computevirtual(org_addr2,SCRNvirtualSubData,r);
			OrgAddr2=stralloc(org_addr2->value.string_value);
		}
		if(org_addr3!=NULL)
		{
			org_addr3->computed=FALSE;
			computevirtual(org_addr3,SCRNvirtualSubData,r);
			OrgAddr3=stralloc(org_addr3->value.string_value);
		}
		if(diagrsrc!=NULL)
		{
			killwindow(diagrsrc);
			free_rsrc(diagrsrc);
			diagrsrc=NULL;
		}
#ifdef XXX
		switch(process_except)
		{
			default:
			case 1: /* Automatic */
				runoutputscreen();
				break;
			case 0: /* Process Exceptions */
				brsrc=RDArsrcNEW(module,"PA RETIREMENT REPORT BROWSE");
				srsrc=RDArsrcNEW(module,"PA RETIREMENT REPORT SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"PA RETIREMENT REPORT DEFINE LIST");
				addDFincvir(brsrc,module,"PAYPER",NULL,payper);
				ZERNRD(sortfile->fileno);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				mbl_trans=ADVbrowse(sortfile->fileno,1,
					brsrc,srsrc,drsrc,NULL,selectfunction,
					doexit_test,okfunc,NULL,sortfile,
					TRUE,NULL,0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
		rsrc=RDArsrcNEW("PURORD","ARCHIVE PURCHASE ORDERS OUTPUT DEVICE SCREEN");
		defaultprinter=DefaultPrinter();
		addbtnrsrc(rsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncOutputDevice);
		addstdrsrc(rsrc,"OUTPUT DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
		if(defaultprinter!=NULL) Rfree(defaultprinter);
		
		addrfcbrsrc(rsrc,"PRINT",TRUE,printpos,NULL);
		addrfcbrsrc(rsrc,"QUIT",TRUE,quitprint,NULL);
		addrfcbrsrc(rsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(rsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);

		APPmakescrn(rsrc,TRUE,quitprint,NULL,TRUE);
#endif
		printpos();
	} else {
		prterr("Warning:  No Purchase Orders were selected using the given criteria.");
		ERRORDIALOG("NO RECORDS SELECTED!","No records were selected for printing from the given criteria.  Please reselect and/or change criteria.",NULL,FALSE);
		if(sortfile!=NULL)
		{
			CLSDBsort(sortfile);
			FreeDBsort(sortfile);
			sortfile=NULL;
		}
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(sortname!=NULL) Rfree(sortname);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"ARCHIVE PURCHASE ORDERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	selected_poid=RDA_GetEnv("SELECTED_PO_IDENTIFICATION");

	temp1=RDA_GetEnv("SELECTED_FISCAL_YEAR");
	if(!isEMPTY(temp1))
	{
		selected_fyear=Ratoi(temp1);
	}

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew(module,"USE PURCHASE ORDER LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_po_lines=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			use_po_lines=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRINT ACCOUNTS ON ACTUAL PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			print_po_accounts=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			print_po_accounts=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"DISABLE ACCOUNT DESCRIPTIONS ON PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			disable_account_desc=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			disable_account_desc=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRINT COMMENTS ON ACTUAL PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			print_po_comments=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			print_po_comments=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRINT COMMENTS FROM PURLIN");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			print_purlin_comments=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			print_purlin_comments=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"REMIT ADDRESS POSITION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			address_position=*gsv->value.short_value;
		} else {
			address_position=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRTAPO PAGE FOOTER 1");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			page_footer1=stralloc(gsv->value.string_value);
		} else {
			page_footer1=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRTAPO PAGE FOOTER 2");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			page_footer2=stralloc(gsv->value.string_value);
		} else {
			page_footer2=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRTAPO PAGE FOOTER 3");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			page_footer3=stralloc(gsv->value.string_value);
		} else {
			page_footer3=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRTAPO PAGE FOOTER 4");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			page_footer4=stralloc(gsv->value.string_value);
		} else {
			page_footer4=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRINT SALES REP ON ACTUAL PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			print_sales_rep=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			print_sales_rep=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRTAPO USE PAGE FOOTER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			print_page_footers=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			print_page_footers=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"SWB_PO17_USE_ENCRYPTED_SIGNATURES_ON_PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			po_signature=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			po_signature=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRTAPO SHOW LONG ACCOUNT CODE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			long_acct_code=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			long_acct_code=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if((purtrn=APPOPNNRD(module,"PURTRN",TRUE,FALSE))==(-1)) return;
	if((puracc=APPOPNNRD(module,"PURACC",TRUE,FALSE))==(-1)) return;
	if((purlin=APPOPNNRD(module,"PURLIN",TRUE,FALSE))==(-1)) return;
	if((purmst=APPOPNNRD(module,"PURMST",TRUE,FALSE))==(-1)) return;
	if((pursit=APPOPNNRD(module,"PURSIT",TRUE,FALSE))==(-1)) return;
	if((venmsr=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((venadr=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((fineacc=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((finracc=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
	if((finbacc=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"ARCHIVE PURCHASE ORDERS");
	addDFincvir(mainrsrc,module,"PURMST",NULL,purmst);
	addDFincvir(mainrsrc,module,"PURLIN",NULL,purlin);
	addDFincvir(mainrsrc,module,"PURACC",NULL,puracc);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadr);
	/*
	if(purmst!=(-1)) file2rsrc(purmst,mainrsrc,FALSE);
	*/
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	if(purmst!=(-1)) file2rangersrc(purmst,mainrsrc);
	if(purlin!=(-1)) file2rangersrc(purlin,mainrsrc);
	if(puracc!=(-1)) file2rangersrc(puracc,mainrsrc);
	if(venadr!=(-1)) file2rangersrc(venadr,mainrsrc);
	/*
	addstdrsrc(mainrsrc,"MESSAGE 1",PLAINTEXT,72,NULL,TRUE);
	filerecord2rsrc(venadr,mainrsrc);
	*/
	SCRNvirtual2rangersrc(mainrsrc);
	if(isEMPTY(selected_poid))
	{
		DefaultScreens(mainrsrc);
	}
	/*
	rsrc2filerecord(paymst,mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	filerecord2rsrc(paymst,mainrsrc);
	*/
	SetRangersrcsensitive(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,selectpos,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	computeallSCRNvirtuals(mainrsrc);
	if(isEMPTY(selected_poid))
	{
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	ReadRDAScrolledLists(mainrsrc);
	RDAAPPMAINLOOP();
	} else {
	selectpos(mainrsrc);
	}
}
