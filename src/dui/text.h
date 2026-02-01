#ifndef DUI_TEXT_H
#define DUI_TEXT_H

#include <raylib.h>

#include <dui/types.h>

Vector2 dui_text_measure(const char* text);

typedef struct
{
    DUI_Alignment horizontal_alignment;
    DUI_Alignment vertical_alignment;
    Color color;
    int offset_x;
    int offset_y;
} DUI_TextData;

#define dui_text(text, bounds, ...) \
dui_text_impl(text, bounds, CLITERAL(DUI_TextData) { __VA_ARGS__ })

void dui_text_impl(const char* text, const Rectangle bounds, const DUI_TextData data);


#endif // DUI_TEXT_H
