#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "freq_list.h"
#include "worker.h"


int main(int argc, char **argv) {
    char ch;
    char path[PATHLENGTH];
    char *startdir = ".";

    while ((ch = getopt(argc, argv, "d:")) != -1) {
        switch (ch) {
        case 'd':
            startdir = optarg;
            break;
        default:
            fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME]\n");
            exit(1);
        }
    }

    // Open the directory provided by the user (or current working directory)
    DIR *dirp;
    if ((dirp = opendir(startdir)) == NULL) {
        perror("opendir");
        exit(1);
    }

    // copy the initialization from queryone.c
    // initialize a master record array, add additional 1 block for the
    // sentinel marker. Add a counter for number of elements in array
    int m_counter = 0;
    FreqRecord *master_records = allocate_freq_record(MAXRECORDS + 1);

    int dir_counter = 0;
    // create MAXFILES number of file descriptors, each directory has 2 pipes,
    // each pipe has 2 entrence, set 0 for read, 1 for write
    int pipe_fd[MAXFILES][2][2];
    // set up parent pid for tracking
    pid_t pid_parent = getpid();

    struct dirent *dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0 ||
                strcmp(dp->d_name, ".svn") == 0) {
            continue;
        }
        strncpy(path, startdir, PATHLENGTH);
        strncat(path, "/", PATHLENGTH - strlen(path) - 1);
        strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);

        struct stat sbuf;
        if (stat(path, &sbuf) == -1) {
            // This should only fail if we got the path wrong
            // or we don't have permissions on this entry.
            perror("stat");
            exit(1);
        }

        // ========== pipe work start here ========== //
        // Only call run_worker if it is a directory. Otherwise ignore it.
        if (S_ISDIR(sbuf.st_mode)) {
            dir_counter ++;
            // create 2 pipes for this directory (0 - read, 1 - write)
            // pipe0: parent write to child/child read from parent pipe
            pipe(pipe_fd[dir_counter - 1][0]);
            // pipe1: parent read from child/child write to parent pipe
            pipe(pipe_fd[dir_counter - 1][1]);

            // fork the process, call run_worker in child process
            int pid;
            if ((pid = fork()) < 0) {
                perror("fork child process failed.");
                exit(EXIT_FAILURE);
            }
            // actions in child process
            else if (pid == 0) {
                // child read from parent process, thus the parent write pipe
                // is the input of the child process; and parent read pipe is
                // the output of the child process

                // close pipe0 write entrance and pipe1 read entrance
                close(pipe_fd[dir_counter - 1][0][1]);
                close(pipe_fd[dir_counter - 1][1][0]);
                // call run_worker in child process
                run_worker(path, pipe_fd[dir_counter - 1][0][0], pipe_fd[dir_counter - 1][1][1]);
                return 0;
            }
        }
    }

    // if this is the parent process, run the following code.
    if (getpid() == pid_parent) {

        while (1) {
            // prompt user to enter a word from stdin
            int read_byte;
            char src_word[MAXWORD + 1] = { '\0' };
            printf("Type a word to search:\n");
            if ((read_byte = read(STDIN_FILENO, src_word, MAXWORD + 1)) < 0) {
                perror("Parent process: read from stdin failed.");
                exit(EXIT_FAILURE);
            }
            // if the user terminate the process (^D), read_byte reads 0
            else if (read_byte == 0) {
                int i_dir;
                for (i_dir = 0; i_dir < dir_counter; i_dir++) {
                    // close the 2 pipes for each directory, close read pipe
                    // read/in entrance and write pipe write/out entrance
                    close(pipe_fd[i_dir][0][1]);
                    close(pipe_fd[i_dir][1][0]);
                }
                // wait for all child process done
                for (i_dir = 0; i_dir < dir_counter; i_dir++)
                    wait(NULL);

                // print out the finish line
                printf("Search engine terminated. Thank you.\n");
                exit(EXIT_SUCCESS);
            }
            // read a word from stdin, read_byte > 0
            else {
                // trim the newline character in the src_word
                src_word[strlen(src_word) - 1] = '\0';

                int i_dir;
                for (i_dir = 0; i_dir < dir_counter; i_dir++) {
                    // close the 2 pipes for each directory in parent process,
                    // close pipe0 read entrance and pipe1 write entrance
                    close(pipe_fd[i_dir][0][0]);
                    close(pipe_fd[i_dir][1][1]);
                    // parent process write the src_word to all child process
                    if (write(pipe_fd[i_dir][0][1], src_word, sizeof(src_word)) != sizeof(src_word)) {
                        perror("Parent process write to child process failed.");
                        exit(EXIT_FAILURE);
                    }

                    // allocate one record space for read from child process
                    FreqRecord *record = allocate_freq_record(1);
                    do {
                        // read one record from child process
                        if (read(pipe_fd[i_dir][1][0], record, sizeof(FreqRecord)) < 0) {
                            perror("Parent process read from child process failed.");
                            exit(EXIT_FAILURE);
                        }
                        // the read record has non-zero frequency
                        if (record->freq != 0) {
                            // fill the master array to each empty space
                            if (m_counter < MAXRECORDS) {
                                master_records[m_counter] = *record;
                                m_counter ++;
                            }
                            else {
                                // if the master array spaces are all filled,
                                // then compare the frequency of the last record
                                // and read out record, replace the last record
                                // if the master record is less than read out record
                                if (master_records[MAXRECORDS - 1].freq < record->freq)
                                    master_records[MAXRECORDS - 1] = *record;
                            }
                            // no efficiency check, sort after every insert
                            sort_records(&master_records);
                        }
                    } while (record->freq);
                    // free the read_record pointer
                    free(record);
                }
                // print the master frequency record
                print_freq_records(master_records);
            }
        }
    }
    return 0;
}
