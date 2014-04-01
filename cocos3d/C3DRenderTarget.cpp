#include "Base.h"
#include "C3DRenderTarget.h"
#include "C3DTexture.h"

namespace cocos3d
{
static std::vector<C3DRenderTarget*> __renderTargets;

C3DRenderTarget::C3DRenderTarget(const std::string& id)
    : _id(id)
{
	C3DTextureMgr::getInstance()->add(this);
}

C3DRenderTarget::~C3DRenderTarget()
{
    // Remove ourself from the cache
    std::vector<C3DRenderTarget*>::iterator it = std::find(__renderTargets.begin(), __renderTargets.end(), this);
    if (it != __renderTargets.end())
    {
        __renderTargets.erase(it);
    }
}

C3DRenderTarget* C3DRenderTarget::create(const std::string& id, unsigned int width, unsigned int height, unsigned int fmt)
{
    // Create a new texture with the given width
    C3DRenderTarget* renderTarget = new C3DRenderTarget(id);
    renderTarget->init(width, height, C3DTexture::Format(fmt), false);

    __renderTargets.push_back(renderTarget);

    renderTarget->autorelease();
    return renderTarget;
}

C3DRenderTarget* C3DRenderTarget::getRenderTarget(const std::string& id)
{
    // Search the vector for a matching ID.
    std::vector<C3DRenderTarget*>::const_iterator it;
    for (it = __renderTargets.begin(); it < __renderTargets.end(); it++)
    {
        C3DRenderTarget* dst = *it;
        if (id == dst->getID())
        {
            return dst;
        }
    }

    return NULL;
}

const std::string& C3DRenderTarget::getID() const
{
    return _id;
}

}