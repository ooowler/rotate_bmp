#include "../include/image.h"

#include <inttypes.h>
#include <stdlib.h>

struct image create_image(uint64_t width, uint64_t height) {
    return (struct image) {width, height, malloc(sizeof(struct pixel) * width * height)};
}
