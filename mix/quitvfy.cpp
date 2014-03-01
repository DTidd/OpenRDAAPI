/* quitvfy.c - Quit Verification Function used with New File Locking */
#include<mix.hpp>

short xQUITvfy(short fileno,RDATData *previous,int line,char *file)
{
	int x,numflds=0;
	NRDfield *f,*p,*fields=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG QUITvfy for Fileno [%d] [%s] [%s] at line [%d] program [%s].",fileno,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(previous==NULL) return(FALSE);
	else if(previous->field==NULL) return(FALSE);
	numflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		if(previous!=NULL)
		{
			if(previous->field!=NULL)
			{
				for(x=0,f=fields,p=previous->field;x<numflds;++x,++f,++p)
				{
					if(COMPARENRDfield(f,p)) 
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagmix || diagmix_field)
						{
							prterr("DIAG QUITvfy Finding Differences Between the Current and Previous Record's Value at line [%d] program [%s].",line,file);
							SEENRDFIELD(f);
							SEENRDFIELD(p);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						return(TRUE);
					}
				}
			}
		}
	} 
	return(FALSE);
}
char *xQUITvfydesc(short fileno,RDATData *previous,int line,char *file)
{
	char *tmp=NULL,*modx=NULL,*filx=NULL,*temp1=NULL,*temp2=NULL;
	int x,numflds=0,count=0,length=0;
	NRDfield *f,*p,*fields=NULL;

	modx=MODULENAME(fileno);
	filx=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG QUITvfydesc for Fileno [%d] [%s] [%s] at line [%d] program [%s].",fileno,modx,filx,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	count=RDAstrlen(modx)+RDAstrlen(filx)+44;
	tmp=Rmalloc(count+1);
	sprintf(tmp,"The following have been made to File [%s][%s]:\n\n",
		MODULENAME(fileno),FILENAME(fileno)); 
	numflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		if(previous!=NULL)
		{
			if(previous->field)
			{
			for(x=0,f=fields,p=previous->field;x<numflds;++x,++f,++p)
			{
				if(COMPARENRDfield(f,p)) 
				{
					switch(f->type)
					{
						case SCROLLEDTEXT:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							length=RDAstrlen(f->name)+RDAstrlen(standardfieldtypes[f->type])+(f->data.string_value!=NULL ? RDAstrlen(f->data.string_value):0)+(p->data.string_value!=NULL ? RDAstrlen(p->data.string_value):0)+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%s]\n\tResulting Value: [%s]\n",f->name,standardfieldtypes[f->type],f->len,(p->data.string_value!=NULL ? p->data.string_value:""),(f->data.string_value!=NULL ? f->data.string_value:""));
							break;
						case CHARACTERS:
							length=RDAstrlen(f->name)+RDAstrlen(standardfieldtypes[f->type])+71;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%c]\n\tResulting Value: [%c]\n",f->name,standardfieldtypes[f->type],f->len,*p->data.string_value,*f->data.string_value);
							break;
						case BOOLNS:
							if(*f->data.string_value) *f->data.string_value=TRUE;
							if(*p->data.string_value) *p->data.string_value=TRUE;
							length=RDAstrlen(f->name)+RDAstrlen(standardfieldtypes[f->type])+RDAstrlen(bolstring[*f->data.string_value])+RDAstrlen(bolstring[*p->data.string_value])+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%s]\n\tResulting Value: [%s]\n",f->name,standardfieldtypes[f->type],f->len,bolstring[*p->data.string_value],bolstring[*f->data.string_value]);
							break;
						case DOLLARS_NOCENTS:
							temp1=famtncents(*f->data.float_value*100,f->len,' ');
							temp2=famtncents(*p->data.float_value*100,p->len,' ');
							length=RDAstrlen(f->name)+RDAstrlen(temp1)+RDAstrlen(temp2)+RDAstrlen(standardfieldtypes[f->type])+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%s]\n\tResulting Value: [%s]\n",f->name,standardfieldtypes[f->type],f->len,temp2,temp1);
							if(temp1!=NULL) Rfree(temp1);
							if(temp2!=NULL) Rfree(temp2);
							break;
						case DOLLARS:
							temp1=famt(*f->data.float_value,f->len);
							temp2=famt(*p->data.float_value,p->len);
							length=RDAstrlen(f->name)+RDAstrlen(temp1)+RDAstrlen(temp2)+RDAstrlen(standardfieldtypes[f->type])+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%s]\n\tResulting Value: [%s]\n",f->name,standardfieldtypes[f->type],f->len,temp2,temp1);
							if(temp1!=NULL) Rfree(temp1);
							if(temp2!=NULL) Rfree(temp2);
							break;
						case DECIMALV:
						case SDECIMALV:
							sprintf(stemp,"%*f",f->len,*f->data.float_value);
							length=RDAstrlen(stemp);
							sprintf(stemp,"%*f",p->len,*p->data.float_value);
							length+=RDAstrlen(stemp);
							length+=RDAstrlen(f->name)+RDAstrlen(standardfieldtypes[f->type])+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*f]\n\tResulting Value: [%*f]\n",f->name,standardfieldtypes[f->type],f->len,p->len,*p->data.float_value,f->len,*f->data.float_value);
							break;
						case DOUBLEV:
						case SDOUBLEV:
							length=RDAstrlen(f->name)+(2*f->len)+RDAstrlen(standardfieldtypes[f->type])+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*.0f]\n\tResulting Value: [%*.0f]\n",f->name,standardfieldtypes[f->type],f->len,f->len,*p->data.float_value,f->len,*f->data.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							length=RDAstrlen(f->name)+RDAstrlen(standardfieldtypes[f->type])+(2*f->len)+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*d]\n\tResulting Value: [%*d]\n",f->name,standardfieldtypes[f->type],f->len,f->len,*p->data.short_value,f->len,*f->data.short_value);
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							length=RDAstrlen(f->name)+RDAstrlen(standardfieldtypes[f->type])+(2*f->len)+69;
							tmp=Rrealloc(tmp,count+length);
							sprintf(&tmp[count],"Field: %s Type: %s Len: %5d\n\t  Initial Value: [%*d]\n\tResulting Value: [%*d]\n",f->name,standardfieldtypes[f->type],f->len,f->len,*p->data.integer_value,f->len,*f->data.integer_value);
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
					count+=length-1;
				}
			}
			}
		}
	}
	tmp=Rrealloc(tmp,count+37);
	sprintf(&tmp[count],"\nDo you wish to save these changes?\n");
	return(tmp);
}
