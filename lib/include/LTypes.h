#ifndef LTYPES_H
#define LTYPES_H

namespace Lifeline
{
  typedef struct
  {
    unsigned char enabled; //if it is drawn
    unsigned char type; //0=normal;1=endpoint;2=branch
    //unsigned char componentNum; //when counting connected components, we give each component a number
    //unsigned char diagonalNum; // each diagonal gets a number, we can use this for moments
    //unsigned char curvature; //measure of how big the curvature in a point is
    unsigned char dummy; //used internally in algorithms, has no output meaning
  } GridPoint;

#define MAX_GEOMETRIC_ORDER	5
#define MAX_ZERNIKE_N		10
#define MAX_ZERNIKE_N_M_COMBINATIONS	((MAX_ZERNIKE_N + 1)*(MAX_ZERNIKE_N + 1) - 1)
#define ZERNIKE_INDEX(n,m)	(n*n + m + n - 1)

  typedef struct
  {
    float geometricMoments[MAX_GEOMETRIC_ORDER][MAX_GEOMETRIC_ORDER];
    float geometricDeviations[MAX_GEOMETRIC_ORDER][MAX_GEOMETRIC_ORDER];
  } FeatureGeometric;

  typedef struct
  {
    float zernikeMoments[MAX_ZERNIKE_N_M_COMBINATIONS][2]; //Complex numbers so 2 components
    float zernikeDeviations[MAX_ZERNIKE_N_M_COMBINATIONS][2]; //Complex numbers so 2 components
  } FeatureZernike;

  typedef struct
  {
    float geometricMoments[MAX_GEOMETRIC_ORDER][MAX_GEOMETRIC_ORDER]; //[p-1][q-1]

    //
    // Pseudo zernike moments have two integers, n and m with the condition |m| <= n
    // So m = -n,..,0,..,n
    // We start at n=1 and go up to n = MAX_ZERNIKE_N
    // For each n we have 2n+1 possible values for m. Adding those for all n gives
    // 3 + 5 + 7 + 9 + ... = (n+1)*(n+1) - 1    (you see this when you write it on paper for the first few n) 
    //
    // To calculate the array index for (n,m) use the follwing formula: (n*n - 1 + m+n)
    // zernikeMoments[ZERNIKE_INDEX(n,m)][0] == real part
    // zernikeMoments[ZERNIKE_INDEX(n,m)][1] == imaginary part
    //
    float zernikeMoments[MAX_ZERNIKE_N_M_COMBINATIONS][2]; //Complex numbers so 2 components

  } FeatureVector;

}

#endif
