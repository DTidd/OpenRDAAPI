/* payw2ss.c - Print Side-By-Side W2's */
#ifndef WIN32
#define __NAM__ "payw2ss.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payw2ss.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct PassableStructs
{
	RDArsrc *mainrsrc;
	DBsort *sortfile;
	RangeScreen *rs;
	FILE *fp;
};
typedef struct PassableStructs PassableStruct;

struct Box13Items
{
	char *label;
	double amount;
};
typedef struct Box13Items Box13Item;
struct Box13Structs
{
	int num;
	Box13Item *items;
};
typedef struct Box13Structs Box13Struct;

/*
struct Box14Items
{
	char *label;
	double amount;
};
typedef struct Box14Items Box14Item;
struct Box14Structs
{
	int num;
	Box14Item *items;
};
typedef struct Box14Structs Box14Struct;


struct BoxStateItems
{
	char *label;
	char *ein;
	double gross;
	double amount;
	double unemp;
};
typedef struct BoxStateItems BoxStateItem;
struct BoxStateStructs
{
	int num;
	BoxStateItem *items;
};
typedef struct BoxStateStructs BoxStateStruct;

struct BoxLocalItems
{
	char *label;
	double gross;
	double amount;
};
typedef struct BoxLocalItems BoxLocalItem;
struct BoxLocalStructs
{
	int num;
	BoxLocalItem *items;
};
typedef struct BoxLocalStructs BoxLocalStruct;
*/

char *module="PAYROLL",*w2_expression=NULL;
short perdmg=(-1),payw2=(-1),payempr=(-1);
RDAvirtual *csortsequence=NULL;
double body_count=41;
/*
double body_count=11;
*/
char non_std=FALSE;
static void printw2func(RDArsrc *,PassableStruct *);
static void printw2alignfunc(RDArsrc *,PassableStruct *);

/*
static Box14Struct *Box14StructNew()
{
	Box14Struct *tmp=NULL;

	tmp=Rmalloc(sizeof(Box14Struct));
	tmp->num=0;
	tmp->items=NULL;
	return(tmp);
}
static BoxStateStruct *BoxStateStructNew()
{
	BoxStateStruct *tmp=NULL;

	tmp=Rmalloc(sizeof(BoxStateStruct));
	tmp->num=0;
	tmp->items=NULL;
	return(tmp);
}
static BoxLocalStruct *BoxLocalStructNew()
{
	BoxLocalStruct *tmp=NULL;

	tmp=Rmalloc(sizeof(BoxLocalStruct));
	tmp->num=0;
	tmp->items=NULL;
	return(tmp);
}
*/

