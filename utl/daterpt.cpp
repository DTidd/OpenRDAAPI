/* daterpt.c - Program to Display Virtuals with date implementations */
/*lint -library */
#ifndef WIN32
#define __NAM__ "daterpt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "daterpt.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="RPTGEN";
APPlib *DateFields=NULL;
APPlib *DateVirtuals=NULL;
APPlib *ReportValues=NULL;
static void daterpt(RDArsrc *);
static void REPORT_ContainsDate(RDA_PFILE *,RDAreport *,char *,char *,int *,int *,char *);
static char *ContainsADateField(char *,char);
static void quitfunc(RDArsrc *);
static void checkheader(RDA_PFILE *,int *,int *,char *);
static void printheader(RDA_PFILE *,int *,int *,char *,char *);
char *RDA_FormatOutputReportDate(char *,char *,char *,char *,char *,char *);
static void MaintainMaster_ContainsDate(RDA_PFILE *,MaintainMaster *,RDAreport *,char *,char *,int *,int *,char *,int *);

char *RDA_FormatOutputReportDate(char *type,char *id,char *offset1,char *offset2,char *offset3,char *reason)
{
	char *temp=NULL;
	short type_length=20;
	char type_string[21];
	short id_length=31;
	char id_string[32];
	short offset1_length=6;
	char offset1_string[7];
	short offset2_length=6;
	char offset2_string[7];
	short offset3_length=6;
	char offset3_string[7];
	short reason_length=53;
	char reason_string[54];
	int x=0;

	memset(type_string,0,21);
	for(x=0;x<type_length;++x)
	{
		if(x<RDAstrlen(type))
		{
			type_string[x]=type[x];
		} else {
			type_string[x]=' ';
		}
	}
	memset(id_string,0,32);
	for(x=0;x<id_length;++x)
	{
		if(x<RDAstrlen(id))
		{
			id_string[x]=id[x];
		} else {
			id_string[x]=' ';
		}
	}
	memset(offset1_string,0,7);
	for(x=0;x<offset1_length;++x)
	{
		if(x<RDAstrlen(offset1))
		{
			offset1_string[x]=offset1[x];
		} else {
			offset1_string[x]=' ';
		}
	}
	memset(offset2_string,0,7);
	for(x=0;x<offset2_length;++x)
	{
		if(x<RDAstrlen(offset2))
		{
			offset2_string[x]=offset2[x];
		} else {
			offset2_string[x]=' ';
		}
	}
	memset(offset3_string,0,7);
	for(x=0;x<offset3_length;++x)
	{
		if(x<RDAstrlen(offset3))
		{
			offset3_string[x]=offset3[x];
		} else {
			offset3_string[x]=' ';
		}
	}
	memset(reason_string,0,54);
	for(x=0;x<reason_length;++x)
	{
		if(x<RDAstrlen(reason))
		{
			reason_string[x]=reason[x];
		} else {
			reason_string[x]=' ';
		}
	}	
	temp=Rmalloc(RDAstrlen(type_string)+RDAstrlen(id_string)+RDAstrlen(offset1_string)+RDAstrlen(offset2_string)+RDAstrlen(offset3_string)+RDAstrlen(reason_string)+9+1);
	sprintf(temp,"    %s %s %s %s %s %s",type_string,id_string,offset1_string,offset2_string,offset3_string,reason_string);
	return(temp);
}
static void REPORT_ContainsDate(RDA_PFILE *fp1,RDAreport *rpt,char *rpt_module,char *rptfile,int *lines,int *pages,char *modname)
{
	RDAvirtual *v=NULL;
	RDAsearch *s=NULL;
	RDAimage *i=NULL;
	RDApval *p=NULL;
	int x,y;
	char *string_temp=NULL;
	int *foundone=NULL;
	char *temp1=NULL;
	char *temp2=NULL;
	char *temp3=NULL;
	char *temp4=NULL;
	char *suspect_string=NULL;
	short output_type=0;

	
	if(rpt!=NULL)
	{
		if(foundone!=NULL) Rfree(foundone);
		foundone=Rmalloc(sizeof(int)); 
		if(rpt->virtual!=NULL)
		{
			for(x=0,v=rpt->virtual;x<rpt->numvirtuals;++x,++v)
			{
				if(v->type==DATES)
				{
					if(*foundone==0) 
					{
						*foundone=1;
						string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
						sprintf(string_temp,"REPORT: [%s][%s]",
							(rpt_module!=NULL?rpt_module:""),
							(rpt->name!=NULL?rpt->name:""));
						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
					temp2=Rmalloc(6+1);
					sprintf(temp2,"%.6d",(x+1));
					string_temp=RDA_FormatOutputReportDate("Virtual Field",(v->name!=NULL?v->name:""),temp2,"","","Is a date type field");
					if(temp2!=NULL) Rfree(temp2);

					addAPPlib(DateVirtuals,string_temp);
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,modname);
				} else if(!isEMPTY(v->expression))
				{
					suspect_string=ContainsADateField(v->expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+27+1);
						sprintf(temp3,"(%s) date field in expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Virtual Field",(v->name!=NULL?v->name:""),temp2,"","",temp3);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				} else if(v->len==8)
				{
					switch(v->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case SOCSECNUM:
						case TIMES:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
							if(*foundone==0) 
							{
								*foundone=1;
								string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
								sprintf(string_temp,"REPORT: [%s][%s]",
									(rpt_module!=NULL?rpt_module:""),
									(rpt->name!=NULL?rpt->name:""));
								addAPPlib(DateVirtuals,string_temp);
								RDA_fprintf(fp1,"%s\r\n",string_temp);
								if(string_temp!=NULL) Rfree(string_temp);
								checkheader(fp1,lines,pages,modname);
							}
							temp2=Rmalloc(6+1);
							sprintf(temp2,"%.6d",(x+1));
							string_temp=RDA_FormatOutputReportDate("Virtual Field",(v->name!=NULL?v->name:""),temp2,"","","Has a length of 8");
							if(temp2!=NULL) Rfree(temp2);
	
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
							break;
						default:
							break;
					}
				}
			}
		}
		if(rpt->search!=NULL)
		{
			for(x=0,s=rpt->search;x<rpt->numsearchs;++x,++s)
			{
				if(!isEMPTY(s->expression))
				{
					suspect_string=ContainsADateField(s->expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}	
						temp1=Rmalloc(RDAstrlen(s->module)+RDAstrlen(s->filename)+5);
						sprintf(temp1,"[%s][%s]",(s->module!=NULL?s->module:""),(s->filename!=NULL?s->filename:""));
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+27+1);
						sprintf(temp3,"(%s) date field in expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Search File",temp1,temp2,"","",temp3);
						if(temp1!=NULL) Rfree(temp1);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
				if(!isEMPTY(s->skip_detail))
				{
					suspect_string=ContainsADateField(s->skip_detail,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}
						temp1=Rmalloc(RDAstrlen(s->module)+RDAstrlen(s->filename)+5);
						sprintf(temp1,"[%s][%s]",(s->module!=NULL?s->module:""),(s->filename!=NULL?s->filename:""));
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+27+1);
						sprintf(temp3,"(%s) field in skip detail exp",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Search File",temp1,temp2,"","",temp3);
						if(temp1!=NULL) Rfree(temp1);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);

					}
				}
				if(!isEMPTY(s->wc_expression))
				{
					suspect_string=ContainsADateField(s->wc_expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}
						temp1=Rmalloc(RDAstrlen(s->module)+RDAstrlen(s->filename)+5);
						sprintf(temp1,"[%s][%s]",(s->module!=NULL?s->module:""),(s->filename!=NULL?s->filename:""));
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+27+1);
						sprintf(temp3,"(%s) field in write count exp",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Search File",temp1,temp2,"","",temp3);
						if(temp1!=NULL) Rfree(temp1);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
				if(!isEMPTY(s->skip_detail_error))
				{
					suspect_string=ContainsADateField(s->skip_detail_error,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}
						temp1=Rmalloc(RDAstrlen(s->module)+RDAstrlen(s->filename)+5);
						sprintf(temp1,"[%s][%s]",(s->module!=NULL?s->module:""),(s->filename!=NULL?s->filename:""));
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+26+1);
						sprintf(temp3,"(%s) in skip detail desc exp",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Search File",temp1,temp2,"","",temp3);
						if(temp1!=NULL) Rfree(temp1);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
			}
		}
		if(rpt->image!=NULL)
		{
			for(x=0,i=rpt->image;x<rpt->numimages;++x,++i)
			{
				if(!isEMPTY(i->expression))
				{
					suspect_string=ContainsADateField(i->expression,TRUE);
					if(suspect_string!=NULL) 
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+22+1);
						sprintf(temp3,"(%s) in image expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Image Line",(i->line!=NULL?i->line:""),temp2,"","",temp3);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
				if(i->pvals!=NULL)
				{
					if(rpt->display!=NULL) output_type=rpt->display->device;
					else output_type=0;
					for(y=0,p=i->pvals;y<i->numpvals;++y,++p)
					{
						if(p!=NULL)
						{
							if((output_type==3) || (output_type==4))
							{
							if(!isEMPTY(p->name))
							{
								suspect_string=ContainsADateField(p->name,TRUE);
								if(suspect_string!=NULL)
								{
									if(*foundone==0) 
									{
										*foundone=1;
										string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
										sprintf(string_temp,"REPORT: [%s][%s]",
											(rpt_module!=NULL?rpt_module:""),
											(rpt->name!=NULL?rpt->name:""));
										addAPPlib(DateVirtuals,string_temp);
										RDA_fprintf(fp1,"%s\r\n",string_temp);
										if(string_temp!=NULL) Rfree(string_temp);
										checkheader(fp1,lines,pages,modname);
									}
									temp2=Rmalloc(6+1);
									sprintf(temp2,"%.6d",(x+1));
									temp3=Rmalloc(6+1);
									sprintf(temp3,"%.6d",(y+1));
									temp4=Rmalloc(RDAstrlen(suspect_string)+11+1);
									sprintf(temp4,"(%s) in value",suspect_string);
									string_temp=RDA_FormatOutputReportDate("Image Value","COPY-TO VALUE",temp2,temp3,"",temp4);
									if(temp2!=NULL) Rfree(temp2);
									if(temp3!=NULL) Rfree(temp3);
									if(temp4!=NULL) Rfree(temp4);
			
									addAPPlib(DateVirtuals,string_temp);
									RDA_fprintf(fp1,"%s\r\n",string_temp);
									if(string_temp!=NULL) Rfree(string_temp);
									checkheader(fp1,lines,pages,modname);
								}
							}
							if(!isEMPTY(p->fieldname))
							{
								suspect_string=ContainsADateField(p->fieldname,TRUE);
								if(suspect_string!=NULL)
								{
									if(*foundone==0) 
									{
										*foundone=1;
										string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
										sprintf(string_temp,"REPORT: [%s][%s]",
											(rpt_module!=NULL?rpt_module:""),
											(rpt->name!=NULL?rpt->name:""));
										addAPPlib(DateVirtuals,string_temp);
										RDA_fprintf(fp1,"%s\r\n",string_temp);
										if(string_temp!=NULL) Rfree(string_temp);
										checkheader(fp1,lines,pages,modname);
									}
									temp2=Rmalloc(6+1);
									sprintf(temp2,"%.6d",(x+1));
									temp3=Rmalloc(6+1);
									sprintf(temp3,"%.6d",(y+1));
									temp4=Rmalloc(RDAstrlen(suspect_string)+11+1);
									sprintf(temp4,"(%s) in value",suspect_string);
									string_temp=RDA_FormatOutputReportDate("Image Value","COPY-FROM VALUE",temp2,temp3,"",temp4);
									if(temp2!=NULL) Rfree(temp2);
									if(temp3!=NULL) Rfree(temp3);
									if(temp4!=NULL) Rfree(temp4);
			
									addAPPlib(DateVirtuals,string_temp);
									RDA_fprintf(fp1,"%s\r\n",string_temp);
									if(string_temp!=NULL) Rfree(string_temp);
									checkheader(fp1,lines,pages,modname);
	
								}
							}
							} else {		
							if(!isEMPTY(p->name))
							{
								suspect_string=ContainsADateField(p->name,FALSE);
								if(suspect_string!=NULL)
								{
									if(*foundone==0) 
									{
										*foundone=1;
										string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
										sprintf(string_temp,"REPORT: [%s][%s]",
											(rpt_module!=NULL?rpt_module:""),
											(rpt->name!=NULL?rpt->name:""));
										addAPPlib(DateVirtuals,string_temp);
										RDA_fprintf(fp1,"%s\r\n",string_temp);
										if(string_temp!=NULL) Rfree(string_temp);
										checkheader(fp1,lines,pages,modname);
									}
									temp2=Rmalloc(6+1);
									sprintf(temp2,"%.6d",(x+1));
									temp3=Rmalloc(6+1);
									sprintf(temp3,"%.6d",(y+1));
									temp4=Rmalloc(RDAstrlen(suspect_string)+11+1);
									sprintf(temp4,"(%s) in value",suspect_string);
									string_temp=RDA_FormatOutputReportDate("Image Value","COPY-FROM VALUE",temp2,temp3,"",temp4);
									if(temp2!=NULL) Rfree(temp2);
									if(temp3!=NULL) Rfree(temp3);
									if(temp4!=NULL) Rfree(temp4);
			
									addAPPlib(DateVirtuals,string_temp);
									RDA_fprintf(fp1,"%s\r\n",string_temp);
									if(string_temp!=NULL) Rfree(string_temp);
									checkheader(fp1,lines,pages,modname);
								}
							}
							if(!isEMPTY(p->fieldname))
							{
								suspect_string=ContainsADateField(p->fieldname,TRUE);
								if(suspect_string!=NULL)
								{
									if(*foundone==0) 
									{
										*foundone=1;
										string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
										sprintf(string_temp,"REPORT: [%s][%s]",
											(rpt_module!=NULL?rpt_module:""),
											(rpt->name!=NULL?rpt->name:""));
										addAPPlib(DateVirtuals,string_temp);
										RDA_fprintf(fp1,"%s\r\n",string_temp);
										if(string_temp!=NULL) Rfree(string_temp);
										checkheader(fp1,lines,pages,modname);
									}
									temp2=Rmalloc(6+1);
									sprintf(temp2,"%.6d",(x+1));
									temp3=Rmalloc(6+1);
									sprintf(temp3,"%.6d",(y+1));
									temp4=Rmalloc(RDAstrlen(suspect_string)+11+1);
									sprintf(temp4,"(%s) in value",suspect_string);
									string_temp=RDA_FormatOutputReportDate("Image Value","COPY-TO VALUE",temp2,temp3,"",temp4);
									if(temp2!=NULL) Rfree(temp2);
									if(temp3!=NULL) Rfree(temp3);
									if(temp4!=NULL) Rfree(temp4);
			
									addAPPlib(DateVirtuals,string_temp);
									RDA_fprintf(fp1,"%s\r\n",string_temp);
									if(string_temp!=NULL) Rfree(string_temp);
									checkheader(fp1,lines,pages,modname);
	
								}
							}
							}
						}
					}
				}
			}
		}
		if(rpt->MTNMSTR!=NULL)
		{
			/* make new function that is comes from datemtn for this one */
			MaintainMaster_ContainsDate(fp1,rpt->MTNMSTR,rpt,rpt_module,rptfile,lines,pages,modname,foundone);
		}
	}
}
static void MaintainMaster_ContainsDate(RDA_PFILE *fp1,MaintainMaster *p,RDAreport *rpt,char *rpt_module,char *rptfile,int *lines,int *pages,char *modname,int *foundone)
{
	int x,y,z;
	PowerWrite *AW=NULL;
	PowerWriteField *AWF=NULL;
	MaintainButton *BTN=NULL;
	RDAsearch *s=NULL;
	RDAsubord *addsub=NULL;
	MTNAction *act=NULL;
	Actionstruct *confld1=NULL;
	char *temp1=NULL,*temp2=NULL,*temp3=NULL,*temp4=NULL;
	char *string_temp=NULL;
	char *suspect_string=NULL;

	if(p!=NULL)
	{
		if(p->s!=NULL)
		{
			for(x=0,s=p->s;x<p->num;++x,++s)
			{
				if(!isEMPTY(s->expression))
				{
					suspect_string=ContainsADateField(s->expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}	
						temp1=Rmalloc(RDAstrlen(s->module)+RDAstrlen(s->filename)+5);
						sprintf(temp1,"[%s][%s]",(s->module!=NULL?s->module:""),(s->filename!=NULL?s->filename:""));
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+27+1);
						sprintf(temp3,"(%s) in expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Maintain Supporting",temp1,temp2,"","",temp3);
						if(temp1!=NULL) Rfree(temp1);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
			}
		}
		if(p->subord!=NULL)
		{
			for(x=0,addsub=p->subord;x<p->subnum;++x,++addsub)
			{
				if(!isEMPTY(addsub->expression))
				{
					suspect_string=ContainsADateField(addsub->expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}	
						temp1=Rmalloc(RDAstrlen(addsub->module)+RDAstrlen(addsub->filename)+5);
						sprintf(temp1,"[%s][%s]",(addsub->module!=NULL?addsub->module:""),(addsub->filename!=NULL?addsub->filename:""));
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+16+1);
						sprintf(temp3,"(%s) in expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Maintain Subordinate",temp1,temp2,"","",temp3);
						if(temp1!=NULL) Rfree(temp1);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);

						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
			}
		}
		if(!isEMPTY(p->save_error))
		{
			suspect_string=ContainsADateField(p->save_error,TRUE);
			if(suspect_string!=NULL)
			{
				if(*foundone==0) 
				{
					*foundone=1;
					string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
					sprintf(string_temp,"REPORT: [%s][%s]",
						(rpt_module!=NULL?rpt_module:""),
						(rpt->name!=NULL?rpt->name:""));
					addAPPlib(DateVirtuals,string_temp);
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,modname);
				}	
				temp3=Rmalloc(RDAstrlen(suspect_string)+16+1);
				sprintf(temp3,"(%s) in expression",suspect_string);
				string_temp=RDA_FormatOutputReportDate("Save Error","SAVE ERROR Expression","","","",temp3);

				if(temp3!=NULL) Rfree(temp3);
				addAPPlib(DateVirtuals,string_temp);
				RDA_fprintf(fp1,"%s\r\n",string_temp);
				if(string_temp!=NULL) Rfree(string_temp);
				checkheader(fp1,lines,pages,modname);
			}
		}
		if(!isEMPTY(p->save_error_desc))
		{
			suspect_string=ContainsADateField(p->save_error_desc,TRUE);
			if(suspect_string!=NULL)
			{
				if(*foundone==0) 
				{
					*foundone=1;
					string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
					sprintf(string_temp,"REPORT: [%s][%s]",
						(rpt_module!=NULL?rpt_module:""),
						(rpt->name!=NULL?rpt->name:""));
					addAPPlib(DateVirtuals,string_temp);
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,modname);
				}	
				temp3=Rmalloc(RDAstrlen(suspect_string)+16+1);
				sprintf(temp3,"(%s) in expression",suspect_string);
				string_temp=RDA_FormatOutputReportDate("Save Error Desc","SAVE ERROR DESCRIPTION Expression","","","",temp3);
				if(temp3!=NULL) Rfree(temp3);

				addAPPlib(DateVirtuals,string_temp);
				RDA_fprintf(fp1,"%s\r\n",string_temp);
				if(string_temp!=NULL) Rfree(string_temp);
				checkheader(fp1,lines,pages,modname);
			}
		}
		if(!isEMPTY(p->save_warning))
		{
			suspect_string=ContainsADateField(p->save_warning,TRUE);
			if(suspect_string!=NULL) 
			{
				if(*foundone==0) 
				{
					*foundone=1;
					string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
					sprintf(string_temp,"REPORT: [%s][%s]",
						(rpt_module!=NULL?rpt_module:""),
						(rpt->name!=NULL?rpt->name:""));
					addAPPlib(DateVirtuals,string_temp);
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,modname);
				}	
				temp3=Rmalloc(RDAstrlen(suspect_string)+16+1);
				sprintf(temp3,"(%s) in expression",suspect_string);
				string_temp=RDA_FormatOutputReportDate("Save Warning","SAVE WARNING Expression","","","",temp3);
				if(temp3!=NULL) Rfree(temp3);

				addAPPlib(DateVirtuals,string_temp);
				RDA_fprintf(fp1,"%s\r\n",string_temp);
				if(string_temp!=NULL) Rfree(string_temp);
				checkheader(fp1,lines,pages,modname);
			}
		}
		if(!isEMPTY(p->save_warning_desc))
		{
			suspect_string=ContainsADateField(p->save_warning_desc,TRUE);
			if(suspect_string!=NULL)
			{
				if(*foundone==0) 
				{
					*foundone=1;
					string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
					sprintf(string_temp,"REPORT: [%s][%s]",
						(rpt_module!=NULL?rpt_module:""),
						(rpt->name!=NULL?rpt->name:""));
					addAPPlib(DateVirtuals,string_temp);
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,modname);
				}	
				temp3=Rmalloc(RDAstrlen(suspect_string)+16+1);
				sprintf(temp3,"(%s) in expression",suspect_string);
				string_temp=RDA_FormatOutputReportDate("Save Warning Desc","SAVE WARNING DESC Expression","","","",temp3);
				if(temp3!=NULL) Rfree(temp3);

				addAPPlib(DateVirtuals,string_temp);
				RDA_fprintf(fp1,"%s\r\n",string_temp);
				if(string_temp!=NULL) Rfree(string_temp);
				checkheader(fp1,lines,pages,modname);
			}
		}
		if(!isEMPTY(p->save_expression))
		{
			suspect_string=ContainsADateField(p->save_expression,TRUE);
			if(suspect_string!=NULL)
			{
				if(*foundone==0) 
				{
					*foundone=1;
					string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
					sprintf(string_temp,"REPORT: [%s][%s]",
						(rpt_module!=NULL?rpt_module:""),
						(rpt->name!=NULL?rpt->name:""));
					addAPPlib(DateVirtuals,string_temp);
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,modname);
				}	
				temp3=Rmalloc(RDAstrlen(suspect_string)+16+1);
				sprintf(temp3,"(%s) in expression",suspect_string);
				string_temp=RDA_FormatOutputReportDate("Save Expression","SAVE EXPRESSION","","","",temp3);
				if(temp3!=NULL) Rfree(temp3);

				addAPPlib(DateVirtuals,string_temp);
				RDA_fprintf(fp1,"%s\r\n",string_temp);
				if(string_temp!=NULL) Rfree(string_temp);
				checkheader(fp1,lines,pages,modname);
			}
		}
		if(p->PrevWrites!=NULL)
		{
			for(x=0,AW=p->PrevWrites;x<p->numPrevWrites;++x,++AW)
			{
				if(!isEMPTY(AW->expression))
				{
					suspect_string=ContainsADateField(AW->expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}	
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp3=Rmalloc(RDAstrlen(suspect_string)+16+1);
						sprintf(temp3,"(%s) in expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Prev Write","PREVIOUS WRITE EXPRESSION",temp2,"","",temp3);
						if(temp2!=NULL) Rfree(temp2);
						if(temp3!=NULL) Rfree(temp3);
		
						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
				if(AW->fields!=NULL)
				{
					for(y=0,AWF=AW->fields;y<AW->numflds;++y,++AWF)
					{
						if(!isEMPTY(AWF->from_name))
						{
							suspect_string=ContainsADateField(AWF->from_name,TRUE);
							if(suspect_string!=NULL)
							{
								if(*foundone==0) 
								{
									*foundone=1;
									string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
									sprintf(string_temp,"REPORT: [%s][%s]",
										(rpt_module!=NULL?rpt_module:""),
										(rpt->name!=NULL?rpt->name:""));
									addAPPlib(DateVirtuals,string_temp);
									RDA_fprintf(fp1,"%s\r\n",string_temp);
									if(string_temp!=NULL) Rfree(string_temp);
									checkheader(fp1,lines,pages,modname);
								}	
								temp2=Rmalloc(6+1);
								sprintf(temp2,"%.6d",(x+1));
								temp3=Rmalloc(6+1);
								sprintf(temp3,"%.6d",(y+1));
								temp4=Rmalloc(RDAstrlen(suspect_string)+16+1);
								sprintf(temp4,"(%s) in expression",suspect_string);
								string_temp=RDA_FormatOutputReportDate("Prev Write From","PREVIOUS WRITE Expression",temp2,temp3,"",temp4);
								if(temp2!=NULL) Rfree(temp2);
								if(temp3!=NULL) Rfree(temp3);
								if(temp4!=NULL) Rfree(temp4);
				
								addAPPlib(DateVirtuals,string_temp);
								RDA_fprintf(fp1,"%s\r\n",string_temp);
								if(string_temp!=NULL) Rfree(string_temp);
								checkheader(fp1,lines,pages,modname);
							}
						}
					}
				}
			}
		}
		if(p->NewWrites!=NULL)
		{
			for(x=0,AW=p->NewWrites;x<p->numNewWrites;++x,++AW)
			{
				if(!isEMPTY(AW->expression))
				{
					suspect_string=ContainsADateField(AW->expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}	
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp4=Rmalloc(RDAstrlen(suspect_string)+16+1);
						sprintf(temp4,"(%s) in expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("New Write","NEW WRITE EXPRESSION",temp2,"","",temp4);
						if(temp2!=NULL) Rfree(temp2);
						if(temp4!=NULL) Rfree(temp4);
		
						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
				if(AW->fields!=NULL)
				{
					for(y=0,AWF=AW->fields;y<AW->numflds;++y,++AWF)
					{
						if(!isEMPTY(AWF->from_name))
						{
							suspect_string=ContainsADateField(AWF->from_name,TRUE);
							if(suspect_string!=NULL)
							{
								if(*foundone==0) 
								{
									*foundone=1;
									string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
									sprintf(string_temp,"REPORT: [%s][%s]",
										(rpt_module!=NULL?rpt_module:""),
										(rpt->name!=NULL?rpt->name:""));
									addAPPlib(DateVirtuals,string_temp);
									RDA_fprintf(fp1,"%s\r\n",string_temp);
									if(string_temp!=NULL) Rfree(string_temp);
									checkheader(fp1,lines,pages,modname);
								}	
								temp2=Rmalloc(6+1);
								sprintf(temp2,"%.6d",(x+1));
								temp3=Rmalloc(6+1);
								sprintf(temp3,"%.6d",(y+1));
								temp4=Rmalloc(RDAstrlen(suspect_string)+16+1);
								sprintf(temp4,"(%s) in expression",suspect_string);
								string_temp=RDA_FormatOutputReportDate("New Write From",AWF->from_name,temp2,temp3,"",temp4);
								if(temp2!=NULL) Rfree(temp2);
								if(temp3!=NULL) Rfree(temp3);
								if(temp4!=NULL) Rfree(temp4);
				
								addAPPlib(DateVirtuals,string_temp);
								RDA_fprintf(fp1,"%s\r\n",string_temp);
								if(string_temp!=NULL) Rfree(string_temp);
								checkheader(fp1,lines,pages,modname);
							}
						}
					}
				}
			}
		}
		if(p->buttons!=NULL)
		{
			for(x=0,BTN=p->buttons;x<p->numbuttons;++x,++BTN)
			{
				if(!isEMPTY(BTN->expression))
				{
					suspect_string=ContainsADateField(BTN->expression,TRUE);
					if(suspect_string!=NULL)
					{
						if(*foundone==0) 
						{
							*foundone=1;
							string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
							sprintf(string_temp,"REPORT: [%s][%s]",
								(rpt_module!=NULL?rpt_module:""),
								(rpt->name!=NULL?rpt->name:""));
							addAPPlib(DateVirtuals,string_temp);
							RDA_fprintf(fp1,"%s\r\n",string_temp);
							if(string_temp!=NULL) Rfree(string_temp);
							checkheader(fp1,lines,pages,modname);
						}	
						temp2=Rmalloc(6+1);
						sprintf(temp2,"%.6d",(x+1));
						temp4=Rmalloc(RDAstrlen(suspect_string)+16+1);
						sprintf(temp4,"(%s) in expression",suspect_string);
						string_temp=RDA_FormatOutputReportDate("Maintain Button","BUTTON EXPRESSION",temp2,"","",temp4);
						if(temp2!=NULL) Rfree(temp2);
						if(temp4!=NULL) Rfree(temp4);
		
						addAPPlib(DateVirtuals,string_temp);
						RDA_fprintf(fp1,"%s\r\n",string_temp);
						if(string_temp!=NULL) Rfree(string_temp);
						checkheader(fp1,lines,pages,modname);
					}
				}
			}
		}
		if(p->Actions!=NULL)
		{
			for(x=0,act=p->Actions;x<p->numActions;++x,++act)
			{
				if(act->a!=NULL)
				{
					for(z=0,confld1=act->a;z<act->num;++z,++confld1)
					{
						if(!isEMPTY(confld1->expression))
						{
							suspect_string=ContainsADateField(confld1->expression,TRUE);
							if(suspect_string!=NULL)
							{
								if(*foundone==0) 
								{
									*foundone=1;
									string_temp=Rmalloc(RDAstrlen(rpt_module)+RDAstrlen(rpt->name)+14); 
									sprintf(string_temp,"REPORT: [%s][%s]",
										(rpt_module!=NULL?rpt_module:""),
										(rpt->name!=NULL?rpt->name:""));
									addAPPlib(DateVirtuals,string_temp);
									RDA_fprintf(fp1,"%s\r\n",string_temp);
									if(string_temp!=NULL) Rfree(string_temp);
									checkheader(fp1,lines,pages,modname);
								}	
								temp2=Rmalloc(6+1);
								sprintf(temp2,"%.6d",(x+1));
								temp3=Rmalloc(6+1);
								sprintf(temp3,"%.6d",(z+1));
								temp4=Rmalloc(RDAstrlen(suspect_string)+16+1);
								sprintf(temp4,"(%s) in expression",suspect_string);
								string_temp=RDA_FormatOutputReportDate("Maintain Action","ACTION EXPRESSION",temp2,temp3,"",temp4);
								if(temp2!=NULL) Rfree(temp2);
								if(temp3!=NULL) Rfree(temp3);
								if(temp4!=NULL) Rfree(temp4);
			
								addAPPlib(DateVirtuals,string_temp);
								RDA_fprintf(fp1,"%s\r\n",string_temp);
								if(string_temp!=NULL) Rfree(string_temp);
								checkheader(fp1,lines,pages,modname);
							}
						}
					}
				}
			}
		}
	}
}
static char *ContainsADateField(char *expression,char check_systemdate)
{
	int x=0;
	
	if(isEMPTY(expression))
	{
		return(NULL);
	}
	if(check_systemdate==TRUE)
	{
		if(RDAstrstr(expression,"[SYSTEM DATE]")!=NULL)
		{
			return("[SYSTEM DATE]");
		}
		if(RDAstrstr(expression,"[SYSTEM DATE-YYYY]")!=NULL)
		{
			return("[SYSTEM DATE-YYYY]");
		}
	}
	if(DateFields!=NULL)
	{
		for(x=0;x<DateFields->numlibs;++x)
		{
			if(RDAstrstr(expression,DateFields->libs[x])!=NULL)
			{
				return(DateFields->libs[x]);
			}
		}
		if(x>=DateFields->numlibs)
		{
			if(RDAstrstr(expression,"DATE")!=NULL)
			{
				return(expression);
			}
		}
	}
	return(NULL);
}
static void checkheader(RDA_PFILE *fp,int *lines,int *pages,char *modname)
{
	char *temp=NULL;

	if(*lines>60 || (*lines==0 && *pages==0))
	{
		if(*lines>60)
		{
			RDA_fprintf(fp,"\f");
		}
		if(XPERT_SETUP->software_type==2)
		{
			temp=stralloc("MBGUI SUSPECT REPORT VALUES CONTAINING DATES"); 
			printheader(fp,lines,pages,modname,temp);
			if(temp!=NULL) Rfree(temp);
		} else {
			temp=stralloc("XPERT SUSPECT REPORT VALUES CONTAINING DATES"); 
			printheader(fp,lines,pages,modname,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *modname,char *header)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=131;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\r\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",header,tail_ws,"",10,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(10+RDAstrlen(modname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(10+RDAstrlen(modname)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	RDA_fprintf(fp,"%*s Module: %s %*sDate: %s\r\n",
		lead_ws,"",(modname!=NULL?modname:""),tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\r\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	RDA_fprintf(fp,"\r\n");
	++*lines;
	RDA_fprintf(fp,"    TYPE                 IDENTIFIER                      OFFSET OFFST2 OFFST3 REASON                                               \r\n");
	++*lines;
	RDA_fprintf(fp,"    -------------------- ------------------------------- ------ ------ ------ ------------------------------------------------------\r\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void daterpt(RDArsrc *mainrsrc)
{
	RDA_PFILE *fp1=NULL;
	char *outputdevice=NULL;
	char *prtmessage=NULL;
	char *dirx=NULL,*tmp=NULL,*libx=NULL,*temp=NULL;
	char rng_mod=FALSE,*mod_from=NULL,*mod_to=NULL;
	char *rpt_module=NULL;
	int x=0,y=0;
	APPlib *modlst=NULL,*file_def=NULL,*reports=NULL;
	short fileno=(-1),nofields=0,z=0;
	NRDfield *fields=NULL,*field=NULL;
	RDAreport *rpt=NULL;
	char string_temp[256];
	int lines=0;
	int pages=0;

	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON REPORT",&rng_mod);
	FINDRSCGETSTRING(mainrsrc,"MODULE",&rpt_module);
	FINDRSCGETSTRING(mainrsrc,"FROM REPORT",&mod_from);
	FINDRSCGETSTRING(mainrsrc,"TO REPORT",&mod_to);
	FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&outputdevice);
	if(DateFields!=NULL) freeapplib(DateFields);
	DateFields=APPlibNEW();
	if(DateVirtuals!=NULL) freeapplib(DateVirtuals);
	DateVirtuals=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	modlst=APPlibNEW();
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(RDAstrcmp(tmp,"RPTGEN"))
		{
			addAPPlib(modlst,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modlst->numlibs<1)
	{
		if(modlst!=NULL) freeapplib(modlst);
		prterr("Error No Databases for any module to search with....");
		return;
	}
	for(x=0;x<modlst->numlibs;++x)
	{
		if(file_def!=NULL) freeapplib(file_def); 
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
		file_def=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_def!=NULL)
		{
			for(y=0;y<file_def->numlibs;++y)
			{
				fileno=OPNNRD(modlst->libs[x],file_def->libs[y]);
				if(fileno!=(-1))
				{
					fields=FLDPOINTER(fileno);
					nofields=NUMFLDS(fileno);
					if(fields!=NULL)
					{
						for(z=0,field=fields;z<nofields;++z,++field)
						{
							if(field->type==DATES)
							{
							temp=Rmalloc(RDAstrlen(modlst->libs[x])+RDAstrlen(file_def->libs[y])+RDAstrlen(field->name)+7);
							sprintf(temp,"[%s][%s][%s]",modlst->libs[x],file_def->libs[y],field->name);
							addAPPlib(DateFields,temp);
							if(temp!=NULL) Rfree(temp);
							}
						}
					}
					CLSNRD(fileno);
				} else {
					prterr("Error:  Search will not include fields from database [%s][%s], it was unable to be opened.",modlst->libs[x],file_def->libs[y]);
				}
			}
			if(file_def!=NULL) freeapplib(file_def);
		}
	}
	if(modlst!=NULL) freeapplib(modlst);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,(rpt_module!=NULL ? rpt_module:""));
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,(rpt_module!=NULL ? rpt_module:""));
#endif
	reports=APPlibNEW();
	for(x=0;findfile(dirx,"*.RPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(rng_mod==TRUE)
		{
			if((RDAstrcmp(tmp,mod_from)>=0) &&
				(RDAstrcmp(tmp,mod_to)<=0))
			{
				addAPPlib(reports,tmp);
			}
		} else { 
			addAPPlib(reports,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(reports->numlibs<1)
	{
		memset(string_temp,0,256);
		sprintf(string_temp,"There were no reports selected from the given criteria!");
		addAPPlib(DateVirtuals,string_temp);
/*
		if(rpt_module!=NULL) Rfree(rpt_module0;
		if(mod_from!=NULL) Rfree(mod_from);
		if(mod_to!=NULL) Rfree(mod_to);
		return;
*/
	}
	SORTAPPlib(reports);
	if((fp1=RDA_popen(outputdevice,"w"))==NULL)
	{			
		prtmessage=Rmalloc(RDAstrlen(outputdevice)+69);
		sprintf(prtmessage,"The output device [%s] does not exist or cannot otherwise be opened!",outputdevice);
		ERRORDIALOG("Output Device Failure",prtmessage,NULL,FALSE);
		if(prtmessage!=NULL) Rfree(prtmessage);
		return;
	} else {
		lines=0;
		pages=0;
		checkheader(fp1,&lines,&pages,rpt_module);
		if(reports!=NULL)
		{
			if(reports->numlibs>0)
			{
				for(x=0;x<reports->numlibs;++x)
				{
					rpt=GETRDAreport(rpt_module,reports->libs[x]);
					if(rpt!=NULL)
					{
						REPORT_ContainsDate(fp1,rpt,rpt_module,reports->libs[x],&lines,&pages,rpt_module);
						FreeRDAreport(rpt);
					} else {
						prterr("Error:  Report [%s][%s] could not be read.",(rpt_module!=NULL ? rpt_module:""),(reports->libs[x]!=NULL ? reports->libs[x]:""));
					}
				}
			} else {
				RDA_fprintf(fp1,"There were no reports selected that contained suspect values!\r\n");
				checkheader(fp1,&lines,&pages,rpt_module);
			}
		} else {
			RDA_fprintf(fp1,"There were no reports selected that contained suspect values!\r\n");
			checkheader(fp1,&lines,&pages,rpt_module);
		}
	}
/*

	if(DateVirtuals!=NULL)
	{
		for(y=0;y<DateVirtuals->numlibs;++y)
		{
			RDA_fprintf(fp1,"%s\r\n",DateVirtuals->libs[y]);
			checkheader(fp1,&lines,&pages)
		}
	} else {
		RDA_fprintf(fp1,"There were no reports selected that contained suspect values!\r\n");
		checkheader(fp1,&lines,&pages)
	}
*/
	RDA_pclose(fp1);
	quitfunc(mainrsrc);
}
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(DateFields!=NULL) freeapplib(DateFields);
	if(DateVirtuals!=NULL) freeapplib(DateVirtuals);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char b=FALSE;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"FIND DATE REPORT")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"FIND DATE REPORT");
	addstdrsrc(mainrsrc,"MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"RANGE ON REPORT",BOOLNS,1,&b,TRUE);
	addstdrsrc(mainrsrc,"FROM REPORT",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO REPORT",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,NULL,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"SELECT",TRUE,daterpt,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADV2GET_SCREEN_VIRTUALS(mainrsrc,0,FALSE,TRUE,FALSE);	
	ADVSCRNvirtual2rsrc(mainrsrc,SCRNvirtualSubData,mainrsrc);
	SetVirFieldFuncs(mainrsrc);

	ADVcomputeandupdateallSCRNvirtuals(mainrsrc,SCRNvirtualSubData,mainrsrc,TRUE);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
