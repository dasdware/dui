#include <dui/button.h>
#include <dui/environment.h>

#include <dui/layout.h>
#include <dui/text.h>

bool dui_button_impl(const int id, const DUI_ButtonData data)
{
    static int element_kind_button = 0;
    if (element_kind_button == 0)
    {
        element_kind_button = dui_env_next_element_kind();
    }

    const DUI_Environment *env = dui_env();

    const int text_width = dui_text_measure(data.caption).x;
    const int padding = dui_env_spacing(1);
    const int preferred_width = text_width + 4 * padding;
    const int preferred_height = env->font_height + 2 * padding;

    const DUI_Layout_BoundsData bounds_data = dui_lay_rectangle(
        .width = preferred_width,
        .height = preferred_height,
        // TODO: Which layout fields should be forwarded and how
        .anchor = data.anchor,
        .opposite = data.opposite,
        .remaining = data.remaining,
    );

    DUI_ButtonElement* element;
    if (dui_ctx_element_by_id(element_kind_button, id, bounds_data.tabOrderBack, data.disabled, element))
    {
        element->background = dui_ca_create(dui_env_color(data.kind, ENV_SHADE_NORMAL));
        element->state = BUTTON_NORMAL;
    }

    if (data.disabled)
    {
        dui_ca_start(&element->background, dui_env_color(ENV_DEFAULT, ENV_SHADE_NORMAL), 0.1);
    }

    dui_ca_update(&element->background);

    bool result = false;
    if (!data.disabled)
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            if (element->state == BUTTON_DOWN)
            {
                result = true;
            }

            element->state = BUTTON_NORMAL;
            dui_ca_start(&element->background, dui_env_color(data.kind, ENV_SHADE_NORMAL), 0.1);
        }

        if (CheckCollisionPointRec(GetMousePosition(), bounds_data.bounds))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                element->state = BUTTON_DOWN;
                dui_ca_start(&element->background, dui_env_color(data.kind, ENV_SHADE_DOWN), 0.1);
                dui_env_focus(element);
            }
            if (element->state != BUTTON_HOVER && element->state != BUTTON_DOWN)
            {
                element->state = BUTTON_HOVER;
                dui_ca_start(&element->background, dui_env_color(data.kind, ENV_SHADE_HOVER), 0.1);
            }
        }
        else
        {
            if (element->state == BUTTON_HOVER)
            {
                element->state = BUTTON_NORMAL;
                dui_ca_start(&element->background, dui_env_color(data.kind, ENV_SHADE_NORMAL), 0.1);
            }
        }
    }

    DrawRectangleRec(bounds_data.bounds, element->background.current);
    if (!data.disabled && dui_env_has_focus(element))
    {
        dui_env_draw_focus_frame(bounds_data.bounds, dui_env_color(data.kind, ENV_SHADE_FOCUS_FRAME));
        if (IsKeyPressed(KEY_SPACE) ||IsKeyPressed(KEY_ENTER))
        {
            result = true;
        }
    }

    if (data.caption)
    {
        dui_text(
            data.caption,
            bounds_data.bounds,
            .horizontal_alignment = ALIGN_CENTER,
            .vertical_alignment = ALIGN_CENTER,
            .offset_y = (element->state == BUTTON_DOWN) ? 1 : 0,
            .color = dui_env_color(data.kind, data.disabled ? ENV_SHADE_DISABLED_TEXT : ENV_SHADE_TEXT));
    }

    return result;
}
