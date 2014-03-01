/* payw2mm.c - Create Magnetic Media for W2's */
#ifndef WIN32
#define __NAM__ "payw2mm.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payw2mm.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
#include <pay.hpp>
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
	RDA_PFILE *RDA_fp;
};
typedef struct PassableStructs PassableStruct;

/*
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

char MakeSRecords=FALSE;
char *module="PAYROLL",*w2_expression=NULL;
short perdmg=(-1),payw2=(-1),payempr=(-1);
RDAvirtual *csortsequence=NULL;
static APPlib *name_code_list=NULL;
double body_count=41;
/*
double body_count=11;
*/
static void makew2mm(RDArsrc *);
static void makew2mmfunc(RDArsrc *,PassableStruct *,short);
static RDApayroll *PAYROLL_SETUP=NULL;

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
*/
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
	if(name_code_list!=NULL) freeapplib(name_code_list);
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
			if(!RDAstrcmp(field->name,"CONTROL NUMBER"))
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
			{
				if(!RDAstrcmp(part->name,"EMPLOYER IDENTIFICATION"))
				{
					addDBfield(dbsrt,field->name,field->type,field->len);
					addDBfield(dbsrt,"W2 TYPE",SHORTV,2);
				} else {
					addDBfield(dbsrt,field->name,field->type,field->len);
				}
			}
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
static void print_report(RDArsrc *prtrsrc,PassableStruct *p)
{
	char *message=NULL,*outdevice=NULL;
	RDA_PFILE *fp=NULL;

	readwidget(prtrsrc,"DEVICE");
	FINDRSCGETSTRING(prtrsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			p->fp=NULL;
			p->RDA_fp=fp;
			makew2mmfunc(prtrsrc,p,TRUE);
			RDA_pclose(fp);
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
	char *header=NULL,*message=NULL;
	char *filename="W2REPORT";

	if((fp=fopen(filename,"w"))==NULL)
	{
		header=stralloc("FILE OPEN FAILED");
		message=Rmalloc(RDAstrlen(filename)+38);
		sprintf(message,"The fopen function failed on file [%s]!",filename);
		prterr("Error Can't fopen file passed as a parameter.");
		if(message!=NULL) Rfree(message); 
		if(header!=NULL) Rfree(header); 
		return;
	} else if(fp!=NULL)
	{
		p->fp=fp;
		p->RDA_fp=NULL;
		makew2mmfunc(prtrsrc,p,FALSE);
		fclose(fp);
	} else {
		message=Rmalloc(RDAstrlen(filename)+75);
		message=Rmalloc(RDAstrlen(filename)+75);
		sprintf(message,"The output attemp to the ASCII file [%s] failed when trying to create file.",filename);
		ERRORDIALOG("FILE OPEN FAILED!",message,NULL,FALSE);
		prterr(message);
		if(message!=NULL) Rfree(message); 
		return;
	}
}
static void makew2mm(RDArsrc *mainrsrc)
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
	FINDRSCGETCHAR(mainrsrc,"MAKE S RECORDS",&MakeSRecords);
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
			if(XPERT_SETUP->software_type==2)
			{
				FINDFLDSETSTRING(perdmg,"EMPLOYEE ID",per_id);
			} else {
				FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",per_id);
			}
			ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc);
