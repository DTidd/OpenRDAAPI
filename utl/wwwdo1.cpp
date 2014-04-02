#include<rptgen.hpp>
#include<sys/stat.h>
#include <curl/curl.h>
#include <curl/easy.h>

struct FtpFile {
	char *filename;
	FILE *stream;
};
int getftpfile(void *buffer,size_t size,size_t nmemb,void *stream)
{
	struct FtpFile *out=(struct FtpFile *)stream;

	if(out && !out->stream)
	{
		out->stream=fopen(out->filename,"wb");
		if(!out->stream) return(-1);
	}
	return(fwrite(buffer,size,nmemb,out->stream));
}
void WWWprocess_dataline(RDArunrpt *rrpt,RDAreport *rpt,RDAimage *image,
	short update)
{
	char *modx=NULL,*filx=NULL,*temp,*fieldname=NULL;
	char *sm1=NULL,*sm2=NULL,*sm3=NULL,*sm4=NULL,*sm5=NULL;
	char *sm6=NULL,temp1[1024];
	ReportFile *fx;
	int x,y,count=0,hd=0;
	NRDfield *field,*field2,*field3,*field4,*field5,*field6;
	RDApval *pval;
	char *trnfield=NULL;
	HoldReport *h=NULL;
	RDATData *prev=NULL;
	CURL *RDAcurl=NULL;
	CURLcode res=0;
	FILE *fp=NULL;
	struct stat file_info;
	struct FtpFile plst={NULL,NULL};

	MAIL_VARS *email_vars=NULL;
	email_vars=Rmalloc(sizeof(MAIL_VARS));
	email_vars->from_name=NULL;
	email_vars->from_addr=NULL;
	email_vars->subject=NULL;
	email_vars->body=NULL;
	email_vars->tolist=NULL;
	email_vars->cclist=NULL;
	email_vars->bcclist=NULL;
	email_vars->ignerr=1;
	for(x=0;x<10;x++)
	{
		email_vars->fname[x]=NULL;
		email_vars->mtype[x]=NULL;
		email_vars->dname[x]=NULL;
		email_vars->fdata[x]=NULL;
	}


	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG WWWprocess_dataline Process Image Line [%s] for writing to database.",image->line);
	}
	if(!strncmp(&image->line[0],"OPEN",4) || 
		!strncmp(&image->line[0],"open",4)) /* open data file */
	{
		temp=image->line+4;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		filx=stripfilename(temp);
		if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
		{
			if(addReportFile(rrpt,modx,filx,TRUE,0,FALSE)==(-1))
			{
				prterr("Error Module [%s] File [%s] couldn't be opened.",
					modx,filx);
			}
		} else {
			prterr("Error Module [%s] File [%s] names in error.",(modx!=NULL ?
				modx:"NULL"),(filx!=NULL ? filx:"NULL"));
		}
		if(modx!=NULL) Rfree(modx);
		if(filx!=NULL) Rfree(filx);
	} else if(!strncmp(&image->line[0],"CLOSE",5) ||
		!strncmp(&image->line[0],"close",5)) /* close data file */
	{
		temp=image->line+5;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		filx=stripfilename(temp);
		if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
		{
			if(rrpt->ofiles!=NULL)
			{
				for(x=0,fx=rrpt->ofiles;x<rrpt->numofiles;++x,++fx)
				{
					if(fx->fileno!=(-1) && !RDAstrcmp(fx->module,modx)
						&& !RDAstrcmp(fx->filename,filx)) 
					{
						CLSNRD(fx->fileno);
						fx->fileno=(-1);
						break;
					}
				}
			}
		} else {
			prterr("Error Module [%s] File [%s] names in error cannot CLOSE.",
				(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
		}
		if(modx!=NULL) Rfree(modx);
		if(filx!=NULL) Rfree(filx);

	} else if(!strncmp(&image->line[0],"SENDATTACH-1",12) || 
		!strncmp(&image->line[0],"sendattach-1",12))
	{
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"NAME OF SENDER"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_name=stralloc(field->data.string_value);
					} else email_vars->from_name=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SENDER EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_addr=stralloc(field->data.string_value);
					} else email_vars->from_addr=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SUBJECT"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->subject=stralloc(field->data.string_value);
					} else email_vars->subject=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BODY"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->body=stralloc(field->data.string_value);
					} else email_vars->body=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"RECIPIENT EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->tolist=stralloc(field->data.string_value);
					} else email_vars->tolist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"FILENAME TO BE ATTACHED"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[0]=stralloc(field->data.string_value);
					} else email_vars->fname[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"EMAIL ATTACHMENT TO BE NAMED"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[0]=stralloc(field->data.string_value);
					} else email_vars->dname[0]=NULL;
				}
			}
			if(field!=NULL)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				Rfree(field);
			}

			if(!isEMPTY(email_vars->from_name) && !isEMPTY(email_vars->from_addr) && 
				!isEMPTY(email_vars->subject) &&!isEMPTY(email_vars->body) && 
				!isEMPTY(email_vars->tolist) && !isEMPTY(email_vars->fname[0]) && !isEMPTY(email_vars->dname[0]))
			{
				RunVMimeSendmail(email_vars);
			} else {
				prterr("Error: sendattach-1 Requires 1st 6 parameters established.");
				prterr("\t\t               NAME OF SENDER: [%s] ",(email_vars->from_name!=NULL ? email_vars->from_name:""));
				prterr("\t\t         SENDER EMAIL ADDRESS: [%s] ",(email_vars->from_addr!=NULL ? email_vars->from_addr:""));
				prterr("\t\t                      SUBJECT: [%s] ",(email_vars->subject!=NULL ? email_vars->subject:""));
				prterr("\t\t                         BODY: [%s] ",(email_vars->body!=NULL ? email_vars->body:""));
				prterr("\t\t      RECIPIENT EMAIL ADDRESS: [%s] ",(email_vars->tolist!=NULL ? email_vars->tolist:""));
				prterr("\t\t       FILENAME TO BE ATTACHED [%s] ",(email_vars->fname[0]!=NULL ? email_vars->fname[0]:""));
				prterr("\t\t EMAIL ATTACHMENT TO BE NAMED: [%s] ",(email_vars->dname[0]!=NULL ? email_vars->dname[0]:""));
			}
			if(email_vars!=NULL) Rfree(email_vars);
		}
	} else if(!strncmp(&image->line[0],"SENDMAIL",8) || 
		!strncmp(&image->line[0],"sendmail",8))
	{
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"NAME OF SENDER"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_name=stralloc(field->data.string_value);
					} else email_vars->from_name=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SENDER EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_addr=stralloc(field->data.string_value);
					} else email_vars->from_addr=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SUBJECT"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->subject=stralloc(field->data.string_value);
					} else email_vars->subject=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BODY"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->body=stralloc(field->data.string_value);
					} else email_vars->body=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"RECIPIENT EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->tolist=stralloc(field->data.string_value);
					} else email_vars->tolist=NULL;
				}
			}
			if(field!=NULL)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				Rfree(field);
			}
			if(!isEMPTY(email_vars->from_name) && !isEMPTY(email_vars->from_addr) && !isEMPTY(email_vars->subject) &&
				!isEMPTY(email_vars->body) && !isEMPTY(email_vars->tolist))
			{
				RunVMimeSendmail(email_vars);
			} else {
				prterr("Error: SENDMAIL Requires all 5 parameters established.");
			}
			if(email_vars!=NULL) Rfree(email_vars);
		}
	} else if(!strncmp(&image->line[0],"SEND EMAIL",10) || 
		!strncmp(&image->line[0],"send email",10))
	{
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"NAME OF SENDER"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_name=stralloc(field->data.string_value);
					} else email_vars->from_name=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SENDER EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_addr=stralloc(field->data.string_value);
					} else email_vars->from_addr=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SUBJECT"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->subject=stralloc(field->data.string_value);
					} else email_vars->subject=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BODY"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->body=stralloc(field->data.string_value);
					} else email_vars->body=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"TO RECIPIENTS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->tolist=stralloc(field->data.string_value);
					} else email_vars->tolist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"CC RECIPIENTS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->cclist=stralloc(field->data.string_value);
					} else email_vars->cclist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BCC RECIPIENTS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->bcclist=stralloc(field->data.string_value);
					} else email_vars->bcclist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"IGNORE ERRORS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->ignerr=atoi(field->data.string_value);
					} else email_vars->ignerr=0;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[0]=stralloc(field->data.string_value);
					} else email_vars->fname[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[1]=stralloc(field->data.string_value);
					} else email_vars->fname[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[2]=stralloc(field->data.string_value);
					} else email_vars->fname[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[3]=stralloc(field->data.string_value);
					} else email_vars->fname[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[4]=stralloc(field->data.string_value);
					} else email_vars->fname[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[5]=stralloc(field->data.string_value);
					} else email_vars->fname[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[6]=stralloc(field->data.string_value);
					} else email_vars->fname[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[7]=stralloc(field->data.string_value);
					} else email_vars->fname[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[8]=stralloc(field->data.string_value);
					} else email_vars->fname[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[9]=stralloc(field->data.string_value);
					} else email_vars->fname[9]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[0]=stralloc(field->data.string_value);
					} else email_vars->fdata[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[1]=stralloc(field->data.string_value);
					} else email_vars->fdata[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[2]=stralloc(field->data.string_value);
					} else email_vars->fdata[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[3]=stralloc(field->data.string_value);
					} else email_vars->fdata[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[4]=stralloc(field->data.string_value);
					} else email_vars->fdata[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[5]=stralloc(field->data.string_value);
					} else email_vars->fdata[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[6]=stralloc(field->data.string_value);
					} else email_vars->fdata[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[7]=stralloc(field->data.string_value);
					} else email_vars->fdata[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[8]=stralloc(field->data.string_value);
					} else email_vars->fdata[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[9]=stralloc(field->data.string_value);
					} else email_vars->fdata[9]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[0]=stralloc(field->data.string_value);
					} else email_vars->dname[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[1]=stralloc(field->data.string_value);
					} else email_vars->dname[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[2]=stralloc(field->data.string_value);
					} else email_vars->dname[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[3]=stralloc(field->data.string_value);
					} else email_vars->dname[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[4]=stralloc(field->data.string_value);
					} else email_vars->dname[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[5]=stralloc(field->data.string_value);
					} else email_vars->dname[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[6]=stralloc(field->data.string_value);
					} else email_vars->dname[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[7]=stralloc(field->data.string_value);
					} else email_vars->dname[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[8]=stralloc(field->data.string_value);
					} else email_vars->dname[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[9]=stralloc(field->data.string_value);
					} else email_vars->dname[9]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[0]=stralloc(field->data.string_value);
					} else email_vars->mtype[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[1]=stralloc(field->data.string_value);
					} else email_vars->mtype[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[2]=stralloc(field->data.string_value);
					} else email_vars->mtype[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[3]=stralloc(field->data.string_value);
					} else email_vars->mtype[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[4]=stralloc(field->data.string_value);
					} else email_vars->mtype[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[5]=stralloc(field->data.string_value);
					} else email_vars->mtype[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[6]=stralloc(field->data.string_value);
					} else email_vars->mtype[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[7]=stralloc(field->data.string_value);
					} else email_vars->mtype[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[8]=stralloc(field->data.string_value);
					} else email_vars->mtype[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[9]=stralloc(field->data.string_value);
					} else email_vars->mtype[9]=NULL;
				}
			}
			if(field!=NULL)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				Rfree(field);
			}
			if(!isEMPTY(email_vars->from_name) && !isEMPTY(email_vars->from_addr) && !isEMPTY(email_vars->subject) &&
				!isEMPTY(email_vars->body) && ((!isEMPTY(email_vars->tolist)) || (!isEMPTY(email_vars->cclist)) || (!isEMPTY(email_vars->bcclist))))
			{
				RunVMimeSendmail(email_vars);
			} else {
				prterr("Error: SEND EMAIL Requires From User Info, Valid Recipients, a Subject and a Body to be set at a minimum.");
			}
			if(email_vars!=NULL) Rfree(email_vars);
		}
	} else if(!strncmp(&image->line[0],"WRITE BLOB",10) || 
		!strncmp(&image->line[0],"write blob",10))
	{
#ifdef USE_MYSQL
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;

			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;

			field3=Rmalloc(sizeof(NRDfield));
			field3->data.string_value=NULL;
			field3->dlen=0;
			field3->type=VARIABLETEXT;
			field3->len=0;

			field4=Rmalloc(sizeof(NRDfield));
			field4->data.string_value=NULL;
			field4->dlen=0;
			field4->type=VARIABLETEXT;
			field4->len=0;

			field5=Rmalloc(sizeof(NRDfield));
			field5->data.string_value=NULL;
			field5->dlen=0;
			field5->type=VARIABLETEXT;
			field5->len=0;

			field6=Rmalloc(sizeof(NRDfield));
			field6->data.string_value=NULL;
			field6->dlen=0;
			field6->type=VARIABLETEXT;
			field6->len=0;

			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"DATABASE"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"TABLE"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"FIELD LIST"))
				{
					if(field3->data.string_value!=NULL) Rfree(field3->data.string_value);
					field3->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field3,pval->name,rpt,rrpt);
					} else {
						field3->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"KEY VALUES"))
				{
					if(field4->data.string_value!=NULL) Rfree(field4->data.string_value);
					field4->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field4,pval->name,rpt,rrpt);
					} else {
						field4->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"DECODING METHOD"))
				{
					if(field5->data.string_value!=NULL) Rfree(field5->data.string_value);
					field5->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field5,pval->name,rpt,rrpt);
					} else {
						field5->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					if(field6->data.string_value!=NULL) Rfree(field6->data.string_value);
					field6->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field6,pval->name,rpt,rrpt);
					} else {
						field6->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			download_blob_data(field->data.string_value,field2->data.string_value,field3->data.string_value,field4->data.string_value,field5->data.string_value,field6->data.string_value);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
			FreeNRDfieldvalue(field3);
			FreeNRDfieldvalue(field4);
			FreeNRDfieldvalue(field5);
			FreeNRDfieldvalue(field6);
		}
		if(h!=NULL) Rfree(h);
