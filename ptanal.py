#!/usr/bin/env python3

import sys
import pprint as pp
import binascii
from ctypes import *
from construct import *

import z85

DUMP_TAG = 'DUMP: '
DUMP_NAME_SIZE = 32

DumpRecord = Struct(
    'addr'	/ Hex(Int64ub),
    'len' 	/ Hex(Int32ub),
    'name'	/ PaddedString(DUMP_NAME_SIZE, 'utf8'),
    'buf'	/ Bytes(this.len),
)

def get_z85_dump(z85_file):
	dump = {}
	for line in z85_file:
		if DUMP_TAG not in line:
			continue
		line = line.strip()
		z85_buf = line.split(DUMP_TAG)[1]
		buf = z85.decode(z85_buf)
		rec = DumpRecord.parse(buf)
		print(rec)

	return None

def main(dump_text_fname):
	with open(dump_text_fname) as z85_file:
		print(get_z85_dump(z85_file))
	return 0

if __name__ == '__main__':
	sys.exit(main(sys.argv[1]))
