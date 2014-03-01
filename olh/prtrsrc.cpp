#include <olh.hpp>

static void quit_printrsrcs(RDArsrc *prsrc)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG quit_printrsrcs Quiting Print Screen Resources.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void print_header(RDA_PFILE *fp,int *lines,int *pages,
	char *header,unsigned line_length,unsigned max_lines,
	char *module,char *file)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	if(fp!=NULL)
	{
	if((*lines>=max_lines) || (*pages==0 && *lines==0))
	{
		*lines=0;
		if(!(*pages==0 && *lines==0))
		{
			RDA_fprintf(fp,"\f");
		}
		RDA_fprintf(fp,"\n");
		*lines+=1;
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(header)+8+8+lead_ws+tail_ws)>line_length) header[(line_length-8-8-lead_ws-tail_ws)]=0;
		RDA_fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
		*lines+=1;
		lead_ws=(int)(RDAroundup((double)(line_length-(20+RDAstrlen(module)+RDAstrlen(file)))/2));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint((((double)(line_length-(20+RDAstrlen(module)+RDAstrlen(file)))/2)-14)));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(module)+RDAstrlen(file)+14+20+lead_ws+tail_ws)>(line_length))
		{
			RDA_fprintf(fp,"%*s Module [%s] %*sDate: %s\n",
				(int)(round(((double)(line_length-(11+RDAstrlen(module)))/2))),"",
				module,
				(int)(Rint((((double)(line_length-(11+RDAstrlen(module)))/2)-14))),"",
				date);
			RDA_fprintf(fp,"%*s Screen [%s]\n",
				(int)(round(((double)(line_length-(10+RDAstrlen(file)))/2))),"",
				file);
			*lines+=2;
		} else {
			RDA_fprintf(fp,"%*sModule [%s] Screen [%s] %*sDate: %s\n",
				lead_ws,"",module,file,tail_ws,"",date);
			*lines+=1;
		}
		lead_ws=(int)(RDAroundup(((double)(line_length-(13+RDAstrlen(USERLOGIN)))/2)));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(13+RDAstrlen(USERLOGIN)))/2)-14));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(USERLOGIN)+13+14+lead_ws+tail_ws)>line_length)
		{
			RDA_fprintf(fp,"%*s %s %*sTime: %s\n",
				(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
				USERLOGIN,
				(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
				timex);
			*lines+=1;
		} else {
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\n",
				lead_ws,"",USERLOGIN,tail_ws,"",timex);
			*lines+=1;
		}
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
	}
	RDA_fprintf(fp,"\n");
	*lines+=1;
	}
}
static void fixoutput(RDA_PFILE *fp,char *data,int *pages,int *numpagelines,unsigned line_length,char *initstring,int add_length)
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
		temp2=Rmalloc(initoffset+add_length+1);
		sprintf(temp2,"%*s",initoffset+add_length,"");
		if(y<length)
		{
			RDA_fprintf(fp,"%s%.*s\n",initstring,y+1,data);
			fixoutput(fp,data+y+1,pages,numpagelines,line_length,temp2,0);
		} else {
			RDA_fprintf(fp,"%s%.*s",initstring,length,data);
			if(temp!=NULL || temp1!=NULL)
			{
				fixoutput(fp,data+length+1,pages,numpagelines,line_length,temp2,0);
			}
		}
		if(temp2!=NULL) Rfree(temp2);
	}
	}
}
static void printscrnrsrcs(RDArsrc *prsrc,RDArsrc *parent)
{
	RDA_PFILE *fp=NULL;
	int x=0;
	RDArmem *member=NULL;
	char *outdevice=NULL,*message=NULL;
	char *temp=NULL,*temp_head=NULL;
	DFincvir *i=NULL;
	RDAvirtual *v=NULL;
	int pages=0,lines=0;
	unsigned line_length=79,max_lines=60;

	if(RDA_GetEnv("SAVE_RESOURCES")!=NULL) SAVE_RSRC_BINARY(parent);
	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			print_header(fp,&lines,&pages,
				"XPERT SCREEN RESOURCES AVAILABLE",
				line_length,max_lines,
				parent->module,parent->screen);
			if(parent->incvir!=NULL)
			{
				print_header(fp,&lines,&pages,
					"XPERT SCREEN RESOURCES AVAILABLE",
					line_length,max_lines,
					parent->module,parent->screen);
				RDA_fprintf(fp,"Files Available:");
				print_header(fp,&lines,&pages,
					"XPERT SCREEN RESOURCES AVAILABLE",
					line_length,max_lines,
					parent->module,parent->screen);
				for(x=0,i=parent->incvir;x<parent->numvir;++x,++i)
				{
					temp_head=stralloc("   ");
					temp=Rmalloc(50+RDAstrlen(i->module)+RDAstrlen(i->file)+RDAstrlen(i->keyname)+6+1);
					sprintf(temp,"Module [%s] File [%s] Keyname [%s] File Number [%6d]",(i->module!=NULL?i->module:""),(i->file!=NULL?i->file:""),(i->keyname!=NULL?i->keyname:""),i->fileno);
					if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
					{
						fixoutput(fp,temp,&pages,&lines,line_length,temp_head,3);
						print_header(fp,&lines,&pages,
							"XPERT SCREEN RESOURCES AVAILABLE",
							line_length,max_lines,
							parent->module,parent->screen);
					} else {
						RDA_fprintf(fp,"%s%s",temp_head,temp);
						print_header(fp,&lines,&pages,
							"XPERT SCREEN RESOURCES AVAILABLE",
							line_length,max_lines,
							parent->module,parent->screen);
					}
					if(temp_head!=NULL) Rfree(temp_head);
					if(temp!=NULL) Rfree(temp);
				}
			}
			if(parent->virflds!=NULL)
			{
				RDA_fprintf(fp,"Virtual Fields Displayed:");
				print_header(fp,&lines,&pages,
					"XPERT SCREEN RESOURCES AVAILABLE",
					line_length,max_lines,
					parent->module,parent->screen);
				for(x=0,v=parent->virflds;x<parent->num;++x,++v)
				{
					temp_head=stralloc("   ");
					temp=Rmalloc(40+RDAstrlen(v->name)+RDAstrlen(standardfieldtypes[v->type])+3+2+4+1);
					sprintf(temp,"Name [%s] Type [%3d]:[%s] When [%2d] Length [%4d]",v->name,v->type,standardfieldtypes[v->type],v->when,v->len);
					if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
					{
						fixoutput(fp,temp,&pages,&lines,line_length,temp_head,3);
						print_header(fp,&lines,&pages,
							"XPERT SCREEN RESOURCES AVAILABLE",
							line_length,max_lines,
							parent->module,parent->screen);
					} else {
						RDA_fprintf(fp,"%s%s",temp_head,temp);
						print_header(fp,&lines,&pages,
							"XPERT SCREEN RESOURCES AVAILABLE",
							line_length,max_lines,
							parent->module,parent->screen);
					}
					if(temp_head!=NULL) Rfree(temp_head);
					if(temp!=NULL) Rfree(temp);
					temp_head=stralloc("      Expression: [");
					temp=Rmalloc(1+RDAstrlen(v->expression)+1);
					sprintf(temp,"%s]",v->expression);
					if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
					{
						fixoutput(fp,temp,&pages,&lines,line_length,temp_head,0);
						print_header(fp,&lines,&pages,
							"XPERT SCREEN RESOURCES AVAILABLE",
							line_length,max_lines,
							parent->module,parent->screen);
					} else {
						RDA_fprintf(fp,"%s%s",temp_head,temp);
						print_header(fp,&lines,&pages,
							"XPERT SCREEN RESOURCES AVAILABLE",
							line_length,max_lines,
							parent->module,parent->screen);
					}
					if(temp_head!=NULL) Rfree(temp_head);
					if(temp!=NULL) Rfree(temp);
				}
			}
			RDA_fprintf(fp,"Screen Resources Available:");
			print_header(fp,&lines,&pages,
				"XPERT SCREEN RESOURCES AVAILABLE",
				line_length,max_lines,
				parent->module,parent->screen);
			for(x=0,member=parent->rscs;x<parent->numrscs;++x,++member)
			{
				RDA_fprintf(fp,"   Name [%s]  Field Type [%d]:[%s]",member->rscrname,member->field_type,standardfieldtypes[member->field_type]);
				print_header(fp,&lines,&pages,
					"XPERT SCREEN RESOURCES AVAILABLE",
					line_length,max_lines,
					parent->module,parent->screen);
				RDA_fprintf(fp,"      Length [%d]  Displayed [%s]  Editable [%s]  Sensitive [%s]",member->field_length,(member->w!=NULL?"Yes":"No"),(member->editable==TRUE?"Yes":"No"),(member->sensitive==TRUE?"Yes":"No"));
				print_header(fp,&lines,&pages,
					"XPERT SCREEN RESOURCES AVAILABLE",
					line_length,max_lines,
					parent->module,parent->screen);
				RDA_fprintf(fp,"      User Editable [%s]  User Sensitive [%s]",(member->user_editable==TRUE?"Yes":"No"),(member->user_sensitive==TRUE?"Yes":"No"));
				print_header(fp,&lines,&pages,
					"XPERT SCREEN RESOURCES AVAILABLE",
					line_length,max_lines,
					parent->module,parent->screen);
				temp_head=stralloc("      Value: [");
				switch(member->field_type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case PLAINTEXT:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						temp=Rmalloc(1+(member->value.string_value!=NULL?RDAstrlen(member->value.string_value):3)+1);
						sprintf(temp,"%s]",(member->value.string_value!=NULL?member->value.string_value:"NULL"));
						break;
					case BOOLNS:
						temp=Rmalloc(1+3+1);
						sprintf(temp,"%s]",(*member->value.string_value?"Yes":"No"));
						break;
					case CHARACTERS:
						temp=Rmalloc(1+1+1);
						sprintf(temp,"%c]",*member->value.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case SDECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
						sprintf(stemp,"%f]",*member->value.float_value);
						temp=stralloc(stemp);
						break;
					case SHORTV:
					case SSHORTV:
						sprintf(stemp,"%d]",*member->value.short_value);
						temp=stralloc(stemp);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						sprintf(stemp,"%d]",*member->value.integer_value);
						temp=stralloc(stemp);
						break;
					case BUTTONS:
						temp=Rmalloc(1+3+1);
						sprintf(temp,"%s]","n/a");
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s].",member->field_type,member->rscrname,parent->module,parent->screen);
						break;
				}
				if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
				{
					fixoutput(fp,temp,&pages,&lines,line_length,temp_head,3);
					print_header(fp,&lines,&pages,
						"XPERT SCREEN RESOURCES AVAILABLE",
						line_length,max_lines,
						parent->module,parent->screen);
				} else {
					RDA_fprintf(fp,"%s%s",temp_head,temp);
					print_header(fp,&lines,&pages,
						"XPERT SCREEN RESOURCES AVAILABLE",
						line_length,max_lines,
						parent->module,parent->screen);
				}
				if(temp_head!=NULL) Rfree(temp_head);
				if(temp!=NULL) Rfree(temp);
			}
			RDA_fprintf(fp,"\f\n");
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
	if(outdevice!=NULL) Rfree(outdevice);
}
void printrsrcs(RDArsrc *r,RDArsrc *parent)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG printrsrcs Displaying the Print Screen in preparation to print Screen Resources.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printscrnrsrcs,parent);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printrsrcs,prsrc);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,parent);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(xmakescrn(prsrc,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,NULL,NULL,NULL,NULL,parent,__LINE__,__FILE__))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_printrsrcs(prsrc);
	}
}
