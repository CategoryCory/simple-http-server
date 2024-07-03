#include <string.h>

const char *get_file_extension(const char *f_name)
{
    const char *dot = strrchr(f_name, '.');
    if (!dot || dot == f_name) return "";

    return dot + 1;
}

const char *get_mime_type(const char *f_ext)
{
    if (strcasecmp(f_ext, "html") == 0 || strcasecmp(f_ext, "htm") == 0)
    {
        return "text/html";
    }
    else if (strcasecmp(f_ext, "txt") == 0)
    {
        return "text/plain";
    }
    else if (strcasecmp(f_ext, "jpg") == 0 || strcasecmp(f_ext, "jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcasecmp(f_ext, "png") == 0)
    {
        return "image/png";
    }
    else if (strcasecmp(f_ext, "json") == 0)
    {
        return "application/json";
    }
    else
    {
        return "application/octet-stream";
    }
}