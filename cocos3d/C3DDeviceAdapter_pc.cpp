#include "CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "Base.h"
#include "C3DDeviceAdapter.h"

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
	_cpuCount = 4;

    WARN_VARG("default set the cpu counter of pc is : %d", _cpuCount);

}

void C3DDeviceAdapter::checkNeon()
{
	_supportNeon = false;
}

void C3DDeviceAdapter::checkVAO()
{
	

	if(_deviceLevel == DeviceLevel::High)
	{
		_supportVAO = cocos2d::Configuration::getInstance()->checkForGLExtension("vertex_array_object");
	}
	else
		_supportVAO = false;

	WARN_VARG("the gpu support VAO : %d",_supportVAO);
}

void C3DDeviceAdapter::checkDevice()
{
	if(_cpuCount == 1)
		_deviceLevel = DeviceLevel::Low;
	else if(_cpuCount == 2)
		_deviceLevel = DeviceLevel::Mid;
	else if(_cpuCount >= 4)
		_deviceLevel = DeviceLevel::High;
	else
		_deviceLevel = DeviceLevel::Unknow;

	WARN_VARG("the devicelevel is : %d",(int)_deviceLevel);
}

void C3DDeviceAdapter::checkShadow()
{
	_supportShadow = false;
}

void C3DDeviceAdapter::checkPostProcess()
{

	_supportPostProcess = false;
}







}

#endif