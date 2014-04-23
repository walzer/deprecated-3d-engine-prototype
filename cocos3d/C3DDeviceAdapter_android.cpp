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


void C3DDeviceAdapter::checkNeon()
{
	AndroidCpuFamily cpuFamily = android_getCpuFamily();
	if( cpuFamily == ANDROID_CPU_FAMILY_ARM )
	{
		WARN("the cpu family is arm");
	}
	else if( cpuFamily == ANDROID_CPU_FAMILY_X86 )
	{
		WARN("the cpu family is x86");
	}
	else
	{
		WARN("unknown cpufamily");
	}

	if( cpuFamily == ANDROID_CPU_FAMILY_ARM )
	{
		uint64_t  cpuFeatures = android_getCpuFeatures();

		if( cpuFeatures & ANDROID_CPU_ARM_FEATURE_NEON )
		{
			_supportNeon = true;
			
		}
		else
		{
			_supportNeon = false;
			
		}
	}

	WARN_VARG("the cpu support arm neon:%d",_supportNeon);

	// 


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
	if(_deviceLevel == DeviceLevel::High)
		_supportShadow = true;
	else
		_supportShadow = false;

	_supportShadow = true;
}

void C3DDeviceAdapter::checkPostProcess()
{
	if(_deviceLevel == DeviceLevel::High)
		_supportPostProcess = true;
	else
		_supportPostProcess = false;

	_supportPostProcess = true;
}



}

#endif