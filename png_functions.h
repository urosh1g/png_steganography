#ifndef PNG_FUNC_H
#define PNG_FUNC_H

#include <stdlib.h>
#include <string.h>
#include "./png_types.h"

png_chunk_t* new_chunk();
void free_chunk(png_chunk_t* chunk);
void print_chunk(png_chunk_t* chunk);
void read_signature(FILE* f, void* buffer);
void read_chunk(FILE* f, png_chunk_t* chunk);
void read_chunks(FILE* f, png_chunk_t* chunks);
void write_chunk(FILE* f, png_chunk_t* chunk);
void write_chunks(FILE* f, png_chunk_t* chunks);
void print_chunks(png_chunk_t* chunks);
void swap_endianness(uint32_t* x);
bool test_signature(uint8_t* sig);

png_chunk_t* new_chunk() {
	png_chunk_t* chunk = (png_chunk_t*)malloc(sizeof(png_chunk_t));
	memset(chunk, 0, sizeof(png_chunk_t));
	return chunk;
}

void free_chunk(png_chunk_t* chunk){
	if(chunk->data != NULL)
		free(chunk->data);
	free(chunk);
	chunk = NULL;
}

void print_chunk(png_chunk_t* chunk){
	printf("Data length (bytes): %u\n", chunk->length);
	printf("Chunk type: %s\n", chunk->type);
	printf("Chunk data: ");
	for(size_t i = 0; i < chunk->length; i++)
		printf("0x%x ", chunk->data[i]);
	printf("\nChunk CRC: ");
	for(size_t i = 0; i < 4; i++)
		printf("0x%x ", chunk->crc[i]);
	printf("\n");
}

void read_signature(FILE* f, void* buffer){
	fread(buffer, byte, 8, f);
}

void swap_endianness(uint32_t* x){
	uint32_t left   = (*x & 0xff000000) >> 24;
	uint32_t right  = (*x & 0x000000ff);
	uint32_t mleft  = (*x & 0x00ff0000) >> 16;
	uint32_t mright = (*x & 0x0000ff00) >> 8;
	*x = (right << 24) | (mright << 16) | (mleft << 8) | left;
}

void read_chunk(FILE* f, png_chunk_t* chunk){
	fread(&chunk->length, byte, 4, f);
	swap_endianness(&chunk->length);
	fread(chunk->type, byte, 4, f);
	chunk->data = (uint8_t*)malloc(chunk->length);
	fread(chunk->data, byte, chunk->length, f);
	fread(chunk->crc, byte, 4, f);
}

void read_chunks(FILE* f, png_chunk_t* chunks){
	png_chunk_t* chunk = chunks;
	while(true){
		read_chunk(f, chunk);
		if(strcmp((char*)(chunk->type), "IEND") == 0) break;
		if(strcmp((char*)(chunk->type), "teXt") == 0) {
			printf("Found hidden data\n");
		}
		chunk->next = new_chunk();
		chunk = chunk->next;
	}
}

void write_chunk(FILE* f, png_chunk_t* chunk){
	uint32_t sz = chunk->length;
	swap_endianness(&chunk->length);
	fwrite(&chunk->length, byte, 4, f);
	fwrite(chunk->type, byte, 4, f);
	fwrite(chunk->data, byte, sz, f);
	fwrite(chunk->crc, byte, 4, f);
}

void write_chunks(FILE* f, png_chunk_t* chunks){
	png_chunk_t* chunk = chunks;
	fwrite(png_sig, byte, 8, f);
	while(chunk){
		write_chunk(f, chunk);
		chunk = chunk->next;
	}
}

void print_chunks(png_chunk_t* chunks){
	png_chunk_t* chunk = chunks;
	while(chunk){
		print_chunk(chunk);
		chunk = chunk->next;
	}
}
bool test_signature(uint8_t* sig){
	for(size_t i = 0; i < 8; i++)
		if(sig[i] != png_sig[i])
			return false;
	return true;
}

#endif
