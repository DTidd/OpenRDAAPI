#include<guip.hpp>
#include<mix.hpp>

extern int RDAapi;
extern char *prevlink,*url,*prog_path;
int line_count=0,box_count=0;
#define HTMLcrtline(a,b,c,d)	xHTMLcrtline(a,b,c,d,__LINE__,__FILE__) 
static void xHTMLcrtline(FILE *,RDAscrn *,int *,char,int,char *);
#define HTMLcrtbox(a,b,c,d)	xHTMLcrtbox(a,b,c,d,__LINE__,__FILE__)
static void xHTMLcrtbox(FILE *,RDAscrn *,int *,char,int,char *);
#define HTMLnumperline(a,b)	xHTMLnumperline(a,b,__LINE__,__FILE__)
static int xHTMLnumperline(RDAscrn *,int,int,char *);
#ifdef XXX
#define HTMLfindmaxlines(a)	xHTMLfindmaxlines(a,__LINE__,__FILE__)
static int xHTMLfindmaxlines(RDAscrn *,int,char *);
#endif /* XXX */
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
#ifdef USE_SCROLLED_WINDOW
				if(!box)
				{
					box=TRUE;
					++count;
				}
				++boxcount;
#endif /* USE_SCROLLED_WINDOW */
				break;
			case 3: /* new box */
				if(!box)
				{
					box=TRUE;
					++count;
				}
				++boxcount;
				break;
			case 13: /* end scrolled window */
#ifdef USE_SCROLLED_WINDOW
				--boxcount;
				if(boxcount==0) box=FALSE;
#endif /* USE_SCROLLED_WINDOW */
				break;
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
#ifdef XXX
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
#ifdef USE_SCROLLED_WINDOW
				++boxcount;
				box=TRUE;
#endif /* USE_SCROLLED_WINDOW */
				break;
			case 3: /* new box */
				++boxcount;
				box=TRUE;
				break;
			case 13:
#ifdef USE_SCROLLED_WINDOW
				--boxcount;
				if(!boxcount) box=FALSE;
#endif /* USE_SCROLLED_WINDOW */
				break;
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
#endif /* XXX */
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
				if(boxcount==1) ++count;
				++boxcount;
				break;
			case 12: /* new scrolled window */
#ifdef USE_SCROLLED_WINDOW
				if(boxcount==1) ++count;
				++boxcount;
#endif /* USE_SCROLLED_WINDOW */
				break;
			case 4: /* end box */
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
			case 13: /* end scrolled window */
#ifdef USE_SCROLLED_WINDOW
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
#endif /* USE_SCROLLED_WINDOW */
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
				if(boxcount==1) ++wcount;
				++boxcount;
				break;
			case 12: /* new scrolled window */
#ifdef USE_SCROLLED_WINDOW
				if(boxcount==1) ++wcount;
				++boxcount;
#endif /* USE_SCROLLED_WINDOW */
				break;
			case 4: /* end box */
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
			case 13: /* end scrolled window */
#ifdef USE_SCROLLED_WINDOW
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
#endif /* USE_SCROLLED_WINDOW */
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
	int *widgetcount,char perl_wrap,int linex,char *file)
{
	RDAwdgt *wdgt=NULL,*wdgt2=NULL;
	int num=0;
	char *dashes=NULL,*modx=NULL,*libx=NULL;
	RDAScrolledList *MYLIST=NULL;
	int w=0;
	FILE *fpx=NULL;
	char buffer[512];

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
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+*widgetcount;
		if(*widgetcount>0) wdgt2=scn->wdgts+(*widgetcount-1);
			else wdgt2=NULL;
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
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<TEXTAREA NAME=\"%s\" ROWS=%d COLS=%d></TEXTAREA>\";\n",(dashes!=NULL ? dashes:""),wdgt->rows,wdgt->cols);
				} else {
					fprintf(fp,"<TEXTAREA NAME=\"%s\" ROWS=%d COLS=%d></TEXTAREA>",(dashes!=NULL ? dashes:""),wdgt->rows,wdgt->cols);
				}
				break;
			case 7: /* scrolled list */
				modx=adddashes(scn->module);
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+20);
#ifndef WIN32
				sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modx);
