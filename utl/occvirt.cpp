/* occvirt.c - Makes OCCTAX Virtual Fields */
#include <cstdio>





#include <rptgen.hpp>
#include <mkvir.hpp>

void OCCTAXCreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];
	int field_length=0;


	d=DFvirtualNEW("OCCTAX","OWNER NAME");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("OCCTAX");
	i->file=stralloc("OCCBUS");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("OCCTAX","OCCBUS","OWNER NAME");
	if(field_length>0)
	{
		d->type=PLAINTEXT;
		d->length=field_length;
	} else {
		d->type=VARIABLETEXT;
		d->length=0;
	}
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s",
		"[OCCTAX][OCCBUS][OWNER NAME]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field reflects the Owner's Name.  Thi",
		"s virtual field is used to place this value in the",
		" BROWSE LIST.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("OCCTAX",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("OCCTAX","RECEIPT TYPE STRING");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("OCCTAX");
	i->file=stralloc("OCCREC");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=30;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"IF([OCCTAX][OCCREC][RECEIPT TYPE]=0) THEN RETURN_VALUE=\"ADMINISTRATIVE FEE PAYMENT\" \n",
		"ELSE IF([OCCTAX][OCCREC][RECEIPT TYPE]=1) THEN RETURN_VALUE=\"LATE CHARGE PAYMENT\" \n",  
		"ELSE IF([OCCTAX][OCCREC][RECEIPT TYPE]=2) THEN RETURN_VALUE=\"PENALTY PAYMENT\" \n",  
		"ELSE IF([OCCTAX][OCCREC][RECEIPT TYPE]=3) THEN RETURN_VALUE=\"TAX PAYMENT\" \n",  
		"ELSE IF([OCCTAX][OCCREC][RECEIPT TYPE]=4) THEN RETURN_VALUE=\"REGULATORY FEE PAYMENT\" \n", 
		"ELSE RETURN_VALUE=\"Unknown Reciept Type:\"+(INTEGER_TO_STRING([OCCTAX][OCCREC][RECEIPT TYPE])) \n",
		"FI FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field returns the string value for a numerical Receipt",
		" Type field.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("OCCTAX",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("OCCTAX","CURRENT YEAR BALANCE DUE AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("OCCTAX");
	i->file=stralloc("OCCCHG");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([OCCTAX][OCCCHG][TAX AMOUNT]+",
		"[OCCTAX][OCCCHG][ADMINISTRATIVE FEE]+",
		"[OCCTAX][OCCCHG][REGULATORY FEE]+",
		"[OCCTAX][OCCCHG][PENALTY AMOUNT]+",
		"[OCCTAX][OCCCHG][LATE CHARGE AMOUNT])-",
		"([OCCTAX][OCCCHG][TAX PAID]+",
		"[OCCTAX][OCCCHG][ADMINISTRATIVE PAID]+",
		"[OCCTAX][OCCCHG][REGULATORY PAID]+",
		"[OCCTAX][OCCCHG][PENALTY PAID]+",
		"[OCCTAX][OCCCHG][LATE CHARGE PAID])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the remainingb balance",
		" amount for a Business.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("OCCTAX",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("OCCTAX","GROSS RECEIPTS");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("OCCTAX");
	i->file=stralloc("OCCCHG");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"IF([OCCTAX][OCCCHG][ACTUAL GROSS RECEIPTS]>0) ", 
		"THEN RETURN_VALUE=[OCCTAX][OCCCHG][ACTUAL GROSS RECEIPTS] ", 
		"ELSE RETURN_VALUE=[OCCTAX][OCCCHG][ESTIMATED GROSS RECEIPTS] ",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to calculate the Gross Receipts, ",
		"it chooses the Actual Gross Receipts if they are greater-than ",
		"(>) Zero (0) else it chooses the Estimated Gross Receipts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("OCCTAX",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("OCCTAX","NUMBER OF EMPLOYEES");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("OCCTAX");
	i->file=stralloc("OCCCHG");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=LONGV;
	d->length=10;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"IF([OCCTAX][OCCCHG][ACTUAL NUMBER OF EMPLOYEES]>0) ", 
		"THEN RETURN_VALUE=[OCCTAX][OCCCHG][ACTUAL NUMBER OF EMPLOYEES] ", 
		"ELSE RETURN_VALUE=[OCCTAX][OCCCHG][ESTIMATED NUMBER OF EMPLOYEES] ",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to calculate the Number of Employees, ",
		"it chooses the Actual Number of Employees if they are greater-than ",
		"(>) Zero (0) else it chooses the Estimated Number of Employees.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("OCCTAX",d);
	if(d!=NULL) FreeDFvirtual(d);
}
