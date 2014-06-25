#include <mix.hpp>
#include <gui.hpp>
#include <ldval.hpp>
void kw_browse_exit(RDArsrc *,Browse_Exit *);
void proc_browse_exit(Browse_Exit *,void (*)(...));
void browse_exit(RDArsrc *,Browse_Exit *);
APPlib *DEFAULT_GROUPLIST=NULL;


RDAbrowse *RDAbrowseNEW(short fileno,short keyno,void (*listfunc)(...))
{
/* this function is used to allocate an RDAbrowse Structure */
	RDAbrowse *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDAbrowseNEW Creating RDAbrowse Structure.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAbrowse));
	tmp->fileno=fileno;
	tmp->keyno=keyno;
	tmp->num=0;
	tmp->key=NULL;
	tmp->string=NULL;
	tmp->keyvalue=NULL;
	tmp->listfunc=listfunc;
	return tmp;
}
#define seeRDAbrowse(x)	xseeRDAbrowse(x,__LINE__,__FILE__)
void xseeRDAbrowse(RDAbrowse *b,int line,char *file)
{
/* this function is used to print the contents of an RDAbrowse structure to
	standard error (RDA_STDERR) */
	int x;
	char **string;

	prterr("DIAG Showing RDAbrowse Elements Selected for file [%d] key [%d] at line [%d] program [%s].",b->fileno,b->keyno,line,file);
	for(x=0,string=b->string;x<b->num;++x)
	{
		prterr("Element [%d] String [%s]\n",x,string[x]);
	}
}
void RDAbrowseFREE(RDAbrowse *b)
{
/* this function is used to free a previously allocated RDAbrowse structure */
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDAbrowseFREE Freeing RDAbrowse Structure.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(b!=NULL)
	{
	if(b->key!=NULL)
	{
		for(x=0;x<b->num;++x) if(b->key[x]!=NULL) Rfree(b->key[x]);
		Rfree(b->key);
	}
	if(b->string!=NULL)
	{
		for(x=0;x<b->num;++x) 
		{
			if(b->string[x]!=NULL) Rfree(b->string[x]);
		}
		Rfree(b->string);
	}
	Rfree(b);
	b=NULL;
	}
}
void addRDAbrowseelement(RDAbrowse *b,short keysize,void *key,char *string)
{
/* this function is used to add a browse element to the RDAbrowse structure as
	well as the corresponding target/key value required to bring up a 
	record.  The target/key value may be a (void *) for Ctree or a 
	long value (representing physical record no) for RDA Database */
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG addRDAbrowseelement Adding Browse Element [%s] with key size [%d].",string,keysize);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(keysize>0)
	{
		if(b->key==NULL)
		{
			b->key=Rmalloc(sizeof(void *));
		} else {
			b->key=Rrealloc(b->key,((b->num+1)*sizeof(void *)));
		}
		b->key[b->num]=Rmalloc(keysize+1);
		memset(b->key[b->num],0,keysize+1);
		if(key!=NULL)
		{
			memcpy(b->key[b->num],key,keysize);
		} 
	}
	if(b->string==NULL) 
	{
		b->string=Rmalloc(sizeof(char *));
	} else { 
		b->string=Rrealloc(b->string,(b->num+1)*sizeof(char *));
	}
	b->string[b->num]=stralloc(string);
	++b->num;
}
void addRDAbrowse(RDAbrowse *b,char *string)
{
/* this function is used to add a key and viewable string to RDA browse structure */
	void *tmp=NULL,*hold=NULL;
	short length=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG addRDAbrowse Adding Browse Element [%s].",string);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=GETNRDKEY((short)b->fileno,(short)b->keyno,&length);
	if(RDAstrcmp(string,"***************    No Records Selected from current file.   ***************"))
	{
		hold=tmp;
	}
	addRDAbrowseelement(b,length,hold,string);
	if(tmp!=NULL) Rfree(tmp);	
}
void swap_browse(RDAbrowse *b,int x,int y)
{
/* currently not used, could be used to arrange the values in a sorted 
	sequence */
	char *tmpc=b->string[x];
	void *tmpv=b->key[x];

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG Swapping Browse elements [%d] and [%d].",x,y);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	b->key[x]=b->key[y];
	b->string[x]=b->string[y];
	b->key[y]=tmpv;
	b->string[y]=tmpc;
}
void sortRDAbrowse(RDAbrowse *b)
{
/* currently not used, could be used with "swap browse" to produce a 
	browse list in sorted order based upon the viewable list's content's
	rather than the sorted order of datafile's keys */
	int x,y;
 
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG sortRDAbrowse Sorting RDAbrowse Elements.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(b->num>1)
	{
		for(x=0;x<(b->num-1);++x) for(y=(x+1);y<b->num;++y)
		{
			if(RDAstrcmp(b->string[y],b->string[x])<0)
			{
				swap_browse(b,y,x);
			}
		} 
	}
}
static short test_browse_record(MakeBrowseList *m,char *expression)
{
/* this function is used to test the range values from the Search Browse
	screen */
	int f;
	RDAvirtual *v;

/* EVALUATE Search Browse Expression Here */
	if(!isEMPTY(expression))
	{
		if(!PP_EVALUATEbol(expression,SCRNvirtualSubData,m->searchrsrc))
		{
/*
			if(expression!=NULL) Rfree(expression);
*/
			return(FALSE);
		}
	}
	if(testfilerangersrc(m->fileno,m->rs)) return(FALSE);
	if(m->searchrsrc->virflds!=NULL)
	{
		for(f=0,v=m->searchrsrc->virflds;f<m->searchrsrc->num;++f,++v)
		{
			if(ADVtestvirtualrangersrc(v,m->rs,SCRNvirtualSubData,m->searchrsrc))
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen)
				{
					prterr("DIAG testvirtualrangersrc rejected record.");
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		}
	}
	return(TRUE);
}
void GetDFincvirSettings(MakeBrowseList *m)
{
	short l=0;
	int x;
	DFincvir *f;

	if(m!=NULL)
	{
	if(m->definelist!=NULL)
	{
	for(x=0,f=m->definelist->incvir;x<m->definelist->numvir;++x,++f)
	{
		if(f->fileno!=(-1) && f->fileno!=m->fileno)
		{
			l=0;
			f->v=GETNRDKEY(f->fileno,1,&l);
		}
	}
	}
	}
}
void FreeAndSetDFincvir(MakeBrowseList *m)
{
	int x;
	DFincvir *f;

	if(m!=NULL)
	{
	if(m->definelist!=NULL)
	{
	for(x=0,f=m->definelist->incvir;x<m->definelist->numvir;++x,++f)
	{
		if(f->fileno!=(-1) && f->fileno!=m->fileno)
		{
			ZERNRD(f->fileno);	
			ADVEQLNRDKEYsec(f->fileno,1,f->v,SCRNvirtualSubData,m->definelist);
			if(f->v!=NULL) Rfree(f->v);
		}
	}
	}
	}
}
void xSetRDArsrcFiles(short filenum,RDArsrc *m,int line,char *file)
{
	int x,y;
	DFincvir *f;
	NRDkey *key;
	NRDpart *part;
	short getfile=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG SetRDArsrcFiles at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(m!=NULL)
	{
	if(m->incvir!=NULL)
	{
	for(x=0,f=m->incvir;x<m->numvir;++x,++f)
	{
		if(f->fileno!=(-1) && f->fileno!=filenum)
		{
			key=KEYNUM(f->fileno,1);
			if(key!=NULL)
			{
				getfile=FALSE;
				for(y=0,part=key->part;(y<key->numparts && !getfile);
					++y,++part)
				{
/* getting errors here 05-16-2006 (dct) */
					getfile=COPYFIELD(filenum,f->fileno,part->name);
				}
				if(y>=key->numparts)
				{
					if(ADVEQLNRDsec(f->fileno,1,SCRNvirtualSubData,m)) ZERNRD(f->fileno);
				}
			}
		}
	}
	}
	}
}
void xmakebrowselist(MakeBrowseList *m,RDArsrc *tmprsrc)
{
/* this function is used to take the define list values, search browse values, and the file pointer, and create the viewable browse list and array of key values. */
	short f=0,x,cur=1,length=0,lcount=0,addit=FALSE;
	short test_browse=FALSE,breakout=TRUE,select=TRUE;
	int z,w=0,total_widgets=0;
	RDArmem *len_member=NULL,*pos_member=NULL,*mem=NULL;
	char *s=NULL,*hold=NULL,*temp=NULL;
	NRDfield *field=NULL;
	RDArsrc *definelist=NULL;
	RDAbrowse *list=NULL;
	char *test_exp=NULL,*label=NULL,*test_exp1=NULL;
	RDAvirtual *v=NULL;
	short keyrange=FALSE;
	char strtemp[1024];
	char *bl_desc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG makebrowselist for file [%d] [%s] keyno [%d].",m->fileno,FILENAME(m->fileno),m->keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	ZeroBrowseListTotals(m->mainrsrc,m->fileno,m->definelist);
	FINDRSCGETSTRING(m->searchrsrc,"EXPRESSION",&test_exp1);
	if(!isEMPTY(test_exp1)) test_exp=PP_translate(test_exp1);
	if(test_exp1!=NULL) Rfree(test_exp1);
	definelist=m->definelist;
	for(w=0,mem=definelist->rscs;w<definelist->numrscs;++w,++mem)
	{
		if(mem->field_type==SHORTV && (!RDAstrstr(mem->rscrname," POSITION") || !RDAstrstr(mem->rscrname," LENGTH"))) ++total_widgets;
	}
/* determining the maximum length of the viewable browse line */
	for(x=1,length=0;x<(total_widgets);x+=2)
	{
		pos_member=definelist->rscs+x;
		len_member=definelist->rscs+(x+1);
		if(*pos_member->value.short_value>0 && *len_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+1);
		}
	}
	list=m->list;
/* freeing previously existing key values form RDAbrowse structure */
	if(m->list->key!=NULL && m->list->num>0)
	{
		for(x=0;x<m->list->num;++x) 
		{ 
			if(m->list->key[x]!=NULL)
			{
				Rfree(m->list->key[x]);
			}
		}
		Rfree(m->list->key);
		m->list->key=NULL;
	}
/* freeing previously existing string (viewable line) values from RDAbrowse 
	structure */
	if(m->list->string!=NULL && m->list->num>0)
	{
		for(x=0;x<m->list->num;++x) 
		{
			if(m->list->string[x]!=NULL) Rfree(m->list->string[x]);
		}
		Rfree(m->list->string);
		m->list->string=NULL;
	}
	m->list->num=0;
	ZERNRD(m->fileno);
	GetDFincvirSettings(m);
	if(m->passkey!=NULL)
	{
		if(m->passkey->fields!=NULL)
		{
/*lint -e746 */
			if(m->initfunc!=NULL) m->initfunc(m);
/*lint +e746 */
/* making first movement in file to record greater than or equal to the values specified in the initialization function */
			if(m->file_type==1) 
			{
				f=GTENRD(m->fileno,m->keyno);
			} else {
				f=ADVGTENRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
			}
		} else {
			keyrange=setfilekeyFROMrangersrc(m->fileno,m->keyno,m->rs);
			if(keyrange)
			{
				if(m->file_type==1) 
				{
					f=GTENRD(m->fileno,m->keyno);
				} else {
					f=ADVGTENRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
				}
			} else {
				if(m->file_type==1) 
				{
					f=BEGNRD(m->fileno);
				} else {
					f=ADVBEGNRDsec(m->fileno,SCRNvirtualSubData,m->searchrsrc);
				}
			}
		}
	} else if(m->initfunc!=NULL)
	{
/*lint -e746 */
		m->initfunc(m);
/*lint +e746 */
/* making first movement in file to record greater than or equal to the values specified in the initialization function */
		f=ADVGTENRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
	} else {
		keyrange=setfilekeyFROMrangersrc(m->fileno,m->keyno,m->rs);
		if(keyrange)
		{
			if(m->file_type==1) 
			{
				f=GTENRD(m->fileno,m->keyno);
			} else {
				f=ADVGTENRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
			}
		} else {
			if(m->file_type==1) 
			{
				f=BEGNRD(m->fileno);
			} else {
				f=ADVBEGNRDsec(m->fileno,SCRNvirtualSubData,m->searchrsrc);
			}
		}
	}

