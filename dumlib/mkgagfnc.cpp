/* mkgaucoa.c - functions for GA Universal Chart of Accounts */
/*lint -library */
#include <mkgag.hpp>

char xisGAUCOA(int accttype,RDAacct *acc,int line,char *file)
{
	RDAaccpart *part=NULL;
	short x=0;

	switch(accttype)
	{
		case EXP_ACCT:
			if(acc->acct!=NULL)
			{
				if(acc->num!=4) return(-1);
				for(x=0,part=acc->acct;x<acc->num;++x,++part)
				{
					switch(x)
					{
						case 0:
							if(RDAstrcmp(part->name,"FD")) return(-1);
							if(part->length!=3) return(-1);
							if(RDAstrcmp(part->definition,"NNN")) return(-1);
							break;
						case 1:
							if(RDAstrcmp(part->name,"FACT")) return(0);
							if(part->length!=4) return(0);
							if(RDAstrcmp(part->definition,"NNNN")) return(0);
							break;
						case 2:
							if(RDAstrcmp(part->name,"DEPT")) return(0);
							if(part->length!=2) return(0);
							if(RDAstrcmp(part->definition,"NN")) return(0);
							break;
						case 3:
							if(RDAstrcmp(part->name,"CLSOBJ")) return(0);
							if(part->length!=6) return(0);
							if(RDAstrcmp(part->definition,"NNNNNN")) return(0);
							break;
						default:
							return(0);
					}
				}
			}
			break;
		case REV_ACCT: 
			if(acc->acct!=NULL)
			{
				if(acc->num!=2) return(0);
				for(x=0,part=acc->acct;x<acc->num;++x,++part)
				{
					switch(x)
					{
						case 0:
							if(RDAstrcmp(part->name,"FD")) return(0);
							if(part->length!=3) return(0);
							if(RDAstrcmp(part->definition,"NNN")) return(0);
							break;
						case 1:
							if(RDAstrcmp(part->name,"CLSSRC")) return(0);
							if(part->length!=6) return(0);
							if(RDAstrcmp(part->definition,"NNNNNN")) return(0);
							break;
						default:
							return(0);
					}
				}
			}
			break;
		case BAL_ACCT:
			if(acc->acct!=NULL)
			{
				if(acc->num!=2) return(0);
				for(x=0,part=acc->acct;x<acc->num;++x,++part)
				{
					switch(x)
					{
						case 0:
							if(RDAstrcmp(part->name,"FD")) return(0);
							if(part->length!=3) return(0);
							if(RDAstrcmp(part->definition,"NNN")) return(0);
							break;
						case 1:
							if(RDAstrcmp(part->name,"CLSACC")) return(0);
							if(part->length!=6) return(0);
							if(RDAstrcmp(part->definition,"NNNNNN")) return(0);
							break;
						default:
							return(0);
					}
				}
			}
			break;
		case BEG_ACCT:
		default:
			break;
	}
	return(1);
}
/*
void xCreateGAUCOAs(char *modx,int line,char *file)
{
	short x,y,z;
	short if_count=0
	

	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			no_parts=0;
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
			}
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						if(isGAUCOA(BAL_ACCT,acc)
						if(acc->num>x)
						{
							part=acc->acct+x;
							memset(temp,0,15001);
							acc_count=1;
							for(z=0,p=acc->acct;z<x;++z,++p) 
							{
								if(RDAstrlen(p->name)>p->length)
								{
									acc_count+=RDAstrlen(p->name)+1;
								} else {
									acc_count+=p->length+1;
								}
							}
							sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count,part->length);
							if_count+=1;
							if(tmpexp!=NULL)
							{
								tmpexp=Rrealloc(tmpexp,
									exp_count+RDAstrlen(temp)+6);
								sprintf(&tmpexp[exp_count],"ELSE %s",temp);
								exp_count+=(RDAstrlen(temp)+5);
							} else {
								tmpexp=stralloc(temp);
								exp_count+=RDAstrlen(temp);
							}
							memset(temp,0,15001);
							sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
							if(tmpdesc!=NULL)
							{
								tmpdesc=Rrealloc(tmpdesc,
									desc_count+RDAstrlen(temp)+6);
								sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
								desc_count+=(RDAstrlen(temp)+5);
							} else {
								tmpdesc=stralloc(temp);
								desc_count+=RDAstrlen(temp);
							}	
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(50);
				sprintf(vname,"GAUCOA BALANCE SHEET CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINBACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				d->avail2rsrc=TRUE;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Balance Sheet Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);

				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(50);
				sprintf(vname,"GAUCOA BALANCE SHEET DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINBACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Balance Sheet Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
}
*/
