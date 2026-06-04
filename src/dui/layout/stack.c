#include <dui/layout/stack.h>
#include <dui/environment.h>

DUI_BoundsData dui__stack_next_bounds(void* element, const DUI_LayoutData data) {
    DUI_StackElement* stack_element = element;

    DUI_BoundsData result = {
        .bounds = stack_element->layout_element.element.bounds,
        .tabOrderBack = false,
    };

    int item_size = (stack_element->direction == DUI_HORIZONTAL)
                        ? max(data.size, data.width)
                        : max(data.size, data.height);

    if (data.remaining) {
        item_size = (stack_element->direction == DUI_HORIZONTAL)
                        ? result.bounds.width - stack_element->inset_begin - stack_element->inset_end
                        : result.bounds.height - stack_element->inset_begin - stack_element->inset_end;
    } else if (item_size == 0) {
        item_size = stack_element->item_size;
    }

    if (stack_element->direction == DUI_HORIZONTAL) {
        if (data.opposite) {
            result.bounds.x += result.bounds.width - stack_element->inset_end - item_size;
            stack_element->inset_end += item_size + DUI_SPACING(stack_element->gap);
            result.tabOrderBack = true;
        } else {
            result.bounds.x += stack_element->inset_begin;
            stack_element->inset_begin += item_size + DUI_SPACING(stack_element->gap);
        }
        result.bounds.width = item_size;
    } else {
        if (data.opposite) {
            result.bounds.y += result.bounds.height - stack_element->inset_end - item_size;
            stack_element->inset_end += item_size + DUI_SPACING(stack_element->gap);
            result.tabOrderBack = true;
        } else {
            result.bounds.y += stack_element->inset_begin;
            stack_element->inset_begin += item_size + DUI_SPACING(stack_element->gap);
        }
        result.bounds.height = item_size;
    }
    return result;
}

void dui__stack_reset(void* element) {
    DUI_StackElement* stack_element = element;
    stack_element->inset_begin = 0;
    stack_element->inset_end = 0;
}

void dui_begin_stack_impl(const int id, const DUI_StackData data) {
    DUI_StackElement* element;
    dui_begin_layout(DUI_STACK_TYPE_ID, id, data.layout_data, element);
    element->layout_element.callback = dui__stack_next_bounds;
    element->layout_element.reset_callback = dui__stack_reset;
    element->direction = data.direction;
    element->gap = data.gap;
    element->item_size = data.item_size;
    element->inset_begin = 0;
    element->inset_end = 0;
}