#endif
				MYLIST=RDAScrolledListNew(scn->module,wdgt->resource_name);
				if((getScrolledListbin(libx,MYLIST))==(-1))
				{
				} else if(MYLIST->list!=NULL && MYLIST->type==1)
				{
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"<SELECT NAME=\"%s\">\";\n",wdgt->resource_name);
					} else fprintf(fp,"<SELECT NAME=\"%s\">",wdgt->resource_name);
					for(w=0;w<MYLIST->list->numlibs;++w)
					{
						if(!w)
						{
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<OPTION SELECTED VALUE=\"%s\">%s\";\n",MYLIST->list->libs[w],MYLIST->list->libs[w]);
							} else fprintf(fp,"<OPTION SELECTED VALUE=\"%s\">%s",MYLIST->list->libs[w],MYLIST->list->libs[w]);
						} else {
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<OPTION VALUE=\"%s\">%s\";\n",MYLIST->list->libs[w],MYLIST->list->libs[w]);
							} else fprintf(fp,"<OPTION VALUE=\"%s\">%s",MYLIST->list->libs[w],MYLIST->list->libs[w]);
						}
					}
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"</SELECT>\";\n");
					} else fprintf(fp,"</SELECT>");
				} else if(MYLIST->type==0)
				{
					if(!perl_wrap)
					{
						memset(stemp,0,101);
#ifdef WIN32
						sprintf(stemp,"wwwdorpt.exe %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
						sprintf(stemp,"wwwdorpt.lnx %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef LINUX
						sprintf(stemp,"wwwdorpt.lnx %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef RISC6000
						sprintf(stemp,"wwwdorpt.rsc %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef UNIXWARE7
						sprintf(stemp,"wwwdorpt.unw %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef SCO50P
						sprintf(stemp,"wwwdorpt.sco %s \"%s\"",scn->module,wdgt->resource_name);
#endif
						fpx=popen(stemp,"r");
						if(fpx!=NULL)
						{
							while(fgets(buffer,512,fpx)!=NULL)
							{
								fprintf(fp,"%s",buffer);
							}
							pclose(fpx);
						} else {
							prterr("Error:  Couldn't open pipe for reading.");
						}
					} else {
#ifdef WIN32
						fprintf(fp,"\texec '%s/wwwdorpt.exe','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
						fprintf(fp,"\texec '%s/wwwdorpt.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef LINUX
						fprintf(fp,"\texec '%s/wwwdorpt.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef RISC6000
						fprintf(fp,"\texec '%s/wwwdorpt.rsc','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef UNIXWARE7
						fprintf(fp,"\texec '%s/wwwdorpt.unw','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef SCO50P
						fprintf(fp,"\texec '%s/wwwdorpt.sco','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
					}
				}
				if(modx!=NULL) Rfree(modx);
				if(libx!=NULL) Rfree(libx);
				if(MYLIST!=NULL) FreeRDAScrolledList(MYLIST);
				break;
			case 15:/* custom input fields */
			case 16:/* expenditure fields */
			case 17:/* revenue fields */
			case 18:/* balance sheet fields */
			case 19:/* beginning balance fields */
			case 0: /* standard resource fields */
				if(perl_wrap)
				{ 
					fprintf(fp,"\tprint \"<INPUT TYPE=\"text\" NAME=\"%s\" SIZE=\"%d\" MAXLENGTH=%d>\";\n",(dashes!=NULL ? dashes:""),wdgt->cols,wdgt->cols);
				} else fprintf(fp,"<INPUT TYPE=\"text\" NAME=\"%s\" SIZE=\"%d\" MAXLENGTH=%d>",(dashes!=NULL ? dashes:""),wdgt->cols,wdgt->cols);
				break;
			case 9: /* toggle buttons */
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<INPUT TYPE=\"CheckBox\" NAME=\"%s\" VALUE=\"%s\">%s\";\n",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""),(wdgt->label!=NULL ? wdgt->label:""));
				} else fprintf(fp,"<INPUT TYPE=\"CheckBox\" NAME=\"%s\" VALUE=\"%s\">%s",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""),(wdgt->label!=NULL ? wdgt->label:""));
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
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A>\";\n",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A>",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
						} else if(!RDAstrcmp(wdgt->resource_name,"SAVE"))
						{
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<INPUT TYPE=\"Submit\" VALUE=\"%s\">\";\n",(wdgt->label!=NULL ? wdgt->label:""));
								fprintf(fp,"\tprint \"<INPUT TYPE=\"reset\">\";\n");
							} else {
								fprintf(fp,"<INPUT TYPE=\"Submit\" VALUE=\"%s\">",(wdgt->label!=NULL ? wdgt->label:""));
								fprintf(fp,"<INPUT TYPE=\"reset\">");
							}
						} else {
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A>\";\n",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A>",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
						}
						break;
					case 2: /* menu buttons */
						if(!RDAstrcmp(wdgt->resource_name,"QUIT"))
						{
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A>\";\n",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A>",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
						} else {
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A>\";\n",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A>",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
						}
						break;
				}
				break;
			case 1: /* start newline */
				if(num)
				{
					HTMLcrtline(fp,scn,widgetcount,perl_wrap);
				}
				break;
			case 2: /* endline */
				if(wdgt2!=NULL)
				{
					if(wdgt2->type!=11)
					{
						if(perl_wrap)
						{ 
							fprintf(fp,"\tprint \"<BR>\";\n");
						} else fprintf(fp,"<BR>");
					}
				}
				return;
			case 3: /* start box */
				if(num)
				{
					HTMLcrtbox(fp,scn,widgetcount,perl_wrap);
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
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"%s\";\n",(wdgt->label!=NULL ? wdgt->label:""));
					} else fprintf(fp,"%s",(wdgt->label!=NULL ? wdgt->label:""));
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
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"<HR>\";\n");
					} else fprintf(fp,"<HR>");
				}
				break;
			case 12: /* New ScrolledWindow */
