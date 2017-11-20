import mmap
import struct

import click

from .opcodes import OPCODES, ADDRESSING_MODES


OPCODE_FORMAT = '<BBH'
DATA_FORMAT = '<L'

OPCODE_LOOKUP = dict(OPCODES)
ADDR_MODE_LOOKUP = dict(ADDRESSING_MODES)


@click.command()
@click.argument('image', type=click.File('rb'))
def main(image):
    image_map = mmap.mmap(image.fileno(), 0, access=mmap.ACCESS_READ)

    for i, image_index in enumerate(range(len(image_map) // 4)):
        if i == 0:
            # skip always-zero word
            continue
        ins_bytes = image_map[image_index * 4: (image_index + 1) * 4]
        opcode, addrmode, address = struct.unpack(OPCODE_FORMAT, ins_bytes)
        raw = ''.join(['{:02x}'.format(c) for c in ins_bytes])
        if opcode in OPCODE_LOOKUP and addrmode in ADDR_MODE_LOOKUP:
            opcode_mnem = OPCODE_LOOKUP[opcode]
            addr_mnem = ADDR_MODE_LOOKUP[addrmode]
            if addr_mnem == 'none':
                addr = ''
            elif addr_mnem == 'rel':
                addr = '+{:04x}'.format(address)
            elif addr_mnem == 'abs':
                addr = '${:04x}'.format(address)
            elif addr_mnem == 'indir':
                addr = '[{:04x}]'.format(address)
            elif addr_mnem == 'imm':
                addr = '{:04x}'.format(address)
            cooked = '{} {}'.format(opcode_mnem, addr)
        else:
            cooked = ''

        click.echo('{:04x}: {} {}'.format(i, raw, cooked))


if __name__ == '__main__':
    main()
