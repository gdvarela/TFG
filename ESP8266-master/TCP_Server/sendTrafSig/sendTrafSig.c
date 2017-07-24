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
#include "comm_TCP.h"

#define period_ms	100


int main(void) {

	msg_handler_t *handler;
	uint16_t dst_port = RECV_PORT;
	uint16_t port = 9000;
	msg_type_t packet_type = MSG_WOUT_ACK;
	sck_type_t sck_type = TCP_CLIENT;

	/* For testing */
	char dst_ip[] = "127.0.0.1";
	char *payload = "\nHello world";
	int payload_size = strlen(payload)+1;

	/* Set the period */
	struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = period_ms*1000000L;

	int ret;
	while(1){
		/* Scan for APs... */
		/* Connect... */
		/* dst_ip = ... 	Get IP of the next AP */
		handler = init_msg(port,packet_type, sck_type, dst_ip, dst_port);
		/* Build Payload...*/
		ret = msg_send(handler, payload, payload_size);
		ret = msg_close(handler);
		nanosleep(&time,NULL);
	}
}
