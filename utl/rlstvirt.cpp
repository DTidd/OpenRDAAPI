/* rlstvirt.c - Makes RLSTMGT Virtual Fields */
#include <cstdio>





#include <rptgen.hpp>
#include <mkvir.hpp>

void RLSTMGTCreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];
	int field_length=0;

	d=DFvirtualNEW("RLSTMGT","OWNER NAME");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("TAXCLT");
	i->file=stralloc("TAXMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("TAXCLT","TAXMST","NAME");
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
		"[TAXCLT][TAXMST][NAME]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field reflects the Owner's Name.  Thi",
		"s virtual field is used to place this value in the",
		" BROWSE LIST.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([RLSTMGT][RESTMST][INTEREST AGED BALANCE #1]+[RLS",
		"TMGT][RESTMST][INTEREST AGED BALANCE #2]+[RLSTMGT]",
		"[RESTMST][INTEREST AGED BALANCE #3]+[RLSTMGT][REST",
		"MST][INTEREST AGED BALANCE #4]+[RLSTMGT][RESTMST][",
		"UNPROCESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the interest",
		" amount for a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE NAME");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("RLSTMGT","RESTMST","NAME");
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
		"[RLSTMGT][RESTMST][NAME]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field reflects the [RLSTMGT][RESTMST]",
		"[NAME].  It is used to place that data in other ma",
		"intain master's BROWSE LIST's.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([RLSTMGT][RESTOWN][INTEREST AGED BALANCE #1]+[RLS",
		"TMGT][RESTOWN][INTEREST AGED BALANCE #2]+[RLSTMGT]",
		"[RESTOWN][INTEREST AGED BALANCE #3]+[RLSTMGT][REST",
		"OWN][INTEREST AGED BALANCE #4]+[RLSTMGT][RESTOWN][",
		"UNPROCESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the interest",
		" amount for an owner of a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([RLSTMGT][RESTOWN][PENALTY AGED BALANCE #1]+[RLST",
		"MGT][RESTOWN][PENALTY AGED BALANCE #2]+[RLSTMGT][R",
		"ESTOWN][PENALTY AGED BALANCE #3]+[RLSTMGT][RESTOWN",
		"][PENALTY AGED BALANCE #4]+[RLSTMGT][RESTOWN][UNPR",
		"OCESSED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for an owner of a piece of real estat",
		"e.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([RLSTMGT][RESTOWN][TAX AGED BALANCE #1]+[RLSTMGT]",
		"[RESTOWN][TAX AGED BALANCE #2]+[RLSTMGT][RESTOWN][",
		"TAX AGED BALANCE #3]+[RLSTMGT][RESTOWN][TAX AGED B",
		"ALANCE #4]+[RLSTMGT][RESTOWN][UNPROCESSED TAX AMOU",
		"NT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes for an owner of a piece of real esta",
		"te.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([REAL ESTATE OWNER TAX AMOUNT TOTAL]+[REAL ESTATE",
		" OWNER PENALTY AMOUNT TOTAL]+[REAL ESTATE OWNER IN",
		"TEREST AMOUNT TOTAL]-[RLSTMGT][RESTOWN][UNPROCESSE",
		"D PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed by an owner on a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([REAL ESTATE OWNER YEAR TAX TOTAL]+[REAL ESTATE O",
		"WNER YEAR PENALTY TOTAL]+[REAL ESTATE OWNER YEAR I",
		"NTEREST TOTAL]-[REAL ESTATE OWNER YEAR PAID TOTAL]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the owner's ",
		"balance in a given year for a piece of real estate",
		".  It includes both processed and unprocessed amou",
		"nts.  This balance doesn't reflect the amount owed",
		" on a piece of real estate, since the payments mig",
		"ht have been collected in another year.  This mere",
		"ly show's the balance of a particular year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY INTEREST AMOUNT]+[RLST",
		"MGT][RESTOYR][FEBRUARY INTEREST AMOUNT]+[RLSTMGT][",
		"RESTOYR][MARCH INTEREST AMOUNT]+[RLSTMGT][RESTOYR]",
		"[APRIL INTEREST AMOUNT]+[RLSTMGT][RESTOYR][MAY INT",
		"EREST AMOUNT]+[RLSTMGT][RESTOYR][JUNE INTEREST AMO",
		"UNT]+[RLSTMGT][RESTOYR][JULY INTEREST AMOUNT]+[RLS",
		"TMGT][RESTOYR][AUGUST INTEREST AMOUNT]+[RLSTMGT][R",
		"ESTOYR][SEPTEMBER INTEREST AMOUNT]+[RLSTMGT][RESTO",
		"YR][OCTOBER INTEREST AMOUNT]+[RLSTMGT][RESTOYR][NO",
		"VEMBER INTEREST AMOUNT]+[RLSTMGT][RESTOYR][DECEMBE",
		"R INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for an owner of a piece o",
		"f real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([REAL ESTATE OWNER YEAR INTEREST AMOUNT TOTAL]+[R",
		"EAL ESTATE OWNER YEAR UNPROCESSED INTEREST AMOUNT ",
		"TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for an owner of a piece of real est",
		"ate.  It includes processed and unprocessed intere",
		"st.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY PAID AMOUNT]+[RLSTMGT]",
		"[RESTOYR][FEBRUARY PAID AMOUNT]+[RLSTMGT][RESTOYR]",
		"[MARCH PAID AMOUNT]+[RLSTMGT][RESTOYR][APRIL PAID ",
		"AMOUNT]+[RLSTMGT][RESTOYR][MAY PAID AMOUNT]+[RLSTM",
		"GT][RESTOYR][JUNE PAID AMOUNT]+[RLSTMGT][RESTOYR][",
		"JULY PAID AMOUNT]+[RLSTMGT][RESTOYR][AUGUST PAID A",
		"MOUNT]+[RLSTMGT][RESTOYR][SEPTEMBER PAID AMOUNT]+[",
		"RLSTMGT][RESTOYR][OCTOBER PAID AMOUNT]+[RLSTMGT][R",
		"ESTOYR][NOVEMBER PAID AMOUNT]+[RLSTMGT][RESTOYR][D",
		"ECEMBER PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for an owner of a piece of re",
		"al estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([REAL ESTATE OWNER YEAR PAID AMOUNT TOTAL]+[REAL ",
		"ESTATE OWNER YEAR UNPROCESSED PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for an owner of a piece of real estate.",
		"  It includes both processed and unprocessed payme",
		"nts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY PENALTY AMOUNT]+[RLSTM",
		"GT][RESTOYR][FEBRUARY PENALTY AMOUNT]+[RLSTMGT][RE",
		"STOYR][MARCH PENALTY AMOUNT]+[RLSTMGT][RESTOYR][AP",
		"RIL PENALTY AMOUNT]+[RLSTMGT][RESTOYR][MAY PENALTY",
		" AMOUNT]+[RLSTMGT][RESTOYR][JUNE PENALTY AMOUNT]+[",
		"RLSTMGT][RESTOYR][JULY PENALTY AMOUNT]+[RLSTMGT][R",
		"ESTOYR][AUGUST PENALTY AMOUNT]+[RLSTMGT][RESTOYR][",
		"SEPTEMBER PENALTY AMOUNT]+[RLSTMGT][RESTOYR][OCTOB",
		"ER PENALTY AMOUNT]+[RLSTMGT][RESTOYR][NOVEMBER PEN",
		"ALTY AMOUNT]+[RLSTMGT][RESTOYR][DECEMBER PENALTY A",
		"MOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for an owner of a piece of",
		" real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([REAL ESTATE OWNER YEAR PENALTY AMOUNT TOTAL]+[RE",
		"AL ESTATE OWNER YEAR UNPROCESSED PENALTY AMOUNT TO",
		"TAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for an owner of a piece of real esta",
		"te.  It includes both processed and unprocessed pe",
		"nalties.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([REAL ESTATE OWNER YEAR TAX AMOUNT TOTAL]+[REAL E",
		"STATE OWNER YEAR PENALTY AMOUNT TOTAL]+[REAL ESTAT",
		"E OWNER YEAR INTEREST AMOUNT TOTAL]-[REAL ESTATE O",
		"WNER YEAR PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" an owner of a piece of real estate in a given yea",
		"r.  This includes only processed amounts.  Remembe",
		"r, payments may have been collected in another yea",
		"r.  This is only the annual balance, not the balan",
		"ce owed on the Real Estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY TAX AMOUNT]+[RLSTMGT][",
		"RESTOYR][FEBRUARY TAX AMOUNT]+[RLSTMGT][RESTOYR][M",
		"ARCH TAX AMOUNT]+[RLSTMGT][RESTOYR][APRIL TAX AMOU",
		"NT]+[RLSTMGT][RESTOYR][MAY TAX AMOUNT]+[RLSTMGT][R",
		"ESTOYR][JUNE TAX AMOUNT]+[RLSTMGT][RESTOYR][JULY T",
		"AX AMOUNT]+[RLSTMGT][RESTOYR][AUGUST TAX AMOUNT]+[",
		"RLSTMGT][RESTOYR][SEPTEMBER TAX AMOUNT]+[RLSTMGT][",
		"RESTOYR][OCTOBER TAX AMOUNT]+[RLSTMGT][RESTOYR][NO",
		"VEMBER TAX AMOUNT]+[RLSTMGT][RESTOYR][DECEMBER TAX",
		" AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for an owner of a piece of rea",
		"l estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([REAL ESTATE OWNER YEAR TAX AMOUNT TOTAL]+[REAL E",
		"STATE OWNER YEAR UNPROCESSED TAX AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for an owner of a piece of real estate",
		".  It includes processed and unprocessed taxes.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY UNPROCESSED INTEREST A",
		"MOUNT]+[RLSTMGT][RESTOYR][FEBRUARY UNPROCESSED INT",
		"EREST AMOUNT]+[RLSTMGT][RESTOYR][MARCH UNPROCESSED",
		" INTEREST AMOUNT]+[RLSTMGT][RESTOYR][APRIL UNPROCE",
		"SSED INTEREST AMOUNT]+[RLSTMGT][RESTOYR][MAY UNPRO",
		"CESSED INTEREST AMOUNT]+[RLSTMGT][RESTOYR][JUNE UN",
		"PROCESSED INTEREST AMOUNT]+[RLSTMGT][RESTOYR][JULY",
		" UNPROCESSED INTEREST AMOUNT]+[RLSTMGT][RESTOYR][A",
		"UGUST UNPROCESSED INTEREST AMOUNT]+[RLSTMGT][RESTO",
		"YR][SEPTEMBER UNPROCESSED INTEREST AMOUNT]+[RLSTMG",
		"T][RESTOYR][OCTOBER UNPROCESSED INTEREST AMOUNT]+[",
		"RLSTMGT][RESTOYR][NOVEMBER UNPROCESSED INTEREST AM",
		"OUNT]+[RLSTMGT][RESTOYR][DECEMBER UNPROCESSED INTE",
		"REST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for an owner of a piece",
		" of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY UNPROCESSED PAID AMOUN",
		"T]+[RLSTMGT][RESTYEAR][FEBRUARY UNPROCESSED PAID A",
		"MOUNT]+[RLSTMGT][RESTOYR][MARCH UNPROCESSED PAID A",
		"MOUNT]+[RLSTMGT][RESTOYR][APRIL UNPROCESSED PAID A",
		"MOUNT]+[RLSTMGT][RESTOYR][MAY UNPROCESSED PAID AMO",
		"UNT]+[RLSTMGT][RESTOYR][JUNE UNPROCESSED PAID AMOU",
		"NT]+[RLSTMGT][RESTOYR][JULY UNPROCESSED PAID AMOUN",
		"T]+[RLSTMGT][RESTOYR][AUGUST UNPROCESSED PAID AMOU",
		"NT]+[RLSTMGT][RESTOYR][SEPTEMBER UNPROCESSED PAID ",
		"AMOUNT]+[RLSTMGT][RESTOYR][OCTOBER UNPROCESSED PAI",
		"D AMOUNT]+[RLSTMGT][RESTOYR][NOVEMBER UNPROCESSED ",
		"PAID AMOUNT]+[RLSTMGT][RESTOYR][DECEMBER UNPROCESS",
		"ED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for an owner of a piece of ",
		"real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY UNPROCESSED PENALTY AM",
		"OUNT]+[RLSTMGT][RESTOYR][FEBRUARY UNPROCESSED PENA",
		"LTY AMOUNT]+[RLSTMGT][RESTOYR][MARCH UNPROCESSED P",
		"ENALTY AMOUNT]+[RLSTMGT][RESTOYR][APRIL UNPROCESSE",
		"D PENALTY AMOUNT]+[RLSTMGT][RESTOYR][MAY UNPROCESS",
		"ED PENALTY AMOUNT]+[RLSTMGT][RESTOYR][JUNE UNPROCE",
		"SSED PENALTY AMOUNT]+[RLSTMGT][RESTOYR][JULY UNPRO",
		"CESSED PENALTY AMOUNT]+[RLSTMGT][RESTOYR][AUGUST U",
		"NPROCESSED PENALTY AMOUNT]+[RLSTMGT][RESTOYR][SEPT",
		"EMBER UNPROCESSED PENALTY AMOUNT]+[RLSTMGT][RESTOY",
		"R][OCTOBER UNPROCESSED PENALTY AMOUNT]+[RLSTMGT][R",
		"ESTOYR][NOVEMBER UNPROCESSED PENALTY AMOUNT]+[RLST",
		"MGT][RESTOYR][DECEMBER UNPROCESSED PENALTY AMOUNT]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for an owner of a piece ",
		"of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE OWNER YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTOYR][JANUARY UNPROCESSED TAX AMOUNT",
		"]+[RLSTMGT][RESTOYR][FEBRUARY UNPROCESSED TAX AMOU",
		"NT]+[RLSTMGT][RESTOYR][MARCH UNPROCESSED TAX AMOUN",
		"T]+[RLSTMGT][RESTOYR][APRIL UNPROCESSED TAX AMOUNT",
		"]+[RLSTMGT][RESTOYR][MAY UNPROCESSED TAX AMOUNT]+[",
		"RLSTMGT][RESTOYR][JUNE UNPROCESSED TAX AMOUNT]+[RL",
		"STMGT][RESTOYR][JULY UNPROCESSED TAX AMOUNT]+[RLST",
		"MGT][RESTOYR][AUGUST UNPROCESSED TAX AMOUNT]+[RLST",
		"MGT][RESTOYR][SEPTEMBER UNPROCESSED TAX AMOUNT]+[R",
		"LSTMGT][RESTOYR][OCTOBER UNPROCESSED TAX AMOUNT]+[",
		"RLSTMGT][RESTOYR][NOVEMBER UNPROCESSED TAX AMOUNT]",
		"+[RLSTMGT][RESTOYR][DECEMBER UNPROCESSED TAX AMOUN",
		"T])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for an owner of a piece",
		" of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([RLSTMGT][RESTMST][PENALTY AGED BALANCE #1]+[RLST",
		"MGT][RESTMST][PENALTY AGED BALANCE #2]+[RLSTMGT][R",
		"ESTMST][PENALTY AGED BALANCE #3]+[RLSTMGT][RESTMST",
		"][PENALTY AGED BALANCE #4]+[RLSTMGT][RESTMST][UNPR",
		"OCESSED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([RLSTMGT][RESTMST][TAX AGED BALANCE #1]+[RLSTMGT]",
		"[RESTMST][TAX AGED BALANCE #2]+[RLSTMGT][RESTMST][",
		"TAX AGED BALANCE #3]+[RLSTMGT][RESTMST][TAX AGED B",
		"ALANCE #4]+[RLSTMGT][RESTMST][UNPROCESSED TAX AMOU",
		"NT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes on a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([REAL ESTATE TAX AMOUNT TOTAL]+[REAL ESTATE PENAL",
		"TY AMOUNT TOTAL]+[REAL ESTATE INTEREST AMOUNT TOTA",
		"L]-[RLSTMGT][RESTMST][UNPROCESSED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed on a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE TOTAL VALUE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([RLSTMGT][RESTMST][LAND VALUE]+[RLSTMGT][RESTMST]",
		"[BUILDING AND IMPROVEMENT VALUE])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to define the total val",
		"ue of a piece of property.  The total value and ma",
		"rket value are the same.  This information is dete",
		"rmined for the Real Estate Master File.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([REAL ESTATE YEAR TAX TOTAL]+[REAL ESTATE YEAR PE",
		"NALTY TOTAL]+[REAL ESTATE YEAR INTEREST TOTAL]-[RE",
		"AL ESTATE YEAR PAID TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the balance ",
		"in a given year for a piece of real estate.  It in",
		"cludes both processed and unprocessed amounts.  Th",
		"is balance doesn't reflect the amount owed on a pi",
		"ece of real estate, since the payments might have ",
		"been collected in another year.  This merely show'",
		"s the balance of a particular year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY INTEREST AMOUNT]+[RLS",
		"TMGT][RESTYEAR][FEBRUARY INTEREST AMOUNT]+[RLSTMGT",
		"][RESTYEAR][MARCH INTEREST AMOUNT]+[RLSTMGT][RESTY",
		"EAR][APRIL INTEREST AMOUNT]+[RLSTMGT][RESTYEAR][MA",
		"Y INTEREST AMOUNT]+[RLSTMGT][RESTYEAR][JUNE INTERE",
		"ST AMOUNT]+[RLSTMGT][RESTYEAR][JULY INTEREST AMOUN",
		"T]+[RLSTMGT][RESTYEAR][AUGUST INTEREST AMOUNT]+[RL",
		"STMGT][RESTYEAR][SEPTEMBER INTEREST AMOUNT]+[RLSTM",
		"GT][RESTYEAR][OCTOBER INTEREST AMOUNT]+[RLSTMGT][R",
		"ESTYEAR][NOVEMBER INTEREST AMOUNT]+[RLSTMGT][RESTY",
		"EAR][DECEMBER INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for a piece of real estat",
		"e.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([REAL ESTATE YEAR INTEREST AMOUNT TOTAL]+[REAL ES",
		"TATE YEAR UNPROCESSED INTEREST AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for a piece of real estate.  It inc",
		"ludes processed and unprocessed interest.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY PAID AMOUNT]+[RLSTMGT",
		"][RESTYEAR][FEBRUARY PAID AMOUNT]+[RLSTMGT][RESTYE",
		"AR][MARCH PAID AMOUNT]+[RLSTMGT][RESTYEAR][APRIL P",
		"AID AMOUNT]+[RLSTMGT][RESTYEAR][MAY PAID AMOUNT]+[",
		"RLSTMGT][RESTYEAR][JUNE PAID AMOUNT]+[RLSTMGT][RES",
		"TYEAR][JULY PAID AMOUNT]+[RLSTMGT][RESTYEAR][AUGUS",
		"T PAID AMOUNT]+[RLSTMGT][RESTYEAR][SEPTEMBER PAID ",
		"AMOUNT]+[RLSTMGT][RESTYEAR][OCTOBER PAID AMOUNT]+[",
		"RLSTMGT][RESTYEAR][NOVEMBER PAID AMOUNT]+[RLSTMGT]",
		"[RESTYEAR][DECEMBER PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([REAL ESTATE YEAR PAID AMOUNT TOTAL]+[REAL ESTATE",
		" YEAR UNPROCESSED PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for a piece of real estate.  It include",
		"s both processed and unprocessed payments.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY PENALTY AMOUNT]+[RLST",
		"MGT][RESTYEAR][FEBRUARY PENALTY AMOUNT]+[RLSTMGT][",
		"RESTYEAR][MARCH PENALTY AMOUNT]+[RLSTMGT][RESTYEAR",
		"][APRIL PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][MAY PE",
		"NALTY AMOUNT]+[RLSTMGT][RESTYEAR][JUNE PENALTY AMO",
		"UNT]+[RLSTMGT][RESTYEAR][JULY PENALTY AMOUNT]+[RLS",
		"TMGT][RESTYEAR][AUGUST PENALTY AMOUNT]+[RLSTMGT][R",
		"ESTYEAR][SEPTEMBER PENALTY AMOUNT]+[RLSTMGT][RESTY",
		"EAR][OCTOBER PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][N",
		"OVEMBER PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][DECEMB",
		"ER PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for a piece of real estate",
		".");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([REAL ESTATE YEAR PENALTY AMOUNT TOTAL]+[REAL EST",
		"ATE YEAR UNPROCESSED PENALTY AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for a piece of real estate.  It incl",
		"udes both processed and unprocessed penalties.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([REAL ESTATE YEAR TAX AMOUNT TOTAL]+[REAL ESTATE ",
		"YEAR PENALTY AMOUNT TOTAL]+[REAL ESTATE YEAR INTER",
		"EST AMOUNT TOTAL]-[REAL ESTATE YEAR PAID AMOUNT TO",
		"TAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" a piece of real estate in a given year.  This inc",
		"ludes only processed amounts.  Remember, payments ",
		"may have been collected in another year.  This is ",
		"only the annual balance, not the balance owed on t",
		"he Real Estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY TAX AMOUNT]+[RLSTMGT]",
		"[RESTYEAR][FEBRUARY TAX AMOUNT]+[RLSTMGT][RESTYEAR",
		"][MARCH TAX AMOUNT]+[RLSTMGT][RESTYEAR][APRIL TAX ",
		"AMOUNT]+[RLSTMGT][RESTYEAR][MAY TAX AMOUNT]+[RLSTM",
		"GT][RESTYEAR][JUNE TAX AMOUNT]+[RLSTMGT][RESTYEAR]",
		"[JULY TAX AMOUNT]+[RLSTMGT][RESTYEAR][AUGUST TAX A",
		"MOUNT]+[RLSTMGT][RESTYEAR][SEPTEMBER TAX AMOUNT]+[",
		"RLSTMGT][RESTYEAR][OCTOBER TAX AMOUNT]+[RLSTMGT][R",
		"ESTYEAR][NOVEMBER TAX AMOUNT]+[RLSTMGT][RESTYEAR][",
		"DECEMBER TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([REAL ESTATE YEAR TAX AMOUNT TOTAL]+[REAL ESTATE ",
		"YEAR UNPROCESSED TAX AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for a piece of real estate.  It includ",
		"es processed and unprocessed taxes.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY UNPROCESSED INTEREST ",
		"AMOUNT]+[RLSTMGT][RESTYEAR][FEBRUARY UNPROCESSED I",
		"NTEREST AMOUNT]+[RLSTMGT][RESTYEAR][MARCH UNPROCES",
		"SED INTEREST AMOUNT]+[RLSTMGT][RESTYEAR][APRIL UNP",
		"ROCESSED INTEREST AMOUNT]+[RLSTMGT][RESTYEAR][MAY ",
		"UNPROCESSED INTEREST AMOUNT]+[RLSTMGT][RESTYEAR][J",
		"UNE UNPROCESSED INTEREST AMOUNT]+[RLSTMGT][RESTYEA",
		"R][JULY UNPROCESSED INTEREST AMOUNT]+[RLSTMGT][RES",
		"TYEAR][AUGUST UNPROCESSED INTEREST AMOUNT]+[RLSTMG",
		"T][RESTYEAR][SEPTEMBER UNPROCESSED INTEREST AMOUNT",
		"]+[RLSTMGT][RESTYEAR][OCTOBER UNPROCESSED INTEREST",
		" AMOUNT]+[RLSTMGT][RESTYEAR][NOVEMBER UNPROCESSED ",
		"INTEREST AMOUNT]+[RLSTMGT][RESTYEAR][DECEMBER UNPR",
		"OCESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for a piece of real est",
		"ate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY UNPROCESSED PAID AMOU",
		"NT]+[RLSTMGT][RESTYEAR][FEBRUARY UNPROCESSED PAID ",
		"AMOUNT]+[RLSTMGT][RESTYEAR][MARCH UNPROCESSED PAID",
		" AMOUNT]+[RLSTMGT][RESTYEAR][APRIL UNPROCESSED PAI",
		"D AMOUNT]+[RLSTMGT][RESTYEAR][MAY UNPROCESSED PAID",
		" AMOUNT]+[RLSTMGT][RESTYEAR][JUNE UNPROCESSED PAID",
		" AMOUNT]+[RLSTMGT][RESTYEAR][JULY UNPROCESSED PAID",
		" AMOUNT]+[RLSTMGT][RESTYEAR][AUGUST UNPROCESSED PA",
		"ID AMOUNT]+[RLSTMGT][RESTYEAR][SEPTEMBER UNPROCESS",
		"ED PAID AMOUNT]+[RLSTMGT][RESTYEAR][OCTOBER UNPROC",
		"ESSED PAID AMOUNT]+[RLSTMGT][RESTYEAR][NOVEMBER UN",
		"PROCESSED PAID AMOUNT]+[RLSTMGT][RESTYEAR][DECEMBE",
		"R UNPROCESSED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for a piece of real estate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY UNPROCESSED PENALTY A",
		"MOUNT]+[RLSTMGT][RESTYEAR][FEBRUARY UNPROCESSED PE",
		"NALTY AMOUNT]+[RLSTMGT][RESTYEAR][MARCH UNPROCESSE",
		"D PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][APRIL UNPROC",
		"ESSED PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][MAY UNPR",
		"OCESSED PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][JUNE U",
		"NPROCESSED PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][JUL",
		"Y UNPROCESSED PENALTY AMOUNT]+[RLSTMGT][RESTYEAR][",
		"AUGUST UNPROCESSED PENALTY AMOUNT]+[RLSTMGT][RESTY",
		"EAR][SEPTEMBER UNPROCESSED PENALTY AMOUNT]+[RLSTMG",
		"T][RESTYEAR][OCTOBER UNPROCESSED PENALTY AMOUNT]+[",
		"RLSTMGT][RESTYEAR][NOVEMBER UNPROCESSED PENALTY AM",
		"OUNT]+[RLSTMGT][RESTYEAR][DECEMBER UNPROCESSED PEN",
		"ALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for a piece of real esta",
		"te.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","REAL ESTATE YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([RLSTMGT][RESTYEAR][JANUARY UNPROCESSED TAX AMOUN",
		"T]+[RLSTMGT][RESTYEAR][FEBRUARY UNPROCESSED TAX AM",
		"OUNT]+[RLSTMGT][RESTYEAR][MARCH UNPROCESSED TAX AM",
		"OUNT]+[RLSTMGT][RESTYEAR][APRIL UNPROCESSED TAX AM",
		"OUNT]+[RLSTMGT][RESTYEAR][MAY UNPROCESSED TAX AMOU",
		"NT]+[RLSTMGT][RESTYEAR][JUNE UNPROCESSED TAX AMOUN",
		"T]+[RLSTMGT][RESTYEAR][JULY UNPROCESSED TAX AMOUNT",
		"]+[RLSTMGT][RESTYEAR][AUGUST UNPROCESSED TAX AMOUN",
		"T]+[RLSTMGT][RESTYEAR][SEPTEMBER UNPROCESSED TAX A",
		"MOUNT]+[RLSTMGT][RESTYEAR][OCTOBER UNPROCESSED TAX",
		" AMOUNT]+[RLSTMGT][RESTYEAR][NOVEMBER UNPROCESSED ",
		"TAX AMOUNT]+[RLSTMGT][RESTYEAR][DECEMBER UNPROCESS",
		"ED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for a piece of real est",
		"ate.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("RLSTMGT","TAXABLE VALUE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTMST");
	i->keyname=NULL;
	i->module=stralloc("RLSTMGT");
	i->file=stralloc("RESTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=2;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"IF([RLSTMGT][RESTYEAR][LAND VALUE]!=0.00) THEN",
		"RETURN_VALUE=([RLSTMGT][RESTYEAR][LAND VALUE]+[R",
		"LSTMGT][RESTYEAR][BUILDING AND IMPROVEMENT VALUE]-",
		"[RLSTMGT][RESTYEAR][TAX DEFERRED AMOUNT]) ELSE RE",
		"TURN_VALUE=([RLSTMGT][RESTMST][LAND VALUE]+[RLST",
		"MGT][RESTMST][BUILDING AND IMPROVEMENT VALUE]-[RLS",
		"TMGT][RESTMST][TAX DEFERRED AMOUNT]) FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"This virtual field is a calculation of the Taxable",
		" Value of Real Estate Property from the formula (M",
		"arket Value+Building & Improvements-Defered Amount",
		") and any other factors that are included in the e",
		"xpression.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("RLSTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

}
