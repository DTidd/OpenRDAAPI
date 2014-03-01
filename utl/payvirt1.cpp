/* payvirt1.c - Makes PAYROLL Virtual Fields Part 1*/
#include <cstdio>





#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
/*ADDLIB ctpf */

#define TO_DATE

void PAYROLLCreate1(char *modx)
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];

	d=DFvirtualNEW(modx,"PERSONNEL ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([PRSNNL][PERDMG][ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][ADDRESS #2]=\"\") AND ",
		"([PRSNNL][PERDMG][CITY]=\"\") AND ",
		"([PRSNNL][PERDMG][STATE]=\"\") AND ",
		"([PRSNNL][PERDMG][ZIP CODE]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PRSNNL][PERDMG][ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=([PRSNNL][PERDMG][CITY]+\", \"+",
		"[PRSNNL][PERDMG][STATE]+\"  \"+[PRSNNL][PERDMG][ZIP CODE])",
		"ELSE IF([PRSNNL][PERDMG][ADDRESS #1]=\"\")",
		"THEN RETURN_VALUE=[PRSNNL][PERDMG][ADDRESS #2]",
		"ELSE RETURN_VALUE=[PRSNNL][PERDMG][ADDRESS #1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([PRSNNL][PERDMG][ADDRESS #1]=\"\")",
		"AND ([PRSNNL][PERDMG][ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PRSNNL][PERDMG][ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][CITY]=\"\") AND",
		"([PRSNNL][PERDMG][STATE]=\"\") AND",
		"([PRSNNL][PERDMG][ZIP CODE]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([PRSNNL][PERDMG][ADDRESS #2]=\"\")",
		"THEN RETURN_VALUE=([PRSNNL][PERDMG][CITY]+\", \"+",
		"[PRSNNL][PERDMG][STATE]+\"  \"+[PRSNNL][PERDMG][ZIP CODE])",
		"ELSE RETURN_VALUE=[PRSNNL][PERDMG][ADDRESS #2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([PRSNNL][PERDMG][ADDRESS #1]=\"\")",
		"OR ([PRSNNL][PERDMG][ADDRESS #2]=\"\")",
		"OR (([PRSNNL][PERDMG][CITY]=\"\")AND",
		"([PRSNNL][PERDMG][STATE]=\"\") AND",
		"([PRSNNL][PERDMG][ZIP CODE]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([PRSNNL][PERDMG][CITY]+\", \"+",
		"[PRSNNL][PERDMG][STATE]+\"  \"+[PRSNNL][PERDMG][ZIP CODE])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"PERSONNEL MAILING ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\") AND ",
		"([PRSNNL][PERDMG][MAILING CITY]=\"\") AND ",
		"([PRSNNL][PERDMG][MAILING STATE]=\"\") AND ",
		"([PRSNNL][PERDMG][MAILING ZIP CODE]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=([PRSNNL][PERDMG][MAILING CITY]+\", \"+",
		"[PRSNNL][PERDMG][MAILING STATE]+\"  \"+[PRSNNL][PERDMG][MAILING ZIP CODE])",
		"ELSE IF([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\")",
		"THEN RETURN_VALUE=[PRSNNL][PERDMG][MAILING ADDRESS #2]",
		"ELSE RETURN_VALUE=[PRSNNL][PERDMG][MAILING ADDRESS #1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Mailing Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL MAILING ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\")",
		"AND ([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][MAILING CITY]=\"\") AND",
		"([PRSNNL][PERDMG][MAILING STATE]=\"\") AND",
		"([PRSNNL][PERDMG][MAILING ZIP CODE]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\")",
		"THEN RETURN_VALUE=([PRSNNL][PERDMG][MAILING CITY]+\", \"+",
		"[PRSNNL][PERDMG][MAILING STATE]+\"  \"+[PRSNNL][PERDMG][MAILING ZIP CODE])",
		"ELSE RETURN_VALUE=[PRSNNL][PERDMG][MAILING ADDRESS #2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Mailing Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL MAILING ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\")",
		"OR ([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\")",
		"OR (([PRSNNL][PERDMG][MAILING CITY]=\"\")AND",
		"([PRSNNL][PERDMG][MAILING STATE]=\"\") AND",
		"([PRSNNL][PERDMG][MAILING ZIP CODE]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([PRSNNL][PERDMG][MAILING CITY]+\", \"+",
		"[PRSNNL][PERDMG][MAILING STATE]+\"  \"+[PRSNNL][PERDMG][MAILING ZIP CODE])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Mailing Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"PERSONNEL EMERGENCY ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([PRSNNL][PERDMG][EMERGENCY ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][EMERGENCY ADDRESS #2]=\"\") AND ",
		"([PRSNNL][PERDMG][EMERGENCY CITY]=\"\") AND ",
		"([PRSNNL][PERDMG][EMERGENCY STATE]=\"\") AND ",
		"([PRSNNL][PERDMG][EMERGENCY ZIP CODE]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PRSNNL][PERDMG][EMERGENCY ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][EMERGENCY ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=([PRSNNL][PERDMG][EMERGENCY CITY]+\", \"+",
		"[PRSNNL][PERDMG][EMERGENCY STATE]+\"  \"+[PRSNNL][PERDMG][EMERGENCY ZIP CODE])",
		"ELSE IF([PRSNNL][PERDMG][EMERGENCY ADDRESS #1]=\"\")",
		"THEN RETURN_VALUE=[PRSNNL][PERDMG][EMERGENCY ADDRESS #2]",
		"ELSE RETURN_VALUE=[PRSNNL][PERDMG][EMERGENCY ADDRESS #1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Emergency Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL EMERGENCY ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([PRSNNL][PERDMG][EMERGENCY ADDRESS #1]=\"\")",
		"AND ([PRSNNL][PERDMG][EMERGENCY ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PRSNNL][PERDMG][EMERGENCY ADDRESS #1]=\"\") AND",
		"([PRSNNL][PERDMG][EMERGENCY CITY]=\"\") AND",
		"([PRSNNL][PERDMG][EMERGENCY STATE]=\"\") AND",
		"([PRSNNL][PERDMG][EMERGENCY ZIP CODE]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([PRSNNL][PERDMG][EMERGENCY ADDRESS #2]=\"\")",
		"THEN RETURN_VALUE=([PRSNNL][PERDMG][EMERGENCY CITY]+\", \"+",
		"[PRSNNL][PERDMG][EMERGENCY STATE]+\"  \"+[PRSNNL][PERDMG][EMERGENCY ZIP CODE])",
		"ELSE RETURN_VALUE=[PRSNNL][PERDMG][EMERGENCY ADDRESS #2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Emergency Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL EMERGENCY ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PRSNNL");
	i->file=stralloc("PERDMG");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([PRSNNL][PERDMG][EMERGENCY ADDRESS #1]=\"\")",
		"OR ([PRSNNL][PERDMG][EMERGENCY ADDRESS #2]=\"\")",
		"OR (([PRSNNL][PERDMG][EMERGENCY CITY]=\"\")AND",
		"([PRSNNL][PERDMG][EMERGENCY STATE]=\"\") AND",
		"([PRSNNL][PERDMG][EMERGENCY ZIP CODE]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([PRSNNL][PERDMG][EMERGENCY CITY]+\", \"+",
		"[PRSNNL][PERDMG][EMERGENCY STATE]+\"  \"+[PRSNNL][PERDMG][EMERGENCY ZIP CODE])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Personnel's Emergency Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"LOCATION ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("POSTRK");
	i->file=stralloc("POSLMST");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([POSTRK][POSLMST][ADDRESS 1]=\"\") AND",
		"([POSTRK][POSLMST][ADDRESS 2]=\"\") AND ",
		"([POSTRK][POSLMST][CITY]=\"\") AND ",
		"([POSTRK][POSLMST][STATE]=\"\") AND ",
		"([POSTRK][POSLMST][ZIP]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([POSTRK][POSLMST][ADDRESS 1]=\"\") AND",
		"([POSTRK][POSLMST][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=([POSTRK][POSLMST][CITY]+\", \"+",
		"[POSTRK][POSLMST][STATE]+\"  \"+[POSTRK][POSLMST][ZIP])",
		"ELSE IF([POSTRK][POSLMST][ADDRESS 1]=\"\")",
		"THEN RETURN_VALUE=[POSTRK][POSLMST][ADDRESS 2]",
		"ELSE RETURN_VALUE=[POSTRK][POSLMST][ADDRESS 1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Location's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"LOCATION ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("POSTRK");
	i->file=stralloc("POSLMST");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([POSTRK][POSLMST][ADDRESS 1]=\"\")",
		"AND ([POSTRK][POSLMST][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([POSTRK][POSLMST][ADDRESS 1]=\"\") AND",
		"([POSTRK][POSLMST][CITY]=\"\") AND",
		"([POSTRK][POSLMST][STATE]=\"\") AND",
		"([POSTRK][POSLMST][ZIP]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([POSTRK][POSLMST][ADDRESS 2]=\"\")",
		"THEN RETURN_VALUE=([POSTRK][POSLMST][CITY]+\", \"+",
		"[POSTRK][POSLMST][STATE]+\"  \"+[POSTRK][POSLMST][ZIP])",
		"ELSE RETURN_VALUE=[POSTRK][POSLMST][ADDRESS 2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Location's Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"LOCATION ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("POSTRK");
	i->file=stralloc("POSLMST");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([POSTRK][POSLMST][ADDRESS 1]=\"\")",
		"OR ([POSTRK][POSLMST][ADDRESS 2]=\"\")",
		"OR (([POSTRK][POSLMST][CITY]=\"\")AND",
		"([POSTRK][POSLMST][STATE]=\"\") AND",
		"([POSTRK][POSLMST][ZIP]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([POSTRK][POSLMST][CITY]+\", \"+",
		"[POSTRK][POSLMST][STATE]+\"  \"+[POSTRK][POSLMST][ZIP])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Location's Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"INSURANCE PROVIDER ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("IIDMGT");
	i->file=stralloc("PERMED");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #1]=\"\") AND",
		"([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #2]=\"\") AND ",
		"([IIDMGT][PERMED][INSURANCE PROVIDER CITY]=\"\") AND ",
		"([IIDMGT][PERMED][INSURANCE PROVIDER STATE]=\"\") AND ",
		"([IIDMGT][PERMED][INSURANCE PROVIDER ZIP]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #1]=\"\") AND",
		"([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=([IIDMGT][PERMED][INSURANCE PROVIDER CITY]+\", \"+",
		"[IIDMGT][PERMED][INSURANCE PROVIDER STATE]+\"  \"+[IIDMGT][PERMED][INSURANCE PROVIDER ZIP])",
		"ELSE IF([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #1]=\"\")",
		"THEN RETURN_VALUE=[IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #2]",
		"ELSE RETURN_VALUE=[IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Insurance Provider's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"INSURANCE PROVIDER ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("IIDMGT");
	i->file=stralloc("PERMED");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #1]=\"\")",
		"AND ([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #1]=\"\") AND",
		"([IIDMGT][PERMED][INSURANCE PROVIDER CITY]=\"\") AND",
		"([IIDMGT][PERMED][INSURANCE PROVIDER STATE]=\"\") AND",
		"([IIDMGT][PERMED][INSURANCE PROVIDER ZIP]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #2]=\"\")",
		"THEN RETURN_VALUE=([IIDMGT][PERMED][INSURANCE PROVIDER CITY]+\", \"+",
		"[IIDMGT][PERMED][INSURANCE PROVIDER STATE]+\"  \"+[IIDMGT][PERMED][INSURANCE PROVIDER ZIP])",
		"ELSE RETURN_VALUE=[IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Insurance Provider's Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"INSURANCE PROVIDER ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("IIDMGT");
	i->file=stralloc("PERMED");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #1]=\"\")",
		"OR ([IIDMGT][PERMED][INSURANCE PROVIDER ADDRESS #2]=\"\")",
		"OR (([IIDMGT][PERMED][INSURANCE PROVIDER CITY]=\"\")AND",
		"([IIDMGT][PERMED][INSURANCE PROVIDER STATE]=\"\") AND",
		"([IIDMGT][PERMED][INSURANCE PROVIDER ZIP]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([IIDMGT][PERMED][INSURANCE PROVIDER CITY]+\", \"+",
		"[IIDMGT][PERMED][INSURANCE PROVIDER STATE]+\"  \"+[IIDMGT][PERMED][INSURANCE PROVIDER ZIP])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Insurance Provider's Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"PERSONNEL REFERENCE ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("APPMGT");
	i->file=stralloc("PERMSTR");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([APPMGT][PERMSTR][ADDRESS 1]=\"\") AND",
		"([APPMGT][PERMSTR][ADDRESS 2]=\"\") AND ",
		"([APPMGT][PERMSTR][CITY]=\"\") AND ",
		"([APPMGT][PERMSTR][STATE]=\"\") AND ",
		"([APPMGT][PERMSTR][ZIP]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([APPMGT][PERMSTR][ADDRESS 1]=\"\") AND",
		"([APPMGT][PERMSTR][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=([APPMGT][PERMSTR][CITY]+\", \"+",
		"[APPMGT][PERMSTR][STATE]+\"  \"+[APPMGT][PERMSTR][ZIP])",
		"ELSE IF([APPMGT][PERMSTR][ADDRESS 1]=\"\")",
		"THEN RETURN_VALUE=[APPMGT][PERMSTR][ADDRESS 2]",
		"ELSE RETURN_VALUE=[APPMGT][PERMSTR][ADDRESS 1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Reference's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL REFERENCE ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("APPMGT");
	i->file=stralloc("PERMSTR");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([APPMGT][PERMSTR][ADDRESS 1]=\"\")",
		"AND ([APPMGT][PERMSTR][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([APPMGT][PERMSTR][ADDRESS 1]=\"\") AND",
		"([APPMGT][PERMSTR][CITY]=\"\") AND",
		"([APPMGT][PERMSTR][STATE]=\"\") AND",
		"([APPMGT][PERMSTR][ZIP]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([APPMGT][PERMSTR][ADDRESS 2]=\"\")",
		"THEN RETURN_VALUE=([APPMGT][PERMSTR][CITY]+\", \"+",
		"[APPMGT][PERMSTR][STATE]+\"  \"+[APPMGT][PERMSTR][ZIP])",
		"ELSE RETURN_VALUE=[APPMGT][PERMSTR][ADDRESS 2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Reference's Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"PERSONNEL REFERENCE ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("APPMGT");
	i->file=stralloc("PERMSTR");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([APPMGT][PERMSTR][ADDRESS 1]=\"\")",
		"OR ([APPMGT][PERMSTR][ADDRESS 2]=\"\")",
		"OR (([APPMGT][PERMSTR][CITY]=\"\")AND",
		"([APPMGT][PERMSTR][STATE]=\"\") AND",
		"([APPMGT][PERMSTR][ZIP]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([APPMGT][PERMSTR][CITY]+\", \"+",
		"[APPMGT][PERMSTR][STATE]+\"  \"+[APPMGT][PERMSTR][ZIP])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Reference's Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"COURSE ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PERDEV");
	i->file=stralloc("CRSREMAS");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([PERDEV][CRSREMAS][ADDRESS 1]=\"\") AND",
		"([PERDEV][CRSREMAS][ADDRESS 2]=\"\") AND ",
		"([PERDEV][CRSREMAS][CITY]=\"\") AND ",
		"([PERDEV][CRSREMAS][STATE]=\"\") AND ",
		"([PERDEV][CRSREMAS][ZIP]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PERDEV][CRSREMAS][ADDRESS 1]=\"\") AND",
		"([PERDEV][CRSREMAS][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=([PERDEV][CRSREMAS][CITY]+\", \"+",
		"[PERDEV][CRSREMAS][STATE]+\"  \"+[PERDEV][CRSREMAS][ZIP])",
		"ELSE IF([PERDEV][CRSREMAS][ADDRESS 1]=\"\")",
		"THEN RETURN_VALUE=[PERDEV][CRSREMAS][ADDRESS 2]",
		"ELSE RETURN_VALUE=[PERDEV][CRSREMAS][ADDRESS 1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Course's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"COURSE ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PERDEV");
	i->file=stralloc("CRSREMAS");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([PERDEV][CRSREMAS][ADDRESS 1]=\"\")",
		"AND ([PERDEV][CRSREMAS][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PERDEV][CRSREMAS][ADDRESS 1]=\"\") AND",
		"([PERDEV][CRSREMAS][CITY]=\"\") AND",
		"([PERDEV][CRSREMAS][STATE]=\"\") AND",
		"([PERDEV][CRSREMAS][ZIP]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([PERDEV][CRSREMAS][ADDRESS 2]=\"\")",
		"THEN RETURN_VALUE=([PERDEV][CRSREMAS][CITY]+\", \"+",
		"[PERDEV][CRSREMAS][STATE]+\"  \"+[PERDEV][CRSREMAS][ZIP])",
		"ELSE RETURN_VALUE=[PERDEV][CRSREMAS][ADDRESS 2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Course's Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"COURSE ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PERDEV");
	i->file=stralloc("CRSREMAS");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([PERDEV][CRSREMAS][ADDRESS 1]=\"\")",
		"OR ([PERDEV][CRSREMAS][ADDRESS 2]=\"\")",
		"OR (([PERDEV][CRSREMAS][CITY]=\"\")AND",
		"([PERDEV][CRSREMAS][STATE]=\"\") AND",
		"([PERDEV][CRSREMAS][ZIP]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([PERDEV][CRSREMAS][CITY]+\", \"+",
		"[PERDEV][CRSREMAS][STATE]+\"  \"+[PERDEV][CRSREMAS][ZIP])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Course's Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"VENDOR ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENADR");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([VENPMT][VENADR][ADDRESS 1]=\"\") AND",
		"([VENPMT][VENADR][ADDRESS 2]=\"\") AND ",
		"([VENPMT][VENADR][CITY]=\"\") AND ",
		"([VENPMT][VENADR][STATE]=\"\") AND ",
		"([VENPMT][VENADR][ZIP]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([VENPMT][VENADR][ADDRESS 1]=\"\") AND",
		"([VENPMT][VENADR][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=([VENPMT][VENADR][CITY]+\", \"+",
		"[VENPMT][VENADR][STATE]+\"  \"+[VENPMT][VENADR][ZIP])",
		"ELSE IF([VENPMT][VENADR][ADDRESS 1]=\"\")",
		"THEN RETURN_VALUE=[VENPMT][VENADR][ADDRESS 2]",
		"ELSE RETURN_VALUE=[VENPMT][VENADR][ADDRESS 1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Vendor's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"VENDOR ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENADR");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([VENPMT][VENADR][ADDRESS 1]=\"\")",
		"AND ([VENPMT][VENADR][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([VENPMT][VENADR][ADDRESS 1]=\"\") AND",
		"([VENPMT][VENADR][CITY]=\"\") AND",
		"([VENPMT][VENADR][STATE]=\"\") AND",
		"([VENPMT][VENADR][ZIP]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([VENPMT][VENADR][ADDRESS 2]=\"\")",
		"THEN RETURN_VALUE=([VENPMT][VENADR][CITY]+\", \"+",
		"[VENPMT][VENADR][STATE]+\"  \"+[VENPMT][VENADR][ZIP])",
		"ELSE RETURN_VALUE=[VENPMT][VENADR][ADDRESS 2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Vendor's Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"VENDOR ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENADR");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([VENPMT][VENADR][ADDRESS 1]=\"\")",
		"OR ([VENPMT][VENADR][ADDRESS 2]=\"\")",
		"OR (([VENPMT][VENADR][CITY]=\"\")AND",
		"([VENPMT][VENADR][STATE]=\"\") AND",
		"([VENPMT][VENADR][ZIP]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([VENPMT][VENADR][CITY]+\", \"+",
		"[VENPMT][VENADR][STATE]+\"  \"+[VENPMT][VENADR][ZIP])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Vendor's Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
#ifdef TO_DATE
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL DEDUCTION TOTAL TO DATE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PAYROLL");
	i->file=stralloc("PAYDCSM");
	i->keyname=NULL;
	++i;
	i->module=stralloc("PAYROLL");
	i->file=stralloc("PAYMSTR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
	"IF ([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT]) ",
	" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Personnel Deduction Amount for the Calendar Year from January to the Present Month which is calculated by the sum of the Monthly amounts from the Calendar Year Beginning up to the Month according to the [PAYROLL][PAYMSTR][MONTH] field.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PAYROLL");
	i->file=stralloc("PAYPCSM");
	i->keyname=NULL;
	++i;
	i->module=stralloc("PAYROLL");
	i->file=stralloc("PAYMSTR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
	"IF ([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS]+[PAYROLL][PAYPCSM][DECEMBER GROSS]) ",
	" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Personnel Gross for the Calendar Year from January to the Present Month which is calculated by the sum of the Monthly gross amounts from the Calendar Year Beginning up to the Month according to the [PAYROLL][PAYMSTR][MONTH] field.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL UNITS TOTAL TO DATE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PAYROLL");
	i->file=stralloc("PAYPCSM");
	i->keyname=NULL;
	++i;
	i->module=stralloc("PAYROLL");
	i->file=stralloc("PAYMSTR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
	"IF ([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]+[PAYROLL][PAYPCSM][JUNE UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]+[PAYROLL][PAYPCSM][JUNE UNITS]+[PAYROLL][PAYPCSM][JULY UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]+[PAYROLL][PAYPCSM][JUNE UNITS]+[PAYROLL][PAYPCSM][JULY UNITS]+[PAYROLL][PAYPCSM][AUGUST UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]+[PAYROLL][PAYPCSM][JUNE UNITS]+[PAYROLL][PAYPCSM][JULY UNITS]+[PAYROLL][PAYPCSM][AUGUST UNITS]+[PAYROLL][PAYPCSM][SEPTEMBER UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]+[PAYROLL][PAYPCSM][JUNE UNITS]+[PAYROLL][PAYPCSM][JULY UNITS]+[PAYROLL][PAYPCSM][AUGUST UNITS]+[PAYROLL][PAYPCSM][SEPTEMBER UNITS]+[PAYROLL][PAYPCSM][OCTOBER UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]+[PAYROLL][PAYPCSM][JUNE UNITS]+[PAYROLL][PAYPCSM][JULY UNITS]+[PAYROLL][PAYPCSM][AUGUST UNITS]+[PAYROLL][PAYPCSM][SEPTEMBER UNITS]+[PAYROLL][PAYPCSM][OCTOBER UNITS]+[PAYROLL][PAYPCSM][NOVEMBER UNITS]) ",
	"\nELSE IF ([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY UNITS]+[PAYROLL][PAYPCSM][FEBRUARY UNITS]+[PAYROLL][PAYPCSM][MARCH UNITS]+[PAYROLL][PAYPCSM][APRIL UNITS]+[PAYROLL][PAYPCSM][MAY UNITS]+[PAYROLL][PAYPCSM][JUNE UNITS]+[PAYROLL][PAYPCSM][JULY UNITS]+[PAYROLL][PAYPCSM][AUGUST UNITS]+[PAYROLL][PAYPCSM][SEPTEMBER UNITS]+[PAYROLL][PAYPCSM][OCTOBER UNITS]+[PAYROLL][PAYPCSM][NOVEMBER UNITS]+[PAYROLL][PAYPCSM][DECEMBER UNITS]) ",
	" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Personnel Units for the Calendar Year from January to the Present Month which is calculated by the sum of the Monthly gross amounts from the Calendar Year Beginning up to the Current Month according to the [PAYROLL][PAYMSTR][MONTH] field.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
}
