/*
 * =====================================================================================
 *
 *       Filename:  fucker.c
 *
 *    Description:  libfucker implementation for interview daemon
 *
 *        Version:  1.0
 *        Created:  04/11/2014 13:22:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Peter H. Ezetta (phe), peter.ezetta@zonarsystems.com
 *   Organization:  R&D
 *
 * =====================================================================================
 */

#include <sys/socket.h>
#include <netdb.h>
#include "fucker.h"

// return a pointer to the client address string
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
