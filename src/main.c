#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define WIDTH  40
#define HEIGHT 15

struct termios orig_termios;

int snake_x = WIDTH / 2;
int snake_y = HEIGHT / 2;
int dx = 1;
int dy = 0;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void sleep_ms(int ms) {
    usleep(ms * 1000);
}

void draw() {
    printf("\033[2J\033[H");

    for (int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    for (int y = 0; y < HEIGHT - 2; y++) {
        printf("#");
        for (int x = 0; x < WIDTH - 2; x++) {
            if (x == snake_x && y == snake_y)
                printf("@");
            else
                printf(" ");
        }
        printf("#\n");
    }

    for (int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    printf("\nWASD to move | Q to quit");
}

int main() {
    enable_raw_mode();

    while (1) {
        draw();

        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 'q' || c == 'Q') break;
            if (c == 'w') { dx = 0; dy = -1; }
            if (c == 's') { dx = 0; dy = 1; }
            if (c == 'a') { dx = -1; dy = 0; }
            if (c == 'd') { dx = 1; dy = 0; }
        }

        snake_x += dx;
        snake_y += dy;

        // keep inside walls
        if (snake_x < 0) snake_x = 0;
        if (snake_x >= WIDTH - 2) snake_x = WIDTH - 3;
        if (snake_y < 0) snake_y = 0;
        if (snake_y >= HEIGHT - 2) snake_y = HEIGHT - 3;

        fflush(stdout);
        sleep_ms(120);
    }

    return 0;
}
