#include "../include/LDatabase.h"

#define MAX_BUFFERS 16

void* symbolbuffers[MAX_BUFFERS];
int bufCount = 0;

//
// The application is supposed to load the files in memory and then pass a pointer to this memory
// On failure, this function returns zero and the memory should be freed by the caller
// On success, this function returns non-zero and the memory block will be used by the library and may
//   not be freed untill database_free_pointer is called
//   After database_free_pointer is called the caller should free the memory
//
int database_add_pointer(void* fileData){
	if( bufCount == MAX_BUFFERS ) return 0;
	symbolbuffers[bufCount] = fileData;
	++bufCount;
	return 1;
}

void database_free_pointer(void* fileData){
	int i;
	for( i = 0; i < bufCount; ++i ){
		if( symbolbuffers[i] == fileData ) break;
	}
	if( i == bufCount ) return; //was not in list

	//Shift the other entries one position downwards
	--bufCount;
	while(i < bufCount){
		symbolbuffers[i] = symbolbuffers[i+1];
		++i;
	}
	symbolbuffers[bufCount] = 0;
	return;
}

void* database_get_symbol_handle(char* symbol){
	return 0;
}

void* database_get_symbol_feature(void* handle, LFeatureType featureType){
	return 0;
}

int database_set_symbol_feature(void* symbol, LFeatureType featureType, void* buffer){
	return 0;
}

void database_do_scores(scoreCallback scoreFunc, void* recog){
	//while( symbols ){
	//	symbol.score = scoreFunc(recog, symbol.features);
	//}
}

