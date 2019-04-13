#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define PAGE "<html><head><title>libmicrohttpd demo</title>" \
             "</head><body>libmicrohttpd demo</body></html>"

static int ahc_echo(void *cls,
                    struct MHD_Connection *connection,
                    const char *url,
                    const char *method,
                    const char *version,
                    const char *upload_data,
                    size_t *upload_data_size,
                    void **ptr)
{
    static int dummy;
    const char *page = cls;
    struct MHD_Response *response;
    int ret;

    if (0 != strcmp(method, "GET"))
        return MHD_NO; /* unexpected method */
    if (&dummy != *ptr)
    {
        /* The first time only the headers are valid,
         do not respond in the first round... */
        *ptr = &dummy;
        return MHD_YES;
    }
    if (0 != *upload_data_size)
        return MHD_NO; /* upload data in a GET!? */
    *ptr = NULL;       /* clear context pointer */
    response = MHD_create_response_from_buffer(strlen(page),
                                               (void *)page,
                                               MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection,
                             MHD_HTTP_OK,
                             response);
    MHD_destroy_response(response);
    return ret;
}


int read_file(char** side, char* file_name){
    FILE *fp;
    struct stat s;
    int rett = stat(file_name, &s);
    if (rett != 0)
    {
        perror(file_name);

        return 1;
    }
    printf("Size of index.html: %ld\n", s.st_size);
    *side = malloc(s.st_size);

    fp = fopen(file_name, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // printf("The contents of %s file are:\n", file_name);

    fread(*side, s.st_size, 1, fp);

    fclose(fp);
}

int main(int argc,
         char **argv)
{
    struct MHD_Daemon *d, d2;
    if (argc != 2)
    {
        printf("%s PORT\n",
               argv[0]);
        return 1;
    }

    char *file_name = "index.html";
    char* side;

    int ret = read_file(&side, file_name);
    if(ret != 0){
        perror("read_file failed");
        return ret;
    }

    d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
                         atoi(argv[1]),
                         NULL,
                         NULL,
                         &ahc_echo,
                         side,
                         MHD_OPTION_END);
    if (d == NULL)
        return 1;
    
    (void)getc(stdin);
    MHD_stop_daemon(d);
    return 0;
}