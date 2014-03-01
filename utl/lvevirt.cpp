/* lvevirt.c - Makes LVEMGT Virtual Fields */
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

void LVEMGTCreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];

	d=DFvirtualNEW("LVEMGT","TOTAL LEAVE EARNED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("LVEMGT");
	i->file=stralloc("LVEMSTR");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=8;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"([LVEMGT][LVEMSTR][PROCESSED EARNINGS]+[LVEMGT][LVEMSTR][UNPROCESSED EARNINGS])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field Calculates the Total Leave Earned from the addition of Processed Earnings and Unprocessed Earnings for the Leave Description Identification.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("LVEMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("LVEMGT","TOTAL LEAVE USED");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("LVEMGT");
	i->file=stralloc("LVEMSTR");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=8;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"([LVEMGT][LVEMSTR][PROCESSED USED]+[LVEMGT][LVEMSTR][UNPROCESSED USED])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field Calculates the Total Leave Used from the addition of Processed Earnings and Unprocessed Earnings for the Leave Description Identification.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("LVEMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW("LVEMGT","TOTAL LEAVE BALANCE");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("LVEMGT");
	i->file=stralloc("LVEMSTR");
	d->vtype=0;
	d->num=1;
	d->type=SDECIMALV;
	d->length=8;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"([LVEMGT][LVEMSTR][BEGINNING BALANCE]+[TOTAL LEAVE EARNED]-[TOTAL LEAVE USED])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field Calculates the Total Leave Balance from the formula of Beginning Balance + Total Leave Earned - Total Leave Used for the Leave Description Identification.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("LVEMGT",d);
	if(d!=NULL) FreeDFvirtual(d);
}
