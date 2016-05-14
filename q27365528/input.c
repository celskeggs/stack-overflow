#include <unistd.h>
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

int getch() {
    unsigned char c;
    read(0, &c, sizeof(c));
    return c;
}

int main(int argc, char *argv[]) {
    struct ktermios orig, new;
    ioctl(0, TCGETS, &orig);
    ioctl(0, TCGETS, &new);

    // from cfmakeraw documentation
    new.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    new.c_oflag &= ~OPOST;
    new.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    new.c_cflag &= ~(CSIZE | PARENB);
    new.c_cflag |= CS8;

    ioctl(0, TCSETS, &new);

    while (1) {
        if (getch() == 0x1b && getch() == 0x4f && getch() == 0x50) {
            break;
        }
    }

    write(1, "Got F1!\r\n", 9);
    ioctl(0, TCSETS, &orig);
    return 0;
}
