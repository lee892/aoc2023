
#include "sources.h"
#include "queue.h"


int main(int argc, char* argv[]) {
	FILE* fp = NULL;
	int err = fopen_s(&fp, "day7.input", "r");
	if (err != 0) {
		perror("File open failed");
		return 0;
	}
	day7(fp);

	fclose(fp);
	return 0;
}
