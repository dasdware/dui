#ifndef DUI_LAYOUT_GRID_H
#define DUI_LAYOUT_GRID_H

#include <dui/layout/layout.h>
#include <dui/element.h>

#define DUI_GRID_TYPE_ID DUI_ID("dui-grid", 8, 3961429178)

typedef struct {
    DUI_Placement placement;
    int columns;
    int rows;
    DUI_Direction direction;
    int gap;
} DUI_GridData;

typedef struct {
    DUI_LayoutElement layout_element;
    int columns;
    int rows;
    DUI_Direction direction;
    int gap;

    int row;
    int column;
} DUI_GridElement;

#define dui_grid(...) dui_grid_impl_(__COUNTER__, __VA_ARGS__)
#define dui_grid_impl_(id, ...) dui_grid_impl(id, __VA_ARGS__)
#define dui_grid_impl(id, ...) \
bool dui__guard_ ## id = true; \
    for (dui_begin_grid_impl(id, CLITERAL(DUI_GridData) { __VA_ARGS__ }); dui__guard_ ## id; dui_end_grid(), dui__guard_ ## id = false)

#define dui_begin_grid(...) dui_begin_grid_impl(__COUNTER__, CLITERAL(DUI_GridData) { __VA_ARGS__ })
void dui_begin_grid_impl(int id, DUI_GridData data);

#define dui_end_grid dui_end_layout


#endif // DUI_LAYOUT_GRID_H
