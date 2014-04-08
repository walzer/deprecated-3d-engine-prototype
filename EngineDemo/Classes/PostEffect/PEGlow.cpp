#include "PEGlow.h"

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
PEGlow::PEGlow(C3DPostProcess* postProcess, const std::string& name)
	: C3DPostEffect(postProcess, name)
	, _blurFrameBufferX(NULL)
	, _blurFrameBufferY(NULL)
	, _glowFrameBuffer(NULL)
	, _tintColor(1.0f, 1.0f, 1.0f, 1.0f)
	, _densityScale(1.0f)
	, _glowIntensity( 1.0 )
{
}

C3DPostEffect* PEGlow::create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess )
{
	C3DPostEffect* pe = new PEGlow( postProcess, name );
	bool bInit = pe->init( materialName );
	if (!bInit)
	{
		SAFE_RELEASE(pe);
	}
	return pe;
}

bool PEGlow::init(const std::string& szMaterial)
{
	bool bRet = C3DPostEffect::init(szMaterial);
	if (!bRet)
		return false;

	CCAssert(_material->getTechniqueCount() == 3, "material error");
	return initGlowaram();
}

bool PEGlow::initGlowaram()
{
	unsigned int fmtColor = C3DTexture::RGBA;

	_blurFrameBufferX = C3DFrameBuffer::create("glow_postprocess_blurX", _postProcess->getFBWidth()*0.5, _postProcess->getFBHeight()*0.5, fmtColor/*, fmtDepth*/);
	_blurFrameBufferY = C3DFrameBuffer::create("glow_postprocess_blurY", _postProcess->getFBWidth()*0.5, _postProcess->getFBHeight()*0.5, fmtColor/*, fmtDepth*/);

	if (_blurFrameBufferX == NULL || _blurFrameBufferY == NULL)
	{
		return  false;
	}

	_blurFrameBufferX->retain();
	_blurFrameBufferY->retain();

	C3DSampler* sampler = _postProcess->getFramebufferSampler();
	sampler->setFilterMode(Texture_Filter_NEAREST, Texture_Filter_NEAREST);
	sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);

	C3DPass* pass1 = _material->getTechnique(1u)->getPass(0u);
	C3DPass* pass2 = _material->getTechnique(2u)->getPass(0u);

	//pass0->getParameter("u_texture")->setValue(sampler);
	pass2->getParameter("u_texture")->setValue(sampler);

	sampler = C3DSampler::create(_blurFrameBufferX->getRenderTarget()->getTexture());
	sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
	sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
	pass1->getParameter("u_texture")->setValue(sampler);
	SAFE_RELEASE(sampler);

	sampler = C3DSampler::create(_blurFrameBufferY->getRenderTarget()->getTexture());
	sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
	sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
	pass2->getParameter("u_glowTexture")->setValue(sampler);
	SAFE_RELEASE(sampler);

	setTintColor(_tintColor);
	setDensityScale(_densityScale);
	setGlowIntensity(_glowIntensity);
	return true;
}

void PEGlow::setTintColor(const C3DVector4& color)
{
	_tintColor = color;

	if (_material)
	{
		C3DPass* pass2 = _material->getTechnique(2u)->getPass(0u);
		pass2->getParameter("u_tintColor")->setValue(_tintColor);
	}
}

void PEGlow::setDensityScale(float density)
{
	_densityScale = density;
	if (_material)
	{
		C3DPass* pass0 = _material->getTechnique(0u)->getPass(0u);
		C3DPass* pass1 = _material->getTechnique(1u)->getPass(0u);

		C3DVector2 vdelta;
		vdelta.x = 1.0f / ((float)_postProcess->getFBWidth() / 4.0f);
		vdelta.y = 0.0f;
		pass0->getParameter("u_delta")->setValue(vdelta*_densityScale);
		vdelta.x = 0.0f;
		vdelta.y = 1.0f / ((float)_postProcess->getFBHeight() / 4.0f);
		pass1->getParameter("u_delta")->setValue(vdelta*_densityScale);
	}
}

void PEGlow::setGlowIntensity( float f )
{
	_glowIntensity = f;

	if (_material)
	{
		C3DPass* pass2 = _material->getTechnique(2u)->getPass(0u);
		pass2->getParameter("u_glowIntensity")->setValue(_glowIntensity);
	}
}

PEGlow::~PEGlow()
{
	SAFE_RELEASE(_blurFrameBufferX);
	SAFE_RELEASE(_blurFrameBufferY);
	SAFE_RELEASE(_glowFrameBuffer);
}

void PEGlow::draw()
{
	if (_material == NULL)
		return;

	if(_model->getMaterial() != _material)
	{
		_model->setMaterial(_material);
	}

	static C3DVector4 clearColor =  C3DVector4::zero();

	//blur x
	_blurFrameBufferX->bind();
	C3DRenderSystem::getInstance()->clear(CLEAR_COLOR,&clearColor,1.0f,0);
	_material->setTechnique(C3DMaterial::TECH_USAGE_SCREEN, 0u);
	_model->draw();
	_blurFrameBufferX->unbind();

	//blur y
	_blurFrameBufferY->bind();
	C3DRenderSystem::getInstance()->clear(CLEAR_COLOR,&clearColor,1.0f,0);
	_material->setTechnique(C3DMaterial::TECH_USAGE_SCREEN, 1u);
	_model->draw();
	_blurFrameBufferY->unbind();

	_material->setTechnique(C3DMaterial::TECH_USAGE_SCREEN, 2u);
	_model->draw();
}
}