#include "C3DPostProcess.h"
#include "C3DFrameBuffer.h"
#include "C3DRenderTarget.h"
#include "C3DDepthStencilTarget.h"
#include "C3DTexture.h"
#include "C3DSampler.h"
#include "C3DMaterial.h"
#include "C3DRenderState.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "MaterialParameter.h"
#include "C3DSkinlessModel.h"
#include "Geo.h"
#include "Base.h"
#include "C3DMesh.h"
#include "C3DRenderSystem.h"
#include "C3DVector4.h"
#include "C3DPostEffect.h"
#include <map>
#include "C3DDeviceAdapter.h"

namespace cocos3d
{
C3DPostProcess* g_pPostProcess = NULL;

C3DPostProcess::C3DPostProcess(const std::string& id,C3DFrameBuffer* buffer)
	: C3DNode(id)
	, _framebuffer(buffer)
	, _isDraw( false )
{
	_curPostEffect = NULL;
	g_pPostProcess = this;
}

C3DPostProcess::~C3DPostProcess()
{
	PostEffects::iterator ite = _postEffects.begin();
	for ( ; ite != _postEffects.end(); ++ite )
	{
		SAFE_RELEASE( (*ite).second );
		_postEffects.erase( ite );
	}
	_postEffects.clear();

	SAFE_RELEASE(_framebuffer);

	SAFE_RELEASE(_sampler);

	//SAFE_RELEASE(_model);

	for ( PostEffects::iterator ite = _postEffects.begin(); ite != _postEffects.end(); ++ite )
	{
		SAFE_RELEASE( (*ite).second );
	}
	_postEffects.clear();

	g_pPostProcess = NULL;
}

C3DPostProcess* C3DPostProcess::create(const std::string& id, unsigned int texWidth, unsigned int texHeight/*, const std::string& techniqueId*/)
{
	if(C3DDeviceAdapter::getInstance()->isSuppertPostProcess() == false)
		return NULL;

	unsigned int fmtColor = C3DTexture::RGBA;
	//unsigned int fmtDepth = C3DDepthStencilTarget::DEPTH24_STENCIL8;
	unsigned int fmtDepth = C3DDepthStencilTarget::DEPTH16;

	C3DFrameBuffer* framebuffer = C3DFrameBuffer::create(id, texWidth, texHeight, fmtColor, fmtDepth);
	if (!framebuffer)
		return NULL;
	framebuffer->retain();

	C3DPostProcess* pp = new C3DPostProcess(id,framebuffer);
	pp->_fbWidth = texWidth;
	pp->_fbHeight = texHeight;
	pp->autorelease();

	C3DSampler* sampler = C3DSampler::create(pp->_framebuffer->getRenderTarget()->getTexture());
	sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
	sampler->setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
    pp->_sampler = sampler;

	return pp;
}

void C3DPostProcess::preChannelDraw()
{
    if (_curPostEffect == NULL)
        return;

    CCAssert(_framebuffer, "Empty frame buffer");;
    _framebuffer->bind();

	static C3DVector4 clearColor =  C3DVector4::zero();
	C3DRenderSystem::getInstance()->clear(CLEAR_COLOR_DEPTH,&clearColor,1.0f,0);
}

void C3DPostProcess::postChannelDraw()
{
    if (_curPostEffect == NULL)
        return;

    CCAssert(_framebuffer, "Empty frame buffer");;
    _framebuffer->unbind();
}

void C3DPostProcess::beginDraw()
{
	_isDraw = true;
}

void C3DPostProcess::endDraw()
{
	_isDraw = false;
}

void C3DPostProcess::draw()
{
    if (_curPostEffect && _isDraw)
	{
        _curPostEffect->draw();
	}
}

bool C3DPostProcess::addPostEffect( C3DPostEffect* effect )
{
	if ( effect == NULL )
	{
		return false;
	}
	else
	{
		if ( _postEffects.find( effect->getName() ) != _postEffects.end() )
		{
			return false;
		}
		else
		{
			_postEffects.insert( std::make_pair( effect->getName(), effect ) );
			return true;
		}
	}
}

void C3DPostProcess::removePostEffect( const std::string& name )
{
	PostEffects::iterator ite = _postEffects.find( name );
	if ( ite != _postEffects.end() )
	{
		SAFE_RELEASE( (*ite).second );
		_postEffects.erase( ite );
	}
}

C3DPostEffect* C3DPostProcess::getPostEffect( const std::string& name )
{
	PostEffects::iterator ite = _postEffects.find( name );
	if ( ite != _postEffects.end() )
	{
		return (*ite).second;
	}
	else
	{
		return NULL;
	}
}

C3DPostEffect* C3DPostProcess::setActiveEffect( const std::string& name )
{
	PostEffects::iterator ite = _postEffects.find( name );
	if ( ite == _postEffects.end() )
	{
		_curPostEffect = NULL;
		return NULL;
	}
	else
	{
		_curPostEffect = (*ite).second;
		return _curPostEffect;
	}
	/**
    for (size_t i = 0; i < _postEffects.size(); i++) {
        if (_postEffects[i]->getPostEffectType() == type)
        {
            _curPostEffect = _postEffects[i];
            return;
        }
    }

    _curPostEffect = C3DPostEffect::create(type, this, "");
    _postEffects.push_back(_curPostEffect);
	//*/
}

unsigned int C3DPostProcess::getFBWidth()
{
	return _fbWidth;
}

unsigned int C3DPostProcess::getFBHeight()
{
	return _fbHeight;
}

void C3DPostProcess::transformChanged()
{
    C3DNode::transformChanged();
    //  _dirty |= DIRTY_MATRIX;
}

std::vector<std::string> C3DPostProcess::getEffectNames(void) const
{
	std::vector<std::string> names;
	for ( PostEffects::const_iterator ite = _postEffects.begin(); ite != _postEffects.end(); ++ite )
	{
		names.push_back( (*ite).first );
	}
	return names;
}

void C3DPostProcess::reload()
{
	for ( PostEffects::const_iterator ite = _postEffects.begin(); ite != _postEffects.end(); ++ite )
	{
		(*ite).second->reload();
	}
}

}