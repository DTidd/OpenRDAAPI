/* finvirt4.c - Makes FINMGT BALANCE 2 Virtual Fields */
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

#define POSTED_TO_DATE
#define DISTRIBUTED_TO_DATE
#define UNDISTRIBUTED_TO_DATE
#define QUARTER_POSTED
#define QUARTER_DISTRIBUTED
#define QUARTER_UNDISTRIBUTED

extern RDAfinmgt *finsetup;
extern char *getmonth(int);
extern char *makepretodateexpres(char *,char *,char *);

void FINMGTCreateBV2()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];

#ifdef TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][JANUARY DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to January which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to February which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]",
		"[FINMGT][FINBYR][MARCH DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to March which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]",
		"[FINMGT][FINBYR][MARCH DEBITS]",
		"[FINMGT][FINBYR][APRIL DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to April which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]",
		"[FINMGT][FINBYR][MARCH DEBITS]",
		"[FINMGT][FINBYR][APRIL DEBITS]",
		"[FINMGT][FINBYR][MAY DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to May which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]",
		"[FINMGT][FINBYR][MARCH DEBITS]",
		"[FINMGT][FINBYR][APRIL DEBITS]",
		"[FINMGT][FINBYR][MAY DEBITS]",
		"[FINMGT][FINBYR][JUNE DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to June which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to July which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to August which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to September which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to October which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to November which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to December which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to January which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to February which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]",
		"[FINMGT][FINBYR][MARCH CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to March which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]",
		"[FINMGT][FINBYR][MARCH CREDITS]",
		"[FINMGT][FINBYR][APRIL CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to April which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]",
		"[FINMGT][FINBYR][MARCH CREDITS]",
		"[FINMGT][FINBYR][APRIL CREDITS]",
		"[FINMGT][FINBYR][MAY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to May which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]",
		"[FINMGT][FINBYR][MARCH CREDITS]",
		"[FINMGT][FINBYR][APRIL CREDITS]",
		"[FINMGT][FINBYR][MAY CREDITS]",
		"[FINMGT][FINBYR][JUNE CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to June which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to July which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to August which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to September which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to October which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to November which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to December which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef POSTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINBYR","DEBITS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted debits Fiscal Year to Present Month which is calculated by the sum of the posted debits from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINBYR","CREDITS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted credits Fiscal Year to Present Month which is calculated by the sum of the posted credits from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef DISTRIBUTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED DEBITS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINBYR","DISTRIBUTED DEBITS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed debits Fiscal Year to Present Month which is calculated by the sum of the distributed debits from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED CREDITS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINBYR","DISTRIBUTED CREDITS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed credits Fiscal Year to Present Month which is calculated by the sum of the posted distributed from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef UNDISTRIBUTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED DEBITS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINBYR","UNDISTRIBUTED DEBITS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed debits Fiscal Year to Present Month which is calculated by the sum of the undistributed debits from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED CREDITS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINBYR","UNDISTRIBUTED CREDITS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed credits Fiscal Year to Present Month which is calculated by the sum of the posted undistributed from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
	d=DFvirtualNEW("FINMGT","CURRENT MONTH POSTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([FINMGT][FINBYR][JANUARY DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([FINMGT][FINBYR][FEBRUARY DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([FINMGT][FINBYR][MARCH DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([FINMGT][FINBYR][APRIL DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([FINMGT][FINBYR][MAY DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([FINMGT][FINBYR][JUNE DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([FINMGT][FINBYR][JULY DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([FINMGT][FINBYR][AUGUST DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([FINMGT][FINBYR][SEPTEMBER DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([FINMGT][FINBYR][OCTOBER DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([FINMGT][FINBYR][NOVEMBER DEBITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([FINMGT][FINBYR][DECEMBER DEBITS]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the current month posted debits which is calculated by the posted debits of the Current Month according to the CURRENT FISCAL MONTH.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","CURRENT MONTH POSTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([FINMGT][FINBYR][JANUARY CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([FINMGT][FINBYR][FEBRUARY CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([FINMGT][FINBYR][MARCH CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([FINMGT][FINBYR][APRIL CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([FINMGT][FINBYR][MAY CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([FINMGT][FINBYR][JUNE CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([FINMGT][FINBYR][JULY CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([FINMGT][FINBYR][AUGUST CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([FINMGT][FINBYR][SEPTEMBER CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([FINMGT][FINBYR][OCTOBER CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([FINMGT][FINBYR][NOVEMBER CREDITS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([FINMGT][FINBYR][DECEMBER CREDITS]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the current month posted credits which is calculated by the posted credits of the Current Month according to the CURRENT FISCAL MONTH.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#ifdef QUARTER_POSTED
	d=DFvirtualNEW("FINMGT","FIRST QUARTER POSTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]+[FINMGT][FINBYR][PRE-YEAR DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+3));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter posted debits, i.e. the total posted July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER POSTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter posted debits, i.e. the total posted October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER POSTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter posted debits, i.e. the total posted January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER POSTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][POST YEAR DEBITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter posted debits, i.e. the total posted April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER POSTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]+[FINMGT][FINBYR][PRE-YEAR CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter posted credits, i.e. the total posted July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER POSTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter posted credits, i.e. the total posted October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER POSTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter posted credits, i.e. the total posted January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER POSTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][POST YEAR CREDITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter posted credits, i.e. the total posted April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef QUARTER_DISTRIBUTED
	d=DFvirtualNEW("FINMGT","FIRST QUARTER DISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter distributed debits, i.e. the total distributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER DISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter distributed debits, i.e. the total distributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER DISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter distributed debits, i.e. the total distributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER DISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][POST YEAR DEBITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter distributed debits, i.e. the total distributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER DISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter distributed credits, i.e. the total distributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER DISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter distributed credits, i.e. the total distributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER DISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter distributed credits, i.e. the total distributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER DISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter distributed credits, i.e. the total distributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef QUARTER_UNDISTRIBUTED
	d=DFvirtualNEW("FINMGT","FIRST QUARTER UNDISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter undistributed debits, i.e. the total undistributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER UNDISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter undistributed debits, i.e. the total undistributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER UNDISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter undistributed debits, i.e. the total undistributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER UNDISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter undistributed debits, i.e. the total undistributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER UNDISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter undistributed credits, i.e. the total undistributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER UNDISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter undistributed credits, i.e. the total undistributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER UNDISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter undistributed credits, i.e. the total undistributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER UNDISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter undistributed credits, i.e. the total undistributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
	d=DFvirtualNEW("FINMGT","FIRST QUARTER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]+[FINMGT][FINBYR][PRE-YEAR DEBITS]+[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS]+[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]",
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2),getmonth(finsetup->month),
		getmonth(finsetup->month+1),getmonth(finsetup->month+2),
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter debits, i.e. the total posted, distributed and undistributed for July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]",
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5),getmonth(finsetup->month+3),
		getmonth(finsetup->month+4),getmonth(finsetup->month+5),
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter debits, i.e. the total posted, distributed and undistributed for October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]",
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8),getmonth(finsetup->month+6),
		getmonth(finsetup->month+7),getmonth(finsetup->month+8),
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter debits, i.e. the total posted, distributed and undistributed for January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s DISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED DEBITS]+[FINMGT][FINBYR][POST YEAR DEBITS]+[FINMGT][FINBYR][POST YEAR DISTRIBUTED DEBITS]+[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]",
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11),getmonth(finsetup->month+9),
		getmonth(finsetup->month+10),getmonth(finsetup->month+11),
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter debits, i.e. the total posted, distributed and undistributed for April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]+[FINMGT][FINBYR][PRE-YEAR CREDITS]+[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS]+[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]",
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2),getmonth(finsetup->month),
		getmonth(finsetup->month+1),getmonth(finsetup->month+2),
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter credits, i.e. the total posted, distributed and undistributed for July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]",
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5),getmonth(finsetup->month+3),
		getmonth(finsetup->month+4),getmonth(finsetup->month+5),
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter credits, i.e. the total posted, distributed and undistributed for October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]",
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8),getmonth(finsetup->month+6),
		getmonth(finsetup->month+7),getmonth(finsetup->month+8),
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter credits, i.e. the total posted, distributed and undistributed for January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s DISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][%s UNDISTRIBUTED CREDITS]+[FINMGT][FINBYR][POST YEAR CREDITS]+[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS]+[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]",
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11),getmonth(finsetup->month+9),
		getmonth(finsetup->month+10),getmonth(finsetup->month+11),
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter credits, i.e. the total posted, distributed and undistributed for April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
}
