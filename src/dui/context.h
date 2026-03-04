#ifndef DUI_CONTEXT_H
#define DUI_CONTEXT_H

#include <stdbool.h>
#include <stddef.h>
#include <raylib.h>

#include <dui/layout.h>
#include <dui/types.h>

// - Elements

typedef struct DUI_Element DUI_Element;
typedef struct DUI_ContextElement DUI_ContextElement;

struct DUI_Element {
    int id;
    int index;
    int type;
    DUI_Kind kind;
    Rectangle bounds;

    DUI_ContextElement* parent;
    DUI_Element *tabOrderNext;
    DUI_Element *tabOrderPrev;
};

typedef struct
{
    DUI_Element **items;
    size_t count;
    size_t capacity;
} DUI_Elements;

typedef struct
{
    int id;
    int index;
} DUI_ElementIndex;

typedef struct
{
    DUI_ElementIndex *items;
    size_t count;
    size_t capacity;
} DUI_ElementIndices;

// - Contexts

struct DUI_ContextElement
{
    DUI_Element element;

    DUI_Elements children;
    DUI_ElementIndices indices;

    DUI_Element* tabOrderFront;
    DUI_Element* tabOrderFrontCursor;
    DUI_Element* tabOrderBack;
    DUI_Element* tabOrderBackCursor;

    bool placed_at_back;
};

typedef struct
{
    int index;
    bool tabOrderBack;
} DUI_ContextData;

DUI_ContextElement* dui_ctx();

#define dui_ctx_begin(...) dui_ctx_begin_impl(__COUNTER__, (DUI_ContextData) { __VA_ARGS__ })
void dui_ctx_begin_impl(int id, DUI_ContextData data);
void dui_ctx_end();

#define dui_ctx_element_by_id(type, id, kind, tabOrderBack, disabled, element) \
    dui_ctx_element_by_id_impl(type, id, kind, tabOrderBack, disabled, sizeof(*element), (void**)&element)
bool dui_ctx_element_by_id_impl(int type, int id, DUI_Kind kind, bool tabOrderBack, bool disabled, int size, void** element);

#define dui_ctx_active_element_by_id(type, id, kind, disabled, element, ...) \
    dui_ctx_active_element_by_id_impl(type, id, kind, disabled, sizeof(*element), (void**)&element, CLITERAL(DUI_Layout_Data) { __VA_ARGS__ })
bool dui_ctx_active_element_by_id_impl(int type, int id, DUI_Kind kind, bool disabled, int size,
                                       void** element, DUI_Layout_Data layout_data);

void* dui_ctx_allocate_state(int id, int index, size_t size);
void* dui_ctx_state_by_id(int id, int index);

typedef struct
{
    bool activated;
    DUI_State state;
} DUI_NextState;

DUI_NextState dui_ctx_next_state(DUI_State current_state, Rectangle bounds, bool disabled, DUI_Element* element);

#endif //DUI_CONTEXT_H