#endif
	} else if(!strncmp(&image->line[0],"PUTENV",6) ||
		!strncmp(&image->line[0],"RDA_PutEnv",6))
	{
		if(image->pvals!=NULL)
		{
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!isEMPTY(pval->fieldname))
				{
				field=Rmalloc(sizeof(NRDfield));
				field->data.string_value=NULL;
				field->dlen=0;
				field->type=VARIABLETEXT;
				field->len=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!isEMPTY(field->data.string_value))
				{
					sprintf(temp1,"%s=%s",pval->fieldname,field->data.string_value);
				} else sprintf(temp1,"%s=",pval->fieldname);
				if(field!=NULL)
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					Rfree(field);
				}
				}
			}
		}
	} else if(!strncmp(&image->line[0],"HTTP POST",9) || 
		!strncmp(&image->line[0],"http post",9))
	{
	} else if(!strncmp(&image->line[0],"FTPPUT",6) || 
		!strncmp(&image->line[0],"ftpput",6))
	{
		if(image->pvals!=NULL)
		{
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"URL"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm1=stralloc(field->data.string_value);
					} else sm1=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"USER ID"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm2=stralloc(field->data.string_value);
					} else sm2=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"PASSWORD"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm3=stralloc(field->data.string_value);
					} else sm3=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"PROXY"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm4=stralloc(field->data.string_value);
					} else sm4=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"PROXY PORT"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.integer_value=Rmalloc(sizeof(int));
					*field->data.integer_value=0;
					field->dlen=sizeof(int);
					field->type=LONGV;
					field->len=4;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					sprintf(stemp,"%d",*field->data.integer_value);
					sm5=stralloc(stemp);
					if(field!=NULL)
					{
						if(field->data.integer_value!=NULL) Rfree(field->data.integer_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm6=stralloc(field->data.string_value);
					} else sm6=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				}
			}
		}
		if(!isEMPTY(sm1) && !isEMPTY(sm6))
		{
			hd=open(sm6,O_RDONLY);
			if(hd!=(-1))
			{
				fstat(hd,&file_info);
				close(hd);
			}

			fp=fopen(sm6,"rb");
			if(fp!=NULL)
			{
				RDAcurl=curl_easy_init();
				curl_easy_setopt(RDAcurl,CURLOPT_URL,sm1);
				if(!isEMPTY(sm2))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s:%s",sm2,sm3);
					curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,(char *)stemp);
				}
				if(!isEMPTY(sm4))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s%s%s",sm4,(sm5!=NULL ? ":":""),(sm5!=NULL ? sm5:""));
					curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
				}
	
				curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE);
				curl_easy_setopt(RDAcurl,CURLOPT_UPLOAD,TRUE);
				curl_easy_setopt(RDAcurl,CURLOPT_INFILE,fp);
				curl_easy_setopt(RDAcurl,CURLOPT_INFILESIZE,file_info.st_size);
				res=curl_easy_perform(RDAcurl);
				curl_easy_cleanup(RDAcurl);
				RDAcurl=NULL;
				if(CURLE_OK!=res)
				{
					prterr("Error:  Failed to FTPPUT file [%s] with error [%d].",sm6,res);
				}
				fclose(fp);
			}
		}
		if(sm1!=NULL) Rfree(sm1);
		if(sm2!=NULL) Rfree(sm2);
		if(sm3!=NULL) Rfree(sm3);
		if(sm4!=NULL) Rfree(sm4);
		if(sm5!=NULL) Rfree(sm5);
		if(sm6!=NULL) Rfree(sm6);
	} else if(!strncmp(&image->line[0],"FTPGET",6) || 
		!strncmp(&image->line[0],"ftpget",6))
	{
		if(image->pvals!=NULL)
		{
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"URL"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm1=stralloc(field->data.string_value);
					} else sm1=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"USER ID"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm2=stralloc(field->data.string_value);
					} else sm2=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"PASSWORD"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm3=stralloc(field->data.string_value);
					} else sm3=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"PROXY"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm4=stralloc(field->data.string_value);
					} else sm4=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"PROXY PORT"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.integer_value=Rmalloc(sizeof(int));
					*field->data.integer_value=0;
					field->dlen=sizeof(int);
					field->type=LONGV;
					field->len=4;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					sprintf(stemp,"%d",*field->data.integer_value);
					sm5=stralloc(stemp);
					if(field!=NULL)
					{
						if(field->data.integer_value!=NULL) Rfree(field->data.integer_value);
						Rfree(field);
					}
				} else if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
					if(!isEMPTY(field->data.string_value))
					{
						sm6=stralloc(field->data.string_value);
					} else sm6=NULL;
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						Rfree(field);
					}
				}
			}
		}


		if(!isEMPTY(sm1) && !isEMPTY(sm6))
		{
			RDAcurl=curl_easy_init();
			curl_easy_setopt(RDAcurl,CURLOPT_URL,sm1);
			if(!isEMPTY(sm2))
			{
				memset(stemp,0,101);
				sprintf(stemp,"%s:%s",sm2,sm3);
				curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
			}
			if(!isEMPTY(sm4))
			{
				memset(stemp,0,101);
				sprintf(stemp,"%s%s%s",sm4,(sm5!=NULL ? ":":""),(sm5!=NULL ? sm5:""));
				curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,(char *)stemp);
			}
			if(plst.filename!=NULL) Rfree(plst.filename);
			plst.filename=stralloc(sm1);
			plst.stream=NULL;
	
			curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE);
			curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,getftpfile);
			curl_easy_setopt(RDAcurl,CURLOPT_FILE,&plst);
			res=curl_easy_perform(RDAcurl);
			curl_easy_cleanup(RDAcurl);
			if(plst.stream) fclose(plst.stream);
			RDAcurl=NULL;
			if(CURLE_OK!=res)
			{
				prterr("Error:  Failed to FTPPUT file [%s] with error [%d].",sm6,res);
			}
			if(plst.filename!=NULL) Rfree(plst.filename);
		}
		if(sm1!=NULL) Rfree(sm1);
		if(sm2!=NULL) Rfree(sm2);
		if(sm3!=NULL) Rfree(sm3);
		if(sm4!=NULL) Rfree(sm4);
		if(sm5!=NULL) Rfree(sm5);
		if(sm6!=NULL) Rfree(sm6);
	} else if(!strncmp(&image->line[0],"WRITE_GENERIC_COUNTER",21) ||
		!strncmp(&image->line[0],"write_generic_counter",21))
	{
		temp=image->line+5;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		if(!isEMPTY(modx))
		{
			count=1;
			temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+15);
#ifndef WIN32
			sprintf(temp,"%s/%s",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
			sprintf(temp,"%s\\%s",CURRENTDIRECTORY,modx);
#endif
			if(image->pvals!=NULL)
			{
				pval=image->pvals;
				for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
				{
					if(!RDAstrcmp(pval->fieldname,"FILENAME"))
					{
						field=Rmalloc(sizeof(NRDfield));
						field->data.string_value=NULL;
						field->dlen=0;
						field->type=VARIABLETEXT;
						field->len=0;
						if(!pval->symbol_type)
						{
							SetFieldValue1(field,pval->name,rpt,rrpt);
						} else {
							field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
						}
						if(!isEMPTY(field->data.string_value))
						{
							filx=stralloc(field->data.string_value);
						} else filx=NULL;
						if(field!=NULL)
						{
							if(field->data.string_value!=NULL) Rfree(field->data.string_value);
							Rfree(field);
						}
					} else if(!RDAstrcmp(pval->fieldname,"GENERIC COUNTER"))
					{
						field=Rmalloc(sizeof(NRDfield));
						field->data.integer_value=Rmalloc(sizeof(int));
						*field->data.integer_value=1;
						field->dlen=sizeof(int);
						field->type=LONGV;
						field->len=8;
						if(!pval->symbol_type)
						{
							SetFieldValue1(field,pval->name,rpt,rrpt);
						} else {
							*field->data.integer_value=PP_EVALUATEint(pval->name,ReportGenSubData1,h);
						}
						count=*field->data.integer_value;
						if(field!=NULL)
						{
							if(field->data.integer_value!=NULL) Rfree(field->data.integer_value);
							Rfree(field);
						}
					}
				}
			}
			if(!isEMPTY(filx))
			{
				getnexttran(temp,filx,count);
			} else {
				prterr("Error: Filename not found for WRITE_GENERIC_COUNTER.");
			}
			if(filx!=NULL) Rfree(filx);
			if(temp!=NULL) Rfree(temp);
			
		}
		if(modx!=NULL) Rfree(modx);
	} else if(!strncmp(&image->line[0],"WRITE",5) || 
		!strncmp(&image->line[0],"write",5)) /* write to data file */
	{
		h=HoldReportNew(rrpt,rpt);
		temp=image->line+5;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		filx=stripfilename(temp);
		if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
		{
			for(x=0,fx=rrpt->ofiles;x<rrpt->numofiles;++x,++fx)
			{
				if(fx->fileno!=(-1) && !RDAstrcmp(fx->module,modx)
					&& !RDAstrcmp(fx->filename,filx)) break;
			}
			if(x<rrpt->numofiles)
			{
				ZERNRD(fx->fileno);
				if(image->pvals!=NULL)
				{
					for(y=0,pval=image->pvals;y<image->numpvals;
						++y,++pval)
					{
						fieldname=stripfieldname(pval->fieldname);
						field=FLDNRD(fx->fileno,fieldname);
						if(!pval->symbol_type)
						{
							if(!RDAstrcmp(pval->name,"NEXT TRANSACTION NO"))
							{
								trnfield=stralloc(fieldname);
							} else {
								if(field!=NULL)
								{
									SetFieldValue1(field,pval->name,rpt,rrpt);
								} else {
									prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
								}
							}
						} else {
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
										if(field->data.string_value!=NULL)
											Rfree(field->data.string_value);
										field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
										if(field->data.string_value!=NULL)
										{
											field->dlen=RDAstrlen(field->data.string_value)+1;
											if(RDAstrlen(field->data.string_value)>field->len && field->len>0)  field->data.string_value[field->len]=0;
										} else field->dlen=0;
										break;
									case BOOLNS:
										*field->data.string_value=PP_EVALUATEbol(pval->name,ReportGenSubData1,h);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=PP_EVALUATEsht(pval->name,ReportGenSubData1,h);
										break;
									case SLONGV:
									case SCROLLEDLIST:
									case LONGV:
										*field->data.integer_value=PP_EVALUATEint(pval->name,ReportGenSubData1,h);
										break;
									case SDOUBLEV:
									case DOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
										*field->data.float_value=PP_EVALUATEdbl(pval->name,ReportGenSubData1,h);
										if(field->type==DOLLARS || 
											field->type==DOLLARS_NOCENTS ||
											field->type==DOUBLEV ||
											field->type==SDOUBLEV)
										{
											*field->data.float_value=round(*field->data.float_value);
										}
										break;
									default:
										prterr("Error unrecognized field type [%d] for field [%s] in Module [%s] File [%d] [%s].",field->type,field->name,MODULENAME(fx->fileno),fx->fileno,FILENAME(fx->fileno));
										break;
								}
							} else {
								prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
							}
						}
						if(fieldname!=NULL) Rfree(fieldname);
						fieldname=NULL;
					}
					LOCNRDFILE(fx->fileno);
					if(trnfield!=NULL)
					{
						ADVWRTTRANSsec(fx->fileno,1,
							trnfield,NULL,ReportGenSubData1,h); 
						Rfree(trnfield);
						trnfield=NULL;
					} else if(!update)
					{
						ADVWRTTRANSsec(fx->fileno,0,NULL,NULL,
							ReportGenSubData1,h);
					} else if(ADVEQLNRDsec(fx->fileno,1,ReportGenSubData1,h))
					{
						ADVWRTTRANSsec(fx->fileno,0,NULL,NULL,
							ReportGenSubData1,h);
					} else {
						prev=RDATDataNEW(fx->fileno);
						for(y=0,pval=image->pvals;y<image->numpvals;++y,++pval)
						{
							fieldname=stripfieldname(pval->fieldname);
							field=FLDNRD(fx->fileno,fieldname);
							if(!pval->symbol_type)
							{
								if(field!=NULL)
								{
									SetFieldValue1(field,pval->name,rpt,rrpt);
								} else {
									prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
								}
							} else {
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
											if(field->data.string_value!=NULL)
												Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0)  field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(pval->name,ReportGenSubData1,h);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(pval->name,ReportGenSubData1,h);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(pval->name,ReportGenSubData1,h);
											break;
										case SDOUBLEV:
										case DOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(pval->name,ReportGenSubData1,h);
											if(field->type==DOLLARS || 
												field->type==DOLLARS_NOCENTS ||
												field->type==DOUBLEV ||
												field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s] in Module [%s] File [%d] [%s].",field->type,field->name,MODULENAME(fx->fileno),fx->fileno,FILENAME(fx->fileno));
											break;
									}
								} else {
									prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
								}
							}
							if(fieldname!=NULL) Rfree(fieldname);
							fieldname=NULL;
						}
						ADVWRTTRANSsec(fx->fileno,0,NULL,prev,
							ReportGenSubData1,h);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(fx->fileno);
				}
			} else {
				prterr("Error Module [%s] File [%s] not found in open file list.",(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
			}
		} else {
			prterr("Error Module [%s] File [%s] names in error cannot WRITE.",(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
		}
		if(modx!=NULL) Rfree(modx);
		if(filx!=NULL) Rfree(filx);
		if(h!=NULL) Rfree(h);
	} else {
		prterr("Error in syntax of Image Line Definition [%s].",image->line);
	}
}
