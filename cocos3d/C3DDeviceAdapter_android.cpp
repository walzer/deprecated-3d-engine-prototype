#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "Base.h"
#include "C3DDeviceAdapter.h"
#include <cpu-features.h>
namespace cocos3d
{
C3DDeviceAdapter* C3DDeviceAdapter::_instance = NULL;

C3DDeviceAdapter::C3DDeviceAdapter()
{
}

C3DDeviceAdapter::~C3DDeviceAdapter()
{

}

C3DDeviceAdapter* C3DDeviceAdapter::getInstance()
{
	if ( _instance == NULL )
	{
		_instance = new C3DDeviceAdapter();
		_instance->autorelease();
	}
	return _instance;
}

int C3DDeviceAdapter::getCpuCount()
{
	int m = android_getCpuCount();

    LOG_ERROR_VARG("the cpu counter is : %d", m);

	return m;
}



}

#endif