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

    const DUI_Environment* env = dui_env();

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
        if (data.disabled)
        {
            element->background = dui_ca_create(DUI_BUTTON_BG_DISABLED(data.kind));
            element->foreground = dui_ca_create(DUI_BUTTON_FG_DISABLED(data.kind));
            element->state = STATE_DISABLED;
        }
        else
        {
            element->background = dui_ca_create(DUI_BUTTON_BG(data.kind));
            element->foreground = dui_ca_create(DUI_BUTTON_FG(data.kind));
            element->state = STATE_NORMAL;
        }
    }

    dui_ca_update(&element->background);
    dui_ca_update(&element->foreground);

    const DUI_NextState next_state = dui_next_state(element->state,
                                                    bounds_data.bounds, data.disabled,
                                                    &element->element);
    if (next_state.state != element->state)
    {
        element->state = next_state.state;
        switch (element->state)
        {
        case STATE_NORMAL:
            dui_ca_start(&element->background, DUI_BUTTON_BG(data.kind), 0.1);
            dui_ca_start(&element->foreground, DUI_BUTTON_FG(data.kind), 0.1);
            break;
        case STATE_HOVER:
            dui_ca_start(&element->background, DUI_BUTTON_BG_HOVER(data.kind), 0.1);
            dui_ca_start(&element->foreground, DUI_BUTTON_FG(data.kind), 0.1);
            break;
        case STATE_DOWN:
            dui_ca_start(&element->background, DUI_BUTTON_BG_DOWN(data.kind), 0.1);
            dui_ca_start(&element->foreground, DUI_BUTTON_FG(data.kind), 0.1);
            break;
        case STATE_DISABLED:
            dui_ca_start(&element->background, DUI_BUTTON_BG_DISABLED(data.kind), 0.1);
            dui_ca_start(&element->foreground, DUI_BUTTON_FG_DISABLED(data.kind), 0.1);
            break;
        }
    }

    DrawRectangleRec(bounds_data.bounds, element->background.current);
    if (!data.disabled && dui_env_has_focus(element))
    {
        dui_env_draw_focus_frame(bounds_data.bounds, DUI_FOCUS_COLOR(data.kind));
    }

    if (data.caption)
    {
        dui_text(
            data.caption,
            bounds_data.bounds,
            .horizontal_alignment = ALIGN_CENTER,
            .vertical_alignment = ALIGN_CENTER,
            .offset_y = (element->state == STATE_DOWN) ? 1 : 0,
            .color = element->foreground.current,
        );
    }

    return next_state.activated;
}
