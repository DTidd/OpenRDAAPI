/* rptez.c - EZ-RPT Functions */
#include<rptgen.hpp>

#ifdef WIN32
#include <io.h>
#endif


RDAezrpt *RDAezrptNEW(char *name)
{
	RDAezrpt *ez=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG RDAezrptNEW Creating/Allocating an EZ-Report Structure for Report [%s].",name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ez=Rmalloc(sizeof(RDAezrpt));
	if(name!=NULL)
	{
		ez->name=stralloc(name);
	} else ez->name=NULL;
	ez->title=NULL;
	ez->desc=NULL;
	ez->majmod=NULL;
	ez->majfil=NULL;
	ez->print=0;
	ez->devicename=NULL;
	ez->printsize=NULL;
	ez->account_report=FALSE;
	ez->skip_finmgt=FALSE;
	ez->numsrt=0;
	ez->sort=NULL;
	ez->alignment_test=FALSE;
	ez->page_length=0.0;
	ez->numdtl=0;
	ez->detail=NULL;
	ez->num=0;
	ez->incvir=NULL;
	ez->range_screen=TRUE;
	return(ez);
}
void xFreeRDAezrpt(RDAezrpt *ez,int line,char *file)
{
	RDAezsrt *sort;
	RDAezdtl *detail;
	int x;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG FreeRDAezrpt Freeing EZ-Report Structure for [%s] at line [%d] program [%s].",ez->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(ez!=NULL)
	{
		if(ez->name!=NULL) Rfree(ez->name);
		if(ez->title!=NULL) Rfree(ez->title);
		if(ez->desc!=NULL) Rfree(ez->desc);
		if(ez->majmod!=NULL) Rfree(ez->majmod);
		if(ez->majfil!=NULL) Rfree(ez->majfil);
		if(ez->devicename!=NULL) Rfree(ez->devicename);
		if(ez->printsize!=NULL) Rfree(ez->printsize);
		if(ez->incvir!=NULL)
		{
			for(x=0,i=ez->incvir;x<ez->num;++x,++i)
			{
				if(i->module!=NULL) Rfree(i->module);
				if(i->file!=NULL) Rfree(i->file);
				if(i->keyname!=NULL) Rfree(i->keyname);
			}
			Rfree(ez->incvir);
			ez->num=0;
		} 
		if(ez->sort!=NULL)
		{
			for(x=0,sort=ez->sort;x<ez->numsrt;++x,++sort)
			{
				if(sort->name!=NULL) Rfree(sort->name);
				switch(sort->type)
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
					case BOOLNS:
					case CHARACTERS:
						if(sort->from.string_value!=NULL)
							Rfree(sort->from.string_value);
						if(sort->to.string_value!=NULL)
							Rfree(sort->to.string_value);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						if(sort->from.float_value!=NULL)
							Rfree(sort->from.float_value);
						if(sort->to.float_value!=NULL)
							Rfree(sort->to.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(sort->from.short_value!=NULL)
							Rfree(sort->from.short_value);
						if(sort->to.short_value!=NULL)
							Rfree(sort->to.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(sort->from.integer_value!=NULL)
							Rfree(sort->from.integer_value);
						if(sort->to.integer_value!=NULL)
							Rfree(sort->to.integer_value);
						break;
					default:
						prterr("Error field type [%d] not found for field [%s].",sort->type,sort->name);
						break;
				}
			}
			Rfree(ez->sort);
			ez->numsrt=0;
		}
		if(ez->detail!=NULL)
		{
			for(x=0,detail=ez->detail;x<ez->numdtl;++x,++detail)
			{
				if(detail->title1!=NULL) Rfree(detail->title1);
				if(detail->title2!=NULL) Rfree(detail->title2);
				if(detail->fieldname!=NULL) Rfree(detail->fieldname);
				if(detail->expression!=NULL) Rfree(detail->expression);
			}
			Rfree(ez->detail);
			ez->numdtl=0;
		}
		Rfree(ez);
	}
}
RDAezrpt *xGETRDAezrpt(char *module,char *name,int line,char *file)
{
	int fd;
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL,*temp=NULL;
	short x,test=0,version=FALSE;
	int y;
	RDAezsrt *sort;
	RDAezdtl *detail;
	RDAezrpt *ez;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG Getting EZ-Report Structure for [%s] [%s] from Binary [%s.EZRPT] at line [%d] file [%s].",module,name,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(name)+9+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		prterr("Error in permissions, user can't read report file [%s] at line [%d] program [%s].",fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd==(-1))
	{
		prterr("Error [%s] can't open file [%s] at line [%d] program [%s].",errname(),fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=BINread(fd);
	if(mainbin==NULL)
	{
		prterr("Error can't read file [%s] at line [%d] program [%s].",fname,line,file);
		if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1694) && x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1702) && x!=(-1703) 
		&& x!=(-1700))
	{
		prterr("Error invalid version or bad binary for Module [%s] EZ-Report [%s] at line [%d] program [%s].  EZ-Report may be corrupted.",module,name,line,file);
		BINfree(mainbin);
		return(NULL);
	}
	if(x==(-1694)) version=9;
	else if(x==(-1695)) version=8;
	else if(x==(-1696)) version=7;
	else if(x==(-1697)) version=6;
	else if(x==(-1698)) version=5;
	else if(x==(-1699)) version=4;
	else if(x==(-1700)) version=3;
	else if(x==(-1703)) version=2;
	else if(x==(-1702)) version=TRUE;
	else version=FALSE;
	ez=RDAezrptNEW(name);
	test=BINgetshort(mainbin);
	if(test)
	{
		temp=BINgetstring(mainbin);
		if(temp!=NULL) Rfree(temp);
	} 
	test=BINgetshort(mainbin);
	if(test) ez->title=BINgetstring(mainbin);
		else ez->title=NULL;
	test=BINgetshort(mainbin);
	if(test) ez->desc=BINgetstring(mainbin);
		else ez->desc=NULL;
	test=BINgetshort(mainbin);
	if(test) ez->majmod=BINgetstring(mainbin);
		else ez->majmod=NULL;
	test=BINgetshort(mainbin);
	if(test) ez->majfil=BINgetstring(mainbin);
		else ez->majfil=NULL;
	ez->print=BINgetshort(mainbin);
	if(version<3 && !ez->print) ez->print=TRUE;
	if(version<7)
	{
	if(version>2)
	{
		ez->page_length=BINgetshort(mainbin);
	} else ez->page_length=10.0;
	} else {
		ez->page_length=BINgetdouble(mainbin);
	}
	if(version>4)
	{
		ez->account_report=BINgetshort(mainbin);
	} else ez->account_report=FALSE;
	if(version>8)
	{
		test=BINgetshort(mainbin);
		if(test) ez->skip_finmgt=TRUE;
		else ez->skip_finmgt=FALSE;
	} else {
		ez->skip_finmgt=FALSE;
	}
	if(version>5)
	{
		test=BINgetshort(mainbin);
		if(test) ez->range_screen=TRUE;
			else ez->range_screen=FALSE;
	} else ez->range_screen=TRUE;
	if(version>3)
	{
		ez->alignment_test=BINgetshort(mainbin);
	} else ez->alignment_test=FALSE;
	if(version>1)
	{
		test=BINgetshort(mainbin);
		if(test) ez->devicename=BINgetstring(mainbin);
			else ez->devicename=NULL;
		test=BINgetshort(mainbin);
		if(test) ez->printsize=BINgetstring(mainbin);
			else ez->printsize=NULL;
	}
	if(version>0)
	{
		ez->num=BINgetshort(mainbin);
		if(ez->num>0)
		{
			ez->incvir=Rmalloc(ez->num*sizeof(DFincvir));
			for(y=0,i=ez->incvir;y<ez->num;++y,++i)
			{
				test=BINgetshort(mainbin);
				if(test) i->module=BINgetstring(mainbin);
					else i->module=NULL;
				test=BINgetshort(mainbin);
				if(test) i->file=BINgetstring(mainbin);
					else i->file=NULL;
				test=BINgetshort(mainbin);
				if(test) i->keyname=BINgetstring(mainbin);
					else i->keyname=NULL;
			}
		} else {
			ez->incvir=NULL;
			ez->num=0;
		}
	}
	ez->numsrt=BINgetint(mainbin);
	if(ez->numsrt>0)
	{
		ez->sort=Rmalloc(ez->numsrt*(sizeof(RDAezsrt)));
		for(y=0,sort=ez->sort;y<ez->numsrt;++y,++sort)
		{
			test=BINgetshort(mainbin);
			if(test) sort->name=BINgetstring(mainbin);
				else sort->name=NULL;
			sort->total=BINgetshort(mainbin);
			sort->start=BINgetshort(mainbin);
			sort->length=BINgetshort(mainbin);
			sort->type=BINgetshort(mainbin);
			test=BINgetshort(mainbin);
				if(test) sort->range=TRUE;
					else sort->range=FALSE;
			switch(sort->type)
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
					test=BINgetshort(mainbin);
					if(test) sort->from.string_value=BINgetstring(mainbin);
						else sort->from.string_value=NULL;
					test=BINgetshort(mainbin);
					if(test) sort->to.string_value=BINgetstring(mainbin);
						else sort->to.string_value=NULL;
					break;
				case BOOLNS:
				case CHARACTERS:
					sort->from.string_value=Rmalloc(1);
					test=BINgetshort(mainbin);
					*sort->from.string_value=test;
					sort->to.string_value=Rmalloc(1);
					test=BINgetshort(mainbin);
					*sort->to.string_value=test;		
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					sort->from.float_value=Rmalloc(sizeof(double));
					*sort->from.float_value=BINgetdouble(mainbin);
					sort->to.float_value=Rmalloc(sizeof(double));
					*sort->to.float_value=BINgetdouble(mainbin);
					break;
				case SHORTV:
				case SSHORTV:
					sort->from.short_value=Rmalloc(sizeof(short));
					*sort->from.short_value=BINgetshort(mainbin);
					sort->to.short_value=Rmalloc(sizeof(short));
					*sort->to.short_value=BINgetshort(mainbin);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					sort->from.integer_value=Rmalloc(sizeof(int));
					*sort->from.integer_value=BINgetint(mainbin);
					sort->to.integer_value=Rmalloc(sizeof(int));
					*sort->to.integer_value=BINgetint(mainbin);
					break;
				default:
					prterr("Error field type [%d] not found for field [%s].",sort->type,sort->name);
					break;
			}
		}
	} else {
		ez->sort=NULL;
		ez->numsrt=0;
	}
	ez->numdtl=BINgetint(mainbin);
	if(ez->numdtl>0)
	{
		ez->detail=Rmalloc(ez->numdtl*sizeof(RDAezdtl));
		for(y=0,detail=ez->detail;y<ez->numdtl;++y,++detail)
		{
			test=BINgetshort(mainbin);
			if(test) detail->title1=BINgetstring(mainbin);
				else detail->title1=NULL;
			test=BINgetshort(mainbin);
			if(test) detail->title2=BINgetstring(mainbin);
				else detail->title2=NULL;
			detail->start=BINgetshort(mainbin);	
			detail->length=BINgetshort(mainbin);
			detail->type=BINgetshort(mainbin);
			if(version>8)
			{
				test=BINgetshort(mainbin);
				if(test) detail->range=TRUE;
				else detail->range=FALSE;
			} else {
				detail->range=FALSE;
			}
			if(version>6)
			{
				detail->fieldtype=BINgetshort(mainbin);
			} else {
				detail->fieldtype=0;
			}
			test=BINgetshort(mainbin);
			if(test) detail->fieldname=BINgetstring(mainbin);
				else detail->fieldname=NULL;
			test=BINgetshort(mainbin);
			if(test) detail->expression=BINgetstring(mainbin);
				else detail->expression=NULL;
			if(version)
			{
				detail->fieldtype=BINgetshort(mainbin);
			}
		}
	} else {
		ez->detail=NULL;
		ez->numdtl=0;
	}
	BINfree(mainbin);
	if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
	if(fname!=NULL) Rfree(fname);
	return(ez);
}
short xWRTRDAezrpt(char *module,RDAezrpt *ez,int line,char *file)
{
	int fd,y;
	short x;
	BIN *bin;
	char *fname=NULL,*fname1=NULL;
	RDAezsrt *sort;
	RDAezdtl *detail;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG WRTRDAezrpt Writing EZ-Report Defintion [%s] [%s] to Binary [%s.RPT] at line [%d] file [%s].",(module!=NULL ? module:""),(ez->name!=NULL ? ez->name:""),(ez->name!=NULL ? ez->name:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(ez->name)+9+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(ez->name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd!=(-1))
	{
		if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
		if(access(fname,02))
		{
			prterr("Error in permissions, user can't write report file [%s] at line [%d] program [%s].",fname,line,file);
			if(fname!=NULL) Rfree(fname);
			return(-1);
		}
		unlink(fname);
	}
	bin=BINnew();
	BINaddshort(bin,-1694);	/*type identifier*/
	if(!isEMPTY(ez->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,ez->name);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(ez->title))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,ez->title);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(ez->desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,ez->desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(ez->majmod))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,ez->majmod);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(ez->majfil))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,ez->majfil);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,ez->print);
	BINadddouble(bin,ez->page_length);
	BINaddshort(bin,(ez->account_report==TRUE ? TRUE:FALSE));
	BINaddshort(bin,(ez->skip_finmgt==TRUE ? TRUE:FALSE));
	BINaddshort(bin,(ez->range_screen==TRUE ? TRUE:FALSE));
	BINaddshort(bin,ez->alignment_test);
	if(!isEMPTY(ez->devicename))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,ez->devicename);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(ez->printsize))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,ez->printsize);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,ez->num);
	if(ez->num>0 && ez->incvir!=NULL)
	{
		for(y=0,i=ez->incvir;y<ez->num;++y,++i)
		{
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
			if(!isEMPTY(i->keyname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->keyname);
			} else BINaddshort(bin,FALSE);
		}
	}
	if(ez->numsrt>0 && ez->sort!=NULL)
	{
		BINaddint(bin,ez->numsrt);
		for(y=0,sort=ez->sort;y<ez->numsrt;++y,++sort)
		{
			if(!isEMPTY(sort->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sort->name);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,sort->total);
			BINaddshort(bin,sort->start);
			BINaddshort(bin,sort->length);
			BINaddshort(bin,sort->type);
			x=sort->range;
			BINaddshort(bin,x);	
			switch(sort->type)
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
					if(!isEMPTY(sort->from.string_value))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,sort->from.string_value);
					} else BINaddshort(bin,FALSE);
					if(!isEMPTY(sort->to.string_value))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,sort->to.string_value);
					} else BINaddshort(bin,FALSE);
					break;
				case BOOLNS:
				case CHARACTERS:
					x=*sort->from.string_value;
					BINaddshort(bin,x);
					x=*sort->to.string_value;
					BINaddshort(bin,x);
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					BINadddouble(bin,*sort->from.float_value);
					BINadddouble(bin,*sort->to.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					BINaddshort(bin,*sort->from.short_value);
					BINaddshort(bin,*sort->to.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					BINaddint(bin,*sort->from.integer_value);
					BINaddint(bin,*sort->to.integer_value);
					break;
				default:
					prterr("Error field type [%d] not found for field [%s].",sort->type,sort->name);
					break;
			}
		}
	} else {
		BINaddint(bin,0);
	}
	if(ez->numdtl>0 && ez->detail!=NULL)
	{
		BINaddint(bin,ez->numdtl);
		for(y=0,detail=ez->detail;y<ez->numdtl;++y,++detail)
		{
			if(!isEMPTY(detail->title1))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,detail->title1);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(detail->title2))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,detail->title2);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,detail->start);
			BINaddshort(bin,detail->length);
			BINaddshort(bin,detail->type);
			BINaddshort(bin,(detail->range==TRUE ? TRUE:FALSE));
			BINaddshort(bin,detail->fieldtype);
			if(!isEMPTY(detail->fieldname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,detail->fieldname);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(detail->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,detail->expression);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,detail->fieldtype);
		}
	} else BINaddint(bin,0);
