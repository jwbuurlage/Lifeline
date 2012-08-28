// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

/* Defines structures and types used for matching */
 
#ifndef LMATCH_H
#define LMATCH_H

#include "DataStructures.h"
#include <math.h>

///////////////////////////////////////////////////////////////////////////////

typedef enum
{
    CharacterSetSlashes,
    CharacterSetNumeric,
    CharacterSetAlphabet,
    CharacterSetAlphanumeric
} LCharacterSet;

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned char enabled; //if it is drawn
	unsigned char type; //0=normal;1=endpoint;2=branch
	unsigned char componentNum; //when counting connected components, we give each component a number
	unsigned char diagonalNum; // each diagonal gets a number, we can use this for moments
	unsigned char curvature; //measure of how big the curvature in a point is
	unsigned char dummy; //used internally in algorithms, has no output meaning
} LGridPoint;

typedef struct
{
    int size;      // n
    LGridPoint* grid;    // n x n
} LImage;

typedef struct
{
    List images;
} LImageSet;

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    float x;
    float y;
    float t;
} LPoint;

typedef struct
{
    float x_min;
    float x_max;
    float y_min;
    float y_max;
} LRect;

typedef List LPointData;

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    char character;
    LImage* image;
} LCharacterImage;

typedef struct
{
    char* feature_name;
    float value;
} LCharacterFeature;

typedef List LCharacterFeatures;

typedef struct
{
    char* feature_name;
    float value;
} LFeature;

// typedef List LFeatureSet;

typedef struct 
{
    float score;
    char character;
} LMatchData;

typedef List LResultSet;


#define MAX_GEOMETRIC_ORDER	5
#define MAX_ZERNIKE_N		5
#define MAX_ZERNIKE_N_M_COMBINATIONS	((MAX_ZERNIKE_N + 1)*(MAX_ZERNIKE_N + 1) - 1)
#define ZERNIKE_INDEX(n,m)	(n*n + m + n - 1)

typedef struct
{
	float geometricMoments[MAX_GEOMETRIC_ORDER][MAX_GEOMETRIC_ORDER];
	float geometricDeviations[MAX_GEOMETRIC_ORDER][MAX_GEOMETRIC_ORDER];
} LFeatureGeometric;

typedef struct
{
	float zernikeMoments[MAX_ZERNIKE_N_M_COMBINATIONS][2]; //Complex numbers so 2 components
	float zernikeMomentsDeviations[MAX_ZERNIKE_N_M_COMBINATIONS][2]; //Complex numbers so 2 components
} LFeatureZernike;

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

} LFeatureSet;

///////////////////////////////////////////////////////////////////////////////

static inline LPoint* LPointMake(float x, float y, float t)
{
    LPoint *point = (LPoint*)malloc(sizeof(LPoint));
    point->x = x;
    point->y = y;
    point->t = t;
    return point;
}

static inline LRect* LRectMake(float x_min, float x_max, 
                               float y_min, float y_max)
{
    LRect *rect = (LRect*)malloc(sizeof(LRect));
    rect->x_min = x_min;
    rect->x_max = x_max;
    rect->y_min = y_min;
    rect->y_max = y_max;
    return rect;
}

static inline void LRectSet(LRect* rect, float x_min, float x_max, 
                            float y_min, float y_max)
{
    rect->x_min = x_min;
    rect->x_max = x_max;
    rect->y_min = y_min;
    rect->y_max = y_max;
}

static inline int LPointInRect(LPoint point, LRect rect)
{
    if(point.x >= rect.x_min && point.x <= rect.x_max
       && point.y >= rect.y_min && point.y <= rect.y_max)
        return 1;
    
    return 0;
}

static inline float LPointDistance(LPoint* point, LPoint* next_point)
{
    // Euclidian distance
    float distance = sqrtf(powf(point->x - next_point->x, 2) + 
                           powf(point->y - next_point->y, 2));
    return distance;
}

///////////////////////////////////////////////////////////////////////////////

#endif
