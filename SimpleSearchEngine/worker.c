#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"


/* The function get_word should be added to this file */
/*
 * The function looks up a given word in an word index linked list, and
 * return an array of FreqRecords for this word in the index.
 * Each individual FreqRecord provides the number of occurrences of the word
 * for a particular file.
 */
FreqRecord *get_word(const char *word, Node **word_list_ptr, char **filenames) {
    Node *word_node = *word_list_ptr;

    // loop through the linked list to find the node with the word '*word'
    while (word_node != NULL && (strcmp(word, word_node->word) != 0)) {
        word_node = word_node->next;
    }

    int *frequency_list = NULL;
    int non_zero_counter = 0;
    get_frequencies(word_node, &frequency_list, &non_zero_counter);

    // allocate memory 'non_zero_counter' number of memory for the non-zero
    // element, add additional 1 block for the sentinel marker
    FreqRecord *records = allocate_freq_record(non_zero_counter + 1);

    int i_ff = 0; // the index of both frequency_list and filenames array
    int i_record = 0; // the index of FreqRecord array
    // when word node exists, loop through the frequency list and filenames
    // array, if the frequency list at a specific index is not 0, copy the
    // frequency value and filename to the FreqRecord
    while (word_node != NULL && filenames[i_ff] != NULL && i_ff < MAXFILES) {

        // if the frequency_list at index i has a non-zero value, then copy
        // the value to one FreqRecord space with this value and file path
        if (frequency_list[i_ff] > 0) {
            records[i_record].freq = frequency_list[i_ff];
            // if the length of file_names[i] is less than n, then strncpy()
            // writes additional '\0' to freq_records[i_record]->filename
            // to ensure a total n bytes are written.
            strncpy(records[i_record].filename, filenames[i_ff], PATHLENGTH);
            records[i_record].filename[PATHLENGTH - 1] = '\0';
            // increament the FreqRecord array counter
            i_record++;
        }
        // increament the file counter
        i_ff++;
    }

    /*** use this block to set the sentinel marker ***/
    // if the word_node does not exist, or the loop terminate,
    // the 'non_zero_counter' index is the last block in reserved memory
    records[non_zero_counter].freq = 0;
    records[non_zero_counter].filename[0] = '\0';

    return records;
}

/*
 * run_worker
 * - load the index found in dirname
 * - read a word from the file descriptor "in"
 * - find the word in the index list
 * - write the frequency records to the file descriptor "out"
 */
void run_worker(char *dirname, int in, int out) {

    // concatenate the dirname with index file to get the listfile path
    char listfile[strlen(dirname) + strlen("/index") + 1];
    strcpy(listfile, dirname);
    strncat(listfile, "/index", strlen("/index") + 1);
    // concatenate the dirname with filenames text file to get namefile path
    char namefile[strlen(dirname) + strlen("/filenames") + 1];
    strcpy(namefile, dirname);
    strncat(namefile, "/filenames", strlen("/filenames") + 1);

    // get the filenames array
    char **filenames = init_filenames();

    // read the index file, get the word node linked list
    Node *head = NULL;
    read_list(listfile, namefile, &head, filenames);

    int r_byte;
    // set the MAXWORD amount of null space for the word array,
    char word[MAXWORD + 1] = { '\0' };
    // =====================================================
    // ========== This chunk is used for queryone ==========
    // =====================================================
    if (in == STDIN_FILENO && out == STDOUT_FILENO) {
        // read the word from stdin, note that each time the ENTER is hit, a '\n'
        // character will be added to the end of the array
        FreqRecord *records;
        printf("Type a word to search in directory [%s]:\n", dirname);
        while ((r_byte = read(in, word, MAXWORD + 1)) > 0) {
            // trim the newline character
            word[strlen(word) - 1] = '\0';
            // get frequency record of this word, then sort the record array
            // and write to stdout in human readable format
            records = get_word(word, &head, filenames);
            sort_records(&records);
            write_to_stdout(records, out);
            // reset word array and free the memory for next iteration
            memset(word, '\0', MAXWORD + 1);
            free(records);

            printf("Type another word: \n");
        }

        if (r_byte == 0) {
            printf("Search words from directory [%s] done.\n\n", dirname);
        }
    }
    // =====================================================
    // ========== This chunk is used for queryone ==========
    // =====================================================
    else {
        // put into a loop to force the child process to run continuously
        while (1) {
            if ((r_byte = read(in, word, MAXWORD + 1)) < 0) {
                perror("Run worker read word failed.");
                exit(EXIT_FAILURE);
            }
            else if (r_byte == 0) {
                // close both in and out file descriptor and exit
                close(in);
                close(out);
                exit(EXIT_SUCCESS );
            }
            else {
                // set null byte to the last character, then get the records
                // and write the records to out
                word[MAXWORD] = '\0';
                FreqRecord *records = get_word(word, &head, filenames);
                write_to_out(records, out);
                // once the records are written to fd out, free the pointer
                // and reset word array and free the memory for next iteration
                free(records);
                memset(word, '\0', MAXWORD + 1);
            }
        }
    }
}

