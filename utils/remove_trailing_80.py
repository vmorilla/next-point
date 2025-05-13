#!/usr/bin/env python3
import sys


def trim_leading_0x80(input_stream, output_stream):
    """
    Reads from input_stream and writes to output_stream,
    removing any leading 0x80 bytes at the beginning.
    """
    # Read the input in binary mode
    content = input_stream.buffer.read()

    # Find index of first non-0x80 byte
    start_index = 0
    while start_index < len(content) and content[start_index] == 0x80:
        start_index += 1

    # Write the trimmed content to output
    output_stream.buffer.write(content[start_index:])
    output_stream.buffer.flush()


if __name__ == "__main__":
    # Process from stdin to stdout
    trim_leading_0x80(sys.stdin, sys.stdout)
