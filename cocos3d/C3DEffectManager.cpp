#include "Base.h"
#include "C3DEffect.h"
#include "C3DElementNode.h"
#include "C3DEffectManager.h"
#include "C3DElementNode.h"
#include "StringTool.h"
#include "Base.h"

#include "C3DResourcePool.h"
namespace cocos3d
{
static C3DEffectManager* __effectManagerInstance = NULL;

C3DEffectManager::C3DEffectManager()
{
}

C3DEffectManager::~C3DEffectManager()
{
	__effectManagerInstance = NULL;
}

C3DEffectManager* C3DEffectManager::getInstance()
{
	if (!__effectManagerInstance)
    {
        __effectManagerInstance = new C3DEffectManager();
        __effectManagerInstance->autorelease();
    }
    return __effectManagerInstance;
}

void C3DEffectManager::preload(const std::string& name)
{
	if(!name.empty())
	{
		C3DElementNode* doc = C3DElementNode::create(name);
		if (!doc)
		{
			LOG_ERROR_VARG("Error loading effect: Could not load file: %s", name.c_str());
			return;
		}

		loadAllEffect(doc->getNodeType().empty() ? doc->getNextChild():doc);
		SAFE_DELETE(doc);
	}
}

void C3DEffectManager::loadAllEffect(C3DElementNode* effectNodes)
{
	if( effectNodes != NULL )
	{
		if (effectNodes->getNodeType() != "Effects")
		{
			LOG_ERROR("Error proLoading Effects: No 'Effects' namespace found");
			return;
		}
		else
		{
			effectNodes->rewind();
			C3DElementNode* effectNode = NULL;

			while ((effectNode = effectNodes->getNextChild()))
			{
				if (effectNode->getNodeType()=="Effect")
				{
					const std::string& vspath = effectNode->getElement("vertexShader");
					assert(!vspath.empty());

					const std::string& fspath = effectNode->getElement("fragmentShader");
					assert(!fspath.empty());
					std::vector<std::string> flags;
					flags.push_back("");
					C3DElementNode* flagsNode = effectNode->getNextChild();
					if (flagsNode != NULL && flagsNode->getNodeType()=="flags")
					{
						flagsNode->rewind();

						std::string defines("");
						std::string val("");
						while (!(defines = flagsNode->getNextElement()).empty())
						{
							val = flagsNode->getElement("");

							std::string flag;
							if (!defines.empty())
							{
								flag = val;
								flag.insert(0, "#define ");
								unsigned int pos;
								while ((pos = flag.find(';')) != std::string::npos)
								{
									flag.replace(pos, 1, "\n#define ");
								}
								flag += "\n";
							}

							flags.push_back(flag);
						}
					}

					C3DElementNode* elementNode = C3DElementNode::createEmptyNode("test", "effect");
					elementNode->setElement("vertexShader",  vspath);
					elementNode->setElement("fragmentShader", fspath);

					std::string define;
					for(std::vector<std::string>::iterator iter = flags.begin();iter!=flags.end();++iter)
					{
						define = *iter;
						if(!define.empty())
							elementNode->setElement("defines", define);

						preload(elementNode);
					}
					SAFE_DELETE(elementNode);
				}
			}
		}
	}
}

C3DResource* C3DEffectManager::createResource(const std::string& name)
{
	std::vector<std::string> a = StringTool::StringSplitByChar(name,';');
	if(a.size() < 2)
		return NULL;

	std::string vshpath = a[0];
	std::string fshpath = a[1];
	std::string define;
	if(a.size()==3)
		define = a[2];

	C3DElementNode* elementNode = C3DElementNode::createEmptyNode("test", "effect");
	if(elementNode != NULL)
	{
		elementNode->setElement("vertexShader", vshpath);
		elementNode->setElement("fragmentShader", fshpath);
		if(!define.empty())
			elementNode->setElement("defines", define);
	}

	C3DResource* effect = new C3DEffect(name);
	effect->autorelease();

	if(effect->load(elementNode) == true)
	{
		this->setResourceState(effect,C3DResource::State_Used);
	}
	effect->retain();

	SAFE_DELETE(elementNode);

	return effect;
}

std::string C3DEffectManager::generateID( std::string& vshPath, std::string& fshPath, std::string& defines )
{
	assert(vshPath.c_str());

	assert(fshPath.c_str());

    std::string define;
	if (defines.size() != 0)
    {
        define = defines;
        define.insert(0, "#define ");
        unsigned int pos;
        while ((pos = define.find(';')) != std::string::npos)
        {
            define.replace(pos, 1, "\n#define ");
        }
        define += "\n";
    }

	std::string uniqueId = vshPath;
	uniqueId += ';';
	uniqueId += fshPath;

	if (define.c_str() != 0)
	{	
		uniqueId += ';';
		uniqueId += define;
	}

	return uniqueId;
}

C3DResource* C3DEffectManager::cloneResource(C3DResource* resource)
{
	if(resource != NULL)
    {
		resource->retain();
		this->setResourceState(resource,C3DResource::State_Used);
		return resource;
    }
	else
		return NULL;
}

void C3DEffectManager::preload(C3DElementNode* node)
{
	const std::string& vshPath = node->getElement("vertexShader");
	const std::string& fshPath = node->getElement("fragmentShader");
	const std::string& defines = node->getElement("defines");

	std::string uniqueId = vshPath;
    uniqueId += ';';
    uniqueId += fshPath;
    
	if (!defines.empty())
    {
		uniqueId += ';';
        uniqueId += defines;
    }

	C3DResource* effect = this->findResource(uniqueId);

	if(effect != NULL)
		return;
	else
	{
		C3DEffect* effect = new C3DEffect(uniqueId);

		effect->autorelease();

		if(effect->load(node) == true)
		{
			this->setResourceState(effect,C3DResource::State_Used);
		}
	}
}
}