FreqRecord *allocate_freq_record(int size) {
    FreqRecord *records = malloc(sizeof(FreqRecord) * size);
    if (records == NULL) {
        perror("malloc FreqRecord failed.");
        exit(EXIT_FAILURE);
    }
    // initialize the FreqRecord frequency to 0, filename to null char
    int i;
    for (i = 0; i < size; i++) {
        records[i].freq = 0;
        records[i].filename[0] = '\0';
    }
    return records;
}

/*
 * Print to standard output the frequency records for a word.
 * Used for testing.
 */
void print_freq_records(FreqRecord *frp) {
    int i = 0;
    while (frp != NULL && frp[i].freq != 0) {
        printf("%d    %s\n", frp[i].freq, frp[i].filename);
        i++;
    }
}

/*
 * A helper function to write the frequent records to out.
 */
void write_to_out(FreqRecord *records, int out) {
    // loop through each non-zero record, write to fd out
    int i = 0;
    while (records[i].freq != 0) {
        if (write(out, &records[i], sizeof(FreqRecord)) != sizeof(FreqRecord)) {
            perror("Run worker write FreqRecord to out failed.");
            exit(EXIT_FAILURE);
        }
        i ++;
    }
    // write the sentinel marker to out, indicate the end of the array
    if (write(out, &records[i], sizeof(FreqRecord)) != sizeof(FreqRecord)) {
        perror("Run worker write FreqRecord sentinel marker to out failed.");
        exit(EXIT_FAILURE);
    }
}

/*
 * A helper function to write the frequent records to stdout
 * in a human readable format.
 */
void write_to_stdout(FreqRecord *records, int out) {
    char *str = " occurrences in file ";
    int i = 0;
    while (records[i].freq != 0) {
        // process the output string to stdout
        char output[MAXWORD + 22] = { '\0' };
        sprintf(output, "%d", records[i].freq);
        strncat(output, str, strlen(str));
        strncat(output, records[i].filename, strlen(records[i].filename));
        strncat(output, "\n", strlen("\n"));
        output[MAXWORD + 21] = '\0'; // last index
        // write to the stdout
        if (write(out, output, sizeof(output)) != sizeof(output)) {
            perror("Run worker write output string to stdout failed.");
            exit(EXIT_FAILURE);
        }
        i++;
    }
}

/*
 * A helper function to get frequency list and number of non-zero elements
 */
void get_frequencies(Node *node, int **freq_list, int *counter) {
    // if the the node exist, then retrieve the frequency list
    if (node != NULL) {
        *freq_list = node->freq;
        // loop through each number in the frequency list,
        int i;
        for (i = 0; i < MAXFILES; i++) {
            // update counter if non-zero value present
            node->freq[i] > 0 ? ((*counter)++) : ((*counter) += 0);
        }
    }
}

/*
 * A helper function to sort the FreqRecord array
 */
void sort_records(FreqRecord **frp) {
    FreqRecord *records = *frp;
    int i = 0;
    int valid_records = 0;
    while (records[i].freq != 0) {
        valid_records ++;
        i ++;
    }
    // use qsort to sort the struct, +1 for sentinel marker
    qsort(records, valid_records + 1, sizeof(FreqRecord), comparator);
}

int comparator(const void *fr1, const void *fr2) {
    // the comparator function use to sort the array
    return ((FreqRecord *)fr2)->freq - ((FreqRecord *)fr1)->freq;
}
