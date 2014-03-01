/* dochg3.c - dochg_nokey */
#include<dfchg.hpp>

extern CHGstruct *CHGSTRUCT;
#define SetCHGstructSupportingFiles(a,b,c)	xSetCHGstructSupportingFiles(a,b,c,__LINE__,__FILE__)
extern char *xSetCHGstructSupportingFiles(CHGfile *,void (*)(...),void *,int,char *);
extern short Write_Change(CHGfile *);

void dochg_nokey(RDArsrc *tmprsrc,RangeScreen *rs,CHGfile *chgfile)
{
	long Update_Count=0;
	short ef=FALSE,testfile=FALSE;
	RDAsearch *s=NULL;
	int x,y,z;
	NRDfield *fields=NULL,*field=NULL;
	short d_update=0,numflds=0;	
	CHGfield *fld=NULL;
	char *temp=NULL;
	NRDkey *keys=NULL,*key=NULL;
	NRDpart *part=NULL;
	RDArsrc *xrsrc=NULL;
#ifdef WIN32
	char *libname=NULL;
#endif

	if(diagapps)
	{
		prterr("DIAG dochg_nokey [%s][%s].",(chgfile->module!=NULL ? 
			chgfile->module:""),(chgfile->filename!=NULL ? 
			chgfile->filename:""));
	}
	chgfile->prev=RDATDataNEW(chgfile->fileno);
	temp=unique_name();
	chgfile->sort=DBsortNEW(CHGSTRUCT->module,temp,0);
	fields=FLDPOINTER(chgfile->fileno);
	numflds=NUMFLDS(chgfile->fileno);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<numflds;++x,++field)
		{
			addDBfilefield(chgfile->sort,chgfile->fileno,
				field->name);
		}
	}	
	keys=KEYPOINTER(chgfile->fileno);
	if(keys!=NULL)
	{
		key=keys;
		addDBkey(chgfile->sort,key->name);
		if(key->part!=NULL)
		{
			for(x=0,part=key->part;x<key->numparts;++x,++part)
			{
				addDBkeypart(chgfile->sort,1,part->name);
			}	
		}
	}
	chgfile->sort->fileno=OPNDBsort(chgfile->sort);
	ZERNRD(chgfile->fileno);

	ef=ADVBEGNRDsec(chgfile->fileno,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
	while(!ef)
	{
		GetRDATData(chgfile->fileno,chgfile->prev);
		SetCHGstructSupportingFiles(chgfile,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
		if(CHGSTRUCT->simple_from==FALSE)
		{
			if(chgfile->supfil!=NULL)
			{
				for(x=0,s=chgfile->supfil;x<chgfile->num;++x,++s)
				{
					if(s->range)
					{
						testfile=testfilerangersrc(s->fileno,rs);
					} 
					if(testfile) break;
				}
				if(testfile==FALSE)
				{
					if(isEMPTY(chgfile->expression))
					{
						d_update=Write_Change(chgfile);
						++Update_Count;
					} else if(PP_EVALUATEbol(chgfile->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs))
					{
						d_update=Write_Change(chgfile);
						++Update_Count;
					}
				}
			}
		} else {
			fields=FLDPOINTER(chgfile->fileno);
			numflds=NUMFLDS(chgfile->fileno);
			if(fields!=NULL && chgfile->chgflds!=NULL)
			{
				testfile=TRUE;
				for(z=0,fld=chgfile->chgflds;z<chgfile->numflds;++z,++fld)
				{
					for(y=0,field=fields;y<numflds;++y,++field)
					{
						if(!RDAstrcmp(field->name,fld->fieldname)) break;
					}
					if(y<numflds)
					{
						switch(CHGSTRUCT->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case PLAINTEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(!RDAstrcmp(CHGSTRUCT->from.string_value,field->data.string_value)) testfile=FALSE;
								break;
							case BOOLNS:
							case CHARACTERS:
								if(*CHGSTRUCT->from.string_value==*field->data.string_value) testfile=FALSE;
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DOUBLEV:
							case SDOUBLEV:
								if(*CHGSTRUCT->from.float_value==*field->data.float_value) testfile=FALSE;
								break;
							case SHORTV:
							case SSHORTV:
								if(*CHGSTRUCT->from.short_value==*field->data.short_value) testfile=FALSE;
								break;	
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								if(*CHGSTRUCT->from.integer_value==*field->data.integer_value) testfile=FALSE;
								break;
							default:
								prterr("Error Field type [%d] invalid.",CHGSTRUCT->type);
								break;
						}
					}
					if(testfile==FALSE)
					{
						if(isEMPTY(chgfile->expression))
						{
							d_update=Write_Change(chgfile);
							++Update_Count;
						} else if(PP_EVALUATEbol(chgfile->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs))
						{
							d_update=Write_Change(chgfile);
							++Update_Count;
						}
					}
				}
			}
		}
		if(tmprsrc!=NULL)
		{
			update_diagnostic(tmprsrc,(d_update ? TRUE:FALSE));
		}
		ef=ADVSEQNRDsec(chgfile->fileno,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
	}
	if(chgfile->prev!=NULL) FreeRDATData(chgfile->prev);
#ifdef XXXX
	if(SIZNRD(chgfile->sort->fileno)>0)
#endif /* XXXX */
	if(Update_Count>0)
	{
		xrsrc=diagscrn(chgfile->sort->fileno,"DIAGNOSTIC SCREEN",CHGSTRUCT->module,NULL,NULL);
		if(xrsrc!=NULL)
		{
			if(ADVmakescrn(xrsrc,TRUE))
			{
			} else {
				ForceWindowUpdate(xrsrc);
			}
		}
		fields=FLDPOINTER(chgfile->sort->fileno);
		numflds=NUMFLDS(chgfile->sort->fileno);
		ef=BEGNRD(chgfile->sort->fileno);
		while(!ef)
		{
			if(fields!=NULL)
			{
				for(x=0,field=fields;x<numflds;++x,++field)
				{
					COPYFIELD(chgfile->sort->fileno,chgfile->fileno,field->name);	
				}
				ADVWRTTRANSsec(chgfile->fileno,0,NULL,NULL,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
			}
			if(xrsrc!=NULL) update_diagnostic(xrsrc,TRUE);
			ef=SEQNRD(chgfile->sort->fileno);
		}
		if(xrsrc!=NULL)
		{
			killwindow(xrsrc);
			free_rsrc(xrsrc);
			xrsrc=NULL;
		}
	}
	if(chgfile->sort!=NULL)
	{
		if(chgfile->sort->fileno!=(-1)) CLSNRD(chgfile->sort->fileno);
		ADVFreeDBsort(chgfile->sort,TRUE);
	}
}
