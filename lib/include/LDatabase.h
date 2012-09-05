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
// 04 VERSION - version number of this file
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

#ifndef LDATABASE_H
#define LDATABASE_H

namespace Lifeline
{
  typedef enum
  {
    FeatureTypeUnkown = 0,
    FeatureTypeGeometric,
    FeatureTypeZernike,
    FeatureTypeComponents, //amount of components, endpoints, etc
    FeatureTypeMAX
  } FeatureType;

  //Because I do not want to include all the std headers
  //and declare the structures that are used internally
  //by the Database I put this in a DatabaseContainer
  //class and use forward-declarations here.
  class DatabaseContainer;

  class Database
  {
    public:
      Database();
      ~Database();

      /*!
       * The application is supposed to load files into memory and then pass the file pointer
       * to Database. On success this function returns non-zero and the memory block will be used
       * by Database and therefore should NOT be freed by the application untill
       * Database::freePointer has been called.
       * On failure this returns zero and the memory can be freed immediately.
       *
       * Note that adding a new symbol file invalidates all symbol handles
       */
      int addPointer(char* fileData, unsigned int size);
      void freePointer(char* fileData);

      /*!
       * getSymbolHandle returns a handle of the symbol (internal pointer)
       * Returns zero when not found.
       * Currently this method is really slow
       * After a handle has been obtained it can be used with
       * getSymbolFeature to obtain feature data. This function
       * return zero when the feature was not found.
       */
      void* getSymbolHandle(char* symbol);
      void* getSymbolFeature(void* handle, FeatureType featuretype);

      /*!
       * The following is used by Classifier to iterate
       * over all symbols. The return value of getSymbolHandle
       * is always valid if index is smaller than getSymbolCount.
       */
      unsigned int getSymbolCount() const;
      void* getSymbolHandle(unsigned int index);
    private:
      DatabaseContainer* base;
  };
}

#endif
