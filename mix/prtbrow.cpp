#include <mix.hpp>

static void fixoutput(RDA_PFILE *fp,char *data,int *pages,int *numpagelines,unsigned line_length,char *initstring,int add_length,int add_length2)
{
	unsigned y=0,x=0,length=0,initoffset=0;
	char *temp=NULL,*temp1=NULL,*temp2=NULL;

	if(fp!=NULL)
	{
	if(!isEMPTY(data))
	{
		initoffset=RDAstrlen(initstring);
		temp=strchr(data,'\n');
		temp1=strchr(data,'\r');
		if(temp==NULL && temp1==NULL)
		{
			length=RDAstrlen(data);
		} else if(temp==NULL)
		{
			length=temp1-data;
		} else if(temp1==NULL)
		{
			length=temp-data;
		} else if(temp<temp1)
		{
			length=temp-data;
		} else {
			length=temp1-data;
		}
		for(x=0,y=0;x<length;++x,++y)
		{
			if(data[x]=='\t')
			{
				if(y+9>(line_length-initoffset))
				{
					break;
				}
				y+=7;
			}
			if(y+2>(line_length-initoffset))
			{
				break;
			}
		}
		y=x;
		if(x<(length-1))
		{
			temp2=(data+x+1);
			if(temp2[0]!=0)
			{
				if(temp2[0]!=' ')
				{
					for(y=x;y>0;--y)
					{
						if(data[y]==' ') break;
					}
				}
				if(y<1) y=x;
			}
		}
		temp2=Rmalloc(initoffset+add_length+4);
		sprintf(temp2,"%*s",initoffset+add_length,"");
		if(y<length)
		{
			RDA_fprintf(fp,"%s%.*s\r\n",
				(initstring==NULL?"":initstring),
				y+1,(data==NULL?"":data));
			fixoutput(fp,data+y+1,pages,numpagelines,line_length,temp2,add_length2,0);
		} else {
			RDA_fprintf(fp,"%s%.*s",
				(initstring==NULL?"":initstring)
				,length,(data==NULL?"":data));
			if(temp!=NULL || temp1!=NULL)
			{
				fixoutput(fp,data+length+1,pages,numpagelines,line_length,temp2,add_length2,0);
			}
		}
		if(temp2!=NULL) Rfree(temp2);
	}
	}
}
static void print_browse_header(RDA_PFILE *fp,int *lines,int *pages,
	char *header,char *sub_header,unsigned line_length,unsigned max_lines,
	char *module,char *screen,char *page_header,char *page_subheader)
{
        char *organizationname = XPERT_SETUP->company_name;
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	if(fp!=NULL)
	{
	if((*lines>=max_lines) || (*pages==0 && *lines==0))
	{
		if(!(*pages==0 && *lines==0))
		{
			RDA_fprintf(fp,"\f");
		}
		*lines=0;
		*lines+=1;
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(header)+8+8+lead_ws+tail_ws)>line_length) header[(line_length-8-8-lead_ws-tail_ws)]=0;
		RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
			lead_ws,"",header,tail_ws,"",8,++*pages);
		*lines+=1;
		if(!isEMPTY(sub_header))
		{
			lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(sub_header)))/2));
			if(lead_ws<0) lead_ws=0;
			tail_ws=lead_ws;
