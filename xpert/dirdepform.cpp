/* dirdepform.c - Print Payroll Direct Deposit Slips */

#ifndef WIN32
#define __NAM__ "dirdepform.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dirdepform.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct CheckRates 
{
	double salary;
	double units;
	double unit_amt;
	double overs;
	double over_amt;
	double adjust;
	double current;
};
typedef struct CheckRates CheckRate;

#define NUMBER_JOBS	10
#define NUMBER_DEDUCTIONS	36
#define NUMBER_LVEMSTR	10
#define NUMBER_MESSAGE	9

static char display_ssn=FALSE,USE_PAYDCSM=FALSE,SHOW_FILING_STATUS=FALSE;
static char LEAVE_SHORT_DESC=FALSE;
static int number_jobs=NUMBER_JOBS;
static int number_deductions=NUMBER_DEDUCTIONS;
static int number_lvemstr=NUMBER_LVEMSTR;
static short PRINT_NUMBER_MESSAGE=NUMBER_MESSAGE;
static short ADDRESS_OFFSET=0,dirdep_type=0;
static char show_zero_jobs=FALSE,sdesc=FALSE;
static char print_employer_deductions=FALSE;
static char *output_device=NULL;
/*
static char use_sign_locks=FALSE;
static short check_signature;
*/
static short DD_NET_TYPE=0,check_type=0;

static char *module="PAYROLL";
static short perdmg=(-1),payjmst=(-1),payded=(-1),paydds=(-1),posgrs=(-1);
static short bnkacc=(-1),lvemst=(-1),lvedsc=(-1),paydtyp=(-1);
static short paymst=(-1),payjdpm=(-1),payjpms=(-1),payjcsm=(-1),payjdcm=(-1);
static short posmst=(-1),payppms=(-1),payjprm=(-1),formdecimal=FALSE;
static short formleave=FALSE,jdtype=0;
static RDArsrc *diagrsrc=NULL;
static char *message1=NULL,*message2=NULL,*message3=NULL;
static char *message4=NULL,*message5=NULL,*message6=NULL;
static char *message7=NULL,*message8=NULL,*message9=NULL;
static char *paystarting=NULL,*payending=NULL;
static char check_formfeed=FALSE,low_address=FALSE,skip_leave=FALSE;
static short dir_dep_offset=0;
static char *bankno=NULL,*payid=NULL;
static char *defbankid=NULL;
static short DID_STUFF=0;
static DBsort *sortfile=NULL;
static RDAvirtual *EmployeeName=NULL;
static RDAvirtual *EmployeeAddr1=NULL,*EmployeeAddr2=NULL,*EmployeeAddr3=NULL;
static RDAvirtual *OrgAddr1=NULL,*OrgAddr2=NULL,*OrgAddr3=NULL;
static int calyear=0;
static int federal_exemption_location=0;
static int state_exemption_location=0;
static char stemp1[101],stemp2[101];

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
static short PRINT_COLUMN_HEADERS=0,startup_type=0,printstubform=0;

int SetCoordinateValues(short vals[13])
{
	//short myaddr=240;
	//myaddr=(myaddr+(ADDRESS_OFFSET*30));

	checkdate_xpos=vals[0];
	checkdate_ypos=vals[1];
	checknumber_xpos=vals[2];
	checknumber_ypos=vals[3];
	amount_xpos=vals[4];
	amount_ypos=vals[5];
	spelled_amount_xpos=vals[6];
	spelled_amount_ypos=vals[7];
	/*payto_xpos=myaddr;*/
	payto_xpos=vals[8];
	//prterr("in SetCoordinateValues payto_xpos = %d",payto_xpos);
	payto1_ypos=vals[9];
	payto2_ypos=vals[10];
	payto3_ypos=vals[11];
	payto4_ypos=vals[12];
	security_font_type=0;
	//prterr("vals [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d]\n",vals[0],vals[1],vals[2],vals[3],vals[4],vals[5],vals[6],vals[7],vals[8],vals[9],vals[10],vals[11],vals[12]);
}
int PreloadCoordinates()
{
	short high_address_8x11 [] = { 1400, 2636, 1738, 2636, 1950, 2636, 60, 2488, 480, 2717, 2767, 2817, 2867 };
	short defaultsetting [] =    { 1320, 2436, 1738, 2436, 1950, 2436, 149, 2287, 480, 2795, 2845, 2895, 2945 };
		//prterr ("payto_xpos= %d, payto1_ypos= %d, payto2_ypos= %d, payto3_ypos= %d, payto4_ypos= %d",payto_xpos,payto1_ypos,payto2_ypos,payto3_ypos,payto4_ypos);

	switch(check_type)
	{

		case 7:   /* 8.5x11 High Address  */
			SetCoordinateValues(high_address_8x11);
			break;

		default:
			SetCoordinateValues(defaultsetting);
			//prterr("in SetCoordinateValues payto_xpos = %d",payto_xpos);
			//prterr("Using default placement values.\n");
			break;
	}

}
static void QAccFld(short tfileno,char *tfieldname,short ffileno,char *ffieldname)
{
	double amt=0.0,camt=0.0;

	FINDFLDGETDOUBLE(tfileno,tfieldname,&camt);
	FINDFLDGETDOUBLE(ffileno,ffieldname,&amt);
	camt+=amt;
	FINDFLDSETDOUBLE(tfileno,tfieldname,camt);
}
static char UpdatedAlready(char *perid,int job)
{
	char *payid1=NULL,*perid1=NULL,v=FALSE,delflag=FALSE;
	int job1=0;
	short ef=0;

	ZERNRD(payjprm);
	FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjprm,"JOB NUMBER",job);
	ef=ADVGTENRDsec(payjprm,1,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjprm,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETSTRING(payjprm,"PERSONNEL IDENTIFICATION",&perid1);
		FINDFLDGETINT(payjprm,"JOB NUMBER",&job1);
		if(job!=job1 || RDAstrcmp(payid,payid1) || RDAstrcmp(perid,perid1)) break;
		FINDFLDGETCHAR(payjprm,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETCHAR(payjprm,"UPDATED",&v);
			if(v==TRUE) break;
		}
		ef=ADVNXTNRDsec(payjprm,1,SCRNvirtualSubData,diagrsrc);
	}
	if(payid1!=NULL) Rfree(payid1);
	if(perid1!=NULL) Rfree(perid1);
	return(v);
}
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(posmst!=(-1)) CLSNRD(posmst);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(payded!=(-1)) CLSNRD(payded);
	if(paydds!=(-1)) CLSNRD(paydds);
	if(paydtyp!=(-1)) CLSNRD(paydtyp);
	if(posgrs!=(-1)) CLSNRD(posgrs);
	if(payppms!=(-1)) CLSNRD(payppms);
	if(paymst!=(-1)) CLSNRD(paymst);
	if(payjdpm!=(-1)) CLSNRD(payjdpm);
	if(payjpms!=(-1)) CLSNRD(payjpms);
	if(payjprm!=(-1)) CLSNRD(payjprm);
	if(payjcsm!=(-1)) CLSNRD(payjcsm);
	if(payjdpm!=(-1)) CLSNRD(payjdpm);
	if(bnkacc!=(-1)) CLSNRD(bnkacc);
	if(lvemst!=(-1)) CLSNRD(lvemst);
	if(lvedsc!=(-1)) CLSNRD(lvedsc);
	if(message1!=NULL) Rfree(message1);
	if(message2!=NULL) Rfree(message2);
	if(message3!=NULL) Rfree(message3);
	if(message4!=NULL) Rfree(message4);
	if(message5!=NULL) Rfree(message5);
	if(message6!=NULL) Rfree(message6);
	if(message7!=NULL) Rfree(message7);
	if(message8!=NULL) Rfree(message8);
	if(message9!=NULL) Rfree(message9);
	if(paystarting!=NULL) Rfree(paystarting);
	if(payending!=NULL) Rfree(payending);
	if(bankno!=NULL) Rfree(bankno);
	if(defbankid!=NULL) Rfree(defbankid);
	if(payid!=NULL) Rfree(payid);
	/* FreeRDAvirtual is defined in this file */
	FreeRDAvirtual(EmployeeName);
	FreeRDAvirtual(EmployeeAddr1);
	FreeRDAvirtual(EmployeeAddr2);
	FreeRDAvirtual(EmployeeAddr3);
	FreeRDAvirtual(OrgAddr1);
	FreeRDAvirtual(OrgAddr2);
	FreeRDAvirtual(OrgAddr3);
	ShutdownSubsystems();
	exit(0);
}
static void quitprint(RDArsrc *r,RDArsrc *mainrsrc)
{
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(sortfile!=NULL)
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
		sortfile=NULL;
	}
	quitfunc(mainrsrc);
}
static void alignment(RDArsrc *r)
{
	RDA_PFILE *fp=NULL;

	readwidget(r,"OUTPUT DEVICE");
	FINDRSCGETSTRING(r,"OUTPUT DEVICE",&output_device);
	if(!isEMPTY(output_device))
	{
		fp=RDA_popen(output_device,"w");
		SelectPrintTypeByName(output_device,"PCL JOB HEADER",fp);
		SelectPrintType(output_device,0,fp);
		if(fp!=NULL)
		{
			RDA_fprintf(fp,"       XXX\r\n");
			SelectPrintTypeByName(output_device,"PCL JOB FOOTER",fp);
			RDA_pclose(fp);
		} else {
			prterr("Error:  Couldn't open output device [%s].",output_device);
		}	
	}
}
static char *decimal(double x)
{
	char *tmp=NULL;
	int y;

	switch(formdecimal)
	{
		default:
		case 0:
			sprintf(stemp1,"%.02f",x);
			break;
		case 1:
			sprintf(stemp1,"%.01f",x);
			break;
		case 2:
			sprintf(stemp1,"%.02f",x);
			y=RDAstrlen(stemp1);
			if(stemp1[y-1]=='0')
			{
				if(stemp1[y-2]=='0' && stemp1[y-3]=='.')  stemp1[y-3]=0;
					else stemp1[y-1]=0;
			}
			break;
	}
	if(!isEMPTY(stemp1)) tmp=stralloc(stemp1);
	return(tmp);
}
int PrintMICRCheckForm(int print_type,RDA_PFILE *fp,int checknumber,char *checkdate,int amount, char *empname, char *emp_add1,char *emp_add2,char *emp_add3,char *output_device)
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

		/*prterr("print_type = %d", print_type);*/
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
			/*RDA_fprintf(fp,"%c*p%dx%dY%s",27,spelled_amount_xpos,spelled_amount_ypos,spelled_amount);*/
			RDA_fprintf(fp,"\r\n");
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
		}else if((spelled_amount!=NULL)&&(print_type==3)) {
			//RDA_fprintf(fp,"%c*p%dx%dY{%s}",27,spelled_amount_xpos,spelled_amount_ypos,spelled_amount);
			RDA_fprintf(fp,"\r\n");
			SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
			SelectPrintTypeByName(output_device,"NORMAL TEXT",fp);
		}else{
			//RDA_fprintf(fp,"%c*p%dx%dY%s",27,spelled_amount_xpos,spelled_amount_ypos,spelled_amount);
			RDA_fprintf(fp,"\r\n");

		}
		if(spelled_amount!=NULL) Rfree(spelled_amount);

		//payto_xpos=510;
		memset(payto1,0,120);
		memset(payto2,0,120);
		memset(payto3,0,120);
		memset(payto4,0,120);

#ifdef WIN32
		_snprintf(payto1,120,"%s",empname);
		_snprintf(payto2,120,"%s",emp_add1);
		_snprintf(payto3,120,"%s",emp_add2);
		_snprintf(payto4,120,"%s",emp_add3);
