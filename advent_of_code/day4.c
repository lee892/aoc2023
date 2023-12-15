#include "sources.h"

#define WIN_NUM_COUNT 10

int rows = 0;
int line_width = 0;

int row_size = 0;

int start = 0;
int pipe_index = 0;


int winning_nums[WIN_NUM_COUNT];
int winning_nums_index = 0;

int card_counts[MAX_LINES];

int add_num(int i, int j) {
	winning_nums[winning_nums_index] = atoi(&grid[i][j]);
	printf("%d ", winning_nums[winning_nums_index]);
	winning_nums_index++;
	assert(winning_nums_index <= 10);
	int index = j+1;
	while (index < row_size && is_num(grid[i][index])) {
		index++;
	}
	return index;
}

int is_winning(int num) {
	for (int i = 0; i < WIN_NUM_COUNT; i++) {
		if (winning_nums[i] == num) {
			return 1;
		}
	}
	return 0;
}

void day4(FILE* fp) {
	if (fgets(line, MAX_LINE, fp) != NULL) {
		line_width = strlen(line) - 1;
		line[line_width] = '\0';

		start = find_char(line, ':') + 2;
		row_size = line_width - start;
		grid[0] = malloc((row_size + 1) * sizeof(char));
		strcpy_s(grid[0], row_size + 1, &line[start]);
		for (int j = 0; j < row_size; j++) {
			if (grid[0][j] == '|') {
				pipe_index = j;
			}
		}
		
		card_counts[rows] = 1;
		rows++;
	} else {
		printf("Failed to read line from file\n");
		exit(1);
	}

	while (fgets(line, MAX_LINE, fp) != NULL) {
		line[line_width] = '\0';
		grid[rows] = malloc((row_size + 1) * sizeof(char));
		strcpy_s(grid[rows], row_size + 1, &line[start]);

		card_counts[rows] = 1;
		rows++;
	}


	int sum = 0;

	for (int i = 0; i < rows; i++) {
		int j = 0;
		while (j < row_size && j != pipe_index) {
			if (is_num(grid[i][j])) {
				j = add_num(i, j);
			}
			else {
				j++;
			}
			printf("\n");
		}
		int points = 0;
		int cards_won = 0;
		winning_nums_index = 0;
		for (int j = pipe_index + 1; j < row_size; j++) {
			printf("%c", grid[i][j]);
			if (is_num(grid[i][j])) {
				int curr_num = 0;
				int len = get_num(&curr_num, grid[i], j);
				if (is_winning(curr_num)) {
					printf("*");
					points = points == 0 ? 1 : points * 2;
					cards_won++;
				}
				j = j + len - 1;
			}
		}
		sum = sum + points;
		printf("\n");
		printf("points: %d\n", cards_won);
		printf("card count: %d\n", card_counts[i]);
		for (int k = 0; k < card_counts[i]; k++) {
			for (int j = i + 1; j < rows && j < i + cards_won + 1; j++) {
				card_counts[j]++;
			}
		}

	}
	printf("sum: %d\n", sum);
	int sum2 = 0;
	for (int i = 0; i < rows; i++) {
		sum2 = sum2 + card_counts[i];
		free(grid[i]);
	}
	printf("sum2: %d\n", sum2);
}