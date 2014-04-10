#include "Base.h"
#include "C3DMaterial.h"
#include "C3DElementNode.h"
#include "C3DMaterialManager.h"
#include "C3DElementNode.h"

namespace cocos3d
{
static C3DMaterialManager* __materialManagerInstance = NULL;

C3DMaterialManager::C3DMaterialManager()
{
	LOG_TRACE_VARG("@C3DMaterialManager born at: %p", this);
}

C3DMaterialManager::~C3DMaterialManager()
{
	LOG_TRACE_VARG("@C3DMaterialManager dead at: %p", this);
	__materialManagerInstance = NULL;
}

C3DMaterialManager* C3DMaterialManager::getInstance()
{
	if (!__materialManagerInstance)
    {
        __materialManagerInstance = new C3DMaterialManager();
        __materialManagerInstance->autorelease();
    }
    return __materialManagerInstance;
}

C3DResource* C3DMaterialManager::createResource(const std::string& name)
{
    // Load the material properties from file
	C3DElementNode* nodes = C3DElementNode::create(name);
    //assert(nodes);
    if (nodes == NULL)
    {
        return NULL;
    }

    C3DElementNode* materialNodes = nodes->getNodeType().empty() ? nodes->getNextChild() : nodes;
	assert(materialNodes);
    if (!materialNodes || materialNodes->getNodeType()!="material")
    {
        return NULL;
    }

	C3DMaterial* material = new C3DMaterial(name);

	material->autorelease();

	if(material->load(materialNodes) == true)
	{
		this->setResourceState(material, C3DResource::State_Used);
	}
	else
	{
		SAFE_RELEASE(material);
		SAFE_DELETE(nodes);
		return NULL;
	}

    SAFE_DELETE(nodes);

	return material;
}

C3DResource* C3DMaterialManager::cloneResource(C3DResource* resource)
{
	if(resource != NULL)
    {
		C3DResource* newResource = resource->clone();

		this->setResourceState(newResource,C3DResource::State_Used);
		return newResource;
    }
	return NULL;
}
}