#ifdef USE_SCROLLED_WINDOW
				if(num)
				{
					HTMLcrtbox(fp,scn,widgetcount,perl_wrap);
				}
#endif /* USE_SCROLLED_WINDOW */
				break;
			case 13: /* End ScrolledWindow */
#ifdef USE_SCROLLED_WINDOW
				if(num)
				{
					prterr("Error Widget [%d] Type [%d] [%s] on screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],scn->module,scn->name);
				}
#endif /* USE_SCROLLED_WINDOW */
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
				break;
		}
		if(dashes!=NULL) Rfree(dashes);
		}
	}
}
static void xHTMLcrtbox(FILE *fp,RDAscrn *scn,int *widgetcount,char perl_wrap,int line,char *file)
{
	RDAScrolledList *MLST=NULL;
	RDAwdgt *wdgt=NULL;
	int fractbase=0,boxmaxr=0,boxmaxc=0,w=0;
	char *dashes=NULL,*modx=NULL,*libx=NULL;
	FILE *fpx=NULL;
	char buffer[512];

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
	if(perl_wrap)
	{
		fprintf(fp,"\tprint \"<TABLE BORDER>\";\n");
	} else fprintf(fp,"<TABLE BORDER>\n");
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
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<TD><TEXTAREA NAME=\"%s\" ROWS=%d COLS=%d></TEXTAREA></TD>\";\n",(dashes!=NULL ? dashes:""),wdgt->rows,wdgt->cols);
				} else fprintf(fp,"<TD><TEXTAREA NAME=\"%s\" ROWS=%d COLS=%d></TEXTAREA></TD>",(dashes!=NULL ? dashes:""),wdgt->rows,wdgt->cols);
				break;
			case 7: /* scrolled list */
				modx=adddashes(scn->module);
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+20);
#ifndef WIN32
				sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modx);
