#include <dui/checkbox.h>
#include <dui/environment.h>
#include <dui/text.h>

bool dui_checkbox_impl(const int id, const DUI_CheckboxData data) {
    static int element_type_checkbox = 0;
    if (element_type_checkbox == 0) {
        element_type_checkbox = dui_env_next_element_type();
    }

    const DUI_Environment* env = dui_env();

    const int text_width = dui_text_measure(data.caption).x;
    const int padding = dui_env_spacing(1);
    const int preferred_width = text_width + 4 * padding;
    const int preferred_height = env->font_height + 2 * padding;

    DUI_CheckboxElement* element;
    if (dui_ctx_active_element_by_id(
        element_type_checkbox,
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
    )) {
        if (data.disabled) {
            element->text_color = dui_ca_create(DUI_CHECKBOX_TEXT_DISABLED(data.kind));
            element->box_color = dui_ca_create(DUI_CHECKBOX_BOX_DISABLED(data.kind));
            element->state = STATE_DISABLED;
        } else {
            element->text_color = dui_ca_create(DUI_CHECKBOX_TEXT(data.kind));
            element->box_color = dui_ca_create(DUI_CHECKBOX_BOX(data.kind));
            element->state = STATE_NORMAL;
        }
    }
    element->element.bounds = element->element.bounds;

    dui_ca_update(&element->text_color);
    dui_ca_update(&element->box_color);

    if (data.checked) {
        element->check_state = (*data.checked) ? CHECK_CHECKED : CHECK_UNCHECKED;
    }

    const DUI_NextState next_state = dui_ctx_next_state(
        element->state,
        data.disabled,
        &element->element
    );
    if (next_state.state != element->state) {
        element->state = next_state.state;
        switch (element->state) {
        case STATE_NORMAL:
            dui_ca_start(&element->text_color, DUI_CHECKBOX_TEXT(data.kind), 0.1);
            dui_ca_start(&element->box_color, DUI_CHECKBOX_BOX(data.kind), 0.1);
            break;
        case STATE_HOVER:
            dui_ca_start(&element->text_color, DUI_CHECKBOX_TEXT_HOVER(data.kind), 0.1);
            dui_ca_start(&element->box_color, DUI_CHECKBOX_BOX_HOVER(data.kind), 0.1);
            break;
        case STATE_DOWN:
            dui_ca_start(&element->text_color, DUI_CHECKBOX_TEXT_DOWN(data.kind), 0.1);
            dui_ca_start(&element->box_color, DUI_CHECKBOX_BOX_DOWN(data.kind), 0.1);
            break;
        case STATE_DISABLED:
            dui_ca_start(&element->text_color, DUI_CHECKBOX_TEXT_DISABLED(data.kind), 0.1);
            dui_ca_start(&element->box_color, DUI_CHECKBOX_BOX_DISABLED(data.kind), 0.1);
            break;
        }
    }

    if (next_state.activated) {
        if (element->check_state != CHECK_CHECKED) {
            element->check_state = CHECK_CHECKED;
        } else {
            element->check_state = CHECK_UNCHECKED;
        }
    }

    Rectangle box_bounds = dui_lay_padding(element->element.bounds, 1, 1, 0, 1);
    box_bounds.width = box_bounds.height;

    DrawRectangleRec(box_bounds, element->box_color.current);
    if (element->check_state == CHECK_CHECKED) {
        const Rectangle mark_bounds = dui_lay_padding_all(box_bounds, 1);
        DrawLineEx(
            CLITERAL(Vector2){mark_bounds.x, mark_bounds.y + mark_bounds.height / 2},
            CLITERAL(Vector2){mark_bounds.x + mark_bounds.width / 2.25, mark_bounds.y + mark_bounds.height},
            2,
            DUI_CHECKBOX_CHECK(data.kind)
        );
        DrawLineEx(
            CLITERAL(Vector2){mark_bounds.x + mark_bounds.width / 2.25, mark_bounds.y + mark_bounds.height},
            CLITERAL(Vector2){mark_bounds.x + mark_bounds.width, mark_bounds.y},
            2,
            DUI_CHECKBOX_CHECK(data.kind)
        );
    }

    if (data.caption) {
        const Rectangle caption_bounds = dui_lay_padding(element->element.bounds, 7, 0, 0, 0);
        dui_text(
            data.caption,
            caption_bounds,
            .color = element->text_color.current,
            .vertical_alignment = ALIGN_CENTER,
            .offset_y = (element->state == STATE_DOWN) ? 1 : 0,
        );
    }

    if (data.checked) {
        *data.checked = (element->check_state == CHECK_CHECKED);
    }

    return (element->check_state == CHECK_CHECKED);
}
