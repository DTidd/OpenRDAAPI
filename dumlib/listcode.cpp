/* listcode.c - Generate Scrolled List C Code for speed of development */


static char *fixLSTname(char *name)
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
			} else {
				new_name[x]=name[x];
			}
		}
	} else {
		new_name=stralloc("makeLSTfunc");
	}
	return(new_name);
}
void makeLSTcode(RDAScrolledList *list,FILE *fp,char *libname,char *name)
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
		fprintf(fp,"/* %s's - Make (%s) List */\n",list->module,list->name);
		fprintf(fp,"#include <stdio.h>\n");
		fprintf(fp,"#include <mix.h>\n");
		fprintf(fp,"#include <mkmsc.h>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		temp=fixLSTname(list->name);
		fprintf(fp,"static void LST_%s(void);\n",temp);
		fprintf(fp,"\n");
		fprintf(fp,"static void LST_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDAScrolledList *list=NULL;\n");
		fprintf(fp,"\tchar *defdir=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
		fprintf(fp,"\n");

		fprintf(fp,"\tlist=RDAScrolledListNew(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(name!=NULL ? name:""));
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
