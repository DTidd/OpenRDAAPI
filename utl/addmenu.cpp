/* addmenu.c - Add Menu Option to Screen */
#ifndef WIN32
#define __NAM__ "addmenu.lnx"
#endif
#ifdef WIN32
#define __NAM__ "addmenu.exe"
#endif
#include <app.hpp>
#include <cstdlib>
#ifdef WIN32
#include <io.h>
#endif /* WIN32 */
#include <mix.hpp>
#include <ldval.hpp>
#include <doheader.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void doliblist(RDArsrc *),getscreenlist(RDArsrc *);
static void close_preview_screen(RDArmem *,RDArsrc *);
static void show_preview_screen(RDArsrc *);
RDArsrc *mainrsrc=NULL;

static APPlib *scr_libs;
static APPlib *scr_defs,*new_dir=NULL;
static APPlib *dir_libs,*plist=NULL;
static RDAscrn *screen=NULL;
char *MenuItem=NULL,*MenuLabel=NULL,*MemberName=NULL,*libx=NULL;
int placement=0;

static void doexit(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(scr_libs!=NULL) freeapplib(scr_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(new_dir!=NULL) freeapplib(new_dir);
	if(plist!=NULL) freeapplib(plist);
	if(screen!=NULL) { free_scrn(screen); screen=NULL; }
	if(libx!=NULL) Rfree(libx);
	ShutdownSubsystems();
}
static void doliblist(RDArsrc *mainrsrc)
{
	int selected,selecteds=0,x=0;
	char *libx=NULL;
	char *dirx=NULL;
	APPlib *tmp_defs=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"SCREEN LIST",&selecteds);
	freeapplib(scr_defs);
	if(!RDAstrcmp(scr_libs->libs[selected],"Directory Contains No Libraries"))
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Screen Definitions");
		selecteds=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[selected])+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.SCN",dirx,scr_libs->libs[selected]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.SCN",dirx,scr_libs->libs[selected]);
#endif
		tmp_defs=getlibnames(libx,FALSE);
		scr_defs=APPlibNEW();
		if(tmp_defs==NULL)
		{
			addAPPlib(scr_defs,"Contains No Screen Definitions");
			selecteds=0;
		}  else {
			for(x=0;x<tmp_defs->numlibs;++x)
			{
				if(RDAstrstr(tmp_defs->libs[x]," MENU") || RDAstrstr(tmp_defs->libs[x]," SUBMENU"))
				{
					addAPPlib(scr_defs,tmp_defs->libs[x]);
				}
			}
			freeapplib(tmp_defs);
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(selecteds>=scr_defs->numlibs) selecteds=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN LIST",selecteds,scr_defs))
		updatersrc(mainrsrc,"SCREEN LIST");
}
static void show_preview_screen(RDArsrc *rsrc)
{
	RDArsrc *previewrsrc=NULL;
	RDAwdgt *wdgt;
	short x;
	int l;
	char blankboolean=TRUE;
	APPlib *blanklist=NULL;
	char *visible_expression=NULL;

	previewrsrc=RDArsrcNEW("GUI","PREVIEW");
	addstdrsrc(previewrsrc,"BLANK STDRSRC",VARIABLETEXT,80,NULL,TRUE);
	addstdrsrc(previewrsrc,"BLANK CUSTOM TYPES",CUSTOMTYPE,80,NULL,TRUE);
	l=0;
	if(blanklist==NULL) blanklist=APPlibNEW();
	addAPPlib(blanklist,"Testing Item 1...");
	addAPPlib(blanklist,"Testing Item 2...");
	addAPPlib(blanklist,"Testing Item 3...");
	addlstrsrc(previewrsrc,"BLANK SCROLLEDLIST",&l,TRUE,NULL,blanklist->numlibs,&blanklist->libs,NULL);
	addsctrsrc(previewrsrc,"BLANK SCROLLEDTEXT",0,NULL,TRUE);
	addstdrsrc(previewrsrc,"BLANK TOGGLEBUTTON",BOOLNS,1,&blankboolean,TRUE);
	if(previewrsrc->scn!=NULL) { free_scrn(previewrsrc->scn); previewrsrc->scn=NULL; }
	previewrsrc->scn=RDAscrnNEW("GUI","PREVIEW SCREEN");
	for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
	{
		/* Don't use the expression unless it's FALSE or (FALSE). */
		if((RDAstrcmp(wdgt->expression, "FALSE")) && (RDAstrcmp(wdgt->expression, "(FALSE)")))
		{
			visible_expression=NULL;
		}
		else
		{
			visible_expression=wdgt->expression;
		}
		if(wdgt->type==0)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK STDRSRC",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==6)
		{
			RMEMaddbtnrsrc(previewrsrc,wdgt->resource_name,TRUE,close_preview_screen,mainrsrc);
			ADVaddwdgt(previewrsrc->scn,wdgt->type,wdgt->resource_name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==7)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK SCROLLEDLIST",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==8)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK SCROLLEDTEXT",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==9)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK TOGGLEBUTTON",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==14)
		{
			/* Converts it to a button... */
			RMEMaddbtnrsrc(previewrsrc,wdgt->resource_name,TRUE,close_preview_screen,mainrsrc);
			ADVaddwdgt(previewrsrc->scn,6,wdgt->resource_name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==15)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK CUSTOM TYPES",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if((wdgt->type==5 || wdgt->type==9) && wdgt->rtype==1)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,wdgt->resource_name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if((wdgt->type==6 || wdgt->type==14 )&& wdgt->rtype==5) 
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,wdgt->resource_name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,
				wdgt->editable_expression,wdgt->sensitive_expression,
				wdgt->transversal_expression);
		} else {
			ADVaddwdgt(previewrsrc->scn,wdgt->type,wdgt->resource_name,wdgt->label,
				NULL,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		}
	}
	APPmakescrn(previewrsrc,TRUE,NULL,NULL,FALSE);
}
static void close_preview_screen(RDArmem *member,RDArsrc *mainrsrc)
{
	RDArsrc *rsrc=(RDArsrc *)member->parent;
	RDAscrn *scn=NULL;
	RDAwdgt *wdgt=NULL;
	int x=0,found_at=0;
	char found_it=FALSE,added_it=FALSE;
	char *dashes=NULL;
	int m=0,s=0,d=0;
	
	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"NEW DIRECTORY LIST",&d);
	FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&m);
	FINDRSCGETINT(mainrsrc,"SCREEN LIST",&s);
	FINDRSCGETINT(mainrsrc,"PLACEMENT LIST",&placement);

	if(libx!=NULL) Rfree(libx);
	dashes=adddashes(scr_libs->libs[m]);
	libx=Rmalloc(RDAstrlen(new_dir->libs[d])+RDAstrlen(dashes)+15);
