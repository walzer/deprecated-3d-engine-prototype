#ifndef C3DRANDOM_H
#define C3DRANDOM_H

#include <cstdlib>

namespace cocos3d
{
#define    RAND_LIMIT    32767

/// Random number in range [-1,1]
float RandomFloat();

/// Random floating point number in range [lo, hi]
float RandomFloat(float lo, float hi);
}	//namespace cocos3d
#endif	//#ifndef C3DRANDOM_H
