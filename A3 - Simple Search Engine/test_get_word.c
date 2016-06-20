#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "freq_list.h"
#include "worker.h"


int main() {

	Node *head = NULL;
	char *listfile = "index";
	char *namefile = "filenames";
	char **filenames = init_filenames();

	read_list(listfile, namefile, &head, filenames);
	FreqRecord *fr = get_word("light", &head, filenames);
	sort_records(&fr);
	print_freq_records(fr);

	return 0;
}
