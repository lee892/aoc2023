#include "sources.h"

#define MAX_INSTRUCTIONS 1024
#define MAX_BNODES 1024

char instructions[MAX_INSTRUCTIONS];
size_t instruction_len;

char* node_children[MAX_BNODES][2];

void day8(FILE* fp) {

	if (fgets(line, MAX_LINE, fp) != NULL) {
		instruction_len = strlen(line);
		strcpy_s(instructions, instruction_len + 1, line);
	}

	for (int i = 0; i < instruction_len; i++) {
		printf("%c ", instructions[i]);
	}
	printf("\n");

	int word_len = 0;
	while (fgets(line, MAX_LINE, fp) != NULL) {
		word_len = get_word_length(line, 0);
	}
}