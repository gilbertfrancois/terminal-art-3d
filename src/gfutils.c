#if defined(_WIN32)
#include <Windows.h>
#else
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#endif
#include "gfutils.h"

void get_terminal_size(int *rows, int *cols) {
    int term_rows = 0;
    int term_cols = 0;
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    term_cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    term_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#elif defined(TIOCGSIZE)
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    term_cols = ts.ts_cols;
    term_rows = ts.ts_lines;
#elif defined(WIOCGETD)
    struct uwdata w;
    ioctl(STDOUT_FILENO, WIOCGETD, &w);
    if (w.uw_vs) {
        term_rows = w.uw_height / w.uw_vs;
    }
    if (w.uw_hs) {
        term_cols = w.uw_width / w.uw_hs;
    }
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    term_cols = ts.ws_col;
    term_rows = ts.ws_row;
#endif
    if (term_rows != 0 && term_cols != 0) {
        *rows = term_rows;
        *cols = term_cols;
    }
}

long gf_timediff_ms(struct timespec tic, struct timespec toc) {
    long tic_ms = (tic.tv_sec * 1000) + (tic.tv_nsec / 1000000);
    long toc_ms = (toc.tv_sec * 1000) + (toc.tv_nsec / 1000000);
    return toc_ms - tic_ms;
}

void gf_sleep(int sleep_ms) {
#if defined(_WIN32)
    Sleep(sleep_ms);
#else
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = sleep_ms * 1000000;
    nanosleep(&delay, NULL);
#endif
}

void gf_delay(int target_ms, struct timespec tic, struct timespec toc) {
    long elapsed_ms = gf_timediff_ms(tic, toc);
    int sleep_ms = (int)(target_ms - elapsed_ms);
    if (sleep_ms > 0) {
        gf_sleep(sleep_ms);
    }
}
