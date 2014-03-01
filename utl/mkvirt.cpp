/* mkvirtual.c - Makes Virtual Fields from Define Account Code Specifications */
/*lint -library */
#ifndef WIN32
#define __NAM__ "mkvirt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkvirt.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkvir.hpp>
#include <fin.hpp>
/*COMPMEM 40960 */
/*CPP_OPART finvirt1 */
/*CPP_OPART finvirt2 */
/*CPP_OPART finvirt3 */
/*CPP_OPART finvirt4 */
/*CPP_OPART finvirt5 */
/*CPP_OPART finvirt6 */
/*CPP_OPART venvirt */
/*CPP_OPART purvirt */
/*CPP_OPART payvirt1 */
/*CPP_OPART payvirt2 */
/*CPP_OPART lvevirt */
/*CPP_OPART rlstvirt */
/*CPP_OPART occvirt */
/*ADDLIB mkvir */
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void FINMGTCreateEV1(void);
extern void FINMGTCreateEV2(void);
extern void FINMGTCreateRV1(void);
extern void FINMGTCreateRV2(void);
extern void FINMGTCreateBV1(void);
extern void FINMGTCreateBV2(void);
extern void VENPMTCreate(void);
extern void PURORDCreate(void);
static void CONVERSIONCreate(void);
static void BNKRECCreate(void);
extern void PAYROLLCreate1(char *);
extern void PAYROLLCreate2(char *);
extern void LVEMGTCreate(void);
extern void RLSTMGTCreate(void);
extern void OCCTAXCreate(void);
static void getmonths(void);
char *getmonth(int);
char *makepretodateexpres(char *,char *,char *);
RDAfinmgt *finsetup=NULL;
char *modulename="RPTGEN",*vname=NULL;
APPlib *modlst=NULL;
APPlib *months=NULL;

struct REPEATfields
{
	int num;
	DFvirtual *df;
};
typedef struct REPEATfields REPEATfield;

REPEATfield *REPEATfieldNEW()
{
	REPEATfield *tmp;

	tmp=Rmalloc(sizeof(REPEATfield));
	tmp->num=0;
	tmp->df=NULL;
	return(tmp);
}
void FreeREPEATfield(REPEATfield *f)
{
	int x,y;
	DFvirtual *d;
	DFincvir *i;

	if(f!=NULL)
	{
		if(f->df!=NULL)
		{
			for(x=0,d=f->df;x<f->num;++x,++d)
			{
				if(d->incvir!=NULL)
				{
					for(y=0,i=d->incvir;y<d->num;++y,++i)
					{
						if(i->module!=NULL) Rfree(i->module);
						if(i->file!=NULL) Rfree(i->file);
						if(i->keyname!=NULL) Rfree(i->keyname);
					}
					Rfree(d->incvir);
				}
				if(d->expression!=NULL) Rfree(d->expression);
				if(d->name!=NULL) Rfree(d->name);
				if(d->module!=NULL) Rfree(d->module);
				if(d->desc!=NULL) Rfree(d->desc);
			}
			Rfree(f->df);
		}
		if(f!=NULL) Rfree(f);
	}
}
void AddField2REPEAT(REPEATfield *f,char *modx,char *name,short type,
	short length,char *desc)
{
	DFvirtual *d;

	if(f->df!=NULL)
	{
		f->df=Rrealloc(f->df,(f->num+1)*sizeof(DFvirtual));
	} else {
		f->df=Rmalloc(sizeof(DFvirtual));
		f->num=0;
	}
	d=f->df+f->num;
	d->vtype=1;
	if(modx!=NULL) d->module=stralloc(modx);
		else d->module=NULL;
	if(name!=NULL) d->name=stralloc(name);
		else d->name=NULL;
	d->when=0;
	d->length=length;
	d->type=type;
	d->num=0;
	d->incvir=NULL;
	d->expression=NULL;
	if(desc!=NULL) d->desc=stralloc(desc);
		else d->desc=NULL;
}
void SaveRepeatFields(REPEATfield *f)
{
	int x;
	DFvirtual *d;

	if(f!=NULL)
	{
		if(f->df!=NULL)
		{
			for(x=0,d=f->df;x<f->num;++x,++d)
			{
				writeDFvirtual(d->module,d);
			}
		}
	} 
}
void FindRepeatFields(char *modx,REPEATfield *RF)
{
	char *libx=NULL;
	APPlib *filenames=NULL;
	int x=0,y,z;
	short engine=0,nofields=0,nokeys=0;
	NRDkey *keys=NULL,*key;
	NRDpart *part;
	NRDfield *fields=NULL,*field;
	DFvirtual *d;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modx);
