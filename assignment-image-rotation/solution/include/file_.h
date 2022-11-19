#ifndef FILE_H
#define FILE_H

#include <stdio.h>

int open_file_handler(char *path, FILE **file, char *f_open_argument);
int close_file_handler(FILE **file);

#endif
