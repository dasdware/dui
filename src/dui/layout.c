#include <nob.h>
#include <math.h>

#include <dui/environment.h>
#include <dui/layout.h>

#define dui__lay_rectangle_data(data) dui_lay_rectangle_impl(*((DUI_Layout_Data*) &data))

static DUI_Layout layout_stack[DUI_LAYOUT_STACK_CAPACITY] = {0};
static size_t layout_stack_count = 0;

Rectangle dui_lay_padding(const Rectangle bounds, const int left, const int top, const int right, const int bottom)
{
    Rectangle result = bounds;
    result.x += dui_env_spacing(left);
    result.y += dui_env_spacing(top);
    result.width -= dui_env_spacing(left + right);
    result.height -= dui_env_spacing(top + bottom);
    return result;
}

Rectangle dui_lay_center(const Rectangle bounds, const int width, const int height)
{
    Rectangle result = {0};
    result.x = bounds.x + (bounds.width - width) / 2;
    result.y = bounds.y + (bounds.height - height) / 2;
    result.width = width;
    result.height = height;
    return result;
}

DUI_Layout_BoundsData dui_lay_rectangle_impl(const DUI_Layout_Data data)
{
    NOB_ASSERT(layout_stack_count > 0);

    DUI_Layout* layout = &layout_stack[layout_stack_count - 1];
    DUI_Layout_BoundsData result = {
        .bounds = layout->parent_bounds,
        .tabOrderBack = false,
    };

    switch (layout->kind)
    {
    case LAYOUT_SCREEN:
        result.bounds = CLITERAL(Rectangle){
            .x = 0,
            .y = 0,
            .width = GetScreenWidth(),
            .height = GetScreenHeight(),
        };
        result.bounds = dui_lay_padding_all(result.bounds, layout->as.screen.padding);
        return result;
    case LAYOUT_ANCHOR:
        switch (data.anchor)
        {
        case ANCHOR_LEFT:
            result.bounds.x += layout->as.anchored.inset_left;
            result.bounds.y += layout->as.anchored.inset_top;
            result.bounds.width = data.size;
            result.bounds.height -= layout->as.anchored.inset_top + layout->as.anchored.inset_bottom;
            layout->as.anchored.inset_left += data.size + layout->as.anchored.gap;
            return result;
        case ANCHOR_RIGHT:
            result.bounds.x += result.bounds.width - layout->as.anchored.inset_right - data.size;
            result.bounds.y += layout->as.anchored.inset_top;
            result.bounds.width = data.size;
            result.bounds.height -= layout->as.anchored.inset_top + layout->as.anchored.inset_bottom;
            layout->as.anchored.inset_right += data.size + layout->as.anchored.gap;
            result.tabOrderBack = true;
            return result;
        case ANCHOR_TOP:
            result.bounds.x += layout->as.anchored.inset_left;
            result.bounds.y += layout->as.anchored.inset_top;
            result.bounds.height = data.size;
            result.bounds.width -= layout->as.anchored.inset_left - layout->as.anchored.inset_right;
            layout->as.anchored.inset_top += data.size + layout->as.anchored.gap;
            return result;
        case ANCHOR_BOTTOM:
            result.bounds.x += layout->as.anchored.inset_left;
            result.bounds.y += result.bounds.height - layout->as.anchored.inset_bottom - data.size;
            result.bounds.height = data.size;
            result.bounds.width -= layout->as.anchored.inset_left - layout->as.anchored.inset_right;
            layout->as.anchored.inset_bottom += data.size + layout->as.anchored.gap;
            result.tabOrderBack = true;
            return result;
        case ANCHOR_REMAINING:
            result.bounds.x += layout->as.anchored.inset_left;
            result.bounds.y += layout->as.anchored.inset_top;
            result.bounds.width -= layout->as.anchored.inset_left + layout->as.anchored.inset_right;
            result.bounds.height -= layout->as.anchored.inset_top + layout->as.anchored.inset_bottom;
            return result;
        default:
            NOB_UNREACHABLE("Unknown anchor.");
        }
    case LAYOUT_STACK:
        {
            int item_size = (layout->as.stack.direction == DIRECTION_HORIZONTAL)
                ? max(data.size, data.width)
                : max(data.size, data.height);

            if (data.remaining)
            {
                item_size = (layout->as.stack.direction == DIRECTION_HORIZONTAL)
                    ? result.bounds.width - layout->as.stack.inset_begin - layout->as.stack.inset_end
                    : result.bounds.height - layout->as.stack.inset_begin - layout->as.stack.inset_end;
            }
            else if (item_size == 0)
            {
                item_size = layout->as.stack.item_size;
            }

            if (layout->as.stack.direction == DIRECTION_HORIZONTAL)
            {
                if (data.opposite)
                {
                    result.bounds.x += result.bounds.width - layout->as.stack.inset_end - item_size;
                    layout->as.stack.inset_end += item_size + layout->as.stack.gap;
                    result.tabOrderBack = true;
                }
                else
                {
                    result.bounds.x += layout->as.stack.inset_begin;
                    layout->as.stack.inset_begin += item_size + layout->as.stack.gap;
                }
                result.bounds.width = item_size;
            }
            else
            {
                if (data.opposite)
                {
                    result.bounds.y += result.bounds.height - layout->as.stack.inset_end - item_size;
                    layout->as.stack.inset_end += item_size + layout->as.stack.gap;
                    result.tabOrderBack = true;
                }
                else
                {
                    result.bounds.y += layout->as.stack.inset_begin;
                    layout->as.stack.inset_begin += item_size + layout->as.stack.gap;
                }
                result.bounds.height = item_size;
            }
            return result;
        }
    case LAYOUT_SPACED:
        {
            const size_t count = layout->as.spaced.count;
            const size_t gap = layout->as.spaced.gap;
            const size_t cursor = layout->as.spaced.cursor;
            const int weight = max(data.weight, 1);

            switch (layout->as.spaced.direction)
            {
            case DIRECTION_HORIZONTAL:
                {
                    const float cell_size = 1.0 * (result.bounds.width - (count - 1) * gap) / count;
                    result.bounds.x += cursor * (cell_size + gap);
                    result.bounds.width = ceilf(weight * cell_size + (weight - 1) * gap);
                    break;
                }
            case DIRECTION_VERTICAL:
                {
                    const float cell_size = 1.0 * (result.bounds.height - (count - 1) * gap) / count;
                    result.bounds.y += cursor * (cell_size + gap);
                    result.bounds.height = ceilf(weight * cell_size + (weight - 1) * gap);
                    break;
                }
            default:
                NOB_UNREACHABLE("Unknown spaced direction.");
            }

            layout->as.spaced.cursor += weight;
            return result;
        }
    case LAYOUT_RECTANGLE:
        result.bounds = layout->as.rectangle;
        return result;
    default:
        NOB_UNREACHABLE("Unknown layout kind.");
    }
}