#endif
#ifndef WIN32
		snprintf(payto1,120,"%s",empname);
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
			//RDA_fprintf(fp,"%c*p%dx%dY%06d",27,checknumber_xpos,checknumber_ypos,checknumber);
			RDA_fprintf(fp,"\r\n");
		}
		if((print_type==1)||(print_type==3))
		{
			SelectPrintTypeByName(output_device,"USE FONT ARIAL 18",fp);
			//RDA_fprintf(fp,"%c*p%dx%dY%06d",27,checknumber_xpos,checknumber_ypos,checknumber);
			RDA_fprintf(fp,"\r\n");
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
				/*RDA_fprintf(fp,"%c*p%dx%dY%s",27,spelled_amount_xpos,spelled_amount_ypos,(spelled_amount!=NULL ? spelled_amount:""));*/
				//RDA_fprintf(fp,"%c*p%dx%dY%s",27,checkdate_xpos,checkdate_ypos,(checkdate!=NULL ? checkdate:""));
				//RDA_fprintf(fp,"%c*p%dx%dY%s",27,amount_xpos,amount_ypos,(tempstr!=NULL?tempstr:""));
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"\r\n");
				RDA_fprintf(fp,"\r\n");
				break;
			case 1:
			case 2:
				//RDA_fprintf(fp,"%c*p%dx%dY%s",27,checkdate_xpos,checkdate_ypos,(checkdate!=NULL ? checkdate:""));
				RDA_fprintf(fp,"\r\n");
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
				//RDA_fprintf(fp,"%c*p%dx%dY%s",27,amount_xpos,amount_ypos,stemp);
				RDA_fprintf(fp,"\r\n");
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
				//RDA_fprintf(fp,"%c*p%dx%dY%s}",27,amount_xpos,amount_ypos,stemp);
				RDA_fprintf(fp,"\r\n");
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
		//prterr ("payto_xpos= %d, payto1_ypos= %d, payto2_ypos= %d, payto3_ypos= %d, payto4_ypos= %d",payto_xpos,payto1_ypos,payto2_ypos,payto3_ypos,payto4_ypos);
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
				if((RDAstrlen(payto1))!=04)
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
		SelectPrintTypeByName(output_device,"USE DEFAULT PRIMARY FONT",fp);
		SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);
}
static void printactualcheck(RDA_PFILE *fp)
{
	char *ssn=NULL,*empid=NULL,*loc=NULL,*empname=NULL,*sfs=NULL; /* state filing status */
	int ffs=0; /* federal filing status */
	int fed_exemption=0,state_exemption=0,state_dependents=0;
	char *temp=NULL,*spelled_amt=NULL,*temp1=NULL,*temp2=NULL,*temp3=NULL;
	char *temp4=NULL;
	char *emp_add1=NULL,*emp_add2=NULL,*emp_add3=NULL;
	double gross=0.0,adjust=0.0,regular=0.0,units=0.0,overtime=0.0;
	double overunits=0.0,ytd=0.0;
	double total_cur_gross=0.0,total_ytd=0.0,cur_gross=0.0;
	double total_dirdep=0.0,total_dirdep_ytd=0.0;
	double cur=0.0,total_cur=0.0,beg=0.0,earn=0.0,used=0.0,ending=0.0;
	double net=0.0,n=0.0,total_ecur=0.0,total_eytd=0.0;
	int checkno=0,x,y=0,skipped=0;
	char *check_date=NULL,emplr_paid=FALSE,outoforder=FALSE;
	/*int f=0,default_lines=0,new_lines=0,fill_lines=17;*/
	static short printvoid=0;


	if((amount_xpos==0)&&(amount_ypos==0))
	{
		PreloadCoordinates();
	}
	if((startup_type!=3)&&(startup_type!=6))
	{
		security_font_type=0;
	}
	SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);

	FINDFLDGETSTRING(sortfile->fileno,"LOCATION",&loc);
	FINDFLDGETINT(sortfile->fileno,"DIRECT DEPOSIT NUMBER",&checkno);
	FINDFLDGETSTRING(sortfile->fileno,"DIRECT DEPOSIT DATE",&check_date);
	FINDFLDGETSTRING(sortfile->fileno,"START DATE",&paystarting);
	FINDFLDGETSTRING(sortfile->fileno,"END DATE",&payending);
	FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDGETCHAR(sortfile->fileno,"OUT OF ORDER",&outoforder);
	FINDFLDGETSTRING(sortfile->fileno,"SOCIAL SECURITY NUMBER",&ssn);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 1",&emp_add1);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 2",&emp_add2);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 3",&emp_add3);

	for(x=0;x<dir_dep_offset;++x)
	{
		RDA_fprintf(fp,"\r\n");
	}

	RDA_fprintf(fp,"%7s%-40s       Location:%-15s %15s\r\n","",
		(XPERT_SETUP->company_name?XPERT_SETUP->company_name:""),
		(loc!=NULL?loc:""),(outoforder ? "HISTORY UPDATED":""));

	FINDFLDGETSTRING(sortfile->fileno,"EMPLOYEE NAME",&empname);
	if(RDAstrlen(empname)>25) empname[25]=0;
	RDA_fprintf(fp,"%7s%-40s       Employee Name: %-25s\r\n","",
		(OrgAddr1->value.string_value!=NULL?OrgAddr1->value.string_value:""),
		(empname!=NULL ? empname:""));


	if(display_ssn==TRUE)
	{
	RDA_fprintf(fp,"%7s%-40s       Personnel ID: %15s            SSN:  %11s\r\n","",
		(OrgAddr2->value.string_value!=NULL?OrgAddr2->value.string_value:""),
		(empid!=NULL ? empid:""),
		(ssn!=NULL ? ssn:""));
	} else {
	RDA_fprintf(fp,"%7s%-40s       Personnel ID: %15s\r\n","",
		(OrgAddr2->value.string_value!=NULL?OrgAddr2->value.string_value:""),
		(empid!=NULL ? empid:""));
	}

	RDA_fprintf(fp,"%7s%-40s       Period Start:      %-10s     Period End:   %-10s\r\n","",
		(OrgAddr3->value.string_value!=NULL? OrgAddr3->value.string_value:""),
		(paystarting!=NULL?paystarting:""),
		(payending!=NULL?payending:""));

	RDA_fprintf(fp,"%7s%-40s       Direct Dep.#:    %12d           Date:   %-10s\r\n","",
		(XPERT_SETUP->phone!=NULL ? XPERT_SETUP->phone:""),
		checkno,
		(check_date!=NULL?check_date:""));
	
	RDA_fprintf(fp,"\r\n");
	if(SHOW_FILING_STATUS==FALSE)
	{
		RDA_fprintf(fp,"\r\n");
	} else {
		temp1=Rmalloc(200+1);
		temp2=Rmalloc(200+1);
		if(federal_exemption_location == 0)
		{
			FINDFLDGETINT(sortfile->fileno,"FEDERAL FILING STATUS",&ffs);
			FINDFLDGETINT(sortfile->fileno,"FEDERAL EXEMPTION",&fed_exemption);
		    	sprintf(temp1,"%3sFederal Status: %-7s    Fed Exemptions: %.2d","",(ffs==0) ? "Married" : "Single", fed_exemption);
		}
		else
		{
		    	sprintf(temp1,"%8s                          ","");
		}
		if(state_exemption_location == 0)
		{
			FINDFLDGETSTRING(sortfile->fileno,"STATE FILING STATUS",&sfs);
			FINDFLDGETINT(sortfile->fileno,"STATE EXEMPTION",&state_exemption);
			FINDFLDGETINT(sortfile->fileno,"STATE DEPENDENTS",&state_dependents);
			sprintf(temp2,"%4sState Status: %-20s State Exemptions: %.2d %3sDependents:  %.2d", "", sfs, state_exemption, "", state_dependents);
		}
		else
		{
			sprintf(temp2,"");
		}
		RDA_fprintf(fp," %s  %s\r\n",temp1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
	}
	RDA_fprintf(fp,"\r\n");
	if(printstubform)
	{
		RDA_fprintf(fp,"\r\n");
	}else{
		RDA_fprintf(fp,"%17sJob Title                       Gross     Adjust    Regular    Units    OT Gross      OTU     Current        YTD\r\n","");
	}
	/*prterr("PRINT_NUMBER_JOBS = %d",number_jobs);*/
	for(x=0;x<number_jobs;++x)
	{
		sprintf(stemp1,"NET %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&n);
		net+=n;
		sprintf(stemp1,"DESCRIPTION %d",(x+1));
		FINDFLDGETSTRING(sortfile->fileno,stemp1,&temp);
		sprintf(stemp1,"GROSS %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&gross);
		sprintf(stemp1,"ADJUSTMENTS %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&adjust);
		sprintf(stemp1,"NUMBER UNITS %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&units);
		sprintf(stemp1,"UNIT AMOUNT %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&regular);
		sprintf(stemp1,"OVERTIME GROSS %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&overtime);
		sprintf(stemp1,"OVERTIME UNITS %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&overunits);
		sprintf(stemp1,"GROSS YTD %d",(x+1));
		FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&ytd);
		cur_gross=gross+adjust+regular+overtime;
		total_cur_gross+=cur_gross;
		total_ytd+=ytd;
		if((((cur_gross!=0.0 || ytd!=0.0)) || (!isEMPTY(temp) && show_zero_jobs)) && outoforder==FALSE)
		{
			RDA_fprintf(fp,"%3s%-40s  %9.02f  %9.02f  %9.02f  %8.02f  %9.02f  %8.02f  %9.02f  %12.02f\r\n","",
				(temp!=NULL ? temp:""),gross/100,adjust/100,
				regular/100,units,overtime/100,overunits,
				cur_gross/100,ytd/100);
		} else if((((cur_gross!=0.0 || ytd!=0.0)) || (!isEMPTY(temp) && show_zero_jobs)) && outoforder==TRUE)
		{
			RDA_fprintf(fp,"%3s%-40s  %9.02f  %9.02f  %9.02f  %8.02f  %9.02f  %8.02f  %9.02f\r\n","",
				(temp!=NULL ? temp:""),gross/100,adjust/100,
				regular/100,units,overtime/100,overunits,
				cur_gross/100);
		} else {
			++skipped;
		}
		if(temp!=NULL) Rfree(temp);
	}
	for(x=0;x<skipped;++x) RDA_fprintf(fp,"\r\n");
	/*prterr("\n After printed line=%d skipped=%d line =%d ",x,skipped,__LINE__);*/
	RDA_fprintf(fp,"%-90s Total Gross:      %9.02f  %12.02f\r\n","",
		total_cur_gross/100,total_ytd/100);
	RDA_fprintf(fp,"\r\n");

	if(printstubform)
	{
		RDA_fprintf(fp,"\r\n");
	}else{
		RDA_fprintf(fp,"   Deduction         Current         YTD      Deduction         Current         YTD      Deduction         Current         YTD\r\n");
	}
	total_cur=0.0;
	total_ytd=0.0;
	total_ecur=0.0;
	total_eytd=0.0;
	for(x=0;x<number_deductions;x+=3)
	{
		RDA_fprintf(fp,"%3s","");
		for(y=0;y<3;++y)
		{
			sprintf(stemp1,"VOL DEDUCTION TITLE %d",(x+y+1));
			FINDFLDGETSTRING(sortfile->fileno,stemp1,&temp);
			sprintf(stemp1,"VOL W/H CURRENT %d",(x+y+1));
			FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&cur);
			sprintf(stemp1,"VOL W/H YTD %d",(x+y+1));
			FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&ytd);
			if(!isEMPTY(temp) || cur!=0.0 || ytd!=0.0)
			{
				if(y!=0)
				{
					RDA_fprintf(fp," ");
				}
				if(!outoforder)
				{
					RDA_fprintf(fp,"%-13s %11.02f  %10.02f%5s",
						(temp!=NULL?temp:""),
						cur/100,ytd/100,"");
				} else {
					RDA_fprintf(fp,"%-13s %11.02f  %10s%5s",
						(temp!=NULL?temp:""),
						cur/100,"","");
				}
			}
			if(temp!=NULL) Rfree(temp);
			sprintf(stemp1,"VOL EMPLOYER PAID %d",(x+y+1));
			FINDFLDGETCHAR(sortfile->fileno,stemp1,&emplr_paid);
			if(!emplr_paid)
			{
				total_cur+=cur;
				total_ytd+=ytd;
			} else {
				total_ecur+=cur;
				total_eytd+=ytd;
			}
		}
		RDA_fprintf(fp,"\r\n");

	}
	FINDFLDGETDOUBLE(sortfile->fileno,"CURRENT DIRECT DEPOSIT",&total_dirdep);
	FINDFLDGETDOUBLE(sortfile->fileno,"YTD DIRECT DEPOSIT",&total_dirdep_ytd);
	if(print_employer_deductions==FALSE)
	{
		if(!outoforder)
		{
			RDA_fprintf(fp,"%-85sTotal Deductions: %11.02f  %10.02f\r\n","",
				(total_cur-total_dirdep)/100,(total_ytd-total_dirdep_ytd)/100);
		} else {
			RDA_fprintf(fp,"%-85sTotal Deductions: %11.02f  %10s\r\n","",
				(total_cur-total_dirdep)/100,"");
		}
	} else {
		if(!outoforder)
		{
			RDA_fprintf(fp,"%-47sEmployer:    %11.02f  %10.02f       Employee:    %11.02f  %10.02f\r\n","",total_ecur/100,total_eytd/100,(total_cur-total_dirdep)/100,(total_ytd-total_dirdep_ytd)/100);
		} else { 
			RDA_fprintf(fp,"%-47sEmployer:    %11.02f  %10s       Employee:    %11.02f  %10s\r\n","",total_ecur/100,"",(total_cur-total_dirdep)/100,"");
		}
	}
	RDA_fprintf(fp,"\r\n");

if((XPERT_SETUP->LVEMGT==TRUE || XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
{
	if(number_lvemstr==0)
	{
		RDA_fprintf(fp,"\r\n");
	} else {
		if(printstubform)
		{
			RDA_fprintf(fp,"\r\n");
		}else{
			RDA_fprintf(fp,"%3sLeave Description         Beginning  Earned    Used   Ending     Leave Description         Beginning  Earned    Used   Ending\r\n","");
		}
	}
	for(x=0;x<number_lvemstr;x+=2)
	{
		for(y=0;y<2;++y)
		{
			if(y) RDA_fprintf(fp,"  ");
			sprintf(stemp1,"LEAVE ID %d",(x+y+1));
			FINDFLDGETSTRING(sortfile->fileno,stemp1,&temp);
			sprintf(stemp1,"BEGINNING %d",(x+y+1));
			FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&beg);
			sprintf(stemp1,"EARNED %d",(x+y+1));
			FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&earn);
			sprintf(stemp1,"USED %d",(x+y+1));
			FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&used);
			sprintf(stemp1,"BALANCE %d",(x+y+1));
			FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&ending);
			RDA_fprintf(fp,"%3s","");
			if(!isEMPTY(temp) || ending!=0.0 || earn!=0.0 
				|| used!=0.0 || beg!=0.0)
			{
				temp1=decimal(beg);
				temp2=decimal(earn);
				temp3=decimal(used);
				temp4=decimal(ending);
				if(RDAstrlen(temp)>23) temp[23]=0;
				switch(formleave)
				{
					default:
					case 0:
						RDA_fprintf(fp,"%-23s %11s  %6s  %6s  %7s",
							(temp!=NULL ? temp:""),
							(temp1!=NULL ? temp1:""),
							(temp2!=NULL ? temp2:""),
							(temp3!=NULL ? temp3:""),
							(temp4!=NULL ? temp4:""));
						break;
					case 1:
						RDA_fprintf(fp,"%-23s %11s  %6s  %6s  %7s",
							(temp!=NULL ? temp:""),
							(temp1!=NULL ? temp1:""),
							(temp2!=NULL ? temp2:""),
							(temp3!=NULL ? temp3:""),
							(temp4!=NULL ? temp4:""));
						break;
				}
				if(temp1!=NULL) Rfree(temp1);
				if(temp2!=NULL) Rfree(temp2);
				if(temp3!=NULL) Rfree(temp3);
				if(temp4!=NULL) Rfree(temp4);
			}
			if(temp!=NULL) Rfree(temp);
		}
		RDA_fprintf(fp,"\r\n");
	}
} else { 
	for(x=0;x<(number_lvemstr/2)+1;++x)
	{
		RDA_fprintf(fp,"\r\n");
	}
}
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"%53sCurrent Gross: %10.02f  -  Deductions: %10.02f  =   Dir. Dep: %10.02f\r\n","",total_cur_gross/100,(total_cur-net)/100,net/100);
	RDA_fprintf(fp,"\r\n");
	for(x=1;x<PRINT_NUMBER_MESSAGE+1 && x<10;++x)
	{
		switch(x)
		{
			default:
			case 1:
				RDA_fprintf(fp,"    %-s\r\n",(message1!=NULL ? message1:""));
				break;
			case 2:
				RDA_fprintf(fp,"    %-s\r\n",(message2!=NULL ? message2:""));
				break;
			case 3:
				RDA_fprintf(fp,"    %-s\r\n",(message3!=NULL ? message3:""));
				break;
			case 4:
				RDA_fprintf(fp,"    %-s\r\n",(message4!=NULL ? message4:""));
				break;
			case 5:
				RDA_fprintf(fp,"    %-s\r\n",(message5!=NULL ? message5:""));
				break;
			case 6:
				RDA_fprintf(fp,"    %-s\r\n",(message6!=NULL ? message6:""));
				break;
			case 7:
				RDA_fprintf(fp,"    %-s\r\n",(message7!=NULL ? message7:""));
				break;
			case 8:
				RDA_fprintf(fp,"    %-s\r\n",(message8!=NULL ? message8:""));
				break;
			case 9:
				RDA_fprintf(fp,"    %-s\r\n",(message9!=NULL ? message9:""));
				break;
		}
	}

	RDA_fprintf(fp,"\r\n");
	if(net>0)
	{
		spelled_amt=spell(net);
	} else if(total_cur==0 && net==0) 
	{
		spelled_amt=spell(total_cur_gross);
	} else {
		printvoid=1;
		prterr("This is VOID.\r\n");
	}
	if(spelled_amt!=NULL) Rfree(spelled_amt);

	if( (!(net>0))  &&  (total_cur!=0 && net!=0)  )
	{
		prterr("This should be VOID.\r\n");
	}
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
	if(total_cur==0 && net==0) /* no deductions */
	{
		/* Use total_cur_gross for check amount */

		if((use_full_micr_check==0)&&(printvoid))
		{
			PrintMICRCheckForm(2,fp,checkno,check_date,total_cur_gross,empname,emp_add1,emp_add2,emp_add3,output_device); 
		}else if((use_full_micr_check==0)&&(printvoid)) 
		{
			PrintMICRCheckForm(0,fp,checkno,check_date,total_cur_gross,empname,emp_add1,emp_add2,emp_add3,output_device); 

		}else if((use_full_micr_check==1)&&(printvoid))
		{
			PrintMICRCheckForm(3,fp,checkno,check_date,total_cur_gross,empname,emp_add1,emp_add2,emp_add3,output_device); 
		}else if((use_full_micr_check==1)&&(printvoid)) 
		{
			PrintMICRCheckForm(1,fp,checkno,check_date,total_cur_gross,empname,emp_add1,emp_add2,emp_add3,output_device); 
		}
	} else {
		/* Use net for check amount */

		if((use_full_micr_check==0)&&(printvoid))
		{
			PrintMICRCheckForm(2,fp,checkno,check_date,net,empname,emp_add1,emp_add2,emp_add3,output_device); 
		}else if(use_full_micr_check==0) 
		{
			PrintMICRCheckForm(0,fp,checkno,check_date,net,empname,emp_add1,emp_add2,emp_add3,output_device); 
		}else if((use_full_micr_check==1)&&(printvoid))
		{
			PrintMICRCheckForm(3,fp,checkno,check_date,net,empname,emp_add1,emp_add2,emp_add3,output_device); 
		}else if(use_full_micr_check==1) 
		{
			PrintMICRCheckForm(1,fp,checkno,check_date,net,empname,emp_add1,emp_add2,emp_add3,output_device); 
		}
	}
	SelectPrintTypeByName(output_device,"POP CURSOR",fp);
	/*
	if((check_signature==5))
	{
		SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
		SelectPrintTypeByName(output_device,"PAYROLL SIGNATURE",fp);
		SelectPrintTypeByName(output_device,"POP CURSOR",fp);
	} else if((check_signature==4))
	{
		SelectPrintTypeByName(output_device,"PUSH CURSOR",fp);
		if(use_sign_locks)
		{
			SelectPrintTypeByName(output_device,"SIGNATURE UNLOCK",fp);
		}
		SelectPrintTypeByName(output_device,"SIGNATURE FILE",fp);
		if(use_sign_locks)
		{
			SelectPrintTypeByName(output_device,"SIGNATURE LOCK",fp);
		}
		SelectPrintTypeByName(output_device,"POP CURSOR",fp);
	}
	*/
	if(printstubform==1)
	{
		if(diagrptgen) { prterr("Using PCL Macro 3020.\n"); }
		/*prterr("Using PCL Macro 3020.\n"); TRACE;*/
		SelectPrintTypeByName(output_device,"USE_PCLMACRO_3020",fp);
	}else{
		if(diagrptgen) { prterr("Not Using PCL Macro 3020.\n"); }
	}
	RDA_fprintf(fp,"\f");

	/*
	default_lines=NUMBER_JOBS+(NUMBER_DEDUCTIONS/3)+(NUMBER_LVEMSTR/2)+NUMBER_MESSAGE;
	new_lines=number_jobs+(number_deductions/3)+(number_lvemstr/2)+PRINT_NUMBER_MESSAGE;
	if(dirdep_type==5) fill_lines=15;
	for(f=0;f<(fill_lines-(new_lines-default_lines)-dir_dep_offset);++f)
	{
		RDA_fprintf(fp,"\r\n");
	}
	if(low_address)
	{
		RDA_fprintf(fp,"\r\n");
	}
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(empname!=NULL ? empname:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add1!=NULL ? emp_add1:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add2!=NULL ? emp_add2:""));
	if(low_address)
	{
		RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add3!=NULL ? emp_add3:""));
	} else {
		RDA_fprintf(fp,"%*s%-40s\r\n\f",8+ADDRESS_OFFSET,"",(emp_add3!=NULL ? emp_add3:""));
	}
	*/

	if(ssn!=NULL) Rfree(ssn);
	if(empid!=NULL) Rfree(empid);
	if(loc!=NULL) Rfree(loc);
	if(check_date!=NULL) Rfree(check_date);
	if(emp_add1!=NULL) Rfree(emp_add1);
	if(emp_add2!=NULL) Rfree(emp_add2);
	if(emp_add3!=NULL) Rfree(emp_add3);
}
static void printchecks(RDArsrc *r,RDArsrc *mainrsrc)
{
	short ef=FALSE;
	RDA_PFILE *fp=NULL;
	/*char *output_device=NULL;*/

	readwidget(r,"OUTPUT DEVICE");
	FINDRSCGETSTRING(r,"OUTPUT DEVICE",&output_device);
	if(!isEMPTY(output_device))
	{
		fp=RDA_popen(output_device,"w");
		SelectPrintTypeByName(output_device,"PCL JOB HEADER",fp);

		if(micr_font_type==0)
		{
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_60",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",60); }
		}else if(micr_font_type==1){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_61",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",61); }
		}else if(micr_font_type==2){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_62",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",62); }
		}else if(micr_font_type==3){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_63",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",63); }
		}else if(micr_font_type==4){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_64",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",64); }
		}else if(micr_font_type==5){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_65",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",65); }
		}else if(micr_font_type==6){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_66",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",66); }
		}else if(micr_font_type==7){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_67",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",67); }
		}else if(micr_font_type==8){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_68",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",68); }
		}else if(micr_font_type==9){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_69",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",69); }
		}else if(micr_font_type==10){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_70",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",70); }
		}else if(micr_font_type==11){
			SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_71",fp);
			if(diagrptgen) { prterr("Loading Soft Font [%d].\n",71); }
		}

		switch(security_font_type)
		{
			case 1:
			case 2:
				SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_73",fp);
				if(diagrptgen) { prterr("Loading Soft Font [%d].\n",73); }
				break;
			case 3:
			case 4:
				SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_77",fp);
				if(diagrptgen) { prterr("Loading Soft Font [%d].\n",77); }
				break;
			case 5:
			case 6:
			case 7:
				SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_77",fp);
				SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_79",fp);
				if(diagrptgen) { prterr("Loading Soft Fonts [%d] and [%d].\n",77,79); }
				break;
			case 8:
			case 9:
				SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_83",fp);
				if(diagrptgen) { prterr("Loading Soft Font [%d].\n",83); }
				break;
			case 10:
			case 11:
			case 12:
				SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_83",fp);
				SelectPrintTypeByName(output_device,"LOAD_SOFTFONT_85",fp);
				if(diagrptgen) { prterr("Loading Soft Fonts [%d] and [%d].\n",83,85); }
				break;
			case 0:
			default:
				break;
		}

		if(printstubform==1)
		{
			/*prterr("Deleting PCL Macro 3020.\n"); TRACE;*/
			SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3020",fp);
			/*prterr("Loading PCL Macro 3020.\n"); TRACE;*/
			SelectPrintTypeByName(output_device,"LOAD_PCLMACRO_3020",fp);
		}
		memset(stemp,0,101);
		if(micr_check_type==0)
		{
			sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3100));
			SelectPrintTypeByName(output_device,stemp,fp);
			sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3300));
			SelectPrintTypeByName(output_device,stemp,fp);
		}else if(micr_check_type==1) {
			sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3200));
			SelectPrintTypeByName(output_device,stemp,fp);
			sprintf(stemp,"LOAD_PCLMACRO_%04d",(checkform+3400));
			SelectPrintTypeByName(output_device,stemp,fp);
		}

		SelectPrintTypeByName(output_device,"SIMPLEX",fp);
		SelectPrintTypeByName(output_device,"PORTRAIT",fp);
		SelectPrintTypeByName(output_device,"COMPRESSED TEXT",fp);
		RDA_fprintf(fp,"\f");
	} else {
		prterr("Error:  Output device not found.");
		return;
	}

	diagrsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
		"Printing Direct Deposits",NULL);
	addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(diagrsrc,module,"PAYDEDM",NULL,payded);
	addDFincvir(diagrsrc,module,"PAYDDMS",NULL,paydds);
	addDFincvir(diagrsrc,module,"PAYDTYP",NULL,paydtyp);
	if(XPERT_SETUP->LVSIMP==TRUE)
	{
	addDFincvir(diagrsrc,"LVSIMP","LVSMSTR",NULL,lvemst);
	addDFincvir(diagrsrc,"LVSIMP","LVSDSC",NULL,lvedsc);
	} else {
	addDFincvir(diagrsrc,"LVEMGT","LVEMSTR",NULL,lvemst);
	addDFincvir(diagrsrc,"LVEMGT","LVEDSC",NULL,lvedsc);
	}
	addDFincvir(diagrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(diagrsrc,module,"PAYMSTR",NULL,paymst);
	addDFincvir(diagrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(diagrsrc,module,"PAYJCSM",NULL,payjcsm);
	if(diagrsrc!=NULL)
	{
		if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(diagrsrc);
	}
	ZERNRD(sortfile->fileno);
	ef=FRSNRD(sortfile->fileno,1);
	while(!ef)
	{
		printactualcheck(fp);
		ef=NXTNRD(sortfile->fileno,1);
	}

	/*prterr("use_full_micr_check = %d", use_full_micr_check); TRACE;*/
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

		if(printstubform==1)
		{
			/*prterr("Deleting PCL Macro 3020.\n"); TRACE;*/
			SelectPrintTypeByName(output_device,"DELETE_PCLMACRO_3020",fp);
		}
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

	SelectPrintTypeByName(output_device,"PCL JOB FOOTER",fp);
	RDA_pclose(fp);
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
		diagrsrc=NULL;
	}
	quitprint(r,mainrsrc);
}
static void GatherDeductions()
{
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL,reduce_net=FALSE;
	char *code2=NULL,*typ1=NULL,bfs=FALSE,*dedtype=NULL;
	short ef=0;
	int x=0,y,cyear=0,job=0;

	ZERNRD(payjdcm);
	FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDSETSTRING(payjdcm,"PERSONNEL IDENTIFICATION",empid);
	ef=ADVGTENRDsec(payjdcm,1,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjdcm,"PERSONNEL IDENTIFICATION",&empid1);
		if(RDAstrcmp(empid,empid1)) break;
		FINDFLDGETCHAR(payjdcm,"DELETEFLAG",&deleteflag);
		FINDFLDGETINT(payjdcm,"CALENDAR YEAR",&cyear);
		FINDFLDGETINT(payjdcm,"JOB NUMBER",&job);
		if(!UpdatedAlready(empid,job))
		{
		if(!deleteflag && cyear==calyear)
		{
			COPYFIELD(payjdcm,payded,"PERSONNEL IDENTIFICATION");
			COPYFIELD(payjdcm,payded,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(ADVEQLNRDsec(payded,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payded,1);
			COPYFIELD(payded,paydds,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(ADVEQLNRDsec(paydds,1,SCRNvirtualSubData,diagrsrc))
			{
				KEYNRD(paydds,1);
			}
			FINDFLDGETCHAR(paydds,"REDUCE NET",&reduce_net);
			FINDFLDGETCHAR(paydds,"BLOCK FROM STUB",&bfs);
			FINDFLDGETSTRING(paydds,"DEDUCTION TYPE",&dedtype);
			if(bfs==FALSE)
			{
			if(reduce_net==TRUE || (reduce_net==FALSE && print_employer_deductions==TRUE))
			{
				FINDFLDGETSTRING(paydds,"DEDUCTION DESCRIPTION IDENTIFICATION",&code2);

				if(((!strncmp(dedtype,"DIRECT DEP",10)) && (DD_NET_TYPE==0)) || ((!strncmp(dedtype,"DIRECT DEPOSIT",14))))
				{
					for(y=1;y<=12;++y)
					{
						sprintf(stemp2,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"YTD DIRECT DEPOSIT",payjdcm,stemp2);
					}
				}
				for(x=0;x<number_deductions;++x)
				{
					sprintf(stemp1,"VOL DEDUCTION DESCRIPTION IDENTIFICATION %d",x+1);
					FINDFLDGETSTRING(sortfile->fileno,stemp1,&typ1);
					if(!RDAstrcmp(code2,typ1) && !isEMPTY(typ1))
					{
						sprintf(stemp1,"VOL W/H YTD %d",(x+1));
						for(y=1;y<=12;++y)
						{
							sprintf(stemp2,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,stemp1,payjdcm,stemp2);
						}
						break;
					} else if(isEMPTY(typ1))
					{
						sprintf(stemp1,"VOL DEDUCTION DESCRIPTION IDENTIFICATION %d",x+1);
						FINDFLDSETSTRING(sortfile->fileno,stemp1,code2);
						sprintf(stemp1,"VOL EMPLOYER PAID %d",(x+1));
						FINDFLDSETCHAR(sortfile->fileno,stemp1,!reduce_net);
						if(sdesc==FALSE)
						{
						sprintf(stemp1,"VOL DEDUCTION TITLE %d",(x+1));
						FINDFLDSETSTRING(sortfile->fileno,stemp1,code2);
						} else {
						sprintf(stemp1,"VOL DEDUCTION TITLE %d",(x+1));
						FIELDCOPY(paydds,"SHORT DESCRIPTION",sortfile->fileno,stemp1);
						}
						sprintf(stemp1,"VOL W/H YTD %d",(x+1));
						for(y=1;y<=12;++y)
						{
							sprintf(stemp2,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,stemp1,payjdcm,stemp2);
						}
						break;
					}
					if(typ1!=NULL) Rfree(typ1);
				}	
				if(code2!=NULL) Rfree(code2);
			}
			}
		}
		} else {
			FINDFLDSETCHAR(sortfile->fileno,"OUT OF ORDER",TRUE);
		}
		ef=ADVNXTNRDsec(payjdcm,1,SCRNvirtualSubData,diagrsrc);
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);	
	if(dedtype!=NULL) Rfree(dedtype);
}
static void GatherLeaveSimple()
{
	short ef=0,x=0;
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL,nos=FALSE,active=FALSE;
	double camt=0.0,beg=0.0,pearn=0.0,pused=0.0,earn=0.0,used=0.0;
	char *leaveid=NULL,*leaveid1=NULL;

	if(lvemst!=(-1) && lvedsc!=(-1) && XPERT_SETUP->LVSIMP==TRUE && skip_leave==FALSE)
	{
		FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
		ZERNRD(lvemst);
		FINDFLDSETSTRING(lvemst,"PERSONNEL IDENTIFICATION",empid);
		ef=ADVGTENRDsec(lvemst,1,SCRNvirtualSubData,diagrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(lvemst,"PERSONNEL IDENTIFICATION",&empid1);
			if(RDAstrcmp(empid,empid1)) break;
			FINDFLDGETCHAR(lvemst,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				COPYFIELD(lvemst,lvedsc,"LEAVE DESCRIPTION IDENTIFICATION");
				if(ADVEQLNRDsec(lvedsc,1,SCRNvirtualSubData,diagrsrc)) 
				{
					KEYNRD(lvedsc,1);
				} else {
					if(LEAVE_SHORT_DESC==TRUE)
					{
						FINDFLDGETSTRING(lvedsc,"SHORT DESCRIPTION",&leaveid);
					} else {
						FINDFLDGETSTRING(lvemst,"LEAVE DESCRIPTION IDENTIFICATION",&leaveid);
					}
					for(x=0;x<number_lvemstr;++x)
					{
						sprintf(stemp1,"LEAVE ID %d",(x+1));
						FINDFLDGETSTRING(sortfile->fileno,stemp1,&leaveid1);
						FINDFLDGETDOUBLE(lvemst,"BEGINNING BALANCE",&beg);
						FINDFLDGETDOUBLE(lvemst,"PROCESSED EARNINGS",&pearn);
						FINDFLDGETDOUBLE(lvemst,"PROCESSED USED",&pused);
						FINDFLDGETDOUBLE(lvemst,"UNPROCESSED EARNINGS",&earn);
						FINDFLDGETDOUBLE(lvemst,"UNPROCESSED USED",&used);
						FINDFLDGETCHAR(lvedsc,"NOT ON STUB",&nos);
						FINDFLDGETCHAR(lvedsc,"ACTIVE",&active);
						if(!nos && active)
						{
						if(!RDAstrcmp(leaveid,leaveid1))
						{
							sprintf(stemp1,"BEGINNING %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=beg+pearn-pused;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							
							sprintf(stemp1,"EARNED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=earn;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"USED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"BALANCE %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=beg+pearn-pused+earn-used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							break;
						} else if(isEMPTY(leaveid1))
						{
							FINDFLDSETSTRING(sortfile->fileno,stemp1,leaveid);
							sprintf(stemp1,"BEGINNING %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=beg+pearn-pused;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							
							sprintf(stemp1,"EARNED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=earn;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"USED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"BALANCE %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=beg+pearn-pused+earn-used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							break;
						}
						}
					}
					if(leaveid!=NULL) Rfree(leaveid);
					if(leaveid1!=NULL) Rfree(leaveid1);
				}
			}
			ef=ADVNXTNRDsec(lvemst,1,SCRNvirtualSubData,diagrsrc);
		}
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);
}
static void GatherLeave()
{
	short ef=0,x=0;
	char *empid1=NULL,deleteflag=FALSE,active=FALSE,*empid=NULL,nos=FALSE;
	double camt=0.0,outlve=0.0,beg=0.0,pearn=0.0,pused=0.0,earn=0.0,used=0.0,masterbal=0.0;
	char *leaveid=NULL,*leaveid1=NULL;

	if(lvemst!=(-1) && lvedsc!=(-1) && (XPERT_SETUP->LVEMGT==TRUE || XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
	{
		FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
		ZERNRD(lvemst);
		FINDFLDSETSTRING(lvemst,"PERSONNEL IDENTIFICATION",empid);
		ef=ADVGTENRDsec(lvemst,1,SCRNvirtualSubData,diagrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(lvemst,"PERSONNEL IDENTIFICATION",&empid1);
			if(RDAstrcmp(empid,empid1)) break;
			FINDFLDGETCHAR(lvemst,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(lvemst,"LEAVE DESCRIPTION IDENTIFICATION",&leaveid);
				FINDFLDSETSTRING(lvedsc,"LEAVE DESCRIPTION IDENTIFICATION",leaveid);
				if(ADVEQLNRDsec(lvedsc,1,SCRNvirtualSubData,diagrsrc)) 
				{
					KEYNRD(lvedsc,1);
				} else {
					FINDFLDGETCHAR(lvedsc,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					if(LEAVE_SHORT_DESC) FINDFLDGETSTRING(lvedsc,"SHORT DESCRIPTION",&leaveid);
					FINDFLDGETDOUBLE(lvemst,"OUTSIDE ORGANIZATION",&outlve);
					FINDFLDGETDOUBLE(lvemst,"BEGINNING BALANCE",&beg);
					FINDFLDGETDOUBLE(lvemst,"PROCESSED EARNINGS",&pearn);
					FINDFLDGETDOUBLE(lvemst,"PROCESSED USED",&pused);
					FINDFLDGETDOUBLE(lvemst,"UNPROCESSED EARNINGS",&earn);
					FINDFLDGETDOUBLE(lvemst,"UNPROCESSED USED",&used);
					FINDFLDGETCHAR(lvedsc,"NOT ON STUB",&nos);
					FINDFLDGETCHAR(lvemst,"ACTIVE",&active);
					masterbal=outlve+beg+pearn-pused+earn-used;
					if(!nos && (masterbal!=0.0 || active))
					{
					for(x=0;x<number_lvemstr;++x)
					{
						sprintf(stemp1,"LEAVE ID %d",(x+1));
						FINDFLDGETSTRING(sortfile->fileno,stemp1,&leaveid1);
						if(!RDAstrcmp(leaveid,leaveid1))
						{
							sprintf(stemp1,"BEGINNING %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=outlve+beg+pearn-pused;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							
							sprintf(stemp1,"EARNED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=earn;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"USED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"BALANCE %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=outlve+beg+pearn-pused+earn-used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							break;
						} else if(isEMPTY(leaveid1))
						{
							FINDFLDSETSTRING(sortfile->fileno,stemp1,leaveid);
							sprintf(stemp1,"BEGINNING %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=outlve+beg+pearn-pused;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							
							sprintf(stemp1,"EARNED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=earn;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"USED %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							sprintf(stemp1,"BALANCE %d",(x+1));
							FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
							camt+=outlve+beg+pearn-pused+earn-used;
							FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
							break;
						}
					}
					}
					if(leaveid1!=NULL) Rfree(leaveid1);
					} else {
						if(diagapps)
						{
							prterr("DIAG APPLICATION: Skipped empid[%s], leaveid[%s] Because NOT ON STUB set to TRUE.\r\n",empid,leaveid);
						}
					}
					if(leaveid!=NULL) Rfree(leaveid);
				}
			}
			ef=ADVNXTNRDsec(lvemst,1,SCRNvirtualSubData,diagrsrc);
		}
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);
}
static void GatherHistory()
{
	short ef=0,nopayjcsm=0;
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL;
	int jobno=0,jobno1=0,x=0,y=0;
	char *temp1=NULL;

	FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
	ZERNRD(payjmst);
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",empid);
	ef=ADVGTENRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjmst,"PERSONNEL IDENTIFICATION",&empid1);
		if(RDAstrcmp(empid,empid1)) break;
		FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
		FINDFLDGETINT(payjmst,"JOB NUMBER",&jobno);
		if(!UpdatedAlready(empid,jobno))
		{
		if(!deleteflag)
		{
			FINDFLDGETINT(payjmst,"JOB NUMBER",&jobno);
			if(diagapps)
			{
				prterr("Personnel ID: %s Job Number: %d \r\n",empid,jobno);
			}
			ZERNRD(payjcsm);
			COPYFIELD(payjmst,payjcsm,"PERSONNEL IDENTIFICATION");
			COPYFIELD(payjmst,payjcsm,"JOB NUMBER");
			FINDFLDSETINT(payjcsm,"CALENDAR YEAR",calyear);
			nopayjcsm=(ADVEQLNRDsec(payjcsm,1,SCRNvirtualSubData,diagrsrc));
			if(nopayjcsm) KEYNRD(payjcsm,1);
			if(!nopayjcsm || show_zero_jobs)
			{
			COPYFIELD(payjmst,posmst,"POSITION IDENTIFICATION");
			if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posmst,1);
			for(x=0;x<number_jobs;++x)
			{
				sprintf(stemp1,"JOB NUMBER %d",x+1);
				FINDFLDGETINT(sortfile->fileno,stemp1,&jobno1);
				if(jobno==jobno1)
				{
					sprintf(stemp1,"GROSS YTD %d",(x+1));
					for(y=1;y<=12;++y)
					{
						sprintf(stemp2,"%s GROSS",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,stemp1,payjcsm,stemp2);
					}
					break;
				} else if(jobno1==0)
				{
					FINDFLDSETINT(sortfile->fileno,stemp1,jobno);
					switch(jdtype)
					{
						default:
						case 0:
							FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&temp1);
							break;
						case 1:
							FINDFLDGETSTRING(posmst,"DESCRIPTION",&temp1);
							break;
						case 2:
							FINDFLDGETSTRING(payjmst,"DESCRIPTION",&temp1);
							break;
					}
					sprintf(stemp1,"DESCRIPTION %d",(x+1));
					FINDFLDSETSTRING(sortfile->fileno,stemp1,temp1);


					if(temp1!=NULL) Rfree(temp1);
					sprintf(stemp1,"GROSS YTD %d",(x+1));
					for(y=1;y<=12;++y)
					{
						sprintf(stemp2,"%s GROSS",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,stemp1,payjcsm,stemp2);
					}
					break;
				} else if(x==(number_jobs-1))
				{
					sprintf(stemp1,"GROSS YTD %d",(x+1));
					for(y=1;y<=12;++y)
					{
						sprintf(stemp2,"%s GROSS",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,stemp1,payjcsm,stemp2);
					}
					break;
				}
			}
			}
		}
		} else {
			FINDFLDSETCHAR(sortfile->fileno,"OUT OF ORDER",TRUE);
		}
		ef=ADVNXTNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc);
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);
}
static void GatherCurrentDeductions(char *empid,char *payid,int jobno)
{
	char *empid1=NULL,deleteflag=FALSE,reduce_net=FALSE;
	char *payid1=NULL,*typ1=NULL,*code2=NULL,bfs=FALSE,*dedtype=NULL;
	short ef=0;
	int jobno1=0,x=0;

	ZERNRD(payjdpm);
	FINDFLDSETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",empid);
	FINDFLDSETSTRING(payjdpm,"PAY IDENTIFICATION",payid);
	FINDFLDSETINT(payjdpm,"JOB NUMBER",jobno);
	ef=ADVGTENRDsec(payjdpm,1,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETSTRING(payjdpm,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETINT(payjdpm,"JOB NUMBER",&jobno1);
		if(RDAstrcmp(empid,empid1) || RDAstrcmp(payid,payid1) || jobno!=jobno1) break;
		FINDFLDGETCHAR(payjdpm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			ZERNRD(payded);
			COPYFIELD(payjdpm,payded,"PERSONNEL IDENTIFICATION");
			COPYFIELD(payjdpm,payded,"DEDUCTION IDENTIFICATION");
			if(ADVEQLNRDsec(payded,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payded,1);	
			
			COPYFIELD(payjdpm,paydds,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(ADVEQLNRDsec(paydds,1,SCRNvirtualSubData,diagrsrc))
			{
				KEYNRD(paydds,1);
			}
			FINDFLDGETCHAR(paydds,"REDUCE NET",&reduce_net);
			FINDFLDGETCHAR(paydds,"BLOCK FROM STUB",&bfs);
			FINDFLDGETSTRING(paydds,"DEDUCTION TYPE",&dedtype);
			if(bfs==FALSE)
			{
			if(reduce_net==TRUE || (reduce_net==FALSE && print_employer_deductions==TRUE))
			{
				FINDFLDGETSTRING(paydds,"DEDUCTION DESCRIPTION IDENTIFICATION",&code2);

				if(((!strncmp(dedtype,"DIRECT DEP",10)) && (DD_NET_TYPE==0)) || ((!strncmp(dedtype,"DIRECT DEPOSIT",14))))
				{
					QAccFld(sortfile->fileno,"CURRENT DIRECT DEPOSIT",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"YTD DIRECT DEPOSIT",payjdpm,"AMOUNT");
				}
				for(x=0;x<number_deductions;++x)
				{
					sprintf(stemp1,"VOL DEDUCTION DESCRIPTION IDENTIFICATION %d",x+1);
					FINDFLDGETSTRING(sortfile->fileno,stemp1,&typ1);
					if(!RDAstrcmp(code2,typ1) && !isEMPTY(typ1)) 
					{
						sprintf(stemp1,"VOL W/H CURRENT %d",(x+1));
						QAccFld(sortfile->fileno,stemp1,payjdpm,"AMOUNT");
						
						sprintf(stemp1,"VOL W/H YTD %d",(x+1));
						QAccFld(sortfile->fileno,stemp1,payjdpm,"AMOUNT");
						break;
					} else if(isEMPTY(typ1))
					{
						sprintf(stemp1,"VOL DEDUCTION DESCRIPTION IDENTIFICATION %d",x+1);
						FINDFLDSETSTRING(sortfile->fileno,stemp1,code2);
						sprintf(stemp1,"VOL EMPLOYER PAID %d",(x+1));
						FINDFLDSETCHAR(sortfile->fileno,stemp1,!reduce_net);
						if(sdesc==FALSE)
						{
						sprintf(stemp1,"VOL DEDUCTION TITLE %d",(x+1));
						FINDFLDSETSTRING(sortfile->fileno,stemp1,code2);
						} else {
						sprintf(stemp1,"VOL DEDUCTION TITLE %d",(x+1));
						FIELDCOPY(paydds,"SHORT DESCRIPTION",sortfile->fileno,stemp1);
						}
						sprintf(stemp1,"VOL W/H CURRENT %d",(x+1));
						QAccFld(sortfile->fileno,stemp1,payjdpm,"AMOUNT");
						sprintf(stemp1,"VOL W/H YTD %d",(x+1));
						QAccFld(sortfile->fileno,stemp1,payjdpm,"AMOUNT");
						break;
					}
					if(typ1!=NULL) Rfree(typ1);
				}	
				if(code2!=NULL) Rfree(code2);
			}
			}
		}
		ef=ADVNXTNRDsec(payjdpm,1,SCRNvirtualSubData,diagrsrc);
	}
	if(empid1!=NULL) Rfree(empid1);	
	if(payid1!=NULL) Rfree(payid1);
	if(dedtype!=NULL) Rfree(dedtype);
}
static CheckRate *GatherCurrentCheckRate(char *empid,char *payid,int jobno)
{
	CheckRate *C=NULL;
	short ef=0;
	char *empid1=NULL,*payid1=NULL,delflag=FALSE,*rate=NULL;
	int jobno1=0;
	double gross=0.0,units=0.0;

	C=Rmalloc(sizeof(CheckRate));
	ZERNRD(payjprm);
	FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",empid);
	FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
	FINDFLDSETINT(payjprm,"JOB NUMBER",jobno);
	ef=ADVGTENRDsec(payjprm,2,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjprm,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETSTRING(payjprm,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETINT(payjprm,"JOB NUMBER",&jobno1);
		if(RDAstrcmp(empid,empid1) || RDAstrcmp(payid,payid1) || jobno!=jobno1) break;
		FINDFLDGETCHAR(payjprm,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(payjprm,"RATE IDENTIFICATION",&rate);
			FINDFLDGETDOUBLE(payjprm,"GROSS",&gross);
			FINDFLDGETDOUBLE(payjprm,"UNITS",&units);
			if(!RDAstrcmp(rate,"SALARY") || !RDAstrcmp(rate,"EXTENDED DAY") || !RDAstrcmp(rate,"EXTENDED YEAR") || !RDAstrcmp(rate,"EXTENDED YEARNI") || RDAstrstr(rate,"SUPPL"))		
			{
				C->salary+=gross;
			} else if(RDAstrstr(rate,"HOURLY") || RDAstrstr(rate,"DAILY") || RDAstrstr(rate,"UNIT") || RDAstrstr(rate,"MEETING") || !RDAstrcmp(rate,"MISC SALARIES")) /* Unit Rates */
			{
				C->units+=units;
				C->unit_amt+=gross;
			} else if(!strncmp(rate,"OVERTIME",8)) /* Overtime Rates */
			{
				C->overs+=units;
				C->over_amt+=gross;
			} else { /* Numerous Adjustment Type Rates */
				C->adjust+=gross;
			}
			C->current+=gross;
		}
		if(rate!=NULL) Rfree(rate);
		ef=ADVNXTNRDsec(payjprm,2,SCRNvirtualSubData,diagrsrc);
	}	
	if(payid1!=NULL) Rfree(payid1);
	if(empid1!=NULL) Rfree(empid1);	
	return(C);
}
static short AtLeastOneJob(char *empid,RangeScreen *rs,RDArsrc *mainrsrc)
{
	short retval=FALSE,ef=FALSE;
	char *empid1=NULL,deleteflag=FALSE;
	int checkno=0,jobno=0,jobno1=0,x=0;
	char *temp1=NULL,*bankno1=NULL,*payid1=NULL;
	short doit=FALSE;
	double camt=0.0,net=0.0;
	RDAvirtual *v=NULL;
	CheckRate *C=NULL;

	ZERNRD(payjpms);
	FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",empid);	
	FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(payjpms,2,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		if(diagapps)
		{
			SEENRDRECORD(payjpms);TRACE;
		}
		FINDFLDGETSTRING(payjpms,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETSTRING(payjpms,"PAY IDENTIFICATION",&payid1);
		if(diagapps)
		{
			prterr("empid[%s]==empid1[%s] || payid[%s]==payid1[%s]\r\n",empid,empid1,payid,payid1);
		}
		if(RDAstrcmp(empid,empid1) || RDAstrcmp(payid,payid1)) break;
		FINDFLDGETCHAR(payjpms,"DELETEFLAG",&deleteflag);
		FINDFLDGETSTRING(payjpms,"BANK IDENTIFICATION",&bankno1);
		FINDFLDGETINT(payjpms,"DIRECT DEPOSIT NUMBER",&checkno);
		ZERNRD(payjmst);
		COPYFIELD(payjpms,payjmst,"PERSONNEL IDENTIFICATION");
		COPYFIELD(payjpms,payjmst,"JOB NUMBER");
		if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payjmst,1);
		if(diagapps)
		{
			SEENRDRECORD(payjmst);TRACE;
		}
		ZERNRD(posgrs);
		COPYFIELD(payjmst,posmst,"POSITION IDENTIFICATION");
		if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posmst,1);
		if(diagapps)
		{
			SEENRDRECORD(posmst);TRACE;
		}
		FINDFLDGETDOUBLE(payjpms,"DIRECT DEPOSIT NET",&net);
		/* uncompute virtual fields */
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(!deleteflag && !testfilerangersrc(payjmst,rs) &&
			!testfilerangersrc(payjpms,rs) && 
			checkno!=0 && !RDAstrcmp(bankno,bankno1))
		{
			ZERNRD(sortfile->fileno);
			FINDFLDSETINT(sortfile->fileno,"DIRECT DEPOSIT NUMBER",checkno);
			if(EQLNRD(sortfile->fileno,1)) 
			{
				KEYNRD(sortfile->fileno,1);
				FIELDCOPY(perdmg,"LOCATION IDENTIFICATION",sortfile->fileno,"LOCATION");
				COPYFIELD(perdmg,sortfile->fileno,"SOCIAL SECURITY NUMBER");
				COPYFIELD(perdmg,sortfile->fileno,"FEDERAL FILING STATUS");
				COPYFIELD(perdmg,sortfile->fileno,"FEDERAL EXEMPTION");
				COPYFIELD(perdmg,sortfile->fileno,"STATE FILING STATUS");
				COPYFIELD(perdmg,sortfile->fileno,"STATE EXEMPTION");
				COPYFIELD(perdmg,sortfile->fileno,"STATE DEPENDENTS");
				if(EmployeeName!=NULL)
				{
					EmployeeName->computed=FALSE;
					computevirtual(EmployeeName,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"EMPLOYEE NAME",EmployeeName->value.string_value);
				}
				if(EmployeeAddr1!=NULL)
				{
					EmployeeAddr1->computed=FALSE;
					computevirtual(EmployeeAddr1,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"ADDRESS 1",EmployeeAddr1->value.string_value);
				} 
				if(EmployeeAddr2!=NULL)
				{
					EmployeeAddr2->computed=FALSE;
					computevirtual(EmployeeAddr2,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"ADDRESS 2",EmployeeAddr2->value.string_value);
				}
				if(EmployeeAddr3!=NULL)
				{
					EmployeeAddr3->computed=FALSE;
					computevirtual(EmployeeAddr3,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"ADDRESS 3",EmployeeAddr3->value.string_value);
				}
			} 
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
				COPYFIELD(payjpms,sortfile->fileno,"DIRECT DEPOSIT DATE");
				COPYFIELD(paymst,sortfile->fileno,"START DATE");
				COPYFIELD(paymst,sortfile->fileno,"END DATE");
				COPYFIELD(payjpms,sortfile->fileno,"PERSONNEL IDENTIFICATION");
				FINDFLDGETINT(payjpms,"JOB NUMBER",&jobno);
				if(diagapps)
				{
					SEENRDRECORD(payjpms);TRACE;
				}
				C=GatherCurrentCheckRate(empid,payid,jobno);
				if(C!=NULL)
				{
				for(x=0;x<number_jobs;++x)
				{
					sprintf(stemp1,"JOB NUMBER %d",x+1);
					FINDFLDGETINT(sortfile->fileno,stemp1,&jobno1);
					if(jobno1==0)
					{
						FINDFLDSETINT(sortfile->fileno,stemp1,jobno);
						switch(jdtype)
						{
							default:
							case 0:
								FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&temp1);
								break;
							case 1:
								FINDFLDGETSTRING(posmst,"DESCRIPTION",&temp1);
								break;
							case 2:
								FINDFLDGETSTRING(payjmst,"DESCRIPTION",&temp1);
								break;
						}
						sprintf(stemp1,"DESCRIPTION %d",(x+1));
						FINDFLDSETSTRING(sortfile->fileno,stemp1,temp1);
						if(temp1!=NULL) Rfree(temp1);
						sprintf(stemp1,"GROSS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->salary;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"ADJUSTMENTS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->adjust;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"NUMBER UNITS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->units;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"UNIT AMOUNT %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->unit_amt;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"OVERTIME GROSS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->over_amt;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"OVERTIME UNITS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->overs;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);

						sprintf(stemp1,"GROSS YTD %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->salary+C->unit_amt+C->over_amt+C->adjust;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"NET %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=net;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						break;
							
					} else if(x==(number_jobs-1))
					{
						sprintf(stemp1,"GROSS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->salary;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"ADJUSTMENTS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->adjust;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"NUMBER UNITS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->units;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"UNIT AMOUNT %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->unit_amt;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"OVERTIME GROSS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->over_amt;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"OVERTIME UNITS %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->overs;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"NET %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=net;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
						sprintf(stemp1,"GROSS YTD %d",(x+1));
						FINDFLDGETDOUBLE(sortfile->fileno,stemp1,&camt);
						camt+=C->salary+C->unit_amt+C->over_amt+C->adjust;
						FINDFLDSETDOUBLE(sortfile->fileno,stemp1,camt);
					}
				}	
				if(C!=NULL) Rfree(C);
				GatherCurrentDeductions(empid,payid,jobno);
				}
				WRTNRD(sortfile->fileno,0,NULL);
				retval=TRUE;
			}
		}
		if(bankno1!=NULL) Rfree(bankno1);
		ef=ADVNXTNRDsec(payjpms,2,SCRNvirtualSubData,diagrsrc);
	}
	if(empid1!=NULL) Rfree(empid1);
	return(retval);
}
static void CreateSortFile(RangeScreen *rs,RDArsrc *mainrsrc)
{
	short ef=0,diag_update=FALSE;
	char *empid=NULL,deleteflag=FALSE,computed=FALSE;
	char *payid1=NULL;
	short doit=FALSE;
	RDAvirtual *v=NULL;
	int x=0;


	diagrsrc=diagscrn(payppms,"DIAGNOSTIC SCREEN",module,
		"Selecting Direct Deposits",NULL);
	addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(diagrsrc,module,"POSGRS",NULL,posgrs);
	addDFincvir(diagrsrc,module,"PAYMSTR",NULL,paymst);
	addDFincvir(diagrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(diagrsrc,module,"PAYJPRM",NULL,payjpms);
	addDFincvir(diagrsrc,module,"PAYJCSM",NULL,payjcsm);
	if(diagrsrc!=NULL)
	{
		if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(diagrsrc);
	}
	ZERNRD(payppms);
	FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(payppms,2,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		diag_update=FALSE;
		FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(payppms,"BENEFITS COMPUTED",&computed);
		FINDFLDGETSTRING(payppms,"PAY IDENTIFICATION",&payid1);
		COPYFIELD(payppms,perdmg,"PERSONNEL IDENTIFICATION");
		if(RDAstrcmp(payid,payid1)) break;
		if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(perdmg,1);
		/* uncompute virtual fields */
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
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
					DID_STUFF=TRUE;
				}
			}
		}
		if(diagrsrc!=NULL) 
			update_diagnostic(diagrsrc,
				(diag_update==TRUE ? TRUE:FALSE));
		ef=ADVNXTNRDsec(payppms,2,SCRNvirtualSubData,diagrsrc);
	}
	if(empid!=NULL) Rfree(empid);
	if(payid1!=NULL) Rfree(payid1);
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
		diagrsrc=NULL;
	}
}

static void selectchecks(RDArsrc *r)
{
	char *sortname=NULL,*temp1=NULL,*defaultprinter=NULL;
	RangeScreen *rs=NULL;
	short ef=FALSE;
	DFvirtual *d=NULL;
	RDAvirtual *v=NULL;
	RDArsrc *rsrc=NULL;
	int x=0;

	readallwidgets(r);
	rs=RangeScreenNew(r);
	ReadRangeScreen(perdmg,rs);
	ReadRangeScreen(posmst,rs);
	ReadRangeScreen(payjmst,rs);
	ReadRangeScreen(payjpms,rs);
	ReadRangeScreen(payppms,rs);
	if(r->virflds!=NULL)
	{
		for(x=0,v=r->virflds;x<r->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}

	d=getDFvirtual("PAYROLL","NAME FIRST MIDDLE LAST LINEAGE");
	if(d!=NULL)
	{
		EmployeeName=Rmalloc(sizeof(RDAvirtual));
		EmployeeName->name=stralloc(d->name);
		EmployeeName->when=0;
		EmployeeName->len=d->length;
		EmployeeName->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeName->expression=PP_translate(d->expression);
		} else {
			EmployeeName->expression=NULL;
		}
		EmployeeName->computed=FALSE;
		EmployeeName->range=FALSE;
		EmployeeName->dlen=0;
		switch(EmployeeName->type)
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
				EmployeeName->value.string_value=NULL;
				EmployeeName->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeName->value.string_value=Rmalloc(1);
				*EmployeeName->value.string_value=0;
				EmployeeName->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeName->value.float_value=Rmalloc(sizeof(double));
				*EmployeeName->value.float_value=0;
				EmployeeName->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeName->value.short_value=Rmalloc(sizeof(short));
				*EmployeeName->value.short_value=0;
				EmployeeName->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeName->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeName->value.integer_value=0;
				EmployeeName->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeName->type,EmployeeName->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","PERSONNEL MAILING ADDRESS 1");
	if(d!=NULL)
	{
		EmployeeAddr1=Rmalloc(sizeof(RDAvirtual));
		EmployeeAddr1->name=stralloc(d->name);
		EmployeeAddr1->when=0;
		EmployeeAddr1->len=d->length;
		EmployeeAddr1->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeAddr1->expression=PP_translate(d->expression);
		} else {
			EmployeeAddr1->expression=NULL;
		}
		EmployeeAddr1->computed=FALSE;
		EmployeeAddr1->range=FALSE;
		EmployeeAddr1->dlen=0;
		switch(EmployeeAddr1->type)
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
				EmployeeAddr1->value.string_value=NULL;
				EmployeeAddr1->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeAddr1->value.string_value=Rmalloc(1);
				*EmployeeAddr1->value.string_value=0;
				EmployeeAddr1->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeAddr1->value.float_value=Rmalloc(sizeof(double));
				*EmployeeAddr1->value.float_value=0;
				EmployeeAddr1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeAddr1->value.short_value=Rmalloc(sizeof(short));
				*EmployeeAddr1->value.short_value=0;
				EmployeeAddr1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeAddr1->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeAddr1->value.integer_value=0;
				EmployeeAddr1->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeAddr1->type,EmployeeAddr1->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","PERSONNEL MAILING ADDRESS 2");
	if(d!=NULL)
	{
		EmployeeAddr2=Rmalloc(sizeof(RDAvirtual));
		EmployeeAddr2->name=stralloc(d->name);
		EmployeeAddr2->when=0;
		EmployeeAddr2->len=d->length;
		EmployeeAddr2->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeAddr2->expression=PP_translate(d->expression);
		} else {
			EmployeeAddr2->expression=NULL;
		}
		EmployeeAddr2->computed=FALSE;
		EmployeeAddr2->range=FALSE;
		EmployeeAddr2->dlen=0;
		switch(EmployeeAddr2->type)
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
				EmployeeAddr2->value.string_value=NULL;
				EmployeeAddr2->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeAddr2->value.string_value=Rmalloc(1);
				*EmployeeAddr2->value.string_value=0;
				EmployeeAddr2->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeAddr2->value.float_value=Rmalloc(sizeof(double));
				*EmployeeAddr2->value.float_value=0;
				EmployeeAddr2->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeAddr2->value.short_value=Rmalloc(sizeof(short));
				*EmployeeAddr2->value.short_value=0;
				EmployeeAddr2->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeAddr2->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeAddr2->value.integer_value=0;
				EmployeeAddr2->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeAddr2->type,EmployeeAddr2->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","PERSONNEL MAILING ADDRESS 3");
	if(d!=NULL)
	{
		EmployeeAddr3=Rmalloc(sizeof(RDAvirtual));
		EmployeeAddr3->name=stralloc(d->name);
		EmployeeAddr3->when=0;
		EmployeeAddr3->len=d->length;
		EmployeeAddr3->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeAddr3->expression=PP_translate(d->expression);
		} else {
			EmployeeAddr3->expression=NULL;
		}
		EmployeeAddr3->computed=FALSE;
		EmployeeAddr3->range=FALSE;
		EmployeeAddr3->dlen=0;
		switch(EmployeeAddr3->type)
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
				EmployeeAddr3->value.string_value=NULL;
				EmployeeAddr3->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeAddr3->value.string_value=Rmalloc(1);
				*EmployeeAddr3->value.string_value=0;
				EmployeeAddr3->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeAddr3->value.float_value=Rmalloc(sizeof(double));
				*EmployeeAddr3->value.float_value=0;
				EmployeeAddr3->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeAddr3->value.short_value=Rmalloc(sizeof(short));
				*EmployeeAddr3->value.short_value=0;
				EmployeeAddr3->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeAddr3->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeAddr3->value.integer_value=0;
				EmployeeAddr3->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeAddr3->type,EmployeeAddr3->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","ORGANIZATIONAL ADDRESS 1");
	if(d!=NULL)
	{
		OrgAddr1=Rmalloc(sizeof(RDAvirtual));
		OrgAddr1->name=stralloc(d->name);
		OrgAddr1->when=0;
		OrgAddr1->len=d->length;
		OrgAddr1->type=d->type;
		if(d->expression!=NULL) 
		{
			OrgAddr1->expression=PP_translate(d->expression);
		} else {
			OrgAddr1->expression=NULL;
		}
		OrgAddr1->computed=FALSE;
		OrgAddr1->range=FALSE;
		OrgAddr1->dlen=0;
		switch(OrgAddr1->type)
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
				OrgAddr1->value.string_value=NULL;
				OrgAddr1->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				OrgAddr1->value.string_value=Rmalloc(1);
				*OrgAddr1->value.string_value=0;
				OrgAddr1->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				OrgAddr1->value.float_value=Rmalloc(sizeof(double));
				*OrgAddr1->value.float_value=0;
				OrgAddr1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				OrgAddr1->value.short_value=Rmalloc(sizeof(short));
				*OrgAddr1->value.short_value=0;
				OrgAddr1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				OrgAddr1->value.integer_value=Rmalloc(sizeof(int));
				*OrgAddr1->value.integer_value=0;
				OrgAddr1->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",OrgAddr1->type,OrgAddr1->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","ORGANIZATIONAL ADDRESS 2");
	if(d!=NULL)
	{
		OrgAddr2=Rmalloc(sizeof(RDAvirtual));
		OrgAddr2->name=stralloc(d->name);
		OrgAddr2->when=0;
		OrgAddr2->len=d->length;
		OrgAddr2->type=d->type;
		if(d->expression!=NULL) 
		{
			OrgAddr2->expression=PP_translate(d->expression);
		} else {
			OrgAddr2->expression=NULL;
		}
		OrgAddr2->computed=FALSE;
		OrgAddr2->range=FALSE;
		OrgAddr2->dlen=0;
		switch(OrgAddr2->type)
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
				OrgAddr2->value.string_value=NULL;
				OrgAddr2->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				OrgAddr2->value.string_value=Rmalloc(1);
				*OrgAddr2->value.string_value=0;
				OrgAddr2->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				OrgAddr2->value.float_value=Rmalloc(sizeof(double));
				*OrgAddr2->value.float_value=0;
				OrgAddr2->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				OrgAddr2->value.short_value=Rmalloc(sizeof(short));
				*OrgAddr2->value.short_value=0;
				OrgAddr2->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				OrgAddr2->value.integer_value=Rmalloc(sizeof(int));
				*OrgAddr2->value.integer_value=0;
				OrgAddr2->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",OrgAddr2->type,OrgAddr2->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","ORGANIZATIONAL ADDRESS 3");
	if(d!=NULL)
	{
		OrgAddr3=Rmalloc(sizeof(RDAvirtual));
		OrgAddr3->name=stralloc(d->name);
		OrgAddr3->when=0;
		OrgAddr3->len=d->length;
		OrgAddr3->type=d->type;
		if(d->expression!=NULL) 
		{
			OrgAddr3->expression=PP_translate(d->expression);
		} else {
			OrgAddr3->expression=NULL;
		}
		OrgAddr3->computed=FALSE;
		OrgAddr3->range=FALSE;
		OrgAddr3->dlen=0;
		switch(OrgAddr3->type)
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
				OrgAddr3->value.string_value=NULL;
				OrgAddr3->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				OrgAddr3->value.string_value=Rmalloc(1);
				*OrgAddr3->value.string_value=0;
				OrgAddr3->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				OrgAddr3->value.float_value=Rmalloc(sizeof(double));
				*OrgAddr3->value.float_value=0;
				OrgAddr3->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				OrgAddr3->value.short_value=Rmalloc(sizeof(short));
				*OrgAddr3->value.short_value=0;
				OrgAddr3->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				OrgAddr3->value.integer_value=Rmalloc(sizeof(int));
				*OrgAddr3->value.integer_value=0;
				OrgAddr3->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",OrgAddr3->type,OrgAddr3->name);
				break;
		}
		FreeDFvirtual(d);
	}
	sortname=unique_name();
	sortfile=DBsortNEW("PAYROLL",sortname,NRDRdaEngine);
	addDBfield(sortfile,"DELETEFLAG",BOOLNS,1);
	addDBfield(sortfile,"DIRECT DEPOSIT NUMBER",LONGV,8);
	addDBfield(sortfile,"DIRECT DEPOSIT DATE",DATES,10);
	addDBfield(sortfile,"START DATE",DATES,10);
	addDBfield(sortfile,"END DATE",DATES,10);
	addDBfield(sortfile,"LOCATION",PLAINTEXT,15);

/* Personnel Fields */
	addDBfield(sortfile,"PERSONNEL IDENTIFICATION",PLAINTEXT,15);
	addDBfield(sortfile,"SOCIAL SECURITY NUMBER",SOCSECNUM,11);
	addDBfield(sortfile,"EMPLOYEE NAME",1,40);
	addDBfield(sortfile,"OUT OF ORDER",10,1);
	addDBfield(sortfile,"ADDRESS 1",1,40);
	addDBfield(sortfile,"ADDRESS 2",1,40);
	addDBfield(sortfile,"ADDRESS 3",1,40);
	addDBfield(sortfile,"FEDERAL FILING STATUS",8,1);
	addDBfield(sortfile,"FEDERAL EXEMPTION",7,2);
	addDBfield(sortfile,"STATE FILING STATUS",1,40);
	addDBfield(sortfile,"STATE EXEMPTION",7,2);
	addDBfield(sortfile,"STATE DEPENDENTS",7,2);

/* Job Fields */
	for(x=0;x<number_jobs;++x)
	{
		sprintf(stemp1,"JOB NUMBER %d",(x+1));
		addDBfield(sortfile,stemp1,LONGV,4);
		sprintf(stemp1,"DESCRIPTION %d",(x+1));
		addDBfield(sortfile,stemp1,1,30);
		sprintf(stemp1,"GROSS %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"ADJUSTMENTS %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"NUMBER UNITS %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"UNIT AMOUNT %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"OVERTIME GROSS %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"OVERTIME UNITS %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"GROSS YTD %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"NET %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
	}

/* Deduction Fields */
	for(x=0;x<number_deductions;++x)
	{
		sprintf(stemp1,"VOL DEDUCTION DESCRIPTION IDENTIFICATION %d",(x+1));
		addDBfield(sortfile,stemp1,1,30);
		sprintf(stemp1,"VOL DEDUCTION TITLE %d",(x+1));
		addDBfield(sortfile,stemp1,1,12);
		sprintf(stemp1,"VOL W/H CURRENT %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"VOL W/H YTD %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"VOL EMPLOYER PAID %d",(x+1));
		addDBfield(sortfile,stemp1,BOOLNS,1);
	}

/* leave fields */
	for(x=0;x<number_lvemstr;++x)
	{
		sprintf(stemp1,"LEAVE ID %d",(x+1));
		addDBfield(sortfile,stemp1,1,24);	
		sprintf(stemp1,"BEGINNING %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"EARNED %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"USED %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"BALANCE %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
	}
	addDBfield(sortfile,"CURRENT DIRECT DEPOSIT",2,20);
	addDBfield(sortfile,"YTD DIRECT DEPOSIT",2,20);

/* Key Info */
	addDBkey(sortfile,"PRINT CHECKS KEY");
	addDBkeypart(sortfile,1,"DIRECT DEPOSIT NUMBER");
	sortfile->fileno=OPNDBsort(sortfile);
	if(sortfile->fileno==(-1))
	{
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(6+7+77+1+RDAstrlen(sortfile->filename));
		sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [PAYROLL] Filename [%s], Can Not Save Database File!",sortfile->filename);
		prterr(temp1);
		return;
	}

	FINDRSCGETSTRING(r,"[BNKACC][BANK IDENTIFICATION]",&bankno);
	/* Set MICR Values */
	FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankno);
	if(EQLNRD(bnkacc,1)) KEYNRD(bnkacc,1);

	FINDFLDGETSTRING(bnkacc,"CHECK MICR",&micrtext);
	myptr=micrtext;
	memset(micr_xval,0,66);
	for(x=65;x>0;x--)
	{
		micr_xval[x]=*myptr;
		++myptr;
	}
	
	FINDFLDGETSHORT(bnkacc,"CHECK TYPE",&micr_check_type);
	FINDFLDGETSHORT(bnkacc,"MICR MIDDLE YPOS",&micr_middle_ypos);
	FINDFLDGETSHORT(bnkacc,"MICR BOTTOM YPOS",&micr_bottom_ypos);

	FINDFLDGETSHORT(bnkacc,"CHECK FORM",&checkform);
	FINDFLDGETSHORT(bnkacc,"CHECK DATE XPOS",&checkdate_xpos);
	FINDFLDGETSHORT(bnkacc,"CHECK DATE YPOS",&checkdate_ypos);
	FINDFLDGETSHORT(bnkacc,"CHECK NUMBER XPOS",&checknumber_xpos);
	FINDFLDGETSHORT(bnkacc,"CHECK NUMBER YPOS",&checknumber_ypos);
	/*
	FINDFLDGETSHORT(bnkacc,"AMOUNT XPOS",&amount_xpos);
	FINDFLDGETSHORT(bnkacc,"AMOUNT YPOS",&amount_ypos);
	*/
	FINDFLDGETSHORT(bnkacc,"SPELLED AMOUNT XPOS",&spelled_amount_xpos);
	FINDFLDGETSHORT(bnkacc,"SPELLED AMOUNT YPOS",&spelled_amount_ypos);
	FINDFLDGETSHORT(bnkacc,"PAY TO XPOS",&payto_xpos);
	FINDFLDGETSHORT(bnkacc,"MICR FONT TYPE",&micr_font_type);
	FINDFLDGETSHORT(bnkacc,"SECURITY FONT TYPE",&security_font_type);
	FINDFLDGETSHORT(bnkacc,"PAY TO 1 YPOS",&payto1_ypos);
	FINDFLDGETSHORT(bnkacc,"PAY TO 2 YPOS",&payto2_ypos);
	FINDFLDGETSHORT(bnkacc,"PAY TO 3 YPOS",&payto3_ypos);
	FINDFLDGETSHORT(bnkacc,"PAY TO 4 YPOS",&payto4_ypos);
	myptr=NULL;

	 /*
	 prterr("\nmicr_check_type = %d\n micr_bottom_ypos =%d\n micr_middle_ypos = %d\n (strlen(micrtext) = %d\n dirdep_type =%d\n",micr_check_type,micr_bottom_ypos,micr_middle_ypos,(strlen(micrtext)),dirdep_type); 
	prterr((((((micr_check_type==0)&&(micr_bottom_ypos!=0)||((micr_check_type==1)&&(micr_middle_ypos!=0))))&&((strlen(micrtext)!=0))&&(dirdep_type==7))>0)?"Yes...":"No...");
	prterr("use_full_micr_check = %d line = %d", use_full_micr_check,__LINE__); 
	 */
	if((((micr_check_type==0)&&(micr_bottom_ypos!=0)||((micr_check_type==1)&&(micr_middle_ypos!=0))))&&((strlen(micrtext)!=0))&&(dirdep_type==7))
	{
		/*use_full_micr_check=1;	*/
		use_full_micr_check=0;	
	}
	if(micrtext!=NULL) Rfree(micrtext);

	for(x=0;x<66;x++)
	{
		micr_xpos[x]=0;
		if(use_full_micr_check)
		{
			memset(stemp,0,101);
			sprintf(stemp,"MICR CHAR %02d XPOS",x);
			FINDFLDGETSHORT(bnkacc,stemp,&micr_xpos[x]);
		}
	}
	/* End Setting MICR Values */

	FINDRSCGETSTRING(r,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymst,"PAY IDENTIFICATION",payid);
	EQLNRD(paymst,1);
	FINDFLDGETINT(paymst,"CALENDAR YEAR",&calyear);
	FINDRSCGETSTRING(r,"MESSAGE 1",&message1);
	FINDRSCGETSTRING(r,"MESSAGE 2",&message2);
	FINDRSCGETSTRING(r,"MESSAGE 3",&message3);
	FINDRSCGETSTRING(r,"MESSAGE 4",&message4);
	FINDRSCGETSTRING(r,"MESSAGE 5",&message5);
	FINDRSCGETSTRING(r,"MESSAGE 6",&message6);
	FINDRSCGETSTRING(r,"MESSAGE 7",&message7);
	FINDRSCGETSTRING(r,"MESSAGE 8",&message8);
	FINDRSCGETSTRING(r,"MESSAGE 9",&message9);

	CreateSortFile(rs,r);
	if(DID_STUFF)
	{	
		diagrsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
			"Collecting Details",NULL);
		addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
		addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmst);
		addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
		addDFincvir(diagrsrc,"POSTRK","POSGRS",NULL,posgrs);
		addDFincvir(diagrsrc,module,"PAYDEDM",NULL,payded);
		addDFincvir(diagrsrc,module,"PAYDDMS",NULL,paydds);
		addDFincvir(diagrsrc,module,"PAYDTYP",NULL,paydtyp);
		if(XPERT_SETUP->LVSIMP==TRUE)
		{
		addDFincvir(diagrsrc,"LVSIMP","LVSMSTR",NULL,lvemst);
		addDFincvir(diagrsrc,"LVSIMP","LVSDSC",NULL,lvedsc);
		} else {
		addDFincvir(diagrsrc,"LVEMGT","LVEMSTR",NULL,lvemst);
		addDFincvir(diagrsrc,"LVEMGT","LVEDSC",NULL,lvedsc);
		}
		addDFincvir(diagrsrc,"BNKREC","BNKACC",NULL,bnkacc);
		if(diagrsrc!=NULL)
		{
			if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(diagrsrc);
		}
		ZERNRD(sortfile->fileno);
		ef=FRSNRD(sortfile->fileno,1);
		while(!ef)
		{
			GatherHistory();
			if(XPERT_SETUP->LVEMGT)
			{
				GatherLeave();	
			} else if(XPERT_SETUP->LVSIMP)
			{
				GatherLeaveSimple();
			}
			GatherDeductions();
			WRTNRD(sortfile->fileno,0,NULL);
			if(diagrsrc!=NULL) update_diagnostic(diagrsrc,TRUE);
			if(diagapps)
			{
				SEENRDRECORD(sortfile->fileno);TRACE;
			}
			ef=NXTNRD(sortfile->fileno,1);
		}
		if(OrgAddr1!=NULL)
		{
			OrgAddr1->computed=FALSE;
			computevirtual(OrgAddr1,SCRNvirtualSubData,diagrsrc);
		}
		if(OrgAddr2!=NULL)
		{
			OrgAddr2->computed=FALSE;
			computevirtual(OrgAddr2,SCRNvirtualSubData,diagrsrc);
		}
		if(OrgAddr3!=NULL)
		{
			OrgAddr3->computed=FALSE;
			computevirtual(OrgAddr3,SCRNvirtualSubData,diagrsrc);
		}
		if(diagrsrc!=NULL)
		{
			killwindow(diagrsrc);
			free_rsrc(diagrsrc);
			diagrsrc=NULL;
		}

		rsrc=RDArsrcNEW("PAYROLL","PRINT DIRDEP OUTPUT DEVICE SCREEN");
		defaultprinter=DefaultPrinter();
		addbtnrsrc(rsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncOutputDevice);
		addstdrsrc(rsrc,"OUTPUT DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
		if(defaultprinter!=NULL) Rfree(defaultprinter);
		
		addrfcbrsrc(rsrc,"ALIGNMENT",TRUE,alignment,NULL);
		if(check_formfeed)
		{
			FINDRSCSETEDITABLE(rsrc,"ALIGNMENT",FALSE);
			FINDRSCSETSENSITIVE(rsrc,"ALIGNMENT",FALSE);
		}
		addrfcbrsrc(rsrc,"PRINT",TRUE,printchecks,r);
		addrfcbrsrc(rsrc,"QUIT",TRUE,quitprint,r);
		addrfcbrsrc(rsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		addbtnrsrc(rsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);

		APPmakescrn(rsrc,TRUE,quitprint,r,TRUE);

	} else {
		prterr("Warning:  No Direct Deposits were selected using the given criteria.");
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
	RDAwdgt *w=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"PRINT DIRECT DEPOSIT MICRO 8.5x11")) 
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

	gsv=RDAGenericSetupNew(module,"FORM FEED CHECKS");
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
	gsv=RDAGenericSetupNew(module,"FORM DECIMAL TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			formdecimal=*gsv->value.short_value;
		} else {
			formdecimal=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"TRANSMITTAL TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			dirdep_type=*gsv->value.short_value;
		} else {
			dirdep_type=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PAYCHECK ADDRESS OFFSET");
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
	gsv=RDAGenericSetupNew(module,"8.5x11 SHOW FILING STATUS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SHOW_FILING_STATUS=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			SHOW_FILING_STATUS=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"FORM LEAVE TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			formleave=*gsv->value.short_value;
		} else {
			formleave=FALSE;
		}
	}
	/*
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 NUMBER OF DEDUCTIONS LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			number_deductions=(*gsv->value.short_value)*3;
		} else {
			number_deductions=NUMBER_DEDUCTIONS;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 NUMBER OF JOB LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			number_jobs=(*gsv->value.short_value);
		} else {
			number_jobs=NUMBER_JOBS;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 NUMBER OF LEAVE LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			number_lvemstr=(*gsv->value.short_value)*2;
		} else {
			number_lvemstr=NUMBER_LVEMSTR;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 NUMBER OF MESSAGE LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			PRINT_NUMBER_MESSAGE=(*gsv->value.short_value);
		} else {
			PRINT_NUMBER_MESSAGE=NUMBER_MESSAGE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	*/
	gsv=RDAGenericSetupNew(module,"8.5x11 SHOW ZERO JOBS");
	if(gsv!=NULL)
	{
		show_zero_jobs=FALSE;
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			if(*gsv->value.string_value==TRUE) show_zero_jobs=TRUE;
				else show_zero_jobs=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 LEAVE SHORT DESCRIPTION");
	if(gsv!=NULL)
	{
		LEAVE_SHORT_DESC=FALSE;
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			if(*gsv->value.string_value==TRUE) LEAVE_SHORT_DESC=TRUE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 DEDUCTION SHORT DESCRIPTION");
	if(gsv!=NULL)
	{
		sdesc=FALSE;
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			if(*gsv->value.string_value==TRUE) sdesc=TRUE;
				else sdesc=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRINT EMPLOYER PAID DEDUCTIONS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			print_employer_deductions=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			print_employer_deductions=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"FORMS JOB DESCRIPTION TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			jdtype=(*gsv->value.short_value);
		} else jdtype=0;
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"DISPLAY SSN");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			display_ssn=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			display_ssn=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"HISTORY FROM PAYDCSM");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			USE_PAYDCSM=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			USE_PAYDCSM=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"FEDERAL EXEMPTION LOCATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			federal_exemption_location=*gsv->value.short_value;
		} else {
			federal_exemption_location=0;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"SKIP LEAVE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			skip_leave=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			skip_leave=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"STATE EXEMPTION LOCATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			state_exemption_location=*gsv->value.short_value;
		} else {
			state_exemption_location=0;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"DIRECT DEPOSIT OFFSET");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			dir_dep_offset=*gsv->value.short_value;
		} else {
			dir_dep_offset=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"DIRECT DEPOSIT NET TYPE");
	if(getRDAGenericSetupbin(libx,gsv))
	{
		DD_NET_TYPE=FALSE;
	} else {
		DD_NET_TYPE=*gsv->value.short_value;
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"LOW ADDRESS ON DIRECT DEPOSITS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			low_address=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			low_address=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PAYROLL","DEFAULT BANK IDENTIFICATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			defbankid=stralloc(gsv->value.string_value);
		} else {
			defbankid=NULL;
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
			/* FORM STUB CHECK */
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

	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((payded=APPOPNNRD(module,"PAYDEDM",TRUE,FALSE))==(-1)) return;
	if((paydds=APPOPNNRD(module,"PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((paydtyp=APPOPNNRD(module,"PAYDTYP",TRUE,FALSE))==(-1)) return;
	if((posgrs=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((posmst=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((paymst=APPOPNNRD(module,"PAYMSTR",TRUE,FALSE))==(-1)) return;
	if((payjpms=APPOPNNRD(module,"PAYJPMS",TRUE,FALSE))==(-1)) return;
	if((payjprm=APPOPNNRD(module,"PAYJPRM",TRUE,FALSE))==(-1)) return;
	if((payppms=APPOPNNRD(module,"PAYPPMS",TRUE,TRUE))==(-1)) return;
	if((payjdpm=APPOPNNRD(module,"PAYJDPM",TRUE,FALSE))==(-1)) return;
	if((payjcsm=APPOPNNRD(module,"PAYJCSM",TRUE,FALSE))==(-1)) return;
	if(USE_PAYDCSM==TRUE)
	{
		if((payjdcm=APPOPNNRD(module,"PAYDCSM",TRUE,FALSE))==(-1)) return;
	} else {
		if((payjdcm=APPOPNNRD(module,"PAYJDCM",TRUE,FALSE))==(-1)) return;
	}
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if(XPERT_SETUP->LVEMGT==TRUE)
	{
		if((lvemst=APPOPNNRD("LVEMGT","LVEMSTR",TRUE,FALSE))==(-1)) return;
		if((lvedsc=APPOPNNRD("LVEMGT","LVEDSC",TRUE,FALSE))==(-1)) return;
	} else if(XPERT_SETUP->LVSIMP==TRUE)
	{
		if((lvemst=APPOPNNRD("LVSIMP","LVSMSTR",TRUE,FALSE))==(-1)) return;
		if((lvedsc=APPOPNNRD("LVSIMP","LVSDSC",TRUE,FALSE))==(-1)) return;
	} else {
		lvemst=(-1);
		lvedsc=(-1);
	}
	mainrsrc=RDArsrcNEW(module,"PRINT PAYROLL DIRECT DEPOSIT FORM 8.5x11");
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(mainrsrc,module,"PAYMSTR",NULL,paymst);
	if(bnkacc!=(-1)) file2rsrc(bnkacc,mainrsrc,FALSE);
	if(paymst!=(-1)) file2rsrc(paymst,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	FINDRSCSETEDITABLE(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",TRUE);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(mainrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mainrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(mainrsrc,module,"PAYPPMS",NULL,payppms);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	if(perdmg!=(-1)) file2rangersrc(perdmg,mainrsrc);
	if(posmst!=(-1)) file2rangersrc(posmst,mainrsrc);
	if(payjmst!=(-1)) file2rangersrc(payjmst,mainrsrc);
	if(payjpms!=(-1)) file2rangersrc(payjpms,mainrsrc);
	if(payppms!=(-1)) file2rangersrc(payppms,mainrsrc);
	addstdrsrc(mainrsrc,"MESSAGE 1",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 2",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 3",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 4",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 5",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 6",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 7",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 8",PLAINTEXT,122,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 9",PLAINTEXT,122,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	rsrc2filerecord(bnkacc,mainrsrc);
	rsrc2filerecord(paymst,mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	if(!isEMPTY(defbankid))
	{
		FINDRSCSETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",defbankid);
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",defbankid);
	} else {
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bankno);
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankno);
	}
	filerecord2rsrc(bnkacc,mainrsrc);
	filerecord2rsrc(paymst,mainrsrc);
	addrfcbrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,selectchecks,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	switch(startup_type)
	{
		case  1:
		default:
			/* PLAIN TEXT DIRECT DEPOSIT */
			mainrsrc->scn->name=stralloc("PRINT PAYROLL PLAIN DIRECT DEPOSIT");
			w=mainrsrc->scn->wdgts+3;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  2:
			/* DECORATED STUB PLAIN DIRECT DEPOSIT */
			mainrsrc->scn->name=stralloc("PRINT PAYROLL DECORATED STUB PLAIN DIRECT DEPOSIT");
			w=mainrsrc->scn->wdgts+10;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
		case  3:
			/* FORM STUB DIRECT DEPOSIT */
			mainrsrc->scn->name=stralloc("PRINT PAYROLL DECORATED STUB FORM DIRECT DEPOSIT");
			w=mainrsrc->scn->wdgts+23;
			if(w->expression!=NULL) Rfree(w->expression);
			break;
	}
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	ReadRDAScrolledLists(mainrsrc);
	RDAAPPMAINLOOP();
}
