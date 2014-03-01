/* finvirt2.c - Makes FINMGT EXPENDITURE 2 Virtual Fields */
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

extern char *makepretodateexpres(char *,char *,char *);

void FINMGTCreateEV2()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];

#ifdef TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to January which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to February which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to March which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to April which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL EXPENDITURES]",
		"[FINMGT][FINEYR][MAY EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to May which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL EXPENDITURES]",
		"[FINMGT][FINEYR][MAY EXPENDITURES]",
		"[FINMGT][FINEYR][JUNE EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to June which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to July which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to August which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to September which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to October which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to November which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to December which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to January which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to February which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to March which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to April which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to May which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JUNE ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to June which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to July which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to August which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to September which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to October which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to November which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to December which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to January which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to February which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to March which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to April which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to May which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JUNE APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to June which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to July which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to August which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to September which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to October which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to November which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to December which is calculated by the sum of the encumbrances from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to January which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to February which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to March which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to April which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to May which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][MAY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JUNE ENCUMBRANCES]",
		"[FINMGT][FINEYR][JUNE DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JUNE UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to June which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to July which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to August which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to September which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to October which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to November which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to December which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to January which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to February which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to March which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to April which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MAY EXPENDITURES]",
		"[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to May which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MAY EXPENDITURES]",
		"[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JUNE EXPENDITURES]",
		"[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to June which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to July which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to August which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to September which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to October which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to November which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY EXPENDITURES]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to December which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to January which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to February which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to March which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to April which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to May which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MARCH UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][APRIL UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][MAY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JUNE APPROPRIATIONS]",
		"[FINMGT][FINEYR][JUNE DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JUNE UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to June which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to July which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to August which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to September which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to October which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to November which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]",
		"[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to December which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO JANUARY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO FEBRUARY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO MARCH]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO APRIL]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO MAY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO JUNE]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO JULY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO AUGUST]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO SEPTEMBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO OCTOBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO NOVEMBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([ANNUAL APPROPRIATIONS TOTAL PRE-YEAR TO DECEMBER]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual appropriations Fiscal Year to Present Month which is calculated by the sum of the posted, distributed, and undistributed appropriations from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL EXPENDITURES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO JANUARY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO FEBRUARY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO MARCH]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO APRIL]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO MAY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO JUNE]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO JULY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO AUGUST]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO SEPTEMBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO OCTOBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO NOVEMBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO DECEMBER]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual expenditures Fiscal Year to Present Month which is calculated by the sum of the posted, distributed, and undistributed expenditures from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO JANUARY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO FEBRUARY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO MARCH]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO APRIL]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO MAY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO JUNE]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO JULY]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO AUGUST]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO SEPTEMBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO OCTOBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO NOVEMBER]) ",
		"ELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO DECEMBER]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual encumbrances Fiscal Year to Present Month which is calculated by the sum of the posted, distributed, and undistributed encumbrances from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef POSTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","APPROPRIATIONS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted appropriations Fiscal Year to Present Month which is calculated by the sum of the posted appropriations from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","EXPENDITURES");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted expenditures Fiscal Year to Present Month which is calculated by the sum of the posted expenditures from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","ENCUMBRANCES");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted encumbrances Fiscal Year to Present Month which is calculated by the sum of the posted encumbrances from the Fiscal Year Beginning up to the Current Month according to the CURRRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef DISTRIBUTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED APPROPRIATIONS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","DISTRIBUTED APPROPRIATIONS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed appropriations Fiscal Year to Present Month which is calculated by the sum of the distributed appropriations from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED EXPENDITURES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","DISTRIBUTED EXPENDITURES");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed expenditures Fiscal Year to Present Month which is calculated by the sum of the distributed expenditures from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED ENCUMBRANCES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","DISTRIBUTED ENCUMBRANCES");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed encumbrances Fiscal Year to Present Month which is calculated by the sum of the distributed encumbrances from the Fiscal Year Beginning up to the Current Month according to the CURRRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef UNDISTRIBUTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED APPROPRIATIONS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","UNDISTRIBUTED APPROPRIATIONS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed appropriations Fiscal Year to Present Month which is calculated by the sum of the undistributed appropriations from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED EXPENDITURES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","UNDISTRIBUTED EXPENDITURES");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed expenditures Fiscal Year to Present Month which is calculated by the sum of the undistributed expenditures from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED ENCUMBRANCES TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINEYR","UNDISTRIBUTED ENCUMBRANCES");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed encumbrances Fiscal Year to Present Month which is calculated by the sum of the undistributed encumbrances from the Fiscal Year Beginning up to the Current Month according to the CURRRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
	d=DFvirtualNEW("FINMGT","CURRENT MONTH POSTED APPROPRIATIONS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([FINMGT][FINEYR][JANUARY APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([FINMGT][FINEYR][MARCH APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([FINMGT][FINEYR][APRIL APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([FINMGT][FINEYR][MAY APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([FINMGT][FINEYR][JUNE APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([FINMGT][FINEYR][JULY APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([FINMGT][FINEYR][AUGUST APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([FINMGT][FINEYR][OCTOBER APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([FINMGT][FINEYR][DECEMBER APPROPRIATIONS]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the current month posted appropriations which is calculated by the posted appropriations of the Current Month according to the CURRENT FISCAL MONTH.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","CURRENT MONTH POSTED EXPENDITURES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([FINMGT][FINEYR][JANUARY EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([FINMGT][FINEYR][FEBRUARY EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([FINMGT][FINEYR][MARCH EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([FINMGT][FINEYR][APRIL EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([FINMGT][FINEYR][MAY EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([FINMGT][FINEYR][JUNE EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([FINMGT][FINEYR][JULY EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([FINMGT][FINEYR][AUGUST EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([FINMGT][FINEYR][SEPTEMBER EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([FINMGT][FINEYR][OCTOBER EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([FINMGT][FINEYR][NOVEMBER EXPENDITURES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([FINMGT][FINEYR][DECEMBER EXPENDITURES]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the current month posted expenditures which is calculated by the posted expenditures of the Current Month according to the CURRENT FISCAL MONTH.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","CURRENT MONTH POSTED ENCUMBRANCES");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([FINMGT][FINEYR][JANUARY ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([FINMGT][FINEYR][MARCH ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([FINMGT][FINEYR][APRIL ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([FINMGT][FINEYR][MAY ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([FINMGT][FINEYR][JUNE ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([FINMGT][FINEYR][JULY ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([FINMGT][FINEYR][AUGUST ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([FINMGT][FINEYR][OCTOBER ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([FINMGT][FINEYR][DECEMBER ENCUMBRANCES]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the current month posted encumbrances which is calculated by the posted encumbrances of the Current Month according to the CURRENT FISCAL MONTH.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([ANNUAL UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([ANNUAL UNDISTRIBUTED EXPENDITURES]+[ANNUAL UNDISTRIBUTED ENCUMBRANCES])/[ANNUAL UNDISTRIBUTED APPROPRIATIONS])*100)",
		 "FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of undistributed appropriations to undistributed expenditures and encumbrances. If the undistributed appropriations is equal to $ 0.00 then 100 is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([ANNUAL DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE-((([ANNUAL DISTRIBUTED EXPENDITURES]+[ANNUAL DISTRIBUTED ENCUMBRANCES])/[ANNUAL DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of distributed appropriations to distributed expenditures and encumbrances. If the distributed appropriations is equal to $ 0.00 then 100 is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([ANNUAL POSTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([ANNUAL POSTED EXPENDITURES]+[ANNUAL POSTED ENCUMBRANCES])/[ANNUAL POSTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of posted appropriations to posted expenditures and encumbrances. If the posted appropriations is equal to $ 0.00 then 100 is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","ANNUAL POSTED PERCENTAGE USED PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO DATE]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([ANNUAL POSTED EXPENDITURES TOTAL PRE-YEAR TO DATE]+[ANNUAL POSTED ENCUMBRANCES TOTAL PRE-YEAR TO DATE])/[ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO DATE])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of posted appropriations to posted expenditures and encumbrances. If the posted appropriations is equal to $ 0.00 then 100 is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);



	d=DFvirtualNEW("FINMGT","ANNUAL PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([ANNUAL POSTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([ANNUAL EXPENDITURES TOTAL]+[ANNUAL ENCUMBRANCES TOTAL])/[ANNUAL POSTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of posted appropriations already used up by posted, distributed, and undistributed expenditures and encumbrances. If the posted appropriations is equal to $ 0.00 then 100 is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","ANNUAL PERCENTAGE USED PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO DATE]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([ANNUAL EXPENDITURES TOTAL PRE-YEAR TO DATE]+[ANNUAL ENCUMBRANCES TOTAL PRE-YEAR TO DATE])/[ANNUAL POSTED APPROPRIATIONS TOTAL PRE-YEAR TO DATE])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of posted appropriations already used up by posted, distributed, and undistributed expenditures and encumbrances. If the posted appropriations is equal to $ 0.00 then 100 is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);



	d=DFvirtualNEW("FINMGT","PRE-YEAR POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][PRE-YEAR EXPENDITURES]+[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES])/[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year posted percentage used which is calculated by dividing the sum of the Pre-Year posted expenditures and encumbrances by the Pre-Year posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","POST YEAR POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][POST YEAR APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][POST YEAR EXPENDITURES]+[FINMGT][FINEYR][POST YEAR ENCUMBRANCES])/[FINMGT][FINEYR][POST YEAR APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year posted percentage used which is calculated by dividing the sum of the Post Year posted expenditures and encumbrances by the Post Year posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JANUARY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JANUARY EXPENDITURES]+[FINMGT][FINEYR][JANUARY ENCUMBRANCES])/[FINMGT][FINEYR][JANUARY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January posted percentage used which is calculated by dividing the sum of the January posted expenditures and encumbrances by the January posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][FEBRUARY EXPENDITURES]+[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES])/[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February posted percentage used which is calculated by dividing the sum of the February posted expenditures and encumbrances by the February posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MARCH APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][MARCH EXPENDITURES]+[FINMGT][FINEYR][MARCH ENCUMBRANCES])/[FINMGT][FINEYR][MARCH APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March posted percentage used which is calculated by dividing the sum of the March posted expenditures and encumbrances by the March posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][APRIL APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][APRIL EXPENDITURES]+[FINMGT][FINEYR][APRIL ENCUMBRANCES])/[FINMGT][FINEYR][APRIL APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April posted percentage used which is calculated by dividing the sum of the April posted expenditures and encumbrances by the April posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MAY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][MAY EXPENDITURES]+[FINMGT][FINEYR][MAY ENCUMBRANCES])/[FINMGT][FINEYR][MAY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May posted percentage used which is calculated by dividing the sum of the May posted expenditures and encumbrances by the May posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JUNE APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JUNE EXPENDITURES]+[FINMGT][FINEYR][JUNE ENCUMBRANCES])/[FINMGT][FINEYR][JUNE APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June posted percentage used which is calculated by dividing the sum of the June posted expenditures and encumbrances by the June posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JULY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JULY EXPENDITURES]+[FINMGT][FINEYR][JULY ENCUMBRANCES])/[FINMGT][FINEYR][JULY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July posted percentage used which is calculated by dividing the sum of the July posted expenditures and encumbrances by the July posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][AUGUST APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][AUGUST EXPENDITURES]+[FINMGT][FINEYR][AUGUST ENCUMBRANCES])/[FINMGT][FINEYR][AUGUST APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August posted percentage used which is calculated by dividing the sum of the August posted expenditures and encumbrances by the August posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][SEPTEMBER EXPENDITURES]+[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES])/[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September posted percentage used which is calculated by dividing the sum of the September posted expenditures and encumbrances by the September posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][OCTOBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][OCTOBER EXPENDITURES]+[FINMGT][FINEYR][OCTOBER ENCUMBRANCES])/[FINMGT][FINEYR][OCTOBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October posted percentage used which is calculated by dividing the sum of the October posted expenditures and encumbrances by the October posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][NOVEMBER EXPENDITURES]+[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES])/[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November posted percentage used which is calculated by dividing the sum of the November posted expenditures and encumbrances by the November posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER POSTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][DECEMBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][DECEMBER EXPENDITURES]+[FINMGT][FINEYR][DECEMBER ENCUMBRANCES])/[FINMGT][FINEYR][DECEMBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December posted percentage used which is calculated by dividing the sum of the December posted expenditures and encumbrances by the December posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","PRE-YEAR DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year distributed percentage used which is calculated by dividing the sum of the Pre-Year distributed expenditures and encumbrances by the Pre-Year distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","POST YEAR DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][POST YEAR DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][POST YEAR DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][POST YEAR DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][POST YEAR DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year distributed percentage used which is calculated by dividing the sum of the Post Year distributed expenditures and encumbrances by the Post Year distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][JANUARY DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January distributed percentage used which is calculated by dividing the sum of the January distributed expenditures and encumbrances by the January distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][FEBRUARY DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][FEBRUARY DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February distributed percentage used which is calculated by dividing the sum of the February distributed expenditures and encumbrances by the February distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MARCH DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MARCH DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][MARCH DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March distributed percentage used which is calculated by dividing the sum of the March distributed expenditures and encumbrances by the March distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][APRIL DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][APRIL DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][APRIL DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April distributed percentage used which is calculated by dividing the sum of the April distributed expenditures and encumbrances by the April distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MAY DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MAY DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][MAY DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May distributed percentage used which is calculated by dividing the sum of the May distributed expenditures and encumbrances by the May distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JUNE DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JUNE DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][JUNE DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June distributed percentage used which is calculated by dividing the sum of the June distributed expenditures and encumbrances by the June distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][JULY DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July distributed percentage used which is calculated by dividing the sum of the July distributed expenditures and encumbrances by the July distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][AUGUST DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August distributed percentage used which is calculated by dividing the sum of the August distributed expenditures and encumbrances by the August distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September distributed percentage used which is calculated by dividing the sum of the September distributed expenditures and encumbrances by the September distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][OCTOBER DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October distributed percentage used which is calculated by dividing the sum of the October distributed expenditures and encumbrances by the October distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][NOVEMBER DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November distributed percentage used which is calculated by dividing the sum of the November distributed expenditures and encumbrances by the November distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER DISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][DECEMBER DISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December distributed percentage used which is calculated by dividing the sum of the December distributed expenditures and encumbrances by the December distributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","PRE-YEAR UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year undistributed percentage used which is calculated by dividing the sum of the Pre-Year undistributed expenditures and encumbrances by the Pre-Year undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","POST YEAR UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][POST YEAR UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][POST YEAR UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year undistributed percentage used which is calculated by dividing the sum of the Post Year undistributed expenditures and encumbrances by the Post Year undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][JANUARY UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January undistributed percentage used which is calculated by dividing the sum of the January undistributed expenditures and encumbrances by the January undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February undistributed percentage used which is calculated by dividing the sum of the February undistributed expenditures and encumbrances by the February undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MARCH UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MARCH UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][MARCH UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March undistributed percentage used which is calculated by dividing the sum of the March undistributed expenditures and encumbrances by the March undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][APRIL UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][APRIL UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][APRIL UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April undistributed percentage used which is calculated by dividing the sum of the April undistributed expenditures and encumbrances by the April undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MAY UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MAY UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][MAY UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May undistributed percentage used which is calculated by dividing the sum of the May undistributed expenditures and encumbrances by the May undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JUNE UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JUNE UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][JUNE UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June undistributed percentage used which is calculated by dividing the sum of the June undistributed expenditures and encumbrances by the June undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][JULY UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July undistributed percentage used which is calculated by dividing the sum of the July undistributed expenditures and encumbrances by the July undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][AUGUST UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August undistributed percentage used which is calculated by dividing the sum of the August undistributed expenditures and encumbrances by the August undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September undistributed percentage used which is calculated by dividing the sum of the September undistributed expenditures and encumbrances by the September undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October undistributed percentage used which is calculated by dividing the sum of the October undistributed expenditures and encumbrances by the October undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November undistributed percentage used which is calculated by dividing the sum of the November undistributed expenditures and encumbrances by the November undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER UNDISTRIBUTED PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=((([FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES])/[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December undistributed percentage used which is calculated by dividing the sum of the December undistributed expenditures and encumbrances by the December undistributed appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","PRE-YEAR AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]-[FINMGT][FINEYR][PRE-YEAR EXPENDITURES]-[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]-[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year available balance which is calculated by the difference between the Pre-Year posted appropriations and the sum of the Pre-Year posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","POST YEAR AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][POST YEAR APPROPRIATIONS]-[FINMGT][FINEYR][POST YEAR EXPENDITURES]-[FINMGT][FINEYR][POST YEAR DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][POST YEAR ENCUMBRANCES]-[FINMGT][FINEYR][POST YEAR DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year available balance which is calculated by the difference between the Post Year posted appropriations and the sum of the Post Year posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][JANUARY APPROPRIATIONS]-[FINMGT][FINEYR][JANUARY EXPENDITURES]-[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][JANUARY ENCUMBRANCES]-[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January available balance which is calculated by the difference between the January posted appropriations and the sum of the January posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY EXPENDTURES TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]-[FINMGT][FINEYR][FEBRUARY EXPENDITURES]-[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]-[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February available balance which is calculated by the difference between the February posted appropriations and the sum of the February posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][MARCH APPROPRIATIONS]-[FINMGT][FINEYR][MARCH EXPENDITURES]-[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][MARCH ENCUMBRANCES]-[FINMGT][FINEYR][MARCH DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][MARCH UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March available balance which is calculated by the difference between the March posted appropriations and the sum of the March posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][APRIL APPROPRIATIONS]-[FINMGT][FINEYR][APRIL EXPENDITURES]-[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][APRIL ENCUMBRANCES]-[FINMGT][FINEYR][APRIL DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][APRIL UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April available balance which is calculated by the difference between the April posted appropriations and the sum of the April posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][MAY APPROPRIATIONS]-[FINMGT][FINEYR][MAY EXPENDITURES]-[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][MAY ENCUMBRANCES]-[FINMGT][FINEYR][MAY DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][MAY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May available balance which is calculated by the difference between the May posted appropriations and the sum of the May posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][JUNE APPROPRIATIONS]-[FINMGT][FINEYR][JUNE EXPENDITURES]-[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][JUNE ENCUMBRANCES]-[FINMGT][FINEYR][JUNE DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][JUNE UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June available balance which is calculated by the difference between the June posted appropriations and the sum of the June posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][JULY APPROPRIATIONS]-[FINMGT][FINEYR][JULY EXPENDITURES]-[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][JULY ENCUMBRANCES]-[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July available balance which is calculated by the difference between the July posted appropriations and the sum of the July posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][AUGUST APPROPRIATIONS]-[FINMGT][FINEYR][AUGUST EXPENDITURES]-[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][AUGUST ENCUMBRANCES]-[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August available balance which is calculated by the difference between the August posted appropriations and the sum of the August posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]-[FINMGT][FINEYR][SEPTEMBER EXPENDITURES]-[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]-[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September available balance which is calculated by the difference between the September posted appropriations and the sum of the September posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][OCTOBER APPROPRIATIONS]-[FINMGT][FINEYR][OCTOBER EXPENDITURES]-[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]-[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October available balance which is calculated by the difference between the October posted appropriations and the sum of the October posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]-[FINMGT][FINEYR][NOVEMBER EXPENDITURES]-[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]-[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November available balance which is calculated by the difference between the November posted appropriations and the sum of the November posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER AVAILABLE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINEYR][DECEMBER APPROPRIATIONS]-[FINMGT][FINEYR][DECEMBER EXPENDITURES]-[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]-[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]-[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]-[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December available balance which is calculated by the difference between the December posted appropriations and the sum of the December posted, distributed, and undistributed expenditures and encumbrances.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","PRE-YEAR PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][PRE-YEAR EXPENDITURES]+[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][PRE-YEAR ENCUMBRANCES]+[FINMGT][FINEYR][PRE-YEAR DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][PRE-YEAR UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][PRE-YEAR APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year percentage used which is calculated by dividing the sum of the Pre-Year posted, distributed, and undistributed expenditures and encumbrances by the Pre-Year posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","POST YEAR PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][POST YEAR APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][POST YEAR EXPENDITURES]+[FINMGT][FINEYR][POST YEAR DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][POST YEAR ENCUMBRANCES]+[FINMGT][FINEYR][POST YEAR DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][POST YEAR UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][POST YEAR APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year percentage used which is calculated by dividing the sum of the Post Year posted, distributed, and undistributed expenditures and encumbrances by the Post Year posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JANUARY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][JANUARY EXPENDITURES]+[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JANUARY ENCUMBRANCES]+[FINMGT][FINEYR][JANUARY DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][JANUARY UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][JANUARY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January percentage used which is calculated by dividing the sum of the January posted, distributed, and undistributed expenditures and encumbrances by the January posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][FEBRUARY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][FEBRUARY EXPENDITURES]+[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][FEBRUARY ENCUMBRANCES]+[FINMGT][FINEYR][FEBRUARY DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][FEBRUARY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February percentage used which is calculated by dividing the sum of the February posted, distributed, and undistributed expenditures and encumbrances by the February posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MARCH APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][MARCH EXPENDITURES]+[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MARCH ENCUMBRANCES]+[FINMGT][FINEYR][MARCH DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][MARCH UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][MARCH APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March percentage used which is calculated by dividing the sum of the March posted, distributed, and undistributed expenditures and encumbrances by the March posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][APRIL APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][APRIL EXPENDITURES]+[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][APRIL ENCUMBRANCES]+[FINMGT][FINEYR][APRIL DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][APRIL UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][APRIL APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April percentage used which is calculated by dividing the sum of the April posted, distributed, and undistributed expenditures and encumbrances by the April posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][MAY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][MAY EXPENDITURES]+[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][MAY ENCUMBRANCES]+[FINMGT][FINEYR][MAY DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][MAY UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][MAY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May percentage used which is calculated by dividing the sum of the May posted, distributed, and undistributed expenditures and encumbrances by the May posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JUNE APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][JUNE EXPENDITURES]+[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JUNE ENCUMBRANCES]+[FINMGT][FINEYR][JUNE DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][JUNE UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][JUNE APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June percentage used which is calculated by dividing the sum of the June posted, distributed, and undistributed expenditures and encumbrances by the June posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][JULY APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][JULY EXPENDITURES]+[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][JULY ENCUMBRANCES]+[FINMGT][FINEYR][JULY DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][JULY UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][JULY APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July percentage used which is calculated by dividing the sum of the July posted, distributed, and undistributed expenditures and encumbrances by the July posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][AUGUST APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][AUGUST EXPENDITURES]+[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][AUGUST ENCUMBRANCES]+[FINMGT][FINEYR][AUGUST DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][AUGUST UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][AUGUST APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August percentage used which is calculated by dividing the sum of the August posted, distributed, and undistributed expenditures and encumbrances by the August posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][SEPTEMBER EXPENDITURES]+[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][SEPTEMBER ENCUMBRANCES]+[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][SEPTEMBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September percentage used which is calculated by dividing the sum of the September posted, distributed, and undistributed expenditures and encumbrances by the September posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][OCTOBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][OCTOBER EXPENDITURES]+[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][OCTOBER ENCUMBRANCES]+[FINMGT][FINEYR][OCTOBER DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][OCTOBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October percentage used which is calculated by dividing the sum of the October posted, distributed, and undistributed expenditures and encumbrances by the October posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][NOVEMBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][NOVEMBER EXPENDITURES]+[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][NOVEMBER ENCUMBRANCES]+[FINMGT][FINEYR][NOVEMBER DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][NOVEMBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November percentage used which is calculated by dividing the sum of the November posted, distributed, and undistributed expenditures and encumbrances by the November posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER PERCENTAGE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s",
		"IF ([FINMGT][FINEYR][DECEMBER APPROPRIATIONS]=0.0)",
		"THEN RETURN_VALUE=(100.0)",
		"ELSE RETURN_VALUE=(([FINMGT][FINEYR][DECEMBER EXPENDITURES]+[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]+[FINMGT][FINEYR][DECEMBER ENCUMBRANCES]+[FINMGT][FINEYR][DECEMBER DISTRIBUTED ENCUMBRANCES]+[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED ENCUMBRANCES])/([FINMGT][FINEYR][DECEMBER APPROPRIATIONS])*100)",
		"FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December percentage used which is calculated by dividing the sum of the December posted, distributed, and undistributed expenditures and encumbrances by the December posted appropriations and then multipling the value by 100.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
}
