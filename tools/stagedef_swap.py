#!/usr/bin/env python3
"""Convert SMB1 .stagedef files from big-endian to little-endian.

This script parses the stagedef format described in smb1_stagedef_format.txt
and outputs a little-endian version of the file.  Only numeric fields are
byte-swapped; ASCII name strings are left untouched.
"""
import struct
import sys
from pathlib import Path

if len(sys.argv) != 3:
    print("Usage: stagedef_swap.py input.stagedef output.stagedef")
    sys.exit(1)

inp_path = Path(sys.argv[1])
out_path = Path(sys.argv[2])

data = inp_path.read_bytes()
orig = data  # immutable for reading big-endian values
out = bytearray(data)

# helper functions
read_u32 = lambda off: struct.unpack_from('>I', orig, off)[0]
read_u16 = lambda off: struct.unpack_from('>H', orig, off)[0]
read_f32 = lambda off: struct.unpack_from('>f', orig, off)[0]

def swap_fmt(off: int, fmt: str):
    vals = struct.unpack_from('>' + fmt, orig, off)
    struct.pack_into('<' + fmt, out, off, *vals)

def swap32_range(start: int, end: int):
    for off in range(start, end, 4):
        swap_fmt(off, 'I')

def swap16_range(start: int, end: int):
    for off in range(start, end, 2):
        swap_fmt(off, 'H')

def find_ascii_start(buf: bytes) -> int:
    idx = len(buf)
    while idx > 0 and (buf[idx-1] == 0 or 32 <= buf[idx-1] < 127):
        idx -= 1
    while idx < len(buf) and buf[idx] == 0:
        idx += 1
    return idx

ascii_start = find_ascii_start(orig)

# 1. Header
HEADER_SIZE = 0xA0
swap32_range(0, HEADER_SIZE)

# 2. Start positions
start_off = read_u32(0x10)
fallout_off = read_u32(0x14)
start_count = (fallout_off - start_off) // 0x14
for i in range(start_count):
    base = start_off + i * 0x14
    swap_fmt(base, 'fffhhhh')
# Fallout Y
swap_fmt(fallout_off, 'f')

# 3. Goals
goal_count = read_u32(0x18)
goal_ptr = read_u32(0x1C)
for i in range(goal_count):
    base = goal_ptr + i * 0x14
    swap_fmt(base, 'fffhhhh')

# 4. Bumpers
bumper_count = read_u32(0x28)
bumper_ptr = read_u32(0x2C)
for i in range(bumper_count):
    base = bumper_ptr + i * 0x20
    swap_fmt(base, 'fffhhhHfff')

# 5. Jamabars
jamabar_count = read_u32(0x30)
jamabar_ptr = read_u32(0x34)
for i in range(jamabar_count):
    base = jamabar_ptr + i * 0x20
    swap_fmt(base, 'fffhhhHfff')

# 6. Bananas
banana_count = read_u32(0x38)
banana_ptr = read_u32(0x3C)
for i in range(banana_count):
    base = banana_ptr + i * 0x10
    swap_fmt(base, 'fffI')

# 7. Unknown list after header
unk_count = read_u32(0x50)
unk_ptr = read_u32(0x54)
level_ptr = read_u32(0x5C)
if unk_ptr and unk_count and level_ptr > unk_ptr:
    size = level_ptr - unk_ptr
    for off in range(unk_ptr, unk_ptr + size, 4):
        swap_fmt(off, 'I')

# 8. Level models
level_count = read_u32(0x58)
for i in range(level_count):
    base = level_ptr + i * 0x0C
    swap_fmt(base, 'III')

# 9. Background models
bg_count = read_u32(0x68)
bg_ptr = read_u32(0x6C)
for i in range(bg_count):
    base = bg_ptr + i * 0x38
    swap_fmt(base, 'IIIfffhhhhfffIII')

# 10. Reflective models
refl_count = read_u32(0x80)
refl_ptr = read_u32(0x84)
for i in range(refl_count):
    base = refl_ptr + i * 0x08
    swap_fmt(base, 'II')

# 11. Collision headers
coll_count = read_u32(0x08)
coll_ptr = read_u32(0x0C)
tri_start = None
grid_ptr = None
cell_x = cell_z = 0
for i in range(coll_count):
    base = coll_ptr + i * 0xC4
    swap_fmt(base, 'fffhhhhIIIIffffII' + 'I'*34)
    if tri_start is None:
        tri_start = read_u32(base + 0x1C)
        grid_ptr = read_u32(base + 0x20)
        cell_x = read_u32(base + 0x34)
        cell_z = read_u32(base + 0x38)

if tri_start and grid_ptr:
    # Determine grid-list start from grid pointers
    ptrs = [read_u32(grid_ptr + i*4) for i in range(cell_x*cell_z)]
    ptrs = [p for p in ptrs if p]
    if ptrs:
        grid_list_start = min(ptrs)
    else:
        grid_list_start = grid_ptr
    tri_count = (grid_list_start - tri_start) // 0x40
    for i in range(tri_count):
        base = tri_start + i * 0x40
        swap_fmt(base, 'ffffffhhhhffffffff')
    # Grid triangle lists (array of uint16 indices)
    swap16_range(grid_list_start, grid_ptr)
    # Grid pointers
    swap32_range(grid_ptr, grid_ptr + cell_x * cell_z * 4)

# 12. Everything between last processed offset and ascii is treated as 32-bit
processed_end = max(grid_ptr + cell_x*cell_z*4 if grid_ptr else 0,
                    refl_ptr + refl_count*0x08 if refl_count else 0,
                    bg_ptr + bg_count*0x38 if bg_count else 0,
                    level_ptr + level_count*0x0C if level_count else 0,
                    banana_ptr + banana_count*0x10 if banana_count else 0,
                    jamabar_ptr + jamabar_count*0x20 if jamabar_count else 0,
                    bumper_ptr + bumper_count*0x20 if bumper_count else 0,
                    goal_ptr + goal_count*0x14 if goal_count else 0,
                    fallout_off + 4)
for off in range(processed_end, ascii_start, 4):
    if off + 4 <= ascii_start:
        swap_fmt(off, 'I')

out_path.write_bytes(out)
print(f"Converted {inp_path} -> {out_path}")
