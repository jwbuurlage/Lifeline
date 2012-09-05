// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
//
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
//
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef LRECOGNIZER_H 
#define LRECOGNIZER_H

#include "LPublicTypes.h"
#include <vector>

#ifdef LDEBUG
#include "LTypes.h"
#endif

namespace Lifeline
{
  class ImageProcessor;
  class Database;

  class Recognizer
  {
    public:
      Recognizer(int _imageSize = 65);
      ~Recognizer();

      /*!
       * The client should load the symbol file into memory
       * and then pass a pointer to the file buffer.
       * This buffer needs to stay in memory untill freeSymbols
       * has been called as the file buffer is used internally.
       */
      int loadSymbolsFromMemory(char* fileData, int size);
      void freeSymbols(char* fileData);

      /*!
       * Sets the point data to use, and processes and prepares it for
       * feature extraction and matching. Outputs a ResultSet with the
       * matching characters.
       */
      void processPoints(const PointData &data, ResultSet &results);

#ifdef LDEBUG
      int getImageDimension();
      GridPoint* getImageGrid();
#endif

    private:
      ImageProcessor* imageProcessor;
      Database* database;
      int imageSize;
  };
}

#endif
