#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER_SIZE 4096
#define PORT 80

int link_num = 0;

void printLink(char *buffer) {
    char *pos = buffer;
    while ((pos = strstr(pos, "<a")) != NULL) {
        pos = strstr(pos, "href");
        if (pos == NULL) {
            continue;
        }

        pos = strchr(pos, '"');
        if (pos == NULL) {
            continue;
        }

        pos++;
        char *end = strchr(pos, '"');
        if (end == NULL) {
            continue;
        }

        int len = end - pos;
        char link[MAX_BUFFER_SIZE];
        strncpy(link, pos, len);
        link[len] = '\0';
        printf("Link_num: %d\n%s\n", link_num+1, link);

        link_num++;
        pos = end + 1;
    }
}

int main(void) {
    char address[300];
    char address2[300];
    printf("student's Id: 110062271\n");
    printf("Please Enter The URL:\n");
    scanf("%s", address);
    strcpy(address2, address);

    printf("======================\n");
    printf("======================\n");

    char *host;
    char *t_path;
    char path[300];


    host = strtok(address, "/");
    host = strtok(NULL, "/");
    t_path = strtok(address2, "//");
    t_path = strtok(NULL, "/");
    t_path = strtok(NULL, "/");

    if(t_path) sprintf(path, "/%s", t_path); 
    else sprintf(path, "/");  // path is empty


    struct hostent *hostent = gethostbyname(host);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr, hostent->h_addr, hostent->h_length);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
    }

    const char *getRequest = "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n";
    char request[MAX_BUFFER_SIZE];
    sprintf(request, getRequest, path, host);
    send(sockfd, request, strlen(request), 0);


    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        printLink(buffer);
    }

    printf("======================\n");

    printf("\nWe have found %d hyperlinks\n", link_num);

    close(sockfd);

    return 0;
}
