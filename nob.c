#define NOB_IMPLEMENTATION
#include "thirdparty/nob.h"

#define BUILD_DIR "build/"
#define BUILD_OUTPUT(output) "./" BUILD_DIR output

#define RAYLIB_SRC_DIR "thirdparty/raylib/src/"
#define RAYLIB_BUILD_DIR BUILD_DIR "raylib/"

#define MAIN_OUTPUT BUILD_OUTPUT("main.exe")

static Nob_Cmd cmd = {0};

bool build_raylib() {
    static const char* raylib_modules[] = {
        "rcore",
        "raudio",
        "rglfw",
        "rmodels",
        "rshapes",
        "rtext",
        "rtextures",
        "utils",
    };

    bool result = true;
    Nob_File_Paths object_files = {0};

    if (!nob_mkdir_if_not_exists(RAYLIB_BUILD_DIR)) {
        nob_return_defer(true);
    }

    Nob_Procs procs = {0};

    // Add raylib modules
    for (size_t i = 0; i < NOB_ARRAY_LEN(raylib_modules); ++i) {
        const char* input_path = nob_temp_sprintf(RAYLIB_SRC_DIR "%s.c", raylib_modules[i]);
        const char* output_path = nob_temp_sprintf(RAYLIB_BUILD_DIR "%s.o", raylib_modules[i]);

        nob_da_append(&object_files, output_path);

        if (nob_needs_rebuild(output_path, &input_path, 1)) {
            cmd.count = 0;
            nob_cmd_append(&cmd, "gcc");
            nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb");

            nob_cmd_append(&cmd, "-DPLATFORM_DESKTOP");
            nob_cmd_append(&cmd, "-fPIC");
            nob_cmd_append(&cmd, "-I./" RAYLIB_SRC_DIR "/external/glfw/include");
            nob_cmd_append(&cmd, "-c", input_path);
            nob_cmd_append(&cmd, "-o", output_path);

            nob_cmd_run(&cmd, .async = &procs);
        }
    }

    if (!nob_procs_wait(procs))
        nob_return_defer(false);

    const char* libraylib_path = RAYLIB_BUILD_DIR "/libraylib.a";

    if (nob_needs_rebuild(libraylib_path, object_files.items, object_files.count)) {
        nob_cmd_append(&cmd, "ar", "-crs", libraylib_path);
        for (size_t i = 0; i < object_files.count; ++i) {
            nob_cmd_append(&cmd, object_files.items[i]);
        }
        if (!nob_cmd_run(&cmd))
            nob_return_defer(false);
    }

defer:
    nob_cmd_free(cmd);
    nob_da_free(object_files);
    return result;
}

bool build_main() {
    nob_cmd_append(&cmd, "gcc");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb", "-static", "-std=c99");
    nob_cmd_append(&cmd, "-I./src/");
    nob_cmd_append(&cmd, "-I./thirdparty/");
    nob_cmd_append(&cmd, "-I./thirdparty/raylib/src");
    nob_cmd_append(&cmd, "-L./" RAYLIB_BUILD_DIR);
    nob_cmd_append(&cmd, "./src/dui/animation.c");
    nob_cmd_append(&cmd, "./src/dui/button.c");
    nob_cmd_append(&cmd, "./src/dui/checkbox.c");
    nob_cmd_append(&cmd, "./src/dui/element.c");
    nob_cmd_append(&cmd, "./src/dui/edit.c");
    nob_cmd_append(&cmd, "./src/dui/environment.c");
    nob_cmd_append(&cmd, "./src/dui/keyboard.c");
    nob_cmd_append(&cmd, "./src/dui/label.c");
    nob_cmd_append(&cmd, "./src/dui/layout/layout.c");
    nob_cmd_append(&cmd, "./src/dui/layout/anchored.c");
    nob_cmd_append(&cmd, "./src/dui/layout/grid.c");
    nob_cmd_append(&cmd, "./src/dui/layout/screen.c");
    nob_cmd_append(&cmd, "./src/dui/layout/spaced.c");
    nob_cmd_append(&cmd, "./src/dui/layout/stack.c");
    nob_cmd_append(&cmd, "./src/dui/layout/window.c");
    nob_cmd_append(&cmd, "./src/dui/text.c");
    nob_cmd_append(&cmd, "./src/main.c");
    nob_cmd_append(&cmd, "-lraylib");
    nob_cmd_append(&cmd, "-lgdi32");
    nob_cmd_append(&cmd, "-lwinmm");
    nob_cmd_append(&cmd, "-lm");
    nob_cmd_append(&cmd, "-o", MAIN_OUTPUT);

    if (!nob_cmd_run(&cmd)) {
        return false;
    }

    return true;
}

