#include <mix.hpp>
#ifdef WIN32
#include<pay.hpp>
#include<winbase.h>
#include<rdalic.h>
#include<windows.h>
#include<sec.hpp>
#include<direct.h>
#endif
#include<guip.hpp>

#if defined(LINUX2_2) || defined(UNIXWARE7) || defined(UBUNTU_OS)
#endif
#ifdef WIN32
#ifdef __GTK_H__
#endif /* __GTK_H__ */
#endif /* WIN32 */

short GUI_AUTO_COMPLETE=(-1);

void xmakefieldrsrc(RDArsrc *rsrc,char *name,short type,unsigned len,char editable,short Rmem)
{
	short defshort=0;
	char defaultch=0,*defaultc=NULL;
	double deffloat=0;
	int defint=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG makefieldrsrc Make Resource for [%s] Type [%s] Length [%d] editable [%s] for Screen [%s] [%s].",name,standardfieldtypes[type],len,(editable ? "Yes":"No"),rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	switch(type)
	{
		case SCROLLEDTEXT:
			if(Rmem==0) addsctrsrc(rsrc,name,len,defaultc,editable);
				else RMEMaddsctrsrc(rsrc,name,len,defaultc,editable);
			break;
		case VARIABLETEXT:
		case PLAINTEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PHONE:
		case CUSTOMTYPE:
		case ZIPCODE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			if(Rmem==0) addstdrsrc(rsrc,name,type,len,defaultc,editable);
				else RMEMaddstdrsrc(rsrc,name,type,len,defaultc,editable);
			break;
		case SHORTV:
		case SSHORTV:
			if(Rmem==0) addstdrsrc(rsrc,name,type,len,&defshort,editable);
			else RMEMaddstdrsrc(rsrc,name,type,len,&defshort,editable);
			break;
		case LONGV:
		case SLONGV:
			if(Rmem==0) addstdrsrc(rsrc,name,type,len,&defint,editable);
			else RMEMaddstdrsrc(rsrc,name,type,len,&defint,editable);
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
			if(Rmem==0) addstdrsrc(rsrc,name,type,len,&deffloat,editable);
			else RMEMaddstdrsrc(rsrc,name,type,len,&deffloat,editable);
			break;
		case BOOLNS:
			defaultc=FALSE;
			if(Rmem==0) addstdrsrc(rsrc,name,type,len,&defaultch,editable);
			else RMEMaddstdrsrc(rsrc,name,type,len,&defaultch,editable);
			break;
		case CHARACTERS:
			defaultc=0;
			if(Rmem==0) addstdrsrc(rsrc,name,type,len,&defaultch,editable);
			else RMEMaddstdrsrc(rsrc,name,type,len,&defaultch,editable);
			break;
		default:
			prterr("Error Unrecognized Field type [%d] for resource [%s] not added to Screen [%s] [%s].",type,name,rsrc->module,rsrc->screen); 
			break;
	}
}
/*-----------------------------------------------------------------------------
	fields2file	function to copy fields into a file buffer.
-----------------------------------------------------------------------------*/
short xfields2file(short fileno,NRDfield *fields,int line,char *file)
{
	int y=0,numflds=0;
	NRDfield *fldx=NULL,*tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG fields2file Copies an arrary of NRDfields into Module [%s] Fileno [%d] [%s] at line [%d] program [%s] ",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	numflds=NUMFLDS(fileno);
	if(numflds>0)
	{
		for(y=0,fldx=fields;y<numflds;++y,++fldx)
		{
			tmp=FLDNUM(fileno,y);
			switch(tmp->type)
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
					if(RDAstrcmp(tmp->data.string_value,fldx->data.string_value))
					{
						if(tmp->data.string_value!=NULL) 
							Rfree(tmp->data.string_value);
						tmp->data.string_value=fldx->data.string_value;
						tmp->dlen=RDAstrlen(tmp->data.string_value)+1;
					} else {
						if(fldx->data.string_value!=NULL) 
							Rfree(fldx->data.string_value);
					}
					if(fldx->name!=NULL) Rfree(fldx->name);
					break;
				case CHARACTERS:
					if(*tmp->data.string_value!=*fldx->data.string_value)
					{
						*tmp->data.string_value=*fldx->data.string_value;
					}
					Rfree(fldx->data.string_value);
					if(fldx->name!=NULL) Rfree(fldx->name);
					break;
				case BOOLNS:
					if(*tmp->data.string_value!=*fldx->data.string_value)
					{
						*tmp->data.string_value=*fldx->data.string_value;
					}
					Rfree(fldx->data.string_value);
					if(fldx->name!=NULL) Rfree(fldx->name);
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(*tmp->data.float_value!=*fldx->data.float_value)
					{
						*tmp->data.float_value=*fldx->data.float_value;
					}
					Rfree(fldx->data.float_value);
					if(fldx->name!=NULL) Rfree(fldx->name);
					break;
				case SHORTV: 
				case SSHORTV:
					if(*tmp->data.short_value!=*fldx->data.short_value)
					{
						*tmp->data.short_value=*fldx->data.short_value;
					}
					Rfree(fldx->data.short_value);
					if(fldx->name!=NULL) Rfree(fldx->name);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(*tmp->data.integer_value!=*fldx->data.integer_value)
					{
						*tmp->data.integer_value=*fldx->data.integer_value;
					}
					Rfree(fldx->data.integer_value);
					if(fldx->name!=NULL) Rfree(fldx->name);
					break;
				default:
					prterr("Error cannot recognize field type [%d] for field [%s] file [%s] at line [%d] program [%s].",tmp->type,tmp->name,FILENAME(fileno),line,file);
					break;
			}
		}
		if(fields!=NULL) { Rfree(fields); fields=NULL; }
	}
	return(0);
}
/*-----------------------------------------------------------------------------
	Rfreetmpfld	function to free temporary NRDfield structure.
-----------------------------------------------------------------------------*/
short xRfreetmpfld(NRDfield *fields,short numflds,int line,char *file)
{
	short y=0;
	NRDfield *fldx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG Rfreetmpfld Rfree's a temporary NRDfield structure at line [%d] program [%s] ",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(numflds>0)
	{
		for(y=0,fldx=fields;y<numflds;++y,++fldx)
		{
			FreeNRDfield(fldx);
		}
		if(fields!=NULL) { Rfree(fields); fields=NULL; }
	}
	return(0);
}
/*-----------------------------------------------------------------------------
	file2rsrc	function to create resources for each field in the file 
-----------------------------------------------------------------------------*/
short xfile2rsrc(short fileno,RDArsrc *rsrc,char editable,short type,void (*func)(...),void *args,int line,char *file)
{
	NRDfield *field=NULL;
	short x,numflds=0;
	char *name=NULL,*filename;
	short edit_rsrc=0;

	if(!TESTFILENO(fileno,line,file)) return(-1);
	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Creating Resources for Module [%s] File [%d] [%s] with editable status [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,filename,(editable ? "Yes":"No"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if((numflds=NUMFLDS(fileno))==(-1))
	{
		prterr("Error Cannot get number of fields in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,filename,line,file);
		return(-1);
	}
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(fileno,x);
		if(field!=NULL)
		{
			edit_rsrc=editable;
			if(FIELDscrn(fileno,field->name,
				&edit_rsrc))
			{
				if(!type)
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(field->name)+5+RDAstrlen(filename));
					} else name=Rmalloc(RDAstrlen(field->name)+5+RDAstrlen(filename));
					sprintf(name,"[%s][%s]",filename,field->name);
				} else {
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(field->name)+3);
					} else name=Rmalloc(RDAstrlen(field->name)+3);
					sprintf(name,"%s",field->name);
				}
				if(!alreadyrsrc(name,rsrc))
				{
					makefieldrsrc(rsrc,name,field->type,
						field->len,edit_rsrc);
					FINDRSCSETFUNC(rsrc,name,func,args);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	return(0);
}
void xsetfile2rsrccallbacks(short fileno,RDArsrc *rsrc,short type,int line,char *file)
{
	NRDfield *field=NULL;
	short x,y,numflds=0;
	char *name=NULL,*filename=NULL;
	RDArmem *member=NULL;
	
	if(!TESTFILENO(fileno,line,file)) return;
	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG setfile2rsrccallbacks Setting File to Resource Callbacks for Module [%s] File [%d] [%s] onto Screen [%s] [%s].",MODULENAME(fileno),fileno,filename,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if((numflds=NUMFLDS(fileno))==(-1))
	{
		prterr("Error Cannot get number of fields in Module [%s] File [%d] [%s].",MODULENAME(fileno),fileno,filename);
		return;
	}
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(fileno,x);
		if(field!=NULL)
		{
			if(field->type!=BOOLNS)
			{
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+14+RDAstrlen(filename));
					sprintf(name,"RANGE ON [%s][%s]",filename,field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+8);
					sprintf(name,"RANGE %s",field->name);
				}
				ADVFINDRSCSETFUNC(rsrc,name,setrangesensitivecb,NULL,TRUE);
				if(name!=NULL) Rfree(name);
			}
			if(!RDAstrcmp(field->name,"DEFINITION TYPE"))
			{
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+10+RDAstrlen(filename));
					sprintf(name,"FROM [%s][%s]",filename,field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"FROM %s",field->name);
				}
				y=FINDRSC(rsrc,name);
				if(y!=(-1))
				{
					member=rsrc->rscs+y;
				} else member=NULL;
				ADVFINDRSCSETFUNC(rsrc,name,setdeftypefrom,member,0);
				Rfree(name);
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+8+RDAstrlen(filename));
					sprintf(name,"TO [%s][%s]",filename,field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"TO %s",field->name);
				}
				y=FINDRSC(rsrc,name);
				if(y!=(-1))
				{
					member=rsrc->rscs+y;
				} else member=NULL;
				ADVFINDRSCSETFUNC(rsrc,name,setdeftypeto,member,0);
				Rfree(name);
			} else if(!RDAstrcmp(field->name,"ACCOUNT TYPE"))
			{
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+10+RDAstrlen(filename));
					sprintf(name,"FROM [%s][%s]",filename,field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"FROM %s",field->name);
				}
				y=FINDRSC(rsrc,name);
				if(y!=(-1))
				{
					member=rsrc->rscs+y;
				} else { 
					member=NULL;
				}
				ADVFINDRSCSETFUNC(rsrc,name,setacctypefrom,member,0);
				Rfree(name);
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+10+RDAstrlen(filename));
					sprintf(name,"TO [%s][%s]",filename,field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+4);
					sprintf(name,"TO %s",field->name);
				}
				y=FINDRSC(rsrc,name);
				if(y!=(-1))
				{
					member=rsrc->rscs+y;
				} else { 
					member=NULL;
				}
				ADVFINDRSCSETFUNC(rsrc,name,setacctypeto,member,0);
				Rfree(name);
			}
		}
	}
}
void setdeftype(RDArmem *mem,short deftype)
{
	RDArsrc *h;

	h=mem->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG setdeftype Setting Definition Type on Screen [%s] [%s] in Resource [%s] to Definition Type [%d].",h->module,h->screen,mem->rscrname,deftype);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	switch(mem->field_type)
	{
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
			if(!mem->rtype)
			{
				mem->items=deftype;
				if(mem->value.string_value!=NULL) Rfree(mem->value.string_value);
				mem->value.string_value=NULL;
				MEMBERSETDEFTYPE(mem,deftype);
				updatemember(mem);
			}
			break;
		default:
			prterr("Error Resource [%s] has wrong field type [%d] [%s] on Screen [%s] [%s] to receive a Definition Type.",mem->rscrname,mem->field_type,standardfieldtypes[mem->field_type],h->module,h->screen);
			break;
	}
}
void setdeftypefrom(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	char *temp=NULL,*tmp=NULL;
	short x;

	if(member!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG setdeftypefrom SetDefinitionType to FROM part of range using resource [%s] off Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL)
	{
		readwidget(rsrc,member->rscrname);
		temp=member->rscrname+5;
		tmp=stripmodulename(temp);
		temp=Rmalloc(RDAstrlen(tmp)+22);
		sprintf(temp,"FROM [%s][ACCOUNT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(20);
		sprintf(temp,"FROM [ACCOUNT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(RDAstrlen(tmp)+20);
		sprintf(temp,"FROM [%s][DEBIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(18);
		sprintf(temp,"FROM [DEBIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(RDAstrlen(tmp)+21);
		sprintf(temp,"FROM [%s][CREDIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(19);
		sprintf(temp,"FROM [CREDIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		if(tmp!=NULL) Rfree(tmp);
	}
	} else {
		prterr("Error member is NULL");
	}
}
void setdeftypeto(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem;
	char *temp=NULL,*tmp=NULL;
	short x;

	if(member!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG setdeftypeto Setting Definition Types to TO part of range using Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL)
	{
		readwidget(rsrc,member->rscrname);
		temp=member->rscrname+3;
		tmp=stripmodulename(temp);
		temp=Rmalloc(RDAstrlen(tmp)+20);
		sprintf(temp,"TO [%s][ACCOUNT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(18);
		sprintf(temp,"TO [ACCOUNT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(RDAstrlen(tmp)+18);
		sprintf(temp,"TO [%s][DEBIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(16);
		sprintf(temp,"TO [DEBIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(RDAstrlen(tmp)+19);
		sprintf(temp,"TO [%s][CREDIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(17);
		sprintf(temp,"TO [CREDIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		if(tmp!=NULL) Rfree(tmp);
	}
	} else {
		prterr("Error member is NULL");
	}
}
void setacctype(RDArmem *mem,short acctype)
{
	RDArsrc *r;
	
	r=mem->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG setaccttype Setting the Account Type to [%d] for Resource [%s] on Screen [%s] [%s].",acctype,mem->rscrname,r->module,r->screen);		
	}
#endif /* USE_RDA_DIAGNOSTICS */
	switch(mem->field_type)
	{
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
			switch(acctype)
			{
				case EXP_ACCT:
					mem->field_type=EXPENDITURE;
					break;
				case REV_ACCT:
					mem->field_type=REVENUE;
					break;
				case BAL_ACCT:
					mem->field_type=BALANCESHEET;
					break;
				case BEG_ACCT:
					mem->field_type=BEGINNINGBALANCE;
					break;
				default:
					prterr("ERROR - Account Type [%d] is incorrect and will not be set for Resource [%s] on Screen [%s] [%s].",acctype,mem->rscrname,r->module,r->screen);
					break;
			}
			MEMBERSETACCTTYPE(mem,acctype);
			break;
		default:
			prterr("Error Resource [%s] on Screen [%s] [%s] has the wrong field type [%d] [%s] to use the account types.",mem->rscrname,r->module,r->screen,mem->field_type,standardfieldtypes[mem->field_type]);
			break;
	}
	if(mem->dlen>0)
	{
		memset(mem->value.string_value,0,mem->dlen);
	} else {
		mem->value.string_value=NULL;
		mem->dlen=0;
	}
	updatemember(mem);
}
void setacctypefrom(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem=NULL;
	char *temp=NULL,*tmp=NULL;
	short x=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG setacctypefrom Set the Account to for the FROM parts in a range structure using Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL && member!=NULL)
	{
		readwidget(rsrc,member->rscrname);
		temp=member->rscrname+5;
		tmp=stripmodulename(temp);
		temp=Rmalloc(RDAstrlen(tmp)+22);
		sprintf(temp,"FROM [%s][ACCOUNT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
		temp=Rmalloc(20);
		sprintf(temp,"FROM [ACCOUNT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
#ifdef XXXX
		temp=Rmalloc(RDAstrlen(tmp)+20);
		sprintf(temp,"FROM [%s][DEBIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
		temp=Rmalloc(18);
		sprintf(temp,"FROM [DEBIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
		temp=Rmalloc(RDAstrlen(tmp)+21);
		sprintf(temp,"FROM [%s][CREDIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
		temp=Rmalloc(19);
		sprintf(temp,"FROM [CREDIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setdeftype(mem,*member->value.short_value);
		}
#endif /* XXXX */
		if(tmp!=NULL) Rfree(tmp);
	}
}
void setacctypeto(RDArsrc *rsrc,RDArmem *member)
{
	RDArmem *mem=NULL;
	char *temp=NULL,*tmp=NULL;
	short x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG setacctypeto Setting the Account types in the TO parts of a range using Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(member!=NULL)
	{
	if(rsrc!=NULL)
	{
		readwidget(rsrc,member->rscrname);
		temp=member->rscrname+3;
		tmp=stripmodulename(temp);
		temp=Rmalloc(RDAstrlen(tmp)+20);
		sprintf(temp,"TO [%s][ACCOUNT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
		temp=member->rscrname+3;
		temp=Rmalloc(18);
		sprintf(temp,"TO [ACCOUNT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
#ifdef XXXX
		temp=Rmalloc(RDAstrlen(tmp)+18);
		sprintf(temp,"TO [%s][DEBIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
		temp=Rmalloc(16);
		sprintf(temp,"TO [DEBIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
		temp=Rmalloc(RDAstrlen(tmp)+19);
		sprintf(temp,"TO [%s][CREDIT CODE]",tmp);
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
		temp=Rmalloc(17);
		sprintf(temp,"TO [CREDIT CODE]");
		x=FINDRSC(rsrc,temp);
		if(temp!=NULL) Rfree(temp);
		if(x!=(-1))
		{
			mem=rsrc->rscs+x;
			setacctype(mem,*member->value.short_value);
		}
#endif /* XXXX */
		if(tmp!=NULL) Rfree(tmp);
	}
	}
}
char *gfname(char *filename,char *rname)
{
	char *mod=NULL,*tmp=NULL;

	mod=stripmodulename(rname);
	if(!RDAstrcmp(mod,filename)) tmp=stripfilename(rname);
	if(mod!=NULL) Rfree(mod);
	return tmp;
}
/*-----------------------------------------------------------------------------
	filerecord2rsrc
		Function to take current record contents and fill in 
		the value for defined resources.
-----------------------------------------------------------------------------*/
void xfilerecord2rsrc(short fileno,RDArsrc *rsrc,short type,int line,char *file)
{
	short x;
	NRDfield *field=NULL;
	RDArmem *res;
	char *filename=NULL,*fieldname=NULL;
	short edit_rsrc=FALSE;

	if(!TESTFILENO(fileno,line,file)) return;
	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG filerecord2rsrc Setting resource values on Screen [%s] [%s] corresponding to fields in module [%s] file [%d] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,MODULENAME(fileno),fileno,filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(filename!=NULL)
	{
	for(x=0,res=rsrc->rscs;x<rsrc->numrscs;++x,++res)
	{
		if(!type)
		{
			fieldname=gfname(filename,res->rscrname);
			if(fieldname!=NULL)
			{
				field=FLDNRD(fileno,fieldname);
				if(!RDAstrcmp(fieldname,"DELETEFLAG"))
				{
					FIELDscrn(fileno,fieldname,&edit_rsrc);
					FINDRSCSETEDITABLE(rsrc,res->rscrname,(edit_rsrc ? TRUE:FALSE));
				}
				Rfree(fieldname);
			} else field=NULL;
		} else {
			field=FLDNRD(fileno,res->rscrname);
			if(!RDAstrcmp(res->rscrname,"DELETEFLAG"))
			{
				FIELDscrn(fileno,res->rscrname,&edit_rsrc);
				FINDRSCSETEDITABLE(rsrc,res->rscrname,(edit_rsrc ? TRUE:FALSE));
			}
		}
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(field->data.string_value))
					{
						QUICKALLOC(res->value.string_value,res->dlen,field->dlen);
						memcpy(res->value.string_value,field->data.string_value,field->dlen);
					} else if(res->dlen>0)
					{
						memset(res->value.string_value,0,res->dlen);
					} else {
						res->value.string_value=NULL;
						res->dlen=0;
					}
					break;
				case SHORTV:
				case SSHORTV:
					*res->value.short_value=*field->data.short_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*res->value.integer_value=*field->data.integer_value;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					*res->value.float_value=*field->data.float_value;
					break;
				case BOOLNS:
				case CHARACTERS:
					*res->value.string_value=
						*field->data.string_value;
					break;
				default:
					prterr("Error Field type [%d] is invalid for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),fileno,filename,line,file); 
					break;
			}
		}
	}
	}
}
/*-----------------------------------------------------------------------------
	rsrc2filerecord	
		Function to take current resource contents and fill in 
		the values in the current record of the corresponding file.
-----------------------------------------------------------------------------*/
void xrsrc2filerecord(short fileno,RDArsrc *rsrc,short type,int line,char *file)
{
	int x;
	NRDfield *field=NULL;
	RDArmem *res;
	char *filename=NULL,*fieldname=NULL;
	
	if(!TESTFILENO(fileno,line,file)) return;
	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG rsrc2filerecord Setting field structures in Module [%s] File [%d] [%s] from values in Screen [%s] [%s]'s Resources at line [%d] program [%s].",MODULENAME(fileno),fileno,filename,rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(filename!=NULL)
	{
	for(x=0,res=rsrc->rscs;x<rsrc->numrscs;++x,++res)
	{
		if(!type)
		{
			fieldname=gfname(filename,res->rscrname);
			if(fieldname!=NULL) 
			{
/*
				field=FLDNRDsec(fileno,fieldname,TRUE,TRUE,FALSE);
*/
				field=FLDNRD(fileno,fieldname);
				Rfree(fieldname);
			} else field=NULL;
		} else {
/*
			field=FLDNRDsec(fileno,res->rscrname,TRUE,TRUE,FALSE);
*/
			field=FLDNRD(fileno,res->rscrname);
		}
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(res->value.string_value))
					{
						unpad(res->value.string_value);
						QUICKALLOC(field->data.string_value,field->dlen,res->dlen);
						memcpy(field->data.string_value,res->value.string_value,res->dlen);
						if(field->dlen>field->len && field->len>0)
						{
							field->data.string_value[field->len]=0;
						}
					} else if(field->dlen>0)
					{
						memset(field->data.string_value,0,field->dlen);
					} else {
						field->data.string_value=NULL;
						field->dlen=0;
					}
					break;
				case SHORTV:
				case SSHORTV:
					*field->data.short_value=
						*res->value.short_value;
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					*field->data.integer_value=
						*res->value.integer_value;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					*field->data.float_value=
						*res->value.float_value;
					break;
				case BOOLNS:
				case CHARACTERS:
					*field->data.string_value=
						*res->value.string_value;
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),fileno,filename,line,file); 
					break;
			}
		}
	}
	}
}
void ShutdownOnError()
{
	ShutdownSubsystems();
	std::exit;
}
short DoPROCsecurity(char *module,char *process)
{
	SET_SECURITY_USERLOGIN();
	if(RDA_GROUP!=NULL) Rfree(RDA_GROUP);
	RDA_GROUP=Users_Department();
	if(!PROCsecurity(module,process)) 
	{
#ifdef XXXX
		if(RDAstrcmp(USERLOGIN,"GUEST"))
		{
			USERLOGIN=stralloc("GUEST");
			SET_SECURITY_USERLOGIN();
			if(RDA_GROUP!=NULL) Rfree(RDA_GROUP);
			RDA_GROUP=Users_Department();
			if(PROCsecurity(module,process)) return(TRUE);
		}
#endif /* XXXX */
	} else return(TRUE);	 
	return(FALSE);
}
short DoInitializeSecurity()
{
	if(InitializeSecurity())
	{	
		if(RDAstrcmp(USERLOGIN,"root"))
		{
			USERLOGIN=stralloc("root");
		}
		return(InitializeSecurity());
	} else return(FALSE); 
}
#ifdef __GTK_H__
static void SetGtkRC()
{
	char *XPERTRC=NULL,*temp=NULL;

	if(RDAstrcmp(CURRENTDIRECTORY,XPERT_SETUP->DataDir)) return;
	if(SEC_USERS_FILENO!=(-1))
	{
		ZERNRD(SEC_USERS_FILENO);
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"GTKRC",&XPERTRC);
			if(!isEMPTY(XPERTRC))
			{
#ifndef WIN32
				temp=Rmalloc(RDAstrlen(XPERTRC)+30);
				sprintf(temp,"./rda/%s",XPERTRC);
				gtk_rc_parse(temp);
				sprintf(temp,"./site/%s",XPERTRC);
				gtk_rc_parse(temp);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen(XPERTRC)+30);
				sprintf(temp,"./%s/%s",(USERLOGIN!=NULL ? USERLOGIN:""),XPERTRC);
				gtk_rc_parse(temp);
				if(temp!=NULL) Rfree(temp);
#endif /* Not WIN32 */
#ifdef WIN32
				temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(XPERTRC)+14);
				sprintf(temp,"%s\\rda\\%s",CURRENTDIRECTORY,XPERTRC);
				gtk_rc_parse(temp);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(XPERTRC)+14);
				sprintf(temp,"%s\\site\\%s",CURRENTDIRECTORY,XPERTRC);
				gtk_rc_parse(temp);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+RDAstrlen(XPERTRC)+14);
				sprintf(temp,"%s\\%s\\%s",CURRENTDIRECTORY,(USERLOGIN!=NULL ? USERLOGIN:""),XPERTRC);
				gtk_rc_parse(temp);
				if(temp!=NULL) Rfree(temp);
#endif /* WIN32 */
			}
		}
	}
} 
#endif /* __GTK_H__ */
short xInitializeSubsystems(int argc,char **argv,char *module,char *process,int line,char *file)
{
	char *desc=NULL;
	short x=0;
#ifdef RDA_LICENSE
	short y=0;
#endif /* RDA_LICENSE */
	short override_rdalic=FALSE;
	short return_value=0;
	long longtemp=0;
	char *libx=NULL,*temp=NULL,*temp1=NULL,*MyDir2Use=NULL;
	RDAGenericSetup *gsv=NULL;

	initrdadiag();

#ifdef USE_CPU_PROFILER
	ProfilerStart("output.prof");
#endif 

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG InitializeSubsystems Initializing Subsystems for Module [%s] Process [%s] at line [%d] program [%s].",module,process,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	SETUSERLOGIN();
	SETCWD();
	desc=RDA_GetEnv("RDA_CURRENCY_SYMBOL");
	if(!isEMPTY(desc))
	{
		longtemp=strtol(desc,NULL,0);
		RDA_CURRENCY_SYMBOL=(char)longtemp;
	} else {
		RDA_CURRENCY_SYMBOL='$';
	}
#ifdef WIN32
	Pay_Balance_of_Contract=FALSE;
	Dump_Deferred_Pay_Balance=FALSE;
#endif
	XPERT_SETUP=XPERTstpNEW();
	x=getXPERTbin(XPERT_SETUP);
	MyDir2Use=XPERT_SETUP->DataDir;
	temp=RDA_GetEnv("DEV_LICENSE");
	if(temp!=NULL)
	{
		temp1=strtok(temp," ");
		if(CheckDevLicense(temp1)) MyDir2Use=CURRENTDIRECTORY;
	}
	INITGUI(argc,argv,MyDir2Use);
	PP_translate_GUIFUNC=PP_translate_GUI;
	PP_translate_GUIFUNCALL=PP_translate_GUIALL;
	override_rdalic=FALSE;
	if(x==(-1))
	{
		prterr("ERROR: Error Invalid Version or Bad Binary for [XPERT SETUP] from Module: [%s] Process: [%s] at line [%d] program [%s].",module,process,line,file);
		ERRORDIALOG("INVALID BINARY VERSION!","The Xpert/MBGUI Setup binary is invalid or missing.",ShutdownOnError,TRUE);
		return_value=(-1);
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagapps)
		{
			prterr("DIAG XPERT.STP version=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",x,module,process,line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
	}
	if(XPERT_SETUP->OPENSOURCE==TRUE) override_rdalic=TRUE;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","USE SPEECH SYNTHESIZER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			USE_SPEECH_SYNTHESIZER=*gsv->value.string_value;
		} else {
			USE_SPEECH_SYNTHESIZER=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","CENTER WINDOWS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			CENTER_WINDOWS=*gsv->value.string_value;
		} else {
			CENTER_WINDOWS=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(!RDAstrcmp(process,"XPERT SETUP"))
	{
		desc=RDA_GetEnv("DEV_LICENSE");
		if(desc!=NULL)
		{
			if(CheckDevLicense(desc))
			{
				override_rdalic=TRUE;
			}
		}
	}
#ifdef RDA_LICENSE
	if(RDAstrcmp(USERLOGIN,"root"))
	{
	if(override_rdalic==FALSE)
	{
	}
	} else {
		override_rdalic=TRUE;
		getXPERTbin(XPERT_SETUP);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagapps)
		{
			prterr("DIAG XPERT.STP version=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",x,module,process,line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
	}
	if(!override_rdalic) y=INITRDALIC();
		else y=0;
	if(y!=0 && override_rdalic==FALSE)
	{
		prterr("ERROR: Cannot Initialize the License SubSystem at line [%d] program [%s].",line,file);
		ERRORDIALOG("CANNOT INITIALIZE LICENSE SUBSYSTEM!","The Licensing SubSystem reports no more Licenses available.\nPlease call your installer for support.",ShutdownOnError,TRUE);
		return_value=(-1);
		
	} else 
#endif
#ifdef USE_MYSQL
	INITNRD_MYSQL_GSVS();
#endif /* USE_MYSQL */
	if(INITNRD()) 
	{
		prterr("ERROR: Cannot Initialize the Database Subsystem at line [%d] program [%s].",line,file);
		ERRORDIALOG("CANNOT INITIALIZE DATABASE SUBSYSTEM!","The Database SubSystem cannot be initialized.\nPlease call your installer for support.",ShutdownOnError,TRUE);
		return_value=(-1);
		return(return_value);
	}
	if(DoInitializeSecurity()) 
	{
		prterr("Error Cannot Initialize the Security Subsystem at line [%d] program [%s].",line,file);
		ERRORDIALOG("CANNOT INITIALIZE SECURITY SUBSYSTEMS!","The Security SubSystem cannot be initialized.\nPlease call your installer for support.",ShutdownOnError,TRUE);
		return_value=(-1);
	} else if(!DoPROCsecurity(module,process)) 
	{
		prterr("ERROR: Security Access to Module [%s] Process [%s] DENIED! at line [%d] program [%s].",module,process,line,file);
		desc=Rmalloc(RDAstrlen(module)+RDAstrlen(process)+111);
		sprintf(desc,"The Security SubSystem denied you access to Module [%s] Process [%s].\nPlease call your installer for support.",module,process);
		ERRORDIALOG("SECURITY ACCESS DENIED!",desc,ShutdownOnError,TRUE);
		Rfree(desc);
		return_value=(-1);
	} else {
		if(RDA_SOFTWARE_TYPE!=(-1))
		{
			if(RDA_SOFTWARE_TYPE!=XPERT_SETUP->software_type && !(RDA_SOFTWARE_TYPE==3 && XPERT_SETUP->software_type<2))
			{ 
				prterr("ERROR: Invalid RDA Software Type of [%d] for application from Module: [%s] Process: [%s] at line [%d] program [%s].",RDA_SOFTWARE_TYPE,module,process,line,file);
				desc=Rmalloc(10+RDAstrlen(module)+RDAstrlen(process)+141);
				sprintf(desc,"Invalid RDA Software Type of [%2d] for application from Module: [%s]  Process: [%s], Access Denied.\nPlease call your installer for support.",RDA_SOFTWARE_TYPE,module,process);
				ERRORDIALOG("INVALID SOFTWARE TYPE!",desc,ShutdownOnError,TRUE);
				Rfree(desc);
				return_value=(-1);
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diagmix || diagapps)
				{
					prterr("DIAG RDA_SOFTWARE_TYPE=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",RDA_SOFTWARE_TYPE,module,process,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
			}
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagmix || diagapps)
			{
				prterr("DIAG RDA_SOFTWARE_TYPE=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",RDA_SOFTWARE_TYPE,module,process,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
		}
		if(RDA_NOGUI==FALSE)
		{
#ifdef __GTK_H__
			SetGtkRC();
#endif /* __GTK_H__ */
			ADVINITHELP(GUIEVAL,addoswidgets);
		}
		INITTRANS();
		OPNLOG(module,process);
	}
	if(UsersAutoComplete())
	{
		GUI_AUTO_COMPLETE=OPNNRD("GUI","AUTOCOMP");
		RDA_AutoComplete=OpenRDA_AutoComplete;
		RDA_CaptureAutoComplete=OpenRDA_CaptureAutoComplete;
	} else {
		GUI_AUTO_COMPLETE=(-1);
		RDA_AutoComplete=NULL;
		RDA_CaptureAutoComplete=NULL;
	}
	if(!RDA_NOGUI)
	{
		USE_BROWSELISTLABEL=Users_BrowseListLabel();
		VENPMT_SIGNATURE_FILE=UsersVenpmtSignatureFilename();
		VENPMT_PASSWORD=UsersVenpmtSignaturePassword();
		PAYROLL_SIGNATURE_FILE=UsersPayrollSignatureFilename();
		PAYROLL_PASSWORD=UsersPayrollSignaturePassword();
		PURORD_SIGNATURE_FILE=UsersPurordSignatureFilename();
		PURORD_PASSWORD=UsersPurordSignaturePassword();
		PUSHBUTTON_STYLE=UsersPushButtonStyle();
	}
	return(return_value);
}
void xShutdownSubsystems(int line,char *file)
{
#ifdef USE_CPU_PROFILER
	ProfilerStop();
#endif 

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG ShutdownSubsystems Stopping Help, Security, Transaction Processing, etc at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef RDA_LICENSE
	ShutdownRDALIC();
#endif /* RDA_LICENSE */
	ShutdownSecurity();
	EXITHELP();
	CLSLOG();
	EXITNRD();
	EXITGUI();
	if(VENPMT_SIGNATURE_FILE!=NULL) Rfree(VENPMT_SIGNATURE_FILE);
	if(VENPMT_PASSWORD!=NULL) Rfree(VENPMT_PASSWORD);
	if(PAYROLL_SIGNATURE_FILE!=NULL) Rfree(PAYROLL_SIGNATURE_FILE);
	if(PAYROLL_PASSWORD!=NULL) Rfree(PAYROLL_PASSWORD);
	if(PURORD_SIGNATURE_FILE!=NULL) Rfree(PURORD_SIGNATURE_FILE);
	if(PURORD_PASSWORD!=NULL) Rfree(PURORD_PASSWORD);
	if(CURRENTDIRECTORY!=NULL) Rfree(CURRENTDIRECTORY);
	if(CURRENT_DATE!=NULL) Rfree(CURRENT_DATE);
	if(CURRENT_DATE10!=NULL) Rfree(CURRENT_DATE10);
	if(CURRENT_TIME!=NULL) Rfree(CURRENT_TIME);
	if(RDA_GROUP!=NULL) Rfree(RDA_GROUP);
	free_xpert(XPERT_SETUP);
#ifdef WIN32
	if(RDA_STDERR!=NULL) fclose(RDA_STDERR);
#endif
}
char *xunique_name(char *s,int line,char *file)
{
	char *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG unique_name Returns a Unique Name using the login identification and process number (use as sortfile names) at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(s))
	{
		tmp=Rmalloc(RDAstrlen(s)+10);
		sprintf(tmp,"%d%s",RGETPID(),s);
	} else {
		tmp=Rmalloc(10);
		sprintf(tmp,"%d",RGETPID());
	}
	return(tmp);
}
short xAPPOPNNRDEDITABLE(char *mod,char *fil,char *editable,int line,char *file)
{
	char *edesc=NULL;
	short fileno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG APPOPNNRDEDITABLE Opening Module [%s] File [%s] at line [%d] program [%s].",mod,fil,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if((fileno=OPNNRDsec(mod,fil,TRUE,TRUE,FALSE))==(-1)) 
	{
		if((fileno=OPNNRDsec(mod,fil,TRUE,FALSE,FALSE))==(-1)) 
		{
			edesc=Rmalloc(82+RDAstrlen(mod)+RDAstrlen(fil));
			sprintf(edesc,"Security Access denied to the [%s][%s] file or the file cannot otherwise be opened.\n",mod,fil);
			ERRORDIALOG("Security Access Denied",edesc,ShutdownOnError,TRUE);
			if(edesc!=NULL) Rfree(edesc);
			RDAAPPMAINLOOP();
			return(-1);
		}
		*editable=FALSE;
		return(fileno);
	}
	return(fileno);
}
short xAPPOPNNRD(char *mod,char *fil,short read,short write,int line,char *file)
{
	char *edesc=NULL;
	short fileno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG APPOPNNRD Opening Module [%s] File [%s] at line [%d] program [%s].",mod,fil,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if((fileno=OPNNRDsec(mod,fil,read,write,FALSE))==(-1)) 
	{
		edesc=Rmalloc(82+RDAstrlen(mod)+RDAstrlen(fil));
		sprintf(edesc,"Security Access denied to the [%s][%s] file or the file cannot otherwise be opened.\n",mod,fil);
		ERRORDIALOG("Security Access Denied",edesc,ShutdownOnError,TRUE);
		if(edesc!=NULL) Rfree(edesc);
		RDAAPPMAINLOOP();
		return(-1);
	}
	return(fileno);
}
short xAPPmakescrn(RDArsrc *mainrsrc,short modal,void (*quitfunc)(...),void *arg,short exit,int line,char *file)
{
	char *edesc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG APPmakescrn Making Screen for [%s] [%s] at line [%d] program [%s].",mainrsrc->module,mainrsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(ADVmakescrn(mainrsrc,modal))
	{
		edesc=Rmalloc(180+RDAstrlen(mainrsrc->module)+RDAstrlen(mainrsrc->screen));
		sprintf(edesc,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",(mainrsrc->module==NULL?"":mainrsrc->module),(mainrsrc->screen==NULL?"":mainrsrc->screen));
		ERRORDIALOG("MAKESCRN FAILED",edesc,ShutdownOnError,(exit ? TRUE:FALSE));
		if(edesc!=NULL) Rfree(edesc);
/*lint -e746 */
		if(quitfunc!=NULL) quitfunc(mainrsrc,arg);
/*lint +e746 */
		return(-1);
	}
	return(0);
}
short xCOMPARE_RSRCS(RDArsrc *rsrc,RDAdefault *d,short listtype,int line,char *file)
{
	int x;
	DEFfield *field;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG COMPARE_RSRCS for Screen [%s] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL && d!=NULL)
	{
		for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
		{
			field=FINDFIELDDEFAULT(d,member->rscrname);
			if(field!=NULL)
			{
				switch(member->field_type)
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
						if(RDAstrcmp(field->data.string_value,member->value.string_value)) return(1);
						break;
					case BOOLNS:
					case CHARACTERS:
						if(*field->data.string_value!=*member->value.string_value) return(1);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						if(*field->data.float_value!=*member->value.float_value) return(1);
						break;
					case SHORTV:
					case SSHORTV:
						if(*field->data.short_value!=*member->value.short_value) return(1);
						break;
					case SCROLLEDLIST:
						switch(listtype)
						{
							default:
							case 0:
								if(field->items!=member->items) return(1);
								break;
							case 1:
								if(*field->data.integer_value!=*member->value.integer_value) return(1);
								break;
							case 2:
								if(*field->data.integer_value!=*member->value.integer_value || member->items!=field->items) return(1);
								break;
						}
						break;
					case LONGV:
					case SLONGV:
						if(*field->data.integer_value!=*member->value.integer_value) return(1);
						break;
					default:
						prterr("Error RDArmem's field type [%d] not found for member [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->field_type,member->rscrname,rsrc->module,rsrc->screen,line,file);
						break;
				}
			}
		}
		return(0);
	} 
	prterr("Error RDArsrc and/or RDAdefault not found at line [%d] program [%s].",line,file);
	return(-1);
}
/*-----------------------------------------------------------------------------
	updatefilerecord2rsrc
		Function to take current record contents and fill in 
		the value for defined resources, and update member.
-----------------------------------------------------------------------------*/
void xupdatefilerecord2rsrc(short fileno,RDArsrc *rsrc,short type,int line,char *file)
{
	short x;
	NRDfield *field=NULL;
	RDArmem *res;
	char *filename=NULL,*fieldname=NULL;
	short edit_rsrc=0;

	if(!TESTFILENO(fileno,line,file)) return;
	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG updatefilerecord2rsrc Setting & updating resource values on Screen [%s] [%s] corresponding to fields in module [%s] file [%d] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,MODULENAME(fileno),fileno,filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(filename!=NULL)
	{
	for(x=0,res=rsrc->rscs;x<rsrc->numrscs;++x,++res)
	{
		if(!type)
		{
			fieldname=gfname(filename,res->rscrname);
			if(fieldname!=NULL)
			{
				field=FLDNRD(fileno,fieldname);
				if(!RDAstrcmp(fieldname,"DELETEFLAG"))
				{
					FIELDscrn(fileno,fieldname,&edit_rsrc);
					FINDRSCSETEDITABLE(rsrc,res->rscrname,(edit_rsrc ? TRUE:FALSE));
				}
				Rfree(fieldname);
			} else field=NULL;
		} else {
			field=FLDNRD(fileno,res->rscrname);
			if(!RDAstrcmp(res->rscrname,"DELETEFLAG"))
			{
				FIELDscrn(fileno,res->rscrname,&edit_rsrc);
				FINDRSCSETEDITABLE(rsrc,res->rscrname,(edit_rsrc ? TRUE:FALSE));
			}
		}
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(field->data.string_value))
					{
						QUICKALLOC(res->value.string_value,res->dlen,field->dlen);
						memcpy(res->value.string_value,field->data.string_value,field->dlen);
					} else if(res->dlen>0)
					{
						memset(res->value.string_value,0,res->dlen);
					} else {
						res->value.string_value=NULL;
						res->dlen=0;
					}
					break;
				case SHORTV:
				case SSHORTV:
					*res->value.short_value=*field->data.short_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*res->value.integer_value=*field->data.integer_value;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					*res->value.float_value=*field->data.float_value;
					break;
				case BOOLNS:
				case CHARACTERS:
					*res->value.string_value=
						*field->data.string_value;
					break;
				default:
					prterr("Error Field type [%d] is invalid for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),fileno,filename,line,file); 
					break;
			}
			updatemember(res);
		}
	}
	}
}
void AddFileFields(APPlib *list,short fileno)
{
	int x,numfields=0;
	NRDfield *field=NULL,*fields=NULL;
	char *tmp=NULL,*mod,*fil;

	fields=FLDPOINTER(fileno);
	numfields=NUMFLDS(fileno);
	if(fields!=NULL)
	{
		mod=MODULENAME(fileno);
		fil=FILENAME(fileno);
		for(x=0,field=fields;x<numfields;++x,++field)
		{
			tmp=Rmalloc(RDAstrlen(mod)+RDAstrlen(fil)+RDAstrlen(field->name)+7);
			sprintf(tmp,"[%s][%s][%s]",mod,fil,field->name);
			addAPPlib(list,tmp);
			if(tmp!=NULL) Rfree(tmp);
		}
	}
}
int xFORMAT_MEDIA(char *device_spec,int line,char *program)
{
	APPlib *args=NULL;
	int x=(-1);
	char *temp=NULL;

	if(!isEMPTY(device_spec))
	{
		memset(stemp,0,101);
#ifdef WIN32
		args=APPlibNEW();
		addAPPlib(args,device_spec);
/*
		temp=Rmalloc(RDAstrlen(device_spec)+7+1);
		sprintf(temp,"format.com %s",device_spec);
		x=system_command(temp);
		if(temp!=NULL) Rfree(temp);
*/
		x=ADV3Execute2Program("format.com",args,NULL,FALSE,FALSE);
#endif
#ifdef RS6000
/*
		sprintf(stemp,"-D %s",device_spec);
		args=APPlibNEW();
		addAPPlib(args,"-D");
		addAPPlib(args,device_spec);
		x=ADV2Execute2Program("dosformat",args,NULL,FALSE);
*/
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(device_spec)+22+1);
		sprintf(temp,"/usr/bin/dosformat -D %s",device_spec);
		memset(stemp,0,101);
		sprintf(stemp,"DOS FORMATTING -- %s",temp);
		args=APPlibNEW();
		addAPPlib(args,"-ls");
		addAPPlib(args,"-T");
		addAPPlib(args,stemp);
		addAPPlib(args,"-fn");
		addAPPlib(args,"6x13");
		addAPPlib(args,"-cu");
		addAPPlib(args,"-cr");
		addAPPlib(args,"red");
		addAPPlib(args,"-fullcursor");
		addAPPlib(args,"-e");
		addAPPlib(args,"/usr/bin/dosformat");
		addAPPlib(args,"-D");
		addAPPlib(args,device_spec);
/*
		addAPPlib(args,temp);
*/
		x=ADV2Execute2Program("/usr/bin/X11/aixterm",args,NULL,FALSE);
		if(temp!=NULL) Rfree(temp);
#endif
#ifdef SCO50P
/*
		args=APPlibNEW();
		addAPPlib(args,device_spec);
		x=ADV2Execute2Program("dosformat",args,NULL,FALSE);
*/
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(device_spec)+22+1);
		sprintf(temp,"/usr/bin/dosformat -v %s",device_spec);
		memset(stemp,0,101);
		sprintf(stemp,"DOS FORMATTING -- %s",temp);
		args=APPlibNEW();
		addAPPlib(args,"-blink");
		addAPPlib(args,"-j");
		addAPPlib(args,"-ls");
		addAPPlib(args,"-menu");
		addAPPlib(args,"-rw");
		addAPPlib(args,"-sb");
		addAPPlib(args,"-si");
		addAPPlib(args,"-sk");
		addAPPlib(args,"-wf");
		addAPPlib(args,"-r");
		addAPPlib(args,"-T");
		addAPPlib(args,stemp);
		addAPPlib(args,"-geometry");
		addAPPlib(args,"85x25");
		addAPPlib(args,"-e");
		addAPPlib(args,"/usr/bin/dosformat");
		addAPPlib(args,"-v");
		addAPPlib(args,device_spec);
/*
		addAPPlib(args,temp);
*/
		x=ADV2Execute2Program("/usr/bin/X11/scoterm",args,NULL,FALSE);
		if(temp!=NULL) Rfree(temp);
#endif
#ifdef UNIXWARE21
/*
		args=APPlibNEW();
		addAPPlib(args,device_spec);
		x=ADV2Execute2Program("dosformat",args,NULL,FALSE);
*/
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(device_spec)+22+1);
		sprintf(temp,"/usr/bin/dosformat -v %s",device_spec);
		memset(stemp,0,101);
		sprintf(stemp,"DOS FORMATTING -- %s",temp);
		args=APPlibNEW();
		addAPPlib(args,"-ls");
		addAPPlib(args,"-T");
		addAPPlib(args,stemp);
		addAPPlib(args,"-geometry");
		addAPPlib(args,"85x25");
		addAPPlib(args,"-e");
		addAPPlib(args,"/usr/bin/dosformat");
		addAPPlib(args,"-v");
		addAPPlib(args,device_spec);
/*
		addAPPlib(args,temp);
*/
		x=ADV2Execute2Program("/usr/bin/X11/xterm",args,NULL,FALSE);
		if(temp!=NULL) Rfree(temp);
#endif
#ifdef UNIXWARE7
/*
		args=APPlibNEW();
		addAPPlib(args,device_spec);
		x=ADV2Execute2Program("dosformat",args,NULL,FALSE);
*/
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(device_spec)+22+1);
		sprintf(temp,"/usr/bin/dosformat -v %s",device_spec);
		memset(stemp,0,101);
		sprintf(stemp,"DOS FORMATTING -- %s",temp);
		args=APPlibNEW();
		addAPPlib(args,"-ls");
		addAPPlib(args,"-T");
		addAPPlib(args,stemp);
		addAPPlib(args,"-geometry");
		addAPPlib(args,"85x25");
		addAPPlib(args,"-e");
		addAPPlib(args,"/usr/bin/dosformat");
		addAPPlib(args,"-v");
		addAPPlib(args,device_spec);
/*
		addAPPlib(args,temp);
*/
		x=ADV2Execute2Program("/usr/bin/X11/xterm",args,NULL,FALSE);
		if(temp!=NULL) Rfree(temp);
#endif
#ifdef LINUX
		args=APPlibNEW();
		addAPPlib(args,"-t");
		addAPPlib(args,"80");
		addAPPlib(args,"-h");
		addAPPlib(args,"2");
		addAPPlib(args,"-s");
		addAPPlib(args,"18");
		addAPPlib(args,device_spec);
		x=ADV2Execute2Program("mformat",args,NULL,FALSE);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		args=APPlibNEW();
		addAPPlib(args,"-t");
		addAPPlib(args,"80");
		addAPPlib(args,"-h");
		addAPPlib(args,"2");
		addAPPlib(args,"-s");
		addAPPlib(args,"18");
		addAPPlib(args,device_spec);
		x=ADV2Execute2Program("mformat",args,NULL,FALSE);
#endif
#ifdef SUNX86
/*
		args=APPlibNEW();
		addAPPlib(args,device_spec);
		x=ADV2Execute2Program("dosformat",args,NULL,FALSE);
*/
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(device_spec)+22+1);
		sprintf(temp,"/usr/bin/fdformat -v -t dos %s",device_spec);
		memset(stemp,0,101);
		sprintf(stemp,"DOS FORMATTING -- %s",temp);
		args=APPlibNEW();
		addAPPlib(args,"-ls");
		addAPPlib(args,"-T");
		addAPPlib(args,stemp);
		addAPPlib(args,"-geometry");
		addAPPlib(args,"85x25");
		addAPPlib(args,"-e");
		addAPPlib(args,"/usr/bin/dosformat");
		addAPPlib(args,"-v");
		addAPPlib(args,device_spec);
/*
		addAPPlib(args,temp);
*/
		x=ADV2Execute2Program("/usr/bin/X11/xterm",args,NULL,FALSE);
		if(temp!=NULL) Rfree(temp);
#endif
		if(args!=NULL) freeapplib(args);
		if(x!=0)
		{
			temp=Rmalloc(RDAstrlen(device_spec)+37+1);
			sprintf(temp,"FORMAT ERROR: Can Not Format Device %s!",device_spec);
			prterr(temp);
			if(temp!=NULL) Rfree(temp);
			return(-2);
		} 
	} else {
		return(-1);
	}
	return(0);
}
int xCOPY_FILE(char *from_filename,char *to_filename,char *device_spec,int line,char *program)
{
	int x=(-1);
	short modal=TRUE;
	APPlib *args=NULL;
	char *temp=NULL;
#ifdef WIN32
	char *error_message=NULL;
#endif
#ifdef UNIXWARE21
	RDArsrc *tmprsrc=NULL;
#endif

	if(!isEMPTY(from_filename)) 
	{
	if(!isEMPTY(to_filename))
	{
	memset(stemp,0,101);
#ifdef WIN32
	sprintf(stemp,"%s%s",device_spec,to_filename);
	args=APPlibNEW();
/*
	addAPPlib(args,from_filename);
	addAPPlib(args,stemp);
	addAPPlib(args,"\/V");
*/
/*
	x=ADV2Execute2Program("xcopy",args,NULL,FALSE);
*/
/*
	x=CopyFileEx(from_filename,stemp,NULL,NULL,FALSE,0);
*/
/*
	sprintf(stemp,"copy %s %s%s",from_filename,device_spec,to_filename);
	x=system_command(stemp);
*/
	x=CopyFile(from_filename,stemp,FALSE);
#endif
#ifdef RS6000
/*
	sprintf(stemp,"-D%s",device_spec);
	args=APPlibNEW();
	addAPPlib(args,stemp);
	addAPPlib(args,from_filename);
	addAPPlib(args,to_filename);
	x=ADV2Execute2Program("doswrite",args,NULL,FALSE);
*/
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(device_spec)+RDAstrlen(from_filename)+RDAstrlen(to_filename)+22+1);
	sprintf(temp,"/usr/bin/doswrite -D%s %s %s",device_spec,from_filename,to_filename);
	memset(stemp,0,101);
	sprintf(stemp,"DOS COPYING -- %s",temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(device_spec)+2+1);
	sprintf(temp,"-D%s",device_spec);
	args=APPlibNEW();
	addAPPlib(args,"-ls");
	addAPPlib(args,"-T");
	addAPPlib(args,stemp);
	addAPPlib(args,"-fn");
	addAPPlib(args,"6x13");
	addAPPlib(args,"-cu");
	addAPPlib(args,"-cr");
	addAPPlib(args,"red");
	addAPPlib(args,"-fullcursor");
	addAPPlib(args,"-e");
	addAPPlib(args,"/usr/bin/doswrite");
	addAPPlib(args,temp);
	addAPPlib(args,from_filename);
	addAPPlib(args,to_filename);
/*
	addAPPlib(args,temp);
*/
	x=ADV2Execute2Program("/usr/bin/X11/aixterm",args,NULL,FALSE);
	if(temp!=NULL) Rfree(temp);
	
#endif
#ifdef SCO50P  
/*
	sprintf(stemp,"%s:%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,stemp);
	x=ADV2Execute2Program("doscp",args,NULL,FALSE);
*/
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(device_spec)+RDAstrlen(to_filename)+20+1);
	sprintf(temp,"/usr/bin/doscp -r %s %s:%s",from_filename,device_spec,to_filename);
	memset(stemp,0,101);
	sprintf(stemp,"DOS COPYING -- %s",temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(device_spec)+1+RDAstrlen(to_filename)+1);
	sprintf(temp,"%s:%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-blink");
	addAPPlib(args,"-j");
	addAPPlib(args,"-ls");
	addAPPlib(args,"-menu");
	addAPPlib(args,"-rw");
	addAPPlib(args,"-sb");
	addAPPlib(args,"-si");
	addAPPlib(args,"-sk");
	addAPPlib(args,"-wf");
	addAPPlib(args,"-r");
	addAPPlib(args,"-T");
	addAPPlib(args,stemp);
	addAPPlib(args,"-geometry");
	addAPPlib(args,"85x25");
	addAPPlib(args,"-e");
	addAPPlib(args,"/usr/bin/doscp");
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,temp);
	x=ADV2Execute2Program("/usr/bin/X11/scoterm",args,NULL,FALSE);
	if(temp!=NULL) Rfree(temp);
