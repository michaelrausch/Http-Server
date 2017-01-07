#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "http.h"
#include "mime.h"

#define MAX_THREADS 30
#define WS_PORT 8082

pthread_mutex_t lock;

/*
	Create a TCP socket for the webserver to listen on

	@param portno, the port number for the server to listen on
	@returns the socket file descriptor
*/
int
make_ws_socket(int portno)
{
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// Error opening socket
	if (sockfd < 0)
		return -1;

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Error binding socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		return -1;

	return sockfd;
}

/*
	Thread handler for HTTP requests

	@param sockfd, socket descriptor for client connection
*/
void
handle_http_request(int sockfd)
{
	char* rx_buffer = read_http_request(sockfd);
	http_request_t req;
	int response_len = 0;
	
	// Lock thread while parsing request
	pthread_mutex_lock(&lock);
	req = parse_http_request(rx_buffer);
	pthread_mutex_unlock(&lock);

	// Generate and send HTTP response
	char* resp = generate_http_response(req.uri, 200, false, &response_len);

	write(sockfd, resp, response_len);

	free(rx_buffer);
	close(sockfd);
}

int
main(void)
{
	// Create sockets for web server
	int ws_sock = make_ws_socket(WS_PORT);
	long int recv_sock;

	struct sockaddr_in client_addr;
	unsigned int client_addr_len = sizeof(client_addr);

	// Create thread pool
	pthread_t threads[MAX_THREADS];
	int threadno = 0;

	init_mimes();

	// Could not bind socket
	if (ws_sock < 0){
		printf("FATAL ERROR: Could not bind to port %d\n", WS_PORT);
		return 1;
	}

	listen(ws_sock, 5);

	while (1){
		// Client socket
		recv_sock = accept(ws_sock, (struct sockaddr*) &client_addr, &client_addr_len);

		if (recv_sock < 0)
			printf("Error handling a client");

		// Create response thread
		pthread_create(&threads[threadno], NULL, (void*) handle_http_request, (void*) recv_sock);

		threadno++;

		if (threadno >= MAX_THREADS){
			threadno = 0;
		}
	}

	close(ws_sock);
}