#include "../include/bmp_.h"
#include "../include/image.h"
#include "../include/read_enum.h"
#include "../include/utils.h"
#include "../include/write_enum.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
struct bmp_header *create_bmp_header(struct image const *img) {
    static const uint16_t bf_Type = 0x4D42;
    static const uint16_t bi_Bit_Count = 24;
    static const uint16_t bi_Planes = 1;
    static const uint32_t bi_Size = 40;
    static const uint32_t bf_Reserved = 0;
    static const uint32_t bi_Compression = 0;
    static const uint32_t bi_ClrUsed = 0;
    static const uint32_t bi_ClrImportant = 0;
    static const uint32_t bi_XPelsPerMeter = 2834;
    static const uint32_t bi_YPelsPerMeter = 2834;

    struct bmp_header *bh = malloc(sizeof(struct bmp_header));
    uint8_t padding = calculate_padding(img);

    *bh = (struct bmp_header) {
            .bfType = bf_Type,
            .bfileSize = sizeof(struct bmp_header) + (img->height * img->width) * 3 + padding * img->height,
            .bfReserved = bf_Reserved,
            .bOffBits = sizeof(struct bmp_header),
            .biSize = bi_Size,
            .biWidth = img->width,
            .biHeight = img->height,
            .biPlanes = bi_Planes,
            .biBitCount = bi_Bit_Count,
            .biCompression = bi_Compression,
            .biSizeImage = (img->height * img->width) * 3 + padding * img->height,
            .biXPelsPerMeter = bi_XPelsPerMeter,
            .biYPelsPerMeter = bi_YPelsPerMeter,
            .biClrUsed = bi_ClrUsed,
            .biClrImportant = bi_ClrImportant
    };

    return bh;
}

static enum write_status write_bmp_header(FILE **out, struct image const *img) {
//    if (*out == NULL) return WRITE_EMPTY_FILE;

    struct bmp_header *bh = create_bmp_header(img);
//    uint8_t was_written = fwrite(bh, sizeof(struct bmp_header), 1, *out);

    if (fwrite(bh, sizeof(struct bmp_header), 1, *out) != 1) {
        free(bh);
        return WRITE_ERROR;
    }
    free(bh);

    return WRITE_OK;
}

static enum write_status write_picture(FILE **out, struct image const *img) {
    if (*out == NULL) return WRITE_EMPTY_FILE;

    uint8_t padding = calculate_padding(img);

    for (size_t i = 0; i < img->height; i++) {
        for (size_t j = 0; j < img->width; j++) {
            size_t result_f_write = fwrite(img->data + (img->height - 1 - i) * img->width + j, sizeof(struct pixel), 1,*out);
            if (result_f_write != 1) return WRITE_ERROR;
        }
        int result_fseek = fseek(*out, padding, SEEK_CUR);

        switch (result_fseek) {
            case(EINVAL):
                return WRITE_INVALID_WHENCE;
            case(ESPIPE):
                return WRITE_INVALID_OFFSET;
        }
    }

    return WRITE_OK;
}

static enum write_status to_bmp(FILE **out, struct image const *img) {
//    if (*out == NULL) return WRITE_EMPTY_FILE;

    enum write_status res_bmp = write_bmp_header(out, img);
    if (res_bmp != 0) return res_bmp;

    enum write_status res_pict = write_picture(out, img);
    if (res_pict != 0) return res_pict;

    return WRITE_OK;
}

static enum read_status check_bmp(struct bmp_header *bh) {
    if (bh->bOffBits != 14 + bh->biSize) return READ_INVALID_OFFBITS;
    if (bh->biWidth < 1 || bh->biWidth > 10000) return READ_INVALID_WIDTH;
    if (bh->biPlanes != 1) return READ_INVALID_PLANES;
    if (bh->biBitCount != 24) return READ_INVALID_BITS_COUNT;
    if (bh->biSize != 40 && bh->biSize != 108 && bh->biSize != 124) return READ_INVALID_SIZE;//Проверяем, что наш хеддер 40 или 108 или 124 байт
    if (bh->bfReserved != 0) return READ_INVALID_RESERVED;
    if (bh->biHeight < 1 || bh->biHeight > 10000) return READ_INVALID_HEIGTH;
    if (bh->bfType != 0x4d42 && bh->bfType != 0x4349 && bh->bfType != 0x5450) return READ_INVALID_TYPE;

    return READ_OK;
}


static enum read_status from_bmp(FILE **in, struct image *img) {
    struct bmp_header *bh = malloc(sizeof(struct bmp_header));
    fread(bh, sizeof(struct bmp_header), 1, *in);

    if (check_bmp(bh) != READ_OK) return check_bmp(bh);

    uint64_t width = bh->biWidth;
    uint64_t height = bh->biHeight;

    *img = (struct image) {bh->biWidth, bh->biHeight, malloc(sizeof(struct pixel) * width * height)};
    uint8_t padding = calculate_padding(img);

    free(bh);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            fread(img->data + (height - 1 - i) * width + j, sizeof(struct pixel), 1, *in);
        }

        fseek(*in, padding, SEEK_CUR);
    }

    return READ_OK;
}

int from_bmp_handler(FILE **in, struct image *img) {
    enum read_status result_from_bmp = from_bmp(in, img);
    if (result_from_bmp != 0) {
        fprintf(stderr, "Ошибка при попытке чтения файла, код ошибки: %d\n", result_from_bmp);
        return 1;
    }

    return 0;
}

int to_bmp_handler(FILE **out, struct image const *img) {
    enum write_status result_to_bmp = to_bmp(out, img);
    if (result_to_bmp != 0) {
        fprintf(stderr, "Ошибка при попытке записать файл, код ошибки: %d\n", result_to_bmp);
        return 1;
    }

    return 0;
}
