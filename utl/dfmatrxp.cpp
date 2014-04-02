#include <mix.hpp>

extern RDAMATRIX *DFV;
extern char *module;
extern APPlib *rowlist,*columnlist;
static APPlib *dumprttypes=NULL;
extern void readmatrix(void);
extern void changeRangeRows(RDArsrc *);
extern void changeRangeCols(RDArsrc *);
extern void changeRangeCells(RDArsrc *);
extern void CreateColumnList(void);
extern void CreateRowList(void);
static void print_regular_matrix(RDA_PFILE *,char,int,int,char,int,int,int,int);
static void print_reverse_matrix(RDA_PFILE *,char,int,int,char,int,int,int,int);
static void print_linear_matrix(RDA_PFILE *,char,int,int,int,int);

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
			RDA_fprintf(fp,"%s%.*s\r\n",(initstring!=NULL ? initstring:""),y+1,data);
			fixoutput(fp,data+y+1,pages,numpagelines,line_length,temp2,add_length2,0);
		} else {
			RDA_fprintf(fp,"%s%.*s",(initstring!=NULL ? initstring:""),length,data);
			if(temp!=NULL || temp1!=NULL)
			{
				fixoutput(fp,data+length+1,pages,numpagelines,line_length,temp2,add_length2,0);
			}
		}
		if(temp2!=NULL) Rfree(temp2);
	}
	}
}
void print_matrix_header(RDA_PFILE *fp,int *lines,int *pages,
	char *head,char *sub_head,unsigned line_length,unsigned max_lines,
	char *module,char *screen,char *page_header,char *page_subheader)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	char *header=NULL,*sub_header=NULL;

	if(fp!=NULL)
	{
	if(head!=NULL) header=stralloc(head);
	else header=stralloc("Matrix definition");
	sub_header=stralloc(sub_head);
	if((*lines>=max_lines) || (*pages==0 && *lines==0))
	{
		*lines=0;
		if(!(*pages==0 && *lines==0))
		{
			RDA_fprintf(fp,"\f");
		}
		RDA_fprintf(fp,"\r\n");
		*lines+=1;
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
		RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
			lead_ws,"",header,tail_ws,"",8,++*pages);
		*lines+=1;
		if(!isEMPTY(sub_header))
		{
			lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(sub_header)))/2));
			if(lead_ws<0) lead_ws=0;
			tail_ws=lead_ws;
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
			RDA_fprintf(fp,"%*s Matrix [%s]\r\n",
				(int)(round(((double)(line_length-(10+RDAstrlen(screen)))/2))),"",
				screen);
			*lines+=2;
		} else {
			RDA_fprintf(fp,"%*sModule [%s] Matrix [%s] %*sDate: %s\r\n",
				lead_ws,"",module,screen,tail_ws,"",date);
			*lines+=1;
		}
		lead_ws=(int)(RDAroundup(((double)(line_length-(13+RDAstrlen(USERLOGIN)))/2)));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(13+RDAstrlen(USERLOGIN)))/2)-14));
		if(tail_ws<0) tail_ws=0;
		if((RDAstrlen(USERLOGIN)+13+14+lead_ws+tail_ws)>line_length)
		{
			RDA_fprintf(fp,"%*s %s %*sTime: %s\r\n",
				(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
				USERLOGIN,
				(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
				timex);
			*lines+=1;
		} else {
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\r\n",
				lead_ws,"",USERLOGIN,tail_ws,"",timex);
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
	if(header!=NULL) Rfree(header);
	if(sub_header!=NULL) Rfree(sub_header);
	}
}
static void quit_printmatrix(RDArsrc *prsrc)
{
        if(prsrc!=NULL) free_rsrc(prsrc);
	if(dumprttypes!=NULL) freeapplib(dumprttypes);
}
static void printmatrixlist(RDArsrc *prsrc)
{
	RDA_PFILE *fp=NULL;
	char matrix_orientation=FALSE,*outdevice=NULL,*message=NULL;
	int from_cell=0,to_cell=0,fc=0,tc=0,fr=0,tr=0;
	int line_length=79,max_lines=60,s=0;
	char *temp=NULL,*temp1=NULL;
	char *seperator=NULL,rcell=FALSE,rc=FALSE,rr=FALSE;

	readallwidgets(prsrc);
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			FINDRSCGETINT(prsrc,"SIMPLE PRINTER TYPES",&s);
			switch(s)
			{
				default:
				case 0:
					SelectPrintTypeByName(outdevice,"NORMAL TEXT",fp);
					break;
				case 1:
					SelectPrintTypeByName(outdevice,"COMPRESSED TEXT",fp);
					break;
				case 2:
					SelectPrintTypeByName(outdevice,"NORMAL TEXT",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
					break;
				case 3:
					SelectPrintTypeByName(outdevice,"COMPRESSED TEXT",fp);
					SelectPrintTypeByName(outdevice,"LANDSCAPE",fp);
					break;
			}
			FINDRSCGETINT(prsrc,"CHARACTERS",&line_length);
			FINDRSCGETINT(prsrc,"LINES",&max_lines);
			FINDRSCGETCHAR(prsrc,"REVERSE ORIENTATION",
				&matrix_orientation);
			FINDRSCGETCHAR(prsrc,"RANGE ON ROWS",&rr);
			FINDRSCGETINT(prsrc,"FROM ROW LIST",&fr);
			FINDRSCGETINT(prsrc,"TO ROW LIST",&tr);
			FINDRSCGETCHAR(prsrc,"RANGE ON COLUMNS",&rc);
			FINDRSCGETINT(prsrc,"FROM COLUMN LIST",&fc);
			FINDRSCGETINT(prsrc,"TO COLUMN LIST",&tc);
			FINDRSCGETCHAR(prsrc,"RANGE ON CELLS",&rcell);
			FINDRSCGETINT(prsrc,"FROM CELL",&from_cell);
			FINDRSCGETINT(prsrc,"TO CELL",&to_cell);
			if(DFV->locate_method==0)
			{
				if(matrix_orientation==FALSE)
				{
					print_regular_matrix(fp,rr,fr,tr,rc,fc,tc,line_length,max_lines);
				} else { /* REVESE ORIENTATION */
					print_reverse_matrix(fp,rr,fr,tr,rc,fc,tc,line_length,max_lines);
				}
			} else {
				print_linear_matrix(fp,rcell,from_cell,to_cell,line_length,max_lines);
			}
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			if(seperator!=NULL) Rfree(seperator);
                        RDA_fprintf(fp,"\r\n\f");
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
static void changeptypes(RDArsrc *r)
{
	int x=0,l=60,w=80;

	FINDRSCGETINT(r,"SIMPLE PRINTER TYPES",&x);
	switch(x)
	{
		default:
		case 0:
			l=60;
			w=80;
			break;
		case 1:
			l=80;
			w=128;
			break;
		case 2:
			l=46;
			w=104;
			break;
		case 3:
			l=60;
			w=177;
			break;
	}
	FINDRSCSETINT(r,"LINES",l);
	FINDRSCSETINT(r,"CHARACTERS",w);
	updatersrc(r,"LINES");
	updatersrc(r,"CHARACTERS");
}
void printmatrix(RDArsrc *parent)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;
	char tvalue=TRUE;
	int x=0,y=0;

	readmatrix();
	prsrc=RDArsrcNEW(module,"PRINT MATRIX SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(dumprttypes!=NULL) freeapplib(dumprttypes);
	dumprttypes=APPlibNEW();
	addAPPlib(dumprttypes,"NORMAL TEXT");
	addAPPlib(dumprttypes,"COMPRESSED TEXT");
	addAPPlib(dumprttypes,"LANDSCAPE NORMAL TEXT");
	addAPPlib(dumprttypes,"LANDSCAPE COMPRESSED TEXT");
	y=0;
	addlstrsrc(prsrc,"SIMPLE PRINTER TYPES",&y,TRUE,changeptypes,dumprttypes->numlibs,
		&dumprttypes->libs,NULL);
	x=60;
	addstdrsrc(prsrc,"LINES",LONGV,3,&x,TRUE);
	x=80;
	addstdrsrc(prsrc,"CHARACTERS",LONGV,3,&x,TRUE);
	addstdrsrc(prsrc,"RANGE ON ROWS",BOOLNS,1,&tvalue,TRUE);
	CreateRowList();
	x=0;
	addlstrsrc(prsrc,"FROM ROW LIST",&x,TRUE,NULL,rowlist->numlibs,
		&rowlist->libs,NULL);
	x=rowlist->numlibs-1;
	addlstrsrc(prsrc,"TO ROW LIST",&x,TRUE,NULL,rowlist->numlibs,
		&rowlist->libs,NULL);
	addstdrsrc(prsrc,"RANGE ON COLUMNS",BOOLNS,1,&tvalue,TRUE);
	CreateColumnList();
	x=0;
	addlstrsrc(prsrc,"FROM COLUMN LIST",&x,TRUE,NULL,columnlist->numlibs,
		&columnlist->libs,NULL);
	x=columnlist->numlibs-1;
	addlstrsrc(prsrc,"TO COLUMN LIST",&x,TRUE,NULL,columnlist->numlibs,
		&columnlist->libs,NULL);
	addstdrsrc(prsrc,"RANGE ON CELLS",BOOLNS,1,&tvalue,TRUE);
	x=1;
	addstdrsrc(prsrc,"FROM CELL",LONGV,10,&x,TRUE);
	/* (JAB) Changing this to default the correct maximum */
	/*x=(DFV->num*DFV->xaxis.num*DFV->numdefs);*/
	x=(DFV->yaxis.num*DFV->xaxis.num);
	addstdrsrc(prsrc,"TO CELL",LONGV,10,&x,TRUE);
	tvalue=FALSE;
	addstdrsrc(prsrc,"REVERSE ORIENTATION",BOOLNS,1,&tvalue,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printmatrixlist,NULL);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printmatrix,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	if(DFV->locate_method==0) /* row and column */
	{
		/* FROM ROW LIST and TO ROW LIST shouldn't be editable/sensitive unless RANGE ON ROWS is true */
		FINDRSCSETFUNC(prsrc,"RANGE ON ROWS",changeRangeRows,NULL);
		/* FROM COLUMN LIST and TO COLUMN LIST shouldn't be editable/sensitive unless RANGE ON COLUMNS is true */
		FINDRSCSETFUNC(prsrc,"RANGE ON COLUMNS",changeRangeCols,NULL);
		FINDRSCSETEDITABLE(prsrc,"RANGE ON CELLS",FALSE);
		FINDRSCSETEDITABLE(prsrc,"FROM CELL",FALSE);
		FINDRSCSETEDITABLE(prsrc,"TO CELL",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"RANGE ON CELLS",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"FROM CELL",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"TO CELL",FALSE);
		changeRangeRows(prsrc);
		changeRangeCols(prsrc);
	} else { /* linear */
		/* FROM CELL and TO CELL shouldn't be editable unless RANGE ON CELLS is true */
		FINDRSCSETFUNC(prsrc,"RANGE ON CELLS",changeRangeCells,NULL);
		FINDRSCSETEDITABLE(prsrc,"RANGE ON ROWS",FALSE);
		FINDRSCSETEDITABLE(prsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETEDITABLE(prsrc,"TO ROW LIST",FALSE);
		FINDRSCSETEDITABLE(prsrc,"RANGE ON COLUMNS",FALSE);
		FINDRSCSETEDITABLE(prsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETEDITABLE(prsrc,"TO COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"RANGE ON ROWS",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"FROM ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"TO ROW LIST",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"RANGE ON COLUMNS",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"FROM COLUMN LIST",FALSE);
		FINDRSCSETSENSITIVE(prsrc,"TO COLUMN LIST",FALSE);
		changeRangeCells(prsrc);
	}
	DefaultScreens(prsrc);
	if(xmakescrn(prsrc,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,NULL,NULL,NULL,NULL,parent,__LINE__,__FILE__))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed f or the custom screen PRINT MATRIX SCREEN in GUI module. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_printmatrix(prsrc);
	}
}
static char *MakeMatrixHeader(char rc,int fc,int tc,int line_length,
	short Y_axis_max,short X_axis_max)
{
	char *tmp=NULL;
	AXISmem *mem;
	int x,count=0;
	short doit=FALSE;

	if(DFV->xaxis.mem!=NULL && DFV->xaxis.print_title1)
	{
		tmp=Rmalloc(Y_axis_max+2);
		sprintf(tmp,"%*s ",Y_axis_max,"");
		count=Y_axis_max+1;
		for(x=0,mem=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++mem)
		{
			if(rc)
			{
				if(x>=fc && x<=tc) doit=TRUE;
					else doit=FALSE;
			} else doit=TRUE;
			if(doit)
			{
				tmp=Rrealloc(tmp,count+X_axis_max+2);
				sprintf(&tmp[count],"%-*s ",X_axis_max,
					(mem->title1!=NULL ? mem->title1:""));
				count+=(X_axis_max+1);
			}
		}
	}
	if(count>line_length && line_length>0) tmp[line_length-1]=0;
	return(tmp);
}
static char *MakeMatrixSubHeader(char rc,int fc,int tc,int line_length,
	short Y_axis_max,short X_axis_max)
{
	char *tmp=NULL,*temp=NULL;
	AXISmem *mem;
	int x,count=0;
	short doit=FALSE;

	if(DFV->xaxis.mem!=NULL && DFV->xaxis.print_title2)
	{
		tmp=Rmalloc(Y_axis_max+2);
		sprintf(tmp,"%*s ",Y_axis_max,"");
		count=Y_axis_max+1;
		for(x=0,mem=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++mem)
		{
			if(rc)
			{
				if(x>=fc && x<=tc) doit=TRUE;
					else doit=FALSE;
			} else doit=TRUE;
			if(doit)
			{
			tmp=Rrealloc(tmp,count+X_axis_max+2);
			if(DFV->xaxis.axis_type!=5)
			{
				switch(DFV->xaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						sprintf(&tmp[count],"%-*s ",X_axis_max,(mem->data.string_value!=NULL ? mem->data.string_value:""));
						break;
					case BOOLNS:
						sprintf(&tmp[count],"%*s ",X_axis_max,(*mem->data.string_value==TRUE ? "Yes":"No"));
						break;
					case CHARACTERS:
						sprintf(&tmp[count],"%*s%c ",X_axis_max-1,"",*mem->data.string_value);
						break;
					case DOLLARS:
						if(X_axis_max>DFV->xaxis.len)
						{
							temp=famt(*mem->data.float_value,DFV->xaxis.len);
							sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
						} else {
							temp=famt(*mem->data.float_value,X_axis_max);
							sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case DOLLARS_NOCENTS:
						if(X_axis_max>DFV->xaxis.len)
						{
							temp=famtncents((*mem->data.float_value*100),DFV->xaxis.len,' ');
							sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
						} else {
							temp=famtncents((*mem->data.float_value*100),X_axis_max,' ');
							sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
						if(X_axis_max>DFV->xaxis.len)
						{
							temp=ufloatamt(*mem->data.float_value,DFV->xaxis.len);
							sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
						} else {
							temp=ufloatamt(*mem->data.float_value,X_axis_max);
							sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case DOUBLEV:
						if(X_axis_max>DFV->xaxis.len)
						{
							sprintf(&tmp[count],"%*s%*.0f ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,*mem->data.float_value);
						} else {
							sprintf(&tmp[count],"%*.0f ",X_axis_max,*mem->data.float_value);
						}
						break;
					case SDOUBLEV:
					case SDECIMALV:
						if(X_axis_max>DFV->xaxis.len)
						{
							temp=floatamt(*mem->data.float_value,DFV->xaxis.len-1);
							sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
						} else {
							temp=floatamt(*mem->data.float_value,X_axis_max-1);
							sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
						if(X_axis_max>DFV->xaxis.len)
						{
							sprintf(&tmp[count],"%*s%*d ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,*mem->data.short_value);
						} else {
							sprintf(&tmp[count],"%*d ",X_axis_max,*mem->data.short_value);
						}
						break;
					case SSHORTV:
						if(X_axis_max>DFV->xaxis.len)
						{
							temp=sintamt(*mem->data.short_value,DFV->xaxis.len-1);
							sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
						} else {
							temp=sintamt(*mem->data.short_value,X_axis_max-1);
							sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case LONGV:
						if(X_axis_max>DFV->xaxis.len)
						{
							sprintf(&tmp[count],"%*s%*d ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,*mem->data.integer_value);
						} else {
							sprintf(&tmp[count],"%*d ",X_axis_max,*mem->data.integer_value);
						}
						break;
					case SLONGV:
						if(X_axis_max>DFV->xaxis.len)
						{
							temp=sintamt(*mem->data.integer_value,DFV->xaxis.len-1);
							sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
						} else {
							temp=sintamt(*mem->data.integer_value,X_axis_max-1);
							sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for X-Axis..",DFV->xaxis.type);
						break;
				}
			} else {
				sprintf(&tmp[count],"%-*s ",X_axis_max,
					(mem->title2!=NULL ? mem->title2:""));
			}
			count+=(X_axis_max+1);
			}
		}
	}
	if(count>line_length && line_length>0) tmp[line_length-1]=0;
	return(tmp);
}
static void print_regular_matrix(RDA_PFILE *fp,char rr,int fr,int tr,char rc,int fc,
	int tc,int xline_length,int max_lines)
{
	int line_length=xline_length;
	short cell_current=0,Y_axis_max=0,X_axis_max=0;
	short cell_vertical=0;
	int x,y,z,w,count=0;
	int len=0,pages=0,lines=0;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXcell *c;
	MATRIXvalue *v;
	AXISmem *am;
	char doit=FALSE;
	char *header=NULL,*sub_header=NULL;
	char *tmp=NULL,*temp=NULL,*title1=NULL;

	if(!isEMPTY(DFV->yaxis.title1) || !isEMPTY(DFV->xaxis.title1))
	{
		title1=Rmalloc(RDAstrlen(DFV->yaxis.title1)+RDAstrlen(DFV->xaxis.title1)+7);
		sprintf(title1,"%s .VS. %s",
			(DFV->yaxis.title1!=NULL ? DFV->yaxis.title1:""),
			(DFV->xaxis.title1!=NULL ? DFV->xaxis.title1:""));
	} else title1=NULL;
	if(DFV->defs!=NULL)
	{
		for(x=0,c=DFV->defs;x<DFV->numdefs;++x,++c)
		{
			if(c->displayed) ++cell_vertical;
			if(c->len>X_axis_max) X_axis_max=c->len;
		}
	}
/* Loop to Determine Max Size of the X-Axis Labels and Titles */
	for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
	{
		doit=FALSE;
		if(rc)
		{
			if(x>=fc && x<=tc) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;
		if(doit)
		{
			if(DFV->xaxis.print_title1)
			{
				if(X_axis_max<RDAstrlen(am->title1))
					X_axis_max=RDAstrlen(am->title1);
			}
			if(DFV->xaxis.print_title2)
			{
				if(DFV->xaxis.axis_type!=5)
				{
					len=DFV->xaxis.len;
					if(len==0)
					{
					switch(DFV->xaxis.type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							len=RDAstrlen(am->data.string_value);
							if(len==0) len=20;
							break;
						default:
							len=20;
							break;
					}
					}
					if(X_axis_max<len)
						X_axis_max=len;
				} else {
					if(X_axis_max<RDAstrlen(am->title2))
						X_axis_max=RDAstrlen(am->title2);
				}
			}
		}
	}
/* Loop to Determine Max Size of the Y-Axis Labels and Titles */
	for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
	{
		doit=FALSE;
		if(rr)
		{
			if(x>=fr && x<=tr) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;
		if(doit)
		{
			if(DFV->yaxis.print_title1)
			{
				if(Y_axis_max<RDAstrlen(am->title1))
					Y_axis_max=RDAstrlen(am->title1);
			}
			if(DFV->yaxis.print_title2)
			{
				if(DFV->yaxis.axis_type!=5)
				{
					len=DFV->yaxis.len;
					if(len==0)
					{
					switch(DFV->yaxis.type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							len=RDAstrlen(am->data.string_value);
							if(len==0) len=20;
							break;
						default:
							len=20;
							break;
					}
					}
					if(Y_axis_max<len)
						Y_axis_max=len;
				} else {
					if(Y_axis_max<RDAstrlen(am->title2))
						Y_axis_max=RDAstrlen(am->title2);
				}
			}
		}
	}
	if(line_length==0)
	{
		if(rc)
		{
			line_length=(Y_axis_max+1)+((X_axis_max+1)*(tc-fc+1));
		} else {
			line_length=(Y_axis_max+1)+((X_axis_max+1)*DFV->xaxis.num);
		}
	}
/* Determine the number of values to account for in vertical cells */
	if(DFV->yaxis.print_title1 && DFV->yaxis.print_title2 &&
		cell_vertical<2)
	{
		cell_vertical=2;
	} else if(cell_vertical<1) cell_vertical=1;
	header=MakeMatrixHeader(rc,fc,tc,line_length,Y_axis_max,X_axis_max);
	sub_header=MakeMatrixSubHeader(rc,fc,tc,line_length,Y_axis_max,X_axis_max);
/* Print Matrix */
	for(x=0,am=DFV->yaxis.mem,rec=DFV->recs;x<DFV->num;++x,++am,++rec)
	{
		doit=FALSE;
		if(rr)
		{
			if(x>=fr && x<=tr) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;
		if(doit)
		{
			cell_current=0;
			for(z=0;z<cell_vertical;++z)
			{
				print_matrix_header(fp,&lines,&pages,DFV->title1,title1,line_length,
					max_lines,DFV->module,DFV->name,header,sub_header);
				count=0;
				tmp=Rmalloc(Y_axis_max+2);
				if(z==0 && DFV->yaxis.print_title1)
				{
					sprintf(tmp,"%-*s ",Y_axis_max,
						(am->title1!=NULL ? am->title1:""));
				} else if((z==0 && DFV->yaxis.print_title2) ||
					(z==1 && DFV->yaxis.print_title2
					&& DFV->yaxis.print_title1))
				{
					if(DFV->yaxis.axis_type!=5)
					{
						switch(DFV->yaxis.type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case PLAINTEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								sprintf(&tmp[count],"%-*s ",Y_axis_max,(am->data.string_value!=NULL ? am->data.string_value:""));
								break;
							case BOOLNS:
								sprintf(&tmp[count],"%*s ",Y_axis_max,(*am->data.string_value==TRUE ? "Yes":"No"));
								break;
							case CHARACTERS:
								sprintf(&tmp[count],"%*s%c ",Y_axis_max-1,"",*am->data.string_value);
								break;
							case DOLLARS:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=famt(*am->data.float_value,DFV->yaxis.len);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=famt(*am->data.float_value,Y_axis_max);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DOLLARS_NOCENTS:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=famtncents((*am->data.float_value*100),DFV->yaxis.len,' ');
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=famtncents((*am->data.float_value*100),Y_axis_max,' ');
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DECIMALV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=ufloatamt(*am->data.float_value,DFV->yaxis.len);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=ufloatamt(*am->data.float_value,Y_axis_max);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DOUBLEV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									sprintf(&tmp[count],"%*s%*.0f ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*am->data.float_value);
								} else {
									sprintf(&tmp[count],"%*.0f ",Y_axis_max,*am->data.float_value);
								}
								break;
							case SDOUBLEV:
							case SDECIMALV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=floatamt(*am->data.float_value,DFV->yaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=floatamt(*am->data.float_value,Y_axis_max-1);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case SHORTV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									sprintf(&tmp[count],"%*s%*d ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*am->data.short_value);
								} else {
									sprintf(&tmp[count],"%*d ",Y_axis_max,*am->data.short_value);
								}
								break;
							case SSHORTV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=sintamt(*am->data.short_value,DFV->yaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=sintamt(*am->data.short_value,Y_axis_max-1);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case LONGV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									sprintf(&tmp[count],"%*s%*d ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*am->data.integer_value);
								} else {
									sprintf(&tmp[count],"%*d ",Y_axis_max,*am->data.integer_value);
								}
								break;
							case SLONGV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=sintamt(*am->data.integer_value,DFV->yaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=sintamt(*am->data.integer_value,Y_axis_max-1);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case SCROLLEDLIST:
							case BUTTONS:
								prterr("Error call RaddogTiddog cause this is weird....");
								break;
							default:
								prterr("Error field type [%d] invalid for Y-Axis..",DFV->yaxis.type);
								break;
						}
					} else {
						sprintf(tmp,"%-*s ",Y_axis_max,
							(am->title2!=NULL ? am->title2:""));
					}
				} else {
					sprintf(tmp,"%*s ",Y_axis_max,"");
				}
				count=Y_axis_max+1;
				if(DFV->defs!=NULL && cell_current!=(-1))
				{
					if(z==0) cell_current=(-1);
					for(w=cell_current+1;w<DFV->numdefs;++w)
					{
						c=DFV->defs+w;
						if(c->displayed) break;
					}
					if(w<DFV->numdefs)
					{
						cell_current=w;
					} else cell_current=(-1);
				}
				if(rec->mems!=NULL)
				{
					for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
					{
						if(rc)
						{
							if(y>=fc && y<=tc) doit=TRUE;
							else doit=FALSE;

						} else doit=TRUE;
						if(doit)
						{
							if(mem->values!=NULL)
							{
								tmp=Rrealloc(tmp,count+X_axis_max+2);
								if(cell_current!=(-1))
								{
									v=mem->values+cell_current;
									c=DFV->defs+cell_current;
									switch(c->type)
									{
										case SCROLLEDTEXT:
										case VARIABLETEXT:
										case PLAINTEXT:
										case DATES:
										case TIMES:
										case SOCSECNUM:
										case PHONE:
										case CUSTOMTYPE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:
										case ZIPCODE:
											sprintf(&tmp[count],"%-*s ",X_axis_max,(v->data.string_value!=NULL ? v->data.string_value:""));
											break;
										case BOOLNS:
											sprintf(&tmp[count],"%*s ",X_axis_max,(*v->data.string_value==TRUE ? "Yes":"No"));
											break;
										case CHARACTERS:
											sprintf(&tmp[count],"%*s%c ",X_axis_max-1,"",*v->data.string_value);
											break;
										case DOLLARS:
											if(X_axis_max>c->len)
											{
												temp=famt(*v->data.float_value,c->len);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=famt(*v->data.float_value,X_axis_max);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case DOLLARS_NOCENTS:
											if(X_axis_max>c->len)
											{
												temp=famtncents((*v->data.float_value*100),c->len,' ');
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=famtncents((*v->data.float_value*100),X_axis_max,' ');
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case DECIMALV:
											if(X_axis_max>c->len)
											{
												temp=ufloatamt(*v->data.float_value,c->len);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=ufloatamt(*v->data.float_value,X_axis_max);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case DOUBLEV:
											if(X_axis_max>c->len)
											{
												sprintf(&tmp[count],"%*s%*.0f ",X_axis_max-c->len,"",c->len,*v->data.float_value);
											} else {
												sprintf(&tmp[count],"%*.0f ",X_axis_max,*v->data.float_value);
											}
											break;
										case SDOUBLEV:
										case SDECIMALV:
											if(X_axis_max>c->len)
											{
												temp=floatamt(*v->data.float_value,c->len-1);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=floatamt(*v->data.float_value,X_axis_max-1);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case SHORTV:
											if(X_axis_max>c->len)
											{
												sprintf(&tmp[count],"%*s%*d ",X_axis_max-c->len,"",c->len,*v->data.short_value);
											} else {
												sprintf(&tmp[count],"%*d ",X_axis_max,*v->data.short_value);
											}
											break;
										case SSHORTV:
											if(X_axis_max>c->len)
											{
												temp=sintamt(*v->data.short_value,c->len-1);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=sintamt(*v->data.short_value,X_axis_max-1);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case LONGV:
											if(X_axis_max>c->len)
											{
												sprintf(&tmp[count],"%*s%*d ",X_axis_max-c->len,"",c->len,*v->data.integer_value);
											} else {
												sprintf(&tmp[count],"%*d ",X_axis_max,*v->data.integer_value);
											}
											break;
										case SLONGV:
											if(X_axis_max>c->len)
											{
												temp=sintamt(*v->data.integer_value,c->len-1);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=sintamt(*v->data.integer_value,X_axis_max-1);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case SCROLLEDLIST:
										case BUTTONS:
											prterr("Error call RaddogTiddog cause this is weird....");
											break;
										default:
											prterr("Error field type [%d] invalid for Cell [%s]..",c->type,c->name);
											break;
									}
								} else {
									sprintf(&tmp[count],"%*s ",X_axis_max,"");
								}
							}
							count+=(X_axis_max+1);
						}
					}
				}
				if(count>line_length && line_length>0) tmp[line_length-1]=0;
				fixoutput(fp,tmp,&pages,&lines,line_length,NULL,0,2);
				if(tmp!=NULL) Rfree(tmp);
			}
		}
	}
	if(title1!=NULL) Rfree(title1);
	if(header!=NULL) Rfree(header);
	if(sub_header!=NULL) Rfree(sub_header);
}
static void print_linear_matrix(RDA_PFILE *fp,char rcell,int from_cell,int to_cell,
	int line_length,int max_lines)
{
	short cell_current=0,Y_axis_max=0,X_axis_max=0;
	short cell_vertical=0;
	int x,y,z,w,count=0;
	int len=0,pages=0,lines=0;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXcell *c;
	MATRIXvalue *v;
	AXISmem *am;
	char doit=FALSE;
	char *header=NULL,*sub_header=NULL;
	char *tmp=NULL,*temp=NULL,*title1=NULL;

	if(!isEMPTY(DFV->yaxis.title1) || !isEMPTY(DFV->xaxis.title1))
	{
		title1=Rmalloc(RDAstrlen(DFV->yaxis.title1)+RDAstrlen(DFV->xaxis.title1)+7);
		sprintf(title1,"%s .VS. %s",
			(DFV->yaxis.title1!=NULL ? DFV->yaxis.title1:""),
			(DFV->xaxis.title1!=NULL ? DFV->xaxis.title1:""));
	} else title1=NULL;
	if(DFV->defs!=NULL)
	{
		for(x=0,c=DFV->defs;x<DFV->numdefs;++x,++c)
		{
			if(c->displayed) ++cell_vertical;
			if(c->len>X_axis_max) X_axis_max=c->len;
		}
	}
/* Loop to Determine Max Size of the X-Axis Labels and Titles */
	for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
	{
		/*doit=FALSE;
		if(rc)
		{
			if(x>=fc && x<=tc) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;*/
		/* Just checking max size for all cells, even if only printing some... (JAB)  Maybe this should be changed? */
		doit=TRUE;
		if(doit)
		{
			if(DFV->xaxis.print_title1)
			{
				if(X_axis_max<RDAstrlen(am->title1))
					X_axis_max=RDAstrlen(am->title1);
			}
			if(DFV->xaxis.print_title2)
			{
				if(DFV->xaxis.axis_type!=5)
				{
					len=DFV->xaxis.len;
					if(len==0)
					{
					switch(DFV->xaxis.type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							len=RDAstrlen(am->data.string_value);
							if(len==0) len=20;
							break;
						default:
							len=20;
							break;
					}
					}
					if(X_axis_max<len)
						X_axis_max=len;
				} else {
					if(X_axis_max<RDAstrlen(am->title2))
						X_axis_max=RDAstrlen(am->title2);
				}
			}
		}
	}
/* Loop to Determine Max Size of the Y-Axis Labels and Titles */
	for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
	{
		/*doit=FALSE;
		if(rr)
		{
			if(x>=fr && x<=tr) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;*/
		/* Just checking max size for all cells, even if only printing some... (JAB)  Maybe this should be changed? */
		doit=TRUE;
		if(doit)
		{
			if(DFV->yaxis.print_title1)
			{
				if(Y_axis_max<RDAstrlen(am->title1))
					Y_axis_max=RDAstrlen(am->title1);
			}
			if(DFV->yaxis.print_title2)
			{
				if(DFV->yaxis.axis_type!=5)
				{
					len=DFV->yaxis.len;
					if(len==0)
					{
					switch(DFV->yaxis.type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							len=RDAstrlen(am->data.string_value);
							if(len==0) len=20;
							break;
						default:
							len=20;
							break;
					}
					}
					if(Y_axis_max<len)
						Y_axis_max=len;
				} else {
					if(Y_axis_max<RDAstrlen(am->title2))
						Y_axis_max=RDAstrlen(am->title2);
				}
			}
		}
	}
	if(line_length==0)
	{
		/* This is for when ranging on certain rows and columns and has been commented out (JAB). */
		/*if(rc)
		{
			line_length=(Y_axis_max+1)+((X_axis_max+1)*(tc-fc+1));
		} else {
			line_length=(Y_axis_max+1)+((X_axis_max+1)*DFV->xaxis.num);
		}*/
		line_length=(Y_axis_max+1)+((X_axis_max+1)*DFV->xaxis.num);
	}
/* Determine the number of values to account for in vertical cells */
	if(DFV->yaxis.print_title1 && DFV->yaxis.print_title2 &&
		cell_vertical<2)
	{
		cell_vertical=2;
	} else if(cell_vertical<1) cell_vertical=1;
	header=MakeMatrixHeader(FALSE,0,0,line_length,Y_axis_max,X_axis_max);
	sub_header=MakeMatrixSubHeader(FALSE,0,0,line_length,Y_axis_max,X_axis_max);
/* Print Matrix */
	for(x=0,am=DFV->yaxis.mem,rec=DFV->recs;x<DFV->num;++x,++am,++rec)
	{
		doit=FALSE;
		if(rcell)
		{
			if((((x+1)*rec->num))>=from_cell && ((x*rec->num)+1)<=to_cell) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;
		if(doit)
		{
			cell_current=0;
			for(z=0;z<cell_vertical;++z)
			{
				print_matrix_header(fp,&lines,&pages,DFV->title1,title1,line_length,
					max_lines,DFV->module,DFV->name,header,sub_header);
				count=0;
				tmp=Rmalloc(Y_axis_max+2);
				if(z==0 && DFV->yaxis.print_title1)
				{
					sprintf(tmp,"%-*s ",Y_axis_max,
						(am->title1!=NULL ? am->title1:""));
				} else if((z==0 && DFV->yaxis.print_title2) ||
					(z==1 && DFV->yaxis.print_title2
					&& DFV->yaxis.print_title1))
				{
					if(DFV->yaxis.axis_type!=5)
					{
						switch(DFV->yaxis.type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case PLAINTEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								sprintf(&tmp[count],"%-*s ",Y_axis_max,(am->data.string_value!=NULL ? am->data.string_value:""));
								break;
							case BOOLNS:
								sprintf(&tmp[count],"%*s ",Y_axis_max,(*am->data.string_value==TRUE ? "Yes":"No"));
								break;
							case CHARACTERS:
								sprintf(&tmp[count],"%*s%c ",Y_axis_max-1,"",*am->data.string_value);
								break;
							case DOLLARS:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=famt(*am->data.float_value,DFV->yaxis.len);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=famt(*am->data.float_value,Y_axis_max);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DOLLARS_NOCENTS:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=famtncents((*am->data.float_value*100),DFV->yaxis.len,' ');
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=famtncents((*am->data.float_value*100),Y_axis_max,' ');
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DECIMALV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=ufloatamt(*am->data.float_value,DFV->yaxis.len);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=ufloatamt(*am->data.float_value,Y_axis_max);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DOUBLEV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									sprintf(&tmp[count],"%*s%*.0f ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*am->data.float_value);
								} else {
									sprintf(&tmp[count],"%*.0f ",Y_axis_max,*am->data.float_value);
								}
								break;
							case SDOUBLEV:
							case SDECIMALV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=floatamt(*am->data.float_value,DFV->yaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=floatamt(*am->data.float_value,Y_axis_max-1);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case SHORTV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									sprintf(&tmp[count],"%*s%*d ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*am->data.short_value);
								} else {
									sprintf(&tmp[count],"%*d ",Y_axis_max,*am->data.short_value);
								}
								break;
							case SSHORTV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=sintamt(*am->data.short_value,DFV->yaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=sintamt(*am->data.short_value,Y_axis_max-1);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case LONGV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									sprintf(&tmp[count],"%*s%*d ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*am->data.integer_value);
								} else {
									sprintf(&tmp[count],"%*d ",Y_axis_max,*am->data.integer_value);
								}
								break;
							case SLONGV:
								if(Y_axis_max>DFV->yaxis.len)
								{
									temp=sintamt(*am->data.integer_value,DFV->yaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
								} else {
									temp=sintamt(*am->data.integer_value,Y_axis_max-1);
									sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case SCROLLEDLIST:
							case BUTTONS:
								prterr("Error call RaddogTiddog cause this is weird....");
								break;
							default:
								prterr("Error field type [%d] invalid for Y-Axis..",DFV->yaxis.type);
								break;
						}
					} else {
						sprintf(tmp,"%-*s ",Y_axis_max,
							(am->title2!=NULL ? am->title2:""));
					}
				} else {
					sprintf(tmp,"%*s ",Y_axis_max,"");
				}
				count=Y_axis_max+1;
				if(DFV->defs!=NULL && cell_current!=(-1))
				{
					if(z==0) cell_current=(-1);
					for(w=cell_current+1;w<DFV->numdefs;++w)
					{
						c=DFV->defs+w;
						if(c->displayed) break;
					}
					if(w<DFV->numdefs)
					{
						cell_current=w;
					} else cell_current=(-1);
				}
				if(rec->mems!=NULL)
				{
					for(y=0,mem=rec->mems;y<rec->num;++y,++mem)
					{
						doit=FALSE;
						if(rcell)
						{
							if(((x*rec->num)+(y+1))>=from_cell && ((x*rec->num)+(y+1))<=to_cell) doit=TRUE;
								else doit=FALSE;
						} else doit=TRUE;
						if(doit)
						{
							/* The following line was moved out of inside the if(mem->values!=NULL)
							 *  block, because if mem->values is NULL, wouldn't a buffer overrun
							 *  result because count is still incremented after the block? (JAB)
							 */
							tmp=Rrealloc(tmp,count+X_axis_max+2);
							if(mem->values!=NULL)
							{
								if(cell_current!=(-1))
								{
									v=mem->values+cell_current;
									c=DFV->defs+cell_current;
									switch(c->type)
									{
										case SCROLLEDTEXT:
										case VARIABLETEXT:
										case PLAINTEXT:
										case DATES:
										case TIMES:
										case SOCSECNUM:
										case PHONE:
										case CUSTOMTYPE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:
										case ZIPCODE:
											sprintf(&tmp[count],"%-*s ",X_axis_max,(v->data.string_value!=NULL ? v->data.string_value:""));
											break;
										case BOOLNS:
											sprintf(&tmp[count],"%*s ",X_axis_max,(*v->data.string_value==TRUE ? "Yes":"No"));
											break;
										case CHARACTERS:
											sprintf(&tmp[count],"%*s%c ",X_axis_max-1,"",*v->data.string_value);
											break;
										case DOLLARS:
											if(X_axis_max>c->len)
											{
												temp=famt(*v->data.float_value,c->len);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=famt(*v->data.float_value,X_axis_max);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case DOLLARS_NOCENTS:
											if(X_axis_max>c->len)
											{
												temp=famtncents((*v->data.float_value*100),c->len,' ');
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=famtncents((*v->data.float_value*100),X_axis_max,' ');
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case DECIMALV:
											if(X_axis_max>c->len)
											{
												temp=ufloatamt(*v->data.float_value,c->len);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=ufloatamt(*v->data.float_value,X_axis_max);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case DOUBLEV:
											if(X_axis_max>c->len)
											{
												sprintf(&tmp[count],"%*s%*.0f ",X_axis_max-c->len,"",c->len,*v->data.float_value);
											} else {
												sprintf(&tmp[count],"%*.0f ",X_axis_max,*v->data.float_value);
											}
											break;
										case SDOUBLEV:
										case SDECIMALV:
											if(X_axis_max>c->len)
											{
												temp=floatamt(*v->data.float_value,c->len-1);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=floatamt(*v->data.float_value,X_axis_max-1);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case SHORTV:
											if(X_axis_max>c->len)
											{
												sprintf(&tmp[count],"%*s%*d ",X_axis_max-c->len,"",c->len,*v->data.short_value);
											} else {
												sprintf(&tmp[count],"%*d ",X_axis_max,*v->data.short_value);
											}
											break;
										case SSHORTV:
											if(X_axis_max>c->len)
											{
												temp=sintamt(*v->data.short_value,c->len-1);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=sintamt(*v->data.short_value,X_axis_max-1);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case LONGV:
											if(X_axis_max>c->len)
											{
												sprintf(&tmp[count],"%*s%*d ",X_axis_max-c->len,"",c->len,*v->data.integer_value);
											} else {
												sprintf(&tmp[count],"%*d ",X_axis_max,*v->data.integer_value);
											}
											break;
										case SLONGV:
											if(X_axis_max>c->len)
											{
												temp=sintamt(*v->data.integer_value,c->len-1);
												sprintf(&tmp[count],"%*s%*s ",X_axis_max-c->len,"",c->len,temp);
											} else {
												temp=sintamt(*v->data.integer_value,X_axis_max-1);
												sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
											}
											if(temp!=NULL) Rfree(temp);
											break;
										case SCROLLEDLIST:
										case BUTTONS:
											prterr("Error call RaddogTiddog cause this is weird....");
											break;
										default:
											prterr("Error field type [%d] invalid for Cell [%s]..",c->type,c->name);
											break;
									}
								} else {
									sprintf(&tmp[count],"%*s ",X_axis_max,"");
								}
							}
							count+=(X_axis_max+1);
						}
						else /* if ranging on cells and this one isn't included, print blank space */
						{
							tmp=Rrealloc(tmp,count+X_axis_max+2);
							sprintf(&tmp[count],"%*s ",X_axis_max,"");
							count+=(X_axis_max+1);
						}
					}
				}
				if(count>line_length && line_length>0) tmp[line_length-1]=0;
				fixoutput(fp,tmp,&pages,&lines,line_length,NULL,0,2);
				if(tmp!=NULL) Rfree(tmp);
			}
		}
	}
	if(title1!=NULL) Rfree(title1);
	if(header!=NULL) Rfree(header);
	if(sub_header!=NULL) Rfree(sub_header);
}
static char *MakeReverseHeader(char rr,int fr,int tr,int line_length,
	short Y_axis_max,short X_axis_max)
{
	char *tmp=NULL;
	AXISmem *mem;
	int x,count=0;
	short doit=FALSE;

	if(DFV->yaxis.mem!=NULL && DFV->yaxis.print_title1)
	{
		tmp=Rmalloc(X_axis_max+2);
		sprintf(tmp,"%*s ",X_axis_max,"");
		count=X_axis_max+1;
		for(x=0,mem=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++mem)
		{
			if(rr)
			{
				if(x>=fr && x<=tr) doit=TRUE;
					else doit=FALSE;
			} else doit=TRUE;
			if(doit)
			{
				tmp=Rrealloc(tmp,count+Y_axis_max+2);
				sprintf(&tmp[count],"%-*s ",Y_axis_max,
					(mem->title1!=NULL ? mem->title1:""));
				count+=(Y_axis_max+1);
			}
		}
	}
	if(count>line_length && line_length>0) tmp[line_length-1]=0;
	return(tmp);
}
static char *MakeReverseSubHeader(char rr,int fr,int tr,int line_length,
	short Y_axis_max,short X_axis_max)
{
	char *tmp=NULL,*temp=NULL;
	AXISmem *mem;
	int x,count=0;
	short doit=FALSE;

	if(DFV->yaxis.mem!=NULL && DFV->yaxis.print_title2)
	{
		tmp=Rmalloc(X_axis_max+2);
		sprintf(tmp,"%*s ",X_axis_max,"");
		count=X_axis_max+1;
		for(x=0,mem=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++mem)
		{
			if(rr)
			{
				if(x>=fr && x<=tr) doit=TRUE;
					else doit=FALSE;
			} else doit=TRUE;
			if(doit)
			{
			tmp=Rrealloc(tmp,count+Y_axis_max+2);
			if(DFV->yaxis.axis_type!=5)
			{
				switch(DFV->yaxis.type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						sprintf(&tmp[count],"%-*s ",Y_axis_max,(mem->data.string_value!=NULL ? mem->data.string_value:""));
						break;
					case BOOLNS:
						sprintf(&tmp[count],"%*s ",Y_axis_max,(*mem->data.string_value==TRUE ? "Yes":"No"));
						break;
					case CHARACTERS:
						sprintf(&tmp[count],"%*s%c ",Y_axis_max-1,"",*mem->data.string_value);
						break;
					case DOLLARS:
						if(Y_axis_max>DFV->yaxis.len)
						{
							temp=famt(*mem->data.float_value,DFV->yaxis.len);
							sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
						} else {
							temp=famt(*mem->data.float_value,Y_axis_max);
							sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case DOLLARS_NOCENTS:
						if(Y_axis_max>DFV->yaxis.len)
						{
							temp=famtncents((*mem->data.float_value*100),DFV->yaxis.len,' ');
							sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
						} else {
							temp=famtncents((*mem->data.float_value*100),Y_axis_max,' ');
							sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
						if(Y_axis_max>DFV->yaxis.len)
						{
							temp=ufloatamt(*mem->data.float_value,DFV->yaxis.len);
							sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
						} else {
							temp=ufloatamt(*mem->data.float_value,Y_axis_max);
							sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case DOUBLEV:
						if(Y_axis_max>DFV->yaxis.len)
						{
							sprintf(&tmp[count],"%*s%*.0f ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*mem->data.float_value);
						} else {
							sprintf(&tmp[count],"%*.0f ",Y_axis_max,*mem->data.float_value);
						}
						break;
					case SDOUBLEV:
					case SDECIMALV:
						if(Y_axis_max>DFV->yaxis.len)
						{
							temp=floatamt(*mem->data.float_value,DFV->yaxis.len-1);
							sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
						} else {
							temp=floatamt(*mem->data.float_value,Y_axis_max-1);
							sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
						if(Y_axis_max>DFV->yaxis.len)
						{
							sprintf(&tmp[count],"%*s%*d ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*mem->data.short_value);
						} else {
							sprintf(&tmp[count],"%*d ",Y_axis_max,*mem->data.short_value);
						}
						break;
					case SSHORTV:
						if(Y_axis_max>DFV->yaxis.len)
						{
							temp=sintamt(*mem->data.short_value,DFV->yaxis.len-1);
							sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
						} else {
							temp=sintamt(*mem->data.short_value,Y_axis_max-1);
							sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case LONGV:
						if(Y_axis_max>DFV->yaxis.len)
						{
							sprintf(&tmp[count],"%*s%*d ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,*mem->data.integer_value);
						} else {
							sprintf(&tmp[count],"%*d ",Y_axis_max,*mem->data.integer_value);
						}
						break;
					case SLONGV:
						if(Y_axis_max>DFV->yaxis.len)
						{
							temp=sintamt(*mem->data.integer_value,DFV->yaxis.len-1);
							sprintf(&tmp[count],"%*s%*s ",Y_axis_max-DFV->yaxis.len,"",DFV->yaxis.len,temp);
						} else {
							temp=sintamt(*mem->data.integer_value,Y_axis_max-1);
							sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
						}
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%d] invalid for X-Axis..",DFV->yaxis.type);
						break;
				}
			} else {
				sprintf(&tmp[count],"%-*s ",Y_axis_max,
					(mem->title2!=NULL ? mem->title2:""));
			}
			count+=(Y_axis_max+1);
			}
		}
	}
	if(count>line_length && line_length>0) tmp[line_length-1]=0;
	return(tmp);
}
static void print_reverse_matrix(RDA_PFILE *fp,char rr,int fr,int tr,char rc,int fc,
	int tc,int xline_length,int max_lines)
{
	int line_length=xline_length;
	short cell_current=0,Y_axis_max=0,X_axis_max=0;
	short cell_vertical=0;
	int x,y,z,w,count=0;
	int len=0,pages=0,lines=0;
	MATRIXrec *rec;
	MATRIXmem *mem;
	MATRIXcell *c;
	MATRIXvalue *v;
	AXISmem *am;
	char doit=FALSE;
	char *header=NULL,*sub_header=NULL;
	char *tmp=NULL,*temp=NULL,*title1=NULL;

	if(!isEMPTY(DFV->yaxis.title1) || !isEMPTY(DFV->xaxis.title1))
	{
		title1=Rmalloc(RDAstrlen(DFV->yaxis.title1)+
			RDAstrlen(DFV->xaxis.title1)+7);
		sprintf(title1,"%s .VS. %s",
			(DFV->xaxis.title1!=NULL ? DFV->xaxis.title1:""),
			(DFV->yaxis.title1!=NULL ? DFV->yaxis.title1:""));
	} else title1=NULL;
	if(DFV->defs!=NULL)
	{
		for(x=0,c=DFV->defs;x<DFV->numdefs;++x,++c)
		{
			if(c->displayed) ++cell_vertical;
			if(c->len>Y_axis_max) Y_axis_max=c->len;
		}
	}
/* Loop to Determine Max Size of the X-Axis Labels and Titles */
	for(x=0,am=DFV->xaxis.mem;x<DFV->xaxis.num;++x,++am)
	{
		doit=FALSE;
		if(rc)
		{
			if(x>=fc && x<=tc) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;
		if(doit)
		{
			if(DFV->xaxis.print_title1)
			{
				if(X_axis_max<RDAstrlen(am->title1))
					X_axis_max=RDAstrlen(am->title1);
			}
			if(DFV->xaxis.print_title2)
			{
				if(DFV->xaxis.axis_type!=5)
				{
					len=DFV->xaxis.len;
					if(len==0)
					{
					switch(DFV->xaxis.type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							len=RDAstrlen(am->data.string_value);
							if(len==0) len=20;
							break;
						default:
							len=20;
							break;
					}
					}
					if(X_axis_max<len)
						X_axis_max=len;
				} else {
					if(X_axis_max<RDAstrlen(am->title2))
						X_axis_max=RDAstrlen(am->title2);
				}
			}
		}
	}
/* Loop to Determine Max Size of the Y-Axis Labels and Titles */
	for(x=0,am=DFV->yaxis.mem;x<DFV->yaxis.num;++x,++am)
	{
		doit=FALSE;
		if(rr)
		{
			if(x>=fr && x<=tr) doit=TRUE;
				else doit=FALSE;
		} else doit=TRUE;
		if(doit)
		{
			if(DFV->yaxis.print_title1)
			{
				if(Y_axis_max<RDAstrlen(am->title1))
					Y_axis_max=RDAstrlen(am->title1);
			}
			if(DFV->yaxis.print_title2)
			{
				if(DFV->yaxis.axis_type!=5)
				{
					len=DFV->yaxis.len;
					if(len==0)
					{
					switch(DFV->yaxis.type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							len=RDAstrlen(am->data.string_value);
							if(len==0) len=20;
							break;
						default:
							len=20;
							break;
					}
					}
					if(Y_axis_max<len)
						Y_axis_max=len;
				} else {
					if(Y_axis_max<RDAstrlen(am->title2))
						Y_axis_max=RDAstrlen(am->title2);
				}
			}
		}
	}
	if(line_length==0)
	{
		if(rr)
		{
			line_length=(X_axis_max+1)+((Y_axis_max+1)*(tr-fr+1));
		} else {
			line_length=(X_axis_max+1)+((Y_axis_max+1)*DFV->num);
		}
	}
	if(line_length<79) line_length=79;
/* Determine the number of values to account for in vertical cells */
	if(DFV->xaxis.print_title1 && DFV->xaxis.print_title2 &&
		cell_vertical<2)
	{
		cell_vertical=2;
	} else if(cell_vertical<1) cell_vertical=1;
	header=MakeReverseHeader(rr,fr,tr,line_length,Y_axis_max,X_axis_max);
	sub_header=MakeReverseSubHeader(rr,fr,tr,line_length,Y_axis_max,X_axis_max);
/* Print Matrix */
	for(y=0,am=DFV->xaxis.mem;y<DFV->xaxis.num;++y,++am)
	{
		if(rc)
		{
			if(y>=fc && y<=tc) doit=TRUE;
			else doit=FALSE;
		} else doit=TRUE;
		if(doit)
		{
			for(z=0;z<cell_vertical;++z)
			{
				count=0;
				cell_current=0;
				print_matrix_header(fp,&lines,&pages,DFV->title1,title1,line_length,
					max_lines,DFV->module,DFV->name,header,sub_header);
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+X_axis_max+2);
				} else {
					tmp=Rmalloc(X_axis_max+2);
					count=0;
				}
				if(z==0 && DFV->xaxis.print_title1)
				{
					sprintf(&tmp[count],"%-*s ",X_axis_max,
						(am->title1!=NULL ? am->title1:""));
				} else if((z==0 && DFV->xaxis.print_title2) ||
					(z==1 && DFV->xaxis.print_title2
					&& DFV->xaxis.print_title1))
				{
					if(DFV->xaxis.axis_type!=5)
					{
						switch(DFV->xaxis.type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case PLAINTEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								sprintf(&tmp[count],"%-*s ",X_axis_max,(am->data.string_value!=NULL ? am->data.string_value:""));
								break;
							case BOOLNS:
								sprintf(&tmp[count],"%*s ",X_axis_max,(*am->data.string_value==TRUE ? "Yes":"No"));
								break;
							case CHARACTERS:
								sprintf(&tmp[count],"%*s%c ",X_axis_max-1,"",*am->data.string_value);
								break;
							case DOLLARS:
								if(X_axis_max>DFV->xaxis.len)
								{
									temp=famt(*am->data.float_value,DFV->xaxis.len);
									sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
								} else {
									temp=famt(*am->data.float_value,X_axis_max);
									sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DOLLARS_NOCENTS:
								if(X_axis_max>DFV->xaxis.len)
								{
									temp=famtncents((*am->data.float_value*100),DFV->xaxis.len,' ');
									sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
								} else {
									temp=famtncents((*am->data.float_value*100),X_axis_max,' ');
									sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DECIMALV:
								if(X_axis_max>DFV->xaxis.len)
								{
									temp=ufloatamt(*am->data.float_value,DFV->xaxis.len);
									sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
								} else {
									temp=ufloatamt(*am->data.float_value,X_axis_max);
									sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case DOUBLEV:
								if(X_axis_max>DFV->xaxis.len)
								{
									sprintf(&tmp[count],"%*s%*.0f ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,*am->data.float_value);
								} else {
									sprintf(&tmp[count],"%*.0f ",X_axis_max,*am->data.float_value);
								}
								break;
							case SDOUBLEV:
							case SDECIMALV:
								if(X_axis_max>DFV->xaxis.len)
								{
									temp=floatamt(*am->data.float_value,DFV->xaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
								} else {
									temp=floatamt(*am->data.float_value,X_axis_max-1);
									sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case SHORTV:
								if(X_axis_max>DFV->xaxis.len)
								{
									sprintf(&tmp[count],"%*s%*d ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,*am->data.short_value);
								} else {
									sprintf(&tmp[count],"%*d ",X_axis_max,*am->data.short_value);
								}
								break;
							case SSHORTV:
								if(X_axis_max>DFV->xaxis.len)
								{
									temp=sintamt(*am->data.short_value,DFV->xaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
								} else {
									temp=sintamt(*am->data.short_value,X_axis_max-1);
									sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case LONGV:
								if(X_axis_max>DFV->xaxis.len)
								{
									sprintf(&tmp[count],"%*s%*d ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,*am->data.integer_value);
								} else {
									sprintf(&tmp[count],"%*d ",X_axis_max,*am->data.integer_value);
								}
								break;
							case SLONGV:
								if(X_axis_max>DFV->xaxis.len)
								{
									temp=sintamt(*am->data.integer_value,DFV->xaxis.len-1);
									sprintf(&tmp[count],"%*s%*s ",X_axis_max-DFV->xaxis.len,"",DFV->xaxis.len,temp);
								} else {
									temp=sintamt(*am->data.integer_value,X_axis_max-1);
									sprintf(&tmp[count],"%*s ",X_axis_max,(temp!=NULL ? temp:""));
								}
								if(temp!=NULL) Rfree(temp);
								break;
							case SCROLLEDLIST:
							case BUTTONS:
								prterr("Error call RaddogTiddog cause this is weird....");
								break;
							default:
								prterr("Error field type [%d] invalid for X-Axis..",DFV->xaxis.type);
								break;
						}
					} else {
						sprintf(&tmp[count],"%-*s ",X_axis_max,
							(am->title2!=NULL ? am->title2:""));
					}
					count+=(X_axis_max+1);
					for(x=0;x<DFV->yaxis.num;++x)
					{
						doit=FALSE;
						if(rr)
						{
							if(x>=fr && x<=tr) doit=TRUE;
								else doit=FALSE;
						} else doit=TRUE;
						if(doit)
						{
							if(DFV->defs!=NULL && cell_current!=(-1))
							{
								if(z==0) cell_current=(-1);
								for(w=cell_current+1;w<DFV->numdefs;++w)
								{
									c=DFV->defs+w;
									if(c->displayed) break;
								}
								if(w<DFV->numdefs)
								{
									cell_current=w;
								} else cell_current=(-1);
							}
							rec=DFV->recs+x;
							if(rec->mems!=NULL)
							{
								mem=rec->mems+y;
								if(mem->values!=NULL)
								{
									tmp=Rrealloc(tmp,count+Y_axis_max+2);
									if(cell_current!=(-1))
									{
										v=mem->values+cell_current;
										c=DFV->defs+cell_current;
										switch(c->type)
										{
											case SCROLLEDTEXT:
											case VARIABLETEXT:
											case PLAINTEXT:
											case DATES:
											case TIMES:
											case SOCSECNUM:
											case PHONE:
											case CUSTOMTYPE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case OPTIONALFIELDS:
											case ZIPCODE:
												sprintf(&tmp[count],"%-*s ",Y_axis_max,(v->data.string_value!=NULL ? v->data.string_value:""));
												break;
											case BOOLNS:
												sprintf(&tmp[count],"%*s ",Y_axis_max,(*v->data.string_value==TRUE ? "Yes":"No"));
												break;
											case CHARACTERS:
												sprintf(&tmp[count],"%*s%c ",Y_axis_max-1,"",*v->data.string_value);
												break;
											case DOLLARS:
												if(Y_axis_max>c->len)
												{
													temp=famt(*v->data.float_value,c->len);
													sprintf(&tmp[count],"%*s%*s ",Y_axis_max-c->len,"",c->len,temp);
												} else {
													temp=famt(*v->data.float_value,Y_axis_max);
													sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
												}
												if(temp!=NULL) Rfree(temp);
												break;
											case DOLLARS_NOCENTS:
												if(Y_axis_max>c->len)
												{
													temp=famtncents((*v->data.float_value*100),c->len,' ');
													sprintf(&tmp[count],"%*s%*s ",Y_axis_max-c->len,"",c->len,temp);
												} else {
													temp=famtncents((*v->data.float_value*100),Y_axis_max,' ');
													sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
												}
												if(temp!=NULL) Rfree(temp);
												break;
											case DECIMALV:
												if(Y_axis_max>c->len)
												{
													temp=ufloatamt(*v->data.float_value,c->len);
													sprintf(&tmp[count],"%*s%*s ",Y_axis_max-c->len,"",c->len,temp);
												} else {
													temp=ufloatamt(*v->data.float_value,Y_axis_max);
													sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
												}
												if(temp!=NULL) Rfree(temp);
												break;
											case DOUBLEV:
												if(Y_axis_max>c->len)
												{
													sprintf(&tmp[count],"%*s%*.0f ",Y_axis_max-c->len,"",c->len,*v->data.float_value);
												} else {
													sprintf(&tmp[count],"%*.0f ",Y_axis_max,*v->data.float_value);
												}
												break;
											case SDOUBLEV:
											case SDECIMALV:
												if(Y_axis_max>c->len)
												{
													temp=floatamt(*v->data.float_value,c->len-1);
													sprintf(&tmp[count],"%*s%*s ",Y_axis_max-c->len,"",c->len,temp);
												} else {
													temp=floatamt(*v->data.float_value,Y_axis_max-1);
													sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
												}
												if(temp!=NULL) Rfree(temp);
												break;
											case SHORTV:
												if(Y_axis_max>c->len)
												{
													sprintf(&tmp[count],"%*s%*d ",Y_axis_max-c->len,"",c->len,*v->data.short_value);
												} else {
													sprintf(&tmp[count],"%*d ",Y_axis_max,*v->data.short_value);
												}
												break;
											case SSHORTV:
												if(Y_axis_max>c->len)
												{
													temp=sintamt(*v->data.short_value,c->len-1);
													sprintf(&tmp[count],"%*s%*s ",Y_axis_max-c->len,"",c->len,temp);
												} else {
													temp=sintamt(*v->data.short_value,Y_axis_max-1);
													sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
												}
												if(temp!=NULL) Rfree(temp);
												break;
											case LONGV:
												if(Y_axis_max>c->len)
												{
													sprintf(&tmp[count],"%*s%*d ",Y_axis_max-c->len,"",c->len,*v->data.integer_value);
												} else {
													sprintf(&tmp[count],"%*d ",Y_axis_max,*v->data.integer_value);
												}
												break;
											case SLONGV:
												if(Y_axis_max>c->len)
												{
													temp=sintamt(*v->data.integer_value,c->len-1);
													sprintf(&tmp[count],"%*s%*s ",Y_axis_max-c->len,"",c->len,temp);
												} else {
													temp=sintamt(*v->data.integer_value,Y_axis_max-1);
													sprintf(&tmp[count],"%*s ",Y_axis_max,(temp!=NULL ? temp:""));
												}
												if(temp!=NULL) Rfree(temp);
												break;
											case SCROLLEDLIST:
											case BUTTONS:
												prterr("Error call RaddogTiddog cause this is weird....");
												break;
											default:
												prterr("Error field type [%d] invalid for Cell [%s]..",c->type,c->name);
												break;
										}
									} else {
										sprintf(&tmp[count],"%*s ",Y_axis_max,"");
									}
								}
								count+=(Y_axis_max+1);
							}
						}
					}
				}
				if(count>line_length && line_length>0) tmp[line_length-1]=0;
				fixoutput(fp,tmp,&pages,&lines,line_length,NULL,0,2);
				if(tmp!=NULL) Rfree(tmp);
			}
		} else {
			sprintf(&tmp[count],"%*s ",X_axis_max,"");
		}
	}
	if(title1!=NULL) Rfree(title1);
	if(header!=NULL) Rfree(header);
	if(sub_header!=NULL) Rfree(sub_header);
}
