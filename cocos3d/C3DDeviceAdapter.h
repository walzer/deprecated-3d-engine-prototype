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
	enum DeviceLevel
    {
		Unknow = 0,
        Low, //one cpu
		Mid, //two cpu
        High, // four cpu
    };
public:
    
	 C3DDeviceAdapter()
	 {
		 _cpuCount = 1;
		 _deviceLevel = DeviceLevel::Low;
		 _supportVAO = false;

		 checkDevice();
		 checkVAO();
	 }

    ~C3DDeviceAdapter()
    {
 
        _deviceAdapterInstance = NULL;
    }
    

	static C3DDeviceAdapter* getInstance();
	
    void checkCpuInfo();

	void checkVAO()
	{
		_supportVAO = cocos2d::Configuration::getInstance()->checkForGLExtension("vertex_array_object");

		WARN_VARG("the gpu support VAO : %d",_supportVAO);
	}

	void checkDevice()
	{
		checkCpuInfo();
		
		if(_cpuCount == 1)
			_deviceLevel = DeviceLevel::Low;
		else if(_cpuCount == 2)
			_deviceLevel = DeviceLevel::Mid;
		else if(_cpuCount >= 4)
			_deviceLevel = DeviceLevel::High;
		else
			_deviceLevel = DeviceLevel::Unknow;

		WARN_VARG("the devicelevel is : %d",(int)_deviceLevel);
	}

	unsigned int getCpuCount()
	{
		return _cpuCount;
	}

	DeviceLevel getDeviceLevel()
	{
		return _deviceLevel;
	}

	bool isSupportVAO()
	{
	
    //	return _supportVAO;

		if(_deviceLevel == DeviceLevel::High)
			return _supportVAO;
		else
			return false;
	}

	bool isSupportShadow()
	{
		if(_deviceLevel == DeviceLevel::High)
			return true;
		else
			return false;
	}

	bool isSuppertPostProcess()
	{
		if(_deviceLevel == DeviceLevel::High)
			return  true;
		else
			return false;
	}
	

private:

	static C3DDeviceAdapter* _deviceAdapterInstance;
	unsigned int _cpuCount;
	DeviceLevel _deviceLevel;
	bool _supportVAO;

};
}


#endif
