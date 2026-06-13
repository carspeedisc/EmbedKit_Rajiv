# EmbedKit_Rajiv
Ring Buffer Assignment - Embed Square Solutions

# EmbedKit_Rajiv

**Ring Buffer Module** - Embedded C Utility Library

## Assignment
Fresher Embedded Developer Assessment for **Embed Square Solutions Pvt. Ltd.**

## Module Implemented
- **Ring Buffer** (`ringbuf.c`)

## Features
- Fixed capacity of 8 bytes (`uint8_t`)
- Safe write (fails when full, no overwrite)
- Safe read (fails when empty)
- `count()`, `is_full()`, `is_empty()`
- **Bonus**: Bitwise wrap-around using `& (BUFFER_SIZE-1)` for better performance on MCUs

## Build Instructions
```bash
gcc -Wall -std=c99 ringbuf.c -o ringbuf
./ringbuf
