/* miscbill.c - Misc Billing Functions */
#include <miscbill.hpp>

char *BillTypeFields[4]={"RATE","PENALTY","INTEREST","PAID"};

char *xMakeMiscBillingField(short trantype,int line,char *file)
{
	char *tmp=NULL;

	tmp=Rmalloc(RDAstrlen(BillTypeFields[trantype])+20);
	sprintf(tmp,"%s AMOUNT",BillTypeFields[trantype]);
	return(tmp);
}

void xUpdateMiscBilling(short taxdwr,short rstmst,short rstrn,short rstpd,
	double xunits,double amount,void (*SubFunc)(...),
	void *args,int line,char *file)
{
	RDATData *prev=NULL;
	double total=0.0,total_units=0.0;
	int trantype=0,yearid=0,trans=0,paynum=(-1);
	short pmonth=0,ef=FALSE;
	char *temp=NULL,*mbid=NULL,*classid=NULL,*ownerid=NULL,dist=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		if(rstpd!=(-1))
		{
			prterr("DIAG UpdateMiscBillingPayments at line [%d] program [%s].",line,file);
		} else {
			prterr("DIAG UpdateMiscBillingTransactions at line [%d] program [%s].",line,file);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rstpd!=(-1))
	{
		trantype=3;
		FINDFLDGETSHORT(rstpd,"PROCESSING MONTH",&pmonth);
		FINDFLDGETSTRING(rstpd,"MISC BILLING IDENTIFICATION",&mbid);
		FINDFLDGETSTRING(rstpd,"CLASS IDENTIFICATION",&classid);
		FINDFLDGETSTRING(rstpd,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDGETINT(rstpd,"TAX YEAR",&yearid);
		FINDFLDGETINT(rstpd,"TRANSACTION NUMBER",&trans);
		FINDFLDGETINT(rstpd,"PAYMENT NUMBER",&paynum);
		FIELDCOPY(rstpd,"ENTRY DATE",rstmst,"LAST PAID DATE");
		FINDFLDSETSTRING(rstrn,"MISC BILLING IDENTIFICATION",mbid);
		FINDFLDSETSTRING(rstrn,"CLASS IDENTIFICATION",classid);
		FINDFLDSETSTRING(rstrn,"OWNER IDENTIFICATION",ownerid);
		FINDFLDSETINT(rstrn,"TAX YEAR",yearid);
		FINDFLDSETINT(rstrn,"TRANSACTION NUMBER",trans);
		LOCNRDFILE(rstrn);
		if(SubFunc!=NULL) ef=ADVEQLNRDsec(rstrn,1,SubFunc,args);
			else ef=EQLNRDsec(rstrn,1);
		if(!ef)
		{
			prev=RDATDataNEW(rstrn);
			FINDFLDGETDOUBLE(rstrn,"OUTSTANDING AMOUNT",&total);
			total-=amount;
			FINDFLDSETDOUBLE(rstrn,"OUTSTANDING AMOUNT",total);
			if(SubFunc!=NULL) ADVWRTTRANSsec(rstrn,0,NULL,prev,SubFunc,args);
				else WRTTRANSsec(rstrn,0,NULL,prev);
			if(prev!=NULL) FreeRDATData(prev);
		} else {
			prterr("Error Misc Billing Transaction Master not found for Payment Record [%s][%s][%s][%d][%d][%d] at line [%d] program [%s].",classid,mbid,ownerid,yearid,trans,paynum,line,file);
		}
		UNLNRDFILE(rstrn);
		FINDFLDGETCHAR(rstpd,"CASHED OUT",&dist);
		FINDFLDGETSTRING(rstpd,"CASH DRAWER",&temp);
		if(!dist && !isEMPTY(temp))
		{
			if(taxdwr!=(-1))
			{
				FINDFLDSETSTRING(taxdwr,"CASH DRAWER",temp);
				LOCNRDFILE(taxdwr);
				if(SubFunc!=NULL) ef=ADVEQLNRDsec(taxdwr,1,SubFunc,args);
					else ef=EQLNRDsec(taxdwr,1);
				if(!ef)
				{
					prev=RDATDataNEW(taxdwr);
					FINDFLDGETDOUBLE(taxdwr,"COLLECTED AMOUNT",&total);
					total=total+amount;
					FINDFLDSETDOUBLE(taxdwr,"COLLECTED AMOUNT",total);
					if(SubFunc!=NULL) ADVWRTTRANSsec(taxdwr,0,NULL,prev,SubFunc,args);
						else WRTTRANSsec(taxdwr,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				}
				UNLNRDFILE(taxdwr);
			}
		}
	} else {
		FINDFLDGETSTRING(rstrn,"MISC BILLING IDENTIFICATION",&mbid);
		FINDFLDGETSTRING(rstrn,"CLASS IDENTIFICATION",&classid);
		FINDFLDGETSTRING(rstrn,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDGETINT(rstrn,"TAX YEAR",&yearid);
		FINDFLDGETINT(rstrn,"TRANSACTION TYPE",&trantype);
		FINDFLDGETSHORT(rstrn,"MONTH",&pmonth);
	}
	FINDFLDSETSTRING(rstmst,"MISC BILLING IDENTIFICATION",mbid);
	FINDFLDSETSTRING(rstmst,"CLASS IDENTIFICATION",classid);
	FINDFLDSETSTRING(rstmst,"OWNER IDENTIFICATION",ownerid);
	FINDFLDSETINT(rstmst,"TAX YEAR",yearid);
	total=0.0;
	LOCNRDFILE(rstmst);
	if(SubFunc!=NULL) ef=ADVEQLNRDsec(rstmst,1,SubFunc,args);
		else ef=EQLNRDsec(rstmst,1);
	if(!ef)
	{
		prev=RDATDataNEW(rstmst);
	} else {
		KEYNRD(rstmst,1);
		prev=NULL;
	}
	if(rstpd!=(-1)) trantype=3;
	temp=MakeMiscBillingField(trantype);
	if(temp!=NULL)
	{
		FINDFLDGETDOUBLE(rstmst,temp,&total);
		FINDFLDGETDOUBLE(rstmst,"UNIT AMOUNT",&total_units);
		total+=amount;
		total_units+=xunits;
		FINDFLDSETDOUBLE(rstmst,temp,total);
		FINDFLDSETDOUBLE(rstmst,"UNIT AMOUNT",total_units);
		Rfree(temp);
	}
	if(SubFunc!=NULL) ADVWRTTRANSsec(rstmst,0,NULL,prev,SubFunc,args);
		else WRTTRANSsec(rstmst,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);
	UNLNRDFILE(rstmst);
	if(mbid!=NULL) Rfree(mbid);
	if(ownerid!=NULL) Rfree(ownerid);
	if(classid!=NULL) Rfree(classid);
}
