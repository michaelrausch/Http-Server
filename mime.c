#include "mime.h"

file_type_t mimes[100]; // Mime Pool
int mime_no = 0; // Current Mime Position


/*
	Add a mime to the pool

	@param ext, File extension for the MIME type
	@param mime, The MIME text
*/
void 
add_mime(char* ext, char* mime)
{
	// Allocate memory for MIME type
	mimes[mime_no].ext = malloc((strlen(ext) * sizeof(char)) + 2);
	mimes[mime_no].mime = malloc((strlen(mime) * sizeof(char)) + 2);

	// Clear Memory
	memset(mimes[mime_no].ext, 0, (strlen(ext) * sizeof(char)) + 2);
	memset(mimes[mime_no].mime, 0, (strlen(mime) * sizeof(char)) + 2); 

	// Copy ext and mime to pool
	strncpy(mimes[mime_no].ext, ext, strlen(ext));
	strncpy(mimes[mime_no].mime, mime, strlen(mime));

	mime_no++;
}


/*
	Return the MIME for an extension, uses octet-stream if 
	extension could not be found

	@param ext, the file extension
	@returns the MIME type
*/
char* 
get_mime(char* ext)
{
	if(!ext || ext[0] == '\0'){
		return "application/octet-stream";
	}
	
	for (int i = 0; i < mime_no; i++){
		if (strlen(mimes[i].ext) == strlen(ext) && strncmp(mimes[i].ext, ext, strlen(mimes[i].ext)) == 0){
			return mimes[i].mime;
		}
	}

	return "application/octet-stream";
}


/*
	Initialise the mime types
*/
void init_mimes()
{
	add_mime("zip", "application/zip");
	add_mime("xml", "application/xml");
	add_mime("txt", "text/plain");
	add_mime("rar", "application/x-rar-compressed");
	add_mime("png", "image/png");
	add_mime("ogg", "audio/ogg");
	add_mime("mp4", "video/mp4");
	add_mime("jpg", "image/jpeg");
	add_mime("jpeg", "image/jpeg");
	add_mime("json", "application/json");
	add_mime("jar", "application/java-archive");
	add_mime("html", "text/html");
	add_mime("gif", "image/gif");
	add_mime("css", "text/css");
	add_mime("js", "application/javascript");
}