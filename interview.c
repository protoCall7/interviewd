/*
 * =====================================================================================
 *
 *       Filename:  interview.c
 *
 *    Description:  The interview daemon!
 *
 *        Version:  1.0
 *        Created:  04/11/2014 11:36:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Peter H. Ezetta (phe), peter.ezetta@zonarsystems.com
 *   Organization:  R&D
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT "5555"
#define QUEUE 10

void *get_in_addr(struct sockaddr *sa);

int main(int argc, char *argv[])
{
    int status, sock, new_fd, errno;
    struct addrinfo hints, *res, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];

    /*-----------------------------------------------------------------------------
     *  set up hints struct
     *-----------------------------------------------------------------------------*/
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    /*-----------------------------------------------------------------------------
     *  set up servinfo struct or exit and return 1
     *-----------------------------------------------------------------------------*/
    if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    /*-----------------------------------------------------------------------------
     *  loop through returned addresses and bind to the first one possible
     *-----------------------------------------------------------------------------*/
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket error");
            exit(2);
        }

        if ((status = bind(sock, p->ai_addr, p->ai_addrlen)) != 0) {
            perror("bind error");
            exit(3); 
        } 
        break;
    }
    
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(4);
    }

    freeaddrinfo(res);

    /*-----------------------------------------------------------------------------
     *  listen for incoming connections on socket or exit and return 4
     *-----------------------------------------------------------------------------*/
    if ((status = listen(sock, QUEUE)) != 0 ) {
        perror("listen error");
        exit(5);
    }

    printf("Now waiting for connections\n");

    while (1) {
        sin_size = sizeof their_addr;
        if ((new_fd = accept(sock, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
    }

    return 0;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
