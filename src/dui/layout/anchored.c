#include <dui/layout/anchored.h>
#include <dui/environment.h>

DUI_BoundsData dui__anchored_next_bounds(void* element, DUI_LayoutData data) {
    DUI_AnchoredElement* anchored_element = element;

    DUI_BoundsData result = {
        .bounds = anchored_element->layout_element.element.bounds,
        .tabOrderBack = false,
    };

    switch (data.anchor) {
    case DUI_LEFT:
        result.bounds.x += anchored_element->inset_left;
        result.bounds.y += anchored_element->inset_top;
        result.bounds.width = data.size;
        result.bounds.height -= anchored_element->inset_top + anchored_element->inset_bottom;
        anchored_element->inset_left += data.size + anchored_element->gap;
        break;
    case DUI_RIGHT:
        result.bounds.x += result.bounds.width - anchored_element->inset_right - data.size;
        result.bounds.y += anchored_element->inset_top;
        result.bounds.width = data.size;
        result.bounds.height -= anchored_element->inset_top + anchored_element->inset_bottom;
        anchored_element->inset_right += data.size + anchored_element->gap;
        result.tabOrderBack = true;
        break;
    case DUI_TOP:
        result.bounds.x += anchored_element->inset_left;
        result.bounds.y += anchored_element->inset_top;
        result.bounds.height = data.size;
        result.bounds.width -= anchored_element->inset_left - anchored_element->inset_right;
        anchored_element->inset_top += data.size + anchored_element->gap;
        break;
    case DUI_BOTTOM:
        result.bounds.x += anchored_element->inset_left;
        result.bounds.y += result.bounds.height - anchored_element->inset_bottom - data.size;
        result.bounds.height = data.size;
        result.bounds.width -= anchored_element->inset_left - anchored_element->inset_right;
        anchored_element->inset_bottom += data.size + anchored_element->gap;
        result.tabOrderBack = true;
        break;
    case DUI_REMAINING:
        result.bounds.x += anchored_element->inset_left;
        result.bounds.y += anchored_element->inset_top;
        result.bounds.width -= anchored_element->inset_left + anchored_element->inset_right;
        result.bounds.height -= anchored_element->inset_top + anchored_element->inset_bottom;
        break;
    default:
        NOB_UNREACHABLE("Unknown anchor.");
    }

    return result;
}

void dui__anchored_reset(void* element) {
    DUI_AnchoredElement* anchored_element = element;
    anchored_element->inset_left = 0;
    anchored_element->inset_right = 0;
    anchored_element->inset_top = 0;
    anchored_element->inset_bottom = 0;
}

void dui_begin_anchored_impl(const int id, const DUI_AnchoredData data) {
    DUI_AnchoredElement* element;
    dui_begin_layout(DUI_ANCHORED_TYPE_ID, id, data.layout_data, element);
    element->layout_element.callback = dui__anchored_next_bounds;
    element->layout_element.reset_callback = dui__anchored_reset;
    element->gap = data.gap;
    element->inset_left = 0;
    element->inset_right = 0;
    element->inset_top = 0;
    element->inset_bottom = 0;
}
