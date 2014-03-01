/* prttest.c - Print Test Pages */
#include<rptgen.hpp>

void print_test_pages(RDArsrc *parent,HoldReport *h)
{
	int z,y;
	RDAdisplay *d;
	RDArunrpt *rrpt;
	RDAreport *rpt;
	RDAcontrol *c;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG print_test_pages Printing Test Pages for Alignment.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rrpt=h->rrpt;
	rpt=h->rpt;
	d=rpt->display;
	rrpt->pageno=0;
	rrpt->page_count=(int)d->page_length*72;
	if(rrpt->curfont!=NULL) Rfree(rrpt->curfont);
	rrpt->curfont=NULL;
	if(rrpt->longfont!=NULL) Rfree(rrpt->longfont);
	rrpt->longfont=NULL;
	rrpt->curpoints=0;
	rrpt->curpitch=0;
	rrpt->points=0;
	rrpt->startline=FALSE;
	rrpt->pitch=0;
	rrpt->forced=FALSE;
	rrpt->body_count=0;
	rrpt->numlines=0;
	if(d->device==0 || d->device==5)
	{
		rrpt->pfile=RDA_popen(d->set_lpp,"w");
		rrpt->fp=NULL;
	} else {
		rrpt->pfile=NULL;
		if(d->device==1)
		{
			rrpt->fp=fopen(d->set_lpp,"w");
		} else {
			rrpt->fp=fopen(d->set_lpp,"a+");
		}
	}
	if(rrpt->pfile==NULL && rrpt->fp==NULL)
	{
		prterr("Error Couldn't open device [%s].",d->set_lpp);
		return;
	}
	while(rrpt->pageno<d->test_pages)
	{
		if(rpt->control!=NULL)
		{
			for(y=rpt->numcontrols;y>0;--y)
			{
				c=rpt->control+(y-1);
				temp=Rmalloc(RDAstrlen(c->name)+8);
				sprintf(temp,"%s HEADER",c->name);
				print_section(rrpt,rpt,temp,TRUE);
			}
		}
		if(rpt->display->body_count==0)
		{
				print_section(rrpt,rpt,"DETAIL LINES",TRUE);
		} else {
			for(z=0;z<rpt->display->body_count;++z)
			{
				print_section(rrpt,rpt,"DETAIL LINES",TRUE);
				++rrpt->body_count;
			}
		}
		if(rpt->control!=NULL)
		{
			for(y=rpt->numcontrols;y>0;--y)
			{
				c=rpt->control+(y-1);
				temp=Rmalloc(RDAstrlen(c->name)+8);
				sprintf(temp,"%s FOOTER",c->name);
				print_section(rrpt,rpt,temp,TRUE);
				if(rrpt->body_count>0) rrpt->body_count=0;
			}
		}
		if(rpt->display->page_length>0.0)
		{
			rrpt->forced=TRUE;
			print_section(rrpt,rpt,"PAGE FOOTER",TRUE);
			rrpt->forced=FALSE;
		}
		rrpt->body_count=0;
	}
	RDA_pclose(rrpt->pfile);
	rrpt->pageno=0;
	rrpt->page_count=(int)d->page_length*72;
	if(rrpt->curfont!=NULL) Rfree(rrpt->curfont);
	rrpt->curfont=NULL;
	if(rrpt->longfont!=NULL) Rfree(rrpt->longfont);
	rrpt->longfont=NULL;
	rrpt->curpoints=0;
	rrpt->curpitch=0;
	rrpt->points=0;
	rrpt->startline=FALSE;
	rrpt->pitch=0;
	rrpt->forced=FALSE;
	rrpt->body_count=0;
	rrpt->numlines=0;
}
