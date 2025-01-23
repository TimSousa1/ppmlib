#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define PPM_LIB_IMPL_TIM
#include "ppm.h"


enum MAIN_ERROR {
    MALLOC_ERR=1,
    IO_ERR,
};


void print_main_error(int err) {
    switch (err) {
        case MALLOC_ERR:
            fprintf(stderr, "error: MALLOC_ERR\n");
            break;
        case OVER_BIT_LIM:
            fprintf(stderr, "error: OVER_BIT_LIM\n");
            break;
    }
}


int main(void) {
    int32_t err = 0;

    FILE *img = NULL, 
         *out = NULL;
    u_int8_t *pixels = NULL;

    int32_t w = 0, 
        h = 0, 
        b = 0, 
        magic_type = -1;

    img = fopen("in.ppm", "r");

    if ((err = read_ppm_header(img, &w, &h, &b, &magic_type))) {
        print_ppm_error(err);
        return err;
    }

    printf("magic: %d, width: %d, height: %d, bitmax: %d\n", magic_type, w, h, b);
    pixels = malloc(w*h*3);
    if (!pixels) {
        fclose(img);
        return MALLOC_ERR;
    }

    err = read_ppm(pixels, img, w, h, b, magic_type);
    if (err) {
        fclose(img);
        free(pixels);
        print_ppm_error(err);
        return err; 
    }


    out = fopen("out.ppm", "w");
    if (!out) {
        fclose(img);
        free(pixels);
        return IO_ERR; 
    }

    write_ppm_p6(pixels, out, w, h, b);

    fclose(img);
    fclose(out);

    return 0;
}
