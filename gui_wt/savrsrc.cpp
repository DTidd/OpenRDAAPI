/* savrsrc.c - Save Resource Functions */
#include<gui.hpp>
#ifndef WIN32
#include<unistd.h>
#endif
#ifdef WIN32
#include<io.h>
#include<fcntl.h>
#include<stdio.h>
#endif
#include<guip.hpp>

void SAVE_RSRC_BINARY(RDArsrc *rsrc)
{
	int x=0,fd=0,y=0;
	RDArmem *member=NULL;
	BIN *bin;
	char *fname=NULL,*fname1=NULL;
	short test=0;

	fname=Rmalloc(RDAstrlen(rsrc->module)+10+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(rsrc->module);
#ifndef WIN32
	sprintf(fname,"%s/rda/%s.RSC",CURRENTDIRECTORY,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\rda\\%s.RSC",CURRENTDIRECTORY,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd!=(-1))
	{
		if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
		if(access(fname,02))
		{
			prterr("Error in permissions, user can't write report file [%s].",fname);
			if(fname!=NULL) Rfree(fname);
			return;
		}
	}
	bin=BINnew();
	BINaddshort(bin,-1700);	/*type identifier*/
	if(!isEMPTY(rsrc->screen))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,rsrc->screen);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,rsrc->numrscs);
	for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
	{
		if(!isEMPTY(member->rscrname))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,member->rscrname);
		} else BINaddshort(bin,FALSE);
		BINaddshort(bin,member->field_type);
		switch(member->field_type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case PLAINTEXT:
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(!isEMPTY(member->value.string_value))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,member->value.string_value);
				} else BINaddshort(bin,FALSE);
				break;
			case BOOLNS:
				test=*member->value.string_value;
				BINaddshort(bin,test);
				break;
			case CHARACTERS:
				test=*member->value.string_value;
				BINaddshort(bin,test);
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
				BINadddouble(bin,*member->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				BINaddshort(bin,*member->value.short_value);
				break;
			case SCROLLEDLIST:
				BINaddint(bin,*member->value.integer_value);
				BINaddint(bin,member->items);
				for(y=0;y<member->items;++y)
				{
					if(!isEMPTY(*(member->list[0]+y)))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,*(member->list[0]+y));
					} else BINaddshort(bin,FALSE);
				}
				break;
			case LONGV:
			case SLONGV:
				BINaddint(bin,*member->value.integer_value);
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s].",member->field_type,member->rscrname,rsrc->module,rsrc->screen);
				break;
		}
	}
	if(writelibbin(fname,bin,rsrc->screen))
	{
		prterr("Error Can't write Resource binary [%s] into library [%s].",(rsrc->screen!=NULL ? rsrc->screen:""),fname);
	}
	if(fname!=NULL) Rfree(fname);
}
