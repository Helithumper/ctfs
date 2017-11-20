import os.path

import click
import jinja2

from .opcodes import OPCODES


@click.command()
@click.argument('target', type=click.Path())
def main(target):
    env = jinja2.Environment(loader=jinja2.PackageLoader(__name__))

    with open(target, 'wt') as outfile:
        print(env
              .get_template(os.path.basename(target))
              .render(opcodes=OPCODES),
              file=outfile)