#endif
				MLST=RDAScrolledListNew(scn->module,wdgt->resource_name);
				if((getScrolledListbin(libx,MLST))==(-1))
				{
				} else if(MLST->list!=NULL && MLST->type==1)
				{
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"<TD><SELECT NAME=\"%s\">\";\n",wdgt->resource_name);
					} else fprintf(fp,"<TD><SELECT NAME=\"%s\">",wdgt->resource_name);
					for(w=0;w<MLST->list->numlibs;++w)
					{
						if(!w)
						{
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<OPTION SELECTED VALUE=\"%s\">%s\";\n",MLST->list->libs[w],MLST->list->libs[w]);
							} else fprintf(fp,"<OPTION SELECTED VALUE=\"%s\">%s",MLST->list->libs[w],MLST->list->libs[w]);
						} else {
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<OPTION VALUE=\"%s\">%s\";\n",MLST->list->libs[w],MLST->list->libs[w]);
							} else fprintf(fp,"<OPTION VALUE=\"%s\">%s",MLST->list->libs[w],MLST->list->libs[w]);
						}
					}
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"</SELECT></TD>\";\n");
					} else fprintf(fp,"</SELECT></TD>");
				} else if(MLST->type==0)
				{
					if(!perl_wrap)
					{
						memset(stemp,0,101);
#ifdef WIN32
						sprintf(stemp,"wwwdorpt.exe %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
						sprintf(stemp,"wwwdorpt.lnx %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef LINUX
						sprintf(stemp,"wwwdorpt.lnx %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef RISC6000
						sprintf(stemp,"wwwdorpt.rsc %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef UNIXWARE7
						sprintf(stemp,"wwwdorpt.unw %s \"%s\"",scn->module,wdgt->resource_name);
#endif
#ifdef SCO50P
						sprintf(stemp,"wwwdorpt.sco %s \"%s\"",scn->module,wdgt->resource_name);
#endif
						fpx=popen(stemp,"r");
						if(fpx!=NULL)
						{
							while(fgets(buffer,512,fpx)!=NULL)
							{
								fprintf(fp,"%s",buffer);
							}
							pclose(fpx);
						} else {
							prterr("Error:  Couldn't open pipe for reading.");
						}
					} else {
#ifdef WIN32
						fprintf(fp,"\texec '%s/wwwdorpt.exe','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
						fprintf(fp,"\texec '%s/wwwdorpt.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef LINUX
						fprintf(fp,"\texec '%s/wwwdorpt.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef RISC6000
						fprintf(fp,"\texec '%s/wwwdorpt.rsc','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef UNIXWARE7
						fprintf(fp,"\texec '%s/wwwdorpt.unw','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
#ifdef SCO50P
						fprintf(fp,"\texec '%s/wwwdorpt.sco','%s','%s';\n",(prog_path!=NULL ? prog_path:""),scn->module,wdgt->resource_name);
#endif
					}
				} 
				if(modx!=NULL) Rfree(modx);
				if(libx!=NULL) Rfree(libx);
				if(MLST!=NULL) FreeRDAScrolledList(MLST);
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<TD><INPUT TYPE=\"text\" NAME=\"%s\" SIZE=\"%d\" MAXLENGTH=%d></TD>\";\n",(dashes!=NULL ? dashes:""),wdgt->cols,wdgt->cols);
				} else fprintf(fp,"<TD><INPUT TYPE=\"text\" NAME=\"%s\" SIZE=\"%d\" MAXLENGTH=%d></TD>",(dashes!=NULL ? dashes:""),wdgt->cols,wdgt->cols);
				break;
			case 9: /* toggle buttons */
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<TD><INPUT TYPE=\"CheckBox\" NAME=\"%s\" VALUE=\"%s\">%s</TD>\";\n",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""),(wdgt->label!=NULL ? wdgt->label:""));
				} else fprintf(fp,"<TD><INPUT TYPE=\"CheckBox\" NAME=\"%s\" VALUE=\"%s\">%s</TD>",(dashes!=NULL ? dashes:""),(wdgt->label!=NULL ? wdgt->label:""),(wdgt->label!=NULL ? wdgt->label:""));
				break;
			case 6: /* buttons */
			case 14:/* optional screens */
				switch(RDAapi)
				{
					default:
					case 0: /* doadd buttons */
					case 1: /* doreport buttons */
						if(!RDAstrcmp(wdgt->resource_name,"QUIT"))
						{
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A></TD>\";\n",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A></TD>",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
						} else if(!RDAstrcmp(wdgt->resource_name,"SAVE"))
						{
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<TD><INPUT TYPE=\"Submit\" VALUE=\"%s\"></TD>\";\n",(wdgt->label!=NULL ? wdgt->label:""));
								fprintf(fp,"\tprint \"<TD><INPUT TYPE=\"reset\"></TD>\";\n");
							} else {
								fprintf(fp,"<TD><INPUT TYPE=\"Submit\" VALUE=\"%s\"></TD>",(wdgt->label!=NULL ? wdgt->label:""));
								fprintf(fp,"<TD><INPUT TYPE=\"reset\"></TD>");
							}
						} else {
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A></TD>\";\n",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A></TD>",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
						}
						break;
					case 2: /* menu buttons */
						if(!RDAstrcmp(wdgt->resource_name,"QUIT"))
						{
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A></TD>\";\n",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"cancel.gif\" ALT=\"CANCEL\"></A></TD>",(url!=NULL ? url:""),(prevlink!=NULL ? prevlink:""),(wdgt->label!=NULL ? wdgt->label:""));
						} else {
							if(perl_wrap)
							{
								fprintf(fp,"\tprint \"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A></TD>\";\n",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
							} else fprintf(fp,"<TD><A HREF=\"%s/%s.htm\"><IMG SRC=\"%s.gif\" ALT=\"%s\"></A></TD>",(url!=NULL ? url:""),wdgt->resource_name,wdgt->resource_name,(wdgt->label!=NULL ? wdgt->label:""));
						}
						break;
				}
				break;
			case 1: /* start newline */
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<TR>\";\n");
				} else fprintf(fp,"<TR>");
				break;
			case 2: /* endline */
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"</TR>\";\n");
				} else fprintf(fp,"</TR>\n");
				break;
			case 3: /* start box */
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<TD>\";\n");
				} else fprintf(fp,"<TD>");
				HTMLcrtbox(fp,scn,widgetcount,perl_wrap);
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"</TD>\";");
				} else fprintf(fp,"</TD>");
				break;
			case 4: /* end box */
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"</TABLE>\";\n");
				} else fprintf(fp,"</TABLE>\n");
				return;
			case 5: /* LABELS */
				if(fractbase)
				{
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"<TD>%s</TD>\";\n",(wdgt->label!=NULL ? wdgt->label:""));
					} else fprintf(fp,"<TD>%s</TD>",(wdgt->label!=NULL ? wdgt->label:""));
				}
				break;
			case 10: /* FRAME */
				break;
			case 11: /* Separator */
				if(fractbase)
				{
					if(perl_wrap)
					{
						fprintf(fp,"\tprint \"<HR>\";\n");
					} else fprintf(fp,"<HR>");
				}
				break;
			case 12: /* New ScrolledWindow */
