#include<guip.hpp>
#include<mix.hpp>

extern int RDAapi;
extern char *prevlink,*url;
int line_count=0,box_count=0;
#define HTMLcrtline(a,b,c)	xHTMLcrtline(a,b,c,__LINE__,__FILE__) 
static void xHTMLcrtline(FILE *,RDAscrn *,int *,int,char *);
#define HTMLcrtbox(a,b,c)	xHTMLcrtbox(a,b,c,__LINE__,__FILE__)
static void xHTMLcrtbox(FILE *,RDAscrn *,int *,int,char *);
#define HTMLnumperline(a,b)	xHTMLnumperline(a,b,__LINE__,__FILE__)
static int xHTMLnumperline(RDAscrn *,int,int,char *);
#define HTMLfindmaxlines(a)	xHTMLfindmaxlines(a,__LINE__,__FILE__)
static int xHTMLfindmaxlines(RDAscrn *,int,char *);
#define HTMLboxmaxcols(a,b)	xHTMLboxmaxcols(a,b,__LINE__,__FILE__)
static int xHTMLboxmaxcols(RDAscrn *,int,int,char *);
#define HTMLboxmaxrows(a,b)	xHTMLboxmaxrows(a,b,__LINE__,__FILE__)
static int xHTMLboxmaxrows(RDAscrn *,int,int,char *);

