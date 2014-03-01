
/* payvwcheckds.c - Vendor Withholding Checks - Double Stub */
#ifndef WIN32
#define __NAM__ "payvwcheckmicr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payvwcheckmicr.exe"
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

int BODY_COUNT=0;

char *module="PAYROLL",*output_device=NULL;
char check_formfeed=FALSE,top_feeder=FALSE;
char *bank_id=NULL;
short vennum=(-1),payddsv=(-1),ddsnum=(-1);
short banknum=(-1),venadr=(-1);
short ADDRESS_OFFSET=(-1),check_offset=(-1),signature_type=(-1);
static RDAvirtual *VendorAddr1=NULL,*VendorAddr2=NULL,*VendorAddr3=NULL;
static short VWCHECK_TYPE=0;
char *comments1=NULL,*comments2=NULL;
static char PRINT_COLUMN_HEADERS=FALSE;
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
static short printstubform=0;
static short usersigsec=0;

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
	/* payto_xpos=vals[8]; */
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
	short vwcheck_laser [] = { 1380, 2640, 1780, 2640, 1980, 2640, 1, 2440, 475, 2735, 2785, 2835, 2885 };
	short payvwcheckds [] = { 1350, 2688, 1785, 2688, 1980, 2688, 60, 2540, 480, 2788, 2838, 2888, 2938 };

	switch(VWCHECK_TYPE)
	{
		default:
		case 0:    /* VWCHECK-PCG  */
		case 1:    /* VWCHECK-SHORT STUB DOT MATRIX  */
		case 2:    /* VWCHECK-CCH  */
		case 3:    /* VWCHECK-LASER  */
		case 4:    /* VWCHECK-CLARKE  */
		case 5:    /* VWCHECK-TAB  */
		case 6:    /* VWCHECK-ASCII  */
		case 7:    /* VWCHECK-MBGUI  */
			SetCoordinateValues(vwcheck_laser);
			break;
		case 8:    /* PAYVWCHECKDS  */
			SetCoordinateValues(payvwcheckds);
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
	addDBfield(dbsrt,"SHORT DESCRIPTION",1,40);
	addDBfield(dbsrt,"DDSV DESCRIPTION",1,40);

	addDBfield(dbsrt,"CHECK DATE",DATES,10);
	addDBfield(dbsrt,"AMOUNT",2,20);
	addDBfield(dbsrt,"VENDOR NAME",1,40);
	addDBfield(dbsrt,"VENDOR IDENTIFICATION",1,30);
	addDBfield(dbsrt,"VENDOR ADDRESS 1",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 2",1,40);
	addDBfield(dbsrt,"VENDOR ADDRESS 3",1,40);
	addDBfield(dbsrt,"IDENTIFICATION",1,20);
	addDBfield(dbsrt,"IDENTIFICATION NUMBER",1,15);
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

	COPYFIELD(ddsnum,dbsrt->fileno,"SHORT DESCRIPTION");
	COPYFIELD(ddsnum,dbsrt->fileno,"IDENTIFICATION");
	COPYFIELD(ddsnum,dbsrt->fileno,"IDENTIFICATION NUMBER");
	
	
	COPYFIELD(payddsv,dbsrt->fileno,"CHECK DATE");
	COPYFIELD(payddsv,dbsrt->fileno,"AMOUNT");
	COPYFIELD(payddsv,dbsrt->fileno,"VENDOR IDENTIFICATION");
	COPYFIELD(vennum,dbsrt->fileno,"VENDOR NAME");
	COPYFIELD(vennum,dbsrt->fileno,"DEFAULT CHECK DETAIL");
	FIELDCOPY(payddsv,"DESCRIPTION",dbsrt->fileno,"DDSV DESCRIPTION");
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
		}else if((print_type==2)||(security_font_type==0)) {
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
			RDA_fprintf(fp,"%c*p%dx%dY%s",27,spelled_amount_xpos,spelled_amount_ypos,spelled_amount);
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
		}else if((spelled_amount!=NULL)&&(print_type==3)) {
			RDA_fprintf(fp,"%c*p%dx%dY{%s}",27,spelled_amount_xpos,spelled_amount_ypos,spelled_amount);
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
		}else{
			RDA_fprintf(fp,"%c*p%dx%dY%s",27,spelled_amount_xpos,spelled_amount_ypos,spelled_amount);

		}
		if(spelled_amount!=NULL) Rfree(spelled_amount);

		memset(payto1,0,120);
		memset(payto2,0,120);
		memset(payto3,0,120);
		memset(payto4,0,120);

#ifdef WIN32
		_snprintf(payto1,120,"%s",emp_name);
		_snprintf(payto2,120,"%s",emp_add1);
		_snprintf(payto3,120,"%s",emp_add2);
		_snprintf(payto4,120,"%s",emp_add3);
#endif
#ifndef WIN32
		snprintf(payto1,120,"%s",emp_name);
		snprintf(payto2,120,"%s",emp_add1);
		snprintf(payto3,120,"%s",emp_add2);
		snprintf(payto4,120,"%s",emp_add3);
#endif
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
					for(x=0;x<120;x++)
					{
						if(payto1[x]==' ')
						{
							payto1[x]='_';
						}
						/*
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
						*/
					}
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
			RDA_fprintf(fp,"%c*p%dx%dY%06d",27,checknumber_xpos,checknumber_ypos,checknumber);
		}
		if((print_type==1)||(print_type==3))
		{
			SelectPrintTypeByName(output_device,"USE FONT ARIAL 18",fp);
			RDA_fprintf(fp,"%c*p%dx%dY%06d",27,checknumber_xpos,checknumber_ypos,checknumber);
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
				RDA_fprintf(fp,"%c*p%dx%dY%s",27,spelled_amount_xpos,spelled_amount_ypos,(spelled_amount!=NULL ? spelled_amount:""));
				RDA_fprintf(fp,"%c*p%dx%dY%s",27,checkdate_xpos,checkdate_ypos,(checkdate!=NULL ? checkdate:""));
				RDA_fprintf(fp,"%c*p%dx%dY%s",27,amount_xpos,amount_ypos,(tempstr!=NULL?tempstr:""));
				break;
			case 1:
			case 2:
				RDA_fprintf(fp,"%c*p%dx%dY%s",27,checkdate_xpos,checkdate_ypos,(checkdate!=NULL ? checkdate:""));
				memset(stemp,0,101);
#ifdef WIN32
				_snprintf(stemp,(RDAstrlen(tempstr)),"%s",tempstr);
#endif
#ifndef WIN32
				snprintf(stemp,(RDAstrlen(tempstr)),"%s",tempstr);
#endif
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
				RDA_fprintf(fp,"%c*p%dx%dY%s",27,amount_xpos,amount_ypos,stemp);
				break;
			default:
				RDA_fprintf(fp,"%c*p%dx%dY{%s}",27,checkdate_xpos,checkdate_ypos,(checkdate!=NULL ? checkdate:""));
				memset(stemp,0,101);
#ifdef WIN32
				_snprintf(stemp,(RDAstrlen(tempstr)+1),"{%s",tempstr);
#endif
#ifndef WIN32
				snprintf(stemp,(RDAstrlen(tempstr)+1),"{%s",tempstr);
#endif
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
				RDA_fprintf(fp,"%c*p%dx%dY%s}",27,amount_xpos,amount_ypos,stemp);
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
					RDA_fprintf(fp,"%c*p%dx%dY[%s]",27,payto_xpos,payto1_ypos,payto1);
				}
				SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
				SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
				if((RDAstrlen(payto2))!=0)
				{
					RDA_fprintf(fp,"%c*p%dx%dY%s",27,payto_xpos,payto2_ypos,payto2);
				}
				if((RDAstrlen(payto3))!=0)
				{
					RDA_fprintf(fp,"%c*p%dx%dY%s",27,payto_xpos,payto3_ypos,payto3);
				}
				if((RDAstrlen(payto4))!=0)
				{
					RDA_fprintf(fp,"%c*p%dx%dY%s",27,payto_xpos,payto4_ypos,payto4);
				}
				break;
			default:
				if((RDAstrlen(payto1))!=0)
				{
					RDA_fprintf(fp,"%c*p%dx%dY%s",27,payto_xpos,payto1_ypos,payto1);
				}
				SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
				SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
				if((RDAstrlen(payto2))!=0)
				{
					RDA_fprintf(fp,"%c*p%dx%dY%s",27,payto_xpos,payto2_ypos,payto2);
				}
				if((RDAstrlen(payto3))!=0)
				{
					RDA_fprintf(fp,"%c*p%dx%dY%s",27,payto_xpos,payto3_ypos,payto3);
				}
				if((RDAstrlen(payto4))!=0)
				{
					RDA_fprintf(fp,"%c*p%dx%dY%s",27,payto_xpos,payto4_ypos,payto4);
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
							RDA_fprintf(fp,"%c*p%dx%dY%c",27,micr_xpos[x],micr_bottom_ypos,my_micr_xval[x]);
						}else if(micr_check_type==1) {
							RDA_fprintf(fp,"%c*p%dx%dY%c",27,micr_xpos[x],micr_middle_ypos,my_micr_xval[x]);
						}
					}
				}
			}
			memset(stemp,0,101);
			if(micr_check_type==0)
			{
				sprintf(stemp,"USE_PCLMACRO_%04d",(checkform+3100));
				if(diagrptgen) { prterr("Sending Call For PCL Macro [%d].\n",(checkform+3100)); }
			}else if(micr_check_type==1) {
				sprintf(stemp,"USE_PCLMACRO_%04d",(checkform+3200));
				if(diagrptgen) { prterr("Sending Call For PCL Macro [%d].\n",(checkform+3200)); }
			}
			SelectPrintTypeByName(output_device,stemp,fp);
		}
		SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
		SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);
}
static void RepeatStub(short sfile,int checkno,int vcheckno,RDA_PFILE *fp)
{
	short ef=0,count=0;
	int checkno1=0,vcheckno1=0,fyear=0,trnnum=0;
	char *ddsid=NULL;
	double amt=0.0;
	char *desc=NULL;
	char *ddms_ident=NULL,*ddms_id_num=NULL;
	char *emp_id=NULL,*emp_name=NULL,*checkdate=NULL;
	char dcd=FALSE;

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

/*
		FINDFLDGETSTRING(sfile,"SHORT DESCRIPTION",&desc);
*/
		FINDFLDGETSTRING(sfile,"DDSV DESCRIPTION",&desc);
		if(RDAstrlen(desc)>40) desc[40]=0;

		FINDFLDGETSTRING(sfile,"IDENTIFICATION",&ddms_ident);
		FINDFLDGETSTRING(sfile,"IDENTIFICATION NUMBER",&ddms_id_num);

		FINDFLDGETSTRING(sfile,"CHECK DATE",&checkdate);
		FINDFLDGETSTRING(sfile,"VENDOR IDENTIFICATION",&emp_id);
		FINDFLDGETSTRING(sfile,"VENDOR NAME",&emp_name);
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&dcd);

		if(count==0)
		{
			RDA_fprintf(fp,"\r\n");	
			RDA_fprintf(fp,"\r\n");	
			RDA_fprintf(fp,"\r\n");	
			RDA_fprintf(fp,"\r\n");	
			RDA_fprintf(fp,"\r\n");	
			if(PRINT_COLUMN_HEADERS)
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"    %-12s   %-30s         %-40s         %-7s      %-12s  \r\n",CenterText("FISCAL YEAR",12),CenterText("DEDUCTION DESCRIPTION ID",30),CenterText("SHORT DESCRIPTION",40),CenterText("TRANS #",7),CenterText("AMOUNT",12));
				RDA_fprintf(fp,"\r\n");	
			}else{
				RDA_fprintf(fp,"                   %8d   %-10s   %-40s   %-30s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),CenterText((emp_name!=NULL ? emp_name:""),40),CenterText((emp_id!=NULL ? emp_id:""),30));
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
			}
			if(dcd==FALSE)
			{
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"     SEE ATTACHED LIST FOR CHECK DETAIL\r\n");
				count+=2;
			}
		}

		if(dcd==TRUE)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%d",fyear);
		/* Center Text
			RDA_fprintf(fp,"   %-8s        %-30s        %-40s        %7d       $%12.2f\r\n",CenterText(stemp,8),CenterText((ddsid!=NULL ? ddsid:""),30),CenterText((desc!=NULL ? desc:""),40),trnnum,amt/100);
		*/
			RDA_fprintf(fp,"   %-8s        %-30s        %-40s        %7d       $%12.2f\r\n",CenterText(stemp,8),(ddsid!=NULL ? ddsid:""),(desc!=NULL ? desc:""),trnnum,amt/100);
			if(ddsid!=NULL) Rfree(ddsid);
			++count;
		}
		ef=NXTNRD(sfile,1);
	}

	while(count<BODY_COUNT)
	{
		RDA_fprintf(fp,"\r\n");	
		++count;
	}
	RDA_fprintf(fp,"\r\n");	
}
static void OutputChecks(short sfile)
{
	short ef=0,count=0;
	int checkno=0,vcheckno=0,last_checkno=0,last_vcheckno=0,fyear=0,trnnum=0;
	RDA_PFILE *fp=NULL;
	double total_amt=0.0,amt=0.0;
	char *emp_id=NULL,*emp_name=NULL,*emp_add1=NULL,*emp_add2=NULL,*emp_add3=NULL;
	char *checkdate=NULL;
	char *ddms_ident=NULL,*ddms_id_num=NULL;
	char dcd=FALSE;
	static short stubcounter=0;
	static short close_page=0;
/*
	int x=0;
*/
	char *desc=NULL;
	char *ddsid=NULL;
	char *sig_filename=NULL,*sig_password=NULL;

	if(isEMPTY(output_device))
	{
		prterr("Error:  An appropriate output device must be selected to print checks.");
		return;
	}

	if((strncmp(output_device,"viewpdf",7))==0) 
	{
		prterr("Error:  Can Not Use VIEWPDF As An Output Device.\n");
		ERRORDIALOG("Invalid Output Device","Error:  Can Not Use VIEWPDF As An Output Device.\n",NULL,TRUE);
		RDAAPPMAINLOOP();
		return;
	}

	/* Check Users Security For Sig Auth Before Enabling it. */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PAYROLL SIGNATURE FILENAME",&sig_filename);
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PAYROLL SIGNATURE PASSWORD",&sig_password);
		}
	}else{
		prterr("Error Opening Security File For Signature Security Check.\n");
	}
	if((!isEMPTY(sig_filename))&&(!isEMPTY(sig_password)))
	{
		usersigsec=1;
	}
	if(sig_filename!=NULL) { Rfree(sig_filename); }
	if(sig_password!=NULL) { Rfree(sig_password); }
	


	fp=RDA_popen(output_device,"w");
	if(fp==NULL)
	{
		prterr("Error:  Couldn't open output device [%s].",output_device);
	}
	SelectPrintTypeByName(output_device,"PCL JOB HEADER",fp);
	if((usersigsec)&&(signature_type==5))
	{
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_200",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_201",fp);
		SelectPrintTypeByName(output_device,"LOAD PAYROLL SIGNATURE",fp);
	}
	if(micr_font_type==0)
	{
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_60",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_60",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",60); }
	}else if(micr_font_type==1){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_61",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_61",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",61); }
	}else if(micr_font_type==2){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_62",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_62",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",62); }
	}else if(micr_font_type==3){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_63",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_63",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",63); }
	}else if(micr_font_type==4){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_64",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_64",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",64); }
	}else if(micr_font_type==5){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_65",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_65",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",65); }
	}else if(micr_font_type==6){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_66",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_66",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",66); }
	}else if(micr_font_type==7){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_67",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_67",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",67); }
	}else if(micr_font_type==8){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_68",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_68",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",68); }
	}else if(micr_font_type==9){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_69",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_69",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",69); }
	}else if(micr_font_type==10){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_70",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_70",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",70); }
	}else if(micr_font_type==11){
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_71",fp);
		SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_71",fp);
		if(diagrptgen) { prterr("Loading Soft Font [%d].\n",71); }
	}

	switch(security_font_type)
	{
		case 1:
		case 2:
			SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_73",fp);
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_73",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",73); }
			break;
		case 3:
		case 4:
			SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_77",fp);
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_77",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",77); }
			break;
		case 5:
		case 6:
		case 7:
			SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_77",fp);
			SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_79",fp);
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_77",fp);
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_79",fp);
			if(diagrptgen) { prterr("Loading Soft Fonts [%d] and [%d].\n",77,79); }
			break;
		case 8:
		case 9:
			SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_83",fp);
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_83",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",83); }
			break;
		case 10:
		case 11:
		case 12:
			SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_83",fp);
			SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_85",fp);
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_83",fp);
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_85",fp);
			if(diagrptgen) { prterr("Loading Soft Fonts [%d] and [%d].\n",83,85); }
			break;
		case 0:
		default:
			break;
	}

	if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
	{
		SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3012",fp);
		SelectPrintTypeByName(output_device,"LOAD_PCLMACRO_3012",fp);

		if(!FileExists("forms/load_pclmacro_3012.pcl"))
		{
			prterr("Can not find PCL form [%d]\n",3012);
		}
	}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
	{
		SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3013",fp);
		SelectPrintTypeByName(output_device,"LOAD_PCLMACRO_3013",fp);

		if(!FileExists("forms/load_pclmacro_3013.pcl"))
		{
			prterr("Can not find PCL form [%d]\n",3013);
		}
	}else if((printstubform==1)&&(BODY_COUNT==46))
	{
		SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3010",fp);
		SelectPrintTypeByName(output_device,"LOAD_PCLMACRO_3010",fp);

		if(!FileExists("forms/load_pclmacro_3010.pcl"))
		{
			prterr("Can not find PCL form [%d]\n",3010);
		}
	}

	memset(stemp,0,101);
	if(micr_check_type==0)
	{
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3100));
		SelectPrintTypeByName(output_device,stemp,fp);
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3300));
		SelectPrintTypeByName(output_device,stemp,fp);

		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3100));
		SelectPrintTypeByName(output_device,stemp,fp);
		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3300));
		SelectPrintTypeByName(output_device,stemp,fp);


		sprintf(stemp,"forms/load_pclmacro_%04d.pcl",(checkform+3100));
		if(!FileExists(stemp))
		{
			prterr("Can not find PCL form [%d]\n",(checkform+3100));
		}
		if(diagrptgen)
		{ 
			sprintf(stemp,"forms/load_pclmacro_%04d.pcl",(checkform+3300));
			if(!FileExists(stemp))
			{
				prterr("Can not find PCL form [%d]\n",(checkform+3300));
			}
		}
	}else if(micr_check_type==1) {
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3200));
		SelectPrintTypeByName(output_device,stemp,fp);
		sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3400));
		SelectPrintTypeByName(output_device,stemp,fp);

		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3200));
		SelectPrintTypeByName(output_device,stemp,fp);
		sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3400));
		SelectPrintTypeByName(output_device,stemp,fp);

		sprintf(stemp,"forms/load_pclmacro_%04d.pcl",(checkform+3200));
		if(!FileExists(stemp))
		{
			prterr("Can not find PCL form [%d]\n",(checkform+3200));
		}
		if(diagrptgen)
		{ 
			sprintf(stemp,"forms/load_pclmacro_%04d.pcl",(checkform+3400));
			if(!FileExists(stemp))
			{
				prterr("Can not find PCL form [%d]\n",(checkform+3400));
			}
		}
	}


	SelectPrintTypeByName(output_device,"SIMPLEX",fp);
	SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);
	RDA_fprintf(fp,"\f");
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
		if((last_vcheckno!=0)&&(last_checkno!=0)&&(checkno!=last_checkno || vcheckno!=last_vcheckno))
		{
			close_page=1;
		}
		if((close_page==1)||(close_page==2))
		{
			/* Print Totals and MICR Line */
			if((count!=0)&&(count<BODY_COUNT))
			{
				while(count<BODY_COUNT)
				{
					RDA_fprintf(fp,"\r\n");	
					++count;
				}
			}else if((stubcounter!=0)&&(stubcounter<BODY_COUNT)) 
			{
				while(stubcounter<BODY_COUNT)
				{
					RDA_fprintf(fp,"\r\n");	
					++stubcounter;
				}
			}

			/* Allow names longer than 30 characters if using Courier font.  Field default is 40 */
			if(security_font_type!=0)
			{
				if(RDAstrlen(emp_name)>30) emp_name[30]=0;
			}
			if((checkno==last_checkno)&&(printstubform==1))
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"                  %-20s                          %-15s                            Sub-Total   $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());

			}else if((checkno==last_checkno)&&(printstubform==0))
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"%10s %8d %-30s  Sub-Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());

			}else if((checkno!=last_checkno)&&(printstubform==1))
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"                  %-20s                          %-15s                              Total     $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());

			}else if((checkno!=last_checkno)&&(printstubform==0))
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"%10s %8d %-30s      Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
			}

			/* Process Double Stub Lines */
			if((doublestub==1)&&(micr_check_type!=1)&&(use_full_micr_check==1))
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RepeatStub(sfile,last_checkno,last_vcheckno,fp);
				if(printstubform)
				{
					RDA_fprintf(fp,"                  %-20s                          %-15s                              Total     $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
				}else{
					RDA_fprintf(fp,"%10s %8d %-30s       Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
				}
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
			}else if((doublestub==1)&&(micr_check_type!=1)&&(use_full_micr_check==0))
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
				RepeatStub(sfile,last_checkno,last_vcheckno,fp);
					if(printstubform)
				{
					RDA_fprintf(fp,"                  %-20s                          %-15s                              Total     $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
				}else{
					RDA_fprintf(fp,"%10s %8d %-30s       Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
				}
				RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
				RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
			}
			/* End Double Stub Lines */

			if(use_full_micr_check==0)
			{
				SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
				if(checkno!=last_checkno)
				{
					PrintMICRCheckForm(0,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					if((usersigsec)&&(signature_type==5))
					{
						SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
						SelectPrintTypeByName(output_device,"PAYROLL SIGNATURE",fp);
						SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					}
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
					if((usersigsec)&&(signature_type==5))
					{
						SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
						SelectPrintTypeByName(output_device,"PAYROLL SIGNATURE",fp);
						SelectPrintTypeByName(output_device,"POP CURSOR",fp);
					}
				} else {
					PrintMICRCheckForm(3,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
					SelectPrintTypeByName(output_device,"POP CURSOR",fp);
				}
			}
			SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);


			/* Print Decorated Stub */
			if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
			{
				SelectPrintTypeByName(output_device,"USE_PCLMACRO_3012",fp);
			}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
			{
				SelectPrintTypeByName(output_device,"USE_PCLMACRO_3013",fp);
			}else if((printstubform==1)&&(BODY_COUNT==46))
			{
				SelectPrintTypeByName(output_device,"USE_PCLMACRO_3010",fp);
			}

			RDA_fprintf(fp,"\f");
			close_page=0;
		}

		/* Increment counters for detail lines */

		if(checkno!=last_checkno || vcheckno!=last_vcheckno)
		{
			count=0;	
			stubcounter=0;	
			if(checkno!=last_checkno)
			{
				total_amt=0;
			}
			last_checkno=checkno;
			last_vcheckno=vcheckno;
		}else{
			if(doublestub==0)
			{
			}

		}

	/* print 1st detail stub */
		FINDFLDGETINT(sfile,"FISCAL YEAR",&fyear);
		FINDFLDGETSTRING(sfile,"DEDUCTION DESCRIPTION IDENTIFICATION",&ddsid);
		FINDFLDGETINT(sfile,"TRANSACTION NUMBER",&trnnum);
		FINDFLDGETSTRING(sfile,"CHECK DATE",&checkdate);
		FINDFLDGETSTRING(sfile,"NAME",&emp_name);
		FINDFLDGETDOUBLE(sfile,"AMOUNT",&amt);
		FINDFLDGETSTRING(sfile,"VENDOR IDENTIFICATION",&emp_id);
		FINDFLDGETSTRING(sfile,"VENDOR NAME",&emp_name);
		if(security_font_type!=0)
		{
			if(RDAstrlen(emp_name)>30) emp_name[30]=0;
		}

		FINDFLDGETSTRING(sfile,"DDSV DESCRIPTION",&desc);
		/*
		FINDFLDGETSTRING(sfile,"SHORT DESCRIPTION",&desc);
		*/
		if(RDAstrlen(desc)>40) desc[40]=0;
/*
*/
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 1",&emp_add1);
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 2",&emp_add2);
		FINDFLDGETSTRING(sfile,"VENDOR ADDRESS 3",&emp_add3);
		FINDFLDGETSTRING(sfile,"IDENTIFICATION",&ddms_ident);
		FINDFLDGETSTRING(sfile,"IDENTIFICATION NUMBER",&ddms_id_num);
		FINDFLDGETCHAR(sfile,"DEFAULT CHECK DETAIL",&dcd);


		if((count==0)||(stubcounter==0))
		{
			if(PRINT_COLUMN_HEADERS)
			{
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"    %-12s   %-30s         %-40s         %-7s      %-12s  \r\n",CenterText("FISCAL YEAR",12),CenterText("DEDUCTION DESCRIPTION ID",30),CenterText("SHORT DESCRIPTION",40),CenterText("TRANS #",7),CenterText("AMOUNT",12));
				RDA_fprintf(fp,"\r\n");	
			}else{
				RDA_fprintf(fp,"                   %8d   %-10s   %-40s   %-30s\r\n",vcheckno,(checkdate!=NULL ? checkdate:""),CenterText((emp_name!=NULL ? emp_name:""),40),CenterText((emp_id!=NULL ? emp_id:""),30));
				RDA_fprintf(fp,"\r\n");	
				RDA_fprintf(fp,"\r\n");	
			}
			if(dcd==FALSE)
			{
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"     SEE ATTACHED LIST FOR CHECK DETAIL\r\n");
				stubcounter+=2;
				count+=2;
			}
		}


		if(dcd==TRUE)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%d",fyear);
		// This should really be caught into array of pointers so that the values can be reused in the RepeatStub function
		/* Center Text
			RDA_fprintf(fp,"   %-8s        %-30s        %-40s        %7d       $%12.2f\r\n",CenterText(stemp,8),CenterText((ddsid!=NULL ? ddsid:""),30),CenterText((desc!=NULL ? desc:""),40),trnnum,amt/100);
		*/
			RDA_fprintf(fp,"   %-8s        %-30s        %-40s        %7d       $%12.2f\r\n",CenterText(stemp,8),(ddsid!=NULL ? ddsid:""),(desc!=NULL ? desc:""),trnnum,amt/100);
			++stubcounter;
			if(ddsid!=NULL) Rfree(ddsid);
			if(doublestub==0)
			{
				if((stubcounter==BODY_COUNT)&&(use_full_micr_check==1))
				{
					close_page=2;
					stubcounter=0;
				} else if((stubcounter==BODY_COUNT)&&(use_full_micr_check==0)) {
					close_page=2;
					stubcounter=0;
				}
			}
			++count;
		}
		total_amt+=amt;
		ef=NXTNRD(sfile,1);
	}


