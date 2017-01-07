#ifndef HTTP_H
#define HTTP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include "mime.h"

#define MAX_REQ_SIZE 8000
#define ERROR_404_MSG "Error 404, Page Not Found"

/*
	HTTP Methods
*/
typedef enum {
	GET,
	POST,
	UNSUPPORTED
} http_method_t;

/*
	HTTP Request Struct
*/
typedef struct {
	http_method_t method;
	char* uri;
	char* host;
} http_request_t;

char* 
read_http_request(int sockfd);

http_request_t 
parse_http_request(char* http_request);
char* 
generate_http_response(char* uri, int status, bool needAuth, int* response_len);

#endif