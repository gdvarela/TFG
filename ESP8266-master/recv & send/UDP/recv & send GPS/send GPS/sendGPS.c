#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include "comm.h"
#include "readNSI.h"

#define period_ms	90						/* The nsi table is filled every 10-90 ms */

void buildPayload(neighInfo_t neighInfo, char *payload, int *payload_size){

	strcpy(payload,neighInfo.lat);
	strcat(payload," ");
	strcat(payload,neighInfo.lon);
	strcat(payload," ");
	strcat(payload,neighInfo.elev);
	strcat(payload," ");
	strcat(payload,neighInfo.head);
	strcat(payload," ");
	strcat(payload,neighInfo.speed);
	strcat(payload," ");
	strcat(payload,neighInfo.id);

	*payload_size = strlen(payload)+1;
}
int main(int argc, char** argv){

	uint16_t dst_port = RECV_PORT;
	uint16_t port = 9000;
	msg_type_t packet_type = MSG_W_ACK;
	msg_handler_t *handler = init_msg(port, packet_type);

	char* target_ip = "127.0.0.1";
	struct timespec time;
	neighInfo_t neighList[MAX_NEIGHBORS];

	char payload[64];
	int payload_size;

	time.tv_sec = 0;
	time.tv_nsec = period_ms*1000000L;

	int ret = 0;
	int numNeigh;
	int i = 0;
	while(1){
		numNeigh = neigh_info(neighList);
		for(i = 0;i<numNeigh;i++){
			buildPayload(neighList[i], &payload,  &payload_size);
			ret = msg_send(handler, target_ip, dst_port, payload, payload_size);
			nanosleep(&time,NULL);
		}
	}
	close(handler->fd);
}
