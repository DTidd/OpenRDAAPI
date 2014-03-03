/* Packaging functions included in the tool library */
#include <packlib.hpp>
#include <mkmtn.hpp>

char *EscXHTMLLabel(char *s)
{
	char *n=NULL,*temp=NULL;
	int l=0,x=0,len=0;

	if(isEMPTY(s)) return(NULL);
	len=RDAstrlen(s)*2+1;
	n=Rmalloc(len);
	memset(n,0,len);
	l=0;
	n[l++]='"';
	for(x=0,temp=s;*temp && x<len;++temp,++x)
	{
		if(*temp=='\n')
		{
			sprintf(&n[l],"\\n");
			l+=2;
		} else if(*temp=='"')
		{
			sprintf(&n[l],"\\\"");
			l+=2;
		} else {
			n[l++]=*temp;
		}
	}
	n[l++]='"';
	return(n);
}
char *fixname(char *name)
{
	int str_length=0,x;
	char *new_name=NULL;

	if(name!=NULL)
	{
		str_length=RDAstrlen(name);
		new_name=Rmalloc(str_length+1);
		memset(new_name,0,str_length);
		for(x=0;x<str_length;++x)
		{
			if(name[x]=='-')
			{
				new_name[x]='_';
			} else if(name[x]==' ')
			{
				new_name[x]='_';
			} else if(name[x]=='.')
			{
				new_name[x]='_';
			} else if(name[x]=='/')
			{
				new_name[x]='_';
			} else {
				new_name[x]=name[x];
			}
		}
	} else {
		new_name=stralloc("makefunc");
	}
	return(new_name);
}
void makeDefaultMaintainMenu(MaintainMaster *MTN)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(MTN==NULL) return;
	if(isEMPTY(MTN->module)) return;
	sprintf(temp1,"%s %s",MTN->module,MTN->name);
	sprintf(temp2,"mtnmst.EXT %s \"%s\"",MTN->module,MTN->name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(MTN->module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,MTN->module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,MTN->module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
void makeDefaultAddMenu(PowerAdd *POWERADD)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(POWERADD==NULL) return;
	if(isEMPTY(POWERADD->module)) return;
	sprintf(temp1,"%s %s",POWERADD->module,POWERADD->name);
	sprintf(temp2,"doadd.EXT %s \"%s\"",POWERADD->module,POWERADD->name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(POWERADD->module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,POWERADD->module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,POWERADD->module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
void makeDefaultReportMenu(char *module,RDAreport *RPT)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(RPT==NULL) return;
	if(isEMPTY(module)) return;
	sprintf(temp1,"%s %s",module,RPT->name);
	sprintf(temp2,"doreport.EXT %s \"%s\"",module,RPT->name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
void makeDefaultChangeMenu(char *module,CHGstruct *CHG)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(CHG==NULL) return;
	if(isEMPTY(module)) return;
	sprintf(temp1,"%s %s",module,CHG->name);
	sprintf(temp2,"dochg.EXT %s \"%s\"",module,CHG->name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
void makeDefaultIMPEXPMenu(char *module,IEfile *IMPEXP)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(IMPEXP==NULL) return;
	if(isEMPTY(module)) return;
	sprintf(temp1,"%s %s",module,IMPEXP->name);
	sprintf(temp2,"doie.EXT %s \"%s\"",module,IMPEXP->name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
void pack_virfld(FILE *fp,char *dirname,char *libname,char *virfldname)
{
	FILE *v_fp; /* virtual fields list file */
	DFvirtual *virfld=NULL;

	virfld=okvirfld(libname,virfldname);
	if(virfld!=NULL)
	{
		makevirfldcode(virfld,fp,libname,virfldname);
		v_fp=fopen("virfields.list","a");
		fprintf(v_fp,"%s\n", virfldname);
		fflush(v_fp);
		fclose(v_fp);
	}
}
void pack_menu(FILE *fp,char *dirname,char *libname,char *menuname)
{
	RDAmenu *menu=NULL;

	menu=okmenu(dirname,libname,menuname);
	if(menu!=NULL)
	{
		makemenucode(menu,fp,dirname,libname,menuname);
	}
}
void pack_any_screen(FILE *fp,char *dirname,char *libname,char *scrnname,
 		     char packoptscrns,char packlists,char packvirflds,char addscreen,char reportscreen)
{
	FILE *s_fp; /* packed screens list file */
	FILE *vc_fp; /* virtual fields code file */
	RDAscrn *scrn=NULL;
	RDAwdgt *wdgt=NULL;
	RDALoadValue *loadtemp=NULL;
	RDAhelp *help=NULL;
	char *temp1=NULL;
	int a=0;
	int x=0;

	s_fp=fopen("packedscreens.list","a");
#ifndef WIN32
	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(40+RDAstrlen(scrnname)+1);
	sprintf(temp1,"grep -q \"%s\" packedscreens.list", scrnname);
	if(system_command_retvalue(temp1)==0)
	{
		fclose(s_fp);
		return;
	}
	if(temp1!=NULL) Rfree(temp1);
#endif /* !WIN32 */
	/* This may be changed later, but I want site to override rda, otherwise go with rda */
	/* Also, this prints some errors if not found in site; they may be annoying, but it works */
	scrn=okscreen("site",libname,scrnname);
	if(scrn==NULL)
	{
		scrn=okscreen("rda",libname,scrnname);
	}
	if(scrn!=NULL)
	{
		makescrncode(scrn,fp,"rda",libname,scrnname);
	}
	else
	{
		fclose(s_fp);
		return;
	}
	fprintf(s_fp,"%s\n",scrnname);
	fclose(s_fp);
	/* Pack screen defaults if they exist */
	pack_def(fp,dirname,libname,scrnname,0);

	/* Pack scrolled lists, virtual fields, and optional screens off the main screen if there are any */
	vc_fp=fopen("virfields.c","a");
	for(x=0,wdgt=scrn->wdgts;x<scrn->numwdgts;++x,++wdgt)
	{
		/* standard resource, possibly a virtual field */
		if(wdgt->type==0 && packvirflds!=0 && !isEMPTY(wdgt->resource_name))
		{
			if(wdgt->resource_name[0]!='[')
			{
				/* do not try to look up add widgets */
				if(!addscreen || (RDAstrstr(wdgt->resource_name,"ADD ")!=wdgt->resource_name))
				{
#ifndef WIN32
					if(temp1!=NULL) Rfree(temp1);
					temp1=Rmalloc(40+RDAstrlen(wdgt->resource_name)+1);
					sprintf(temp1,"grep -q \"%s\" virfields.list", wdgt->resource_name);
					if(system_command_retvalue(temp1)!=0)
					{
#endif /* !WIN32 */
						pack_virfld(vc_fp,dirname,libname,wdgt->resource_name);
#ifndef WIN32
					}
					if(temp1!=NULL) Rfree(temp1);
#endif /* !WIN32 */
				}
			}
		}
		/* button, possibly a special load value */
		else if(wdgt->type==6 && !isEMPTY(wdgt->resource_name))
		{
			/* get the distance between the substring and the start of the string, if the substring exists */
			if(RDAstrstr(wdgt->resource_name,"LOAD VALUE")!=NULL)
			{
				a = RDAstrstr(wdgt->resource_name,"LOAD VALUE")-wdgt->resource_name;
				if(a!=0)
				{
					temp1=stralloc(wdgt->resource_name);
					temp1[a-1]=0; /* set to null, chopping off the rest of the string */
					loadtemp=okload(libname,temp1);
					if(loadtemp!=NULL)
					{
						makeloadcode(loadtemp,fp,libname,temp1);
					}
				}
			}
		}
		/* scrolled list */
		else if(wdgt->type==7 && packlists!=0 && !isEMPTY(wdgt->resource_name))
		{
			if(!addscreen || RDAstrcmp(wdgt->resource_name,"PREVIOUS ADD'S"))
			{
				pack_list(fp,libname,wdgt->resource_name);
			}
		}
		/* optional screen */
		else if(wdgt->type==14 && packoptscrns!=0 && !isEMPTY(wdgt->resource_name))
		{
			if(reportscreen)
			{
				pack_report_screen(fp,"rda",libname,wdgt->resource_name,1,1,0);
			}
			else
			{
				pack_screen(fp,"rda",libname,wdgt->resource_name,1,1,1);
			}
		}
	}
	fclose(vc_fp);
	/* Pack help for the screen if it exists */
	help=okhelp(dirname,libname,scrnname);
	if(help!=NULL)
	{
		makehelpcode(help,fp,dirname,libname,scrnname);
		free_help(help);
	}
}
void pack_screen(FILE *fp,char *dirname,char *libname,char *scrnname,char packoptscrns,char packlists,char packvirflds)
{
	pack_any_screen(fp,dirname,libname,scrnname,packoptscrns,packlists,packvirflds,0,0);
}
void pack_add_screen(FILE *fp,char *dirname,char *libname,char *scrnname,char packoptscrns,char packlists,char packvirflds)
{
	pack_any_screen(fp,dirname,libname,scrnname,packoptscrns,packlists,packvirflds,1,0);
}
void pack_report_screen(FILE *fp,char *dirname,char *libname,char *scrnname,char packoptscrns,char packlists,char packvirflds)
{
	pack_any_screen(fp,dirname,libname,scrnname,packoptscrns,packlists,packvirflds,0,1);
}
void pack_def(FILE *fp,char *dirname,char *libname,char *defname,char recursive)
{
	FILE *s_fp=NULL;
	RDAdefault *def=NULL;
	RDAscrn *scrn=NULL;
	RDAwdgt *wdgt=NULL;
	char *temp1=NULL;
	int x=0;

	s_fp=fopen("scndefs.list","a");
#ifndef WIN32
	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(40+RDAstrlen(defname)+1);
	sprintf(temp1,"grep -q \"%s\" scndefs.list", defname);
	if(system_command_retvalue(temp1)==0)
	{
		fclose(s_fp);
		return;
	}
	if(temp1!=NULL) Rfree(temp1);
#endif /* !WIN32 */
	/* This may be changed later, but I want site to override rda, otherwise go with rda */
	def=okdef_noerrors("site",libname,defname);
	if(def==NULL)
	{
		def=okdef_noerrors("rda",libname,defname);
	}
	if(def!=NULL)
	{
		makedefcode(def,fp,"rda",libname,defname);
	}

	/* This may be changed later, but I want site to override rda, otherwise go with rda */
	/* Also, this prints some errors if not found in site; they may be annoying, but it works */
	scrn=okscreen("site",libname,defname);
	if(scrn==NULL)
	{
		scrn=okscreen("rda",libname,defname);
	}
	if(scrn==NULL)
	{
		fclose(s_fp);
		return;
	}

	if(recursive)
	{
		/* Pack defaults for optional screens off the main screen if there are any */
		for(x=0,wdgt=scrn->wdgts;x<scrn->numwdgts;++x,++wdgt)
		{
			/* optional screen */
			if(wdgt->type==14 && !isEMPTY(wdgt->resource_name))
			{
				pack_def(fp,"rda",libname,wdgt->resource_name,1);
			}
		}
	}
	if(def!=NULL)
	{
		fprintf(s_fp,"%s\n",defname);
	}
	fclose(s_fp);
}
void pack_list(FILE *fp,char *libname,char *listname)
{
	FILE *s_fp;
	RDAScrolledList *listtemp=NULL;
	char *temp1=NULL;

	s_fp=fopen("packedlists.list","a");
#ifndef WIN32
	if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(40+RDAstrlen(listname)+1);
	sprintf(temp1,"grep -q \"%s\" packedlists.list", listname);
	if(system_command_retvalue(temp1)==0)
	{
		fclose(s_fp);
		return;
	}
	if(temp1!=NULL) Rfree(temp1);
#endif /* !WIN32 */
	listtemp=oklist(libname,listname);
	if(listtemp!=NULL)
	{
		makelistcode(listtemp,fp,libname,listname);
	}
	else
	{
		fclose(s_fp);
		return;
	}
	fprintf(s_fp,"%s\n",listname);
	fclose(s_fp);
}
#ifndef WIN32
short system_command_retvalue(char *command)
{
	int list=0;

	if(!isEMPTY(command))
	{
		list=system(command);
		return list;
	} else return(-1);
}
#endif /* WIN32 */
/* From mkscncd */
void makescrncode(RDAscrn *scrn,FILE *fp,char *dirname,char *libname,char *scrnname)
{
	int x=0,q=0;
	char *temp=NULL,*tempx=NULL;
	RDAwdgt *wdgt=NULL;
	char *dirx=NULL,*libx=NULL;
	APPlib *tempapplib=NULL;
	int overall_length=0;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(dirname)+5);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
		sprintf(libx,"%s/%s.SCN",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
		temp=fixname(scrn->name);
#ifdef PAIN_IN_
		fprintf(fp,"/* %s's - Make (%s) Screen */\n",scrn->module,scrn->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"\n");
		fprintf(fp,"char *module=\"%s\";\n",scrn->module);
		fprintf(fp,"\n");
#endif /* PAIN_IN_ */
		fprintf(fp,"static void SCN_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDAscrn *scrn=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
#ifndef OLDWAY
		fprintf(fp,"\tchar *temp1=NULL,*temp2=NULL;\n");
		fprintf(fp,"\tchar *temp3=NULL,*temp4=NULL;\n");
#endif /* OLDWAY */
#ifdef OLDWAY
		fprintf(fp,"\tchar *temp1=NULL;\n");
#endif
		fprintf(fp,"\n");


#ifdef RADDOG

		fprintf(fp,"/*============================================================================\n");
		fprintf(fp,"Adding Screen Widgets\n");
		fprintf(fp,"void xaddwdgt(RDAscrn *screen,short type,char *resource_name,char *label,\n");
		fprintf(fp,"\tchar *pixmap,char *XHTML_Label,short rows,short cols,int rtype,\n");
		fprintf(fp,"\tchar *expression,char *editable_expression,char *sensitive_expression,\n");
		fprintf(fp,"\tchar *transversal_expression,int line,char *file)\n");
		fprintf(fp,"\n");
		fprintf(fp,"#define ADVaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l)   xaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l,__LINE__,__FILE__)\n");
		fprintf(fp,"\n");
		fprintf(fp,"RDAwdgts types\n");
		fprintf(fp,"{\n");
		fprintf(fp,"\t0\t-\tStandard Resource\n");
		fprintf(fp,"\t1\t-\tNew Line\n");
		fprintf(fp,"\t2\t-\tEnd Line\n");
		fprintf(fp,"\t3\t-\tNew Box\n");
		fprintf(fp,"\t4\t-\tEnd Box\n");
		fprintf(fp,"\t5\t-\tLabel\n");
		fprintf(fp,"\t6\t-\tButton\n");
		fprintf(fp,"\t7\t-\tScrolled List\n");
		fprintf(fp,"\t8\t-\tScrolled Text\n");
		fprintf(fp,"\t9\t-\tToggle Button (Boolean)\n");
		fprintf(fp,"\t10\t-\tFrame\n");
		fprintf(fp,"\t11\t-\tSeperator\n");
		fprintf(fp,"\t12\t-\tNew Scrolled Window\n");
		fprintf(fp,"\t13\t-\tEnd Scrolled Window\n");
		fprintf(fp,"\t14\t-\tOptional Screen\n");
		fprintf(fp,"\t15\t-\tCustom Type (Same as Resource Type, except for rtype.)\n");
		fprintf(fp,"\t16\t-\tExpenditure Type (Same as Resource Type, except for rtype.)\n");
		fprintf(fp,"\t17\t-\tRevenue Type (Same as Resource Type, except for rtype.)\n");
		fprintf(fp,"\t18\t-\tBalance Sheet Type (Same as Resource Type, except for rtype.)\n");
		fprintf(fp,"\t19\t-\tBeginning Balance Type (Same as Resource Type, except for rtype.)\n");
		fprintf(fp,"\n");
		fprintf(fp,"New Widgets\n");
		fprintf(fp,"\t??\t-\tNew Menu Bar (Start Menu Bar)\n");
		fprintf(fp,"\t??\t-\tEnd Menu Bar (End Menu Bar)\n");
		fprintf(fp,"\t??\t-\tMenu Control (Menu Control Option)\n");
		fprintf(fp,"\t??\t-\tNeeds a Modifying Character????????\n");
		fprintf(fp,"\t??\t-\tMenu Button (Buttons available to the menu)\n");
		fprintf(fp,"\t??\t-\tNeeds a Modifying Character????????\n");
		fprintf(fp,"}\n");
		fprintf(fp,"Resource Types\n");
		fprintf(fp,"\t5-Label\t0\t-\tText Label\n");
		fprintf(fp,"\t\t1\t-\tPixmap Label\n");
		fprintf(fp,"\n");
		fprintf(fp,"\t6-Button\t0\t-\tPush Button\n");
		fprintf(fp,"\t\t1\t-\tArrow Button Right\n");
		fprintf(fp,"\t\t2\t-\tArrow Button Left\n");
		fprintf(fp,"\t\t3\t-\tArrow Button Up\n");
		fprintf(fp,"\t\t4\t-\tArrow Button Down\n");
		fprintf(fp,"\t\t5\t-\tPixmap\n");
		fprintf(fp,"\n");
		fprintf(fp,"\t8-Scrolled Text\t0\t-\tWordWap & Horizontal SB (Side Bar)\n");
		fprintf(fp,"\t\t1\t-\tHorizontal SB & No WordWrap\n");
		fprintf(fp,"\n");
		fprintf(fp,"\t9-Toggle Button\t0\t-\tText Label\n");
		fprintf(fp,"\t\t1\t-\tPixmap Label\n");
		fprintf(fp,"\n");
		fprintf(fp,"\t10-Frame\t0\t-\tShadow In\n");
		fprintf(fp,"\t\t1\t-\tShadow Out\n");
		fprintf(fp,"\t\t2\t-\tShadow Etched In\n");
		fprintf(fp,"\t\t3\t-\tShadow Etched Out\n");
		fprintf(fp,"\n");
		fprintf(fp,"\t11-Seperator\t0\t-\tHorizontal Single Line\n");
		fprintf(fp,"\t\t1\t-\tHorizontal Double Line\n");
		fprintf(fp,"\t\t2\t-\tHorizontal Single Dashed\n");
		fprintf(fp,"\t\t3\t-\tHorizontal Double Dashed\n");
		fprintf(fp,"\t\t4\t-\tVertical Single Line\n");
		fprintf(fp,"\t\t5\t-\tVertical Double Line\n");
		fprintf(fp,"\t\t6\t-\tVertical Single Dashed\n");
		fprintf(fp,"\t\t7\t-\tVertical Double Dashed\n");
		fprintf(fp,"\n");
		fprintf(fp,"\t\t14-OptionalScrn\t0\t-\tPush Button\n");
		fprintf(fp,"\t\t1\t-\tArrow Button Right\n");
		fprintf(fp,"\t\t2\t-\tArrow Button Left\n");
		fprintf(fp,"\t\t3\t-\tArrow Button Up\n");
		fprintf(fp,"\t\t4\t-\tArrow Button Down\n");
		fprintf(fp,"\t\t5\t-\tPixmap\n");
		fprintf(fp,"\n");
		fprintf(fp,"============================================================================*/\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
#endif /* RADDOG */


		fprintf(fp,"\tscrn=RDAscrnNEW(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"\tif(scrn!=NULL)\n");
		fprintf(fp,"\t{\n");
		for(x=0,wdgt=scrn->wdgts;x<scrn->numwdgts;++x,++wdgt)
		{	
			if(!isEMPTY(wdgt->expression) || 
				!isEMPTY(wdgt->editable_expression) 
				|| !isEMPTY(wdgt->sensitive_expression) 
				|| !isEMPTY(wdgt->transversal_expression))
			{
				overall_length=0;
				tempapplib=break_expression2(wdgt->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				overall_length=0;
				tempapplib=break_expression2(wdgt->editable_expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp2=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp2,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				overall_length=0;
				tempapplib=break_expression2(wdgt->sensitive_expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp3=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp3,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				overall_length=0;
				tempapplib=break_expression2(wdgt->transversal_expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp4=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp4,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				tempx=EscXHTMLLabel(wdgt->XHTML_Label);
				fprintf(fp,"\t\tADVaddwdgt(scrn,%d,\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d,temp1,temp2,temp3,temp4);\n",
					wdgt->type,
					(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
					(wdgt->label!=NULL ? wdgt->label:""),
					(wdgt->pixmap!=NULL ? wdgt->pixmap:""),
					(tempx!=NULL ? tempx:""),
					wdgt->rows,
					wdgt->cols,
					wdgt->rtype);
				if(tempx!=NULL) Rfree(tempx);
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\t\tif(temp2!=NULL) Rfree(temp2);\n");
				fprintf(fp,"\t\tif(temp3!=NULL) Rfree(temp3);\n");
				fprintf(fp,"\t\tif(temp4!=NULL) Rfree(temp4);\n");
			} else {
				tempx=EscXHTMLLabel(wdgt->XHTML_Label);
				fprintf(fp,"\t\tADVaddwdgt(scrn,%d,\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d,NULL,NULL,NULL,NULL);\n",
					wdgt->type,
					(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
					(wdgt->label!=NULL ? wdgt->label:""),
					(wdgt->pixmap!=NULL ? wdgt->pixmap:""),
					(tempx!=NULL ? tempx:""),
					wdgt->rows,
					wdgt->cols,
					wdgt->rtype);
				if(tempx!=NULL) Rfree(tempx);
			}
		}
		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.SCN\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.SCN\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writescrnbin(defdir,scrn))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+89+1);\n",RDAstrlen(scrn->name),RDAstrlen(scrn->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"SCREEN WRITE ERROR: Module [%s] Screen [%s], Can Not Save Maintain Master!\");\n",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(scrn!=NULL) free_scrn(scrn);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");
		fprintf(fp,"\tif(temp3!=NULL) Rfree(temp3);\n");
		fprintf(fp,"\tif(temp4!=NULL) Rfree(temp4);\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}

}
RDAscrn *okscreen(char *dirname,char *libname,char *scrnname)
{
	RDAscrn *scrn=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(scrn!=NULL) free_scrn(scrn); 
		scrn=RDAscrnNEW(libname,scrnname);
		dirx=Rmalloc(RDAstrlen(dirname)+2);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.SCN",dirx,libname);
		if(dirx!=NULL) Rfree(dirx);
		if(getscrnbin(libx,scrn,FALSE))
		{
			prterr("ERROR: Cannot Read Screen Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,scrnname);
			if(scrn!=NULL) free_scrn(scrn);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(scrn);
		}
	} else { 
		return(NULL);
	}
}
/* End from mkscncd */
/* From mkhelpcd */
void makehelpcode(RDAhelp *help,FILE *fp,char *dirname,char *libname,char *scrnname)
{
	int x=0,q=0;
	RDArhlp *h;
	char *temp=NULL;
	char *dirx=NULL,*libx=NULL;
	APPlib *tempapplib=NULL;
	int overall_length=0;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(dirname)+5);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
		sprintf(libx,"%s/%s.SCN",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
		temp=fixname(help->name);
#ifdef PAIN_IN_
		fprintf(fp,"/* %s's - Make (%s) Screen */\n",libname,help->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"\n");
		fprintf(fp,"char *module=\"%s\";\n",libname);
		fprintf(fp,"\n");
#endif /* PAIN_IN_ */
		fprintf(fp,"static void HELP_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDAhelp *help=NULL;\n");
		fprintf(fp,"\tRDArhlp *h=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
#ifndef OLDWAY
		fprintf(fp,"\tchar *temp1=NULL,*temp2=NULL;\n");
		fprintf(fp,"\tchar *temp3=NULL,*temp4=NULL;\n");
#endif /* OLDWAY */
#ifdef OLDWAY
		fprintf(fp,"\tchar *temp1=NULL;\n");
#endif
		fprintf(fp,"\n");


		fprintf(fp,"\thelp=RDAhelpNEW();\n");
		fprintf(fp,"\tif(help!=NULL)\n");
		fprintf(fp,"\t{\n");
		if(help->name!=NULL)
		{
			fprintf(fp,"\t\thelp->name=stralloc(\"%s\");\n",help->name);
		}
		else
		{
			fprintf(fp,"\t\thelp->name=NULL;\n");
		}
		if(help->desc!=NULL)
		{
			overall_length=0;
			tempapplib=break_expression2(help->desc,&overall_length);
			if(tempapplib!=NULL)
			{
				fprintf(fp,"\t\thelp->desc=Rmalloc(%d+1);\n",overall_length);
				fprintf(fp,"\t\tsprintf(help->desc,\"");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					fprintf(fp,"%%s");
				}
				fprintf(fp,"\",\n");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					if(q!=(tempapplib->numlibs-1))
					{
						fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
					} else {
						fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
					}
				}
				freeapplib(tempapplib);
			}
		}
		else
		{
			fprintf(fp,"\t\thelp->desc=NULL;\n");
		}
		fprintf(fp,"\t\thelp->numrschs=%d;\n",help->numrschs);
		if(help->rhlp!=NULL)
		{
			fprintf(fp,"\t\thelp->rhlp=Rmalloc(%d*sizeof(RDArhlp));\n",help->numrschs);
			fprintf(fp,"\t\th=help->rhlp;\n");
			for(x=0,h=help->rhlp;x<help->numrschs;++x,++h)
			{
				if(!isEMPTY(h->name))
				{
					fprintf(fp,"\t\th->name=stralloc(\"%s\");\n",h->name);
				}
				else
				{
					fprintf(fp,"\t\th->name=NULL;\n");
				}
				if(!isEMPTY(h->desc))
				{
					overall_length=0;
					tempapplib=break_expression2(h->desc,&overall_length);
					if(tempapplib!=NULL)
					{
						fprintf(fp,"\t\th->desc=Rmalloc(%d+1);\n",overall_length);
						fprintf(fp,"\t\tsprintf(h->desc,\"");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							fprintf(fp,"%%s");
						}
						fprintf(fp,"\",\n");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							if(q!=(tempapplib->numlibs-1))
							{
								fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
							} else {
								fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
							}
						}
						freeapplib(tempapplib);
					}
				}
				else
				{
					fprintf(fp,"\t\th->desc=NULL;\n");
				}
				if(!isEMPTY(h->label))
				{
					fprintf(fp,"\t\th->label=stralloc(\"%s\");\n",h->label);
				}
				else
				{
					fprintf(fp,"\t\th->label=NULL;\n");
				}
				fprintf(fp,"\t\t++h;\n");
			}
		}
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.HLP",dirx,libname);
		if(dirx!=NULL) Rfree(dirx);
		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.HLP\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.HLP\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writehelpbin(defdir,help))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+87+1);\n",RDAstrlen(help->name),RDAstrlen(libname));
		fprintf(fp,"\t\t\tsprintf(temp1,\"HELP WRITE ERROR: Module [%s] Screen [%s], Can Not Save Help Definition!\");\n",(libname!=NULL?libname:""),(help->name!=NULL?help->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(help!=NULL) free_help(help);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");
		fprintf(fp,"\tif(temp3!=NULL) Rfree(temp3);\n");
		fprintf(fp,"\tif(temp4!=NULL) Rfree(temp4);\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}

}
RDAhelp *okhelp(char *dirname,char *libname,char *scrnname)
{
	RDAhelp *help=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(help!=NULL) free_help(help); 
		help=RDAhelpNEW();
		dirx=Rmalloc(RDAstrlen(dirname)+2);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.HLP",dirx,libname);
		if(dirx!=NULL) Rfree(dirx);
		if(gethelpbin(libx,scrnname,help,FALSE))
		{
			prterr("ERROR: Cannot Read Help Definition.  Error in permissions or help for the screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,scrnname);
			if(help!=NULL) free_help(help);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(help);
		}
	} else { 
		return(NULL);
	}
}
/* End from mkhelpcd */
/* From mkmtncd */
void makeMaintainMastercode(MaintainMaster *MTNMASTER,FILE *fp,char *dirname,char *libname,char *mtnname)
{
	char *libx=NULL,*dirx=NULL,*funcname=NULL;
	int x=0,y=0,z=0;
/*
	int q=0;
	MTNAction *action=NULL;
	PowerWrite *wrte=NULL;
	RDAsearch *s=NULL;
	Actionstruct *as=NULL;
	RDAconfld *confld=NULL;
	APPlib *tempapplib=NULL;
	ButtonField *bf=NULL;
	int overall_length=0;
	MaintainButton *button=NULL;
	PowerWriteField *wf=NULL;
*/
	RDAsubord *subord=NULL;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(dirname)+5);
#ifndef WIN32
		sprintf(dirx,"%s/",dirname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\",dirname);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
#ifndef WIN32
		sprintf(libx,"%s/%s.MTN",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.MTN",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
		fprintf(fp,"/* %s's - Make (%s) Maintain Master */\n",MTNMASTER->module,MTNMASTER->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mkmtn.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		funcname=fixname(MTNMASTER->name);
		fprintf(fp,"\n");
		fprintf(fp,"static void MTN_%s()\n",funcname);
		if(funcname!=NULL) Rfree(funcname);
		fprintf(fp,"{\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\tint x=0;\n");
		if(MTNMASTER->s!=NULL)
		{
			fprintf(fp,"\tRDAsearch *S=NULL;\n");
			z=TRUE;
		}
		if(MTNMASTER->subord!=NULL)
		{
			fprintf(fp,"\tRDAsubord *SUBORD=NULL;\n");
			z=TRUE;
		}
		if(MTNMASTER->NewWrites!=NULL || MTNMASTER->PrevWrites!=NULL)
		{
			y=TRUE;		
		}
		if(MTNMASTER->subord!=NULL && y==FALSE)
		{
			for(x=0,subord=MTNMASTER->subord;x<MTNMASTER->subnum;++x,++subord)
			{
				if(subord->UsePowerWrite==TRUE)
				{
					y=TRUE;
					break;
				}
			}
		}
		if(y==TRUE)
		{
			fprintf(fp,"\tPowerWrite *WRTE=NULL;\n");
			z=TRUE;
		}
		y=0;
		if(MTNMASTER->Actions!=NULL)
		{
			fprintf(fp,"\tMTNAction *ACTION=NULL;\n");
			z=TRUE;
		}
		if(z==TRUE)
		{
			fprintf(fp,"\tshort r=0;\n");
		}
		z=FALSE;
		fprintf(fp,"\tMaintainMaster *MTNMSTR=NULL;\n");
		fprintf(fp,"\tAPPlib *args=NULL,*args2=NULL;\n");
		if(MTNMASTER->buttons!=NULL)
		{
			fprintf(fp,"\tMaintainButton *button=NULL;\n");
		}
		fprintf(fp,"\tchar *temp1=NULL,*temp2=NULL;\n");
		fprintf(fp,"\tchar *temp3=NULL,*temp4=NULL;\n");
		fprintf(fp,"\tchar *temp5=NULL,*temp6=NULL;\n");
		fprintf(fp,"\n");

		makemtncode(MTNMASTER,fp,mtnname);

		fprintf(fp,"\n");

		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.MTN\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.MTN\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writeMaintainMaster(defdir,MTNMSTR))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+89+1);\n",RDAstrlen(MTNMASTER->name),RDAstrlen(MTNMASTER->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"MAINTAIN MTNMASTER WRITE ERROR: Module [%s] Maintain Master [%s], Can Not Save Maintain Master!\");\n",(MTNMASTER->module!=NULL?MTNMASTER->module:""),(MTNMASTER->name!=NULL?MTNMASTER->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\t\tif(args!=NULL) freeapplib(args);\n");
		fprintf(fp,"\t\t\tif(args2!=NULL) freeapplib(args2);\n");
		fprintf(fp,"\t\t\tif(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);\n");
		fprintf(fp,"\t\t} else {\n");
		fprintf(fp,"\t\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\t\tif(args!=NULL) freeapplib(args);\n");
		fprintf(fp,"\t\t\tif(args2!=NULL) freeapplib(args2);\n");
		fprintf(fp,"\t\t\tif(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);\n");
		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}
}
void makeAllMaintainMastercode(MaintainMaster *MTNMASTER,FILE *fp,char *dirname,
	                       char *libname,char *mtnname,char packmtn)
{
	FILE *s_fp; /* load screens list file */
	FILE *sc_fp; /* load screens code file */
	char *libx=NULL,*dirx=NULL;
	char *temp1=NULL;
	int x=0;
	RDAsearch *s=NULL;

	if(fp!=NULL)
	{
		/* Pack maintain master if packmtn is true */
		if(packmtn)
		{
			makeMaintainMastercode(MTNMASTER,fp,dirname,libname,mtnname);
		}

		/* Pack main screens */
		if(!isEMPTY(MTNMASTER->browse_screen))
		{
			pack_screen(fp,dirname,libname,MTNMASTER->browse_screen,0,0,0);
		}
		if(!isEMPTY(MTNMASTER->define_list))
		{
			pack_screen(fp,dirname,libname,MTNMASTER->define_list,0,0,0);
		}
		if(!isEMPTY(MTNMASTER->search_browse))
		{
			pack_screen(fp,dirname,libname,MTNMASTER->search_browse,1,1,0);
		}
		if(!isEMPTY(MTNMASTER->maintain_screen))
		{
			pack_screen(fp,dirname,libname,MTNMASTER->maintain_screen,1,1,1);
		}
		/* Make default menu and pack it */
		if(packmtn)
		{
			makeDefaultMaintainMenu(MTNMASTER);
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(40+RDAstrlen(MTNMASTER->module)+RDAstrlen(MTNMASTER->name)+1);
			sprintf(temp1,"%s %s",MTNMASTER->module,MTNMASTER->name);
			pack_menu(fp,"rda",libname,temp1);
			if(temp1!=NULL) Rfree(temp1);
		}

		/* Pack other stuff */

		/* Pack load screens for supporting files if necessary */
		s_fp=fopen("loadscreens.list","a");
		sc_fp=fopen("loadscreens.c","a");
		if(MTNMASTER->s!=NULL)
		{
			for(x=0,s=MTNMASTER->s;x<MTNMASTER->num;++x,++s)
			{
				if(!isEMPTY(s->browse_screen)) stoupper(s->browse_screen);
				if(!isEMPTY(s->define_list)) stoupper(s->define_list);
				if(!isEMPTY(s->search_browse)) stoupper(s->search_browse);

				/* Make sure the screen isn't already packed */
				/* The grep stuff is just for my convenience and probably doesn't work on Win32 - JAB */
				if(!isEMPTY(s->browse_screen))
				{
#ifndef WIN32
					if(temp1!=NULL) Rfree(temp1);
					temp1=Rmalloc(40+RDAstrlen(s->browse_screen)+1);
					sprintf(temp1,"grep -q \"%s\" loadscreens.list", s->browse_screen);
					if(system_command_retvalue(temp1)!=0)
					{
#endif /* !WIN32 */
						fprintf(s_fp,"%s\n", s->browse_screen);
						pack_screen(sc_fp,dirname,libname,s->browse_screen,0,0,0);
#ifndef WIN32
					}
					if(temp1!=NULL) Rfree(temp1);
#endif /* !WIN32 */
				}
				if(!isEMPTY(s->define_list))
				{
#ifndef WIN32
					if(temp1!=NULL) Rfree(temp1);
				        temp1=Rmalloc(40+RDAstrlen(s->define_list)+1);
					sprintf(temp1,"grep -q \"%s\" loadscreens.list", s->define_list);
					if(system_command_retvalue(temp1)!=0)
					{
#endif /* !WIN32 */
						fprintf(s_fp,"%s\n", s->define_list);
						pack_screen(sc_fp,dirname,libname,s->define_list,0,0,0);
#ifndef WIN32
					}
					if(temp1!=NULL) Rfree(temp1);
#endif /* !WIN32 */
				}
				if(!isEMPTY(s->search_browse))
				{
#ifndef WIN32
					if(temp1!=NULL) Rfree(temp1);
				        temp1=Rmalloc(40+RDAstrlen(s->search_browse)+1);
					sprintf(temp1,"grep -q \"%s\" loadscreens.list", s->search_browse);
					if(system_command_retvalue(temp1)!=0)
					{
#endif /* !WIN32 */
						fprintf(s_fp,"%s\n", s->search_browse);
						pack_screen(sc_fp,dirname,libname,s->search_browse,1,1,0);
#ifndef WIN32
					}
					if(temp1!=NULL) Rfree(temp1);
#endif /* !WIN32 */
				}
				fflush(s_fp);
			}
		}
		fclose(sc_fp);
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);
		s_fp=fopen("virfields.list","a");
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);
		s_fp=fopen("packedscreens.list","a");
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);
		s_fp=fopen("scndefs.list","a");
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);
		s_fp=fopen("packedlists.list","a");
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	} else {
		ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
		if(dirname!=NULL) dirname=NULL;
	}
}
/* End from mkmtncd */
/* From mkaddcd */
void makeaddcode(PowerAdd *add,FILE *fp,char *dirname,char *libname,char *addname)
{
	char *temp=NULL;
	int x=0,y=0,q=0;
	MaintainButton *button=NULL;
	ButtonField *bf=NULL;
	RDAaddsub *subord=NULL;
	RDAconfld *confld=NULL;
	AddWrite *Write=NULL;
	AddWriteField *field=NULL;
	AddEditWdgt *EditWdgt=NULL;
	RDAeditfld *editfld=NULL;
	char *dirx=NULL,*libx=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;
	char *thing1="/*",*thing2="*/",set_loadvirtual=FALSE;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(dirname)+5);
#ifndef WIN32
		sprintf(dirx,"%s/",dirname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\",dirname);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
#ifndef WIN32
		sprintf(libx,"%s/%s.ADD",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.ADD",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
		fprintf(fp,"/* %s's - Make (%s) Power Add */\n",add->module,add->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mkadd.hpp>\n");
                fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
                fprintf(fp,"extern char *module;\n");
                fprintf(fp,"extern APPlib *errorlist;\n");
		temp=fixname(add->name);
		fprintf(fp,"\n");
		fprintf(fp,"static void ADD_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tint x=0;\n");
		fprintf(fp,"\tPowerAdd *add=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\tAPPlib *args=NULL,*args2=NULL;\n");
		fprintf(fp,"\tRDAaddsub *subord=NULL;\n");
		fprintf(fp,"\tAddWrite *Write=NULL;\n");
		if(add->buttons!=NULL)
		{
			fprintf(fp,"\tMaintainButton *button=NULL;\n");
		} else {
			fprintf(fp,"/*\n");
			fprintf(fp,"\tMaintainButton *button=NULL;\n");
			fprintf(fp,"*/\n");
		}
		if(add->EditWdgts!=NULL)
		{
			fprintf(fp,"\tAddEditWdgt *EditWdgt=NULL;\n");
			for(x=0,EditWdgt=add->EditWdgts;x<add->numwdgts;++x,++EditWdgt)
			{
				if(EditWdgt->loadvirtual==TRUE) set_loadvirtual=TRUE;
			}
		} else {
			fprintf(fp,"/*\n");
			fprintf(fp,"\tAddEditWdgt *EditWdgt=NULL;\n");
			fprintf(fp,"*/\n");
		}
		if(set_loadvirtual==1)
		{
			fprintf(fp,"\tDFvirtual *d=NULL;\n");
		}
		fprintf(fp,"\tchar *temp1=NULL,*temp2=NULL;\n");
		fprintf(fp,"\tchar *temp3=NULL,*temp4=NULL;\n");
		fprintf(fp,"\tchar *temp5=NULL,*temp6=NULL;\n");
		fprintf(fp,"\tchar *temp7=NULL,*temp8=NULL;\n");
		fprintf(fp,"\tchar *temp9=NULL;\n");
		fprintf(fp,"\n");


		fprintf(fp,"%s ADD DESCRIPTION %s\n",thing1,thing2);
		overall_length=0;
		tempapplib=break_expression2(add->desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		fprintf(fp,"\n\n");

		fprintf(fp,"%s SAVE EXPRESSION %s\n",thing1,thing2);
		overall_length=0;
		tempapplib=break_expression2(add->save_expression,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp2=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp2,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		fprintf(fp,"\n\n");

		fprintf(fp,"%s SAVE ERROR DESCRIPTION %s\n",thing1,thing2);
		overall_length=0;
		tempapplib=break_expression2(add->save_error_desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp3=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp3,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		fprintf(fp,"\n\n");

		fprintf(fp,"%s SAVE WARNING %s\n",thing1,thing2);

		overall_length=0;
		tempapplib=break_expression2(add->save_warning,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp4=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp4,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\n\n");

		fprintf(fp,"%s SAVE WARNING DESCRIPTION %s\n",thing1,thing2);
		overall_length=0;
		tempapplib=break_expression2(add->save_warning_desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp5=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp5,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\n\n");

		fprintf(fp,"%s QUIT ERROR %s\n",thing1,thing2);
		overall_length=0;
		tempapplib=break_expression2(add->quit_error,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp6=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp6,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		fprintf(fp,"\n\n");

		fprintf(fp,"%s QUIT ERROR DESCRIPTION %s\n",thing1,thing2);

		overall_length=0;
		tempapplib=break_expression2(add->quit_error_desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp7=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp7,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\n\n");

		fprintf(fp,"%s QUIT WARNING %s\n",thing1,thing2);
		overall_length=0;
		tempapplib=break_expression2(add->quit_warning,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp8=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp8,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\n\n");

		fprintf(fp,"%s QUIT WARNING DESCRIPTION %s\n",thing1,thing2);
		overall_length=0;
		tempapplib=break_expression2(add->quit_warning_desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp9=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp9,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		if(!isEMPTY(add->addscreen)) stoupper(add->addscreen);
		if(!isEMPTY(add->define_list)) stoupper(add->define_list);

		fprintf(fp,"\tadd=RDAPowerAddNEW(module,\"%s\",temp1,\"%s\",\"%s\",\"%s\",%d,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);\n",
			addname,
			(add->addscreen!=NULL ? add->addscreen:""),
			(add->define_list!=NULL ? add->define_list:""),
			(add->reportname!=NULL ? add->reportname:""),
			add->save_type);
		fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");
		fprintf(fp,"\tif(temp3!=NULL) Rfree(temp3);\n");
		fprintf(fp,"\tif(temp4!=NULL) Rfree(temp4);\n");
		fprintf(fp,"\tif(temp5!=NULL) Rfree(temp5);\n");
		fprintf(fp,"\tif(temp6!=NULL) Rfree(temp6);\n");
		fprintf(fp,"\tif(temp7!=NULL) Rfree(temp7);\n");
		fprintf(fp,"\tif(temp8!=NULL) Rfree(temp8);\n");
		fprintf(fp,"\tif(temp9!=NULL) Rfree(temp9);\n");
		fprintf(fp,"\tif(add!=NULL)\n");
		fprintf(fp,"\t{\n");
		if(add->buttons!=NULL)
		{
		fprintf(fp,"\n%s POWERADD BUTTONS  %s\n\n",thing1,thing2);
		for(x=0,button=add->buttons;x<add->numbuttons;++x,++button)
		{	
			fprintf(fp,"%s %s %s BUTTON  %s\n",thing1,(button->buttonname!=NULL ? 
				button->buttonname:""),(button->progname!=NULL ? button->progname:""),thing2);
			if(button!=NULL)
			{
				overall_length=0;
				tempapplib=break_expression2(button->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

				if(button->args!=NULL)
				{
					fprintf(fp,"\t\t\n");
					fprintf(fp,"\t\targs=APPlibNEW();\n");
					if(button->evalargs==FALSE)
					{
						for(y=0;y<button->args->numlibs;++y)
						{
							fprintf(fp,"\t\taddAPPlib(args,\"%s\");\n",
								button->args->libs[y]);
						}
					} else {
						for(y=0;y<button->args->numlibs;++y)
						{
							overall_length=0;
							tempapplib=break_expression2(button->args->libs[y],&overall_length);
							if(tempapplib!=NULL)
							{
								fprintf(fp,"\t\ttemp2=Rmalloc(%d+1);\n",overall_length);
								fprintf(fp,"\t\tsprintf(temp2,\"");
								for(q=0;q<tempapplib->numlibs;++q)
								{
									fprintf(fp,"%%s");
								}
								fprintf(fp,"\",\n");
								for(q=0;q<tempapplib->numlibs;++q)
								{
									if(q!=(tempapplib->numlibs-1))
									{
										fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
									} else {
										fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
									}
								}
								freeapplib(tempapplib);
							}
							fprintf(fp,"\t\taddAPPlib(args,temp2);\n");
							fprintf(fp,"\t\tif(temp2!=NULL) Rfree(temp2);\n");
						}
					}
				}
				fprintf(fp,"\t\tx=addPowerAddButton(add,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",temp1,%d,%d,%s,0,NULL);\n",
					(button->progname!=NULL ? button->progname : ""),
					(button->listname!=NULL ? button->listname: ""),
					(button->buttonname!=NULL ? button->buttonname: ""),
					(button->modulename!=NULL ? button->modulename: ""),
					(button->filename!=NULL ? button->filename: ""),
					(button->keyname!=NULL ? button->keyname: ""),
					button->type,
					button->evalargs,
					(button->args!=NULL ? "args":"NULL"));
				if(button->args!=NULL)
				{
					fprintf(fp,"\t\tif(args!=NULL) freeapplib(args);\n");
				}
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				if(button->bf!=NULL && button->num>0)
				{
					fprintf(fp,"\t\tif(x!=(-1))\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tbutton=add->buttons+(x-1);\n");
					for(y=0,bf=button->bf;y<button->num;++y,++bf)
					{	
						fprintf(fp,"\t\t\tx=addPowerAddButtonField(button,\"%s\",\"%s\");\n",
							(bf->rname!=NULL ? bf->rname: ""),
							(bf->ename!=NULL ? bf->ename: ""));
					}
					fprintf(fp,"\t\t}\n");
				}
			}
		}
		fprintf(fp,"\n");
		}
		if(add->subord!=NULL)
		{
		fprintf(fp,"\n%s POWERADD SUPPORTING FILES  %s\n\n",thing1,thing2);
		for(x=0,subord=add->subord;x<add->subnum;++x,++subord)
		{	
			fprintf(fp,"\n%s %s %s SUPPORTING FILE %s\n\n",thing1,(subord->module!=NULL ? 
				subord->module:""),(subord->filename!=NULL ? subord->filename:""),thing2);
			if(subord!=NULL)
			{
				overall_length=0;
				tempapplib=break_expression2(subord->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				if(!isEMPTY(subord->browse_screen)) stoupper(subord->browse_screen);
				if(!isEMPTY(subord->define_list)) stoupper(subord->define_list);
				if(!isEMPTY(subord->search_browse)) stoupper(subord->search_browse);
				fprintf(fp,"\t\tx=addPowerAddSupportingADV(add,\"%s\",\"%s\",%d,0,NULL,\"%s\",temp1,%d,\"%s\",\"%s\",\"%s\",\"%s\",%d);\n",
					(subord->module!=NULL ? subord->module: ""),
					(subord->filename!=NULL ? subord->filename: ""),
					subord->dtype,
					(subord->subkeyname!=NULL ? subord->subkeyname: ""),
					subord->browse,
					(subord->button_name!=NULL ? subord->button_name: ""),
					(subord->browse_screen!=NULL ? subord->browse_screen: ""),
					(subord->define_list!=NULL ? subord->define_list: ""),
					(subord->search_browse!=NULL ? subord->search_browse: ""),
					subord->smart_browse);
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				if(subord->confld!=NULL && subord->num>0)
				{
					fprintf(fp,"\t\tif(x!=(-1))\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tsubord=add->subord+(x-1);\n");
					for(y=0,confld=subord->confld;y<subord->num;++y,++confld)
					{	
						overall_length=0;
						tempapplib=break_expression2(confld->conname,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}

						fprintf(fp,"\t\t\tx=addPowerAddSupportingConfld(subord,\"%s\",temp1,%d);\n",
							(confld->fldname!=NULL ? confld->fldname: ""),
							confld->inherit);
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
					}
					fprintf(fp,"\t\t}\n");
				}
			}
		}
		fprintf(fp,"\n");
		}
		if(add->Writes!=NULL)
		{
		fprintf(fp,"\n%s POWERADD WRITES  %s\n\n",thing1,thing2);
		for(x=0,Write=add->Writes;x<add->numWrites;++x,++Write)
		{	
			fprintf(fp,"\n%s %s %s POWER WRITE %s\n\n",thing1,(Write->module!=NULL ? 
				Write->module:""),(Write->filename!=NULL ? Write->filename:""),thing2);
			if(Write!=NULL)
			{
				overall_length=0;
				tempapplib=break_expression2(Write->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

				fprintf(fp,"\t\tx=addPowerAddWrite(add,\"%s\",\"%s\",temp1,0,NULL);\n",
					(Write->module!=NULL ? Write->module: ""),
					(Write->filename!=NULL ? Write->filename: ""));
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				if(Write->fields!=NULL && Write->numflds>0)
				{
					fprintf(fp,"\t\tif(x!=(-1))\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tWrite=add->Writes+(x-1);\n");
					for(y=0,field=Write->fields;y<Write->numflds;++y,++field)
					{	
						overall_length=0;
						tempapplib=break_expression2(field->from_name,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}

						fprintf(fp,"\t\t\tx=addPowerAddWriteField(Write,\"%s\",%d,temp1);\n",
							(field->to_name!=NULL ? field->to_name: ""),
							field->copy_type);
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
					}
					fprintf(fp,"\t\t}\n");
				}
			}
		}
		fprintf(fp,"\n");
		}
		if(add->EditWdgts!=NULL)
		{
		fprintf(fp,"\n%s POWERADD EDIT WIDGETS  %s\n\n",thing1,thing2);
		for(x=0,EditWdgt=add->EditWdgts;x<add->numwdgts;++x,++EditWdgt)
		{	
			fprintf(fp,"\n%s %s EDIT WIDGET %s\n",thing1,(EditWdgt->name!=NULL ? 
				EditWdgt->name:""),thing2);
			if(EditWdgt!=NULL)
			{
				if(EditWdgt->loadvirtual==1)
				{
					fprintf(fp,"\t\td=getDFvirtual(module,\"%s\");\n",EditWdgt->name);
				} else {
				overall_length=0;
				tempapplib=break_expression2(EditWdgt->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				}

				overall_length=0;
				tempapplib=break_expression2(EditWdgt->mgt_expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp2=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp2,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

				overall_length=0;
				tempapplib=break_expression2(EditWdgt->override_expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp3=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp3,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

				if(EditWdgt->acctrsc!=NULL)
				{
					fprintf(fp,"\t\t\n");
					fprintf(fp,"\t\targs=APPlibNEW();\n");
					for(y=0;y<EditWdgt->acctrsc->numlibs;++y)
					{
						fprintf(fp,"\t\taddAPPlib(args,\"%s\");\n",
							EditWdgt->acctrsc->libs[y]);
					}
				}
				if(EditWdgt->defrsc!=NULL)
				{
					fprintf(fp,"\t\t\n");
					fprintf(fp,"\t\targs2=APPlibNEW();\n");
					for(y=0;y<EditWdgt->defrsc->numlibs;++y)
					{
						fprintf(fp,"\t\taddAPPlib(args2,\"%s\");\n",
							EditWdgt->defrsc->libs[y]);
					}
				}
				if(EditWdgt->loadvirtual!=1)
				{
					fprintf(fp,"\t\tx=addPowerAddEditWdgts(add,\"%s\",%d,%d,%d,temp1,%d,\"%s\",\"%s\",\"%s\",0,NULL,temp2,\"%s\",\"%s\",temp3,%d,%s,%d,%s);\n",
						(EditWdgt->name!=NULL ? EditWdgt->name: ""),
						EditWdgt->type,
						EditWdgt->len,
						EditWdgt->etype,
						EditWdgt->id_mgt,
						(EditWdgt->module!=NULL ? EditWdgt->module: ""),
						(EditWdgt->filename!=NULL ? EditWdgt->filename: ""),
						(EditWdgt->subkeyname!=NULL ? EditWdgt->subkeyname: ""),
						(EditWdgt->assignname!=NULL ? EditWdgt->assignname: ""),
						(EditWdgt->id!=NULL ? EditWdgt->id: ""),
						EditWdgt->setaccttype,
						(EditWdgt->acctrsc!=NULL ? "args":"NULL"),
						EditWdgt->setdeftype,
						(EditWdgt->defrsc!=NULL ? "args2":"NULL"));
					if(EditWdgt->loadvirtual==2)
					{
						fprintf(fp,"\t\tEditWdgt=add->EditWdgts+(x-1);\n");
						fprintf(fp,"\t\tEditWdgt->loadvirtual=2;\n");
					}
				} else {
					fprintf(fp,"\t\tx=addPowerAddEditWdgts(add,\"%s\",d->type,d->length,%d,d->expression,%d,\"%s\",\"%s\",\"%s\",0,NULL,temp2,\"%s\",\"%s\",temp3,%d,%s,%d,%s);\n",
						(EditWdgt->name!=NULL ? EditWdgt->name: ""),
						EditWdgt->etype,
						EditWdgt->id_mgt,
						(EditWdgt->module!=NULL ? EditWdgt->module: ""),
						(EditWdgt->filename!=NULL ? EditWdgt->filename: ""),
						(EditWdgt->subkeyname!=NULL ? EditWdgt->subkeyname: ""),
						(EditWdgt->assignname!=NULL ? EditWdgt->assignname: ""),
						(EditWdgt->id!=NULL ? EditWdgt->id: ""),
						EditWdgt->setaccttype,
						(EditWdgt->acctrsc!=NULL ? "args":"NULL"),
						EditWdgt->setdeftype,
						(EditWdgt->defrsc!=NULL ? "args2":"NULL"));
				}
				if(EditWdgt->acctrsc!=NULL)
				{
					fprintf(fp,"\t\tif(args!=NULL) freeapplib(args);\n");
				}
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\t\tif(temp2!=NULL) Rfree(temp2);\n");
				fprintf(fp,"\t\tif(temp3!=NULL) Rfree(temp3);\n");
				if(EditWdgt->defrsc!=NULL)
				{
					fprintf(fp,"\t\tif(args2!=NULL) freeapplib(args2);\n");
				}
				if(EditWdgt->loadvirtual==TRUE)
				{
					fprintf(fp,"\t\tif(d!=NULL)\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tFreeDFvirtual(d);\n");
					fprintf(fp,"\t\t}\n");
				}
				if(EditWdgt->confld!=NULL && EditWdgt->num>0)
				{
					fprintf(fp,"\t\tif(x!=(-1))\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tEditWdgt=add->EditWdgts+(x-1);\n");
					for(y=0,editfld=EditWdgt->confld;y<EditWdgt->num;++y,++editfld)
					{	
						overall_length=0;
						tempapplib=break_expression2(editfld->conname,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}

						fprintf(fp,"\t\t\tx=addPowerAddEditWdgtEditFld(EditWdgt,\"%s\",temp1);\n",
							(editfld->fldname!=NULL ? editfld->fldname: ""));
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
					}
					fprintf(fp,"\t\t}\n");
				}
			}
		}
		}

		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.ADD\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.ADD\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writePowerAdd(defdir,add))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+89+1);\n",RDAstrlen(add->name),RDAstrlen(add->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"POWER ADD WRITE ERROR: Module [%s] Power Add [%s], Can Not Save Power Add!\");\n",(add->module!=NULL?add->module:""),(add->name!=NULL?add->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(args!=NULL) freeapplib(args);\n");
		fprintf(fp,"\t\tif(args2!=NULL) freeapplib(args2);\n");
		fprintf(fp,"\t\tif(add!=NULL) FreePowerAdd(add);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}
}
/* From mkaddcd */
/* From mklistcd */
void makelistcode(RDAScrolledList *list,FILE *fp,char *libname,char *listname)
{
	int x=0,q=0;
	DFincvir *i=NULL;
	char *temp=NULL;
	char *dirx=NULL,*libx=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
		sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\rda",CURRENTDIRECTORY);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
#ifndef WIN32
		sprintf(libx,"%s/%s.LST",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.LST",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
		temp=fixname(list->name);
#ifdef PAIN_IN_
		fprintf(fp,"/* %s's - Make (%s) List */\n",list->module,list->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"static void LST_%s(void);\n",temp);
		fprintf(fp,"\n");
#endif /* PAIN_IN_ */
		fprintf(fp,"static void LST_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDAScrolledList *list=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
		fprintf(fp,"\n");

		fprintf(fp,"\tlist=RDAScrolledListNew(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(listname!=NULL ? listname:""));
		fprintf(fp,"\tif(list!=NULL)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tlist->vtype=%d;\n",list->vtype);
		fprintf(fp,"\t\tlist->type=%d;\n",list->type);
		if(list->incvir!=NULL)
		{
			for(x=0,i=list->incvir;x<list->numincvirs;++x,++i)
			{
				fprintf(fp,"\t\tScrolledListDFincvir(list,\"%s\",\"%s\",\"%s\",%d);\n",(i->module!=NULL ? i->module:""),
					(i->file!=NULL ? i->file:""),	
					(i->keyname!=NULL ? i->keyname:""),
					i->fileno);
			}
		}
		overall_length=0;
		tempapplib=break_expression2(list->select_formula,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\t\tlist->select_formula=stralloc(temp1);\n");
		fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\tlist->special_function=stralloc(\"%s\");\n",(list->special_function!=NULL ? list->special_function:""));
		fprintf(fp,"\t\tlist->num=0;\n");
		fprintf(fp,"\t\tlist->initfld=NULL;\n");
		fprintf(fp,"\t\tlist->ffield=stralloc(\"%s\");\n",(list->ffield!=NULL ? list->ffield:""));
		fprintf(fp,"\t\tlist->ffinfo=Rmalloc(sizeof(DFincvir));\n");
		fprintf(fp,"\t\tlist->ffinfo->module=stralloc(\"%s\");\n",(list->ffinfo->module!=NULL ? list->ffinfo->module:""));
		fprintf(fp,"\t\tlist->ffinfo->file=stralloc(\"%s\");\n",(list->ffinfo->file!=NULL ? list->ffinfo->file:""));
		fprintf(fp,"\t\tlist->ffinfo->keyname=stralloc(\"%s\");\n",(list->ffinfo->keyname!=NULL ? list->ffinfo->keyname:""));
		fprintf(fp,"\t\tlist->contype=%d;\n",list->contype);
		fprintf(fp,"\t\tlist->conmod=stralloc(\"%s\");\n",(list->conmod!=NULL ? list->conmod:""));
		fprintf(fp,"\t\tlist->confil=stralloc(\"%s\");\n",(list->confil!=NULL ? list->confil:""));
		fprintf(fp,"\t\tlist->confld=stralloc(\"%s\");\n",(list->confld!=NULL ? list->confld:""));
		overall_length=0;
		tempapplib=break_expression2(list->format_formula,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\t\tlist->format_formula=stralloc(temp1);\n");
		fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");


		overall_length=0;
		tempapplib=break_expression2(list->reformat_formula,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\t\tlist->reformat_formula=stralloc(temp1);\n");
		fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");

		overall_length=0;
		tempapplib=break_expression2(list->unformat_formula,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\t\tlist->unformat_formula=stralloc(temp1);\n");
		fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\tlist->desc=stralloc(\"%s\");\n",(list->desc!=NULL ? list->desc:""));
		fprintf(fp,"\t\tlist->timing=%d;\n",list->timing);
		if(list->list!=NULL)
		{
			fprintf(fp,"\t\tlist->list=APPlibNEW();\n");
			for(x=0;x<list->list->numlibs;++x)
			{
				if(list->type<3)
				{
					fprintf(fp,"\t\taddAPPlib(list->list,\"%s\");\n",list->list->libs[x]);
				} else {
					temp=Rmalloc(RDAstrlen(list->list->libs[x])+3);
					sprintf(temp,"\"%s\"",list->list->libs[x]);
					sub_quotes(&temp);
					fprintf(fp,"\t\taddAPPlib(list->list,%s);\n",temp);
					if(temp!=NULL) Rfree(temp);
				}
			}
		}


		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.LST\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.LST\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writeScrolledListbin(defdir,list))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+100+1);\n",RDAstrlen(list->name),RDAstrlen(list->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"SCROLLED LIST WRITE ERROR: Module [%s] RDAScrolledList [%s], Can Not Save Scrolled List!\");\n",(list->module!=NULL?list->module:""),(list->name!=NULL?list->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(list!=NULL) FreeRDAScrolledList(list);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}
}
RDAScrolledList *oklist(char *libname,char *listname)
{
	RDAScrolledList *list=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(list!=NULL) FreeRDAScrolledList(list); 
		list=RDAScrolledListNew(libname,listname);
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
		sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.LST",dirx,libname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\rda",CURRENTDIRECTORY);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s\\%s.LST",dirx,libname);
#endif
		if(dirx!=NULL) Rfree(dirx);
		if(getScrolledListbin(libx,list))
		{
			prterr("ERROR: Cannot Read RDAScrolledList Definition.  Error in permissions or list [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,listname);
			if(list!=NULL) FreeRDAScrolledList(list);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(list);
		}
	} else { 
		return(NULL);
	}
}
/* End from mklistcd */
/* From mkvircd */
void makevirfldcode(DFvirtual *virfld,FILE *fp,char *libname,char *virfldname)
{
	int x=0,q=0;
	DFincvir *i=NULL;
	char *temp=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;

	if(fp!=NULL)
	{	
		temp=fixname(virfld->name);
#ifdef PAIN_IN_
		fprintf(fp,"/* %s's - Make (%s) List */\n",virfld->module,virfld->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		fprintf(fp,"static void VIR_%s(void);\n",temp);
		fprintf(fp,"\n");
#endif /* PAIN_IN_ */
		fprintf(fp,"static void VIR_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tDFvirtual *virfld=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");

		fprintf(fp,"\n");

		fprintf(fp,"\tvirfld=DFvirtualNEW(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(virfldname!=NULL ? virfldname:""));
		fprintf(fp,"\tif(virfld!=NULL)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tvirfld->vtype=%d;\n",virfld->vtype);
		fprintf(fp,"\t\tvirfld->type=%d;\n",virfld->type);
		if(virfld->incvir!=NULL)
		{
			for(x=0,i=virfld->incvir;x<virfld->num;++x,++i)
			{
				fprintf(fp,"\t\tDFvirtualDFincvir(virfld,\"%s\",\"%s\",\"%s\",%d);\n",(i->module!=NULL ? i->module:""),
					(i->file!=NULL ? i->file:""),	
					(i->keyname!=NULL ? i->keyname:""),
					i->fileno);
			}
		}
		fprintf(fp,"\t\tvirfld->avail2rsrc=%d;\n",(virfld->avail2rsrc ? TRUE:FALSE));
		fprintf(fp,"\t\tvirfld->type=%d;\n",virfld->type);
		fprintf(fp,"\t\tvirfld->length=%d;\n",virfld->length);
		fprintf(fp,"\t\tvirfld->when=%d;\n",virfld->when);

		overall_length=0;
		tempapplib=break_expression2(virfld->expression,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\t\tvirfld->expression=stralloc(temp1);\n");
		fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\tvirfld->desc=stralloc(\"%s\");\n",(virfld->desc!=NULL ? virfld->desc:""));

		fprintf(fp,"\n");
		fprintf(fp,"\t\tif(writeDFvirtual(module,virfld))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+110+1);\n",RDAstrlen(virfld->name),RDAstrlen(virfld->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"VIRTUAL FIELD WRITE ERROR: Module [%s] Virtual Field [%s], Can Not Save Virtual Field!\");\n",(virfld->module!=NULL?virfld->module:""),(virfld->name!=NULL?virfld->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(virfld!=NULL) FreeDFvirtual(virfld);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");
	}
}
DFvirtual *okvirfld(char *libname,char *virfldname)
{
	DFvirtual *virfld=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(virfld!=NULL) FreeDFvirtual(virfld); 
		virfld=getDFvirtual(libname,virfldname);
		if(virfld==NULL)
		{
			prterr("ERROR: Cannot Read DFvirtual Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,virfldname);
			if(virfld!=NULL) FreeDFvirtual(virfld);
			return(NULL);
		} else {
			return(virfld);
		}
	} else { 
		return(NULL);
	}
}
/* End from mkvircd */
/* From mkmenucd */
void makemenucode(RDAmenu *menu,FILE *fp,char *dirname,char *libname,char *menuname)
{
	char *temp=NULL,*temp1=NULL;
	char *dirx=NULL,*libx=NULL;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(dirname)+5);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
		sprintf(libx,"%s/%s.MNU",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
		temp=fixname(menuname);
#ifdef PAIN_IN_
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"char *module=\"%s\";\n",libname);
		fprintf(fp,"static void MNU_%s(void);\n",temp);
		fprintf(fp,"\n");
#endif /* PAIN_IN_ */
		fprintf(fp,"static void MNU_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDAmenu *menu=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\n");

		temp1=Rmalloc(RDAstrlen(menu->command)+4);
		sprintf(temp1,"\"%s\"",menu->command);
		sub_quotes(&temp1);
		fprintf(fp,"\tmenu=RDAmenuNEW(\"%s\",%s);\n",(menu->name!=NULL ? menu->name:""),(temp1!=NULL ? temp1:""));
		if(temp1!=NULL) Rfree(temp1);
		fprintf(fp,"\tif(menu!=NULL)\n");
		fprintf(fp,"\t{\n");
#ifdef EXT_EXTENSION /* DONE THRU .EXT EXTENSION */
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tConvMenu(menu,\"exe\");\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef RS6000\n");
		fprintf(fp,"\t\tConvMenu(menu,\"rsc\");\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#if defined(SCO50) || defined(SCO50P)\n");
		fprintf(fp,"\t\tConvMenu(menu,\"sco\");\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)\n");
		fprintf(fp,"\t\tConvMenu(menu,\"lnx\");\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef UNIXWARE7\n");
		fprintf(fp,"\t\tConvMenu(menu,\"unw\");\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef UNIXWARE21\n");
		fprintf(fp,"\t\tConvMenu(menu,\"unw\");\n");
		fprintf(fp,"#endif\n");
#endif  /* EXT_EXTENSION */
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.MNU\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.MNU\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writemenubin(defdir,menu))\n");
		fprintf(fp,"\t\t{\n");
		fprintf(fp,"\t\t\tERRORDIALOG(\"Cannot Save Menu\",\"This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.\",NULL,FALSE);\n");
		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(menu!=NULL) free_menu(menu);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}
}
RDAmenu *okmenu(char *dirname,char *libname,char *menuname)
{
	RDAmenu *menu=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(menu!=NULL) free_menu(menu); 
		menu=RDAmenuNEW(menuname,NULL);
		dirx=Rmalloc(RDAstrlen(dirname)+2);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.MNU",dirx,libname);
		if(dirx!=NULL) Rfree(dirx);
		if(getmenubin(libx,menu))
		{
			prterr("ERROR: Cannot Read Menu Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,menuname);
			if(menu!=NULL) free_menu(menu);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(menu);
		}
	} else { 
		return(NULL);
	}
}
/* End from mkmenucd */
/* From mkloadcd */
void makeloadcode(RDALoadValue *scrn,FILE *fp,char *libname,char *scrnname)
{
	int x=0;
	DFincvir *i=NULL;
	char *temp=NULL;
	char *dirx=NULL,*libx=NULL;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
		sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\rda",CURRENTDIRECTORY);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
#ifndef WIN32
		sprintf(libx,"%s/%s.LDV",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.LDV",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
		temp=fixname(scrn->name);
#ifdef PAIN_IN_
		fprintf(fp,"/* %s's - Make (%s) Load Value */\n",scrn->module,scrn->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"static void RLV_%s(void);\n",temp);
		fprintf(fp,"\n");
#endif /* PAIN_IN_ */
		fprintf(fp,"static void RLV_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDALoadValue *scrn=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
		fprintf(fp,"\n");

		fprintf(fp,"\tscrn=RDALoadValueNEW(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"\tif(scrn!=NULL)\n");
		fprintf(fp,"\t{\n");
		if(scrn->incvir!=NULL)
		{
			for(x=0,i=scrn->incvir;x<scrn->numincvirs;++x,++i)
			{
				fprintf(fp,"\t\tRDALoadValueDFincvir(scrn,\"%s\",\"%s\",\"%s\",%d);\n",(i->module!=NULL ? i->module:""),
					(i->file!=NULL ? i->file:""),	
					(i->keyname!=NULL ? i->keyname:""),
					i->fileno);
			}
		}
		fprintf(fp,"\t\tscrn->conname=stralloc(\"%s\");\n",scrn->conname);
		fprintf(fp,"\t\tscrn->desc=stralloc(\"%s\");\n",(scrn->desc!=NULL ? scrn->desc:""));
		if(scrn->mlist!=NULL)
		{
			fprintf(fp,"\t\tscrn->mlist=APPlibNEW();\n");
			for(x=0;x<scrn->mlist->numlibs;++x)
			{
				fprintf(fp,"\t\taddAPPlib(scrn->mlist,\"%s\");\n",scrn->mlist->libs[x]);
			}
		}

		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.LDV\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.LDV\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writeRDALoadValuebin(defdir,scrn))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+100+1);\n",RDAstrlen(scrn->name),RDAstrlen(scrn->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"LOAD VALUE WRITE ERROR: Module [%s] RDALoadValue [%s], Can Not Save Load Value!\");\n",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(scrn!=NULL) FreeRDALoadValue(scrn);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}
}
RDALoadValue *okload(char *libname,char *scrnname)
{
	RDALoadValue *scrn=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(scrn!=NULL) FreeRDALoadValue(scrn); 
		scrn=RDALoadValueNEW(libname,scrnname);
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
		sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.LDV",dirx,libname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\rda",CURRENTDIRECTORY);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s\\%s.LDV",dirx,libname);
#endif
		if(dirx!=NULL) Rfree(dirx);
		if(getRDALoadValuebin(libx,scrn))
		{
			prterr("ERROR: Cannot Read RDALoadValue Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,scrnname);
			if(scrn!=NULL) FreeRDALoadValue(scrn);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(scrn);
		}
	} else { 
		return(NULL);
	}
}
/* End from mkloadcd */
/* From mkdefcd */
void makedefcode(RDAdefault *def,FILE *fp,char *dirname,char *libname,char *scrnname)
{
	int x=0;
	char *temp=NULL;
	DEFfield *tmpfield=NULL;
	char *dirx=NULL,*libx=NULL;

	if(def!=NULL)
	{
		if(!isEMPTY(def->name)) stoupper(def->name);
	}
	if(!isEMPTY(scrnname)) stoupper(scrnname);
	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(dirname)+5);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
		sprintf(libx,"%s/%s.DEF",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
		temp=fixname(def->name);
#ifdef _PAIN_IN_
		fprintf(fp,"/* %s's - Make (%s) Screen Default */\n",def->module,def->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"#include <mkdef.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"char *module=\"%s\";\n",def->module);
		fprintf(fp,"extern APPlib *errorlist;\n");
		fprintf(fp,"static void DEF_%s(void);\n",temp);
		fprintf(fp,"\n");
#endif /* _PAIN_IN_ */
		fprintf(fp,"static void DEF_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDAdefault *def=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
		fprintf(fp,"\n");


		fprintf(fp,"\tdef=RDAdefaultNEW(\"%s\",\"%s\");\n",
			(libname!=NULL ? libname:""),
			(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"\tif(def!=NULL)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tdef->input_focus=stralloc(\"%s\");\n",(def->input_focus!=NULL?def->input_focus:""));
		if(def->field!=NULL && def->num>0)
		{
			for(x=0,tmpfield=def->field;x<def->num;++x,++tmpfield)
			{
				switch(tmpfield->type)
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
						fprintf(fp,"\t\taddDefaultFieldSTRING(def,\"%s\",%s,%d,\"%s\");\n",
							(tmpfield->name!=NULL?tmpfield->name:""),
							(tmpfield->reset==TRUE?"TRUE":"FALSE"),
							tmpfield->type,
							(tmpfield->data.string_value!=NULL?tmpfield->data.string_value:""));
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case SDECIMALV:
					case SDOUBLEV:
					case DOUBLEV:
						fprintf(fp,"\t\taddDefaultFieldDOUBLE(def,\"%s\",%s,%d,%f);\n",
							(tmpfield->name!=NULL?tmpfield->name:""),
							(tmpfield->reset==TRUE?"TRUE":"FALSE"),
							tmpfield->type,
							*tmpfield->data.float_value);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						fprintf(fp,"\t\taddDefaultFieldINT(def,\"%s\",%s,%d,%d);\n",
							(tmpfield->name!=NULL?tmpfield->name:""),
							(tmpfield->reset==TRUE?"TRUE":"FALSE"),
							tmpfield->type,
							*tmpfield->data.integer_value);
						break;
					case SHORTV:
					case SSHORTV:
						fprintf(fp,"\t\taddDefaultFieldSHORT(def,\"%s\",%s,%d,%d);\n",
							(tmpfield->name!=NULL?tmpfield->name:""),
							(tmpfield->reset==TRUE?"TRUE":"FALSE"),
							tmpfield->type,
							*tmpfield->data.short_value);
						break;
					case BOOLNS:
					case CHARACTERS:
						fprintf(fp,"\t\taddDefaultFieldCHAR(def,\"%s\",%s,%d,%d);\n",
							(tmpfield->name!=NULL?tmpfield->name:""),
							(tmpfield->reset==TRUE?"TRUE":"FALSE"),
							tmpfield->type,
							*tmpfield->data.string_value);
						break;
					default :
						prterr("Error: Unrecognized Field Type.");
						break;
				}
			}
		}
		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.DEF\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.DEF\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writedefaultbin(defdir,def))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+100+1);\n",RDAstrlen(def->name),RDAstrlen(def->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"SCREEN DEFAULT WRITE ERROR: Module [%s] Screen [%s], Can Not Save Screen Defaults!\");\n",(def->module!=NULL?def->module:""),(def->name!=NULL?def->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(def!=NULL) FreeRDAdefault(def);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}

}
RDAdefault *okdef(char *dirname,char *libname,char *scrnname)
{
	return okdef_full(dirname,libname,scrnname,1);
}
RDAdefault *okdef_noerrors(char *dirname,char *libname,char *scrnname)
{
	return okdef_full(dirname,libname,scrnname,0);
}
RDAdefault *okdef_full(char *dirname,char *libname,char *scrnname,char printerrors)
{
	RDAdefault *def=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(def!=NULL) FreeRDAdefault(def); 
		def=RDAdefaultNEW(libname,scrnname);
		dirx=Rmalloc(RDAstrlen(dirname)+2);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.DEF",dirx,libname);
		if(dirx!=NULL) Rfree(dirx);
		if(getdefaultbin(libx,scrnname,def,FALSE))
		{
			if(printerrors)
			{
				prterr("ERROR: Cannot Read Screen Default Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,scrnname);
			}
			if(def!=NULL) FreeRDAdefault(def);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(def);
		}
	} else { 
		return(NULL);
	}
}
/* End from mkdefcd */
/* From mkrptcd */
void makerptcode(RDAreport *r,FILE *fp,char *module,char printheader)
{
/*
	int z,q,numlines=0;
	RDAaccval *accval=NULL;
*/
	int z=0;
	int x=0,y=0,q=0;
	char *funcname=NULL,set_RDAvirtual=FALSE;
	RDAsearch *s=NULL;
	RDAvirtual *v=NULL;
	RDAsort *sort=NULL;
	RDAimage *i=NULL;
	RDApval *pval=NULL;
	RDApvar *pvar=NULL;
	RDAcontrol *c=NULL;
	RDAaccum *acc=NULL;
	RDAdisplay *d=NULL;
	RDAconfld *confld=NULL;
	RDAsubord *subord=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;
	MaintainMaster *MTNMASTER=NULL;
	char *thing1="/*",*thing2="*/";

	if(fp!=NULL)
	{	
		if(printheader)
		{
			fprintf(fp,"/* %s's - Make (%s) Report */\n",
					(module!=NULL?module:""),
					(r->name!=NULL?r->name:""));
			fprintf(fp,"#include <cstdio>\n");
			fprintf(fp,"#include <rptgen.hpp>\n");
			fprintf(fp,"#include <mkmtn.hpp>\n");
			fprintf(fp,"#include <mkmsc.hpp>\n");
			fprintf(fp,"#include <mkez.hpp>\n");
			fprintf(fp,"/*COMPMEM 40960 */\n");
			fprintf(fp,"\n");
			fprintf(fp,"extern char *module;\n");
			fprintf(fp,"extern APPlib *errorlist;\n");
			fprintf(fp,"\n");
		}
		funcname=fixname(r->name);
		fprintf(fp,"static void RPT_%s()\n",(funcname!=NULL?funcname:""));
		if(funcname!=NULL) Rfree(funcname);
		fprintf(fp,"{\n");
		fprintf(fp,"\tint x=0;\n");
		fprintf(fp,"\tRDAreport *tmp=NULL;\n");
		fprintf(fp,"\tRDAsearch *s=NULL;\n");
		fprintf(fp,"\tshort engine=0;\n");
		if(r->virflds!=NULL)
		{
			for(x=0,v=r->virflds;x<r->numvirtuals;++x,++v)
			{
				if(v->computed==TRUE) set_RDAvirtual=TRUE;
			}
		}
		if(set_RDAvirtual)
		{
			fprintf(fp,"\tDFvirtual *d=NULL;\n");
		}
		fprintf(fp,"/*\n");
		fprintf(fp,"\tRDApval *pval=NULL,*pvals=NULL;\n");
		fprintf(fp,"\tint y,z;\n");
		fprintf(fp,"\tshort longest=132;\n");
		fprintf(fp,"\tint numpvars=0;\n");
		fprintf(fp,"\tRDAacct *acc=NULL;\n");
		fprintf(fp,"\tRDAaccpart *part=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"*/\n");
		fprintf(fp,"\tint imageoff=0;\n");
		fprintf(fp,"\tRDAimage *image=NULL;\n");
		fprintf(fp,"\tchar *t=NULL,*desc=NULL,*rptname=NULL;\n");
		if(r->display!=NULL)
		{
			d=r->display;
			if(d->pvars!=NULL)
			{
				fprintf(fp,"\tRDApvar *pvar=NULL;\n");
			}
		}
		fprintf(fp,"\tRDApvar *pvars=NULL;\n");
		fprintf(fp,"\n");
		if(r->MTNMSTR!=NULL)
		{
			MTNMASTER=r->MTNMSTR;
			if(MTNMASTER->s!=NULL)
			{
				fprintf(fp,"\tRDAsearch *S=NULL;\n");
				z=TRUE;
			}
			if(MTNMASTER->subord!=NULL)
			{
				fprintf(fp,"\tRDAsubord *SUBORD=NULL;\n");
				z=TRUE;
			}
			if(MTNMASTER->NewWrites!=NULL || MTNMASTER->PrevWrites!=NULL)
			{
				y=TRUE;		
			}
			if(MTNMASTER->subord!=NULL && y==FALSE)
			{
				for(x=0,subord=MTNMASTER->subord;x<MTNMASTER->subnum;++x,++subord)
				{
					if(subord->UsePowerWrite==TRUE)
					{
						y=TRUE;
						break;
					}
				}
			}
			if(y==TRUE)
			{
				fprintf(fp,"\tPowerWrite *WRTE=NULL;\n");
				z=TRUE;
			}
			y=0;
			if(MTNMASTER->Actions!=NULL)
			{
				fprintf(fp,"\tMTNAction *ACTION=NULL;\n");
				z=TRUE;
			}
			if(z==TRUE)
			{
				fprintf(fp,"\tshort r=0;\n");
			}
			z=FALSE;
			fprintf(fp,"\tMaintainMaster *MTNMSTR=NULL;\n");
			fprintf(fp,"\tAPPlib *args=NULL,*args2=NULL;\n");
			if(MTNMASTER->buttons!=NULL)
			{
				fprintf(fp,"\tMaintainButton *button=NULL;\n");
			}
			fprintf(fp,"\tchar *temp4=NULL;\n");
			fprintf(fp,"\tchar *temp5=NULL,*temp6=NULL;\n");
			fprintf(fp,"\n");
		}
		fprintf(fp,"\tchar *temp1=NULL,*temp2=NULL,*temp3=NULL;\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");


		fprintf(fp,"\tmemset(stemp,0,101);\n");
		fprintf(fp,"\trptname=Rmalloc(%d+1);\n",RDAstrlen(r->name));
		fprintf(fp,"\tsprintf(rptname,\"%s\");\n",(r->name!=NULL?r->name:""));
		fprintf(fp,"\tt=Rmalloc(%d+1);\n",(r->title!=NULL?RDAstrlen(r->title):3));
		fprintf(fp,"\tsprintf(t,\"%s\");\n",(r->title!=NULL?r->title:""));
		fprintf(fp,"\tdesc=Rmalloc(%d+1);\n",(r->desc!=NULL?RDAstrlen(r->desc):3));
		fprintf(fp,"\tsprintf(desc,\"%s\");\n",(r->desc!=NULL?r->desc:""));
		fprintf(fp,"#if defined(USE_MYISAM) \n");
		fprintf(fp,"\tengine=NRDMyIsamEngine;\n");
		fprintf(fp,"#else /* USE_MYISAM */\n");
		fprintf(fp,"#if !defined(WIN32) && !defined(LINUX) \n");
		if(r->numsorts>31)
		{
			fprintf(fp,"\tengine=NRDRdaEngine;\n");
		} else if(r->numsorts>7) 
		{
			fprintf(fp,"\tengine=NRDLocalCtreeEngine;\n");
		} else {	
			fprintf(fp,"\tengine=NRDInformixIsamEngine;\n");
		}
		fprintf(fp,"#endif\n");
		fprintf(fp,"#if defined(WIN32) || defined(LINUX) \n");
		if(r->numsorts>31)
		{
			fprintf(fp,"\tengine=NRDRdaEngine;\n");
		} else {	
			fprintf(fp,"\tengine=NRDLocalCtreeEngine;\n");
		}
		fprintf(fp,"#endif\n");
		fprintf(fp,"#endif /* USE_MYISAM */\n");

		fprintf(fp,"\ttmp=RDAreportNEWADV3(rptname,t,desc,engine,\"%s\",%s,%s,%s,%s,%s,\"%s\",\"%s\",%s,%d,%s,\"%s\",%s,%s,%s,%s);\n",
			(r->server_name!=NULL ? r->server_name:""),
			(r->process_exceptions==TRUE?"TRUE":"FALSE"),
			(r->use_print_screen==TRUE?"TRUE":"FALSE"),
			(r->stop_on_error==TRUE?"TRUE":"FALSE"),
			(r->ask_b4_execute==TRUE?"TRUE":"FALSE"),
			(r->chain_report==TRUE?"TRUE":"FALSE"),
			(r->chain_module!=NULL?r->chain_module:""),
			(r->chain_name!=NULL?r->chain_name:""),
			(r->range_screen==TRUE?"TRUE":"FALSE"),
			r->diag_breakable,
			(r->removesort==TRUE?"TRUE":"FALSE"),
			(r->remove_name!=NULL?r->remove_name:""),
			(r->no_empty_error==TRUE?"TRUE":"FALSE"),
			(r->no_wc_diag==TRUE?"TRUE":"FALSE"),
			(r->writecount_only==TRUE ? "TRUE":"FALSE"),
			(r->write_all_detail_levels==TRUE ? "TRUE":"FALSE"));
		fprintf(fp,"\ttmp->report_completion=%d;\n",r->report_completion);
		fprintf(fp,"\ttmp->remove_file=%d;\n",r->remove_file);
		fprintf(fp,"\ttmp->remove_name=stralloc(\"%s\");\n",(r->remove_name!=NULL ? r->remove_name:""));
		fprintf(fp,"\ttmp->manual_form=%d;\n",r->manual_form);
		fprintf(fp,"\tif(t!=NULL) Rfree(t);\n");
		fprintf(fp,"\tif(desc!=NULL) Rfree(desc);\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");

		if(r->display!=NULL)
		{
			fprintf(fp,"%s REPORT DISPLAY DEFINITION %s\n\n",thing1,thing2);
			d=r->display;
			if(d->pvars!=NULL)
			{
				if(d->num>0)
				{
					fprintf(fp,"\tpvars=Rmalloc(%d*sizeof(RDApvar));\n",d->num);
					fprintf(fp,"\tpvar=pvars;\n");
					for(y=0,pvar=d->pvars;y<d->num;++y,++pvar)
					{
						if(y!=0)
						{
							fprintf(fp,"\n");
							fprintf(fp,"\t++pvar;\n");
						}
						if(pvar->name!=NULL)
						{
							fprintf(fp,"\tpvar->name=stralloc(\"%s\");\n",(pvar->name!=NULL?pvar->name:""));
						} else {
							fprintf(fp,"\tpvar->name=NULL;\n");
						}
						if(pvar->command!=NULL)
						{
							fprintf(fp,"\tpvar->command=stralloc(\"%s\");\n",(pvar->command!=NULL?pvar->command:""));
						} else {
							fprintf(fp,"\tpvar->command=NULL;\n");
						}
						fprintf(fp,"\tpvar->chgsize=%s;\n",(pvar->chgsize==TRUE?"TRUE":"FALSE"));
						if(pvar->chgsize==TRUE)
						{
							fprintf(fp,"\tpvar->pitch=%d;\n",pvar->pitch);
							fprintf(fp,"\tpvar->points=%d;\n",pvar->points);
						}
					}
				} else {
					fprintf(fp,"\tpvars=NULL;\n");
				}
			} else {
				fprintf(fp,"\tpvars=NULL;\n");
			}
			fprintf(fp,"\tadddisplayADV5(tmp,(double)%f,(double)%f,%d,%s,%d,NULL,%s,%d,%d,%d,pvars,%d,%d,%d,%s,\"%s\");\n",
				d->page_width,
				d->page_length,
				d->device,
				(d->date_order==TRUE?"TRUE":"FALSE"),
				d->test_pages,
				(d->set_lpp_eval==FALSE?"FALSE":"TRUE"),
				d->body_count,
				d->starting_page,
				d->num,
				d->dumprinter,
				d->dumprinter_type,
				d->count_invisible,
				(d->doscopy==TRUE?"TRUE":"FALSE"),
				(d->device_spec==NULL?"":d->device_spec));
			if(!isEMPTY(d->set_lpp))
			{
			if(d->set_lpp_eval==TRUE)
			{
				if(!isEMPTY(d->set_lpp))
				{
					overall_length=0;
					tempapplib=break_expression2(d->set_lpp,&overall_length);
					if(tempapplib!=NULL)
					{
						fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
						fprintf(fp,"\tsprintf(temp1,\"");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							fprintf(fp,"%%s");
						}
						fprintf(fp,"\",\n");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							if(q!=(tempapplib->numlibs-1))
							{
								fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
							} else {
								fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
							}
						}
						freeapplib(tempapplib);
					}
					fprintf(fp,"\ttmp->display->set_lpp=stralloc(temp1);\n");
					fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
				}
			} else {
				fprintf(fp,"\ttmp->display->set_lpp=stralloc(\"%s\");\n",d->set_lpp);
			}
			}
			if(d->num>0)
			{
				fprintf(fp,"\tfree_pvars(pvars,%d);\n",d->num);
			}
		}
	}
	fprintf(fp,"\n");
	fprintf(fp,"/*----------------------------------------------------------------------*/\n");
	fprintf(fp,"\n");

	if(r->select!=NULL)
	{
		fprintf(fp,"%s SELECT RECORDS EXPRESSION %s\n\n",thing1,thing2);
		if(!isEMPTY(r->select))
		{
			overall_length=0;
			tempapplib=break_expression2(r->select,&overall_length);
			if(tempapplib!=NULL)
			{
				fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
				fprintf(fp,"\tsprintf(temp1,\"");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					fprintf(fp,"%%s");
				}
				fprintf(fp,"\",\n");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					if(q!=(tempapplib->numlibs-1))
					{
						fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
					} else {
						fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
					}
				}
				freeapplib(tempapplib);
			}
			fprintf(fp,"\ttmp->select=stralloc(temp1);\n");
			fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		}
	} else {
		fprintf(fp,"\ttmp->select=NULL;\n");
	}
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");

	if(r->search!=NULL)
	{
		fprintf(fp,"%s SEARCH FILES DEFINITIONS %s\n\n",thing1,thing2);
		for(x=0,s=r->search;x<r->numsearchs;++x,++s)
		{
			fprintf(fp,"%s %s %s SEARCH FILE %s\n\n",
					thing1,(s->module!=NULL ? s->module:""),
					(s->filename!=NULL ? s->filename:""),
					thing2);
			overall_length=0;
			tempapplib=break_expression2(s->expression,&overall_length);
			if(tempapplib!=NULL)
			{
				fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
				fprintf(fp,"\tsprintf(temp1,\"");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					fprintf(fp,"%%s");
				}
				fprintf(fp,"\",\n");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					if(q!=(tempapplib->numlibs-1))
					{
						fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
					} else {
						fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
					}
				}
				freeapplib(tempapplib);
			}

			overall_length=0;
			tempapplib=break_expression2(s->skip_detail,&overall_length);
			if(tempapplib!=NULL)
			{
				fprintf(fp,"\ttemp2=Rmalloc(%d+1);\n",overall_length);
				fprintf(fp,"\tsprintf(temp2,\"");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					fprintf(fp,"%%s");
				}
				fprintf(fp,"\",\n");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					if(q!=(tempapplib->numlibs-1))
					{
						fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
					} else {
						fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
					}
				}
				freeapplib(tempapplib);
			}
			overall_length=0;
			tempapplib=break_expression2(s->wc_expression,&overall_length);
			if(tempapplib!=NULL)
			{
				fprintf(fp,"\ttemp3=Rmalloc(%d+1);\n",overall_length);
				fprintf(fp,"\tsprintf(temp3,\"");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					fprintf(fp,"%%s");
				}
				fprintf(fp,"\",\n");
				for(q=0;q<tempapplib->numlibs;++q)
				{
					if(q!=(tempapplib->numlibs-1))
					{
						fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
					} else {
						fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
					}
				}
				freeapplib(tempapplib);
			}

			if(s->cmodule!=NULL)
			{
				fprintf(fp,"\tx=addsearchfileADV2(tmp,\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\",%s,temp1,temp2,%d,%d,temp3);\n",
						(s->module!=NULL?s->module:""),
						(s->filename!=NULL?s->filename:""),
						s->ctype,
						(s->cmodule!=NULL?s->cmodule:""),
						(s->cfilename!=NULL?s->cfilename:""),
						(s->subkeyname!=NULL?s->subkeyname:""),
						(s->range==TRUE?"TRUE":"FALSE"),
						s->num_writes,
						s->wc_eval);
			} else {
				fprintf(fp,"\tx=addsearchfileADV2(tmp,\"%s\",\"%s\",%d,NULL,NULL,\"%s\",%s,temp1,temp2,%d,%d,temp3);\n",
						(s->module!=NULL?s->module:""),
						(s->filename!=NULL?s->filename:""),
						s->ctype,
						(s->subkeyname!=NULL?s->subkeyname:""),
						(s->range==TRUE?"TRUE":"FALSE"),
						s->num_writes,
						s->wc_eval);
			}
			fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
			fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");
			fprintf(fp,"\tif(temp3!=NULL) Rfree(temp3);\n");
			if(s->confld!=NULL)
			{
				fprintf(fp,"\tif(x!=(-1))\n");
				fprintf(fp,"\t{\n");
				fprintf(fp,"\t\ts=tmp->search+(x-1);\n");
				for(y=0,confld=s->confld;y<s->num;++y,++confld)
				{
					fprintf(fp,"\t\taddsearchconfld(s,\"%s\",\"%s\");\n",(confld->fldname!=NULL?confld->fldname:""),(confld->conname!=NULL?confld->conname:""));
				}
				fprintf(fp,"\t}\n");
			}
			if(s->expression!=NULL)
			{
				fprintf(fp,"\tif(t!=NULL) Rfree(t);\n");
			}
			if(s->skip_detail!=NULL)
			{
				fprintf(fp,"\tif(desc!=NULL) Rfree(desc);\n");
			}
			fprintf(fp,"\n\n");
		}
	}
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");

	if(r->virflds!=NULL)
	{
		fprintf(fp,"%s VIRTUAL FIELD DEFINITIONS %s\n\n",thing1,thing2);
		for(x=0,v=r->virflds;x<r->numvirtuals;++x,++v)
		{
			fprintf(fp,"%s %s VIRTUAL FIELD %s\n",
					thing1,(v->name!=NULL ? v->name:""),
					thing2);
			if(v->computed==TRUE)
			{
				fprintf(fp,"\td=getDFvirtual(module,\"%s\");\n",v->name);
				fprintf(fp,"\tif(d!=NULL)\n");
				fprintf(fp,"\t{\n");
				fprintf(fp,"\t\taddvirtuallineADV1(tmp,\"%s\",d->type,d->length,d->expression,%d,%d,%d);\n",(v->name!=NULL ? v->name:""),v->when,v->range,TRUE);
				fprintf(fp,"\t\tFreeDFvirtual(d);\n");
				fprintf(fp,"\t}\n");
			} else {
				overall_length=0;
				tempapplib=break_expression2(v->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

				fprintf(fp,"\taddvirtuallineADV(tmp,\"%s\",%d,%d,temp1,%d,%d);\n",
						(v->name!=NULL?v->name:""),
						v->type,
						v->len,
						v->when,
						v->range);
				fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\tif(t!=NULL) Rfree(t);\n");
			}
			fprintf(fp,"\n\n");
		}
	}
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");

	if(r->sort!=NULL)
	{
		fprintf(fp,"%s SORT DEFINITIONS %s\n\n",thing1,thing2);
		for(x=0,sort=r->sort;x<r->numsorts;++x,++sort)
		{
			fprintf(fp,"\taddsortline(tmp,%d,\"%s\");\n",
					sort->type,
					(sort->name!=NULL?sort->name:""));
		}
	}
	fprintf(fp,"\n\n");
	fprintf(fp,"\tSetReportDefaultEngine(tmp);\n");

	fprintf(fp,"\n\n");


	if(r->accum!=NULL)
	{
		fprintf(fp,"%s ACCUMULATORS %s\n\n",thing1,thing2);
		for(x=0,acc=r->accum;x<r->numaccums;++x,++acc)
		{
			fprintf(fp,"\taddaccumline(tmp,\"%s\",%d,\"%s\",%d);\n",
					(acc->name!=NULL?acc->name:""),
					acc->atype,
					(acc->accname!=NULL?acc->accname:""),
					acc->fieldtype);

		}
	}
	if(r->control!=NULL)
	{
		fprintf(fp,"%s CONTROL BREAKS %s\n\n",thing1,thing2);
		for(x=0,c=r->control;x<r->numcontrols;++x,++c)
		{
			fprintf(fp,"\taddcontrolline(tmp,\"%s\",%d);\n",c->name,c->fieldtype);
		}
	}
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");

	if(r->image!=NULL)
	{
		fprintf(fp,"%s IMAGE LINE DEFINITIONS %s\n\n",thing1,thing2);
		for(x=0,i=r->image;x<r->numimages;++x,++i)
		{
			if(!isEMPTY(i->expression))
			{
				fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
				overall_length=0;
				tempapplib=break_expression2(i->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				fprintf(fp,"\timageoff=addimageline(tmp,\"%s\",\"%s\",TRUE,temp1,0,NULL);\n",
						(i->breakname!=NULL?i->breakname:""),
						(i->line!=NULL?i->line:""));
				fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
			} else {
				fprintf(fp,"\timageoff=addimageline(tmp,\"%s\",\"%s\",FALSE,NULL,0,NULL);\n",
						(i->breakname!=NULL?i->breakname:""),
						(i->line!=NULL?i->line:""));
			}
			if(i->pvals!=NULL)
			{
				fprintf(fp,"\tif(imageoff>0)\n");
				fprintf(fp,"\t{\n");
				fprintf(fp,"\t\timage=tmp->image+(imageoff-1);\n");
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(!isEMPTY(pval->name))
					{
						if((RDAstrlen(pval->name)>60) && pval->symbol_type==1)
						{
							overall_length=0;
							tempapplib=break_expression2(pval->name,&overall_length);
							if(tempapplib!=NULL)
							{
								fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
								fprintf(fp,"\t\tsprintf(temp1,\"");
								for(q=0;q<tempapplib->numlibs;++q)
								{
									fprintf(fp,"%%s");
								}
								fprintf(fp,"\",\n");
								for(q=0;q<tempapplib->numlibs;++q)
								{
									if(q!=(tempapplib->numlibs-1))
									{
										fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
									} else {
										fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
									}
								}
								freeapplib(tempapplib);
							}
						}
					}
					if((RDAstrlen(pval->name)>60) && pval->symbol_type==1)
					{
						fprintf(fp,"\t\taddimagepval(image,%d,temp1,%d,%s%s%s);\n",
								(pval->symbol_type),
								(pval->type),
								(!isEMPTY(pval->fieldname)?"\"":""),
								(!isEMPTY(pval->fieldname)?pval->fieldname:"NULL"),
								(!isEMPTY(pval->fieldname)?"\"":""));
						fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
					} else {
						fprintf(fp,"\t\taddimagepval(image,%d,\"%s\",%d,%s%s%s);\n",
								(pval->symbol_type),
								(!isEMPTY(pval->name)?pval->name:""),
								(pval->type),
								(!isEMPTY(pval->fieldname)?"\"":""),
								(!isEMPTY(pval->fieldname)?pval->fieldname:"NULL"),
								(!isEMPTY(pval->fieldname)?"\"":""));
					}
				}
				fprintf(fp,"\t}\n");
				fprintf(fp,"\n");
			}
			fprintf(fp,"\n");
		}
	}
	if(r->MTNMSTR!=NULL)
	{
		MTNMASTER=r->MTNMSTR;
		fprintf(fp,"%s PROCESS EXCEPTIONS MAINTAIN MTNMASTER %s\n\n",thing1,thing2);
		makemtncode(MTNMASTER,fp,MTNMASTER->name);
		fprintf(fp,"\t}\n");
		fprintf(fp,"\ttmp->MTNMSTR=MTNMSTR;\n");
	} else {
		fprintf(fp,"\ttmp->MTNMSTR=NULL;\n");
	}

	fprintf(fp,"\n");
	fprintf(fp,"\tif(tmp!=NULL)\n");
	fprintf(fp,"\t{\n");
	fprintf(fp,"\t\tif(WRTRDAreport(module,tmp))\n");
	fprintf(fp,"\t\t{\n");

	fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
	fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+100+1);\n",RDAstrlen(r->name),RDAstrlen(module));
	fprintf(fp,"\t\t\tsprintf(temp1,\"REPORT WRITE ERROR: Module [%s] Report [%s], Can Not Save Report!\");\n",(module!=NULL?module:""),(r->name!=NULL?r->name:""));
	fprintf(fp,"\t\t\tprterr(temp1);\n");
	fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
	fprintf(fp,"\t\t\t{\n");
	fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
	fprintf(fp,"\t\t\t}\n");
	fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

	fprintf(fp,"\t\t}\n");
	fprintf(fp,"\t\tFreeRDAreport(tmp);\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"\tif(rptname!=NULL) Rfree(rptname);\n");
	fprintf(fp,"}\n");
}
/* End from mkrptcd */
/* From mkchgcd */
void makechgcode(CHGstruct *CHGDEF,FILE *fp,char *dirname,char *libname,char *chgname)
{
	char *temp=NULL;
	int x=0,y=0,z=0,q=0;
	CHGfile *chgfile=NULL;
	CHGfield *chgfld=NULL;
	RDAsearch *supfil=NULL;
	RDAconfld *confld=NULL;
	char *dirx=NULL,*libx=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;

	if(fp!=NULL)
	{
		dirx=Rmalloc(RDAstrlen(dirname)+5);
#ifndef WIN32
		sprintf(dirx,"%s/",dirname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\",dirname);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
#ifndef WIN32
		sprintf(libx,"%s/%s.CHG",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.CHG",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
#endif
		fprintf(fp,"/* %s's - Make (%s) Change File */\n",CHGDEF->module,CHGDEF->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mkchg.hpp>\n");
                fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
                fprintf(fp,"extern char *module;\n");
                fprintf(fp,"extern APPlib *errorlist;\n");
		temp=fixname(CHGDEF->name);
		fprintf(fp,"\n");
		fprintf(fp,"static void CHG_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tint x2=0,x3=0;\n");
		fprintf(fp,"\tCHGstruct *CHGDEF=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\tCHGfile *chgfile=NULL;\n");
		fprintf(fp,"\tRDAsearch *supfil=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL,*temp2=NULL;\n");
		fprintf(fp,"\n");



		overall_length=0;
		tempapplib=break_expression2(CHGDEF->desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		overall_length=0;
		tempapplib=break_expression2(CHGDEF->expression,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp2=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp2,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		fprintf(fp,"\tCHGDEF=RDACHGstructNEW(module,\"%s\",temp1,\"%s\",%d,%d,%s,\"%s\",temp2,0,NULL);\n",
			chgname,
			(CHGDEF->fieldname!=NULL ? CHGDEF->fieldname:""),
			CHGDEF->type,CHGDEF->len,
			(CHGDEF->range==TRUE ? "TRUE":"FALSE"),
			(CHGDEF->range_screen!=NULL ? CHGDEF->range_screen:""));
		fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");

		fprintf(fp,"\tif(CHGDEF!=NULL)\n");
		fprintf(fp,"\t{\n");
		for(x=0,chgfile=CHGDEF->chgfiles;x<CHGDEF->numfiles;++x,++chgfile)
		{	
			if(chgfile!=NULL)
			{
				overall_length=0;
				tempapplib=break_expression2(chgfile->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

				fprintf(fp,"\t\tx2=addChangeFile(CHGDEF,\"%s\",\"%s\",\"%s\",temp1,0,NULL,0,NULL);\n",
					(chgfile->module!=NULL ? chgfile->module: ""),
					(chgfile->filename!=NULL ? chgfile->filename: ""),
					(chgfile->keyname!=NULL ? chgfile->keyname: ""));
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				if(chgfile->chgflds!=NULL && chgfile->numflds>0)
				{
					fprintf(fp,"\t\tif(x2!=(-1))\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tchgfile=CHGDEF->chgfiles+(x2-1);\n");
					for(y=0,chgfld=chgfile->chgflds;y<chgfile->numflds;++y,++chgfld)
					{	
						fprintf(fp,"\t\t\tx3=addChangeField(chgfile,%d,\"%s\");\n",
							chgfld->keypart,
							(chgfld->fieldname!=NULL ? chgfld->fieldname: ""));
					}
					fprintf(fp,"\t\t}\n");
				}
				fprintf(fp,"\n");
				if(chgfile->supfil!=NULL && chgfile->num>0)
				{
					fprintf(fp,"\t\tif(x2!=(-1))\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tchgfile=CHGDEF->chgfiles+(x2-1);\n");
					for(y=0,supfil=chgfile->supfil;y<chgfile->num;++y,++supfil)
					{	
						tempapplib=break_expression2(supfil->expression,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}
						if(!isEMPTY(supfil->browse_screen)) stoupper(supfil->browse_screen);
						if(!isEMPTY(supfil->define_list)) stoupper(supfil->define_list);
						if(!isEMPTY(supfil->search_browse)) stoupper(supfil->search_browse);
						fprintf(fp,"\t\t\tx3=addChangeFileSupporting(chgfile,\"%s\",\"%s\",%d,0,NULL,\"%s\",\"%s\",\"%s\",%d,temp1,%d,\"%s\",\"%s\",\"%s\",\"%s\");\n",
							(supfil->module!=NULL ? supfil->module: ""),
							(supfil->filename!=NULL ? supfil->filename: ""),
							supfil->ctype,
							(supfil->cmodule!=NULL ? supfil->cmodule: ""),
							(supfil->cfilename!=NULL ? supfil->cfilename: ""),
							(supfil->subkeyname!=NULL ? supfil->subkeyname: ""),
							supfil->range,
							supfil->browse,
							(supfil->button_name!=NULL ? supfil->button_name: ""),
							(supfil->browse_screen!=NULL ? supfil->browse_screen: ""),
							(supfil->define_list!=NULL ? supfil->define_list: ""),
							(supfil->search_browse!=NULL ? supfil->search_browse: ""));
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
						if(supfil->confld!=NULL && supfil->num>0)
						{
							fprintf(fp,"\t\t\tif(x3!=(-1))\n");
							fprintf(fp,"\t\t\t{\n");
							fprintf(fp,"\t\t\t\tsupfil=chgfile->supfil+(x3-1);\n");
							for(z=0,confld=supfil->confld;z<supfil->num;++z,++confld)
							{	
								overall_length=0;
								tempapplib=break_expression2(confld->conname,&overall_length);
								if(tempapplib!=NULL)
								{
									fprintf(fp,"\t\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
									fprintf(fp,"\t\t\t\tsprintf(temp1,\"");
									for(q=0;q<tempapplib->numlibs;++q)
									{
										fprintf(fp,"%%s");
									}
									fprintf(fp,"\",\n");
									for(q=0;q<tempapplib->numlibs;++q)
									{
										if(q!=(tempapplib->numlibs-1))
										{
											fprintf(fp,"\t\t\t\t\t%s,\n",tempapplib->libs[q]);
										} else {
											fprintf(fp,"\t\t\t\t\t%s);\n",tempapplib->libs[q]);
										}
									}
									freeapplib(tempapplib);
								}
		
								fprintf(fp,"\t\t\t\taddChangeFileSupportingRDAconfld(supfil,\"%s\",temp1,%d);\n",
									(confld->fldname!=NULL ? confld->fldname: ""),
									confld->inherit);
								fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
							}
							fprintf(fp,"\t\t\t}\n");
						}
					}
					fprintf(fp,"\t\t}\n");
				}
			}
		}
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\t\tdefdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s/rda/%%s.CHG\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\t\tsprintf(defdir,\"%%s\\\\rda\\\\%%s.CHG\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\t\tif(writeCHGstruct(defdir,CHGDEF))\n");
		fprintf(fp,"\t\t{\n");
		fprintf(fp,"/*\n");
		fprintf(fp,"\t\t\tERRORDIALOG(\"CANNOT SAVE CHANGE FILE\",\"This Change Database File definition may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.\",NULL,FALSE);\n");
		fprintf(fp,"*/\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+89+1);\n",RDAstrlen(CHGDEF->name),RDAstrlen(CHGDEF->module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"CHANGE DATABASE FILE WRITE ERROR: Module [%s] Change File Definition [%s], Can Not Save Change Database File!\");\n",(CHGDEF->module!=NULL?CHGDEF->module:""),(CHGDEF->name!=NULL?CHGDEF->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(defdir!=NULL) Rfree(defdir);\n");
		fprintf(fp,"\t\tif(CHGDEF!=NULL) FreeCHGstruct(CHGDEF);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

		if(libx!=NULL) Rfree(libx);
		if(dirx!=NULL) Rfree(dirx);
	}
}
/* End from mkchgcd */
/* From mkimpcd */
void makeIMPEXPcode(IEfile *IMPEXP,FILE *fp,char *libname,char *name)
{
	int x=0,q=0;
	IEfield *field=NULL;
	char *temp=NULL;
	char *tempdos=NULL,*tempunix=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"/* %s's - Make (%s) Import/Export */\n",IMPEXP->module,IMPEXP->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"#include <mkimp.hpp>\n");
/*
		fprintf(fp,"#include <impexp.hpp>\n");
*/
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		temp=fixname(IMPEXP->name);
/*
		tempdos=fixdataname(IMPEXP->ifile,'/','\\');
		tempunix=fixdataname(IMPEXP->ifile,'\\','/');
*/
		tempdos=fixdosname(IMPEXP->ifile);
		tempunix=fixunixname(IMPEXP->ifile);
		fprintf(fp,"\n");
		fprintf(fp,"static void IMPEXP_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tIEfile *IMPEXP=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
		fprintf(fp,"\tchar *temp2=NULL;\n");
		fprintf(fp,"\n");

		overall_length=0;
		tempapplib=break_expression2(IMPEXP->desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		fprintf(fp,"\n");

		overall_length=0;
		tempapplib=break_expression2(IMPEXP->expression,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp2=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp2,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		fprintf(fp,"\n");


		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\tIMPEXP=IMPEXPdefNEW(\"%s\",\"%s\",\"%s\",\"%s\",temp1,\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,temp2,0,NULL);\n",
			(name!=NULL ? name:""),
			(libname!=NULL ? libname:""),
			(IMPEXP->file!=NULL ? IMPEXP->file:""),
			tempdos,
			(IMPEXP->fdel!=NULL ? IMPEXP->fdel:""),
			(IMPEXP->rdel!=NULL ? IMPEXP->rdel:""),
			IMPEXP->which,
			IMPEXP->type,
			IMPEXP->force_quotes,
			IMPEXP->carriage_return,
			IMPEXP->line_feed,
			IMPEXP->skip_dups,
			IMPEXP->range,
			IMPEXP->ask_b4_execute);
		fprintf(fp,"#endif /* ifdef WIN32 */\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\tIMPEXP=IMPEXPdefNEW(\"%s\",\"%s\",\"%s\",\"%s\",temp1,\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,temp2,0,NULL);\n",
			(name!=NULL ? name:""),
			(libname!=NULL ? libname:""),
			(IMPEXP->file!=NULL ? IMPEXP->file:""),
			tempunix,
			(IMPEXP->fdel!=NULL ? IMPEXP->fdel:""),
			(IMPEXP->rdel!=NULL ? IMPEXP->rdel:""),
			IMPEXP->which,
			IMPEXP->type,
			IMPEXP->force_quotes,
			IMPEXP->carriage_return,
			IMPEXP->line_feed,
			IMPEXP->skip_dups,
			IMPEXP->range,
			IMPEXP->ask_b4_execute);
		fprintf(fp,"#endif /* ifndef WIN32 */\n");
		fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");
		fprintf(fp,"\tif(IMPEXP!=NULL)\n");
		fprintf(fp,"\t{\n");
		if(IMPEXP->fields!=NULL)
		{
			for(x=0,field=IMPEXP->fields;x<IMPEXP->num;++x,++field)
			{
				if(!isEMPTY(field->expression) || !isEMPTY(field->value_expression))
				{
					overall_length=0;
					tempapplib=break_expression2(field->expression,&overall_length);
					if(tempapplib!=NULL)
					{
						fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
						fprintf(fp,"\t\tsprintf(temp1,\"");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							fprintf(fp,"%%s");
						}
						fprintf(fp,"\",\n");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							if(q!=(tempapplib->numlibs-1))
							{
								fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
							} else {
								fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
							}
						}
						freeapplib(tempapplib);
					}
					overall_length=0;
					tempapplib=break_expression2(field->value_expression,&overall_length);
					if(tempapplib!=NULL)
					{
						fprintf(fp,"\t\ttemp2=Rmalloc(%d+1);\n",overall_length);
						fprintf(fp,"\t\tsprintf(temp2,\"");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							fprintf(fp,"%%s");
						}
						fprintf(fp,"\",\n");
						for(q=0;q<tempapplib->numlibs;++q)
						{
							if(q!=(tempapplib->numlibs-1))
							{
								fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
							} else {
								fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
							}
						}
						freeapplib(tempapplib);
					}
					fprintf(fp,"\t\taddIMPEXPfieldADV(IMPEXP,\"%s\",%d,%d,%d,temp1,%d,temp2);\n",
						(field->name!=NULL ? field->name:""),
						field->type,
						field->start,
						field->length,
						(field->use_quotes));
					fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
					fprintf(fp,"\t\tif(temp2!=NULL) Rfree(temp2);\n");
					fprintf(fp,"\n");
				} else {
					fprintf(fp,"\t\taddIMPEXPfieldADV(IMPEXP,\"%s\",%d,%d,%d,NULL,%d,NULL);\n",
						(field->name!=NULL ? field->name:""),
						field->type,
						field->start,
						field->length,
						field->use_quotes);
				}
			}
		}
		fprintf(fp,"\t\tif(writeIMPORTbin(module,IMPEXP))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+110+1);\n",RDAstrlen(IMPEXP->name),RDAstrlen(libname));
		fprintf(fp,"\t\t\tsprintf(temp1,\"IMPORT/EXPORT WRITE ERROR: Module [%s] Import/Export definition [%s], Can Not Save Import/Export Definition!\");\n",(libname!=NULL?libname:""),(IMPEXP->name!=NULL?IMPEXP->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\tif(temp2!=NULL) Rfree(temp2);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(IMPEXP!=NULL) free_import(IMPEXP);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");
	}
}
/* End from mkimpcd */
