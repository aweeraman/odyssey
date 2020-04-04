#include "types.h"
#include "print.h"
#include "serial.h"

cell *matrix = (cell *) 0xb8000;

static size_t cur_x = 0;
static size_t cur_y = 0;

void scroll() {
  for (size_t i=0; i<(TERMINAL_ROWS-1); i++) {
    for (size_t j=0; j<TERMINAL_COLS; j++) {
      matrix[(i*TERMINAL_COLS) + j].ch    = matrix[(i+1)*TERMINAL_COLS + j].ch;
      matrix[(i*TERMINAL_COLS) + j].clr   = matrix[(i+1)*TERMINAL_COLS + j].clr;
      matrix[(i+1)*TERMINAL_COLS + j].ch  = 0;
      matrix[(i+1)*TERMINAL_COLS + j].clr = 0;
    }
  }
}

void print_char(__uint8_t ch) {
  if (cur_y >= TERMINAL_COLS) {
    cur_x++;
    cur_y = 0;
  }
  if (cur_x >= TERMINAL_ROWS) {
    scroll();
    cur_x = TERMINAL_ROWS-1;
    cur_y = 0;
  }
  if (ch == '\n') {
    cur_x++;
    cur_y = 0;
    if (cur_x >= TERMINAL_ROWS) {
      scroll();
      cur_x = TERMINAL_ROWS-1;
    }
    write_serial('\r');
    write_serial('\n');
    return;
  }
  matrix[(cur_x*TERMINAL_COLS) + cur_y++] = (cell) {
    .ch = ch,
    .clr = CLR_WHITE
  };
  write_serial(ch);
}

void clear_screen(void) {
  for (size_t i=0; i<(TERMINAL_ROWS*TERMINAL_COLS); i++) {
    matrix[i].ch = 0;
    matrix[i].clr = 0;
  }
}

void print(char *str) {
  for (size_t i=0; str[i] != '\0'; i++) {
    print_char(str[i]);
  }
}
