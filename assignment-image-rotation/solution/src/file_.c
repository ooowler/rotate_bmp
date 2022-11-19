#include "../include/file_.h"
#include "../include/open_close_enum.h"


#include  <stdint.h>
#include <stdio.h>


static enum open_status open_file(char *path, FILE **file, char *fopen_argument) {
    *file = fopen(path, fopen_argument);
    if (*file == NULL) {
        return OPEN_ERROR;
    }

    return OPEN_OK;
}

static enum close_status close_file(FILE **file) {
    int is_close = fclose(*file);
    if (is_close != 0) {
        return CLOSE_ERROR;
    }

    return CLOSE_OK;
}

int open_file_handler(char *path, FILE **file, char *f_open_argument) {
    enum open_status is_valid = open_file(path, file, f_open_argument);
    if (is_valid != 0) {
        fprintf(stderr, "Ошибка при попытке открытия файла, код ошибки: %d\n", is_valid);
        return 1;
    }

    return 0;
}

int close_file_handler(FILE **file) {
    enum close_status is_valid = close_file(file);
    if (is_valid != 0) {
        fprintf(stderr, "Ошибка при попытке закрытия файла, код ошибки: %d\n", is_valid);
        return 1;
    }

    return 0;
}
