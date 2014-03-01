#include <stdio.h>
#include <roxml.h>

#define BUF_SIZE 1024

int ReadXml(node_t *node) 
{
	const int buf_size = BUF_SIZE;
	char buf[BUF_SIZE+1];
	int read_size;
	node_t *tmp;

	node_t *root;
	node_t *db;
	node_t *table;
	char *root_val=NULL;
	char *db_val=NULL;
	char *table_val=NULL;


	root_val = roxml_get_name(node, NULL, 0);

	// Should check to make sure there is a child before getting the child.  use _nb function
	db = roxml_get_chld(node, NULL, 0);
	db_val = roxml_get_name(db, NULL, 0);

	// If previous check for a child fails, skip this if and go straight to cleanup and error.
	if (strcmp(root_val,"mysqldump") == 0) 
	{
	
		if (strcmp(db_val,"database") == 0) 
		{
			tmp = roxml_get_nodes( db, ROXML_ALL_NODE, "name", 0);
			printf("MySQL Dump for DB [%s]\n",roxml_get_content(tmp, buf, buf_size, &read_size));
		}else{
			printf("Not mysqldump database xml\n");
			return(1);
		}
	}else{
		printf("Not mysqldump xml\n");
		return(1);
	}
	return(0);
	//printf("Root [%s]  DB [%s]\n",root_val,db_val);
	
}
void get_item_info(node_t *node) 
{
	const int buf_size = BUF_SIZE;
	char buf[BUF_SIZE+1];
	int read_size;
	node_t *tmp;

	int z=0;
	int t=0;

	if( node == NULL )
		return;

	t=roxml_get_nodes_nb(node, ROXML_ALL_NODE);	
	if(t>0)
	{
		printf("t=[%d]\n",t);

		for (z=0; z<t; z++) { 
			read_size = 0;
			tmp = roxml_get_nodes( node, ROXML_ALL_NODE, NULL, z);
			printf( "key: %s\tvalue: %s\n", roxml_get_name(tmp, NULL, 0), roxml_get_content(tmp, buf, buf_size, &read_size) );

		}
	}else{
		printf("t=is empty\n");
	}


	if( (tmp = roxml_get_nodes( node, ROXML_ELM_NODE, "name" , 0) ) != NULL ) {
		read_size = 0;
		printf( "key: %s\tvalue: %s\n", roxml_get_name(tmp, NULL, 0), roxml_get_content(tmp, buf, buf_size, &read_size) );
	}

	if( (tmp = roxml_get_nodes( node, ROXML_ELM_NODE, "Field" , 0) ) != NULL ) {
		read_size = 0;
		printf( "key: %s\tvalue: %s\n", roxml_get_name(tmp, NULL, 0), roxml_get_content(tmp, buf, buf_size, &read_size) );
	}

	if( (tmp = roxml_get_nodes( node, ROXML_ELM_NODE, "Table" , 0) ) != NULL ) {
		read_size = 0;
		printf( "key: %s\tvalue: %s\n", roxml_get_name(tmp, NULL, 0), roxml_get_content(tmp, buf, buf_size, &read_size) );
	}
}

