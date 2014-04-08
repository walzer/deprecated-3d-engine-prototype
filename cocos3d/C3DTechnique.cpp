#include "Base.h"
#include "C3DTechnique.h"
#include "C3DMaterial.h"
#include "C3DNode.h"
#include "C3DPass.h"
#include "C3DRenderChannel.h"
#include "C3DLayer.h"
#include "C3DScene.h"
#include "C3DRenderSystem.h"

namespace cocos3d
{
C3DTechnique::C3DTechnique()
    : _id(""), _material(NULL)
{
}

C3DTechnique::C3DTechnique(const std::string& id, C3DMaterial* material)
    : _id(id), _material(material)
{
    assert(material);

    C3DRenderState::_parent = material;
}

C3DTechnique::~C3DTechnique()
{
    // Destroy all the passes.
    for (unsigned int i = 0, count = _passes.size(); i < count; ++i)
    {
        SAFE_RELEASE(_passes[i]);
    }
}

const std::string& C3DTechnique::getId() const
{
    return _id;
}

unsigned int C3DTechnique::getPassCount() const
{
    return _passes.size();
}

C3DPass* C3DTechnique::getPass(unsigned int index) const
{
    assert(index < _passes.size());

    return _passes[index];
}

C3DPass* C3DTechnique::getPass(const std::string& id) const
{
    for (unsigned int i = 0, count = _passes.size(); i < count; ++i)
    {
        C3DPass* pass = _passes[i];
        if (pass->getId()== id)
        {
            return pass;
        }
    }

    return NULL;
}

void C3DTechnique::setChannel(const std::string& channelName)
{
	if(!channelName.empty())
	{
		_renderChannelName = channelName;
	}

	/**
	//_renderChannel = C3DRenderSystem::getInstance()->getRenderChannelManager()->getRenderChannel(channelName);
	if ( _renderChannel != NULL )
	{
		_renderChannelName = channelName;
	}
	else
	{
		_renderChannelName = std::string();
		WARN_VARG( "C3DTechnique::setChannel can not find channel named %s, channel will set NULL", channelName.c_str() );
	}
	//*/
}

void passReload(C3DPass* pass)
{
	pass->reload();
}

void C3DTechnique::reload()
{
	LOG_TRACE_VARG("   C3DTechnique:%s begin reload", _id.c_str());
	C3DRenderState::reload();
	for_each(_passes.begin(), _passes.end(), passReload);
}

C3DRenderChannel* C3DTechnique::getChannel()
{
	return C3DRenderSystem::getInstance()->getRenderChannelManager()->getRenderChannel(_renderChannelName);
}

C3DTechnique* C3DTechnique::clone() const
{
	C3DTechnique* other = new C3DTechnique(_id, _material);
	other->copyFrom(this);

	for (size_t i = 0; i < _passes.size(); i++)
	{
		other->_passes.push_back(_passes[i]->clone());
		other->_passes[i]->retain();
		other->_passes[i]->_parent = other;
		other->_passes[i]->_technique = other;
	}

	if (_renderChannelName != "")
	{
		other->setChannel(_renderChannelName);
	}
	other->autorelease();
	return other;
}

bool C3DTechnique::load(C3DElementNode* techniqueNodes)
{
    // Create a new technique
  //  C3DTechnique* technique = new C3DTechnique(techniqueNodes->getNodeName(), material);
	this->setChannel( techniqueNodes->getElementFlag("channel") );
	this->_id = techniqueNodes->getNodeName();

    // Go through all the properties and create passes under this technique.
    techniqueNodes->rewind();
    C3DElementNode* passNodes = NULL;
    while ((passNodes = techniqueNodes->getNextChild()))
    {
        if (passNodes->getNodeType()=="pass")
        {
			C3DPass* pass = new C3DPass();
            // Create and load passes.
            if (!pass->load(passNodes))
            {
                SAFE_RELEASE(pass);
                return false;
            }
			else
			{
				pass->_parent = this;
				pass->_technique = this;
				this->_passes.push_back(pass);
			}
        }
    }

    // Load uniform value parameters for this technique
    C3DRenderState::load(techniqueNodes);

    // Add the new technique to the material
  //  material->_techniques.push_back(technique);

    return true;
}

bool C3DTechnique::save(C3DElementNode* node)
{
   // C3DElementNode* node = C3DElementNode::createEmptyNode(this->getId(), "technique");

    //C3DRenderChannel* channel = this->getChannel();
    //if (channel)
    //{
    //    node->setElementFlag("channel", channel->getName());
    //}
	if ( !_renderChannelName.empty()  )
    {
        node->setElementFlag("channel", _renderChannelName);
    }

    // Load uniform value parameters for this technique
    C3DRenderState::save(node);

    for (size_t i = 0; i < this->_passes.size(); i++)
    {
        C3DPass* pass = this->_passes[i];
		C3DElementNode* passNode = C3DElementNode::createEmptyNode(pass->getId(), "pass");
      //  C3DElementNode* passnode = savePass(pass);
		if (pass->save(passNode) == true)
        {
            node->addChildNode(passNode);
        }
		else
		{
			SAFE_DELETE(passNode);
			return false;
		}
    }

    return true;
}

}