#ifndef BMP_H
#define BMP_H

#include "../include/image.h"
#include <stdio.h>

int from_bmp_handler(FILE **in, struct image *img);

int to_bmp_handler(FILE **out, struct image const *img);


#endif
