/* viewtran.c - View Transactions and Undo */
#ifndef WIN32
#define __NAM__ "viewtran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "viewtran.exe"
#endif
#include <app.hpp>


#include <fcntl.h>

#include <mix.hpp>
#ifdef UNIXWARE7
#include <cerrno>
#endif
#ifdef UNIXWARE21
#include <cerrno>
#endif
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

static APPlib *modlst=NULL,*loglst=NULL,*trnlst=NULL;
static char *module="DATABASE";
static RDAtran *tran=NULL;
static void printtrans(RDArsrc *,RDArsrc *);
static void printtranscb(RDArsrc *);
static void printheader(RDA_PFILE *,int *,int *,char *,unsigned,char *,char *,char *,char *);
static void printdetail(RDA_PFILE *,int *,int *,unsigned,char *,char *,char *,char *);
static char *readlistmaketransdesc(RDArsrc *),*maketransdesc(int,char);
static char *makechangedesc(RDAtrec *,char),*makenewdesc(RDAtrec *,char);
static void quitrsrc(RDArsrc *);
static void deletetranstest(RDArsrc *);
void deletetrans(RDArsrc *);
void viewtrans(RDArsrc *,RDArsrc *,char *,char *,char,char);

void gettransentries(RDArsrc *);
APPlib *maketrnlst(void);
char *status[2]={"","Undone"};
char *devlicense=NULL;
short allow_undo=FALSE,dev_license=FALSE;
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







#define RDAtranNEW(x,y)	xRDAtranNEW(x,y,__LINE__,__FILE__)
/* RDAtranNEW allocates memory for a new RDAtran Structure */
RDAtran *xRDAtranNEW(char *,char *,int,char *);
#define FreeRDAtran(x)	{xFreeRDAtran(x,__LINE__,__FILE__);x=NULL;}
/* FreeRDAtran free's the memory allocated in an RDAtran Structure */
void xFreeRDAtran(RDAtran *,int,char *);
#define CLOSEtranFiles(x)	xCLOSEtranFiles(x,__LINE__,__FILE__)
/* CLOSEtranFILES - Closes all datafiles opened in a specific RDAtran 
	Structure */
void xCLOSEtranFiles(RDAtran *,int,char *);
#define GETTRANSACTION(x)	xGETTRANSACTION(x,__LINE__,__FILE__)
/* GETTRANSACTION reads the specified transaction processing text file and 
	returns an RDAtran Structure (RDAtran *) reflecting it's contents */
RDAtran *xGETTRANSACTION(char *,int,char *);
char *makekeystring(short,short,RDAtkey *);

