#ifndef DUI_ANIMATION_H
#define DUI_ANIMATION_H

#include <stdbool.h>
#include <raylib.h>

// - Animations

// -- Colors
typedef struct
{
    Color from;
    Color to;
    Color current;
    double start_time;
    double duration;
    bool finished;
} DUI_ColorAnimation;

DUI_ColorAnimation dui_ca_create(Color initial_color);
void dui_ca_start(DUI_ColorAnimation* animation, Color to, double duration);
void dui_ca_update(DUI_ColorAnimation* animation);

#endif //DUI_ANIMATION_H
