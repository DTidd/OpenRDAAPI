/* finvirt6.c - Makes FINMGT REVENUE 2 Virtual Fields */
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

void FINMGTCreateRV2()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];
	char revenuepos=FALSE;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif

	gsv=RDAGenericSetupNew("FINMGT","USE REVENUE POS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			revenuepos=*gsv->value.string_value;
		} else {
			revenuepos=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);

#ifdef TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]",
		"[FINMGT][FINRYR][DECEMBER REVENUE]",
		"[FINMGT][FINRYR][JANUARY REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to January which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]",
		"[FINMGT][FINRYR][DECEMBER REVENUE]",
		"[FINMGT][FINRYR][JANUARY REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to February which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]",
		"[FINMGT][FINRYR][DECEMBER REVENUE]",
		"[FINMGT][FINRYR][JANUARY REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY REVENUE]",
		"[FINMGT][FINRYR][MARCH REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to March which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]",
		"[FINMGT][FINRYR][DECEMBER REVENUE]",
		"[FINMGT][FINRYR][JANUARY REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY REVENUE]",
		"[FINMGT][FINRYR][MARCH REVENUE]",
		"[FINMGT][FINRYR][APRIL REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to April which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]",
		"[FINMGT][FINRYR][DECEMBER REVENUE]",
		"[FINMGT][FINRYR][JANUARY REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY REVENUE]",
		"[FINMGT][FINRYR][MARCH REVENUE]",
		"[FINMGT][FINRYR][APRIL REVENUE]",
		"[FINMGT][FINRYR][MAY REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to May which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]",
		"[FINMGT][FINRYR][DECEMBER REVENUE]",
		"[FINMGT][FINRYR][JANUARY REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY REVENUE]",
		"[FINMGT][FINRYR][MARCH REVENUE]",
		"[FINMGT][FINRYR][APRIL REVENUE]",
		"[FINMGT][FINRYR][MAY REVENUE]",
		"[FINMGT][FINRYR][JUNE REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to June which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to July which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to August which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to September which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to October which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to November which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]",
		"[FINMGT][FINRYR][JULY REVENUE]",
		"[FINMGT][FINRYR][AUGUST REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER REVENUE]",
		"[FINMGT][FINRYR][OCTOBER REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER REVENUE]",
		"[FINMGT][FINRYR][DECEMBER REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to December which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO JANUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JANUARY ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to January which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the January.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO FEBRUARY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JANUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to February which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the February.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO MARCH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JANUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][MARCH ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to March which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO APRIL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JANUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][MARCH ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][APRIL ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to April which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the April.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO MAY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JANUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][MARCH ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][APRIL ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][MAY ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to May which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the May.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO JUNE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JANUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][FEBRUARY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][MARCH ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][APRIL ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][MAY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JUNE ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to June which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO JULY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to July which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the July.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO AUGUST");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to August which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the August.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO SEPTEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to September which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO OCTOBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to October which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the October.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO NOVEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to November which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the November.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO DECEMBER");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][JULY ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]",
		"[FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to December which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef POSTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED ESTIMATED REVENUE TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","ESTIMATED REVENUE");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted estimated revenue Fiscal Year to Present Month which is calculated by the sum of the posted estimated revenue from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","REVENUE");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue Fiscal Year to Present Month which is calculated by the sum of the posted revenue from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	if(revenuepos==TRUE)
	{
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED REVENUE POS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","REVENUE POS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual posted revenue pos Fiscal Year to Present Month which is calculated by the sum of the posted revenue pos from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	}
#endif
#ifdef DISTRIBUTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED ESTIMATED REVENUE TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","DISTRIBUTED ESTIMATED REVENUE");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed estimated revenue Fiscal Year to Present Month which is calculated by the sum of the distributed estimated revenue from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED REVENUE TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","DISTRIBUTED REVENUE");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed revenue Fiscal Year to Present Month which is calculated by the sum of the distributed revenue from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	if(revenuepos==TRUE)
	{
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED REVENUE POS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","DISTRIBUTED REVENUE POS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual distributed revenue pos Fiscal Year to Present Month which is calculated by the sum of the distributed revenue pos from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	}
#endif
#ifdef UNDISTRIBUTED_TO_DATE
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED ESTIMATED REVENUE TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","UNDISTRIBUTED ESTIMATED REVENUE");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed estimated revenue Fiscal Year to Present Month which is calculated by the sum of the undistributed estimated revenue from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED REVENUE TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","UNDISTRIBUTED REVENUE");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed revenue Fiscal Year to Present Month which is calculated by the sum of the undistributed revenue from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	if(revenuepos==TRUE)
	{
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED REVENUE POS TOTAL PRE-YEAR TO DATE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	d->expression=makepretodateexpres("FINMGT","FINRYR","UNDISTRIBUTED REVENUE POS");
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the annual undistributed revenue pos Fiscal Year to Present Month which is calculated by the sum of the undistributed revenue pos from the Fiscal Year Beginning up to the Current Month according to the CURRENT FISCAL MONTH including the PRE-YEAR values.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	}
#endif
	d=DFvirtualNEW("FINMGT","CURRENT MONTH DISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([FINMGT][FINRYR][JANUARY ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([FINMGT][FINRYR][FEBRUARY ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([FINMGT][FINRYR][MARCH ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([FINMGT][FINRYR][APRIL ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([FINMGT][FINRYR][MAY ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([FINMGT][FINRYR][JUNE ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([FINMGT][FINRYR][JULY ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([FINMGT][FINRYR][AUGUST ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([FINMGT][FINRYR][SEPTEMBER ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([FINMGT][FINRYR][OCTOBER ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([FINMGT][FINRYR][NOVEMBER ESTIMATED REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([FINMGT][FINRYR][DECEMBER ESTIMATED REVENUE]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the current month posted estimated revenue which is calculated by the posted estimated revenue of the Current Month according to the CURRENT FISCAL MONTH.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","CURRENT MONTH POSTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([CURRENT FISCAL MONTH]=1) THEN RETURN_VALUE=([FINMGT][FINRYR][JANUARY REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=2) THEN RETURN_VALUE=([FINMGT][FINRYR][FEBRUARY REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=3) THEN RETURN_VALUE=([FINMGT][FINRYR][MARCH REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=4) THEN RETURN_VALUE=([FINMGT][FINRYR][APRIL REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=5) THEN RETURN_VALUE=([FINMGT][FINRYR][MAY REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=6) THEN RETURN_VALUE=([FINMGT][FINRYR][JUNE REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=7) THEN RETURN_VALUE=([FINMGT][FINRYR][JULY REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=8) THEN RETURN_VALUE=([FINMGT][FINRYR][AUGUST REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=9) THEN RETURN_VALUE=([FINMGT][FINRYR][SEPTEMBER REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=10) THEN RETURN_VALUE=([FINMGT][FINRYR][OCTOBER REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=11) THEN RETURN_VALUE=([FINMGT][FINRYR][NOVEMBER REVENUE]) ",
		"\nELSE IF ([CURRENT FISCAL MONTH]=12) THEN RETURN_VALUE=([FINMGT][FINRYR][DECEMBER REVENUE]) ",
		" FI FI FI FI FI FI FI FI FI FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the current month posted revenue which is calculated by the posted revenue of the Current Month according to the CURRENT FISCAL MONTH.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","ANNUAL POSTED PERCENTAGE UNREALIZED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"IF([ANNUAL POSTED ESTIMATED REVENUE]=0.0) THEN RETURN_VALUE=(0.0) ELSE RETURN_VALUE=(100.0-(([ANNUAL POSTED REVENUE]/[ANNUAL POSTED ESTIMATED REVENUE])*100.0)) FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of posted estimated revenue that has not been realized by the posted revenue. If the posted estimated revenue is equal to $ 0.00 then zero (0) is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","ANNUAL POSTED PERCENTAGE REALIZED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DECIMALV;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"IF([ANNUAL POSTED ESTIMATED REVENUE]=0.0) THEN RETURN_VALUE=(100.0) ELSE RETURN_VALUE=(([ANNUAL POSTED REVENUE]/[ANNUAL POSTED ESTIMATED REVENUE])*100.0) FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total percentage of the posted estimated revenue that has been realized in the posted revenue. If the posted estimated revenue is equal to $ 0.00 then 100 is returned.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","FIRST QUARTER ESTIMATED REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]+[FINMGT][FINRYR][PRE-YEAR DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]",
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2),getmonth(finsetup->month),
		getmonth(finsetup->month+1),getmonth(finsetup->month+2),
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter estimated revenue, i.e. the total posted, distributed and undistributed for July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER ESTIMATED REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]",
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5),getmonth(finsetup->month+3),
		getmonth(finsetup->month+4),getmonth(finsetup->month+5),
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter estimated revenue, i.e. the total posted, distributed and undistributed for October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER ESTIMATED REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]",
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8),getmonth(finsetup->month+6),
		getmonth(finsetup->month+7),getmonth(finsetup->month+8),
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter estimated revenue, i.e. the total posted, distributed and undistributed for January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER ESTIMATED REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][POST YEAR ESTIMATED REVENUE]+[FINMGT][FINRYR][POST YEAR DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][POST YEAR UNDISTRIBUTED ESTIMATED REVENUE]",
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11),getmonth(finsetup->month+9),
		getmonth(finsetup->month+10),getmonth(finsetup->month+11),
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter estimated revenue, i.e. the total posted, distributed and undistributed for April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][PRE-YEAR REVENUE]+[FINMGT][FINRYR][PRE-YEAR DISTRIBUTED REVENUE]+[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]",
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2),getmonth(finsetup->month),
		getmonth(finsetup->month+1),getmonth(finsetup->month+2),
		getmonth(finsetup->month),getmonth(finsetup->month+1),
		getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter revenue, i.e. the total posted, distributed and undistributed for July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]",
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5),getmonth(finsetup->month+3),
		getmonth(finsetup->month+4),getmonth(finsetup->month+5),
		getmonth(finsetup->month+3),getmonth(finsetup->month+4),
		getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter revenue, i.e. the total posted, distributed and undistributed for October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]",
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8),getmonth(finsetup->month+6),
		getmonth(finsetup->month+7),getmonth(finsetup->month+8),
		getmonth(finsetup->month+6),getmonth(finsetup->month+7),
		getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter revenue, i.e. the total posted, distributed and undistributed for January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER REVENUE TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,
		"[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][POST YEAR REVENUE]+[FINMGT][FINRYR][POST YEAR DISTRIBUTED REVENUE]+[FINMGT][FINRYR][POST YEAR UNDISTRIBUTED REVENUE]",
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11),getmonth(finsetup->month+9),
		getmonth(finsetup->month+10),getmonth(finsetup->month+11),
		getmonth(finsetup->month+9),getmonth(finsetup->month+10),
		getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter revenue, i.e. the total posted, distributed and undistributed for April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#ifdef QUARTER_POSTED
	d=DFvirtualNEW("FINMGT","FIRST QUARTER POSTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][PRE-YEAR ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter posted estimated revenue, i.e. the total posted July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER POSTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter posted estimated revenue, i.e. the total posted October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER POSTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter posted estimated revenue, i.e. the total posted January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER POSTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][%s ESTIMATED REVENUE]+[FINMGT][FINRYR][POST YEAR ESTIMATED REVENUE]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter posted estimated revenue, i.e. the total posted April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER POSTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][PRE-YEAR REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter posted revenue, i.e. the total posted July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER POSTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter posted revenue, i.e. the total posted October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER POSTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter posted revenue, i.e. the total posted January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER POSTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][%s REVENUE]+[FINMGT][FINRYR][POST YEAR REVENUE]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter posted revenue, i.e. the total posted April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef QUARTER_DISTRIBUTED
	d=DFvirtualNEW("FINMGT","FIRST QUARTER DISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][PRE-YEAR DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter distributed estimated revenue, i.e. the total distributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER DISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter distributed estimated revenue, i.e. the total distributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER DISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter distributed estimated revenue, i.e. the total distributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER DISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][POST YEAR DISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter distributed estimated revenue, i.e. the total distributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER DISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][PRE-YEAR DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter distributed revenue, i.e. the total distributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER DISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter distributed revenue, i.e. the total distributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER DISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter distributed revenue, i.e. the total distributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER DISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s DISTRIBUTED REVENUE]+[FINMGT][FINRYR][POST YEAR DISTRIBUTED REVENUE]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter distributed revenue, i.e. the total distributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
#ifdef QUARTER_UNDISTRIBUTED
	d=DFvirtualNEW("FINMGT","FIRST QUARTER UNDISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter undistributed estimated revenue, i.e. the total undistributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER UNDISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter undistributed estimated revenue, i.e. the total undistributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER UNDISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter undistributed estimated revenue, i.e. the total undistributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER UNDISTRIBUTED ESTIMATED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED ESTIMATED REVENUE]+[FINMGT][FINRYR][POST YEAR UNDISTRIBUTED ESTIMATED REVENUE]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter undistributed estimated revenue, i.e. the total undistributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FIRST QUARTER UNDISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][PRE-YEAR UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]",getmonth(finsetup->month),getmonth(finsetup->month+1),getmonth(finsetup->month+2));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total first quarter undistributed revenue, i.e. the total undistributed July, August, and September.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SECOND QUARTER UNDISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]",getmonth(finsetup->month+3),getmonth(finsetup->month+4),getmonth(finsetup->month+5));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total second quarter undistributed revenue, i.e. the total undistributed October, November, and December.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","THIRD QUARTER UNDISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]",getmonth(finsetup->month+6),getmonth(finsetup->month+7),getmonth(finsetup->month+8));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total third quarter undistributed revenue, i.e. the total undistributed January, February, and March.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FOURTH QUARTER UNDISTRIBUTED REVENUE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=FALSE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][%s UNDISTRIBUTED REVENUE]+[FINMGT][FINRYR][POST YEAR UNDISTRIBUTED REVENUE]",getmonth(finsetup->month+9),getmonth(finsetup->month+10),getmonth(finsetup->month+11));
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total fourth quarter undistributed revenue, i.e. the total undistributed April, May, and June.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
#endif
}
