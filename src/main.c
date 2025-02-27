#include "../include/server.h"
#include "../include/database.h"
#include <stdio.h>

int main() {
    printf("Initializing database...\n");
    init_db();
    start_server();

    return 0;
}