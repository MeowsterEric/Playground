#define PATHLENGTH 128
#define MAXRECORDS 100

// This data structure is used by the workers to prepare the output
// to be sent to the master process.

typedef struct {
	int freq;
	char filename[PATHLENGTH];
} FreqRecord;

FreqRecord *get_word(const char *word, Node **head, char **filenames);
void print_freq_records(FreqRecord *frp);
void run_worker(char *dirname, int in, int out);

FreqRecord *allocate_freq_record(int size);
void write_to_out(FreqRecord *records, int out);
void write_to_stdout(FreqRecord *records, int out);
void get_frequencies(Node *node, int **freq_list, int *non_zero_counter);
void sort_records(FreqRecord **frp);
int comparator(const void *fr1, const void *fr2);
