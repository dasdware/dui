#ifndef DUI_LABEL_H
#define DUI_LABEL_H

#include <dui/environment.h>
#include <dui/layout.h>

typedef DUI_EnvironmentColorKind DUI_LabelKind;

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;

    DUI_LabelKind kind;
} DUI_LabelData;

#define dui_label(caption, ...) dui_label_impl(caption, (DUI_LabelData) { __VA_ARGS__ })
void dui_label_impl(const char* caption, DUI_LabelData data);

#endif //DUI_LABEL_H
