#ifndef _TERMUTILS_H
#define _TERMUTILS_H

#include <time.h>

/**
 * Platform independent function to get the terminal size.
 *
 * \param *rows Return variable for the number of rows in the terminal.
 * \param *cols Return variable for the number of columns in the terminal.
 */
void get_terminal_size(int *rows, int *cols);

/**
 * Get the time delta between toc and tic.
 *
 * \param tic Start time in timespec.
 * \param toc End time as timespec.
 * \returns time delta in milliseconds.
 */
long gf_timediff_ms(struct timespec tic, struct timespec toc);

/**
 * Platform independent sleep function.
 *
 * \param sleep time in milliseconds.
 */
void gf_sleep(int sleep_ms);

/**
 * Wait function to sync with a target FPS.
 *
 * \param target_ms Amount of milliseconds between frames, calculated as 1/fps * 1000.
 * \param tic Start time of the game loop processing.
 * \param toc End time of the game loop processing.
 */
void gf_delay(int target_ms, struct timespec tic, struct timespec toc);

#endif
