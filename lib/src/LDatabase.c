#include "../include/LDatabase.h"

//
// The application is supposed to load the files in memory and then pass a pointer to this memory
// On failure, this function returns zero and the memory should be freed by the caller
// On success, this function returns non-zero and the memory block will be used by the library and may
//   not be freed untill database_freePointer is called
//
int database_add_pointer(void* fileData){
	return 1;
}

void database_free_pointer(void* fileData){
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

