#include <nrdp.hpp>
#ifdef USE_CTREE

#ifdef UNIXWARE7
#include <errno.h>
#endif
#ifdef UNIXWARE21
#include <errno.h>
#endif

char *ctdb_errors(short errorno)
{
	char *temp=NULL;
	char *temp1=NULL;

	if(errno!=0) 
	{
		temp1=stralloc(errname());
		temp=Rmalloc(RDAstrlen(temp1)+10+20+1);
		sprintf(temp,"System Error [%s] [%d].",temp1,errno);
		if(temp1!=NULL) Rfree(temp1);
	} else {
		temp=Rmalloc(56);
		sprintf(temp,"Ctree Isam Error: Errors Exist Though Not Identifiable.");
	}
	return(temp);
}
#ifdef XXX
#define IDRK_ERR	107	/* too many keys for data file in ISAM */
				/* parameter file */
#define IMKY_ERR	108	/* incorrect keyno for index member in */
				/* parameter file */
#define IKRS_ERR	109	/* too many key segments defined in ISAM */
				/* parameter file */
#define ISRC_ERR	110	/* could not read segment record in ISAM */
				/* parameter file */
#define	IKRI_ERR	111	/* could not read index file record in ISAM */
				/* parameter file */
#define IPND_ERR	112	/* LKISAM(ENABLE) found pending locks	   */
#define INOL_ERR	113	/* no memory for user lock table */
#define IRED_ERR	114	/* 1st byte of data record equals delete flag */
				/* or bad variable length record mark	      */
#define ISLN_ERR	115	/* key segments do not match key length */
#define IMOD_ERR	116	/* bad mode parameter			   */
#define	IMRI_ERR	117	/* could not read index member record	   */
#define SKEY_ERR	118	/* NXTSET called before FRSSET for keyno   */
#define SKTY_ERR	119	/* FRSSET called for index with wrong keytyp */

#define RRLN_ERR	120	/* data record length exceeds rebuild max  */
#define RSPC_ERR	121	/* not enough space for sort area.	   */
#define RMOD_ERR	122	/* attempt to change fixed vs variable len */
#define	RVHD_ERR	123	/* var length header has bad record mark   */
#define INIX_ERR	124	/* # of indices does not match (OPNIFIL)   */
#define IINT_ERR	125	/* c-tree already initialized		   */

#define ABDR_ERR	126	/* bad directory path get		    */
#define ARQS_ERR	127	/* could not send request		    */
#define ARSP_ERR	128	/* could not receive answer		    */
#define NINT_ERR	129	/* c-tree not initialized		    */
#define AFNM_ERR	130	/* null file name pointer in OPNFIL	    */
#define AFLN_ERR	131	/* file name length exceeds msg size	    */
#define ASPC_ERR	132	/* no room for application message buffer   */
#define ASKY_ERR	133	/* server is not active			    */
#define ASID_ERR	134	/* could not get servers message id	    */
#define AAID_ERR	135	/* could not allocate application id	    */
#define AMST_ERR	136	/* could not get application msg status	    */
#define AMQZ_ERR	137	/* could not set message appl msg size	    */
#define AMRD_ERR	138	/* could not get rid of application msg	    */ 
#define ABNM_ERR	139	/* badly formed file name		    */
#define VMAX_ERR	140	/* variable record length too long */
#define AMSG_ERR	141	/* required message size exceeds maximum    */

#define SMXL_ERR	142	/* application MAXLEN > server's MAXLEN     */
#define SHND_ERR	143	/* communications handler not installed	    */
#define QMEM_ERR	144	/* application could not id output queue    */
#define SCSF_ERR	145	/* could not find COMM software */
#define	VDLK_ERR	146	/* could not update free space info */
#define VDLFLG_ERR	147	/* space to be reused is not marked deleted */
#define	VLEN_ERR	148	/* WRTVREC cannot fit record at recbyt */
#define	VRLN_ERR	149	/* varlen less than minimum in ADDVREC */
#define SHUT_ERR	150	/* server is shutting down		    */
#define STRN_ERR	151	/* could not shut down. transactions pending*/
#define LEXT_ERR	152	/* could not extend logfile */
#define	VBSZ_ERR	153	/* buffer too small */
#define	VRCL_ERR	154	/* zero length record in REDVREC */
#define SYST_ERR	155	/* native system failure */
#define	VFLG_ERR	158	/* REDVREC record not marked active */
#define	VPNT_ERR	159	/* zero recbyt value */
#define ITIM_ERR	160	/* multi-user interefernce: index informa-  */
				/* tion updated by the time user got to	*/
				/* actual data record. */
#define SINA_ERR	161	/* user appears inactive		    */
#define SGON_ERR	162	/* server has gone away			    */
#define SFRE_ERR	163	/* no more room in server lock table	    */
#define SFIL_ERR	164	/* file number out of range		    */
#define SNFB_ERR	165	/* no file control block available	    */
#define SNMC_ERR	166	/* no more ct file control blocks in server */
#define SRQS_ERR	167	/* could not read request		    */
#define SRSP_ERR	168	/* could not send answer		    */

#define SFUN_ERR	170	/* bad function number */
#define SMSG_ERR	171	/* application msg size exceeds server size */
#define SSPC_ERR	172	/* could not allocate server msg buffer	    */
#define SSKY_ERR	173	/* could not identify server		    */
#define SSID_ERR	174	/* could not get server message id	    */
#define SAMS_ERR	175	/* server could not allocate user msg area  */	
#define SMST_ERR	176	/* could not get server msg status	    */
#define SMQZ_ERR	177	/* could not set message server msg size    */
#define SINM_ERR	178	/* unexpected file# assigned to [si] in rcv */ 
#define SOUT_ERR	179	/* server is at full user capacity	    */

