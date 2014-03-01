/* Vendor Check */
#ifndef WIN32
#define __NAM__ "vencheckmicra.lnx"
#endif
#ifdef WIN32
#define __NAM__ "vencheckmicra.exe"
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

APPlib *BANKids=NULL;
char *module="VENPMT",*bank_id=NULL,*output_device=NULL;
static void InitializeArchivePage(RDA_PFILE **fp, int checkno);
static void CloseArchivePage(RDA_PFILE **fp);
char *comments1=NULL,*comments2=NULL;
static char PRINT_COLUMN_HEADERS=FALSE,select_reissued=FALSE;
short venyr=(-1),vennum=(-1),invnum=(-1),vchnum=(-1),venadr=(-1);
short eaccnum=(-1),raccnum=(-1),baccnum=(-1),banknum=(-1);
static void getBANKids(RDArsrc *),changeBANKids(RDArsrc *);
static RDAvirtual *VendorAddr1=NULL,*VendorAddr2=NULL,*VendorAddr3=NULL;
static short ADDRESS_OFFSET=0,ACCDESC_TYPE=0,CHECK_OFFSET=0;
static short SIGNATURE_TYPE=0,SHOW_INVOICE_DATE=0,VENCHECK_TYPE=0,BODY_COUNT=0;

static short mydebug=0;
static short startup_type=0,doublestub=0;
static short micr_xpos[65];
static char micr_xval[66];
static char *micrtext=NULL,*myptr=NULL;
static short micr_check_type=0,micr_middle_ypos=0,micr_bottom_ypos=0;
static short checkdate_xpos=0,checkdate_ypos=0;
static short checknumber_xpos=0,checknumber_ypos=0;
static short amount_xpos=0,amount_ypos=0;
static short spelled_amount_xpos=0,spelled_amount_ypos=0;
static short payto_xpos=0,micr_font_type=0,security_font_type=0;
static short payto1_ypos=0,payto2_ypos=0,payto3_ypos=0,payto4_ypos=0;
static short use_full_micr_check=0,checkform=0;
static short archive_full_micr_check=0;
static short printstubform=0;