int xmlread(const char *filename) {
	int m,n,o;
	int i,j,k;
	int len;
	char* sTag1;
	char* sTag2;
	node_t* nRoot;
	node_t* nUser;
	node_t* nItem;
	node_t* nName;
	node_t* nTime;

	char *db_tag=NULL;
	char *db_val=NULL;
	char *tb_tag=NULL;
	char *tb_val=NULL;
	node_t *text=NULL;

	//time_unit tu;
	//user* userptr=NULL;
	//night* nightptr=NULL;

	nRoot = roxml_load_doc( filename );
	ReadXml(nRoot);

	sTag1 = roxml_get_name(nRoot, NULL, 0);
	printf("root elem=[%s] has [%d] children and [%d] nodes\n",roxml_get_name(nRoot, NULL, 0),roxml_get_chld_nb(nRoot),roxml_get_nodes_nb(nRoot, ROXML_ALL_NODE));

	m = roxml_get_chld_nb(nRoot);
	printf("root elem has [%d] children\n",m);
	get_item_info(nRoot);

	//db_val=malloc(512);

	for (i=0 ; i<m ; i++) {

		nUser = roxml_get_chld(nRoot, NULL, i);
		printf("nUser elem=[%s] has [%d] children and [%d] nodes\n",roxml_get_name(nUser, NULL, 0),roxml_get_chld_nb(nUser),roxml_get_nodes_nb(nUser, ROXML_ALL_NODE));
		get_item_info(nUser);

		db_tag = roxml_get_name(nUser, NULL, 0);
		printf("DB TAG=[%s]\n",db_tag);

		//nName=roxml_get_ns(nUser);
		//db_val=roxml_get_content(nUser, NULL, 0, NULL);
		//db_val=roxml_get_attr(nUser, "database", 0); //not used at the moment
		//db_val =roxml_get_content(nUser,NULL,0,&len);

		//printf("type=[%d]\n",roxml_get_type(nUser));
		k=roxml_get_type(nUser);
		switch(k)
		{
			case ROXML_ATTR_NODE:
				printf("ROXML_ATTR_NODE\n");
				break;
			case ROXML_TXT_NODE:
				printf("ROXML_TXT_NODE\n");
				break;
			case ROXML_PI_NODE:
				printf("ROXML_PI_NODE\n");
				break;
			case ROXML_CMT_NODE:
				printf("ROXML_CMT_NODE\n");
				break;
			case ROXML_ELM_NODE:
				printf("ROXML_ELM_NODE\n");
				break;
			default: 
				printf("NONE\n");
				break;


		}
		printf("DB TAG=[%s]  len=[%d]\n",db_val,len);

		//nId = roxml_get_attr(nUser, "id", 0); not used at the moment
		printf("i=[%d]   n=[%d]\n",i,n);

		n = roxml_get_chld_nb(nUser);
		printf("i=[%d]   n=[%d]\n",i,n);

		for (j=0; j<n; j++) {

			nItem = roxml_get_chld(nUser, NULL,j);
			sTag1 = roxml_get_name(nItem, NULL, 0);

			printf("nItem elem=[%s] has [%d] children and [%d] nodes\n",roxml_get_name(nItem, NULL, 0),roxml_get_chld_nb(nItem),roxml_get_nodes_nb(nItem, ROXML_ALL_NODE));
			get_item_info(nItem);
			k=roxml_get_type(nItem);
			switch(k)
			{
				case ROXML_ATTR_NODE:
					printf("ROXML_ATTR_NODE\n");
					break;
				case ROXML_TXT_NODE:
					printf("ROXML_TXT_NODE\n");
					break;
				case ROXML_PI_NODE:
					printf("ROXML_PI_NODE\n");
					break;
				case ROXML_CMT_NODE:
					printf("ROXML_CMT_NODE\n");
					break;
				case ROXML_ELM_NODE:
					printf("ROXML_ELM_NODE\n");
					break;
				default: 
					printf("NONE\n");
					break;
			}

			o = roxml_get_chld_nb(nItem);

			for (k=0 ; k<o ; k++) {
				nTime = roxml_get_chld(nItem, NULL,k);
				sTag2 = roxml_get_name(nTime, NULL, 0);
				printf("nTime elem=[%s] has [%d] children and [%d] nodes\n",roxml_get_name(nTime, NULL, 0),roxml_get_chld_nb(nTime),roxml_get_nodes_nb(nTime, ROXML_ALL_NODE));
				//get_item_info(nTime);

				//printf("nTime content [%s]\n",roxml_get_content(roxml_get_txt(nTime,0),NULL,0,&len));
				//printf("nTime content [%s]\n",roxml_get_content(roxml_get_ns(nTime),NULL,0,&len));
				//printf("nTime content [%s]\n",roxml_get_content(roxml_get_ns(nTime),NULL,0,&len));
			}

			/*
			   if (strcmp(sTag1, "name") == 0) {

			   nName = roxml_get_txt(nItem,0);
			   userptr = new user( roxml_get_content(nName,NULL,0,&len) );

			} else if (strcmp(sTag1, "night") == 0) {

				nightptr = new night;
				o = roxml_get_chld_nb(nItem);

				for (k=0 ; k<o ; k++) {

					nTime = roxml_get_chld(nItem, NULL,k);
					sTag2 = roxml_get_name(nTime, NULL, 0);
					if (readTimeUnit(nTime,&tu)) {
						if (strcmp(sTag2, "start") == 0) {
							nightptr->setStart(tu);
						} else if (strcmp(sTag2, "stop") == 0) {
							nightptr->setStop(tu);
						} else if (strcmp(sTag2, "step") == 0) {
							nightptr->addStep(tu);
						}
					}
					roxml_release( (void*)sTag2 ); // has been created by the library
				}
				userptr->addNight(*nightptr); // the night has been totally parsed

			} else if (strcmp(sTag1, "cycle") == 0) {
				if (readTimeUnit(nItem,&tu)) {
					userptr->setCycle(tu);
				}
			} else if (strcmp(sTag1, "alarm") == 0) {
				if (readTimeUnit(nItem,&tu)) {
					userptr->setAlarm(tu);
				}
			}
		*/

			roxml_release( (void*)sTag1 ); // has been created by the library

		}
		//_model->add(userptr); // the user has been totally parsed
	}
	roxml_close(nRoot);

	return 1;
}

int main()
{
	return(xmlread("/tmp/db-table.xml"));
}
