#include "linkedlist.h"
#include <curses.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#define DELAY 30000
#define TIMEOUT 10

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
Node *head = NULL;
pos head_next;
rng_stuff rng = {0};
int xMax, yMax;
int score = 0;

int tail_len;
direction current_dir;
WINDOW *board;
int game_over = 0;
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
  nodelay(stdscr, TRUE);
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(TIMEOUT);
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
void draw_food() {
  wattron(board, COLOR_PAIR(2));
  mvwprintw(board, food.y, food.x, "o");
  wattroff(board, COLOR_PAIR(2));
  wrefresh(board);
}

void spawn_food() {
  food.x = ((rng_rand() % (xMax / 2)) + 10);
  food.y = ((rng_rand() % (yMax / 2)) + 10);
  draw_food();
}

void init() {
  rng.seed = (int)time(NULL);
  rng.multiplier = 1103515245;
  rng.increment = 12345;
  rng.modulus = 0x8000000;
  spawn_food();
  current_dir = RIGHT;
  tail_len = 0;
  head->x = 10;
  head->y = 10;
  head = createNode(10, 10);
}

void draw_head(Node *node) {
  wattron(board, COLOR_PAIR(1));
  mvwprintw(board, node->y, node->x, "0");
  wattroff(board, COLOR_PAIR(1));
  wrefresh(board);
}
void draw_body(Node *node) { mvwprintw(board, node->y, node->x, "#"); }

void clear_print(Node *node) { mvwprintw(board, node->y, node->x, " "); }
void add_segment() {
  tail_len++;
  Node *new_tail = createNode(head->x, head->y);

  switch (current_dir) {
  case UP:
    new_tail->y++;
    break;
  case DOWN:
    new_tail->y--;
    break;
  case LEFT:
    new_tail->x++;
    break;
  case RIGHT:
    new_tail->x--;
    break;
  }
  new_tail->next = head;
  head = new_tail;
}

void input_handler(char ch) {
  if ((ch == 'l' || ch == 'L' || ch == KEY_RIGHT || ch == 'D' || ch == 'd') &&
      (current_dir != RIGHT && current_dir != LEFT)) {
    current_dir = RIGHT;
  } else if ((ch == 'h' || ch == 'H' || ch == KEY_LEFT || ch == 'A' ||
              ch == 'a') &&
             (current_dir != RIGHT && current_dir != LEFT)) {
    current_dir = LEFT;
  } else if ((ch == 'j' || ch == 'J' || ch == KEY_DOWN || ch == 's' ||
              ch == 'S') &&
             (current_dir != UP && current_dir != DOWN)) {
    current_dir = DOWN;
  } else if ((ch == 'k' || ch == 'K' || ch == KEY_UP || ch == 'W' ||
              ch == 'w') &&
             (current_dir != UP && current_dir != DOWN)) {
    current_dir = UP;
  } else if ((ch == KEY_EXIT) || ch == 'q' || ch == 'e') {
    game_over = true;

  } else if ((ch == 'p' || ch == 'P')) {
    add_segment();
  }
}
void move_snake() {
  head_next.x = head->x;
  head_next.y = head->y;

  switch (current_dir) {
  case UP:
    head_next.y--;
    break;
  case DOWN:
    head_next.y++;
    break;
  case LEFT:
    head_next.x--;
    break;
  case RIGHT:
    head_next.x++;
    break;
  }

  // Check if the new head position is valid
  int collision = 0;
  if (head_next.x >= 0 && head_next.x < (xMax - 10) && head_next.y >= 0 &&
      head_next.y < (yMax - 5)) {
    // Check if the new head position is not colliding with the snake's body
    Node *current = head->next;
    // probably will segfault lol
    for (int i = 1; i <= tail_len - 1; i++) {
      pos segment;
      segment.x = current->x;
      segment.y = current->y;
      if (head_next.x == segment.x && head_next.y == segment.y) {
        int collision = 1;
        break;
      }
      current = current->next;
    }
  } else {
    collision = 1;
  }

  if (!collision) {
    // Move the snake by shifting each body segment
    Node *new_head = createNode(head_next.x, head_next.y);
    new_head->next = head;
    head = new_head;

    // Remove the tail if not growing
    if (tail_len > 0) {
      removeTail(&head);
      tail_len--;
    }
  } else {
    game_over = 1;
  }

  if (head->x == food.x && head->y == food.y) {
    spawn_food();
    tail_len++;
  }
}

void draw_screen() {
  box(board, 0, 0);
  mvprintw(1, 2, "       ");
  mvprintw(1, 2, "Score: %i", score);
  mvprintw(1, xMax - 20, "Head_pos: %i, %i", head->x, head->y);
  Node *current = head;
  while (current != NULL) {
    if (current == head) {
      draw_head(current);
    } else {
      draw_body(current);
    }
    current = current->next;
  };
  draw_food();
  refresh();
  usleep(DELAY * 2);
}
int main() {
  init_curses();
  init();
  while (1) {
    if (game_over) {
      clear();
      printw("Womp Womp! Game Over!");
      refresh();
      nodelay(stdscr, FALSE);
      getch();
      break;
    } else {
      draw_head(head);
      input_handler(
          getch()); // handling inputs in a separate function for code clarity
      move_snake();

      draw_screen();
    }
  }

  endwin(); // Restore normal terminal behavior
  nocbreak();
  return 0;
}
