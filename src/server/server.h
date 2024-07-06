#ifndef SERVER_H
#define SERVER_H

const char *get_file_extension(const char *f_name);
const char *get_mime_type(const char *f_ext);
const char *start_server(void);

#endif // SERVER_H