/* loop while movements reflect a record found */
	while(!f && !abort_diagnostic)
	{
/* clear screen virtuals so they may be recomputed for each record read */
		CLEAR_SCREEN_VIRTUAL(m->definelist);
		CLEAR_SCREEN_VIRTUAL(m->searchrsrc);
		SetRDArsrcFiles(m->fileno,m->definelist);
/*lint -e746 */
		breakout=TRUE;
		if(m->breakfunc!=NULL) breakout=m->breakfunc(m);
		if(!breakout) break;
		test_browse=test_browse_record(m,test_exp);
		if(!test_browse)
		{
			addit=FALSE;
		} else {
			select=TRUE;
			if(m->selectfunc!=NULL) select=m->selectfunc(m);
			if(!select)
			{
				addit=FALSE;
			} else {
				addit=TRUE;
			}
		}
/*lint +e746 */
		if(addit)
		{
			hold=Rmalloc(length+1);
			memset(hold,0,length+1);
			lcount=0;
			cur=0;
/* loop through the resources in the define list structure, to create the
	viewable browse list strings in the specified order */
			while(cur<(total_widgets))
			{
				for(x=1;x<(total_widgets);x+=2)
				{
/* getting the appropriate RDArmem structure for the position and length */ 
					pos_member=definelist->rscs+x;
					len_member=definelist->rscs+(x+1);
#ifdef USE_RDA_DIAGNOSTICS
					if(diagmix || diagmix_field)
					{
						prterr("DIAG makebrowselist() building browse list: testing pos_member of [%s] and len_member of [%s] from file [%s] for inclusion into the browse list.",pos_member->rscrname,len_member->rscrname,FILENAME(m->fileno));
					}
#endif /* USE_RDA_DIAGNOSTICS */
/* if position value is not zero, and there is a length, then add this 
	member to the list */
					if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
					{
/* this test ensures the resources get placed in the order specified by
	pos_member.  It is using an independent counter to handle 
	multiple resources having the same position order */
						if(*pos_member->value.short_value==cur)
						{
							s=stralloc(pos_member->rscrname);
							s[RDAstrlen(s)-9]=0;
#ifdef USE_RDA_DIAGNOSTICS
							if(diagmix || diagmix_field)
							{
								prterr("DIAG makebrowselist() building browse list: searching for element to put into browse list of [%s] from file of [%s].",s,FILENAME(m->fileno));
							}
#endif /* USE_RDA_DIAGNOSTICS */
/* finding appropriate field to be displayed */
							if(m->file_type==1) 
							{
								field=FLDNRD(m->fileno,s);
							} else {
								field=FLDNRDsec(m->fileno,s,TRUE,FALSE,FALSE);
							}
							if(field!=NULL)
							{
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case ZIPCODE:
									case SOCSECNUM:
									case DATES:
									case TIMES:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
										if(*len_member->value.short_value<RDAstrlen(field->data.string_value) && field->data.string_value!=NULL)
										{
											sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(field->data.string_value!=NULL ? field->data.string_value:""));
										} else {
											sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(field->data.string_value!=NULL ? field->data.string_value:""));
										}
										break;
									case DECIMALV:
										memset(strtemp,0,1024);
										sprintf(strtemp,"%*f",*len_member->value.short_value,*field->data.float_value);
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;
									case SDECIMALV:
										memset(strtemp,0,1024);
										if(*field->data.float_value>=0) 
										{
											sprintf(strtemp,"%*f ",*len_member->value.short_value-1,*field->data.float_value);
										} else {
											sprintf(strtemp,"%*f-",*len_member->value.short_value-1,(*field->data.float_value)*(-1));
										}
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;
									case SHORTV:
										memset(strtemp,0,1024);
										sprintf(strtemp,"%*d",*len_member->value.short_value,*field->data.short_value);
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;
									case SSHORTV:
										memset(strtemp,0,1024);
										if(*field->data.short_value>=0) 
										{
											sprintf(strtemp,"%*d ",*len_member->value.short_value-1,*field->data.short_value);
										} else {
											sprintf(strtemp,"%*d-",*len_member->value.short_value-1,(*field->data.short_value)*(-1));
										}
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;

									case LONGV:
										memset(strtemp,0,1024);
										sprintf(strtemp,"%*d",*len_member->value.short_value,*field->data.integer_value);
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;
									case SLONGV:
										memset(strtemp,0,1024);
										if(*field->data.integer_value>=0) 
										{
											sprintf(strtemp,"%*d ",*len_member->value.short_value-1,*field->data.integer_value);
										} else {
											sprintf(strtemp,"%*d-",*len_member->value.short_value-1,(*field->data.integer_value)*(-1));
										}
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;
									case DOLLARS_NOCENTS:
										temp=famtncents((*field->data.float_value)*100,*len_member->value.short_value,' ');
										sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case DOLLARS:
										temp=famt(*field->data.float_value,*len_member->value.short_value);
										sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case DOUBLEV:
										memset(strtemp,0,1024);
										sprintf(strtemp,"%*.0f",*len_member->value.short_value,*field->data.float_value);
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;
									case SDOUBLEV:
										memset(strtemp,0,1024);
										if(*field->data.float_value>=0)
										{
											sprintf(strtemp,"%*.0f ",*len_member->value.short_value-1,*field->data.float_value);
										} else {
											sprintf(strtemp,"%*.0f-",*len_member->value.short_value-1,(*field->data.float_value)*(-1));
										}
										strtemp[*len_member->value.short_value]=0;
										sprintf(&hold[lcount],"%s ",strtemp);
										break;
									case BOOLNS:
										if(*len_member->value.short_value>2)
										{
											sprintf(&hold[lcount],"%-s%*s ",(*field->data.string_value ? "Yes":"No "),(*len_member->value.short_value-3),"");
										} else {
											sprintf(&hold[lcount],"%-s%*s ",(*field->data.string_value ? "Y":"N"),(*len_member->value.short_value-1),"");
										}
										break;
									case CHARACTERS:
										sprintf(&hold[lcount],"%c ",*field->data.string_value);
										break;
									case BUTTONS:
									case SCROLLEDLIST:
										prterr("Error Field Type [%d] [%s] for Field [%s] not available on Browse Screen.",field->type,standardfieldtypes[field->type],field->name);
										break;
									default:
										prterr("Error Field Type [%d] is invalid for Field [%s] not available on Browse Screen.",field->type,field->name);
										break;
								}
								lcount+=*len_member->value.short_value+1;
							} else {
/* Searching for appropriate virtual field to be displayed on browse list */
#ifdef USE_RDA_DIAGNOSTICS
								if(diagmix || diagmix_field)
								{
									prterr("DIAG makebrowselist() building browse list: element of [%s] not found in file [%s], searching virtual fields.",s,FILENAME(m->fileno));
								}
#endif /* USE_RDA_DIAGNOSTICS */
								if(m->definelist->virflds!=NULL)
								{
									for(z=0,v=m->definelist->virflds;
										z<m->definelist->num;++z,++v)
									{
										if(!RDAstrcmp(v->name,s)) break;
									}
									if(z<m->definelist->num)
									{
										if(!v->computed)
										{
											computevirtual(v,SCRNvirtualSubData,
												m->definelist);
										}
										switch(v->type)
										{
											case SCROLLEDTEXT:
											case VARIABLETEXT:
											case PLAINTEXT:
											case PHONE:
											case CUSTOMTYPE:
											case ZIPCODE:
											case SOCSECNUM:
											case DATES:
											case TIMES:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
												if(*len_member->value.short_value<v->dlen && v->value.string_value!=NULL)
												{
													sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(v->value.string_value!=NULL ? v->value.string_value:""));
												} else {
													sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(v->value.string_value!=NULL ? v->value.string_value:""));
												}
												break;
											case DECIMALV:
												memset(strtemp,0,1024);
												sprintf(strtemp,"%*f",*len_member->value.short_value,*v->value.float_value);
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case SDECIMALV:
												memset(strtemp,0,1024);
												if(*v->value.float_value>=0) 
												{
													sprintf(strtemp,"%*f ",*len_member->value.short_value-1,*v->value.float_value);
												} else {
													sprintf(strtemp,"%*f-",*len_member->value.short_value-1,(*v->value.float_value)*(-1));
												}
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case SHORTV:
												memset(strtemp,0,1024);
												sprintf(strtemp,"%*d",*len_member->value.short_value,*v->value.short_value);
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case SSHORTV:
												memset(strtemp,0,1024);
												if(*v->value.short_value>=0) 
												{
													sprintf(strtemp,"%*d ",*len_member->value.short_value-1,*v->value.short_value);
												} else {
													sprintf(strtemp,"%*d-",*len_member->value.short_value-1,(*v->value.short_value)*(-1));
												}
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case LONGV:
												memset(strtemp,0,1024);
												sprintf(strtemp,"%*d",*len_member->value.short_value,*v->value.integer_value);
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case SLONGV:
												memset(strtemp,0,1024);
												if(*v->value.integer_value>=0)
												{
													sprintf(strtemp,"%*d ",*len_member->value.short_value-1,*v->value.integer_value);
												} else {
													sprintf(strtemp,"%*d-",*len_member->value.short_value-1,(*v->value.integer_value)*(-1));
												}
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case DOLLARS_NOCENTS:
												temp=famtncents((*v->value.float_value)*100,*len_member->value.short_value,' ');
												sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
												if(temp!=NULL) Rfree(temp);
												break;
											case DOLLARS:
												temp=famt(*v->value.float_value,*len_member->value.short_value);
												sprintf(&hold[lcount],"%-*s ",((*len_member->value.short_value)-1),temp);
												if(temp!=NULL) Rfree(temp);
												break;
											case DOUBLEV:
												memset(strtemp,0,1024);
												sprintf(strtemp,"%*.0f",*len_member->value.short_value,*v->value.float_value);
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case SDOUBLEV:
												memset(strtemp,0,1024);
												if(*v->value.float_value>=0)
												{
													sprintf(strtemp,"%*.0f ",*len_member->value.short_value-1,*v->value.float_value);
												} else {
													sprintf(strtemp,"%*.0f-",*len_member->value.short_value-1,(*v->value.float_value)*(-1));
												}
												strtemp[*len_member->value.short_value]=0;
												sprintf(&hold[lcount],"%s ",strtemp);
												break;
											case BOOLNS:
												if(*len_member->value.short_value>2)
												{
													sprintf(&hold[lcount],"%-s%*s ",(*v->value.string_value ? "Yes":"No "),(*len_member->value.short_value-3),"");
												} else {
													sprintf(&hold[lcount],"%-s%*s ",(*v->value.string_value ? "Y":"N"),(*len_member->value.short_value-1),"");
												}
/*
												sprintf(&hold[lcount],"%c ",(*v->value.string_value ? 'Y':'N'));
*/
												break;
											case CHARACTERS:
												sprintf(&hold[lcount],"%c ",*v->value.string_value);
												break;
											case BUTTONS:
											case SCROLLEDLIST:
												prterr("Error Field Type [%d] [%s] for Virtual Field [%s] not available on Browse Screen.",v->type,standardfieldtypes[v->type],v->name);
												break;
											default:
												prterr("Error Field Type [%d] is invalid for Virtual Field [%s] not available on Browse Screen.",v->type,v->name);
												break;
										}
										lcount+=*len_member->value.short_value+1;
									} else {
										prterr("Error field [%s] not found in Module [%s] File [%s].",s,MODULENAME(m->fileno),FILENAME(m->fileno));
									}
								} else {
									prterr("Error field [%s] not found in Module [%s] File [%s].",s,MODULENAME(m->fileno),FILENAME(m->fileno));
								}
							}
							if(s!=NULL) Rfree(s);
						}
					}
				}
				++cur;
			}
