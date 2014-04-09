#ifndef SPRITEMANAGER_H_
#define SPRITEMANAGER_H_
#include "Base.h"

#include "C3DResourceManager.h"

namespace cocos3d
{
class C3DElementNode;
class C3DEffect;

/**
 *A materialManager manager material's load£¬preload, and so on.
 */
class C3DRenderNodeManager : public C3DResourceManager
{
public:

	static C3DRenderNodeManager* getInstance();

	virtual C3DResource* createResource(const std::string& name);

	virtual C3DResource* cloneResource(C3DResource* sprite);

	virtual void preload( const std::string& name );

private:

    C3DRenderNodeManager();

    virtual ~C3DRenderNodeManager();

};
}

#endif
