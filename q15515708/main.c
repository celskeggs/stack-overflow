#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

// -fno-omit-frame-pointer might be required - otherwise you might get a crash
// might not be exactly at the top; but probably very close

void *stack_top() {
	void **top;
	asm("movq %%rbp, %0" : "=r" (top));
	// if top is higher in memory than the variable, then still part of the stack.
	while ((uintptr_t) *top > (uintptr_t) &top) {
		top = *top;
	}
	return top;
}

void *test_another_layer(int x) {
	return stack_top();
}

void *subthread(void *ptr) {
	void *st1 = stack_top();
	void *st2 = test_another_layer(0);
	void *st3 = &ptr;
	printf("stack tops 2: %x %x %x\n", st1, st2, st3);
	return NULL;
}

int main(int argc, char *argv[]) {
	void *st1 = stack_top();
	void *st2 = test_another_layer(0);
	void *st3 = &argc;
	printf("stack tops: %x %x %x\n", st1, st2, st3);
	pthread_t ot;
	if (pthread_create(&ot, NULL, subthread, NULL) != 0) {
		perror("cannot create");
		return 1;
	}
	if (pthread_join(ot, NULL) != 0) {
		perror("cannot join");
		return 2;
	}
	return 0;
}
