#include <dui/edit.h>
#include <dui/environment.h>

#include "keyboard.h"
#include "text.h"
#include "theme.h"

static int dui__edit_cursor_offset(const Nob_String_Builder* text, const size_t position) {
    return dui_text_measure(TextFormat("%.*s", (int)position, text->items)).x + 1;
}

static void dui__edit_change_selection(
    DUI_EditElement* element, const size_t new_selection_start, const int new_selection_length,
    const Rectangle client_bounds, const Nob_String_Builder* text
) {
    const int new_cursor_offset = dui__edit_cursor_offset(text, new_selection_start);

    if (new_selection_start > element->selection_start) {
        if (new_cursor_offset > client_bounds.width) {
            element->scroll_offset = max(element->scroll_offset, new_cursor_offset - client_bounds.width);
        }
    } else if (new_selection_start < element->selection_start) {
        while (new_cursor_offset < element->scroll_offset) {
            element->scroll_offset = max(0, element->scroll_offset - DUI_EDIT_BACK_SCROLL_STEP);
        }
    }

    element->selection_start = new_selection_start;
    element->selection_length = new_selection_length;
}

static void dui__edit_replace_selection(
    DUI_EditElement* element, const Rectangle client_bounds, Nob_String_Builder* text, const char* replacement,
    const size_t replacement_length
) {
    const int first = min(element->selection_start, (int)element->selection_start - element->selection_length);
    const int last = max(element->selection_start, (int)element->selection_start - element->selection_length);

    const int length_diff = (int)replacement_length - (last - first);
    nob_da_reserve(text, text->count + length_diff);

    memmove(text->items + first + replacement_length, text->items + last, text->count - last);
    memcpy(text->items + first, replacement, replacement_length);
    text->count += length_diff;

    dui__edit_change_selection(element, first + replacement_length, 0, client_bounds, text);
}

