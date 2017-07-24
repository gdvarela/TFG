/*
 * readNSI.h
 *
 *  Created on: Feb 2, 2017
 *      Author: joao
 */

#ifndef READNSI_H_
#define READNSI_H_

#define MAX_NEIGHBORS 8
struct neighInfo{

	char lat[16];
	char lon[16];
	char elev[16];

	char head[8];
	char speed[8];
	char id[8];
};
typedef struct neighInfo neighInfo_t;

int neigh_info(neighInfo_t *);

#endif /* READNSI_H_ */
