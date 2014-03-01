/* vircode.c - Generate Virtual Field C Code for speed of development */

static char *fixVIRname(char *name)
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
		new_name=stralloc("makeVIRfunc");
	}
	return(new_name);
}
void makeVIRcode(DFvirtual *virtual,FILE *fp,char *libname,char *name)
{
	int x=0,q=0;
	DFincvir *i=NULL;
	char *temp=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"/* %s's - Make (%s) Virtual */\n",virtual->module,virtual->name);
		fprintf(fp,"#include <stdio.h>\n");
		fprintf(fp,"#include <mix.h>\n");
		fprintf(fp,"#include <mkmsc.h>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		temp=fixVIRname(virtual->name);
		fprintf(fp,"static void VIR_%s(void);\n",temp);
		fprintf(fp,"\n");
		fprintf(fp,"static void VIR_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tDFvirtual *virtual=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");

		fprintf(fp,"\n");

		fprintf(fp,"\tvirtual=DFvirtualNEW(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(name!=NULL ? name:""));
		fprintf(fp,"\tif(virtual!=NULL)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tvirtual->vtype=%d;\n",virtual->vtype);
		fprintf(fp,"\t\tvirtual->type=%d;\n",virtual->type);
		if(virtual->incvir!=NULL)
		{
			for(x=0,i=virtual->incvir;x<virtual->num;++x,++i)
			{
				fprintf(fp,"\t\tDFvirtualDFincvir(virtual,\"%s\",\"%s\",\"%s\",%d);\n",(i->module!=NULL ? i->module:""),
					(i->file!=NULL ? i->file:""),	
					(i->keyname!=NULL ? i->keyname:""),
					i->fileno);
			}
		}
		fprintf(fp,"\t\tvirtual->avail2rsrc=%d;\n",(virtual->avail2rsrc ? TRUE:FALSE));
		fprintf(fp,"\t\tvirtual->type=%d;\n",virtual->type);
		fprintf(fp,"\t\tvirtual->length=%d;\n",virtual->length);
		fprintf(fp,"\t\tvirtual->when=%d;\n",virtual->when);

		overall_length=0;
		tempapplib=break_expression2(virtual->expression,&overall_length);
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

		fprintf(fp,"\t\tvirtual->expression=stralloc(temp1);\n");
		fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\tvirtual->desc=stralloc(\"%s\");\n",(virtual->desc!=NULL ? virtual->desc:""));

		fprintf(fp,"\n");
		fprintf(fp,"\t\tif(writeDFvirtual(module,virtual))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+110+1);\n",RDAstrlen(virtual->name),RDAstrlen(module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"VIRTUAL FIELD WRITE ERROR: Module [%s] Virtual Field [%s], Can Not Save Virtual Field!\");\n",(module!=NULL?module:""),(virtual->name!=NULL?virtual->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(virtual!=NULL) FreeDFvirtual(virtual);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");
	}
}
