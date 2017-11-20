#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "machine.h"
#include "opcode_lookup.h"

machine_state_t state;
int trace = 0;

struct option longopts[] = {
    {"trace", no_argument, 0, 't'},
};

void
run_machine() {
    int stop = 0;
    while (!stop && !state.trap) {
        state.inst.raw = state.memory[state.ip++];
        uint8_t opcode = OPCODE(state.inst);
        uint16_t address = ADDRESS(state.inst);
        opcode_f do_op = opcode_handlers[opcode];
        if (!do_op) {
            state.trap = "Unrecognized opcode!";
            break;
        }
        if (trace) {
            printf("%04x: %s", state.ip - 1, opcode_names[opcode]);
            switch (ADDRMODE(state.inst)) {
                case 0x0:
                    break;
                case 0x1:
                    printf(" +%04x", address);
                    break;
                case 0x2:
                    printf(" $%04x", address);
                    break;
                case 0x3:
                    printf(" [%04x]", address);
                    break;
                case 0x4:
                    printf(" %04x", address);
                    break;
                default:
                    break;
            }
            printf("\n");
        }
        stop = do_op(&state);
    }
}

int
main(int argc, char **argv) {
    int c;
    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, "t", longopts, &option_index);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 't':
                trace = 1;
                break;
            case '?':
                break;
            default:
                fprintf(stderr, "Unrecognized option %c\n", c);
                return 1;
        }
    }

    if (optind == argc) {
        fprintf(stderr, "You must specify an image.\n");
        return 1;
    }

    machine_init(&state, argv[optind++]);

    while (optind < argc) {
        uint32_t val = strtol(argv[optind++], NULL, 16);
        stack_push(&state, val);
    }

    run_machine();

    machine_print_state(&state);

    return 0;
}
