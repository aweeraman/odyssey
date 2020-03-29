#define ROWS 25
#define COLS 80

void kernel_main(void) {

  volatile unsigned char *terminal_buf = (volatile unsigned char *) 0xb8000;
  const char *str = "oh hai";

  for (int i = 0; i < (ROWS * COLS); i += 2) {
    terminal_buf[i] = 0;
    terminal_buf[i+1] = 15;
  }

  for (int i = 0, j = 0; str[i] != '\0'; i++, j += 2) {
    terminal_buf[j] = str[i];
    terminal_buf[j+1] = 15;
  }
}
