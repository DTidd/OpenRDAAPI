/* mkdeffnc.c - Make Screen Default Functions */
#include<mkdef.hpp>

int xaddDefaultFieldSTRING(RDAdefault *d,char *name,
		char reset,short type,char *value,
		int line,char *file)
{
	DEFfield *field=NULL;

	if(d!=NULL)
	{
		if(d->field!=NULL)
		{
			d->field=Rrealloc(d->field,(d->num+1)*sizeof(DEFfield));
		} else {
			d->field=Rmalloc(sizeof(DEFfield));
			d->num=0;
		}
		field=d->field+d->num;
		if(!isEMPTY(name))
		{
			field->name=stralloc(name);
		} else field->name=NULL;
		field->type=type;
		field->reset=reset;
		switch(type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(value!=NULL)
				{
					field->data.string_value=stralloc(value);
				} else {
					field->data.string_value=NULL;
				}
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
			case SHORTV:
			case SSHORTV:
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
			case BOOLNS:
			case CHARACTERS:
				prterr("Error: Incorrect Field Type, should be an STRING [%s].",(field->name!=NULL ? field->name:""));
				break;
			default :
				prterr("Error: Unrecognized Field Type on field [%s].",(field->name!=NULL ? field->name:""));
				break;

		}
		return(++d->num);
	} else {
		return(-1);
	}
}
int xaddDefaultFieldINT(RDAdefault *d,char *name,
		char reset,short type,int value,
		int line,char *file)
{
	DEFfield *field=NULL;

	if(d!=NULL)
	{
		if(d->field!=NULL)
		{
			d->field=Rrealloc(d->field,(d->num+1)*sizeof(DEFfield));
		} else {
			d->field=Rmalloc(sizeof(DEFfield));
			d->num=0;
		}
		field=d->field+d->num;
		if(!isEMPTY(name))
		{
			field->name=stralloc(name);
		} else field->name=NULL;
		field->type=type;
		field->reset=reset;
		switch(type)
		{
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				field->data.integer_value=Rmalloc(sizeof(int));
				*field->data.integer_value=value;
				break;
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
			case SHORTV:
			case SSHORTV:
			case BOOLNS:
			case CHARACTERS:
				prterr("Error: Incorrect Field Type, should be an INTEGER.");
				break;
			default :
				prterr("Error: Unrecognized Field Type.");
				break;

		}
		return(++d->num);
	} else {
		return(-1);
	}
}
int xaddDefaultFieldSHORT(RDAdefault *d,char *name,
		char reset,short type,short value,
		int line,char *file)
{
	DEFfield *field=NULL;

	if(d!=NULL)
	{
		if(d->field!=NULL)
		{
			d->field=Rrealloc(d->field,(d->num+1)*sizeof(DEFfield));
		} else {
			d->field=Rmalloc(sizeof(DEFfield));
			d->num=0;
		}
		field=d->field+d->num;
		if(!isEMPTY(name))
		{
			field->name=stralloc(name);
		} else field->name=NULL;
		field->type=type;
		field->reset=reset;
		switch(type)
		{
			case SHORTV:
			case SSHORTV:
				field->data.short_value=Rmalloc(sizeof(short));
				*field->data.short_value=value;
				break;
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
			case BOOLNS:
			case CHARACTERS:
				prterr("Error: Incorrect Field Type, should be an SHORT.");
				break;
			default :
				prterr("Error: Unrecognized field type.");
				break;

		}
		return(++d->num);
	} else {
		return(-1);
	}
}
int xaddDefaultFieldDOUBLE(RDAdefault *d,char *name,
		char reset,short type,double value,
		int line,char *file)
{
	DEFfield *field=NULL;

	if(d!=NULL)
	{
		if(d->field!=NULL)
		{
			d->field=Rrealloc(d->field,(d->num+1)*sizeof(DEFfield));
		} else {
			d->field=Rmalloc(sizeof(DEFfield));
			d->num=0;
		}
		field=d->field+d->num;
		if(!isEMPTY(name))
		{
			field->name=stralloc(name);
		} else field->name=NULL;
		field->type=type;
		field->reset=reset;
		switch(type)
		{
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
				field->data.float_value=Rmalloc(sizeof(double));
				*field->data.float_value=value;
				break;
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case SHORTV:
			case SSHORTV:
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
			case BOOLNS:
			case CHARACTERS:
				prterr("Error: Incorrect Field Type, should be an DOUBLE.");
				break;
			default :
				prterr("Error: Unrecognized Field Type.");
				break;

		}
		return(++d->num);
	} else {
		return(-1);
	}
}
int xaddDefaultFieldCHAR(RDAdefault *d,char *name,
		char reset,short type,char value,
		int line,char *file)
{
	DEFfield *field=NULL;

	if(d!=NULL)
	{
		if(d->field!=NULL)
		{
			d->field=Rrealloc(d->field,(d->num+1)*sizeof(DEFfield));
		} else {
			d->field=Rmalloc(sizeof(DEFfield));
			d->num=0;
		}
		field=d->field+d->num;
		if(!isEMPTY(name))
		{
			field->name=stralloc(name);
		} else field->name=NULL;
		field->type=type;
		field->reset=reset;
		switch(type)
		{
			case BOOLNS:
			case CHARACTERS:
				field->data.string_value=Rmalloc(sizeof(1));
				*field->data.string_value=value;
				break;
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
			case SHORTV:
			case SSHORTV:
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				prterr("Error: Incorrect Field Type, should be an DOUBLE.");
				break;
			default :
				prterr("Error: Unrecognized Field Type.");
				break;

		}
		return(++d->num);
	} else {
		return(-1);
	}
}


