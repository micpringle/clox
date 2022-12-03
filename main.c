#include "common.h"
#include "debug.h"
#include "vm.h"
#include "chunk.h"

int main(int argc, const char* argv[]) {
    build_virtual_machine();

    lox_chunk chunk;
    build_chunk(&chunk);

    int index = add_constant(&chunk, 1.2);
    write_chunk(&chunk, OP_CONSTANT, 100);
    write_chunk(&chunk, index, 100);

    index = add_constant(&chunk, 3.4);
    write_chunk(&chunk, OP_CONSTANT, 100);
    write_chunk(&chunk, index, 100);

    write_chunk(&chunk, OP_ADD, 100);

    index = add_constant(&chunk, 5.6);
    write_chunk(&chunk, OP_CONSTANT, 100);
    write_chunk(&chunk, index, 100);

    write_chunk(&chunk, OP_DIVIDE, 100);
    write_chunk(&chunk, OP_NEGATE, 100);

    write_chunk(&chunk, OP_RETURN, 100);

    disassemble_chunk(&chunk, "test chunk");
    interpret_chunk(&chunk);

    purge_chunk(&chunk);
    purge_virtual_machine();

    return 0;
}
