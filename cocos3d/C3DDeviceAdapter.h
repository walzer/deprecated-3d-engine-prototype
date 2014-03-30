#ifndef DEVICEADAPTER_H_
#define DEVICEADAPTER_H_

#include "Base.h"
#include "CCPlatformConfig.h"
#include "CCConfiguration.h"

namespace cocos3d
{


class C3DDeviceAdapter
{
  
public:
	 C3DDeviceAdapter();
    ~C3DDeviceAdapter();

	int getCpuCount();

	bool isSupportVAO()
	{
		
		int supportsShareableVAO = cocos2d::Configuration::getInstance()->checkForGLExtension("vertex_array_object");

		 LOG_ERROR_VARG("the gpu support VAO : %d",supportsShareableVAO);

     	 return supportsShareableVAO;

	}
	

  
private:

   

};
}


#endif
