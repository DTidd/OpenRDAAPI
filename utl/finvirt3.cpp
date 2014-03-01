/* finvirt3.c - Makes FINMGT BALANCE 1 Virtual Fields */
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

void FINMGTCreateBV1()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];
	char *libx=NULL,bsbudget=FALSE;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif

	gsv=RDAGenericSetupNew("FINMGT","USE BALANCE SHEET BUDGETS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			bsbudget=*gsv->value.string_value;
		} else {
			bsbudget=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);

	if(bsbudget==TRUE)
	{
	d=DFvirtualNEW("FINMGT","BUDGET POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][BUDGET DEBITS]",
		"[FINMGT][FINBYR][BUDGET CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Balance Sheet Budget posted balance which is calculated by the difference between the Budget posted debits and the Budget posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","BUDGET DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BUDGET DISTRIBUTED DEBITS]-[FINMGT][FINBYR][BUDGET DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Budget distributed balance total which is calculated by the difference between the Budget distributed debits and the Budget distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","BUDGET UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BUDGET UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][BUDGET UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Budget undistributed balance total which is calculated by the difference between the Budget undistributed debits and the Budget undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","BUDGET BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][BUDGET DEBITS]",
		"[FINMGT][FINBYR][BUDGET DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][BUDGET UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][BUDGET CREDITS]",
		"[FINMGT][FINBYR][BUDGET DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][BUDGET UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total Budget balance calculated by the total Beg Bal debits minus the total Beg Bal credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","BUDGET CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BUDGET CREDITS]+[FINMGT][FINBYR][BUDGET DISTRIBUTED CREDITS]+[FINMGT][FINBYR][BUDGET UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Budget credits total which is calculated by the sum of the Budget posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","BUDGET DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BUDGET DEBITS]+[FINMGT][FINBYR][BUDGET DISTRIBUTED DEBITS]+[FINMGT][FINBYR][BUDGET UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Budget credits total which is calculated by the sum of the Budget posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);



	}

	d=DFvirtualNEW("FINMGT","BEGINNING BALANCE POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Beginning Balance posted balance which is calculated by the difference between the Beginning Balance posted debits and the Beginning Balance posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","PRE-YEAR POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year posted balance which is calculated by the difference between the Pre-Year posted debits and the Pre-Year posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","POST YEAR POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][POST YEAR DEBITS]",
		"[FINMGT][FINBYR][POST YEAR CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year posted balance which is calculated by the difference between the Post Year posted debits and the Post Year posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","JANUARY POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January posted balance which is calculated by the difference between the January posted debits and the January posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February posted balance which is calculated by the difference between the February posted debits and the February posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MARCH DEBITS]-[FINMGT][FINBYR][MARCH CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March posted balance which is calculated by the difference between the March posted debits and the March posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][APRIL DEBITS]",
		"[FINMGT][FINBYR][APRIL CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April posted balance which is calculated by the difference between the April posted debits and the April posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][MAY DEBITS]",
		"[FINMGT][FINBYR][MAY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May posted balance which is calculated by the difference between the May posted debits and the May posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][JUNE DEBITS]",
		"[FINMGT][FINBYR][JUNE CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June posted balance which is calculated by the difference between the June posted debits and the June posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][JULY CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July posted balance which is calculated by the difference between the July posted debits and the July posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August posted balance which is calculated by the difference between the August posted debits and the August posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September posted balance which is calculated by the difference between the September posted debits and the September posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October posted balance which is calculated by the difference between the October posted debits and the October posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November posted balance which is calculated by the difference between the November posted debits and the November posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s-%s",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December posted balance which is calculated by the difference between the December posted debits and the December posted credit.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("FINMGT","BEGINNING BALANCE DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED DEBITS]-[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Beginning Balance distributed balance total which is calculated by the difference between the Beginning Balance distributed debits and the Beginning Balance distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","PRE-YEAR DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS]-[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year distributed balance total which is calculated by the difference between the Pre-Year distributed debits and the Pre-Year distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","POST YEAR DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][POST YEAR DISTRIBUTED DEBITS]-[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year distributed balance total which is calculated by the difference between the Post Year distributed debits and the Post Year distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]-[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January distributed balance total which is calculated by the difference between the January distributed debits and the January distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]-[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February distributed balance total which is calculated by the difference between the February distributed debits and the February distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]-[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March distributed balance total which is calculated by the difference between the March distributed debits and the March distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]-[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April distributed balance total which is calculated by the difference between the April distributed debits and the April distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]-[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May distributed balance total which is calculated by the difference between the May distributed debits and the May distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]-[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June distributed balance total which is calculated by the difference between the June distributed debits and the June distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]-[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July distributed balance total which is calculated by the difference between the July distributed debits and the July distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]-[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August distributed balance total which is calculated by the difference between the August distributed debits and the August distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]-[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September distributed balance total which is calculated by the difference between the September distributed debits and the September distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]-[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October distributed balance total which is calculated by the difference between the October distributed debits and the October distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]-[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November distributed balance total which is calculated by the difference between the November distributed debits and the November distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]-[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December distributed balance total which is calculated by the difference between the December distributed debits and the December distributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);



	d=DFvirtualNEW("FINMGT","BEGINNING BALANCE UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Beginning Balance undistributed balance total which is calculated by the difference between the Beginning Balance undistributed debits and the Beginning Balance undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","PRE-YEAR UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year undistributed balance total which is calculated by the difference between the Pre-Year undistributed debits and the Pre-Year undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","POST YEAR UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year undistributed balance total which is calculated by the difference between the Post Year undistributed debits and the Post Year undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January undistributed balance total which is calculated by the difference between the January undistributed debits and the January undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February undistributed balance total which is calculated by the difference between the February undistributed debits and the February undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March undistributed balance total which is calculated by the difference between the March undistributed debits and the March undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April undistributed balance total which is calculated by the difference between the April undistributed debits and the April undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May undistributed balance total which is calculated by the difference between the May undistributed debits and the May undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June undistributed balance total which is calculated by the difference between the June undistributed debits and the June undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July undistributed balance total which is calculated by the difference between the July undistributed debits and the July undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August undistributed balance total which is calculated by the difference between the August undistributed debits and the August undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September undistributed balance total which is calculated by the difference between the September undistributed debits and the September undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October undistributed balance total which is calculated by the difference between the October undistributed debits and the October undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November undistributed balance total which is calculated by the difference between the November undistributed debits and the November undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]-[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December undistributed balance total which is calculated by the difference between the December undistributed debits and the December undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("FINMGT","BEGINNING BALANCE DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]+[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED DEBITS]+[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Beginning Balance credits total which is calculated by the sum of the Beginning Balance posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("FINMGT","PRE-YEAR DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR DEBITS]+[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS]+[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year credits total which is calculated by the sum of the Pre-Year posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","POST YEAR DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][POST YEAR DEBITS]+[FINMGT][FINBYR][POST YEAR DISTRIBUTED DEBITS]+[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year credits total which is calculated by the sum of the Post Year posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JANUARY DEBITS]+[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]+[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January credits total which is calculated by the sum of the January posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][FEBRUARY DEBITS]+[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]+[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February credits total which is calculated by the sum of the February posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MARCH DEBITS]+[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]+[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March credits total which is calculated by the sum of the March posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][APRIL DEBITS]+[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]+[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April credits total which is calculated by the sum of the April posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MAY DEBITS]+[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]+[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May credits total which is calculated by the sum of the May posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JUNE DEBITS]+[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]+[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June credits total which is calculated by the sum of the June posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JULY DEBITS]+[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]+[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July credits total which is calculated by the sum of the July posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][AUGUST DEBITS]+[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]+[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August credits total which is calculated by the sum of the August posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][SEPTEMBER DEBITS]+[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]+[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September credits total which is calculated by the sum of the September posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][OCTOBER DEBITS]+[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]+[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October credits total which is calculated by the sum of the October posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][NOVEMBER DEBITS]+[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]+[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November credits total which is calculated by the sum of the November posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][DECEMBER DEBITS]+[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]+[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December credits total which is calculated by the sum of the December posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("FINMGT","BEGINNING BALANCE CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]+[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED CREDITS]+[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Beginning Balance credits total which is calculated by the sum of the Beginning Balance posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","PRE-YEAR CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][PRE-YEAR CREDITS]+[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS]+[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Year credits total which is calculated by the sum of the Pre-Year posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","POST YEAR CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][POST YEAR CREDITS]+[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS]+[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Post Year credits total which is calculated by the sum of the Post Year posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JANUARY CREDITS]+[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]+[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the January credits total which is calculated by the sum of the January posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][FEBRUARY CREDITS]+[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]+[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the February credits total which is calculated by the sum of the February posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MARCH CREDITS]+[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]+[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the March credits total which is calculated by the sum of the March posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][APRIL CREDITS]+[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]+[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the April credits total which is calculated by the sum of the April posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][MAY CREDITS]+[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]+[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the May credits total which is calculated by the sum of the May posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JUNE CREDITS]+[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]+[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the June credits total which is calculated by the sum of the June posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][JULY CREDITS]+[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]+[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the July credits total which is calculated by the sum of the July posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][AUGUST CREDITS]+[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]+[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the August credits total which is calculated by the sum of the August posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][SEPTEMBER CREDITS]+[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]+[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the September credits total which is calculated by the sum of the September posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][OCTOBER CREDITS]+[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]+[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the October credits total which is calculated by the sum of the October posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][NOVEMBER CREDITS]+[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]+[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the November credits total which is calculated by the sum of the November posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[FINMGT][FINBYR][DECEMBER CREDITS]+[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]+[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the December credits total which is calculated by the sum of the December posted, distributed, and undistributed credits.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("FINMGT","BEGINNING BALANCE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total Beginning Balance balance calculated by the total Beg Bal debits minus the total Beg Bal credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("FINMGT","PRE-YEAR BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total Pre-Year balance calculated by the total Pre-Year debits minus the total Pre-Year credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("FINMGT","POST YEAR BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][POST YEAR DEBITS]",
		"[FINMGT][FINBYR][POST YEAR DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][POST YEAR CREDITS]",
		"[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total Post Year balance calculated by the total Post Year debits minus the total Post Year credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JANUARY BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]",
		"[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total January balance calculated by the total January debits minus the total January credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","FEBRUARY BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total February balance calculated by the total February debits minus the total February credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MARCH BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][MARCH DEBITS]",
		"[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MARCH CREDITS]",
		"[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total March balance calculated by the total March debits minus the total March credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","APRIL BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][APRIL DEBITS]",
		"[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][APRIL CREDITS]",
		"[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total April balance calculated by the total April debits minus the total April credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","MAY BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][MAY DEBITS]",
		"[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MAY CREDITS]",
		"[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total May balance calculated by the total May debits minus the total May credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JUNE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][JUNE DEBITS]",
		"[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JUNE CREDITS]",
		"[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total June balance calculated by the total June debits minus the total June credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","JULY BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total July balance calculated by the total July debits minus the total July credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","AUGUST BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total August balance calculated by the total August debits minus the total August credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","SEPTEMBER BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total September balance calculated by the total September debits minus the total September credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","OCTOBER BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total October balance calculated by the total October debits minus the total October credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","NOVEMBER BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total November balance calculated by the total November debits minus the total November credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","DECEMBER BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s-%s-%s-%s",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]"
		);
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total December balance calculated by the total December debits minus the total December credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[ANNUAL UNDISTRIBUTED DEBITS]-[ANNUAL UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual undistributed balance calculated by the total annual undistributed debits minus the total annual undistributed credits for a balance sheet account.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[ANNUAL DISTRIBUTED DEBITS]-[ANNUAL DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual distributed balance calculated by the total annual distributed debits minus the total annual distributed credits for a balance sheet account.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"[ANNUAL POSTED DEBITS]-[ANNUAL POSTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual posted balance calculated by the total annual posted debits minus the total annual posted credits for a balance sheet account.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL CREDITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	sprintf(temp,"[ANNUAL POSTED CREDITS]+[ANNUAL DISTRIBUTED CREDITS]+[ANNUAL UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual credits calculated by the sum of the posted, distributed, and undistributed credits for a balance sheet account.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DEBITS TOTAL");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	sprintf(temp,"[ANNUAL POSTED DEBITS]+[ANNUAL DISTRIBUTED DEBITS]+[ANNUAL UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual debits calculated by the sum of the posted, distributed, and undistributed debits for a balance sheet account.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	sprintf(temp,"[ANNUAL DEBITS TOTAL]-[ANNUAL CREDITS TOTAL]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual balance calculated by the total annual debits minus the total annual credits for a balance sheet account.  It includes the posted, distributed, and undistributed amounts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);


	d=DFvirtualNEW("FINMGT","ANNUAL POSTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DEBITS]",
		"[FINMGT][FINBYR][JANUARY DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DEBITS]",
		"[FINMGT][FINBYR][MARCH DEBITS]",
		"[FINMGT][FINBYR][APRIL DEBITS]",
		"[FINMGT][FINBYR][MAY DEBITS]",
		"[FINMGT][FINBYR][JUNE DEBITS]",
		"[FINMGT][FINBYR][JULY DEBITS]",
		"[FINMGT][FINBYR][AUGUST DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DEBITS]",
		"[FINMGT][FINBYR][POST YEAR DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual posted debits, i.e. the total posted Beginning Balance, Pre-Year, January, February, March, April, May, June, July, August, September, October, November, December, and Post Year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][POST YEAR DISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual distributed debits, i.e. the total distributed Beginning Balance, Pre-Year, January, February, March, April, May, June, July, August, September, October, November, December, and Post Year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED DEBITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]",
		"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED DEBITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual undistributed debits, i.e. the total undistributed Beg Bal, Pre-Year, January, February, March, April, May, June, July, August, September, October, November, December, and Post Year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL POSTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR CREDITS]",
		"[FINMGT][FINBYR][JANUARY CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY CREDITS]",
		"[FINMGT][FINBYR][MARCH CREDITS]",
		"[FINMGT][FINBYR][APRIL CREDITS]",
		"[FINMGT][FINBYR][MAY CREDITS]",
		"[FINMGT][FINBYR][JUNE CREDITS]",
		"[FINMGT][FINBYR][JULY CREDITS]",
		"[FINMGT][FINBYR][AUGUST CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER CREDITS]",
		"[FINMGT][FINBYR][OCTOBER CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER CREDITS]",
		"[FINMGT][FINBYR][DECEMBER CREDITS]",
		"[FINMGT][FINBYR][POST YEAR CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual posted credits, i.e. the total posted Beg Bal, Pre-Year, January, February, March, April, May, June, July, August, September, October, November, December, and Post Year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL DISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][POST YEAR DISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual distributed credits, i.e. the total distributed Beg Bal, Pre-Year, January, February, March, April, May, June, July, August, September, October, November, December, and Post Year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("FINMGT","ANNUAL UNDISTRIBUTED CREDITS");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBYR");
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s",
		"[FINMGT][FINBYR][BEGINNING BALANCE UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][PRE-YEAR UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]",
		"[FINMGT][FINBYR][POST YEAR UNDISTRIBUTED CREDITS]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the total annual undistributed credits, i.e. the total undistributed Beg Bal, Pre-Year, January, February, March, April, May, June, July, August, September, October, November, December, and Post Year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("FINMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
}
