#include<misc.hpp>

double RDAroundup(double amt)
{
	double ramt=0;

	if(amt>0) 
	{
		amt=amt+0.9999999999999;
		ramt=Rint(amt);
		return(ramt); 
	} else {
		amt=amt-0.9999999999999;
		ramt=Rint(amt);
		return(ramt);
	}
}

