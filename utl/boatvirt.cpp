/* boatvirt.c - Makes BOATMGT Virtual Fields */
#include <cstdio>
#include <rptgen.hpp>
#include <mkvir.hpp>

void BOATMGTCreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];
	int field_length=0;

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([BOATMGT][BOATMST][INTEREST AGED BALANCE #1]+[BOA",
		"TMGT][BOATMST][INTEREST AGED BALANCE #2]+[BOATMGT]",
		"[BOATMST][INTEREST AGED BALANCE #3]+[BOATMGT][BOAT",
		"MST][INTEREST AGED BALANCE #4]+[BOATMGT][BOATMST][",
		"UNPROCESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the interest",
		" amount for a piece of boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);



	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT NAME");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("BOATMGT","BOATMST","NAME");
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
		"[BOATMGT][BOATMST][NAME]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field reflects the [BOATMGT][BOATMST]",
		"[NAME].  It is used to place that data in other ma",
		"intain master's BROWSE LIST's.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([BOATMGT][BOATOWN][INTEREST AGED BALANCE #1]+[BOA",
		"TMGT][BOATOWN][INTEREST AGED BALANCE #2]+[BOATMGT]",
		"[BOATOWN][INTEREST AGED BALANCE #3]+[BOATMGT][BOAT",
		"OWN][INTEREST AGED BALANCE #4]+[BOATMGT][BOATOWN][",
		"UNPROCESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the interest",
		" amount for an owner of a piece of boat management",
		".");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([BOATMGT][BOATOWN][PENALTY AGED BALANCE #1]+[BOAT",
		"MGT][BOATOWN][PENALTY AGED BALANCE #2]+[BOATMGT][B",
		"OATOWN][PENALTY AGED BALANCE #3]+[BOATMGT][BOATOWN",
		"][PENALTY AGED BALANCE #4]+[BOATMGT][BOATOWN][UNPR",
		"OCESSED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for an owner of a piece of boat manag",
		"ement.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([BOATMGT][BOATOWN][TAX AGED BALANCE #1]+[BOATMGT]",
		"[BOATOWN][TAX AGED BALANCE #2]+[BOATMGT][BOATOWN][",
		"TAX AGED BALANCE #3]+[BOATMGT][BOATOWN][TAX AGED B",
		"ALANCE #4]+[BOATMGT][BOATOWN][UNPROCESSED TAX AMOU",
		"NT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes for an owner of a piece of boat mana",
		"gement.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([BOAT MANAGEMENT OWNER TAX AMOUNT TOTAL]+[BOAT MA",
		"NAGEMENT OWNER PENALTY AMOUNT TOTAL]+[BOAT MANAGEM",
		"ENT OWNER INTEREST AMOUNT TOTAL]-[BOATMGT][BOATOWN",
		"][UNPROCESSED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed by an owner on a piece of boat managemen",
		"t.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([BOAT MANAGEMENT OWNER YEAR TAX TOTAL]+[BOAT MANA",
		"GEMENT OWNER YEAR PENALTY TOTAL]+[BOAT MANAGEMENT ",
		"OWNER YEAR INTEREST TOTAL]-[BOAT MANAGEMENT OWNER ",
		"YEAR PAID TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the owner's ",
		"balance in a given year for a piece of boat manage",
		"ment.  It includes both processed and unprocessed ",
		"amounts.  This balance doesn't reflect the amount ",
		"owed on a piece of boat management, since the paym",
		"ents might have been collected in another year.  T",
		"his merely show's the balance of a particular year",
		".");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY INTEREST AMOUNT]+[BOAT",
		"MGT][BOATOYR][FEBRUARY INTEREST AMOUNT]+[BOATMGT][",
		"BOATOYR][MARCH INTEREST AMOUNT]+[BOATMGT][BOATOYR]",
		"[APRIL INTEREST AMOUNT]+[BOATMGT][BOATOYR][MAY INT",
		"EREST AMOUNT]+[BOATMGT][BOATOYR][JUNE INTEREST AMO",
		"UNT]+[BOATMGT][BOATOYR][JULY INTEREST AMOUNT]+[BOA",
		"TMGT][BOATOYR][AUGUST INTEREST AMOUNT]+[BOATMGT][B",
		"OATOYR][SEPTEMBER INTEREST AMOUNT]+[BOATMGT][BOATO",
		"YR][OCTOBER INTEREST AMOUNT]+[BOATMGT][BOATOYR][NO",
		"VEMBER INTEREST AMOUNT]+[BOATMGT][BOATOYR][DECEMBE",
		"R INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for an owner of a piece o",
		"f boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([BOAT MANAGEMENT OWNER YEAR INTEREST AMOUNT TOTAL",
		"]+[BOAT MANAGEMENT OWNER YEAR UNPROCESSED INTEREST",
		" AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for an owner of a piece of boat man",
		"agement.  It includes processed and unprocessed in",
		"terest.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY PAID AMOUNT]+[BOATMGT]",
		"[BOATOYR][FEBRUARY PAID AMOUNT]+[BOATMGT][BOATOYR]",
		"[MARCH PAID AMOUNT]+[BOATMGT][BOATOYR][APRIL PAID ",
		"AMOUNT]+[BOATMGT][BOATOYR][MAY PAID AMOUNT]+[BOATM",
		"GT][BOATOYR][JUNE PAID AMOUNT]+[BOATMGT][BOATOYR][",
		"JULY PAID AMOUNT]+[BOATMGT][BOATOYR][AUGUST PAID A",
		"MOUNT]+[BOATMGT][BOATOYR][SEPTEMBER PAID AMOUNT]+[",
		"BOATMGT][BOATOYR][OCTOBER PAID AMOUNT]+[BOATMGT][B",
		"OATOYR][NOVEMBER PAID AMOUNT]+[BOATMGT][BOATOYR][D",
		"ECEMBER PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for an owner of a piece of bo",
		"at management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([BOAT MANAGEMENT OWNER YEAR PAID AMOUNT TOTAL]+[B",
		"OAT MANAGEMENT OWNER YEAR UNPROCESSED PAID AMOUNT ",
		"TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for an owner of a piece of boat managem",
		"ent.  It includes both processed and unprocessed p",
		"ayments.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY PENALTY AMOUNT]+[BOATM",
		"GT][BOATOYR][FEBRUARY PENALTY AMOUNT]+[BOATMGT][BO",
		"ATOYR][MARCH PENALTY AMOUNT]+[BOATMGT][BOATOYR][AP",
		"RIL PENALTY AMOUNT]+[BOATMGT][BOATOYR][MAY PENALTY",
		" AMOUNT]+[BOATMGT][BOATOYR][JUNE PENALTY AMOUNT]+[",
		"BOATMGT][BOATOYR][JULY PENALTY AMOUNT]+[BOATMGT][B",
		"OATOYR][AUGUST PENALTY AMOUNT]+[BOATMGT][BOATOYR][",
		"SEPTEMBER PENALTY AMOUNT]+[BOATMGT][BOATOYR][OCTOB",
		"ER PENALTY AMOUNT]+[BOATMGT][BOATOYR][NOVEMBER PEN",
		"ALTY AMOUNT]+[BOATMGT][BOATOYR][DECEMBER PENALTY A",
		"MOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for an owner of a piece of",
		" boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([BOAT MANAGEMENT OWNER YEAR PENALTY AMOUNT TOTAL]",
		"+[BOAT MANAGEMENT OWNER YEAR UNPROCESSED PENALTY A",
		"MOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for an owner of a piece of boat mana",
		"gement.  It includes both processed and unprocesse",
		"d penalties.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([BOAT MANAGEMENT OWNER YEAR TAX AMOUNT TOTAL]+[BO",
		"AT MANAGEMENT OWNER YEAR PENALTY AMOUNT TOTAL]+[BO",
		"AT MANAGEMENT OWNER YEAR INTEREST AMOUNT TOTAL]-[B",
		"OAT MANAGEMENT OWNER YEAR PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" an owner of a piece of boat management in a given",
		" year.  This includes only processed amounts.  Rem",
		"ember, payments may have been collected in another",
		" year.  This is only the annual balance, not the b",
		"alance owed on the Boat Management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY TAX AMOUNT]+[BOATMGT][",
		"BOATOYR][FEBRUARY TAX AMOUNT]+[BOATMGT][BOATOYR][M",
		"ARCH TAX AMOUNT]+[BOATMGT][BOATOYR][APRIL TAX AMOU",
		"NT]+[BOATMGT][BOATOYR][MAY TAX AMOUNT]+[BOATMGT][B",
		"OATOYR][JUNE TAX AMOUNT]+[BOATMGT][BOATOYR][JULY T",
		"AX AMOUNT]+[BOATMGT][BOATOYR][AUGUST TAX AMOUNT]+[",
		"BOATMGT][BOATOYR][SEPTEMBER TAX AMOUNT]+[BOATMGT][",
		"BOATOYR][OCTOBER TAX AMOUNT]+[BOATMGT][BOATOYR][NO",
		"VEMBER TAX AMOUNT]+[BOATMGT][BOATOYR][DECEMBER TAX",
		" AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for an owner of a piece of boa",
		"t management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([BOAT MANAGEMENT OWNER YEAR TAX AMOUNT TOTAL]+[BO",
		"AT MANAGEMENT OWNER YEAR UNPROCESSED TAX AMOUNT TO",
		"TAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for an owner of a piece of boat manage",
		"ment.  It includes processed and unprocessed taxes",
		".");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY UNPROCESSED INTEREST A",
		"MOUNT]+[BOATMGT][BOATOYR][FEBRUARY UNPROCESSED INT",
		"EREST AMOUNT]+[BOATMGT][BOATOYR][MARCH UNPROCESSED",
		" INTEREST AMOUNT]+[BOATMGT][BOATOYR][APRIL UNPROCE",
		"SSED INTEREST AMOUNT]+[BOATMGT][BOATOYR][MAY UNPRO",
		"CESSED INTEREST AMOUNT]+[BOATMGT][BOATOYR][JUNE UN",
		"PROCESSED INTEREST AMOUNT]+[BOATMGT][BOATOYR][JULY",
		" UNPROCESSED INTEREST AMOUNT]+[BOATMGT][BOATOYR][A",
		"UGUST UNPROCESSED INTEREST AMOUNT]+[BOATMGT][BOATO",
		"YR][SEPTEMBER UNPROCESSED INTEREST AMOUNT]+[BOATMG",
		"T][BOATOYR][OCTOBER UNPROCESSED INTEREST AMOUNT]+[",
		"BOATMGT][BOATOYR][NOVEMBER UNPROCESSED INTEREST AM",
		"OUNT]+[BOATMGT][BOATOYR][DECEMBER UNPROCESSED INTE",
		"REST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for an owner of a piece",
		" of boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY UNPROCESSED PAID AMOUN",
		"T]+[BOATMGT][BOATYEAR][FEBRUARY UNPROCESSED PAID A",
		"MOUNT]+[BOATMGT][BOATOYR][MARCH UNPROCESSED PAID A",
		"MOUNT]+[BOATMGT][BOATOYR][APRIL UNPROCESSED PAID A",
		"MOUNT]+[BOATMGT][BOATOYR][MAY UNPROCESSED PAID AMO",
		"UNT]+[BOATMGT][BOATOYR][JUNE UNPROCESSED PAID AMOU",
		"NT]+[BOATMGT][BOATOYR][JULY UNPROCESSED PAID AMOUN",
		"T]+[BOATMGT][BOATOYR][AUGUST UNPROCESSED PAID AMOU",
		"NT]+[BOATMGT][BOATOYR][SEPTEMBER UNPROCESSED PAID ",
		"AMOUNT]+[BOATMGT][BOATOYR][OCTOBER UNPROCESSED PAI",
		"D AMOUNT]+[BOATMGT][BOATOYR][NOVEMBER UNPROCESSED ",
		"PAID AMOUNT]+[BOATMGT][BOATOYR][DECEMBER UNPROCESS",
		"ED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for an owner of a piece of ",
		"boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY UNPROCESSED PENALTY AM",
		"OUNT]+[BOATMGT][BOATOYR][FEBRUARY UNPROCESSED PENA",
		"LTY AMOUNT]+[BOATMGT][BOATOYR][MARCH UNPROCESSED P",
		"ENALTY AMOUNT]+[BOATMGT][BOATOYR][APRIL UNPROCESSE",
		"D PENALTY AMOUNT]+[BOATMGT][BOATOYR][MAY UNPROCESS",
		"ED PENALTY AMOUNT]+[BOATMGT][BOATOYR][JUNE UNPROCE",
		"SSED PENALTY AMOUNT]+[BOATMGT][BOATOYR][JULY UNPRO",
		"CESSED PENALTY AMOUNT]+[BOATMGT][BOATOYR][AUGUST U",
		"NPROCESSED PENALTY AMOUNT]+[BOATMGT][BOATOYR][SEPT",
		"EMBER UNPROCESSED PENALTY AMOUNT]+[BOATMGT][BOATOY",
		"R][OCTOBER UNPROCESSED PENALTY AMOUNT]+[BOATMGT][B",
		"OATOYR][NOVEMBER UNPROCESSED PENALTY AMOUNT]+[BOAT",
		"MGT][BOATOYR][DECEMBER UNPROCESSED PENALTY AMOUNT]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for an owner of a piece ",
		"of boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT OWNER YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATOYR][JANUARY UNPROCESSED TAX AMOUNT",
		"]+[BOATMGT][BOATOYR][FEBRUARY UNPROCESSED TAX AMOU",
		"NT]+[BOATMGT][BOATOYR][MARCH UNPROCESSED TAX AMOUN",
		"T]+[BOATMGT][BOATOYR][APRIL UNPROCESSED TAX AMOUNT",
		"]+[BOATMGT][BOATOYR][MAY UNPROCESSED TAX AMOUNT]+[",
		"BOATMGT][BOATOYR][JUNE UNPROCESSED TAX AMOUNT]+[BO",
		"ATMGT][BOATOYR][JULY UNPROCESSED TAX AMOUNT]+[BOAT",
		"MGT][BOATOYR][AUGUST UNPROCESSED TAX AMOUNT]+[BOAT",
		"MGT][BOATOYR][SEPTEMBER UNPROCESSED TAX AMOUNT]+[B",
		"OATMGT][BOATOYR][OCTOBER UNPROCESSED TAX AMOUNT]+[",
		"BOATMGT][BOATOYR][NOVEMBER UNPROCESSED TAX AMOUNT]",
		"+[BOATMGT][BOATOYR][DECEMBER UNPROCESSED TAX AMOUN",
		"T])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for an owner of a piece",
		" of boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([BOATMGT][BOATMST][PENALTY AGED BALANCE #1]+[BOAT",
		"MGT][BOATMST][PENALTY AGED BALANCE #2]+[BOATMGT][B",
		"OATMST][PENALTY AGED BALANCE #3]+[BOATMGT][BOATMST",
		"][PENALTY AGED BALANCE #4]+[BOATMGT][BOATMST][UNPR",
		"OCESSED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for a piece of boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([BOATMGT][BOATMST][TAX AGED BALANCE #1]+[BOATMGT]",
		"[BOATMST][TAX AGED BALANCE #2]+[BOATMGT][BOATMST][",
		"TAX AGED BALANCE #3]+[BOATMGT][BOATMST][TAX AGED B",
		"ALANCE #4]+[BOATMGT][BOATMST][UNPROCESSED TAX AMOU",
		"NT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes on a piece of boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([BOAT MANAGEMENT TAX AMOUNT TOTAL]+[BOAT MANAGEME",
		"NT PENALTY AMOUNT TOTAL]+[BOAT MANAGEMENT INTEREST",
		" AMOUNT TOTAL]-[BOATMGT][BOATMST][UNPROCESSED PAID",
		" AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed on a piece of boat management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT TOTAL VALUE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([BOATMGT][BOATMST][BOAT VALUE]+[BOATMGT][BOATMST]",
		"[IMPROVEMENT VALUE])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to define the total val",
		"ue of a piece of property.  The total value and ma",
		"rket value are the same.  This information is dete",
		"rmined for the Boat Management Master File.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([BOAT MANAGEMENT YEAR TAX TOTAL]+[BOAT MANAGEMENT",
		" YEAR PENALTY TOTAL]+[BOAT MANAGEMENT YEAR INTERES",
		"T TOTAL]-[BOAT MANAGEMENT YEAR PAID TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the balance ",
		"in a given year for a piece of boat management.  I",
		"t includes both processed and unprocessed amounts.",
		"  This balance doesn't reflect the amount owed on ",
		"a piece of boat management, since the payments mig",
		"ht have been collected in another year.  This mere",
		"ly show's the balance of a particular year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY INTEREST AMOUNT]+[BOA",
		"TMGT][BOATYEAR][FEBRUARY INTEREST AMOUNT]+[BOATMGT",
		"][BOATYEAR][MARCH INTEREST AMOUNT]+[BOATMGT][BOATY",
		"EAR][APRIL INTEREST AMOUNT]+[BOATMGT][BOATYEAR][MA",
		"Y INTEREST AMOUNT]+[BOATMGT][BOATYEAR][JUNE INTERE",
		"ST AMOUNT]+[BOATMGT][BOATYEAR][JULY INTEREST AMOUN",
		"T]+[BOATMGT][BOATYEAR][AUGUST INTEREST AMOUNT]+[BO",
		"ATMGT][BOATYEAR][SEPTEMBER INTEREST AMOUNT]+[BOATM",
		"GT][BOATYEAR][OCTOBER INTEREST AMOUNT]+[BOATMGT][B",
		"OATYEAR][NOVEMBER INTEREST AMOUNT]+[BOATMGT][BOATY",
		"EAR][DECEMBER INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for a piece of boat manag",
		"ement.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([BOAT MANAGEMENT YEAR INTEREST AMOUNT TOTAL]+[BOA",
		"T MANAGEMENT YEAR UNPROCESSED INTEREST AMOUNT TOTA",
		"L])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for a piece of boat management.  It",
		" includes processed and unprocessed interest.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY PAID AMOUNT]+[BOATMGT",
		"][BOATYEAR][FEBRUARY PAID AMOUNT]+[BOATMGT][BOATYE",
		"AR][MARCH PAID AMOUNT]+[BOATMGT][BOATYEAR][APRIL P",
		"AID AMOUNT]+[BOATMGT][BOATYEAR][MAY PAID AMOUNT]+[",
		"BOATMGT][BOATYEAR][JUNE PAID AMOUNT]+[BOATMGT][BOA",
		"TYEAR][JULY PAID AMOUNT]+[BOATMGT][BOATYEAR][AUGUS",
		"T PAID AMOUNT]+[BOATMGT][BOATYEAR][SEPTEMBER PAID ",
		"AMOUNT]+[BOATMGT][BOATYEAR][OCTOBER PAID AMOUNT]+[",
		"BOATMGT][BOATYEAR][NOVEMBER PAID AMOUNT]+[BOATMGT]",
		"[BOATYEAR][DECEMBER PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for a piece of boat managemen",
		"t.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([BOAT MANAGEMENT YEAR PAID AMOUNT TOTAL]+[BOAT MA",
		"NAGEMENT YEAR UNPROCESSED PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for a piece of boat management.  It inc",
		"ludes both processed and unprocessed payments.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY PENALTY AMOUNT]+[BOAT",
		"MGT][BOATYEAR][FEBRUARY PENALTY AMOUNT]+[BOATMGT][",
		"BOATYEAR][MARCH PENALTY AMOUNT]+[BOATMGT][BOATYEAR",
		"][APRIL PENALTY AMOUNT]+[BOATMGT][BOATYEAR][MAY PE",
		"NALTY AMOUNT]+[BOATMGT][BOATYEAR][JUNE PENALTY AMO",
		"UNT]+[BOATMGT][BOATYEAR][JULY PENALTY AMOUNT]+[BOA",
		"TMGT][BOATYEAR][AUGUST PENALTY AMOUNT]+[BOATMGT][B",
		"OATYEAR][SEPTEMBER PENALTY AMOUNT]+[BOATMGT][BOATY",
		"EAR][OCTOBER PENALTY AMOUNT]+[BOATMGT][BOATYEAR][N",
		"OVEMBER PENALTY AMOUNT]+[BOATMGT][BOATYEAR][DECEMB",
		"ER PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for a piece of boat manage",
		"ment.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([BOAT MANAGEMENT YEAR PENALTY AMOUNT TOTAL]+[BOAT",
		" MANAGEMENT YEAR UNPROCESSED PENALTY AMOUNT TOTAL]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for a piece of boat management.  It ",
		"includes both processed and unprocessed penalties.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([BOAT MANAGEMENT YEAR TAX AMOUNT TOTAL]+[BOAT MAN",
		"AGEMENT YEAR PENALTY AMOUNT TOTAL]+[BOAT MANAGEMEN",
		"T YEAR INTEREST AMOUNT TOTAL]-[BOAT MANAGEMENT YEA",
		"R PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" a piece of boat management in a given year.  This",
		" includes only processed amounts.  Remember, payme",
		"nts may have been collected in another year.  This",
		" is only the annual balance, not the balance owed ",
		"on the Boat Management.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY TAX AMOUNT]+[BOATMGT]",
		"[BOATYEAR][FEBRUARY TAX AMOUNT]+[BOATMGT][BOATYEAR",
		"][MARCH TAX AMOUNT]+[BOATMGT][BOATYEAR][APRIL TAX ",
		"AMOUNT]+[BOATMGT][BOATYEAR][MAY TAX AMOUNT]+[BOATM",
		"GT][BOATYEAR][JUNE TAX AMOUNT]+[BOATMGT][BOATYEAR]",
		"[JULY TAX AMOUNT]+[BOATMGT][BOATYEAR][AUGUST TAX A",
		"MOUNT]+[BOATMGT][BOATYEAR][SEPTEMBER TAX AMOUNT]+[",
		"BOATMGT][BOATYEAR][OCTOBER TAX AMOUNT]+[BOATMGT][B",
		"OATYEAR][NOVEMBER TAX AMOUNT]+[BOATMGT][BOATYEAR][",
		"DECEMBER TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for a piece of boat management",
		".");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([BOAT MANAGEMENT YEAR TAX AMOUNT TOTAL]+[BOAT MAN",
		"AGEMENT YEAR UNPROCESSED TAX AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for a piece of boat management.  It in",
		"cludes processed and unprocessed taxes.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY UNPROCESSED INTEREST ",
		"AMOUNT]+[BOATMGT][BOATYEAR][FEBRUARY UNPROCESSED I",
		"NTEREST AMOUNT]+[BOATMGT][BOATYEAR][MARCH UNPROCES",
		"SED INTEREST AMOUNT]+[BOATMGT][BOATYEAR][APRIL UNP",
		"ROCESSED INTEREST AMOUNT]+[BOATMGT][BOATYEAR][MAY ",
		"UNPROCESSED INTEREST AMOUNT]+[BOATMGT][BOATYEAR][J",
		"UNE UNPROCESSED INTEREST AMOUNT]+[BOATMGT][BOATYEA",
		"R][JULY UNPROCESSED INTEREST AMOUNT]+[BOATMGT][BOA",
		"TYEAR][AUGUST UNPROCESSED INTEREST AMOUNT]+[BOATMG",
		"T][BOATYEAR][SEPTEMBER UNPROCESSED INTEREST AMOUNT",
		"]+[BOATMGT][BOATYEAR][OCTOBER UNPROCESSED INTEREST",
		" AMOUNT]+[BOATMGT][BOATYEAR][NOVEMBER UNPROCESSED ",
		"INTEREST AMOUNT]+[BOATMGT][BOATYEAR][DECEMBER UNPR",
		"OCESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for a piece of boat man",
		"agement.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY UNPROCESSED PAID AMOU",
		"NT]+[BOATMGT][BOATYEAR][FEBRUARY UNPROCESSED PAID ",
		"AMOUNT]+[BOATMGT][BOATYEAR][MARCH UNPROCESSED PAID",
		" AMOUNT]+[BOATMGT][BOATYEAR][APRIL UNPROCESSED PAI",
		"D AMOUNT]+[BOATMGT][BOATYEAR][MAY UNPROCESSED PAID",
		" AMOUNT]+[BOATMGT][BOATYEAR][JUNE UNPROCESSED PAID",
		" AMOUNT]+[BOATMGT][BOATYEAR][JULY UNPROCESSED PAID",
		" AMOUNT]+[BOATMGT][BOATYEAR][AUGUST UNPROCESSED PA",
		"ID AMOUNT]+[BOATMGT][BOATYEAR][SEPTEMBER UNPROCESS",
		"ED PAID AMOUNT]+[BOATMGT][BOATYEAR][OCTOBER UNPROC",
		"ESSED PAID AMOUNT]+[BOATMGT][BOATYEAR][NOVEMBER UN",
		"PROCESSED PAID AMOUNT]+[BOATMGT][BOATYEAR][DECEMBE",
		"R UNPROCESSED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for a piece of boat managem",
		"ent.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY UNPROCESSED PENALTY A",
		"MOUNT]+[BOATMGT][BOATYEAR][FEBRUARY UNPROCESSED PE",
		"NALTY AMOUNT]+[BOATMGT][BOATYEAR][MARCH UNPROCESSE",
		"D PENALTY AMOUNT]+[BOATMGT][BOATYEAR][APRIL UNPROC",
		"ESSED PENALTY AMOUNT]+[BOATMGT][BOATYEAR][MAY UNPR",
		"OCESSED PENALTY AMOUNT]+[BOATMGT][BOATYEAR][JUNE U",
		"NPROCESSED PENALTY AMOUNT]+[BOATMGT][BOATYEAR][JUL",
		"Y UNPROCESSED PENALTY AMOUNT]+[BOATMGT][BOATYEAR][",
		"AUGUST UNPROCESSED PENALTY AMOUNT]+[BOATMGT][BOATY",
		"EAR][SEPTEMBER UNPROCESSED PENALTY AMOUNT]+[BOATMG",
		"T][BOATYEAR][OCTOBER UNPROCESSED PENALTY AMOUNT]+[",
		"BOATMGT][BOATYEAR][NOVEMBER UNPROCESSED PENALTY AM",
		"OUNT]+[BOATMGT][BOATYEAR][DECEMBER UNPROCESSED PEN",
		"ALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for a piece of boat mana",
		"gement.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","BOAT MANAGEMENT YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([BOATMGT][BOATYEAR][JANUARY UNPROCESSED TAX AMOUN",
		"T]+[BOATMGT][BOATYEAR][FEBRUARY UNPROCESSED TAX AM",
		"OUNT]+[BOATMGT][BOATYEAR][MARCH UNPROCESSED TAX AM",
		"OUNT]+[BOATMGT][BOATYEAR][APRIL UNPROCESSED TAX AM",
		"OUNT]+[BOATMGT][BOATYEAR][MAY UNPROCESSED TAX AMOU",
		"NT]+[BOATMGT][BOATYEAR][JUNE UNPROCESSED TAX AMOUN",
		"T]+[BOATMGT][BOATYEAR][JULY UNPROCESSED TAX AMOUNT",
		"]+[BOATMGT][BOATYEAR][AUGUST UNPROCESSED TAX AMOUN",
		"T]+[BOATMGT][BOATYEAR][SEPTEMBER UNPROCESSED TAX A",
		"MOUNT]+[BOATMGT][BOATYEAR][OCTOBER UNPROCESSED TAX",
		" AMOUNT]+[BOATMGT][BOATYEAR][NOVEMBER UNPROCESSED ",
		"TAX AMOUNT]+[BOATMGT][BOATYEAR][DECEMBER UNPROCESS",
		"ED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for a piece of boat man",
		"agement.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","OWNER NAME");
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
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("BOATMGT","TAXABLE VALUE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATMST");
	i->keyname=NULL;
	i->module=stralloc("BOATMGT");
	i->file=stralloc("BOATYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=2;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"IF([BOATMGT][BOATYEAR][BOAT VALUE]!=0.00) THEN ",
		"RETURN_VALUE=([BOATMGT][BOATYEAR][BOAT VALUE]+[B",
		"OATMGT][BOATYEAR][IMPROVEMENT VALUE]-[BOATMGT][BOA",
		"TYEAR][TAX DEFERRED AMOUNT]) ELSE RETURN_VALUE=([",
		"BOATMGT][BOATMST][BOAT VALUE]+[BOATMGT][BOATMST]",
		"[IMPROVEMENT VALUE]-[BOATMGT][BOATMST][TAX DEFERRE",
		"D AMOUNT]) FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"This virtual field is a calculation of the Taxable",
		" Value of Boat Management Property from the formul",
		"a (Market Value+Building & Improvements-Defered Am",
		"ount) and any other factors that are included in t",
		"he expression.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("BOATMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

}
