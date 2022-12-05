#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "vm.h"

static void run_repl() {
    char line[1024];
    for (;;) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        interpret_source(line);
    }
}

static char* read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open file: \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read file: \"%s\".\n", path);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Couldn't read file: \"%s\".\n", path);
        exit(74);
    }
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static void run_script(const char *path) {
    char *source = read_file(path);
    lox_interpret_result result = interpret_source(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
    build_virtual_machine();

    if (argc == 1) {
        run_repl();
    } else if (argc == 2) {
        run_script(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
    }

    purge_virtual_machine();
    return 0;
}