#endif
#ifdef UNIXWARE21
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","GUI","Copying DOS File",NULL,1);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,modal,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
	}
	sprintf(stemp,"%s:%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,stemp);
	x=ADV2Execute2Program("doscp",args,NULL,FALSE);
	if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
/* Xterm on Unixware doesn't process -e commands correctly when they have 
   more than 1 argument, so dosformat works but doscp won't */
/*
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(device_spec)+RDAstrlen(to_filename)+20+1);
	sprintf(temp,"`/usr/bin/doscp -r %s %s:%s`",from_filename,device_spec,to_filename);
	memset(stemp,0,101);
	sprintf(stemp,"DOS COPYING -- %s",temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(device_spec)+2+RDAstrlen(to_filename)+1);
	sprintf(temp,"%s:%s`",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-ls");
	addAPPlib(args,"-T");
	addAPPlib(args,stemp);
	addAPPlib(args,"-geometry");
	addAPPlib(args,"85x25");
	addAPPlib(args,"-e");
	addAPPlib(args,"`/usr/bin/doscp");
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,temp);
	x=ADV2Execute2Program("/usr/bin/X11/xterm",args,NULL,FALSE);
	if(temp!=NULL) Rfree(temp);
*/
#endif
#ifdef UNIXWARE7
	sprintf(stemp,"%s:%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,stemp);
	x=ADV2Execute2Program("doscp",args,NULL,FALSE);
	if(temp!=NULL) Rfree(temp);
/*
	temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(device_spec)+RDAstrlen(to_filename)+20+1);
	sprintf(temp,"/usr/bin/doscp -r %s %s:%s",from_filename,device_spec,to_filename);
	memset(stemp,0,101);
	sprintf(stemp,"DOS COPYING -- %s",temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(device_spec)+1+RDAstrlen(to_filename)+1);
	sprintf(temp,"%s:%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-ls");
	addAPPlib(args,"-T");
	addAPPlib(args,stemp);
	addAPPlib(args,"-geometry");
	addAPPlib(args,"85x25");
	addAPPlib(args,"-e");
	addAPPlib(args,"/usr/bin/doscp");
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,temp);
	x=ADV2Execute2Program("/usr/bin/X11/xterm",args,NULL,FALSE);
	if(temp!=NULL) Rfree(temp);
*/
#endif
#ifdef LINUX
	sprintf(stemp,"%s%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,from_filename);
	addAPPlib(args,stemp);
	x=ADV2Execute2Program("mcopy",args,NULL,FALSE);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(stemp,"%s%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,from_filename);
	addAPPlib(args,stemp);
	x=ADV2Execute2Program("mcopy",args,NULL,FALSE);
#endif
#ifdef SUNX86
/*
	sprintf(stemp,"%s:%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,stemp);
	x=ADV2Execute2Program("doscp",args,NULL,FALSE);
*/
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(device_spec)+RDAstrlen(to_filename)+20+1);
	sprintf(temp,"/usr/bin/doscp -r %s %s:%s",from_filename,device_spec,to_filename);
	memset(stemp,0,101);
	sprintf(stemp,"DOS COPYING -- %s",temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(device_spec)+1+RDAstrlen(to_filename)+1);
	sprintf(temp,"%s:%s",device_spec,to_filename);
	args=APPlibNEW();
	addAPPlib(args,"-blink");
	addAPPlib(args,"-j");
	addAPPlib(args,"-ls");
	addAPPlib(args,"-menu");
	addAPPlib(args,"-rw");
	addAPPlib(args,"-sb");
	addAPPlib(args,"-si");
	addAPPlib(args,"-sk");
	addAPPlib(args,"-wf");
	addAPPlib(args,"-r");
	addAPPlib(args,"-T");
	addAPPlib(args,stemp);
	addAPPlib(args,"-geometry");
	addAPPlib(args,"85x25");
	addAPPlib(args,"-e");
	addAPPlib(args,"/usr/bin/doscp");
	addAPPlib(args,"-r");
	addAPPlib(args,from_filename);
	addAPPlib(args,temp);
	x=ADV2Execute2Program("/usr/bin/X11/scoterm",args,NULL,FALSE);
	if(temp!=NULL) Rfree(temp);
#endif
#ifndef WIN32
	if(args!=NULL) freeapplib(args);
	if(x!=0)
	{
		temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(to_filename)+RDAstrlen(device_spec)+54+1);
		sprintf(temp,"COPY ERROR: Can Not Copy File %s To %s on device %s!",from_filename,to_filename,device_spec);
		prterr(temp);
		if(temp!=NULL) Rfree(temp);
		return(-2);
	} 
#endif
#ifdef WIN32
	if(x==0)
	{
		x=GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,x,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR) &error_message,0,NULL);
		switch(x)
		{
			case 1005:
				temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(to_filename)+RDAstrlen(device_spec)+123+1);
				sprintf(temp,"DOS COPY ERROR: Can Not Copy File %s To %s on device %s because media is unreconizable!\nDo you wish to DOS FORMAT the media?",from_filename,to_filename,device_spec);
				ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","DOS COPY FILE FAILED",temp,xFORMAT_MEDIA,NULL,FALSE,3,device_spec,__LINE__,__FILE__);
				prterr(temp);
				break;
			default:
				temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(to_filename)+RDAstrlen(device_spec)+123+1);
				sprintf(temp,"DOS COPY ERROR: Can Not Copy File %s To %s on device %s because of error [%d] %s!?",from_filename,to_filename,device_spec,error_message);
				ADVWARNINGDIALOG("WARNING DIALOG SCREEN","DOS COPY FILE FAILED",temp,NULL,NULL,FALSE,2,NULL,NULL,NULL);
				prterr(temp);
				break;
		}
