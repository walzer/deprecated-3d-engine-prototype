#ifndef PESpaceWarp_H
#define PESpaceWarp_H
#include "cocos2d.h"
#include "C3DPostProcess.h"

namespace cocos3d
{
/**
Space wrap post effect
*/
class PESpaceWrap : public C3DPostEffect
{
    friend class C3DPostEffect;
protected:
	/**
     * Constructor & Destructor
     */
	PESpaceWrap(C3DPostProcess* postProcess, const std::string& name);

	virtual ~PESpaceWrap();

public:

	static C3DPostEffect* create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess );

	void update( float dt );

	/**
     * set post effect shader parameter
     */
	virtual void setShaderParameter();

	/**
     * set pixelsize
     */
	void setPixelSize(const C3DVector2& pixelsize)
	{
		_pixelSize = pixelsize;
	}

	/**
     * set space wrap speed
     */
	void setWrapSpeed(float wrapSpeed)
	{
		_wrapSpeed = wrapSpeed;
	}

	/**
     * set space wrap scale
     */
	void setWrapScale(float wrapScale)
	{
		_wrapScale = wrapScale;
	}

protected:
	// properties
	C3DVector2 _pixelSize;
	float _wrapSpeed;
	float _wrapScale;
	C3DSampler* _wrapSampler;
	float _time;
};
}

#endif	//PESpaceWarp_H