bool dui_edit_impl(const int id, const DUI_EditData data) {
    static int element_type_edit = 0;
    if (element_type_edit == 0) {
        element_type_edit = dui_env_next_element_type();
    }

    const int padding = DUI_SPACING(1);
    const int preferred_width = 200;
    const int preferred_height = DUI_FONT_SIZE + 2 * padding;

    DUI_EditElement* element;
    if (dui_ctx_active_element_by_id(
        element_type_edit,
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
            element->background = dui_ca_create(DUI_EDIT_BG_DISABLED(data.kind));
            element->border = dui_ca_create(DUI_EDIT_BORDER_DISABLED(data.kind));
            element->state = STATE_DISABLED;
        } else {
            element->background = dui_ca_create(DUI_EDIT_BG(data.kind));
            element->border = dui_ca_create(DUI_EDIT_BORDER(data.kind));
            element->state = STATE_NORMAL;
        }
    }

    dui_ca_update(&element->background);
    dui_ca_update(&element->border);

    const DUI_NextState next_state = dui_ctx_next_state(
        element->state,
        data.disabled,
        &element->element
    );
    if (next_state.state != element->state) {
        element->state = next_state.state;
        switch (element->state) {
        case STATE_NORMAL:
        default:
            dui_ca_start(&element->background, DUI_EDIT_BG(data.kind), 0.1);
            dui_ca_start(&element->border, DUI_EDIT_BORDER(data.kind), 0.1);
            break;
        case STATE_HOVER:
        case STATE_DOWN:
            dui_ca_start(&element->background, DUI_EDIT_BG_HOVER(data.kind), 0.1);
            dui_ca_start(&element->border, DUI_EDIT_BORDER(data.kind), 0.1);
            break;
        case STATE_DISABLED:
            dui_ca_start(&element->background, DUI_EDIT_BG_DISABLED(data.kind), 0.1);
            dui_ca_start(&element->border, DUI_EDIT_BORDER_DISABLED(data.kind), 0.1);
            break;
        }
    }

    const Rectangle client_bounds = dui_lay_padding_all(element->element.bounds, 1);
    if (data.text && dui_env_has_focus(&element->element) && !data.disabled) {
        // MOUSE INTERACTIVITY
        // ======================

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && next_state.state == STATE_DOWN) {
            for (int i = data.text->count; i >= 0; i--) {
                const int pos = client_bounds.x - element->scroll_offset + dui__edit_cursor_offset(data.text, i);
                if (pos < GetMouseX()) {
                    dui__edit_change_selection(element, i, 0, client_bounds, data.text);
                    break;
                }
            }
        }

        // TODO: Allow selection via mouse dragging

        // KEYBOARD INTERACTIVITY
        // ======================

        // Navigation and selection
        // -------------------------

        // (Shift+)Left
        if (
            dui_is_key_down(KEY_LEFT, .state = &element->left_key_state, .delay = DUI_EDIT_KEY_DELAY)
            && element->selection_start > 0
        ) {
            const int new_selection_start = element->selection_start - 1;
            int new_selection_length = element->selection_length;
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                new_selection_length -= 1;
            } else {
                new_selection_length = 0;
            }
            dui__edit_change_selection(
                element, new_selection_start, new_selection_length, client_bounds, data.text
            );
        }

        // (Shift+)Right
        if (
            dui_is_key_down(KEY_RIGHT, .state = &element->right_key_state, .delay = DUI_EDIT_KEY_DELAY)
            && element->selection_start < data.text->count
        ) {
            const int new_selection_start = element->selection_start + 1;
            int new_selection_length = element->selection_length;
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                new_selection_length += 1;
            } else {
                new_selection_length = 0;
            }
            dui__edit_change_selection(
                element, new_selection_start, new_selection_length, client_bounds, data.text
            );
        }

        // End
        if (IsKeyPressed(KEY_END)) {
            const int new_selection_start = data.text->count;
            int new_selection_length = 0;
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                new_selection_length = data.text->count - element->selection_start + element->selection_length;
            }
            dui__edit_change_selection(
                element, new_selection_start, new_selection_length, client_bounds, data.text
            );
        }

        // Home
        if (IsKeyPressed(KEY_HOME)) {
            const int new_selection_start = 0;
            int new_selection_length = 0;
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                new_selection_length = -(element->selection_start - element->selection_length);
            }
            dui__edit_change_selection(
                element, new_selection_start, new_selection_length, client_bounds, data.text
            );
        }

        // Ctrl+A - Select all
        if (IsKeyPressed(KEY_A) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))) {
            dui__edit_change_selection(element, data.text->count, data.text->count, client_bounds, data.text);
        }

        // Text manipulation
        // -------------------------

        // Printable characters
        const int pressed_key = GetCharPressed();
        if (pressed_key != 0) {
            dui__edit_replace_selection(element, client_bounds, data.text, (char*)&pressed_key, 1);
        }

        // Backspace
        if (
            dui_is_key_down(KEY_BACKSPACE, .state = &element->backspace_key_state, .delay = DUI_EDIT_KEY_DELAY)
            && data.text->count > 0
            && (element->selection_start > 0 || element->selection_length > 0)
        ) {
            if (element->selection_length == 0) {
                element->selection_length = 1;
            }
            dui__edit_replace_selection(element, client_bounds, data.text, "", 0);
        }

        // Delete
        if (
            dui_is_key_down(KEY_DELETE, .state = &element->delete_key_state, .delay = DUI_EDIT_KEY_DELAY)
            && data.text->count > 0
            && (element->selection_start < data.text->count || element->selection_length > 0)
        ) {
            if (element->selection_length == 0) {
                element->selection_length = -1;
            }
            dui__edit_replace_selection(element, client_bounds, data.text, "", 0);
        }

        // TODO: Add clipboard support (Ctrl+C / Ctrl+V)
    }

    DrawRectangleRec(element->element.bounds, element->background.current);
    DrawRectangleLinesEx(element->element.bounds, 1, element->border.current);

    if (data.text) {
        BeginScissorMode(client_bounds.x, client_bounds.y, client_bounds.width, client_bounds.height);
        {
            const int cursor_position = client_bounds.x - element->scroll_offset
                + dui__edit_cursor_offset(data.text, element->selection_start);
            if (element->selection_length != 0) {
                const int selection_position = client_bounds.x - element->scroll_offset
                    + dui__edit_cursor_offset(data.text, element->selection_start - element->selection_length);
                const Rectangle selection_bounds = {
                    .x = min(cursor_position, selection_position),
                    .y = client_bounds.y,
                    .width = abs(selection_position - cursor_position),
                    .height = client_bounds.height,
                };
                DrawRectangleRec(selection_bounds, DUI_EDIT_BG_SELECTION(data.kind));
            }

            if (dui_env_has_focus(element) && !data.disabled) {
                DrawLineEx(
                    CLITERAL(Vector2){cursor_position, client_bounds.y},
                    CLITERAL(Vector2){cursor_position, client_bounds.y + client_bounds.height},
                    1,
                    DUI_EDIT_TEXT(data.kind)
                );
            }

            if (data.text->count > 0) {
                const Rectangle text_bounds = {
                    .x = client_bounds.x - element->scroll_offset,
                    .y = client_bounds.y,
                    .width = client_bounds.width + element->scroll_offset,
                    .height = client_bounds.height,
                };
                dui_text(
                    TextFormat("%.*s", (int) data.text->count, data.text->items),
                    text_bounds,
                    .vertical_alignment = ALIGN_CENTER,
                    .color = (data.disabled) ? DUI_EDIT_TEXT_DISABLED(data.kind) : DUI_EDIT_TEXT(data.kind),
                );
            }
        }
        EndScissorMode();
    }

    return IsKeyPressed(KEY_ENTER);
}
