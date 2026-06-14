#include <dui/layout/spaced.h>
#include <dui/environment.h>

DUI_BoundsData dui__spaced_next_bounds(void* element, const DUI_Placement placement) {
    DUI_SpacedElement* spaced_element = element;

    DUI_BoundsData result = {
        .bounds = spaced_element->layout_element.element.bounds,
        .tabOrderBack = false,
    };

    const int weight = max(placement.weight, 1);
    switch (spaced_element->direction) {
    case DUI_HORIZONTAL: {
        const float cell_size = 1.0 * (result.bounds.width - (spaced_element->count - 1) * DUI_SPACING(
                spaced_element->gap
            )) /
            spaced_element->count;
        result.bounds.x += spaced_element->cursor * (cell_size + DUI_SPACING(spaced_element->gap));
        result.bounds.width = ceilf(weight * cell_size + (weight - 1) * DUI_SPACING(spaced_element->gap));
        break;
    }
    case DUI_VERTICAL: {
        const float cell_size = 1.0 * (result.bounds.height - (spaced_element->count - 1) * DUI_SPACING(
                spaced_element->gap
            )) /
            spaced_element->count;
        result.bounds.y += spaced_element->cursor * (cell_size + DUI_SPACING(spaced_element->gap));
        result.bounds.height = ceilf(weight * cell_size + (weight - 1) * DUI_SPACING(spaced_element->gap));
        break;
    }
    default:
        NOB_UNREACHABLE("Unknown spaced direction.");
    }

    spaced_element->cursor += weight;
    return result;
}

void dui__spaced_reset(void* element) {
    DUI_SpacedElement* spaced_element = element;
    spaced_element->cursor = 0;
}

void dui_begin_spaced_impl(const int id, const DUI_SpacedData data) {
    DUI_SpacedElement* element;
    dui_begin_layout(DUI_SPACED_TYPE_ID, id, data.placement, element);
    element->layout_element.callback = dui__spaced_next_bounds;
    element->layout_element.reset_callback = dui__spaced_reset;
    element->direction = data.direction;
    element->count = data.count;
    element->gap = data.gap;
    element->cursor = 0;
}
