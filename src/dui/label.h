#ifndef DUI_LABEL_H
#define DUI_LABEL_H

#include <dui/colors.h>
#include <dui/layout.h>

// Label theming
#define DUI_LABEL_TEXT_COLOR(kind) DUI_SHADE_DISPATCH(kind, 500, 500, 300)

typedef DUI_PaletteKind DUI_LabelKind;

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;

    DUI_LabelKind kind;
} DUI_LabelData;

#define dui_label(caption, ...) dui_label_impl(caption, (DUI_LabelData) { __VA_ARGS__ })
void dui_label_impl(const char* caption, DUI_LabelData data);

#endif //DUI_LABEL_H
