/*
 * Note: It's not very clear what the arguments to perror() should be in these
 * cases, but I'm not very happy with my choices below.  More thought required.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int main() {

    struct stat statbuf;
    ino_t childdir;
    DIR *dp;
    struct dirent *r;

    while (1) {
        if (lstat(".", &statbuf)) {
            perror("lstat(\".\")");
            exit(1);
        }

        if (statbuf.st_ino == 2)
            break;

        childdir = statbuf.st_ino;

        if (chdir("..")) {
            perror("chdir(\"..\")");
            exit(1);
        }

        if ((dp = opendir(".")) == NULL) {
            perror("opendir(\"..\")");
            exit(1);
        }

        while ((r = readdir(dp)) && r->d_ino != childdir);

        if (!r) {
            fprintf(stderr, "Can't find %ld in parent!\n", (long)childdir);
            exit(1);
        }

        printf("%s\n", r->d_name);
        closedir(dp);
    }

    printf("and then we're at the root directory.\n");
    return 0;
}
