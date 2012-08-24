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


// The following functions are used by the library internally

//Comparing scores:
//  resetScores();
//  int handle = getFirst();
//  while( handle ){
//  	void* data = getFeatures(handle, ZERNIKE);
//  	//score against data
//  	void* data2 = getFeatures(handle, GEOMETRIC);
//  	//score against more data
//  	setScore(handle, score);
//  	handle = getNext(handle);
//  }

void database_resetScores();
int database_getFirst();
void* database_getFeatures(int handle, int type);
void database_setScore(int handle, float score);
int database_getNext(int handle);
