#include "../include/LImageProcessor.h"
#include "../include/LFeatures.h"
#include <math.h>
#include <cstring> //for memset

#define NEW_STROKE_OFFSET_T 0.1

namespace Lifeline
{
  ImageProcessor::ImageProcessor(int _dimension)
  {
    dimension = _dimension;
    grid = new GridPoint[dimension*dimension + 1];
    features = new Features;
  }

  ImageProcessor::~ImageProcessor()
  {
    delete[] grid;
    delete features;
  }

  int ImageProcessor::processData(const PointData &inputData, FeatureVector &output)
  {
    PointData data(inputData); //copy the input data because we normalize it

    memset(grid, 0, sizeof(GridPoint)*(dimension*dimension+1));

    normalize(&data);
    connect(&data);
    createImage(&data);
    thinImage();

//  features->getMoments(grid, dimension, output);

    return 1;
  }

  void ImageProcessor::normalize(PointData* data)
  {
    //Find the center
    float x_min = 10000.0f, x_max = 0.0f, y_min = x_min, y_max = x_max;
    for(PointData::iterator point = data->begin(); point != data->end(); ++point)
    {
      if (point->x < x_min) x_min = point->x;
      if (point->x > x_max) x_max = point->x;
      if (point->y < y_min) y_min = point->y;
      if (point->y > y_max) y_max = point->y;
    }
    float centerX = 0.5f * (x_min + x_max);
    float centerY = 0.5f * (y_min + y_max);

    //Translate all points and divide by maximum to ensure [-1,1] range
    x_max -= centerX;
    y_max -= centerY;
    float max = (x_max > y_max ? x_max : y_max);
    float invMax = 1.0f/max;
    for(PointData::iterator point = data->begin(); point != data->end(); ++point)
    {
      point->x -= centerX;
      point->x *= invMax;
      point->y -= centerY;
      point->y *= invMax;
    }
 }

  void ImageProcessor::connect(PointData* data)
  {
    // if the distance between two subsequent points is greater
    // than the interval (width of rects in grid) then we need
    // to add points between the two points till it isn't.
    float b = 2.0f / dimension;

    //We will insert points at the end of the vector
    //We only need to iterate over the current contents
    //So we save the amount of points since it changes
    unsigned int count = data->size() - 1; //the last point has no next point to be connected to
    Touch point, next_point, newPoint;
    for(unsigned int i = 0; i < count; ++i)
    {
      point = data->at(i);
      next_point = data->at(i+1);

      if( (next_point.t - point.t) > NEW_STROKE_OFFSET_T ) continue;

      float dx = next_point.x - point.x;
      float dy = next_point.y - point.y;
      float d = sqrt(dx*dx+dy*dy);
      int steps = floorf(d/b) + 1;
      float ival = b/d;

      for(int j = 1; j < steps; ++j)
      {
        float t = j * ival;
        newPoint.x = t * point.x + (1-t) * next_point.x;
        newPoint.y = t * point.y + (1-t) * next_point.y;
        data->push_back(newPoint);
      }
    }
  }

  void ImageProcessor::createImage(PointData* data)
  {
    for(PointData::iterator point = data->begin(); point != data->end(); ++point)
    {
      //Calculate the place on the grid for this point
      int j = (int)( 0.5f*(point->x + 1) * (float)dimension );
      int i = (int)( 0.5f*(point->y + 1) * (float)dimension );
      if( i == dimension ) --i; //When y is exactly 1.0f
      if( j == dimension ) --j; //When x is exactly 1.0f
      grid[i*dimension + j].enabled = 1;
    }
  }

  int ImageProcessor::getNeighbour(int i, int j, int neighbour)
  {
    neighbour = neighbour % 8;
    int n = dimension;
    //Check edges
    //If edge return the dummy grid point n*n which is disabled
    if( i == 0 ){
      if( neighbour <= 2 ) return n*n;
    }else if( i == n-1 ){
      if( neighbour >= 4 && neighbour <= 6 ) return n*n;
    }
    if( j == 0 ){
      if( neighbour >= 6 || neighbour == 0 ) return n*n;
    }else if( j == n-1 ){
      if( neighbour >= 2 && neighbour <= 4 ) return n*n;
    }
    switch(neighbour)
    {
      case 0: return (i-1)*n+j-1;
      case 1: return (i-1)*n+j;
      case 2: return (i-1)*n+j+1;
      case 3: return (i)*n+j+1;
      case 4: return (i+1)*n+j+1;
      case 5: return (i+1)*n+j;
      case 6: return (i+1)*n+j-1;
      case 7: return (i)*n+j-1;
      default: return n*n;
    }
  }

  int ImageProcessor::getNeighbourCount(int i, int j)
  {
    int count = 0;
    for(int k = 0; k < 8; ++k) if( grid[getNeighbour(i,j,k)].enabled ) ++count;
    return count;
  }

  void ImageProcessor::thinImage()
  {
    int n = dimension;
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      {
        if(grid[i*n+j].enabled)
        {
          //We want to check if we can remove this pixel
          //We have to check it the surrounding 8 pixels are
          //still connected when we remove this pixel			
          int count = getNeighbourCount(i, j);
          if(count >= 7) //With 7 or more surrounding pixels they will always be connected
          {
            grid[i*n+j].enabled = 0; //So we can remove the center pixel
          }
          else if(count >= 2)
          {
            //Now we want to check if the surrounding pixels are connected
            //We find the first black pixel, then from there search for the
            //first white pixel that might disconnect it
            int firstBlack = -1;
            int state = 0;
            int disconnections = 0;
            for(int k = 0; k < 16; k++)
            {
              if(state == 0) //Searching for black pixel
              {
                if(grid[getNeighbour(i,j,k)].enabled) 
                {
                  if( firstBlack == -1 ) firstBlack = k;
                  state = 1;
                }
              }
              else if(state == 1) //Black pixel was found, keep following untill white
              {
                if(!grid[getNeighbour(i,j,k)].enabled) state = 2;	
              }
              else if(state == 2) //White pixel found. Did this white pixel disconnect the black pixels?
              {
                //Check the pixel after the white pixel
                if(grid[getNeighbour(i,j,k)].enabled)
                {
                  //There was a black pixel after the white pixel
                  //If the white pixel was at a corner, then it
                  //is not disconnected. If the white pixel is in the middle
                  //then it is disconnected
                  if(k%2 == 1)
                    state = 1;
                  else
                  {
                    state = 0;
                    disconnections++;
                  }
                }
                else //A second white pixel. Then yes this was a disconnection
                {
                  state = 0;
                  disconnections++;
                }
              }
              //Check if we already went a full round
              if( k >= 8 ){
                if( (k-8) >= firstBlack ) break;						
              }
            }
            //If the 8 pixels have one disconnection or less
            //Then they are still connected so we can remove this pixel
            if(disconnections <= 1)
            {
              grid[i*n+j].enabled = 0;
            }
          }
        }
      }
    }
    return;
  }
}
