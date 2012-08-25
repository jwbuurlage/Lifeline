//
// File format
//
// Unless stated otherwise, all strings have the following format:
//  a byte with the string length, followed by the string, NOT zero-terminated
// 
// The number in front of each line is the size of the data in bytes
//
// File header:
//
// 04 magic int
// 04 HEADERSIZE - header lenght including magic integer, so it is a file offset to the data
// xx NAME - string with the display name of this symbol set
// 
// Data: starting at HEADERSIZE, offsets are relative, one data block per symbol
//
// 04 BLOCKSIZE - size of this data block
// xx SYMBOL - string that contains the symbol which can be a character but also a latex symbol
//  04 SIZE - size of this feature block
//  01 FEATURETYPE - geometric, zernike, etc
//  xx Feature data - depends on FEATURETYPE
//  repeat for each feature set
//

//
// The application is supposed to load the files in memory and then pass a pointer to this memory
// On failure, this function returns zero and the memory should be freed by the caller
// On success, this function returns non-zero and the memory block will be used by the library and may
//   not be freed untill database_freePointer is called
//
int database_addPointer(void* fileData);
void database_freePointer(void* fileData);


// The following is used by the library internally

struct LRecognizer; //forward declarations
struct LCalibratedFeatureSet;

typedef float (*scoreCallback)(struct LRecognizer* recog, struct LCalibratedFeatureSet* features);

void database_doScores(scoreCallback scoreFunc, struct LRecognizer* recog);

