#include "rotating_cube.h"
#include "gfutils.h"
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Context ctx;

void setup(Context *ctx) {
    get_terminal_size(&ctx->height, &ctx->width);
    int minsize = MIN(ctx->width, ctx->height);
    ctx->char_width = 2.0;
    ctx->char_height = 1.0;
    ctx->cube_size = (float)minsize / 2.0f;
    ctx->K1 = 2.0 * ctx->cube_size;
    ctx->inc = 0.8;
    ctx->target_fps = 60;
    ctx->target_ms = (int)round(1000.0 / ctx->target_fps);
    ctx->distance = 5 * (int)ctx->cube_size;
    ctx->buffer = (char *)malloc(ctx->width * ctx->height * sizeof(char));
    ctx->z_buffer = (float *)malloc(ctx->width * ctx->height * sizeof(float));
    ctx->background = ' ';
    ctx->rot.a = 0.0;
    ctx->rot.b = 0.0;
    ctx->rot.a = 0.0;
    printf("ctx.width: %d, ctx.height: %d, cube_size: %f\n", ctx->width, ctx->height, ctx->cube_size);
    printf("ctx.K1: %f, ctx.distance: %d\n", ctx->K1, ctx->distance);
    /* Clear screen */
    printf("\x1b[2J");
    /* Hide cursor. */
    printf("\033[?25l");
}

void update(Context *ctx) {
    memset(ctx->buffer, ctx->background, ctx->width * ctx->height * sizeof(char));
    memset(ctx->z_buffer, 0, ctx->width * ctx->height * sizeof(float));
    for (float cube_x = -ctx->cube_size; cube_x < ctx->cube_size; cube_x += ctx->inc) {
        for (float cube_y = -ctx->cube_size; cube_y < ctx->cube_size; cube_y += ctx->inc) {
            calc_uv(ctx, (vec3){cube_x, cube_y, -ctx->cube_size}, ctx->rot, '@');
            calc_uv(ctx, (vec3){cube_x, cube_y, ctx->cube_size}, ctx->rot, '#');
            calc_uv(ctx, (vec3){ctx->cube_size, cube_y, cube_x}, ctx->rot, 'X');
            calc_uv(ctx, (vec3){-ctx->cube_size, cube_y, cube_x}, ctx->rot, '~');
            calc_uv(ctx, (vec3){cube_x, -ctx->cube_size, cube_y}, ctx->rot, ';');
            calc_uv(ctx, (vec3){cube_x, ctx->cube_size, cube_y}, ctx->rot, '+');
        }
    }
    ctx->rot.a += 0.008;
    ctx->rot.b -= 0.010;
    ctx->rot.c += 0.012;
}

void draw(Context *ctx) {
    /* Move cursor to top left. */
    printf("\x1b[H");
    /* Draw buffer. */
    for (int j = 0; j < ctx->height; j++) {
        for (int i = 0; i < ctx->width; i++) {
            const int k = j * ctx->width + i;
            putchar(ctx->buffer[k]);
        }
        if (j < ctx->height - 1) {
            putchar('\n');
        }
    }
}

void teardown(void) {
    free(ctx.buffer);
    free(ctx.z_buffer);
    ctx.buffer = NULL;
    ctx.z_buffer = NULL;
    /* Reset colors and cursor. */
    printf("\033[0m\033[?25h\n");
    printf("\nBye...\n");
}

float calc_x(vec3 v, rot3 r) {
    return v.x * (cosf(r.a) * cosf(r.b)) + v.y * (cosf(r.a) * sinf(r.b) * sinf(r.c) - sinf(r.a) * cosf(r.c)) +
           v.z * (sinf(r.a) * sinf(r.c) + cosf(r.a) * sinf(r.b) * cosf(r.c));
}

float calc_y(vec3 v, rot3 r) {
    return v.x * (sinf(r.a) * cosf(r.b)) + v.y * (cosf(r.a) * cosf(r.c) + sinf(r.a) * sinf(r.b) * sinf(r.c)) +
           v.z * (sinf(r.a) * sinf(r.b) * cosf(r.c) - cosf(r.a) * sinf(r.c));
}

float calc_z(vec3 v, rot3 r) {
    return v.x * (-sinf(r.b)) + v.y * (cosf(r.b) * sinf(r.c)) + v.z * (cosf(r.b) * cosf(r.c));
}

void calc_uv(Context *ctx, vec3 point, rot3 rot, char ch) {
    const float x = calc_x(point, rot);
    const float y = calc_y(point, rot);
    const float z = calc_z(point, rot) + ctx->distance;
    const float ooz = 1.0 / z;
    const int col = (int)roundf(ctx->width / 2.0 + ctx->K1 * ooz * x * ctx->char_width);
    const int row = (int)roundf(ctx->height / 2.0 + ctx->K1 * ooz * y * ctx->char_height);
    const int k = col + row * ctx->width;
    if (k >= 0 && k < ctx->width * ctx->height) {
        if (ooz > ctx->z_buffer[k]) {
            ctx->z_buffer[k] = ooz;
            ctx->buffer[k] = ch;
        }
    }
}

int main(int argc, char *argv[]) {
    atexit(teardown);
    signal(SIGINT, exit);
    signal(SIGTERM, exit);
    setup(&ctx);
    struct timespec tic = {0, 0};
    struct timespec toc = {0, 0};
    struct timespec end = {0, 0};
    /* Game loop. Exit by pressing [ctl]-[c] */
    while (true) {
        clock_gettime(CLOCK_MONOTONIC, &tic);
        update(&ctx);
        draw(&ctx);
        clock_gettime(CLOCK_MONOTONIC, &toc);
        gf_delay(ctx.target_ms, tic, toc);
    }
    return 0;
}
