/* venvirt.c - Makes VENPMT Virtual Fields */
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

extern RDAfinmgt *finsetup;
extern char *getmonth(int);

void VENPMTCreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];

	d=DFvirtualNEW("VENPMT","VENDOR ADDRESS 1");
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
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("VENPMT","VENDOR ADDRESS 2");
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
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("VENPMT","VENDOR ADDRESS 3");
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
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","ANNUAL CALENDAR 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[VENPMT][VENYR][JANUARY 1099 AMOUNT]",
		"[VENPMT][VENYR][FEBRUARY 1099 AMOUNT]",
		"[VENPMT][VENYR][MARCH 1099 AMOUNT]",
		"[VENPMT][VENYR][APRIL 1099 AMOUNT]",
		"[VENPMT][VENYR][MAY 1099 AMOUNT]",
		"[VENPMT][VENYR][JUNE 1099 AMOUNT]",
		"[VENPMT][VENYR][JULY 1099 AMOUNT]",
		"[VENPMT][VENYR][AUGUST 1099 AMOUNT]",
		"[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]",
		"[VENPMT][VENYR][OCTOBER 1099 AMOUNT]",
		"[VENPMT][VENYR][NOVEMBER 1099 AMOUNT]",
		"[VENPMT][VENYR][DECEMBER 1099 AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Total 1099 Amount of vouchers Issed to a Vendor that have been distributed in the financials.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","ANNUAL CALENDAR GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[VENPMT][VENYR][JANUARY GROSS AMOUNT]",
		"[VENPMT][VENYR][FEBRUARY GROSS AMOUNT]",
		"[VENPMT][VENYR][MARCH GROSS AMOUNT]",
		"[VENPMT][VENYR][APRIL GROSS AMOUNT]",
		"[VENPMT][VENYR][MAY GROSS AMOUNT]",
		"[VENPMT][VENYR][JUNE GROSS AMOUNT]",
		"[VENPMT][VENYR][JULY GROSS AMOUNT]",
		"[VENPMT][VENYR][AUGUST GROSS AMOUNT]",
		"[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT]",
		"[VENPMT][VENYR][OCTOBER GROSS AMOUNT]",
		"[VENPMT][VENYR][NOVEMBER GROSS AMOUNT]",
		"[VENPMT][VENYR][DECEMBER GROSS AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Total Gross Amount of vouchers Issed to a Vendor that have been distributed in the financials.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","ANNUAL FISCAL 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][PRE-YEAR 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][POST YEAR 1099 AMOUNT]",
		getmonth(finsetup->month),
		getmonth(finsetup->month+1),
		getmonth(finsetup->month+2),
		getmonth(finsetup->month+3),
		getmonth(finsetup->month+4),
		getmonth(finsetup->month+5),
		getmonth(finsetup->month+6),
		getmonth(finsetup->month+7),
		getmonth(finsetup->month+8),
		getmonth(finsetup->month+9),
		getmonth(finsetup->month+10),
		getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fiscal Total 1099 Amount of vouchers Issed to a Vendor that have been distributed in the financials.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","ANNUAL FISCAL GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][PRE-YEAR GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][POST YEAR GROSS AMOUNT]",
		getmonth(finsetup->month),
		getmonth(finsetup->month+1),
		getmonth(finsetup->month+2),
		getmonth(finsetup->month+3),
		getmonth(finsetup->month+4),
		getmonth(finsetup->month+5),
		getmonth(finsetup->month+6),
		getmonth(finsetup->month+7),
		getmonth(finsetup->month+8),
		getmonth(finsetup->month+9),
		getmonth(finsetup->month+10),
		getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fiscal Total Gross Amount of vouchers Issed to a Vendor that have been distributed in the financials.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FIRST CALENDAR QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][JANUARY 1099 AMOUNT]+[VENPMT][VENYR][FEBRUARY 1099 AMOUNT]+[VENPMT][VENYR][MARCH 1099 AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the First Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","SECOND CALENDAR QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][APRIL 1099 AMOUNT]+[VENPMT][VENYR][MAY 1099 AMOUNT]+[VENPMT][VENYR][JUNE 1099 AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the Second Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","THIRD CALENDAR QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][JULY 1099 AMOUNT]+[VENPMT][VENYR][AUGUST 1099 AMOUNT]+[VENPMT][VENYR][SEPTEMBER 1099 AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the Third Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FOURTH CALENDAR QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][OCTOBER 1099 AMOUNT]+[VENPMT][VENYR][NOVEMBER 1099 AMOUNT]+[VENPMT][VENYR][DECEMBER 1099 AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the Fourth Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FIRST CALENDAR QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][JANUARY GROSS AMOUNT]+[VENPMT][VENYR][FEBRUARY GROSS AMOUNT]+[VENPMT][VENYR][MARCH GROSS AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the First Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","SECOND CALENDAR QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][APRIL GROSS AMOUNT]+[VENPMT][VENYR][MAY GROSS AMOUNT]+[VENPMT][VENYR][JUNE GROSS AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the Second Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","THIRD CALENDAR QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][JULY GROSS AMOUNT]+[VENPMT][VENYR][AUGUST GROSS AMOUNT]+[VENPMT][VENYR][SEPTEMBER GROSS AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the Third Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FOURTH CALENDAR QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][OCTOBER GROSS AMOUNT]+[VENPMT][VENYR][NOVEMBER GROSS AMOUNT]+[VENPMT][VENYR][DECEMBER GROSS AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the Fourth Quarter of the Calendar Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FIRST FISCAL QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the First Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","SECOND FISCAL QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the Second Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","THIRD FISCAL QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the Third Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FOURTH FISCAL QUARTER 1099 AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]+[VENPMT][VENFYR][%s 1099 AMOUNT]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the 1099 Amount of Vouchers distributed for the Fourth Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FIRST FISCAL QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the First Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","SECOND FISCAL QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the Second Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","THIRD FISCAL QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the Third Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FOURTH FISCAL QUARTER GROSS AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENFYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]+[VENPMT][VENFYR][%s GROSS AMOUNT]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Gross Amount of Vouchers distributed for the Fourth Quarter of the Fiscal Year to the Vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);
