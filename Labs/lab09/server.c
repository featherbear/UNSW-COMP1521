//
// server.c -- A webserver written in C
// Based on code from LambdaSchool
// 
// Test with curl
//    curl -D - http://localhost:3490/
//    curl -D - http://localhost:3490/date
//    curl -D - http://localhost:3490/hello
//    curl -D - http://localhost:3490/hello?You
// 
// You can also test the above URLs in your browser! They should work!

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>

#define PORT 8321  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

#define BUF_SIZE 65536

void failed(char *);

// Send an HTTP response
//
// header:       "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK", etc.
// content_type: "text/plain", etc.
// body:         the data to send.
// 
int send_response(int fd, char *header, char *content_type, char *body) {
    char response[BUF_SIZE];
    int response_length; // Total length of header plus body

    sprintf(response, "%s\n%s\r\n\r\n%s\n", header, content_type, body);
    response_length = strlen(response);
    int rv = send(fd, response, response_length, 0);
    if (rv < 0) failed("send");
    return rv;
}

// Send a generic 200 response
void send_generic(int fd, char* msg) {
    send_response(fd, "HTTP/1.1 200 OK",
                  "text/html",
                  msg);
}
// Send a 404 response
void send_404(int fd) {
    send_response(fd, "HTTP/1.1 404 NOT FOUND",
                  "text/html",
                  "<h1>404 Page Not Found</h1>");
}

// Send a 500 response
void send_500(int fd) {
    send_response(fd, "HTTP/1.1 500 Internal Server Error",
                  "text/html",
                  "<h1>Mangled request</h1>");
}

// Send response for server root
void send_root(int fd) {
    send_response(fd, "HTTP/1.1 200 OK",
                  "text/html",
                  "Server running...");
}

// Send a /date endpoint response
void send_date(int fd) {
    // http://www.cplusplus.com/reference/clibrary/ctime/localtime/
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    send_response(fd, "HTTP/1.1 200 OK",
                  "text/html",
                  asctime(timeinfo));
}

// Send a /hello endpoint response
void send_hello(int fd, char *arg) {
    // arg is NULL if empty
    if (arg) {
        char *body = malloc(5 + 2 + strlen(arg) + 1 + 1);
        sprintf(body, "Hello, %s!", arg);
        send_response(fd, "HTTP/1.1 200 OK",
                      "text/html",
                      body);
        free(body);
    } else {
        send_response(fd, "HTTP/1.1 200 OK",
                      "text/html",
                      "Hello");
    }


}

int isEndpoint(char *path, char *endpoint) {
    int endpointLength = strlen(endpoint);
    if (endpoint[endpointLength - 1] == '/') endpointLength--;

    int pathLength = strlen(path);
    if (path[pathLength - 1] == '/') endpointLength--;

    return (strncmp(path, endpoint, pathLength > endpointLength ? pathLength : endpointLength) == 0);
}

int getQueryPosition(char *req_path) {
    for (int i = 0; i < (1024 - 1) && req_path[i] != '\0'; i++) {
        if (req_path[i] == '?') {
            return i;
        }
    }
    return -1;
}

char *extractPath(char *req_path) {
    // malloc even if there is no query, so we can free later without checking
    char *path;
    int position = getQueryPosition(req_path);

    if (position == -1) position = strlen(req_path);

    path = malloc(position + 1);
    strncpy(path, req_path, position);
    path[position] = '\0';

    return path;
}

char *extractQuery(char *req_path) {
    int position = getQueryPosition(req_path);
    if (position != -1 && req_path[++position] != '\0') return req_path + position;
    return NULL;
}

// Handle HTTP request and send response
void handle_http_request(int fd) {
    char request[BUF_SIZE];
    char req_type[8]; // GET or POST
    char req_path[1024]; // /info etc.
    char req_protocol[128]; // HTTP/1.1

    // Read the request
    int nbytes = recv(fd, request, BUF_SIZE - 1, 0);
    if (nbytes < 0) failed("recv");
    request[nbytes] = '\0';

    // for logging/debugging
//    printf("Request: ");
//    for (char *c = request; *c != '\n'; c++)
//        putchar(*c);
//    putchar('\n');
    // if you want to see the whole HTTP header, use
    // printf("%s\n", request);

    // Get the request type, path and protocol from the first line
    // If you can't decode the request, generate a 500 error response
    // Otherwise call appropriate handler function, based on path
    // Hint: use sscanf() and strcmp()

    sscanf(request, "%s %s %s", req_type, req_path, req_protocol);

    char *path = extractPath(req_path);
    char *query = extractQuery(req_path);

    printf("Path: %s\n", path);
    if (query) printf("Query: %s\n", query);


    if (strcmp(req_type, "GET") == 0) {
        // GET request
        if (isEndpoint(path, "/")) send_root(fd);
        else if (isEndpoint(path, "/date/")) send_date(fd);
        else if (isEndpoint(path, "/hello/")) send_hello(fd, query);
        else send_404(fd);
    } else if (strcmp(req_type, "POST") == 0) {
        // POST request
        if (isEndpoint(path, "/hello/")) send_generic(fd, "YEEET");
        else send_404(fd);
    } else {
        send_500(fd);
    }

    free(path);
}

// fatal error handler
void failed(char *msg) {
    char buf[100];
    sprintf(buf, "WebServer: %s", msg);
    perror(buf);
    exit(1);
}

int main(int argc, char *argv[]) {
    int listenfd, newfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    struct sockaddr *addr;

    // set up a socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) failed("opening socket");
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    // bind the socket to an address localhost:PortNo
    portno = PORT;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    addr = (struct sockaddr *) &serv_addr;
    if (bind(listenfd, addr, sizeof(serv_addr)) < 0)
        failed("binding socket");
    // listen for connections to that address
    listen(listenfd, BACKLOG);
    printf("WebServer (PID %d): waiting for connections on port %d...\n", getpid(), portno);

    socklen_t cli_len = sizeof(cli_addr);
    addr = (struct sockaddr *) &cli_addr;
    while (1) {
        // accept a connection from a client
        newfd = accept(listenfd, addr, &cli_len);
        if (newfd < 0) failed("accept");

        // Print out a message that we got the connection
        printf("WebServer: got connection\n");

        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.
        handle_http_request(newfd);
        close(newfd);
    }
    close(listenfd);
    return 0; /* we never get here */
}

