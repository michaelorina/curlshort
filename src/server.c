#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/database.h"

#define PORT 8080

enum MHD_Result request_handler(void *cls, struct MHD_Connection *conn, 
                                const char *url, const char *method, 
                                const char *version, const char *upload_data,
                                size_t *upload_data_size, void **con_cls) {
    struct MHD_Response *response;
    enum MHD_Result ret;

    static char original_url[1024]; 

    if (strcmp(method, "POST") == 0) {
        if (*upload_data_size > 0) {
            strncpy(original_url, upload_data, *upload_data_size);
            original_url[*upload_data_size] = '\0'; 

            *upload_data_size = 0;  
            return MHD_YES;
        }
        
        char *short_url = insert_url(original_url);
        if (!short_url) {
            response = MHD_create_response_from_buffer(19, "URL Shortening Failed", MHD_RESPMEM_PERSISTENT);
        } else {
            response = MHD_create_response_from_buffer(strlen(short_url), short_url, MHD_RESPMEM_MUST_FREE);
        }
    } else {
        response = MHD_create_response_from_buffer(21, "Welcome to curlshort!", MHD_RESPMEM_PERSISTENT);
    }

    ret = MHD_queue_response(conn, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}


void start_server() {
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, 
                              NULL, NULL, &request_handler, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "Failed to start server\n");
        return;
    }
    printf("Server running on port %d...\n", PORT);
    getchar();
    MHD_stop_daemon(daemon);
}
