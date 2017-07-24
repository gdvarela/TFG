#include "comm_TCP.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>

#define LISTEN_BACKLOG 50
void die(char *s)
{
    perror(s);
    exit(1);
}

msg_handler_t * init_msg(uint16_t port, msg_type_t pkt_type, sck_type_t sck_type, char dst_ip[], uint16_t dst_port){
	msg_handler_t *handler = NULL;

	handler = calloc(1, sizeof(msg_handler_t));
	handler->port = port;
	handler->pkt_type = pkt_type;
	handler->sck_type = sck_type;


	if ( (handler->fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		die("socket()");
		if(sck_type == TCP_SERVER){
			struct sockaddr_in msg_addr;

			msg_addr.sin_family = AF_INET;
			msg_addr.sin_addr.s_addr = INADDR_ANY;
			msg_addr.sin_port = htons(port);

			struct timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 1000 * TIMEOUT_MS;
			int enable = 1;

			if ( setsockopt(handler->fd, SOL_SOCKET, SO_REUSEADDR,&enable,sizeof(int)) ==-1)
				die("setsockopt()");

			/*if ( setsockopt(handler->fd, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) ==-1)
				die("setsockopt()");
			*/
			if (bind(handler->fd, (struct sockaddr *)&msg_addr, sizeof(msg_addr)) < 0) {
				die("bind()");
			}

			if ( listen( handler->fd,LISTEN_BACKLOG) ==-1)
				die("listen()");
		}
	else if(sck_type == TCP_CLIENT){
		struct sockaddr_in targAddr;

	  	memset((char *) &targAddr, 0, sizeof(targAddr));
	  	targAddr.sin_family = AF_INET;
	  	targAddr.sin_port = htons(dst_port);
	  	targAddr.sin_addr.s_addr = inet_addr(dst_ip);

		if( connect(handler->fd, (struct sockaddr *)&targAddr,sizeof(targAddr)) == -1)
			die("connect()");

	}
  	return handler;
}


int msg_send(msg_handler_t *handler, void *payload, int payload_size){
	int ret = -1;

 	printf("Send msg id: %u [type: Data]\n",handler->id);
	if(handler->pkt_type == MSG_WOUT_ACK){
		ret = msg_send_pkt(handler, payload, payload_size);
		handler->id++;
	}
		return ret;
}

int msg_send_pkt(msg_handler_t *handler, void *payload, int payload_size){

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

	if( write(handler->fd,buf,buf_size) == -1)
		die("sendto()");

	        if( write(handler->fd,buf,buf_size) == -1)
                die("sendto()");

	ret = handler->id;
	if(buf)
	free(buf);

	return ret;
}

int msg_recv(msg_handler_t *handler){
	return msg_recv_pkt(handler);
}

int msg_recv_pkt(msg_handler_t *handler){

	char buf[MAX_PACKET_SIZE];
	int buf_size = 0;

	int conn_fd = accept(handler->fd, (struct sockaddr*)NULL ,NULL);

  	buf_size = read(conn_fd, buf, MAX_PACKET_SIZE);
  	if( buf_size >= sizeof(msg_hdr_t) && buf_size != -1){
		msg_hdr_t *hdr = (msg_hdr_t *)buf;
		hdr->msg_type = ntohs(hdr->msg_type);
		hdr->payload_size = ntohs(hdr->payload_size);
		hdr->msg_id = ntohl(hdr->msg_id);

    		printf("Recv msg id: %u [type: Data]\n", hdr->msg_id);
    		memcpy(handler->data,buf + sizeof(msg_hdr_t),buf_size);
		printf("Data Received: %s\n",handler->data);
    		close(conn_fd);
	return buf_size;
  	}
  	else
  		return -1;
}


int msg_close(msg_handler_t *handler){
	if(!handler)
		return -EINVAL;
	close(handler->fd);
	free(handler);
	return 1;
}
