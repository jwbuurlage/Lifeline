#include "../include/LFeatures.h"
#include <cstring>
#include <math.h>

#ifndef abs
#define abs(n)  ((n)>0?(n):-(n))
#endif

int factorial(int n){
  switch(n){
    case 0: return 1;
    case 1: return 1;
    case 2: return 2;
    case 3: return 6;
    case 4: return 24;
    case 5: return 120;
    case 6: return 720;
    default: return n*factorial(n-1);
  }
}

namespace Lifeline
{
  Features::Features()
  {
    zernikeFactors = 0;
  }

  Features::~Features()
  {
    if( zernikeFactors ) delete[] zernikeFactors;
  }

  void Features::precalculateZernikeFactors()
  {
    if( zernikeFactors != 0 ) return;

    //First we need to calculate how big the array needs to be
    int floatCount = 0;
    for(int n = 1; n <= MAX_ZERNIKE_N; ++n){
      for(int m = -n; m <= n; ++m){
        floatCount += (n - abs(m)) + 1;
      }
    }
    //Allocate the array
    zernikeFactors = new float[floatCount];
    //Fill the array
    int arrayIndex = 0;
    for(int n = 1; n <= MAX_ZERNIKE_N; ++n){
      for(int m = -n; m <= n; ++m){
        int absM = abs(m);
        int sMax = n - absM;
        for(int s = 0; s <= sMax; ++s){
          zernikeFactors[arrayIndex] = factorial(2*n+1-s)/((float)( factorial(s) * factorial(n-absM-s) * factorial(n+absM+1-s) ));
          arrayIndex++;
        }
      }
    }
  }

  void Features::getMoments(GridPoint* grid, int n, FeatureVector& output)
  {
    //Set the moments to zero
    memset(output.geometricMoments, 0, sizeof(output.geometricMoments));
    memset(output.zernikeMoments, 0, sizeof(output.zernikeMoments));

    float mid = n/2;
    for(int i = 0; i < n; ++i)
    {
      for(int j = 0; j < n; ++j)
      {
        if(grid[i*n+j].enabled)
        {
          //We need to add this pixels coordinate to the sum
          //But the coordinates are first mapped to [-1,1]
          float x = ((float)j-mid)/mid;
          float y = ((float)i-mid)/mid;

          // Geometric moments
          float powx[MAX_GEOMETRIC_ORDER];
          float powy[MAX_GEOMETRIC_ORDER];
          for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p){
            powx[p-1] = pow(x,p);
            powy[p-1] = pow(y,p);
          }
          for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p){
            for(int q = 1; q <= MAX_GEOMETRIC_ORDER; ++q){
              output.geometricMoments[p-1][q-1] += powx[p]*powy[q];
            }
          }

          // Pseudo-Zernike moments
          float rho = sqrt(x*x + y*y);
          float theta = atan2(y,x);
          zernikeValues(rho, theta, output);
        }
      }
    }
    return;
  }

  //outputArray is expected to be of size MAX_ZERNIKE_N_M_COMBINATIONS
  //this function calculates the contribution to the moment for a single pixel
  //for all possible values of n,m
  //This function should be called on all pixels, and it will keep adding to outputArray
  void Features::zernikeValues(float rho, float theta, FeatureVector& output)
  {
    if( zernikeFactors == 0 ) precalculateZernikeFactors();

    //Since we need the first n powers of rho multiple times we save them
    //Same goes for cosine and sine of m*theta
    float rhoPowers[MAX_ZERNIKE_N];
    float cosines[MAX_ZERNIKE_N+1];
    float sines[MAX_ZERNIKE_N+1];
    rhoPowers[0] = rho;
    for(int n = 2; n <= MAX_ZERNIKE_N; ++n){
      rhoPowers[n-1] = pow(rho, n);
    }
    cosines[0] = 1;
    sines[0] = 0;
    for(int m = 1; m <= MAX_ZERNIKE_N; ++m){
      cosines[m] = cos(m*theta);
      sines[m] = sin(m*theta);
    }
    //Now calculate the value of the zernike function for all n,m for this pixel
    int index = 0;
    for(int n = 1; n <= MAX_ZERNIKE_N; ++n){
      for(int m = -n; m <= n; ++m){
        float sum = 0.0f;
        int absM = abs(m);
        int sMax = n - absM;
        for( int s = 0; s <= sMax; ++s ){
          float factor = zernikeFactors[index];
          index++;
          factor *= rhoPowers[n-s-1]; //pow(rho, n-s);
          //Multiply with (-1)^s is the same as: if( s is odd ) apply minus sign
          if( s & 1 ) sum -= factor;
          else sum += factor;
        }
        //Add it to the sum of all the pixels
        output.zernikeMoments[ZERNIKE_INDEX(n,m)][0] += sum*cosines[absM];
        output.zernikeMoments[ZERNIKE_INDEX(n,m)][1] += sum*sines[absM];
      }
    }
  }

}

