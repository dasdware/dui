#include <dui/animation.h>

#include "raymath.h"

DUI_ColorAnimation dui_ca_create(const Color initial_value) {
    return (DUI_ColorAnimation){
        .from = initial_value,
        .to = initial_value,
        .current = initial_value,
        .finished = true,
    };
}

void dui_ca_start(DUI_ColorAnimation* animation, const Color to, const double duration) {
    animation->from = animation->current;
    animation->to = to;
    animation->start_time = GetTime();
    animation->duration = duration;
    animation->finished = false;
}

void dui_ca_update(DUI_ColorAnimation* animation) {
    if (animation->finished) {
        return;
    }

    const double current_time = GetTime();
    if (current_time > animation->start_time + animation->duration) {
        animation->finished = true;
        return;
    }

    const double t = (current_time - animation->start_time) / animation->duration;
    animation->current = ColorLerp(animation->from, animation->to, t);
}

DUI_FloatAnimation dui_fa_create(const float initial_value) {
    return (DUI_FloatAnimation){
        .from = initial_value,
        .to = initial_value,
        .current = initial_value,
        .finished = true,
    };
}

void dui_fa_start(DUI_FloatAnimation* animation, const float to, const double duration) {
    animation->from = animation->current;
    animation->to = to;
    animation->start_time = GetTime();
    animation->duration = duration;
    animation->finished = false;
}

void dui_fa_update(DUI_FloatAnimation* animation) {
    if (animation->finished) {
        return;
    }

    const double current_time = GetTime();
    if (current_time > animation->start_time + animation->duration) {
        animation->finished = true;
        return;
    }

    const double t = (current_time - animation->start_time) / animation->duration;
    animation->current = Lerp(animation->from, animation->to, t);
}
