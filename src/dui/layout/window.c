#include <dui/layout/window.h>
#include <dui/environment.h>

DUI_BoundsData dui__window_next_bounds(void* element, DUI_LayoutData data) {
    NOB_UNUSED(data);
    const DUI_WindowElement* window_element = element;

    return CLITERAL(DUI_BoundsData){
        .bounds = {
            .x = 0,
            .y = 0,
            .width = window_element->bounds.width,
            .height = window_element->bounds.height
        },
        .tabOrderBack = false,
    };
}

void dui_begin_window_impl(const int id, const DUI_WindowData data) {
    DUI_WindowElement* element;
    dui_begin_inactive_layout(DUI_WINDOW_TYPE_ID, id, element);
    element->layout_element.callback = dui__window_next_bounds;
    element->bounds = data.bounds;

    const DUI_Window* window = dui_enqueue_window(data.bounds, data.transparency);
    BeginTextureMode(window->texture);
}

void dui_end_window_impl() {
    EndTextureMode();
    dui_end_layout();
}

