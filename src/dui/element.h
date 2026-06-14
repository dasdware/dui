#ifndef DUI_CONTEXT_H
#define DUI_CONTEXT_H

#include <stdbool.h>
#include <stddef.h>
#include <nob.h>
#include <raylib.h>

#include <dui/layout/types.h>
#include <dui/types.h>

// - Ids

typedef struct {
    Nob_String_View value;
    unsigned long hash;
} DUI_Id;

#define DUI_ID(value, length, hash) (CLITERAL(DUI_Id) { { (length), (const char*) (value) }, (hash) })

DUI_Id dui_create_id_cstr(const char* cstr);

// - Elements

#define DUI_ROOT_TYPE_ID DUI_ID("dui-root", 8, 3961821432)

typedef struct DUI_Element DUI_Element;

typedef struct {
    DUI_Element** items;
    size_t count;
    size_t capacity;
} DUI_Elements;

typedef struct {
    int id;
    int index;
} DUI_ElementIndex;

typedef struct {
    DUI_ElementIndex* items;
    size_t count;
    size_t capacity;
} DUI_ElementIndices;

struct DUI_Element {
    int id;
    int index;
    DUI_Id type;
    DUI_Kind kind;
    Rectangle bounds;

    DUI_Element* parent;
    DUI_Element* tabOrderNext;
    DUI_Element* tabOrderPrev;

    DUI_Elements children;
    DUI_ElementIndices indices;

    DUI_Element* tabOrderFront;
    DUI_Element* tabOrderFrontCursor;
    DUI_Element* tabOrderBack;
    DUI_Element* tabOrderBackCursor;

    bool placed_at_back;
};

typedef struct {
    bool tabOrderBack;
} DUI_ElementData;


DUI_Element* dui_element();

#define dui_begin_element(type, ...) dui_begin_element_impl(type, __COUNTER__, (DUI_ElementData) { __VA_ARGS__ })
void dui_begin_element_impl(DUI_Id type, int id, DUI_ElementData data);
void dui_end_element();

typedef enum {
    DUI_NEWLY_CREATED,
    DUI_CACHED
} DUI_ElementCacheState;

#define dui_get_element(type, id, kind, tabOrderBack, disabled, element) \
    dui_get_element_impl(type, id, kind, tabOrderBack, disabled, sizeof(*element), (void**)&element)
DUI_ElementCacheState dui_get_element_impl(
    DUI_Id type, int id, DUI_Kind kind, bool tabOrderBack, bool disabled, int size, void** element
);

#define dui_get_active_element(type, id, kind, disabled, element, placement) \
    dui_get_active_element_impl(type, id, kind, disabled, sizeof(*element), (void**)&element, placement)
DUI_ElementCacheState dui_get_active_element_impl(
    DUI_Id type, int id, DUI_Kind kind, bool disabled, int size,
    void** element, DUI_Placement placement
);

typedef struct {
    bool activated;
    DUI_State state;
} DUI_NextState;

DUI_NextState dui_next_state(DUI_State current_state, bool disabled, DUI_Element* element);

#endif //DUI_CONTEXT_H
