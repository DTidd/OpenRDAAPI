/* payavw.c - Payroll Add Vendor Withholding Vouchers */
#ifndef WIN32
#define __NAM__ "payavw.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payavw.exe"
#endif
#include <app.hpp>

#include <trans.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;
struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *addravl;
	RDArsrc *mtnrsrc;
	RDATData *previous;
	DBsort *dbsort;
	PassableStruct *p;
};
typedef struct mtnstructs mtnstruct;

static APPlib *banklist=NULL;
static char *module="PAYROLL",**selecttype=NULL,*reportname=NULL;
static char *select_expression=NULL,**monthlist=NULL,*bank_id=NULL;
static char **fmonthlist=NULL;
static short payddsv=(-1),payddac=(-1),payddms=(-1),venmst=(-1),bnkacc=(-1);
static short finbacc=(-1),finbyr=(-1),finbact=(-1),venadr=(-1),finyear=(-1);
static int select_type=0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static void setfiles(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArsrc *,mtnstruct *);
static void getnextcb(RDArsrc *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,short,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void getsupportingrecords(RDArsrc *,DBsort *);
static void setscrlst(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);
static void changeADDRlist(RDArsrc *,mtnstruct *);
static MakeBrowseList *mbl_venmsr=NULL;
static void browse_venmsr(RDArsrc *,mtnstruct *);
static void doexit1(MakeBrowseList *);
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
static void changemonth(RDArsrc *);
static RDAfinmgt *fmgt=NULL;
static void selectm(short,void *,int,char **,DBsort *);
static int VOUCHER_COUNT=0;
static short setaddrid(RDArsrc *,mtnstruct *);
static void setaddrlist(RDArsrc *,mtnstruct *,short);
static void makeaddrlist(RDArsrc *,mtnstruct *,short);
static void check_venid(RDArsrc *,mtnstruct *);
static short setvenid(RDArsrc *,mtnstruct *);
static short checkmanualvoucher(RDArsrc *,mtnstruct *);
static void setDEFtypes1(RDArsrc *,mtnstruct *);
static void setDEFtypes(RDArsrc *,mtnstruct *,short);
static short chkdebitcode(RDArsrc *,mtnstruct *);
static void chkdebitcodecb(RDArsrc *,mtnstruct *);
static short chkcreditcode(RDArsrc *,mtnstruct *);
static void chkcreditcodecb(RDArsrc *,mtnstruct *);
static void manualvouchercb(RDArsrc *,mtnstruct *);


static void PayavwSubData(char **tmp,PassableStruct *p)
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
		prterr("DIAG PayavwSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
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
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(field->data.string_value))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
/* OK, found in globals */
	} else { /* virtual field */
		for(y=0,v=r->virflds;y<r->num;++y,++v)
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
						temp=EVALUATEstr(v->expression,PayavwSubData,p);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
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
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<v->dlen && !isEMPTY(temp))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
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
						*v->value.short_value=EVALUATEsht(v->expression,PayavwSubData,p);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.integer_value=EVALUATEint(v->expression,PayavwSubData,p);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
						*v->value.float_value=EVALUATEdbl(v->expression,PayavwSubData,p);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,PayavwSubData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,PayavwSubData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
						temp=stralloc(field->data.string_value);
						str_length=(temp!=NULL?RDAstrlen(temp):0);
						if(length==0) length=str_length;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if(!isEMPTY(temp) && str_length>length)
						{
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(length))
								temp[start-1+length]=0;
							if(RDAstrlen(temp)>(start-1+length))
								temp[start-1+length]=0;
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else if(start<=str_length && !isEMPTY(field->data.string_value))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else {
							memset(*tmp,0,RDAstrlen(*tmp)+1);
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
						memset(*tmp,0,RDAstrlen(*tmp)+1);
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
						memset(*tmp,0,RDAstrlen(*tmp)+1);
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
						memset(*tmp,0,RDAstrlen(*tmp)+1);
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
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
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
		prterr("DIAG PayavwSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
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
	AddFileFields(list,payddsv);
	AddFileFields(list,payddac);
	AddFileFields(list,payddms);
	AddFileFields(list,venmst);
	AddFileFields(list,venadr);
	AddFileFields(list,bnkacc);
	AddFileFields(list,finbacc);
	AddFileFields(list,finbyr);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"PAYDDSV",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYDDAC",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYDDMS",NULL,dumb);
	addDFincvir(tmprsrc,"VENPMT","VENMSR",NULL,dumb);
	addDFincvir(tmprsrc,"VENPMT","VENADR",NULL,dumb);
	addDFincvir(tmprsrc,"BNKREC","BNKACC",NULL,dumb);
	addDFincvir(tmprsrc,"FINMGT","FINBACC",NULL,finbacc);
	addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,finbyr);
	addDFincvir(tmprsrc,"FINMGT","FINYEAR",NULL,finyear);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/PAYROLL.VIR",CURRENTDIRECTORY);
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
	short ef=FALSE,update_diag=FALSE,deftype=0/*,pmonth=0*/;
	short fmonth=0,cmonth=0;
	char deleteflag=FALSE,*curdate=NULL,*curtime=NULL,*dedid=NULL;
	char manual_voucher=FALSE,electronic_transfer=FALSE,*bankid=NULL,record_check_number=FALSE;
	int x,numfields=0,trans=0,fiscal_year=0;
	NRDfield *fields=NULL,*field=NULL;
	double amt=0;
	int check_no=0,orig_check=0;
	RDArsrc *tmprsrc=NULL;
	PassableStruct *p=NULL;
	char *e=NULL,*libx=NULL;
	RDATData *prev=NULL;
	RDAGenericSetup *gsv=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Creating Vendor Withholding Vouchers...",NULL);
		addDFincvir(tmprsrc,module,"PAYDDSV",NULL,payddsv);
		addDFincvir(tmprsrc,module,"PAYDDAC",NULL,payddac);
		addDFincvir(tmprsrc,module,"PAYDDMS",NULL,payddms);
		addDFincvir(tmprsrc,"VENPMT","VENMSR",NULL,venmst);
		addDFincvir(tmprsrc,"VENPMT","VENADR",NULL,venadr);
		addDFincvir(tmprsrc,"BNKREC","BNKACC",NULL,bnkacc);
		addDFincvir(tmprsrc,"FINMGT","FINBACC",NULL,finbacc);
		addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,finbyr);
		addDFincvir(tmprsrc,"FINMGT","FINYEAR",NULL,finyear);
		GET_SCREEN_VIRTUALS(tmprsrc,0);
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
		field=FLDNRD(payddsv,"ENTRY DATE");
		if(field->len==10)
		{
			curdate=GETCURRENTDATE10();
		} else {
			curdate=GETCURRENTDATE();
		}
		numfields=NUMFLDS(payddsv);
		fields=FLDPOINTER(payddsv);
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(!deleteflag)
			{
				for(x=0,field=fields;x<numfields;++x,++field)
				{
					if(!RDAstrcmp(field->name,"FISCAL YEAR"))
					{
						FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",
							&fiscal_year);
						FINDFLDSETINT(payddsv,"FISCAL YEAR",fiscal_year);
					} else if(!RDAstrcmp(field->name,"FISCAL PROCESSING MONTH"))
					{
						FINDFLDGETSHORT(dbsort->fileno,"FISCAL PROCESSING MONTH",&fmonth);
						FINDFLDSETSHORT(payddsv,"FISCAL PROCESSING MONTH",fmonth);
					} else if(!RDAstrcmp(field->name,"CALENDAR PROCESSING MONTH"))
					{
						FINDFLDGETSHORT(dbsort->fileno,"CALENDAR PROCESSING MONTH",&cmonth);
						FINDFLDSETSHORT(payddsv,"CALENDAR PROCESSING MONTH",cmonth);
					} else if(!RDAstrcmp(field->name,"TRANSACTION NUMBER"))
					{
						FINDFLDSETINT(payddsv,"TRANSACTION NUMBER",trans);
					} else if(!RDAstrcmp(field->name,
						"DEDUCTION DESCRIPTION IDENTIFICATION"))
					{
						FINDFLDGETSTRING(dbsort->fileno,
							"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
						FINDFLDSETSTRING(payddsv,
							"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
					} else if(!RDAstrcmp(field->name,"DEFINITION TYPE"))
					{
						FINDFLDGETSHORT(dbsort->fileno,
							"DEFINITION TYPE",&deftype);
						FINDFLDSETSHORT(payddsv,"DEFINITION TYPE",deftype);
					} else if(!RDAstrcmp(field->name,"AMOUNT"))
					{
						FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amt);
						FINDFLDSETDOUBLE(payddsv,"AMOUNT",amt);
					} else if(!RDAstrcmp(field->name,"ENTRY DATE"))
					{
						FINDFLDSETSTRING(payddsv,"ENTRY DATE",curdate);
					} else if(!RDAstrcmp(field->name,"ENTRY TIME"))
					{
						FINDFLDSETSTRING(payddsv,"ENTRY TIME",curtime);
					} else if(!RDAstrcmp(field->name,"SOURCE USER"))
					{
						FINDFLDSETSTRING(payddsv,"SOURCE USER",USERLOGIN);
					} else if(!RDAstrcmp(field->name,"MANUAL VOUCHER"))
					{
						FINDFLDGETCHAR(dbsort->fileno,"MANUAL VOUCHER",&manual_voucher);
						FINDFLDGETCHAR(dbsort->fileno,"ELECTRONIC TRANSFER",&electronic_transfer);
						FINDFLDSETCHAR(payddsv,"MANUAL VOUCHER",manual_voucher);
						if(diagapps)
						{
							prterr("Manual Voucher: %s \r\n",(manual_voucher ? "TRUE":"FALSE"));
						}
						if(manual_voucher && !electronic_transfer)
						{
							if(diagapps)
							{
								FINDFLDGETINT(dbsort->fileno,"CHECK NUMBER",&check_no);
								prterr("Check Number: %d \r\n",check_no);
							}
							FIELDCOPY(dbsort->fileno,"CHECK NUMBER",payddsv,"VIRTUAL CHECK NUMBER");
						}
					} else if(!RDAstrcmp(field->name,"VIRTUAL CHECK NUMBER"))
					{
					} else {
						COPYFIELD(dbsort->fileno,payddsv,field->name);
					} 
				}
				ADVWRTTRANSsec(payddsv,1,"TRANSACTION NUMBER",NULL,PayavwSubData,p);
				++VOUCHER_COUNT;
				ADVupdateundist(payddsv,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),finbyr,fiscal_year,deftype,"EXPENDITURES",FISCALPERIOD[fmonth],amt,PayavwSubData,p);
				update_diag=TRUE;
				if(manual_voucher && !electronic_transfer)
				{
					libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+11);
