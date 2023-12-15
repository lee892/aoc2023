
#include "sources.h"

int grid_size = 0;

int line_size = 0;

int gears[MAX_LINES][MAX_LINE];
int gear_ratios[MAX_LINES][MAX_LINE];

int is_num(char c) {
	if ((int)'0' <= (int)c && (int)c <= (int)'9') {
		return 1;
	}
	return 0;
}

int is_symbol(char c) {
	if (((int)c < (int)'0' || (int)'9' < (int)c) && (int)c != (int)'.') {
		return 1;
	}
	return 0;
}

int is_alphabet(char c) {
	if ((int)'a' <= (int)c && (int)c <= (int)'z' ||
		(int)'A' <= (int)c && (int)c <= (int)'Z') {
		return 1;
	}
	return 0;
}

int get_num(int* n, char* line, int i) {
	int num_length = 0;
	int index = i;
	while (line[index] != '\0' && is_num(line[index])) {
		num_length++;
		index++;
	}
	*n = atoi(&line[i]);
	return num_length;
}

int get_word_length(char* line, int i) {
	int len = 0;
	int index = i;
	while (line[index] != '\0' && is_alphabet(line[index])) {
		len++;
		index++;
	}
	return len;
}

int get_llu(llu* n, char* line, int i) {
	int num_length = 0;
	int index = i;
	while (line[index] != '\0' && is_num(line[index])) {
		num_length++;
		index++;
	}
	*n = strtoull(&line[i], NULL, 10);
	return num_length;
}

int get_float(float* n, char* line, int i) {
	int num_length = 0;
	int index = i;
	while (line[index] != '\0' && is_num(line[index])) {
		num_length++;
		index++;
	}
	*n = atof(&line[i]);
	return num_length;
}


int in_grid(int i, int j) {
	return (i >= 0 && i < grid_size && j >= 0 && j < line_size);
}

void set_gear(int i, int j, int num) {
	gears[i][j] += 1;
	gear_ratios[i][j] =
		gear_ratios[i][j] == 0 ?
		num : gear_ratios[i][j] * num;
}

int is_part_num(int i, int j, int num, int num_length) {
	for (int x = j - 1; x < j + num_length + 1; x++) {
		if (in_grid(i - 1, x) && is_symbol(grid[i - 1][x])) {
			if ((int)grid[i-1][x] == (int)'*') {
				set_gear(i - 1, x, num);
			}
			return 1;
		}
		if (in_grid(i + 1, x) && is_symbol(grid[i + 1][x])) {
			if ((int)grid[i + 1][x] == (int)'*') {
				set_gear(i + 1, x, num);
			}
			return 1;
		}
	}

	//if (in_grid(i, j - 1) && is_symbol(grid[i][j - 1])) {
	//	printf("%cleft ", grid[i][j - 1]);
	//}
	//if (in_grid(i, j + 1)) {
//		printf("(%c) ", grid[i][j + 1]);
//	}
	if (in_grid(i, j - 1) && is_symbol(grid[i][j - 1])) {
		if ((int)grid[i][j-1] == (int)'*') {
			set_gear(i, j-1, num);
		}
		return 1;
	}

	if (in_grid(i, j + num_length) && is_symbol(grid[i][j + num_length])) {
		if ((int)grid[i][j + num_length] == (int)'*') {
			set_gear(i, j + num_length, num);
		}

		return 1;
	}
	return 0;
}

int is_gear(int i, int j) {
	if ((int)grid[i][j] != (int)'*') {
		return 0;
	}
	int adj_nums = 0;

	for (int x = j - 1; x <= j + 1; x++) {
		if (in_grid(i - 1, x) && is_num(grid[i - 1][x])) {
			return 1;
		}
		if (in_grid(i + 1, x) && is_num(grid[i + 1][x])) {
			return 1;
		}
	}
	return 1;
}

void day3(FILE* fp) {
	int sum = 0;
	int gear_ratio_sum = 0;

	if (fgets(line, MAX_LINE, fp) != NULL) {
		line_size = strlen(line) - 1;
		line[line_size] = '\0';
		grid[0] = malloc((line_size + 1) * sizeof(char));
		strcpy_s(grid[0], line_size + 1, line);
		grid_size++;
	} else {
		printf("Failed to read line from file\n");
		exit(1);
	}

	while (fgets(line, MAX_LINE, fp) != NULL) {
		line[line_size] = '\0';
		grid[grid_size] = malloc((line_size + 1) * sizeof(char));
		strcpy_s(grid[grid_size], line_size + 1, line);
		grid_size++;
	}

	int row = 1;
	for (int i = 0; i < grid_size; i++) {
		//printf("%d: ", row);
		row++;
		for (int j = 0; j < line_size; j++) {
			if (is_num(grid[i][j])) {
				int num = 0;
				int num_length = get_num(&num, grid[i], j);
				//printf("num %d: %d", num, is_symbol(grid[i][j]));
				if (is_part_num(i, j, num, num_length)) {
					sum = sum + num;
					//printf("%d ", num);
				}

				j += num_length-1;
			}
			//printf("%c: %d ", grid[i][j], is_symbol(grid[i][j]));
		}
		//printf("\n");
	}
	printf("sum: %d\n", sum);
	row = 1;
	for (int i = 0; i < grid_size; i++) {
		printf("%d: ", row);
		row++;
		for (int j = 0; j < line_size; j++) {
			if (gears[i][j] == 2) {
				gear_ratio_sum = gear_ratio_sum + gear_ratios[i][j];
				printf("%d-%d ", gears[i][j], gear_ratios[i][j]);
			}
		}
		printf("\n");
		free(grid[i]);
	}
	printf("gear ratio sum: %d\n", gear_ratio_sum);
}