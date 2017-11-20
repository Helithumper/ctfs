#ifndef _MACHINE_H
#define _MACHINE_H

#include <stdint.h>
#include <string.h>

// The memory is 2 ** 16 4-byte words, so 2 ** 20 bytes
#define MEMORY_SIZE (1 << 20)
#define CALL_STACK_SIZE (1 << 16)

#define FLAG_ZERO	0x0001
#define FLAG_CARRY	0x0002
#define FLAG_FLAG	0x8000

#define OPCODE(inst) ((inst).cooked.opcode)
#define ADDRMODE(inst) ((inst).cooked.addrmode)
#define ADDRESS(inst) ((inst).cooked.address)

typedef struct {
    uint32_t *memory;
    uint16_t ip;
    uint16_t sp;
    uint16_t flags;

    uint16_t *callstack;
    size_t callstackptr;

    char *trap;

    union {
        uint32_t raw;
        struct {
            unsigned int opcode : 8;
            unsigned int addrmode : 3;
            unsigned int reserved : 5;
            unsigned int address : 16;
        } cooked;
    } inst;
} machine_state_t;

int
machine_init(machine_state_t *state, const char *image_filename);

void
machine_destroy(machine_state_t *state);

uint32_t
stack_pop(machine_state_t *state);

void
stack_push(machine_state_t *state, uint32_t value);

void
machine_print_state(machine_state_t *state);

#endif // _MACHINE_H
