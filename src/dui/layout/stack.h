#ifndef DUI_LAYOUT_STACK_H
#define DUI_LAYOUT_STACK_H

#include <dui/layout/layout.h>
#include <dui/element.h>

#define DUI_STACK_TYPE_ID DUI_ID("dui-stack", 9, 1892438282)

typedef struct {
    DUI_LayoutData layout_data;

    DUI_Direction direction;
    int item_size;
    int gap;
} DUI_StackData;

typedef struct {
    DUI_LayoutElement layout_element;

    DUI_Direction direction;
    int item_size;
    int gap;
    int inset_begin;
    int inset_end;
} DUI_StackElement;

#define dui_stack(...) dui_stack_impl_(__COUNTER__, __VA_ARGS__)
#define dui_stack_impl_(id, ...) dui_stack_impl(id, __VA_ARGS__)
#define dui_stack_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_begin_stack_impl(id, CLITERAL(DUI_StackData) { __VA_ARGS__ }); dui__guard_ ## id; dui_end_stack(), dui__guard_ ## id = false)

#define dui_begin_stack(...) dui_begin_stack_impl(__COUNTER__, CLITERAL(DUI_StackData) { __VA_ARGS__ })
void dui_begin_stack_impl(int id, DUI_StackData data);

#define dui_end_stack dui_end_layout

#endif //DUI_LAYOUT_STACK_H
