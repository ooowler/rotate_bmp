#include "../include/image.h"
#include "../include/rotate.h"


#include <inttypes.h>
#include <stdio.h>

struct image rotate(struct image const source) {
    uint32_t new_width = source.height;
    uint32_t new_height = source.width;
    struct image img_rotated = create_image(new_width, new_height);

    for (size_t i = 0; i < new_height; i++) {
        for (size_t j = 0; j < new_width; j++) {
            *(img_rotated.data + (new_height - (i + 1)) * new_width + j) = *(source.data + j * source.width + i);
        }
    }

    return img_rotated;
}
