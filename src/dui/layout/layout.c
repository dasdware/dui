#include <dui/layout/layout.h>
#include <dui/environment.h>

DUI_BoundsData dui_next_bounds_impl(const DUI_Placement placement) {
    DUI_BoundsData bounds = {0};

    DUI_LayoutElement* current_layout_element = dui_env()->layout_element_stack_top;
    if (current_layout_element) {
        bounds = current_layout_element->callback(current_layout_element, placement);
    }

    return bounds;
}

void dui_begin_layout_impl(
    const DUI_Id type, const int id, const DUI_Placement placement, const size_t size, void** element
) {
    dui_get_active_element_impl(type, id, DUI_DEFAULT, true, size, element, placement);

    DUI_LayoutElement* layout_element = *element;
    layout_element->parent_layout_element = dui_env()->layout_element_stack_top;
    dui_env()->layout_element_stack_top = layout_element;

    if (layout_element->reset_callback) {
        layout_element->reset_callback(layout_element);
    }
}

void dui_begin_inactive_layout_impl(
    const DUI_Id type, const int id, const size_t size, void** element
) {
    dui_get_element_impl(type, id, DUI_DEFAULT, false, true, size, element);

    DUI_LayoutElement* layout_element = *element;
    layout_element->parent_layout_element = dui_env()->layout_element_stack_top;
    dui_env()->layout_element_stack_top = layout_element;

    if (layout_element->reset_callback) {
        layout_element->reset_callback(layout_element);
    }
}

void dui_end_layout() {
    dui_env()->layout_element_stack_top = dui_env()->layout_element_stack_top->parent_layout_element;
}

DUI_Placement dui_forward_placement(const int preferred_width, const int preferred_height, DUI_Placement placement) {
    if (placement.width == 0) {
        placement.width = preferred_width;
    }
    if (placement.height == 0) {
        placement.height = preferred_height;
    }
    return placement;
}
