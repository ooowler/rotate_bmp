#ifndef UTILS_H
#define UTILS_H

#include "image.h"
#include <inttypes.h>
#include <stdio.h>


uint8_t calculate_padding(struct image const *img);

int is_error_open_img(char *path_in, FILE *in, struct image *img);

int is_error_close_img(char *path_out, FILE *out, struct image img_rot);

#endif
