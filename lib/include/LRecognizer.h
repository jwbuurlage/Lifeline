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

#include "LTypes.h"
#include <vector>

namespace Lifeline
{
  class Recognizer
  {
    Recognizer(int _imageSize = 65);
    ~Recognizer();

    /*!
     * Loads a database from file with characters to match against.
     * The client needs to load the data and provide the function with a 
     * pointer.
     */
    void loadSymbolsWithData(char* data);
    
    /*!
     * Sets the point data to use, and processes and prepares it for
     * feature extraction and matching. Returns a ResultSet with the
     * matching characters.
     */
    ResultSet resultsWithPointData(PointData *data);
  };
}

#endif
