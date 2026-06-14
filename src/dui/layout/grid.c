#include <dui/layout/grid.h>
#include <dui/environment.h>

DUI_BoundsData dui__grid_next_bounds(void* element, const DUI_Placement placement) {
    DUI_GridElement* grid_element = element;

    DUI_BoundsData result = {
        .bounds = grid_element->layout_element.element.bounds,
        .tabOrderBack = false,
    };

    const int horizontal_weight = (grid_element->direction == DUI_HORIZONTAL)
                                      ? min(max(placement.weight, 1), grid_element->columns - grid_element->column)
                                      : 1;
    const int vertical_weight = (grid_element->direction == DUI_VERTICAL)
                                    ? min(max(placement.weight, 1), grid_element->rows - grid_element->row)
                                    : 1;
    const int gap_px = DUI_SPACING(grid_element->gap);
    const float cell_width = 1.0 * (result.bounds.width - (grid_element->columns - 1) * gap_px) /
        grid_element->columns;
    const float cell_height = 1.0 * (result.bounds.height - (grid_element->rows - 1) * gap_px) /
        grid_element->rows;

    result.bounds.x += grid_element->column * (cell_width + gap_px);
    result.bounds.y += grid_element->row * (cell_height + gap_px);
    result.bounds.width = horizontal_weight * cell_width + (horizontal_weight - 1) * gap_px;
    result.bounds.height = vertical_weight * cell_height + (vertical_weight - 1) * gap_px;

    if (grid_element->direction == DUI_HORIZONTAL) {
        grid_element->column += horizontal_weight;
        if (grid_element->column >= grid_element->columns) {
            grid_element->column = 0;
            grid_element->row += 1;
        }
    } else {
        grid_element->row += vertical_weight;
        if (grid_element->row >= grid_element->rows) {
            grid_element->row = 0;
            grid_element->column += 1;
        }
    }

    return result;
}

void dui__grid_reset(void* element) {
    DUI_GridElement* grid_element = element;
    grid_element->column = 0;
    grid_element->row = 0;
}

void dui_begin_grid_impl(const int id, const DUI_GridData data) {
    DUI_GridElement* element;
    dui_begin_layout(DUI_GRID_TYPE_ID, id, data.placement, element);
    element->layout_element.callback = dui__grid_next_bounds;
    element->layout_element.reset_callback = dui__grid_reset;
    element->direction = data.direction;
    element->columns = data.columns;
    element->rows = data.rows;
    element->gap = data.gap;
    element->row = 0;
    element->column = 0;
}

