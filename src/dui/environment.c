#include <raylib.h>
#include <dui/environment.h>
#include <dui/keyboard.h>
#include <dui/theme.h>

extern DUI_BoundsData dui__stack_next_bounds(void* element, DUI_Placement placement);
extern void dui__stack_reset(void* element);

DUI_Environment* dui_env() {
    static DUI_Environment environment = {0};
    if (!environment.initialized) {
        environment.root_element.layout_element.element.type = DUI_ROOT_TYPE_ID;
        environment.root_element.layout_element.callback = dui__stack_next_bounds;
        environment.root_element.layout_element.reset_callback = dui__stack_reset;
        environment.root_element.gap = 2;
        environment.element_stack_top = &environment.root_element.layout_element.element;
        environment.layout_element_stack_top = &environment.root_element.layout_element;

        environment.focus_frame_offset = 0;
        environment.focus_frame_width = 2;

        environment.font = LoadFontEx(DUI_FONT_FILE, DUI_FONT_SIZE, NULL, 0);

        environment.initialized = true;
    }
    return &environment;
}

void dui_env_clear_background() {
    ClearBackground(DUI_BACKGROUND_COLOR);
}

void dui_env_draw_focus_frame(const Rectangle bounds, const Color color) {
    const DUI_Environment* env = dui_env();

    const Rectangle focus_frame_bounds = {
        bounds.x - env->focus_frame_offset - env->focus_frame_width,
        bounds.y - env->focus_frame_offset - env->focus_frame_width,
        bounds.width + 2 * (env->focus_frame_offset + env->focus_frame_width),
        bounds.height + 2 * (env->focus_frame_offset + env->focus_frame_width),
    };
    DrawRectangleLinesEx(focus_frame_bounds, env->focus_frame_width, color);
}

void dui_env_focus_impl(DUI_Element* element) {
    dui_env()->focused_element = element;
}

bool dui_env_has_focus_impl(const DUI_Element* element) {
    return dui_env()->focused_element == element;
}

void dui_root_begin() {
    BeginDrawing();
    dui_env_clear_background();

    DUI_LayoutElement* root_layout = &dui_env()->root_element.layout_element;
    root_layout->element.bounds = (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()};
    root_layout->element.bounds = DUI_PADDING_ALL(root_layout->element.bounds, 3);

    if (root_layout->reset_callback) {
        root_layout->reset_callback(root_layout);
    }

    DUI_Element* root_element = &root_layout->element;
    root_element->tabOrderFrontCursor = NULL;
    root_element->tabOrderBackCursor = NULL;
    root_element->placed_at_back = false;
    root_element->indices.count = 0;

    DUI_Environment* env = dui_env();
    env->previous_window_count = env->window_count;
    for (size_t i = 0; i < env->window_count; ++i) {
        env->previous_windows_bounds[i] = env->windows[i].bounds;
    }
    env->window_count = 0;
}

void dui_root_end() {
    DUI_Element* root_element = &dui_env()->root_element.layout_element.element;
    if (root_element->tabOrderFrontCursor == NULL) {
        root_element->tabOrderFront = root_element->tabOrderBackCursor;
    } else if (root_element->tabOrderBackCursor == NULL) {
        root_element->tabOrderBack = root_element->tabOrderFrontCursor;
    } else if (root_element->tabOrderFrontCursor != root_element->tabOrderBackCursor) {
        root_element->tabOrderFrontCursor->tabOrderNext = root_element->tabOrderBackCursor;
        root_element->tabOrderBackCursor->tabOrderPrev = root_element->tabOrderFrontCursor;
    }

    if (root_element->tabOrderBack != NULL) {
        root_element->tabOrderBack->tabOrderNext = root_element->tabOrderFront;
    }
    if (root_element->tabOrderFront != NULL) {
        root_element->tabOrderFront->tabOrderBack = root_element->tabOrderBack;
    }

    const DUI_Element* focused_element = dui_env()->focused_element;
    if (focused_element) {
        dui_env_draw_focus_frame(focused_element->bounds, DUI_FOCUS_COLOR(focused_element->kind));
    }

    for (size_t i = 0; i < dui_env()->window_count; ++i) {
        const DUI_Window* window = &dui_env()->windows[i];
        const Rectangle bounds = {
            0, DUI_WINDOWS_HEIGHT_CAPACITY - window->bounds.height, window->bounds.width, -window->bounds.height
        };
        const Vector2 position = {window->bounds.x, window->bounds.y};
        if (window->transparency != 0) {
            const Color blend = {255, 255, 255, 255 * (1 - window->transparency)};
            DrawTextureRec(window->texture.texture, bounds, position, blend);
        } else {
            DrawTextureRec(window->texture.texture, bounds, position, WHITE);
        }
    }

    EndDrawing();

    dui_handle_global_keys();
}

DUI_Window* dui_enqueue_window(const Rectangle bounds, const float transparency) {
    NOB_ASSERT(dui_env()->window_count < DUI_WINDOWS_CAPACITY - 1);
    NOB_ASSERT(bounds.width <= DUI_WINDOWS_WIDTH_CAPACITY);
    NOB_ASSERT(bounds.height < DUI_WINDOWS_HEIGHT_CAPACITY);

    DUI_Window* window = &dui_env()->windows[dui_env()->window_count++];
    if (!window->allocated) {
        window->texture = LoadRenderTexture(DUI_WINDOWS_WIDTH_CAPACITY, DUI_WINDOWS_HEIGHT_CAPACITY);
        window->allocated = true;
    }

    window->bounds = bounds;
    window->transparency = transparency;

    return window;
}
