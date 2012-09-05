// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef LFEATURES_H
#define LFEATURES_H

#include "LTypes.h"

namespace Lifeline
{
  class Features
  {
    public:
      Features();
      ~Features();

      /*!
       * Calculates geometric and zernike moments of the image
       * stored in grid and stores results in output.
       * When called for the first time it will initialize
       * an array with pre-calculated zernike factors.
       */
      void getMoments(GridPoint* grid, int n, FeatureVector& output);
    private:
      //Pre-calculated factors
      float* zernikeFactors;

      void precalculateZernikeFactors();

      void zernikeValues(float rho, float theta, FeatureVector& output);
  };
}

#endif
