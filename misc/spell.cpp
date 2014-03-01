#include<misc.hpp>

/* spell(amount,word) -	creates a string(word) of letters equivalent to 
						an amount (i.e. $20.00 = twenty and 0/100 */

static char ones_digit[10][7]={"","One ","Two ","Three ","Four ","Five ","Six ","Seven ","Eight ","Nine "};
static char teens_digit[10][10]={"Ten","Eleven","Twelve","Thirteen","Fourteen","Fifteen","Sixteen","Seventeen","Eighteen","Nineteen"};
static char tens_digit[8][8]={"Twenty","Thirty","Forty","Fifty","Sixty","Seventy","Eighty","Ninety"};
static char after_digit[3][10]={"","Thousand ","Million "};
static double d[3]={100.0,100000.0,100000000.0};

char *spell(double net)
{
	int x;
	int temp[4],spoint=0;
	double cents,prevtemp=0,dollar;
	char *word=NULL;
	
	word=(char *)Rmalloc(512);
	word[0]='\0';
	for (x=3;x>0;--x)
	{
		dollar=Rint((net-prevtemp)/d[x-1]);
		if(dollar>=1)
		{
			prevtemp+= dollar*d[x-1];
			temp[3]  = (int)Rint(dollar/100.0);
			temp[2]  = (int)Rint((dollar-(temp[3]*100.0))/10.0);
			temp[1]  = (int)Rint((dollar-(temp[3]*100.0)-(temp[2]*10.0)));
			if (temp[3])
			{
				sprintf(&word[spoint],"%sHundred ",ones_digit[temp[3]]);
				spoint += 8+(int)RDAstrlen(ones_digit[temp[3]]);
			}
			if (temp[2]*10+temp[1]>19)
			{
				sprintf(&word[spoint],"%s%s%s",tens_digit[temp[2]-2],
					(temp[1]?"-":" "),ones_digit[temp[1]]);
				spoint +=1+(int)RDAstrlen(tens_digit[temp[2]-2]);
				spoint +=(int)RDAstrlen(ones_digit[temp[1]]);
			} else if (temp[2]*10+temp[1]>9 && temp[2]*10+temp[1]<20)
			{
				sprintf(&word[spoint],"%s ",teens_digit[temp[1]]);
				spoint += 1+(int)RDAstrlen(teens_digit[temp[1]]);
			} else {
				sprintf(&word[spoint],"%s",ones_digit[temp[1]]);
				spoint += (int)RDAstrlen(ones_digit[temp[1]]);
			}
			sprintf(&word[spoint],"%s",after_digit[x-1]);
			spoint += (int)RDAstrlen(after_digit[x-1]);
		}
	}
	if (spoint)
	{
		sprintf(&word[spoint],"And ");
		spoint += 4;
	}
	cents=Rint(net-prevtemp);
	sprintf(&word[spoint],"%02.0f/100 Dollars",cents);
	return(word);
}

