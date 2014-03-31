#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "Base.h"
#include "C3DDeviceAdapter.h"

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
	int m = 4;

    LOG_ERROR_VARG("the cpu counter is : %d", m);

	return m;
}







}

#endif