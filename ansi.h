#ifndef ANSI_H
#define ANSI_H

#define ANSI_COLOR_RED     "\e[31m"
#define ANSI_COLOR_GREEN   "\e[32m"
#define ANSI_COLOR_YELLOW  "\e[33m"
#define ANSI_COLOR_BLUE    "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN    "\e[36m"
#define ANSI_COLOR_WHITE   "\e[37m"
#define ANSI_COLOR_DEFAULT "\e[39m"
#define ANSI_COLOR_RESET   "\e[0m"

#define ANSI_BOLD          "\e[1m"
#define ANSI_BOLD_RESET    "\e[22m"

#define RED(string)        ANSI_COLOR_RED     string ANSI_COLOR_RESET
#define GREEN(string)      ANSI_COLOR_GREEN   string ANSI_COLOR_RESET
#define YELLOW(string)     ANSI_COLOR_YELLOW  string ANSI_COLOR_RESET
#define BLUE(string)       ANSI_COLOR_BLUE    string ANSI_COLOR_RESET
#define MAGENTA(string)    ANSI_COLOR_MAGENTA string ANSI_COLOR_RESET
#define CYAN(string)       ANSI_COLOR_CYAN    string ANSI_COLOR_RESET

#define BOLD(string)       ANSI_BOLD          string ANSI_BOLD_RESET

#endif // ANSI_H