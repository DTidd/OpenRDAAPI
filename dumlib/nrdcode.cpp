/* nrdcode.c - Generate Database C Code for speed of development */
#ifndef WIN32
#define __NAM__ "nrdcode.lnx"
#endif
#ifdef WIN32
#define __NAM__ "nrdcode.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <nrdp1.hpp>

void makeDBcode(NRDbuffer *NRDtmp,FILE *fp,char *libname,char *filename)
{
	int x=0,y=0;
	NRDfield *f=NULL;
	NRDkey *k=NULL;
	NRDpart *p=NULL;	
	char *temp=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"/* %s's - Make (%s) Database File */\n",NRDtmp->modname,NRDtmp->name);
		fprintf(fp,"#include <stdio.h>\n");
		fprintf(fp,"#include <mix.h>\n");
		fprintf(fp,"#include <mknrd.h>\n");
		fprintf(fp,"#include <mkmsc.h>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		fprintf(fp,"\n");
		temp=adddashes(NRDtmp->name);
		fprintf(fp,"void DB_%s(void);\n",temp);
		fprintf(fp,"\n");
		fprintf(fp,"void DB_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tDBsort *sort=NULL;\n");
		fprintf(fp,"\tshort x=(-1);\n");
		fprintf(fp,"\tchar same=FALSE;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
/*
		fprintf(fp,"\tchar *tempstr=NULL;\n");
*/
		fprintf(fp,"\n");

		fprintf(fp,"\tsort=DBsortNEW(\"%s\",\"%s\",0);\n",(libname!=NULL ? libname:""),(filename!=NULL?filename:""));
		fprintf(fp,"\tif(sort!=NULL)\n");
		fprintf(fp,"\t{\n");
		if(NRDtmp->field!=NULL)
		{
			for(x=0,f=NRDtmp->field;x<NRDtmp->nofields;++x,++f)
			{
				fprintf(fp,"\t\taddDBfield(sort,\"%s\",%d,%d);\n",(f->name!=NULL ? f->name:""),f->type,f->len);
			}
		}
		if(NRDtmp->key!=NULL)
		{
			for(x=0,k=NRDtmp->key;x<NRDtmp->nokeys;++x,++k)
			{
				fprintf(fp,"\t\taddDBkey(sort,\"%s\");\n",(k->name!=NULL ? k->name:""));
				if(k->part!=NULL)
				{
					for(y=0,p=k->part;y<k->numparts;++y,++p)
					{
						fprintf(fp,"\t\taddDBkeypart(sort,%d,\"%s\");\n",(x+1),(p->name!=NULL ? p->name:""));
					}
				}
			}
		}
		fprintf(fp,"\n");
		fprintf(fp,"\t\tsame=CompareDATABASE(sort);\n");
		fprintf(fp,"\t\tif(same==FALSE)\n");
		fprintf(fp,"\t\t{\n");
		fprintf(fp,"\t\t\tMergeDATABASE(sort);\n");
		fprintf(fp,"\t\t\tSetupFileDefinition(sort);\n");
		fprintf(fp,"\t\t\tx=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);\n");
		fprintf(fp,"\t\t\tif(x==(-1))\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\t\ttemp1=Rmalloc(%d+%d+77+1);\n",RDAstrlen(NRDtmp->name),RDAstrlen(NRDtmp->modname));
		fprintf(fp,"\t\t\t\tsprintf(temp1,\"DATABASE FILE WRITE ERROR: Module [%s] Screen [%s], Can Not Save Database File!\");\n",(NRDtmp->modname!=NULL?NRDtmp->modname:""),(NRDtmp->name!=NULL?NRDtmp->name:""));
		fprintf(fp,"\t\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t\t{\n");
		fprintf(fp,"\t\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t\t}\n");
		fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

/*
		fprintf(fp,"\t\t\t\ttempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));\n");
		fprintf(fp,"\t\t\t\tsprintf(tempstr,\"The Database File Changes may not be saved to the specified library: [%%s][%%s].  Check your permissions and re-try.  Call your installer for further instructions.\",sort->module,sort->filename);\n");
		fprintf(fp,"\t\t\t\tprterr(\"ERROR: Cannot Save Database File, [%%s]\",tempstr);\n");
		fprintf(fp,"\t\t\t\tERRORDIALOG(\"Cannot Save Database File\",tempstr,NULL,FALSE);\n");
		fprintf(fp,"\t\t\t\tif(tempstr!=NULL) Rfree(tempstr);\n");
*/

		fprintf(fp,"\t\t\t} else {\n");
		fprintf(fp,"\t\t\t\tCLSNRD(x);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(sort!=NULL) ADVFreeDBsort(sort,FALSE);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");

	}

}
