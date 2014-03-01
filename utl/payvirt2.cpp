/* payvirt2.c - Makes PAYROLL Virtual Fields Part 2*/
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

void PAYROLLCreate2(char *modx)
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYPFSM][PRE-YEAR GROSS]",
		"[PAYROLL][PAYPFSM][JANUARY GROSS]",
		"[PAYROLL][PAYPFSM][FEBRUARY GROSS]",
		"[PAYROLL][PAYPFSM][MARCH GROSS]",
		"[PAYROLL][PAYPFSM][APRIL GROSS]",
		"[PAYROLL][PAYPFSM][MAY GROSS]",
		"[PAYROLL][PAYPFSM][JUNE GROSS]",
		"[PAYROLL][PAYPFSM][JULY GROSS]",
		"[PAYROLL][PAYPFSM][AUGUST GROSS]",
		"[PAYROLL][PAYPFSM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYPFSM][OCTOBER GROSS]",
		"[PAYROLL][PAYPFSM][NOVEMBER GROSS]",
		"[PAYROLL][PAYPFSM][DECEMBER GROSS]",
		"[PAYROLL][PAYPFSM][POST YEAR GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][PRE-YEAR GROSS]+[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Total Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL NET");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][PRE-YEAR NET]+[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Total Net Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Total Gross Amount Issued to an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL NET");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Total Net Amount Issued to an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Total Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL NET");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Total Net Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][%s GROSS]+[PAYROLL][PAYPFSM][POST YEAR GROSS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Total Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL NET");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][%s NET]+[PAYROLL][PAYPFSM][POST YEAR NET]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Total Net Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYPCSM][JANUARY GROSS]",
		"[PAYROLL][PAYPCSM][FEBRUARY GROSS]",
		"[PAYROLL][PAYPCSM][MARCH GROSS]",
		"[PAYROLL][PAYPCSM][APRIL GROSS]",
		"[PAYROLL][PAYPCSM][MAY GROSS]",
		"[PAYROLL][PAYPCSM][JUNE GROSS]",
		"[PAYROLL][PAYPCSM][JULY GROSS]",
		"[PAYROLL][PAYPCSM][AUGUST GROSS]",
		"[PAYROLL][PAYPCSM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYPCSM][OCTOBER GROSS]",
		"[PAYROLL][PAYPCSM][NOVEMBER GROSS]",
		"[PAYROLL][PAYPCSM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Gross Total Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][JANUARY GROSS]",
			"[PAYROLL][PAYPCSM][FEBRUARY GROSS]",
			"[PAYROLL][PAYPCSM][MARCH GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Total Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][APRIL GROSS]",
			"[PAYROLL][PAYPCSM][MAY GROSS]",
			"[PAYROLL][PAYPCSM][JUNE GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Total Gross Amount Issued to an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][JULY GROSS]",
			"[PAYROLL][PAYPCSM][AUGUST GROSS]",
			"[PAYROLL][PAYPCSM][SEPTEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Total Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][OCTOBER GROSS]",
			"[PAYROLL][PAYPCSM][NOVEMBER GROSS]",
			"[PAYROLL][PAYPCSM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Total Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYPFSM][PRE-YEAR UNITS]",
		"[PAYROLL][PAYPFSM][JANUARY UNITS]",
		"[PAYROLL][PAYPFSM][FEBRUARY UNITS]",
		"[PAYROLL][PAYPFSM][MARCH UNITS]",
		"[PAYROLL][PAYPFSM][APRIL UNITS]",
		"[PAYROLL][PAYPFSM][MAY UNITS]",
		"[PAYROLL][PAYPFSM][JUNE UNITS]",
		"[PAYROLL][PAYPFSM][JULY UNITS]",
		"[PAYROLL][PAYPFSM][AUGUST UNITS]",
		"[PAYROLL][PAYPFSM][SEPTEMBER UNITS]",
		"[PAYROLL][PAYPFSM][OCTOBER UNITS]",
		"[PAYROLL][PAYPFSM][NOVEMBER UNITS]",
		"[PAYROLL][PAYPFSM][DECEMBER UNITS]",
		"[PAYROLL][PAYPFSM][POST YEAR UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][PRE-YEAR UNITS]+[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Total Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Total Units contributed by an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Total Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][%s UNITS]+[PAYROLL][PAYPFSM][POST YEAR UNITS",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Total Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYPCSM][JANUARY UNITS]",
		"[PAYROLL][PAYPCSM][FEBRUARY UNITS]",
		"[PAYROLL][PAYPCSM][MARCH UNITS]",
		"[PAYROLL][PAYPCSM][APRIL UNITS]",
		"[PAYROLL][PAYPCSM][MAY UNITS]",
		"[PAYROLL][PAYPCSM][JUNE UNITS]",
		"[PAYROLL][PAYPCSM][JULY UNITS]",
		"[PAYROLL][PAYPCSM][AUGUST UNITS]",
		"[PAYROLL][PAYPCSM][SEPTEMBER UNITS]",
		"[PAYROLL][PAYPCSM][OCTOBER UNITS]",
		"[PAYROLL][PAYPCSM][NOVEMBER UNITS]",
		"[PAYROLL][PAYPCSM][DECEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Total Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][JANUARY UNITS]",
			"[PAYROLL][PAYPCSM][FEBRUARY UNITS]",
			"[PAYROLL][PAYPCSM][MARCH UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Total Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][APRIL UNITS]",
			"[PAYROLL][PAYPCSM][MAY UNITS]",
			"[PAYROLL][PAYPCSM][JUNE UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Total Units contributed by an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][JULY UNITS]",
			"[PAYROLL][PAYPCSM][AUGUST UNITS]",
			"[PAYROLL][PAYPCSM][SEPTEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Total contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYPCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYPCSM][OCTOBER UNITS]",
			"[PAYROLL][PAYPCSM][NOVEMBER UNITS]",
			"[PAYROLL][PAYPCSM][DECEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Total Gross Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJFSM][PRE-YEAR GROSS]",
		"[PAYROLL][PAYJFSM][JANUARY GROSS]",
		"[PAYROLL][PAYJFSM][FEBRUARY GROSS]",
		"[PAYROLL][PAYJFSM][MARCH GROSS]",
		"[PAYROLL][PAYJFSM][APRIL GROSS]",
		"[PAYROLL][PAYJFSM][MAY GROSS]",
		"[PAYROLL][PAYJFSM][JUNE GROSS]",
		"[PAYROLL][PAYJFSM][JULY GROSS]",
		"[PAYROLL][PAYJFSM][AUGUST GROSS]",
		"[PAYROLL][PAYJFSM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYJFSM][OCTOBER GROSS]",
		"[PAYROLL][PAYJFSM][NOVEMBER GROSS]",
		"[PAYROLL][PAYJFSM][DECEMBER GROSS]",
		"[PAYROLL][PAYJFSM][POST YEAR GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][PRE-YEAR GROSS]+[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Gross Job Amount Issued to an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][%s GROSS]+[PAYROLL][PAYJFSM][POST YEAR GROSS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJCSM][JANUARY GROSS]",
		"[PAYROLL][PAYJCSM][FEBRUARY GROSS]",
		"[PAYROLL][PAYJCSM][MARCH GROSS]",
		"[PAYROLL][PAYJCSM][APRIL GROSS]",
		"[PAYROLL][PAYJCSM][MAY GROSS]",
		"[PAYROLL][PAYJCSM][JUNE GROSS]",
		"[PAYROLL][PAYJCSM][JULY GROSS]",
		"[PAYROLL][PAYJCSM][AUGUST GROSS]",
		"[PAYROLL][PAYJCSM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYJCSM][OCTOBER GROSS]",
		"[PAYROLL][PAYJCSM][NOVEMBER GROSS]",
		"[PAYROLL][PAYJCSM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][JANUARY GROSS]",
			"[PAYROLL][PAYJCSM][FEBRUARY GROSS]",
			"[PAYROLL][PAYJCSM][MARCH GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][APRIL GROSS]",
			"[PAYROLL][PAYJCSM][MAY GROSS]",
			"[PAYROLL][PAYJCSM][JUNE GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Gross Job Amount Issued to an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][JULY GROSS]",
			"[PAYROLL][PAYJCSM][AUGUST GROSS]",
			"[PAYROLL][PAYJCSM][SEPTEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL JOB GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][OCTOBER GROSS]",
			"[PAYROLL][PAYJCSM][NOVEMBER GROSS]",
			"[PAYROLL][PAYJCSM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Gross Job Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJFSM][PRE-YEAR UNITS]",
		"[PAYROLL][PAYJFSM][JANUARY UNITS]",
		"[PAYROLL][PAYJFSM][FEBRUARY UNITS]",
		"[PAYROLL][PAYJFSM][MARCH UNITS]",
		"[PAYROLL][PAYJFSM][APRIL UNITS]",
		"[PAYROLL][PAYJFSM][MAY UNITS]",
		"[PAYROLL][PAYJFSM][JUNE UNITS]",
		"[PAYROLL][PAYJFSM][JULY UNITS]",
		"[PAYROLL][PAYJFSM][AUGUST UNITS]",
		"[PAYROLL][PAYJFSM][SEPTEMBER UNITS]",
		"[PAYROLL][PAYJFSM][OCTOBER UNITS]",
		"[PAYROLL][PAYJFSM][NOVEMBER UNITS]",
		"[PAYROLL][PAYJFSM][DECEMBER UNITS]",
		"[PAYROLL][PAYJFSM][POST YEAR UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][PRE-YEAR UNITS]+[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Job Units contributed by an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJFSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][%s UNITS]+[PAYROLL][PAYJFSM][POST YEAR UNITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJCSM][JANUARY UNITS]",
		"[PAYROLL][PAYJCSM][FEBRUARY UNITS]",
		"[PAYROLL][PAYJCSM][MARCH UNITS]",
		"[PAYROLL][PAYJCSM][APRIL UNITS]",
		"[PAYROLL][PAYJCSM][MAY UNITS]",
		"[PAYROLL][PAYJCSM][JUNE UNITS]",
		"[PAYROLL][PAYJCSM][JULY UNITS]",
		"[PAYROLL][PAYJCSM][AUGUST UNITS]",
		"[PAYROLL][PAYJCSM][SEPTEMBER UNITS]",
		"[PAYROLL][PAYJCSM][OCTOBER UNITS]",
		"[PAYROLL][PAYJCSM][NOVEMBER UNITS]",
		"[PAYROLL][PAYJCSM][DECEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][JANUARY UNITS]",
			"[PAYROLL][PAYJCSM][FEBRUARY UNITS]",
			"[PAYROLL][PAYJCSM][MARCH UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][APRIL UNITS]",
			"[PAYROLL][PAYJCSM][MAY UNITS]",
			"[PAYROLL][PAYJCSM][JUNE UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Job Units contributed by an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][JULY UNITS]",
			"[PAYROLL][PAYJCSM][AUGUST UNITS]",
			"[PAYROLL][PAYJCSM][SEPTEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL JOB UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJCSM");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJCSM][OCTOBER UNITS]",
			"[PAYROLL][PAYJCSM][NOVEMBER UNITS]",
			"[PAYROLL][PAYJCSM][DECEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Job Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJRFS][PRE-YEAR GROSS]",
		"[PAYROLL][PAYJRFS][JANUARY GROSS]",
		"[PAYROLL][PAYJRFS][FEBRUARY GROSS]",
		"[PAYROLL][PAYJRFS][MARCH GROSS]",
		"[PAYROLL][PAYJRFS][APRIL GROSS]",
		"[PAYROLL][PAYJRFS][MAY GROSS]",
		"[PAYROLL][PAYJRFS][JUNE GROSS]",
		"[PAYROLL][PAYJRFS][JULY GROSS]",
		"[PAYROLL][PAYJRFS][AUGUST GROSS]",
		"[PAYROLL][PAYJRFS][SEPTEMBER GROSS]",
		"[PAYROLL][PAYJRFS][OCTOBER GROSS]",
		"[PAYROLL][PAYJRFS][NOVEMBER GROSS]",
		"[PAYROLL][PAYJRFS][DECEMBER GROSS]",
		"[PAYROLL][PAYJRFS][POST YEAR GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Gross Job Rate Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][PRE-YEAR GROSS]+[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Gross Job Rate Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Gross Job Rate Amount Issued to an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Gross Job Rate Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][%s GROSS]+[PAYROLL][PAYJRFS][POST YEAR GROSS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRFS][APRIL GROSS]",
			"[PAYROLL][PAYJRFS][MAY GROSS]",
			"[PAYROLL][PAYJRFS][JUNE GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Gross Job Rate Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJRCS][JANUARY GROSS]",
		"[PAYROLL][PAYJRCS][FEBRUARY GROSS]",
		"[PAYROLL][PAYJRCS][MARCH GROSS]",
		"[PAYROLL][PAYJRCS][APRIL GROSS]",
		"[PAYROLL][PAYJRCS][MAY GROSS]",
		"[PAYROLL][PAYJRCS][JUNE GROSS]",
		"[PAYROLL][PAYJRCS][JULY GROSS]",
		"[PAYROLL][PAYJRCS][AUGUST GROSS]",
		"[PAYROLL][PAYJRCS][SEPTEMBER GROSS]",
		"[PAYROLL][PAYJRCS][OCTOBER GROSS]",
		"[PAYROLL][PAYJRCS][NOVEMBER GROSS]",
		"[PAYROLL][PAYJRCS][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Gross Job Rate Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][JANUARY GROSS]",
			"[PAYROLL][PAYJRCS][FEBRUARY GROSS]",
			"[PAYROLL][PAYJRCS][MARCH GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Gross Job Rate Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][APRIL GROSS]",
			"[PAYROLL][PAYJRCS][MAY GROSS]",
			"[PAYROLL][PAYJRCS][JUNE GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Gross Job Rate Amount Issued to an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][JULY GROSS]",
			"[PAYROLL][PAYJRCS][AUGUST GROSS]",
			"[PAYROLL][PAYJRCS][SEPTEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Gross Job Rate  Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL JOB RATE GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][OCTOBER GROSS]",
			"[PAYROLL][PAYJRCS][NOVEMBER GROSS]",
			"[PAYROLL][PAYJRCS][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Gross Job Rate Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJRFS][PRE-YEAR UNITS]",
		"[PAYROLL][PAYJRFS][JANUARY UNITS]",
		"[PAYROLL][PAYJRFS][FEBRUARY UNITS]",
		"[PAYROLL][PAYJRFS][MARCH UNITS]",
		"[PAYROLL][PAYJRFS][APRIL UNITS]",
		"[PAYROLL][PAYJRFS][MAY UNITS]",
		"[PAYROLL][PAYJRFS][JUNE UNITS]",
		"[PAYROLL][PAYJRFS][JULY UNITS]",
		"[PAYROLL][PAYJRFS][AUGUST UNITS]",
		"[PAYROLL][PAYJRFS][SEPTEMBER UNITS]",
		"[PAYROLL][PAYJRFS][OCTOBER UNITS]",
		"[PAYROLL][PAYJRFS][NOVEMBER UNITS]",
		"[PAYROLL][PAYJRFS][DECEMBER UNITS]",
		"[PAYROLL][PAYJRFS][POST YEAR UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Job Rate Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][PRE-YEAR UNITS]+[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Job Rate Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Job Rate Units contributed by an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Job Rate Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRFS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][%s UNITS]+[PAYROLL][PAYJRFS][POST YEAR UNITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRFS][APRIL UNITS]",
			"[PAYROLL][PAYJRFS][MAY UNITS]",
			"[PAYROLL][PAYJRFS][JUNE UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Job Rate Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJRCS][JANUARY UNITS]",
		"[PAYROLL][PAYJRCS][FEBRUARY UNITS]",
		"[PAYROLL][PAYJRCS][MARCH UNITS]",
		"[PAYROLL][PAYJRCS][APRIL UNITS]",
		"[PAYROLL][PAYJRCS][MAY UNITS]",
		"[PAYROLL][PAYJRCS][JUNE UNITS]",
		"[PAYROLL][PAYJRCS][JULY UNITS]",
		"[PAYROLL][PAYJRCS][AUGUST UNITS]",
		"[PAYROLL][PAYJRCS][SEPTEMBER UNITS]",
		"[PAYROLL][PAYJRCS][OCTOBER UNITS]",
		"[PAYROLL][PAYJRCS][NOVEMBER UNITS]",
		"[PAYROLL][PAYJRCS][DECEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Job Rate Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][JANUARY UNITS]",
			"[PAYROLL][PAYJRCS][FEBRUARY UNITS]",
			"[PAYROLL][PAYJRCS][MARCH UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Job Rate Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][APRIL UNITS]",
			"[PAYROLL][PAYJRCS][MAY UNITS]",
			"[PAYROLL][PAYJRCS][JUNE UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Job Rate Units contributed by an Individual Personnel");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][JULY UNITS]",
			"[PAYROLL][PAYJRCS][AUGUST UNITS]",
			"[PAYROLL][PAYJRCS][SEPTEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Gross Job Rate  Amount Issued to an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL JOB RATE UNITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJRCS");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=12;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYJRCS][OCTOBER UNITS]",
			"[PAYROLL][PAYJRCS][NOVEMBER UNITS]",
			"[PAYROLL][PAYJRCS][DECEMBER UNITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Job Rate Units contributed by an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYDFSM][PRE-YEAR GROSS]",
		"[PAYROLL][PAYDFSM][JANUARY GROSS]",
		"[PAYROLL][PAYDFSM][FEBRUARY GROSS]",
		"[PAYROLL][PAYDFSM][MARCH GROSS]",
		"[PAYROLL][PAYDFSM][APRIL GROSS]",
		"[PAYROLL][PAYDFSM][MAY GROSS]",
		"[PAYROLL][PAYDFSM][JUNE GROSS]",
		"[PAYROLL][PAYDFSM][JULY GROSS]",
		"[PAYROLL][PAYDFSM][AUGUST GROSS]",
		"[PAYROLL][PAYDFSM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYDFSM][OCTOBER GROSS]",
		"[PAYROLL][PAYDFSM][NOVEMBER GROSS]",
		"[PAYROLL][PAYDFSM][DECEMBER GROSS]",
		"[PAYROLL][PAYDFSM][POST YEAR GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Gross Amount used in calculating the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][PRE-YEAR GROSS]+[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][%s GROSS]+[PAYROLL][PAYDFSM][POST YEAR GROSS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYDFSM][PRE-YEAR AMOUNT]",
		"[PAYROLL][PAYDFSM][JANUARY AMOUNT]",
		"[PAYROLL][PAYDFSM][FEBRUARY AMOUNT]",
		"[PAYROLL][PAYDFSM][MARCH AMOUNT]",
		"[PAYROLL][PAYDFSM][APRIL AMOUNT]",
		"[PAYROLL][PAYDFSM][MAY AMOUNT]",
		"[PAYROLL][PAYDFSM][JUNE AMOUNT]",
		"[PAYROLL][PAYDFSM][JULY AMOUNT]",
		"[PAYROLL][PAYDFSM][AUGUST AMOUNT]",
		"[PAYROLL][PAYDFSM][SEPTEMBER AMOUNT]",
		"[PAYROLL][PAYDFSM][OCTOBER AMOUNT]",
		"[PAYROLL][PAYDFSM][NOVEMBER AMOUNT]",
		"[PAYROLL][PAYDFSM][DECEMBER AMOUNT]",
		"[PAYROLL][PAYDFSM][POST YEAR AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][PRE-YEAR AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][%s AMOUNT]+[PAYROLL][PAYDFSM][POST YEAR AMOUNT]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYDCSM][JANUARY GROSS]",
		"[PAYROLL][PAYDCSM][FEBRUARY GROSS]",
		"[PAYROLL][PAYDCSM][MARCH GROSS]",
		"[PAYROLL][PAYDCSM][APRIL GROSS]",
		"[PAYROLL][PAYDCSM][MAY GROSS]",
		"[PAYROLL][PAYDCSM][JUNE GROSS]",
		"[PAYROLL][PAYDCSM][JULY GROSS]",
		"[PAYROLL][PAYDCSM][AUGUST GROSS]",
		"[PAYROLL][PAYDCSM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYDCSM][OCTOBER GROSS]",
		"[PAYROLL][PAYDCSM][NOVEMBER GROSS]",
		"[PAYROLL][PAYDCSM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Gross Amount used in calculating the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][JANUARY GROSS]",
			"[PAYROLL][PAYDCSM][FEBRUARY GROSS]",
			"[PAYROLL][PAYDCSM][MARCH GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][APRIL GROSS]",
			"[PAYROLL][PAYDCSM][MAY GROSS]",
			"[PAYROLL][PAYDCSM][JUNE GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][JULY GROSS]",
			"[PAYROLL][PAYDCSM][AUGUST GROSS]",
			"[PAYROLL][PAYDCSM][SEPTEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][OCTOBER GROSS]",
			"[PAYROLL][PAYDCSM][NOVEMBER GROSS]",
			"[PAYROLL][PAYDCSM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ANNUAL CALENDAR PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYDCSM][JANUARY AMOUNT]",
		"[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]",
		"[PAYROLL][PAYDCSM][MARCH AMOUNT]",
		"[PAYROLL][PAYDCSM][APRIL AMOUNT]",
		"[PAYROLL][PAYDCSM][MAY AMOUNT]",
		"[PAYROLL][PAYDCSM][JUNE AMOUNT]",
		"[PAYROLL][PAYDCSM][JULY AMOUNT]",
		"[PAYROLL][PAYDCSM][AUGUST AMOUNT]",
		"[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]",
		"[PAYROLL][PAYDCSM][OCTOBER AMOUNT]",
		"[PAYROLL][PAYDCSM][NOVEMBER AMOUNT]",
		"[PAYROLL][PAYDCSM][DECEMBER AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][JANUARY AMOUNT]",
			"[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]",
			"[PAYROLL][PAYDCSM][MARCH AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][APRIL AMOUNT]",
			"[PAYROLL][PAYDCSM][MAY AMOUNT]",
			"[PAYROLL][PAYDCSM][JUNE AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][JULY AMOUNT]",
			"[PAYROLL][PAYDCSM][AUGUST AMOUNT]",
			"[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDCSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
			"[PAYROLL][PAYDCSM][OCTOBER AMOUNT]",
			"[PAYROLL][PAYDCSM][NOVEMBER AMOUNT]",
			"[PAYROLL][PAYDCSM][DECEMBER AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Deduction Amount for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJDFM][PRE-YEAR GROSS]",
		"[PAYROLL][PAYJDFM][JANUARY GROSS]",
		"[PAYROLL][PAYJDFM][FEBRUARY GROSS]",
		"[PAYROLL][PAYJDFM][MARCH GROSS]",
		"[PAYROLL][PAYJDFM][APRIL GROSS]",
		"[PAYROLL][PAYJDFM][MAY GROSS]",
		"[PAYROLL][PAYJDFM][JUNE GROSS]",
		"[PAYROLL][PAYJDFM][JULY GROSS]",
		"[PAYROLL][PAYJDFM][AUGUST GROSS]",
		"[PAYROLL][PAYJDFM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYJDFM][OCTOBER GROSS]",
		"[PAYROLL][PAYJDFM][NOVEMBER GROSS]",
		"[PAYROLL][PAYJDFM][DECEMBER GROSS]",
		"[PAYROLL][PAYJDFM][POST YEAR GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Gross Amount for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][PRE-YEAR GROSS]+[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][%s GROSS]+[PAYROLL][PAYJDFM][POST YEAR GROSS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJDFM][PRE-YEAR AMOUNT]",
		"[PAYROLL][PAYJDFM][JANUARY AMOUNT]",
		"[PAYROLL][PAYJDFM][FEBRUARY AMOUNT]",
		"[PAYROLL][PAYJDFM][MARCH AMOUNT]",
		"[PAYROLL][PAYJDFM][APRIL AMOUNT]",
		"[PAYROLL][PAYJDFM][MAY AMOUNT]",
		"[PAYROLL][PAYJDFM][JUNE AMOUNT]",
		"[PAYROLL][PAYJDFM][JULY AMOUNT]",
		"[PAYROLL][PAYJDFM][AUGUST AMOUNT]",
		"[PAYROLL][PAYJDFM][SEPTEMBER AMOUNT]",
		"[PAYROLL][PAYJDFM][OCTOBER AMOUNT]",
		"[PAYROLL][PAYJDFM][NOVEMBER AMOUNT]",
		"[PAYROLL][PAYJDFM][DECEMBER AMOUNT]",
		"[PAYROLL][PAYJDFM][POST YEAR AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Deduction Amount for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][PRE-YEAR AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][%s AMOUNT]+[PAYROLL][PAYJDFM][POST YEAR AMOUNT]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL CALENDAR JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJDCM][JANUARY GROSS]",
		"[PAYROLL][PAYJDCM][FEBRUARY GROSS]",
		"[PAYROLL][PAYJDCM][MARCH GROSS]",
		"[PAYROLL][PAYJDCM][APRIL GROSS]",
		"[PAYROLL][PAYJDCM][MAY GROSS]",
		"[PAYROLL][PAYJDCM][JUNE GROSS]",
		"[PAYROLL][PAYJDCM][JULY GROSS]",
		"[PAYROLL][PAYJDCM][AUGUST GROSS]",
		"[PAYROLL][PAYJDCM][SEPTEMBER GROSS]",
		"[PAYROLL][PAYJDCM][OCTOBER GROSS]",
		"[PAYROLL][PAYJDCM][NOVEMBER GROSS]",
		"[PAYROLL][PAYJDCM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Gross Amount for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][JANUARY GROSS]+[PAYROLL][PAYJDCM][FEBRUARY GROSS]+[PAYROLL][PAYJDCM][MARCH GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][APRIL GROSS]+[PAYROLL][PAYJDCM][MAY GROSS]+[PAYROLL][PAYJDCM][JUNE GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][JULY GROSS]+[PAYROLL][PAYJDCM][AUGUST GROSS]+[PAYROLL][PAYJDCM][SEPTEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR JOB DEDUCTION GROSS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][OCTOBER GROSS]+[PAYROLL][PAYJDCM][NOVEMBER GROSS]+[PAYROLL][PAYJDCM][DECEMBER GROSS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Gross Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL CALENDAR JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJDCM][JANUARY AMOUNT]",
		"[PAYROLL][PAYJDCM][FEBRUARY AMOUNT]",
		"[PAYROLL][PAYJDCM][MARCH AMOUNT]",
		"[PAYROLL][PAYJDCM][APRIL AMOUNT]",
		"[PAYROLL][PAYJDCM][MAY AMOUNT]",
		"[PAYROLL][PAYJDCM][JUNE AMOUNT]",
		"[PAYROLL][PAYJDCM][JULY AMOUNT]",
		"[PAYROLL][PAYJDCM][AUGUST AMOUNT]",
		"[PAYROLL][PAYJDCM][SEPTEMBER AMOUNT]",
		"[PAYROLL][PAYJDCM][OCTOBER AMOUNT]",
		"[PAYROLL][PAYJDCM][NOVEMBER AMOUNT]",
		"[PAYROLL][PAYJDCM][DECEMBER AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Calendar Deduction Amount for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER CALENDAR JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][JANUARY AMOUNT]+[PAYROLL][PAYJDCM][FEBRUARY AMOUNT]+[PAYROLL][PAYJDCM][MARCH AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Calendar Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER CALENDAR JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][APRIL AMOUNT]+[PAYROLL][PAYJDCM][MAY AMOUNT]+[PAYROLL][PAYJDCM][JUNE AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Calendar Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER CALENDAR JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][JULY AMOUNT]+[PAYROLL][PAYJDCM][AUGUST AMOUNT]+[PAYROLL][PAYJDCM][SEPTEMBER AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Calendar Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER CALENDAR JOB DEDUCTION AMOUNT");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDCM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDCM][OCTOBER AMOUNT]+[PAYROLL][PAYJDCM][NOVEMBER AMOUNT]+[PAYROLL][PAYJDCM][DECEMBER AMOUNT]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Calendar Quarter Deduction Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL PERSONNEL DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYDFSM][PRE-YEAR SHELTERED]",
		"[PAYROLL][PAYDFSM][JANUARY SHELTERED]",
		"[PAYROLL][PAYDFSM][FEBRUARY SHELTERED]",
		"[PAYROLL][PAYDFSM][MARCH SHELTERED]",
		"[PAYROLL][PAYDFSM][APRIL SHELTERED]",
		"[PAYROLL][PAYDFSM][MAY SHELTERED]",
		"[PAYROLL][PAYDFSM][JUNE SHELTERED]",
		"[PAYROLL][PAYDFSM][JULY SHELTERED]",
		"[PAYROLL][PAYDFSM][AUGUST SHELTERED]",
		"[PAYROLL][PAYDFSM][SEPTEMBER SHELTERED]",
		"[PAYROLL][PAYDFSM][OCTOBER SHELTERED]",
		"[PAYROLL][PAYDFSM][NOVEMBER SHELTERED]",
		"[PAYROLL][PAYDFSM][DECEMBER SHELTERED]",
		"[PAYROLL][PAYDFSM][POST YEAR SHELTERED]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Sheltered Amount used in calculating the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL PERSONNEL DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][PRE-YEAR SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL PERSONNEL DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL PERSONNEL DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL PERSONNEL DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYDFSM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][%s SHELTERED]+[PAYROLL][PAYDFSM][POST YEAR SHELTERED]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual Personnel.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ANNUAL FISCAL JOB DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[PAYROLL][PAYJDFM][PRE-YEAR SHELTERED]",
		"[PAYROLL][PAYJDFM][JANUARY SHELTERED]",
		"[PAYROLL][PAYJDFM][FEBRUARY SHELTERED]",
		"[PAYROLL][PAYJDFM][MARCH SHELTERED]",
		"[PAYROLL][PAYJDFM][APRIL SHELTERED]",
		"[PAYROLL][PAYJDFM][MAY SHELTERED]",
		"[PAYROLL][PAYJDFM][JUNE SHELTERED]",
		"[PAYROLL][PAYJDFM][JULY SHELTERED]",
		"[PAYROLL][PAYJDFM][AUGUST SHELTERED]",
		"[PAYROLL][PAYJDFM][SEPTEMBER SHELTERED]",
		"[PAYROLL][PAYJDFM][OCTOBER SHELTERED]",
		"[PAYROLL][PAYJDFM][NOVEMBER SHELTERED]",
		"[PAYROLL][PAYJDFM][DECEMBER SHELTERED]",
		"[PAYROLL][PAYJDFM][POST YEAR SHELTERED]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Annual Fiscal Sheltered Amount for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FIRST QUARTER FISCAL JOB DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][PRE-YEAR SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the First Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"SECOND QUARTER FISCAL JOB DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Second Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"THIRD QUARTER FISCAL JOB DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Third Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"FOURTH QUARTER FISCAL JOB DEDUCTION SHELTERED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=stralloc("PAYJDFM");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	memset(temp,0,15000);
	sprintf(temp,"[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][%s SHELTERED]+[PAYROLL][PAYJDFM][POST YEAR SHELTERED]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Fourth Fiscal Quarter Sheltered Amount used for calculation of the Deduction for an Individual's Job.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
}
