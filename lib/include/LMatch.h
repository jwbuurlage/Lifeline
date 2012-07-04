// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

// Defines many of the structures and types used extensively in the library for
// - matching characters
// - providing results
// - pre- and postprocessing
 
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
    int size;       // n
    short* grid;    // n x n
} LImage;

/* Pixel structs used by the Image */

typedef struct
{
    float x;
    float y;
} LPoint;

typedef struct
{
    List points;
} LPointData;

/* Structs that define the matches */

typedef struct 
{
    float score;
    char character;
} LMatchData;

typedef struct 
{
    List matchData;
} LResultSet;

#endif