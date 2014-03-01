#ifndef WIN32
#define __NAM__ "trvirt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "trvirt.exe"
#endif
#include <app.hpp>




#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *modulename="RPTGEN";

int main(int argc,char **argv)
{
	int x=0;
	char *filename=NULL,*tmpx=NULL,*libx=NULL;
	DFvirtual *d=NULL;
	APPlib *virtnames=NULL;
/*
	DFincvir *i;
	int y=0;
*/

	if(InitializeSubsystems(argc,argv,modulename,"SHOW VIRTUAL FIELDS")) 
	{
		return;
	}
	if(argc>1)
	{
		filename=stralloc(argv[1]);
	} else filename=stralloc("FINMGT");
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(filename)+10);
	tmpx=adddashes(filename);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
	virtnames=getlibnames(libx,FALSE);
	if(virtnames!=NULL)
	{
		fprintf(stdout,"VIRTUAL FIELDS AVAILABLE in %s Total: [%d]\n",libx,virtnames->numlibs);
		for(x=0;x<virtnames->numlibs;++x)
		{
			d=getDFvirtual(filename,virtnames->libs[x]);
			if(d!=NULL)
			{
				fprintf(stdout,"Name: %s Type: [%d] Screen Widget: [%d]\n",d->name,d->vtype,d->avail2rsrc);
/*
				for(y=0,i=d->incvir;y<d->num;++y,++i)
				{
					fprintf(stdout,"\tModule [%s] File [%s] Keyname [%s] Fileno [%d]\n",(i->module!=NULL ? i->module:""),(i->file!=NULL ? i->file:""),(i->keyname!=NULL ? i->keyname:""),i->fileno);
				}
				fprintf(stdout,"\tWhen [%d] Type [%d] [%s] Length [%d]\n",d->when,d->type,standardfieldtypes[d->type],d->length);
				fprintf(stdout,"\tExpression [%s]\n",(d->expression!=NULL ?
					d->expression:""));
*/
				fprintf(stdout,"\tDescription [%s]\n\n",(d->desc!=NULL ? d->desc:""));
				FreeDFvirtual(d);
				d=NULL;
			}
		}
		freeapplib(virtnames);
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);

}
