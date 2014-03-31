#ifndef DEVICEADAPTER_H_
#define DEVICEADAPTER_H_

#include "Base.h"
#include "CCPlatformConfig.h"
#include "CCConfiguration.h"

#include "cocos2d.h"

namespace cocos3d
{


class C3DDeviceAdapter : public cocos2d::CCObject
{
  
public:
	 C3DDeviceAdapter();
    ~C3DDeviceAdapter();

	static C3DDeviceAdapter* getInstance();
	

	int getCpuCount();

	bool isSupportVAO()
	{
		
		int supportsShareableVAO = cocos2d::Configuration::getInstance()->checkForGLExtension("vertex_array_object");

		// LOG_ERROR_VARG("the gpu support VAO : %d",supportsShareableVAO);

     	 return supportsShareableVAO;

	}
	

  
private:

	static C3DDeviceAdapter* _instance;

   

};
}


#endif
