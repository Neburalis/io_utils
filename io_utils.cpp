#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

#include "io_utils.h"

void show_gif(const char * const filename) {
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

#ifdef __APPLE__ // show_gif работает только на MACOS
    size_t index = arc4random_uniform((uint32_t)n);
    show_gif(filenames[index]);
#endif // __APPLE__
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

int nsleep(size_t nsec) {
    struct timespec ts = {};
    int res = 0;

    ts.tv_sec = nsec / 1000000000ULL;              // miliseconds in one second
    ts.tv_nsec = (nsec % 1000000000ULL);           // nanoseconds io one second
    //                       ^----------------- remove int part seconds in ms

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void spinner(const char * const str, uint32_t time, uint32_t period) {
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
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        continue;
    return 1;
}

int is_stdin_buffer_clean() {
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (c == ' ' || c == '\t')
            continue;
        return 0;
    }
    return 1;
}

double safe_get_double(const char * const prompt) {
    assert(prompt != NULL && "You must pass variable name");

    if (prompt == NULL) {
        errno = EINVAL;
        return -1;
    }

    int scanf_status = 0;
    double var = NAN;
    for (;;) {
        printf("%s", prompt);
        scanf_status = scanf("%lg", &var);
        if (scanf_status == EOF) {
            fprintf(stderr, "Ввод завершен. Завершение программы.\n");
            return NAN;
        }
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

ssize_t file_byte_size(const char * const filename) {
    assert(filename != NULL && "U must provide valid filename");

    if (filename == NULL) {
        errno = EINVAL;
        return -2;
    }

    struct stat file_info = {};

    if (stat(filename, &file_info) == -1) {
        ERROR_MSG("Не удалось получить информацию о файле %s\n", filename);
        return -1;
    }

    return (ssize_t) file_info.st_size;
}

char * read_file_to_buf(const char * const filename, size_t * const buf_len) {
    assert(filename != NULL && "You must provide a valid filename");
    assert(buf_len != NULL  && "You must provide a valid pointer to buf_len");

    if (filename == NULL) {
        errno = EINVAL;
        return NULL;
    }
    if (buf_len == NULL) {
        errno = EINVAL;
        return NULL;
    }

    FILE * fp = fopen(filename, "rb");
    if (!fp) {
        // errno уже установлен fopen()
        return NULL;
    }

    ssize_t byte_len = file_byte_size(filename);
    if (byte_len < 0) {
        // Ошибка уже установлена в file_byte_size()
        fclose(fp);
        return NULL;
    }
    if (byte_len == 0) {
        // Пустой файл — допустимый случай
        fclose(fp);
    }

    // Проверка на переполнение: byte_len + 1 не должно превышать SIZE_MAX
    if ((size_t)byte_len > SIZE_MAX - 1) {
        fclose(fp);
        errno = ENOMEM; // или EOVERFLOW, но ENOMEM уместнее при аллокации
        return NULL;
    }

    size_t alloc_size = (size_t)byte_len + 1;

    char * buf = (char *) calloc(alloc_size, sizeof(char));
    if (buf == NULL) {
        fclose(fp);
        return NULL;
    }

    if (byte_len > 0) {
        fread(buf, 1, (size_t)byte_len, fp);
        if (ferror(fp)) {
            free(buf);
            fclose(fp);
            return NULL;
        }
    }
    // Для пустого файла nread остаётся 0 — это нормально

    fclose(fp);

    // Убеждаемся, что буфер завершён нулём (calloc уже обнулил, но на всякий случай)
    buf[byte_len] = '\0';

    // Возвращаем **полный размер выделенного буфера**, включая '\0'
    *buf_len = alloc_size;

    return buf;
}

size_t * read_file_to_size_t_buf(const char *filename, size_t *count) {
    // Проверки аргументов
    if (filename == NULL || count == NULL) {
        errno = EINVAL;
        return NULL;
    }
    *count = 0;

    // Открываем файл в бинарном режиме
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return NULL; // errno установлен fopen()
    }

    // Получаем размер файла
    struct stat st;
    if (fstat(fileno(fp), &st) != 0) {
        fclose(fp);
        return NULL; // errno установлен fstat()
    }

    long long file_size = st.st_size;

    // Проверка: файл должен быть кратен sizeof(size_t)
    if ((unsigned long long)file_size % sizeof(size_t) != 0) {
        fclose(fp);
        errno = EINVAL; // повреждённый или не тот формат
        return NULL;
    }

    // Проверка: не превышает ли размер SSIZE_MAX (защита от переполнения при приведении)
    if (file_size > SSIZE_MAX) {
        fclose(fp);
        errno = EFBIG;
        return NULL;
    }

    size_t total_bytes = (size_t)file_size;
    size_t n = total_bytes / sizeof(size_t);

    // Выделяем память (malloc, не calloc — данные бинарные, обнуление не нужно)
    size_t *buf = (size_t *)malloc(total_bytes);
    if (!buf) {
        fclose(fp);
        return NULL; // errno = ENOMEM (установлен malloc)
    }

    // Читаем весь файл за один раз
    size_t nread = fread(buf, 1, total_bytes, fp);
    if (nread != total_bytes) {
        int saved_errno = ferror(fp) ? errno : EIO;
        free(buf);
        fclose(fp);
        errno = saved_errno;
        return NULL;
    }

    fclose(fp);
    *count = n;
    return buf;
}

int create_folder_if_not_exists(const char *dir_path) {
    assert(dir_path != NULL);

    if (dir_path == NULL) {
        errno = EINVAL;
        return 0;
    }

    if (mkdir(dir_path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
        if (errno == EEXIST) {
            // Directory already exists.
            return 0;
        } else {
            perror("Error creating directory");
            return 1; // Indicate an error
        }
    } else {
        // Directory created successfully.
        return 0;
    }
}