/* doadd.c - Power Add */
/*lint -library */
#ifndef WIN32
#define __NAM__ "wwwdoadd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "wwwdoadd.exe"
#endif
#include <app.hpp>
#include <cstdio>


#include <rptgen.hpp>
#include <powadd.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
/*CPP_OPART wwwdo1 */
/*ADDLIB curl */
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct FtpFile {
	char *filename;
	RDA_PFILE *fp;
};

extern void WWWprocess_dataline(RDArunrpt *,RDAreport *,RDAimage *,short);

static void doexit(PowerAdd *);
static void save_record_now(RDArsrc *,PowerAdd *);
static void save_record(RDArsrc *,PowerAdd *);
static void quit_record(RDArsrc *,PowerAdd *);
void doadd(int,void (*)(...),RDArsrc *,void *); 
#define RUNWWWADD(a,b,c,d,e,f)	xRUNWWWADD(a,b,c,d,e,f,__LINE__,__FILE__)
#define WWWSetPowerAddSupportingFiles(a,b,c,d,e,f)	xWWWSetPowerAddSupportingFiles(a,b,c,d,e,f,__LINE__,__FILE__)
char xWWWSetPowerAddSupportingFiles(RDArsrc *,PowerAdd *,void (*)(...),void *,short,short,int,char *);

