import struct

import click
import pkg_resources
import tatsu

from .opcodes import OPCODES, ADDRESSING_MODES


GRAMMAR = pkg_resources.resource_string(__name__, 'asm.ebnf').decode('utf-8')

OPCODE_FORMAT = '<BBH'
DATA_FORMAT = '<L'

OPCODE_LOOKUP = {mnemonic: code for code, mnemonic in OPCODES}
ADDR_MODE_LOOKUP = {mnemonic: code for code, mnemonic in ADDRESSING_MODES}


class AssemblySemantics(object):
    def data(self, ast):
        return int(ast, 16) & 0xffffffff

    def hex_literal(self, ast):
        return int(ast, 16) & 0xffff

    def signed_hex_literal(self, ast):
        return int(ast, 16) & 0xffff

    def opcode(self, ast):
        return OPCODE_LOOKUP[ast.lower()]


@click.command()
@click.option('--debug/--no-debug', default=False)
@click.option('--output', '-o', type=click.File('wb'), default='out.h3i')
@click.argument('source', type=click.File('rt'))
def main(debug, output, source):
    parser = tatsu.compile(GRAMMAR, semantics=AssemblySemantics())
    instructions = []
    labels = {}
    addr = 1

    for line in source:
        line = line.strip()
        if line:
            ast = parser.parse(line)

            if debug:
                click.echo('{!r}'.format(ast))

            instructions.append(ast)
            if ast['label']:
                labels[ast['label']] = addr
            addr += 1

    output.write(struct.pack(DATA_FORMAT, 0))

    for inst in instructions:
        if inst['data'] is not None:
            output.write(struct.pack(DATA_FORMAT, inst['data']))
        else:
            address = 0
            addr_mode = 0

            if inst['address'] is not None:
                addr_mode = ADDR_MODE_LOOKUP[inst['address']['mode']]
                if inst['address']['value']['offset'] is not None:
                    address = inst['address']['value']['offset']
                else:
                    address = labels[inst['address']['value']['label']]

            output.write(struct.pack(OPCODE_FORMAT,
                                     inst['opcode'], addr_mode, address))
