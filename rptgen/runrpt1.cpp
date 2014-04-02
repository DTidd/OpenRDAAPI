/* runrpt1.c - Run Report Functions Additional Program #1 */
/*COMPMEM 36864 */
#include<rptgen.hpp>
#include<doheader.hpp>

char string_temparray[501];
char ArchiveExtension=0;
short ArchiveSource=4;

static void print_list(RDArsrc *,HoldReport *);
static void checkheader(RDA_PFILE *,int *,int *,char *);
static void printheader(RDA_PFILE *,int *,int *,char *);
static void quit_print(RDArsrc *,HoldReport *);
static void printerrorlist(RDA_PFILE *,HoldReport *,int *,int *);
static void printerrorlistcb(RDArsrc *,HoldReport *);
short RPTGEN_GUIEVAL(char *,RDArsrc *,HoldReport *);
char *RPTGEN_GUIEVALSTR(char *,RDArsrc *,void *,HoldReport *);
void run_report_chain(RDArsrc *,HoldReport *);
void report_run_dos_copy(RDArsrc *,HoldReport *);


static void makeaccum(RDAreport *,RDAaccum *,short,short);
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */


void ReportComputeSCRNcallback(RDArsrc *r,HoldReport *HoldReportx)
{
	readallwidgets(r);
	IncVirRsrcstoFile(r);
	ADVcomputeandupdateallSCRNvirtuals(r,ReportGenSubData,HoldReportx,TRUE);
}

static void checkheader(RDA_PFILE *fp,int *lines,int *pages,char *header)
{
	if(doheaders)
	{
		if(*lines>57)
		{
			RDA_fprintf(fp,"\f");
			printheader(fp,lines,pages,header);
		}
	}
}
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *header)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;
	char *mainheader=NULL;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\n");
	*lines=1;
	
	if(!isEMPTY(XPERT_SETUP->company_name))
	{
		mainheader=stralloc(XPERT_SETUP->company_name);
	} else {
		mainheader=stralloc("No Organization Name");
	}



	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(mainheader)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(mainheader)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(mainheader)+2+16+lead_ws+tail_ws)>line_length) mainheader[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",mainheader,tail_ws,"",10,++*pages);
	++*lines;
	if(mainheader!=NULL) Rfree(mainheader);


	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sDate: %s\r\n",
		lead_ws,"",header,tail_ws,"",date);
	++*lines;



	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\r\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
			++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}

	RDA_fprintf(fp,"\r\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void quit_print(RDArsrc *prsrc,HoldReport *h)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(prsrc!=NULL) free_rsrc(prsrc);
}

static void printerrorlist(RDA_PFILE *fp,HoldReport *h,int *lines,int *pages)
{
	int x;
	char *temp=NULL;

	if(fp!=NULL)
	{
		if(h!=NULL)
		{
			temp=Rmalloc(RDAstrlen(h->rrpt->module)+RDAstrlen(h->rpt->name)+24);
			sprintf(temp,"REPORT [%s][%s] ERROR LIST",(h->rrpt->module!=NULL?h->rrpt->module:""),(h->rpt->name!=NULL?h->rpt->name:""));
			printheader(fp,lines,pages,temp);
	
			for(x=0;x<h->rrpt->errorlist->numlibs;++x)
			{
				RDA_fprintf(fp,"(%5d) %s\r\n",(x+1),h->rrpt->errorlist->libs[x]);
				++*lines;
				checkheader(fp,lines,pages,temp);
			}
			if(temp!=NULL) Rfree(temp);
			RDA_fprintf(fp,"\f");
		}
	}
}

static void printerrorlistcb(RDArsrc *prsrc,HoldReport *h)
{
	char *outdevice=NULL,*message=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;
	int lines=0,pages=0;

	if(h!=NULL)
	{
		if(h->rpt!=NULL)
		{
			if(h->rrpt!=NULL)
			{
				readwidget(prsrc,"DEVICE");
				readwidget(prsrc,"NORMAL PRINT");
				FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
				FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
				if(outdevice!=NULL)
				{
					fp=RDA_popen(outdevice,"w");
					if(fp!=NULL)
					{
						SelectPrintType(outdevice,print_style,fp);
						printerrorlist(fp,h,&lines,&pages);
						RDA_pclose(fp);
					} else {
						message=Rmalloc(RDAstrlen(outdevice)+69);
						sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
						WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
						prterr(message);
						if(message!=NULL) Rfree(message);
					}
					if(outdevice!=NULL) Rfree(outdevice);
				} else {
					ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
				}
			}
		}
	}
}
static void print_list(RDArsrc *parent,HoldReport *h)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(h!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG printerrorlist Displaying the Print Screen in preparation to print Error List.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printerrorlistcb,h);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print,h);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print(prsrc,h);
	}
	}
}

void RDAreportSetFunc(HoldReport *h)
{
	RDAreport *rpt=NULL;
	RDAvirtual *v=NULL;
	int x=0;

	if(h!=NULL)
	{
		rpt=h->rpt;
		if(rpt!=NULL)
		{
			if(rpt->virflds!=NULL)
			{
				for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
				{
					v->computed=FALSE;
				}
			}
		}
	}
}

static void shutdown_report(RDArsrc *rsrc)
{
	ShutdownSubsystems();
}
void xexit_cleanly(RDArunrpt *runrpt,RDAreport *rpt,short exit_report,char *err_header,char *err_message,short remove_sort,char error_type,int line,char *file)
{
	char *temp=NULL;
	RDAvirtual *v;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG xexit_cleanly called at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(runrpt->sortno!=(-1)) CLSNRD(runrpt->sortno);
	if(remove_sort==TRUE)
	{
		DELETENRD("RPTGEN",runrpt->sortfile,rpt->engine_type,rpt->server_name);
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6+10);
#ifndef WIN32
		sprintf(temp,"%s/rda/%s.FIL",CURRENTDIRECTORY,"RPTGEN");
#endif
#ifdef WIN32
		sprintf(temp,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,"RPTGEN");
#endif
		deletelibbin(temp,runrpt->sortfile);
		if(temp!=NULL) Rfree(temp);
	}
	if(rpt!=NULL)
	{
		if(rpt->virflds!=NULL)
		{
			for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
			{
				if(v->range==3) break;
			}
			if(x<rpt->numvirtuals)
			{
				temp=adddashes(v->name);
				RDA_UnSetEnv(temp);
				if(temp!=NULL) Rfree(temp);
			}
		}
	}
	if(!isEMPTY(err_header))
	{
		if(rpt->no_empty_error==FALSE && error_type==0)
		{
			if(runrpt->exit_status!=2)
			{
				ERRORDIALOG(err_header,err_message,(exit_report==TRUE?shutdown_report:NULL),exit_report);
			} else {
				ERRORDIALOG(err_header,err_message,NULL,FALSE);
			}
			prterr(err_message);
		} else {
			if(runrpt->exit_status!=2)
			{
				if(exit_report==TRUE) 
				{
					prterr(err_message);
					shutdown_report(runrpt->rsrsrc);
				}
			}
		}
	}
	if(runrpt!=NULL) CLSReportfiles(runrpt,FALSE);
	if(isEMPTY(err_header))
	{
		if((exit_report==TRUE) && (runrpt->exit_status!=2))
		{
			shutdown_report(runrpt->rsrsrc);
			return;
		}
	}
	if(exit_report==2)
	{
/*lint -e746 */
		if(runrpt->quitfunc!=NULL) 
		{
			runrpt->quitfunc(runrpt->quitfuncrsrc,runrpt->quitfuncArgs);
		}
/*lint +e746 */
		if(rpt!=NULL)
		{
			FreeRDAreport(rpt);
			rpt=NULL;
		}
 		if(runrpt!=NULL)
		{
			FreeRDArunrpt(runrpt);
			runrpt=NULL;
		}
/* DCT: added this ShutdownSubsystems() on 4/1/2014 before finding the
        audit trail, POAUDITENV was chained to didn't have the environment 
	type virtual fields, so it couldn't remove the cookies.  This might
	not be necessary.  Haven't yet found that it fails though....   
*/
		ShutdownSubsystems();
	} else if(exit_report==1)
	{
		if(!runrpt->APPmainLoop) 
		{
			runrpt->APPmainLoop=TRUE;
			if(rpt!=NULL)
			{
				FreeRDAreport(rpt);
				rpt=NULL;
			}
 			if(runrpt!=NULL)
			{
				FreeRDArunrpt(runrpt);
				runrpt=NULL;
			}
			return;
		}
		if(rpt!=NULL)
		{
			FreeRDAreport(rpt);
			rpt=NULL;
		}
 		if(runrpt!=NULL)
		{
			FreeRDArunrpt(runrpt);
			runrpt=NULL;
		}
	}
}
static void PreParseRDAreport(RDAreport *r)
{
	int x,y;
	RDAvirtual *v=NULL;
	RDAsearch *s=NULL;
	RDAimage *i=NULL;	
	RDApval *pval=NULL;
	RDAdisplay *d=NULL;
	char *temp=NULL;

	if(r!=NULL)
	{
		d=r->display;
		if(r->virflds!=NULL)
		{
			for(x=0,v=r->virflds;x<r->numvirtuals;++x,++v)
			{
				if(!isEMPTY(v->expression))
				{
					temp=PP_translate(v->expression);
					Rfree(v->expression);	
					if(temp!=NULL)
					{
						v->expression=stralloc(temp);
						Rfree(temp);
					}
				}		
				v->computed=FALSE;
			}
		}
		if(r->search!=NULL)
		{
			for(x=0,s=r->search;x<r->numsearchs;++x,++s)
			{
				if(s->ctype==18)
				{
					s->ctype=0;
					s->key_fastbreak=TRUE;
				} else if(s->ctype==19)
				{
					s->ctype=16;
					s->key_fastbreak=TRUE;
				}
				if(!isEMPTY(s->expression))
				{
					temp=PP_translate(s->expression);
					Rfree(s->expression);
					if(temp!=NULL)
					{
						s->expression=stralloc(temp);
						Rfree(temp);
					}
				}
				if(!isEMPTY(s->skip_detail))
				{
					temp=PP_translate(s->skip_detail);
					Rfree(s->skip_detail);
					if(temp!=NULL)
					{
						s->skip_detail=stralloc(temp);
						Rfree(temp);
					}
				}
				if(!isEMPTY(s->wc_expression))
				{
					temp=PP_translate(s->wc_expression);
					Rfree(s->wc_expression);
					if(temp!=NULL)
					{
						s->wc_expression=stralloc(temp);
						Rfree(temp);
					}
				}
			}
		}
		if(!isEMPTY(r->select))
		{
			temp=PP_translate(r->select);
			Rfree(r->select);
			if(temp!=NULL) 
			{
				r->select=stralloc(temp);
				Rfree(temp);
			}
		}
		if(r->image!=NULL)
		{
			for(x=0,i=r->image;x<r->numimages;++x,++i)
			{
				if(!isEMPTY(i->expression))
				{
					temp=PP_translate(i->expression);
					Rfree(i->expression);
					if(temp!=NULL) 
					{
						i->expression=stralloc(temp);
						Rfree(temp);
					}
				}
				if(i->pvals!=NULL && (d->device==3 ||
					d->device==4))
				{
					for(y=0,pval=i->pvals;y<i->numpvals;
						++y,++pval)
					{
						if(pval->symbol_type &&
							!isEMPTY(pval->name))
						{
							temp=PP_translate(pval->name);
							Rfree(pval->name);
							if(temp!=NULL)
							{
								pval->name=stralloc(temp);
								Rfree(temp);
							}
						}
					}
				}
			}
		}
	}
}
void clear_runtime_virtuals(RDAreport *rpt)
{
	RDAvirtual *v;
	int x;

	if(rpt->virflds!=NULL)
	{
		for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
		{
			if(v->when)
			{
				v->computed=FALSE;
			}
		}
	}
}
void compute_runtime_virtuals(RDArunrpt *rrpt,RDAreport *rpt)
{
	RDAvirtual *v;
	int x;
	HoldReport *h=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG compute_runtime_virtuals Computing Runtime Virtual fields.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rpt->virflds!=NULL)
	{
		h=HoldReportNew(rrpt,rpt);
		for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
		{
			if(v->when)
			{
				compute_virtual_field(rrpt,rpt,v->name,ReportGenSubData1,h);
			}
		}
		if(h!=NULL) Rfree(h);
	}
}
int xPageFooterSize(RDArunrpt *rrpt,RDAreport *rpt,int line,char *file)
{
	int tmp=0,hold=0;
	int x,y,z;
	RDAimage *i;
	RDApval *pval;
	RDAdisplay *d;
	RDApvar *pvar;
	HoldReport *h=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG PageFooterSize Computing the Page size to reserve for the footer at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rpt->display==NULL) 
	{
		return(0);
	}
	d=rpt->display;
	switch(d->device)
	{
		default:
		case 0: /* printer */
		case 1: /* ascii file */
		case 2: /* append ascii file */
		case 5: /* popen */
			d=rpt->display;
			h=HoldReportNew(rrpt,rpt);
			for(x=0,i=rpt->image;x<rpt->numimages;++x,++i)
			{
				hold=0;
				if(!RDAstrcmp(i->breakname,"PAGE FOOTER"))
				{
					if(!isEMPTY(i->expression))
					{
						if(PP_EVALUATEbol(i->expression,ReportGenSubData1,h))
						{
							for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
							{
								if(pval->symbol_type==1)
								{
									for(z=0,pvar=d->pvars;z<d->num;++z,++pvar)
									{
										if(!RDAstrcmp(pvar->name,pval->name)) break;
									}
									if(z<d->num)
									{
										if(pvar->points>hold) 
											hold=pvar->points;
									}
								}
							}
							if(!hold) hold=rrpt->points;
						}
					} else {
						for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
						{
							if(pval->symbol_type==1)
							{
								for(z=0,pvar=d->pvars;z<d->num;++z,++pvar)
								{
									if(!RDAstrcmp(pvar->name,pval->name)) break;
								}
								if(z<d->num)
								{
									if(pvar->points>hold) 
										hold=pvar->points;
								}
							}
						}
						if(!hold) hold=rrpt->points;
					}
				}
				tmp+=hold;
			}
			if(h!=NULL) Rfree(h);
			h=NULL;
			break;
		case 3: /* datafile */
		case 4: /* update datafile */
			break;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG PageFooterSize returning [%d].",tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(tmp);
}
short xtestform(RDArunrpt *rrpt,RDAreport *rpt,short forced,short test_page,int line,char *file)
{
	RDAdisplay *d;
	short x=0,blevel=0;
	char *temp_cur_brk_name=NULL;

	/* save the current break name so it can be restored later */
	blevel=rpt->currbrk;
	if(rrpt->cur_brk_name!=NULL)
	{
		temp_cur_brk_name = stralloc(rrpt->cur_brk_name);
	}

	d=rpt->display;
	if(rrpt->points==0) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen)
		{
			prterr("DIAG Assuming 12 points, this is standard Pica Print.");
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		rrpt->points=12;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG testform Testing Form Position [%d] per Page [%d] Points [%d] Body Count [%d] Break at Body Count [%d] forced [%s] rrpt->forced [%s] Page [%d] test_page [%d] rrpt->cur_brk_name [%s] at line [%d] program [%s].",
			(rrpt->numlines+rrpt->points),rrpt->page_count,
			rrpt->points,
			rrpt->body_count,d->body_count,(forced ? "Yes":"No"),
			(rrpt->forced ? "Yes":"No"),rrpt->pageno,test_page,
			(rrpt->cur_brk_name!=NULL?rrpt->cur_brk_name:""),
			line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
/* Do d->body_count_type==TRUE test over limit here */
	if(d->body_count_type==TRUE && 
		!RDAstrcmp(rrpt->cur_brk_name,"DETAIL LINES"))
	{
		if((rrpt->body_count+1)>d->body_count && d->body_count>0)
		{
			process_controls(rrpt,rpt,(rpt->numcontrols-1),FALSE);
			rrpt->body_count=0;
		}
	}
	if((((rrpt->numlines+rrpt->points)>rrpt->page_count && 
		rrpt->page_count!=0) || rrpt->body_count<0 || !rrpt->pageno || 
		forced) && !rrpt->forced)
	{
		rrpt->forced=TRUE;
		if(rrpt->pageno) 
		{
			print_section(rrpt,rpt,"PAGE FOOTER",test_page);
			if(test_page && d->test_pages>=rrpt->pageno) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen)
				{
					prterr("DIAG testform returning (-1)");
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				rrpt->forced=FALSE;
				/* restore the current break name */
				if(rrpt->cur_brk_name!=NULL) Rfree(rrpt->cur_brk_name);
				rpt->currbrk=blevel;
				if(temp_cur_brk_name!=NULL)
				{
					/* restore the current break name */
					rrpt->cur_brk_name = stralloc(temp_cur_brk_name);
					Rfree(temp_cur_brk_name);
				}
				return(-1);
			}
			++rrpt->pageno;
			rrpt->numlines=PageFooterSize(rrpt,rpt);
		} else {
			if(d->starting_page!=0) rrpt->pageno=d->starting_page;
				else rrpt->pageno=1;
			rrpt->numlines=PageFooterSize(rrpt,rpt);
			print_section(rrpt,rpt,"REPORT HEADER",test_page);
		}
		print_section(rrpt,rpt,"PAGE HEADER",test_page);
		rrpt->forced=FALSE;
	}
	rrpt->numlines+=rrpt->points;
	if(rrpt->numlines==rrpt->page_count && rrpt->pageno && !test_page)
	{
		x=PRVNRD(rrpt->sortno,1);
		rrpt->forced=TRUE;
		print_section(rrpt,rpt,"PAGE FOOTER",test_page);
		if(!x) NXTNRD(rrpt->sortno,1);
		++rrpt->pageno;
		rrpt->numlines=PageFooterSize(rrpt,rpt);
		print_section(rrpt,rpt,"PAGE HEADER",test_page);
		rrpt->forced=FALSE;
	}
	if(rrpt->cur_brk_name!=NULL) Rfree(rrpt->cur_brk_name);
	/* restore the current break name */
	if(temp_cur_brk_name!=NULL)
	{
		rrpt->cur_brk_name = stralloc(temp_cur_brk_name);
		Rfree(temp_cur_brk_name);
		rpt->currbrk=blevel;
	}
	return(TRUE);
}
void clear_accum(RDAreport *rpt,int number)
{
	int x;
	RDAaccum *a;
	RDAaccval *accval;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG clear_accum Clearing Accumulator Values for break level [%d].",number);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rpt->accum!=NULL)
	{
	for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
	{
#ifdef XXX
		c=rpt->control+(number-2);
		c->header_count=0;
		c->footer_count=0;
#endif
		if(a->num<number)
		{
			prterr("Error:  Accumulator a->name [%s] a->num [%d] number [%d] couldn't be resolved....",(a->name!=NULL ? a->name:""),a->num,number);
		} else {
		accval=a->accvals+number;
		if(accval!=NULL)
		{
			accval->count=0;
		} else {
			prterr("Error:  What are you doing here?... love Raddog...");TRACE;
		}
		switch(a->fieldtype)
		{
			case SCROLLEDTEXT:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case VARIABLETEXT:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				switch(a->atype)
				{
					case 2: /* FIRST OCCURRENCE */
					case 3: /* LAST OCCURRENCE */
						break;
					case 6: /* PREVIOUS OCCURRENCE */
						if(!isEMPTY(accval->value.string_value))
						{
							QUICKALLOC(accval->prev_value.string_value,accval->pdlen,accval->dlen);
							memcpy(accval->prev_value.string_value,accval->value.string_value,accval->dlen);
							Rfree(accval->value.string_value);
							accval->value.string_value=NULL;
							accval->dlen=0;
						} else if(accval->pdlen>0)
						{
							memset(accval->prev_value.string_value,0,accval->pdlen);
						} else {
							accval->prev_value.string_value=NULL;
							accval->pdlen=0;
						}
						break;
					case 0: /* AVERAGE */
					case 4: /* MAXIMUM */
					case 5: /* MINIMUM */
					case 7: /* SUMMATION */
						if(accval->value.string_value!=NULL) Rfree(accval->value.string_value);
						accval->value.string_value=NULL;
						accval->dlen=0;
						if(accval->prev_value.string_value!=NULL) Rfree(accval->prev_value.string_value);
						accval->prev_value.string_value=NULL;
						accval->pdlen=0;
						break;
					case 1: /* FREQUENCY */
						*accval->prev_value.float_value=*accval->value.float_value;
						*accval->value.float_value=0;
						break;
					default:
						prterr("Error unrecognized accumulator type.");
						break;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SHORTV:
			case LONGV:
			case SCROLLEDLIST:
			case SSHORTV:
			case SLONGV:
			case SDOUBLEV:
			case SDECIMALV:
				switch(a->atype)
				{
					case 2: /* FIRST OCCURRENCE */
					case 3: /* LAST OCCURRENCE */
						break;
					case 6: /* PREVIOUS OCCURRENCE */
					case 0: /* AVERAGE */
					case 4: /* MAXIMUM */
					case 5: /* MINIMUM */
					case 7: /* SUMMATION */
					case 1: /* FREQUENCY */
						*accval->prev_value.float_value=*accval->value.float_value;
						*accval->value.float_value=0;
						break;
					default:
						prterr("Error unrecognized accumulator type.");
						break;
				}
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error unrecognized field type.");
				break;
		}
		}
	}
	}
}
short xprint_section(RDArunrpt *rrpt,RDAreport *rpt,char *name,short test_page,
	int line,char *file)
{
	RDAimage *i;
	int x;
	RDAcontrol *c;
	short ret=FALSE,ret1=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG print_section Printing Section [%s] of Report Image at line [%d] program [%s].",name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rpt->currbrk=(-1);
	if(rrpt->prev_brk_name2!=NULL) Rfree(rrpt->prev_brk_name2);
	if(rrpt->prev_brk_name!=NULL) 
	{
		rrpt->prev_brk_name2=stralloc(rrpt->prev_brk_name);
		Rfree(rrpt->prev_brk_name);
	}
	if(!isEMPTY(rrpt->cur_brk_name)) 
	{
		rrpt->prev_brk_name=stralloc(rrpt->cur_brk_name);
	} else rrpt->prev_brk_name=NULL;
	if(rrpt->cur_brk_name!=NULL) Rfree(rrpt->cur_brk_name);
	rrpt->cur_brk_name=stralloc(name);
	if(!RDAstrcmp(name,"REPORT HEADER") || !RDAstrcmp(name,"REPORT FOOTER"))
	{
		rpt->currbrk=0;
	} else if(!RDAstrcmp(name,"PAGE HEADER") || !RDAstrcmp(name,"PAGE FOOTER"))
	{
		rpt->currbrk=1;
	} else {
		if(rpt->control!=NULL)
		{
			for(x=0,c=rpt->control;x<rpt->numcontrols;++x,++c)
			{
				if(!strncmp(c->name,name,RDAstrlen(c->name)))
				{
					rpt->currbrk=x+2;
					break;
				}
			}
		}
	}
	if(rpt->currbrk==(-1))
	{
		rpt->currbrk=rpt->numcontrols+1;
	}
	for(x=0,i=rpt->image;x<rpt->numimages;++x,++i)
	{
		if(!RDAstrcmp(i->breakname,name)) 
		{
			ret1=print_line(rrpt,rpt,i,test_page,rpt->currbrk);
			if(ret1) ret=TRUE;
		}
	}
	if(!test_page)
	{
		if(!RDAstrcmp(name,"PAGE FOOTER")) 
		{
			clear_accum(rpt,1);
		}
		if(ret || !rpt->manual_form) clear_runtime_virtuals(rpt);
	}
	return(ret);
}
HoldReport *xHoldReportNew(RDArunrpt *rrpt,RDAreport *rpt,int line,char *file)
{
	HoldReport *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG HoldReportNew at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(HoldReport));
	if(tmp!=NULL)
	{
		tmp->rrpt=rrpt;
		tmp->rpt=rpt;
		tmp->mtnrsrc=NULL;
	}
	return(tmp);
}
short RPTGEN_GUIEVAL(char *string,RDArsrc *r,HoldReport *h)
{
	if(!isEMPTY(string))
	{
		return(EVALUATEbol(string,ReportGenSubData,(void *)h));
	} else return(TRUE);
}
char *RPTGEN_GUIEVALSTR(char *string,RDArsrc *r,void *SubFuncArgs,HoldReport *h)
{
	char *tmp=NULL;

	if(!isEMPTY(string))
	{
		tmp=PP_EVALUATEstr(string,ReportGenSubData,(void *)h);
	}
	return(tmp);
}
static void ResetDefaults(RDArsrc *r,HoldReport *HoldReportx)
{
	DefaultScreens(HoldReportx->rrpt->rsrsrc);
	ADVcomputeandupdateallSCRNvirtuals(HoldReportx->rrpt->rsrsrc,ReportGenSubData,HoldReportx,TRUE);
	updateallrsrc(HoldReportx->rrpt->rsrsrc);
}

