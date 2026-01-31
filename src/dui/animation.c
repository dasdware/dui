#include <dui/animation.h>

DUI_ColorAnimation dui_ca_create(const Color initial_color)
{
    return (DUI_ColorAnimation) {
        .from = initial_color,
        .to = initial_color,
        .current = initial_color,
        .finished = true,
    };
}

void dui_ca_start(DUI_ColorAnimation* animation, const Color to, const double duration)
{
    animation->from = animation->current;
    animation->to = to;
    animation->start_time = GetTime();
    animation->duration = duration;
    animation->finished = false;
}

void dui_ca_update(DUI_ColorAnimation* animation)
{
    if (animation->finished)
    {
        return;
    }

    const double current_time = GetTime();
    if (current_time > animation->start_time + animation->duration)
    {
        animation->finished = true;
        return;
    }

    const double t = (current_time - animation->start_time) / animation->duration;
    animation->current = ColorLerp(animation->from, animation->to, t);
}