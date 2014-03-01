/* process exceptions function */
#include <rptgen.hpp>

static short procexcpselectfunction(void);
static void mtnexcpts(void *,HoldReport *);
static void procexcpsaverecord(RDArsrc *,HoldReport *,short);
static void procexcpquitrecord(RDArsrc *,HoldReport *);
static void procexcpdoexit(RDArsrc *,HoldReport *);
/*
static void procexcpdoexit(MakeBrowseList *);
*/
static void procexcpokfunc(MakeBrowseList *,void *);
static void procexcpnewrecord(RDArsrc *,MakeBrowseList *);
static void abort_procexcp(MakeBrowseList *);
static void doabort_procexcptest(MakeBrowseList *);
/*
static void abort_procexcp(RDArsrc *,HoldReport *);
*/
static void save_update(RDArsrc *,HoldReport *);
static void save_no_update(RDArsrc *,HoldReport *);

void process_exceptions(RDArunrpt *rrpt,RDAreport *rpt)
{
	HoldReport *h=NULL;
	char *namex=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG process_exceptions Process Exceptions on Report [%s]'s sort file [%s].",rpt->name,FILENAME(rrpt->sortno));  
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	namex=Rmalloc(RDAstrlen(rpt->name)+8);
	sprintf(namex,"%s BROWSE",rpt->name);
	rrpt->mainrsrc=RDArsrcNEW(rrpt->module,namex);
	if(namex!=NULL) Rfree(namex);
	namex=Rmalloc(RDAstrlen(rpt->name)+15);
	sprintf(namex,"%s SEARCH BROWSE",rpt->name);
	rrpt->searchrsrc=RDArsrcNEW(rrpt->module,namex);
	if(namex!=NULL) Rfree(namex);
	namex=Rmalloc(RDAstrlen(rpt->name)+13);
	sprintf(namex,"%s DEFINE LIST",rpt->name);
	rrpt->definelist=RDArsrcNEW(rrpt->module,namex);
	if(namex!=NULL) Rfree(namex);
	h=HoldReportNew(rrpt,rpt);
	if(rpt->display->device<3)
	{
		addbtnrsrc(rrpt->mainrsrc,"CONTINUE",TRUE,procexcpdoexit,h);
	} else {
		addbtnrsrc(rrpt->mainrsrc,"SAVE",TRUE,procexcpdoexit,h);
	}
/* before the doabort_procexcptest added 
	rrpt->mbl=ADVbrowse(rrpt->sortno,1,rrpt->mainrsrc,rrpt->searchrsrc,rrpt->definelist,NULL,procexcpselectfunction,abort_procexcp,procexcpokfunc,procexcpnewrecord,h,TRUE,NULL,0,0,1,TRUE);
*/
	rrpt->mbl=ADVbrowse(rrpt->sortno,1,rrpt->mainrsrc,rrpt->searchrsrc,rrpt->definelist,NULL,procexcpselectfunction,doabort_procexcptest,procexcpokfunc,procexcpnewrecord,h,TRUE,NULL,0,0,0,TRUE);
/*
	addrfexrsrc(rrpt->mainrsrc,"ABORT",TRUE,abort_procexcp,h);
	rrpt->mbl=ADVbrowse(rrpt->sortno,1,rrpt->mainrsrc,rrpt->searchrsrc,rrpt->definelist,NULL,procexcpselectfunction,procexcpdoexit,procexcpokfunc,procexcpnewrecord,h,TRUE,NULL,0,0,1,TRUE);
*/
	++rrpt->screen_count;
}
static void doabort_procexcptest(MakeBrowseList *blist)
{
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	HoldReport *h=NULL;
	char *warnmessage=NULL;

	h=blist->funcargs;
	rrpt=h->rrpt;
	rpt=h->rpt;
	if(rpt!=NULL)
	{
		if(rrpt->sortno!=(-1))
		{
			warnmessage=Rmalloc(RDAstrlen(rrpt->module)+RDAstrlen(rpt->name)+160+1);
			sprintf(warnmessage,"Are you sure that you want to quit/abort this report without processing this data?\n  !!!ALL DATA PROCESSED WILL BE LOST IF ABORTED!!!\nModule: [%s] Report: [%s]",
				(rrpt->module==NULL?"":rrpt->module),
				(rpt->name=NULL?"":rpt->name));
			WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!",
				warnmessage,abort_procexcp,NULL,FALSE,blist);
			if(warnmessage!=NULL) Rfree(warnmessage);
		} else {
			abort_procexcp(blist);
		}
	}
}
/*
static void abort_procexcp(RDArsrc *parent,HoldReport *h)
*/
static void abort_procexcp(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDArunrpt *rrpt;
	RDAreport *rpt;
	HoldReport *h=NULL;
	int wcount=1;
	Browse_Exit *BE=NULL;

	if(blist!=NULL)
	{
		h=blist->funcargs;
		rrpt=h->rrpt;
		rpt=h->rpt;
		drsrc=blist->definelist;
		srsrc=blist->searchrsrc;
		brsrc=blist->mainrsrc;
		BE=RETURN_BROWSE_EXIT(blist);
		if(drsrc!=NULL) 
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
			drsrc=NULL;
			rrpt->definelist=NULL;
		}
		if(srsrc!=NULL) 
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
			srsrc=NULL;
			rrpt->searchrsrc=NULL;
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			free_rsrc(brsrc);
			brsrc=NULL;
			rrpt->mainrsrc=NULL;
		}
		if(rpt->range_screen && IsScreenDisplayed(rrpt->rsrsrc)) ++wcount;
		if(rrpt->screen_count==wcount) 
		{
			quitreport(brsrc,h);
			FreeBrowseExit(BE);
			ShutdownSubsystems();
		} else {
			FreeBrowseExit(BE);
			--rrpt->screen_count;
		}
	}
}
static short procexcpselectfunction(void)
{
	return TRUE;
}
/*
static void procexcpdoexit(MakeBrowseList *blist)
*/
static void procexcpdoexit(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;
/*
	HoldReport *h=NULL;
*/


#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpdoexit Exiting Process Exceptions.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
/*
	h=blist->funcargs;
*/
	if(h!=NULL)
	{
		rrpt=h->rrpt;
		if(rrpt->definelist!=NULL) 
		{
			killwindow(rrpt->definelist);
			free_rsrc(rrpt->definelist);
			rrpt->definelist=NULL;
		}
		if(rrpt->searchrsrc!=NULL) 
		{
			killwindow(rrpt->searchrsrc);
			free_rsrc(rrpt->searchrsrc);
			rrpt->searchrsrc=NULL;
		}
/* SJSFIX???
		if(rrpt->mainrsrc!=NULL)
		{
			killwindow(rrpt->mainrsrc);
			free_rsrc(rrpt->mainrsrc);
			rrpt->mainrsrc=NULL;
		}
*/
		if(rrpt->mainrsrc!=NULL) free_rsrc(rrpt->mainrsrc);
		rrpt->mainrsrc=NULL;
		runprtscreen(h);
	}
}
static void procexcpokfunc(MakeBrowseList *blist,void *targetkey)
{
	mtnexcpts(targetkey,blist->funcargs);
}
static void procexcpnewrecord(RDArsrc *parent,MakeBrowseList *blist)
{
	mtnexcpts(NULL,blist->funcargs);
} 
static void mtnexcpts(void *targetkey,HoldReport *h)
{
	RDAreport *rpt;
	RDArunrpt *rrpt;
	char *namex=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG mtnexcpts Display Maintain Screen for Process Exceptions."); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(h!=NULL)
	{
		rrpt=h->rrpt;
		rpt=h->rpt;
	} else return;
	namex=Rmalloc(RDAstrlen(rpt->name)+17);
	sprintf(namex,"%s MAINTAIN SCREEN",rpt->name);
	h->mtnrsrc=RDArsrcNEW(rrpt->module,namex);
	if(namex!=NULL) Rfree(namex);
	namex=NULL;
	singlefile2rsrc(rrpt->sortno,h->mtnrsrc,TRUE);
	DefaultScreens(h->mtnrsrc);
	if(targetkey!=NULL)
	{
		if(!EQLNRDKEYsec(rrpt->sortno,1,targetkey))
		{
			singlefilerecord2rsrc(rrpt->sortno,h->mtnrsrc);
		}
	}
	addbtnrsrc(h->mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfkwrsrc(h->mtnrsrc,"SAVE NO UPDATE",TRUE,save_no_update,h);
	addrfkwrsrc(h->mtnrsrc,"SAVE",TRUE,save_update,h);
	addrfkwrsrc(h->mtnrsrc,"QUIT",TRUE,procexcpquitrecord,h);
	addrfcbrsrc(h->mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(h->mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(ADVmakescrn(h->mtnrsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom Process Exceptions MAINTAIN SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		procexcpquitrecord(h->mtnrsrc,h);
		return;
	}
	++rrpt->screen_count;
}
/*
static void SETID(HoldReport *h)
{
	RDArunrpt *rrpt;

	if(h!=NULL)
	{
		rrpt=h->rrpt;
		readallwidgets(h->mtnrsrc);
		rsrc2singlefilerecord(rrpt,h->mtnrsrc);
		KEYNRD(rrpt->sortno,1);
		EQLNRDsec(rrpt->sortno,1); 
		singlefilerecord2rsrc(rrpt,h->mtnrsrc);
		updateallrsrc(h->mtnrsrc);
	}
}
*/
static void save_no_update(RDArsrc *parent,HoldReport *h)
{
	procexcpsaverecord(parent,h,FALSE);
}
static void save_update(RDArsrc *parent,HoldReport *h)
{
	procexcpsaverecord(parent,h,TRUE);
}
void procexcpsaverecord(RDArsrc *parent,HoldReport *h,short update)
{
	char *rcddesc=NULL;
	RDArunrpt *rrpt;
/*
	int valuex=0;
	RDAbrowse *list;
	MakeBrowseList *mbl;
*/

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpsaverecord Process Exceptions Save Record Function from Screen [%s] [%s].",parent->module,parent->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(h!=NULL)
	{
		rrpt=h->rrpt;
	} else return;
	readallwidgets(h->mtnrsrc);
	rsrc2singlefilerecord(rrpt->sortno,h->mtnrsrc);
	if(RECORDsec(rrpt->sortno))
	{
		rcddesc=RECORDsecDesc(rrpt->sortno);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTNRD(rrpt->sortno,0,NULL);
		updatebrowse(update,rrpt->mbl,rrpt->sortno,rrpt->mainrsrc->module,
			rrpt->mbl->mainrsrc);
/*
		if(rrpt->mainrsrc!=NULL)
		{
			mbl=rrpt->mbl;
			FINDRSCGETINT(mbl->mainrsrc,"BROWSE LIST",&valuex);
			makebrowselist(rrpt->mbl);
			list=rrpt->mbl->list;
			if(list->num<=valuex) valuex=0;
			list->keyvalue=list->key[valuex];
			mbl=rrpt->mbl;
			if(!FINDRSCSETLIST(mbl->mainrsrc,"BROWSE LIST",valuex,list->num,
				&list->string))
			{
				updatersrc(mbl->mainrsrc,"BROWSE LIST");
			}
		}
*/
  	}
	procexcpquitrecord(parent,h);
}
void procexcpquitrecord(RDArsrc *parent,HoldReport *h)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpquitrecord Quit Process Exceptions Maintain Screen [%s] [%s] function.",parent->module,parent->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	--h->rrpt->screen_count;
	if(parent!=NULL) free_rsrc(parent);
	parent=NULL;
	h->mtnrsrc=NULL;
}