#ifdef USE_SCROLLED_WINDOW
				if(fractbase)
				{
					HTMLcrtbox(fp,scn,widgetcount,perl_wrap);
				}
#endif /* USE_SCROLLED_WINDOW */
				break;
			case 13: /* End ScrolledWindow */
#ifdef USE_SCROLLED_WINDOW
				if(fractbase)
				{
					prterr("Error Widget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],scn->module,scn->name);
				}
#endif /* USE_SCROLLED_WINDOW */
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
				break;
		}
		if(dashes!=NULL) Rfree(dashes);
		}
	}
#ifdef XXXX
	if(perl_wrap)
	{
		fprintf(fp,"\tprint \"</TABLE><BR>\";\n");
	} else fprintf(fp,"</TABLE><BR>\n");
#endif /* XXXX */
}
/*---------------------------------------------------------------------------
	HTMLcrtwdgts - function to create widgets inside a window
---------------------------------------------------------------------------*/
void HTMLCreateWidgets(FILE *fp,RDAscrn *scn,char perl_wrap) 
{
	int widgetcount=0;
	RDAwdgt *wdgt=NULL;

	if(scn==NULL)
	{
		prterr("Error:  RDAscrn pointer is NULL.");
		return;
	}
	if(diagapps) { prterr("DIAG Creating Custom Window Widgets for Screen [%s] [%s]",scn->module,scn->name); }
/*
	maxrows=HTMLfindmaxlines(scn);
*/
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
				if(perl_wrap)
				{
					fprintf(fp,"\tprint \"<P ALIGN=\\\"CENTER\\\">\";\n");
				} else fprintf(fp,"<P ALIGN=\"CENTER\">");
				HTMLcrtline(fp,scn,&widgetcount,perl_wrap);
				if(perl_wrap)
				{
				       	fprintf(fp,"\tprint \"</P>\";\n");
				} else fprintf(fp,"</P>");
			} else {
				prterr("Error No Widgets on line# [%d] of Screen [%s][%s].",widgetcount,scn->module,scn->name);
			}
		} else if(wdgt->type==10) /* FRAME */
		{
		} else if(wdgt->type==3) /* NEW BOX */
		{
			if(perl_wrap)
			{
				fprintf(fp,"\tprint \"<P ALIGN=\\\"CENTER\\\">\";\n");
			} else fprintf(fp,"<P ALIGN=\"CENTER\">");
			HTMLcrtbox(fp,scn,&widgetcount,perl_wrap);
			if(perl_wrap)
			{ 
				fprintf(fp,"\tprint \"</P>\";\n");
			} else  fprintf(fp,"</P>");
		} else if(wdgt->type==12) /* NEW SCROLLED WINDOW */
		{
#ifdef USE_SCROLLED_WINDOW
			if(perl_wrap)
			{
				fprintf(fp,"\tprint \"<P ALIGN=\\\"CENTER\\\">\";\n");
			} else fprintf(fp,"<P ALIGN=\"CENTER\">");
			HTMLcrtbox(fp,scn,&widgetcount,perl_wrap);
			if(perl_wrap)
			{
				fprintf(fp,"\tprint \"</P>\";\n");
			} else fprintf(fp,"</P>");
#endif /* USE_SCROLLED_WINDOW */
		} else if(wdgt->type==13) /* End Scrolled Window */
		{
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
