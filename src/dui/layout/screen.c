#include <dui/layout/screen.h>
#include <dui/environment.h>

DUI_BoundsData dui__screen_next_bounds(void* element, DUI_Placement placement) {
    NOB_UNUSED(placement);
    const DUI_ScreenElement* screen_element = element;

    DUI_BoundsData result = {
        .bounds = CLITERAL(Rectangle){
            .x = 0,
            .y = 0,
            .width = GetScreenWidth(),
            .height = GetScreenHeight(),
        },
        .tabOrderBack = false,
    };
    result.bounds = DUI_PADDING_ALL(result.bounds, screen_element->padding);
    return result;
}

void dui_begin_screen_impl(const int id, const DUI_ScreenData data) {
    DUI_ScreenElement* element;
    dui_begin_layout(DUI_SCREEN_TYPE_ID, id, data.placement, element);
    element->layout_element.callback = dui__screen_next_bounds;
    element->padding = data.padding;
}
