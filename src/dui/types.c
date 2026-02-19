#include <raylib.h>

#include <dui/environment.h>
#include <dui/types.h>

DUI_NextState dui_next_state(const DUI_State current_state, const Rectangle bounds, const bool disabled, DUI_Element* element)
{
    DUI_NextState next_state = {
        .activated = false,
        .state = current_state,
    };

    if (disabled)
    {
        next_state.state = STATE_DISABLED;
        return next_state;
    }

    const bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        if (current_state == STATE_DOWN && hovered)
        {
            next_state.activated = true;
        }

        next_state.state = STATE_NORMAL;
    }

    if (hovered)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            next_state.state = STATE_DOWN;
            dui_env_focus(element);
        }
        else if (current_state != STATE_HOVER && current_state != STATE_DOWN)
        {
            next_state.state = STATE_HOVER;
        }
    }
    else
    {
        if (current_state == STATE_HOVER)
        {
            next_state.state = STATE_NORMAL;
        }
    }

    if (dui_env_has_focus(element) && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)))
    {
        next_state.activated = true;
    }

    return next_state;
}