/*
		temp=Rmalloc(RDAstrlen(from_filename)+RDAstrlen(to_filename)+RDAstrlen(device_spec)+54+1);
		sprintf(temp,"COPY ERROR: Can Not Copy File %s To %s on device %s!",from_filename,to_filename,device_spec);
*/
		temp=Rmalloc(RDAstrlen(error_message)+20+5+1);
		sprintf(temp,"COPY ERROR: [%d] %s!",x,error_message);
		prterr(temp);
		if(temp!=NULL) Rfree(temp);
		LocalFree(error_message);
		return(-2);
	} 
#endif
	return(0);
	} else {
		temp=Rmalloc(RDAstrlen(device_spec)+54+1);
		prterr("COPY ERROR: No filename given to copy to.");
		if(temp!=NULL) Rfree(temp);
		return(-2);
	}
	} else {
		prterr("COPY ERROR: No filename given to copy from.");
		return(-2);
	}
}
void quit_dos_copy(RDArsrc *parent,RDADosCopy *DosCopyStruct)
{
	void (*quitfunc)(...)=NULL;
	void *quitfuncargs=NULL;
	RDArsrc *parentrsrc=NULL;

	if(DosCopyStruct!=NULL)
	{
		if(DosCopyStruct->from_filename!=NULL) Rfree(DosCopyStruct->from_filename);
		if(DosCopyStruct->to_filename!=NULL) Rfree(DosCopyStruct->to_filename);
		if(DosCopyStruct->device_spec!=NULL) Rfree(DosCopyStruct->device_spec);
		quitfunc=DosCopyStruct->quitfunc;
		quitfuncargs=DosCopyStruct->quitfuncargs;
		parentrsrc=DosCopyStruct->parentrsrc;
		Rfree(DosCopyStruct);
/*lint -e746 */
		if(quitfunc!=NULL) quitfunc(parentrsrc,quitfuncargs);
/*lint +e746 */
	}
}
void do_dos_download(RDArsrc *parent,RDADosCopy *DosCopyStruct)
{
	APPlib *args=NULL;
	char *work_ip=NULL,*work_documents=NULL,*work_login=NULL,*work_passwd=NULL;

	work_ip=WorkStationIPAddr();
	work_login=UsersWorkstationLogin();
	work_passwd=UsersWorkstationPassword();
	work_documents=UsersWorkstationDocuments();
	readallwidgets(parent);
	FINDRSCGETSTRING(parent,"DEVICE SPECIFICATION",&DosCopyStruct->device_spec);
	FINDRSCGETSTRING(parent,"FROM FILE NAME",&DosCopyStruct->from_filename);
	args=APPlibNEW();
	addAPPlib(args,"\"\"");
	addAPPlib(args,DosCopyStruct->from_filename);
	addAPPlib(args,work_ip);
	addAPPlib(args,work_documents);
	addAPPlib(args,work_login);
	addAPPlib(args,work_passwd);
	ADVExecute2Program("sftpfile",args,NULL);
	if(args!=NULL) freeapplib(args);
}
void do_dos_copy(RDArsrc *parent,RDADosCopy *DosCopyStruct)
{
	int x=0;

	readallwidgets(parent);
	FINDRSCGETSTRING(parent,"DEVICE SPECIFICATION",&DosCopyStruct->device_spec);
	FINDRSCGETSTRING(parent,"FROM FILE NAME",&DosCopyStruct->from_filename);
	FINDRSCGETSTRING(parent,"TO FILE NAME",&DosCopyStruct->to_filename);
	x=COPY_FILE(DosCopyStruct->from_filename,DosCopyStruct->to_filename,DosCopyStruct->device_spec);
	if(x!=0)
	{
	}
}
void do_dos_format(RDArsrc *parent,RDADosCopy *DosCopyStruct)
{
	int x=0;
	
	readallwidgets(parent);
	FINDRSCGETSTRING(parent,"DEVICE SPECIFICATION",&DosCopyStruct->device_spec);
	x=FORMAT_MEDIA(DosCopyStruct->device_spec);
	if(x!=0)
	{
	}
}
void do_zip_file(RDArsrc *parent,RDADosCopy *DosCopyStruct)
{
	int x=0;
	char *ffile=NULL,*zfile=NULL,*temp=NULL,*temp1=NULL;
	APPlib *args=NULL;
	
	readallwidgets(parent);
	FINDRSCGETSTRING(parent,"FROM FILE NAME",&ffile);
	if(isEMPTY(ffile)) 
	{
		if(ffile!=NULL) Rfree(ffile);
		return;
	}
	temp=stralloc(ffile);
	for(x=RDAstrlen(temp);x>0;--x)
	{
		temp1=temp+x;
		if(*temp1=='.')
		{
			*temp1=0;
			break;
		}
	}
	zfile=Rmalloc(RDAstrlen(temp)+5);
	sprintf(zfile,"%s.zip",temp);
	args=APPlibNEW();
	addAPPlib(args,zfile);
	addAPPlib(args,ffile);
	ExecuteProgram("zip",args);
	if(args!=NULL) freeapplib(args);
	FINDRSCSETSTRING(parent,"FROM FILE NAME",zfile);
	FINDRSCSETSTRING(parent,"TO FILE NAME",zfile);
	updatersrc(parent,"FROM FILE NAME");
	updatersrc(parent,"TO FILE NAME");
	if(zfile!=NULL) Rfree(zfile);
	if(ffile!=NULL) Rfree(ffile);
}
void DOHTTP(int argc,char **argv)
{
	char delflag=FALSE,*prog_name=NULL; 
	short gsv_type=(-1),u_type=(-1),x=0;
	char *libx=NULL,*temp=NULL;
	RDAGenericSetup *gsv=NULL;
	APPlib *args=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","BROWSER TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			gsv_type=*gsv->value.short_value;
		} else {
			gsv_type=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(SEC_USERS_FILENO!=(-1))
	{
		ZERNRD(SEC_USERS_FILENO);
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(EQLNRD(SEC_USERS_FILENO,1)) 
		{
			KEYNRD(SEC_USERS_FILENO,1);
		} else {
			FINDFLDGETCHAR(SEC_USERS_FILENO,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSHORT(SEC_USERS_FILENO,"BROWSER TYPE",&u_type);
			}
		}
	}
	switch(u_type)
	{
		default:
		case (-1):
			break;
#ifndef WIN32
		case 0: /* OPERA */
			prog_name=stralloc("/usr/bin/opera");
			break;
		case 1: /* MOZILLA */
			prog_name=stralloc("/usr/bin/mozilla");
			break;
		case 2: /* FIREFOX */ 
			prog_name=stralloc("/usr/bin/firefox");
			break;
#endif
#ifdef WIN32
		case 0: /* NETSCAPE */
/*
			prog_name=stralloc("C:\\Program Files\\Netscape\\Netscape\\NETSCP.EXE");
*/
			prog_name=stralloc("NETSCP.EXE");
			break;
		case 1: /* MOZILLA */
			prog_name=stralloc("MOZILLA.EXE");
			break;
		case 2: /* EXPLORER */ 
			prog_name=stralloc("C:\\windows\\EXPLORER.EXE");
			break;
		case 3: /* FIREFOX */ 
			prog_name=stralloc("C:\\Program Files\\Mozilla Firefox\\FIREFOX.EXE");
			break;
#endif
	}
	if(isEMPTY(prog_name))
	{
		switch(gsv_type)
		{
			default:
			case (-1):
				break;
#ifndef WIN32
			case 0: /* OPERA */
				prog_name=stralloc("/usr/bin/opera");
				break;
			case 1: /* MOZILLA */
				prog_name=stralloc("/usr/bin/mozilla");
				break;
			case 2: /* FIREFOX */ 
				prog_name=stralloc("/usr/bin/firefox");
				break;
#endif
#ifdef WIN32
			case 0: /* NETSCAPE */
				prog_name=stralloc("C:\\Program Files\\Netscape\\Netscape\\NETSCP.EXE");
				break;
			case 1: /* MOZILLA */
				prog_name=stralloc("MOZILLA.EXE");
				break;
			case 2: /* EXPLORER */ 
				prog_name=stralloc("EXPLORER.EXE");
				break;
			case 3: /* FIREFOX */ 
				prog_name=stralloc("FIREFOX.EXE");
				break;
#endif
		}
	}
	if(isEMPTY(prog_name))
	{
		prterr("Error:  dohttp was unable to dispatch browser.");
	} else {
		args=APPlibNEW();
		for(x=0,temp=*argv;x<argc;++x,++temp)
		{
			addAPPlib(args,temp);
		}
		ADVExecuteProgramB(prog_name,args,NULL,TRUE);
		if(args!=NULL) freeapplib(args);
	}
	if(prog_name!=NULL) Rfree(prog_name);
}
void do_url(RDArsrc *parent,char *url)
{
	GUI_OpenURL(url);
}
int run_dos_copy(RDArsrc *parent,char *procname,char *device_spec_override,char *from_filename,char *to_filename,short APPmainLoop,APPlib *errorlist,void (*quitfunc)(...),void *quitfuncargs,char *url)
{
	RDADosCopy *DosCopyStruct=NULL;
	int x=(-1);
	short modal=TRUE;

	DosCopyStruct=Rmalloc(sizeof(RDADosCopy));
	DosCopyStruct->from_filename=stralloc(from_filename);
	DosCopyStruct->to_filename=stralloc(to_filename);
	DosCopyStruct->quitfunc=quitfunc;
	DosCopyStruct->quitfuncargs=quitfuncargs;
	DosCopyStruct->parentrsrc=parent;
	DosCopyStruct->doscopyrsrc=RDArsrcNEW("UTILITIES","DOS COPY WINDOW");
	addstdrsrc(DosCopyStruct->doscopyrsrc,"PROCESS",VARIABLETEXT,0,procname,FALSE);
	if(errorlist->numlibs<1)
	{
		addAPPlib(errorlist,"No Errors Encountered");
	}
	x=0;
	addlstrsrc(DosCopyStruct->doscopyrsrc,"ERROR LIST",&x,TRUE,NULL,errorlist->numlibs,&errorlist->libs,NULL);
	if(!isEMPTY(device_spec_override))
	{
		DosCopyStruct->device_spec=stralloc(device_spec_override);
	} else {
		DosCopyStruct->device_spec=DefaultMediaOutDevice();
	}
	addstdrsrc(DosCopyStruct->doscopyrsrc,"DEVICE SPECIFICATION",VARIABLETEXT,0,(DosCopyStruct->device_spec!=NULL?DosCopyStruct->device_spec:""),TRUE);
	addstdrsrc(DosCopyStruct->doscopyrsrc,"FROM FILE NAME",VARIABLETEXT,0,(DosCopyStruct->from_filename!=NULL?DosCopyStruct->from_filename:""),TRUE);
	addstdrsrc(DosCopyStruct->doscopyrsrc,"TO FILE NAME",VARIABLETEXT,0,(DosCopyStruct->to_filename!=NULL?DosCopyStruct->to_filename:DosCopyStruct->from_filename),TRUE);
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"COPY FILE",TRUE,do_dos_copy,DosCopyStruct);
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"ZIP FILE",TRUE,do_zip_file,DosCopyStruct);
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"URL",TRUE,do_url,url);
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"DOWNLOAD",TRUE,do_dos_download,DosCopyStruct);
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"FORMAT MEDIA",TRUE,do_dos_format,DosCopyStruct);
/*
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"PRINT ERROR LIST",TRUE,print_list,DosCopyStruct->errorlist);
*/
	addrfkwrsrc(DosCopyStruct->doscopyrsrc,"QUIT",TRUE,quit_dos_copy,DosCopyStruct);
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(DosCopyStruct->doscopyrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(ADVmakescrn(DosCopyStruct->doscopyrsrc,modal))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen DOS COPY WINDOW. Check to see the screen is available. If it is, call your installer.",NULL,TRUE);
		prterr("Error can't make screen DOS COPY WINDOW.");
		if(quitfunc!=NULL)
		{
			quit_dos_copy(DosCopyStruct->doscopyrsrc,DosCopyStruct);
		}
		return(-2);
	}
	if(!APPmainLoop) 
	{
		APPmainLoop=TRUE;
		RDAAPPMAINLOOP();
	}
	return(0);
}
char *MakeArchiveName(char *module,char *usemename)
{
	char *libx=NULL,*name=NULL;
	RDAGenericSetup *gsv=NULL;
	short namestyle=4;
	char *time=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("ARCHIVE")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","DEFAULT NAME STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			namestyle=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	name=Rmalloc(61);
	switch(namestyle)
	{
		case 0: /* YYYY-MM-DD.HH-MM.MODULE.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],module,usemename);
			if(time!=NULL) Rfree(time);
			break;
		case 1: /* YYYY-MM-DD.HH-MM.MODULE.PROCESS.USER */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s.%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],module,usemename,USERLOGIN);
			if(time!=NULL) Rfree(time);
			break;
		case 2: /* YYYY-MM-DD.HH-MM.MODULE.USER.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],module,USERLOGIN,usemename);
			if(time!=NULL) Rfree(time);
			break;
		case 3: /* YYYY-MM-DD.HH-MM.USER.MODULE.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN,module,usemename);
			if(time!=NULL) Rfree(time);
			break;
		default:
		case 4: /* YYYY-MM-DD.HH-MM.USER.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN,usemename);
			if(time!=NULL) Rfree(time);
			break;
	}
	return(name);
}
void CheckDiskSpace()
{
	char *temp=NULL,*temp1=NULL;
#ifndef WIN32
	FILE *fp=NULL;
	char buffer[513];
	char *base=NULL;
	short EExists=FALSE,s=0;
#endif /* WIN32 */
#ifdef WIN32
	unsigned uErr=0;
   	LPCWSTR pszDrive = NULL;
	long f_free=0,test=1500;
	char MyMessage[4096];
	__int64 lpFreeBytesAvailable, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
