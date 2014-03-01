#include <cstdio>
#include <mix.hpp>

short xisfilersrc(short fileno,char *filename,char *rscrname,short type)
{
	char *fieldname=NULL;

	if(!type)
	{
		fieldname=gfname(filename,rscrname);
		if(fieldname!=NULL)
		{
			if(FLDNRD(fileno,fieldname)==NULL)
			{
				Rfree(fieldname);
				return(-1);
			} else {
				Rfree(fieldname);
			}
		} else return(-1);
	} else {
		if(FLDNRD(fileno,rscrname)==NULL) return(-1);
	}
	return(0);
}
/*-----------------------------------------------------------------------------
	realfilewidgets
		Function to read all widgets corresponding to file 
		record fields.
-----------------------------------------------------------------------------*/
void xreadfilewidgets(short fileno,RDArsrc *rsrc,short type,int line,char *file)
{
	short x;
	RDArmem *member;
	char *filename=NULL;

	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG readfilewidgets Reading resource values on Screen [%s] [%s] corresponding to fields in module [%s] file [%d] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,MODULENAME(fileno),fileno,filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	if(filename!=NULL)
	{
		for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
		{
			if(xisfilersrc(fileno,filename,member->rscrname,type)!=(-1))
			{
				if(member->w!=NULL) readwidget(rsrc,member->rscrname);
			}
		}
	}
}
/*-----------------------------------------------------------------------------
	updateeditfilersrcs
		Function to update and set editable values for all 
		widgets corresponding to file record fields.
-----------------------------------------------------------------------------*/
void xupdateeditfilersrcs(short fileno,short keyno,RDArsrc *rsrc,short type,short seteditable,short editable,short dokeys,int line,char *file)
{
	short x;
	RDArmem *member;
	char *filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	int y;
	NRDkey *key;
	NRDpart *part;

	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG updateeditfilersrcs Updating resource values to Screen [%s] [%s] corresponding to fields in module [%s] file [%d] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,MODULENAME(fileno),fileno,filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	key=KEYNUM(fileno,keyno);
	for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
	{
		if(!type)
		{
			fieldname=gfname(filename,member->rscrname);
			if(fieldname!=NULL)
			{
				field=FLDNRD(fileno,fieldname);
				Rfree(fieldname);
			} else field=NULL;
		} else {
			field=FLDNRD(fileno,member->rscrname);
		}
		if(field!=NULL)
		{
			updatemember(member);
			if(!dokeys && seteditable)
			{
				for(y=0,part=key->part;y<key->numparts;++y,++part)
				{
					if(!RDAstrcmp(part->name,field->name)) break;
				}
				if(y>=key->numparts)
				{
					MEMBERSETEDITABLE(member,editable);
				}
			} else {
				if(seteditable)
					MEMBERSETEDITABLE(member,editable);
			}
		}
	}
}
void xupdatefilersrc(short fileno,RDArsrc *rsrc,short type,int line,char *file)
{
	short x;
	RDArmem *member;
	char *filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;

	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG updatefilersrc Updating resource values to Screen [%s] [%s] corresponding to fields in module [%s] file [%d] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,MODULENAME(fileno),fileno,filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
	{
		if(!type)
		{
			fieldname=gfname(filename,member->rscrname);
			if(fieldname!=NULL)
			{
				field=FLDNRD(fileno,fieldname);
				Rfree(fieldname);
			} else field=NULL;
		} else {
			field=FLDNRD(fileno,member->rscrname);
		}
		if(field!=NULL) updatemember(member);
	}
}
void xseteditfilersrc(short fileno,short keyno,RDArsrc *rsrc,short type,
	short editable,short dokeys,int line,char *file)
{
	short x;
	RDArmem *member;
	char *filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	int y;
	NRDkey *key;
	NRDpart *part;

	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG seteditfilersrcs Updating resource values to Screen [%s] [%s] corresponding to fields in module [%s] file [%d] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,MODULENAME(fileno),fileno,filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	key=KEYNUM(fileno,keyno);
	for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
	{
		if(!type)
		{
			fieldname=gfname(filename,member->rscrname);
			if(fieldname!=NULL)
			{
				field=FLDNRD(fileno,fieldname);
				Rfree(fieldname);
			} else field=NULL;
		} else {
			field=FLDNRD(fileno,member->rscrname);
		}
		if(field!=NULL)
		{
			if(!dokeys)
			{
				for(y=0,part=key->part;y<key->numparts;++y,++part)
				{
					if(!RDAstrcmp(part->name,field->name)) break;
				}
				if(y>=key->numparts)
				{
					MEMBERSETEDITABLE(member,editable);
				}
			} else {
				MEMBERSETEDITABLE(member,editable);
			}
		}
	}
}
