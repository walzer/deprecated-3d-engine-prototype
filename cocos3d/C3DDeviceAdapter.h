#ifndef DEVICEADAPTER_H_
#define DEVICEADAPTER_H_
#include "CCPlatformConfig.h"


namespace cocos3d
{


class C3DDeviceAdapter
{
  
public:
	 C3DDeviceAdapter();
    ~C3DDeviceAdapter();

	int getCpuCount();

	int isSupportVAO();



  
private:

   

};
}


#endif