static int trnstartpos=0;
static int trncounter=0;
static short repeatprtsubtotal=0;
static int last_trncounter=0;

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
int SetCoordinateValues(short vals[13])
{
	short myaddr=240;
	myaddr=(myaddr+(ADDRESS_OFFSET*30));

	checkdate_xpos=vals[0];
	checkdate_ypos=vals[1];
	checknumber_xpos=vals[2];
	checknumber_ypos=vals[3];
	amount_xpos=vals[4];
	amount_ypos=vals[5];
	spelled_amount_xpos=vals[6];
	spelled_amount_ypos=vals[7];
	//payto_xpos=vals[8];
	payto_xpos=myaddr;
	payto1_ypos=vals[9];
	payto2_ypos=vals[10];
	payto3_ypos=vals[11];
	payto4_ypos=vals[12];
	security_font_type=0;
}
int PreloadCoordinates()
{
/*
	1 checkdate_xpos
	2 checkdate_ypos
	3 checknumber_xpos
	4 checknumber_ypos
	5 amount_xpos
	6 amount_ypos
	7 spelled_amount_xpos
	8 spelled_amount_ypos
	9 payto_xpos
	10 payto1_ypos
	11 payto2_ypos
	12 payto3_ypos
	13 payto4_ypos

	short template [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
*/
	short high_spelled_amount [] = { 1320, 2590, 1738, 2590, 1950, 2590, 150, 2435, 475, 2735, 2785, 2835, 2885 };
	short low_spelled_amount [] = { 1320, 2640, 1745, 2640, 1950, 2640, 150, 2490, 475, 2735, 2785, 2835, 2885 };
	short high_address [] = { 1320, 2590, 1745, 2590, 1950, 2590, 150, 2435, 480, 2685, 2735, 2785, 2835 };
	short vencheckds [] = { 1320, 2585, 1745, 2585, 1950, 2585, 65, 2485, 245, 2735, 2785, 2835, 2885 };

	
	switch(VENCHECK_TYPE)
	{
		case 0: 
			/* VENCHECK High Spelled Amount  */
			SetCoordinateValues(high_spelled_amount);
			prterr("Using default high spelled amount\n");
			break;
		case 1:
			/* VENCHECKDS  */
			prterr("Using default vencheckds\n");
			SetCoordinateValues(vencheckds);
			break;
		case 3:
			/* VENCHECK Low Spelled Amount  */
			SetCoordinateValues(low_spelled_amount);
			prterr("Using default low spelled amount\n");
			break;
		case 5:
			/* VENCHECK High Address  */
			SetCoordinateValues(high_address);
			prterr("Using default high address\n");
			break;
		case 2:
			/* VENCHECK-CCH  */
		case 4:
			/* VENCHECK-CLARKE  */
		case 6:
			/* VENCHECK-TAB  */
		case 7:
			/* VENCHECK-ASCII  */
		case 8:
			/* GLENDALE-UNION  */
		default:
			SetCoordinateValues(high_spelled_amount);
			break;
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
	addDBfield(dbsrt,"CREDIT CARD NUMBER",1,20);
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
	COPYFIELD(invnum,dbsrt->fileno,"CREDIT CARD NUMBER");
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
const char *CenterText(char *input,int size)
{
	char *holdstring=NULL;
	int inputlength=0;

	if(input!=NULL)
	{
		inputlength=RDAstrlen(input);
		holdstring=Rmalloc(size+1);
		memset(holdstring,0,(size+1));

		if(inputlength==0)
		{
			memset(holdstring,' ',size);
		}else if(inputlength>size)
		{
			strncpy(holdstring,input,size);
		}else{
			if(((((size-inputlength)/2)*2)+inputlength)<size)
			{
				RDA_snprintf(holdstring,(size+1),"%*s%s%*s",((size-inputlength)/2),"",input,(((size-inputlength)/2)+1),"");
			}else{
				RDA_snprintf(holdstring,(size+1),"%*s%s%*s",((size-inputlength)/2),"",input,((size-inputlength)/2),"");
			}
			if((strlen(holdstring))!=size)
			{
				prterr("input size  [%d]  size [%d]  hold  [%d]  hstr [%s]  input [%s]",inputlength,size,strlen(holdstring),holdstring,input);
			}
		}

		return(holdstring);
	}else{
		return(NULL);
	}
}
int PrintMICRCheckForm(int print_type,RDA_PFILE *fp,int checknumber,char *checkdate,int amount, char *emp_name, char *emp_add1,char *emp_add2,char *emp_add3,char *output_device)
{
		int x=0,y=0;
		char *tempstr=NULL;
		char my_micr_xval[66];
		char payto1[120],payto2[120],payto3[120],payto4[120];
		char *spelled_amount=NULL;
		
		/* print_type == 0  No MICR, No Form
		 * print_type == 1  MICR and FORM
		 * print_type == 2  VOID No MICR, No Form
		 * print_type == 3  VOID MICR and FORM
		 */

		if((print_type==0)||(print_type==1))
		{
			spelled_amount=spell(amount);
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
		}else if((print_type==2)||(security_font_type==0)) 
			{
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
			spelled_amount=stralloc("*** V O I D **** V O I D **** V O I D ****");
		}else if(print_type==3) {
			spelled_amount=stralloc("***_VOID_****_VOID_****_VOID_****");
			switch(security_font_type)
			{
				case 1:
				case 2:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_73",fp);
					break;
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_77",fp);
					break;
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_83",fp);
					break;
				default:
					break;
			}
		}
		if((spelled_amount!=NULL)&&(print_type==3)&&((security_font_type==0)||(security_font_type==1)||(security_font_type==2)))
		{
			RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,spelled_amount_xpos,27,27,spelled_amount_ypos,27,spelled_amount);
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
		}else if((spelled_amount!=NULL)&&(print_type==3)) {
			RDA_fprintf(fp,"%c240%d%c%c241%d%c{%s}",27,spelled_amount_xpos,27,27,spelled_amount_ypos,27,spelled_amount);
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
		}else{
			RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,spelled_amount_xpos,27,27,spelled_amount_ypos,27,spelled_amount);

		}
		if(spelled_amount!=NULL) Rfree(spelled_amount);

		memset(payto1,0,120);
		memset(payto2,0,120);
		memset(payto3,0,120);
		memset(payto4,0,120);

		RDA_snprintf(payto1,120,"%s",emp_name);
		RDA_snprintf(payto2,120,"%s",emp_add1);
		RDA_snprintf(payto3,120,"%s",emp_add2);
		RDA_snprintf(payto4,120,"%s",emp_add3);

		if((print_type==1)||(print_type==3))
		{
			switch(security_font_type)
			{
				case 2:
				case 4:
				case 5:
				case 6:
				case 9:
				case 10:
				case 11:
					/*
					for(x=0;x<120;x++)
					{
						if(payto1[x]==' ')
						{
							payto1[x]='_';
						}
						if(payto2[x]==' ')
						{
							payto2[x]='_';
						}
						if(payto3[x]==' ')
						{
							payto3[x]='_';
						}
						if(payto4[x]==' ')
						{
							payto4[x]='_';
						}
					}
					*/
					break;
				case 0:
				default:
					break;
			}
		}

		if(((print_type==0)||(print_type==2))&&(!use_full_micr_check))
		{
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
			RDA_fprintf(fp,"%c240%d%c%c241%c%06d",27,checknumber_xpos,27,27,checknumber_ypos,27,checknumber);
		}
		if((print_type==1)||(print_type==3))
		{
			SelectPrintTypeByName(output_device,"USE FONT ARIAL 18",fp);
			RDA_fprintf(fp,"%c240%d%c%c241%d%c%06d",27,checknumber_xpos,27,27,checknumber_ypos,27,checknumber);
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
	
			if(diagrptgen) { prterr("Security Font Selection [%d].\n",security_font_type); }

			switch(security_font_type)
			{
				case 1:
				case 2:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_73",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",73); }
					break;
				case 3:
				case 4:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_77",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",77); }
					break;
				case 5:
				case 6:
				case 7:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_79",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",79); }
					break;
				case 8:
				case 9:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_83",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",83); }
					break;
				case 10:
				case 11:
				case 12:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_85",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",85); }
					break;
				default:
					break;
			}
		}

		if((print_type==2)||(print_type==3))
		{
			tempstr=famtpad(0,15,'*');
		}else{
			tempstr=famtpad(amount,15,'*');
		}
		switch(security_font_type)
		{
			case 0:
				RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,spelled_amount_xpos,27,27,spelled_amount_ypos,27,(spelled_amount!=NULL ? spelled_amount:""));
				RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,checkdate_xpos,27,27,checkdate_ypos,27,(checkdate!=NULL ? checkdate:""));
				RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,amount_xpos,27,27,amount_ypos,27,(tempstr!=NULL?tempstr:""));
				break;
			case 1:
			case 2:
				RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,checkdate_xpos,27,27,checkdate_ypos,27,(checkdate!=NULL ? checkdate:""));
				memset(stemp,0,101);
				RDA_snprintf(stemp,(RDAstrlen(tempstr)),"%s",tempstr);
				y=0;
				for(x=0;x<101;x++)
				{
					if((stemp[x]=='$')||(stemp[x]=='*'))
					{
						stemp[x]=' ';
						y++;
					}else{
						stemp[x-1]='$';
						break;
					}
				}

				/* prterr("y = [%d]  \n",x); */
				RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,amount_xpos,27,27,amount_ypos,27,stemp);
				break;
			default:
				RDA_fprintf(fp,"%c240%d%c%c241%d%c{%s}",27,checkdate_xpos,27,27,checkdate_ypos,27,(checkdate!=NULL ? checkdate:""));
				memset(stemp,0,101);
				RDA_snprintf(stemp,(RDAstrlen(tempstr)+1),"{%s",tempstr);
				for(x=0;x<101;x++)
				{
					if((stemp[x]=='$')||(stemp[x]=='*')||(stemp[x]=='{'))
					{
						stemp[x]=' ';
					}else{
						stemp[x-2]='{';
						stemp[x-1]='$';
						break;
					}
				}
				RDA_fprintf(fp,"%c240%d%c%c241%d%c%s}",27,amount_xpos,27,27,amount_ypos,27,stemp);
				break;
		}
		if(tempstr!=NULL) Rfree(tempstr);

		if((print_type==1)||(print_type==3))
		{
			switch(security_font_type)
			{
				case 2:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_73",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",73); }
					break;
				case 4:
				case 5:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_77",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",77); }
					break;
				case 6:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_79",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",79); }
					break;
				case 9:
				case 10:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_83",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",83); }
					break;
				case 11:
					SelectPrintTypeByName(output_device,"USE_SOFTFONT_85",fp);
					if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",85); }
					break;
				default:
					SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
					SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
					break;
			}
		}
		switch(security_font_type)
		{
			case 4:
			case 5:
			case 6:
			case 9:
			case 10:
			case 11:
				if((RDAstrlen(payto1))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c[%s]",27,payto_xpos,27,27,payto1_ypos,27,payto1);
				}
				SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
				SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
				if((RDAstrlen(payto2))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,payto_xpos,27,27,payto2_ypos,27,payto2);
				}
				if((RDAstrlen(payto3))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,payto_xpos,27,27,payto3_ypos,27,payto3);
				}
				if((RDAstrlen(payto4))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,payto_xpos,27,27,payto4_ypos,27,payto4);
				}
				break;
			default:
				if((RDAstrlen(payto1))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,payto_xpos,27,27,payto1_ypos,27,payto1);
				}
				SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
				SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
				if((RDAstrlen(payto2))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,payto_xpos,27,27,payto2_ypos,27,payto2);
				}
				if((RDAstrlen(payto3))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,payto_xpos,27,27,payto3_ypos,27,payto3);
				}
				if((RDAstrlen(payto4))!=0)
				{
					RDA_fprintf(fp,"%c240%d%c%c241%d%c%s",27,payto_xpos,27,27,payto4_ypos,27,payto4);
				}
				break;
		}

		if((print_type==1)||(print_type==3))
		{
			if(micr_font_type==0)
			{
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_60",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",60); }
			}else if(micr_font_type==1){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_61",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",61); }
			}else if(micr_font_type==2){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_62",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",62); }
			}else if(micr_font_type==3){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_63",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",63); }
			}else if(micr_font_type==4){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_64",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",64); }
			}else if(micr_font_type==5){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_65",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",65); }
			}else if(micr_font_type==6){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_66",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",66); }
			}else if(micr_font_type==7){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_67",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",67); }
			}else if(micr_font_type==8){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_68",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",68); }
			}else if(micr_font_type==9){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_69",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",69); }
			}else if(micr_font_type==10){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_70",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",70); }
			}else if(micr_font_type==11){
				SelectPrintTypeByName(output_device,"USE_SOFTFONT_71",fp);
				if(diagrptgen) { prterr("Sending Call For Soft Font [%d].\n",71); }
			}
			if(print_type==1)
			{
				memset(my_micr_xval,0,66);
				memset(stemp,0,101);
				sprintf(stemp,"%06d",checknumber);
				y=0;
				for(x=65;x>0;x--)
				{
					/*prterr(" ==> Array micr_xval[x] = %d *(micr_xval+x)= %d ",micr_xval[x],*(micr_xval+x));*/
					/*if((micr_xpos[x]!=0)&&((micr_xval+x)!=NULL))*/
					if((micr_xpos[x]!=0)&&(micr_xval[x]!=NULL))
					{			
						if(micr_xval[x]=='N')
						{
							my_micr_xval[x]=stemp[y];
							y++;
						}else{
							my_micr_xval[x]=micr_xval[x];
						}
						if(micr_check_type==0)
						{
							RDA_fprintf(fp,"%c240%d%c%c241%d%c%c",27,micr_xpos[x],27,27,micr_bottom_ypos,27,my_micr_xval[x]);
						}else if(micr_check_type==1) {
							RDA_fprintf(fp,"%c240%d%c%c241%d%c%c",27,micr_xpos[x],27,27,micr_middle_ypos,27,my_micr_xval[x]);
						}
					}
				}
			}
			memset(stemp,0,101);
			if((micr_check_type==0)&&(print_type==1))
			{
				sprintf(stemp,"USE_PCLMACRO_%04d",(checkform+3100));
				if(diagrptgen) { prterr("Sending Call For PCL Macro [%d].\n",(checkform+3100)); }
			}else if((micr_check_type==0)&&(print_type==3))
			{
				sprintf(stemp,"USE_PCLMACRO_%04d",(checkform+3300));
				if(diagrptgen) { prterr("Sending Call For PCL Macro [%d].\n",(checkform+3300)); }
			}else if((micr_check_type==1)&&(print_type==1))
			{
				sprintf(stemp,"USE_PCLMACRO_%04d",(checkform+3200));
				if(diagrptgen) { prterr("Sending Call For PCL Macro [%d].\n",(checkform+3200)); }
			}else if((micr_check_type==1)&&(print_type==3))
			{
				sprintf(stemp,"USE_PCLMACRO_%04d",(checkform+3400));
				if(diagrptgen) { prterr("Sending Call For PCL Macro [%d].\n",(checkform+3400)); }
			}
			SelectPrintTypeByName(output_device,stemp,fp);
		}
		RDA_fprintf(fp,"\r\n");	
		SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
		SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);
}
static void RepeatStub(short sfile,int checkno,int vcheckno,double total_amt,RDA_PFILE *fp,int startingrecordpos,int prtsubtotal)
{
	short ef=0,count=0;
	int checkno1=0,vcheckno1=0,x=0,y=0;
	char *poid=NULL,*invid=NULL,*acc=NULL,*desc=NULL;
	char *emp_name=NULL,*checkdate=NULL,*invoicedate=NULL,*creditcard=NULL;
	char *emp_id=NULL,*custno=NULL;
	char prtcheckdetail=FALSE;
	double amt=0.0;
	int rpttrncounter=0;

	ZERNRD(sfile);
	FINDFLDSETINT(sfile,"CHECK NUMBER",checkno);
	FINDFLDSETINT(sfile,"VIRTUAL CHECK NUMBER",vcheckno);
	FINDFLDSETINT(sfile,"COUNTER",startingrecordpos);
	ef=GTENRD(sfile,1);
	while(!ef)
	{
		FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno1);
		FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno1);
		FINDFLDGETINT(sfile,"COUNTER",&rpttrncounter);

		if(checkno!=checkno1 || vcheckno!=vcheckno1) break;
		if(archive_full_micr_check)
		{
			if(count==(BODY_COUNT))
			{
				break;
			}
		}

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
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&prtcheckdetail);
		if(RDAstrlen(checkdate)>10) checkdate[10]=0;
		if(RDAstrlen(invoicedate)>10) invoicedate[10]=0;
	
		if(count==0)
		{
			FINDFLDGETSTRING(sfile,"VENDOR NAME",&emp_name);
			if(RDAstrlen(emp_name)>40) emp_name[40]=0;
			FINDFLDGETSTRING(sfile,"VENDOR IDENTIFICATION",&emp_id);
			if(RDAstrlen(emp_id)>40) emp_id[40]=0;

			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			RDA_fprintf(fp,"\r\n");
			if(printstubform)
			{
				RDA_fprintf(fp,"                   %8d   %-10s   %-40s   %-30s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),CenterText((emp_name!=NULL ? emp_name:""),40),CenterText((emp_id!=NULL ? emp_id:""),30));
			}else{
				RDA_fprintf(fp,"Check: %8d Date: %-10s Vendor: %-40s  Vendor ID: %-40s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),(emp_name!=NULL ? emp_name:""),(emp_id!=NULL ? emp_id:""));
			}

			if(emp_name!=NULL) Rfree(emp_name);
			if(emp_id!=NULL) Rfree(emp_id);
			if(!prtcheckdetail)
			{
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"%-90sSEE ATTACHED LIST \r\n","");
                                RDA_fprintf(fp,"%-90sFOR CHECK DETAIL\r\n","");
				count+=4;
			}
			if((PRINT_COLUMN_HEADERS)&&(prtcheckdetail))
			{
				RDA_fprintf(fp,"%-12s  %-14s  %-16s  %-34s  %-33s  %-12s\r\n",CenterText("INVOICE DATE",12),CenterText("INVOICE ID",14),CenterText("PO ID / CC #",16),CenterText("ACCOUNT CODE",34),CenterText("DESCRIPTION",33),CenterText("AMOUNT",12));
			}else if(!PRINT_COLUMN_HEADERS) 
			{
				RDA_fprintf(fp,"\r\n");
			}
			RDA_fprintf(fp,"\r\n");
		}
		if(prtcheckdetail)
		{
			if(mydebug==2)
			{
				sprintf(invoicedate,"[%d]",count);
			}else if(mydebug==3)
			{
				sprintf(poid,"123456789012345");
				sprintf(invid,"123456789012345");
				sprintf(acc,"123456789012345678901234567890123456789012345");
				sprintf(desc,"1234567890123456789012345678901234567890123");
				amt=123456789012;
				sprintf(invoicedate,"[%d]",count);
			}
		
			if(RDAstrlen(acc)>35) acc[35]=0;
			if(RDAstrlen(desc)>33) desc[33]=0;
			if(RDAstrlen(invoicedate)>10) invoicedate[10]=0;

			memset(stemp,0,101);
			if(creditcard!=NULL)
			{
				y=0;
				for(x=0;x<20;x++)
				{
					switch(creditcard[x])
					{
						case '0' :
						case '1' :
						case '2' :
						case '3' :
						case '4' :
						case '5' :
						case '6' :
						case '7' :
						case '8' :
						case '9' :
							stemp[y]=creditcard[x];
							y++;
							break;
						default :
							break;
					}
				}
			}
			
			if(!prtcheckdetail)
                        {
                                RDA_fprintf(fp,"\n");
                        }else if((RDAstrlen(stemp))>0)
			{	
				RDA_fprintf(fp," %-10s  %-15s  %-16s  %-34s  %-33s  %10.02f\r\n",CenterText((invoicedate!=NULL ? invoicedate:""),10),CenterText((invid!=NULL ? invid:""),15),CenterText((creditcard!=NULL ? creditcard:stemp),16),CenterText((acc!=NULL ? acc:""),35),CenterText((desc!=NULL ? desc:""),33),amt/100);
			}else{
				RDA_fprintf(fp," %-10s  %-15s  %-16s  %-34s  %-33s  %10.02f\r\n",CenterText((invoicedate!=NULL ? invoicedate:""),10),CenterText((invid!=NULL ? invid:""),15),CenterText((poid!=NULL ? poid:""),15),CenterText((acc!=NULL ? acc:""),35),CenterText((desc!=NULL ? desc:""),33),amt/100);
			}

		}
		if(poid!=NULL) Rfree(poid);
		if(invid!=NULL) Rfree(invid);
		if(acc!=NULL) Rfree(acc);
		if(checkdate!=NULL) Rfree(checkdate);
		if(invoicedate!=NULL) Rfree(invoicedate);
		if(desc!=NULL) Rfree(desc);
		if(prtcheckdetail) ++count;
		ef=NXTNRD(sfile,1);
	}
	while(count<BODY_COUNT)
	{
		if(mydebug)
		{
			RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d] \r\n",__LINE__,BODY_COUNT,count);
		}else{
			RDA_fprintf(fp,"\r\n");
		}
		++count;
	}

	RDA_fprintf(fp,"\r\n");
	if((checkno1==vcheckno1)&&(printstubform==1))
	{
		if(prtsubtotal)
		{
			RDA_fprintf(fp,"                %-40s                                        Sub Total                 %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		}else{
			RDA_fprintf(fp,"                %-40s                                       Check Total                %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		}
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}else if((checkno==vcheckno)&&(printstubform==0))
	{
		RDA_fprintf(fp,"                      Customer No.: %-40s  Check Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}else if(printstubform==1)
	{
		RDA_fprintf(fp,"                %-40s                                        Sub Total                 %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	} else {
		RDA_fprintf(fp,"                      Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}

	if(custno!=NULL) Rfree(custno);
	/*
	while(count<BODY_COUNT)
	{
		RDA_fprintf(fp,"\r\n");	
		//RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d]\r\n",__LINE__,BODY_COUNT,count); 
		++count;
	}
	*/
}
/*static void InitializeArchivePage(RDA_PFILE **fp, int checkno,long pagecount)*/
static void InitializeArchivePage(RDA_PFILE **fp, int checkno)
{
	char *temp,*temp1,*temp5=NULL, *temp6=NULL;
	int ret_int=0;


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

	sprintf(stemp,"BNKREC/%s/CHECKS/%d.pdf",bank_id,checkno);
	temp1=adddashes(stemp);
#ifndef WIN32
	sprintf(temp,"viewpdf.lnx --dont-view --dont-delete -o \"%s/%s\" >/dev/null 2>&1",CURRENTDIRECTORY,temp1);
	/*sprintf(temp,"dd of=\"./%s\".txt2ps",temp1);*/
#endif 
#ifdef WIN32
	sprintf(temp,"viewpdf.exe --dont-view --dont-delete -o \"%s/%s\"",CURRENTDIRECTORY,temp1);
#endif 

	*fp=RDA_popen(temp,"w");
	if(*fp==NULL)
	{
		prterr("Error:  Couldn't open output device [%s].\n",temp);
	}
	SelectPrintTypeByName(output_device,"PCL JOB HEADER",*fp);
/*
	*fp=RDA_popen(output_device,"w");
	if(fp==NULL)
	{
		prterr("Error:  Couldn't open output device [%s].\n",output_device);
	}
	SelectPrintTypeByName(output_device,"PCL JOB HEADER",*fp);
*/
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);

	memset(stemp,0,101);

	if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
	{
		SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3002",*fp);
		SelectPrintTypeByName(output_device,"LOAD_PCLMACRO_3002",*fp);
	}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
	{
		SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3003",*fp);
		SelectPrintTypeByName(output_device,"LOAD_PCLMACRO_3003",*fp);
	}else if((printstubform==1)&&(BODY_COUNT==46))
	{
		SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3000",*fp);
		/*prterr(" ==> Loading PCL Macro 3000.\n");*/
		SelectPrintTypeByName(output_device,"LOAD_PCLMACRO_3000",*fp);
	}

	if(micr_check_type==0)
	{
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3100));
		SelectPrintTypeByName(output_device,stemp,*fp);
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3300));
		SelectPrintTypeByName(output_device,stemp,*fp);

		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3100));
		SelectPrintTypeByName(output_device,stemp,*fp);
		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3300));
		SelectPrintTypeByName(output_device,stemp,*fp);
	}else if(micr_check_type==1) {
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3200));
		SelectPrintTypeByName(output_device,stemp,*fp);
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3400));
		SelectPrintTypeByName(output_device,stemp,*fp);

		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3200));
		SelectPrintTypeByName(output_device,stemp,*fp);
		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3400));
		SelectPrintTypeByName(output_device,stemp,*fp);
	}

	SelectPrintTypeByName(output_device,"SIMPLEX",*fp);
	SelectPrintTypeByName(output_device,"COMPRESSED TEXT",*fp);
	/*RDA_fprintf(*fp,"\f");*/
}
static void CloseArchivePage(RDA_PFILE **fp)
{
			/*prterr("Entering CloseArchivePage.\n");*/
		if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
		{
			SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3002",*fp);
		}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
		{
			SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3003",*fp);
		}else if((printstubform==1)&&(BODY_COUNT==46))
		{
			/*prterr(" ==> Deleting PCL Macro 3000.\n");*/
			SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3000",*fp);
		}

	if(use_full_micr_check)
	{
		memset(stemp,0,101);
		if(micr_check_type==0)
		{
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3100));
			SelectPrintTypeByName(output_device,stemp,*fp);
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3300));
			SelectPrintTypeByName(output_device,stemp,*fp);
		}else if(micr_check_type==1) {
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3200));
			SelectPrintTypeByName(output_device,stemp,*fp);
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3400));
			SelectPrintTypeByName(output_device,stemp,*fp);
		}
		if(diagrptgen) { prterr("Check Form:  [%d]\n",checkform);}
	}
	/*
	if(SIGNATURE_TYPE==4)
	{
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_200",*fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_201",*fp);
	}
	*/
	SelectPrintTypeByName(output_device,"COMPRESSED TEXT",*fp);
	SelectPrintTypeByName(output_device,"PCL JOB FOOTER",*fp);
	RDA_pclose(*fp);
			/*prterr("Entering CloseArchivePage.\n");*/
}
static void OutputChecks(short sfile)
{
	short ef=0,count=0;
	int checkno=0,vcheckno=0,last_checkno=0,last_vcheckno=0;
	int x=0,y=0;
	RDA_PFILE *fp=NULL;
	double total_amt=0.0,amt=0.0;
	char *emp_name=NULL,*emp_add1=NULL,*emp_add2=NULL,*emp_add3=NULL;
	char *emp_id=NULL,*custno=NULL;
	char *checkdate=NULL,*poid=NULL,*invid=NULL,*creditcard=NULL;
	char *acc=NULL,*desc=NULL,*invoicedate=NULL;
	char prtcheckdetail=FALSE;
	static short stubcounter=0;
	static short lastpage=0;
	long pagecount=0;

	if(isEMPTY(output_device))
	{
		prterr("Error:  An appropriate output device must be selected to print checks.");
		return;
	}

	ZERNRD(sfile);
	ef=FRSNRD(sfile,1);
	FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno);
	FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno);
	FINDFLDGETINT(sfile,"COUNTER",&trncounter);
	last_checkno=checkno;
	last_vcheckno=vcheckno;
	last_trncounter=trncounter;
	InitializeArchivePage(&fp,checkno);
	while(!ef)
	{
		//prterr(" ==> lastpage = %d use_full_micr_check =%d checkno =%d last_checkno =%d", lastpage,use_full_micr_check,checkno,last_checkno);
		FINDFLDGETINT(sfile,"CHECK NUMBER",&checkno);
		FINDFLDGETINT(sfile,"VIRTUAL CHECK NUMBER",&vcheckno);
		last_trncounter=trncounter;
		FINDFLDGETINT(sfile,"COUNTER",&trncounter);
		if((count==1)||(stubcounter==1))
		{
			trnstartpos=last_trncounter;
		}
		if(checkno!=last_checkno || vcheckno!=last_vcheckno)
		{
			if(lastpage==0)
			{
				if((count!=0)&&(count<BODY_COUNT))
				{
					while(count<BODY_COUNT)
					{
						if(mydebug)
						{
							RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d]  Stub [%d]  LP [%d]\r\n",__LINE__,BODY_COUNT,count,stubcounter,lastpage); 
						}else{
							RDA_fprintf(fp,"\r\n");	
						}
						++count;
					}
				}else if((stubcounter!=0)&&(stubcounter<BODY_COUNT)) 
				{
					while(stubcounter<BODY_COUNT)
					{
						if(mydebug)
						{
							RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d]  Stub [%d]  LP [%d]\r\n",__LINE__,BODY_COUNT,count,stubcounter,lastpage); 
						}else{
							RDA_fprintf(fp,"\r\n");	
						}
						++stubcounter;
					}
				}
			}else{
				prterr("lastpage == 1\n");
			}

			if((last_checkno==last_vcheckno)&&(printstubform==1))
			{
				repeatprtsubtotal=FALSE;
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"                %-40s                                       Check Total                %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
			/*prterr("==> pagecount =%d   Customer No.: %-20s  Check Total: %10.02f\r\n",pagecount,(custno!=NULL ? custno:""),total_amt/100);*/
			pagecount=0;
			}else if((last_checkno==last_vcheckno)&&(printstubform==0))
			{
				repeatprtsubtotal=FALSE;
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"                      Customer No.: %-40s  Check Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
			/*prterr("==> pagecount =%d   Customer No.: %-20s  Check Total: %10.02f\r\n",pagecount,(custno!=NULL ? custno:""),total_amt/100);*/
			}else if(printstubform==1)
			{
				repeatprtsubtotal=TRUE;
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"                %-40s                                        Sub Total                 %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
			/*prterr("==> pagecount =%d   Customer No.: %-20s  Check Total: %10.02f\r\n",pagecount,(custno!=NULL ? custno:""),total_amt/100);*/
			} else {
				repeatprtsubtotal=TRUE;
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"                      Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
			/*prterr("==> pagecount =%d   Customer No.: %-20s  Check Total: %10.02f\r\n",pagecount,(custno!=NULL ? custno:""),total_amt/100);*/
			}

			/*prterr("==> pagecount =%d   Customer No.: %-20s  Check Total: %10.02f\r\n",pagecount,(custno!=NULL ? custno:""),total_amt/100);*/
			if(custno!=NULL) Rfree(custno);