unsigned long djb2_hash(const char* string, const size_t length) {
    unsigned long hash = 5381;
    for (size_t i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) + string[i]; /* hash * 33 + c */
    }
    return hash;
}

void generate_dui_type_id(const char* type_id) {
    const size_t length = strlen(type_id);
    const unsigned long hash = djb2_hash(type_id, length);
    printf("#define ");
    for (size_t i = 0; i < length; ++i) {
        if (isalpha(type_id[i])) {
            printf("%c", toupper(type_id[i]));
        } else if (isdigit(type_id[i])) {
            printf("%c", type_id[i]);
        } else {
            printf("_");
        }
    }
    printf("_TYPE_ID DUI_ID(\"%s\", %lld, %lu)\n", type_id, length, hash);
}

typedef enum {
    DUI_CREATE_CONTROL,
    DUI_CREATE_LAYOUT,
} DUI_CreateMode;

void append_uppercase_name(Nob_String_Builder* sb, Nob_String_View name) {
    for (size_t i = 0; i < name.count; ++i) {
        if (isdigit(name.data[i])) {
            nob_da_append(sb, name.data[i]);
        } else if (isalpha(name.data[i])) {
            nob_da_append(sb, toupper(name.data[i]));
        } else {
            nob_da_append(sb, '_');
        }
    }
}

void append_uppercase_first_name(Nob_String_Builder* sb, Nob_String_View name) {
    for (size_t i = 0; i < name.count; ++i) {
        if (isdigit(name.data[i])) {
            nob_da_append(sb, name.data[i]);
        } else if (isalpha(name.data[i])) {
            if (i == 0) {
                nob_da_append(sb, toupper(name.data[i]));
            } else {
                nob_da_append(sb, name.data[i]);
            }
        } else {
            nob_da_append(sb, '_');
        }
    }
}

void append_normalized_name(Nob_String_Builder* sb, Nob_String_View name) {
    for (size_t i = 0; i < name.count; ++i) {
        if (isalnum(name.data[i])) {
            nob_da_append(sb, name.data[i]);
        } else {
            nob_da_append(sb, '_');
        }
    }
}

void append_private_normalized_name(Nob_String_Builder* sb, Nob_String_View name) {
    bool have_prefix = false;
    for (size_t i = 0; i < name.count; ++i) {
        if (isalnum(name.data[i])) {
            nob_da_append(sb, name.data[i]);
        } else if (name.data[i] == '-' && !have_prefix) {
            nob_sb_append_cstr(sb, "__");
            have_prefix = true;
        } else {
            nob_da_append(sb, '_');
        }
    }
}

void append_include_guard_name(Nob_String_Builder* sb, Nob_String_View type_name, DUI_CreateMode create_mode) {
    append_uppercase_name(sb, type_name);
    if (create_mode == DUI_CREATE_LAYOUT) {
        nob_sb_append_cstr(sb, "_LAYOUT");
    }
    nob_sb_append_cstr(sb, "_H");
}

void append_type_id_name(Nob_String_Builder* sb, Nob_String_View type_name) {
    append_uppercase_name(sb, type_name);
    nob_sb_append_cstr(sb, "_TYPE_ID");
}

void append_type_name(Nob_String_Builder* sb, Nob_String_View type_name) {
    for (size_t i = 0; i < type_name.count; ++i) {
        if (isalpha(type_name.data[i])) {
            nob_da_append(sb, toupper(type_name.data[i]));
        } else if (type_name.data[i] == '-') {
            nob_da_append(sb, '_');
            append_uppercase_first_name(sb, nob_sv_from_parts(type_name.data + i + 1, type_name.count - i - 1));
            break;
        } else {
            nob_da_append(sb, type_name.data[i]);
        }
    }
}

