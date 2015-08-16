#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define ITER_COUNT (1024 * 64)
#define DELAY (2500 * 1000)

int main(int argc, char *argv[]) {
	clockid_t clk_id = CLOCK_REALTIME;

	struct timespec resolution;
	if (clock_getres(clk_id, &resolution) != 0) {
		perror("getres failed");
		return 1;
	}
	printf("RESOLUTION: %d us + %d s\n", resolution.tv_nsec, resolution.tv_sec);

	struct timespec sleep_for;
	sleep_for.tv_sec = DELAY / 1000000000;
	sleep_for.tv_nsec = DELAY % 1000000000;

	char out[ITER_COUNT+1];
	memset(out, '0', ITER_COUNT);
	int i;
	struct timespec now;
	if (clock_gettime(clk_id, &now) != 0) {
		perror("gettime failed");
		return 1;
	}
	uint64_t start = ((uint64_t) now.tv_nsec) + 1000000000 * ((uint64_t) now.tv_sec);
	printf("start: %" PRIu64 " from %" PRIu64 ":%.9" PRIu64 "\n", start, now.tv_sec, now.tv_nsec);
	uint64_t iters = 0;
	uint64_t max_delay = 0;
	uint64_t last = start;
	while (1) {
		if (clock_gettime(clk_id, &now) != 0) {
			perror("gettime failed");
			return 1;
		}
		uint64_t abs = ((uint64_t) now.tv_nsec) + 1000000000 * ((uint64_t) now.tv_sec);
		uint64_t offset = (abs - start) / DELAY;
		if (offset < 0 || offset >= ITER_COUNT) {
			// printf("failed with bad offset: %" PRIu64 " -> %" PRIu64 " from %" PRIu64 ":%.9" PRIu64 "\n", rel, offset, now.tv_sec, now.tv_nsec);
			break;
		}
		out[offset]++;
		if (iters >= 128 && abs - last > max_delay) {
			max_delay = abs - last;
		}
		last = abs;
		if (nanosleep(&sleep_for, NULL) != 0) {
			perror("nanosleep failed");
			return 1;
		}
		iters++;
	}
	out[ITER_COUNT] = 0;
	//fputs(out, stdout);
	printf("\nIterations: %d\nMax delay: %d\n", iters, max_delay);

	return 0;
}
