/* mnucode.c - Generate Menu C Code for speed of development */

static char *fixMNUname(char *name)
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
		new_name=stralloc("makeMNUfunc");
	}
	return(new_name);
}
void makeMNUcode(RDAmenu *menu,FILE *fp,char *dirname,char *libname,char *name)
{
	char *temp=NULL,*temp1=NULL;
	char *dirx=NULL,*libx=NULL;

	if(fp!=NULL)
	{	
		dirx=Rmalloc(RDAstrlen(dirname)+5);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+10);
		sprintf(libx,"%s/%s.MNU",(dirx!=NULL ? dirx:""),(libname!=NULL ? libname:""));
		fprintf(fp,"#include <stdio.h>\n");
		fprintf(fp,"#include <mix.h>\n");
		fprintf(fp,"\n");
		fprintf(fp,"char *module=\"%s\";\n",libname);
		temp=fixMNUname(name);
		fprintf(fp,"static void MNU_%s(void);\n",temp);
		fprintf(fp,"\n");
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
