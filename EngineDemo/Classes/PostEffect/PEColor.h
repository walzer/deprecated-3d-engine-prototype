#ifndef PEColor_H
#define PEColor_H
#include "C3DPostEffect.h"

namespace cocos3d
{

/**
Gray color post effect
*/
class PEColor : public C3DPostEffect
{
    friend class C3DPostEffect;
protected:
	/**
     * Constructor & Destructor
     */
	PEColor(C3DPostProcess* postProcess, const std::string& name): C3DPostEffect(postProcess, name), _colorRate(1.5f, 1.0f, 0.5f)
    {
    }
    virtual ~PEColor();

public:

	static C3DPostEffect* create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess );

	/**
     * set post effect shader parameter
     */
	virtual void setShaderParameter();

    void setColorRate(const C3DVector3& colorRate)
    {
        _colorRate = colorRate;
    }

    const C3DVector3& getColorRate() const
    {
        return _colorRate;
    }

protected:

    C3DVector3 _colorRate;
};

}

#endif	//PEColor_H
