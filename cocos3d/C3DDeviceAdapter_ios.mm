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

	WARN_VARG("default set the cpu counter of pc is : %d", _cpuCount);
}

}

#endif