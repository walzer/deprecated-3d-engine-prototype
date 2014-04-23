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
        
        sysctlbyname("hw.model", NULL, &size, NULL, 0);
        char *answer = (char*)malloc(size);
        sysctlbyname("hw.model", answer, &size, NULL, 0);
        _deviceName = answer;
    }
    
    void C3DDeviceAdapter::checkVAO()
    {
    }
	
    void C3DDeviceAdapter::checkNeon()
    {
        
    }
    
	void C3DDeviceAdapter::checkPostProcess()
    {
        
    }

	void C3DDeviceAdapter::checkShadow()
    {
        
    }
    
	void C3DDeviceAdapter::checkDevice()
    {
        
    }

}

#endif