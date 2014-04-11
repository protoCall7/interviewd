/*
 * =====================================================================================
 *
 *       Filename:  fork.c
 *
 *    Description:  fork functions
 *
 *        Version:  1.0
 *        Created:  04/11/2014 13:47:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Peter H. Ezetta (phe), peter.ezetta@zonarsystems.com
 *   Organization:  R&D
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <sys/wait.h>
#include "fork.h"

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
