#ifndef DUI_LAYOUT_H
#define DUI_LAYOUT_H

#include <raylib.h>

#ifndef DUI_LAYOUT_STACK_CAPACITY
#   define DUI_LAYOUT_STACK_CAPACITY 32
#endif

typedef enum
{
    LAYOUT_SCREEN,
    LAYOUT_ANCHOR,
    LAYOUT_STACK,
    LAYOUT_SPACED,
    LAYOUT_RECTANGLE,
} RL_LayoutKind;

typedef enum
{
    DIRECTION_VERTICAL,
    DIRECTION_HORIZONTAL,
} RL_LayoutDirection;

typedef enum
{
    ANCHOR_REMAINING,
    ANCHOR_LEFT,
    ANCHOR_TOP,
    ANCHOR_RIGHT,
    ANCHOR_BOTTOM,
} DUI_Layout_Anchor;

typedef struct
{
    int padding;
} RL_LayoutPadding;

typedef struct
{
    int gap;
    int inset_left;
    int inset_top;
    int inset_right;
    int inset_bottom;
} RL_LayoutAnchored;

typedef struct
{
    RL_LayoutDirection direction;
    int item_size;
    int gap;
    int inset_begin;
    int inset_end;
} RL_LayoutStack;

typedef struct
{
    RL_LayoutDirection direction;
    int count;
    int gap;
    int cursor;
} RL_LayoutSpaced;

typedef union
{
    RL_LayoutPadding screen;
    RL_LayoutAnchored anchored;
    RL_LayoutStack stack;
    RL_LayoutSpaced spaced;
    Rectangle rectangle;
} RL_LayoutAs;

typedef struct
{
    RL_LayoutKind kind;
    Rectangle parent_bounds;
    RL_LayoutAs as;
} DUI_Layout;

#define DUI_LAYOUT_DATA_FIELDS \
    int index; \
    int size; \
    int width; \
    int height; \
    int weight; \
    bool remaining; \
    bool opposite; \
    DUI_Layout_Anchor anchor

Rectangle dui_lay_padding(Rectangle bounds, int left, int top, int right, int bottom);
#define dui_lay_padding_all(bounds, padding) dui_lay_padding(bounds, padding, padding, padding, padding)
#define dui_lay_padding_symmetric(bounds, horizontal, vertical) dui_lay_padding(bounds, horizontal, vertical, horizontal, vertical)

Rectangle dui_lay_center(Rectangle bounds, int width, int height);

void dui_lay_spacing(int amount);

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;
} DUI_Layout_Data;

typedef struct
{
    Rectangle bounds;
    bool tabOrderBack;
} DUI_Layout_BoundsData;

#define dui_lay_rectangle(...) dui_lay_rectangle_impl(CLITERAL(DUI_Layout_Data) { __VA_ARGS__ })
DUI_Layout_BoundsData dui_lay_rectangle_impl(DUI_Layout_Data data);

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;
    int padding;
} DUI_Layout_ScreenData;

#define dui_lay_begin_screen(...) dui_lay_begin_screen_impl(__COUNTER__, CLITERAL(DUI_Layout_ScreenData) { __VA_ARGS__ })
void dui_lay_begin_screen_impl(int id, DUI_Layout_ScreenData data);

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;
    int gap;
} DUI_Layout_AnchoredData;

#define dui_lay_begin_anchored(...) dui_lay_begin_anchored_impl(__COUNTER__, CLITERAL(DUI_Layout_AnchoredData) { __VA_ARGS__ })
void dui_lay_begin_anchored_impl(int id, DUI_Layout_AnchoredData data);

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;
    RL_LayoutDirection direction;
    int item_size;
    int gap;
} DUI_Layout_StackData;

#define dui_lay_begin_stack(...) dui_lay_begin_stack_impl(__COUNTER__, CLITERAL(DUI_Layout_StackData) { __VA_ARGS__ })
void dui_lay_begin_stack_impl(int id, DUI_Layout_StackData data);

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;
    RL_LayoutDirection direction;
    int count;
    int gap;
} DUI_Layout_SpacedData;

#define dui_lay_begin_spaced(...) dui_lay_begin_spaced_impl(__COUNTER__, CLITERAL(DUI_Layout_SpacedData) { __VA_ARGS__ })
void dui_lay_begin_spaced_impl(int id, DUI_Layout_SpacedData data);

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;
    Rectangle rectangle;
} DUI_Layout_RectangleData;

#define dui_lay_begin_rectangle(...) dui_lay_begin_rectangle_impl(__COUNTER__, CLITERAL(DUI_Layout_RectangleData) { __VA_ARGS__ })
void dui_lay_begin_rectangle_impl(int id, DUI_Layout_RectangleData data);

void dui_lay_end();

#endif // DUI_LAYOUT_H