#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include "wrapsock.h"

#ifndef PORT
#define PORT 30000
#endif

int main(int argc, char* argv[]) {
    int soc;
    struct hostent *hp;
    struct sockaddr_in peer;
    struct stat sbuf;
    FILE *fp;
    char buf[2048];

    if ( argc != 3 ) {
        fprintf(stderr, "Usage: %s hostname filename\n", argv[0]);
        exit(1);
    }

    /* fill in peer address */
    hp = gethostbyname(argv[1]);
    if ( hp == NULL ) {
        fprintf(stderr, "%s: %s unknown host\n",
                argv[0], argv[1]);
        exit(1);
    }

    /* Set up the struct to initialize the socket */
    peer.sin_family = PF_INET;
    peer.sin_port = htons(PORT);

    printf("PORT = %d\n", PORT);
    peer.sin_addr = *((struct in_addr *)hp->h_addr);

    /* create socket */
    soc = Socket(PF_INET, SOCK_STREAM, 0);

    /* request connection to server */
    if (Connect(soc, (struct sockaddr *)&peer, sizeof(peer)) == -1) {
        Close(soc);
        exit(1);
    }

    /* Find the size of the file */
    if (stat(argv[2], &sbuf) == -1) {
        perror("stat");
        exit(1);
    }

    /* sbuf.st_size now holds the size of the file, but it
     * must first be translated to network byte order. */
    // Add code here
    int num = htonl(sbuf.st_size);

    /* Write the int to the server */
    // Add code here
    write(soc, &num, sizeof(int));

    /* Open the file and read each line using fgets, and write
     * each line to the server using write.  Do not null terminate
     * each string that is written, or remove the newline characters.
     */
    fp = fopen(argv[2], "r");
    // Add code here
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        write(soc, buf, strlen(buf)) ;
    }

    close(soc);
    return (0);
}
