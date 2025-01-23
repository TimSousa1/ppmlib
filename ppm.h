#ifndef PPM_LIB_TIM
#define PPM_LIB_TIM

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

enum PPM_ERROR {
    MAGIC_MALFORMED=1,
    OVER_BIT_LIM,
};

enum MAGIC_TYPE {
    P3, P6
};

#ifdef PPM_LIB_IMPL_TIM

void print_ppm_error(int err) {
    switch (err) {
        case MAGIC_MALFORMED:
            fprintf(stderr, "error: MAGIC_MALFORMED\n");
            break;
        case OVER_BIT_LIM:
            fprintf(stderr, "error: OVER_BIT_LIM\n");
            break;
    }
}



// expects a 255 bitwitdh
int32_t read_ppm_p3(u_int8_t *pixels, FILE *img, int32_t w, int32_t h, int32_t b) {
    if (b > 255) return OVER_BIT_LIM;
    rewind(img);
    fscanf(img, "%*s %*d %*d %*d ");

    for (int32_t i = 0; i < w*h; i++) {
        fscanf(img, "%d %d %d", &pixels[3*i], &pixels[3*i+1], &pixels[3*i+2]);
    }

    return 0;
}

int32_t write_ppm_p3(u_int8_t *pixels, FILE *img, int w, int h, int b) {
    rewind(img);
    fprintf(img, "%s\n%d %d\n%d\n", "P3", w, h, b);

    int32_t i = 0;
    for (int32_t y = 0; y < h; y++) {
        for (int32_t x = 0; x < w; x++) {
            fprintf(img, "%u %u %u ", pixels[3*i], pixels[3*i+1], pixels[3*i+2]);
            i++;
        }
        fprintf(img, "\n");
    }

    return 0;
}



int32_t read_ppm_p6(u_int8_t *pixels, FILE *img, int32_t w, int32_t h, int32_t b) {
    if (b < 255) return OVER_BIT_LIM;
    rewind(img);
    fscanf(img, "%*s %*d %*d %*d ");

    int32_t n_bytes;
    n_bytes = fread(pixels, 1, w*h*3, img);

    printf("bytes read: %d\n", n_bytes);

    return 0;
}

int32_t write_ppm_p6(u_int8_t *pixels, FILE *img, int32_t w, int32_t h, int32_t b) {
    rewind(img);
    fprintf(img, "%s\n%d %d\n%d\n", "P6", w, h, b);

    int32_t n_bytes;
    n_bytes = fwrite(pixels, 1, w*h*3, img);

    printf("bytes written: %d\n", n_bytes);

    return 0;
}


int32_t read_ppm_header(FILE *img, int32_t *w, int32_t *h, int32_t *b, int32_t *magic_type) {
    char magic[3];

    rewind(img);
    fscanf(img, "%s %d %d %d", magic, w, h, b);

    if (strcmp(magic, "P3") == 0) *magic_type = P3;
    else if (strcmp(magic, "P6") == 0) *magic_type = P6;
    else return MAGIC_MALFORMED;

    return 0;
}

int32_t read_ppm(u_int8_t *pixels, FILE *img, int32_t w, int32_t h, int32_t b, int32_t magic_type) {
    int32_t err = 0;
    switch (magic_type) {
        case P3:
            err = read_ppm_p3(pixels, img, w, h, b);
            break;

        case P6:
            err = read_ppm_p6(pixels, img, w, h, b);
            break;

        default:
            err = MAGIC_MALFORMED;
            break;
    }

    return err;
}

#endif
#endif
