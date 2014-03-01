#include<rptgen.hpp>

void setDefaultComputedeftype(RDArsrc *rsrc,RDArmem *member)
{
	short val=0;

	readwidget(rsrc,member->rscrname);
	FINDRSCGETSHORT(rsrc,member->rscrname,&val);
	setdeftype(member,val);
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
/*
	SetRangersrcsensitive(rsrc);
	ADVupdateSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs);
*/
}
void setDefaultComputeacctype(RDArsrc *rsrc,RDArmem *member)
{
	short val=0;

	readwidget(rsrc,member->rscrname);
	FINDRSCGETSHORT(rsrc,member->rscrname,&val);
	setacctype(member,val);
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
}
void setvirtualrangecallbacks(RDAreport *rpt,RDArsrc *rsrc)
{
	RDAvirtual *field=NULL;
	short x,y;
	char *name=NULL;
	RDArmem *member;
	
	if(rpt!=NULL && rsrc!=NULL)
	{
	if(rpt->virflds!=NULL)
	{
		for(x=0,field=rpt->virflds;x<rpt->numvirtuals;++x,++field)
		{
			if(!field->when && field->range==1)
			{
				if(!RDAstrcmp(field->name,"DEFINITION TYPE"))
				{
					name=Rmalloc(RDAstrlen(field->name)+8);
					sprintf(name,"FROM [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setdeftypefrom,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"TO [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setdeftypeto,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
				} else if(!RDAstrcmp(field->name,"ACCOUNT TYPE"))
				{
					name=Rmalloc(RDAstrlen(field->name)+8);
					sprintf(name,"FROM [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setacctypefrom,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"TO [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setacctypeto,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
				}
			} else if(!field->when && field->range==2)
			{
				if(!RDAstrcmp(field->name,"DEFINITION TYPE"))
				{
					name=Rmalloc(RDAstrlen(field->name)+3);
					sprintf(name,"%s",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setvirtualdeftype,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
				} else if(!RDAstrcmp(field->name,"ACCOUNT TYPE"))
				{
					name=Rmalloc(RDAstrlen(field->name)+3);
					sprintf(name,"%s",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setvirtualacctype,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
				}
			} else if(!field->when && field->range==4)
			{
				if(!RDAstrcmp(field->name,"DEFINITION TYPE"))
				{
					name=Rmalloc(RDAstrlen(field->name)+3);
					sprintf(name,"%s",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setvirtualdeftype,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
					name=Rmalloc(RDAstrlen(field->name)+8);
					sprintf(name,"FROM [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setdeftypefrom,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"TO [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setdeftypeto,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
				} else if(!RDAstrcmp(field->name,"ACCOUNT TYPE"))
				{
					name=Rmalloc(RDAstrlen(field->name)+3);
					sprintf(name,"%s",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setvirtualacctype,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
					name=Rmalloc(RDAstrlen(field->name)+8);
					sprintf(name,"FROM [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setacctypefrom,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"TO [%s]",field->name);
					y=FINDRSC(rsrc,name);
					if(y!=(-1))
					{
						member=rsrc->rscs+y;
						FINDRSCSETFUNC(rsrc,name,setacctypeto,member);
					} else member=NULL;
					if(name!=NULL) Rfree(name);
				}
			}
		}
	}
	}
}
void setvirtualdeftype(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	short x;
	char *temp;

	if(member!=NULL && rsrc!=NULL)
	{	
	if(member->sensitive==TRUE && member->user_sensitive==TRUE)
	{
	readwidget(rsrc,member->rscrname);
	if(rsrc!=NULL)
	{
		for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
		{
			if(mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE)
			{
				for(temp=mem->rscrname;*temp;++temp)
				{
					if(!strncmp(temp,"ACCOUNT CODE",12) ||
						!strncmp(temp,"DEBIT CODE",10) || 
						!strncmp(temp,"CREDIT CODE",11) ||
						!strncmp(temp,"PAYABLE CODE",12) ||
						!strncmp(temp,"ENCUMBRANCE CODE",16) ||
						!strncmp(temp,"RESERVE CODE",12))
					{
						MEMBERSETDEFTYPE(mem,*member->value.short_value);
						switch(mem->field_type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								mem->items=*member->value.short_value;
								if(mem->dlen>0)
								{
									memset(mem->value.string_value,0,mem->dlen);
								} else {
									mem->value.string_value=NULL;
									mem->dlen=0;
								}
								updatemember(mem);
								break;
							default:
								prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
								break;
						}
						break;
					}
				}
			}
		}
	}
	}
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
	}
}
void setvirtualdeftypefrom(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	short x;
	char *temp;

	if(member!=NULL && rsrc!=NULL)
	{
	if(member->sensitive==TRUE && member->user_sensitive==TRUE)
	{
	readwidget(rsrc,member->rscrname);
	if(rsrc!=NULL)
	{
		for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
		{
			if(!strncmp(mem->rscrname,"FROM",4) && (mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE))
			{
				for(temp=mem->rscrname;*temp;++temp)
				{
					if(!strncmp(temp,"ACCOUNT CODE",12) ||
						!strncmp(temp,"DEBIT CODE",10) || 
						!strncmp(temp,"CREDIT CODE",11))
					{
						MEMBERSETDEFTYPE(mem,*member->value.short_value);
						switch(mem->field_type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								mem->items=*member->value.short_value;
								if(mem->dlen>0)
								{
									memset(mem->value.string_value,0,mem->dlen);
								} else {
									mem->value.string_value=NULL;
									mem->dlen=0;
								}
								updatemember(mem);
								break;
							default:
								prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
								break;
						}
						break;
					}
				}
			}
		}
	}
	}
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
	}
}
void setvirtualacctype(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	short x;
	char *temp=NULL;

	if(rsrc!=NULL && member!=NULL)
	{
	if(member->sensitive==TRUE && member->user_sensitive==TRUE)
	{
	readwidget(rsrc,member->rscrname);
	if(rsrc->rscs!=NULL)
	{
		for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
		{
			if(mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE)
			{
				for(temp=mem->rscrname;*temp;++temp)
				{
					if(!strncmp(temp,"ACCOUNT CODE",12))
					{
						MEMBERSETACCTTYPE(mem,*member->value.short_value);
						switch(mem->field_type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								switch(*member->value.short_value)
								{
									case EXP_ACCT:
										mem->field_type=EXPENDITURE;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									case REV_ACCT:
										mem->field_type=REVENUE;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									case BAL_ACCT:
										mem->field_type=BALANCESHEET;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									case BEG_ACCT:
										mem->field_type=BEGINNINGBALANCE;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									default:
										prterr("Error Resource [%s] has the wrong acctype [%d].",member->rscrname,*member->value.short_value);
										return;
								}
								break;
							default:
								prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
								break;
						}
						break;
					}
				}
			}
		}
	}
	}
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
	}
}
void setvirtualdeftypeto(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	short x;
	char *temp;

	if(member!=NULL && rsrc!=NULL)
	{
	if(member->sensitive==TRUE && member->user_sensitive==TRUE)
	{
	readwidget(rsrc,member->rscrname);
	if(rsrc!=NULL)
	{
		for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
		{
			if(!strncmp(mem->rscrname,"TO",2) && (mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE))
			{
				for(temp=mem->rscrname;*temp;++temp)
				{
					if(!strncmp(temp,"ACCOUNT CODE",12) ||
						!strncmp(temp,"DEBIT CODE",10) || 
						!strncmp(temp,"CREDIT CODE",11))
					{
						MEMBERSETDEFTYPE(mem,*member->value.short_value);
						switch(mem->field_type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								mem->items=*member->value.short_value;
								if(mem->dlen>0)
								{
									memset(mem->value.string_value,0,mem->dlen);
								} else {
									mem->value.string_value=NULL;
									mem->dlen=0;
								}
								updatemember(mem);
								break;
							default:
								prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
								break;
						}
						break;
					}
				}
			}
		}
	}
	}
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
	}
}
void setvirtualacctypefrom(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	short x;
	char *temp;

	if(member!=NULL && rsrc!=NULL)
	{
	if(member->sensitive==TRUE && member->user_sensitive==TRUE)
	{
	readwidget(rsrc,member->rscrname);
	if(rsrc!=NULL)
	{
		for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
		{
			if(!strncmp(mem->rscrname,"FROM",4) && (mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE))
			{
				for(temp=mem->rscrname;*temp;++temp)
				{
					if(!strncmp(temp,"ACCOUNT CODE",12))
					{
						switch(mem->field_type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								switch(*member->value.short_value)
								{
									case EXP_ACCT:
										mem->field_type=EXPENDITURE;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									case REV_ACCT:
										mem->field_type=REVENUE;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									case BAL_ACCT:
										mem->field_type=BALANCESHEET;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									case BEG_ACCT:
										mem->field_type=BEGINNINGBALANCE;
										if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										updatemember(mem);
										break;
									default:
										prterr("Error Resource [%s] has the wrong acctype [%d].",member->rscrname,*member->value.short_value);
										return;
								}
								break;
							default:
								prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
								break;
						}
						break;
					}
				}
			}
		}
	}
	}
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
	}
}
void setvirtualacctypeto(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	short x;
	char *temp;

	if(member!=NULL && rsrc!=NULL)
	{
	if(member->sensitive==TRUE && member->user_sensitive==TRUE)
	{
	readwidget(rsrc,member->rscrname);
	if(rsrc->rscs!=NULL)
	{
		for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
		{
			if(!strncmp(mem->rscrname,"TO",2) && (mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE))
			{
				for(temp=mem->rscrname;*temp;++temp)
				{
					if(!strncmp(temp,"ACCOUNT CODE",12))
					{
						switch(mem->field_type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								switch(*member->value.short_value)
								{
									case EXP_ACCT:
										mem->field_type=EXPENDITURE;
										break;
									case REV_ACCT:
										mem->field_type=REVENUE;
										break;
									case BAL_ACCT:
										mem->field_type=BALANCESHEET;
										break;
									case BEG_ACCT:
										mem->field_type=BEGINNINGBALANCE;
										break;
									default:
										prterr("Error Resource [%s] has the wrong acctype [%d].",mem->rscrname,*member->value.short_value);
										return;
								}
								break;
							default:
								prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
								break;
						}
						break;
					}
				}
			}
		}
	}
	}
	SensitiveADVcomputeandupdateallSCRNvirtuals(rsrc,rsrc->SubFunc,rsrc->SubFuncArgs,TRUE,TRUE);
	}
}
