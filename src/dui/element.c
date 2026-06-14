#include <dui/element.h>
#include <dui/environment.h>

#include "nob.h"

DUI_Element* dui_element() {
    return dui_env()->element_stack_top;
}

static void dui__link_tab_order(DUI_Element* element, const bool tabOrderBack) {
    DUI_Element* parent = element->parent;

    if (tabOrderBack) {
        if (parent->tabOrderBackCursor == NULL) {
            parent->tabOrderBack = element;
            parent->tabOrderBackCursor = element;
            element->tabOrderPrev = NULL;
            element->tabOrderNext = NULL;
        } else {
            element->tabOrderNext = parent->tabOrderBackCursor;
            element->tabOrderPrev = NULL;

            parent->tabOrderBackCursor->tabOrderPrev = element;
            parent->tabOrderBackCursor = element;
        }
    } else {
        if (parent->tabOrderFrontCursor == NULL) {
            parent->tabOrderFront = element;
            parent->tabOrderFrontCursor = element;
            element->tabOrderPrev = NULL;
            element->tabOrderNext = NULL;
        } else {
            element->tabOrderPrev = parent->tabOrderFrontCursor;
            element->tabOrderNext = NULL;

            parent->tabOrderFrontCursor->tabOrderNext = element;
            parent->tabOrderFrontCursor = element;
        }
    }
}

int dui__next_index_by_id(const int id) {
    DUI_Element* element = dui_element();

    nob_da_foreach(DUI_ElementIndex, it, &element->indices) {
        if (it->id == id) {
            it->index += 1;
            return it->index;
        }
    }

    const DUI_ElementIndex element_index = {
        .id = id,
        .index = 0
    };
    nob_da_append(&element->indices, element_index);

    return element_index.index;
}

DUI_ElementCacheState dui_get_element_impl(
    const DUI_Id type, const int id, const DUI_Kind kind, const bool tabOrderBack, const bool disabled,
    const int size, void** element
) {
    DUI_Element* parent = dui_element();
    DUI_Environment* env = dui_env();
    const int index = dui__next_index_by_id(id);

    nob_da_foreach(DUI_Element*, it, &parent->children) {
        DUI_Element* candidate = *it;
        if (candidate->type.hash == type.hash && candidate->id == id && candidate->index == index) {
            if (!disabled) {
                dui__link_tab_order(candidate, tabOrderBack);
            }
            candidate->kind = kind;
            *element = candidate;
            return DUI_CACHED;
        }
    }

    DUI_Element* new_element = arena_alloc(&env->memory, size);
    memset(new_element, 0, size);

    new_element->type = type;
    new_element->id = id;
    new_element->index = index;
    new_element->kind = kind;
    new_element->parent = parent;
    nob_da_append(&parent->children, new_element);

    if (!disabled) {
        dui__link_tab_order(new_element, tabOrderBack);
    }
    *element = new_element;
    return DUI_NEWLY_CREATED;
}

DUI_ElementCacheState dui_get_active_element_impl(
    const DUI_Id type, const int id, const DUI_Kind kind, const bool disabled, const int size,
    void** element, const DUI_Placement placement
) {
    const DUI_BoundsData bounds_data = dui_next_bounds_impl(placement);

    const DUI_ElementCacheState result = dui_get_element_impl(
        type, id, kind, bounds_data.tabOrderBack, disabled, size, element
    );
    ((DUI_Element*)*element)->bounds = bounds_data.bounds;

    return result;
}

void dui_begin_element_impl(const DUI_Id type, const int id, const DUI_ElementData data) {
    DUI_Environment* env = dui_env();

    DUI_Element* element;
    dui_get_element(type, id, DUI_DEFAULT, data.tabOrderBack, false, element);
    element->tabOrderFrontCursor = NULL;
    element->tabOrderBackCursor = NULL;
    element->placed_at_back = data.tabOrderBack;
    element->indices.count = 0;

    env->element_stack_top = element;
}

