#ifndef DUI_ENVIRONMENT_H
#define DUI_ENVIRONMENT_H

#include <math.h>
#include <stdbool.h>
#include <arena.h>

#include <dui/colors.h>
#include <dui/context.h>

typedef struct {
    Arena memory;
    bool initialized;

    DUI_ContextElement root_context;
    DUI_ContextElement* context_stack_top;

    int focus_frame_offset;
    int focus_frame_width;

    Font font;

    DUI_Element* focused_element;
} DUI_Environment;

DUI_Environment* dui_env();
void dui_env_begin();
void dui_env_end();

void dui_env_clear_background();
void dui_env_draw_focus_frame(Rectangle bounds, Color color);

#define dui_env_focus(element) dui_env_focus_impl((DUI_Element*) (element))
void dui_env_focus_impl(DUI_Element* element);

#define dui_env_has_focus(element) dui_env_has_focus_impl((DUI_Element*) (element))
bool dui_env_has_focus_impl(const DUI_Element* element);

int dui_env_next_element_type();

#endif // DUI_ENVIRONMENT_H