/*
	d=DFvirtualNEW("VENPMT","ANNUAL NUMBER INVOICES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=LONGV;
	d->length=8;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[VENPMT][VENYR][JANUARY NUMBER INVOICES]",
		"[VENPMT][VENYR][FEBRUARY NUMBER INVOICES]",
		"[VENPMT][VENYR][MARCH NUMBER INVOICES]",
		"[VENPMT][VENYR][APRIL NUMBER INVOICES]",
		"[VENPMT][VENYR][MAY NUMBER INVOICES]",
		"[VENPMT][VENYR][JUNE NUMBER INVOICES]",
		"[VENPMT][VENYR][JULY NUMBER INVOICES]",
		"[VENPMT][VENYR][AUGUST NUMBER INVOICES]",
		"[VENPMT][VENYR][SEPTEMBER NUMBER INVOICES]",
		"[VENPMT][VENYR][OCTOBER NUMBER INVOICES]",
		"[VENPMT][VENYR][NOVEMBER NUMBER INVOICES]",
		"[VENPMT][VENYR][DECEMBER NUMBER INVOICES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Number of Invoices Issued to a Vendor that have cleared checks issued for them.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","ANNUAL AMOUNT INVOICES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[VENPMT][VENYR][JANUARY AMOUNT]",
		"[VENPMT][VENYR][FEBRUARY AMOUNT]",
		"[VENPMT][VENYR][MARCH AMOUNT]",
		"[VENPMT][VENYR][APRIL AMOUNT]",
		"[VENPMT][VENYR][MAY AMOUNT]",
		"[VENPMT][VENYR][JUNE AMOUNT]",
		"[VENPMT][VENYR][JULY AMOUNT]",
		"[VENPMT][VENYR][AUGUST AMOUNT]",
		"[VENPMT][VENYR][SEPTEMBER AMOUNT]",
		"[VENPMT][VENYR][OCTOBER AMOUNT]",
		"[VENPMT][VENYR][NOVEMBER AMOUNT]",
		"[VENPMT][VENYR][DECEMBER AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Total Amount Issued to a Vendor that have cleared checks issued for them.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FIRST QUARTER INVOICES AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Quarter total amount of the invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","SECOND QUARTER INVOICES AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Quarter total amount of the invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","THIRD QUARTER INVOICES AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Quarter total amount of the invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FOURTH QUARTER INVOICES AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]+[VENPMT][VENYR][%s AMOUNT]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Quarter total amount of the invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FIRST QUARTER NUMBER INVOICES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=LONGV;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Quarter total number of invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","SECOND QUARTER NUMBER INVOICES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=LONGV;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Quarter total number of invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","THIRD QUARTER NUMBER INVOICES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=LONGV;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Quarter total number of invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("VENPMT","FOURTH QUARTER NUMBER INVOICES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("VENPMT");
	i->file=stralloc("VENYR");
	d->vtype=0;
	d->num=1;
	d->type=LONGV;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]+[VENPMT][VENYR][%s NUMBER INVOICES]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Quarter total number of invoices issued to the vendor.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("VENPMT",d);
	if(d!=NULL) FreeDFvirtual(d);
*/
}
