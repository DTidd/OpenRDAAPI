/* payfixnames.c - Payroll Fix Naming Conventions in screens, help, datafiles, and reports */
#ifndef WIN32
#define __NAM__ "payfixnames.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payfixnames.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <impexp.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

APPlib *PayrollOld=NULL,*PayrollNew=NULL;
short UserNum=(-1);

void change_pattern(APPlib *namold,APPlib *namnew,char **tmp)
{
	char *temp,*tmp1=NULL;
	int x,y;

	if(namold==NULL || namnew==NULL) return;
	if(isEMPTY(*tmp)) return;
	for(x=0;x<(RDAstrlen(*tmp));++x)
	{
		temp=*tmp+x;
		for(y=0;y<namold->numlibs;++y)
		{
			if(!strncmp(temp,namold->libs[y],RDAstrlen(namold->libs[y]))) break;
		}
		if(y<namold->numlibs)
		{
			temp+=RDAstrlen(namold->libs[y]);
			tmp1=Rmalloc(RDAstrlen(namnew->libs[y])+(temp!=NULL ? 
				RDAstrlen(temp):0)+1+x);
			if(x>0 && x<RDAstrlen(*tmp) && *tmp!=NULL)
			{
				sprintf(tmp1,"%.*s%s%s",x,*tmp,namnew->libs[y],
					(temp!=NULL ? temp:""));
			} else if(x>0 && x>=RDAstrlen(*tmp) && *tmp!=NULL)
			{
				sprintf(tmp1,"%s%s",*tmp,namnew->libs[y]);
			} else if(x==0)
			{
				sprintf(tmp1,"%s%s",namnew->libs[y],(temp!=NULL ? temp:""));
			}
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc(tmp1);
			if(tmp1!=NULL) Rfree(tmp1);
			tmp1=NULL;
		}
	}
}
APPlib *GetOldPattern(char *modname)
{
	if(!RDAstrcmp(modname,"PAYROLL")) return(PayrollOld);
	else return(NULL);
}
APPlib *GetNewPattern(char *modname)
{
	if(!RDAstrcmp(modname,"PAYROLL")) return(PayrollNew);
	else return(NULL);
}
void change_scrnlib(char *dirx,char *modname)
{
/* verified screen structure 01/07/95 */
	APPlib *scrlist=NULL,*OldPattern=NULL,*NewPattern=NULL;
	int x,y;
	RDAscrn *scn=NULL;
	RDAwdgt *wdgt;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modname)+6);
	sprintf(libx,"%s/%s.SCN",dirx,modname);
	scrlist=getunsortedlibnames(libx,FALSE);
	if(scrlist!=NULL)
	{
		OldPattern=GetOldPattern(modname);
		NewPattern=GetNewPattern(modname);
		for(x=0;x<scrlist->numlibs;++x)
		{
			scn=RDAscrnNEW(modname,scrlist->libs[x]);
			if(!getscrnbin(libx,scn,TRUE))
			{
				change_pattern(OldPattern,NewPattern,&scn->name);
				for(y=0,wdgt=scn->wdgts;y<scn->numwdgts;++y,++wdgt)
				{
					if(wdgt->resource_name!=NULL)
						change_pattern(OldPattern,NewPattern,
							&wdgt->resource_name);
					if(wdgt->label!=NULL)
						change_pattern(OldPattern,NewPattern,
							&wdgt->label);
					if(wdgt->expression!=NULL)
						change_pattern(OldPattern,NewPattern,
							&wdgt->expression);
				}
				writescrnbin(libx,scn);
			}
			if(scn!=NULL) free_scrn(scn);
			scn=NULL;
		}
		freeapplib(scrlist);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void change_screens(char *modname)
{
	char *dirx=NULL,*userid=NULL,deleteflag=FALSE;
	short ef;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+5);
	sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
	change_scrnlib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(dirx,"%s/site",CURRENTDIRECTORY);
	change_scrnlib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	ef=BEGNRDsec(UserNum);
	while(!ef)
	{
		FINDFLDGETCHAR(UserNum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(UserNum,"USER IDENTIFICATION",&userid);
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+
				RDAstrlen(userid)+2);
			sprintf(dirx,"%s/%s",CURRENTDIRECTORY,userid);
			change_scrnlib(dirx,modname);
			if(dirx!=NULL) Rfree(dirx);
		}
		ef=SEQNRDsec(UserNum);
	}
	if(userid!=NULL) Rfree(userid);
}
void change_helplib(char *dirx,char *modname)
{
	APPlib *scrlist=NULL,*OldPattern=NULL,*NewPattern=NULL;
	int x,y;
	RDAhelp *scn=NULL;
	RDArhlp *rhlp;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modname)+6);
	sprintf(libx,"%s/%s.HLP",dirx,modname);
	scrlist=getunsortedlibnames(libx,FALSE);
	if(scrlist!=NULL)
	{
		OldPattern=GetOldPattern(modname);
		NewPattern=GetNewPattern(modname);
		for(x=0;x<scrlist->numlibs;++x)
		{
			scn=RDAhelpNEW();
			if(!gethelpbin(libx,scrlist->libs[x],scn,TRUE))
			{
				change_pattern(OldPattern,NewPattern,&scn->name);
				change_pattern(OldPattern,NewPattern,&scn->desc);
				for(y=0,rhlp=scn->rhlp;y<scn->numrschs;++y,++rhlp)
				{
					if(rhlp->name!=NULL)
						change_pattern(OldPattern,NewPattern,&rhlp->name);
					if(rhlp->desc!=NULL)
						change_pattern(OldPattern,NewPattern,&rhlp->desc);
				}
				writehelpbin(libx,scn);
			}
			if(scn!=NULL) free_help(scn);
			scn=NULL;
		}
		freeapplib(scrlist);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void change_helps(char *modname)
{
	char *dirx=NULL,*userid=NULL,deleteflag=FALSE;
	short ef;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+5);
	sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
	change_helplib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(dirx,"%s/site",CURRENTDIRECTORY);
	change_helplib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	ef=BEGNRDsec(UserNum);
	while(!ef)
	{
		FINDFLDGETCHAR(UserNum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(UserNum,"USER IDENTIFICATION",&userid);
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+RDAstrlen(userid)+2);
			sprintf(dirx,"%s/%s",CURRENTDIRECTORY,userid);
			change_helplib(dirx,modname);
			if(dirx!=NULL) Rfree(dirx);
		}
		ef=SEQNRDsec(UserNum);
	}
	if(userid!=NULL) Rfree(userid);
}
void change_defaultlib(char *dirx,char *modname)
{
	APPlib *scrlist=NULL,*OldPattern=NULL,*NewPattern=NULL;
	int x,y;
	RDAdefault *scn=NULL;
	DEFfield *rhlp;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modname)+6);
	sprintf(libx,"%s/%s.DEF",dirx,modname);
	scrlist=getunsortedlibnames(libx,FALSE);
	if(scrlist!=NULL)
	{
		OldPattern=GetOldPattern(modname);
		NewPattern=GetNewPattern(modname);
		for(x=0;x<scrlist->numlibs;++x)
		{
			scn=RDAdefaultNEW(modname,scrlist->libs[x]);
			if(!getdefaultbin(libx,scrlist->libs[x],scn,TRUE))
			{
				change_pattern(OldPattern,NewPattern,&scn->name);
				change_pattern(OldPattern,NewPattern,&scn->input_focus);
				for(y=0,rhlp=scn->field;y<scn->num;++y,++rhlp)
				{
					if(rhlp->name!=NULL)
						change_pattern(OldPattern,NewPattern,&rhlp->name);
				}
				writedefaultbin(libx,scn);
			}
			if(scn!=NULL) FreeRDAdefault(scn);
			scn=NULL;
		}
		freeapplib(scrlist);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void change_defaults(char *modname)
{
	char *dirx=NULL,*userid=NULL,deleteflag=FALSE;
	short ef;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
	sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
	change_defaultlib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
	sprintf(dirx,"%s/site",CURRENTDIRECTORY);
	change_defaultlib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	ef=BEGNRDsec(UserNum);
	while(!ef)
	{
		FINDFLDGETCHAR(UserNum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(UserNum,"USER IDENTIFICATION",&userid);
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(userid)+2);
			sprintf(dirx,"%s/%s",CURRENTDIRECTORY,userid);
			change_defaultlib(dirx,modname);
			if(dirx!=NULL) Rfree(dirx);
		}
		ef=SEQNRDsec(UserNum);
	}
	if(userid!=NULL) Rfree(userid);
}
void change_menulib(char *dirx,char *modname)
{
	APPlib *scrlist=NULL,*OldPattern=NULL,*NewPattern=NULL;
	int x;
	RDAmenu *scn=NULL;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modname)+6);
	sprintf(libx,"%s/%s.MNU",dirx,modname);
	scrlist=getunsortedlibnames(libx,FALSE);
	if(scrlist!=NULL)
	{
		OldPattern=GetOldPattern(modname);
		NewPattern=GetNewPattern(modname);
		for(x=0;x<scrlist->numlibs;++x)
		{
			scn=RDAmenuNEW(modname,scrlist->libs[x]);
			if(!getmenubin(libx,scn))
			{
				change_pattern(OldPattern,NewPattern,&scn->name);
				change_pattern(OldPattern,NewPattern,&scn->command);
				writemenubin(libx,scn);
			}
			if(scn!=NULL) free_menu(scn);
			scn=NULL;
		}
		freeapplib(scrlist);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void change_menus(char *modname)
{
	char *dirx=NULL,*userid=NULL,deleteflag=FALSE;
	short ef;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
	sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
	change_menulib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(dirx,"%s/site",CURRENTDIRECTORY);
	change_menulib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	ef=BEGNRDsec(UserNum);
	while(!ef)
	{
		FINDFLDGETCHAR(UserNum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(UserNum,"USER IDENTIFICATION",&userid);
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+RDAstrlen(userid)+2);
			sprintf(dirx,"%s/%s",CURRENTDIRECTORY,userid);
			change_menulib(dirx,modname);
			if(dirx!=NULL) Rfree(dirx);
		}
		ef=SEQNRDsec(UserNum);
	}
	if(userid!=NULL) Rfree(userid);
}
void change_mtnlib(char *dirx,char *modname)
{
	APPlib *scrlist=NULL,*OldPattern=NULL,*NewPattern=NULL;
	MaintainMaster *Master=NULL;
	char *libx=NULL;
	RDAsearch *s;
	MaintainList *l;
	int x,a,b,c;
	RDAconfld *confld;
	MaintainButton *button;
	ButtonField *bf;

	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modname)+6);
	sprintf(libx,"%s/%s.MTN",dirx,modname);
	scrlist=getunsortedlibnames(libx,FALSE);
	if(scrlist!=NULL)
	{
		OldPattern=GetOldPattern(modname);
		NewPattern=GetNewPattern(modname);
		for(x=0;x<scrlist->numlibs;++x)
		{
			Master=MaintainMasterNew(modname,scrlist->libs[x]);
			if(!getMaintainMaster(libx,Master))
			{
				change_pattern(OldPattern,NewPattern,&Master->name);
				change_pattern(OldPattern,NewPattern,&Master->browse_screen);
				change_pattern(OldPattern,NewPattern,&Master->define_list);
				change_pattern(OldPattern,NewPattern,&Master->search_browse);
				change_pattern(OldPattern,NewPattern,&Master->maintain_screen);
				if(Master->s!=NULL)
				{
					for(a=0,s=Master->s;a<Master->num;++a,++s)
					{
						OldPattern=GetOldPattern(modname);
						NewPattern=GetNewPattern(modname);
						if(s->expression!=NULL)
							change_pattern(OldPattern,NewPattern,
								&s->expression);
						if(s->confld!=NULL)
						{
							for(b=0,confld=s->confld;b<s->num;++b,++confld)
							{
								if(confld->conname!=NULL) 
								{
									OldPattern=GetOldPattern(s->cmodule);
									NewPattern=GetNewPattern(s->cmodule);
									change_pattern(OldPattern,NewPattern,
										&confld->conname);
								}
								if(confld->fldname!=NULL) 
								{
									OldPattern=GetOldPattern(s->module);
									NewPattern=GetNewPattern(s->module);
									change_pattern(OldPattern,NewPattern,
										&confld->fldname);
								}
							}
						}
					}
				}
				OldPattern=GetOldPattern(modname);
				NewPattern=GetNewPattern(modname);
				if(Master->lists!=NULL)
				{
					for(a=0,l=Master->lists;a<Master->numlists;++a,++l)
					{
						if(l->name!=NULL) 
						{
							change_pattern(OldPattern,NewPattern,&l->name);
						}
					}
				}
				if(Master->buttons!=NULL)
				{
					for(a=0,button=Master->buttons;a<Master->numbuttons;
						++a,++button)
					{
						OldPattern=GetOldPattern(modname);
						NewPattern=GetNewPattern(modname);
						if(button->listname!=NULL)
						{
							change_pattern(OldPattern,NewPattern,
								&button->listname);
						}
						if(button->buttonname!=NULL) 
						{
							change_pattern(OldPattern,NewPattern,
								&button->buttonname);
						}
						if(button->bf!=NULL)
						{
							OldPattern=GetOldPattern(button->modulename);
							NewPattern=GetNewPattern(button->modulename);
							for(c=0,bf=button->bf;c<button->num;++c,++bf)
							{
								if(bf->rname!=NULL) 
								{
									change_pattern(OldPattern,NewPattern,
										&bf->rname);
								}
								if(bf->ename!=NULL) Rfree(bf->ename);
								bf->ename=adddashes(bf->rname);
							}
						}
					}
				}
				writeMaintainMaster(libx,Master);
			}
			if(Master!=NULL) FreeMaintainMaster(Master);
			Master=NULL;
		}
		freeapplib(scrlist);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void change_maintains(char *modname)
{
	char *dirx=NULL,*userid=NULL,deleteflag=FALSE;
	short ef;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
	sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
	change_mtnlib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(dirx,"%s/site",CURRENTDIRECTORY);
	change_mtnlib(dirx,modname);
	if(dirx!=NULL) Rfree(dirx);
	ef=BEGNRDsec(UserNum);
	while(!ef)
	{
		FINDFLDGETCHAR(UserNum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(UserNum,"USER IDENTIFICATION",&userid);
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(userid)+2);
			sprintf(dirx,"%s/%s",CURRENTDIRECTORY,userid);
			change_mtnlib(dirx,modname);
			if(dirx!=NULL) Rfree(dirx);
		}
		ef=SEQNRDsec(UserNum);
	}
	if(userid!=NULL) Rfree(userid);
}
void changeRDAreport(char *modx,char *name)
{
	RDAreport *report=NULL;
	RDAvirtual *v;
	RDAsort *sort;
	RDAaccum *accum;
	RDAimage *i;
	RDApval *pval;
	int x,y;
	RDAsearch *s;
	RDAcontrol *c;
	RDAconfld *confld;
	APPlib *OldPattern=NULL,*NewPattern=NULL;
	RDAdisplay *d=NULL;
	char *temp=NULL,*modi=NULL;

	report=GETRDAreport(modx,name);
	if(report!=NULL)
	{
		OldPattern=GetOldPattern(modx);
		NewPattern=GetNewPattern(modx);
		change_pattern(OldPattern,NewPattern,&report->select);
		for(x=0,s=report->search;x<report->numsearchs;++x,++s)
		{
			for(y=0,confld=s->confld;y<s->num;++y,++confld)
			{
				if(confld->fldname!=NULL)
				{
					OldPattern=GetOldPattern(s->module);
					NewPattern=GetNewPattern(s->module);
					change_pattern(OldPattern,NewPattern,&confld->fldname);
				} 
				if(confld->conname!=NULL)
				{
					OldPattern=GetOldPattern(s->cmodule);
					NewPattern=GetNewPattern(s->cmodule);
					change_pattern(OldPattern,NewPattern,&confld->conname);
				}
			}
			if(s->expression!=NULL)
			{
				OldPattern=GetOldPattern(s->module);
				NewPattern=GetNewPattern(s->module);
				change_pattern(OldPattern,NewPattern,&s->expression);
			}
		}
		OldPattern=GetOldPattern(modx);
		NewPattern=GetNewPattern(modx);
		for(x=0,v=report->virtual;x<report->numvirtuals;++x,++v)
		{
			if(!isEMPTY(v->name))
				change_pattern(OldPattern,NewPattern,&v->name);
			if(!isEMPTY(v->expression))
				change_pattern(OldPattern,NewPattern,&v->expression);
		}
		for(x=0,sort=report->sort;x<report->numsorts;++x,++sort)
		{
			if(!isEMPTY(sort->name))
				change_pattern(OldPattern,NewPattern,&sort->name);
		}
		for(x=0,accum=report->accum;x<report->numaccums;++x,++accum)
		{
			if(!isEMPTY(accum->name))
				change_pattern(OldPattern,NewPattern,&accum->name);
			if(!isEMPTY(accum->accname))
				change_pattern(OldPattern,NewPattern,&accum->accname);
		}
		for(x=0,c=report->control;x<report->numcontrols;++x,++c)
		{
			if(!isEMPTY(c->name))
				change_pattern(OldPattern,NewPattern,&c->name);
		}
		d=report->display;
		for(x=0,i=report->image;x<report->numimages;++x,++i)
		{
			OldPattern=GetOldPattern(modx);
			NewPattern=GetNewPattern(modx);
			if(i->line!=NULL)
			{
				change_pattern(OldPattern,NewPattern,&i->line);
			}
			if(i->breakname!=NULL)
			{
				change_pattern(OldPattern,NewPattern,&i->breakname);
			}
			if(i->pvals!=NULL)
			{
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->name!=NULL) 
					{
						if(d->device==3 || d->device==4)
						{
							temp=i->line+5;
							while(iswhite(*temp)) ++temp;
							modi=stripmodulename(temp);
							OldPattern=GetOldPattern(modi);
							NewPattern=GetNewPattern(modi);
							if(modi!=NULL) Rfree(modi);
						} else {
							OldPattern=GetOldPattern(modx);
							NewPattern=GetNewPattern(modx);
						}
						change_pattern(OldPattern,NewPattern,&pval->name);
					}
					if(pval->fieldname!=NULL) 
					{
						OldPattern=GetOldPattern(modx);
						NewPattern=GetNewPattern(modx);
						change_pattern(OldPattern,NewPattern,&pval->fieldname);
					}
				}
			}
			if(i->expression!=NULL) 
			{
				OldPattern=GetOldPattern(modx);
				NewPattern=GetNewPattern(modx);
				change_pattern(OldPattern,NewPattern,
					&i->expression);
			}
		}
		WRTRDAreport(modx,report);
		FreeRDAreport(report);
	}
}
void changeRDAreports(char *modname)
{
	char *dirx=NULL,*tmp=NULL;
	int x;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+3);
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modname);
	for(x=0;findfile(dirx,"*.RPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		changeRDAreport(modname,tmp);
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
} 
void changeEZreport(char *modx,char *name)
{
	RDAezrpt *ez=NULL;
	int x;
	RDAezsrt *sort;
	RDAezdtl *detail;
	APPlib *OldPattern=NULL,*NewPattern=NULL;

	ez=GETRDAezrpt(modx,name);
	if(ez!=NULL)
	{
		OldPattern=GetOldPattern(modx);
		NewPattern=GetNewPattern(modx);
		for(x=0,sort=ez->sort;x<ez->numsrt;++x,++sort)
		{
			if(sort->name!=NULL) 
				change_pattern(OldPattern,NewPattern,&sort->name);
		}
		for(x=0,detail=ez->detail;x<ez->numdtl;++x,++detail)
		{
			if(detail->fieldname!=NULL) 
				change_pattern(OldPattern,NewPattern,&detail->fieldname);
			if(detail->expression!=NULL) 
				change_pattern(OldPattern,NewPattern,&detail->expression);
		}
		WRTRDAezrpt(modx,ez);
		FreeRDAezrpt(ez);
		ez=NULL;
	}
}
void change_virtuals(char *modname)
{
	APPlib *scrlist=NULL,*OldPattern=NULL,*NewPattern=NULL;
	int x;
	DFvirtual *scn=NULL;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,modname);
	scrlist=getunsortedlibnames(libx,FALSE);
	if(scrlist!=NULL)
	{
		OldPattern=GetOldPattern(modname);
		NewPattern=GetNewPattern(modname);
		for(x=0;x<scrlist->numlibs;++x)
		{
			scn=DFvirtualNEW(modname,scrlist->libs[x]);
			if((scn=getDFvirtual(modname,scrlist->libs[x]))!=NULL)
			{
				change_pattern(OldPattern,NewPattern,&scn->name);
				change_pattern(OldPattern,NewPattern,&scn->expression);
				change_pattern(OldPattern,NewPattern,&scn->desc);
				writeDFvirtual(modname,scn);
			}
			if(scn!=NULL) FreeDFvirtual(scn);
			scn=NULL;
		}
		freeapplib(scrlist);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void change_imports(char *modname)
{
	APPlib *scrlist=NULL,*OldPattern=NULL,*NewPattern=NULL;
	int x,y;
	IEfile *scn=NULL;
	IEfield *field;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
	sprintf(libx,"%s/rda/%s.IMP",CURRENTDIRECTORY,modname);
	scrlist=getunsortedlibnames(libx,FALSE);
	if(scrlist!=NULL)
	{
		OldPattern=GetOldPattern(modname);
		NewPattern=GetNewPattern(modname);
		for(x=0;x<scrlist->numlibs;++x)
		{
			scn=IMPORTstpNEW(scrlist->libs[x]);
			if(!getIMPORTbin(modname,scn))
			{
				change_pattern(OldPattern,NewPattern,&scn->name);
				change_pattern(OldPattern,NewPattern,&scn->expression);
				change_pattern(OldPattern,NewPattern,&scn->desc);
				if(scn->fields!=NULL)
				{
					for(y=0,field=scn->fields;y<scn->num;++y,++field)
					{
						change_pattern(OldPattern,NewPattern,
							&field->name);
						change_pattern(OldPattern,NewPattern,
							&field->expression);
					}
				}
				writeIMPORTbin(modname,scn);
			}
			if(scn!=NULL) free_import(scn);
			scn=NULL;
		}
		freeapplib(scrlist);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void changeEZreports(char *modname)
{
	char *dirx=NULL,*tmp=NULL;
	int x;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+3);
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modname);
	for(x=0;findfile(dirx,"*.EZRPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-6]=0;
		changeEZreport(modname,tmp);
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
}
static void change_reports(char *modname)
{
	changeRDAreports(modname);
	changeEZreports(modname);
}
struct chg_fld_mgrs
{
	char *name;
	char *newname;
};
typedef struct chg_fld_mgrs chgfldmgr;

static short CHANGE_FILE(char *modulename,char *filename)
{
	short x,tmp_engine=0,tmp_nofields=0,tmp_nokeys=0;
	int y,z;
	short tmpfileno=0,ef=0,s=0,engine=0,nofields=0,nokeys=0;
	NRDfield *fld=NULL,*tmp_fields=NULL,*fields=NULL;
	NRDkey *tmp_keys=NULL,*keyx;
	char *tmp=NULL,*libx=NULL;
	char *e=NULL;
	RDArsrc *tmprsrc=NULL;
	NRDkey *keys=NULL;
	NRDpart *part;
	chgfldmgr *HoldChgFld=NULL,*tHCF;
	short numHCFS=0;
	APPlib *OldPattern=NULL,*NewPattern=NULL;

	OldPattern=GetOldPattern(modulename);
	NewPattern=GetNewPattern(modulename);
	if(!GETBIN(modulename,filename,&engine,&nofields,&fields,
			&nokeys,&keys))
	{
		for(x=0,fld=fields;x<nofields;++x,++fld)
		{
			if(HoldChgFld!=NULL)
			{
				HoldChgFld=Rrealloc(HoldChgFld,(numHCFS+1)*
					sizeof(chgfldmgr));
			} else {
				HoldChgFld=Rmalloc(sizeof(chgfldmgr));
				numHCFS=0;
			}
			tHCF=HoldChgFld+numHCFS;
			tHCF->name=stralloc(fld->name);
			change_pattern(OldPattern,NewPattern,&fld->name);
			tHCF->newname=stralloc(fld->name);
			++numHCFS;
		}
		for(y=0,keyx=keys;y<nokeys;++y,++keyx)
		{
			if(keyx->part!=NULL)
			{
				for(z=0,part=keyx->part;z<keyx->numparts;
					++z,++part)
				{
					if(part->name!=NULL) 
						change_pattern(OldPattern,NewPattern,&part->name);
				}
			}
		}
		for(x=0,tHCF=HoldChgFld;x<numHCFS;++x,++tHCF)
		{
			if(RDAstrcmp(tHCF->name,tHCF->newname)) break;
		}
		if(x>=numHCFS)
		{
			for(x=0,tHCF=HoldChgFld;x<numHCFS;++x,++tHCF)
			{
				if(tHCF->name!=NULL) Rfree(tHCF->name);
				if(tHCF->newname!=NULL) Rfree(tHCF->newname);
			}
			Rfree(HoldChgFld);
			HoldChgFld=NULL;
			numHCFS=0;
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
			}
			if(fields!=NULL)
			{
				for(x=0,fld=fields;x<nofields;++x,++fld)
				{
					if(fld->name!=NULL) Rfree(fld->name);
				}
				Rfree(fields);
				nofields=0;
				fields=NULL;
			}
			return(1);
		}
	} else return(-1);
	if((x=OPNNRD(modulename,filename))==(-1))
	{
		libx=Rmalloc(RDAstrlen(filename)+RDAstrlen(modulename)+RDAstrlen(CURRENTDIRECTORY)+3);
		sprintf(libx,"%s/%s/%s",CURRENTDIRECTORY,modulename,filename);
		DELETENRD(libx);
		if(libx!=NULL) Rfree(libx);
		libx=NULL;
		if(HoldChgFld!=NULL)
		{
			for(x=0,tHCF=HoldChgFld;x<numHCFS;++x,++tHCF)
			{
				if(tHCF->name!=NULL) Rfree(tHCF->name);
				if(tHCF->newname!=NULL) Rfree(tHCF->newname);
			}
			Rfree(HoldChgFld);
			HoldChgFld=NULL;
			numHCFS=0;
		}
		for(y=0,keyx=keys;y<nokeys;++y,++keyx)
		{
			if(keyx->name!=NULL) Rfree(keyx->name);
			if(keyx->part!=NULL)
			{
				for(z=0,part=keyx->part;z<keyx->numparts;
					++z,++part)
				{
					if(part->name!=NULL) Rfree(part->name);
				}
			}
		}
		if(fields!=NULL)
		{
			for(x=0,fld=fields;x<nofields;++x,++fld)
			{
				if(fld->name!=NULL) Rfree(fld->name);
			}
			Rfree(fields);
			nofields=0;
			fields=NULL;
		}
		return(CRTNRD(modulename,engine,filename,nofields,fields,nokeys,keys));
	} else {
		if(!GETBIN(modulename,filename,&tmp_engine,&tmp_nofields,&tmp_fields,
			&tmp_nokeys,&tmp_keys))
		{
			tmp=Rmalloc(RDAstrlen(USERLOGIN)+10);
			sprintf(tmp,"%s%d",USERLOGIN,RGETPID());	
			libx=Rmalloc(RDAstrlen(tmp)+RDAstrlen(modulename)+RDAstrlen(CURRENTDIRECTORY)+3);
			sprintf(libx,"%s/%s/%s",CURRENTDIRECTORY,modulename,tmp);
			DELETENRD(libx);
			if(libx!=NULL) Rfree(libx);
			libx=NULL;
			tmpfileno=CRTNRD(modulename,tmp_engine,tmp,tmp_nofields,tmp_fields,tmp_nokeys,tmp_keys);
			if(tmpfileno!=(-1))
			{
				ef=FRSNRD(x,1);
				e=Rmalloc(RDAstrlen(tmp)+RDAstrlen(modulename)+27);
				sprintf(e,"Writing Temp File %s Module %s",tmp,modulename);
				tmprsrc=diagscrn(x,"DIAGNOSTIC SCREEN",modulename,e,NULL);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
				}
				while(!ef)
				{
					for(s=0,fld=tmp_fields;s<tmp_nofields;++s,++fld)
					{
						COPYFIELD(x,tmpfileno,
							fld->name);
					}
					if(!WRTNRD(tmpfileno,0,NULL)) RCKNRD(tmpfileno);
					ef=NXTNRD(x,1);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				}
				CLSNRD(x);
				if(tmprsrc!=NULL)
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
				for(y=0,keyx=tmp_keys;y<tmp_nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;
							++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
					}
				}
				if(tmp_fields!=NULL)
				{
					for(x=0,fld=tmp_fields;x<tmp_nofields;++x,++fld)
					{
						if(fld->name!=NULL) Rfree(fld->name);
					}
					Rfree(tmp_fields);
					tmp_nofields=0;
					tmp_fields=NULL;
				}

				libx=Rmalloc(RDAstrlen(filename)+RDAstrlen(modulename)+RDAstrlen(CURRENTDIRECTORY)+3);
				sprintf(libx,"%s/%s/%s",CURRENTDIRECTORY,modulename,filename);
				DELETENRD(libx);
				if(libx!=NULL) Rfree(libx);
				libx=NULL;
				x=CRTNRD(modulename,engine,filename,nofields,fields,
					nokeys,keys);
				if(x!=(-1))
				{
					ef=FRSNRD(tmpfileno,1);
					e=Rmalloc(RDAstrlen(filename)+RDAstrlen(modulename)+21);
					sprintf(e,"Saving File %s Module %s",filename,modulename);
					tmprsrc=diagscrn(tmpfileno,"DIAGNOSTIC SCREEN",modulename,e,NULL);
					if(e!=NULL) Rfree(e);
					e=NULL;
					if(tmprsrc!=NULL)
					{
						if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE))
							ForceWindowUpdate(tmprsrc);
					}
					while(!ef)
					{
						for(s=0,tHCF=HoldChgFld;s<numHCFS;++s,++tHCF)
						{
							FIELDCOPY(tmpfileno,tHCF->name,x,
								tHCF->newname);
						}
						if(!WRTNRD(x,0,NULL)) RCKNRD(x);
						ef=NXTNRD(tmpfileno,1);
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
					}
					CLSNRD(tmpfileno);		
					if(tmprsrc!=NULL)
					{
						killwindow(tmprsrc);
						free_rsrc(tmprsrc);
					}
					libx=Rmalloc(RDAstrlen(tmp)+RDAstrlen(CURRENTDIRECTORY)+11);
					sprintf(libx,"%s/DATABASE/%s",CURRENTDIRECTORY,tmp);
					DELETENRD(libx);
					if(libx!=NULL) Rfree(libx);
					libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
					sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modulename);
					deletelibbin(libx,tmp);
					if(libx!=NULL) Rfree(libx);
					if(tmp!=NULL) Rfree(tmp);
					tmp=NULL;
					if(HoldChgFld!=NULL)
					{
						for(x=0,tHCF=HoldChgFld;x<numHCFS;++x,++tHCF)
						{
							if(tHCF->name!=NULL) Rfree(tHCF->name);
							if(tHCF->newname!=NULL) Rfree(tHCF->newname);
						}
						Rfree(HoldChgFld);
						HoldChgFld=NULL;
						numHCFS=0;
					}
					CLSNRD(x);
					for(y=0,keyx=keys;y<nokeys;++y,++keyx)
					{
						if(keyx->name!=NULL) Rfree(keyx->name);
						if(keyx->part!=NULL)
						{
							for(z=0,part=keyx->part;z<keyx->numparts;
								++z,++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
						}
					}
					if(fields!=NULL)
					{
						for(x=0,fld=fields;x<nofields;++x,++fld)
						{
							if(fld->name!=NULL) Rfree(fld->name);
						}
						Rfree(fields);
						nofields=0;
						fields=NULL;
					}
					return(0);
				} else {
					prterr("Error - Cannot create file [%s]. Data Saved in temporary file [%d] [%s]. Call your installer.",filename,tmpfileno,FILENAME(tmpfileno));
					CLSNRD(tmpfileno);
					if(tmp!=NULL) Rfree(tmp);
					tmp=NULL;
					if(HoldChgFld!=NULL)
					{
						for(x=0,tHCF=HoldChgFld;x<numHCFS;++x,++tHCF)
						{
							if(tHCF->name!=NULL) Rfree(tHCF->name);
							if(tHCF->newname!=NULL) Rfree(tHCF->newname);
						}
						Rfree(HoldChgFld);
						HoldChgFld=NULL;
						numHCFS=0;
					}
					for(y=0,keyx=keys;y<nokeys;++y,++keyx)
					{
						if(keyx->name!=NULL) Rfree(keyx->name);
						if(keyx->part!=NULL)
						{
							for(z=0,part=keyx->part;z<keyx->numparts;
								++z,++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
						}
					}
					if(fields!=NULL)
					{
						for(x=0,fld=fields;x<nofields;++x,++fld)
						{
							if(fld->name!=NULL) Rfree(fld->name);
						}
						Rfree(fields);
						nofields=0;
						fields=NULL;
					}
					return(-1);
				}
			} else {
				prterr("Error - Couldn't create temporary file [%s] conversion failed.",tmp);
			}
			if(tmp!=NULL) Rfree(tmp);
			tmp=NULL;
			if(HoldChgFld!=NULL)
			{
				for(x=0,tHCF=HoldChgFld;x<numHCFS;++x,++tHCF)
				{
					if(tHCF->name!=NULL) Rfree(tHCF->name);
					if(tHCF->newname!=NULL) Rfree(tHCF->newname);
				}
				Rfree(HoldChgFld);
				HoldChgFld=NULL;
				numHCFS=0;
			}
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
			}
			if(fields!=NULL)
			{
				for(x=0,fld=fields;x<nofields;++x,++fld)
				{
					if(fld->name!=NULL) Rfree(fld->name);
				}
				Rfree(fields);
				nofields=0;
				fields=NULL;
			}
			return(-1);
		} else {
			prterr("Error - Cannot Read Binary File.");
			if(HoldChgFld!=NULL)
			{
				for(x=0,tHCF=HoldChgFld;x<numHCFS;++x,++tHCF)
				{
					if(tHCF->name!=NULL) Rfree(tHCF->name);
					if(tHCF->newname!=NULL) Rfree(tHCF->newname);
				}
				Rfree(HoldChgFld);
				HoldChgFld=NULL;
				numHCFS=0;
			}
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
			}
			if(fields!=NULL)
			{
				for(x=0,fld=fields;x<nofields;++x,++fld)
				{
					if(fld->name!=NULL) Rfree(fld->name);
				}
				Rfree(fields);
				nofields=0;
				fields=NULL;
			}
			return(-1);
		}
	}
}
void change_filelib(char *modname)
{
	char *libx=NULL;
	APPlib *filenames=NULL;
	int x;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modname);
	filenames=getunsortedlibnames(libx,FALSE);
	if(filenames!=NULL)
	{
		for(x=0;x<filenames->numlibs;++x)
		{
			if(CHANGE_FILE(modname,filenames->libs[x])==(-1))
			{
				ERRORDIALOG("FIX NAMES CONVERT FILE FAILED","The file attempting to convert for it's field name changes failed.  Contact your installer.",NULL,FALSE);
			}
		}
		freeapplib(filenames);
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
}
void freeAPPlibs()
{
	if(PayrollOld!=NULL) freeapplib(PayrollOld);
	if(PayrollNew!=NULL) freeapplib(PayrollNew);
}
void makePayrollAPPlibs()
{
	PayrollOld=APPlibNEW();
	PayrollNew=APPlibNEW();
	addAPPlib(PayrollOld,"UPDATED");
	addAPPlib(PayrollNew,"DISTRIBUTED");
	addAPPlib(PayrollOld,"Updated");
	addAPPlib(PayrollNew,"Distributed");
	addAPPlib(PayrollOld,"updated");
	addAPPlib(PayrollNew,"distributed");
/*
	addAPPlib(PayrollOld,"CLEAR DATE");
	addAPPlib(PayrollNew,"CLEARED DATE");
	addAPPlib(PayrollOld,"CLEAR TIME");
	addAPPlib(PayrollNew,"CLEARED TIME");
	addAPPlib(PayrollOld,"BY USER");
	addAPPlib(PayrollNew,"BY");
	addAPPlib(PayrollOld,"DISTRIBUTION DATE");
	addAPPlib(PayrollNew,"DISTRIBUTED DATE");
	addAPPlib(PayrollOld,"DISTRIBUTION TIME");
	addAPPlib(PayrollNew,"DISTRIBUTED TIME");
	addAPPlib(PayrollOld,"PROCESSING YEAR");
	addAPPlib(PayrollNew,"FISCAL YEAR");
	addAPPlib(PayrollOld,"YEAR IDENTIFICATION");
	addAPPlib(PayrollNew,"FISCAL YEAR");
*/
}
void makeAPPlibs()
{
	makePayrollAPPlibs();
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,"UTILITIES",
		"IN-HOUSE NAME FIXER")) 
	{
		return;
	}
	if((UserNum=APPOPNNRD("SECURITY","USERS",TRUE,FALSE))==(-1)) return;
	makeAPPlibs();
/* Start PAYROLL */
	fprintf(stdout,"Started Payroll\n");
	change_screens("PAYROLL");
	change_helps("PAYROLL");
	change_filelib("PAYROLL");
	change_reports("PAYROLL");
#ifdef XXX
	change_virtuals("PAYROLL");
#endif
	change_menus("PAYROLL");
	change_imports("PAYROLL");
	change_maintains("PAYROLL");
	change_defaults("PAYROLL");
	fprintf(stdout,"Completed Payroll\n");
	prterr("Completed PAYROLL ");TRACE;
/* Completed PAYROLL */
	CLSNRD(UserNum);
	freeAPPlibs();
	ShutdownSubsystems();
}
