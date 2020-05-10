#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pixel {
        u_char r;
        u_char g;
        u_char b;
} __attribute__((packed));

int main(int argc, char **argv)
{
        FILE *fp;
        u_char buf;
        char *line = NULL;
        size_t len = 0;
        size_t cols, rows, b;
        size_t maxVal;
        struct pixel pix;

        if (argc != 2) {
                fprintf(stderr, "Usage: %s ppm-file\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
        }

        getline(&line, &len, fp);
        if (strncmp(line, "P6", 2) != 0) {
                fprintf(stderr, "%s is not a valid PPM file\n", argv[1]);
                exit(EXIT_FAILURE);
        }
        fscanf(fp, "%d %d\n", &cols, &rows);
        fscanf(fp, "%d\n", &maxVal);
        printf("const int PPM_ROWS = %d;\n", rows);
        printf("const int PPM_COLS = %d;\n", cols);
        printf("const char ppm_array[%d * %d * 3] = {\n", rows, cols);
        fseek(fp, 1, SEEK_CUR);
        while (fread(&pix, sizeof(struct pixel), 1, fp) == 1) {
                printf("0x%x, 0x%x, 0x%x,\n", pix.r, pix.g, pix.b);
        }
        printf("};");

        free(line);
        fclose(fp);
        exit(EXIT_SUCCESS);
}
