#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "Base.h"
#include "C3DDeviceAdapter.h"

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
	_cpuCount = 4;

    WARN_VARG("default set the cpu counter of pc is : %d", _cpuCount);

}







}

#endif