static int xHTMLnumperline(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=0,boxcount=0;
	RDAwdgt *wdgt;
	char box=FALSE;

	if(diagapps)
	{
		prterr("DIAG HTMLnumperline Find Number Widgets per Line Starting at [%d] in screen [%s] [%s] at line [%d] program [%s].",curnum,scn->module,scn->name,line,file);  
	}
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		if(wdgt->resource_num>(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* Frame */
				if(!box) ++count;
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle buttons */
			case 11:/* Seperator */
			case 14:/* optional screen */
			case 5: /* labels */
				if(!box) ++count;
				break;
			case 1: /* new line */
			case 2: /* end line */
				if(!box)
				{
					if(diagapps)
					{
						prterr("DIAG HTMLnumperline Starting at [%d] on Screen [%s] [%s] Returning [%d] at line [%d] program [%s].",curnum,scn->module,scn->name,count,line,file);
					}
					return count;
				}
				break;
			case 12: /* scrolled window */
			case 3: /* new box */
				if(!box)
				{
					box=TRUE;
					++count;
				}
				++boxcount;
				break;
			case 13: /* end scrolled window */
			case 4: /* end box */
				--boxcount;
				if(boxcount==0) box=FALSE;
				break;
			default:
				prterr("Error Unrecognized Widget Type.");
				break;
		}
		}
	}
	if(diagapps)
	{
		prterr("DIAG HTMLnumperline Starting at [%d] on Screen [%s] [%s] Returning [%d] at line [%d] program [%s].",curnum,scn->module,scn->name,count,line,file);
	}
	return count;
}
static int xHTMLfindmaxlines(RDAscrn *scn,int line,char *file)
{
	int x,boxcount=0,count=0,line_height=1;
	RDAwdgt *wdgt;
	char box=FALSE;

	if(diagapps)
	{
		prterr("DIAG Find Maximum Lines on Screen [%s] [%s] at line [%d] program [%s].",scn->module,scn->name,line,file);
	}
	for(x=0,wdgt=scn->wdgts;x<scn->numwdgts;++x,++wdgt)
	{
		if(wdgt->resource_num>(-1))
		{
		switch(wdgt->type)
		{
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* togglebutton */
			case 5: /* label */
			case 14: /* optional screen */
			case 11:
				break;
			case 1: /* new line */
				if(!box)
				{
					if(x>0) count+=line_height;
					line_height=1;
				}
				break;
			case 10:
			case 2: /* end line */
				break;
			case 12:
			case 3: /* new box */
				++boxcount;
				box=TRUE;
				break;
			case 13:
			case 4: /* end box */
				--boxcount;
				if(!boxcount) box=FALSE;
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s].",wdgt->type,x,scn->module,scn->name);
				break;
		}
		}
	}
	if(diagapps)
	{
		prterr("DIAG HTMLfindmaxlines for Screen [%s] [%s] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
	return count;
}
static int xHTMLboxmaxcols(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=0,hold=0,boxcount=1;
	RDAwdgt *wdgt;

	if(diagapps)
	{
		prterr("DIAG HTMLboxmaxcols Find Maximum Number Columns in Box/Scrolled Wiundow for Screen [%s] [%s] Starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);
	}
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		if(wdgt->resource_num>(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* frame */
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle button */
			case 11: /* seperator */
			case 14: /* optional screens */
			case 5: /* label */
				if(boxcount==1) ++count;
				break;
			case 1: /* new line */
			case 2: /* end line */
				if(boxcount==1)
				{
					if(hold<count) hold=count;
					count=0;
				}
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
				if(boxcount==1) ++count;
				++boxcount;
				break;
			case 4: /* end box */
			case 13: /* end scrolled window */
				--boxcount;
				if(boxcount==0)
				{
					if(hold<count) hold=count;
					if(diagapps)
					{
						prterr("DIAG HTMLboxmaxcols For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,curnum,hold,line,file);
					} 
					return(hold);
				}
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,x,line,file);
				break;
		}
		}
	}
	if(hold<count) hold=count;
	if(diagapps)
	{
		prterr("DIAG HTMLboxmaxcols For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,curnum,hold,line,file);
	} 
	return(hold);
}
static int xHTMLboxmaxrows(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=0,wcount=0,boxcount=1;
	RDAwdgt *wdgt;
	
	if(diagapps) 
	{ 
		prterr("DIAG HTMLboxmaxrows Find Maximum Number of Rows in Box on Screen [%s] [%s] Starting with Widget [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file); 
	}
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		if(wdgt->resource_num>(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* frame */
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginnging balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle button */
			case 14: /* optional screens */
			case 11: /* seperator */
			case 5: /* label */
				if(boxcount==1) ++wcount;
				break;
			case 1: /* new line */
				if(boxcount==1) wcount=0;
				break;
			case 2: /* end line */
				if(boxcount==1) if(wcount>0) ++count;
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
				if(boxcount==1) ++wcount;
				++boxcount;
				break;
			case 4: /* end box */
			case 13: /* end scrolled window */
				--boxcount;
				if(boxcount==0)
				{
					if(diagapps)
					{
						prterr("DIAG HTMLboxmaxrows For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,curnum,count,line,file);
					}
					if(count==0) ++count;
					return(count);
				}
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%d] [%d] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}
		}		
	}
	if(diagapps)
	{
		prterr("DIAG HTMLboxmaxrows For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
	return(count);
}

/*---------------------------------------------------------------------------
	HTMLcrtline - function to create lines inside a window
---------------------------------------------------------------------------*/
static void xHTMLcrtline(FILE *fp,RDAscrn *scn,
	int *widgetcount,int linex,char *file)
{
	RDAwdgt *wdgt=NULL;
	int num=0,cur_line=0;
	char *dashes=NULL;

	if(diagapps)
	{
		prterr("DIAG HTMLcrtline Creating Custom Line for Screen [%s] [%s] Starting with Widget [%d] at line [%d] program [%s].",scn->module,scn->name,*widgetcount,linex,file);
	}
	num=HTMLnumperline(scn,*widgetcount);
	if(num<1)
	{
		return;
	}
	++line_count;
	cur_line=line_count;
	fprintf(fp,"<LINE %2d>",cur_line);
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+*widgetcount;
		if(wdgt->resource_num>(-1))
		{
		if(diagapps)
		{
			prterr("DIAG Creating Widget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
				*widgetcount,wdgttypes[wdgt->type],(wdgt->resource_name!=NULL ? 
				wdgt->resource_name:""),(wdgt->label!=NULL ? wdgt->label:""),
				wdgt->rows,wdgt->cols,wdgt->rtype); 
		}
		dashes=adddashes(wdgt->resource_name);
		switch(wdgt->type)
		{
			case 8: /* scrolled text */
				fprintf(fp,"<TEXTAREA NAME=\"%s\" ROWS=%d COLS=%d></TEXTAREA>",(dashes!=NULL ? dashes:""),wdgt->rows,wdgt->cols);
				break;
			case 7: /* scrolled list */
				break;
			case 15:/* custom input fields */
			case 16:/* expenditure fields */
			case 17:/* revenue fields */
			case 18:/* balance sheet fields */
			case 19:/* beginning balance fields */
			case 0: /* standard resource fields */
				fprintf(fp,"<INPUT TYPE=\"text\" NAME=\"%s\" SIZE=\"%d\">",(dashes!=NULL ? dashes:""),wdgt->cols);
				break;
			case 9: /* toggle buttons */
				fprintf(fp,"<INPUT TYPE=\"CheckBox\" NAME=\"%s\" VALUE=\"%s\">%s",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""),(wdgt->label!=NULL ? wdgt->label:""));
				break;
			case 6: /* buttons */
			case 14: /* optional screen button */
				switch(RDAapi)
				{
					default:
					case 0: /* doadd buttons */
					case 1: /* doreport buttons */
						if(!RDAstrcmp(wdgt->resource_name,"QUIT"))
						{
							fprintf(fp,"<A HREF=\"%s/%s.HTML\">%s</A HREF>",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
						} else {
							fprintf(fp,"<INPUT TYPE=\"Submit\" VALUE=\"%s\">",(wdgt->label!=NULL ? wdgt->label:""));
						}
						break;
					case 2: /* menu buttons */
						fprintf(fp,"<A HREF=\"%s.HTML\">%s</A HREF>",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""));
						break;
				}
				break;
			case 1: /* start newline */
				if(num)
				{
					HTMLcrtline(fp,scn,widgetcount);
				}
				break;
			case 2: /* endline */
				fprintf(fp,"</LINE %2d><BR>\n",cur_line);
				return;
			case 3: /* start box */
				if(num)
				{
					HTMLcrtbox(fp,scn,widgetcount);
				}
				break;
			case 4: /* end box */
				if(num)
				{
					prterr("Error Widget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],scn->module,scn->name);
				}
				break;
			case 5: /* LABELS */
				if(num)
				{
					fprintf(fp,"%s",(wdgt->label!=NULL ? wdgt->label:""));
				}
				break;
			case 10: /* FRAME */
				if(num)
				{
				}
				break;
			case 11: /* Separator */
				if(num)
				{
					fprintf(fp,"<HR>\n");
				}
				break;
			case 12: /* New ScrolledWindow */
				if(num)
				{
					HTMLcrtbox(fp,scn,widgetcount);
				}
				break;
			case 13: /* End ScrolledWindow */
				if(num)
				{
					prterr("Error Widget [%d] Type [%d] [%s] on screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],scn->module,scn->name);
				}
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
				break;
		}
		if(dashes!=NULL) Rfree(dashes);
		}
	}
}
static void xHTMLcrtbox(FILE *fp,RDAscrn *scn,int *widgetcount,int line,char *file)
{
	RDAwdgt *wdgt=NULL;
	int fractbase=0,boxmaxr=0,boxmaxc=0,cur_box=0,boxline=0;
	char *dashes=NULL;

	if(diagapps)
	{
		prterr("DIAG HTMLcrtbox Creating Custom Box/Scrolled Window for Screen [%s] [%s] Starting with Widget [%d] at line [%d] program [%s].",scn->module,scn->name,*widgetcount,line,file);
	}
	boxmaxr=HTMLboxmaxrows(scn,*widgetcount);
	boxmaxc=HTMLboxmaxcols(scn,*widgetcount);
	fractbase=boxmaxr*boxmaxc;
	if(fractbase<1) 
	{
		return;
	} else {
	}
	boxmaxr=0;boxmaxc=0;
	++box_count;
	cur_box=box_count;
	fprintf(fp,"<BOX %2d>\n",cur_box);
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+*widgetcount;
		if(wdgt->resource_num>(-1))
		{
		if(diagapps)
		{
			prterr("DIAG Creating Widget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
				*widgetcount,wdgttypes[wdgt->type],(wdgt->resource_name!=NULL ? 
				wdgt->resource_name:""),(wdgt->label!=NULL ? wdgt->label:""),
				wdgt->rows,wdgt->cols,wdgt->rtype); 
		}
		dashes=adddashes(wdgt->resource_name);
		switch(wdgt->type)
		{
			case 8: /* scrolled text */
				fprintf(fp,"<TEXTAREA NAME=\"%s\" ROWS=%d COLS=%d></TEXTAREA>",(dashes!=NULL ? dashes:""),wdgt->rows,wdgt->cols);
				break;
			case 7: /* scrolled list */
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
				fprintf(fp,"<INPUT TYPE=\"text\" NAME=\"%s\" SIZE=\"%d\">",(dashes!=NULL ? dashes:""),wdgt->cols);
				break;
			case 9: /* toggle buttons */
				fprintf(fp,"<INPUT TYPE=\"CheckBox\" NAME=\"%s\" VALUE=\"%s\">%s",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""),(wdgt->label!=NULL ? wdgt->label:""));
				break;
			case 6: /* buttons */
			case 14:/* optional screens */
				switch(RDAapi)
				{
					default:
					case 0: /* doadd buttons */
					case 1: /* doreport buttons */
						fprintf(fp,"<INPUT TYPE=\"Submit\" VALUE=\"%s\">",(wdgt->label!=NULL ? wdgt->label:""));
						break;
					case 2: /* menu buttons */
						fprintf(fp,"<A HREF=\"%s.HTML\">%s</A HREF>",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""));
						break;
				}
				break;
			case 1: /* start newline */
				fprintf(fp,"<BOX %2d LINE %2d>",cur_box,++boxline);
				break;
			case 2: /* endline */
				fprintf(fp,"</BOX %2d LINE %2d><BR>\n",cur_box,++boxline);
				break;
			case 3: /* start box */
				HTMLcrtbox(fp,scn,widgetcount);
				break;
			case 4: /* end box */
				fprintf(fp,"</BOX %2d>\n",cur_box);
				return;
			case 5: /* LABELS */
				if(fractbase)
				{
					fprintf(fp,"%s",(wdgt->label!=NULL ? wdgt->label:""));
				}
				break;
			case 10: /* FRAME */
				break;
			case 11: /* Separator */
				if(fractbase)
				{
					fprintf(fp,"<HR>\n");
				}
				break;
			case 12: /* New ScrolledWindow */
				if(fractbase)
				{
					HTMLcrtbox(fp,scn,widgetcount);
				}
				break;
			case 13: /* End ScrolledWindow */
				if(fractbase)
				{
					prterr("Error Widget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],scn->module,scn->name);
				}
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
				break;
		}
		if(dashes!=NULL) Rfree(dashes);
		}
	}
	fprintf(fp,"</BOX %2d>\n",cur_box);
}
/*---------------------------------------------------------------------------
	HTMLcrtwdgts - function to create widgets inside a window
---------------------------------------------------------------------------*/
void HTMLCreateWidgets(FILE *fp,RDAscrn *scn) 
{
	int maxrows=0;
	int widgetcount=0;
	RDAwdgt *wdgt=NULL;

	if(scn==NULL)
	{
		prterr("Error:  RDAscrn pointer is NULL.");
		return;
	}
	if(diagapps) { prterr("DIAG Creating Custom Window Widgets for Screen [%s] [%s]",scn->module,scn->name); }
	maxrows=HTMLfindmaxlines(scn);
	widgetcount=0;
	while(widgetcount<scn->numwdgts)
	{
		wdgt=scn->wdgts+widgetcount;
		if(wdgt->resource_num>(-1))
		{
		if(diagapps)
		{
			prterr("DIAG creating widget [%d] type [%s] name [%s] label [%s] rows [%d] cols [%d] rtype [%d]",
				widgetcount,wdgttypes[wdgt->type],
				(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
				(wdgt->label!=NULL ? wdgt->label:""),
				wdgt->rows,wdgt->cols,wdgt->rtype); 
		}
		if(wdgt->type==1) /* NEW LINE */
		{
			if(HTMLnumperline(scn,widgetcount))
			{
				HTMLcrtline(fp,scn,&widgetcount);
			} else {
				prterr("Error No Widgets on line# [%d] of Screen [%s][%s].",widgetcount,scn->module,scn->name);
			}
		} else if(wdgt->type==10) /* FRAME */
		{
		} else if(wdgt->type==3) /* NEW BOX */
		{
			HTMLcrtbox(fp,scn,&widgetcount);
		} else if(wdgt->type==12) /* NEW SCROLLED WINDOW */
		{
			HTMLcrtbox(fp,scn,&widgetcount);
		} else { /* ERROR */
			prterr("Error Widget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",widgetcount,wdgt->type,wdgttypes[wdgt->type],scn->module,scn->name);
		}
		}
		++widgetcount;
	}
}
void HTMLsetrscnum(RDAscrn *scn)
{
	int x,norsc=0;
	RDAwdgt *wdgt;

	if(diagapps) 
	{ 
		prterr("DIAG HTMLsetrscnum Setting Resource Numbers on Screen [%s] [%s].",scn->module,scn->name); 
	}
	if(scn!=NULL) 
	{
		for(x=0,wdgt=scn->wdgts;x<scn->numwdgts;++x,++wdgt)
		{
			wdgt->resource_num=(-1);
			if(RDAstrstr(wdgt->resource_name,"["))
			{
				wdgt->resource_num=(-1);
			} else if((wdgt->type==6) || (wdgt->type==14))
			{
				if(!RDAstrcmp(wdgt->resource_name,"HELP") ||
					RDAstrstr(wdgt->resource_name,"DEFINE LIST") ||
					RDAstrstr(wdgt->resource_name,"LOAD") ||
					RDAstrstr(wdgt->resource_name,"PREVIOUS") ||
					RDAstrstr(wdgt->resource_name,"NEXT") ||
					RDAstrstr(wdgt->resource_name,"DEFAULTS"))
				{
					wdgt->resource_num=(-1);
				} else wdgt->resource_num=(++norsc);
			} else if((wdgt->type==15 || wdgt->type==16 || 
				wdgt->type==17 || wdgt->type==18 || 
				wdgt->type==19 || wdgt->type==0 || 
				wdgt->type==7 || wdgt->type==8 || wdgt->type==9) && 
				wdgt->resource_name!=NULL)
			{
				if(!RDAstrcmp(wdgt->resource_name,"HELP") ||
					!RDAstrcmp(wdgt->resource_name,"DEFINE LIST"))
				{
					wdgt->resource_num=(-1);
				} else wdgt->resource_num=(++norsc);
			} else {
				wdgt->resource_num=(++norsc);
			}
		}
	}
}
