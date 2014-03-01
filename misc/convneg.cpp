#include<misc.hpp>
/* convneg.c -convertes a string to a double by calling atof function and 
	sets the sign */ 
double convneg(char *s)
{
	short neg_flag=FALSE;
	double ret_double=0.0;

	if(isEMPTY(s)) return(ret_double);
	if((RDAstrstr(s,"-"))!=NULL)
	{
		neg_flag=TRUE;
	}
	if(!isEMPTY(s))
	{
	ret_double=atof(s);
	} else ret_double=0;
	if(neg_flag==1) ret_double *=(-1);
	return ret_double;
}
int convintneg(char *s)
{
	short neg_flag=FALSE;
	int ret_int=0;

	if(isEMPTY(s)) return(ret_int);
	if((RDAstrstr(s,"-"))!=NULL)
	{
		neg_flag=TRUE;
	}
	if(!isEMPTY(s))
	{
	ret_int=Ratoi(s);
	} else ret_int=0;
	if(neg_flag==TRUE) ret_int *=(-1);
	return(ret_int);
}
short convshortneg(char *s)
{
	short neg_flag=FALSE;
	short ret_short=0;

	if(isEMPTY(s)) return(ret_short);
	if((RDAstrstr(s,"-"))!=NULL)
	{
		neg_flag=TRUE;
	}
	if(!isEMPTY(s))
	{
	ret_short=Ratoi(s);
	} else ret_short=0;
	if(neg_flag==TRUE) ret_short *=(-1);
	return(ret_short);
}
