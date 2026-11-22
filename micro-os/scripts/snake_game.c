/*
 * CYBERLAB OS - RETRO SNAKE ARCADE GAME
 * A fast, lightweight terminal and desktop arcade game.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define WIDTH  40
#define HEIGHT 20

typedef struct {
    int x, y;
} Point;

static Point snake[WIDTH * HEIGHT];
static int snake_len = 4;
static Point food;
static int dir_x = 1, dir_y = 0;
static int score = 0;
static int high_score = 0;
static bool game_over = false;

static struct termios orig_termios;

void reset_terminal_mode() {
    tcsetattr(0, TCSANOW, &orig_termios);
    printf("\e[?25h"); // show cursor
}

void set_conio_terminal_mode() {
    struct termios new_termios;
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
    printf("\e[?25l"); // hide cursor
}

int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch() {
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) return r;
    return c;
}

void spawn_food() {
    bool on_snake;
    do {
        on_snake = false;
        food.x = 1 + rand() % (WIDTH - 2);
        food.y = 1 + rand() % (HEIGHT - 2);
        for (int i = 0; i < snake_len; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                on_snake = true;
                break;
            }
        }
    } while (on_snake);
}

void init_game() {
    snake_len = 4;
    dir_x = 1;
    dir_y = 0;
    score = 0;
    game_over = false;
    int start_x = WIDTH / 4;
    int start_y = HEIGHT / 2;
    for (int i = 0; i < snake_len; i++) {
        snake[i].x = start_x - i;
        snake[i].y = start_y;
    }
    spawn_food();
}

void draw() {
    // Clear screen and draw board
    printf("\e[H");
    printf("\r\e[1;36m+--------------------------------------+\r\n");
    printf("|       CYBERLAB RETRO SNAKE ARCADE    |\r\n");
    printf("+--------------------------------------+\e[0m\r\n");
    printf("\r\e[1;33mScore: %-5d   High Score: %-5d\e[0m\r\n", score, high_score);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                printf("\e[1;34m#\e[0m");
            } else if (x == food.x && y == food.y) {
                printf("\e[1;31m@\e[0m"); // Food
            } else {
                bool is_body = false;
                if (x == snake[0].x && y == snake[0].y) {
                    printf("\e[1;32mO\e[0m"); // Head
                    is_body = true;
                } else {
                    for (int i = 1; i < snake_len; i++) {
                        if (snake[i].x == x && snake[i].y == y) {
                            printf("\e[0;32mo\e[0m"); // Body
                            is_body = true;
                            break;
                        }
                    }
                }
                if (!is_body) printf(" ");
            }
        }
        printf("\r\n");
    }
    printf("\r\e[1;37mControls: W/A/S/D or Arrow Keys | Q: Quit | R: Restart\e[0m\r\n");
}

void update() {
    Point next_head = { snake[0].x + dir_x, snake[0].y + dir_y };

    // Wall collision
    if (next_head.x <= 0 || next_head.x >= WIDTH - 1 ||
        next_head.y <= 0 || next_head.y >= HEIGHT - 1) {
        game_over = true;
        return;
    }

    // Self collision
    for (int i = 0; i < snake_len; i++) {
        if (snake[i].x == next_head.x && snake[i].y == next_head.y) {
            game_over = true;
            return;
        }
    }

    // Move body
    for (int i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = next_head;

    // Eat food
    if (next_head.x == food.x && next_head.y == food.y) {
        score += 10;
        if (score > high_score) high_score = score;
        if (snake_len < WIDTH * HEIGHT - 1) {
            snake[snake_len] = snake[snake_len - 1];
            snake_len++;
        }
        spawn_food();
    }
}

int main() {
    srand(time(NULL));
    set_conio_terminal_mode();
    printf("\e[2J"); // Clear screen
    init_game();

    while (1) {
        draw();
        if (game_over) {
            printf("\r\n\e[1;31m*** GAME OVER! Press 'R' to restart or 'Q' to quit ***\e[0m\r\n");
            while (1) {
                if (kbhit()) {
                    int c = getch();
                    if (c == 'r' || c == 'R') {
                        init_game();
                        printf("\e[2J");
                        break;
                    } else if (c == 'q' || c == 'Q') {
                        return 0;
                    }
                }
                usleep(50000);
            }
        }

        if (kbhit()) {
            int c = getch();
            if (c == 'q' || c == 'Q') break;
            if (c == 'r' || c == 'R') { init_game(); printf("\e[2J"); }
            if ((c == 'w' || c == 'W') && dir_y != 1)  { dir_x = 0; dir_y = -1; }
            if ((c == 's' || c == 'S') && dir_y != -1) { dir_x = 0; dir_y = 1; }
            if ((c == 'a' || c == 'A') && dir_x != 1)  { dir_x = -1; dir_y = 0; }
            if ((c == 'd' || c == 'D') && dir_x != -1) { dir_x = 1; dir_y = 0; }
            // Arrow keys escape sequence: \e[A, \e[B, etc.
            if (c == 27) {
                int c2 = getch();
                if (c2 == '[') {
                    int c3 = getch();
                    if (c3 == 'A' && dir_y != 1)  { dir_x = 0; dir_y = -1; }
                    if (c3 == 'B' && dir_y != -1) { dir_x = 0; dir_y = 1; }
                    if (c3 == 'C' && dir_x != -1) { dir_x = 1; dir_y = 0; }
                    if (c3 == 'D' && dir_x != 1)  { dir_x = -1; dir_y = 0; }
                }
            }
        }

        update();
        usleep(120000); // 120ms tick
    }

    return 0;
}
