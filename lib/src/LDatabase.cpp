#include "../include/LDatabase.h"
#include "../include/LTypes.h"
#include <vector>
#include <iostream>

namespace Lifeline
{
  typedef struct FeatureBlock
  {
    char* data; //points in file to start of feature block NOT to actual feature data
    unsigned int size; //size of data block
    FeatureType type;
  } FeatureBlock;

  //Note that a symbol handle is a pointer to a SymbolBlock
  typedef struct SymbolBlock
  {
    char* data; //points somewhere in file
    unsigned int size; //size of data block
    char* fileBuffer; //the file that this symbol was from. Used when the file is deleted so that this symbol can be deleted
    std::vector<FeatureBlock> features;
  } SymbolBlock;

  class DatabaseContainer
  {
    public:
      std::vector<SymbolBlock> symbols;
  };

  Database::Database()
  {
    base = new DatabaseContainer;
  }

  Database::~Database()
  {
    delete base;
  }

  int Database::addPointer(char* fileData, unsigned int size)
  {
    //Check if it is a symbol file
    if( (size < 12) || (*(int*)fileData != 0x4546494c) )
    {
      std::cout << "ERROR: File is not a symbol database file.\n";
      return 0;
    }

    char* data = fileData;
    unsigned int headersize = *(unsigned int*)(data+4);
    unsigned int blockcount = *(unsigned int*)(data+8);
    data += headersize;

    for(unsigned int i = 0; i < blockcount; ++i)
    {
      unsigned int blocksize = *(unsigned int*)data;
      unsigned int featureCount = *(unsigned int*)(data+4);
      unsigned char stringlen = *(unsigned char*)(data+8);
      char* nextblock = data + blocksize;

      base->symbols.push_back(SymbolBlock());
      SymbolBlock* block = &base->symbols.back();

      block->data = data;
      block->size = blocksize;
      block->fileBuffer = fileData;

      data += 9+stringlen;

      for(unsigned int j = 0; j < featureCount; ++j){
        block->features.push_back(FeatureBlock());
        FeatureBlock* feature = &block->features.back();

        feature->data = data;
        feature->size = *(unsigned int*)data;
        feature->type = (FeatureType)data[4];
        data += feature->size;

        //A check if the size is correct:
        unsigned int expected = 0;
        switch(feature->type){
          case FeatureTypeGeometric: expected = sizeof(FeatureGeometric)+5; break;
          case FeatureTypeZernike: expected = sizeof(FeatureZernike)+5; break;
          default: break;
        }
        if( expected ){
          if( feature->size != expected ){
            std::cout << "Warning: symbol has invalid feature block size.\n";
          }
        }else{
          std::cout << "Warning: unkown feature type in file.\n";
        }
      }

      data = nextblock;
    }
    return 1;
	}

  void Database::freePointer(char* fileData)
  {
    //Find all symbols that were in this file
    //and delete them since their file pointers will be invalid
    for(std::vector<SymbolBlock>::iterator symbol = base->symbols.begin(); symbol != base->symbols.end();)
    {
      if( symbol->fileBuffer == fileData )
        symbol = base->symbols.erase(symbol);
      else
        ++symbol;
    }
    return;
  }

  void* Database::getSymbolHandle(char* symbol)
  {
    //Get length
    unsigned char searchlen = 0;
    while( symbol[searchlen] != 0 ) ++searchlen;

    //Search in the list
    for(std::vector<SymbolBlock>::iterator symboliter = base->symbols.begin(); symboliter != base->symbols.end(); ++symboliter)
    {
      char* data = symboliter->data;
      unsigned char stringlen = *(unsigned char*)(data + 8);
      //Note: since the string is not 0-terminated we do not use a standard strcmp here
      if( stringlen != searchlen ) continue;
      unsigned char k;
      for(k = 0; k < stringlen; ++k){
        if( data[9+k] != symbol[k] ) break;
      }
      if( k == stringlen ){
        return &(*symboliter);
      }
    }
    return 0;
  }

  void* Database::getSymbolFeature(void* handle, FeatureType featuretype)
  {
    if( handle == 0 ) return 0;
    SymbolBlock* symbol = (SymbolBlock*)handle;
    for(std::vector<FeatureBlock>::iterator feature = symbol->features.begin(); feature != symbol->features.end(); ++feature)
    {
      if( feature->type == featuretype )
        return feature->data + 5;
    }
    return 0;
  }

  unsigned int Database::getSymbolCount() const
  {
    return base->symbols.size();
  }

  void* Database::getSymbolHandle(unsigned int index)
  {
    if( index >= getSymbolCount() ) return 0;
    return &(base->symbols[index]);
  }
}

