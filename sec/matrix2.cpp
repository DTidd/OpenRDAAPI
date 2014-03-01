/* matrix1.c - RDAMatrix Compute Functions */
#include<sec.hpp>

NRDfield *xCOMPUTE_MATRIX_LINEARLY(char *string,int line,char *file)
{
	MATRIX_RUNTIME *R=NULL;
	NRDfield *tmp=NULL;
	char *mod=NULL,*name=NULL,*temp=NULL,*s=NULL,okfrom=FALSE;
	char okto=FALSE;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXvalue *v;
	MATRIXcell *c;
	int a,b=0,x,y,z,CELL_COUNTER=0,from_cell=(-1),to_cell=(-1);
	int cellno=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmatrix)
	{
		prterr("DIAG COMPUTE_MATRIX_LINEARLY for [%s] at line [%d] program [%s].",(string!=NULL ? string:""),line,file);
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
				prterr("Error in MATRIX Operator Implementation.....");
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
					for(a=0;a<2;++a)
					{
						while((iswhite(*temp) && *temp!='\0')) ++temp;
						if(a==1)
						{
							if(!strncmp(temp,"TO",2) ||
								!strncmp(temp,"to",2))
							{
								temp=temp+2;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
							}
						}
						s=stripmodulename(temp);
						if(a<1)
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
						if(!RDAstrcmp(s,"CELL NUMBER"))
						{
							if(a<1)
							{
								R->from_row=Rmalloc(sizeof(NRDfield));
								R->from_row->name=stralloc("CELL NUMBER");
								R->from_row->type=LONGV;
								R->from_row->len=12;
								R->from_row->data.integer_value=
									Rmalloc(sizeof(int));
								*R->from_row->data.integer_value=0;
								R->from_row->dlen=sizeof(int);
							} else {
								R->to_row=Rmalloc(sizeof(NRDfield));
								R->to_row->name=stralloc("CELL NUMBER");
								R->to_row->type=LONGV;
								R->to_row->len=12;
								R->to_row->data.integer_value=
									Rmalloc(sizeof(int));
								*R->to_row->data.integer_value=0;
								R->to_row->dlen=sizeof(int);
							}
							temp=temp+(RDAstrlen(s)+2);
							while((iswhite(*temp) && *temp!='\0')) ++temp;
							if(s!=NULL) Rfree(s);
							if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
							{
								s=stripmodulename(temp);
								if(!isEMPTY(s))
								{
								if(a<1)
								{
									*R->from_row->data.integer_value=Ratoi(s);
								} else *R->to_row->data.integer_value=Ratoi(s);
								} else {
								if(a<1)
								{
									*R->from_row->data.integer_value=0;
								} else *R->to_row->data.integer_value=0;
								}	
								temp=temp+(RDAstrlen(s)+2);
								while((iswhite(*temp) && *temp!='\0')) ++temp;
								if(s!=NULL) Rfree(s);
							}
						} else {
							for(b=0,c=R->m->defs;b<R->m->numdefs;++b,++c)
							{
								if(!RDAstrcmp(s,c->name)) break;
							}
							if(b<R->m->numdefs) 
							{
								if(a<1)
								{
									from_cell=b;
								} else to_cell=b;
								while((iswhite(*temp) && *temp!='\0')) ++temp;
								if(s!=NULL) Rfree(s);
								if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
								{
									s=stripmodulename(temp);
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
											if(a<1)
											{
												R->from_row=Rmalloc(sizeof(NRDfield));
												R->from_row->name=stralloc(c->name);
												R->from_row->type=c->type;
												R->from_row->len=c->len;
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
												R->to_row->name=stralloc(c->name);
												R->to_row->type=c->type;
												R->to_row->len=c->len;
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
											if(a<1)
											{
												R->from_row=Rmalloc(sizeof(NRDfield));
												R->from_row->name=stralloc(c->name);
												R->from_row->type=c->type;
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
												R->to_row->name=stralloc(c->name);
												R->to_row->type=c->type;
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
											if(a<1)
											{
												R->from_row=Rmalloc(sizeof(NRDfield));
												R->from_row->name=stralloc(c->name);
												R->from_row->type=c->type;
												R->from_row->len=1;
												R->from_row->data.string_value=Rmalloc(1);
/*lint -e524*/
												*R->from_row->data.string_value=*s;
/*lint +e524*/
												R->from_row->dlen=1;
											} else {
												R->to_row=Rmalloc(sizeof(NRDfield));
												R->to_row->name=stralloc(c->name);
												R->to_row->type=c->type;
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
											if(a<1)
											{
												R->from_row=Rmalloc(sizeof(NRDfield));
												R->from_row->name=stralloc(c->name);
												R->from_row->type=c->type;
												R->from_row->len=c->len;
												R->from_row->data.float_value=
													Rmalloc(sizeof(double));
												if(!isEMPTY(s))
												{
												*R->from_row->data.float_value=atof(s);
												} else *R->from_row->data.string_value=0;
												R->from_row->dlen=sizeof(double);
											} else {
												R->to_row=Rmalloc(sizeof(NRDfield));
												R->to_row->name=stralloc(c->name);
												R->to_row->type=c->type;
												R->to_row->len=c->len;
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
											if(a<1)
											{
												R->from_row=Rmalloc(sizeof(NRDfield));
												R->from_row->name=stralloc(c->name);
												R->from_row->type=c->type;
												R->from_row->len=c->len;
												R->from_row->data.short_value=
														Rmalloc(sizeof(short));
												if(!isEMPTY(s))
												{
/*lint -e524*/
												*R->from_row->data.short_value=Ratoi(s);
/*lint +e524*/
												} else *R->from_row->data.short_value=0; 
												R->from_row->dlen=sizeof(short);
											} else {
												R->to_row=Rmalloc(sizeof(NRDfield));
												R->to_row->name=stralloc(c->name);
												R->to_row->type=c->type;
												R->to_row->len=c->len;
												R->to_row->data.short_value=
														Rmalloc(sizeof(short));
												if(!isEMPTY(s))
												{
/*lint -e524*/
												*R->to_row->data.short_value=Ratoi(s);
/*lint +e524*/
												} else *R->to_row->data.short_value=0; 
												R->to_row->dlen=sizeof(short);
											}
											temp=temp+RDAstrlen(s)+2;
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(a<1)
											{
												R->from_row=Rmalloc(sizeof(NRDfield));
												R->from_row->name=stralloc(c->name);
												R->from_row->type=c->type;
												R->from_row->len=c->len;
												R->from_row->data.integer_value=
													Rmalloc(sizeof(int));
												if(!isEMPTY(s))
												{
												*R->from_row->data.integer_value=Ratoi(s);
												} else *R->from_row->data.integer_value=0;
												R->from_row->dlen=sizeof(int);
											} else {
												R->to_row=Rmalloc(sizeof(NRDfield));
												R->to_row->name=stralloc(c->name);
												R->to_row->type=c->type;
												R->to_row->len=c->len;
												R->to_row->data.integer_value=
													Rmalloc(sizeof(int));
												if(!isEMPTY(s))
												{
												*R->to_row->data.integer_value=Ratoi(s);
												} else *R->to_row->data.integer_value=0;
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
											prterr("Error field type [%d] invalid  at line [%d] program [%s].",R->m->yaxis.type,line,file);
											FreeMATRIX_RUNTIME(R);
											if(mod!=NULL) Rfree(mod);
											if(name!=NULL) Rfree(name);
											if(s!=NULL) Rfree(s);
											return(NULL);
									}
									if(s!=NULL) Rfree(s);
								} else {
									prterr("Error in MATRIX Implementation.....");
									FreeMATRIX_RUNTIME(R);
									if(mod!=NULL) Rfree(mod);
									if(name!=NULL) Rfree(name);
									if(s!=NULL) Rfree(s);
									return(NULL);
								}
							}
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
					while((iswhite(*temp) && *temp!='\0')) ++temp;
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
										*R->data->data.float_value=0;
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
										*R->data->data.float_value=0;
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
										*R->data->data.float_value=0;
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
										*R->data->data.float_value=0;
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
										*R->data->data.float_value=0;
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
/* SOLVING POWER FUNCTIONS */
						if(R->m->recs!=NULL && R->m->defs!=NULL &&
							R->data!=NULL && R->from_row!=NULL &&
								R->to_row!=NULL)
						{
							for(x=0,rec=R->m->recs;x<R->m->num;++x,++rec)
							{
								if(rec->mems!=NULL)
								{
								for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
								{
									okfrom=FALSE;
									okto=FALSE;
									if(!RDAstrcmp(R->from_row->name,"CELL NUMBER"))
									{
										cellno=((x*R->m->xaxis.num)+y+1);
										if(*R->from_row->data.integer_value<=
											cellno) okfrom=TRUE;
									} else {
										c=R->m->defs+from_cell;
										v=mem->values+from_cell;
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
												if(RDAstrcmp(v->data.string_value,R->from_row->data.string_value)>=0)  okfrom=TRUE;
												break;
											case BOOLNS:
											case CHARACTERS:
												if(*v->data.string_value>=
													*R->from_row->data.string_value)
													okfrom=TRUE;
												break;
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												if(*v->data.float_value>=
													*R->from_row->data.float_value)
													okfrom=TRUE;
												break;
											case SHORTV:
											case SSHORTV:
												if(*v->data.short_value>=
													*R->from_row->data.short_value)
													okfrom=TRUE;
												break;
											case SCROLLEDLIST:
											case LONGV:
											case SLONGV:
												if(*v->data.integer_value>=
													*R->from_row->data.integer_value)
													okfrom=TRUE;
												break;
											case BUTTONS:
												prterr("Error call RaddogTiddog cause this is weird....");
												break;
											default:
												prterr("Error field type [%d] invalid at line [%d] program [%s].",c->type,line,file);
												FreeMATRIX_RUNTIME(R);
												if(mod!=NULL) Rfree(mod);
												if(name!=NULL) Rfree(name);
												if(s!=NULL) Rfree(s);
												return(NULL);
										}
									}
									if(!RDAstrcmp(R->to_row->name,"CELL NUMBER"))
									{
										cellno=((x*R->m->xaxis.num)+y+1);
										if(*R->to_row->data.integer_value<=
											cellno) okto=TRUE;
									} else {
										c=R->m->defs+to_cell;
										v=mem->values+to_cell;
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
												if(RDAstrcmp(v->data.string_value,R->to_row->data.string_value)<=0)  okto=TRUE;
												break;
											case BOOLNS:
											case CHARACTERS:
												if(*v->data.string_value<=
													*R->to_row->data.string_value)
													okto=TRUE;
												break;
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case DOUBLEV:
											case SDOUBLEV:
											case SDECIMALV:
												if(*v->data.float_value<=
													*R->to_row->data.float_value)
													okto=TRUE;
												break;
											case SHORTV:
											case SSHORTV:
												if(*v->data.short_value<=
													*R->to_row->data.short_value)
													okto=TRUE;
												break;
											case SCROLLEDLIST:
											case LONGV:
											case SLONGV:
												if(*v->data.integer_value<=
													*R->to_row->data.integer_value)
													okto=TRUE;
												break;
											case BUTTONS:
												prterr("Error call RaddogTiddog cause this is weird....");
												break;
											default:
												prterr("Error field type [%d] invalid at line [%d] program [%s].",c->type,line,file);
												FreeMATRIX_RUNTIME(R);
												if(mod!=NULL) Rfree(mod);
												if(name!=NULL) Rfree(name);
												if(s!=NULL) Rfree(s);
												return(NULL);
										}
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
						prterr("Error something bad happened....");
						FreeMATRIX_RUNTIME(R);
						if(mod!=NULL) Rfree(mod);
						if(name!=NULL) Rfree(name);
						if(s!=NULL) Rfree(s);
						return(NULL);
					}
					tmp=R->data;
				} else {
/* VALUE FUNCTIONS */
					if(R->from_row!=NULL)
					{
						FreeNRDfield(R->from_row);
						Rfree(R->from_row);
					}
					while((iswhite(*temp) && *temp!='\0')) ++temp;
					s=stripmodulename(temp);
					temp=temp+(RDAstrlen(s)+2);
					while((iswhite(*temp) && *temp!='\0')) ++temp;
					if(!RDAstrcmp(s,"CELL NUMBER"))
					{
						R->from_row=Rmalloc(sizeof(NRDfield));
						R->from_row->name=stralloc("CELL NUMBER");
						R->from_row->type=LONGV;
						R->from_row->len=12;
						R->from_row->data.integer_value=Rmalloc(sizeof(int));
						*R->from_row->data.integer_value=0;
						R->from_row->dlen=sizeof(int);
						if(s!=NULL) Rfree(s);
						if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
						{
							s=stripmodulename(temp);
							if(!isEMPTY(s))
							{
							*R->from_row->data.integer_value=Ratoi(s);
							} else *R->from_row->data.integer_value=0;
							temp=temp+(RDAstrlen(s)+2);
							while((iswhite(*temp) && *temp!='\0')) ++temp;
							if(s!=NULL) Rfree(s);
						}
					} else {
						for(b=0,c=R->m->defs;b<R->m->numdefs;++b,++c)
						{
							if(!RDAstrcmp(s,c->name)) break;
						}
						if(b<R->m->numdefs) 
						{
							from_cell=b;
							while((iswhite(*temp) && *temp!='\0')) ++temp;
							if(s!=NULL) Rfree(s);
							if(RDAstrlen(temp) && temp!=NULL && *temp!='\0')
							{
								s=stripmodulename(temp);
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
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc(c->name);
										R->from_row->type=c->type;
										R->from_row->len=c->len;
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
										R->from_row->name=stralloc(c->name);
										R->from_row->type=c->type;
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
										R->from_row->name=stralloc(c->name);
										R->from_row->type=c->type;
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
										R->from_row->name=stralloc(c->name);
										R->from_row->type=c->type;
										R->from_row->len=c->len;
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
										R->from_row->name=stralloc(c->name);
										R->from_row->type=c->type;
										R->from_row->len=c->len;
										R->from_row->data.short_value=
												Rmalloc(sizeof(short));
										if(!isEMPTY(s))
										{
/*lint -e524*/
										*R->from_row->data.short_value=Ratoi(s);
/*lint +e524*/
										} else *R->from_row->data.short_value=0;
										R->from_row->dlen=sizeof(short);
										temp=temp+RDAstrlen(s)+2;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										R->from_row=Rmalloc(sizeof(NRDfield));
										R->from_row->name=stralloc(c->name);
										R->from_row->type=c->type;
										R->from_row->len=c->len;
										R->from_row->data.integer_value=
											Rmalloc(sizeof(int));
										if(!isEMPTY(s))
										{
										*R->from_row->data.integer_value=Ratoi(s);
										} else *R->from_row->data.integer_value=0;
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
								prterr("Error value not found...."); 
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else {
							prterr("Error field [%s] not found....",s);
							FreeMATRIX_RUNTIME(R);
							if(mod!=NULL) Rfree(mod);
							if(name!=NULL) Rfree(name);
							if(s!=NULL) Rfree(s);
							return(NULL);
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
							} else {
								prterr("Error RETURN_VALUE [%s] not found.",s);
								FreeMATRIX_RUNTIME(R);
								if(mod!=NULL) Rfree(mod);
								if(name!=NULL) Rfree(name);
								if(s!=NULL) Rfree(s);
								return(NULL);
							}
						} else {
							prterr("Error RETURN_VALUE not designated....");
							FreeMATRIX_RUNTIME(R);
							if(mod!=NULL) Rfree(mod);
							if(name!=NULL) Rfree(name);
							if(s!=NULL) Rfree(s);
							return(NULL);
						}
						if(R->data!=NULL && R->m->recs!=NULL && R->from_row!=NULL)
						{
							for(x=0,rec=R->m->recs;x<R->m->num;++x,++rec)
							{
								if(rec->mems!=NULL)
								{
									for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
									{
										okfrom=FALSE;
										if(!RDAstrcmp(R->from_row->name,"CELL NUMBER"))
										{
											cellno=((x*R->m->xaxis.num)+y+1);
											if(*R->from_row->data.integer_value<=
												cellno) okfrom=TRUE;
										} else {
											c=R->m->defs+from_cell;
											v=mem->values+from_cell;
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
													if(!RDAstrcmp(v->data.string_value,R->from_row->data.string_value))  okfrom=TRUE;
													break;
												case BOOLNS:
												case CHARACTERS:
													if(*v->data.string_value==
														*R->from_row->data.string_value)
														okfrom=TRUE;
													break;
												case DOLLARS:
												case DOLLARS_NOCENTS:
												case DECIMALV:
												case DOUBLEV:
												case SDOUBLEV:
												case SDECIMALV:
													if(*v->data.float_value==
														*R->from_row->data.float_value)
														okfrom=TRUE;
													break;
												case SHORTV:
												case SSHORTV:
													if(*v->data.short_value==
														*R->from_row->data.short_value)
														okfrom=TRUE;
													break;
												case SCROLLEDLIST:
												case LONGV:
												case SLONGV:
													if(*v->data.integer_value==
														*R->from_row->data.integer_value)
													okfrom=TRUE;
													break;
												case BUTTONS:
													prterr("Error call RaddogTiddog cause this is weird....");
													break;
												default:
													prterr("Error field type [%d] invalid at line [%d] program [%s].",c->type,line,file);
													FreeMATRIX_RUNTIME(R);
													if(mod!=NULL) Rfree(mod);
													if(name!=NULL) Rfree(name);
													if(s!=NULL) Rfree(s);
													return(NULL);
											}
										}
										if(okfrom)
										{
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
													prterr("Error Matrix Value [%s] not found ......",(R->data->name!=NULL ? R->data->name:""));
													FreeMATRIX_RUNTIME(R);
													if(mod!=NULL) Rfree(mod);
													if(name!=NULL) Rfree(name);
													if(s!=NULL) Rfree(s);
													return(NULL);
												}
											} else {
												prterr("Error Member has no values....");
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
/* SOLVING SINGLE */
						} else {
							prterr("Error bad things must have happened....");
							FreeMATRIX_RUNTIME(R);
							if(mod!=NULL) Rfree(mod);
							if(name!=NULL) Rfree(name);
							if(s!=NULL) Rfree(s);
							return(NULL);
						}
					}
				}
				if(s!=NULL) Rfree(s);
			} else {
				prterr("Error Incorrect Operations (rangetype unknown).....");
				FreeMATRIX_RUNTIME(R);
				if(mod!=NULL) Rfree(mod);
				if(name!=NULL) Rfree(name);
				if(s!=NULL) Rfree(s);
				return(NULL);
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