#ifndef WIN32
	fd=open(fname,O_WRONLY | O_CREAT | O_EXCL,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_WRONLY | O_CREAT | O_BINARY | O_EXCL,0666);
#endif
	if(fd==(-1))
	{
		prterr("Error [%s] can't open file [%s] at line [%d] program [%s].",errname(),fname,line,file);
		BINfree(bin);
		return(-1);
	}
	BINwrite(fd,bin);
	BINfree(bin);
	if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
	if(fname!=NULL) Rfree(fname);
	return(0);
}
char *xgetezrpttitle(char *module,char *name,int line,char *file)
{
	int fd;
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL,*temp=NULL,*hold=NULL;
	short x,test=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG Getting EZ-Report Title [%s] [%s] from binary at line [%d] file [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(name)+9+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		prterr("Error in permissions, user can't read report file [%s] at line [%d] program [%s].",fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd==(-1))
	{
		prterr("Error [%s] can't open file [%s] at line [%d] program [%s].",errname(),fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=BINread(fd);
	if(mainbin==NULL)
	{
		prterr("Error can't read file [%s] at line [%d[ program [%s].",fname,line,file);
		if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1694) && x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1702) && x!=(-1703) 
		&& x!=(-1700))
	{
		prterr("Error invalid version or bad binary in Module [%s] EZ-Report [%s] at line [%d] program [%s].",module,name,line,file);
		BINfree(mainbin);
		return(NULL);
	}
	test=BINgetshort(mainbin);
	if(test)
	{
		temp=BINgetstring(mainbin);
		if(temp!=NULL) Rfree(temp);
	} 
	test=BINgetshort(mainbin);
	if(test) hold=BINgetstring(mainbin);
		else hold=NULL;
	BINfree(mainbin);
	if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
	if(fname!=NULL) Rfree(fname);
	return(hold);
}
char *xgetezrptdesc(char *module,char *name,int line,char *file)
{
	int fd;
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL,*temp=NULL,*hold=NULL;
	short x,test=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG Getting EZ-Report Description [%s] [%s] from binary at line [%d] file [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(name)+9+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.EZRPT",CURRENTDIRECTORY,module,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		prterr("Error in permissions, user can't read report file [%s] at line [%d] program [%s].",fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd==(-1))
	{
		prterr("Error [%s] can't open file [%s] at line [%d] program [%s].",errname(),fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=BINread(fd);
	if(mainbin==NULL)
	{
		prterr("Error can't read file [%s] at line [%d] program [%s].",fname,line,file);
		if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1694) && x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1702) && x!=(-1703) 
		&& x!=(-1700))
	{
		prterr("Error invalid version or bad binary in Module [%s] EZ-Report [%s] at line [%d] program [%s].",module,name,line,file);
		BINfree(mainbin);
		return(NULL);
	}
	test=BINgetshort(mainbin);
	if(test)
	{
		temp=BINgetstring(mainbin);
		if(temp!=NULL) Rfree(temp);
	} 
	test=BINgetshort(mainbin);
	if(test) temp=BINgetstring(mainbin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(mainbin);
	if(test) hold=BINgetstring(mainbin);
		else hold=NULL;
	BINfree(mainbin);
	if(close(fd)==(-1)) {prterr("Error can't close file [%s] at line [%d] program [%s].",fname,line,file);}
	if(fname!=NULL) Rfree(fname);
	return(hold);
}
#ifdef SAVE_SWITCH_CASE

			switch(sort->type)
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
				case BOOLNS:
				case CHARACTERS:
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					break;
				case SHORTV:
				case SSHORTV:
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					break;
				default:
					prterr("field type [%d] not found for field [%s].",field->type,field->name);
					break;
			}
#endif
