/* import.c - Import / Export Functions */
#include <mix.hpp>
#include <impexp.hpp>
#include<cctype>
#define MAXLINEBUFF	32164
#include <IBM390lib.hpp>

static void ArchiveExport(IEfile *);
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */
static char removewc=FALSE;


struct HoldIMPEXPs
{
	RDArsrc *rsrc;
	int record_counter;
}; 
typedef struct HoldIMPEXPs HoldIMPEXP;

HoldIMPEXP *xHoldIMPEXPNew(RDArsrc *rsrc,int record_counter)
{
	HoldIMPEXP *tmp=NULL;
	
	tmp=Rmalloc(sizeof(HoldIMPEXP));
	if(tmp!=NULL)
	{
		tmp->rsrc=rsrc;
		tmp->record_counter=record_counter;
	}
	return(tmp);
}
short xgetIMPORTbin(char *module,IEfile *i,int line,char *file)
{
	int x;
	short y,test=0,version=FALSE;
	BIN *bin;
	char *libname=NULL,*temp=NULL;
	IEfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG getIMPORTbin Getting Import/Export Binary for Module [%s] Definition [%s] at line [%d] program [%s].",module,i->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.IMP",CURRENTDIRECTORY,module);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.IMP",CURRENTDIRECTORY,module);	
