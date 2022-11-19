#include "../include/image.h"
#include "../include/rotate.h"
#include "../include/utils.h"


#include <stdio.h>
#include <stdlib.h>


int main(int argcnt, char **files) {
    if (argcnt < 3) return 1;

    char *path_in = files[1];
    char *path_out = files[2];
    FILE *in = 0;
    FILE *out = 0;

    struct image *image = &(struct image) {0, 0, 0};
    if (is_error_open_img(path_in, in, image)) {
        fprintf(stderr, "READ_ERROR\n");
        return 1;
    }

    struct image image_rotated = rotate(*image);
    if (is_error_close_img(path_out, out, image_rotated)) {
        fprintf(stderr, "CLOSE_ERROR\n");
        return 1;
    }

    printf("Successful!\n");

    free(image->data);
    free(image_rotated.data);

    return 0;
}
