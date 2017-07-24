/*
 * readNSI.c
 *
 *  Created on: Feb 2, 2017
 *      Author: joao
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <errno.h>
#include "readNSI.h"

int neigh_info(neighInfo_t neighList[MAX_NEIGHBORS]){

	FILE *fp;
	int status;
	int i = 0;											/* Number of neighbors in the NSI table */
	char buf[1024];

	//fp = popen("uwme -n", "r");						/* Read the NSI table */
	fp = fopen("test4","r");

	if (fp == NULL)
		return -1;

	fscanf(fp," %s %s %s %s",buf,buf,buf,buf);			/* "Showing NSI table entries:" */

	while(1){
		status = fscanf(fp," %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",buf,buf,buf,neighList[i].id,buf,buf,buf,buf,buf,buf,buf,neighList[i].lat,buf,buf,neighList[i].lon,buf,buf,neighList[i].speed,buf,buf,neighList[i].elev,buf,buf,neighList[i].head,buf,buf,buf,buf,buf,buf,buf,buf,buf,buf,buf,buf,buf,buf,buf,buf);
		if(status == 40)								/* If there is another entry in the NSI table */
			i++;
		else
			break;
	}
	status = fclose(fp);
	return i;											/* Returns the number of neighbors available in the NSI table*/
}
