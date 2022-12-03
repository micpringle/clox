#include "common.h"
#include "debug.h"
#include "chunk.h"

int main(int argc, const char* argv[]) {
    lox_chunk chunk;
    build_chunk(&chunk);

    int index = add_constant(&chunk, 1.2);
    write_chunk(&chunk, OP_CONSTANT, 100);
    write_chunk(&chunk, index, 100);

    write_chunk(&chunk, OP_RETURN, 100);
    disassemble_chunk(&chunk, "test chunk");
    purge_chunk(&chunk);

    return 0;
}