#endif /* WIN32 */

	if(XPERT_SETUP->ASP==TRUE) return;

	temp=RDA_GetEnv("SKIP_DISK_CHECK");
	if(temp!=NULL) return;

	temp=RDA_GetEnv("DEV_LICENSE");
	if(temp!=NULL)
	{
		temp1=strtok(temp," ");
		if(CheckDevLicense(temp1)) 
		{
			return;
		}
	}
#ifndef WIN32
	fp=popen("df -P","r");
	if(fp!=NULL)
	{
		while(fgets(buffer,512,fp)!=NULL)
		{
			if((RDAstrstr(buffer,"cdrom")!=NULL) || (RDAstrstr(buffer,"/dev/sr0")!=NULL) || (RDAstrstr(buffer,"/dev/scd0")!=NULL))
			{
			} else {
				if(base!=NULL)
				{
					base=Rrealloc(base,RDAstrlen(base)+RDAstrlen(buffer)+1);
					strcat(base,buffer);
				} else base=stralloc(buffer);
				for(temp=buffer;*temp;++temp) 
				{
					if(*temp=='%') 
					{
						*temp=0;
						temp-=3;
						break;
					}
				}
				if(*temp) s=atoi(temp);
				if(s>90) EExists=TRUE;
			}
			memset(buffer,0,512);
		}
		pclose(fp);
	}
	fp=popen("df -iP","r");
	if(fp!=NULL)
	{
		while(fgets(buffer,512,fp)!=NULL)
		{
			if((RDAstrstr(buffer,"cdrom")!=NULL) || (RDAstrstr(buffer,"/dev/sr0")!=NULL) || (RDAstrstr(buffer,"/dev/scd0")!=NULL))
			{
			} else {
				if(base!=NULL)
				{
					base=Rrealloc(base,RDAstrlen(base)+RDAstrlen(buffer)+1);
					strcat(base,buffer);
				} else base=stralloc(buffer);
				for(temp=buffer;*temp;++temp) 
				{
					if(*temp=='%') 
					{
						*temp=0;
						temp-=3;
						break;
					}
				}
				if(*temp) s=atoi(temp);
				if(s>90) EExists=TRUE;
			}
			memset(buffer,0,512);
		}
		pclose(fp);
	}
	if(EExists)
	{
		AddSupportRequest("UTILITIES","LOW DISK SPACE",base);
	}
	if(base!=NULL) Rfree(base);
