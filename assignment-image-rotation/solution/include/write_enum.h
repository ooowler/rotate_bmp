#ifndef WRITE_ENUM_H
#define WRITE_ENUM_H

enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR,
    WRITE_EMPTY_FILE,
    WRITE_INVALID_WHENCE,
    WRITE_INVALID_OFFSET
};

#endif
