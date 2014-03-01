/* process exceptions function */
#include <rptgen.hpp>

static short procexcpselectfunction(void);
static void mtnexcpts(void *,HoldReport *);
static void procexcpsaverecord(RDArsrc *,HoldReport *,short);
static void procexcpquitrecord(RDArsrc *,HoldReport *);
static void procexcpdoexit(RDArsrc *,HoldReport *);
static void procexcpokfunc(MakeBrowseList *,void *);
static void procexcpnewrecord(RDArsrc *,MakeBrowseList *);
static void abort_procexcp(MakeBrowseList *);
static void doabort_procexcptest(MakeBrowseList *);
static void save_update(RDArsrc *,HoldReport *);
static void save_no_update(RDArsrc *,HoldReport *);
extern void ClearProcExcpAccums(MakeBrowseList *);
extern void UpdateProcExcpAccums(MakeBrowseList *);

static short RPTGEN_GUIEVAL1(char *string,RDArsrc *r,HoldReport *h)
{
	if(!isEMPTY(string))
	{
		return(EVALUATEbol(string,ReportGenSubData1,(void *)h));
	} else return(TRUE);
}
static char *RPTGEN_GUIEVALSTR1(char *string,RDArsrc *r,void *SubFuncArgs,HoldReport *h)
{
	char *tmp=NULL;

	if(!isEMPTY(string))
	{
		tmp=PP_EVALUATEstr(string,ReportGenSubData1,(void *)h);
	}
	return(tmp);
}

void process_exceptions(RDArunrpt *rrpt,RDAreport *rpt)
{
	HoldReport *h=NULL;
	char *namex=NULL;
	RDAaccum *a=NULL;
	NRDfield *field=NULL;
	int x=0;

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
	if(rpt->display->device<3 || rpt->display->device==5)
	{
		addbtnrsrc(rrpt->mainrsrc,"CONTINUE",TRUE,procexcpdoexit,h);
	} else {
		addbtnrsrc(rrpt->mainrsrc,"SAVE",TRUE,procexcpdoexit,h);
	}
	rrpt->no_pe_accums=TRUE;
	rrpt->mainrsrc->scn=RDAscrnNEW(rrpt->mainrsrc->module,rrpt->mainrsrc->screen);
	if(findscrnbin(rrpt->mainrsrc->scn))
	{
		prterr("Error couldn't read Screen Binary [%s] [%s] from Binary Library [%s.SCN]",rrpt->mainrsrc->module,(rrpt->mainrsrc->screen!=NULL ? rrpt->mainrsrc->screen:""),rrpt->mainrsrc->module);
		free_scrn(rrpt->mainrsrc->scn);
		rrpt->mainrsrc->scn=NULL;
	}
	if(rpt->accum!=NULL)
	{
		for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
		{
			a->displayed=FALSE;
			if((FINDWDGT(rrpt->mainrsrc->scn,a->name))!=(-1)) 
			{
				if(!alreadyrsrc(a->name,rrpt->mainrsrc))
				{
					a->displayed=TRUE;
					rrpt->no_pe_accums=FALSE;
					
					if(a->atype==1)
					{
						makefieldrsrc(rrpt->mainrsrc,a->name,LONGV,8,FALSE);
					} else if(a->atype==7)
					{
						field=FLDNRD(rrpt->sortno,a->accname);
						if(field!=NULL)
						{
							switch(field->type)
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
									makefieldrsrc(rrpt->mainrsrc,a->name,LONGV,8,FALSE);
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									makefieldrsrc(rrpt->mainrsrc,a->name,field->type,field->len,FALSE);
									break;
								case SHORTV:
								case SSHORTV:
									makefieldrsrc(rrpt->mainrsrc,a->name,field->type,field->len,FALSE);
									break;
								case LONGV:
								case SLONGV:
								case SCROLLEDLIST:
									makefieldrsrc(rrpt->mainrsrc,a->name,field->type,field->len,FALSE);
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
	}
	rrpt->definelist->scn=RDAscrnNEW(rrpt->definelist->module,rrpt->definelist->screen);
	findscrnbin(rrpt->definelist->scn);
	rrpt->mbl=ADVbrowse(rrpt->sortno,1,rrpt->mainrsrc,rrpt->searchrsrc,rrpt->definelist,NULL,procexcpselectfunction,doabort_procexcptest,procexcpokfunc,procexcpnewrecord,h,TRUE,NULL,0,0,0,TRUE);
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
static short procexcpselectfunction()
{
	return(TRUE);
}
static void procexcpdoexit(RDArsrc *parent,HoldReport *h)
{
	RDArunrpt *rrpt;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG procexcpdoexit Exiting Process Exceptions.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
		if(rrpt->mainrsrc!=NULL) 
		{
			killwindow(rrpt->mainrsrc);
			free_rsrc(rrpt->mainrsrc);
		}
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
void updateallcb(RDArsrc *mtnrsrc,HoldReport *h)
{
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
	h->mtnrsrc->SubFunc=(void (*)(...))ReportGenSubData1;
	h->mtnrsrc->SubFuncArgs=h;
	if(namex!=NULL) Rfree(namex);
	namex=NULL;
	singlefile2rsrc(rrpt->sortno,h->mtnrsrc,TRUE);
	ADV2GET_SCREEN_VIRTUALS(h->mtnrsrc,0,FALSE,TRUE,FALSE);	
	ADVSCRNvirtual2rsrc(h->mtnrsrc,ReportGenSubData1,h);
	addbtnrsrc(h->mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfkwrsrc(h->mtnrsrc,"SAVE NO UPDATE",TRUE,save_no_update,h);
	addrfkwrsrc(h->mtnrsrc,"SAVE",TRUE,save_update,h);
	addrfkwrsrc(h->mtnrsrc,"QUIT",TRUE,procexcpquitrecord,h);
	addrfcbrsrc(h->mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(h->mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(h->mtnrsrc);
	if(targetkey!=NULL)
	{
		if(!EQLNRDKEYsec(rrpt->sortno,1,targetkey))
		{
			singlefilerecord2rsrc(rrpt->sortno,h->mtnrsrc);
		}
	}
	xSetVirFieldFuncs(h->mtnrsrc,updateallcb,(void *)h,1,NULL,NULL,__LINE__,__FILE__);
	ADVcomputeandupdateallSCRNvirtuals(h->mtnrsrc,ReportGenSubData1,h,TRUE);
	if(xmakescrn(h->mtnrsrc,TRUE,RPTGEN_GUIEVAL1,(void *)h,addoswidgets,RPTGEN_GUIEVALSTR1,(void *)h,ReportGenSubData1,h,NULL,__LINE__,__FILE__))
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
	RDArunrpt *rrpt=NULL;

	if(h!=NULL)
	{
		rrpt=h->rrpt;
		readallwidgets(h->mtnrsrc);
		rsrc2singlefilerecord(rrpt->sortno,h->mtnrsrc);
		KEYNRD(rrpt->sortno,1);
		EQLNRDsec(rrpt->sortno,1); 
		singlefilerecord2rsrc(rrpt->sortno,h->mtnrsrc);
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
		if(rrpt->no_pe_accums==FALSE) ClearProcExcpAccums(rrpt->mbl);
		updatebrowse(update,rrpt->mbl,rrpt->sortno,rrpt->mainrsrc->module,
			rrpt->mbl->mainrsrc);
		if(rrpt->no_pe_accums==FALSE) UpdateProcExcpAccums(rrpt->mbl);
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