#endif /* WIN32 */
#ifdef WIN32
   	uErr = GetDiskFreeSpaceEx(pszDrive,
               (PULARGE_INTEGER)&lpFreeBytesAvailable,
               (PULARGE_INTEGER)&lpTotalNumberOfBytes,
               (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes
               );
	f_free=(long)(lpFreeBytesAvailable/(1024*1024));


	if(f_free<test)
	{
		prterr("FreeBytesAvailable [%ld]\n TotalNumberOfBytes [%ld]\n TotalFreeBytes [%ld] \n",(PULARGE_INTEGER)&lpFreeBytesAvailable,(PULARGE_INTEGER)&lpTotalNumberOfBytes,(PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
		prterr("Free Megabytes [%ld] \n",f_free);
		sprintf(MyMessage,"Free Megabytes [%ld] \n",f_free);
		AddSupportRequest("UTILITIES","LOW DISK SPACE",MyMessage);
	}
#endif /* WIN32 */
}
short xLongestAccountCode(short type,int line,char *file)
{
	short len=0,longest=0;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	int z=0,x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG Finding the longest account code of type [%d] at line [%d] program [%s].",type,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(type==0 || type==(-1))
	{
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
					z<EXPENDITURE_ACCOUNT->num;++z,++acc)
				{
					len=0;
					for(x=0,part=acc->acct;x<(acc->num);++x,++part)
					{
						if(RDAstrlen(part->name)>RDAstrlen(part->definition)) len+=((RDAstrlen(part->name))+(x<(acc->num-1) ? 1:0));
						else len+=((RDAstrlen(part->definition))+(x<(acc->num-1) ? 1:0));
					}
					if(len>longest) longest=len;
				}
			}
		}
	}
	if(type==1 || type==(-1))
	{
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				for(z=0,acc=REVENUE_ACCOUNT->codetype;
					z<REVENUE_ACCOUNT->num;++z,++acc)
				{
					len=0;
					for(x=0,part=acc->acct;x<(acc->num);++x,++part)
					{
						if(RDAstrlen(part->name)>RDAstrlen(part->definition)) len+=((RDAstrlen(part->name))+(x<(acc->num-1) ? 1:0));
						else len+=((RDAstrlen(part->definition))+(x<(acc->num-1) ? 1:0));
					}
					if(len>longest) longest=len;
				}
			}
		}
	}
	if(type==2 || type==(-1))
	{
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
				{
					len=0;
					for(x=0,part=acc->acct;x<(acc->num);++x,++part)
					{
						if(RDAstrlen(part->name)>RDAstrlen(part->definition)) len+=((RDAstrlen(part->name))+(x<(acc->num-1) ? 1:0));
						else len+=((RDAstrlen(part->definition))+(x<(acc->num-1) ? 1:0));
					}
					if(len>longest) longest=len;
				}
			}
		}
	}
	return(longest);
}
char *OpenRDA_AutoComplete(char *module,char *screen,char *w,char *v)
{
	char *m1=NULL,*s1=NULL,*w1=NULL;
	char *temp=NULL;

	if(GUI_AUTO_COMPLETE!=(-1))
	{
		ZERNRD(GUI_AUTO_COMPLETE);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"MODULE",module);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"SCREEN NAME",screen);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"WIDGET",w);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"VALUE",v);
		if(!EQLNRD(GUI_AUTO_COMPLETE,1))
		{
		} else if(!NXTNRD(GUI_AUTO_COMPLETE,1)) 
		{
			FINDFLDGETSTRING(GUI_AUTO_COMPLETE,"MODULE",&m1);
			FINDFLDGETSTRING(GUI_AUTO_COMPLETE,"SCREEN NAME",&s1);
			FINDFLDGETSTRING(GUI_AUTO_COMPLETE,"WIDGET",&w1);
			if(!RDAstrcmp(module,m1) && !RDAstrcmp(screen,s1) && !RDAstrcmp(w,w1))
			{
				FINDFLDGETSTRING(GUI_AUTO_COMPLETE,"VALUE",&temp);
				if(strncmp(v,temp,RDAstrlen(v)))
				{
					if(temp!=NULL) Rfree(temp);
				}
			}
		}
	} else {
		prterr("Error:  Unable to access GUI -> AUTOCOMP.");
	}
	return(temp);
}
void OpenRDA_CaptureAutoComplete(char *module,char *screen,char *w,char *v)
{
	if(GUI_AUTO_COMPLETE!=(-1) && !isEMPTY(v))
	{
		ZERNRD(GUI_AUTO_COMPLETE);
		FINDFLDSETCHAR(GUI_AUTO_COMPLETE,"DELETEFLAG",FALSE);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"MODULE",module);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"SCREEN NAME",screen);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"WIDGET",w);
		FINDFLDSETSTRING(GUI_AUTO_COMPLETE,"VALUE",v);
		WRTNRD(GUI_AUTO_COMPLETE,0,NULL);
	}
}
char *EscXHTMLLabel(char *s,short type)
{
	char *n=NULL,*temp=NULL,ok=TRUE;
	int l=0,x=0,len=0;

	if(isEMPTY(s)) return(NULL);
	len=(RDAstrlen(s)*2)+1;
	n=Rmalloc(len);
	memset(n,0,len);
	l=0;
	for(x=0,temp=s;*temp && x<len;++temp,++x)
	{
		if(type==0)
		{
			if((*temp==13) || (l==0 && *temp==' '))
			{
			} else if(!RDAstrcmp(temp,"<!--"))
			{
				temp+=3;
				x+=3;
				ok=FALSE;
			} else if(!RDAstrcmp(temp,"--!>"))
			{
				temp+=3;
				x+=3;
				ok=TRUE;
			} else if(*temp=='"' && ok==TRUE)
			{
				n[l++]='\"';
			} else if(ok==TRUE)
			{
				n[l++]=*temp;
			}
		} else {
			if(*temp=='\n')
			{
				sprintf(&n[l],"\\\n");
				l+=2;
			} else if(*temp=='"')
			{
				sprintf(&n[l],"\\\"");
				l+=2;
			} else {
				n[l++]=*temp;
			}
		}
	}
	if(!type) unpad(n);
	return(n);
}
