/**
 * @file graphics.c
 * @date 2024-12-13
 * @author Alessandro Bridi [ale.bridi15@gmail.com]
 *
 * @brief Graphics handling functions
 */

#include "graphics.h"
#include "text.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void _draw_text_box(struct Box *box, draw_pixel_callback_t draw_pixel, draw_rectangle_callback_t draw_rectangle)
{
#if GRAPHICS_OPT
    if (!box->updated) return;
#endif
    uint32_t bg_color = box->default_bg_color;
    uint32_t fg_color = box->default_fg_color;

    // In this block colors are selected based on thresholds selected for value (if any)
    if (box->value && box->value->colors)
    {
        for (int i=0; i<box->value->colors_num; i++)
        {
            if (box->value->colors[i].min < box->value->value && box->value->value < box->value->colors[i].max)
            {
                bg_color = box->value->colors[i].bg_color;
                fg_color = box->value->colors[i].fg_color;
            }
        }
    }

    // Draw the basic rectangle
    draw_rectangle(box->rect.x, box->rect.y, box->rect.w, box->rect.h, bg_color);
    if (box->value)
    {
        // Format the value accordingly to what we want
        char buf[15];
        if (box->value->is_float)
        {
            snprintf(buf, sizeof(buf), "%.2f", box->value->value);
        } else
        {
            snprintf(buf, sizeof(buf), "%d", (int) box->value->value);
        }
        // Plot the value
        draw_text(box->rect.x + box->value->pos.x,
                   box->rect.y + box->value->pos.y,
                   box->value->align,
                   buf, fg_color,
                   box->value->font_size,
                   draw_pixel);
    }

    if (box->label)
    {
        // Plot the label
        draw_text(box->rect.x + box->label->pos.x,
                   box->rect.y + box->label->pos.y,
                   box->label->align,
                   box->label->text,
                   fg_color,
                   box->label->font_size,
                   draw_pixel);
    }
}


void render_interface(struct Box *text_boxes, uint16_t num,
                      draw_pixel_callback_t draw_pixel,
                      draw_rectangle_callback_t draw_rectangle
#if GRAPHICS_OPT == 0
                      ,clear_screen_callback_t clear_screen
#endif
                      )
{
// Do not clear full screen for max optimization (less time spent)
#if GRAPHICS_OPT == 0
    clear_screen();
#endif
    for (int i = 0; i < num; i++)
    {
        _draw_text_box(text_boxes + i, draw_pixel, draw_rectangle);
    }
}


uint8_t get_alpha(uint32_t color)
{
    return (color >> 24) & 0xff;
}


uint8_t get_red(uint32_t color)
{
    return (color >> 16) & 0xff;
}


uint8_t get_green(uint32_t color)
{
    return (color >> 8) & 0xff;
}


uint8_t get_blue(uint32_t color)
{
    return color & 0xff;
}


struct Box *get_box(struct Box *boxes, uint16_t num, uint16_t id)
{
    // Loops and search for IDs (can be good for CAN IDs)
    for (int i=0; i<num; i++) {
        if ((boxes + i)->id == id) {
            return (boxes + i);
        }
    }
    return NULL;
}


struct Label *create_label(char *text, struct Coords pos, float font_size, enum FontAlign align)
{
    struct Label *label = malloc(sizeof(struct Label));
    if (label)
    {
        label->text = text;
        label->pos = pos;
        label->font_size = font_size;
        label->align = align;
    }
    return label;
}


struct Value *create_value(float val, bool is_float, struct Coords pos, float font_size, enum FontAlign align, struct ColorRange *colors, uint8_t colors_num)
{
    struct Value *value = malloc(sizeof(struct Value));
    if (value)
    {
        value->value = val;
        value->is_float = is_float;
        value->pos = pos;
        value->font_size = font_size;
        value->align = align;
        value->colors = colors;
        value->colors_num = colors_num;
    }
    return value;
}


void free_boxes(struct Box *boxes, uint16_t num)
{
    for (int i=0; i<num; i++)
    {
        struct Box *box = boxes + i;
        if (box)
        {
            struct Label *label = box->label;
            if (label)
            {
                free(label);
            }
            struct Value *value = box->value;
            if (value)
            {
                free(value);
            }
        }
    }
}
