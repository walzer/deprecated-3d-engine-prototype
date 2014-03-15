#ifndef PESceneChange_H
#define PESceneChange_H

#include "C3DPostEffect.h"

namespace cocos3d
{
class C3DPass;
class C3DPostEffect;
class C3DPostProcess;
class MaterialParameter;

class PESceneChange : public C3DPostEffect
{
	friend class C3DPostEffect;

protected:

	PESceneChange(C3DPostProcess* postProcess, const std::string& name);

	virtual ~PESceneChange();

public:
	static C3DPostEffect* create( const std::string& name,const std::string& materialName, C3DPostProcess* postProcess );

	virtual bool init(const std::string& szMaterial);
	virtual void draw();

	void update( float dt );

	virtual void setShaderParameter();

	void setAngle( float f )
	{
		_angle = f;
	}
	float getAngel() const
	{
		return _angle;
	}

	void setRadius( float f )
	{
		_radius = f;
	}
	float getRadius()
	{
		return _radius;
	}

	float _angleSpeed;
	float _radiusSpeed;
	float _Time;
	float _maxTime;

private:
	float _angle;
	float _radius;
	MaterialParameter* _paramRadus;
	MaterialParameter* _paramAngle;
	C3DFrameBuffer* _halfFrameBuffer;

	C3DMaterial* _matCopyToScreen;
};
}

#endif	//#define PESceneChange_H
