//
// File format
//
// ! Unless stated otherwise, all strings have the following format:
//  a byte with the string length, followed by the string, NOT zero-terminated
// 
// The number in front of each line is the size of the data in bytes
//
// File header:
//
// 04 magic int - 0x4546494c this spells L I F E
// 04 HEADERSIZE - header lenght including magic integer, so it is a file offset to the data
// 04 BLOCKCOUNT - amount of symbol blocks in the data section
// xx NAME - string with the display name of this symbol set
// 
// Data: starting at HEADERSIZE, offsets are relative
// Each symbolblock can contain multiple feature blocks
//
// 04 BLOCKSIZE - size of this symbol block, including this integer
// 04 FEATURECOUNT - amount of feature blocks
// xx SYMBOL - string that contains the symbol which can be a character but also a latex symbol
//    04 SIZE - size of this feature block, including this integer
//    01 LFeatureType - geometric, zernike, etc
//    xx Feature data - depends on FEATURETYPE
//    repeat for each feature set
//

typedef enum
{
	FeatureTypeUnkown = 0,
	FeatureTypeGeometric,
	FeatureTypeZernike,
	FeatureTypeComponents, //amount of components, endpoints, etc
	FeatureTypeMAX
} LFeatureType;


//
// The application is supposed to load the files in memory and then pass a pointer to this memory
// On failure, this function returns zero and the memory should be freed by the caller
// On success, this function returns non-zero and the memory block will be used by the library and may
//   not be freed untill database_freePointer is called
//
int database_add_pointer(void* fileData, unsigned int size);
void database_free_pointer(void* fileData);


// The following is used by the library internally


//The buffer should be big enough to hold featureType
void* database_get_symbol_handle(char* symbol);
void* database_add_symbol(char* symbol);

//returns a (read-only) pointer to the symbol feature
void* database_get_symbol_feature(void* symbol, LFeatureType featureType);

//callbackData is used for LRecognizer*
typedef float (*scoreCallback)(void* callbackData, void* symbolhandle);

void database_do_scores(scoreCallback scoreFunc, void* callbackData);