/* 
RDAdefault *xRDADefaultNEW(char *module,char *name,char *input_focus,
			int num,DEFfield *fields,int line,char *file)
{
	RDAdefault *tmp=NULL;
	int x=0;
	DEFfield *tmpfield=NULL,*field=NULL;

	tmp=Rmalloc(sizeof(RDAdefault));
	if(!isEMPTY(module))
	{
		tmp->module=stralloc(module);
	} else tmp->module=NULL;
	if(!isEMPTY(name))
	{
		tmp->name=stralloc(name);
	} else tmp->name=NULL;
	if(!isEMPTY(input_focus))
	{
		tmp->input_focus=stralloc(input_focus);
	} else tmp->input_focus=NULL;
	if(fields!=NULL && num>0)
	{
		tmp->field=Rmalloc(num*sizeof(DEFfield));
		tmp->num=num;
		for(x=0,field=fields,tmpfield=tmp->field;x<tmp->num;++x,++field,++tmpfield)
		{
			if(!isEMPTY(name))
			{
				tmpfield->name=stralloc(field->name);
			} else tmpfield->name=NULL;
			tmpfield->type=field->type;
			tmpfield->reset=field->reset;
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(field->data.string_value))
					{ 
						tmpfield->data.string_value=stralloc(field->data.string_value);
					} else {
						tmpfield->data.string_value=NULL;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case SDOUBLEV:
				case DOUBLEV:
					tmpfield->data.float_value=Rmalloc(sizeof(double));
					*tmpfield->data.float_value=*field->data.float_value;
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					tmpfield->data.integer_value=Rmalloc(sizeof(int));
					*tmpfield->data.integer_value=*field->data.integer_value;
					break;
				case SHORTV:
				case SSHORTV:
					tmpfield->data.short_value=Rmalloc(sizeof(short));
					*tmpfield->data.short_value=*field->data.short_value;
					break;
				case BOOLNS:
				case CHARACTERS:
					tmpfield->data.string_value=Rmalloc(sizeof(1));
					*tmpfield->data.string_value=*field->data.string_value;
					break;
				
				default :
					prterr("Error: Unrecognized Field Type.");
					break;
			}
		}
	} else {
		tmp->field=NULL;
		tmp->num=0;
	}
	return(tmp);
}
*/