#endif
	bin=getlibbin(libname,i->name,TRUE);
	if(bin==NULL) return(-1);
	y=BINgetshort(bin);
	if(y!=(-1695) && y!=(-1696) && y!=(-1697) && y!=(-1699) && y!=(-1701) && y!=(-1700) && y!=(-1698))
	{
		prterr("Error Invalid version or bad binary for Module [%s] Definition [%s] in Library [%s] at line [%d] program [%s].",module,i->name,libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(y==(-1695)) 
	{
		version=6;
	} else if(y==(-1696)) 
	{
		version=5;
	} else if(y==(-1697))
	{
		version=4;
	} else if(y==(-1698))
	{
		version=3;
	} else if(y==(-1699))
	{
		version=2;
	} else if(y==(-1700))
	{
		version=TRUE;
	} else version=FALSE;
	if(libname!=NULL) Rfree(libname);
	test=BINgetshort(bin);
	if(test) temp=BINgetstring(bin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(bin);
	if(test) i->module=BINgetstring(bin);
		else i->module=NULL;
	test=BINgetshort(bin);
	if(test) i->file=BINgetstring(bin);
		else i->file=NULL;
	test=BINgetshort(bin);
	if(test) i->ifile=BINgetstring(bin);
		else i->ifile=NULL;
	test=BINgetshort(bin);
	if(test) i->desc=BINgetstring(bin);
		else i->desc=NULL;
	test=BINgetshort(bin);
	if(test) i->fdel=BINgetstring(bin);
		else i->fdel=NULL;
	test=BINgetshort(bin);
	if(test) i->rdel=BINgetstring(bin);
		else i->rdel=NULL;
	i->which=BINgetint(bin);
	i->type=BINgetint(bin);
	i->range=BINgetshort(bin);
	i->ask_b4_execute=BINgetshort(bin);
	if(version>2) test=BINgetshort(bin);
		else test=FALSE;
	if(test) i->force_quotes=TRUE;
		else i->force_quotes=FALSE;
	if(version>3)
	{
		test=BINgetshort(bin);	
		if(test) i->carriage_return=TRUE;
			else i->carriage_return=FALSE;
		test=BINgetshort(bin);	
		if(test) i->line_feed=TRUE;
			else i->line_feed=FALSE;
	} else {
		i->carriage_return=TRUE;
		i->line_feed=TRUE;
	}
	if(version>4)
	{
		test=BINgetshort(bin);	
		if(test) i->skip_dups=TRUE;
			else i->skip_dups=FALSE;
	} else i->skip_dups=FALSE;
	i->num=BINgetint(bin);
	if(i->num>0)
	{
		i->fields=Rmalloc(i->num*sizeof(IEfield));
		for(x=0,f=i->fields;x<i->num;++x,++f)
		{
			test=BINgetshort(bin);
			if(test) f->name=BINgetstring(bin);
				else f->name=NULL;
			f->type=BINgetshort(bin);
			f->start=BINgetshort(bin);
			if(version) f->length=BINgetint(bin);
				else f->length=BINgetshort(bin);
			if(version>1)
			{
				test=BINgetshort(bin);
				if(test) f->expression=BINgetstring(bin);
					else f->expression=NULL;
			} else f->expression=NULL;
			if(version>2)
			{
				test=BINgetshort(bin);
				f->use_quotes=test;
			} else f->use_quotes=FALSE;
			if(version>5)
			{
				test=BINgetshort(bin);
				if(test) f->value_expression=BINgetstring(bin);
					else f->value_expression=NULL;
			} else f->value_expression=NULL;
		}
	}
	if(version>1)
	{
		test=BINgetshort(bin);
		if(test) i->expression=BINgetstring(bin);
			else i->expression=NULL;
	} else i->expression=NULL;
	i->fileno=(-1);
	i->rs=NULL;
	BINfree(bin);
	return(0);
}
short xwriteIMPORTbin(char *module,IEfile *i,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;
	IEfield *f;
	int x;
	short q=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG writeIMPORTbin Writing Import/Export Definition [%s] [%s] to Library [%s.IMP] at line [%d] program [%s].",module,i->name,module,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.IMP",CURRENTDIRECTORY,module);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.IMP",CURRENTDIRECTORY,module);	
#endif
	bin=BINnew();
	BINaddshort(bin,-1695);	/*type identifier*/
	if(!isEMPTY(i->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->name);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(i->module))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->module);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(i->file))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->file);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(i->ifile))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->ifile);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(i->desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(i->fdel))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->fdel);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(i->rdel))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->rdel);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,i->which);
	BINaddint(bin,i->type);
	BINaddshort(bin,i->range);
	BINaddshort(bin,i->ask_b4_execute);
	BINaddshort(bin,i->force_quotes);
	BINaddshort(bin,i->carriage_return);
	BINaddshort(bin,i->line_feed);
	BINaddshort(bin,i->skip_dups);
	BINaddint(bin,i->num);
	if(i->num>0)
	{
		for(x=0,f=i->fields;x<i->num;++x,++f)
		{
			if(!isEMPTY(f->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,f->name);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,f->type);
			BINaddshort(bin,f->start);
			BINaddint(bin,f->length);
			if(!isEMPTY(f->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,f->expression);
			} else { 
				BINaddshort(bin,FALSE);
			}
			q=f->use_quotes;
			BINaddshort(bin,q);
			if(!isEMPTY(f->value_expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,f->value_expression);
			} else { 
				BINaddshort(bin,FALSE);
			}
		}
	}
	if(!isEMPTY(i->expression))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,i->expression);
	} else { 
		BINaddshort(bin,FALSE);
	}
	if(writelibbin(libname,bin,i->name))
	{
		prterr("Error Can't write Import/Export Definition binary [%s] into library [%s] at line [%d] program [%s].",i->name,libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
IEfile *xIMPEXPdefNEW(char *name,char *module,char *datafile,char *ifile,char *desc,char *fdel,char *rdel,int which,int type,char force_quotes,char carriage_return,char line_feed,char skip_dups,short range,short ask_b4_execute,char *expression,int num,IEfield *ffields,int line,char *file)
{
	IEfile *IMPEXP=NULL;
	IEfield *ff=NULL,*tf=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		prterr("DIAG IMPEXPdefNEW() Creating Import/Export Definition for [%s] at line [%d] program [%s].",name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	IMPEXP=Rmalloc(sizeof(IEfile));
	if(!isEMPTY(name))
	{
		IMPEXP->name=stralloc(name);
	} else IMPEXP->name=NULL;
	if(!isEMPTY(module))
	{
		IMPEXP->module=stralloc(module);
	} else IMPEXP->module=NULL;
	if(!isEMPTY(datafile))
	{
		IMPEXP->file=stralloc(datafile);
	} else IMPEXP->file=NULL;
	if(!isEMPTY(ifile))
	{
		IMPEXP->ifile=stralloc(ifile);
	} else IMPEXP->ifile=NULL;
	if(!isEMPTY(desc))
	{
		IMPEXP->desc=stralloc(desc);
	} else IMPEXP->desc=NULL;
	if(!isEMPTY(fdel))
	{
		IMPEXP->fdel=stralloc(fdel);
	} else IMPEXP->fdel=NULL;
	if(!isEMPTY(rdel))
	{
		IMPEXP->rdel=stralloc(rdel);
	} else IMPEXP->rdel=NULL;
	IMPEXP->which=which;
	IMPEXP->type=type;
	IMPEXP->force_quotes=(force_quotes==FALSE?FALSE:TRUE);
	IMPEXP->carriage_return=(carriage_return==FALSE?FALSE:TRUE);
	IMPEXP->line_feed=(line_feed==FALSE?FALSE:TRUE);
	IMPEXP->skip_dups=(skip_dups==FALSE?FALSE:TRUE);
	IMPEXP->ask_b4_execute=(ask_b4_execute==FALSE?FALSE:TRUE);
	IMPEXP->range=(range==FALSE?FALSE:TRUE);
	if(!isEMPTY(expression))
	{
		IMPEXP->expression=stralloc(expression);
	} else IMPEXP->expression=NULL;
	IMPEXP->num=num;
	if(num>0 && ffields!=NULL)
	{
		IMPEXP->fields=Rmalloc(sizeof(IEfield)*num);
		for(x=0,ff=ffields,tf=IMPEXP->fields;x<num;++x,++ff,++tf)
		{
			if(!isEMPTY(ff->name))
			{
				tf->name=stralloc(ff->name);
			} else tf->name=NULL;
			tf->type=ff->type;
			tf->start=ff->start;
			tf->length=ff->length;
			if(!isEMPTY(ff->expression))
			{
				tf->expression=stralloc(ff->expression);
			} else tf->expression=NULL;
			tf->use_quotes=ff->use_quotes;
			if(!isEMPTY(ff->value_expression))
			{
				tf->value_expression=stralloc(ff->value_expression);
			} else tf->value_expression=NULL;
		}
	} else {
		IMPEXP->fields=0;
		IMPEXP->num=0;
	}
	IMPEXP->MainLoop=FALSE;
	IMPEXP->fileno=(-1);
	IMPEXP->rs=NULL;
	return(IMPEXP);
}
void xfree_import(IEfile *x,int line,char *file)
{
	int y;
	IEfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		prterr("DIAG free_import Freeing Import/Export Definition [%s] at line [%d] program [%s].",x->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(x!=NULL)
	{
		if(x->name!=NULL) Rfree(x->name);
		if(x->module!=NULL) Rfree(x->module);
		if(x->file!=NULL) Rfree(x->file);
		if(x->ifile!=NULL) Rfree(x->ifile);
		if(x->fdel!=NULL) Rfree(x->fdel);
		if(x->rdel!=NULL) Rfree(x->rdel);
		if(x->desc!=NULL) Rfree(x->desc);
		if(x->expression!=NULL) Rfree(x->expression);
		if(x->fields!=NULL)
		{
			for(y=0,f=x->fields;y<x->num;++y,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
				if(f->expression!=NULL) Rfree(f->expression);
				if(f->value_expression!=NULL) Rfree(f->value_expression);
			}
			Rfree(x->fields);
		}
		if(x->rs!=NULL) FreeRangeScreen(x->rs);
		Rfree(x);
		x=NULL;
	}
}
char *xgetIMPORTdesc(char *module,char *name,int line,char *file)
{
	short y,test=0;
	BIN *bin;
	char *libname=NULL,*temp=NULL,*hold=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG getIMPORTdesc Get Import Description for Module [%s] Import/Export Definition [%s] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
	temp=adddashes(module);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.IMP",CURRENTDIRECTORY,temp);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.IMP",CURRENTDIRECTORY,temp);	
#endif
	if(temp!=NULL) Rfree(temp);
	temp=NULL;
	bin=getlibbin(libname,name,TRUE);
	if(libname!=NULL) Rfree(libname);
	if(bin==NULL) return(NULL);
	y=BINgetshort(bin);
	if(y!=(-1695) && y!=(-1696) && y!=(-1697) && y!=(-1698) && y!=(-1699) && y!=(-1701) && y!=(-1700))
	{
		prterr("Error Invalid version or bad binary for definition [%s] in Library [%s.IMP] at line [%d] program [%s].",name,module,line,file);
		BINfree(bin);
		return(NULL);
	}
	test=BINgetshort(bin);
	if(test) temp=BINgetstring(bin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(bin);
	if(test) temp=BINgetstring(bin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(bin);
	if(test) temp=BINgetstring(bin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(bin);
	if(test) temp=BINgetstring(bin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(bin);
	if(test) hold=BINgetstring(bin);
		else hold=NULL;
	BINfree(bin);
	return(hold);
}
void IMPEXPGenSubData(char **tmp,HoldIMPEXP *h)
{
	RDArsrc *rsrc=NULL;
	char *modulename=NULL;
	char *filename=NULL;
	char *fieldname=NULL;
	char temp_string[101];
	int length_string=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diageval)
	{
		prterr("DIAG IMPEXPGenSubData Substituting for [%s].",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	rsrc=h->rsrc;

	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
		filename=stripfilename(*tmp);
		fieldname=stripfieldname(*tmp);
		if(isEMPTY(fieldname))
		{
			if(!RDAstrcmp(modulename,"RECORD COUNTER"))
			{
				memset(temp_string,0,101);
				sprintf(temp_string,"%d",h->record_counter);
				length_string=RDAstrlen(temp_string);
				if(RDAstrlen(*tmp)<length_string)
				{
					*tmp=Rrealloc(*tmp,length_string+1);
				}
				memset(*tmp,0,RDAstrlen(*tmp)+1);
				memcpy(*tmp,temp_string,length_string+1);
			} else if(GLOBALSubData(tmp,modulename)==0) 
			{
			} else {
				if(RDAstrcmp(*tmp,"[NEXT TRANSACTION NO]"))
				{
					prterr("Error: IMPEXPSubData() [%s] not found.",*tmp);
				}
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
		} else {
			SCRNvirtualSubData(tmp,rsrc);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diageval)
	{
		prterr("DIAG IMPEXPGenSubData Returning [%s].",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(filename!=NULL) Rfree(filename);
	if(fieldname!=NULL) Rfree(fieldname);
	if(modulename!=NULL) Rfree(modulename);
}
static short DeCryptSignSht(char *string)
{
	short r=0;
	char temp=0;
	int len=0;

	if(!isEMPTY(string))
	{
		len=RDAstrlen(string);
		temp=string[len-1];
		r=Ratoi(string);
		switch(temp)
		{
			default:
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case '{':
				r=r*10;
				break;
			case 'A':
				r=r*10;
				r+=1;
				break;
			case 'B':
				r=r*10;
				r+=2;
				break;
			case 'C':
				r=r*10;
				r+=3;
				break;
			case 'D':
				r=r*10;
				r+=4;
				break;
			case 'E':
				r=r*10;
				r+=5;
				break;
			case 'F':
				r=r*10;
				r+=6;
				break;
			case 'G':
				r=r*10;
				r+=7;
				break;
			case 'H':
				r=r*10;
				r+=8;
				break;
			case 'I':
				r=r*10;
				r+=9;
				break;
			case '}':
				r=r*10;
				r*=(-1);
				break;
			case 'J':
				r=r*10;
				r+=1;
				r*=(-1);
				break;
			case 'K':
				r=r*10;
				r+=2;
				r*=(-1);
				break;
			case 'L':
				r=r*10;
				r+=3;
				r*=(-1);
				break;
			case 'M':
				r=r*10;
				r+=4;
				r*=(-1);
				break;
			case 'N':
				r=r*10;
				r+=5;
				r*=(-1);
				break;
			case 'O':
				r=r*10;
				r+=6;
				r*=(-1);
				break;
			case 'P':
				r=r*10;
				r+=7;
				r*=(-1);
				break;
			case 'Q':
				r=r*10;
				r+=8;
				r*=(-1);
				break;
			case 'R':
				r=r*10;
				r+=9;
				r*=(-1);
				break;
		}
	}
	return(r);
}
static int DeCryptSignInt(char *string)
{
	int r=0;
	char temp=0;
	int len=0;

	if(!isEMPTY(string))
	{
		len=RDAstrlen(string);
		temp=string[len-1];
		r=Ratoi(string);
		switch(temp)
		{
			default:
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case '{':
				r=r*10;
				break;
			case 'A':
				r=r*10;
				r+=1;
				break;
			case 'B':
				r=r*10;
				r+=2;
				break;
			case 'C':
				r=r*10;
				r+=3;
				break;
			case 'D':
				r=r*10;
				r+=4;
				break;
			case 'E':
				r=r*10;
				r+=5;
				break;
			case 'F':
				r=r*10;
				r+=6;
				break;
			case 'G':
				r=r*10;
				r+=7;
				break;
			case 'H':
				r=r*10;
				r+=8;
				break;
			case 'I':
				r=r*10;
				r+=9;
				break;
			case '}':
				r=r*10;
				r*=(-1);
				break;
			case 'J':
				r=r*10;
				r+=1;
				r*=(-1);
				break;
			case 'K':
				r=r*10;
				r+=2;
				r*=(-1);
				break;
			case 'L':
				r=r*10;
				r+=3;
				r*=(-1);
				break;
			case 'M':
				r=r*10;
				r+=4;
				r*=(-1);
				break;
			case 'N':
				r=r*10;
				r+=5;
				r*=(-1);
				break;
			case 'O':
				r=r*10;
				r+=6;
				r*=(-1);
				break;
			case 'P':
				r=r*10;
				r+=7;
				r*=(-1);
				break;
			case 'Q':
				r=r*10;
				r+=8;
				r*=(-1);
				break;
			case 'R':
				r=r*10;
				r+=9;
				r*=(-1);
				break;
		}
	}
	return(r);
}
static double DeCryptSignDbl(char *string)
{
	double r=0;
	char temp=0;
	int len=0;

	if(!isEMPTY(string))
	{
		len=RDAstrlen(string);
		temp=string[len-1];
		r=Ratoi(string);
		switch(temp)
		{
			default:
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case '{':
				r=r*10;
				break;
			case 'A':
				r=r*10;
				r+=1;
				break;
			case 'B':
				r=r*10;
				r+=2;
				break;
			case 'C':
				r=r*10;
				r+=3;
				break;
			case 'D':
				r=r*10;
				r+=4;
				break;
			case 'E':
				r=r*10;
				r+=5;
				break;
			case 'F':
				r=r*10;
				r+=6;
				break;
			case 'G':
				r=r*10;
				r+=7;
				break;
			case 'H':
				r=r*10;
				r+=8;
				break;
			case 'I':
				r=r*10;
				r+=9;
				break;
			case '}':
				r=r*10;
				r*=(-1);
				break;
			case 'J':
				r=r*10;
				r+=1;
				r*=(-1);
				break;
			case 'K':
				r=r*10;
				r+=2;
				r*=(-1);
				break;
			case 'L':
				r=r*10;
				r+=3;
				r*=(-1);
				break;
			case 'M':
				r=r*10;
				r+=4;
				r*=(-1);
				break;
			case 'N':
				r=r*10;
				r+=5;
				r*=(-1);
				break;
			case 'O':
				r=r*10;
				r+=6;
				r*=(-1);
				break;
			case 'P':
				r=r*10;
				r+=7;
				r*=(-1);
				break;
			case 'Q':
				r=r*10;
				r+=8;
				r*=(-1);
				break;
			case 'R':
				r=r*10;
				r+=9;
				r*=(-1);
				break;
		}
	}
	return(r);
}
static short importflatfile(RDArsrc *mainrsrc,IEfile *importx)
{
	FILE *fp;
	char temp[32164],*temp2=NULL;
	char *e,*temp1=NULL;
	short x,skip_field=FALSE,testval=FALSE;
	IEfield *f;
	NRDfield *field;
	RDArsrc *tmprsrc=NULL;
	int counters=0,len_temp=0,use_temp=0;
	RDAvirtual *v;
	RDATData *previous=NULL;
	HoldIMPEXP *h=NULL;
	char *temp_string=NULL,temp_boolns=0;
	char *trnfield=NULL;
	int len=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG importflatfile Import Flat File Using Definition [%s].",importx->name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	memset(stemp,0,101);
	if(importx->carriage_return || importx->line_feed) 
	{
		use_temp=32163;
	} else {
		use_temp=0;
		for(x=0,f=importx->fields;x<importx->num;++x,++f)
		{
			if(use_temp<(f->start+f->length-1))
				use_temp=f->start+f->length-1;
				
		}
	} 
	counters=CountLines(importx->ifile);
	temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(importx->ifile)+2);
	sprintf(temp2,"%s/%s",CURRENTDIRECTORY,importx->ifile);
#ifndef WIN32
	fp=fopen(temp2,"rb");
#else
	fp=fopen(temp2,"rbc");
#endif
	if(temp2!=NULL) Rfree(temp2);
	if(fp==NULL) 
	{
		prterr("Error Attempting to open and read file [%s] to import from.",
			importx->ifile);
		return(-1);
	}
	e=Rmalloc(RDAstrlen(importx->file)+RDAstrlen(importx->module)+25);
	sprintf(e,"Importing Module %s File %s",importx->module,importx->file);
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","DATABASE",e,stop_diagnostic,counters);
	addDFincvir(tmprsrc,importx->module,importx->file,NULL,importx->fileno);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	previous=RDATDataNEW(importx->fileno);
	if(e!=NULL) Rfree(e);
#ifdef __USE_DIAGNOSTIC_SCREENS__
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(importx->module)+RDAstrlen(importx->name)+15);
			sprintf(e,"%s %s IMPORTING",importx->module,importx->name);
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
	for(x=0,f=importx->fields;x<importx->num;++x,++f)
	{
		if(!isEMPTY(f->value_expression))
		{
			if(!RDAstrcmp(f->value_expression,"[NEXT TRANSACTION NO]"))
			{
				trnfield=stralloc(f->name);
				break;
			}
		}
	}
	h=xHoldIMPEXPNew(tmprsrc,0);
	h->record_counter=0;
	if(importx->carriage_return || importx->line_feed)
	{
	while((len=readline(fp,temp,use_temp))!=(-1))
	{
		++h->record_counter;
		if(abort_diagnostic) break;
#ifdef USE_RDA_DIAGNOSTICS
		if(diagimport || diagimport_field)
		{
			prterr("DIAG Importing Line [%s] from file [%s].",temp,importx->ifile);
			/*
			prterr("The hexadecimal input is [");
			for(x=0;x<use_temp;x++)
			{
				prterr("%x",temp[x]);
			}
			prterr("].\n");
			*/
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(temp!=NULL && (importx->carriage_return || importx->line_feed))
		{
			for(temp1=temp;*temp1;++temp1)
			{
				if((*temp1=='\r') || (*temp1=='\n')) *temp1=0; 
			}
		}
		temp[use_temp]=0;
		len_temp=RDAstrlen(temp);
		ZERNRD(importx->fileno);
		CLEAR_SCREEN_VIRTUAL(tmprsrc);
		memset(stemp,0,101);
		for(x=0,f=importx->fields;x<importx->num;++x,++f)
		{
			if(RDAstrcmp(trnfield,f->name))
			{
			if((f->start-1)>=0 && (f->start-1)<len_temp)
			{
				if(!isEMPTY(f->expression))
				{
					skip_field=!PP_EVALUATEbol(f->expression,SCRNvirtualSubData,tmprsrc);
				} else {
					skip_field=FALSE;
				}
				if(!skip_field)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diagimport || diagimport_field)
					{
						prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					field=FLDNRD(importx->fileno,f->name);
					if(field!=NULL)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagimport || diagimport_field)
						{
							prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
								} else {
									strncpy(field->data.string_value,&temp[f->start-1],f->length);
								}
								field->data.string_value[f->length]=0;
								if(field->dlen>field->len && field->len>0)
								{
									field->data.string_value[field->len]=0;
								}
								unpad(field->data.string_value);
								if(!isEMPTY(f->value_expression))
								{
									temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
									if(field->data.string_value!=NULL) Rfree(field->data.string_value);
									field->data.string_value=stralloc(temp_string);
									field->dlen=RDAstrlen(temp_string)+1;
									if(temp_string!=NULL) Rfree(temp_string);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case BOOLNS:
								memset(stemp,0,101);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
								} else {
									stemp[0]=temp[f->start-1];
								}
								if(stemp[0]=='N' ||
									stemp[0]=='n' ||
									stemp[0]=='F' ||
									stemp[0]=='f' ||
									stemp[0]==' ' ||
									stemp[0]=='0')
								{
									*field->data.string_value=0;
								} else { 
									*field->data.string_value=1;
								}
								if(!isEMPTY(f->value_expression))
								{
									temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
									if(temp_boolns) *field->data.string_value=TRUE;
										else *field->data.string_value=FALSE;
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case CHARACTERS:
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
								} else {
									*field->data.string_value=temp[f->start-1];
								}
								if(!isEMPTY(f->value_expression))
								{
									*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.float_value=DeCryptSignDbl(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.float_value=atof(stemp);
									}
								} else { 
									*field->data.float_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS_NOCENTS:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.float_value=DeCryptSignDbl(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.float_value=atof(stemp);
									}
								} else { 
									*field->data.float_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2)
									{
										*field->data.float_value=DeCryptSignDbl(stemp);
									} else if(f->use_quotes==14)
									{
										*field->data.float_value=(round(DeCryptSignDbl(stemp)*100));
									} else if(f->use_quotes==4)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.float_value=atof(stemp);
									}
								} else { 
									*field->data.float_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SHORTV:
							case SSHORTV:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.short_value=DeCryptSignSht(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.short_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.short_value=Ratoi(stemp);
									}
								} else { 
									*field->data.short_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.integer_value=DeCryptSignInt(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
									} else  if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else  if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.integer_value=Ratoi(stemp);
									}
								} else { 
									*field->data.integer_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							default:
								prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
								break;
						}
					} else {
						prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
					}
				}
			} else if(((f->start-1)>=0) && (f->use_quotes>2 && f->use_quotes<14)) { /* For conversions from IBM390 */
				if(!isEMPTY(f->expression))
				{
					skip_field=!PP_EVALUATEbol(f->expression,SCRNvirtualSubData,tmprsrc);
				} else {
					skip_field=FALSE;
				}
				if(!skip_field)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diagimport || diagimport_field)
					{
						prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					field=FLDNRD(importx->fileno,f->name);
					if(field!=NULL)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagimport || diagimport_field)
						{
							prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
								} else {
									strncpy(field->data.string_value,&temp[f->start-1],f->length);
								}
								field->data.string_value[f->length]=0;
								if(field->dlen>field->len && field->len>0)
								{
									field->data.string_value[field->len]=0;
								}
								unpad(field->data.string_value);
								if(!isEMPTY(f->value_expression))
								{
									temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
									if(field->data.string_value!=NULL) Rfree(field->data.string_value);
									field->data.string_value=stralloc(temp_string);
									field->dlen=RDAstrlen(temp_string)+1;
									if(temp_string!=NULL) Rfree(temp_string);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes==14)
								{
									*field->data.float_value=(round(DeCryptSignDbl(stemp)));
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								} else {
									*field->data.float_value=atof(stemp);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS_NOCENTS:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes==14)
								{
									*field->data.float_value=(round(DeCryptSignDbl(stemp)));
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								} else {
									*field->data.float_value=atof(stemp);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes==14)
								{
									*field->data.float_value=(round(DeCryptSignDbl(stemp)*100));
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								} else {
									*field->data.float_value=atof(stemp)*100;
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SHORTV:
							case SSHORTV:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.short_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								} else if(f->use_quotes==14)
								{
									*field->data.short_value=(round(DeCryptSignSht(stemp)));
								} else {
									*field->data.short_value=atoi(stemp);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
								} else  if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else  if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								} else if(f->use_quotes==14)
								{
									*field->data.integer_value=(round(DeCryptSignInt(stemp)));
								} else {
									*field->data.integer_value=atoi(stemp);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case BOOLNS:
								memset(stemp,0,101);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
								} else {
									stemp[0]=temp[f->start-1];
								}
								if(stemp[0]=='N' ||
									stemp[0]=='n' ||
									stemp[0]=='F' ||
									stemp[0]=='f' ||
									stemp[0]==' ' ||
									stemp[0]=='0')
								{
									*field->data.string_value=0;
								} else { 
									*field->data.string_value=1;
								}
								if(!isEMPTY(f->value_expression))
								{
									temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
									if(temp_boolns) *field->data.string_value=TRUE;
										else *field->data.string_value=FALSE;
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case CHARACTERS:
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
								} else {
									*field->data.string_value=temp[f->start-1];
								}
								if(!isEMPTY(f->value_expression))
								{
									*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							default:
								break;
						}
					} else {
						prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
					}
				}
			}
			}
		}
		skip_field=TRUE;
		if(mainrsrc!=NULL && importx->range)
		{
			testval=!testfilerangersrc(importx->fileno,importx->rs);
			if(testval)
			{
				for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
				{
					if(ADVtestvirtualrangersrc(v,importx->rs,SCRNvirtualSubData,mainrsrc))
					{
						testval=FALSE;
						skip_field=FALSE;
						break;
					}
				}
			}
		} else { 
			testval=TRUE;
		}
		if(testval)
		{
			if(!isEMPTY(importx->expression))
			{
				if(PP_EVALUATEbol(importx->expression,SCRNvirtualSubData,tmprsrc))
				{
					if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield))
					{
						if(!importx->skip_dups)
						{
							GetRDATData(importx->fileno,previous);
							CLEAR_SCREEN_VIRTUAL(tmprsrc);
							memset(stemp,0,101);
							for(x=0,f=importx->fields;x<importx->num;++x,++f)
							{
								if((f->start-1)>=0 && (f->start-1)<len_temp)
								{
									if(!isEMPTY(f->expression))
									{
										skip_field=!PP_EVALUATEbol(f->expression,
											SCRNvirtualSubData,tmprsrc);
									} else {
										skip_field=FALSE;
									}
									if(!skip_field)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										field=FLDNRD(importx->fileno,f->name);
										if(field!=NULL)
										{
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
												case CUSTOMTYPE:
												case EXPENDITURE:
												case REVENUE:
												case BALANCESHEET:
												case BEGINNINGBALANCE:
												case OPTIONALFIELDS:
													QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
													if(f->use_quotes==3)
													{
														CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
													} else {
														strncpy(field->data.string_value,&temp[f->start-1],f->length);
													}
													field->data.string_value[f->length]=0;
													if(field->dlen>field->len && field->len>0)
													{
														field->data.string_value[field->len]=0;
													}
													unpad(field->data.string_value);
													if(!isEMPTY(f->value_expression))
													{
														temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
														if(field->data.string_value!=NULL) Rfree(field->data.string_value);
														field->data.string_value=stralloc(temp_string);
														field->dlen=RDAstrlen(temp_string)+1;
														if(temp_string!=NULL) Rfree(temp_string);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case BOOLNS:
													memset(stemp,0,101);
													if(f->use_quotes==3)
													{
														CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
													} else {
														stemp[0]=temp[f->start-1];
													}
													if(stemp[0]=='N' ||
														stemp[0]=='n' ||
														stemp[0]=='F' ||
														stemp[0]=='f' ||
														stemp[0]==' ' ||
														stemp[0]=='0')
													{
														*field->data.string_value=0;
													} else { 
														*field->data.string_value=1;
													}
													if(!isEMPTY(f->value_expression))
													{
														temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
														if(temp_boolns) *field->data.string_value=TRUE;
															else *field->data.string_value=FALSE;
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case CHARACTERS:
													if(f->use_quotes==3)
													{
														CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
													} else {
														*field->data.string_value=temp[f->start-1];
													}
													if(!isEMPTY(f->value_expression))
													{
														*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DECIMALV:
												case DOUBLEV:
												case SDOUBLEV:
												case SDECIMALV:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.float_value=DeCryptSignDbl(stemp);
														} else if(f->use_quotes==4)
				{
															*field->data.float_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.float_value=atof(stemp);
														}

													} else { 
														*field->data.float_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS_NOCENTS:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.float_value=DeCryptSignDbl(stemp);
														} else if(f->use_quotes==4)
				{
															*field->data.float_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.float_value=atof(stemp);
														}

													} else { 
														*field->data.float_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2)
														{
															*field->data.float_value=DeCryptSignDbl(stemp);
														} else if(f->use_quotes==14)
														{
															*field->data.float_value=DeCryptSignDbl(stemp)*100;
														} else if(f->use_quotes==4)
				{
															*field->data.float_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.float_value=atof(stemp);
														}

													} else { 
														*field->data.float_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SHORTV:
												case SSHORTV:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.short_value=DeCryptSignSht(stemp);
														} else if(f->use_quotes==4)
				{
															*field->data.short_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.short_value=Ratoi(stemp);
														}
													} else { 
														*field->data.short_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SCROLLEDLIST:
												case LONGV:
												case SLONGV:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.integer_value=DeCryptSignInt(stemp);
														} else if(f->use_quotes==4)
				{
															*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.integer_value=Ratoi(stemp);
														}
													} else { 
														*field->data.integer_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												default:
													prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
													break;
											}
										} else {
											prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
										}
									}
								}
							}
							WRTTRANSsec(importx->fileno,0,NULL,previous);
						}
					} else {  /* if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield)) */
						ClearRDATData(previous);
						CLEAR_SCREEN_VIRTUAL(tmprsrc);
						memset(stemp,0,101);
						for(x=0,f=importx->fields;x<importx->num;++x,++f)
						{
							if(!isEMPTY(f->value_expression))
							{
								if((f->start-1)>=0 && (f->start-1)<len_temp)
								{
									if(!isEMPTY(f->expression))
									{
										skip_field=!PP_EVALUATEbol(f->expression,SCRNvirtualSubData,tmprsrc);
									} else {
										skip_field=FALSE;
									}
									if(!skip_field)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										field=FLDNRD(importx->fileno,f->name);
										if(field!=NULL)
										{
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
												case CUSTOMTYPE:
												case EXPENDITURE:
												case REVENUE:
												case BALANCESHEET:
												case BEGINNINGBALANCE:
												case OPTIONALFIELDS:
													temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
													if(!isEMPTY(temp_string))
													{
														QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
														strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
														field->data.string_value[RDAstrlen(temp_string)]=0;
														if(field->dlen>field->len && field->len>0)
														{
															field->data.string_value[field->len]=0;
														}
													} else {
														field->data.string_value[0]=0;
													}
													if(temp_string!=NULL) Rfree(temp_string);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case BOOLNS:
													temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													if(temp_boolns) *field->data.string_value=TRUE;
														else *field->data.string_value=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case CHARACTERS:
													*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DECIMALV:
												case DOUBLEV:
												case SDOUBLEV:
												case SDECIMALV:
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS_NOCENTS:
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS:
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SHORTV:
												case SSHORTV:
													*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SCROLLEDLIST:
												case LONGV:
												case SLONGV:
													*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												default:
													prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
													break;
											}
										} else {
											prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
										}
									}
								}
							}
						}
						if(!isEMPTY(trnfield))
						{
							WRTTRANSsec(importx->fileno,1,trnfield,NULL);
						} else {
							WRTTRANSsec(importx->fileno,0,NULL,previous);
						}
					}
				} else { 
					skip_field=FALSE;
				}
			} else { /* NO IMPORT EXPRESSION per LINE */
				if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield))
				{
					if(!importx->skip_dups)
					{
						GetRDATData(importx->fileno,previous);
						CLEAR_SCREEN_VIRTUAL(tmprsrc);
						memset(stemp,0,101);
						for(x=0,f=importx->fields;x<importx->num;++x,++f)
						{
							if((f->start-1)>=0 && (f->start-1)<len_temp)
							{
								if(!isEMPTY(f->expression))
								{
									skip_field=!PP_EVALUATEbol(f->expression,
										SCRNvirtualSubData,tmprsrc);
								} else {
									skip_field=FALSE;
								}
								if(!skip_field)
								{
#ifdef USE_RDA_DIAGNOSTICS
									if(diagimport || diagimport_field)
									{
										prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
									field=FLDNRD(importx->fileno,f->name);
									if(field!=NULL)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
												QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
												} else {
													strncpy(field->data.string_value,&temp[f->start-1],f->length);
												}
												field->data.string_value[f->length]=0;
												if(field->dlen>field->len && field->len>0)
												{
													field->data.string_value[field->len]=0;
												}
												unpad(field->data.string_value);
												if(!isEMPTY(f->value_expression))
												{
													temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
													if(!isEMPTY(temp_string))
													{
														QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
														strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
														field->data.string_value[RDAstrlen(temp_string)]=0;
														if(field->dlen>field->len && field->len>0)
														{
															field->data.string_value[field->len]=0;
														}
													} else if(field->data.string_value!=NULL)
													{
														field->data.string_value[0]=0;
													}
													if(temp_string!=NULL) Rfree(temp_string);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case BOOLNS:
												memset(stemp,0,101);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
												} else {
													stemp[0]=temp[f->start-1];
												}
												if(stemp[0]=='N' ||
													stemp[0]=='n' ||
													stemp[0]=='F' ||
													stemp[0]=='f' ||
													stemp[0]==' ' ||
													stemp[0]=='0')
												{
													*field->data.string_value=0;
												} else { 
													*field->data.string_value=1;
												}
												if(!isEMPTY(f->value_expression))
												{
													temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													if(temp_boolns) *field->data.string_value=TRUE;
														else *field->data.string_value=FALSE;
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case CHARACTERS:
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
												} else {
													*field->data.string_value=temp[f->start-1];
												}
												if(!isEMPTY(f->value_expression))
												{
													*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.float_value=DeCryptSignDbl(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.float_value=atof(stemp);
													}
												} else { 
													*field->data.float_value=0;
												}	
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS_NOCENTS:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.float_value=DeCryptSignDbl(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.float_value=atof(stemp);
													}
												} else { 
													*field->data.float_value=0;
												}	
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2)
													{
														*field->data.float_value=DeCryptSignDbl(stemp);
													} else if(f->use_quotes==14)
													{
														*field->data.float_value=DeCryptSignDbl(stemp)*100;
													} else if(f->use_quotes==4)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.float_value=atof(stemp);
													}
												} else { 
													*field->data.float_value=0;
												}	
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SHORTV:
											case SSHORTV:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.short_value=DeCryptSignSht(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.short_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else  if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.short_value=Ratoi(stemp);
													}
												} else { 
													*field->data.short_value=0;
												}
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SCROLLEDLIST:
											case LONGV:
											case SLONGV:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.integer_value=DeCryptSignInt(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else  if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.integer_value=Ratoi(stemp);
													}
												} else { 
													*field->data.integer_value=0;
												}
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											default:
												prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
												break;
										}
									} else {
										prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
									}
								}
							} else if(((f->start-1)>=0) && (f->use_quotes>3 && f->use_quotes<14)) /* For conversions from IBM390 */
							{
								if(!isEMPTY(f->expression))
								{
									skip_field=!PP_EVALUATEbol(f->expression,
										SCRNvirtualSubData,tmprsrc);
								} else {
									skip_field=FALSE;
								}
								if(!skip_field)
								{
#ifdef USE_RDA_DIAGNOSTICS
									if(diagimport || diagimport_field)
									{
										prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
									field=FLDNRD(importx->fileno,f->name);
									if(field!=NULL)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
												QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
												} else {
													strncpy(field->data.string_value,&temp[f->start-1],f->length);
												}
												field->data.string_value[f->length]=0;
												if(field->dlen>field->len && field->len>0)
												{
													field->data.string_value[field->len]=0;
												}
												unpad(field->data.string_value);
												if(!isEMPTY(f->value_expression))
												{
													temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
													if(!isEMPTY(temp_string))
													{
														QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
														strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
														field->data.string_value[RDAstrlen(temp_string)]=0;
														if(field->dlen>field->len && field->len>0)
														{
															field->data.string_value[field->len]=0;
														}
													} else if(field->data.string_value!=NULL)
													{
														field->data.string_value[0]=0;
													}
													if(temp_string!=NULL) Rfree(temp_string);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												} else if(f->use_quotes==14)
												{
													*field->data.float_value=DeCryptSignDbl(stemp);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS_NOCENTS:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												} else if(f->use_quotes==14)
												{
													*field->data.float_value=DeCryptSignDbl(stemp);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												} else if(f->use_quotes==14)
												{
													*field->data.float_value=DeCryptSignDbl(stemp)*100;
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SHORTV:
											case SSHORTV:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==2)
												{
													*field->data.short_value=DeCryptSignSht(stemp);
												} else if(f->use_quotes==4)
												{
													*field->data.short_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else  if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												} else if(f->use_quotes==14)
												{
													*field->data.short_value=DeCryptSignSht(stemp);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SCROLLEDLIST:
											case LONGV:
											case SLONGV:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else  if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												} else if(f->use_quotes==14)
												{
													*field->data.integer_value=DeCryptSignInt(stemp);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case BOOLNS:
												memset(stemp,0,101);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
												} else {
													stemp[0]=temp[f->start-1];
												}
												if(stemp[0]=='N' ||
													stemp[0]=='n' ||
													stemp[0]=='F' ||
													stemp[0]=='f' ||
													stemp[0]==' ' ||
													stemp[0]=='0')
												{
													*field->data.string_value=0;
												} else { 
													*field->data.string_value=1;
												}
												if(!isEMPTY(f->value_expression))
												{
													temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													if(temp_boolns) *field->data.string_value=TRUE;
														else *field->data.string_value=FALSE;
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case CHARACTERS:
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
												} else {
													*field->data.string_value=temp[f->start-1];
												}
												if(!isEMPTY(f->value_expression))
												{
													*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
										}
									} else {
										prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
									}
								}
							}
						}
						WRTTRANSsec(importx->fileno,0,NULL,previous);
					}
				} else {  /* if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield)) */
					ClearRDATData(previous);
					CLEAR_SCREEN_VIRTUAL(tmprsrc);
					memset(stemp,0,101);
					for(x=0,f=importx->fields;x<importx->num;++x,++f)
					{
						if((f->start-1)>=0 && (f->start-1)<len_temp)
						{
							if(!isEMPTY(f->expression))
							{
								skip_field=!PP_EVALUATEbol(f->expression,
									SCRNvirtualSubData,tmprsrc);
							} else {
								skip_field=FALSE;
							}
							if(!skip_field)
							{
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								field=FLDNRD(importx->fileno,f->name);
								if(field!=NULL)
								{
#ifdef USE_RDA_DIAGNOSTICS
									if(diagimport || diagimport_field)
									{
										prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
										case CUSTOMTYPE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:
											if(!isEMPTY(f->value_expression))
											{
												temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
												if(!isEMPTY(temp_string))
												{
													QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
													strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
													field->data.string_value[RDAstrlen(temp_string)]=0;
													if(field->dlen>field->len && field->len>0)
													{
														field->data.string_value[field->len]=0;
													}
												} else {
													field->data.string_value[0]=0;
												}
												if(temp_string!=NULL) Rfree(temp_string);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case BOOLNS:
											if(!isEMPTY(f->value_expression))
											{
												temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
												if(temp_boolns) *field->data.string_value=TRUE;
													else *field->data.string_value=FALSE;
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case CHARACTERS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
										case SDECIMALV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DOLLARS_NOCENTS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DOLLARS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case SHORTV:
										case SSHORTV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										default:
											prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
											break;
									}
								} else {
									prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
								}
							}
						}
					}
					if(!isEMPTY(trnfield))
					{
						WRTTRANSsec(importx->fileno,1,trnfield,NULL);
					} else {
						WRTTRANSsec(importx->fileno,0,NULL,previous);
					}
				}
			}
		}
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(skip_field ? TRUE:FALSE));
		}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
	}
	} else {
	while(fread(temp,1,use_temp,fp))
	{
		++h->record_counter;
		if(abort_diagnostic) break;
#ifdef USE_RDA_DIAGNOSTICS
		if(diagimport || diagimport_field)
		{
			prterr("DIAG Importing Line [%s] from file [%s].",temp,importx->ifile);
			/*
			prterr("The hexadecimal input is [");
			for(x=0;x<use_temp;x++)
			{
				prterr("%x",temp[x]);
			}
			prterr("].\n");
			*/
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(temp!=NULL && (importx->carriage_return || importx->line_feed))
		{
			for(temp1=temp;*temp1;++temp1)
			{
				if((*temp1=='\r') || (*temp1=='\n')) *temp1=0; 
			}
		}
		temp[use_temp]=0;
		len_temp=RDAstrlen(temp);
		ZERNRD(importx->fileno);
		CLEAR_SCREEN_VIRTUAL(tmprsrc);
		memset(stemp,0,101);
		for(x=0,f=importx->fields;x<importx->num;++x,++f)
		{
			if(RDAstrcmp(trnfield,f->name))
			{
			if((f->start-1)>=0 && (f->start-1)<len_temp)
			{
				if(!isEMPTY(f->expression))
				{
					skip_field=!PP_EVALUATEbol(f->expression,SCRNvirtualSubData,tmprsrc);
				} else {
					skip_field=FALSE;
				}
				if(!skip_field)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diagimport || diagimport_field)
					{
						prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					field=FLDNRD(importx->fileno,f->name);
					if(field!=NULL)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagimport || diagimport_field)
						{
							prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
								} else {
									strncpy(field->data.string_value,&temp[f->start-1],f->length);
								}
								field->data.string_value[f->length]=0;
								if(field->dlen>field->len && field->len>0)
								{
									field->data.string_value[field->len]=0;
								}
								unpad(field->data.string_value);
								if(!isEMPTY(f->value_expression))
								{
									temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
									if(field->data.string_value!=NULL) Rfree(field->data.string_value);
									field->data.string_value=stralloc(temp_string);
									field->dlen=RDAstrlen(temp_string)+1;
									if(temp_string!=NULL) Rfree(temp_string);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case BOOLNS:
								memset(stemp,0,101);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
								} else {
									stemp[0]=temp[f->start-1];
								}
								if(stemp[0]=='N' ||
									stemp[0]=='n' ||
									stemp[0]=='F' ||
									stemp[0]=='f' ||
									stemp[0]==' ' ||
									stemp[0]=='0')
								{
									*field->data.string_value=0;
								} else { 
									*field->data.string_value=1;
								}
								if(!isEMPTY(f->value_expression))
								{
									temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
									if(temp_boolns) *field->data.string_value=TRUE;
										else *field->data.string_value=FALSE;
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case CHARACTERS:
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
								} else {
									*field->data.string_value=temp[f->start-1];
								}
								if(!isEMPTY(f->value_expression))
								{
									*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.float_value=DeCryptSignDbl(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.float_value=atof(stemp);
									}
								} else { 
									*field->data.float_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS_NOCENTS:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.float_value=DeCryptSignDbl(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.float_value=atof(stemp);
									}
								} else { 
									*field->data.float_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2)
									{
										*field->data.float_value=DeCryptSignDbl(stemp);
									} else if(f->use_quotes==14)
									{
										*field->data.float_value=DeCryptSignDbl(stemp)*100;
									} else if(f->use_quotes==4)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.float_value=atof(stemp);
									}
								} else { 
									*field->data.float_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SHORTV:
							case SSHORTV:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.short_value=DeCryptSignSht(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.short_value=CF_zoned2num(stemp,f->length,0);
									} else if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.short_value=Ratoi(stemp);
									}
								} else { 
									*field->data.short_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								memset(stemp,0,101);
								strncpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(!isEMPTY(stemp))
								{
									if(f->use_quotes==2 || f->use_quotes==14)
									{
										*field->data.integer_value=DeCryptSignInt(stemp);
									} else if(f->use_quotes==4)
									{
										*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
									} else  if(f->use_quotes>=10 && f->use_quotes<=13)
									{
										*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
									} else  if(f->use_quotes>=5 && f->use_quotes<=9)
									{
										*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
									} else {
										*field->data.integer_value=Ratoi(stemp);
									}
								} else { 
									*field->data.integer_value=0;
								}
								memset(stemp,0,RDAstrlen(stemp));
								if(!isEMPTY(f->value_expression))
								{
									*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							default:
								prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
								break;
						}
					} else {
						prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
					}
				}
			} else if(((f->start-1)>=0) && (f->use_quotes>2 && f->use_quotes<14)) { /* For conversions from IBM390 */
				if(!isEMPTY(f->expression))
				{
					skip_field=!PP_EVALUATEbol(f->expression,SCRNvirtualSubData,tmprsrc);
				} else {
					skip_field=FALSE;
				}
				if(!skip_field)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diagimport || diagimport_field)
					{
						prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					field=FLDNRD(importx->fileno,f->name);
					if(field!=NULL)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagimport || diagimport_field)
						{
							prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
								} else {
									strncpy(field->data.string_value,&temp[f->start-1],f->length);
								}
								field->data.string_value[f->length]=0;
								if(field->dlen>field->len && field->len>0)
								{
									field->data.string_value[field->len]=0;
								}
								unpad(field->data.string_value);
								if(!isEMPTY(f->value_expression))
								{
									temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
									if(field->data.string_value!=NULL) Rfree(field->data.string_value);
									field->data.string_value=stralloc(temp_string);
									field->dlen=RDAstrlen(temp_string)+1;
									if(temp_string!=NULL) Rfree(temp_string);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS_NOCENTS:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case DOLLARS:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SHORTV:
							case SSHORTV:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.short_value=CF_zoned2num(stemp,f->length,0);
								} else if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								memset(stemp,0,101);
								memcpy(stemp,&temp[f->start-1],f->length);
								stemp[f->length]=0;
								if(f->use_quotes==4)
								{
									*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
								} else  if(f->use_quotes>=10 && f->use_quotes<=13)
								{
									*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
								} else  if(f->use_quotes>=5 && f->use_quotes<=9)
								{
									*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case BOOLNS:
								memset(stemp,0,101);
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
								} else {
									stemp[0]=temp[f->start-1];
								}
								if(stemp[0]=='N' ||
									stemp[0]=='n' ||
									stemp[0]=='F' ||
									stemp[0]=='f' ||
									stemp[0]==' ' ||
									stemp[0]=='0')
								{
									*field->data.string_value=0;
								} else { 
									*field->data.string_value=1;
								}
								if(!isEMPTY(f->value_expression))
								{
									temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
									if(temp_boolns) *field->data.string_value=TRUE;
										else *field->data.string_value=FALSE;
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							case CHARACTERS:
								if(f->use_quotes==3)
								{
									CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
								} else {
									*field->data.string_value=temp[f->start-1];
								}
								if(!isEMPTY(f->value_expression))
								{
									*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
								}
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break;
							default:
								break;
						}
					} else {
						prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
					}
				}
			}
			}
		}
		skip_field=TRUE;
		if(mainrsrc!=NULL && importx->range)
		{
			testval=!testfilerangersrc(importx->fileno,importx->rs);
			if(testval)
			{
				for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
				{
					if(ADVtestvirtualrangersrc(v,importx->rs,SCRNvirtualSubData,mainrsrc))
					{
						testval=FALSE;
						skip_field=FALSE;
						break;
					}
				}
			}
		} else { 
			testval=TRUE;
		}
		if(testval)
		{
			if(!isEMPTY(importx->expression))
			{
				if(PP_EVALUATEbol(importx->expression,SCRNvirtualSubData,tmprsrc))
				{
					if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield))
					{
						if(!importx->skip_dups)
						{
							GetRDATData(importx->fileno,previous);
							CLEAR_SCREEN_VIRTUAL(tmprsrc);
							memset(stemp,0,101);
							for(x=0,f=importx->fields;x<importx->num;++x,++f)
							{
								if((f->start-1)>=0 && (f->start-1)<len_temp)
								{
									if(!isEMPTY(f->expression))
									{
										skip_field=!PP_EVALUATEbol(f->expression,
											SCRNvirtualSubData,tmprsrc);
									} else {
										skip_field=FALSE;
									}
									if(!skip_field)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										field=FLDNRD(importx->fileno,f->name);
										if(field!=NULL)
										{
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
												case CUSTOMTYPE:
												case EXPENDITURE:
												case REVENUE:
												case BALANCESHEET:
												case BEGINNINGBALANCE:
												case OPTIONALFIELDS:
													QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
													if(f->use_quotes==3)
													{
														CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
													} else {
														strncpy(field->data.string_value,&temp[f->start-1],f->length);
													}
													field->data.string_value[f->length]=0;
													if(field->dlen>field->len && field->len>0)
													{
														field->data.string_value[field->len]=0;
													}
													unpad(field->data.string_value);
													if(!isEMPTY(f->value_expression))
													{
														temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
														if(field->data.string_value!=NULL) Rfree(field->data.string_value);
														field->data.string_value=stralloc(temp_string);
														field->dlen=RDAstrlen(temp_string)+1;
														if(temp_string!=NULL) Rfree(temp_string);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case BOOLNS:
													memset(stemp,0,101);
													if(f->use_quotes==3)
													{
														CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
													} else {
														stemp[0]=temp[f->start-1];
													}
													if(stemp[0]=='N' ||
														stemp[0]=='n' ||
														stemp[0]=='F' ||
														stemp[0]=='f' ||
														stemp[0]==' ' ||
														stemp[0]=='0')
													{
														*field->data.string_value=0;
													} else { 
														*field->data.string_value=1;
													}
													if(!isEMPTY(f->value_expression))
													{
														temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
														if(temp_boolns) *field->data.string_value=TRUE;
															else *field->data.string_value=FALSE;
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case CHARACTERS:
													if(f->use_quotes==3)
													{
														CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
													} else {
														*field->data.string_value=temp[f->start-1];
													}
													if(!isEMPTY(f->value_expression))
													{
														*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DECIMALV:
												case DOUBLEV:
												case SDOUBLEV:
												case SDECIMALV:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.float_value=DeCryptSignDbl(stemp);
														} else if(f->use_quotes==4)
				{
															*field->data.float_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.float_value=atof(stemp);
														}

													} else { 
														*field->data.float_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS_NOCENTS:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.float_value=DeCryptSignDbl(stemp);
														} else if(f->use_quotes==4)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.float_value=atof(stemp);
														}

													} else { 
														*field->data.float_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2)
														{
															*field->data.float_value=DeCryptSignDbl(stemp);
														} else if(f->use_quotes==14)
														{
															*field->data.float_value=DeCryptSignDbl(stemp)*100;
														} else if(f->use_quotes==4)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.float_value=atof(stemp);
														}

													} else { 
														*field->data.float_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SHORTV:
												case SSHORTV:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.short_value=DeCryptSignSht(stemp);
														} else if(f->use_quotes==4)
				{
															*field->data.short_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else  if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.short_value=Ratoi(stemp);
														}
													} else { 
														*field->data.short_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SCROLLEDLIST:
												case LONGV:
												case SLONGV:
													memset(stemp,0,101);
													strncpy(stemp,&temp[f->start-1],f->length);
													stemp[f->length]=0;
													if(!isEMPTY(stemp))
													{
														if(f->use_quotes==2 || f->use_quotes==14)
														{
															*field->data.integer_value=DeCryptSignInt(stemp);
														} else if(f->use_quotes==4)
				{
															*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
														} else if(f->use_quotes>=10 && f->use_quotes<=13)
														{
															*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
														} else if(f->use_quotes>=5 && f->use_quotes<=9)
														{
															*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
														} else {
															*field->data.integer_value=Ratoi(stemp);
														}
													} else { 
														*field->data.integer_value=0;
													}
													memset(stemp,0,RDAstrlen(stemp));
													if(!isEMPTY(f->value_expression))
													{
														*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
													}
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												default:
													prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
													break;
											}
										} else {
											prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
										}
									}
								}
							}
							WRTTRANSsec(importx->fileno,0,NULL,previous);
						}
					} else {  /* if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield)) */
						ClearRDATData(previous);
						CLEAR_SCREEN_VIRTUAL(tmprsrc);
						memset(stemp,0,101);
						for(x=0,f=importx->fields;x<importx->num;++x,++f)
						{
							if(!isEMPTY(f->value_expression))
							{
								if((f->start-1)>=0 && (f->start-1)<len_temp)
								{
									if(!isEMPTY(f->expression))
									{
										skip_field=!PP_EVALUATEbol(f->expression,SCRNvirtualSubData,tmprsrc);
									} else {
										skip_field=FALSE;
									}
									if(!skip_field)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										field=FLDNRD(importx->fileno,f->name);
										if(field!=NULL)
										{
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
												case CUSTOMTYPE:
												case EXPENDITURE:
												case REVENUE:
												case BALANCESHEET:
												case BEGINNINGBALANCE:
												case OPTIONALFIELDS:
													temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
													if(!isEMPTY(temp_string))
													{
														QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
														strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
														field->data.string_value[RDAstrlen(temp_string)]=0;
														if(field->dlen>field->len && field->len>0)
														{
															field->data.string_value[field->len]=0;
														}
													} else {
														field->data.string_value[0]=0;
													}
													if(temp_string!=NULL) Rfree(temp_string);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case BOOLNS:
													temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													if(temp_boolns) *field->data.string_value=TRUE;
														else *field->data.string_value=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case CHARACTERS:
													*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DECIMALV:
												case DOUBLEV:
												case SDOUBLEV:
												case SDECIMALV:
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS_NOCENTS:
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case DOLLARS:
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SHORTV:
												case SSHORTV:
													*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												case SCROLLEDLIST:
												case LONGV:
												case SLONGV:
													*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
													if(diagimport || diagimport_field)
													{
														prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
													}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
													break;
												default:
													prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
													break;
											}
										} else {
											prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
										}
									}
								}
							}
						}
						if(!isEMPTY(trnfield))
						{
							WRTTRANSsec(importx->fileno,1,trnfield,NULL);
						} else {
							WRTTRANSsec(importx->fileno,0,NULL,previous);
						}
					}
				} else { 
					skip_field=FALSE;
				}
			} else { /* NO IMPORT EXPRESSION per LINE */
				if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield))
				{
					if(!importx->skip_dups)
					{
						GetRDATData(importx->fileno,previous);
						CLEAR_SCREEN_VIRTUAL(tmprsrc);
						memset(stemp,0,101);
						for(x=0,f=importx->fields;x<importx->num;++x,++f)
						{
							if((f->start-1)>=0 && (f->start-1)<len_temp)
							{
								if(!isEMPTY(f->expression))
								{
									skip_field=!PP_EVALUATEbol(f->expression,
										SCRNvirtualSubData,tmprsrc);
								} else {
									skip_field=FALSE;
								}
								if(!skip_field)
								{
#ifdef USE_RDA_DIAGNOSTICS
									if(diagimport || diagimport_field)
									{
										prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
									field=FLDNRD(importx->fileno,f->name);
									if(field!=NULL)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
												QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
												} else {
													strncpy(field->data.string_value,&temp[f->start-1],f->length);
												}
												field->data.string_value[f->length]=0;
												if(field->dlen>field->len && field->len>0)
												{
													field->data.string_value[field->len]=0;
												}
												unpad(field->data.string_value);
												if(!isEMPTY(f->value_expression))
												{
													temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
													if(!isEMPTY(temp_string))
													{
														QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
														strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
														field->data.string_value[RDAstrlen(temp_string)]=0;
														if(field->dlen>field->len && field->len>0)
														{
															field->data.string_value[field->len]=0;
														}
													} else if(field->data.string_value!=NULL)
													{
														field->data.string_value[0]=0;
													}
													if(temp_string!=NULL) Rfree(temp_string);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case BOOLNS:
												memset(stemp,0,101);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
												} else {
													stemp[0]=temp[f->start-1];
												}
												if(stemp[0]=='N' ||
													stemp[0]=='n' ||
													stemp[0]=='F' ||
													stemp[0]=='f' ||
													stemp[0]==' ' ||
													stemp[0]=='0')
												{
													*field->data.string_value=0;
												} else { 
													*field->data.string_value=1;
												}
												if(!isEMPTY(f->value_expression))
												{
													temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													if(temp_boolns) *field->data.string_value=TRUE;
														else *field->data.string_value=FALSE;
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case CHARACTERS:
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
												} else {
													*field->data.string_value=temp[f->start-1];
												}
												if(!isEMPTY(f->value_expression))
												{
													*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.float_value=DeCryptSignDbl(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.float_value=atof(stemp);
													}
												} else { 
													*field->data.float_value=0;
												}	
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS_NOCENTS:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.float_value=DeCryptSignDbl(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.float_value=atof(stemp);
													}
												} else { 
													*field->data.float_value=0;
												}	
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2)
													{
														*field->data.float_value=DeCryptSignDbl(stemp);
													} else if(f->use_quotes==14)
													{
														*field->data.float_value=DeCryptSignDbl(stemp)*100;
													} else if(f->use_quotes==4)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.float_value=atof(stemp);
													}
												} else { 
													*field->data.float_value=0;
												}	
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SHORTV:
											case SSHORTV:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.short_value=DeCryptSignSht(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.short_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else  if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.short_value=Ratoi(stemp);
													}
												} else { 
													*field->data.short_value=0;
												}
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SCROLLEDLIST:
											case LONGV:
											case SLONGV:
												memset(stemp,0,101);
												strncpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(!isEMPTY(stemp))
												{
													if(f->use_quotes==2 || f->use_quotes==14)
													{
														*field->data.integer_value=DeCryptSignInt(stemp);
													} else if(f->use_quotes==4)
													{
														*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
													} else if(f->use_quotes>=10 && f->use_quotes<=13)
													{
														*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
													} else  if(f->use_quotes>=5 && f->use_quotes<=9)
													{
														*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
													} else {
														*field->data.integer_value=Ratoi(stemp);
													}
												} else { 
													*field->data.integer_value=0;
												}
												memset(stemp,0,RDAstrlen(stemp));
												if(!isEMPTY(f->value_expression))
												{
													*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											default:
												prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
												break;
										}
									} else {
										prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
									}
								}
							} else if(((f->start-1)>=0) && (f->use_quotes>3 && f->use_quotes<14)) /* For conversions from IBM390 */
							{
								if(!isEMPTY(f->expression))
								{
									skip_field=!PP_EVALUATEbol(f->expression,
										SCRNvirtualSubData,tmprsrc);
								} else {
									skip_field=FALSE;
								}
								if(!skip_field)
								{
#ifdef USE_RDA_DIAGNOSTICS
									if(diagimport || diagimport_field)
									{
										prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
									field=FLDNRD(importx->fileno,f->name);
									if(field!=NULL)
									{
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
												QUICKALLOC(field->data.string_value,field->dlen,f->length+1);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],(field->dlen<f->length ? field->dlen:f->length),e2a_table);
												} else {
													strncpy(field->data.string_value,&temp[f->start-1],f->length);
												}
												field->data.string_value[f->length]=0;
												if(field->dlen>field->len && field->len>0)
												{
													field->data.string_value[field->len]=0;
												}
												unpad(field->data.string_value);
												if(!isEMPTY(f->value_expression))
												{
													temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
													if(!isEMPTY(temp_string))
													{
														QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
														strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
														field->data.string_value[RDAstrlen(temp_string)]=0;
														if(field->dlen>field->len && field->len>0)
														{
															field->data.string_value[field->len]=0;
														}
													} else if(field->data.string_value!=NULL)
													{
														field->data.string_value[0]=0;
													}
													if(temp_string!=NULL) Rfree(temp_string);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS_NOCENTS:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case DOLLARS:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.float_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.float_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SHORTV:
											case SSHORTV:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==2)
												{
													*field->data.short_value=DeCryptSignSht(stemp);
												} else if(f->use_quotes==4)
												{
													*field->data.short_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.short_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else  if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.short_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case SCROLLEDLIST:
											case LONGV:
											case SLONGV:
												memset(stemp,0,101);
												memcpy(stemp,&temp[f->start-1],f->length);
												stemp[f->length]=0;
												if(f->use_quotes==4)
												{
													*field->data.integer_value=CF_zoned2num(stemp,f->length,0);
												} else if(f->use_quotes>=10 && f->use_quotes<=13)
												{
													*field->data.integer_value=CF_zoned2num(stemp,f->length,(f->use_quotes-9));
												} else  if(f->use_quotes>=5 && f->use_quotes<=9)
												{
													*field->data.integer_value=CF_packed2num(stemp,f->length,(f->use_quotes-5));
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case BOOLNS:
												memset(stemp,0,101);
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(stemp,&temp[f->start-1],1,e2a_table);
												} else {
													stemp[0]=temp[f->start-1];
												}
												if(stemp[0]=='N' ||
													stemp[0]=='n' ||
													stemp[0]=='F' ||
													stemp[0]=='f' ||
													stemp[0]==' ' ||
													stemp[0]=='0')
												{
													*field->data.string_value=0;
												} else { 
													*field->data.string_value=1;
												}
												if(!isEMPTY(f->value_expression))
												{
													temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
													if(temp_boolns) *field->data.string_value=TRUE;
														else *field->data.string_value=FALSE;
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
											case CHARACTERS:
												if(f->use_quotes==3)
												{
													CF_fcs_xlate(field->data.string_value,&temp[f->start-1],1,e2a_table);
												} else {
													*field->data.string_value=temp[f->start-1];
												}
												if(!isEMPTY(f->value_expression))
												{
													*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
												}
#ifdef USE_RDA_DIAGNOSTICS
												if(diagimport || diagimport_field)
												{
													prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
												}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
												break;
										}
									} else {
										prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
									}
								}
							}
						}
						WRTTRANSsec(importx->fileno,0,NULL,previous);
					}
				} else {  /* if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield)) */
					ClearRDATData(previous);
					CLEAR_SCREEN_VIRTUAL(tmprsrc);
					memset(stemp,0,101);
					for(x=0,f=importx->fields;x<importx->num;++x,++f)
					{
						if((f->start-1)>=0 && (f->start-1)<len_temp)
						{
							if(!isEMPTY(f->expression))
							{
								skip_field=!PP_EVALUATEbol(f->expression,
									SCRNvirtualSubData,tmprsrc);
							} else {
								skip_field=FALSE;
							}
							if(!skip_field)
							{
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								field=FLDNRD(importx->fileno,f->name);
								if(field!=NULL)
								{
#ifdef USE_RDA_DIAGNOSTICS
									if(diagimport || diagimport_field)
									{
										prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
										case CUSTOMTYPE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:
											if(!isEMPTY(f->value_expression))
											{
												temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
												if(!isEMPTY(temp_string))
												{
													QUICKALLOC(field->data.string_value,field->dlen,RDAstrlen(temp_string)+1);
													strncpy(field->data.string_value,temp_string,RDAstrlen(temp_string));
													field->data.string_value[RDAstrlen(temp_string)]=0;
													if(field->dlen>field->len && field->len>0)
													{
														field->data.string_value[field->len]=0;
													}
												} else {
													field->data.string_value[0]=0;
												}
												if(temp_string!=NULL) Rfree(temp_string);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case BOOLNS:
											if(!isEMPTY(f->value_expression))
											{
												temp_boolns=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
												if(temp_boolns) *field->data.string_value=TRUE;
													else *field->data.string_value=FALSE;
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case CHARACTERS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
										case SDECIMALV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DOLLARS_NOCENTS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DOLLARS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case SHORTV:
										case SSHORTV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										default:
											prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
											break;
									}
								} else {
									prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
								}
							}
						}
					}
					if(!isEMPTY(trnfield))
					{
						WRTTRANSsec(importx->fileno,1,trnfield,NULL);
					} else {
						WRTTRANSsec(importx->fileno,0,NULL,previous);
					}
				}
			}
		}
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(skip_field ? TRUE:FALSE));
		}
	}
	}
	if(tmprsrc!=NULL)
	{
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic)
		{
			killwindow(tmprsrc);
		}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(h!=NULL) Rfree(h);
	if(trnfield!=NULL) Rfree(trnfield);
	if(previous!=NULL) FreeRDATData(previous);
	fclose(fp);
	return(0);
}
char *READIEfield(char **string,char *fdel,short use_quotes,
	short *exit)
{
	int count=0,len_fdel=0;
	char *tread=NULL,*tmp=NULL,quotes=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		if(RDAstrlen(*string)>100)
		{
			prterr("DIAG READIEfield starting at [%.100s].",*string);
		} else {
			prterr("DIAG READIEfield starting at [%s].",*string);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!*exit)
	{
		quotes=FALSE;
		len_fdel=RDAstrlen(fdel);
		while(TRUE)
		{
			tread=*string;
			if(!(*tread)) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagimport) 
				{ 
					prterr("DIAG READFIELD reached end of file."); 
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				*exit=TRUE;
				break;
			}
			if(use_quotes==TRUE && !quotes && *tread=='\"') 
			{
				quotes=TRUE;
			} else if(use_quotes==TRUE && quotes && strchr("\"",*tread))
			{
				quotes=FALSE;
			} else if(!quotes && !strncmp(tread,fdel,len_fdel) && count>0) 
			{
				++(*string);
				break;
			} else if(!quotes && !strncmp(tread,fdel,len_fdel))
			{
				++(*string);
				break;
			} else {
/* HERE */
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+2);
				} else tmp=Rmalloc(2);
				tmp[count]=*tread;
				tmp[count+1]=0;
				++count;
			}
			++(*string);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		if(RDAstrlen(*string)>100)
		{
			prterr("DIAG READIEfield returning [%s] Pointing to [%.100s].",(tmp!=NULL ? tmp:"NULL"),(*string!=NULL ? *string:""));
		} else {
			prterr("DIAG READIEfield returning [%s] Pointing to [%s].",(tmp!=NULL ? tmp:"NULL"),(*string!=NULL ? *string:""));
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(tmp);
}
char *xComputeDelimiter(char *string,int line,char *file)
{
	char *d=NULL,*temp=NULL,*holdx=NULL,*hold=NULL;
	int x=0,y=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		prterr("DIAG ComputeDelimiter with [%s] at line [%d] program [%s].",(string!=NULL ? string:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	prterr("DIAG ComputeDelimiter with [%s] at line [%d] program [%s].",(string!=NULL ? string:""),line,file);
	if(!isEMPTY(string))
	{
		d=Rmalloc(RDAstrlen(string)+1);
		memset(d,0,RDAstrlen(string)+1);
		holdx=d;
		hold=stralloc(string);
		memset(hold,0,RDAstrlen(hold)+1);
		temp=string;
		while(*temp!='\0')
		{
			if(*temp=='\\')
			{
				y=0;
				++temp;
				if(!*temp) break;
				if(*temp=='t')
				{
					*holdx='\t';
					++temp;
					++holdx;
				} else if(*temp=='r')
				{
					*holdx='\r';
					++temp;
					++holdx;
				} else if(*temp=='n')
				{
					*holdx='\n';
					++temp;
					++holdx;
				} else if(*temp=='f')
				{
					*holdx='\f';
					++temp;
					++holdx;
				} else if(isdigit(*temp))
				{
					while(isdigit(*temp))
					{
						hold[y]=*temp;
						++temp;++y;
					}
					hold[y]=0;
					if(!isEMPTY(hold)) x=Ratoi(hold);
						else x=0;
					if(x>0 && x<255) 
					{
						*holdx=x;
						++holdx;
					}
				}
			} else {
				*holdx=*temp;
				++temp;
				++holdx;
			}
		}
		if(hold!=NULL) Rfree(hold);
	}
	unpad(d);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		prterr("DIAG ComputeDelimiter has [%s] at line [%d] program [%s].",(d!=NULL ? d:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	prterr("DIAG ComputeDelimiter has [%s] at line [%d] program [%s].",(d!=NULL ? d:""),line,file);
	return(d);
}
static short importdelimitedfile(RDArsrc *mainrsrc,IEfile *importx)
{
	FILE *fp;
	char *e,*fdel=NULL,*rdel=NULL,*retval=NULL,*ptr=NULL,*temp2=NULL;
	short x,skip_field=FALSE,testval=FALSE,exitstatus=FALSE;
	IEfield *f;
	NRDfield *field;
	RDArsrc *tmprsrc=NULL;
	int counters=0,len_retval=0;
	RDAvirtual *v;
	RDATData *previous=NULL;
	long total=0;
	char *ImportLine=NULL;
	HoldIMPEXP *h=NULL;
	char *temp_string=NULL;
	char *trnfield=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG importdelimited file Import Delimited File Using Definition [%s].",importx->name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	memset(stemp,0,101);
	counters=CountLines(importx->ifile);
	temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(importx->ifile)+2);
	sprintf(temp2,"%s/%s",CURRENTDIRECTORY,importx->ifile);
#ifndef WIN32
	fp=fopen(temp2,"rb");
#else
	fp=fopen(temp2,"rbc");
#endif
	if(temp2!=NULL) Rfree(temp2);
	if(fp==NULL) 
	{
		prterr("Error Attempting to open and read file [%s] to import from.",
			importx->ifile);
		return(-1);
	}
	fdel=ComputeDelimiter(importx->fdel);
	rdel=ComputeDelimiter(importx->rdel);
	e=Rmalloc(RDAstrlen(importx->file)+RDAstrlen(importx->module)+25);
	sprintf(e,"Importing Module %s File %s",importx->module,importx->file);
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","DATABASE",e,stop_diagnostic,counters);
	addDFincvir(tmprsrc,importx->module,importx->file,NULL,importx->fileno);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	previous=RDATDataNEW(importx->fileno);
	if(e!=NULL) Rfree(e);
	e=NULL;
#ifdef __USE_DIAGNOSTIC_SCREENS__
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(importx->module)+RDAstrlen(importx->name)+15);
			sprintf(e,"%s %s IMPORTING",importx->module,importx->name);
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
	for(x=0,f=importx->fields;x<importx->num;++x,++f)
	{
		if(!isEMPTY(f->value_expression))
		{
			if(!RDAstrcmp(f->value_expression,"[NEXT TRANSACTION NO]"))
			{
				trnfield=stralloc(f->name);
				break;
			}
		}
	}
	h=xHoldIMPEXPNew(tmprsrc,0);
	h->record_counter=0;
	total=0;
	ImportLine=readDelimitedline(fp,rdel,&total); 
	while(ImportLine!=NULL)
	{
		++h->record_counter;
		ZERNRD(importx->fileno);
		CLEAR_SCREEN_VIRTUAL(tmprsrc);
		exitstatus=FALSE;
		memset(stemp,0,101);
		ptr=ImportLine;
		if(abort_diagnostic) break;
		for(x=0,f=importx->fields;x<importx->num;++x,++f)
		{
			if(retval!=NULL) Rfree(retval);
			if(exitstatus && isEMPTY(f->expression))
			{
				exitstatus=FALSE;
				break;
			}
			retval=READIEfield(&ptr,fdel,f->use_quotes,&exitstatus);
			if(!isEMPTY(retval))
			{
				len_retval=RDAstrlen(retval);
			} else {
				len_retval=0;
			}
			if(!isEMPTY(f->expression))
			{
				skip_field=!PP_EVALUATEbol(f->expression,SCRNvirtualSubData,tmprsrc);
			} else {
				skip_field=FALSE;
			}
			if((!skip_field && len_retval>0) || (!skip_field && !isEMPTY(f->expression))) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagimport || diagimport_field)
				{
					prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				field=FLDNRD(importx->fileno,f->name);
				if(field!=NULL)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diagimport || diagimport_field)
					{
						prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:	
							if(len_retval>0)
							{
							QUICKALLOC(field->data.string_value,field->dlen,len_retval+1);
							memcpy(field->data.string_value,retval,len_retval+1);
							if(field->dlen>field->len && field->len>0)
							{
								field->data.string_value[field->len]=0;
							}
							unpad(field->data.string_value);
							} else if(!isEMPTY(field->data.string_value))
							{
								memset(field->data.string_value,0,field->dlen);
							} 
							if(!isEMPTY(f->value_expression))
							{
								temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
								if(field->data.string_value!=NULL) Rfree(field->data.string_value);
								field->data.string_value=stralloc(temp_string);
								if(!isEMPTY(temp_string)) field->dlen=RDAstrlen(temp_string)+1;
									else field->dlen=0;
								if(temp_string!=NULL) Rfree(temp_string);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						case BOOLNS:
							if(!isEMPTY(retval))
							{
							if(retval[0]=='N' || retval[0]=='F' ||
								retval[0]=='n' || retval[0]=='f' ||
								retval[0]==' ' || retval[0]=='0')
							{
								*field->data.string_value=0;
							} else *field->data.string_value=1;
							} else *field->data.string_value=0;
							if(!isEMPTY(f->value_expression))
							{
								*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						case CHARACTERS:
							if(!isEMPTY(retval))
							{
								*field->data.string_value=retval[0];
							} else *field->data.string_value=0;
							if(!isEMPTY(f->value_expression))
							{
								*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(!isEMPTY(retval))
							{
							*field->data.float_value=atof(retval);
							} else *field->data.float_value=0;
							if(!isEMPTY(f->value_expression))
							{
								*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						case DOLLARS_NOCENTS:
							if(!isEMPTY(retval))
							{
							*field->data.float_value=atof(retval);
							} else *field->data.float_value=0;
							if(!isEMPTY(f->value_expression))
							{
								*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						case DOLLARS:
							if(!isEMPTY(retval))
							{
								if(f->use_quotes==14)
								{
									*field->data.float_value=atof(retval)*100;
								} else *field->data.float_value=atof(retval);
							} else *field->data.float_value=0;
							if(!isEMPTY(f->value_expression))
							{
								*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						case SHORTV:
						case SSHORTV:
							if(!isEMPTY(retval))
							{
							*field->data.short_value=Ratoi(retval);
							} else *field->data.short_value=0;
							if(!isEMPTY(f->value_expression))
							{
								*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(!isEMPTY(retval))
							{
							*field->data.integer_value=Ratoi(retval);
							} else *field->data.integer_value=0;
							if(!isEMPTY(f->value_expression))
							{
								*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							break;
						default:
							prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
							break;
					}
				} else {
					prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
				}
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diagimport)
				{
					prterr("DIAG Field [%s] Skipped because of Field Expression returned FALSE.",f->name);
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			}
		}
		if(retval!=NULL) Rfree(retval);
		skip_field=TRUE;
		if(mainrsrc!=NULL && importx->range)
		{
			testval=!testfilerangersrc(importx->fileno,importx->rs);
			if(testval)
			{
				for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
				{
					if(ADVtestvirtualrangersrc(v,importx->rs,SCRNvirtualSubData,mainrsrc))
					{
						testval=FALSE;
						skip_field=FALSE;
						break;
					}
				}
			}
		} else {
			testval=TRUE;
		}
		if(testval)
		{
			if(!isEMPTY(importx->expression))
			{
				if(PP_EVALUATEbol(importx->expression,SCRNvirtualSubData,mainrsrc))
				{
					if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield))
					{
						if(!importx->skip_dups)
						{
							GetRDATData(importx->fileno,previous);
							CLEAR_SCREEN_VIRTUAL(tmprsrc);
							exitstatus=FALSE;
							memset(stemp,0,101);
							ptr=ImportLine;
							for(x=0,f=importx->fields;x<importx->num;++x,++f)
							{
								if(retval!=NULL) Rfree(retval);
								if(exitstatus && isEMPTY(f->expression)) 
								{
									exitstatus=FALSE;
									break;
								}
								retval=READIEfield(&ptr,fdel,f->use_quotes,&exitstatus);
								len_retval=RDAstrlen(retval);
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								field=FLDNRD(importx->fileno,f->name);
								if(field!=NULL)
								{
#ifdef USE_RDA_DIAGNOSTICS
									if(diagimport || diagimport_field)
									{
										prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
										case CUSTOMTYPE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:	
											if(!isEMPTY(f->value_expression))
											{
												temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
												if(field->data.string_value!=NULL) Rfree(field->data.string_value);
												field->data.string_value=stralloc(temp_string);
												if(!isEMPTY(temp_string)) field->dlen=RDAstrlen(temp_string)+1;
													else field->dlen=0;
												if(temp_string!=NULL) Rfree(temp_string);
											} else {
												if(!isEMPTY(retval))
												{
													QUICKALLOC(field->data.string_value,field->dlen,len_retval+1);
													memcpy(field->data.string_value,retval,len_retval+1);
													if(field->dlen>field->len && field->len>0)
													{
														field->data.string_value[field->len]=0;
													}
													unpad(field->data.string_value);
												} else if(!isEMPTY(field->data.string_value))
												{
													memset(field->data.string_value,0,field->dlen);
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case BOOLNS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
											} else {
												if(!isEMPTY(retval))
												{
													if(retval[0]=='N' || retval[0]=='F' ||
														retval[0]=='n' || retval[0]=='f' ||
														retval[0]==' ' || retval[0]=='0')
													{
														*field->data.string_value=0;
													} else { 
														*field->data.string_value=1;
													}
												} else { 
													*field->data.string_value=0;
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case CHARACTERS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
											} else {
												if(!isEMPTY(retval))
												{
													*field->data.string_value=retval[0];
												} else { 
													*field->data.string_value=0;
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
										case SDECIMALV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											} else {
												if(!isEMPTY(retval))
												{
													*field->data.float_value=atof(retval);
												} else { 
													*field->data.float_value=0;
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DOLLARS_NOCENTS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											} else {
												if(!isEMPTY(retval))
												{
													*field->data.float_value=atof(retval);
												} else { 
													*field->data.float_value=0;
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case DOLLARS:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
											} else {
												if(!isEMPTY(retval))
												{
													*field->data.float_value=atof(retval);
												} else { 
													*field->data.float_value=0;
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case SHORTV:
										case SSHORTV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
											} else {
												if(!isEMPTY(retval))
												{
													*field->data.short_value=Ratoi(retval);
												} else { 
													*field->data.short_value=0;
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(!isEMPTY(f->value_expression))
											{
												*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
											} else {
												if(!isEMPTY(retval))
												{
													*field->data.integer_value=Ratoi(retval);
												} else { 
													*field->data.integer_value=0;
												}
											}
#ifdef USE_RDA_DIAGNOSTICS
											if(diagimport || diagimport_field)
											{
												prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
											}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
											break;
										default:
											prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
											break;
									}
								} else {
									prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
								}
							}
							if(retval!=NULL) Rfree(retval);
							WRTTRANSsec(importx->fileno,0,NULL,previous);
						}
					} else { /* if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield)) */
						ClearRDATData(previous);
						if(!isEMPTY(trnfield))
						{
							WRTTRANSsec(importx->fileno,1,trnfield,previous);
						} else {
							WRTTRANSsec(importx->fileno,0,NULL,previous);
						}
					}
				} else {
					skip_field=FALSE;
				}
			} else {
				if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield))
				{
					if(!importx->skip_dups)
					{
						GetRDATData(importx->fileno,previous);
						CLEAR_SCREEN_VIRTUAL(tmprsrc);
						exitstatus=FALSE;
						memset(stemp,0,101);
						ptr=ImportLine;
						for(x=0,f=importx->fields;x<importx->num;++x,++f)
						{
							retval=READIEfield(&ptr,fdel,f->use_quotes,&exitstatus);
							len_retval=RDAstrlen(retval);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagimport || diagimport_field)
							{
								prterr("DIAG Importing Field [%s] Type [%d] [%s] Length [%d] ",f->name,f->type,standardfieldtypes[f->type],f->length);
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							field=FLDNRD(importx->fileno,f->name);
							if(field!=NULL)
							{
#ifdef USE_RDA_DIAGNOSTICS
								if(diagimport || diagimport_field)
								{
									prterr("DIAG DATABASE FILE [%s] Field [%s] Type [%d] [%s] Length [%d].",FILENAME(importx->fileno),field->name,field->type,standardfieldtypes[field->type],field->len);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:	
										if(!isEMPTY(f->value_expression))
										{
											temp_string=PP_EVALUATEstr(f->value_expression,IMPEXPGenSubData,h);
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=stralloc(temp_string);
											if(temp_string!=NULL) Rfree(temp_string);
										} else {
											if(!isEMPTY(retval))
											{
												QUICKALLOC(field->data.string_value,field->dlen,len_retval+1);
												memcpy(field->data.string_value,retval,len_retval+1);
												if(field->dlen>field->len && field->len>0)
												{
													field->data.string_value[field->len]=0;
												}
												unpad(field->data.string_value);
											} else if(!isEMPTY(field->data.string_value))
											{
												memset(field->data.string_value,0,field->dlen);
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(field->data.string_value!=NULL ? field->data.string_value:""));
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									case BOOLNS:
										if(!isEMPTY(f->value_expression))
										{
											*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
										} else {
											if(!isEMPTY(retval))
											{
												if(retval[0]=='N' || retval[0]=='F' ||
													retval[0]=='n' || retval[0]=='f' ||
													retval[0]==' ' || retval[0]=='0')
												{
													*field->data.string_value=0;
												} else { 
													*field->data.string_value=1;
												}
											} else { 
												*field->data.string_value=0;
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%s] ",field->name,(*field->data.string_value ? "True":"False"));
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									case CHARACTERS:
										if(!isEMPTY(f->value_expression))
										{
											*field->data.string_value=PP_EVALUATEbol(f->value_expression,IMPEXPGenSubData,h);
										} else {
											if(!isEMPTY(retval))
											{
												*field->data.string_value=retval[0];
											} else { 
												*field->data.string_value=0;
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%c] ",field->name,*field->data.string_value);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(!isEMPTY(f->value_expression))
										{
											*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
										} else {
											if(!isEMPTY(retval))
											{
												*field->data.float_value=atof(retval);
											} else { 
												*field->data.float_value=0;
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									case DOLLARS_NOCENTS:
										if(!isEMPTY(f->value_expression))
										{
											*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
										} else {
											if(!isEMPTY(retval))
											{
												*field->data.float_value=atof(retval);
											} else { 
												*field->data.float_value=0;
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									case DOLLARS:
										if(!isEMPTY(f->value_expression))
										{
											*field->data.float_value=PP_EVALUATEdbl(f->value_expression,IMPEXPGenSubData,h);
										} else {
											if(!isEMPTY(retval))
											{
												*field->data.float_value=atof(retval);
											} else { 
												*field->data.float_value=0;
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%f] ",field->name,*field->data.float_value);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									case SHORTV:
									case SSHORTV:
										if(!isEMPTY(f->value_expression))
										{
											*field->data.short_value=PP_EVALUATEsht(f->value_expression,IMPEXPGenSubData,h);
										} else {
											if(!isEMPTY(retval))
											{
												*field->data.short_value=Ratoi(retval);
											} else {
												*field->data.short_value=0;
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.short_value);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(!isEMPTY(f->value_expression))
										{
											*field->data.integer_value=PP_EVALUATEint(f->value_expression,IMPEXPGenSubData,h);
										} else {
											if(!isEMPTY(retval))
											{
												*field->data.integer_value=Ratoi(retval);
											} else {
												*field->data.integer_value=0;
											}
										}
#ifdef USE_RDA_DIAGNOSTICS
										if(diagimport || diagimport_field)
										{
											prterr("DIAG Import Database Field [%s] Value [%d] ",field->name,*field->data.integer_value);
										}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
										break;
									default:
										prterr("Error field type [%d] not found for field [%s] file [%s].",field->type,field->name,FILENAME(importx->fileno));
										break;
								}
							} else {
								prterr("Error Field [%s] not found in File [%s][%s].",f->name,MODULENAME(importx->fileno),FILENAME(importx->fileno));
							}
							if(exitstatus)
							{
								exitstatus=FALSE;
								break;
							}
							if(retval!=NULL) Rfree(retval);
						}
						WRTTRANSsec(importx->fileno,0,NULL,previous);
					}
				} else {  /* if(!EQLNRDsec(importx->fileno,1) && isEMPTY(trnfield)) */
					ClearRDATData(previous);
					if(!isEMPTY(trnfield))
					{
						WRTTRANSsec(importx->fileno,1,trnfield,previous);
					} else {
						WRTTRANSsec(importx->fileno,0,NULL,previous);
					}
				}
			}
		}
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(skip_field ? TRUE:FALSE));
		}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		if(ImportLine!=NULL) Rfree(ImportLine);
		ImportLine=readDelimitedline(fp,rdel,&total);
	}
	if(ImportLine!=NULL) Rfree(ImportLine);
	if(tmprsrc!=NULL)
	{
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic)
		{
			killwindow(tmprsrc);
		}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(previous!=NULL) FreeRDATData(previous);
	if(h!=NULL) Rfree(h);
	fclose(fp);
	if(trnfield!=NULL) Rfree(trnfield);
	if(rdel!=NULL) Rfree(rdel);
	if(fdel!=NULL) Rfree(fdel);
	return(0);
}
static short importdata(RDArsrc *mainrsrc,IEfile *importx)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		prterr("DIAG importdata Importing From file [%s] to Database File [%s] [%s].",importx->ifile,importx->module,importx->file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!importx->type)
	{
		return(importflatfile(mainrsrc,importx));
	} else {
		return(importdelimitedfile(mainrsrc,importx));
	}
}
static short exportflatfile(RDArsrc *mainrsrc,IEfile *importx)
{
	FILE *fp;
	char *xtemp,*e,*xtemp1=NULL,*temp2=NULL;
	short x,ef=FALSE,testval=FALSE;
	IEfield *f;
	NRDfield *field;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v;
	double pn=0.0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG exportflatfile Exporting Database File [%s] [%s] to a flat text file [%s].",importx->module,importx->file,importx->ifile);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	memset(stemp,0,101);
	temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(importx->ifile)+2);
	sprintf(temp2,"%s/%s",CURRENTDIRECTORY,importx->ifile);
	unlink(temp2);
#ifndef WIN32
	fp=fopen(temp2,"a+b");
#else
	fp=fopen(temp2,"abc");
#endif
	if(temp2!=NULL) Rfree(temp2);
	if(fp==NULL) 
	{
		prterr("Error Attempting to open/create file [%s] to export to.",
			importx->ifile);
		return(-1);
	}
	e=Rmalloc(RDAstrlen(importx->file)+RDAstrlen(importx->module)+25);
	sprintf(e,"Exporting Module %s File %s",importx->module,importx->file);
	tmprsrc=diagscrn(importx->fileno,"DIAGNOSTIC SCREEN","DATABASE",e,stop_diagnostic);
	if(e!=NULL) Rfree(e);
	e=NULL;
	addDFincvir(tmprsrc,MODULENAME(importx->fileno),FILENAME(importx->fileno),NULL,importx->fileno);
#ifdef __USE_DIAGNOSTIC_SCREENS__
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(importx->module)+RDAstrlen(importx->name)+15);
			sprintf(e,"%s %s EXPORTING",importx->module,importx->name);
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
	ZERNRD(importx->fileno);
	ef=FRSNRDsec(importx->fileno,1);
	while(!ef)
	{
		if(abort_diagnostic) break;
		if(importx->range)
		{
			testval=!testfilerangersrc(importx->fileno,importx->rs);
			if(testval)
			{
				if(mainrsrc->virflds!=NULL)
				{
					for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
					{
						if(ADVtestvirtualrangersrc(v,importx->rs,SCRNvirtualSubData,mainrsrc))
						{
							testval=FALSE;
							break;
						}
					}
				}
			}
		} else testval=TRUE;
		if(testval)
		{
			if(!isEMPTY(importx->expression))
			{
				testval=PP_EVALUATEbol(importx->expression,SCRNvirtualSubData,tmprsrc);
			} 
		} 
		if(testval)
		{
			for(x=0,f=importx->fields;x<importx->num;++x,++f)
			{
				field=FLDNRD(importx->fileno,f->name);
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
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(field->data.string_value!=NULL)
							{
								if(RDAstrlen(field->data.string_value)>=f->length)
								{
									xtemp=stralloc(field->data.string_value);
									if(RDAstrlen(xtemp)>f->length) xtemp[f->length]=0;
								} else {
									xtemp=Rmalloc(f->length+1);
									sprintf(xtemp,"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
								}
							} else xtemp=NULL;
							if(f->use_quotes==3)
							{
								xtemp1=Rmalloc(f->length+1);
								if(xtemp==NULL) 
								{
									xtemp=Rmalloc(f->length+1);
									memset(xtemp,0,f->length+1);
									memset(xtemp,' ',f->length);
								}
								pad(xtemp,f->length);
								CF_fcs_xlate(xtemp1,xtemp,f->length,a2e_table);
								fwrite(xtemp1,f->length,1,fp);
							} else {
								xtemp1=stralloc(xtemp);
								fprintf(fp,"%-*s",f->length,(xtemp1!=NULL ? xtemp1:""));
							}
							if(xtemp!=NULL) Rfree(xtemp);
							if(xtemp1!=NULL) Rfree(xtemp1);
							break;
						case BOOLNS:
							if(f->use_quotes==3)
							{
								xtemp=Rmalloc(2);
								xtemp1=Rmalloc(2);
								sprintf(xtemp,"%c",(*field->data.string_value ? 'Y':'N'));
								CF_fcs_xlate(xtemp1,xtemp,1,a2e_table);
								fwrite(xtemp1,1,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
								if(xtemp!=NULL) Rfree(xtemp);
							} else {
								fprintf(fp,"%c",(*field->data.string_value ? 'Y':'N'));
							}
							break;
						case CHARACTERS:
							if(f->use_quotes==3)
							{
								xtemp1=Rmalloc(2);
								memset(stemp,0,101);
								sprintf(stemp,"%c",*field->data.string_value);
								CF_fcs_xlate(xtemp1,stemp,1,a2e_table);
								fwrite(xtemp1,1,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else fprintf(fp,"%c",*field->data.string_value);
							break;
						case DOLLARS_NOCENTS:
							if(f->use_quotes==2)
							{
							} else if(f->use_quotes==4)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,0);
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=10 && f->use_quotes<=13)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,(f->use_quotes-9));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=5 && f->use_quotes<=9)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2packed(xtemp1,*field->data.float_value,f->length,f->use_quotes-5,(*field->data.float_value>=0.0 ? TRUE:FALSE));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"%*.0f",f->length,*field->data.float_value);
							} else {
								fprintf(fp,"%0*.0f",f->length,*field->data.float_value);
							}
							break;
						case DOLLARS:
							if(f->use_quotes==2)
							{
							} else if(f->use_quotes==4)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,0);
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=10 && f->use_quotes<=13)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,(f->use_quotes-9));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=5 && f->use_quotes<=9)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2packed(xtemp1,*field->data.float_value,f->length,f->use_quotes-5,(*field->data.float_value>=0.0 ? TRUE:FALSE));
								fwrite(xtemp1,f->length,1,fp);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"%*.02f",f->length,*field->data.float_value/100);
							} else {
								fprintf(fp,"%0*.0f",f->length,*field->data.float_value);
							}
							break;
						case DOUBLEV:
						case SDOUBLEV:
							if(f->use_quotes==2)
							{
							} else if(f->use_quotes==4)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,0);
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=10 && f->use_quotes<=13)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,(f->use_quotes-9));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=5 && f->use_quotes<=9)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2packed(xtemp1,*field->data.float_value,f->length,f->use_quotes-5,(*field->data.float_value>=0.0 ? TRUE:FALSE));
								fwrite(xtemp1,f->length,1,fp);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"%*.0f",f->length,*field->data.float_value);
							} else {
								fprintf(fp,"%0*.0f",f->length,*field->data.float_value);
							}
							break;
						case DECIMALV:
						case SDECIMALV:
							if(f->use_quotes==2)
							{
							} else if(f->use_quotes==4)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,0);
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=10 && f->use_quotes<=13)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2zoned(xtemp1,*field->data.float_value,f->length,(f->use_quotes-9));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=5 && f->use_quotes<=9)
							{
								xtemp1=Rmalloc(f->length+1);
								CF_num2packed(xtemp1,*field->data.float_value,f->length,(f->use_quotes-5),TRUE);
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"%*f",f->length,*field->data.float_value);
							} else {
								fprintf(fp,"%0*f",f->length,*field->data.float_value);
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(f->use_quotes==2)
							{
							} else if(f->use_quotes==4)
							{
								xtemp1=Rmalloc(f->length+1);
								pn=*field->data.short_value;
								CF_num2zoned(xtemp1,pn,f->length,0);
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=10 && f->use_quotes<=13)
							{
								xtemp1=Rmalloc(f->length+1);
								pn=*field->data.short_value;
								CF_num2zoned(xtemp1,pn,f->length,(f->use_quotes-9));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=5 && f->use_quotes<=9)
							{
								xtemp1=Rmalloc(f->length+1);
								pn=*field->data.short_value;
								CF_num2packed(xtemp1,pn,f->length,f->use_quotes-5,(pn>=0.0 ? TRUE:FALSE));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"%*d",f->length,*field->data.short_value);
							} else {
								fprintf(fp,"%0*d",f->length,*field->data.short_value);
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(f->use_quotes==2)
							{
							} else if(f->use_quotes==4)
							{
								xtemp1=Rmalloc(f->length+1);
								pn=*field->data.integer_value;
								CF_num2zoned(xtemp1,pn,f->length,0);
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=10 && f->use_quotes<=13)
							{
								xtemp1=Rmalloc(f->length+1);
								pn=*field->data.integer_value;
								CF_num2zoned(xtemp1,pn,f->length,(f->use_quotes-9));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes>=5 && f->use_quotes<=9)
							{
								xtemp1=Rmalloc(f->length+1);
								pn=*field->data.integer_value;
								CF_num2packed(xtemp1,pn,f->length,f->use_quotes-5,(pn>=0.0 ? TRUE:FALSE));
								fwrite(xtemp1,f->length,1,fp);
								if(xtemp1!=NULL) Rfree(xtemp1);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"%*d",f->length,*field->data.integer_value);
							} else {
								fprintf(fp,"%0*d",f->length,*field->data.integer_value);
							}
							break;
						default:
							prterr("Error field type [%d] not found for field [%s] File [%s].",field->type,field->name,FILENAME(importx->fileno));
							break;
					}
				}
			}
			if(importx->carriage_return==TRUE) fprintf(fp,"\r");
			if(importx->line_feed==TRUE) fprintf(fp,"\n");
		}
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(testval ? TRUE:FALSE));
		}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		ef=NXTNRDsec(importx->fileno,1);
	}
	if(tmprsrc!=NULL)
	{
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic) killwindow(tmprsrc);
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	fclose(fp);
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
		ArchiveExport(importx);
	}
	return(0);
}
static short exportdelimitedfile(RDArsrc *mainrsrc,IEfile *importx)
{
	FILE *fp;
	char *fdel=NULL,*rdel=NULL;
	char *xtemp,*e,*temp2=NULL;
	short x,ef=FALSE,testval=FALSE;
	IEfield *f;
	NRDfield *field;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG exportdelimitedfile Exporting Database File [%s] [%s] to a delimited file [%s].",importx->module,importx->file,importx->ifile);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	memset(stemp,0,101);
	temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(importx->ifile)+2);
	sprintf(temp2,"%s/%s",CURRENTDIRECTORY,importx->ifile);
	unlink(temp2);
