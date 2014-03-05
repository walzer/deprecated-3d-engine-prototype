#ifndef PEPointWarp_H
#define PEPointWarp_H

#include "C3DPostEffect.h"

namespace cocos3d
{
static const unsigned int MAX_CLICKS = 8;

class MaterialParameter;

class PEPointWarp : public C3DPostEffect
{
	friend class C3DPostEffect;
protected:

	PEPointWarp(C3DPostProcess* postProcess, const std::string& name);
	virtual ~PEPointWarp();

public:
	static C3DPostEffect* create( const std::string& name,const std::string& materialName, C3DPostProcess* postProcess );

	virtual bool init(const std::string& szMaterial);

	virtual void setShaderParameter();

	C3DVector4* getClicks(void)
	{
		return _clicks;
	}

	C3DVector4* getEmptyClick(void);
	void addClick( float x, float y );

	void update( float dt );

	void setSpeed( float f )
	{
		_speed = f;
	}
	float getSpeed(void) const
	{
		return _speed;
	}

	void setMaxRadius( float f )
	{
		_maxRadius = f;
	}
	float getMaxRadius(void) const
	{
		return _maxRadius;
	}

private:
	void resetClick( C3DVector4* click );

private:
	float _speed;
	float _maxRadius;
	C3DVector4 _clicks[MAX_CLICKS];
	MaterialParameter* _paramClicks;
};
}	//namespace cocos3d
#endif	//#define PEPointWarp_H