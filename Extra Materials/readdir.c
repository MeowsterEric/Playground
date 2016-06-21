#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {

    DIR *dp;
    struct dirent *p;

    if ((dp = opendir(".")) == NULL) {
        perror(".");
        exit(1);
    }

    while ((p = readdir(dp)))
        printf("%s\n", p->d_name);

    closedir(dp);

    return (0);
}
