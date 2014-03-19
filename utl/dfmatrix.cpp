/* dfmatrix.c - Define Tables */
#ifndef WIN32
#define __NAM__ "dfmatrix.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfmatrix.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>
/*CPP_OPART dfmatrx1 */
/*CPP_OPART dfmatrxp */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define CONVERTFIELDTYPE(a,b,c,d,e,f)	xCONVERTFIELDTYPE(a,b,c,d,e,b,f,__LINE__,__FILE__)
#define COPYCONVERTFIELDTYPE(a,b,c,d,e,f,g)	xCONVERTFIELDTYPE(a,b,c,d,e,f,g,__LINE__,__FILE__)
extern void printmatrix(RDArsrc *);
extern short xCONVERTFIELDTYPE(short *,union rda_field_types *,unsigned *,unsigned *,short,union rda_field_types *,short,int,char *);
char *module="UTILITIES";
APPlib *modlst=NULL,*matrixavl=NULL,*edittypes=NULL,*cellvalues=NULL;
APPlib *fieldtypes=NULL,*adddels=NULL,*beforeafters=NULL,*columnlist=NULL;
APPlib *rowlist=NULL,*computetypes=NULL,*axistypes=NULL;
APPlib *windowtypes=NULL,*cellconfig=NULL;
RDAMATRIX *DFV=NULL;
static void getmodulesavailable(RDArsrc *),domodulelist(RDArsrc *);
static void savematrix(RDArsrc *);
static void quitmatrixtest(RDArsrc *),setvaluesx(RDArsrc *);
static RDAdefault *matrixdefaults=NULL,*celldefaults=NULL;
static void print_matrixscb(RDArsrc *);
static void print_matrixs(RDArsrc *,RDArsrc *);
static char changedvalues=FALSE;
static void quitmatrixmenu(RDArsrc *);
static void GenerateMatrixRDAscrn(RDArsrc *);
static RDArsrc *GenerateMatrixRDArsrc(void);
static void quiteditmatrix(RDArsrc *);
static void quitedit(RDArsrc *);
static void save_cell(RDArsrc *,RDArsrc *);
static void quit_cell(RDArsrc *,RDArsrc *);
void CreateRowList(void),CreateColumnList(void);
static MATRIXcell *HoldCells=NULL;
static int numHoldCells=0;
void readmatrix(void);
#define MakeSpaceMatrixColumn(a)	xMakeSpaceMatrixColumn(a,__LINE__,__FILE__)
extern void xMakeSpaceMatrixColumn(int,int,char *);
#define DeleteSpaceMatrixColumn(a)	xDeleteSpaceMatrixColumn(a,__LINE__,__FILE__)
extern void xDeleteSpaceMatrixColumn(int,int,char *);
#define MakeSpaceMatrixRow(a)	xMakeSpaceMatrixRow(a,__LINE__,__FILE__)
extern void xMakeSpaceMatrixRow(int,int,char *);
#define DeleteSpaceMatrixRow(a)	xDeleteSpaceMatrixRow(a,__LINE__,__FILE__)
void xDeleteSpaceMatrixRow(int,int,char *);
short RowMultiplier=0;
int RowNumber=0,ColumnNumber=0,ValueNumber=0;
char AddPercentage=FALSE,AddAmount=FALSE,RoundDollars=FALSE;
double Percentage=0.0,Amount=0.0;
RDArsrc *MAINRSRC=NULL;