#ifndef WIN32
	sprintf(libx,"%s/%s.SCN",new_dir->libs[d],dashes);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.SCN",new_dir->libs[d],dashes);
#endif
	if(dashes!=NULL) Rfree(dashes);
	if(rsrc->scn!=NULL) { free_scrn(rsrc->scn); rsrc->scn=NULL; }
	scn=RDAscrnNEW(screen->module,screen->name);
	switch(placement)
	{
		default:
		case 0: /* After */
			for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
			{
				if((found_it==TRUE) && (wdgt->type!=2) && (wdgt->type!=4) && (wdgt->type!=13) && (wdgt->type!=1) && (wdgt->type!=3) && (wdgt->type!=12) && (added_it==FALSE))
				{
					addbutton(scn,MenuItem,MenuLabel,"","");
					added_it=TRUE;
				} else if((found_it==TRUE) && ((wdgt->type==1) || (wdgt->type==3) || (wdgt->type==12)) && (added_it==FALSE))
				{
					addnewline(scn);
					addnewbox(scn);
					addnewline(scn);
					addbutton(scn,MenuItem,MenuLabel,"","");
					addendline(scn);
					addendbox(scn);
					addendline(scn);
					added_it=TRUE;
				}
				xaddwdgt(scn,wdgt->type,wdgt->resource_name,wdgt->label,wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,wdgt->cols,wdgt->rtype,wdgt->expression,wdgt->editable_expression,wdgt->sensitive_expression,wdgt->transversal_expression,__LINE__,__FILE__);
				if(added_it==FALSE)
				{
					if(!RDAstrcmp(wdgt->resource_name,member->rscrname))
					{
						found_it=TRUE;
					}
				}
			}
			break;
		case 1: /* Before */
			for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
			{
				if(!RDAstrcmp(wdgt->resource_name,member->rscrname))
				{
					found_at=x;
					break;
				}
			}	
			--found_at;
			wdgt=screen->wdgts+(found_at);
			while(TRUE)
			{
				if((wdgt->type!=1) && (wdgt->type!=3) && (wdgt->type!=12)) break;
				--found_at;

				if(found_at<0) break;
				wdgt=screen->wdgts+(found_at-1);
				
			}
			if(found_at<=0)
			{
				addnewline(scn);
				addnewbox(scn);
				addnewline(scn);
				addbutton(scn,MenuItem,MenuLabel,"","");
				addendline(scn);
				addendbox(scn);
				addendline(scn);
				for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
				{
					xaddwdgt(scn,wdgt->type,wdgt->resource_name,wdgt->label,wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,wdgt->cols,wdgt->rtype,wdgt->expression,wdgt->editable_expression,wdgt->sensitive_expression,wdgt->transversal_expression,__LINE__,__FILE__);
				}

			} else {
				for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
				{
					if(x==found_at)
					{
						addnewline(scn);
						addnewbox(scn);
						addnewline(scn);
						addbutton(scn,MenuItem,MenuLabel,"","");
						addendline(scn);
						addendbox(scn);
						addendline(scn);
					}
					xaddwdgt(scn,wdgt->type,wdgt->resource_name,wdgt->label,wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,wdgt->cols,wdgt->rtype,wdgt->expression,wdgt->editable_expression,wdgt->sensitive_expression,wdgt->transversal_expression,__LINE__,__FILE__);
				}
			}
			break;
		case 2: /* Left */
			for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
			{
				if(!RDAstrcmp(wdgt->resource_name,member->rscrname))
				{
					addbutton(scn,MenuItem,MenuLabel,"","");
				}
				xaddwdgt(scn,wdgt->type,wdgt->resource_name,wdgt->label,wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,wdgt->cols,wdgt->rtype,wdgt->expression,wdgt->editable_expression,wdgt->sensitive_expression,wdgt->transversal_expression,__LINE__,__FILE__);
			}
			break;
		case 3: /* Right */
			for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
			{
				xaddwdgt(scn,wdgt->type,wdgt->resource_name,wdgt->label,wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,wdgt->cols,wdgt->rtype,wdgt->expression,wdgt->editable_expression,wdgt->sensitive_expression,wdgt->transversal_expression,__LINE__,__FILE__);
				if(!RDAstrcmp(wdgt->resource_name,member->rscrname))
				{
					addbutton(scn,MenuItem,MenuLabel,"","");
				}
			}
			break;
	}
	writescrnbin(libx,scn);
	if(rsrc!=NULL)
	{
		killwindow(rsrc);
		free_rsrc(rsrc);
	}
	doexit(mainrsrc);
}
static void okfunction(RDArsrc *mainrsrc)
{
	char *dashes=NULL;
	int m=0,s=0,d=0;
	
	readallwidgets(mainrsrc);
	if(MemberName!=NULL) Rfree(MemberName);
	FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&d);
	FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&m);
	FINDRSCGETINT(mainrsrc,"SCREEN LIST",&s);
	FINDRSCGETSTRING(mainrsrc,"MENU ITEM",&MenuItem);
	FINDRSCGETSTRING(mainrsrc,"MENU LABEL",&MenuLabel);
	FINDRSCGETINT(mainrsrc,"PLACEMENT LIST",&placement);

	if(libx!=NULL) Rfree(libx);
	screen=RDAscrnNEW(scr_libs->libs[m],scr_defs->libs[s]);
	dashes=adddashes(scr_libs->libs[m]);
	libx=Rmalloc(RDAstrlen(dir_libs->libs[d])+RDAstrlen(dashes)+15);
