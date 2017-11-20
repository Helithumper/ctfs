H3ASM = ./h3asm
CFLAGS = --std=gnu99 -Wall -Wextra -Werror -Wpedantic

GENERATED = src/opcodes.h src/opcode_lookup.c

all: h3emu

examples: examples/trivial.h3i

%.h3i: %.h3a
	$(H3ASM) -o $@ $<

codegen: $(GENERATED)

src/%: h3assembler/templates/%
	pipenv run python -c 'import h3assembler.codegen; h3assembler.codegen.main()' $@

src/machine.o: src/machine.c src/machine.h

src/opcodes.o: src/opcodes.c src/opcodes.h src/machine.h

src/opcode_lookup.o: src/opcode_lookup.c src/opcode_lookup.h src/opcodes.h src/machine.h

src/h3emu.o: src/machine.h src/opcode_lookup.h src/opcodes.h

h3emu: src/h3emu.o src/machine.o src/opcodes.o src/opcode_lookup.o
	$(CC) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o $@ $^

.PHONY: clean all codegen

clean:
	$(RM) examples/*.h3i src/*.o $(GENERATED)
