#ifndef MIME_H
#define MIME_H

#include <string.h>
#include <stdlib.h>

typedef struct{
	char* ext;
	char* mime;
} file_type_t;

/*
	Add a mime to the pool

	@param ext, File extension for the MIME type
	@param mime, The MIME text
*/
void 
add_mime(char* ext, char* mime);

/*
	Return the MIME for an extension, uses octet-stream if 
	extension could not be found

	@param ext, the file extension
	@returns the MIME type
*/
char* 
get_mime(char* ext);

/*
	Initialise the mime types
*/
void 
init_mimes();

#endif