#define IKRU_ERR	180	/* could not read symbolic key name	  */
#define IKMU_ERR	181	/* could not get mem for key symb name	  */
#define IKSR_ERR	182	/* no room for sort key. increase MAXFIL  */
#define IDRU_ERR	183	/* could not read file field number values*/
#define ISDP_ERR	184	/* attempt to reallocate set space	  */
#define ISAL_ERR	185	/* not enough memory for multiple sets	  */
#define ISNM_ERR	186	/* set number out of range		  */
#define IRBF_ERR	187	/* null buffer in rtread.c		  */
#define ITBF_ERR	188	/* null target buffer in rtread.c	  */
#define IJSK_ERR	189	/* join_to skip				  */
#define IJER_ERR	190	/* join_to error			  */
#define IJNL_ERR	191	/* join_to null fill			  */
#define IDSK_ERR	192	/* detail_for skip			  */
#define IDER_ERR	193	/* detail_for error			  */
#define IDNL_ERR	194	/* detail_for null fill			  */
#define IDMU_ERR	195	/* could not get mem for dat symb name	  */
#define ITML_ERR	196	/* exceeded RETRY_LIMIT in RTREAD.C	  */

#define IMEM_ERR	197	/* could net get memory for ifil block	  */
#define BIFL_ERR	198	/* improper ifil block			  */
#define NSCH_ERR	199	/* schema not defined for data file	  */

#define RCRE_ERR	400	/* resource already enabled		    */
#define RNON_ERR	401	/* resources not enabled		    */
#define RXCL_ERR	402	/* file must be exclusive to enable res	    */
#define RZRO_ERR	403	/* empty resource id			    */
#define RBUF_ERR	404	/* output buffer to small		    */
#define RDUP_ERR	405	/* resource id already added		    */
#define RCSE_ERR	406	/* bad resource search mode		    */
#define RRED_ERR	407	/* attempt to get non-resource info	    */
#define RNOT_ERR	408	/* resource not found			    */
#define LKEP_ERR	409	/* not in use: available		    */
#define USTP_ERR	410	/* user not active			    */
#define BTFN_ERR	422	/* other functions not sup		    */
#define BTIC_ERR	423	/* incomplete				    */
#define BTAD_ERR	424	/* add list err				    */
#define BTIP_ERR	425	/* batch in progress			    */
#define BTNO_ERR	426	/* no batch active			    */
#define BTST_ERR	427	/* status info already returned		    */
#define BTMT_ERR	428	/* no more info, batch cancelled	    */
#define BTBZ_ERR	429	/* bufsiz too small for record		    */
#define BTRQ_ERR	430	/* request is empty or inconsistent	    */
#define LAGR_ERR	431	/* aggregate/serialization lock denied	    */
#define FLEN_ERR	432	/* fixed length string requires len in DODA */
#define SSCH_ERR	433	/* segment def inconsistent with schema	    */
#define DLEN_ERR	434	/* very long def block not supported	    */
#define FMEM_ERR	435	/* file def memory error		    */
#define DNUM_ERR	436	/* bad def number			    */
#define DADR_ERR	437	/* defptr NULL during GETDEFBLK		    */
#define	DZRO_ERR	438	/* requested def blk is empty		    */
#define DCNV_ERR	439	/* no conversion routine for Def'n Block    */
#define DDDM_ERR	440	/* dynamic dump already in progress	    */
#define DMEM_ERR	441	/* no memory for dynamic dump file buffer   */
#define DAVL_ERR	442	/* one or more files not available for dump */
#define DSIZ_ERR	443	/* file length discrepancy		    */
#define DCRE_ERR	444	/* could not create file during dump rcv    */
#define SDAT_ERR	445	/* not enough data to assemble key value    */
#define BMOD_ERR	446	/* bad key segment mode			    */
#define BOWN_ERR	447	/* only the file's owner can perform op	    */
#define DEFP_ERR	448	/* permission to set file definition denied */
#define DADM_ERR	449	/* ADMIN has opened file. Cannot delete file*/
#define SRES_ERR	460	/* resource not enabled			    */
#define SPER_ERR	461	/* bad permission flag			    */
#define UQID_ERR	463	/* file id uniqueness error		    */
#define IISM_ERR	464	/* ISAM level logon not performed	    */

#define IINI_ERR	465	/* Incremental Index: dnumidx < 1	    */
#define IIDT_ERR	466	/* Incremental Index: dfilno not a ISAM file*/
#define IINM_ERR	467	/* Incremental Index: aidxnam NULL for 1st  */
#define IITR_ERR	468	/* Incremental Index: active tran not allowed*/
#define NGIO_ERR	469	/* negative I/O request */

#define SORT_ERR	370	/* sort base: errors SORT_ERR + 101 thru 126*/
				/* see CTSORT.C for error listing	    */
#define RFCK_ERR	510 	/* active chkpnt at start of roll-forward */
#define ILOP_ERR	511 	/* index nodes form illegal loop: rebuild */
#define DLOP_ERR	512 	/* data file loop detected */
#define SBLF_ERR	513 	/* FPUTFGET does not support CTSBLDX () */

#endif /* ifdef XXX */
#endif /* ifdef USE_CTREE */
