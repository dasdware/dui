#include <dui/context.h>
#include <dui/environment.h>

#include "nob.h"

DUI_ContextElement* dui_ctx()
{
    return dui_env()->context_stack_top;
}

static void dui__ctx_link_tab_order(DUI_Element* element, const bool tabOrderBack)
{
    DUI_ContextElement* parent = element->parent;

    if (tabOrderBack)
    {
        if (parent->tabOrderBackCursor == NULL)
        {
            parent->tabOrderBack = element;
            parent->tabOrderBackCursor = element;
            element->tabOrderPrev = NULL;
            element->tabOrderNext = NULL;
        }
        else
        {
            element->tabOrderNext = parent->tabOrderBackCursor;
            element->tabOrderPrev = NULL;

            parent->tabOrderBackCursor->tabOrderPrev = element;
            parent->tabOrderBackCursor = element;
        }
    }
    else
    {
        if (parent->tabOrderFrontCursor == NULL)
        {
            parent->tabOrderFront = element;
            parent->tabOrderFrontCursor = element;
            element->tabOrderPrev = NULL;
            element->tabOrderNext = NULL;
        }
        else
        {
            element->tabOrderPrev = parent->tabOrderFrontCursor;
            element->tabOrderNext = NULL;

            parent->tabOrderFrontCursor->tabOrderNext = element;
            parent->tabOrderFrontCursor = element;
        }
    }
}

int dui__ctx_next_index_by_id(const int id)
{
    DUI_ContextElement *ctx = dui_ctx();

    nob_da_foreach(DUI_ElementIndex, it, &ctx->indices)
    {
        if (it->id == id)
        {
            it->index += 1;
            return it->index;
        }
    }

    const DUI_ElementIndex element_index = {
        .id = id,
        .index = 0
    };
    nob_da_append(&ctx->indices, element_index);

    return element_index.index;
}

bool dui_ctx_element_by_id_impl(const int kind, const int id, const bool tabOrderBack, const bool disabled, const int size, void** element)
{
    DUI_ContextElement *ctx = dui_ctx();
    DUI_Environment *env = dui_env();
    const int index = dui__ctx_next_index_by_id(id);

    nob_da_foreach(DUI_Element*, it, &ctx->children)
    {
        DUI_Element* candidate = *it;
        if (candidate->kind == kind && candidate->id == id && candidate->index == index)
        {
            if (!disabled)
            {
                dui__ctx_link_tab_order(candidate, tabOrderBack);
            }
            *element = candidate;
            return false;
        }
    }

    DUI_Element *new_element = arena_alloc(&env->memory, size);
    memset(new_element, 0, size);

    new_element->kind = kind;
    new_element->id = id;
    new_element->index = index;
    new_element->parent = ctx;
    nob_da_append(&ctx->children, new_element);

    if (!disabled)
    {
        dui__ctx_link_tab_order(new_element, tabOrderBack);
    }
    *element = new_element;
    return true;
}

void dui_ctx_begin_impl(const int id, const DUI_ContextData data)
{
    static int element_kind_context = 0;
    if (element_kind_context == 0)
    {
        element_kind_context = dui_env_next_element_kind();
    }

    DUI_Environment *env = dui_env();

    DUI_ContextElement* element;
    dui_ctx_element_by_id(element_kind_context, id, data.tabOrderBack, false, element);
    element->tabOrderFrontCursor = NULL;
    element->tabOrderBackCursor = NULL;
    element->placed_at_back = data.tabOrderBack;
    element->indices.count = 0;

    env->context_stack_top = element;
}

void dui_ctx_end()
{
    DUI_Environment *env = dui_env();

    DUI_ContextElement* ctx = env->context_stack_top;
    DUI_ContextElement* parent = ctx->element.parent;

    assert(parent != NULL && "Cannot end a DUI context that has not begun.");

    if (ctx->tabOrderFrontCursor == NULL)
    {
        ctx->tabOrderFront = ctx->tabOrderBackCursor;
    }
    else if (ctx->tabOrderBackCursor == NULL)
    {
        ctx->tabOrderBack = ctx->tabOrderFrontCursor;
    }
    else if (ctx->tabOrderFrontCursor != ctx->tabOrderBackCursor)
    {
        ctx->tabOrderFrontCursor->tabOrderNext = ctx->tabOrderBackCursor;
        ctx->tabOrderBackCursor->tabOrderPrev = ctx->tabOrderFrontCursor;
    }

    if (!ctx->placed_at_back)
    {
        assert(parent->tabOrderFrontCursor == (DUI_Element*)ctx && "Inconsistent context nesting state");
        DUI_Element* before = parent->tabOrderFrontCursor->tabOrderPrev;
        if (before)
        {
            before->tabOrderNext = ctx->tabOrderFront;
            if (ctx->tabOrderFront)
            {
                ctx->tabOrderFront->tabOrderPrev = before;
            }
            parent->tabOrderFrontCursor = ctx->tabOrderBack;
        }
        else
        {
            parent->tabOrderFront = ctx->tabOrderFront;
            parent->tabOrderFrontCursor = ctx->tabOrderBack;
        }
    }
    else
    {
        assert(parent->tabOrderBackCursor == (DUI_Element*)ctx && "Inconsistent context nesting state");
        DUI_Element* after = parent->tabOrderBackCursor->tabOrderNext;
        if (after)
        {
            after->tabOrderPrev = ctx->tabOrderBack;
            if (ctx->tabOrderBack)
            {
                ctx->tabOrderBack->tabOrderNext = after;
            }
            parent->tabOrderBackCursor = ctx->tabOrderFront;
        }
        else
        {
            parent->tabOrderBack = ctx->tabOrderBack;
            parent->tabOrderBackCursor = ctx->tabOrderFront;
        }
    }

    if (parent->element.parent == NULL)
    {
        parent->tabOrderFront = ctx->tabOrderFront;
        parent->tabOrderBack = ctx->tabOrderBack;
    }

    env->context_stack_top = parent;
}
