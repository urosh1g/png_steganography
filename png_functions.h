#include "./png_types.h"

/*
 *	TODO
 *	Variable argument lista za new_chunk(),
 *	sa Param struktorom
 *	typedef struct {
 *		int type;
 *		union {
 *			byte* byte_ptr;
 *			uint length;
 *			...
 *		};
 *	};
 */

png_chunk_t* new_empty_chunk(){
	png_chunk_t* chunk = (png_chunk_t*)calloc(1, sizeof(png_chunk_t)); 
	memset(chunk, 0, sizeof(png_chunk_t));
	return chunk;
}

png_chunk_t* new_data_chunk(byte* data, uint length){
	png_chunk_t* chunk = new_empty_chunk();
	chunk->data = (byte*)malloc(length);
	memcpy(chunk->data, data, length);
	chunk->length = length;
	return chunk;
}

png_chunk_t* new_all_chunk(uint length, byte* type, byte* data, byte* crc){
	png_chunk_t* chunk = new_data_chunk(data, length);
	memcpy(chunk->type, type, sizeof(chunk->type));
	memcpy(chunk->crc, crc, sizeof(chunk->crc));
	return chunk;
}

void free_png_chunk(png_chunk_t* chunk){
	free(chunk->data);
	free(chunk);
	chunk = NULL;
}
