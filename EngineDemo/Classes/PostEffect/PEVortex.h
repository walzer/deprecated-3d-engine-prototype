#ifndef PEVortex_h
#define PEVortex_h

#include "C3DPostEffect.h"

namespace cocos3d
{
class MaterialParameter;

class PEVortex : public C3DPostEffect
{
	friend class C3DPostEffect;
protected:

	PEVortex(C3DPostProcess* postProcess, const std::string& name);
	virtual ~PEVortex();

public:

	static C3DPostEffect* create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess );

	virtual bool init(const std::string& szMaterial);

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
};
}	//namespace cocos3d
#endif	//#define PEVortex_h