#endif
	filenames=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(filenames!=NULL)
	{
		for(z=0;z<filenames->numlibs;++z)
		{
			if(!GETBIN(modx,filenames->libs[z],&engine,&nofields,&fields,
				&nokeys,&keys)) 
			{
				if(keys!=NULL)
				{
					for(y=0,key=keys;y<nokeys;++y,++key)
					{
						if(key->name!=NULL) Rfree(key->name);
						if(key->part!=NULL)
						{
							for(x=0,part=key->part;x<key->numparts;
								++x,++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
						}
						Rfree(key->part);
					}
				}
				if(fields!=NULL)
				{
					for(y=0,field=fields;y<nofields;++y,++field)
					{
						for(x=0,d=RF->df;x<RF->num;++x,++d)
						{
							if(!RDAstrcmp(field->name,d->name)) break;
						}
						if(x<RF->num)
						{
						}
						if(field->name!=NULL) Rfree(field->name);
					}
				}
			}
		}
	}
}
void MakeSpecialFields(char *modx)
{
	char *libx=NULL;
	APPlib *filenames=NULL;
	int x,y,z;
	short engine=0,nofields=0,nokeys=0;
	NRDkey *keys=NULL,*key;
	NRDpart *part;
	NRDfield *fields=NULL,*field;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modx);
