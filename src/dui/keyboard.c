#include <dui/environment.h>
#include <dui/keyboard.h>

#include "raylib.h"

bool dui_is_key_down_impl(const int key, DUI_IsKeyDownData data)
{
    if (!data.state)
    {
        return IsKeyDown(key);
    }

    if (data.initialDelay == 0.0f)
    {
        data.initialDelay = 0.5f;
    }
    if (data.delay == 0.0f)
    {
        data.delay = 0.075f;
    }

    if (IsKeyDown(key)) {
        const double now = GetTime();
        if (*data.state <= 0.f) {
            *data.state = now + (data.initialDelay - data.delay);
            return true;
        }
        if (now - *data.state >= data.delay) {
            *data.state = now;
            return true;
        }
    } else {
        *data.state = -1.f;
    }
    return false;
}

void dui_handle_global_keys()
{
    const DUI_Environment* env = dui_env();

    static float tabKeyDownState = 0.0f;
    if (dui_is_key_down(KEY_TAB, .state = &tabKeyDownState))
    {
        if (env->focused_element != NULL)
        {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            {
                dui_env_focus(env->focused_element->tabOrderPrev);
            } else
            {
                dui_env_focus(env->focused_element->tabOrderNext);
            }
        }
        else
        {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            {
                dui_env_focus(dui_env()->root_context.tabOrderBack);
            }
            else
            {
                dui_env_focus(dui_env()->root_context.tabOrderFront);
            }
        }
    }
}