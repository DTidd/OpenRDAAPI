/* paycheck.c - Print Payroll Checks */

#ifndef WIN32
#define __NAM__ "paycheck.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paycheck.exe"
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

#define DEFAULT_NUMBER_JOBS	5
#define DEFAULT_NUMBER_DEDUCTIONS	30
#define DEFAULT_NUMBER_LVEMSTR	10
#define DEFAULT_NUMBER_MESSAGE	3
static char *module="PAYROLL",sdesc=FALSE;
static char LEAVE_SHORT_DESC=FALSE,SHOW_FILING_STATUS=FALSE;
static short perdmg=(-1),payjmst=(-1),payded=(-1),paydds=(-1),posgrs=(-1);
static short bnkacc=(-1),lvemst=(-1),lvedsc=(-1),paydtyp=(-1);
static short paymst=(-1),payjdpm=(-1),payjpms=(-1),payjcsm=(-1),payjdcm=(-1);
static short posmst=(-1),payppms=(-1),payjprm=(-1);
static short formdecimal=FALSE,formleave=FALSE;
static char *defbankid=NULL;
static char display_ssn=FALSE,USE_PAYDCSM=FALSE;
static short PRINT_NUMBER_JOBS=DEFAULT_NUMBER_JOBS,check_type=0;
static short PRINT_NUMBER_DEDUCTIONS=DEFAULT_NUMBER_DEDUCTIONS;
static short PRINT_NUMBER_LVEMSTR=DEFAULT_NUMBER_LVEMSTR;
static short PRINT_NUMBER_MESSAGE=DEFAULT_NUMBER_MESSAGE;
static short ADDRESS_OFFSET=0;
static RDArsrc *diagrsrc=NULL;
static char *message1=NULL,*message2=NULL,*message3=NULL;
static char *message4=NULL,*message5=NULL,*message6=NULL;
static char *message7=NULL,*message8=NULL,*message9=NULL;
static char *paystarting=NULL,*payending=NULL;
static char check_formfeed=FALSE,use_sign_locks=FALSE;
static char *bankno=NULL,*payid=NULL;
static short DID_STUFF=0,check_signature=0,jdtype=0;
static DBsort *sortfile=NULL;
static RDAvirtual *EmployeeName=NULL;
static RDAvirtual *EmployeeAddr1=NULL,*EmployeeAddr2=NULL,*EmployeeAddr3=NULL;
static RDAvirtual *OrgAddr1=NULL,*OrgAddr2=NULL,*OrgAddr3=NULL;
static int calyear=0;
static char stemp1[101],stemp2[101];
static char *outdevice=NULL;
static char show_zero_jobs=FALSE;
static int federal_exemption_location=0;
static int state_exemption_location=0;
static char print_employer_deductions=FALSE,skip_leave=FALSE;
static short spelled_amount_position=0;

