#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Base.h"
#include "C3DDeviceAdapter.h"

#include <sys/sysctl.h>

namespace cocos3d
{
    C3DDeviceAdapter* C3DDeviceAdapter::_deviceAdapterInstance = NULL;
    
    C3DDeviceAdapter* C3DDeviceAdapter::getInstance()
    {
        if ( _deviceAdapterInstance == NULL )
        {
            _deviceAdapterInstance = new C3DDeviceAdapter();
            _deviceAdapterInstance->autorelease();
        }
        return _deviceAdapterInstance;
    }
    
    void C3DDeviceAdapter::checkCpuInfo()
    {
        size_t size = sizeof(int);
   
        int mib[2] = {CTL_HW, HW_NCPU};
        sysctl(mib, 2, &_cpuCount, &size, NULL, 0);

        WARN_VARG("the cpu counter is : %d", _cpuCount);
        
        size = sizeof(int);
        int mib_[2] = {CTL_HW, HW_CPU_FREQ};
        sysctl(mib_, 2, &_cpuFrequency, &size, NULL, 0);
        WARN_VARG("the cpu frequency is : %d", _cpuFrequency);
        
        sysctlbyname("hw.machine", NULL, &size, NULL, 0);
        char *answer = (char*)malloc(size);
        sysctlbyname("hw.machine", answer, &size, NULL, 0);
        _platformName = answer;
        WARN_VARG("the device name is : %s", answer);

    }
    
    void C3DDeviceAdapter::checkVAO()
    {
        _supportVAO = true;
    }
	
    void C3DDeviceAdapter::checkNeon()
    {
        _supportNeon = true;
    }
    
	void C3DDeviceAdapter::checkPostProcess()
    {
        _supportPostProcess = true;
    }

	void C3DDeviceAdapter::checkShadow()
    {
        _supportShadow = true;
    }
    
	void C3DDeviceAdapter::checkDevice()
    {
        if(_platformName.find("iPhone") != std::string::npos)
        {
            if(_platformName.find("iPhone1") != std::string::npos)       // iPhone 1g
                _deviceLevel = DeviceLevel::Low;
            else if(_platformName.find("iPhone2") != std::string::npos)  // iPhone 3g
                _deviceLevel = DeviceLevel::Low;
            else if(_platformName.find("iPhone3") != std::string::npos)  // iPhone 4
                _deviceLevel = DeviceLevel::Low;
            else if(_platformName.find("iPhone4") != std::string::npos)  // iPhone 4s
                _deviceLevel = DeviceLevel::Mid;
            else
                _deviceLevel = DeviceLevel::High;                        // other iPhone
        }
        else if (_platformName.find("iPad") != std::string::npos)
        {
            if(_platformName.find("iPad1") != std::string::npos)         // iPad 1
                _deviceLevel = DeviceLevel::Low;
            else if(_platformName.find("iPad2") != std::string::npos)    // iPad 2
                _deviceLevel = DeviceLevel::Mid;
            else
                _deviceLevel = DeviceLevel::High;                        // other iPad
            
        }
        else if (_platformName.find("iPod") != std::string::npos)
        {
            if(_platformName.find("iPod1") != std::string::npos)         // iPod 1
                _deviceLevel = DeviceLevel::Low;
            else if(_platformName.find("iPod2") != std::string::npos)    // iPod 2
                _deviceLevel = DeviceLevel::Low;
            else if(_platformName.find("iPod3") != std::string::npos)    // iPod 3
                _deviceLevel = DeviceLevel::Low;
            else if(_platformName.find("iPod4") != std::string::npos)    // iPod 4
                _deviceLevel = DeviceLevel::Mid;
            else
                _deviceLevel = DeviceLevel::High;                        // other iPod
        }
        else if(_platformName.find("x86") != std::string::npos)
        {
            _deviceLevel = DeviceLevel::High;                            // Simulator
        }
        else
        {
            _deviceLevel = DeviceLevel::Unknow;                          // other ios device
        }
        
        WARN_VARG("the device level is : %d", _deviceLevel);
    }

}

#endif