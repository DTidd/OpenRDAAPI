#include<misc.hpp>

double round(double amt)
{
	if(amt>=0.0) return(Rint(amt+0.5)); else return(Rint(amt-0.5));
}

