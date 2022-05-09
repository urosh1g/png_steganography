#ifndef PNG_TYPES_H
#define PNG_TYPES_H

#include <stdio.h>
#include <stdint.h>

const uint8_t png_sig[] = {
	0x89, 0x50, 0x4e, 0x47,
	0x0d, 0x0a, 0x1a, 0x0a
};

typedef struct pngc {
	uint32_t length;
	uint8_t type[5];
	uint8_t* data;
	uint8_t crc[5];
	struct pngc* next;
}png_chunk_t;

typedef struct {
	FILE* file;
	png_chunk_t* chunks;
	void (*read_chunks)(FILE** file, png_chunk_t* chunks);
}png_file_reader;

typedef struct {
	FILE* file;
	png_chunk_t* chunks;
	void (*write_chunks)(FILE** file, png_chunk_t* chunks);
}png_file_writer;

#endif
