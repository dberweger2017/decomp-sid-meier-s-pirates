"""Reject assembly implementations before awarding source recovery credit.

Run on the matching compiler's preprocessed translation unit, so included files,
forced includes and macro expansions have the same policy as the main source.
GNU declaration labels and register bindings remain available for ABI recovery.
This is a source policy, not a proof of behavioral correctness.
"""
import re

from .util import ToolError


SOURCE_POLICY = {'version': 2, 'validated': True}
TOKEN = re.compile(
    r'(?P<skip>^[ \t]*\#[^\n]*|//[^\n]*|/\*[\s\S]*?\*/)'
    r'|(?P<raw>(?:u8|[uUL])?R"(?P<delimiter>[^ ()\\\t\r\n]{0,16})'
    r'\([\s\S]*?\)(?P=delimiter)")'
    r'|(?P<string>(?:u8|[uUL])?"(?:\\[\s\S]|[^"\\])*")'
    r"|(?P<char>(?:u8|[uUL])?'(?:\\[\s\S]|[^'\\])*')"
    r'|(?P<identifier>[A-Za-z_$][A-Za-z_0-9$]*)|(?P<other>\S)', re.MULTILINE)
SYMBOL = re.compile(r'[A-Za-z_$][A-Za-z_0-9.$]*\Z')
NON_DECLARATORS = {'if', 'for', 'while', 'switch', 'catch', 'else', 'do', 'return', 'goto',
                   '__extension__', '__attribute__', '__attribute', 'sizeof', 'alignof',
                   '__alignof__', 'typeof', '__typeof__', 'decltype', 'noexcept', 'throw'}
LINE_MARKER = re.compile(r'^\#\s+(\d+)\s+"([^"\n]+)"[^\n]*', re.MULTILINE)


def location(source, offset):
    marker = None
    for candidate in LINE_MARKER.finditer(source, 0, offset):
        marker = candidate
    if marker:
        line = int(marker[1]) + source.count('\n', marker.end(), offset) - 1
        return f'{marker[2]}:{line}'
    return f'<source>:{source.count(chr(10), 0, offset) + 1}'


def declaration_label(tokens, index):
    """A label follows a variable name or a function/array declarator."""
    if index == 0:
        return False
    previous = tokens[index - 1]
    if previous.lastgroup == 'identifier':
        return previous[0] not in NON_DECLARATORS
    closing = previous[0]
    if closing not in (')', ']'):
        return False
    opening = '(' if closing == ')' else '['
    depth = 1
    for pos in range(index - 2, -1, -1):
        value = tokens[pos][0]
        if value == closing:
            depth += 1
        elif value == opening:
            depth -= 1
            if depth == 0:
                return (pos > 0 and tokens[pos - 1].lastgroup == 'identifier'
                        and tokens[pos - 1][0] not in NON_DECLARATORS)
    return False


def validate_source(source):
    tokens = [token for token in TOKEN.finditer(source) if token.lastgroup != 'skip']
    for index, token in enumerate(tokens):
        if token.lastgroup != 'identifier':
            continue
        if token[0] in ('asm', '__asm', '__asm__'):
            # Compiler SDK headers contain target-specific inline assembly
            # (for example in libc's memcpy implementation). The recovery
            # policy applies to project source, not its system dependencies.
            source_path = location(source, token.start()).rsplit(':', 1)[0]
            if source_path.startswith(('/sdk/', '/usr/include/',
                                      '/opt/pirates/lib/gcc/', '/opt/pirates/include/')):
                continue
            # Only a plain declaration label containing a symbol/register name
            # is allowed. No instructions, directives, operands or modifiers.
            args = tokens[index + 1:index + 4]
            if (len(args) == 3 and args[0][0] == '(' and args[2][0] == ')'
                    and args[1].lastgroup == 'string' and args[1][0].startswith('"')
                    and (SYMBOL.fullmatch(args[1][0][1:-1])
                         or re.fullmatch(r'[-+]\[[A-Za-z_$][A-Za-z_0-9$]*\s+[A-Za-z_$][A-Za-z_0-9$:]*\]',
                                         args[1][0][1:-1]))
                    and declaration_label(tokens, index)):
                continue
            raise ToolError('Source policy violation at ' + location(source, token.start())
                            + ': assembly statements and original-byte payloads are not recovered source;'
                            + ' only declaration labels and register bindings are allowed')
        if token[0] in ('section', '__section__') and index + 2 < len(tokens):
            # Data arrays must not masquerade as functions by being placed in
            # an instruction section. Ordinary constant/data sections are fine.
            argument = tokens[index + 2]
            if tokens[index + 1][0] == '(' and argument.lastgroup == 'string':
                value = argument[0][1:-1]
                fields = [field.strip() for field in value.split(',')]
                # Escaped/concatenated section names are refused conservatively.
                if ('\\' in value
                        or (tokens[index + 3:index + 4] and tokens[index + 3].lastgroup == 'string')
                        or (len(fields) > 1 and fields[1].startswith('__text'))
                        or any(field in ('pure_instructions', 'some_instructions') for field in fields)):
                    raise ToolError('Source policy violation at ' + location(source, token.start())
                                    + ': explicit executable-section placement is not recovered source')
