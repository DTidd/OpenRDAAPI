/* lveale.c - Leave Managment's Add Leave Earned */
#ifndef WIN32
#define __NAM__ "lveale.lnx"
#endif
#ifdef WIN32
#define __NAM__ "lveale.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <mix.h>
#include <ldval.h>
#include <rptgen.h>
/*ADDLIB rpt */
/*ADDLIB add */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


static APPlib *lvedesclist=NULL;
static char *module="LVEMGT",**selecttype=NULL;
static char *select_expression=NULL,*lvedescid=NULL;
static char *earned_expression=NULL;
static short lvemstr=(-1),prsnnl=(-1),lvedesc=(-1),lvetran=(-1),lvetpms=(-1);
static short payjob=(-1),lvejob=(-1),posmst=(-1);
static int select_type=0,TRANSACTION_COUNT=0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static RDAfinmgt *fmgt=NULL;
static char *reportname=NULL;
static void save_record(RDArsrc *,short,DBsort *);
static void quit_record(RDArsrc *,DBsort *);
static void save_record_up(RDArsrc *,DBsort *);
static void save_record_nup(RDArsrc *,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *);
DBsort *dbsort=NULL;
static void selectm(short,void *,int,char **,DBsort *);
static void doexit_test(MakeBrowseList *);
static void doexit(MakeBrowseList *);
static void saveexit(RDArsrc *,MakeBrowseList *);

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static void LveSelSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn;
	int y;
	RDAvirtual *v;
	DBsort *dbsort;
	RDArsrc *r;

	r=p->r;
	dbsort=p->a;
	if(diageval)
	{
		prterr("DIAG LveSelSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
/* BREAK_HERE file fields */
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	fieldname=stripfieldname(*tmp);
	if(fieldname!=NULL)
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+
			RDAstrlen(fieldname)+6))
		{
			temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+
				RDAstrlen(fieldname)+7);
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				occurrence=Ratoi(gn);
				gn=strtok(NULL,",]");
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
		if(fileno!=(-1))
		{
			field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
		} else field=NULL;
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
					temp=stralloc(field->data.string_value);
					str_length=(temp!=NULL?RDAstrlen(temp):0);
					if(length<1) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(!isEMPTY(temp) && str_length>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(temp))
					{
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(*field->data.short_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.short_value);
					} else {
						sprintf(stemp,"%d",*field->data.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					if(*field->data.integer_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.integer_value);
					} else {
						sprintf(stemp,"%d",*field->data.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(*field->data.float_value<0)
					{
						sprintf(stemp,"(%f)",*field->data.float_value);
					} else {
						sprintf(stemp,"%f",*field->data.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(stemp,"%d",(*field->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(*tmp,"%c",*field->data.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		} else {
			prterr("Error Field [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
	} else if(GLOBALSubData(tmp,modulename)==0)
	{
/* OK, found in Global Virtuals */
/* BREAK_HERE virtual fields */
	} else { 
		for(y=0,v=r->virtual;y<r->num;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<r->num)
		{
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						temp=EVALUATEstr(v->expression,LveSelSubData,p);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=temp;
						temp=NULL;
						if(v->value.string_value!=NULL)
						{
							v->dlen=RDAstrlen(v->value.string_value)+1;
							if(v->dlen>v->len && v->len>0)
							{
								v->value.string_value[v->len]=0;
							}
						}
					}
					if(RDAstrlen(*tmp)>(RDAstrlen(v->name)+2))
					{
						temp=*tmp+(RDAstrlen(v->name)+3);
						gn=strtok(temp,",]");
						if(gn!=NULL)
						{
							start=Ratoi(gn);
							gn=strtok(NULL,"]");
							length=Ratoi(gn);
						}
						if(start<1) start=1;

					}
					if(length==0) length=v->dlen;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					temp=stralloc(v->value.string_value);
					if(!isEMPTY(temp) && v->dlen>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<v->dlen && !isEMPTY(temp))
					{
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=EVALUATEsht(v->expression,LveSelSubData,p);
					}
					if(*v->value.short_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.short_value);
					} else {
						sprintf(stemp,"%d",*v->value.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.integer_value=EVALUATEint(v->expression,LveSelSubData,p);
					}
					if(*v->value.integer_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.integer_value);
					} else {
						sprintf(stemp,"%d",*v->value.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.float_value=EVALUATEdbl(v->expression,LveSelSubData,p);
						if(v->type==DOLLARS ||
							v->type==DOUBLEV ||
							v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}	
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,LveSelSubData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(stemp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,LveSelSubData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(*tmp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		} else {
			field=FLDNRD(dbsort->fileno,modulename);
			if(field!=NULL)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
						{
							temp=*tmp+RDAstrlen(modulename)+3;
							gn=strtok(temp,",]");
							if(gn!=NULL)
							{
								start=Ratoi(gn);
								gn=strtok(NULL,"]");
								length=Ratoi(gn);
							}
							if(start<1) start=1;

						}
						str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
						if(length==0) length=str_length;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if(field->data.string_value!=NULL && str_length>length)
						{
							temp=stralloc(field->data.string_value);
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(start-1+length))
								temp[start-1+length]=0;
							if(RDAstrlen(temp)>(start-1+length))
								temp[start-1+length]=0;
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} else {
							sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(*field->data.short_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.short_value);
						} else {
							sprintf(stemp,"%d",*field->data.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SLONGV:
					case SCROLLEDLIST:
					case LONGV:
						if(*field->data.integer_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.integer_value);
						} else {
							sprintf(stemp,"%d",*field->data.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case DECIMALV:
					case SDECIMALV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SDOUBLEV:
					case DOLLARS:
					case DOUBLEV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%.0f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%.0f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%c",*field->data.string_value);
						break;
					default:
						prterr("Error field type %d is invalid for %s.",
							field->type,field->name); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=NULL;
						break;
				}
			} else {
/* BREAK_HERE Scrolled Lists */
				if(ScrolledListSubData(tmp,r)==(-1))
				{
					prterr("Error Field [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
				}
			}
		}
	}
	Rfree(modulename);
	}
	if(diageval)
	{
		prterr("DIAG LveSelSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
APPlib *makefieldvallist()
{
	APPlib *list=NULL,*virflist=NULL;
	RDArsrc *tmprsrc=NULL;
	short dumb=0,addvf=FALSE;
	char *libx=NULL;
	int y,x,z;
	DFincvir *i,*a;
	DFvirtual *d;

	list=APPlibNEW();
	AddFileFields(list,prsnnl);
	AddFileFields(list,payjob);
	AddFileFields(list,posmst);
	AddFileFields(list,lvejob);
	AddFileFields(list,lvemstr);
	AddFileFields(list,lvedesc);
	AddFileFields(list,lvetpms);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,dumb);
	addDFincvir(tmprsrc,module,"LVEMSTR",NULL,dumb);
	addDFincvir(tmprsrc,module,"LVEJOB",NULL,dumb);
	addDFincvir(tmprsrc,module,"LVEDSC",NULL,dumb);
	addDFincvir(tmprsrc,module,"LVETPMS",NULL,dumb);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/LVEMGT.VIR",CURRENTDIRECTORY);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(list,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	loadglobals(list);
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
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
void select_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE;
	char deleteflag=FALSE,true_flag=TRUE,*e=NULL;
	char *perid=NULL,*lvedescid=NULL,*curdate=NULL,*curtime=NULL;
	char *curdate10=NULL;
	double earnings_amt=0.0,total=0.0;
	short earning_type=0;
	int x=0,numfields=0;
	NRDfield *fields=NULL,*field=NULL;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Updating Leave Masters",NULL);
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
		numfields=NUMFLDS(dbsort->fileno);
		fields=FLDPOINTER(dbsort->fileno);
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			earnings_amt=0.0;
			earning_type=0;
			total=0.0;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&earnings_amt);
			FINDFLDGETSHORT(dbsort->fileno,"TYPE",&earning_type);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(curdate!=NULL) Rfree(curdate);
			if(curdate10!=NULL) Rfree(curdate10);
			curdate=GETCURRENTDATE();
			curdate10=GETCURRENTDATE10();
			if(!deleteflag && earning_type==0)
			{
				for(x=0,field=fields;x<numfields;++x,++field)
				{
					if(!RDAstrcmp(field->name,"PERSONNEL IDENTIFICATION"))
					{
						FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&perid);
						FINDFLDSETSTRING(lvetran,"PERSONNEL IDENTIFICATION",perid);
					} else if(!RDAstrcmp(field->name,"LEAVE DESCRIPTION IDENTIFICATION"))
					{
						FINDFLDGETSTRING(dbsort->fileno,"LEAVE DESCRIPTION IDENTIFICATION",&lvedescid);
						FINDFLDSETSTRING(lvetran,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid);
					} else if(!RDAstrcmp(field->name,"AMOUNT"))
					{
						FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&earnings_amt);
						FINDFLDSETDOUBLE(lvetran,"AMOUNT",earnings_amt);
					} else if(!RDAstrcmp(field->name,"ENTRY DATE"))
					{
						if(field->len>8)
						{
						FINDFLDSETSTRING(lvetran,field->name,curdate10);
						} else {
						FINDFLDSETSTRING(lvetran,field->name,curdate);
						}
					} else if(!RDAstrcmp(field->name,"ENTRY TIME"))
					{
						FINDFLDSETSTRING(lvetran,field->name,curtime);
					} else {
						COPYFIELD(dbsort->fileno,lvetran,field->name);
					}
				}
				if(earnings_amt!=0.0)
				{
					WRTTRANSsec(lvetran,1,"TRANSACTION NUMBER",NULL);
					++TRANSACTION_COUNT;
				}
				FINDFLDSETSTRING(lvemstr,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETSTRING(lvemstr,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid);
				LOCNRDFILE(lvemstr);
				if(!EQLNRDsec(lvemstr,1))
				{
					prev=RDATDataNEW(lvemstr);
					FINDFLDGETDOUBLE(lvemstr,"UNPROCESSED EARNINGS",&total);
					total+=earnings_amt;
					FINDFLDSETDOUBLE(lvemstr,"UNPROCESSED EARNINGS",total);
					WRTTRANSsec(lvemstr,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				} else {
					KEYNRD(lvemstr,1);
					FINDFLDSETCHAR(lvemstr,"ACTIVE",true_flag);
					FINDFLDGETDOUBLE(lvemstr,"UNPROCESSED EARNINGS",&total);
					total+=earnings_amt;
					FINDFLDSETDOUBLE(lvemstr,"UNPROCESSED EARNINGS",total);
					WRTTRANSsec(lvemstr,0,NULL,NULL);
				}
				UNLNRDFILE(lvemstr);
				FINDFLDSETSTRING(lvejob,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETSTRING(lvejob,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid);
				COPYFIELD(lvetran,lvejob,"JOB NUMBER");
				LOCNRDFILE(lvejob);
				if(!EQLNRDsec(lvejob,1))
				{
					prev=RDATDataNEW(lvejob);
					FINDFLDGETDOUBLE(lvejob,"UNPROCESSED EARNINGS",&total);
					total+=earnings_amt;
					FINDFLDSETDOUBLE(lvejob,"UNPROCESSED EARNINGS",total);
					WRTTRANSsec(lvejob,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				} else {
					KEYNRD(lvejob,1);
					FINDFLDSETCHAR(lvejob,"ACTIVE",true_flag);
					FINDFLDGETDOUBLE(lvejob,"UNPROCESSED EARNINGS",&total);
					total+=earnings_amt;
					FINDFLDSETDOUBLE(lvejob,"UNPROCESSED EARNINGS",total);
					WRTTRANSsec(lvejob,0,NULL,NULL);
				}
				UNLNRDFILE(lvejob);
			}
			if(tmprsrc!=NULL) 
				update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			if(curdate!=NULL) Rfree(curdate);
			if(curdate10!=NULL) Rfree(curdate10);
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(curtime!=NULL) Rfree(curtime);
		if(curdate!=NULL) Rfree(curdate);
		if(perid!=NULL) Rfree(perid);
		if(lvedescid!=NULL) Rfree(lvedescid);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field,*FIELDS=NULL;
	int x=0,numfields=0;
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"ADD LEAVE EARNED SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=stralloc(d->expression);
		} else csortsequence->expression=NULL;
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
		addDBkeypart(dbsrt,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	numfields=NUMFLDS(lvetran);
	FIELDS=FLDPOINTER(lvetran);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!strncmp(field->name,"DOCK",4))
			{
			} else if(!RDAstrcmp(field->name,"STARTING DATE"))
			{
			} else if(!RDAstrcmp(field->name,"ENDING DATE"))
			{
			} else if(!RDAstrcmp(field->name,"TYPE"))
			{
			} else if(!strncmp(field->name,"UPDATED",7))
			{
			} else {
				addDBfield(dbsrt,field->name,field->type,field->len);
			}
		}
	}
	key=KEYNUM(lvetran,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt,RDArsrc *mainrsrc,char *perid,
	char *lvedescid1,double default_amt,char *descript,int jobno,
	PassableStruct *p)
{
	char deleteflag=FALSE;
	char *date=NULL,*timex=NULL;
	NRDfield *field=NULL;

	ZERNRD(dbsrt->fileno);
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,LveSelSubData,p);
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
	FINDFLDSETSTRING(dbsrt->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(dbsrt->fileno,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	FINDFLDSETINT(dbsrt->fileno,"JOB NUMBER",jobno);
	FINDFLDSETDOUBLE(dbsrt->fileno,"AMOUNT",default_amt);
	FINDFLDSETSTRING(dbsrt->fileno,"DESCRIPTION",descript);
	FINDFLDSETCHAR(dbsrt->fileno,"SOURCE USER VERIFIED",FALSE);
	FINDFLDSETSTRING(dbsrt->fileno,"SOURCE USER",USERLOGIN);
	timex=GETCURRENTTIME();
	field=FLDNRD(dbsrt->fileno,"ENTRY DATE");
	if(field!=NULL)
	{
		if(field->len>8) date=GETCURRENTDATE10();
			else date=GETCURRENTDATE();
		FINDFLDSETSTRING(dbsrt->fileno,"ENTRY DATE",date);
	} else {
		date=GETCURRENTDATE();
		FINDFLDSETSTRING(dbsrt->fileno,"ENTRY DATE",date);
	}
	FINDFLDSETSTRING(dbsrt->fileno,"ENTRY TIME",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	WRTNRD(dbsrt->fileno,1,"TRANSACTION NUMBER");
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	selectm(0,targetkey,0,NULL,blist->funcargs);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	selectm(2,NULL,0,NULL,blist->funcargs);
} 
static void doexit(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	DBsort *dbsort=NULL;

	if(blist!=NULL)
	{
		dbsort=blist->funcargs;
		brsrc=blist->mainrsrc;
		srsrc=blist->searchrsrc;
		drsrc=blist->definelist;
		FREE_BROWSE_EXIT(blist);
		if(drsrc!=NULL)
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
		}
		if(srsrc!=NULL)
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			free_rsrc(brsrc);
		}
		if(mbl_trans!=NULL) mbl_trans=NULL;
		if(dbsort!=NULL)
		{
			CLSDBsort(dbsort);
			FreeDBsort(dbsort);
		}
	}
}
static void doexit_test(MakeBrowseList *blist)
{
        DBsort *dbsort=NULL;

	if(blist!=NULL)
	{
		dbsort=blist->funcargs;
		if(dbsort!=NULL)
		{
			WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!",
				"Are you sure that you want to quit without processing the Leave Earnings Records?",
				doexit,NULL,FALSE,blist);
		} else {
			doexit(blist);
		}
	}
}
static void saveexit(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	DBsort *dbsort=NULL;

	if(blist!=NULL)
	{
		dbsort=blist->funcargs;
		brsrc=blist->mainrsrc;
		drsrc=blist->definelist;
		srsrc=blist->searchrsrc;
		FREE_BROWSE_EXIT(blist);
		if(drsrc!=NULL) 
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
		}
		if(srsrc!=NULL) 
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			free_rsrc(brsrc);
		}
		if(WindowCount<=2) select_backend(dbsort);
	}
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
void selectlve(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,etest=FALSE,dodiag=FALSE,testfile=FALSE,keyrange=FALSE;
	double default_amt=0.0;
	char deleteflag=FALSE,active=FALSE,*sortname=NULL,lveallowed=FALSE;
	char *perid=NULL,*lvetypeid=NULL,*descript=NULL,*e=NULL;
	int selected_lvedesc=0,x=0,jobno=0;
	long size_of_file=0;
	RDAvirtual *v=NULL;
	PassableStruct *p=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",&selected_lvedesc);
	FINDRSCGETINT(mainrsrc,"SELECT TYPE",&select_type);
	if(lvedescid!=NULL) Rfree(lvedescid);
	if(RDAstrcmp(lvedesclist->libs[selected_lvedesc],"No Leave Descriptions Available"))
	{
		lvedescid=stralloc(lvedesclist->libs[selected_lvedesc]);
	} else return;
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&select_expression);
	FINDRSCGETSTRING(mainrsrc,"EARNED AMOUNT EXPRESSION",&earned_expression);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(prsnnl,rs);
	ReadRangeScreen(payjob,rs);
	ReadRangeScreen(posmst,rs);
	if(mainrsrc->virtual!=NULL)
	{
		for(x=0,v=mainrsrc->virtual;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(payjob,"DIAGNOSTIC SCREEN",module,
		"Selecting Personnel Jobs",NULL);
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
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=dbsort;
	ZERNRD(payjob);
/*
	ef=FRSNRDsec(payjob,1);
*/
        keyrange=setfilekeyFROMrangersrc(payjob,1,rs);
	if(keyrange)
	{
		ef=ADVGTENRDsec(payjob,1,LveSelSubData,p);
	} else {
		ef=ADVBEGNRDsec(payjob,LveSelSubData,p);
	}
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETCHAR(payjob,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(payjob,"ACTIVE",&active);
		FINDFLDGETCHAR(payjob,"LEAVE ALLOWED",&lveallowed);
		testfile=testfilerangersrc(payjob,rs);
		if(!deleteflag && active && lveallowed && !testfile)
		{
			FINDFLDGETSTRING(payjob,"PERSONNEL IDENTIFICATION",&perid);
			FINDFLDGETINT(payjob,"JOB NUMBER",&jobno);
			FINDFLDSETSTRING(lvejob,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETINT(lvejob,"JOB NUMBER",jobno);
			FINDFLDSETSTRING(lvejob,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid);
			if(EQLNRDsec(lvejob,1))
			{
				ZERNRD(lvejob);
			} else {
				KEYNRD(lvejob,1);
			}
			FINDFLDSETSTRING(lvemstr,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETSTRING(lvemstr,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid);
			if(EQLNRDsec(lvemstr,1))
			{
				ZERNRD(lvemstr);
			} else {
				KEYNRD(lvemstr,1);
			}
			
			FINDFLDSETSTRING(prsnnl,"PERSONNEL IDENTIFICATION",perid);
			if(!EQLNRDsec(prsnnl,1))
			{
				FINDFLDGETCHAR(prsnnl,"DELETEFLAG",&deleteflag);
				testfile=testfilerangersrc(prsnnl,rs);
				if(!deleteflag && !testfile)
				{
					COPYFIELD(payjob,posmst,"POSITION IDENTIFICATION");
					if(!EQLNRDsec(posmst,1))
					{
						FINDFLDGETCHAR(posmst,"DELETEFLAG",&deleteflag);
						testfile=testfilerangersrc(posmst,rs);
						if(!deleteflag && !testfile)
						{
							FINDFLDSETSTRING(lvedesc,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid);
							if(!EQLNRDsec(lvedesc,1))
							{
								FINDFLDGETCHAR(lvedesc,"DELETEFLAG",&deleteflag);
								FINDFLDGETCHAR(lvedesc,"ACTIVE",&active);
								if(!deleteflag && active)
								{
									FINDFLDGETSTRING(lvedesc,"LEAVE TYPE IDENTIFICATION",&lvetypeid);
									FINDFLDSETSTRING(lvetpms,"LEAVE TYPE IDENTIFICATION",lvetypeid);
									if(!EQLNRDsec(lvetpms,1))
									{
										FINDFLDGETCHAR(lvetpms,"DELETEFLAG",
											&deleteflag);
										if(!deleteflag)
										{
											if(select_expression!=NULL && RDAstrlen(select_expression)>0)
											{
												etest=EVALUATEbol(select_expression,LveSelSubData,p);
											} else etest=TRUE;
											if(etest)
											{
												for(x=0,v=mainrsrc->virtual;x<mainrsrc->num;++x,++v)
												{
													if(ADVtestvirtualrangersrc(v,rs,(void (*)())LveSelSubData,p)) break;
												}
												if(x>=mainrsrc->num)
												{
													FINDFLDGETSTRING(lvetpms,"DESCRIPTION",&descript);
													default_amt=EVALUATEdbl(earned_expression,LveSelSubData,p);
													addsortrecord(dbsort,mainrsrc,perid,lvedescid,default_amt,descript,jobno,p);
												} else if(diagapps)
												{
													prterr("Skipped by Ranging on Virtual Field {%s} with Expression of {%s}.",v->name,v->expression);
												}
											} else if(diagapps)
											{
												prterr("Skipped by Evaluating Expression {%s}.",select_expression);
											}
										} else if(diagapps)
										{
											prterr("Skipped by deleted Leave Type Master Record.");KEYNRD(lvetpms,1);SEENRDRECORD(lvetpms);
										}
									} else if(diagapps)
									{
										prterr("Skipped by not finding Leave Type Master Record.");KEYNRD(lvetpms,1);SEENRDRECORD(lvetpms);
									}
								} else if(diagapps)
								{
									prterr("Skipped deleted Leave Description or Inactive Leave Description Record.");KEYNRD(lvedesc,1);SEENRDRECORD(lvedesc);
								}
							} else if(diagapps)
							{
								prterr("Skipped by not finding Leave Description Record.");KEYNRD(lvedesc,1);SEENRDRECORD(lvedesc);
							}
						} else if(diagapps)
						{
							prterr("Skipped deleted Position Master or Inactive Position Master.");KEYNRD(posmst,1);SEENRDRECORD(posmst);
						}
					} else if(diagapps)
					{
						prterr("Skipped by not finding Position Master.");KEYNRD(posmst,1);SEENRDRECORD(posmst);
					}
				} else if(diagapps)
				{
					prterr("Skipped by Ranging on Personnel Master or deleted personnel master.");KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
				}
			} else if(diagapps)
			{
				prterr("Skipped by not finding Personnel Master.");KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
			}
		} else if(diagapps)
		{
			prterr("Skipped by Ranging on Payroll Job Master or Deleted or Inactive Job Master.");KEYNRD(payjob,1);SEENRDRECORD(payjob);
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) 
		{
				update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		}
		if(keyrange)
		{
			ef=ADVNXTNRDsec(payjob,1,LveSelSubData,p);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(payjob,1,rs);
			}
		} else {
			ef=ADVSEQNRDsec(payjob,LveSelSubData,p);
		}
/*
		ef=NXTNRDsec(payjob,1);
*/
	}
	if(p!=NULL) Rfree(p);
	if(perid!=NULL) Rfree(perid);
	if(lvetypeid!=NULL) Rfree(lvetypeid);
	if(descript!=NULL) Rfree(descript);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		switch(select_type)
		{
			default:
			case 0: /* Automatic */
				select_backend(dbsort);
				break;
			case 1: /* Process Exceptions */
				brsrc=RDArsrcNEW(module,"ADD LEAVE EARNED BROWSE");
				srsrc=RDArsrcNEW(module,"ADD LEAVE EARNED SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"ADD LEAVE EARNED DEFINE LIST");
				addDFincvir(drsrc,module,"LVEMSTR",NULL,lvemstr);
				addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,prsnnl);
				addDFincvir(drsrc,module,"LVEDESC",NULL,lvedesc);
				addDFincvir(drsrc,module,"LVETPMS",NULL,lvetpms);
				addDFincvir(drsrc,module,"LVEJOB",NULL,lvejob);
				addDFincvir(drsrc,"PAYROLL","PAYJMST",NULL,payjob);
				addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmst);
				addDFincvir(srsrc,module,"LVEMSTR",NULL,lvemstr);
				addDFincvir(srsrc,module,"LVEJOB",NULL,lvejob);
				addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,prsnnl);
				addDFincvir(srsrc,"PAYROLL","PAYJMST",NULL,payjob);
				addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmst);
				addDFincvir(srsrc,module,"LVEDESC",NULL,lvedesc);
				addDFincvir(srsrc,module,"LVETPMS",NULL,lvetpms);
				GET_SCREEN_VIRTUALS(drsrc,1);
				GET_SCREEN_VIRTUALS(srsrc,2);
				ZERNRD(dbsort->fileno);
				addbtnrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				mbl_trans=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,selectfunction,
					doexit_test,okfunc,new_record,
					dbsort,TRUE,NULL,0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
	} else {
		ERRORDIALOG("Add Leave Earned Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static void makeselecttype()
{
	selecttype=Rmalloc(2*sizeof(char *));
	selecttype[0]=stralloc("Automatic");
	selecttype[1]=stralloc("Process Exceptions");
}
static void freeselecttype()
{
	short x;

	if(selecttype!=NULL)
	{
		for(x=0;x<2;++x) Rfree(selecttype[x]);
		Rfree(selecttype);
	}
}
void quitselect(RDArsrc *mainrsrc)
{
	short exit_program=FALSE;

	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	CLSNRD(lvemstr);
	CLSNRD(prsnnl);
	CLSNRD(lvedesc);
	CLSNRD(lvejob);
	CLSNRD(payjob);
	CLSNRD(posmst);
	CLSNRD(lvetpms);
	CLSNRD(lvetran);
	if(lvedescid!=NULL) Rfree(lvedescid);
	if(lvedesclist!=NULL) freeapplib(lvedesclist);
	if(selecttype!=NULL) freeselecttype();
	if(select_expression!=NULL) Rfree(select_expression);
	if(earned_expression!=NULL) Rfree(earned_expression);
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(TRANSACTION_COUNT>0)
	{
		RUNREPORT(module,reportname,NULL,NULL,TRUE,TRUE,NULL);
	} else {
		exit_program=TRUE;
	}
	if(exit_program)
	{
		ShutdownSubsystems();
		if(reportname!=NULL) Rfree(reportname);
		exit(0);
	}
}
void quitbpaymstr(MakeBrowseList *blist)
{
	if(blist->definelist!=NULL) 
	{
		killwindow(blist->definelist);
		free_rsrc(blist->definelist);
	}
	if(blist->searchrsrc!=NULL) 
	{
		killwindow(blist->searchrsrc);
		free_rsrc(blist->searchrsrc);
	}
	if(blist->mainrsrc!=NULL)
	{
		free_rsrc(blist->mainrsrc);
	}
}
static void changeleavedesc(RDArsrc *mainrsrc)
{
	int sellve=0;

	if(lvedesclist!=NULL)
	{
		if(lvedesclist->libs!=NULL)
		{
			FINDRSCGETINT(mainrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellve);
			if(sellve>=lvedesclist->numlibs) sellve=0;
			if(RDAstrcmp(lvedesclist->libs[sellve],"No Leave Descriptions Available"))
			{
				FINDFLDSETSTRING(lvedesc,"LEAVE DESCRIPTION IDENTIFICATION",lvedesclist->libs[sellve]);
				if(EQLNRDsec(lvedesc,1)) KEYNRD(lvedesc,1);

			} else ZERNRD(lvedesc);
		} else ZERNRD(lvedesc);
	} else ZERNRD(lvedesc);
	filerecord2rsrc(lvedesc,mainrsrc);
	if(IsScreenDisplayed(mainrsrc))
	{
		updatefilerecord2rsrc(lvedesc,mainrsrc);
		updateSCRNvirtuals(mainrsrc);
	} else {
		computeallSCRNvirtuals(mainrsrc);
	}
}
void main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	int x=0;
#ifdef XXX
	int nofields=0;
	NRDfield *fldx,*fields;
#endif

	if(InitializeSubsystems(argc,argv,module,"ADD LEAVE EARNED")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("LEAUDIT");
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if((lvemstr=APPOPNNRD(module,"LVEMSTR",TRUE,FALSE))==(-1)) return;
	if((prsnnl=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((lvetpms=APPOPNNRD(module,"LVETPMS",TRUE,FALSE))==(-1)) return;
	if((lvetran=APPOPNNRD(module,"LVETRAN",TRUE,FALSE))==(-1)) return;
	if((lvejob=APPOPNNRD(module,"LVEJOB",TRUE,FALSE))==(-1)) return;
	if((payjob=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((posmst=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((lvedesc=APPOPNNRD(module,"LVEDSC",TRUE,FALSE))==(-1)) return;
	makeselecttype();
	mainrsrc=RDArsrcNEW(module,"ADD LEAVE EARNED");
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mainrsrc,"PAYROLL","PAYJMST",NULL,payjob);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(mainrsrc,module,"LVEDSC",NULL,lvedesc);
	addDFincvir(mainrsrc,module,"LVETPMS",NULL,lvetpms);
	addDFincvir(mainrsrc,module,"LVEMSTR",NULL,lvemstr);
	addDFincvir(mainrsrc,module,"LVEJOB",NULL,lvejob);
	file2rangersrc(prsnnl,mainrsrc);
	file2rangersrc(payjob,mainrsrc);
	file2rangersrc(posmst,mainrsrc);
	file2rsrc(lvedesc,mainrsrc,FALSE);
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,
		&selecttype,NULL);
	lvedesclist=MakeNRDlistAllSTRING(lvedesc,"LVEDSC",
		"Creating Leave Descrition Identification List",1,"LEAVE DESCRIPTION IDENTIFICATION");
	if(lvedesclist->numlibs<1)
	{
		addAPPlib(lvedesclist,"Not Available");
	}
	x=0;
	addlstrsrc(mainrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",&x,TRUE,
		changeleavedesc,lvedesclist->numlibs,&lvedesclist->libs,NULL);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"EARNED AMOUNT EXPRESSION",0,NULL,TRUE);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"EARNED AMOUNT LOAD VALUE",TRUE,loadvalue,
		"EARNED AMOUNT EXPRESSION");
	addbtnrsrc(mainrsrc,"EARNED AMOUNT LOAD OPERATOR",TRUE,loadoperand,
		"EARNED AMOUNT EXPRESSION");
	addbtnrsrc(mainrsrc,"EARNED AMOUNT LOAD GROUPER",TRUE,loadgrouper,
		"EARNED AMOUNT EXPRESSION");
	addbtnrsrc(mainrsrc,"EARNED AMOUNT LOAD CONTROL",TRUE,loadcontrol,
		"EARNED AMOUNT EXPRESSION");
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectlve,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changeleavedesc(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *lvedescid=NULL,*perid=NULL;
	int transno=0;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETSTRING(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATION",&lvedescid);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&transno);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(dbsort->fileno,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",transno);
	if(EQLNRD(dbsort->fileno,1)) KEYNRD(dbsort->fileno,1);
	filerecord2rsrc(dbsort->fileno,mtnrsrc);	
	if(perid!=NULL) Rfree(perid);
	if(lvedescid!=NULL) Rfree(lvedescid);
	getsupportingfiles(mtnrsrc,dbsort);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
void selectm(short dowhich,void *targetkey,int argc,char **argv,DBsort *t)
{
	int x=0;
	RDArsrc *mtnrsrc=NULL;
        NRDkey *key=NULL;
        NRDpart *part=NULL;

	mtnrsrc=RDArsrcNEW(module,"ADD LEAVE EARNED MAINTAIN SCREEN");
	singlefile2rsrc(t->fileno,mtnrsrc,TRUE);
	if(lvemstr!=(-1)) file2rsrc(lvemstr,mtnrsrc,FALSE);
	if(prsnnl!=(-1)) file2rsrc(prsnnl,mtnrsrc,FALSE);
	if(lvedesc!=(-1)) file2rsrc(lvedesc,mtnrsrc,FALSE);
	if(lvetpms!=(-1)) file2rsrc(lvetpms,mtnrsrc,FALSE);
	if(lvetran!=(-1)) file2rsrc(lvetran,mtnrsrc,FALSE);
	addDFincvir(mtnrsrc,"SECURITY","USERS",NULL,t->fileno); 
	addDFincvir(mtnrsrc,module,"LVEMSTR",NULL,lvemstr);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mtnrsrc,module,"LVEDESC",NULL,lvedesc);
	addDFincvir(mtnrsrc,module,"LVETPMS",NULL,lvetpms);
	addDFincvir(mtnrsrc,module,"LVETRAN",NULL,lvetran);
	key=KEYNUM(t->fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			FINDRSCSETFUNC(mtnrsrc,part->name,getfile,t);
		}
	}
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* this option isn't used */
			break;
		case 2:
			rsrc2singlefilerecord(t->fileno,mtnrsrc);
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(t->fileno,1,targetkey)) 
			{
				KEYNRD(dbsort->fileno,1);
			} else singlefilerecord2rsrc(t->fileno,mtnrsrc);
			break;
	}
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	getsupportingfiles(mtnrsrc,t);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mtnrsrc,TRUE,quit_record,dbsort,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_record(mtnrsrc,TRUE,dbsort);
}
static void save_record_nup(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_record(mtnrsrc,FALSE,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN","SECURITY","Browse Diagnostic",NULL);
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
			xmakebrowselist(mbl_trans,tmprsrc);
			if(mbl_trans->list->num<=valuex) valuex=0;
			mbl_trans->list->keyvalue=mbl_trans->list->key[valuex];
			if(!FINDRSCSETLIST(mbl_trans->mainrsrc,"BROWSE LIST",valuex,
				mbl_trans->list->num,&mbl_trans->list->string))
			{
				updatersrc(mbl_trans->mainrsrc,"BROWSE LIST");
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			ForceWindowUpdate(mtnrsrc);
		}
	}
	if(mtnrsrc!=NULL) killwindow(mtnrsrc);
	quit_record(mtnrsrc,dbsort);
}
static void quit_record(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(WindowCount<2) select_backend(dbsort);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	COPYFIELD(dbsort->fileno,lvemstr,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,lvemstr,"LEAVE DESCRIPTION IDENTIFICATION");
	if(EQLNRDsec(lvemstr,1)) KEYNRD(lvemstr,1);
	filerecord2rsrc(lvemstr,mtnrsrc);
	COPYFIELD(dbsort->fileno,lvedesc,"LEAVE DESCRIPTION IDENTIFICATION");
	if(EQLNRDsec(lvedesc,1)) KEYNRD(lvedesc,1);
	filerecord2rsrc(lvedesc,mtnrsrc);
	COPYFIELD(lvedesc,lvetpms,"LEAVE TYPE IDENTIFICATION");
	if(EQLNRDsec(lvetpms,1)) KEYNRD(lvetpms,1);
	filerecord2rsrc(lvetpms,mtnrsrc);
	COPYFIELD(dbsort->fileno,prsnnl,"PERSONNEL IDENTIFICATION");
	if(EQLNRDsec(prsnnl,1)) KEYNRD(prsnnl,1);
	filerecord2rsrc(prsnnl,mtnrsrc);
	COPYFIELD(dbsort->fileno,lvejob,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,lvejob,"LEAVE DESCRIPTION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,lvejob,"JOB NUMBER");
	if(EQLNRDsec(lvejob,1)) KEYNRD(lvejob,1);
	filerecord2rsrc(lvejob,mtnrsrc);
	COPYFIELD(dbsort->fileno,payjob,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjob,"JOB NUMBER");
	if(EQLNRDsec(payjob,1)) KEYNRD(payjob,1);
	filerecord2rsrc(payjob,mtnrsrc);
	COPYFIELD(payjob,posmst,"POSITION IDENTIFICATION");
	if(EQLNRDsec(posmst,1)) KEYNRD(posmst,1);
	filerecord2rsrc(posmst,mtnrsrc);
}