/*
			tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(sub_header)))/2)-2));
*/
			if(tail_ws<0) tail_ws=0;
			if((RDAstrlen(sub_header)+2+lead_ws+tail_ws)>line_length) sub_header[(line_length-2-lead_ws-tail_ws)]=0;
			RDA_fprintf(fp,"%*s %s %*s\r\n",
				lead_ws,"",sub_header,tail_ws,"");
			*lines+=1;
		}
		lead_ws=(int)(RDAroundup((double)(line_length-(20+RDAstrlen(module)+RDAstrlen(screen)))/2));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint((((double)(line_length-(20+RDAstrlen(module)+RDAstrlen(screen)))/2)-14)));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(module)+RDAstrlen(screen)+14+20+lead_ws+tail_ws)>(line_length))
		{
			RDA_fprintf(fp,"%*s Module [%s] %*sDate: %s\r\n",
				(int)(round(((double)(line_length-(11+RDAstrlen(module)))/2))),"",
				module,
				(int)(Rint((((double)(line_length-(11+RDAstrlen(module)))/2)-14))),"",
				date);
			RDA_fprintf(fp,"%*s Screen [%s]\r\n",
				(int)(round(((double)(line_length-(10+RDAstrlen(screen)))/2))),"",
				screen);
			*lines+=2;
		} else {
			RDA_fprintf(fp,"%*sModule [%s] Screen [%s] %*sDate: %s\r\n",
				lead_ws,"",module,screen,tail_ws,"",date);
			*lines+=1;
		}
		lead_ws=(int)(RDAroundup(((double)(line_length-(RDAstrlen(organizationname)))/2)));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(RDAstrlen(organizationname)))/2)-14));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(organizationname)+14+lead_ws+tail_ws)>line_length)
		{
			RDA_fprintf(fp,"%*s %s %*sTime: %s\r\n",
				(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(organizationname)))/2))),"",
				organizationname,
				(int)(Rint((((double)(line_length-(2+RDAstrlen(organizationname)))/2)-14))),"",
				timex);
			*lines+=1;
		} else {
			RDA_fprintf(fp,"%*s%s%*sTime: %s\r\n",
				lead_ws,"",organizationname,tail_ws,"",timex);
			*lines+=1;
		}
		lead_ws=(int)(RDAroundup(((double)(line_length-(13+RDAstrlen(USERLOGIN)))/2)));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(13+RDAstrlen(USERLOGIN)))/2)-14));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(USERLOGIN)+14+lead_ws+tail_ws)>line_length)
		{
			RDA_fprintf(fp,"%*s %s\r\n",
				(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
				USERLOGIN);
			*lines+=1;
		} else {
			RDA_fprintf(fp,"%*sExecuted By: %s\r\n",
				lead_ws,"",USERLOGIN);
			*lines+=1;
		}
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		RDA_fprintf(fp,"\r\n");
		*lines+=1;
		fixoutput(fp,page_header,pages,lines,line_length,"",0,2);
		RDA_fprintf(fp,"\r\n");
		*lines+=1;
		fixoutput(fp,page_subheader,pages,lines,line_length,"",0,2);
		RDA_fprintf(fp,"\r\n");
		*lines+=1;
	}
	RDA_fprintf(fp,"\r\n");
	*lines+=1;
	}
}
char *make_definelist_string(RDArsrc *definelist,short length,char **seperator)
{
	short x=0,z=0,cur=1,lcount=0,w=0;
	RDArmem *len_member,*pos_member;
	char *hold=NULL,*s=NULL,*label=NULL;
	char *temp=NULL,*temp1=NULL;
	RDAwdgt *wdgt=NULL;

	hold=Rmalloc(length+2);
	memset(hold,0,length+1);
	temp=Rmalloc(length+2);
	memset(temp,0,length+1);
	lcount=0;
	cur=0;
	while(cur<(definelist->numrscs-6))
	{
		for(x=1;x<(definelist->numrscs-6);x+=2)
		{
			pos_member=definelist->rscs+x;
			len_member=definelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					if(label!=NULL) Rfree(label);	
	
					w=FINDWDGT(definelist->scn,pos_member->rscrname);
					if(w!=(-1))	
					{
						wdgt=definelist->scn->wdgts+w;
						label=stralloc(wdgt->label);
						if(!isEMPTY(label))
						{
							stoupper(label);
							if(RDAstrstr(label," POSITION") && (RDAstrlen(label)>9)) label[RDAstrlen(label)-9]=0;
						}
					}
					if(!isEMPTY(label)) 
					{
						s=stralloc(label);
					} else {
						s=stralloc(pos_member->rscrname);
						s[RDAstrlen(s)-9]=0;
					}
					if(RDAstrlen(s)>*len_member->value.short_value)
					{	
						s[*len_member->value.short_value]=0;
					} else if(RDAstrlen(s)<*len_member->value.short_value)
					{
						s=Rrealloc(s,*len_member->value.short_value+1);
						s[*len_member->value.short_value]=0;
					} 
					if(*len_member->value.short_value<RDAstrlen(s))
					{
						sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(s!=NULL?s:""));
					} else {
						sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(s!=NULL?s:""));
					}
					temp1=&temp[lcount];
					for(z=0;z<(*len_member->value.short_value);++z)
					{
						temp1[z]='-';
					}
					temp1[z]=' ';
					lcount+=*len_member->value.short_value+1;
				}
				if(s!=NULL) Rfree(s);
			}
		}
		cur++;
	}
	if(s!=NULL) Rfree(s);
	*seperator=temp;
	return(hold);
}
static void quit_printbrowse(RDArsrc *prsrc)
{
#ifdef USE_RDA_DIAGNOSTICS
        if(diaggui)
        {
                prterr("DIAG quit_printbrowse Quiting Print Browse List.");
        }
#endif /* ifdef USE_RDA_DIAGNOSTICS */
        if(prsrc!=NULL) free_rsrc(prsrc);
}
static char in_definelist_string(RDArsrc *definelist,char *name)
{
	RDArmem *len_member,*pos_member;
	short lpos=0,ppos=0;

	if(isEMPTY(name)) return(FALSE);
	if(RDAstrstr(name,"MONTH")) return(FALSE);
	if(RDAstrstr(name,"YEAR")) return(FALSE);
	if(RDAstrstr(name,"NUMBER")) return(FALSE);
	memset(stemp,0,101);
	sprintf(stemp,"%s POSITION",(name!=NULL ? name:""));
	ppos=FINDRSC(definelist,stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%s LENGTH",(name!=NULL ? name:""));
	lpos=FINDRSC(definelist,stemp);
	if(lpos==(-1) || ppos==(-1)) return(FALSE);
	pos_member=definelist->rscs+ppos;
	len_member=definelist->rscs+lpos;
	if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
	{
		return(TRUE);
	}
	return(FALSE);
}
static void MakeBrowseListTotalTemp(RDArsrc *r,short fileno,RDArsrc *d)
{
	char *name=NULL;
	short nofields=0,x=0;
	NRDfield *fields=NULL,*fldx=NULL;
	RDAvirtual *v=NULL;

	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if((in_definelist_string(d,fldx->name) || ((!RDAstrcmp(fldx->name,"DELETEFLAG")) && RDAstrcmp(fldx->name,"JOB NUMBER")) && RDAstrcmp(fldx->name,"TRANSACTION NUMBER") && RDAstrcmp(fldx->name,"DEFINITION TYPE") && RDAstrcmp(fldx->name,"ACCOUNT TYPE")))
			{
			switch(fldx->type)
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
					break;
				case BOOLNS:
				case CHARACTERS:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					if(!alreadyrsrc(name,r))
					{
						makefieldrsrc(r,name,LONGV,8,FALSE);
					}
					if(name!=NULL) Rfree(name);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					if(!alreadyrsrc(name,r))
					{
						makefieldrsrc(r,name,fldx->type,fldx->len,FALSE);
					}
					if(name!=NULL) Rfree(name);
					break;
				case SHORTV:
				case SSHORTV:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					if(!alreadyrsrc(name,r))
					{
						makefieldrsrc(r,name,fldx->type,fldx->len,FALSE);
					}
					if(name!=NULL) Rfree(name);
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					if(!alreadyrsrc(name,r))
					{
						makefieldrsrc(r,name,fldx->type,fldx->len,FALSE);
					}
					if(name!=NULL) Rfree(name);
					break;
				default:
				case BUTTONS:
					break;
			}
			}
		}
	}
	if(d!=NULL)
	{
		if(d->virflds!=NULL)
		{
			for(x=0,v=d->virflds;x<d->num;++x,++v)
			{
				if(in_definelist_string(d,v->name))
				{
				switch(v->type)
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
						break;
					case BOOLNS:
					case CHARACTERS:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if(!alreadyrsrc(name,r))
						{
							makefieldrsrc(r,name,LONGV,8,FALSE);
						}
						if(name!=NULL) Rfree(name);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if(!alreadyrsrc(name,r))
						{
							makefieldrsrc(r,name,v->type,v->len,FALSE);
						}
						if(name!=NULL) Rfree(name);
						break;
					case SHORTV:
					case SSHORTV:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if(!alreadyrsrc(name,r))
						{
							makefieldrsrc(r,name,v->type,v->len,FALSE);
						}
						if(name!=NULL) Rfree(name);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if(!alreadyrsrc(name,r))
						{
							makefieldrsrc(r,name,v->type,v->len,FALSE);
						}
						if(name!=NULL) Rfree(name);
						break;
					default:
					case BUTTONS:
						break;
				}
				}
			}
		}
	}
}
static void printbrowselist(RDArsrc *prsrc,MakeBrowseList *b)
{
	RDA_PFILE *fp=NULL;
	int x=0;
	char *outdevice=NULL,*message=NULL;
/*
	char *temp=NULL,*temp_head=NULL;
	RDArmem *member=NULL;
	DFincvir *i=NULL;
	RDAvirtual *v=NULL;
*/
	int pages=0,lines=0;
	int line_length=79,max_lines=60;
	RDArsrc *definelist=NULL;
	RDArsrc *mainrsrc=NULL;
	RDArsrc *tmprsrc=NULL;
	char *defineliststring=NULL,*sub_title=NULL;
	char *temp=NULL,*temp1=NULL,*temp_head=NULL;
	char **tmplist=NULL,print_style=0,sc=FALSE;
	char *tempdefineliststring=NULL,*seperator=NULL;
	RDArmem *len_member,*pos_member,*member=NULL;
	short length=0;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	readwidget(prsrc,"PRINT SEARCH CRITERIA");
	FINDRSCGETCHAR(prsrc,"PRINT SEARCH CRITERIA",&sc);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
			if((!strncmp(outdevice,"viewpdf",7) || !strncmp(outdevice,"VIEWPDF",7) || !strncmp(outdevice,"dd of=viewpdf",13)))
			{
				readwidget(prsrc,"COLUMNS");
				FINDRSCGETINT(prsrc,"COLUMNS",&line_length);
				readwidget(prsrc,"LINES");
				FINDRSCGETINT(prsrc,"LINES",&max_lines);

				if((max_lines<62)&&(line_length<80)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LETTER",fp);
				} else if((max_lines<45)&&(line_length<105)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LETTER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);

				} else if((max_lines<80)&&(line_length<135)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LETTER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else if((max_lines<61)&&(line_length<175)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LETTER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else if((max_lines<75)&&(line_length<80)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEGAL",fp);
				} else if((max_lines<45)&&(line_length<133)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEGAL",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else if((max_lines<102)&&(line_length<133)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEGAL",fp);
				} else if((max_lines<61)&&(line_length<224)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEGAL",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else if((max_lines<80)&&(line_length<105)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_POSTER",fp);
				} else if((max_lines<60)&&(line_length<133)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_POSTER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else if((max_lines<105)&&(line_length<175)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_POSTER",fp);
				} else if((max_lines<80)&&(line_length<225)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_POSTER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else if((max_lines<95)&&(line_length<105)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEDGER",fp);
				} else if((max_lines<60)&&(line_length<160)&&(print_style=0)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEDGER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else if((max_lines<130)&&(line_length<175)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEDGER",fp);
				} else if((max_lines<80)&&(line_length<275)&&(print_style=1)) 
				{
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEDGER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				} else {
					SelectPrintTypeByName(outdevice,"PAGE_SIZE_LEDGER",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
				}

			}

			tmprsrc=RDArsrcNEW(b->mainrsrc->module,b->mainrsrc->screen);
			MakeBrowseListTotalTemp(tmprsrc,b->fileno,b->definelist);
			ZeroBrowseListTotals(tmprsrc,b->fileno,b->definelist);

			SelectPrintType(outdevice,print_style,fp);
			mainrsrc=b->mainrsrc;
			definelist=b->definelist;
			readwidget(prsrc,"SUB-TITLE");
			FINDRSCGETSTRING(prsrc,"SUB-TITLE",&sub_title);
			readwidget(prsrc,"COLUMNS");
			FINDRSCGETINT(prsrc,"COLUMNS",&line_length);
			if(line_length<=0) line_length=79;
			else line_length-=1;
			readwidget(prsrc,"LINES");
			FINDRSCGETINT(prsrc,"LINES",&max_lines);
			if(max_lines<=0) max_lines=60;
/* determining the maximum length of the viewable browse line */
			for(x=1,length=0;x<(definelist->numrscs-6);x+=2)
			{
				pos_member=definelist->rscs+x;
				len_member=definelist->rscs+(x+1);
				if(*pos_member->value.short_value>0)
				{
					length+=(*len_member->value.short_value+2);
				}
        		}
			defineliststring=make_definelist_string(definelist,length,&seperator);
			temp_head=stralloc("");
			tempdefineliststring=Rmalloc(RDAstrlen(defineliststring)+4);
			sprintf(tempdefineliststring,"%s",(defineliststring!=NULL?defineliststring:""));
			if(defineliststring!=NULL) Rfree(defineliststring);
			print_browse_header(fp,&lines,&pages,
				"XPERT BROWSE PRINTOUT",sub_title,
				line_length,max_lines,
				mainrsrc->module,mainrsrc->screen,tempdefineliststring,seperator);
			if(temp!=NULL) Rfree(temp);
			for(x=0,tmplist=b->list->string;x<b->list->num;++x,++tmplist)
			{
				CLEAR_SCREEN_VIRTUAL(b->definelist);
				EQLNRDKEY(b->fileno,b->keyno,b->list->key[x]);
				IncrementBrowseListTotals(tmprsrc,b->fileno,b->definelist);
				temp=Rmalloc(1+(*tmplist!=NULL?RDAstrlen(*tmplist):3)+4);
				sprintf(temp,"%s",(*tmplist!=NULL?*tmplist:""));
				fixoutput(fp,temp,&pages,&lines,line_length,temp_head,0,2);
				print_browse_header(fp,&lines,&pages,
					"XPERT BROWSE PRINTOUT",sub_title,
					line_length,max_lines,
					mainrsrc->module,mainrsrc->screen,tempdefineliststring,seperator);
				if(temp!=NULL) Rfree(temp);
			}
			fixoutput(fp,"",&pages,&lines,line_length,temp_head,0,2);
			print_browse_header(fp,&lines,&pages,
				"XPERT BROWSE PRINTOUT",sub_title,
				line_length,max_lines,
				mainrsrc->module,mainrsrc->screen,tempdefineliststring,seperator);
			if(tmprsrc->rscs!=NULL)
			{
				for(x=0,member=tmprsrc->rscs;x<tmprsrc->numrscs;++x,++member)
				{
					temp=Rmalloc(RDAstrlen(member->rscrname)+81);
					switch(member->field_type)
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
							break;
						case DOLLARS:
							temp1=famt(*member->value.float_value,20);
							sprintf(temp,"%40s: %s",member->rscrname,(temp1!=NULL ? temp1:""));
							if(temp1!=NULL) Rfree(temp1);
							break;
						case DOLLARS_NOCENTS:
							temp1=famtncents((*member->value.float_value*100),20,' ');
							sprintf(temp,"%40s: %s",member->rscrname,(temp1!=NULL ? temp1:""));
							if(temp1!=NULL) Rfree(temp1);
							break;
						case DOUBLEV:
						case SDOUBLEV:
							sprintf(temp,"%40s: %.0f",member->rscrname,*member->value.float_value);
							break;
						case DECIMALV:
						case SDECIMALV:
							sprintf(temp,"%40s: %.04f",member->rscrname,*member->value.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							sprintf(temp,"%40s: %d",member->rscrname,*member->value.short_value);
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							if(RDAstrcmp(member->rscrname,"DELETEFLAG TOTAL"))
							{
							sprintf(temp,"%40s: %d",member->rscrname,*member->value.integer_value);
							} else {
							sprintf(temp,"%40s: %d","RECORD COUNT",*member->value.integer_value);
							}
							break;
						default:
						case BUTTONS:
							break;
					}
					if(!isEMPTY(temp))
					{
					fixoutput(fp,temp,&pages,&lines,line_length,temp_head,0,2);
					print_browse_header(fp,&lines,&pages,"XPERT BROWSE PRINTOUT",sub_title,
						line_length,max_lines,mainrsrc->module,mainrsrc->screen,
						tempdefineliststring,seperator);
					}
					if(temp!=NULL) Rfree(temp);
				}
			}

			fixoutput(fp,"",&pages,&lines,line_length,temp_head,0,2);
			print_browse_header(fp,&lines,&pages,
				"XPERT BROWSE PRINTOUT",sub_title,
				line_length,max_lines,
				mainrsrc->module,mainrsrc->screen,tempdefineliststring,seperator);
			if(sc==TRUE)
			{
			fixoutput(fp,"Search Criteria:",&pages,&lines,line_length,temp_head,0,2);
			print_browse_header(fp,&lines,&pages,
				"XPERT BROWSE PRINTOUT",sub_title,
				line_length,max_lines,
				mainrsrc->module,mainrsrc->screen,tempdefineliststring,seperator);
			FINDRSCGETSTRING(mainrsrc,"SEARCH CRITERIA",&temp1);
			temp=Rmalloc((!isEMPTY(temp1)?RDAstrlen (temp1):3)+8);
			sprintf(temp,"[%s]",(temp1!=NULL?temp1:""));
			fixoutput(fp,temp,&pages,&lines,line_length,temp_head,0,2);
			print_browse_header(fp,&lines,&pages,
				"XPERT BROWSE PRINTOUT",sub_title,
				line_length,max_lines,
				mainrsrc->module,mainrsrc->screen,tempdefineliststring,seperator);
			}
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			if(temp_head!=NULL) Rfree(temp_head);
			if(tempdefineliststring!=NULL) Rfree(tempdefineliststring);
			if(seperator!=NULL) Rfree(seperator);
			if(sub_title!=NULL) Rfree(sub_title);
			RDA_fprintf(fp,"\r\n\f");
			SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attempt to the device [%s] failed when trying to open pipe.",outdevice);
			ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!",message,NULL,FALSE);
			prterr("Error the output Attempt to the device [%s] failed when trying to open pipe.",outdevice);
			if(message!=NULL) Rfree(message);
		}
	} else {
		message=Rmalloc(50);
		sprintf(message,"No output device given.");
		ERRORDIALOG("INVALID OUTPUT DEVICE!",message,NULL,FALSE);
		prterr("Error no output device given.");
		if(message!=NULL) Rfree(message);
	}
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	if(outdevice!=NULL) Rfree(outdevice);
}
void printbrowse(RDArsrc *parent,MakeBrowseList *b)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;
	char *sub_title="",normal_print=TRUE,sc=FALSE;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG printbrowse Displaying the Print Browse List in preparation to print Screen Resources.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	prsrc=RDArsrcNEW("GUI","PRINT BROWSE SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	addstdrsrc(prsrc,"PRINT SEARCH CRITERIA",BOOLNS,1,&sc,TRUE);
	x=60;
	addstdrsrc(prsrc,"LINES",LONGV,3,&x,TRUE);
	x=128;
	addstdrsrc(prsrc,"COLUMNS",LONGV,3,&x,TRUE);
	addstdrsrc(prsrc,"SUB-TITLE",VARIABLETEXT,0,sub_title,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printbrowselist,b);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printbrowse,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(xmakescrn(prsrc,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,NULL,NULL,NULL,NULL,parent,__LINE__,__FILE__))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed f or the custom screen PRINT BROWSE SCREEN in GUI module. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_printbrowse(prsrc);
	}
}
void browselistcsv(RDArsrc *r,MakeBrowseList *m)
{
	FILE *fp=NULL;
	int x=0,cur=0,k=0,count=0,z=0;
	RDArsrc *definelist=NULL;
	RDArsrc *mainrsrc=NULL;
	char *temp=NULL,tempx[1024];
	char *s=NULL;
	RDArmem *len_member,*pos_member;
	NRDfield *field=NULL;
	RDAvirtual *v=NULL;

	memset(tempx,0,1024);
	sprintf(tempx,"%s/%d.csv",CURRENTDIRECTORY,RGETPID());
	unlink(tempx);
	fp=fopen(tempx,"w");
	if(fp!=NULL)
	{
		mainrsrc=m->mainrsrc;
		definelist=m->definelist;
		cur=0;
		count=0;
		while(cur<(definelist->numrscs-6))
		{
			for(x=1;x<(definelist->numrscs-6);x+=2)
			{
				pos_member=definelist->rscs+x;
				len_member=definelist->rscs+(x+1);
				if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
				{
					if(*pos_member->value.short_value==cur)
					{
						if(count>0) 
						{
							fprintf(fp,",");
						}
						if(s!=NULL) Rfree(s);
						s=stralloc(pos_member->rscrname);
						s[RDAstrlen(s)-9]=0;
						fprintf(fp,"\"%s\"",(s!=NULL ? s:""));
						++count;
					}
				}
			}
			++cur;
		}
		fprintf(fp,"\r\n");
		for(k=0;k<m->list->num;++k)
		{
			CLEAR_SCREEN_VIRTUAL(m->definelist);
			CLEAR_SCREEN_VIRTUAL(m->searchrsrc);
			EQLNRDKEY(m->fileno,m->keyno,m->list->key[k]);
			SetRDArsrcFiles(m->fileno,m->definelist);
			cur=0;
			count=0;
			while(cur<(definelist->numrscs-6))
			{
				for(x=1;x<(definelist->numrscs-6);x+=2)
				{
					pos_member=definelist->rscs+x;
					len_member=definelist->rscs+(x+1);
					if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
					{
						if(*pos_member->value.short_value==cur)
						{
							if(s!=NULL) Rfree(s);
							s=stralloc(pos_member->rscrname);
							s[RDAstrlen(s)-9]=0;
							if(m->file_type==1)
							{
								field=FLDNRD(m->fileno,s);
							} else {
								field=FLDNRDsec(m->fileno,s,TRUE,FALSE,FALSE);
							}
							if(field!=NULL)
							{
								if(count>0) 
								{
									fprintf(fp,",");
								}
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
										fprintf(fp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
										break;
									case DECIMALV:
										fprintf(fp,"\"%f\"",*field->data.float_value);
										break;
									case SDECIMALV:
										if(*field->data.float_value>=0) 
										{
											fprintf(fp,"\"%f\"",*field->data.float_value);
										} else {
											fprintf(fp,"\"%f-\"",(*field->data.float_value*(-1)));
										}
										break;
									case SHORTV:
										fprintf(fp,"\"%d\"",*field->data.short_value);
										break;
									case SSHORTV:
										if(*field->data.short_value>=0) 
										{
											fprintf(fp,"\"%d\"",*field->data.short_value);
										} else {
											fprintf(fp,"\"%d-\"",(*field->data.short_value*(-1)));
										}
										break;

									case LONGV:
										fprintf(fp,"\"%d\"",*field->data.integer_value);
										break;
									case SLONGV:
										if(*field->data.integer_value>=0) 
										{
											fprintf(fp,"\"%d\"",*field->data.integer_value);
										} else {
											fprintf(fp,"\"%d-\"",(*field->data.integer_value*(-1)));
										}
										break;
									case DOLLARS_NOCENTS:
										temp=famtncents((*field->data.float_value)*100,*len_member->value.short_value,' ');
										fprintf(fp,"\"%s\"",(temp!=NULL ? temp:""));
										if(temp!=NULL) Rfree(temp);
										break;
									case DOLLARS:
										temp=famt(*field->data.float_value,*len_member->value.short_value);
										fprintf(fp,"\"%s\"",(temp!=NULL ? temp:""));
										if(temp!=NULL) Rfree(temp);
										break;
									case DOUBLEV:
										fprintf(fp,"\"%.0f\"",*field->data.float_value);
										break;
									case SDOUBLEV:
										if(*field->data.float_value>=0)
										{
											fprintf(fp,"\"%.0f\"",*field->data.float_value);
										} else {
											fprintf(fp,"\"%.0f-\"",(*field->data.float_value*(-1)));
										}
										break;
									case BOOLNS:
										if(*len_member->value.short_value>2)
										{
											fprintf(fp,"\"%s\"",(*field->data.string_value ? "Yes":"No"));
										} else {
											fprintf(fp,"\"%s\"",(*field->data.string_value ? "Y":"N"));
										}
										break;
									case CHARACTERS:
										fprintf(fp,"\"%c\"",*field->data.string_value);
										break;
									case BUTTONS:
									case SCROLLEDLIST:
										break;
									default:
										break;
								}
								++count;
							} else {
								if(m->definelist->virflds!=NULL)
								{
									for(z=0,v=m->definelist->virflds;z<m->definelist->num;++z,++v)
									{
										if(!RDAstrcmp(v->name,s)) break;
									}
									if(z<m->definelist->num)
									{
										if(!v->computed) computevirtual(v,SCRNvirtualSubData,m->definelist);
										if(count>0) 
										{
											fprintf(fp,",");
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
												fprintf(fp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
												break;
											case DECIMALV:
												fprintf(fp,"\"%f\"",*v->value.float_value);
												break;
											case SDECIMALV:
												if(*v->value.float_value>=0) 
												{
													fprintf(fp,"\"%f\"",*v->value.float_value);
												} else {
													fprintf(fp,"\"%f-\"",(*v->value.float_value*(-1)));
												}
												break;
											case SHORTV:
												fprintf(fp,"\"%d\"",*v->value.short_value);
												break;
											case SSHORTV:
												if(*v->value.short_value>=0) 
												{
													fprintf(fp,"\"%d\"",*v->value.short_value);
												} else {
													fprintf(fp,"\"%d-\"",(*v->value.short_value*(-1)));
												}
												break;

											case LONGV:
												fprintf(fp,"\"%d\"",*v->value.integer_value);
												break;
											case SLONGV:
												if(*v->value.integer_value>=0) 
												{
													fprintf(fp,"\"%d\"",*v->value.integer_value);
												} else {
													fprintf(fp,"\"%d-\"",(*v->value.integer_value*(-1)));
												}
												break;
											case DOLLARS_NOCENTS:
												temp=famtncents((*v->value.float_value)*100,*len_member->value.short_value,' ');
												fprintf(fp,"\"%s\"",(temp!=NULL ? temp:""));
												if(temp!=NULL) Rfree(temp);
												break;
											case DOLLARS:
												temp=famt(*v->value.float_value,*len_member->value.short_value);
												fprintf(fp,"\"%s\"",(temp!=NULL ? temp:""));
												if(temp!=NULL) Rfree(temp);
												break;
											case DOUBLEV:
												fprintf(fp,"\"%.0f\"",*v->value.float_value);
												break;
											case SDOUBLEV:
												if(*v->value.float_value>=0)
												{
													fprintf(fp,"\"%.0f\"",*v->value.float_value);
												} else {
													fprintf(fp,"\"%.0f-\"",(*v->value.float_value*(-1)));
												}
												break;
											case BOOLNS:
												if(*len_member->value.short_value>2)
												{
													fprintf(fp,"\"%s\"",(*v->value.string_value ? "Yes":"No"));
												} else {
													fprintf(fp,"\"%s\"",(*v->value.string_value ? "Y":"N"));
												}
												break;
											case CHARACTERS:
												fprintf(fp,"\"%c\"",*v->value.string_value);
												break;
											case BUTTONS:
											case SCROLLEDLIST:
												break;
											default:
												break;
										}
										++count;
									}
								}
							}
						}
					}
				}
				++cur;
			}
			fprintf(fp,"\r\n");
		}
		fclose(fp);
		memset(stemp,0,101);
		sprintf(stemp,"%d.csv",RGETPID());
		DisplayFile(stemp);
	}
	if(s!=NULL) Rfree(s);
}
