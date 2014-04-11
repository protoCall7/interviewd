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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include "net.h"
#include "fork.h"

int main(int argc, char *argv[])
{
    int status, sock, new_fd, errno;
    struct addrinfo hints, *res, *p;
    struct sockaddr_storage their_addr;
    struct sigaction sa;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];

    // set up hints structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // fill out the res structure from values in hints
    if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // loop through linked list and bind to first address possible
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

    // we don't need this anymore
    freeaddrinfo(res);

    // start listening on bound socket and enter main loop
    if ((status = listen(sock, QUEUE)) != 0 ) {
        perror("listen error");
        exit(5);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(6);
    }

    printf("Now waiting for connections\n");

    /*-----------------------------------------------------------------------------
     *  main loop starts here!!
     *-----------------------------------------------------------------------------*/
    while (1) {
        sin_size = sizeof their_addr;
        
        // accept incoming connections
        if ((new_fd = accept(sock, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        // report new connection
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        // child process starts here
        if (!fork()) { 
            // the child doesn't need the socket
            close(sock); 
            if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }

        // parent doesn't need the connection
        close(new_fd);
    }

    return 0;
}
