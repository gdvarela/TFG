#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

#include "comm_TCP.h"

int main(int argc, char** argv){

	uint16_t port = RECV_PORT;
	msg_type_t packet_type = MSG_WOUT_ACK;
	sck_type_t sck_type = TCP_SERVER;

	msg_handler_t *handler = init_msg(port, packet_type, sck_type, NULL, NULL);

	int msg_length = 0;
  	while(1){
		msg_length = msg_recv(handler);
  	}

	close(handler->fd);

}
