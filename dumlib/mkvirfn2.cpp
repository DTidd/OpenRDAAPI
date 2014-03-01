/* mkvirfn2.c - Makes Virtual Fields functions*/
/*lint -library */
#include <mkvir.hpp>

void xCreateBEGBALs(char *modx,int line,char *file)
{
	short q=0,z,x,y;
	short if_count=0,exp_count=0,desc_count=0,acc_count=0,no_parts=0;
	short struct_count=0,label_count=0,partlabel_count=0;
	short longest_length=0,exp_no_parts=0,rev_no_parts=0,bal_no_parts=0;
	RDAaccpart *part=NULL,*p=NULL;
	RDAacct *acc=NULL;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char temp[15001],*tmpexp=NULL,*tmpdesc=NULL;
	char *exp_label=NULL,*tmpstruct=NULL,*tmplabel=NULL,*tmppartlabel=NULL;
	char prev_defused=FALSE;


	no_parts=0;
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_no_parts=acc->num;
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=REVENUE_ACCOUNT->codetype;
				z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				rev_no_parts=acc->num;
			}
		}
	}
	if(BEGINNING_BALANCE_ACCOUNT!=NULL)
	{
		if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=BEGINNING_BALANCE_ACCOUNT->codetype;
				z<BEGINNING_BALANCE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				bal_no_parts=acc->num;
			}
		}
	}
	for(x=0;x<no_parts;++x)
	{
		exp_count=0;
		desc_count=0;
		struct_count=0;
		label_count=0;
		partlabel_count=0;
		if_count=0;
		if(tmpexp!=NULL) Rfree(tmpexp);
		if(tmpdesc!=NULL) Rfree(tmpdesc); 
		if(tmpstruct!=NULL) Rfree(tmpstruct); 
		if(tmplabel!=NULL) Rfree(tmplabel); 
		if(tmppartlabel!=NULL) Rfree(tmppartlabel); 
		longest_length=0;
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
				tmpexp=stralloc(temp);
				tmpdesc=stralloc(temp);
				if(x==0)
				{
				tmplabel=stralloc(temp);
				tmpstruct=stralloc(temp);
				}
				tmppartlabel=stralloc(temp);
				exp_count+=RDAstrlen(temp);
				desc_count+=RDAstrlen(temp);
				if(x==0)
				{
				label_count+=RDAstrlen(temp);
				struct_count+=RDAstrlen(temp);
				}
				partlabel_count+=RDAstrlen(temp);
				if_count=0;
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<exp_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<exp_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<exp_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<exp_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<exp_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(x==0)
				{
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				if(tmpstruct!=NULL)
				{
					tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
					sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
					struct_count+=RDAstrlen(temp)+5;
				} else {
					tmpstruct=stralloc(temp);
					struct_count+=RDAstrlen(temp);
				}
				}
				if(tmppartlabel!=NULL)
				{
					tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+6);
					sprintf(&tmppartlabel[partlabel_count],"ELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+5;
				} else {
					tmppartlabel=stralloc(temp);
					partlabel_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<rev_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<rev_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<rev_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<rev_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<rev_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(BEGINNING_BALANCE_ACCOUNT!=NULL)
		{
			if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=3) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(x==0)
				{
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				if(tmpstruct!=NULL)
				{
					tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
					sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
					struct_count+=RDAstrlen(temp)+5;
				} else {
					tmpstruct=stralloc(temp);
					struct_count+=RDAstrlen(temp);
				}
				}
				if(tmppartlabel!=NULL)
				{
					tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+6);
					sprintf(&tmppartlabel[partlabel_count],"ELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+5;
				} else {
					tmppartlabel=stralloc(temp);
					partlabel_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=BEGINNING_BALANCE_ACCOUNT->codetype;
					y<BEGINNING_BALANCE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						acc_count=1;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBBACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<bal_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<bal_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<bal_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<bal_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<bal_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(tmpexp!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
			sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpdesc!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+16);
			sprintf(&tmpdesc[desc_count],"ELSE %s FI FI FI\n",temp);
		}
		if(x==0)
		{
		if(tmplabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
			sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpstruct!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+16);
			sprintf(&tmpstruct[struct_count],"ELSE %s FI FI FI\n",temp);
		}
		}
		if(tmppartlabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+16);
			sprintf(&tmppartlabel[partlabel_count],"ELSE %s FI FI FI\n",temp);
		}
		memset(temp,0,15001);
		memset(stemp,0,101);
		sprintf(stemp,"BEGBAL DIMENSION #%d CODE",x);
		d=DFvirtualNEW(modx,stemp);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBBACC");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=longest_length;
		d->when=0;
		d->avail2rsrc=TRUE;
		if(tmpexp!=NULL)
		{
			d->expression=stralloc(tmpexp);
			Rfree(tmpexp);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code Dimension %d representing all account type and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"BEGBAL DIMENSION #%d DESCRIPTION",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		if(tmpdesc!=NULL)
		{
			d->expression=stralloc(tmpdesc);
			Rfree(tmpdesc);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Description for Code Dimension %d representing all accounts and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"BEGBAL DIMENSION #%d",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=SHORTV;
		d->length=2;
		d->when=0;
		sprintf(stemp,"%d",x);
		d->expression=stralloc(stemp);
		memset(stemp,0,101);
		memset(temp,0,15001);
		sprintf(temp,"Represents Dimension number %d for use in the Chart of Accounts.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
		memset(stemp,0,101);
		if(x==0)
		{
			memset(stemp,0,101);
			sprintf(stemp,"ACCOUNT CODE LABEL");
			d=DFvirtualNEW(modx,stemp);
			memset(stemp,0,101);
			d->incvir=Rmalloc(4*sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBBACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINCOA");
			i->keyname=NULL;
			d->vtype=1;
			d->num=4;
			d->type=PLAINTEXT;
			d->length=40;
			d->when=0;
			d->avail2rsrc=TRUE;
			if(tmplabel!=NULL)
			{
				d->expression=stralloc(tmplabel);
				Rfree(tmplabel);
			} else d->expression=NULL;
			memset(temp,0,15001);
			sprintf(temp,"Account Label for each Account Type.");
			d->desc=stralloc(temp);
			memset(temp,0,15001);
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			memset(stemp,0,101);
			sprintf(stemp,"ACCOUNT CODE STRUCTURE");
			d=DFvirtualNEW(modx,stemp);
			memset(stemp,0,101);
			d->incvir=Rmalloc(4*sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBBACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINCOA");
			i->keyname=NULL;
			d->vtype=1;
			d->num=4;
			d->type=PLAINTEXT;
			d->length=40;
			d->when=0;
			d->avail2rsrc=FALSE;
			if(tmpstruct!=NULL)
			{
				d->expression=stralloc(tmpstruct);
				Rfree(tmpstruct);
			} else d->expression=NULL;
			memset(temp,0,15001);
			sprintf(temp,"Account Code Structure for each Account Type.");
			d->desc=stralloc(temp);
			memset(temp,0,15001);
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
		}
		memset(stemp,0,101);
		sprintf(stemp,"BEGBAL DIMENSION #%d LABEL",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		if(tmppartlabel!=NULL)
		{
			d->expression=stralloc(tmppartlabel);
			Rfree(tmppartlabel);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Account Part Name for Dimension %d or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
	}
}
