/* matrix1.c - RDAMatrix Compute Functions */
#include<sec.hpp>

MATRIX_RUNTIME *xMATRIX_RUNTIME_NEW(char *module,char *name,
	int line,char *file)
{
	MATRIX_RUNTIME *m=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG MATRIX_RUNTIME_NEW at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	m=Rmalloc(sizeof(MATRIX_RUNTIME));
	if(module!=NULL) m->module=stralloc(module);
		else m->module=NULL;
	if(name!=NULL) m->name=stralloc(name);
		else m->name=NULL;
	m->rangetype=(-1);
	m->rangename=NULL;
	m->from_col=NULL;
	m->to_col=NULL;
	m->from_row=NULL;
	m->to_row=NULL;
	m->data=NULL;
	m->m=GetInternalMatrix(m->module,m->name);
	if(m->m==NULL)
	{
		prterr("Error RDAMATRIX [%s][%s] not found.",m->module,m->name);
	}
	return(m);
}
void xFreeMATRIX_RUNTIME(MATRIX_RUNTIME *M,short freedata,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG FreeMATRIX_RUNTIME at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(M!=NULL)
	{
		if(M->module!=NULL) Rfree(M->module);
		if(M->name!=NULL) Rfree(M->name);
		if(M->rangename!=NULL) Rfree(M->rangename);
		if(M->from_row!=NULL) 
		{
			FreeNRDfield(M->from_row);
			Rfree(M->from_row);
		}
		if(M->to_row!=NULL)
		{
			FreeNRDfield(M->to_row);
			Rfree(M->to_row);
		}
		if(M->from_col!=NULL)
		{
			FreeNRDfield(M->from_col);
			Rfree(M->from_col);
		}
		if(M->to_col!=NULL)
		{
			FreeNRDfield(M->to_col);
			Rfree(M->to_col);
		}
		if(freedata)
		{
			if(M->data!=NULL) 
			{
				FreeNRDfield(M->data);
				Rfree(M->data);
			}
		}
		M->m=NULL;
		Rfree(M);
	}
}
NRDfield *xCOMPUTE_MATRIX_ROWCOLUMN(char *string,int line,char *file)
{
	MATRIX_RUNTIME *R=NULL;
	NRDfield *tmp=NULL;
	char *mod=NULL,*name=NULL,*temp=NULL,*s=NULL,doit=FALSE,okfrom=FALSE;
	char okto=FALSE;
	AXISmem *amx=NULL,*amy=NULL;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXvalue *v;
	MATRIXcell *c;
	int a,x,y,z,CELL_COUNTER=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG COMPUTE_MATRIX_ROWCOLUMN for [%s] at line [%d] program [%s].",(string!=NULL ? string:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	mod=stripmodulename(string);
	name=stripfilename(string);
	if(!isEMPTY(mod) && !isEMPTY(name))
	{
		R=MATRIX_RUNTIME_NEW(mod,name);
		if(R->m!=NULL)
		{
			temp=string+(RDAstrlen(mod)+RDAstrlen(name))+4;
			while((iswhite(*temp))) ++temp;
			if(!strncmp(temp,"SUMMATION",9))
			{
				R->rangetype=1;
				temp=temp+9;
			} else if(!strncmp(temp,"AVERAGE",7))
			{
				R->rangetype=2;
				temp=temp+7;
			} else if(!strncmp(temp,"MAXIMUM",7))
			{
				R->rangetype=3;
				temp=temp+7;
			} else if(!strncmp(temp,"MINIMUM",7))
			{
				R->rangetype=4;
				temp=temp+7;
			} else if(!strncmp(temp,"FREQUENCY",9))
			{
				R->rangetype=5;
				temp=temp+9;
			} else if(!strncmp(temp,"VALUE",5))
			{
				R->rangetype=0;
				temp=temp+5;
			} else {
				prterr("Error in MATRIX Implementation...Operator must be: SUMMATION, MAXIMUM, MINIMUM, AVERAGE, FREQUENCY or VALUE in [%s][%s].",mod,name);
				FreeMATRIX_RUNTIME(R);
				if(mod!=NULL) Rfree(mod);
				if(name!=NULL) Rfree(name);
				return(NULL);
			}
			if(R->rangetype!=(-1))
			{
				while((iswhite(*temp) && *temp!='\0')) ++temp;
/* POWER FUNCTIONS */
				if(R->rangetype>0)
				{
					for(a=0;a<4;++a)
					{
						while((iswhite(*temp) && *temp!='\0')) ++temp;
						if(a==2)
						{
							if(!strncmp(temp,"TO",2) ||
								!strncmp(temp,"to",2))
							{
								temp=temp+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							}
						}
						s=stripmodulename(temp);
						if(!RDAstrcmp(s,"Y-AXIS VALUE"))
						{
							if(a<2)
							{
								if(R->from_row!=NULL)
								{
									FreeNRDfield(R->from_row);
									Rfree(R->from_row);
								}
							} else {
								if(R->to_row!=NULL)
								{
									FreeNRDfield(R->to_row);
									Rfree(R->to_row);
								}
							}
							temp=temp+(RDAstrlen(s)+2);
							while((iswhite(*temp) && *temp!='\0')) ++temp;
							if(s!=NULL) Rfree(s);
							if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
							{
								s=stripmodulename(temp);
								switch(R->m->yaxis.type)
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
										if(a<2)
										{
											R->from_row=Rmalloc(sizeof(NRDfield));
											R->from_row->name=stralloc("Y-AXIS VALUE");
											R->from_row->type=R->m->yaxis.type;
											R->from_row->len=R->m->yaxis.len;
											if(s!=NULL)
											{
												R->from_row->data.string_value=stralloc(s);
												R->from_row->dlen=RDAstrlen(s)+1;
												temp=temp+RDAstrlen(s)+2;
											} else {
												R->from_row->data.string_value=NULL;
												R->from_row->dlen=0;
												temp=temp+2;
											}
										} else {
											R->to_row=Rmalloc(sizeof(NRDfield));
											R->to_row->name=stralloc("Y-AXIS VALUE");
											R->to_row->type=R->m->yaxis.type;
											R->to_row->len=R->m->yaxis.len;
											if(s!=NULL)
											{
												R->to_row->data.string_value=stralloc(s);
												R->to_row->dlen=RDAstrlen(s)+1;
												temp=temp+RDAstrlen(s)+2;
											} else {
												R->to_row->data.string_value=NULL;
												R->to_row->dlen=0;
												temp=temp+2;
											}
										}
										break;
									case BOOLNS:
										if(a<2)
										{
											R->from_row=Rmalloc(sizeof(NRDfield));
											R->from_row->name=stralloc("Y-AXIS VALUE");
											R->from_row->type=R->m->yaxis.type;
											R->from_row->len=1;
											R->from_row->data.string_value=Rmalloc(1);
											*R->from_row->data.string_value=FALSE;
											if(!isEMPTY(s))
											{
												if(!strncmp(s,"TRUE",4) ||
													!strncmp(s,"True",4) ||
													!strncmp(s,"Yes",3) ||
													!strncmp(s,"YES",3) ||
													!RDAstrcmp(s,"Y") || !RDAstrcmp(s,"T")
													|| !RDAstrcmp(s,"1"))
												{
													*R->from_row->data.string_value=TRUE;
												}
											}
											R->from_row->dlen=1;
										} else {
											R->to_row=Rmalloc(sizeof(NRDfield));
											R->to_row->name=stralloc("Y-AXIS VALUE");
											R->to_row->type=R->m->yaxis.type;
											R->to_row->len=1;
											R->to_row->data.string_value=Rmalloc(1);
											*R->to_row->data.string_value=FALSE;
											if(!isEMPTY(s))
											{
												if(!strncmp(s,"TRUE",4) ||
													!strncmp(s,"True",4) ||
													!strncmp(s,"Yes",3) ||
													!strncmp(s,"YES",3) ||
													!RDAstrcmp(s,"Y") || !RDAstrcmp(s,"T")
													|| !RDAstrcmp(s,"1"))
												{
													*R->to_row->data.string_value=TRUE;
												}
											}
											R->to_row->dlen=1;
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case CHARACTERS:
										if(a<2)
										{
											R->from_row=Rmalloc(sizeof(NRDfield));
											R->from_row->name=stralloc("Y-AXIS VALUE");
											R->from_row->type=R->m->yaxis.type;
											R->from_row->len=1;
											R->from_row->data.string_value=Rmalloc(1);
/*lint -e524*/
											*R->from_row->data.string_value=*s;
/*lint +e524*/
											R->from_row->dlen=1;
										} else {
											R->to_row=Rmalloc(sizeof(NRDfield));
											R->to_row->name=stralloc("Y-AXIS VALUE");
											R->to_row->type=R->m->yaxis.type;
											R->to_row->len=1;
											R->to_row->data.string_value=Rmalloc(1);
/*lint -e524*/
											*R->to_row->data.string_value=*s;
/*lint +e524*/
											R->to_row->dlen=1;
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(a<2)
										{
											R->from_row=Rmalloc(sizeof(NRDfield));
											R->from_row->name=stralloc("Y-AXIS VALUE");
											R->from_row->type=R->m->yaxis.type;
											R->from_row->len=R->m->yaxis.len;
											R->from_row->data.float_value=
												Rmalloc(sizeof(double));
											if(!isEMPTY(s))
											{
											*R->from_row->data.float_value=atof(s);
											} else *R->from_row->data.float_value=0;
											R->from_row->dlen=sizeof(double);
										} else {
											R->to_row=Rmalloc(sizeof(NRDfield));
											R->to_row->name=stralloc("Y-AXIS VALUE");
											R->to_row->type=R->m->yaxis.type;
											R->to_row->len=R->m->yaxis.len;
											R->to_row->data.float_value=
												Rmalloc(sizeof(double));
											if(!isEMPTY(s))
											{
											*R->to_row->data.float_value=atof(s);
											} else *R->to_row->data.float_value=0;
											R->to_row->dlen=sizeof(double);
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case SHORTV:
									case SSHORTV:
										if(a<2)
										{
											R->from_row=Rmalloc(sizeof(NRDfield));
											R->from_row->name=stralloc("Y-AXIS VALUE");
											R->from_row->type=R->m->yaxis.type;
											R->from_row->len=R->m->yaxis.len;
											R->from_row->data.short_value=
													Rmalloc(sizeof(short));
/*lint -e524*/
											if(!isEMPTY(s)) 
											{
											*R->from_row->data.short_value=Ratoi(s);
											} else {
											*R->from_row->data.short_value=0;
											}
/*lint +e524*/
											R->from_row->dlen=sizeof(short);
										} else {
											R->to_row=Rmalloc(sizeof(NRDfield));
											R->to_row->name=stralloc("Y-AXIS VALUE");
											R->to_row->type=R->m->yaxis.type;
											R->to_row->len=R->m->yaxis.len;
											R->to_row->data.short_value=
													Rmalloc(sizeof(short));
											if(!isEMPTY(s))
											{
/*lint -e524*/
											*R->to_row->data.short_value=Ratoi(s);
/*lint +e524*/
											} else {
											*R->to_row->data.short_value=0;
											}
											R->to_row->dlen=sizeof(short);
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(a<2)
										{
											R->from_row=Rmalloc(sizeof(NRDfield));
											R->from_row->name=stralloc("Y-AXIS VALUE");
											R->from_row->type=R->m->yaxis.type;
											R->from_row->len=R->m->yaxis.len;
											R->from_row->data.integer_value=
												Rmalloc(sizeof(int));
											if(!isEMPTY(s))
											{
											*R->from_row->data.integer_value=Ratoi(s);
											} else {
											*R->from_row->data.integer_value=0;
											}
											R->from_row->dlen=sizeof(int);
										} else {
											R->to_row=Rmalloc(sizeof(NRDfield));
											R->to_row->name=stralloc("Y-AXIS VALUE");
											R->to_row->type=R->m->yaxis.type;
											R->to_row->len=R->m->yaxis.len;
											R->to_row->data.integer_value=
												Rmalloc(sizeof(int));
											if(!isEMPTY(s))
											{
											*R->to_row->data.integer_value=Ratoi(s);
											} else {
											*R->to_row->data.integer_value=0;
											}
											R->to_row->dlen=sizeof(int);
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
									default:
										prterr("Error field type [%d] invalid on Y-AXIS VALUE of [%s][%s] at line [%d] program [%s].",R->m->yaxis.type,mod,name,line,file);
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
							} else {
								prterr("Error in MATRIX Implementation... Expecting a Y-AXIS VALUE in [%s][%s]",mod,name);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"Y-AXIS TITLE 1"))
						{
							if(a<2)
							{
								if(R->from_row!=NULL)
								{
									FreeNRDfield(R->from_row);
									Rfree(R->from_row);
								}
								R->from_row=Rmalloc(sizeof(NRDfield));
								R->from_row->name=stralloc("Y-AXIS TITLE 1");
								R->from_row->type=0;
								R->from_row->len=0;
								R->from_row->dlen=0;
							} else {
								if(R->to_row!=NULL)
								{
									FreeNRDfield(R->to_row);
									Rfree(R->to_row);
								}
								R->to_row=Rmalloc(sizeof(NRDfield));
								R->to_row->name=stralloc("Y-AXIS TITLE 1");
								R->to_row->type=0;
								R->to_row->len=0;
								R->to_row->dlen=0;
							}
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									if(a<2)
									{
										R->from_row->data.string_value=stralloc(s);
										R->from_row->dlen=
											RDAstrlen(R->from_row->data.string_value);
									} else {
										R->to_row->data.string_value=stralloc(s);
										R->to_row->dlen=
											RDAstrlen(R->to_row->data.string_value);
									}
								} else {
									if(a<2)
									{
										R->from_row->data.string_value=NULL; 
									} else {
										R->to_row->data.string_value=NULL;
									}
								}
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation..... Expecting a Y-AXIS TITLE 1 in [%s][%s]",mod,name);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"Y-AXIS TITLE 2"))
						{
							if(a<2)
							{
								if(R->from_row!=NULL)
								{
									FreeNRDfield(R->from_row);
									Rfree(R->from_row);
								}
								R->from_row=Rmalloc(sizeof(NRDfield));
								R->from_row->name=stralloc("Y-AXIS TITLE 2");
								R->from_row->type=0;
								R->from_row->len=0;
								R->from_row->dlen=0;
							} else {
								if(R->to_row!=NULL)
								{
									FreeNRDfield(R->to_row);
									Rfree(R->to_row);
								}
								R->to_row=Rmalloc(sizeof(NRDfield));
								R->to_row->name=stralloc("Y-AXIS TITLE 2");
								R->to_row->type=0;
								R->to_row->len=0;
								R->to_row->dlen=0;
							}
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									if(a<2)
									{
										R->from_row->data.string_value=stralloc(s);
										R->from_row->dlen=
											RDAstrlen(R->from_row->data.string_value);
									} else {
										R->to_row->data.string_value=stralloc(s);
										R->to_row->dlen=
											RDAstrlen(R->to_row->data.string_value);
									}
								} else {
									if(a<2)
									{
										R->from_row->data.string_value=NULL; 
									} else {
										R->to_row->data.string_value=NULL; 
									}
								}
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation..... Expecting a Y-AXIS TITLE 2 in [%s][%s]",mod,name);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"X-AXIS VALUE"))
						{
							if(a<2)
							{
								if(R->from_col!=NULL)
								{
									FreeNRDfield(R->from_col);
									Rfree(R->from_col);
								}
							} else {
								if(R->to_col!=NULL)
								{
									FreeNRDfield(R->to_col);
									Rfree(R->to_col);
								}
							}
							temp=temp+(RDAstrlen(s)+2);
							while((iswhite(*temp) && *temp!='\0')) ++temp;
							if(s!=NULL) Rfree(s);
							if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
							{
								s=stripmodulename(temp);
								switch(R->m->xaxis.type)
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
										if(a<2)
										{
											R->from_col=Rmalloc(sizeof(NRDfield));
											R->from_col->name=stralloc("X-AXIS VALUE");
											R->from_col->type=R->m->xaxis.type;
											R->from_col->len=R->m->xaxis.len;
											if(s!=NULL)
											{
												R->from_col->data.string_value=stralloc(s);
												R->from_col->dlen=RDAstrlen(s)+1;
												temp=temp+RDAstrlen(s)+2;
											} else {
												R->from_col->data.string_value=NULL;
												R->from_col->dlen=0;
												temp=temp+2;
											}
										} else {
											R->to_col=Rmalloc(sizeof(NRDfield));
											R->to_col->name=stralloc("X-AXIS VALUE");
											R->to_col->type=R->m->xaxis.type;
											R->to_col->len=R->m->xaxis.len;
											if(s!=NULL)
											{
												R->to_col->data.string_value=stralloc(s);
												R->to_col->dlen=RDAstrlen(s)+1;
												temp=temp+RDAstrlen(s)+2;
											} else {
												R->to_col->data.string_value=NULL;
												R->to_col->dlen=0;
												temp=temp+2;
											}
										}
										break;
									case BOOLNS:
										if(a<2)
										{
											R->from_col=Rmalloc(sizeof(NRDfield));
											R->from_col->name=stralloc("X-AXIS VALUE");
											R->from_col->type=R->m->xaxis.type;
											R->from_col->len=1;
											R->from_col->data.string_value=Rmalloc(1);
											*R->from_col->data.string_value=FALSE;
											if(!isEMPTY(s))
											{
												if(!strncmp(s,"TRUE",4) ||
													!strncmp(s,"True",4) ||
													!strncmp(s,"Yes",3) ||
													!strncmp(s,"YES",3) ||
													!RDAstrcmp(s,"Y") || !RDAstrcmp(s,"T")
													|| !RDAstrcmp(s,"1"))
												{
													*R->from_col->data.string_value=TRUE;
												}
											}
											R->from_col->dlen=1;
										} else {
											R->to_col=Rmalloc(sizeof(NRDfield));
											R->to_col->name=stralloc("X-AXIS VALUE");
											R->to_col->type=R->m->xaxis.type;
											R->to_col->len=1;
											R->to_col->data.string_value=Rmalloc(1);
											*R->to_col->data.string_value=FALSE;
											if(!isEMPTY(s))
											{
												if(!strncmp(s,"TRUE",4) ||
													!strncmp(s,"True",4) ||
													!strncmp(s,"Yes",3) ||
													!strncmp(s,"YES",3) ||
													!RDAstrcmp(s,"Y") || !RDAstrcmp(s,"T")
													|| !RDAstrcmp(s,"1"))
												{
													*R->to_col->data.string_value=TRUE;
												}
											}
											R->to_col->dlen=1;
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case CHARACTERS:
										if(a<2)
										{
											R->from_col=Rmalloc(sizeof(NRDfield));
											R->from_col->name=stralloc("X-AXIS VALUE");
											R->from_col->type=R->m->xaxis.type;
											R->from_col->len=1;
											R->from_col->data.string_value=Rmalloc(1);
											*R->from_col->data.string_value=*s;
											R->from_col->dlen=1;
										} else {
											R->to_col=Rmalloc(sizeof(NRDfield));
											R->to_col->name=stralloc("X-AXIS VALUE");
											R->to_col->type=R->m->xaxis.type;
											R->to_col->len=1;
											R->to_col->data.string_value=Rmalloc(1);
											*R->to_col->data.string_value=*s;
											R->to_col->dlen=1;
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(a<2)
										{
											R->from_col=Rmalloc(sizeof(NRDfield));
											R->from_col->name=stralloc("X-AXIS VALUE");
											R->from_col->type=R->m->xaxis.type;
											R->from_col->len=R->m->xaxis.len;
											R->from_col->data.float_value=
												Rmalloc(sizeof(double));
											if(!isEMPTY(s))
											{
											*R->from_col->data.float_value=atof(s);
											} else *R->from_col->data.float_value=0;
											R->from_col->dlen=sizeof(double);
										} else {
											R->to_col=Rmalloc(sizeof(NRDfield));
											R->to_col->name=stralloc("X-AXIS VALUE");
											R->to_col->type=R->m->xaxis.type;
											R->to_col->len=R->m->xaxis.len;
											R->to_col->data.float_value=
												Rmalloc(sizeof(double));
											if(!isEMPTY(s))
											{
											*R->to_col->data.float_value=atof(s);
											} else *R->to_col->data.float_value=0;
											R->to_col->dlen=sizeof(double);
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case SHORTV:
									case SSHORTV:
										if(a<2)
										{
											R->from_col=Rmalloc(sizeof(NRDfield));
											R->from_col->name=stralloc("X-AXIS VALUE");
											R->from_col->type=R->m->xaxis.type;
											R->from_col->len=R->m->xaxis.len;
											R->from_col->data.short_value=
												Rmalloc(sizeof(short));
											if(!isEMPTY(s))
											{
/*lint -e524*/
											*R->from_col->data.short_value=Ratoi(s);
/*lint +e524*/
											} else {
											*R->from_col->data.short_value=0;
											} 
											R->from_col->dlen=sizeof(short);
										} else {
											R->to_col=Rmalloc(sizeof(NRDfield));
											R->to_col->name=stralloc("X-AXIS VALUE");
											R->to_col->type=R->m->xaxis.type;
											R->to_col->len=R->m->xaxis.len;
											R->to_col->data.short_value=
												Rmalloc(sizeof(short));
											if(!isEMPTY(s))
											{
/*lint -e524*/
											*R->to_col->data.short_value=Ratoi(s);
/*lint +e524*/
											} else {
											*R->to_col->data.short_value=0;
											} 
											R->to_col->dlen=sizeof(short);
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(a<2)
										{
											R->from_col=Rmalloc(sizeof(NRDfield));
											R->from_col->name=stralloc("X-AXIS VALUE");
											R->from_col->type=R->m->xaxis.type;
											R->from_col->len=R->m->xaxis.len;
											R->from_col->data.integer_value=
												Rmalloc(sizeof(int));
											if(!isEMPTY(s))
											{
											*R->from_col->data.integer_value=Ratoi(s);
											} else {
											*R->from_col->data.integer_value=0;
											}
											R->from_col->dlen=sizeof(int);
										} else {
											R->to_col=Rmalloc(sizeof(NRDfield));
											R->to_col->name=stralloc("X-AXIS VALUE");
											R->to_col->type=R->m->xaxis.type;
											R->to_col->len=R->m->xaxis.len;
											R->to_col->data.integer_value=
												Rmalloc(sizeof(int));
											if(!isEMPTY(s))
											{
											*R->to_col->data.integer_value=Ratoi(s);
											} else {
											*R->to_col->data.integer_value=0;
											}
											R->to_col->dlen=sizeof(int);
										}
										temp=temp+RDAstrlen(s)+2;
										break;
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
									default:
										prterr("Error field type [%d] invalid for X-AXIS VALUE on [%s][%s] at line [%d] program [%s].",R->m->xaxis.type,mod,name,line,file);
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
							} else {
								prterr("Error in MATRIX Implementation..... Expecting a X-AXIS VALUE for [%s][%s].",mod,name);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"X-AXIS TITLE 1"))
						{
							if(a<2)
							{
								if(R->from_col!=NULL)
								{
									FreeNRDfield(R->from_col);
									Rfree(R->from_col);
								}
								R->from_col=Rmalloc(sizeof(NRDfield));
								R->from_col->name=stralloc("X-AXIS TITLE 1");
								R->from_col->type=0;
								R->from_col->len=0;
								R->from_col->dlen=0;
							} else {
								if(R->to_col!=NULL)
								{
									FreeNRDfield(R->to_col);
									Rfree(R->to_col);
								}
								R->to_col=Rmalloc(sizeof(NRDfield));
								R->to_col->name=stralloc("X-AXIS TITLE 1");
								R->to_col->type=0;
								R->to_col->len=0;
								R->to_col->dlen=0;
							}
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									if(a<2)
									{
										R->from_col->data.string_value=stralloc(s);
										R->from_col->dlen=
											RDAstrlen(R->from_col->data.string_value);
									} else {
										R->to_col->data.string_value=stralloc(s);
										R->to_col->dlen=
											RDAstrlen(R->to_col->data.string_value);
									}
								} else {
									if(a<2)
									{
										R->from_col->data.string_value=NULL; 
									} else {
										R->to_col->data.string_value=NULL; 
									}
								}
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation..... Expecting a value for X-AXIS TITLE 1 on [%s][%s]",mod,name);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"X-AXIS TITLE 2"))
						{
							if(a<2)
							{
								if(R->from_col!=NULL)
								{
									FreeNRDfield(R->from_col);
									Rfree(R->from_col);
								}
								R->from_col=Rmalloc(sizeof(NRDfield));
								R->from_col->name=stralloc("X-AXIS TITLE 2");
								R->from_col->type=0;
								R->from_col->len=0;
								R->from_col->dlen=0;
							} else {
								if(R->to_col!=NULL)
								{
									FreeNRDfield(R->to_col);
									Rfree(R->to_col);
								}
								R->to_col=Rmalloc(sizeof(NRDfield));
								R->to_col->name=stralloc("X-AXIS TITLE 2");
								R->to_col->type=0;
								R->to_col->len=0;
								R->to_col->dlen=0;
							}
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									if(a<2)
									{
										R->from_col->data.string_value=stralloc(s);
										R->from_col->dlen=
											RDAstrlen(R->from_col->data.string_value);
									} else {
										R->to_col->data.string_value=stralloc(s);
										R->to_col->dlen=
											RDAstrlen(R->to_col->data.string_value);
									}
								} else {
									if(a<2)
									{
										R->from_col->data.string_value=NULL; 
									} else {
										R->to_col->data.string_value=NULL; 
									}
								}
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation..... Expecting a Value on X-AXIS TITLE 2 for [%s][%s]",mod,name);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else {
							prterr("Error in MATRIX Implementation..... Control Value [%s] not found for [%s][%s]",s,mod,name);
							FreeMATRIX_RUNTIME(R);
							if(mod!=NULL) Rfree(mod);
							if(name!=NULL) Rfree(name);
							if(s!=NULL) Rfree(s);
							return(NULL);
						}
					}
					if(s!=NULL) Rfree(s);
				}
				while((iswhite(*temp) && *temp!='\0')) ++temp;
				if(!strncmp(temp,"RETURN_VALUE",12) ||
					!strncmp(temp,"return_value",12))
				{
					temp=temp+12;
					while((iswhite(*temp) && *temp!='\0')) ++temp;
					if(*temp=='=') ++temp;
					if(s!=NULL) Rfree(s);
					s=stripmodulename(temp);
					for(a=0,c=R->m->defs;a<R->m->numdefs;++a,++c)
					{
						if(!RDAstrcmp(s,c->name)) break;
					}
					if(a<R->m->numdefs)
					{
						R->data=Rmalloc(sizeof(NRDfield));
						R->data->name=stralloc(s);
						R->data->type=c->type;
						R->data->len=c->len;
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
								switch(R->rangetype)
								{
									case 1: /* SUMMATION */
									case 2: /* AVERAGE */
									case 3: /* MINIMUM */
									case 4: /* MAXIMUM */
										R->data->data.string_value=NULL;
										R->data->dlen=0;
										break;
									case 5: /* FREQUENCY */
										R->data->type=DOUBLEV;
										R->data->len=20;
										R->data->data.float_value=Rmalloc(sizeof(double));
										R->data->dlen=sizeof(double);
										break;
									default:
									case 0: /* VALUE */
										prterr("Error How did it get here....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								R->data->type=SDOUBLEV;
								R->data->len=20;
								switch(R->rangetype)
								{
									case 1: /* SUMMATION */
									case 2: /* AVERAGE */
									case 3: /* MINIMUM */
									case 4: /* MAXIMUM */
									case 5: /* FREQUENCY */
										R->data->type=DOUBLEV;
										R->data->len=20;
										R->data->data.float_value=Rmalloc(sizeof(double));
										R->data->dlen=sizeof(double);
										break;
									default:
									case 0: /* VALUE */
										prterr("Error How did it get here....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								R->data->type=SDOUBLEV;
								R->data->len=20;
								switch(R->rangetype)
								{
									case 1: /* SUMMATION */
									case 2: /* AVERAGE */
									case 3: /* MINIMUM */
									case 4: /* MAXIMUM */
									case 5: /* FREQUENCY */
										R->data->type=DOUBLEV;
										R->data->len=20;
										R->data->data.float_value=Rmalloc(sizeof(double));
										R->data->dlen=sizeof(double);
										break;
									default:
									case 0: /* VALUE */
										prterr("Error How did it get here....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
								break;
							case SHORTV:
							case SSHORTV:
								R->data->type=SDOUBLEV;
								R->data->len=20;
								switch(R->rangetype)
								{
									case 1: /* SUMMATION */
									case 2: /* AVERAGE */
									case 3: /* MINIMUM */
									case 4: /* MAXIMUM */
									case 5: /* FREQUENCY */
										R->data->type=DOUBLEV;
										R->data->len=20;
										R->data->data.float_value=Rmalloc(sizeof(double));
										R->data->dlen=sizeof(double);
										break;
									default:
									case 0: /* VALUE */
										prterr("Error How did it get here....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								R->data->type=SDOUBLEV;
								R->data->len=20;
								switch(R->rangetype)
								{
									case 1: /* SUMMATION */
									case 2: /* AVERAGE */
									case 3: /* MINIMUM */
									case 4: /* MAXIMUM */
									case 5: /* FREQUENCY */
										R->data->type=DOUBLEV;
										R->data->len=20;
										R->data->data.float_value=Rmalloc(sizeof(double));
										R->data->dlen=sizeof(double);
										break;
									default:
									case 0: /* VALUE */
										prterr("Error How did it get here....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
								break;
							case BUTTONS:
								prterr("Error call RaddogTiddog cause this is weird....");
								break;
							default:
								prterr("Error field type [%d] invalid for cell value [%s] at line [%d] program [%s].",c->type,c->name,line,file);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
						}
						if(s!=NULL) Rfree(s);
						if(R->m->recs!=NULL && R->m->yaxis.mem!=NULL &&
							R->m->xaxis.mem!=NULL)
						{
							for(x=0,amy=R->m->yaxis.mem;x<R->m->yaxis.num;
								++x,++amy)
							{
								if(!RDAstrcmp(R->from_row->name,"Y-AXIS TITLE 1"))
								{
									if(!RDAstrcmp(amy->title1,
										R->from_row->data.string_value)) break;
								} else if(!RDAstrcmp(R->from_row->name,"Y-AXIS TITLE 2"))
								{
									if(!RDAstrcmp(amy->title2,
										R->from_row->data.string_value)) break;
								} else if(!RDAstrcmp(R->from_row->name,"Y-AXIS VALUE"))
								{
									doit=FALSE;
									switch(R->m->yaxis.type)
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
											if(!RDAstrcmp(R->from_row->data.string_value,amy->data.string_value)) doit=TRUE;
											break;
										case BOOLNS:
										case CHARACTERS:
											if(*R->from_row->data.string_value==*amy->data.string_value) doit=TRUE;
											break;
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
										case SDECIMALV:
											if(*R->from_row->data.float_value==*amy->data.float_value) doit=TRUE;
											break;
										case SHORTV:
										case SSHORTV:
											if(*R->from_row->data.short_value==*amy->data.short_value) doit=TRUE;
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(*R->from_row->data.integer_value==*amy->data.integer_value) doit=TRUE;
											break;
										case BUTTONS:
											prterr("Error call RaddogTiddog cause this is weird....");
											FreeMATRIX_RUNTIME(R);
											if(mod!=NULL) Rfree(mod);
											if(name!=NULL) Rfree(name);
											if(s!=NULL) Rfree(s);
											return(NULL);
										default:
											prterr("Error field type [%d] invalid for yaxis value  at line [%d] program [%s].",R->m->yaxis.type,line,file);
											FreeMATRIX_RUNTIME(R);
											if(mod!=NULL) Rfree(mod);
											if(name!=NULL) Rfree(name);
											if(s!=NULL) Rfree(s);
											return(NULL);
									}
								} else {
									prterr("Error Y-Axis Member not specified....");
									FreeMATRIX_RUNTIME(R);
									if(mod!=NULL) Rfree(mod);
									if(name!=NULL) Rfree(name);
									if(s!=NULL) Rfree(s);
									return(NULL);
								}
								if(doit) break;
							}
						}
					}
/* SOLVE PUZZLE HERE */
					if(R->data!=NULL && R->m->recs!=NULL && R->m->defs!=NULL
						&& R->m->yaxis.mem!=NULL && R->m->xaxis.mem!=NULL &&
							R->from_row!=NULL && R->to_row!=NULL &&
							R->from_col!=NULL && R->to_col!=NULL)
					{
						for(x=0,amy=R->m->yaxis.mem,rec=R->m->recs;
							x<R->m->num;++x,++amy,++rec)
						{
							okfrom=FALSE;
							okto=FALSE;
							if(!RDAstrcmp(R->from_row->name,"Y-AXIS TITLE 1"))
							{
								if(RDAstrcmp(amy->title1,
									R->from_row->data.string_value)>=0) 
									okfrom=TRUE;
							} else if(!RDAstrcmp(R->from_row->name,
								"Y-AXIS TITLE 2"))
							{
								if(RDAstrcmp(amy->title2,
									R->from_row->data.string_value)>=0) 
									okfrom=TRUE;
							} else if(!RDAstrcmp(R->from_row->name,
								"Y-AXIS VALUE"))
							{
								switch(R->m->yaxis.type)
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
										if(RDAstrcmp(amy->data.string_value,
											R->from_row->data.string_value)>=0) 
											okfrom=TRUE;
										break;
									case BOOLNS:
									case CHARACTERS:
										if(*amy->data.string_value>=
											*R->from_row->data.string_value)
											okfrom=TRUE;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(*amy->data.float_value>=
											*R->from_row->data.float_value)
											okfrom=TRUE;
										break;
									case SHORTV:
									case SSHORTV:
										if(*amy->data.short_value>=
											*R->from_row->data.short_value)
											okfrom=TRUE;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(*amy->data.integer_value>=
											*R->from_row->data.integer_value)
											okfrom=TRUE;
										break;
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										break;
									default:
										prterr("Error field type [%d] invalid at line [%d] program [%s].",R->from_row->type,line,file);
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
							} else {
								prterr("Error Ranging on Y-AXIS (From Row).");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
							if(!RDAstrcmp(R->to_row->name,"Y-AXIS TITLE 1"))
							{
								if(RDAstrcmp(amy->title1,
									R->to_row->data.string_value)<=0) 
									okto=TRUE;
							} else if(!RDAstrcmp(R->to_row->name,
								"Y-AXIS TITLE 2"))
							{
								if(RDAstrcmp(amy->title2,
									R->to_row->data.string_value)<=0) 
									okto=TRUE;
							} else if(!RDAstrcmp(R->to_row->name,"Y-AXIS VALUE"))
							{
								switch(R->m->yaxis.type)
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
										if(RDAstrcmp(amy->data.string_value,
											R->to_row->data.string_value)<=0) 
											okto=TRUE;
										break;
									case BOOLNS:
									case CHARACTERS:
										if(*amy->data.string_value<=
											*R->to_row->data.string_value)
											okto=TRUE;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(*amy->data.float_value<=
											*R->to_row->data.float_value)
											okto=TRUE;
										break;
									case SHORTV:
									case SSHORTV:
										if(*amy->data.short_value<=
											*R->to_row->data.short_value)
											okto=TRUE;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(*amy->data.integer_value<=
											*R->to_row->data.integer_value)
											okto=TRUE;
										break;
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										break;
									default:
										prterr("Error field type [%d] invalid at line [%d] program [%s].",R->from_row->type,line,file);
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
							} else {
								prterr("Error Ranging on Y-AXIS (From Row).");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
							if(okfrom && okto)
							{
								if(rec->mems!=NULL)
								{
									for(y=0,amx=R->m->xaxis.mem,mem=rec->mems;
										y<rec->num;++y,++amx,++mem)
									{
										okfrom=FALSE;
										okto=FALSE;
										if(!RDAstrcmp(R->from_col->name,"X-AXIS TITLE 1"))
										{
											if(RDAstrcmp(amx->title1,
												R->from_col->data.string_value)>=0) 
												okfrom=TRUE;
										} else if(!RDAstrcmp(R->from_col->name,
											"X-AXIS TITLE 2"))
										{
											if(RDAstrcmp(amx->title2,
												R->from_col->data.string_value)>=0) 
												okfrom=TRUE;
										} else if(!RDAstrcmp(R->from_col->name,
											"X-AXIS VALUE"))
										{
											switch(R->m->xaxis.type)
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
													if(RDAstrcmp(amx->data.string_value,
														R->from_col->data.string_value)>=0) 
														okfrom=TRUE;
													break;
												case BOOLNS:
												case CHARACTERS:
													if(*amx->data.string_value>=
														*R->from_col->data.string_value)
														okfrom=TRUE;
													break;
												case DOLLARS:
												case DOLLARS_NOCENTS:
												case DECIMALV:
												case DOUBLEV:
												case SDOUBLEV:
												case SDECIMALV:
													if(*amx->data.float_value>=
														*R->from_col->data.float_value)
														okfrom=TRUE;
													break;
												case SHORTV:
												case SSHORTV:
													if(*amx->data.short_value>=
														*R->from_col->data.short_value)
														okfrom=TRUE;
													break;
												case SCROLLEDLIST:
												case LONGV:
												case SLONGV:
													if(*amx->data.integer_value>=
														*R->from_col->data.integer_value)
														okfrom=TRUE;
													break;
												case BUTTONS:
													prterr("Error call RaddogTiddog cause this is weird....");
													break;
												default:
													prterr("Error field type [%d] invalid at line [%d] program [%s].",R->from_col->type,line,file);
													FreeMATRIX_RUNTIME(R);
													if(mod!=NULL) Rfree(mod);
													if(name!=NULL) Rfree(name);
													if(s!=NULL) Rfree(s);
													return(NULL);
											}
										} else {
											prterr("Error Ranging on X-AXIS (From Column).");
											FreeMATRIX_RUNTIME(R);
											if(mod!=NULL) Rfree(mod);
											if(name!=NULL) Rfree(name);
											if(s!=NULL) Rfree(s);
											return(NULL);
										}
										if(!RDAstrcmp(R->to_col->name,"X-AXIS TITLE 1"))
										{
											if(RDAstrcmp(amx->title1,
												R->to_col->data.string_value)<=0) 
												okto=TRUE;
										} else if(!RDAstrcmp(R->to_col->name,
											"X-AXIS TITLE 2"))
										{
											if(RDAstrcmp(amx->title2,
												R->to_col->data.string_value)<=0) 
												okto=TRUE;
										} else if(!RDAstrcmp(R->to_col->name,"X-AXIS VALUE"))
										{
											switch(R->m->xaxis.type)
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
													if(RDAstrcmp(amx->data.string_value,
														R->to_col->data.string_value)<=0) 
														okto=TRUE;
													break;
												case BOOLNS:
												case CHARACTERS:
													if(*amx->data.string_value<=
														*R->to_col->data.string_value)
														okto=TRUE;
													break;
												case DOLLARS:
												case DOLLARS_NOCENTS:
												case DECIMALV:
												case DOUBLEV:
												case SDOUBLEV:
												case SDECIMALV:
													if(*amx->data.float_value<=
														*R->to_col->data.float_value)
														okto=TRUE;
													break;
												case SHORTV:
												case SSHORTV:
													if(*amx->data.short_value<=
														*R->to_col->data.short_value)
														okto=TRUE;
													break;
												case SCROLLEDLIST:
												case LONGV:
												case SLONGV:
													if(*amx->data.integer_value<=
														*R->to_col->data.integer_value)
														okto=TRUE;
													break;
												case BUTTONS:
													prterr("Error call RaddogTiddog cause this is weird....");
													break;
												default:
													prterr("Error field type [%d] invalid at line [%d] program [%s].",R->to_col->type,line,file);
													FreeMATRIX_RUNTIME(R);
													if(mod!=NULL) Rfree(mod);
													if(name!=NULL) Rfree(name);
													if(s!=NULL) Rfree(s);
													return(NULL);
											}
										} else {
											prterr("Error Ranging on X-AXIS (To Column).");
											FreeMATRIX_RUNTIME(R);
											if(mod!=NULL) Rfree(mod);
											if(name!=NULL) Rfree(name);
											if(s!=NULL) Rfree(s);
											return(NULL);
										}
										if(okfrom && okto)
										{
											for(z=0,c=R->m->defs,v=mem->values;
												z<mem->num;++z,++c,++v)
											{
												if(!RDAstrcmp(c->name,
													R->data->name)) break;
											}
											if(z<mem->num)
											{
												++CELL_COUNTER;
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
														switch(R->rangetype)
														{
															case 1: /* SUMMATION */
															case 2: /* AVERAGE */
															case 3: /* MINIMUM */
															case 4: /* MAXIMUM */
																if(R->data->data.string_value!=NULL) Rfree(R->data->data.string_value);
																R->data->data.string_value=NULL;
																R->data->dlen=0;
																if(v->data.string_value!=NULL)
																{
																	R->data->data.string_value=stralloc(v->data.string_value);
																	R->data->dlen=RDAstrlen(v->data.string_value)+1;
																} 
																break;
															case 5: /* FREQUENCY */
																*R->data->data.float_value+=1;
																break;
															default:
															case 0: /* VALUE */
																prterr("Error How did it get here....");
																FreeMATRIX_RUNTIME(R);
																if(mod!=NULL) Rfree(mod);
																if(name!=NULL) Rfree(name);
																if(s!=NULL) Rfree(s);
																return(NULL);
														}
														break;
													case BOOLNS:
													case CHARACTERS:
														switch(R->rangetype)
														{
															case 1: /* SUMMATION */
																*R->data->data.float_value+=*v->data.string_value;
																break;
															case 2: /* AVERAGE */
																*R->data->data.float_value=((*R->data->data.float_value*(CELL_COUNTER-1))+*v->data.string_value)/CELL_COUNTER;
																break;
															case 3: /* MINIMUM */
																if(*R->data->data.float_value>*v->data.string_value)
																	*R->data->data.float_value=*v->data.string_value;
																break;
															case 4: /* MAXIMUM */
																if(*R->data->data.float_value<*v->data.string_value)
																	*R->data->data.float_value=*v->data.string_value;
																break;
															case 5: /* FREQUENCY */
																*R->data->data.float_value+=1;
																break;
															default:
															case 0: /* VALUE */
																prterr("Error How did it get here....");
																FreeMATRIX_RUNTIME(R);
																if(mod!=NULL) Rfree(mod);
																if(name!=NULL) Rfree(name);
																if(s!=NULL) Rfree(s);
																return(NULL);
														}
														break;
													case DOLLARS:
													case DOLLARS_NOCENTS:
													case DECIMALV:
													case DOUBLEV:
													case SDOUBLEV:
													case SDECIMALV:
														switch(R->rangetype)
														{
															case 1: /* SUMMATION */
																*R->data->data.float_value+=*v->data.float_value;
																break;
															case 2: /* AVERAGE */
																*R->data->data.float_value=((*R->data->data.float_value*(CELL_COUNTER-1))+*v->data.float_value)/CELL_COUNTER;
																break;
															case 3: /* MINIMUM */
																if(*R->data->data.float_value>*v->data.float_value)
																	*R->data->data.float_value=*v->data.float_value;
																break;
															case 4: /* MAXIMUM */
																if(*R->data->data.float_value<*v->data.float_value)
																	*R->data->data.float_value=*v->data.float_value;
																break;
															case 5: /* FREQUENCY */
																*R->data->data.float_value+=1;
																break;
															default:
															case 0: /* VALUE */
																prterr("Error How did it get here....");
																FreeMATRIX_RUNTIME(R);
																if(mod!=NULL) Rfree(mod);
																if(name!=NULL) Rfree(name);
																if(s!=NULL) Rfree(s);
																return(NULL);
														}
														break;
													case SHORTV:
													case SSHORTV:
														switch(R->rangetype)
														{
															case 1: /* SUMMATION */
																*R->data->data.float_value+=*v->data.short_value;
																break;
															case 2: /* AVERAGE */
																*R->data->data.float_value=((*R->data->data.float_value*(CELL_COUNTER-1))+*v->data.short_value)/CELL_COUNTER;
																break;
															case 3: /* MINIMUM */
																if(*R->data->data.float_value>*v->data.short_value)
																	*R->data->data.float_value=*v->data.short_value;
																break;
															case 4: /* MAXIMUM */
																if(*R->data->data.float_value<*v->data.short_value)
																	*R->data->data.float_value=*v->data.short_value;
																break;
															case 5: /* FREQUENCY */
																*R->data->data.float_value+=1;
																break;
															default:
															case 0: /* VALUE */
																prterr("Error How did it get here....");
																FreeMATRIX_RUNTIME(R);
																if(mod!=NULL) Rfree(mod);
																if(name!=NULL) Rfree(name);
																if(s!=NULL) Rfree(s);
																return(NULL);
														}
														break;
													case SCROLLEDLIST:
													case LONGV:
													case SLONGV:
														switch(R->rangetype)
														{
															case 1: /* SUMMATION */
																*R->data->data.float_value+=*v->data.integer_value;
																break;
															case 2: /* AVERAGE */
																*R->data->data.float_value=((*R->data->data.float_value*(CELL_COUNTER-1))+*v->data.integer_value)/CELL_COUNTER;
																break;
															case 3: /* MINIMUM */
																if(*R->data->data.float_value>*v->data.integer_value)
																	*R->data->data.float_value=*v->data.integer_value;
																break;
															case 4: /* MAXIMUM */
																if(*R->data->data.float_value<*v->data.integer_value)
																	*R->data->data.float_value=*v->data.integer_value;
																break;
															case 5: /* FREQUENCY */
																*R->data->data.float_value+=1;
																break;
															default:
															case 0: /* VALUE */
																prterr("Error How did it get here....");
																FreeMATRIX_RUNTIME(R);
																if(mod!=NULL) Rfree(mod);
																if(name!=NULL) Rfree(name);
																if(s!=NULL) Rfree(s);
																return(NULL);
														}
														break;
													case BUTTONS:
														prterr("Error call RaddogTiddog cause this is weird....");
														FreeMATRIX_RUNTIME(R);
														if(mod!=NULL) Rfree(mod);
														if(name!=NULL) Rfree(name);
														if(s!=NULL) Rfree(s);
														return(NULL);
													default:
														prterr("Error field type [%d] invalid for Cell [%s] at line [%d] program [%s].",c->type,c->name,line,file);
														FreeMATRIX_RUNTIME(R);
														if(mod!=NULL) Rfree(mod);
														if(name!=NULL) Rfree(name);
														if(s!=NULL) Rfree(s);
														return(NULL);
												}
											} else {
												prterr("Error Matrix Value [%s] not found ......",(R->data->name!=NULL ? R->data->name:""));
												FreeMATRIX_RUNTIME(R);
												if(mod!=NULL) Rfree(mod);
												if(name!=NULL) Rfree(name);
												if(s!=NULL) Rfree(s);
												return(NULL);
											}
										}
									}
								}
							}
						}
					} else {
						prterr("Error Bad stuff happened somewhere cause there's no datad....");
						FreeMATRIX_RUNTIME(R);
						if(mod!=NULL) Rfree(mod);
						if(name!=NULL) Rfree(name);
						if(s!=NULL) Rfree(s);
						return(NULL);
					}
				} else {
/* VALUE FUNCTIONS */
					for(a=0;a<2;++a)
					{
						while((iswhite(*temp) && *temp!='\0')) ++temp;
						s=stripmodulename(temp);
						if(!RDAstrcmp(s,"Y-AXIS VALUE"))
						{
							if(R->from_row!=NULL)
							{
								FreeNRDfield(R->from_row);
								Rfree(R->from_row);
							}
							temp=temp+(RDAstrlen(s)+2);
							while((iswhite(*temp) && *temp!='\0')) ++temp;
							if(s!=NULL) Rfree(s);
							if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
							{
								s=stripmodulename(temp);
								switch(R->m->yaxis.type)
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
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc("Y-AXIS VALUE");
										R->from_row->type=R->m->yaxis.type;
										R->from_row->len=R->m->yaxis.len;
										if(s!=NULL)
										{
											R->from_row->data.string_value=stralloc(s);
											R->from_row->dlen=RDAstrlen(s)+1;
											temp=temp+RDAstrlen(s)+2;
										} else {
											R->from_row->data.string_value=NULL;
											R->from_row->dlen=0;
											temp=temp+2;
										}
										break;
									case BOOLNS:
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc("Y-AXIS VALUE");
										R->from_row->type=R->m->yaxis.type;
										R->from_row->len=1;
										R->from_row->data.string_value=Rmalloc(1);
										*R->from_row->data.string_value=FALSE;
										if(!isEMPTY(s))
										{
											if(!strncmp(s,"TRUE",4) ||
												!strncmp(s,"True",4) ||
												!strncmp(s,"Yes",3) ||
												!strncmp(s,"YES",3) ||
												!RDAstrcmp(s,"Y") || !RDAstrcmp(s,"T")
												|| !RDAstrcmp(s,"1"))
											{
												*R->from_row->data.string_value=TRUE;
											}
										}
										R->from_row->dlen=1;
										temp=temp+RDAstrlen(s)+2;
										break;
									case CHARACTERS:
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc("Y-AXIS VALUE");
										R->from_row->type=R->m->yaxis.type;
										R->from_row->len=1;
										R->from_row->data.string_value=Rmalloc(1);
/*lint -e524*/
										*R->from_row->data.string_value=*s;
/*lint +e524*/
										R->from_row->dlen=1;
										temp=temp+RDAstrlen(s)+2;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc("Y-AXIS VALUE");
										R->from_row->type=R->m->yaxis.type;
										R->from_row->len=R->m->yaxis.len;
										R->from_row->data.float_value=
											Rmalloc(sizeof(double));
										if(!isEMPTY(s))
										{
										*R->from_row->data.float_value=atof(s);
										} else *R->from_row->data.float_value=0;
										R->from_row->dlen=sizeof(double);
										temp=temp+RDAstrlen(s)+2;
										break;
									case SHORTV:
									case SSHORTV:
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc("Y-AXIS VALUE");
										R->from_row->type=R->m->yaxis.type;
										R->from_row->len=R->m->yaxis.len;
										R->from_row->data.short_value=
											Rmalloc(sizeof(short));
/*lint -e524*/
										if(!isEMPTY(s))
										{
										*R->from_row->data.short_value=Ratoi(s);
/*lint +e524*/
										} else {
										*R->from_row->data.short_value=0;
										}
										R->from_row->dlen=sizeof(short);
										temp=temp+RDAstrlen(s)+2;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc("Y-AXIS VALUE");
										R->from_row->type=R->m->yaxis.type;
										R->from_row->len=R->m->yaxis.len;
										R->from_row->data.integer_value=
											Rmalloc(sizeof(int));
										if(!isEMPTY(s))
										{
										*R->from_row->data.integer_value=Ratoi(s);
										} else {
										*R->from_row->data.integer_value=0;
										}
										R->from_row->dlen=sizeof(int);
										temp=temp+RDAstrlen(s)+2;
										break;
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
									default:
										prterr("Error field type [%d] invalid  at line [%d] program [%s].",R->m->yaxis.type,line,file);
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
							} else {
								prterr("Error in MATRIX Implementation.....");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"Y-AXIS TITLE 1"))
						{
							if(R->from_row!=NULL)
							{
								FreeNRDfield(R->from_row);
								Rfree(R->from_row);
							}
							R->from_row=Rmalloc(sizeof(NRDfield));
							R->from_row->name=stralloc("Y-AXIS TITLE 1");
							R->from_row->type=0;
							R->from_row->len=0;
							R->from_row->dlen=0;
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									R->from_row->data.string_value=stralloc(s);
									R->from_row->dlen=
										RDAstrlen(R->from_row->data.string_value);
								} else R->from_row->data.string_value=NULL; 
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation.....");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"Y-AXIS TITLE 2"))
						{
							if(R->from_row!=NULL)
							{
								FreeNRDfield(R->from_row);
								Rfree(R->from_row);
							}
							R->from_row=Rmalloc(sizeof(NRDfield));
							R->from_row->name=stralloc("Y-AXIS TITLE 2");
							R->from_row->type=0;
							R->from_row->len=0;
							R->from_row->dlen=0;
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									R->from_row->data.string_value=stralloc(s);
									R->from_row->dlen=
										RDAstrlen(R->from_row->data.string_value);
								} else R->from_row->data.string_value=NULL; 
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation.....");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"X-AXIS VALUE"))
						{
							if(R->from_col!=NULL)
							{
								FreeNRDfield(R->from_col);
								Rfree(R->from_col);
							}
							temp=temp+(RDAstrlen(s)+2);
							while((iswhite(*temp) && *temp!='\0')) ++temp;
							if(s!=NULL) Rfree(s);
							if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
							{
								s=stripmodulename(temp);
								switch(R->m->xaxis.type)
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
										R->from_col=Rmalloc(sizeof(NRDfield));
										R->from_col->name=stralloc("X-AXIS VALUE");
										R->from_col->type=R->m->xaxis.type;
										R->from_col->len=R->m->xaxis.len;
										if(s!=NULL)
										{
											R->from_col->data.string_value=stralloc(s);
											R->from_col->dlen=RDAstrlen(s)+1;
											temp=temp+RDAstrlen(s)+2;
										} else {
											R->from_col->data.string_value=NULL;
											R->from_col->dlen=0;
											temp=temp+2;
										}
										break;
									case BOOLNS:
										R->from_col=Rmalloc(sizeof(NRDfield));
										R->from_col->name=stralloc("X-AXIS VALUE");
										R->from_col->type=R->m->xaxis.type;
										R->from_col->len=1;
										R->from_col->data.string_value=Rmalloc(1);
										*R->from_col->data.string_value=FALSE;
										if(!isEMPTY(s))
										{
											if(!strncmp(s,"TRUE",4) ||
												!strncmp(s,"True",4) ||
												!strncmp(s,"Yes",3) ||
												!strncmp(s,"YES",3) ||
												!RDAstrcmp(s,"Y") || !RDAstrcmp(s,"T")
												|| !RDAstrcmp(s,"1"))
											{
												*R->from_col->data.string_value=TRUE;
											}
										}
										R->from_col->dlen=1;
										temp=temp+RDAstrlen(s)+2;
										break;
									case CHARACTERS:
										R->from_col=Rmalloc(sizeof(NRDfield));
										R->from_col->name=stralloc("X-AXIS VALUE");
										R->from_col->type=R->m->xaxis.type;
										R->from_col->len=1;
										R->from_col->data.string_value=Rmalloc(1);
/*lint -e524*/
										*R->from_col->data.string_value=*s;
/*lint +e524*/
										R->from_col->dlen=1;
										temp=temp+RDAstrlen(s)+2;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										R->from_col=Rmalloc(sizeof(NRDfield));
										R->from_col->name=stralloc("X-AXIS VALUE");
										R->from_col->type=R->m->xaxis.type;
										R->from_col->len=R->m->xaxis.len;
										R->from_col->data.float_value=
											Rmalloc(sizeof(double));
										if(!isEMPTY(s))
										{
										*R->from_col->data.float_value=atof(s);
										} else *R->from_col->data.float_value=0;
										R->from_col->dlen=sizeof(double);
										temp=temp+RDAstrlen(s)+2;
										break;
									case SHORTV:
									case SSHORTV:
										R->from_col=Rmalloc(sizeof(NRDfield));
										R->from_col->name=stralloc("X-AXIS VALUE");
										R->from_col->type=R->m->xaxis.type;
										R->from_col->len=R->m->xaxis.len;
										R->from_col->data.short_value=
											Rmalloc(sizeof(short));
										if(!isEMPTY(s))
										{
/*lint -e524*/
										*R->from_col->data.short_value=Ratoi(s);
/*lint +e524*/
										} else {
										*R->from_col->data.short_value=0;
										}
										R->from_col->dlen=sizeof(short);
										temp=temp+RDAstrlen(s)+2;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										R->from_col=Rmalloc(sizeof(NRDfield));
										R->from_col->name=stralloc("X-AXIS VALUE");
										R->from_col->type=R->m->xaxis.type;
										R->from_col->len=R->m->xaxis.len;
										R->from_col->data.integer_value=
											Rmalloc(sizeof(int));
										if(!isEMPTY(s))
										{
										*R->from_col->data.integer_value=Ratoi(s);
										} else {
										*R->from_col->data.integer_value=0;
										}
										R->from_col->dlen=sizeof(int);
										temp=temp+RDAstrlen(s)+2;
										break;
									case BUTTONS:
										prterr("Error call RaddogTiddog cause this is weird....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
									default:
										prterr("Error field type [%d] invalid  at line [%d] program [%s].",R->m->xaxis.type,line,file);
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
								}
							} else {
								prterr("Error in MATRIX Implementation.....");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"X-AXIS TITLE 1"))
						{
							if(R->from_col!=NULL)
							{
								FreeNRDfield(R->from_col);
								Rfree(R->from_col);
							}
							R->from_col=Rmalloc(sizeof(NRDfield));
							R->from_col->name=stralloc("X-AXIS TITLE 1");
							R->from_col->type=0;
							R->from_col->len=0;
							R->from_col->dlen=0;
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									R->from_col->data.string_value=stralloc(s);
									R->from_col->dlen=
										RDAstrlen(R->from_col->data.string_value);
								} else R->from_col->data.string_value=NULL; 
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation.....");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else if(!RDAstrcmp(s,"X-AXIS TITLE 2"))
						{
							if(R->from_col!=NULL)
							{
								FreeNRDfield(R->from_col);
								Rfree(R->from_col);
							}
							R->from_col=Rmalloc(sizeof(NRDfield));
							R->from_col->name=stralloc("X-AXIS TITLE 2");
							R->from_col->type=0;
							R->from_col->len=0;
							R->from_col->dlen=0;
							if(RDAstrlen(temp))
							{
								s=stripmodulename(temp);
								if(s!=NULL)
								{
									R->from_col->data.string_value=stralloc(s);
									R->from_col->dlen=
										RDAstrlen(R->from_col->data.string_value);
								} else R->from_col->data.string_value=NULL; 
								temp=temp+RDAstrlen(s)+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							} else {
								prterr("Error in MATRIX Implementation.....");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else {
							prterr("Error in MATRIX Implementation.....");
							FreeMATRIX_RUNTIME(R);
							if(mod!=NULL) Rfree(mod);
							if(name!=NULL) Rfree(name);
							if(s!=NULL) Rfree(s);
							return(NULL);
						}
					}
					if(s!=NULL) Rfree(s);
					while((iswhite(*temp) && *temp!='\0')) ++temp;
					if(!strncmp(temp,"RETURN_VALUE",12) ||
						!strncmp(temp,"return_value",12))
					{
						temp=temp+12;
						while((iswhite(*temp) && *temp!='\0')) ++temp;
						if(*temp=='=') ++temp;
						if(s!=NULL) Rfree(s);
						s=stripmodulename(temp);
						for(a=0,c=R->m->defs;a<R->m->numdefs;++a,++c)
						{
							if(!RDAstrcmp(s,c->name)) break;
						}
						if(a<R->m->numdefs)
						{
							R->data=Rmalloc(sizeof(NRDfield));
							R->data->name=stralloc(s);
							R->data->type=c->type;
							R->data->len=c->len;
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
									R->data->data.string_value=NULL;
									R->data->dlen=0;
									break;
								case BOOLNS:
								case CHARACTERS:
									R->data->data.string_value=Rmalloc(1);
									*R->data->data.string_value=0;
									R->data->dlen=1;
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									R->data->data.float_value=Rmalloc(sizeof(double));
									*R->data->data.float_value=0;
									R->data->dlen=sizeof(double);
									break;
								case SHORTV:
								case SSHORTV:
									R->data->data.short_value=Rmalloc(sizeof(short));
									*R->data->data.short_value=0;
									R->data->dlen=sizeof(short);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									R->data->data.integer_value=Rmalloc(sizeof(int));
									*R->data->data.integer_value=0;
									R->data->dlen=sizeof(int);
									break;
								case BUTTONS:
									prterr("Error call RaddogTiddog cause this is weird....");
									break;
								default:
									prterr("Error field type [%d] invalid for cell value [%s] at line [%d] program [%s].",c->type,c->name,line,file);
									FreeMATRIX_RUNTIME(R);
									if(mod!=NULL) Rfree(mod);
									if(name!=NULL) Rfree(name);
									if(s!=NULL) Rfree(s);
									return(NULL);
							}
							if(s!=NULL) Rfree(s);
							if(R->m->recs!=NULL && R->m->yaxis.mem!=NULL &&
								R->m->xaxis.mem!=NULL)
							{
								for(x=0,amy=R->m->yaxis.mem;x<R->m->yaxis.num;
									++x,++amy)
								{
									if(!RDAstrcmp(R->from_row->name,"Y-AXIS TITLE 1"))
									{
										if(!RDAstrcmp(amy->title1,
											R->from_row->data.string_value)) break;
									} else if(!RDAstrcmp(R->from_row->name,"Y-AXIS TITLE 2"))
									{
										if(!RDAstrcmp(amy->title2,
											R->from_row->data.string_value)) break;
									} else if(!RDAstrcmp(R->from_row->name,"Y-AXIS VALUE"))
									{
										doit=FALSE;
										switch(R->m->yaxis.type)
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
												if(!RDAstrcmp(R->from_row->data.string_value,amy->data.string_value)) doit=TRUE;
												break;
											case BOOLNS:
											case CHARACTERS:
												if(*R->from_row->data.string_value==*amy->data.string_value) doit=TRUE;
												break;
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												if(*R->from_row->data.float_value==*amy->data.float_value) doit=TRUE;
												break;
											case SHORTV:
											case SSHORTV:
												if(*R->from_row->data.short_value==*amy->data.short_value) doit=TRUE;
												break;
											case SCROLLEDLIST:
											case LONGV:
											case SLONGV:
												if(*R->from_row->data.integer_value==*amy->data.integer_value) doit=TRUE;
												break;
											case BUTTONS:
												prterr("Error call RaddogTiddog cause this is weird....");
												FreeMATRIX_RUNTIME(R);
												if(mod!=NULL) Rfree(mod);
												if(name!=NULL) Rfree(name);
												if(s!=NULL) Rfree(s);
												return(NULL);
											default:
												prterr("Error field type [%d] invalid for yaxis value  at line [%d] program [%s].",R->m->yaxis.type,line,file);
												FreeMATRIX_RUNTIME(R);
												if(mod!=NULL) Rfree(mod);
												if(name!=NULL) Rfree(name);
												if(s!=NULL) Rfree(s);
												return(NULL);
										}
									} else {
										prterr("Error Y-Axis Member not specified....");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
									}
									if(doit) break;
								}
								if(x<R->m->yaxis.num && x<R->m->num)
								{
									rec=R->m->recs+x;
									if(rec->mems!=NULL)
									{
										for(y=0,amx=R->m->xaxis.mem;
											y<R->m->xaxis.num;++y,++amx)
										{
											doit=FALSE;
											if(!RDAstrcmp(R->from_col->name,"X-AXIS TITLE 1"))
											{
												if(!RDAstrcmp(amx->title1,
													R->from_col->data.string_value)) break;

											} else if(!RDAstrcmp(R->from_col->name,"X-AXIS TITLE 2"))
											{
												if(!RDAstrcmp(amx->title2,
													R->from_col->data.string_value)) break;
											} else if(!RDAstrcmp(R->from_col->name,"X-AXIS VALUE"))
											{
												doit=FALSE;
												switch(R->m->xaxis.type)
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
														if(!RDAstrcmp(R->from_col->data.string_value,amx->data.string_value)) doit=TRUE;
														break;
													case BOOLNS:
													case CHARACTERS:
														if(*R->from_col->data.string_value==*amx->data.string_value) doit=TRUE;
														break;
													case DOLLARS:
													case DOLLARS_NOCENTS:
													case DECIMALV:
													case DOUBLEV:
													case SDOUBLEV:
													case SDECIMALV:
														if(*R->from_col->data.float_value==*amx->data.float_value) doit=TRUE;
														break;
													case SHORTV:
													case SSHORTV:
														if(*R->from_col->data.short_value==*amx->data.short_value) doit=TRUE;
														break;
													case SCROLLEDLIST:
													case LONGV:
													case SLONGV:
														if(*R->from_col->data.integer_value==*amx->data.integer_value) doit=TRUE;
														break;
													case BUTTONS:
														prterr("Error call RaddogTiddog cause this is weird....");
														FreeMATRIX_RUNTIME(R);
														if(mod!=NULL) Rfree(mod);
														if(name!=NULL) Rfree(name);
														if(s!=NULL) Rfree(s);
														return(NULL);
													default:
														prterr("Error field type [%s] invalid for xaxis value  at line [%d] program [%s].",R->m->xaxis.type,line,file);
														FreeMATRIX_RUNTIME(R);
														if(mod!=NULL) Rfree(mod);
														if(name!=NULL) Rfree(name);
														if(s!=NULL) Rfree(s);
														return(NULL);
												}
											} else {
												prterr("Error incorrect Column Designation....");
												FreeMATRIX_RUNTIME(R);
												if(mod!=NULL) Rfree(mod);
												if(name!=NULL) Rfree(name);
												if(s!=NULL) Rfree(s);
												return(NULL);
											}
											if(doit) break;
										}
										if(y<R->m->xaxis.num && y<rec->num)
										{
											mem=rec->mems+y;
											if(mem->values!=NULL)
											{
												for(z=0,c=R->m->defs,v=mem->values;
													z<mem->num;++z,++c,++v)
												{
													if(!RDAstrcmp(c->name,
														R->data->name)) break;
												}
												if(z<mem->num)
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
															if(R->data->data.string_value!=NULL) Rfree(R->data->data.string_value);
															R->data->data.string_value=NULL;
															R->data->dlen=0;
															if(v->data.string_value!=NULL)
															{
																R->data->data.string_value=stralloc(v->data.string_value);
																R->data->dlen=RDAstrlen(v->data.string_value)+1;
															} 
															break;
														case BOOLNS:
														case CHARACTERS:
															*R->data->data.string_value=*v->data.string_value;
															break;
														case DOLLARS:
														case DOLLARS_NOCENTS:
														case DECIMALV:
														case DOUBLEV:
														case SDOUBLEV:
														case SDECIMALV:
															*R->data->data.float_value=*v->data.float_value;
															break;
														case SHORTV:
														case SSHORTV:
															*R->data->data.short_value=*v->data.short_value;
															break;
														case SCROLLEDLIST:
														case LONGV:
														case SLONGV:
															*R->data->data.integer_value=*v->data.integer_value;
															break;
														case BUTTONS:
															prterr("Error call RaddogTiddog cause this is weird....");
															FreeMATRIX_RUNTIME(R);
															if(mod!=NULL) Rfree(mod);
															if(name!=NULL) Rfree(name);
															if(s!=NULL) Rfree(s);
															return(NULL);
														default:
															prterr("Error field type [%d] invalid for Cell [%s] at line [%d] program [%s].",c->type,c->name,line,file);
															FreeMATRIX_RUNTIME(R);
															if(mod!=NULL) Rfree(mod);
															if(name!=NULL) Rfree(name);
															if(s!=NULL) Rfree(s);
															return(NULL);
													}
												} else {
													prterr("Error Matrix Value [%s] not found on [%s][%s].",(R->data->name!=NULL ? R->data->name:""),mod,name);
													FreeMATRIX_RUNTIME(R);
													if(mod!=NULL) Rfree(mod);
													if(name!=NULL) Rfree(name);
													if(s!=NULL) Rfree(s);
													return(NULL);
												}
											} else {
												prterr("Error Matrix Cell without values at Row [%d] Column [%d] on [%s][%s]",(x+1),(y+1),mod,name);
												FreeMATRIX_RUNTIME(R);
												if(mod!=NULL) Rfree(mod);
												if(name!=NULL) Rfree(name);
												if(s!=NULL) Rfree(s);
												return(NULL);
											}
										} else {
											prterr("Error Column not found on matrix [%s][%s] at line [%d] program [%s].",mod,name,line,file);
											FreeMATRIX_RUNTIME(R);
											if(mod!=NULL) Rfree(mod);
											if(name!=NULL) Rfree(name);
											if(s!=NULL) Rfree(s);
											return(NULL);
										}
									} else {
										prterr("Error no Columns in Matrix...");
										FreeMATRIX_RUNTIME(R);
										if(mod!=NULL) Rfree(mod);
										if(name!=NULL) Rfree(name);
										if(s!=NULL) Rfree(s);
										return(NULL);
									}
								} else {
#ifdef USE_RDA_DIAGNOSTICS
									if(diagmatrix)
									{
										prterr("Error Row not found in Matrix...");
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
									FreeMATRIX_RUNTIME(R);
									if(mod!=NULL) Rfree(mod);
									if(name!=NULL) Rfree(name);
									if(s!=NULL) Rfree(s);
									return(NULL);
								}
							} else {
								prterr("Error return value not found as cell member.....");
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else {
							prterr("Error in MATRIX Implementation.....");
							FreeMATRIX_RUNTIME(R);
							if(mod!=NULL) Rfree(mod);
							if(name!=NULL) Rfree(name);
							if(s!=NULL) Rfree(s);
							return(NULL);
						}
					} else {
						prterr("Error incorrect Operations....");
						FreeMATRIX_RUNTIME(R);
						if(mod!=NULL) Rfree(mod);
						if(name!=NULL) Rfree(name);
						if(s!=NULL) Rfree(s);
						return(NULL);
					}
				}
				if(s!=NULL) Rfree(s);
			}
			tmp=R->data;
			FreeMATRIX_RUNTIME_EVAL(R,FALSE);
		} else {
			prterr("Error MATRIX [%s][%s] not found at line [%d] program [%s].",mod,name,line,file);
			FreeMATRIX_RUNTIME(R);
		}
	}
	if(mod!=NULL) Rfree(mod);
	if(name!=NULL) Rfree(name);
	return(tmp);
}
#ifdef XXXX
void print_error_func(char *mod,char *name,short rc,short which,short type,
	void *data,int line,char *file)
{
	char *types[2]={"Row":"Column"};

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG print_error__func at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!which) /* TITLE 1 */
	{
		prterr("Error %s %s using [%s] not found in matrix [%s][%s] at line [%d] program [%s].",types[rc],(char *)data,"TITLE 1",mod,name,line,file);
	} else if(which==1) /* TITLE 2 */
	{
		prterr("Error %s %s using [%s] not found in matrix [%s][%s] at line [%d] program [%s].",types[rc],(char *)data,"TITLE 2",mod,name,line,file);
	} else if(which==2) /* VALUE */
	{
		memset(stemp,0,101);
		if(!rc) sprintf(stemp,"ROW VALUE");
			else sprintf(stemp,"COLUMN VALUE");
		switch(type)
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
				prterr("Error %s %s using [%s] not found in matrix [%s][%s] at line [%d] program [%s].",types[rc],(char *)data,stemp,mod,name,line,file);
				break;
			case BOOLNS:
			case CHARACTERS:
				prterr("Error %s %d using [%s] not found in matrix [%s][%s] at line [%d] program [%s].",types[rc],*(char *)data,stemp,mod,name,line,file);
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				prterr("Error %s %f using [%s] not found in matrix [%s][%s] at line [%d] program [%s].",types[rc],*(double *)data,stemp,mod,name,line,file);
				break;
			case SHORTV:
			case SSHORTV:
				prterr("Error %s %d using [%s] not found in matrix [%s][%s] at line [%d] program [%s].",types[rc],*(short *)data,stemp,mod,name,line,file);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				prterr("Error %s %d using [%s] not found in matrix [%s][%s] at line [%d] program [%s].",types[rc],*(int *)data,stemp,mod,name,line,file);
				break;
			case BUTTONS:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
			default:
				prterr("Error field type [%d] invalid for yaxis value  at line [%d] program [%s].",type,line,file);
				break;
		}
	}
}
#endif /* XXXX */
