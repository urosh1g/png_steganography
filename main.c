#include <unistd.h>
#include <stdbool.h>
#include "./png_functions.h"

void print_usage(){
	printf("USAGE:\n");
	printf("HELP: ./program -h\n");
	printf("WRITING TEXT TO PNG: \n./program -f input.png -w -o output.png -t \"text to be inserted\"\nREADING TEXT FROM PNG: ./program -f input.png\n");
}

int main(int argc, char** argv){
	FILE* input_image, *output_image = NULL;
	png_chunk_t* chunk = new_chunk();
	png_chunk_t* chunk_to_insert = new_chunk();
	uint8_t signature[8];
	int opt;
	bool write = false, read = true, verbose = false;
	char* text = NULL;
	uint8_t text_len, output_len;

	if(argc <= 1) {
		print_usage();
		return -1;
	}

	while((opt = getopt(argc, argv, "whvf:t:o:")) != -1){
		switch(opt){
		case 'f':
			input_image = fopen(optarg, "rb+");
			break;
		case 'v':
			verbose = true;
			break;
		case 'w':
			write = true;
			read = false;
			break;
		case 't':
			text_len = strlen(optarg);
			text = malloc(text_len + 1);
			strncpy(text, optarg, text_len);
			break;
		case 'o':
			output_image = fopen(optarg, "wb+");
			break;
		default:
		case 'h':
			print_usage();
			return -1;
		}
	}

	read_signature(input_image, &signature);
	if(!test_signature(signature)){
		printf("ERROR: Given file is not a PNG!\n");
		return -1;
	}
	png_file_reader* reader = &(png_file_reader){
		.file = input_image,
		.chunks = chunk,
		.read_chunks = &read_chunks,
	};
	reader->read_chunks(reader->file, reader->chunks);
	if(write){
		png_file_writer* writer = &(png_file_writer) {
			.file = output_image,
			.chunks = reader->chunks,
			.write_chunks = &write_chunks
		};

		chunk_to_insert->length = text_len;
		strncpy((char*)(chunk_to_insert->type), "teXt", 4);
		chunk_to_insert->data = (uint8_t*)malloc(text_len + 1);
		strncpy((char*)(chunk_to_insert->data), text, text_len);
		memset(chunk_to_insert->crc, 0, 4);

		chunk_to_insert->next = reader->chunks->next;
		reader->chunks->next = chunk_to_insert;

		writer->write_chunks(writer->file, writer->chunks);
		writer->chunks->next = chunk_to_insert->next;
		free_chunk(chunk_to_insert);
	}
	if(read)
		print_chunks(hidden_chunk);
	fclose(input_image);
	if(output_image) fclose(output_image);
	while(chunk){
		png_chunk_t* remove = chunk;
		chunk = chunk->next;
		free(remove);
	}
	return 0;
}