/* repeat stub and print negotiable */
			if((doublestub==1)&&(micr_check_type!=1))
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RepeatStub(sfile,last_checkno,last_vcheckno,total_amt,fp,trnstartpos,repeatprtsubtotal);
			}
/* negotiable check */
			/*prterr("use_full_micr_check [%d]   printstubform [%d]  vcheckno [%d]   last_vcheckno [%d]  \n",use_full_micr_check,printstubform,vcheckno,last_vcheckno);*/
			//if(use_full_micr_check==0)
			if(use_full_micr_check==0 && printstubform==0)
			{
				SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
				if(checkno!=last_checkno)
				{
					PrintMICRCheckForm(0,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					/*
					if(SIGNATURE_TYPE==4)
					{
						SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
						SelectPrintTypeByName(output_device,"VENPMT SIGNATURE",fp);
						SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					}
					*/
				} else {
					PrintMICRCheckForm(2,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);
				}
			}else{
				SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
				if(checkno!=last_checkno)
				{
					PrintMICRCheckForm(1,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device);
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					/*
					if(SIGNATURE_TYPE==4)
					{
						SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
						SelectPrintTypeByName(output_device,"VENPMT SIGNATURE",fp);
						SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					}
					*/
				} else {
					PrintMICRCheckForm(3,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);
				}
			}
			SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);

			if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
			{
				SelectPrintTypeByName(output_device,"USE_PCLMACRO_3002",fp);
			}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
			{
				SelectPrintTypeByName(output_device,"USE_PCLMACRO_3003",fp);
			}else if((printstubform==1)&&(BODY_COUNT==46))
			{
				SelectPrintTypeByName(output_device,"USE_PCLMACRO_3000",fp);
			}
			if(checkno!=last_checkno)
			{
				RDA_pclose(fp);
				InitializeArchivePage(&fp,checkno);
				/*
			}else if(vcheckno!=last_vcheckno)
			{
				RDA_pclose(fp);
				InitializeArchivePage(&fp,checkno);
				*/
			}else{
				RDA_fprintf(fp,"\f");
			}
			count=0;	
			stubcounter=0;
			lastpage=0;
			if(checkno!=last_checkno)
			{
				total_amt=0;
			}
			last_checkno=checkno;
			last_vcheckno=vcheckno;
		}else{
			if((prtcheckdetail)&&((doublestub==0)||((doublestub==1)&&(archive_full_micr_check==1))))
			{
				if(lastpage==1)
				{
					if((count!=0)&&(count<BODY_COUNT))
					{
						while(count<BODY_COUNT)
						{
							RDA_fprintf(fp,"\r\n");	
							RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d]  Stub [%d]  LP [%d]\r\n",__LINE__,BODY_COUNT,count,stubcounter,lastpage); 
							++count;
						}
					}else if((stubcounter!=0)&&(stubcounter<BODY_COUNT)) 
					{
						while(stubcounter<BODY_COUNT)
						{
							if(mydebug)
							{
								RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d]  Stub [%d]  LP [%d]\r\n",__LINE__,BODY_COUNT,count,stubcounter,lastpage); 
							}else{
								RDA_fprintf(fp,"\r\n");	
							}
							++stubcounter;
						}
					}
				}

				if((lastpage==1)&&(archive_full_micr_check==1))
				{
					//RDA_fprintf(fp,"\r\n");	
					repeatprtsubtotal=TRUE;
					if(printstubform==1)
					{
						RDA_fprintf(fp,"\r\n");	
						RDA_fprintf(fp,"                %-40s                                        Sub Total                 %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
						RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
						RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
					} else {
						RDA_fprintf(fp,"\r\n");	
						RDA_fprintf(fp,"                      Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
						RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
						RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
					}
					if((doublestub==1)&&(micr_check_type!=1))
					{
						RDA_fprintf(fp,"\r\n");	
						RDA_fprintf(fp,"\r\n");	
						RDA_fprintf(fp,"\r\n");	
						RDA_fprintf(fp,"\r\n");	
						RepeatStub(sfile,last_checkno,last_vcheckno,total_amt,fp,trnstartpos,repeatprtsubtotal);
					}
					//prterr("MICR BODY_COUNT [%d]  count [%d]  subcounter [%d]\n",BODY_COUNT,count,stubcounter);
					SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
					PrintMICRCheckForm(3,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);

					if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
					{
						SelectPrintTypeByName(output_device,"USE_PCLMACRO_3002",fp);
					}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
					{
						SelectPrintTypeByName(output_device,"USE_PCLMACRO_3003",fp);
					}else if((printstubform==1)&&(BODY_COUNT==46))
					{
						SelectPrintTypeByName(output_device,"USE_PCLMACRO_3000",fp);
					}
					/*RDA_pclose(fp);*/
					//fp=NULL;
					RDA_fprintf(fp,"\f");
					stubcounter=0;
					lastpage=0;
				}else if((lastpage==1)&&(use_full_micr_check==0)) 
					{
					//RDA_fprintf(fp,"\r\n");	
					repeatprtsubtotal=TRUE;
					if(printstubform==1)
					{
						RDA_fprintf(fp,"\r\n");	
		/*if(checkno!=last_checkno || vcheckno!=last_vcheckno)*/
						RDA_fprintf(fp,"                %-40s                                        Sub Total                 %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
						RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
						RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
					} else {
						RDA_fprintf(fp,"\r\n");	
						RDA_fprintf(fp,"                      Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
						RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
						RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
					}
					//prterr(" ==> NO FORM BODY_COUNT [%d]  count [%d]  subcounter [%d]\n",BODY_COUNT,count,stubcounter);
					//prterr(" ==> Lastpage = %d use_full_micr_check = %d checkno = %d last_checkno =%d",lastpage,use_full_micr_check,checkno,last_checkno);
					SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
					//PrintMICRCheckForm(2,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					PrintMICRCheckForm(3,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
					{
						SelectPrintTypeByName(output_device,"USE_PCLMACRO_3002",fp);
					}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
					{
						SelectPrintTypeByName(output_device,"USE_PCLMACRO_3003",fp);
					}else if((printstubform==1)&&(BODY_COUNT==46))
					{
						SelectPrintTypeByName(output_device,"USE_PCLMACRO_3000",fp);
					}
					//RDA_pclose(fp);
					//fp=NULL;
					RDA_fprintf(fp,"\f");
					stubcounter=0;
					lastpage=0;
				}
			}
	
		}
/* print 1st detail stub */
		FINDFLDGETDOUBLE(sfile,"AMOUNT",&amt);
		FINDFLDGETSTRING(sfile,"PO IDENTIFICATION",&poid);
		if(RDAstrlen(poid)>15) poid[15]=0;
		FINDFLDGETSTRING(sfile,"INVOICE IDENTIFICATION",&invid);
		if(RDAstrlen(invid)>15) invid[15]=0;
		FINDFLDGETSTRING(sfile,"ACCOUNT CODE",&acc);
		FINDFLDGETSTRING(sfile,"DESCRIPTION",&desc);

		FINDFLDGETSTRING(sfile,"CREDIT CARD NUMBER",&creditcard);

		FINDFLDGETSTRING(sfile,"CHECK DATE",&checkdate);
		if(RDAstrlen(checkdate)>10) checkdate[10]=0;
		FINDFLDGETSTRING(sfile,"INVOICE DATE",&invoicedate);
		if(RDAstrlen(invoicedate)>10) invoicedate[10]=0;
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&prtcheckdetail);
		FINDFLDGETSTRING(sfile,"CUSTOMER NUMBER",&custno);
		FINDFLDGETSTRING(sfile,"VENDOR NAME",&emp_name);
		if(RDAstrlen(emp_name)>40) emp_name[40]=0;
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 1",&emp_add1);
		if(RDAstrlen(emp_add1)>40) emp_add1[40]=0;
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 2",&emp_add2);
		if(RDAstrlen(emp_add2)>40) emp_add2[40]=0;
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 3",&emp_add3);
		if(RDAstrlen(emp_add3)>40) emp_add3[40]=0;
			
		if(checkno!=last_checkno)
		{
			RDA_pclose(fp);
			InitializeArchivePage(&fp,checkno);
		}else if(vcheckno!=last_vcheckno)
		{
			RDA_pclose(fp);
			InitializeArchivePage(&fp,checkno);
		}else if(fp==NULL)
		{
			/*InitializeArchivePage(&fp,checkno,++pagecount);*/
			InitializeArchivePage(&fp,checkno);
		}
		if((count==0)||(stubcounter==0))
		{
			FINDFLDGETSTRING(sfile,"VENDOR IDENTIFICATION",&emp_id);

			if(!prtcheckdetail)
			{
				if(printstubform)
                                {
                                        RDA_fprintf(fp,"                   %8d   %-10s   %-40s   %-30s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),CenterText((emp_name!=NULL ? emp_name:""),40),CenterText((emp_id!=NULL ? emp_id:""),30));
                                }else{
                                       if(RDAstrlen(emp_name)>38) emp_name[38]=0;
                                       RDA_fprintf(fp,"Check: %8d Date: %-10s Vendor: %-38s  Vendor ID: %-30s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),(emp_name!=NULL ? emp_name:""),(emp_id!=NULL ? emp_id:""));
                                }
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"%-90sSEE ATTACHED LIST \r\n","");
				RDA_fprintf(fp,"%-90sFOR CHECK DETAIL\r\n","");
				count+=3;

			}else{
				if(printstubform)
				{
					/* prterr("checkno [%d]   last_checkno [%d]  vcheckno [%d]   last_vcheckno [%d]  \n",checkno,last_checkno,vcheckno,last_vcheckno); */
					RDA_fprintf(fp,"                   %8d   %-10s   %-40s   %-30s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),CenterText((emp_name!=NULL ? emp_name:""),40),CenterText((emp_id!=NULL ? emp_id:""),30));
				}else{
					if(RDAstrlen(emp_name)>38) emp_name[38]=0;
					RDA_fprintf(fp,"Check: %8d Date: %-10s Vendor: %-38s  Vendor ID: %-30s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),(emp_name!=NULL ? emp_name:""),(emp_id!=NULL ? emp_id:""));
				}
				if(PRINT_COLUMN_HEADERS)
				{
					/*RDA_fprintf(fp,"%-12s   %-15s  %-15s  %-35s  %-33s %10s\r\n","INVOICE DATE","PO ID","INVOICE","ACCOUNT CODE","DESCRIPTION","AMOUNT");*/
					RDA_fprintf(fp,"%-12s  %-14s  %-16s  %-34s  %-33s  %-12s\r\n",CenterText("INVOICE DATE",12),CenterText("INVOICE ID",14),CenterText("PO ID / CC #",16),CenterText("ACCOUNT CODE",34),CenterText("DESCRIPTION",33),CenterText("AMOUNT",12));
				}else if(!PRINT_COLUMN_HEADERS) 
				{
					RDA_fprintf(fp,"\r\n");
				}
			}
			RDA_fprintf(fp,"\r\n");
		}
		if(prtcheckdetail)
		{
			/* RDA_fprintf(fp,"Count [%d]  Stub [%d]   ",count,stubcounter); */
			if(mydebug)
			{
				sprintf(poid,"123456789012345");
				sprintf(invid,"123456789012345");
				sprintf(acc,"123456789012345678901234567890123456789012345");
				sprintf(desc,"1234567890123456789012345678901234567890123");
				amt=123456789012;
				sprintf(invoicedate,"[%d]",count);
				sprintf(invid,"[%d]",stubcounter);
			}
		
			if(RDAstrlen(acc)>35) acc[35]=0;
			if(RDAstrlen(desc)>33) desc[33]=0;
			if(RDAstrlen(invoicedate)>10) invoicedate[10]=0;

			memset(stemp,0,101);
			if(creditcard!=NULL)
			{
				y=0;
				for(x=0;x<20;x++)
				{
					switch(creditcard[x])
					{
						case '0' :
						case '1' :
						case '2' :
						case '3' :
						case '4' :
						case '5' :
						case '6' :
						case '7' :
						case '8' :
						case '9' :
							stemp[y]=creditcard[x];
							y++;
							break;
						default :
							break;
					}
				}
			}
			
			if((RDAstrlen(stemp))>0)
			{	
				RDA_fprintf(fp," %-10s  %-15s  %-16s  %-34s  %-33s  %10.02f\r\n",CenterText((invoicedate!=NULL ? invoicedate:""),10),CenterText((invid!=NULL ? invid:""),15),CenterText((creditcard!=NULL ? creditcard:stemp),16),CenterText((acc!=NULL ? acc:""),35),CenterText((desc!=NULL ? desc:""),33),amt/100);
			}else{
				RDA_fprintf(fp," %-10s  %-15s  %-16s  %-34s  %-33s  %10.02f\r\n",CenterText((invoicedate!=NULL ? invoicedate:""),10),CenterText((invid!=NULL ? invid:""),15),CenterText((poid!=NULL ? poid:""),15),CenterText((acc!=NULL ? acc:""),35),CenterText((desc!=NULL ? desc:""),33),amt/100);
			}

		}
		if(poid!=NULL) Rfree(poid);
		if(invid!=NULL) Rfree(invid);
		if(acc!=NULL) Rfree(acc);
		if(desc!=NULL) Rfree(desc);
		if(prtcheckdetail) ++count;
		total_amt+=amt;
		++stubcounter;

		if((prtcheckdetail)&&((doublestub==0)||((doublestub==1)&&(archive_full_micr_check==1))))
		{
			if((stubcounter==BODY_COUNT)&&(archive_full_micr_check==1))
			{
				stubcounter=0;
				lastpage=1;
			}else if((stubcounter==BODY_COUNT)&&(use_full_micr_check==0)) {
				stubcounter=0;
				lastpage=1;
			}
		}

		ef=NXTNRD(sfile,1);
	}

	if((count!=0)&&(count<BODY_COUNT))
	{
		while(count<BODY_COUNT)
		{
			if(mydebug)
			{
				RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d]  Stub [%d]  LP [%d]\r\n",__LINE__,BODY_COUNT,count,stubcounter,lastpage); 
			}else{
				RDA_fprintf(fp,"\r\n");	
			}
			++count;
		}
	}else if((stubcounter!=0)&&(stubcounter<BODY_COUNT)) 
	{
		while(stubcounter<BODY_COUNT)
		{
			if(mydebug)
			{
				RDA_fprintf(fp,"Added blank line  LINE: [%d]  BC [%d]  Count [%d]  Stub [%d]  LP [%d]\r\n",__LINE__,BODY_COUNT,count,stubcounter,lastpage); 
			}else{
				RDA_fprintf(fp,"\r\n");	
			}
			++stubcounter;
		}
	}
	if((last_checkno==last_vcheckno)&&(printstubform==1))
	{
		repeatprtsubtotal=FALSE;
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"                %-40s                                       Check Total                %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}else if((last_checkno==last_vcheckno)&&(printstubform==0))
	{
		repeatprtsubtotal=FALSE;
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"                      Customer No.: %-40s  Check Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}else if(printstubform==1)
	{
		repeatprtsubtotal=TRUE;
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"                %-40s                                        Sub Total                 %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	} else {
		repeatprtsubtotal=TRUE;
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"                      Customer No.: %-40s    Sub Total: %10.02f\r\n",(custno!=NULL ? custno:""),total_amt/100);
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s        Date:    %s          Time:  %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}

	if(custno!=NULL) Rfree(custno);

/* repeat stub and print negotiable */
	if((doublestub==1)&&(micr_check_type!=1))
	{
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RepeatStub(sfile,last_checkno,last_vcheckno,total_amt,fp,trnstartpos,repeatprtsubtotal);
	}
/* negotiable check */

	/*prterr("checkno [%d]   last_checkno [%d]  vcheckno [%d]   last_vcheckno [%d]  \n",checkno,last_checkno,vcheckno,last_vcheckno);*/
	if(use_full_micr_check==0 && printstubform==0)
	{
		PrintMICRCheckForm(0,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
		stubcounter=0;
		lastpage=0;
	}else{
		PrintMICRCheckForm(1,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device);
		stubcounter=0;
		lastpage=0;
	}
	count=0;	

	if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
	{
		SelectPrintTypeByName(output_device,"USE_PCLMACRO_3002",fp);
	}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
	{
		SelectPrintTypeByName(output_device,"USE_PCLMACRO_3003",fp);
	}else if((printstubform==1)&&(BODY_COUNT==46))
	{
		/*prterr(" ==> Using PCL Macro 3000.\n");*/
		SelectPrintTypeByName(output_device,"USE_PCLMACRO_3000",fp);
	}
	/*
	if(SIGNATURE_TYPE==4)
	{
		SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
		SelectPrintTypeByName(output_device,"VENPMT SIGNATURE",fp);
		SelectPrintTypeByName(output_device,"POP CURSOR",fp);
	}
	*/
	//SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);
	//RDA_fprintf(fp,"\f");

	if(checkno!=last_checkno)
	{
		total_amt=0;
	}
	last_checkno=checkno;
	last_vcheckno=vcheckno;
	CloseArchivePage(&fp);
	if(checkdate!=NULL) Rfree(checkdate);
	if(invoicedate!=NULL) Rfree(invoicedate);
}
void setcheck(RDArsrc *mainrsrc)
{
	short accttype=0,ef=0,increment_diagnostic=FALSE;
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*vendorid=NULL,*last_vendorid=NULL;
	char *bankid1=NULL;
	RDArsrc *tmprsrc=NULL;
	int selbankid=0,x=0,cnum=0;
	RDAvirtual *v;
	char manual_voucher=FALSE,deleteflag=FALSE,cleared=FALSE;
	char verified=FALSE,electronic=FALSE,re=FALSE,void_check=FALSE;
	char *vouch_desc=NULL;
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

	/* Set MICR Values */
	FINDRSCGETCHAR(mainrsrc,"SELECT REISSUED",&select_reissued);
	FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",bank_id);
	if(EQLNRD(banknum,1)) KEYNRD(banknum,1);

	FINDFLDGETSTRING(banknum,"CHECK MICR",&micrtext);

	myptr=micrtext;
	memset(micr_xval,0,66);
	for(x=65;x>0;x--)
	{
		micr_xval[x]=*myptr;
		++myptr;
	}
	
	FINDFLDGETSHORT(banknum,"CHECK TYPE",&micr_check_type);
	FINDFLDGETSHORT(banknum,"MICR MIDDLE YPOS",&micr_middle_ypos);
	FINDFLDGETSHORT(banknum,"MICR BOTTOM YPOS",&micr_bottom_ypos);

	FINDFLDGETSHORT(banknum,"CHECK FORM",&checkform);
	FINDFLDGETSHORT(banknum,"CHECK DATE XPOS",&checkdate_xpos);
	FINDFLDGETSHORT(banknum,"CHECK DATE YPOS",&checkdate_ypos);
	FINDFLDGETSHORT(banknum,"CHECK NUMBER XPOS",&checknumber_xpos);
	FINDFLDGETSHORT(banknum,"CHECK NUMBER YPOS",&checknumber_ypos);
	FINDFLDGETSHORT(banknum,"AMOUNT XPOS",&amount_xpos);
	FINDFLDGETSHORT(banknum,"AMOUNT YPOS",&amount_ypos);
	FINDFLDGETSHORT(banknum,"SPELLED AMOUNT XPOS",&spelled_amount_xpos);
	FINDFLDGETSHORT(banknum,"SPELLED AMOUNT YPOS",&spelled_amount_ypos);
	FINDFLDGETSHORT(banknum,"PAY TO XPOS",&payto_xpos);
	FINDFLDGETSHORT(banknum,"MICR FONT TYPE",&micr_font_type);
	FINDFLDGETSHORT(banknum,"SECURITY FONT TYPE",&security_font_type);
	FINDFLDGETSHORT(banknum,"PAY TO 1 YPOS",&payto1_ypos);
	FINDFLDGETSHORT(banknum,"PAY TO 2 YPOS",&payto2_ypos);
	FINDFLDGETSHORT(banknum,"PAY TO 3 YPOS",&payto3_ypos);
	FINDFLDGETSHORT(banknum,"PAY TO 4 YPOS",&payto4_ypos);
	myptr=NULL;

	if((((micr_check_type==0)&&(micr_bottom_ypos!=0)||((micr_check_type==1)&&(micr_middle_ypos!=0))))&&(((strlen(micrtext)!=0)))&&((startup_type==3)||(startup_type==6)))
	{
		use_full_micr_check=1;	
	}

	if(micrtext!=NULL) Rfree(micrtext);

	for(x=0;x<66;x++)
	{
		micr_xpos[x]=0;
		if(use_full_micr_check)
		{
			memset(stemp,0,101);
			sprintf(stemp,"MICR CHAR %02d XPOS",x);
			FINDFLDGETSHORT(banknum,stemp,&micr_xpos[x]);
		}
	}

	if((amount_xpos==0)&&(amount_ypos==0))
	{
		PreloadCoordinates();
	}
	if((startup_type!=3)&&(startup_type!=6))
	{
		security_font_type=0;
	}
	/* End Setting MICR Values */
	/* prterr("VENCHECK_TYPE=[%d]  micr_check_type=[%d]    use_full_micr_check=[%d]\n",VENCHECK_TYPE,micr_check_type,use_full_micr_check); */

	if(doublestub==1)
	{
		BODY_COUNT=15;
	}else if((doublestub==0)&&(micr_check_type==1))
	{
		/* Springfield Middle Check */
		BODY_COUNT=15;
	}else{		
		BODY_COUNT=46;
	}

	/* prterr("BODY_COUNT [%d]\n",BODY_COUNT); */
	/*FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&output_device);*/
	if(output_device!=NULL) Rfree(output_device);
#ifndef WIN32
	output_device=stralloc("viewpdf.lnx");
#endif 
#ifdef WIN32
	output_device=stralloc("viewpdf.exe");
#endif 

	FINDRSCGETSTRING(mainrsrc,"COMMENTS 1",&comments1);
	FINDRSCGETSTRING(mainrsrc,"COMMENTS 2",&comments2);
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
		FINDFLDGETCHAR(vchnum,"VOID CHECK",&void_check);
		FINDFLDGETSTRING(vchnum,"DESCRIPTION",&vouch_desc);
		FINDFLDGETSTRING(vchnum,"VENDOR IDENTIFICATION",&vendorid);
		FINDFLDGETINT(vchnum,"CHECK NUMBER",&cnum);
		/* SELECT CRITERIA */
		/*if(verified && !cleared && !manual_voucher && !electronic && !deleteflag && cnum!=0 && ((re==FALSE || select_reissued==TRUE)))*/
		/*if(verified && !electronic && !deleteflag && cnum!=0)*/
		if(verified && !electronic && !deleteflag && cnum!=0 && (!void_check || ((RDAstrcmp(vouch_desc,"RUINED CHECKS") || RDAstrcmp(vendorid,"RUINED CHECKS")) && (strncmp(vouch_desc,"VOID VOUCHER TRANSACTION",24)))))
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
		ERRORDIALOG("Error Printing Checks","No Vendor Vouchers Selected for Check Archiving.\n",NULL,TRUE);
		if(last_vendorid!=NULL) Rfree(last_vendorid);
		if(vendorid!=NULL) Rfree(vendorid);
		if(bankid1!=NULL) Rfree(bankid1);
		if(sortfile!=NULL) 
		{	
			SetPrintedFlag(sortfile->fileno);
			CLSDBsort(sortfile);
			FreeDBsort(sortfile);
		}	
		RDAAPPMAINLOOP();
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
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int which_type=0;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	RDAwdgt *w=NULL;

	if(argc<=1)
	{
		startup_type=1;
	}else if(argv[1][1]=='1')
	{
		startup_type=1;
	}else if(argv[1][0]=='2')
	{
		startup_type=2;
	}else if(argv[1][0]=='3')
	{
		/* changed from 3 to 2 to disable MICR portions */
		startup_type=2;
		archive_full_micr_check=1;	
	}else if(argv[1][0]=='4')
	{
		startup_type=4;
	}else if(argv[1][0]=='5')
	{
		startup_type=5;
	}else if(argv[1][0]=='6')
	{
		/* changed from 6 to 5 to disable MICR portions */
		startup_type=5;
		archive_full_micr_check=1;	
	}else{
		startup_type=1;
	}
	switch(startup_type)
	{
		case  2:
			/* PLAIN TEXT STUB CHECK */
			printstubform=1;
			PRINT_COLUMN_HEADERS=FALSE;
			break;
		case  3:
			/* MICR STUB CHECK */
			printstubform=1;
			PRINT_COLUMN_HEADERS=FALSE;
			break;
		case  4:
			/* PLAIN TEXT DS CHECK */
			doublestub=1;
			printstubform=0;
			PRINT_COLUMN_HEADERS=TRUE;
			break;
		case  5:
			/* DS STUB CHECK */
			doublestub=1;
			printstubform=1;
			PRINT_COLUMN_HEADERS=FALSE;
			break;
		case  6:
			/* DS MICR STUB CHECK */
			doublestub=1;
			printstubform=1;
			PRINT_COLUMN_HEADERS=FALSE;
			break;
		case  1:
		default:
			/* PLAIN TEXT CHECK */
			printstubform=0;
			PRINT_COLUMN_HEADERS=TRUE;
			break;
	}

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"VENDOR CHECKS MICR")) 
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
	/*
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
	*/
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
	gsv=RDAGenericSetupNew(module,"VENCHECK TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			VENCHECK_TYPE=*gsv->value.short_value;
		} else {
			VENCHECK_TYPE=0;
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
	mainrsrc=RDArsrcNEW(module,"ARCHIVE VENDOR CHECKS MICR");
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
	/*
	if(output_device!=NULL) Rfree(output_device);
	output_device=DefaultPrinter();
	addbtnrsrc(mainrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncOutputDevice);
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,output_device,TRUE);
	*/
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,output_device,FALSE);
	addstdrsrc(mainrsrc,"COMMENTS 1",PLAINTEXT,75,comments1,TRUE);
	addstdrsrc(mainrsrc,"COMMENTS 2",PLAINTEXT,75,comments2,TRUE);
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
	SetRangersrcsensitive(mainrsrc);
	if(mainrsrc->scn->name!=NULL) Rfree(mainrsrc->scn->name);
	switch(startup_type)
	{
		case  1:
		default:
			/* PLAIN TEXT CHECK */
			mainrsrc->scn->name=stralloc("ARCHIVE VENDOR PLAIN CHECKS");
			w=mainrsrc->scn->wdgts+3;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  2:
			/* DECORATED STUB PLAIN CHECK */
			mainrsrc->scn->name=stralloc("ARCHIVE VENDOR DECORATED STUB PLAIN CHECKS");
			w=mainrsrc->scn->wdgts+19;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  3:
			/* MICR STUB CHECK */
			mainrsrc->scn->name=stralloc("ARCHIVE VENDOR DECORATED STUB MICR CHECKS");
			w=mainrsrc->scn->wdgts+41;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  4:
			/* PLAIN TEXT DS CHECK */
			mainrsrc->scn->name=stralloc("ARCHIVE VENDOR PLAIN DOUBLE STUB CHECKS");
			w=mainrsrc->scn->wdgts+63;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  5:
			/* DS STUB CHECK */
			mainrsrc->scn->name=stralloc("ARCHIVE VENDOR DECORATED DOUBLE STUB CHECKS");
			w=mainrsrc->scn->wdgts+79;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  6:
			/* DS MICR STUB CHECK */
			mainrsrc->scn->name=stralloc("ARCHIVE VENDOR DECORATED DOUBLE STUB MICR CHECKS");
			w=mainrsrc->scn->wdgts+101;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
	}
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
