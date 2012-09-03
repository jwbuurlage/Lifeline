#include "../include/LDatabase.h"
#include "../include/LMatch.h"
#include <stdlib.h> //for malloc
#include <string.h> //for memcpy
#include <stdio.h> //for printf

#define MAX_BUFFERS 16

//Note that a symbol handle is a pointer to a LSymbolBlock

typedef struct LFeatureBlock{
	char* data;
	unsigned int size;
	LFeatureType type;
} LFeatureBlock;

typedef struct LSymbolBlock{
	char* data;
	unsigned int size;
	LFeatureBlock* features;
	unsigned int featureCount;
	float score; //Not part of the database file. Used for comparing
} LSymbolBlock;

//The 256 ascii symbols are also stored in this table
LSymbolBlock* singlesymbols[256];

typedef struct LSymbolBuffer{
	char* buffer;
	unsigned int size;
	LSymbolBlock* blocks;
	unsigned int blockcount;
} LSymbolBuffer;

LSymbolBuffer symbolbuffers[MAX_BUFFERS];
int bufCount = 0;

//When LDatabase has to return a char* it uses this buffer
//instead of doing a malloc+free every time
char stringbuffer[64];


//
// The application is supposed to load the files in memory and then pass a pointer to this memory
// On failure, this function returns zero and the memory should be freed by the caller
// On success, this function returns non-zero and the memory block will be used by the library and may
//   not be freed untill database_free_pointer is called
//   After database_free_pointer is called the caller should free the memory
//
int database_add_pointer(void* fileData, unsigned int size){
	if( bufCount == MAX_BUFFERS ) return 0;
	if( (size < 12) || (*(int*)fileData != 0x4546494c) ){ //magic int (LIFE)
		printf("Not a symbol file.\n");
		return 0;
	}

	char* data = fileData;
	unsigned int headersize = *(unsigned int*)(data+4);
	unsigned int blockcount = *(unsigned int*)(data+8);
	LSymbolBlock* blocks = 0;
	if( blockcount != 0 ) blocks = (LSymbolBlock*)malloc(sizeof(LSymbolBlock) * blockcount);

	symbolbuffers[bufCount].buffer = data;
	symbolbuffers[bufCount].size = size;
	symbolbuffers[bufCount].blockcount = blockcount;
	symbolbuffers[bufCount].blocks = blocks;
	++bufCount;

	data += headersize;
	for(unsigned int i = 0; i < blockcount; ++i){
		unsigned int blocksize = *(unsigned int*)data;
		unsigned int featureCount = *(unsigned int*)(data+4);
		unsigned char stringlen = *(unsigned char*)(data+8);

		char* nextblock = data + blocksize;
		LFeatureBlock* features = 0;
		if( featureCount != 0 ) features = (LFeatureBlock*)malloc(sizeof(LFeatureBlock) * featureCount);

		blocks[i].data = data;
		blocks[i].size = blocksize;
		blocks[i].featureCount = featureCount;
		blocks[i].features = features;

		//If it is a single-byte character
		if( stringlen == 1 ){
			char character = *(char*)(data+9);
			singlesymbols[character] = &blocks[i];
		}

		data += 9+stringlen;

		for(unsigned int j = 0; j < featureCount; ++j){
			features[j].data = data;
			features[j].size = *(unsigned int*)data;
			features[j].type = (LFeatureType)data[4];
			data += features[j].size;
			//A check if the size is correct:
			unsigned int expected = 0;
			switch(features[j].type){
				case FeatureTypeGeometric: expected = sizeof(LFeatureGeometric)+5; break;
				case FeatureTypeZernike: expected = sizeof(LFeatureZernike)+5; break;
				default: break;
			}
			if( expected ){
				if( features[j].size != expected ){
					printf("Warning: symbol has invalid feature block size.\n");
				}
			}else{
				printf("Warning: unkown feature type in file.\n");
			}
		}

		data = nextblock;
	}

	return 1;
}

void database_free_pointer(void* fileData){
	int i;
	for( i = 0; i < bufCount; ++i ){
		if( symbolbuffers[i].buffer == (char*)fileData ) break;
	}
	if( i == bufCount ) return; //was not in list

	//Deallocate buffers
	for(unsigned int j = 0; j < symbolbuffers[i].blockcount; ++j){
		if( symbolbuffers[i].blocks[j].featureCount )
			free(symbolbuffers[i].blocks[j].features);
	}
	free(symbolbuffers[i].blocks);

	//Shift the other entries one position downwards
	--bufCount;
	while(i < bufCount){
		symbolbuffers[i] = symbolbuffers[i+1];
		++i;
	}
	symbolbuffers[bufCount].buffer = 0;
	symbolbuffers[bufCount].size = 0;
	return;
}

void* database_get_symbol_handle(char* symbol){
	//Currently symbols of more than one byte are not supported
	if( symbol[1] != 0 ) return 0;

	return singlesymbols[symbol[0]];

	//Get length
	unsigned char searchlen = 0;
	while( symbol[searchlen] != 0 ) ++searchlen;

	//Search in the list
	for(unsigned int i = 0; i < bufCount; ++i){
		for(unsigned int j = 0; j < symbolbuffers[i].blockcount; ++j){
			char* data = symbolbuffers[i].blocks[j].data;
			unsigned char stringlen = *(unsigned char*)(data + 8);
			//Note: since the string is not 0-terminated we do not use a standard strcmp here
			if( stringlen != searchlen ) continue;
			unsigned char k;
			for(k = 0; k < stringlen; ++k){
				if( data[9+k] != symbol[k] ) break;
			}
			if( k == stringlen ){
				return (void*)&(symbolbuffers[i].blocks[j]);
			}
		}
	}
	return 0;
}

void* database_get_symbol_feature(void* handle, LFeatureType featureType){
	if( handle == 0 ) return 0;
	LSymbolBlock* symbol = (LSymbolBlock*)handle;
	for(unsigned int i = 0; i < symbol->featureCount; ++i){
		if( symbol->features[i].type == featureType ){
			return symbol->features[i].data + 5;
		}
	}
	return 0;
}

int database_update_symbol_feature(void* handle, LFeatureType featureType, void* buffer){
	if( handle == 0 ) return 0;
	LSymbolBlock* symbol = (LSymbolBlock*)handle;
	for(unsigned int i = 0; i < symbol->featureCount; ++i){
		if( symbol->features[i].type == featureType ){
			memcpy(symbol->features[i].data + 5, buffer, symbol->features[i].size - 5 );
			return 1;
		}
	}
	return 0;
}

char* database_do_scores(scoreCallback scoreFunc, void* recog){
	float lowest = 9999999.0f;
	LSymbolBlock* best_symbol = 0;
	for(unsigned int i = 0; i < bufCount; ++i){
		for(unsigned int j = 0; j < symbolbuffers[i].blockcount; ++j){
			LSymbolBlock* symbol = &symbolbuffers[i].blocks[j];
			symbol->score = scoreFunc(recog, (void*)symbol);
			
			if( symbol->score <= 0.0f ) continue;

			printf("DEBUG: Score for '%c' is %f\n",
					symbol->data[9],
					symbol->score);
			if( symbol->score <= lowest ){
				best_symbol = symbol;
				lowest = symbol->score;
			}
		}
	}
	if( best_symbol ){
		char* data = best_symbol->data;
		unsigned char stringlen = *(unsigned char*)(data + 8);
		memcpy(stringbuffer, data+9, stringlen);
		stringbuffer[stringlen] = 0;
		return stringbuffer;
	}
	return 0;
}
