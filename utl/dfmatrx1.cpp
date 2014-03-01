#include<mix.hpp>
extern RDAMATRIX *DFV;

short xCONVERTFIELDTYPE(short *type,union rda_field_types *data,unsigned *len,
	unsigned *dlen,short newtype,union rda_field_types *from,short newlen,
	int line,char *file)
{
	char cvalue=FALSE,*string=NULL,freeit=FALSE;
	int ivalue=0;
	short svalue=0;
	double fvalue=0.0;

	if(data==from) freeit=TRUE;
		else freeit=FALSE;
	switch(*type)
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
			if(from->string_value!=NULL) string=stralloc(from->string_value);
				else string=NULL;
			if(freeit)
			{
				if(data->string_value!=NULL)
					Rfree(data->string_value);
			}
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			if(from->float_value!=NULL)  fvalue=*from->float_value;
				else fvalue=0;
			if(freeit)
			{
				if(data->float_value!=NULL)
					Rfree(data->float_value);
			}
			break;
		case SHORTV:
		case SSHORTV:
			if(from->short_value!=NULL) svalue=*from->short_value;
				else svalue=0;
			if(freeit)
			{
				if(data->short_value!=NULL)
					Rfree(data->short_value);
			}
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(from->integer_value!=NULL) ivalue=*from->integer_value;
				else ivalue=0;
			if(freeit) if(data->integer_value!=NULL) Rfree(data->integer_value);
			break;
		case CHARACTERS:
		case BOOLNS:
			if(from->string_value!=NULL) cvalue=*from->string_value;
				else cvalue=0;
			if(freeit) if(data->string_value!=NULL) Rfree(data->string_value);
			break;
		default:
			prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",newtype,line,file);
			if(string!=NULL) Rfree(string);
			return(-1);
	}
	switch(*type)
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
			switch(newtype)
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
					if(!isEMPTY(string))
					{
						data->string_value=stralloc(string);
						*dlen=RDAstrlen(data->string_value)+1;
					} else {
						data->string_value=NULL;
						*dlen=0;
					}
					break;
				case CHARACTERS:
				case BOOLNS:
					data->string_value=Rmalloc(1);
					*data->string_value=0;
					*dlen=1;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					data->float_value=Rmalloc(sizeof(double));
					*data->float_value=0;
					*dlen=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					data->short_value=Rmalloc(sizeof(short));
					*data->short_value=0;
					*dlen=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					data->integer_value=Rmalloc(sizeof(int));
					*data->integer_value=0;
					*dlen=sizeof(int);
					break;
				default:
					prterr("Error Field Type [%d] is invalid at line [%d] program [%s].",newtype,line,file);
					if(string!=NULL) Rfree(string);
					return(-1);
			}
			break;
		case CHARACTERS:
		case BOOLNS:
			switch(newtype)
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
					if(*type==BOOLNS)
					{
						sprintf(stemp,"%d",cvalue);
					} else sprintf(stemp,"%c",cvalue);
					data->string_value=stralloc(stemp);;
					*dlen=RDAstrlen(data->string_value)+1;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					data->float_value=Rmalloc(sizeof(double));
					*data->float_value=cvalue;
					*dlen=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					data->short_value=Rmalloc(sizeof(short));
					*data->short_value=cvalue;
					*dlen=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					data->integer_value=Rmalloc(sizeof(int));
					*data->integer_value=cvalue;
					*dlen=sizeof(int);
					break;
				case CHARACTERS:
				case BOOLNS:
					data->string_value=Rmalloc(1);
					*data->string_value=cvalue;
					*dlen=1;
					break;
				default:
					prterr("Error Incompatible Field Type [%d] is invalid at line [%d] program [%s].",newtype,line,file);
					if(string!=NULL) Rfree(string);
					return(-1);
			}
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			switch(newtype)
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
					data->string_value=NULL;
					*dlen=0;
					break;
				case CHARACTERS:
				case BOOLNS:
					data->string_value=Rmalloc(1);
					*data->string_value=(fvalue ? TRUE:FALSE);
					*dlen=1;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					data->float_value=Rmalloc(sizeof(double));
					*dlen=sizeof(double);
					if(newtype==DOLLARS && *type==DOLLARS_NOCENTS)
					{
						*data->float_value=fvalue/100;
					} else if(newtype==DOLLARS_NOCENTS &&
						*type==DOLLARS)
					{
						*data->float_value=fvalue*100;
					} else {
						*data->float_value=fvalue;
					}
					break;
				case SHORTV:
				case SSHORTV:
					data->short_value=Rmalloc(sizeof(short));
					*dlen=sizeof(short);
/*lint -e524 */
					*data->short_value=fvalue;
/*lint +e524 */
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					data->integer_value=Rmalloc(sizeof(int));
					*dlen=sizeof(int);
/*lint -e524 */
					*data->integer_value=fvalue;
