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
         _cpuFrequency = 0;
         _platformName = "unknown";
		 _deviceLevel = DeviceLevel::Low;
		 _supportVAO = false;

		 checkCpuInfo();
		 checkNeon();
		 checkDevice();
		 checkVAO();
		 checkPostProcess();
		 checkShadow();
	 }

    ~C3DDeviceAdapter()
    {
 
        _deviceAdapterInstance = NULL;
    }
    

	static C3DDeviceAdapter* getInstance();
	
    void checkCpuInfo();

	void checkVAO();
	
	void checkNeon();

	void checkPostProcess();
	void checkShadow();

	void checkDevice();
	
	unsigned int getCpuCount()
	{
		return _cpuCount;
	}
    
    unsigned int getCpuFrequency()
    {
        return _cpuFrequency;
    }
    
    std::string getPlatformName()
    {
        return _platformName;
    }

	DeviceLevel getDeviceLevel()
	{
		return _deviceLevel;
	}

	bool isSuppoetNeon()
	{
		return _supportNeon;
	}

	
	bool isSupportVAO()
	{
		return _supportVAO;
	}

	void setSupportShadow(bool support)
	{
		_supportShadow = support;
	}
	bool isSupportShadow()
	{
		return _supportShadow;
	}

	void setSupportPostProcess(bool support)
	{
		_supportPostProcess = support;
	}

	bool isSupportPostProcess()
	{
		return _supportPostProcess;
	}
	

private:

	static C3DDeviceAdapter* _deviceAdapterInstance;
	unsigned int _cpuCount;
    unsigned int _cpuFrequency;
    std::string _platformName;
	DeviceLevel _deviceLevel;
	bool _supportVAO;
	bool _supportPostProcess;
	bool _supportShadow;
	bool _supportNeon;

};
}


#endif
