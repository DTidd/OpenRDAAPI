/* olhfuncs.c - On-Line Help Functions */
#include <olh.hpp>
#include <misc.hpp>

short SCNNOTES_FILENO=(-1);
short USCNNOTES_FILENO=(-1);
short RSCNOTES_FILENO=(-1);
short URSCNOTES_FILENO=(-1);
char WRITE_SCNNOTES=TRUE;
char WRITE_USCNNOTES=TRUE;
char WRITE_RSCNOTES=TRUE;
char WRITE_URSCNOTES=TRUE;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

short (*HELPEXPRESSFUNC)(...);
void (*HELPADDOSFUNC)(...);

static char GetHelpInclusionType()
{
	short filexx=(-1);
	char x=FALSE;

	filexx=OPNNRD("SECURITY","USERS");
	if(filexx!=(-1))
	{
		ZERNRD(filexx);
		FINDFLDSETSTRING(filexx,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(filexx,1))
		{
			FINDFLDGETCHAR(filexx,"HELP INCLUSION TYPE",&x);
		}
		CLSNRD(filexx);
	}
	return(x);
}
void xINITHELP(short (*f)(...),void (*g)(...),int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG INITHELP Initializing the Help System at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(f!=NULL) HELPEXPRESSFUNC=f;
		else HELPEXPRESSFUNC=NULL;
	if(g!=NULL) HELPADDOSFUNC=g;
		else HELPADDOSFUNC=NULL;
	if(CURRENTDIRECTORY==NULL) SETCWD();
	if(USERLOGIN==NULL) SETUSERLOGIN();
	SCNNOTES_FILENO=OPNNRDsec("OLHELP","SCNNOT",TRUE,TRUE,FALSE);
	if(SCNNOTES_FILENO==(-1))
	{
		SCNNOTES_FILENO=OPNNRDsec("OLHELP","SCNNOT",TRUE,FALSE,FALSE);
		WRITE_SCNNOTES=FALSE;
	}
	USCNNOTES_FILENO=OPNNRDsec("OLHELP","USCNNOT",TRUE,TRUE,FALSE);
	if(USCNNOTES_FILENO==(-1))
	{
		USCNNOTES_FILENO=OPNNRDsec("OLHELP","USCNNOT",TRUE,FALSE,FALSE);
		WRITE_USCNNOTES=FALSE;
	}
	RSCNOTES_FILENO=OPNNRDsec("OLHELP","RSCNOT",TRUE,TRUE,FALSE);
	if(RSCNOTES_FILENO==(-1))
	{
		RSCNOTES_FILENO=OPNNRDsec("OLHELP","RSCNOT",TRUE,FALSE,FALSE);
		WRITE_RSCNOTES=FALSE;
	}
	URSCNOTES_FILENO=OPNNRDsec("OLHELP","URSCNOT",TRUE,TRUE,FALSE);
	if(URSCNOTES_FILENO==(-1))
	{
		URSCNOTES_FILENO=OPNNRDsec("OLHELP","URSCNOT",TRUE,FALSE,FALSE);
		WRITE_URSCNOTES=FALSE;
	}
}
void xEXITHELP(int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG EXITHELP at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SCNNOTES_FILENO!=(-1)) CLSNRD(SCNNOTES_FILENO);
	if(USCNNOTES_FILENO!=(-1)) CLSNRD(USCNNOTES_FILENO);
	if(RSCNOTES_FILENO!=(-1)) CLSNRD(RSCNOTES_FILENO);
	if(URSCNOTES_FILENO!=(-1)) CLSNRD(URSCNOTES_FILENO);
}
RDAhelp *xRDAhelpNEW(int line,char *file)
{
	RDAhelp *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG RDAhelpNEW Creating new RDAhelp structure at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAhelp));
	tmp->name=NULL;
	tmp->desc=NULL;
	tmp->rhlp=NULL;
	tmp->numrschs=0;
	return(tmp);
}
RDArhlp *xFINDRHELP(RDAhelp *h,char *name,int line,char *file)
{
	int x=0;
	RDArhlp *r=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG FINDRHELP Find Resource Help Element [%s] on Screen [%s] from RDAhelp structure at line [%d] program [%s].",name,h->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(h!=NULL)
	{
		if(h->rhlp!=NULL)
		{
			for(x=0,r=h->rhlp;x<h->numrschs;++x,++r)
			{
				if(!RDAstrcmp(r->name,name)) return(r);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("Error FINDRHELP Resource Help Element [%s] doesn't exist for Screen [%s] at line [%d] program [%s].",name,h->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(NULL);
}
void xfree_help(RDAhelp *h,int line,char *file)
{
	int x;
	RDArhlp *r;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG free_help Freeing RDAhelp structure for Screen [%s] at line [%d] program [%s].",h->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(h!=NULL)
	{
		if(h->name!=NULL) Rfree(h->name);
		if(h->desc!=NULL) Rfree(h->desc);
		if(h->rhlp!=NULL)
		{
			for(x=0,r=h->rhlp;x<h->numrschs;++x,++r)
			{
				if(r->name!=NULL) Rfree(r->name);
				if(r->desc!=NULL) Rfree(r->desc);
				if(r->label!=NULL) Rfree(r->label);
			}
			Rfree(h->rhlp);
		}
		Rfree(h);
	}
}
void getresourcedesc(char *module,char *screen,char *resource,char **tmp,
	char **label)
{
	int x;
	RDAhelp *help;
	RDArhlp *rhlp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG getresourcedesc Get Resource Description for Module [%s] Screen [%s] Resource [%s].",module,screen,resource);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	help=findhelpbin(module,screen);
	if(help!=NULL)
	{
		for(x=0,rhlp=help->rhlp;x<help->numrschs;++x,++rhlp)
		{
			if(!RDAstrcmp(rhlp->name,resource))
			{
				*tmp=stralloc(rhlp->desc);
				if(!isEMPTY(rhlp->label)) *label=stralloc(rhlp->label);
					else *label=stralloc(rhlp->name);
				break;
			}
		}
		if(x>=help->numrschs)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagolh || diagolh_field)
			{
			prterr("Error Help for Resource [%s] doesn't exist for Module [%s] Screen [%s].",resource,module,screen);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		}
		free_help(help);
	} else {
		prterr("Error Help for Module [%s] Screen [%s] not found.",module,screen); 
	}
}
char *getrscnotes(char *module,char *screen,char *resource)
{
	char *tmp=NULL;
	char deleteflag=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG getrscnotes Get Resource Notes for Module [%s] Screen [%s] Resource [%s].",module,screen,resource);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(RSCNOTES_FILENO!=(-1))
	{
		FINDFLDSETSTRING(RSCNOTES_FILENO,"MODULE NAME",(module!=NULL ? module:""));
		FINDFLDSETSTRING(RSCNOTES_FILENO,"SCREEN NAME",(screen!=NULL ? screen:""));
		FINDFLDSETSTRING(RSCNOTES_FILENO,"RESOURCE NAME",(resource!=NULL ? resource:""));
		if(!EQLNRDsec(RSCNOTES_FILENO,1))
		{
			FINDFLDGETCHAR(RSCNOTES_FILENO,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(RSCNOTES_FILENO,"NOTES",&tmp);
			}
		}
	} else {
		prterr("Error couldn't retrieve resource notes because file [RSCNOT] wasn't opened or doesn't exist.");
	}
	return tmp;
}
char *getusrrscnotes(char *module,char *screen,char *resource)
{
	char *tmp=NULL;
	char deleteflag=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG getusrrscnotes Getting User Resource Notes for Module [%s] Screen [%s] Resource [%s].",module,screen,resource);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(URSCNOTES_FILENO!=(-1))
	{
		FINDFLDSETSTRING(URSCNOTES_FILENO,"MODULE NAME",(module!=NULL ? module:""));
		FINDFLDSETSTRING(URSCNOTES_FILENO,"SCREEN NAME",(screen!=NULL ? screen:""));
		FINDFLDSETSTRING(URSCNOTES_FILENO,"RESOURCE NAME",(resource!=NULL ? resource:""));
		FINDFLDSETSTRING(URSCNOTES_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRDsec(URSCNOTES_FILENO,1))
		{
			FINDFLDGETCHAR(URSCNOTES_FILENO,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(URSCNOTES_FILENO,"NOTES",&tmp);
			}
		}
	} else {
		prterr("Error couldn't retrieve user resource notes because file [URSCNOT] wasn't opened or doesn't exist.");
	}
	return tmp;
}
void quit_resource_help(RDArsrc *parent,WriteRsrcHelp *WR)
{
	RDArsrc *rsrc;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG quit_resource_help Quit Resource Help Window [%s] [%s].",parent->module,parent->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(WR!=NULL)
	{
		rsrc=WR->rsrc;
		free_rsrc(rsrc);
		Rfree(WR);
	}
}
void saverscnotes(RDArsrc *xparent,WriteRsrcHelp *WR)
{
	RDArmem *member;
	RDArsrc *parent,*rsrc;
	char *notes=NULL;
	char deleteflag=FALSE;

	if(WR!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagolh || diagolh_field)
		{
			prterr("DIAG saverscnotes Saving Resource Notes from Module [%s] Screen [%s].",xparent->module,xparent->screen);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		member=WR->member;
		rsrc=WR->rsrc;
		parent=member->parent;
		ZERNRD(RSCNOTES_FILENO);
		FINDFLDSETSTRING(RSCNOTES_FILENO,"MODULE NAME",parent->module);
		FINDFLDSETSTRING(RSCNOTES_FILENO,"SCREEN NAME",parent->screen);
		FINDFLDSETSTRING(RSCNOTES_FILENO,"RESOURCE NAME",member->rscrname);
		readwidget(rsrc,"RESOURCE NOTES");
		if(!FINDRSCGETSTRING(rsrc,"RESOURCE NOTES",&notes))
		{
			FINDFLDSETSTRING(RSCNOTES_FILENO,"NOTES",notes);
		}
		FINDFLDSETCHAR(RSCNOTES_FILENO,"DELETEFLAG",deleteflag);
		WRTTRANSsec(RSCNOTES_FILENO,0,NULL,NULL);
		if(notes!=NULL) Rfree(notes);
	}
}
void saveurscnotes(RDArsrc *xparent,WriteRsrcHelp *WR)
{
	RDArmem *member;
	RDArsrc *parent,*rsrc;
	char *notes=NULL;
	char deleteflag=FALSE;

	if(WR!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagolh || diagolh_field)
		{
			prterr("DIAG saveurscnotes Saving User's Resource Notes from Module [%s] Screen [%s].",xparent->module,xparent->screen);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		member=WR->member;
		rsrc=WR->rsrc;
		parent=member->parent;
		ZERNRD(URSCNOTES_FILENO);
		FINDFLDSETSTRING(URSCNOTES_FILENO,"MODULE NAME",parent->module);
		FINDFLDSETSTRING(URSCNOTES_FILENO,"SCREEN NAME",parent->screen);
		FINDFLDSETSTRING(URSCNOTES_FILENO,"RESOURCE NAME",member->rscrname);
		FINDFLDSETSTRING(URSCNOTES_FILENO,"USER IDENTIFICATION",USERLOGIN);
		readwidget(rsrc,"USER RESOURCE NOTES");
		if(!FINDRSCGETSTRING(rsrc,"USER RESOURCE NOTES",&notes))
		{
			FINDFLDSETSTRING(URSCNOTES_FILENO,"NOTES",notes);
		}
		FINDFLDSETCHAR(URSCNOTES_FILENO,"DELETEFLAG",deleteflag);
		WRTTRANSsec(URSCNOTES_FILENO,0,NULL,NULL);
		if(notes!=NULL) Rfree(notes);
	}
}
void RDA_User_Profile()
{
	APPlib *args=NULL,*envx=NULL;

	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"USER PROFILE");
	envx=APPlibNEW();
	memset(stemp,0,101);
	sprintf(stemp,"ADD_USER_IDENTIFICATION=%s",USERLOGIN);
	addAPPlib(envx,stemp);
	ADVExecuteProgram("doadd",args,envx);
	if(args!=NULL) freeapplib(args);
	if(envx!=NULL) freeapplib(envx);
}
static void systemprintmgt(RDArsrc *parent)
{
#ifdef WIN32
	APPlib *args=NULL;
	char *windir=NULL;
	char *explorer_path=NULL;

	windir=RDA_GetEnv("WINDIR");
	explorer_path=Rmalloc((strlen(windir))+20);
	memset(explorer_path,0,(strlen(windir))+20);
	sprintf(explorer_path,"%s\\Explorer");
	
	prterr("WinDir [%s],   Explorer [%s]\n",windir,explorer_path);
	args=APPlibNEW();
	addAPPlib(args,"::{2227A280-3AEA-1069-A2DE-08002B30309D}");
	ExecuteProgram(explorer_path,args);
	if(args!=NULL) freeapplib(args);
	if(explorer_path!=NULL) free(explorer_path);
#endif
#ifndef WIN32
	GUI_OpenURL("http://127.0.0.1:631/printers");
#endif
}
#ifndef WIN32
static void sendmyvnc(RDArsrc *parent)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"SENDMYVNC");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
#endif
static void wikisupport(RDArsrc *parent,RDArsrc *mainrsrc)
{
	WikiSupport(mainrsrc);
}
static void cybersupportr(RDArsrc *parent,WriteRsrcHelp *hlp)
{
	APPlib *args=NULL,*envpx=NULL;
	char temp1[512];
	RDArsrc *rsrc=NULL;

	if(hlp->member==NULL) return;
	rsrc=(RDArsrc *)hlp->member->parent;
	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"CYBERSUPPORT");
	envpx=APPlibNEW();
	sprintf(temp1,"ADD_PRODUCT=%s",((rsrc->module!=NULL ? rsrc->module:"")));
	addAPPlib(envpx,temp1);
	memset(temp1,0,512);
	sprintf(temp1,"ADD_PROCESS=%s",(rsrc->screen!=NULL ? rsrc->screen:""));
	addAPPlib(envpx,temp1);
	ADVExecuteProgram("doadd",args,envpx);
	if(args!=NULL) freeapplib(args);
	if(envpx!=NULL) freeapplib(envpx);
	quit_resource_help(parent,hlp);
}
static void rda_website(RDArsrc *r)
{
	GUI_OpenURL("http://www.openrda.net");
}
static void rda_feedback(RDArsrc *r)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"ADD FEEDBACK");
	ADVExecuteProgram("doadd",args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void rda_newsletters(RDArsrc *r)
{
	GUI_OpenURL("http://openrda.net/rda-announcements");
}
static void rda_announcements(RDArsrc *r)
{
	if(XPERT_SETUP->software_type<2) 
	{
		GUI_OpenURL("http://openrda.net/rda-announcements");
	} else {
		GUI_OpenURL("http://openrda.net/openrda-documentation-downloads");
	}
}
static void rda_videos(RDArsrc *r)
{
	GUI_OpenURL("http://www.openrda.net/videos/");
}
static void rda_docs(RDArsrc *r)
{
	if(XPERT_SETUP->software_type<2) 
	{
		GUI_OpenURL("http://openrda.net/openrda-documentation-downloads");
	} else {
		GUI_OpenURL("http://openrda.net/openrda-documentation-downloads");
	}
}
static void copy_menu(RDArsrc *r,RDArmem *member)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,member->rscrname);
	addAPPlib(args,member->label);
	ADVExecuteProgram("addmenu",args,NULL);
	if(args!=NULL) freeapplib(args);
}
void resourcehelp(RDArsrc *parent,RDArmem *member)
{
	RDArsrc *resourcersrc=NULL;
	char *resourcedesc=NULL,*rscnotes=NULL,*usrrscnotes=NULL;
	char *resourcelabel=NULL;
	RDArsrc *rsrcx;
	NRDfield *f;
	short length=0;
	WriteRsrcHelp *WR=NULL;

	rsrcx=parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG ResourceHelp Creating Resource Help Window for member [%s] of Module [%s] Screen [%s].",member->rscrname,rsrcx->module,rsrcx->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	resourcersrc=RDArsrcNEW("OLHELP","RESOURCE HELP SCREEN");
	getresourcedesc(rsrcx->module,rsrcx->screen,member->rscrname,
		&resourcedesc,&resourcelabel);
	addstdrsrc(resourcersrc,"RESOURCE NAME",VARIABLETEXT,0,member->rscrname,
		FALSE);
	addstdrsrc(resourcersrc,"RESOURCE LABEL",VARIABLETEXT,0,(resourcelabel!=NULL ? resourcelabel:member->label),FALSE);
	if(resourcelabel!=NULL) Rfree(resourcelabel);
	addstdrsrc(resourcersrc,"RESOURCE TYPE",VARIABLETEXT,0,
		standardfieldtypes[member->field_type],FALSE);
	addstdrsrc(resourcersrc,"RESOURCE LENGTH",SHORTV,5,&member->field_length,FALSE);
	addstdrsrc(resourcersrc,"EDITABLE RESOURCE",BOOLNS,1,&member->editable,FALSE);
	if(!isEMPTY(resourcedesc))
	{
		if(member->editable) resourcedesc=stralloccat(resourcedesc,"\n\nEDITABLE STATUS: This resource is currently editable.");
		else resourcedesc=stralloccat(resourcedesc,"\n\nEDITABLE STATUS: This resource is currently not editable.");
	} else {
		if(member->editable) resourcedesc=stralloccat(resourcedesc,"EDITABLE STATUS: This resource is currently editable.");
		else resourcedesc=stralloccat(resourcedesc,"EDITABLE STATUS: This resource is currently not editable.");
	}
	addstdrsrc(resourcersrc,"RESOURCE DESCRIPTION",SCROLLEDTEXT,0,
		resourcedesc,FALSE);
	if(resourcedesc!=NULL) Rfree(resourcedesc);
	rscnotes=getrscnotes(rsrcx->module,rsrcx->screen,member->rscrname);
	f=FLDNRD(URSCNOTES_FILENO,"NOTES");
	if(f==NULL) length=0;
		else length=f->len;
	addstdrsrc(resourcersrc,"RESOURCE NOTES",SCROLLEDTEXT,length,
		rscnotes,WRITE_RSCNOTES);
	if(rscnotes!=NULL) Rfree(rscnotes);
	usrrscnotes=getusrrscnotes(rsrcx->module,rsrcx->screen,
		member->rscrname);
	f=FLDNRD(URSCNOTES_FILENO,"NOTES");
	if(f==NULL) length=0;
		else length=f->len;
	addstdrsrc(resourcersrc,"USER RESOURCE NOTES",SCROLLEDTEXT,length,
		usrrscnotes,WRITE_URSCNOTES);
	if(usrrscnotes!=NULL) Rfree(usrrscnotes);
	WR=Rmalloc(sizeof(WriteRsrcHelp));
	WR->member=member;
	WR->rsrc=resourcersrc;
	if(WRITE_RSCNOTES)
	{
		addbtnrsrc(resourcersrc,"SAVE RESOURCE NOTES",TRUE,saverscnotes,WR);
	}
	if(WRITE_URSCNOTES)
	{
		addbtnrsrc(resourcersrc,"SAVE USER RESOURCE NOTES",TRUE,saveurscnotes,WR);
	}
	addrfkwrsrc(resourcersrc,"CYBERSUPPORT",TRUE,cybersupportr,WR);
	addrfkwrsrc(resourcersrc,"WIKI SUPPORT",TRUE,wikisupport,rsrcx);
#ifndef WIN32
	addbtnrsrc(resourcersrc,"SENDMYVNC",TRUE,sendmyvnc,NULL);
#endif 
	addbtnrsrc(resourcersrc,"SYSTEM PRINTER MANAGEMENT",TRUE,systemprintmgt,NULL);
	addbtnrsrc(resourcersrc,"ANNOUNCEMENTS",TRUE,rda_announcements,NULL);
	addbtnrsrc(resourcersrc,"NEWSLETTERS",TRUE,rda_newsletters,NULL);
	addbtnrsrc(resourcersrc,"FEEDBACK",TRUE,rda_feedback,NULL);
	addbtnrsrc(resourcersrc,"USER PROFILE",TRUE,RDA_User_Profile,NULL);
	if(XPERT_SETUP->software_type<2) 
	{
	addbtnrsrc(resourcersrc,"VIDEOS",TRUE,rda_videos,NULL);
	}
	addbtnrsrc(resourcersrc,"WWW.RDASYS.COM",TRUE,rda_website,NULL);
	addbtnrsrc(resourcersrc,"ON-LINE DOCUMENTS",TRUE,rda_docs,NULL);
	addrfkwrsrc(resourcersrc,"QUIT",TRUE,quit_resource_help,WR);
	addbtnrsrc(resourcersrc,"COPY MENU",TRUE,copy_menu,member);
	if(!strncmp(PROGRAM_NAME,"domenu",6))
	{
		FINDRSCSETEDITABLE(resourcersrc,"COPY MENU",TRUE);
		FINDRSCSETSENSITIVE(resourcersrc,"COPY MENU",TRUE);
	} else {
		FINDRSCSETEDITABLE(resourcersrc,"COPY MENU",FALSE);
		FINDRSCSETSENSITIVE(resourcersrc,"COPY MENU",FALSE);
	}
	addbtnrsrc(resourcersrc,"PRINT RESOURCES",TRUE,printrsrcs,parent);
	addbtnrsrc(resourcersrc,"HELP",TRUE,screenhelp,NULL);
	if(xmakescrn(resourcersrc,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,NULL,NULL,NULL,NULL,parent,__LINE__,__FILE__)) 
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom help RESOURCE HELP SCREEN. Check to see the help is available. If it is, call your installer.",NULL,FALSE);
		prterr("Error RESOURCE HELP SCREEN was not found and/or could not be created.");
		quit_resource_help(resourcersrc,WR);
	}
}
static void addmemberscreendesc(char **tmp,int *count,RDArsrc *rsrc,
	RDAhelp *help,char *name,char *label,short type,RDArmem *member)
{
	RDArhlp *rhlp=NULL;
	char *temp=NULL,*q=NULL;
	short lf=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG addmemberscreendesc for [%s] for Screen [%s] [%s].",name,rsrc->module,rsrc->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rhlp=FINDRHELP(help,name);
	if(rhlp!=NULL)
	{
/* Format Resource Help Here */
		if(!isEMPTY(rhlp->desc))
		{
		if(member->editable) rhlp->desc=stralloccat(rhlp->desc,"\n\nEDITABLE STATUS: This resource is currently editable.");
		else rhlp->desc=stralloccat(rhlp->desc,"\n\nEDITABLE STATUS: This resource is currently not editable.");
		} else {
		if(member->editable) rhlp->desc=stralloccat(rhlp->desc,"EDITABLE STATUS: This resource is currently editable.");
		else rhlp->desc=stralloccat(rhlp->desc,"EDITABLE STATUS: This resource is currently not editable.");
		}
		*tmp=Rrealloc(*tmp,*count+(rhlp->label!=NULL ? RDAstrlen(rhlp->label):RDAstrlen(rhlp->name))+RDAstrlen(rhlp->desc)+5);
		temp=*tmp+*(int *)count;
		sprintf(temp,"%s: %s\n\n",(rhlp->label!=NULL ? 
			rhlp->label:rhlp->name),rhlp->desc);
		*count+=((int)(rhlp->label!=NULL ? RDAstrlen(rhlp->label):RDAstrlen(rhlp->name))+RDAstrlen(rhlp->desc)+4);
/* Format Resource Help Above Here */
		q=getrscnotes(rsrc->module,rsrc->screen,rhlp->name);
		if(q!=NULL)
		{
			*tmp=Rrealloc(*tmp,*count+19+RDAstrlen(q));
			temp=*tmp+*(int *)count;
			sprintf(temp,"Resource Notes:\n%s\n\n",q);
			*count+=(RDAstrlen(q)+18);
			Rfree(q);
		}
		q=getusrrscnotes(rsrc->module,rsrc->screen,rhlp->name);
		if(q!=NULL)
		{
			*tmp=Rrealloc(*tmp,*count+24+RDAstrlen(q));
			temp=*tmp+*(int *)count;
			sprintf(temp,"User Resource Notes:\n%s\n\n",q);
			*count+=(RDAstrlen(q)+23);
			Rfree(q);
		}
	} else if(!type || type==8 || type==9 || 
		type==7 || type==6)
	{
		if(type!=6)
		{
			lf=FINDVIRTUAL(rsrc,name);
		} else lf=(-1);
		if(lf!=(-1))
		{
			q=getvirtualdesc(rsrc->module,name);
			*tmp=Rrealloc(*tmp,*count+(q!=NULL ? RDAstrlen(q):0)+(label!=NULL?RDAstrlen(label):RDAstrlen(name))+36);
			temp=*tmp+*(int *)count;
			sprintf(temp,"%s: %s This resource is DISPLAY only.\n\n",(label!=NULL?label:name),(q!=NULL ? q:""));
			*count+=(RDAstrlen(q)+(label!=NULL?RDAstrlen(label):RDAstrlen(name))+35);
			if(q!=NULL) Rfree(q);
			q=NULL;
		} else if(member!=NULL)
		{
			if(member->editable)
			{
				q=Rmalloc(96+(label!=NULL ? RDAstrlen(label):RDAstrlen(name))+RDAstrlen(standardfieldtypes[member->field_type]));
				sprintf(q,"%s: There is NO On-Line Help available.  It is a %s resource.  This resource is currently editable.",(label!=NULL ? label:name),standardfieldtypes[member->field_type]);
			} else {
				q=Rmalloc(100+(label!=NULL ? RDAstrlen(label):RDAstrlen(name))+RDAstrlen(standardfieldtypes[member->field_type]));
				sprintf(q,"%s: There is NO On-Line Help available.  It is a %s resource.  This resource is currently not editable.",(label!=NULL ? label:name),standardfieldtypes[member->field_type]);
			}
			*tmp=Rrealloc(*tmp,*count+RDAstrlen(q)+3);
			temp=*tmp+*(int *)count;
			sprintf(temp,"%s\n\n",q);
			*count+=(RDAstrlen(q)+2);
			if(q!=NULL) Rfree(q);
			q=NULL;
		}
	} else if(type==14)
	{
		q=Rmalloc(68+RDAstrlen(name)+RDAstrlen(label));
		sprintf(q,"%s: This is an Optional Screen Button used to display the screen of %s.",label,name);
		*tmp=Rrealloc(*tmp,*count+RDAstrlen(q)+3);
		temp=*tmp+*(int *)count;
		sprintf(temp,"%s\n\n",q);
		*count+=(RDAstrlen(q)+2);
		Rfree(q);
		q=NULL;
	}
}
short Member2Widget(RDArmem *member)
{
	short type=0;

	if(member->field_type==SCROLLEDLIST) type=7;
	else if(member->field_type==SCROLLEDTEXT) type=8;
	else if(member->field_type==BOOLNS)	type=9;
	else if(member->field_type==BUTTONS) type=6;
	else type=0;
	return(type);
}
char *getscreendesc(RDArsrc *rsrc)
{
	int count=0,z;
	char *tmp=NULL,*temp=NULL,y;
	RDAhelp *help;
	RDAwdgt *wdgt;
	short type=FALSE;
	RDArmem *mem=NULL;
	short lf=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG getscreendesc Getting Screen's Help Description for Screen [%s] [%s].",rsrc->module,rsrc->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	help=findhelpbin(rsrc->module,rsrc->screen);
	if(help!=NULL)
	{
		rsrc->scn=RDAscrnNEW(rsrc->module,rsrc->screen);
		if(findscrnbin(rsrc->scn)) 
		{
			prterr("Error couldn't read help for Screen [%s] [%s] and/or Help Library [%s.HLP] doesn't exist.",(rsrc->module!=NULL ? rsrc->module:""),(rsrc->screen!=NULL ? rsrc->screen:""),rsrc->module);
			free_scrn(rsrc->scn);
			free_help(help);
			return(NULL);
		} 
		if(help->desc!=NULL)
		{
			tmp=Rmalloc(RDAstrlen(help->desc)+1);
			tmp=stralloc(help->desc);
			count+=RDAstrlen(help->desc);
		}
		temp="\n\nDISPLAYED RESOURCES\n-------------------\n\n";
		tmp=Rrealloc(tmp,count+RDAstrlen(temp)+1);
		sprintf(&tmp[count],"%s",temp);
		count+=RDAstrlen(temp);
		for(z=0,y=FALSE,wdgt=rsrc->scn->wdgts;z<rsrc->scn->numwdgts;++z,++wdgt)
		{
			if(!isEMPTY(wdgt->resource_name))
			{
				lf=FINDRSC(rsrc,wdgt->resource_name);
				if(lf!=(-1))
				{
					if(y)
					{
						temp="-----------------------------------------------------------------------------\n\n";
						tmp=Rrealloc(tmp,count+RDAstrlen(temp)+1);
						sprintf(&tmp[count],"%s",temp);
						count+=RDAstrlen(temp);
					} else y=TRUE;
					mem=rsrc->rscs+lf;
					addmemberscreendesc(&tmp,&count,rsrc,help,
						wdgt->resource_name,wdgt->label,wdgt->type,mem);
				}
			}
		}
		if(rsrc->scn!=NULL) free_scrn(rsrc->scn);
		rsrc->scn=NULL;
	}
	if(GetHelpInclusionType())
	{
		temp="\nHIDDEN RESOURCES\n----------------\n\n";
		tmp=Rrealloc(tmp,count+RDAstrlen(temp)+1);
		sprintf(&tmp[count],"%s",temp);
		count+=RDAstrlen(temp);
		for(z=0,y=FALSE,mem=rsrc->rscs;z<rsrc->numrscs;++z,++mem)
		{
			if(!isEMPTY(mem->rscrname) && (!IsWidgetDisplayed(mem))) 
			{
				if(y)
				{
					temp="-----------------------------------------------------------------------------\n\n";
					tmp=Rrealloc(tmp,count+RDAstrlen(temp)+1);
					sprintf(&tmp[count],"%s",temp);
					count+=RDAstrlen(temp);
				} else y=TRUE;
				type=Member2Widget(mem);
				addmemberscreendesc(&tmp,&count,rsrc,help,mem->rscrname,
					NULL,type,mem);
			}
		}
	}
	if(help!=NULL) free_help(help);
	return tmp;
}
char *getscreennotes(char *module,char *screen)
{
	char *tmp=NULL;
	char deleteflag=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG getscreennotes Getting Screen Notes for Module [%s] Screen [%s].",module,screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SCNNOTES_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SCNNOTES_FILENO,"MODULE NAME",module);
		FINDFLDSETSTRING(SCNNOTES_FILENO,"SCREEN NAME",screen);
		if(!EQLNRDsec(SCNNOTES_FILENO,1))
		{
			FINDFLDGETCHAR(SCNNOTES_FILENO,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(SCNNOTES_FILENO,"NOTES",&tmp);
			}
		}
	} else {
		prterr("Error couldn't retrieve screen notes because file [SCNNOT] wasn't opened or doesn't exist.");
	}
	return tmp;
}
char *getusrscreennotes(char *module,char *screen)
{
	char *tmp=NULL;
	char deleteflag=FALSE;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG getusrscreennotes Get User Screen Notes for Module [%s] Screen [%s].",module,screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(USCNNOTES_FILENO!=(-1))
	{
		FINDFLDSETSTRING(USCNNOTES_FILENO,"MODULE NAME",module);
		FINDFLDSETSTRING(USCNNOTES_FILENO,"SCREEN NAME",screen);
		FINDFLDSETSTRING(USCNNOTES_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRDsec(USCNNOTES_FILENO,1))
		{
			FINDFLDGETCHAR(USCNNOTES_FILENO,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(USCNNOTES_FILENO,"NOTES",&tmp);
			}
		}
	} else {
		prterr("Error couldn't retrieve user screen notes because file [USCNNOT] wasn't opened or doesn't exist.");
	}
	return tmp;
}
void quit_screen_help(RDArsrc *parent,WriteScnHelp *w)
{
	RDArsrc *rsrc;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG quit_screen_help Quit Screen Help [%s] [%s].",parent->module,parent->screen); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(w!=NULL)
	{
		rsrc=w->helprsrc;
		if(rsrc!=NULL) free_rsrc(rsrc);
		Rfree(w);
	}
}
void saveuscnnotes(RDArsrc *parent,WriteScnHelp *WS)
{
	RDArsrc *helprsrc=NULL,*rsrc=NULL;
	char *notes=NULL,deleteflag=FALSE;

	if(WS!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagolh || diagolh_field)
		{
			prterr("DIAG saveuscnnotes Saving User Screen Notes for Screen [%s] [%s].",WS->rsrcx->module,WS->rsrcx->screen);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		/*
		helprsrc=WS->helprsrc;
		*/
		helprsrc=parent;
		rsrc=WS->rsrcx;
		ZERNRD(USCNNOTES_FILENO);
		FINDFLDSETSTRING(USCNNOTES_FILENO,"MODULE NAME",rsrc->module);
		FINDFLDSETSTRING(USCNNOTES_FILENO,"SCREEN NAME",rsrc->screen);
		FINDFLDSETCHAR(USCNNOTES_FILENO,"DELETEFLAG",deleteflag);
		FINDFLDSETSTRING(USCNNOTES_FILENO,"USER IDENTIFICATION",USERLOGIN);
		readwidget(helprsrc,"USER SCREEN NOTES");
		if(!FINDRSCGETSTRING(helprsrc,"USER SCREEN NOTES",&notes))
		{
			FINDFLDSETSTRING(USCNNOTES_FILENO,"NOTES",notes);
			FINDRSCSETSTRING(WS->helprsrc,"USER SCREEN NOTES",notes);
		}
		WRTTRANSsec(USCNNOTES_FILENO,0,NULL,NULL);
		if(notes!=NULL) Rfree(notes);
/*
		if(WS!=NULL) Rfree(WS);
*/
	}
}
void savescnnotes(RDArsrc *parent,WriteScnHelp *WS)
{
	RDArsrc *helprsrc=NULL,*rsrc=NULL;
	char *notes=NULL,deleteflag=FALSE;

	if(WS!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagolh || diagolh_field)
		{
			prterr("DIAG Savescnnotes Saving Screen Notes for Screen [%s] [%s].",WS->rsrcx->module,WS->rsrcx->screen);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		/*
		helprsrc=WS->helprsrc;
		*/
		helprsrc=parent;
		rsrc=WS->rsrcx;
		ZERNRD(SCNNOTES_FILENO);
		FINDFLDSETSTRING(SCNNOTES_FILENO,"MODULE NAME",rsrc->module);
		FINDFLDSETSTRING(SCNNOTES_FILENO,"SCREEN NAME",rsrc->screen);
		FINDFLDSETCHAR(SCNNOTES_FILENO,"DELETEFLAG",deleteflag);
		readwidget(helprsrc,"SCREEN NOTES");
		if(!FINDRSCGETSTRING(helprsrc,"SCREEN NOTES",&notes))
		{
			FINDFLDSETSTRING(SCNNOTES_FILENO,"NOTES",notes);
			FINDRSCSETSTRING(WS->helprsrc,"SCREEN NOTES",notes);
		}
		WRTTRANSsec(SCNNOTES_FILENO,0,NULL,NULL);
		if(notes!=NULL) Rfree(notes);
/*
		if(WS!=NULL) Rfree(WS);
*/
	}
}
void quit_print_section(RDArsrc *parent,PrintSection *p)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG quit_print_section Quiting Help's Print Section Screen [%s] [%s].",parent->module,parent->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(p!=NULL)
	{
		if(p->prsrc!=NULL) free_rsrc(p->prsrc);
		Rfree(p);
	}
}
void print_header(RDA_PFILE *fp,int *lines,int *pages,
	char *header,unsigned line_length,char *module,char *screen)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
	*lines+=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(17+RDAstrlen(module)+RDAstrlen(screen)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(17+RDAstrlen(module)+RDAstrlen(screen)))/2)-14)));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(module)+RDAstrlen(screen)+14+17+lead_ws+tail_ws)>(line_length))
	{
		RDA_fprintf(fp,"%*s%s Module %*sDate: %s\n",
			(int)(round(((double)(line_length-(8+RDAstrlen(module)))/2))),"",
			module,
			(int)(Rint((((double)(line_length-(8+RDAstrlen(module)))/2)-14))),"",
			date);
		RDA_fprintf(fp,"%*s%s Screen %*s\n",
			(int)(round(((double)(line_length-(8+RDAstrlen(screen)))/2))),"",
			screen,
			(int)(Rint((((double)(line_length-(8+RDAstrlen(screen)))/2)-14))),"");
		*lines+=2;
	} else {
		RDA_fprintf(fp,"%*s %s Module %s Screen %*sDate: %s\n",
			lead_ws,"",module,screen,tail_ws,"",date);
		*lines+=1;
	}
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+14+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime: %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
			timex);
		*lines+=1;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime: %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		*lines+=1;
	}
	RDA_fprintf(fp,"\n");
	*lines+=1;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
