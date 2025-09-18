#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "io_utils.h"

void show_gif(string_t filename) {
    assert(filename != NULL && "U must pass filename");

    if (filename == NULL) {
        errno = EINVAL;
        return;
    }

#ifdef __APPLE__ // command for macos
    // const uint8_t max_len = 100; // max comand len (max filename len is 50)
    char cmd[512] = {0};
    snprintf(cmd, sizeof(cmd), "qlmanage -p cat_gifs/%s.gif > /dev/null 2>&1", filename);
    system(cmd);
#endif // __APPLE__

    return;
}

void show_random_gif(size_t n, const char ** filenames) {
    assert(n != 0               && "U must pass at less one element");
    assert(filenames != NULL    && "U must pass array");

    if (n == 0) {
        errno = EINVAL;
        return;
    }
    if (filenames == NULL) {
        errno = EINVAL;
        return;
    }

    if (n == 0) {
        errno = EINVAL;
        return;
    }
    if (filenames == NULL) {
        errno = EINVAL;
        return;
    }
    size_t index = ((size_t) random() ) % n;
    show_gif(filenames[index]);

    return;
}

int msleep(long msec) {
    assert(msec >= 0 && "Time cannot be negative");

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    struct timespec ts = {};
    int res = 0;

    ts.tv_sec = msec / 1000;              // miliseconds in one second
    ts.tv_nsec = (msec % 1000) * 1000000; // nanoseconds io one second
    //                    ^----------------- remove int part seconds in ms

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void spinner(string_t str, uint32_t time, uint32_t period) {
    assert(str != NULL);

    if (str == NULL) {
        errno = EINVAL;
        return;
    }

    const char pp[] = {'-', '\\', '|', '/'}; // spinner char
    for (uint32_t i = 0; i < (uint32_t) time/period; ++i) {
        printf("%s", str);
        fflush(stdout);
        printf("%c", pp[i % 4]);
        msleep(period);
        printf("\r");
    }

    return;
}

size_t lines_in_file(FILE * const fp) {
    assert(fp != NULL && "You must pass FILE");

    if (!fp){
        errno = EBADF;
        return 0;
    }

    size_t lines = 1;
    int ch = 0;

    while(!feof(fp)) {
        ch = fgetc(fp);
        if(ch == '\n')
        {
            ++lines;
        }
    }

    if (fseek(fp, 0, SEEK_SET)) {
        errno = EBADF;
        return 0;
    }
    return lines;
}

int clear_stdin_buffer() {
    while (getchar() != '\n')
        continue;
    return 1;
}

int is_stdin_buffer_clean() {
    int c = 0;
    while ((c = getchar()) != '\n') {
        if (c == ' ' || c == '\t')
            continue;
        return 0;
    }
    return 1;
}

double safe_get_double(const char * const var_name) {
    assert(var_name != NULL && "You must pass variable name");

    if (var_name == NULL) {
        errno = EINVAL;
        return -1;
    }

    int scanf_status = 0;
    double var = NAN;
    for (;;) {
        printf("%s", var_name); // TODO: rename
        scanf_status = scanf("%lg", &var);
        if (scanf_status && !isinf(var) && isfinite(var) && is_stdin_buffer_clean())
            return var;
        clear_stdin_buffer();
        // Показываем смешную гифку с котиком
        {
                const char * cat_gifs[] = {"lvl6cat1", "123"};
                show_random_gif(2, (const char **) cat_gifs);
            }
        printf("😢 Вы ввели число, которое я не смог распознать.     \n"
               "Попробуйте ввести правильное число (например 12.34). \n"
               "Десятичный разделитель точка (.).                    \n");
    }
}

int is_user_want_continue(const char * const ask_message) {
    assert(ask_message != NULL && "You must pass message for user");

    if (ask_message == NULL) {
        errno = EINVAL;
        return -1;
    }

    int choice = 0;

    printf("%s", ask_message);
    fflush(stdout);

    do {
        choice = getchar();
    }
    while (choice != 'y' &&
           choice != 'Y' &&
           choice != 'n' &&
           choice != 'N' &&
           choice != '\n');

    if (choice == 'Y' || choice == 'y')
        return 1;
    return 0;
}
