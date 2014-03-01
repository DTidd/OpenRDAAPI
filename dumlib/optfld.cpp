/* optfld.c - Optional Field Conversion for MBGUI */
#include<mbplus.hpp>

/* NOTES -----------------------------------------------------------------

-In all of microBUDGET it appears as if the optional fields come from a
 file with a single short integer as it's key or from set variables which
 are basically fixed for all records within the database.
------------------------------------------------------------------------*/

MBOFC *xMBOFCNEW(DBsort *sort,short type,int line,char *file)
{
	MBOFC *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG MBOFCNEW at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(MBOFC));
	tmp->sort=sort;
	tmp->type=type;
	tmp->num=0;
	tmp->t=NULL;
	return(tmp);
}
void xFreeMBOFC(MBOFC *m,int line,char *file)
{
	OFCtype *t;
	int x,y;

	if(m!=NULL)
	{
		if(m->conname!=NULL) Rfree(m->conname);
		if(m->fldname!=NULL) Rfree(m->fldname);
		if(m->t!=NULL)
		{
			for(x=0,t=m->t;x<m->num;++x,++t)
			{
				if(t->title!=NULL)
				{
					for(y=0;y<m->count;++y) 
					{
						if(t->title[y]!=NULL) Rfree(t->title[y]);
					}
					Rfree(t->title);
					t->title=NULL;
				}
				if(t->fldname!=NULL)
				{
					for(y=0;y<m->count;++y) 
					{
						if(t->fldname[y]!=NULL) Rfree(t->fldname[y]);
					}
					Rfree(t->fldname);
					t->fldname=NULL;
				}
			}
		}
		Rfree(m);
	}
}
MBOFC *xMBOFC_MANUAL(DBsort *sort,short num,char title[][11],
	char *type,char *len,int line,char *file)
{
	MBOFC *tmp=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG MBOFC_MANUAL at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	
	tmp=MBOFCNEW(sort,0);
	tmp->sort=sort;
	tmp->num=1;
	tmp->t=Rmalloc(sizeof(OFCtype));
	tmp->count=num;
	if(title!=NULL && num>0)
	{
		tmp->t->title=Rmalloc(num*sizeof(char *));
		for(x=0;x<num;++x)
		{
			tmp->t->title[x]=stralloc(title[x]);
		}
	} else {
		tmp->t->title=NULL;
		tmp->count=0;
	}
	if(num>0)
	{
		for(x=0;x<num;++x)
		{
			tmp->t->type[x]=type[x];
			tmp->t->len[x]=len[x];
		}
	}
	SetFieldNames(tmp);
	return(tmp);
	
}
void xGatherCombos(MBOFC *m,int line,char *file)
{
	short ef=FALSE,x,len=0;
	OFCtype *t=NULL;
	char *s=NULL;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG GatherCombos at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(m!=NULL)
	{
		if(m->confileno!=(-1) && m->count>0)
		{
			ZERNRD(m->confileno);
			ef=BEGNRD(m->confileno);
			while(!ef)
			{
				if(m->t!=NULL)
				{
					m->t=Rrealloc(m->t,(m->num+1)*sizeof(OFCtype));
				} else {
					m->t=Rmalloc(sizeof(OFCtype));
					m->num=0;
				}
				t=m->t+m->num;
				FINDFLDGETSHORT(m->confileno,m->conname,&t->code);
				t->title=Rmalloc(m->count*sizeof(char *));
				for(x=0;x<m->count;++x)
				{
					memset(stemp,0,101);
					sprintf(stemp,"OPTIONAL FIELD TITLE %d",x+1);
					FINDFLDGETSTRING(m->confileno,stemp,&s);
					t->title[x]=stralloc(s);
				}
				if(s!=NULL) Rfree(s);
				t->fldname=NULL;
				for(x=0;x<m->count;++x)
				{
					memset(stemp,0,101);
					sprintf(stemp,"OPTIONAL FIELD TYPE %d",x+1);
					FINDFLDGETSTRING(m->confileno,stemp,&temp);
					t->type[x]=temp[0];
					memset(stemp,0,101);
					sprintf(stemp,"OPTIONAL FIELD LENGTH %d",x+1);
					FINDFLDGETSHORT(m->confileno,stemp,&len);
					t->len[x]=len;	
				}
				if(temp!=NULL) Rfree(temp);
				++m->num;
				ef=SEQNRD(m->confileno);
			}
		}
	}
}
MBOFC *xMBOFC_DYNAMIC(DBsort *sort,short confileno,short cunt,char *confld,
	char *fldname,int line,char *file)
{
	MBOFC *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG MBOFC_DYNAMIC at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=MBOFCNEW(sort,1);
	tmp->confileno=confileno;
	tmp->conname=stralloc(confld);
	tmp->fldname=stralloc(fldname);
	tmp->count=cunt;
	tmp->num=0;
	tmp->t=NULL;
	GatherCombos(tmp);
	SetFieldNames(tmp);
	return(tmp);
}
void xSetFieldNames(MBOFC *m,int line,char *file)
{
	NRDfield *field=NULL;
	short x=0,y;
	char *s=NULL,*s1=NULL;
	OFCtype *t=NULL;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("MakeNewFields at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(m!=NULL)
	{
		if(m->sort==NULL) 
		{
			prterr("Error SetFieldNames without a DBsort at line [%d] program [%s].!!!!!");
			return;
		}
		if(m->type==0)
		{
			for(y=0,t=m->t;y<m->num;++y,++t)
			{
			t->fldname=Rmalloc(m->count*sizeof(char *));
			if(s!=NULL) Rfree(s);
			for(x=0;x<m->count;++x)
			{
				if(!isEMPTY(t->title[x]))
				{
				s1=t->title[x];
				if(!isEMPTY(s1))
				{
					s=Rmalloc(RDAstrlen(m->sort->module)+RDAstrlen(m->sort->filename)+RDAstrlen(s1)+7);
					sprintf(s,"[%s][%s][%s]",m->sort->module,m->sort->filename,(s1!=NULL ? s1:""));
				} else {
					s=NULL;
				}
				if(!isEMPTY(s))
				{
					field=DBsortFIELD(m->sort,s1);
				} else field=NULL;
				if(field!=NULL)
				{
					t->fldtype[x]=field->type;
				} else t->fldtype[x]=PLAINTEXT;
				t->fldname[x]=stralloc(s);
				if(s!=NULL) Rfree(s);
				}
			}
			}
		} else {
			for(y=0,t=m->t;y<m->num;++y,++t)
			{
			t->fldname=Rmalloc(m->count*sizeof(char *));
			for(x=0;x<m->count;++x)
			{
				if(!isEMPTY(t->title[x]))
				{
				s1=t->title[x];
				if(!isEMPTY(s1))
				{
					s=Rmalloc(RDAstrlen(m->sort->module)+RDAstrlen(m->sort->filename)+RDAstrlen(s1)+7);
					sprintf(s,"[%s][%s][%s]",m->sort->module,m->sort->filename,(s1!=NULL ? s1:""));
				} else {
					s=NULL;
				}
				if(!isEMPTY(s))
				{
					field=DBsortFIELD(m->sort,s1);
				} else field=NULL;
				if(field!=NULL)
				{
					t->fldtype[x]=field->type;
				} else t->fldtype[x]=PLAINTEXT;
				t->fldname[x]=stralloc(s);
				if(s!=NULL) Rfree(s);
				}
			}
			}
		}
	}
}
void xMBOFC_addDBfield(MBOFC *tmp,int line,char *file)
{
	OFCtype *t=NULL;
	int x=0,y=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG MBOFC_addDBfield at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(tmp!=NULL)
	{
		if(tmp->t!=NULL && tmp->sort!=NULL)
		{
			for(x=0,t=tmp->t;x<tmp->num;++x,++t)
			{
				for(y=0;y<tmp->count;++y)
				{
					if(!isEMPTY(t->title[y]))
					{
						memset(stemp,0,101);
						sprintf(stemp,"[%s][%s][%s]",tmp->sort->module,tmp->sort->filename,t->title[y]);
						if(!RDAstrcmp(stemp,t->fldname[y]))
						{
							addDBfieldNoDuplicates(tmp->sort,t->title[y],t->fldtype[y],t->len[y]);
						}
					}
				}
			}
		}
	} else {
		prterr("Error in MBOFC_addDBfield one structure or the other was NULL!");
	}
}
