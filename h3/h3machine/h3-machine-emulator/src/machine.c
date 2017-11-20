#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "machine.h"

uint32_t * load_image(const char *filename);

void unload_image(uint32_t *memory);

int
machine_init(machine_state_t *state, const char *image_filenane) {
    if ((state->memory = load_image(image_filenane)) == NULL) {
        return 1;
    }
    state->ip = 0x0001;
    state->sp = 0x0000;
    state->flags = 0x0000;

    state->trap = NULL;

    if ((state->callstack = calloc(sizeof(uint16_t), CALL_STACK_SIZE)) == NULL) {
        perror("calloc");
        unload_image(state->memory);
        return -1;
    }
    state->callstackptr = 0;

    return 0;
}

void
machine_destroy(machine_state_t *state) {
    unload_image(state->memory);
    free(state->callstack);
}

uint32_t
stack_pop(machine_state_t *state) {
    if (state->sp == 0x0000) {
        state->trap = "Stack underflow!";
        return 0xDEADBEEF;
    }
    return state->memory[state->sp++];
}

void
stack_push(machine_state_t *state, uint32_t value) {
    if (state->trap)
        return;
    state->memory[--state->sp] = value;
    if (value == 0) {
        state->flags |= FLAG_ZERO;
    } else {
        state->flags &= ~FLAG_ZERO;
    }
}

void
machine_print_state(machine_state_t *state) {
    printf("Stack:\n");
    if (state->sp) {
        uint16_t sp;
        for(sp = 0xffff; sp > state->sp; sp--) {
            printf("%04x: %08x\n", sp, state->memory[sp]);
        }
        if (!state->trap && state->flags & FLAG_FLAG) {
            printf("%04x: flag{%08x}\n", sp, state->memory[sp]);
        } else {
            printf("%04x: %08x\n", sp, state->memory[sp]);
        }
    }
    printf("\nRegisters:\n");
    printf("IP: %04x\n", state->ip);
    printf("SP: %04x\n", state->sp);
    printf("Flags: ");
    if (state->flags & FLAG_ZERO)
        printf("Z");
    else
        printf(" ");
    if (state->flags & FLAG_CARRY)
        printf("C");
    else
        printf(" ");
    if (state->flags & FLAG_FLAG)
        printf("F");
    else
        printf(" ");
    printf("\n");

    if (state->trap)
        printf("%s\n", state->trap);
}

void
unload_image(uint32_t *memory) {
    if (munmap(memory, MEMORY_SIZE) < 0) {
        perror("munmap");
    }
}

uint32_t *
load_image(const char *filename) {
    uint32_t *memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
    if (!memory) {
        perror("mmap");
        return NULL;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        unload_image(memory);
        return NULL;
    }

    int read_bytes = 0;
    while (read_bytes < MEMORY_SIZE) {
        int n = read(fd, memory + read_bytes, MEMORY_SIZE - read_bytes);
        if (n < 0) {
            perror("read");
            unload_image(memory);
            return NULL;
        } else if (n == 0) {
            break;
        } else {
            read_bytes += n;
        }
    }

    if (close(fd) < 0) {
        perror("close");
        unload_image(memory);
        return NULL;
    }

    return memory;
}
