#include "C3DDepthStencilTarget.h"
#include "C3DTexture.h"

namespace cocos3d
{
C3DDepthStencilTarget::C3DDepthStencilTarget(const std::string& id, Format format)
    :_format(format), _stencilBuffer(0)
{
	_path = id;
}

C3DDepthStencilTarget::~C3DDepthStencilTarget()
{
    // Destroy GL resources.
    if (_stencilBuffer)
    {
        GL_ASSERT( glDeleteTextures(1, &_stencilBuffer) );
    }

}

C3DDepthStencilTarget* C3DDepthStencilTarget::create(const std::string& id, Format format, unsigned int width, unsigned int height)
{
    // Create stencil renderbuffer if format is DEPTH24_STENCIL8
    RenderBufferHandle stencilBuffer = 0;
    if (format == DEPTH24_STENCIL8)
    {
        // Backup the existing render buffer
        GLint currentRbo = 0;
        GL_ASSERT( glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRbo) );

        // Create the new render buffer
        GL_ASSERT( glGenRenderbuffers(1, &stencilBuffer) );
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, stencilBuffer) );
        GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height) );

        // Restore the old render buffer
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, currentRbo) );
    }

    // Create the depth stencil target
    C3DDepthStencilTarget* depthStencilTarget = new C3DDepthStencilTarget(id, format);
    depthStencilTarget->init(width, height, C3DTexture::DEPTH, false);;
    depthStencilTarget->_stencilBuffer = stencilBuffer;

    depthStencilTarget->autorelease();
    return depthStencilTarget;
}

C3DDepthStencilTarget* C3DDepthStencilTarget::getDepthStencilTarget(const std::string& id)
{
	return static_cast<C3DDepthStencilTarget*>(C3DTextureMgr::getInstance()->get(id));
}

C3DDepthStencilTarget::Format C3DDepthStencilTarget::getFormat() const
{
    return _format;
}

void C3DDepthStencilTarget::reload()
{
	C3DTexture::reload();

	RenderBufferHandle stencilBuffer = 0;
	if (_format == DEPTH24_STENCIL8)
	{
		// Backup the existing render buffer
		GLint currentRbo = 0;
		GL_ASSERT( glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRbo) );

		// Create the new render buffer
		GL_ASSERT( glGenRenderbuffers(1, &stencilBuffer) );
		GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, stencilBuffer) );
		GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, _width, _height) );

		// Restore the old render buffer
		GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, currentRbo) );
	}

	_stencilBuffer = stencilBuffer;
}

}