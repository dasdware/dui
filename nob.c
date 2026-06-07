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

    return 0;
}
