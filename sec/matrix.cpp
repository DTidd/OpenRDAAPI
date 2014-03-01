/* matrix.c - RDAMatrix Functions */
#include<sec.hpp>

RDAMATRIX *Internal_RDAMATRIXES=NULL;
int Internal_num_RDAMATRIXES=0;

RDAMATRIX *xRDAMATRIXNEW(char *module,char *name,int line,char *file)
{
	RDAMATRIX *m=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG RDAMATRIXNEW at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	m=Rmalloc(sizeof(RDAMATRIX));
	if(!isEMPTY(module)) m->module=stralloc(module);
		else m->module=NULL;
	if(!isEMPTY(name)) m->name=stralloc(name);
		else m->name=NULL;
	m->title1=NULL;
	m->title2=NULL;
	m->desc=NULL;
	m->matrix_buttons=0;
	m->cell_config=0;
	m->locate_method=0;
	m->yaxis.title1=NULL;
	m->yaxis.title2=NULL;
	m->yaxis.desc=NULL;
	m->yaxis.columns=0;
	m->yaxis.axis_type=5;
	m->yaxis.contype=0;
	m->yaxis.conresource=NULL;
	m->yaxis.type=(-1);
	m->yaxis.len=0;
	m->yaxis.num=0;
	m->yaxis.mem=NULL;
	m->yaxis.print_title1=TRUE;
	m->yaxis.print_title2=TRUE;
	m->xaxis.title1=NULL;
	m->xaxis.title2=NULL;
	m->xaxis.desc=NULL;
	m->xaxis.columns=0;
	m->xaxis.axis_type=5;
	m->xaxis.type=(-1);
	m->xaxis.len=0;
	m->xaxis.print_title1=TRUE;
	m->xaxis.print_title2=TRUE;
	m->xaxis.contype=0;
	m->xaxis.conresource=NULL;
	m->xaxis.num=0;
	m->xaxis.mem=NULL;
	m->numdefs=0;
	m->defs=NULL;
	m->num=0;
	m->recs=NULL;
	m->which=0;
	m->numinc=0;
	m->incvir=NULL;
	return(m);
}
void xFreeRDAMATRIX(RDAMATRIX *m,int line,char *file)
{
	DFincvir *i;
	MATRIXrec *rec;
	MATRIXcell *cell;
	MATRIXmem *mem;
	MATRIXvalue *v;
	AXISmem *am=NULL;
	int x,y,z;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG FreeRDAMATRIX at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(m!=NULL)
	{
		if(m->module!=NULL) Rfree(m->module);
		if(m->name!=NULL) Rfree(m->name);
		if(m->title1!=NULL) Rfree(m->title1);
		if(m->title2!=NULL) Rfree(m->title2);
		if(m->desc!=NULL) Rfree(m->desc);
		if(m->yaxis.title1!=NULL) Rfree(m->yaxis.title1);
		if(m->yaxis.title2!=NULL) Rfree(m->yaxis.title2);
		if(m->yaxis.desc!=NULL) Rfree(m->yaxis.desc);
		if(m->yaxis.mem!=NULL)
		{
			for(x=0,am=m->yaxis.mem;x<m->yaxis.num;++x,++am)
			{
				if(am->title1!=NULL) Rfree(am->title1);
				if(am->title2!=NULL) Rfree(am->title2);
				if(m->yaxis.axis_type<5)
				{
				switch(m->yaxis.type)
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
						prterr("Error field type [%s] invalid for Connect Resource [%s] at line [%d] program [%s].",m->yaxis.type,m->yaxis.conresource,line,file);
						break;
				}
				}
			}
			Rfree(m->yaxis.mem);
		}
		if(m->yaxis.conresource!=NULL) Rfree(m->yaxis.conresource);
		if(m->xaxis.title1!=NULL) Rfree(m->xaxis.title1);
		if(m->xaxis.title2!=NULL) Rfree(m->xaxis.title2);
		if(m->xaxis.desc!=NULL) Rfree(m->xaxis.desc);
		if(m->xaxis.mem!=NULL)
		{
			for(x=0,am=m->xaxis.mem;x<m->xaxis.num;++x,++am)
			{
				if(am->title1!=NULL) Rfree(am->title1);
				if(am->title2!=NULL) Rfree(am->title2);
				if(m->xaxis.axis_type<5)
				{
				switch(m->xaxis.type)
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
						prterr("Error field type [%s] invalid for Connect Resource [%s] at line [%d] program [%s].",m->xaxis.type,m->xaxis.conresource,line,file);
						break;
				}
				}
			}
			Rfree(m->xaxis.mem);
		}
		if(m->xaxis.conresource!=NULL) Rfree(m->xaxis.conresource);
		if(m->incvir!=NULL)
		{
			for(x=0,i=m->incvir;x<m->numinc;++x,++i)
			{
				if(i->module!=NULL) Rfree(i->module);
				if(i->file!=NULL) Rfree(i->file);
				if(i->keyname!=NULL) Rfree(i->keyname);
			}
			Rfree(m->incvir);
		}
		if(m->recs!=NULL)
		{
			for(x=0,rec=m->recs;x<m->num;++x,++rec)
			{
				if(rec->mems!=NULL && m->defs!=NULL)
				{
					for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
					{
						if(mem->values!=NULL)
						{
							for(z=0,v=mem->values;z<mem->num;++z,++v)
							{
								cell=m->defs+z;
								switch(cell->type)
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
									case LONGV:
									case SLONGV:
										if(v->data.integer_value!=NULL) 
											Rfree(v->data.integer_value);
										break;
									default:
										prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",cell->type,cell->name,line,file);
										break;
								}
								v->dlen=0;
							}
							Rfree(mem->values);
						}
					}
					Rfree(rec->mems);
				}
			}
			Rfree(m->recs);
		}
		if(m->defs!=NULL)
		{
			for(x=0,cell=m->defs;x<m->numdefs;++x,++cell)
			{
				if(cell->name!=NULL) Rfree(cell->name);
			}
			Rfree(m->defs);
		}
		Rfree(m);
	}
}
short xgetRDAMatrixbin(char *libname,RDAMATRIX *matrix,int line,char *file)
{
	short a,test=FALSE;
	BIN *bin;
	DFincvir *i;
	MATRIXrec *rec;
	MATRIXcell *cell;
	MATRIXmem *mem;
	MATRIXvalue *v;
	AXISmem *am=NULL;
	int x,y,z;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG getRDAMatrixbin Attempting to Read RDAMatrix Binary [%s] from RDAMatrix Library [%s] at line [%d] program [%s].",matrix->name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,matrix->name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmatrix)
		{
			prterr("Error Attempting to Read RDAMatrix Binary [%s] from RDAMatrix Library [%s] Failed at line [%d] program [%s].",matrix->name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	a=BINgetshort(bin);
	if(a!=(-1701))
	{
		prterr("Error Invalid version or bad binary for RDAMatrix [%s] in RDAMatrix Binary Library [%s] at line [%d] program [%s].",matrix->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(matrix->module!=NULL) Rfree(matrix->module);
	if(matrix->name!=NULL) Rfree(matrix->name);
	test=BINgetshort(bin);
	if(test) matrix->module=BINgetstring(bin);
		else matrix->module=NULL;
	test=BINgetshort(bin);
	if(test) matrix->name=BINgetstring(bin);
		else matrix->name=NULL;
	test=BINgetshort(bin);
	if(test) matrix->desc=BINgetstring(bin);
		else matrix->desc=NULL;
	test=BINgetshort(bin);
	if(test) matrix->title1=BINgetstring(bin);
		else matrix->title1=NULL;
	test=BINgetshort(bin);
	if(test) matrix->title2=BINgetstring(bin);
		else matrix->title2=NULL;
	matrix->display_cell=BINgetint(bin);
	matrix->matrix_buttons=BINgetint(bin);
	matrix->cell_config=BINgetint(bin);
	matrix->locate_method=BINgetint(bin);
	matrix->which=BINgetint(bin);
	matrix->numinc=BINgetint(bin);
	if(matrix->numinc>0)
	{
		matrix->incvir=Rmalloc(matrix->numinc*sizeof(DFincvir));
		for(x=0,i=matrix->incvir;x<matrix->numinc;++x,++i)
		{
			test=BINgetshort(bin);
			if(test) i->module=BINgetstring(bin);
				else i->module=NULL;
			test=BINgetshort(bin);
			if(test) i->file=BINgetstring(bin);
				else i->file=NULL;
			test=BINgetshort(bin);
			if(test) i->keyname=BINgetstring(bin);
				else i->keyname=NULL;
			i->fileno=(-1);
			i->v=NULL;
		}
	} else {
		matrix->incvir=NULL;
		matrix->numinc=0;
	}
	test=BINgetshort(bin);
	if(test) matrix->yaxis.title1=BINgetstring(bin);
		else matrix->yaxis.title1=NULL;
	test=BINgetshort(bin);
	if(test) matrix->yaxis.title2=BINgetstring(bin);
		else matrix->yaxis.title2=NULL;
	test=BINgetshort(bin);
	if(test) matrix->yaxis.desc=BINgetstring(bin);
		else matrix->yaxis.desc=NULL;
	matrix->yaxis.columns=BINgetint(bin);
	matrix->yaxis.print_title1=BINgetshort(bin);
	matrix->yaxis.print_title2=BINgetshort(bin);
	matrix->yaxis.axis_type=BINgetint(bin);
	if(matrix->yaxis.axis_type!=5)
	{
		matrix->yaxis.type=BINgetshort(bin);
		matrix->yaxis.len=BINgetshort(bin);
	} else {
		matrix->yaxis.type=(-1);
		matrix->yaxis.len=0;
	}
	matrix->yaxis.contype=BINgetint(bin);
	test=BINgetshort(bin);
	if(test) matrix->yaxis.conresource=BINgetstring(bin);
		else matrix->yaxis.conresource=NULL;
	matrix->yaxis.num=BINgetint(bin);
	if(matrix->yaxis.num>0)
	{
		matrix->yaxis.mem=Rmalloc(matrix->yaxis.num*sizeof(AXISmem));
		for(x=0,am=matrix->yaxis.mem;x<matrix->yaxis.num;++x,++am)
		{
			test=BINgetshort(bin);
			if(test) am->title1=BINgetstring(bin);
				else am->title1=NULL;
			test=BINgetshort(bin);
			if(test) am->title2=BINgetstring(bin);
				else am->title2=NULL;
			if(matrix->yaxis.axis_type!=5)
			{
				switch(matrix->yaxis.type)
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
						test=BINgetshort(bin);
						if(test) 
						{
							am->data.string_value=BINgetstring(bin);
							am->dlen=RDAstrlen(am->data.string_value)+1;
						} else {
							am->data.string_value=NULL;
							am->dlen=0;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						am->data.string_value=Rmalloc(1);
						am->dlen=1;
						*am->data.string_value=BINgetshort(bin);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						am->data.float_value=Rmalloc(sizeof(double));
						am->dlen=sizeof(double);
						*am->data.float_value=BINgetdouble(bin);
						break;
					case SHORTV:
					case SSHORTV:
						am->data.short_value=Rmalloc(sizeof(short));
						am->dlen=sizeof(short);
						*am->data.short_value=BINgetshort(bin);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						am->data.integer_value=Rmalloc(sizeof(int));
						am->dlen=sizeof(int);
						*am->data.integer_value=BINgetint(bin);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%s] invalid for Connect Resource [%s] at line [%d] program [%s].",matrix->yaxis.type,matrix->yaxis.conresource,line,file);
						break;
				}
			} else {
				am->dlen=0;
				am->data.string_value=NULL;
				am->data.float_value=NULL;
				am->data.integer_value=NULL;
				am->data.short_value=NULL;
			}
		}
	} else {
		matrix->yaxis.mem=NULL;
		matrix->yaxis.num=0;
	}
	test=BINgetshort(bin);
	if(test) matrix->xaxis.title1=BINgetstring(bin);
		else matrix->xaxis.title1=NULL;
	test=BINgetshort(bin);
	if(test) matrix->xaxis.title2=BINgetstring(bin);
		else matrix->xaxis.title2=NULL;
	test=BINgetshort(bin);
	if(test) matrix->xaxis.desc=BINgetstring(bin);
		else matrix->xaxis.desc=NULL;
	matrix->xaxis.columns=BINgetint(bin);
	matrix->xaxis.print_title1=BINgetshort(bin);
	matrix->xaxis.print_title2=BINgetshort(bin);
	matrix->xaxis.axis_type=BINgetint(bin);
	if(matrix->xaxis.axis_type!=5)
	{
		matrix->xaxis.type=BINgetshort(bin);
		matrix->xaxis.len=BINgetshort(bin);
	} else {
		matrix->xaxis.type=(-1);
		matrix->xaxis.len=0;
	}
	matrix->xaxis.contype=BINgetint(bin);
	test=BINgetshort(bin);
	if(test) matrix->xaxis.conresource=BINgetstring(bin);
		else matrix->xaxis.conresource=NULL;
	matrix->xaxis.num=BINgetint(bin);
	if(matrix->xaxis.num>0)
	{
		matrix->xaxis.mem=Rmalloc(matrix->xaxis.num*sizeof(AXISmem));
		for(x=0,am=matrix->xaxis.mem;x<matrix->xaxis.num;++x,++am)
		{
			test=BINgetshort(bin);
			if(test) am->title1=BINgetstring(bin);
				else am->title1=NULL;
			test=BINgetshort(bin);
			if(test) am->title2=BINgetstring(bin);
				else am->title2=NULL;
			if(matrix->xaxis.axis_type!=5)
			{
				switch(matrix->xaxis.type)
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
						test=BINgetshort(bin);
						if(test) 
						{
							am->data.string_value=BINgetstring(bin);
							am->dlen=RDAstrlen(am->data.string_value)+1;
						} else {
							am->data.string_value=NULL;
							am->dlen=0;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						am->data.string_value=Rmalloc(1);
						am->dlen=1;
						*am->data.string_value=BINgetshort(bin);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						am->data.float_value=Rmalloc(sizeof(double));
						am->dlen=sizeof(double);
						*am->data.float_value=BINgetdouble(bin);
						break;
					case SHORTV:
					case SSHORTV:
						am->data.short_value=Rmalloc(sizeof(short));
						am->dlen=sizeof(short);
						*am->data.short_value=BINgetshort(bin);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						am->data.integer_value=Rmalloc(sizeof(int));
						am->dlen=sizeof(int);
						*am->data.integer_value=BINgetint(bin);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%s] invalid for Connect Resource [%s] at line [%d] program [%s].",matrix->xaxis.type,matrix->xaxis.conresource,line,file);
						break;
				}
			} else {
				am->data.string_value=NULL;
				am->data.float_value=NULL;
				am->data.integer_value=NULL;
				am->data.short_value=NULL;
				am->dlen=0;
			}
		}
	} else {
		matrix->xaxis.mem=NULL;
		matrix->xaxis.num=0;
	}
	matrix->numdefs=BINgetint(bin);
	if(matrix->numdefs>0)
	{
		matrix->defs=Rmalloc(matrix->numdefs*sizeof(MATRIXcell));
		for(x=0,cell=matrix->defs;x<matrix->numdefs;++x,++cell)
		{
			test=BINgetshort(bin);
			if(test) cell->name=BINgetstring(bin);
				else cell->name=NULL;
			cell->type=BINgetshort(bin);
			cell->len=BINgetshort(bin);
			cell->displayed=BINgetshort(bin);
		}
	} else matrix->defs=NULL;
	if(matrix->defs!=NULL)
	{
		matrix->num=BINgetint(bin);
		if(matrix->num>0)
		{
			matrix->recs=Rmalloc(matrix->num*sizeof(MATRIXrec));
			for(x=0,rec=matrix->recs;x<matrix->num;++x,++rec)
			{
				rec->num=BINgetint(bin);
				if(rec->num>0)
				{
					rec->mems=Rmalloc(rec->num*sizeof(MATRIXmem));
					for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
					{
						mem->sensitive=BINgetshort(bin);
						mem->num=BINgetint(bin);
						if(mem->num>0)
						{
							mem->values=Rmalloc(mem->num*sizeof(MATRIXvalue));
							for(z=0,v=mem->values;z<mem->num;++z,++v)
							{
								cell=matrix->defs+z;
								switch(cell->type)
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
										test=BINgetshort(bin);
										if(test) 
										{
											v->data.string_value=
												BINgetstring(bin);
											v->dlen=RDAstrlen(v->data.string_value)+1;
										} else v->data.string_value=NULL;
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										v->data.float_value=Rmalloc(sizeof(double));
										*v->data.float_value=BINgetdouble(bin);
										v->dlen=sizeof(double);
										break;
									case CHARACTERS:
									case BOOLNS:
										v->data.string_value=Rmalloc(1);
										*v->data.string_value=BINgetshort(bin);
										v->dlen=1;
										break;
									case SHORTV:
									case SSHORTV:
										v->data.short_value=Rmalloc(sizeof(short));
										*v->data.short_value=BINgetshort(bin);
										v->dlen=sizeof(short);
										break;
									case LONGV:
									case SLONGV:
										v->data.integer_value=Rmalloc(sizeof(int));
										*v->data.integer_value=BINgetint(bin);
										v->dlen=sizeof(int);
										break;
									default:
										prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",cell->type,cell->name,line,file);
										break;
								}
							}
						} else mem->values=NULL;
					}
				} else rec->mems=NULL;
			}
		} else matrix->recs=NULL;
	}
	BINfree(bin);
	return(0);
}
short xwriteRDAMatrixbin(char *libname,RDAMATRIX *matrix,int line,char *file)
{
	BIN *bin;
	DFincvir *i;
	MATRIXrec *rec;
	MATRIXcell *cell;
	MATRIXmem *mem;
	MATRIXvalue *v;
	AXISmem *am=NULL;
	int x,y,z;
	short sv=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG writeRDAMatrixbin Writing RDAMatrix [%s] into RDAMatrix Binary Library [%s] at line [%d] program [%s].",(matrix->name!=NULL ? matrix->name:""),libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1701);	/*type identifier*/
	if(!isEMPTY(matrix->module))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->module);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->name);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->title1))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->title1);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->title2))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->title2);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,matrix->display_cell);
	BINaddint(bin,matrix->matrix_buttons);
	BINaddint(bin,matrix->cell_config);
	BINaddint(bin,matrix->locate_method);
	BINaddint(bin,matrix->which);
	if(matrix->incvir!=NULL)
	{
		BINaddint(bin,matrix->numinc);
		for(x=0,i=matrix->incvir;x<matrix->numinc;++x,++i)
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
	} else BINaddint(bin,0);
	if(!isEMPTY(matrix->yaxis.title1))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->yaxis.title1);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->yaxis.title2))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->yaxis.title2);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->yaxis.desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->yaxis.desc);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,matrix->yaxis.columns);
	BINaddshort(bin,matrix->yaxis.print_title1);
	BINaddshort(bin,matrix->yaxis.print_title2);
	BINaddint(bin,matrix->yaxis.axis_type);
	if(matrix->yaxis.axis_type!=5)
	{
		BINaddshort(bin,matrix->yaxis.type);
		sv=matrix->yaxis.len;
		BINaddshort(bin,sv);
	}
	BINaddint(bin,matrix->yaxis.contype);
	if(!isEMPTY(matrix->yaxis.conresource))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->yaxis.conresource);
	} else BINaddshort(bin,FALSE);
	if(matrix->yaxis.mem!=NULL)
	{
		BINaddint(bin,matrix->yaxis.num);
		for(x=0,am=matrix->yaxis.mem;x<matrix->yaxis.num;++x,++am)
		{
			if(!isEMPTY(am->title1))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,am->title1);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(am->title2))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,am->title2);
			} else BINaddshort(bin,FALSE);
			if(matrix->yaxis.axis_type!=5)
			{
			switch(matrix->yaxis.type)
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
					if(!isEMPTY(am->data.string_value))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,am->data.string_value);
					} else BINaddshort(bin,FALSE);
					break;
				case BOOLNS:
				case CHARACTERS:
					BINaddshort(bin,*am->data.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					BINadddouble(bin,*am->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					BINaddshort(bin,*am->data.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					BINaddint(bin,*am->data.integer_value);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%d] invalid for Connect Resource [%s] at line [%d] program [%s].",matrix->yaxis.type,matrix->yaxis.conresource,line,file);
					break;
			}
			}
		}
	} else {
		BINaddint(bin,0);
	}
	if(!isEMPTY(matrix->xaxis.title1))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->xaxis.title1);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->xaxis.title2))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->xaxis.title2);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(matrix->xaxis.desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->xaxis.desc);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,matrix->xaxis.columns);
	BINaddshort(bin,matrix->xaxis.print_title1);
	BINaddshort(bin,matrix->xaxis.print_title2);
	BINaddint(bin,matrix->xaxis.axis_type);
	if(matrix->xaxis.axis_type!=5)
	{
		BINaddshort(bin,matrix->xaxis.type);
		sv=matrix->xaxis.len;
		BINaddshort(bin,sv);
	}
	BINaddint(bin,matrix->xaxis.contype);
	if(!isEMPTY(matrix->xaxis.conresource))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,matrix->xaxis.conresource);
	} else BINaddshort(bin,FALSE);
	if(matrix->xaxis.mem!=NULL)
	{
		BINaddint(bin,matrix->xaxis.num);
		for(x=0,am=matrix->xaxis.mem;x<matrix->xaxis.num;++x,++am)
		{
			if(!isEMPTY(am->title1))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,am->title1);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(am->title2))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,am->title2);
			} else BINaddshort(bin,FALSE);
			if(matrix->xaxis.axis_type!=5)
			{
			switch(matrix->xaxis.type)
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
					if(!isEMPTY(am->data.string_value))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,am->data.string_value);
					} else BINaddshort(bin,FALSE);
					break;
				case BOOLNS:
				case CHARACTERS:
					BINaddshort(bin,*am->data.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					BINadddouble(bin,*am->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					BINaddshort(bin,*am->data.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					BINaddint(bin,*am->data.integer_value);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%s] invalid for Connect Resource [%s] at line [%d] program [%s].",matrix->xaxis.type,matrix->xaxis.conresource,line,file);
					break;
			}
			}
		}
	} else {
		BINaddint(bin,0);
	}
	if(matrix->defs!=NULL)
	{
		BINaddint(bin,matrix->numdefs);
		for(x=0,cell=matrix->defs;x<matrix->numdefs;++x,++cell)
		{
			if(!isEMPTY(cell->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,cell->name);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,cell->type);
			BINaddshort(bin,cell->len);
			BINaddshort(bin,cell->displayed);
		}
	} else BINaddint(bin,0);
	if(matrix->recs!=NULL)
	{
		BINaddint(bin,matrix->num);
		for(x=0,rec=matrix->recs;x<matrix->num;++x,++rec)
		{
			if(rec->mems!=NULL)
			{
				BINaddint(bin,rec->num);
				for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
				{
					if(mem->values!=NULL)
					{
						BINaddshort(bin,mem->sensitive);
						BINaddint(bin,mem->num);
						for(z=0,v=mem->values;z<mem->num;++z,++v)
						{
							cell=matrix->defs+z;
							switch(cell->type)
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
									if(!isEMPTY(v->data.string_value))
									{
										BINaddshort(bin,TRUE);
										BINaddstring(bin,v->data.string_value);
									} else BINaddshort(bin,FALSE);
									break;
								case CHARACTERS:
								case BOOLNS:
									BINaddshort(bin,*v->data.string_value);
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									BINadddouble(bin,*v->data.float_value);
									break;
								case SHORTV:
								case SSHORTV:
									BINaddshort(bin,*v->data.short_value);
									break;
								case LONGV:
								case SLONGV:
									BINaddint(bin,*v->data.integer_value);
									break;
								default:
									prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",cell->type,cell->name,line,file);
									break;
							}
						}
					} else BINaddint(bin,0);
				}
			} else BINaddint(bin,0);
		}
	} else BINaddint(bin,0);
	if(writelibbin(libname,bin,matrix->name))
	{
		prterr("Error Can't write RDAMatrix binary [%s] into RDAMatrix library [%s] at line [%d] program [%s].",(matrix->name!=NULL ? matrix->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
short xgetRDAMatrixdesc(char *libname,char *name,char **desc,char **title1,
	char **title2,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG getRDAMatrixdesc Attempting to Read RDAMatrix description  for [%s] from RDAMatrix Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmatrix)
		{
			prterr("Error Attempting to Read RDAMatrix Binary [%s] from RDAMatrix Library [%s] Failed at line [%d] program [%s].",name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for RDAMatrix [%s] in RDAMatrix Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	if(*desc!=NULL) Rfree(*desc);
	test=BINgetshort(bin);
	if(test) *desc=BINgetstring(bin);
		else *desc=NULL;
	if(*title1!=NULL) Rfree(*title1);
	test=BINgetshort(bin);
	if(test) *title1=BINgetstring(bin);
		else *title1=NULL;
	if(*title2!=NULL) Rfree(*title2);
	test=BINgetshort(bin);
	if(test) *title2=BINgetstring(bin);
		else *title2=NULL;
	BINfree(bin);
	return(0);
}
RDAMATRIX *xGetInternalMatrix(char *module,char *name,int line,char *file)
{
	RDAMATRIX *m=NULL;
	char *libname=NULL;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG GetInternalMatrix [%s][%s] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(Internal_RDAMATRIXES!=NULL)
	{
		for(x=0,m=Internal_RDAMATRIXES;x<Internal_num_RDAMATRIXES;++x,++m)
		{
			if(!RDAstrcmp(m->module,module) && !RDAstrcmp(m->name,name)) break;
		}
		if(x<Internal_num_RDAMATRIXES)
		{
			return(m);
		} else {
			Internal_RDAMATRIXES=Rrealloc(Internal_RDAMATRIXES,
				(Internal_num_RDAMATRIXES+1)*sizeof(RDAMATRIX));
			m=Internal_RDAMATRIXES+Internal_num_RDAMATRIXES;
			if(!isEMPTY(module)) m->module=stralloc(module);
				else m->module=NULL;
			if(!isEMPTY(name)) m->name=stralloc(name);
				else m->name=NULL;
			m->title1=NULL;
			m->title2=NULL;
			m->desc=NULL;
			m->matrix_buttons=0;
			m->cell_config=0;
			m->locate_method=0;
			m->yaxis.title1=NULL;
			m->yaxis.title2=NULL;
			m->yaxis.desc=NULL;
			m->yaxis.columns=0;
			m->yaxis.axis_type=5;
			m->yaxis.contype=0;
			m->yaxis.conresource=NULL;
			m->yaxis.type=(-1);
			m->yaxis.len=0;
			m->yaxis.num=0;
			m->yaxis.mem=NULL;
			m->yaxis.print_title1=TRUE;
			m->yaxis.print_title2=TRUE;
			m->xaxis.title1=NULL;
			m->xaxis.title2=NULL;
			m->xaxis.desc=NULL;
			m->xaxis.columns=0;
			m->xaxis.axis_type=5;
			m->xaxis.type=(-1);
			m->xaxis.len=0;
			m->xaxis.print_title1=TRUE;
			m->xaxis.print_title2=TRUE;
			m->xaxis.contype=0;
			m->xaxis.conresource=NULL;
			m->xaxis.num=0;
			m->xaxis.mem=NULL;
			m->numdefs=0;
			m->defs=NULL;
			m->num=0;
			m->recs=NULL;
			m->which=0;
			m->numinc=0;
			m->incvir=NULL;
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
			sprintf(libname,"%s/rda/%s.MTX",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
			sprintf(libname,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,module);
#endif
			if(getRDAMatrixbin(libname,m)) m=NULL;
			if(libname!=NULL) Rfree(libname);
			++Internal_num_RDAMATRIXES;
		}
	} else {
		Internal_RDAMATRIXES=RDAMATRIXNEW(module,name);
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.MTX",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,module);
#endif
		if(!getRDAMatrixbin(libname,Internal_RDAMATRIXES))
		{
			m=Internal_RDAMATRIXES;
		} else m=NULL;
		if(libname!=NULL) Rfree(libname);
		Internal_num_RDAMATRIXES=1;
	}
	return(m);
}
void xFree_Internal_RDAMATRIXES(int line,char *file)
{
	RDAMATRIX *m;
	DFincvir *i;
	MATRIXrec *rec;
	MATRIXcell *cell;
	MATRIXmem *mem;
	MATRIXvalue *v;
	AXISmem *am=NULL;
	int x,y,z,a;


#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG Free_Internal_RDAMATRIXES at line [%d] program [%s].",
			line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(Internal_RDAMATRIXES!=NULL)
	{
		for(a=0,m=Internal_RDAMATRIXES;a<Internal_num_RDAMATRIXES;++a,++m)
		{
			if(m!=NULL)
			{
				if(m->module!=NULL) Rfree(m->module);
				if(m->name!=NULL) Rfree(m->name);
				if(m->title1!=NULL) Rfree(m->title1);
				if(m->title2!=NULL) Rfree(m->title2);
				if(m->desc!=NULL) Rfree(m->desc);
				if(m->yaxis.title1!=NULL) Rfree(m->yaxis.title1);
				if(m->yaxis.title2!=NULL) Rfree(m->yaxis.title2);
				if(m->yaxis.desc!=NULL) Rfree(m->yaxis.desc);
				if(m->yaxis.mem!=NULL)
				{
					for(x=0,am=m->yaxis.mem;x<m->yaxis.num;++x,++am)
					{
						if(am->title1!=NULL) Rfree(am->title1);
						if(am->title2!=NULL) Rfree(am->title2);
						if(m->yaxis.axis_type<5)
						{
							switch(m->yaxis.type)
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
									prterr("Error field type [%s] invalid for Connect Resource [%s] at line [%d] program [%s].",m->yaxis.type,m->yaxis.conresource,line,file);
									break;
							}
						}
					}
					Rfree(m->yaxis.mem);
				}
				if(m->yaxis.conresource!=NULL) Rfree(m->yaxis.conresource);
				if(m->xaxis.title1!=NULL) Rfree(m->xaxis.title1);
				if(m->xaxis.title2!=NULL) Rfree(m->xaxis.title2);
				if(m->xaxis.desc!=NULL) Rfree(m->xaxis.desc);
				if(m->xaxis.mem!=NULL)
				{
					for(x=0,am=m->xaxis.mem;x<m->xaxis.num;++x,++am)
					{
						if(am->title1!=NULL) Rfree(am->title1);
						if(am->title2!=NULL) Rfree(am->title2);
						if(m->xaxis.axis_type<5)
						{
							switch(m->xaxis.type)
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
									prterr("Error field type [%s] invalid for Connect Resource [%s] at line [%d] program [%s].",m->xaxis.type,m->xaxis.conresource,line,file);
									break;
							}
						}
					}
					Rfree(m->xaxis.mem);
				}
				if(m->xaxis.conresource!=NULL) Rfree(m->xaxis.conresource);
				if(m->incvir!=NULL)
				{
					for(x=0,i=m->incvir;x<m->numinc;++x,++i)
					{
						if(i->module!=NULL) Rfree(i->module);
						if(i->file!=NULL) Rfree(i->file);
						if(i->keyname!=NULL) Rfree(i->keyname);
					}
					Rfree(m->incvir);
				}
				if(m->recs!=NULL)
				{
					for(x=0,rec=m->recs;x<m->num;++x,++rec)
					{
						if(rec->mems!=NULL && m->defs!=NULL)
						{
							for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
							{
								if(mem->values!=NULL)
								{
									for(z=0,v=mem->values;z<mem->num;++z,++v)
									{
										cell=m->defs+z;
										switch(cell->type)
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
											case LONGV:
											case SLONGV:
												if(v->data.integer_value!=NULL) 
													Rfree(v->data.integer_value);
												break;
											default:
												prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",cell->type,cell->name,line,file);
												break;
										}
										v->dlen=0;
									}
									Rfree(mem->values);
								}
							}
							Rfree(rec->mems);
						}
					}
					Rfree(m->recs);
				}
				if(m->defs!=NULL)
				{
					for(x=0,cell=m->defs;x<m->numdefs;++x,++cell)
					{
						if(cell->name!=NULL) Rfree(cell->name);
					}
					Rfree(m->defs);
				}
			}
		}
		Rfree(Internal_RDAMATRIXES);
		Internal_RDAMATRIXES=NULL;
		Internal_num_RDAMATRIXES=0;
	}
}
