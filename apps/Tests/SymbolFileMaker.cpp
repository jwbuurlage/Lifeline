#include <fstream>
#include <iostream>
extern "C"{
#include "../../lib/include/LDatabase.h"
#include "../../lib/include/LMatch.h"
}

using namespace std;

int main(){
	cout << "This program will create a symbol file for A-Z with all feature data set to zero.\n";
	cout << "It will output to emptysymbols.data\n";
	cout << "NOTE: This program does not do any data-alignment of any kind yet. We might want to implement this later (4-byte align).\n";

	ofstream file("emptysymbols.data", std::ofstream::binary);
	if( file.is_open() == false ){
		cerr << "ERROR: unable to create file.\n";
		return 1;
	}

	//For each symbol we have:
	// blocksize: 4 bytes
	// featurecount: 4 bytes
	// symbol: 2 bytes (1 byte for the count and 1 for the symbol itself)
	//   For each feature we have:
	//     feature size: 4 bytes
	//     feature type: 1 byte

	int HeaderSize = 12; //magic + headersize + blockcount
	int SymbolCount = 26;	
	int BlockSize = 10 + (sizeof(LFeatureGeometric)+5) + (sizeof(LFeatureZernike)+5); //block header plus two features
	int TotalBufferSize = HeaderSize + SymbolCount*BlockSize;

	char* buffer = new char[TotalBufferSize];

	char* pointer = buffer;
	*(int*)pointer = 0x4546494c; pointer += 4;
	*(int*)pointer = HeaderSize; pointer += 4;
	*(int*)pointer = SymbolCount; pointer += 4;

	for(int i = 0; i < SymbolCount; ++i){
		*(int*)pointer = BlockSize; pointer += 4;
		*(int*)pointer = 2; pointer += 4; //feature count
		*(char*)pointer = 1; pointer += 1; //string length
		*(char*)pointer = 'A' + i; pointer += 1; //the character (string)
		//Geometric
		*(int*)pointer = sizeof(LFeatureGeometric)+5; pointer += 4;
		*(char*)pointer = (char)FeatureTypeGeometric; pointer += 1;
		memset(pointer, 0, sizeof(LFeatureGeometric)); pointer += sizeof(LFeatureGeometric);
		//Zernike
		*(int*)pointer = sizeof(LFeatureZernike)+5; pointer += 4;
		*(char*)pointer = (char)FeatureTypeZernike; pointer += 1;
		memset(pointer, 0, sizeof(LFeatureZernike)); pointer += sizeof(LFeatureZernike);
	}

	file.write(buffer, TotalBufferSize);
	file.close();

	delete[] buffer;

	cout << "Done.\n";
}
