#ifndef ROTATING_CUBE_H
#define ROTATING_CUBE_H

#define MAX(a, b) ((a) > (b) ? a : b)
#define MIN(a, b) ((a) < (b) ? a : b)

/* Spacial vector */
typedef struct {
    float x;
    float y;
    float z;
} vec3;

/* Rotation vector */
typedef struct {
    float a; // alpha
    float b; // beta
    float c; // gamma
} rot3;

typedef struct {
    int width;
    int height;
    int char_width;
    int char_height;
    int distance;
    int idx;
    int background;
    int target_fps;
    int target_ms;
    float K1;
    float cube_size;
    float inc;
    float *z_buffer;
    char *buffer;
    rot3 rot;
} Context;

/**
 * Initialize the varables in the context.
 *
 * \param *ctx the context
 */
void setup(Context *ctx);

/**
 * Update the game state.
 *
 * \param *ctx the context
 */
void update(Context *ctx);

/**
 * Draw the game state.
 *
 * \param *ctx the context
 */
void draw(Context *ctx);

/**
 * Free the memory allocated in the context.
 * Note that this function does not take the context as an argument, since it
 * is passed to the function atexit(), which does not allow arguments.
 * Therefore, the context must be declared globally.
 */
void teardown(void);

/**
 * Get the terminal size in rows and cols.
 *
 * \param *rows return value for rows
 * \param *cols return value for cols
 */
void get_terminal_size(int *rows, int *cols);

float calc_x(vec3 v, rot3 r);

float calc_y(vec3 v, rot3 r);

float calc_z(vec3 v, rot3 r);

void calculate_surface(Context *ctx, vec3 point, rot3 rot, char ch);

#endif /* ROTATING_CUBE_H */
