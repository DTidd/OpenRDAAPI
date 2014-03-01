/* purvirt.c - Makes PURORD Virtual Fields */
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

void PURORDCreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];

	d=DFvirtualNEW("PURORD","SITE ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PURORD");
	i->file=stralloc("PURSIT");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
		"IF(([PURORD][PURSIT][ADDRESS 1]=\"\") AND",
		"([PURORD][PURSIT][ADDRESS 2]=\"\") AND ",
		"([PURORD][PURSIT][CITY]=\"\") AND ",
		"([PURORD][PURSIT][STATE]=\"\") AND ",
		"([PURORD][PURSIT][ZIP]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PURORD][PURSIT][ADDRESS 1]=\"\") AND",
		"([PURORD][PURSIT][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=([PURORD][PURSIT][CITY]+\", \"+",
		"[PURORD][PURSIT][STATE]+\"  \"+[PURORD][PURSIT][ZIP])",
		"ELSE IF([PURORD][PURSIT][ADDRESS 1]=\"\")",
		"THEN RETURN_VALUE=[PURORD][PURSIT][ADDRESS 2]",
		"ELSE RETURN_VALUE=[PURORD][PURSIT][ADDRESS 1]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Site Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PURORD",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("PURORD","SITE ADDRESS 2");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PURORD");
	i->file=stralloc("PURSIT");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
		"IF(([PURORD][PURSIT][ADDRESS 1]=\"\")",
		"AND ([PURORD][PURSIT][ADDRESS 2]=\"\"))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE IF(([PURORD][PURSIT][ADDRESS 1]=\"\") AND",
		"([PURORD][PURSIT][CITY]=\"\") AND",
		"([PURORD][PURSIT][STATE]=\"\") AND",
		"([PURORD][PURSIT][ZIP]=\"\"))",
		"THEN RETURN_VALUE=(\"\")",
		"ELSE IF([PURORD][PURSIT][ADDRESS 2]=\"\")",
		"THEN RETURN_VALUE=([PURORD][PURSIT][CITY]+\", \"+",
		"[PURORD][PURSIT][STATE]+\"  \"+[PURORD][PURSIT][ZIP])",
		"ELSE RETURN_VALUE=[PURORD][PURSIT][ADDRESS 2]",
		"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Site Address line 2.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PURORD",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("PURORD","SITE ADDRESS 3");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PURORD");
	i->file=stralloc("PURSIT");
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s%s %s",
		"IF(([PURORD][PURSIT][ADDRESS 1]=\"\")",
		"OR ([PURORD][PURSIT][ADDRESS 2]=\"\")",
		"OR (([PURORD][PURSIT][CITY]=\"\")AND",
		"([PURORD][PURSIT][STATE]=\"\") AND",
		"([PURORD][PURSIT][ZIP]=\"\")))",
		"THEN RETURN_VALUE=\"\"",
		"ELSE RETURN_VALUE=([PURORD][PURSIT][CITY]+\", \"+",
		"[PURORD][PURSIT][STATE]+\"  \"+[PURORD][PURSIT][ZIP])",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Site Address line 3.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PURORD",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("PURORD","UNIT AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PURORD");
	i->file=stralloc("PURLIN");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"([PURORD][PURLIN][UNIT RATE]*100)");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field Calculates the Unit Dollar Amount by 100xUnit Rate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PURORD",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PURORD","VENDOR ADDRESS 1");
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
	writeDFvirtual("PURORD",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("PURORD","VENDOR ADDRESS 2");
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
	writeDFvirtual("PURORD",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("PURORD","VENDOR ADDRESS 3");
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
	writeDFvirtual("PURORD",d);
	if(d!=NULL) FreeDFvirtual(d);
}
