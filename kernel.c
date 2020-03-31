#define ROWS 25
#define COLS 80

void pause(void) {
  for (long l=0L; l<1000000; l++);
}

void kernel_main(void) {
  volatile unsigned char *terminal_buf = (volatile unsigned char *) 0xb8000;

  for (int i = 0; i < (ROWS * COLS); i += 2) {
    terminal_buf[i] = 0;
    terminal_buf[i+1] = 15;
  }

  while (1) {
    terminal_buf[0] = '|';
    pause();
    terminal_buf[0] = '/';
    pause();
    terminal_buf[0] = '-';
    pause();
    terminal_buf[0] = '\\';
  }
}
