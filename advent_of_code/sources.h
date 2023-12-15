#pragma once

typedef unsigned long long llu;

#define MAX_LINES 1024
#define MAX_LINE 512

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

char* grid[MAX_LINES];

char line[MAX_LINE];


int find_char(char str[], char c);
int is_num(char c);
int get_num(int* num, char* line, int i);
int get_word_length(char* line, int i);

void day2(FILE* fp);
void day3(FILE* fp);
void day4(FILE* fp);
void day5(FILE* fp);
void day6(FILE* fp);
