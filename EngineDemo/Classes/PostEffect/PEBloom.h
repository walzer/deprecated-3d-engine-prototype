#ifndef PEBloom_H
#define PEBloom_H
#include "cocos2d.h"
#include "C3DPostEffect.h"

namespace cocos3d
{
/**
* Bloom effect
*/
class PEBloom : public C3DPostEffect
{
    friend class C3DPostEffect;

protected:
    /**
     * Constructor & Destructor
     */
	PEBloom(C3DPostProcess* postProcess, const std::string& name);

	virtual ~PEBloom();

public:

	static C3DPostEffect* create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess );

    virtual bool init(const std::string& szMaterial);

    /**
     * draw routine
     */
    virtual void draw();

    /**
     * get & set bloom parameter, brightness threshold
     */
    void setBloomThreshold(float threshold);
    float getBloomThreshold() const;

	void setDensityScale(float scale);
	float getDensityScale() const{  return _densityScale; }
protected:

    bool initBloomParam();

    //bloom parameter
    float  _BloomThreshold;    //

    C3DFrameBuffer* _brightFrameBuffer; // frame buffer for select bright pixels
    C3DFrameBuffer* _blurFrameBufferX; // frame buffer for blur
    C3DFrameBuffer* _blurFrameBufferY;

    C3DFrameBuffer* _blurFrameBuffer;

	float _densityScale;
};
}

#endif	//PEBloom_H
