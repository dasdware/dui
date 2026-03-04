#include <dui/button.h>
#include <dui/environment.h>

#include <dui/layout.h>
#include <dui/text.h>

bool dui_button_impl(const int id, const DUI_ButtonData data)
{
    static int element_type_button = 0;
    if (element_type_button == 0)
    {
        element_type_button = dui_env_next_element_type();
    }

    const DUI_Environment* env = dui_env();

    const int text_width = dui_text_measure(data.caption).x;
    const int padding = dui_env_spacing(1);
    const int preferred_width = text_width + 4 * padding;
    const int preferred_height = env->font_height + 2 * padding;

    DUI_ButtonElement* element;
    if (dui_ctx_active_element_by_id(
        element_type_button,
        id,
        data.kind,
        data.disabled,
        element,
        .width = preferred_width,
        .height = preferred_height,
        // TODO: Which layout fields should be forwarded and how
        .anchor = data.anchor,
        .opposite = data.opposite,
        .remaining = data.remaining,
    ))
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

    const DUI_NextState next_state = dui_ctx_next_state(element->state,
                                                    element->element.bounds, data.disabled,
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

    DrawRectangleRec(element->element.bounds, element->background.current);

    if (data.caption)
    {
        dui_text(
            data.caption,
            element->element.bounds,
            .horizontal_alignment = ALIGN_CENTER,
            .vertical_alignment = ALIGN_CENTER,
            .offset_y = (element->state == STATE_DOWN) ? 1 : 0,
            .color = element->foreground.current,
        );
    }

    return next_state.activated;
}
