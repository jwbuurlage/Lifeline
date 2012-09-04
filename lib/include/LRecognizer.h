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

  class Recognizer
  {
    public:
      Recognizer(int _imageSize = 65);
      ~Recognizer();

      /*!
       * Loads a database from file with characters to match against.
       * The client needs to load the data and provide the function with a 
       * pointer.
       */
      void loadSymbolsWithData(char* filedata);

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
      int imageSize;
  };
}

#endif
