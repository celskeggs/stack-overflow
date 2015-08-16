#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int is_mmaped(void *ptr, size_t length) {
	FILE *file = fopen("/proc/self/maps", "r");
	char line[1024];
	int result = 0;
	while (!feof(file)) {
		if (fgets(line, sizeof(line) / sizeof(char), file) == NULL) {
			break;
		}
		unsigned long start, end;
		if (sscanf(line, "%lx-%lx", &start, &end) != 2) {
			continue; // could not parse. fail gracefully and try again on the next line.
		}
		unsigned long ptri = (long) ptr;
		if (ptri >= start && ptri + length <= end) {
			result = 1;
			break;
		}
	}
	fclose(file);
	return result;
}

int main(int argc, char *argv[]) {
	void *test = mmap(NULL, 16384, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	printf("T %d\n", is_mmaped(test, 16384));
	printf("F %d\n", is_mmaped(test, 16385));
	printf("F %d\n", is_mmaped(test + 1, 16384));
	printf("T %d\n", is_mmaped(test, 1024));
	printf("T %d\n", is_mmaped(test, 256));
	printf("T %d\n", is_mmaped(test, 8));
	printf("T %d\n", is_mmaped(test + 16383, 1));
	munmap(test, 16384);
	printf("F %d\n", is_mmaped(test, 16384));

	printf("T %d\n", is_mmaped(main, 32));
	return 0;
}