static void dui__lay_begin(const int id, const int index, const bool tabOrderBack, const DUI_Layout layout)
{
    dui_ctx_begin_impl(id, CLITERAL(DUI_ContextData) { .index = index, .tabOrderBack = tabOrderBack });

    NOB_ASSERT(layout_stack_count < DUI_LAYOUT_STACK_CAPACITY);
    layout_stack[layout_stack_count] = layout;
    ++layout_stack_count;
}

void dui_lay_begin_screen_impl(const int id, const DUI_Layout_ScreenData data)
{
    const DUI_Layout layout = {
        .kind = LAYOUT_SCREEN,
        .parent_bounds = {0},
        .as.screen.padding = data.padding,
    };
    dui__lay_begin(id, data.index, false, layout);
}

void dui_lay_begin_anchored_impl(const int id, const DUI_Layout_AnchoredData data)
{
    const DUI_Layout_BoundsData bounds = dui__lay_rectangle_data(data);
    const DUI_Layout layout = {
        .kind = LAYOUT_ANCHOR,
        .parent_bounds = bounds.bounds,
        .as.anchored = {
            .inset_left = 0,
            .inset_top = 0,
            .inset_right = 0,
            .inset_bottom = 0,
            .gap = dui_env_spacing(data.gap),
        },
    };

    dui__lay_begin(id, data.index, bounds.tabOrderBack, layout);
}

void dui_lay_begin_stack_impl(const int id, const DUI_Layout_StackData data)
{
    const DUI_Layout_BoundsData bounds = dui__lay_rectangle_data(data);
    const DUI_Layout layout = {
        .kind = LAYOUT_STACK,
        .parent_bounds = bounds.bounds,
        .as.stack = {
            .direction = data.direction,
            .item_size = data.item_size,
            .gap = dui_env_spacing(data.gap),
            .inset_begin = 0,
            .inset_end = 0,
        },
    };

    dui__lay_begin(id, data.index, bounds.tabOrderBack, layout);
}

void dui_lay_begin_spaced_impl(const int id, const DUI_Layout_SpacedData data)
{
    const DUI_Layout_BoundsData bounds = dui__lay_rectangle_data(data);
    const DUI_Layout layout = {
        .kind = LAYOUT_SPACED,
        .parent_bounds = bounds.bounds,
        .as.spaced = {
            .direction = data.direction,
            .count = data.count,
            .gap = dui_env_spacing(data.gap),
            .cursor = 0,
        },
    };

    dui__lay_begin(id, data.index, bounds.tabOrderBack, layout);
}

void dui_lay_begin_rectangle_impl(const int id, const DUI_Layout_RectangleData data)
{
    const DUI_Layout_BoundsData bounds = dui__lay_rectangle_data(data);
    const DUI_Layout layout = {
        .kind = LAYOUT_RECTANGLE,
        .parent_bounds = bounds.bounds,
        .as.rectangle = data.rectangle,
    };

    dui__lay_begin(id, data.index, bounds.tabOrderBack, layout);
}

void dui_lay_spacing(const int amount)
{
    DUI_Layout* layout = &layout_stack[layout_stack_count - 1];
    if (layout->kind == LAYOUT_STACK)
    {
        layout->as.stack.inset_begin += amount;
    }
}

void dui_lay_end()
{
    NOB_ASSERT(layout_stack_count > 0);
    layout_stack_count--;

    dui_ctx_end();
}
