#ifndef PRINT_H
#define PRINT_H

#define ROWS 25
#define COLS 80

typedef struct {
  unsigned char ch;
  unsigned char clr;
} __attribute__((packed)) cell;

void clear_screen(void);
void print(char *str);

#endif
