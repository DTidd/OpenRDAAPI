/* pptvirt.c - Makes PPTMGT Virtual Fields */
#include <cstdio>





#include <rptgen.hpp>
#include <mkvir.hpp>

void PPTMGTCreate()
{
	DFvirtual *d=NULL;
	DFincvir *i;
	char temp[15000];
	int field_length=0;

	d=DFvirtualNEW("PPTMGT","OWNER NAME");
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
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([PPTMGT][PPTMST][INTEREST AGED BALANCE #1]+[PPTMG",
		"T][PPTMST][INTEREST AGED BALANCE #2]+[PPTMGT][PPTM",
		"ST][INTEREST AGED BALANCE #3]+[PPTMGT][PPTMST][INT",
		"EREST AGED BALANCE #4]+[PPTMGT][PPTMST][UNPROCESSE",
		"D INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the interest",
		" amount for a piece of personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY NAME");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("PPTMGT","PPTMST","NAME");
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
		"[PPTMGT][PPTMST][NAME]");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field reflects the [PPTMGT][PPTMST][N",
		"AME].  It is used to place that data in other main",
		"tain master's BROWSE LIST's.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([PPTMGT][PPTOWN][INTEREST AGED BALANCE #1]+[PPTMG",
		"T][PPTOWN][INTEREST AGED BALANCE #2]+[PPTMGT][PPTO",
		"WN][INTEREST AGED BALANCE #3]+[PPTMGT][PPTOWN][INT",
		"EREST AGED BALANCE #4]+[PPTMGT][PPTOWN][UNPROCESSE",
		"D INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the interest",
		" amount for an owner of a piece of personal proper",
		"ty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([PPTMGT][PPTOWN][PENALTY AGED BALANCE #1]+[PPTMGT",
		"][PPTOWN][PENALTY AGED BALANCE #2]+[PPTMGT][PPTOWN",
		"][PENALTY AGED BALANCE #3]+[PPTMGT][PPTOWN][PENALT",
		"Y AGED BALANCE #4]+[PPTMGT][PPTOWN][UNPROCESSED PE",
		"NALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for an owner of a piece of personal p",
		"roperty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([PPTMGT][PPTOWN][TAX AGED BALANCE #1]+[PPTMGT][PP",
		"TOWN][TAX AGED BALANCE #2]+[PPTMGT][PPTOWN][TAX AG",
		"ED BALANCE #3]+[PPTMGT][PPTOWN][TAX AGED BALANCE #",
		"4]+[PPTMGT][PPTOWN][UNPROCESSED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes for an owner of a piece of personal ",
		"property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOWN");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([PERSONAL PROPERTY OWNER TAX AMOUNT TOTAL]+[PERSO",
		"NAL PROPERTY OWNER PENALTY AMOUNT TOTAL]+[PERSONAL",
		" PROPERTY OWNER INTEREST AMOUNT TOTAL]-[PPTMGT][PP",
		"TOWN][UNPROCESSED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed by an owner on a piece of personal prope",
		"rty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([PERSONAL PROPERTY OWNER YEAR TAX TOTAL]+[PERSONA",
		"L PROPERTY OWNER YEAR PENALTY TOTAL]+[PERSONAL PRO",
		"PERTY OWNER YEAR INTEREST TOTAL]-[PERSONAL PROPERT",
		"Y OWNER YEAR PAID TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the owner's ",
		"balance in a given year for a piece of personal pr",
		"operty.  It includes both processed and unprocesse",
		"d amounts.  This balance doesn't reflect the amoun",
		"t owed on a piece of personal property, since the ",
		"payments might have been collected in another year",
		".  This merely show's the balance of a particular ",
		"year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY INTEREST AMOUNT]+[PPTMGT",
		"][PPTOYR][FEBRUARY INTEREST AMOUNT]+[PPTMGT][PPTOY",
		"R][MARCH INTEREST AMOUNT]+[PPTMGT][PPTOYR][APRIL I",
		"NTEREST AMOUNT]+[PPTMGT][PPTOYR][MAY INTEREST AMOU",
		"NT]+[PPTMGT][PPTOYR][JUNE INTEREST AMOUNT]+[PPTMGT",
		"][PPTOYR][JULY INTEREST AMOUNT]+[PPTMGT][PPTOYR][A",
		"UGUST INTEREST AMOUNT]+[PPTMGT][PPTOYR][SEPTEMBER ",
		"INTEREST AMOUNT]+[PPTMGT][PPTOYR][OCTOBER INTEREST",
		" AMOUNT]+[PPTMGT][PPTOYR][NOVEMBER INTEREST AMOUNT",
		"]+[PPTMGT][PPTOYR][DECEMBER INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for an owner of a piece o",
		"f personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([PERSONAL PROPERTY OWNER YEAR INTEREST AMOUNT TOT",
		"AL]+[PERSONAL PROPERTY OWNER YEAR UNPROCESSED INTE",
		"REST AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for an owner of a piece of personal",
		" property.  It includes processed and unprocessed ",
		"interest.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY PAID AMOUNT]+[PPTMGT][PP",
		"TOYR][FEBRUARY PAID AMOUNT]+[PPTMGT][PPTOYR][MARCH",
		" PAID AMOUNT]+[PPTMGT][PPTOYR][APRIL PAID AMOUNT]+",
		"[PPTMGT][PPTOYR][MAY PAID AMOUNT]+[PPTMGT][PPTOYR]",
		"[JUNE PAID AMOUNT]+[PPTMGT][PPTOYR][JULY PAID AMOU",
		"NT]+[PPTMGT][PPTOYR][AUGUST PAID AMOUNT]+[PPTMGT][",
		"PPTOYR][SEPTEMBER PAID AMOUNT]+[PPTMGT][PPTOYR][OC",
		"TOBER PAID AMOUNT]+[PPTMGT][PPTOYR][NOVEMBER PAID ",
		"AMOUNT]+[PPTMGT][PPTOYR][DECEMBER PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for an owner of a piece of pe",
		"rsonal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([PERSONAL PROPERTY OWNER YEAR PAID AMOUNT TOTAL]+",
		"[PERSONAL PROPERTY OWNER YEAR UNPROCESSED PAID AMO",
		"UNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for an owner of a piece of personal pro",
		"perty.  It includes both processed and unprocessed",
		" payments.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY PENALTY AMOUNT]+[PPTMGT]",
		"[PPTOYR][FEBRUARY PENALTY AMOUNT]+[PPTMGT][PPTOYR]",
		"[MARCH PENALTY AMOUNT]+[PPTMGT][PPTOYR][APRIL PENA",
		"LTY AMOUNT]+[PPTMGT][PPTOYR][MAY PENALTY AMOUNT]+[",
		"PPTMGT][PPTOYR][JUNE PENALTY AMOUNT]+[PPTMGT][PPTO",
		"YR][JULY PENALTY AMOUNT]+[PPTMGT][PPTOYR][AUGUST P",
		"ENALTY AMOUNT]+[PPTMGT][PPTOYR][SEPTEMBER PENALTY ",
		"AMOUNT]+[PPTMGT][PPTOYR][OCTOBER PENALTY AMOUNT]+[",
		"PPTMGT][PPTOYR][NOVEMBER PENALTY AMOUNT]+[PPTMGT][",
		"PPTOYR][DECEMBER PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for an owner of a piece of",
		" personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([PERSONAL PROPERTY OWNER YEAR PENALTY AMOUNT TOTA",
		"L]+[PERSONAL PROPERTY OWNER YEAR UNPROCESSED PENAL",
		"TY AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for an owner of a piece of personal ",
		"property.  It includes both processed and unproces",
		"sed penalties.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([PERSONAL PROPERTY OWNER YEAR TAX AMOUNT TOTAL]+[",
		"PERSONAL PROPERTY OWNER YEAR PENALTY AMOUNT TOTAL]",
		"+[PERSONAL PROPERTY OWNER YEAR INTEREST AMOUNT TOT",
		"AL]-[PERSONAL PROPERTY OWNER YEAR PAID AMOUNT TOTA",
		"L])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" an owner of a piece of personal property in a giv",
		"en year.  This includes only processed amounts.  R",
		"emember, payments may have been collected in anoth",
		"er year.  This is only the annual balance, not the",
		" balance owed on the Personal Property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY TAX AMOUNT]+[PPTMGT][PPT",
		"OYR][FEBRUARY TAX AMOUNT]+[PPTMGT][PPTOYR][MARCH T",
		"AX AMOUNT]+[PPTMGT][PPTOYR][APRIL TAX AMOUNT]+[PPT",
		"MGT][PPTOYR][MAY TAX AMOUNT]+[PPTMGT][PPTOYR][JUNE",
		" TAX AMOUNT]+[PPTMGT][PPTOYR][JULY TAX AMOUNT]+[PP",
		"TMGT][PPTOYR][AUGUST TAX AMOUNT]+[PPTMGT][PPTOYR][",
		"SEPTEMBER TAX AMOUNT]+[PPTMGT][PPTOYR][OCTOBER TAX",
		" AMOUNT]+[PPTMGT][PPTOYR][NOVEMBER TAX AMOUNT]+[PP",
		"TMGT][PPTOYR][DECEMBER TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for an owner of a piece of per",
		"sonal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([PERSONAL PROPERTY OWNER YEAR TAX AMOUNT TOTAL]+[",
		"PERSONAL PROPERTY OWNER YEAR UNPROCESSED TAX AMOUN",
		"T TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for an owner of a piece of personal pr",
		"operty.  It includes processed and unprocessed tax",
		"es.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY UNPROCESSED INTEREST AMO",
		"UNT]+[PPTMGT][PPTOYR][FEBRUARY UNPROCESSED INTERES",
		"T AMOUNT]+[PPTMGT][PPTOYR][MARCH UNPROCESSED INTER",
		"EST AMOUNT]+[PPTMGT][PPTOYR][APRIL UNPROCESSED INT",
		"EREST AMOUNT]+[PPTMGT][PPTOYR][MAY UNPROCESSED INT",
		"EREST AMOUNT]+[PPTMGT][PPTOYR][JUNE UNPROCESSED IN",
		"TEREST AMOUNT]+[PPTMGT][PPTOYR][JULY UNPROCESSED I",
		"NTEREST AMOUNT]+[PPTMGT][PPTOYR][AUGUST UNPROCESSE",
		"D INTEREST AMOUNT]+[PPTMGT][PPTOYR][SEPTEMBER UNPR",
		"OCESSED INTEREST AMOUNT]+[PPTMGT][PPTOYR][OCTOBER ",
		"UNPROCESSED INTEREST AMOUNT]+[PPTMGT][PPTOYR][NOVE",
		"MBER UNPROCESSED INTEREST AMOUNT]+[PPTMGT][PPTOYR]",
		"[DECEMBER UNPROCESSED INTEREST AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for an owner of a piece",
		" of personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY UNPROCESSED PAID AMOUNT]",
		"+[PPTMGT][PPTYEAR][FEBRUARY UNPROCESSED PAID AMOUN",
		"T]+[PPTMGT][PPTOYR][MARCH UNPROCESSED PAID AMOUNT]",
		"+[PPTMGT][PPTOYR][APRIL UNPROCESSED PAID AMOUNT]+[",
		"PPTMGT][PPTOYR][MAY UNPROCESSED PAID AMOUNT]+[PPTM",
		"GT][PPTOYR][JUNE UNPROCESSED PAID AMOUNT]+[PPTMGT]",
		"[PPTOYR][JULY UNPROCESSED PAID AMOUNT]+[PPTMGT][PP",
		"TOYR][AUGUST UNPROCESSED PAID AMOUNT]+[PPTMGT][PPT",
		"OYR][SEPTEMBER UNPROCESSED PAID AMOUNT]+[PPTMGT][P",
		"PTOYR][OCTOBER UNPROCESSED PAID AMOUNT]+[PPTMGT][P",
		"PTOYR][NOVEMBER UNPROCESSED PAID AMOUNT]+[PPTMGT][",
		"PPTOYR][DECEMBER UNPROCESSED PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for an owner of a piece of ",
		"personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY UNPROCESSED PENALTY AMOU",
		"NT]+[PPTMGT][PPTOYR][FEBRUARY UNPROCESSED PENALTY ",
		"AMOUNT]+[PPTMGT][PPTOYR][MARCH UNPROCESSED PENALTY",
		" AMOUNT]+[PPTMGT][PPTOYR][APRIL UNPROCESSED PENALT",
		"Y AMOUNT]+[PPTMGT][PPTOYR][MAY UNPROCESSED PENALTY",
		" AMOUNT]+[PPTMGT][PPTOYR][JUNE UNPROCESSED PENALTY",
		" AMOUNT]+[PPTMGT][PPTOYR][JULY UNPROCESSED PENALTY",
		" AMOUNT]+[PPTMGT][PPTOYR][AUGUST UNPROCESSED PENAL",
		"TY AMOUNT]+[PPTMGT][PPTOYR][SEPTEMBER UNPROCESSED ",
		"PENALTY AMOUNT]+[PPTMGT][PPTOYR][OCTOBER UNPROCESS",
		"ED PENALTY AMOUNT]+[PPTMGT][PPTOYR][NOVEMBER UNPRO",
		"CESSED PENALTY AMOUNT]+[PPTMGT][PPTOYR][DECEMBER U",
		"NPROCESSED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for an owner of a piece ",
		"of personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY OWNER YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTOYR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTOYR][JANUARY UNPROCESSED TAX AMOUNT]+",
		"[PPTMGT][PPTOYR][FEBRUARY UNPROCESSED TAX AMOUNT]+",
		"[PPTMGT][PPTOYR][MARCH UNPROCESSED TAX AMOUNT]+[PP",
		"TMGT][PPTOYR][APRIL UNPROCESSED TAX AMOUNT]+[PPTMG",
		"T][PPTOYR][MAY UNPROCESSED TAX AMOUNT]+[PPTMGT][PP",
		"TOYR][JUNE UNPROCESSED TAX AMOUNT]+[PPTMGT][PPTOYR",
		"][JULY UNPROCESSED TAX AMOUNT]+[PPTMGT][PPTOYR][AU",
		"GUST UNPROCESSED TAX AMOUNT]+[PPTMGT][PPTOYR][SEPT",
		"EMBER UNPROCESSED TAX AMOUNT]+[PPTMGT][PPTOYR][OCT",
		"OBER UNPROCESSED TAX AMOUNT]+[PPTMGT][PPTOYR][NOVE",
		"MBER UNPROCESSED TAX AMOUNT]+[PPTMGT][PPTOYR][DECE",
		"MBER UNPROCESSED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for an owner of a piece",
		" of personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"([PPTMGT][PPTMST][PENALTY AGED BALANCE #1]+[PPTMGT",
		"][PPTMST][PENALTY AGED BALANCE #2]+[PPTMGT][PPTMST",
		"][PENALTY AGED BALANCE #3]+[PPTMGT][PPTMST][PENALT",
		"Y AGED BALANCE #4]+[PPTMGT][PPTMST][UNPROCESSED PE",
		"NALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total pe",
		"nalty amount for a piece of personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([PPTMGT][PPTMST][TAX AGED BALANCE #1]+[PPTMGT][PP",
		"TMST][TAX AGED BALANCE #2]+[PPTMGT][PPTMST][TAX AG",
		"ED BALANCE #3]+[PPTMGT][PPTMST][TAX AGED BALANCE #",
		"4]+[PPTMGT][PPTMST][UNPROCESSED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount of taxes on a piece of personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY TOTAL AMOUNT");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=15;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([PERSONAL PROPERTY TAX AMOUNT TOTAL]+[PERSONAL PR",
		"OPERTY PENALTY AMOUNT TOTAL]+[PERSONAL PROPERTY IN",
		"TEREST AMOUNT TOTAL]-[PPTMGT][PPTMST][UNPROCESSED ",
		"PAID AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"This virtual field is used to reflect the total am",
		"ount owed on a piece of personal property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY TOTAL VALUE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTMST");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([PPTMGT][PPTMST][PROPERTY VALUE]+[PPTMGT][PPTMST]",
		"[IMPROVEMENT VALUE])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to define the total val",
		"ue of a piece of property.  The total value and ma",
		"rket value are the same.  This information is dete",
		"rmined for the Personal Property Master File.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([PERSONAL PROPERTY YEAR TAX TOTAL]+[PERSONAL PROP",
		"ERTY YEAR PENALTY TOTAL]+[PERSONAL PROPERTY YEAR I",
		"NTEREST TOTAL]-[PERSONAL PROPERTY YEAR PAID TOTAL]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"This virtual field is used to reflect the balance ",
		"in a given year for a piece of personal property. ",
		" It includes both processed and unprocessed amount",
		"s.  This balance doesn't reflect the amount owed o",
		"n a piece of personal property, since the payments",
		" might have been collected in another year.  This ",
		"merely show's the balance of a particular year.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY INTEREST AMOUNT]+[PPTMG",
		"T][PPTYEAR][FEBRUARY INTEREST AMOUNT]+[PPTMGT][PPT",
		"YEAR][MARCH INTEREST AMOUNT]+[PPTMGT][PPTYEAR][APR",
		"IL INTEREST AMOUNT]+[PPTMGT][PPTYEAR][MAY INTEREST",
		" AMOUNT]+[PPTMGT][PPTYEAR][JUNE INTEREST AMOUNT]+[",
		"PPTMGT][PPTYEAR][JULY INTEREST AMOUNT]+[PPTMGT][PP",
		"TYEAR][AUGUST INTEREST AMOUNT]+[PPTMGT][PPTYEAR][S",
		"EPTEMBER INTEREST AMOUNT]+[PPTMGT][PPTYEAR][OCTOBE",
		"R INTEREST AMOUNT]+[PPTMGT][PPTYEAR][NOVEMBER INTE",
		"REST AMOUNT]+[PPTMGT][PPTYEAR][DECEMBER INTEREST A",
		"MOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed interest amount for a piece of personal p",
		"roperty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR INTEREST TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([PERSONAL PROPERTY YEAR INTEREST AMOUNT TOTAL]+[P",
		"ERSONAL PROPERTY YEAR UNPROCESSED INTEREST AMOUNT ",
		"TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual i",
		"nterest amount for a piece of personal property.  ",
		"It includes processed and unprocessed interest.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY PAID AMOUNT]+[PPTMGT][P",
		"PTYEAR][FEBRUARY PAID AMOUNT]+[PPTMGT][PPTYEAR][MA",
		"RCH PAID AMOUNT]+[PPTMGT][PPTYEAR][APRIL PAID AMOU",
		"NT]+[PPTMGT][PPTYEAR][MAY PAID AMOUNT]+[PPTMGT][PP",
		"TYEAR][JUNE PAID AMOUNT]+[PPTMGT][PPTYEAR][JULY PA",
		"ID AMOUNT]+[PPTMGT][PPTYEAR][AUGUST PAID AMOUNT]+[",
		"PPTMGT][PPTYEAR][SEPTEMBER PAID AMOUNT]+[PPTMGT][P",
		"PTYEAR][OCTOBER PAID AMOUNT]+[PPTMGT][PPTYEAR][NOV",
		"EMBER PAID AMOUNT]+[PPTMGT][PPTYEAR][DECEMBER PAID",
		" AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed paid amount for a piece of personal prope",
		"rty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR PAID TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([PERSONAL PROPERTY YEAR PAID AMOUNT TOTAL]+[PERSO",
		"NAL PROPERTY YEAR UNPROCESSED PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"aid amount for a piece of personal property.  It i",
		"ncludes both processed and unprocessed payments.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY PENALTY AMOUNT]+[PPTMGT",
		"][PPTYEAR][FEBRUARY PENALTY AMOUNT]+[PPTMGT][PPTYE",
		"AR][MARCH PENALTY AMOUNT]+[PPTMGT][PPTYEAR][APRIL ",
		"PENALTY AMOUNT]+[PPTMGT][PPTYEAR][MAY PENALTY AMOU",
		"NT]+[PPTMGT][PPTYEAR][JUNE PENALTY AMOUNT]+[PPTMGT",
		"][PPTYEAR][JULY PENALTY AMOUNT]+[PPTMGT][PPTYEAR][",
		"AUGUST PENALTY AMOUNT]+[PPTMGT][PPTYEAR][SEPTEMBER",
		" PENALTY AMOUNT]+[PPTMGT][PPTYEAR][OCTOBER PENALTY",
		" AMOUNT]+[PPTMGT][PPTYEAR][NOVEMBER PENALTY AMOUNT",
		"]+[PPTMGT][PPTYEAR][DECEMBER PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual p",
		"rocessed penalty amount for a piece of personal pr",
		"operty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR PENALTY TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"([PERSONAL PROPERTY YEAR PENALTY AMOUNT TOTAL]+[PE",
		"RSONAL PROPERTY YEAR UNPROCESSED PENALTY AMOUNT TO",
		"TAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"This virtual field is used to reflect the annual p",
		"enalty amount for a piece of personal property.  I",
		"t includes both processed and unprocessed penaltie",
		"s.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR PROCESSED BALANCE");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s",
		"([PERSONAL PROPERTY YEAR TAX AMOUNT TOTAL]+[PERSON",
		"AL PROPERTY YEAR PENALTY AMOUNT TOTAL]+[PERSONAL P",
		"ROPERTY YEAR INTEREST AMOUNT TOTAL]-[PERSONAL PROP",
		"ERTY YEAR PAID AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"This virtual field represents the balance owed for",
		" a piece of personal property in a given year.  Th",
		"is includes only processed amounts.  Remember, pay",
		"ments may have been collected in another year.  Th",
		"is is only the annual balance, not the balance owe",
		"d on the Personal Property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY TAX AMOUNT]+[PPTMGT][PP",
		"TYEAR][FEBRUARY TAX AMOUNT]+[PPTMGT][PPTYEAR][MARC",
		"H TAX AMOUNT]+[PPTMGT][PPTYEAR][APRIL TAX AMOUNT]+",
		"[PPTMGT][PPTYEAR][MAY TAX AMOUNT]+[PPTMGT][PPTYEAR",
		"][JUNE TAX AMOUNT]+[PPTMGT][PPTYEAR][JULY TAX AMOU",
		"NT]+[PPTMGT][PPTYEAR][AUGUST TAX AMOUNT]+[PPTMGT][",
		"PPTYEAR][SEPTEMBER TAX AMOUNT]+[PPTMGT][PPTYEAR][O",
		"CTOBER TAX AMOUNT]+[PPTMGT][PPTYEAR][NOVEMBER TAX ",
		"AMOUNT]+[PPTMGT][PPTYEAR][DECEMBER TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the Annual P",
		"rocessed Tax amount for a piece of personal proper",
		"ty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR TAX TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s",
		"([PERSONAL PROPERTY YEAR TAX AMOUNT TOTAL]+[PERSON",
		"AL PROPERTY YEAR UNPROCESSED TAX AMOUNT TOTAL])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to represent the annual",
		" tax amount for a piece of personal property.  It ",
		"includes processed and unprocessed taxes.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR UNPROCESSED INTEREST AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY UNPROCESSED INTEREST AM",
		"OUNT]+[PPTMGT][PPTYEAR][FEBRUARY UNPROCESSED INTER",
		"EST AMOUNT]+[PPTMGT][PPTYEAR][MARCH UNPROCESSED IN",
		"TEREST AMOUNT]+[PPTMGT][PPTYEAR][APRIL UNPROCESSED",
		" INTEREST AMOUNT]+[PPTMGT][PPTYEAR][MAY UNPROCESSE",
		"D INTEREST AMOUNT]+[PPTMGT][PPTYEAR][JUNE UNPROCES",
		"SED INTEREST AMOUNT]+[PPTMGT][PPTYEAR][JULY UNPROC",
		"ESSED INTEREST AMOUNT]+[PPTMGT][PPTYEAR][AUGUST UN",
		"PROCESSED INTEREST AMOUNT]+[PPTMGT][PPTYEAR][SEPTE",
		"MBER UNPROCESSED INTEREST AMOUNT]+[PPTMGT][PPTYEAR",
		"][OCTOBER UNPROCESSED INTEREST AMOUNT]+[PPTMGT][PP",
		"TYEAR][NOVEMBER UNPROCESSED INTEREST AMOUNT]+[PPTM",
		"GT][PPTYEAR][DECEMBER UNPROCESSED INTEREST AMOUNT]",
		")");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed interest amount for a piece of personal",
		" property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR UNPROCESSED PAID AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY UNPROCESSED PAID AMOUNT",
		"]+[PPTMGT][PPTYEAR][FEBRUARY UNPROCESSED PAID AMOU",
		"NT]+[PPTMGT][PPTYEAR][MARCH UNPROCESSED PAID AMOUN",
		"T]+[PPTMGT][PPTYEAR][APRIL UNPROCESSED PAID AMOUNT",
		"]+[PPTMGT][PPTYEAR][MAY UNPROCESSED PAID AMOUNT]+[",
		"PPTMGT][PPTYEAR][JUNE UNPROCESSED PAID AMOUNT]+[PP",
		"TMGT][PPTYEAR][JULY UNPROCESSED PAID AMOUNT]+[PPTM",
		"GT][PPTYEAR][AUGUST UNPROCESSED PAID AMOUNT]+[PPTM",
		"GT][PPTYEAR][SEPTEMBER UNPROCESSED PAID AMOUNT]+[P",
		"PTMGT][PPTYEAR][OCTOBER UNPROCESSED PAID AMOUNT]+[",
		"PPTMGT][PPTYEAR][NOVEMBER UNPROCESSED PAID AMOUNT]",
		"+[PPTMGT][PPTYEAR][DECEMBER UNPROCESSED PAID AMOUN",
		"T])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed paid amount for a piece of personal pro",
		"perty.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR UNPROCESSED PENALTY AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY UNPROCESSED PENALTY AMO",
		"UNT]+[PPTMGT][PPTYEAR][FEBRUARY UNPROCESSED PENALT",
		"Y AMOUNT]+[PPTMGT][PPTYEAR][MARCH UNPROCESSED PENA",
		"LTY AMOUNT]+[PPTMGT][PPTYEAR][APRIL UNPROCESSED PE",
		"NALTY AMOUNT]+[PPTMGT][PPTYEAR][MAY UNPROCESSED PE",
		"NALTY AMOUNT]+[PPTMGT][PPTYEAR][JUNE UNPROCESSED P",
		"ENALTY AMOUNT]+[PPTMGT][PPTYEAR][JULY UNPROCESSED ",
		"PENALTY AMOUNT]+[PPTMGT][PPTYEAR][AUGUST UNPROCESS",
		"ED PENALTY AMOUNT]+[PPTMGT][PPTYEAR][SEPTEMBER UNP",
		"ROCESSED PENALTY AMOUNT]+[PPTMGT][PPTYEAR][OCTOBER",
		" UNPROCESSED PENALTY AMOUNT]+[PPTMGT][PPTYEAR][NOV",
		"EMBER UNPROCESSED PENALTY AMOUNT]+[PPTMGT][PPTYEAR",
		"][DECEMBER UNPROCESSED PENALTY AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual u",
		"nprocessed penalty amount for a piece of personal ",
		"property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","PERSONAL PROPERTY YEAR UNPROCESSED TAX AMOUNT TOTAL");
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=1;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s%s%s%s%s%s",
		"([PPTMGT][PPTYEAR][JANUARY UNPROCESSED TAX AMOUNT]",
		"+[PPTMGT][PPTYEAR][FEBRUARY UNPROCESSED TAX AMOUNT",
		"]+[PPTMGT][PPTYEAR][MARCH UNPROCESSED TAX AMOUNT]+",
		"[PPTMGT][PPTYEAR][APRIL UNPROCESSED TAX AMOUNT]+[P",
		"PTMGT][PPTYEAR][MAY UNPROCESSED TAX AMOUNT]+[PPTMG",
		"T][PPTYEAR][JUNE UNPROCESSED TAX AMOUNT]+[PPTMGT][",
		"PPTYEAR][JULY UNPROCESSED TAX AMOUNT]+[PPTMGT][PPT",
		"YEAR][AUGUST UNPROCESSED TAX AMOUNT]+[PPTMGT][PPTY",
		"EAR][SEPTEMBER UNPROCESSED TAX AMOUNT]+[PPTMGT][PP",
		"TYEAR][OCTOBER UNPROCESSED TAX AMOUNT]+[PPTMGT][PP",
		"TYEAR][NOVEMBER UNPROCESSED TAX AMOUNT]+[PPTMGT][P",
		"PTYEAR][DECEMBER UNPROCESSED TAX AMOUNT])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s",
		"This virtual field is used to reflect the annual a",
		"mount of unprocessed taxes for a piece of personal",
		" property.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW("PPTMGT","TAXABLE VALUE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTMST");
	i->keyname=NULL;
	i->module=stralloc("PPTMGT");
	i->file=stralloc("PPTYEAR");
	i->keyname=NULL;
	d->vtype=0;
	d->num=2;
	d->type=DOLLARS;
	d->length=20;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s%s",
		"IF([PPTMGT][PPTYEAR][PROPERTY VALUE]!=0.00) THEN RE",
		"TURN_VALUE=([PPTMGT][PPTYEAR][PROPERTY VALUE]+[PPTMG",
		"T][PPTYEAR][IMPROVEMENT VALUE]-[PPTMGT][PPTYEAR][T",
		"AX DEFERRED AMOUNT]) ELSE RETURN_VALUE=([PPTMGT][",
		"PPTMST][PROPERTY VALUE]+[PPTMGT][PPTMST][IMPROVEMENT",
		" VALUE]-[PPTMGT][PPTMST][TAX DEFERRED AMOUNT]) FI",
		" ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s",
		"This virtual field is a calculation of the Taxable",
		" Value of Personal Property Property from the form",
		"ula (Market Value+Building & Improvements-Defered ",
		"Amount) and any other factors that are included in",
		" the expression.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual("PPTMGT",d);
	if(d!=NULL) FreeDFvirtual(d);

}
