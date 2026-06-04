#include <dui/layout/window.h>
#include <dui/environment.h>

DUI_BoundsData dui__window_next_bounds(const void* element, const DUI_LayoutData data) {
    NOB_UNUSED(data);
    const DUI_WindowElement* window_element = element;

    DUI_BoundsData result = {
        .bounds = window_element->layout_element.element.bounds,
        .tabOrderBack = false,
    };

    result.bounds = window_element->bounds;

    return result;
}

void dui_begin_window_impl(const int id, const DUI_WindowData data) {
    DUI_WindowElement* element;
    dui_begin_layout(DUI_WINDOW_TYPE_ID, id, data.layout_data, element);
    element->layout_element.callback = dui__window_next_bounds;
    element->bounds = data.bounds;
    element->floating = data.floating;
}
