#include <curses.h>
#include <ncurses.h>
#include <time.h>

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
} direction;

typedef struct {
  int x;
  int y;
} pos;

typedef struct {
  int seed;
  int multiplier;
  int increment;
  int modulus;
} rng_stuff;

pos food;
pos snake[511] = {};
rng_stuff rng = {0};
int xMax, yMax;

int tail_len;
direction current_dir;
WINDOW *board;

pos board_dim;

int rng_rand() {
  rng.seed = (rng.seed * rng.multiplier + rng.increment) % rng.modulus;
  if (rng.seed < 0) {
    rng.seed = rng.seed * -1;
  }
  return rng.seed;
}
void init_curses() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();
  refresh();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);
  init_pair(8, COLOR_BLACK, COLOR_BLACK);
  getmaxyx(stdscr, yMax, xMax);
  board_dim.x = xMax - (10);
  board_dim.y = yMax - 5;
  board = newwin(board_dim.y, board_dim.x, (yMax / 2) - (board_dim.y / 2),
                 (xMax / 2) - (board_dim.x) / 2);
  box(board, 0, 0);
  wrefresh(board);
}
void init() {
  rng.seed = (int)time(NULL);
  rng.multiplier = 1103515245;
  rng.increment = 12345;
  rng.modulus = 0x8000000;

  current_dir = RIGHT;
  int tail_len = 0;
}

void drawHead(pos *loc) {
  attron(COLOR_PAIR(1));
  mvprintw(loc->y, loc->x, "0");
  attroff(COLOR_PAIR(1));
  wrefresh(board);
}

void spawnFood() {
  food.x = ((rng_rand() % (xMax / 2)) + 10);
  food.y = ((rng_rand() % (yMax / 2)) + 10);
  attron(COLOR_PAIR(2));
  mvprintw(food.y, food.x, "o");
  attroff(COLOR_PAIR(2));
  wrefresh(board);
}
int main() {
  init_curses();
  init();
  int count = 0;
  while (1) {
    if (count == 20) {
      break;
    }
    mvprintw(food.y, food.x, " ");
    spawnFood();
    pos loc = {20, 20};
    drawHead(&loc);

    getch();
    count++;
  }

  endwin(); // Restore normal terminal behavior
  nocbreak();
  return 0;
}