#ifndef WIN32
	fp=fopen(temp2,"a+b");
#else
	fp=fopen(temp2,"abc");
#endif
	if(temp2!=NULL) Rfree(temp2);
	if(fp==NULL) 
	{
		prterr("Error Attempting to open/create file [%s] to export to.",
			importx->ifile);
		return(-1);
	}
	fdel=ComputeDelimiter(importx->fdel);
	rdel=ComputeDelimiter(importx->rdel);
	e=Rmalloc(RDAstrlen(importx->file)+RDAstrlen(importx->module)+25);
	sprintf(e,"Exporting Module %s File %s",importx->module,importx->file);
	tmprsrc=diagscrn(importx->fileno,"DIAGNOSTIC SCREEN","DATABASE",e,stop_diagnostic);
	if(e!=NULL) Rfree(e);
	e=NULL;
	addDFincvir(tmprsrc,MODULENAME(importx->fileno),FILENAME(importx->fileno),NULL,importx->fileno);
#ifdef __USE_DIAGNOSTIC_SCREENS__
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(importx->module)+RDAstrlen(importx->name)+15);
			sprintf(e,"%s %s EXPORTING",importx->module,importx->name);
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
	if(importx->fields!=NULL)
	{
		for(x=0,f=importx->fields;x<importx->num;++x,++f)
		{
			field=FLDNRD(importx->fileno,f->name);
			if(field!=NULL)
			{
				if(f->use_quotes==TRUE)
				{
					fprintf(fp,"\"%s\"",(field->name!=NULL ? field->name:""));
				} else {
					fprintf(fp,"%s",(field->name!=NULL ? field->name:""));
				}
				if((x+1)<importx->num)
				{
					fprintf(fp,"%s",fdel);
				}
			}
		}
		fprintf(fp,"%s",rdel);
	}
	ZERNRD(importx->fileno);
	ef=FRSNRDsec(importx->fileno,1);
	while(!ef)
	{
		if(abort_diagnostic) break;
		if(importx->range)
		{
			testval=!testfilerangersrc(importx->fileno,importx->rs);
			if(testval)
			{
				if(mainrsrc->virflds!=NULL)
				{
					for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
					{
						if(ADVtestvirtualrangersrc(v,importx->rs,SCRNvirtualSubData,mainrsrc))
						{
							testval=FALSE;
							break;
						}
					}
				}
			}
		} else testval=TRUE;
		if(testval)
		{
			if(!isEMPTY(importx->expression))
			{
				testval=PP_EVALUATEbol(importx->expression,SCRNvirtualSubData,tmprsrc);
			} 
		}
		if(testval)
		{
			for(x=0,f=importx->fields;x<importx->num;++x,++f)
			{
				field=FLDNRD(importx->fileno,f->name);
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
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(field->data.string_value!=NULL)
							{
								xtemp=stralloc(field->data.string_value);
							} else xtemp=NULL;
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%s\"",(xtemp!=NULL ? xtemp:""));
							} else {
								fprintf(fp,"%s",(xtemp!=NULL ? xtemp:""));
							}
							if(xtemp!=NULL) Rfree(xtemp);
							break;
						case BOOLNS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%c\"",(*field->data.string_value ? 'Y':'N'));
							} else {
								fprintf(fp,"%c",(*field->data.string_value ? 'Y':'N'));
							}
							break;
						case CHARACTERS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%c\"",*field->data.string_value);
							} else {
								fprintf(fp,"%c",*field->data.string_value);
							}
							break;
						case DOUBLEV:
						case SDOUBLEV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%.0f\"",*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%.0f\"",*field->data.float_value);
							} else {
								fprintf(fp,"%.0f",*field->data.float_value);
							}
							break;
						case DOLLARS_NOCENTS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%.0f\"",*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%.0f\"",*field->data.float_value);
							} else {
								fprintf(fp,"%.0f",*field->data.float_value);
							}
							break;
						case DOLLARS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%.0f\"",*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%.02f\"",*field->data.float_value/100);
							} else {
								fprintf(fp,"%.0f",*field->data.float_value);
							}
							break;
						case DECIMALV:
						case SDECIMALV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%f\"",*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%f\"",*field->data.float_value);
							} else {
								fprintf(fp,"%f",*field->data.float_value);
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%d\"",*field->data.short_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%d\"",*field->data.short_value);
							} else {
								fprintf(fp,"%d",*field->data.short_value);
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%d\"",*field->data.integer_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%d\"",*field->data.integer_value);
							} else {
								fprintf(fp,"%d",*field->data.integer_value);
							}
							break;
						default:
							prterr("Error field type [%d] not found for field [%s] File [%s].",field->type,field->name,FILENAME(importx->fileno));
							break;
					}
					if((x+1)<importx->num)
					{
						fprintf(fp,"%s",fdel);
					}
				}
			}
			fprintf(fp,"%s",rdel);
		}
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(testval ? TRUE:FALSE));
		}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		ef=NXTNRDsec(importx->fileno,1);
	}
	if(tmprsrc!=NULL)
	{
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic) killwindow(tmprsrc);
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	fclose(fp);
	if(fdel!=NULL) Rfree(fdel);
	if(rdel!=NULL) Rfree(rdel);
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
		ArchiveExport(importx);
	}
	return(0);
}
static short exportxmlfile(RDArsrc *mainrsrc,IEfile *importx)
{
	FILE *fp;
	char *fdelbeg=NULL,*fdelend=NULL;
	char *xtemp,*e,*temp2=NULL;
	short x,ef=FALSE,testval=FALSE;
	IEfield *f;
	NRDfield *field;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG exportdelimitedfile Exporting Database File [%s] [%s] to a delimited file [%s].",importx->module,importx->file,importx->ifile);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	memset(stemp,0,101);
	temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(importx->ifile)+2);
	sprintf(temp2,"%s/%s",CURRENTDIRECTORY,importx->ifile);
