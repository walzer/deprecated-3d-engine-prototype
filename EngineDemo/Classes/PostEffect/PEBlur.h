#ifndef PEBlur_H
#define PEBlur_H
#include "cocos2d.h"
#include "C3DPostEffect.h"

namespace cocos3d
{
/**
Blur post effect
*/
class PEBlur : public C3DPostEffect
{
    friend class C3DPostEffect;
protected:
	/**
     * Constructor & Destructor
     */

    PEBlur(C3DPostProcess* postProcess, const std::string& szName): C3DPostEffect(postProcess, szName)
    {
    }
    virtual ~PEBlur();

public:

	static C3DPostEffect* create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess );

	/**
     * set post effect shader parameter
     */
	virtual void setShaderParameter();
};
}

#endif	//PEBlur_H
