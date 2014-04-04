#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "Base.h"
#include "C3DDeviceAdapter.h"
#include <cpu-features.h>
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
	_cpuCount = android_getCpuCount();

    WARN_VARG("the cpu counter is : %d", _cpuCount);

}



}

#endif