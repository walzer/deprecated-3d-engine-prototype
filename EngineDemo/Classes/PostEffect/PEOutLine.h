#ifndef OutLine_H
#define OutLine_H
#include "cocos2d.h"
#include "C3DPostEffect.h"

namespace cocos3d
{
/**
Edge post effect
*/
class PEOutLine : public C3DPostEffect
{
    friend class C3DPostEffect;
protected:
	/**
     * Constructor & Destructor
     */
	PEOutLine(C3DPostProcess* postProcess, const std::string& name): C3DPostEffect(postProcess, name)
    {
    }
    virtual ~PEOutLine();

public:
	static C3DPostEffect* create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess );

	/**
     * set post effect shader parameter
     */
	virtual void setShaderParameter();
};
}

#endif	//OutLine_H