/* list item variable "hold" has been established and is added to the 
	RDAbrowse structure */
			if(!isEMPTY(m->query))
			{
				if(patmatch(m->query,hold))
				{
					addRDAbrowse(m->list,hold);
					IncrementBrowseListTotals(m->mainrsrc,m->fileno,m->definelist);
				} else if(RDAstrstr(hold,m->query)!=NULL)
				{
					addRDAbrowse(m->list,hold);
					IncrementBrowseListTotals(m->mainrsrc,m->fileno,m->definelist);
				}
			} else {
				addRDAbrowse(m->list,hold);
				IncrementBrowseListTotals(m->mainrsrc,m->fileno,m->definelist);
			}
			if(hold!=NULL) Rfree(hold);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		} else {
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
		}
/* reading the next record in sorted sequence to be displayed in the browse  */
		if(m->passkey!=NULL)
		{
			if(m->passkey->fields!=NULL)
			{
				if(m->file_type==1) 
				{
					f=NXTNRD(m->fileno,m->keyno);
				} else {
					f=ADVNXTNRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
				}
			} else {
				if(keyrange)
				{
					if(m->file_type==1) 
					{
						f=NXTNRD(m->fileno,m->keyno);
					} else {
						f=ADVNXTNRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
					}
					if(!f)
					{
						f=testfilekeyTOrangersrc(m->fileno,m->keyno,m->rs);
					}
				} else {
					if(m->file_type==1) 
					{
						f=SEQNRD(m->fileno);
					} else {
						f=ADVSEQNRDsec(m->fileno,SCRNvirtualSubData,m->searchrsrc);
					}
				}
			}
/*lint -e746 */
		} else if(m->initfunc!=NULL)
		{
/*lint +e746 */
			if(m->file_type==1) 
			{
				f=NXTNRD(m->fileno,m->keyno);
			} else {
				f=ADVNXTNRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
			}
		} else { 
			if(keyrange)
			{
				if(m->file_type==1) 
				{
					f=NXTNRD(m->fileno,m->keyno);
				} else {
					f=ADVNXTNRDsec(m->fileno,m->keyno,SCRNvirtualSubData,m->searchrsrc);
				}
				if(!f)
				{
					f=testfilekeyTOrangersrc(m->fileno,m->keyno,m->rs);
				}
			} else {
				if(m->file_type==1) 
				{
					f=SEQNRD(m->fileno);
				} else {
					f=ADVSEQNRDsec(m->fileno,SCRNvirtualSubData,m->searchrsrc);
				}
			}
		}
	}
	sortRDAbrowse(m->list);
	list=m->list;
	if(list->string==NULL)
	{
/* Establishing the browse structure that has found no records available */
		list->key=NULL;
		list->keyvalue=NULL;
       		addRDAbrowse(list,"***************    No Records Selected from current file.   ***************");
	} else {
		list->keyvalue=list->key[0];
	}
	m->list=list;
	if(test_exp!=NULL) Rfree(test_exp);
	FreeAndSetDFincvir(m);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field) seeRDAbrowse(m->list);
#endif /* USE_RDA_DIAGNOSTICS */
	label=getfilerangedesc((short)m->fileno,m->rs);
/* updating the SEARCH CRITERIA from the specified values on Search Browse Screen */
	if(!FINDRSCSETSTRING(m->mainrsrc,"SEARCH CRITERIA",label))
	{
		updatersrc(m->mainrsrc,"SEARCH CRITERIA");
	}
	for(x=1,length=0;x<(total_widgets);x+=2)
	{
		pos_member=m->definelist->rscs+x;
		len_member=m->definelist->rscs+(x+1);
		if(*pos_member->value.short_value>0)
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	bl_desc=make_definelist_string(m->definelist,length,&s);
	if(!FINDRSCSETSTRING(m->mainrsrc,"BROWSE LIST DESCRIPTION",bl_desc)) 
	{
		updatersrc(m->mainrsrc,"BROWSE LIST DESCRIPTION");
	}
	if(bl_desc!=NULL) Rfree(bl_desc);
	if(s!=NULL) Rfree(s);
	if(label!=NULL) Rfree(label);
}
void runokfunc(RDArsrc *parent,MakeBrowseList *b)
{
/* executes the OKFUNC when pressing the SELECT button */
	RDAbrowse *list;
	char **key;
	int sellist=0;

	list=b->list;

	key=(char **)list->key;
/* getting the current value of BROWSE LIST */
	FINDRSCGETINT(b->mainrsrc,"BROWSE LIST",&sellist);
	if(sellist>=list->num)
	{
		prterr("Error: Selected Browse Element [%d] isn't in actual Browse List.  Resetting list to first element in Browse List, element number ZERO (0).",sellist);
	}
	list->keyvalue=key[sellist];

	if(b->okfunction!=NULL)
	{
/*lint -e746 */
		if(b->okfunction!=NULL) b->okfunction(b,list->keyvalue);
/*lint +e746 */
	}
}
void readlist(RDArsrc *parent,MakeBrowseList *b)
{
/* readlist function is executed when selecting off the define list screen,
	this function read's the define list screen, then calls makebrowse
	to re-create the RDAbrowse structure.  Then updates the Browse window
	with the new information */

	RDAbrowse *list;
	int valuex=0,x=0;
	int listvalue=0;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v=NULL;

/* reading the BROWSE LIST's selected value */
	FINDRSCGETINT(b->mainrsrc,"BROWSE LIST",&valuex);
/* reading the entire define list screen */
	readallwidgets(b->definelist);
/* getting the key value from the define list screen */
	FINDRSCGETINT(b->definelist,"KEY LIST",&listvalue);
	b->keyno=listvalue+1;
	if(b->rs!=NULL) 
	{
/* Allocating a RangeScreen structure for the Search Browse Screen */
		b->rs=RangeScreenNew(b->searchrsrc);
/* Reading the Search Browse Screen establishing the Range Screen Structure */
		xReadRangeScreen(b->fileno,b->rs,b->file_type,__LINE__,__FILE__);
		if(b->searchrsrc->virflds!=NULL)
		{
			for(x=0,v=b->searchrsrc->virflds;x<b->searchrsrc->num;++x,
				++v)
			{
				ReadVirtualRangeStruct(v,b->rs);
			}
		}
	}
/* creating a diagnostic screen's RDArsrc pointer */
	if(USE_DIAGNOSTIC_SCREENS)
	{
		tmprsrc=diagscrn(b->fileno,"DIAGNOSTIC SCREEN",(char *)b->searchrsrc->module,"Browse Diagnostic",stop_diagnostic);
	}else{
		tmprsrc=NULL;
	}

	if(tmprsrc!=NULL)
	{
/* creating the diagnostic screen's window */
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED.","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that this screen is available in this module.  If not, call your installer.\n",NULL,NULL,TRUE,NULL);
			prterr("Error Cannot Create Screen [%s] [%s].",tmprsrc->module,tmprsrc->screen);
		} else {
			SETTITLE(tmprsrc,"DEFINE LIST DIAG");
			ForceWindowUpdate(tmprsrc);
		}
	}
/* reading file and establishing new values in the RDAbrowse structure */
	xmakebrowselist(b,tmprsrc);
	list=b->list;
	list->keyvalue=list->key[0];
	if(list->num<=valuex) valuex=0;
/* updating the BROWSE LIST with new information */
	if(!FINDRSCSETLIST(b->mainrsrc,"BROWSE LIST",valuex,list->num,&list->string)) 
	{
		updatersrc(b->mainrsrc,"BROWSE LIST");
	}
	if(tmprsrc!=NULL)
	{
		if(!abort_diagnostic) killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	ForceWindowUpdate(b->definelist);
	NULL_RSCS(b->definelist);
}
void dosearchquit(RDArsrc *parent,Browse_Exit *BE)
{
	MakeBrowseList *b=NULL;
	
	b=BE->b;
	if(b!=NULL)
	{
		switch(b->quit_type)
		{
			case 0: /* normal button */
				kw_browse_exit(parent,BE);
				break;
			case 1: /* kill window */
				kw_browse_exit(parent,BE);
				break;
			default:
			case 2: /* exit window */
				browse_exit(parent,BE);
				break;
		}
	}
}
void dosearchinit(MakeBrowseList *b,Browse_Exit *BE)
{
/* this function is used to create the Previously define Search Browse Screen*/
	RDArsrc *srsrc=NULL;
/*
	MakeBrowseList *b=NULL;
*/

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Creating screen for obtaining Browse selection criteria.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	srsrc=b->searchrsrc;
	if(IsScreenDisplayed(srsrc)) return;
	FINDRSCSETFUNC(b->searchrsrc,"QUIT",dosearchquit,BE);
	if(ADVmakescrn(b->searchrsrc,TRUE)) 
	{
		prterr("Error Cannot Create Screen [%s] [%s].",b->searchrsrc->module,b->searchrsrc->screen);
	}
	setfile2rsrccallbacks(b->fileno,b->searchrsrc,b->file_type);
}
void dosearch(RDArsrc *parent,MakeBrowseList *b)
{
/* this function is used to create the Previously define Search Browse Screen*/
	RDArsrc *srsrc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Creating screen for obtaining Browse selection criteria.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	srsrc=b->searchrsrc;
	if(IsScreenDisplayed(srsrc)) return;
	FINDRSCSETFUNC(b->searchrsrc,"QUIT",NULL_RSCS,NULL);
	if(ADVmakescrn(b->searchrsrc,TRUE)) 
	{
		prterr("Error Cannot Create Screen [%s] [%s].",b->searchrsrc->module,b->searchrsrc->screen);
	}
	setfile2rsrccallbacks(b->fileno,b->searchrsrc,b->file_type);
}
short alreadyrsrc(char *fname,RDArsrc *mainrsrc)
{
/* this function returns TRUE a widget with name, fname, already exists on the specified resource, mainrsrc */
	int x=0;
	RDArmem *member;

	if(mainrsrc!=NULL)
	{
		if(mainrsrc->rscs!=NULL)
		{
			for(x=0,member=mainrsrc->rscs;x<mainrsrc->numrscs;++x,++member)
			{
				if(!RDAstrcmp(member->rscrname,fname)) return(TRUE);
			}
		}
	}
	return FALSE;
}
short alreadyrsrctopofbrowse(char *fname,RDArsrc *mainrsrc)
{
/* this is a hybrid function of alreadyrsrc, used specifically on the browse 
function, this one keeps keys in the detail file from being repeated if they 
are also available in a resource at the top of the browse */
	int x=0;
	RDArmem *member;
	char *gf=NULL;

	if(mainrsrc!=NULL)
	{
		if(mainrsrc->rscs!=NULL)
		{
			for(x=0,member=mainrsrc->rscs;x<mainrsrc->numrscs;++x,++member)
			{
				gf=stripfilename(member->rscrname);
				if(!RDAstrcmp(fname,gf)) 
				{
					if(gf!=NULL) Rfree(gf);
					return TRUE;
				}
				if(gf!=NULL) Rfree(gf);
			}
		}
	}
	return FALSE;
}
void SaveDefaultsUsedOnly(RDArsrc *definelist)
{
/* This function is similar to SaveDefaults, only is specifically is used on
the define list screen, to save only those values visible, only those 
defined on the screen.  This has to be done so that boolean values that aren't
displayed don't inhibit the use of ADVbrowse. */
	int x;
	RDArmem *member;

	if(definelist!=NULL)
	{
	for(x=0,member=definelist->rscs;x<definelist->numrscs;++x,++member)
	{
		if(member->w==NULL)
		{
			switch(member->field_type)
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
					if(member->value.string_value!=NULL)
						Rfree(member->value.string_value);
					member->value.string_value=NULL;
					member->dlen=0;
					break;
				case BOOLNS:
					break;
				case CHARACTERS:
					*member->value.string_value=0;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					*member->value.float_value=0;
					break;
				case SHORTV:
				case SSHORTV:
					*member->value.short_value=0;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*member->value.integer_value=0;
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s].",member->field_type,member->rscrname,definelist->module,definelist->screen);
					break;
			}
		}
	}
	}
