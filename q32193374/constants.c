#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <asm/unistd_64.h>
#include <sys/ioctl.h>
#include <termios.h>

struct ktermios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_line;
    cc_t c_cc[19];
};

int main(int argc, char *argv[]) {
    struct ktermios orig;

	int ints[] = {__NR_ioctl, __NR_read, TCGETS, TCSETS, sizeof(tcflag_t), sizeof(cc_t)};
#define OO(x) (offsetof(struct ktermios, x))
	int ints2[] = {OO(c_iflag), OO(c_oflag), OO(c_cflag), OO(c_lflag), OO(c_line), OO(c_cc)};
	int ints3[] = {~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON), ~OPOST, ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN), ~(CSIZE | PARENB), CS8};

	for (int i = 0; i < sizeof(ints) / sizeof(int); i++) {
		printf("%d\n", ints[i]);
	}
	puts("===");
	for (int i = 0; i < sizeof(ints2) / sizeof(int); i++) {
		printf("%d\n", ints2[i]);
	}
	puts("===");
	for (int i = 0; i < sizeof(ints3) / sizeof(int); i++) {
		printf("%d\n", ints3[i]);
	}
	puts("===");
	return 0;
}