static void Set_Skip_Widgets(PowerAdd *p)
{
	RDAaddsub *addsub=NULL,*addsub1=NULL;
	int x,y;
	short count=0;

	if(p->subord!=NULL)
	{
		for(x=0,addsub=p->subord;x<p->subnum;++x,++addsub)
		{
			count=0;
			for(y=0,addsub1=p->subord;y<p->subnum;++y,++addsub1)
			{
				if(!RDAstrcmp(addsub->module,addsub1->module) && 
					!RDAstrcmp(addsub->filename,addsub1->filename))
				{
					 ++count;
					if(count>1) addsub1->skip_widget=TRUE;
				}
			}
		}
	}
}
char xWWWSetPowerAddSupportingFiles(RDArsrc *mrsrc,PowerAdd *POWERADD,void (*SubFunc)(...),void *args,short update_rsrcs,short display_diags,int line,char *file)
{
	int x,y;
	short fileno=(-1);
	RDAaddsub *s;
	NRDkey *key;
	NRDpart *part;
	RDAconfld *confld;
	NRDfield *field=NULL;
	short keyno=1,getfile=FALSE;
	char *temp=NULL,*temp1=NULL;

	if(diagadd)
	{
		prterr("DIAG xWWWSetPowerAddSupportingFiles Update Resources [%s] Display Diagnostics [%s] at line [%d] program [%s].",
			(update_rsrcs==TRUE?"TRUE":"FALSE"),
			(display_diags==TRUE?"TRUE":"FALSE"),line,file);
	}
	if(POWERADD!=NULL)
	{
	if(POWERADD->subord!=NULL)
	{
	for(x=0,s=POWERADD->subord;x<POWERADD->subnum;++x,++s)
	{
		fileno=SCRNFILENUMBER(mrsrc,s->module,s->filename,s->occurrence);
		if(fileno!=(-1))
		{
			keyno=1;
			key=KEYNUM(fileno,keyno);
			if(s->confld!=NULL && key!=NULL)
			{
				if(key->numparts==s->num && key->part!=NULL)
				{
					for(y=0,confld=s->confld,part=key->part;(y<key->numparts && !getfile);
						++y,++confld,++part)
					{
						field=FLDNRD(fileno,confld->fldname);
						if(field!=NULL)
						{
							if(!isEMPTY(confld->conname))
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->dlen=0;
										field->data.string_value=PP_EVALUATEstr(confld->conname,SubFunc,args);
										if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
										if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
										break;
									case BOOLNS:
									case CHARACTERS:
										*field->data.string_value=PP_EVALUATEbol(confld->conname,SubFunc,args);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										*field->data.float_value=PP_EVALUATEdbl(confld->conname,SubFunc,args);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=PP_EVALUATEsht(confld->conname,SubFunc,args);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										*field->data.integer_value=PP_EVALUATEint(confld->conname,SubFunc,args);
										break;
									default:
									case BUTTONS:
										break;
								}
							} else {
								temp=Rmalloc(RDAstrlen(s->filename)+RDAstrlen(confld->fldname)+5);
								sprintf(temp,"[%s][%s]",s->filename,confld->fldname);
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										FINDRSCGETSTRING(mrsrc,temp,&field->data.string_value);
										break;
									case BOOLNS:
									case CHARACTERS:
										FINDRSCGETCHAR(mrsrc,temp,field->data.string_value);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										FINDRSCGETDOUBLE(mrsrc,temp,field->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										FINDRSCGETSHORT(mrsrc,temp,field->data.short_value);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										FINDRSCGETINT(mrsrc,temp,field->data.integer_value);
										break;
									default:
									case BUTTONS:
										break;
								}
								
								if(temp!=NULL) Rfree(temp);
							}	
						} else {
							prterr("Error field not found for confld->fldname [%s] ",(confld->fldname!=NULL ? confld->fldname:""));
						}
					}
					switch(s->dtype)
					{
						default:
						case 0: /* Equal To */
						case 1:
							if(ADVEQLNRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==0)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									prterr("ERROR:%s\n",(temp1!=NULL ? temp1:""));
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==0)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
						case 2: /* Greater Than or Equal To */ 
						case 3:
							if(ADVGTENRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==2)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==2)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
										if(temp1!=NULL) Rfree(temp1);
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										return(FALSE);
									}
									}
								}
							}
							break;
						case 4: /* Greater Than */
						case 5:
							if(ADVNXTNRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								if(display_diags && s->dtype==4)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,
										"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==4)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
						case 6: /* Less Than or Equal */
						case 7:
							if(ADVLTENRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==6)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==6)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
						case 8: /* Less Than */
						case 9:
							if(ADVPRVNRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==8)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,
										"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==8)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
					}
				}
			}
		} else {
			if(display_diags && (s->dtype==0 || s->dtype==2 || s->dtype==4 || s->dtype==6 || s->dtype==8))
			{
			temp1=Rmalloc(300+RDAstrlen(s->module)+RDAstrlen(s->filename));
			sprintf(temp1,"Cannot find the Supporting file of [%s][%s].  Please have this corrected in the Power Add definition so that records can be saved to this file.",(s->module!=NULL?s->module:""),(s->filename!=NULL?s->filename:""));
			fprintf(stdout,"ERROR:%s\n",(temp1!=NULL ? temp1:""));
			if(temp1!=NULL) Rfree(temp1);
			return(FALSE);
			}
		}
		if(fileno!=(-1))
		{
			if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
			else filerecord2rsrc(fileno,mrsrc); 
		}
	}
	}
	}
	return(TRUE);
}
static void doexit(PowerAdd *POWERADD)
{
	int level=0; 

	if(POWERADD!=NULL)
	{
		level=POWERADD->level;
		CLOSE_POWERADD_FILES(POWERADD);
		if(POWERADD!=NULL) FreePowerAdd(POWERADD);
		if(level==0)
		{
		 	ShutdownSubsystems();
		}
	}
}
static void xRUNWWWADD(char *module,char *poweradd,int level,
	void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromarg,
	int line,char *file)
{
	char *warnmessage=NULL;
	RDArsrc *add_rsrc=NULL;
	AddEditWdgt *EditWdgt=NULL;
	int x=0;
	PowerAdd *POWERADD=NULL;
	ADDPassableStruct *PSTRUCT=NULL;

	if(diagadd)
	{
		prterr("DIAG RUNWWWADD(%s,%s) called at line [%d] program [%s].",module,poweradd,line,file);
	}
	POWERADD=PowerAddNew(module,poweradd);
	POWERADD->APPmainLoop=FALSE;
	PSTRUCT=Rmalloc(sizeof(ADDPassableStruct));
	PSTRUCT->r=add_rsrc;
	PSTRUCT->quitfunc=quitfunc;
	PSTRUCT->fromrsrc=fromrsrc;
	PSTRUCT->fromarg=fromarg;

	if(findPowerAdd(POWERADD))
	{
		warnmessage=Rmalloc(300+RDAstrlen(module)+RDAstrlen(poweradd));
		sprintf(warnmessage,"Error Power Add Definition not found, this process cannot be executed without it's definition [%s] in the [%s.ADD] library.",poweradd,module);
		prterr("ERROR:%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(POWERADD);
		return;
	} else {
		if(!OPEN_POWERADD_FILES(POWERADD))
		{
			doexit(POWERADD);
			prterr("Error cannot open files for this power add..");
			return;
		}
		Set_Skip_Widgets(POWERADD);

		AllocateEditWdgts(POWERADD);
		PP_translate_POWERADD(POWERADD);

		add_rsrc=RDArsrcNEW(POWERADD->module,POWERADD->addscreen);
		PSTRUCT->r=add_rsrc;
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;

		POWERADD->SubFunc=POWERADDSubData;
		POWERADD->SubFuncArgs=POWERADD;
		POWERADD->addrsrc=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;
		POWERADD->level=level;

		GET_POWERADD_INCVARS(add_rsrc,POWERADD);
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
				++x,++EditWdgt)
			{
				if(!isEMPTY(EditWdgt->name))
				{
					makefieldrsrc(add_rsrc,EditWdgt->name,EditWdgt->type,EditWdgt->len,TRUE);
				}
			}
		}
		GET_POWERADD_SCREEN_VIRTUALS(add_rsrc,0,FALSE,TRUE,POWERADD);
		SET_POWERADD_FILE2RSRCS(add_rsrc,POWERADD);

		ADVSCRNvirtual2rsrc(add_rsrc,POWERADDSubData,(void *)POWERADD);

		PSTRUCT->r=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;

		DefaultScreens(add_rsrc);
		GetAddEditWdgts(add_rsrc,POWERADD);
		ReadRDAScrolledLists(add_rsrc);
		READEditWdgts(POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,FALSE,FALSE);
		DefaultAddEditWdgts(add_rsrc,POWERADD,0,POWERADDSubData,(void *)POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;
		PSTRUCT->r=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;
		save_record_now(add_rsrc,POWERADD);
	}
}
struct curl_httppost *PowerAdd2HttpPost(MaintainButton *button,PowerAdd *POWERADD)
{
	struct curl_httppost **tmp=NULL,**lastptr=NULL;
	char *temp3=NULL,*temp2=NULL,*temp1=NULL,*temp=NULL;
	int x=0,y=0,z=0,l=0;
	ButtonField *btnfld=NULL;

