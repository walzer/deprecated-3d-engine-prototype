#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Base.h"
#include "C3DDeviceAdapter.h"

#include <sys/sysctl.h>

namespace cocos3d
{
    
C3DDeviceAdapter::C3DDeviceAdapter()
{
}

C3DDeviceAdapter::~C3DDeviceAdapter()
{

}

int C3DDeviceAdapter::getCpuCount()
{
    size_t size = sizeof(int);
    int cpu_num = 0;
    int mib[2] = {CTL_HW, HW_NCPU};
    sysctl(mib, 2, &cpu_num, &size, NULL, 0);
    return cpu_num;
}



}

#endif