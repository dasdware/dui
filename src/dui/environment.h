#ifndef DUI_ENVIRONMENT_H
#define DUI_ENVIRONMENT_H

#include <stdbool.h>
#include <arena.h>

#include <dui/colors.h>
#include <dui/element.h>
#include <dui/layout/layout.h>
#include <dui/layout/stack.h>

typedef struct {
    RenderTexture2D texture;
    bool allocated;
    Rectangle bounds;
} DUI_Window;

#define DUI_WINDOWS_WIDTH_CAPACITY 512
#define DUI_WINDOWS_HEIGHT_CAPACITY 512
#define DUI_WINDOWS_CAPACITY 8

typedef struct {
    Arena memory;
    bool initialized;

    DUI_StackElement root_element;
    DUI_Element* element_stack_top;
    DUI_LayoutElement* layout_element_stack_top;

    int focus_frame_offset;
    int focus_frame_width;

    Font font;

    DUI_Element* focused_element;

    DUI_Window windows[DUI_WINDOWS_CAPACITY];
    size_t window_count;
    Rectangle previous_windows_bounds[DUI_WINDOWS_CAPACITY];
    size_t previous_window_count;
} DUI_Environment;

DUI_Environment* dui_env();

#define dui_root() dui_root_impl_(__COUNTER__)
#define dui_root_impl_(id) dui_root_impl(id)
#define dui_root_impl(id) \
    bool dui__guard_ ## id = true; \
    for (dui_root_begin(); dui__guard_ ## id; dui_root_end(), dui__guard_ ## id = false)
void dui_root_begin();
void dui_root_end();

void dui_env_clear_background();
void dui_env_draw_focus_frame(Rectangle bounds, Color color);

#define dui_env_focus(element) dui_env_focus_impl((DUI_Element*) (element))
void dui_env_focus_impl(DUI_Element* element);

#define dui_env_has_focus(element) dui_env_has_focus_impl((DUI_Element*) (element))
bool dui_env_has_focus_impl(const DUI_Element* element);

DUI_Window* dui_enqueue_window(Rectangle bounds);

#endif // DUI_ENVIRONMENT_H
