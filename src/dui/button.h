//
// Created by thomas on 04.01.2026.
//

#ifndef DUI_BUTTON_H
#define DUI_BUTTON_H

#include <dui/animation.h>
#include <dui/context.h>
#include <dui/environment.h>
#include <dui/layout.h>

#define ELEMENT_KIND_BUTTON 2

typedef DUI_EnvironmentColorKind DUI_ButtonKind;


typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;

    const char* caption;
    DUI_ButtonKind kind;
    bool disabled;
} DUI_ButtonData;

typedef enum
{
    BUTTON_NORMAL,
    BUTTON_HOVER,
    BUTTON_DOWN,
} DUI_Button_State;

typedef struct
{
    DUI_Element element;

    DUI_ColorAnimation background;
    DUI_Button_State state;
} DUI_ButtonElement;

#define dui_button(...) dui_button_impl(__COUNTER__, (DUI_ButtonData) { __VA_ARGS__ })
bool dui_button_impl(int id, DUI_ButtonData data);

#endif //DUI_BUTTON_H
