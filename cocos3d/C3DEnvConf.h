#ifndef __C3DEnvConf_H__
#define __C3DEnvConf_H__

#include "cocos3d.h"

#include <string>

#include "C3DRenderState.h"

namespace cocos3d
{
/**
*3D environment configuration.
*
*/
class C3DEnvConf
{
public:

	/**
	* fog type
	*/
	enum FogType
	{
		None,
		Linear,
		Exp,
		Exp2,
	};

public:

	C3DEnvConf()
		:_fogColor(1.0f, 1.0f, 1.0f, 1.0f), _clearColor(0.0f, 0.0f, 0.0f, 0.0f), _ambientColor(0.7f, 0.7f, 0.7f)
	{
		_fogType = None;
		_fogStart = 0.0f;
		_fogEnd = 1.0f;
		_fogDensity = 0.01f;
	}

	void set(C3DEnvConf config)
	{
		_clearColor = config._clearColor;
		_fogColor = config._fogColor;
		_fogType = config._fogType;
		_fogStart = config._fogStart;
		_fogEnd = config._fogEnd;
		_fogParam = C3DVector4(config._fogDensity,config._fogStart,config._fogEnd,(float)((int)config._fogType));
	}

	C3DVector4 _clearColor; //clear color

	C3DVector3 _ambientColor;//ambient color

	//fog type
	FogType _fogType;

	// fog parameter
	C3DVector4 _fogColor;
	C3DVector4 _fogParam;
	float _fogDensity;
	float _fogStart;
	float _fogEnd;
};
}
#endif
