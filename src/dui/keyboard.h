#ifndef DUI_KEYBOARD_H
#define DUI_KEYBOARD_H

#include <stdbool.h>

typedef struct
{
    float initialDelay;
    float delay;
    float *state;

} DUI_IsKeyDownData;

#define dui_is_key_down(key, ...) \
    dui_is_key_down_impl(key, (DUI_IsKeyDownData) { __VA_ARGS__ })
bool dui_is_key_down_impl(int key, DUI_IsKeyDownData data);

void dui_handle_global_keys();

#endif // DUI_KEYBOARD_H