#ifndef WIN32
	sprintf(libx,"%s/%s.SCN",dir_libs->libs[d],dashes);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.SCN",dir_libs->libs[d],dashes);
#endif
	if(dashes!=NULL) Rfree(dashes);
	if(!getscrnbin(libx,screen,FALSE))
	{
		show_preview_screen(mainrsrc);
	} else {
/* How did you get here? */
	}

}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int x,selectedx=0;
	char *tmp=NULL,*Tdirectory=NULL,*dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&selectedx)) selectedx=0;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&Tdirectory)) return;
	if(scr_libs!=NULL) freeapplib(scr_libs);
	scr_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(Tdirectory)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",Tdirectory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",Tdirectory);
#endif
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(scr_libs,tmp);
	}
	if(scr_libs->numlibs<1)
	{
		addAPPlib(scr_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(scr_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(Tdirectory!=NULL) Rfree(Tdirectory);
	if(dirx!=NULL) Rfree(dirx);
	if(selectedx>=scr_libs->numlibs) selectedx=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LIBRARY LIST",selectedx,scr_libs))
		updatersrc(mainrsrc,"LIBRARY LIST");
	doliblist(mainrsrc);
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory)) 
		{
			inlist=TRUE;		
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
		updatersrc(mainrsrc,"DIRECTORY LIST");
	getscreenlist(mainrsrc);
}
static void getdirnew(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"NEW DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"NEW DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,new_dir->libs[s])) 
	{
		if(FINDRSCSETSTRING(mainrsrc,"NEW DIRECTORY",new_dir->libs[s])) return;
			updatersrc(mainrsrc,"NEW DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s])) 
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s])) return;
			updatersrc(mainrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getscreenlist(mainrsrc);
}
static void setnewdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"NEW DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"NEW DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<new_dir->numlibs;++x)
	{
		if(!RDAstrcmp(new_dir->libs[x],directory)) 
		{
			inlist=TRUE;		
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(new_dir,directory);
		x=new_dir->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"NEW DIRECTORY LIST",x,new_dir);
		updatersrc(mainrsrc,"NEW DIRECTORY LIST");
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *defdir=NULL,*MenuItem=NULL,*MenuLabel=NULL;
	int w=2,m=0,s=0,z=0,n=1;

	RDA_SOFTWARE_TYPE=RDA_ALL_APP;
	if(InitializeSubsystems(argc,argv,"GUI","ADD MENU OPTION")) 
	{
		return(-1);
	}
	mainrsrc=RDArsrcNEW("GUI","ADD MENU OPTION");
	scr_libs=APPlibNEW();
	scr_defs=APPlibNEW();
	dir_libs=APPlibNEW();
	new_dir=APPlibNEW();
	plist=APPlibNEW();
	addAPPlib(plist,"After");
	addAPPlib(plist,"Before");
	addAPPlib(plist,"Left");
	addAPPlib(plist,"Right");
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,defdir);
	addAPPlib(new_dir,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	if(!isEMPTY(RDA_GROUP))
	{
	w=3;
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(RDA_GROUP)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
	addAPPlib(dir_libs,defdir);
	addAPPlib(new_dir,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	}
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	addAPPlib(new_dir,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	addAPPlib(new_dir,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	if(argc>1)
	{
		MenuItem=stralloc(argv[1]);
	}
	if(argc>2)
	{
		MenuLabel=stralloc(argv[2]);
	}
	addstdrsrc(mainrsrc,"MENU ITEM",VARIABLETEXT,0,MenuItem,(MenuItem!=NULL ? FALSE:TRUE));
	addstdrsrc(mainrsrc,"MENU LABEL",VARIABLETEXT,0,MenuLabel,TRUE);
	addlstrsrc(mainrsrc,"PLACEMENT LIST",&z,TRUE,NULL,plist->numlibs,&plist->libs,NULL);
	addlstrsrc(mainrsrc,"DIRECTORY LIST",&w,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
	addlstrsrc(mainrsrc,"NEW DIRECTORY LIST",&n,TRUE,getdirnew,new_dir->numlibs,&new_dir->libs,NULL);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[w],TRUE);
	addstdrsrc(mainrsrc,"NEW DIRECTORY",VARIABLETEXT,0,new_dir->libs[n],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",w,dir_libs);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist,NULL);
	FINDRSCSETFUNC(mainrsrc,"NEW DIRECTORY",setnewdirlist,NULL);
	addlstrsrc(mainrsrc,"LIBRARY LIST",m,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
	addlstrsrc(mainrsrc,"SCREEN LIST",s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
	getscreenlist(mainrsrc);
	if(argc>3 && scr_libs!=NULL)
	{
		for(m=0;m<scr_libs->numlibs;++m)
		{
			if(!RDAstrcmp(scr_libs->libs[m],argv[3])) break;
		}
		if(m<scr_libs->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"LIBRARY LIST",m);
		}
		else
		{
			m=0;
			FINDRSCSETINT(mainrsrc,"LIBRARY LIST",m);
		}
		doliblist(mainrsrc);
	}
	if(argc>4 && scr_defs!=NULL)
	{
		for(s=0;s<scr_defs->numlibs;++s)
		{
			if(!RDAstrcmp(scr_defs->libs[s],argv[4])) break;
		}
		if(s<scr_defs->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"SCREEN LIST",s);
		}
		else
		{
			s=0;
			FINDRSCSETINT(mainrsrc,"SCREEN LIST",s);
		}
	}
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	FINDRSCSETINPUTFOCUS(mainrsrc,"LIBRARY LIST");
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