/*
			if(!ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(perdmg,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
*/
					FINDFLDGETSTRING(payw2,"EMPLOYER IDENTIFICATION",&empr_id);
					FINDFLDSETSTRING(payempr,"PERSONNEL IDENTIFICATION",per_id);
					FINDFLDSETSTRING(payempr,"EMPLOYER IDENTIFICATION",empr_id);
					if(!ADVEQLNRDsec(payempr,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDGETCHAR(payempr,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							if(!testfilerangersrc(payw2,rs) && 
								!testfilerangersrc(payempr,rs) 
								&& !testfilerangersrc(perdmg,rs))  
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
/*
				}
			}
*/
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		ef=ADVNXTNRDsec(payw2,payw2_keyno,SCRNvirtualSubData,mainrsrc);
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

	prsrc=RDArsrcNEW("PAYROLL","MAKE W2 DISK DEVICE");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addbtnrsrc(prsrc,"PRINT REPORT",TRUE,print_report,p);
	addbtnrsrc(prsrc,"MAKE DISK",TRUE,print,p);
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
static char *formatamt(double amount,int len,char padchar)
{
	int x,y,length;
	char *ret_string, *val_string;

	sprintf(stemp,"%f",amount);
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	val_string=Rmalloc(len+1);
	for(x=length-1;x>-1;x--)
	{
		if(ret_string[x]=='0')
		{
			ret_string[x]=0;
		} else if (ret_string[x]=='.')
		{
			ret_string[x]=0;
			break;
		} else break;
	}
	length=RDAstrlen(ret_string);
	y=(len-length);
	x=0;
	if(y>-1) for(x=0;x<y;++x) val_string[x]=padchar;
	for(x=y,y=0;x<len;++x,++y)
	{
		val_string[x]=ret_string[y];
	}
	if(ret_string!=NULL) Rfree(ret_string);
	val_string[len]=0;
	return(val_string);
}
static char *getstnum(char *st_label)
{
	char *st_number=NULL;

	if(!RDAstrcmp(st_label,"AL")) st_number=stralloc("01");
	else if(!RDAstrcmp(st_label,"AK")) st_number=stralloc("02");
	else if(!RDAstrcmp(st_label,"AZ")) st_number=stralloc("04");
	else if(!RDAstrcmp(st_label,"AR")) st_number=stralloc("05");
	else if(!RDAstrcmp(st_label,"CA")) st_number=stralloc("06");
	else if(!RDAstrcmp(st_label,"CO")) st_number=stralloc("08");
	else if(!RDAstrcmp(st_label,"CT")) st_number=stralloc("09");
	else if(!RDAstrcmp(st_label,"DE")) st_number=stralloc("10");
	else if(!RDAstrcmp(st_label,"DC")) st_number=stralloc("11");
	else if(!RDAstrcmp(st_label,"FL")) st_number=stralloc("12");
	else if(!RDAstrcmp(st_label,"GA")) st_number=stralloc("13");
	else if(!RDAstrcmp(st_label,"HI")) st_number=stralloc("15");
	else if(!RDAstrcmp(st_label,"ID")) st_number=stralloc("16");
	else if(!RDAstrcmp(st_label,"IL")) st_number=stralloc("17");
	else if(!RDAstrcmp(st_label,"IN")) st_number=stralloc("18");
	else if(!RDAstrcmp(st_label,"IA")) st_number=stralloc("19");
	else if(!RDAstrcmp(st_label,"KS")) st_number=stralloc("20");
	else if(!RDAstrcmp(st_label,"KY")) st_number=stralloc("21");
	else if(!RDAstrcmp(st_label,"LA")) st_number=stralloc("22");
	else if(!RDAstrcmp(st_label,"ME")) st_number=stralloc("23");
	else if(!RDAstrcmp(st_label,"MD")) st_number=stralloc("24");
	else if(!RDAstrcmp(st_label,"MA")) st_number=stralloc("25");
	else if(!RDAstrcmp(st_label,"MI")) st_number=stralloc("26");
	else if(!RDAstrcmp(st_label,"MN")) st_number=stralloc("27");
	else if(!RDAstrcmp(st_label,"MS")) st_number=stralloc("28");
	else if(!RDAstrcmp(st_label,"MO")) st_number=stralloc("29");
	else if(!RDAstrcmp(st_label,"MT")) st_number=stralloc("30");
	else if(!RDAstrcmp(st_label,"NE")) st_number=stralloc("31");
	else if(!RDAstrcmp(st_label,"NV")) st_number=stralloc("32");
	else if(!RDAstrcmp(st_label,"NH")) st_number=stralloc("33");
	else if(!RDAstrcmp(st_label,"NJ")) st_number=stralloc("34");
	else if(!RDAstrcmp(st_label,"NM")) st_number=stralloc("35");
	else if(!RDAstrcmp(st_label,"NY")) st_number=stralloc("36");
	else if(!RDAstrcmp(st_label,"NC")) st_number=stralloc("37");
	else if(!RDAstrcmp(st_label,"ND")) st_number=stralloc("38");
	else if(!RDAstrcmp(st_label,"OH")) st_number=stralloc("39");
	else if(!RDAstrcmp(st_label,"OK")) st_number=stralloc("40");
	else if(!RDAstrcmp(st_label,"OR")) st_number=stralloc("41");
	else if(!RDAstrcmp(st_label,"PA")) st_number=stralloc("42");
	else if(!RDAstrcmp(st_label,"RI")) st_number=stralloc("44");
	else if(!RDAstrcmp(st_label,"SC")) st_number=stralloc("45");
	else if(!RDAstrcmp(st_label,"SD")) st_number=stralloc("46");
	else if(!RDAstrcmp(st_label,"TN")) st_number=stralloc("47");
	else if(!RDAstrcmp(st_label,"TX")) st_number=stralloc("48");
	else if(!RDAstrcmp(st_label,"UT")) st_number=stralloc("49");
	else if(!RDAstrcmp(st_label,"VT")) st_number=stralloc("50");
	else if(!RDAstrcmp(st_label,"VA")) st_number=stralloc("51");
	else if(!RDAstrcmp(st_label,"WA")) st_number=stralloc("53");
	else if(!RDAstrcmp(st_label,"WV")) st_number=stralloc("54");
	else if(!RDAstrcmp(st_label,"WI")) st_number=stralloc("55");
	else if(!RDAstrcmp(st_label,"WY")) st_number=stralloc("56");
	return(st_number);
}
static void w2employer_header(RDArsrc *prtrsrc,PassableStruct *p,int cal_yr,short w2type,int empl_name_type)
{
	FILE *fp=NULL;
	RDA_PFILE *RDA_fp=NULL;
/*
	DBsort *sortfile=NULL;
*/
	RDArsrc *mainrsrc=NULL;
	DFvirtual *d=NULL;
	int x=0,y=0;
	char *empr_idno=NULL;
	char *empr_name1=NULL,*empr_name2=NULL;
	char *empr_city=NULL,*empr_state=NULL;
	char *empr_zip4=NULL,*empr_zip=NULL;
	char *empr_addr1=NULL,*empr_addr2=NULL;
	char *w2type_str=NULL,*tmp_string=NULL;

	if(p!=NULL) 
	{
		mainrsrc=p->mainrsrc;
/*
		sortfile=p->sortfile;
*/
		fp=p->fp;
		RDA_fp=p->RDA_fp;
	}
	FINDFLDGETSTRING(payempr,"FEDERAL EIN",&empr_idno);
	if(!isEMPTY(empr_idno))
	{
		if(strstr(empr_idno,"-")!=NULL)
		{
			tmp_string=Rmalloc(RDAstrlen(empr_idno)+1);
			for(x=0,y=0;x<RDAstrlen(empr_idno);++x)
			{
				if(empr_idno[x]!='-')
				{
					tmp_string[y]=empr_idno[x];
					++y;
				}
			}
			Rfree(empr_idno);
			empr_idno=stralloc(tmp_string);
			if(tmp_string!=NULL) Rfree(tmp_string);
		}
	}
	FINDFLDGETSTRING(payempr,"EMPLOYER NAME",&empr_name1);
	if(empr_name1!=NULL) 
	{
		empr_name1=Rrealloc(empr_name1,51);
	}
	if(RDAstrlen(empr_name1)>32)
	{
		empr_name1[32]=0;
	}
	FINDFLDGETSTRING(payempr,"EMPLOYER NAME 2",&empr_name2);
	if(!isEMPTY(empr_name2))
	{
		if(RDAstrlen(empr_name2)>17)
		{
			empr_name2[17]=0;
		}
		strcat(empr_name1," ");
		strcat(empr_name1,empr_name2);
	}
	stoupper(empr_name1);
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
	if(RDAstrlen(empr_addr1)>40)
	{
		empr_addr1[40]=0;
	}
	stoupper(empr_addr1);
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
	FINDFLDGETSTRING(payempr,"CITY",&empr_city);
	if(RDAstrlen(empr_city)>25)
	{
		empr_city[25]=0;
	}
	stoupper(empr_city);
	FINDFLDGETSTRING(payempr,"STATE",&empr_state);
	if(RDAstrlen(empr_state)>2)
	{
		empr_state[2]=0;
	}
	stoupper(empr_state);
	FINDFLDGETSTRING(payempr,"ZIP",&empr_zip);
	if(empr_zip!=NULL)
	{
		empr_zip4=Rmalloc(6);
		if((RDAstrlen(empr_zip)==10) && (empr_zip[5]=='-'))
		{
			empr_zip4[0]=empr_zip[5];
			empr_zip4[1]=empr_zip[6];
			empr_zip4[2]=empr_zip[7];
			empr_zip4[3]=empr_zip[8];
			empr_zip4[4]=empr_zip[9];
		}
	}
	if(RDAstrlen(empr_zip)>5)
	{
		empr_zip[5]=0;
	}
	if(w2type==0) 
	{
		w2type_str=stralloc("R");
	} else if(w2type==1)
	{
		w2type_str=stralloc("Q");
	} else {
		w2type_str=stralloc(" ");
	}
	if(fp!=NULL)
	{
		fprintf(fp,"1E%4d%-9s%-9s%-50s%-40s%-1s%-13s\r\n",cal_yr,(empr_idno!=NULL?empr_idno:"")," ",(empr_name1!=NULL?empr_name1:""),(empr_addr1!=NULL?empr_addr1:"")," "," ");
		fprintf(fp,"2E%-25s%-2s%-8s%-5s%-5s%-1s%-1s%-2s%-4s%-1s%-1s%-9s%-62s\r\n",(empr_city!=NULL?empr_city:""),(empr_state!=NULL?empr_state:"")," ",(empr_zip4!=NULL?empr_zip4:""),(empr_zip!=NULL?empr_zip:""),(empl_name_type==0?"S":"F"),w2type_str," ","    "," "," "," ","         ");
	}
	if(RDA_fp!=NULL)
	{
		RDA_fprintf(RDA_fp,"1E%4d%-9s%-9s%-50s%-40s%-1s%-13s\r\n",cal_yr,(empr_idno!=NULL?empr_idno:"")," ",(empr_name1!=NULL?empr_name1:""),(empr_addr1!=NULL?empr_addr1:"")," "," ");
		RDA_fprintf(RDA_fp,"2E%-25s%-2s%-8s%-5s%-5s%-1s%-1s%-2s%-4s%-1s%-1s%-9s%-62s\r\n",(empr_city!=NULL?empr_city:""),(empr_state!=NULL?empr_state:"")," ",(empr_zip4!=NULL?empr_zip4:""),(empr_zip!=NULL?empr_zip:""),(empl_name_type==0?"S":"F"),w2type_str," ","    "," "," "," ","         ");
	}
	if(w2type_str!=NULL) Rfree(w2type_str);
	if(empr_idno!=NULL) Rfree(empr_idno);
	if(empr_name1!=NULL) Rfree(empr_name1);
	if(empr_name2!=NULL) Rfree(empr_name2);
	if(empr_addr1!=NULL) Rfree(empr_addr1);
	if(empr_addr2!=NULL) Rfree(empr_addr2);
	if(empr_city!=NULL) Rfree(empr_city);
	if(empr_state!=NULL) Rfree(empr_state);
	if(empr_zip4!=NULL) Rfree(empr_zip4);
	if(empr_zip!=NULL) Rfree(empr_zip);
}
static void w2employer_subfooter(RDArsrc *prtrsrc,PassableStruct *p,double box3,double box7,double box1,double box4,double box2,double control_number,double box13C,double uncol_empl_tax,double box9,double box8,double box12,double DEFGH,double box10,double box11A,double box11B,double box5,double box6,double military_pay)
{
        FILE *fp=NULL;
        RDA_PFILE *RDA_fp=NULL;
/*
        DBsort *sortfile=NULL;
        RDArsrc *mainrsrc=NULL;
*/
	char *box3_string=NULL,*box7_string=NULL,*box1_string=NULL;
	char *box4_string=NULL,*box2_string=NULL,*box13C_string=NULL;
	char *uncol_empl_string=NULL,*box9_string=NULL,*box8_string=NULL;
	char *box12_string=NULL,*DEFGH_string=NULL;
	char *box10_string=NULL,*box11A_string=NULL,*box11B_string=NULL;
	char *box5_string=NULL,*box6_string=NULL,*military_pay_string=NULL;
	char *control_number_string=NULL;

        if(p!=NULL)
        {
/*
                mainrsrc=p->mainrsrc;
                sortfile=p->sortfile;
*/
                fp=p->fp;
                RDA_fp=p->RDA_fp;
        }
	box3_string=formatamt(box3,10,'0');
	box7_string=formatamt(box7,10,'0');
	box1_string=formatamt(box1,10,'0');
	box4_string=formatamt(box4,10,'0');
	box2_string=formatamt(box2,10,'0');
	control_number_string=formatamt(control_number,7,'0');
	uncol_empl_string=formatamt(uncol_empl_tax,10,'0');
	box9_string=formatamt(box9,11,'0');
	box8_string=formatamt(box8,10,'0');
	box12_string=formatamt(box12,10,'0');
	box13C_string=formatamt(box13C,10,'0');
	DEFGH_string=formatamt(DEFGH,10,'0');
	if(fp!=NULL)
	{
		fprintf(fp,"1I%-10s%-1s%-10s%-1s%-10s%-1s%-10s%-1s%-10s%-7s%-10s%-10s%-11s%-10s%-10s%-1s%-10s%-3s\r\n",box3_string," ",box7_string," ",box1_string," ",box4_string," ",box2_string,control_number_string,box13C_string,uncol_empl_string,box9_string,box8_string,box12_string," ",DEFGH_string,"   ");
	}
	if(RDA_fp!=NULL)
	{
		RDA_fprintf(RDA_fp,"1I%-10s%-1s%-10s%-1s%-10s%-1s%-10s%-1s%-10s%-7s%-10s%-10s%-11s%-10s%-10s%-1s%-10s%-3s\r\n",box3_string," ",box7_string," ",box1_string," ",box4_string," ",box2_string,control_number_string,box13C_string,uncol_empl_string,box9_string,box8_string,box12_string," ",DEFGH_string,"   ");
	}
	box10_string=formatamt(box10,10,'0');
	box11A_string=formatamt(box11A,10,'0');
	box11B_string=formatamt(box11B,10,'0');
	box5_string=formatamt(box5,11,'0');
	box6_string=formatamt(box6,10,'0');
	military_pay_string=formatamt(military_pay,12,'0');
	if(fp!=NULL)
	{
		fprintf(fp,"2I%-10s%-1s%-10s%-1s%-10s%-1s%-11s%-1s%-10s%-1s%-12s%-58s\r\n",box10_string," ",box11A_string," ",box11B_string," ",box5_string," ",box6_string," ",military_pay_string," ");
	}
	if(RDA_fp!=NULL)
	{
		RDA_fprintf(RDA_fp,"2I%-10s%-1s%-10s%-1s%-10s%-1s%-11s%-1s%-10s%-1s%-12s%-58s\r\n",box10_string," ",box11A_string," ",box11B_string," ",box5_string," ",box6_string," ",military_pay_string," ");
	}
	if(box3_string!=NULL) Rfree(box3_string);
	if(box7_string!=NULL) Rfree(box7_string);
	if(box1_string!=NULL) Rfree(box1_string);
	if(box4_string!=NULL) Rfree(box4_string);
	if(box2_string!=NULL) Rfree(box2_string);
	if(control_number_string!=NULL) Rfree(control_number_string);
	if(box13C_string!=NULL) Rfree(box13C_string);
	if(uncol_empl_string!=NULL) Rfree(uncol_empl_string);
	if(box9_string!=NULL) Rfree(box9_string);
	if(box8_string!=NULL) Rfree(box8_string);
	if(box12_string!=NULL) Rfree(box12_string);
	if(DEFGH_string!=NULL) Rfree(DEFGH_string);
	if(box10_string!=NULL) Rfree(box10_string);
	if(box11A_string!=NULL) Rfree(box11A_string);
	if(box11B_string!=NULL) Rfree(box11B_string);
	if(box5_string!=NULL) Rfree(box5_string);
	if(box6_string!=NULL) Rfree(box6_string);
	if(military_pay_string!=NULL) Rfree(military_pay_string);
}
static void w2employer_footer(RDArsrc *prtrsrc,PassableStruct *p,double number_empl,double box3,double box7,double box1,double box4,double box2,double box13C,double uncol_empl_tax,double box9,double box8,double box12,double DEFGH,double box10,double box11A,double box11B,double box5,double box6,double trdparty,double military_pay,short report)
{
        FILE *fp=NULL;
        RDA_PFILE *RDA_fp=NULL;
/*
        DBsort *sortfile=NULL;
        RDArsrc *mainrsrc=NULL;
*/
	char *box3_string=NULL,*box7_string=NULL,*box1_string=NULL;
	char *box4_string=NULL,*box2_string=NULL,*box13C_string=NULL;
	char *uncol_empl_string=NULL,*box9_string=NULL,*box8_string=NULL;
	char *trdparty_string=NULL;
	char *box12_string=NULL,*DEFGH_string=NULL;
	char *box10_string=NULL,*box11A_string=NULL,*box11B_string=NULL;
	char *box5_string=NULL,*box6_string=NULL,*military_pay_string=NULL;
	char *number_empl_string=NULL;

        if(p!=NULL)
        {
/*
                mainrsrc=p->mainrsrc;
                sortfile=p->sortfile;
*/
                fp=p->fp;
                RDA_fp=p->RDA_fp;
        }
	number_empl_string=formatamt(number_empl,7,'0');
	box3_string=formatamt(box3,13,'0');
	box7_string=formatamt(box7,12,'0');
	box1_string=formatamt(box1,13,'0');
	box4_string=formatamt(box4,12,'0');
	box2_string=formatamt(box2,12,'0');
	box13C_string=formatamt(box13C,12,'0');
	uncol_empl_string=formatamt(uncol_empl_tax,12,'0');
	box9_string=formatamt(box9,12,'0');
	box8_string=formatamt(box8,12,'0');
	if(fp!=NULL)
	{
		fprintf(fp,"1T%-7s%-13s%-1s%-12s%-13s%-1s%-12s%-1s%-12s%-12s%-12s%-12s%-12s%-6s\r\n",number_empl_string,box3_string," ",box7_string,box1_string," ",box4_string," ",box2_string,box13C_string,uncol_empl_string,box9_string,box8_string,"      ");
	} 
	if(RDA_fp!=NULL)
	{
		RDA_fprintf(RDA_fp,"1T%-7s%-13s%-1s%-12s%-13s%-1s%-12s%-1s%-12s%-12s%-12s%-12s%-12s%-6s\r\n",number_empl_string,box3_string," ",box7_string,box1_string," ",box4_string," ",box2_string,box13C_string,uncol_empl_string,box9_string,box8_string,"      ");
	}
	box12_string=formatamt(box12,12,'0');
	DEFGH_string=formatamt(DEFGH,13,'0');
	box10_string=formatamt(box10,12,'0');
	box11A_string=formatamt(box11A,13,'0');
	box11B_string=formatamt(box11B,13,'0');
	box5_string=formatamt(box5,13,'0');
	box6_string=formatamt(box6,12,'0');
	trdparty_string=formatamt(trdparty,12,'0');
	military_pay_string=formatamt(military_pay,12,'0');
	if(fp!=NULL)
	{
		fprintf(fp,"2T%-12s%-1s%-13s%-1s%-12s%-1s%-13s%-1s%-13s%-1s%-13s%-1s%-12s%-1s%-12s%-1s%-12s%-6s\r\n",box12_string," ",DEFGH_string," ",box10_string," ",box11A_string," ",box11B_string," ",box5_string," ",box6_string," ",trdparty_string," ",military_pay_string," ");
	}
	if(RDA_fp!=NULL)
	{
		RDA_fprintf(RDA_fp,"2T%-12s%-1s%-13s%-1s%-12s%-1s%-13s%-1s%-13s%-1s%-13s%-1s%-12s%-1s%-12s%-1s%-12s%-6s\r\n",box12_string," ",DEFGH_string," ",box10_string," ",box11A_string," ",box11B_string," ",box5_string," ",box6_string," ",trdparty_string," ",military_pay_string," ");
	}
	if(report==TRUE)
	{
		if(fp!=NULL)
		{
			fprintf(fp,"NO. EMPLOYEES=%7.0f  FICA WAGES=%13.02f  FED WAGES=%13.02f  FICA W/H=%13.02f  FED W/H=%13.02f  INS=%10.02f  EIC=%10.02f\r\n",(number_empl),(box3/100),(box1/100),(box4/100),(box2/100),(box13C/100),(box9/100));
			fprintf(fp,"FRG=%10.02f  DEFERRED=%10.02f  DEP=%13.02f  NQ457=%10.02f  NQ non 457=%10.02f\r\n",(box12/100),(DEFGH/100),(box10/100),(box11A/100),(box11B/100));
			fprintf(fp,"MEDICARE W/H=%13.02f  MEDICARE W/H=%13.02f  3RD PARTY FEDERAL W/H=%13.02f\r\n",(box5/100),(box6/100),(trdparty/100));
		}
		if(RDA_fp!=NULL)
		{
			RDA_fprintf(RDA_fp,"NO. EMPLOYEES=%7.0f  FICA WAGES=%13.02f  FED WAGES=%13.02f  FICA W/H=%13.02f  FED W/H=%13.02f  INS=%10.02f  EIC=%10.02f\r\n",(number_empl),(box3/100),(box1/100),(box4/100),(box2/100),(box13C/100),(box9/100));
			RDA_fprintf(RDA_fp,"FRG=%10.02f  DEFERRED=%10.02f  DEP=%13.02f  NQ457=%10.02f  NQ non 457=%10.02f\r\n",(box12/100),(DEFGH/100),(box10/100),(box11A/100),(box11B/100));
			RDA_fprintf(RDA_fp,"MEDICARE W/H=%13.02f  MEDICARE W/H=%13.02f  3RD PARTY FEDERAL W/H=%13.02f\r\n",(box5/100),(box6/100),(trdparty/100));
		}
	}
	if(number_empl_string!=NULL) Rfree(number_empl_string);
	if(box3_string!=NULL) Rfree(box3_string);
	if(box7_string!=NULL) Rfree(box7_string);
	if(box1_string!=NULL) Rfree(box1_string);
	if(box4_string!=NULL) Rfree(box4_string);
	if(box2_string!=NULL) Rfree(box2_string);
	if(box13C_string!=NULL) Rfree(box13C_string);
	if(uncol_empl_string!=NULL) Rfree(uncol_empl_string);
	if(box9_string!=NULL) Rfree(box9_string);
	if(box8_string!=NULL) Rfree(box8_string);
	if(box12_string!=NULL) Rfree(box12_string);
	if(DEFGH_string!=NULL) Rfree(DEFGH_string);
	if(box10_string!=NULL) Rfree(box10_string);
	if(box11A_string!=NULL) Rfree(box11A_string);
	if(box11B_string!=NULL) Rfree(box11B_string);
	if(box5_string!=NULL) Rfree(box5_string);
	if(box6_string!=NULL) Rfree(box6_string);
	if(trdparty_string!=NULL) Rfree(trdparty_string);
	if(military_pay_string!=NULL) Rfree(military_pay_string);
}
static void makew2mmfunc(RDArsrc *prtrsrc,PassableStruct *p,short report)
{
	FILE *fp=NULL;
	RDA_PFILE *RDA_fp=NULL;
	DBsort *sortfile=NULL;
	RDArsrc *mainrsrc=NULL;
	short ef=0,increment_diagnostic=TRUE,payw2_keyno=1;
	int cal_yr=0,cont_no=0;
	double cont_no_double=0.0;
	char *cont_no_string=NULL;
	int sizeof_sort=0;
	RDArsrc *tmprsrc=NULL;
	DFvirtual *d=NULL;
	char *e=NULL;
	double w2_subcounter=0.0; 
	double mod_value=0.0; 
	char *empr_id=NULL,*last_emprid=NULL,*per_id=NULL;
	short w2type=0,last_w2type=0;
/*
	Box13Struct *BOX13=NULL;
	Box13Item *b=NULL;
*/
	char *ssn=NULL,*tmp_string=NULL;
	char statutory=' ',deceased=FALSE,penplan=' ',legrep=' ',hsldemp=' ';
	char defcomp=' ';
	int x=0,y=0;
	char *empl_name=NULL,*empl_addr1=NULL;
	char *new_empl_name=NULL;
	char *empl_addr2=NULL,*empl_addr3=NULL;
	char *empl_city=NULL,*empl_state=NULL,*empl_zip=NULL;
	char *empl_zip4=NULL;
	char *tran_ein=NULL,*tran_name=NULL,*tran_addr1=NULL;
	char *tran_city=NULL,*tran_state=NULL,*tran_zip=NULL,*tran_zip4=NULL;
	char *computer=NULL,*state_number_code=NULL;
	char *st1_label=NULL,*st1_ein=NULL,*loc1_label=NULL;
	double st1_gross=0.0,st1_amount=0.0,loc1_gross=0.0,loc1_amount=0.0;
	char *strptperiod=NULL,*stempdate=NULL;
	char *st1_gross_string=NULL,*st1_amount_string=NULL;
	char *loc1_gross_string=NULL,*loc1_amount_string=NULL;

	double box1=0.0,box2=0.0,box3=0.0; 
	double box4=0.0,box5=0.0,box6=0.0; 
	double box7=0.0,box8=0.0,box9=0.0; 
	double box10=0.0;
	double box11A=0.0,box11B=0.0,box12=0.0; 
	double box13C=0.0; 
	double box13D=0.0,box13E=0.0,box13F=0.0; 
	double box13G=0.0,box13H=0.0;
	double box13DEFGH=0.0;
/*
	double box13A=0.0,box13B=0.0;
	double box13J=0.0; 
	double box13K=0.0,box13L=0.0,box13M=0.0; 
	double box13N=0.0,box13P=0.0,box13Q=0.0; 
	double box14A=0.0,box14B=0.0; 
*/
	double trdparty=0.0;
/*
	char *box13A_label=NULL,*box13B_label=NULL,*box13C_label=NULL; 
	char *box13D_label=NULL,*box13E_label=NULL,*box13F_label=NULL; 
	char *box13G_label=NULL,*box13H_label=NULL,*box13J_label=NULL; 
	char *box13K_label=NULL,*box13L_label=NULL,*box13M_label=NULL; 
	char *box13N_label=NULL,*box13P_label=NULL,*box13Q_label=NULL; 
	char *box14A_label=NULL,*box14B_label=NULL; 
*/
	char *box3_string=NULL,*box7_string=NULL,*box1_string=NULL; 
	char *box4_string=NULL,*box2_string=NULL,*box11A_string=NULL; 
	char *box11B_string=NULL,*box13C_string=NULL,*box9_string=NULL; 
	char *box8_string=NULL,*box12_string=NULL,*box13DEFGH_string=NULL; 
	char *box10_string=NULL,*box5_string=NULL,*box6_string=NULL; 

	double subtotal_box1=0.0,subtotal_box2=0.0,subtotal_box3=0.0; 
	double subtotal_box4=0.0,subtotal_box5=0.0,subtotal_box6=0.0; 
	double subtotal_box7=0.0,subtotal_box8=0.0,subtotal_box9=0.0; 
	double subtotal_box10=0.0;
	double subtotal_box11A=0.0,subtotal_box11B=0.0,subtotal_box12=0.0; 
	double subtotal_box13C=0.0; 
	double subtotal_box13D=0.0,subtotal_box13E=0.0,subtotal_box13F=0.0; 
	double subtotal_box13G=0.0,subtotal_box13H=0.0; 

	double empr_total_box1=0.0,empr_total_box2=0.0,empr_total_box3=0.0; 
	double empr_total_box4=0.0,empr_total_box5=0.0,empr_total_box6=0.0; 
	double empr_total_box7=0.0,empr_total_box8=0.0,empr_total_box9=0.0; 
	double empr_total_box10=0.0;
	double empr_total_box11A=0.0,empr_total_box11B=0.0,empr_total_box12=0.0; 
	double empr_total_box13C=0.0; 
	double empr_total_box13D=0.0,empr_total_box13E=0.0,empr_total_box13F=0.0; 
	double empr_total_box13G=0.0,empr_total_box13H=0.0;
	double w2_subempr_counter=0.0;
	double w2_empr_counter=0.0;
	double w2_total_counter=0.0;
	char *w2_total_counter_string=NULL;
	char do_empr_footer=FALSE,do_empr_header=FALSE;
	double empr_total_trdparty=0.0;
	double total_box3=0.0,total_box7=0.0;
	double total_box4=0.0,total_box2=0.0;
	double total_box9=0.0,total_box1=0.0;
	char *total_box3_string=NULL,*total_box7_string=NULL;
	char *total_box4_string=NULL,*total_box2_string=NULL;
	char *total_box9_string=NULL,*total_box1_string=NULL;
	int empl_name_type=0;
	char *temp1=NULL;
	char *libname=NULL;
	RDAGenericSetup *g=NULL;

	if(p!=NULL) 
	{
		mainrsrc=p->mainrsrc;
		sortfile=p->sortfile;
		fp=p->fp;
		RDA_fp=p->RDA_fp;
	}
	sizeof_sort=SIZNRD(sortfile->fileno);
	if(sizeof_sort>0)
	{	
		if(diagapps)
		{
			SEENRDRECORD(sortfile->fileno);TRACE;
		}
		FINDRSCGETINT(mainrsrc,"EMPLOYEE NAME CODES",&empl_name_type);
		FINDRSCGETINT(mainrsrc,"CALENDAR YEAR",&cal_yr);
		if(XPERT_SETUP->software_type==2)
		{ 
			g=RDAGenericSetupNew("PAYROLL","EIN NUMBER");
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+10);
#ifndef WIN32
			sprintf(libname,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
			sprintf(libname,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
			if(!getRDAGenericSetupbin(libname,g))
			{
				switch(g->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case BOOLNS:
					case CHARACTERS:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						tran_ein=stralloc(g->value.string_value);
						break;
					default:
						if(g->type<28)
						{
							tran_ein=NULL;
							temp1=Rmalloc(RDAstrlen(g->name)+RDAstrlen(fieldtypedesc[g->type])+100);
							sprintf(temp1,"GENERIC SETUP VARIABLE ERROR: Field Type [%d][%s] invalid for Setup Variable [%s][%s].",g->type,fieldtypedesc[g->type],"PAYROLL",(g->name!=NULL ? g->name:""));
						} else {
							tran_ein=NULL;
							temp1=Rmalloc(RDAstrlen(g->name)+100);
							sprintf(temp1,"GENERIC SETUP VARIABLE ERROR: Field Type [%d] invalid for Setup Variable [%s][%s].",g->type,"PAYROLL",(g->name!=NULL ? g->name:""));
						}
						prterr(temp1);
						if(temp1!=NULL) Rfree(temp1);
						break;
				}
			}
			if(g!=NULL) FreeRDAGenericSetup(g);
			if(libname!=NULL) Rfree(libname);
		} else {
			PAYROLL_SETUP=RDApayrollNEW();
			getpayrollbin(PAYROLL_SETUP);
			tran_ein=stralloc(PAYROLL_SETUP->ein);
		}
		if(!isEMPTY(tran_ein))
		{
			if(RDAstrstr(tran_ein,"-")!=NULL)
			{
				tmp_string=Rmalloc(RDAstrlen(tran_ein)+1);
				for(x=0,y=0;x<RDAstrlen(tran_ein);++x)
				{
					if(tran_ein[x]!='-')
					{
						tmp_string[y]=tran_ein[x];
						++y;
					}
				}
				Rfree(tran_ein);
				tran_ein=stralloc(tmp_string);
				if(tmp_string!=NULL) Rfree(tmp_string);
			}
		}
		if(RDAstrlen(tran_ein)>9)
		{
			tran_ein[9]=0;
		}
		stoupper(tran_ein);
		FINDRSCGETSTRING(mainrsrc,"TRANSMITTER NAME",&tran_name);
		if(RDAstrlen(tran_name)>50)
		{
			tran_name[50]=0;
		}
		stoupper(tran_name);
		tran_addr1=stralloc(XPERT_SETUP->addr1);
		if(RDAstrlen(tran_addr1)>40)
		{
			tran_addr1[40]=0;
		}
		stoupper(tran_addr1);
		if(fp!=NULL)
		{
			fprintf(fp,"1A%4d%-9s%-8s%-1s%-50s%-40s%-14s\r\n",cal_yr,(tran_ein!=NULL?tran_ein:"")," "," ",(tran_name!=NULL?tran_name:""),(tran_addr1!=NULL?tran_addr1:"")," ");
		}
		if(RDA_fp!=NULL)
		{
			RDA_fprintf(RDA_fp,"1A%4d%-9s%-8s%-1s%-50s%-40s%-14s\r\n",cal_yr,(tran_ein!=NULL?tran_ein:"")," "," ",(tran_name!=NULL?tran_name:""),(tran_addr1!=NULL?tran_addr1:"")," ");
		}
	
		tran_city=stralloc(XPERT_SETUP->city);
		if(RDAstrlen(tran_city)>25)
		{
			tran_city[25]=0;
		}
		stoupper(tran_city);
		tran_state=stralloc(XPERT_SETUP->state);
		if(RDAstrlen(tran_state)>2)
		{
			tran_state[2]=0;
		}
		stoupper(tran_state);
		tran_zip=stralloc(XPERT_SETUP->zip);
		if(tran_zip!=NULL)
		{
			tran_zip4=Rmalloc(6);
			if((RDAstrlen(tran_zip)==10) && (tran_zip[5]=='-'))
			{
				tran_zip4[0]=tran_zip[5];
				tran_zip4[1]=tran_zip[6];
				tran_zip4[2]=tran_zip[7];
				tran_zip4[3]=tran_zip[8];
				tran_zip4[4]=tran_zip[9];
			}
		}
		if(RDAstrlen(tran_zip)>5)
		{
			tran_zip[5]=0;
		}
		if(fp!=NULL)
		{
			fprintf(fp,"2A%-25s%-2s%-8s%-5s%-5s%-81s\r\n",(tran_city!=NULL?tran_city:""),(tran_state!=NULL?tran_state:"")," ",(tran_zip4!=NULL?tran_zip4:""),(tran_zip!=NULL?tran_zip:"")," ");
		}
		if(RDA_fp!=NULL)
		{
			RDA_fprintf(RDA_fp,"2A%-25s%-2s%-8s%-5s%-5s%-81s\r\n",(tran_city!=NULL?tran_city:""),(tran_state!=NULL?tran_state:"")," ",(tran_zip4!=NULL?tran_zip4:""),(tran_zip!=NULL?tran_zip:"")," ");
		}

		FINDRSCGETSTRING(mainrsrc,"COMPUTER TYPE",&computer);
		if(fp!=NULL)
		{
			fprintf(fp,"1B%4d%-9s%-8s%-105s\r\n",cal_yr,(tran_ein!=NULL?tran_ein:""),(computer!=NULL?computer:"")," ");
		}
		if(RDA_fp!=NULL)
		{
			RDA_fprintf(RDA_fp,"1B%4d%-9s%-8s%-105s\r\n",cal_yr,(tran_ein!=NULL?tran_ein:""),(computer!=NULL?computer:"")," ");
		}
		if(RDAstrlen(tran_name)>44)
		{
			tran_name[44]=0;
		}
		if(RDAstrlen(tran_addr1)>35)
		{
			tran_addr1[35]=0;
		}
		if(RDAstrlen(tran_city)>20)
		{
			tran_city[20]=0;
		}
		if(fp!=NULL)
		{
			fprintf(fp,"2B%-13s%-1s%-44s%-35s%-20s%-2s%-5s%-5s%-1s\r\n"," "," ",(tran_name!=NULL?tran_name:""),(tran_addr1!=NULL?tran_addr1:""),(tran_city!=NULL?tran_city:""),(tran_state!=NULL?tran_state:""),(tran_zip4!=NULL?tran_zip4:""),(tran_zip!=NULL?tran_zip:"")," ");
		}
		if(RDA_fp!=NULL)
		{
			RDA_fprintf(RDA_fp,"2B%-13s%-1s%-44s%-35s%-20s%-2s%-5s%-5s%-1s\r\n"," "," ",(tran_name!=NULL?tran_name:""),(tran_addr1!=NULL?tran_addr1:""),(tran_city!=NULL?tran_city:""),(tran_state!=NULL?tran_state:""),(tran_zip4!=NULL?tran_zip4:""),(tran_zip!=NULL?tran_zip:"")," ");
		}
		tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
			module,"Creating W2 Magnetic Media",NULL);
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
		FINDFLDGETSTRING(sortfile->fileno,"EMPLOYER IDENTIFICATION",&empr_id);
		last_emprid=stralloc(empr_id);
		FINDFLDGETSHORT(sortfile->fileno,"W2 TYPE",&w2type);
		last_w2type=w2type;
		do_empr_header=TRUE;
		do_empr_footer=FALSE;
		w2_subcounter=0.0; 
		w2_total_counter=0.0; 
		while(!ef)
		{
			FINDFLDGETINT(sortfile->fileno,"CALENDAR YEAR",&cal_yr);
			FINDFLDGETSTRING(sortfile->fileno,"EMPLOYER IDENTIFICATION",&empr_id);
			FINDFLDGETSHORT(sortfile->fileno,"W2 TYPE",&w2type);
			if(diagapps)
			{
				prterr("Compairing: ((last_emprid=[%s],empr_id=[%s]) || (last_w2type=[%d],w2type=[%d]))",last_emprid,empr_id,last_w2type,w2type);TRACE;
			}
			if(RDAstrcmp(last_emprid,empr_id) || last_w2type!=w2type)
			{
				do_empr_footer=TRUE;
				do_empr_header=TRUE;
			} else {
				do_empr_footer=FALSE;
			}
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
/* begin Employer Header */
					if(do_empr_footer==TRUE)
					{
						if(mod_value!=0)
						{
							w2employer_subfooter(prtrsrc,p,subtotal_box3,subtotal_box7,subtotal_box1,subtotal_box4,subtotal_box2,w2_subempr_counter,subtotal_box13C,0.0,subtotal_box9,subtotal_box8,subtotal_box12,(subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H),subtotal_box10,subtotal_box11A,subtotal_box11B,subtotal_box5,subtotal_box6,0.0);
							w2_subempr_counter=0.0;
							w2_subcounter=0;
						}
						w2employer_footer(prtrsrc,p,w2_empr_counter,empr_total_box3,empr_total_box7,empr_total_box1,empr_total_box4,empr_total_box2,empr_total_box13C,0.0,empr_total_box9,empr_total_box8,empr_total_box12,(empr_total_box13D+empr_total_box13E+empr_total_box13F+empr_total_box13G+empr_total_box13H),empr_total_box10,empr_total_box11A,empr_total_box11B,empr_total_box5,empr_total_box6,empr_total_trdparty,0.0,report);
						w2_empr_counter=0.0;
					}
					if(do_empr_header==TRUE)
					{
						w2employer_header(prtrsrc,p,cal_yr,w2type,empl_name_type);
						empr_total_trdparty=0.0;
						empr_total_box1=0.0;
						empr_total_box2=0.0;
						empr_total_box3=0.0;
						empr_total_box4=0.0;
						empr_total_box5=0.0;
						empr_total_box6=0.0;
						empr_total_box7=0.0;
						empr_total_box8=0.0;
						empr_total_box9=0.0;
						empr_total_box10=0.0;
						empr_total_box11A=0.0;
						empr_total_box11B=0.0;
						empr_total_box12=0.0;
/*
						empr_total_box13A=0.0;
						empr_total_box13B=0.0;
*/
						empr_total_box13C=0.0;
						empr_total_box13D=0.0;
						empr_total_box13E=0.0;
						empr_total_box13F=0.0;
						empr_total_box13G=0.0;
						empr_total_box13H=0.0;

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
						subtotal_box11B=0.0;
						subtotal_box12=0.0;
						subtotal_box13C=0.0;
						subtotal_box13D=0.0;
						subtotal_box13E=0.0;
						subtotal_box13F=0.0;
						subtotal_box13G=0.0;
						subtotal_box13H=0.0;

						do_empr_header=FALSE;
					}
/* end Employer Header */
					if(XPERT_SETUP->software_type==2)
					{
						FINDFLDSETSTRING(perdmg,"EMPLOYEE ID",per_id);
					} else {
						FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",per_id);
					}
					ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc);
/*
					if(!ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc))
					{
*/
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
						FINDFLDGETDOUBLE(payw2,"FEDERAL WITHHOLDING THIRD-PARTY",&trdparty);
						FINDFLDGETDOUBLE(payw2,"BOX 13-C AMOUNT",&box13C);
						FINDFLDGETDOUBLE(payw2,"BOX 13-D AMOUNT",&box13D);
						FINDFLDGETDOUBLE(payw2,"BOX 13-E AMOUNT",&box13E);
						FINDFLDGETDOUBLE(payw2,"BOX 13-F AMOUNT",&box13F);
						FINDFLDGETDOUBLE(payw2,"BOX 13-G AMOUNT",&box13G);
						FINDFLDGETDOUBLE(payw2,"BOX 13-H AMOUNT",&box13H);
						total_box3+=box3;
						total_box7+=box7;
						total_box1+=box1;
						total_box4+=box4;
						total_box2+=box2;
						total_box9+=box9;

						empr_total_trdparty+=trdparty;
						empr_total_box1+=box1;
						empr_total_box2+=box2;
						empr_total_box3+=box3;
						empr_total_box4+=box4;
						empr_total_box5+=box5;
						empr_total_box6+=box6;
						empr_total_box7+=box7;
						empr_total_box8+=box8;
						empr_total_box9+=box9;
						empr_total_box10+=box10;
						empr_total_box11A+=box11A;
						empr_total_box11B+=box11B;
						empr_total_box12+=box12;
						empr_total_box13C+=box13C;
						empr_total_box13D+=box13D;
						empr_total_box13E+=box13E;
						empr_total_box13F+=box13F;
						empr_total_box13G+=box13G;

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
						subtotal_box13C+=box13C;
						subtotal_box13D+=box13D;
						subtotal_box13E+=box13E;
						subtotal_box13F+=box13F;
						subtotal_box13G+=box13G;
						subtotal_box13H+=box13H;

						FINDFLDGETINT(payw2,"CONTROL NUMBER",&cont_no);
						cont_no_double=(double)cont_no;
						cont_no_string=formatamt(cont_no_double,7,'0');
						FINDFLDGETSTRING(payw2,"SOCIAL SECURITY NUMBER",&ssn);
/*
						FINDFLDGETSTRING(perdmg,"SOCIAL SECURITY NUMBER",&ssn);
*/
						if(!isEMPTY(ssn))
						{
							if(strstr(ssn,"-")!=NULL)
							{
								tmp_string=Rmalloc(RDAstrlen(ssn)+1);
								for(x=0,y=0;x<RDAstrlen(ssn);++x)
								{
									if(ssn[x]!='-')
									{
										tmp_string[y]=ssn[x];
										++y;
									}
								}
								Rfree(ssn);
								ssn=stralloc(tmp_string);
								if(tmp_string!=NULL) Rfree(tmp_string);
							}
						}
						if(RDAstrlen(ssn)>9)
						{
							ssn[9]=0;
						}
/*
						FINDFLDGETSTRING(payw2,"EMPLOYEE NAME",&empl_name);
*/
						if(empl_name_type==0)
						{
							d=getDFvirtual("PAYROLL","W2 NAME LAST FIRST MIDDLE LINEAGE");
						} else {
							d=getDFvirtual("PAYROLL","W2 NAME FIRST MIDDLE LAST LINEAGE");
						}
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
							FINDFLDGETSTRING(payw2,"EMPLOYEE NAME",&empl_name);
						}
						if(RDAstrlen(empl_name)>27)
						{
							empl_name[27]=0;
						}
						stoupper(empl_name);
						FINDFLDGETSTRING(payw2,"EMPLOYEE ADDRESS 1",&empl_addr1);
						FINDFLDGETSTRING(payw2,"EMPLOYEE ADDRESS 2",&empl_addr2);
/*
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
*/
						if(RDAstrlen(empl_addr1)>40)
						{
							empl_addr1[40]=0;
						}
						stoupper(empl_addr1);
/*
						FINDFLDGETSTRING(payw2,"EMPLOYEE ADDRESS 2",&empl_addr2);
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
						if(RDAstrlen(empl_addr2)>40)
						{
							empl_addr2[40]=0;
						}
						stoupper(empl_addr2);
*/
						FINDFLDGETSTRING(payw2,"EMPLOYEE CITY",&empl_city);
/*
						FINDFLDGETSTRING(perdmg,"MAILING CITY",&empl_city);
*/
						if(RDAstrlen(empl_city)>25)
						{
							empl_city[25]=0;
						}
						stoupper(empl_city);
/*
						FINDFLDGETSTRING(perdmg,"MAILING STATE",&empl_state);
*/
						FINDFLDGETSTRING(payw2,"EMPLOYEE STATE",&empl_state);
						if(RDAstrlen(empl_state)>2)
						{
							empl_state[2]=0;
						}
						stoupper(empl_state);
/*
						FINDFLDGETSTRING(perdmg,"MAILING ZIP CODE",&empl_zip);
*/
						FINDFLDGETSTRING(payw2,"EMPLOYEE ZIP CODE",&empl_zip);
						if(!isEMPTY(empl_zip))
						{
							empl_zip4=Rmalloc(6);
							if((RDAstrlen(empl_zip)==10) && (empl_zip[5]=='-'))
							{
								empl_zip4[0]=empl_zip[5];
								empl_zip4[1]=empl_zip[6];
								empl_zip4[2]=empl_zip[7];
								empl_zip4[3]=empl_zip[8];
								empl_zip4[4]=empl_zip[9];
								empl_zip4[5]=0;
							}
						}
						if(RDAstrlen(empl_zip)>5)
						{
							empl_zip[5]=0;
						}
						FINDFLDGETCHAR(payw2,"STATUTORY EMPLOYEE",&statutory);
						FINDFLDGETCHAR(payw2,"DECEASED",&deceased);
						FINDFLDGETCHAR(payw2,"PENSION PLAN",&penplan);
						FINDFLDGETCHAR(payw2,"LEGAL REP",&legrep);
						FINDFLDGETCHAR(payw2,"942 HSHLD EMP",&hsldemp);
						FINDFLDGETCHAR(payw2,"DEFERRED COMPENSATION",&defcomp);
						if(deceased==TRUE)
						{
							new_empl_name=Rmalloc(strlen(empl_name)+8);
							sprintf(new_empl_name,"%s DEC'D",empl_name);
						} else { 
							new_empl_name=stralloc(empl_name);
						}
						if(RDAstrlen(new_empl_name)>27)
						{
							new_empl_name[27]=0;
						}
						w2_subcounter+=1;
						w2_empr_counter+=1;
						w2_subempr_counter+=1;
						w2_total_counter+=1;
						if(fp!=NULL)
						{
							fprintf(fp,"1W%-9s%-27s%-40s%-25s%-2s%-8s%-5s%-5s%-1s%-4s\r\n",((!isEMPTY(ssn))?ssn:"I        "),(new_empl_name!=NULL?new_empl_name:""),(empl_addr1!=NULL?empl_addr1:""),(empl_city!=NULL?empl_city:""),(empl_state!=NULL?empl_state:"")," ",(empl_zip4!=NULL?empl_zip4:""),(empl_zip!=NULL?empl_zip:""),(statutory==TRUE?"S":" ")," ");
						}
						if(RDA_fp!=NULL)
						{
							RDA_fprintf(RDA_fp,"1W%-9s%-27s%-40s%-25s%-2s%-8s%-5s%-5s%-1s%-4s\r\n",((!isEMPTY(ssn))?ssn:"I        "),(new_empl_name!=NULL?new_empl_name:""),(empl_addr1!=NULL?empl_addr1:""),(empl_city!=NULL?empl_city:""),(empl_state!=NULL?empl_state:"")," ",(empl_zip4!=NULL?empl_zip4:""),(empl_zip!=NULL?empl_zip:""),(statutory==TRUE?"S":" ")," ");
						}
						box3_string=formatamt(box3,7,'0');
						box7_string=formatamt(box7,7,'0');
						box1_string=formatamt(box1,9,'0');
						box4_string=formatamt(box4,6,'0');
						box2_string=formatamt(box2,9,'0');
						box11A_string=formatamt(box11A,9,'0');
						box11B_string=formatamt(box11B,9,'0');
						box13C_string=formatamt(box13C,7,'0');
						box9_string=formatamt(box9,7,'0');
						box8_string=formatamt(box8,7,'0');
						box12_string=formatamt(box12,9,'0');
						box13DEFGH=box13D+box13E+box13F+box13G+box13H;
						box13DEFGH_string=formatamt((box13DEFGH),9,'0');
						box10_string=formatamt(box10,7,'0');
						if(fp!=NULL)
						{
							fprintf(fp,"2W%-7s%-1s%-7s%-1s%-9s%-1s%-6s%-9s%-9s%-1s%-9s%-7s%-7s%-7s%-7s%-7s%-9s%-1s%-1s%-1s%-1s%-1s%-9s%-1s%-7s\r\n",box3_string," ",box7_string," ",box1_string," ",box4_string,box2_string,box11A_string," ",box11B_string,cont_no_string,box13C_string,"0000000",box9_string,box8_string,box12_string," ",(penplan==TRUE?"P":" ")," ",(defcomp==TRUE?"D":" ")," ",box13DEFGH_string," ",box10_string);
						}
						if(RDA_fp!=NULL)
						{
							RDA_fprintf(RDA_fp,"2W%-7s%-1s%-7s%-1s%-9s%-1s%-6s%-9s%-9s%-1s%-9s%-7s%-7s%-7s%-7s%-7s%-9s%-1s%-1s%-1s%-1s%-1s%-9s%-1s%-7s\r\n",box3_string," ",box7_string," ",box1_string," ",box4_string,box2_string,box11A_string," ",box11B_string,cont_no_string,box13C_string,"0000000",box9_string,box8_string,box12_string," ",(penplan==TRUE?"P":" ")," ",(defcomp==TRUE?"D":" ")," ",box13DEFGH_string," ",box10_string);
						}
						if(report==TRUE)
						{
							if(fp!=NULL)
							{
								fprintf(fp,"FICA WAGES=%10.02f  FED WAGES=%10.02f  FICA W/H=%10.02f  FED W/H=%10.02f  INS=%10.02f  FRG=%10.02f  DEFERRED=%10.02f\r\n",(box3/100),(box1/100),(box4/100),(box2/100),(box13C/100),(box12/100),((box13D+box13E+box13F+box13G+box13H)/100));
								fprintf(fp,"DEPENDENT=%10.02f  NQ457=%-10.02f  NQ non 457=%10.02f  EIC=%10.02f\r\n",(box10/100),(box11A/100),(box11B/100),(box9/100));
							}
							if(RDA_fp!=NULL)
							{
								RDA_fprintf(RDA_fp,"FICA WAGES=%10.02f  FED WAGES=%10.02f  FICA W/H=%10.02f  FED W/H=%10.02f  INS=%10.02f  FRG=%10.02f  DEFERRED=%10.02f\r\n",(box3/100),(box1/100),(box4/100),(box2/100),(box13C/100),(box12/100),((box13D+box13E+box13F+box13G+box13H)/100));
								RDA_fprintf(RDA_fp,"DEPENDENT=%10.02f  NQ457=%-10.02f  NQ non 457=%10.02f  EIC=%10.02f\r\n",(box10/100),(box11A/100),(box11B/100),(box9/100));
							}
						}
						box5_string=formatamt(box5,9,'0');
						box6_string=formatamt(box6,7,'0');
						if(fp!=NULL)
						{
							fprintf(fp,"3W%-9s%-7s%-15s%-7s%-88s\r\n",box5_string,box6_string," ","0000000"," ");
						}
						if(RDA_fp!=NULL)
						{
							RDA_fprintf(RDA_fp,"3W%-9s%-7s%-15s%-7s%-88s\r\n",box5_string,box6_string," ","0000000"," ");
						}
						if(report==TRUE)
						{
							if(fp!=NULL)
							{
								fprintf(fp,"MEDICARE WAGES=%10.02f  MEDICARE W/H=%-10.02f\r\n",(box5/100),(box6/100));
							}
							if(RDA_fp!=NULL)
							{
								RDA_fprintf(RDA_fp,"MEDICARE WAGES=%10.02f  MEDICARE W/H=%-10.02f\r\n",(box5/100),(box6/100));
							}
						}
						if(box3_string!=NULL) Rfree(box3_string);
						if(box7_string!=NULL) Rfree(box7_string);
						if(box1_string!=NULL) Rfree(box1_string);
						if(box4_string!=NULL) Rfree(box4_string);
						if(box2_string!=NULL) Rfree(box2_string);
						if(box11A_string!=NULL) Rfree(box11A_string);
						if(box11B_string!=NULL) Rfree(box11B_string);
						if(box13C_string!=NULL) Rfree(box13C_string);
						if(box9_string!=NULL) Rfree(box9_string);
						if(box8_string!=NULL) Rfree(box8_string);
						if(box12_string!=NULL) Rfree(box12_string);
						if(box13DEFGH_string!=NULL) Rfree(box13DEFGH_string);
						if(box10_string!=NULL) Rfree(box10_string);
						if(box5_string!=NULL) Rfree(box5_string);
						if(box6_string!=NULL) Rfree(box6_string);
						FINDFLDGETSTRING(payw2,"STATE 1 LABEL",&st1_label);
						if(RDAstrlen(st1_label)>2)
						{
							st1_label[2]=0;
						}
						stoupper(st1_label);
						FINDFLDGETSTRING(payw2,"STATE 1 EIN",&st1_ein);
						if(RDAstrlen(st1_ein)>12)
						{
							st1_ein[12]=0;
						}
						FINDFLDGETDOUBLE(payw2,"STATE 1 WAGES",&st1_gross);
						FINDFLDGETDOUBLE(payw2,"STATE 1 WITHHOLDING",&st1_amount);
						FINDFLDGETSTRING(payw2,"LOCAL 1 LABEL",&loc1_label);
						if(RDAstrlen(loc1_label)>7)
						{
							loc1_label[7]=0;
						}
						FINDFLDGETDOUBLE(payw2,"LOCAL 1 WAGES",&loc1_gross);
						FINDFLDGETDOUBLE(payw2,"LOCAL 1 WITHHOLDING",&loc1_amount);
						stoupper(st1_label);
						state_number_code=getstnum(st1_label);
						if(MakeSRecords==TRUE)
						{
						if(fp!=NULL)
						{
							fprintf(fp,"1S%-9s%-27s%-40s%-25s%-2s%-8s%-5s%-5s%-1s%-2s%-2s\r\n",((!isEMPTY(ssn))?ssn:"I        "),(new_empl_name!=NULL?new_empl_name:""),(empl_addr1!=NULL?empl_addr1:""),(empl_city!=NULL?empl_city:""),(empl_state!=NULL?empl_state:"")," ",(empl_zip4!=NULL?empl_zip4:""),(empl_zip!=NULL?empl_zip:"")," ",(state_number_code!=NULL?state_number_code:""),"  ");
						} 
						if(RDA_fp!=NULL)
						{
							RDA_fprintf(RDA_fp,"1S%-9s%-27s%-40s%-25s%-2s%-8s%-5s%-5s%-1s%-2s%-2s\r\n",((!isEMPTY(ssn))?ssn:"I        "),(new_empl_name!=NULL?new_empl_name:""),(empl_addr1!=NULL?empl_addr1:""),(empl_city!=NULL?empl_city:""),(empl_state!=NULL?empl_state:"")," ",(empl_zip4!=NULL?empl_zip4:""),(empl_zip!=NULL?empl_zip:"")," ",(state_number_code!=NULL?state_number_code:""),"  ");
						}
						}
						st1_gross_string=formatamt(st1_gross,9,'0');
						st1_amount_string=formatamt(st1_amount,8,'0');
						loc1_gross_string=formatamt(loc1_gross,9,'0');
						loc1_amount_string=formatamt(loc1_amount,7,'0');
						tmp_string=Rmalloc(5);
						sprintf(tmp_string,"%4d",cal_yr);
						strptperiod=Rmalloc(5);
						sprintf(strptperiod,"12%c%c",tmp_string[2],tmp_string[3]);
						if(tmp_string!=NULL) Rfree(tmp_string);
						stempdate=Rmalloc(5);
						if(XPERT_SETUP->software_type==2)
						{
							FINDFLDGETSTRING(perdmg,"HIRE DATE",&tmp_string);
						} else {
							FINDFLDGETSTRING(perdmg,"HIRE DATE",&tmp_string);
						}
						if(!isEMPTY(tmp_string))
						{
							if(RDAstrlen(tmp_string)==10)
							{
								sprintf(stempdate,"%c%c%c%c",tmp_string[0],tmp_string[1],tmp_string[8],tmp_string[9]);
							} else if(RDAstrlen(tmp_string)==8)
							{
								sprintf(stempdate,"%c%c%c%c",tmp_string[0],tmp_string[1],tmp_string[6],tmp_string[7]);
							}
						}
						if(MakeSRecords==TRUE)
						{
						if(fp!=NULL)
						{
							fprintf(fp,"2S%-12s%-4s%-9s%-9s%-2s%-4s%-4s%-5s%-2s%-9s%-8s%-10s%-1s%-5s%-9s%-7s%-7s%-19s\r\n",(st1_ein!=NULL?st1_ein:""),strptperiod,"000000000","000000000","52",(!isEMPTY(stempdate)?stempdate:"    "),"    ","     ",(state_number_code!=NULL?state_number_code:""),st1_gross_string,st1_amount_string,"          ",(!isEMPTY(loc1_label)?"C":" "),(loc1_label!=NULL?loc1_label:""),loc1_gross_string,loc1_amount_string,cont_no_string," ");
/*
							fprintf(fp,"2S%-12s%-4s%-9s%-9s%-2s%-4s%-4s%-5s%-2s%-9s%-8s%-10s%-1s%-5s%-9s%-7s%-7s%-19s\r\n",(st1_ein!=NULL?st1_ein:""),strptperiod,st1_gross_string,st1_gross_string,"52",(!isEMPTY(stempdate)?stempdate:"    "),"    ","     ",(state_number_code!=NULL?state_number_code:""),st1_gross_string,st1_amount_string,"          ",(!isEMPTY(loc1_label)?"C":" "),(loc1_label!=NULL?loc1_label:""),loc1_gross_string,loc1_amount_string,cont_no_string," ");
*/
						}
						if(RDA_fp!=NULL)
						{
							RDA_fprintf(RDA_fp,"2S%-12s%-4s%-9s%-9s%-2s%-4s%-4s%-5s%-2s%-9s%-8s%-10s%-1s%-5s%-9s%-7s%-7s%-19s\r\n",(st1_ein!=NULL?st1_ein:""),strptperiod,"000000000","000000000","52",(!isEMPTY(stempdate)?stempdate:"    "),"    ","     ",(state_number_code!=NULL?state_number_code:""),st1_gross_string,st1_amount_string,"          ",(!isEMPTY(loc1_label)?"C":" "),(loc1_label!=NULL?loc1_label:""),loc1_gross_string,loc1_amount_string,cont_no_string," ");
/*
							RDA_fprintf(RDA_fp,"2S%-12s%-4s%-9s%-9s%-2s%-4s%-4s%-5s%-2s%-9s%-8s%-10s%-1s%-5s%-9s%-7s%-7s%-19s\r\n",(st1_ein!=NULL?st1_ein:""),strptperiod,st1_gross_string,st1_gross_string,"52",(!isEMPTY(stempdate)?stempdate:"    "),"    ","     ",(state_number_code!=NULL?state_number_code:""),st1_gross_string,st1_amount_string,"          ",(!isEMPTY(loc1_label)?"C":" "),(loc1_label!=NULL?loc1_label:""),loc1_gross_string,loc1_amount_string,cont_no_string," ");
*/
						}
						}
						if(report==TRUE)
						{
							if(fp!=NULL)
							{
								fprintf(fp,"STATE WAGES=%10.02f  STATE W/H=%-10.02f\r\n",(st1_gross/100),(st1_amount/100));
							}
							if(RDA_fp!=NULL)
							{
								RDA_fprintf(RDA_fp,"STATE WAGES=%10.02f  STATE W/H=%-10.02f\r\n",(st1_gross/100),(st1_amount/100));
							}
						}
						if(state_number_code!=NULL) Rfree(state_number_code);
						if(strptperiod!=NULL) Rfree(strptperiod);
						if(stempdate!=NULL) Rfree(stempdate);
						if(st1_gross_string!=NULL) Rfree(st1_gross_string);
						if(st1_amount_string!=NULL) Rfree(st1_amount_string);
						if(loc1_gross_string!=NULL) Rfree(loc1_amount_string);
						if(loc1_amount_string!=NULL) Rfree(loc1_gross_string);
/*
						FreeBox13Struct(BOX13);
*/

						mod_value=fmod(w2_subcounter,body_count);
						if(mod_value==0) 
						{
							w2employer_subfooter(prtrsrc,p,subtotal_box3,subtotal_box7,subtotal_box1,subtotal_box4,subtotal_box2,w2_subempr_counter,subtotal_box13C,0.0,subtotal_box9,subtotal_box8,subtotal_box12,(subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H),subtotal_box10,subtotal_box11A,subtotal_box11B,subtotal_box5,subtotal_box6,0.0);
							w2_subcounter=0;
							w2_subempr_counter=0.0;
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
							subtotal_box13C=0.0; 
							subtotal_box13D=0.0; 
							subtotal_box13E=0.0; 
							subtotal_box13F=0.0; 
							subtotal_box13G=0.0; 
							subtotal_box13H=0.0; 
						}
/*
					}
*/
				}
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
			if(last_emprid!=NULL) Rfree(last_emprid);
			last_emprid=stralloc(empr_id);
			last_w2type=w2type;
			ef=ADVNXTNRDsec(sortfile->fileno,1,SCRNvirtualSubData,mainrsrc);

			if(empr_id!=NULL) Rfree(empr_id);
			if(per_id!=NULL) Rfree(per_id);
			if(ssn!=NULL) Rfree(ssn);
			if(empl_name!=NULL) Rfree(empl_name);
			if(new_empl_name!=NULL) Rfree(new_empl_name);
			if(empl_addr1!=NULL) Rfree(empl_addr1);
			if(empl_addr2!=NULL) Rfree(empl_addr2);
			if(empl_addr3!=NULL) Rfree(empl_addr3);
			if(st1_label!=NULL) Rfree(st1_label);
			if(st1_ein!=NULL) Rfree(st1_ein);
			if(loc1_label!=NULL) Rfree(loc1_label);
			if(cont_no_string!=NULL) Rfree(cont_no_string);
		}
		if(mod_value!=0)
		{
			w2employer_subfooter(prtrsrc,p,subtotal_box3,subtotal_box7,subtotal_box1,subtotal_box4,subtotal_box2,w2_subempr_counter,subtotal_box13C,0.0,subtotal_box9,subtotal_box8,subtotal_box12,(subtotal_box13D+subtotal_box13E+subtotal_box13F+subtotal_box13G+subtotal_box13H),subtotal_box10,subtotal_box11A,subtotal_box11B,subtotal_box5,subtotal_box6,0.0);
			w2_subcounter=0;
		}
		w2employer_footer(prtrsrc,p,w2_empr_counter,empr_total_box3,empr_total_box7,empr_total_box1,empr_total_box4,empr_total_box2,empr_total_box13C,0.0,empr_total_box9,empr_total_box8,empr_total_box12,(empr_total_box13D+empr_total_box13E+empr_total_box13F+empr_total_box13G+empr_total_box13H),empr_total_box10,empr_total_box11A,empr_total_box11B,empr_total_box5,empr_total_box6,empr_total_trdparty,0.0,report);
		w2_total_counter_string=formatamt(w2_total_counter,7,'0');
		total_box3_string=formatamt(total_box3,16,'0');
		total_box7_string=formatamt(total_box7,16,'0');
		total_box1_string=formatamt(total_box1,16,'0');
		total_box4_string=formatamt(total_box4,16,'0');
		total_box2_string=formatamt(total_box2,16,'0');
		total_box9_string=formatamt(total_box9,16,'0');
		if(fp!=NULL)
		{
			fprintf(fp,"1F%-7s%-16s%-1s%-16s%-1s%-16s%-1s%-16s%-1s%-16s%-1s%-16s%-18s",w2_total_counter_string,total_box3_string," ",total_box7_string," ",total_box1_string," ",total_box4_string," ",total_box2_string," ",total_box9_string,"                  ");
		}
		if(RDA_fp!=NULL)
		{
			RDA_fprintf(RDA_fp,"1F%-7s%-16s%-1s%-16s%-1s%-16s%-1s%-16s%-1s%-16s%-1s%-16s%-18s",w2_total_counter_string,total_box3_string," ",total_box7_string," ",total_box1_string," ",total_box4_string," ",total_box2_string," ",total_box9_string,"                  ");
		}
		if(report==TRUE)
		{
			if(fp!=NULL)
			{
				fprintf(fp,"\r\nNO. EMPLOYEES=%9.0f  FICA WAGES=%16.02f  FED WAGES=%16.02f  FICA W/H=%16.02f  FED W/H=%16.02f\r\n",(w2_total_counter),(total_box3/100),(total_box1/100),(total_box4/100),(total_box2/100));
				fprintf(fp,"EIC=%16.02f\r\n",(total_box9/100));
			}
			if(RDA_fp!=NULL)
			{
				RDA_fprintf(RDA_fp,"\r\nNO. EMPLOYEES=%9.0f  FICA WAGES=%16.02f  FED WAGES=%16.02f  FICA W/H=%16.02f  FED W/H=%16.02f\r\n",(w2_total_counter),(total_box3/100),(total_box1/100),(total_box4/100),(total_box2/100));
				RDA_fprintf(RDA_fp,"EIC=%16.02f\r\n",(total_box9/100));
			}
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	if(w2_total_counter_string!=NULL) Rfree(w2_total_counter_string);
	if(total_box3_string!=NULL) Rfree(total_box3_string);
	if(total_box7_string!=NULL) Rfree(total_box7_string);
	if(total_box4_string!=NULL) Rfree(total_box4_string);
	if(total_box2_string!=NULL) Rfree(total_box2_string);
	if(total_box9_string!=NULL) Rfree(total_box9_string);
	if(total_box1_string!=NULL) Rfree(total_box1_string);
	if(PAYROLL_SETUP!=NULL) free_payroll(PAYROLL_SETUP);
	if(tmp_string!=NULL) Rfree(tmp_string);
	if(tran_name!=NULL) Rfree(tran_name);
	if(tran_ein!=NULL) Rfree(tran_ein);
	if(tran_addr1!=NULL) Rfree(tran_addr1);
	if(tran_city!=NULL) Rfree(tran_city);
	if(tran_state!=NULL) Rfree(tran_state);
	if(tran_zip!=NULL) Rfree(tran_zip);
	if(tran_zip4!=NULL) Rfree(tran_zip4);
	if(computer!=NULL) Rfree(computer);
	if(empr_id!=NULL) Rfree(empr_id);
	if(per_id!=NULL) Rfree(per_id);
	if(ssn!=NULL) Rfree(ssn);
	if(empl_name!=NULL) Rfree(empl_name);
	if(new_empl_name!=NULL) Rfree(new_empl_name);
	if(empl_addr1!=NULL) Rfree(empl_addr1);
	if(empl_addr2!=NULL) Rfree(empl_addr2);
	if(empl_addr3!=NULL) Rfree(empl_addr3);
	if(empl_city!=NULL) Rfree(empl_city);
	if(empl_state!=NULL) Rfree(empl_state);
	if(empl_zip!=NULL) Rfree(empl_zip);
	if(empl_zip4!=NULL) Rfree(empl_zip4);
	if(last_emprid!=NULL) Rfree(last_emprid);
}
int main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	int cal_year=0;
	char *tran_name=NULL,*computer=NULL;
	int x=0;

	if(InitializeSubsystems(argc,argv,module,"MAKE W2 DISK")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((payw2=APPOPNNRD(module,"PAYW2",TRUE,FALSE))==(-1)) return;
	if(XPERT_SETUP->software_type==2)
	{
		if((perdmg=APPOPNNRD("PAYROLL","PAYPER",TRUE,FALSE))==(-1)) return;
	} else {
		if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	}
	if((payempr=APPOPNNRD(module,"PAYEMPR",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"MAKE W2 DISK");
	addDFincvir(mainrsrc,module,"PAYW2",NULL,payw2);
	if(XPERT_SETUP->software_type==2)
	{
		addDFincvir(mainrsrc,"PAYROLL","PAYPER",NULL,perdmg);
	} else {
		addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	}
	addDFincvir(mainrsrc,module,"PAYEMPR",NULL,payempr);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	MakeSRecords=TRUE;
	addstdrsrc(mainrsrc,"MAKE S RECORDS",BOOLNS,1,&MakeSRecords,TRUE);
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&cal_year,TRUE);
	addstdrsrc(mainrsrc,"TRANSMITTER NAME",PLAINTEXT,50,&tran_name,TRUE);
	addstdrsrc(mainrsrc,"COMPUTER TYPE",PLAINTEXT,8,&computer,TRUE);
	if(name_code_list!=NULL) freeapplib(name_code_list);
	name_code_list=APPlibNEW();
	addAPPlib(name_code_list,"Surname First");
	addAPPlib(name_code_list,"First Name First");
	addlstrsrc(mainrsrc,"EMPLOYEE NAME CODES",&x,TRUE,NULL,name_code_list->numlibs,&name_code_list->libs,NULL);
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
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,makew2mm,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
