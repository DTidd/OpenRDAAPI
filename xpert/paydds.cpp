/* dftable.c - Define Tables */
#ifndef WIN32
#define __NAM__ "paydds.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paydds.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static short paydds=(-1),payddms=(-1),payddac=(-1),paytbl=(-1);

static void MakePAYDDMS(short paydds)
{
	char *temp=NULL,ben=0,whopays=0,stdded=0,lmt=0,exmpt=0;
	char *title=NULL,*dedid=NULL;
	short computation_type=0,deduction_type=0;
	double p100=100.0,whoamt=0.0,benamt=0.0,amt=0.0,amt1=0.0;
	double eamt=0.0,samt=0.0,lamt=0.0;
	int dedcode=0,tnum=0;
	RDATData *prev=NULL;

	ZERNRD(payddms);
	FINDFLDGETSTRING(paydds,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
	if(isEMPTY(dedid))
	{
		FINDFLDGETINT(paydds,"DEDUCTION DESCRIPTION CODE",&dedcode);
		FINDFLDGETSTRING(paydds,"TITLE",&title);
		if(title==NULL)
		{
			FINDFLDGETSTRING(paydds,"DESCRIPTION",&title);
		}
		memset(stemp,0,101);
		if(!isEMPTY(title))
		{
			sprintf(stemp,"%04d-%s",dedcode,title);
		} else sprintf(stemp,"%04d",dedcode);
		dedid=stralloc(stemp);
	}
	FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	FINDFLDGETDOUBLE(paydds,"AMOUNT",&amt);
	if(EQLNRD(payddms,1))
	{
		KEYNRD(payddms,1);
		FINDFLDGETDOUBLE(paydds,"WHO PAYS AMOUNT",&whoamt);
		FIELDCOPY(paydds,"ACTIVE",payddms,"ACTIVE");
		FINDFLDSETCHAR(payddms,"LIABILITY",TRUE);
		FIELDCOPY(paydds,"VENDOR IDENTIFICATION",payddms,"VENDOR IDENTIFICATION");
		FIELDCOPY(paydds,"TITLE",payddms,"SHORT DESCRIPTION");
		COPYFIELD(paydds,payddms,"DESCRIPTION");
		COPYFIELD(paydds,payddms,"EMPLOYER IDENTIFICATION");
		COPYFIELD(paydds,payddms,"IDENTIFICATION");
		COPYFIELD(paydds,payddms,"CODE");
		FIELDCOPY(paydds,"IDENTIFICATION",payddms,"TRANSMITTAL ROUTE NUMBER");
		FIELDCOPY(paydds,"WRITE W/H CHECK",payddms,"PRINT CHECK");
		COPYFIELD(paydds,payddms,"ELECTRONIC TRANSFER");
		FINDFLDSETSTRING(payddms,"ADDRESS IDENTIFICATION","MAILING ADDRESS");
		FINDFLDSETSTRING(payddms,"RATE SELECTION","[STANDARD RATE SELECTION]");
		FINDFLDSETSTRING(payddms,"GROSS AMOUNT","[STANDARD GROSS AMOUNT]");
		FINDFLDSETSTRING(payddms,"AFFECT SELECT","[STANDARD AFFECT SELECT]");
		FINDFLDSETSTRING(payddms,"COMPUTATION","[STANDARD COMPUTATION]");

		FINDFLDSETCHAR(payddms,"SELECT BASE PAY",TRUE);
		FINDFLDSETCHAR(payddms,"SELECT OVERTIME",TRUE);
		FINDFLDSETCHAR(payddms,"SELECT SUPPLEMENTAL",TRUE);
		FINDFLDSETCHAR(payddms,"BLOCK NOTHING",TRUE);

		FINDFLDGETDOUBLE(paydds,"STANDARD DEDUCTION AMOUNT",&samt);
		if(samt!=0.0)
		{
			FINDFLDSETDOUBLE(payddms,"STANDARD DEDUCTION AMOUNT",samt);
			FINDFLDSETCHAR(payddms,"USE STANDARD DEDUCTION",TRUE);
		}
		FINDFLDGETDOUBLE(paydds,"EXEMPTIONS AMOUNT",&eamt);
		if(eamt!=0.0)
		{
			FINDFLDSETDOUBLE(payddms,"EXEMPTION AMOUNT",eamt);
			FINDFLDSETCHAR(payddms,"USE EXEMPTIONS",TRUE);
		}

		FINDFLDGETSTRING(paydds,"STANDARD DEDUCTION TYPE",&temp);
		stdded=temp[0];
		if(temp!=NULL) Rfree(temp);
		FINDFLDGETSTRING(paydds,"EXEMPTIONS TYPE",&temp);
		exmpt=temp[0];
		if(temp!=NULL) Rfree(temp);
		FINDFLDGETSTRING(paydds,"COMPUTE LIMIT TYPE",&temp);
		lmt=temp[0];
		if(temp!=NULL) Rfree(temp);
		FINDFLDGETDOUBLE(paydds,"COMPUTE LIMIT AMOUNT",&lamt);

		switch(lmt) /* Compute Limit Type */
		{
			case 'E': /* Annual Earnings Amount */
				if(lamt!=0.0)
				{
					FINDFLDSETCHAR(payddms,"USE GROSS AMOUNT LIMITS",TRUE);
					FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",lamt);
				}
				break;
			case 'U': /* Annual Earnings Amount */
				if(lamt!=0.0)
				{
					FINDFLDSETCHAR(payddms,"USE GROSS AMOUNT LIMITS",TRUE);
					FINDFLDSETDOUBLE(payddms,"GROSS AMOUNT LIMIT",lamt);
				}
				break;
			case 'W': /* Current Pay Cycle Benefit Amount */
				break;
			case 'Y': /* Maximum Benefit Amount per Year */
				break;
			case 'P': /* Maximum Earnings Amount per Year */
				break;
			default:
				break;
		}

		FINDFLDGETSTRING(paydds,"WHO PAYS TYPE",&temp);
		whopays=temp[0];
		switch(whopays) /* Who Pays Type */
		{
			case 'C': /* Charge Employee Not Expensed */
				FINDFLDSETCHAR(payddms,"EXPENSED",FALSE);
				FINDFLDSETCHAR(payddms,"REDUCE NET",TRUE);
				break;
			case 'D': /* Organization Paid */
				FINDFLDSETCHAR(payddms,"EXPENSED",TRUE);
				FINDFLDSETCHAR(payddms,"REDUCE NET",FALSE);
				break;
			case 'E': /* Employee Paid */
				FINDFLDSETCHAR(payddms,"REDUCE NET",TRUE);
				FINDFLDSETCHAR(payddms,"EXPENSED",TRUE);
				break;
			case 'I': /* Organization Paid as a Salary Increase */
				FINDFLDSETCHAR(payddms,"EXPENSED",TRUE);
				FINDFLDSETCHAR(payddms,"REDUCE NET",FALSE);
				break;
			case 'J': /* Not paid by anybody */
				FINDFLDSETCHAR(payddms,"EXPENSED",FALSE);
				FINDFLDSETCHAR(payddms,"REDUCE NET",FALSE);
				FINDFLDSETCHAR(payddms,"LIABILITY",FALSE);
				break;
			case 'R': /* Employee paid as a salary decrease */
				FINDFLDSETCHAR(payddms,"REDUCE NET",TRUE);
				FINDFLDSETCHAR(payddms,"EXPENSED",TRUE);
				break;
			case 'N': /* Employee paid as a salary decrease */
				FINDFLDSETCHAR(payddms,"EXPENSED",FALSE);
				FINDFLDSETCHAR(payddms,"REDUCE NET",TRUE);
				FINDFLDSETCHAR(payddms,"LIABILITY",FALSE);
				break;
			default:
				FINDFLDSETCHAR(payddms,"EXPENSED",TRUE);
				FINDFLDSETCHAR(payddms,"REDUCE NET",TRUE);
				FINDFLDSETCHAR(payddms,"LIABILITY",FALSE);
				break;
		}
		if(temp!=NULL) Rfree(temp);
		FINDFLDGETSTRING(paydds,"COMPUTE BENEFIT TYPE",&temp);
		ben=temp[0];
		if(temp!=NULL) Rfree(temp);
		switch(ben) /* Compute Benefit Type */
		{
			case '$':  /* Flat Amount */
			case 'F': /* First Job Only as flat amount */
				computation_type=0;
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"FLAT AMOUNT");
				break;
			case '%':  /* Percentage of Gross */
				computation_type=3;
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				break;
			case '1':  /* Percentage of Shelter Lane # */
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=6;
				break;
			case '2':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=7;
				break;
			case '3':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=3;
				break;
			case '4':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=4;
				break;
			case '5':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=5;
				break;
			case '6':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=3;
				break;
			case '7':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=3;
				break;
			case '8':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=3;
				break;
			case '9':
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				computation_type=3;
				break;
			case 'A': /* Age of Table */
				computation_type=10;
				break;
			case 'D': /* VA optional life flat amount */
				break;
			case 'G': /* Percentage of Contract Gross */
				computation_type=1;
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				FINDFLDSETCHAR(payddms,"SELECT OVERTIME",FALSE);
				FINDFLDSETCHAR(payddms,"SELECT SUPPLEMENTAL",FALSE);
				break;
			case 'I': /* Age Lookup with Special threshold */
				break;
			case 'K': /* Lookup Annual Gross - Federal Withholding on Table */
				break;
			case 'L': /* Lookup Gross on Table */
				computation_type=8;
				break;
			case 'M': /* Percentage of Net */
				computation_type=2;
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				break;
			case 'N': /* Inserts Amount of Net */
				computation_type=2;
				FINDFLDSETDOUBLE(payddms,"PERCENTAGE",p100);
				break;
			case 'R': /* Percentage of Gross not Including Overtime Gross */
				FINDFLDSETCHAR(payddms,"SELECT OVERTIME",FALSE);
				FIELDCOPY(paydds,"COMPUTE BENEFIT AMOUNT",payddms,"PERCENTAGE");
				break;
			case 'S': /* Spouse Age Lookup */
				break;
			case 'T': /* Lookup Annual Gross on Table Number */
				break;
			case 'U': /* Lookup Annual Gross even 1000s on Table Number */
				break;
			case 'V': /* VA optional life age lookup amount */
				break;
			case 'W': /* Percentage of Regular Wages */
				FINDFLDSETCHAR(payddms,"SELECT OVERTIME",FALSE);
				FINDFLDSETCHAR(payddms,"SELECT SUPPLEMENTAL",FALSE);
				break;
			case 'X': /* Percentage of 1/12 or 1/24 of annual gross */
				break;
			case 'Z': /* Lookup Gross on Table Number, round dollars */
				break;
			default:
				break;
		}
		if(whoamt!=100.0 && whoamt!=0.0)
		{
			memset(stemp,0,101);
			sprintf(stemp,"([STANDARD COMPUTATION]*%03.02f)",whoamt);
			FINDFLDSETSTRING(payddms,"COMPUTATION",stemp);
			FINDFLDSETCHAR(payddms,"CUSTOM CONFIGURATION",TRUE);
		} else if(whoamt==0.0)
		{
			FINDFLDSETCHAR(payddms,"ACTIVE",FALSE);
			FINDFLDSETSTRING(payddms,"COMPUTATION","[STANDARD COMPUTATION]");
		} else {
			FINDFLDSETSTRING(payddms,"COMPUTATION","[STANDARD COMPUTATION]");
		}
		FINDFLDGETSHORT(paydds,"DEDUCTION TYPE",&deduction_type);
		FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","NON-W2");
		switch(deduction_type)
		{
			case 0: /* Not on Pay Stub */
				break;
			case 1: /* Federal */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FEDERAL");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","FEDERAL");
				break;
			case 2: /* Fica */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FICA");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","FICA");
				break;
			case 3: /* State */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","STATE");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","STATE");
				break;
			case 4: /* local */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","LOCAL");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","LOCAL");
				break;
			case 5: /* State Retirement */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","RETIREMENT");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","RETIREMENT");
				break;
			case 6: /* Other Retirement  */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","RETIREMENT");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","RETIREMENT");
				break;
			case 7: /* Matched Retirement */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","RETIREMENT-M");
				break;
			case 8: /* Matched FICA */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FICA-M");
				break;
			case 9: /* Medicare */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","MEDICARE");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","MEDICARE");
				break;
			case 10: /* Matched Medicare  */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","MEDICARE-M");
				break;
			case 11: /* Insurance >50K(org) */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","INS>50K");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","GROUP-TERM>50K");
				break;
			case 12: /* Voluntary */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","VOLUNTARY");
				break;
			case 19: /* Direct Deposit  */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","DIRECT DEPOSIT");
				break;
			case 20: /* Unemployment  */
				break;
			case 21: /* TSA#1 */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","ANNUITY");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","PENSION PLAN");
				break;
			case 22: /* Fringe Benefits #1 */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FRINGE");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","FRINGE BENEFIT");
				break;
			case 23: /* TSA #2 */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","ANNUITY");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","PENSION PLAN");
				break;
			case 24: /* TSA #3 */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","ANNUITY");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","PENSION PLAN");
				break;
			case 25: /* Fringe Benefits #2 */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FRINGE");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","FRINGE BENEFIT");
				break;
			case 26: /* Fringe Benefits #3 */	
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FRINGE");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","FRINGE BENEFIT");
				break;
			case 27: /* NQ 457 Plan  */	
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","457-NQ");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","457NQ");
				break;
			case 28: /* Dependant Care  */	
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","DEPENDENT CARE");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","DEPENDENT CARE");
				break;
			case 29: /* Sick Pay */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","SICK PAY");
				break;
			case 30: /* 401K plan */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","401K");
				break;
			case 31: /* 408k6 Plan */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","408K6");
				break;
			case 32: /* 457 Plan */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","457 DEFERRED");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","457B");
				break;
			case 33: /* 501c18D Plan */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","501C18D");
				break;
			case 34: /* NQ Non 457 Plan */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","457NQ-NON");
				break;
			case 35: /* Business Expenses */
				break;
			case 36: /* Excess Bus. Expenses */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","BUS EXP-NON TAX");
				break;
			case 37: /* Excess Dependant Care */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","DEPENDENT CARE");
				break;
			case 38: /* Earn. Inc. Credit */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FEDERAL");
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","AEIC");
				break;
			case 39: /* Fed by 3rd Party */
				FINDFLDSETSTRING(payddms,"DEDUCTION TYPE","FEDERAL");
				break;
			case 40: /* Uncollected MED */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","GLI MEDI UNCOL");
				break;
			case 41: /* Uncollected SSA */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","GLI SSN UNCOL");
				break;
			case 42: /* Excludable Moving Expenses */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","MOV EXP REIM");
				break;
			case 43: /* Military */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","MILITARY");
				break;
			case 44: /* 408(p) SIMPLE */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","408p SIMPLE");
				break;
			case 45: /* Adoption Benefits */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","ADOPTION BENEF");
				break;
			case 46: /* Medical Saving Account-Employer */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","MSA EMPLOYER");
				break;
			case 47: /* Excess Golden Parachute */
				FINDFLDSETSTRING(payddms,"W2 DEDUCTION TYPE","GOLDEN PARA");
				break;
			default:
				break;
		}


		FINDFLDSETSHORT(payddms,"COMPUTATION TYPE",computation_type);
		if(computation_type==8 || computation_type==9 || computation_type==10)
		{
			FINDFLDGETDOUBLE(paydds,"COMPUTE BENEFIT AMOUNT",&benamt);
			tnum=(int)benamt;
			FINDFLDSETINT(paytbl,"TABLE CODE",tnum);
			if(!EQLNRD(paytbl,1))
			{
				FINDFLDGETSTRING(paytbl,"DESCRIPTION",&temp);
				if(!isEMPTY(temp))
				{
					sprintf(stemp,"\"%s\"",temp);
					FINDFLDSETSTRING(payddms,"TABLE NAME",stemp);
				}
				if(temp!=NULL) Rfree(temp);
			}
		}
		WRTTRANS(payddms,0,NULL,NULL);
	} else {
		FINDFLDGETINT(paydds,"DEDUCTION DESCRIPTION CODE",&dedcode);
		prterr("INFO:  Deduction Description Master for [%d] already existed and will not be created.",dedcode);
	}
	ZERNRD(payddac);
	FINDFLDSETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	COPYFIELD(paydds,payddac,"DEFINITION TYPE");
	if(EQLNRD(payddac,1))
	{
		KEYNRD(payddac,1);
	} else {
		prev=RDATDataNEW(payddac);
	}
	FINDFLDSETCHAR(payddac,"DELETEFLAG",FALSE);
	FINDFLDGETDOUBLE(payddac,"AMOUNT",&amt1);
	amt1+=amt;
	FINDFLDSETDOUBLE(payddac,"AMOUNT",amt1);	
	WRTTRANS(payddac,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);
	if(dedid!=NULL) Rfree(dedid);
}
int main(int argc,char **argv)
{
	short ef=0;
	char delflag=FALSE;

	if(InitializeSubsystems(argc,argv,"CONVERSION","PAYDDS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	paydds=OPNNRD("CONVERSION","PAYDDS");
	paytbl=OPNNRD("CONVERSION","PAYTBL");
	payddms=OPNNRD("PAYROLL","PAYDDMS");
	payddac=OPNNRD("PAYROLL","PAYDDAC");
	if(paydds!=(-1))
	{
		ef=FRSNRD(paydds,1);
		while(!ef)
		{
			FINDFLDGETCHAR(paydds,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				MakePAYDDMS(paydds);
			}
			ef=NXTNRD(paydds,1);
		}
		CLSNRD(paydds);
	}	
	if(paytbl!=(-1)) CLSNRD(paytbl);
	if(payddms!=(-1)) CLSNRD(payddms);
	if(payddac!=(-1)) CLSNRD(payddac);
	ShutdownSubsystems();
	exit(0);
}