void xRUNREPORT(char *module,char *name,char *server_name,char *sortfile,short APPmainLoop,short exitstatus,RDArsrc *sharedrsrc,void (*quitfunc)(...),RDArsrc *quitfuncrsrc,void *quitfuncArgs,int level,void (*PDLine)(...),int line,char *file)
{
	int x,fpage=1,tpage=99999999;
	RDAreport *rpt=NULL;
	RDArunrpt *runrpt=NULL;
	char *errordesc=NULL;
	char *temp=NULL,rop=FALSE;
	ReportFile *filex=NULL;
	HoldReport *HoldReportx=NULL,*h=NULL;
	RDAvirtual *v=NULL;
	short Default_Duplex_Printing=(-1);
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

#ifdef XXXX
#ifdef USE_CPU_PROFILER
	//ProfilerStart("rptgen.log");
	//prterr("Testing CPU_PROFILER");
#endif /* USE_CPU_PROFILER */

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		x=0;
#ifdef WIN32
		while(environ[x]!=NULL)
		{
			prterr("Environment Variable: %s",environ[x]);
			++x;
		}
#else
		while(__environ[x]!=NULL)
		{
			prterr("Environment Variable: %s",__environ[x]);
			++x;
		}
#endif
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#endif /* XXXX */

	x=0;
	ArchiveSource=4;
	ArchiveExtension=0;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		if(exitstatus==0) temp=stralloc("FALSE");
		else if(exitstatus==1) temp=stralloc("TRUE");
		else if(exitstatus==2) temp=stralloc("FORCE FALSE");
		prterr("DIAG RUNREPORT Executing Report [%s] [%s] with Exit Status [%s] at line [%d] program [%s].",module,name,temp,line,file);
		if(temp!=NULL) Rfree(temp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
/* Reading RDAreport Definition */
	rpt=GETRDAreportEXE(module,name);
	if(rpt==NULL) 
	{
		errordesc=Rmalloc(RDAstrlen(name)+115);
		sprintf(errordesc,"Report [%s] was not found or can not otherwise be read.  Please refer to DEFINE REPORTS to verify the report exists.",name);
		ERRORDIALOG("REPORT NOT FOUND!",errordesc,NULL,(exitstatus!=2?TRUE:FALSE));
		if(errordesc!=NULL) Rfree(errordesc);
		return;
	}	
	if(RDA_NOGUI==1)
	{
		rpt->process_exceptions=FALSE;
		rpt->use_print_screen=FALSE;
		rpt->ask_b4_execute=FALSE;
		rpt->range_screen=FALSE;
		rpt->diag_breakable=2;
		rpt->no_empty_error=TRUE;
	}
	runrpt=RDArunrptNEWADV2(module,server_name,sortfile,APPmainLoop,exitstatus,quitfunc,quitfuncrsrc,quitfuncArgs,level);
	runrpt->ProcDataLine=PDLine;
	runrpt->print_range_criteria=FALSE;
	runrpt->range_desc=NULL;
	if(rpt->range_screen==TRUE && (rpt->display->device==0 || rpt->display->device==5) && rpt->display->dumprinter==TRUE)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

		gsv=RDAGenericSetupNew("UTILITIES","PRINT RANGE CRITERIA");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				runrpt->print_range_criteria=*gsv->value.string_value;
			} else {
				runrpt->print_range_criteria=FALSE;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","DEFAULT DUPLEX PRINTING");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				Default_Duplex_Printing=*gsv->value.string_value;
				if(rpt->display->dumprinter==TRUE && Default_Duplex_Printing==FALSE && rpt->display->dumprinter_type<4) rpt->display->dumprinter_type+=4;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);
	}
	if(XPERT_SETUP->ARCHIVE==FALSE)
	{
		runrpt->archive=FALSE;
	} else if((rpt->chain_report==TRUE && (rpt->display->device==0 || rpt->display->device==5) && rpt->display->dumprinter==TRUE) || (rpt->display->device>0 && rpt->display->device<3))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("ARCHIVE")+11);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif

		gsv=RDAGenericSetupNew("ARCHIVE","AUTOMATIC REPORT CAPTURE");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				runrpt->archive=*gsv->value.string_value;
			} else {
				runrpt->archive=FALSE;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);
	}