#endif
	filenames=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(filenames!=NULL)
	{
		for(z=0;z<filenames->numlibs;++z)
		{
			if(!GETBIN(modx,filenames->libs[z],&engine,&nofields,&fields,
				&nokeys,&keys)) 
			{
				if(keys!=NULL)
				{
					for(y=0,key=keys;y<nokeys;++y,++key)
					{
						if(key->name!=NULL) Rfree(key->name);
						if(key->part!=NULL)
						{
							for(x=0,part=key->part;x<key->numparts;
								++x,++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
						}
						Rfree(key->part);
					}
				}
				if(fields!=NULL)
				{
					for(y=0,field=fields;y<nofields;++y,++field)
					{
						switch(field->type)
						{
							case DATES:
								break;
							case TIMES:
								break;
							default:
								break;
						}
						if(field->name!=NULL) Rfree(field->name);
					}
				}
			}
		}
	}
}
#ifdef XXXX
int GetFieldLength(char *modname,char *filename,char *fieldname)
{
	short engine=0,nofields=0,nokeys=0;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	NRDfield *fields=NULL,*fldx=NULL;
	int y=0,z=0;
	int return_value=0;
	

	if(!GETBIN(modname,filename,&engine,&nofields,&fields,&nokeys,&keys))
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);                                      
					}
				}
				Rfree(keyx->part);
			}
		}
		Rfree(keys);
		if(fields!=NULL)
		{
			for(y=0,fldx=fields;y<nofields;++y,++fldx)
			{
				if(fldx->name!=NULL)
				{
					if(!RDAstrcmp(fldx->name,fieldname))
					{
						if(fldx->len>0)
						{
							return_value=fldx->len;
						} else {
							return_value=0;
						}
					}
					Rfree(fldx->name);
				}
			}
			Rfree(fields);
		}
	}
	return(return_value);
}
void CreateCOAs(char *modx)
{
	short q=0,z,x,y;
	short if_count=0,exp_count=0,desc_count=0,acc_count=0,no_parts=0;
	short struct_count=0,label_count=0,partlabel_count=0;
	short longest_length=0,exp_no_parts=0,rev_no_parts=0,bal_no_parts=0;
	RDAaccpart *part=NULL,*p=NULL;
	RDAacct *acc=NULL;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char temp[15001],*tmpexp=NULL,*tmpdesc=NULL;
	char *exp_label=NULL,*tmpstruct=NULL,*tmplabel=NULL,*tmppartlabel=NULL;
	int field_length=0;
	char prev_defused=FALSE;


	d=DFvirtualNEW(modx,"EXECUTED BY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("SECURITY","USERS","USER IDENTIFICATION");
	if(field_length<1)
	{
		d->type=VARIABLETEXT;
		d->length=0;
	} else {
		d->type=PLAINTEXT;
		d->length=field_length+15;
	}
	d->when=1;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Titled Executing User's Identification.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"CURRENT FISCAL MONTH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	d->type=SHORTV;
	d->length=2;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([SYSTEM DATE][1,2]=\"01\") THEN RETURN_VALUE=(1) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"02\") THEN RETURN_VALUE=(2) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"03\") THEN RETURN_VALUE=(3) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"04\") THEN RETURN_VALUE=(4) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"05\") THEN RETURN_VALUE=(5) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"06\") THEN RETURN_VALUE=(6) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"07\") THEN RETURN_VALUE=(7) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"08\") THEN RETURN_VALUE=(8) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"09\") THEN RETURN_VALUE=(9) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"10\") THEN RETURN_VALUE=(10) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"11\") THEN RETURN_VALUE=(11) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"12\") THEN RETURN_VALUE=(12) ",
		"\nFI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Current Fiscal Month with Pre-Year=0,January=1, February=2,... calculated from the [SYSTEM DATE] virtual field.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"CURRENT CALENDAR MONTH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	d->type=SHORTV;
	d->length=2;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([SYSTEM DATE][1,2]=\"01\") THEN RETURN_VALUE=(0) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"02\") THEN RETURN_VALUE=(1) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"03\") THEN RETURN_VALUE=(2) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"04\") THEN RETURN_VALUE=(3) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"05\") THEN RETURN_VALUE=(4) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"06\") THEN RETURN_VALUE=(5) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"07\") THEN RETURN_VALUE=(6) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"08\") THEN RETURN_VALUE=(7) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"09\") THEN RETURN_VALUE=(8) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"10\") THEN RETURN_VALUE=(9) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"11\") THEN RETURN_VALUE=(10) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"12\") THEN RETURN_VALUE=(11) ",
		"\nFI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Current Calendar Month with January=0, February=1,... calculated from the [SYSTEM DATE] virtual field .");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ORGANIZATIONAL ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	longest_length=(RDAstrlen(XPERT_SETUP->addr1)+RDAstrlen(XPERT_SETUP->addr2)+RDAstrlen(XPERT_SETUP->city)+RDAstrlen(XPERT_SETUP->state)+RDAstrlen(XPERT_SETUP->zip)+4);
	d->length=longest_length;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
	"IF(([ORGANIZATION ADDRESS #1]=\"\") AND",
	"([ORGANIZATION ADDRESS #2]=\"\") AND ",
	"([ORGANIZATION CITY]=\"\") AND ",
	"([ORGANIZATION STATE]=\"\") AND ",
	"([ORGANIZATION ZIP]=\"\"))",
	"THEN RETURN_VALUE=\"\"",
	"ELSE IF(([ORGANIZATION ADDRESS #1]=\"\") AND",
	"([ORGANIZATION ADDRESS #2]=\"\"))",
	"THEN RETURN_VALUE=([ORGANIZATION CITY]+\", \"+",
	"[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP])",
	"ELSE IF([ORGANIZATION ADDRESS #1]=\"\")",
	"THEN RETURN_VALUE=[ORGANIZATION ADDRESS #2]",
	"ELSE RETURN_VALUE=[ORGANIZATION ADDRESS #1]",
	"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Organization's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
        d=DFvirtualNEW(modx,"ORGANIZATIONAL ADDRESS 2");
        d->incvir=Rmalloc(sizeof(DFincvir));
        i=d->incvir;
        i->module=stralloc(modx);
        i->file=NULL;
        d->vtype=0;
        d->num=1;
        d->type=PLAINTEXT;
	longest_length=(RDAstrlen(XPERT_SETUP->addr1)+RDAstrlen(XPERT_SETUP->addr2)+RDAstrlen(XPERT_SETUP->city)+RDAstrlen(XPERT_SETUP->state)+RDAstrlen(XPERT_SETUP->zip)+4);
        d->length=longest_length;
        d->when=0;
        d->avail2rsrc=TRUE;
        memset(temp,0,15000);
        sprintf(temp,"%s %s %s %s %s %s %s %s %s %s%s %s %s",
                "IF(([ORGANIZATION ADDRESS #1]=\"\")",
                "AND ([ORGANIZATION ADDRESS #2]=\"\"))",
                "THEN RETURN_VALUE=\"\"",
                "ELSE IF(([ORGANIZATION ADDRESS #1]=\"\") AND",
                "([ORGANIZATION CITY]=\"\") AND",
                "([ORGANIZATION STATE]=\"\") AND",
                "([ORGANIZATION ZIP]=\"\"))",
                "THEN RETURN_VALUE=(\"\")",
                "ELSE IF([ORGANIZATION ADDRESS #2]=\"\")",
                "THEN RETURN_VALUE=([ORGANIZATION CITY]+\", \"+",
                "[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP])",
                "ELSE RETURN_VALUE=[ORGANIZATION ADDRESS #2]",
                "FI FI FI");
        d->expression=stralloc(temp);
        memset(temp,0,15000);
        sprintf(temp,"This virtual field formats the Organizations's Address line 2.");
        d->desc=stralloc(temp);
        memset(temp,0,15000);
        writeDFvirtual(modx,d);
        if(d!=NULL) FreeDFvirtual(d);
        d=DFvirtualNEW(modx,"ORGANIZATIONAL ADDRESS 3");
        d->incvir=Rmalloc(sizeof(DFincvir));
        i=d->incvir;
        i->module=stralloc(modx);
        i->file=NULL;
        d->vtype=0;
        d->num=1;
        d->type=PLAINTEXT;
	longest_length=(RDAstrlen(XPERT_SETUP->addr1)+RDAstrlen(XPERT_SETUP->addr2)+RDAstrlen(XPERT_SETUP->city)+RDAstrlen(XPERT_SETUP->state)+RDAstrlen(XPERT_SETUP->zip)+4);
        d->length=longest_length;
        d->when=0;
        d->avail2rsrc=TRUE;
        memset(temp,0,15000);
        sprintf(temp,"%s %s %s %s %s %s %s%s %s",
                "IF(([ORGANIZATION ADDRESS #1]=\"\")",
                "OR ([ORGANIZATION ADDRESS #2]=\"\")",
                "OR (([ORGANIZATION CITY]=\"\") AND",
                "([ORGANIZATION STATE]=\"\") AND",
                "([ORGANIZATION ZIP]=\"\")))",
                "THEN RETURN_VALUE=\"\"",
                "ELSE RETURN_VALUE=([ORGANIZATION CITY]+\", \"+",
                "[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP])",
                "FI");
        d->expression=stralloc(temp);
        memset(temp,0,15000);
        sprintf(temp,"This virtual field formats the Organizations's Address line 3.");
        d->desc=stralloc(temp);
        memset(temp,0,15000);
        writeDFvirtual(modx,d);
        if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ACCOUNT TYPE");
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=SHORTV;
	d->length=1;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"IF([FINMGT][FINEACC][ACCOUNT CODE]) THEN RETURN_VALUE=0 ELSE IF([FINMGT][FINRACC][ACCOUNT CODE]) THEN RETURN_VALUE=1 ELSE RETURN_VALUE=2 FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Account Type.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
#ifndef PARTIALLIST
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
				y<EXPENDITURE_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+23);
						sprintf(vname,"EXPENDITURE CODE %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINEACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+44);
						sprintf(d->desc,"%s Part of the %s Expenditure Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+31);
						sprintf(vname,"EXPENDITURE DESCRIPTION %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINEACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+65);
						sprintf(d->desc,"Description For %s Part of the %s Expenditure Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(35);
			sprintf(vname,"EXPENDITURE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			if(tmpexp!=NULL) Rfree(tmpexp);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			vname=Rmalloc(20);
			sprintf(vname,"EXPENDITURE LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=0;
						for(z=0,p=acc->acct;z<x;++z,++p) 
							acc_count+=p->length+1;
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
							desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(35);
				sprintf(vname,"EXPENDITURE CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINEACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Expenditure Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(45);
				sprintf(vname,"EXPENDITURE DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINEACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Expenditure Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			for(y=0,acc=REVENUE_ACCOUNT->codetype;
				y<REVENUE_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+19);
						sprintf(vname,"REVENUE CODE %s %s",acc->name,
							part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINRACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+44);
						sprintf(d->desc,"%s Part of the %s Revenue Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+26);
						sprintf(vname,"REVENUE DESCRIPTION %s %s",
							acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINRACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+55);
						sprintf(d->desc,"Description For %s Part of the %s Revenue Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=REVENUE_ACCOUNT->codetype;
				z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(30);
			sprintf(vname,"REVENUE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Revenue Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			vname=Rmalloc(15);
			sprintf(vname,"REVENUE LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Revenue Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=0;
						for(z=0,p=acc->acct;z<x;++z,++p) 
							acc_count+=p->length+1;
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(31);
				sprintf(vname,"REVENUE CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINRACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Revenue Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(41);
				sprintf(vname,"REVENUE DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINRACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Revenue Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+
							RDAstrlen(acc->name)+25);
						sprintf(vname,"BALANCE SHEET CODE %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINBACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+60);
						sprintf(d->desc,"%s Part of the %s Balance Sheet Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+32);
						sprintf(vname,"BALANCE SHEET DESCRIPTION %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINBACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+58);
						sprintf(d->desc,"Description For %s Part of the %s Balance Sheet Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(35);
			sprintf(vname,"BALANCE SHEET ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			if(tmpexp!=NULL) Rfree(tmpexp);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Balance Sheet Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			vname=Rmalloc(20);
			sprintf(vname,"BALANCE SHEET LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Balance Sheet Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=0;
						for(z=0,p=acc->acct;z<x;++z,++p) 
							acc_count+=p->length+1;
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(40);
				sprintf(vname,"BALANCE SHEET CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINBACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Balance Sheet Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(50);
				sprintf(vname,"BALANCE SHEET DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINBACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Balance Sheet Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
#endif /* End Partial list */
	no_parts=0;
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_no_parts=acc->num;
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=REVENUE_ACCOUNT->codetype;
				z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				rev_no_parts=acc->num;
			}
		}
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				bal_no_parts=acc->num;
			}
		}
	}
	for(x=0;x<no_parts;++x)
	{
		exp_count=0;
		desc_count=0;
		struct_count=0;
		label_count=0;
		partlabel_count=0;
		if_count=0;
		if(tmpexp!=NULL) Rfree(tmpexp);
		if(tmpdesc!=NULL) Rfree(tmpdesc); 
		if(tmpstruct!=NULL) Rfree(tmpstruct); 
		if(tmplabel!=NULL) Rfree(tmplabel); 
		if(tmppartlabel!=NULL) Rfree(tmppartlabel); 
		longest_length=0;
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
				tmpexp=stralloc(temp);
				tmpdesc=stralloc(temp);
				if(x==0)
				{
				tmplabel=stralloc(temp);
				tmpstruct=stralloc(temp);
				}
				tmppartlabel=stralloc(temp);
				exp_count+=RDAstrlen(temp);
				desc_count+=RDAstrlen(temp);
				if(x==0)
				{
				label_count+=RDAstrlen(temp);
				struct_count+=RDAstrlen(temp);
				}
				partlabel_count+=RDAstrlen(temp);
				if_count=0;
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<exp_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<exp_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<exp_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<exp_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<exp_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(x==0)
				{
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				if(tmpstruct!=NULL)
				{
					tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
					sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
					struct_count+=RDAstrlen(temp)+5;
				} else {
					tmpstruct=stralloc(temp);
					struct_count+=RDAstrlen(temp);
				}
				}
				if(tmppartlabel!=NULL)
				{
					tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+6);
					sprintf(&tmppartlabel[partlabel_count],"ELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+5;
				} else {
					tmppartlabel=stralloc(temp);
					partlabel_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
							acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<rev_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<rev_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<rev_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<rev_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<rev_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=2) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(x==0)
				{
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				if(tmpstruct!=NULL)
				{
					tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
					sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
					struct_count+=RDAstrlen(temp)+5;
				} else {
					tmpstruct=stralloc(temp);
					struct_count+=RDAstrlen(temp);
				}
				}
				if(tmppartlabel!=NULL)
				{
					tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+6);
					sprintf(&tmppartlabel[partlabel_count],"ELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+5;
				} else {
					tmppartlabel=stralloc(temp);
					partlabel_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<bal_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<bal_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<bal_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<bal_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<bal_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(tmpexp!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
			sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpdesc!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+16);
			sprintf(&tmpdesc[desc_count],"ELSE %s FI FI FI\n",temp);
		}
		if(x==0)
		{
		if(tmplabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
			sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpstruct!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+16);
			sprintf(&tmpstruct[struct_count],"ELSE %s FI FI FI\n",temp);
		}
		}
		if(tmppartlabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+16);
			sprintf(&tmppartlabel[partlabel_count],"ELSE %s FI FI FI\n",temp);
		}
		memset(temp,0,15001);
		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d CODE",x);
		d=DFvirtualNEW(modx,stemp);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=longest_length;
		d->when=0;
		if(x==0)
			d->avail2rsrc=TRUE;
		else
			d->avail2rsrc=FALSE;
		if(tmpexp!=NULL)
		{
			d->expression=stralloc(tmpexp);
			Rfree(tmpexp);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code Dimension %d representing all account type and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d DESCRIPTION",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		if(tmpdesc!=NULL)
		{
			d->expression=stralloc(tmpdesc);
			Rfree(tmpdesc);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Description for Code Dimension %d representing all accounts and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=SHORTV;
		d->length=2;
		d->when=0;
		sprintf(stemp,"%d",x);
		d->expression=stralloc(stemp);
		memset(stemp,0,101);
		memset(temp,0,15001);
		sprintf(temp,"Represents Dimension number %d for use in the Chart of Accounts.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
		memset(stemp,0,101);
		if(x==0)
		{
		memset(stemp,0,101);
		sprintf(stemp,"ACCOUNT CODE LABEL");
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		d->avail2rsrc=TRUE;
		if(tmplabel!=NULL)
		{
			d->expression=stralloc(tmplabel);
			Rfree(tmplabel);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Account Label for each Account Type.");
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
		memset(stemp,0,101);
		sprintf(stemp,"ACCOUNT CODE STRUCTURE");
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		d->avail2rsrc=TRUE;
		if(tmpstruct!=NULL)
		{
			d->expression=stralloc(tmpstruct);
			Rfree(tmpstruct);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Account Code Structure for each Account Type.");
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
		}
		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d LABEL",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		if(tmppartlabel!=NULL)
		{
			d->expression=stralloc(tmppartlabel);
			Rfree(tmppartlabel);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Account Part Name for Dimension %d or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
	}
}
#endif /* XXXX */
void BNKRECCreate()
{
}
void CONVERSIONCreate()
{
}
static void getmonths(void)
{
	if(months!=NULL) freeapplib(months);
	months=APPlibNEW();
	addAPPlib(months,"JANUARY");
	addAPPlib(months,"FEBRUARY");
	addAPPlib(months,"MARCH");
	addAPPlib(months,"APRIL");
	addAPPlib(months,"MAY");
	addAPPlib(months,"JUNE");
	addAPPlib(months,"JULY");
	addAPPlib(months,"AUGUST");
	addAPPlib(months,"SEPTEMBER");
	addAPPlib(months,"OCTOBER");
	addAPPlib(months,"NOVEMBER");
	addAPPlib(months,"DECEMBER");
}
char *getmonth(int monthno)
{
	char *returnstr=NULL;
 
	if(monthno>11)
	{
		returnstr=months->libs[monthno-12];
	} else {
		returnstr=months->libs[monthno];
	}
	return(returnstr);
}
char *makepretodateexpres(char *module,char *file,char *endfieldname)
{
	int x=0,y=0;
	char *expression=NULL,*month=NULL,*endmonth=NULL;
	char temp[15001];

	memset(temp,0,15000);
	if(!RDAstrcmp(module,"FINMGT") && !RDAstrcmp(file,"FINBYR"))
	{
		sprintf(temp,"IF([CURRENT FISCAL MONTH]=0) THEN RETURN_VALUE=[%s][%s][BEGINNING BALANCE %s]+[%s][%s][PRE-YEAR %s]",module,file,endfieldname,module,file,endfieldname);
	} else {
		sprintf(temp,"IF([CURRENT FISCAL MONTH]=0) THEN RETURN_VALUE=[%s][%s][PRE-YEAR %s]",module,file,endfieldname);
	}
	expression=stralloc(temp);
	for(x=0;x<12;++x)
	{
		memset(temp,0,15000);
		if(!RDAstrcmp(module,"FINMGT") && !RDAstrcmp(file,"FINBYR"))
		{
			sprintf(temp,"\nELSE IF([CURRENT FISCAL MONTH]=%d) THEN RETURN_VALUE=[%s][%s][BEGINNING BALANCE %s]+[%s][%s][PRE-YEAR %s]",(x+1),module,file,endfieldname,module,file,endfieldname);
		} else {
			sprintf(temp,"\nELSE IF([CURRENT FISCAL MONTH]=%d) THEN RETURN_VALUE=[%s][%s][PRE-YEAR %s]",(x+1),module,file,endfieldname);
		}
		expression=Rrealloc(expression,RDAstrlen(expression)+RDAstrlen(temp)+1);
		strcat(expression,temp);
		endmonth=getmonth(x);
		for(y=0;;++y)
		{
			memset(temp,0,15000);
			month=getmonth(finsetup->month+y);
			sprintf(temp,"+[%s][%s][%s %s]",module,file,month,endfieldname);
			expression=Rrealloc(expression,RDAstrlen(expression)+RDAstrlen(temp)+1);
			strcat(expression,temp);
			if(!RDAstrcmp(month,endmonth)) break;
		}
	}
	memset(temp,0,15000);
	if(!RDAstrcmp(module,"FINMGT") && !RDAstrcmp(file,"FINBYR"))
	{
		sprintf(temp,"\nELSE IF([CURRENT FISCAL MONTH]=%d) THEN RETURN_VALUE=[%s][%s][BEGINNING BALANCE %s]+[%s][%s][PRE-YEAR %s]",(x+1),module,file,endfieldname,module,file,endfieldname);
	} else {
		sprintf(temp,"\nELSE IF([CURRENT FISCAL MONTH]=%d) THEN RETURN_VALUE=[%s][%s][PRE-YEAR %s]",(x+1),module,file,endfieldname);
	}
	expression=Rrealloc(expression,RDAstrlen(expression)+RDAstrlen(temp)+1);
	strcat(expression,temp);
	endmonth=getmonth(finsetup->month+11);
	for(y=0;;++y)
	{
		memset(temp,0,15000);
		month=getmonth(finsetup->month+y);
		sprintf(temp,"+[%s][%s][%s %s]",module,file,month,endfieldname);
		expression=Rrealloc(expression,RDAstrlen(expression)+RDAstrlen(temp)+1);
		strcat(expression,temp);
		if(!RDAstrcmp(month,endmonth))
		{
			memset(temp,0,15000);
			sprintf(temp,"+[%s][%s][POST YEAR %s]",module,file,endfieldname);
			expression=Rrealloc(expression,RDAstrlen(expression)+RDAstrlen(temp)+1);
			strcat(expression,temp);
			break;
		}
	}
	expression=Rrealloc(expression,RDAstrlen(expression)+44+1);
	strcat(expression,"\nFI FI FI FI FI FI FI FI FI FI FI FI FI FI ");
	memset(temp,0,15000);
	return(expression);
}
void okcreate(char *module)
{
	if(!RDAstrcmp(module,"BFTMGT"))
	{
		PAYROLLCreate1("BFTMGT");
		PAYROLLCreate2("BFTMGT");
		CreateCOAs("BFTMGT");
	} else if(!RDAstrcmp(module,"BNKREC"))
	{
		BNKRECCreate();
		CreateCOAs("BNKREC");
	} else if(!RDAstrcmp(module,"CONVERSION"))
	{
		CONVERSIONCreate();
		CreateCOAs("CONVERSION");
	} else if(!RDAstrcmp(module,"EMPABS"))
	{
		PAYROLLCreate1("EMPABS");
		PAYROLLCreate2("EMPABS");
		CreateCOAs("EMPABS");
	} else if(!RDAstrcmp(module,"FINMGT"))
	{
		FINMGTCreateEV1();
		FINMGTCreateEV2();
		FINMGTCreateRV1();
		FINMGTCreateRV2();
		FINMGTCreateBV1();
		FINMGTCreateBV2();
		CreateCOAs("FINMGT");
	} else if(!RDAstrcmp(module,"IIDMGT"))
	{
		PAYROLLCreate1("IIDMGT");
		PAYROLLCreate2("IIDMGT");
	} else if(!RDAstrcmp(module,"LVEMGT"))
	{
		LVEMGTCreate();
		PAYROLLCreate1("LVEMGT");
		PAYROLLCreate2("LVEMGT");
	} else if(!RDAstrcmp(module,"OCCTAX"))
	{
		OCCTAXCreate();
		CreateCOAs("OCCTAX");
	} else if(!RDAstrcmp(module,"PAYROLL"))
	{
		PAYROLLCreate1("PAYROLL");
		PAYROLLCreate2("PAYROLL");
		CreateCOAs("PAYROLL");
	} else if(!RDAstrcmp(module,"POSTRK"))
	{
		PAYROLLCreate1("POSTRK");
		PAYROLLCreate2("POSTRK");
		CreateCOAs("POSTRK");
	} else if(!RDAstrcmp(module,"PRSNNL"))
	{
		PAYROLLCreate1("PRSNNL");
		PAYROLLCreate2("PRSNNL");
		CreateCOAs("PRSNNL");
	} else if(!RDAstrcmp(module,"PURORD"))
	{
		PURORDCreate();
		CreateCOAs("PURORD");
	} else if(!RDAstrcmp(module,"RLSTMGT"))
	{
		RLSTMGTCreate();
		CreateCOAs("RLSTMGT");
	} else if(!RDAstrcmp(module,"SUBMGT"))
	{
		PAYROLLCreate1("SUBMGT");
		PAYROLLCreate2("SUBMGT");
		CreateCOAs("SUBMGT");
	} else if(!RDAstrcmp(module,"VENPMT"))
	{
		VENPMTCreate();
		CreateCOAs("VENPMT");
	} else if(!RDAstrcmp(module,"TAXCLT"))
	{
		CreateCOAs("TAXCLT");
	} else {
		prterr("Error: Module [%s] not found for creating/updating Virtual Fields.",module);
	}
}
void okcreatecb(RDArsrc *mainrsrc)
{
	int selected_module=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected_module)) return;
	okcreate(modlst->libs[selected_module]);
	
}
void quitvirt(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(modlst!=NULL) freeapplib(modlst);
	if(finsetup!=NULL) free_finmgt(finsetup);
	if(months!=NULL) freeapplib(months);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;
	char *module=NULL;

	if(argc>2 && (!RDAstrcmp(argv[2],"CREATE VIRTUALS")))
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE VIRTUAL FIELDS"))
		{
			return;
		}
		finsetup=RDAfinmgtNEW();
		if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
		module=stralloc(argv[1]);
		getmonths();
		okcreate(module);
		if(module!=NULL) Rfree(module);
		quitvirt(mainrsrc);
		ShutdownSubsystems();
		std::exit;
	} else {
		if(InitializeSubsystems(argc,argv,modulename,"MAKE VIRTUAL FIELDS")) 
		{
			return;
		}
		finsetup=RDAfinmgtNEW();
		if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
		mainrsrc=RDArsrcNEW(modulename,"MAKE VIRTUAL FIELDS");
		modlst=APPlibNEW();
		addAPPlib(modlst,"BFTMGT");
		addAPPlib(modlst,"BNKREC");
		addAPPlib(modlst,"CONVERSION");
		addAPPlib(modlst,"EMPABS");
		addAPPlib(modlst,"FINMGT");
		addAPPlib(modlst,"IIDMGT");
		addAPPlib(modlst,"LVEMGT");
		addAPPlib(modlst,"OCCTAX");
		addAPPlib(modlst,"PAYROLL");
		addAPPlib(modlst,"POSTRK");
		addAPPlib(modlst,"PRSNNL");
		addAPPlib(modlst,"PURORD");
		addAPPlib(modlst,"RLSTMGT");
		addAPPlib(modlst,"SUBMGT");
		addAPPlib(modlst,"TAXCLT");
		addAPPlib(modlst,"VENPMT");
		if(argc>1)
		{
			if((x=FINDAPPLIBELEMENT(modlst,argv[1]))==(-1))
			{
				x=0;
			}
		}
		addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,NULL,modlst->numlibs,
			&modlst->libs,NULL);
		getmonths();
		addbtnrsrc(mainrsrc,"SELECT",TRUE,okcreatecb,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitvirt,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitvirt,NULL,TRUE);
	}
}