/* calls standard SaveDefaults function after zero'ing unused fields */
	SaveDefaults(definelist);
}
APPlib *mk_define_list(RDArsrc *definelist,short nofields,NRDfield *fields,
	short nokeys,NRDkey *keys,RDArsrc *mainrsrc,short keyno)
{
/* this function is used to create a define list RDArsrc structure for a
	specific file.  It creates two resources per field, one representing
	the POSITION and the other the LENGTH to be displayed in the 
	Browse List.  Also created are the Buttons used to control the screen*/
	short x,count;
	NRDfield *field;
	NRDkey *key;
	char *s;
	APPlib *keysavl=NULL;
	RDAvirtual *v;
	int e;
	short l=0,value_offset=0,value_length=0,tw=0;
	int y=0,nokeysfound=0;
	NRDpart *keypart=NULL;
	RDAdefault *d=NULL;
	char has_defaults=TRUE;
	

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Creating the Define List Resources for [%d] fields.",nofields);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	keysavl=APPlibNEW();
/* creating APPlib structure for keys available */
	for(x=0,key=keys;x<nokeys;++x,++key) 
	{
		addAPPlib(keysavl,key->name);
	}
	e=keyno-1;
/* creating KEY LIST resource */
	addlstrsrc(definelist,"KEY LIST",&e,FALSE,NULL,keysavl->numlibs,
		&keysavl->libs,NULL);
	count=1;
/* looping through available fields creating POSITION and LENGTH resources */
	nokeysfound=0;
	if(XPERT_SETUP->ShowDefineListNew==FALSE)
	{
		
		d=finddefaultbin(definelist->module,definelist->screen);
		if(d!=NULL) 
		{
			has_defaults=TRUE;
			FreeRDAdefault(d);
		} else has_defaults=FALSE;
	}
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		if(XPERT_SETUP->ShowDefineListNew==TRUE)
		{
			value_offset=count;
			value_length=field->len;
		} else {
			if(has_defaults==TRUE)
			{
				value_offset=0;
				value_length=field->len;
			} else {
				FINDRSCGETINT(definelist,"KEY LIST",&e);
				if(e<nokeys) key=(keys+e);
				else key=(keys+0);
				if(key->numparts>0)
				{
					if(nokeysfound==key->numparts)
					{
						if((RDAstrstr(field->name,"NAME"))!=NULL)
						{
							value_offset=count;
							if(field->len>20) value_length=20;
							else value_length=field->len;
						} else if((RDAstrstr(field->name,"DESCRIPTION"))!=NULL)
						{
							value_offset=count;
							if(field->len>20) value_length=20;
							else value_length=field->len;
						} else { 
							value_offset=0;
							value_length=field->len;
						}
					} else {
						for(y=0,keypart=key->part;y<key->numparts;++y,++keypart) 
						{
							value_offset=0;
							value_length=field->len;
							if(!RDAstrcmp(field->name,keypart->name))
							{
								value_offset=(y+1);
								nokeysfound+=1;
								value_length=field->len;
								break;
							}
						}
						if(y==key->numparts)
						{
							if((RDAstrstr(field->name,"NAME"))!=NULL)
							{
								value_offset=count;
								if(field->len>20) value_length=20;
								else value_length=field->len;
							} else if((RDAstrstr(field->name,"DESCRIPTION"))!=NULL)
							{
								value_offset=count;
								if(field->len>20) value_length=20;
								else value_length=field->len;
							} else { 
								value_offset=0;
								value_length=field->len;
							}
						} 
					}
				} else {
					value_offset=0;
					value_length=field->len;
				}
			}
		}
		l=RDAstrlen(field->name)+10;
		s=Rmalloc(l);
		sprintf(s,"%s POSITION",field->name);
		tw=FINDRSC(definelist,s);
		if(tw==(-1))
		{
		addstdrsrc(definelist,s,SHORTV,3,&value_offset,TRUE);	
		Rfree(s);
		l=RDAstrlen(field->name)+8;
		s=Rmalloc(l);
		sprintf(s,"%s LENGTH",field->name);
		l=field->len;
		if(field->type==SCROLLEDTEXT || alreadyrsrctopofbrowse(field->name,mainrsrc) || !RDAstrcmp(field->name,"DELETEFLAG")) value_length=0;
		if(value_length>40) value_length=40;
		addstdrsrc(definelist,s,SHORTV,3,&value_length,TRUE);	
		++count;
		}
		Rfree(s);
	}
/* looping through available virtuals creating POSITION and LENGTH resources */
	if(definelist->virflds!=NULL)
	{
		for(x=0,v=definelist->virflds;x<definelist->num;++x,++v)
		{
			l=RDAstrlen(v->name)+10;
			s=Rmalloc(l);
			sprintf(s,"%s POSITION",v->name);
			tw=FINDRSC(definelist,s);
			if(tw==(-1))
			{
			
			addstdrsrc(definelist,s,SHORTV,3,&count,TRUE);	
			Rfree(s);
			l=RDAstrlen(v->name)+8;
			s=Rmalloc(l);
			sprintf(s,"%s LENGTH",v->name);
			l=v->len;
			if(v->type==SCROLLEDTEXT || 
				alreadyrsrctopofbrowse(v->name,mainrsrc)) l=0;
			if(l>40) l=40;
			addstdrsrc(definelist,s,SHORTV,3,&l,TRUE);	
			++count;
			}
			Rfree(s);
		}	
	}
