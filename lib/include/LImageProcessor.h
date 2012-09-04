#ifndef LIMAGEPROCESSOR_H
#define LIMAGEPROCESSOR_H

#include "LTypes.h"
#include "LPublicTypes.h"
#include <vector>

namespace Lifeline
{
  class Features;

  class ImageProcessor
  {
    public:
      ImageProcessor(int dimension);
      ~ImageProcessor();

      /*!
       * Takes touch data and produces a feature vector
       * Does not modify touch data
       * Feature vector contains image moments as well as 
       * the amount of branch points and so on
       */
      int processData(const PointData &inputData, FeatureVector &output);

    private:
      int dimension; //n
      GridPoint* grid; //array of  n x n + 1  gridpoints
      Features* features;

      void normalize(PointData* data);
      void connect(PointData* data);
      void createImage(PointData* data);
      void thinImage();

      int getNeighbour(int i, int j, int neighbour);
      int getNeighbourCount(int i, int j);
  };
}

#endif
