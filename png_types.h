#ifndef PNG_TYPES_H
#define PNG_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef char byte;
typedef unsigned int uint;

typedef struct {
	uint length;
	byte type[4];
	byte* data;
	byte crc[4];
}png_chunk_t;

png_chunk_t* new_empty_chunk();
png_chunk_t* new_data_chunk(byte* data, uint length);
png_chunk_t* new_all_chunk(uint length, byte* type, byte* data, byte* crc);
void free_chunk(png_chunk_t* chunk);

#endif
