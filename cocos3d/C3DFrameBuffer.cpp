#include "C3DFrameBuffer.h"
#include "C3DRenderTarget.h"
#include "C3DDepthStencilTarget.h"
#include "C3DTexture.h"
#include "ccMacros.h"
#include "C3DRenderSystem.h"

namespace cocos3d
{

C3DFrameBuffer::C3DFrameBuffer(const std::string& id) :
    _id(id), _handle(0), _renderTarget(NULL), _depthStencilTarget(NULL), _width(1), _height(1),
    _isBind(false), _oldFBO(0), _oldViewport(0, 0, 1, 1)
{
	C3DFrameBufferMgr::getInstance()->add(this);
}

C3DFrameBuffer::~C3DFrameBuffer()
{
	SAFE_RELEASE(_renderTarget);
    SAFE_RELEASE(_depthStencilTarget);
    // Release GL resource.
    if (_handle)
    {
        GL_ASSERT( glDeleteFramebuffers(1, &_handle) );
    }

	C3DFrameBufferMgr::getInstance()->remove(this);
}

C3DFrameBuffer* C3DFrameBuffer::create(const std::string& id, unsigned int width, unsigned int height, unsigned int fmtColor, unsigned int fmtDepth)
{
    // Create C3DRenderTarget with same ID
	LOG_TRACE_VARG("++++++++create FBO:%s    %d    %d    %d    %d", id.c_str(), width, height, fmtColor, fmtDepth);
    C3DRenderTarget* renderTarget = NULL;

    if (fmtColor)
    {
        renderTarget = C3DRenderTarget::create(id, width, height, fmtColor);
        if (renderTarget == NULL)
        {
            LOG_ERROR("Failed to create render target");
        }
    }

    C3DDepthStencilTarget* depthTarget = NULL;

    if (fmtDepth)
    {
        depthTarget = C3DDepthStencilTarget::create(id, (C3DDepthStencilTarget::Format) fmtDepth, width, height);
        if (depthTarget == NULL)
        {
            LOG_ERROR("Failed to create depth & stencil target");
        }
    }

    // Create the frame buffer
    GLuint handle = 0;
    GL_ASSERT( glGenFramebuffers(1, &handle) );

    // Create the new frame buffer
    C3DFrameBuffer* frameBuffer = new C3DFrameBuffer(id);
	if (frameBuffer == NULL)
    {
        LOG_ERROR("Failed to create frame buffer object");
        return NULL;
    }

    frameBuffer->_handle = handle;

    // Add the render target as the first color attachment
    if (renderTarget)
        frameBuffer->setRenderTarget(renderTarget);

    if (depthTarget)
        frameBuffer->setDepthStencilTarget(depthTarget);

    // Add to the global list of managed frame buffers
    frameBuffer->_width = width;
    frameBuffer->_height = height;
    frameBuffer->autorelease();
    return frameBuffer;
}

C3DFrameBuffer* C3DFrameBuffer::getFrameBuffer(const std::string& id)
{
	return C3DFrameBufferMgr::getInstance()->get(id);
}

const std::string& C3DFrameBuffer::getID() const
{
    return _id;
}

void C3DFrameBuffer::reload()
{
	GLuint handle = 0;
	GL_ASSERT( glGenFramebuffers(1, &handle) );

	_handle = handle;

	setRenderTarget(_renderTarget);
	setDepthStencilTarget(_depthStencilTarget);
	LOG_TRACE_VARG("++++++++Reload FBO:%s    %d    %d", _id.c_str(), _width, _height);
}

void C3DFrameBuffer::setRenderTarget(C3DRenderTarget* target)
{    
	if (target)
	{
		target->retain();

		SAFE_RELEASE(_renderTarget);

		_renderTarget = target;

		// Store the current FBO binding so we can restore it
		GLint currentFbo;
		GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo) );

		// Now set this target as the color attachment.
		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );
		GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTarget->getTexture()->getHandle(), 0) );
		GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_ERROR_VARG("Framebuffer status incomplete: 0x%x", fboStatus);
		}

		// Restore the FBO binding
		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, currentFbo) );
	}
}

