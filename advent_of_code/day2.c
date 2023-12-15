#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* colors[] = {"red", "green", "blue"};
int max_colors[] = {12, 13, 14};

int find_char(char str[], char c) {
	int i = 0;
	while (str[i] != '\0') {
		if (str[i] == c) {
			return i;
		}
		i++;
	}
	return -1;
}

int count_char(char* str, char c) {
	int i = 0;
	int sum = 0;

	while (str[i] != '\0') {
		if (str[i] == c) {
			sum++;
		}
		i++;
	}
	return sum;
}

int get_index(char str[]) {
	int start = find_char(str, ' ') + 1;
	int end = find_char(str, ':');
	int len = end - start;
	
	char* index = malloc(len+1);
	//printf("%s", str);
	memcpy(index, &str[start], len);
	index[len] = 0;

	int res = atoi(index);
	free(index);
	return res;
}


char* get_token(char* str, int strlen, char delimiter, int start) {
	char* res = NULL;

	for (int i = start; i < strlen; i++) {
		if (str[i] == delimiter || i == strlen - 1) {
			int size = i == strlen - 1 ? i - start + 1 : i - start;
			res = malloc(size + 1);

			strncpy_s(res, size + 1, &str[start], size);
			//printf("token: %s len: %d %d\n", res, strlen, size);
			return res;
		}
	}
	return res;
}

char** split_string(char* str, int str_len, int* num_token, char delimiter) {
		
	int token_count = count_char(str, delimiter) + 1;
	*num_token = token_count;

	char** res = malloc(token_count * sizeof(char*));

	int start = 0;

	char* token = NULL;
	for (int i = 0; i < token_count; i++) {
		token = get_token(str, str_len, delimiter, start);
		res[i] = token;
		start = start + strlen(token) + 1;
	}
	return res;
}

int is_color(char* color) {
	for (int i = 0; i < 3; i++) {
		if (strcmp(color, colors[i]) == 0) {
			return i;
		}
	}
	return -1;
}


void day2(FILE* fp) {
	int sum = 0;
	int sum_power = 0;

	char buffer[256];
	while (fgets(buffer, sizeof buffer, fp) != NULL) {
		int index = get_index(buffer);
		printf("index: %d\n", index);
		//char* token = get_token(buffer, ';', 0);
		//printf("token: %s\n", token);
		//free(token);
		int begin = find_char(buffer, ':') + 1;

		int token_count = 0;
		char** line = split_string(&buffer[begin], strlen(&buffer[begin]) - 1, &token_count, ';');

		int add_game = 1;
		int min_cubes[3] = { 0, 0, 0 };

		for (int i = 0; i < token_count; i++) {

			int cubes_len = 0;
			char** cubes = split_string(line[i], strlen(line[i]), &cubes_len, ',');
			//printf("cubes_len: %d\n", cubes_len);
			for (int j = 0; j < cubes_len; j++) {
				int cube_len = 0;
				char** cube = split_string(&cubes[j][1], strlen(cubes[j])-1, &cube_len, ' ');
				//printf("#: %s, color: %s\n", cube[0], cube[1]);
				int cube_count = atoi(cube[0]);
				int color = is_color(cube[1]);
				printf("#: %d, color: %d\n", cube_count, color);
				min_cubes[color] = min_cubes[color] < cube_count ? cube_count : min_cubes[color];
				
				if (cube_count > max_colors[color]) {
					//printf("game %d is an invalid game\n", i);
					add_game = 0;
				}
			}
		}
		sum_power = sum_power + min_cubes[0] * min_cubes[1] * min_cubes[2];
		memset(min_cubes, 0, sizeof(min_cubes));

		if (add_game) sum = sum + index;

		for (int i = 0; i < token_count; i++) {
			free(line[i]);
		}
		free(line);
	}
	printf("sum of powers: %d\n", sum_power);
	printf("sum: %d\n", sum);
}