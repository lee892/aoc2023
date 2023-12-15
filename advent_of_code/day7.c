#include "sources.h"

#define CARD_COUNT 5
#define NUM_CARDS 13
#define MAX_HANDS 1024

enum HandType {
	HIGHCARD,
	ONEPAIR,
	TWOPAIR,
	THREEOFKIND,
	FULLHOUSE,
	FOUROFKIND,
	FIVEOFKIND
}; 

typedef struct {
	char k;
} Key;

typedef struct {
	Key key;
} CardMap;

typedef struct {
	char hand[CARD_COUNT + 1];
	int card_counts[NUM_CARDS];
	int bid_amount;
	int type;
	int wc_type;
} Hand;


int get_card_strength(char c) {
	switch (c) {
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'T':
			return 10;
		case 'J':
			return 11;
		case 'Q':
			return 12;
		case 'K':
			return 13;
		case 'A':
			return 14;
	}
}

int get_wc_strength(char c) {
	if (c == 'J') {
		return 1;
	}
	return get_card_strength(c);
}

void init_card_counts(Hand* hand) {
	for (int i = 0; i < NUM_CARDS; i++) {
		hand->card_counts[i] = 0;
	}
}

void set_card_counts(Hand* hand) {
	int key = 0;
	for (int i = 0; i < CARD_COUNT; i++) {
		key = get_card_strength(hand->hand[i]) - 2;
		hand->card_counts[key] = hand->card_counts[key] + 1;
	}
}

int convert_wc(Hand* hand) {
	int max_card_count_id = 0;
	int max_card_count = 0;
	for (int i = 0; i < NUM_CARDS; i++) {
		if (hand->card_counts[i] > max_card_count && i != 9) {
			max_card_count = hand->card_counts[i];
			max_card_count_id = i;
		}
	}
	printf("max card count id: %d", max_card_count_id);

	hand->card_counts[max_card_count_id] =
		hand->card_counts[max_card_count_id] + hand->card_counts[9];
	hand->card_counts[9] = 0;
}

int count_distinct(Hand* hand) {
	int distinct_count = 0;

	for (int i = 0; i < NUM_CARDS; i++) {
		if (hand->card_counts[i] > 0) {
			distinct_count++;
		}
	}
	return distinct_count;
}

int count_wc(char* hand) {
	int wc_count = 0;
	for (int i = 0; i < CARD_COUNT; i++) {
		if (hand[i] == 'J') {
			wc_count++;
		}
	}
	return wc_count;
}

int get_max_count(Hand* hand) {
	int max = 0;
	int curr_count = 0;
	for (int i = 0; i < NUM_CARDS; i++) {
		curr_count = hand->card_counts[i];
		max = MAX(max, curr_count);
	}
	return max;
}

int get_type(int distinct_count, int max_count) {
	if (max_count == 5) {
		return FIVEOFKIND;
	} else if (max_count == 4) {
		return FOUROFKIND;
	} else if (distinct_count == 2) {
		return FULLHOUSE;
	} else if (max_count == 3) {
		return THREEOFKIND;
	} else if (distinct_count == 3) {
		return TWOPAIR;
	} else if (max_count == 2) {
		return ONEPAIR;
	}
	return HIGHCARD;
}

int compare_same_type(Hand* h1, Hand* h2) {
	int strength1 = 0;
	int strength2 = 0;
	for (int i = 0; i < CARD_COUNT; i++) {
		strength1 = get_card_strength(h1->hand[i]);
		strength2 = get_card_strength(h2->hand[i]);
		if (strength1 < strength2) {
			return -1;
		} else if (strength1 > strength2) {
			return 1;
		}
	}
	return 0;
}

int compare_same_wc_type(Hand* h1, Hand* h2) {
	int strength1 = 0;
	int strength2 = 0;
	for (int i = 0; i < CARD_COUNT; i++) {
		strength1 = get_wc_strength(h1->hand[i]);
		strength2 = get_wc_strength(h2->hand[i]);
		if (strength1 < strength2) {
			return -1;
		} else if (strength1 > strength2) {
			return 1;
		}
	}
	return 0;
}

int compare_hands(Hand* h1, Hand* h2) {
	if (h1->type < h2->type) {
		return -1;
	} else if (h1->type > h2->type) {
		return 1;
	}
	return compare_same_type(h1, h2);
}

int compare_wc_hands(Hand* h1, Hand* h2) {
	if (h1->type < h2->type) {
		return -1;
	} else if (h1->type > h2->type) {
		return 1;
	}
	return compare_same_wc_type(h1, h2);
}

void day7(FILE* fp) {
	Hand hands[MAX_HANDS];
	int hand_count = 0;
	int bid_amount = 0;
	int distinct_count = 0;
	int max_count = 0;
	int type = 0;

	while (fgets(line, MAX_LINE, fp) != NULL) {
		int i = 0;
		for (i = 0; i < CARD_COUNT; i++) {
			hands[hand_count].hand[i] = line[i];
		}
		hands[hand_count].hand[i] = '\0';

		get_num(&bid_amount, line, i + 1);
		hands[hand_count].bid_amount = bid_amount;

		init_card_counts(&hands[hand_count]);
		set_card_counts(&hands[hand_count]);
		printf("card counts: ");
		for (int i = 0; i < NUM_CARDS; i++) {
			printf("%d: %d ", i, hands[hand_count].card_counts[i]);
		}

		distinct_count = count_distinct(hands[hand_count].hand);
		max_count = get_max_count(&hands[hand_count]);
		type = get_type(distinct_count, max_count);
		printf("\ndistinct hands of %s: %d\n", hands[hand_count].hand, distinct_count);
		printf("max count of %s: %d\n", hands[hand_count].hand, max_count);
		printf("type of %s: %d\n\n", hands[hand_count].hand, type);
		hands[hand_count].type = type;

		hand_count++;
	}
	
	qsort(hands, hand_count, sizeof(Hand), &compare_hands);

	int solution = 0;
	distinct_count = 0;
	max_count = 0;
	type = 0;
	for (int i = 0; i < hand_count; i++) {
		printf("%s: %d\n", hands[i].hand, hands[i].bid_amount);
		solution = solution + (i + 1) * hands[i].bid_amount;

		convert_wc(&hands[i]);
		distinct_count = count_distinct(&hands[i]);
		max_count = get_max_count(&hands[i]);
		type = get_type(distinct_count, max_count);
		hands[i].type = type;
	}
	printf("solution: %d\n\n\n", solution);

	qsort(hands, hand_count, sizeof(Hand), &compare_wc_hands);
	
	solution = 0;
	for (int i = 0; i < hand_count; i++) {
		printf("%s: %d\n", hands[i].hand, hands[i].bid_amount);
		solution = solution + (i + 1) * hands[i].bid_amount;
	}
	printf("solution 2 : %d", solution);
}