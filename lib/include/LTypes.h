// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include <vector>

namespace Lifeline
{
  typedef struct
  {
    float x;
    float y;
    float t;
  } Touch;

  typedef std::vector<Touch> PointData;

  typedef stuct
  {
    float score;
    char* character;
  } Result;

  typedef std::vector<Result> ResultSet;
}
