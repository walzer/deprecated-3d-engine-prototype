#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "Base.h"
#include "C3DDeviceAdapter.h"
#include <cpu-features.h>
namespace cocos3d
{
C3DDeviceAdapter* C3DDeviceAdapter::_instance = NULL;

C3DDeviceAdapter* C3DDeviceAdapter::getInstance()
{
	if ( _instance == NULL )
	{
		_instance = new C3DDeviceAdapter();
		_instance->autorelease();
	}
	return _instance;
}

void C3DDeviceAdapter::checkCpuInfo()
{
	_cpuCount = android_getCpuCount();

    WARN_VARG("the cpu counter is : %d", _cpuCount);

}



}

#endif