#ifndef PEGlow_H
#define PEGlow_H
#include "cocos2d.h"
#include "C3DPostEffect.h"

namespace cocos3d
{
class C3DPostProcess;
class C3DTechnique;
class C3DSampler;
class C3DMaterial;
class C3DFrameBuffer;
class C3DModel;

/**
* Glow effect
*/
class PEGlow : public C3DPostEffect
{
    friend class C3DPostEffect;

protected:
    /**
     * Constructor & Destructor
     */
	PEGlow(C3DPostProcess* postProcess, const std::string& name);

	virtual ~PEGlow();

public:

	static C3DPostEffect* create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess );

    virtual bool init(const std::string& szMaterial);

    /**
     * draw routine
     */
    virtual void draw();

	/**
	* get set glow parameter
	*/
	void setTintColor(const C3DVector4& color);

	const C3DVector4& getTintColor() const { return _tintColor; }

	void setDensityScale(float scale);

	float getDensityScale() const { return _densityScale; }

	void setGlowIntensity( float f );

	float getGlowIntensity(void) const
	{
		return _glowIntensity;
	}
protected:

	bool initGlowaram();

    C3DFrameBuffer* _blurFrameBufferX; // frame buffer for blur
    C3DFrameBuffer* _blurFrameBufferY;

    C3DFrameBuffer* _glowFrameBuffer;

	C3DVector4 _tintColor;

    float _densityScale;
	float _glowIntensity;
};
}

#endif	//PEGlow_H
