#include <inttypes.h>

#ifndef IMAGE_H
#define IMAGE_H

struct pixel {
    uint8_t b, g, r;
};
struct image {
    uint64_t width, height;
    struct pixel *data;
};

struct image create_image(uint64_t width, uint64_t height);

#endif
