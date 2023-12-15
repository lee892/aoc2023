#include "sources.h"
#include "queue.h"

#define MAX_SEED_RANGES 64
#define MAX_SEEDS 64
#define MAX_MAPPINGS 32
#define MAX_MAP_ITEMS 64
#define MAP_ITEM_COLS 3


llu seeds[MAX_SEEDS];
int seed_count = 0;

llu seed_ranges[MAX_SEED_RANGES][2];
int seed_range_count = 0;

llu mappings[MAX_MAPPINGS][MAX_MAP_ITEMS][MAP_ITEM_COLS];

int map_item_counts[MAX_MAPPINGS];
int mappings_index = -1;

int in_range(llu num, llu start, llu end) {
	if (start <= num && num < end) {
		return 1;
	}
	return 0;
}

void apply_range(Queue* q, int map_index) {
	Queue intersections;
	init_queue(&intersections);

	llu before[2];
	llu between[2];
	llu after[2];

	for (int j = 0; j < map_item_counts[map_index]; j++) {
		Queue nextQ;
		init_queue(&nextQ);
		
		llu dest = mappings[map_index][j][0];
		llu src = mappings[map_index][j][1];
		llu src_end = src + mappings[map_index][j][2];
		
		while (!empty_queue(q)) {
			llu start = pop_queue(q);
			llu end = pop_queue(q);
			
			//printf("range: %llu %llu, %llu %llu %llu\n", start, end, dest, src, src_end);
			before[0] = start;
			before[1] = MIN(end, src);
			//printf("end, src_end: %llu %llu\n", end, src_end);
			between[0] = MAX(start, src);
			between[1] = MIN(end, src_end);
			after[0] = MAX(start, src_end);
			after[1] = end;
			//printf("%llu %llu\n", between[0], between[1]);
			
			if (before[0] < before[1]) {
				//printf("pushing before: %llu %llu\n", before[0], before[1]);
				push_queue(&nextQ, before[0]);
				push_queue(&nextQ, before[1]);
			}
			if (between[0] < between[1]) {
				between[0] = between[0] - src + dest;
				between[1] = between[1] - src + dest;
				//printf("push between: %llu %llu\n", between[0], between[1]);
				push_queue(&intersections, between[0]);
				push_queue(&intersections, between[1]);
			}
			if (after[0] < after[1]) {
				//printf("pushing after: %llu %llu\n", after[0], after[1]);
				push_queue(&nextQ, after[0]);
				push_queue(&nextQ, after[1]);
			}
		}
		concat_queue(q, &nextQ);
		destroy_queue(&nextQ);
	}
	concat_queue(q, &intersections);
	destroy_queue(&intersections);
}

llu apply_mappings(llu item, int map_index) {
	for (int i = 0; i < map_item_counts[map_index]; i++) {
		llu src = mappings[map_index][i][1];
		llu end = src + mappings[map_index][i][2];
		if (in_range(item, src, end)) {
			llu diff = item - src;
			printf("|%llu %llu %llu\n", item, src, mappings[map_index][i][0] + diff);
			return mappings[map_index][i][0] + diff;
		}
	}
	return item;
}

void day5(FILE* fp) {
	if (fgets(line, MAX_LINE, fp) != NULL) {
		int i = 0;
		while (line[i] != '\0') {
			if (is_num(line[i])) {
				llu num = 0;
				int len = get_llu(&num, line, i);
				printf("%llu ", num);
				seeds[seed_count] = num;
				seed_count++;
				i = i + len;
			} else {
				i++;
			}
		}
		printf("\n");
		i = 0;
		while (line[i] != '\0') {
			if (is_num(line[i])) {
				llu num = 0;
				int len = get_llu(&num, line, i);
				llu count = 0;
				i = i + len + 1;
				len = get_llu(&count, line, i);
				seed_ranges[seed_range_count][0] = num;
				seed_ranges[seed_range_count][1] = num + count;
				seed_range_count++;
				i = i + len;
			} else {
				i++;
			}
		}
	} else {
		printf("Failed to read line from file\n");
		exit(1);
	}
	printf("\n");
	for (int i = 0; i < seed_range_count; i++) {
		printf("|%llu %llu| ", seed_ranges[i][0], seed_ranges[i][1]);
	}
	printf("\n");

	int map_item_index = 0;
	while (fgets(line, MAX_LINE, fp) != NULL) {
		if (find_char(line, ':') != -1) {
			map_item_counts[mappings_index] = map_item_index;
			mappings_index++;
			map_item_index = 0;
			continue;
		}
		if (!is_num(line[0])) {
			continue;
		}

		int i = 0;
		int map_item_col = 0;
		while (line[i] != '\0') {
			if (is_num(line[i])) {
				llu num = 0;
				int len = get_llu(&num, line, i);
				mappings[mappings_index][map_item_index][map_item_col] = num;
				i = i + len;
				assert(map_item_col <= MAP_ITEM_COLS);
				map_item_col++;
			} else {
				i++;
			}
		}
		map_item_counts[mappings_index] = map_item_index;
		map_item_index++;
	}
	map_item_counts[mappings_index] = map_item_index;

	for (int i = 0; i < mappings_index + 1; i++) {
		printf("%d ", map_item_counts[i]);
	}
	printf("\n");
	
	Queue q;
	init_queue(&q);

	for (int i = 0; i < mappings_index + 1; i++) {
		for (int j = 0; j < seed_count; j++) {
			seeds[j] = apply_mappings(seeds[j], i);
		}

		for (int j = 0; j < map_item_counts[i]; j++) {
			for (int k = 0; k < MAP_ITEM_COLS; k++) {
				printf("%llu ", mappings[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	llu min_range = seed_ranges[0][0];

	for (int j = 0; j < seed_range_count; j++) {
		printf("adding range: % llu % llu\n", seed_ranges[j][0], seed_ranges[j][1]);
		push_queue(&q, seed_ranges[j][0]);
		push_queue(&q, seed_ranges[j][1]);
		for (int i = 0; i < mappings_index + 1; i++) {
			apply_range(&q, i);
			int k = 0;
			printf("size %d ", q.size);
			printf("queue vals: ");
			while (k < q.size) {
				llu start = peek_index_queue(&q, k);
				k++;
				llu end = peek_index_queue(&q, k);
				printf("|%llu %llu| ", start, end);
				k++;
			}
			printf("\n");

		}
		while (!empty_queue(&q)) {
			llu start = pop_queue(&q);
			llu end = pop_queue(&q);
			min_range = MIN(min_range, start);
		}
		printf("min_range of range %llu %llu: %llu\n", seed_ranges[j][0], seed_ranges[j][1], min_range);
	}
	printf("min from range: %llu\n", min_range);

	printf("final seed mappings: ");
	llu min = seeds[0];
	for (int i = 0; i < seed_count; i++) {
		printf("%llu ", seeds[i]);
		min = seeds[i] < min ? seeds[i] : min;
	}
	printf("\n");
	printf("min from single: %llu\n", min);
}