void fixoutput(RDA_PFILE *fp,char *data,int *lines,int *pages,
		unsigned line_length,char *module,char *screen)
{
	unsigned y=0,x=0,length=0;
	char *temp=NULL;

	if(!isEMPTY(data))
	{
		if(strchr("\n",data[0])==NULL)
		{
		for(y=0;((strchr("\n\t\r ",data[y])!=NULL) && (data[y]!='\0'));++y)
		{
		}	
		for(x=y;(((x-y)<(line_length-1)) && (data[x]!='\0'));++x)
		{
			if(strchr("\t",data[x])) 
			{ 
				x+=7;
			} else if(strchr("\n",data[x])!=NULL) 
			{
				data[x]=' ';
				++x;
				break;
			}
		}
		while(TRUE)
		{
			if((strchr("\n\t\r ",data[x]))!=NULL) break;
			--x;
		}
		} else { 
			data[0]=' '; 
			++x;
		}
		if((*lines+1)>58)
		{
			RDA_fprintf(fp,"\f");
			temp=stralloc("XPERT ON-LINE HELP SYSTEM");
			print_header(fp,lines,pages,temp,line_length,module,screen);
			if(temp!=NULL) Rfree(temp);
		}
		++*lines;
		length=(RDAstrlen(data)-y);
		temp=Rmalloc(length+1);
		memset(temp,0,length+1);
		memcpy(temp,&data[y],(x-y));
		RDA_fprintf(fp,"%s\n",temp);
		if(temp!=NULL) Rfree(temp);
		if(data[x]!='\0')
		{
			fixoutput(fp,&data[x],lines,pages,line_length,module,screen);
		} else {
			return;
		}
	} 
}
void printfunction(RDArsrc *parent,PrintSection *s)
{
	RDA_PFILE *fp;
	char *output=NULL,*temp=NULL,*notes=NULL;
	RDArsrc *rsrc,*hrsrc,*prsrc;
	int lines=0,pages=0;
	unsigned line_length=79;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG printfunction Printing Help for Screen [%s] [%s].",s->rsrc->module,s->rsrc->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rsrc=s->rsrc;
	hrsrc=s->hrsrc;
	prsrc=s->prsrc;
	readwidget(prsrc,"OUTPUT DEVICE");
	FINDRSCGETSTRING(prsrc,"OUTPUT DEVICE",&output);
	if(output!=NULL)
	{
		fp=RDA_popen(output,"w");
		if(fp!=NULL)
		{
			temp=stralloc("XPERT ON-LINE HELP SYSTEM");
			print_header(fp,&lines,&pages,temp,line_length,rsrc->module,rsrc->screen);
			if(temp!=NULL) Rfree(temp);
			FINDRSCGETSTRING(hrsrc,"SCREEN DESCRIPTION",&notes);
			if(notes!=NULL)
			{
				fixoutput(fp,notes,&lines,&pages,line_length,rsrc->module,rsrc->screen);
				Rfree(notes);
			}
			FINDRSCGETSTRING(hrsrc,"SCREEN NOTES",&notes);
			if(notes!=NULL)
			{
				temp=Rmalloc(RDAstrlen(notes)+43);
				sprintf(temp,"SITE SCREEN NOTES\n-----------------\n%s\n\n",notes);
				fixoutput(fp,temp,&lines,&pages,line_length,rsrc->module,rsrc->screen);
				Rfree(notes);
				Rfree(temp);
			}
			FINDRSCGETSTRING(hrsrc,"USER SCREEN NOTES",&notes);
			if(notes!=NULL)
			{
				temp=Rmalloc(RDAstrlen(notes)+42);
				sprintf(temp,"USER SCREEN NOTES\n-----------------\n%s\n",notes);
				fixoutput(fp,temp,&lines,&pages,line_length,rsrc->module,rsrc->screen);
				Rfree(notes);
				Rfree(temp);
			}
			RDA_fprintf(fp,"\f");
			RDA_pclose(fp);
		} else {
			ERRORDIALOG("Output Device Failed","The output device provided on the PRINT SECTION window cannot be opened for writing.",NULL,FALSE);
		}
		Rfree(output);
	} else {
		ERRORDIALOG("Output Device Failed","An output device must be provided on the PRINT SECTION window to complete this task.",NULL,FALSE);
	}
	quit_print_section(parent,s);
}
void printsection(RDArsrc *parent,WriteScnHelp *hlp)
{
	RDArsrc *r=NULL;
	PrintSection *s;
	char *defaultprinter=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG printsection Displaying the Print Section Screen in preparation to print help for Screen [%s] [%s].",hlp->rsrcx->module,hlp->rsrcx->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	s=Rmalloc(sizeof(PrintSection));
	s->hrsrc=hlp->helprsrc;
	s->rsrc=hlp->rsrcx;
	r=RDArsrcNEW("OLHELP","PRINT SECTION");
	s->prsrc=r;
	defaultprinter=DefaultPrinter();
	addstdrsrc(r,"OUTPUT DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(r,"PRINT",TRUE,printfunction,s);
	addrfkwrsrc(r,"QUIT",TRUE,quit_print_section,s);
	addbtnrsrc(r,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(r,"PRINT RESOURCES",TRUE,printrsrcs,parent);
	if(xmakescrn(r,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,NULL,NULL,NULL,NULL,parent,__LINE__,__FILE__)) 
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom help PRINT SECTION. Check to see the help is available. If it is, call your installer.",NULL,FALSE);
		quit_print_section(r,s);
	}
}
static void cybersupports(RDArsrc *parent,WriteScnHelp *hlp)
{
	APPlib *args=NULL,*envpx=NULL;
	char temp1[512],*cyber_product=NULL,*cyber_process=NULL;

	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"CYBERSUPPORT");
	envpx=APPlibNEW();
	cyber_product=RDA_GetEnv("CYBER_PRODUCT");
	if(!isEMPTY(cyber_product) && (!RDAstrcmp(hlp->rsrcx->module,"UTILITIES") && !RDAstrcmp(hlp->rsrcx->screen,"TEXT BROWSER")) || (!RDAstrcmp(hlp->rsrcx->module,"RPTGEN") && !RDAstrcmp(hlp->rsrcx->screen,"RUN REPORT CHAIN WINDOW")))
	{
		sprintf(temp1,"ADD_PRODUCT=%s",cyber_product);
	} else {
		sprintf(temp1,"ADD_PRODUCT=%s",((hlp->rsrcx->module!=NULL ? hlp->rsrcx->module:"")));
	}
	addAPPlib(envpx,temp1);
	memset(temp1,0,512);
	cyber_process=RDA_GetEnv("CYBER_PROCESS");
	if(!isEMPTY(cyber_product) && (!RDAstrcmp(hlp->rsrcx->module,"UTILITIES") && !RDAstrcmp(hlp->rsrcx->screen,"TEXT BROWSER")) || (!RDAstrcmp(hlp->rsrcx->module,"RPTGEN") && !RDAstrcmp(hlp->rsrcx->screen,"RUN REPORT CHAIN WINDOW")))
	{
		sprintf(temp1,"ADD_PROCESS=%s",cyber_process);
	} else {
		sprintf(temp1,"ADD_PROCESS=%s",(hlp->rsrcx->screen!=NULL ? hlp->rsrcx->screen:""));
	}
	addAPPlib(envpx,temp1);
	ADVExecuteProgram("doadd",args,envpx);
	if(args!=NULL) freeapplib(args);
	if(envpx!=NULL) freeapplib(envpx);
	quit_screen_help(parent,hlp);
}
static void SimpleChat(RDArsrc *parent,WriteScnHelp *hlp)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"-s");
	ADVExecuteProgram("rdaout",args,NULL);
	if(args!=NULL) freeapplib(args);
	quit_screen_help(parent,hlp);
}
static void ksnapshot(RDArsrc *parent,WriteScnHelp *hlp)
{
	ADVExecuteProgramB("ksnapshot",NULL,NULL,TRUE);
	quit_screen_help(parent,hlp);
}
void screenhelp(RDArsrc *rsrcx)
{
	RDArsrc *helprsrc=NULL;
	char *helpdesc=NULL,*scnnotes=NULL,*uscnnotes=NULL,temp[512];
	char *temp1=NULL;
	WriteScnHelp *WS=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG screenhelp Displaying Screen Help for Module [%s] Screen [%s].",rsrcx->module,rsrcx->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	helprsrc=RDArsrcNEW("OLHELP","SCREEN HELP");
	helpdesc=getscreendesc(rsrcx);
	memset(temp,0,512);
	sprintf(temp,"Supported Users Login to our website will be FirstName.LastName and your password is your Web Password used when submitting Cybersupport Helpdesk Tickets.");
	temp1=Rmalloc(RDAstrlen(helpdesc)+RDAstrlen(temp)+5);
	sprintf(temp1,"%s\n\n%s",temp,(helpdesc!=NULL ? helpdesc:""));
	if(helpdesc!=NULL) Rfree(helpdesc);
	helpdesc=stralloc(temp1);
	if(temp1!=NULL) Rfree(temp1);
	addstdrsrc(helprsrc,"SCREEN DESCRIPTION",SCROLLEDTEXT,0,helpdesc,
		FALSE);
	if(helpdesc!=NULL) Rfree(helpdesc);
	scnnotes=getscreennotes(rsrcx->module,rsrcx->screen);
	addstdrsrc(helprsrc,"SCREEN NOTES",SCROLLEDTEXT,0,
		scnnotes,WRITE_SCNNOTES);
	if(scnnotes!=NULL) Rfree(scnnotes);
	uscnnotes=getusrscreennotes(rsrcx->module,rsrcx->screen);
	addstdrsrc(helprsrc,"USER SCREEN NOTES",SCROLLEDTEXT,0,
		uscnnotes,WRITE_USCNNOTES);
	if(uscnnotes!=NULL) Rfree(uscnnotes);
	WS=Rmalloc(sizeof(WriteScnHelp));
	WS->rsrcx=rsrcx;
	WS->helprsrc=helprsrc;	
	if(WRITE_SCNNOTES)
	{
		addbtnrsrc(helprsrc,"SAVE SCREEN NOTES",TRUE,savescnnotes,WS);
	}
	if(WRITE_USCNNOTES)
	{
		addbtnrsrc(helprsrc,"SAVE USER SCREEN NOTES",TRUE,saveuscnnotes,WS);
	}
	addrfkwrsrc(helprsrc,"QUIT",TRUE,quit_screen_help,WS);
	addbtnrsrc(helprsrc,"PRINT SECTION",TRUE,printsection,WS);
	addrfkwrsrc(helprsrc,"CYBERSUPPORT",TRUE,cybersupports,WS);
	addrfkwrsrc(helprsrc,"WIKI SUPPORT",TRUE,wikisupport,rsrcx);
#ifndef WIN32
	addrfkwrsrc(helprsrc,"KSNAPSHOT",TRUE,ksnapshot,WS);
	addrfkwrsrc(helprsrc,"SIMPLECHAT",TRUE,SimpleChat,WS);
	addbtnrsrc(helprsrc,"SENDMYVNC",TRUE,sendmyvnc,NULL);
#endif 
	addbtnrsrc(helprsrc,"SYSTEM PRINTER MANAGEMENT",TRUE,systemprintmgt,NULL);
	addbtnrsrc(helprsrc,"ANNOUNCEMENTS",TRUE,rda_announcements,NULL);
	addbtnrsrc(helprsrc,"NEWSLETTERS",TRUE,rda_newsletters,NULL);
	addbtnrsrc(helprsrc,"FEEDBACK",TRUE,rda_feedback,NULL);
	if(XPERT_SETUP->software_type<2) 
	{
	addbtnrsrc(helprsrc,"VIDEOS",TRUE,rda_videos,NULL);
	}
	addbtnrsrc(helprsrc,"USER PROFILE",TRUE,RDA_User_Profile,NULL);
	addbtnrsrc(helprsrc,"WWW.RDASYS.COM",TRUE,rda_website,NULL);
	addbtnrsrc(helprsrc,"ON-LINE DOCUMENTS",TRUE,rda_docs,NULL);
	addbtnrsrc(helprsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(helprsrc,"PRINT RESOURCES",TRUE,printrsrcs,rsrcx);
	if(xmakescrn(helprsrc,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,
		NULL,NULL,NULL,NULL,rsrcx,__LINE__,__FILE__)) 
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom help SCREEN HELP. Check to see the help is available. If it is, call your installer.",NULL,FALSE);
		prterr("Error Custom Screen [SCREEN HELP] not found.");
		quit_screen_help(helprsrc,WS);
	}
}
short xwritehelpbin(char *libname,RDAhelp *help,int line,char *file)
{
	short x;
	BIN *bin;
	RDArhlp *rsch;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG Writing help binary [%s] into library [%s] at line [%d] file [%s].",(help->name!=NULL ? help->name:""),libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1703);	/*type identifier*/
	if(help->name!=NULL) 
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,help->name);
	} else BINaddshort(bin,FALSE);
	if(help->desc!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,help->desc);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,help->numrschs);
	for(x=0,rsch=help->rhlp;x<help->numrschs;++x,++rsch)
	{
		if(rsch->name!=NULL) 
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,rsch->name);
		} else BINaddshort(bin,FALSE);
		if(rsch->desc!=NULL)
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,rsch->desc);
		} else BINaddshort(bin,FALSE);
		if(rsch->label!=NULL)
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,rsch->label);
		} else BINaddshort(bin,FALSE);
	}
	if(writelibbin(libname,bin,help->name))
	{
		prterr("Error Can't write help binary [%s] into library [%s].",
			(help->name!=NULL ? help->name:""),libname);
	}
	return(0);
}
RDAhelp *xfindhelpbin(char *module,char *name,int line,char *file)
{
	RDAhelp *help=NULL;
	char *libname=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field) { prterr("DIAG Looking for help [%s] [%s] using findhelpbin at line [%d] program [%s].",module,name,line,file); }
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+RDAstrlen(USERLOGIN)+7);
#ifndef WIN32
	sprintf(libname,"%s/%s/%s.HLP",CURRENTDIRECTORY,USERLOGIN,module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s\\%s.HLP",CURRENTDIRECTORY,USERLOGIN,module);
#endif
	help=RDAhelpNEW();
	if(!gethelpbin(libname,name,help,FALSE)) 
	{
		if(libname!=NULL) Rfree(libname);
		return(help);
	}
	libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libname,"%s/site/%s.HLP",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\site\\%s.HLP",CURRENTDIRECTORY,module);
#endif
	if(!gethelpbin(libname,name,help,FALSE)) 
	{
		if(libname!=NULL) Rfree(libname);
		return(help);
	}
	libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.HLP",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.HLP",CURRENTDIRECTORY,module);
