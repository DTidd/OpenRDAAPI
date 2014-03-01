/* rlstfunc.c - Real Estate Functions */
#include <rlstmgt.hpp>
#include <mix.hpp>

char *TranTypeFields[4]={"TAX","PENALTY","INTEREST","PAID"};

char *xMakeRealEstateField(short trantype,int line,char *file)
{
	char *tmp=NULL;

	tmp=Rmalloc(RDAstrlen(TranTypeFields[trantype])+20);
	sprintf(tmp,"%s AMOUNT",TranTypeFields[trantype]);
	return(tmp);
}

void xUpdateRealEstate(short taxdwr,short rstmst,short rstnew,
	short rstrn,short rstpd,double amount,void (*SubFunc)(...),
	void *args,int line,char *file)
{
	RDATData *prev=NULL;
	double total=0.0;
	int x=0,numflds=0,trantype=0,yearid=0,trans=0,paynum=(-1);
	short pmonth=0,ef=FALSE;
	char *temp=NULL,*realestateid=NULL,*ownerid=NULL,dist=FALSE;
	NRDfield *fields=NULL,*field=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		if(rstpd!=(-1))
		{
			prterr("DIAG UpdateRealEstatePayments at line [%d] program [%s].",line,file);
		} else {
			prterr("DIAG UpdateRealEstateTransactions at line [%d] program [%s].",line,file);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rstpd!=(-1))
	{
		trantype=3;
		FINDFLDGETSHORT(rstpd,"PROCESSING MONTH",&pmonth);
		FINDFLDGETSTRING(rstpd,"REAL ESTATE IDENTIFICATION",&realestateid);
		FINDFLDGETSTRING(rstpd,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDGETINT(rstpd,"TAX YEAR",&yearid);
		FINDFLDGETINT(rstpd,"TRANSACTION NUMBER",&trans);
		FINDFLDGETINT(rstpd,"PAYMENT NUMBER",&paynum);
		FIELDCOPY(rstpd,"ENTRY DATE",rstmst,"LAST PAID DATE");
		FINDFLDSETSTRING(rstrn,"REAL ESTATE IDENTIFICATION",realestateid);
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
			prterr("Error Real Estate Transaction Master not found for Payment Record [%s][%s][%d][%d][%d] at line [%d] program [%s].",realestateid,ownerid,yearid,trans,paynum,line,file);
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
		FINDFLDGETSTRING(rstrn,"REAL ESTATE IDENTIFICATION",&realestateid);
		FINDFLDGETSTRING(rstrn,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDGETINT(rstrn,"TAX YEAR",&yearid);
		FINDFLDGETINT(rstrn,"TRANSACTION TYPE",&trantype);
		FINDFLDGETSHORT(rstrn,"MONTH",&pmonth);
	}
	FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid);
	FINDFLDSETSTRING(rstmst,"OWNER IDENTIFICATION",ownerid);
	FINDFLDSETINT(rstmst,"TAX YEAR",yearid);
	total=0.0;
	LOCNRDFILE(rstmst);
	if(SubFunc!=NULL) ef=ADVEQLNRDsec(rstmst,1,SubFunc,args);
		else ef=EQLNRDsec(rstmst,1);
	if(!ef)
	{
		prev=RDATDataNEW(rstmst);
		if(rstnew!=(-1) && !trantype)
		{
			FINDFLDSETSTRING(rstnew,"REAL ESTATE IDENTIFICATION",
				realestateid);
			FINDFLDSETSTRING(rstnew,"OWNER IDENTIFICATION",ownerid);
			if(SubFunc!=NULL) ef=ADVEQLNRDsec(rstnew,1,SubFunc,args);
				else ef=EQLNRDsec(rstnew,1);
			if(!ef)
			{
				COPYFIELD(rstnew,rstmst,"LAND VALUE");
				COPYFIELD(rstnew,rstmst,"BUILDING AND IMPROVEMENT VALUE");
			}
		}
	} else {
		KEYNRD(rstmst,1);
		prev=NULL;
		if(rstnew!=(-1))
		{
			FINDFLDSETSTRING(rstnew,"REAL ESTATE IDENTIFICATION",
				realestateid);
			FINDFLDSETSTRING(rstnew,"OWNER IDENTIFICATION",ownerid);
			if(SubFunc!=NULL) ef=ADVEQLNRDsec(rstnew,1,SubFunc,args);
				else ef=EQLNRDsec(rstnew,1);
			if(!ef)
			{
				numflds=NUMFLDS(rstnew);
				fields=FLDPOINTER(rstnew);
				if(fields!=NULL)
				{
					for(x=0,field=fields;x<numflds;++x,++field)
					{
						COPYFIELD(rstnew,rstmst,field->name);
					}
				}
				FIELDCOPY(rstnew,"ADDRESS 1",rstmst,"STATEMENT ADDRESS 1");
				FIELDCOPY(rstnew,"ADDRESS 2",rstmst,"STATEMENT ADDRESS 2");
				FIELDCOPY(rstnew,"CITY",rstmst,"STATEMENT CITY");
				FIELDCOPY(rstnew,"STATE",rstmst,"STATEMENT STATE");
				FIELDCOPY(rstnew,"ZIP",rstmst,"STATEMENT ZIP CODE");
			}
		}
	}
	if(rstpd!=(-1)) trantype=3;
	temp=MakeRealEstateField(trantype);
	if(temp!=NULL)
	{
		FINDFLDGETDOUBLE(rstmst,temp,&total);
		total+=amount;
		FINDFLDSETDOUBLE(rstmst,temp,total);
		Rfree(temp);
	}
	if(SubFunc!=NULL) ADVWRTTRANSsec(rstmst,0,NULL,prev,SubFunc,args);
		else WRTTRANSsec(rstmst,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);
	UNLNRDFILE(rstmst);
	if(realestateid!=NULL) Rfree(realestateid);
	if(ownerid!=NULL) Rfree(ownerid);
}
