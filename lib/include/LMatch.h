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

/* The different charsets to test against */

typedef enum
{
    CharacterSetSlashes,
    CharacterSetNumeric,
    CharacterSetAlphabet,
    CharacterSetAlphanumeric
} LCharacterSet;

/* An image is n x n-grid with values 0, 1 */

typedef struct
{
    int size;      // n
    unsigned int* grid;    // n x n
} LImage;

typedef struct
{
    List images;
} LImageSet;

/* Pixel structs used by the Image */

typedef struct
{
    float x;
    float y;
} LPoint;

typedef struct
{
    float x_min;
    float x_max;
    float y_min;
    float y_max;
} LRect;

typedef struct
{
    List points;
} LPointData;

/* Structs that define the matches */

typedef struct
{
    char character;
    LImage* image;
} LCharacterImage;

typedef struct 
{
    float score;
    char character;
} LMatchData;

typedef struct 
{
    List matchData;
} LResultSet;


#define MAX_GEOMETRIC_ORDER	5
#define MAX_ZERNIKE_N		5

typedef struct
{
	float geometricMoments[MAX_GEOMETRIC_ORDER][MAX_GEOMETRIC_ORDER]; //[p-1][q-1]

	//
	// Pseudo zernike moments have two integers, n and m with the condition |m| <= n
	// So m = -n,..,0,..,n
	// Since we can not have negative array indices, zernikeMoments[n][0] corresponds to m=-n
	// and zernikeMoments[n][1] to m=-n+1, and m=n corresponds with zernikeMoments[n][2n]
	//
	float zernikeMoments[MAX_ZERNIKE_N][2*MAX_ZERNIKE_N+1][2]; //Complex numbers so 2 components

} LFeatureSet;

///////////////////////////////////////////////////////////////////////////////

static inline LPoint* LPointMake(float x, float y)
{
    LPoint *point = malloc(sizeof(LPoint));
    point->x = x;
    point->y = y;
    return point;
}

static inline LRect* LRectMake(float x_min, float x_max, 
                               float y_min, float y_max)
{
    LRect *rect = malloc(sizeof(LRect));
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

///////////////////////////////////////////////////////////////////////////////

LImage* image_contour(LImage *image);

LImage* image_thin(LImage *image);

///////////////////////////////////////////////////////////////////////////////

#endif