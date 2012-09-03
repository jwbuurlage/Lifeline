// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

namespace Lifeline
{
  class Config
  {
  public:
    static Config* shared
    {
      static Config instance;
      return &instance;
    }

    float getFloatForKey(std::string key);
    int setFloatForKey(float value, std::string key);

    float getIntForKey(std::string key);
    int setIntForKey(int value, std::string key);

  protected:
    Config();
    ~Config();

  private:
    std::map<std::string, float> floatMap;
    std::map<std::string, int> intMap;
  };
}

#endif
