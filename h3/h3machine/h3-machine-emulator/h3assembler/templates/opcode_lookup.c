#include "opcode_lookup.h"

opcode_f opcode_handlers[256] = {
{%- for code, name in opcodes %}
	[{{ code }}] = &opcode_{{ name }},
{%- endfor %}
};

char *opcode_names[256] = {
{%- for code, name in opcodes %}
	[{{ code }}] = "{{ name }}",
{%- endfor %}
};
