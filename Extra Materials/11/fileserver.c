/***** inetserver.c *****/
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>        /* for getenv */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>    /* Internet domain header */
#include <time.h>
#include "wrapsock.h"

#ifndef PORT
#define PORT 30000
#endif

ssize_t Readn(int fd, void *ptr, size_t nbytes);
void Writen(int fd, void *ptr, size_t nbytes);

int main()
{
    int soc, ns;
    int on = 1, status;
    char buf[256];
    struct sockaddr_in peer;
    struct sockaddr_in self;
    unsigned int peer_len = sizeof(peer);
    char host[256];

    self.sin_family = PF_INET;
    self.sin_port = htons(PORT);
    printf("Listening on %d\n", PORT);
    self.sin_addr.s_addr = INADDR_ANY;
    bzero(&(self.sin_zero), 8);

    printf("PORT=%d\n", PORT);

    peer.sin_family = PF_INET;
    /* set up listening socket soc */
    soc = Socket(PF_INET, SOCK_STREAM, 0);
    if (soc < 0) {
        perror("server:socket");
        exit(1);
    }

    // Make sure we can reuse the port immediately after the
    // server terminates.
    status = setsockopt(soc, SOL_SOCKET, SO_REUSEADDR,
                        (const char *) &on, sizeof(on));
    if (status == -1) {
        perror("setsockopt -- REUSEADDR");
    }

    // Associate the proess with the address and a port
    Bind(soc, (struct sockaddr *)&self, sizeof(self));

    // Sets up a queue in the kernel to hold pending connections
    Listen(soc, 1);

    /* accept connection request */
    printf("Calling accept\n");
    ns = Accept(soc, (struct sockaddr *)&peer, &peer_len);

    /* Find out our name */
    int err;
    if ((err = gethostname(host, 256)) == -1) {
        perror("gethostname");
        exit(1);
    }
    /* Server first expects to read a size as an int */
    int fsize = 0;
    if (Readn(ns, &fsize, sizeof(int)) == 0) {
        exit(1);
    }
    fsize = ntohl(fsize);
    printf("SERVER ON %s RECEIVED: %d\n", host, fsize);

    /* Now read the file that the client is sending, sizeof(buf) bytes
     * at a time.  Keep track of how many bytes have been read so far
     * in cursize, so that you know when to stop reading. Print what you
     * have read to stderr.
     * Hint: Readn (or read) returns the number of bytes actually read.
     */
    int n, cursize;

    // Add code here

    n = Readn(ns, buf, sizeof(buf));
    fprintf(stderr, "%s", buf);
    cursize = n;
    while (n != 0 && fsize - cursize > 0) {
        n = Readn(ns, buf, sizeof(buf));
        fprintf(stderr, "%s", buf);
        cursize = n;
    }

    Close(ns);
    Close(soc);
    return (0);
}
