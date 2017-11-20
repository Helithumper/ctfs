#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "machine.h"
#include "opcodes.h"

uint32_t get_first_op(machine_state_t *state) {
    uint16_t addr = ADDRESS(state->inst);
    switch (ADDRMODE(state->inst)) {
        case 0x0:
            return stack_pop(state);
        case 0x1:
            return state->memory[(state->sp + addr) & 0xffff];
        case 0x2:
            return state->memory[addr];
        case 0x3:
            return state->memory[(stack_pop(state) + addr) & 0xffff];
        case 0x4:
            return addr;
        default:
            state->trap = "Unknown addressing mode.";
            return 0;
    }
}

int opcode_halt(__attribute__((unused)) machine_state_t *state) {
    return 1;
}

int opcode_drop(machine_state_t *state) {
    state->sp++;
    return 0;
}

int opcode_swap(machine_state_t *state) {
    uint32_t first = get_first_op(state);
    uint32_t second = stack_pop(state);
    stack_push(state, first);
    stack_push(state, second);
    return 0;
}

int opcode_push(machine_state_t *state) {
    stack_push(state, get_first_op(state));
    return 0;
}

int opcode_pop(machine_state_t *state) {
    uint32_t value = stack_pop(state);
    uint16_t addr = ADDRESS(state->inst);
    switch (ADDRMODE(state->inst)) {
        case 0x0:
            stack_push(state, value);
            break;
        case 0x1:
            state->memory[(state->sp + addr) & 0xffff] = value;
            break;
        case 0x2:
            state->memory[addr] = value;
            break;
        case 0x3:
            state->memory[(stack_pop(state) + addr) & 0xffff] = value;
            break;
        case 0x4:
            break;
        default:
            state->trap = "Unknown addressing mode.";
            break;
    }
    return 0;
}

void
update_cf(machine_state_t *state, bool cf) {
    if (cf) {
        state->flags |= FLAG_CARRY;
    } else {
        state->flags &= ~FLAG_CARRY;
    }
}

int opcode_add(machine_state_t *state) {
    uint32_t res;
    update_cf(state, __builtin_add_overflow(get_first_op(state), stack_pop(state), &res));
    stack_push(state, res);
    return 0;
}

int opcode_sub(machine_state_t *state) {
    uint32_t subtrahend = get_first_op(state);
    uint32_t minuend = stack_pop(state);
    uint32_t res;
    update_cf(state, __builtin_sub_overflow(minuend, subtrahend, &res));
    stack_push(state, res);
    return 0;
}

int opcode_mul(machine_state_t *state) {
    uint32_t res;
    update_cf(state, __builtin_mul_overflow(get_first_op(state), stack_pop(state), &res));
    stack_push(state, res);
    return 0;
}

int opcode_div(machine_state_t *state) {
    uint32_t divisor = get_first_op(state);
    uint32_t dividend = stack_pop(state);
    if (divisor == 0) {
        state->trap = "Divide by zero!";
        return 1;
    }
    stack_push(state, dividend / divisor);
    state->flags &= ~FLAG_CARRY;
    return 0;
}

int opcode_mod(machine_state_t *state) {
    uint32_t divisor = get_first_op(state);
    uint32_t dividend = stack_pop(state);
    if (divisor == 0) {
        state->trap = "Divide by zero!";
        return 1;
    }
    stack_push(state, dividend % divisor);
    state->flags &= ~FLAG_CARRY;
    return 0;
}

int opcode_negate(machine_state_t *state) {
    stack_push(state, -get_first_op(state));
    state->flags &= ~FLAG_CARRY;
    return 0;
}

int opcode_and(machine_state_t *state) {
    stack_push(state, get_first_op(state) & stack_pop(state));
    return 0;
}

int opcode_or(machine_state_t *state) {
    stack_push(state, get_first_op(state) | stack_pop(state));
    return 0;
}

int opcode_not(machine_state_t *state) {
    stack_push(state, ~get_first_op(state));
    return 0;
}

int opcode_xor(machine_state_t *state) {
    stack_push(state, get_first_op(state) ^ stack_pop(state));
    return 0;
}

int opcode_nand(machine_state_t *state) {
    stack_push(state, ~(get_first_op(state) & stack_pop(state)));
    return 0;
}

int opcode_nor(machine_state_t *state) {
    stack_push(state, ~(get_first_op(state) | stack_pop(state)));
    return 0;
}

int opcode_ashift(machine_state_t *state) {
    int32_t distance = (int32_t) get_first_op(state);
    int32_t value = (int32_t) stack_pop(state);
    if (distance < 0) {
        value <<= -distance;
    } else {
        value >>= distance;
    }
    stack_push(state, (uint32_t) value);
    return 0;
}

int opcode_lshift(machine_state_t *state) {
    int32_t distance = (int32_t) get_first_op(state);
    uint32_t value = stack_pop(state);
    if (distance < 0) {
        value >>= -distance;
    } else {
        value <<= distance;
    }
    stack_push(state, value);
    return 0;
}

int opcode_rotate(machine_state_t *state) {
    int32_t distance = (int32_t) get_first_op(state);
    uint32_t value = stack_pop(state);
    if (distance < 0) {
        value = (value << -distance) | (value >> (32 + distance));
    } else {
        value = (value >> distance) | (value << (32 - distance));
    }
    stack_push(state, value);
    return 0;
}

int opcode_jmp(machine_state_t *state) {
    state->ip = get_first_op(state);
    return 0;
}

int opcode_jz(machine_state_t *state) {
    if (state->flags & FLAG_ZERO)
        state->ip = get_first_op(state);
    return 0;
}

int opcode_jc(machine_state_t *state) {
    if (state->flags & FLAG_CARRY)
        state->ip = get_first_op(state);
    return 0;
}

int opcode_setf(machine_state_t *state) {
    state->flags |= get_first_op(state) & 0xffff;
    return 0;
}

int opcode_clf(machine_state_t *state) {
    state->flags &= ~(get_first_op(state) & 0xffff);
    return 0;
}

int opcode_call(machine_state_t *state) {
    if (state->callstackptr >= CALL_STACK_SIZE) {
        fprintf(stderr, "Call depth exceeded.\n");
        return 1;
    }

    state->callstack[state->callstackptr++] = state->ip;

    state->ip = get_first_op(state);

    return 0;
}

int opcode_ret(machine_state_t *state) {
    if (state->callstackptr <= 0) {
        fprintf(stderr, "Return not preceeded by call.\n");
        return 1;
    }

    state->ip = state->callstack[--state->callstackptr];

    return 0;
}
