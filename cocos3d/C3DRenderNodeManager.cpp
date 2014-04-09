#include "Base.h"
#include "C3DSprite.h"
#include "C3DStaticObj.h"
#include "C3DRenderNodeManager.h"
#include "StringTool.h"
#include "C3DRenderNode.h"

#include "C3DResourceLoader.h"

namespace cocos3d
{
static C3DRenderNodeManager* __spriteManagerInstance = NULL;

// zhukaixy: 这个类叫C3DRenderNodeManager更合适点
C3DRenderNodeManager::C3DRenderNodeManager()
{
}

C3DRenderNodeManager::~C3DRenderNodeManager()
{
	__spriteManagerInstance = NULL;
}

C3DRenderNodeManager* C3DRenderNodeManager::getInstance()
{
	if (!__spriteManagerInstance)
    {
        __spriteManagerInstance = new C3DRenderNodeManager();
        __spriteManagerInstance->autorelease();
    }
    return __spriteManagerInstance;
}

C3DResource* C3DRenderNodeManager::createResource(const std::string& name)
{
	// zhukaixy：这个地方和C3DRenderNode重复了
	C3DResourceLoader* loader = C3DResourceLoader::create(name);
	loader->autorelease();

	if (loader == NULL)
		return NULL;

	C3DRenderNode* renderNode = NULL;
	if(loader->_isSkin == true)
	{
		renderNode = C3DSprite::create(name);
	}
	else
	{
		renderNode = C3DStaticObj::create(name);
	}
	
	if(renderNode->load(loader, true) == true)
	{
		this->setResourceState(renderNode, C3DResource::State_Used);
	}

	return renderNode;
}

void C3DRenderNodeManager::preload( const std::string& name )
{
	C3DResource* sprite = this->findResource(name);

	if(sprite != NULL)
		return;
	else
	{
		//sprite = new C3DSprite(name);
		sprite = new C3DRenderNode(name);
		sprite->autorelease();

		if(sprite->load(name) == true)
		{
			this->setResourceState(sprite,C3DResource::State_Used);
		}
	}
}

C3DResource* C3DRenderNodeManager::cloneResource(C3DResource* sprite)
{
	if(sprite != NULL)
    {
		C3DNode::CloneContext context;
	    context.cloneChildren = true;

		context.cloneMap.clear();
		context.clonedAnim.clear();
		context.clonedMeshSkin.clear();

		context.idSuffix = StringTool::toString( sprite->getCloneNum()+1 );

		C3DRenderNode* renderNode = static_cast<C3DRenderNode*>(sprite);
		if(renderNode)
		{
			C3DRenderNode* newSprite = static_cast<C3DRenderNode*>(renderNode->clone(context));
			this->setResourceState(newSprite, C3DResource::State_Used);
			return newSprite;
		}
	}
	return NULL;
}
}