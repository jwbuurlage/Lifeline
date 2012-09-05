// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef LPUBLICTYPES_H
#define LPUBLICTYPES_H

#include <vector>

namespace Lifeline
{
  struct Touch
  {
    float x;
    float y;
    float t;
    Touch(){}
    Touch(float _x, float _y, float _t) : x(_x), y(_y), t(_t) {}
    ~Touch(){}
  };

  typedef std::vector<Touch> PointData;

  typedef struct
  {
    float score;
    char* character;
  } Result;

  typedef std::vector<Result> ResultSet;
}

#endif
