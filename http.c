#include "http.h"

/*
	Read HTTP Request from client socket

	@param sockfd, client socket
	@returns request string
*/
char* 
read_http_request(int sockfd)
{
	// Allocate and clear memory for request
	char* http_req_buffer = malloc(sizeof(char) * MAX_REQ_SIZE);
	memset(http_req_buffer, 0, MAX_REQ_SIZE);

	read(sockfd, http_req_buffer, MAX_REQ_SIZE);
	return http_req_buffer;
}

/*
	Int to string
	@param val, value
	@param base, base (use 10)
*/
char* 
itoa(uint32_t val, int base)
{
	
	static char buf[32] = {0};
	
	uint32_t i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

/*
	Parse HTTP Request into a http_request_t struct

	@param http_request, the request string
*/
http_request_t 
parse_http_request(char* http_request)
{
	http_request_t req;

	if (strncmp(strtok(http_request, " "), "GET", 3)){
		req.method = GET;
	}
	else{
		req.method = UNSUPPORTED;
	}

	req.uri = strtok(NULL, " ");
	req.host = " ";

	return req;
}

/*
	Add a field to the HTTP header

	@param header, pointer to the http header
	@field_name, name of the field
	@contents, contents of the field
*/
void 
add_field_to_header(char* header, char* field_name, char* contents)
{
	char* field = malloc((strlen(field_name) + strlen(contents)) * sizeof(char));
	int header_len = strlen(header) + strlen(field_name) + strlen(contents) + 7;

	header = realloc(header, header_len * sizeof(char));

	sprintf(field, "%s: %s\r\n", field_name, contents);

	strncat(header, field, strlen(field));
}

/*
	Generate the response header

	@param status, the HTTP response status
	@param content_len, the length of the content
	@param ext, the file extension
*/
char* 
generate_http_response_header(int status, int content_len, char* ext)
{
	int resp_len = 200;
	char* resp = malloc(resp_len * sizeof(char));
	char* mime = get_mime(ext);

	sprintf(resp, "HTTP/1.1 %d OK\r\nContent-Type: %s; charset=utf-8\r\n", status, mime);

	// Add length to header
	add_field_to_header(resp, "Content-Length", itoa(content_len, 10));

	resp = realloc(resp, (strlen(resp) + 4) * sizeof(char));
	int pos = strlen(resp);

	// Add end /r/n to header
	resp[pos] = '\r';
	resp[pos+1] = '\n';
	resp[pos+2] = '\0';

	return resp;
}

/*
	Read a file from the webroot

	@param filename, the name of the file
	@param len, a pointer where the length of the file will be updated
*/
char* 
read_file_webroot(char* file_name, int* len)
{
	long int file_length;
	FILE* fp;
	char* file_contents;

	fp = fopen(file_name, "r");

	printf("%s\n", file_name);

	if (!fp)
		return NULL;

	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	file_contents = malloc(file_length + 1);
	fread(file_contents, file_length, 1, fp);

	// Null terminate
	file_contents[file_length] = 0;
	
	fclose(fp);

	*len = file_length;

	return file_contents;
}

/*
	Generate the HTTP response (header + content)

	@param uri, the request URI
	@param status, the HTTP status code
	@param needAuth (not used)
	@param *response_len, the length of the response
*/
char* 
generate_http_response(char* uri, int status, bool needAuth, int* response_len)
{
	int content_len = 0;
	char* ext = malloc(sizeof(char) * 100);

	memmove(uri, uri+1, strlen(uri)); //Remove leading forward slash

	// Find the . and set the extension to the following text
	if (strrchr(uri, '.') != NULL){
		ext = strrchr(uri, '.') + 1;
	}
	else{
		// signal that there is no extension
		ext[0] = '\0';
	}
	
	char* contents = read_file_webroot(uri, &content_len);
	char* content_start = NULL;

	// File did not exist
	if (contents == NULL){
		status = 404;
		contents = malloc(sizeof(char) * strlen(ERROR_404_MSG));
		strncpy(contents, ERROR_404_MSG, strlen(ERROR_404_MSG));
		strncpy(ext, "html", strlen("html"));
		content_len = strlen(ERROR_404_MSG);
	}

	char* response = generate_http_response_header(status, content_len, ext);
	int header_len = strlen(response);
	response = realloc(response, (header_len + content_len + 2) * sizeof(char));
	content_start = response + (header_len * sizeof(char));
	memcpy(content_start, contents, content_len);
	*response_len = content_len + header_len;

	return response;
}
