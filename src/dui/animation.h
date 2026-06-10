#ifndef DUI_ANIMATION_H
#define DUI_ANIMATION_H

#include <stdbool.h>
#include <raylib.h>

typedef struct {
    Color from;
    Color to;
    Color current;
    double start_time;
    double duration;
    bool finished;
} DUI_ColorAnimation;

DUI_ColorAnimation dui_ca_create(Color initial_value);
void dui_ca_start(DUI_ColorAnimation* animation, Color to, double duration);
void dui_ca_update(DUI_ColorAnimation* animation);

typedef struct {
    float from;
    float to;
    float current;
    double start_time;
    double duration;
    bool finished;
} DUI_FloatAnimation;

DUI_FloatAnimation dui_fa_create(float initial_value);
void dui_fa_start(DUI_FloatAnimation* animation, float to, double duration);
void dui_fa_update(DUI_FloatAnimation* animation);

#endif //DUI_ANIMATION_H
