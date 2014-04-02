#include "Base.h"
#include "C3DRenderTarget.h"
#include "C3DTexture.h"

namespace cocos3d
{
C3DRenderTarget::C3DRenderTarget(const std::string& id)
{
	_path = id;
}

C3DRenderTarget::~C3DRenderTarget()
{
}

C3DRenderTarget* C3DRenderTarget::create(const std::string& id, unsigned int width, unsigned int height, unsigned int fmt)
{
    // Create a new texture with the given width
    C3DRenderTarget* renderTarget = new C3DRenderTarget(id);
    renderTarget->init(width, height, C3DTexture::Format(fmt), false);

    renderTarget->autorelease();
    return renderTarget;
}

C3DRenderTarget* C3DRenderTarget::getRenderTarget(const std::string& id)
{
	return static_cast<C3DRenderTarget*>(C3DTextureMgr::getInstance()->get(id));
}

}