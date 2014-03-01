#include <nrdp.hpp>

#ifdef USE_CTREE
void seeifil(IFIL *filedef)
{
	int x,y;
	IIDX *index;
	ISEG *element;

	prterr("IFIL STRUCTURE in file %s at line# %d\n",__FILE__,
		__LINE__);
	prterr("pfilnam |%s|\n",filedef->pfilnam);
	prterr("dfilno %d\n",filedef->dfilno);
	prterr("dreclen %d\n",filedef->dreclen);
	prterr("dxtdsiz %d\n",filedef->dxtdsiz);
	prterr("dfilmod %d\n",filedef->dfilmod);
	prterr("dnumidx %d\n",filedef->dnumidx);
	prterr("ixtdsiz %d\n",filedef->ixtdsiz);
	prterr("rfstfld |%s|\n",filedef->rfstfld);
	prterr("rlstfld |%s|\n",filedef->rlstfld);
	for(x=0,index=filedef->ix+x;x<filedef->dnumidx;++x,++index)
	{
		prterr("IIDX STRUCTURE for Index #%d\n",x+1);
		prterr("ikeylen %d\n",index->ikeylen);
		prterr("ikeytyp %d\n",index->ikeytyp);
		prterr("ikeydup %d\n",index->ikeydup);
		prterr("inulkey %d\n",index->inulkey);
		prterr("iempchr %d\n",index->iempchr);
		prterr("inumseg %d\n",index->inumseg);
		prterr("ridxnam |%s|\n",index->ridxnam);
/*
		prterr("aidxnam |%s|\n",index->aidxnam);
*/
		for(y=0,element=index->seg+y;y<index->inumseg;++y,++element)
		{
			switch(element->segmode)
			{
				case VARSEG:
				case UVARSEG:
					prterr("Relative field# %d\n",element->soffset);
					break;
				case REGSEG:
				case INTSEG:
				case UREGSEG:
				case SRLSEG:
				case SGNSEG:
				case FLTSEG:
				case DECSEG:
				case BCDSEG:
					prterr("Abs. byte offset %d\n",element->soffset);
					break;
				case SCHSEG:
				case USCHSEG:
				case VSCHSEG:
				case UVSCHSEG:
					prterr("Schema field number %d\n",
						element->soffset);
					break;
				default:
					prterr("Undefined Segmode Value %d\n",
						element->soffset);
					break;
			}
			prterr("slength %d\n",element->slength);
			prterr("segmode %d\n",element->segmode);
		}
	}
	prterr("\n");
}
#endif
