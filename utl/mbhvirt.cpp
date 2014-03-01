/* mbhvirt.c - Makes MBLHOME Virtual Fields */
#include <cstdio>





#include <rptgen.hpp>
#include <mkvir.hpp>

void MBLHOMECreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];
	int field_length=0;

	d=DFvirtualNEW("MBLHOME","MOBIL HOME INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([MBLHOME][MBHMST][INTEREST AGED BALANCE #1]+[MBLH",
		"OME][MBHMST][INTEREST AGED BALANCE #2]+[MBLHOME][M",
		"BHMST][INTEREST AGED BALANCE #3]+[MBLHOME][MBHMST]",
		"[INTEREST AGED BALANCE #4]+[MBLHOME][MBHMST][UNPRO",
		"CESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the interest",
		" amount for a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME NAME");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("MBLHOME","MBHMST","NAME");
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
		"[MBLHOME][MBHMST][NAME]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field reflects the [MBLHOME][MBHMST][",
		"NAME].  It is used to place that data in other mai",
		"ntain master's BROWSE LIST's.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([MBLHOME][MBHOWN][INTEREST AGED BALANCE #1]+[MBLH",
		"OME][MBHOWN][INTEREST AGED BALANCE #2]+[MBLHOME][M",
		"BHOWN][INTEREST AGED BALANCE #3]+[MBLHOME][MBHOWN]",
		"[INTEREST AGED BALANCE #4]+[MBLHOME][MBHOWN][UNPRO",
		"CESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the interest",
		" amount for an owner of a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([MBLHOME][MBHOWN][PENALTY AGED BALANCE #1]+[MBLHO",
		"ME][MBHOWN][PENALTY AGED BALANCE #2]+[MBLHOME][MBH",
		"OWN][PENALTY AGED BALANCE #3]+[MBLHOME][MBHOWN][PE",
		"NALTY AGED BALANCE #4]+[MBLHOME][MBHOWN][UNPROCESS",
		"ED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for an owner of a piece of mobil home",
		".");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([MBLHOME][MBHOWN][TAX AGED BALANCE #1]+[MBLHOME][",
		"MBHOWN][TAX AGED BALANCE #2]+[MBLHOME][MBHOWN][TAX",
		" AGED BALANCE #3]+[MBLHOME][MBHOWN][TAX AGED BALAN",
		"CE #4]+[MBLHOME][MBHOWN][UNPROCESSED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes for an owner of a piece of mobil hom",
		"e.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([MOBIL HOME OWNER TAX AMOUNT TOTAL]+[MOBIL HOME O",
		"WNER PENALTY AMOUNT TOTAL]+[MOBIL HOME OWNER INTER",
		"EST AMOUNT TOTAL]-[MBLHOME][MBHOWN][UNPROCESSED PA",
		"ID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed by an owner on a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([MOBIL HOME OWNER YEAR TAX TOTAL]+[MOBIL HOME OWN",
		"ER YEAR PENALTY TOTAL]+[MOBIL HOME OWNER YEAR INTE",
		"REST TOTAL]-[MOBIL HOME OWNER YEAR PAID TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the owner's ",
		"balance in a given year for a piece of mobil home.",
		"  It includes both processed and unprocessed amoun",
		"ts.  This balance doesn't reflect the amount owed ",
		"on a piece of mobil home, since the payments might",
		" have been collected in another year.  This merely",
		" show's the balance of a particular year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY INTEREST AMOUNT]+[MBLHO",
		"ME][MBHOYR][FEBRUARY INTEREST AMOUNT]+[MBLHOME][MB",
		"HOYR][MARCH INTEREST AMOUNT]+[MBLHOME][MBHOYR][APR",
		"IL INTEREST AMOUNT]+[MBLHOME][MBHOYR][MAY INTEREST",
		" AMOUNT]+[MBLHOME][MBHOYR][JUNE INTEREST AMOUNT]+[",
		"MBLHOME][MBHOYR][JULY INTEREST AMOUNT]+[MBLHOME][M",
		"BHOYR][AUGUST INTEREST AMOUNT]+[MBLHOME][MBHOYR][S",
		"EPTEMBER INTEREST AMOUNT]+[MBLHOME][MBHOYR][OCTOBE",
		"R INTEREST AMOUNT]+[MBLHOME][MBHOYR][NOVEMBER INTE",
		"REST AMOUNT]+[MBLHOME][MBHOYR][DECEMBER INTEREST A",
		"MOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for an owner of a piece o",
		"f mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([MOBIL HOME OWNER YEAR INTEREST AMOUNT TOTAL]+[MO",
		"BIL HOME OWNER YEAR UNPROCESSED INTEREST AMOUNT TO",
		"TAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for an owner of a piece of mobil ho",
		"me.  It includes processed and unprocessed interes",
		"t.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY PAID AMOUNT]+[MBLHOME][",
		"MBHOYR][FEBRUARY PAID AMOUNT]+[MBLHOME][MBHOYR][MA",
		"RCH PAID AMOUNT]+[MBLHOME][MBHOYR][APRIL PAID AMOU",
		"NT]+[MBLHOME][MBHOYR][MAY PAID AMOUNT]+[MBLHOME][M",
		"BHOYR][JUNE PAID AMOUNT]+[MBLHOME][MBHOYR][JULY PA",
		"ID AMOUNT]+[MBLHOME][MBHOYR][AUGUST PAID AMOUNT]+[",
		"MBLHOME][MBHOYR][SEPTEMBER PAID AMOUNT]+[MBLHOME][",
		"MBHOYR][OCTOBER PAID AMOUNT]+[MBLHOME][MBHOYR][NOV",
		"EMBER PAID AMOUNT]+[MBLHOME][MBHOYR][DECEMBER PAID",
		" AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for an owner of a piece of mo",
		"bil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([MOBIL HOME OWNER YEAR PAID AMOUNT TOTAL]+[MOBIL ",
		"HOME OWNER YEAR UNPROCESSED PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for an owner of a piece of mobil home. ",
		" It includes both processed and unprocessed paymen",
		"ts.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY PENALTY AMOUNT]+[MBLHOM",
		"E][MBHOYR][FEBRUARY PENALTY AMOUNT]+[MBLHOME][MBHO",
		"YR][MARCH PENALTY AMOUNT]+[MBLHOME][MBHOYR][APRIL ",
		"PENALTY AMOUNT]+[MBLHOME][MBHOYR][MAY PENALTY AMOU",
		"NT]+[MBLHOME][MBHOYR][JUNE PENALTY AMOUNT]+[MBLHOM",
		"E][MBHOYR][JULY PENALTY AMOUNT]+[MBLHOME][MBHOYR][",
		"AUGUST PENALTY AMOUNT]+[MBLHOME][MBHOYR][SEPTEMBER",
		" PENALTY AMOUNT]+[MBLHOME][MBHOYR][OCTOBER PENALTY",
		" AMOUNT]+[MBLHOME][MBHOYR][NOVEMBER PENALTY AMOUNT",
		"]+[MBLHOME][MBHOYR][DECEMBER PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for an owner of a piece of",
		" mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([MOBIL HOME OWNER YEAR PENALTY AMOUNT TOTAL]+[MOB",
		"IL HOME OWNER YEAR UNPROCESSED PENALTY AMOUNT TOTA",
		"L])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for an owner of a piece of mobil hom",
		"e.  It includes both processed and unprocessed pen",
		"alties.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([MOBIL HOME OWNER YEAR TAX AMOUNT TOTAL]+[MOBIL H",
		"OME OWNER YEAR PENALTY AMOUNT TOTAL]+[MOBIL HOME O",
		"WNER YEAR INTEREST AMOUNT TOTAL]-[MOBIL HOME OWNER",
		" YEAR PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" an owner of a piece of mobil home in a given year",
		".  This includes only processed amounts.  Remember",
		", payments may have been collected in another year",
		".  This is only the annual balance, not the balanc",
		"e owed on the Mobil Home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY TAX AMOUNT]+[MBLHOME][M",
		"BHOYR][FEBRUARY TAX AMOUNT]+[MBLHOME][MBHOYR][MARC",
		"H TAX AMOUNT]+[MBLHOME][MBHOYR][APRIL TAX AMOUNT]+",
		"[MBLHOME][MBHOYR][MAY TAX AMOUNT]+[MBLHOME][MBHOYR",
		"][JUNE TAX AMOUNT]+[MBLHOME][MBHOYR][JULY TAX AMOU",
		"NT]+[MBLHOME][MBHOYR][AUGUST TAX AMOUNT]+[MBLHOME]",
		"[MBHOYR][SEPTEMBER TAX AMOUNT]+[MBLHOME][MBHOYR][O",
		"CTOBER TAX AMOUNT]+[MBLHOME][MBHOYR][NOVEMBER TAX ",
		"AMOUNT]+[MBLHOME][MBHOYR][DECEMBER TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for an owner of a piece of mob",
		"il home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([MOBIL HOME OWNER YEAR TAX AMOUNT TOTAL]+[MOBIL H",
		"OME OWNER YEAR UNPROCESSED TAX AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for an owner of a piece of mobil home.",
		"  It includes processed and unprocessed taxes.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY UNPROCESSED INTEREST AM",
		"OUNT]+[MBLHOME][MBHOYR][FEBRUARY UNPROCESSED INTER",
		"EST AMOUNT]+[MBLHOME][MBHOYR][MARCH UNPROCESSED IN",
		"TEREST AMOUNT]+[MBLHOME][MBHOYR][APRIL UNPROCESSED",
		" INTEREST AMOUNT]+[MBLHOME][MBHOYR][MAY UNPROCESSE",
		"D INTEREST AMOUNT]+[MBLHOME][MBHOYR][JUNE UNPROCES",
		"SED INTEREST AMOUNT]+[MBLHOME][MBHOYR][JULY UNPROC",
		"ESSED INTEREST AMOUNT]+[MBLHOME][MBHOYR][AUGUST UN",
		"PROCESSED INTEREST AMOUNT]+[MBLHOME][MBHOYR][SEPTE",
		"MBER UNPROCESSED INTEREST AMOUNT]+[MBLHOME][MBHOYR",
		"][OCTOBER UNPROCESSED INTEREST AMOUNT]+[MBLHOME][M",
		"BHOYR][NOVEMBER UNPROCESSED INTEREST AMOUNT]+[MBLH",
		"OME][MBHOYR][DECEMBER UNPROCESSED INTEREST AMOUNT]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for an owner of a piece",
		" of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY UNPROCESSED PAID AMOUNT",
		"]+[MBLHOME][MBHYEAR][FEBRUARY UNPROCESSED PAID AMO",
		"UNT]+[MBLHOME][MBHOYR][MARCH UNPROCESSED PAID AMOU",
		"NT]+[MBLHOME][MBHOYR][APRIL UNPROCESSED PAID AMOUN",
		"T]+[MBLHOME][MBHOYR][MAY UNPROCESSED PAID AMOUNT]+",
		"[MBLHOME][MBHOYR][JUNE UNPROCESSED PAID AMOUNT]+[M",
		"BLHOME][MBHOYR][JULY UNPROCESSED PAID AMOUNT]+[MBL",
		"HOME][MBHOYR][AUGUST UNPROCESSED PAID AMOUNT]+[MBL",
		"HOME][MBHOYR][SEPTEMBER UNPROCESSED PAID AMOUNT]+[",
		"MBLHOME][MBHOYR][OCTOBER UNPROCESSED PAID AMOUNT]+",
		"[MBLHOME][MBHOYR][NOVEMBER UNPROCESSED PAID AMOUNT",
		"]+[MBLHOME][MBHOYR][DECEMBER UNPROCESSED PAID AMOU",
		"NT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for an owner of a piece of ",
		"mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY UNPROCESSED PENALTY AMO",
		"UNT]+[MBLHOME][MBHOYR][FEBRUARY UNPROCESSED PENALT",
		"Y AMOUNT]+[MBLHOME][MBHOYR][MARCH UNPROCESSED PENA",
		"LTY AMOUNT]+[MBLHOME][MBHOYR][APRIL UNPROCESSED PE",
		"NALTY AMOUNT]+[MBLHOME][MBHOYR][MAY UNPROCESSED PE",
		"NALTY AMOUNT]+[MBLHOME][MBHOYR][JUNE UNPROCESSED P",
		"ENALTY AMOUNT]+[MBLHOME][MBHOYR][JULY UNPROCESSED ",
		"PENALTY AMOUNT]+[MBLHOME][MBHOYR][AUGUST UNPROCESS",
		"ED PENALTY AMOUNT]+[MBLHOME][MBHOYR][SEPTEMBER UNP",
		"ROCESSED PENALTY AMOUNT]+[MBLHOME][MBHOYR][OCTOBER",
		" UNPROCESSED PENALTY AMOUNT]+[MBLHOME][MBHOYR][NOV",
		"EMBER UNPROCESSED PENALTY AMOUNT]+[MBLHOME][MBHOYR",
		"][DECEMBER UNPROCESSED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for an owner of a piece ",
		"of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME OWNER YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHOYR][JANUARY UNPROCESSED TAX AMOUNT]",
		"+[MBLHOME][MBHOYR][FEBRUARY UNPROCESSED TAX AMOUNT",
		"]+[MBLHOME][MBHOYR][MARCH UNPROCESSED TAX AMOUNT]+",
		"[MBLHOME][MBHOYR][APRIL UNPROCESSED TAX AMOUNT]+[M",
		"BLHOME][MBHOYR][MAY UNPROCESSED TAX AMOUNT]+[MBLHO",
		"ME][MBHOYR][JUNE UNPROCESSED TAX AMOUNT]+[MBLHOME]",
		"[MBHOYR][JULY UNPROCESSED TAX AMOUNT]+[MBLHOME][MB",
		"HOYR][AUGUST UNPROCESSED TAX AMOUNT]+[MBLHOME][MBH",
		"OYR][SEPTEMBER UNPROCESSED TAX AMOUNT]+[MBLHOME][M",
		"BHOYR][OCTOBER UNPROCESSED TAX AMOUNT]+[MBLHOME][M",
		"BHOYR][NOVEMBER UNPROCESSED TAX AMOUNT]+[MBLHOME][",
		"MBHOYR][DECEMBER UNPROCESSED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for an owner of a piece",
		" of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([MBLHOME][MBHMST][PENALTY AGED BALANCE #1]+[MBLHO",
		"ME][MBHMST][PENALTY AGED BALANCE #2]+[MBLHOME][MBH",
		"MST][PENALTY AGED BALANCE #3]+[MBLHOME][MBHMST][PE",
		"NALTY AGED BALANCE #4]+[MBLHOME][MBHMST][UNPROCESS",
		"ED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([MBLHOME][MBHMST][TAX AGED BALANCE #1]+[MBLHOME][",
		"MBHMST][TAX AGED BALANCE #2]+[MBLHOME][MBHMST][TAX",
		" AGED BALANCE #3]+[MBLHOME][MBHMST][TAX AGED BALAN",
		"CE #4]+[MBLHOME][MBHMST][UNPROCESSED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes on a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([MOBIL HOME TAX AMOUNT TOTAL]+[MOBIL HOME PENALTY",
		" AMOUNT TOTAL]+[MOBIL HOME INTEREST AMOUNT TOTAL]-",
		"[MBLHOME][MBHMST][UNPROCESSED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed on a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME TOTAL VALUE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([MBLHOME][MBHMST][VALUE]+[MBLHOME][MBHMST][IMPROV",
		"EMENT VALUE])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to define the total val",
		"ue of a piece of property.  The total value and ma",
		"rket value are the same.  This information is dete",
		"rmined for the Mobil Home Master File.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([MOBIL HOME YEAR TAX TOTAL]+[MOBIL HOME YEAR PENA",
		"LTY TOTAL]+[MOBIL HOME YEAR INTEREST TOTAL]-[MOBIL",
		" HOME YEAR PAID TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the balance ",
		"in a given year for a piece of mobil home.  It inc",
		"ludes both processed and unprocessed amounts.  Thi",
		"s balance doesn't reflect the amount owed on a pie",
		"ce of mobil home, since the payments might have be",
		"en collected in another year.  This merely show's ",
		"the balance of a particular year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY INTEREST AMOUNT]+[MBLH",
		"OME][MBHYEAR][FEBRUARY INTEREST AMOUNT]+[MBLHOME][",
		"MBHYEAR][MARCH INTEREST AMOUNT]+[MBLHOME][MBHYEAR]",
		"[APRIL INTEREST AMOUNT]+[MBLHOME][MBHYEAR][MAY INT",
		"EREST AMOUNT]+[MBLHOME][MBHYEAR][JUNE INTEREST AMO",
		"UNT]+[MBLHOME][MBHYEAR][JULY INTEREST AMOUNT]+[MBL",
		"HOME][MBHYEAR][AUGUST INTEREST AMOUNT]+[MBLHOME][M",
		"BHYEAR][SEPTEMBER INTEREST AMOUNT]+[MBLHOME][MBHYE",
		"AR][OCTOBER INTEREST AMOUNT]+[MBLHOME][MBHYEAR][NO",
		"VEMBER INTEREST AMOUNT]+[MBLHOME][MBHYEAR][DECEMBE",
		"R INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for a piece of mobil home",
		".");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([MOBIL HOME YEAR INTEREST AMOUNT TOTAL]+[MOBIL HO",
		"ME YEAR UNPROCESSED INTEREST AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for a piece of mobil home.  It incl",
		"udes processed and unprocessed interest.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY PAID AMOUNT]+[MBLHOME]",
		"[MBHYEAR][FEBRUARY PAID AMOUNT]+[MBLHOME][MBHYEAR]",
		"[MARCH PAID AMOUNT]+[MBLHOME][MBHYEAR][APRIL PAID ",
		"AMOUNT]+[MBLHOME][MBHYEAR][MAY PAID AMOUNT]+[MBLHO",
		"ME][MBHYEAR][JUNE PAID AMOUNT]+[MBLHOME][MBHYEAR][",
		"JULY PAID AMOUNT]+[MBLHOME][MBHYEAR][AUGUST PAID A",
		"MOUNT]+[MBLHOME][MBHYEAR][SEPTEMBER PAID AMOUNT]+[",
		"MBLHOME][MBHYEAR][OCTOBER PAID AMOUNT]+[MBLHOME][M",
		"BHYEAR][NOVEMBER PAID AMOUNT]+[MBLHOME][MBHYEAR][D",
		"ECEMBER PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([MOBIL HOME YEAR PAID AMOUNT TOTAL]+[MOBIL HOME Y",
		"EAR UNPROCESSED PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for a piece of mobil home.  It includes",
		" both processed and unprocessed payments.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY PENALTY AMOUNT]+[MBLHO",
		"ME][MBHYEAR][FEBRUARY PENALTY AMOUNT]+[MBLHOME][MB",
		"HYEAR][MARCH PENALTY AMOUNT]+[MBLHOME][MBHYEAR][AP",
		"RIL PENALTY AMOUNT]+[MBLHOME][MBHYEAR][MAY PENALTY",
		" AMOUNT]+[MBLHOME][MBHYEAR][JUNE PENALTY AMOUNT]+[",
		"MBLHOME][MBHYEAR][JULY PENALTY AMOUNT]+[MBLHOME][M",
		"BHYEAR][AUGUST PENALTY AMOUNT]+[MBLHOME][MBHYEAR][",
		"SEPTEMBER PENALTY AMOUNT]+[MBLHOME][MBHYEAR][OCTOB",
		"ER PENALTY AMOUNT]+[MBLHOME][MBHYEAR][NOVEMBER PEN",
		"ALTY AMOUNT]+[MBLHOME][MBHYEAR][DECEMBER PENALTY A",
		"MOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([MOBIL HOME YEAR PENALTY AMOUNT TOTAL]+[MOBIL HOM",
		"E YEAR UNPROCESSED PENALTY AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for a piece of mobil home.  It inclu",
		"des both processed and unprocessed penalties.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([MOBIL HOME YEAR TAX AMOUNT TOTAL]+[MOBIL HOME YE",
		"AR PENALTY AMOUNT TOTAL]+[MOBIL HOME YEAR INTEREST",
		" AMOUNT TOTAL]-[MOBIL HOME YEAR PAID AMOUNT TOTAL]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" a piece of mobil home in a given year.  This incl",
		"udes only processed amounts.  Remember, payments m",
		"ay have been collected in another year.  This is o",
		"nly the annual balance, not the balance owed on th",
		"e Mobil Home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY TAX AMOUNT]+[MBLHOME][",
		"MBHYEAR][FEBRUARY TAX AMOUNT]+[MBLHOME][MBHYEAR][M",
		"ARCH TAX AMOUNT]+[MBLHOME][MBHYEAR][APRIL TAX AMOU",
		"NT]+[MBLHOME][MBHYEAR][MAY TAX AMOUNT]+[MBLHOME][M",
		"BHYEAR][JUNE TAX AMOUNT]+[MBLHOME][MBHYEAR][JULY T",
		"AX AMOUNT]+[MBLHOME][MBHYEAR][AUGUST TAX AMOUNT]+[",
		"MBLHOME][MBHYEAR][SEPTEMBER TAX AMOUNT]+[MBLHOME][",
		"MBHYEAR][OCTOBER TAX AMOUNT]+[MBLHOME][MBHYEAR][NO",
		"VEMBER TAX AMOUNT]+[MBLHOME][MBHYEAR][DECEMBER TAX",
		" AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([MOBIL HOME YEAR TAX AMOUNT TOTAL]+[MOBIL HOME YE",
		"AR UNPROCESSED TAX AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for a piece of mobil home.  It include",
		"s processed and unprocessed taxes.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY UNPROCESSED INTEREST A",
		"MOUNT]+[MBLHOME][MBHYEAR][FEBRUARY UNPROCESSED INT",
		"EREST AMOUNT]+[MBLHOME][MBHYEAR][MARCH UNPROCESSED",
		" INTEREST AMOUNT]+[MBLHOME][MBHYEAR][APRIL UNPROCE",
		"SSED INTEREST AMOUNT]+[MBLHOME][MBHYEAR][MAY UNPRO",
		"CESSED INTEREST AMOUNT]+[MBLHOME][MBHYEAR][JUNE UN",
		"PROCESSED INTEREST AMOUNT]+[MBLHOME][MBHYEAR][JULY",
		" UNPROCESSED INTEREST AMOUNT]+[MBLHOME][MBHYEAR][A",
		"UGUST UNPROCESSED INTEREST AMOUNT]+[MBLHOME][MBHYE",
		"AR][SEPTEMBER UNPROCESSED INTEREST AMOUNT]+[MBLHOM",
		"E][MBHYEAR][OCTOBER UNPROCESSED INTEREST AMOUNT]+[",
		"MBLHOME][MBHYEAR][NOVEMBER UNPROCESSED INTEREST AM",
		"OUNT]+[MBLHOME][MBHYEAR][DECEMBER UNPROCESSED INTE",
		"REST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for a piece of mobil ho",
		"me.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY UNPROCESSED PAID AMOUN",
		"T]+[MBLHOME][MBHYEAR][FEBRUARY UNPROCESSED PAID AM",
		"OUNT]+[MBLHOME][MBHYEAR][MARCH UNPROCESSED PAID AM",
		"OUNT]+[MBLHOME][MBHYEAR][APRIL UNPROCESSED PAID AM",
		"OUNT]+[MBLHOME][MBHYEAR][MAY UNPROCESSED PAID AMOU",
		"NT]+[MBLHOME][MBHYEAR][JUNE UNPROCESSED PAID AMOUN",
		"T]+[MBLHOME][MBHYEAR][JULY UNPROCESSED PAID AMOUNT",
		"]+[MBLHOME][MBHYEAR][AUGUST UNPROCESSED PAID AMOUN",
		"T]+[MBLHOME][MBHYEAR][SEPTEMBER UNPROCESSED PAID A",
		"MOUNT]+[MBLHOME][MBHYEAR][OCTOBER UNPROCESSED PAID",
		" AMOUNT]+[MBLHOME][MBHYEAR][NOVEMBER UNPROCESSED P",
		"AID AMOUNT]+[MBLHOME][MBHYEAR][DECEMBER UNPROCESSE",
		"D PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for a piece of mobil home.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY UNPROCESSED PENALTY AM",
		"OUNT]+[MBLHOME][MBHYEAR][FEBRUARY UNPROCESSED PENA",
		"LTY AMOUNT]+[MBLHOME][MBHYEAR][MARCH UNPROCESSED P",
		"ENALTY AMOUNT]+[MBLHOME][MBHYEAR][APRIL UNPROCESSE",
		"D PENALTY AMOUNT]+[MBLHOME][MBHYEAR][MAY UNPROCESS",
		"ED PENALTY AMOUNT]+[MBLHOME][MBHYEAR][JUNE UNPROCE",
		"SSED PENALTY AMOUNT]+[MBLHOME][MBHYEAR][JULY UNPRO",
		"CESSED PENALTY AMOUNT]+[MBLHOME][MBHYEAR][AUGUST U",
		"NPROCESSED PENALTY AMOUNT]+[MBLHOME][MBHYEAR][SEPT",
		"EMBER UNPROCESSED PENALTY AMOUNT]+[MBLHOME][MBHYEA",
		"R][OCTOBER UNPROCESSED PENALTY AMOUNT]+[MBLHOME][M",
		"BHYEAR][NOVEMBER UNPROCESSED PENALTY AMOUNT]+[MBLH",
		"OME][MBHYEAR][DECEMBER UNPROCESSED PENALTY AMOUNT]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for a piece of mobil hom",
		"e.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","MOBIL HOME YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([MBLHOME][MBHYEAR][JANUARY UNPROCESSED TAX AMOUNT",
		"]+[MBLHOME][MBHYEAR][FEBRUARY UNPROCESSED TAX AMOU",
		"NT]+[MBLHOME][MBHYEAR][MARCH UNPROCESSED TAX AMOUN",
		"T]+[MBLHOME][MBHYEAR][APRIL UNPROCESSED TAX AMOUNT",
		"]+[MBLHOME][MBHYEAR][MAY UNPROCESSED TAX AMOUNT]+[",
		"MBLHOME][MBHYEAR][JUNE UNPROCESSED TAX AMOUNT]+[MB",
		"LHOME][MBHYEAR][JULY UNPROCESSED TAX AMOUNT]+[MBLH",
		"OME][MBHYEAR][AUGUST UNPROCESSED TAX AMOUNT]+[MBLH",
		"OME][MBHYEAR][SEPTEMBER UNPROCESSED TAX AMOUNT]+[M",
		"BLHOME][MBHYEAR][OCTOBER UNPROCESSED TAX AMOUNT]+[",
		"MBLHOME][MBHYEAR][NOVEMBER UNPROCESSED TAX AMOUNT]",
		"+[MBLHOME][MBHYEAR][DECEMBER UNPROCESSED TAX AMOUN",
		"T])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for a piece of mobil ho",
		"me.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","OWNER NAME");
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
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("MBLHOME","TAXABLE VALUE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHMST");
	i->keyname=NULL;
	i->module=stralloc("MBLHOME");
	i->file=stralloc("MBHYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=2;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"IF([MBLHOME][MBHYEAR][VALUE]!=0.00) THEN R",
		"ETURN_VALUE=([MBLHOME][MBHYEAR][VALUE]+[MBL",
		"HOME][MBHYEAR][IMPROVEMENT VALUE]-[MBLHOME][MBHYEA",
		"R][TAX DEFERRED AMOUNT]) ELSE RETURN_VALUE=([MBLH",
		"OME][MBHMST][VALUE]+[MBLHOME][MBHMST][IMPRO",
		"VEMENT VALUE]-[MBLHOME][MBHMST][TAX DEFERRED AMOUN",
		"T]) FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"This virtual field is a calculation of the Taxable",
		" Value of Mobil Home Property from the formula (Ma",
		"rket Value+Building & Improvements-Defered Amount)",
		" and any other factors that are included in the ex",
		"pression.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("MBLHOME",d);
	if(d!=NULL) FreeDFvirtual(d);

}
