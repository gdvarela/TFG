/*
 * comm.h
 *
 *  Created on: Oct 5, 2016
 *      Author: joao
 */
#ifndef COMM_TCP_H_
#define COMM_TCP_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>



#define MAX_PACKET_SIZE 1460
#define RECV_PORT 7891
#define TIMEOUT_MS 100

typedef enum{
	MSG_WOUT_ACK,
	MSG_W_ACK,
	MSG_ACK
} msg_type_t;

typedef enum{
	UDP_CLIENT,
	UDP_SERVER,
	TCP_CLIENT,
	TCP_SERVER
} sck_type_t;

struct msg_handler{
	int fd;
	uint16_t port;
	uint32_t id;
	msg_type_t pkt_type;
	sck_type_t sck_type;
	char data[MAX_PACKET_SIZE];
};
typedef struct msg_handler msg_handler_t;

struct msg_hdr {
	uint32_t msg_id;
	uint16_t msg_type;
	uint16_t payload_size;
};
typedef struct msg_hdr msg_hdr_t;

int msg_recv(msg_handler_t *);
int msg_recv_pkt(msg_handler_t *);
msg_handler_t * init_msg(uint16_t , msg_type_t, sck_type_t, char[] , uint16_t);
int msg_send(msg_handler_t *, void*, int);
int msg_send_pkt(msg_handler_t *, void*, int);
int msg_close(msg_handler_t *);

#endif /* COMM_TCP_H_ */
