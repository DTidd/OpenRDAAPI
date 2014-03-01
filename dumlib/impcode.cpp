/* impcode.c - Generate Import/Export C Code for speed of development */


static char *fixIMPEXPname(char *name)
{
	int str_length=0,x;
	char *new_name=NULL;

	if(!isEMPTY(name))
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
		new_name=stralloc("makeimpexpfunc");
	}
	return(new_name);
}
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
		fprintf(fp,"#include <stdio.h>\n");
		fprintf(fp,"#include <mix.h>\n");
		fprintf(fp,"#include <mkmsc.h>\n");
		fprintf(fp,"#include <mkimp.h>\n");
/*
		fprintf(fp,"#include <impexp.h>\n");
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
		fprintf(fp,"static void IMPEXP_%s(void);\n",temp);
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
					fprintf(fp,"\t\taddIMPEXPfieldADV(IMPEXP,\"%s\",%d,%d,%d,temp1,%s,temp2);\n",
						(field->name!=NULL ? field->name:""),
						field->type,
						field->start,
						field->length,
						(field->use_quotes==FALSE?"FALSE":"TRUE"));
					fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
					fprintf(fp,"\t\tif(temp2!=NULL) Rfree(temp2);\n");
					fprintf(fp,"\n");
				} else {
					fprintf(fp,"\t\taddIMPEXPfieldADV(IMPEXP,\"%s\",%d,%d,%d,NULL,%s);\n",
						(field->name!=NULL ? field->name:""),
						field->type,
						field->start,
						field->length,
						(field->use_quotes==FALSE?"FALSE":"TRUE"));
				}
			}
		}
		fprintf(fp,"\t\tif(writeIMPORTbin(module,IMPEXP))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+110+1);\n",RDAstrlen(IMPEXP->name),RDAstrlen(module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"IMPORT/EXPORT WRITE ERROR: Module [%s] Import/Export definition [%s], Can Not Save Import/Export Definition!\");\n",(module!=NULL?module:""),(IMPEXP->name!=NULL?IMPEXP->name:""));
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
