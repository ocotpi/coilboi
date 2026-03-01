#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  40
#define HEIGHT 15

struct termios orig_termios;

// snake state
int snake_x = WIDTH / 2;
int snake_y = HEIGHT / 2;
int dx = 1;
int dy = 0;

// food state
int food_x;
int food_y;

// score
int score = 0;

/* ---------- terminal handling ---------- */

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

/* ---------- game helpers ---------- */

void spawn_food() {
    food_x = rand() % (WIDTH - 2);
    food_y = rand() % (HEIGHT - 2);
}

void draw() {
    printf("\033[2J\033[H");

    // top border
    for (int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    // playfield
    for (int y = 0; y < HEIGHT - 2; y++) {
        printf("#");
        for (int x = 0; x < WIDTH - 2; x++) {
            if (x == snake_x && y == snake_y)
                printf("@");
            else if (x == food_x && y == food_y)
                printf("*");
            else
                printf(" ");
        }
        printf("#\n");
    }

    // bottom border
    for (int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    printf("Score: %d | WASD to move | Q to quit\n", score);
}

/* ---------- main ---------- */

int main() {
    srand(time(NULL));
    spawn_food();
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

        // keep snake inside walls
        if (snake_x < 0) snake_x = 0;
        if (snake_x >= WIDTH - 2) snake_x = WIDTH - 3;
        if (snake_y < 0) snake_y = 0;
        if (snake_y >= HEIGHT - 2) snake_y = HEIGHT - 3;

        // eat food
        if (snake_x == food_x && snake_y == food_y) {
            score += 10;
            spawn_food();
        }

        fflush(stdout);
        sleep_ms(120);
    }

    return 0;
}