/* repeat stub and print negotiable */

	if((doublestub==1))
	{
		if((count!=0)&&(count<BODY_COUNT))
		{
			while(count<BODY_COUNT)
			{
				RDA_fprintf(fp,"\r\n");	
				++count;
			}
		}else if((stubcounter!=0)&&(stubcounter<BODY_COUNT)) 
		{
			while(stubcounter<BODY_COUNT)
			{
				RDA_fprintf(fp,"\r\n");	
				++stubcounter;
			}
		}

		RDA_fprintf(fp,"\r\n");	
		if(printstubform)
		{
			RDA_fprintf(fp,"                  %-20s                          %-15s                              Total     $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
		}else{
			RDA_fprintf(fp,"%10s %8d %-30s       Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
		}
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}

	if((doublestub==1)&&(micr_check_type!=1)&&(use_full_micr_check==1))
	{
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RepeatStub(sfile,last_checkno,last_vcheckno,fp);
		if(printstubform)
		{
			RDA_fprintf(fp,"                  %-20s                          %-15s                              Total     $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
		}else{
			RDA_fprintf(fp,"%10s %8d %-30s       Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
		}
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}else if((doublestub==1)&&(micr_check_type!=1)&&(use_full_micr_check==0))
	{
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RDA_fprintf(fp,"\r\n");	
		RepeatStub(sfile,last_checkno,last_vcheckno,fp);
		if(printstubform)
		{
			RDA_fprintf(fp,"                  %-20s                          %-15s                              Total     $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
		}else{
			RDA_fprintf(fp,"%10s %8d %-30s       Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
		}
		RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
		RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
	}else{
		/* Print Totals and MICR Line */
		if((count!=0)&&(count<BODY_COUNT))
		{
			while(count<BODY_COUNT)
			{
				RDA_fprintf(fp,"\r\n");	
				++count;
			}
		}else if((stubcounter!=0)&&(stubcounter<BODY_COUNT)) 
		{
			while(stubcounter<BODY_COUNT)
			{
				RDA_fprintf(fp,"\r\n");	
				++stubcounter;
			}
		}

		if(security_font_type!=0)
		{
			if(RDAstrlen(emp_name)>30) emp_name[30]=0;
		}

		if((checkno==last_checkno)&&(printstubform==1))
		{
			RDA_fprintf(fp,"\r\n");	
			RDA_fprintf(fp,"                  %-20s                          %-15s                              Total     $%12.2f\r\n",ddms_ident,ddms_id_num,total_amt/100);
			RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
			RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());

		}else if((checkno==last_checkno)&&(printstubform==0))
		{
			RDA_fprintf(fp,"\r\n");	
			RDA_fprintf(fp,"%10s %8d %-30s      Total: $%12.2f\r\n",(checkdate!=NULL?checkdate:""),last_checkno,(emp_name!=NULL ? emp_name:""),total_amt/100);
			RDA_fprintf(fp,"  %-75s\r\n",(comments1!=NULL ? comments1:""));
			RDA_fprintf(fp,"  %-75s                 %s                 %s\r\n",(comments2!=NULL ? comments2:""),GETCURRENTDATE10(),GETCURRENTTIME());
		}

	}



	if(use_full_micr_check==0)
	{
		SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
		if(checkno==last_checkno)
		{
			PrintMICRCheckForm(0,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
			SelectPrintTypeByName(output_device,"POP CURSOR",fp);
			if((usersigsec)&&(signature_type==5))
			{
				SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
				SelectPrintTypeByName(output_device,"PAYROLL SIGNATURE",fp);
				SelectPrintTypeByName(output_device,"POP CURSOR",fp);
			}
		} else {
			PrintMICRCheckForm(2,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
			SelectPrintTypeByName(output_device,"POP CURSOR",fp);
		}
	}else{
		SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
		if(checkno==last_checkno)
		{
			PrintMICRCheckForm(1,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device);
			SelectPrintTypeByName(output_device,"POP CURSOR",fp);
			if((usersigsec)&&(signature_type==5))
			{
				SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
				SelectPrintTypeByName(output_device,"PAYROLL SIGNATURE",fp);
				SelectPrintTypeByName(output_device,"POP CURSOR",fp);
			}
		} else {
			PrintMICRCheckForm(3,fp,last_vcheckno,checkdate,total_amt,emp_name,emp_add1,emp_add2,emp_add3,output_device); 
			SelectPrintTypeByName(output_device,"POP CURSOR",fp);
		}
	}

	/* Print Decorated Stub */
	if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==0))
	{
		SelectPrintTypeByName(output_device,"USE_PCLMACRO_3012",fp);
	}else if((printstubform==1)&&(BODY_COUNT==15)&&(doublestub==1))
	{
		SelectPrintTypeByName(output_device,"USE_PCLMACRO_3013",fp);
	}else if((printstubform==1)&&(BODY_COUNT==46))
	{
		SelectPrintTypeByName(output_device,"USE_PCLMACRO_3010",fp);
	}


	count=0;	
	stubcounter=0;

	/* prterr("COUNT [%d]  SUB COUNT [%d]\n",count,stubcounter); */
	if(checkno!=last_checkno)
	{
		close_page=1;
		total_amt=0;
	}
	last_checkno=checkno;
	last_vcheckno=vcheckno;
	if(use_full_micr_check)
	{
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_60",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_61",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_62",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_63",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_64",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_65",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_66",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_67",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_68",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_69",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_70",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_71",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_72",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_73",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_74",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_75",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_76",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_77",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_78",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_79",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_80",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_81",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_82",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_83",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_84",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_85",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_86",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_87",fp);
		memset(stemp,0,101);
		if(micr_check_type==0)
		{
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3100));
			SelectPrintTypeByName(output_device,stemp,fp);
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3300));
			SelectPrintTypeByName(output_device,stemp,fp);
		}else if(micr_check_type==1) {
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3200));
			SelectPrintTypeByName(output_device,stemp,fp);
			sprintf(stemp,"DELETE_PCLMACRO_%04d",(checkform+3400));
			SelectPrintTypeByName(output_device,stemp,fp);
		}
	}
	if(signature_type==5)
	{
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_200",fp);
		SelectPrintTypeByName(output_device,"DELETE_SOFTFONT_201",fp);
	}
	SelectPrintTypeByName(output_device,"PCL JOB FOOTER",fp);
	RDA_pclose(fp);
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
	char manual_voucher=FALSE,deleteflag=FALSE,verified=FALSE;
	char electronic=FALSE;
	/*
	char cleared=FALSE;
	char distributed=FALSE;
	*/
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
	/* Set MICR Values */
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
	myptr=NULL;

	if((((micr_check_type==0)&&(micr_bottom_ypos!=0)||((micr_check_type==1)&&(micr_middle_ypos!=0))))&&(((strlen(micrtext)!=0)))&&((startup_type==3)||(startup_type==6)))
	{
		use_full_micr_check=1;	
	}

	if(micrtext!=NULL) Rfree(micrtext);

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
	/* End Setting MICR Values */

	if((amount_xpos==0)&&(amount_ypos==0))
	{
		PreloadCoordinates();
	}
	if((startup_type!=3)&&(startup_type!=6))
	{
		security_font_type=0;
	}

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



	/* prterr("BODY_COUNT = [%d]\n",BODY_COUNT); */

	FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&output_device);
	FINDRSCGETSTRING(mainrsrc,"COMMENTS 1",&comments1);
	FINDRSCGETSTRING(mainrsrc,"COMMENTS 2",&comments2);
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
		FINDFLDGETCHAR(payddsv,"ELECTRONIC TRANSFER",&electronic);
		/*
		FINDFLDGETCHAR(payddsv,"BNKREC UPDATED",&cleared);
		FINDFLDGETCHAR(payddsv,"DISTRIBUTED",&distributed);
		if(verified && !cleared && !manual_voucher && !electronic && !deleteflag && !distributed)
		*/
		if(verified && !manual_voucher && !electronic && !deleteflag)
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
		ERRORDIALOG("Error Printing Checks","No Vendor Vouchers Selected for Check Printing.\n",NULL,TRUE);
		if(bankid1!=NULL) Rfree(bankid1);
		if(sortfile!=NULL) 
		{
			CLSDBsort(sortfile);
			FreeDBsort(sortfile);
		}
		RDAAPPMAINLOOP();
	}
	if(bankid1!=NULL) Rfree(bankid1);
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
	quitfunc(mainrsrc);
}
static void PayvwResetGroupDefaults(RDArsrc *r)
{
	ResetGroupDefaults(r);
	/*
	rsrc2filerecord(banknum,r);
	ReadRDAScrolledLists(r);
	filerecord2rsrc(banknum,r);
	updatersrc(r,"BNKACC BANK IDENTIFICATIONS");
	SetListValue(banknum,r,RDAScrolledList *List,updatevirtual);
	computeallSCRNvirtuals(r);
	*/
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
	RDAwdgt *w=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"VENDOR WITHHOLDING CHECKS MICR")) 
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
			ADDRESS_OFFSET=*gsv->value.short_value;
		} else {
			ADDRESS_OFFSET=0;
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
	gsv=RDAGenericSetupNew("PAYROLL","V/W BANK IDENTIFICATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			bank_id=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"VW CHECK TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			VWCHECK_TYPE=*gsv->value.short_value;
		} else {
			VWCHECK_TYPE=0;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
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
		startup_type=3;
	}else if(argv[1][0]=='4')
	{
		startup_type=4;
	}else if(argv[1][0]=='5')
	{
		startup_type=5;
	}else if(argv[1][0]=='6')
	{
		startup_type=6;
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

	if((payddsv=APPOPNNRD("PAYROLL","PAYDDSV",TRUE,FALSE))==(-1)) return;
	if((ddsnum=APPOPNNRD("PAYROLL","PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((venadr=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((banknum=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"VENDOR WITHHOLDING CHECKS MICR");
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
	addstdrsrc(mainrsrc,"COMMENTS 1",PLAINTEXT,75,comments1,TRUE);
	addstdrsrc(mainrsrc,"COMMENTS 2",PLAINTEXT,75,comments2,TRUE);
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
		/*filerecord2rsrc(banknum,mainrsrc);*/
	addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,PayvwResetGroupDefaults,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,setcheck,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	if(mainrsrc->scn->name!=NULL) Rfree(mainrsrc->scn->name);
	switch(startup_type)
	{
		case  1:
		default:
			/* PLAIN TEXT CHECK */
			mainrsrc->scn->name=stralloc("VENDOR WITHHOLDING PLAIN CHECKS");
			w=mainrsrc->scn->wdgts+3;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  2:
			/* DECORATED STUB PLAIN CHECK */
			mainrsrc->scn->name=stralloc("VENDOR WITHHOLDING DECORATED STUB PLAIN CHECKS");
			w=mainrsrc->scn->wdgts+13;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  3:
			/* MICR STUB CHECK */
			mainrsrc->scn->name=stralloc("VENDOR WITHHOLDING DECORATED STUB MICR CHECKS");
			w=mainrsrc->scn->wdgts+29;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  4:
			/* PLAIN TEXT DS CHECK */
			mainrsrc->scn->name=stralloc("VENDOR WITHHOLDING PLAIN DOUBLE STUB CHECKS");
			w=mainrsrc->scn->wdgts+45;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  5:
			/* DS STUB CHECK */
			mainrsrc->scn->name=stralloc("VENDOR WITHHOLDING DECORATED DOUBLE STUB CHECKS");
			w=mainrsrc->scn->wdgts+55;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  6:
			/* DS MICR STUB CHECK */
			mainrsrc->scn->name=stralloc("VENDOR WITHHOLDING DECORATED DOUBLE STUB MICR CHECKS");
			w=mainrsrc->scn->wdgts+71;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
	}
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