static void QAccFld(short tfileno,char *tfieldname,short ffileno,char *ffieldname)
{
	double amt=0.0,camt=0.0;

	FINDFLDGETDOUBLE(tfileno,tfieldname,&camt);
	FINDFLDGETDOUBLE(ffileno,ffieldname,&amt);
	camt+=amt;
	FINDFLDSETDOUBLE(tfileno,tfieldname,camt);
}
static void chkResetGroupDefaults(RDArsrc *r)
{
	ResetGroupDefaults(r);
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
	if(defbankid!=NULL) Rfree(defbankid);
	if(bankno!=NULL) Rfree(bankno);
	if(payid!=NULL) Rfree(payid);
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
	FINDRSCGETSTRING(r,"OUTPUT DEVICE",&outdevice);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
		SelectPrintType(outdevice,0,fp);
		if(fp!=NULL)
		{
			RDA_fprintf(fp,"       XXX\r\n");
			SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
			RDA_pclose(fp);
		} else {
			prterr("Error:  Couldn't open output device [%s].",outdevice);
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
static void printactualcheck(RDA_PFILE *fp)
{
	char *ssn=NULL,*empid=NULL,*loc=NULL,*empname=NULL,*sfs=NULL; /* state filing status */
	int ffs=0; /* federal filing status */
	int fed_exemption=0,state_exemption=0,state_dependents=0;
	char *temp=NULL,*spelled_amt=NULL,*temp1=NULL,*temp2=NULL,*temp3=NULL;
	char *temp4=NULL;
	char *tempstr=NULL;
	char *emp_add1=NULL,*emp_add2=NULL,*emp_add3=NULL;
	double gross=0.0,adjust=0.0,regular=0.0,units=0.0,overtime=0.0;
	double overunits=0.0,ytd=0.0;
	double total_cur_gross=0.0,total_ytd=0.0,cur_gross=0.0;
	double cur=0.0,total_cur=0.0,beg=0.0,earn=0.0,used=0.0,ending=0.0;
	double net=0.0,n=0.0,total_ecur=0.0,total_eytd=0.0;
	int checkno=0,x,y=0,skipped=0;
	char *check_date=NULL,emplr_paid=FALSE,outoforder=FALSE;
	short prtsig=1;


	FINDFLDGETSTRING(sortfile->fileno,"LOCATION",&loc);
	FINDFLDGETINT(sortfile->fileno,"CHECK NUMBER",&checkno);
	FINDFLDGETSTRING(sortfile->fileno,"CHECK DATE",&check_date);
	FINDFLDGETSTRING(sortfile->fileno,"START DATE",&paystarting);
	FINDFLDGETSTRING(sortfile->fileno,"END DATE",&payending);
	FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDGETCHAR(sortfile->fileno,"OUT OF ORDER",&outoforder);
	FINDFLDGETSTRING(sortfile->fileno,"SOCIAL SECURITY NUMBER",&ssn);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 1",&emp_add1);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 2",&emp_add2);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 3",&emp_add3);

	RDA_fprintf(fp,"%-40sLocation:%-15s %15s\r\n",
		(XPERT_SETUP->company_name?XPERT_SETUP->company_name:""),
		(loc!=NULL?loc:""),(outoforder ? "HISTORY UPDATED":""));

	FINDFLDGETSTRING(sortfile->fileno,"EMPLOYEE NAME",&empname);
	if(RDAstrlen(empname)>25) empname[25]=0;
	RDA_fprintf(fp,"%-40sEmployee Name: %-25s\r\n",
		(OrgAddr1->value.string_value!=NULL?OrgAddr1->value.string_value:""),
		(empname!=NULL ? empname:""));

	if(display_ssn==TRUE)
	{
	RDA_fprintf(fp,"%-40sID: %-15s     SSN: %-11s\r\n",
		(OrgAddr2->value.string_value!=NULL?OrgAddr2->value.string_value:""),
		(empid!=NULL ? empid:""),
		(ssn!=NULL ? ssn:""));
	} else {
	RDA_fprintf(fp,"%-40sID: %-15s\r\n",
		(OrgAddr2->value.string_value!=NULL?OrgAddr2->value.string_value:""),
		(empid!=NULL ? empid:""));
	}

	RDA_fprintf(fp,"%-40sPeriod Start:%-10s End:  %-10s\r\n",
		(OrgAddr3->value.string_value!=NULL? OrgAddr3->value.string_value:""),
		(paystarting!=NULL?paystarting:""),
		(payending!=NULL?payending:""));

	RDA_fprintf(fp,"%-40sCheck No.: %12d Date: %-10s\r\n",
		(XPERT_SETUP->phone!=NULL ? XPERT_SETUP->phone:""),
		checkno,
		(check_date!=NULL?check_date:""));
	
	if(SHOW_FILING_STATUS==FALSE)
	{
		RDA_fprintf(fp,"\r\n");
	} else {
		/* JAB:  Begin line to conditionally include federal and state filing status and exemptions */
		temp1=Rmalloc(200+1);
		temp2=Rmalloc(200+1);
		if(federal_exemption_location == 0)
		{
			FINDFLDGETINT(sortfile->fileno,"FEDERAL FILING STATUS",&ffs);
			FINDFLDGETINT(sortfile->fileno,"FEDERAL EXEMPTION",&fed_exemption);
		    	sprintf(temp1,"Federal Status: %-7s %.2d",(ffs==0) ? "Married" : "Single", fed_exemption);
		}
		else
		{
		    	sprintf(temp1,"                          ","");
		}
		if(state_exemption_location == 0)
		{
			FINDFLDGETSTRING(sortfile->fileno,"STATE FILING STATUS",&sfs);
			FINDFLDGETINT(sortfile->fileno,"STATE EXEMPTION",&state_exemption);
			FINDFLDGETINT(sortfile->fileno,"STATE DEPENDENTS",&state_dependents);
			sprintf(temp2,"State Status: %-20s %.2d Dependents %.2d", sfs, state_exemption, state_dependents);
		}
		else
		{
			sprintf(temp2,"");
		}
		RDA_fprintf(fp," %s  %s\r\n",temp1,temp2);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
	}
	/* JAB:  End line to conditionally include state and federal filing status and exemptions */
	RDA_fprintf(fp,"Job Title           Gross  Adjust Regular Units OT Gross   OTU Current       YTD\r\n");
	for(x=0;x<PRINT_NUMBER_JOBS;++x)
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
			RDA_fprintf(fp,"%-15s %9.02f %7.02f %7.02f %5.02f %8.02f %5.02f %7.02f %9.02f\r\n",
				(temp!=NULL ? temp:""),gross/100,adjust/100,
				regular/100,units,overtime/100,overunits,
				cur_gross/100,ytd/100);
		} else if((((cur_gross!=0.0 || ytd!=0.0)) || (!isEMPTY(temp) && show_zero_jobs)) && outoforder==TRUE)
		{
			RDA_fprintf(fp,"%-15s %9.02f %7.02f %7.02f %5.02f %8.02f %5.02f %7.02f\r\n",
				(temp!=NULL ? temp:""),gross/100,adjust/100,
				regular/100,units,overtime/100,overunits,
				cur_gross/100);
		} else {
			++skipped;
		}
		if(temp!=NULL) Rfree(temp);
	}
	for(x=0;x<skipped;++x) RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"%-48s Total Gross: %8.02f %9.02f\r\n","",
		total_cur_gross/100,total_ytd/100);
	RDA_fprintf(fp,"\r\n");

	RDA_fprintf(fp,"Deduction     Cur      YTD Deduction     Cur      YTD Deduction     Cur      YTD\r\n");
	total_cur=0.0;
	total_ytd=0.0;
	total_ecur=0.0;
	total_eytd=0.0;
	for(x=0;x<PRINT_NUMBER_DEDUCTIONS;x+=3)
	{
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
				RDA_fprintf(fp,"%-9s %7.02f %8.02f",
					(temp!=NULL?temp:""),
					cur/100,ytd/100);
				} else {
				RDA_fprintf(fp,"%-9s %7.02f %8s",
					(temp!=NULL?temp:""),
					cur/100,"");
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
	if(print_employer_deductions==FALSE)
	{
		if(!outoforder)
		{
			RDA_fprintf(fp,"%-46sTotal Deductions: %7.02f %8.02f\r\n","",
				total_cur/100,total_ytd/100);
		} else {
			RDA_fprintf(fp,"%-46sTotal Deductions: %7.02f %8s\r\n","",
				total_cur/100,"");
		}
	} else {
		if(!outoforder)
		{
			RDA_fprintf(fp,"%-27sEmployer: %7.02f %8.02f Employee: %7.02f %8.02f\r\n","",total_ecur/100,total_eytd/100,total_cur/100,total_ytd/100);
		} else {
			RDA_fprintf(fp,"%-27sEmployer: %7.02f %8s Employee: %7.02f %8s\r\n","",total_ecur/100,"",total_cur/100,"");
		}
	}
	RDA_fprintf(fp,"\r\n");

if((XPERT_SETUP->LVEMGT==TRUE || XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
{
	if(PRINT_NUMBER_LVEMSTR==0)
	{
		RDA_fprintf(fp,"\r\n");
	} else {
		RDA_fprintf(fp,"Leave Desc        Beg Earn Used  Ending  Leave Desc        Beg Earn Used  Ending\r\n");
	}
	for(x=0;x<PRINT_NUMBER_LVEMSTR;x+=2)
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
			if(!isEMPTY(temp) || ending!=0.0 || earn!=0.0 
				|| used!=0.0 || beg!=0.0)
			{
				temp1=decimal(beg);
				temp2=decimal(earn);
				temp3=decimal(used);
				temp4=decimal(ending);
				switch(formleave)
				{
					default:
					case 0:
						if(RDAstrlen(temp)>14) temp[14]=0;
						RDA_fprintf(fp,"%-14s%6s %4s %5s %7s",
							(temp!=NULL ? temp:""),
							(temp1!=NULL ? temp1:""),
							(temp2!=NULL ? temp2:""),
							(temp3!=NULL ? temp3:""),
							(temp4!=NULL ? temp4:""));
						break;
					case 1:
						if(RDAstrlen(temp)>12) temp[12]=0;
						RDA_fprintf(fp,"%-12s%7s %4s %5s %8s",
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
	for(x=0;x<(PRINT_NUMBER_LVEMSTR/2)+1;++x)
	{
		RDA_fprintf(fp,"\r\n");
	}
}
	RDA_fprintf(fp,"Current Gross:%10.02f - Deductions:%10.02f = Net Check:%10.02f\r\n",total_cur_gross/100,total_cur/100,net/100);
	for(x=1;x<PRINT_NUMBER_MESSAGE+1 && x<10;++x)
	{
		switch(x)
		{
			default:
			case 1:
				RDA_fprintf(fp,"%-s\r\n",(message1!=NULL ? message1:""));
				break;
			case 2:
				RDA_fprintf(fp,"%-s\r\n",(message2!=NULL ? message2:""));
				break;
			case 3:
				RDA_fprintf(fp,"%-s\r\n",(message3!=NULL ? message3:""));
				break;
			case 4:
				RDA_fprintf(fp,"%-s\r\n",(message4!=NULL ? message4:""));
				break;
			case 5:
				RDA_fprintf(fp,"%-s\r\n",(message5!=NULL ? message5:""));
				break;
			case 6:
				RDA_fprintf(fp,"%-s\r\n",(message6!=NULL ? message6:""));
				break;
			case 7:
				RDA_fprintf(fp,"%-s\r\n",(message7!=NULL ? message7:""));
				break;
			case 8:
				RDA_fprintf(fp,"%-s\r\n",(message8!=NULL ? message8:""));
				break;
			case 9:
				RDA_fprintf(fp,"%-s\r\n",(message9!=NULL ? message9:""));
				break;
		}
	}
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	if(spelled_amount_position!=2)
	{
		RDA_fprintf(fp,"\r\n");
	}
	if(check_signature!=3)
	{
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	}
	RDA_fprintf(fp,"\r\n");
	if(net>0)
	{
		spelled_amt=spell(net);
		RDA_fprintf(fp,"  %s\r\n",spelled_amt);
		if(spelled_amt!=NULL) Rfree(spelled_amt);
		prtsig=1;
	} else if(total_cur==0 && net==0) /* no deductions */
	{
		spelled_amt=spell(total_cur_gross);
		RDA_fprintf(fp,"  %s\r\n",spelled_amt);
		if(spelled_amt!=NULL) Rfree(spelled_amt);
		prtsig=1;
	} else {
		RDA_fprintf(fp,"  *** V O I D **** V O I D **** V O I D **** \r\n");
		prtsig=0;
	}
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	if(total_cur==0 && net==0) /* no deductions */
	{
		tempstr=famtpad(total_cur_gross,12,'*');
		RDA_fprintf(fp,"%48s%-10s %8d %-12s\r\n",
			"",
			(check_date!=NULL ? check_date:""),
			checkno,
			tempstr);
/*
			total_cur_gross/100);
*/
		if(tempstr!=NULL) Rfree(tempstr);
	} else {
		tempstr=famtpad(net,12,'*');
		RDA_fprintf(fp,"%48s%-10s %8d %-12s\r\n",
			"",
			(check_date!=NULL ? check_date:""),
			checkno,
			tempstr);
/*
			net/100);
*/
		if(tempstr!=NULL) Rfree(tempstr);
	}
	if(check_type!=7) RDA_fprintf(fp,"\r\n");

	if(check_signature==0)
	{
		RDA_fprintf(fp,"\r\n");
	} else if((check_signature==5))
	{
		SelectPrintTypeByName(outdevice,"PUSH CURSOR",fp);
		if(prtsig) { SelectPrintTypeByName(outdevice,"PAYROLL SIGNATURE",fp); }
		SelectPrintTypeByName(outdevice,"POP CURSOR",fp);
		if(check_type!=7) RDA_fprintf(fp,"\r\n");
	} else if((check_signature==4))
	{
		SelectPrintTypeByName(outdevice,"PUSH CURSOR",fp);
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE UNLOCK",fp);
		}
		if(prtsig) { SelectPrintTypeByName(outdevice,"SIGNATURE FILE",fp); }
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE LOCK",fp);
		}
		SelectPrintTypeByName(outdevice,"POP CURSOR",fp);
		if(check_type!=7) RDA_fprintf(fp,"\r\n");
	} else if((check_signature==1) || (check_signature==3))
	{
		RDA_fprintf(fp,"%52s","");
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE UNLOCK",fp);
		}
		if(prtsig) { SelectPrintTypeByName(outdevice,"SIGNATURE DIMM 1",fp); }
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE LOCK",fp);
		}
		RDA_fprintf(fp,"\r\n");
	} else if(check_signature==2)
	{
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE UNLOCK",fp);
		}
		if(prtsig) { SelectPrintTypeByName(outdevice,"SIGNATURE DIMM A",fp); }
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE LOCK",fp);
		}
		RDA_fprintf(fp,"\r\n");
	}
	if(check_signature==3)
	{
		RDA_fprintf(fp,"\r\n");
		RDA_fprintf(fp,"\r\n");
	}
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(empname!=NULL ? empname:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add1!=NULL ? emp_add1:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add2!=NULL ? emp_add2:""));
	RDA_fprintf(fp,"%*s%-40s\r\n\f",8+ADDRESS_OFFSET,"",(emp_add3!=NULL ? emp_add3:""));
	
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

	readwidget(r,"OUTPUT DEVICE");
	FINDRSCGETSTRING(r,"OUTPUT DEVICE",&outdevice);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
		if(check_signature==5)
		{
			SelectPrintTypeByName(outdevice,"LOAD PAYROLL SIGNATURE",fp);
		}
		SelectPrintType(outdevice,0,fp);
		if(check_formfeed==TRUE)
		{
			SelectPrintTypeByName(outdevice,"SIMPLEX",fp);
			SelectPrintTypeByName(outdevice,"PORTRAIT",fp);
			RDA_fprintf(fp,"\f");
		}
	} else {
		prterr("Error:  Output device not found.");
		return;
	}
	diagrsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
		"Printing Checks",NULL);
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
	if(check_signature==5)
	{
		SelectPrintTypeByName(outdevice,"DELETE_SOFTFONT_200",fp);
		SelectPrintTypeByName(outdevice,"DELETE_SOFTFONT_201",fp);
	}
	SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
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
	char *code2=NULL,*typ1=NULL,bfs=FALSE;
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
			if(bfs==FALSE)
			{
			if(reduce_net==TRUE || (reduce_net==FALSE && print_employer_deductions==TRUE))
			{
				FINDFLDGETSTRING(paydds,"DEDUCTION DESCRIPTION IDENTIFICATION",&code2);

				for(x=0;x<PRINT_NUMBER_DEDUCTIONS;++x)
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
}
static void GatherLeaveSimple()
{
	short ef=0,x=0;
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL,nos=FALSE,active=FALSE;
	double camt=0.0,beg=0.0,pearn=0.0,pused=0.0,earn=0.0,used=0.0;
	char *leaveid=NULL,*leaveid1=NULL;

	if(lvemst!=(-1) && lvedsc!=(-1) && (XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
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
					for(x=0;x<PRINT_NUMBER_LVEMSTR;++x)
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
					for(x=0;x<PRINT_NUMBER_LVEMSTR;++x)
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
	short ef=0;
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
			ZERNRD(payjcsm);
			COPYFIELD(payjmst,payjcsm,"PERSONNEL IDENTIFICATION");
			COPYFIELD(payjmst,payjcsm,"JOB NUMBER");
			FINDFLDSETINT(payjcsm,"CALENDAR YEAR",calyear);
			if(ADVEQLNRDsec(payjcsm,1,SCRNvirtualSubData,diagrsrc))
			{
				KEYNRD(payjcsm,1);
			} else {
			ZERNRD(posmst);
			COPYFIELD(payjmst,posmst,"POSITION IDENTIFICATION");
			if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posmst,1);
			for(x=0;x<PRINT_NUMBER_JOBS;++x)
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
				} else if(x==(PRINT_NUMBER_JOBS-1))
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
	char *payid1=NULL,*typ1=NULL,*code2=NULL,bfs=FALSE;
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
			if(bfs==FALSE)
			{
			if(reduce_net==TRUE || (reduce_net==FALSE && print_employer_deductions==TRUE))
			{
				FINDFLDGETSTRING(paydds,"DEDUCTION DESCRIPTION IDENTIFICATION",&code2);

				for(x=0;x<PRINT_NUMBER_DEDUCTIONS;++x)
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
}
static CheckRate *GatherCurrentCheckRate(char *empid,char *payid,int jobno)
{
	CheckRate *C=NULL;
	short ef=0,payjprm_keyno=0;
	char *empid1=NULL,*payid1=NULL,delflag=FALSE,*rate=NULL;
	int jobno1=0;
	double gross=0.0,units=0.0;

	C=Rmalloc(sizeof(CheckRate));
	ZERNRD(payjprm);
	FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",empid);
	FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
	FINDFLDSETINT(payjprm,"JOB NUMBER",jobno);
	payjprm_keyno=KEYNUMBER(payjprm,"RDA PAYJPRM KEY");
	if(payjprm_keyno<1)
	{
		prterr("ERROR: Key Not Found - [PAYROLL][PAYJPRM] missing [RDA PAYJPRM KEY]");TRACE;
		payjprm_keyno=1; 
	}
	ef=ADVGTENRDsec(payjprm,payjprm_keyno,SCRNvirtualSubData,diagrsrc);
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
			} else if(RDAstrstr(rate,"HOURLY") || RDAstrstr(rate,"DAILY") || RDAstrstr(rate,"UNIT") || RDAstrstr(rate,"MEETING")) /* Unit Rates */
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
		ef=ADVNXTNRDsec(payjprm,payjprm_keyno,SCRNvirtualSubData,diagrsrc);
	}	
	if(payid1!=NULL) Rfree(payid1);
	if(empid1!=NULL) Rfree(empid1);	
	return(C);
}
static short AtLeastOneJob(char *empid,RangeScreen *rs,RDArsrc *mainrsrc)
{
	short retval=FALSE,ef=FALSE,payjpms_keyno=0;
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
	payjpms_keyno=KEYNUMBER(payjpms,"RDA PAYJPMS KEY");
	if(payjpms_keyno<1)
	{
		prterr("ERROR: Key Not Found - [PAYROLL][PAYJPMS] missing [RDA PAYJPMS KEY]");TRACE;
		payjpms_keyno=1; 
	}
	ef=ADVGTENRDsec(payjpms,payjpms_keyno,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjpms,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETSTRING(payjpms,"PAY IDENTIFICATION",&payid1);
		if(RDAstrcmp(empid,empid1) || RDAstrcmp(payid,payid1)) break;
		FINDFLDGETCHAR(payjpms,"DELETEFLAG",&deleteflag);
		FINDFLDGETSTRING(payjpms,"BANK IDENTIFICATION",&bankno1);
		FINDFLDGETINT(payjpms,"CHECK NUMBER",&checkno);
		ZERNRD(payjmst);
		COPYFIELD(payjpms,payjmst,"PERSONNEL IDENTIFICATION");
		COPYFIELD(payjpms,payjmst,"JOB NUMBER");
		if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payjmst,1);
		ZERNRD(posgrs);
		COPYFIELD(payjmst,posmst,"POSITION IDENTIFICATION");
		if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posmst,1);
		FINDFLDGETDOUBLE(payjpms,"CHECK NET",&net);
		/* uncompute virtual fields */
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(!deleteflag && !testfilerangersrc(payjmst,rs) &&
			!testfilerangersrc(payjpms,rs) && 
			checkno!=0 && !RDAstrcmp(bankno,bankno1))
		{
			ZERNRD(sortfile->fileno);
			FINDFLDSETINT(sortfile->fileno,"CHECK NUMBER",checkno);
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
				COPYFIELD(paymst,sortfile->fileno,"START DATE");
				COPYFIELD(paymst,sortfile->fileno,"END DATE");
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
				COPYFIELD(payjpms,sortfile->fileno,"PERSONNEL IDENTIFICATION");
				COPYFIELD(payjpms,sortfile->fileno,"CHECK DATE");
				FINDFLDGETINT(payjpms,"JOB NUMBER",&jobno);
				C=GatherCurrentCheckRate(empid,payid,jobno);
				if(C!=NULL)
				{
				for(x=0;x<PRINT_NUMBER_JOBS;++x)
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
							
					} else if(x==(PRINT_NUMBER_JOBS-1))
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
		ef=ADVNXTNRDsec(payjpms,payjpms_keyno,SCRNvirtualSubData,diagrsrc);
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
		"Selecting Checks",NULL);
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
	addDBfield(sortfile,"CHECK NUMBER",LONGV,8);
	addDBfield(sortfile,"CHECK DATE",DATES,10);
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
	for(x=0;x<PRINT_NUMBER_JOBS;++x)
	{
		sprintf(stemp1,"JOB NUMBER %d",(x+1));
		addDBfield(sortfile,stemp1,LONGV,4);
		sprintf(stemp1,"DESCRIPTION %d",(x+1));
		addDBfield(sortfile,stemp1,1,15);
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
	for(x=0;x<PRINT_NUMBER_DEDUCTIONS;++x)
	{
		sprintf(stemp1,"VOL DEDUCTION DESCRIPTION IDENTIFICATION %d",(x+1));
		addDBfield(sortfile,stemp1,1,30);
		sprintf(stemp1,"VOL DEDUCTION TITLE %d",(x+1));
		addDBfield(sortfile,stemp1,1,9);
		sprintf(stemp1,"VOL W/H CURRENT %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"VOL W/H YTD %d",(x+1));
		addDBfield(sortfile,stemp1,2,20);
		sprintf(stemp1,"VOL EMPLOYER PAID %d",(x+1));
		addDBfield(sortfile,stemp1,BOOLNS,1);
	}

/* leave fields */
	for(x=0;x<PRINT_NUMBER_LVEMSTR;++x)
	{
		sprintf(stemp1,"LEAVE ID %d",(x+1));
		addDBfield(sortfile,stemp1,1,40);	
		sprintf(stemp1,"BEGINNING %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"EARNED %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"USED %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
		sprintf(stemp1,"BALANCE %d",(x+1));
		addDBfield(sortfile,stemp1,20,10);
	}

/* Key Info */
	addDBkey(sortfile,"PRINT CHECKS KEY");
	addDBkeypart(sortfile,1,"CHECK NUMBER");
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

	CreateSortFile(rs, r);
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

		rsrc=RDArsrcNEW("PAYROLL","PRINT CHECKS OUTPUT DEVICE SCREEN");
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
		prterr("Warning:  No Checks were selected using the given criteria.");
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

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"PRINT PAYROLL CHECKS 8.5x11")) 
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
	gsv=RDAGenericSetupNew(module,"PAYCHECK TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_type=*gsv->value.short_value;
		} else {
			check_type=FALSE;
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
	gsv=RDAGenericSetupNew(module,"PAY CHECK SIGNATURE TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_signature=*gsv->value.short_value;
		} else {
			check_signature=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 NUMBER OF DEDUCTIONS LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			PRINT_NUMBER_DEDUCTIONS=(*gsv->value.short_value)*3;
		} else {
			PRINT_NUMBER_DEDUCTIONS=DEFAULT_NUMBER_DEDUCTIONS;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 NUMBER OF JOB LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			PRINT_NUMBER_JOBS=(*gsv->value.short_value);
		} else {
			PRINT_NUMBER_JOBS=DEFAULT_NUMBER_JOBS;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"8.5x11 NUMBER OF LEAVE LINES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			PRINT_NUMBER_LVEMSTR=(*gsv->value.short_value)*2;
		} else {
			PRINT_NUMBER_LVEMSTR=DEFAULT_NUMBER_LVEMSTR;
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
			PRINT_NUMBER_MESSAGE=DEFAULT_NUMBER_MESSAGE;
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
	gsv=RDAGenericSetupNew(module,"USE SIGNATURE LOCKS");
	if(gsv!=NULL)
	{
		use_sign_locks=FALSE;
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			if(*gsv->value.string_value==TRUE) use_sign_locks=TRUE;
				else use_sign_locks=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
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
	gsv=RDAGenericSetupNew(module,"PAY CHECK SPELLED AMOUNT POSITION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			spelled_amount_position=*gsv->value.short_value;
		} else {
			spelled_amount_position=0;
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
	mainrsrc=RDArsrcNEW(module,"PRINT PAYROLL CHECKS 8.5x11");
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
	addstdrsrc(mainrsrc,"MESSAGE 1",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 2",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 3",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 4",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 5",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 6",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 7",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 8",PLAINTEXT,72,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 9",PLAINTEXT,72,NULL,TRUE);
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
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	//addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
	addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,chkResetGroupDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,selectchecks,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	ReadRDAScrolledLists(mainrsrc);
	RDAAPPMAINLOOP();
}
