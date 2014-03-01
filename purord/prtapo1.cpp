		page_no); /* Handled internally by the program */
		(vendor_name!=NULL ? vendor_name:""),
		(attention_to!=NULL ? attention_to:""));
		(vendor_addr1!=NULL ? vendor_addr1:""),
		(vendor_addr2!=NULL ? vendor_addr2:""),
		(vendor_addr3!=NULL ? vendor_addr3:""),
		(shipping_addr1!=NULL ? shipping_addr1:""));
		(shipping_addr2!=NULL ? shipping_addr2:""));
		(shipping_addr3!=NULL ? shipping_addr3:""));
		(shipping_addr4!=NULL ? shipping_addr4:""));
		(purlin_entrydt!=NULL ? purlin_entrydt:""),
		(vendor_id!=NULL ? vendor_id:""),
		(po_id!=NULL ? po_id:""));
		(purmst_salesman!=NULL ? purmst_salesman:"")
		(pursit_phone!=NULL ? pursit_phone:""));
		(purmst_dlvrydt!=NULL ? purmst_dlvrydt:""),
		(purmst_shipvia!=NULL ? purmst_shipvia:""),
		(purmst_fob!=NULL ? purmst_fob:""),
		(purmst_terms!=NULL ? purmst_terms:""));
		(tot_purtrn_amt,); /* Handled internally by the program */
		enc_code,
		(acc_desc!=NULL ? acc_desc:"")
		(purlin_comments1))
		(purlin_comments2))
		(purlin_comments3))
		(purlin_comments4))
		(purlin_comments5))
		(purlin_comments6))
		(purlin_comments7))
		(purlin_comments8))
		(purmst_remarks1))
		(purmst_remarks2))
		(purmst_remarks3))
		(purmst_remarks4))
		(purmst_remarks5))
		(purmst_remarks6))
		(purmst_remarks7))
		(purmst_remarks8))
		(ship_amt==0)
		(purlin_itemid!=NULL ? purlin_itemid:""),
		(line_desc!=NULL ? line_desc:""),
		unit_amt,
		purlin_total);
	COPYFIELD(purlin,sfile,"PO IDENTIFICATION");
	COPYFIELD(purlin,sfile,"LINE NUMBER");
	COPYFIELD(purmst,sfile,"VENDOR IDENTIFICATION");
	COPYFIELD(venmsr,sfile,"VENDOR NAME");
	FIELDCOPY(purlin,"DESCRIPTION",sfile,"PURLIN DESCRIPTION");
	FIELDCOPY(purlin,"PRORATE AMOUNT",sfile,"PURLIN SHIPPING AMOUNT");
	FIELDCOPY(purlin,"ENTRY DATE",sfile,"PURLIN ENTRY DATE");
	FIELDCOPY(purlin,"TOTAL",sfile,"PURLIN AMOUNT");
	FIELDCOPY(purlin,"ITEM IDENTIFICATION",sfile,"PURLIN ITEM IDENTIFICATION");
	FIELDCOPY(purlin,"UNIT RATE",sfile,"PURLIN UNIT AMOUNT");
	FIELDCOPY(purlin,"QUANTITY",sfile,"PURLIN QUANTITY");
	FIELDCOPY(purmst,"DELIVERY DATE",sfile,"PURMST DELIVERY DATE");
	FIELDCOPY(purmst,"SHIP VIA",sfile,"PURMST SHIP VIA");
	FIELDCOPY(purmst,"FOB",sfile,"PURMST FOB");
	FIELDCOPY(purmst,"TERMS",sfile,"PURMST TERMS");
	FIELDCOPY(pursit,"PHONE",sfile,"PURSIT PHONE");
	FIELDCOPY(purmst,"SALESMAN",sfile,"PURMST SALESMAN");
	FIELDCOPY(purmst,"ATTENTION TO",sfile,"PURMST ATTENTION TO");
	FIELDCOPY(purmst,"REMARKS 1",sfile,"PURMST REMARKS 1");
	FIELDCOPY(purmst,"REMARKS 2",sfile,"PURMST REMARKS 2");
	FIELDCOPY(purmst,"REMARKS 3",sfile,"PURMST REMARKS 3");
	FIELDCOPY(purmst,"REMARKS 4",sfile,"PURMST REMARKS 4");
	FIELDCOPY(purmst,"REMARKS 5",sfile,"PURMST REMARKS 5");
	FIELDCOPY(purmst,"REMARKS 6",sfile,"PURMST REMARKS 6");
	FIELDCOPY(purmst,"REMARKS 7",sfile,"PURMST REMARKS 7");
	FIELDCOPY(purmst,"REMARKS 8",sfile,"PURMST REMARKS 8");
	FIELDCOPY(purlin,"COMMENTS 1",sfile,"PURLIN COMMENTS 1");
	FIELDCOPY(purlin,"COMMENTS 2",sfile,"PURLIN COMMENTS 2");
	FIELDCOPY(purlin,"COMMENTS 3",sfile,"PURLIN COMMENTS 3");
	FIELDCOPY(purlin,"COMMENTS 4",sfile,"PURLIN COMMENTS 4");
	FIELDCOPY(purlin,"COMMENTS 5",sfile,"PURLIN COMMENTS 5");
	FIELDCOPY(purlin,"COMMENTS 6",sfile,"PURLIN COMMENTS 6");
	FIELDCOPY(purlin,"COMMENTS 7",sfile,"PURLIN COMMENTS 7");
	FIELDCOPY(purlin,"COMMENTS 8",sfile,"PURLIN COMMENTS 8");
	FIELDCOPY(venadr,"PHONE",sfile,"VENADR PHONE");
	FIELDCOPY(venadr,"FAX",sfile,"VENADR FAX");
	FIELDCOPY(pursit,"ADDRESS 1",sfile,"SHIPPING ADDRESS 1");
	FIELDCOPY(pursit,"ADDRESS 2",sfile,"SHIPPING ADDRESS 2");
	FIELDCOPY(pursit,"ADDRESS 3",sfile,"SHIPPING ADDRESS 3");
	FINDFLDSETSTRING(sfile,"SHIPPING ADDRESS 4",stemp);
	FIELDCOPY(puracc,"DEFAULT DEBIT CODE",sfile,"PURACC ENCUMBRANCE CODE");
	FIELDCOPY(finbacc,"DESCRIPTION",sfile,"ACCOUNT CODE DESCRIPTION");
	FINDFLDSETSTRING(sfile,"VENDOR ADDRESS 1",VendorAddr1->value.string_value);
	FINDFLDSETSTRING(sfile,"VENDOR ADDRESS 2",VendorAddr2->value.string_value);
	FINDFLDSETSTRING(sfile,"VENDOR ADDRESS 3",VendorAddr3->value.string_value);
