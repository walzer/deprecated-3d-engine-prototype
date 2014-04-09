#include "PEBloom.h"

#include "C3DPostProcess.h"
#include "C3DMaterial.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "C3DFrameBuffer.h"
#include "C3DTexture.h"
#include "C3DDepthStencilTarget.h"
#include "C3DSampler.h"
#include "MaterialParameter.h"
#include "C3DRenderTarget.h"
#include "C3DModel.h"
#include "C3DRenderSystem.h"

namespace cocos3d
{
PEBloom::PEBloom(C3DPostProcess* postProcess, const std::string& name)
	: C3DPostEffect(postProcess, name)
	, _brightFrameBuffer(NULL)
	, _blurFrameBufferX(NULL)
	, _blurFrameBufferY(NULL)
	, _densityScale(1.5f)
{
}

bool PEBloom::init(const std::string& szMaterial)
{
    bool bRet = C3DPostEffect::init(szMaterial);
    if (!bRet)
        return false;

    CCAssert(_material->getTechniqueCount() == 4, "material error");
    return initBloomParam();
}

PEBloom::~PEBloom()
{
    SAFE_RELEASE(_blurFrameBufferX);
    SAFE_RELEASE(_blurFrameBufferY);
    SAFE_RELEASE(_brightFrameBuffer);
}

C3DPostEffect* PEBloom::create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess )
{
	C3DPostEffect* pe = new PEBloom( postProcess, name );
	bool bInit = pe->init( materialName );
	if (!bInit)
	{
		SAFE_RELEASE(pe);
	}
	return pe;
}

bool PEBloom::initBloomParam()
{
    unsigned int fmtColor = C3DTexture::RGBA;

    _brightFrameBuffer = C3DFrameBuffer::create("bloom_postprocess_bright", _postProcess->getFBWidth(), _postProcess->getFBHeight(), fmtColor/*, fmtDepth*/);
    _blurFrameBufferX = C3DFrameBuffer::create("bloom_postprocess_blurX", _postProcess->getFBWidth(), _postProcess->getFBHeight(), fmtColor/*, fmtDepth*/);
    _blurFrameBufferY = C3DFrameBuffer::create("bloom_postprocess_blurY", _postProcess->getFBWidth(), _postProcess->getFBHeight(), fmtColor/*, fmtDepth*/);

    if (_brightFrameBuffer == NULL || _blurFrameBufferX == NULL || _blurFrameBufferY == NULL)
    {
        return  false;
    }
    _brightFrameBuffer->retain();
    _blurFrameBufferX->retain();
    _blurFrameBufferY->retain();

    setBloomThreshold(0.25f);

    C3DSampler* sampler = _postProcess->getFramebufferSampler();
    sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
    sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
    
    C3DPass* pass1 = _material->getTechnique(1u)->getPass(0u);
    C3DPass* pass2 = _material->getTechnique(2u)->getPass(0u);
    C3DPass* pass3 = _material->getTechnique(3u)->getPass(0u);

    pass3->getParameter("u_texture")->setValue(sampler);

	sampler = C3DSampler::create(_brightFrameBuffer->getRenderTarget()->getTexture());
    sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
    sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
    pass1->getParameter("u_texture")->setValue(sampler);
    SAFE_RELEASE(sampler);

	sampler = C3DSampler::create(_blurFrameBufferX->getRenderTarget()->getTexture());
    sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
    sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
    pass2->getParameter("u_texture")->setValue(sampler);
    SAFE_RELEASE(sampler);

	sampler = C3DSampler::create(_blurFrameBufferY->getRenderTarget()->getTexture());
    sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
    sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
    pass3->getParameter("u_bloomTexture")->setValue(sampler);
    SAFE_RELEASE(sampler);

    setDensityScale(_densityScale);

    return true;
}

void PEBloom::draw()
{
    if (_material == NULL)
        return;

    if(_model->getMaterial() != _material)
    {
        _model->setMaterial(_material);
    }

	static C3DVector4 clearColor =  C3DVector4::zero();
    //extract brightness
	_brightFrameBuffer->bind();
	C3DRenderSystem::getInstance()->clear(CLEAR_COLOR,&clearColor,1.0f,0);
    _material->setTechnique(C3DMaterial::TECH_USAGE_SCREEN, 0u);
    _model->draw();
    _brightFrameBuffer->unbind();

    //blur x
	_blurFrameBufferX->bind();
	C3DRenderSystem::getInstance()->clear(CLEAR_COLOR,&clearColor,1.0f,0);
    _material->setTechnique(C3DMaterial::TECH_USAGE_SCREEN, 1u);
    _model->draw();
    _blurFrameBufferX->unbind();

    //blur y
	_blurFrameBufferY->bind();
	C3DRenderSystem::getInstance()->clear(CLEAR_COLOR,&clearColor,1.0f,0);
    _material->setTechnique(C3DMaterial::TECH_USAGE_SCREEN, 2u);
    _model->draw();
    _blurFrameBufferY->unbind();

    _material->setTechnique(C3DMaterial::TECH_USAGE_SCREEN, 3u);
    _model->draw();
}

void PEBloom::setBloomThreshold(float threshold)
{
    CCAssert(_material, "material null");
    _BloomThreshold = threshold;

    C3DTechnique* tech = _material->getTechnique(0u);
    C3DPass* pass = tech->getPass(0u);

    pass->getParameter("u_bloomThreshold")->setValue(_BloomThreshold);
}

float PEBloom::getBloomThreshold() const
{
    return _BloomThreshold;
}

void PEBloom::setDensityScale(float scale)
{
	_densityScale = scale;
	if (_material)
	{
		C3DPass* pass1 = _material->getTechnique(1u)->getPass(0u);
		C3DPass* pass2 = _material->getTechnique(2u)->getPass(0u);

		C3DVector2 vdelta;
		vdelta.x = 1.0f / ((float)_postProcess->getFBWidth() / 2.0f);
		vdelta.y = 0.0f;
		pass1->getParameter("u_delta")->setValue(vdelta);
		vdelta.x = 0.0f;
		vdelta.y = 1.0f / ((float)_postProcess->getFBHeight() / 2.0f);
		pass2->getParameter("u_delta")->setValue(vdelta);
	}
}

}