/*lint +e524 */
					break;
				default:
					prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",newtype,line,file);
					if(string!=NULL) Rfree(string);
					return(-1);
			}
			break;
		case SHORTV:
		case SSHORTV:
			switch(newtype)
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
					data->string_value=NULL;
					*dlen=0;
					break;
				case CHARACTERS:
				case BOOLNS:
					data->string_value=Rmalloc(1);
					*dlen=1;
					*data->string_value=svalue;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					data->float_value=Rmalloc(sizeof(float));
					*dlen=sizeof(float);
					*data->float_value=svalue;
					break;
				case SHORTV:
				case SSHORTV:
					data->short_value=Rmalloc(sizeof(short));
					*dlen=sizeof(short);
					*data->short_value=svalue;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					data->integer_value=Rmalloc(sizeof(int));
					*dlen=sizeof(int);
					*data->integer_value=svalue;
					break;
				default:
					prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",newtype,line,file);
					if(string!=NULL) Rfree(string);
					return(-1);
			}
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			switch(newtype)
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
					data->string_value=NULL;
					*dlen=0;
					break;
				case CHARACTERS:
				case BOOLNS:
					data->string_value=Rmalloc(1);
					*dlen=1;
					*data->string_value=ivalue;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					data->float_value=Rmalloc(sizeof(double));
					*dlen=sizeof(double);
					*data->float_value=ivalue;
					break;
				case SHORTV:
				case SSHORTV:
					data->short_value=Rmalloc(sizeof(short));
					*dlen=sizeof(short);
/*lint -e524*/
					*data->short_value=ivalue;
