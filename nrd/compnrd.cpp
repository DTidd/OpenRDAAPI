#include <nrdp.hpp>

short xCOMPARENRDfield(NRDfield *fld,NRDfield *field,int line,char *file)
{
	short empty1=FALSE,empty2=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG COMPARENRDfield Comparing Field [%s]'s value to Field [%s] at line [%d] program [%s].",field->name,fld->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
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
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case CUSTOMTYPE:
			switch(fld->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					empty1=isEMPTY(fld->data.string_value);
					empty2=isEMPTY(field->data.string_value);
					if(empty1 && empty2) 
					{
						return(FALSE);
					} else if(!empty1 && !empty2)
					{
						return(RDAstrcmp(fld->data.string_value,field->data.string_value));
					} else return(TRUE);
				case CHARACTERS:
				case BOOLNS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
				case SHORTV:
				case SSHORTV:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				default:
					prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
		case CHARACTERS:
		case BOOLNS:
			switch(fld->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
/*lint -e524 */
					if(*field->data.string_value==*fld->data.float_value)
					{
							return(FALSE);
					} else return(TRUE);
/*lint +e524 */
				case SHORTV:
				case SSHORTV:
/*lint -e524 */
					if(*field->data.string_value==*fld->data.short_value)
					{
							return(FALSE);
					} else return(TRUE);
/*lint +e524 */
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
/*lint -e524 */
					if(*field->data.string_value==*fld->data.integer_value)
					{
							return(FALSE);
					} else return(TRUE);
/*lint +e524 */
				case CHARACTERS:
				case BOOLNS:
/*lint -e524 */
					if(*field->data.string_value==*fld->data.string_value)
					{
							return(FALSE);
					} else return(TRUE);
/*lint +e524 */
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			switch(fld->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case CHARACTERS:
				case BOOLNS:
					if(*field->data.float_value==*fld->data.string_value)
					{
							return(FALSE);
					} else return(TRUE);
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if((*field->data.float_value)==(*fld->data.float_value))
					{
						return(FALSE);
					} else {
						return(TRUE);
					}
				case SHORTV:
				case SSHORTV:
					if(*field->data.float_value==*fld->data.short_value)
					{
							return(FALSE);
					} else return(TRUE);
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(*field->data.float_value==*fld->data.integer_value)
					{
							return(FALSE);
					} else return(TRUE);
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
		case SHORTV:
		case SSHORTV:
			switch(fld->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case CHARACTERS:
				case BOOLNS:
					if(*field->data.short_value==*fld->data.string_value)
					{
							return(FALSE);
					} else return(TRUE);
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
/*lint -e524 */
					if(*field->data.short_value==*fld->data.float_value)
					{
							return(FALSE);
					} else return(TRUE);
/*lint +e524 */
				case SHORTV:
				case SSHORTV:
					if(*field->data.short_value==*fld->data.short_value)
					{
							return(FALSE);
					} else return(TRUE);
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
/*lint -e524 */
					if(*field->data.short_value==*fld->data.integer_value)
					{
							return(FALSE);
					} else return(TRUE);
/*lint +e524 */
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			switch(fld->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case CHARACTERS:
				case BOOLNS:
					if(*field->data.integer_value==*fld->data.string_value)
					{
							return(FALSE);
					} else return(TRUE);
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
/*lint -e524 */
					if(*field->data.integer_value==*fld->data.float_value)
					{
							return(FALSE);
					} else return(TRUE);
/*lint +e524 */
				case SHORTV:
				case SSHORTV:
					if(*field->data.integer_value==*fld->data.short_value)
					{
							return(FALSE);
					} else return(TRUE);
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(*field->data.integer_value==*fld->data.integer_value)
					{
							return(FALSE);
					} else return(TRUE);
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
		default:
			prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
			return(-1);
	}
}
