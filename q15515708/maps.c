#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	FILE *file = fopen("/proc/self/maps", "r");
	char line[1024];
	void *result = NULL;
	while (!feof(file)) {
		if (fgets(line, sizeof(line) / sizeof(char), file) == NULL) {
			break;
		}
		unsigned long start, end, offset;
		unsigned int devma, devmi, ino;
		char perms[6];
		char path[128];
		if (sscanf(line, "%lx-%lx %5s %lx %d:%d %d %127s", &start, &end, &perms, &offset, &devma, &devmi, &ino, &path) != 8) {
			continue; // could not parse. fail gracefully and try again on the next line.
		}
		if (strcmp(path, "[stack]") == 0) { // use [stack:TID] for a thread besides the main thread
			printf("Stack found from %lx to %lx\n", start, end);
			break;
		}
	}
	fclose(file);
	return 0;
}