/*lint +e524*/
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					data->integer_value=Rmalloc(sizeof(int));
					*dlen=sizeof(int);
					*data->integer_value=ivalue;
					break;
				default:
					prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",newtype,line,file);
					if(string!=NULL) Rfree(string);
					return(-1);
			}
			break;
		default:
			prterr("Error Field Type [%d] is invalid at line [%d] program [%s].",*type,line,file);
			if(string!=NULL) Rfree(string);
			return(-1);
	}
	*type=newtype;
	*len=newlen;
	if(string!=NULL) Rfree(string);
	return(0);
}
void xMakeSpaceMatrixColumn(int column,int line,char *file)
{
	MATRIXvalue *v,*v1,*v2;
	MATRIXcell *c;
	MATRIXmem *mem1=NULL,*mem2=NULL,*holdmem=NULL;
	MATRIXrec *rec;
	int x,y,z,a=0,holdnum=0;

	if(diagapps)
	{
		prterr("DIAG MakeSpaceMatrixColumn at line [%d] program [%s].",
			line,file);
	}
	if(DFV->recs!=NULL)
	{
		for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
		{
			if(rec->mems!=NULL)
			{
				holdmem=Rmalloc((rec->num+1)*sizeof(MATRIXmem));
				holdnum=rec->num+1;
				a=0;
				for(y=0,mem2=rec->mems;y<rec->num;++y,++mem2)
				{
					if(y==column)
					{
						mem1=holdmem+a;
						mem1->sensitive=TRUE;
						if(DFV->defs!=NULL)
						{
							mem1->num=DFV->numdefs;
							mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
							for(z=0,v=mem1->values,c=DFV->defs;z<DFV->numdefs;
								++z,++c,++v)
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
										v->data.string_value=NULL;
										v->dlen=0;
										break;
									case CHARACTERS:
									case BOOLNS:
										v->data.string_value=Rmalloc(1);
										*v->data.string_value=0;
										v->dlen=1;
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										v->data.float_value=Rmalloc(sizeof(double));
										*v->data.float_value=0;
										v->dlen=sizeof(double);
										break;
									case SHORTV:
									case SSHORTV:
										v->data.short_value=Rmalloc(sizeof(short));
										*v->data.short_value=0;
										v->dlen=sizeof(short);
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										v->data.integer_value=Rmalloc(sizeof(int));
										*v->data.integer_value=0;
										v->dlen=sizeof(int);
										break;
									default:
										prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
										break;
								}
							}
						} else {
							mem1->num=0;
							mem1->values=NULL;
						}
						++a;
					}
					mem1=holdmem+a;
					mem1->sensitive=mem2->sensitive;
					if(mem2->values!=NULL && DFV->defs!=NULL)
					{
						mem1->num=mem2->num;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem1->values,v2=mem2->values;z<mem1->num;
							++z,++v1,++v2)
						{
							c=DFV->defs+z;
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									if(v2->data.string_value!=NULL)
									{
										v1->data.string_value=
											stralloc(v2->data.string_value);
										Rfree(v2->data.string_value);
									} else {
										v1->data.string_value=NULL;
										v1->dlen=0;
									}
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									if(v2->data.string_value!=NULL)
									{
										*v1->data.string_value=*v2->data.string_value;
										Rfree(v2->data.string_value);
									}
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v1->data.float_value=Rmalloc(sizeof(double));
									v1->dlen=sizeof(double);
									*v1->data.float_value=0;
									if(v2->data.float_value!=NULL)
									{
										*v1->data.float_value=
											*v2->data.float_value;
										Rfree(v2->data.float_value);
									}
									break;
								case SHORTV:
								case SSHORTV:
									v1->data.short_value=Rmalloc(sizeof(short));
									v1->dlen=sizeof(short);
									*v1->data.short_value=0;
									if(v2->data.short_value!=NULL)
									{
										*v1->data.short_value=
											*v2->data.short_value;
										Rfree(v2->data.short_value);
									}
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v1->data.integer_value=Rmalloc(sizeof(int));
									v1->dlen=sizeof(int);
									*v1->data.integer_value=0;
									if(v2->data.integer_value!=NULL)
									{
										*v1->data.integer_value=
											*v2->data.integer_value;
										Rfree(v2->data.integer_value);
									}
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
						Rfree(mem2->values);
					}  else if(DFV->defs!=NULL)
					{
						mem1->num=DFV->numdefs;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem1->values,c=DFV->defs;z<DFV->numdefs;
							++z,++c,++v1)
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									v1->data.string_value=NULL;
									v1->dlen=0;
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
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
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
					} else {
						mem1->num=0;
						mem1->values=NULL;
					}
					++a;
				}
				if(a<holdnum)
				{
					mem1=holdmem+a;
					mem1->sensitive=TRUE;
					if(DFV->defs!=NULL)
					{
						mem1->num=DFV->numdefs;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v=mem1->values,c=DFV->defs;z<DFV->numdefs;
							++z,++c,++v)
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									v->data.string_value=NULL;
									v->dlen=0;
									break;
								case CHARACTERS:
								case BOOLNS:
									v->data.string_value=Rmalloc(1);
									*v->data.string_value=0;
									v->dlen=1;
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v->data.float_value=Rmalloc(sizeof(double));
									*v->data.float_value=0;
									v->dlen=sizeof(double);
									break;
								case SHORTV:
								case SSHORTV:
									v->data.short_value=Rmalloc(sizeof(short));
									*v->data.short_value=0;
									v->dlen=sizeof(short);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v->data.integer_value=Rmalloc(sizeof(int));
									*v->data.integer_value=0;
									v->dlen=sizeof(int);
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
					} else {
						mem1->num=0;
						mem1->values=NULL;
					}
				}
				Rfree(rec->mems);
				rec->num=holdnum;
				rec->mems=Rmalloc(rec->num*sizeof(MATRIXmem));
				for(y=0,mem1=rec->mems,mem2=holdmem;y<rec->num;++y,++mem1,
					++mem2)
				{
					mem1->sensitive=mem2->sensitive;
					if(mem2->values!=NULL && DFV->defs!=NULL)
					{
						mem1->num=mem2->num;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem1->values,v2=mem2->values;z<mem1->num;
							++z,++v1,++v2)
						{
							c=DFV->defs+z;
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									if(v2->data.string_value!=NULL)
									{
										v1->data.string_value=
											stralloc(v2->data.string_value);
										Rfree(v2->data.string_value);
									} else {
										v1->data.string_value=NULL;
										v1->dlen=0;
									}
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									if(v2->data.string_value!=NULL)
									{
										*v1->data.string_value=
											*v2->data.string_value;
										Rfree(v2->data.string_value);
									}
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v1->data.float_value=Rmalloc(sizeof(double));
									v1->dlen=sizeof(double);
									*v1->data.float_value=0;
									if(v2->data.float_value!=NULL)
									{
										*v1->data.float_value=
											*v2->data.float_value;
										Rfree(v2->data.float_value);
									}
									break;
								case SHORTV:
								case SSHORTV:
									v1->data.short_value=Rmalloc(sizeof(short));
									v1->dlen=sizeof(short);
									*v1->data.short_value=0;
									if(v2->data.short_value!=NULL)
									{
										*v1->data.short_value=
											*v2->data.short_value;
										Rfree(v2->data.short_value);
									}
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v1->data.integer_value=Rmalloc(sizeof(int));
									v1->dlen=sizeof(int);
									*v1->data.integer_value=0;
									if(v2->data.integer_value!=NULL)
									{
										*v1->data.integer_value=
											*v2->data.integer_value;
										Rfree(v2->data.integer_value);
									}
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
						Rfree(mem2->values);
					}
				}
				Rfree(holdmem);
				holdmem=NULL;
				holdnum=0;
			} else { /* Currently No Members */
				rec->mems=Rmalloc(sizeof(MATRIXmem));
				rec->num=1;
				mem1=rec->mems;
				mem1->sensitive=TRUE;
				if(DFV->defs!=NULL)
				{
					mem1->num=DFV->numdefs;
					mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
					for(z=0,v=mem1->values,c=DFV->defs;z<DFV->numdefs;
						++z,++c,++v)
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
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
								v->data.string_value=NULL;
								v->dlen=0;
								break;
							case CHARACTERS:
							case BOOLNS:
								v->data.string_value=Rmalloc(1);
								*v->data.string_value=0;
								v->dlen=1;
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								v->data.float_value=Rmalloc(sizeof(double));
								*v->data.float_value=0;
								v->dlen=sizeof(double);
								break;
							case SHORTV:
							case SSHORTV:
								v->data.short_value=Rmalloc(sizeof(short));
								*v->data.short_value=0;
								v->dlen=sizeof(short);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								v->data.integer_value=Rmalloc(sizeof(int));
								*v->data.integer_value=0;
								v->dlen=sizeof(int);
								break;
							default:
								prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
								break;
						}
					}
				} else {
					mem1->num=0;
					mem1->values=NULL;
				}
			}
		}
	}
}
void xDeleteSpaceMatrixColumn(int column,int line,char *file)
{
	MATRIXvalue *v,*v1,*v2;
	MATRIXcell *c;
	MATRIXmem *mem1=NULL,*mem2=NULL,*holdmem=NULL;
	MATRIXrec *rec;
	int x,y,z,a=0,holdnum=0;

	if(diagapps)
	{
		prterr("DIAG DeleteSpaceMatrixColumn at line [%d] program [%s].",
			line,file);
	}
	if(DFV->recs!=NULL)
	{
		for(x=0,rec=DFV->recs;x<DFV->num;++x,++rec)
		{
			if(rec->mems!=NULL && rec->num>column && rec->num>1)
			{
				holdmem=Rmalloc((rec->num-1)*sizeof(MATRIXmem));
				holdnum=rec->num-1;
				a=0;
				for(y=0,mem2=rec->mems;y<rec->num;++y,++mem2)
				{
					if(y==column)
					{
						if(mem2->values!=NULL && DFV->defs!=NULL) /* Removing Cells */
						{
							for(z=0,v=mem2->values;z<mem2->num;++z,++v)
							{
								c=DFV->defs+z;
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
									case CHARACTERS:
									case BOOLNS:
										if(v->data.string_value!=NULL)
											Rfree(v->data.string_value);
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										if(v->data.float_value!=NULL)
											Rfree(v->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										if(v->data.short_value!=NULL)
											Rfree(v->data.short_value);
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(v->data.integer_value!=NULL)
											Rfree(v->data.integer_value);
										break;
									default:
										prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
										break;
								}
							}
							Rfree(mem2->values);
						}
					} else {
						mem1=holdmem+a;
						mem1->sensitive=mem2->sensitive;
						if(mem2->values!=NULL && DFV->defs!=NULL) /* Copying Cells */
						{
							mem1->values=Rmalloc(mem2->num*
								sizeof(MATRIXvalue));
							mem1->num=mem2->num;
							for(z=0,v1=mem1->values,v2=mem2->values;
								z<mem1->num;++z,++v1,++v2)
							{
								c=DFV->defs+z;
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
										if(v2->data.string_value!=NULL)
										{
											v1->data.string_value=
												stralloc(v2->data.string_value);
											Rfree(v2->data.string_value);
										} else {
											v1->data.string_value=NULL;
											v1->dlen=0;
										}
										break;
									case CHARACTERS:
									case BOOLNS:
										v1->data.string_value=Rmalloc(1);
										*v1->data.string_value=0;
										v1->dlen=1;
										if(v2->data.string_value!=NULL)
										{
											*v1->data.string_value=
												*v2->data.string_value;
											Rfree(v2->data.string_value);
										}
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										v1->data.float_value=Rmalloc(sizeof(double));
										v1->dlen=sizeof(double);
										*v1->data.float_value=0;
										if(v2->data.float_value!=NULL)
										{
											*v1->data.float_value=
												*v2->data.float_value;
											Rfree(v2->data.float_value);
										}
										break;
									case SHORTV:
									case SSHORTV:
										v1->data.short_value=Rmalloc(sizeof(short));
										v1->dlen=sizeof(short);
										*v1->data.short_value=0;
										if(v2->data.short_value!=NULL)
										{
											*v1->data.short_value=
												*v2->data.short_value;
											Rfree(v2->data.short_value);
										}
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										v1->data.integer_value=Rmalloc(sizeof(int));
										v1->dlen=sizeof(int);
										*v1->data.integer_value=0;
										if(v2->data.integer_value!=NULL)
										{
											*v1->data.integer_value=
												*v2->data.integer_value;
											Rfree(v2->data.integer_value);
										}
										break;
									default:
										prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
										break;
								}
							}
							Rfree(mem2->values);
						} else {
							mem1->values=NULL; /* No Cells */
							mem1->num=0;
						}
						++a;
					}
				}
				Rfree(rec->mems);
				rec->num=holdnum;
				rec->mems=Rmalloc(rec->num*sizeof(MATRIXmem));
				for(y=0,mem1=holdmem,mem2=rec->mems;y<rec->num;
					++y,++mem1,++mem2)
				{
					mem2->sensitive=mem1->sensitive;
					if(mem1->values!=NULL && DFV->defs!=NULL)
					{
						mem2->num=mem1->num;
						mem2->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem2->values,v2=mem1->values;z<mem1->num;
							++z,++v1,++v2)
						{
							c=DFV->defs+z;
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									if(v2->data.string_value!=NULL)
									{
										v1->data.string_value=
											stralloc(v2->data.string_value);
										Rfree(v2->data.string_value);
									} else {
										v1->data.string_value=NULL;
										v1->dlen=0;
									}
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									if(v2->data.string_value!=NULL)
									{
										*v1->data.string_value=
											*v2->data.string_value;
										Rfree(v2->data.string_value);
									}
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v1->data.float_value=Rmalloc(sizeof(double));
									v1->dlen=sizeof(double);
									*v1->data.float_value=0;
									if(v2->data.float_value!=NULL)
									{
										*v1->data.float_value=
											*v2->data.float_value;
										Rfree(v2->data.float_value);
									}
									break;
								case SHORTV:
								case SSHORTV:
									v1->data.short_value=Rmalloc(sizeof(short));
									v1->dlen=sizeof(short);
									*v1->data.short_value=0;
									if(v2->data.short_value!=NULL)
									{
										*v1->data.short_value=
											*v2->data.short_value;
										Rfree(v2->data.short_value);
									}
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v1->data.integer_value=Rmalloc(sizeof(int));
									v1->dlen=sizeof(int);
									*v1->data.integer_value=0;
									if(v2->data.integer_value!=NULL)
									{
										*v1->data.integer_value=
											*v2->data.integer_value;
										Rfree(v2->data.integer_value);
									}
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
						Rfree(mem1->values);
					} else { /* No Cells */
						mem2->values=NULL;
						mem2->num=0;
					}
				}
				Rfree(holdmem);
				holdnum=0;
			} else if(rec->mems!=NULL) /* Free Members */
			{
				for(y=0,mem1=rec->mems;y<rec->num;++y,++mem1)
				{
					if(mem1->values!=NULL && DFV->defs!=NULL)
					{
						for(z=0,v=mem1->values;z<mem1->num;++z,++v)
						{
							c=DFV->defs+z;
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
								case CHARACTERS:
								case BOOLNS:
									if(v->data.string_value!=NULL)
										Rfree(v->data.string_value);
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									if(v->data.float_value!=NULL)
										Rfree(v->data.float_value);
									break;
								case SHORTV:
								case SSHORTV:
									if(v->data.short_value!=NULL)
										Rfree(v->data.short_value);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									if(v->data.integer_value!=NULL)
										Rfree(v->data.integer_value);
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
						Rfree(mem1->values);
					}
				}
				Rfree(rec->mems);
			}
		}
	}
}
void xMakeSpaceMatrixRow(int row,int line,char *file)
{
	MATRIXvalue *v,*v1,*v2;
	MATRIXcell *c;
	MATRIXmem *mem1=NULL,*mem2=NULL;
	MATRIXrec *rec1=NULL,*rec2=NULL,*holdrec=NULL;
	int q=0,x,y,z,a=0,holdnum=0;

	if(diagapps)
	{
		prterr("DIAG MakeSpaceMatrixRow at line [%d] program [%s].",
			line,file);
	}
	if(DFV->recs!=NULL)
	{
		holdnum=DFV->num+1;
		holdrec=Rmalloc(holdnum*sizeof(MATRIXrec));
		a=0;
		for(q=0,rec2=DFV->recs;q<DFV->num;++q,++rec2)
		{
			if(row==q)
			{
				rec1=holdrec+a;
				if(DFV->xaxis.mem!=NULL)
				{
					rec1->num=DFV->xaxis.num;
					rec1->mems=Rmalloc(rec1->num*sizeof(MATRIXmem));
					for(y=0,mem1=rec1->mems;y<rec1->num;++y,++mem1)
					{
						mem1->sensitive=TRUE;
						if(DFV->defs!=NULL)
						{
							mem1->num=DFV->numdefs;
							mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
							for(x=0,v=mem1->values,c=DFV->defs;x<mem1->num;
								++x,++v,++c)
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
										v->data.string_value=NULL;
										v->dlen=0;
										break;
									case CHARACTERS:
									case BOOLNS:
										v->data.string_value=Rmalloc(1);
										*v->data.string_value=0;
										v->dlen=1;
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										v->data.float_value=Rmalloc(sizeof(double));
										*v->data.float_value=0;
										v->dlen=sizeof(double);
										break;
									case SHORTV:
									case SSHORTV:
										v->data.short_value=Rmalloc(sizeof(short));
										*v->data.short_value=0;
										v->dlen=sizeof(short);
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										v->data.integer_value=Rmalloc(sizeof(int));
										*v->data.integer_value=0;
										v->dlen=sizeof(int);
										break;
									default:
										prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
										break;
								}
							}
						} else {
							mem1->values=NULL;
							mem1->num=0;
						}
					}
				} else {
					rec1->num=0;
					rec1->mems=NULL;
				}
				++a;
			}
			rec1=holdrec+a;
			if(rec2->mems!=NULL)
			{
				rec1->num=rec2->num;
				rec1->mems=Rmalloc(rec1->num*sizeof(MATRIXmem));
				for(y=0,mem1=rec1->mems,mem2=rec2->mems;y<rec2->num;
					++y,++mem1,++mem2)
				{
					mem1->sensitive=mem2->sensitive;
					if(mem2->values!=NULL && DFV->defs!=NULL)
					{
						mem1->num=mem2->num;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem1->values,v2=mem2->values;z<mem1->num;
							++z,++v1,++v2)
						{
							c=DFV->defs+z;
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									if(v2->data.string_value!=NULL)
									{
										v1->data.string_value=
											stralloc(v2->data.string_value);
										Rfree(v2->data.string_value);
									} else {
										v1->data.string_value=NULL;
										v1->dlen=0;
									}
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									if(v2->data.string_value!=NULL)
									{
										*v1->data.string_value=
											*v2->data.string_value;
										Rfree(v2->data.string_value);
									}
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v1->data.float_value=Rmalloc(sizeof(double));
									v1->dlen=sizeof(double);
									*v1->data.float_value=0;
									if(v2->data.float_value!=NULL)
									{
										*v1->data.float_value=
											*v2->data.float_value;
										Rfree(v2->data.float_value);
									}
									break;
								case SHORTV:
								case SSHORTV:
									v1->data.short_value=Rmalloc(sizeof(short));
									v1->dlen=sizeof(short);
									*v1->data.short_value=0;
									if(v2->data.short_value!=NULL)
									{
										*v1->data.short_value=
											*v2->data.short_value;
										Rfree(v2->data.short_value);
									}
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v1->data.integer_value=Rmalloc(sizeof(int));
									v1->dlen=sizeof(int);
									*v1->data.integer_value=0;
									if(v2->data.integer_value!=NULL)
									{
										*v1->data.integer_value=
											*v2->data.integer_value;
										Rfree(v2->data.integer_value);
									}
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
						Rfree(mem2->values);
					} else {
						mem1->num=0;
						mem1->values=NULL;
					}
				}
				Rfree(rec2->mems);
			} else {
				rec1->num=0;
				rec1->mems=NULL;
			}
			++a;
		}
		if(a<holdnum)
		{
			rec1=holdrec+a;
			if(DFV->xaxis.mem!=NULL)
			{
				rec1->num=DFV->xaxis.num;
				rec1->mems=Rmalloc(rec1->num*sizeof(MATRIXmem));
				for(y=0,mem1=rec1->mems;y<rec1->num;++y,++mem1)
				{
					mem1->sensitive=TRUE;
					if(DFV->defs!=NULL)
					{
						mem1->num=DFV->numdefs;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(x=0,v=mem1->values,c=DFV->defs;x<mem1->num;
							++x,++v,++c)
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									v->data.string_value=NULL;
									v->dlen=0;
									break;
								case CHARACTERS:
								case BOOLNS:
									v->data.string_value=Rmalloc(1);
									*v->data.string_value=0;
									v->dlen=1;
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v->data.float_value=Rmalloc(sizeof(double));
									*v->data.float_value=0;
									v->dlen=sizeof(double);
									break;
								case SHORTV:
								case SSHORTV:
									v->data.short_value=Rmalloc(sizeof(short));
									*v->data.short_value=0;
									v->dlen=sizeof(short);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v->data.integer_value=Rmalloc(sizeof(int));
									*v->data.integer_value=0;
									v->dlen=sizeof(int);
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
					} else {
						mem1->values=NULL;
						mem1->num=0;
					}
				}
			} else {
				rec1->num=0;
				rec1->mems=NULL;
			}
		}
		Rfree(DFV->recs);
		DFV->num=holdnum;
		DFV->recs=Rmalloc(holdnum*sizeof(MATRIXrec));
		for(x=0,rec1=DFV->recs,rec2=holdrec;x<DFV->num;++x,++rec1,++rec2)
		{
			if(rec2->mems!=NULL)
			{
				rec1->num=rec2->num;
				rec1->mems=Rmalloc(rec1->num*sizeof(MATRIXmem));
				for(y=0,mem1=rec1->mems,mem2=rec2->mems;
					y<rec1->num;++y,++mem1,++mem2)
				{
					mem1->sensitive=mem2->sensitive;
					if(mem2->values!=NULL && DFV->defs!=NULL)
					{
						mem1->num=mem2->num;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem1->values,v2=mem2->values;z<mem1->num;
							++z,++v1,++v2)
						{
							c=DFV->defs+z;
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									if(v2->data.string_value!=NULL)
									{
										v1->data.string_value=
											stralloc(v2->data.string_value);
										Rfree(v2->data.string_value);
									} else {
										v1->data.string_value=NULL;
										v1->dlen=0;
									}
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									if(v2->data.string_value!=NULL)
									{
										*v1->data.string_value=
											*v2->data.string_value;
										Rfree(v2->data.string_value);
									}
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v1->data.float_value=Rmalloc(sizeof(double));
									v1->dlen=sizeof(double);
									*v1->data.float_value=0;
									if(v2->data.float_value!=NULL)
									{
										*v1->data.float_value=
											*v2->data.float_value;
										Rfree(v2->data.float_value);
									}
									break;
								case SHORTV:
								case SSHORTV:
									v1->data.short_value=Rmalloc(sizeof(short));
									v1->dlen=sizeof(short);
									*v1->data.short_value=0;
									if(v2->data.short_value!=NULL)
									{
										*v1->data.short_value=
											*v2->data.short_value;
										Rfree(v2->data.short_value);
									}
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v1->data.integer_value=Rmalloc(sizeof(int));
									v1->dlen=sizeof(int);
									*v1->data.integer_value=0;
									if(v2->data.integer_value!=NULL)
									{
										*v1->data.integer_value=
											*v2->data.integer_value;
										Rfree(v2->data.integer_value);
									}
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
						Rfree(mem2->values);
					} else if(DFV->defs!=NULL)
					{
						mem1->num=DFV->numdefs;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem1->values,c=DFV->defs;z<DFV->numdefs;
							++z,++c,++v1)
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									v1->data.string_value=NULL;
									v1->dlen=0;
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
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
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
					} else {
						mem1->values=NULL;
						mem1->num=0;
					}
				}
				Rfree(rec2->mems);
			} else {
				rec1->num=0;
				rec1->mems=NULL;
			}
		}
		Rfree(holdrec);
		holdrec=NULL;
		holdnum=0;
	} else {
		DFV->recs=Rmalloc(sizeof(MATRIXrec));
		DFV->num=1;
		rec1=DFV->recs;
		if(DFV->xaxis.mem!=NULL)
		{
			rec1->num=DFV->xaxis.num;
			rec1->mems=Rmalloc(rec1->num*sizeof(MATRIXmem));
			for(y=0,mem1=rec1->mems;y<rec1->num;++y,++mem1)
			{
				mem1->sensitive=TRUE;
				if(DFV->defs!=NULL)
				{
					mem1->num=DFV->numdefs;
					mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
					for(x=0,v=mem1->values,c=DFV->defs;x<mem1->num;++x,++v,++c)
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
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
								v->data.string_value=NULL;
								v->dlen=0;
								break;
							case CHARACTERS:
							case BOOLNS:
								v->data.string_value=Rmalloc(1);
								*v->data.string_value=0;
								v->dlen=1;
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								v->data.float_value=Rmalloc(sizeof(double));
								*v->data.float_value=0;
								v->dlen=sizeof(double);
								break;
							case SHORTV:
							case SSHORTV:
								v->data.short_value=Rmalloc(sizeof(short));
								*v->data.short_value=0;
								v->dlen=sizeof(short);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								v->data.integer_value=Rmalloc(sizeof(int));
								*v->data.integer_value=0;
								v->dlen=sizeof(int);
								break;
							default:
								prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
								break;
						}
					}
				} else {
					mem1->values=NULL;
					mem1->num=0;
				}
			}
		} else {
			rec1->num=0;
			rec1->mems=NULL;
		}
	}
}
void xDeleteSpaceMatrixRow(int row,int line,char *file)
{
	MATRIXvalue *v,*v1,*v2;
	MATRIXcell *c;
	MATRIXmem *mem1=NULL,*mem2=NULL;
	MATRIXrec *rec1=NULL,*rec2=NULL,*holdrec=NULL;
	int x,y,z,a=0,holdnum=0;

	if(diagapps)
	{
		prterr("DIAG DeleteSpaceMatrixRow at line [%d] program [%s].",
			line,file);
	}
	if(DFV->recs!=NULL && DFV->num>1)
	{
		holdnum=DFV->num-1;
		holdrec=Rmalloc(holdnum*sizeof(MATRIXrec));
		a=0;
		for(x=0,rec2=DFV->recs;x<DFV->num;++x,++rec2)
		{
			if(row!=x)
			{
				rec1=holdrec+a;
				if(rec2->mems!=NULL)
				{
					rec1->num=rec2->num;
					rec1->mems=Rmalloc(rec1->num*sizeof(MATRIXmem));
					for(y=0,mem1=rec1->mems,mem2=rec2->mems;y<rec2->num;
						++y,++mem1,++mem2)
					{
						mem1->sensitive=mem2->sensitive;
						if(mem2->values!=NULL && DFV->defs!=NULL)
						{
							mem1->num=mem2->num;
							mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
							for(z=0,v1=mem1->values,v2=mem2->values;
								z<mem2->num;++z,++v1,++v2)
							{
								c=DFV->defs+z;
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
										if(v2->data.string_value!=NULL)
										{
											v1->data.string_value=
												stralloc(v2->data.string_value);
											Rfree(v2->data.string_value);
										} else {
											v1->data.string_value=NULL;
											v1->dlen=0;
										}
										break;
									case CHARACTERS:
									case BOOLNS:
										v1->data.string_value=Rmalloc(1);
										*v1->data.string_value=0;
										v1->dlen=1;
										if(v2->data.string_value!=NULL)
										{
											*v1->data.string_value=
												*v2->data.string_value;
											Rfree(v2->data.string_value);
										}
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										v1->data.float_value=Rmalloc(sizeof(double));
										v1->dlen=sizeof(double);
										*v1->data.float_value=0;
										if(v2->data.float_value!=NULL)
										{
											*v1->data.float_value=
												*v2->data.float_value;
											Rfree(v2->data.float_value);
										}
										break;
									case SHORTV:
									case SSHORTV:
										v1->data.short_value=Rmalloc(sizeof(short));
										v1->dlen=sizeof(short);
										*v1->data.short_value=0;
										if(v2->data.short_value!=NULL)
										{
											*v1->data.short_value=
												*v2->data.short_value;
											Rfree(v2->data.short_value);
										}
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										v1->data.integer_value=Rmalloc(sizeof(int));
										v1->dlen=sizeof(int);
										*v1->data.integer_value=0;
										if(v2->data.integer_value!=NULL)
										{
											*v1->data.integer_value=
												*v2->data.integer_value;
											Rfree(v2->data.integer_value);
										}
										break;
									default:
										prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
										break;
								}
							}
							Rfree(mem2->values);
						} else {
							mem1->num=0;
							mem1->values=NULL;
						}
					}
					Rfree(rec2->mems);
				} else {
					rec1->num=0;
					rec1->mems=NULL;
				}
				++a;
			} else {
				if(rec2->mems!=NULL)
				{
					for(y=0,mem2=rec2->mems;y<rec2->num;++y,++mem2)
					{
						if(mem2->values!=NULL && DFV->defs!=NULL)
						{
							for(z=0,v2=mem2->values;z<mem2->num;++z,++v2)
							{
								c=DFV->defs+z;
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
										if(v2->data.string_value!=NULL)
										{
											Rfree(v2->data.string_value);
											v2->dlen=0;
										}
										break;
									case CHARACTERS:
									case BOOLNS:
										if(v2->data.string_value!=NULL)
										{
											Rfree(v2->data.string_value);
											v2->dlen=0;
										}
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										if(v2->data.float_value!=NULL)
										{
											Rfree(v2->data.float_value);
											v2->dlen=0;
										}
										break;
									case SHORTV:
									case SSHORTV:
										if(v2->data.short_value!=NULL)
										{
											Rfree(v2->data.short_value);
											v2->dlen=0;
										}
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(v2->data.integer_value!=NULL)
										{
											Rfree(v2->data.integer_value);
											v2->dlen=0;
										}
										break;
									default:
										prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
										break;
								}
							}
							Rfree(mem2->values);
						}
					}
					Rfree(rec2->mems);
				}
			}
		}
		Rfree(DFV->recs);
		DFV->num=holdnum;
		DFV->recs=Rmalloc(holdnum*sizeof(MATRIXrec));
		for(x=0,rec1=DFV->recs,rec2=holdrec;x<DFV->num;++x,++rec1,++rec2)
		{
			if(rec2->mems!=NULL)
			{
				rec1->num=rec2->num;
				rec1->mems=Rmalloc(rec1->num*sizeof(MATRIXmem));
				for(y=0,mem1=rec1->mems,mem2=rec2->mems;
					y<rec1->num;++y,++mem1,++mem2)
				{
					mem1->sensitive=mem2->sensitive;
					if(mem2->values!=NULL && DFV->defs!=NULL)
					{
						mem1->num=mem2->num;
						mem1->values=Rmalloc(mem1->num*sizeof(MATRIXvalue));
						for(z=0,v1=mem1->values,v2=mem2->values;z<mem1->num;
							++z,++v1,++v2)
						{
							c=DFV->defs+z;
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									if(v2->data.string_value!=NULL)
									{
										v1->data.string_value=
											stralloc(v2->data.string_value);
										Rfree(v2->data.string_value);
									} else {
										v1->data.string_value=NULL;
										v1->dlen=0;
									}
									break;
								case CHARACTERS:
								case BOOLNS:
									v1->data.string_value=Rmalloc(1);
									*v1->data.string_value=0;
									v1->dlen=1;
									if(v2->data.string_value!=NULL)
									{
										*v1->data.string_value=
											*v2->data.string_value;
										Rfree(v2->data.string_value);
									}
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									v1->data.float_value=Rmalloc(sizeof(double));
									v1->dlen=sizeof(double);
									*v1->data.float_value=0;
									if(v2->data.float_value!=NULL)
									{
										*v1->data.float_value=
											*v2->data.float_value;
										Rfree(v2->data.float_value);
									}
									break;
								case SHORTV:
								case SSHORTV:
									v1->data.short_value=Rmalloc(sizeof(short));
									v1->dlen=sizeof(short);
									*v1->data.short_value=0;
									if(v2->data.short_value!=NULL)
									{
										*v1->data.short_value=
											*v2->data.short_value;
										Rfree(v2->data.short_value);
									}
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									v1->data.integer_value=Rmalloc(sizeof(int));
									v1->dlen=sizeof(int);
									*v1->data.integer_value=0;
									if(v2->data.integer_value!=NULL)
									{
										*v1->data.integer_value=
											*v2->data.integer_value;
										Rfree(v2->data.integer_value);
									}
									break;
								default:
									prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
									break;
							}
						}
						Rfree(mem2->values);
					} else {
						mem1->values=NULL;
						mem1->num=0;
					}
				}
				Rfree(rec2->mems);
			} else {
				rec1->num=0;
				rec1->mems=NULL;
			}
		}
		Rfree(holdrec);
		holdrec=NULL;
		holdnum=0;
	} else if(DFV->recs!=NULL)
	{
		rec1=DFV->recs;
		for(y=0,mem1=rec1->mems;y<rec1->num;++y,++mem1)
		{
			if(mem1->values!=NULL && DFV->defs!=NULL)
			{
				for(x=0,v=mem1->values,c=DFV->defs;x<mem1->num;++x,++v,++c)
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
						case CHARACTERS:
						case BOOLNS:
							if(v->data.string_value!=NULL)
								Rfree(v->data.string_value);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							if(v->data.float_value!=NULL)
								Rfree(v->data.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							if(v->data.short_value!=NULL)
								Rfree(v->data.short_value);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(v->data.integer_value!=NULL)
								Rfree(v->data.integer_value);
							break;
						default:
							prterr("Error Incompatible New Field Type [%d] is invalid at line [%d] program [%s].",c->type,line,file);
							break;
					}
				}
				Rfree(mem1->values);
			}
		}
		Rfree(DFV->recs);
		DFV->num=0;
		DFV->recs=NULL;
	}
}
