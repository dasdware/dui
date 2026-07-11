#include <dui/layout/window.h>
#include <dui/environment.h>

DUI_BoundsData dui__window_next_bounds(void* element, DUI_Placement placement) {
    NOB_UNUSED(placement);
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

Vector2 dui__window_transform_coordinate(
    void* element, Vector2 coordinate, DUI_TransformCoordinateDirection direction
) {
    const DUI_WindowElement* window_element = element;

    Vector2 result = coordinate;
    switch (direction) {
    case DUI_TRANSFORM_FROM_CLIENT:
        result.x += window_element->bounds.x;
        result.y += window_element->bounds.y;
        break;
    case DUI_TRANSFORM_TO_CLIENT:
        result.x -= window_element->bounds.x;
        result.y -= window_element->bounds.y;
        break;
    default:
        NOB_UNREACHABLE("dui__window_transform_coordinate - switch on direction");
    }

    return result;
}


void dui_begin_window_impl(const int id, const DUI_WindowData data) {
    DUI_WindowElement* element;
    dui_begin_inactive_layout(DUI_WINDOW_TYPE_ID, id, element);
    element->layout_element.callback = dui__window_next_bounds;
    element->layout_element.transform_coordinate_callback = dui__window_transform_coordinate;
    element->bounds = data.bounds;

    const DUI_Window* window = dui_enqueue_window(data.bounds, data.transparency);
    BeginTextureMode(window->texture);
}

void dui_end_window_impl() {
    EndTextureMode();
    dui_end_layout();
}