#ifdef XXXX
#define SEEMATRIXDATA(a,b,c,d,e)	xSEEMATRIXDATA(a,b,c,d,e,__LINE__,__FILE__)
#endif
void xSEEMATRIXDATA(char *,short,union rda_field_types,unsigned,unsigned,int,char *);
void xSEEMATRIXDATA(char *name,short type,union rda_field_types data,unsigned len,unsigned dlen,int line,char *file)
{
	prterr("[%s] Type [%d] [%s] Length [%d] dlen [%d] ",(name!=NULL ? name:""),type,fieldtypedesc[type],len,dlen);
	switch(type)
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
			prterr("[%s]",(data.string_value!=NULL ? data.string_value:"NULL"));
			break;
		case BOOLNS:
			prterr("[%d] [%s]",*data.string_value,(*data.string_value ? "Yes":"No"));
			break;
		case CHARACTERS:
			prterr("[%c]",*data.string_value);
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			prterr("[%f]",*data.float_value);
			break;
		case SHORTV:
		case SSHORTV:
			prterr("[%d]",*data.short_value);
			break;
		case LONGV:
		case SLONGV:
			prterr("[%d]",*data.integer_value);
			break;
		default:
			prterr(" at line [%d] program [%s]\n",line,file);
			prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",type,(name!=NULL ? name:""),line,file);
			return;
	}
	prterr(" at line [%d] program [%s]\n",line,file);
}
void MatrixSubData(char **tmp,RDAMATRIX *M)
{
	char *string=NULL,*temp=NULL,*gn=NULL;
	short start=1,length=0;
	int rowx=0,colx=0,numx=0,value=0,str_length=0;
	double dnum=0,dnum1=0,dnum2=0;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXcell *c;
	MATRIXvalue *v;
	AXISmem *am=NULL;

	if(M!=NULL)
	{
		string=stripmodulename(*tmp);
		if(!RDAstrcmp(string,"ROW NUMBER"))
		{
			sprintf(stemp,"%d",RowNumber+1);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"COLUMN NUMBER"))
		{
			sprintf(stemp,"%d",ColumnNumber+1);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"CELL NUMBER"))
		{
			sprintf(stemp,"%d",(RowNumber*M->xaxis.num)+ColumnNumber+1);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"ADD PERCENTAGE"))
		{
			sprintf(stemp,"%d",(AddPercentage ? TRUE:FALSE));
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"ROUND DOLLARS"))
		{
			sprintf(stemp,"%d",(RoundDollars ? TRUE:FALSE));
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"PERCENTAGE"))
		{
			sprintf(stemp,"%f",Percentage);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"ADD AMOUNT"))
		{
			sprintf(stemp,"%d",(AddAmount ? TRUE:FALSE));
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"AMOUNT"))
		{
			sprintf(stemp,"%f",Amount);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"CELL VALUE NUMBER"))
		{
			sprintf(stemp,"%d",ValueNumber+1);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
/*
		} else if(!RDAstrcmp(string,"CELL VALUE INDEX"))
		{
			sprintf(stemp,"[R%dC%dV%d]",RowNumber+1,ColumnNumber+1,ValueNumber+1);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
*/
		} else if(!RDAstrcmp(string,"CELL VALUE"))
		{
			if(RowNumber<M->num)
			{
				rec=M->recs+RowNumber;
				if(rec->num>ColumnNumber)
				{
					mem=rec->mems+ColumnNumber;
					if(mem->sensitive)
					{
						if(mem->values!=NULL && DFV->defs!=NULL)
						{
							v=mem->values+ValueNumber;
							c=M->defs+(ValueNumber);
							switch(c->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case ZIPCODE:
									if(v->data.string_value!=NULL)
									{
										sprintf(stemp,"\"%s\"",v->data.string_value);
										str_length=RDAstrlen(stemp);
										if(RDAstrlen(*tmp)<str_length)
										{
											*tmp=Rrealloc(*tmp,str_length+1);
										}
										memset(*tmp,0,RDAstrlen(*tmp)+1);
										memcpy(*tmp,stemp,str_length+1);
									}
									break;
								case BOOLNS:
								case CHARACTERS:
									sprintf(stemp,"%d",*v->data.string_value);
									str_length=RDAstrlen(stemp);
									if(RDAstrlen(*tmp)<str_length)
									{
										*tmp=Rrealloc(*tmp,str_length+1);
									}
									memset(*tmp,0,RDAstrlen(*tmp)+1);
									memcpy(*tmp,stemp,str_length+1);
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									sprintf(stemp,"%f",*v->data.float_value);
									str_length=RDAstrlen(stemp);
									if(RDAstrlen(*tmp)<str_length)
									{
										*tmp=Rrealloc(*tmp,str_length+1);
									}
									memset(*tmp,0,RDAstrlen(*tmp)+1);
									memcpy(*tmp,stemp,str_length+1);
									break;
								case SHORTV:
								case SSHORTV:
									sprintf(stemp,"%d",*v->data.short_value);
									str_length=RDAstrlen(stemp);
									if(RDAstrlen(*tmp)<str_length)
									{
										*tmp=Rrealloc(*tmp,str_length+1);
									}
									memset(*tmp,0,RDAstrlen(*tmp)+1);
									memcpy(*tmp,stemp,str_length+1);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									sprintf(stemp,"%d",*v->data.integer_value);
									str_length=RDAstrlen(stemp);
									if(RDAstrlen(*tmp)<str_length)
									{
										*tmp=Rrealloc(*tmp,str_length+1);
									}
									memset(*tmp,0,RDAstrlen(*tmp)+1);
									memcpy(*tmp,stemp,str_length+1);
									break;
								case BUTTONS:
									prterr("Error: Buttons have no value, call Installer");
									break;
								default:
									prterr("Error field type [%d] invalid for Cell Value # [%d] on Table [%s] [%s].",c->type,((RowNumber*M->xaxis.num)+ColumnNumber+1),DFV->module,DFV->name);
									break;
							}
						}
					}
				}
			}
			sprintf(stemp,"%d",(RowNumber*M->xaxis.num)+ColumnNumber+1);
		} else if(!RDAstrcmp(string,"CELL SENSITIVITY"))
		{
			length=FALSE;
			if(RowNumber<M->num)
			{
				rec=M->recs+RowNumber;
				if(rec->num>ColumnNumber)
				{
					mem=rec->mems+ColumnNumber;
					if(mem->sensitive) length=TRUE;
						else length=FALSE;
				}
			}
			sprintf(stemp,"%d",length);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
			length=0;
		} else if(!RDAstrcmp(string,"NUMBER OF CELLS"))
		{
			sprintf(stemp,"%d",(RowNumber*M->xaxis.num)+ColumnNumber+1);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"NUMBER OF COLUMNS"))
		{
			sprintf(stemp,"%d",M->xaxis.num);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"NUMBER OF ROWS"))
		{
			sprintf(stemp,"%d",M->yaxis.num);
			str_length=RDAstrlen(stemp);
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			memcpy(*tmp,stemp,str_length+1);
		} else if(!RDAstrcmp(string,"CURRENT Y-AXIS VALUE"))
		{
			if(RowNumber<M->yaxis.num)
			{
				am=M->yaxis.mem+RowNumber;
				switch(M->yaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						temp=stralloc(am->data.string_value);
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
						} else if(start<=str_length && !isEMPTY(temp))
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
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%d",
							(*am->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%c",*am->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(*am->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*am->data.float_value);
						} else {
							sprintf(stemp,"%f",*am->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SHORTV:
					case SSHORTV:
						if(*am->data.short_value<0)
						{
							sprintf(stemp,"(%d)",*am->data.short_value);
						} else {
							sprintf(stemp,"%d",*am->data.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case LONGV:
					case SLONGV:
						if(*am->data.integer_value<0)
						{
							sprintf(stemp,"(%d)",*am->data.integer_value);
						} else {
							sprintf(stemp,"%d",*am->data.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SCROLLEDLIST:
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for Y-Axis on Table [%s] [%s].",M->yaxis.type,M->module,M->name);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						break;
				}
			} else {
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
		} else if(!RDAstrcmp(string,"CURRENT Y-AXIS TITLE 1"))
		{
			if(RowNumber<M->yaxis.num)
			{
				am=M->yaxis.mem+RowNumber;
				temp=am->title1;
			} else temp=NULL;
			str_length=RDAstrlen(temp)+2;
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",(temp!=NULL ? temp:""));
		} else if(!RDAstrcmp(string,"CURRENT Y-AXIS TITLE 2"))
		{
			if(RowNumber<M->yaxis.num)
			{
				am=M->yaxis.mem+RowNumber;
				temp=am->title2;
			} else temp=NULL;
			str_length=RDAstrlen(temp)+2;
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",(temp!=NULL ? temp:""));
		} else if(!RDAstrcmp(string,"CURRENT X-AXIS VALUE"))
		{
			if(ColumnNumber<M->xaxis.num)
			{
				am=M->xaxis.mem+ColumnNumber;
				switch(M->xaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						temp=stralloc(am->data.string_value);
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
						} else if(start<=str_length && !isEMPTY(temp))
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
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%d",
							(*am->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%c",*am->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(*am->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*am->data.float_value);
						} else {
							sprintf(stemp,"%f",*am->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SHORTV:
					case SSHORTV:
						if(*am->data.short_value<0)
						{
							sprintf(stemp,"(%d)",*am->data.short_value);
						} else {
							sprintf(stemp,"%d",*am->data.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case LONGV:
					case SLONGV:
						if(*am->data.integer_value<0)
						{
							sprintf(stemp,"(%d)",*am->data.integer_value);
						} else {
							sprintf(stemp,"%d",*am->data.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SCROLLEDLIST:
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for X-Axis on Table [%s] [%s].",M->xaxis.type,M->module,M->name);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						break;
				}
			} else {
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
		} else if(!RDAstrcmp(string,"CURRENT X-AXIS TITLE 1"))
		{
			if(ColumnNumber<M->xaxis.num)
			{
				am=M->xaxis.mem+ColumnNumber;
				temp=am->title1;
			} else temp=NULL;
			str_length=RDAstrlen(temp)+2;
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",(temp!=NULL ? temp:""));
		} else if(!RDAstrcmp(string,"CURRENT X-AXIS TITLE 2"))
		{
			if(ColumnNumber<M->xaxis.num)
			{
				am=M->xaxis.mem+ColumnNumber;
				temp=am->title2;
			} else temp=NULL;
			str_length=RDAstrlen(temp)+2;
			if(RDAstrlen(*tmp)<str_length)
			{
				*tmp=Rrealloc(*tmp,str_length+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",(temp!=NULL ? temp:""));
		} else if(!isEMPTY(string))
		{
			if(!M->locate_method)
			{
				temp=string+1;
				rowx=Ratoi(temp);
				temp=strchr(string,'C');
				++temp;
				colx=Ratoi(temp);
				temp=strchr(temp,'V');
				++temp;
				value=Ratoi(temp);
			} else {
				temp=string+11;
				numx=Ratoi(temp);
				if(numx>M->xaxis.num)
				{
					dnum1=numx;
					dnum2=M->xaxis.num;
					dnum=fmod(dnum1,dnum2);
/*lint -e524 */
					rowx=dnum;
					colx=(numx-(rowx*M->xaxis.num));
/*lint +e524 */
					temp=RDAstrstr(string,"VALUE");
					temp=temp+6;
					value=Ratoi(temp);
				} else {
					rowx=1;
					colx=numx;
					temp=RDAstrstr(string,"VALUE");
					temp=temp+6;
					value=Ratoi(temp);
				}
			}
			if(M->defs!=NULL)
			{
				if((rowx-1)<M->num && (colx-1)<M->xaxis.num)
				{
					rec=DFV->recs+(rowx-1);
					mem=rec->mems+(colx-1);
					if(value<=mem->num && value<=M->numdefs)
					{
						c=M->defs+(value-1);
/*
						c=M->defs+value;
*/
						v=mem->values+(value-1);
/*
						v=mem->values+value;
*/
						switch(c->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								temp=stralloc(v->data.string_value);
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
								} else if(start<=str_length && !isEMPTY(temp))
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
							case BOOLNS:
								if(RDAstrlen(*tmp)<2)
								{
									*tmp=Rrealloc(*tmp,2);
								}
								memset(*tmp,0,RDAstrlen(*tmp)+1);
								sprintf(*tmp,"%d",
									(*v->data.string_value ? 1:0));
								break;
							case CHARACTERS:
								if(RDAstrlen(*tmp)<2)
								{
									*tmp=Rrealloc(*tmp,2);
								}
								memset(*tmp,0,RDAstrlen(*tmp)+1);
								sprintf(*tmp,"%c",*v->data.string_value);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(*v->data.float_value<0)
								{
									sprintf(stemp,"(%f)",*v->data.float_value);
								} else {
									sprintf(stemp,"%f",*v->data.float_value);
								}
								str_length=RDAstrlen(stemp);
								if(RDAstrlen(*tmp)<str_length)
								{
									*tmp=Rrealloc(*tmp,str_length+1);
								}
								memset(*tmp,0,RDAstrlen(*tmp)+1);
								memcpy(*tmp,stemp,str_length+1);
								break;
							case SHORTV:
							case SSHORTV:
								if(*v->data.short_value<0)
								{
									sprintf(stemp,"(%d)",*v->data.short_value);
								} else {
									sprintf(stemp,"%d",*v->data.short_value);
								}
								str_length=RDAstrlen(stemp);
								if(RDAstrlen(*tmp)<str_length)
								{
									*tmp=Rrealloc(*tmp,str_length+1);
								}
								memset(*tmp,0,RDAstrlen(*tmp)+1);
								memcpy(*tmp,stemp,str_length+1);
								break;
							case LONGV:
							case SLONGV:
								if(*v->data.integer_value<0)
								{
									sprintf(stemp,"(%d)",*v->data.integer_value);
								} else {
									sprintf(stemp,"%d",*v->data.integer_value);
								}
								str_length=RDAstrlen(stemp);
								if(RDAstrlen(*tmp)<str_length)
								{
									*tmp=Rrealloc(*tmp,str_length+1);
								}
								memset(*tmp,0,RDAstrlen(*tmp)+1);
								memcpy(*tmp,stemp,str_length+1);
								break;
							case SCROLLEDLIST:
							case BUTTONS:
								prterr("Error call RaddogTiddog cause this is weird....");
								break;
							default:
								prterr("Error field type [%d] invalid for cell # [%d] on Table [%s] [%s].",c->type,value,M->module,M->name);
								break;
						}
					} else {
						prterr("Error Field [%s] not found.",*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
					}
				} else {
					prterr("Error Field [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
				}
			} else {
				prterr("Error Field [%s] not found.",*tmp);
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
		} else {
			prterr("Error Field [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
		}
	} else {
		prterr("Error Field [%s] not found.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=stralloc("\"\"");
	}
	if(string!=NULL) Rfree(string);
}
static void savematrixtest(RDArsrc *mainrsrc)
{
	char *libname=NULL;

	readmatrix();
	if(DFV!=NULL)
	{
		if(!isEMPTY(DFV->module) && !isEMPTY(DFV->name))
		{
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(DFV->module)+10);
#ifndef WIN32
			sprintf(libname,"%s/rda/%s.MTX",CURRENTDIRECTORY,DFV->module);
#endif
#ifdef WIN32
			sprintf(libname,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,DFV->module);
#endif
			if(writeRDAMatrixbin(libname,DFV))
			{
				ERRORDIALOG("Cannot Write RDA Matrix","Error attempting to write the matrix.  Check the permissions on the RDA Matrix Binary Library, and retry.  Call your installer.",NULL,FALSE);
			}
			if(libname!=NULL) Rfree(libname);
			quiteditmatrix(MAINRSRC);
		}
	} else {
		ERRORDIALOG("Cannot Write RDA Matrix","Error RDA Matrix Structure not found and/or is NULL.  Call your installer.",NULL,FALSE);
	}
}
static void quitmatrixtest(RDArsrc *mainrsrc)
{
	readallwidgets(MAINRSRC);
	if(COMPARE_RSRCS(MAINRSRC,matrixdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the TABLE DEFINITION.\nDo you want to Save these changes?",savematrixtest,quiteditmatrix,FALSE,1,MAINRSRC,NULL,NULL);
	} else {
		quiteditmatrix(MAINRSRC);
	}
}
static void savematrix(RDArsrc *mainrsrc)
{
	char *libname=NULL;

	readmatrix();
	if(DFV!=NULL)
	{
		if(!isEMPTY(DFV->module) && !isEMPTY(DFV->name))
		{
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(DFV->module)+10);
#ifndef WIN32
			sprintf(libname,"%s/rda/%s.MTX",CURRENTDIRECTORY,DFV->module);
#endif
#ifdef WIN32
			sprintf(libname,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,DFV->module);
#endif
			if(writeRDAMatrixbin(libname,DFV))
			{
				ERRORDIALOG("Cannot Write RDA Matrix","Error attempting to write the matrix.  Check the permissions on the RDA Matrix Binary Library, and retry.  Call your installer.",NULL,FALSE);
			}
			if(libname!=NULL) Rfree(libname);
			if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
			matrixdefaults=GetDefaults(MAINRSRC);
		}
	} else {
		ERRORDIALOG("Cannot Write RDA Matrix","Error RDA Matrix Structure not found and/or is NULL.  Call your installer.",NULL,FALSE);
	}
}
static void okmatrix(RDArsrc *matrixrsrc)
{
	int selected=0,selectedv=0;
	char *libname=NULL;

	if(FINDRSCGETINT(matrixrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(matrixrsrc,"MATRIX AVAILABLE",&selectedv)) return;
	if(matrixrsrc!=NULL) killwindow(matrixrsrc);
	if(DFV!=NULL) { FreeRDAMATRIX(DFV); DFV=NULL; }
	if(RDAstrcmp(matrixavl->libs[selectedv],"No Matrix Definitions"))
	{
		DFV=RDAMATRIXNEW(modlst->libs[selected],matrixavl->libs[selectedv]);
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.MTX",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
		getRDAMatrixbin(libname,DFV);
		if(libname!=NULL) Rfree(libname);
		if(DFV==NULL)
		{
			ERRORDIALOG("Cannot Read RDA Table","Error in permissions, the user cannot read the matrix binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			quitmatrixmenu(matrixrsrc);
			return;
		}
	}
	quitmatrixmenu(matrixrsrc);
	if(MAINRSRC!=NULL)
	{
		killwindow(MAINRSRC);
		free_rsrc(MAINRSRC);
	}
	if(DFV==NULL)
	{
		DFV=RDAMATRIXNEW(modlst->libs[selected],NULL);
	}
	MAINRSRC=GenerateMatrixRDArsrc();
	if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
	matrixdefaults=GetDefaults(MAINRSRC);
	APPmakescrn(MAINRSRC,TRUE,quiteditmatrix,NULL,TRUE);
}
static void newmatrix(RDArsrc *matrixrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(matrixrsrc,"MODULE LIST",&selected)) return;
	if(matrixrsrc!=NULL) killwindow(matrixrsrc);
	if(DFV!=NULL) { FreeRDAMATRIX(DFV); DFV=NULL; }
	DFV=RDAMATRIXNEW(modlst->libs[selected],NULL);
	quitmatrixmenu(matrixrsrc);
	if(MAINRSRC!=NULL)
	{
		killwindow(MAINRSRC);
		free_rsrc(MAINRSRC);
	}
	MAINRSRC=GenerateMatrixRDArsrc();
	if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
	matrixdefaults=GetDefaults(MAINRSRC);
	APPmakescrn(MAINRSRC,TRUE,quiteditmatrix,NULL,TRUE);
}
static void getmodulesavailable(RDArsrc *matrixrsrc)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETINT(matrixrsrc,"MODULE LIST",&selectedm)) return;
	if(modlst!=NULL) { freeapplib(modlst); modlst=NULL; }
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
/* Changed this to list only modules that have matricies in them like
   changes made for Virtual Fields */
/*
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x)
*/
	for(x=0;findfile(dirx,"*.MTX",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
		selectedm=0;
	} else {
		SORTAPPlib(modlst);
		if(selectedm>=modlst->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(!FINDRSCLISTAPPlib(matrixrsrc,"MODULE LIST",selectedm,modlst))
	{
		updatersrc(matrixrsrc,"MODULE LIST");
	}
	domodulelist(matrixrsrc);
}
static void deletematrix(RDArsrc *matrixrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulename=NULL;

	readallwidgets(matrixrsrc);
	if(FINDRSCGETINT(matrixrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(matrixrsrc,"MATRIX AVAILABLE",&s)) return;
	modulename=adddashes(modlst->libs[m]);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.MTX",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,modulename);
#endif
	if(deletelibbin(libx,matrixavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Virtual","Error attempting to delete matrix.  Check the permissions on the RDA Table Binary Library, and retry.  Call your installer.",NULL,FALSE);
	} else getmodulesavailable(matrixrsrc);
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
}
static void deletematrixtest(RDArsrc *matrixrsrc)
{
	int s=0;

	if(!RDAstrcmp(matrixavl->libs[s],"No Matrix Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE MATRIX","Are you sure you want to delete this matrix?",deletematrix,NULL,FALSE,matrixrsrc);
}
static void setvaluesx(RDArsrc *matrixrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL,*title1=NULL,*title2=NULL;
	char *libname=NULL;

	if(FINDRSCGETINT(matrixrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(matrixrsrc,"MATRIX AVAILABLE",&selected)) return;
	if(!RDAstrcmp(matrixavl->libs[selected],"No Matrix Definitions"))
	{
		temp=NULL;
	} else {
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selectedm])+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.MTX",CURRENTDIRECTORY,modlst->libs[selectedm]);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,modlst->libs[selectedm]);
#endif
		getRDAMatrixdesc(libname,matrixavl->libs[selected],&temp,&title1,&title2);
		if(libname!=NULL) Rfree(libname);
	}
	if(!FINDRSCSETSTRING(matrixrsrc,"DESCRIPTION",temp))
		updatersrc(matrixrsrc,"DESCRIPTION");
	if(!FINDRSCSETSTRING(matrixrsrc,"TITLE 1",title1))
		updatersrc(matrixrsrc,"TITLE 1");
	if(!FINDRSCSETSTRING(matrixrsrc,"TITLE 2",title2))
		updatersrc(matrixrsrc,"TITLE 2");
	if(temp!=NULL) Rfree(temp);
	if(title1!=NULL) Rfree(title1);
	if(title2!=NULL) Rfree(title2);
}
static void domodulelist(RDArsrc *matrixrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(matrixrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(matrixrsrc,"MATRIX AVAILABLE",&selectedf);
	if(matrixavl!=NULL) { freeapplib(matrixavl); matrixavl=NULL; }
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.MTX",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
	matrixavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(matrixavl==NULL)
	{
		matrixavl=APPlibNEW();
		addAPPlib(matrixavl,"No Matrix Definitions");
	}
	if(selectedf>=matrixavl->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(matrixrsrc,"MATRIX AVAILABLE",selectedf,matrixavl))
		updatersrc(matrixrsrc,"MATRIX AVAILABLE");
	setvaluesx(matrixrsrc);
}
static void print_matrixs(RDArsrc *prsrc,RDArsrc *m)
{
	RDA_PFILE *fp=NULL;
	int x=0,selected=0;
	char *tmpx=NULL,*libx=NULL,*odevice=NULL,print_style=TRUE;
	RDAMATRIX *d=NULL;
	APPlib *virtnames=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&odevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(odevice==NULL)
	{
		ERRORDIALOG("Output Device Failure","There was no device specified.",NULL,FALSE);
		return;
	}
	if(FINDRSCGETINT(m,"MODULE LIST",&selected)) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
	tmpx=adddashes(modlst->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.MTX",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,tmpx);
#endif
	virtnames=getlibnames(libx,FALSE);
	if(virtnames!=NULL)
	{
		fp=RDA_popen(odevice,"w");
		if(fp==NULL)
		{
			prterr("Error Couldn't open device.");
			if(virtnames!=NULL) freeapplib(virtnames);
			if(tmpx!=NULL) Rfree(tmpx);
			if(libx!=NULL) Rfree(libx);
			if(odevice!=NULL) Rfree(odevice);
			return;
		}
		SelectPrintType(odevice,print_style,fp);
		if(odevice!=NULL) Rfree(odevice);
		RDA_fprintf(fp,"MATRIX AVAILABLE\r\n");
		for(x=0;x<virtnames->numlibs;++x)
		{
			d=RDAMATRIXNEW(modlst->libs[selected],virtnames->libs[x]);
			getRDAMatrixbin(libx,d);
			if(d!=NULL)
			{
				RDA_fprintf(fp,"Module:%s Name:%s\r\n",d->module,d->name);
				RDA_fprintf(fp,"\tTitle #1:%s\r\n",(d->title1!=NULL ? d->title1:""));
				RDA_fprintf(fp,"\tTitle #2:%s\r\n",(d->title2!=NULL ? d->title2:""));
				RDA_fprintf(fp,"\tDescription:%s\r\n",(d->desc!=NULL ? d->desc:""));
				RDA_fprintf(fp,"\r\n");
				FreeRDAMATRIX(d);
				d=NULL;
			}
		}
		RDA_pclose(fp);
		if(virtnames!=NULL) freeapplib(virtnames);
	} else {
		prterr("Error No Matrix Definitions Available.");
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
}
static void quit_print_matrixs(RDArsrc *prsrc)
{
	free_rsrc(prsrc);
}
/***************************************************************************
***************************************************************************/
static void print_matrixscb(RDArsrc *mainrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,print_matrixs,mainrsrc);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_matrixs,MAINRSRC);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_matrixs(prsrc);
	}
}
static void quitmatrixmenu(RDArsrc *matrix)
{
	if(matrix!=NULL) free_rsrc(matrix);
	if(modlst!=NULL) freeapplib(modlst);
	if(matrixavl!=NULL) freeapplib(matrixavl);
}
/***************************************************************************
***************************************************************************/
static void matrix_menu(RDArsrc *mainrsrc)
{
	RDArsrc *matrixrsrc=NULL;
	int x=0;
	char *modx=NULL,*namex=NULL;

	matrixrsrc=RDArsrcNEW(module,"MATRIX MENU");
	readwidget(MAINRSRC,"MODULE");
	readwidget(MAINRSRC,"NAME");
	FINDRSCGETSTRING(MAINRSRC,"MODULE",&modx);
	FINDRSCGETSTRING(MAINRSRC,"NAME",&namex);
	x=0;
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	addlstrsrc(matrixrsrc,"MODULE LIST",&x,TRUE,domodulelist,
		modlst->numlibs,&modlst->libs,NULL);
	x=0;
	if(matrixavl!=NULL) { freeapplib(matrixavl); matrixavl=NULL; }
	matrixavl=APPlibNEW();
	addlstrsrc(matrixrsrc,"MATRIX AVAILABLE",&x,TRUE,setvaluesx,
		matrixavl->numlibs,&matrixavl->libs,NULL);
	addstdrsrc(matrixrsrc,"TITLE 1",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(matrixrsrc,"TITLE 2",VARIABLETEXT,0,NULL,FALSE);
	addsctrsrc(matrixrsrc,"DESCRIPTION",0,NULL,FALSE);
	getmodulesavailable(matrixrsrc);
	if(modx!=NULL)
	{
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(modlst->libs[x],modx)) break;
		}
		if(x<modlst->numlibs)
		{
			FINDRSCSETINT(matrixrsrc,"MODULE LIST",x);
		}
		Rfree(modx);
	}
	domodulelist(matrixrsrc);
	if(namex!=NULL)
	{
		for(x=0;x<matrixavl->numlibs;++x)
		{
			if(!RDAstrcmp(matrixavl->libs[x],namex)) break;
		}
		if(x<matrixavl->numlibs)
		{
			FINDRSCSETINT(matrixrsrc,"MATRIX AVAILABLE",x);
		}
		Rfree(namex);
	}
	addbtnrsrc(matrixrsrc,"ADD",TRUE,newmatrix,NULL);
	addbtnrsrc(matrixrsrc,"SELECT",TRUE,okmatrix,NULL);
	addbtnrsrc(matrixrsrc,"PRINT",TRUE,print_matrixscb,NULL);
	addbtnrsrc(matrixrsrc,"DELETE",TRUE,deletematrixtest,NULL);
	addrfkwrsrc(matrixrsrc,"QUIT",TRUE,quitmatrixmenu,NULL);
	addbtnrsrc(matrixrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(matrixrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(ADVmakescrn(matrixrsrc,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the MATRIX MENU.  Call your installer.",NULL,FALSE);
		quitmatrixmenu(matrixrsrc);
	}
}
static void quiteditmatrix(RDArsrc *mainrsrc)
{
	if(MAINRSRC!=NULL)
	{
		killwindow(MAINRSRC);
		free_rsrc(MAINRSRC);
	}
	if(modlst!=NULL) freeapplib(modlst);
	if(matrixavl!=NULL) freeapplib(matrixavl);
	if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
	if(DFV!=NULL) FreeRDAMATRIX(DFV);
	ShutdownSubsystems();
}
/***************************************************************************
***************************************************************************/
static void changeedittypes(RDArsrc *ersrc)
{
	int selected=0,adddel=FALSE;

	FINDRSCGETINT(ersrc,"EDIT TYPES",&selected);
	FINDRSCGETINT(ersrc,"ADD/DELETE",&adddel);
	if(!selected) /* columns */
	{
		FINDRSCSETEDITABLE(ersrc,"ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(ersrc,"ROW LIST",FALSE);
		if(DFV->yaxis.axis_type!=5)
		{
			FINDRSCSETEDITABLE(ersrc,"ROW VALUE",FALSE);
			FINDRSCSETSENSITIVE(ersrc,"ROW VALUE",FALSE);
		}
		FINDRSCSETSENSITIVE(ersrc,"COLUMN LIST",TRUE);
		FINDRSCSETEDITABLE(ersrc,"COLUMN LIST",TRUE);
		if(DFV->xaxis.axis_type!=5)
		{
			FINDRSCSETSENSITIVE(ersrc,"COLUMN VALUE",(adddel==0 ? TRUE:FALSE));
			FINDRSCSETEDITABLE(ersrc,"COLUMN VALUE",(adddel==0 ? TRUE:FALSE));
		}
	} else { /* rows */
		FINDRSCSETSENSITIVE(ersrc,"ROW LIST",TRUE);
		FINDRSCSETEDITABLE(ersrc,"ROW LIST",TRUE);
		if(DFV->yaxis.axis_type!=5)
		{
			FINDRSCSETSENSITIVE(ersrc,"ROW VALUE",(adddel==0 ? TRUE:FALSE));
			FINDRSCSETEDITABLE(ersrc,"ROW VALUE",(adddel==0 ? TRUE:FALSE));
		}
		FINDRSCSETEDITABLE(ersrc,"COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(ersrc,"COLUMN LIST",FALSE);
		if(DFV->xaxis.axis_type!=5)
		{
			FINDRSCSETEDITABLE(ersrc,"COLUMN VALUE",FALSE);
			FINDRSCSETSENSITIVE(ersrc,"COLUMN VALUE",FALSE);
		}
	}
}
static void quitedit(RDArsrc *ersrc)
{
	if(ersrc!=NULL) free_rsrc(ersrc);
	if(edittypes!=NULL) freeapplib(edittypes);
	if(adddels!=NULL) freeapplib(adddels);
	if(columnlist!=NULL) freeapplib(columnlist);
	if(rowlist!=NULL) freeapplib(rowlist);
	if(beforeafters!=NULL) freeapplib(beforeafters);
}
static void makespaceaxis(int pos,MATRIXAXIS *a)
{
	short x,y;
	AXISmem *temp=NULL,*temp1=NULL,*new_fields=NULL;

	++a->num;
	new_fields=Rmalloc(a->num*sizeof(AXISmem));
	y=0;
	for(x=0,temp=a->mem;x<(a->num-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->title1=NULL;
			temp1->title2=NULL;
			if(a->axis_type!=5)
			{
			switch(a->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					temp1->data.string_value=NULL;
					temp1->dlen=0;
					break;
				case BOOLNS:
				case CHARACTERS:
					temp1->data.string_value=Rmalloc(1);
					*temp1->data.string_value=0;
					temp1->dlen=1;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					temp1->data.float_value=Rmalloc(sizeof(double));
					*temp1->data.float_value=0;
					temp1->dlen=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					temp1->data.short_value=Rmalloc(sizeof(short));
					*temp1->data.short_value=0;
					temp1->dlen=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp1->data.integer_value=Rmalloc(sizeof(int));
					*temp1->data.integer_value=0;
					temp1->dlen=sizeof(int);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] on Table [%s] [%s].",a->type);
					break;
			}
			}
			++y;
			temp1=new_fields+y;
		}
		if(temp->title1!=NULL)
		{
			temp1->title1=stralloc(temp->title1);
			Rfree(temp->title1);
		} else temp1->title1=NULL;
		if(temp->title2!=NULL)
		{
			temp1->title2=stralloc(temp->title2);
			Rfree(temp->title2);
		} else temp1->title2=NULL;
		if(a->axis_type!=5)
		{
		switch(a->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=stralloc(temp->data.string_value);
					temp1->dlen=RDAstrlen(temp1->data.string_value)+1;
					Rfree(temp->data.string_value);
				} else {
					temp1->data.string_value=NULL;
					temp1->dlen=0;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
				temp1->data.string_value=Rmalloc(1);
				if(temp->data.string_value!=NULL)
				{
					*temp1->data.string_value=*temp->data.string_value;
					Rfree(temp->data.string_value);
				} else *temp1->data.string_value=0;
				temp1->dlen=1;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				temp1->data.float_value=Rmalloc(sizeof(double));
				if(temp->data.float_value!=NULL)
				{
					*temp1->data.float_value=*temp->data.float_value;
					Rfree(temp->data.float_value);
				} else  *temp1->data.float_value=0;
				temp1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				temp1->data.short_value=Rmalloc(sizeof(short));
				if(temp->data.short_value!=NULL)
				{
					*temp1->data.short_value=*temp->data.short_value;
					Rfree(temp->data.short_value);
				} else *temp1->data.short_value=0;
				temp1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				temp1->data.integer_value=Rmalloc(sizeof(int));
				if(temp->data.integer_value!=NULL)
				{
					*temp1->data.integer_value=*temp->data.integer_value;
					Rfree(temp->data.integer_value);
				} else *temp1->data.integer_value=0;
				temp1->dlen=sizeof(int);
				break;
			case BUTTONS:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
			default:
				prterr("Error field type [%d] on Table [%s] [%s].",a->type);
				break;
		}
		}
		++y;
	}
	Rfree(a->mem);
	a->mem=Rmalloc(a->num*sizeof(AXISmem));
	for(x=0,temp1=a->mem,temp=new_fields;x<a->num;++x,
		++temp,++temp1)
	{
		if(temp->title1!=NULL)
		{
			temp1->title1=stralloc(temp->title1);
			Rfree(temp->title1);
		} else temp1->title1=NULL;
		if(temp->title2!=NULL)
		{
			temp1->title2=stralloc(temp->title2);
			Rfree(temp->title2);
		} else temp1->title2=NULL;
		if(a->axis_type!=5)
		{
		switch(a->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=stralloc(temp->data.string_value);
					temp1->dlen=RDAstrlen(temp1->data.string_value)+1;
					Rfree(temp->data.string_value);
				} else {
					temp1->data.string_value=NULL;
					temp1->dlen=0;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
				temp1->data.string_value=Rmalloc(1);
				if(temp->data.string_value!=NULL)
				{
					*temp1->data.string_value=*temp->data.string_value;
					Rfree(temp->data.string_value);
				} else *temp1->data.string_value=0;
				temp1->dlen=1;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				temp1->data.float_value=Rmalloc(sizeof(double));
				if(temp->data.float_value!=NULL)
				{
					*temp1->data.float_value=*temp->data.float_value;
					Rfree(temp->data.float_value);
				} else  *temp1->data.float_value=0;
				temp1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				temp1->data.short_value=Rmalloc(sizeof(short));
				if(temp->data.short_value!=NULL)
				{
					*temp1->data.short_value=*temp->data.short_value;
					Rfree(temp->data.short_value);
				} else *temp1->data.short_value=0;
				temp1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				temp1->data.integer_value=Rmalloc(sizeof(int));
				if(temp->data.integer_value!=NULL)
				{
					*temp1->data.integer_value=*temp->data.integer_value;
					Rfree(temp->data.integer_value);
				} else *temp1->data.integer_value=0;
				temp1->dlen=sizeof(int);
				break;
			case BUTTONS:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
			default:
				prterr("Error field type [%d] on Table [%s] [%s].",a->type);
				break;
		}
		}
	}
	Rfree(new_fields);
}
static void deletespaceaxis(int pos,MATRIXAXIS *a)
{
	short x,y;
	AXISmem *temp=NULL,*temp1=NULL,*new_fields=NULL;

	if(a->num<2) return;
	new_fields=Rmalloc((a->num-1)*sizeof(AXISmem));
	y=0;
	for(x=0,temp=a->mem;x<a->num;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=pos)
		{
			if(temp->title1!=NULL)
			{
				temp1->title1=stralloc(temp->title1);
				Rfree(temp->title1);
			} else temp1->title1=NULL;
			if(temp->title2!=NULL)
			{
				temp1->title2=stralloc(temp->title2);
				Rfree(temp->title2);
			} else temp1->title2=NULL;
			if(a->axis_type!=5)
			{
			switch(a->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					if(temp->data.string_value!=NULL)
					{
						temp1->data.string_value=stralloc(temp->data.string_value);
						temp1->dlen=RDAstrlen(temp1->data.string_value)+1;
						Rfree(temp->data.string_value);
					} else {
						temp1->data.string_value=NULL;
						temp1->dlen=0;
					}
					break;
				case BOOLNS:
				case CHARACTERS:
					temp1->data.string_value=Rmalloc(1);
					if(temp->data.string_value!=NULL)
					{
						*temp1->data.string_value=*temp->data.string_value;
						Rfree(temp->data.string_value);
					} else *temp1->data.string_value=0;
					temp1->dlen=1;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					temp1->data.float_value=Rmalloc(sizeof(double));
					if(temp->data.float_value!=NULL)
					{
						*temp1->data.float_value=*temp->data.float_value;
						Rfree(temp->data.float_value);
					} else  *temp1->data.float_value=0;
					temp1->dlen=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					temp1->data.short_value=Rmalloc(sizeof(short));
					if(temp->data.short_value!=NULL)
					{
						*temp1->data.short_value=*temp->data.short_value;
						Rfree(temp->data.short_value);
					} else *temp1->data.short_value=0;
					temp1->dlen=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp1->data.integer_value=Rmalloc(sizeof(int));
					if(temp->data.integer_value!=NULL)
					{
						*temp1->data.integer_value=*temp->data.integer_value;
						Rfree(temp->data.integer_value);
					} else *temp1->data.integer_value=0;
					temp1->dlen=sizeof(int);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] on Table [%s] [%s].",a->type);
					break;
			}
			}
			++y;
		} else {
			if(temp->title1!=NULL) Rfree(temp->title1);
			if(temp->title2!=NULL) Rfree(temp->title2);
			if(a->axis_type!=5)
			{
			switch(a->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					if(temp->data.string_value!=NULL)
						Rfree(temp->data.string_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					if(temp->data.string_value!=NULL)
						Rfree(temp->data.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(temp->data.float_value!=NULL)
						Rfree(temp->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					if(temp->data.short_value!=NULL)
						Rfree(temp->data.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(temp->data.integer_value!=NULL)
						Rfree(temp->data.integer_value);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] on Table [%s] [%s].",a->type);
					break;
			}
			}
		}
	}
	Rfree(a->mem);
	--a->num;
	a->mem=Rmalloc(a->num*sizeof(AXISmem));
	for(x=0,temp1=a->mem,temp=new_fields;x<a->num;++x,
		++temp,++temp1)
	{
		if(temp->title1!=NULL)
		{
			temp1->title1=stralloc(temp->title1);
			Rfree(temp->title1);
		} else temp1->title1=NULL;
		if(temp->title2!=NULL)
		{
			temp1->title2=stralloc(temp->title2);
			Rfree(temp->title2);
		} else temp1->title2=NULL;
		if(a->axis_type!=5)
		{
		switch(a->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=stralloc(temp->data.string_value);
					temp1->dlen=RDAstrlen(temp1->data.string_value)+1;
					Rfree(temp->data.string_value);
				} else {
					temp1->data.string_value=NULL;
					temp1->dlen=0;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
				temp1->data.string_value=Rmalloc(1);
				if(temp->data.string_value!=NULL)
				{
					*temp1->data.string_value=*temp->data.string_value;
					Rfree(temp->data.string_value);
				} else *temp1->data.string_value=0;
				temp1->dlen=1;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				temp1->data.float_value=Rmalloc(sizeof(double));
				if(temp->data.float_value!=NULL)
				{
					*temp1->data.float_value=*temp->data.float_value;
					Rfree(temp->data.float_value);
				} else  *temp1->data.float_value=0;
				temp1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				temp1->data.short_value=Rmalloc(sizeof(short));
				if(temp->data.short_value!=NULL)
				{
					*temp1->data.short_value=*temp->data.short_value;
					Rfree(temp->data.short_value);
				} else *temp1->data.short_value=0;
				temp1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				temp1->data.integer_value=Rmalloc(sizeof(int));
				if(temp->data.integer_value!=NULL)
				{
					*temp1->data.integer_value=*temp->data.integer_value;
					Rfree(temp->data.integer_value);
				} else *temp1->data.integer_value=0;
				temp1->dlen=sizeof(int);
				break;
			case BUTTONS:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
			default:
				prterr("Error field type [%d] on Table [%s] [%s].",a->type);
				break;
		}
		}
	}
	Rfree(new_fields);
}
static void selectedit(RDArsrc *ersrc)
{
	int edittype=0,op=0,wh=0,clist=0,rlist=0;
	int pos=0;
	AXISmem *am;

	readallwidgets(ersrc);
	readmatrix();
	FINDRSCGETINT(ersrc,"EDIT TYPES",&edittype);
	FINDRSCGETINT(ersrc,"ADD/DELETE",&op);
	FINDRSCGETINT(ersrc,"BEFORE/AFTER",&wh);
	FINDRSCGETINT(ersrc,"COLUMN LIST",&clist);
	FINDRSCGETINT(ersrc,"ROW LIST",&rlist);
	if(ersrc!=NULL) killwindow(ersrc);
	if(!op) /* ADD */
	{
		if(DFV->yaxis.mem!=NULL && edittype==1)
		{
			if(!wh) pos=rlist+1;
				else pos=rlist;
			makespaceaxis(pos,&DFV->yaxis);
			am=DFV->yaxis.mem+pos;
			FINDRSCGETSTRING(ersrc,"TITLE 1",&am->title1);
			FINDRSCGETSTRING(ersrc,"TITLE 2",&am->title2);
			if(DFV->yaxis.axis_type!=5)
			{
				switch(DFV->yaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						FINDRSCGETSTRING(ersrc,"ROW VALUE",&am->data.string_value);
						if(am->data.string_value!=NULL)
						{
							am->dlen=RDAstrlen(am->data.string_value)+1;
						} else am->dlen=0;
						break;
					case BOOLNS:
					case CHARACTERS:
						FINDRSCGETCHAR(ersrc,"ROW VALUE",am->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						FINDRSCGETDOUBLE(ersrc,"ROW VALUE",am->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						FINDRSCGETSHORT(ersrc,"ROW VALUE",am->data.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						FINDRSCGETINT(ersrc,"ROW VALUE",am->data.integer_value);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] on Table [%s] [%s].",DFV->yaxis.type);
						break;
				}
			}
			MakeSpaceMatrixRow(pos);
		} else if(DFV->recs==NULL)
		{
			pos=0;
			DFV->yaxis.mem=Rmalloc(sizeof(AXISmem));
			am=DFV->yaxis.mem;
			if(edittype==1)
			{
				am->title1=NULL;
				am->title2=NULL;
			} else {
				am->title1=NULL;
				FINDRSCGETSTRING(ersrc,"TITLE 1",&am->title1);
				am->title2=NULL;
				FINDRSCGETSTRING(ersrc,"TITLE 2",&am->title2);
			}
			if(DFV->yaxis.axis_type!=5)
			{
				switch(DFV->yaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						am->data.string_value=NULL;
						am->dlen=0;
						if(edittype==1)
						{
							FINDRSCGETSTRING(ersrc,"ROW VALUE",&am->data.string_value);
							if(am->data.string_value!=NULL)
							{
								am->dlen=RDAstrlen(am->data.string_value)+1;
							} else am->dlen=0;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						am->data.string_value=Rmalloc(1);
						*am->data.string_value=0;
						am->dlen=1;
						if(edittype==1)
						{
							FINDRSCGETCHAR(ersrc,"ROW VALUE",am->data.string_value);
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						am->data.float_value=Rmalloc(sizeof(double));
						*am->data.float_value=0;
						am->dlen=sizeof(double);
						if(edittype==1)
						{
							FINDRSCGETDOUBLE(ersrc,"ROW VALUE",am->data.float_value);
						}
						break;
					case SHORTV:
					case SSHORTV:
						am->data.short_value=Rmalloc(sizeof(short));
						*am->data.short_value=0;
						am->dlen=sizeof(short);
						if(edittype==1)
						{
							FINDRSCGETSHORT(ersrc,"ROW VALUE",am->data.short_value);
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						am->data.integer_value=Rmalloc(sizeof(int));
						*am->data.integer_value=0;
						am->dlen=sizeof(int);
						if(edittype==1)
						{
							FINDRSCGETINT(ersrc,"ROW VALUE",am->data.integer_value);
						}
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid [%d] on Table [%s] [%s].",DFV->yaxis.type,DFV->module,DFV->name);
						break;
				}
			}
			DFV->yaxis.num=1;
			pos=0;
			MakeSpaceMatrixRow(pos);
		}
		if(DFV->xaxis.mem!=NULL && edittype==0)
		{
			if(!wh) pos=clist+1;
				else pos=clist;
			makespaceaxis(pos,&DFV->xaxis);
			am=DFV->xaxis.mem+pos;
			FINDRSCGETSTRING(ersrc,"TITLE 1",&am->title1);
			FINDRSCGETSTRING(ersrc,"TITLE 2",&am->title2);
			if(DFV->xaxis.axis_type!=5)
			{
				switch(DFV->xaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						FINDRSCGETSTRING(ersrc,"COLUMN VALUE",&am->data.string_value);
						if(am->data.string_value!=NULL)
						{
							am->dlen=RDAstrlen(am->data.string_value)+1;
						} else am->dlen=0;
						break;
					case BOOLNS:
					case CHARACTERS:
						FINDRSCGETCHAR(ersrc,"COLUMN VALUE",am->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						FINDRSCGETDOUBLE(ersrc,"COLUMN VALUE",am->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						FINDRSCGETSHORT(ersrc,"COLUMN VALUE",am->data.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						FINDRSCGETINT(ersrc," VALUE",am->data.integer_value);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] on Table [%s] [%s].",DFV->xaxis.type);
						break;
				}
			} else {
				am->data.string_value=NULL;
				am->data.float_value=NULL;
				am->data.short_value=NULL;
				am->data.integer_value=NULL;
			}
			MakeSpaceMatrixColumn(pos);
		} else if(edittype==0)
		{
			DFV->xaxis.num=1;
			DFV->xaxis.mem=Rmalloc(sizeof(AXISmem));
			am=DFV->xaxis.mem;
			am->title1=NULL;
			FINDRSCGETSTRING(ersrc,"TITLE 1",&am->title1);
			am->title2=NULL;
			FINDRSCGETSTRING(ersrc,"TITLE 2",&am->title2);
			if(DFV->xaxis.axis_type!=5)
			{
				switch(DFV->xaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						am->data.string_value=NULL;
						am->dlen=0;
						FINDRSCGETSTRING(ersrc,"COLUMN VALUE",&am->data.string_value);
						if(am->data.string_value!=NULL)
						{
							am->dlen=RDAstrlen(am->data.string_value)+1;
						} else am->dlen=0;
						break;
					case BOOLNS:
					case CHARACTERS:
						am->data.string_value=Rmalloc(1);
						*am->data.string_value=0;
						am->dlen=1;
						FINDRSCGETCHAR(ersrc,"COLUMN VALUE",am->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						am->data.float_value=Rmalloc(sizeof(double));
						*am->data.float_value=0;
						am->dlen=sizeof(double);
						FINDRSCGETDOUBLE(ersrc,"COLUMN VALUE",am->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						am->data.short_value=Rmalloc(sizeof(short));
						*am->data.short_value=0;
						am->dlen=sizeof(short);
						FINDRSCGETSHORT(ersrc,"COLUMN VALUE",am->data.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						am->data.integer_value=Rmalloc(sizeof(int));
						*am->data.integer_value=0;
						am->dlen=sizeof(int);
						FINDRSCGETINT(ersrc,"COLUMN VALUE",am->data.integer_value);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid [%d] on Table [%s] [%s].",DFV->xaxis.type,DFV->module,DFV->name);
						break;
				}
			} else {
				am->data.string_value=NULL;
				am->data.float_value=NULL;
				am->data.short_value=NULL;
				am->data.integer_value=NULL;
			}
			DFV->xaxis.num=1;
			pos=0;
			MakeSpaceMatrixColumn(pos);
		}
	} else { /* DELETE */
		if(edittype==0) /* Column */
		{
			if(DFV->xaxis.mem!=NULL && DFV->xaxis.num>1)
			{
				deletespaceaxis(clist,&DFV->xaxis);
			} else if(DFV->xaxis.mem!=NULL)
			{
				am=DFV->xaxis.mem;
				if(am->title1!=NULL) Rfree(am->title1);
				if(am->title2!=NULL) Rfree(am->title2);
				switch(DFV->xaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
					case BOOLNS:
					case CHARACTERS:
						if(am->data.string_value!=NULL)
							Rfree(am->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(am->data.float_value!=NULL)
							Rfree(am->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(am->data.short_value!=NULL)
							Rfree(am->data.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(am->data.integer_value!=NULL)
							Rfree(am->data.integer_value);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid [%d] on Table [%s] [%s].",DFV->xaxis.type,DFV->module,DFV->name);
						break;
				}
				Rfree(DFV->xaxis.mem);
				DFV->xaxis.mem=NULL;
				DFV->xaxis.num=0;
			}
			DeleteSpaceMatrixColumn(clist);
		} else { /* Row */
			if(DFV->yaxis.mem!=NULL && DFV->yaxis.num>1)
			{
				deletespaceaxis(rlist,&DFV->yaxis);
			} else if(DFV->yaxis.mem!=NULL)
			{
				am=DFV->yaxis.mem;
				if(am->title1!=NULL) Rfree(am->title1);
				if(am->title2!=NULL) Rfree(am->title2);
				switch(DFV->yaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
					case BOOLNS:
					case CHARACTERS:
						if(am->data.string_value!=NULL)
							Rfree(am->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(am->data.float_value!=NULL)
							Rfree(am->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(am->data.short_value!=NULL)
							Rfree(am->data.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(am->data.integer_value!=NULL)
							Rfree(am->data.integer_value);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid [%d] on Table [%s] [%s].",DFV->yaxis.type,DFV->module,DFV->name);
						break;
				}
				Rfree(DFV->yaxis.mem);
				DFV->yaxis.mem=NULL;
				DFV->yaxis.num=0;
			}
			DeleteSpaceMatrixRow(rlist);
		}
	}
	if(MAINRSRC!=NULL)
	{
		killwindow(MAINRSRC);
		free_rsrc(MAINRSRC);
	}
	quitedit(ersrc);
	MAINRSRC=GenerateMatrixRDArsrc();
	if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
	matrixdefaults=GetDefaults(MAINRSRC);
	APPmakescrn(MAINRSRC,TRUE,quiteditmatrix,NULL,TRUE);
}
void CreateColumnList()
{
	AXISmem *am;
	int y=0;
	char *temp=NULL,*amtstring=NULL;

	if(columnlist!=NULL) freeapplib(columnlist);
	columnlist=APPlibNEW();
	if(DFV->xaxis.mem!=NULL)
	{
		for(y=0,am=DFV->xaxis.mem;y<DFV->xaxis.num;++y,++am)
		{
			if(DFV->xaxis.axis_type==5)
			{
				temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(am->title2)+20);
				sprintf(temp,"#%d:[%s] [%s]",y,(am->title1!=NULL ?
					am->title1:""),(am->title2!=NULL ? am->title2:""));
			} else {
				switch(DFV->xaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(am->data.string_value)+30);
							sprintf(temp,"#%d:[%s][%s]",y,
								(am->data.string_value!=NULL
								? am->data.string_value:""),
								(am->title1!=NULL ?
								am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(am->data.string_value)+30);
							sprintf(temp,"#%d:[%s]",y,
								(am->data.string_value!=NULL ?
								am->data.string_value:""));
						}
						break;
					case BOOLNS:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%s][%s]",y,
								(*am->data.string_value ? "True":"False"),
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(30);
							sprintf(temp,"#%d:[%s]",y,
								(*am->data.string_value ? "True":"False"));
						}
						break;
					case CHARACTERS:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%c][%s]",y,
								*am->data.string_value,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(30);
							sprintf(temp,"#%d:[%c]",y,
								*am->data.string_value);
						}
						break;
					case DOLLARS:
						amtstring=famt(*am->data.float_value,DFV->xaxis.len-1);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case DOLLARS_NOCENTS:
						amtstring=famtncents((*am->data.float_value*100),
							(DFV->xaxis.len-1),' ');
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case DECIMALV:
						amtstring=ufloatamt(*am->data.float_value,DFV->xaxis.len);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case DOUBLEV:
						amtstring=Rmalloc(DFV->xaxis.len+1);
						sprintf(amtstring,"%*.0f",DFV->xaxis.len,
							*am->data.float_value);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						break;
					case SDOUBLEV:
						amtstring=floatamt(*am->data.float_value,DFV->xaxis.len-1);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case SDECIMALV:
						amtstring=floatamt(*am->data.float_value,DFV->xaxis.len-1);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case SHORTV:
					case SSHORTV:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%d][%s]",y,
								*am->data.short_value,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(30);
							sprintf(temp,"#%d:[%d]",y,*am->data.short_value);
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%d][%s]",y,
								*am->data.integer_value,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(am->data.string_value)+30);
							sprintf(temp,"#%d:[%d]",y,*am->data.integer_value);
						}
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for Axis Member [%d] on Table [%s] [%s].",DFV->xaxis.type,y,DFV->module,DFV->name);
						break;
				}
			}
			addAPPlib(columnlist,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
	if(columnlist->numlibs<1)
	{
		addAPPlib(columnlist,"Not Available");
	}
}
void CreateRowList()
{
	AXISmem *am;
	int y=0;
	char *temp=NULL,*amtstring=NULL;

	if(rowlist!=NULL) freeapplib(rowlist);
	rowlist=APPlibNEW();
	if(DFV->yaxis.mem!=NULL)
	{
		for(y=0,am=DFV->yaxis.mem;y<DFV->yaxis.num;++y,++am)
		{
			if(DFV->yaxis.axis_type==5)
			{
				temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(am->title2)+20);
				sprintf(temp,"#%d:[%s] [%s]",y,(am->title1!=NULL ?
					am->title1:""),(am->title2!=NULL ? am->title2:""));
			} else {
				switch(DFV->yaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(am->data.string_value)+30);
							sprintf(temp,"#%d:[%s][%s]",y,
								(am->data.string_value!=NULL
								? am->data.string_value:""),
								(am->title1!=NULL ?
								am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(am->data.string_value)+30);
							sprintf(temp,"#%d:[%s]",y,
								(am->data.string_value!=NULL ?
								am->data.string_value:""));
						}
						break;
					case BOOLNS:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%s][%s]",y,
								(*am->data.string_value ? "True":"False"),
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(30);
							sprintf(temp,"#%d:[%s]",y,
								(*am->data.string_value ? "True":"False"));
						}
						break;
					case CHARACTERS:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%c][%s]",y,
								*am->data.string_value,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(30);
							sprintf(temp,"#%d:[%c]",y,
								*am->data.string_value);
						}
						break;
					case DOLLARS:
						amtstring=famt(*am->data.float_value,DFV->xaxis.len-1);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case DOLLARS_NOCENTS:
						amtstring=famtncents((*am->data.float_value*100),
							(DFV->xaxis.len-1),' ');
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case DECIMALV:
						amtstring=ufloatamt(*am->data.float_value,DFV->xaxis.len);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case DOUBLEV:
						amtstring=Rmalloc(DFV->xaxis.len+1);
						sprintf(amtstring,"%*.0f",DFV->xaxis.len,
							*am->data.float_value);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						break;
					case SDOUBLEV:
						amtstring=floatamt(*am->data.float_value,DFV->xaxis.len-1);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case SDECIMALV:
						amtstring=floatamt(*am->data.float_value,DFV->xaxis.len-1);
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s][%s]",y,amtstring,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(amtstring)+20);
							sprintf(temp,"#%d:[%s]",y,amtstring);
						}
						if(amtstring!=NULL) Rfree(amtstring);
						break;
					case SHORTV:
					case SSHORTV:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%d][%s]",y,
								*am->data.short_value,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(30);
							sprintf(temp,"#%d:[%d]",y,*am->data.short_value);
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(!isEMPTY(am->title1))
						{
							temp=Rmalloc(RDAstrlen(am->title1)+30);
							sprintf(temp,"#%d:[%d][%s]",y,
								*am->data.integer_value,
								(am->title1!=NULL ? am->title1:""));
						} else {
							temp=Rmalloc(RDAstrlen(am->data.string_value)+30);
							sprintf(temp,"#%d:[%d]",y,*am->data.integer_value);
						}
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for Axis Member [%d] on Table [%s] [%s].",DFV->yaxis.type,y,DFV->module,DFV->name);
						break;
				}
			}
			addAPPlib(rowlist,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
	if(rowlist->numlibs<1)
	{
		addAPPlib(rowlist,"Not Available");
	}
}
static void changeadddelete(RDArsrc *ersrc)
{
	int selected=0;

	FINDRSCGETINT(ersrc,"ADD/DELETE",&selected);
	if(!selected)
	{
		FINDRSCSETSENSITIVE(ersrc,"BEFORE/AFTER",TRUE);
		FINDRSCSETEDITABLE(ersrc,"BEFORE/AFTER",TRUE);
	} else {
		FINDRSCSETEDITABLE(ersrc,"BEFORE/AFTER",FALSE);
		FINDRSCSETSENSITIVE(ersrc,"BEFORE/AFTER",FALSE);
	}
}
/***************************************************************************
Edit Menu to Edit the Matrix's Rows and Columns
***************************************************************************/
static void edit_menu(RDArsrc *mainrsrc)
{
	RDArsrc *ersrc=NULL;
	int x=0;

	readmatrix();
	ersrc=RDArsrcNEW(module,"EDIT MATRIX MENU");
	if(edittypes!=NULL) freeapplib(edittypes);
	edittypes=APPlibNEW();
	addAPPlib(edittypes,"Columns");
	addAPPlib(edittypes,"Rows");
	x=0;
	addlstrsrc(ersrc,"EDIT TYPES",&x,TRUE,changeedittypes,edittypes->numlibs,
		&edittypes->libs,NULL);
	if(adddels!=NULL) freeapplib(adddels);
	adddels=APPlibNEW();
	addAPPlib(adddels,"Add");
	addAPPlib(adddels,"Delete");
	x=0;
	addlstrsrc(ersrc,"ADD/DELETE",&x,TRUE,changeadddelete,adddels->numlibs,
		&adddels->libs,NULL);
	if(beforeafters!=NULL) freeapplib(beforeafters);
	beforeafters=APPlibNEW();
	addAPPlib(beforeafters,"After");
	addAPPlib(beforeafters,"Before");
	x=0;
	addlstrsrc(ersrc,"BEFORE/AFTER",&x,TRUE,NULL,beforeafters->numlibs,
		&beforeafters->libs,NULL);
	CreateColumnList();
	x=(columnlist->numlibs-1);
	addlstrsrc(ersrc,"COLUMN LIST",&x,TRUE,NULL,columnlist->numlibs,
		&columnlist->libs,NULL);
	CreateRowList();
	x=(rowlist->numlibs-1);
	addlstrsrc(ersrc,"ROW LIST",&x,TRUE,NULL,rowlist->numlibs,&rowlist->libs,
		NULL);
	addstdrsrc(ersrc,"TITLE 1",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(ersrc,"TITLE 2",VARIABLETEXT,0,NULL,TRUE);
	addsctrsrc(ersrc,"DESCRIPTION",0,NULL,TRUE);
	if(DFV->xaxis.axis_type!=5)
	{
		switch(DFV->xaxis.type)
		{
			case SCROLLEDTEXT:
				addsctrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.len,NULL,TRUE);
				break;
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				addstdrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.type,
					DFV->xaxis.len,NULL,TRUE);
				break;
			case CHARACTERS:
				addstdrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.type,
					DFV->xaxis.len,NULL,TRUE);
				break;
			case DOLLARS:
				addstdrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.type,
					DFV->xaxis.len,NULL,TRUE);
				break;
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				addstdrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.type,
					DFV->xaxis.len,NULL,TRUE);
				break;
			case SHORTV:
			case SSHORTV:
				addstdrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.type,
					DFV->xaxis.len,NULL,TRUE);
				break;
			case LONGV:
			case SLONGV:
					addstdrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.type,
					DFV->xaxis.len,NULL,TRUE);
				break;
			case BOOLNS:
				addstdrsrc(ersrc,"COLUMN VALUE",DFV->xaxis.type,
					DFV->xaxis.len,NULL,TRUE);
				break;
			case SCROLLEDLIST:
			case BUTTONS:
			default:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
		}
	}
	if(DFV->yaxis.axis_type!=5)
	{
		switch(DFV->yaxis.type)
		{
			case SCROLLEDTEXT:
				addsctrsrc(ersrc,"ROW VALUE",DFV->yaxis.len,NULL,TRUE);
				break;
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				addstdrsrc(ersrc,"ROW VALUE",DFV->yaxis.type,
					DFV->yaxis.len,NULL,TRUE);
				break;
			case CHARACTERS:
				addstdrsrc(ersrc,"ROW VALUE",DFV->yaxis.type,
					DFV->yaxis.len,NULL,TRUE);
				break;
			case DOLLARS:
				addstdrsrc(ersrc,"ROW VALUE",DFV->yaxis.type,
					DFV->yaxis.len,NULL,TRUE);
				break;
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				addstdrsrc(ersrc,"ROW VALUE",DFV->yaxis.type,
					DFV->yaxis.len,NULL,TRUE);
				break;
			case SHORTV:
			case SSHORTV:
				addstdrsrc(ersrc,"ROW VALUE",DFV->yaxis.type,
					DFV->yaxis.len,NULL,TRUE);
				break;
			case LONGV:
			case SLONGV:
				addstdrsrc(ersrc,"ROW VALUE",DFV->yaxis.type,
					DFV->yaxis.len,NULL,TRUE);
				break;
			case BOOLNS:
				addstdrsrc(ersrc,"ROW VALUE",DFV->yaxis.type,
					DFV->yaxis.len,NULL,TRUE);
				break;
			case SCROLLEDLIST:
			case BUTTONS:
			default:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
		}
	}
	addbtnrsrc(ersrc,"SELECT",TRUE,selectedit,NULL);
	addrfkwrsrc(ersrc,"QUIT",TRUE,quitedit,NULL);
	addbtnrsrc(ersrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ersrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changeedittypes(ersrc);
	changeadddelete(ersrc);
/* CREATE EDIT MATRIX MENU RDAscn * BELOW */
	ersrc->scn=RDAscrnNEW(ersrc->module,ersrc->screen);
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Edit Types:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,7,"EDIT TYPES","Edit Types",NULL,NULL,1,0,0,NULL,
		NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Operation:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,7,"ADD/DELETE","Add/Delete",NULL,NULL,1,0,0,NULL,
		NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Where:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,7,"BEFORE/AFTER","Before/After",NULL,NULL,1,0,0,NULL,
		NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Column List:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,7,"COLUMN LIST","Column List",NULL,NULL,1,0,0,NULL,
		NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Row List:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,7,"ROW LIST","Row List",NULL,NULL,1,0,0,NULL,
		NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(DFV->xaxis.axis_type!=5)
	{
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	switch(DFV->xaxis.type)
	{
		case SCROLLEDTEXT:
			addwdgt(ersrc->scn,5,NULL,"Column Value:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,8,"COLUMN VALUE","Column Value",NULL,NULL,5,80,0,NULL,NULL,NULL);
			break;
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case ZIPCODE:
			addwdgt(ersrc->scn,5,NULL,"Column Value:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,0,"COLUMN VALUE","Column Value",NULL,NULL,0,DFV->xaxis.len,0,NULL,NULL,NULL);
			break;
		case CHARACTERS:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
		case SHORTV:
		case SSHORTV:
		case LONGV:
		case SLONGV:
			addwdgt(ersrc->scn,5,NULL,"Column Value:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,0,"COLUMN VALUE","Column Value",NULL,NULL,0,DFV->xaxis.len,0,NULL,NULL,NULL);
			break;
		case BOOLNS:
			addwdgt(ersrc->scn,9,"COLUMN VALUE","Column Value",NULL,NULL,0,DFV->xaxis.len,0,NULL,NULL,NULL);
			break;
		case SCROLLEDLIST:
		case BUTTONS:
		default:
			prterr("Error call RaddogTiddog cause this is weird....");
			break;
	}
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if(DFV->yaxis.axis_type!=5)
	{
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	switch(DFV->yaxis.type)
	{
		case SCROLLEDTEXT:
			addwdgt(ersrc->scn,5,NULL,"Row Value:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,14,"ROW VALUE","Row Value",NULL,NULL,5,80,0,NULL,NULL,NULL);
			break;
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case ZIPCODE:
			addwdgt(ersrc->scn,5,NULL,"Row Value:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,DFV->yaxis.type,"ROW VALUE","Row Value",NULL,NULL,0,DFV->yaxis.len,0,NULL,NULL,NULL);
			break;
		case CHARACTERS:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
		case SHORTV:
		case SSHORTV:
		case LONGV:
		case SLONGV:
			addwdgt(ersrc->scn,5,NULL,"Row Value:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(ersrc->scn,DFV->yaxis.type,"ROW VALUE","Row Value",NULL,NULL,0,DFV->yaxis.len,0,NULL,NULL,NULL);
			break;
		case BOOLNS:
			addwdgt(ersrc->scn,BOOLNS,"ROW VALUE","Row Value",NULL,NULL,0,DFV->yaxis.len,0,NULL,NULL,NULL);
			break;
		case SCROLLEDLIST:
		case BUTTONS:
		default:
			prterr("Error call RaddogTiddog cause this is weird....");
			break;
	}
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Title 1:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,0,"TITLE 1","Title 1",NULL,NULL,0,60,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Title 2:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,0,"TITLE 2","Title 2",NULL,NULL,0,60,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,5,NULL,"Description:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,8,"DESCRIPTION","Description",NULL,NULL,5,80,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,6,"SELECT","Select",NULL,NULL,0,0,24,NULL,NULL,NULL);
	addwdgt(ersrc->scn,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(ersrc->scn,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(ersrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* CREATE EDIT MATRIX MENU RDAscn * ABOVE */
	if(ADVmakescrn(ersrc,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the EDIT MATRIX MENU.  Call your installer.",NULL,FALSE);
		quitedit(ersrc);
	}
}
static void quitsetup(RDArsrc *srsrc)
{
	MATRIXcell *c;
	int x;

	if(srsrc!=NULL) free_rsrc(srsrc);
	if(axistypes!=NULL) freeapplib(axistypes);
	if(fieldtypes!=NULL) freeapplib(fieldtypes);
	if(computetypes!=NULL) freeapplib(computetypes);
	if(HoldCells!=NULL)
	{
		for(x=0,c=HoldCells;x<numHoldCells;++x,++c)
		{
			if(c->name!=NULL) Rfree(c->name);
		}
		Rfree(HoldCells);
		HoldCells=NULL;
		numHoldCells=0;
	}
}
static void ConvertMATRIXmem(MATRIXmem **mems)
{
	MATRIXmem *mem=NULL;
	MATRIXvalue *v1=NULL,*v2=NULL,*hold=NULL;
	MATRIXcell *c,*o=NULL;
	short holdtype=0;
	unsigned holdlen=0;
	int x,num=0;

	mem=*mems;
	if(mem->values!=NULL)
	{
		hold=mem->values;
		num=mem->num;
		mem->values=NULL;
		mem->num=0;
		if(HoldCells!=NULL)
		{
			mem->values=Rmalloc(numHoldCells*sizeof(MATRIXvalue));
			mem->num=numHoldCells;
			for(x=0,v1=mem->values,c=HoldCells;x<numHoldCells;++x,++v1,++c)
			{
				if(x<DFV->numdefs && x<num)
				{
					v2=hold+x;
					o=DFV->defs+x;
					holdtype=o->type;
					holdlen=o->len;
					switch(o->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							v1->data.string_value=NULL;
							v1->dlen=0;
							break;
						case BOOLNS:
						case CHARACTERS:
							v1->data.string_value=NULL;
							v1->dlen=0;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							v1->data.float_value=NULL;
							v1->dlen=0;
							break;
						case SHORTV:
						case SSHORTV:
							v1->data.short_value=NULL;
							v1->dlen=0;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							v1->data.integer_value=NULL;
							v1->dlen=0;
							break;
						case BUTTONS:
							prterr("Error call RaddogTiddog cause this is weird....");
							break;
						default:
							prterr("Error field type [%d] is invalid.",o->type);
							break;
					}
					COPYCONVERTFIELDTYPE(&holdtype,&v1->data,&holdlen,
						&v1->dlen,c->type,&v2->data,c->len);
				} else {
					switch(c->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							v1->data.string_value=NULL;
							v1->dlen=0;
							break;
						case BOOLNS:
						case CHARACTERS:
							v1->data.string_value=Rmalloc(1);
							*v1->data.string_value=0;
							v1->dlen=1;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							v1->data.float_value=Rmalloc(sizeof(double));
							*v1->data.float_value=0;
							v1->dlen=sizeof(double);
							break;
						case SHORTV:
						case SSHORTV:
							v1->data.short_value=Rmalloc(sizeof(short));
							*v1->data.short_value=0;
							v1->dlen=sizeof(short);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							v1->data.integer_value=Rmalloc(sizeof(int));
							*v1->data.integer_value=0;
							v1->dlen=sizeof(int);
							break;
						case BUTTONS:
							prterr("Error call RaddogTiddog cause this is weird....");
							break;
						default:
							prterr("Error field type [%d] is invalid.",c->type);
							break;
					}
				}
			}
		} else {
			mem->values=NULL;
			mem->num=0;
		}
		if(hold!=NULL)
		{
			for(x=0,v1=hold,c=DFV->defs;x<num;++x,++v1,++c)
			{
				switch(c->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
					case BOOLNS:
					case CHARACTERS:
						if(v1->data.string_value!=NULL)
							Rfree(v1->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(v1->data.float_value!=NULL)
							Rfree(v1->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(v1->data.short_value!=NULL)
							Rfree(v1->data.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(v1->data.integer_value!=NULL)
							Rfree(v1->data.integer_value);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] is invalid.",c->type);
						break;
				}
				v1->dlen=0;
			}
			Rfree(hold);
		}
	} else {
		mem->values=Rmalloc(numHoldCells*sizeof(MATRIXvalue));
		mem->num=numHoldCells;
		for(x=0,v1=mem->values,c=HoldCells;x<numHoldCells;++x,++c,++v1)
		{
			switch(c->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					v1->data.string_value=NULL;
					v1->dlen=0;
					break;
				case BOOLNS:
				case CHARACTERS:
					v1->data.string_value=Rmalloc(1);
					*v1->data.string_value=0;
					v1->dlen=1;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					v1->data.float_value=Rmalloc(sizeof(double));
					*v1->data.float_value=0;
					v1->dlen=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					v1->data.short_value=Rmalloc(sizeof(short));
					*v1->data.short_value=0;
					v1->dlen=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					v1->data.integer_value=Rmalloc(sizeof(int));
					*v1->data.integer_value=0;
					v1->dlen=sizeof(int);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] is invalid.",c->type);
					break;
			}
		}
	}
}
static void savesetup(RDArsrc *srsrc)
{
	MATRIXcell *c1,*c2;
	MATRIXrec *rec;
	MATRIXmem *mem=NULL;
	int x,a,b,Yfieldtype=0,Xfieldtype=0,xtype=0,ytype=0;
	short Yfieldlen=0,Xfieldlen=0;
	AXISmem *am=NULL;
	char cvalue=FALSE;

	readallwidgets(srsrc);
	readmatrix();
	FINDRSCGETINT(srsrc,"COMPUTE TYPES",&DFV->locate_method);
	FINDRSCGETSTRING(srsrc,"X-AXIS TITLE 1",&DFV->xaxis.title1);
	FINDRSCGETSTRING(srsrc,"X-AXIS TITLE 2",&DFV->xaxis.title2);
	FINDRSCGETSTRING(srsrc,"X-AXIS DESCRIPTION",&DFV->xaxis.desc);
	FINDRSCGETINT(srsrc,"X-AXIS TYPE",&xtype);
	FINDRSCGETCHAR(srsrc,"X-AXIS PRINT TITLE 1",&cvalue);
	DFV->xaxis.print_title1=cvalue;
	FINDRSCGETCHAR(srsrc,"X-AXIS PRINT TITLE 2",&cvalue);
	DFV->xaxis.print_title2=cvalue;
	FINDRSCGETCHAR(srsrc,"Y-AXIS PRINT TITLE 1",&cvalue);
	DFV->yaxis.print_title1=cvalue;
	FINDRSCGETCHAR(srsrc,"Y-AXIS PRINT TITLE 2",&cvalue);
	DFV->yaxis.print_title2=cvalue;
	if(xtype!=5)
	{
		FINDRSCGETINT(srsrc,"X-AXIS FIELD TYPES",&Xfieldtype);
		FINDRSCGETSHORT(srsrc,"X-AXIS FIELD LENGTH",&Xfieldlen);
		if(DFV->xaxis.title2!=NULL) Rfree(DFV->xaxis.title2);
	} else {
		FINDRSCGETSTRING(srsrc,"X-AXIS TITLE 2",&DFV->xaxis.title2);
		Xfieldtype=(-1);
		Xfieldlen=0;
	}
	FINDRSCGETSTRING(srsrc,"Y-AXIS TITLE 1",&DFV->yaxis.title1);
	FINDRSCGETSTRING(srsrc,"Y-AXIS DESCRIPTION",&DFV->yaxis.desc);
	FINDRSCGETINT(srsrc,"Y-AXIS TYPE",&ytype);
	if(ytype!=5)
	{
		FINDRSCGETINT(srsrc,"Y-AXIS FIELD TYPES",&Yfieldtype);
		FINDRSCGETSHORT(srsrc,"Y-AXIS FIELD LENGTH",&Yfieldlen);
		if(DFV->yaxis.title2!=NULL) Rfree(DFV->yaxis.title2);
	} else {
		FINDRSCGETSTRING(srsrc,"Y-AXIS TITLE 2",&DFV->yaxis.title2);
		Yfieldtype=(-1);
		Yfieldlen=0;
	}
	if(srsrc!=NULL) killwindow(srsrc);
/* CONVERT AXIS */
	if((DFV->xaxis.mem!=NULL) && ((xtype!=DFV->xaxis.axis_type) ||
		(Xfieldtype!=DFV->xaxis.type) || (Xfieldlen!=DFV->xaxis.len)))
	{
		for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
		{
			CONVERTFIELDTYPE(&DFV->xaxis.type,&am->data,&DFV->xaxis.len,&am->dlen,Xfieldtype,Xfieldlen);
		}
	}
	if((DFV->yaxis.mem!=NULL) && ((ytype!=DFV->yaxis.axis_type) ||
		(Yfieldtype!=DFV->yaxis.type) || (Yfieldlen!=DFV->yaxis.len)))
	{
		for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
		{
			CONVERTFIELDTYPE(&DFV->yaxis.type,&am->data,&DFV->yaxis.len,&am->dlen,Yfieldtype,Yfieldlen);
		}
	}
/* CONVERT MATRIX */
	if(DFV->recs!=NULL)
	{
/* CHECK FOR CELL DEFERENCES */
		for(a=0,rec=DFV->recs;a<DFV->num;++a,++rec)
		{
			if(rec->mems!=NULL)
			{
				for(b=0,mem=rec->mems;b<rec->num;++b,++mem)
				{
					ConvertMATRIXmem(&mem);
				}
			}
		}
	}
/* Setting Axis Types */
	DFV->xaxis.axis_type=xtype;
	DFV->xaxis.type=Xfieldtype;
	DFV->xaxis.len=Xfieldlen;
	DFV->yaxis.axis_type=ytype;
	DFV->yaxis.type=Yfieldtype;
	DFV->yaxis.len=Yfieldlen;
/* MATRIX CONVERTED */
/* CHANGING CELL DEFINITIONS */
	if(DFV->defs!=NULL)
	{
		for(x=0,c1=DFV->defs;x<DFV->numdefs;++x,++c1)
		{
			if(c1->name!=NULL) Rfree(c1->name);
		}
		Rfree(DFV->defs);
		DFV->numdefs=0;
	}
	if(HoldCells!=NULL)
	{
		DFV->defs=Rmalloc(numHoldCells*sizeof(MATRIXcell));
		DFV->numdefs=numHoldCells;
		for(x=0,c1=HoldCells,c2=DFV->defs;x<numHoldCells;++x,++c1,++c2)
		{
			if(c1->name!=NULL)
			{
				c2->name=stralloc(c1->name);
				Rfree(c1->name);
			} else c2->name=NULL;
			c2->type=c1->type;
			c2->len=c1->len;
			c2->displayed=c1->displayed;
		}
		Rfree(HoldCells);
		HoldCells=NULL;
		numHoldCells=0;
	}
	quitsetup(srsrc);
/* CELL DEFINITIONS CHANGED */
	if(MAINRSRC!=NULL)
	{
		killwindow(MAINRSRC);
		free_rsrc(MAINRSRC);
	}
	MAINRSRC=GenerateMatrixRDArsrc();
	if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
	matrixdefaults=GetDefaults(MAINRSRC);
	APPmakescrn(MAINRSRC,TRUE,quiteditmatrix,NULL,TRUE);
}
static void makecelllist()
{
	int l=0,x;
	MATRIXcell *c;
	char *s=NULL;

	if(cellvalues!=NULL) freeapplib(cellvalues);
	cellvalues=APPlibNEW();
	if(HoldCells!=NULL)
	{
		for(x=0,c=HoldCells;x<numHoldCells;++x,++c)
		{
			l=RDAstrlen(c->name)+RDAstrlen(standardfieldtypes[c->type])+50;
			s=Rmalloc(l);
			sprintf(s,"Name:[%s] Type:[%s] Length:[%d] Displayed:[%s]",
				(c->name!=NULL ? c->name:""),standardfieldtypes[c->type],
				c->len,(c->displayed ? "Yes":"No"));
			addAPPlib(cellvalues,s);
			Rfree(s);
		}
	}
	if(cellvalues->numlibs<1)
	{
		addAPPlib(cellvalues,"No Cell Definitions");
	}
}
static void makespacecellvalues(int pos)
{
	short x,y;
	MATRIXcell *temp=NULL,*temp1=NULL,*new_fields=NULL;

	++numHoldCells;
	new_fields=Rmalloc(numHoldCells*sizeof(MATRIXcell));
	y=0;
	for(x=0,temp=HoldCells;x<(numHoldCells-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->type=(-1);
			temp1->len=0;
			temp1->displayed=TRUE;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->len=temp->len;
		temp1->displayed=temp->displayed;
		++y;
	}
	Rfree(HoldCells);
	HoldCells=Rmalloc(numHoldCells*sizeof(MATRIXcell));
	for(x=0,temp1=HoldCells,temp=new_fields;x<numHoldCells;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->len=temp->len;
		temp1->displayed=temp->displayed;
	}
	Rfree(new_fields);
}
static void quit_cell(RDArsrc *cellrsrc,RDArsrc *srsrc)
{
	if(fieldtypes!=NULL) freeapplib(fieldtypes);
	if(cellrsrc!=NULL)
	{
		killwindow(cellrsrc);
		free_rsrc(cellrsrc);
	}
	if(celldefaults!=NULL) FreeRDAdefault(celldefaults);
}
static void quit_celltest(RDArsrc *cellrsrc,RDArsrc *srsrc)
{
	readallwidgets(cellrsrc);
	if(COMPARE_RSRCS(cellrsrc,celldefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_cell,quit_cell,FALSE,2,cellrsrc,srsrc,NULL);
	} else {
		quit_cell(cellrsrc,srsrc);
	}
}
static void save_cell(RDArsrc *cellrsrc,RDArsrc *srsrc)
{
	int value=0,x=0;
	MATRIXcell *i=NULL;
	char disp=FALSE;
	short len=0;

	if(FINDRSCGETINT(srsrc,"CELL VALUES",&value)) return;
	i=HoldCells+value;
	readallwidgets(cellrsrc);
	FINDRSCGETSTRING(cellrsrc,"NAME",&i->name);
	FINDRSCGETINT(cellrsrc,"FIELD TYPES",&x);
	i->type=x;
	FINDRSCGETSHORT(cellrsrc,"LENGTH",&len);
	i->len=len;
	FINDRSCGETCHAR(cellrsrc,"DISPLAYED",&disp);
	i->displayed=disp;
	makecelllist();
	if(!FINDRSCLISTAPPlib(srsrc,"CELL VALUES",value,cellvalues))
		updatersrc(srsrc,"CELL VALUES");
	if(COMPARE_RSRCS(cellrsrc,celldefaults,2))
	{
		changedvalues=TRUE;
	}
	quit_cell(cellrsrc,srsrc);
}
static void save_cell_above(RDArsrc *cellrsrc,RDArsrc *srsrc)
{
	int x;

	if(FINDRSCGETINT(srsrc,"CELL VALUES",&x)) return;
	makespacecellvalues(x);
	save_cell(cellrsrc,srsrc);
}
static void save_cell_below(RDArsrc *cellrsrc,RDArsrc *srsrc)
{
	int x;

	if(FINDRSCGETINT(srsrc,"CELL VALUES",&x)) return;
	makespacecellvalues(++x);
	FINDRSCSETINT(srsrc,"CELL VALUES",x);
	updatersrc(srsrc,"CELL VALUES");
	save_cell(cellrsrc,srsrc);
}
static void quit_cell_abovetest(RDArsrc *cellrsrc,RDArsrc *srsrc)
{
	readallwidgets(cellrsrc);
	if(COMPARE_RSRCS(cellrsrc,celldefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_cell_above,quit_cell,FALSE,2,cellrsrc,srsrc,NULL);
	} else {
		quit_cell(cellrsrc,srsrc);
	}
}
static void quit_cell_belowtest(RDArsrc *cellrsrc,RDArsrc *srsrc)
{
	readallwidgets(cellrsrc);
	if(COMPARE_RSRCS(cellrsrc,celldefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_cell_below,quit_cell,FALSE,2,cellrsrc,srsrc,NULL);
	} else {
		quit_cell(cellrsrc,srsrc);
	}
}
static void celldefinitionscreen(RDArsrc *srsrc,char *name,short type,
	short len,short displayed,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0;
	RDArsrc *cellrsrc=NULL;
	char disp=FALSE;

	cellrsrc=RDArsrcNEW(module,"MAINTAIN MATRIX CELLS");
	addstdrsrc(cellrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),TRUE);
	if(fieldtypes!=NULL) freeapplib(fieldtypes);
	fieldtypes=APPlibNEW();
	for(x=0;x<28;++x) addAPPlib(fieldtypes,standardfieldtypes[x]);
	if(type<0) x=0;
		else x=type;
	addlstrsrc(cellrsrc,"FIELD TYPES",&x,TRUE,NULL,fieldtypes->numlibs,
		&fieldtypes->libs,NULL);
	addstdrsrc(cellrsrc,"LENGTH",SHORTV,4,&len,TRUE);
	disp=displayed;
	addstdrsrc(cellrsrc,"DISPLAYED",BOOLNS,1,&disp,TRUE);
	addrfcbrsrc(cellrsrc,"SAVE",TRUE,savefunc,srsrc);
	addrfcbrsrc(cellrsrc,"QUIT",TRUE,quitfunctest,srsrc);
	addbtnrsrc(cellrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(cellrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	celldefaults=GetDefaults(cellrsrc);
	if(ADVmakescrn(cellrsrc,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the MAINTAIN MATRIX CELLS.  Call your installer.",NULL,FALSE);
		quit_cell(cellrsrc,srsrc);
	}
}
static void add_cell_above(RDArsrc *srsrc)
{
	int selected=0;

	if(FINDRSCGETINT(srsrc,"CELL VALUES",&selected)) return;
	if(cellvalues->numlibs==1 && !RDAstrcmp(cellvalues->libs[0],"No Cell Definitions")) return;
	celldefinitionscreen(srsrc,NULL,0,0,FALSE,save_cell_above,quit_cell_abovetest);
}
static void add_cell_below(RDArsrc *srsrc)
{
	int selected=0;

	if(FINDRSCGETINT(srsrc,"CELL VALUES",&selected)) return;
	if(cellvalues->numlibs==1 && !RDAstrcmp(cellvalues->libs[0],"No Cell Definitions")) return;
	celldefinitionscreen(srsrc,NULL,0,0,FALSE,save_cell_below,quit_cell_belowtest);
}
static void edit_cell(RDArsrc *srsrc)
{
	int x;
	MATRIXcell *c;

	if(FINDRSCGETINT(srsrc,"CELL VALUES",&x)) return;
	if(HoldCells==NULL)
	{
		HoldCells=Rmalloc(sizeof(MATRIXcell));
		c=HoldCells;
		c->name=NULL;
		c->type=(-1);
		c->len=0;
		c->displayed=TRUE;
		numHoldCells=1;
	}
	c=HoldCells+x;
	celldefinitionscreen(srsrc,c->name,c->type,c->len,c->displayed,
		save_cell,quit_celltest);
}
static void delete_cell(RDArsrc *srsrc)
{
	short x,y;
	MATRIXcell *temp=NULL,*temp1=NULL,*new_fields=NULL;
	int value=0;

	readallwidgets(srsrc);
	if(FINDRSCGETINT(srsrc,"CELL VALUES",&value)) return;
	if(numHoldCells<2) return;
	new_fields=Rmalloc((numHoldCells-1)*sizeof(MATRIXcell));
	y=0;
	for(x=0,temp=HoldCells;x<numHoldCells;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->type=temp->type;
			temp1->len=temp->len;
			temp1->displayed=temp->displayed;
			++y;
		} else {
			if(temp->name!=NULL) Rfree(temp->name);
		}
	}
	Rfree(HoldCells);
	--numHoldCells;
	HoldCells=Rmalloc(numHoldCells*sizeof(MATRIXcell));
	for(x=0,temp1=HoldCells,temp=new_fields;x<numHoldCells;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->len=temp->len;
		temp1->displayed=temp->displayed;
	}
	Rfree(new_fields);
	makecelllist();
	if(!FINDRSCLISTAPPlib(srsrc,"CELL VALUES",value,cellvalues))
		updatersrc(srsrc,"CELL VALUES");
	changedvalues=TRUE;
}
static void changexaxistypes(RDArsrc *srsrc)
{
	int selected=0;

	FINDRSCGETINT(srsrc,"X-AXIS TYPE",&selected);
	if(selected==5)
	{
		FINDRSCSETEDITABLE(srsrc,"X-AXIS FIELD TYPES",FALSE);
		FINDRSCSETEDITABLE(srsrc,"X-AXIS FIELD LENGTH",FALSE);
		FINDRSCSETSENSITIVE(srsrc,"X-AXIS FIELD TYPES",FALSE);
		FINDRSCSETSENSITIVE(srsrc,"X-AXIS FIELD LENGTH",FALSE);
		FINDRSCSETEDITABLE(srsrc,"X-AXIS TITLE 2",TRUE);
		FINDRSCSETSENSITIVE(srsrc,"X-AXIS TITLE 2",TRUE);
	} else {
		FINDRSCSETSENSITIVE(srsrc,"X-AXIS FIELD TYPES",TRUE);
		FINDRSCSETSENSITIVE(srsrc,"X-AXIS FIELD LENGTH",TRUE);
		FINDRSCSETEDITABLE(srsrc,"X-AXIS FIELD TYPES",TRUE);
		FINDRSCSETEDITABLE(srsrc,"X-AXIS FIELD LENGTH",TRUE);
		FINDRSCSETSENSITIVE(srsrc,"X-AXIS TITLE 2",FALSE);
		FINDRSCSETEDITABLE(srsrc,"X-AXIS TITLE 2",FALSE);
	}
}
static void changeyaxistypes(RDArsrc *srsrc)
{
	int selected=0;

	FINDRSCGETINT(srsrc,"Y-AXIS TYPE",&selected);
	if(selected==5)
	{
		FINDRSCSETEDITABLE(srsrc,"Y-AXIS FIELD TYPES",FALSE);
		FINDRSCSETEDITABLE(srsrc,"Y-AXIS FIELD LENGTH",FALSE);
		FINDRSCSETSENSITIVE(srsrc,"Y-AXIS FIELD TYPES",FALSE);
		FINDRSCSETSENSITIVE(srsrc,"Y-AXIS FIELD LENGTH",FALSE);
		FINDRSCSETEDITABLE(srsrc,"Y-AXIS TITLE 2",TRUE);
		FINDRSCSETSENSITIVE(srsrc,"Y-AXIS TITLE 2",TRUE);
	} else {
		FINDRSCSETSENSITIVE(srsrc,"Y-AXIS FIELD TYPES",TRUE);
		FINDRSCSETSENSITIVE(srsrc,"Y-AXIS FIELD LENGTH",TRUE);
		FINDRSCSETEDITABLE(srsrc,"Y-AXIS FIELD TYPES",TRUE);
		FINDRSCSETEDITABLE(srsrc,"Y-AXIS FIELD LENGTH",TRUE);
		FINDRSCSETSENSITIVE(srsrc,"Y-AXIS TITLE 2",FALSE);
		FINDRSCSETEDITABLE(srsrc,"Y-AXIS TITLE 2",FALSE);
	}
}
/***************************************************************************
Setup Menu used to define Cell Definitions ........
***************************************************************************/
static void setup_menu(RDArsrc *mainrsrc)
{
	RDArsrc *srsrc=NULL;
	MATRIXcell *c1,*c2;
	int x=0;
	short sv=0;
	char cvalue=FALSE;

	srsrc=RDArsrcNEW(module,"SETUP MATRIX MENU");
	if(DFV->defs!=NULL)
	{
		numHoldCells=DFV->numdefs;
		HoldCells=Rmalloc(DFV->numdefs*sizeof(MATRIXcell));
		for(x=0,c1=DFV->defs,c2=HoldCells;x<DFV->numdefs;++x,++c1,++c2)
		{
			if(!isEMPTY(c1->name)) c2->name=stralloc(c1->name);
			c2->type=c1->type;
			c2->len=c1->len;
			c2->displayed=c1->displayed;
		}
	} else {
		HoldCells=NULL;
		numHoldCells=0;
	}
	if(computetypes!=NULL) freeapplib(computetypes);
	computetypes=APPlibNEW();
	addAPPlib(computetypes,"Locate by Row/Column");
	addAPPlib(computetypes,"Locate Linearly");
	x=DFV->locate_method;
	addlstrsrc(srsrc,"COMPUTE TYPES",&x,TRUE,NULL,computetypes->numlibs,
		&computetypes->libs,NULL);
	if(cellvalues!=NULL) freeapplib(cellvalues);
	cellvalues=APPlibNEW();
	x=0;
	addlstrsrc(srsrc,"CELL VALUES",&x,TRUE,NULL,cellvalues->numlibs,
		&cellvalues->libs,NULL);
	addbtnrsrc(srsrc,"ADD ABOVE",TRUE,add_cell_above,NULL);
	addbtnrsrc(srsrc,"ADD BELOW",TRUE,add_cell_below,NULL);
	addbtnrsrc(srsrc,"DELETE",TRUE,delete_cell,NULL);
	addbtnrsrc(srsrc,"SELECT",TRUE,edit_cell,NULL);
	makecelllist();
	FINDRSCLISTAPPlib(srsrc,"CELL VALUES",x,cellvalues);
	if(axistypes!=NULL) freeapplib(axistypes);
	axistypes=APPlibNEW();
	addAPPlib(axistypes,"Equal To");
	addAPPlib(axistypes,"Greater Than");
	addAPPlib(axistypes,"Greater Than or Equal To");
	addAPPlib(axistypes,"Less Than");
	addAPPlib(axistypes,"Less Than or Equal To");
	addAPPlib(axistypes,"Display Only");
	addstdrsrc(srsrc,"X-AXIS TITLE 1",VARIABLETEXT,0,(DFV->xaxis.title1!=NULL
		? DFV->xaxis.title1:""),TRUE);
	addstdrsrc(srsrc,"X-AXIS TITLE 2",VARIABLETEXT,0,(DFV->xaxis.title2!=NULL
		? DFV->xaxis.title2:""),TRUE);
	addsctrsrc(srsrc,"X-AXIS DESCRIPTION",0,(DFV->xaxis.desc!=NULL ?
		DFV->xaxis.desc:""),TRUE);
	x=DFV->xaxis.axis_type;
	addlstrsrc(srsrc,"X-AXIS TYPE",&x,TRUE,changexaxistypes,
		axistypes->numlibs,&axistypes->libs,NULL);
	if(fieldtypes!=NULL) freeapplib(fieldtypes);
	fieldtypes=APPlibNEW();
	for(x=0;x<28;++x) addAPPlib(fieldtypes,standardfieldtypes[x]);
	if(DFV->xaxis.type<0)
	{
		x=0;
	} else {
		x=DFV->xaxis.type;
	}
	addlstrsrc(srsrc,"X-AXIS FIELD TYPES",&x,TRUE,NULL,fieldtypes->numlibs,
		&fieldtypes->libs,NULL);
	sv=DFV->xaxis.len;
	addstdrsrc(srsrc,"X-AXIS FIELD LENGTH",SHORTV,4,&sv,TRUE);
	cvalue=DFV->xaxis.print_title1;
	addstdrsrc(srsrc,"X-AXIS PRINT TITLE 1",BOOLNS,1,&cvalue,TRUE);
	cvalue=DFV->xaxis.print_title2;
	addstdrsrc(srsrc,"X-AXIS PRINT TITLE 2",BOOLNS,1,&cvalue,TRUE);
	changexaxistypes(srsrc);
	addstdrsrc(srsrc,"Y-AXIS TITLE 1",VARIABLETEXT,0,(DFV->yaxis.title1!=NULL
		? DFV->yaxis.title1:""),TRUE);
	addstdrsrc(srsrc,"Y-AXIS TITLE 2",VARIABLETEXT,0,(DFV->yaxis.title2!=NULL
		? DFV->yaxis.title2:""),TRUE);
	addsctrsrc(srsrc,"Y-AXIS DESCRIPTION",0,(DFV->yaxis.desc!=NULL ?
		DFV->yaxis.desc:""),TRUE);
	x=DFV->yaxis.axis_type;
	addlstrsrc(srsrc,"Y-AXIS TYPE",&x,TRUE,changeyaxistypes,
		axistypes->numlibs,&axistypes->libs,NULL);
	if(DFV->yaxis.type<0)
	{
		x=0;
	} else {
		x=DFV->yaxis.type;
	}
	addlstrsrc(srsrc,"Y-AXIS FIELD TYPES",&x,TRUE,NULL,fieldtypes->numlibs,
		&fieldtypes->libs,NULL);
	sv=DFV->yaxis.len;
	addstdrsrc(srsrc,"Y-AXIS FIELD LENGTH",SHORTV,4,&sv,TRUE);
	cvalue=DFV->yaxis.print_title1;
	addstdrsrc(srsrc,"Y-AXIS PRINT TITLE 1",BOOLNS,1,&cvalue,TRUE);
	cvalue=DFV->yaxis.print_title2;
	addstdrsrc(srsrc,"Y-AXIS PRINT TITLE 2",BOOLNS,1,&cvalue,TRUE);
	changeyaxistypes(srsrc);
	addbtnrsrc(srsrc,"SAVE",TRUE,savesetup,NULL);
	addrfkwrsrc(srsrc,"QUIT",TRUE,quitsetup,NULL);
	addbtnrsrc(srsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(srsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(ADVmakescrn(srsrc,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the SETUP MATRIX MENU.  Call your installer.",NULL,FALSE);
		quitsetup(srsrc);
	}
}
static void quitwindowmenu(RDArsrc *wrsrc)
{
	if(wrsrc!=NULL) free_rsrc(wrsrc);
	if(windowtypes!=NULL) freeapplib(windowtypes);
	if(cellconfig!=NULL) freeapplib(cellconfig);
}
static void savewindowmenu(RDArsrc *wrsrc)
{
	readallwidgets(wrsrc);
	readmatrix();
	FINDRSCGETINT(wrsrc,"WINDOW TYPES",&DFV->matrix_buttons);
	FINDRSCGETINT(wrsrc,"CELL TYPES",&DFV->cell_config);
	if(wrsrc!=NULL) killwindow(wrsrc);
	if(MAINRSRC!=NULL)
	{
		killwindow(MAINRSRC);
		free_rsrc(MAINRSRC);
	}
	quitwindowmenu(wrsrc);
	MAINRSRC=GenerateMatrixRDArsrc();
	if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
	matrixdefaults=GetDefaults(MAINRSRC);
	APPmakescrn(MAINRSRC,TRUE,quiteditmatrix,NULL,TRUE);
}
/***************************************************************************
Window Menu is for Configuring the EDIT MATRIX Window Definition....
***************************************************************************/
static void window_menu(RDArsrc *mainrsrc)
{
	RDArsrc *wrsrc=NULL;
	int x=0;

	wrsrc=RDArsrcNEW(module,"MATRIX WINDOW MENU");
	if(windowtypes!=NULL) freeapplib(windowtypes);
	windowtypes=APPlibNEW();
	addAPPlib(windowtypes,"Standard Window");
	addAPPlib(windowtypes,"Graphic Buttons");
	x=DFV->matrix_buttons;
	addlstrsrc(wrsrc,"WINDOW TYPES",&x,TRUE,NULL,windowtypes->numlibs,
		&windowtypes->libs,NULL);
	if(cellconfig!=NULL) freeapplib(cellconfig);
	cellconfig=APPlibNEW();
	addAPPlib(cellconfig,"Normal Matrix");
	addAPPlib(cellconfig,"Power Matrix");
	x=DFV->cell_config;
	addlstrsrc(wrsrc,"CELL TYPES",&x,TRUE,NULL,cellconfig->numlibs,
		&cellconfig->libs,NULL);
	addbtnrsrc(wrsrc,"SAVE",TRUE,savewindowmenu,NULL);
	addrfkwrsrc(wrsrc,"QUIT",TRUE,quitwindowmenu,NULL);
	addbtnrsrc(wrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(wrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(wrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	if(ADVmakescrn(wrsrc,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the MATRIX WINDOW MENU.  Call your installer.",NULL,FALSE);
		quitwindowmenu(wrsrc);
	}
}
static void quitapply(RDArsrc *applyrsrc)
{
	if(applyrsrc!=NULL) free_rsrc(applyrsrc);
	if(columnlist!=NULL) freeapplib(columnlist);
	if(rowlist!=NULL) freeapplib(rowlist);
	if(cellvalues!=NULL) freeapplib(cellvalues);
}
static void saveapply(RDArsrc *applyrsrc)
{
	int sc=0,x,y,from_cell=0,to_cell=0,fc=0,tc=0,fr=0,tr=0,cellnum=0;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXcell *c;
	MATRIXvalue *v;
	char *expression=NULL,rcell=FALSE,rc=FALSE,rr=FALSE,doit=FALSE;
	char *select_expression=NULL,xstemp[101];

	readallwidgets(applyrsrc);
	FINDRSCGETINT(applyrsrc,"CELL LIST",&sc);
	FINDRSCGETSTRING(applyrsrc,"EXPRESSION",&expression);
	FINDRSCGETSTRING(applyrsrc,"SELECT EXPRESSION",&select_expression);
	FINDRSCGETCHAR(applyrsrc,"RANGE ON CELLS",&rcell);
	FINDRSCGETINT(applyrsrc,"FROM CELL",&from_cell);
	FINDRSCGETINT(applyrsrc,"TO CELL",&to_cell);
	FINDRSCGETCHAR(applyrsrc,"RANGE ON COLUMNS",&rc);
	FINDRSCGETINT(applyrsrc,"FROM COLUMN LIST",&fc);
	FINDRSCGETINT(applyrsrc,"TO COLUMN LIST",&tc);
	FINDRSCGETCHAR(applyrsrc,"RANGE ON ROWS",&rr);
	FINDRSCGETINT(applyrsrc,"FROM ROW LIST",&fr);
	FINDRSCGETINT(applyrsrc,"TO ROW LIST",&tr);
	FINDRSCGETCHAR(applyrsrc,"ADD PERCENTAGE",&AddPercentage);
	FINDRSCGETCHAR(applyrsrc,"ROUND DOLLARS",&RoundDollars);
	FINDRSCGETCHAR(applyrsrc,"ADD AMOUNT",&AddAmount);
	FINDRSCGETDOUBLE(applyrsrc,"PERCENTAGE",&Percentage);
	FINDRSCGETDOUBLE(applyrsrc,"AMOUNT",&Amount);
	if(applyrsrc!=NULL) killwindow(applyrsrc);
	readmatrix();
	if(!isEMPTY(expression))
	{
		if(DFV!=NULL)
		{
		if(DFV->recs!=NULL)
		{
			ValueNumber=sc;
			if(!DFV->locate_method)
			{
				for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
				{
					RowNumber=x;
					doit=FALSE;
					if(rr)
					{
						if(x>=fr && x<=tr)
						{
							doit=TRUE;
						} else doit=FALSE;
					} else doit=TRUE;
					if(doit)
					{
						if(rec->mems!=NULL)
						{
							for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
							{
								ColumnNumber=y;
								doit=FALSE;
								if(rc)
								{
									if(y>=fc && y<=tc)
									{
										doit=TRUE;
									} else doit=FALSE;
								} else doit=TRUE;
								if(doit)
								{
									if(!isEMPTY(select_expression))
									{
										doit=EVALUATEbol(select_expression,MatrixSubData,DFV);
									} else doit=TRUE;
									if(doit)
									{
									v=mem->values+sc;
									c=DFV->defs+sc;
									sprintf(xstemp,"R%dC%dV%d",x+1,y+1,sc+1);
									switch(c->type)
									{
										case SCROLLEDTEXT:
										case VARIABLETEXT:
										case DATES:
										case TIMES:
										case SOCSECNUM:
										case PLAINTEXT:
										case PHONE:
										case CUSTOMTYPE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:
										case ZIPCODE:
											if(v->data.string_value!=NULL) Rfree(v->data.string_value);
											v->data.string_value=NULL;
											v->dlen=0;
											v->data.string_value=EVALUATEstr(expression,MatrixSubData,DFV);
											if(!isEMPTY(v->data.string_value)) v->dlen=RDAstrlen(v->data.string_value)+1;
											FINDRSCSETSTRING(MAINRSRC,xstemp,v->data.string_value);
											break;
										case BOOLNS:
										case CHARACTERS:
											*v->data.string_value=EVALUATEbol(expression,MatrixSubData,DFV);
											FINDRSCSETCHAR(MAINRSRC,xstemp,*v->data.string_value);
											break;
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
										case SDECIMALV:
											*v->data.float_value=EVALUATEdbl(expression,MatrixSubData,DFV);
											FINDRSCSETDOUBLE(MAINRSRC,xstemp,*v->data.float_value);
											break;
										case SHORTV:
										case SSHORTV:
											*v->data.short_value=EVALUATEsht(expression,MatrixSubData,DFV);
											FINDRSCSETSHORT(MAINRSRC,xstemp,*v->data.short_value);
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											*v->data.integer_value=EVALUATEint(expression,MatrixSubData,DFV);
											FINDRSCSETINT(MAINRSRC,xstemp,*v->data.integer_value);
											break;
										case BUTTONS:
											prterr("Error call RaddogTiddog cause this is weird....");
											break;
										default:
											prterr("Error field type [%d] invalid for Cell Value # [%d] on Table [%s] [%s].",c->type,x,DFV->module,DFV->name);
											break;
									}
									updatersrc(MAINRSRC,xstemp);
									}
								}
							}
						}
					}
				}
			} else {
				for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
				{
					RowNumber=x;
					if(rec->mems!=NULL)
					{
						for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
						{
							ColumnNumber=y;
							if(rcell)
							{
								cellnum=(x*DFV->xaxis.num)+y+1;
								if(cellnum>=from_cell && cellnum<=to_cell)
								{
									doit=TRUE;
								} else doit=FALSE;
							} else doit=TRUE;
							if(doit)
							{
								if(!isEMPTY(select_expression))
								{
									doit=EVALUATEbol(select_expression,MatrixSubData,DFV);
								} else doit=TRUE;
								if(doit)
								{
								v=mem->values+sc;
								c=DFV->defs+sc;
								sprintf(xstemp,"CELL %d VALUE %d",
									((x*rec->num)+y+1),sc+1);
								switch(c->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										if(v->data.string_value!=NULL) Rfree(v->data.string_value);
										v->data.string_value=NULL;
										v->dlen=0;
										v->data.string_value=EVALUATEstr(expression,MatrixSubData,DFV);
										if(!isEMPTY(v->data.string_value)) v->dlen=RDAstrlen(v->data.string_value)+1;
										FINDRSCSETSTRING(MAINRSRC,xstemp,v->data.string_value);
										break;
									case BOOLNS:
									case CHARACTERS:
										*v->data.string_value=EVALUATEbol(expression,MatrixSubData,DFV);
										FINDRSCSETCHAR(MAINRSRC,xstemp,*v->data.string_value);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										*v->data.float_value=EVALUATEdbl(expression,MatrixSubData,DFV);
										FINDRSCSETDOUBLE(MAINRSRC,xstemp,*v->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										*v->data.short_value=EVALUATEsht(expression,MatrixSubData,DFV);
										FINDRSCSETSHORT(MAINRSRC,xstemp,*v->data.short_value);
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										*v->data.integer_value=EVALUATEint(expression,MatrixSubData,DFV);
										FINDRSCSETINT(MAINRSRC,xstemp,*v->data.integer_value);
										break;
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										break;
									default:
										prterr("Error field type [%d] invalid for Cell Value # [%d] on Table [%s] [%s].",c->type,x,DFV->module,DFV->name);
										break;
								}
								updatersrc(MAINRSRC,xstemp);
								}
							}
						}
					}
				}
			}
		}
		}
	}
	if(expression!=NULL) Rfree(expression);
	if(select_expression!=NULL) Rfree(select_expression);
#ifdef XXX
	if(MAINRSRC!=NULL)
	{
		killwindow(MAINRSRC);
		free_rsrc(MAINRSRC);
	}
#endif
	quitapply(applyrsrc);
#ifdef XXX
	MAINRSRC=GenerateMatrixRDArsrc();
	if(matrixdefaults!=NULL) FreeRDAdefault(matrixdefaults);
	matrixdefaults=GetDefaults(MAINRSRC);
	APPmakescrn(MAINRSRC,TRUE,quiteditmatrix,NULL,TRUE);
#endif
}
static APPlib *ListMatrixValues()
{
	APPlib *tmp=NULL;
	int x,y,z;
	MATRIXrec *rec;
	MATRIXmem *mem;

	tmp=APPlibNEW();
	if(DFV!=NULL)
	{
		if(DFV->recs!=NULL)
		{
			for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
			{
				if(rec->mems!=NULL)
				{
					for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
					{
						if(mem->sensitive)
						{
							if(mem->values!=NULL)
							{
								for(z=0;z<mem->num;++z)
								{
									if(!DFV->locate_method)
									{
										sprintf(stemp,"[R%dC%dV%d]",x+1,y+1,
											z+1);
									} else {
										sprintf(stemp,"[CELL %d VALUE %d]",
											((x*rec->num)+y+1),z+1);
									}
									addAPPlib(tmp,stemp);
								}
							}
						}
					}
				}
			}
		}
	}
	addAPPlib(tmp,"[CELL SENSITIVITY]");
	addAPPlib(tmp,"[CELL NUMBER]");
	addAPPlib(tmp,"[CELL VALUE NUMBER]");
/*
	addAPPlib(tmp,"[CELL VALUE INDEX]");
*/
	addAPPlib(tmp,"[CELL VALUE]");
	addAPPlib(tmp,"[ROW NUMBER]");
	addAPPlib(tmp,"[COLUMN NUMBER]");
	addAPPlib(tmp,"[NUMBER OF ROWS]");
	addAPPlib(tmp,"[NUMBER OF COLUMNS]");
	addAPPlib(tmp,"[NUMBER OF CELLS]");
	addAPPlib(tmp,"[CURRENT X-AXIS VALUE]");
	addAPPlib(tmp,"[CURRENT X-AXIS TITLE 1]");
	addAPPlib(tmp,"[CURRENT X-AXIS TITLE 2]");
	addAPPlib(tmp,"[CURRENT Y-AXIS VALUE]");
	addAPPlib(tmp,"[CURRENT Y-AXIS TITLE 1]");
	addAPPlib(tmp,"[CURRENT Y-AXIS TITLE 2]");
	addAPPlib(tmp,"[ADD PERCENTAGE]");
	addAPPlib(tmp,"[PERCENTAGE]");
	addAPPlib(tmp,"[ADD AMOUNT]");
	addAPPlib(tmp,"[AMOUNT]");
	SORTAPPlib(tmp);
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;
	RDArsrc *ldvalrsrc1=NULL;

	ldvalrsrc1=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc1,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=ListMatrixValues();
	x=0;
	addlstrsrc(ldvalrsrc1,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc1,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc1,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc1,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc1,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(ADVmakescrn(ldvalrsrc1,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the LOAD VALUE SCREEN.  Call your installer.",NULL,FALSE);
		quitldval(ldvalrsrc1,load);
	}
}
/***************************************************************************
Apply Menu is for Apply an Expression to a Row or Column
***************************************************************************/
static void setDefExpression(RDArsrc *mainrsrc)
{
	char DefExpression=FALSE;

	readwidget(mainrsrc,"DEFAULT FORMULA");
	FINDRSCGETCHAR(mainrsrc,"DEFAULT FORMULA",&DefExpression);
	if(DefExpression==TRUE)
	{
		FINDRSCSETSTRING(mainrsrc,"EXPRESSION","(ROUND(([CELL VALUE]+(NOT([ROUND DOLLARS]))*([CELL VALUE]*[ADD PERCENTAGE]*[PERCENTAGE]/100))+((([ROUND DOLLARS]*(ROUND([CELL VALUE]*[ADD PERCENTAGE]*[PERCENTAGE]/10000)))*100)+([ADD AMOUNT]*[AMOUNT]))))");
		FINDRSCSETEDITABLE(mainrsrc,"EXPRESSION",FALSE);
		updatersrc(mainrsrc,"EXPRESSION");
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"EXPRESSION",TRUE);
	}
}
static void setAddPercentage(RDArsrc *mainrsrc)
{
	char DefExpression=FALSE;

	readwidget(mainrsrc,"ADD PERCENTAGE");
	FINDRSCGETCHAR(mainrsrc,"ADD PERCENTAGE",&DefExpression);
	if(DefExpression==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"PERCENTAGE",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"PERCENTAGE",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"ROUND DOLLARS",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"ROUND DOLLARS",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"PERCENTAGE",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"PERCENTAGE",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"ROUND DOLLARS",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"ROUND DOLLARS",FALSE);
	}
	updatersrc(mainrsrc,"PERCENTAGE");
}
static void setAddAmount(RDArsrc *mainrsrc)
{
	char DefExpression=FALSE;

	readwidget(mainrsrc,"ADD AMOUNT");
	FINDRSCGETCHAR(mainrsrc,"ADD AMOUNT",&DefExpression);
	if(DefExpression==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"AMOUNT",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"AMOUNT",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"AMOUNT",FALSE);
	}
	updatersrc(mainrsrc,"AMOUNT");
}
void changeRangeRows(RDArsrc *mainrsrc)
{
	char rr=FALSE;
	readwidget(mainrsrc,"RANGE ON ROWS");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON ROWS",&rr);
	if(rr)
	{
		FINDRSCSETEDITABLE(mainrsrc,"FROM ROW LIST",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"TO ROW LIST",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"FROM ROW LIST",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO ROW LIST",TRUE);
	}
	else
	{
		FINDRSCSETEDITABLE(mainrsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"TO ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO ROW LIST",FALSE);
	}
}
void changeRangeCols(RDArsrc *mainrsrc)
{
	char rc=FALSE;
	readwidget(mainrsrc,"RANGE ON COLUMNS");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON COLUMNS",&rc);
	if(rc)
	{
		FINDRSCSETEDITABLE(mainrsrc,"FROM COLUMN LIST",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"TO COLUMN LIST",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"FROM COLUMN LIST",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO COLUMN LIST",TRUE);
	}
	else
	{
		FINDRSCSETEDITABLE(mainrsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"TO COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO COLUMN LIST",FALSE);
	}
}
void changeRangeCells(RDArsrc *mainrsrc)
{
	char rcell=FALSE;
	readwidget(mainrsrc,"RANGE ON CELLS");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON CELLS",&rcell);
	if(rcell)
	{
		FINDRSCSETEDITABLE(mainrsrc,"FROM CELL",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"TO CELL",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"FROM CELL",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO CELL",TRUE);
	}
	else
	{
		FINDRSCSETEDITABLE(mainrsrc,"FROM CELL",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"TO CELL",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"FROM CELL",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO CELL",FALSE);
	}
}
static void setCellType(RDArsrc *mainrsrc)
{
	MATRIXcell *c;
	int cnumber=0;
	char use_them=FALSE;

	FINDRSCGETINT(mainrsrc,"CELL LIST",&cnumber);
	if(DFV->defs!=NULL)
	{
		c=DFV->defs+cnumber;
		if(c!=NULL)
		{
			switch(c->type)
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
					break;
				case BOOLNS:
					break;
				case CHARACTERS:
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					use_them=TRUE;
					break;
				case SHORTV:
				case SSHORTV:
					break;
				case LONGV:
				case SLONGV:
					break;
				default:
					prterr("Error Field Type [%d] is invalid in Field [%s].",c->type,(c->name!=NULL ? c->name:""));
					break;
			}
		}
	}
	if(use_them==TRUE)
	{
		FINDRSCSETCHAR(mainrsrc,"DEFAULT FORMULA",TRUE);
		FINDRSCSETSTRING(mainrsrc,"EXPRESSION","(ROUND(([CELL VALUE]+([CELL VALUE]*[ADD PERCENTAGE]*[PERCENTAGE]/100)+([ADD AMOUNT]*[AMOUNT]))))");
		FINDRSCSETCHAR(mainrsrc,"ADD PERCENTAGE",AddPercentage);
		FINDRSCSETCHAR(mainrsrc,"ROUND DOLLARS",RoundDollars);
		FINDRSCSETCHAR(mainrsrc,"ADD AMOUNT",AddAmount);
		FINDRSCSETEDITABLE(mainrsrc,"DEFAULT FORMULA",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"ADD PERCENTAGE",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"ADD AMOUNT",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"DEFAULT FORMULA",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"ADD PERCENTAGE",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"ADD AMOUNT",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"EXPRESSION",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD CONTROL",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD CONTROL",FALSE);
		updatersrc(mainrsrc,"DEFAULT FORMULA");
		updatersrc(mainrsrc,"EXPRESSION");
		updatersrc(mainrsrc,"ADD PERCENTAGE");
		updatersrc(mainrsrc,"ADD AMOUNT");
	} else {
		FINDRSCSETCHAR(mainrsrc,"DEFAULT FORMULA",FALSE);
		FINDRSCSETSTRING(mainrsrc,"EXPRESSION",NULL);
		FINDRSCSETCHAR(mainrsrc,"ADD PERCENTAGE",FALSE);
		FINDRSCSETCHAR(mainrsrc,"ADD AMOUNT",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"DEFAULT FORMULA",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"ADD PERCENTAGE",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"ADD AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"DEFAULT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"ADD PERCENTAGE",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"ADD AMOUNT",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"EXPRESSION",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD VALUE",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"LOAD CONTROL",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"LOAD CONTROL",TRUE);
		updatersrc(mainrsrc,"DEFAULT FORMULA");
		updatersrc(mainrsrc,"EXPRESSION");
		updatersrc(mainrsrc,"ADD PERCENTAGE");
		updatersrc(mainrsrc,"ADD AMOUNT");
	}
	setDefExpression(mainrsrc);
	setAddPercentage(mainrsrc);
	setAddAmount(mainrsrc);
}
static void apply_menu(RDArsrc *mainrsrc)
{
	RDArsrc *applyrsrc=NULL;
	MATRIXcell *c=NULL;
	int x=0;
	char tvalue=FALSE,DefExpression=TRUE;

	readmatrix();
	applyrsrc=RDArsrcNEW(module,"APPLY MATRIX MENU");
	addstdrsrc(applyrsrc,"RANGE ON COLUMNS",BOOLNS,1,&tvalue,TRUE);
	CreateColumnList();
	x=0;
	addlstrsrc(applyrsrc,"FROM COLUMN LIST",&x,TRUE,NULL,columnlist->numlibs,&columnlist->libs,NULL);
	x=columnlist->numlibs-1;
	addlstrsrc(applyrsrc,"TO COLUMN LIST",&x,TRUE,NULL,columnlist->numlibs,&columnlist->libs,NULL);
	addstdrsrc(applyrsrc,"RANGE ON ROWS",BOOLNS,1,&tvalue,TRUE);
	CreateRowList();
	x=0;
	addlstrsrc(applyrsrc,"FROM ROW LIST",&x,TRUE,NULL,rowlist->numlibs,&rowlist->libs,NULL);
	x=rowlist->numlibs-1;
	addlstrsrc(applyrsrc,"TO ROW LIST",&x,TRUE,NULL,rowlist->numlibs,&rowlist->libs,NULL);
	addstdrsrc(applyrsrc,"RANGE ON CELLS",BOOLNS,1,&tvalue,TRUE);
	x=1;
	addstdrsrc(applyrsrc,"FROM CELL",LONGV,10,&x,TRUE);
	/* (JAB) Changing this to default the correct maximum */
	/*x=(DFV->num*DFV->xaxis.num*DFV->numdefs);*/
	x=(DFV->yaxis.num*DFV->xaxis.num);
	addstdrsrc(applyrsrc,"TO CELL",LONGV,10,&x,TRUE);
	if(cellvalues!=NULL) freeapplib(cellvalues);
	cellvalues=APPlibNEW();
	if(DFV->defs!=NULL)
	{
		for(x=0,c=DFV->defs;x<DFV->numdefs;++x,++c)
		{
			addAPPlib(cellvalues,c->name);
		}
	}
	if(cellvalues->numlibs<1)
	{
		addAPPlib(cellvalues,"No Cells");
	}
	x=0;
	addlstrsrc(applyrsrc,"CELL LIST",&x,TRUE,NULL,cellvalues->numlibs,&cellvalues->libs,NULL);
	FINDRSCSETFUNC(applyrsrc,"CELL LIST",setCellType,NULL);
	addstdrsrc(applyrsrc,"DEFAULT FORMULA",BOOLNS,1,&DefExpression,TRUE);
	FINDRSCSETFUNC(applyrsrc,"DEFAULT FORMULA",setDefExpression,NULL);
	addstdrsrc(applyrsrc,"ADD PERCENTAGE",BOOLNS,1,&AddPercentage,TRUE);
	addstdrsrc(applyrsrc,"ROUND DOLLARS",BOOLNS,1,&RoundDollars,TRUE);
	FINDRSCSETFUNC(applyrsrc,"ADD PERCENTAGE",setAddPercentage,NULL);
	addstdrsrc(applyrsrc,"PERCENTAGE",SDECIMALV,10,&Percentage,TRUE);
	addstdrsrc(applyrsrc,"ADD AMOUNT",BOOLNS,1,&AddAmount,TRUE);
	FINDRSCSETFUNC(applyrsrc,"ADD AMOUNT",setAddAmount,NULL);
	addstdrsrc(applyrsrc,"AMOUNT",DOLLARS,20,&Amount,TRUE);
	addsctrsrc(applyrsrc,"SELECT EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(applyrsrc,"LOAD VALUE SELECT",TRUE,loadvalue,"SELECT EXPRESSION");
	addbtnrsrc(applyrsrc,"LOAD CONTROL SELECT",TRUE,loadcontrol,"SELECT EXPRESSION");
	addbtnrsrc(applyrsrc,"LOAD OPERATOR SELECT",TRUE,loadoperand,"SELECT EXPRESSION");
	addbtnrsrc(applyrsrc,"LOAD GROUPER SELECT",TRUE,loadgrouper,"SELECT EXPRESSION");
	addsctrsrc(applyrsrc,"EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(applyrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(applyrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(applyrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(applyrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(applyrsrc,"SAVE",TRUE,saveapply,NULL);
	addbtnrsrc(applyrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfkwrsrc(applyrsrc,"QUIT",TRUE,quitapply,NULL);
	addbtnrsrc(applyrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(applyrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(DFV->locate_method==0) /* row and column */
	{
		/* FROM ROW LIST and TO ROW LIST shouldn't be editable/sensitive unless RANGE ON ROWS is true */
		FINDRSCSETFUNC(applyrsrc,"RANGE ON ROWS",changeRangeRows,NULL);
		/* FROM COLUMN LIST and TO COLUMN LIST shouldn't be editable/sensitive unless RANGE ON COLUMNS is true */
		FINDRSCSETFUNC(applyrsrc,"RANGE ON COLUMNS",changeRangeCols,NULL);
		FINDRSCSETEDITABLE(applyrsrc,"RANGE ON CELLS",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"FROM CELL",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"TO CELL",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"RANGE ON CELLS",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"FROM CELL",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"TO CELL",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"TO ROW LIST",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"TO COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"TO ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"TO COLUMN LIST",FALSE);
		changeRangeRows(applyrsrc);
		changeRangeCols(applyrsrc);
	} else { /* linear */
		/* FROM CELL and TO CELL shouldn't be editable/sensitive unless RANGE ON CELLS is true */
		FINDRSCSETFUNC(applyrsrc,"RANGE ON CELLS",changeRangeCells,NULL);
		FINDRSCSETEDITABLE(applyrsrc,"FROM CELL",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"TO CELL",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"FROM CELL",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"TO CELL",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"RANGE ON ROWS",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"TO ROW LIST",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"RANGE ON COLUMNS",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETEDITABLE(applyrsrc,"TO COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"RANGE ON ROWS",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"TO ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"RANGE ON COLUMNS",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(applyrsrc,"TO COLUMN LIST",FALSE);
		changeRangeCells(applyrsrc);
	}
	DefaultScreens(applyrsrc);
	setCellType(applyrsrc);
	if(ADVmakescrn(applyrsrc,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the APPLY MATRIX MENU.  Call your installer.",NULL,FALSE);
		quitapply(applyrsrc);
	}
}
static void quitmatrixfield(RDArsrc *frsrc)
{
	if(frsrc!=NULL) free_rsrc(frsrc);
}
static void savematrixfield(RDArsrc *frsrc)
{
	int rowx=0,colx=0;
	MATRIXrec *rec=NULL;
	MATRIXmem *mem=NULL;
	MATRIXvalue *v=NULL;
	MATRIXcell *c=NULL;
	int x,resnum=(-1);
	RDArmem *res=NULL;
	char tvalue=FALSE,svalue=FALSE,xtemp[101];

	readallwidgets(frsrc);
	FINDRSCGETINT(frsrc,"ROW",&rowx);
	FINDRSCGETINT(frsrc,"COL",&colx);
	FINDRSCGETCHAR(frsrc,"SENSITIVE",&svalue);
	if(DFV->recs!=NULL && DFV->num>(rowx-1))
	{
		rec=DFV->recs+(rowx-1);
		if(rec->mems!=NULL && rec->num>(colx-1))
		{
			mem=rec->mems+(colx-1);
			mem->sensitive=svalue;
			if(mem->values!=NULL && DFV->defs!=NULL)
			{
				for(x=0,v=mem->values,c=DFV->defs;x<mem->num;
					++x,++v,++c)
				{
					if(!DFV->locate_method)
					{
						sprintf(xtemp,"R%dC%dV%d",rowx,colx,x+1);
					} else {
						sprintf(xtemp,"CELL %d VALUE %d",
							(((rowx-1)*rec->num)+colx),x+1);
					}
					resnum=FINDRSC(frsrc,c->name);
					if(resnum>(-1))
					{
					res=frsrc->rscs+resnum;
					switch(c->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(res->value.string_value!=NULL)
							{
								unpad(res->value.string_value);
								QUICKALLOC(v->data.string_value,v->dlen,res->dlen);
								memcpy(v->data.string_value,res->value.string_value,res->dlen);
								if(v->dlen>c->len && c->len>0)
								{
									v->data.string_value[c->len]=0;
								}
							} else if(v->dlen>0)
							{
								memset(v->data.string_value,0,v->dlen+1);
							} else {
								v->data.string_value=NULL;
								v->dlen=0;
							}
							FINDRSCSETSTRING(MAINRSRC,xtemp,v->data.string_value);
							break;
						case BOOLNS:
						case CHARACTERS:
							*v->data.string_value=*res->value.string_value;
							tvalue=*v->data.string_value;
							if(c->displayed) FINDRSCSETCHAR(MAINRSRC,xtemp,tvalue);
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							*v->data.float_value=*res->value.float_value;
							if(c->displayed) FINDRSCSETDOUBLE(MAINRSRC,xtemp,*v->data.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							*v->data.short_value=*res->value.short_value;
							if(c->displayed) FINDRSCSETSHORT(MAINRSRC,xtemp,*v->data.short_value);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							*v->data.integer_value=*res->value.integer_value;
							if(c->displayed) FINDRSCSETINT(MAINRSRC,xtemp,*v->data.integer_value);
							break;
						case BUTTONS:
							prterr("Error call RaddogTiddog cause this is weird....");
							break;
						default:
							prterr("Error field type [%d] invalid for Cell Value # [%d] on Table [%s] [%s].",c->type,x,DFV->module,DFV->name);
							break;
					}
					if(c->displayed) updatersrc(MAINRSRC,xtemp);
					}
					if(c->displayed)
					{
						if(!mem->sensitive)
						{
							FINDRSCSETEDITABLE(MAINRSRC,xtemp,FALSE);
							FINDRSCSETSENSITIVE(MAINRSRC,xtemp,FALSE);
						} else {
							FINDRSCSETEDITABLE(MAINRSRC,xtemp,TRUE);
							FINDRSCSETSENSITIVE(MAINRSRC,xtemp,TRUE);
						}
					}
				}
			}
		}
	}
	quitmatrixfield(frsrc);
}
static void editmatrixfield(RDArmem *member)
{
	RDArsrc *frsrc=NULL;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXvalue *v;
	MATRIXcell *c;
	int x,rowx=0,colx=0,numx=0;
	char *temp=NULL,Tvalue=TRUE;
	double dnum=0.0,dnum1=0.0,dnum2=0.0;
	unsigned flen=0;

	readmatrix();
	frsrc=RDArsrcNEW(module,"EDIT MATRIX FIELD");
	if(!DFV->locate_method)
	{
		temp=member->rscrname+1;
		rowx=Ratoi(temp);
		temp=strchr(member->rscrname,'C');
		++temp;
		colx=Ratoi(temp);
	} else {
		temp=member->rscrname+11;
		numx=Ratoi(temp);
		if(numx>DFV->xaxis.num)
		{
			dnum1=numx;
			dnum2=DFV->xaxis.num;
			dnum=fmod(dnum1,dnum2);
/*lint -e524 */
			if(dnum>0)
			{
				rowx=(dnum1-dnum)/DFV->xaxis.num+1;
				colx=dnum;
			} else {
				rowx=(dnum1-dnum)/DFV->xaxis.num;
				colx=DFV->xaxis.num;
			}
/*lint +e524 */
		} else {
			rowx=1;
			colx=numx;
		}
	}
	addstdrsrc(frsrc,"ROW",LONGV,8,&rowx,FALSE);
	addstdrsrc(frsrc,"COL",LONGV,8,&colx,FALSE);
	if(DFV->defs!=NULL)
	{
		if((rowx-1)<DFV->num && (colx-1)<DFV->xaxis.num)
		{
			rec=DFV->recs+(rowx-1);
			mem=rec->mems+(colx-1);
			Tvalue=mem->sensitive;
			addstdrsrc(frsrc,"SENSITIVE",BOOLNS,1,&Tvalue,TRUE);
			for(x=0,v=mem->values,c=DFV->defs;x<DFV->numdefs;++x,++v,++c)
			{
				flen=c->len;
				switch(c->type)
				{
					case SCROLLEDTEXT:
						addsctrsrc(frsrc,c->name,flen,
							v->data.string_value,TRUE);
						break;
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						addstdrsrc(frsrc,c->name,c->type,flen,
							v->data.string_value,TRUE);
						break;
					case BOOLNS:
					case CHARACTERS:
						addstdrsrc(frsrc,c->name,c->type,flen,
							v->data.string_value,TRUE);
						break;
					case DOLLARS:
						addstdrsrc(frsrc,c->name,c->type,flen,
							v->data.float_value,TRUE);
						break;
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						addstdrsrc(frsrc,c->name,c->type,flen,
							v->data.float_value,TRUE);
						break;
					case SHORTV:
					case SSHORTV:
						addstdrsrc(frsrc,c->name,c->type,flen,
							v->data.short_value,TRUE);
						break;
					case LONGV:
					case SLONGV:
						addstdrsrc(frsrc,c->name,c->type,flen,
							v->data.integer_value,TRUE);
						break;
					case SCROLLEDLIST:
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for cell # [%d] on Table [%s] [%s].",c->type,x,DFV->module,DFV->name);
						break;
				}
			}
		}
	}
	addrfkwrsrc(frsrc,"SAVE",TRUE,savematrixfield,NULL);
	addrfkwrsrc(frsrc,"QUIT",TRUE,quitmatrixfield,NULL);
	addbtnrsrc(frsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(frsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	frsrc->scn=RDAscrnNEW(frsrc->module,frsrc->screen);
	addwdgt(frsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,5,NULL,"Row:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,0,"ROW","Row",NULL,NULL,0,10,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,5,NULL,"Column:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,0,"COL","Column",NULL,NULL,0,10,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(DFV->defs!=NULL)
	{
		if((rowx-1)<DFV->num && (colx-1)<DFV->xaxis.num)
		{
		addwdgt(frsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(frsrc->scn,9,"SENSITIVE","Sensitive",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(frsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		for(x=0,c=DFV->defs;x<DFV->numdefs;++x,++c)
		{
			addwdgt(frsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			switch(c->type)
			{
				case SCROLLEDTEXT:
					addwdgt(frsrc->scn,5,NULL,c->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,8,c->name,c->name,NULL,NULL,5,80,0,
						NULL,NULL,NULL);
					break;
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					addwdgt(frsrc->scn,5,NULL,c->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,0,c->name,c->name,NULL,NULL,0,c->len,
						0,NULL,NULL,NULL);
					break;
				case BOOLNS:
					addwdgt(frsrc->scn,9,c->name,c->name,NULL,NULL,0,c->len,
						0,NULL,NULL,NULL);
					break;
				case CHARACTERS:
					addwdgt(frsrc->scn,5,NULL,c->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,0,c->name,c->name,NULL,NULL,0,c->len,
						0,NULL,NULL,NULL);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					addwdgt(frsrc->scn,5,NULL,c->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,0,c->name,c->name,NULL,NULL,0,c->len,
						0,NULL,NULL,NULL);
					break;
				case SHORTV:
				case SSHORTV:
					addwdgt(frsrc->scn,5,NULL,c->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,0,c->name,c->name,NULL,NULL,0,c->len,
						0,NULL,NULL,NULL);
					break;
				case LONGV:
				case SLONGV:
					addwdgt(frsrc->scn,5,NULL,c->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(frsrc->scn,0,c->name,c->name,NULL,NULL,0,c->len,
						0,NULL,NULL,NULL);
					break;
				case SCROLLEDLIST:
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] invalid for Cell Value # [%d] on Table [%s] [%s].",c->type,x,DFV->module,DFV->name);
					break;
			}
			addwdgt(frsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		}
	}
	addwdgt(frsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,6,"SAVE","Save",NULL,NULL,0,0,24,NULL,NULL,NULL);
	addwdgt(frsrc->scn,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(frsrc->scn,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(frsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(frsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(ADVmakescrn(frsrc,TRUE))
	{
		ERRORDIALOG("Cannot Find Screen","Error attempting to find the EDIT MATRIX FIELD.  Call your installer.",NULL,FALSE);
		quitmatrixfield(frsrc);
	}
}
void readmatrix()
{
	RDArmem *res=NULL;
	AXISmem *am;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXvalue *v;
	MATRIXcell *c;
	int x,y,z,resnum=(-1);

	readallwidgets(MAINRSRC);
	FINDRSCGETSTRING(MAINRSRC,"MODULE",&DFV->module);
	FINDRSCGETSTRING(MAINRSRC,"NAME",&DFV->name);
	FINDRSCGETSTRING(MAINRSRC,"TITLE 1",&DFV->title1);
	FINDRSCGETSTRING(MAINRSRC,"TITLE 2",&DFV->title2);
	FINDRSCGETSTRING(MAINRSRC,"DESCRIPTION",&DFV->desc);
	if(DFV->xaxis.mem!=NULL)
	{
		for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
		{
			if(DFV->xaxis.print_title1)
			{
				memset(stemp,0,101);
				sprintf(stemp,"X-AXIS %d TITLE 1",x+1);
				FINDRSCGETSTRING(MAINRSRC,stemp,&am->title1);
			}
			if(DFV->xaxis.print_title2)
			{
			memset(stemp,0,101);
			sprintf(stemp,"X-AXIS %d TITLE 2",x+1);
			FINDRSCGETSTRING(MAINRSRC,stemp,&am->title2);
			if(DFV->xaxis.axis_type!=5)
			{
			memset(stemp,0,101);
			sprintf(stemp,"X-AXIS %d",x+1);
			resnum=FINDRSC(MAINRSRC,stemp);
			if(resnum>(-1))
			{
				res=MAINRSRC->rscs+resnum;
				switch(DFV->xaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						if(res->value.string_value!=NULL)
						{
							unpad(res->value.string_value);
							QUICKALLOC(am->data.string_value,am->dlen,res->dlen);
							memcpy(am->data.string_value,res->value.string_value,res->dlen);
							if(am->dlen>DFV->xaxis.len && DFV->xaxis.len>0)
							{
								am->data.string_value[DFV->xaxis.len]=0;
							}
						} else if(am->dlen>0)
						{
							memset(am->data.string_value,0,am->dlen+1);
						} else {
							am->data.string_value=NULL;
							am->dlen=0;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						*am->data.string_value=*res->value.string_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*am->data.float_value=*res->value.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						*am->data.short_value=*res->value.short_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						*am->data.integer_value=*res->value.integer_value;
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for [%s] on Table [%s] [%s].",DFV->xaxis.type,stemp,DFV->module,DFV->name);
						break;
				}
			}
			}
			}
		}
	}
	if(DFV->yaxis.mem!=NULL)
	{
		for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
		{
			if(DFV->yaxis.print_title1)
			{
				memset(stemp,0,101);
				sprintf(stemp,"Y-AXIS %d TITLE 1",x+1);
				FINDRSCGETSTRING(MAINRSRC,stemp,&am->title1);
			}
			if(DFV->yaxis.print_title2)
			{
			memset(stemp,0,101);
			sprintf(stemp,"Y-AXIS %d TITLE 2",x+1);
			FINDRSCGETSTRING(MAINRSRC,stemp,&am->title2);
			if(DFV->yaxis.axis_type!=5)
			{
			memset(stemp,0,101);
			sprintf(stemp,"Y-AXIS %d",x+1);
			resnum=FINDRSC(MAINRSRC,stemp);
			if(resnum>(-1))
			{
				res=MAINRSRC->rscs+resnum;
				switch(DFV->yaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						if(res->value.string_value!=NULL)
						{
							unpad(res->value.string_value);
							QUICKALLOC(am->data.string_value,am->dlen,res->dlen);
							memcpy(am->data.string_value,res->value.string_value,res->dlen);
							if(am->dlen>DFV->yaxis.len && DFV->yaxis.len>0)
							{
								am->data.string_value[DFV->yaxis.len]=0;
							}
						} else if(am->dlen>0)
						{
							memset(am->data.string_value,0,am->dlen+1);
						} else {
							am->data.string_value=NULL;
							am->dlen=0;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						*am->data.string_value=*res->value.string_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*am->data.float_value=*res->value.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						*am->data.short_value=*res->value.short_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						*am->data.integer_value=*res->value.integer_value;
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for [%s] on Table [%s] [%s].",DFV->yaxis.type,stemp,DFV->module,DFV->name);
						break;
				}
			}
			}
			}
		}
	}
	if(DFV->recs!=NULL)
	{
		for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
		{
			if(rec->mems!=NULL)
			{
				for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
				{
					if(mem->sensitive)
					{
					if(mem->values!=NULL && DFV->defs!=NULL)
					{
						for(z=0,v=mem->values;z<mem->num;++z,++v)
						{
							c=DFV->defs+z;
							if(c->displayed)
							{
							if(!DFV->locate_method)
							{
								sprintf(stemp,"R%dC%dV%d",x+1,y+1,z+1);
							} else {
								sprintf(stemp,"CELL %d VALUE %d",
									((x*rec->num)+y+1),z+1);
							}
							resnum=FINDRSC(MAINRSRC,stemp);
							if(resnum>(-1))
							{
							res=MAINRSRC->rscs+resnum;
							switch(c->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case ZIPCODE:
									if(res->value.string_value!=NULL)
									{
										unpad(res->value.string_value);
										QUICKALLOC(v->data.string_value,v->dlen,res->dlen);
										memcpy(v->data.string_value,res->value.string_value,res->dlen);
										if(v->dlen>c->len && c->len>0)
										{
											v->data.string_value[c->len]=0;
										}
									} else if(v->dlen>0 && v->data.string_value!=NULL)
									{
										memset(v->data.string_value,0,v->dlen+1);
									} else {
										v->data.string_value=NULL;
										v->dlen=0;
									}
									break;
								case BOOLNS:
								case CHARACTERS:
									*v->data.string_value=*res->value.string_value;
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									*v->data.float_value=*res->value.float_value;
									break;
								case SHORTV:
								case SSHORTV:
									*v->data.short_value=*res->value.short_value;
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									*v->data.integer_value=*res->value.integer_value;
									break;
								case BUTTONS:
									prterr("Error call RaddogTiddog cause this is weird....");
									break;
								default:
									prterr("Error field type [%d] invalid for Cell Value # [%d] on Table [%s] [%s].",c->type,z,DFV->module,DFV->name);
									break;
							}
							}
							}
						}
					}
					}
				}
			}
		}
	}
}
static RDArsrc *GenerateMatrixRDArsrc()
{
	RDArsrc *newrsrc=NULL;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXvalue *v;
	AXISmem *am;
	MATRIXcell *cell;
	int x,y,z;

	newrsrc=RDArsrcNEW(module,"EDIT MATRIX");
	addbtnrsrc(newrsrc,"MATRIX",TRUE,matrix_menu,NULL);
	addbtnrsrc(newrsrc,"SETUP",TRUE,setup_menu,NULL);
	addbtnrsrc(newrsrc,"SAVE",TRUE,savematrix,NULL);
	addbtnrsrc(newrsrc,"APPLY",TRUE,apply_menu,NULL);
	addbtnrsrc(newrsrc,"EDIT",TRUE,edit_menu,NULL);
	addbtnrsrc(newrsrc,"PRINT",TRUE,printmatrix,NULL);
	addbtnrsrc(newrsrc,"WINDOW",TRUE,window_menu,NULL);
	addbtnrsrc(newrsrc,"QUIT",TRUE,quitmatrixtest,NULL);
	addbtnrsrc(newrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(newrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addstdrsrc(newrsrc,"MODULE",VARIABLETEXT,15,(DFV->module!=NULL ?
		DFV->module:""),TRUE);
	if(DFV->defs==NULL)
	{
		FINDRSCSETEDITABLE(newrsrc,"EDIT",FALSE);
		FINDRSCSETSENSITIVE(newrsrc,"EDIT",FALSE);
		FINDRSCSETEDITABLE(newrsrc,"APPLY",FALSE);
		FINDRSCSETSENSITIVE(newrsrc,"APPLY",FALSE);
	}
	if(DFV->xaxis.mem==NULL || DFV->yaxis.mem==NULL)
	{
		FINDRSCSETEDITABLE(newrsrc,"APPLY",FALSE);
		FINDRSCSETSENSITIVE(newrsrc,"APPLY",FALSE);
	}
	addstdrsrc(newrsrc,"NAME",VARIABLETEXT,40,(DFV->name!=NULL ? DFV->name:""),
		TRUE);
	addstdrsrc(newrsrc,"TITLE 1",VARIABLETEXT,0,(DFV->title1!=NULL ?
		DFV->title1:""),TRUE);
	addstdrsrc(newrsrc,"TITLE 2",VARIABLETEXT,0,(DFV->title2!=NULL ?
		DFV->title2:""),TRUE);
	addsctrsrc(newrsrc,"DESCRIPTION",0,(DFV->desc!=NULL ? DFV->desc:""),TRUE);
	if(DFV->xaxis.mem!=NULL)
	{
		for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
		{
			memset(stemp,0,101);
			sprintf(stemp,"X-AXIS %d TITLE 1",x+1);
			RMEMaddstdrsrc(newrsrc,stemp,VARIABLETEXT,
				0,(am->title1!=NULL ? am->title1:""),TRUE);
			memset(stemp,0,101);
			sprintf(stemp,"X-AXIS %d TITLE 2",x+1);
			RMEMaddstdrsrc(newrsrc,stemp,VARIABLETEXT,
				0,(am->title2!=NULL ? am->title2:""),TRUE);
			if(DFV->xaxis.axis_type!=5)
			{
			memset(stemp,0,101);
			sprintf(stemp,"X-AXIS %d",x+1);
			switch(DFV->xaxis.type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->xaxis.type,
						DFV->xaxis.len,am->data.string_value,TRUE);
					break;
				case BOOLNS:
				case CHARACTERS:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->xaxis.type,
						DFV->xaxis.len,am->data.string_value,TRUE);
					break;
				case DOLLARS:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->xaxis.type,
						DFV->xaxis.len,am->data.float_value,TRUE);
					break;
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->xaxis.type,
						DFV->xaxis.len,am->data.float_value,TRUE);
					break;
				case SHORTV:
				case SSHORTV:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->xaxis.type,
						DFV->xaxis.len,am->data.short_value,TRUE);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->xaxis.type,
						DFV->xaxis.len,am->data.integer_value,TRUE);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] invalid for Axis Member [%d] on Table [%s] [%s].",DFV->xaxis.type,x,DFV->module,DFV->name);
					break;
			}
			}
		}
	}
	if(DFV->yaxis.mem!=NULL)
	{
		for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
		{
			memset(stemp,0,101);
			sprintf(stemp,"Y-AXIS %d TITLE 1",x+1);
			RMEMaddstdrsrc(newrsrc,stemp,VARIABLETEXT,
				0,(am->title1!=NULL ? am->title1:""),TRUE);
			memset(stemp,0,101);
			sprintf(stemp,"Y-AXIS %d TITLE 2",x+1);
			RMEMaddstdrsrc(newrsrc,stemp,VARIABLETEXT,
				0,(am->title2!=NULL ? am->title2:""),TRUE);
			if(DFV->yaxis.axis_type!=5)
			{
			memset(stemp,0,101);
			sprintf(stemp,"Y-AXIS %d",x+1);
			switch(DFV->yaxis.type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->yaxis.type,
						DFV->yaxis.len,am->data.string_value,TRUE);
					break;
				case BOOLNS:
				case CHARACTERS:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->yaxis.type,
						DFV->yaxis.len,am->data.string_value,TRUE);
					break;
				case DOLLARS:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->yaxis.type,
						DFV->yaxis.len,am->data.float_value,TRUE);
					break;
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->yaxis.type,
						DFV->yaxis.len,am->data.float_value,TRUE);
					break;
				case SHORTV:
				case SSHORTV:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->yaxis.type,
						DFV->yaxis.len,am->data.short_value,TRUE);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					RMEMaddstdrsrc(newrsrc,stemp,DFV->yaxis.type,
						DFV->yaxis.len,am->data.integer_value,TRUE);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] invalid for Axis Member [%d] on Table [%s] [%s].",DFV->yaxis.type,x,DFV->module,DFV->name);
					break;
			}
			}
		}
	}
	if(DFV->recs!=NULL)
	{
		for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
		{
			for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
			{
				if(DFV->cell_config>0)
				{
					memset(stemp,0,101);
					if(!DFV->locate_method)
					{
						sprintf(stemp,"R%dC%d BUTTON",x+1,y+1);
					} else {
						sprintf(stemp,"CELL BUTTON %d",((x*rec->num)+y+1));
					}
					RMEMaddbtnrsrc(newrsrc,stemp,TRUE,editmatrixfield,NULL);
				}
				for(z=0,v=mem->values;z<mem->num;++z,++v)
				{
					cell=DFV->defs+z;
					if(cell->displayed)
					{
						memset(stemp,0,101);
						if(!DFV->locate_method)
						{
							sprintf(stemp,"R%dC%dV%d",x+1,y+1,z+1);
						} else {
							sprintf(stemp,"CELL %d VALUE %d",
								((x*rec->num)+y+1),z+1);
						}
						switch(cell->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								RMEMaddstdrsrc(newrsrc,stemp,cell->type,
									cell->len,v->data.string_value,TRUE);
								break;
							case BOOLNS:
							case CHARACTERS:
								RMEMaddstdrsrc(newrsrc,stemp,cell->type,
									cell->len,v->data.string_value,TRUE);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								RMEMaddstdrsrc(newrsrc,stemp,cell->type,
									cell->len,v->data.float_value,TRUE);
								break;
							case SHORTV:
							case SSHORTV:
								RMEMaddstdrsrc(newrsrc,stemp,cell->type,
									cell->len,v->data.short_value,TRUE);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								RMEMaddstdrsrc(newrsrc,stemp,cell->type,
									cell->len,v->data.integer_value,TRUE);
								break;
							case BUTTONS:
								prterr("Error call RaddogTiddog cause this is weird....");
								break;
							default:
								prterr("Error field type [%d] invalid for Cell Value # [%d] on Table [%s] [%s].",cell->type,z,DFV->module,DFV->name);
								break;
						}
						if(!mem->sensitive)
						{
							FINDRSCSETEDITABLE(newrsrc,stemp,FALSE);
							FINDRSCSETSENSITIVE(newrsrc,stemp,FALSE);
						}
					}
				}
			}
		}
	}
	GenerateMatrixRDAscrn(newrsrc);
	return(newrsrc);
}
static void GenerateMatrixRDAscrn(RDArsrc *newrsrc)
{
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXcell *c;
	AXISmem *am;
	int x,y,z;
	short dcount=0,Ylen1=0,Ylen2=0,Xlen=0,len=0,xcount=0,ycount=0;

	if(newrsrc->scn!=NULL) free_scrn(newrsrc->scn);
	newrsrc->scn=RDAscrnNEW(newrsrc->module,newrsrc->screen);
/* START MODULE MATRIX LINE */
/* New Line */
	addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* Module Label */
	addwdgt(newrsrc->scn,5,NULL,"Module:",NULL,NULL,0,0,0,NULL,NULL,NULL);
/* Module Input */
	addwdgt(newrsrc->scn,0,"MODULE","Module",NULL,NULL,0,15,0,NULL,NULL,NULL);
/* Name Label */
	addwdgt(newrsrc->scn,5,NULL,"Name:",NULL,NULL,0,0,0,NULL,NULL,NULL);
/* Name Input */
	addwdgt(newrsrc->scn,0,"NAME","Name",NULL,NULL,0,40,0,NULL,NULL,NULL);
/* New Box */
	addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* New Line */
	addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* Title / Description Optional Screen */
	addwdgt(newrsrc->scn,14,"EDIT MATRIX TITLES","Title/Desc",NULL,NULL,
		0,0,0,NULL,NULL,NULL);
/* End Line */
	addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* End Box */
	addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* Blank Label */
	addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* End Line */
	addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* END MODULE MATRIX LINE */
/* START COMMAND BOX */
/* New Line */
	addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* New Box */
	addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* New Line */
	addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* Matrix Menu Button */
	addwdgt(newrsrc->scn,6,"MATRIX","Matrix",NULL,NULL,0,0,36,NULL,
		NULL,NULL);
/* Setup Menu Button */
	addwdgt(newrsrc->scn,6,"SETUP","Setup",NULL,NULL,0,0,35,NULL,NULL,NULL);
/* Save Button */
	addwdgt(newrsrc->scn,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
/* Apply Button */
	addwdgt(newrsrc->scn,6,"APPLY","Apply",NULL,NULL,0,0,34,NULL,NULL,NULL);
/* Edit Button */
	addwdgt(newrsrc->scn,6,"EDIT","Edit",NULL,NULL,0,0,13,NULL,NULL,NULL);
/* Quit Button */
	addwdgt(newrsrc->scn,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
/* Print Button */
	addwdgt(newrsrc->scn,6,"PRINT","Print",NULL,NULL,0,0,7,NULL,NULL,NULL);
/* Window Button */
	addwdgt(newrsrc->scn,6,"WINDOW","Window",NULL,NULL,0,0,29,NULL,NULL,NULL);
/* Help Button */
	addwdgt(newrsrc->scn,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
/* End Line */
	addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* End Box */
	addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* End Line */
	addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* END COMMAND BOX */
	dcount=0;
	if(DFV->defs!=NULL)
	{
		for(x=0,c=DFV->defs;x<DFV->numdefs;++x,++c)
		{
			if(c->displayed) ++dcount;
		}
	}
	if(DFV->recs!=NULL && DFV->xaxis.mem!=NULL && DFV->yaxis.mem!=NULL &&
		dcount>0)
	{
/* START SCROLLED WINDOW LINE */
/* New Line */
		addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* New Scrolled Window */
		addwdgt(newrsrc->scn,12,NULL,NULL,NULL,NULL,400,700,0,NULL,NULL,NULL);
		addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* New Line */
		addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		ycount=0;
		if(DFV->yaxis.print_title1) ++ycount;
		if(DFV->yaxis.print_title2) ++ycount;
		xcount=0;
		if(DFV->xaxis.print_title1) ++xcount;
		if(DFV->xaxis.print_title2) ++xcount;
/* Row Names / Titles */
/* New Box */
		if(xcount<=dcount && ycount<=dcount)
		{
			if(DFV->yaxis.mem!=NULL)
			{
				len=0;Ylen1=0;Ylen2=0;
				for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
				{
					if(am->title1!=NULL)  len=RDAstrlen(am->title1);
						else len=0;
					if(len>Ylen1) Ylen1=len;
					if(am->title2!=NULL) len=RDAstrlen(am->title2);
					else len=0;
					if(len>Ylen2) Ylen2=len;
				}
				if(Ylen1==0) Ylen1=5;
				if(Ylen2==0) Ylen2=5;
			}
/* New Box */
			addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			len=0;Xlen=0;
			for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
			{
				if(am->title1!=NULL) len=RDAstrlen(am->title1);
					else len=0;
				if(len>Xlen) Xlen=len;
				if(DFV->xaxis.axis_type!=5)
				{
					if(am->title2!=NULL) len=RDAstrlen(am->title2);
						else len=0;
					if(len>Xlen) Xlen=len;
				}
			}
			if(Xlen==0) Xlen=5;
/* New Line */
			addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(ycount>0)
			{
				addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				for(x=0,c=DFV->defs;x<DFV->numdefs;++x,++c)
				{
					if(c->displayed)
					{
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					}
				}
				addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
			{
				addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				y=xcount;
				while(y<dcount)
				{
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					++y;
				}
				if(DFV->xaxis.print_title1)
				{
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					memset(stemp,0,101);
					sprintf(stemp,"X-AXIS %d TITLE 1",x+1);
					addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Xlen,0,
						NULL,NULL,NULL);
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
				}
				if(DFV->xaxis.print_title2)
				{
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					if(DFV->xaxis.axis_type<5)
					{
						memset(stemp,0,101);
						sprintf(stemp,"X-AXIS %d",x+1);
						switch(DFV->xaxis.type)
						{
							case SCROLLEDTEXT:
								addwdgt(newrsrc->scn,8,stemp,stemp,NULL,NULL,
									5,80,0,NULL,NULL,NULL);
								break;
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,
									0,DFV->xaxis.len,0,NULL,NULL,NULL);
								break;
							case BOOLNS:
								addwdgt(newrsrc->scn,9,stemp,(am->title2!=NULL ?
									am->title2:stemp),NULL,NULL,0,DFV->xaxis.len,
									0,NULL,NULL,NULL);
								break;
							case CHARACTERS:
								addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
									DFV->xaxis.len,0,NULL,NULL,NULL);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
									DFV->xaxis.len,0,NULL,NULL,NULL);
								break;
							case SHORTV:
							case SSHORTV:
								addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
									DFV->xaxis.len,0,NULL,NULL,NULL);
								break;
							case LONGV:
							case SLONGV:
								addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
									DFV->xaxis.len,0,NULL,NULL,NULL);
								break;
							case SCROLLEDLIST:
							case BUTTONS:
								prterr("Error call RaddogTiddog cause this is weird....");
								break;
							default:
								prterr("Error field type [%d] invalid for X-Axis# [%d] on Table [%s] [%s].",DFV->xaxis.type,x,DFV->module,DFV->name);
								break;
						}
					} else {
						memset(stemp,0,101);
						sprintf(stemp,"X-AXIS %d TITLE 2",x+1);
						addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Xlen,0,
							NULL,NULL,NULL);
					}
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
				}
				addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
			}
			addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* End Line */
			if(DFV->recs!=NULL)
			{
				for(x=0,am=DFV->yaxis.mem,rec=DFV->recs;x<DFV->num;++x,++rec,
					++am)
				{
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					if(ycount)
					{
						addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
						if(DFV->yaxis.print_title1)
						{
							addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
							memset(stemp,0,101);
							sprintf(stemp,"Y-AXIS %d TITLE 1",x+1);
							addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Ylen1,0,
								NULL,NULL,NULL);
							addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
						}
						if(DFV->yaxis.print_title2)
						{
							addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
							if(DFV->yaxis.axis_type<5)
							{
								memset(stemp,0,101);
								sprintf(stemp,"Y-AXIS %d",x+1);
								switch(DFV->yaxis.type)
								{
									case SCROLLEDTEXT:
										addwdgt(newrsrc->scn,8,stemp,stemp,NULL,NULL,
											5,80,0,NULL,NULL,NULL);
										break;
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,
											0,DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case BOOLNS:
										addwdgt(newrsrc->scn,9,stemp,(am->title2!=NULL ?
											am->title2:stemp),NULL,NULL,0,DFV->yaxis.len,
											0,NULL,NULL,NULL);
										break;
									case CHARACTERS:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case SHORTV:
									case SSHORTV:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case LONGV:
									case SLONGV:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case SCROLLEDLIST:
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										break;
									default:
										prterr("Error field type [%d] invalid for Y-Axis# [%d] on Table [%s] [%s].",DFV->yaxis.type,x,DFV->module,DFV->name);
										break;
								}
							} else {
								memset(stemp,0,101);
								sprintf(stemp,"Y-AXIS %d TITLE 2",x+1);
								addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Ylen2,0,NULL,NULL,NULL);
							}
							addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
							y=ycount;
							while(y<dcount)
							{
								addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
									NULL,NULL,NULL);
								addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
									NULL,NULL,NULL);
								addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
									NULL,NULL,NULL);
								++y;
							}
						}
						addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
					}
					if(rec->mems!=NULL)
					{
						for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
						{
							addwdgt(newrsrc->scn,10,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							if(mem->values!=NULL)
							{
								addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								for(z=0,c=DFV->defs;z<mem->num;++z,++c)
								{
									if(c->displayed)
									{
										addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,
											0,0,0,NULL,NULL,NULL);
										if(!z && DFV->cell_config>0)
										{
											memset(stemp,0,101);
											if(!DFV->locate_method)
											{
												sprintf(stemp,"R%dC%d BUTTON",x+1,y+1);
											} else {
												sprintf(stemp,"CELL BUTTON %d",((x*rec->num)+y+1));
											}
											addwdgt(newrsrc->scn,6,stemp,stemp,NULL,NULL,0,0,1,NULL,NULL,NULL);
										}
										memset(stemp,0,101);
										if(!DFV->locate_method)
										{
											sprintf(stemp,"R%dC%dV%d",x+1,y+1,z+1);
										} else {
											sprintf(stemp,"CELL %d VALUE %d",
												((x*rec->num)+y+1),z+1);
										}
										switch(c->type)
										{
											case SCROLLEDTEXT:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,5,80,0,NULL,NULL,NULL);
												break;
											case VARIABLETEXT:
											case DATES:
											case TIMES:
											case SOCSECNUM:
											case PLAINTEXT:
											case PHONE:
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
											case ZIPCODE:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case BOOLNS:
												addwdgt(newrsrc->scn,9,stemp,c->name,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case CHARACTERS:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case SHORTV:
											case SSHORTV:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case LONGV:
											case SLONGV:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case SCROLLEDLIST:
											case BUTTONS:
												prterr("Error call RaddogTiddog cause this is weird....");
												break;
											default:
												prterr("Error field type [%d] invalid for X-Axis# [%d] on Table [%s] [%s].",c->type,z,DFV->module,DFV->name);
												break;
										}
										addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,
											0,0,0,NULL,NULL,NULL);
									}
								}
								addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							} else {
/* Avoiding an Empty Frame */
								addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,
									0,0,0,NULL,NULL,NULL);
							}
						}
					}
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
				}
			}
/* End Box */
			addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		} else {
			addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(xcount<=dcount)
			{
				addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
			}
			if(DFV->xaxis.print_title1)
			{
/* New Line */
				addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
/* Blank Label */
				addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
				if(ycount>dcount)
				{
					addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
				}
/* End Line */
				addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
			}
			if(DFV->xaxis.print_title2)
			{
/* New Line */
				addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
/* Blank Label */
				addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
				if(ycount>dcount)
				{
					addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
				}
/* End Line */
				addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
			}
			if(xcount<=dcount)
			{
				addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,
					NULL,NULL,NULL);
			}
			if(DFV->yaxis.mem!=NULL)
			{
				len=0;Ylen1=0;Ylen2=0;
				for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
				{
					if(am->title1!=NULL)  len=RDAstrlen(am->title1);
						else len=0;
					if(len>Ylen1) Ylen1=len;
					if(am->title2!=NULL) len=RDAstrlen(am->title2);
					else len=0;
					if(len>Ylen2) Ylen2=len;
				}
				if(Ylen1==0) Ylen1=5;
				if(Ylen2==0) Ylen2=5;
				if(ycount<=dcount)
				{
					for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
					{
						addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
						addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
						if(DFV->yaxis.print_title1)
						{
							addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
							memset(stemp,0,101);
							sprintf(stemp,"Y-AXIS %d TITLE 1",x+1);
							addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Ylen1,0,
								NULL,NULL,NULL);
							addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
						}
						if(DFV->yaxis.print_title2)
						{
							addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
							if(DFV->yaxis.axis_type==5)
							{
								memset(stemp,0,101);
								sprintf(stemp,"Y-AXIS %d TITLE 2",x+1);
								addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Ylen2,0,NULL,NULL,NULL);
							} else {
								memset(stemp,0,101);
								sprintf(stemp,"Y-AXIS %d",x+1);
								switch(DFV->yaxis.type)
								{
									case SCROLLEDTEXT:
										addwdgt(newrsrc->scn,8,stemp,stemp,NULL,NULL,
											5,80,0,NULL,NULL,NULL);
										break;
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,
											0,DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case BOOLNS:
										addwdgt(newrsrc->scn,9,stemp,(am->title2!=NULL ?
											am->title2:stemp),NULL,NULL,0,DFV->yaxis.len,
											0,NULL,NULL,NULL);
										break;
									case CHARACTERS:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case SHORTV:
									case SSHORTV:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case LONGV:
									case SLONGV:
										addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
											DFV->yaxis.len,0,NULL,NULL,NULL);
										break;
									case SCROLLEDLIST:
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										break;
									default:
										prterr("Error field type [%d] invalid for Y-Axis# [%d] on Table [%s] [%s].",DFV->yaxis.type,x,DFV->module,DFV->name);
										break;
								}
							}
							addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
							y=ycount;
							while(y<dcount)
							{
								addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
									NULL,NULL,NULL);
								addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,
									NULL,NULL,NULL);
								addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
									NULL,NULL,NULL);
								++y;
							}
							addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
							addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
								NULL,NULL,NULL);
						}
					}
				} else {
					for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
					{
						addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
						if(DFV->yaxis.print_title1)
						{
							memset(stemp,0,101);
							sprintf(stemp,"Y-AXIS %d TITLE 1",x+1);
							addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Ylen1,0,
								NULL,NULL,NULL);
						}
						if(DFV->yaxis.print_title2)
						{
						if(DFV->yaxis.axis_type==5)
						{
							memset(stemp,0,101);
							sprintf(stemp,"Y-AXIS %d TITLE 2",x+1);
							addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Ylen2,0,
								NULL,NULL,NULL);
						} else {
							memset(stemp,0,101);
							sprintf(stemp,"Y-AXIS %d",x+1);
							switch(DFV->yaxis.type)
							{
								case SCROLLEDTEXT:
									addwdgt(newrsrc->scn,8,stemp,stemp,NULL,NULL,
										5,80,0,NULL,NULL,NULL);
									break;
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case ZIPCODE:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,
										0,DFV->yaxis.len,0,NULL,NULL,NULL);
									break;
								case BOOLNS:
									addwdgt(newrsrc->scn,9,stemp,(am->title2!=NULL ?
										am->title2:stemp),NULL,NULL,0,DFV->yaxis.len,
										0,NULL,NULL,NULL);
									break;
								case CHARACTERS:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->yaxis.len,0,NULL,NULL,NULL);
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->yaxis.len,0,NULL,NULL,NULL);
									break;
								case SHORTV:
								case SSHORTV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->yaxis.len,0,NULL,NULL,NULL);
									break;
								case LONGV:
								case SLONGV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->yaxis.len,0,NULL,NULL,NULL);
									break;
								case SCROLLEDLIST:
								case BUTTONS:
									prterr("Error call RaddogTiddog cause this is weird....");
									break;
								default:
									prterr("Error field type [%d] invalid for Y-Axis# [%d] on Table [%s] [%s].",DFV->yaxis.type,x,DFV->module,DFV->name);
									break;
							}
						}
						}
						addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
					}
				}
			}
/* End Box */
			addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* New Box */
			addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			len=0;Xlen=0;
			for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
			{
				if(am->title1!=NULL) len=RDAstrlen(am->title1);
					else len=0;
				if(len>Xlen) Xlen=len;
				if(DFV->xaxis.axis_type!=5)
				{
					if(am->title2!=NULL) len=RDAstrlen(am->title2);
						else len=0;
					if(len>Xlen) Xlen=len;
				}
			}
			if(Xlen==0) Xlen=5;
/* New Line */
			if(xcount<=dcount)
			{
				addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
				{
					addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(DFV->xaxis.print_title1)
					{
						addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
						memset(stemp,0,101);
						sprintf(stemp,"X-AXIS %d TITLE 1",x+1);
						addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Xlen,0,
							NULL,NULL,NULL);
						addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
					}
					if(DFV->xaxis.print_title2)
					{
						addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
						if(DFV->xaxis.axis_type<5)
						{
							memset(stemp,0,101);
							sprintf(stemp,"X-AXIS %d",x+1);
							switch(DFV->xaxis.type)
							{
								case SCROLLEDTEXT:
									addwdgt(newrsrc->scn,8,stemp,stemp,NULL,NULL,
										5,80,0,NULL,NULL,NULL);
									break;
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case ZIPCODE:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,
										0,DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case BOOLNS:
									addwdgt(newrsrc->scn,9,stemp,(am->title2!=NULL ?
										am->title2:stemp),NULL,NULL,0,DFV->xaxis.len,
										0,NULL,NULL,NULL);
									break;
								case CHARACTERS:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case SHORTV:
								case SSHORTV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case LONGV:
								case SLONGV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case SCROLLEDLIST:
								case BUTTONS:
									prterr("Error call RaddogTiddog cause this is weird....");
									break;
								default:
									prterr("Error field type [%d] invalid for X-Axis# [%d] on Table [%s] [%s].",DFV->xaxis.type,x,DFV->module,DFV->name);
									break;
							}
						} else {
							memset(stemp,0,101);
							sprintf(stemp,"X-AXIS %d TITLE 2",x+1);
							addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Xlen,0,
								NULL,NULL,NULL);
						}
						addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
							NULL,NULL,NULL);
					}
					addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
				}
				addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			} else {
/* Column Titles */
				if(DFV->xaxis.print_title1)
				{
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
					{
						memset(stemp,0,101);
						sprintf(stemp,"X-AXIS %d TITLE 1",x+1);
						addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Xlen,0,
							NULL,NULL,NULL);
					}
/* End Line */
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				if(DFV->xaxis.print_title2)
				{
/* New Line */
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
					{
						if(DFV->xaxis.axis_type<5)
						{
							memset(stemp,0,101);
							sprintf(stemp,"X-AXIS %d",x+1);
							switch(DFV->xaxis.type)
							{
								case SCROLLEDTEXT:
									addwdgt(newrsrc->scn,8,stemp,stemp,NULL,NULL,
										5,80,0,NULL,NULL,NULL);
									break;
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case ZIPCODE:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,
										0,DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case BOOLNS:
									addwdgt(newrsrc->scn,9,stemp,(am->title2!=NULL ?
										am->title2:stemp),NULL,NULL,0,DFV->xaxis.len,
										0,NULL,NULL,NULL);
									break;
								case CHARACTERS:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case SHORTV:
								case SSHORTV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case LONGV:
								case SLONGV:
									addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,
										DFV->xaxis.len,0,NULL,NULL,NULL);
									break;
								case SCROLLEDLIST:
								case BUTTONS:
									prterr("Error call RaddogTiddog cause this is weird....");
									break;
								default:
									prterr("Error field type [%d] invalid for X-Axis# [%d] on Table [%s] [%s].",DFV->xaxis.type,x,DFV->module,DFV->name);
									break;
							}
						} else {
							memset(stemp,0,101);
							sprintf(stemp,"X-AXIS %d TITLE 2",x+1);
							addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,Xlen,0,
								NULL,NULL,NULL);
						}
					}
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
			}
/* End Line */
			if(DFV->recs!=NULL)
			{
				for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
				{
					addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
					if(rec->mems!=NULL)
					{
						for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
						{
							addwdgt(newrsrc->scn,10,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							if(mem->values!=NULL)
							{
								addwdgt(newrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								for(z=0,c=DFV->defs;z<mem->num;++z,++c)
								{
									if(c->displayed)
									{
										addwdgt(newrsrc->scn,1,NULL,NULL,NULL,NULL,
											0,0,0,NULL,NULL,NULL);
										if(!z && DFV->cell_config>0)
										{
											memset(stemp,0,101);
											if(!DFV->locate_method)
											{
												sprintf(stemp,"R%dC%d BUTTON",x+1,y+1);
											} else {
												sprintf(stemp,"CELL BUTTON %d",((x*rec->num)+y+1));
											}
											addwdgt(newrsrc->scn,6,stemp,stemp,NULL,NULL,0,0,1,NULL,NULL,NULL);
										}
										memset(stemp,0,101);
										if(!DFV->locate_method)
										{
											sprintf(stemp,"R%dC%dV%d",x+1,y+1,z+1);
										} else {
											sprintf(stemp,"CELL %d VALUE %d",
												((x*rec->num)+y+1),z+1);
										}
										switch(c->type)
										{
											case SCROLLEDTEXT:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,5,80,0,NULL,NULL,NULL);
												break;
											case VARIABLETEXT:
											case DATES:
											case TIMES:
											case SOCSECNUM:
											case PLAINTEXT:
											case PHONE:
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
											case ZIPCODE:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case BOOLNS:
												addwdgt(newrsrc->scn,9,stemp,c->name,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case CHARACTERS:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case SHORTV:
											case SSHORTV:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case LONGV:
											case SLONGV:
												addwdgt(newrsrc->scn,0,stemp,stemp,NULL,NULL,0,c->len,0,NULL,NULL,NULL);
												break;
											case SCROLLEDLIST:
											case BUTTONS:
												prterr("Error call RaddogTiddog cause this is weird....");
												break;
											default:
												prterr("Error field type [%d] invalid for X-Axis# [%d] on Table [%s] [%s].",c->type,z,DFV->module,DFV->name);
												break;
										}
										addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,
											0,0,0,NULL,NULL,NULL);
									}
								}
								addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							} else {
/* Avoiding an Empty Frame */
								addwdgt(newrsrc->scn,5,NULL,NULL,NULL,NULL,
									0,0,0,NULL,NULL,NULL);
							}
						}
					}
					addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,
						NULL,NULL,NULL);
				}
			}
/* End Box */
			addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
/* End Line */
		addwdgt(newrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* End Scrolled Window */
		addwdgt(newrsrc->scn,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* End Line */
		addwdgt(newrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
/* END SCROLLED WINDOW LINE */
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *libname=NULL;
	char *temp=NULL;

	if(InitializeSubsystems(argc,argv,module,"EDIT MATRIX"))
	{
		return;
	}
	if(argc>2)
	{
		DFV=RDAMATRIXNEW(argv[1],argv[2]);
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(argv[1])+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.MTX",CURRENTDIRECTORY,argv[1]);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,argv[1]);
#endif
		getRDAMatrixbin(libname,DFV);
		if(libname!=NULL) Rfree(libname);
	} else if(argc>1)
	{
		DFV=RDAMATRIXNEW(argv[1],NULL);
	} else {
		DFV=RDAMATRIXNEW(NULL,NULL);
	}
	temp=RDA_GetEnv("ROWMULTIPLIER");
	if(!isEMPTY(temp))
	{
/*lint -e524 */
		RowMultiplier=Ratoi(temp);
/*lint +e524 */
		if(RowMultiplier<1) RowMultiplier=7;
	} else {
		RowMultiplier=8;
	}
	MAINRSRC=GenerateMatrixRDArsrc();
	matrixdefaults=GetDefaults(MAINRSRC);
	APPmakescrn(MAINRSRC,TRUE,quiteditmatrix,NULL,TRUE);
}
