#ifndef DUI_TYPES_H
#define DUI_TYPES_H

#include <dui/context.h>

typedef enum
{
    ALIGN_DEFAULT,
    ALIGN_START,
    ALIGN_CENTER,
    ALIGN_END,
} DUI_Alignment;

#define ALIGN_LEFT ALIGN_START
#define ALIGN_TOP ALIGN_START
#define ALIGN_RIGHT ALIGN_END
#define ALIGN_BOTTOM ALIGN_END

typedef enum
{
    STATE_NORMAL,
    STATE_HOVER,
    STATE_DOWN,
    STATE_DISABLED,
} DUI_State;

typedef struct
{
    bool activated;
    DUI_State state;
} DUI_NextState;

DUI_NextState dui_next_state(DUI_State current_state, Rectangle bounds, bool disabled, DUI_Element* element);

#endif // DUI_TYPES_H