	if(button!=NULL)
	{
		for(x=0,btnfld=button->bf;x<button->num;++x,++btnfld)
		{
			temp1=stralloc(btnfld->rname);
			if(POWERADD->SubFunc!=NULL)
			{
/*lint -e746*/
				POWERADD->SubFunc(&temp1,POWERADD->SubFuncArgs);
/*lint +e746*/
			}
			z=RDAstrlen(temp1);
			for(l=0,temp=temp1;*temp;++temp)
			{
				if(l==0)
				{
					if((*temp!='"') && (*temp!='('))
					{
						++l;
					}
				} else if(l==(z-1))
				{
					if(*temp!='"')
					{
						++l;
					}
				} else {
					if(*temp!='"')
					{
						++l;
					} else {
						l+=2;
					}
				}
			}
			temp3=Rmalloc(l+1);
			temp2=temp3;
			for(y=0,temp=temp1;*temp;++temp,++y)
			{
				if(y==0)
				{
					if((*temp!='"') && (*temp!='('))
					{
						*temp2=*temp;
						++temp2;
					}
				} else if(y==(z-1))
				{
					if(*temp!='"')
					{
						*temp2=*temp;
						++temp2;
					}
				} else {
					if(*temp!='"')
					{
						*temp2=*temp;
						++temp2;
					} else {
						*temp2='\\';
						++temp2;
						*temp2=*temp;
						++temp2;
					}
				}
			}
			curl_formadd(tmp,lastptr,CURLFORM_COPYNAME,btnfld->ename,CURLFORM_COPYCONTENTS,(temp3!=NULL ? temp3:""),CURLFORM_END);
			if(temp1!=NULL) Rfree(temp1);
			if(temp3!=NULL) Rfree(temp3);
		}
	}
	return(*tmp);
}
static void WWWPowerAddButtonFunction(RDArsrc *mainrsrc,MaintainButton *button)
{
	APPlib *envpx=NULL,*newargs=NULL;
	struct curl_httppost *formpost=NULL;
	int x;
	char *value=NULL;
	PowerAdd *POWERADD=NULL;
	CURL *RDAcurl=NULL;
	CURLcode res=0;
	FILE *fp=NULL;

	if(button!=NULL)
	{
	POWERADD=(PowerAdd *)button->parent;
	if(POWERADD!=NULL)
	{
		envpx=PowerAdd2PassKey(button,POWERADD);
		newargs=APPlibNEW();
		for(x=0;x<button->args->numlibs;++x)
		{
			addAPPlib(newargs,button->args->libs[x]);
		}
		if(button->evalargs==TRUE)
		{
			for(x=0;x<newargs->numlibs;++x)
			{
				if(!isEMPTY(newargs->libs[x]))
				{
					value=EVALUATEstr(newargs->libs[x],POWERADD->SubFunc,POWERADD->SubFuncArgs);
					if(value==NULL) value=stralloc("");
					if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
					newargs->libs[x]=stralloc(value);
					if(value!=NULL) Rfree(value);
				} else {
					if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
					newargs->libs[x]=stralloc("");
				}
			}
		}
		if(!RDAstrcmp(button->progname,"RUNREPORT"))
		{
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			RUNREPORTADV3(newargs->libs[0],newargs->libs[1],NULL,NULL,TRUE,2,NULL,button->func,mainrsrc,(void *)POWERADD,(POWERADD->level+1),(void (*)(...))WWWprocess_dataline);
		} else if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
		{
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(POWERADD->level+1),button->func,mainrsrc,(void *)POWERADD,TRUE);
		} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
		{
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(POWERADD->level+1),button->func,mainrsrc,(void *)POWERADD,TRUE);
		} else if(!RDAstrcmp(button->progname,"HTTP POST"))
		{
			if(newargs->numlibs<1)
			{
				prterr("Error: 1st Argument must be URL.  No URL Found.");
			} else {
				RDAcurl=curl_easy_init();
				if(RDAcurl && newargs->numlibs>0)
				{
					curl_easy_setopt(RDAcurl,CURLOPT_URL,newargs->libs[0]);
					if(newargs->numlibs>1) 
					{
						if(!isEMPTY(newargs->libs[1]))
						{
							curl_easy_setopt(RDAcurl,CURLOPT_PROXY,newargs->libs[1]);
						}
					}
					curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE);
					formpost=PowerAdd2HttpPost(button,POWERADD);
					if(formpost!=NULL)
					{
						curl_easy_setopt(RDAcurl,CURLOPT_HTTPPOST,formpost);
						res=curl_easy_perform(RDAcurl);
						if(CURLE_OK!=res)
						{
							prterr("Error:  Failed to HTTP Post Form.");
						}
						curl_easy_cleanup(RDAcurl);
						curl_formfree(formpost);
						formpost=NULL;
					} else {
						prterr("Error: Unable to build form for posting.");
						curl_easy_cleanup(RDAcurl);
					}
				} else {
					prterr("Error: curl_easy_init failed.");
				}
			}
		} else if(!RDAstrcmp(button->progname,"HTTP POST-GET"))
		{
			if(newargs->numlibs<1)
			{
				prterr("Error: 1st Argument must be URL.  No URL Found.");
			} else {
				RDAcurl=curl_easy_init();
				if(RDAcurl)
				{
					curl_easy_setopt(RDAcurl,CURLOPT_URL,newargs->libs[0]);
					if(newargs->numlibs>1) 
					{
						if(!isEMPTY(newargs->libs[1]))
						{
							curl_easy_setopt(RDAcurl,CURLOPT_PROXY,newargs->libs[1]);
						}
					}
					curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE);
					formpost=PowerAdd2HttpPost(button,POWERADD);
					prterr("formpost [%s]  formpost [%d]",formpost,formpost);
					if(formpost!=NULL)
					{
						memset(stemp,0,101);
						sprintf(stemp,"helpdesk-ticket.%d.txt",RGETPID());
						fp=freopen(stemp,"w+b",stdout);	
						if(fp==NULL) 
						{
							prterr("Error: freopen didn't redirect anything.....");
						}
						curl_easy_setopt(RDAcurl,CURLOPT_HTTPPOST,formpost);
						res=curl_easy_perform(RDAcurl);
						if(CURLE_OK!=res)
						{
							prterr("Error:  Failed to HTTP Post Form.");
						} else {
							fclose(fp);
							memset(stemp,0,101);
							sprintf(stemp,"helpdesk-ticket.%d.txt",RGETPID());
							newargs=APPlibNEW();
							addAPPlib(newargs,"-ps2");
							addAPPlib(newargs,"-i");
							addAPPlib(newargs,stemp);
							Execute2Program("viewpdf",newargs);
							if(newargs!=NULL) freeapplib(newargs);
							unlink(stemp);
						}
						curl_easy_cleanup(RDAcurl);
						curl_formfree(formpost);
						formpost=NULL;
					} else {
						prterr("Error: Unable to build form for posting.");
						curl_easy_cleanup(RDAcurl);
					}
						
				} else {
					prterr("Error: curl_easy_init failed.");
				}
			}
		} else {
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			if(button->type==0)
			{
				if(!RDAstrcmp(button->progname,"UNLINK"))
				{
					ADVExecuteProgramB(button->progname,newargs,envpx,FALSE);
				} else {
					ADVExecuteProgram(button->progname,newargs,envpx);
				}
			} else {
				if(!RDAstrcmp(button->progname,"UNLINK"))
				{
					ADV3Execute2Program(button->progname,newargs,envpx,FALSE,TRUE);
				} else {
					ADVExecute2Program(button->progname,newargs,envpx);
				}
			}
		}
		if(newargs!=NULL) freeapplib(newargs);
	} else {
		prterr("Error in WWWPowerAddButtonFunction.");
	}
	}
}
static void WWWupdateallPowerAdd(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	if(POWERADD!=NULL)
	{
		if(add_rsrc!=NULL)
		{
       			GetAddEditWdgts(add_rsrc,POWERADD);
			GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,TRUE,FALSE);
			DefaultAddEditWdgts(add_rsrc,POWERADD,2,POWERADDSubData,(void *)POWERADD);
			SetAddEditWdgts(add_rsrc,POWERADD);
			xcomputeandupdateallSCRNvirtuals(add_rsrc,POWERADDSubData,(void *)POWERADD,FALSE,FALSE,NULL,POWERADD,__LINE__,__FILE__);
		}
	}
}
static void run_presave_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			if(button->type==1)
			{
				do_btn=FALSE;
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
				}  else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=WWWupdateallPowerAdd;
					WWWPowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void run_postsave_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			do_btn=FALSE;
			if(button->type==2)
			{
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
				} else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=WWWupdateallPowerAdd;
					WWWPowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void save_record_now(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	char *desc=NULL;

	if(POWERADD!=NULL)
	{
		if(!WWWSetPowerAddSupportingFiles(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,TRUE,TRUE))
		{
			fprintf(stdout,"ERROR: wwwdoadd Supporting Files not found.\n");
		 	ShutdownSubsystems();
			return;
		}
		if(!isEMPTY(POWERADD->save_expression))
		{
			if(PP_EVALUATEbol(POWERADD->save_expression,POWERADDSubData,(void *)POWERADD))
			{
				desc=PP_EVALUATEstr(POWERADD->save_error_desc,POWERADDSubData,POWERADD);
				prterr("ERROR:%s",desc);
				if(desc!=NULL) Rfree(desc);
		 		ShutdownSubsystems();
				return;
			}
		}
		if(!isEMPTY(POWERADD->save_warning))
		{
			if(PP_EVALUATEbol(POWERADD->save_warning,POWERADDSubData,(void *)POWERADD))
			{
				desc=PP_EVALUATEstr(POWERADD->save_warning_desc,
					POWERADDSubData,(void *)POWERADD);
				prterr("WARNING:%s\n",desc);
				if(desc!=NULL) Rfree(desc);
		 		ShutdownSubsystems();
				return;
			}
		}
		run_presave_buttons(add_rsrc,POWERADD);
		save_record(add_rsrc,POWERADD);
		run_postsave_buttons(add_rsrc,POWERADD);
		quit_record(add_rsrc,POWERADD);
	}
}
static void save_record(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	AddWrite *Write=NULL,*pWrite=NULL;
	AddWriteField *WriteField=NULL;
	NRDfield *field=NULL;
	RDAaddsub *subord=NULL;
	int a,x,b;
	char dowrite=FALSE,*trnfld=NULL,*fieldname=NULL;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL,*holdrsrc=NULL;
	void *holdarg=NULL;
	AddEditWdgt *AE;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	holdrsrc=PSTRUCT->r;
	holdarg=PSTRUCT->arg;
	if(POWERADD->Writes!=NULL)
	{
		if(POWERADD->save_type==0 || POWERADD->save_type>3)
		{
			tmprsrc=RDArsrcNEW(POWERADD->module,NULL);
/*
			PX=Rmalloc(sizeof(ADDPassableStruct));
*/
/* This code segment is causing performance problems when a large number of 
   EZ VIRTUAL FIELDS are defind in the module, e.g. FINMGT 400+ in ADD RECEIPTS 
*/
			GET_POWERADD_INCVARS(tmprsrc,POWERADD);
			GET_POWERADD_SCREEN_VIRTUALS(tmprsrc,0,TRUE,FALSE,POWERADD);
/* END This code segment ... */
/*
			GET_POWERADD_SCREEN_VIRTUALS(tmprsrc,0,TRUE,TRUE,POWERADD);
			GET_POWERADD_SCREEN_VIRTUALS(tmprsrc,0,FALSE,TRUE,POWERADD);
*/
		} else {
			tmprsrc=add_rsrc;
		}
		holdrsrc=PSTRUCT->r;
		holdarg=PSTRUCT->arg;
		PSTRUCT->r=tmprsrc;
		PSTRUCT->arg=POWERADD;
		for(a=0,Write=POWERADD->Writes;a<POWERADD->numWrites;
			++a,++Write)
		{
			if(isEMPTY(Write->expression)) dowrite=TRUE;
				else dowrite=PP_EVALUATEbol(Write->expression,POWERADDSubData,(void *)POWERADD);
			if(dowrite==TRUE)
			{
			if(POWERADD->save_type==0 || POWERADD->save_type>3)
			{
			Write->closefile=FALSE;
			if(POWERADD->subord!=NULL)
			{
				for(x=0,subord=POWERADD->subord;x<POWERADD->subnum;++x,++subord)
				{
					if(!RDAstrcmp(subord->module,Write->module) && !RDAstrcmp(subord->filename,Write->filename)) break;
				}
				if(x<POWERADD->subnum)
				{
					Write->fileno=subord->fileno;
					Write->closefile=FALSE;
					Write->zerofile=FALSE;
				}
			}
			if(Write->fileno==(-1))
			{
				for(b=0,pWrite=POWERADD->Writes;b<a;++b,++pWrite)
				{
					if(!RDAstrcmp(pWrite->module,Write->module) && !RDAstrcmp(pWrite->filename,Write->filename)) break;
				}
				if(a<b)
				{
					Write->fileno=pWrite->fileno;
					Write->closefile=FALSE;
					Write->zerofile=TRUE;
				}
			}
			if(Write->fileno==(-1))
			{
				Write->fileno=OPNNRDsec(Write->module,Write->filename,TRUE,TRUE,FALSE);
				addDFincvir(tmprsrc,Write->module,Write->filename,NULL,Write->fileno);
				Write->closefile=TRUE;
				Write->zerofile=TRUE;
			}
			}
			if(Write->fileno!=(-1))
			{
				if(Write->zerofile==TRUE) ZERNRD(Write->fileno);
				if(Write->fields!=NULL)
				{
					for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
					{
						fieldname=stripfieldname(WriteField->to_name);
						if(!isEMPTY(fieldname))
						{
							field=FLDNRD(Write->fileno,fieldname);
						}
						if(!WriteField->copy_type)
						{
							if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
							{
								trnfld=stralloc(fieldname);
								if(fieldname!=NULL) Rfree(fieldname);
							} else {
								if(fieldname!=NULL) Rfree(fieldname);
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
										case CHARACTERS:
										case OPTIONALFIELDS:
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								}
							}
						} else {
							if(fieldname!=NULL) Rfree(fieldname);
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
									case CHARACTERS:
									case OPTIONALFIELDS:
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										if(field->data.string_value!=NULL)
										{
											field->dlen=RDAstrlen(field->data.string_value)+1;
											if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
										} else field->dlen=0;
										break;
									case BOOLNS:
										*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case SLONGV:
									case SCROLLEDLIST:
									case LONGV:
										*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
										*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
										{
											*field->data.float_value=round(*field->data.float_value);
										}
										break;
									default:
										prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
								}
							}
						}
					}
					LOCNRDFILE(Write->fileno);
					if(trnfld!=NULL)
					{
						ADVWRTTRANSsec(Write->fileno,1,trnfld,NULL,POWERADDSubData,(void *)POWERADD);	
						Rfree(trnfld);
						trnfld=NULL;
					} else if(ADVEQLNRDsec(Write->fileno,1,POWERADDSubData,(void *)POWERADD))
					{
						ADVWRTTRANSsec(Write->fileno,0,NULL,NULL,POWERADDSubData,(void *)POWERADD);	
					} else {
						prev=RDATDataNEW(Write->fileno);
						
						for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
						{
							fieldname=stripfieldname(WriteField->to_name);
							field=FLDNRD(Write->fileno,fieldname);
							if(!WriteField->copy_type)
							{
								if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
								{
									trnfld=stralloc(fieldname);
									if(fieldname!=NULL) Rfree(fieldname);
								} else {
									if(fieldname!=NULL) Rfree(fieldname);
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
											case CHARACTERS:
											case OPTIONALFIELDS:
												if(field->data.string_value!=NULL) Rfree(field->data.string_value);
												field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												if(field->data.string_value!=NULL)
												{
													field->dlen=RDAstrlen(field->data.string_value)+1;
													if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
												} else field->dlen=0;
												break;
											case BOOLNS:
												*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case SHORTV:
											case SSHORTV:
												*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case SLONGV:
											case SCROLLEDLIST:
											case LONGV:
												*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case DOUBLEV:
											case SDOUBLEV:
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case SDECIMALV:
												*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
												{
													*field->data.float_value=round(*field->data.float_value);
												}
												break;
											default:
												prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
										}
									} else {
									}
								}
							} else {		
								if(fieldname!=NULL) Rfree(fieldname);
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
										case CHARACTERS:
										case OPTIONALFIELDS:
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								} else {
								}
							}
						}
						ADVWRTTRANSsec(Write->fileno,0,NULL,prev,POWERADDSubData,(void *)POWERADD);	
						if(prev!=NULL) FreeRDATData(prev);
					}	
				} else {
					prterr("Error AddWriteField's are missing...");
				}
			} else {
				prterr("Error AddWrite File not found [%s][%s].",(Write->module!=NULL ? Write->module:""),(Write->filename!=NULL ? Write->filename:""));
			}	
			}
		}
	}
	DefaultAddEditWdgts(add_rsrc,POWERADD,4,POWERADDSubData,(void *)POWERADD);
	SetAddEditWdgts(add_rsrc,POWERADD);
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,AE=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++AE)
		{
			if(AE->id_mgt==TRUE) 
			{
				REMOVENEWID(POWERADD,AE,POWERADDSubData,(void *)POWERADD);
			}
		}
		if(POWERADD->save_type==0 || POWERADD->save_type>3)
		{
			if(POWERADD->Writes!=NULL)
			{
				for(x=0,Write=POWERADD->Writes;x<POWERADD->numWrites;++x,
					++Write)
				{
					if(Write->fileno!=(-1) && Write->closefile==TRUE) 
						CLSNRD(Write->fileno);
				}
			}
		}
	}
	PSTRUCT->r=holdrsrc;
	PSTRUCT->arg=holdarg;
	if(POWERADD->save_type==0 || POWERADD->save_type>3)
	{
		if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	}
	++POWERADD->WRITE_COUNTER;
}
static void quit_record(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	APPlib *args=NULL;
	int level=0;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	level=POWERADD->level;
	if(add_rsrc!=NULL)
	{
		free_rsrc(add_rsrc); 
	}
	if(PSTRUCT!=NULL) Rfree(PSTRUCT);
	if(POWERADD!=NULL)
	{
		CLOSE_POWERADD_FILES(POWERADD);
		if((POWERADD->WRITE_COUNTER>0) && !isEMPTY(POWERADD->reportname))
		{
			args=APPlibNEW();
			addAPPlib(args,POWERADD->module);
			addAPPlib(args,POWERADD->reportname);
			ExecuteProgram("doreport",args);
			if(args!=NULL) freeapplib(args);
		}
		FreeEditWdgts(POWERADD);
		FreePowerAdd(POWERADD);
	}
	if(level==0)
	{
		ShutdownSubsystems();
	}
}
short WWWInitializeSubsystems(int argc,char **argv,char *module,char *process)
{
	char *desc=NULL;
	short return_value=0;
	long longtemp=0;

	initrdadiag();
	SETUSERLOGIN();
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
	RDA_NOGUI=TRUE;
	INITGUI(argc,argv,CURRENTDIRECTORY);
	SETCWD();
	PP_translate_GUIFUNC=PP_translate_GUI;
	PP_translate_GUIFUNCALL=PP_translate_GUIALL;
	XPERT_SETUP=XPERTstpNEW();
	if((getXPERTbin(XPERT_SETUP))==(-1))
	{
		prterr("ERROR: Error Invalid Version or Bad Binary for [XPERT SETUP] from Module: [%s] Process: [%s].",module,process);
		return_value=(-1);
	}
	if(INITNRD()) 
	{
		prterr("ERROR: Cannot Initialize the Database Subsystem.");
		return_value=(-1);
	} else if(DoInitializeSecurity()) 
	{
		prterr("Error Cannot Initialize the Security Subsystem.");
		return_value=(-1);
	} else if(!DoPROCsecurity(module,process)) 
	{
		prterr("ERROR: Security Access to Module [%s] Process [%s] DENIED!",module,process);
		desc=Rmalloc(RDAstrlen(module)+RDAstrlen(process)+111);
		sprintf(desc,"The Security SubSystem denied you access to Module [%s] Process [%s].\nPlease call your installer for support.",module,process);
		Rfree(desc);
		return_value=(-1);
	} else {
		INITTRANS();
		OPNLOG(module,process);
	}
	return(return_value);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(argc<3) std::exit;
	RDA_NOGUI=TRUE;
#ifdef LINUX 
	umask(002);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS) 
	umask(002);
#endif
	RDA_NOGUI=TRUE;
	if(WWWInitializeSubsystems(argc,argv,argv[1],argv[2]))
	{
		ShutdownSubsystems();
		return;
	}
	RDA_NOGUI=TRUE;
	RUNWWWADD(argv[1],argv[2],0,NULL,NULL,NULL);
}
