#include "PESceneChange.h"
#include "C3DPostProcess.h"
#include "C3DMaterial.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "MaterialParameter.h"
#include "C3DTexture.h"
#include "C3DSampler.h"
#include "C3DFrameBuffer.h"
#include "C3DModel.h"
#include "C3DRenderTarget.h"
#include "C3DFrameBuffer.h"
#include "MaterialParameter.h"
#include "C3DRenderSystem.h"
#include "C3DMaterialManager.h"

namespace cocos3d
{
PESceneChange::PESceneChange(C3DPostProcess* postProcess, const std::string& name)
	: C3DPostEffect( postProcess, name )
	, _angle( 0.f )
	, _radius( 0.f )
	, _paramRadus( NULL )
	, _paramAngle( NULL )
	, _halfFrameBuffer( NULL )
	, _matCopyToScreen( NULL )
	, _angleSpeed(0.75)
	, _radiusSpeed(0.25)
	, _Time( 0.0 )
	, _maxTime( 2.0 )
{
}

PESceneChange::~PESceneChange(void)
{
	SAFE_RELEASE( _halfFrameBuffer );
	SAFE_RELEASE( _matCopyToScreen );
}

C3DPostEffect* PESceneChange::create( const std::string& name,const std::string& materialName, C3DPostProcess* postProcess )
{
	C3DPostEffect* pe = new PESceneChange( postProcess, name );
	bool bInit = pe->init( materialName );
	if (!bInit)
	{
		SAFE_RELEASE(pe);
	}
	return pe;
}

bool PESceneChange::init(const std::string& szMaterial)
{
	bool bRet = C3DPostEffect::init(szMaterial);
	if (!bRet)
	{
		return false;
	}

	_matCopyToScreen = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource("demores/posteffect/postprocess_scenecopy.material"));
	_matCopyToScreen->retain();

	float fbScale = 0.5f;
	_halfFrameBuffer = C3DFrameBuffer::create("PESceneChange::half", _postProcess->getFBWidth()*fbScale, _postProcess->getFBHeight()*fbScale);
	_halfFrameBuffer->retain();

	C3DTechnique* tech = _material->getTechnique(0u);
	if ( tech == NULL )
	{
		return false;
	}

	C3DPass* pass = tech->getPass(0u);
	if ( pass == NULL )
	{
		return false;
	}

	C3DSampler* samplerBase = _postProcess->getFramebufferSampler();
	samplerBase->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
	samplerBase->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
	pass->getParameter("u_texture")->setValue(samplerBase);

	C3DSampler* samplerHalf = C3DSampler::create(_halfFrameBuffer->getRenderTarget()->getTexture());
	samplerHalf->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
	samplerHalf->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
	_matCopyToScreen->getTechnique(0u)->getPass( 0u )->getParameter("u_texture")->setValue( samplerHalf );
	SAFE_RELEASE(samplerHalf);

	_paramRadus = pass->getParameter( "radius" );
	_paramAngle = pass->getParameter( "angle" );

	return true;
}

void PESceneChange::setShaderParameter()
{
	if ( _paramRadus != NULL )
	{
		_paramRadus->setValue( _radius );
	}

	if ( _paramAngle != NULL )
	{
		_paramAngle->setValue( _angle );
	}
}

void PESceneChange::update( float dt )
{
	if ( _Time < _maxTime )
	{
		setAngle( getAngel()+_angleSpeed*dt );
		setRadius( getRadius()+_radiusSpeed*dt );
		_Time += dt;
	}
	else
	{
		_Time = 0.0;
		setAngle( 0.0f );
		setRadius( 0.0f );
	}
}

void PESceneChange::draw()
{
	if (_material == NULL || _postProcess == NULL )
	{
		return;
	}

	setShaderParameter();

	// 渲到1/2尺寸rt上
	_halfFrameBuffer->bind();
	static C3DVector4 clearColor =  C3DVector4::zero();
	C3DRenderSystem::getInstance()->clear(CLEAR_COLOR,&clearColor,1.0f,0);
	_model->setMaterial( _material );
	_model->draw();
	_halfFrameBuffer->unbind();

	// 渲到屏幕上
	_model->setMaterial( _matCopyToScreen );
	_model->draw();
}
}