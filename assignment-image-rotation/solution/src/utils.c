#include "../include/bmp_.h"
#include "../include/file_.h"
#include "../include/image.h"

#include "../include/utils.h"

#include <inttypes.h>


uint8_t calculate_padding(struct image const *img) {
    /**
    Находим количество пикселей, умноженных на размер стурктуры пикселя.
    Далее, находим остаток от деления на 4, путем логического умножения на 3.
    Вычитаем лишние байты из 4, и находим сколько нам не хватает до кратности 4.
     **/

    if (img->width % 4 == 0) {
        return 0;
    }

    uint8_t width_padding = ( (3 * img->width) % 4 );
    return 4 - width_padding;
}


int is_error_open_img(char *path_in, FILE *in, struct image *img) {
    /**
    открытие файла -> преобразование из .bmp -> закрытие файла
     **/
    if (open_file_handler(path_in, &in, "r") != 0 ||
        from_bmp_handler(&in, img) != 0 ||
        close_file_handler(&in) != 0
        ) {
        return 1;
    }

    return 0;
}

int is_error_close_img(char *path_out, FILE *out, struct image img_rot) {
    /**
    открытие файла -> преобразование в .bmp -> закрытие файла
     **/
    if (open_file_handler(path_out, &out, "w") != 0 ||
        to_bmp_handler(&out, &img_rot) != 0 ||
        close_file_handler(&out) != 0) {
        return 1;
    }

    return 0;
}
