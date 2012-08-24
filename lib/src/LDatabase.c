#include "../include/LDatabase.h"

//
// The application is supposed to load the files in memory and then pass a pointer to this memory
// On failure, this function returns zero and the memory should be freed by the caller
// On success, this function returns non-zero and the memory block will be used by the library and may
//   not be freed untill database_freePointer is called
//
int database_addPointer(void* fileData){
	return 1;
}

void database_freePointer(void* fileData){
	return;
}

void database_doScores(scoreCallback scoreFunc, struct LRecognizer* recog){
	//while( symbols ){
	//	symbol.score = scoreFunc(recog, symbol.features);
	//}
}

