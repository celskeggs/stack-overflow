#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/ptrace.h>
#include <sys/user.h>
#include <time.h>

// this number is arbitrary - find a better one.
#define STACK_SIZE (1024 * 1024)

int main_thread(void *ptr) {
	// "main" thread is now running under the monitor
	printf("Hello from main!");
	while (1) {
		int c = getchar();
		if (c == EOF) { break; }
		nanosleep(&(struct timespec) {0, 200 * 1000 * 1000}, NULL);
		putchar(c);
	}
	return 0;
}

int main(int argc, char *argv[]) {
	void *vstack = malloc(STACK_SIZE);
	pid_t v;
	if (clone(main_thread, vstack + STACK_SIZE, CLONE_PARENT_SETTID | CLONE_FILES | CLONE_FS | CLONE_IO, NULL, &v) == -1) { // you'll want to check these flags
		perror("failed to spawn child task");
		return 3;
	}
	printf("Target: %d; %d\n", v, getpid());
	long ptv = ptrace(PTRACE_SEIZE, v, NULL, NULL);
	if (ptv == -1) {
		perror("failed monitor sieze");
		exit(1);
	}
	struct user_regs_struct regs;
	fprintf(stderr, "beginning monitor...\n");
	while (1) {
		sleep(1);
		long ptv = ptrace(PTRACE_INTERRUPT, v, NULL, NULL);
		if (ptv == -1) {
			perror("failed to interrupt main thread");
			break;
		}
		int status;
		if (waitpid(v, &status, __WCLONE) == -1) {
			perror("target wait failed");
			break;
		}
		if (!WIFSTOPPED(status)) { // this section is messy. do it better.
			fputs("target wait went wrong", stderr);
			break;
		}
		if ((status >> 8) != (SIGTRAP | PTRACE_EVENT_STOP << 8)) {
			fputs("target wait went wrong (2)", stderr);
			break;
		}
		ptv = ptrace(PTRACE_GETREGS, v, NULL, &regs);
		if (ptv == -1) {
			perror("failed to peek at registers of thread");
			break;
		}
		fprintf(stderr, "%d -> RIP %x RSP %x\n", time(NULL), regs.rip, regs.rsp);
		ptv = ptrace(PTRACE_CONT, v, NULL, NULL);
		if (ptv == -1) {
			perror("failed to resume main thread");
			break;
		}
	}
	return 2;
}