/* show range window based upon ranges of elements used in the report */
/* Test Field Level Access to fields associated with the file. */
	if(sortfile==NULL)
	{
		PreParseRDAreport(rpt);
		if(definefiles(rpt,runrpt))
		{
			ERRORDIALOG("FILE HIERARCHY ERROR!","Errors found, cannot define the file hierarchy.",NULL,(exitstatus!=2?TRUE:FALSE));
			prterr("Error in File Hierarchy.");
			if(!runrpt->APPmainLoop) 
			{
				runrpt->APPmainLoop=TRUE;
				return;
			}
/*lint -e746 */
			if(quitfunc!=NULL) quitfunc(quitfuncrsrc,quitfuncArgs);
/*lint +e746 */
			FreeRDAreport(rpt);
			FreeRDArunrpt(runrpt);
			if(runrpt->exit_status!=2)
			{
				ShutdownSubsystems();
			}
			return;
		}
/* Open Files Associated With Report */
		if(OPNReportfiles(runrpt)==(-1))
		{
			ERRORDIALOG("OPEN FILE ERROR!","Cannot open files associated with this report.",NULL,(exitstatus!=2?TRUE:FALSE));
			if(!runrpt->APPmainLoop) 
			{
				runrpt->APPmainLoop=TRUE;
				return;
			}
			prterr("Error cannot open files associated with this report.");
			CLSReportfiles(runrpt,FALSE);
/*lint -e746 */
			if(quitfunc!=NULL) quitfunc(quitfuncrsrc,quitfuncArgs);
/*lint +e746 */
			FreeRDAreport(rpt);
			FreeRDArunrpt(runrpt);
			if(runrpt->exit_status!=2) ShutdownSubsystems();
			return;
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen || diagrptgen_field)
		{
			SHOWrunfiles(runrpt);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
/* develop the sort file definition based on the fields in the file and the sort criteria */
/* create the sort file */
		if((runrpt->sortno=makesortfile(runrpt,rpt))==(-1))
		{
			memset(string_temparray,0,501);
			sprintf(string_temparray,"Sort file could not be created for report [%s][%s]. Check permissions and retry.  Call your installer if problems persist.",(runrpt->module!=NULL?runrpt->module:""),(rpt->name!=NULL?rpt->name:""));
			exit_cleanly(runrpt,rpt,TRUE,"CREATE FILE ERROR!",string_temparray,TRUE,1);
			return;
		}
/* Main Loop to Read Select and Write Data */
		if(rpt->range_screen && sharedrsrc!=NULL)
		{
			runrpt->rsrsrc=sharedrsrc;
		} else if(rpt->range_screen)
		{
			temp=Rmalloc(RDAstrlen(rpt->name)+14);
			sprintf(temp,"%s RANGE SCREEN",rpt->name);
			runrpt->rsrsrc=RDArsrcNEW(module,temp);
			for(x=0,filex=runrpt->files;x<runrpt->numfiles;++x,
				++filex)
			{
				if(filex->range)
				{
					file2rangersrc(filex->fileno,runrpt->rsrsrc);
/* Added for RDAScrolledLists Only */
					addDFincvir(runrpt->rsrsrc,MODULENAME(filex->fileno),FILENAME(filex->fileno),NULL,filex->fileno);
				}
			}
			runrpt->rsrsrc->SubFunc=(void (*)(...))ReportGenSubData;
			HoldReportx=HoldReportNew(runrpt,rpt);
			runrpt->rsrsrc->SubFuncArgs=HoldReportx;
			runrpt->rsrsrc->SetFunc=RDAreportSetFunc;
			runrpt->rsrsrc->SetFuncArgs=HoldReportx;
			if(rpt->virflds!=NULL)
			{
				for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
				{
					switch(v->range)
					{
						default:
						case 0: /* normal computational */
							break;
						case 1: /* range on virtual */
							virtual2rangersrc(v,runrpt->rsrsrc);
							break;
						case 2: /* default virtual */
							makefieldrsrc(runrpt->rsrsrc,v->name,v->type,v->len,TRUE);
							FINDRSCSETFUNC(runrpt->rsrsrc,v->name,ReportComputeSCRNcallback,HoldReportx);
							break;
						case 4: /* combo virtual */
							makefieldrsrc(runrpt->rsrsrc,v->name,v->type,v->len,TRUE);
							FINDRSCSETFUNC(runrpt->rsrsrc,v->name,ReportComputeSCRNcallback,HoldReportx);
							virtual2rangersrc(v,runrpt->rsrsrc);
							break;
					}
				}
			}
			if((rpt->display->device>=0 && rpt->display->device<=2) || rpt->display->device==5)
			{
				addstdrsrc(runrpt->rsrsrc,"RANGE ON PAGE NUMBER",BOOLNS,1,&rop,TRUE);
				ADVFINDRSCSETFUNC(runrpt->rsrsrc,"RANGE ON PAGE NUMBER",setrangesensitivecb,NULL,TRUE);
				addstdrsrc(runrpt->rsrsrc,"FROM PAGE NUMBER",LONGV,10,&fpage,TRUE);
				addstdrsrc(runrpt->rsrsrc,"TO PAGE NUMBER",LONGV,10,&tpage,TRUE);
			}
			if(rpt->use_print_screen==FALSE)
			{
				addstdrsrc(runrpt->rsrsrc,"PRINT RANGE CRITERIA",BOOLNS,1,&runrpt->print_range_criteria,TRUE);
			}
			addbtnrsrc(runrpt->rsrsrc,"DEFAULTS",TRUE,SaveDefaults,runrpt->rsrsrc);
			addbtnrsrc(runrpt->rsrsrc,"RUN REPORT",TRUE,executereport,HoldReportx);
			if(runrpt->exit_status==1)
			{
				addrfexrsrc(runrpt->rsrsrc,"QUIT",TRUE,quitreport1,HoldReportx);
			} else {
				addrfkwrsrc(runrpt->rsrsrc,"QUIT",TRUE,quitreport,HoldReportx);
			}
			addbtnrsrc(runrpt->rsrsrc,"RESET DEFAULTS",TRUE,ResetDefaults,HoldReportx);
			addbtnrsrc(runrpt->rsrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
			addbtnrsrc(runrpt->rsrsrc,"HELP",TRUE,screenhelp,NULL);
			addbtnrsrc(runrpt->rsrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
			for(x=0,filex=runrpt->files;x<runrpt->numfiles;++x,
				++filex)
			{
				if(filex->range) 
					setfile2rsrccallbacks(filex->fileno,runrpt->rsrsrc,FALSE);
			}
			ADV2GET_SCREEN_VIRTUALS(runrpt->rsrsrc,0,FALSE,TRUE,TRUE);	
			ADVSCRNvirtual2rsrc(runrpt->rsrsrc,ReportGenSubData,HoldReportx);
			if(rpt->process_exceptions==TRUE)
			{
				if(runrpt->procexcept_types!=NULL) freeapplib(runrpt->procexcept_types);
				runrpt->procexcept_types=APPlibNEW();
				addAPPlib(runrpt->procexcept_types,"Process Exceptions");
				addAPPlib(runrpt->procexcept_types,"Automatic");
				addlstrsrc(runrpt->rsrsrc,"PROCESS EXCEPTIONS TYPES",&runrpt->do_process_exceptions,TRUE,NULL,runrpt->procexcept_types->numlibs,&runrpt->procexcept_types->libs,NULL);
			}

			setvirtualrangecallbacks(rpt,runrpt->rsrsrc);
			DefaultScreens(runrpt->rsrsrc);
			ReadRDAScrolledLists(runrpt->rsrsrc);
			SetRangersrcsensitive(runrpt->rsrsrc);
			++runrpt->screen_count;
			ADVcomputeandupdateallSCRNvirtuals(runrpt->rsrsrc,ReportGenSubData,HoldReportx,TRUE);
			if(xmakescrn(runrpt->rsrsrc,TRUE,RPTGEN_GUIEVAL,(void *)HoldReportx,addoswidgets,RPTGEN_GUIEVALSTR,(void *)HoldReportx,ReportGenSubData,HoldReportx,NULL,__LINE__,__FILE__))
			{
				memset(string_temparray,0,501);
				sprintf(string_temparray,"The Make Screen function failed for the custom Range Screen [%s] for the report [%s][%s]. Check to see if the screen is available.  If it is, call your installer.",(temp!=NULL?temp:""),(runrpt->module!=NULL?runrpt->module:""),(rpt->name!=NULL?rpt->name:""));
				exit_cleanly(runrpt,rpt,TRUE,"MAKE SCREEN FAILED!",string_temparray,TRUE,1);
				return;
			}
			setvirtualrangecallbacks(rpt,runrpt->rsrsrc);
			SetRangersrcsensitive(runrpt->rsrsrc);
			if(temp!=NULL) Rfree(temp);
			runrpt->APPmainLoop=TRUE;
			return;
		}
	} else {
/* if same sort file then do this */
		if(rpt->range_screen && sharedrsrc!=NULL)
			runrpt->rsrsrc=sharedrsrc;
		runrpt->sortfile=stralloc(sortfile);
		if(server_name!=NULL)
		{
			runrpt->sortserver=stralloc(server_name);
		} else runrpt->sortserver=NULL;
/* open the sort file */
		runrpt->sortno=OPNNRD("RPTGEN",runrpt->sortfile);
		if(runrpt->sortno==(-1))
		{
			memset(string_temparray,0,501);
			sprintf(string_temparray,"The sort file would not open properly for the report [%s][%s].  Please verify that the previous report that this report is chained to did not remove the sort file.  If it did not please call your installer.",(runrpt->module!=NULL?runrpt->module:""),(rpt->name!=NULL?rpt->name:""));
			exit_cleanly(runrpt,rpt,TRUE,"OPEN FILE FAILURE!",string_temparray,TRUE,1);
			return;
		}
		PreParseRDAreport(rpt);
	}
	h=HoldReportNew(runrpt,rpt);
	executereport(runrpt->rsrsrc,h);
#ifdef USE_CPU_PROFILER
	//ProfilerStop();
#endif /* USE_CPU_PROFILER */
	return;
}
void quitreport(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *runrpt=NULL;
	RDAreport *rpt=NULL;

	if(h!=NULL)
	{
		runrpt=h->rrpt;
		rpt=h->rpt;
		if(runrpt->archive==TRUE)
		{
			memset(stemp,0,101);
			if(rpt->display->device==0 || rpt->display->device==5) 
			{
				sprintf(stemp,"%d.pdf",RGETPID());
			} else sprintf(stemp,"%d.txt",RGETPID());
			unlink(stemp);
		}
		if(runrpt->askb4exec==FALSE)
		{
			Rfree(h);
			exit_cleanly(runrpt,rpt,TRUE,NULL,NULL,TRUE,1);
		} else {
			Rfree(h);
			exit_cleanly(runrpt,rpt,TRUE,NULL,NULL,TRUE,1);
		}
	}
}
void quitreport1(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;

	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rrpt->askb4exec==FALSE)
	{
		exit_cleanly(rrpt,rpt,TRUE,NULL,NULL,TRUE,1);
	} else {
		exit_cleanly(rrpt,rpt,TRUE,NULL,NULL,TRUE,1);
	}
}
void quitaskrsrc1(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	
	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rrpt->archive==TRUE)
	{
		memset(stemp,0,101);
		if(rpt->display->device==0 || rpt->display->device==5)
		{
			 sprintf(stemp,"%d.pdf",RGETPID());
		} else sprintf(stemp,"%d.txt",RGETPID());
		unlink(stemp);
	}
	if(rrpt->askb4exec==FALSE)
	{
		exit_cleanly(rrpt,rpt,TRUE,NULL,NULL,TRUE,1);
	} else {
		exit_cleanly(rrpt,rpt,TRUE,NULL,NULL,TRUE,1);
	}
}
void quitaskrsrc(RDArsrc *parent,HoldReport *h)
{
	quitreport(parent,h);
}
void quitprtrsrc1(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	
	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rrpt->askb4exec==FALSE)
	{
		exit_cleanly(rrpt,rpt,TRUE,NULL,NULL,TRUE,1);
	} else {
		exit_cleanly(rrpt,rpt,TRUE,NULL,NULL,TRUE,1);
	}
} 
void quitprtrsrc(RDArsrc *parent,HoldReport *h)
{
	quitreport(parent,h);
}
void setdevicevariables(RDArsrc *parent,HoldReport *h)
{
	int y;
	char *outdev=NULL;
	char outdeveval=FALSE;
	RDApdev *pdev;
	RDApvar *pvar,*pv;
	RDAdisplay *d;
	char *temp=NULL;
	RDArunrpt *rrpt;
	RDAreport *rpt;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG setdevicevariables.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rpt==NULL || rrpt==NULL) return;
	if(FINDRSCGETCHAR(rrpt->prtrsrc,"DEVICE NAME EVAL",&outdeveval)) return;
	if(FINDRSCGETSTRING(rrpt->prtrsrc,"DEVICE NAME",&outdev)) return;
	if(outdeveval==FALSE)
	{
		temp=stralloc(outdev);
	} else {
		temp=EVALUATEstr(outdev,ReportGenSubData1,h);
	}
	if(!strncmp(temp,"AUTOARCHIVE",11) && (XPERT_SETUP->ARCHIVE==TRUE))
	{
		ArchiveSource=6;
#ifdef WIN32
		pdev=RDApdevNEW("rdaout.exe");
#endif
#ifndef WIN32
		pdev=RDApdevNEW("rdaout.lnx");
#endif
	} else {
		pdev=RDApdevNEW(temp);
	}
	d=rpt->display;
	if(d->pvars!=NULL)
	{
		for(y=0,pvar=d->pvars;y<d->num;++y,++pvar) 
		{
			if(pvar->name!=NULL) Rfree(pvar->name);
			if(pvar->command!=NULL) Rfree(pvar->command);
		}
		Rfree(d->pvars);
		d->pvars=NULL;
		d->num=0;
	}
	if(!getpdevbin(pdev))
	{
		d->extra_formfeed=pdev->formfeed;
		d->num=pdev->num;
		d->pvars=Rmalloc(d->num*sizeof(RDApvar));
		for(y=0,pvar=d->pvars,pv=pdev->pvar;y<d->num;++y,++pvar,++pv) 
		{
			pvar->name=stralloc(pv->name);
			pvar->command=stralloc(pv->command);
			pvar->chgsize=pv->chgsize;
			pvar->type=pv->type;
			pvar->pitch=pv->pitch;
			pvar->points=pv->points;
		}
	}
	free_pdev(pdev);
	if(outdev!=NULL) Rfree(outdev);
	if(temp!=NULL) Rfree(temp);
}
void skip_alignment(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;

	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rrpt->alignrsrc!=NULL) 
	{
		killwindow(rrpt->alignrsrc);
		free_rsrc(rrpt->alignrsrc);
		rrpt->alignrsrc=NULL;
	}
	ReportBackEnd(rrpt,rpt);
}
void quit_alignment(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	RDAdisplay *d;
	int wcount=1;
	
	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rrpt->alignrsrc!=NULL) 
	{
		killwindow(rrpt->alignrsrc);
		free_rsrc(rrpt->alignrsrc);
		rrpt->alignrsrc=NULL;
	}
	ReportBackEnd(h->rrpt,h->rpt);
	if(h!=NULL) Rfree(h);
	if(rpt->range_screen && IsScreenDisplayed(rrpt->rsrsrc)) ++wcount;
	d=rpt->display;
	if((d->device==0 || d->device==5) && rpt->use_print_screen && IsScreenDisplayed(rrpt->prtrsrc)) ++wcount;
	if(rrpt->screen_count==wcount) 
	{
		quitreport(parent,h);
		if(rrpt->exit_status!=2)
		{
			ShutdownSubsystems();
		}
		return;
	}
}
static void abort_alignment(RDArsrc *parent,HoldReport *h)
{
	quitreport(parent,h);
}
static void abort_alignment1(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	int wcount=1;
	RDAdisplay *d;
	
	rpt=h->rpt;
	rrpt=h->rrpt;
	d=rpt->display;
	if(rpt->range_screen && IsScreenDisplayed(rrpt->rsrsrc)) ++wcount;
	if((d->device==0 || d->device==5) && rpt->use_print_screen && IsScreenDisplayed(rrpt->prtrsrc)) ++wcount;
	if(rrpt->screen_count==wcount) 
	{
		quitreport(parent,h);
		if(rrpt->exit_status!=2)
		{
			ShutdownSubsystems();
		}
		return;
	}
}
void alignment_test(HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *r;
	char *temp=NULL;

	if(h!=NULL)
	{
		r=h->rpt;
		rrpt=h->rrpt;
	} else return;
	ZERNRD(rrpt->sortno);
	rrpt->alignrsrc=RDArsrcNEW("RPTGEN","RUN REPORT ALIGNMENT TEST");
	addbtnrsrc(rrpt->alignrsrc,"PRINT",TRUE,print_test_pages,h);
	addbtnrsrc(rrpt->alignrsrc,"RUN REPORT",TRUE,skip_alignment,h);
	addstdrsrc(rrpt->alignrsrc,"REPORT NAME",VARIABLETEXT,0,r->name,FALSE);
	if(rrpt->exit_status==1)
	{
		addrfexrsrc(rrpt->alignrsrc,"QUIT",TRUE,abort_alignment1,h);
	} else {
		addrfkwrsrc(rrpt->alignrsrc,"QUIT",TRUE,abort_alignment,h);
	}
	addbtnrsrc(rrpt->alignrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(rrpt->alignrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	++rrpt->screen_count;
	if(ADVmakescrn(rrpt->alignrsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen RUN REPORT ALIGNMENT TEST. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_alignment(rrpt->alignrsrc,h);
	} else {
		temp=Rmalloc(RDAstrlen(r->name)+17);
		sprintf(temp,"%s ALIGNMENT TEST",r->name);
		SETTITLE(rrpt->alignrsrc,temp);
		if(temp!=NULL) Rfree(temp);
	}
}
void okprtrsrc(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	RDAdisplay *d;
	char print_alignment=FALSE,*temp=NULL;
	int x=0;

	rrpt=h->rrpt;
	rpt=h->rpt;
	if(rrpt->errorlist->numlibs>0 && strncmp(rrpt->errorlist->libs[0],"No Errors",9))
	{
		if(!isEMPTY(rpt->title))
		{
			temp=Rmalloc((RDAstrlen(rpt->name)+RDAstrlen(rpt->title)+10));
			sprintf(temp,"%s -> %s",rpt->name,rpt->title);
		} else temp=stralloc(rpt->name);
		APPlib2SupportRequest(rrpt->module,temp,rrpt->errorlist);
		if(temp!=NULL) Rfree(temp);
	} else {
		freeapplib(rrpt->errorlist);
		rrpt->errorlist=APPlibNEW();
	}
	readallwidgets(rrpt->prtrsrc);
	d=rpt->display;
	if(d==NULL)
	{
		d=Rmalloc(sizeof(RDAdisplay));
		d->device=0;
		d->page_length=11;
		d->page_width=8.5;
		d->body_count=0;
		d->body_count_type=FALSE;
		d->test_pages=1;
		d->set_lpp=NULL;
		d->set_lpp_eval=FALSE;
		d->starting_page=1;
		d->pvars=NULL;
		d->num=0;
		rpt->display=d;
	}
	FINDRSCGETDOUBLE(rrpt->prtrsrc,"PAGE WIDTH",&d->page_width);
	FINDRSCGETDOUBLE(rrpt->prtrsrc,"PAGE LENGTH",&d->page_length);
	FINDRSCGETSHORT(rrpt->prtrsrc,"BODY COUNT",&d->body_count);
	FINDRSCGETCHAR(rrpt->prtrsrc,"BODY COUNT TYPE",&d->body_count_type);
	FINDRSCGETSHORT(rrpt->prtrsrc,"STARTING PAGE NO",&d->starting_page);
	FINDRSCGETCHAR(rrpt->prtrsrc,"ALIGNMENT TEST",&print_alignment);
	if(rpt->display->dumprinter==TRUE)
	{
		FINDRSCGETINT(rrpt->prtrsrc,"SIMPLE PRINTER TYPES",&x);
		rpt->display->dumprinter_type=x;
	}
	d->date_order=print_alignment;
	FINDRSCGETSHORT(rrpt->prtrsrc,"NUMBER TEST PAGES",&d->test_pages);
	FINDRSCGETSTRING(rrpt->prtrsrc,"DEVICE NAME",&d->set_lpp);
	FINDRSCGETCHAR(rrpt->prtrsrc,"DEVICE NAME EVAL",&d->set_lpp_eval);
	FINDRSCGETCHAR(rrpt->prtrsrc,"PRINT RANGE CRITERIA",&rrpt->print_range_criteria);
	setdevicevariables(rrpt->prtrsrc,h);
	if(rrpt->prtrsrc!=NULL) 
	{
		killwindow(rrpt->prtrsrc);
		free_rsrc(rrpt->prtrsrc);
	}
	rrpt->prtrsrc=NULL;
	if(d->date_order)
	{
		alignment_test(h);
	} else {
		ReportBackEnd(h->rrpt,h->rpt);
		if(h!=NULL) Rfree(h);
	}
}
void aaokprtrsrc(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	RDAdisplay *d;

	rrpt=h->rrpt;
	rpt=h->rpt;
	d=rpt->display;
	if(d==NULL)
	{
		d=Rmalloc(sizeof(RDAdisplay));
		d->device=0;
		d->page_length=11;
		d->page_width=8.5;
		d->body_count=0;
		d->body_count_type=FALSE;
		d->test_pages=1;
		d->set_lpp=stralloc("AUTOARCHIVE");
		d->set_lpp_eval=FALSE;
		d->starting_page=1;
		d->pvars=NULL;
		d->num=0;
		rpt->display=d;
	}
	FINDRSCSETSTRING(parent,"DEVICE NAME","AUTOARCHIVE");
	updatersrc(parent,"DEVICE NAME");
	okprtrsrc(parent,h);
}
void okprtrsrc1(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	int wcount=1;
	
	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rpt->range_screen && IsScreenDisplayed(rrpt->rsrsrc)) ++wcount;
	okprtrsrc(parent,h);
	if(rrpt->screen_count==wcount) 
	{
		if(rrpt->exit_status!=2)
		{
			ShutdownSubsystems();
		}
		return;
	}
}
void executereport(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *r;
	RDAvirtual *v=NULL;
	int x=0,y=0;
	RDArmem *mem=NULL;

/* If output goes to device, show window to ask for device information here. 
   This will allow then to change the device info at runtime.*/
	rrpt=h->rrpt;
	r=h->rpt;
	if(rrpt!=NULL)
	{
		if(rrpt->rsrsrc!=NULL)
		{
			if(IsScreenDisplayed(rrpt->rsrsrc) && rrpt->rsrsrc->lists!=NULL)
			{
				ReadRDAScrolledLists(rrpt->rsrsrc);
			}
			if(r->virflds!=NULL)
			{
				for(x=0,v=r->virflds;x<r->numvirtuals;++x,++v)
				{
					if(v->range==4)
					{
						memset(stemp,0,101);
						sprintf(stemp,"RANGE ON [%s]",v->name);
						y=FINDRSC(rrpt->rsrsrc,stemp);
						if(y!=(-1))
						{
							mem=rrpt->rsrsrc->rscs+y;
							if(mem->sensitive==TRUE && mem->user_sensitive==TRUE)
							{
								v->range=1;
							} else {
								v->range=2;
								if(v->expression!=NULL) Rfree(v->expression);
							}
						}
					}
				}
			}
		}
	}
	if(rrpt->numfiles>0)
	{
		if(READRecords(rrpt,r))
		{
/* USER ABORTED */
			memset(string_temparray,0,501);
			sprintf(string_temparray,"The User has aborted the report [%s][%s] before all records were selected.",(rrpt->module!=NULL?rrpt->module:""),(r->name!=NULL?r->name:""));
			exit_cleanly(rrpt,r,TRUE,"READRecords ABORTED!",string_temparray,TRUE,1);
			return;
		}
/* Close Files Associated With Report */
		if(rrpt->record_count<1)
		{		
/* NO RECORDS SELECTED */
			memset(string_temparray,0,501);
			sprintf(string_temparray,"No Records were selected for the report [%s][%s] from the given criteria.  Please reselect and/or change criteria.",(rrpt->module!=NULL?rrpt->module:""),(r->name!=NULL?r->name:""));
			exit_cleanly(rrpt,r,TRUE,"NO RECORDS SELECTED!",string_temparray,TRUE,0);
			if(h!=NULL) Rfree(h);
			return;
		}
	}
	if(rrpt->RScreen!=NULL) 
	{
		rrpt->range_desc=getrangedesc(rrpt->RScreen);
		FreeRangeScreen(rrpt->RScreen);
		if(r->use_print_screen==FALSE)
		{
			FINDRSCGETCHAR(rrpt->rsrsrc,"PRINT RANGE CRITERIA",&rrpt->print_range_criteria);
		}
	}
	rrpt->RScreen=NULL;
	if(r->process_exceptions && rrpt->do_process_exceptions==0)
	{
		if(r->MTNMSTR==NULL)
		{
			process_exceptions(rrpt,r);
			if(h!=NULL) Rfree(h);
		} else {
			process_exceptions2(h);
		}
	} else {
		runprtscreen(h);
	}
}
void RPTSelectFuncDeviceName(RDArsrc *r,RDArsrc *prtrsrc)
{
	int x=0;
	double l=0;
	RDApdev *pdev=NULL;

	FINDRSCGETINT(r,"OUTPUT DEVICE LIST",&x);
	if(x<=MYout_devs->numlibs)
	{
		if(!RDAstrcmp(MYout_devs->libs[x],"AUTOARCHIVE")) 
		{
			FINDRSCSETCHAR(prtrsrc,"PRINT RANGE CRITERIA",TRUE);
			updatersrc(prtrsrc,"PRINT RANGE CRITERIA");
		}
		FINDRSCSETSTRING(prtrsrc,"DEVICE NAME",MYout_devs->libs[x]);
		updatersrc(prtrsrc,"DEVICE NAME");
		readwidget(prtrsrc,"PAGE LENGTH");
		FINDRSCGETDOUBLE(prtrsrc,"PAGE LENGTH",&l);
		if((!strncmp(MYout_devs->libs[x],"AUTOARCHIVE",11)) && (XPERT_SETUP->ARCHIVE==TRUE))
		{
			ArchiveSource=6;
#ifdef WIN32
			pdev=RDApdevNEW("rdaout.exe");
#endif
#ifndef WIN32
			pdev=RDApdevNEW("rdaout.lnx");
#endif
		} else pdev=RDApdevNEW(MYout_devs->libs[x]);
		if(!getpdevbin(pdev))
		{
			if(pdev->line_58==TRUE && l>9.9)
			{
				l=9.9;
				FINDRSCSETDOUBLE(prtrsrc,"PAGE LENGTH",l);
				updatersrc(prtrsrc,"PAGE LENGTH");
			} else if(pdev->page_length!=0 && l>9.9)
			{
				l=pdev->page_length;
				FINDRSCSETDOUBLE(prtrsrc,"PAGE LENGTH",l);
				updatersrc(prtrsrc,"PAGE LENGTH");
			}
		}
		free_pdev(pdev);
	}
	quitSelectDevice(r);
}
void runprtscreen(HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *r;
	RDAdisplay *d;
	char *prtx=NULL,print_alignment=FALSE,*temp=NULL;
	int errorcount=0,x=0,y,z;
	RDAaccum *a=NULL;
	NRDfield *field;
	RDAvirtual *v;
	RDApdev *pdev=NULL;
	RDApvar *pvar,*pv;

	if(h!=NULL)
	{
		r=h->rpt;
		rrpt=h->rrpt;
	} else return;
	d=r->display;
/* Initialize Accumulators */
	if(r->accum!=NULL)
	{
		for(y=0,a=r->accum;y<r->numaccums;++y,++a)
		{
			field=FLDNRD(rrpt->sortno,a->accname);
			if(field!=NULL)
			{
				makeaccum(r,a,field->type,field->len);
			} else {
				temp=stripmodulename(a->accname);
				for(z=0,v=r->virflds;z<r->numvirtuals;++z,++v)
				{
					if(!RDAstrcmp(v->name,temp)) break;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				if(z<r->numvirtuals)
				{
					makeaccum(r,a,v->type,v->len);
				}
			}
		}
	}
	if(d==NULL) 
	{
		d=Rmalloc(sizeof(RDAdisplay));
		d->device=0;
		d->page_length=11;
		d->page_width=8.5;
		d->body_count=0;
		d->body_count_type=FALSE;
		d->date_order=FALSE;
		d->test_pages=1;
		d->set_lpp=NULL;
		d->set_lpp_eval=FALSE;
		d->starting_page=1;
		d->pvars=NULL;
		d->num=0;
		r->display=d;
	}
	switch(d->device)
	{
		default:
		case 0: /* printer */
		case 5: /* popen */
			if(d->set_lpp_eval==TRUE)
			{
				temp=EVALUATEstr(d->set_lpp,ReportGenSubData1,h);
				if(!isEMPTY(temp))
				{
					if(d->set_lpp!=NULL) Rfree(d->set_lpp);
					d->set_lpp=stralloc(temp);
					d->set_lpp_eval=FALSE;
				} else { /* run to default printer */
					prtx=DefaultPrinter();
					if(prtx!=NULL)
					{
						if(d->set_lpp!=NULL) Rfree(d->set_lpp);
						d->set_lpp=prtx;
						d->set_lpp_eval=FALSE;
					}
				}
				if(temp!=NULL) Rfree(temp);
			} else if(RDAstrcmp(d->set_lpp,"AUTOARCHIVE"))
			{
				prtx=DefaultPrinter();
				if(prtx!=NULL)
				{
					if(d->set_lpp!=NULL) Rfree(d->set_lpp);
					d->set_lpp=prtx;
					d->set_lpp_eval=FALSE;
				}
			} else if(!RDAstrcmp(d->set_lpp,"AUTOARCHIVE"))
			{
				rrpt->archive=TRUE;
			}
			if((!strncmp(d->set_lpp,"AUTOARCHIVE",11)) && (XPERT_SETUP->ARCHIVE==TRUE))
			{
				ArchiveSource=6;
#ifdef WIN32
				sprintf(stemp,"rdaout.exe");
#endif
#ifndef WIN32
				sprintf(stemp,"rdaout.lnx");
#endif
				pdev=RDApdevNEW(stemp);
			} else pdev=RDApdevNEW(d->set_lpp);
			if(!getpdevbin(pdev))
			{
				if(pdev->line_58==TRUE && r->display->page_length>9.9 && r->display->dumprinter==TRUE)
				{
					r->display->page_length=9.9;
				} else if(pdev->page_length!=0 && r->display->page_length>9.9 && r->display->dumprinter==TRUE)
				{
					r->display->page_length=pdev->page_length;
				}
/* needs to set the device variables HERE */
				if(d->pvars!=NULL)
				{
					for(y=0,pvar=d->pvars;y<d->num;++y,++pvar) 
					{
						if(pvar->name!=NULL) Rfree(pvar->name);
						if(pvar->command!=NULL) Rfree(pvar->command);
					}
					Rfree(d->pvars);
					d->pvars=NULL;
					d->num=0;
				}
				d->extra_formfeed=pdev->formfeed;
				d->num=pdev->num;
				d->pvars=Rmalloc(d->num*sizeof(RDApvar));
				for(y=0,pvar=d->pvars,pv=pdev->pvar;y<d->num;++y,++pvar,++pv) 
				{
					pvar->name=stralloc(pv->name);
					pvar->command=stralloc(pv->command);
					pvar->chgsize=pv->chgsize;
					pvar->type=pv->type;
					pvar->pitch=pv->pitch;
					pvar->points=pv->points;
				}
			}
			free_pdev(pdev);
			if(r->use_print_screen)
			{
				rrpt->prtrsrc=RDArsrcNEW("RPTGEN","RUN REPORT DEVICE WINDOW");
				addstdrsrc(rrpt->prtrsrc,"REPORT NAME",VARIABLETEXT,0,r->name,FALSE);
				addbtnrsrc(rrpt->prtrsrc,"LOAD DEVICE NAME",TRUE,(rrpt->archive ? LoadDeviceWindow:LoadDeviceWindowAutoArchive),RPTSelectFuncDeviceName);
				addstdrsrc(rrpt->prtrsrc,"DEVICE NAME",VARIABLETEXT,0,d->set_lpp,TRUE);
				addstdrsrc(rrpt->prtrsrc,"DEVICE NAME EVAL",BOOLNS,3,&d->set_lpp_eval,TRUE);
				addstdrsrc(rrpt->prtrsrc,"PAGE WIDTH",DECIMALV,8,&d->page_width,TRUE);
				addstdrsrc(rrpt->prtrsrc,"PAGE LENGTH",DECIMALV,8,&d->page_length,TRUE);
				addstdrsrc(rrpt->prtrsrc,"BODY COUNT",SHORTV,3,&d->body_count,TRUE);
				addstdrsrc(rrpt->prtrsrc,"BODY COUNT TYPE",BOOLNS,3,&d->body_count_type,TRUE);
				print_alignment=d->date_order;
				addstdrsrc(rrpt->prtrsrc,"ALIGNMENT TEST",BOOLNS,1,&print_alignment,TRUE);
				addstdrsrc(rrpt->prtrsrc,"PRINT RANGE CRITERIA",BOOLNS,1,&rrpt->print_range_criteria,TRUE);
				if(r->range_screen==TRUE && (r->display->device==0 || r->display->device==5))
				{
				} else {
					FINDRSCSETSENSITIVE(rrpt->prtrsrc,"PRINT RANGE CRITERIA",FALSE);
				}
				addstdrsrc(rrpt->prtrsrc,"NUMBER TEST PAGES",SHORTV,3,&d->test_pages,TRUE);
				addstdrsrc(rrpt->prtrsrc,"STARTING PAGE NO",SHORTV,3,&d->starting_page,TRUE);
/* SIMPLE PRINTER */
				rrpt->dumprttypes=APPlibNEW();
				addAPPlib(rrpt->dumprttypes,"NORMAL TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"COMPRESSED TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE NORMAL TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE COMPRESSED TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"NORMAL TEXT");
				addAPPlib(rrpt->dumprttypes,"COMPRESSED TEXT");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE NORMAL TEXT");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE COMPRESSED TEXT");
				y=(int)r->display->dumprinter_type;
				addlstrsrc(rrpt->prtrsrc,"SIMPLE PRINTER TYPES",&y,TRUE,NULL,rrpt->dumprttypes->numlibs,&rrpt->dumprttypes->libs,NULL);
				FINDRSCSETSENSITIVE(rrpt->prtrsrc,"SIMPLE PRINTER TYPES",(r->display->dumprinter ? TRUE:FALSE));
				FINDRSCSETEDITABLE(rrpt->prtrsrc,"SIMPLE PRINTER TYPES",(r->display->dumprinter ? TRUE:FALSE));
				errorcount=rrpt->errorlist->numlibs;
				if(rrpt->errorlist->numlibs<1)
				{
					addAPPlib(rrpt->errorlist,"No Errors Encountered");
				}
				addstdrsrc(rrpt->prtrsrc,"ERROR COUNT",LONGV,10,&errorcount,FALSE);
				x=0;
				addlstrsrc(rrpt->prtrsrc,"ERROR LIST",&x,TRUE,NULL,rrpt->errorlist->numlibs,&rrpt->errorlist->libs,NULL);
				addbtnrsrc(rrpt->prtrsrc,"PRINT ERROR LIST",TRUE,print_list,h);
				addbtnrsrc(rrpt->prtrsrc,"CSV",TRUE,dump_csv,h);
				addbtnrsrc(rrpt->prtrsrc,"OK DEVICE",TRUE,okprtrsrc,h);
				addbtnrsrc(rrpt->prtrsrc,"AUTO ARCHIVE",TRUE,aaokprtrsrc,h);
				if(rrpt->archive==FALSE)
				{
					FINDRSCSETEDITABLE(rrpt->prtrsrc,"AUTO ARCHIVE",TRUE);
					FINDRSCSETSENSITIVE(rrpt->prtrsrc,"AUTO ARCHIVE",TRUE);
				} else {
					FINDRSCSETEDITABLE(rrpt->prtrsrc,"AUTO ARCHIVE",FALSE);
					FINDRSCSETSENSITIVE(rrpt->prtrsrc,"AUTO ARCHIVE",FALSE);
				}
				if(rrpt->exit_status==1)
				{
					addrfexrsrc(rrpt->prtrsrc,"QUIT",TRUE,quitprtrsrc1,h);
				} else {
					addrfkwrsrc(rrpt->prtrsrc,"QUIT",TRUE,quitprtrsrc,h);
				}
				addbtnrsrc(rrpt->prtrsrc,"HELP",TRUE,screenhelp,NULL);
				addbtnrsrc(rrpt->prtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
				++rrpt->screen_count;
				FINDRSCSETINPUTFOCUS(rrpt->prtrsrc,"OK DEVICE");
				if(ADVmakescrn(rrpt->prtrsrc,TRUE))
				{
					ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen RUN REPORT DEVICE WINDOW. Check to see the screen is available. If it is, call your installer.",NULL,TRUE);
					quitprtrsrc(rrpt->prtrsrc,h);
					prterr("Error can't make screen RUN REPORT DEVICE WINDOW.");
					if(!rrpt->APPmainLoop) 
					{
						FreeRDAreport(r);
						FreeRDArunrpt(rrpt);
						return;
					}
					FreeRDAreport(r);
					FreeRDArunrpt(rrpt);
					return;
				} else {
					temp=Rmalloc(RDAstrlen(r->name)+16);
					sprintf(temp,"%s DEVICE WINDOW",r->name);
					SETTITLE(rrpt->prtrsrc,temp);
					if(temp!=NULL) Rfree(temp);
				}
				if(!rrpt->APPmainLoop) 
				{
					rrpt->APPmainLoop=TRUE;
					return;
				}
			} else {
				ReportBackEnd(h->rrpt,h->rpt);
				if(h!=NULL) Rfree(h);
			}
			break;
		case 1: /* ascii file */
		case 2: /* append ascii file */
			if(d->set_lpp_eval==TRUE)
			{
				temp=EVALUATEstr(d->set_lpp,ReportGenSubData1,h);
				if(!isEMPTY(temp))
				{
					if(d->set_lpp!=NULL) Rfree(d->set_lpp);
					d->set_lpp=stralloc(temp);
					d->set_lpp_eval=FALSE;
					if((!strncmp(d->set_lpp,"AUTOARCHIVE",11)) && (XPERT_SETUP->ARCHIVE==TRUE))
					{
						ArchiveSource=6;
#ifdef WIN32
						sprintf(stemp,"rdaout.exe");
#endif
#ifndef WIN32
						sprintf(stemp,"rdaout.lnx");
#endif
						pdev=RDApdevNEW(stemp);
					} else pdev=RDApdevNEW(d->set_lpp);
					if(!getpdevbin(pdev))
					{
						if(pdev->line_58==TRUE && r->display->dumprinter==TRUE)
						{
							r->display->page_length=9.9;
						}
/* needs to set the device variables HERE */
						if(d->pvars!=NULL)
						{
							for(y=0,pvar=d->pvars;y<d->num;++y,++pvar) 
							{
								if(pvar->name!=NULL) Rfree(pvar->name);
								if(pvar->command!=NULL) Rfree(pvar->command);
							}
							Rfree(d->pvars);
							d->pvars=NULL;
							d->num=0;
						}
						d->extra_formfeed=pdev->formfeed;
						d->num=pdev->num;
						d->pvars=Rmalloc(d->num*sizeof(RDApvar));
						for(y=0,pvar=d->pvars,pv=pdev->pvar;y<d->num;++y,++pvar,++pv) 
						{
							pvar->name=stralloc(pv->name);
							pvar->command=stralloc(pv->command);
							pvar->chgsize=pv->chgsize;
							pvar->type=pv->type;
							pvar->pitch=pv->pitch;
							pvar->points=pv->points;
						}
					}
					free_pdev(pdev);
				}
				if(temp!=NULL) Rfree(temp);
			}
/* add this back if setting of device variables for ASCII's is needed 
			if(r->use_print_screen)
			{
				rrpt->prtrsrc=RDArsrcNEW("RPTGEN","RUN REPORT DEVICE WINDOW");
				addstdrsrc(rrpt->prtrsrc,"REPORT NAME",VARIABLETEXT,0,r->name,FALSE);
				addstdrsrc(rrpt->prtrsrc,"DEVICE NAME",VARIABLETEXT,0,d->set_lpp,TRUE);
				addstdrsrc(rrpt->prtrsrc,"DEVICE NAME EVAL",BOOLNS,3,&d->set_lpp_eval,TRUE);
				addstdrsrc(rrpt->prtrsrc,"PAGE WIDTH",DECIMALV,8,&d->page_width,TRUE);
				addstdrsrc(rrpt->prtrsrc,"PAGE LENGTH",DECIMALV,8,&d->page_length,TRUE);
				addstdrsrc(rrpt->prtrsrc,"BODY COUNT",SHORTV,3,&d->body_count,TRUE);
				addstdrsrc(rrpt->prtrsrc,"BODY COUNT TYPE",BOOLNS,3,&d->body_count_type,TRUE);
				print_alignment=d->date_order;
				addstdrsrc(rrpt->prtrsrc,"ALIGNMENT TEST",BOOLNS,1,&print_alignment,TRUE);
				addstdrsrc(rrpt->prtrsrc,"NUMBER TEST PAGES",SHORTV,3,&d->test_pages,TRUE);
				addstdrsrc(rrpt->prtrsrc,"STARTING PAGE NO",SHORTV,3,&d->starting_page,TRUE);
				rrpt->dumprttypes=APPlibNEW();
				addAPPlib(rrpt->dumprttypes,"NORMAL TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"COMPRESSED TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE NORMAL TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE COMPRESSED TEXT (DUPLEX)");
				addAPPlib(rrpt->dumprttypes,"NORMAL TEXT");
				addAPPlib(rrpt->dumprttypes,"COMPRESSED TEXT");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE NORMAL TEXT");
				addAPPlib(rrpt->dumprttypes,"LANDSCAPE COMPRESSED TEXT");
				y=(int)r->display->dumprinter_type;
				addlstrsrc(rrpt->prtrsrc,"SIMPLE PRINTER TYPES",&y,TRUE,NULL,rrpt->dumprttypes->numlibs,&rrpt->dumprttypes->libs,NULL);
				FINDRSCSETSENSITIVE(rrpt->prtrsrc,"SIMPLE PRINTER TYPES",(r->display->dumprinter ? TRUE:FALSE));
				FINDRSCSETEDITABLE(rrpt->prtrsrc,"SIMPLE PRINTER TYPES",(r->display->dumprinter ? TRUE:FALSE));
				errorcount=rrpt->errorlist->numlibs;
				if(rrpt->errorlist->numlibs<1)
				{
					addAPPlib(rrpt->errorlist,"No Errors Encountered");
				}
				addstdrsrc(rrpt->prtrsrc,"ERROR COUNT",LONGV,10,&errorcount,FALSE);
				x=0;
				addlstrsrc(rrpt->prtrsrc,"ERROR LIST",&x,TRUE,NULL,rrpt->errorlist->numlibs,&rrpt->errorlist->libs,NULL);
				addbtnrsrc(rrpt->prtrsrc,"PRINT ERROR LIST",TRUE,print_list,h);
				addrfkwrsrc(rrpt->prtrsrc,"OK DEVICE",TRUE,okprtrsrc,h);
				if(rrpt->exit_status==1)
				{
					addrfexrsrc(rrpt->prtrsrc,"QUIT",TRUE,quitprtrsrc1,h);
				} else {
					addrfkwrsrc(rrpt->prtrsrc,"QUIT",TRUE,quitprtrsrc,h);
				}
				addbtnrsrc(rrpt->prtrsrc,"HELP",TRUE,screenhelp,NULL);
				addbtnrsrc(rrpt->prtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
				++rrpt->screen_count;
				FINDRSCSETINPUTFOCUS(rrpt->prtrsrc,"OK DEVICE");
				if(ADVmakescrn(rrpt->prtrsrc,TRUE))
				{
					ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen RUN REPORT DEVICE WINDOW. Check to see the screen is available. If it is, call your installer.",NULL,TRUE);
					quitprtrsrc(rrpt->prtrsrc,h);
					prterr("Error can't make screen RUN REPORT DEVICE WINDOW.");
					if(!rrpt->APPmainLoop) 
					{
						FreeRDAreport(r);
						FreeRDArunrpt(rrpt);
						return;
					}
					FreeRDAreport(r);
					FreeRDArunrpt(rrpt);
					return;
				} else {
					temp=Rmalloc(RDAstrlen(r->name)+16);
					sprintf(temp,"%s DEVICE WINDOW",r->name);
					SETTITLE(rrpt->prtrsrc,temp);
					if(temp!=NULL) Rfree(temp);
				}
				if(!rrpt->APPmainLoop) 
				{
					rrpt->APPmainLoop=TRUE;
					return;
				}
			} else {
				ReportBackEnd(h->rrpt,h->rpt);
				if(h!=NULL) Rfree(h);
			}
*/
			ReportBackEnd(h->rrpt,h->rpt);
			if(h!=NULL) Rfree(h);
			break;
		case 3: /* datafile */
		case 4: /* update datafile */
			ReportBackEnd(h->rrpt,h->rpt);
			if(h!=NULL) Rfree(h);
			break;
	}
	return;
}
void executereport1(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	int wcount=0;

	rpt=h->rpt;
	rrpt=h->rrpt;
	if(rpt->range_screen && IsScreenDisplayed(rrpt->rsrsrc)) ++wcount;
	executereport(parent,h);
	if(rrpt->screen_count==wcount) 
	{
		if(rrpt->exit_status!=2)
		{
			ShutdownSubsystems();
		}
		return;
	}
}
static void ArchiveReport(RDArunrpt *rrpt,RDAreport *rpt)
{
	char *aname=NULL,*desc=NULL,*fname=NULL,Xstemp[1024];
	char *temp1=NULL,*c=NULL,*t=NULL,*wgdir=NULL;
	RDATData *prev=NULL;
	int aclass=1;
	int ret_int=0;
	char *subject=NULL;
	short a_fileno=(-1),archwork=(-1);
	char movefile[1024];
#ifdef ARCHIVE_GSV
	char *libx=NULL,*workgroup=NULL;
	RDAGenericSetup *gsv=NULL;
#endif /* ARCHIVE_GSV */
	
	a_fileno=APPOPNNRD("ARCHIVE","ARCHIVES",TRUE,TRUE);
	archwork=APPOPNNRD("ARCHIVE","ARCHWORK",TRUE,FALSE);
	if((a_fileno==(-1)) || (archwork==(-1)))
	{
		prterr("Error:  Unable to Archive Database files.");
		memset(Xstemp,0,1024);
		if(rpt->display->device==0 || rpt->display->device==5)
		{
			sprintf(Xstemp,"%s/%d.pdf",CURRENTDIRECTORY,RGETPID());
		} else sprintf(Xstemp,"%s/%d.txt",CURRENTDIRECTORY,RGETPID());
		unlink(Xstemp);
		return;
	}
	aname=MakeArchiveName(rrpt->module,rpt->name);
	temp1=adddashes(aname);
	ZERNRD(archwork);
	FINDFLDSETSTRING(archwork,"WORK GROUP",rrpt->module);
	if(EQLNRD(archwork,1)) KEYNRD(archwork,1);
	FINDFLDGETSTRING(archwork,"DIRECTORY",&wgdir);
	stoupper(temp1);
	fname=Rmalloc(RDAstrlen(temp1)+5);
	switch(ArchiveExtension)
	{
		case 0:
			sprintf(fname,"%s.pdf",temp1);	
			break;
		case 1:
			sprintf(fname,"%s.txt",temp1);	
			break;
		case 2:
			sprintf(fname,"%s.csv",temp1);	
			break;
		case 3:
			sprintf(fname,"%s.tsv",temp1);	
			break;
		case 4:
			sprintf(fname,"%s.gpl",temp1);	
			break;
		case 5:
			sprintf(fname,"%s.xls",temp1);	
			break;
	}
	if(temp1!=NULL) Rfree(temp1);

#ifndef WIN32
	sprintf(Xstemp,"%s/ARCHIVE/.%s",CURRENTDIRECTORY,(rrpt->module!=NULL ? rrpt->module:""));
#endif
#ifdef WIN32
	sprintf(Xstemp,"%s/ARCHIVE\\.%s",CURRENTDIRECTORY,(rrpt->module!=NULL ? rrpt->module:""));
#endif
	RDAmkdir(Xstemp,00770,&ret_int);

	memset(Xstemp,0,1024);
	if(rpt->display->device==0 || rpt->display->device==5)
	{
		sprintf(Xstemp,"%d.pdf",RGETPID());
	} else {
		sprintf(Xstemp,"%d.txt",RGETPID());
	}
	memset(movefile,0,1024);
#ifndef WIN32
	sprintf(movefile,"ARCHIVE/.%s/%s",rrpt->module,fname);
#endif
#ifdef WIN32
	sprintf(movefile,"ARCHIVE\\.%s\\%s",rrpt->module,fname);
#endif
	RDAMoveFile(Xstemp,movefile);
	
	ZERNRD(a_fileno);
#ifdef ARCHIVE_GSV
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(rrpt->module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,rrpt->module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,rrpt->module);
#endif

	gsv=RDAGenericSetupNew(rrpt->module,"AUTOMAGICAL WORK GROUP");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			workgroup=stralloc(gsv->value.string_value);
		} else {
			workgroup=stralloc(rrpt->module);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",workgroup);
#endif /* ARCHIVE_GSV */
#ifndef ARCHIVE_GSV
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",rrpt->module);
#endif /* ARCHIVE_GSV */
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	if(!EQLNRD(a_fileno,1))
	{
		prev=RDATDataNEW(a_fileno);
	} else KEYNRD(a_fileno,1);
	temp1=Users_Department();
	if(!RDAstrcmp(rrpt->module,"FINMGT") || !RDAstrcmp(rrpt->module,"BNKREC")) subject=stralloc("FINANCE");
	else if(!RDAstrcmp(rrpt->module,"VENPMT")) subject=stralloc("A/P");
	else if(!RDAstrcmp(rrpt->module,"PURORD") || !RDAstrcmp(rrpt->module,"FIXASS") || 
		!RDAstrcmp(rrpt->module,"CATALOGUE") ||
		!RDAstrcmp(rrpt->module,"REQMGT")) subject=stralloc("PURCHASING");
	else if(!RDAstrcmp(rrpt->module,"PAYROLL") || 
		!RDAstrcmp(rrpt->module,"LVEMGT") || !RDAstrcmp(rrpt->module,"EMPABS") || !RDAstrcmp(rrpt->module,"SUBMGT") || !RDAstrcmp(rrpt->module,"POSTRK") || 
		!RDAstrcmp(rrpt->module,"LVSIMP")) subject=stralloc("PAYROLL");
	else if(!RDAstrcmp(rrpt->module,"PRSNNL") || !RDAstrcmp(rrpt->module,"IIDMGT") || !RDAstrcmp(rrpt->module,"JOBPERF") || !RDAstrcmp(rrpt->module,"APPMGT")) subject=stralloc("PERSONNEL");
	else if(!RDAstrcmp(rrpt->module,"BUDPREP")) subject=stralloc("BUDGET");
	else if(RDAstrstr(temp1,"TREASURE")) subject=stralloc("TREASURER");
	else if(RDAstrstr(temp1,"COMM")) subject=stralloc("COMMISSIONER");
	else subject=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	FINDFLDSETCHAR(a_fileno,"DELETEFLAG",FALSE);
#ifdef ARCHIVE_GSV
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",workgroup);
#endif /* ARCHIVE_GSV */
#ifndef ARCHIVE_GSV
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",rrpt->module);
#endif /* ARCHIVE_GSV */
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	FINDFLDSETSTRING(a_fileno,"DESCRIPTION",rpt->title);
	FINDFLDSETSTRING(a_fileno,"SUBJECT",subject);
	FINDFLDSETSHORT(a_fileno,"SOURCE",ArchiveSource);
	FINDFLDSETSTRING(a_fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETSTRING(a_fileno,"FILENAME",fname);
	if(rpt->display->device==0 || rpt->display->device==5)
	{
		FINDFLDSETSTRING(a_fileno,"TYPE","pdf");
	} else {
		aclass=8;
		if(ArchiveExtension==2) 
		{
			FINDFLDSETSTRING(a_fileno,"TYPE","csv");
		} else if(ArchiveExtension==5)
		{
			FINDFLDSETSTRING(a_fileno,"TYPE","xls");
		} else if(ArchiveExtension==3)
		{
			FINDFLDSETSTRING(a_fileno,"TYPE","tsv");
		} else if(ArchiveExtension==4)
		{
			FINDFLDSETSTRING(a_fileno,"TYPE","gpl");
		} else {
			FINDFLDSETSTRING(a_fileno,"TYPE","txt");
		}
	}
	FINDFLDSETINT(a_fileno,"CLASS",aclass);
	c=GETCURRENTDATE10();
	FINDFLDSETSTRING(a_fileno,"DATE",c);
	if(c!=NULL) Rfree(c);
	t=GETCURRENTTIME();
	FINDFLDSETSTRING(a_fileno,"TIME",t);
	if(t!=NULL) Rfree(t);
	WRTTRANS(a_fileno,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);	
	if(fname!=NULL) Rfree(fname);
	if(desc!=NULL) Rfree(desc);
	if(aname!=NULL) Rfree(aname);
#ifdef ARCHIVE_GSV
	if(workgroup!=NULL) Rfree(workgroup);
#endif /* ARCHIVE_GSV */
	if(subject!=NULL) Rfree(subject);
	if(a_fileno!=(-1)) CLSNRD(a_fileno);
	if(archwork!=(-1)) CLSNRD(archwork);
	if(temp1!=NULL) Rfree(temp1);
}
void execnextrpt(RDArsrc *parent,HoldReport *h)
{
	RDAreport *rpt;
	RDArunrpt *runrpt;
	RDArsrc *tmprsrc=NULL;

	rpt=h->rpt;
	runrpt=h->rrpt;
	if(runrpt->archive==TRUE)
	{
		ArchiveReport(runrpt,rpt);
	}
	tmprsrc=runrpt->rsrsrc;
 	runrpt->rsrsrc=NULL;
	if(runrpt->askb4exec!=NULL)
	{
		killwindow(runrpt->askb4exec);
		free_rsrc(runrpt->askb4exec);
	}
	if(!rpt->removesort)
	{
		RUNREPORTADV3(rpt->chain_module,rpt->chain_name,runrpt->sortserver,runrpt->sortfile,runrpt->APPmainLoop,runrpt->exit_status,tmprsrc,runrpt->quitfunc,runrpt->quitfuncrsrc,runrpt->quitfuncArgs,0,runrpt->ProcDataLine);
	} else {
		exit_cleanly(runrpt,rpt,FALSE,NULL,NULL,TRUE,1);
		RUNREPORTADV3(rpt->chain_module,rpt->chain_name,NULL,NULL,runrpt->APPmainLoop,runrpt->exit_status,tmprsrc,runrpt->quitfunc,runrpt->quitfuncrsrc,runrpt->quitfuncArgs,0,runrpt->ProcDataLine);
	}
}
static void makeaccum(RDAreport *rpt,RDAaccum *a,short type,short len)
{
	RDAcontrol *c=NULL;
	RDAaccval *accval=NULL;
	int x;

	a->accvals=Rmalloc((rpt->numcontrols+2)*sizeof(RDAaccval));
	a->num=rpt->numcontrols+2;
	a->fieldtype=type;
	a->len=len;
	for(x=0,accval=a->accvals;x<a->num;++x,++accval)
	{
		if(x==0)
		{
			accval->breakname=stralloc("REPORT");
		} else if(x==1)
		{
			accval->breakname=stralloc("PAGE");
		} else {
			c=rpt->control+(x-2);
			accval->breakname=stralloc(c->name);
		}
		switch(type)
		{
			case SCROLLEDTEXT:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case VARIABLETEXT:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				switch(a->atype)
				{
					case 0: /* AVERAGE */
					case 2: /* FIRST OCCURRENCE */
					case 3: /* LAST OCCURRENCE */
					case 4: /* MAXIMUM */
					case 5: /* MINIMUM */
					case 6: /*PREVIOUS OCCURRENCE*/
					case 7: /* SUMMATION */
						accval->value.string_value=NULL;
						accval->prev_value.string_value=NULL;
						accval->dlen=0;
						accval->pdlen=0;
						break;
					case 1: /* FREQUENCY */
						accval->value.float_value=
							Rmalloc(sizeof(double));
						*accval->value.float_value=0;
						accval->prev_value.float_value=Rmalloc(sizeof(double));
						*accval->prev_value.float_value=0;
						accval->dlen=sizeof(double);
						accval->pdlen=sizeof(double);
						break;
					default:
						prterr("Error unrecognized accumulator type.");
						break;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SHORTV:
			case LONGV:
			case SCROLLEDLIST:
			case SSHORTV:
			case SLONGV:
			case SDOUBLEV:
			case SDECIMALV:
				accval->value.float_value=Rmalloc(sizeof(double));
				*accval->value.float_value=0;
				accval->prev_value.float_value=Rmalloc(sizeof(double));
				*accval->prev_value.float_value=0;
				accval->dlen=sizeof(double);
				accval->pdlen=sizeof(double);
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error unrecognized field type.");
				break;
		}
		accval->count=0;
	}
}
void xprocess_controls(RDArunrpt *runrpt,RDAreport *rpt,int x,short incment,
	int line,char *file)
{
	int y=0,z=0;
	char *temp=NULL;
	RDAcontrol *c1=NULL;
	RDAvirtual *v;
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG process_controls Processing Control Break Level [%d] Incrementing [%s] at line [%d] program [%s].",x,(incment ? "Yes":"No"),line,file);
	}		
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rpt->control!=NULL)
	{
		for(y=(rpt->numcontrols-1);y>(x-1);y--)
		{
			c1=rpt->control+y;
			temp=Rmalloc(RDAstrlen(c1->name)+8);
			sprintf(temp,"%s FOOTER",c1->name);
			print_section(runrpt,rpt,temp,FALSE);
/*
			++c1->footer_count;
*/
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			if(incment) clear_accum(rpt,y+2);
		}
	}
	runrpt->body_count=0;
	if(rpt->control!=NULL)
	{
	for(y=x;y<rpt->numcontrols;++y)
	{
		c1=rpt->control+y;
		temp=Rmalloc(RDAstrlen(c1->name)+8);
		sprintf(temp,"%s HEADER",c1->name);
		print_section(runrpt,rpt,temp,FALSE);
#ifdef XXX
		++c1->header_count;
#endif
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		if(incment)
		{
		field=FLDNRD(runrpt->sortno,c1->name);
		if(field!=NULL)
		{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case VARIABLETEXT:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case BOOLNS:
			case CHARACTERS:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(!isEMPTY(field->data.string_value))
				{
					QUICKALLOC(c1->value.string_value,c1->dlen,field->dlen);
					memcpy(c1->value.string_value,field->data.string_value,field->dlen);
				} else if(c1->dlen>0)
				{
					memset(c1->value.string_value,0,c1->dlen);
				} else {
					c1->value.string_value=NULL;
					c1->dlen=0;
				}
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				if(c1->value.float_value==NULL) 
				{
					c1->value.float_value=
						Rmalloc(sizeof(double));
					c1->dlen=sizeof(double);
				}
				*c1->value.float_value=
					*field->data.float_value;
				break;
			case SSHORTV:
			case SHORTV:
				if(c1->value.short_value==NULL)
				{
					c1->value.short_value=
						Rmalloc(sizeof(short));
					c1->dlen=sizeof(short);
				}
				*c1->value.short_value=
					*field->data.short_value;
				break;
			case SLONGV:
			case LONGV:
			case SCROLLEDLIST:
				if(c1->value.integer_value==NULL)
				{
					c1->value.integer_value=
						Rmalloc(sizeof(int));
					c1->dlen=sizeof(int);
				}
				*c1->value.integer_value=
					*field->data.integer_value;
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error unrecognized field type.");
				break;
		}		
		} else {
			temp=stripmodulename(c1->name);
			for(z=0,v=rpt->virflds;z<rpt->numvirtuals;++z,++v)
			{
				if(!RDAstrcmp(v->name,temp)) break;
			}
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			if(z<rpt->numvirtuals)
			{
				switch(v->type)
				{
					case SCROLLEDTEXT:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case VARIABLETEXT:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case BOOLNS:
					case CHARACTERS:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(!isEMPTY(v->value.string_value))
						{
							QUICKALLOC(c1->value.string_value,c1->dlen,v->dlen);
							memcpy(c1->value.string_value,v->value.string_value,v->dlen);
						} else if(c1->dlen>0)
						{
							memset(c1->value.string_value,0,c1->dlen);
						} else {
							c1->value.string_value=NULL;
							c1->dlen=0;
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(c1->value.float_value==NULL) 
						{
							c1->value.float_value=
								Rmalloc(sizeof(double));
							c1->dlen=sizeof(double);
						}
						*c1->value.float_value=
							*v->value.float_value;
						break;
					case SSHORTV:
					case SHORTV:
						if(c1->value.short_value==NULL)
						{
							c1->value.short_value=
								Rmalloc(sizeof(short));
							c1->dlen=sizeof(short);
						}
						*c1->value.short_value=
							*v->value.short_value;
						break;
					case SLONGV:
					case LONGV:
					case SCROLLEDLIST:
						if(c1->value.integer_value==NULL)
						{
							c1->value.integer_value=
								Rmalloc(sizeof(int));
							c1->dlen=sizeof(int);
						}
						*c1->value.integer_value=
							*v->value.integer_value;
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error unrecognized field type.");
						break;
				}
			}
		}
		}
	}
	}
}
static char *ArchiveNameToGet(HoldReport *HoldReportx,char *name)
{
	RDAreport *rpt=HoldReportx->rpt;
	RDArunrpt *runrpt=HoldReportx->rrpt;
	RDAvirtual *v;
	int x=0;
	char *temp=NULL,*dashname=NULL,*tempvalue=NULL;

	if(rpt!=NULL)
	{
		if(rpt->virflds!=NULL)
		{
			for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
			{
				if(!RDAstrcmp(v->name,name)) break;
			}
			if(x<rpt->numvirtuals)
			{
				if(v->range==2)
				{
					if(runrpt->rsrsrc!=NULL)
					{
						FINDRSCGETSTRING(runrpt->rsrsrc,v->name,
							&v->value.string_value);
					}
				} else if(v->range==3)
				{
					dashname=adddashes(v->name);
					tempvalue=RDA_GetEnv(dashname);
					if(dashname!=NULL) Rfree(dashname);
					if(!isEMPTY(tempvalue))
					{
						if(v->value.string_value!=NULL) Rfree(v->value.string_value);
						v->value.string_value=stralloc(tempvalue);
						v->dlen=RDAstrlen(v->value.string_value)+1;
					} else if(!isEMPTY(v->value.string_value))
					{
						memset(v->value.string_value,0,v->dlen);
					} else {
						v->value.string_value=NULL;
						v->dlen=0;
					}
					v->computed=TRUE;
				}
				if(v->when && !v->computed) 
				{
					HoldReportx=HoldReportNew(runrpt,rpt);
					v->computed=TRUE;
					temp=PP_EVALUATEstr(v->expression,ReportGenSubData1,HoldReportx);
					if(v->value.string_value!=NULL) 
						Rfree(v->value.string_value);
					v->value.string_value=stralloc(temp);
					if(temp!=NULL) Rfree(temp);
					if(v->value.string_value!=NULL)
					{
						v->dlen=RDAstrlen(v->value.string_value)+1;
						if(v->dlen>v->len && v->len>0)
						{
							v->value.string_value[v->len]=0;
						}
					} else v->dlen=0;
					if(HoldReportx!=NULL) Rfree(HoldReportx);
					HoldReportx=NULL;
				}
				if(v->when && v->value.string_value!=NULL && v->dlen>0)
				{
					temp=stralloc(v->value.string_value);
				}
			}
		}
	}
	return(temp);
}
void xReportBackEnd(RDArunrpt *runrpt,RDAreport *rpt,int line,char *file)
{
	int x,y,z,q,u;
	short ef;
	RDAdisplay *d=NULL;
	char *dashname=NULL,*tempvalue=NULL,Xstemp[1024];
	char *tempchar=" ";
	RDAcontrol *c=NULL,*c1=NULL;
	RDAaccum *a=NULL;
	char *temp=NULL,*temp1=NULL,*temp2=NULL;
	NRDfield *field;
	RDAvirtual *v;
	RDAaccval *accval;
	HoldReport *HoldReportx=NULL;
	HoldReport *h=NULL;
	RDArsrc *tmprsrc=NULL;
	char deleteflag=FALSE,*HoldString=NULL,t2pdf[1024],movefile[1024];
	RDApvar *Pvar=NULL;
	short inc_body_count=FALSE;
	APPlib *args=NULL;
	int pdfpagetype=1;
	int ret_int=0;

	CLSReportfiles(runrpt,FALSE);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen_outall)
	{
		prterr("DIAG Report Generator and Expression Parser Diagnostics Enabled.");
		diagrptgen=TRUE;
		diageval=TRUE;
	} else if(diagrptgen_output)
	{
		prterr("DIAG Report Generator Diagnostics Enabled.");
		diagrptgen=TRUE;
	} else if(diagrptgen_outexp)
	{
		prterr("DIAG Expression Parser Diagnostics Enabled.");
		diageval=TRUE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef WIN32
	RDA_SetEnv("REPORT_TITLE",(rpt->title!=NULL ? rpt->title:rpt->name));
	RDA_SetEnv("REPORT_DESCRIPTION",(rpt->desc!=NULL ? rpt->desc:""));
	RDA_SetEnv("CYBER_PRODUCT",runrpt->module);
	RDA_SetEnv("CYBER_PROCESS",rpt->name);
#endif /* WIN32 */
#ifndef WIN32
	RDA_SetEnv("REPORT_TITLE",(rpt->title!=NULL ? rpt->title:rpt->name));
	RDA_SetEnv("REPORT_DESCRIPTION",(rpt->desc!=NULL ? rpt->desc:""));
	RDA_SetEnv("CYBER_PRODUCT",runrpt->module);
	RDA_SetEnv("CYBER_PROCESS",rpt->name);
#endif /* not WIN32 */

	if(runrpt->archive==TRUE)
	{
#ifdef WIN32
		RDA_SetEnv("AUTO_ARCHIVE","TRUE");
#endif /* WIN32 */
#ifndef WIN32
		RDA_SetEnv("AUTO_ARCHIVE","TRUE");
#endif /* not WIN32 */
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG ReportBackEnd Sortfileno [%d] Name [%s] Record Count [%d] at line [%d] program [%s].",runrpt->sortno,runrpt->sortfile,runrpt->record_count,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	runrpt->pageno=0;
	runrpt->page_width=0;
	d=rpt->display;
	if(d==NULL)
	{
		memset(string_temparray,0,501);
		sprintf(string_temparray,"There was no display information found for the report definition of [%s][%s].",(runrpt->module!=NULL?runrpt->module:""),(rpt->name!=NULL?rpt->name:""));
		exit_cleanly(runrpt,rpt,FALSE,"DISPLAY INFORMATION NOT FOUND!",string_temparray,TRUE,1);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_outall)
		{
			prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
			diagrptgen=FALSE;
			diageval=FALSE;
		} else if(diagrptgen_output)
		{
			prterr("DIAG Report Generator Diagnostics Disabled.");
			diagrptgen=FALSE;
		} else if(diagrptgen_outexp)
		{
			prterr("DIAG Expression Parser Diagnostics Disabled.");
			diageval=FALSE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return;
	}
	runrpt->page_count=(int)d->page_length*72;
#ifdef WIN32
	memset(stemp,0,101);
	sprintf(stemp,"%f",rpt->display->page_length);
	RDA_SetEnv("PAGE_LENGTH",stemp);
	
	memset(stemp,0,101);
	sprintf(stemp,"%f",rpt->display->page_width);
	RDA_SetEnv("PAGE_WIDTH",stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%d",rpt->display->dumprinter_type);
	RDA_SetEnv("PRINT_TYPE",stemp);
#endif /* WIN32 */
#ifndef WIN32
	memset(stemp,0,101);
	sprintf(stemp,"%f",rpt->display->page_length);
	RDA_SetEnv("PAGE_LENGTH",stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%f",rpt->display->page_width);
	RDA_SetEnv("PAGE_WIDTH",stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%d",rpt->display->dumprinter_type);
	RDA_SetEnv("PRINT_TYPE",stemp);
#endif /* not WIN32 */
	switch(d->device)
	{
		default:
		case 0:	/* Printer */
		case 5:	/* Popen */
			if((!strncmp(d->set_lpp,"AUTOARCHIVE",11)) && XPERT_SETUP->ARCHIVE==TRUE)
			{
				ArchiveSource=6;
				runrpt->archive=TRUE;
				runrpt->pfile=NULL;
				if(rpt->display->dumprinter==FALSE)
				{
					 /* -pt1  --  8x11 PORTRAIT */
					if((rpt->display->page_length<12.0)&&(rpt->display->page_width<9.0)) pdfpagetype=1; 

					/* -pt2  --  8x11 LANDSCAPE */
					else if((rpt->display->page_length<9.0)&&(rpt->display->page_width<12.0)) pdfpagetype=2; 

					/* -pt3  --  8x14 PORTRAIT */
					else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width<9.0)&&(rpt->display->page_width>7.0)) pdfpagetype=3; 

					/* -pt4  --  8x14 LANDSCAPE */
					else if((rpt->display->page_length<9.0)&&(rpt->display->page_length>7.0)&&(rpt->display->page_width<15.0)&&(rpt->display->page_width>11.0)) pdfpagetype=4; 

					/* -pt5  --  11x14 PORTRAIT */
					else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width>8)&&(rpt->display->page_width<12)) pdfpagetype=5; 

					/* -pt6  --  11x14 LANDSCAPE */
					else if((rpt->display->page_length<12.0)&&(rpt->display->page_length>8.0)&&(rpt->display->page_width<15)&&(rpt->display->page_width>11)) pdfpagetype=6; 

					/* -pt7  --  11x17 PORTRAIT */
					else if((rpt->display->page_length>14.0)&&(rpt->display->page_width>10)&&(rpt->display->page_length>rpt->display->page_width)) pdfpagetype=7; 

					/* -pt8  --  11x17 LANDSCAPE */
					else if((rpt->display->page_length>10.0)&&(rpt->display->page_width>14.0)) pdfpagetype=8; 
					else {
						prterr("Should Not Reach Here.  Review Page Size Ranges.   Page Length [%d]   Page Width [%d].\n",rpt->display->page_length,rpt->display->page_width);
						pdfpagetype=1; 
					}


#ifndef WIN32
						sprintf(t2pdf,"%s/viewpdf.lnx --dont-view --dont-delete -pt%d -ps1 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
#ifdef WIN32
						sprintf(t2pdf,"%s/viewpdf.exe --dont-view --dont-delete -pt%d -ps1 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
				}
			} else {
				if(d->set_lpp_eval==FALSE)
				{
					runrpt->pfile=RDA_popen(d->set_lpp,"w");
				} else {
					HoldReportx=HoldReportNew(runrpt,rpt);
					temp=EVALUATEstr(d->set_lpp,ReportGenSubData1,HoldReportx);
					if(!isEMPTY(temp))
					{
						runrpt->pfile=RDA_popen(temp,"w");
					} else {
						runrpt->pfile=RDA_popen(d->set_lpp,"w");
					}
					if(HoldReportx!=NULL) Rfree(HoldReportx);
					HoldReportx=NULL;
				}
				if(runrpt->pfile==NULL)
				{		
#ifdef USE_RDA_DIAGNOSTICS
					if(diagrptgen_outall)
					{
						prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
						diagrptgen=FALSE;
						diageval=FALSE;
					} else if(diagrptgen_output)
					{
						prterr("DIAG Report Generator Diagnostics Disabled.");
						diagrptgen=FALSE;
					} else if(diagrptgen_outexp)
					{
						prterr("DIAG Expression Parser Diagnostics Disabled.");
						diageval=FALSE;
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					memset(string_temparray,0,501);
					sprintf(string_temparray,"There was no display information found for the report definition of [%s][%s].",(runrpt->module!=NULL?runrpt->module:""),(rpt->name!=NULL?rpt->name:""));
					exit_cleanly(runrpt,rpt,FALSE,"DEVICE OPEN ERROR!",string_temparray,TRUE,1);
					return;
				}
			}
/* SIMPLE PRINTER */
			if(rpt->display->pvars!=NULL)
			{
				for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
				{
					if((!RDAstrcmp(Pvar->name,"PCL JOB HEADER"))) break;
				}
				if(u<rpt->display->num)
				{
					HoldString=Rmalloc(256);
					print_font(runrpt,rpt,Pvar->name,&HoldString);
					RDA_fprintf(runrpt->pfile,"%s",HoldString);
					if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
					if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
				}
			}
			if(rpt->display->dumprinter==TRUE)
			{
				if(runrpt->archive==TRUE) memset(t2pdf,0,1024);
				if((rpt->display->dumprinter_type==0) || (rpt->display->dumprinter_type==4))
				{
					if(runrpt->archive)
					{
						 /* -pt1  --  8x11 PORTRAIT */
						if((rpt->display->page_length<12.0)&&(rpt->display->page_width<9.0)) pdfpagetype=1; 

						/* -pt2  --  8x11 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_width<12.0)) pdfpagetype=2; 

						/* -pt3  --  8x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width<9.0)&&(rpt->display->page_width>7.0)) pdfpagetype=3; 

						/* -pt4  --  8x14 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_length>7.0)&&(rpt->display->page_width<15.0)&&(rpt->display->page_width>11.0)) pdfpagetype=4; 

						/* -pt5  --  11x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width>8)&&(rpt->display->page_width<12)) pdfpagetype=5; 

						/* -pt6  --  11x14 LANDSCAPE */
						else if((rpt->display->page_length<12.0)&&(rpt->display->page_length>8.0)&&(rpt->display->page_width<15)&&(rpt->display->page_width>11)) pdfpagetype=6; 

						/* -pt7  --  11x17 PORTRAIT */
						else if((rpt->display->page_length>14.0)&&(rpt->display->page_width>10)&&(rpt->display->page_length>rpt->display->page_width)) pdfpagetype=7; 

						/* -pt8  --  11x17 LANDSCAPE */
						else if((rpt->display->page_length>10.0)&&(rpt->display->page_width>14.0)) pdfpagetype=8; 
						else {
							prterr("Should Not Reach Here.  Review Page Size Ranges.   Page Length [%d]   Page Width [%d].\n",rpt->display->page_length,rpt->display->page_width);
							pdfpagetype=1; 
						}


#ifndef WIN32
						sprintf(t2pdf,"%s/viewpdf.lnx --dont-view --dont-delete -pt%d -ps1 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
#ifdef WIN32
						sprintf(t2pdf,"%s/viewpdf.exe --dont-view --dont-delete -pt%d -ps1 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if((rpt->display->dumprinter_type==0) && (!RDAstrcmp(Pvar->name,"DUPLEX LONG"))) break;
							else if((rpt->display->dumprinter_type==4) && (!RDAstrcmp(Pvar->name,"SIMPLEX"))) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"NORMAL TEXT")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
				} else if((rpt->display->dumprinter_type==2) || (rpt->display->dumprinter_type==6))
				{
					if(runrpt->archive)
					{
						 /* -pt1  --  8x11 PORTRAIT */
						if((rpt->display->page_length<12.0)&&(rpt->display->page_width<9.0)) pdfpagetype=1; 

						/* -pt2  --  8x11 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_width<12.0)) pdfpagetype=2; 

						/* -pt3  --  8x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width<9.0)&&(rpt->display->page_width>7.0)) pdfpagetype=3; 

						/* -pt4  --  8x14 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_length>7.0)&&(rpt->display->page_width<15.0)&&(rpt->display->page_width>11.0)) pdfpagetype=4; 

						/* -pt5  --  11x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width>8)&&(rpt->display->page_width<12)) pdfpagetype=5; 

						/* -pt6  --  11x14 LANDSCAPE */
						else if((rpt->display->page_length<12.0)&&(rpt->display->page_length>8.0)&&(rpt->display->page_width<15)&&(rpt->display->page_width>11)) pdfpagetype=6; 

						/* -pt7  --  11x17 PORTRAIT */
						else if((rpt->display->page_length>14.0)&&(rpt->display->page_width>10)&&(rpt->display->page_length>rpt->display->page_width)) pdfpagetype=7; 

						/* -pt8  --  11x17 LANDSCAPE */
						else if((rpt->display->page_length>10.0)&&(rpt->display->page_width>14.0)) pdfpagetype=8; 
						else {
							prterr("Should Not Reach Here.  Review Page Size Ranges.   Page Length [%d]   Page Width [%d].\n",rpt->display->page_length,rpt->display->page_width);
							pdfpagetype=1; 
						}


#ifndef WIN32
						sprintf(t2pdf,"%s/viewpdf.lnx --dont-view --dont-delete -pt%d -ps1 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
#ifdef WIN32
						sprintf(t2pdf,"%s/viewpdf.exe --dont-view --dont-delete -pt%d -ps1 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if((rpt->display->dumprinter_type==2) && (!RDAstrcmp(Pvar->name,"DUPLEX SHORT"))) break;
							else if((rpt->display->dumprinter_type==6) && (!RDAstrcmp(Pvar->name,"SIMPLEX"))) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"LANDSCAPE")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"NORMAL TEXT")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
				} else if((rpt->display->dumprinter_type==3) || (rpt->display->dumprinter_type==7))
				{
					if(runrpt->archive)
					{
						 /* -pt1  --  8x11 PORTRAIT */
						if((rpt->display->page_length<12.0)&&(rpt->display->page_width<9.0)) pdfpagetype=1; 

						/* -pt2  --  8x11 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_width<12.0)) pdfpagetype=2; 

						/* -pt3  --  8x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width<9.0)&&(rpt->display->page_width>7.0)) pdfpagetype=3; 

						/* -pt4  --  8x14 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_length>7.0)&&(rpt->display->page_width<15.0)&&(rpt->display->page_width>11.0)) pdfpagetype=4; 

						/* -pt5  --  11x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width>8)&&(rpt->display->page_width<12)) pdfpagetype=5; 

						/* -pt6  --  11x14 LANDSCAPE */
						else if((rpt->display->page_length<12.0)&&(rpt->display->page_length>8.0)&&(rpt->display->page_width<15)&&(rpt->display->page_width>11)) pdfpagetype=6; 

						/* -pt7  --  11x17 PORTRAIT */
						else if((rpt->display->page_length>14.0)&&(rpt->display->page_width>10)&&(rpt->display->page_length>rpt->display->page_width)) pdfpagetype=7; 

						/* -pt8  --  11x17 LANDSCAPE */
						else if((rpt->display->page_length>10.0)&&(rpt->display->page_width>14.0)) pdfpagetype=8; 
						else {
							prterr("Should Not Reach Here.  Review Page Size Ranges.   Page Length [%d]   Page Width [%d].\n",rpt->display->page_length,rpt->display->page_width);
							pdfpagetype=1; 
						}


#ifndef WIN32
						sprintf(t2pdf,"%s/viewpdf.lnx --dont-view --dont-delete -pt%d -ps2 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
#ifdef WIN32
						sprintf(t2pdf,"%s/viewpdf.exe --dont-view --dont-delete -pt%d -ps2 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if((rpt->display->dumprinter_type==3) && (!RDAstrcmp(Pvar->name,"DUPLEX SHORT"))) break;
							else if((rpt->display->dumprinter_type==7) && (!RDAstrcmp(Pvar->name,"SIMPLEX"))) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"LANDSCAPE")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"COMPRESSED TEXT")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
				} else {			
					if(runrpt->archive)
					{
						 /* -pt1  --  8x11 PORTRAIT */
						if((rpt->display->page_length<12.0)&&(rpt->display->page_width<9.0)) pdfpagetype=1; 

						/* -pt2  --  8x11 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_width<12.0)) pdfpagetype=2; 

						/* -pt3  --  8x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width<9.0)&&(rpt->display->page_width>7.0)) pdfpagetype=3; 

						/* -pt4  --  8x14 LANDSCAPE */
						else if((rpt->display->page_length<9.0)&&(rpt->display->page_length>7.0)&&(rpt->display->page_width<15.0)&&(rpt->display->page_width>11.0)) pdfpagetype=4; 

						/* -pt5  --  11x14 PORTRAIT */
						else if((rpt->display->page_length<15.0)&&(rpt->display->page_length>11.0)&&(rpt->display->page_width>8)&&(rpt->display->page_width<12)) pdfpagetype=5; 

						/* -pt6  --  11x14 LANDSCAPE */
						else if((rpt->display->page_length<12.0)&&(rpt->display->page_length>8.0)&&(rpt->display->page_width<15)&&(rpt->display->page_width>11)) pdfpagetype=6; 

						/* -pt7  --  11x17 PORTRAIT */
						else if((rpt->display->page_length>14.0)&&(rpt->display->page_width>10)&&(rpt->display->page_length>rpt->display->page_width)) pdfpagetype=7; 

						/* -pt8  --  11x17 LANDSCAPE */
						else if((rpt->display->page_length>10.0)&&(rpt->display->page_width>14.0)) pdfpagetype=8; 
						else {
							prterr("Should Not Reach Here.  Review Page Size Ranges.   Page Length [%d]   Page Width [%d].\n",rpt->display->page_length,rpt->display->page_width);
							pdfpagetype=1; 
						}


#ifndef WIN32
						sprintf(t2pdf,"%s/viewpdf.lnx --dont-view --dont-delete -pt%d -ps2 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
#ifdef WIN32
						sprintf(t2pdf,"%s/viewpdf.exe --dont-view --dont-delete -pt%d -ps2 -o %s/%d.pdf",DOCUMENTROOT,pdfpagetype,CURRENTDIRECTORY,RGETPID());
#endif
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if((rpt->display->dumprinter_type==1) && (!RDAstrcmp(Pvar->name,"DUPLEX LONG"))) break;
							else if((rpt->display->dumprinter_type==5) && (!RDAstrcmp(Pvar->name,"SIMPLEX"))) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"COMPRESSED TEXT")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
				}
			}
			if(runrpt->archive && !isEMPTY(t2pdf))
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen)
				{
					prterr("%s ",t2pdf);TRACE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				runrpt->fp_pdf=popen(t2pdf,"w");
				if(runrpt->fp_pdf==NULL)
				{
					prterr("Error:  Unable to open pipe for pdf creation.");
					runrpt->archive=FALSE;
					runrpt->fp_pdf=NULL;
				}	
			} else {
				runrpt->fp_pdf=NULL;
			}
			break;
		case 1: /* ASCII FILE */
			if(d->set_lpp_eval==FALSE)
			{
				temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(d->set_lpp)+2);
				sprintf(temp2,"%s/%s",CURRENTDIRECTORY,d->set_lpp);
#ifndef WIN32
				runrpt->fp=fopen(temp2,"w+b");
#else
				runrpt->fp=fopen(temp2,"wbc");
#endif
				if(RDAstrstr(d->set_lpp,".csv") || RDAstrstr(d->set_lpp,".CSV")) ArchiveExtension=2;
				else if(RDAstrstr(d->set_lpp,".tsv") || RDAstrstr(d->set_lpp,".TSV")) ArchiveExtension=3;
				else if(RDAstrstr(d->set_lpp,".gpl") || RDAstrstr(d->set_lpp,".GPL")) ArchiveExtension=4;
				else if(RDAstrstr(d->set_lpp,".xls") || RDAstrstr(d->set_lpp,".XLS")) ArchiveExtension=5;
				else ArchiveExtension=1;
				if(temp2!=NULL) Rfree(temp2);
			} else {
				HoldReportx=HoldReportNew(runrpt,rpt);
				temp=EVALUATEstr(d->set_lpp,ReportGenSubData1,HoldReportx);
				if(!isEMPTY(temp))
				{
					temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp)+2);
					sprintf(temp2,"%s/%s",CURRENTDIRECTORY,temp);
#ifndef WIN32
					runrpt->fp=fopen(temp2,"w+b");
#else
					runrpt->fp=fopen(temp2,"wbc");
#endif
					if(RDAstrstr(temp,".csv") || RDAstrstr(temp,".CSV")) ArchiveExtension=2;
					else if(RDAstrstr(temp,".tsv") || RDAstrstr(temp,".TSV")) ArchiveExtension=3;
					else if(RDAstrstr(temp,".gpl") || RDAstrstr(temp,".GPL")) ArchiveExtension=4;
					else if(RDAstrstr(temp,".xls") || RDAstrstr(temp,".XLS")) ArchiveExtension=5;
					else ArchiveExtension=1;
					if(temp2!=NULL) Rfree(temp2);
				} else {
					temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(d->set_lpp)+2);
					sprintf(temp2,"%s/%s",CURRENTDIRECTORY,d->set_lpp);
#ifndef WIN32
					runrpt->fp=fopen(temp2,"w+b");
#else
					runrpt->fp=fopen(temp2,"wbc");
#endif
					if(temp2!=NULL) Rfree(temp2);
					if(RDAstrstr(d->set_lpp,".csv") || RDAstrstr(d->set_lpp,".CSV")) ArchiveExtension=2;
					else if(RDAstrstr(d->set_lpp,".tsv") || RDAstrstr(d->set_lpp,".TSV")) ArchiveExtension=3;
					else if(RDAstrstr(d->set_lpp,".gpl") || RDAstrstr(d->set_lpp,".GPL")) ArchiveExtension=4;
					else if(RDAstrstr(d->set_lpp,".xls") || RDAstrstr(d->set_lpp,".XLS")) ArchiveExtension=5;
					else ArchiveExtension=1;
				}
				if(HoldReportx!=NULL) Rfree(HoldReportx);
				HoldReportx=NULL;
			}
			if(runrpt->fp==NULL)
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen_outall)
				{
					prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
					diagrptgen=FALSE;
					diageval=FALSE;
				} else if(diagrptgen_output)
				{
					prterr("DIAG Report Generator Diagnostics Disabled.");
					diagrptgen=FALSE;
				} else if(diagrptgen_outexp)
				{
					prterr("DIAG Expression Parser Diagnostics Disabled.");
					diageval=FALSE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				memset(string_temparray,0,501);
				sprintf(string_temparray,"The ASCII file cannot be opened for receiving output in the report [%s][%s].",(runrpt->module!=NULL?runrpt->module:""),(rpt->name!=NULL?rpt->name:""));
				exit_cleanly(runrpt,rpt,FALSE,"FILE OPEN ERROR!",temp,TRUE,1);
				return;
			}
			if(runrpt->archive)
			{
				memset(t2pdf,0,1024);
				sprintf(t2pdf,"%s/%d.txt",CURRENTDIRECTORY,RGETPID());
#ifndef WIN32
				runrpt->fp_pdf=fopen(t2pdf,"w+b");
#else
				runrpt->fp_pdf=fopen(t2pdf,"wbc");
#endif
				if(runrpt->fp_pdf==NULL)
				{
					prterr("Error:  Unable to open file for txt creation.");
					runrpt->archive=FALSE;
					runrpt->fp_pdf=NULL;
				}	
			} else {
				runrpt->fp_pdf=NULL;
			}
			break;
		case 2: /* APPEND ASCII FILE */
			if(d->set_lpp_eval==FALSE)
			{
				temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(d->set_lpp)+2);
				sprintf(temp2,"%s/%s",CURRENTDIRECTORY,d->set_lpp);
#ifndef WIN32
				runrpt->fp=fopen(temp2,"a+b");
#else
				runrpt->fp=fopen(temp2,"abc");
#endif
				if(temp2!=NULL) Rfree(temp2);
				if(RDAstrstr(d->set_lpp,".csv") || RDAstrstr(d->set_lpp,".CSV")) ArchiveExtension=2;
				else if(RDAstrstr(d->set_lpp,".tsv") || RDAstrstr(d->set_lpp,".TSV")) ArchiveExtension=3;
				else if(RDAstrstr(d->set_lpp,".gpl") || RDAstrstr(d->set_lpp,".GPL")) ArchiveExtension=4;
				else if(RDAstrstr(d->set_lpp,".xls") || RDAstrstr(d->set_lpp,".XLS")) ArchiveExtension=5;
				else ArchiveExtension=1;
			} else {
				HoldReportx=HoldReportNew(runrpt,rpt);
				temp=EVALUATEstr(d->set_lpp,ReportGenSubData1,HoldReportx);
				if(!isEMPTY(temp))
				{
					temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp)+2);
					sprintf(temp2,"%s/%s",CURRENTDIRECTORY,temp);
#ifndef WIN32
					runrpt->fp=fopen(temp2,"a+b");
#else
					runrpt->fp=fopen(temp2,"abc");
#endif
					if(temp2!=NULL) Rfree(temp2);
					if(RDAstrstr(temp,".csv") || RDAstrstr(temp,".CSV")) ArchiveExtension=2;
					else if(RDAstrstr(temp,".tsv") || RDAstrstr(temp,".TSV")) ArchiveExtension=3;
					else if(RDAstrstr(d->set_lpp,".gpl") || RDAstrstr(d->set_lpp,".GPL")) ArchiveExtension=4;
					else if(RDAstrstr(d->set_lpp,".xls") || RDAstrstr(d->set_lpp,".XLS")) ArchiveExtension=5;
					else ArchiveExtension=1;
				} else {
					temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(d->set_lpp)+2);
					sprintf(temp2,"%s/%s",CURRENTDIRECTORY,d->set_lpp);
#ifndef WIN32
					runrpt->fp=fopen(temp2,"a+b");
#else
					runrpt->fp=fopen(temp2,"abc");
#endif
					if(temp2!=NULL) Rfree(temp2);
					if(RDAstrstr(d->set_lpp,".csv") || RDAstrstr(d->set_lpp,".CSV")) ArchiveExtension=2;
					else if(RDAstrstr(d->set_lpp,".tsv") || RDAstrstr(d->set_lpp,".TSV")) ArchiveExtension=3;
					else if(RDAstrstr(d->set_lpp,".gpl") || RDAstrstr(d->set_lpp,".GPL")) ArchiveExtension=4;
					else if(RDAstrstr(d->set_lpp,".xls") || RDAstrstr(d->set_lpp,".XLS")) ArchiveExtension=5;
					else ArchiveExtension=1;
				}
				if(HoldReportx!=NULL) Rfree(HoldReportx);
				HoldReportx=NULL;
			}
			if(runrpt->fp==NULL)
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen_outall)
				{
					prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
					diagrptgen=FALSE;
					diageval=FALSE;
				} else if(diagrptgen_output)
				{
					prterr("DIAG Report Generator Diagnostics Disabled.");
					diagrptgen=FALSE;
				} else if(diagrptgen_outexp)
				{
					prterr("DIAG Expression Parser Diagnostics Disabled.");
					diageval=FALSE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				memset(string_temparray,0,501);
				sprintf(string_temparray,"The ASCII file cannot be opened for receiving output in the report [%s][%s].",(runrpt->module!=NULL?runrpt->module:""),(rpt->name!=NULL?rpt->name:""));
				exit_cleanly(runrpt,rpt,FALSE,"FILE OPEN ERROR!",temp,TRUE,1);
				return;
			}
			if(runrpt->archive)
			{
				memset(t2pdf,0,1024);
				sprintf(t2pdf,"%s/%d.txt",CURRENTDIRECTORY,RGETPID());
#ifndef WIN32
				runrpt->fp_pdf=fopen(t2pdf,"w+b");
#else
				runrpt->fp_pdf=fopen(t2pdf,"wbc");
#endif
				if(runrpt->fp_pdf==NULL)
				{
					prterr("Error:  Unable to open file for txt creation.");
					runrpt->archive=FALSE;
					runrpt->fp_pdf=NULL;
				}	
			} else {
				runrpt->fp_pdf=NULL;
			}
			break;
		case 3: /* DATAFILE */
		case 4: /* UPDATE DATAFILE */
			print_section(runrpt,rpt,"REPORT HEADER",FALSE);
			break;
	}
	ZERNRD(runrpt->sortno);
	runrpt->diagrsrc=NULL;
#ifdef XXXX
	if(rpt->diag_breakable<2)
	{
		temp1=Rmalloc(RDAstrlen(rpt->name)+19);
		sprintf(temp1,"%s Output Diagnostic",rpt->name);
		abort_diagnostic=FALSE;
		runrpt->diagrsrc=diagscrn(runrpt->sortno,"DIAGNOSTIC SCREEN","RPTGEN",temp1,(rpt->diag_breakable ? stop_diagnostic:NULL));
		if(temp1!=NULL) Rfree(temp1);
	} else runrpt->diagrsrc=NULL;
	if(runrpt->diagrsrc!=NULL)
	{
		if(ADVmakescrn(runrpt->diagrsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that the screen is available in the module.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
			 prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			tmprsrc=runrpt->diagrsrc;
			temp=Rmalloc(RDAstrlen(runrpt->module)+RDAstrlen(rpt->name)+16);
			sprintf(temp,"%s %s REPORT BACK",runrpt->module,rpt->name);
			SETTITLE(tmprsrc,temp);
			if(temp!=NULL) Rfree(temp);
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif /* XXXX */
	ef=FRSNRD(runrpt->sortno,1);
	while(!ef)
	{
		FINDFLDGETCHAR(runrpt->sortno,"DELETEFLAG",&deleteflag);
		if(!deleteflag) break;
		ef=NXTNRD(runrpt->sortno,1);
	}
	if(rpt->control!=NULL)
	{
		for(x=0,c=rpt->control;x<rpt->numcontrols;++x,++c)
		{
			temp=Rmalloc(RDAstrlen(c->name)+8);
			sprintf(temp,"%s HEADER",c->name);
			print_section(runrpt,rpt,temp,FALSE);
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			field=FLDNRD(runrpt->sortno,c->name);
			if(field!=NULL)
			{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case SOCSECNUM:
				case DATES:
				case TIMES:
				case VARIABLETEXT:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case BOOLNS:
				case CHARACTERS:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(field->data.string_value))
					{
						QUICKALLOC(c->value.string_value,c->dlen,field->dlen);
						memcpy(c->value.string_value,field->data.string_value,field->dlen);
					} else if(c->dlen>0)
					{
						memset(c->value.string_value,0,c->dlen);
					} else {
						c->value.string_value=NULL;
						c->dlen=0;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(c->value.float_value==NULL) 
					{
						c->value.float_value=Rmalloc(sizeof(double));
						c->dlen=sizeof(double);
					}
					*c->value.float_value=*field->data.float_value;
					break;
				case SHORTV:
				case SSHORTV:
					if(c->value.short_value==NULL)
					{
						c->value.short_value=Rmalloc(sizeof(short));
						c->dlen=sizeof(short);
					}
					*c->value.short_value=*field->data.short_value;
					break;
				case LONGV:
				case SCROLLEDLIST:
				case SLONGV:
					if(c->value.integer_value==NULL)
					{
						c->value.integer_value=Rmalloc(sizeof(int));
						c->dlen=sizeof(int);
					}
					*c->value.integer_value=*field->data.integer_value;
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error unrecognized field type.");
					break;
			}	
			} else {
				temp=stripmodulename(c->name);
				for(q=0,v=rpt->virflds;q<rpt->numvirtuals;++q,++v)
				{
					if(!RDAstrcmp(v->name,temp)) break;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				if(q<rpt->numvirtuals)
				{
					switch(v->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case BOOLNS:
						case CHARACTERS:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(!isEMPTY(v->value.string_value))
							{
								QUICKALLOC(c->value.string_value,c->dlen,v->dlen); 
								memcpy(c->value.string_value,v->value.string_value,v->dlen);
							} else if(c->dlen>0)
							{
								memset(c->value.string_value,0,c->dlen);
							} else {
								c->value.string_value=NULL;
								c->dlen=0;
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(c->value.float_value==NULL) 
							{
								c->value.float_value=Rmalloc(sizeof(double));
								c->dlen=sizeof(double);
							}
							*c->value.float_value=*v->value.float_value;
							break;
						case SHORTV:
						case SSHORTV:
							if(c->value.short_value==NULL)
							{
								c->value.short_value=Rmalloc(sizeof(short));
								c->dlen=sizeof(short);
							}
							*c->value.short_value=*v->value.short_value;
							break;
						case LONGV:
						case SCROLLEDLIST:
						case SLONGV:
							if(c->value.integer_value==NULL)
							{
								c->value.integer_value=Rmalloc(sizeof(int));
								c->dlen=sizeof(int);
							}
							*c->value.integer_value=*v->value.integer_value;
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}			
				}
			}
		}
	}
	while(!ef && !abort_diagnostic)
	{
/* Increment Totals */
		FINDFLDGETCHAR(runrpt->sortno,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		if(rpt->accum!=NULL)
		{
		for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
		{
			field=FLDNRD(runrpt->sortno,a->accname);
			if(field!=NULL)
			{
				for(x=0,accval=a->accvals;x<a->num;++x,++accval)
				{
					if((x!=1 && rpt->display->body_count==0)  || rpt->display->body_count)
					{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 3: /* LAST OCCURRENCE */
									if(accval->prev_value.string_value!=NULL)
										Rfree(accval->prev_value.string_value);
									accval->pdlen=0;
									if(!isEMPTY(accval->value.string_value))
									{
										accval->prev_value.string_value=accval->value.string_value;
										accval->pdlen=accval->dlen;
									}	
									accval->value.string_value=NULL;
									accval->dlen=0;
									if(!isEMPTY(field->data.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
										memcpy(accval->value.string_value,field->data.string_value,field->dlen);
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									if(!isEMPTY(field->data.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
										memcpy(accval->value.string_value,field->data.string_value,field->dlen);
									} else if(accval->dlen>0)
									{
										memset(accval->value.string_value,0,accval->dlen);
									} else {
										accval->value.string_value=NULL;
										accval->dlen=0;
									}
									break;
								case 2: /* FIRST OCCURRENCE */
									if(!isEMPTY(field->data.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
										memcpy(accval->value.string_value,field->data.string_value,field->dlen);
									} else if(accval->value.string_value!=NULL)
									{
										memset(accval->value.string_value,0,accval->dlen);
									} else {
										accval->value.string_value=NULL;
										accval->dlen=0;
									}
									if(!accval->count)
									{
										if(!isEMPTY(accval->value.string_value))
										{
											QUICKALLOC(accval->prev_value.string_value,accval->pdlen,accval->dlen);
											memcpy(accval->prev_value.string_value,accval->value.string_value,accval->dlen);
										} else if(accval->value.string_value!=NULL)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
									}
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*field->data.string_value;
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.string_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.string_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.float_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=*accval->value.float_value;
									*accval->value.float_value=
										*field->data.float_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SHORTV:
						case SSHORTV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.short_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*field->data.short_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.short_value;
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SLONGV:
						case LONGV:
						case SCROLLEDLIST:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.integer_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*field->data.integer_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.integer_value;
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
					}
				}
			} else {
				temp=stripmodulename(a->accname);
				for(z=0,v=rpt->virflds;z<rpt->numvirtuals;++z,++v)
				{
					if(!RDAstrcmp(v->name,temp)) break;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				if(z<rpt->numvirtuals)
				{
					if(!v->computed) 
					{
						h=HoldReportNew(runrpt,rpt);
						v->computed=TRUE;
						tempvalue=NULL;
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
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSTRING(runrpt->rsrsrc,v->name,
										&v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(!isEMPTY(tempvalue))
									{
										if(v->value.string_value!=NULL) Rfree(v->value.string_value);
										v->value.string_value=stralloc(tempvalue);
										v->dlen=RDAstrlen(v->value.string_value)+1;
										
									} else if(!isEMPTY(v->value.string_value))
									{
										memset(v->value.string_value,0,v->dlen);
									} else {
										v->value.string_value=NULL;
										v->dlen=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									temp=PP_EVALUATEstr(v->expression,ReportGenSubData1,h);
									if(v->value.string_value!=NULL) 
										Rfree(v->value.string_value);
									v->value.string_value=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
								}
								if(v->value.string_value!=NULL)
								{
									v->dlen=RDAstrlen(v->value.string_value)+1;
									if(v->dlen>v->len && v->len>0)
									{
										v->value.string_value[v->len]=0;
									}
								} else v->dlen=0;
								break;
							case SHORTV:
							case SSHORTV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSHORT(runrpt->rsrsrc,v->name,
										v->value.short_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.short_value=Rmalloc(sizeof(short));
										v->dlen=sizeof(short);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.short_value=Ratoi(tempvalue);
									} else {
										*v->value.short_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData1,h);
								}
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETINT(runrpt->rsrsrc,v->name,
										v->value.integer_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.integer_value=Rmalloc(sizeof(int));
										v->dlen=sizeof(int);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.integer_value=Ratoi(tempvalue);
									} else {
										*v->value.integer_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData1,h);
								}
								break;
							case BOOLNS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										if(Ratoi(tempvalue))
										{
											*v->value.string_value=TRUE;
										} else {
											*v->value.string_value=FALSE;
										}
									} else {
										*v->value.string_value=FALSE;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case CHARACTERS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.string_value=*tempvalue;
									} else {
										*v->value.string_value=*tempchar;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETDOUBLE(runrpt->rsrsrc,v->name,
										v->value.float_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.float_value=Rmalloc(sizeof(double));
										v->dlen=sizeof(double);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.float_value=atof(tempvalue);
									} else {
										*v->value.float_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
									if(v->type==DOLLARS || v->type==DOUBLEV ||
										v->type==SDOUBLEV ||
										v->type==DOLLARS_NOCENTS)
									{
										*v->value.float_value=round(*v->value.float_value);
									}
								}
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
						if(h!=NULL) Rfree(h);
						h=NULL;
					}
					for(x=0,accval=a->accvals;x<a->num;++x,++accval)
					{
						if((x!=1 && rpt->display->body_count==0)  || rpt->display->body_count)
						{
						switch(v->type)
						{
							case SCROLLEDTEXT:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case VARIABLETEXT:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										if(!isEMPTY(v->value.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
											memcpy(accval->value.string_value,v->value.string_value,v->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
										if(!accval->count)
										{
											if(!isEMPTY(accval->value.string_value))
											{
												QUICKALLOC(accval->prev_value.string_value,accval->pdlen,accval->dlen);
												memcpy(accval->prev_value.string_value,accval->value.string_value,accval->dlen);
											} else if(accval->dlen>0)
											{
												memset(accval->prev_value.string_value,0,accval->pdlen);
											} else {
												accval->prev_value.string_value=NULL;
												accval->pdlen=0;
											}
										}
										break;
									case 3: /* LAST OCCURRENCE */
										if(accval->prev_value.string_value!=NULL) Rfree(accval->prev_value.string_value);
										accval->prev_value.string_value=
											accval->value.string_value;
										accval->pdlen=accval->dlen;
										accval->value.string_value=NULL;
										accval->dlen=0;
										if(!isEMPTY(v->value.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
											memcpy(accval->value.string_value,v->value.string_value,v->dlen);
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										if(!isEMPTY(v->value.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
											memcpy(accval->value.string_value,v->value.string_value,v->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=
											*accval->value.float_value;
										*accval->value.float_value=
											*v->value.string_value;
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.string_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.string_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.float_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=*accval->value.float_value;
										*accval->value.float_value=
											*v->value.float_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
									case 7: /* SUMMATION */
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.float_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SHORTV:
							case SSHORTV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.short_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=
											*accval->value.float_value;
										*accval->value.float_value=
											*v->value.short_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.short_value;
										break;
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SLONGV:
							case LONGV:
							case SCROLLEDLIST:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.integer_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=
											*accval->value.float_value;
										*accval->value.float_value=
											*v->value.integer_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.integer_value;
										break;
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BUTTONS:
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
						}
					}
				}
			}
		}
		}
/* Increment Total Number Records & Update Screen */
/* Test Diagnostics Screen for aborting */
		if(d->body_count_type==FALSE)
		{
			if((runrpt->body_count+1)>d->body_count && d->body_count>0)
			{
				process_controls(runrpt,rpt,(rpt->numcontrols-1),FALSE);
				runrpt->body_count=0;
			}
		}
/* Loop through sort file in sorted sequence */
		if(rpt->control!=NULL)
		{
			for(x=0,c=rpt->control;x<rpt->numcontrols;++x,++c)
			{
				field=FLDNRD(runrpt->sortno,c->name);
				if(field!=NULL)
				{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(RDAstrcmp(field->data.string_value,
								c->value.string_value))
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(*field->data.float_value!=*c->value.float_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							if(*field->data.integer_value!=
								*c->value.integer_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(*field->data.short_value!=
								*c->value.short_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case BUTTONS:
							break;
						case BOOLNS:
						case CHARACTERS:
							if(*field->data.string_value!=
								*c->value.string_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
				} else {
					temp=stripmodulename(c->name);
					for(q=0,v=rpt->virflds;q<rpt->numvirtuals;++q,++v)
					{
						if(!RDAstrcmp(v->name,temp)) break;
					}
/* SJS TEST */
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					switch(v->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(RDAstrcmp(v->value.string_value,
								c->value.string_value))
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(*v->value.float_value!=*c->value.float_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							if(*v->value.integer_value!=
								*c->value.integer_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(*v->value.short_value!=*c->value.short_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						case BUTTONS:
							break;
						case BOOLNS:
						case CHARACTERS:
							if(*v->value.string_value!=*c->value.string_value)
							{
								process_controls(runrpt,rpt,x,TRUE);
							}
							break;
						default:
							prterr("Error unrecognized field type [%d] in expression [%s].",v->type,c->name);
							break;
					}
				}
			}
		}
/* Increment Totals */
		if(rpt->accum!=NULL)
		{
		for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
		{
			field=FLDNRD(runrpt->sortno,a->accname);
			if(field!=NULL)
			{
				for(x=0,accval=a->accvals;x<a->num;++x,++accval)
				{
					if((x!=1 && rpt->display->body_count==0)  || rpt->display->body_count)
					{
					++accval->count;
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 3: /* LAST OCCURRENCE */
								case 2: /* FIRST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(RDAstrcmp(field->data.string_value,
										accval->value.string_value)>0)
									{
										if(!isEMPTY(field->data.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
											memcpy(accval->value.string_value,field->data.string_value,field->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
									}
									break;
								case 5: /* MINIMUM */
									if(RDAstrcmp(field->data.string_value,
										accval->value.string_value)<0 || 
										accval->count==1)
									{
										if(!isEMPTY(field->data.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
											memcpy(accval->value.string_value,field->data.string_value,field->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
									}
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.string_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 3: /* LAST OCCURRENCE */
								case 2: /* FIRST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.string_value)
									{
										*accval->value.float_value=
											*field->data.string_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.string_value || 
										accval->count==1)
									{
										*accval->value.float_value=
											*field->data.string_value;
									}
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=
										*field->data.string_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SDOUBLEV:
						case SDECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.float_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.float_value)
									{
										*accval->value.float_value=*field->data.float_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.float_value || 
										accval->count==1)
									{
										*accval->value.float_value=
											*field->data.float_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=
										*field->data.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SHORTV:
						case SSHORTV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.short_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.short_value)
									{
										*accval->value.float_value=
											*field->data.short_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.short_value || 
										accval->count==1)
									{
										*accval->value.float_value=
											*field->data.short_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=*field->data.short_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.integer_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 2: /* FIRST OCCURRENCE */
									break;
								case 3: /* LAST OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.integer_value)
									{
										*accval->value.float_value=
											*field->data.integer_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.integer_value || 
										accval->count==1)
									{
										*accval->value.float_value=
											*field->data.integer_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=
										*field->data.integer_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
					}
				}
			} else {
				temp=stripmodulename(a->accname);
				for(z=0,v=rpt->virflds;z<rpt->numvirtuals;++z,++v)
				{
					if(!RDAstrcmp(v->name,temp)) break;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				if(z<rpt->numvirtuals)
				{
					if(!v->computed) 
					{
						h=HoldReportNew(runrpt,rpt);
						v->computed=TRUE;
						tempvalue=NULL;
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
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSTRING(runrpt->rsrsrc,v->name,
										&v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(!isEMPTY(tempvalue))
									{
										if(v->value.string_value!=NULL) Rfree(v->value.string_value);
										v->value.string_value=stralloc(tempvalue);
										v->dlen=RDAstrlen(v->value.string_value)+1;
										
									} else if(v->value.string_value!=NULL)
									{
										memset(v->value.string_value,0,v->dlen);
									} else {
										v->value.string_value=NULL;
										v->dlen=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									temp=PP_EVALUATEstr(v->expression,ReportGenSubData1,h);
									if(v->value.string_value!=NULL) 
										Rfree(v->value.string_value);
									v->value.string_value=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
								}
								if(v->value.string_value!=NULL)
								{
									v->dlen=RDAstrlen(v->value.string_value)+1;
									if(v->dlen>v->len && v->len>0)
									{
										v->value.string_value[v->len]=0;
									}
								} else v->dlen=0;
								break;
							case SHORTV:
							case SSHORTV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSHORT(runrpt->rsrsrc,v->name,
										v->value.short_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.short_value=Rmalloc(sizeof(short));
										v->dlen=sizeof(short);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.short_value=Ratoi(tempvalue);
									} else {
										*v->value.short_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData1,h);
								}
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETINT(runrpt->rsrsrc,v->name,
										v->value.integer_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.integer_value=Rmalloc(sizeof(int));
										v->dlen=sizeof(int);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.integer_value=Ratoi(tempvalue);
									} else {
										*v->value.integer_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData1,h);
								}
								break;
							case BOOLNS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										if(Ratoi(tempvalue))
										{
											*v->value.string_value=TRUE;
										} else {
											*v->value.string_value=FALSE;
										}
									} else {
										*v->value.string_value=FALSE;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case CHARACTERS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.string_value=*tempvalue;
									} else {
										*v->value.string_value=*tempchar;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETDOUBLE(runrpt->rsrsrc,v->name,
										v->value.float_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.float_value=Rmalloc(sizeof(double));
										v->dlen=sizeof(double);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.float_value=atof(tempvalue);
									} else {
										*v->value.float_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
									if(v->type==DOLLARS || v->type==DOUBLEV ||
										v->type==SDOUBLEV ||
										v->type==DOLLARS_NOCENTS)
									{
										*v->value.float_value=round(*v->value.float_value);
									}
								}
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
						if(h!=NULL) Rfree(h);
						h=NULL;
					}
					for(x=0,accval=a->accvals;x<a->num;++x,++accval)
					{
						if((x!=1 && rpt->display->body_count==0)  || rpt->display->body_count)
						{
						++accval->count;
						switch(v->type)
						{
							case SCROLLEDTEXT:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case VARIABLETEXT:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 3: /* LAST OCCURRENCE */
									case 2: /* FIRST OCCURRENCE */
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(RDAstrcmp(v->value.string_value,
											accval->value.string_value)>0)
										{
											if(!isEMPTY(v->value.string_value))
											{
												QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
												memcpy(accval->value.string_value,v->value.string_value,v->dlen);
											} else if(accval->dlen>0)
											{ 
												memset(accval->value.string_value,0,accval->dlen);
											} else {
												accval->value.string_value=NULL;
												accval->dlen=0;
											}
										}
										break;
									case 5: /* MINIMUM */
										if(RDAstrcmp(v->value.string_value,
											accval->value.string_value)<0 || accval->count==1)
										{
											if(!isEMPTY(v->value.string_value))
											{
												QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
												memcpy(accval->value.string_value,v->value.string_value,v->dlen);
											} else if(accval->dlen>0)
											{ 
												memset(accval->value.string_value,0,accval->dlen);
											} else {
												accval->value.string_value=NULL;
												accval->dlen=0;
											}
										}
										break;
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.string_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 3: /* LAST OCCURRENCE */
									case 2: /* FIRST OCCURRENCE */
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.string_value)
										{
											*accval->value.float_value=
												*v->value.string_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.string_value || accval->count==0)
										{
											*accval->value.float_value=
												*v->value.string_value;
										}
										break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=
											*v->value.string_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SDOUBLEV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.float_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 2: /* FIRST OCCURRENCE */
									case 3: /* LAST OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.float_value)
										{
											*accval->value.float_value=*v->value.float_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.float_value || accval->count==1)
										{
											*accval->value.float_value=
												*v->value.float_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
												break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=*v->value.float_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SHORTV:
							case SSHORTV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.short_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 2: /* FIRST OCCURRENCE */
									case 3: /* LAST OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.short_value)
										{
											*accval->value.float_value=
												*v->value.short_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.short_value || 
											accval->count==1)
										{
											*accval->value.float_value=
												*v->value.short_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=*v->value.short_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.integer_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 2: /* FIRST OCCURRENCE */
										break;
									case 3: /* LAST OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.integer_value)
										{
											*accval->value.float_value=
												*v->value.integer_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.integer_value || accval->count==1)
										{
											*accval->value.float_value=
												*v->value.integer_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=
											*v->value.integer_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BUTTONS:
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
						}
					}
				}
			}
		}
		}
/* Process record and Write Output */
		inc_body_count=print_section(runrpt,rpt,"DETAIL LINES",FALSE);
		if(rpt->accum!=NULL && rpt->display->body_count==0)
		{
		for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
		{
			field=FLDNRD(runrpt->sortno,a->accname);
			if(field!=NULL)
			{
				accval=a->accvals+1;
				if(accval!=NULL)
				{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 3: /* LAST OCCURRENCE */
									if(accval->prev_value.string_value!=NULL)
										Rfree(accval->prev_value.string_value);
									accval->pdlen=0;
									if(!isEMPTY(accval->value.string_value))
									{
										accval->prev_value.string_value=accval->value.string_value;
										accval->pdlen=accval->dlen;
									}	
									accval->value.string_value=NULL;
									accval->dlen=0;
									if(!isEMPTY(field->data.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
										memcpy(accval->value.string_value,field->data.string_value,field->dlen);
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									if(!isEMPTY(field->data.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
										memcpy(accval->value.string_value,field->data.string_value,field->dlen);
									} else if(accval->dlen>0)
									{
										memset(accval->value.string_value,0,accval->dlen);
									} else {
										accval->value.string_value=NULL;
										accval->dlen=0;
									}
									break;
								case 2: /* FIRST OCCURRENCE */
									if(!isEMPTY(field->data.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
										memcpy(accval->value.string_value,field->data.string_value,field->dlen);
									
									} else if(accval->dlen>0)
									{
										memset(accval->value.string_value,0,accval->dlen);
									} else {
										accval->value.string_value=NULL;
										accval->dlen=0;
									}
									if(!accval->count)
									{
										if(!isEMPTY(accval->value.string_value))
										{
											QUICKALLOC(accval->prev_value.string_value,accval->pdlen,accval->dlen);
											memcpy(accval->prev_value.string_value,accval->value.string_value,accval->dlen);
										} else if(accval->value.string_value!=NULL)
										{
											memset(accval->prev_value.string_value,0,accval->pdlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
									}
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*field->data.string_value;
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.string_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.string_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.float_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=*accval->value.float_value;
									*accval->value.float_value=
										*field->data.float_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SHORTV:
						case SSHORTV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.short_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*field->data.short_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.short_value;
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SLONGV:
						case LONGV:
						case SCROLLEDLIST:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=
										*field->data.integer_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=
											*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*field->data.integer_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*field->data.integer_value;
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
				}
			} else {
				temp=stripmodulename(a->accname);
				for(z=0,v=rpt->virflds;z<rpt->numvirtuals;++z,++v)
				{
					if(!RDAstrcmp(v->name,temp)) break;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				if(z<rpt->numvirtuals)
				{
					if(!v->computed) 
					{
						h=HoldReportNew(runrpt,rpt);
						v->computed=TRUE;
						tempvalue=NULL;
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
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSTRING(runrpt->rsrsrc,v->name,
										&v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(!isEMPTY(tempvalue))
									{
										if(v->value.string_value!=NULL) Rfree(v->value.string_value);
										v->value.string_value=stralloc(tempvalue);
										v->dlen=RDAstrlen(v->value.string_value)+1;
										
									} else if(!isEMPTY(v->value.string_value))
									{
										memset(v->value.string_value,0,v->dlen);
									} else {
										v->value.string_value=NULL;
										v->dlen=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									temp=PP_EVALUATEstr(v->expression,ReportGenSubData1,h);
									if(v->value.string_value!=NULL) 
										Rfree(v->value.string_value);
									v->value.string_value=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
								}
								if(v->value.string_value!=NULL)
								{
									v->dlen=RDAstrlen(v->value.string_value)+1;
									if(v->dlen>v->len && v->len>0)
									{
										v->value.string_value[v->len]=0;
									}
								} else v->dlen=0;
								break;
							case SHORTV:
							case SSHORTV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSHORT(runrpt->rsrsrc,v->name,
										v->value.short_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.short_value=Rmalloc(sizeof(short));
										v->dlen=sizeof(short);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.short_value=Ratoi(tempvalue);
									} else {
										*v->value.short_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData1,h);
								}
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETINT(runrpt->rsrsrc,v->name,
										v->value.integer_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.integer_value=Rmalloc(sizeof(int));
										v->dlen=sizeof(int);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.integer_value=Ratoi(tempvalue);
									} else {
										*v->value.integer_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData1,h);
								}
								break;
							case BOOLNS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										if(Ratoi(tempvalue))
										{
											*v->value.string_value=TRUE;
										} else {
											*v->value.string_value=FALSE;
										}
									} else {
										*v->value.string_value=FALSE;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case CHARACTERS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.string_value=*tempvalue;
									} else {
										*v->value.string_value=*tempchar;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETDOUBLE(runrpt->rsrsrc,v->name,
										v->value.float_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.float_value=Rmalloc(sizeof(double));
										v->dlen=sizeof(double);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.float_value=atof(tempvalue);
									} else {
										*v->value.float_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
									if(v->type==DOLLARS || v->type==DOUBLEV ||
										v->type==SDOUBLEV ||
										v->type==DOLLARS_NOCENTS)
									{
										*v->value.float_value=round(*v->value.float_value);
									}
								}
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
						if(h!=NULL) Rfree(h);
						h=NULL;
					}
					accval=a->accvals+1;
					if(accval!=NULL)
					{
						switch(v->type)
						{
							case SCROLLEDTEXT:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case VARIABLETEXT:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										if(!isEMPTY(v->value.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
											memcpy(accval->value.string_value,v->value.string_value,v->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
										if(!accval->count)
										{
											if(!isEMPTY(accval->value.string_value))
											{
												QUICKALLOC(accval->prev_value.string_value,accval->pdlen,accval->dlen);
												memcpy(accval->prev_value.string_value,accval->value.string_value,accval->dlen);
											} else if(accval->dlen>0)
											{
												memset(accval->prev_value.string_value,0,accval->pdlen);
											} else {
												accval->prev_value.string_value=NULL;
												accval->pdlen=0;
											}
										}
										break;
									case 3: /* LAST OCCURRENCE */
										if(accval->prev_value.string_value!=NULL) Rfree(accval->prev_value.string_value);
										accval->prev_value.string_value=
											accval->value.string_value;
										accval->pdlen=accval->dlen;
										accval->value.string_value=NULL;
										accval->dlen=0;
										if(!isEMPTY(v->value.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
											memcpy(accval->value.string_value,v->value.string_value,v->dlen);
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										if(!isEMPTY(v->value.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
											memcpy(accval->value.string_value,v->value.string_value,v->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=
											*accval->value.float_value;
										*accval->value.float_value=
											*v->value.string_value;
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.string_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.string_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.float_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=*accval->value.float_value;
										*accval->value.float_value=
											*v->value.float_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
									case 7: /* SUMMATION */
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.float_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SHORTV:
							case SSHORTV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.short_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=
											*accval->value.float_value;
										*accval->value.float_value=
											*v->value.short_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.short_value;
										break;
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SLONGV:
							case LONGV:
							case SCROLLEDLIST:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
									case 1: /* FREQUENCY */
										break;
									case 2: /* FIRST OCCURRENCE */
										*accval->value.float_value=
											*v->value.integer_value;
										if(!accval->count)
										{
											*accval->prev_value.float_value=
												*accval->value.float_value;
										}
										break;
									case 3: /* LAST OCCURRENCE */
										*accval->prev_value.float_value=
											*accval->value.float_value;
										*accval->value.float_value=
											*v->value.integer_value;
										break;
									case 4: /* MAXIMUM */
									case 5: /* MINIMUM */
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										*accval->value.float_value=
											*v->value.integer_value;
										break;
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BUTTONS:
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
					}
				}
			}
		}
		}
		if(rpt->accum!=NULL && rpt->display->body_count==0)
		{
		for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
		{
			field=FLDNRD(runrpt->sortno,a->accname);
			if(field!=NULL)
			{
				accval=a->accvals+1;
				if(accval!=NULL)
				{
					++accval->count;
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 3: /* LAST OCCURRENCE */
								case 2: /* FIRST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(RDAstrcmp(field->data.string_value,
										accval->value.string_value)>0)
									{
										if(!isEMPTY(field->data.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
											memcpy(accval->value.string_value,field->data.string_value,field->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
									}
									break;
								case 5: /* MINIMUM */
									if(RDAstrcmp(field->data.string_value,
										accval->value.string_value)<0 || 
										accval->count==1)
									{
										if(!isEMPTY(field->data.string_value))
										{
											QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
											memcpy(accval->value.string_value,field->data.string_value,field->dlen);
										} else if(accval->dlen>0)
										{
											memset(accval->value.string_value,0,accval->dlen);
										} else {
											accval->value.string_value=NULL;
											accval->dlen=0;
										}
									}
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.string_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 3: /* LAST OCCURRENCE */
								case 2: /* FIRST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.string_value)
									{
										*accval->value.float_value=
											*field->data.string_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.string_value || 
										accval->count==0)
									{
										*accval->value.float_value=
											*field->data.string_value;
									}
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=
										*field->data.string_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SDOUBLEV:
						case SDECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.float_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.float_value)
									{
										*accval->value.float_value=*field->data.float_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.float_value || 
										accval->count==1)
									{
										*accval->value.float_value=
											*field->data.float_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=
										*field->data.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SHORTV:
						case SSHORTV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.short_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.short_value)
									{
										*accval->value.float_value=
											*field->data.short_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.short_value || 
										accval->count==1)
									{
										*accval->value.float_value=
											*field->data.short_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=*field->data.short_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
									*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*field->data.integer_value)/(accval->count);
									break;
								case 1: /* FREQUENCY */
									*accval->value.float_value+=1;
									break;
								case 2: /* FIRST OCCURRENCE */
									break;
								case 3: /* LAST OCCURRENCE */
									break;
								case 4: /* MAXIMUM */
									if(*accval->value.float_value<
										*field->data.integer_value)
									{
										*accval->value.float_value=
											*field->data.integer_value;
									}
									break;
								case 5: /* MINIMUM */
									if(*accval->value.float_value>
										*field->data.integer_value || 
										accval->count==1)
									{
										*accval->value.float_value=
											*field->data.integer_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									break;
								case 7: /* SUMMATION */
									*accval->value.float_value+=
										*field->data.integer_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
				}
			} else {
				temp=stripmodulename(a->accname);
				for(z=0,v=rpt->virflds;z<rpt->numvirtuals;++z,++v)
				{
					if(!RDAstrcmp(v->name,temp)) break;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				if(z<rpt->numvirtuals)
				{
					if(!v->computed) 
					{
						h=HoldReportNew(runrpt,rpt);
						v->computed=TRUE;
						tempvalue=NULL;
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
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSTRING(runrpt->rsrsrc,v->name,
										&v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(!isEMPTY(tempvalue))
									{
										if(v->value.string_value!=NULL) Rfree(v->value.string_value);
										v->value.string_value=stralloc(tempvalue);
										v->dlen=RDAstrlen(v->value.string_value)+1;
										
									} else if(v->value.string_value!=NULL)
									{
										memset(v->value.string_value,0,v->dlen);
									} else {
										v->value.string_value=NULL;
										v->dlen=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									temp=PP_EVALUATEstr(v->expression,ReportGenSubData1,h);
									if(v->value.string_value!=NULL) 
										Rfree(v->value.string_value);
									v->value.string_value=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
								}
								if(v->value.string_value!=NULL)
								{
									v->dlen=RDAstrlen(v->value.string_value)+1;
									if(v->dlen>v->len && v->len>0)
									{
										v->value.string_value[v->len]=0;
									}
								} else v->dlen=0;
								break;
							case SHORTV:
							case SSHORTV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETSHORT(runrpt->rsrsrc,v->name,
										v->value.short_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.short_value=Rmalloc(sizeof(short));
										v->dlen=sizeof(short);
									}
									if(!isEMPTY(tempvalue))
									{	
										*v->value.short_value=Ratoi(tempvalue);
									} else {
										*v->value.short_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData1,h);
								}
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETINT(runrpt->rsrsrc,v->name,
										v->value.integer_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.integer_value=Rmalloc(sizeof(int));
										v->dlen=sizeof(int);
									}
									if(!isEMPTY(tempvalue))
									{	
										*v->value.integer_value=Ratoi(tempvalue);
									} else {
										*v->value.integer_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData1,h);
								}
								break;
							case BOOLNS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{	
										if(Ratoi(tempvalue))
										{
											*v->value.string_value=TRUE;
										} else {
											*v->value.string_value=FALSE;
										}
									} else {
										*v->value.string_value=FALSE;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case CHARACTERS:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
										v->value.string_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{	
										*v->value.string_value=*tempvalue;
									} else {
										*v->value.string_value=*tempchar;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(v->range==2)
								{
									if(runrpt->rsrsrc!=NULL)
									{
									FINDRSCGETDOUBLE(runrpt->rsrsrc,v->name,
										v->value.float_value);
									}
								} else if(v->range==3)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.float_value=Rmalloc(sizeof(double));
										v->dlen=sizeof(double);
									}
									if(!isEMPTY(tempvalue))
									{	
										*v->value.float_value=atof(tempvalue);
									} else {
										*v->value.float_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
									if(v->type==DOLLARS || v->type==DOUBLEV ||
										v->type==SDOUBLEV ||
										v->type==DOLLARS_NOCENTS)
									{
										*v->value.float_value=round(*v->value.float_value);
									}
								}
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
						if(h!=NULL) Rfree(h);
						h=NULL;
					}
					accval=a->accvals+1;
					if(accval!=NULL)
					{
						++accval->count;
						switch(v->type)
						{
							case SCROLLEDTEXT:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case VARIABLETEXT:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 3: /* LAST OCCURRENCE */
									case 2: /* FIRST OCCURRENCE */
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(RDAstrcmp(v->value.string_value,
											accval->value.string_value)>0)
										{
											if(!isEMPTY(v->value.string_value))
											{
												QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
												memcpy(accval->value.string_value,v->value.string_value,v->dlen);
											} else if(accval->dlen>0)
											{ 
												memset(accval->value.string_value,0,accval->dlen);
											} else {
												accval->value.string_value=NULL;
												accval->dlen=0;
											}
										}
										break;
									case 5: /* MINIMUM */
										if(RDAstrcmp(v->value.string_value,
											accval->value.string_value)<0 || accval->count==1)
										{
											if(!isEMPTY(v->value.string_value))
											{
												QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
												memcpy(accval->value.string_value,v->value.string_value,v->dlen);
											} else if(accval->dlen>0)
											{ 
												memset(accval->value.string_value,0,accval->dlen);
											} else {
												accval->value.string_value=NULL;
												accval->dlen=0;
											}
										}
										break;
									case 7: /* SUMMATION */
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.string_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 3: /* LAST OCCURRENCE */
									case 2: /* FIRST OCCURRENCE */
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.string_value)
										{
											*accval->value.float_value=
												*v->value.string_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.string_value || accval->count==0)
										{
											*accval->value.float_value=
												*v->value.string_value;
										}
										break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=
											*v->value.string_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SDOUBLEV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.float_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 2: /* FIRST OCCURRENCE */
									case 3: /* LAST OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.float_value)
										{
											*accval->value.float_value=*v->value.float_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.float_value || accval->count==1)
										{
											*accval->value.float_value=
												*v->value.float_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
												break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=*v->value.float_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case SHORTV:
							case SSHORTV:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.short_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 2: /* FIRST OCCURRENCE */
									case 3: /* LAST OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.short_value)
										{
											*accval->value.float_value=
												*v->value.short_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.short_value || 
											accval->count==1)
										{
											*accval->value.float_value=
												*v->value.short_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=*v->value.short_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								switch(a->atype)
								{
									case 0: /* AVERAGE */
										*accval->value.float_value=((*accval->value.float_value*(accval->count-1))+*v->value.integer_value)/(accval->count);
										break;
									case 1: /* FREQUENCY */
										*accval->value.float_value+=1;
										break;
									case 2: /* FIRST OCCURRENCE */
										break;
									case 3: /* LAST OCCURRENCE */
										break;
									case 4: /* MAXIMUM */
										if(*accval->value.float_value<
											*v->value.integer_value)
										{
											*accval->value.float_value=
												*v->value.integer_value;
										}
										break;
									case 5: /* MINIMUM */
										if(*accval->value.float_value>
											*v->value.integer_value || accval->count==1)
										{
											*accval->value.float_value=
												*v->value.integer_value;
										}
										break;
									case 6: /* PREVIOUS OCCURRENCE */
										break;
									case 7: /* SUMMATION */
										*accval->value.float_value+=
											*v->value.integer_value;
										break;
									default:
										prterr("Error unrecognized accumulator type.");
										break;
								}
								break;
							case BUTTONS:
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
					}
				}
			}
		}
		}
		clear_runtime_virtuals(rpt);
		if(runrpt->diagrsrc!=NULL) 
		{
			tmprsrc=runrpt->diagrsrc;
			update_diagnostic(runrpt->diagrsrc,TRUE);
		}
		if(d->body_count_type==FALSE && (inc_body_count || d->count_invisible==TRUE))
		{
			++runrpt->body_count;
		}
		}
		ef=NXTNRD(runrpt->sortno,1);
	}
	ZERNRD(runrpt->sortno);
	for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
	{
		field=FLDNRD(runrpt->sortno,a->accname);
		if(field!=NULL)
		{
			for(x=0,accval=a->accvals;x<a->num;++x,++accval)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case VARIABLETEXT:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						switch(a->atype)
						{
							case 0: /* AVERAGE */
							case 1: /* FREQUENCY */
								break;
							case 3: /* LAST OCCURRENCE */
								if(accval->prev_value.string_value!=NULL) Rfree(accval->prev_value.string_value);
								accval->prev_value.string_value=
									accval->value.string_value;
								accval->pdlen=accval->dlen;
								accval->value.string_value=NULL;
								accval->dlen=0;
								if(!isEMPTY(field->data.string_value))
								{
									QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
									memcpy(accval->value.string_value,field->data.string_value,field->dlen);
								}
								break;
							case 2: /* FIRST OCCURRENCE */
								if(!isEMPTY(field->data.string_value))
								{
									QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
									memcpy(accval->value.string_value,field->data.string_value,field->dlen);
								} else if(accval->dlen>0)
								{ 
									memset(accval->value.string_value,0,accval->dlen);
								} else {
									accval->value.string_value=NULL;
									accval->dlen=0;
								}
								if(!accval->count)
								{
									if(!isEMPTY(accval->value.string_value))
									{
										QUICKALLOC(accval->prev_value.string_value,accval->pdlen,accval->dlen);
										memcpy(accval->prev_value.string_value,accval->value.string_value,accval->dlen);
									} else if(accval->pdlen>0)
									{ 
										memset(accval->prev_value.string_value,0,accval->pdlen);
									} else {
										accval->prev_value.string_value=NULL;
										accval->pdlen=0;
									}
								}
								break;
							case 6: /* PREVIOUS OCCURRENCE */
								if(!isEMPTY(field->data.string_value))
								{
									QUICKALLOC(accval->value.string_value,accval->dlen,field->dlen);
									memcpy(accval->value.string_value,field->data.string_value,field->dlen);
								} else if(accval->dlen>0)
								{ 
									memset(accval->value.string_value,0,accval->dlen);
								} else {
									accval->value.string_value=NULL;
									accval->dlen=0;
								}
								break;
							case 4: /* MAXIMUM */
							case 5: /* MINIMUM */
							case 7: /* SUMMATION */
								break;
							default:
								prterr("Error unrecognized accumulator type.");
								break;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						switch(a->atype)
						{
							case 0: /* AVERAGE */
							case 1: /* FREQUENCY */
								break;
							case 3: /* LAST OCCURRENCE */
								*accval->prev_value.float_value=
								*accval->value.float_value;
								*accval->value.float_value=
									*field->data.string_value;
								break;
							case 2: /* FIRST OCCURRENCE */
								*accval->value.float_value=*field->data.string_value;
								if(!accval->count)
								{
									*accval->prev_value.float_value=*accval->value.float_value;
								}
								break;
							case 6: /* PREVIOUS OCCURRENCE */
								*accval->value.float_value=*field->data.string_value;
								break;
							case 4: /* MAXIMUM */
							case 5: /* MINIMUM */
							case 7: /* SUMMATION */
								break;
							default:
								prterr("Error unrecognized accumulator type.");
								break;
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						switch(a->atype)
						{
							case 0: /* AVERAGE */
							case 1: /* FREQUENCY */
								break;
							case 2: /* FIRST OCCURRENCE */
								*accval->value.float_value=*field->data.float_value;
								if(!accval->count)
								{
									*accval->prev_value.float_value=*accval->value.float_value;
								}
								break;
							case 3: /* LAST OCCURRENCE */
								*accval->prev_value.float_value=*accval->value.float_value;
								*accval->value.float_value=
									*field->data.float_value;
								break;
							case 4: /* MAXIMUM */
							case 5: /* MINIMUM */
							case 7: /* SUMMATION */
								break;
							case 6: /* PREVIOUS OCCURRENCE */
								*accval->value.float_value=*field->data.float_value;
								break;
							default:
								prterr("Error unrecognized accumulator type.");
								break;
						}
						break;
					case SHORTV:
					case SSHORTV:
						switch(a->atype)
						{
							case 0: /* AVERAGE */
							case 1: /* FREQUENCY */
								break;
							case 2: /* FIRST OCCURRENCE */
								*accval->value.float_value=*field->data.short_value;
								if(!accval->count)
								{
									*accval->prev_value.float_value=*accval->value.float_value;
								}
								break;
							case 3: /* LAST OCCURRENCE */
								*accval->prev_value.float_value=
									*accval->value.float_value;
								*accval->value.float_value=
									*field->data.short_value;
								break;
							case 4: /* MAXIMUM */
							case 5: /* MINIMUM */
								break;
							case 6: /* PREVIOUS OCCURRENCE */
								*accval->value.float_value=
									*field->data.short_value;
								break;
							case 7: /* SUMMATION */
								break;
							default:
								prterr("Error unrecognized accumulator type.");
								break;
						}
						break;
					case SLONGV:
					case LONGV:
					case SCROLLEDLIST:
						switch(a->atype)
						{
							case 0: /* AVERAGE */
							case 1: /* FREQUENCY */
								break;
							case 2: /* FIRST OCCURRENCE */
								*accval->value.float_value=*field->data.integer_value;
								if(!accval->count)
								{
									*accval->prev_value.float_value=*accval->value.float_value;
								}
								break;
							case 3: /* LAST OCCURRENCE */
								*accval->prev_value.float_value=
									*accval->value.float_value;
								*accval->value.float_value=
									*field->data.integer_value;
								break;
							case 4: /* MAXIMUM */
							case 5: /* MINIMUM */
								break;
							case 6: /* PREVIOUS OCCURRENCE */
								*accval->value.float_value=*field->data.integer_value;
								break;
							case 7: /* SUMMATION */
								break;
							default:
								prterr("Error unrecognized accumulator type.");
								break;
						}
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error unrecognized field type.");
						break;
				}
			}
		} else {
			temp=stripmodulename(a->accname);
			for(z=0,v=rpt->virflds;z<rpt->numvirtuals;++z,++v)
			{
				if(!RDAstrcmp(v->name,temp)) break;
			}
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			if(z<rpt->numvirtuals)
			{
				if(!v->computed) 
				{
					h=HoldReportNew(runrpt,rpt);
					v->computed=TRUE;
					tempvalue=NULL;
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
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(v->range==2)
							{
								if(runrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSTRING(runrpt->rsrsrc,v->name,
									&v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(!isEMPTY(tempvalue))
								{
									if(v->value.string_value!=NULL) Rfree(v->value.string_value);
									v->value.string_value=stralloc(tempvalue);
									v->dlen=RDAstrlen(v->value.string_value)+1;
									
								} else if(v->value.string_value!=NULL)
								{
									memset(v->value.string_value,0,v->dlen);
								} else {
									v->value.string_value=NULL;
									v->dlen=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
								temp=PP_EVALUATEstr(v->expression,
									ReportGenSubData1,h);
								if(v->value.string_value!=NULL) 
									Rfree(v->value.string_value);
								v->value.string_value=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
							}
							if(v->value.string_value!=NULL)
							{
								v->dlen=RDAstrlen(v->value.string_value)+1;
								if(v->dlen>v->len && v->len>0)
								{
									v->value.string_value[v->len]=0;
								}
							} else v->dlen=0;
							break;
						case SHORTV:
						case SSHORTV:
							if(v->range==2)
							{
								if(runrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSHORT(runrpt->rsrsrc,v->name,
									v->value.short_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.short_value=Rmalloc(sizeof(short));
									v->dlen=sizeof(short);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.short_value=Ratoi(tempvalue);
								} else {
									*v->value.short_value=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
							*v->value.short_value=PP_EVALUATEsht(v->expression,
								ReportGenSubData1,h);
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(v->range==2)
							{
								if(runrpt->rsrsrc!=NULL)
								{
								FINDRSCGETINT(runrpt->rsrsrc,v->name,
									v->value.integer_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.integer_value=Rmalloc(sizeof(int));
									v->dlen=sizeof(int);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.integer_value=Ratoi(tempvalue);
								} else {
									*v->value.integer_value=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
							*v->value.integer_value=PP_EVALUATEint(v->expression,
								ReportGenSubData1,h);
							}
							break;
						case BOOLNS:
							if(v->range==2)
							{
								if(runrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
									v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								if(!isEMPTY(tempvalue))
								{
									if(Ratoi(tempvalue))
									{
										*v->value.string_value=TRUE;
									} else {
										*v->value.string_value=FALSE;
									}
								} else {
									*v->value.string_value=FALSE;
								}
							}
							if(!isEMPTY(v->expression))
							{
							*v->value.string_value=PP_EVALUATEbol(v->expression,
								ReportGenSubData1,h);
							}
							break;
						case CHARACTERS:
							if(v->range==2)
							{
								if(runrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(runrpt->rsrsrc,v->name,
									v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.string_value=*tempvalue;
								} else {
									*v->value.string_value=*tempchar;
								}
							}
							if(!isEMPTY(v->expression))
							{
							*v->value.string_value=PP_EVALUATEbol(v->expression,
								ReportGenSubData1,h);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(v->range==2)
							{
								if(runrpt->rsrsrc!=NULL)
								{
								FINDRSCGETDOUBLE(runrpt->rsrsrc,v->name,
									v->value.float_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.float_value=Rmalloc(sizeof(double));
									v->dlen=sizeof(double);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.float_value=atof(tempvalue);
								} else {
									*v->value.float_value=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
							*v->value.float_value=PP_EVALUATEdbl(v->expression,
								ReportGenSubData1,h);
							if(v->type==DOLLARS || v->type==DOUBLEV ||
								v->type==SDOUBLEV ||
								v->type==DOLLARS_NOCENTS)
							{
								*v->value.float_value=round(*v->value.float_value);
							}
							}
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
					if(h!=NULL) Rfree(h);
					h=NULL;
				}
				for(x=0,accval=a->accvals;x<a->num;++x,++accval)
				{
					switch(v->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 3: /* LAST OCCURRENCE */
									if(accval->prev_value.string_value!=NULL) Rfree(accval->prev_value.string_value);
									accval->prev_value.string_value=
										accval->value.string_value;
									accval->pdlen=accval->dlen;
									accval->value.string_value=NULL;
									accval->dlen=0;
									if(!isEMPTY(v->value.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
										memcpy(accval->value.string_value,v->value.string_value,v->dlen);
									}
									break;
								case 2: /* FIRST OCCURRENCE */
									if(!isEMPTY(v->value.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
										memcpy(accval->value.string_value,v->value.string_value,v->dlen);
									} else if(accval->dlen>0)
									{
										memset(accval->value.string_value,0,accval->dlen+1);
									} else {
										accval->value.string_value=NULL;
										accval->dlen=0;
									}
									if(!accval->count)
									{
										if(!isEMPTY(accval->value.string_value))
										{
											QUICKALLOC(accval->prev_value.string_value,accval->pdlen,accval->dlen);
											memcpy(accval->prev_value.string_value,accval->value.string_value,accval->dlen);
										} else if(accval->pdlen>0)
										{ 
											memset(accval->prev_value.string_value,0,accval->pdlen);
										} else {
											accval->prev_value.string_value=NULL;
											accval->pdlen=0;
										}
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									if(!isEMPTY(v->value.string_value))
									{
										QUICKALLOC(accval->value.string_value,accval->dlen,v->dlen);
										memcpy(accval->value.string_value,v->value.string_value,v->dlen);
									} else if(accval->dlen>0)
									{ 
										memset(accval->value.string_value,0,accval->dlen);
									} else {
										accval->value.string_value=NULL;
										accval->dlen=0;
									}
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
									*accval->value.float_value;
									*accval->value.float_value=
										*v->value.string_value;
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=*v->value.string_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=*accval->value.float_value;
									}
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=*v->value.string_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=*v->value.float_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=*accval->value.float_value;
									*accval->value.float_value=
										*v->value.float_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=*v->value.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SHORTV:
						case SSHORTV:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=*v->value.short_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*v->value.short_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=
										*v->value.short_value;
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case SLONGV:
						case LONGV:
						case SCROLLEDLIST:
							switch(a->atype)
							{
								case 0: /* AVERAGE */
								case 1: /* FREQUENCY */
									break;
								case 2: /* FIRST OCCURRENCE */
									*accval->value.float_value=*v->value.integer_value;
									if(!accval->count)
									{
										*accval->prev_value.float_value=*accval->value.float_value;
									}
									break;
								case 3: /* LAST OCCURRENCE */
									*accval->prev_value.float_value=
										*accval->value.float_value;
									*accval->value.float_value=
										*v->value.integer_value;
									break;
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
									break;
								case 6: /* PREVIOUS OCCURRENCE */
									*accval->value.float_value=*v->value.integer_value;
									break;
								case 7: /* SUMMATION */
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
				}
			}
		}
	}
/* Increment Total Number Records & Update Screen */
	if(abort_diagnostic)
	{
		prterr("PROCESS TERMINATED BY USER.");
	}
	if(rpt->control!=NULL)
	{
		for(y=rpt->numcontrols;y>0;--y)
		{
			c1=rpt->control+(y-1);
			temp=Rmalloc(RDAstrlen(c1->name)+8);
			sprintf(temp,"%s FOOTER",c1->name);
			print_section(runrpt,rpt,temp,FALSE);
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
		}
	}
/*
	if(d->page_length>0.0)
	{
		runrpt->forced=TRUE;
		print_section(runrpt,rpt,"PAGE FOOTER",FALSE);
		runrpt->forced=FALSE;
	}
*/
	print_section(runrpt,rpt,"REPORT FOOTER",FALSE);
/* Close Sort File */
	if(runrpt->sortno!=(-1)) CLSNRD(runrpt->sortno);
/* Add Extra Formfeed */
	if(d->extra_formfeed)
	{
		if(d->device<3 || d->device==5)
		{
			if(d->device==0 || d->device==5)
			{
				RDA_fprintf(runrpt->pfile,"\f");
#ifndef	 WIN32
				if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
			} else {
				fprintf(runrpt->fp,"\f");
				fflush(runrpt->fp);
			}
		}
	}
/* Close Output Device */
	switch(d->device)
	{
		default:
		case 0: /* printer */
		case 5: /* popen */
/* SIMPLE PRINTER */
			if(runrpt->print_range_criteria==TRUE && !isEMPTY(runrpt->range_desc))
			{
				RDA_fprintf(runrpt->pfile,"\f%s",runrpt->range_desc);
			}
			if(rpt->display->dumprinter==TRUE)
			{
				if((rpt->display->dumprinter_type==1) || (rpt->display->dumprinter_type==3))
				{
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"NORMAL TEXT")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
				}
				if((rpt->display->dumprinter_type==2) || (rpt->display->dumprinter_type==3))
				{
					if(rpt->display->pvars!=NULL)
					{
						for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
						{
							if(!RDAstrcmp(Pvar->name,"PORTRAIT")) break;
						}
						if(u<rpt->display->num)
						{
							HoldString=Rmalloc(256);
							print_font(runrpt,rpt,Pvar->name,&HoldString);
							RDA_fprintf(runrpt->pfile,"%s",HoldString);
							if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
							if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
						}
					}
				}
			}
			if(rpt->display->pvars!=NULL)
			{
				for(u=0,Pvar=rpt->display->pvars;u<rpt->display->num;++u,++Pvar)
				{
					if((!RDAstrcmp(Pvar->name,"PCL JOB FOOTER"))) break;
				}
				if(u<rpt->display->num)
				{
					HoldString=Rmalloc(256);
					print_font(runrpt,rpt,Pvar->name,&HoldString);
					RDA_fprintf(runrpt->pfile,"%s",HoldString);
					if(HoldString!=NULL) Rfree(HoldString);
#ifndef WIN32
					if(runrpt->fp!=NULL) fflush(runrpt->fp);
#endif
				}
			}
			if(d->device==5) runrpt->pfile->wrote_something=FALSE;
			RDA_pclose(runrpt->pfile);
			if(runrpt->archive==TRUE && runrpt->fp_pdf!=NULL)
			{
				if(runrpt->print_range_criteria==TRUE)
				{
					fprintf(runrpt->fp_pdf,"\f%s",runrpt->range_desc);
				}
				pclose(runrpt->fp_pdf);
				runrpt->fp_pdf=NULL;
				if((!strncmp(rpt->display->set_lpp,"AUTOARCHIVE",11)))
				{
/* HERE */					
					HoldReportx=HoldReportNew(runrpt,rpt);
					temp=ArchiveNameToGet(HoldReportx,"DIRECTORY NAME OF ARCHIVE");
					temp1=ArchiveNameToGet(HoldReportx,"FILENAME OF ARCHIVE");
					if(isEMPTY(temp) || isEMPTY(temp1))
					{
						ArchiveSource=6;
						ArchiveReport(runrpt,rpt);
					} else {
						RDAmkdir(temp,00770,&ret_int);
						memset(Xstemp,0,1024);
						memset(movefile,0,1024);
						sprintf(stemp,"%d.pdf",RGETPID());
#ifndef WIN32
						sprintf(movefile,"%s/%s.pdf",temp,temp1);
#endif
#ifdef WIN32
						sprintf(movefile,"%s\\%s.pdf",temp,temp1);
#endif
						RDAMoveFile(stemp,movefile);

						if(args!=NULL) freeapplib(args);
					}
					if(HoldReportx!=NULL) Rfree(HoldReportx);
					HoldReportx=NULL;
				}
			}
			break;
		case 1: /* ascii file */
		case 2: /* append ascii file */
			fclose(runrpt->fp);
			if(runrpt->archive==TRUE && runrpt->fp_pdf!=NULL)
			{
				fclose(runrpt->fp_pdf);
				runrpt->fp_pdf=NULL;
				if(runrpt->archive==TRUE)
				{
					ArchiveReport(runrpt,rpt);
				}
			}
			break;
		case 3: /* datafile */
		case 4: /* update datafile */
			CLSReportfiles(runrpt,TRUE);
			break;
	}
/* if chain_report then run next report */
	if(runrpt->diagrsrc!=NULL)
	{
		if(!abort_diagnostic) killwindow(runrpt->diagrsrc);
		free_rsrc(runrpt->diagrsrc);
		runrpt->diagrsrc=NULL;
	}
	if(rpt->remove_file==1)
	{
		if(unlink(rpt->remove_name)==(-1)) if(errno!=ENOENT) { prterr("Error can't remove file(%s)%s",rpt->remove_name,errname()); }
	}
	HoldReportx=HoldReportNew(runrpt,rpt);
	if(d->doscopy)
	{
		report_run_dos_copy(runrpt->rsrsrc,HoldReportx);
	} else {
		run_report_chain(runrpt->rsrsrc,HoldReportx);
	}
	return;
}
void run_report_chain(RDArsrc *parent,HoldReport *h)
{
	RDAreport *rpt=NULL;
	RDArunrpt *runrpt=NULL;
	HoldReport *HoldReportx=NULL;
	RDArsrc *tmprsrc=NULL;
	int errorcount=0;
	int x=0,q=0,tmp_has_errors=0;
	char *temp=NULL,chained=FALSE;
	APPlib *args=NULL;
	int ret_int=0;

	if(h!=NULL)
	{
	rpt=h->rpt;
	runrpt=h->rrpt;
	if(rpt->chain_report)
	{
		chained=TRUE;
		if(rpt->ask_b4_execute)
		{
			runrpt->askb4exec=RDArsrcNEW("RPTGEN","RUN REPORT CHAIN WINDOW");
			addstdrsrc(runrpt->askb4exec,"MODULE",VARIABLETEXT,0,rpt->chain_module,FALSE);
			addstdrsrc(runrpt->askb4exec,"REPORT",VARIABLETEXT,0,rpt->chain_name,FALSE);
			temp=getreporttitle(rpt->chain_module,rpt->chain_name);
			addstdrsrc(runrpt->askb4exec,"TITLE",VARIABLETEXT,0,temp,FALSE);
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			temp=getreportdesc(rpt->chain_module,rpt->chain_name);
			addsctrsrc(runrpt->askb4exec,"DESCRIPTION",0,temp,FALSE);
			errorcount=runrpt->errorlist->numlibs;
			if(runrpt->errorlist->numlibs<1)
			{
				addAPPlib(runrpt->errorlist,"No Errors Encountered");
				tmp_has_errors = 0;
			} else if(runrpt->errorlist->numlibs==1 && !RDAstrcmp(runrpt->errorlist->libs[0],"No Errors Encountered"))
			{
				tmp_has_errors = 0;
			} else {
				tmp_has_errors = 1;
			}
			addstdrsrc(runrpt->askb4exec,"ERROR COUNT",LONGV,10,&tmp_has_errors,FALSE);
			x=0;
			addlstrsrc(runrpt->askb4exec,"ERROR LIST",&x,TRUE,NULL,runrpt->errorlist->numlibs,&runrpt->errorlist->libs,NULL);
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			HoldReportx=HoldReportNew(runrpt,rpt);
			addbtnrsrc(runrpt->askb4exec,"RUN REPORT",TRUE,execnextrpt,HoldReportx);
			/* If there are errors, disable the RUN REPORT (Select) button */
			if(tmp_has_errors)
			{
				addbtnrsrc(runrpt->askb4exec,"RUN REPORT",TRUE,execnextrpt,HoldReportx);
				FINDRSCSETSENSITIVE(runrpt->askb4exec,"RUN REPORT",FALSE);
			}
			addbtnrsrc(runrpt->askb4exec,"PRINT ERROR LIST",TRUE,print_list,HoldReportx);
			if(runrpt->exit_status==1)
			{
				addrfexrsrc(runrpt->askb4exec,"QUIT",TRUE,quitaskrsrc1,HoldReportx);
			} else {
				addrfkwrsrc(runrpt->askb4exec,"QUIT",TRUE,quitaskrsrc,HoldReportx);
			}
			addbtnrsrc(runrpt->askb4exec,"HELP",TRUE,screenhelp,NULL);
			addbtnrsrc(runrpt->askb4exec,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
			++runrpt->screen_count;
			if(ADVmakescrn(runrpt->askb4exec,TRUE))
			{
				ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen RUN REPORT CHAIN WINDOW. Check to see the screen is available. If it is, call your installer.",NULL,TRUE);
				quitaskrsrc(runrpt->askb4exec,HoldReportx);
				prterr("Error can't make screen RUN REPORT DEVICE WINDOW.");
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen_outall)
			{
				prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
				diagrptgen=FALSE;
				diageval=FALSE;
			} else if(diagrptgen_output)
			{
				prterr("DIAG Report Generator Diagnostics Disabled.");
				diagrptgen=FALSE;
			} else if(diagrptgen_outexp)
			{
				prterr("DIAG Expression Parser Diagnostics Disabled.");
				diageval=FALSE;
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(!runrpt->APPmainLoop) 
			{
				runrpt->APPmainLoop=TRUE;
				return;
			}
			return;
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen_outall)
			{
				prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
				diagrptgen=FALSE;
				diageval=FALSE;
			} else if(diagrptgen_output)
			{
				prterr("DIAG Report Generator Diagnostics Disabled.");
				diagrptgen=FALSE;
			} else if(diagrptgen_outexp)
			{
				prterr("DIAG Expression Parser Diagnostics Disabled.");
				diageval=FALSE;
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			tmprsrc=runrpt->rsrsrc;
			runrpt->rsrsrc=NULL;
			if(!rpt->removesort)
			{
				RUNREPORTADV3(rpt->chain_module,rpt->chain_name,runrpt->sortserver,runrpt->sortfile,runrpt->APPmainLoop,runrpt->exit_status,tmprsrc,NULL,NULL,NULL,0,runrpt->ProcDataLine);

			} else {
				exit_cleanly(runrpt,rpt,FALSE,NULL,NULL,TRUE,1);
				RUNREPORTADV3(rpt->chain_module,rpt->chain_name,NULL,NULL,runrpt->APPmainLoop,runrpt->exit_status,tmprsrc,NULL,NULL,NULL,0,runrpt->ProcDataLine);
			}
			if(runrpt!=NULL) FreeRDArunrpt(runrpt);
			if(rpt!=NULL) FreeRDAreport(rpt);
		}
	} else {		
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_outall)
		{
			prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
			diagrptgen=FALSE;
			diageval=FALSE;
		} else if(diagrptgen_output)
		{
			prterr("DIAG Report Generator Diagnostics Disabled.");
			diagrptgen=FALSE;
		} else if(diagrptgen_outexp)
		{
			prterr("DIAG Expression Parser Diagnostics Disabled.");
			diageval=FALSE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		q=runrpt->exit_status;
		if(rpt->report_completion)
		{
			args=APPlibNEW();
			memset(stemp,0,101);
			sprintf(stemp,"%s.RPT",rpt->name);
			addAPPlib(args,stemp);
			addAPPlib(args,"Has entirely completed, please acknowledge.");
			ExecuteProgram("ok",args);
			if(args!=NULL) freeapplib(args);
		}
		if(runrpt->askb4exec==FALSE)
		{
			exit_cleanly(runrpt,rpt,q,NULL,NULL,(rpt->removesort==TRUE?TRUE:FALSE),1);
		} else {
			exit_cleanly(runrpt,rpt,q,NULL,NULL,TRUE,1);
		}
		/* REPORT_COMPELTION HERE */
	}
	}
	return;
}
void quitdoscopy(RDArsrc *parent,HoldReport *h)
{
	run_report_chain(parent,h);
}
void report_run_dos_copy(RDArsrc *parent,HoldReport *h)
{
	RDAreport *rpt=NULL;
	RDArunrpt *rrpt=NULL;
	HoldReport *HoldReportx=NULL;
	RDAdisplay *d=NULL;

	if(h!=NULL)
	{
		rpt=h->rpt;
		rrpt=h->rrpt;
		if(rpt!=NULL && rrpt!=NULL)
		{
			d=rpt->display;
			if(d!=NULL)
			{
				HoldReportx=HoldReportNew(rrpt,rpt);
				run_dos_copy(parent,rpt->name,d->device_spec,d->set_lpp,d->set_lpp,rrpt->APPmainLoop,rrpt->errorlist,quitdoscopy,HoldReportx,(rpt->remove_file==2 ? rpt->remove_name:""));
			} else {
			}
		} else {
		}
	} else {
	}
	return;
}
static void dump_record_names(FILE *fp,short fileno)
{
	NRDfield *field=NULL,*fields=NULL;
	short numflds=0;
	int x=0;

	numflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<numflds;++x,++field)
		{
			fprintf(fp,"\"%s\"",(field->name!=NULL ? field->name:""));
			if((x+1)<numflds)
			{
				fprintf(fp,",");
			}
		}
	}
	fprintf(fp,"\r\n");
}
static void dump_record(FILE *fp,short fileno)
{
	NRDfield *field=NULL,*fields=NULL;
	short numflds=0;
	char *temp=NULL;
	int x=0;

	numflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<numflds;++x,++field)
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
					fprintf(fp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
					break;
				case BOOLNS:
					fprintf(fp,"\"%s\"",(*field->data.string_value ? "Yes":"No"));
					break;
				case CHARACTERS:
					fprintf(fp,"\"%c\"",*field->data.string_value);
					break;
				case DOLLARS:
					temp=famtncomma(*field->data.float_value,20,' ');
					fprintf(fp,"\"%s\"",(temp!=NULL ? temp:""));
					if(temp!=NULL) Rfree(temp);
					break;
				case DOLLARS_NOCENTS:
					temp=famtncnc(*field->data.float_value,20,' ');
					fprintf(fp,"\"%s\"",(temp!=NULL ? temp:""));
					if(temp!=NULL) Rfree(temp);
					break;
				case DECIMALV:
				case SDECIMALV:
					fprintf(fp,"\"%.09f\"",*field->data.float_value);
					break;
				case DOUBLEV:
				case SDOUBLEV:
					fprintf(fp,"\"%.0f\"",*field->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					fprintf(fp,"\"%d\"",*field->data.short_value);
					break;
				case LONGV:
				case SLONGV:
					fprintf(fp,"\"%d\"",*field->data.integer_value);
					break;
				default:
					break;
			}
			if((x+1)<numflds)
			{
				fprintf(fp,",");
			}
		}
	}
	fprintf(fp,"\r\n");
}
void dump_csv(RDArsrc *p,HoldReport *h)
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	FILE *fp=NULL;
	RDArsrc *tmprsrc=NULL;
	short ef=FALSE;
	char deleteflag=FALSE;
	char *aname=NULL,*desc=NULL,*fname=NULL;
	char *temp1=NULL,*c=NULL,*t=NULL,*wgdir=NULL;
	char *dname=NULL,*temp=NULL;
	RDATData *prev=NULL;
	int aclass=1;
	char *subject=NULL;
	short a_fileno=(-1),archwork=(-1);
#ifdef ARCHIVE_GSV
	char *libx=NULL,*workgroup=NULL;
	RDAGenericSetup *gsv=NULL;
#endif /* ARCHIVE_GSV */
	int ret_int=0;
	

	rpt=h->rpt;
	rrpt=h->rrpt;
	ZERNRD(rrpt->sortno);
	memset(stemp,0,101);
	sprintf(stemp,"%s.csv",rrpt->sortfile);
	fp=fopen(stemp,"w+b");
	dname=stralloc(stemp);
	abort_diagnostic=FALSE;
	if(fp!=NULL)
	{
		tmprsrc=diagscrn(rrpt->sortno,"DIAGNOSTIC SCREEN","RPTGEN","CSV DUMP",(rpt->diag_breakable ? stop_diagnostic:NULL));
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that the screen is available in the module.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
				 prterr("Error Cannot Create Diagnostic Screen.");
			}
		}
		dump_record_names(fp,rrpt->sortno);
		ef=FRSNRD(rrpt->sortno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(rrpt->sortno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				dump_record(fp,rrpt->sortno);
				update_diagnostic(tmprsrc,TRUE);
			}
			ef=NXTNRD(rrpt->sortno,1);
		}
		if(tmprsrc!=NULL)
		{
			if(!abort_diagnostic) killwindow(tmprsrc);
			free_rsrc(tmprsrc);
			tmprsrc=NULL;
		}
		fclose(fp);
	} else {
		ERRORDIALOG("CSV DUMP FAILED","Creating the temporary csv file failed.  Unable to dump file at this time.",NULL,FALSE);
	}
	ZERNRD(rrpt->sortno);
	a_fileno=APPOPNNRD("ARCHIVE","ARCHIVES",TRUE,TRUE);
	archwork=APPOPNNRD("ARCHIVE","ARCHWORK",TRUE,FALSE);
	if((a_fileno==(-1)) || (archwork==(-1)))
	{
		prterr("Error:  Unable to Archive Database files.");
		unlink(dname);
		return;
	}
	aname=MakeArchiveName(rrpt->module,rpt->name);
	temp1=adddashes(aname);
	ZERNRD(archwork);
	FINDFLDSETSTRING(archwork,"WORK GROUP",rrpt->module);
	if(EQLNRD(archwork,1)) KEYNRD(archwork,1);
	FINDFLDGETSTRING(archwork,"DIRECTORY",&wgdir);
	stoupper(temp1);
	fname=Rmalloc(RDAstrlen(temp1)+5);
	sprintf(aname,"%s.csv",temp1);
	fname=stralloc(aname);
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
#ifndef WIN32
	sprintf(stemp,"ARCHIVE/.%s",(rrpt->module!=NULL ? rrpt->module:""));
#endif
#ifdef WIN32
	sprintf(stemp,"ARCHIVE\\.%s",(rrpt->module!=NULL ? rrpt->module:""));
#endif
	RDAmkdir(stemp,00770,&ret_int);
	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"ARCHIVE/.%s/%s",rrpt->module,fname);
#endif
#ifdef WIN32
	sprintf(stemp,"ARCHIVE\\.%s\\%s",rrpt->module,fname);
#endif
	RDAMoveFile(dname,stemp);
	
	ZERNRD(a_fileno);
#ifdef ARCHIVE_GSV
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(rrpt->module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,rrpt->module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,rrpt->module);
#endif

	gsv=RDAGenericSetupNew(rrpt->module,"AUTOMAGICAL WORK GROUP");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			workgroup=stralloc(gsv->value.string_value);
		} else {
			workgroup=stralloc(rrpt->module);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",workgroup);
#endif /* ARCHIVE_GSV */
#ifndef ARCHIVE_GSV
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",rrpt->module);
#endif /* ARCHIVE_GSV */
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	if(!EQLNRD(a_fileno,1))
	{
		prev=RDATDataNEW(a_fileno);
	} else KEYNRD(a_fileno,1);
	temp1=Users_Department();
	if(!RDAstrcmp(rrpt->module,"FINMGT") || !RDAstrcmp(rrpt->module,"BNKREC")) subject=stralloc("FINANCE");
	else if(!RDAstrcmp(rrpt->module,"VENPMT")) subject=stralloc("A/P");
	else if(!RDAstrcmp(rrpt->module,"PURORD") || !RDAstrcmp(rrpt->module,"FIXASS") || 
		!RDAstrcmp(rrpt->module,"CATALOGUE") ||
		!RDAstrcmp(rrpt->module,"REQMGT")) subject=stralloc("PURCHASING");
	else if(!RDAstrcmp(rrpt->module,"PAYROLL") || 
		!RDAstrcmp(rrpt->module,"LVEMGT") || !RDAstrcmp(rrpt->module,"EMPABS") || !RDAstrcmp(rrpt->module,"SUBMGT") || !RDAstrcmp(rrpt->module,"POSTRK") || 
		!RDAstrcmp(rrpt->module,"LVSIMP")) subject=stralloc("PAYROLL");
	else if(!RDAstrcmp(rrpt->module,"PRSNNL") || !RDAstrcmp(rrpt->module,"IIDMGT") || !RDAstrcmp(rrpt->module,"JOBPERF") || !RDAstrcmp(rrpt->module,"APPMGT")) subject=stralloc("PERSONNEL");
	else if(!RDAstrcmp(rrpt->module,"BUDPREP")) subject=stralloc("BUDGET");
	else if(RDAstrstr(temp1,"TREASURE")) subject=stralloc("TREASURER");
	else if(RDAstrstr(temp1,"COMM")) subject=stralloc("COMMISSIONER");
	else subject=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	FINDFLDSETCHAR(a_fileno,"DELETEFLAG",FALSE);
#ifdef ARCHIVE_GSV
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",workgroup);
#endif /* ARCHIVE_GSV */
#ifndef ARCHIVE_GSV
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",rrpt->module);
#endif /* ARCHIVE_GSV */
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	FINDFLDSETSTRING(a_fileno,"DESCRIPTION",rpt->title);
	FINDFLDSETSTRING(a_fileno,"SUBJECT",subject);
	FINDFLDSETSHORT(a_fileno,"SOURCE",ArchiveSource);
	FINDFLDSETSTRING(a_fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETSTRING(a_fileno,"FILENAME",fname);
	aclass=8;
	FINDFLDSETSTRING(a_fileno,"TYPE","csv");
	FINDFLDSETINT(a_fileno,"CLASS",aclass);
	c=GETCURRENTDATE10();
	FINDFLDSETSTRING(a_fileno,"DATE",c);
	if(c!=NULL) Rfree(c);
	t=GETCURRENTTIME();
	FINDFLDSETSTRING(a_fileno,"TIME",t);
	if(t!=NULL) Rfree(t);
	WRTTRANS(a_fileno,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);	
	if(fname!=NULL) Rfree(fname);
	if(desc!=NULL) Rfree(desc);
	if(aname!=NULL) Rfree(aname);
	if(dname!=NULL) Rfree(dname);
#ifdef ARCHIVE_GSV
	if(workgroup!=NULL) Rfree(workgroup);
#endif /* ARCHIVE_GSV */
	if(subject!=NULL) Rfree(subject);
	if(a_fileno!=(-1)) CLSNRD(a_fileno);
	if(archwork!=(-1)) CLSNRD(archwork);
	if(temp1!=NULL) Rfree(temp1);
}