void append_data_type_name(Nob_String_Builder* sb, Nob_String_View type_name) {
    append_type_name(sb, type_name);
    nob_sb_append_cstr(sb, "Data");
}

void append_element_type_name(Nob_String_Builder* sb, Nob_String_View type_name) {
    append_type_name(sb, type_name);
    nob_sb_append_cstr(sb, "Element");
}

void append_cliteral_data_type_name(Nob_String_Builder* sb, Nob_String_View type_name) {
    nob_sb_append_cstr(sb, "CLITERAL(");
    append_data_type_name(sb, type_name);
    nob_sb_append_cstr(sb, ")");
}

void create_header(Nob_String_Builder* sb, Nob_String_View type_name, DUI_CreateMode create_mode) {
    nob_sb_append_cstr(sb, "#ifndef ");
    append_include_guard_name(sb, type_name, create_mode);
    nob_sb_append_cstr(sb, "\n");
    nob_sb_append_cstr(sb, "#define ");
    append_include_guard_name(sb, type_name, create_mode);
    nob_sb_append_cstr(sb, "\n");
    nob_sb_append_cstr(sb, "\n");

    nob_sb_append_cstr(sb, "#include <dui/animation.h>\n");
    nob_sb_append_cstr(sb, "#include <dui/colors.h>\n");
    nob_sb_append_cstr(sb, "#include <dui/element.h>\n");
    nob_sb_append_cstr(sb, "#include <dui/layout/layout.h>\n");
    nob_sb_append_cstr(sb, "#include <dui/types.h>\n");
    nob_sb_append_cstr(sb, "\n");

    nob_sb_append_cstr(sb, "#define ");
    append_type_id_name(sb, type_name);

    const unsigned long type_name_hash = djb2_hash(type_name.data, type_name.count);
    nob_sb_appendf(
        sb, " DUI_ID(\""SV_Fmt"\", %d, %d)\n", SV_Arg(type_name), type_name.count, type_name_hash
    );
    nob_sb_append_cstr(sb, "\n");

    nob_sb_append_cstr(sb, "typedef struct {\n");
    nob_sb_append_cstr(sb, "    DUI_Placement placement;\n");
    nob_sb_append_cstr(sb, "\n");
    nob_sb_append_cstr(sb, "} ");
    append_data_type_name(sb, type_name);
    nob_sb_append_cstr(sb, ";\n");
    nob_sb_append_cstr(sb, "\n");

    nob_sb_append_cstr(sb, "typedef struct {\n");
    if (create_mode == DUI_CREATE_CONTROL) {
        nob_sb_append_cstr(sb, "    DUI_Element element;\n");
    } else if (create_mode == DUI_CREATE_LAYOUT) {
        nob_sb_append_cstr(sb, "    DUI_LayoutElement layout_element;\n");
    }
    nob_sb_append_cstr(sb, "\n");
    nob_sb_append_cstr(sb, "} ");
    append_element_type_name(sb, type_name);
    nob_sb_append_cstr(sb, ";\n");
    nob_sb_append_cstr(sb, "\n");

    if (create_mode == DUI_CREATE_CONTROL) {
        nob_sb_append_cstr(sb, "#define ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "(...) ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_impl(__COUNTER__, ");
        append_cliteral_data_type_name(sb, type_name);
        nob_sb_append_cstr(sb, " { __VA_ARGS__ })\n");

        nob_sb_append_cstr(sb, "int ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_impl(int id, ");
        append_data_type_name(sb, type_name);
        nob_sb_append_cstr(sb, " data);\n");
        nob_sb_append_cstr(sb, "\n");
    } else if (create_mode == DUI_CREATE_LAYOUT) {
        nob_sb_append_cstr(sb, "#define ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "(...) ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_impl_(__COUNTER__, __VA_ARGS__)\n");

        nob_sb_append_cstr(sb, "#define ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_impl_(id, ...) ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_impl(id, __VA_ARGS__)\n");

        nob_sb_append_cstr(sb, "#define ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_impl(id, ...) \\\n");
        nob_sb_append_cstr(sb, "    bool dui__guard_ ## id = true; \\\n");
        nob_sb_append_cstr(sb, "    for (");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_begin_impl(id, CLITERAL(");
        append_data_type_name(sb, type_name);
        nob_sb_append_cstr(sb, ") { __VA_ARGS__ }); dui__guard_ ## id; ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_end(), dui__guard_ ## id = false)\n");
        nob_sb_append_cstr(sb, "\n");

        nob_sb_append_cstr(sb, "#define ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_begin(...) ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_begin_impl(__COUNTER__, ");
        append_cliteral_data_type_name(sb, type_name);
        nob_sb_append_cstr(sb, " { __VA_ARGS__ })\n");
        nob_sb_append_cstr(sb, "\n");

        nob_sb_append_cstr(sb, "void ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_begin_impl(int id, ");
        append_data_type_name(sb, type_name);
        nob_sb_append_cstr(sb, " data);\n");
        nob_sb_append_cstr(sb, "\n");

        nob_sb_append_cstr(sb, "#define ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_end dui_end_layout\n");
        nob_sb_append_cstr(sb, "\n");
    }

    nob_sb_append_cstr(sb, "#endif // ");
    append_include_guard_name(sb, type_name, create_mode);
    nob_sb_append_cstr(sb, "\n");
}

void create_source(
    Nob_String_Builder* sb, const Nob_String_View type_name, const Nob_String_View short_name,
    const DUI_CreateMode create_mode
) {
    nob_sb_append_cstr(sb, "#include <dui/");
    if (create_mode == DUI_CREATE_LAYOUT) {
        nob_sb_append_cstr(sb, "layout/");
    }
    nob_sb_append_buf(sb, short_name.data, short_name.count);
    nob_sb_append_cstr(sb, ".h>\n");

    if (create_mode == DUI_CREATE_CONTROL) {
        nob_sb_append_cstr(sb, "\n");
        nob_sb_append_cstr(sb, "int ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_impl(int id, ");
        append_data_type_name(sb, type_name);
        nob_sb_append_cstr(sb, " data) {\n");
        nob_sb_append_cstr(sb, "    // TODO: Implementation\n");
        nob_sb_append_cstr(sb, "    return 0;\n");
        nob_sb_append_cstr(sb, "}\n");
    } else if (create_mode == DUI_CREATE_LAYOUT) {
        Nob_String_Builder sb_convert_element = {0};
        nob_sb_append_cstr(&sb_convert_element, "    ");
        append_element_type_name(&sb_convert_element, type_name);
        nob_sb_append_cstr(&sb_convert_element, "* ");
        nob_sb_append_buf(&sb_convert_element, short_name.data, short_name.count);
        nob_sb_append_cstr(&sb_convert_element, "_element = element;\n");

        nob_sb_append_cstr(sb, "#include <dui/environment.h>\n");
        nob_sb_append_cstr(sb, "\n");

        nob_sb_append_cstr(sb, "DUI_BoundsData ");
        append_private_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_next_bounds(void* element, const DUI_Placement placement) {\n");
        nob_sb_append_buf(sb, sb_convert_element.items, sb_convert_element.count);
        nob_sb_append_cstr(sb, "\n");
        nob_sb_append_cstr(sb, "    DUI_BoundsData result = {\n");
        nob_sb_append_cstr(sb, "        .bounds = ");
        nob_sb_append_buf(sb, short_name.data, short_name.count);
        nob_sb_append_cstr(sb, "_element->layout_element.element.bounds,\n");
        nob_sb_append_cstr(sb, "        .tabOrderBack = false,\n");
        nob_sb_append_cstr(sb, "    };\n");
        nob_sb_append_cstr(sb, "    // TODO: Implementation\n");
        nob_sb_append_cstr(sb, "    return result;\n");
        nob_sb_append_cstr(sb, "}\n");
        nob_sb_append_cstr(sb, "\n");

        nob_sb_append_cstr(sb, "void ");
        append_private_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_reset(void* element) {\n");
        nob_sb_append_buf(sb, sb_convert_element.items, sb_convert_element.count);
        nob_sb_append_cstr(sb, "    //  TODO: Implementation\n");
        nob_sb_append_cstr(sb, "}\n");
        nob_sb_append_cstr(sb, "\n");

        nob_sb_append_cstr(sb, "Vector2 ");
        append_private_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_transform_coordinate(\n");
        nob_sb_append_cstr(sb, "    void* element, Vector2 coordinate, DUI_TransformCoordinateDirection direction\n");
        nob_sb_append_cstr(sb, ") {\n");
        nob_sb_append_buf(sb, sb_convert_element.items, sb_convert_element.count);
        nob_sb_append_cstr(sb, "    //  TODO: Implementation\n");
        nob_sb_append_cstr(sb, "    return coordinate;\n");
        nob_sb_append_cstr(sb, "}\n");
        nob_sb_append_cstr(sb, "\n");

        nob_sb_append_cstr(sb, "void ");
        append_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_begin_impl(const int id, const ");
        append_data_type_name(sb, type_name);
        nob_sb_append_cstr(sb, " data) {\n");
        nob_sb_append_cstr(sb, "    ");
        append_element_type_name(sb, type_name);
        nob_sb_append_cstr(sb, "* element;\n");
        nob_sb_append_cstr(sb, "    dui_begin_inactive_layout(");
        append_type_id_name(sb, type_name);
        nob_sb_append_cstr(sb, ", id, element);\n");
        nob_sb_append_cstr(sb, "    element->layout_element.callback = ");
        append_private_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_next_bounds;\n");
        nob_sb_append_cstr(sb, "    element->layout_element.reset_callback = ");
        append_private_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_reset;\n");
        nob_sb_append_cstr(sb, "    element->layout_element.transform_coordinate_callback = ");
        append_private_normalized_name(sb, type_name);
        nob_sb_append_cstr(sb, "_transform_coordinate;\n");
        nob_sb_append_cstr(sb, "    // TODO: Further initialization\n");
        nob_sb_append_cstr(sb, "}\n");

        nob_sb_free(sb_convert_element);
    }
}

void create(const char* type, const DUI_CreateMode create_mode) {
    Nob_String_Builder base_filename = {0};
    nob_sb_append_cstr(&base_filename, "./src/dui/");
    if (create_mode == DUI_CREATE_LAYOUT) {
        nob_sb_append_cstr(&base_filename, "layout/");
    }

    const Nob_String_View type_name = nob_sv_from_cstr(type);

    Nob_String_View short_name = type_name;
    for (size_t i = short_name.count - 1; i > 0; --i) {
        if (short_name.data[i] == '-') {
            short_name.data += i + 1;
            short_name.count -= i + 1;
            break;
        }
    }
    nob_sb_append_buf(&base_filename, short_name.data, short_name.count);
    const size_t base_filename_length = base_filename.count;

    Nob_String_Builder sb_header = {0};
    create_header(&sb_header, type_name, create_mode);
    nob_sb_append_cstr(&base_filename, ".h");
    nob_sb_append_null(&base_filename);
    nob_write_entire_file(base_filename.items, sb_header.items, sb_header.count);
    base_filename.count = base_filename_length;

    Nob_String_Builder sb_source = {0};
    create_source(&sb_source, type_name, short_name, create_mode);
    nob_sb_append_cstr(&base_filename, ".c");
    nob_sb_append_null(&base_filename);
    nob_write_entire_file(base_filename.items, sb_source.items, sb_source.count);


    nob_sb_free(sb_header);
    nob_sb_free(sb_source);
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char* program = nob_shift(argv, argc);

    const char* command = NULL;
    if (argc > 0) {
        command = nob_shift(argv, argc);
    }

    if (command == NULL || strcmp(command, "build") == 0) {
        nob_mkdir_if_not_exists(BUILD_DIR);
        if (!build_raylib() || !build_main()) {
            return 1;
        }
    }

    if (command == NULL || strcmp(command, "run") == 0) {
        nob_cmd_append(&cmd, MAIN_OUTPUT);
        if (!nob_cmd_run(&cmd)) {
            return 1;
        }
    }

    if (strcmp(command, "type_id") == 0) {
        while (argc > 0) {
            generate_dui_type_id(nob_shift(argv, argc));
        }
    }

    if (strcmp(command, "create-control") == 0) {
        while (argc > 0) {
            create(nob_shift(argv, argc), DUI_CREATE_CONTROL);
        }
    }
    if (strcmp(command, "create-layout") == 0) {
        while (argc > 0) {
            create(nob_shift(argv, argc), DUI_CREATE_LAYOUT);
        }
    }
    return 0;
}