void dui_end_element() {
    DUI_Environment* env = dui_env();

    DUI_Element* element = env->element_stack_top;
    DUI_Element* parent = element->parent;

    assert(parent != NULL && "Cannot end a DUI context that has not begun.");

    if (element->tabOrderFrontCursor == NULL) {
        element->tabOrderFront = element->tabOrderBackCursor;
    } else if (element->tabOrderBackCursor == NULL) {
        element->tabOrderBack = element->tabOrderFrontCursor;
    } else if (element->tabOrderFrontCursor != element->tabOrderBackCursor) {
        element->tabOrderFrontCursor->tabOrderNext = element->tabOrderBackCursor;
        element->tabOrderBackCursor->tabOrderPrev = element->tabOrderFrontCursor;
    }

    if (!element->placed_at_back) {
        assert(parent->tabOrderFrontCursor == element && "Inconsistent context nesting state");
        DUI_Element* before = parent->tabOrderFrontCursor->tabOrderPrev;
        if (before) {
            before->tabOrderNext = element->tabOrderFront;
            if (element->tabOrderFront) {
                element->tabOrderFront->tabOrderPrev = before;
            }
            if (element->tabOrderBack) {
                parent->tabOrderFrontCursor = element->tabOrderBack;
            } else {
                parent->tabOrderFrontCursor = before;
            }
        } else {
            parent->tabOrderFront = element->tabOrderFront;
            parent->tabOrderFrontCursor = element->tabOrderBack;
        }
    } else {
        assert(parent->tabOrderBackCursor == element && "Inconsistent context nesting state");
        DUI_Element* after = parent->tabOrderBackCursor->tabOrderNext;
        if (after) {
            after->tabOrderPrev = element->tabOrderBack;
            if (element->tabOrderBack) {
                element->tabOrderBack->tabOrderNext = after;
            }
            if (element->tabOrderFront) {
                parent->tabOrderBackCursor = element->tabOrderFront;
            } else {
                parent->tabOrderFrontCursor = after;
            }
        } else {
            parent->tabOrderBack = element->tabOrderBack;
            parent->tabOrderBackCursor = element->tabOrderFront;
        }
    }

    // if (parent->parent == NULL) {
    //     parent->tabOrderFront = element->tabOrderFront;
    //     parent->tabOrderBack = element->tabOrderBack;
    // }

    env->element_stack_top = parent;
}

DUI_NextState dui_next_state(
    const DUI_State current_state, const bool disabled, DUI_Element* element
) {
    DUI_NextState next_state = {
        .activated = false,
        .state = current_state,
    };

    if (disabled) {
        next_state.state = STATE_DISABLED;
        return next_state;
    }


    const Vector2 mouse_position = GetMousePosition();
    const DUI_Environment* env = dui_env();

    bool mouse_blocked = false;
    for (size_t i = 0; i < env->previous_window_count; ++i) {
        if (CheckCollisionPointRec(mouse_position, env->previous_windows_bounds[i])) {
            mouse_blocked = true;
            break;
        }
    }

    if (!mouse_blocked) {
        const bool hovered = CheckCollisionPointRec(mouse_position, element->bounds);
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (current_state == STATE_DOWN && hovered) {
                next_state.activated = true;
            }

            next_state.state = STATE_NORMAL;
        }

        if (hovered) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                next_state.state = STATE_DOWN;
                dui_env_focus(element);
            } else if (current_state != STATE_HOVER && current_state != STATE_DOWN) {
                next_state.state = STATE_HOVER;
            }
        } else {
            if (current_state == STATE_HOVER) {
                next_state.state = STATE_NORMAL;
            }
        }
    }

    if (dui_env_has_focus(element) && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
        next_state.activated = true;
    }

    return next_state;
}

unsigned long dui__djb2_hash(const char* string, const size_t length) {
    unsigned long hash = 5381;
    for (size_t i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) + string[i]; /* hash * 33 + c */
    }
    return hash;
}

DUI_Id dui_create_id_cstr(const char* cstr) {
    const size_t length = strlen(cstr);
    return (DUI_Id){
        .value = {
            .data = cstr,
            .count = length
        },
        .hash = dui__djb2_hash(cstr, length),
    };
}
