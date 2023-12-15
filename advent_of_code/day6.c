#include "sources.h"

#define MAX_TIMES 50
#define MAX_DISTANCES 50

int times[MAX_TIMES];
int race_count = 0;
int records[MAX_DISTANCES];
uint64_t combined_time = 0;
uint64_t combined_distance = 0;

void get_nums(FILE* fp, int arr[], uint64_t* combined_data) {
	if (fgets(line, MAX_LINE, fp) != NULL) {
		int index = find_char(line, ':') + 1;
		int len = 0;
		int num = 0;
		int combined_len = 0;
		while (line[index] != '\0') {
			if (is_num(line[index])) {
				len = get_num(&num, line, index);
				arr[race_count] = num;
				race_count++;
				index = index + len;
			} else {
				index++;
			}
		}
		char buffer[64];
		int i = 0;
		index = find_char(line, ':') + 1;
		while (line[index] != '\0') {
			if (is_num(line[index])) {
				buffer[i] = line[index];
				i++;
			}
			index++;
		}
		buffer[i] = '\0';
		*combined_data = strtoll(buffer, NULL, 10);
	} else {
		printf("Failed to parse line\n");
	}
}

void day6(FILE* fp) {
	get_nums(fp, times, &combined_time);
	race_count = 0;
	get_nums(fp, records, &combined_distance);

	printf("combined time, distance: %zu, %zu\n", combined_time, combined_distance);

	int time_button_held = 0;
	int distance = 0;
	int ways_to_win = 0;
	int solution = 0;
	for (int i = 0; i < race_count; i++) {
		printf("%d: %d, ", times[i], records[i]);
		time_button_held = 0;
		distance = 0;
		ways_to_win = 0;
		while (time_button_held <= times[i]) {
			distance = time_button_held * (times[i] - time_button_held);
			if (distance > records[i]) {
				ways_to_win++;
			}
			time_button_held++;
		}
		printf("ways to win: %d\n", ways_to_win);
		solution = solution == 0 ? ways_to_win : solution * ways_to_win;

	}
	printf("solution 1: %d\n", solution);
	uint64_t dist = 0;
	ways_to_win = 0;
	uint64_t time_held = 0;
	while (time_held <= combined_time) {
		dist = time_held * (combined_time - time_held);
		if (dist > combined_distance) {
			ways_to_win++;
		}
		time_held++;
	}
	printf("solution 2: %d", ways_to_win);
}