#endif
	if(!gethelpbin(libname,name,help,TRUE)) 
	{
		if(libname!=NULL) Rfree(libname);
		return(help);
	}
	if(libname!=NULL) Rfree(libname);
	if(help!=NULL) free_help(help);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("Error Can't find help binary for screen [%s] [%s].",module,name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(NULL);
}
short xgethelpbin(char *libname,char *name,RDAhelp *help,short showerror,int line,char *file)
{
	short x,version=FALSE,test;
	BIN *bin;
	RDArhlp *rsch;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field)
	{
		prterr("DIAG reading help binary [%s] from library [%s] using gethelpbin at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=xgetlibbin(libname,name,showerror,__LINE__,__FILE__);
	if(bin==NULL) return(-1);
	x=BINgetshort(bin);
	if(x!=(-1703) && x!=(-1701) && x!=(-1702))
	{
		prterr("Error Invalid version or bad binary for (%s) in Library [%s] at line [%d] program [%s].",name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1703)) version=2;
	else if(x==(-1702)) version=TRUE;
	else version=FALSE;
	if(version)
	{
		test=BINgetshort(bin);
		if(test) help->name=BINgetstring(bin);
			else help->name=NULL;
	} else help->name=BINgetstring(bin);
	if(version)
	{
		test=BINgetshort(bin);
		if(test) help->desc=BINgetstring(bin);
			else help->desc=NULL;
	} else help->desc=BINgetstring(bin);
	help->numrschs=BINgetint(bin);
	if(help->numrschs>0)
	{
	help->rhlp=Rmalloc(help->numrschs*sizeof(RDArhlp));
	for(x=0,rsch=help->rhlp;x<help->numrschs;++x,++rsch)
	{
		if(version)
		{
			test=BINgetshort(bin);
			if(test) rsch->name=BINgetstring(bin);
				else rsch->name=NULL;
		} else rsch->name=BINgetstring(bin);
		if(version)
		{
			test=BINgetshort(bin);
			if(test) rsch->desc=BINgetstring(bin);
				else rsch->desc=NULL;
		} else rsch->desc=BINgetstring(bin);
		if(version>1)
		{
			test=BINgetshort(bin);
			if(test) rsch->label=BINgetstring(bin);
				else rsch->label=NULL;
		} else rsch->label=NULL;
	}
	} else help->rhlp=NULL;
	BINfree(bin);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diagolh_field) SEERDAHELP(help);
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(0);
}
void xSEERDAHELP(RDAhelp *h,int line,char *file)
{
	int x;
	RDArhlp *r;

	prterr("DIAG SEERDAHELP Showing Contents of RDAhelp Structure at line [%d] program [%s].",line,file);
	prterr("SCREEN [%s] Number Resources [%d]\n",(h->name!=NULL ? h->name:""),h->numrschs);
	prterr("DESC [%s]\n",(h->desc!=NULL ? h->desc:""));
	if(h->rhlp!=NULL)
	{
		for(x=0,r=h->rhlp;x<h->numrschs;++x,++r)
		{
			prterr("Resource Name [%s]\n",(r->name!=NULL ? r->name:"Null"));
			prterr("Resource Label [%s]\n",(r->label!=NULL ? r->label:"Null"));
			prterr("Resource Desc [%s]\n",(r->desc!=NULL ? r->desc:"Null"));
		}
	}
	prterr("\n");
}
void quitdefaults(RDArsrc *r,Hold2Rsrc *hold)
{
	RDArsrc *rsrc=NULL;
	APPlib *holdAPPlibs;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diaggui || diaggui_field || diagolh_field)
	{
		prterr("DIAG quitdefaults Quiting Save Defaults for Screen [%s] [%s].",hold->rsrc2->module,hold->rsrc2->screen); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(hold!=NULL)
	{
		rsrc=hold->rsrc1;
		if(rsrc!=NULL) free_rsrc(rsrc);
		rsrc=NULL;
		if(hold->v!=NULL) freeapplib(hold->v);
		holdAPPlibs=hold->lib;
		if(holdAPPlibs!=NULL) freeapplib(holdAPPlibs);
		Rfree(hold);
	}
}
void deletedefaults(RDArsrc *r,Hold2Rsrc *hold)
{
	RDAdefault *d=NULL;
	char *library=NULL;
	char *directory=NULL;
	int selected_resource=0;
	char *module=NULL,*screen=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diaggui || diaggui_field || diagolh_field)
	{
		prterr("DIAG deletedefaults Deleting Defaults from Screen [%s] [%s].",r->module,r->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(hold!=NULL)
	{
		readwidget(r,"LIBRARY");
		FINDRSCGETSTRING(r,"LIBRARY",&directory);
		readwidget(r,"RESOURCE LIST");
		FINDRSCGETINT(r,"RESOURCE LIST",&selected_resource);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagolh || diaggui || diaggui_field || diagolh_field)
		{
			prterr("DIAG deletedefaults Deleting Defaults from Screen [%s] [%s].",hold->rsrc2->module,hold->rsrc2->screen);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(hold->rsrc2!=NULL)
		{
			module=hold->rsrc2->module;
			screen=hold->rsrc2->screen;
			if(!isEMPTY(screen) && !isEMPTY(directory) && !isEMPTY(module))
			{
				library=Rmalloc(RDAstrlen(directory)+RDAstrlen(module)+6);
#ifndef WIN32
				sprintf(library,"%s/%s.DEF",directory,module);
#endif
#ifdef WIN32
				sprintf(library,"%s\\%s.DEF",directory,module);
#endif
				d=RDAdefaultNEW(library,screen);
				if(d!=NULL)
				{
					if(!getdefaultbin(library,screen,d,FALSE))
					{
						deletelibbin(library,screen);
						if(library!=NULL) Rfree(library);
					} else {
						/* no defaults for screen */
					}
					FreeRDAdefault(d);
				} else {
					prterr("Error GetDefaults Failed for Screen [%s] [%s].",r->module,r->screen);
				}
			} else {
				prterr("Error directory cannot be \"\".  Enter an appropriate directory to save defaults in.");
			}
		} else {
			prterr("Error directory, module or screen cannot be \"\".  Enter an appropriate directory to save defaults in.");
		}
		if(directory!=NULL) Rfree(directory);
		quitdefaults(r,hold);
	} else {
		prterr("Error hold cannot be NULL.  Enter an appropriate directory to save defaults in.");
	}
}
void savedefaults(RDArsrc *r,Hold2Rsrc *hold)
{
	RDAdefault *d=NULL;
	char *library=NULL;
	char *directory=NULL;
	int selected_resource=0;
	APPlib *inpfocus=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diaggui || diaggui_field || diagolh_field)
	{
		prterr("DIAG savedefaults Creating Save Defaults Screen to capture data from Screen [%s] [%s].",r->module,r->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(hold!=NULL)
	{
		readwidget(r,"LIBRARY");
		FINDRSCGETSTRING(r,"LIBRARY",&directory);
#ifdef WIN32
		mkdir(directory);
#endif
#ifndef WIN32
		mkdir(directory,0666);
		chmod(directory,(mode_t)MODE_RWXRWX___);
#endif
		readwidget(r,"RESOURCE LIST");
		FINDRSCGETINT(r,"RESOURCE LIST",&selected_resource);
		readallwidgets(hold->rsrc2);
		d=GetDefaults(hold->rsrc2);
		inpfocus=hold->v;
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui) SEEDEFAULTS(d);
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(directory!=NULL)
		{
			if(d!=NULL)
			{
				d->input_focus=stralloc(inpfocus->libs[selected_resource]);
				library=Rmalloc(RDAstrlen(directory)+RDAstrlen(d->module)+6);
#ifndef WIN32
				sprintf(library,"%s/%s.DEF",directory,d->module);
#endif
#ifdef WIN32
				sprintf(library,"%s\\%s.DEF",directory,d->module);
#endif
				writedefaultbin(library,d);
				FreeRDAdefault(d);
				if(library!=NULL) Rfree(library);
			} else {
				prterr("Error GetDefaults Failed for Screen [%s] [%s].",r->module,r->screen);
			}
			if(directory!=NULL) Rfree(directory);
		} else {
			prterr("Error directory cannot be NULL.  Enter an appropriate directory to save defaults in.");
		}
		quitdefaults(r,hold);
	}
}
void getdirlib(RDArsrc *r,Hold2Rsrc *hold)
{
	int s=0;
	char *dirstr=NULL;
	APPlib *holdAPPlib;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG getdirlib Setting LIBRARY resource to LIBRARY LIST's value on Screen [%s] [%s].",r->module,r->screen);

	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(FINDRSCGETSTRING(r,"LIBRARY",&dirstr)) return;
	if(FINDRSCGETINT(r,"LIBRARY LIST",&s)) return;
	holdAPPlib=hold->lib;
	if(RDAstrcmp(dirstr,holdAPPlib->libs[s]))
	{
		if(FINDRSCSETSTRING(r,"LIBRARY",holdAPPlib->libs[s])) return;
		updatersrc(r,"LIBRARY");
	}
}
void setdirliblist(RDArsrc *r,Hold2Rsrc *hold)
{
	int x=0;
	char *directory=NULL,inlist=FALSE;
	APPlib *holdAPPlib;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG setdirliblist Setting LIBRARY LIST resource to LIBRARY's value (and adding this library entry if apporpriate) on Screen [%s] [%s].",r->module,r->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
 	readwidget(r,"LIBRARY");
 	if(FINDRSCGETSTRING(r,"LIBRARY",&directory)) return;
 	inlist=FALSE;
	holdAPPlib=hold->lib;
 	for(x=0;x<holdAPPlib->numlibs;++x)
 	{
 		if(!RDAstrcmp(holdAPPlib->libs[x],directory))
 		{
 			inlist=TRUE;
 			break;
 		}
 	}
 	if(inlist!=TRUE)
 	{
 		addAPPlib(hold->lib,directory);
 		x=holdAPPlib->numlibs-1;
 	}
 	FINDRSCLISTAPPlib(r,"LIBRARY LIST",x,hold->lib);
 	updatersrc(r,"LIBRARY LIST");
}
void SaveDefaults(RDArsrc *saversrc)
{
	Hold2Rsrc *hold=NULL;
	RDArsrc *rsrc=NULL;
	char *defdir=NULL,*temp=NULL,*temp1=NULL;
	APPlib *holdAPPlib=NULL,*inpfocus=NULL;
	int x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh || diaggui || diaggui_field || diagolh_field)
	{
		prterr("DIAG SaveDefaults Creating a Screen used to gather the starting widget and library name for Screen [%s] [%s]'s defaults.",saversrc->module,saversrc->screen);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	readallwidgets(saversrc);
	hold=Rmalloc(sizeof(Hold2Rsrc));
	hold->rsrc2=saversrc;
	rsrc=RDArsrcNEW("GUI","SAVE SCREEN DEFAULTS");
	hold->lib=APPlibNEW();
	if(!isEMPTY(LastGroupDefault))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s/%s",CURRENTDIRECTORY,LastGroupDefault);
		addAPPlib(hold->lib,stemp);
	}
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(hold->lib,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	temp=Department_Supervisor(RDA_GROUP);
	if(!isEMPTY(RDA_GROUP) && !RDAstrcmp(temp,USERLOGIN))
	{
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(RDA_GROUP)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
	addAPPlib(hold->lib,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	}
	if(temp!=NULL) Rfree(temp);
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(hold->lib,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	temp=RDA_GetEnv("DEV_LICENSE");
	if(temp!=NULL)
	{
		temp1=strtok(temp," ");
		if(CheckDevLicense(temp1))
		{
			defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
			sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
			sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
			addAPPlib(hold->lib,defdir);
			if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
		}	
	}
	inpfocus=APPlibNEW();
	if(saversrc->rscs!=NULL)
	{
		for(x=0,member=saversrc->rscs;x<saversrc->numrscs;++x,++member)
		{
			if(IsWidgetDisplayed(member))
			{
				addAPPlib(inpfocus,member->rscrname);
			}
		}
	}
	SORTAPPlib(inpfocus);
	for(x=0;x<inpfocus->numlibs;++x)
	{
		if(!RDAstrcmp(saversrc->input_focus,inpfocus->libs[x])) break;
	}
	if(x>=inpfocus->numlibs) x=0;
	hold->v=inpfocus;
	addlstrsrc(rsrc,"RESOURCE LIST",&x,TRUE,NULL,inpfocus->numlibs,
		&inpfocus->libs,NULL);
	hold->rsrc1=rsrc;
	holdAPPlib=hold->lib;
	addlstrsrc(rsrc,"LIBRARY LIST",0,TRUE,getdirlib,holdAPPlib->numlibs,&holdAPPlib->libs,hold);
	addstdrsrc(rsrc,"LIBRARY",VARIABLETEXT,0,holdAPPlib->libs[0],TRUE);
	FINDRSCSETFUNC(rsrc,"LIBRARY",setdirliblist,hold);
    	FINDRSCLISTAPPlib(rsrc,"LIBRARY LIST",0,hold->lib);
	addrfkwrsrc(rsrc,"SAVE",TRUE,savedefaults,hold);
	addrfkwrsrc(rsrc,"DELETE",TRUE,deletedefaults,hold);
	addrfkwrsrc(rsrc,"QUIT",TRUE,quitdefaults,hold);
	addbtnrsrc(rsrc,"HELP",TRUE,screenhelp,NULL);
	if(xmakescrn(rsrc,TRUE,HELPEXPRESSFUNC,NULL,HELPADDOSFUNC,NULL,NULL,NULL,NULL,saversrc,__LINE__,__FILE__))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen SAVE SCREEN DEFAULTS. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quitdefaults(rsrc,hold);
		prterr("Error Can't make screen SAVE SCREEN DEFAULTS.");
	}
}
