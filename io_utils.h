#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "ansi.h"

#define RUNTIME_ERROR_BIG_STRING "\n" \
  " .+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+. \n" \
  "(      ____  _   _ _   _ _____ ___ __  __ _____   _____ ____  ____   ___  ____        )                     \n" \
  " )    |  _ \\| | | | \\ | |_   _|_ _|  \\/  | ____| | ____|  _ \\|  _ \\ / _ \\|  _ \\      (               \n" \
  "(     | |_) | | | |  \\| | | |  | || |\\/| |  _|   |  _| | |_) | |_) | | | | |_) |      )                   \n" \
  " )    |  _ <| |_| | |\\  | | |  | || |  | | |___  | |___|  _ <|  _ <| |_| |  _ <      (                     \n" \
  "(     |_| \\_\\\\___/|_| \\_| |_| |___|_|  |_|_____| |_____|_| \\_\\_| \\_\\\\___/|_| \\_\\      )          \n" \
  " )                                                                                   (                      \n" \
  "(                                                                                     )                     \n" \
  " \"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"+.+\"\n\n"

#define DONT_FORGET_COMMIT_GITHUB()                                                 \
  printf(BOLD(CYAN("\n"                                                             \
    " ____              _ _      __                      _                       \n"\
    "|  _ \\  ___  _ __ ( ) |_   / _| ___  _ __ __ _  ___| |_                    \n"\
    "| | | |/ _ \\| '_ \\|/| __| | |_ / _ \\| '__/ _` |/ _ \\ __|                \n"\
    "| |_| | (_) | | | | | |_  |  _| (_) | | | (_| |  __/ |_                     \n"\
    "|____/ \\___/|_| |_|  \\__| |_|  \\___/|_|  \\__, |\\___|\\__|              \n"\
    "                               _ _      _|___/ _   _   _       _            \n"\
    "  ___ ___  _ __ ___  _ __ ___ (_) |_   / ___(_) |_| | | |_   _| |__         \n"\
    " / __/ _ \\| '_ ` _ \\| '_ ` _ \\| | __| | |  _| | __| |_| | | | | '_ \\    \n"\
    "| (_| (_) | | | | | | | | | | | | |_  | |_| | | |_|  _  | |_| | |_) |       \n"\
    " \\___\\___/|_| |_| |_|_| |_| |_|_|\\__|  \\____|_|\\__|_| |_|\\__,_|_.__/  \n\n"\
  )));

// macro for debug print work only in _DEBUG mode
#ifdef _DEBUG
  #warning "_DEBUG is ENABLED"
  #define DEBUG_PRINT(...)                           \
  fprintf(stderr, __VA_ARGS__); fflush(stderr);
#else
  #define DEBUG_PRINT(...) (void)0
#endif

// macro for print pretty error message with code place with problem
#define ERROR_MSG(format, ...)                                                  \
    fprintf(stderr,                                                             \
            "In " GREEN("%s:%d") ", " YELLOW("%s") ".\n" format,                \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); perror("");


void show_gif(const char * const filename);

void show_random_gif(size_t n, const char ** filenames);

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec);

void spinner(const char * const str, uint32_t time, uint32_t period);

size_t lines_in_file(FILE *fp);

/**
  @brief Функция отчистки буфера ввода
  Считывает данные из потока ввода до \\n (\n также будет считан)

  \code{cpp}
    int a = 0;
    a = getchar();

    clear_stdin_buffer();
    // Теперь буфер чист
  \endcode

  @return int 1 если буфер отчищен
 */
int clear_stdin_buffer();

/**
  @brief Функция проверки чист ли буфер ввода (очищает буфер если он чист)
  Считывает данные из потока ввода до \\n (\n также будет считан)
  Если считывается ' ' или '\t' - то считается что буфер чист
  Если считывается любое другое, считается что буфер не чист


  \code{cpp}
    int a = 0, status = 0;
    a = getchar();

    if (is_stdin_buffer_clean())
        // В буфере были только ' ' и '\t' теперь чист
    else
        // В буфере есть что-то еще, он остался не чист
  \endcode

  @return int 1 если буфер содержал только ' ' или '\t' или уже был чист
  @return int 0 если буфер содержит что-то помимо пустоты
 */
int is_stdin_buffer_clean();

double safe_get_double(const char * const var_name);

int is_user_want_continue(const char * const ask_message);

ssize_t file_byte_size(const char * const filename);

// func is allocate buffer, don't forgot free return value
char * read_file_to_buf(const char * const filename, size_t * const buf_len);

#endif // IO_UTILS_H
