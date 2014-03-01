/* undotran.c - View Transactions and Undo */
#ifndef WIN32
#define __NAM__ "undotran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "undotran.exe"
#endif
#include <app.hpp>


#include <fcntl.h>

#ifdef UNIXWARE7
#include <cerrno>
#endif
#ifdef UNIXWARE21
#include <cerrno>
#endif
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


struct RDAtflds
{
	char *name;
	short type;
	unsigned len;
	union rda_field_types from;
	union rda_field_types to;
};
typedef struct RDAtflds RDAtfld;
struct RDAtkeys
{
	union rda_field_types data;
};
typedef struct RDAtkeys RDAtkey;
struct RDAtrecs
{
	short undone;
	char type;
	short fileno;
	char *module;
	char *file;
	short keyno;
	char *keyname;
	short nokeys;
	RDAtkey *kdata;
	int numflds;
	RDAtfld *fld;
};
typedef struct RDAtrecs RDAtrec;
struct RDAtrans
{
	short able;
	short complete;
	char *datetime;
	char *process;
	char *user;
	char *endtime;
	int numrecs;
	RDAtrec *recs;
};
typedef struct RDAtrans RDAtran;
struct RDActrns
{
	short num;
	short *files;
};
typedef struct RDActrns RDActrn;

struct RDAtflds
{
	char *name;
	short type;
	unsigned len;
	union rda_field_types from;
	union rda_field_types to;
};
typedef struct RDAtflds RDAtfld;
struct RDAtkeys
{
	union rda_field_types data;
};
typedef struct RDAtkeys RDAtkey;
struct RDAtrecs
{
	short undone;
	char type;
	short fileno;
	char *module;
	char *file;
	short keyno;
	char *keyname;
	short nokeys;
	RDAtkey *kdata;
	int numflds;
	RDAtfld *fld;
};
typedef struct RDAtrecs RDAtrec;
struct RDAtrans
{
	short able;
	short complete;
	char *datetime;
	char *process;
	char *user;
	char *endtime;
	int numrecs;
	RDAtrec *recs;
};
typedef struct RDAtrans RDAtran;
struct RDActrns
{
	short num;
	short *files;
};
typedef struct RDActrns RDActrn;


static APPlib *modlst=NULL,*loglst=NULL,*trnlst=NULL;
static char *module="DATABASE";
static RDAtran *tran=NULL;
static void printtrans(RDArsrc *,RDArsrc *);
static void printtranscb(RDArsrc *);
static void printheader(FILE *,int *,int *,char *,unsigned,char *,char *,char *,char *);
static void printdetail(FILE *,int *,int *,unsigned,char *,char *,char *,char *);
static char *readlistmaketransdesc(RDArsrc *),*maketransdesc(int,char);
static char *makechangedesc(RDAtrec *,char),*makenewdesc(RDAtrec *,char);
static void quitrsrc(RDArsrc *);

void gettransentries(RDArsrc *);
APPlib *maketrnlst(void);

#define	ADVVERIFYTRANSACTION(x,y)	xVERIFYTRANSACTION(x,y,__LINE__,__FILE__)
#define VERIFYTRANSACTION(x)	xVERIFYTRANSACTION(x,FALSE,__LINE__,__FILE__)
/* VERIFYTRANSACTION - tests the transaction for security etc before to 
	determine if the user has the right to undo a particular transaction */
short xVERIFYTRANSACTION(RDAtran *,short,int,char *);
#define UndoRDAtrec(x)	xUndoRDAtrec(x,__LINE__,__FILE__)
/* UndoRDAtrec Undoes changes to a specific record (RDAtrec *) structure 
	contained in a transaction */
short xUndoRDAtrec(RDAtrec *,int,char *);




#define SEERDAtran(x)	xSEERDAtran(x,__LINE__,__FILE__)
/* SEERDAtran - Diagnostic routine for printing the contents of an RDAtran 
	Structure to RDA_STDERR */
void xSEERDAtran(RDAtran *,int,char *);





char *READFIELD(FILE *,short *);
/* READFIELD used to read a field from a transaction processing text file */
#define RDAtranNEW(x,y)	xRDAtranNEW(x,y,__LINE__,__FILE__)
/* RDAtranNEW allocates memory for a new RDAtran Structure */
RDAtran *xRDAtranNEW(char *,char *,int,char *);
#define FreeRDAtran(x)	{xFreeRDAtran(x,__LINE__,__FILE__);x=NULL;}
/* FreeRDAtran free's the memory allocated in an RDAtran Structure */
void xFreeRDAtran(RDAtran *,int,char *);
char *READFIELD(FILE *,short *);
RDAtkey *READKEYS(FILE *,short *,short,short);
#define CLOSEtranFiles(x)	xCLOSEtranFiles(x,__LINE__,__FILE__)
/* CLOSEtranFILES - Closes all datafiles opened in a specific RDAtran 
	Structure */
void xCLOSEtranFiles(RDAtran *,int,char *);
#define GETTRANSACTION(x)	xGETTRANSACTION(x,__LINE__,__FILE__)
/* GETTRANSACTION reads the specified transaction processing text file and 
	returns an RDAtran Structure (RDAtran *) reflecting it's contents */
RDAtran *xGETTRANSACTION(char *,int,char *);
char *makekeystring(short,short,RDAtkey *);





