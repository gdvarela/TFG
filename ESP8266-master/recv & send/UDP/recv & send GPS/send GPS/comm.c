/*
 ============================================================================
 Name        : comm.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Simple UDP communication
 ============================================================================
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>

#include "comm.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

msg_handler_t * init_msg(uint16_t port, msg_type_t pkt_type){
	msg_handler_t *handler = NULL;

	handler = calloc(1, sizeof(msg_handler_t));

	handler->port = port;

	handler->pkt_type = pkt_type;

	struct sockaddr_in msg_addr;

	if ( (handler->fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket()");

	msg_addr.sin_family = AF_INET;
	msg_addr.sin_addr.s_addr = INADDR_ANY;
	msg_addr.sin_port = htons(port);

	if (bind(handler->fd, (struct sockaddr *)&msg_addr, sizeof(msg_addr)) < 0) {
		die("bind()");
	}

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000 * TIMEOUT_MS;

	if ( setsockopt(handler->fd, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) ==-1)
		die("setsockopt()");

  	return handler;
}

int msg_recv(msg_handler_t *handler){
	return msg_recv_pkt(handler);
}

int msg_send(msg_handler_t *handler, char dst_ip[], uint16_t dst_port, void *payload, int payload_size){
	int ret = -1;
 	printf("Send msg id: %u [type: Data] to %s:%d\n",handler->id, dst_ip, dst_port);
	if(handler->pkt_type == MSG_WOUT_ACK){
		ret = msg_send_pkt(handler, dst_ip, dst_port, payload, payload_size);
		handler->id++;
	}
	else if(handler->pkt_type == MSG_W_ACK){
		int ret, n = -1;
		while(ret != n){
			ret = msg_send_pkt(handler, dst_ip, dst_port, payload, payload_size);
			n = msg_recv_pkt(handler);
		}
		handler->id++;
	}
		return ret;
}
int msg_recv_pkt(msg_handler_t *handler){

	char buf[MAX_PACKET_SIZE];
	int buf_size = 0;
  	struct sockaddr_in remAddr;
  	socklen_t addr_size = sizeof(remAddr);

  	buf_size = recvfrom(handler->fd, buf, MAX_PACKET_SIZE, 0, (struct sockaddr *)&remAddr, &addr_size );

  	if( buf_size >= sizeof(msg_hdr_t) && buf_size != -1){
		msg_hdr_t *hdr = (msg_hdr_t *)buf;
		hdr->msg_type = ntohs(hdr->msg_type);
		hdr->payload_size = ntohs(hdr->payload_size);
		hdr->msg_id = ntohl(hdr->msg_id);

		if(hdr->msg_type == MSG_ACK){
	    	printf("Recv msg id: %u [type: ACK] from %s:%d\n", hdr->msg_id, inet_ntoa(remAddr.sin_addr), ntohs(remAddr.sin_port));
			return hdr->msg_id;
		}
    	printf("Recv msg id: %u [type: Data] from %s:%d\n", hdr->msg_id, inet_ntoa(remAddr.sin_addr), ntohs(remAddr.sin_port));
    	memcpy(handler->data,buf + sizeof(msg_hdr_t),buf_size);

		if(hdr->msg_type == MSG_W_ACK){
			handler->pkt_type = MSG_ACK;
			handler->id = hdr->msg_id;
			int i = msg_send_pkt(handler, inet_ntoa(remAddr.sin_addr), ntohs(remAddr.sin_port), "", 1);
			handler->pkt_type = MSG_W_ACK;
		}

    	return buf_size;
  	}
  	else
  		return -1;
}

int msg_send_pkt(msg_handler_t *handler, char dst_ip[], uint16_t dst_port, void *payload, int payload_size){
	struct sockaddr_in targAddr;

	int buf_size = 0, ret = 0;
	void *buf = NULL;

	if(!handler || !payload || payload_size <=0)
		return -EINVAL;

	buf_size = sizeof(msg_hdr_t) + payload_size;
	buf = malloc(buf_size);

	if(!buf)
		return -ENOMEM;

	msg_hdr_t *hdr = (msg_hdr_t *)buf;
	hdr->msg_id = htonl(handler->id);
	hdr->msg_type = htons(handler->pkt_type);
	hdr->payload_size = htons(payload_size);

	memcpy(buf+sizeof(msg_hdr_t),payload,payload_size);

    memset((char *) &targAddr, 0, sizeof(targAddr));
  	targAddr.sin_family = AF_INET;
  	targAddr.sin_port = htons(dst_port);
  	targAddr.sin_addr.s_addr = inet_addr(dst_ip);
	if( sendto(handler->fd,buf,buf_size,0,(struct sockaddr *)&targAddr,sizeof(targAddr)) == -1)
		die("sendto()");

	ret = handler->id;
	if(buf)
		free(buf);

	return ret;
}

int msg_close(msg_handler_t *handler){
	if(!handler)
		return -EINVAL;
	close(handler->fd);
	free(handler);
	return 1;
}