#define RDATDataFIELD(a,b)	xRDATDataFIELD(a,b,__LINE__,__FILE__)
NRDfield *xRDATDataFIELD(RDATData *,char *,int,char *);
#define FINDRDATDataSETSHORT(a,b,c)	xFINDRDATDataSETSHORT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETSHORT(RDATData *,char *,short,int,char *);
#define FINDRDATDataSETCHAR(a,b,c)	xFINDRDATDataSETCHAR(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETCHAR(RDATData *,char *,char,int,char *);
#define FINDRDATDataSETINT(a,b,c)	xFINDRDATDataSETINT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETINT(RDATData *,char *,int,int,char *);
#define FINDRDATDataSETDOUBLE(a,b,c)	xFINDRDATDataSETDOUBLE(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETDOUBLE(RDATData *,char *,double,int,char *);
#define FINDRDATDataSETSTRING(a,b,c)	xFINDRDATDataSETSTRING(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETSTRING(RDATData *,char *,char *,int,char *);
#define FINDRDATDataGETCHAR(a,b,c)	xFINDRDATDataGETCHAR(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETCHAR(RDATData *,char *,char *,int,char *);
#define FINDRDATDataGETSHORT(a,b,c)	xFINDRDATDataGETSHORT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETSHORT(RDATData *,char *,short *,int,char *);
#define FINDRDATDataGETINT(a,b,c)	xFINDRDATDataGETINT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETINT(RDATData *,char *,int *,int,char *);
#define FINDRDATDataGETDOUBLE(a,b,c)	xFINDRDATDataGETDOUBLE(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETDOUBLE(RDATData *,char *,double *,int,char *);
#define FINDRDATDataGETSTRING(a,b,c)	xFINDRDATDataGETSTRING(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETSTRING(RDATData *,char *,char **,int,char *);
#define RDATData2File(a,b)	xRDATData2File(a,b,__LINE__,__FILE__)
void xRDATData2File(short,RDATData *,int,char *);











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
NRDfield *xRDATDataFIELD(RDATData *previous,char *name,int line,char *file)
{
	NRDfield *tmp=NULL;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG RDATDataFIELD at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(previous!=NULL)
	{
		if(previous->field!=NULL)
		{
			for(x=0,tmp=previous->field;x<previous->num;++x,++tmp)
			{
				if(!RDAstrcmp(tmp->name,name)) return(tmp);
			}
			tmp=NULL;
		}
	}
	return(tmp);
}
short xFINDRDATDataGETCHAR(RDATData *previous,char *fieldname,char *value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETCHAR Get Character Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*(char *)value=*(char *)field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value>-127 &&
					*field->data.float_value<128)
				{
					*(char *)value=*(char *)field->data.float_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				if(*field->data.short_value>-127 &&
					*field->data.short_value<128)
				{
					*(char *)value=*(char *)field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataGETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(*field->data.integer_value>-127 &&
					*field->data.integer_value<128)
				{
					*(char *)value=*(char *)field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataGETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETCHAR Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETSHORT(RDATData *previous,char *fieldname,short *value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETSHORT Get Short Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value<SHRT_MAX)
				{
/*lint -e524*/
					*value=*field->data.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(*field->data.integer_value<SHRT_MAX)
				{
/*lint -e524*/
					*value=*field->data.integer_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataGETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETSHORT Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETINT(RDATData *previous,char *fieldname,int *value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETINT Get Integer Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value<INT_MAX)
				{
/*lint -e524*/
					*value=*field->data.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETINT at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*value=*field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETINT Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETDOUBLE(RDATData *previous,char *fieldname,double *value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETDOUBLE Get Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*value=*field->data.float_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*value=*field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETDOUBLE Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETSTRING(RDATData *previous,char *fieldname,char **value,int line,char *file)
{
	NRDfield *field;
	short h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETSTRING Getting string value of [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				if(field->data.string_value!=NULL)
				{
					h=RDAstrlen(*value);
					if(field->dlen>h || *value==NULL)
					{
						*value=Rrealloc(*value,field->dlen+1);
					}
					memcpy(*value,field->data.string_value,field->dlen+1);
				} else {
					if(*value!=NULL) Rfree(*value);
					*value=NULL;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETSTRING Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SHORTV:
			case SSHORTV:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETSHORT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETINT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETCHAR.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETSTRING Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETSTRING(RDATData *previous,char *fieldname,char *value,
	int line,char *file)
{
	NRDfield *field;
	unsigned h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETSTRING Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				if(!isEMPTY((char *)value))
				{
					
					h=RDAstrlen((char *)value);
					QUICKALLOC(field->data.string_value,field->dlen,h+1);
					memcpy(field->data.string_value,(char *)value,h+1);
					if(field->dlen>field->len && field->len>0)
					{
						field->data.string_value[field->len]=0;
					}
				} else if(field->dlen>0)
				{
					memset(field->data.string_value,0,field->dlen);
				} else {
					field->data.string_value=NULL;
					field->dlen=0;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSTRING Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case BOOLNS:
			case CHARACTERS:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETDOUBLE.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SHORTV:
			case SSHORTV:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETSHORT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case LONGV:
			case SLONGV:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETINT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETSTRING Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETCHAR(RDATData *previous,char *fieldname,char value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETCHAR Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error the FINDRDATDataSETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
				}
				*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETCHAR Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETSHORT(RDATData *previous,char *fieldname,short value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETSHORT Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error the FINDRDATDataSETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(value<128 && value>-127)
				{
					*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETSHORT Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETINT(RDATData *previous,char *fieldname,int value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETINT Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error the FINDRDATDataSETINT at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
					*field->data.string_value=0;
				}
				if(value>-127 && value<128)
				{
					*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETINT at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				if(value<SHRT_MAX)
				{
					*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETINT at line [%d] program [%s] should be replaced with FINDRDATDataSETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETINT Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETDOUBLE(RDATData *previous,char *fieldname,double value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETDOUBLE Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
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
				prterr("Error the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
					*field->data.string_value=0;
				}
				if(value>-127 && value<128)
				{
/*lint -e524*/
					*field->data.string_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				if(value<SHRT_MAX)
				{
/*lint -e524*/
					*field->data.short_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case LONGV:
			case SLONGV:
				if(value<INT_MAX)
				{
/*lint -e524*/
					*field->data.integer_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETDOUBLE Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
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
#define RedoRDAtrec(a)	xRedoRDAtrec(a,__LINE__,__FILE__)
short xRedoRDAtrec(RDAtrec *trec,int line,char *file)
{
	RDAtfld *tfld;
	short af=0;
	int x;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	RDAtkey *tmp;
	char *keydata=NULL,deletef=FALSE;

	if(diagmix || diagmix_field || diagtrans)
	{
		keydata=makekeystring(trec->fileno,trec->keyno,trec->kdata);
		prterr("DIAG RedoRDAtrec Redo an RDA Transaction Record for Module [%s] File [%d] [%s] Key [%d] [%s] Key Values [%s] at line [%d] program [%s].",trec->module,trec->fileno,FILENAME(trec->fileno),trec->keyno,KEYNAME(trec->fileno,trec->keyno),
			(keydata!=NULL ? keydata:""),line,file);
		if(keydata!=NULL) Rfree(keydata);
	}
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
		if(EQLNRDsec(trec->fileno,trec->keyno))
			KEYNRD(trec->fileno,trec->keyno);
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
							if(!isEMPTY(tfld->to.string_value))
							{
								af=RDAstrlen(tfld->to.string_value);
								field->data.string_value=Rrealloc(field->data.string_value,af+1);
								field->dlen=af+1;
								memcpy(field->data.string_value,tfld->to.string_value,af+1);
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
								*tfld->to.string_value;
							break;
						case DOUBLEV:
						case SDOUBLEV:
						case DECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							*field->data.float_value=
								*tfld->to.float_value;
							break;
						case SHORTV:
						case SSHORTV:
							*field->data.short_value=
								*tfld->to.short_value;
							break;
						case LONGV:
						case SLONGV:
							*field->data.integer_value=
								*tfld->to.integer_value;
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
		prterr("Error Key [%d] not Found in Module [%s] File [%d] [%s] at line [%d] program [%s].",trec->keyno,MODULENAME(trec->fileno),trec->fileno,FILENAME(trec->fileno),line,file);
		return(-1);
	}
}
void redopart(RDArsrc *showrsrc,RDArsrc *viewrsrc)
{
	int selected=0;
	RDAtrec *trec=NULL;

	readwidget(viewrsrc,"TRANSACTION LIST");
	if(FINDRSCGETINT(viewrsrc,"TRANSACTION LIST",&selected)) return;
	if(tran->numrecs>0)
	{
		trec=tran->recs+selected;
		RedoRDAtrec(trec);
		if(trnlst!=NULL) freeapplib(trnlst);
		trnlst=maketrnlst();
		if(trnlst==NULL)
		{
			trnlst=APPlibNEW();
			addAPPlib(trnlst,"Contains No Transactions");
		}
		if(!FINDRSCLISTAPPlib(viewrsrc,"TRANSACTION LIST",0,trnlst)) updatersrc(viewrsrc,"TRANSACTION LIST");
	}
	quitshow(showrsrc);
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
		UndoRDAtrec(trec);
		if(trnlst!=NULL) freeapplib(trnlst);
		trnlst=maketrnlst();
		if(trnlst==NULL)
		{
			trnlst=APPlibNEW();
			addAPPlib(trnlst,"Contains No Transactions");
		}
		if(!FINDRSCLISTAPPlib(viewrsrc,"TRANSACTION LIST",0,trnlst)) updatersrc(viewrsrc,"TRANSACTION LIST");
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
void quitview_exit(RDArsrc *viewrsrc)
{
	quitview(viewrsrc);
	ShutdownSubsystems();
	if(devlicense!=NULL) Rfree(devlicense);
	std::exit;
}
void fixtrnoutput(RDA_PFILE *fp,char *data,int *lines,int *pages,unsigned line_length,
		char *modname,char *trnnum,char *process,char *user)
{
	unsigned y=0,x=0,length=0;
	char *temp=NULL,*temp1=NULL;

	if(!isEMPTY(data))
	{
		if((*lines+1)>58)
		{
			RDA_fprintf(fp,"\f");
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
			RDA_fprintf(fp,"%.*s\n",x+1,data);
			fixtrnoutput(fp,data+x+1,lines,pages,line_length,modname,trnnum,process,user);
		} else {
			RDA_fprintf(fp,"%.*s\n",length,data);
			if(temp!=NULL || temp1!=NULL) fixtrnoutput(fp,data+length+1,lines,pages,line_length,modname,trnnum,process,user);
		}
	} 
}
static void printdetail(RDA_PFILE *fp,int *lines,int *pages,unsigned line_length,
			char *modname,char *trnnum,char *process,char *user)
{
	int x;
	char *header=NULL,*transdesc=NULL;

	for(x=0;x<trnlst->numlibs;++x)
	{
		if(*lines+1>58)
		{
			RDA_fprintf(fp,"\f");
			header=stralloc("XPERT TRANSACTION PROCESSING");
			printheader(fp,lines,pages,header,line_length,modname,trnnum,process,user);
			if(header!=NULL) Rfree(header);
		}
		RDA_fprintf(fp,"%s\n",trnlst->libs[x]);
		++*lines;
		transdesc=maketransdesc(x,FALSE);
		fixtrnoutput(fp,transdesc,lines,pages,line_length,modname,trnnum,process,user);
		if(transdesc!=NULL) Rfree(transdesc);
		RDA_fprintf(fp,"\n");
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
			case DOLLARS_NOCENTS:
				temp1=famtncents(((*tfld->from.float_value)*100),tfld->len,' ');
				temp2=famtncents(((*tfld->to.float_value)*100),tfld->len,' ');
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
				case DOLLARS_NOCENTS:
					temp1=famtncents(((*field->data.float_value)*100),field->len,' ');
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
	addrfkwrsrc(showrsrc,"UNDO",TRUE,undopart,viewrsrc);
	if(tran->able && tran->numrecs>0)
	{
		FINDRSCSETEDITABLE(showrsrc,"UNDO",TRUE);
		FINDRSCSETSENSITIVE(showrsrc,"UNDO",TRUE);
	} else {
		FINDRSCSETEDITABLE(showrsrc,"UNDO",FALSE);
		FINDRSCSETSENSITIVE(showrsrc,"UNDO",FALSE);
	}
	addrfkwrsrc(showrsrc,"REDO",TRUE,redopart,viewrsrc);
	if(dev_license==FALSE || tran->numrecs<1)
	{
		FINDRSCSETEDITABLE(showrsrc,"REDO",FALSE);
		FINDRSCSETSENSITIVE(showrsrc,"REDO",FALSE);
	}
	addrfkwrsrc(showrsrc,"QUIT",TRUE,quitshow,NULL);
	addbtnrsrc(showrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(showrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(showrsrc,TRUE,quitshow,NULL,FALSE);
}
void redotrans(RDArsrc *viewrsrc)
{
	int x=0;
	RDAtrec *trec;

/* Must be done in original order */
	for(x=0;x<tran->numrecs;++x)
	{
		trec=tran->recs+x;
		RedoRDAtrec(trec);
	}
	FreeRDAtran(tran);
	tran=NULL;
}
void redotrans_noexit(RDArsrc *viewrsrc)
{
	redotrans(viewrsrc);
	quitview(viewrsrc);
}
void redotrans_exit(RDArsrc *viewrsrc)
{
	redotrans(viewrsrc);
	quitview_exit(viewrsrc);
}
void undotrans(RDArsrc *viewrsrc)
{
	int x=0;
	RDAtrec *trec;

/* Must be done in reverse order */
	for(x=tran->numrecs-1;x>(-1);--x)
	{
		trec=tran->recs+x;
		UndoRDAtrec(trec);
		trec->undone=TRUE;
	}
	FreeRDAtran(tran);
	tran=NULL;
}
void undotrans_noexit(RDArsrc *viewrsrc)
{
	undotrans(viewrsrc);
	quitview(viewrsrc);
}
void undotrans_exit(RDArsrc *viewrsrc)
{
	undotrans(viewrsrc);
	quitview_exit(viewrsrc);
}
APPlib *maketrnlst(void)
{
	short descnum=0;
	APPlib *newapp=NULL;
	RDAtrec *trec=NULL;
	char *desc=NULL,*keystring=NULL;
	int x;

	for(x=0,trec=tran->recs;x<tran->numrecs;++x,++trec)
	{
		if(newapp==NULL) newapp=APPlibNEW();
		if(trec->type=='N')
		{
			descnum=0;
		} else {
			descnum=1;
		}
		keystring=makekeystring(trec->fileno,trec->keyno,trec->kdata);
		desc=Rmalloc(RDAstrlen(trectype[descnum])+RDAstrlen(trec->module)+RDAstrlen(trec->file)+RDAstrlen(trec->keyname)+RDAstrlen(keystring)+10+RDAstrlen(status[trec->undone]));
		sprintf(desc,"%s [%s][%s][%s] %s %s",trectype[descnum],trec->module,trec->file,trec->keyname,(!isEMPTY(keystring)?keystring:""),(!isEMPTY(status[trec->undone])?status[trec->undone]:"")); 
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
void showprev(RDArsrc *viewrsrc,char *modulename,char *tranno)
{
}
void shownext(RDArsrc *viewrsrc,char *modulename,char *tranno)
{
	int x=0;
	short y=0;
	char *trnname=NULL;
	char e=FALSE;
	char allow_force=FALSE;

	trnname=Rmalloc(RDAstrlen(modulename)+RDAstrlen(tranno)+RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(trnname,"%s/%s/%s.TRN",CURRENTDIRECTORY,modulename,tranno);
#endif
#ifdef WIN32
	sprintf(trnname,"%s\\%s\\%s.TRN",CURRENTDIRECTORY,modulename,tranno);
#endif
	if(tran!=NULL) FreeRDAtran(tran);
	tran=NULL;
	tran=GETTRANSACTION(trnname);
	if(tran!=NULL)
	{
		if(allow_undo==TRUE)
		{
			y=ADVVERIFYTRANSACTION(tran,TRUE);
		} else {
			y=ADVVERIFYTRANSACTION(tran,FALSE);
		}
		if(y!=(-1))
		{
			if(allow_undo==TRUE && tran->able==FALSE)
			{
				allow_force=TRUE;
				tran->able=TRUE;
			}
			FINDRSCSETSTRING(viewrsrc,"MODULE NAME",modulename);
			FINDRSCSETSTRING(viewrsrc,"PROCESS NAME",tran->process);
			if(trnname!=NULL) Rfree(trnname);
			FINDRSCSETSTRING(viewrsrc,"USER NAME",tran->user);
			FINDRSCSETSTRING(viewrsrc,"TRANSACTION NUMBER",tranno);
			FINDRSCSETSTRING(viewrsrc,"START TIME",tran->datetime);
			FINDRSCSETSTRING(viewrsrc,"END TIME",tran->endtime);
			if(tran->able==TRUE) e=TRUE; else e=FALSE;
			FINDRSCSETCHAR(viewrsrc,"ABLE TO UNDO",e);
			if(tran->complete==TRUE) e=TRUE; else e=FALSE;
			FINDRSCSETCHAR(viewrsrc,"COMPLETE",e);
			if(trnlst!=NULL) freeapplib(trnlst);
			trnlst=maketrnlst();
			if(trnlst==NULL)
			{
				trnlst=APPlibNEW();
				addAPPlib(trnlst,"Contains No Transactions");
			}
			x=0;
			FINDRSCLISTAPPlib(viewrsrc,"TRANSACTION LIST",x,trnlst);
			if(tran->able && tran->numrecs>0)
			{
				FINDRSCSETEDITABLE(viewrsrc,"UNDO",TRUE);
				FINDRSCSETSENSITIVE(viewrsrc,"UNDO",TRUE);
			} else {
				FINDRSCSETSENSITIVE(viewrsrc,"UNDO",FALSE);
				FINDRSCSETEDITABLE(viewrsrc,"UNDO",FALSE);
			}
			updateallrsrc(viewrsrc);
			if(!tran->able)
			{
				prterr("Error: UNABLE TO UNDO, this Transaction Log contains elements that may not be undone.  Newer transactions exist that must be undone before this transaction can be.");
				ERRORDIALOG("UNABLE TO UNDO","This Transaction Log contains elements that may not be undone.  Newer transactions exist that must be undone before this transaction can be.",NULL,FALSE);
			} else if(allow_force==TRUE)
			{
				prterr("Warning: DEVELOPMENT LICENSE UNDO, this Transaction Log contains elements that indicate that it shouldn't be undone.  Newer transactions exist that must be undone before this transaction can be.");
				WARNINGDIALOG("WARNING DIALOG SCREEN","DEVELOPMENT LICENSE UNDO","This Transaction Log contains elements that indicate that it should not be undone.  Newer transactions exist that must be undone before this transaction can be.",NULL,NULL,FALSE,viewrsrc);
			}
		} else {
			prterr("Error: VERIFY TRANSACTION FAILED, when verifying your access to this transactions, it was found that either you lacked the security access too view this transaction OR records in this transaction no longer exist.");
			ERRORDIALOG("VERIFY TRANSACTION FAILED","When verifying your access to this transactions, it was found that either you lacked the security access too view this transaction OR records in this transaction no longer exist.",NULL,FALSE);
		}
	} else {
		prterr("Error: TRANSACTION NOT FOUND, when attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.");
		ERRORDIALOG("TRANSACTION NOT FOUND","When attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.",NULL,FALSE);
	}
}
void loadtrans(RDArsrc *viewrsrc)
{
	char *modulename=NULL,*tranno=NULL;
	int x=0;
	short y=0;
	char *trnname=NULL;
	char e=FALSE;
	char allow_force=FALSE;

	readwidget(viewrsrc,"MODULE NAME");
	readwidget(viewrsrc,"TRANSACTION NUMBER");
	if(FINDRSCGETSTRING(viewrsrc,"MODULE NAME",&modulename)) return;
	if(FINDRSCGETSTRING(viewrsrc,"TRANSACTION NUMBER",&tranno)) return;

	trnname=Rmalloc(RDAstrlen(modulename)+RDAstrlen(tranno)+RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(trnname,"%s/%s/%s.TRN",CURRENTDIRECTORY,modulename,tranno);
#endif
#ifdef WIN32
	sprintf(trnname,"%s\\%s\\%s.TRN",CURRENTDIRECTORY,modulename,tranno);
#endif
	if(tran!=NULL) FreeRDAtran(tran);
	tran=NULL;
	tran=GETTRANSACTION(trnname);
	if(tran!=NULL)
	{
		if(allow_undo==TRUE)
		{
			y=ADVVERIFYTRANSACTION(tran,TRUE);
		} else {
			y=ADVVERIFYTRANSACTION(tran,FALSE);
		}
		if(y!=(-1))
		{	
			if(allow_undo==TRUE && tran->able==FALSE)
			{
				allow_force=TRUE;
				tran->able=TRUE;
			}
			FINDRSCSETSTRING(viewrsrc,"MODULE NAME",modulename);
			FINDRSCSETSTRING(viewrsrc,"TRANSACTION NUMBER",tranno);
			if(trnname!=NULL) Rfree(trnname);
			FINDRSCSETSTRING(viewrsrc,"PROCESS NAME",tran->process);
			FINDRSCSETSTRING(viewrsrc,"USER NAME",tran->user);
			FINDRSCSETSTRING(viewrsrc,"START TIME",tran->datetime);
			FINDRSCSETSTRING(viewrsrc,"END TIME",tran->endtime);
			if(tran->able==TRUE) e=TRUE; else e=FALSE;
			FINDRSCSETCHAR(viewrsrc,"ABLE TO UNDO",e);
			if(tran->complete==TRUE) e=TRUE; else e=FALSE;
			FINDRSCSETCHAR(viewrsrc,"COMPLETE",e);
			if(trnlst!=NULL) freeapplib(trnlst);
			trnlst=maketrnlst();
			if(trnlst==NULL)
			{
				trnlst=APPlibNEW();
				addAPPlib(trnlst,"Contains No Transactions");
			}
			x=0;
			FINDRSCLISTAPPlib(viewrsrc,"TRANSACTION LIST",x,trnlst);
			if(tran->able && tran->numrecs>0)
			{
				FINDRSCSETEDITABLE(viewrsrc,"UNDO",TRUE);
				FINDRSCSETSENSITIVE(viewrsrc,"UNDO",TRUE);
			} else {
				FINDRSCSETSENSITIVE(viewrsrc,"UNDO",FALSE);
				FINDRSCSETEDITABLE(viewrsrc,"UNDO",FALSE);
			}
			updateallrsrc(viewrsrc);
			if(!tran->able)
			{
				prterr("Error: UNABLE TO UNDO, this Transaction Log contains elements that may not be undone.  Newer transactions exist that must be undone before this transaction can be.");
				ERRORDIALOG("UNABLE TO UNDO","This Transaction Log contains elements that may not be undone.  Newer transactions exist that must be undone before this transaction can be.",NULL,FALSE);
			} else if(allow_force==TRUE)
			{
				prterr("Warning: DEVELOPMENT LICENSE UNDO, this Transaction Log contains elements that indicate that it shouldn't be undone.  Newer transactions exist that must be undone before this transaction can be.");
				WARNINGDIALOG("WARNING DIALOG SCREEN","DEVELOPMENT LICENSE UNDO","This Transaction Log contains elements that indicate that it should not be undone.  Newer transactions exist that must be undone before this transaction can be.",NULL,NULL,FALSE,viewrsrc);
			}
		} else {
			prterr("Error: VERIFY TRANSACTION FAILED, when verifying your access to this transactions, it was found that either you lacked the security access too view this transaction OR records in this transaction no longer exist.");
			ERRORDIALOG("VERIFY TRANSACTION FAILED","When verifying your access to this transactions, it was found that either you lacked the security access too view this transaction OR records in this transaction no longer exist.",NULL,FALSE);
		}
	} else {
		FINDRSCSETSTRING(viewrsrc,"PROCESS NAME",NULL);
		FINDRSCSETSTRING(viewrsrc,"USER NAME",NULL);
		if(trnname!=NULL) Rfree(trnname);
		FINDRSCSETSTRING(viewrsrc,"START TIME",NULL);
		FINDRSCSETSTRING(viewrsrc,"END TIME",NULL);
		e=FALSE;
		FINDRSCSETCHAR(viewrsrc,"ABLE TO UNDO",e);
		e=FALSE;
		FINDRSCSETCHAR(viewrsrc,"COMPLETE",e);
		if(trnlst!=NULL) freeapplib(trnlst);
		trnlst=APPlibNEW();
		addAPPlib(trnlst,"Contains No Transactions");
		x=0;
		FINDRSCLISTAPPlib(viewrsrc,"TRANSACTION LIST",x,trnlst);
		FINDRSCSETSENSITIVE(viewrsrc,"UNDO",FALSE);
		FINDRSCSETEDITABLE(viewrsrc,"UNDO",FALSE);
		updateallrsrc(viewrsrc);
		prterr("Error: TRANSACTION NOT FOUND, when attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.");
		ERRORDIALOG("TRANSACTION NOT FOUND","When attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.",NULL,FALSE);
	}
	if(modulename!=NULL) Rfree(modulename);
	if(tranno!=NULL) Rfree(tranno);
}
void viewtrans(RDArsrc *mainrsrc,RDArsrc *viewrsrc,char *modulename,char *tranno,char exit_status,char nextprev)
{
	int x=0;
	short y=0;
	char *trnname=NULL;
	char e=FALSE;
	char allow_force=FALSE;
	trnname=Rmalloc(RDAstrlen(module)+RDAstrlen(tranno)+RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(trnname,"%s/%s/%s.TRN",CURRENTDIRECTORY,modulename,tranno);
#endif
#ifdef WIN32
	sprintf(trnname,"%s\\%s\\%s.TRN",CURRENTDIRECTORY,modulename,tranno);
#endif
	tran=GETTRANSACTION(trnname);
	if(tran!=NULL)
	{
		if(allow_undo==TRUE)
		{
			y=ADVVERIFYTRANSACTION(tran,TRUE);
		} else {
			y=ADVVERIFYTRANSACTION(tran,FALSE);
		}
		if(y!=(-1))
		{
			if(allow_undo==TRUE && tran->able==FALSE)
			{
				allow_force=TRUE;
				tran->able=TRUE;
			}
			if(viewrsrc!=NULL) free_rsrc(viewrsrc);
			viewrsrc=RDArsrcNEW(module,"VIEW TRANSACTION");
			if(exit_status==0)
			{
				addstdrsrc(viewrsrc,"MODULE NAME",VARIABLETEXT,0,modulename,FALSE);
			} else {
				addstdrsrc(viewrsrc,"MODULE NAME",VARIABLETEXT,0,modulename,TRUE);
				FINDRSCSETFUNC(viewrsrc,"MODULE NAME",loadtrans,NULL);
			}
			addstdrsrc(viewrsrc,"PROCESS NAME",VARIABLETEXT,0,tran->process,FALSE);
			if(nextprev==TRUE)
			{
				addbtnrsrc(viewrsrc,"PREVIOUS",TRUE,showprev,NULL);
				addbtnrsrc(viewrsrc,"NEXT",TRUE,shownext,NULL);
			}
			addstdrsrc(viewrsrc,"USER NAME",VARIABLETEXT,0,tran->user,FALSE);
			if(exit_status==0)
			{
				addstdrsrc(viewrsrc,"TRANSACTION NUMBER",PLAINTEXT,10,tranno,FALSE);
			} else {
				addstdrsrc(viewrsrc,"TRANSACTION NUMBER",PLAINTEXT,10,tranno,TRUE);
				FINDRSCSETFUNC(viewrsrc,"TRANSACTION NUMBER",loadtrans,NULL);
			}
			addstdrsrc(viewrsrc,"START TIME",VARIABLETEXT,0,tran->datetime,FALSE);
			addstdrsrc(viewrsrc,"END TIME",VARIABLETEXT,0,tran->endtime,FALSE);
			if(tran->able==TRUE) e=TRUE; else e=FALSE;
			addstdrsrc(viewrsrc,"ABLE TO UNDO",BOOLNS,0,&e,FALSE);
			if(tran->complete==TRUE) e=TRUE; else e=FALSE;
			addstdrsrc(viewrsrc,"COMPLETE",BOOLNS,0,&e,FALSE);
			if(trnlst!=NULL) freeapplib(trnlst);
			trnlst=maketrnlst();
			if(trnlst==NULL)
			{
				trnlst=APPlibNEW();
				addAPPlib(trnlst,"Contains No Transactions");
			}
			addlstrsrc(viewrsrc,"TRANSACTION LIST",&x,TRUE,NULL,trnlst->numlibs,&trnlst->libs,NULL);
			addbtnrsrc(viewrsrc,"DETAIL",TRUE,showdetail,NULL);
			if(tran->able && tran->numrecs>0)
			{
				if(exit_status==0)
				{
					addrfkwrsrc(viewrsrc,"UNDO",TRUE,undotrans_noexit,NULL);
				} else {
					addrfkwrsrc(viewrsrc,"UNDO",TRUE,undotrans_exit,NULL);
				}
			} else {
				if(exit_status==0)
				{
					addrfkwrsrc(viewrsrc,"UNDO",FALSE,undotrans_noexit,NULL);
				} else {
					addrfkwrsrc(viewrsrc,"UNDO",FALSE,undotrans_exit,NULL);
				}
			}
			if(exit_status==0)
			{
				addrfkwrsrc(viewrsrc,"REDO",FALSE,redotrans_noexit,NULL);
			} else {
				addrfkwrsrc(viewrsrc,"REDO",FALSE,redotrans_exit,NULL);
			}
			if(dev_license==FALSE || tran->numrecs<1)
			{
				FINDRSCSETEDITABLE(viewrsrc,"REDO",FALSE);
				FINDRSCSETSENSITIVE(viewrsrc,"REDO",FALSE);
			}
			if(exit_status==0)
			{
				addrfkwrsrc(viewrsrc,"QUIT",TRUE,quitview,NULL);
			} else {
				addrfexrsrc(viewrsrc,"QUIT",TRUE,quitview_exit,NULL);
			}
			addbtnrsrc(viewrsrc,"HELP",TRUE,screenhelp,NULL);
        		addrfcbrsrc(viewrsrc,"PRINT TRANSACTION",TRUE,printtranscb,NULL);
			if(tran->numrecs<1)
			{
				FINDRSCSETEDITABLE(viewrsrc,"PRINT TRANSACTION",FALSE);
				FINDRSCSETSENSITIVE(viewrsrc,"PRINT TRANSACTION",FALSE);
			}
			addbtnrsrc(viewrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
			APPmakescrn(viewrsrc,TRUE,quitview,NULL,FALSE);
			if(!tran->able)
			{
				prterr("Error: UNABLE TO UNDO, this Transaction Log contains elements that may not be undone.  Newer transactions exist that must be undone before this transaction can be.");
				ERRORDIALOG("UNABLE TO UNDO","This Transaction Log contains elements that may not be undone.  Newer transactions exist that must be undone before this transaction can be.",NULL,FALSE);
			} else if(allow_force==TRUE)
			{
				prterr("Warning: DEVELOPMENT LICENSE UNDO, this Transaction Log contains elements that indicate that it shouldn't be undone.  Newer transactions exist that must be undone before this transaction can be.");
				WARNINGDIALOG("WARNING DIALOG SCREEN","DEVELOPMENT LICENSE UNDO","This Transaction Log contains elements that indicate that it should not be undone.  Newer transactions exist that must be undone before this transaction can be.",NULL,NULL,FALSE,mainrsrc);
			}
			if(tran->able && tran->numrecs>0)
			{
				FINDRSCSETEDITABLE(viewrsrc,"UNDO",TRUE);
				FINDRSCSETSENSITIVE(viewrsrc,"UNDO",TRUE);
			} else {
				FINDRSCSETEDITABLE(viewrsrc,"UNDO",FALSE);
				FINDRSCSETSENSITIVE(viewrsrc,"UNDO",FALSE);
			}
		} else {
			prterr("Error: VERIFY TRANSACTION FAILED, when verifying your access to this transactions, it was found that either you lacked the security access too view this transaction OR records in this transaction no longer exist.");
			if(exit_status==0)
			{
				ERRORDIALOG("VERIFY TRANSACTION FAILED","When verifying your access to this transactions, it was found that either you lacked the security access too view this transaction OR records in this transaction no longer exist.",NULL,FALSE);
			} else {
				ERRORDIALOG("VERIFY TRANSACTION FAILED","When verifying your access to this transactions, it was found that either you lacked the security access too view this transaction OR records in this transaction no longer exist.",NULL,TRUE);
			}
		}
	} else {
		if(exit_status==1)
		{
			if(viewrsrc!=NULL) free_rsrc(viewrsrc);
			viewrsrc=RDArsrcNEW(module,"VIEW TRANSACTION");
			addstdrsrc(viewrsrc,"MODULE NAME",VARIABLETEXT,0,modulename,TRUE);
			FINDRSCSETFUNC(viewrsrc,"MODULE NAME",loadtrans,NULL);
			addstdrsrc(viewrsrc,"TRANSACTION NUMBER",PLAINTEXT,10,tranno,TRUE);
			if(trnname!=NULL) Rfree(trnname);
			FINDRSCSETFUNC(viewrsrc,"TRANSACTION NUMBER",loadtrans,NULL);
			addstdrsrc(viewrsrc,"PROCESS NAME",VARIABLETEXT,0,NULL,FALSE);
			if(nextprev==TRUE)
			{
				addbtnrsrc(viewrsrc,"PREVIOUS",TRUE,showprev,NULL);
				addbtnrsrc(viewrsrc,"NEXT",TRUE,shownext,NULL);
			}
			addstdrsrc(viewrsrc,"USER NAME",VARIABLETEXT,0,NULL,FALSE);
			addstdrsrc(viewrsrc,"START TIME",VARIABLETEXT,0,NULL,FALSE);
			addstdrsrc(viewrsrc,"END TIME",VARIABLETEXT,0,NULL,FALSE);
			e=FALSE;
			addstdrsrc(viewrsrc,"ABLE TO UNDO",BOOLNS,0,&e,FALSE);
			e=FALSE;
			addstdrsrc(viewrsrc,"COMPLETE",BOOLNS,0,&e,FALSE);
			if(trnlst!=NULL) freeapplib(trnlst);
			trnlst=APPlibNEW();
			addAPPlib(trnlst,"Contains No Transactions");
			addlstrsrc(viewrsrc,"TRANSACTION LIST",&x,TRUE,NULL,trnlst->numlibs,&trnlst->libs,NULL);
			addbtnrsrc(viewrsrc,"DETAIL",TRUE,showdetail,NULL);
/*
			addrfkwrsrc(viewrsrc,"UNDO",TRUE,undotrans,NULL);
*/
			addrfkwrsrc(viewrsrc,"UNDO",TRUE,undotrans_exit,NULL);
			addrfkwrsrc(viewrsrc,"REDO",TRUE,redotrans_exit,NULL);
			addrfexrsrc(viewrsrc,"QUIT",TRUE,quitview_exit,NULL);
			addbtnrsrc(viewrsrc,"HELP",TRUE,screenhelp,NULL);
       			addrfcbrsrc(viewrsrc,"PRINT TRANSACTION",TRUE,printtranscb,NULL);
			addbtnrsrc(viewrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
			APPmakescrn(viewrsrc,TRUE,quitview,NULL,FALSE);
			FINDRSCSETEDITABLE(viewrsrc,"UNDO",FALSE);
			FINDRSCSETSENSITIVE(viewrsrc,"UNDO",FALSE);
			FINDRSCSETEDITABLE(viewrsrc,"REDO",FALSE);
			FINDRSCSETSENSITIVE(viewrsrc,"REDO",FALSE);
		}
		prterr("Error: TRANSACTION NOT FOUND, when attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.");
		ERRORDIALOG("TRANSACTION NOT FOUND","When attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.",NULL,FALSE);
	}
}
void viewtranscb(RDArsrc *mainrsrc)
{
	int selected=0,tlist=0;
	char *tmp=NULL,*modulename=NULL;

	readwidget(mainrsrc,"MODULES AVAILABLE");
	readwidget(mainrsrc,"LOG LIST");
	if(FINDRSCGETINT(mainrsrc,"MODULES AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LOG LIST",&tlist)) return;
	modulename=stralloc(modlst->libs[selected]);
	tmp=stralloc(loglst->libs[tlist]);
	tmp[10]=0;
	viewtrans(mainrsrc,NULL,modulename,tmp,0,FALSE);
	if(tmp!=NULL) Rfree(tmp);
	if(modulename!=NULL) Rfree(modulename);
}
void viewtranscl(char *modulename,char *tranno)
{
	int length=0;
	int x=0;

	length=RDAstrlen(tranno);
	memset(stemp,0,101);
	for(x=0;x<(10-length);++x)
	{
		strcat(stemp,"0");
	}
	strcat(stemp,tranno);
/*
	switch(length)
	{
		case 1:
			sprintf(stemp,"000000000%s",tranno);
			break;
		case 2:
			sprintf(stemp,"00000000%s",tranno);
			break;
		case 3:
			sprintf(stemp,"0000000%s",tranno);
			break;
		case 4:
			sprintf(stemp,"000000%s",tranno);
			break;
		case 5:
			sprintf(stemp,"00000%s",tranno);
			break;
		case 6:
			sprintf(stemp,"0000%s",tranno);
			break;
		case 7:
			sprintf(stemp,"000%s",tranno);
			break;
		case 8:
			sprintf(stemp,"00%s",tranno);
			break;
		case 9:
			sprintf(stemp,"0%s",tranno);
			break;
		case 10:
			sprintf(stemp,"%s",tranno);
			break;
	}
*/
	viewtrans(NULL,NULL,modulename,stemp,1,FALSE);
}
static void deletetranstest(RDArsrc *mainrsrc)
{
        int s=0;

/*
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&s)) return;
*/
	if(FINDRSCGETINT(mainrsrc,"LOG LIST",&s)) return;
	if(!RDAstrcmp(loglst->libs[s],"Contains No Transactions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE THIS TRANSACTION?","Deleting this Transction will perminately remove the TRANSACTION LOG.\nAre you sure that you want to DELETE this TRANSACTION?",
 		deletetrans,NULL,FALSE,mainrsrc);
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
/*
	char *sdated=NULL,*sdatet=NULL,*edated=NULL,*edatet=NULL;
*/
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
							edate=stralloc("Incomplete Transaction");
							odesc=stralloc(" ");
						}
					} else {
						edate=stralloc("Incomplete Transaction");
						odesc=stralloc(" ");
					}
				} else {
					edate=stralloc("Incomplete Transaction");
					odesc=stralloc(" ");
				}
			} else {
				edate=stralloc("Incomplete Transaction");
				odesc=stralloc(" ");
			}
		} else {
			edate=stralloc("Incomplete Transaction");
			odesc=stralloc(" ");
		}
		tname=stralloc(name);
		tname[RDAstrlen(tname)-4]=0;
		tmp=Rmalloc((tname!=NULL?RDAstrlen(tname):1)+(user!=NULL?RDAstrlen(user):1)+(process!=NULL?RDAstrlen(process):1)+(sdate!=NULL?RDAstrlen(sdate):1)+(edate!=NULL?RDAstrlen(edate):1)+(odesc!=NULL?RDAstrlen(odesc):1)+6);
		sprintf(tmp,"%s %s %s %s %s %s",(tname!=NULL?tname:" "),
			(user!=NULL?user:" "),(process!=NULL?process:" "),
			(sdate!=NULL?sdate:" "),(edate!=NULL?edate:"?"),
			(odesc!=NULL?odesc:" ")); 
/*
		if(!isEMPTY(sdate))
		{
			sdated=strtok(sdate," ");
			sdatet=strtok(NULL," ");
		} 
		if(!isEMPTY(edate))
		{
			edated=strtok(edate," ");
			edatet=strtok(NULL," ");
		}
		if(RDAstrlen(process)>30)
		{
			process[30]=0;
		}
		if(RDAstrlen(user)>15)
		{
			user[15]=0;
		}
		tmp=Rmalloc(RDAstrlen(tname)+15+30+10+8+10+8+RDAstrlen(odesc)+6);
		sprintf(tmp,"%-s %-15s %-30s %-10s %-8s %-10s %-8s %-s",tname,
			(user!=NULL?user:" "),(process!=NULL?process:" "),
			(sdated!=NULL?sdated:" "),(sdatet!=NULL?sdatet:"?"),
			(edated!=NULL?edated:" "),(edatet!=NULL?edatet:"?"),
			(odesc!=NULL?odesc:" ")); 
*/
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
	RDA_PFILE *fp=NULL;
	char *header=NULL,*modname=NULL,*trnnum=NULL,*process=NULL,*user=NULL;
	char *outputdevice=NULL,*prtmessage=NULL;
	int pages=0,lines=0;
	char AbleToUndo=FALSE,complete=FALSE,print_style=TRUE;
	unsigned line_length=79;

	readwidget(prtrsrc,"DEVICE");
	if(FINDRSCGETSTRING(prtrsrc,"DEVICE",&outputdevice)) return;
	readwidget(prtrsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prtrsrc,"NORMAL PRINT",&print_style);
	if(outputdevice!=NULL)
	{
		if((fp=RDA_popen(outputdevice,"w"))!=NULL)
		{
			SelectPrintType(outputdevice,print_style,fp);
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
			RDA_fprintf(fp,"Status: Able To Undo = [%s]  Complete = [%s]\n",(AbleToUndo?"TRUE":"FALSE"),(complete?"TRUE":"FALSE")); 
			++lines;
			if(RDAstrcmp(trnlst->libs[0],"Contains No Transactions"))
			{
				RDA_fprintf(fp,"Transactions are as follows:\n\n");
				lines+=2;
				printdetail(fp,&lines,&pages,line_length,modname,trnnum,process,user);
			}
			if(modname!=NULL) Rfree(modname);
			if(trnnum!=NULL) Rfree(trnnum);
			if(process!=NULL) Rfree(process);
			if(user!=NULL) Rfree(user);
			RDA_fprintf(fp,"\n");
			RDA_pclose(fp);
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
	char *devicex=NULL,normal_print=TRUE;
	RDArsrc *prtrsrc=NULL;

	devicex=DefaultPrinter();
	prtrsrc=RDArsrcNEW(module,"PRINT SCREEN");
	addbtnrsrc(prtrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prtrsrc,"DEVICE",VARIABLETEXT,0,devicex,TRUE);
	addstdrsrc(prtrsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(devicex!=NULL) Rfree(devicex);
	DefaultScreens(prtrsrc);
	addrfkwrsrc(prtrsrc,"PRINT",TRUE,printtrans,viewrsrc);
	addbtnrsrc(prtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfkwrsrc(prtrsrc,"QUIT",TRUE,quitrsrc,NULL);
	addbtnrsrc(prtrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	APPmakescrn(prtrsrc,TRUE,quitrsrc,NULL,FALSE);
}
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *header,
	unsigned line_length,char *modname,char *trnnum,char *process,char *user)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(25+RDAstrlen(modname)+RDAstrlen(trnnum)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(25+RDAstrlen(modname)+RDAstrlen(trnnum)))/2)-14)));
	if(tail_ws<0) tail_ws=0;
	RDA_fprintf(fp,"%*s Module: %s  Transaction: %s %*sDate: %s\n",
		lead_ws,"",modname,trnnum,tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(19+RDAstrlen(process)+RDAstrlen(user)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(19+RDAstrlen(process)+RDAstrlen(user)))/2)-14)));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(process)+RDAstrlen(user)+14+19+lead_ws+tail_ws)>(line_length))
	{
		RDA_fprintf(fp,"%*sProcess: %s %*s\n",
			(int)(round(((double)(line_length-(10+RDAstrlen(process)))/2))),"",
			process,
			(int)(Rint((((double)(line_length-(10+RDAstrlen(process)))/2)-14))),"");
		RDA_fprintf(fp,"%*sUser: %s %*s\n",
			(int)(round(((double)(line_length-(7+RDAstrlen(user)))/2))),"",
			user,
			(int)(Rint((((double)(line_length-(7+RDAstrlen(user)))/2)-14))),"");
		*lines+=2;
	} else {
		RDA_fprintf(fp,"%*s Process: %s  User: %s %*s\n",
			lead_ws,"",process,user,tail_ws,"");
		++*lines;
	}
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+14+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime: %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
			timex);
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime: %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	RDA_fprintf(fp,"\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
void redoentiretrans(RDArsrc *mainrsrc,RDAtran *tran)
{
	int x=0;
	RDAtrec *trec;

	if(tran!=NULL)
	{
		tran->able=TRUE;
		for(x=tran->numrecs-1;x>(-1);--x)
		{
			trec=tran->recs+x;
			RedoRDAtrec(trec);
		}
		if(tran!=NULL) FreeRDAtran(tran);
		tran=NULL;
	}
}
void redoentiretranscb(RDArsrc *mainrsrc)
{
	int selected=0,tlist=0;
	int y=0;
	char *trnname=NULL,*tmp=NULL;

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
	if(tmp!=NULL) Rfree(tmp);
	tran=GETTRANSACTION(trnname);
	if(trnname!=NULL) Rfree(trnname);
	if(tran!=NULL)
	{
		tran->able=TRUE;
		y=ADVVERIFYTRANSACTION(tran,TRUE);
		if(y!=(-2))
		{
			redoentiretrans(mainrsrc,tran);
		} else {
			prterr("Error: VERIFY TRANSACTION FAILED, when verifying your access to this transactions, it was found that security access was not high enough for you to even view this transaction.",NULL,FALSE);
			ERRORDIALOG("VERIFY TRANSACTION FAILED","When verifying your access to this transactions, it was found that security access was not high enough for you to even view this transaction.",NULL,FALSE);
		}
	} else {
		prterr("Error: TRANSACTION NOT FOUND, when attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.");
		ERRORDIALOG("TRANSACTION NOT FOUND","When attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.",NULL,FALSE);
	}
}
void undoentiretrans(RDArsrc *mainrsrc,RDAtran *tran)
{
	int x=0;
	RDAtrec *trec;

	if(tran!=NULL)
	{
		tran->able=TRUE;
		for(x=tran->numrecs-1;x>(-1);--x)
		{
			trec=tran->recs+x;
			UndoRDAtrec(trec);
			trec->undone=TRUE;
		}
		if(tran!=NULL) FreeRDAtran(tran);
		tran=NULL;
	}
}
void undoentiretranscb(RDArsrc *mainrsrc)
{
	int selected=0,tlist=0;
	int y=0;
	char *trnname=NULL,*tmp=NULL;

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
	if(tmp!=NULL) Rfree(tmp);
	tran=GETTRANSACTION(trnname);
	if(trnname!=NULL) Rfree(trnname);
	if(tran!=NULL)
	{
		tran->able=TRUE;
		y=ADVVERIFYTRANSACTION(tran,TRUE);
		if(y!=(-2))
		{
			if(tran->able==FALSE)
			{
				prterr("Warning: DEVELOPMENT LICENSE UNDO, this Transaction Log contains elements that indicate that it shouldn't be undone.  Newer transactions exist that must be undone before this transaction can be.");
				ADVWARNINGDIALOG("WARNING DIALOG SCREEN","DEVELOPMENT LICENSE UNDO","This Transaction Log contains elements that indicate that it should not be undone.  Newer transactions exist that must be undone before this transaction can be.",undoentiretrans,NULL,FALSE,2,mainrsrc,tran,NULL);
			} else {
				undoentiretrans(mainrsrc,tran);
			}
		} else {
			prterr("Error: VERIFY TRANSACTION FAILED, when verifying your access to this transactions, it was found that security access was not high enough for you to even view this transaction.",NULL,FALSE);
			ERRORDIALOG("VERIFY TRANSACTION FAILED","When verifying your access to this transactions, it was found that security access was not high enough for you to even view this transaction.",NULL,FALSE);
		}
	} else {
		prterr("Error: TRANSACTION NOT FOUND, when attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.");
		ERRORDIALOG("TRANSACTION NOT FOUND","When attempting to verify the selected transaction, it was found that the transaction file did not exist or could not be opened.",NULL,FALSE);
	}
}
static void setdevlicense(RDArsrc *r)
{
	readwidget(r,"DEV_LICENSE");
	FINDRSCGETSTRING(r,"DEV_LICENSE",&devlicense);
	if(CheckDevLicense(devlicense))
	{
		dev_license=TRUE;
	} else {
		dev_license=FALSE;
	}
	FINDRSCSETEDITABLE(r,"DELETE",(dev_license==TRUE ? TRUE:FALSE));
	FINDRSCSETSENSITIVE(r,"DELETE",(dev_license==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"UNDO",(dev_license==TRUE ? TRUE:FALSE));
	FINDRSCSETSENSITIVE(r,"UNDO",(dev_license==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"REDO",(dev_license==TRUE ? TRUE:FALSE));
	FINDRSCSETSENSITIVE(r,"REDO",(dev_license==TRUE ? TRUE:FALSE));
	updateallrsrc(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;
	char *tmp=NULL,*temp1=NULL;

	if(InitializeSubsystems(argc,argv,module,"VIEW TRANSACTIONS")) 
	{
		return;
	}
	tmp=RDA_GetEnv("DEV_LICENSE");
	if(tmp!=NULL)
	{
		temp1=strtok(tmp," ");
		devlicense=stralloc(temp1);
		if(CheckDevLicense(devlicense))
		{
			allow_undo=TRUE;
			dev_license=TRUE;
		} else {
			allow_undo=FALSE;
			dev_license=FALSE;
		}
	} else {
		allow_undo=FALSE;
	}
	if(argc<3)
	{
		mainrsrc=RDArsrcNEW(module,"VIEW LOG ENTRIES");
		addstdrsrc(mainrsrc,"DEV_LICENSE",VARIABLETEXT,0,devlicense,TRUE);
		FINDRSCSETFUNC(mainrsrc,"DEV_LICENSE",setdevlicense,NULL);
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
		addlstrsrc(mainrsrc,"LOG LIST",&x,TRUE,NULL,loglst->numlibs,&loglst->libs,NULL);
		gettransentries(mainrsrc);
		addbtnrsrc(mainrsrc,"VIEW",TRUE,viewtranscb,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quittrans,NULL);
		addbtnrsrc(mainrsrc,"DELETE",TRUE,deletetranstest,NULL);
		addbtnrsrc(mainrsrc,"UNDO",TRUE,undoentiretranscb,NULL);
		addbtnrsrc(mainrsrc,"REDO",TRUE,redoentiretranscb,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		setdevlicense(mainrsrc);
		APPmakescrn(mainrsrc,TRUE,quittrans,NULL,TRUE);
	} else {
		viewtranscl(argv[1],argv[2]);
	}
}