/* UNSURE IF NEEDED IN LIBRARY ANY LONGER */
RDActrn *RDActrnNEW()
{
	RDActrn *tmp;

	tmp=Rmalloc(sizeof(RDActrn));
	tmp->num=0;
	tmp->files=NULL;
	return(tmp);
}
void FreeRDActrn(RDActrn *ctrn)
{
	if(ctrn!=NULL)
	{
		if(ctrn->files!=NULL) Rfree(ctrn->files);
		Rfree(ctrn);
		ctrn=NULL;
	}
}
short addRDActrn(RDActrn *ctrn,short fileno)
{
	if(ctrn->files!=NULL)
	{
		ctrn->files=Rrealloc(ctrn->files,(ctrn->num+1)*sizeof(short));
	} else {
		ctrn->files=Rmalloc(sizeof(short));
	}
	ctrn->files[ctrn->num]=fileno;
	return(0);
}
void xFreeRDAtran(RDAtran *t,int line,char *file)
{
	int x,y;
	RDAtrec *rec;
	RDAtfld *fld;
	RDAtkey *tkey;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	RDActrn *ctrn;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FreeRDAtran Freeing RDAtran Structure at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(t!=NULL)
	{
		ctrn=RDActrnNEW();
		if(t->process!=NULL) Rfree(t->process);
		if(t->user!=NULL) Rfree(t->user);
		if(t->recs!=NULL)
		{
			for(x=0,rec=t->recs;x<t->numrecs;++x,++rec)
			{
				addRDActrn(ctrn,rec->fileno);
				if(rec->module!=NULL) Rfree(rec->module);
				if(rec->file!=NULL) Rfree(rec->file);
				if(rec->keyname!=NULL) Rfree(rec->keyname);
				if(rec->kdata!=NULL)
				{
					key=KEYNUM(rec->fileno,rec->keyno);
					if(key!=NULL)
					{
						for(y=0,tkey=rec->kdata,part=key->part;y<key->numparts;
							++y,++part,++tkey)
						{
							field=FLDNRD(rec->fileno,part->name);
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
									case ZIPCODE:
									case PHONE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
									case CHARACTERS:
									case BOOLNS:
										if(tkey->data.string_value!=NULL) Rfree(tkey->data.string_value);
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										if(tkey->data.float_value!=NULL) Rfree(tkey->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										if(tkey->data.short_value!=NULL) Rfree(tkey->data.short_value);
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(tkey->data.integer_value!=NULL) Rfree(tkey->data.integer_value);
										break;
									default:
										prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(rec->fileno),FILENAME(rec->fileno),line,file);
										break;
								}
							} else {
								prterr("Error Field Information cannot be obtained for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",part->name,MODULENAME(rec->fileno),FILENAME(rec->fileno),line,file);
							}
						}
					} else {
						prterr("Error Key Data cannot be obtained for Key [%d] Module [%s] File [%s] at line [%d] program [%s].",rec->keyno,MODULENAME(rec->fileno),FILENAME(rec->fileno),line,file);
					}
					Rfree(rec->kdata);
				}
				if(rec->fld!=NULL)
				{
					for(y=0,fld=rec->fld;y<rec->numflds;++y,++fld)
					{
						if(fld->name!=NULL) Rfree(fld->name);	
						switch(fld->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
							case CHARACTERS:
							case BOOLNS:
								if(fld->from.string_value!=NULL) Rfree(fld->from.string_value);
								if(fld->to.string_value!=NULL) Rfree(fld->to.string_value);
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								if(fld->from.float_value!=NULL) Rfree(fld->from.float_value);
								if(fld->to.float_value!=NULL) Rfree(fld->to.float_value);
								break;
							case SHORTV:
							case SSHORTV:
								if(fld->from.short_value!=NULL) Rfree(fld->from.short_value);
								if(fld->to.short_value!=NULL) Rfree(fld->to.short_value);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								if(fld->from.integer_value!=NULL) Rfree(fld->from.integer_value);
								if(fld->to.integer_value!=NULL) Rfree(fld->to.integer_value);
								break;
							default:
								prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",fld->type,fld->name,MODULENAME(rec->fileno),FILENAME(rec->fileno),line,file);
								break;
						}
					}
					Rfree(rec->fld);
				}
			}
			Rfree(t->recs);
		}
		Rfree(t);
		for(x=0;x<ctrn->num;++x) 
		{
			if(ctrn->files[x]!=(-1)) CLSNRD(ctrn->files[x]);
		}
		FreeRDActrn(ctrn);
	}
}
RDAtran *xRDAtranNEW(char *process,char *user,int line,char *file)
{
	RDAtran *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG RDAtranNEW Creating Transaction Structure for process [%s] user [%s] at line [%d] program [%s].",(process!=NULL ? process:""),(user!=NULL ? user:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAtran));
	if(process!=NULL) tmp->process=stralloc(process);
		else tmp->process=NULL;
	if(user!=NULL) tmp->user=stralloc(user);
		else tmp->user=NULL;
	tmp->able=TRUE;
	tmp->complete=FALSE;
	tmp->datetime=NULL;
	tmp->endtime=NULL;
	tmp->numrecs=0;
	tmp->recs=NULL;
	return(tmp);
}
char *READFIELD(FILE *fp,short *exit)
{
	int count=0;
	char *tread=NULL,*tmp=NULL,quotes=FALSE;
	char *prevtread=NULL;

	if(!*exit)
	{
		tread=Rmalloc(2);
		prevtread=Rmalloc(2);
		memset(prevtread,0,2);
		while(TRUE)
		{
			if(!fread(tread,sizeof(char),1,fp)) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans) { prterr("DIAG READFIELD reached end of file."); }
#endif /* USE_RDA_DIAGNOSTICS */
				*exit=TRUE;
				break;
			}
			if(!quotes && tread[0]=='\"' && count==0) 
			{
				quotes=TRUE;
			} else if(quotes && strchr("\"",tread[0]) && !strchr("\\",prevtread[0]))
			{
				break;
			} else if(quotes && strchr("\t",tread[0]) && strchr("\"",prevtread[0]))
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]) && count>0) 
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]))
			{
/*
			if(!quotes && tread[0]=='\"') 
			{
				quotes=TRUE;
			} else if(quotes && strchr("\"",tread[0]))
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]) && count>0) 
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]))
			{
*/
			} else {
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+2);
				} else tmp=Rmalloc(2);
				tmp[count]=tread[0];
				tmp[count+1]=0;
				++count;
			}
			memset(prevtread,tread[0],1);
			memset(tread,0,2);
		}
		if(tread!=NULL) Rfree(tread);
		if(prevtread!=NULL) Rfree(prevtread);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG READFIELD returning [%s].",(tmp!=NULL ? tmp:"NULL"));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=reset_quotes(tmp);
	return(tmp);
}
RDAtkey *READKEYS(FILE *fp,short *exit,short fileno,short keyno)
{
	int x;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	RDAtkey *kdata=NULL,*tmp;
	char *fdata=NULL;

	if(fileno!=(-1))
	{
	key=KEYNUM(fileno,keyno);
	if(key!=NULL)
	{
		kdata=Rmalloc(key->numparts*sizeof(RDAtkey));
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			tmp=kdata+x;
			field=FLDNRD(fileno,part->name);
			fdata=READFIELD(fp,exit);
			if(*exit)
			{
				prterr("Error Exiting early from READKEYS.  Suspect a corrupted or partial transaction log.");
				break;
			}
			if(field!=NULL)
			{
				switch(field->type)
				{
					case VARIABLETEXT:
					case PLAINTEXT:
					case SCROLLEDTEXT:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case PHONE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case CUSTOMTYPE:
					case ZIPCODE:
						if(fdata!=NULL)
						{
							tmp->data.string_value=stralloc(fdata);
						} else tmp->data.string_value=NULL;
						break;
					case CHARACTERS:
						tmp->data.string_value=Rmalloc(1);
						if(fdata!=NULL)
						{
							*tmp->data.string_value=fdata[0];
						} else *tmp->data.string_value=0;
						break;
					case BOOLNS:
						tmp->data.string_value=Rmalloc(1);
						if(fdata!=NULL)
						{
							*tmp->data.string_value=Ratoi(fdata);
						} else *tmp->data.string_value=0;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						tmp->data.float_value=Rmalloc(sizeof(double));
						if(fdata!=NULL)
						{
							*tmp->data.float_value=atof(fdata);
						} else *tmp->data.float_value=0;
						break;
					case SHORTV:
					case SSHORTV:
						tmp->data.short_value=Rmalloc(sizeof(short));
						if(fdata!=NULL)
						{
							*tmp->data.short_value=Ratoi(fdata);
						} else *tmp->data.short_value=0;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						tmp->data.integer_value=Rmalloc(sizeof(int));
						if(fdata!=NULL)
						{
							*tmp->data.integer_value=Ratoi(fdata);
						} else *tmp->data.integer_value=0;
						break;
					case BUTTONS:
						prterr("Error no transaction processing for this type [%d] [%s].",field->type,fieldtypedesc[field->type]);
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
						break;
				}
			} else {	
				prterr("Error Field [%s] cannot be found in Module [%s] File [%s].",part->name,MODULENAME(fileno),FILENAME(fileno));
			}
			if(fdata!=NULL) Rfree(fdata);
		}
	} else {
		prterr("Error Key Data couldn't be found for Module [%s] File [%s] Key no [%d] [%s].",MODULENAME(fileno),FILENAME(fileno),keyno,KEYNAME(fileno,keyno));
	}
	return(kdata);
	} else {
		prterr("Error Data File not opened, Key Data couldn't be found for Module [%s] File [%s].",MODULENAME(fileno),FILENAME(fileno));
	 	return(NULL);
	}
}
short opengettranfiles(RDAtran *t,char *module,char *filename)
{
	int x;
	RDAtrec *trec;

	if(t!=NULL)
	{
		if(t->recs!=NULL)
		{
			for(x=0,trec=t->recs;x<t->numrecs;++x,++trec)
			{
				if(!RDAstrcmp(trec->file,filename) && !RDAstrcmp(trec->module,module)) return(trec->fileno);
			}
		}
	}
	return(OPNNRD(module,filename));
}
void xCLOSEtranFiles(RDAtran *t,int line,char *file)
{
	int x,y;
	RDAtrec *trec=NULL,*ttrec=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG CLOSEtranFiles Closing Files Open in a Transaction at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(t!=NULL)
	{
		for(x=0;x<t->numrecs;++x)
		{
			trec=t->recs+x;
			if(trec->fileno!=(-1)) 
			{
				for(y=x+1;y<t->numrecs;++y)
				{
					ttrec=t->recs+y;
					if(ttrec->fileno==trec->fileno) 
					{
						ttrec->fileno=(-1);
					}
				}
				CLSNRD(trec->fileno);
				trec->fileno=(-1);
			}
		}
	}
}
RDAtran *xGETTRANSACTION(char *filename,int line,char *file)
{
	FILE *fp;
	RDAtran *tran=NULL;
	RDAtrec *trec=NULL;
	RDAtfld *tfld=NULL;
	NRDfield *field;
	char *tmp,*from=NULL,*to=NULL;
	short exit=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG GETTRANSACTION Reading transaction [%s] and returning an RDAtran Structure at line [%d] program [%s].",filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	fp=fopen(filename,"r");
	if(fp!=NULL)
	{
		tran=RDAtranNEW(NULL,NULL);
		tran->datetime=READFIELD(fp,&exit);
		tran->process=READFIELD(fp,&exit);
		tran->user=READFIELD(fp,&exit);
		if(exit) { prterr("Error Transaction [%s] is incomplete, returning early to line [%d] program [%s].",filename,line,file); }
		while(!exit)
		{
			tmp=READFIELD(fp,&exit);
			if(!RDAstrcmp(tmp,"END TRANSACTION"))
			{
				tran->endtime=READFIELD(fp,&exit);
				tran->complete=TRUE;
				break;
			} else if(!RDAstrcmp(tmp,"N"))
			{
				if(tran->recs!=NULL)
				{
					tran->recs=Rrealloc(tran->recs,(tran->numrecs+1)*sizeof(RDAtrec));
				} else {
					tran->recs=Rmalloc(sizeof(RDAtrec));
				}
				trec=tran->recs+tran->numrecs;
				memset(trec,0,sizeof(RDAtrec));
				trec->type='N';
				trec->module=READFIELD(fp,&exit);
				trec->file=READFIELD(fp,&exit);
				trec->fileno=opengettranfiles(tran,trec->module,trec->file);
				trec->keyname=READFIELD(fp,&exit);
				if(trec->fileno==(-1))
				{
					exit=TRUE;
				} else {
					trec->keyno=KEYNUMBER(trec->fileno,trec->keyname);
					trec->kdata=READKEYS(fp,&exit,trec->fileno,trec->keyno);
				}
				++tran->numrecs;
				trec->numflds=0;
				trec->fld=NULL;
			} else if(!RDAstrcmp(tmp,"C"))
			{
				if(tran->recs!=NULL)
				{
					tran->recs=Rrealloc(tran->recs,(tran->numrecs+1)*sizeof(RDAtrec));
				} else {
					tran->recs=Rmalloc(sizeof(RDAtrec));
				}
				trec=tran->recs+tran->numrecs;
				memset(trec,0,sizeof(RDAtrec));
				trec->type='C';
				trec->module=READFIELD(fp,&exit);
				trec->file=READFIELD(fp,&exit);
				trec->fileno=opengettranfiles(tran,trec->module,trec->file);
				trec->keyname=READFIELD(fp,&exit);
				if(trec->fileno==(-1))
				{
					exit=TRUE;
				} else {
					trec->keyno=KEYNUMBER(trec->fileno,trec->keyname);
					trec->kdata=READKEYS(fp,&exit,trec->fileno,trec->keyno);
				} 
				++tran->numrecs;
				trec->numflds=0;
				trec->fld=NULL;
			} else if(!RDAstrcmp(tmp,"F"))
			{
				trec=tran->recs+(tran->numrecs-1);
				if(trec->fld!=NULL)
				{
					trec->fld=Rrealloc(trec->fld,(trec->numflds+1)*sizeof(RDAtfld));
				} else {
					trec->fld=Rmalloc(sizeof(RDAtfld));
				}
				tfld=trec->fld+trec->numflds;
				memset(tfld,0,sizeof(RDAtfld));
				tfld->name=READFIELD(fp,&exit);	
				field=FLDNRD(trec->fileno,tfld->name);
				from=READFIELD(fp,&exit);
				to=READFIELD(fp,&exit);
				if(field!=NULL)
				{
					tfld->type=field->type;
					tfld->len=field->len;
					switch(field->type)
					{
						case VARIABLETEXT:
						case PLAINTEXT:
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
						case ZIPCODE:
							if(!isEMPTY(from))
							{
								tfld->from.string_value=stralloc(from);
							} else tfld->from.string_value=NULL;
							if(!isEMPTY(to))
							{
								tfld->to.string_value=stralloc(to);
							} else tfld->to.string_value=NULL;
							break;
						case CHARACTERS:
							tfld->from.string_value=Rmalloc(1);
							*tfld->from.string_value=from[0];
							tfld->to.string_value=Rmalloc(1);
							*tfld->to.string_value=to[0];
							break;
						case BOOLNS:
							tfld->from.string_value=Rmalloc(1);
							if(!isEMPTY(from))
							{
							*tfld->from.string_value=Ratoi(from);
							} else *tfld->from.string_value=0;
							tfld->to.string_value=Rmalloc(1);
							if(!isEMPTY(to))
							{
							*tfld->to.string_value=Ratoi(to);
							} else *tfld->to.string_value=0;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							tfld->from.float_value=Rmalloc(sizeof(double));
							tfld->to.float_value=Rmalloc(sizeof(double));
							if(!isEMPTY(from))
							{
							*tfld->from.float_value=atof(from);
							} else *tfld->from.float_value=0;
							if(!isEMPTY(to))
							{
							*tfld->to.float_value=atof(to);
							} else *tfld->to.float_value=0;
							break;
						case SHORTV:
						case SSHORTV:
							tfld->from.short_value=Rmalloc(sizeof(short));
							tfld->to.short_value=Rmalloc(sizeof(short));
							if(!isEMPTY(from))
							{
							*tfld->from.short_value=Ratoi(from);
							} else *tfld->from.short_value=0;
							if(!isEMPTY(to))
							{
							*tfld->to.short_value=Ratoi(to);
							} else *tfld->to.short_value=0;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							tfld->from.integer_value=Rmalloc(sizeof(int));
							tfld->to.integer_value=Rmalloc(sizeof(int));
							if(!isEMPTY(from))
							{
							*tfld->from.integer_value=Ratoi(from);
							} else *tfld->from.integer_value=0;
							if(!isEMPTY(to))
							{
							*tfld->to.integer_value=Ratoi(to);
							} else *tfld->to.integer_value=0;
							break;
						case BUTTONS:
							prterr("Error no transaction processing for this type [%d] [%s].",field->type,fieldtypedesc[field->type]);
							break;
						default:
							prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(trec->fileno),FILENAME(trec->fileno),line,file);
							break;
					}
				} else {
					prterr("Error Field [%s] not found in Module [%s] File [%s] at line [%d] program [%s].",tfld->name,MODULENAME(trec->fileno),FILENAME(trec->fileno),line,file);
				}
				if(from!=NULL) Rfree(from);
				if(to!=NULL) Rfree(to);
				++trec->numflds;
			}
			if(exit) 
			{ 
				prterr("Error Transaction [%s] is Incomplete returning unexpectantly to line [%d] program [%s].",filename,line,file); 
				break;
			}	
		}
		fclose(fp);
	}
	CLOSEtranFiles(tran);
	return(tran);
}
void xSEERDAtran(RDAtran *t,int line,char *file)
{
	int x,y;
	RDAtrec *trec;
	RDAtfld *tfld;

	prterr("DIAG SEERDAtran Showing Contents of RDAtran Structure at line [%d] program [%s].",line,file);
	if(t!=NULL)
	{
		prterr("Process [%s] User [%s] Start [%s] End [%s]\n",
			(t->process!=NULL ? t->process:""),
			(t->user!=NULL ? t->user:""),(t->datetime!=NULL ? 
			t->datetime:""),(t->endtime!=NULL ? t->endtime:""));
		prterr("Number Trans [%d] Able to Undo [%s] Transaction Completed [%s]\n",t->numrecs,(t->able ? "Yes":"No"),(t->complete ? "Yes":"No"));
		for(x=0,trec=t->recs;x<t->numrecs;++x,++trec)
		{
			prterr("Undone [%s] Type [%s] Module [%s] File [%d] [%s] Key [%d] [%s] No Fields [%d]\n",(trec->undone ? "Yes":"No"),
				(trec->type=='N' ? 
				"New Record":"Change Record"),
				(trec->module!=NULL ? trec->module:""),
				trec->fileno,(trec->file!=NULL ? 
				trec->file:""),trec->keyno,(trec->keyname!=NULL
				? trec->keyname:""),trec->numflds);
			if(trec->type=='C')
			{
				for(y=0,tfld=trec->fld;y<trec->numflds;++y,++tfld)
				{
					prterr("Name [%s] type [%d] [%s] len [%d]\n",(tfld->name!=NULL ? tfld->name:""),tfld->type,fieldtypedesc[tfld->type],tfld->len);
				}
			}
		}
	} else {
		prterr("RDAtran Structure is equal to a NULL pointer.\n");
	}
}
char *trectype[2]={"New   ","Change"};
char *makekeystring(short fileno,short keyno,RDAtkey *kdata)
{
	char *string=NULL;
	int x,count=0,length=0;
	NRDfield *tmp;
	NRDkey *key;
	NRDpart *part;
	RDAtkey *keydata;

	if(fileno!=(-1))
	{
	key=KEYNUM(fileno,keyno);
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		keydata=kdata+x;
		tmp=FLDNRD(fileno,part->name);
		if(tmp!=NULL)
		{
			switch(tmp->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					length=RDAstrlen(keydata->data.string_value)+4;
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+length);
					} else {
						string=Rmalloc(length);
					} 
					sprintf(&string[count],"(%s) ",(keydata->data.string_value!=NULL ? keydata->data.string_value:""));
					count+=(length-1);
					break;
				case CHARACTERS:
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+5);
					} else {
						string=Rmalloc(5);
					} 
					sprintf(&string[count],"(%c) ",*keydata->data.string_value);
					count+=4;
					break;
				case BOOLNS:
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+5);
					} else {
						string=Rmalloc(5);
					} 
					sprintf(&string[count],"(%1d) ",*keydata->data.string_value);
					count+=4;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+tmp->len+4);
					} else {
						string=Rmalloc(tmp->len+4);
					} 
					sprintf(&string[count],"(%*f) ",tmp->len,*keydata->data.float_value);
					count+=tmp->len+3;
					break;
				case SHORTV:
				case SSHORTV:
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+tmp->len+4);
					} else {
						string=Rmalloc(tmp->len+4);
					} 
					sprintf(&string[count],"(%*d) ",tmp->len,*keydata->data.short_value);
					count+=tmp->len+3;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+tmp->len+4);
					} else {
						string=Rmalloc(tmp->len+4);
					} 
					sprintf(&string[count],"(%*d) ",tmp->len,*keydata->data.integer_value);
					count+=tmp->len+3;
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",tmp->type,tmp->name,MODULENAME(fileno),FILENAME(fileno));
					break;
			}
		} else {
			prterr("Error Field [%s] not found in Module [%s] File [%s].",part->name,MODULENAME(fileno),FILENAME(fileno));
		}
	}
	string[count-1]=0;
	string[count]=0;
	}
	return(string);
}
/* new functions for file locking below here */
short xUndoRDAtrec(RDAtrec *trec,int line,char *file)
{
	RDAtfld *tfld;
	short af=0;
	int x;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	RDAtkey *tmp;
	char deletef=TRUE;
#ifdef USE_RDA_DIAGNOSTICS
	char *keydata=NULL;

	if(diagmix || diagmix_field || diagtrans)
	{
		keydata=makekeystring(trec->fileno,trec->keyno,trec->kdata);
		prterr("DIAG UndoRDAtrec Undo an RDA Transaction Record for Module [%s] File [%d] [%s] Key [%d] [%s] Key Values [%s] at line [%d] program [%s].",trec->module,trec->fileno,FILENAME(trec->fileno),trec->keyno,KEYNAME(trec->fileno,trec->keyno),
			(keydata!=NULL ? keydata:""),line,file);
		if(keydata!=NULL) Rfree(keydata);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	key=KEYNUM(trec->fileno,trec->keyno);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			tmp=trec->kdata+x;
			field=FLDNRD(trec->fileno,part->name);
			if(field!=NULL)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT: 
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case ZIPCODE:
					case CUSTOMTYPE:
					case PHONE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case PLAINTEXT: /* plain text field type */
						if(!isEMPTY(tmp->data.string_value))
						{
							af=RDAstrlen(tmp->data.string_value);
							field->data.string_value=Rrealloc(field->data.string_value,af+1);
							field->dlen=af+1;
							memcpy(field->data.string_value,tmp->data.string_value,af+1);
						} else if(field->dlen>0)
						{
							memset(field->data.string_value,0,field->dlen);
						} else {
							field->data.string_value=NULL;
							field->dlen=0;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						if(field->data.string_value==NULL)
						{
							field->data.string_value=Rmalloc(1);
							field->dlen=1;
						}
						*field->data.string_value=*tmp->data.string_value;
						break;
					case DOUBLEV:
					case SDOUBLEV:
					case DECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						*field->data.float_value=*tmp->data.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						*field->data.short_value=*tmp->data.short_value;
						break;
					case LONGV:
					case SLONGV:
						*field->data.integer_value=*tmp->data.integer_value;
						break;
					default:
						prterr("Error Unrecognized field type [%d] on field [%s] File [%d] [%s] at line [%d] program [%s].",field->type,field->name,trec->fileno,FILENAME(trec->fileno),line,file);
						break;
				}
			}
		}
		if(!EQLNRDsec(trec->fileno,trec->keyno))
		{
			if(trec->type=='C')
			{
				for(x=0,tfld=trec->fld;x<trec->numflds;++x,++tfld)
				{
					field=FLDNRD(trec->fileno,tfld->name);
					if(field!=NULL)
					{
						switch(field->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT: 
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case ZIPCODE:
							case CUSTOMTYPE:
							case PHONE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case PLAINTEXT: /* plain text field type */
								if(!isEMPTY(tfld->from.string_value))
								{
									af=RDAstrlen(tfld->from.string_value);
									field->data.string_value=Rrealloc(field->data.string_value,af+1);
									field->dlen=af+1;
									memcpy(field->data.string_value,tfld->from.string_value,af+1);
								} else if(field->dlen>0)
								{
									memset(field->data.string_value,0,field->dlen);
								} else {
									field->data.string_value=NULL;
									field->dlen=0;
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								if(field->data.string_value==NULL)
								{
									field->data.string_value=Rmalloc(1);
									field->dlen=1;
								} 
								*field->data.string_value=
									*tfld->from.string_value;
								break;
							case DOUBLEV:
							case SDOUBLEV:
							case DECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								*field->data.float_value=
									*tfld->from.float_value;
								break;
							case SHORTV:
							case SSHORTV:
								*field->data.short_value=
									*tfld->from.short_value;
								break;
							case LONGV:
							case SLONGV:
								*field->data.integer_value=
									*tfld->from.integer_value;
								break;
							default:
								prterr("Error Unrecognized field type [%d] on field [%s] File [%d] [%s] at line [%d] program [%s].",field->type,field->name,trec->fileno,FILENAME(trec->fileno),line,file);
								break;
						}
					}
				}
			} else {
				FINDFLDSETCHAR(trec->fileno,"DELETEFLAG",deletef);
			}
			WRTNRDtranssec(trec->fileno,0,NULL);
			return(0);
		} else {
			prterr("Error Corresponding Record not Found in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(trec->fileno),trec->fileno,FILENAME(trec->fileno),line,file);
			return(-1);
		}
	} else {
		prterr("Error Key [%d] not Found in Module [%s] File [%d] [%s] at line [%d] program [%s].",trec->keyno,MODULENAME(trec->fileno),trec->fileno,FILENAME(trec->fileno),line,file);
		return(-1);
	}
}
short getverifyfile(RDAtran *t,RDAtrec *trec)
{
	int x;
	RDAtrec *rec;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field || diagtrans)
	{
		prterr("DIAG getverifyfile Verifing Access to the Files Associated with this Transaction.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(t->numrecs>0)
	{
		for(x=0,rec=t->recs;x<t->numrecs-1;++x,++rec)
		{
			if(!RDAstrcmp(rec->file,trec->file) && 
				!RDAstrcmp(rec->module,trec->module) && rec->fileno!=(-1)) 
			{
				trec->fileno=rec->fileno;
				trec->keyno=KEYNUMBER(trec->fileno,trec->keyname);
				return(0);
			}
		}
	}
	trec->fileno=OPNNRDsec(trec->module,trec->file,TRUE,TRUE,FALSE);
	if(trec->fileno==(-1))
	{
		trec->fileno=OPNNRDsec(trec->module,trec->file,TRUE,FALSE,FALSE);
		if(trec->fileno==(-1)) 
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagmix || diagmix_field || diagtrans)
			{
				prterr("Error Security Access is denied to Module [%s] File [%s] and the corresponding transaction.",trec->module,trec->file); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	}
	trec->keyno=KEYNUMBER(trec->fileno,trec->keyname);
	return(0);
}
short xVERIFYTRANSACTION(RDAtran *tran,short force,int line,char *file)
{
	short able=TRUE,test,af=0;
	int x,y;
	RDAtrec *trec;
	RDAtfld *tfld;
	RDAtkey *tmp;
	NRDfield *field;
	NRDkey *key=NULL;
	NRDpart *part;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG VERIFYTRANSACTION at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tran->able=TRUE;
	if(tran!=NULL && tran->numrecs>0)
	{
		for(y=0,trec=tran->recs;y<tran->numrecs;++y,++trec)
		{
			trec->fileno=(-1);
			trec->keyno=(-1);
			test=getverifyfile(tran,trec);
/*
			if(test==(-1) && !force) 
*/
			if(test==(-1)) 
			{
				tran->able=FALSE;
				prterr("Error Verify Transaction is aborting with Access Denied to the Datafiles at line [%d] program [%s].",line,file);
				return(-2); /* All Files can't be opened, maybe a security problem or missing/deleted file */
			} else if(test) 
			{
				tran->able=FALSE;
			}
			ZERNRD(trec->fileno);
			key=KEYNUM(trec->fileno,trec->keyno);
			if(key!=NULL)
			{
				ZERNRD(trec->fileno);
				for(x=0,part=key->part;x<key->numparts;++x,++part)
				{
					tmp=trec->kdata+x;
					field=FLDNRD(trec->fileno,part->name);
					if(field!=NULL)
					{
						switch(field->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT: 
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case ZIPCODE:
							case CUSTOMTYPE:
							case PHONE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case PLAINTEXT: /* plain text field type */
								if(!isEMPTY(tmp->data.string_value))
								{
									af=RDAstrlen(tmp->data.string_value);
									field->data.string_value=Rrealloc(field->data.string_value,af+1);
									field->dlen=af+1;
									memcpy(field->data.string_value,tmp->data.string_value,af+1);
								} else if(field->dlen>0)
								{
									memset(field->data.string_value,0,field->dlen);
								} else {
									field->data.string_value=NULL;
									field->dlen=0;
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								if(field->data.string_value==NULL)
								{
									field->data.string_value=Rmalloc(1);
									field->dlen=1;
								}
								*field->data.string_value=
									*tmp->data.string_value;
								break;
							case DOUBLEV:
							case SDOUBLEV:
							case DECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								*field->data.float_value=
									*tmp->data.float_value;
								break;
							case SHORTV:
							case SSHORTV:
								*field->data.short_value=
									*tmp->data.short_value;
								break;
							case LONGV:
							case SLONGV:
								*field->data.integer_value=
									*tmp->data.integer_value;
								break;
							default:
								prterr("Error Unrecognized field type [%d] on field [%s] File [%d] [%s] at line [%d] program [%s].",field->type,field->name,trec->fileno,FILENAME(trec->fileno),line,file);
								break;
						}
					}
				}
				if(EQLNRDsec(trec->fileno,trec->keyno)) 
				{
					tran->able=FALSE;		
#ifdef USE_RDA_DIAGNOSTICS
					if(diagmix || diagmix_field || diagtrans)
					{
						prterr("Error Verify Transaction is aborting with Can't Find Record in Module [%s] File [%d] [%s] key [%d] at line [%d] program [%s].",MODULENAME(trec->fileno),trec->fileno,FILENAME(trec->fileno),trec->keyno,line,file);
						SEENRDRECORD(trec->fileno);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return(-1); /* Can't find Record */
/*
					if(!force) return(-1);
*/
				} else {
					if(trec->type=='C')
					{
						for(x=0,tfld=trec->fld;x<trec->numflds;++x,++tfld)
						{
							field=FLDNRD(trec->fileno,tfld->name);
							if(field!=NULL)
							{
								switch(tfld->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT: 
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case ZIPCODE:
									case CUSTOMTYPE:
									case PHONE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case PLAINTEXT: /* plain text field type */
										if(RDAstrcmp(field->data.string_value,
											tfld->to.string_value)) 
										{
											tran->able=FALSE;
										}
										break;
									case BOOLNS:
									case CHARACTERS:
										if(*field->data.string_value!=
											*tfld->to.string_value) 
											tran->able=FALSE;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
/*  SJS FOUND THIS SUPPOSED ERROR on 10/30/97 from PDF problem
										break;
*/
									case DOUBLEV:
									case SDOUBLEV:
									case DECIMALV:
									case SDECIMALV:
										if(*field->data.float_value!=
											*tfld->to.float_value) 
											tran->able=FALSE;
										break;
									case SHORTV:
									case SSHORTV:
										if(*field->data.short_value!=
											*tfld->to.short_value) 
											tran->able=FALSE;
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										if(*field->data.integer_value!=
											*tfld->to.integer_value) 
											tran->able=FALSE;
										break;
									default:
										prterr("Error Unrecognized field type.");
										break;
								}
							} else {
								prterr("Error Field [%s] not found in Module [%s] File [%d] [%s] at line [%d] program [%s].",tfld->name,MODULENAME(trec->fileno),trec->fileno,FILENAME(trec->fileno),line,file);
							}
						}
					}
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field || diagtrans)
	{
		prterr("DIAG VERIFYTRANSACTION is returning [%s] at line [%d] program [%s].",(able ? "Yes":"No"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(able);
}

void quitshow(RDArsrc *showrsrc)
{
	if(showrsrc!=NULL) free_rsrc(showrsrc);
	showrsrc=NULL;
}
void undopart(RDArsrc *showrsrc,RDArsrc *viewrsrc)
{
	int selected=0;
	RDAtrec *trec=NULL;

	readwidget(viewrsrc,"TRANSACTION LIST");
	if(FINDRSCGETINT(viewrsrc,"TRANSACTION LIST",&selected)) return;
	if(tran->numrecs>0)
	{
		trec=tran->recs+selected;
		if(!trec->undone)
		{
			UndoRDAtrec(trec);
			trec->undone=TRUE;
			if(trnlst!=NULL) freeapplib(trnlst);
			trnlst=maketrnlst();
			if(trnlst==NULL)
			{
				trnlst=APPlibNEW();
				addAPPlib(trnlst,"Contains No Transactions");
			}
			if(!FINDRSCLISTAPPlib(viewrsrc,"TRANSACTION LIST",0,trnlst)) updatersrc(viewrsrc,"TRANSACTION LIST");
		}
	}
	quitshow(showrsrc);
}
void quitview(RDArsrc *viewrsrc)
{
	if(viewrsrc!=NULL) free_rsrc(viewrsrc);
	viewrsrc=NULL;
	if(tran!=NULL) FreeRDAtran(tran);
	tran=NULL;
}
void fixtrnoutput(FILE *fp,char *data,int *lines,int *pages,unsigned line_length,
		char *modname,char *trnnum,char *process,char *user)
{
	unsigned y=0,x=0,length=0;
	char *temp=NULL,*temp1=NULL;

	if(!isEMPTY(data))
	{
		if((*lines+1)>58)
		{
			fprintf(fp,"\f");
			temp=stralloc("XPERT TRANSACTION PROCESSING");
			printheader(fp,lines,pages,temp,line_length,modname,trnnum,process,user);
			if(temp!=NULL) Rfree(temp);
		}
		++*lines;
		temp=strchr(data,'\n');
		temp1=strchr(data,'\r');
		if(temp==NULL && temp1==NULL) length=RDAstrlen(data);
		else if(temp==NULL) length=temp1-data;
		else if(temp1==NULL) length=temp-data;
		else if(temp<temp1) length=temp-data;
		else length=temp1-data;
		for(x=0,y=0;x<length;++x,++y)
		{
			if(data[x]=='\t')
			{
				if(y+9>line_length) break;
				y+=7;
			}
			if(y+2>line_length) break;
		}
		if(x<length)
		{
			fprintf(fp,"%.*s\n",x+1,data);
			fixtrnoutput(fp,data+x+1,lines,pages,line_length,modname,trnnum,process,user);
		} else {
			fprintf(fp,"%.*s\n",length,data);
			if(temp!=NULL || temp1!=NULL) fixtrnoutput(fp,data+length+1,lines,pages,line_length,modname,trnnum,process,user);
		}
	} 
}
static void printdetail(FILE *fp,int *lines,int *pages,unsigned line_length,
			char *modname,char *trnnum,char *process,char *user)
{
	int x;
	char *header=NULL,*transdesc=NULL;

	for(x=0;x<trnlst->numlibs;++x)
	{
		if(*lines+1>58)
		{
			fprintf(fp,"\f");
			header=stralloc("XPERT TRANSACTION PROCESSING");
			printheader(fp,lines,pages,header,line_length,modname,trnnum,process,user);
			if(header!=NULL) Rfree(header);
		}
		fprintf(fp,"%s\n",trnlst->libs[x]);
		++*lines;
		transdesc=maketransdesc(x,FALSE);
		fixtrnoutput(fp,transdesc,lines,pages,line_length,modname,trnnum,process,user);
		if(transdesc!=NULL) Rfree(transdesc);
		fprintf(fp,"\n");
		++*lines;
	}
}
char *readlistmaketransdesc(RDArsrc *viewrsrc)
{
	int selected=0;

	readwidget(viewrsrc,"TRANSACTION LIST");
	if(FINDRSCGETINT(viewrsrc,"TRANSACTION LIST",&selected)) return(NULL);
	return(maketransdesc(selected,TRUE));
}
char *makechangedesc(RDAtrec *trec,char include_type)
{
	char *tmp=NULL;
	int x,count=0,length=0;
	RDAtfld *tfld;
	char *temp1=NULL,*temp2=NULL;

	for(x=0,tfld=trec->fld;x<trec->numflds;++x,++tfld)
	{
		switch(tfld->type)
		{
			case SCROLLEDTEXT:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case VARIABLETEXT:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(include_type) length=RDAstrlen(tfld->name)+RDAstrlen(standardfieldtypes[tfld->type])+(tfld->from.string_value!=NULL ? RDAstrlen(tfld->from.string_value):0)+(tfld->to.string_value!=NULL ? RDAstrlen(tfld->to.string_value):0)+69;
				else length=RDAstrlen(tfld->name)+(tfld->from.string_value!=NULL ? RDAstrlen(tfld->from.string_value):0)+(tfld->to.string_value!=NULL ? RDAstrlen(tfld->to.string_value):0)+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%s]\n\tResulting Value: [%s]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,(tfld->from.string_value!=NULL ? tfld->from.string_value:""),
					(tfld->to.string_value!=NULL ? tfld->to.string_value:""));
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%s]\n\t\tResulting Value: [%s]\n",tfld->name,(tfld->from.string_value!=NULL ? tfld->from.string_value:""),(tfld->to.string_value!=NULL ? tfld->to.string_value:""));
				break;
			case CHARACTERS:
				if(include_type) length=RDAstrlen(tfld->name)+RDAstrlen(standardfieldtypes[tfld->type])+69;
				else length=RDAstrlen(tfld->name)+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%c]\n\tResulting Value: [%c]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,*tfld->from.string_value,*tfld->to.string_value);
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%c]\n\t\tResulting Value: [%c]\n",tfld->name,*tfld->from.string_value,*tfld->to.string_value);
				break;
			case BOOLNS:
				if(include_type) length=RDAstrlen(tfld->name)+RDAstrlen(standardfieldtypes[tfld->type])+RDAstrlen(bolstring[*tfld->from.string_value])+RDAstrlen(bolstring[*tfld->to.string_value])+69;
				else length=RDAstrlen(tfld->name)+RDAstrlen(bolstring[*tfld->from.string_value])+RDAstrlen(bolstring[*tfld->to.string_value])+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%s]\n\tResulting Value: [%s]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,bolstring[*tfld->from.string_value],bolstring[*tfld->to.string_value]);
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%s]\n\t\tResulting Value: [%s]\n",tfld->name,bolstring[*tfld->from.string_value],bolstring[*tfld->to.string_value]);
				break;
			case DOLLARS:
				temp1=famt(*tfld->from.float_value,tfld->len);
				temp2=famt(*tfld->to.float_value,tfld->len);
				if(include_type) length=RDAstrlen(tfld->name)+RDAstrlen(temp1)+RDAstrlen(temp2)+RDAstrlen(standardfieldtypes[tfld->type])+69;
				else length=RDAstrlen(tfld->name)+RDAstrlen(temp1)+RDAstrlen(temp2)+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%s]\n\tResulting Value: [%s]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,temp1,temp2);
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%s]\n\t\tResulting Value: [%s]\n",tfld->name,temp1,temp2);
				if(temp1!=NULL) Rfree(temp1);
				if(temp2!=NULL) Rfree(temp2);
				break;
			case DECIMALV:
			case SDECIMALV:
				sprintf(stemp,"%*f",tfld->len,*tfld->from.float_value);
				length=RDAstrlen(stemp);
				sprintf(stemp,"%*f",tfld->len,*tfld->to.float_value);
				length+=RDAstrlen(stemp);
				if(include_type) length+=RDAstrlen(tfld->name)+RDAstrlen(standardfieldtypes[tfld->type])+69;
				else length+=RDAstrlen(tfld->name)+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*f]\n\tResulting Value: [%*f]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,tfld->len,*tfld->from.float_value,tfld->len,*tfld->to.float_value);
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%*f]\n\t\tResulting Value: [%*f]\n",tfld->name,tfld->len,*tfld->from.float_value,tfld->len,*tfld->to.float_value);
				break;
			case DOUBLEV:
			case SDOUBLEV:
				if(include_type) length=RDAstrlen(tfld->name)+(2*tfld->len)+RDAstrlen(standardfieldtypes[tfld->type])+69;
				else length=RDAstrlen(tfld->name)+(2*tfld->len)+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*.0f]\n\tResulting Value: [%*.0f]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,tfld->len,*tfld->from.float_value,tfld->len,*tfld->to.float_value);
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%*.0f]\n\t\tResulting Value: [%*.0f]\n",tfld->name,tfld->len,*tfld->from.float_value,tfld->len,*tfld->to.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				if(include_type) length=RDAstrlen(tfld->name)+RDAstrlen(standardfieldtypes[tfld->type])+(2*tfld->len)+69;
				else length=RDAstrlen(tfld->name)+(2*tfld->len)+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*d]\n\tResulting Value: [%*d]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,tfld->len,*tfld->from.short_value,tfld->len,*tfld->to.short_value);
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%*d]\n\t\tResulting Value: [%*d]\n",tfld->name,tfld->len,*tfld->from.short_value,tfld->len,*tfld->to.short_value);
				break;
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				if(include_type) length=RDAstrlen(tfld->name)+RDAstrlen(standardfieldtypes[tfld->type])+(2*tfld->len)+69;
				else length=RDAstrlen(tfld->name)+(2*tfld->len)+54;
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+length);
				} else {
					tmp=Rmalloc(length);
				}
				if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*d]\n\tResulting Value: [%*d]\n",tfld->name,standardfieldtypes[tfld->type],tfld->len,tfld->len,*tfld->from.integer_value,tfld->len,*tfld->to.integer_value);
				else sprintf(&tmp[count],"\tField: %s\n\t\t  Initial Value: [%*d]\n\t\tResulting Value: [%*d]\n",tfld->name,tfld->len,*tfld->from.integer_value,tfld->len,*tfld->to.integer_value);
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error unrecognized field type.");
				break;
		}
		count+=length-1;
	}
	return(tmp);
}
char *makenewdesc(RDAtrec *trec,char include_type)
{
	char *tmp=NULL;
	int x,count=0,length=0;
	char *temp1=NULL;
	short numflds,af=0;
	RDAtkey *tkey;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;
	

	if(!TESTFILENO(trec->fileno,__LINE__,__FILE__)) return(NULL);
	key=KEYNUM(trec->fileno,trec->keyno);
	if(key==NULL) return(NULL);
	ZERNRD(trec->fileno);
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		tkey=trec->kdata+x;
		field=FLDNRD(trec->fileno,part->name);
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT: 
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case ZIPCODE:
				case CUSTOMTYPE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case PLAINTEXT: /* plain text field type */
					if(tkey->data.string_value!=NULL)
					{
						af=RDAstrlen(tkey->data.string_value);
						field->data.string_value=
							Rrealloc(field->data.string_value,af+1);
						field->dlen=af;
						memcpy(field->data.string_value,
							tkey->data.string_value,af+1);
					} else if(field->dlen>0)
					{
						memset(field->data.string_value,0,field->dlen+1);
					} else {
						field->data.string_value=NULL;
						field->dlen=0;
					}
					break;
				case BOOLNS:
				case CHARACTERS:
					if(field->data.string_value==NULL)
					{
						field->data.string_value=Rmalloc(1);
						field->dlen=1;
					}
					*field->data.string_value=*tkey->data.string_value;
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case DECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					*field->data.float_value=*tkey->data.float_value;
					break;
				case SHORTV:
				case SSHORTV:
					*field->data.short_value=*tkey->data.short_value;
					break;
				case LONGV:
				case SLONGV:
					*field->data.integer_value=*tkey->data.integer_value;
					break;
				default:
					prterr("Error Unrecognized field type [%d] on field [%s] File [%d] [%s].",field->type,field->name,trec->fileno,FILENAME(trec->fileno));
					break;
			}
		}
	}
	if(EQLNRDsec(trec->fileno,trec->keyno)) return(NULL);
	numflds=NUMFLDS(trec->fileno);
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(trec->fileno,x);
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case SOCSECNUM:
				case DATES:
				case TIMES:
				case VARIABLETEXT:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(include_type) length=RDAstrlen(field->name)+RDAstrlen(standardfieldtypes[field->type])+(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0)+38;
					else length=RDAstrlen(field->name)+(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0)+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%s]\n",field->name,standardfieldtypes[field->type],field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%s]\n",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
					break;
				case CHARACTERS:
					if(include_type) length=RDAstrlen(field->name)+RDAstrlen(standardfieldtypes[field->type])+38;
					else length=RDAstrlen(field->name)+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%c]\n",field->name,standardfieldtypes[field->type],field->len,*field->data.string_value);
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%c]\n",field->name,*field->data.string_value);
					break;
				case BOOLNS:
					if(include_type) length=RDAstrlen(field->name)+RDAstrlen(standardfieldtypes[field->type])+RDAstrlen(bolstring[*field->data.string_value])+38;
					else length=RDAstrlen(field->name)+RDAstrlen(bolstring[*field->data.string_value])+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%s]\n",field->name,standardfieldtypes[field->type],field->len,bolstring[*field->data.string_value]);
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%s]\n",field->name,bolstring[*field->data.string_value]);
					break;
				case DOLLARS:
					temp1=famt(*field->data.float_value,field->len);
					if(include_type) length=RDAstrlen(field->name)+RDAstrlen(temp1)+RDAstrlen(standardfieldtypes[field->type])+38;
					else length=RDAstrlen(field->name)+RDAstrlen(temp1)+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%s]\n",field->name,standardfieldtypes[field->type],field->len,temp1);
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%s]\n",field->name,temp1);
					if(temp1!=NULL) Rfree(temp1);
					break;
				case DECIMALV:
				case SDECIMALV:
					sprintf(stemp,"%*f",field->len,*field->data.float_value);
					length=RDAstrlen(stemp);
					if(include_type) length+=RDAstrlen(field->name)+RDAstrlen(standardfieldtypes[field->type])+38;
					else length+=RDAstrlen(field->name)+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%*f]\n",field->name,standardfieldtypes[field->type],field->len,field->len,*field->data.float_value);
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%*f]\n",field->name,field->len,*field->data.float_value);
					break;
				case DOUBLEV:
				case SDOUBLEV:
					if(include_type) length=RDAstrlen(field->name)+field->len+RDAstrlen(standardfieldtypes[field->type])+38;
					else length=RDAstrlen(field->name)+field->len+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%*.0f]\n",field->name,standardfieldtypes[field->type],field->len,field->len,*field->data.float_value);
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%*.0f]\n",field->name,field->len,*field->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					if(include_type) length=RDAstrlen(field->name)+RDAstrlen(standardfieldtypes[field->type])+field->len+38;
					else length=RDAstrlen(field->name)+field->len+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%*d]\n",field->name,standardfieldtypes[field->type],field->len,field->len,*field->data.short_value);
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%*d]\n",field->name,field->len,*field->data.short_value);
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					if(include_type) length=RDAstrlen(field->name)+RDAstrlen(standardfieldtypes[field->type])+field->len+38;
					else length=RDAstrlen(field->name)+field->len+22;
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length);
					} else {
						tmp=Rmalloc(length);
					}
					if(include_type) sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\tValue: [%*d]\n",field->name,standardfieldtypes[field->type],field->len,field->len,*field->data.integer_value);
					else sprintf(&tmp[count],"\tField: %s\n\t\tValue: [%*d]\n",field->name,field->len,*field->data.integer_value);
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error unrecognized field type.");
					break;
			}
			count+=length-1;
		}
	}
	return(tmp);
}
char *maketransdesc(int selected,char include_type)
{
	char *tmp=NULL;
	RDAtrec *trec;

	if(tran->numrecs>0)
	{
		trec=tran->recs+selected;
		if(trec->type=='N')
		{
			if(tran->able) tmp=makenewdesc(trec,include_type);
		} else {
			tmp=makechangedesc(trec,include_type);
		}
	} else {
		tmp=stralloc("No Transactions Available to Describe.");
	}
	return(tmp);
}
void showdetail(RDArsrc *viewrsrc)
{
	char *modname=NULL,*trnnum=NULL,*trndesc=NULL;
	RDArsrc *showrsrc=NULL;

	FINDRSCGETSTRING(viewrsrc,"MODULE NAME",&modname);
	FINDRSCGETSTRING(viewrsrc,"TRANSACTION NUMBER",&trnnum);
	if(showrsrc!=NULL) free_rsrc(showrsrc);
	showrsrc=RDArsrcNEW(module,"VIEW TRANSACTION DETAIL");
	addstdrsrc(showrsrc,"MODULE NAME",VARIABLETEXT,0,modname,FALSE);
	addstdrsrc(showrsrc,"TRANSACTION NUMBER",VARIABLETEXT,0,trnnum,FALSE);
	trndesc=readlistmaketransdesc(viewrsrc);
	addsctrsrc(showrsrc,"TRANSACTION DESCRIPTION",0,trndesc,FALSE);
	if(trndesc!=NULL) Rfree(trndesc);
	if(tran->numrecs>0)
		addrfkwrsrc(showrsrc,"UNDO",TRUE,undopart,viewrsrc);
	addrfkwrsrc(showrsrc,"QUIT",TRUE,quitshow,NULL);
	addbtnrsrc(showrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(showrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(showrsrc,TRUE,quitshow,NULL,FALSE);
}
void undoentiretrans(RDArsrc *mainrsrc)
{
	int selected=0,tlist=0,x=0;
	char *trnname=NULL,*tmp=NULL;
	RDAtrec *trec;

	readwidget(mainrsrc,"MODULES AVAILABLE");
	readwidget(mainrsrc,"LOG LIST");
	if(FINDRSCGETINT(mainrsrc,"MODULES AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LOG LIST",&tlist)) return;
	tmp=stralloc(loglst->libs[tlist]);
	tmp[10]=0;
	trnname=Rmalloc(RDAstrlen(modlst->libs[selected])+RDAstrlen(tmp)+RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(trnname,"%s/%s/%s.TRN",CURRENTDIRECTORY,modlst->libs[selected],tmp);
#endif
#ifdef WIN32
	sprintf(trnname,"%s\\%s\\%s.TRN",CURRENTDIRECTORY,modlst->libs[selected],tmp);
#endif
	tran=GETTRANSACTION(trnname);
	tran->able=TRUE;
	ADVVERIFYTRANSACTION(tran,TRUE);
	tran->able=TRUE;
	if(trnname!=NULL) Rfree(trnname);
	for(x=tran->numrecs-1;x>(-1);--x)
	{
		trec=tran->recs+x;
		UndoRDAtrec(trec);
		trec->undone=TRUE;
	}
	FreeRDAtran(tran);
	tran=NULL;
}
void undotrans(RDArsrc *viewrsrc)
{
	int x=0;
	RDAtrec *trec;

/* I suspect this is wrong... must do Undo these transactions in reverse
   order.  Otherwise, it will not allow it to undo upon verifying against
   the initial value. */
	for(x=tran->numrecs-1;x>(-1);--x)
	{
		trec=tran->recs+x;
		UndoRDAtrec(trec);
		trec->undone=TRUE;
	}
	FreeRDAtran(tran);
	tran=NULL;
	quitview(viewrsrc);
}
char *status[2]={"","Undone"};
APPlib *maketrnlst(void)
{
	short descnum=0;
	APPlib *newapp=NULL;
	RDAtrec *trec;
	char *desc=NULL,*keystring=NULL;
	int x;

	for(x=0,trec=tran->recs;x<tran->numrecs;++x,++trec)
	{
		if(newapp==NULL) newapp=APPlibNEW();
		if(trec->type=='N') descnum=0;
			else descnum=1;
		keystring=makekeystring(trec->fileno,trec->keyno,trec->kdata);
		desc=Rmalloc(RDAstrlen(trectype[descnum])+RDAstrlen(trec->module)+RDAstrlen(trec->file)+RDAstrlen(trec->keyname)+RDAstrlen(keystring)+10+RDAstrlen(status[trec->undone]));
		sprintf(desc,"%s [%s][%s][%s] %s %s",trectype[descnum],trec->module,trec->file,trec->keyname,keystring,status[trec->undone]); 
		if(keystring!=NULL) 
		{
			Rfree(keystring);
			keystring=NULL;
		}
		addAPPlib(newapp,desc);
		if(desc!=NULL) Rfree(desc);
	}
	return(newapp);
}
void viewtrans(RDArsrc *mainrsrc)
{
	int selected=0,tlist=0,x=0;
	char *trnname=NULL,*tmp=NULL;
	char e=FALSE;
	RDArsrc *viewrsrc=NULL;

	readwidget(mainrsrc,"MODULES AVAILABLE");
	readwidget(mainrsrc,"LOG LIST");
	if(FINDRSCGETINT(mainrsrc,"MODULES AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LOG LIST",&tlist)) return;
	tmp=stralloc(loglst->libs[tlist]);
	tmp[10]=0;
	trnname=Rmalloc(RDAstrlen(modlst->libs[selected])+RDAstrlen(tmp)+RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(trnname,"%s/%s/%s.TRN",CURRENTDIRECTORY,modlst->libs[selected],tmp);
#endif
#ifdef WIN32
	sprintf(trnname,"%s\\%s\\%s.TRN",CURRENTDIRECTORY,modlst->libs[selected],tmp);
#endif
	tran=GETTRANSACTION(trnname);
	tran->able=TRUE;
	ADVVERIFYTRANSACTION(tran,TRUE);
	tran->able=TRUE;
	if(trnname!=NULL) Rfree(trnname);
	if(viewrsrc!=NULL) free_rsrc(viewrsrc);
	viewrsrc=RDArsrcNEW(module,"VIEW TRANSACTION");
	addstdrsrc(viewrsrc,"MODULE NAME",VARIABLETEXT,0,modlst->libs[selected],FALSE);
	addstdrsrc(viewrsrc,"PROCESS NAME",VARIABLETEXT,0,tran->process,FALSE);
	addstdrsrc(viewrsrc,"USER NAME",VARIABLETEXT,0,tran->user,FALSE);
	addstdrsrc(viewrsrc,"TRANSACTION NUMBER",VARIABLETEXT,0,tmp,FALSE);
	addstdrsrc(viewrsrc,"START TIME",VARIABLETEXT,0,tran->datetime,FALSE);
	addstdrsrc(viewrsrc,"END TIME",VARIABLETEXT,0,tran->endtime,FALSE);
	if(tran->able==TRUE) e=TRUE;
		else e=FALSE;
	addstdrsrc(viewrsrc,"ABLE TO UNDO",BOOLNS,0,&e,FALSE);
	if(tran->complete==TRUE) e=TRUE;
		else e=FALSE;
	addstdrsrc(viewrsrc,"COMPLETE",BOOLNS,0,&e,FALSE);
	if(tmp!=NULL) Rfree(tmp);
	if(trnlst!=NULL) freeapplib(trnlst);
	trnlst=maketrnlst();
	if(trnlst==NULL)
	{
		trnlst=APPlibNEW();
		addAPPlib(trnlst,"Contains No Transactions");
	}
	addlstrsrc(viewrsrc,"TRANSACTION LIST",&x,TRUE,NULL,
		trnlst->numlibs,&trnlst->libs,NULL);
	addbtnrsrc(viewrsrc,"DETAIL",TRUE,showdetail,NULL);
	if(tran->able && tran->numrecs>0)
	{
		addrfkwrsrc(viewrsrc,"UNDO",TRUE,undotrans,NULL);
	}
	addrfkwrsrc(viewrsrc,"QUIT",TRUE,quitview,NULL);
	addbtnrsrc(viewrsrc,"HELP",TRUE,screenhelp,NULL);
       	addrfcbrsrc(viewrsrc,"PRINT TRANSACTION",TRUE,printtranscb,NULL);
	addbtnrsrc(viewrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(viewrsrc,TRUE,quitview,NULL,FALSE);
	if(!tran->able)
	{
		ERRORDIALOG("Unable To Undo","This Transaction Log contains elements that may not be undone.  Newer transactions exist that must be undone before this transaction can be.",NULL,FALSE);
	}
}
void deletetrans(RDArsrc *mainrsrc)
{
	int selected=0,tlist=0;
	char *trnname=NULL,*tmp=NULL,errordesc[1024];

	readwidget(mainrsrc,"MODULES AVAILABLE");
	readwidget(mainrsrc,"LOG LIST");
	if(FINDRSCGETINT(mainrsrc,"MODULES AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LOG LIST",&tlist)) return;
	tmp=stralloc(loglst->libs[tlist]);
	tmp[10]=0;
	trnname=Rmalloc(RDAstrlen(modlst->libs[selected])+RDAstrlen(tmp)+RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(trnname,"%s/%s/%s.TRN",CURRENTDIRECTORY,modlst->libs[selected],tmp);
#endif
#ifdef WIN32
	sprintf(trnname,"%s\\%s\\%s.TRN",CURRENTDIRECTORY,modlst->libs[selected],tmp);
#endif
	if(unlink(trnname)==(-1)) 
	{
		if(errno!=ENOENT) 
		{ 
			sprintf(errordesc,"Can't remove transaction file [%s] %s.",trnname,errname());
			ERRORDIALOG("CAN'T REMOVE FILE",errordesc,NULL,FALSE);
			prterr("%s",errordesc); 
		}
	}
	if(trnname!=NULL) Rfree(trnname);
	if(tmp!=NULL) Rfree(tmp);
	gettransentries(mainrsrc);
}
void quittrans(RDArsrc *mainrsrc)
{
	if(modlst!=NULL) freeapplib(modlst);
	if(loglst!=NULL) freeapplib(loglst);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	ShutdownSubsystems();
}
void getdirectorytrans(RDArsrc *mainrsrc)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6); 
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Entries");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(modlst);
}
char *gettrandesc(char *dirx,char *name)
{
	FILE *fp=NULL;
	char *tmp=NULL,*tname=NULL,*filename=NULL,*odesc=NULL,*test=NULL;
	char *sdate=NULL,*process=NULL,*user=NULL,*edate=NULL;
	short exit=FALSE;
	long pos=0,curpos=0;
#ifdef _POSIX_SOURCE
#ifdef WIN32
	long offset2=-35;
	long offset=-37;
#endif 
#ifndef WIN32
	long offset2=-34;
	long offset=-36;
#endif
#else
/*
	long offset=-31;
*/
#ifdef WIN32
	long offset2=-35;
	long offset=-37;
#endif
#ifndef WIN32
	long offset2=-34;
	long offset=-36;
#endif
#endif
	filename=Rmalloc(RDAstrlen(dirx)+RDAstrlen(name)+1);
	sprintf(filename,"%s%s",dirx,name);
	fp=fopen(filename,"r");
	if(fp!=NULL)
	{
		sdate=READFIELD(fp,&exit);
		process=READFIELD(fp,&exit);
		user=READFIELD(fp,&exit);
		pos=ftell(fp);
		if(!fseek(fp,offset2,SEEK_END) && !exit)
		{
			curpos=ftell(fp);
			if(curpos>=pos)
			{
				test=READFIELD(fp,&exit);
				if(!exit && !RDAstrcmp(test,"END TRANSACTION"))
				{
					edate=READFIELD(fp,&exit);
					if(curpos>(pos))
					{
						odesc=stralloc("Has Transactions");
					} else {
						odesc=stralloc("Log Only");
					}
				} else if(!fseek(fp,offset,SEEK_END) && !exit)
				{
					curpos=ftell(fp);
					if(curpos>=pos)
					{
						test=READFIELD(fp,&exit);
						if(!exit && !RDAstrcmp(test,"END TRANSACTION"))
						{
							edate=READFIELD(fp,&exit);
							if(curpos>(pos))
							{
								odesc=stralloc("Has Transactions");
							} else {
								odesc=stralloc("Log Only");
							}
						} else {
							edate=stralloc("Uncomplete Transaction");
							odesc=stralloc(" ");
						}
					} else {
						edate=stralloc("Uncomplete Transaction");
						odesc=stralloc(" ");
					}
				} else {
					edate=stralloc("Uncomplete Transaction");
					odesc=stralloc(" ");
				}
			} else {
				edate=stralloc("Uncomplete Transaction");
				odesc=stralloc(" ");
			}
		} else {
			edate=stralloc("Uncomplete Transaction");
			odesc=stralloc(" ");
		}
		tname=stralloc(name);
		tname[RDAstrlen(tname)-4]=0;
		tmp=Rmalloc((tname!=NULL?RDAstrlen(tname):1)+(user!=NULL?RDAstrlen(user):1)+(process!=NULL?RDAstrlen(process):1)+(sdate!=NULL?RDAstrlen(sdate):1)+(edate!=NULL?RDAstrlen(edate):1)+(odesc!=NULL?RDAstrlen(odesc):1)+6);
		sprintf(tmp,"%s %s %s %s %s %s",(tname!=NULL?tname:" "),
			(user!=NULL?user:" "),(process!=NULL?process:" "),
			(sdate!=NULL?sdate:" "),(edate!=NULL?edate:"?"),
			(odesc!=NULL?odesc:" "));
		if(tname!=NULL) Rfree(tname);
		fclose(fp);
		if(sdate!=NULL) Rfree(sdate);
		if(process!=NULL) Rfree(process);
		if(user!=NULL) Rfree(user);
		if(odesc!=NULL) Rfree(odesc);
		if(edate!=NULL) Rfree(edate);
		if(test!=NULL) Rfree(test);
	}
	if(filename!=NULL) Rfree(filename);
	return(tmp);
}

/*  This is the old function before the 4-digit year transactions */
/*
char *gettrandesc(char *dirx,char *name)
{
	FILE *fp=NULL;
	char *tmp=NULL,*tname=NULL,*filename=NULL,*odesc=NULL,*test=NULL;
	char *sdate=NULL,*process=NULL,*user=NULL,*edate=NULL;
	short exit=FALSE;
	long pos=0,curpos=0;
#ifdef _POSIX_SOURCE
	long offset=-34;
#else
	long offset=-34;
#endif

	filename=Rmalloc(RDAstrlen(dirx)+RDAstrlen(name)+1);
	sprintf(filename,"%s%s",dirx,name);
	fp=fopen(filename,"r");
	if(fp!=NULL)
	{
		sdate=READFIELD(fp,&exit);
		process=READFIELD(fp,&exit);
		user=READFIELD(fp,&exit);
		pos=ftell(fp);
		if(!fseek(fp,offset,SEEK_END) && !exit)
		{
			curpos=ftell(fp);
			if(curpos>=pos)
			{
				test=READFIELD(fp,&exit);
				if(!exit && !RDAstrcmp(test,"END TRANSACTION"))
				{
					edate=READFIELD(fp,&exit);
					if(curpos>(pos)) 
					{
						odesc=stralloc("Has Transactions");
					} else odesc=stralloc("Log Only");
				} else {
					edate=stralloc("Uncomplete Transaction");
					odesc=stralloc(" ");
				}
			} else {
				edate=stralloc("Uncomplete Transaction");
				odesc=stralloc(" ");
			}
		} else {
			edate=stralloc("Uncomplete Transaction");
			odesc=stralloc(" ");
		}
		tname=stralloc(name);
		tname[RDAstrlen(tname)-4]=0;
		tmp=Rmalloc(RDAstrlen(tname)+RDAstrlen(process)+RDAstrlen(user)+RDAstrlen(sdate)+RDAstrlen(edate)+RDAstrlen(odesc)+6);
		sprintf(tmp,"%s %s %s %s %s %s",tname,(user!=NULL ? user:""),(process!=NULL ? process:""),(sdate!=NULL ?sdate:""),(edate!=NULL ? edate:"?"),(odesc!=NULL ? odesc:"")); 
		if(tname!=NULL) Rfree(tname);
		fclose(fp);
		if(sdate!=NULL) Rfree(sdate);
		if(process!=NULL) Rfree(process);
		if(user!=NULL) Rfree(user);
		if(odesc!=NULL) Rfree(odesc);
		if(edate!=NULL) Rfree(edate);
		if(test!=NULL) Rfree(test);
	}
	if(filename!=NULL) Rfree(filename);
	return(tmp);
}
*/
APPlib *gettranslist(char *module)
{
	APPlib *newapp=NULL;
	int x;
	char *tmp=NULL,*desc=NULL;
	char *dirx=NULL;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,module);
#endif
	for(x=0;findfile(dirx,"*.TRN",&tmp,(int)x+1);++x) 
	{
		if(RDAstrcmp(tmp,"NXTTRN.TRN"))
		{
			desc=gettrandesc(dirx,tmp);
			if(desc!=NULL)
			{
				if(newapp==NULL) newapp=APPlibNEW();
				addAPPlib(newapp,desc);
				Rfree(desc);
				desc=NULL;
			}
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	if(newapp!=NULL) SORTAPPlib(newapp);
	return(newapp);
}
void gettransentries(RDArsrc *mainrsrc)
{
	int selected;

	if(FINDRSCGETINT(mainrsrc,"MODULES AVAILABLE",&selected)) return;
	if(loglst!=NULL) freeapplib(loglst);
	loglst=gettranslist(modlst->libs[selected]); 
	if(loglst==NULL)
	{
		loglst=APPlibNEW();
		addAPPlib(loglst,"Contains No Transactions");
	}
	SORTAPPlib(loglst);
	if(!FINDRSCLISTAPPlib(mainrsrc,"LOG LIST",loglst->numlibs-1,loglst))
		updatersrc(mainrsrc,"LOG LIST");
}
static void printtrans(RDArsrc *prtrsrc,RDArsrc *viewrsrc)
{
	FILE *fp=NULL;
	char *header=NULL,*modname=NULL,*trnnum=NULL,*process=NULL,*user=NULL;
	char *outputdevice=NULL,*prtmessage=NULL;
	int pages=0,lines=0;
	char AbleToUndo=FALSE,complete=FALSE;
	unsigned line_length=79;

	readwidget(prtrsrc,"DEVICE");
	if(FINDRSCGETSTRING(prtrsrc,"DEVICE",&outputdevice)) return;
	if(outputdevice!=NULL)
	{
		if((fp=popen(outputdevice,"w"))!=NULL)
		{
			readallwidgets(viewrsrc);
			FINDRSCGETSTRING(viewrsrc,"MODULE NAME",&modname);
			FINDRSCGETSTRING(viewrsrc,"TRANSACTION NUMBER",&trnnum);
			FINDRSCGETSTRING(viewrsrc,"PROCESS NAME",&process);
			FINDRSCGETSTRING(viewrsrc,"USER NAME",&user);
			FINDRSCGETCHAR(viewrsrc,"ABLE TO UNDO",&AbleToUndo);
			FINDRSCGETCHAR(viewrsrc,"COMPLETE",&complete);
			header=stralloc("XPERT TRANSACTION PROCESSING");
			printheader(fp,&lines,&pages,header,line_length,modname,trnnum,process,user);
			if(header!=NULL) Rfree(header);
			fprintf(fp,"Status: Able To Undo = [%s]  Complete = [%s]\n",(AbleToUndo?"TRUE":"FALSE"),(complete?"TRUE":"FALSE")); 
			++lines;
			if(RDAstrcmp(trnlst->libs[0],"Contains No Transactions"))
			{
				fprintf(fp,"Transactions are as follows:\n\n");
				lines+=2;
				printdetail(fp,&lines,&pages,line_length,modname,trnnum,process,user);
			}
			if(modname!=NULL) Rfree(modname);
			if(trnnum!=NULL) Rfree(trnnum);
			if(process!=NULL) Rfree(process);
			if(user!=NULL) Rfree(user);
			fprintf(fp,"\n");
			pclose(fp);
		} else {
			prtmessage=Rmalloc(RDAstrlen(outputdevice)+67);
			sprintf(prtmessage,"The output device [%s] does not exist or cannot otherwise be opened!",outputdevice);
			ERRORDIALOG("Output Device Failure",prtmessage,NULL,FALSE);
			if(prtmessage!=NULL) Rfree(prtmessage);
			quitrsrc(prtrsrc);
			return;
		}
		if(outputdevice!=NULL) Rfree(outputdevice);
		if(prtmessage!=NULL) Rfree(prtmessage);
	} else {
		if(outputdevice==NULL)
			ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
	quitrsrc(prtrsrc);
}
static void quitrsrc(RDArsrc *rsrc)
{
	if(rsrc!=NULL) free_rsrc(rsrc);
} 
void printtranscb(RDArsrc *viewrsrc)
{
	char *devicex=NULL;
	RDArsrc *prtrsrc=NULL;

	devicex=DefaultPrinter();
	prtrsrc=RDArsrcNEW(module,"PRINT SCREEN");
	addstdrsrc(prtrsrc,"DEVICE",VARIABLETEXT,0,devicex,TRUE);
	if(devicex!=NULL) Rfree(devicex);
	DefaultScreens(prtrsrc);
	addrfkwrsrc(prtrsrc,"PRINT",TRUE,printtrans,viewrsrc);
	addbtnrsrc(prtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfkwrsrc(prtrsrc,"QUIT",TRUE,quitrsrc,NULL);
	addbtnrsrc(prtrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	APPmakescrn(prtrsrc,TRUE,quitrsrc,NULL,FALSE);
}
static void printheader(FILE *fp,int *lines,int *pages,char *header,
	unsigned line_length,char *modname,char *trnnum,char *process,char *user)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	fprintf(fp,"\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(25+RDAstrlen(modname)+RDAstrlen(trnnum)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(25+RDAstrlen(modname)+RDAstrlen(trnnum)))/2)-14)));
	if(tail_ws<0) tail_ws=0;
	fprintf(fp,"%*s Module: %s  Transaction: %s %*sDate: %s\n",
		lead_ws,"",modname,trnnum,tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(19+RDAstrlen(process)+RDAstrlen(user)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(19+RDAstrlen(process)+RDAstrlen(user)))/2)-14)));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(process)+RDAstrlen(user)+14+19+lead_ws+tail_ws)>(line_length))
	{
		fprintf(fp,"%*sProcess: %s %*s\n",
			(int)(round(((double)(line_length-(10+RDAstrlen(process)))/2))),"",
			process,
			(int)(Rint((((double)(line_length-(10+RDAstrlen(process)))/2)-14))),"");
		fprintf(fp,"%*sUser: %s %*s\n",
			(int)(round(((double)(line_length-(7+RDAstrlen(user)))/2))),"",
			user,
			(int)(Rint((((double)(line_length-(7+RDAstrlen(user)))/2)-14))),"");
		*lines+=2;
	} else {
		fprintf(fp,"%*s Process: %s  User: %s %*s\n",
			lead_ws,"",process,user,tail_ws,"");
		++*lines;
	}
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+14+15+lead_ws+tail_ws)>line_length)
	{
		fprintf(fp,"%*s %s %*sTime: %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
			timex);
		++*lines;
	} else {
		fprintf(fp,"%*s Executed By: %s %*sTime: %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	fprintf(fp,"\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"VIEW TRANSACTION")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"VIEW LOG ENTRIES");
	getdirectorytrans(mainrsrc);
	if(argc>1)
	{
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(argv[1],modlst->libs[x])) break;
		}
		if(x>=modlst->numlibs) x=0;
	}
	addlstrsrc(mainrsrc,"MODULES AVAILABLE",&x,TRUE,gettransentries,
		modlst->numlibs,&modlst->libs,NULL);
	if(loglst!=NULL) freeapplib(loglst);
	loglst=APPlibNEW();
	x=0;
	addlstrsrc(mainrsrc,"LOG LIST",&x,TRUE,NULL,loglst->numlibs,
		&loglst->libs,NULL);
	gettransentries(mainrsrc);
	addbtnrsrc(mainrsrc,"VIEW",TRUE,viewtrans,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quittrans,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletetrans,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"UNDO",TRUE,undoentiretrans,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quittrans,NULL,TRUE);
}