/*
static void FreeBox14Struct(Box13Struct *BOX14)
{
	Box14Item *b=NULL;
	int x;

	if(BOX14!=NULL)
	{
		if(BOX14->items!=NULL)
		{
			for(x=0,b=BOX14->items;x<BOX14->num;++x,++b)
			{
				if(b->label!=NULL) Rfree(b->label);
			}
			Rfree(BOX14->items);
		}
		Rfree(BOX14);
		BOX14=NULL;
	}
}
static void FreeBoxStateStruct(BoxStateStruct *STATE)
{
	BoxStateItem *b=NULL;
	int x;

	if(STATE!=NULL)
	{
		if(STATE->items!=NULL)
		{
			for(x=0,b=STATE->items;x<STATE->num;++x,++b)
			{
				if(b->label!=NULL) Rfree(b->label);
				if(b->ein!=NULL) Rfree(b->ein);
			}
			Rfree(STATE->items);
		}
		Rfree(STATE);
		STATE=NULL;
	}
}
static void FreeBoxLocalStruct(BoxLocalStruct *LOCAL)
{
	BoxLocalItem *b=NULL;
	int x;

	if(LOCAL!=NULL)
	{
		if(LOCAL->items!=NULL)
		{
			for(x=0,b=LOCAL->items;x<LOCAL->num;++x,++b)
			{
				if(b->label!=NULL) Rfree(b->label);
			}
			Rfree(LOCAL->items);
		}
		Rfree(LOCAL);
		LOCAL=NULL;
	}
}
*/
static Box13Struct *Box13StructNew()
{
	Box13Struct *tmp=NULL;

	tmp=Rmalloc(sizeof(Box13Struct));
	tmp->num=0;
	tmp->items=NULL;
	return(tmp);
}
static void FreeBox13Struct(Box13Struct *B)
{
	Box13Item *b=NULL;
	int x;

	if(B!=NULL)
	{
		if(B->items)
		{
			for(x=0,b=B->items;x<B->num;++x,++b)
			{
				if(b->label!=NULL) Rfree(b->label);
			}
			Rfree(B->items);
		}
		Rfree(B);
		B=NULL;
	}
}
static void AddBox13Struct(Box13Struct *B,char *label,double amount)
{
	Box13Item *b=NULL;

	if(B!=NULL)
	{
		if(B->items!=NULL)
		{
                	B->items=Rrealloc(B->items,(B->num+1)*sizeof(Box13Item));
        	} else {
                	B->items=Rmalloc(sizeof(Box13Item));
                	B->num=0;
        	}
        	b=B->items+B->num;
        	if(!isEMPTY(label)) b->label=stralloc(label);
                	else b->label=NULL;
		b->amount=amount;
		B->num+=1;
	}
}
APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,payw2);
	AddFileFields(list,perdmg);
	AddFileFields(list,payempr);
	addAPPlibNoDuplicates(list,"[PAGE NO]");
	addAPPlibNoDuplicates(list,"[BODY COUNT]");
	addAPPlibNoDuplicates(list,"[BREAK LEVEL]");
	addAPPlibNoDuplicates(list,"[PRIMARY FILE NUMBER]");
	addAPPlibNoDuplicates(list,"[WRITE COUNT]");
	loadglobals(list);
	SORTAPPlib(list);
	return(list);
}
void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist();
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
void quitfunc(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	if(payempr!=(-1)) CLSNRD(payempr);
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(payw2!=(-1)) CLSNRD(payw2);
	if(w2_expression!=NULL) Rfree(w2_expression);
	ShutdownSubsystems();
}
void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
        NRDfield *field,*FIELDS=NULL;
        int x=0,numfields=0;
	short sortkeyno=(-1),keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	sortkeyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("PAYROLL","PRINT W2 SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=PP_translate(d->expression);
		} else { 
			csortsequence->expression=NULL;
		}
		csortsequence->computed=FALSE;
		csortsequence->range=FALSE;
		csortsequence->dlen=0;
		switch(csortsequence->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				csortsequence->value.string_value=NULL;
				csortsequence->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				csortsequence->value.string_value=Rmalloc(1);
				*csortsequence->value.string_value=0;
				csortsequence->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				csortsequence->value.float_value=Rmalloc(sizeof(double));
				*csortsequence->value.float_value=0;
				csortsequence->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				csortsequence->value.short_value=Rmalloc(sizeof(short));
				*csortsequence->value.short_value=0;
				csortsequence->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				csortsequence->value.integer_value=Rmalloc(sizeof(int));
				*csortsequence->value.integer_value=0;
				csortsequence->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
		addDBfield(dbsrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsrt,sortkeyno,csortsequence->name);
		FreeDFvirtual(d);
	}
        numfields=NUMFLDS(payw2);
        FIELDS=FLDPOINTER(payw2);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!strcmp(field->name,"CONTROL NUMBER"))
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
				addDBkeypart(dbsrt,sortkeyno,field->name);
			}
		}
	}
	keyno=KEYNUMBER(payw2,"PAYW2 KEY");
	if(keyno==(-1))
	{
		 prterr("Error Payroll Print W2's: Can't find the PAYW2 KEY, using Key Number 1");
		 keyno=1;
	}
	key=KEYNUM(payw2,keyno);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(payw2,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
			addDBkeypart(dbsrt,sortkeyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt)
{
	char deleteflag=FALSE;
	NRDkey *key=NULL;
	NRDpart *part;
	int x;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		switch(csortsequence->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				FINDFLDSETSTRING(dbsrt->fileno,csortsequence->name,
					csortsequence->value.string_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				FINDFLDSETCHAR(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				FINDFLDSETDOUBLE(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				FINDFLDSETSHORT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				FINDFLDSETINT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.integer_value);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
	}
	key=KEYNUM(dbsrt->fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(payw2,dbsrt->fileno,part->name);
		}
	}
	WRTNRD(dbsrt->fileno,0,NULL);
}
static void quit_print_w2(RDArsrc *prsrc,PassableStruct *p)
{
	if(prsrc!=NULL) free_rsrc(prsrc);
	if(p!=NULL) 
	{
		if(p->sortfile!=NULL) 
		{
			CLSDBsort(p->sortfile);
			FreeDBsort(p->sortfile);
		}
		if(p->rs!=NULL) FreeRangeScreen(p->rs);
		Rfree(p);
	}
}
static void print_align(RDArsrc *prtrsrc,PassableStruct *p)
{
	FILE *fp=NULL;
	char *message=NULL,*outdevice=NULL;

	readwidget(prtrsrc,"DEVICE");
	FINDRSCGETSTRING(prtrsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
			p->fp=fp;
			printw2alignfunc(prtrsrc,p);
			pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the device [%s] failed when trying to open pipe.",outdevice);
			ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!",message,NULL,FALSE);
			prterr(message);
			if(message!=NULL) Rfree(message); 
		}
	} else {
		ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!","No Output Device Selected!",NULL,FALSE);
	}
}
static void print(RDArsrc *prtrsrc,PassableStruct *p)
{
	FILE *fp=NULL;
	char *message=NULL,*outdevice=NULL;

	readwidget(prtrsrc,"DEVICE");
	FINDRSCGETSTRING(prtrsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
			p->fp=fp;
			printw2func(prtrsrc,p);
			pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the device [%s] failed when trying to open pipe.",outdevice);
			ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!",message,NULL,FALSE);
			prterr(message);
			if(message!=NULL) Rfree(message); 
		}
	} else {
		ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!","No Output Device Selected!",NULL,FALSE);
	}
}
void printw2(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE,payw2_keyno=0;
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	int x=0,cal_yr=0,cal_yr1=0;
	RDArsrc *tmprsrc=NULL,*prsrc=NULL;
	RDAvirtual *v;
	char *e=NULL,*sortname=NULL;
	char etest=TRUE,deleteflag=FALSE,*empr_id=NULL,*per_id=NULL;
	char *defaultprinter=NULL;
	PassableStruct *p=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"CALENDAR YEAR",&cal_yr);
	FINDRSCGETCHAR(mainrsrc,"NON STANDARD",&non_std);
	sortname=unique_name();
	sortfile=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(payw2,rs);
	ReadRangeScreen(payempr,rs);
	ReadRangeScreen(perdmg,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(payw2,"DIAGNOSTIC SCREEN",module,
		"Loading W2's....",NULL);
	if(tmprsrc!=NULL)
	{
		if(!ADVmakescrn(tmprsrc,TRUE))
		{	
			ForceWindowUpdate(tmprsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	payw2_keyno=KEYNUMBER(payw2,"PAYW2 KEY");
	if(payw2_keyno<1) payw2_keyno=1;
	ZERNRD(payw2);
	FINDFLDSETINT(payw2,"CALENDAR YEAR",cal_yr);
	ef=ADVGTENRDsec(payw2,payw2_keyno,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETINT(payw2,"CALENDAR YEAR",&cal_yr1);
		if(cal_yr!=cal_yr1) break;
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(payw2,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(payw2,"PERSONNEL IDENTIFICATION",&per_id);
			FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",per_id);
			if(!ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(perdmg,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FINDFLDGETSTRING(payw2,"EMPLOYER IDENTIFICATION",&empr_id);
					FINDFLDSETSTRING(payempr,"PERSONNEL IDENTIFICATION",per_id);
					FINDFLDSETSTRING(payempr,"EMPLOYER IDENTIFICATION",empr_id);
					if(!ADVEQLNRDsec(payempr,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDGETCHAR(payempr,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							if(!testfilerangersrc(payw2,rs) && 
								!testfilerangersrc(payempr,rs) &&
								!testfilerangersrc(perdmg,rs)) 
							{
								for(x=0,v=mainrsrc->virflds;
									x<mainrsrc->num;++x,++v)
								{
									if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mainrsrc)) 
									{
										break;
									}
								}
								if(x>=mainrsrc->num)
								{
									if(!isEMPTY(w2_expression))
									{
										etest=EVALUATEbol(w2_expression,SCRNvirtualSubData,mainrsrc);
									} else {
										etest=TRUE;
									}
									if(etest)
									{
										if(csortsequence!=NULL)
										{
											computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
										}
										addsortrecord(sortfile);
										increment_diagnostic=TRUE;
									}
								}
							}
						}
					}
				}
			}
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		ef=ADVNXTNRDsec(payw2,payw2,SCRNvirtualSubData,mainrsrc);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(empr_id!=NULL) Rfree(empr_id);
	if(per_id!=NULL) Rfree(per_id);

	p=Rmalloc(sizeof(PassableStruct));
	p->mainrsrc=mainrsrc;
	p->sortfile=sortfile;
	p->rs=rs;

	prsrc=RDArsrcNEW("PAYROLL","PRINT W2 DEVICE");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addbtnrsrc(prsrc,"PRINT",TRUE,print,p);
	addbtnrsrc(prsrc,"PRINT ALIGNMENT TEST",TRUE,print_align,p);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_w2,p);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT W2 DEVICE. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_w2(prsrc,p);
	}
}
static void printw2alignfunc(RDArsrc *prtrsrc,PassableStruct *p)
{
	FILE *fp=NULL;
	char *X32="XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

	if(p!=NULL) 
	{
		fp=p->fp;
	}
	if(non_std==TRUE)
	{
		fprintf(fp," %-68s\r\n"," ");
		fprintf(fp," %-68s\r\n"," ");
	}
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %11d%-57s",0," "); fprintf(fp,"%11d\r\n",0);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-32s%-36s",X32," "); fprintf(fp,"%-32s\r\n",X32);
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-32s%-36s",X32," "); fprintf(fp,"%-32s\r\n",X32);
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s","XXX-XX-XXXX",0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n","XXX-XX-XXXX",0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-32s%-36s",X32," ");fprintf(fp,"%-32s\r\n",X32);
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s",X32,"X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s",X32,"X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s",X32,"X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s"," ","X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp,"                                    %c   %c   %c   %c   %c       %c %-8s",'X','X','X','X','X','X'," "); fprintf(fp,"                                  %c   %c   %c   %c   %c       %c \r\n",'X','X','X','X','X','X');
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-3s %-13s  %9.02f %9.02f  ","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f%-4s","XXXXXX",0.00,0.00," "); fprintf(fp,"%-3s %-13s  %9.02f %9.02f","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f\r\n","XXXXXX",0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-3s %-13s  %9.02f %9.02f  ","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f%-4s","XXXXXX",0.00,0.00," "); fprintf(fp,"%-3s %-13s  %9.02f %9.02f ","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f\r\n","XXXXXX",0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	if(non_std==FALSE)
	{
		fprintf(fp," %-68s\r\n"," ");
		fprintf(fp," %-68s\r\n"," ");
	}
 

	if(non_std==TRUE)
	{
		fprintf(fp," %-68s\r\n"," ");
		fprintf(fp," %-68s\r\n"," ");
	}
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %11d%-57s",0," "); fprintf(fp,"%11d\r\n",0);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-32s%-36s",X32," "); fprintf(fp,"%-32s\r\n",X32);
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-32s%-36s",X32," "); fprintf(fp,"%-32s\r\n",X32);
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s","XXX-XX-XXXX",0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n","XXX-XX-XXXX",0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-32s  %14.02f  %13.02f%-5s",X32,0.00,0.00," "); fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",X32,0.00,0.00);
	fprintf(fp," %-32s%-36s",X32," ");fprintf(fp,"%-32s\r\n",X32);
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s",X32,"X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s",X32,"X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s",X32,"X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s"," ","X",0.00,"XXXXXXXXXXXX"," ");fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",X32,"X",0.00,"XXXXXXXXXXXX");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp,"                                    %c   %c   %c   %c   %c       %c %-8s",'X','X','X','X','X','X'," "); fprintf(fp,"                                  %c   %c   %c   %c   %c       %c \r\n",'X','X','X','X','X','X');
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-3s %-13s  %9.02f %9.02f  ","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f%-4s","XXXXXX",0.00,0.00," "); fprintf(fp,"%-3s %-13s  %9.02f %9.02f ","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f\r\n","XXXXXX",0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-3s %-13s  %9.02f %9.02f  ","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f%-4s","XXXXXX",0.00,0.00," "); fprintf(fp,"%-3s %-13s  %9.02f %9.02f ","XX","XXXXXXXXXXXXX",0.00,0.00); fprintf(fp,"%-6s  %8.02f %7.02f\r\n","XXXXXX",0.00,0.00);
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	fprintf(fp," %-68s\r\n"," ");
	if(non_std==FALSE)
	{
		fprintf(fp," %-68s\r\n"," ");
		fprintf(fp," %-68s\r\n"," ");
	}
}
static void printw2func(RDArsrc *prtrsrc,PassableStruct *p)
{
	FILE *fp=NULL;
	DBsort *sortfile=NULL;
	RDArsrc *mainrsrc=NULL;
	short ef=0,increment_diagnostic=TRUE,payw2_keyno=0;
	int cal_yr=0,cont_no=0;
	int sizeof_sort=0;
	RDArsrc *tmprsrc=NULL;
	DFvirtual *d=NULL;
	char *e=NULL;
	double w2_counter=0.0; 
	double mod_value=0.0; 
	char *empr_id=NULL,*per_id=NULL;
	Box13Struct *BOX13=NULL;
	Box13Item *b=NULL;

	char *empr_idno=NULL,*empr_name1=NULL,*empr_name2=NULL;
	char *empr_addr1=NULL,*empr_addr2=NULL,*empr_addr3=NULL;
	char *empr_city=NULL,*empr_state=NULL,*empr_zip=NULL;
	char *ssn=NULL,*empl_name=NULL,*empl_addr1=NULL;
	char *empl_addr2=NULL,*empl_addr3=NULL;
	char *empl_city=NULL,*empl_state=NULL,*empl_zip=NULL;
	char statutory=' ',deceased=' ',penplan=' ',legrep=' ',hsldemp=' ';
	char defcomp=' ';
	char *st1_label=NULL,*st1_ein=NULL,*loc1_label=NULL;
	double st1_gross=0.0,st1_amount=0.0,loc1_gross=0.0,loc1_amount=0.0;
	char *st2_label=NULL,*st2_ein=NULL,*loc2_label=NULL;
	double st2_gross=0.0,st2_amount=0.0,loc2_gross=0.0,loc2_amount=0.0;

	double box1=0.0,box2=0.0,box3=0.0; 
	double box4=0.0,box5=0.0,box6=0.0; 
	double box7=0.0,box8=0.0,box9=0.0; 
	double box10=0.0;
	double box11A=0.0,box11B=0.0,box12=0.0; 
	double box13A=0.0,box13B=0.0,box13C=0.0; 
	double box13D=0.0,box13E=0.0,box13F=0.0; 
	double box13G=0.0,box13H=0.0,box13J=0.0; 
	double box13K=0.0,box13L=0.0,box13M=0.0; 
	double box13N=0.0,box13P=0.0,box13Q=0.0; 
	double box14A=0.0,box14B=0.0; 
	char *box13A_label=NULL,*box13B_label=NULL,*box13C_label=NULL; 
	char *box13D_label=NULL,*box13E_label=NULL,*box13F_label=NULL; 
	char *box13G_label=NULL,*box13H_label=NULL,*box13J_label=NULL; 
	char *box13K_label=NULL,*box13L_label=NULL,*box13M_label=NULL; 
	char *box13N_label=NULL,*box13P_label=NULL,*box13Q_label=NULL; 
	char *box14A_label=NULL,*box14B_label=NULL; 

	double subtotal_box1=0.0,subtotal_box2=0.0,subtotal_box3=0.0; 
	double subtotal_box4=0.0,subtotal_box5=0.0,subtotal_box6=0.0; 
	double subtotal_box7=0.0,subtotal_box8=0.0,subtotal_box9=0.0; 
	double subtotal_box10=0.0;
	double subtotal_box11A=0.0,subtotal_box11B=0.0,subtotal_box12=0.0; 

	double subtotal_box13D=0.0,subtotal_box13E=0.0,subtotal_box13F=0.0; 
	double subtotal_box13G=0.0,subtotal_box13H=0.0; 
/*
	double subtotal_box14A=0.0,subtotal_box14B=0.0; 
	char *subtotal_box14A_label=NULL,*subtotal_box14B_label=NULL; 
	double subtotal_box13A=0.0,subtotal_box13B=0.0,subtotal_box13C=0.0; 
	double subtotal_box13J=0.0; 
	double subtotal_box13K=0.0,subtotal_box13L=0.0,subtotal_box13M=0.0; 
	double subtotal_box13N=0.0,subtotal_box13P=0.0,subtotal_box13Q=0.0; 
*/

/*
	double total_box1=0.0,total_box2=0.0,total_box3=0.0; 
	double total_box4=0.0,total_box5=0.0,total_box6=0.0; 
	double total_box7=0.0,total_box8=0.0,total_box9=0.0; 
	double total_box10=0.0;
	doubld total_box11A=0.0,total_box11B=0.0,total_box12=0.0; 
	double total_box13A=0.0,total_box13B=0.0,total_box13C=0.0; 
	double total_box13D=0.0,total_box13E=0.0,total_box13F=0.0; 
	double total_box13G=0.0,total_box13H=0.0,total_box13J=0.0; 
	double total_box13K=0.0,total_box13L=0.0,total_box13M=0.0; 
	double total_box13N=0.0,total_box13P=0.0,total_box13Q=0.0; 
*/

	if(p!=NULL) 
	{
		mainrsrc=p->mainrsrc;
		sortfile=p->sortfile;
		fp=p->fp;
	}
	sizeof_sort=SIZNRD(sortfile->fileno);
	if(sizeof_sort>0)
	{
		tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
			module,"Printing W2's ...",NULL);
		if(tmprsrc!=NULL)
		{
			if(!ADVmakescrn(tmprsrc,TRUE))
			{	
				ForceWindowUpdate(tmprsrc);
			} else {
				e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
				sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
				prterr("Error %s.",e);
				if(e!=NULL) Rfree(e);
			}
		}
		ZERNRD(sortfile->fileno);
		ef=GTENRD(sortfile->fileno,1);
		w2_counter=0.0; 
		while(!ef)
		{
			FINDFLDGETINT(sortfile->fileno,"CALENDAR YEAR",&cal_yr);
			FINDFLDGETSTRING(sortfile->fileno,"EMPLOYER IDENTIFICATION",&empr_id);
			FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&per_id);
			ZERNRD(payw2);
			FINDFLDSETINT(payw2,"CALENDAR YEAR",cal_yr);
			FINDFLDSETSTRING(payw2,"EMPLOYER IDENTIFICATION",empr_id);
			FINDFLDSETSTRING(payw2,"PERSONNEL IDENTIFICATION",per_id);
			if(!ADVEQLNRDsec(payw2,payw2_keyno,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDSETSTRING(payempr,"EMPLOYER IDENTIFICATION",empr_id);
				if(!ADVEQLNRDsec(payempr,1,SCRNvirtualSubData,mainrsrc))
				{
					FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",per_id);
					if(!ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDGETDOUBLE(payw2,"FEDERAL WAGES",&box1);
						FINDFLDGETDOUBLE(payw2,"FEDERAL WITHHOLDING",&box2);
						FINDFLDGETDOUBLE(payw2,"FICA WAGES",&box3);
						FINDFLDGETDOUBLE(payw2,"FICA WITHHOLDING",&box4);
						FINDFLDGETDOUBLE(payw2,"MEDICARE WAGES",&box5);
						FINDFLDGETDOUBLE(payw2,"MEDICARE WITHHOLDING",&box6);
						FINDFLDGETDOUBLE(payw2,"FICA TIPS",&box7);
						FINDFLDGETDOUBLE(payw2,"ALLOCATED TIPS",&box8);
						FINDFLDGETDOUBLE(payw2,"AEIC PAYMENT",&box9);
						FINDFLDGETDOUBLE(payw2,"DEPENDENT CARE BENEFIT",&box10);
						FINDFLDGETDOUBLE(payw2,"NONQUALIFED 457",&box11A);
						FINDFLDGETDOUBLE(payw2,"NONQUALIFIED NON457",&box11B);
						FINDFLDGETDOUBLE(payw2,"BENEFITS IN FEDERAL WAGES",&box12);
						BOX13=Box13StructNew();
						FINDFLDGETDOUBLE(payw2,"BOX 13-A AMOUNT",&box13A);
						FINDFLDGETSTRING(payw2,"BOX 13-A LABEL",&box13A_label);
						if(box13A!=0.00)
						{
							AddBox13Struct(BOX13,box13A_label,box13A);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-B AMOUNT",&box13B);
						FINDFLDGETSTRING(payw2,"BOX 13-B LABEL",&box13B_label);
						if(box13B!=0.00)
						{
							AddBox13Struct(BOX13,box13B_label,box13B);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-C AMOUNT",&box13C);
						FINDFLDGETSTRING(payw2,"BOX 13-C LABEL",&box13C_label);
						if(box13C!=0.00)
						{
							AddBox13Struct(BOX13,box13C_label,box13C);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-D AMOUNT",&box13D);
						FINDFLDGETSTRING(payw2,"BOX 13-D LABEL",&box13D_label);
						if(box13D!=0.00)
						{
							AddBox13Struct(BOX13,box13D_label,box13D);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-E AMOUNT",&box13E);
						FINDFLDGETSTRING(payw2,"BOX 13-E LABEL",&box13E_label);
						if(box13E!=0.00)
						{
							AddBox13Struct(BOX13,box13E_label,box13E);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-F AMOUNT",&box13F);
						FINDFLDGETSTRING(payw2,"BOX 13-F LABEL",&box13F_label);
						if(box13F!=0.00)
						{
							AddBox13Struct(BOX13,box13F_label,box13F);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-G AMOUNT",&box13G);
						FINDFLDGETSTRING(payw2,"BOX 13-G LABEL",&box13G_label);
						if(box13G!=0.00)
						{
							AddBox13Struct(BOX13,box13G_label,box13G);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-H AMOUNT",&box13H);
						FINDFLDGETSTRING(payw2,"BOX 13-H LABEL",&box13H_label);
						if(box13H!=0.00)
						{
							AddBox13Struct(BOX13,box13H_label,box13H);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-J AMOUNT",&box13J);
						FINDFLDGETSTRING(payw2,"BOX 13-J LABEL",&box13J_label);
						if(box13J!=0.00)
						{
							AddBox13Struct(BOX13,box13J_label,box13J);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-K AMOUNT",&box13K);
						FINDFLDGETSTRING(payw2,"BOX 13-K LABEL",&box13K_label);
						if(box13K!=0.00)
						{
							AddBox13Struct(BOX13,box13K_label,box13K);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-L AMOUNT",&box13L);
						FINDFLDGETSTRING(payw2,"BOX 13-L LABEL",&box13L_label);
						if(box13L!=0.00)
						{
							AddBox13Struct(BOX13,box13L_label,box13L);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-M AMOUNT",&box13M);
						FINDFLDGETSTRING(payw2,"BOX 13-M LABEL",&box13M_label);
						if(box13M!=0.00)
						{
							AddBox13Struct(BOX13,box13M_label,box13M);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-N AMOUNT",&box13N);
						FINDFLDGETSTRING(payw2,"BOX 13-N LABEL",&box13N_label);
						if(box13N!=0.00)
						{
							AddBox13Struct(BOX13,box13N_label,box13N);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-P AMOUNT",&box13P);
						FINDFLDGETSTRING(payw2,"BOX 13-P LABEL",&box13P_label);
						if(box13P!=0.00)
						{
							AddBox13Struct(BOX13,box13P_label,box13P);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 13-Q AMOUNT",&box13Q);
						FINDFLDGETSTRING(payw2,"BOX 13-Q LABEL",&box13Q_label);
						if(box13Q!=0.00)
						{
							AddBox13Struct(BOX13,box13Q_label,box13Q);
						}
						FINDFLDGETDOUBLE(payw2,"BOX 14-A AMOUNT",&box14A);
						FINDFLDGETSTRING(payw2,"BOX 14-A LABEL",&box14A_label);
						if(RDAstrlen(box14A_label)>12)
						{
							box14A_label[12]=0;
						}
						FINDFLDGETDOUBLE(payw2,"BOX 14-B AMOUNT",&box14B);
						FINDFLDGETSTRING(payw2,"BOX 14-B LABEL",&box14B_label);
						if(RDAstrlen(box14B_label)>12)
						{
							box14B_label[12]=0;
						}
/*
						total_box1+=box1;
						total_box2+=box2;
						total_box3+=box3;
						total_box4+=box4;
						total_box5+=box5;
						total_box6+=box6;
						total_box7+=box7;
						total_box8+=box8;
						total_box9+=box9;
						total_box10+=box10;
						total_box11A+=box11A;
						total_box11B+=box11B;
						total_box12+=box12;
						total_box13A+=box13A;
						total_box13B+=box13B;
						total_box13C+=box13C;
						total_box13D+=box13D;
						total_box13E+=box13E;
						total_box13F+=box13F;
						total_box13G+=box13G;
						total_box13H+=box13H;
						total_box13J+=box13J;
						total_box13K+=box13K;
						total_box13L+=box13L;
						total_box13M+=box13M;
						total_box13N+=box13N;
						total_box13P+=box13P;
						total_box13Q+=box13Q;
*/

						subtotal_box1+=box1;
						subtotal_box2+=box2;
						subtotal_box3+=box3;
						subtotal_box4+=box4;
						subtotal_box5+=box5;
						subtotal_box6+=box6;
						subtotal_box7+=box7;
						subtotal_box8+=box8;
						subtotal_box9+=box9;
						subtotal_box10+=box10;
						subtotal_box11A+=box11A;
						subtotal_box11B+=box11B;
						subtotal_box12+=box12;
						subtotal_box13D+=box13D;
						subtotal_box13E+=box13E;
						subtotal_box13F+=box13F;
						subtotal_box13G+=box13G;
						subtotal_box13H+=box13H;
/*
						subtotal_box14A+=box14A;
						subtotal_box14B+=box14B;
						subtotal_box13A+=box13A;
						subtotal_box13B+=box13B;
						subtotal_box13C+=box13C;
						subtotal_box13J+=box13J;
						subtotal_box13K+=box13K;
						subtotal_box13L+=box13L;
						subtotal_box13M+=box13M;
						subtotal_box13N+=box13N;
						subtotal_box13P+=box13P;
						subtotal_box13Q+=box13Q;
*/
						if(empr_id!=NULL) Rfree(empr_id);
						if(empr_idno!=NULL) Rfree(empr_idno);
						if(empr_name1!=NULL) Rfree(empr_name1);
						if(empr_name2!=NULL) Rfree(empr_name2);
						if(empr_addr1!=NULL) Rfree(empr_addr1);
						if(empr_addr2!=NULL) Rfree(empr_addr2);
						if(empr_addr3!=NULL) Rfree(empr_addr3);

						FINDFLDGETINT(payw2,"CONTROL NUMBER",&cont_no);
						FINDFLDGETSTRING(payempr,"FEDERAL EIN",&empr_idno);
						if(RDAstrlen(empr_idno)>32)
						{
							empr_idno[32]=0;
						}
						FINDFLDGETSTRING(payempr,"EMPLOYER NAME",&empr_name1);
						if(RDAstrlen(empr_name1)>32)
						{
							empr_name1[32]=0;
						}
						FINDFLDGETSTRING(payempr,"EMPLOYER NAME 2",&empr_name2);
						if(RDAstrlen(empr_name2)>32)
						{
							empr_name2[32]=0;
						}
						d=getDFvirtual("PAYROLL","EMPLOYER ADDRESS 1");
						if(d!=NULL)
						{
						switch(d->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								empr_addr1=EVALUATEstr(d->expression,SCRNvirtualSubData,mainrsrc);
								break;
							default:
								prterr("Error Field type [%d] is invalid for virtual field [%s].",d->type,d->name);
								FINDFLDGETSTRING(payempr,"ADDRESS 1",&empr_addr1);
								break;
						}
						FreeDFvirtual(d);
						} else {
							FINDFLDGETSTRING(payempr,"ADDRESS 1",&empr_addr1);
						}
						if(RDAstrlen(empr_addr1)>32)
						{
							empr_addr1[32]=0;
						}
						d=getDFvirtual("PAYROLL","EMPLOYER ADDRESS 2");
						if(d!=NULL)
						{
						switch(d->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								empr_addr2=EVALUATEstr(d->expression,SCRNvirtualSubData,mainrsrc);
								break;
							default:
								prterr("Error Field type [%d] is invalid for virtual field [%s].",d->type,d->name);
								FINDFLDGETSTRING(payempr,"ADDRESS 2",&empr_addr2);
								break;
						}
						FreeDFvirtual(d);
						} else {
							FINDFLDGETSTRING(payempr,"ADDRESS 2",&empr_addr2);
						}
						if(RDAstrlen(empr_addr2)>32)
						{
							empr_addr2[32]=0;
						}
						d=getDFvirtual("PAYROLL","EMPLOYER ADDRESS 3");
						if(d!=NULL)
						{
						switch(d->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								empr_addr3=EVALUATEstr(d->expression,SCRNvirtualSubData,mainrsrc);
								break;
							default:
								prterr("Error Field type [%d] is invalid for virtual field [%s].",d->type,d->name);
								FINDFLDGETSTRING(payempr,"CITY",&empr_city);
								FINDFLDGETSTRING(payempr,"STATE",&empr_state);
								FINDFLDGETSTRING(payempr,"ZIP",&empr_zip);
								empr_addr3=Rmalloc(RDAstrlen(empr_city)+RDAstrlen(empr_state)+RDAstrlen(empr_zip)+6);
								sprintf(empr_addr3,"%s, %s %s",empr_city,empr_state,empr_zip);
								if(empr_city!=NULL) Rfree(empr_city);
								if(empr_state!=NULL) Rfree(empr_state);
								if(empr_zip!=NULL) Rfree(empr_zip);
								break;
						}
						FreeDFvirtual(d);
						} else {
							FINDFLDGETSTRING(payempr,"CITY",&empr_city);
							FINDFLDGETSTRING(payempr,"STATE",&empr_state);
							FINDFLDGETSTRING(payempr,"ZIP",&empr_zip);
							empr_addr3=Rmalloc(RDAstrlen(empr_city)+RDAstrlen(empr_state)+RDAstrlen(empr_zip)+6);
							sprintf(empr_addr3,"%s, %s %s",empr_city,empr_state,empr_zip);
							if(empr_city!=NULL) Rfree(empr_city);
							if(empr_state!=NULL) Rfree(empr_state);
							if(empr_zip!=NULL) Rfree(empr_zip);
						}
						if(RDAstrlen(empr_addr3)>32)
						{
							empr_addr3[32]=0;
						}
						FINDFLDGETSTRING(perdmg,"SOCIAL SECURITY NUMBER",&ssn);
						d=getDFvirtual("PAYROLL","W2 EMPLOYEE NAME");
						if(d!=NULL)
						{
						switch(d->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								empl_name=EVALUATEstr(d->expression,SCRNvirtualSubData,mainrsrc);
								break;
							default:
								prterr("Error Field type [%d] is invalid for virtual field [%s].",d->type,d->name);
								FINDFLDGETSTRING(perdmg,"NAME",&empl_name);
								break;
						}
						FreeDFvirtual(d);
						} else {
							FINDFLDGETSTRING(perdmg,"NAME",&empl_name);
						}
						if(RDAstrlen(empl_name)>32)
						{
							empl_name[32]=0;
						}
						d=getDFvirtual("PAYROLL","W2 PERSONNEL MAILING ADDRESS 1");
						if(d!=NULL)
						{
						switch(d->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								empl_addr1=EVALUATEstr(d->expression,SCRNvirtualSubData,mainrsrc);
								break;
							default:
								prterr("Error Field type [%d] is invalid for virtual field [%s].",d->type,d->name);
								FINDFLDGETSTRING(perdmg,"MAILING ADDRESS 1",&empl_addr1);
								break;
						}
						FreeDFvirtual(d);
						} else {
							FINDFLDGETSTRING(payempr,"MAILING ADDRESS 1",&empl_addr1);
						}
						if(RDAstrlen(empl_addr1)>32)
						{
							empl_addr1[32]=0;
						}
						d=getDFvirtual("PAYROLL","W2 PERSONNEL MAILING ADDRESS 2");
						if(d!=NULL)
						{
						switch(d->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								empl_addr2=EVALUATEstr(d->expression,SCRNvirtualSubData,mainrsrc);
								break;
							default:
								prterr("Error Field type [%d] is invalid for virtual field [%s].",d->type,d->name);
								FINDFLDGETSTRING(perdmg,"MAILING ADDRESS 2",&empl_addr2);
								break;
						}
						FreeDFvirtual(d);
						} else {
							FINDFLDGETSTRING(payempr,"MAILING ADDRESS 2",&empl_addr2);
						}
						if(RDAstrlen(empl_addr2)>32)
						{
							empl_addr2[32]=0;
						}
						d=getDFvirtual("PAYROLL","W2 PERSONNEL MAILING ADDRESS 3");
						if(d!=NULL)
						{
						switch(d->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								empl_addr3=EVALUATEstr(d->expression,SCRNvirtualSubData,mainrsrc);
								break;
							default:
								prterr("Error Field type [%d] is invalid for virtual field [%s].",d->type,d->name);
								FINDFLDGETSTRING(perdmg,"CITY",&empl_city);
								FINDFLDGETSTRING(perdmg,"STATE",&empl_state);
								FINDFLDGETSTRING(perdmg,"ZIP",&empl_zip);
								empl_addr3=Rmalloc(RDAstrlen(empl_city)+RDAstrlen(empl_state)+RDAstrlen(empl_zip)+6);
								sprintf(empl_addr3,"%s, %s %s",empl_city,empl_state,empl_zip);
								if(empl_city!=NULL) Rfree(empl_city);
								if(empl_state!=NULL) Rfree(empl_state);
								if(empl_zip!=NULL) Rfree(empl_zip);
								break;
						}
						FreeDFvirtual(d);
						} else {
							FINDFLDGETSTRING(perdmg,"CITY",&empl_city);
							FINDFLDGETSTRING(perdmg,"STATE",&empl_state);
							FINDFLDGETSTRING(perdmg,"ZIP",&empl_zip);
							empl_addr3=Rmalloc(RDAstrlen(empl_city)+RDAstrlen(empl_state)+RDAstrlen(empl_zip)+6);
							sprintf(empr_addr3,"%s, %s %s",empl_city,empl_state,empl_zip);
							if(empl_city!=NULL) Rfree(empl_city);
							if(empl_state!=NULL) Rfree(empl_state);
							if(empl_zip!=NULL) Rfree(empl_zip);
						}
						if(RDAstrlen(empl_addr3)>32)
						{
							empl_addr3[32]=0;
						}
						w2_counter+=1;
						if(non_std==TRUE)
						{
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
						}
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %11d%-57s",cont_no," ");
						fprintf(fp,"%11d\r\n",cont_no);
						fprintf(fp,"     %-68s\r\n"," ");
						fprintf(fp," %-32s  %14.02f  %13.02f%-5s",(empr_idno!=NULL?empr_idno:""),box1/100,box2/100," ");
						fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",(empr_idno!=NULL?empr_idno:" "),box1/100,box2/100);
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-32s  %14.02f  %13.02f%-5s",(empr_name1!=NULL?empr_name1:""),box3/100,box4/100," ");
						fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",(empr_name1!=NULL?empr_name1:""),box3/100,box4/100);
						fprintf(fp," %-32s%-36s",empr_name2," ");
						fprintf(fp,"%-32s\r\n",empr_name2);
						fprintf(fp," %-32s  %14.02f  %13.02f%-5s",(empr_addr1!=NULL?empr_addr1:""),box5/100,box6/100," ");
						fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",(empr_addr1!=NULL?empr_addr1:""),box5/100,box6/100);
						fprintf(fp," %-32s%-36s",(empr_addr2!=NULL?empr_addr2:"")," ");
						fprintf(fp,"%-32s\r\n",(empr_addr2!=NULL?empr_addr2:""));
						fprintf(fp," %-32s  %14.02f  %13.02f%-5s",(empr_addr3!=NULL?empr_addr3:""),box7/100,box8/100," ");
						fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",(empr_addr3!=NULL?empr_addr3:""),box7/100,box8/100);
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-32s  %14.02f  %13.02f%-5s",ssn,box9/100,box10/100," ");
						fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",ssn,box9/100,box10/100);
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-32s  %14.02f  %13.02f%-5s",empl_name,((box11A+box11B)/100),box12/100," ");
						fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",empl_name,((box11A+box11B)/100),box12/100);
						fprintf(fp," %-32s%-36s",empl_addr1," ");
						fprintf(fp,"%-32s\r\n",empl_addr1);
						if((BOX13->num)<1)
						{
							if(box14A!=0.00)
							{
								fprintf(fp," %-32s                    %-12s%-4s",empl_addr2,box14A_label," ");
								fprintf(fp,"%-32s                    %-12s\r\n",empl_addr2,box14A_label);
								fprintf(fp," %-32s                   %12.02f%-5s",(empl_addr3!=NULL?empl_addr3:""),(box14A/100)," ");
								fprintf(fp,"%-32s                   %12.02f\r\n",(empl_addr3!=NULL?empl_addr3:""),(box14A/100));
							} else {
								fprintf(fp," %-32s%-36s",empl_addr2," ");
								fprintf(fp,"%-32s\r\n",empl_addr2);
								fprintf(fp," %-32s%-36s",(empl_addr3!=NULL?empl_addr3:"")," ");
								fprintf(fp,"%-32s\r\n",(empl_addr3!=NULL?empl_addr3:""));
							}
							if(box14B!=0.00)
							{
								fprintf(fp,"                                                    %-12s%-4s",box14B_label," ");
								fprintf(fp,"                                                   %-12s\r\n",box14B_label);
								fprintf(fp,"                                                    %12.02f%-5s",(box14B/100)," ");
								fprintf(fp,"                                                   %12.02f\r\n",(box14B/100));
							} else {
								fprintf(fp," %-68s\r\n"," ");
								fprintf(fp," %-68s\r\n"," ");
							}
						} else {
							if((BOX13->num)>0)
							{
								b=BOX13->items;
								if(box14A!=0.00)
								{
									fprintf(fp," %-32s   %-1s %11.02f    %-12s%-4s",empl_addr2,b->label,b->amount/100,box14A_label," ");
									fprintf(fp,"%-32s   %-1s %11.02f    %-12s\r\n",empl_addr2,b->label,b->amount/100,box14A_label);
								} else {
									fprintf(fp," %-32s   %-1s %11.02f%-20s",empl_addr2,b->label,b->amount/100," ");
									fprintf(fp,"%-32s   %-1s %11.02f\r\n",empl_addr2,b->label,b->amount/100);
								}
							}
							if((BOX13->num)>1)
							{
								b=BOX13->items+1;
								if(box14A!=0.00)
								{
									fprintf(fp," %-32s   %-1s %11.02f   %12.02f%-5s",(empl_addr3!=NULL?empl_addr3:""),b->label,b->amount/100,box14A/100," ");
									fprintf(fp,"%-32s   %-1s %11.02f   %12.02f\r\n",(empl_addr3!=NULL?empl_addr3:""),b->label,b->amount/100,box14A/100);
								} else {
									fprintf(fp," %-32s   %-1s %11.02f%-20s",(empl_addr3!=NULL?empl_addr3:""),b->label,b->amount/100," ");
									fprintf(fp,"%-32s   %-1s %11.02f\r\n",(empl_addr3!=NULL?empl_addr3:""),b->label,b->amount/100);
								}
							} else { 
								if(box14A!=0.00)
								{
									fprintf(fp," %-32s                   %12.02f%-5s",(empl_addr3!=NULL?empl_addr3:""),box14A/100," ");
									fprintf(fp,"%-32s                   %12.02f\r\n",(empl_addr3!=NULL?empl_addr3:""),box14A/100);
								} else {
									fprintf(fp," %-32s%-36s",(empl_addr3!=NULL?empl_addr3:"")," ");
									fprintf(fp,"%-32s\r\n",(empl_addr3!=NULL?empl_addr3:""));
								}
							}
							if((BOX13->num)>2)
							{
								b=BOX13->items+2;
								if(box14B!=0.00)
								{
									fprintf(fp,"                                   %-1s %11.02f    %-12s%-4s",b->label,b->amount/100,box14B_label," ");
									fprintf(fp,"                                  %-1s %11.02f    %-12s\r\n",b->label,b->amount/100,box14B_label);
								} else {
									fprintf(fp,"                                    %-1s %11.02f%-20s",b->label,b->amount/100," ");
									fprintf(fp,"                                   %-1s %11.02f\r\n",b->label,b->amount/100);
								}
							} else { 
								if(box14B!=0.00)
								{
									fprintf(fp,"                                                    %-12s%-4s",box14B_label," ");
									fprintf(fp,"                                                   %-12s\r\n",box14B_label);
								} else {
									fprintf(fp," %-68s\r\n"," ");
								}
							}
							if((BOX13->num)>3)
							{
								b=BOX13->items+3;
								if(box14B!=0.00)
								{
									fprintf(fp,"                                    %-1s %11.02f    %12.02f%-5s",b->label,b->amount/100,box14B/100," ");
									fprintf(fp,"                                   %-1s %11.02f    %12.02f\r\n",b->label,b->amount/100,box14B/100);
								} else {
									fprintf(fp,"                                    %-1s %11.02f%-20s",b->label,b->amount/100," ");
									fprintf(fp,"                                   %-1s %11.02f\r\n",b->label,b->amount/100);
								}
							} else { 
								if(box14B!=0.00)
								{
									fprintf(fp,"                                                    %12.02f%-5s",box14B/100," ");
									fprintf(fp,"                                                   %12.02f\r\n",box14B/100);
								} else {
									fprintf(fp," %-68s\r\n"," ");
								}
							}
						}
						fprintf(fp," %-68s\r\n"," ");
						FINDFLDGETCHAR(payw2,"STATUTORY EMPLOYEE",&statutory);
						FINDFLDGETCHAR(payw2,"DECEASED",&deceased);
						FINDFLDGETCHAR(payw2,"PENSION PLAN",&penplan);
						FINDFLDGETCHAR(payw2,"LEGAL REP",&legrep);
						FINDFLDGETCHAR(payw2,"942 HSHLD EMP",&hsldemp);
						FINDFLDGETCHAR(payw2,"DEFERRED COMPENSATION",&defcomp);
						fprintf(fp,"                                    %c   %c   %c   %c   %c       %c %-8s",(statutory==TRUE?'X':' '),(deceased==TRUE?'X':' '),(penplan==TRUE?'X':' '),(legrep==TRUE?'X':' '),(hsldemp==TRUE?'X':' '),(defcomp==TRUE?'X':' ')," ");
						fprintf(fp,"                                  %c   %c   %c   %c   %c       %c \r\n",(statutory==TRUE?'X':' '),(deceased==TRUE?'X':' '),(penplan==TRUE?'X':' '),(legrep==TRUE?'X':' '),(hsldemp==TRUE?'X':' '),(defcomp==TRUE?'X':' '));
						fprintf(fp," %-68s\r\n"," ");
						FINDFLDGETSTRING(payw2,"STATE 1 LABEL",&st1_label);
						if(RDAstrlen(st1_label)>3)
						{
							st1_label[3]=0;
						}
						FINDFLDGETSTRING(payw2,"STATE 1 EIN",&st1_ein);
						if(RDAstrlen(st1_ein)>13)
						{
							st1_ein[13]=0;
						}
						FINDFLDGETDOUBLE(payw2,"STATE 1 WAGES",&st1_gross);
						FINDFLDGETDOUBLE(payw2,"STATE 1 WITHHOLDING",&st1_amount);
						FINDFLDGETSTRING(payw2,"LOCAL 1 LABEL",&loc1_label);
						if(RDAstrlen(loc1_label)>6)
						{
							loc1_label[6]=0;
						}
						FINDFLDGETDOUBLE(payw2,"LOCAL 1 WAGES",&loc1_gross);
						FINDFLDGETDOUBLE(payw2,"LOCAL 1 WITHHOLDING",&loc1_amount);
						if((!isEMPTY(st1_label)) && (!isEMPTY(st1_ein)))
						{
							fprintf(fp," %-3s %-13s  %9.02f %9.02f  ",st1_label,st1_ein,st1_gross/100,st1_amount/100);
						} else {
							fprintf(fp,"                                            ");
						}
						if((!isEMPTY(loc1_label)) && (loc1_gross!=0.0))
						{
							fprintf(fp,"%-6s  %8.02f %7.02f",loc1_label,loc1_gross/100,loc1_amount/100);
						} else {
							fprintf(fp,"%-28s"," ");
						}
						if((!isEMPTY(st1_label)) && (!isEMPTY(st1_ein)))
						{
							fprintf(fp,"%-3s %-13s  %9.02f %9.02f ",st1_label,st1_ein,st1_gross/100,st1_amount/100);
						} else {
							fprintf(fp,"                                           ");
						}
						if((!isEMPTY(loc1_label)) && (loc1_gross!=0.0))
						{
							fprintf(fp,"%-6s  %8.02f %7.02f\r\n",loc1_label,loc1_gross/100,loc1_amount/100);
						} else {
							fprintf(fp,"\r\n");
						}
						fprintf(fp," %-68s\r\n"," ");
						FINDFLDGETSTRING(payw2,"STATE 2 LABEL",&st2_label);
						if(RDAstrlen(st2_label)>3)
						{
							st2_label[3]=0;
						}
						FINDFLDGETSTRING(payw2,"STATE 2 EIN",&st2_ein);
						if(RDAstrlen(st2_ein)>13)
						{
							st2_ein[13]=0;
						}
						FINDFLDGETDOUBLE(payw2,"STATE 2 WAGES",&st2_gross);
						FINDFLDGETDOUBLE(payw2,"STATE 2 WITHHOLDING",&st2_amount);
						FINDFLDGETSTRING(payw2,"LOCAL 2 LABEL",&loc2_label);
						if(RDAstrlen(loc2_label)>6)
						{
							loc2_label[6]=0;
						}
						FINDFLDGETDOUBLE(payw2,"LOCAL 2 WAGES",&loc2_gross);
						FINDFLDGETDOUBLE(payw2,"LOCAL 2 WITHHOLDING",&loc2_amount);
						if((!isEMPTY(st2_label)) && (!isEMPTY(st2_ein)))
						{
							fprintf(fp," %-3s %-13s  %9.02f %9.02f  ",st2_label,st2_ein,st2_gross/100,st2_amount/100);
						} else {
							fprintf(fp,"                                        ");
						}
						if((!isEMPTY(loc2_label)) && (loc2_gross!=0.0))
						{
							fprintf(fp,"%-6s  %8.02f %7.02f%-2s",loc2_label,loc2_gross/100,loc2_amount/100," ");
						} else {
							fprintf(fp,"%-29s"," ");
						}
						if((!isEMPTY(st2_label)) && (!isEMPTY(st2_ein)))
						{
							fprintf(fp,"%-3s %-13s  %9.02f %9.02f  ",st2_label,st2_ein,st2_gross/100,st2_amount/100);
						} else {
							fprintf(fp,"                                       ");
						}
						if((!isEMPTY(loc2_label)) && (loc2_gross!=0.0))
						{
							fprintf(fp,"%-6s  %8.02f %7.02f\r\n",loc2_label,loc2_gross/100,loc2_amount/100);
						} else {
							fprintf(fp,"\r\n");
						}
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-68s\r\n"," ");
						fprintf(fp," %-68s\r\n"," ");
						if(non_std==FALSE)
						{
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
						}
						FreeBox13Struct(BOX13);

						mod_value=fmod(w2_counter,body_count);
						if(mod_value==0) 
						{
							if(non_std==TRUE)
							{
								fprintf(fp," %-68s\r\n"," ");
								fprintf(fp," %-68s\r\n"," ");
							}
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-32s  %14.02f  %13.02f%-5s",empr_idno,subtotal_box1/100,subtotal_box2/100," ");
							fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",empr_idno,subtotal_box1/100,subtotal_box2/100);
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-32s  %14.02f  %13.02f%-5s",empr_name1,subtotal_box3/100,subtotal_box4/100," ");
							fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",empr_name1,subtotal_box3/100,subtotal_box4/100);
							fprintf(fp," %-32s%-36s",empr_name2," ");
							fprintf(fp,"%-32s\r\n",empr_name2);
							fprintf(fp," %-32s  %14.02f  %13.02f%-5s",empr_addr1,subtotal_box5/100,subtotal_box6/100," ");
							fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",empr_addr1,subtotal_box5/100,subtotal_box6/100);
							fprintf(fp," %-32s%-36s",empr_addr2," ");
							fprintf(fp,"%-32s\r\n",empr_addr2);
							fprintf(fp," %-32s  %14.02f  %13.02f%-5s",(empr_addr3!=NULL?empr_addr3:""),subtotal_box7/100,subtotal_box8/100," ");
							fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",(empr_addr3!=NULL?empr_addr3:""),subtotal_box7/100,subtotal_box8/100);
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-32s  %14.02f  %13.02f%-5s"," ",subtotal_box9/100,subtotal_box10/100," ");
							fprintf(fp,"%-32s  %14.02f  %13.02f\r\n"," ",subtotal_box9/100,subtotal_box10/100);
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-32s  %14.02f  %13.02f%-5s"," ",((subtotal_box11A+subtotal_box11B)/100),subtotal_box12/100," ");
							fprintf(fp,"%-32s  %14.02f  %13.02f\r\n"," ",((subtotal_box11A+subtotal_box11B)/100),subtotal_box12/100);
							fprintf(fp," %-68s\r\n"," ");
							if((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)!=0.00)
							{
								fprintf(fp," %-32s   %-5s%-28s","","DEFGH"," ");
								fprintf(fp,"%-32s   %-5s\r\n","","DEFGH");
							} else {
								fprintf(fp," %-68s\r\n"," ");
							}
							if((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)!=0.00)
							{
								fprintf(fp," %-32s   %13.02f%-20s","",((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)/100)," ");
								fprintf(fp,"%-32s   %13.02f\r\n","",((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)/100));
							} else {
								fprintf(fp," %-68s\r\n"," ");
							}
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp,"                                                        %c%-12s",'X'," ");
							fprintf(fp,"                                                       %c\r\n",'X');
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							fprintf(fp," %-68s\r\n"," ");
							if(non_std==FALSE)
							{
								fprintf(fp," %-68s\r\n"," ");
								fprintf(fp," %-68s\r\n"," ");
							}

							w2_counter=0.0;
							subtotal_box1=0.0; 
							subtotal_box2=0.0; 
							subtotal_box3=0.0; 
							subtotal_box4=0.0; 
							subtotal_box5=0.0; 
							subtotal_box6=0.0; 
							subtotal_box7=0.0; 
							subtotal_box8=0.0; 
							subtotal_box9=0.0; 
							subtotal_box10=0.0; 
							subtotal_box11A=0.0; 
							subtotal_box11A=0.0; 
							subtotal_box12=0.0; 
							subtotal_box13D=0.0; 
							subtotal_box13E=0.0; 
							subtotal_box13F=0.0; 
							subtotal_box13G=0.0; 
							subtotal_box13H=0.0; 
/*
							subtotal_box14A=0.0; 
							subtotal_box14B=0.0; 
							subtotal_box13A=0.0; 
							subtotal_box13B=0.0; 
							subtotal_box13C=0.0; 
							subtotal_box13J=0.0; 
							subtotal_box13K=0.0; 
							subtotal_box13L=0.0; 
							subtotal_box13M=0.0; 
							subtotal_box13N=0.0; 
							subtotal_box13P=0.0; 
							subtotal_box13Q=0.0; 
*/
						}
					}
				}
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
			ef=ADVNXTNRDsec(sortfile->fileno,1,SCRNvirtualSubData,mainrsrc);
/*
			if(empr_id!=NULL) Rfree(empr_id);
			if(empr_idno!=NULL) Rfree(empr_idno);
			if(empr_name1!=NULL) Rfree(empr_name1);
			if(empr_name2!=NULL) Rfree(empr_name2);
			if(empr_addr1!=NULL) Rfree(empr_addr1);
			if(empr_addr2!=NULL) Rfree(empr_addr2);
			if(empr_addr3!=NULL) Rfree(empr_addr3);
*/
			if(per_id!=NULL) Rfree(per_id);
			if(ssn!=NULL) Rfree(ssn);
			if(empl_name!=NULL) Rfree(empl_name);
			if(empl_addr1!=NULL) Rfree(empl_addr1);
			if(empl_addr2!=NULL) Rfree(empl_addr2);
			if(empl_addr3!=NULL) Rfree(empl_addr3);
			if(box13A_label!=NULL) Rfree(box13A_label);
			if(box13B_label!=NULL) Rfree(box13B_label);
			if(box13C_label!=NULL) Rfree(box13C_label);
			if(box13D_label!=NULL) Rfree(box13D_label);
			if(box13E_label!=NULL) Rfree(box13E_label);
			if(box13F_label!=NULL) Rfree(box13F_label);
			if(box13G_label!=NULL) Rfree(box13G_label);
			if(box13H_label!=NULL) Rfree(box13H_label);
			if(box13J_label!=NULL) Rfree(box13J_label);
			if(box13K_label!=NULL) Rfree(box13K_label);
			if(box13L_label!=NULL) Rfree(box13L_label);
			if(box13M_label!=NULL) Rfree(box13M_label);
			if(box13N_label!=NULL) Rfree(box13N_label);
			if(box13P_label!=NULL) Rfree(box13P_label);
			if(box13Q_label!=NULL) Rfree(box13Q_label);
			if(box14A_label!=NULL) Rfree(box14A_label);
			if(box14B_label!=NULL) Rfree(box14B_label);
/*
			if(subtotal_box14A_label!=NULL) Rfree(subtotal_box14A_label); 
			if(subtotal_box14B_label!=NULL) Rfree(subtotal_box14B_label); 
*/
			if(st1_label!=NULL) Rfree(st1_label);
			if(st1_ein!=NULL) Rfree(st1_ein);
			if(st2_label!=NULL) Rfree(st2_label);
			if(st2_ein!=NULL) Rfree(st2_ein);
			if(loc1_label!=NULL) Rfree(loc1_label);
			if(loc2_label!=NULL) Rfree(loc2_label);
		}
		if(mod_value!=0) 
		{
			if(non_std==TRUE)
			{
				fprintf(fp," %-68s\r\n"," ");
				fprintf(fp," %-68s\r\n"," ");
			}
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-32s  %14.02f  %13.02f%-5s",empr_idno,subtotal_box1/100,subtotal_box2/100," ");
			fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",empr_idno,subtotal_box1/100,subtotal_box2/100);
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-32s  %14.02f  %13.02f%-5s",empr_name1,subtotal_box3/100,subtotal_box4/100," ");
			fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",empr_name1,subtotal_box3/100,subtotal_box4/100);
			fprintf(fp," %-32s%-36s",empr_name2," ");
			fprintf(fp,"%-32s\r\n",empr_name2);
			fprintf(fp," %-32s  %14.02f  %13.02f%-5s",empr_addr1,subtotal_box5/100,subtotal_box6/100," ");
			fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",empr_addr1,subtotal_box5/100,subtotal_box6/100);
			fprintf(fp," %-32s%-36s",empr_addr2," ");
			fprintf(fp,"%-32s\r\n",empr_addr2);
			fprintf(fp," %-32s  %14.02f  %13.02f%-5s",(empr_addr3!=NULL?empr_addr3:""),subtotal_box7/100,subtotal_box8/100," ");
			fprintf(fp,"%-32s  %14.02f  %13.02f\r\n",(empr_addr3!=NULL?empr_addr3:""),subtotal_box7/100,subtotal_box8/100);
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-32s  %14.02f  %13.02f%-5s"," ",subtotal_box9/100,subtotal_box10/100," ");
			fprintf(fp,"%-32s  %14.02f  %13.02f\r\n"," ",subtotal_box9/100,subtotal_box10/100);
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-32s  %14.02f  %13.02f%-5s"," ",((subtotal_box11A+subtotal_box11B)/100),subtotal_box12/100," ");
			fprintf(fp,"%-32s  %14.02f  %13.02f\r\n"," ",((subtotal_box11A+subtotal_box11B)/100),subtotal_box12/100);
			fprintf(fp," %-68s\r\n"," ");
			if((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)!=0.00)
			{
				fprintf(fp," %-32s   %-5s%-28s","","DEFGH"," ");
				fprintf(fp,"%-32s   %-5s\r\n","","DEFGH");
			} else {
				fprintf(fp," %-68s\r\n"," ");
			}
			if((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)!=0.00)
			{
				fprintf(fp," %-32s   %13.02f%-20s","",((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)/100)," ");
				fprintf(fp,"%-32s   %13.02f\r\n","",((subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H)/100));
			} else {
				fprintf(fp," %-68s\r\n"," ");
			}
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp,"                                                        %c%-12s",'X'," ");
			fprintf(fp,"                                                       %c\r\n",'X');
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			fprintf(fp," %-68s\r\n"," ");
			if(non_std==FALSE)
			{
				fprintf(fp," %-68s\r\n"," ");
				fprintf(fp," %-68s\r\n"," ");
			}
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	if(empr_id!=NULL) Rfree(empr_id);
	if(empr_idno!=NULL) Rfree(empr_idno);
	if(empr_name1!=NULL) Rfree(empr_name1);
	if(empr_name2!=NULL) Rfree(empr_name2);
	if(empr_addr1!=NULL) Rfree(empr_addr1);
	if(empr_addr2!=NULL) Rfree(empr_addr2);
	if(empr_addr3!=NULL) Rfree(empr_addr3);
	if(empr_city!=NULL) Rfree(empr_city);
	if(empr_state!=NULL) Rfree(empr_state);
	if(empr_zip!=NULL) Rfree(empr_zip);
	if(per_id!=NULL) Rfree(per_id);
	if(ssn!=NULL) Rfree(ssn);
	if(empl_name!=NULL) Rfree(empl_name);
	if(empl_addr1!=NULL) Rfree(empl_addr1);
	if(empl_addr2!=NULL) Rfree(empl_addr2);
	if(empl_addr3!=NULL) Rfree(empl_addr3);
	if(empl_city!=NULL) Rfree(empl_city);
	if(empl_state!=NULL) Rfree(empl_state);
	if(empl_zip!=NULL) Rfree(empl_zip);
	if(box13A_label!=NULL) Rfree(box13A_label);
	if(box13B_label!=NULL) Rfree(box13B_label);
	if(box13C_label!=NULL) Rfree(box13C_label);
	if(box13D_label!=NULL) Rfree(box13D_label);
	if(box13E_label!=NULL) Rfree(box13E_label);
	if(box13F_label!=NULL) Rfree(box13F_label);
	if(box13G_label!=NULL) Rfree(box13G_label);
	if(box13H_label!=NULL) Rfree(box13H_label);
	if(box13J_label!=NULL) Rfree(box13J_label);
	if(box13K_label!=NULL) Rfree(box13K_label);
	if(box13L_label!=NULL) Rfree(box13L_label);
	if(box13M_label!=NULL) Rfree(box13M_label);
	if(box13N_label!=NULL) Rfree(box13N_label);
	if(box13P_label!=NULL) Rfree(box13P_label);
	if(box13Q_label!=NULL) Rfree(box13Q_label);
	if(box14A_label!=NULL) Rfree(box14A_label);
	if(box14B_label!=NULL) Rfree(box14B_label);
}
int main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	int cal_year=0;

	if(InitializeSubsystems(argc,argv,module,"PRINT W2 SIDE-BY-SIDE")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((payw2=APPOPNNRD(module,"PAYW2",TRUE,FALSE))==(-1)) return;
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payempr=APPOPNNRD(module,"PAYEMPR",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"PRINT W2 SIDE-BY-SIDE");
	addDFincvir(mainrsrc,module,"PAYW2",NULL,payw2);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mainrsrc,module,"PAYEMPR",NULL,payempr);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&cal_year,TRUE);
	addstdrsrc(mainrsrc,"NON STANDARD",BOOLNS,1,&non_std,TRUE);
	if(payw2!=(-1)) file2rangersrc(payw2,mainrsrc);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,printw2,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