#ifndef WIN32
	fp=fopen(temp2,"a+b");
#else
	fp=fopen(temp2,"abc");
#endif
	if(temp2!=NULL) Rfree(temp2);
	if(fp==NULL) 
	{
		prterr("Error Attempting to open/create file [%s] to export to.",
			importx->ifile);
		return(-1);
	}

/* HOLD THIS THOUGHT */
/*
	fdel=ComputeDelimiter(importx->fdel);
	rdel=ComputeDelimiter(importx->rdel);
*/
/* END OF HOLD THIS THOUGHT */

	e=Rmalloc(RDAstrlen(importx->file)+RDAstrlen(importx->module)+25);
	sprintf(e,"Exporting Module %s File %s",importx->module,importx->file);
	tmprsrc=diagscrn(importx->fileno,"DIAGNOSTIC SCREEN","DATABASE",e,stop_diagnostic);
	if(e!=NULL) Rfree(e);
	e=NULL;
	addDFincvir(tmprsrc,MODULENAME(importx->fileno),FILENAME(importx->fileno),NULL,importx->fileno);
#ifdef __USE_DIAGNOSTIC_SCREENS__
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(importx->module)+RDAstrlen(importx->name)+15);
			sprintf(e,"%s %s EXPORTING",importx->module,importx->name);
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
/* What?  Skipping  for now.
	if(importx->fields!=NULL)
	{
		for(x=0,f=importx->fields;x<importx->num;++x,++f)
		{
			field=FLDNRD(importx->fileno,f->name);
			if(field!=NULL)
			{
				if(f->use_quotes==TRUE)
				{
					fprintf(fp,"\"%s\"",(field->name!=NULL ? field->name:""));
				} else {
					fprintf(fp,"%s",(field->name!=NULL ? field->name:""));
				}
				if((x+1)<importx->num)
				{
					fprintf(fp,"%s",fdel);
				}
			}
		}
*/
/* Temporarily skipping record delimiter 
		fprintf(fp,"%s",rdel);
	}
*/
	ZERNRD(importx->fileno);
	ef=FRSNRDsec(importx->fileno,1);
	while(!ef)
	{
		if(abort_diagnostic) break;
		if(importx->range)
		{
			testval=!testfilerangersrc(importx->fileno,importx->rs);
			if(testval)
			{
				if(mainrsrc->virflds!=NULL)
				{
					for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
					{
						if(ADVtestvirtualrangersrc(v,importx->rs,SCRNvirtualSubData,mainrsrc))
						{
							testval=FALSE;
							break;
						}
					}
				}
			}
		} else testval=TRUE;
		if(testval)
		{
		if(!isEMPTY(importx->expression))
		{
			testval=PP_EVALUATEbol(importx->expression,SCRNvirtualSubData,tmprsrc);
		} 
		} 
		if(testval)
		{
			for(x=0,f=importx->fields;x<importx->num;++x,++f)
			{
				field=FLDNRD(importx->fileno,f->name);
				if(field!=NULL)
				{
					fdelbeg=Rmalloc(5+RDAstrlen(field->name));
					sprintf(fdelbeg,"\t<%s>",field->name);
					fdelend=Rmalloc(8+RDAstrlen(field->name));
					sprintf(fdelend,"</%s>\r\n",field->name);
					fprintf(fp,"%s",(fdelbeg!=NULL ? fdelbeg:""));
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
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(field->data.string_value!=NULL)
							{
								xtemp=stralloc(field->data.string_value);
							} else xtemp=NULL;
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%s\"",(xtemp!=NULL ? xtemp:""));
							} else {
								fprintf(fp,"%s",(xtemp!=NULL ? xtemp:""));
							}
							if(xtemp!=NULL) Rfree(xtemp);
							break;
						case BOOLNS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%c\"",(*field->data.string_value ? 'Y':'N'));
							} else {
								fprintf(fp,"%c",(*field->data.string_value ? 'Y':'N'));
							}
							break;
						case CHARACTERS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%c\"",*field->data.string_value);
							} else {
								fprintf(fp,"%c",*field->data.string_value);
							}
							break;
						case DOLLARS_NOCENTS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%0*.0f\"",f->length,*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%*.0f\"",f->length,*field->data.float_value);
							} else {
								fprintf(fp,"%*.0f",f->length,*field->data.float_value);
							}
							break;
						case DOLLARS:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%0*.0f\"",f->length,*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%*.02f\"",f->length,*field->data.float_value/100);
							} else {
								fprintf(fp,"%0*.0f",f->length,*field->data.float_value);
							}
							break;
						case DOUBLEV:
						case SDOUBLEV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%0*.0f\"",f->length,*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%*.0f\"",f->length,*field->data.float_value);
							} else {
								fprintf(fp,"%0*.0f",f->length,*field->data.float_value);
							}
							break;
						case DECIMALV:
						case SDECIMALV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%0*f\"",f->length,*field->data.float_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%*f\"",f->length,*field->data.float_value);
							} else {
								fprintf(fp,"%0*f",f->length,*field->data.float_value);
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%0*d\"",f->length,*field->data.short_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%*d\"",f->length,*field->data.short_value);
							} else {
								fprintf(fp,"%0*d",f->length,*field->data.short_value);
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(f->use_quotes==TRUE)
							{
								fprintf(fp,"\"%0*d\"",f->length,*field->data.integer_value);
							} else if(f->use_quotes==14)
							{
								fprintf(fp,"\"%*d\"",f->length,*field->data.integer_value);
							} else {
								fprintf(fp,"%0*d",f->length,*field->data.integer_value);
							}
							break;
						default:
							prterr("Error field type [%d] not found for field [%s] File [%s].",field->type,field->name,FILENAME(importx->fileno));
							break;
					}
					fprintf(fp,"%s",fdelend);
					if(fdelbeg!=NULL) Rfree(fdelbeg);
					if(fdelend!=NULL) Rfree(fdelend);
				}
			}
			/*
			fprintf(fp,"%s",rdel);
			*/
		}
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(testval ? TRUE:FALSE));
		}
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		ef=NXTNRDsec(importx->fileno,1);
	}
	if(tmprsrc!=NULL)
	{
#ifdef __USE_DIAGNOSTIC_SCREENS__
		if(!abort_diagnostic) killwindow(tmprsrc);
#endif /* __USE_DIAGNOSTIC_SCREENS__ *//
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	fclose(fp);
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
		ArchiveExport(importx);
	}
	return(0);
}
short exportdata(RDArsrc *mainrsrc,IEfile *importx)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		prterr("DIAG exportdata Exporting Database File [%s] [%s] to filename [%s].",importx->module,importx->file,importx->ifile);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(importx->type==1)
	{
		return(exportdelimitedfile(mainrsrc,importx));
	} else if(importx->type==2) {
		return(exportxmlfile(mainrsrc,importx));
	} else {
		return(exportflatfile(mainrsrc,importx));
	}
}
void QUITimport(RDArsrc *mainrsrc,IEfile *importx)
{
	if(importx!=NULL)
	{
		CLSNRD(importx->fileno);
		free_import(importx);
	}
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	ShutdownSubsystems();
}
void SELECTimport(RDArsrc *mainrsrc,IEfile *importx)
{
	char *errordesc=NULL;

	if(importx->range)
	{
		readallwidgets(mainrsrc);
		importx->rs=RangeScreenNew(mainrsrc);
		ReadRangeScreen(importx->fileno,importx->rs);
		FINDRSCGETSTRING(mainrsrc,"FILENAME",&importx->ifile);
	}
	if(importx->which) 
	{
		if(exportdata(mainrsrc,importx))
		{
			errordesc=Rmalloc(96+RDAstrlen(importx->module)+RDAstrlen(importx->name));
			sprintf(errordesc,"The export function has failed attempting to execute import/export definition from [%s] named [%s].",importx->module,importx->name);
			ERRORDIALOG("Export Data File Failed",errordesc,NULL,TRUE);
			prterr("Error Export Datafile [%s] [%s] to file [%s] failed.",importx->module,importx->file,importx->ifile);
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=NULL;
			if(!importx->MainLoop)
			{
				importx->MainLoop=TRUE;
				return;
			}
		}
	} else {
		if(importdata(mainrsrc,importx))
		{
			errordesc=Rmalloc(96+RDAstrlen(importx->module)+RDAstrlen(importx->name));
			sprintf(errordesc,"The import function has failed attempting to execute import/export definition from [%s] named [%s].",importx->module,importx->name);
			ERRORDIALOG("Import Data File Failed",errordesc,NULL,TRUE);
			prterr("Error Import Datafile [%s] [%s] from file [%s] failed.",importx->module,importx->file,importx->ifile);
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=NULL;
			if(!importx->MainLoop)
			{
				importx->MainLoop=TRUE;
				return;
			}
		}
		if(removewc==TRUE)
		{
			unlink(importx->ifile);
		}
	}
	QUITimport(mainrsrc,importx);
}
void xEXECUTEimport(char *module,char *name,int line,char *file)
{
	IEfile *importx=NULL;
	char *temp=NULL,*screen=NULL;
	RDArsrc *MainRsrc=NULL;
	int x;
	IEfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport || diagimport_field)
	{
		prterr("DIAG EXECUTEimporrt Executing import/export definition for [%s] [%s] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	importx=IMPORTstpNEW(name);
	if(getIMPORTbin(module,importx))
	{
		ERRORDIALOG("Cannot Read Import/Export","Cannot read the import/export definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		free_import(importx);
		return;
	}
	if(importx!=NULL)
	{
		if(importx->expression!=NULL)
		{
			temp=PP_translate(importx->expression);
			Rfree(importx->expression);
			if(temp!=NULL) 
			{
				importx->expression=stralloc(temp);
				Rfree(temp);
			}
		}
		if(importx->fields!=NULL)
		{
			for(x=0,field=importx->fields;x<importx->num;++x,++field)
			{
				if(field->expression!=NULL)
				{
					temp=PP_translate(field->expression);
					Rfree(field->expression);
					if(temp!=NULL)
					{
						field->expression=stralloc(temp);
						Rfree(temp);
					}
				}
				if(field->value_expression!=NULL)
				{
					temp=PP_translate(field->value_expression);
					Rfree(field->value_expression);
					if(temp!=NULL)
					{
						field->value_expression=stralloc(temp);
						Rfree(temp);
					}
				}
			}
		}	
		if((importx->fileno=APPOPNNRD(importx->module,importx->file,TRUE,(!importx->which ? TRUE:FALSE)))==(-1)) return;
		if(importx->range)
		{
			screen=Rmalloc(RDAstrlen(name)+46);
			sprintf(screen,"%s IMPORT/EXPORT RANGE SCREEN",name);
			MainRsrc=RDArsrcNEW(module,screen);
			addDFincvir(MainRsrc,importx->module,importx->file,NULL,importx->fileno);
			GET_SCREEN_VIRTUALS(MainRsrc,2);
			file2rangersrc(importx->fileno,MainRsrc);
			SCRNvirtual2rangersrc(MainRsrc);
			addstdrsrc(MainRsrc,"FILENAME",VARIABLETEXT,0,importx->ifile,TRUE);
			if(importx->which==0) 
			{
				addstdrsrc(MainRsrc,"REMOVE WHEN COMPLETE",BOOLNS,1,&removewc,TRUE);
			}
			addbtnrsrc(MainRsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
			addrfexrsrc(MainRsrc,"SELECT",TRUE,SELECTimport,importx);
			addrfexrsrc(MainRsrc,"QUIT",TRUE,QUITimport,importx);
			addbtnrsrc(MainRsrc,"HELP",TRUE,screenhelp,NULL);
			addbtnrsrc(MainRsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
			addbtnrsrc(MainRsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
			DefaultScreens(MainRsrc);
			SetRangersrcsensitive(MainRsrc);
			APPmakescrn(MainRsrc,TRUE,QUITimport,importx,TRUE);
			if(screen!=NULL) Rfree(screen);
			importx->MainLoop=TRUE;
			return;
		} else {
			SELECTimport(NULL,importx);
		}
	} else {
		prterr("Error Import/Export Definition [%s] [%s] not found at line [%d] program [%s].",module,name,line,file);
	}
}
char *xreadDelimitedline(FILE *fd,char *rdel,long *total,int line,char *file)
{
	int x,r;
	long end_record=0;
	char *ImportLine=NULL;
	char *temp=NULL,*offset=NULL,*inbuf=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagimport)
	{
		prterr("DIAG readDelimitedline at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	inbuf=(char *)Rmalloc(sizeof(char)*MAXLINEBUFF+1);
	while(TRUE)
	{
		memset(inbuf,0,MAXLINEBUFF);
		x=fread(inbuf,sizeof(char),MAXLINEBUFF,fd);
		if(x<0) x=0;
		inbuf[x]=0;
		if(x<1) return(NULL);
		temp=stralloc(ImportLine);
		ImportLine=Rmalloc(RDAstrlen(temp)+RDAstrlen(inbuf)+1);
		sprintf(ImportLine,"%s%s",(temp!=NULL?temp:""),(inbuf!=NULL?inbuf:""));
		if(temp!=NULL) Rfree(temp);
		offset=RDAstrstr(ImportLine,rdel);
		if(offset!=NULL)
		{
			end_record=(int)(offset-ImportLine);
			ImportLine[end_record]=0;
			*total=*total+end_record+RDAstrlen(rdel);
			r=fseek(fd,(long)(*total),SEEK_SET);
			if(r)
			{
				prterr("Error fseek failed with [%d].",r);
			}
			if(inbuf!=NULL) Rfree(inbuf);
#ifdef USE_RDA_DIAGNOSTICS
			if(diagimport || diagimport_field)
			{
				prterr("DIAG IMPORT: readDelimitedline(): returning import line of [%s].",ImportLine);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			break;
		}
	}
	if(inbuf!=NULL) Rfree(inbuf);
	return(ImportLine);
}
static void ArchiveExport(IEfile *importx)
{
	char *aname=NULL,*desc=NULL,*fname=NULL;
	char *temp1=NULL,*c=NULL,*t=NULL,*wgdir=NULL;
	RDATData *prev=NULL;
	int aclass=1;
	char *subject=NULL;
	short s=4,a_fileno=(-1),archwork=(-1);
	APPlib *args=NULL;
	int ret_int=0;
	
	a_fileno=APPOPNNRD("ARCHIVE","ARCHIVES",TRUE,TRUE);
	archwork=APPOPNNRD("ARCHIVE","ARCHWORK",TRUE,FALSE);
	if((a_fileno==(-1)) || (archwork==(-1)))
	{
		prterr("Error:  Unable to Archive Database files.");
		return;
	}
	aname=MakeArchiveName(importx->module,importx->name);
	temp1=adddashes(aname);
	ZERNRD(archwork);
	FINDFLDSETSTRING(archwork,"WORK GROUP",importx->module);
	if(EQLNRD(archwork,1)) KEYNRD(archwork,1);
	FINDFLDGETSTRING(archwork,"DIRECTORY",&wgdir);
	stoupper(temp1);
	fname=Rmalloc(RDAstrlen(temp1)+5);
	switch(importx->type)
	{
		case 0:
			sprintf(fname,"%s.txt",temp1);	
			break;
		case 1:
#ifdef WIN32
			if(!RDAstrcmp(importx->fdel,"\t")) 
			{
				sprintf(fname,"%s.tsv",temp1);
			} else sprintf(fname,"%s.csv",temp1);	
#else
			sprintf(fname,"%s.csv",temp1);
#endif
			break;
	}
	if(temp1!=NULL) Rfree(temp1);

#ifndef WIN32
	sprintf(stemp,"ARCHIVE/.%s",(importx->module!=NULL ? importx->module:""));
#endif
#ifdef WIN32
	sprintf(stemp,"ARCHIVE\\.%s",(importx->module!=NULL ? importx->module:""));
#endif
	RDAmkdir(stemp,0660,&ret_int);

	args=APPlibNEW();
	addAPPlib(args,importx->ifile);
	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"ARCHIVE/.%s/%s",importx->module,fname);
#endif
#ifdef WIN32
	sprintf(stemp,"ARCHIVE\\.%s\\%s",importx->module,fname);
#endif
	addAPPlib(args,stemp);
	Execute2Program("COPYFILE",args);
	if(args!=NULL) freeapplib(args);
	
	ZERNRD(a_fileno);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",importx->module);
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	if(!EQLNRD(a_fileno,1))
	{
		prev=RDATDataNEW(a_fileno);
	} else KEYNRD(a_fileno,1);
	temp1=Users_Department();
	if(!RDAstrcmp(importx->module,"FINMGT") || !RDAstrcmp(importx->module,"BNKREC")) subject=stralloc("FINANCE");
	else if(!RDAstrcmp(importx->module,"VENPMT")) subject=stralloc("A/P");
	else if(!RDAstrcmp(importx->module,"PURORD") || !RDAstrcmp(importx->module,"FIXASS") || 
		!RDAstrcmp(importx->module,"CATALOGUE") ||
		!RDAstrcmp(importx->module,"REQMGT")) subject=stralloc("PURCHASING");
	else if(!RDAstrcmp(importx->module,"PAYROLL") || 
		!RDAstrcmp(importx->module,"LVEMGT") || !RDAstrcmp(importx->module,"EMPABS") || !RDAstrcmp(importx->module,"SUBMGT") || !RDAstrcmp(importx->module,"POSTRK") || 
		!RDAstrcmp(importx->module,"LVSIMP")) subject=stralloc("PAYROLL");
	else if(!RDAstrcmp(importx->module,"PRSNNL") || !RDAstrcmp(importx->module,"IIDMGT") || !RDAstrcmp(importx->module,"JOBPERF") || !RDAstrcmp(importx->module,"APPMGT")) subject=stralloc("PERSONNEL");
	else if(!RDAstrcmp(importx->module,"BUDPREP")) subject=stralloc("BUDGET");
	else if(RDAstrstr(temp1,"TREASURE")) subject=stralloc("TREASURER");
	else if(RDAstrstr(temp1,"COMM")) subject=stralloc("COMMISSIONER");
	else subject=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	FINDFLDSETCHAR(a_fileno,"DELETEFLAG",FALSE);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",importx->module);
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	FINDFLDSETSTRING(a_fileno,"DESCRIPTION",importx->desc);
	FINDFLDSETSTRING(a_fileno,"SUBJECT",subject);
	FINDFLDSETSHORT(a_fileno,"SOURCE",s);
	FINDFLDSETSTRING(a_fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETSTRING(a_fileno,"FILENAME",fname);
	aclass=8;
	if(importx->type==1) 
	{
#ifdef WIN32
		if(!RDAstrcmp(importx->fdel,"\t")) 
		{
			FINDFLDSETSTRING(a_fileno,"TYPE","tsv");
		} else FINDFLDSETSTRING(a_fileno,"TYPE","csv");
#else
		FINDFLDSETSTRING(a_fileno,"TYPE","csv");
#endif
	} else {
		FINDFLDSETSTRING(a_fileno,"TYPE","txt");
	}
	FINDFLDSETINT(a_fileno,"CLASS",aclass);
	c=GETCURRENTDATE10();
	FINDFLDSETSTRING(a_fileno,"DATE",c);
	if(c!=NULL) Rfree(c);
	t=GETCURRENTTIME();
	FINDFLDSETSTRING(a_fileno,"TIME",t);
	if(t!=NULL) Rfree(t);
	WRTTRANS(a_fileno,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);	
	if(fname!=NULL) Rfree(fname);
	if(desc!=NULL) Rfree(desc);
	if(aname!=NULL) Rfree(aname);
	if(subject!=NULL) Rfree(subject);
	if(a_fileno!=(-1)) CLSNRD(a_fileno);
	if(archwork!=(-1)) CLSNRD(archwork);
	if(temp1!=NULL) Rfree(temp1);
}