/* creating Appropriate control buttons for Define List Screen */
	addbtnrsrc(definelist,"DEFAULTS",TRUE,SaveDefaultsUsedOnly,NULL);
	addrfkwrsrc(definelist,"SELECT",TRUE,NULL,NULL);
	addrfkwrsrc(definelist,"QUIT",TRUE,NULL_RSCS,NULL);
	addrfcbrsrc(definelist,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(definelist,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(definelist,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
/* Setting Defaults for the Define List Screen */
	DefaultScreens(definelist);
	FINDRSCSETINT(definelist,"KEY LIST",e);
/* returning a pointer to the APPlib structure used to create the KEY LIST */
	return(keysavl);
}

void define_list(RDArsrc *parent,Define_List *d)
{
/* this function is used to create the define list screen */
	RDArsrc *drsrc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("Creating Define List Screen.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	drsrc=d->definelist;
	if(IsScreenDisplayed(drsrc)) return;
	if(ADVmakescrn(d->definelist,TRUE)) 
	{
		prterr("Error Cannot Create Screen [%s] [%s].",d->definelist->module,d->definelist->screen);
	}
}
void browse_function(RDArsrc *parent,Browse_Function *bf)
{
/* this function is used to execute a list_callback function or function
	associated with the list, and set the RDAbrowse structure's keyvalue
	to the current highlighted element.  If you want a browse to execute
	a specific function when selecting a element from the BROWSE LIST
	this is where that function will be executed */
	RDAbrowse *b;
	int list=0;
	char **key;

	b=bf->list;
/* getting the current value of BROWSE LIST */
	FINDRSCGETINT(bf->mainrsrc,"BROWSE LIST",&list);
	FINDRSCSETINT(bf->mainrsrc,"BROWSE LIST POSITION",list+1);
	updatersrc(bf->mainrsrc,"BROWSE LIST POSITION");
	key=(char **)b->key;
/* settting the appropriate keyvalue in the RDAbrowse */
	b->keyvalue=key[list];
/* executing any list callback function if appropriate */
	if(b->listfunc!=NULL)
	{
/*lint -e746 */
		b->listfunc(bf);
/*lint +e746 */
	}
	bf->list=b;
}
void kw_browse_exit(RDArsrc *parent,Browse_Exit *BE)
{
/* executing the function associated with the QUIT button when the Quit button
	is established as a Kill window type */
	if(BE->doexit!=NULL)
	{
/*lint -e746 */
		BE->doexit(BE->b);
/*lint +e746 */
	}
}
void proc_browse_exit(Browse_Exit *b,void (*func)(...))
{
/* this function processing the Exit Browse, freeing all private data used by 
the browse function and running the a specified function.  The function 
executed here depends on which button was pushed.  This function get executed 
for all buttons that are clicked, which upon there completion should exit 
Browse */	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG browse_exit Exiting Browse Function.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(b!=NULL)
	{
		if(func!=NULL) 
		{
/*lint -e746 */
			func(b->b);
/*lint +e746 */
		}
		if(b->keysavl!=NULL) freeapplib(b->keysavl);
		if(b->b!=NULL)
		{
			if(b->b->list!=NULL) RDAbrowseFREE(b->b->list);
			if(b->b->rs!=NULL) FreeRangeScreen(b->b->rs);
			if(b->b->query!=NULL) Rfree(b->b->query);
			Rfree(b->b);
		}
		b->b=NULL;
		if(b->l!=NULL) Rfree(b->l);
		if(b->bf!=NULL) Rfree(b->bf);
		b->l=NULL;
		Rfree(b);
		b=NULL;
	}
}
void browse_exit(RDArsrc *parent,Browse_Exit *b)
{
/* this function get executed when the QUIT button is set to Exit upon completion */
	proc_browse_exit(b,b->doexit);
}
void add_browse_exit(RDArsrc *parent,Browse_Exit *BE)
{
/* this function get's executed by the ADD button when the ADD is supposed to
exit upon completion */
	if(BE->b->new_record!=NULL)
	{
/*lint -e746 */
		BE->b->new_record(parent,BE->b);
/*lint +e746 */
	}
	proc_browse_exit(BE,NULL);
}
void select_browse_exit(RDArsrc *parent,Browse_Exit *BE)
{
/* this function get's executed when the SELECT button is supposed to exit upon completion */
	runokfunc(parent,BE->b);
	proc_browse_exit(BE,NULL);
}
void dosmake(RDArsrc *parent,MakeBrowseList *b)
{
/* this function is used when pressing SELECT off the Search Browse window. 
	This function reads the new contents from the Search Browse window
	and then calls makebrowselist to reread the datafile, and create the
	appropriate RDAbrowse structure. */

	RDAbrowse *list;
	int valuex=0,x;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v=NULL;
	char *errordesc=NULL;
	int xlength=0;

/* reading the current BROWSE LIST value */
	ZeroBrowseListTotals(b->mainrsrc,b->fileno,b->definelist);
	FINDRSCGETINT(b->mainrsrc,"BROWSE LIST",&valuex);
	list=b->list;
/* reading the Range Screen Widgets */
	readallwidgets(b->rs->rsrc);
	readallwidgets(b->searchrsrc);
	GET_RANGESCREEN_EXPRESSION_VIRTUALS(b->searchrsrc);
	if(b->rs!=NULL) FreeRangeScreen(b->rs);
	b->rs=NULL;
/* reallocating a new range screen structure */
	b->rs=RangeScreenNew(b->searchrsrc);
/* establishing the range screen structure */
	FINDRSCGETSTRING(b->searchrsrc,"QUERY",&b->query);
	xReadRangeScreen(b->fileno,b->rs,b->file_type,__LINE__,__FILE__);
	if(b->searchrsrc->virflds!=NULL)
	{
		for(x=0,v=b->searchrsrc->virflds;x<b->searchrsrc->num;++x,
			++v)
		{
			ReadVirtualRangeStruct(v,b->rs);
		}
	}
/* creating a RDArsrc * for the diagnostic screen */
	if(USE_DIAGNOSTIC_SCREENS)
	{
		tmprsrc=diagscrn(b->fileno,"DIAGNOSTIC SCREEN",b->searchrsrc->module,"Browse Diagnostic",stop_diagnostic);
	}else{
		tmprsrc=NULL;
	}

	if(tmprsrc!=NULL)
	{
/* creating the diagnostic screen */
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED.","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that this screen is available in this module.  If not, call your installer.\n",NULL,NULL,TRUE,NULL);
			prterr("Error Cannot Create Screen [%s] [%s].",tmprsrc->module,tmprsrc->screen);
		} else {
			SETTITLE(tmprsrc,"SEARCH BROWSE DIAG");
			ForceWindowUpdate(tmprsrc);
		}
	}
/* re-reading the datafile and establishing new values in the RDAbrowse structure */
	xmakebrowselist(b,tmprsrc);
	list->keyvalue=list->key[0];
	b->list=list;
	if(valuex>=list->num) valuex=0;
/* updating the BROWSE LIST */
	if(b->start_with==2)
	{
		if(b->mainrsrc->swidget==NULL)
		{
			if(ADVmakescrn(b->mainrsrc,(b->modal_b ? TRUE:FALSE))) 
			{
				xlength=(RDAstrlen(b->mainrsrc->screen)+RDAstrlen(b->definelist->screen)+RDAstrlen(b->searchrsrc->screen)+90);
				errordesc=Rmalloc(xlength);
				sprintf(errordesc,"The Browse Utility has failed.  Verify that screens:\n\t[%s]\n\t[%s]\n\t[%s]\n are on file.",b->mainrsrc->screen,b->definelist->screen,b->searchrsrc->screen);
				ERRORDIALOG("Browse Data File Failed",errordesc,b->doexit,TRUE);
				if(errordesc!=NULL) Rfree(errordesc);
				errordesc=NULL;
				FREE_BROWSE_EXIT(b);
			}
		}
	}
	if(!FINDRSCSETLIST(b->mainrsrc,"BROWSE LIST",valuex,list->num,&list->string))
	{
		updatersrc(b->mainrsrc,"BROWSE LIST");
	}
	if(tmprsrc!=NULL)
	{
		if(!abort_diagnostic) killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	ForceWindowUpdate(b->searchrsrc);
	NULL_RSCS(b->searchrsrc);
}
void search_field_func(RDArsrc *mainrsrc,MakeBrowseList *b)
{
/* this function is used to locate a specific BROWSE LIST element, by searching the list for the 1st item containing the information int he SEARCH FIELD widget */
	RDAbrowse *list;
	char *valuex=NULL;
	int x,current_value=0;
	Browse_Function *BF=NULL;

/* reading BROWSE LIST for current element */
/* reading SEARCH  FIELD to get target string */
	readwidget(mainrsrc,"SEARCH FIELD");
	FINDRSCGETSTRING(mainrsrc,"SEARCH FIELD",&valuex);
	FINDRSCGETINT(mainrsrc,"BROWSE LIST",&current_value);
	if(!isEMPTY(valuex))
	{
		list=b->list;
/* if you are at the end, start looking at the top */
		if((current_value+1)>=list->num) current_value=(-1);
/* starting from current location, find first BROWSE LIST element, containing the target string */
		for(x=(current_value+1);x<list->num;++x)
		{
			if(patmatch(valuex,list->string[x]))
			{
				if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
				{
					updatersrc(mainrsrc,"BROWSE LIST");	
					Rfree(valuex);
					BF=Rmalloc(sizeof(Browse_Function));
					BF->mainrsrc=mainrsrc;	
					BF->list=b->list;
					BF->okfunction=b->okfunction;
					browse_function(mainrsrc,BF);
					if(BF!=NULL) Rfree(BF);
					return;
				}
			} else if(RDAstrstr(list->string[x],valuex)!=NULL)
			{
/* target found in BROWSE LIST element x, updating BROWSE LIST accordingly  */
				if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
				{
					updatersrc(mainrsrc,"BROWSE LIST");	
					Rfree(valuex);
					BF=Rmalloc(sizeof(Browse_Function));
					BF->mainrsrc=mainrsrc;	
					BF->list=b->list;
					BF->okfunction=b->okfunction;
					browse_function(mainrsrc,BF);
					if(BF!=NULL) Rfree(BF);
					return;
				}
			} 
		}
		for(x=0;x<(current_value);++x)
		{
			if(patmatch(valuex,list->string[x]))
			{
/* target found in BROWSE LIST element x, updating BROWSE LIST accordingly  */
				if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
				{
					updatersrc(mainrsrc,"BROWSE LIST");	
					Rfree(valuex);
					BF=Rmalloc(sizeof(Browse_Function));
					BF->mainrsrc=mainrsrc;	
					BF->list=b->list;
					BF->okfunction=b->okfunction;
					browse_function(mainrsrc,BF);
					if(BF!=NULL) Rfree(BF);
					return;
				}
			} else if(RDAstrstr(list->string[x],valuex)!=NULL)
			{
/* target found in BROWSE LIST element x, updating BROWSE LIST accordingly  */
				if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
				{
					updatersrc(mainrsrc,"BROWSE LIST");	
					Rfree(valuex);
					BF=Rmalloc(sizeof(Browse_Function));
					BF->mainrsrc=mainrsrc;	
					BF->list=b->list;
					BF->okfunction=b->okfunction;
					browse_function(mainrsrc,BF);
					if(BF!=NULL) Rfree(BF);
					return;
				}
			} 
		}
/* target not found, setting BROWSE LIST to last element in list */
		x=current_value;
		if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
			updatersrc(mainrsrc,"BROWSE LIST");
		Rfree(valuex);
	} else {
/* when target is NULL, set BROWSE LIST to the 1st element */
		if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",current_value))
			updatersrc(mainrsrc,"BROWSE LIST");
	}
	BF=Rmalloc(sizeof(Browse_Function));
	BF->mainrsrc=mainrsrc;	
	BF->list=b->list;
	BF->okfunction=b->okfunction;
	browse_function(mainrsrc,BF);
	if(BF!=NULL) Rfree(BF);
}
static void AddHoldValues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG addholdvalues Module [%s] File [%s] ",modulex,filex);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!GETBIN(modulex,filex,&engine,&nofields,&fields,
		&nokeys,&keys)) 
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
			Rfree(keys);
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+
							RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					} else 	{
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modulex,
						filex,f->name);
					addAPPlib(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
static APPlib *MakeFieldValList(RDArsrc *r)
{
	APPlib *tmp=NULL,*virflist=NULL;
	char addvf=FALSE,*libx=NULL;
	int x,y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;

	tmp=APPlibNEW();
	AddRDAGenericSetup2APPlib(tmp);
	if(r->incvir!=NULL)
	{
		for(x=0,i=r->incvir;x<r->numvir;++x,++i)
		{
			AddHoldValues(tmp,i->module,i->file);
		}
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(r->module)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,(r->module==NULL?"":r->module));
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,(r->module==NULL?"":r->module));
#endif
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(r->module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=r->incvir;x<r->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=r->numvir) 
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
					addAPPlibNoDuplicates(tmp,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	addAPPlibNoDuplicates(tmp,"[DEV_LICENSE]");
	addAPPlibNoDuplicates(tmp,"[SIMPLE SCREENS]");
	addAPPlibNoDuplicates(tmp,"[SYSTEM DATE]");
	addAPPlibNoDuplicates(tmp,"[SYSTEM DATE-YYYY]");
	addAPPlibNoDuplicates(tmp,"[SYSTEM TIME]");
	addAPPlibNoDuplicates(tmp,"[SYSTEM TYPE]");
	addAPPlibNoDuplicates(tmp,"[CURRENT DIRECTORY]");
	addAPPlibNoDuplicates(tmp,"[CURRENT PROCESS ID]");
	addAPPlibNoDuplicates(tmp,"[DEFAULT PRINTER]");
	addAPPlibNoDuplicates(tmp,"[LOGIN IDENTIFICATION]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION NAME]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION ADDRESS #1]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION ADDRESS #2]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION CITY]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION STATE]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION ZIP]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION PHONE]");
	addAPPlibNoDuplicates(tmp,"[STARTING FISCAL MONTH]");
	addAPPlibNoDuplicates(tmp,"[USERS DEPARTMENT]");
	addAPPlibNoDuplicates(tmp,"[USERS SITE IDENTIFICATION]");
	addAPPlibNoDuplicates(tmp,"[USERS SUPERVISOR]");
	addAPPlibNoDuplicates(tmp,"[USE BROWSE LIST LABEL]");
	addAPPlibNoDuplicates(tmp,"[USERS SCAN DIRECTORY]");
	addAPPlibNoDuplicates(tmp,"[USERS PHONE]");
	addAPPlibNoDuplicates(tmp,"[USERS PHONE EXTENSION]");
	addAPPlibNoDuplicates(tmp,"[USERS EMAIL]");
	SORTAPPlib(tmp);
	return(tmp);
}
void BrowseLoadValue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load=NULL;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=MakeFieldValList(r);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	for(x=0;x<loadlist->numlibs;++x)
	{
		if(strncmp(loadlist->libs[x],"GENERIC_SETUP",13)) break;
	}
	if(x>=loadlist->numlibs) x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
#ifdef __COMPETES_WITH_FOCUS_CALLBACKS__
static void QueryFunc(RDArsrc *s)
{
	RDArmem *member=NULL;
	int x=0;

	if(s!=NULL)
	{
		for(x=0,member=s->rscs;x<s->numrscs;++x,++member)
		{
			if(!RDAstrcmp(member->rscrname,"SELECT"))
			{
				break;
			}
		}
		if(!RDAstrcmp(member->rscrname,"SELECT"))
		{
			ExecuteRDArmemFunction(member);
			killwindow(s);
		}
		
	}	
}
#endif /* __COMPETES_WITH_FOCUS_CALLBACKS__ */
void ClearSearchLPM(RDArsrc *parent)
{
	char *temp=NULL;

	FINDRSCSETSTRING(parent,"SEARCH FIELD",temp);
	FINDRSCSETSTRING(parent,"BROWSE LIST LPM",temp);
	updatersrc(parent,"SEARCH FIELD");
	updatersrc(parent,"BROWSE LIST LPM");
}
void gototop(RDArsrc *parent,MakeBrowseList *b)
{
	int x=0;
	Browse_Function *BF=NULL;

	FINDRSCSETINT(parent,"BROWSE LIST",x);
	updatersrc(parent,"BROWSE LIST");
	BF=Rmalloc(sizeof(Browse_Function));
	BF->mainrsrc=parent;	
	BF->list=b->list;
	BF->okfunction=b->okfunction;
	browse_function(parent,BF);
	if(BF!=NULL) Rfree(BF);
}
void gotobottom(RDArsrc *parent,MakeBrowseList *b)
{
	int x=b->list->num-1;
	Browse_Function *BF=NULL;

	FINDRSCSETINT(parent,"BROWSE LIST",x);
	updatersrc(parent,"BROWSE LIST");
	BF=Rmalloc(sizeof(Browse_Function));
	BF->mainrsrc=parent;	
	BF->list=b->list;
	BF->okfunction=b->okfunction;
	browse_function(parent,BF);
	if(BF!=NULL) Rfree(BF);
}
void lpm_field_func(RDArsrc *mainrsrc,MakeBrowseList *b)
{
	RDAbrowse *list;
	char *valuex=NULL;
	int x,current_value=0;
	Browse_Function *BF=NULL;

/* reading BROWSE LIST for current element */
/* reading SEARCH  FIELD to get target string */
	readwidget(mainrsrc,"BROWSE LIST LPM");
	FINDRSCGETSTRING(mainrsrc,"BROWSE LIST LPM",&valuex);
	FINDRSCGETINT(mainrsrc,"BROWSE LIST",&current_value);
	list=b->list;
	if(!isEMPTY(valuex))
	{
		list=b->list;
/* if you are at the end, start looking at the top */
/* starting from current location, find first BROWSE LIST element, containing the target string */
		for(x=current_value;x<list->num;++x)
		{
			if(!strncmp(list->string[x],valuex,RDAstrlen(valuex)))
			{
/* target found in BROWSE LIST element x, updating BROWSE LIST accordingly  */
				if(x==current_value)
				{
					Rfree(valuex);
					return;
				}
				if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
				{
					updatersrc(mainrsrc,"BROWSE LIST");	
					Rfree(valuex);
					BF=Rmalloc(sizeof(Browse_Function));
					BF->mainrsrc=mainrsrc;	
					BF->list=b->list;
					BF->okfunction=b->okfunction;
					browse_function(mainrsrc,BF);
					if(BF!=NULL) Rfree(BF);
					return;
				}
			} 
		}
		for(x=0;x<current_value;++x)
		{
			if(!strncmp(list->string[x],valuex,RDAstrlen(valuex)))
			{
/* target found in BROWSE LIST element x, updating BROWSE LIST accordingly  */
				if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
				{
					updatersrc(mainrsrc,"BROWSE LIST");	
					Rfree(valuex);
					BF=Rmalloc(sizeof(Browse_Function));
					BF->mainrsrc=mainrsrc;	
					BF->list=b->list;
					BF->okfunction=b->okfunction;
					browse_function(mainrsrc,BF);
					if(BF!=NULL) Rfree(BF);
					return;
				}
			} 
		}
/* target not found, setting BROWSE LIST to last element in list */
		if(current_value<0) current_value=0;
		else if(current_value>=list->num) current_value=list->num-1;
		x=current_value;
		if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
			updatersrc(mainrsrc,"BROWSE LIST");
		Rfree(valuex);
	} else {
/* when target is NULL, set BROWSE LIST to the 1st element */
		x=current_value;
		if(!FINDRSCSETINT(mainrsrc,"BROWSE LIST",x))
			updatersrc(mainrsrc,"BROWSE LIST");
	}
	BF=Rmalloc(sizeof(Browse_Function));
	BF->mainrsrc=mainrsrc;	
	BF->list=b->list;
	BF->okfunction=b->okfunction;
	browse_function(mainrsrc,BF);
	if(BF!=NULL) Rfree(BF);
}
void ClearSearchSelect(RDArsrc *parent,MakeBrowseList *b)
{
	RDArmem *member=NULL;
	int x=0,valuex=0;
	char *temp=NULL;
	RDAbrowse *browse=NULL;
	Browse_Function *BF=NULL;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v=NULL;
	RDAbrowse *list;

	if(b->searchrsrc==NULL) return;
	if(b->query!=NULL) Rfree(b->query);
	FINDRSCSETSTRING(b->searchrsrc,"QUERY",temp);
/*
	FINDRSCGETINT(b->mainrsrc,"BROWSE LIST",&valuex);
*/
	for(x=0,member=b->searchrsrc->rscs;x<b->searchrsrc->numrscs;++x,++member)
	{
		if(member->field_type==BOOLNS)
		{
			if(!strncmp(member->rscrname,"RANGE ON",8)) 
			{
				FINDRSCSETCHAR(b->searchrsrc,member->rscrname,FALSE);
			} else if(!strncmp(member->rscrname,"SELECT ",7) && RDAstrstr(member->rscrname,"DELETEFLAG") && RDAstrstr(member->rscrname,"TRUE"))
			{
				FINDRSCSETCHAR(b->searchrsrc,member->rscrname,FALSE);
#ifdef RECOMMENDED_REMOVE /* 08-20-2010 */
			} else if(!strncmp(member->rscrname,"SELECT ",7) && RDAstrstr(member->rscrname,"[ACTIVE]") && RDAstrstr(member->rscrname,"FALSE"))
			{
				FINDRSCSETCHAR(b->searchrsrc,member->rscrname,FALSE);
#endif /* RECOMMENDED_REMOVE 08-20-2010 */
			} else if(!strncmp(member->rscrname,"SELECT ",7))
			{
				FINDRSCSETCHAR(b->searchrsrc,member->rscrname,TRUE);
			}
		} 
	} 
	if(b->rs!=NULL) FreeRangeScreen(b->rs);
	b->rs=NULL;
/* Allocating a Range Screen Structure */
	b->rs=RangeScreenNew(b->searchrsrc);
/* Reading Range Screen structure */
	ADVReadRangeScreen(b->fileno,b->rs,b->file_type);
	if(b->searchrsrc->virflds!=NULL)
	{
		for(x=0,v=b->searchrsrc->virflds;x<b->searchrsrc->num;++x,
			++v)
		{
			ReadVirtualRangeStruct(v,b->rs);
		}
	}
	tmprsrc=diagscrn(b->fileno,"DIAGNOSTIC SCREEN",b->searchrsrc->module,"Browse Diagnostic",stop_diagnostic);
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED.","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that this screen is available in this module.  If not, call your installer.\n",NULL,NULL,TRUE,NULL);
			prterr("Error Cannot Create Screen [%s] [%s].",tmprsrc->module,tmprsrc->screen);
		} else {
			SETTITLE(tmprsrc,"CREATING BROWSE DIAG");
			ForceWindowUpdate(tmprsrc);
		}
	}
	xmakebrowselist(b,tmprsrc);
	list=b->list;
	list->keyvalue=list->key[0];
	if(list->num<=valuex) valuex=0;
	if(tmprsrc!=NULL)
	{
		if(!abort_diagnostic) killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	BF=Rmalloc(sizeof(Browse_Function));
	BF->mainrsrc=b->mainrsrc;	
	BF->list=b->list;
	BF->okfunction=b->okfunction;
	browse=b->list;
	browse->keyvalue=browse->key[0];
	browse_function(b->mainrsrc,BF);
	if(BF!=NULL) Rfree(BF);
	if(!FINDRSCSETLIST(b->mainrsrc,"BROWSE LIST",valuex,list->num,&list->string)) 
	{
		updatersrc(b->mainrsrc,"BROWSE LIST");
	}
}
MakeBrowseList *xbrowse(short fileno,short keyno,RDArsrc *mainrsrc,
	RDArsrc *searchrsrc,RDArsrc *definelist,void (*initfunc)(...),
	short (*breakfunc)(...),short (*selectfunc)(...),
	void (*doexit)(...),void (*okfunction)(...),
	void (*new_record)(...),void *funcargs,short modal_b,
	void (*listfunc)(...),short add_type,short select_type,short quit_type,
	short file_type,PassKey *passkey,int start_where,int start_with,int line,char *file)
{
/* This is the Big Bad Browse Function Call itself.  The fileno is the
	datafile to be displayed in the BROWSE LIST.  Keyno denotes the key
	to use, unless a key is specified on the define list.  mainrsrc is
	the RDArsrc structure the BROWSE LIST is to appear on as well as 
	it's associated buttons, SEARCH FIELD and SEARCH CRITERIA.  Also
	any files higher in the hierarchy may be displayed on this resource.
	searchrsrc is the RDArsrc used to create the Search Browse Screen.
	definelist is the RDArsrc used to create the Define List Screen.
	initfunc is a void (*initfunc)(...) used to initialize the file pointer
	before scanning the datafile fileno.  The breakfunc is a pointer 
	to application level function (*breakfunc)(...) that breaks out of the 
	Selection Loop when function returns FALSE.  
	The selectfunc is a short pointer to a function at the application 
	level (*selectfunc)(...) used to restrict the browse to contain specific
	records, it returns FALSE if the record isn't to be in the list.  
	The doexit function is executed when pushing the QUIT button.  The
	okfunction is executed when pushing the SELECT button.  The newrecord 
	function is used when pushing the ADD button.  funcargs is a void 
	pointer globally available to all functions, it is an element of the
	MakeBrowseList, which contains most all of Browse's private data.
	modal_b is used to set force all window's to be FULL_APPLICATION_MODAL.
	listfunc is a list callback function executed when selecting an 
	element on the BROWSE LIST resource.  add_type reflects the type 
	of button ADD should be, e.g. 0 - addbtnrsrc, 1 addrfkwrsrc,
	2 - addrfexrsrc.  select_type and quit_type reflect the same for
	the SELECT and QUIT buttons respectively.  file_type is 0 when
	using the [FILENAME][FIELDNAME] convention for resources, 1 when
	using just FIELDNAME (no []'s).  */
	MakeBrowseList *mkbse=NULL;
	short nofields,nokeys;
	char sch,dfl;
	NRDfield *fields=NULL;
	NRDkey *keys=NULL;
	MakeBrowseList *b=NULL;
	Define_List *d=NULL;
	Browse_Exit *BE=NULL;
	RDAbrowse *browse=NULL;
	int x,valkey=0,xlength=0;
	int start_pos=0,posl=0;
	Browse_Function *BF=NULL;
	char *name1=NULL;
	char **key,*errordesc=NULL;
	APPlib *keysavl=NULL;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Creating Advanced Browse Function for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
/* testing the fileno, ensuring that it is available to the Database Engine */
	if(!TESTFILENO(fileno,line,file)) return(NULL);
/* get number of fields for fileno */
	nofields=NUMFLDS(fileno);
/* get a pointer to the fields structure from Database Engine */
	fields=FLDPOINTER(fileno);
/* get number of keys from database engine for file fileno */
	nokeys=NUMKEYS(fileno);
/* get a pointer to the keys for file fileno from the Database Engine */
	keys=KEYPOINTER(fileno);
/* creating an RDArsrc structure for the Define List Screen */
	keysavl=mk_define_list(definelist,nofields,fields,nokeys,keys,mainrsrc,keyno);
	dfl=TRUE;
/* creating the range screen resources for the fields in file fileno */
/* ADD Search Browse Expression and Load Buttons here */
	addstdrsrc(searchrsrc,"QUERY",VARIABLETEXT,0,NULL,TRUE);
	addsctrsrc(searchrsrc,"EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(searchrsrc,"LOAD VALUE",TRUE,BrowseLoadValue,"EXPRESSION");
	addbtnrsrc(searchrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(searchrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(searchrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	if(!xfile2rangersrc(fileno,searchrsrc,file_type,__LINE__,__FILE__)) 
	{
/* setting the a deleteflag resources so that something get's selected */
		sch=TRUE;
		if(!file_type)
		{
			name1=Rmalloc(RDAstrlen(FILENAME(fileno))+28);
			sprintf(name1,"SELECT [%s][DELETEFLAG] FALSE",FILENAME(fileno));
		} else {
			name1=Rmalloc(26);
			sprintf(name1,"SELECT [DELETEFLAG] FALSE");
		}
		FINDRSCSETCHAR(searchrsrc,name1,sch);
		Rfree(name1);
		if(!file_type)
		{
			name1=Rmalloc(RDAstrlen(FILENAME(fileno))+27);
			sprintf(name1,"SELECT [%s][DELETEFLAG] TRUE",FILENAME(fileno));
		} else {
			name1=Rmalloc(25);
			sprintf(name1,"SELECT [DELETEFLAG] TRUE");
		}
		sch=FALSE;
		FINDRSCSETCHAR(searchrsrc,name1,sch);
		Rfree(name1);
	}
/* creating range resources for virtual fields on the Search Browse Screen */
	SCRNvirtual2rangersrc(searchrsrc);
#ifndef __GTK_H__
	DefaultScreens(searchrsrc);
	setfile2rsrccallbacks(fileno,searchrsrc,file_type);
#endif
	SetRangersrcsensitive(searchrsrc);
	GET_RANGESCREEN_EXPRESSION_VIRTUALS(searchrsrc);
/* establishing the values in the MakeBrowseList Structure */
	b=Rmalloc(sizeof(MakeBrowseList));
	b->breakfunc=breakfunc;
	b->selectfunc=selectfunc;
	b->fileno=fileno;
	FINDRSCGETINT(definelist,"KEY LIST",&valkey);
	b->keyno=valkey+1;
	b->definelist=definelist;
	b->initfunc=initfunc;
	b->list=RDAbrowseNEW(fileno,b->keyno,listfunc);
	b->mainrsrc=mainrsrc;
	b->start_where=start_where;
	b->start_with=start_with;
	b->modal_b=modal_b;
	b->okfunction=okfunction;
	b->doexit=doexit;
	b->passkey=passkey;
	b->new_record=new_record;
	b->searchrsrc=searchrsrc;
	b->funcargs=funcargs;
	b->file_type=file_type;
	b->quit_type=quit_type;
	b->query=NULL;
#ifdef __COMPETES_WITH_FOCUS_CALLBACKS__
	FINDRSCSETFUNC(searchrsrc,"QUERY",QueryFunc,NULL);
#endif /* __COMPETES_WITH_FOCUS_CALLBACKS__ */
	FINDRSCSETFUNC(definelist,"SELECT",readlist,b);
/* creating appropriate search browse control buttons */
	addbtnrsrc(searchrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(searchrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
	addrfkwrsrc(searchrsrc,"SELECT",TRUE,dosmake,b);
	addrfkwrsrc(searchrsrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addrfcbrsrc(searchrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(searchrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
/* setting the Search Browse screen's defaults if a structure exists */
#ifdef __GTK_H__
	DefaultScreens(searchrsrc);
	setfile2rsrccallbacks(fileno,searchrsrc,file_type);
#endif
	d=Rmalloc(sizeof(Define_List));
	d->definelist=definelist;
	addbtnrsrc(mainrsrc,"DEFINE LIST",dfl,define_list,d);
	addbtnrsrc(mainrsrc,"SEARCH BUTTON",TRUE,dosearch,b);
	addstdrsrc(mainrsrc,"SEARCH CRITERIA",VARIABLETEXT,0,"Select All",FALSE);
	addstdrsrc(mainrsrc,"BROWSE LIST DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addbtnrsrc(mainrsrc,"CLEAR SEARCH & LPM",TRUE,ClearSearchLPM,NULL);
	addbtnrsrc(mainrsrc,"SEARCH ALL",TRUE,ClearSearchSelect,b);
	addbtnrsrc(mainrsrc,"TOP",TRUE,gototop,b);
	addbtnrsrc(mainrsrc,"BOTTOM",TRUE,gotobottom,b);
	addstdrsrc(mainrsrc,"BROWSE LIST LPM",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(mainrsrc,"BROWSE LIST LPM",lpm_field_func,b);
	FINDRSCSETINPUTFOCUS(mainrsrc,"BROWSE LIST LPM");
	if(b->rs!=NULL) FreeRangeScreen(b->rs);
	b->rs=NULL;
/* Allocating a Range Screen Structure */
	b->rs=RangeScreenNew(b->searchrsrc);
/* Reading Range Screen structure */
	ADVReadRangeScreen(b->fileno,b->rs,file_type);
	if(b->searchrsrc->virflds!=NULL)
	{
		for(x=0,v=b->searchrsrc->virflds;x<b->searchrsrc->num;++x,
			++v)
		{
			ReadVirtualRangeStruct(v,b->rs);
		}
	}
/* creating diagnostic screen */
	if(USE_DIAGNOSTIC_SCREENS)
	{
		tmprsrc=diagscrn(b->fileno,"DIAGNOSTIC SCREEN",b->searchrsrc->module,"Browse Diagnostic",stop_diagnostic);
	}else{
		tmprsrc=NULL;
	}
	if(start_with==1)
	{
#if ((!defined(WIN32)) || (!defined(__GTK_H__))) 
/* THIS IS COMMENTED OUT BECAUSE OTHERWISE WINDOWS WILL PLACE THE BROWSE 
   SCREEN BEHIND THE PREVIOUSLY EXECUTED PROGRAM */
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED.","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that this screen is available in this module.  If not, call your installer.\n",NULL,NULL,TRUE,NULL);
				prterr("Error Cannot Create Screen [%s] [%s].",tmprsrc->module,tmprsrc->screen);
			} else {
				SETTITLE(tmprsrc,"CREATING BROWSE DIAG");
				ForceWindowUpdate(tmprsrc);
			}
		}
/* THIS IS COMMENTED OUT BECAUSE OTHERWISE WINDOWS WILL PLACE THE BROWSE 
   SCREEN BEHIND THE PREVIOUSLY EXECUTED PROGRAM */
#endif 
/* creating reading file and establishing initial RDAbrowse Structure */
		xmakebrowselist(b,tmprsrc);
#if ((!defined(WIN32)) || (!defined(__GTK_H__))) 
/* THIS IS COMMENTED OUT BECAUSE OTHERWISE WINDOWS WILL PLACE THE BROWSE 
   SCREEN BEHIND THE PREVIOUSLY EXECUTED PROGRAM */
		if(tmprsrc!=NULL)
		{
			if(!abort_diagnostic) killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
/* THIS IS COMMENTED OUT BECAUSE OTHERWISE WINDOWS WILL PLACE THE BROWSE 
   SCREEN BEHIND THE PREVIOUSLY EXECUTED PROGRAM */
#endif
	} else {
/* Establishing empty browse because of search screen comes up first */
		b->list->key=NULL;
		b->list->keyvalue=NULL;
       		addRDAbrowse(b->list,"***************    No Records Selected from current file.   ***************");
	}
	BF=Rmalloc(sizeof(Browse_Function));
	BF->mainrsrc=mainrsrc;	
	BF->list=b->list;
	BF->okfunction=okfunction;
	browse=b->list;
	browse->keyvalue=browse->key[0];
	addstdrsrc(mainrsrc,"SEARCH FIELD",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SEARCH FIELD",search_field_func,b);
	if(b->start_where==FALSE)
	{
		start_pos=0;
	} else {
		if(b->start_where==TRUE)
		{
			if(browse->num>0) start_pos=browse->num-1;
				else start_pos=0;
		}
	}
	addlstrsrc(mainrsrc,"BROWSE LIST",&start_pos,TRUE,browse_function,browse->num,&browse->string,BF);
	start_pos+=1;
	posl=start_pos+1;
	addstdrsrc(mainrsrc,"BROWSE LIST POSITION",LONGV,8,&posl,FALSE);
	BE=Rmalloc(sizeof(Browse_Exit));
	BE->doexit=doexit;
	BE->nofields=nofields;
	BE->fields=fields;
	BE->nokeys=nokeys;
	BE->keysavl=keysavl;
	BE->keys=keys;
	BE->b=b;
	BE->funcargs=b->funcargs;
	BE->l=d;
	BE->bf=BF;
	SCRNvirtual2rsrc(mainrsrc);
	if(new_record!=NULL)
	{
/* creating an ADD button of appropriate type when a new_record function is available */
		switch(add_type)
		{
			default:
			case 0: /* normal button */
				addbtnrsrc(mainrsrc,"ADD",TRUE,new_record,b);
				break;
			case 1: /* kill window */
				addrfkwrsrc(mainrsrc,"ADD",TRUE,new_record,b);
				break;
			case 2: /* exit button */
				addrfexrsrc(mainrsrc,"ADD",TRUE,add_browse_exit,BE);
				break;
		}
	}
	if(okfunction!=NULL)
	{
/* creating appropriate SELECT button when an okfunction is available */
		key=(char **)browse->key;
		browse->keyvalue=key[0];
		switch(select_type)
		{
			default:
			case 0: /* normal button */
				addbtnrsrc(mainrsrc,"SELECT",TRUE,runokfunc,b);
				break;
			case 1: /* kill window */
				addrfkwrsrc(mainrsrc,"SELECT",TRUE,runokfunc,b);
				break;
			case 2: /* exit window */
				addrfexrsrc(mainrsrc,"SELECT",TRUE,select_browse_exit,BE);
				break;
		}
	}
/* creating appropriate QUIT button for the Browse function */
	switch(quit_type)
	{
		case 0: /* normal button */
			addbtnrsrc(mainrsrc,"QUIT",TRUE,kw_browse_exit,BE);
			break;
		case 1: /* kill window */
			addrfkwrsrc(mainrsrc,"QUIT",TRUE,kw_browse_exit,BE);
			break;
		default:
		case 2: /* exit window */
			addrfexrsrc(mainrsrc,"QUIT",TRUE,browse_exit,BE);
			break;
	}
	BE->b=b;
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfcbrsrc(mainrsrc,"PRINT BROWSE",TRUE,printbrowse,b);
	addrfcbrsrc(mainrsrc,"CSV",TRUE,browselistcsv,b);
	if(!file_type)
	{
		if(FILEsecurity(MODULENAME(b->fileno),FILENAME(b->fileno),TRUE,FALSE,TRUE))
		{
			FINDRSCSETEDITABLE(mainrsrc,"PRINT BROWSE",TRUE);
			FINDRSCSETSENSITIVE(mainrsrc,"PRINT BROWSE",TRUE);
		} else {
			FINDRSCSETEDITABLE(mainrsrc,"PRINT BROWSE",FALSE);
			FINDRSCSETSENSITIVE(mainrsrc,"PRINT BROWSE",FALSE);
		}
	}
	BF->mainrsrc=mainrsrc;	
	FINDRSCSETFUNC(mainrsrc,"BROWSE LIST",browse_function,BF);
	b->mainrsrc=mainrsrc;
	b->searchrsrc=searchrsrc;
	BE->b=b;
	FINDRSCSETFUNC(searchrsrc,"SELECT",dosmake,b);
	FINDRSCSETFUNC(definelist,"SELECT",readlist,b);
/* setting initial values for BROWSE LIST function and executing the list_callback function */
	browse_function(mainrsrc,BF);
/* establishing and creating the appropriate virtual field resources for browse */
	BE->b=b;
	setfile2rsrccallbacks(fileno,searchrsrc,file_type);
	SetVirFieldFuncs(mainrsrc);
	CLEAR_SCREEN_VIRTUAL(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	mkbse=b;
	BE->b=b;
/* creating the Main Browse window */
	if(start_with==1)
	{
		if(ADVmakescrn(mainrsrc,(modal_b ? TRUE:FALSE))) 
		{
			xlength=(RDAstrlen(mainrsrc->screen)+RDAstrlen(definelist->screen)+RDAstrlen(searchrsrc->screen)+90);
			errordesc=Rmalloc(xlength);
			sprintf(errordesc,"The Browse Utility has failed.  Verify that screens:\n\t[%s]\n\t[%s]\n\t[%s]\n are on file.",mainrsrc->screen,definelist->screen,searchrsrc->screen);
			ERRORDIALOG("Browse Data File Failed",errordesc,doexit,TRUE);
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=NULL;
			browse_exit(mainrsrc,BE);
		}
	} 
	if(start_with==2)
	{
		BE->b=b;
		dosearchinit(b,BE);
	}
/* returning a pointer to the MakeBrowseList structure */
	return(mkbse);
}
Browse_Exit *xRETURN_BROWSE_EXIT(MakeBrowseList *b,int line,char *file)
{
/* this function isn't necessary yet, it allow's you to obtain a pointer to
	that last piece of private data used by a browse function.  This 
	is here, so that if you are not doing an addrfexrsrc somewhere,
	this data can still be free'd.  This return's a pointer to the
	Browse_Exit structure always available to the QUIT button, whether
	it does a addrfexrsrc or not. */
	int y=0;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RETURN_BROWSE_EXIT at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(b!=NULL)
	{
		if(b->mainrsrc!=NULL)
		{
			y=FINDRSC(b->mainrsrc,"QUIT");
			if(y!=(-1))
			{
				member=b->mainrsrc->rscs+y;
				if(member!=NULL) return(member->arglist);	
			}
		}
	}
	return(NULL);
}
short xFREE_BROWSE_EXIT(MakeBrowseList *b,int line,char *file)
{
	int y=0;
	Browse_Exit *BE=NULL;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG FREE_BROWSE_EXIT at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(b!=NULL)
	{
		if(b->mainrsrc!=NULL)
		{
		y=FINDRSC(b->mainrsrc,"QUIT");
		if(y!=(-1))
		{
			member=b->mainrsrc->rscs+y;
			if(member!=NULL)
			{
				BE=member->arglist;
				FreeBrowseExit(BE);
				BE=NULL;
			}
			return(0);
		} else {
			return(-1);
		}
		} else {
			prterr("Error MakeBrowseList mainrsrc element is NULL.");
			return(-3);
		}
	} else {
		return(-2);
	}
}

void xupdatebrowse(short update_list,MakeBrowseList *mbl,short filenum,char *module,RDArsrc *mtnrsrc,int line,char *file)
{
	int valuex=0;
	RDAbrowse *list=NULL;
	RDArsrc *tmprsrc=NULL;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG updatebrowse at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(update_list==TRUE && mbl!=NULL)
	{
		if(mbl->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl->mainrsrc,"BROWSE LIST",&valuex);
			if(USE_DIAGNOSTIC_SCREENS)
			{
				tmprsrc=diagscrn(filenum,"DIAGNOSTIC SCREEN",module,"Browse Diagnostic",stop_diagnostic);
			}
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
					prterr("Error Cannot Create Diagnostic Screen.");
				} else {
					SETTITLE(tmprsrc,"UPDATING BROWSE DIAG");
					ForceWindowUpdate(tmprsrc);
				}
			}
			xmakebrowselist(mbl,tmprsrc);
			list=mbl->list;
			if(list->num<=valuex) valuex=0;
			list->keyvalue=list->key[valuex];
			if(!FINDRSCSETLIST(mbl->mainrsrc,"BROWSE LIST",valuex,list->num,&list->string))
			{
				updatersrc(mbl->mainrsrc,"BROWSE LIST");
			}
			if(tmprsrc!=NULL)
			{
				if(!abort_diagnostic) killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			if(mtnrsrc!=NULL) ForceWindowUpdate(mtnrsrc);
		}
	}
}
void xFreeBrowseExit(Browse_Exit *BE,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG FreeBrowseExit at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(BE!=NULL)
	{
		if(BE->keysavl!=NULL) freeapplib(BE->keysavl);
		if(BE->b->list!=NULL) RDAbrowseFREE(BE->b->list);
		if(BE->b->rs!=NULL) FreeRangeScreen(BE->b->rs);
		BE->b->rs=NULL;
		if(BE->b!=NULL) Rfree(BE->b);
		BE->b=NULL;
		if(BE->l!=NULL) Rfree(BE->l);
		if(BE->bf!=NULL) Rfree(BE->bf);
		BE->l=NULL;
		Rfree(BE);
		BE=NULL;
	}
}
static void quitrd(RDArsrc *r)
{
	if(DEFAULT_GROUPLIST!=NULL) freeapplib(DEFAULT_GROUPLIST);
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
}
static void changedefgroup(RDArsrc *r,APPlib *g)
{
	int x=0;

	FINDRSCGETINT(r,"GROUP LIST",&x);
	if(g!=NULL)
	{
		if(g->numlibs<x) 
		{
			/* How did you do this? */
		} else {
			if(RDA_GROUP!=NULL) Rfree(RDA_GROUP);
			RDA_GROUP=stralloc(g->libs[x]);
		}
	}	
}
static void GResetDefaults(RDArsrc *r,RDArsrc *rsrc)
{
	char temp[512];
	RDAdefault *d=NULL;
	int x=0;
	DEFfield *field=NULL;

	FINDRSCGETINT(r,"GROUP LIST",&x);
	memset(temp,0,512);
#ifndef WIN32
	sprintf(temp,"%s/%s/%s.DEF",CURRENTDIRECTORY,DEFAULT_GROUPLIST->libs[x],rsrc->module);
#endif 
#ifdef WIN32
	sprintf(temp,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,DEFAULT_GROUPLIST->libs[x],rsrc->module);
#endif
	LastGroupDefault=stralloc(DEFAULT_GROUPLIST->libs[x]);
	d=RDAdefaultNEW(rsrc->module,rsrc->screen);
	if(!getdefaultbin(temp,rsrc->screen,d,FALSE))
	{
		if(d->input_focus!=NULL) 
			FINDRSCSETINPUTFOCUS(rsrc,d->input_focus);
		for(x=0,field=d->field;x<d->num;++x,++field)
		{
			if((field->name!=NULL))
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
					FINDRSCSETSTRING(rsrc,field->name,
						field->data.string_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDRSCSETCHAR(rsrc,field->name,
						*field->data.string_value);
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					FINDRSCSETDOUBLE(rsrc,field->name,
						*field->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDRSCSETSHORT(rsrc,field->name,
						*field->data.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					FINDRSCSETINT(rsrc,field->name,
						*field->data.integer_value);
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Default Screen Field [%s].",field->type,field->name);
					break;
			}
			}
		}
	}	
	FreeRDAdefault(d);
	ADVcomputeandupdateallSCRNvirtuals(rsrc,SCRNvirtualSubData,rsrc,TRUE);
	SetRangersrcsensitive(rsrc);
	updateallrsrc(rsrc);
	quitrd(r);
}
void ResetGroupDefaults(RDArsrc *r)
{
	RDArsrc *grsrc=NULL;
	char *pattern=NULL,*tmp=NULL,temp[512];
	int x=0;
	RDAdefault *d=NULL;
	

	DEFAULT_GROUPLIST=APPlibNEW();
	pattern=stralloc("*");
	for(x=0;(findfile(CURRENTDIRECTORY,pattern,&tmp,(int)x+1));++x) 
	{
		memset(temp,0,512);
#ifndef WIN32
		sprintf(temp,"%s/%s/%s.DEF",CURRENTDIRECTORY,tmp,r->module);
#endif 
#ifdef WIN32
		sprintf(temp,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,tmp,r->module);
#endif
		if(access(temp,00)==0)  /* check for existance of file */
		{
			d=RDAdefaultNEW(r->module,r->screen);
			if(!getdefaultbin(temp,r->screen,d,FALSE))
			{
				addAPPlibNoDuplicates(DEFAULT_GROUPLIST,tmp);
			}
			FreeRDAdefault(d);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(DEFAULT_GROUPLIST);
	x=0;
	if(!isEMPTY(LastGroupDefault))
	{
	for(x=0;x<DEFAULT_GROUPLIST->numlibs;++x)
	{
		if(!RDAstrcmp(DEFAULT_GROUPLIST->libs[x],LastGroupDefault)) break;
	}
	}
	if(x>=DEFAULT_GROUPLIST->numlibs) x=0;
	grsrc=RDArsrcNEW("GUI","RESET DEFAULTS CHANGE GROUP");
	addlstrsrc(grsrc,"GROUP LIST",&x,TRUE,changedefgroup,DEFAULT_GROUPLIST->numlibs,&DEFAULT_GROUPLIST->libs,DEFAULT_GROUPLIST);
	
	addbtnrsrc(grsrc,"RESET DEFAULTS",TRUE,GResetDefaults,r);
	addbtnrsrc(grsrc,"QUIT",TRUE,quitrd,NULL);
	addbtnrsrc(grsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(grsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(xmakescrn(grsrc,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,NULL,NULL,SCRNvirtualSubData,r,r,__LINE__,__FILE__)) 
	{
	}	
}