C3DRenderTarget* C3DFrameBuffer::getRenderTarget() const
{
	return _renderTarget;
}

void C3DFrameBuffer::setDepthStencilTarget(C3DDepthStencilTarget* target)
{
	if (target)
	{
		target->retain();

		// Release our existing depth stencil target
		SAFE_RELEASE(_depthStencilTarget);

		_depthStencilTarget = target;

		// Store the current FBO binding so we can restore it
		GLint currentFbo;
		GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo) );

		// Now set this target as the color attachment.
		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );

		// Bind the depth texture
		GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthStencilTarget->getTexture()->getHandle(), 0) );

		// If the taget has a stencil buffer, bind that as well
		if (target->getFormat() == C3DDepthStencilTarget::DEPTH24_STENCIL8)
		{
			GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTarget->_stencilBuffer) );
		}

		// Check the framebuffer is good to go.
		GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			//LOG_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
		}

		// Restore the FBO binding
		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, currentFbo) );
	}
}

C3DDepthStencilTarget* C3DFrameBuffer::getDepthStencilTarget() const
{
    return _depthStencilTarget;
}

void C3DFrameBuffer::bind()
{
    CCAssert(!_isBind, "Already bind framebuffer");

    GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_oldFBO));

    // Bind this C3DFrameBuffer for rendering.
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _handle) );

    _oldViewport = *C3DRenderSystem::getInstance()->getViewport();
    C3DRenderSystem::getInstance()->setViewport(0, 0, _width, _height);

    _isBind = true;
}

void C3DFrameBuffer::bindDefault()
{
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
    cocos2d::CCDirector::getInstance()->setViewport();

    std::vector<C3DFrameBuffer*>::const_iterator it;
    //for (it = __frameBuffers.begin(); it < __frameBuffers.end(); it++)
    {
        C3DFrameBuffer* fb = *it;
        fb->_isBind = false;
        fb->_oldFBO = 0;
    }
}

void C3DFrameBuffer::unbind()
{
    CCAssert(_isBind, "frame buffer is not bind");
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _oldFBO) );
    C3DRenderSystem::getInstance()->setViewport(&_oldViewport);
    _isBind = false;
}

//-----------------------------------------------------------------------------------
static C3DFrameBufferMgr* __3DFrameBufferMgr = NULL;

C3DFrameBufferMgr::C3DFrameBufferMgr()
{
}

C3DFrameBufferMgr::~C3DFrameBufferMgr()
{
	LOG_TRACE_VARG("@C3DFrameBufferMgr::%d", _frameBufs.size());

	__3DFrameBufferMgr = NULL;
}

C3DFrameBufferMgr* C3DFrameBufferMgr::getInstance()
{
	if (NULL == __3DFrameBufferMgr)
	{
		__3DFrameBufferMgr = new C3DFrameBufferMgr();
		__3DFrameBufferMgr->autorelease();
	}
	return __3DFrameBufferMgr;
}

void C3DFrameBufferMgr::add(C3DFrameBuffer* texture)
{
	T_CACHE_CONTAINER::iterator itr = std::find(_frameBufs.begin(), _frameBufs.end(), texture);
	if (itr == _frameBufs.end())
	{
		_frameBufs.push_back(texture);
	}
}

void C3DFrameBufferMgr::remove(C3DFrameBuffer* texture)
{
	T_CACHE_CONTAINER::iterator itr = std::find(_frameBufs.begin(), _frameBufs.end(), texture);
	if (itr != _frameBufs.end())
	{
		_frameBufs.erase(itr);
	}
}

C3DFrameBuffer* C3DFrameBufferMgr::get(const std::string& strID)
{
	for(T_CACHE_CONTAINER::iterator iter = _frameBufs.begin(); iter!=_frameBufs.end(); ++iter)
	{
		if((*iter)->getID() == strID)
			return *iter;
	}

	return NULL;
}

void C3DFrameBufferMgr::reload()
{
	for(T_CACHE_CONTAINER::iterator iter = _frameBufs.begin(); iter!=_frameBufs.end(); ++iter)
	{
		(*iter)->reload();
	}
}

}