#ifndef WIN32
					sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
					sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif

					gsv=RDAGenericSetupNew("PAYROLL","RECORD CHECK NUMBER");
					if(gsv!=NULL)
					{
						if(!getRDAGenericSetupbin(libx,gsv))
						{
							record_check_number=*gsv->value.string_value;
						} else {
							record_check_number=FALSE;
						}
					}
					if(gsv!=NULL) FreeRDAGenericSetup(gsv);
					if(libx!=NULL) Rfree(libx);
					if(record_check_number)
					{
						FINDFLDGETSTRING(dbsort->fileno,"BANK IDENTIFICATION",&bankid);
						FINDFLDGETINT(dbsort->fileno,"CHECK NUMBER",&check_no);
						prev=RDATDataNEW(bnkacc);	
						FINDFLDGETINT(bnkacc,"LAST CHECK NUMBER",&orig_check);
						if(check_no>orig_check)
						{
							FINDFLDSETINT(bnkacc,"LAST CHECK NUMBER",check_no);
							FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankid);
							WRTTRANS(bnkacc,0,NULL,prev);
						}
						if(prev!=NULL) FreeRDATData(prev);
					}
				} 
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(p!=NULL) Rfree(p);
		if(dedid!=NULL) Rfree(dedid);
		if(curdate!=NULL) Rfree(curdate);
		if(curtime!=NULL) Rfree(curtime);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
			tmprsrc=NULL;
		}
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		dbsort=NULL;
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
	d=getDFvirtual("PAYROLL","ADD VENDOR WITHHOLDING SORT SEQUENCE");
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
	numfields=NUMFLDS(payddsv);
	FIELDS=FLDPOINTER(payddsv);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(RDAstrcmp(field->name,"DELETEFLAG"))
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
			}
		}
	}
	key=KEYNUM(payddsv,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
	addDBfield(dbsrt,"IDENTIFICATION",1,20);
}
void addsortrecord(PassableStruct *p,int fiscal_year,short fmonth,
	int calyear,short cmonth,char *bankid)
{
	char deleteflag=FALSE,*vendorid=NULL,*addressid=NULL;
	char *descript=NULL,elect_trans=FALSE,ddraft=FALSE;
	double total=0.0;
	RDATData *prev_ddac=NULL;
	RDArsrc *mainrsrc=NULL;
	DBsort *dbsrt=NULL;

	mainrsrc=p->r;
	dbsrt=p->a;
	ZERNRD(dbsrt->fileno);
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,PayavwSubData,mainrsrc);
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
	COPYFIELD(payddac,dbsrt->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(payddms,dbsrt->fileno,"IDENTIFICATION");
	FIELDCOPY(payddac,"PAYROLL FUND DEFINITION TYPE",dbsrt->fileno,"DEFINITION TYPE");
	FIELDCOPY(payddac,"CASH CODE",dbsrt->fileno,"CREDIT CODE");
	FIELDCOPY(payddac,"LIABILITY CODE",dbsrt->fileno,"DEBIT CODE");
	COPYFIELD(payddac,dbsrt->fileno,"AMOUNT");
/*
	FIELDCOPY(payddms,"SHORT DESCRIPTION",dbsrt->fileno,"DESCRIPTION");
*/
	FINDFLDGETSTRING(payddms,"SHORT DESCRIPTION",&descript);
	if(isEMPTY(descript))
	{
		FINDFLDGETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",&descript);
	}
	FINDFLDSETSTRING(dbsrt->fileno,"DESCRIPTION",descript);
	if(descript!=NULL) Rfree(descript);
	FINDFLDGETSTRING(payddms,"VENDOR IDENTIFICATION",&vendorid);
	FINDFLDSETSTRING(dbsrt->fileno,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDGETSTRING(payddms,"ADDRESS IDENTIFICATION",&addressid);
	FINDFLDSETSTRING(dbsrt->fileno,"ADDRESS IDENTIFICATION",addressid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(addressid!=NULL) Rfree(addressid);
	FINDFLDGETCHAR(payddms,"ELECTRONIC TRANSFER",&elect_trans);
	FINDFLDSETCHAR(dbsrt->fileno,"ELECTRONIC TRANSFER",elect_trans);
	COPYFIELD(payddms,dbsrt->fileno,"SEPARATE CHECK");
	FINDFLDGETCHAR(venmst,"DIRECT DRAFT",&ddraft);
	if(elect_trans)
	{
		FINDFLDSETCHAR(dbsrt->fileno,"DIRECT DRAFT",ddraft);
		if(ddraft==TRUE) 
		{
			FINDFLDSETCHAR(dbsrt->fileno,"SEPARATE CHECK",TRUE);
		}
	} else {
		FINDFLDSETCHAR(dbsrt->fileno,"DIRECT DRAFT",FALSE);
	}
	FINDFLDSETINT(dbsrt->fileno,"FISCAL YEAR",fiscal_year);
	FINDFLDSETINT(dbsrt->fileno,"CALENDAR YEAR",calyear);
	FINDFLDSETSHORT(dbsrt->fileno,"FISCAL PROCESSING MONTH",fmonth);
	FINDFLDSETSHORT(dbsrt->fileno,"CALENDAR PROCESSING MONTH",cmonth);
	FINDFLDSETSTRING(dbsrt->fileno,"BANK IDENTIFICATION",bankid);
	prev_ddac=RDATDataNEW(payddac);
	FINDFLDSETDOUBLE(payddac,"AMOUNT",total);
	ADVWRTTRANSsec(payddac,0,NULL,prev_ddac,PayavwSubData,p);
	if(prev_ddac!=NULL) FreeRDATData(prev_ddac);
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
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!",
			"Are you sure that you want to quit without processing the Payroll Vendor Withholdings?",
			doexit,NULL,FALSE,blist);
	} else {
		doexit(blist);
	}
}
static void doexit(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL,*tmprsrc=NULL;
	short ef=FALSE;
	double amt=0.0,pamt=0.0;
	RDATData *prev=NULL;
	char deleteflag=FALSE,*e=NULL;
	PassableStruct *p=NULL;

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
	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Re-Establishing PAYDDAC Amounts",NULL);
		addDFincvir(tmprsrc,module,"PAYDDAC",NULL,payddac);
		addDFincvir(tmprsrc,module,"PAYDDMS",NULL,payddms);
		GET_SCREEN_VIRTUALS(tmprsrc,0);
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
		p->r=tmprsrc;
		p->a=dbsort;
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				LOCNRDFILE(payddac);
				COPYFIELD(dbsort->fileno,payddac,"DEDUCTION DESCRIPTION IDENTIFICATION");
				COPYFIELD(dbsort->fileno,payddac,"DEFINITION TYPE");
				FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&pamt);
				if(!ADVEQLNRDsec(payddac,1,PayavwSubData,p))
				{
					prev=RDATDataNEW(payddac);
					FINDFLDGETDOUBLE(payddac,"AMOUNT",&amt);
					amt+=pamt;
					FINDFLDSETDOUBLE(payddac,"AMOUNT",amt);
					ADVWRTTRANSsec(payddac,0,NULL,prev,PayavwSubData,p);
					if(prev!=NULL) FreeRDATData(prev);
				}
				UNLNRDFILE(payddac);
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
			tmprsrc=NULL;
		}
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		Rfree(p);
	}
}
static void saveexit(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

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
		free_rsrc(brsrc);
		brsrc=NULL;
	}
	if(WindowCount<=2)
	{
		doexit1(mbl_venmsr);
		select_backend(dbsort);
	}
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
void addvouchers(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,dodiag=FALSE,testfile=FALSE,deftype=0;
	short fmonth=0,cmonth=0,efl=0,efc=0;
	short efly=0,efcy=0;
	char deleteflag=FALSE,*sortname=NULL,flag=FALSE;
	char *ddedid=NULL,*venid=NULL,*venadrid=NULL;
	char *e=NULL;
	int selected_bank=0,x,fiscal_year=0,calyear=0;
	long size_of_file=0;
	double amt=0;
	RDAvirtual *v=NULL;
	PassableStruct *p=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&selected_bank);
	FINDRSCGETINT(mainrsrc,"SELECT TYPE",&select_type);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&select_expression);
	FINDRSCGETINT(mainrsrc,"FISCAL YEAR",&fiscal_year);
	FINDRSCGETINT(mainrsrc,"CALENDAR YEAR",&calyear);
	FINDRSCGETSHORT(mainrsrc,"FISCAL PROCESSING MONTHS",&fmonth);
	FINDRSCGETSHORT(mainrsrc,"CALENDAR PROCESSING MONTHS",&cmonth);
	if(bank_id!=NULL) Rfree(bank_id);
	if(RDAstrcmp(banklist->libs[selected_bank],"No Banks Available"))
	{
		bank_id=stralloc(banklist->libs[selected_bank]);
	} else bank_id=NULL;
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=dbsort;
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(payddac,rs);
	ReadRangeScreen(payddms,rs);
	ReadRangeScreen(venmst,rs);
	ReadRangeScreen(venadr,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(payddac,"DIAGNOSTIC SCREEN",module,
		"Selecting Payroll Jobs",NULL);
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
	ZERNRD(payddac);
	LOCNRDFILE(payddac);
	ef=ADVFRSNRDsec(payddac,1,PayavwSubData,p);
	while(!ef)
	{
		LOCNRD(payddac);
		UNLNRDFILE(payddac);
		dodiag=FALSE;
		FINDFLDGETSHORT(payddac,"DEFINITION TYPE",&deftype);
		FINDFLDGETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",&ddedid);
		FINDFLDGETCHAR(payddac,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		FINDFLDGETDOUBLE(payddac,"AMOUNT",&amt);
		if(amt!=0)
		{
			ZERNRD(finbacc);
			FIELDCOPY(payddac,"PAYROLL FUND DEFINITION TYPE",finbacc,"DEFINITION TYPE");
			FIELDCOPY(payddac,"LIABILITY CODE",finbacc,"ACCOUNT CODE");
			efl=ADVEQLNRDsec(finbacc,1,PayavwSubData,p);
			ZERNRD(finbacc);
			FIELDCOPY(payddac,"PAYROLL FUND DEFINITION TYPE",finbacc,"DEFINITION TYPE");
			FIELDCOPY(payddac,"CASH CODE",finbacc,"ACCOUNT CODE");
			efc=ADVEQLNRDsec(finbacc,1,PayavwSubData,p);
			testfile=testfilerangersrc(payddac,rs);
			ZERNRD(finbyr);
			FIELDCOPY(payddac,"PAYROLL FUND DEFINITION TYPE",finbyr,"DEFINITION TYPE");
			FIELDCOPY(payddac,"LIABILITY CODE",finbyr,"ACCOUNT CODE");
			FINDFLDSETINT(finbyr,"FISCAL YEAR",fiscal_year);
			efly=ADVEQLNRDsec(finbyr,1,PayavwSubData,p);
			ZERNRD(finbyr);
			FIELDCOPY(payddac,"PAYROLL FUND DEFINITION TYPE",finbyr,"DEFINITION TYPE");
			FIELDCOPY(payddac,"CASH CODE",finbyr,"ACCOUNT CODE");
			FINDFLDSETINT(finbyr,"FISCAL YEAR",fiscal_year);
			efcy=ADVEQLNRDsec(finbyr,1,PayavwSubData,p);
			
			if(!testfile && !efl && !efc && !efly && !efcy)
			{
				FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",ddedid);
				if(ADVEQLNRDsec(payddms,1,PayavwSubData,p)) KEYNRD(payddms,1);
				FINDFLDGETCHAR(payddms,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				FINDFLDGETCHAR(payddms,"ACTIVE",&flag);
				if(flag)
				{
				FINDFLDGETCHAR(payddms,"BENEFIT",&flag);
				if(!flag)
				{
					testfile=testfilerangersrc(payddms,rs);
					if(!testfile)
					{
						FINDFLDGETSTRING(payddms,"VENDOR IDENTIFICATION",&venid);
						FINDFLDSETSTRING(venmst,"VENDOR IDENTIFICATION",venid);
						if(ADVEQLNRDsec(venmst,1,PayavwSubData,p)) KEYNRD(venmst,1);
						FINDFLDGETCHAR(venmst,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
						FINDFLDGETCHAR(venmst,"ACTIVE",&flag);
						if(flag)
						{
							testfile=testfilerangersrc(venmst,rs);
							if(!testfile)
							{
								FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",venid);
								FINDFLDGETSTRING(payddms,"ADDRESS IDENTIFICATION",&venadrid);
								FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",venadrid);
								if(ADVEQLNRDsec(venadr,1,PayavwSubData,p)) KEYNRD(venadr,1);
								FINDFLDGETCHAR(venadr,"DELETEFLAG",&deleteflag);
								if(!deleteflag)
								{
								testfile=testfilerangersrc(venadr,rs);
								if(!testfile)
								{
								for(x=0,v=mainrsrc->virflds;
									x<mainrsrc->num;++x,++v)
								{
									if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))PayavwSubData,p)) break;
								}
								if(x>=mainrsrc->num)
								{
									addsortrecord(p,fiscal_year,
										fmonth,calyear,cmonth,bank_id);
									dodiag=TRUE;
								} else if(diagapps)
								{
									prterr("Skipped by Ranging on Virtual Field [%s].",v->name);
								}
								} else {
								if(diagapps)
								{
									prterr("Skipped the [VENPMT][VENADR] (Vendor Address) data file record Vendor Identification [%s] and Address Identification [%s] because of Ranging on Field Values.",
										(venid==NULL?"":venid),(venadrid=NULL?"":venadrid));
								}
								}
								} else {
								if(diagapps)
								{
									prterr("Skipped the [VENPMT][VENADR] (Vendor Address) data file record Vendor Identification [%s] and Address Identification [%s] because of DELETEFLAG field value of TRUE.",
										(venid==NULL?"":venid),(venadrid==NULL?"":venadrid));
								}
								}
							} else {
							if(diagapps)
							{
								prterr("Skipped the [VENPMT][VENMSR] (Vendor Master) data file record Vendor Identification [%s] because of Ranging on Field Values.",
									(venid==NULL?"":venid));
							}
							}
						} else {
						if(diagapps)
						{
							prterr("Skipped the [VENPMT][VENMSR] (Vendor Master) data file record Vendor Identification [%s] because of ACTIVE field value of FALSE.",
								(venid==NULL?"":venid));
						}
						}
						} else {
						if(diagapps)
						{
							prterr("Skipped the [VENPMT][VENMSR] (Vendor Master) data file record Vendor Identification [%s] because of DELETEFLAG field value of TRUE.",
								(venid==NULL?"":venid));
						}
						}
					} else {
					if(diagapps)
					{
						prterr("Skipped the [PAYROLL][PAYDDMS] (Deduction Description Master) data file record Deduction Description Identification [%s] because of Ranging on Field Values.",
							(ddedid==NULL?"":ddedid));
					}
					}
				} else {
				if(diagapps)
				{
					prterr("Skipped the [PAYROLL][PAYDDMS] (Deduction Description Master) data file record Deduction Description Identification [%s] because of BENEFIT field value of TRUE.",
						(ddedid==NULL?"":ddedid));
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped the [PAYROLL][PAYDDMS] (Deduction Description Master) data file record Deduction Description Identification [%s] because of ACTIVE field value of FALSE.",
						(ddedid==NULL?"":ddedid));
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped the [PAYROLL][PAYDDMS] (Deduction Description Master) data file record Deduction Description Identification [%s] because of DELETEFLAG field value of TRUE.",
						(ddedid==NULL?"":ddedid));
				}
				}
			} else {
			if(diagapps)
			{
				prterr("Skipped the [PAYROLL][PAYDDAC] (Deduction Description Account Master) data file record Deduction Description Identification [%s] and Defintion Type [%d] because of Ranging on Field Values or the Liability and/or Cash Code didn't exist.",
					(ddedid==NULL?"":ddedid),deftype);
			}
			}
		} else {
		if(diagapps)
		{
			prterr("Skipped the [PAYROLL][PAYDDMS] (Deduction Description Account Master) data file record Deduction Description Identification [%s] and Defintion Type [%d] because of AMOUNT field value of [%.02f] less than or equal to $0.00.",
				(ddedid==NULL?"":ddedid),deftype,(amt/100));
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped the [PAYROLL][PAYDDMS] (Deduction Description Account Master) data file record Deduction Description Identification [%s] and Defintion Type [%d] because of DELETEFLAG field value of TRUE.",
				(ddedid==NULL?"":ddedid),deftype);
		}
		}
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		UNLNRD(payddac);
		LOCNRDFILE(payddac);
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		ef=ADVNXTNRDsec(payddac,1,PayavwSubData,p);
	}
	UNLNRDFILE(payddac);
	if(p!=NULL) Rfree(p);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(ddedid!=NULL) Rfree(ddedid);
	if(venid!=NULL) Rfree(venid);
	if(venadrid!=NULL) Rfree(venadrid);
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
				brsrc=RDArsrcNEW(module,"ADD VENDOR WITHHOLDING BROWSE");
				srsrc=RDArsrcNEW(module,"ADD VENDOR WITHHOLDING SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"ADD VENDOR WITHHOLDING DEFINE LIST");
				addDFincvir(drsrc,module,"PAYDDMS",NULL,payddms);
				addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmst);
				addDFincvir(drsrc,"VENPMT","VENADR",NULL,venadr);
				addDFincvir(drsrc,"BNKREC","BNKACC",NULL,bnkacc);
				addDFincvir(srsrc,module,"PAYDDMS",NULL,payddms);
				addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmst);
				addDFincvir(srsrc,"VENPMT","VENADR",NULL,venadr);
				addDFincvir(srsrc,"BNKREC","BNKACC",NULL,bnkacc);
				GET_SCREEN_VIRTUALS(drsrc,1);
				GET_SCREEN_VIRTUALS(srsrc,2);
				ZERNRD(dbsort->fileno);
				brsrc->scn=RDAscrnNEW(brsrc->module,brsrc->screen);
				findscrnbin(brsrc->scn);
				MakeBrowseListTotals(brsrc,dbsort->fileno,drsrc);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				mbl_trans=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,
					selectfunction,doexit_test,
					okfunc,new_record,dbsort,TRUE,NULL,
					0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
	} else {
		ERRORDIALOG("Add Vendor Withholding Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static void selectvouchers(RDArsrc *mainrsrc)
{
	int x=0,cur_month=0,cur_year=0,fin_year=0;
	int fiscal_year=0,calendar_year=0;
	short fiscal_month=0,calendar_month=0,fin_month=0;
	char *temp1=NULL,*temp2=NULL;
	APPlib *select_warnlist=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSHORT(mainrsrc,"FISCAL PROCESSING MONTHS",&fiscal_month);
	FINDRSCGETINT(mainrsrc,"FISCAL YEAR",&fiscal_year);
	FINDRSCGETSHORT(mainrsrc,"CALENDAR PROCESSING MONTHS",&calendar_month);
	FINDRSCGETINT(mainrsrc,"CALENDAR YEAR",&calendar_year);
	cur_month=val(&CURRENT_DATE10[0],2)-1;
	cur_year=val(&CURRENT_DATE10[6],4);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif

	gsv=RDAGenericSetupNew("FINMGT","CURRENT FISCAL YEAR");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			fin_year=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	ZERNRD(finyear);
	FINDFLDSETINT(finyear,"FISCAL YEAR",fiscal_year);
	if(!ADVEQLNRDsec(finyear,1,SCRNvirtualSubData,mainrsrc))
	{
		FINDFLDGETSHORT(finyear,"PROCESSING MONTH",&fin_month);
	}

	select_warnlist=APPlibNEW();
	if((fiscal_month==0) || (fiscal_month==13))
	{
		ERRORDIALOG("ADD VENDOR WITHHOLDING VOUCHERS ERROR","Please select a different fiscal month.  Since this process is balance sheet only and credits cash, pre-year month or post year month is not appropriate.",NULL,FALSE);
		return;
	}
	if(fiscal_year!=fin_year)
	{
		addAPPlib(select_warnlist,"The Fiscal Year specified is not the same as the default fiscal year in Financial Management.");
	}
	if(fiscal_month!=fin_month)
	{
		addAPPlib(select_warnlist,"The Fiscal Month specified is not the same as the default fiscal month for the selected Fiscal Year (from Financial Management).");
	}
	if(calendar_year!=cur_year)
	{
		addAPPlib(select_warnlist,"The Calendar Year specified is not the same as the current year.");
	}
	if(calendar_month!=cur_month)
	{
		addAPPlib(select_warnlist,"The Calendar Month specified is not the same as the current month.");
	}

	if(select_warnlist->numlibs>0)
	{
		addAPPlib(select_warnlist,"Are you sure this is the correct period?");
		for(x=0;x<select_warnlist->numlibs;x++)
		{
			if(temp1==NULL)
			{
				temp1=Rmalloc(RDAstrlen(select_warnlist->libs[x])+1);
				sprintf(temp1,"%s",select_warnlist->libs[x]);
			} else {
				temp2=Rmalloc(RDAstrlen(temp1));
				sprintf(temp2,"%s",temp1);
				temp1=Rrealloc(temp1,RDAstrlen(select_warnlist->libs[x])+RDAstrlen(temp2)+1);
				sprintf(temp1,"%s %s",temp2,select_warnlist->libs[x]);
				if(temp2!=NULL) Rfree(temp2);
			}
		}
		WARNINGDIALOG("WARNING DIALOG SCREEN","ADD VENDOR WITHHOLDING VOUCHERS WARNING",temp1,addvouchers,NULL,FALSE,mainrsrc);
	} else {
		addvouchers(mainrsrc);
	}

	if(temp1!=NULL) Rfree(temp1);
	if(select_warnlist!=NULL) freeapplib(select_warnlist);
}
static void makeselecttype()
{
	int x=0;

	selecttype=Rmalloc(2*sizeof(char *));
	selecttype[0]=stralloc("Automatic");
	selecttype[1]=stralloc("Process Exceptions");
	monthlist=Rmalloc(12*sizeof(char *));
        for(x=0;x<12;++x)
        {
                monthlist[x]=stralloc(CALENDAR_MONTHS[x]);
        }
        fmonthlist=Rmalloc(14*sizeof(char *));
        for(x=0;x<14;++x)
        {
                fmonthlist[x]=stralloc(FISCALPERIOD[x]);
        }
}
static void freeselecttype()
{
	short x;

	if(selecttype!=NULL)
	{
		for(x=0;x<2;++x) Rfree(selecttype[x]);
		Rfree(selecttype);
		selecttype=NULL;
	}
	if(monthlist!=NULL)
	{
		for(x=0;x<12;++x) Rfree(monthlist[x]);
		Rfree(monthlist);
		monthlist=NULL;
	}
	if(fmonthlist!=NULL)
	{
		for(x=0;x<14;++x) Rfree(fmonthlist[x]);
		Rfree(fmonthlist);
		fmonthlist=NULL;
	}
}
void quitselect(RDArsrc *mainrsrc)
{
	APPlib *args=NULL;

	if(mainrsrc!=NULL) 
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	mainrsrc=NULL;
	CLSNRD(payddsv);
	CLSNRD(payddac);
	CLSNRD(payddms);
	CLSNRD(venmst);
	CLSNRD(bnkacc);
	CLSNRD(venadr);
	CLSNRD(finbacc);
	CLSNRD(finbyr);
	CLSNRD(finbact);
	if(bank_id!=NULL) Rfree(bank_id);
	if(banklist!=NULL) freeapplib(banklist);
	if(selecttype!=NULL) freeselecttype();
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(VOUCHER_COUNT>0)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(reportname!=NULL) Rfree(reportname);
	ShutdownSubsystems();
}
static void changemonth(RDArsrc *mainrsrc)
{
	int selmonth=0,year=0;
	
	FINDRSCGETINT(mainrsrc,"CALENDAR PROCESSING MONTHS",&selmonth);
	readwidget(mainrsrc,"CALENDAR YEAR");
	FINDRSCGETINT(mainrsrc,"CALENDAR YEAR",&year);
	if(fmgt->month>0)
	{
		if(selmonth>=fmgt->month) year+=1;
	}
	FINDRSCSETINT(mainrsrc,"FISCAL YEAR",year);
	updatersrc(mainrsrc,"FISCAL YEAR");
	FINDRSCSETINT(mainrsrc,"FISCAL PROCESSING MONTHS",selmonth+1);
	updatersrc(mainrsrc,"FISCAL PROCESSING MONTHS");
	updateSCRNvirtuals(mainrsrc);
}
static void changeyear(RDArsrc *mainrsrc)
{
	int fyear=0,month=0;
	short dmonth=0;

	readwidget(mainrsrc,"FISCAL YEAR");
	FINDRSCGETINT(mainrsrc,"FISCAL YEAR",&fyear);
	ZERNRD(finyear);
	FINDFLDSETINT(finyear,"FISCAL YEAR",fyear);
	if(!ADVEQLNRDsec(finyear,1,SCRNvirtualSubData,mainrsrc))
	{
		FINDFLDGETSHORT(finyear,"PROCESSING MONTH",&dmonth);
		month=dmonth;
		FINDRSCSETINT(mainrsrc,"FISCAL PROCESSING MONTHS",month);
		updatersrc(mainrsrc,"FISCAL PROCESSING MONTHS");
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	int x,xmonth=0,cyear=0,fyear=0;
	char *name=NULL;

	if(InitializeSubsystems(argc,argv,module,"ADD VENDOR WITHHOLDING VOUCHERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("VWAUDIT");
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif

	gsv=RDAGenericSetupNew("PAYROLL","V/W BANK IDENTIFICATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			bank_id=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if((payddsv=APPOPNNRD(module,"PAYDDSV",TRUE,TRUE))==(-1)) return;
	if((payddac=APPOPNNRD(module,"PAYDDAC",TRUE,TRUE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((venmst=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if((finbacc=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((finbyr=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
	if((finbact=APPOPNNRD("FINMGT","FINBACT",TRUE,TRUE))==(-1)) return;
	if((finyear=APPOPNNRD("FINMGT","FINYEAR",TRUE,TRUE))==(-1)) return;
	if((venadr=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	makeselecttype();
	mainrsrc=RDArsrcNEW(module,"ADD VENDOR WITHHOLDING VOUCHERS");
	addDFincvir(mainrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(mainrsrc,module,"PAYDDAC",NULL,payddac);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,venmst);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	file2rangersrc(payddac,mainrsrc);
	file2rangersrc(payddms,mainrsrc);
	file2rangersrc(venmst,mainrsrc);
	file2rangersrc(venadr,mainrsrc);
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,
		&selecttype,NULL);
	xmonth=val(&CURRENT_DATE[0],2)-1;
	if(xmonth>11) xmonth=0;
	addlstrsrc(mainrsrc,"CALENDAR PROCESSING MONTHS",&xmonth,TRUE,changemonth,12,&monthlist,NULL);
	xmonth=val(&CURRENT_DATE[0],2);
	if(xmonth>13) xmonth=0;
	addlstrsrc(mainrsrc,"FISCAL PROCESSING MONTHS",&xmonth,TRUE,NULL,14,&fmonthlist,NULL);
	cyear=GETCURRENTCALENDARYEAR();
	if(fmgt->month!=0)
	{
		if(xmonth>=fmgt->month) fyear=cyear+1;
		else fyear=cyear;
	} else fyear=cyear;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif

	gsv=RDAGenericSetupNew("FINMGT","CURRENT FISCAL YEAR");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			fyear=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&cyear,TRUE);
	addstdrsrc(mainrsrc,"FISCAL YEAR",LONGV,4,&fyear,TRUE);
	FINDRSCSETFUNC(mainrsrc,"FISCAL YEAR",changeyear,NULL);
	if(name!=NULL) Rfree(name);
	banklist=ADVMakeNRDlistAllSTRING(bnkacc,"BNKREC",
		"Creating Bank Identification List",1,"BANK IDENTIFICATION",
		SCRNvirtualSubData,mainrsrc);
	if(banklist->numlibs<1)
	{
		addAPPlib(banklist,"No Banks Available");
	}
	x=0;
	if(!isEMPTY(bank_id))
	{
		for(x=0;x<banklist->numlibs;++x)
		{
			if(!RDAstrcmp(banklist->libs[x],bank_id)) break;
		}
	}
	addlstrsrc(mainrsrc,"BANK IDENTIFICATIONS",&x,TRUE,
		NULL,banklist->numlibs,&banklist->libs,NULL);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	FINDRSCSETINT(mainrsrc,"BANK IDENTIFICATIONS",x);
	changeyear(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectvouchers,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void setscrlst(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	setaddrlist(mtnrsrc,mtn,FALSE);
	setDEFtypes(mtnrsrc,mtn,FALSE);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *desc=NULL;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	COPYFIELD(dbsort->fileno,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
	if(ADVEQLNRDsec(payddms,1,PayavwSubData,p)) KEYNRD(payddms,1);
	filerecord2rsrc(payddms,mtnrsrc);
	COPYFIELD(dbsort->fileno,bnkacc,"BANK IDENTIFICATION");
	if(ADVEQLNRDsec(bnkacc,1,PayavwSubData,p)) KEYNRD(bnkacc,1);
	filerecord2rsrc(bnkacc,mtnrsrc);
	COPYFIELD(dbsort->fileno,finyear,"FISCAL YEAR");
	if(ADVEQLNRDsec(finyear,1,PayavwSubData,p)) KEYNRD(finyear,1);
	filerecord2rsrc(finyear,mtnrsrc);
	COPYFIELD(dbsort->fileno,venmst,"VENDOR IDENTIFICATION");
	if(ADVEQLNRDsec(venmst,1,PayavwSubData,p)) KEYNRD(venmst,1);
	filerecord2rsrc(venmst,mtnrsrc);
	COPYFIELD(dbsort->fileno,finbacc,"DEFINITION TYPE");
	FIELDCOPY(dbsort->fileno,"DEBIT CODE",finbacc,"ACCOUNT CODE");
	if(ADVEQLNRDsec(finbacc,1,PayavwSubData,p)) KEYNRD(finbacc,1);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",desc);
	COPYFIELD(dbsort->fileno,finbacc,"DEFINITION TYPE");
	FIELDCOPY(dbsort->fileno,"CREDIT CODE",finbacc,"ACCOUNT CODE");
	if(ADVEQLNRDsec(finbacc,1,PayavwSubData,p)) KEYNRD(finbacc,1);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",desc);
	if(desc!=NULL) Rfree(desc);
	COPYFIELD(dbsort->fileno,venadr,"VENDOR IDENTIFICATION");
	COPYFIELD(dbsort->fileno,venadr,"ADDRESS IDENTIFICATION");
	if(ADVEQLNRDsec(venadr,1,PayavwSubData,p)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);
	if(p!=NULL) Rfree(p);
}
static void doexit1(MakeBrowseList *blist)
{
	if(blist!=NULL)
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
}
static void okfunc1(MakeBrowseList *b,void *targetkey)
{
	char *vendorid=NULL;
	mtnstruct *mtn=NULL;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;

	mtn=(mtnstruct *)b->funcargs;
	dbsort=(DBsort *)mtn->dbsort;
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtn->mtnrsrc;
	p->a=dbsort;
	ZERNRD(venmst);
	if(ADVEQLNRDKEYsec(venmst,1,targetkey,PayavwSubData,p))
	{
		KEYNRD(venmst,1);
	} else {
		if(vendorid!=NULL) { Rfree(vendorid); vendorid=NULL; }
		FINDFLDGETSTRING(venmst,"VENDOR IDENTIFICATION",&vendorid);
	}
	filerecord2rsrc(venmst,mtn->mtnrsrc);
	FINDRSCSETSTRING(mtn->mtnrsrc,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",vendorid);
	if(venadr!=(-1))
	{
		ZERNRD(venadr);
		FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(venadr,1,PayavwSubData,p))
		KEYNRD(venadr,1);
	}
	ADVcomputeandupdateallSCRNvirtuals(mtn->mtnrsrc,PayavwSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtn->mtnrsrc);
	makeaddrlist(mtn->mtnrsrc,mtn,TRUE);
}
static void browse_venmsr(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(mtnrsrc);
	if(mbl_venmsr==NULL)
	{
		mainrsrc=RDArsrcNEW("PAYROLL","ADD VENDOR WITHHOLDING VENDOR BROWSE");
		searchrsrc=RDArsrcNEW("PAYROLL","ADD VENDOR WITHHOLDING VENDOR SEARCH BROWSE");
		definelist=RDArsrcNEW("PAYROLL","ADD VENDOR WITHHOLDING VENDOR DEFINE LIST");
		addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,venmst);
		addDFincvir(definelist,"VENPMT","VENMSR",NULL,venmst);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_venmsr=ADVbrowse(venmst,1,mainrsrc,searchrsrc,definelist,
			NULL,selectfunction,NULL,okfunc1,NULL,
			mtn,TRUE,NULL,0,1,1,FALSE);
	} else {
		mtn->mtnrsrc=mtnrsrc;
		mbl_venmsr->funcargs=mtn;
		if(APPmakescrn(mbl_venmsr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit1(mbl_venmsr);
		}
	}
}
static void check_venid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *venid=NULL,*venid1=NULL;

	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&venid);
	readallwidgets(mtnrsrc);
	if(setvenid(mtnrsrc,mtn))
	{
		FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&venid1);
		if(RDAstrcmp(venid,venid1))
		{
			makeaddrlist(mtnrsrc,mtn,TRUE);
		}
	} else {
		makeaddrlist(mtnrsrc,mtn,TRUE);
	}
	if(venid!=NULL) Rfree(venid);
	if(venid1!=NULL) Rfree(venid1);
}
static short checkmanualvoucher(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int checknbr=0;
	char manualvoucher,electronicvoucher;
	char *checkdate=NULL;
	DBsort *dbsort=NULL;

	dbsort=(DBsort *)mtn->dbsort;

	FINDRSCGETINT(mtnrsrc,"CHECK NUMBER",&checknbr);
	FINDRSCGETCHAR(mtnrsrc,"MANUAL VOUCHER",&manualvoucher);
	FINDRSCGETCHAR(mtnrsrc,"ELECTRONIC TRANSFER",&electronicvoucher);
	FINDRSCGETSTRING(mtnrsrc,"CHECK DATE",&checkdate);
	if(!manualvoucher || electronicvoucher)
	{
		FINDFLDSETINT(dbsort->fileno,"CHECK NUMBER",0);
		FINDFLDSETSTRING(dbsort->fileno,"CHECK DATE","");
	}
	if(manualvoucher && !electronicvoucher && (RDAstrlen(checkdate)!=10))
	{
		ERRORDIALOG("CHECK DATE IS NOT LONG ENOUGH","Please enter a valid check date.",NULL,FALSE);
		return(TRUE);
	}
	if(manualvoucher && !electronicvoucher && !checkdate)
	{
		ERRORDIALOG("CHECK DATE IS BLANK","Please enter a check date.",NULL,FALSE);
		return(TRUE);
	}
	if(manualvoucher && !electronicvoucher && checknbr==0)
	{
		ERRORDIALOG("CHECK NUMBER IS 0","Please enter a check number greater than 0.",NULL,FALSE);
		return(TRUE);
	}
	return(FALSE);
}
static short setvenid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *venid=NULL;
	char *edesc=NULL,delflag=FALSE;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDFLDSETSTRING(venmst,"VENDOR IDENTIFICATION",venid);
	if(ADVEQLNRDsec(venmst,1,PayavwSubData,p))
	{
		KEYNRD(venmst,1);
		edesc=Rmalloc(RDAstrlen(venid)+126);
		sprintf(edesc,"The Vendor Identification [%s] is invalid.  It doesn't exist.  Please enter a correct identification or cancel this operation.",
			(venid==NULL?"":venid));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION",edesc,
			NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
		if(venid!=NULL) Rfree(venid);
		if(venmst!=(-1)) updatefilerecord2rsrc(venmst,mtnrsrc);
		updateSCRNvirtuals(mtnrsrc);
		if(p!=NULL) Rfree(p);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(venmst,"DELETEFLAG",&delflag);
		if(delflag)
		{
			edesc=Rmalloc(RDAstrlen(venid)+140);
			sprintf(edesc,"The VENDOR IDENTIFICATION of [%s] entered is deleted.  It exist as a VENDOR but is presently set up in a deletion state.",(venid==NULL?"":venid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!",edesc,NULL,NULL,TRUE,NULL);
			if(venid!=NULL) Rfree(venid); 
			if(edesc!=NULL) Rfree(edesc);
			if(p!=NULL) Rfree(p);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(venmst,"ACTIVE",&delflag);
			if(!delflag)
			{
				edesc=Rmalloc(RDAstrlen(venid)+155);
				sprintf(edesc,"The VENDOR IDENTIFICATION of [%s] entered is not active.  It exist as a VENDOR but is presently set up in a non-active state.",(venid==NULL?"":venid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","NON-ACTIVE VENDOR IDENTIFICATION!",edesc,NULL,NULL,TRUE,NULL);
				if(venid!=NULL) Rfree(venid); 
				if(edesc!=NULL) Rfree(edesc);
				if(p!=NULL) Rfree(p);
				return(FALSE);
			}
		}
		if(venid!=NULL) Rfree(venid);
		if(venmst!=(-1)) updatefilerecord2rsrc(venmst,mtnrsrc);
		updateSCRNvirtuals(mtnrsrc);
		if(p!=NULL) Rfree(p);
		return(TRUE);
	}
}
static short setaddrid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *venid=NULL;
	int seladdr=0;
	char *edesc=NULL;
	PassableStruct *p=NULL;

	FINDRSCGETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",&seladdr);
	if(!RDAstrcmp(mtn->addravl->libs[seladdr],"No Addresses Defined."))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ADDRESS IDENTIFICATION!","The Vendor Address Identification entered does not exist for the selected Vendor.",
			NULL,NULL,TRUE,NULL);
			return(FALSE);
	}
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",venid);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addravl->libs[seladdr]);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	if(ADVEQLNRDsec(venadr,1,PayavwSubData,p))
	{
		KEYNRD(venadr,1);
		edesc=Rmalloc(RDAstrlen(mtn->addravl->libs[seladdr])+RDAstrlen(venid)+200);
		sprintf(edesc,"The Address Identification of [%s] is invalid for the Vendor Identification of [%s].  It doesn't exist.  Please enter a correct identifica tion or cancel this operation.",
			(mtn->addravl->libs[seladdr]==NULL?"":mtn->addravl->libs[seladdr]),(venid==NULL?"":venid));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR ADDRESS IDENTIFICATION",edesc,NULL,NULL,TRUE,NULL);
		if(p!=NULL) Rfree(p);
		if(edesc!=NULL) Rfree(edesc);
		if(venid!=NULL) Rfree(venid);
		if(venadr!=(-1)) updatefilerecord2rsrc(venadr,mtnrsrc);
		updateSCRNvirtuals(mtnrsrc);
		return(FALSE);
	} else {
		if(p!=NULL) Rfree(p);
		if(venid!=NULL) Rfree(venid);
		updateSCRNvirtuals(mtnrsrc);
		filerecord2rsrc(venadr,mtnrsrc);
		return(TRUE);
	}
}
/*-----------------------------
static short setbankid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *bnkid=NULL;
	int selbank=0;
	char *edesc=NULL;
	PassableStruct *p=NULL;

	FINDRSCGETINT(mtnrsrc,"BANK IDENTIFICATIONS",&selbank);
	if(!RDAstrcmp(banklist->libs[selbank],"No Banks Available"))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BANK IDENTIFICATION!","The Bank Address Identification entered does not exist.",
			NULL,NULL,TRUE,NULL);
			return(FALSE);
	}
	FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",banklist->libs[selbank]);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	if(ADVEQLNRDsec(bnkacc,1,PayavwSubData,p))
	{
		KEYNRD(bnkacc,1);
		edesc=Rmalloc(RDAstrlen(banklist->libs[selbank])+RDAstrlen(bnkid)+200);
		sprintf(edesc,"The Bank Identification of [%s] is invalid.  It doesn't exist.  Please enter a correct identification or cancel this operation.",
			(banklist->libs[selbank]==NULL?"":banklist->libs[selbank]));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BANK IDENTIFICATION",edesc,NULL,NULL,TRUE,NULL);
		if(p!=NULL) Rfree(p);
		if(edesc!=NULL) Rfree(edesc);
		if(bnkid!=NULL) Rfree(bnkid);
		if(bnkacc!=(-1)) updatefilerecord2rsrc(bnkacc,mtnrsrc);
		updateSCRNvirtuals(mtnrsrc);
		return(FALSE);
	} else {
		if(p!=NULL) Rfree(p);
		if(bnkid!=NULL) Rfree(bnkid);
		updateSCRNvirtuals(mtnrsrc);
		filerecord2rsrc(bnkacc,mtnrsrc);
		return(TRUE);
	}
}
-----------------------------*/
/*-----------------------------
static void BANKidscb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=0;
	char *bankid=NULL;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;

	if(bnkacc!=(-1))
	{
		dbsort=(DBsort *)mtn->dbsort;
		p=Rmalloc(sizeof(PassableStruct));
		p->r=mtnrsrc;
		p->a=dbsort;
		FINDRSCGETINT(mtnrsrc,"BANK IDENTIFICATIONS",&x);
		if(!RDAstrcmp(banklist->libs[x],"No Banks Defined.")) return;
		updatersrc(mtnrsrc,"BANK IDENTIFICATION");
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",banklist->libs[x]);
		if(bankid!=NULL) Rfree(bankid);
		if(ADVEQLNRDsec(bnkacc,1,PayavwSubData,p)) KEYNRD(bnkacc,1);
		updateSCRNvirtuals(mtnrsrc);
		updatefilerecord2rsrc(bnkacc,mtnrsrc);
		if(p!=NULL) Rfree(p);
	}
}
-----------------------------*/
/*
static void getdebit(RDArsrc *mtnrsrc,DBsort *db)
{
	char *acc=NULL,*desc=NULL;
	short deftype=0;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=db;
	readwidget(mtnrsrc,"DEFINITION TYPE");
	FINDRSCGETSHORT(mtnrsrc,"DEFINITION TYPE",&deftype);
	readwidget(mtnrsrc,"DEBIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"DEBIT CODE",&acc);
	FINDFLDSETSHORT(finbacc,"DEFINITION TYPE",deftype);
	FINDFLDSETSTRING(finbacc,"ACCOUNT CODE",acc);
	if(ADVEQLNRDsec(finbacc,1,PayavwSubData,p)) 
	{
		KEYNRD(finbacc,1);
	}
	if(acc!=NULL) Rfree(acc);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
	if(desc!=NULL) Rfree(desc);
	if(p!=NULL) Rfree(p);
}
static void getcredit(RDArsrc *mtnrsrc,DBsort *db)
{
	char *acc=NULL,*desc=NULL;
	short deftype=0;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=db;
	readwidget(mtnrsrc,"DEFINITION TYPE");
	FINDRSCGETSHORT(mtnrsrc,"DEFINITION TYPE",&deftype);
	readwidget(mtnrsrc,"CREDIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"CREDIT CODE",&acc);
	FINDFLDSETSHORT(finbacc,"DEFINITION TYPE",deftype);
	FINDFLDSETSTRING(finbacc,"ACCOUNT CODE",acc);
	if(ADVEQLNRDsec(finbacc,1,PayavwSubData,p)) 
	{
		KEYNRD(finbacc,1);
	}
	if(acc!=NULL) Rfree(acc);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	if(desc!=NULL) Rfree(desc);
	if(p!=NULL) Rfree(p);
}
static void getaddr(RDArsrc *mtnrsrc,DBsort *db)
{
	char *vendorid=NULL,*addrid=NULL;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=db;
	readwidget(mtnrsrc,"VENDOR IDENTIFICATION");
	readwidget(mtnrsrc,"ADDRESS IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&vendorid);
	FINDRSCGETSTRING(mtnrsrc,"ADDRESS IDENTIFICATION",&addrid);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",addrid);
	if(ADVEQLNRDsec(venadr,1,PayavwSubData,p)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);
	if(p!=NULL) Rfree(p);
}
*/
static void changeADDRlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seladdr=0;
	char *vendorid1=NULL;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;

	readwidget(mtnrsrc,"ADDRESS IDENTIFICATIONS");
	FINDRSCGETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",&seladdr);
	readwidget(mtnrsrc,"VENDOR IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&vendorid1);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addravl->libs[seladdr]);
	if(ADVEQLNRDsec(venadr,1,PayavwSubData,p)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);
	if(p!=NULL) Rfree(p);
	FINDRSCSETSTRING(mtnrsrc,"ADDRESS IDENTIFICATION",mtn->addravl->libs[seladdr]);
	FINDFLDSETSTRING(dbsort->fileno,"ADDRESS IDENTIFICATION",mtn->addravl->libs[seladdr]);
	updatersrc(mtnrsrc,"ADDRESS IDENTIFICATION");
	updateSCRNvirtuals(mtnrsrc);
	if(vendorid1!=NULL) Rfree(vendorid1);
}
static void makeaddrlist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid=NULL,*addrid1=NULL;
	char delflag=FALSE;
	int x=0;
	PassableStruct *p=NULL;

	if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
	mtn->addravl=APPlibNEW();
	ZERNRD(venadr);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	ef=ADVGTENRDsec(venadr,1,PayavwSubData,p);
	while(!ef)
	{
		FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadr,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(venadr,"ADDRESS IDENTIFICATION",&addrid);
			addAPPlib(mtn->addravl,addrid);
		}
		ef=ADVNXTNRDsec(venadr,1,PayavwSubData,p);
	}
	FINDRSCGETSTRING(mtnrsrc,"ADDRESS IDENTIFICATION",&addrid1);
	if(mtn->addravl->numlibs<1)
	{
		addAPPlib(mtn->addravl,"No Addresses Defined.");
	} else {
		for(x=0;x<mtn->addravl->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addravl->libs[x]))
			break;
		}
		if(x>=mtn->addravl->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addravl->libs[x]);
	FINDRSCSETSTRING(mtnrsrc,"ADDRESS IDENTIFICATION",mtn->addravl->libs[x]);
	if(ADVEQLNRDsec(venadr,1,PayavwSubData,p)) KEYNRD(venadr,1);
	if(p!=NULL) Rfree(p);
	if(update) updatefilerecord2rsrc(venadr,mtnrsrc);
	else filerecord2rsrc(venadr,mtnrsrc);
	FINDRSCLISTAPPlib(mtnrsrc,"ADDRESS IDENTIFICATIONS",x,mtn->addravl);
	if(update)
	{
		updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
		updatersrc(mtnrsrc,"ADDRESS IDENTIFICATION");
	}
	if(addrid!=NULL) Rfree(addrid);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
/*
static void setbnklist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	DBsort *dbsort=NULL;
	char *bnkid=NULL;
	int x=0;
	PassableStruct *p=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	FINDFLDGETSTRING(dbsort->fileno,"BANK IDENTIFICATION",&bnkid);
	ZERNRD(bnkacc);
	FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bnkid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	if(ADVEQLNRDsec(bnkacc,1,PayavwSubData,p))
	{
		KEYNRD(bnkacc,1);
	}
	if(p!=NULL) Rfree(p);
	if(banklist->numlibs<1)
	{
		addAPPlib(banklist,"No Banks Available");
	} else {
		for(x=0;x<banklist->numlibs;x++)
		{
			if(!RDAstrcmp(bnkid,banklist->libs[x]))
			break;
		}
		if(x>=banklist->numlibs)
		{
			x=0;
		}
	}
	if(bnkid==NULL || RDAstrlen(bnkid)<1)
	{
		x=0;
	}
	FINDRSCSETINT(mtnrsrc,"BANK IDENTIFICATIONS",x);
	if(update)
	{
		updatefilerecord2rsrc(bnkacc,mtnrsrc);
		updatersrc(mtnrsrc,"BANK IDENTIFICATIONS");
	} else filerecord2rsrc(bnkacc,mtnrsrc);
	if(bnkid!=NULL) Rfree(bnkid);
}
*/
static void setDEFtypes1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=(short)x;
	FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",d);
}
static void setDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *edesc=NULL;
	short d=0;
	int x=0,y=0;
	DBsort *dbsort=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&d);
	x=(int)d;
	if(x>=mtn->DEFtypes->numlibs)
	{
		for(y=0;y<(x-(mtn->DEFtypes->numlibs)-1);++y)
		{
			addAPPlib(mtn->DEFtypes,"Invalid-Definition Type");
		}
		edesc=Rmalloc(105);
		sprintf(edesc,"The Definition Type [%d] is invalid.  It doesn't exist. Please correct or deleted this record.",d);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",edesc,NULL,NULL,TRUE,NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",0);
		if(edesc!=NULL) Rfree(edesc);
	
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",d);
	if(update)
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
}
static void setaddrlist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	DBsort *dbsort=NULL;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL;
	char delflag=FALSE;
	int x=0;
	PassableStruct *p=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
	mtn->addravl=APPlibNEW();
	FINDFLDGETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",&vendorid1);
	FINDFLDGETSTRING(dbsort->fileno,"ADDRESS IDENTIFICATION",&addrid1);
	ZERNRD(venadr);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ef=ADVGTENRDsec(venadr,1,PayavwSubData,p);
	while(!ef)
	{
		FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadr,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(venadr,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(mtn->addravl,addrid2);
		}
		ef=ADVNXTNRDsec(venadr,1,PayavwSubData,p);
	}
	if(mtn->addravl->numlibs<1)
	{
		addAPPlib(mtn->addravl,"No Addresses Defined.");
	} else {
		for(x=0;x<mtn->addravl->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addravl->libs[x]))
			break;
		}
		if(x>=mtn->addravl->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addravl->libs[x]);
	if(ADVEQLNRDsec(venadr,1,PayavwSubData,p)) KEYNRD(venadr,1);
	if(p!=NULL) Rfree(p);
	if(update) updatefilerecord2rsrc(venadr,mtnrsrc);
	else filerecord2rsrc(venadr,mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"ADDRESS IDENTIFICATION",&addrid1);
	if(isEMPTY(addrid1))
	{
		x=0;
		FINDRSCSETSTRING(mtnrsrc,"ADDRESS IDENTIFICATION",mtn->addravl->libs[x]);
	}
	FINDRSCLISTAPPlib(mtnrsrc,"ADDRESS IDENTIFICATIONS",x,mtn->addravl);
	if(update) updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
	if(addrid1!=NULL) Rfree(addrid1);
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void getaddr(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *addrid1=NULL;
	int x;
	DBsort *dbsort=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	FINDFLDGETSTRING(dbsort->fileno,"ADDRESS IDENTIFICATION",&addrid1);
	if(RDAstrcmp(mtn->addravl->libs[0],"No Addresses Defined."))
	{
		for(x=0;x<mtn->addravl->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addravl->libs[x]))
			break;
		}
		if(x>=mtn->addravl->numlibs)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ADDRESS IDENTIFICATION!","The Vendor Address Identification entered does not exist for the selected Vendor.",NULL,NULL,TRUE,NULL);
		} else {
			FINDRSCSETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",x);
			updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
		}
	}
	if(addrid1!=NULL) Rfree(addrid1);
}
static void getaddrcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *addrid1=NULL;
	DBsort *dbsort=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	readwidget(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]",&addrid1);
	FINDFLDSETSTRING(dbsort->fileno,"ADDRESS IDENTIFICATION",addrid1);
	getaddr(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
	if(addrid1!=NULL) Rfree(addrid1);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selectedd=0;
	short deftype1=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"DEBIT CODE",&deftype1);
	deftype2=(short)selectedd;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",deftype2);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"DEBIT CODE");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	}
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	changeDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void setfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	PassableStruct *p=NULL;

	readscreen(mtnrsrc,mtn);
	getsupportingrecords(mtnrsrc,mtn->dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PayavwSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
void selectm(short dowhich,void *targetkey,int argc,char **argv,DBsort *t)
{
	int x,which_type=0;
	RDArsrc *mtnrsrc=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nofields=0,edit_rsrc=FALSE;
	RDAacct *acc=NULL;
	mtnstruct *mtn=NULL;
	PassableStruct *p=NULL;

	mtn=Rmalloc(sizeof(mtnstruct));
	ZERNRD(t->fileno);
	mtn->DEFtypes=NULL;
	mtn->DEFtypes=APPlibNEW();
	mtn->addravl=NULL;
	mtn->mtnrsrc=NULL;
	mtn->previous=RDATDataNEW(t->fileno);
	mtn->dbsort=t;
	mtnrsrc=RDArsrcNEW(module,"ADD VENDOR WITHHOLDING MAINTAIN SCREEN");
	mtn->mtnrsrc=mtnrsrc;
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acc=BALANCE_SHEET_ACCOUNT->codetype;x<BALANCE_SHEET_ACCOUNT->num;++x,++acc)
			{
				addAPPlib(mtn->DEFtypes,acc->name);
			}
		}
	}
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=TRUE;
			if(!RDAstrcmp(fldx->name,"ADDRESS IDENTIFICATION"))
			{
				
				if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
				mtn->addravl=APPlibNEW();
				addAPPlib(mtn->addravl,"No Addresses Defined.");
				if(FIELDscrn(t->fileno,fldx->name,&edit_rsrc))
				{
					addstdrsrc(mtnrsrc,"ADDRESS IDENTIFICATION",VARIABLETEXT,0,NULL,edit_rsrc);
					which_type=0;
					addlstrsrc(mtnrsrc,"ADDRESS IDENTIFICATIONS",
						&which_type,edit_rsrc,
						changeADDRlist,mtn->addravl->numlibs,
						&mtn->addravl->libs,mtn);
					FINDRSCSETFUNC(mtnrsrc,"ADDRESS IDENTIFICATION",getaddrcb,mtn);
				}
/*-----------------------------
			} else if(!RDAstrcmp(fldx->name,"BANK IDENTIFICATION"))
			{
				which_type=0;
				if(FIELDscrn(t->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"BANK IDENTIFICATIONS",
						&which_type,edit_rsrc,
						BANKidscb,
						banklist->numlibs,
						&banklist->libs,mtn);
				}
-----------------------------*/
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(t->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					if(mtn->DEFtypes->numlibs<2) edit_rsrc=FALSE;

					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,edit_rsrc,
						DEFtypecb,
						mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else {
				ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,edit_rsrc,TRUE);
				FINDRSCSETFUNC(mtnrsrc,fldx->name,setfiles,mtn);
			}
		}
	}
	if(payddms!=(-1)) file2rsrc(payddms,mtnrsrc,FALSE);
	if(venmst!=(-1)) file2rsrc(venmst,mtnrsrc,FALSE);
	if(venadr!=(-1)) file2rsrc(venadr,mtnrsrc,FALSE);
	if(bnkacc!=(-1)) file2rsrc(bnkacc,mtnrsrc,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addDFincvir(mtnrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,venmst);
	addDFincvir(mtnrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(mtnrsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(mtnrsrc,"FINMGT","FINBACC",NULL,finbacc);
	addDFincvir(mtnrsrc,"FINMGT","FINYEAR",NULL,finyear);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	FINDRSCSETFUNC(mtnrsrc,"VENDOR IDENTIFICATION",check_venid,mtn);
	FINDRSCSETFUNC(mtnrsrc,"DEBIT CODE",chkdebitcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"CREDIT CODE",chkcreditcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"MANUAL VOUCHER",manualvouchercb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"ELECTRONIC TRANSFER",manualvouchercb,mtn);
	key=KEYNUM(t->fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			FINDRSCSETFUNC(mtnrsrc,part->name,SETID,mtn);
		}
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	mtn->p=p;
	ADVSCRNvirtual2rsrc(mtnrsrc,PayavwSubData,p);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* this option isn't used */
			break;
		case 2:
			ReadRDAScrolledLists(mtnrsrc);
			if(EQLNRD(t->fileno,1))
			{
				rsrc2singlefilerecord(t->fileno,mtnrsrc);
				makeaddrlist(mtnrsrc,mtn,FALSE);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				setscrlst(mtnrsrc,mtn);
				GetRDATData(t->fileno,mtn->previous);
			}
			break;
		default:
		case 0:
			if(EQLNRDKEY(t->fileno,1,targetkey))
			{
				ZERNRD(t->fileno);
				ReadRDAScrolledLists(mtnrsrc);
				if(EQLNRD(t->fileno,1))
				{
					makeaddrlist(mtnrsrc,mtn,FALSE);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					singlefilerecord2rsrc(t->fileno,mtnrsrc);
					setscrlst(mtnrsrc,mtn);
					GetRDATData(t->fileno,mtn->previous);
				}
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				setscrlst(mtnrsrc,mtn);
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				GetRDATData(t->fileno,mtn->previous);
			}
			break;
	}
	getsupportingrecords(mtnrsrc,t);
	ADVSetVirFieldFuncs(mtnrsrc,PayavwSubData,p);
	prterr("Made it");
	manualvouchercb(mtnrsrc,mtn);
	addbtnrsrc(mtnrsrc,"PREVIOUS",TRUE,getpreviouscb,mtn);
	addbtnrsrc(mtnrsrc,"NEXT",TRUE,getnextcb,mtn);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	addbtnrsrc(mtnrsrc,"BROWSE VENDORS",TRUE,browse_venmsr,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfkwrsrc(mtnrsrc,"QUIT",TRUE,quit_record,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PayavwSubData,p,FALSE);
/*
	if(p!=NULL) Rfree(p);
*/
	APPmakescrn(mtnrsrc,TRUE,quit_record,mtn,FALSE);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	DBsort *dbsort=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	list2file(mtnrsrc,mtn,TRUE);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selected=0;
	DBsort *dbsort=NULL;
	short deftype=0;

	dbsort=(DBsort *)mtn->dbsort;
	FINDRSCGETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->addravl->libs[selected],"No Addresses Defined."))
	{
		FINDFLDSETSTRING(dbsort->fileno,"ADDRESS IDENTIFICATION",mtn->addravl->libs[selected]);
	} else {
		FINDFLDSETSTRING(dbsort->fileno,"ADDRESS IDENTIFICATION",NULL);
	}
/*-----------------------------
	FINDRSCGETINT(mtnrsrc,"BANK IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->addravl->libs[selected],"No Banks Available"))
	{
		FINDFLDSETSTRING(dbsort->fileno,"BANK IDENTIFICATION",banklist->libs[selected]);
	} else {
		FINDFLDSETSTRING(dbsort->fileno,"BANK IDENTIFICATION",NULL);
	}
-----------------------------*/
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	deftype=(short)selected;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",deftype);
	setDEFtypes1(mtnrsrc,mtn);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,mtnstruct *mtn)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;
	double amount=0.0,pamt=0.0,total=0.0;
	char deleteflag=FALSE,dflag=FALSE;
	PassableStruct *p=NULL;
	char *e=NULL;

	WRTNRD(mtn->dbsort->fileno,0,NULL);
	COPYFIELD(mtn->dbsort->fileno,payddac,
		"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(mtn->dbsort->fileno,payddac,"DEFINITION TYPE");
	FINDRDATDataGETDOUBLE(mtn->previous,"AMOUNT",&pamt);
	FINDRDATDataGETCHAR(mtn->previous,"DELETEFLAG",&dflag);
	FINDFLDGETDOUBLE(mtn->dbsort->fileno,"AMOUNT",&amount);
	FINDFLDGETCHAR(mtn->dbsort->fileno,"DELETEFLAG",&deleteflag);
	tmprsrc=RDArsrcNEW(module,NULL);
	addDFincvir(tmprsrc,module,"PAYDDAC",NULL,payddac);
	addDFincvir(tmprsrc,module,"PAYDDMS",NULL,payddms);
	GET_SCREEN_VIRTUALS(tmprsrc,0);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=tmprsrc;
	p->a=mtn->dbsort;
	LOCNRDFILE(payddac);
	if(!ADVEQLNRDsec(payddac,1,PayavwSubData,p))
	{
		prev=RDATDataNEW(payddac);
		FINDFLDGETDOUBLE(payddac,"AMOUNT",&total);
		if(dflag && !deleteflag)
		{
			total-=amount;
		} else if(!dflag && deleteflag)
		{
			total+=pamt;
		} else if(!dflag && !deleteflag)
		{
			total+=(pamt-amount);
		}
		FINDFLDSETDOUBLE(payddac,"AMOUNT",total);
		ADVWRTTRANSsec(payddac,0,NULL,prev,PayavwSubData,p);
		if(prev!=NULL) FreeRDATData(prev);
	}
	UNLNRDFILE(payddac);
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	if(p!=NULL) Rfree(p);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(mtn->dbsort->fileno,"DIAGNOSTIC SCREEN",
				"SECURITY","Browse Diagnostic",NULL);
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
	quit_record(mtnrsrc,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	DBsort *dbsrt=NULL;

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
		if(mtn->mtnrsrc!=NULL) mtn->mtnrsrc=NULL;
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		dbsrt=mtn->dbsort;
		if(mtn->p!=NULL) Rfree(mtn->p);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(WindowCount<2)
	{
		doexit1(mbl_venmsr);
		select_backend(dbsrt);
	}
}
static short chkdebitcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL;
	short return_value=TRUE;
	int deftype=0,yearid=0;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	FINDFLDGETINT(dbsort->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",&yearid);
	FINDFLDGETSTRING(dbsort->fileno,"DEBIT CODE",&acctcode);
	FINDFLDSETSTRING(finbacc,"ACCOUNT CODE",acctcode);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,2,deftype,yearid,PayavwSubData,p))!=NULL)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
		Rfree(tmp);
		if(acctcode!=NULL) Rfree(acctcode);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		return(FALSE);
	}
	if(p!=NULL) Rfree(p);
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",desc);
	return(return_value);
}
static void chkdebitcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;
	DBsort *dbsort=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"DEBIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"DEBIT CODE",&acctcode);
	FINDFLDSETSTRING(dbsort->fileno,"DEBIT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkdebitcode(mtnrsrc,mtn);
}
static short chkcreditcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL;
	short return_value=TRUE;
	int deftype=0,yearid=0;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	FINDFLDGETINT(dbsort->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",&yearid);
	FINDFLDGETSTRING(dbsort->fileno,"CREDIT CODE",&acctcode);
	FINDFLDSETSTRING(finbacc,"ACCOUNT CODE",acctcode);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,2,deftype,yearid,PayavwSubData,p))!=NULL)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
		Rfree(tmp);
		if(acctcode!=NULL) Rfree(acctcode);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		return(FALSE);
	}
	if(p!=NULL) Rfree(p);
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",desc);
	return(return_value);
}
static void manualvouchercb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char manualvoucher,electronic_transfer;
	readwidget(mtnrsrc,"MANUAL VOUCHER");
	FINDRSCGETCHAR(mtnrsrc,"MANUAL VOUCHER",&manualvoucher);
	readwidget(mtnrsrc,"ELECTRONIC TRANSFER");
	FINDRSCGETCHAR(mtnrsrc,"ELECTRONIC TRANSFER",&electronic_transfer);
	if(manualvoucher && !electronic_transfer)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"CHECK NUMBER",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"CHECK DATE",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"CHECK NUMBER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"CHECK DATE",FALSE);
	}
	if(electronic_transfer)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"MANUAL VOUCHER",FALSE);
		FINDRSCSETCHAR(mtnrsrc,"MANUAL VOUCHER",FALSE);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"MANUAL VOUCHER",TRUE);
	}
		
	
}
static void chkcreditcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;
	DBsort *dbsort=NULL;

	dbsort=(DBsort *)mtn->dbsort;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"CREDIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"CREDIT CODE",&acctcode);
	FINDFLDSETSTRING(dbsort->fileno,"CREDIT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkcreditcode(mtnrsrc,mtn);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char delflag=FALSE,elect_trans=FALSE,electronicvoucher=FALSE;

	readscreen(mtnrsrc,mtn);
	FINDRSCGETCHAR(mtnrsrc,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(checkmanualvoucher(mtnrsrc,mtn)) return;
		if(!setvenid(mtnrsrc,mtn)) return;
		if(!setaddrid(mtnrsrc,mtn)) return;
/*-----------------------------
		if(!setbankid(mtnrsrc,mtn)) return;
-----------------------------*/
		if(!chkdebitcode(mtnrsrc,mtn)) return;
		if(!chkcreditcode(mtnrsrc,mtn)) return;
		FINDFLDGETCHAR(payddms,"ELECTRONIC TRANSFER",&elect_trans);
		FINDRSCGETCHAR(mtnrsrc,"ELECTRONIC TRANSFER",&electronicvoucher);
		if(electronicvoucher && !elect_trans)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","ELECTRONIC TRANSFER DOES NOT MATCH!","The deduction description is not set to electronic transfer.\n",NULL,NULL,TRUE,NULL);
		}
		save_record(mtnrsrc,update_list,mtn);
	} else {
		save_record(mtnrsrc,update_list,mtn);
	}
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;
	char *dedid=NULL;
	int fyear=0,trans=0;

	dbsort=(DBsort *)mtn->dbsort;
	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&trans);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(dedid!=NULL) Rfree(dedid);
	FINDFLDSETINT(mtn->dbsort->fileno,"FISCAL YEAR",fyear);
	FINDFLDSETINT(mtn->dbsort->fileno,"TRANSACTION NUMBER",trans);
	if(EQLNRD(mtn->dbsort->fileno,1))
	{
		KEYNRD(mtn->dbsort->fileno,1);
		ClearRDATData(mtn->previous);
		list2file(mtnrsrc,mtn,TRUE);
	} else {
		GetRDATData(mtn->dbsort->fileno,mtn->previous);
		singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
		setscrlst(mtnrsrc,mtn);
	}
	getsupportingrecords(mtnrsrc,mtn->dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PayavwSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getpreviouscb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;
	char *dedid=NULL;
	int fyear=0,trans=0;

	dbsort=(DBsort *)mtn->dbsort;
	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&trans);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(dedid!=NULL) Rfree(dedid);
	FINDFLDSETINT(mtn->dbsort->fileno,"FISCAL YEAR",fyear);
	FINDFLDSETINT(mtn->dbsort->fileno,"TRANSACTION NUMBER",trans);

	if(EQLNRD(mtn->dbsort->fileno,1))
	{
		KEYNRD(mtn->dbsort->fileno,1);
		if(LTENRD(mtn->dbsort->fileno,1))
		{
			KEYNRD(mtn->dbsort->fileno,1);
			ClearRDATData(mtn->previous);
			list2file(mtnrsrc,mtn,TRUE);
		} else {
			GetRDATData(mtn->dbsort->fileno,mtn->previous);
			singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
			setscrlst(mtnrsrc,mtn);
		}
	} else {
		PRVNRD(mtn->dbsort->fileno,1);
		GetRDATData(mtn->dbsort->fileno,mtn->previous);
		singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
		setscrlst(mtnrsrc,mtn);
	}
	getsupportingrecords(mtnrsrc,mtn->dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PayavwSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getnextcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;
	char *dedid=NULL;
	int fyear=0,trans=0;

	dbsort=(DBsort *)mtn->dbsort;
	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&trans);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(dedid!=NULL) Rfree(dedid);
	FINDFLDSETINT(mtn->dbsort->fileno,"FISCAL YEAR",fyear);
	FINDFLDSETINT(mtn->dbsort->fileno,"TRANSACTION NUMBER",trans);

	if(EQLNRD(mtn->dbsort->fileno,1))
	{
		KEYNRD(mtn->dbsort->fileno,1);
		if(GTENRD(mtn->dbsort->fileno,1))
		{
			KEYNRD(mtn->dbsort->fileno,1);
			ClearRDATData(mtn->previous);
			list2file(mtnrsrc,mtn,TRUE);
		} else {
			GetRDATData(mtn->dbsort->fileno,mtn->previous);
			singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
			setscrlst(mtnrsrc,mtn);
		}
	} else {
		NXTNRD(mtn->dbsort->fileno,1);
		GetRDATData(mtn->dbsort->fileno,mtn->previous);
		singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
		setscrlst(mtnrsrc,mtn);
	}
	getsupportingrecords(mtnrsrc,mtn->dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=mtn->dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PayavwSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
