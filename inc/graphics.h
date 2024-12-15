#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <mcufont.h>
#include <stddef.h>

struct Rect {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
};

struct GraphicsAPI {
    void (*draw_pixel)(int x, int y, uint32_t color);
    void (*draw_rectangle)(int x, int y, int w, int h, uint32_t color);
};

struct Widget {
    struct Rect rect;
    uint32_t bg_color;
    uint32_t fg_color;
    const char *text;
};

void init_graphics_api(struct GraphicsAPI *a);
void draw_text_box(struct Rect rect, uint32_t bg_color, uint32_t fg_color, const char *text);
void render_interface(const struct Widget *widgets, uint16_t num);
uint8_t get_alpha(uint32_t color);
uint8_t get_red(uint32_t color);
uint8_t get_green(uint32_t color);
uint8_t get_blue(uint32_t color);

#endif // GRAPHICS_H
