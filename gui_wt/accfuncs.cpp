#include <guip.hpp>
#include <gui.hpp>

char *fullacctdef(RDAacct *account)
{
	RDAaccpart *accpart=NULL;
	short x, longest;
	char *display_value=NULL,zeros[101];
	int defcount=0;

	if(account!=NULL)
	{
	if(account->acct!=NULL)
	{
	for(x=0,accpart=account->acct;x<account->num;++x,++accpart)
	{
		if(accpart->length>RDAstrlen(accpart->name))
		{
			longest=accpart->length;
		} else {
			longest=RDAstrlen(accpart->name);
		}
		if(display_value!=NULL)
		{
			display_value=Rrealloc((void *)display_value,defcount+longest+2);
		} else {
			display_value=Rmalloc((void *)defcount+longest+1);
		}
		memset(zeros,0,101);
		memset(zeros,'0',100);
		zeros[longest]=0;
		if(defcount>0)
		{
			sprintf(&display_value[defcount],"-%-*s",longest,(accpart->force_zero ? zeros:accpart->definition));
			defcount+=longest+1;
		} else {
			sprintf(&display_value[defcount],"%-*s",longest,(accpart->force_zero ? zeros:accpart->definition));
			defcount+=longest;
		}
	}
	}
	}
	return(display_value);
}
short makeacct(char *old_acct,char *new_acct,char *def,int direction,char *c,unsigned length,short ACCOUNT_CALLBACK_TYPE)
{
	char fillchar='@';
	int x,y,l=0;

	y=RDAstrlen(old_acct);
	for(x=0;(old_acct[x]!=fillchar && x<length && x<y);++x)
	{
		new_acct[x]=old_acct[x];
	}
	y=x;
	switch(direction)
	{
		default:
		case 0: /* removing characters from the end */
			if(RDAstrlen(new_acct)<1) break;
			new_acct[y--]=0;
			if(!ACCOUNT_CALLBACK_TYPE)
			{
				for(;!isalnum(new_acct[y]) || def[y]=='0';--y)
					new_acct[y]=0;
			}
			break;
		case 1: /* adding characters to the end */
			for(l=0;(l<RDAstrlen(c)&&x<length);++l,++x)
			{
				for(;def[x]==' ';) 
				{
					if(c[l]==' ') ++l;
					new_acct[x++]=' ';
				}
				for(;def[x]=='0';) 
				{
					if(c[l]=='0') ++l;
					new_acct[x++]='0';
				}
				if(def[x]=='-')
				{
					if(c[l]=='-') ++l; 
					new_acct[x++]='-';
				}
				for(;def[x]=='0';) 
				{
					if(c[l]=='0') ++l;
					new_acct[x++]='0';
				}
				if(def[x]=='-')
				{
					if(c[l]=='-') ++l; 
					new_acct[x++]='-';
				}
				new_acct[x]=c[l];
			}
			new_acct[x]=0;
			break;
		case 2: /*inserting characters*/
			for(l=0;(l<RDAstrlen(c)&&x<length);++l,++x)
			{
				new_acct[x]=c[l];
			}
			if(x<length-1)
			{
				for(;old_acct[y]==fillchar;y++)
				for(;x<length;++x,++y)
				{
					new_acct[x]=old_acct[y];
				}
			}
			new_acct[length]=0;
			break;
	}
	if(isacctstr(new_acct,def,length,ACCOUNT_CALLBACK_TYPE)==TRUE)
	{
		 return(TRUE);
	} else {
		return(FALSE);
	}
}
char *fullacctlabel(RDAacct *account)
{
	RDAaccpart *accpart=NULL;
	short x, longest;
	char *display_value=NULL;
	int defcount=0;

	if(account!=NULL)
	{
	if(account->acct!=NULL)
	{
	for(x=0,accpart=account->acct;x<account->num;++x,++accpart)
	{
		if(accpart->length>RDAstrlen(accpart->name))
		{
			longest=accpart->length;
		} else {
			longest=RDAstrlen(accpart->name);
		}
		if(display_value!=NULL)
		{
			display_value=Rrealloc((void *)display_value,defcount+longest+2);
		} else {
			display_value=Rmalloc((void *)defcount+longest+1);
		}
		if(defcount>0)
		{
			sprintf(&display_value[defcount],"-%-*s",longest,accpart->name);
			defcount+=longest+1;
		} else {
			sprintf(&display_value[defcount],"%-*s",longest,accpart->name);
			defcount+=longest;
		}
	}
	}
	}
	return(display_value);
}
short isacctstr(char *acctstr, char *def, int len,short ACCOUNT_CALLBACK_TYPE)
{
	int pos=0,acctlen,deflen;
	short return_val=FALSE;

	acctlen=RDAstrlen(acctstr);
	if(acctlen==0) return(TRUE);
	deflen=RDAstrlen(def);
	for(pos=0;(pos<len&&pos<deflen&&pos<acctlen);++pos)
	{
		if(def[pos]=='A')
		{
			if(isalpha(acctstr[pos]) || acctstr[pos]=='.') 
			{
				return_val=TRUE;
			} else if(ACCOUNT_CALLBACK_TYPE && acctstr[pos]==' ')
			{
				 return_val=TRUE;
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui) { prterr("DIAG isacctstr returning FALSE"); }
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		} else if(def[pos]=='N')
		{
			if(isdigit(acctstr[pos]) || acctstr[pos]=='.') 
			{
				return_val=TRUE;
			} else if(ACCOUNT_CALLBACK_TYPE && acctstr[pos]==' ')
			{
				return_val=TRUE;
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui) { prterr("DIAG isacctstr returning FALSE"); }
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		} else if(def[pos]=='X')
		{
			if(isalnum(acctstr[pos]) || acctstr[pos]=='.') 
			{
				return_val=TRUE;
			} else if(ACCOUNT_CALLBACK_TYPE && acctstr[pos]==' ')
			{
				return_val=TRUE;
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui) { prterr("DIAG isacctstr returning FALSE"); }
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		} else if(def[pos]=='*')
		{
			return_val=TRUE;
		} else if((def[pos]=='-') || (def[pos]==' ') || (def[pos]=='0'))
		{
			if(acctstr[pos]==def[pos]) 
			{
				return_val=TRUE;
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui) { prterr("DIAG isacctstr returning FALSE"); }
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		}
	}
	if(pos==len) acctstr[len]=0;
	else if(pos==deflen) acctstr[deflen]=0;
	else if(pos==acctlen) acctstr[acctlen]=0;
	return(return_val);
}
char *xMergeImage(char *acct,char *image,short type,short def_type,
	int line,char *file)
{
	RDAacct *holdacct=NULL;
	char *tmp=NULL,*def=NULL;
	unsigned length=0;
	int x,y;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG MergeImage Merging Account Code [%s] with Image [%s] at line [%d] program [%s].",(acct!=NULL ? acct:""),(image!=NULL ? image:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(isEMPTY(image))
	{
		tmp=stralloc(acct);
		return(tmp);
	} else if(isEMPTY(acct) && isEMPTY(image))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui)
		{
			prterr("DIAG MergeImage Returning [%s] to line [%d] program [%s].",(tmp!=NULL ? tmp:""),line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(tmp);
	}
	switch(type)
	{
		case EXPENDITURE:	
			if(EXPENDITURE_ACCOUNT->num>def_type)
			{
				holdacct=EXPENDITURE_ACCOUNT->codetype+def_type;
			}
			break;
		case REVENUE:		
			if(REVENUE_ACCOUNT->num>def_type)
			{
				holdacct=REVENUE_ACCOUNT->codetype+def_type;
			}
			break;
		case BALANCESHEET:	
			if(BALANCE_SHEET_ACCOUNT->num>def_type) 
			{
				holdacct=BALANCE_SHEET_ACCOUNT->codetype+def_type;
			}
			break;
		case BEGINNINGBALANCE:	
			if(BEGINNING_BALANCE_ACCOUNT->num>def_type)
			{
				holdacct=BEGINNING_BALANCE_ACCOUNT->codetype+def_type;
			}
			break;
		case CUSTOMTYPE:
			if(CUSTOM_INPUTS_ACCOUNT->num>def_type)
			{
				holdacct=CUSTOM_INPUTS_ACCOUNT->codetype+def_type;
			}
			break;
		default:
			prterr("Error Account type [%d] invalid at line [%d] program [%s] in MergeImage.",type,line,file);
			return(NULL);
	}
	def=fullacctdef(holdacct);
	length=RDAstrlen(def);
	tmp=Rmalloc((void *)length+1);
	memset(tmp,0,length+1);
	memset(tmp,' ',length);
	if(!isEMPTY(image)) 
	{
		y=RDAstrlen(image);
		if(y>length)
		{
			memcpy(tmp,image,length);
		} else {
			memcpy(tmp,image,y);
		}
	}
	for(x=0;x<length;++x)
	{
		if((def[x]=='A' || def[x]=='N' || def[x]=='X' || def[x]=='*') && tmp[x]==' ') 
		{
			tmp[x]=acct[x];
		} else if(def[x]=='-' || def[x]==' ' || def[x]=='0') tmp[x]=def[x];
	}
	tmp[length]=0;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG MergeImage Returning [%s] to line [%d] program [%s].",tmp,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(tmp);
}
